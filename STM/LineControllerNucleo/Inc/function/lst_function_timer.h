/*
 * function_timer.h
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#ifndef FUNCTION_LST_FUNCTION_TIMER_H_
#define FUNCTION_LST_FUNCTION_TIMER_H_

// Includes

#include "lst_constants.h"
#ifdef LST_NUCLEO_TEST
	#include "stm32f4xx_hal.h"
#else
	#include "stm32f3xx_hal.h"
#endif

// External variables

extern TIM_HandleTypeDef htim1;

// Constants

#define LST_NANOS_PER_TICK (1000000000UL / LST_F_TIM1)
#define LST_TICKS_PER_MICRO (LST_F_TIM1 / 1000000)
#define LST_REPETITION_MILLI 99
#define LST_REPETITION_DEFAULT 0
#define LST_TIMER1_OVERHEAD_MICRO 9
#ifdef LST_NUCLEO_TEST
	#define LST_NUCLEO_TEST_PIN TIM1_OUTPUT_Pin
	#define LST_NUCLEO_TEST_PORT TIM1_OUTPUT_GPIO_Port
#endif

// Local variables

uint8_t lst_timer1_flag;

// Function declarations

/**
 * \brief
 * 	Blocking function which causes the CPU to idle for at least
 * 	the specified timer clock periods. Based on dummy calculations
 * 	rather than a timer.
 */
void lst_timer1_delay_timClk(uint16_t clk);


/**
 * \brief
 * 	Blocking function which causes the CPU to idle for at least
 * 	the specified time in microseconds. Minimal waiting time is
 * 	around 10us, below that, the function waits for 10us.
 */
void lst_timer1_delay_microSeconds(uint16_t micro);

/**
 * \brief
 * 	Blocking function which causes the CPU to idle for at least
 * 	the specified time in milliseconds.
 */
void lst_timer1_delay_milliSeconds(uint16_t milli);

/**
 * \brief
 * 	Initializes TIM1 PERIOD and REPETITION registers
 */
void lst_timer1_init_timing(uint16_t period, uint16_t repetition);

/**
 * \brief
 * 	Starts TIM1 and waits until PeriodElapsedCallback event
 */
void lst_timer1_start(void);

/**
 * \brief
 * 	HAL callback function, signals lst_timer1_start function
 * 	to return.
 */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
// DO NOT REDEFINE!


#endif /* FUNCTION_LST_FUNCTION_TIMER_H_ */


