/*
 * task_sendData.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include <task/lst_task_sendData.h>

// TODO:test communication w/ mainController again

// TODO:refactor -> update send buffer whenever SS is high
// (there is no ongoing communication)

void lst_sendData(void)
{

	// Send only if previous has completed
	if (lst_spiCompleted)
	{

			lst_sendData_initValues();

			lst_sendData_fillTxBuffer();

			lst_sendData_transmitReceive();

	}

}

void lst_sendData_initValues(void)
{

	for (uint8_t i=0; i<LST_SPI_SIZE; i++)
	{

		lst_spiData_tx[i] = 0;

	}

	for (uint8_t i=0; i<LST_SPI_SIZE; i++)
	{

		lst_spiData_rx[i] = 0;

	}

}

void lst_sendData_fillTxBuffer(void)
{

	// Write 0xff and line count into first two bytes TODO
	lst_spiData_tx[0] = 255;
	lst_spiData_tx[1] = lst_eval_lineCount << 1; // TODO:FIRST BIT SHIT
	// random SPI error first bit always 1

	// Fill line position data into bytes 2,3 and 4,5
	for (uint8_t i=1; i<LST_SPI_TXBUF_VALUE_START / 2; i++)
	{

		lst_spiData_tx[2 * i] = lst_eval_subPositions[i - 1] & 255;
		lst_spiData_tx[2 * i + 1] = lst_eval_subPositions[i - 1] >> 8;

	}

	// Fill sensor output data
	for (uint8_t i=LST_SPI_TXBUF_VALUE_START; i<LST_SPI_TXBUF_SIZE; i++)
	{

		lst_spiData_tx[i] = lst_tcrt_values[i - LST_SPI_TXBUF_VALUE_START];

	}


}

void lst_sendData_transmitReceive(void)
{

	// New packet cannot be sent until this completes
	lst_spiCompleted = 0;

	// Call TxRx function
	lst_spi_transmit_interSTM(&lst_spiData_tx);


}

void lst_sendData_TxRxComplete()
{

	lst_spiCompleted = 1;

}

void lst_sendData_init()
{

	// Set spiCompleted to 1 to enter data transfer function at start
	lst_spiCompleted = 1;

}

