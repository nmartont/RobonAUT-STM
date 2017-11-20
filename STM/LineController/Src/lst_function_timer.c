/*
 * function_timer.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include "lst_function_timer.h"

void lst_timer1_delay_timClk(uint16_t clk)
{

	for (volatile int i=0; i<clk; i++)
	{
		// Dummy loop
	}

}

void lst_timer1_delay_microSeconds(uint16_t micro)
{

	// Handle overhead
	if (micro < LST_TIMER1_OVERHEAD_MICRO + 1)
	{

		micro = 1;

	}
	else
	{

		micro = micro - LST_TIMER1_OVERHEAD_MICRO;

	}

	// Handle overflow
	if (micro > 1000)
	{

		for (int i=0; i<(micro / 1000); i++)
		{

			lst_timer1_init_timing(LST_TICKS_PER_MICRO * 1000,
					LST_REPETITION_DEFAULT);

			lst_timer1_start();

		}

		lst_timer1_init_timing(LST_TICKS_PER_MICRO * (micro % 1000),
				LST_REPETITION_DEFAULT);

	}
	else
	{

		lst_timer1_init_timing(LST_TICKS_PER_MICRO * micro,
				LST_REPETITION_DEFAULT);

	}

	lst_timer1_start();

}

void lst_timer1_delay_milliSeconds(uint16_t milli)
{

	// Handle overflow
	if (milli > 100)
	{

		for (int i=0; i<(milli / 100); i++)
		{

			lst_timer1_init_timing(LST_TICKS_PER_MICRO * 1000,
					LST_REPETITION_MILLI);

			lst_timer1_start();

		}

		lst_timer1_init_timing(LST_TICKS_PER_MICRO * (milli % 100) * 10,
				LST_REPETITION_MILLI);

	}
	else
	{

		lst_timer1_init_timing(LST_TICKS_PER_MICRO * milli * 10,
				LST_REPETITION_MILLI);

	}

	lst_timer1_start();

}

void lst_timer1_init_timing(uint16_t period, uint16_t repetition)
{

	htim1.Init.RepetitionCounter = repetition;
	htim1.Init.Period = period;

	HAL_TIM_Base_Init(&htim1);

}

void lst_timer1_start(void)
{

	lst_timer1_flag = 0;

	HAL_TIM_Base_Start_IT(&htim1);

	while (lst_timer1_flag != 1)
	{

		// Wait

	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	lst_timer1_flag = 1;

}


