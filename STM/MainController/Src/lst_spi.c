/*
 * lst_spi.c
 *
 *  Created on: 2017. okt. 26.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_spi.h"

/* Private variables ---------------------------------------------------------*/
uint8_t cntr_spi1 = 0;
uint8_t cntr_spi2 = 0;
uint8_t lst_spi_master_tx[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
uint8_t lst_spi_master_rx[8] = { 0x00 };
uint8_t lst_spi_slave_tx[8] = { 10, 20, 30, 40, 50, 60, 70, 80 };
uint8_t lst_spi_slave_rx[8] = { 0x00 };

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
  if (hspi->Instance == SPI1) {
    /* Get another 8 bytes from Master */
    HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *) &lst_spi_slave_tx,
        (uint8_t *) &lst_spi_slave_rx, 8);
    
    /* Manip data a little bit */
    lst_spi_slave_tx[0] += 1;
    lst_spi_slave_tx[1] += 1;
    lst_spi_slave_tx[2] += 1;
    lst_spi_slave_tx[3] += 1;
    lst_spi_slave_tx[4] += 1;
    lst_spi_slave_tx[5] += 1;
    lst_spi_slave_tx[6] += 1;
    lst_spi_slave_tx[7] += 1;
  }
}
