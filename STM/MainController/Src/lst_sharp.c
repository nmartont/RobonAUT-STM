/*
 * lst_sharp.c
 *
 *  Created on: 30 Jan 2018
 *      Author: Balazs
 */

#include "lst_sharp.h"

uint16_t LST_Sharp_GetLeftDistance()
{
	return lst_adc_sharp_result[0];
}

uint16_t LST_Sharp_GetFrontDistance()
{
	return lst_adc_sharp_result[1];
}

uint16_t LST_Sharp_GetRightDistance()
{
	return lst_adc_sharp_result[2];
}

