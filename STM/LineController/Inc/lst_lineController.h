/*
 * lst_lineControllerNucleo.h
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_LINECONTROLLERNUCLEO_H_
#define LST_LINECONTROLLERNUCLEO_H_

// Includes

#include <function/lst_function_timer.h> // Timer Test
#include <task/lst_task_displayLine.h>
#include <task/lst_task_evaluateLine.h>
#include <task/lst_task_readLine.h>
#include <task/lst_task_sendData.h>
#include <task/lst_task_testOnly.h>
#include <task/lst_task_testOnly.h>

// Functions

/**
 * \brief
 * 	Calls the function that needs to be executed on the microcontroller.
 * 	Use it to set a Test or the Main function.
 */
void lst_entry_point(void);

/**
 * \brief
 * 	Executes the RACE FUNCTION.
 */
void lst_main_function(void);

#endif /* LST_LINECONTROLLERNUCLEO_H_ */
