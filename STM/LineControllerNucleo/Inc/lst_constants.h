/*
 * lst_constants.h
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_CONSTANTS_H_
#define LST_CONSTANTS_H_

// Test constants
#define LST_NUCLEO_TEST
/*
 * @Warning
 * 	Define if code is run on NUCLEO, comment out if run on STM32F3
 */

// STM constants
#define LST_F_TIM1 64000000UL
/*
 * @Warning
 * 	Timer functions only work correctly below TIM1 frequencies of 64MHz
 */

// TLC5928 constants
#define LST_TLC5928_LATCH_HOLD_CLK 2
/*
 * @Description
 * 	Number of TIM1 clock cycles to wait while the LATCH pin
 * 	of the TLC5928 LED driver is held high to enable output
 */

#endif /* LST_CONSTANTS_H_ */
