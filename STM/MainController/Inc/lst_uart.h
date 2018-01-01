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
#define LST_UART1_RX_BUFFER_SIZE 1
#define LST_UART2_RX_BUFFER_SIZE 1
#define LST_UART2_TX_BUFFER_SIZE 256

#define LST_UART_TX_NOTCPLT 0
#define LST_UART_TX_CPLT    1

/* Function prototypes -------------------------------------------------------*/
void LST_UART_Init();
void LST_UART_BT_Send_Bytes(uint8_t data_bytes);
extern void LST_BT_Process_Incoming_Byte();
extern void LST_Radio_Process_Incoming_Byte();
void LST_UART_Receive_Byte_UART1();
void LST_UART_Receive_Byte_UART2();

#ifdef LST_CONFIG_UART_LINE_COM
void LST_UART_ReceiveLineControllerData();
void LST_UART_WaitForLineControllerData();
#endif

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint8_t lst_uart_buffer_uart1[LST_UART1_RX_BUFFER_SIZE];
extern uint8_t lst_uart_buffer_uart2[LST_UART2_RX_BUFFER_SIZE];
extern uint8_t lst_uart_buffer_tx[LST_UART2_TX_BUFFER_SIZE];

extern uint8_t lst_uart_uart1_txcplt;
extern uint8_t lst_uart_uart2_txcplt;

extern HAL_StatusTypeDef lst_uart_uart1_rx_status;
extern HAL_StatusTypeDef lst_uart_uart2_rx_status;

#endif /* LST_UART_H_ */
