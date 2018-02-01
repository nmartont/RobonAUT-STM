/*
 * lst_steering.c
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#include "control/lst_steering.h"

void LST_Steering_Set()
{

	switch (lst_steering_type)
		{

		case LST_STEERING_LOCKED:
			lst_control_steering = lst_steering_lockedPosition;
			break;

		case LST_STEERING_BTTEST:
			// TODO steering BT control function
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
