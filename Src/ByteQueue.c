/**
 *	\file
 *	\author	<a href="http://www.innomatic.ca">innomatic</a>
 * 	\copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 */
#include "ByteQueue.h"
#include "board.h"

#define ADVANCE_QPTR(x)     ((x+1) % BYTEQUEUE_DEPTH)
#ifndef BQUEUE_ENTER_CS
#define BQUEUE_ENTER_CS		{}
#define BQUEUE_EXIT_CS		{}
#endif

static struct
{
	uint8_t buff[BYTEQUEUE_DEPTH];
	uint8_t head;
	uint8_t tail;
} queue;


/**
 *	\param byte data to put
 *	\return true if successful
 */
bool ByteQueue_Put(uint8_t byte)
{
	uint8_t next;
	bool flag = true;
	

	// enter critical section
	BQUEUE_ENTER_CS;

	// get next head position
	next = ADVANCE_QPTR(queue.head);

	// queue is full
	if(next == queue.tail)
	{
		// failed to put data
		flag = false;
	}
	else if(next != queue.tail)
	{
		// copy data into the buffer
		queue.buff[queue.head] = byte;
		// move head position to the next
		queue.head = next;
	}

	// exit critical section
	BQUEUE_EXIT_CS;

	return flag;
}

/**
 *	\param byte storage to get the data, 0xff will be retruned if failed
 *	\return true if successful
 */
bool ByteQueue_Get(uint8_t *byte)
{
	bool flag = false;

	// enter critical section
	BQUEUE_ENTER_CS;

	// queue is not empty
	if(queue.tail != queue.head)
	{
		// retrieve data from the queue
		*byte = queue.buff[queue.tail];
		// move the tail position to the next
		queue.tail = ADVANCE_QPTR(queue.tail);
		//
		flag = true;
	}
	else
	{
		*byte = 0xff;
		flag = false;
	}

	// exit critical section
	BQUEUE_EXIT_CS;

	return flag;
}

void ByteQueue_Init(void)
{
	// clear queue by resetting the pointers
	queue.head = queue.tail = 0;
}


#if UNIT_TEST
/**
 */
void ByteQueue_UnitTest(void)
{
	unsigned i;
	uint8_t byte;
	bool flag = true;
	uint8_t data[BYTEQUEUE_DEPTH + 1];

	DbgPrintf("\r\n\r\n============== ByteQueue Unit Test ===================");

	// fill data
	for(i = 0; i < sizeof(data); i++)
	{
		data[i] = i;
	}

	// ByteQueue_Init
	ByteQueue_Init();

	// ByteQueue_Put
	DbgPrintf("\r\nByteQueue_Put:");

	for(i = 0; i < sizeof(data); i++)
	{
		if(ByteQueue_Put(data[i]))
		{
			DbgPrintf("\r\n\t%02d: ByteQueue_Put: %02x", i, data[i]);
		}
		else
		{
			DbgPrintf("\r\n\t%02d: <ERR> queue is full: %02x", i, data[i]);
		}
	}
	// ByteQueue_Get
	DbgPrintf("\r\nByteQueue_Get:");
	flag = true;
	i = 0;

	while(flag)
	{
		if((flag = ByteQueue_Get(&byte)))
		{
			DbgPrintf("\r\n\t(%02d): %02x", i, byte);
			i++;
		}
		else
		{
			DbgPrintf("\r\n\t(%02d): <ERR> queue is empty: %02x", i, byte);
		}
	}

	// ByteQueue_Put & ByteQueue_Get
	DbgPrintf("\r\nByteQueue_Put - ByteQueue_Get");

	for(i = 0; i < BYTEQUEUE_DEPTH * 2; i++)
	{
		byte = 0x80 + i;
		ByteQueue_Put(byte);
		DbgPrintf("\r\n\tByteQueue_Put: %02x", byte);
		ByteQueue_Get(&byte);
		DbgPrintf("\tByteQueue_Get: %02x",byte);
	}
}

#else
void ByteQueue_UnitTest(void)
{
}
#endif
