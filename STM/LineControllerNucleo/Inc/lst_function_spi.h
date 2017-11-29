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

uint8_t lst_spi_ledDriver_txBuf[4];
uint8_t lst_spi_ledDriver_rxBuf[4];

// Constants

#define LST_SPI_TXRX_TIMEOUT 200 // msec

// Function declarations

void lst_spi_write_ledSegment(uint8_t segment_id);
/*
 * @Description
 *	TODO
 */

void lst_spi_ledDriver_callback(void);
/*
 * @Description
 *	TODO
 */

void spi_interStm_callback(void);
/*
 * @Description
 *	TODO
 */

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
/*
 * @Description
 *	TODO
 */

#endif /* FUNCTION_SPI_H_ */