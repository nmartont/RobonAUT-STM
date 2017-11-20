/*
 * lst_lineControllerNucleo.c
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#include "lst_lineController.h"

void entry_point(void)
{

	while (1)
	{

		//test_timer();

		main_function();

		//test_LedDrivers();

	}

}

void main_function(void)
{

	read_line();

	//evaluate_line();

	//send_data();

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
