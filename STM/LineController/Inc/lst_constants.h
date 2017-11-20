/*
 * lst_constants.h
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_CONSTANTS_H_
#define LST_CONSTANTS_H_

// Test constants
//#define LST_NUCLEO_TEST
#define LST_NUCLEO_LINECONTROLLER
/*
 * @Warning
 * 	Define if code is run on NUCLEO, comment out if run on STM32F3
 */

#ifdef LST_NUCLEO_TEST
	#include "stm32f4xx_hal.h"
#else
	#include "stm32f3xx_hal.h"
#endif
/*
 * @Description
 * 	Hardware Abstraction Layer Library of the appropriate controller family
 */

// Line evaluation algorithm selection

#define LST_LINEEVAL_BASIC
/*
 * @Description
 * 	Basic algorithm that was developed first.
 */

// Line evaluation parameters - basic

#define LST_LINEEVAL_BASIC_MAX_PRX 2
/*
 * @Description
 * 	The proximity of a valid maximum, where a sufficiently low value
 * 	has to be found.
 */

#define LST_LINEEVAL_BASIC_MAX_THR 40
/*
 * @Description
 * 	A value lower than the local maximum minus this value has to be
 * 	found in the proximity of a valid maximum.
 */

#define LST_LINEEVAL_BASIC_GLOBALMAX_THR 60
/*
 * @Description
 * 	A local maximum can only be valid if it is no less than the global
 * 	maximum minus this value.
 */

//#define LST_LINEEVAL_ADVANCED

// STM constants
#define LST_F_TIM1 64000000UL
/*
 * @Warning
 * 	Timer functions only work correctly below TIM1 frequencies of 64MHz
 */

// Hardware signal hold constants

#define LST_TLC5928_LATCH_HOLD_CLK 2
/*
 * @Description
 * 	Number of TIM1 clock cycles to wait while the LATCH pin
 * 	of the TLC5928 LED driver is held high to enable output
 */

#define LST_CD74HC4051_OUTRISE_HOLD_CLK 100 // TODO check w\ measurement

#endif /* LST_CONSTANTS_H_ */
