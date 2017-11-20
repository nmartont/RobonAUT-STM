/*
 * lst_task_testOnly.c
 *
 *  Created on: 16 Nov 2017
 *      Author: Balazs
 */

#include "lst_task_testOnly.h"


uint8_t lst_test_ledDriver_txBuf[4];

void test_LedDrivers()
{

	for (int i=0; i<4; i++)
	{

		lst_test_ledDriver_txBuf[i] = 255;

	}

	HAL_SPI_Transmit(&hspi2, (uint8_t *) &lst_test_ledDriver_txBuf, 4, 100);

	lst_gpio_latch_ledSegment();



	for (int i=0; i<4; i++)
		{

			lst_test_ledDriver_txBuf[i] = 0;

		}

		HAL_SPI_Transmit(&hspi2, (uint8_t *) &lst_test_ledDriver_txBuf, 4, 100);

		lst_gpio_latch_ledSegment();

}
