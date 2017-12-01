/*
 * task_evaluateLine.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#ifndef TASK_LST_TASK_EVALUATELINE_H_
#define TASK_LST_TASK_EVALUATELINE_H_

// Includes

#include "lst_constants.h"

// Local variables

/**
 * \brief
 * 	Stores the maximum of the detected values.
 */
uint8_t lst_eval_maximum_global;

/**
 * \brief
 * 	Stores the size of the corresponding array
 */
uint8_t lst_eval_localMaxima1_size;

/**
 * \brief
 * 	Stores the size of the corresponding array
 */
uint8_t lst_eval_localMaxima2_size;

/**
 * \brief
 * 	Stores the size of the corresponding array
 */
uint8_t lst_eval_localMaxima3_size;

/**
 * \brief
 * 	Stores the positions of the local maxima, for which the previous
 * 	and next value is	smaller than itself.
 */
uint8_t lst_eval_localMaxima1[16];

/**
 * \brief
 * 	Stores the positions of the local maxima, for which the previous
 * 	and next value is	smaller than itself and are in the range of
 * 	[(lst_maximum_global - LST_LINEEVAL_GLOBALMAX_THR); lst_maximum_global].
 */
uint8_t lst_eval_localMaxima2[16];

/**
 * \brief
 * 	Stores the positions of the detected valid maximum values.
 */
uint8_t lst_eval_localMaxima3[16];

uint8_t lst_eval_salient_ok;

/**
 * \brief
 * 	TODO doxy
 */
uint16_t lst_eval_subPositions[3];

// External variables

extern uint8_t lst_tcrt_values[32];

// Function declarations

/**
 * \brief
 *	Evaluates line data based on the results from
 *	read_line(). Calls the selected evaluation algorithm.
 */
void lst_evaluate_line(void);

/**
 * \brief
 * 	Clears local variables before evaluating line sensor data.
 */
void lst_eval_init_values(void);

/**
 * \brief
 * 	Finds the positions of sensor value maxima with several iterations
 * 	through the sensor data array.
 */
void lst_eval_algorithm_01_findMaxima(void);

/**
 * \brief
 * 	Approximates the position of the detected lines.
 */
void lst_eval_calculate_subSensor_positions(void);

// TODO:temp
void lst_evaluate_line_simple(void);

#endif /* TASK_LST_TASK_EVALUATELINE_H_ */
