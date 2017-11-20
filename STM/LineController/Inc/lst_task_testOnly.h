/*
 * lst_task_testOnly.h
 *
 *  Created on: 16 Nov 2017
 *      Author: Balazs
 */

#ifndef LST_TASK_TESTONLY_H_
#define LST_TASK_TESTONLY_H_

#include "lst_constants.h"
#include "lst_function_gpio.h"
#include "lst_function_timer.h"

extern SPI_HandleTypeDef hspi2;

void test_LedDrivers(void);

#endif /* LST_TASK_TESTONLY_H_ */
