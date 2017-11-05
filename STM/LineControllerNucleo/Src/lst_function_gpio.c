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

void lst_gpio_write_muxes(uint8_t segment_id)
{

	// TODO check MUX and LEDDriver address associations,
	// reorder segment_id bits if needed

	HAL_GPIO_WritePin(ADDR0_GPIO_Port, ADDR0_Pin, (segment_id & 1));
	HAL_GPIO_WritePin(ADDR1_GPIO_Port, ADDR1_Pin, (segment_id & 2));
	HAL_GPIO_WritePin(ADDR2_GPIO_Port, ADDR2_Pin, (segment_id & 4));

	lst_timer1_delay_timClk(LST_CD74HC4051_OUTRISE_HOLD_CLK);

}
