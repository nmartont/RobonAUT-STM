/*
 * lst_steering.h
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#ifndef CONTROL_LST_STEERING_H_
#define CONTROL_LST_STEERING_H_

#include "stm32f4xx_hal.h"
#include "lst_control.h"

// Constants
#define LST_STEERING_FOLLOW 0
#define LST_STEERING_LOCKED 1
#define LST_STEERING_BTTEST 2
#define LST_STEERING_SHARP  3

// Variables

// Servo control
int16_t lst_control_steering;

// Steering controls
uint8_t 	lst_steering_type;
int16_t		lst_steering_lockedPosition;

// Steering watch


// Functions
void LST_Steering_Set();
void LST_Steering_Lock(int16_t steeringValue);
void LST_Steering_Follow();
void LST_Steering_BTTest();
void LST_Steering_Sharp(uint8_t sharp_dir, uint16_t distance_mm);

#endif /* CONTROL_LST_STEERING_H_ */
