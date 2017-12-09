/*
 * lst_lineControllerNucleo.c
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#include <lst_lineControllerNucleo.h>

void lst_entry_point(void)
{

	lst_main_function();

}

void lst_main_function(void)
{

	lst_init_functions();

	while (1)
	{

		lst_readLine();

		lst_evaluate_line();

		lst_sendData();

		lst_displayLine(LST_LINEDISPLAY_RAW_THRESHOLD);

	}

}

void lst_init_functions()
{

	lst_sendData_init();

	lst_readLine_init();

}


