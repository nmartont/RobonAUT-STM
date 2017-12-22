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
#include "task/lst_task_sendData.h"

// External variables

#ifdef LST_NUCLEO_TEST
	/**
	 * \var hspi3 SPI slave connected to mainController
	 */
	extern SPI_HandleTypeDef hspi3;
#else
	/**
	 * \var hspi1 SPI slave connected to mainController
	 */
	extern SPI_HandleTypeDef hspi1;
#endif

/**
 * \var hspi2 SPI master connected to series TLC5928 LED drivers
 */
extern SPI_HandleTypeDef hspi2;

// Variables

/**
 * \var lst_spi_ledDriver_txBuf Stores the data to be transmitted to
 * 	the feedback LED drivers.
 * \var lst_spi_ledDriver_rxBuf Stores the data received from the
 * 	feedback LED drivers. Currently unused.
 */
uint8_t lst_spi_ledDriver_txBuf[4];
uint8_t lst_spi_ledDriver_rxBuf[4];

// Function declarations

/**
 * \brief Writes the contents of lst_ledDriver_control[segment_id]
 * 	on the SPI master line.
 */
void lst_spi_write_ledSegment(uint8_t segment_id);

/**
 * \brief Writes 0x0000 on the SPI master line. All TCRT LEDs are
 * 	turned off.
 */
void lst_spi_clear_ledSegment(void);

/**
 * @brief Transmits the line sensor data over the SPI slave line to
 *  the mainController.
 * @param txData The 38-byte data package being transmitted.
 */
void lst_spi_transmit_interSTM(uint8_t *txData); // TODO:syntax check

#endif /* FUNCTION_SPI_H_ */
