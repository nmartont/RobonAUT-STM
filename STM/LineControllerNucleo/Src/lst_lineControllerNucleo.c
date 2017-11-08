/*
 * lst_lineControllerNucleo.c
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#include "lst_lineControllerNucleo.h"

void entry_point(void)
{

	while (1)
	{

		test_timer();

	}

}

void main_function(void)
{

	read_line();

	evaluate_line();

	send_data();

}

void test_timer(void)
{

	lst_timer1_delay_milliSeconds(1);

#ifdef LST_NUCLEO_TEST
	HAL_GPIO_TogglePin(LST_NUCLEO_TEST_PORT, LST_NUCLEO_TEST_PIN);
#endif

	//HAL_Delay(1);

}
