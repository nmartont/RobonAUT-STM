/*
 * init.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "lineController.h"

// Called from main after HAL initialization
void initialize_controller(void)
{

	main_controller_login();

	while (1)
	{

		main_task();

	}

}

// Signal mainController that the lineSensor is active
void main_controller_login()
{



}

void main_task(void)
{

	read_line();

	evaluate_line();

	send_data();

}
