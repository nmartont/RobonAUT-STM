/*
 * function_spi.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "lst_function_spi.h"

void lst_spi_write_ledSegment(uint8_t segment_id)
{

	lst_spi_ledDriver_writeCompleteFlag = 0;

	// TODO

	// Wait for SPI transaction complete flag
	while (!lst_spi_ledDriver_writeCompleteFlag)
	{

		// Wait

	}

}

void lst_spi_ledDriver_callback(void)
{

	lst_spi_ledDriver_writeCompleteFlag = 1;

}

// TODO inter-STM send/receive functions

void spi_interStm_callback(void)
{

	lst_spi_interStm_writeCompleteFlag = 1;

}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

#ifdef LST_NUCLEO_TEST
	if (hspi->Instance == SPI3)
#else
	if (hspi->Instance == SPI1)
#endif
	{

		spi_interStm_callback();

	}

	if (hspi->Instance == SPI2)
	{

		lst_spi_ledDriver_callback();

	}

}



