/*
 * lst_sharp.h
 *
 *  Created on: 30 Jan 2018
 *      Author: Balazs
 */

#ifndef LST_SHARP_H_
#define LST_SHARP_H_

// TODO Refactor according to naming conventions

// Includes

#include "stm32f4xx_hal.h"
#include "lst_adc.h"

// The maximum values out of the distance request functions
//#define LST_SHARP_DIST_LONGRANGE_MAX 			2200
//#define LST_SHARP_DIST_SHORTRANGE_MAX 		380

// Functions

uint16_t LST_Sharp_GetLeftDistance();
uint16_t LST_Sharp_GetFrontDistance();
uint16_t LST_Sharp_GetRightDistance();

#endif /* LST_SHARP_H_ */
