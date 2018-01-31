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

// Functions
void LST_Encoder_CalculateSpeed();

#endif /* TASKS_LST_ENCODER_H_ */
