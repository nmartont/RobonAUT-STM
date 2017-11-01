/*
 * function_spi.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "function_spi.h"

void lst_spi_write_ledSegment(uint8_t segment_id)
{

	lst_spi_ledDriver_writeCompleteFlag = 0;

	// TODO

}

void lst_spi_ledDriver_callback(void)
{

	lst_spi_ledDriver_writeCompleteFlag = 1;

}

void spi_reset_ledDriver_writeCompleteFlag(void)
{

	lst_spi_ledDriver_writeCompleteFlag = 0;

}

void spi_interStm_callback(void)
{

	lst_spi_interStm_writeCompleteFlag = 1;

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

	if (hspi->Instance == SPI1)
	{

		spi_interStm_callback();

	}

	if (hspi->Instance == SPI2)
	{

		lst_spi_ledDriver_callback();

	}

}



