/*
 * lst_uart.h
 *
 *  Created on: 2017. nov. 1.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_UART_H_
#define LST_UART_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/
#define UART1_RX_BUFFER_SIZE 128
#define UART2_RX_BUFFER_SIZE 256
#define UART2_TX_BUFFER_SIZE 128

/* Function prototypes -------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint8_t lst_uart_buffer_uart1[UART1_RX_BUFFER_SIZE];
extern uint8_t lst_uart_buffer_uart2[UART2_RX_BUFFER_SIZE];
extern uint8_t lst_uart_buffer_tx[UART2_TX_BUFFER_SIZE];

#endif /* LST_UART_H_ */
