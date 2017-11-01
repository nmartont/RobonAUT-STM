/*
 * function_spi.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#ifndef FUNCTION_SPI_H_
#define FUNCTION_SPI_H_

// Includes
#include "lst_constants.h"
#ifdef LST_NUCLEO_TEST
	#include "stm32f4xx_hal.h"
#else
	#include "stm32f3xx_hal.h"
#endif

// External variables
#ifdef LST_NUCLEO_TEST
	extern SPI_HandleTypeDef hspi3;
#else
	extern SPI_HandleTypeDef hspi1;
#endif
/*
 * @Description
 * 	SPI slave connected to mainController
 */
extern SPI_HandleTypeDef hspi2;
/*
 * @Description
 * 	SPI master connected to series TLC5928 LED drivers
 */

// Variables
uint8_t lst_spi_interStm_writeCompleteFlag;
uint8_t lst_spi_ledDriver_writeCompleteFlag;

// Function declarations

void lst_spi_write_ledSegment(uint8_t segment_id);
/*
 * @Description
 *
 */

void lst_spi_ledDriver_callback(void);
/*
 * @Description
 *
 */

void spi_interStm_callback(void);
/*
 * @Description
 *
 */

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
/*
 * @Description
 *
 */

#endif /* FUNCTION_SPI_H_ */
