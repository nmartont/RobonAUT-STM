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

// External variables

extern uint8_t lst_tcrt_values[32];

// Local variables

/**
 * \var lst_eval_subPositions[3] Contains the center of all the lines (TEMPORARILY).
 *
 *  ORIGINAL: Contains the subsensor positions of the three possible lines.
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

uint8_t lst_eval_lineCount;
uint8_t lst_eval_wasLarge;

uint8_t lst_simpleEval_firstLargePos;
uint8_t lst_simpleEval_lastLargePos;

uint8_t lst_simpleEval_firstSumPos;
uint8_t lst_simpleEval_lastSumPos;

uint8_t lst_simpleEval_firstPosMin;
uint8_t lst_simpleEval_lastPosMax;

// Function declarations

/**
 * \brief
 *	Evaluates line data based on the results from
 *	read_line(). Calls the selected evaluation algorithm.
 */
void lst_evaluate_line(void);

/**
 * \brief
 *  Uses weighted average to calculate the line position.
 */
void lst_eval_line(void);

/**
 * \brief
 *  Counts the number of peaks in the TCRT values to get the number
 *  of lines.
 */
void lst_eval_line_countLines(void);


/* OLD algorithm variables
uint8_t lst_eval_largeValues[32];
uint8_t lst_eval_largeValues_size;

int8_t lst_eval_previousPosition;
uint8_t lst_eval_maximumValues[32];

*/


#endif /* TASK_LST_TASK_EVALUATELINE_H_ */
