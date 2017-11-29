/*
 * lst_task_displayLine.c
 *
 *  Created on: 23 Nov 2017
 *      Author: Balazs
 */

#include <task/lst_task_displayLine.h>

void lst_displayLine(uint8_t threshold)
{

	// TODO if threshold = 0 -> display based on evaluation data

	// Clear output variable
	lst_feedbackLed_output = 0;

	// Set bits where the LED is to be lit
	for (uint8_t i=0; i<32; i++)
	{

		if (lst_tcrt_values[i] > threshold)
		{

			lst_feedbackLed_output = lst_feedbackLed_output | (1 << i);

		}

	}

	// Shift bits into LED drivers
	lst_gpio_write_feedbackLeds(lst_feedbackLed_output);

	// Latch LED drivers
	lst_gpio_latch_feedbackLeds();

}
