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

void debug_driveAddress()
{

	// Groups
	for (int i=0; i<4; i++)
	{

		// Sensors
		for (int j=0; j<8; j++)
		{

			// Zeros into driver buffers
			for (int k=0; k<4; k++)
			{

				lst_spi_ledDriver_txBuf[k] = 0;

			}

			// Enable only one LED
			lst_spi_ledDriver_txBuf[i] = 1 << j;

			// Write LedDrivers
			HAL_SPI_Transmit(
					&hspi2,
					(uint8_t *) &lst_spi_ledDriver_txBuf,
					4,
					LST_SPI_TXRX_TIMEOUT);

			// Latch LED drivers
			lst_gpio_latch_ledSegment();

			// Random delay
			lst_timer1_delay_microSeconds(1000);

		}

	}

}

void test_timer(void)
{

	a = 0;

	while (1)
	{

		if (a > 100)
		{

			a = 0;

		}
		else
		{

			a++;

		}

		lst_timer1_delay_milliSeconds(1);

	#ifdef LST_NUCLEO_TEST
		HAL_GPIO_TogglePin(LST_NUCLEO_TEST_PORT, LST_NUCLEO_TEST_PIN);
	#endif

		//HAL_Delay(1);


	}

}
