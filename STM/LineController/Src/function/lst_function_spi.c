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

	// TODO where to handle received LED error data?

}

// TODO inter-STM send/receive functions

void spi_interStm_callback(void)
{

	lst_sendData_TxRxComplete();
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

}

/*
// TODO:temp TEST
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{

	if (hspi->Instance == SPI1)
	{

		spi_interStm_callback();

	}

}
*/
