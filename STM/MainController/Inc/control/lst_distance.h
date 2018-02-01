/*
 * lst_distance.h
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#ifndef CONTROL_LST_DISTANCE_H_
#define CONTROL_LST_DISTANCE_H_

#include "stm32f4xx_hal.h"
#include "tasks/lst_encoder.h"

// Functions
uint8_t lst_distance_measure_mm(uint16_t mm);
void lst_distance_measure_mm_reset();
uint8_t lst_distance_measure_mm_getOngoing();

// External variables
extern int32_t lst_encoder_distance_um;

#endif /* CONTROL_LST_DISTANCE_H_ */
