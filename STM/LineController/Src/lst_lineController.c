/*
 * lst_lineControllerNucleo.c
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#include "lst_lineController.h"

void entry_point(void)
{

	while (1)
	{

		//test_timer();

		main_function();

		//test_LedDrivers();

		//debug_driveAddress();

	}

}

void main_function(void)
{

	read_line();

	//evaluate_line();

	//send_data();

}



