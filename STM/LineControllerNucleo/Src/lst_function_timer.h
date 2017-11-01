/*
 * function_timer.h
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "lst_constants.h"

extern TIM_HandleTypeDef htim1;

uint8_t timer1_flag;

// TIM1: 64MHz -> 15.625 ns / timClk
void timer1_delay_timClk(uint16_t clk);

void timer1_delay_microSeconds(uint16_t micro);

void timer1_delay_milliSeconds(uint16_t milli);

void timer1_init_timing(uint16_t period, uint16_t repetition);

void timer1_start(void);
