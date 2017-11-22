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

	// Write address
		HAL_GPIO_WritePin(ADDR0_GPIO_Port, ADDR0_Pin,
				(lst_mux_control[segment_id] & 1));
		HAL_GPIO_WritePin(ADDR1_GPIO_Port, ADDR1_Pin,
				(lst_mux_control[segment_id] & 2));
		HAL_GPIO_WritePin(ADDR2_GPIO_Port, ADDR2_Pin,
				(lst_mux_control[segment_id] & 4));

	/*
	// Convert segment_id to MUX address TODO check
	switch (segment_id)
	{

	case 0:
		lst_segment_id_mux = 4;
		break;
	case 1:
		lst_segment_id_mux = 6;
		break;
	case 2:
		lst_segment_id_mux = 7;
		break;
	case 3:
		lst_segment_id_mux = 5;
		break;
	case 4:
		lst_segment_id_mux = 2;
		break;
	case 5:
		lst_segment_id_mux = 1;
		break;
	case 6:
		lst_segment_id_mux = 0;
		break;
	case 7:
		lst_segment_id_mux = 3;
		break;

	}
*/

}
