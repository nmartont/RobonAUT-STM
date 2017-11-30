/*
 * task_sendData.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include <task/lst_task_sendData.h>

// TODO:move functions to function source files

void lst_sendData(void)
{

	//while (!lst_spiCompleted) {};

	if (lst_spiCompleted)
		lst_spiTest();
	/*
	{

		lst_sendData_initValues();

		//lst_sendData_fillTxBuffer();

		lst_sendData_transmitReceive();

	}
*/
}

void lst_spiTest()
{

	// Fill data
	for (uint8_t i=0; i<6; i++)
	{

		lst_spiData_tx[i] = 255;

	}


	for (uint8_t i=6; i<38; i++)
	{

		lst_spiData_tx[i] = 0;

	}

	lst_spiCompleted = 0;

	HAL_SPI_TransmitReceive_IT(
				&hspi1,
				(uint8_t *) &lst_spiData_tx,
				(uint8_t *) &lst_spiData_rx,
				LST_SPI_SIZE);

	HAL_GPIO_WritePin(SPI1_DRDY_GPIO_Port, SPI1_DRDY_Pin, 1);

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

	// Fill line position data
	for (uint8_t i=0; i<LST_SPI_TXBUF_VALUE_START; i++)
	{

		// TODO:continue - fill line position data

	}

	for (uint8_t i=0; i<32; i++)
	{

		lst_spiData_rx[i] = lst_tcrt_values[i];

	}

	/*
	// Fill sensor output data
	for (uint8_t i=LST_SPI_TXBUF_VALUE_START; i<LST_SPI_TXBUF_SIZE; i++)
	{

		lst_spiData_tx[i] = lst_tcrt_values[i - LST_SPI_TXBUF_VALUE_START];

	}
	*/

}

void lst_sendData_transmitReceive(void)
{

	// New packet cannot be sent until this completes
	lst_spiCompleted = 0;

	// Call TxRx function
	HAL_SPI_TransmitReceive_IT(
			&hspi1,
			(uint8_t *) &lst_spiData_tx,
			(uint8_t *) &lst_spiData_rx,
			LST_SPI_SIZE);

	// Signal the mainController - ready to transmit/receive
	// (pull DRDY high)
	HAL_GPIO_WritePin(SPI1_DRDY_GPIO_Port, SPI1_DRDY_Pin, 1);

}

void lst_sendData_TxRxComplete(void)
{

	// Signal the mainController - busy (pull DRDY low)
	HAL_GPIO_WritePin(SPI1_DRDY_GPIO_Port, SPI1_DRDY_Pin, 0);

	// Signal sendData that the next packet can be sent in
	// the following cycle
	lst_spiCompleted = 1;

}
