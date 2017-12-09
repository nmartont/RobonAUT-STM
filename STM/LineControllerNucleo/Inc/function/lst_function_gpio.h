/*
 * function_gpio.h
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#ifndef FUNCTION_LST_FUNCTION_GPIO_H_
#define FUNCTION_LST_FUNCTION_GPIO_H_

// Includes

#include <function/lst_function_timer.h>
#include <task/lst_task_readLine.h>
#include "lst_constants.h"
#ifdef LST_NUCLEO_TEST
	#include "stm32f4xx_hal.h"
#else
	#include "stm32f3xx_hal.h"
#endif

// External variables

#define LATCH_PORT			SPI2_LATCH_GPIO_Port
#define LATCH_PIN				SPI2_LATCH_Pin

// Constants

#define LFB_RESET_HOLD_CLK 6
#define LFB_SET_HOLD_CLK 2

// Function declarations

/**
 * \brief
 * 	Sets the TLC5928 LATCH inputs high for the required duration
 * 	to enable the LED drive outputs (at least 30 nanoseconds)
 */
void lst_gpio_latch_ledSegment(void);

/**
 * \brief
 *	Sets all analog MUX addresses to pass on the output
 *	of the segment's TCRT sensors to the 4 ADCs.
 */
void lst_gpio_write_muxes(uint8_t segment_id);

/**
 * \brief
 *  // TODO doxy
 */
void lst_gpio_write_feedbackLeds(uint32_t data);

/**
 * \brief
 *  // TODO doxy
 */
void lst_gpio_latch_feedbackLeds(void);

#endif /* FUNCTION_LST_FUNCTION_GPIO_H_ */
