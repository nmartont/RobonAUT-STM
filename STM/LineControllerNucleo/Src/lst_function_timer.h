/*
 * function_timer.h
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

// Includes
#include "stm32f4xx_hal.h"
#include "lst_constants.h"

// External variables
extern TIM_HandleTypeDef htim1;

// Constants
#define LST_TICKS_PER_MICRO (LST_F_CPU / 1000000)
#define LST_REPETITION_MILLI 999
#define LST_REPETITION_DEFAULT 0
#define LST_NUCLEO_TEST_PIN GPIO_PIN_0
#define LST_NUCLEO_TEST_PORT GPIOC

// Local variables
uint8_t lst_timer1_flag;

// Function declarations

void lst_timer1_delay_timClk(uint16_t clk);
/*
 * @Description
 * 	Blocking function which causes the CPU to idle for at least the
 * 	specified CPU clock cycle count.
 *
 * @Details
 * 	TIM1 clock source: 64MHz
 * 	TIM1 clock period (clk): 1/64M = 15.625 nanoseconds
 */

void lst_timer1_delay_microSeconds(uint16_t micro);
/*
 * @Description
 * 	Blocking function which causes the CPU to idle for at least
 * 	the specified time in microseconds.
 */

void lst_timer1_delay_milliSeconds(uint16_t milli);
/*
 * @Description
 * 	Blocking function which causes the CPU to idle for at least
 * 	the specified time in milliseconds.
 */

void lst_timer1_init_timing(uint16_t period, uint16_t repetition);
/*
 * @Description
 * 	Initializes TIM1 PERIOD and REPETITION registers
 */

void lst_timer1_start(void);
/*
 * @Description
 * 	Starts TIM1 and waits until PeriodElapsedCallback event
 *
 * @Test
 * 	Toggles NUCLEO_TEST_PIN output each time TIM1 is started
 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
/*
 * @Description
 * 	Signals lst_timer1_start function
 */
