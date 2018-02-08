/*
 * lst_spi.c
 *
 *  Created on: 2017. okt. 26.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_spi.h"

/* Private variables ---------------------------------------------------------*/
uint8_t lst_spi_master1_tx[LST_SPI_BUFFER1_SIZE] = { 0x00 };
uint8_t lst_spi_master1_rx[LST_SPI_BUFFER1_SIZE] = { 0x00 };
uint8_t lst_spi_master3_tx[LST_SPI_BUFFER3_SIZE] = { 0x00 };
uint8_t lst_spi_master3_rx[LST_SPI_BUFFER3_SIZE] = { 0x00 };

uint8_t lst_spi_master1_txrx_cmplt = LST_SPI_TXRX_NOT_COMPLETE;

uint8_t lst_spi_linecontroller_lost = 0;

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*             SPI Communication handling for RobonAUT 2018 Team LST          */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the SPI part of the software
 */
void LST_SPI_Init() {
  
}

/**
 * @brief This function handles the SPI receive complete callback.
 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
  
}

/**
 * @brief This function handles the SPI Transfer/Receive complete callback.
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {

  if (hspi->Instance == SPI1)
  {
    /* Flag to signal completion of SPI1 TxRx complete */
    lst_spi_master1_txrx_cmplt = LST_SPI_TXRX_COMPLETE;
    /* Manual Slave Select -> High */
    HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_SET);
  }
}

/**
 * @brief This function gets the line data from the LineController
 */
void LST_SPI_ReceiveLineControllerData(){
  /* Wait for SPI to be ready */
  while (hspi1.State != HAL_SPI_STATE_READY) {
  }

  /* Check LineCntrl GPIO on PB9 */
  GPIO_PinState state = HAL_GPIO_ReadPin(SPI1_DATA_READY_GPIO_Port, SPI1_DATA_READY_Pin);
  if(state==GPIO_PIN_RESET) return;

  // Breakpoint here -> SPI reset works TODO
  //vTaskDelay(1);

  /* Put SPI command into the first byte */
  lst_spi_master1_tx[0] = LST_SPI_MODE_DEBUG;

  /* Manual Slave Select -> Low */
  HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, GPIO_PIN_RESET);

  /* 1000 ticks delay for the SlaveSelect */
  volatile uint16_t vol_cntr = 0;
  for(vol_cntr=0; vol_cntr<1000; ){
    vol_cntr++;
  }

  /* Reset TxRx complete flag */
  lst_spi_master1_txrx_cmplt = LST_SPI_TXRX_NOT_COMPLETE;

  /* Send and receive data via SPI3 */
  HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *) &lst_spi_master1_tx,
    (uint8_t *) &lst_spi_master1_rx, LST_SPI_BUFFER1_SIZE);
}

/**
 * @brief This function waits for data from the LineController
 */
void LST_SPI_WaitForLineControllerData(){
  while (hspi1.State != HAL_SPI_STATE_READY &&
      lst_spi_master1_txrx_cmplt != LST_SPI_TXRX_COMPLETE) {
  }
}
