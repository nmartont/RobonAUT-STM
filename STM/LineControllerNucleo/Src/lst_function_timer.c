/*
 * function_timer.c
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include "lst_function_timer.h"

#define ticks_per_micro (F_CPU / 1000000)
#define repetition_milli 999
#define repetition_default 0

void timer1_delay_timClk(uint16_t clk)
{

	timer1_init_timing(clk, repetition_default);

	timer1_start();

}

// TODO input has to be less than 1000
void timer1_delay_microSeconds(uint16_t micro)
{

	timer1_init_timing(ticks_per_micro * micro, repetition_default);

	timer1_start();

}

// TODO input has to be less than 1000
void timer1_delay_milliSeconds(uint16_t milli)
{

	timer1_init_timing(ticks_per_micro * milli, repetition_milli);

	timer1_start();

}

void timer1_init_timing(uint16_t period, uint16_t repetition)
{
	// TODO 65535 MAX !!

	htim1.Init.RepetitionCounter = repetition;
	htim1.Init.Period = period;

	HAL_TIM_Base_Init(&htim1);

}

void timer1_start(void)
{

	timer1_flag = 0;

	// Toggle PC00
	HAL_GPIO_TogglePin(TIM1_OUTPUT_GPIO_Port, TIM1_OUTPUT_Pin);

	HAL_TIM_Base_Start_IT(&htim1);

	while (timer1_flag != 1)
	{

		// Wait

	}

}

void HAL_TIM_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{

	HAL_TIM_Base_Stop_IT(&htim1);

	timer1_flag = 1;

}

