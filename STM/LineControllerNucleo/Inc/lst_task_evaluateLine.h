/*
 * task_evaluateLine.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_TASK_EVALUATELINE_H_
#define LST_TASK_EVALUATELINE_H_

// Includes
#include "lst_constants.h"

// Local variables

uint8_t lst_maximum_global;
/*
 * @Description
 * 	Stores the maximum of the detected values.
 */

uint8_t lst_local_maxima1[16];
/*
 * @Description
 * 	Stores the positions of the local maxima, for which the previous
 * 	and next value is	smaller than itself.
 */

uint8_t lst_local_maxima2[16];
/*
 * @Description
 * 	Stores the positions of the local maxima, for which the previous
 * 	and next value is	smaller than itself and are in the range of
 * 	[(lst_maximum_global - LST_LINEEVAL_GLOBALMAX_THR); lst_maximum_global].
 */

uint8_t lst_position_maxima[8];
/*
 * @Description
 * 	Stores the positions of the detected valid maximum values.
 */

// External variables
extern uint8_t lst_tcrt_values[32];

// Function declarations

void evaluate_line(void);
/*
 * @Description
 *	Evaluates line data based on the results from
 *	read_line(). Calls the selected evaluation algorithm.
 */

void init_values(void);
/*
 * @Description
 * 	Clears local variables before evaluating line sensor data.
 */

void algorithm_basic_findMaxima(void);
/*
 * @Description
 * 	Finds the positions of sensor value maxima with several iterations
 * 	through the sensor data array.
 */

#endif /* LST_TASK_EVALUATELINE_H_ */
