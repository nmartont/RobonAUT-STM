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
#ifdef LST_CONFIG_LINECONTROLLER_VERBOSE_DATA
#define LST_SPI_BUFFER1_SIZE  38
#define LST_SPI_MODE_DEBUG    1
#else
#define LST_SPI_BUFFER1_SIZE  6
#define LST_SPI_MODE_DEBUG    0
#endif

#define LST_SPI_BUFFER3_SIZE  8

#define LST_SPI_TXRX_NOT_COMPLETE 1
#define LST_SPI_TXRX_COMPLETE     1

#define LST_SPI_SS_DELAY_TICKS 1000

/* Function prototypes -------------------------------------------------------*/
void LST_SPI_Init();
void LST_SPI_ReceiveLineControllerData();
void LST_SPI_WaitForLineControllerData();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

extern uint8_t lst_spi_master1_tx[LST_SPI_BUFFER1_SIZE];
extern uint8_t lst_spi_master1_rx[LST_SPI_BUFFER1_SIZE];
extern uint8_t lst_spi_master3_tx[LST_SPI_BUFFER3_SIZE];
extern uint8_t lst_spi_master3_rx[LST_SPI_BUFFER3_SIZE];

#endif /* LST_SPI_H_ */
