/*
 * function_spi.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#ifndef FUNCTION_SPI_H_
#define FUNCTION_SPI_H_

// Includes

#include <task/lst_task_readLine.h>
#include "lst_constants.h"

// External variables

/**
 * \brief
 * 	SPI slave connected to mainController
 */
#ifdef LST_NUCLEO_TEST
	extern SPI_HandleTypeDef hspi3;
#else
	extern SPI_HandleTypeDef hspi1;
#endif

/**
 * \brief
 * 	SPI master connected to series TLC5928 LED drivers
 */
extern SPI_HandleTypeDef hspi2;


// Variables

uint8_t lst_spi_interStm_writeCompleteFlag;

uint8_t lst_spi_ledDriver_txBuf[4];
uint8_t lst_spi_ledDriver_rxBuf[4];

// Constants

#define LST_SPI_TXRX_TIMEOUT 50 // msec

// Function declarations

/**
 * \brief
 *	TODO
 */
void lst_spi_write_ledSegment(uint8_t segment_id);

/**
 * \brief
 *	TODO
 */
void spi_interStm_callback(void);

/**
 * \brief
 *	TODO
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);

#endif /* FUNCTION_SPI_H_ */
