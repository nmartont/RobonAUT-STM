/*
 * function_timer.h
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#include "stm32f3xx_hal.h"

extern htim1;

uint8_t timer1_flag;

// TIM1: 64MHz -> 15.625 ns / timClk
void timer1_delay_timClk(uint16_t clk);

// Takes more time to calculate timClk count than the actual delay (?)
void timer1_delay_nanoSeconds(uint16_t nano);

void timer1_delay_microSeconds(uint16_t micro);

void timer1_delay_milliSeconds(uint16_t milli);
