/*
 * lst_steering.h
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#ifndef CONTROL_LST_STEERING_H_
#define CONTROL_LST_STEERING_H_

#include "stm32f4xx_hal.h"

// Constants
#define LST_STEERING_FOLLOW 0
#define LST_STEERING_LOCKED 1
#define LST_STEERING_BTTEST 2

// Variables

// Servo control
int16_t lst_control_steering;

// Steering controls
uint8_t 	lst_steering_type;
int16_t		lst_steering_lockedPosition;

// Functions
void LST_Steering_Set();
void LST_Steering_Lock(int16_t steeringValue);
void LST_Steering_Follow();

#endif /* CONTROL_LST_STEERING_H_ */
