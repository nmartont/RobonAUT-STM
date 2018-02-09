/*
 * lst_movement.c
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#include "control/lst_movement.h"

// Private functions
void LST_Movement_LockReverse();
void LST_Movement_UnlockReverse();

uint16_t lst_movement_distance = 0;
int16_t lst_movement_sharp_speed_max = 95;
int16_t lst_movement_sharp_speed_min = 0;

int32_t speed_input = 0;

void LST_Movement_Set()
{
	// Handle reverse start
	if (lst_movement_reverse_start){
		lst_movement_reverse_counter = 0;
		lst_movement_repetition_counter = 0;
		lst_movement_reverse_stage = LST_MOVEMENT_REVERSE_NUL;
		lst_movement_reverse_start = 0;
	}

	// Handle reverse transition
	switch (lst_movement_reverse_stage)
	{

	// First null pulse
	case LST_MOVEMENT_REVERSE_NUL:
		if (lst_movement_reverse_counter < LST_MOVEMENT_REVERSE_COUNT)
		{

			lst_movement_reverse_counter++;
			lst_control_motor = 0;

		}
		else
		{

			lst_movement_reverse_stage = LST_MOVEMENT_REVERSE_NEG;
			lst_movement_reverse_counter = 0;

		}
		return; // Don't enter next switch block

	// First negative pulse
	case LST_MOVEMENT_REVERSE_NEG:
		if (lst_movement_reverse_counter < LST_MOVEMENT_REVERSE_COUNT)
		{

			lst_movement_reverse_counter++;
			lst_control_motor = LST_MOVEMENT_REVERSE_VALUE;

		}
		else
		{

			// End of reverse switch check
			if (lst_movement_repetition_counter > LST_MOVEMENT_REPETITION_COUNT)
			{

				lst_movement_reverse_stage = LST_MOVEMENT_REVERSE_NONE;

			}
			else
			{

				lst_movement_repetition_counter++;
				lst_movement_reverse_stage = LST_MOVEMENT_REVERSE_NUL;
				lst_movement_reverse_counter = 0;

			}

		}
		return; // Don't enter next switch block

	case LST_MOVEMENT_REVERSE_NONE:
	default:
		break;
	}

	switch (lst_movement_type)
	{

	case LST_MOVEMENT_SPEEDCONTROL:
		lst_control_motor = lst_movement_speed;
		break;

	case LST_MOVEMENT_SPEEDCONTROL_FEEDBACK:

		// Lock speed control to reverse values only
		if (lst_movement_speed < 0) LST_Movement_LockReverse(); // Useless
		else LST_Movement_UnlockReverse();                      // Useless

		// Call speed controller
		lst_control_motor = LST_Control_SpeedController(lst_movement_speed);
		break;

	case LST_MOVEMENT_SPEEDCONTROL_SHARP:
	  // Call sharp controller (output is ENCODER SPEED)
    speed_input = LST_MOVEMENT_SHARP_COEFF * LST_Control_SpeedControllerSharp(lst_movement_distance);
    if(speed_input < lst_movement_sharp_speed_min) speed_input = lst_movement_sharp_speed_min;
    if(speed_input > lst_movement_sharp_speed_max) speed_input = lst_movement_sharp_speed_max;

    // Call encoder controller
    lst_control_motor = LST_Control_SpeedController(speed_input);
	  break;

	case LST_MOVEMENT_STOP:
	default:
		lst_control_motor = 0;

	}

	// Reverse handling
  if ((lst_control_motor < 0) && (!lst_movement_reverse_ongoing))
  {

    lst_movement_reverse_start = 1;
    lst_movement_reverse_ongoing = 1;

  }

  // If a positive value is given, reverse has to be initiated
  // again next time
  if (lst_control_motor > 0) lst_movement_reverse_ongoing = 0;

}

void LST_Movement_Move(int16_t speed)
{

	lst_movement_speed = speed;
	lst_movement_type = LST_MOVEMENT_SPEEDCONTROL_FEEDBACK;

}

void LST_Movement_Move_Sharp(uint16_t distance)
{

  lst_movement_distance = distance;
  lst_movement_type = LST_MOVEMENT_SPEEDCONTROL_SHARP;

}

void LST_Movement_Move_Encoderless(int16_t speed)
{

	lst_movement_speed = speed;
	lst_movement_type = LST_MOVEMENT_SPEEDCONTROL;

}

void LST_Movement_Stop()
{

	lst_movement_type = LST_MOVEMENT_STOP;

}

// ToDo useless functions
void LST_Movement_LockReverse()
{

	lst_control_speed_reverseLock = 1;

}

void LST_Movement_UnlockReverse()
{

	lst_control_speed_reverseLock = 0;

}
