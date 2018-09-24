/**
 *	\file
 *	\author	<a href="http://www.innomatic.ca">innomatic</a>
 *	\brief	FIFO queue holds bytes as its data
 * 	\copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 */
#ifndef __BYTE_QUEUE_H
#define __BYTE_QUEUE_H

#include <stdbool.h>
#include <stdint.h>

#define BYTEQUEUE_DEPTH				(20)

/// Enqueue a byte date
bool ByteQueue_Put(uint8_t byte);
/// Dequeue a byte data
bool ByteQueue_Get(uint8_t *byte);
/// Initialize the queue
void ByteQueue_Init(void);
/// Unit test
void ByteQueue_UnitTest(void);

#endif // __BYTE_QUEUE_H
