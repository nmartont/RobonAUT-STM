/*
 * task_evaluateLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "lst_task_evaluateLine.h"

// TODO

void evaluate_line(void)
{

	init_values();

#ifdef LST_LINEEVAL_ADVANCED
	algorithm_advanced_findMaxima();
#else
	algorithm_basic_findMaxima();
#endif


}

void init_values(void)
{

	lst_maximum_global = 0;

	for (int i=0; i<8; i++)
	{
		lst_position_maxima[i] = 0;
	}

}

void algorithm_basic_findMaxima(void)
{

	/* Find global maximum */

	for (int i=0; i<32; i++)
	{

		if (lst_tcrt_values[i] > lst_maximum_global)
		{

			lst_maximum_global = lst_tcrt_values[i];

		}

	}

	/* Find local maxima */

	uint8_t iterator_localMax = 0;

	// Edge 1
	if (lst_tcrt_values[0] > lst_tcrt_values[1])
	{

		lst_local_maxima1[iterator_localMax] = lst_tcrt_values[0];
		iterator_localMax++;

	}

	for (int i=1; i<31; i++)
	{

		if ((lst_tcrt_values[i] > lst_tcrt_values[i-1]) &&
				(lst_tcrt_values[i] > lst_tcrt_values[i+1]))
		{

			lst_local_maxima1[iterator_localMax] = lst_tcrt_values[i];
			iterator_localMax++;

		}

	}

	// Edge 2
	if (lst_tcrt_values[31] > lst_tcrt_values[30])
	{

		lst_local_maxima1[iterator_localMax] = lst_tcrt_values[31];
		iterator_localMax++;

	}

	// Size of local maxima array
	iterator_localMax--;

	/* Filter out those in the allowed range compared to the global maximum */

	// If global maximum is smaller than the threshold, this step is skipped
	if (lst_maximum_global <= LST_LINEEVAL_BASIC_GLOBALMAX_THR)
	{

		for (int i=0; i<iterator_localMax; i++)
		{

			lst_local_maxima2[i] = lst_local_maxima1[i];

		}

	}
	else
	{

		// TODO continue here

	}

}
