/*
 * function_spi.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include <function/lst_function_spi.h>

void lst_spi_write_ledSegment(uint8_t segment_id)
{

	// Fill TX buffer
	for (int i=0; i<4; i++)
	{

		lst_spi_ledDriver_txBuf[i] =
				lst_ledDriver_control[segment_id][i];

	}

	// Write LED drivers
	HAL_SPI_Transmit(
			&hspi2,
			(uint8_t *)&lst_spi_ledDriver_txBuf,
			4,
			LST_SPI_TXRX_TIMEOUT);

}

void lst_spi_clear_ledSegment()
{

	for (int i=0; i<4; i++)
	{

		lst_spi_ledDriver_txBuf[i] = 0;

	}

	HAL_SPI_Transmit(
				&hspi2,
				(uint8_t *)&lst_spi_ledDriver_txBuf,
				4,
				LST_SPI_TXRX_TIMEOUT);

}

// TODO inter-STM send/receive functions
void lst_spi_transmit_interSTM(uint8_t *txData)
{

	HAL_SPI_Transmit_IT(
				&hspi1,
				txData,
				LST_SPI_SIZE
				);

}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

	// inter-STM communication callback
#ifdef LST_NUCLEO_TEST
	if (hspi->Instance == SPI3)
#else
	if (hspi->Instance == SPI1)
#endif
	{

		lst_sendData_TxRxComplete();

	}

}
