/*
 * function_gpio.h
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_FUNCTION_GPIO_H_
#define LST_FUNCTION_GPIO_H_

// Includes
#include "lst_constants.h"
#include "lst_function_timer.h"
#ifdef LST_NUCLEO_TEST
	#include "stm32f4xx_hal.h"
#else
	#include "stm32f3xx_hal.h"
#endif

// Constants
#define LATCH_PORT SPI2_LATCH_GPIO_Port
#define LATCH_PIN SPI2_LATCH_Pin

// Function declarations
void lst_gpio_latch_ledSegment(void);
/*
 * @Description
 * 	Sets the TLC5928 LATCH inputs high for the required duration
 * 	to enable the LED drive outputs (at least 30 nanoseconds)
 */


#endif /* LST_FUNCTION_GPIO_H_ */
