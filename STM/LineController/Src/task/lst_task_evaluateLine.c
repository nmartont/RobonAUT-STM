/*
 * task_evaluateLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include <task/lst_task_evaluateLine.h>

void lst_eval_line()
{

	// Step 0 - Initialize values

	lst_simpleEval_sum = 0;
	lst_simpleEval_weightedSum = 0;

	lst_simpleEval_firstLargePos = 127;
	lst_simpleEval_lastLargePos = 127;

	lst_simpleEval_firstSumPos = 0;
	lst_simpleEval_lastSumPos = 0;
	lst_simpleEval_firstPosMin = 0;
	lst_simpleEval_lastPosMax = 0;

	// Step 1 - Find first and last large value

	for (uint8_t i=0; i<32; i++)
	{

		if (lst_tcrt_values[i] > LST_SIMPLEEVAL_HIGH_THRESHOLD)
		{

			// Last large value position
			lst_simpleEval_lastLargePos = i;

			// First large value position
			if (lst_simpleEval_firstLargePos == 127)
			{

				lst_simpleEval_firstLargePos = i;

			}

		}

	}

	// Step 2 - Find first and last value that is
	// included in the calculation

	if (lst_simpleEval_firstLargePos != 127) // Line found
	{

		// Extend the summing range downwards

		// Include only those below the large value by maximum 2
		if (lst_simpleEval_firstLargePos > 1)
			lst_simpleEval_firstPosMin = lst_simpleEval_firstLargePos - 2;
		else
			lst_simpleEval_firstPosMin = 0;

		// Default: start summing at first large value
		lst_simpleEval_firstSumPos = lst_simpleEval_firstLargePos;

		// If the first large position is at 0 this part is not needed
		if (lst_simpleEval_firstLargePos == 0)
		{
			// Do nothing, firstSumPos is 0
		}

		// Extend summing range (lowest)
		// (decreasing values in 2 sensors range)
		else
		{

			for (uint8_t i=lst_simpleEval_firstLargePos - 1; i>=0; i--)
			{

				if ((lst_tcrt_values[i] < lst_tcrt_values[i+1]) &&
						(i >= lst_simpleEval_firstPosMin))
				{

					lst_simpleEval_firstSumPos--;

				}
				else break;

			}

		}

		// Extend the summing range upwards

		if (lst_simpleEval_lastLargePos < 30)
			lst_simpleEval_lastPosMax = lst_simpleEval_lastLargePos + 2;
		else
			lst_simpleEval_lastPosMax = 31;

		// Default: finish summing at the last large value
		lst_simpleEval_lastSumPos = lst_simpleEval_lastLargePos;

		// If last large position is at 31 this part is not needed
		if (lst_simpleEval_lastLargePos == 31)
		{

			// Do nothing, lastSumPos is 31

		}
		// Extend summing range (highest)
		// (decreasing values in 2 sensors range)
		else
		{

			for (uint8_t i=lst_simpleEval_lastLargePos + 1; i<=31; i++)
			{

				if ((lst_tcrt_values[i] < lst_tcrt_values[i-1]) &&
						(i <= lst_simpleEval_lastPosMax))
				{

					lst_simpleEval_lastSumPos++;

				}
				else break;

			}

		}

		// Step 3 - Do the averaging in the selected range

		// Calculate numerator, denominator
		for (uint8_t i=lst_simpleEval_firstSumPos;
				i<lst_simpleEval_lastSumPos;
				i++)
		{

			lst_simpleEval_weightedSum += i * lst_tcrt_values[i];
					lst_simpleEval_sum += lst_tcrt_values[i];

		}

		// Calculate weighted average
		// 5 -> 16 bit resolution shift
		// position calculation
		// uint -> int conversion
		lst_simpleEval_lineEstimate = ((uint16_t)
				((float) lst_simpleEval_weightedSum
				* LST_LINEEVAL_BASIC_MULTIPLIER
				/ (float) lst_simpleEval_sum));

		// Fill output variable
		for (uint8_t i=0; i<3; i++)
		{

			lst_eval_subPositions[i] = lst_simpleEval_lineEstimate;

		}

		lst_eval_line_countLines();

	}
	else // Send middle position if no line found
	{

		lst_eval_lineCount = 0;

		for (uint8_t i=0; i<3; i++)
		{

			// Set middle position if no line is found
			lst_eval_subPositions[i] = 0x8000;

		}

	}

}

void lst_eval_line_countLines()
{

	lst_eval_lineCount = 0;
	lst_eval_wasLarge = 0;

	for (uint8_t i=0; i<31; i++)
	{

		if (lst_tcrt_values[i] > LST_SIMPLEEVAL_HIGH_THRESHOLD)
		{

			lst_eval_wasLarge = 1;

		}

		if ((lst_tcrt_values[i] < LST_SIMPLEEVAL_HIGH_THRESHOLD) &&
				(lst_eval_wasLarge))
		{

			lst_eval_lineCount++;
			lst_eval_wasLarge = 0;

		}

	}

}

void lst_evaluate_line(void)
{

#ifdef LST_LINEEVAL_ADVANCED
	lst_eval_line_advanced();
#else
	//lst_eval_line_simple();
	lst_eval_line();
#endif

}


/*

void lst_eval_init_values(void)
{

	lst_eval_largeValues_size = 0;
	lst_eval_previousPosition = -1;

	for (uint8_t i=0; i<32; i++)
	{

		lst_eval_largeValues[i] = 0;

	}

}

void lst_eval_line_simple()
{

	// TODO check

	// Initialize sum
	lst_simpleEval_sum = 0;
	lst_simpleEval_weightedSum = 0;

	// Calculate weighted sum
	uint8_t found_one = 0;
	for (uint8_t i=0; i<32; i++)
	{

		if (lst_tcrt_values[i] > 20)
		{

			found_one = 1;
			lst_simpleEval_weightedSum += i * lst_tcrt_values[i];
			lst_simpleEval_sum += lst_tcrt_values[i];

		}

	}

	// Calculate weighted average
	// 5 -> 16 bit resolution shift
	// position calculation
	// uint -> int conversion
	lst_simpleEval_lineEstimate = ((uint16_t)
			((float) lst_simpleEval_weightedSum
			* LST_LINEEVAL_BASIC_MULTIPLIER
			/ (float) lst_simpleEval_sum));


	// Fill output variable
	if (found_one)
	{

		for (uint8_t i=0; i<3; i++)
		{

			lst_eval_subPositions[i] = lst_simpleEval_lineEstimate;

		}

	}
	else
	{

		for (uint8_t i=0; i<3; i++)
		{

			// Set middle position if no line is found
			lst_eval_subPositions[i] = 0x8000;

		}

	}

}

void lst_eval_line_advanced()
{

	lst_eval_init_values();

	lst_eval_algorithm_findMaxima();

	lst_eval_calculate_subSensor_positions();

}

void lst_eval_algorithm_findMaxima(void)
{

	// Step 1 - Find values that are sufficiently large

	for (uint8_t i=0; i<32; i++)
	{

		if (lst_tcrt_values[i] > LST_LINEEVAL_ADVANCED_THR_HIGH)
		{

			lst_eval_largeValues[lst_eval_largeValues_size] = i;
			lst_eval_largeValues_size++;

		}

	}

	// Step 2 - Find the maximum in each separate group of large values

	if (lst_eval_largeValues_size > 0)
	{

		lst_eval_previousPosition = lst_eval_largeValues[0];

		for (uint8_t i=0; i<lst_eval_largeValues_size; i++)
		{
			// if last large value was older than last index,
			//while (lst_eval_largeValues[i])

		}

	}




	// Find global maximum

	for (int i=0; i<32; i++)
	{

		if (lst_tcrt_values[i] > lst_eval_maximum_global)
		{

			lst_eval_maximum_global = lst_tcrt_values[i];

		}

	}

	// Find local maxima

	// Edge 1
	if (lst_tcrt_values[0] > lst_tcrt_values[1])
	{

		lst_eval_localMaxima1[lst_eval_localMaxima1_size] = 0;
		lst_eval_localMaxima1_size++;

	}

	// Sensors in the middle
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

	// Filter out those in the allowed range compared to the global maximum

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
				lst_eval_localMaxima2_size++;

			}

		}

	}

	// Check if the maxima are sufficiently salient values

	// Iterate through previous local maximum positions
	for (int i=0; i<lst_eval_localMaxima2_size; i++)
	{

		lst_eval_salient_ok = 0;

		// Iterate through the neighbours of the local maximum
		for (int 	j = 	lst_eval_localMaxima2[i] - LST_LINEEVAL_BASIC_MAX_PRX;
							j <= 	lst_eval_localMaxima2[i] + LST_LINEEVAL_BASIC_MAX_PRX;
							j++)
		{

			// Skip if the targeted neighbour is outside [0, 31]
			if ((j >=0) && (j<=31))
			{

				if (lst_tcrt_values[lst_eval_localMaxima2[i]] >
						(lst_tcrt_values[j] + LST_LINEEVAL_BASIC_MAX_THR))
				{

					// If at least one sufficiently low value is found, maximum is OK
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

	// TODO write function (subSensor)



}

*/
