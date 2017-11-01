/*
 * function_spi.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "function_spi.h"

void spi_write_ledSegment(uint8_t segment_id)
{

	spi_ledDriver_writeCompleteFlag = 0;

	// TODO

}

void spi_ledDriver_callback(void)
{

	spi_ledDriver_writeCompleteFlag = 1;

}

void spi_reset_ledDriver_writeCompleteFlag(void)
{

	spi_ledDriver_writeCompleteFlag = 0;

}

void spi_interSTM_callback(void)
{

	spi_interSTM_writeCompleteFlag = 1;

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

	if (hspi->Instance == SPI1)
	{

		spi_interSTM_callback();

	}

	if (hspi->Instance == SPI2)
	{

		spi_ledDriver_callback();

	}

}



