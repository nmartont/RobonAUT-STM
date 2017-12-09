/*
 * lst_task_testOnly.h
 *
 *  Created on: 16 Nov 2017
 *      Author: Balazs
 */

#ifndef TASK_LST_TASK_TESTONLY_H_
#define TASK_LST_TASK_TESTONLY_H_

// Includes

#include <function/lst_function_gpio.h>
#include <function/lst_function_spi.h>
#include <function/lst_function_timer.h>
#include "lst_constants.h"

// External variables

extern SPI_HandleTypeDef hspi2;

// Local variables

int a;

// Function declarations

void test_LedDrivers(void);
void debug_driveAddress(void);

/**
 * \brief
 * 	Tests the custom-made timer functions.
 */
void test_timer(void);

#endif /* TASK_LST_TASK_TESTONLY_H_ */
