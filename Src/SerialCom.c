/**
 * \file
 * \author	<a href="http://www.innomatic.ca">innomatic</a>
 * \copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 */
#include <stdbool.h>
#include "board.h"
#include "ByteQueue.h"
#include "SerialCom.h"

// packet decoding state machine states
#define PKT_STATE_HDR			0
#define PKT_STATE_LEN			1
#define PKT_STATE_PLD			2
#define PKT_STATE_CSM			3

#ifndef CLEAR_UART_RX_IT
	#error CLEAR_UART_RX_IT not defined
#endif

/**
 * This function runs packet decoding state machine. It takes stream of serial
 * data one byte at a time and returns the current state of the state machine.
 * Upon successful retrieval of a full packet, it fills the array buffer[]
 * with the received payload.
 *
 * To use this function, define an uint8_t array for the payload data storage
 * and call this function whenever new byte is arrived. The best place to put
 * this function is inside the UART check routine (not inside the interrupt
 * service callback function).
 *
 * \param	byte received byte
 * \param   buffer packet will be retured here
 * \return  pkt_status status of the state machine
 *
\code
uint8_t buffer[MAX_PKTSIZE];
pkt_status pstatus;

if(ByteQueue_Get(&new_byate)
{
    // new data byte is in new_byte
    if(PKT_RECEIVED == SerialCom_Decoder(new_byte, buffer))
    {
        // packet data is in the buffer
    }
}
\endcode
 */
pkt_status SerialCom_Decoder(uint8_t byte, uint8_t *buffer)
{
	static uint8_t state = PKT_STATE_HDR;
	static uint8_t index = 0;
	static uint8_t packet[MAX_PKTSIZE] = {0};
	static uint8_t csum = 0;

	int i;

	// waiting for the header byte
	if(state == PKT_STATE_HDR)
	{
		if(byte == PKT_HEADR)
		{
			// store the header byte
			packet[0] = byte;
			// proceed to the next state
			state = PKT_STATE_LEN;
		}
		else if(byte == PKT_ACK)
		{
			// ACK received but do not change the state
			return ACK_RECEIVED;
		}
		else if(byte == PKT_NAK)
		{
			// NAK received but do not change the state
			return NAK_RECEIVED;
		}
		else if(byte == PKT_IAM)
		{
			// IAM received but do not change the state
			return IAM_RECEIVED;
		}
	}
	// waiting for the length byte
	else if(state == PKT_STATE_LEN)
	{
		// store the length byte
		packet[1] = byte;

		// invalid payload size
		if(byte > MAX_PAYLOAD)
		{
			// start all over
			state = PKT_STATE_HDR;
			// report size error
			return PKT_SIZE_ERR;
		}
		// length byte is valid
		else
		{
			// reset index
			index = 2;
			// clear chesum byte
			csum = 0;
			// proceed to the next state
			state = PKT_STATE_PLD;
		}
	}
	// waiting for the payload
	else if(state == PKT_STATE_PLD)
	{
		// collect data
		packet[index++] = byte;
		// process checksum
		csum ^= byte;
		// proceed to the next if all payload is collected
		if(index == (packet[1] + 2))
		{
			state = PKT_STATE_CSM;
		}
	}
	// waiting for the checksum byte
	else if(state == PKT_STATE_CSM)
	{
		// collect data
		packet[index] = byte;
		// checksum matches
		if(byte == csum)
		{
			// copy packet to the buffer
			for(i = 0; i < packet[1] + 3; i++)
			{
				buffer[i] = packet[i];
			}
			// start all over again
			state = PKT_STATE_HDR;
			// valid packet arrived
			return PKT_RECEIVED;
		}
		// checksum does not match
		else
		{
			// start all over
			state = PKT_STATE_HDR;
			// checksum error
			return PKT_CSUM_ERR;
		}
	}

	return PKT_INPROCES;
}

/** Construct a packet from a give payload and send it via UART
 *
 *	\param payload payload data
 *	\param size size of the payload
 *
\code
uint8_t payload[5];

// pack signed int32 value in big endian order
payload[0] = RPT_S32XXX;
payload[1] = (value >> 24) & 0xff;
payload[2] = (value >> 16) & 0xff;
payload[3] = (value >> 8) & 0xff;
payload[4] = (value) & 0xff;

// send the packet
SerialCom_SendPacket(payload, 5);
\endcode
 */
void SerialCom_SendPacket(uint8_t *payload, int size)
{
	uint8_t csum = 0;
	uint8_t packet[MAX_PKTSIZE];
	int i;

	// header
	packet[0] = PKT_HEADR;
	// payload size
	packet[1] = size;
	// data
	for(i = 0; i < size; i++)
	{
		csum ^= payload[i];
		packet[2+i] = payload[i];
	}
	// checksum
	packet[2 + size] = csum;

	// send the array of bytes
	SerialCom_SendByteArray(packet, size+3);
}

/**	Put this inside of RX callback of the UART.
 *	
 *	\param byte data byte read from UART
 */
inline void SerialCom_RxRoutine(uint8_t byte)
{
	ByteQueue_Put(byte);
	CLEAR_UART_RX_IT;
}


#if UNIT_TEST
/**
 */
#define TO_HEX(a)		(((a) > 9) ? ((a)-10+'A'):((a)+'0'))

/** Call this function inside of main loop. It sends ASCII string via UART,
 *	which represents the state of the decoder in the following way.
 *
 *  xx.xx....xx (P) : packet received without error
 *  (E) : packet received with error
 *  (A) : ACK received
 *  (N) : NAK received
 *  (I) : IAM received
 */
void SerialCom_RcvTest(void)
{
	uint8_t u8val;
	uint8_t rx_buffer[MAX_PKTSIZE];
	static int index = 0;
	static uint8_t dbg_out[200];

	if(ByteQueue_Get(&u8val))
	{
		switch(SerialCom_Decoder(u8val, rx_buffer))
		{
		case PKT_INPROCES:
			dbg_out[index++] = TO_HEX(u8val>>4);
			dbg_out[index++] = TO_HEX(u8val&0x0f);
			dbg_out[index++] = '.';
			break;

		case PKT_RECEIVED:
			dbg_out[index++] = TO_HEX(u8val>>4);
			dbg_out[index++] = TO_HEX(u8val&0x0f);
			dbg_out[index++] = '(';
			dbg_out[index++] = 'P';
			dbg_out[index++] = ')';
			dbg_out[index++] = '\n';
			SerialCom_SendByteArray(dbg_out, index);
			index = 0;
			break;

		case ACK_RECEIVED:
			dbg_out[index++] = TO_HEX(u8val>>4);
			dbg_out[index++] = TO_HEX(u8val&0x0f);
			dbg_out[index++] = '(';
			dbg_out[index++] = 'A';
			dbg_out[index++] = ')';
			dbg_out[index++] = '\n';
			SerialCom_SendByteArray(dbg_out, index);
			index = 0;
			break;

		case NAK_RECEIVED:
			dbg_out[index++] = TO_HEX(u8val>>4);
			dbg_out[index++] = TO_HEX(u8val&0x0f);
			dbg_out[index++] = '(';
			dbg_out[index++] = 'N';
			dbg_out[index++] = ')';
			dbg_out[index++] = '\n';
			SerialCom_SendByteArray(dbg_out, index);
			index = 0;
			break;

		case IAM_RECEIVED:
			dbg_out[index++] = TO_HEX(u8val>>4);
			dbg_out[index++] = TO_HEX(u8val&0x0f);
			dbg_out[index++] = '(';
			dbg_out[index++] = 'I';
			dbg_out[index++] = ')';
			dbg_out[index++] = '\n';
			SerialCom_SendByteArray(dbg_out, index);
			index = 0;
			break;

		case PKT_SIZE_ERR:
		case PKT_CSUM_ERR:
			dbg_out[index++] = TO_HEX(u8val>>4);
			dbg_out[index++] = TO_HEX(u8val&0x0f);
			dbg_out[index++] = '(';
			dbg_out[index++] = 'E';
			dbg_out[index++] = ')';
			dbg_out[index++] = '\n';
			SerialCom_SendByteArray(dbg_out, index);
			index = 0;
			break;

		default:
			break;
		}
	}
}

#else

void SerialCom_RecvTest(void)
{
}

#endif
