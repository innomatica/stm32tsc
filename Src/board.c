#include <stdarg.h>
#include "board.h"

extern UART_HandleTypeDef huart2;
/** Formatted string output to USB_CDC
 *
 *	\param printf() like parameters
#include "usbd_cdc.h"
extern USBD_HandleTypeDef hUsbDeviceFS;
void USB_Printf(const char* format,...)
{
	char buffer[256];
	int length;
	va_list args;
	va_start(args, format);

	length = vsprintf(buffer, format, args);
	if(length)
		USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t*)buffer, length);

	va_end(args);
}
 */

/** Formatted string ouput to UART
 *
 *	\param printf() like parameters
 */
void UART_Printf(const char* format,...)
{
	char buffer[256];
	int length;
	va_list args;
	va_start(args, format);

	length = vsprintf(buffer, format, args);
	if(length)
		HAL_UART_Transmit(&huart2, (uint8_t*)buffer, length, 1000);

	va_end(args);
}


/** Microsecond delay
 *
 * \warning This is subject to error since the delay relies on execution of
 *		nop().
 */
void USecDelay(unsigned usec)
{
	while(usec-- > 0)
	{
		// approximately 1 usec delay in 32MHz clock
		asm(
		    "nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"
		    "nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"
		    "nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"
		);
	}
}

uint8_t PushButton_Read(void)
{
	return BTN_READ;
}

void SerialComm_Init(void)
{
	// clear USART interrupt
	HAL_NVIC_ClearPendingIRQ(USART2_IRQn);
	// enable RX interrupt
	USART2_ENABLE_RX_IT;
}

void SerialComm_SendByteArray(uint8_t *buffer, int size)
{
	// note that HAL_UART_TransmitIT no longer works
	HAL_UART_Transmit(&huart2, buffer, size, 1000);
}
