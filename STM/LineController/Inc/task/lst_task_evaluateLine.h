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
 *  TODO
 */
uint8_t lst_eval_largeValues[32];
uint8_t lst_eval_largeValues_size;

int8_t lst_eval_previousPosition;
uint8_t lst_eval_maximumValues[32];


/**
 * \brief
 * 	TODO doxy
 */
uint16_t lst_eval_subPositions[3];

/**
 * \var lst_simpleEval_sum Sum of sensor values.
 * \var lst_simpleEval_weightedSum Sum of sensor values multiplied by positions.
 * \var lst_simpleEval_lineEstimate Evaluated line position.
 */
uint16_t lst_simpleEval_sum;
uint32_t lst_simpleEval_weightedSum;
uint16_t lst_simpleEval_lineEstimate;

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
 *  Uses complex technique to distinct separate measured lines.
 */
void lst_eval_line_advanced();

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
void lst_eval_algorithm_findMaxima(void);

/**
 * \brief
 * 	Approximates the position of the detected lines.
 */
void lst_eval_calculate_subSensor_positions(void);

/**
 * \brief
 *  Uses weighted average to calculate the line position.
 */
void lst_eval_line_simple(void);

void lst_eval_line_simple2(void);

uint8_t lst_simpleEval_firstLargePos;
uint8_t lst_simpleEval_lastLargePos;

uint8_t lst_simpleEval_firstSumPos;
uint8_t lst_simpleEval_lastSumPos;

uint8_t lst_simpleEval_firstPosMin;
uint8_t lst_simpleEval_lastPosMax;


#endif /* TASK_LST_TASK_EVALUATELINE_H_ */
