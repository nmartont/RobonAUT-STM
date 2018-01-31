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
#define LST_ENCODER_TICKS_TIMEBASE          (LST_TASK_ENCODER_TICKS / LST_RTOS_TICKS_PER_MS)
#define LST_ENCODER_SPEED_CALC_TICKS       	(LST_TASK_ENCODER_TICKS / LST_RTOS_TICKS_PER_MS)

#define LST_ENCODER_DIST_MEAS_ONGOING       0
#define LST_ENCODER_DIST_MEAS_FINISHED      1
#define LST_ENCODER_DIST_MEAS_NOT_STARTED   2

#define LST_ENCODER_INCR_MM                 1.0f

// Functions
void LST_Encoder_CalculateSpeed();
uint8_t LST_Encoder_MeasureDistance(float dist_mm);

#endif /* TASKS_LST_ENCODER_H_ */
