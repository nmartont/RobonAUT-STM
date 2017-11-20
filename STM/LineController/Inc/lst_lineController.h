/*
 * lst_lineControllerNucleo.h
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_LINECONTROLLERNUCLEO_H_
#define LST_LINECONTROLLERNUCLEO_H_

// Includes

#include "lst_function_timer.h" // Timer Test
#include "lst_task_readLine.h"
#include "lst_task_evaluateLine.h"
#include "lst_task_sendData.h"
#include "lst_task_testOnly.h"
#include "lst_task_testOnly.h"

// Variables
int a;

void entry_point(void);
/*
 * @Description
 * 	Calls the function that needs to be executed on the microcontroller.
 * 	Use it to set a Test or the Main function.
 */

void main_function(void);
/*
 * @Description
 * 	Executes the RACE FUNCTION.
 */

void test_timer(void);
/*
 * @Description
 * 	Tests the custom-made timer functions.
 */

#endif /* LST_LINECONTROLLERNUCLEO_H_ */
