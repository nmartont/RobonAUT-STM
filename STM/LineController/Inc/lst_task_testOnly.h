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
#include "lst_function_spi.h"

extern SPI_HandleTypeDef hspi2;

int a;

void test_LedDrivers(void);
void debug_driveAddress(void);

void test_timer(void);
/*
 * @Description
 * 	Tests the custom-made timer functions.
 */

#endif /* LST_TASK_TESTONLY_H_ */
