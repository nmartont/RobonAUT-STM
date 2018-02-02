/*
 * lst_movement.h
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#ifndef CONTROL_LST_MOVEMENT_H_
#define CONTROL_LST_MOVEMENT_H_

#include "stm32f4xx_hal.h"
#include "tasks/lst_encoder.h"
#include "lst_control.h"

// Constants
#define LST_MOVEMENT_STOP 0
#define LST_MOVEMENT_SPEEDCONTROL 1
#define LST_MOVEMENT_SPEEDCONTROL_FEEDBACK 2

// Speeds
#define LST_MOVEMENT_FB_SLOWEST 					60
#define LST_MOVEMENT_FB_SLOW							70
#define LST_MOVEMENT_FB_MEDIUM						100
#define LST_MOVEMENT_FB_BACKING_SLOWEST   -80

#define LST_MOVEMENT_BACKING_SLOW					-440

// Variables

// Reverse switch
uint8_t lst_movement_reverse_counter;
uint8_t lst_movement_repetition_counter;

uint8_t lst_movement_reverse_stage;
uint8_t lst_movement_reverse_start;
uint8_t lst_movement_reverse_ongoing;

#define LST_MOVEMENT_REVERSE_NONE		0
#define LST_MOVEMENT_REVERSE_NUL		1
#define LST_MOVEMENT_REVERSE_NEG		2

#define LST_MOVEMENT_REVERSE_COUNT			5
#define LST_MOVEMENT_REPETITION_COUNT		2
#define LST_MOVEMENT_REVERSE_VALUE			-500

// Motor control
int16_t lst_control_motor;

// Movement controls
uint8_t 	lst_movement_type;
int16_t	lst_movement_speed;

// Functions
void LST_Movement_Set();
void LST_Movement_Move(int16_t speed);
void LST_Movement_Stop();
void LST_Movement_Move_Encoderless(int16_t speed);

#endif /* CONTROL_LST_MOVEMENT_H_ */
