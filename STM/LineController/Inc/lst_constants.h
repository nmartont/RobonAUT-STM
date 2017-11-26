/*
 * lst_constants.h
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_CONSTANTS_H_
#define LST_CONSTANTS_H_

// Test constants

/**
 * \warning
 * 	Define if code is run on NUCLEO, comment out if run on STM32F3
 */
//#define LST_NUCLEO_TEST
#define LST_NUCLEO_LINECONTROLLER

/**
 * \brief
 * 	Hardware Abstraction Layer Library of the appropriate controller family
 */
#ifdef LST_NUCLEO_TEST
	#include "stm32f4xx_hal.h"
#else
	#include "stm32f3xx_hal.h"
#endif

// Line evaluation algorithm selection

/**
 * \brief
 * 	Basic algorithm that was developed first.
 */
#define LST_LINEEVAL_BASIC

// Line evaluation parameters - basic

/**
 * \brief
 * 	The proximity of a valid maximum, where a sufficiently low value
 * 	has to be found.
 */
#define LST_LINEEVAL_BASIC_MAX_PRX 2

/**
 * \brief
 * 	A value lower than the local maximum minus this value has to be
 * 	found in the proximity of a valid maximum.
 */
#define LST_LINEEVAL_BASIC_MAX_THR 40

/**
 * \brief
 * 	A local maximum can only be valid if it is no less than the global
 * 	maximum minus this value.
 */
#define LST_LINEEVAL_BASIC_GLOBALMAX_THR 60

/**
 * \brief
 * 	If line sensing is displayed based on raw sensor input data
 * 	(not based on the evaluation algorithm result) this value
 * 	determines if a feedback LED is to be lit or not.
 */
#define LST_LINEDISPLAY_RAW_THRESHOLD 150

//#define LST_LINEEVAL_ADVANCED

// STM constants

/**
 * @Warning
 * 	Timer functions only work correctly below TIM1 frequencies of 64MHz
 */
#define LST_F_TIM1 64000000UL


// Hardware signal hold constants

/**
 * \brief
 * 	Number of TIM1 clock cycles to wait while the LATCH pin
 * 	of the TLC5928 LED driver is held high to enable output
 */
#define LST_TLC5928_LATCH_HOLD_CLK 2

/**
 * \brief
 * 	TODO
 */
#define LST_CD74HC4051_OUTRISE_HOLD_CLK 100 // TODO check w\ measurement

#endif /* LST_CONSTANTS_H_ */
