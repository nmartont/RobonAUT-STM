/*
 * function_gpio.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include "function_gpio.h"
#include "lst_constants.c"

void lst_gpio_latch_ledSegment(void)
{

	HAL_GPIO_WritePin(SPI2_LATCH_GPIO_Port, SPI2_LATCH_Pin, 1);

	timer1_delay_timClk(lst_latch_hold_timClk); // ~30ns delay

	HAL_GPIO_WritePin(SPI2_LATCH_GPIO_Port, SPI2_LATCH_Pin, 0);

}
