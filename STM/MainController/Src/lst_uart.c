/*
 * lst_uart.c
 *
 *  Created on: 2017. okt. 26.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_uart.h"

/* Private variables ---------------------------------------------------------*/
// ToDo temp
uint8_t lst_uart_uart1_rxcplt = 1;
extern uint8_t lst_spi_master1_rx[38];

uint8_t lst_uart_uart1_txcplt = LST_UART_TX_CPLT;
uint8_t lst_uart_uart2_txcplt = LST_UART_TX_CPLT;
HAL_StatusTypeDef lst_uart_uart1_rx_status = HAL_OK;
HAL_StatusTypeDef lst_uart_uart2_rx_status = HAL_OK;
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
  // LST_UART_Receive_Byte_UART1();
  
  /* Receive a byte on UART2 */
  LST_UART_Receive_Byte_UART2();
}

/**
 * @brief Receives a byte from UART1
 */
void LST_UART_Receive_Byte_UART1(){
  /* Receive a byte on UART1 */
  lst_uart_uart1_rx_status =
    HAL_UART_Receive_IT(&huart1, (uint8_t *) &lst_uart_buffer_uart1[0], 1);
}

/**
 * @brief Receives a byte from UART2
 */
void LST_UART_Receive_Byte_UART2(){
  /* Receive a byte on UART2 */
  lst_uart_uart2_rx_status =
    HAL_UART_Receive_IT(&huart2, (uint8_t *) &lst_uart_buffer_uart2[0], 1);
}

/**
 * @brief This function handles the UART receive complete callback.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    /* Receive another byte */
    LST_UART_Receive_Byte_UART2();
    LST_BT_Process_Incoming_Byte();  // This function needs to be fast
  }

  else if (huart->Instance == USART1) {
    // ToDo temp
    lst_uart_uart1_rxcplt = 1;
    /* Receive another byte */
    // LST_UART_Receive_Byte_UART1();
    // LST_Radio_Process_Incoming_Byte();  // This function needs to be fast
  }
}

/**
 * @brief This function handles the UART transmit complete callback.
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
  /* Data byes below 2 are errors */
  if(data_bytes < 2) return;

  /* Wait until UART2 is ready */
  while (huart2.gState != HAL_UART_STATE_READY) {
  }
  
  /* Swap 0xFF for 0xFE */
  uint8_t i = 0;
  for (i = 1; i < data_bytes - 1; i++) {
    if (lst_uart_buffer_tx[i] == 0xFF) {
      lst_uart_buffer_tx[i] = 0xFE;
    }
  }

  /* Send UART message */
  HAL_UART_Transmit_DMA(&huart2, (uint8_t *) &lst_uart_buffer_tx, data_bytes);
}

/* ToDo temp */
void LST_UART_ReceiveLineControllerData(){
  /* Wait for UART1 to be ready */
  while (huart1.gState != HAL_UART_STATE_READY) {}

  /* Check LineCntrl GPIO on PB9 */
  //GPIO_PinState state = HAL_GPIO_ReadPin(SPI1_DATA_READY_GPIO_Port, SPI1_DATA_READY_Pin);
  //if(state==GPIO_PIN_RESET) return;

  lst_uart_uart1_rxcplt = 0;

  /* receive data via UART1 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *) &lst_spi_master1_rx, 38);

  /* Manual Slave Select -> Low */
  HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_RESET);

}

void LST_UART_WaitForLineControllerData(){
  while (lst_uart_uart1_rxcplt != 1) {}
  /* Manual Slave Select -> High */
  HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_SET);
}
