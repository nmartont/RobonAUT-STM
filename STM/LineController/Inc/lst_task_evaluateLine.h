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

uint8_t lst_eval_maximum_global;
/*
 * @Description
 * 	Stores the maximum of the detected values.
 */

uint8_t lst_eval_localMaxima1_size;
/*
 * @Description
 * 	Stores the size of the corresponding array
 */

uint8_t lst_eval_localMaxima2_size;
/*
 * @Description
 * 	Stores the size of the corresponding array
 */

uint8_t lst_eval_localMaxima3_size;
/*
 * @Description
 * 	Stores the size of the corresponding array
 */

uint8_t lst_eval_localMaxima1[16];
/*
 * @Description
 * 	Stores the positions of the local maxima, for which the previous
 * 	and next value is	smaller than itself.
 */

uint8_t lst_eval_localMaxima2[16];
/*
 * @Description
 * 	Stores the positions of the local maxima, for which the previous
 * 	and next value is	smaller than itself and are in the range of
 * 	[(lst_maximum_global - LST_LINEEVAL_GLOBALMAX_THR); lst_maximum_global].
 */

uint8_t lst_eval_localMaxima3[16];
/*
 * @Description
 * 	Stores the positions of the detected valid maximum values.
 */

uint8_t lst_eval_salient_ok;

// External variables
extern uint8_t lst_tcrt_values[32];

// Function declarations

void lst_evaluate_line(void);
/*
 * @Description
 *	Evaluates line data based on the results from
 *	read_line(). Calls the selected evaluation algorithm.
 */

void lst_eval_init_values(void);
/*
 * @Description
 * 	Clears local variables before evaluating line sensor data.
 */

void lst_eval_algorithm_01_findMaxima(void);
/*
 * @Description
 * 	Finds the positions of sensor value maxima with several iterations
 * 	through the sensor data array.
 */

void lst_eval_calculate_subSensor_positions(void);
/*
 * @Description
 * 	Approximates the position of the detected lines.
 */

#endif /* LST_TASK_EVALUATELINE_H_ */
