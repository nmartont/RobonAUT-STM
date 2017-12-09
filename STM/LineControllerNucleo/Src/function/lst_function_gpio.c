/*
 * function_gpio.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include <function/lst_function_gpio.h>

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

}

void lst_gpio_write_feedbackLeds(uint32_t data)
{

	// Reset SCLK, lATCH
	HAL_GPIO_WritePin(LFB_SCLK_GPIO_Port, LFB_SCLK_Pin, 0);
	HAL_GPIO_WritePin(LFB_LATCH_GPIO_Port, LFB_LATCH_Pin, 0);

	// Latch hold time
	lst_timer1_delay_timClk(LFB_RESET_HOLD_CLK);

	// Shift bits
	for (uint8_t i=0; i<32; i++)
	{

		// Data out
		HAL_GPIO_WritePin(LFB_SIN_GPIO_Port, LFB_SIN_Pin, ((data >> (31 -i)) & 1));
		lst_timer1_delay_timClk(LFB_SET_HOLD_CLK);

		// Clock
		HAL_GPIO_WritePin(LFB_SCLK_GPIO_Port, LFB_SCLK_Pin, 1);
		lst_timer1_delay_timClk(LFB_SET_HOLD_CLK);
		HAL_GPIO_WritePin(LFB_SCLK_GPIO_Port, LFB_SCLK_Pin, 0);


	}

}

void lst_gpio_latch_feedbackLeds(void)
{

	// Pre-latch hold
	//lst_timer1_delay_timClk(LFB_RESET_HOLD_CLK);
	lst_timer1_delay_microSeconds(10);

	// Latch
	HAL_GPIO_WritePin(LFB_LATCH_GPIO_Port, LFB_LATCH_Pin, 1);
	//lst_timer1_delay_timClk(LFB_RESET_HOLD_CLK);
	lst_timer1_delay_microSeconds(10);
	HAL_GPIO_WritePin(LFB_LATCH_GPIO_Port, LFB_LATCH_Pin, 0);

}
