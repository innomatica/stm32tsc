#ifndef __BOARD_H
#define __BOARD_H
/*
 * \author Sungjune Lee
 * \brief Minimal implementation of nRF24L01P communication.
 * \copyright
 *
 * nRF24L01P data transaction based on the enhanced shockbust protocol.
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include "stm32l0xx_hal.h"

// LED
#define LED_ON					(LED_GPIO_Port->BSRR = LED_Pin)
#define LED_OFF					(LED_GPIO_Port->BRR = LED_Pin)
#define LED_TOGGLE				(LED_GPIO_Port->ODR ^= LED_Pin)
// BTN
#define BTN_READ				(((BTN_GPIO_Port->IDR)&(BTN_Pin))==(BTN_Pin)?(0):(1))
// SysTick interrupt control (LL_SYStICK_EnableIT/LL_SYSTICK_DisableIT)
#define SYSTICK_ENABLE_IT		do{SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);}while(0)
#define SYSTICK_DISABLE_IT		do{CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);}while(0)
// USART interrupt control (LL_USART_EnableIT_RXNE/LL_USART_DisableIT_RXNE);
#define USART2_ENABLE_RX_IT		do{SET_BIT(USART2->CR1, USART_CR1_RXNEIE);}while(0)
#define USART2_DISABLE_RX_IT	do{CLEAR_BIT(USART2->CR1, USART_CR1_RXNEIE);}while(0)
// LL_USART_RequestRxDataFlush
#define USART2_CLEAR_RX_IT		do{SET_BIT(USART2->RQR, USART_RQR_RXFRQ);}while(0)
#define CLEAR_UART_RX_IT		USART2_CLEAR_RX_IT
#define USART2_GET_DATA			((uint8_t) READ_REG(USART2->RDR))

// ByteQueue Critical Section
#define BQUEUE_ENTER_CS			USART2_DISABLE_RX_IT
#define BQUEUE_EXIT_CS			USART2_ENABLE_RX_IT


// debug
#define DEBUG_OUTPUT			(1)
#define UNIT_TEST				(1)

#if DEBUG_OUTPUT
#define DbgPrintf(x,arg...)		UART_Printf(x,##arg)
#else
#define DbgPrintf(x,arg...)		{}
#endif

/// General purpose functions
void USB_Printf(const char* format,...);
void UART_Printf(const char* format,...);
void USecDelay(unsigned usec);

#endif	// __BOARD_H
