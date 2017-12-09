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

	// Send only if previous has completed
	if (lst_uartCompleted)
	{

		// Send only if mainController is ready
		if (HAL_GPIO_ReadPin(UART4_RQD_GPIO_Port, UART4_RQD_Pin)
				== GPIO_PIN_RESET)

		{

			lst_sendData_initValues();

			lst_sendData_fillTxBuffer();

			lst_sendData_transmitReceive();

		}

	}

}

void lst_sendData_initValues(void)
{

	for (uint8_t i=0; i<LST_SPI_SIZE; i++)
	{

		lst_uartData_tx[i] = 0;

	}

	for (uint8_t i=0; i<LST_SPI_SIZE; i++)
	{

		lst_uartData_rx[i] = 0;

	}

}

void lst_sendData_fillTxBuffer(void)
{

	// Write 0xff and line count into first two bytes TODO
	lst_uartData_tx[0] = 255;
	lst_uartData_tx[1] = lst_eval_lineCount << 1; // TODO:FIRST BIT SHIT
	// random SPI error first bit always 1

	// Fill line position data into bytes 2,3 and 4,5
	for (uint8_t i=1; i<LST_SPI_TXBUF_VALUE_START / 2; i++)
	{

		lst_uartData_tx[2 * i] = lst_eval_subPositions[i - 1] & 255;
		lst_uartData_tx[2 * i + 1] = lst_eval_subPositions[i - 1] >> 8;

	}

	// Fill sensor output data
	for (uint8_t i=LST_SPI_TXBUF_VALUE_START; i<LST_SPI_TXBUF_SIZE; i++)
	{

		lst_uartData_tx[i] = lst_tcrt_values[i - LST_SPI_TXBUF_VALUE_START];

	}


}

void lst_sendData_transmitReceive(void)
{

	// New packet cannot be sent until this completes
	lst_uartCompleted = 0;

	// Call TxRx function
	HAL_UART_Transmit_IT(
			&huart4,
			(uint8_t *) &lst_uartData_tx,
			LST_SPI_SIZE
			);

}

void lst_sendData_init()
{

	// Set spiCompleted to 1 to enter data transfer function at start
	lst_uartCompleted = 1;

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

	lst_uartCompleted = 1;

}
