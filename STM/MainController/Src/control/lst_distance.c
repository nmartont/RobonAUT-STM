/*
 * lst_distance.c
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#include <control/lst_distance.h>

// Private variables
uint8_t lst_ongoing = 0;
int32_t lst_goal = 0;
uint8_t lst_direction = 0;

// Returns 1 when started and finished, returns 0 if ongoing
uint8_t LST_Distance_Measure_mm(int16_t mm)
{

	// New measurement
	if (!lst_ongoing)
	{

		// Set to ongoing measurement
		lst_ongoing = 1;

		// Save direction
		if (mm < 0) lst_direction = 1;
		else lst_direction = 0;

		// Set the goal distance
		lst_goal =
				lst_encoder_distance_um + (int32_t) mm * 1000;

		return 0;

	}
	// Continue measurement
	else
	{

		// Reached destination
	  if (lst_direction)
	  {

	    if (lst_encoder_distance_um <= lst_goal)
      {

        lst_ongoing = 0;

        return 1;

      }

	  }
	  else
	  {

	    if (lst_encoder_distance_um >= lst_goal)
      {

        lst_ongoing = 0;

        return 1;

      }

	  }

	}

	return 0;

}

// Can be called if suspected to be stuck or has to be cancelled
// due to reaching other condition (e.g. wall detected)
void LST_Distance_Measure_mm_Reset()
{

	lst_ongoing = 0;

}

// Check if an ongoing measurement exists
uint8_t LST_Distance_Measure_mm_GetOngoing()
{

	return lst_ongoing;

}
