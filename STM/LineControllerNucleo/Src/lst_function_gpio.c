/*
 * function_gpio.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include "lst_function_gpio.h"

void lst_gpio_latch_ledSegment(void)
{

	HAL_GPIO_WritePin(LATCH_PORT, LATCH_PIN, 1);

	lst_timer1_delay_timClk(LST_TLC5928_LATCH_HOLD_CLK);

	HAL_GPIO_WritePin(LATCH_PORT, LATCH_PIN, 0);

}
