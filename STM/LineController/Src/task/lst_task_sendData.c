/*
 * task_sendData.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include <task/lst_task_sendData.h>

// TODO:test communication w/ mainController again

// TODO:upgrade -> update send buffer whenever SS is high
// (there is no ongoing communication)

void lst_sendData_checkWatchdog();
void lst_sendData_resetWatchdog();

void lst_sendData(void)
{

	// TODO TEST 2018. 01. 30.
	// Fill data buffer regardless of SPI state
	lst_sendData_fillTxBuffer();

	lst_sendData_checkWatchdog();

	// Send only if previous has completed
	if (lst_spiCompleted)
	{

		// TODO TEST 2018. 01. 30. why is it needed?
		//lst_sendData_initValues();

		//lst_sendData_fillTxBuffer();

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

	// TODO:temporary solution
	// Write 0b11110000 and line count into first two bytes
	lst_spiData_tx[0] = 120;
	lst_spiData_tx[1] = lst_eval_lineCount << 1;
	// FIRST BIT SHIFT
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
	//lst_spi_transmit_interSTM(&lst_spiData_tx);

	/*
	// TODO:TEST 2018. 01. 02.
	for (uint8_t i=0; i<LST_SPI_SIZE; i++)
	{
		lst_spiData_tx[i] = 5;
	}
	*/

	HAL_SPI_TransmitReceive_IT(
					&hspi1,
					(uint8_t *) &lst_spiData_tx,
					(uint8_t *) &lst_spiData_rx,
					LST_SPI_SIZE
					);

	// Signal the mainController - ready to transmit/receive
	// (pull DRDY high)
	HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 1);

}

void lst_sendData_TxRxComplete()
{

	// Signal the mainController - busy (pull DRDY low)
	HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 0);

	// Signal sendData that the next packet can be sent in
	// the following sensor read cycle
	lst_spiCompleted = 1;

	lst_sendData_resetWatchdog();

}

void lst_sendData_init()
{

	// Set spiCompleted to 1 to enter data transfer function at start
	lst_spiCompleted = 1;

	// Pull DRDY low to signal that LineController is busy
	HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 0);

}

void lst_sendData_checkWatchdog()
{

	if (lst_spiWatchdog > LST_SPI_WATCHDOG_TIMEOUT)
	{

		lst_spiCompleted = 0;

		// Signal the mainController - busy (pull DRDY low)
		HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 0);

		HAL_SPI_DeInit(&hspi1);

		lst_timer1_delay_milliSeconds(LST_SPI_INIT_DELAY_MS);

		HAL_SPI_Init(&hspi1);

		lst_timer1_delay_milliSeconds(LST_SPI_INIT_DELAY_MS);

		lst_spiCompleted = 1;

	}

}

void lst_sendData_resetWatchdog()
{

	lst_spiWatchdog = 0;

}
