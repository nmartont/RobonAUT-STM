/*
 * lst_encoder.h
 *
 *  Created on: 31 Jan 2018
 *      Author: Balazs
 */

#ifndef TASKS_LST_ENCODER_H_
#define TASKS_LST_ENCODER_H_

#include "stm32f4xx_hal.h"
#include "lst_timer.h"
#include "lst_control.h"
#include "lst_task.h"

// Constants
#define LST_ENCODER_SPEED_FILTER_ORDER  		3
#define LST_ENCODER_MULTIPLIER							5 // WORKS W/ 2ms TASK CYCLE TIME

#define LST_ENCODER_DIST_MEAS_ONGOING       0
#define LST_ENCODER_DIST_MEAS_FINISHED      1
#define LST_ENCODER_DIST_MEAS_NOT_STARTED   2

// Encoder increment per micrometer
#define LST_ENCODER_INCR_UM                 25.62f // WORKS W/ 2ms TASK CYCLE TIME

// Functions
void LST_Encoder_Calculate();
uint8_t LST_Encoder_MeasureDistance(float dist_mm);

// Variables
float lst_encoder_speed; // Current speed
// Distance measured in micrometers
int32_t lst_encoder_distance_um; // Distance traversed [mm] since last reset

#endif /* TASKS_LST_ENCODER_H_ */
