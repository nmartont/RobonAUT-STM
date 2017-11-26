/*
 * lst_lineControllerNucleo.c
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#include "lst_lineController.h"

void lst_entry_point(void)
{

	while (1)
	{

		lst_main_function();

		//test_timer();

		//test_LedDrivers();

		//debug_driveAddress();

	}

}

void lst_main_function(void)
{

	lst_read_line();

	lst_display_line(LST_LINEDISPLAY_RAW_THRESHOLD);

	//lst_evaluate_line();

	//lst_send_data();

}



