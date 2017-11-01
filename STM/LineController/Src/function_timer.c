/*
 * function_timer.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include "function_timer.h"
#include "lst_constants.c"

void timer1_delay_timClk(uint16_t clk)
{

	timer1_init_period(clk);

	timer1_start();

}

void timer1_delay_nanoSeconds(uint16_t nano)
{

	timer1_init_period(nano * picos_per_tick / 1000);

}

void timer1_delay_microSeconds(uint16_t micro)
{



}

void timer1_delay_milliSeconds(uint16_t milli)
{



}

void timer1_init_period(uint16_t clk)
{
	// TODO 65535 MAX !!
	htim1.Init.Period = clk;

	HAL_TIM_Base_init(htim1);

}

void timer1_start()
{

	timer1_flag = 0;

	HAL_TIM_OnePulse_Start_IT(htim1, TIM_CHANNEL_1);

	while (timer1_flag != 1)
	{

		// Wait

	}

}

HAL_TIM_PeriodElapsedCallback(htim1)
{

	timer1_flag = 1;

}
