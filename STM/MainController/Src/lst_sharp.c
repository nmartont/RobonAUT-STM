/*
 * lst_sharp.c
 *
 *  Created on: 30 Jan 2018
 *      Author: Balazs
 */

#include "lst_sharp.h"

// WARNING
// Use distance functions for approximate measurements
// Use raw ADC value for car follow controller
// [mm] units

// TODO Refactor according to naming conventions

uint16_t LST_Sharp_GetLeftDistance()
{

	if (LST_Sharp_LeftSensorType)
	{
		// Long range type
		return LST_Sharp_ConvertLongValueToDistance(lst_adc_sharp_result[0]);
	}
	else
	{
		// Short range type
		return LST_Sharp_ConvertShortValueToDistance(lst_adc_sharp_result[0]);
	}

}

uint16_t LST_Sharp_GetFrontDistance()
{

	if (LST_Sharp_FrontSensorType)
		{
			// Long range type
			return LST_Sharp_ConvertLongValueToDistance(lst_adc_sharp_result[1]);
		}
		else
		{
			// Short range type
			return LST_Sharp_ConvertShortValueToDistance(lst_adc_sharp_result[1]);
		}

}

uint16_t LST_Sharp_GetRightDistance()
{

	if (LST_Sharp_RightSensorType)
		{
			// Long range type
			return LST_Sharp_ConvertLongValueToDistance(lst_adc_sharp_result[2]);
		}
		else
		{
			// Short range type
			return LST_Sharp_ConvertShortValueToDistance(lst_adc_sharp_result[2]);
		}

}

uint16_t LST_Sharp_GetRawLeftDistance()
{
	return lst_adc_sharp_result[0];
}

uint16_t LST_Sharp_GetRawFrontDistance()
{
	return lst_adc_sharp_result[1];
}

uint16_t LST_Sharp_GetRawRightDistance()
{
	return lst_adc_sharp_result[2];
}

uint16_t LST_Sharp_GetValueFromShortDistance(uint16_t distance)
{

	return 0; // TODO

}

uint16_t LST_Sharp_GetValueFromLongDistance(uint16_t distance)
{

	return 0; // TODO

}

uint16_t LST_Sharp_ConvertLongValueToDistance(uint16_t value)
{

	for (uint8_t i=LST_Sharp_LongRangeTable_Length; i>=0; i--)
	{

		// Underestimate distance
		if (value <= LST_Sharp_LongRangeTable[1][i])
			return LST_Sharp_LongRangeTable[0][i];

	}

	// Not found, return 0 ("closest" distance)
	return 0;

}

uint16_t LST_Sharp_ConvertShortValueToDistance(uint16_t value)
{

	for (uint8_t i=LST_Sharp_ShortRangeTable_Length; i>=0; i--)
		{

			// Underestimate distance
			if (value <= LST_Sharp_ShortRangeTable[1][i])
				return LST_Sharp_ShortRangeTable[0][i];

		}

		// Not found, return 0 ("closest" distance)
		return 0;

}

void LST_Sharp_Init()
{

	for (uint8_t i=0; i<LST_Sharp_ShortRangeTable_Length; i++)
	{

		LST_Sharp_ShortRangeTable[0][i] = LST_Sharp_ShortRangeTable_Base[0][i];

		LST_Sharp_ShortRangeTable[1][i] =
				(uint16_t) ((float) LST_Sharp_ShortRangeTable_Base[1][i] *
						LST_Sharp_Multiplier);

	}

	for (uint8_t i=0; i<LST_Sharp_LongRangeTable_Length; i++)
	{

		LST_Sharp_LongRangeTable[0][i] = LST_Sharp_LongRangeTable_Base[0][i];

		LST_Sharp_LongRangeTable[1][i] =
				(uint16_t) ((float) LST_Sharp_LongRangeTable_Base[1][i] *
						LST_Sharp_Multiplier);
	}

}

