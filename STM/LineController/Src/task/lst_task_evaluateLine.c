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
