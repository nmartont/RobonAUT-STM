/*
 * function_adc.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "lst_function_adc.h"

void lst_adc_convert(void)
{

	lst_adc_resultReadyFlag = 0;

	lst_adc_start_conversions();

	while (!lst_adc_resultReadyFlag)
	{

		// Wait

	}

	// Store measured data

	lst_adc_result[0] = HAL_ADC_GetValue(&hadc1);
	lst_adc_result[1] = HAL_ADC_GetValue(&hadc2);
	lst_adc_result[2] = HAL_ADC_GetValue(&hadc3);

#ifndef LST_NUCLEO_TEST
	lst_adc_result[3] = HAL_ADC_GetValue(&hadc4);
#else
	lst_adc_result[3] = lst_adc_result[2];
#endif




}

void lst_adc_start_conversions(void)
{

	lst_adc_resultCount = 0;

	HAL_ADC_Start_IT(&hadc1);
	HAL_ADC_Start_IT(&hadc2);
	HAL_ADC_Start_IT(&hadc3);
#ifdef LST_NUCLEO_TEST
	// No ADC4
#else
	HAL_ADC_Start_IT(&hadc4);
#endif

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adcHandle)
{

	lst_adc_resultCount++;

#ifdef LST_NUCLEO_TEST
	if (lst_adc_resultCount > 2)
#else
	if (lst_adc_resultCount > 3)
#endif
	{

		lst_adc_resultReadyFlag = 1;

	}

}
