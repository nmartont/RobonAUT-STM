/*
 * lst_task_displayLine.h
 *
 *  Created on: 23 Nov 2017
 *      Author: Balazs
 */

#ifndef TASK_LST_TASK_DISPLAYLINE_H_
#define TASK_LST_TASK_DISPLAYLINE_H_

// Includes

#include <task/lst_task_readLine.h>
#include "lst_constants.h"

// Variables
uint32_t lst_feedbackLed_output;

// Functions

void lst_displayLine(uint8_t threshold);

#endif /* TASK_LST_TASK_DISPLAYLINE_H_ */
