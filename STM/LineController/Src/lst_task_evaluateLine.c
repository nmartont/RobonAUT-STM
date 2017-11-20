/*
 * task_evaluateLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "lst_task_evaluateLine.h"

void lst_evaluate_line(void)
{

	lst_eval_init_values();

#ifdef LST_LINEEVAL_ADVANCED
	lst_algorithm_02_findMaxima();
#else
	lst_eval_algorithm_01_findMaxima();
#endif

	lst_eval_calculate_subSensor_positions();

}

void lst_eval_init_values(void)
{

	lst_eval_maximum_global = 0;

	lst_eval_localMaxima1_size = 0;
	lst_eval_localMaxima2_size = 0;
	lst_eval_localMaxima3_size = 0;

	for (int i=0; i<16; i++)
	{
		lst_eval_localMaxima1[i] = 0;
		lst_eval_localMaxima2[i] = 0;
		lst_eval_localMaxima3[i] = 0;
	}

}

void lst_eval_algorithm_01_findMaxima(void)
{

	/* Find global maximum */

	for (int i=0; i<32; i++)
	{

		if (lst_tcrt_values[i] > lst_eval_maximum_global)
		{

			lst_eval_maximum_global = lst_tcrt_values[i];

		}

	}

	/* Find local maxima */

	// Edge 1
	if (lst_tcrt_values[0] > lst_tcrt_values[1])
	{

		lst_eval_localMaxima1[lst_eval_localMaxima1_size] = 0;
		lst_eval_localMaxima1_size++;

	}

	for (int i=1; i<31; i++)
	{

		if ((lst_tcrt_values[i] > lst_tcrt_values[i-1]) &&
				(lst_tcrt_values[i] > lst_tcrt_values[i+1]))
		{

			lst_eval_localMaxima1[lst_eval_localMaxima1_size] = i;
			lst_eval_localMaxima1_size++;

		}

	}

	// Edge 2
	if (lst_tcrt_values[31] > lst_tcrt_values[30])
	{

		lst_eval_localMaxima1[lst_eval_localMaxima1_size] = 31;
		lst_eval_localMaxima1_size++;

	}

	/* Filter out those in the allowed range compared to the global maximum */

	// If global maximum is smaller than the threshold, this step is skipped
	if (lst_eval_maximum_global <= LST_LINEEVAL_BASIC_GLOBALMAX_THR)
	{

		for (int i=0; i<lst_eval_localMaxima1_size; i++)
		{

			lst_eval_localMaxima2[i] = lst_eval_localMaxima1[i];

		}

	}
	else
	{

		for (int i=0; i<lst_eval_localMaxima1_size; i++)
		{

			if (lst_tcrt_values[lst_eval_localMaxima1[i]] >
					(lst_eval_maximum_global - LST_LINEEVAL_BASIC_GLOBALMAX_THR))
			{

				lst_eval_localMaxima2[lst_eval_localMaxima2_size] = lst_eval_localMaxima1[i];

			}

		}

	}

	/* Check if the maxima are appropriately salient values */

	for (int i=0; i<lst_eval_localMaxima2_size; i++)
	{

		lst_eval_salient_ok = 0;

		for (int j = (i - LST_LINEEVAL_BASIC_MAX_PRX);
				j <= (i + LST_LINEEVAL_BASIC_MAX_PRX);
				j++)
		{

			if ((j >=0) && (j<=31))
			{

				if (lst_tcrt_values[lst_eval_localMaxima2[i]] >
						(lst_tcrt_values[j] + LST_LINEEVAL_BASIC_MAX_THR))
				{

					lst_eval_salient_ok = 1;

				}

			}

		}

		if (lst_eval_salient_ok == 1)
		{

			lst_eval_localMaxima3[lst_eval_localMaxima3_size] =
					lst_eval_localMaxima2[i];
			lst_eval_localMaxima3_size++;

		}

	}

}

void lst_eval_calculate_subSensor_positions()
{

	// TODO

}
