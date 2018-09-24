/**
 *	\file
 *	\author	<a href="http://www.innomatic.ca">innomatic</a>
 *	\brief	Serial communication protocol handler
 *	\copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 *
 *	Code Example
 *
\code

// override SerialCom_Init
void SerialCom_Init()
{
	// clear UART interrupt flag
	// enable RX interrupt
	...
}

// override SerialCom_SendByteArray
void SerialCom_SendByteArray(uint8_t *buffer, int size)
{
	// send data in the buffer, preferably non-blocking fashion
}

// call data collector from the the UART RX interrupt callback
void UART_RX_Interrupt_Callback(void)
{
	// call SerialCom_RxRoutine
	SerialCom_RxRoutine(UART_Get_Byte());
}

// in the main loop
uint8_t new_byte = 0;

int main(void)
{
	...
	while(true)
	{
		// pop any new byte from the queue
		if(ByteQueue.Get(&new_byte))
		{
			// call packet decoder
			if(PKT_RECEIVED == SerialCom_Decoder(new_byte, buffer))
			{
				// packet data is in the buffer
			}
		}
	}
	...
}
\endcode
 *
 *	Code Example (STM32Cube case)
 *
\code
// override SerialCom_Init
void SerialCom_Init()
{
	// clear USART interrupt
	HAL_NVIC_ClearPendingIRQ(USART1_IRQn)
	// enable RX interrupt
	LL_USART_EnableIT_RXNE(USART1);
}

// override SerialCom_SendByteArray
void SerialCom_SendByteArray(uint8_t *buffer, int size)
{
	// note that HAL_UART_TransmitIT no longer works
	HAL_UART_Transmit(&huart1, buffer, size, 1000);
}

// intercept HAL USART IRQ handler in stm32xxxx_it.c
void USART1_IRQHandler(void)
{
	if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
	{
		SerialCom_RxRoutine(USART_GET_DATA);
	}
#if 0
    // Warning: this makes some HAL UART functions unusable
	HAL_UART_IRQHandler(&huart1);
#endif
}
\endcode
 */

#ifndef __SERIAL_COMM_H
#define __SERIAL_COMM_H

#include <stdint.h>

#define MAX_PAYLOAD			10					///< max size of the payload
#define MAX_PKTSIZE			(MAX_PAYLOAD + 3)   ///< max size of the packet
#define MAX_DATSIZE			(MAX_PAYLOAD - 1)   ///< max data bytes

#define PKT_HEADR			0xf5				///< packet header signature
#define PKT_ACK				0xf6				///< ACK packet
#define PKT_NAK				0xf7				///< NAK packet
#define PKT_IAM				0xf8                ///< IAM packet

/// Packet state machine return value
typedef enum
{
	PKT_INPROCES = 0,		///< packet decoding in process
	PKT_RECEIVED,			///< valid packet received
	ACK_RECEIVED,			///< ACK packet received
	NAK_RECEIVED,			///< NAK packet received
	IAM_RECEIVED,           ///< IAM packet received
	PKT_SIZE_ERR,           ///< packet size error detected
	PKT_CSUM_ERR			///< checksum error detected
} pkt_status;

/// Send a packet
void SerialCom_SendPacket(uint8_t *payload, int size);
/// Packet decoding state machine
pkt_status SerialCom_Decoder(uint8_t byte, uint8_t *buffer);
/// Initialize the module module: should be overridden
void SerialCom_Init(void) __attribute((weak));
/// Send a stream of bytes: should be overridden
void SerialCom_SendByteArray(uint8_t *buffer, int size) __attribute((weak));
/// Incoming data collector
void SerialCom_RxRoutine(uint8_t byte);
/// Receiver test
void SerialCom_RcvTest(void);

#endif // __SERIAL_COMM_H
