/*
 * lst_distance.c
 *
 *  Created on: 1 Feb 2018
 *      Author: Balazs
 */

#include <control/lst_distance.h>

// Private variables
uint8_t lst_ongoing = 0;
uint8_t lst_goal = 0;

// Returns 1 when started and finished, returns 0 if ongoing
uint8_t lst_distance_measure_mm(uint16_t mm)
{

	// New measurement
	if (!lst_ongoing)
	{

		// Set to ongoing measurement
		lst_ongoing = 1;

		// Set the goal distance
		lst_goal =
				(float) lst_encoder_distance_um + mm * 1000;

		return 1;

	}
	// Continue measurement
	else
	{

		// Reached destination
		if (lst_encoder_distance_um >= lst_goal)
		{

			lst_ongoing = 0;

			return 1;

		}

	}

	return 0;

}

// Can be called if suspected to be stuck or has to be cancelled
// due to reaching other condition (e.g. wall detected)
void lst_distance_measure_mm_reset()
{

	lst_ongoing = 0;

}

// Check if an ongoing measurement exists
uint8_t lst_distance_measure_mm_getOngoing()
{

	return lst_ongoing;

}
