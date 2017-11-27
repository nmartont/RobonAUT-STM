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
uint8_t lst_uart_uart1_txcplt = LST_UART_TX_CPLT;
uint8_t lst_uart_uart2_txcplt = LST_UART_TX_CPLT;
uint8_t lst_uart_buffer_uart1[LST_UART1_RX_BUFFER_SIZE] = { 0x00 };
uint8_t lst_uart_buffer_uart2[LST_UART2_RX_BUFFER_SIZE] = { 0x00 };
uint8_t lst_uart_buffer_tx[LST_UART2_TX_BUFFER_SIZE] = { 0x00 };

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            UART Communication handling for RobonAUT 2018 Team LST          */
/******************************************************************************/

/* Initialization function -------------------------------------------------- */
/**
 * @brief Initializes the UART part of the software
 */
void LST_UART_Init() {
  /* Receive a byte on UART1 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *) &lst_uart_buffer_uart1, 1);
  
  /* Receive a byte on UART2 */
  HAL_UART_Receive_IT(&huart2, (uint8_t *) &lst_uart_buffer_uart2, 1);
}

/**
 * @brief This function handles the UART receive complete callback.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    /* Receive another byte */
    HAL_UART_Receive_IT(&huart2, (uint8_t *) &lst_uart_buffer_uart2[0], 1);
    LST_BT_Process_Incoming_Byte();  // This function needs to be fast
  }

  else if (huart->Instance == USART1) {
    /* Receive another byte */
    HAL_UART_Receive_IT(&huart1, (uint8_t *) &lst_uart_buffer_uart1[0], 1);
    LST_Radio_Process_Incoming_Byte();  // This function needs to be fast
  }
}

/**
 * @brief This function handles the UART receive complete callback.
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    lst_uart_uart2_txcplt = 1;
  }

  else if (huart->Instance == USART1) {
    lst_uart_uart1_txcplt = 1;
  }
}

/**
 * @brief Sends an UART BT message
 */
void LST_UART_BT_Send_Bytes(uint8_t data_bytes) {
  /* Wait until UART2 is ready */
  while (huart2.gState != HAL_UART_STATE_READY) {
  }
  
  /* Send UART message */
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *) &lst_uart_buffer_tx, data_bytes);
}
