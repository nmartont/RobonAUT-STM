/*
 * lst_steering.c
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#include "control/lst_steering.h"

uint8_t lst_steering_sharp_dir = 0; // 0: left; 1: right
uint16_t lst_steering_distance = 0;

void LST_Steering_Set()
{

	switch (lst_steering_type)
		{

		case LST_STEERING_LOCKED:
			lst_control_steering = lst_steering_lockedPosition;
			break;

		case LST_STEERING_BTTEST:
			lst_control_steering = LST_Control_Servo_BT();
			break;

		case LST_STEERING_SHARP:
		  lst_control_steering = LST_Control_SteeringControllerSharp(
		      lst_steering_sharp_dir, lst_steering_distance);
		  break;

		case LST_STEERING_FOLLOW:
		default:
			lst_control_steering = LST_Control_SteeringController(0);

		}

}

void LST_Steering_Lock(int16_t steeringValue)
{

	lst_steering_type = LST_STEERING_LOCKED;
	lst_steering_lockedPosition = steeringValue;

}

void LST_Steering_Follow()
{

	lst_steering_type = LST_STEERING_FOLLOW;

}

void LST_Steering_Sharp(uint8_t sharp_dir, uint16_t distance)
{

  lst_steering_type = LST_STEERING_SHARP;
  lst_steering_sharp_dir = sharp_dir;
  lst_steering_distance = distance;

}

void LST_Steering_BTTest()
{

	lst_steering_type = LST_STEERING_BTTEST;

}
