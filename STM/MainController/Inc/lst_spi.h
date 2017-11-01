/*
 * lst_spi.h
 *
 *  Created on: 2017. nov. 1.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_SPI_H_
#define LST_SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

extern uint8_t spi_master_tx[8];
extern uint8_t spi_master_rx[8];
extern uint8_t spi_slave_tx[8];
extern uint8_t spi_slave_rx[8];

#endif /* LST_SPI_H_ */
