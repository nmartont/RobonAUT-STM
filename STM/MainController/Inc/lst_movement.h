/*
 * lst_distance.h
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#ifndef LST_MOVEMENT_H_
#define LST_MOVEMENT_H_

#include "stm32f4xx_hal.h"
#include "tasks/lst_encoder.h"

// Functions
uint8_t lst_movement_measure_mm(uint16_t mm);

#endif /* LST_MOVEMENT_H_ */
