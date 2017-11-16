/*
 * lst_uart.c
 *
 *  Created on: 2017. okt. 26.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_uart.h"

/* Private variables ---------------------------------------------------------*/
uint8_t cntr_uart1 = 0;
uint8_t cntr_uart2 = 0;
uint8_t lst_uart_buffer_uart1[UART1_RX_BUFFER_SIZE] = {0x00};
uint8_t lst_uart_buffer_uart2[UART2_RX_BUFFER_SIZE] = {0x00};
uint8_t lst_uart_buffer_tx[UART2_TX_BUFFER_SIZE]    = {0x01, 0xFF};

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            UART Communication handling for RobonAUT 2018 Team LST          */
/******************************************************************************/

/**
* @brief This function handles the UART receive complete callback.
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2){
		if(cntr_uart2 == UART2_RX_BUFFER_SIZE - 1){
			cntr_uart2 = 0;
		}
		else{
			cntr_uart2++;
		}
		HAL_UART_Receive_IT(&huart2, (uint8_t *)&lst_uart_buffer_uart2[cntr_uart2], 1);
	}

	if (huart->Instance == USART1){
		if(cntr_uart1 == UART1_RX_BUFFER_SIZE - 1){
			cntr_uart1 = 0;
		}
		else{
			cntr_uart1++;
		}
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&lst_uart_buffer_uart1[cntr_uart1], 1);
	}
}
