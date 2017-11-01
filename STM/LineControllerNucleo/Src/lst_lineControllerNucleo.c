/*
 * lst_lineControllerNucleo.c
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#include "lst_lineControllerNucleo.h"

void entryPoint(void)
{

	while (1)
	{

		testTimer();

	}

}

void testTimer(void)
{

	//timer1_delay_timClk(400);

	lst_timer1_delay_microSeconds(200);

}
