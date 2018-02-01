/*
 * lst_movement.c
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#include "control/lst_movement.h"

void LST_Movement_Set()
{

	switch (lst_movement_type)
	{

	case LST_MOVEMENT_SPEEDCONTROL:
		lst_control_motor = lst_movement_speed;
		break;

	case LST_MOVEMENT_SPEEDCONTROL_FEEDBACK:
		lst_control_motor = LST_Control_SpeedController(lst_movement_speed);
		break;

	case LST_MOVEMENT_STOP:
	default:
		lst_control_motor = 0;

	}

}

void LST_Movement_Move(uint16_t speed)
{

	lst_movement_speed = speed;
	lst_movement_type = LST_MOVEMENT_SPEEDCONTROL_FEEDBACK;

}

void LST_Movement_Stop()
{

	lst_movement_type = LST_MOVEMENT_STOP;

}
