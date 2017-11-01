/*
 * lst_constants.h
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

// STM-related constants
#define LST_F_CPU 64000000UL
/*
 * @Warning
 * 	Timer functions work correctly only below CPU frequencies of 64MHz
 */

// Debug constants
#define LST_NUCLEO_TEST 1
/*
 * @Warning
 * 	Set to 1 if code is run on NUCLEO, set to 0 if run on STM32F3
 */
