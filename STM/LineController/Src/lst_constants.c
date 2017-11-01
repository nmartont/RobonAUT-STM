/*
 * lst_constants.c
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#include "stm32f3xx_hal.h"

/* task_readline */

// ledDriver latch hold delay TLC5928: min 20ns
#define lst_latch_hold_nanosec 20
#define lst_latch_hold_timClk 2 // 2 TIM1 clocks

/* function_timer */

#define picos_per_tick (1000000 / 64) // clk: 64MHz -> 15625ps
