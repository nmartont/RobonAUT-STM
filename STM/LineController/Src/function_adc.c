/*
 * function_adc.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "function_adc.h"

void lst_adc_start_conversions(void)
{

	adc_resultCount = 0;
	adc_resultReadyFlag = 0;

	HAL_ADC_Start_IT(&hadc1);
	HAL_ADC_Start_IT(&hadc2);
	HAL_ADC_Start_IT(&hadc3);
	HAL_ADC_Start_IT(&hadc4);

}

void adc_reset_resultReadyFlag(void)
{

	adc_resultReadyFlag = 0;

}

// ADC Conversion complete callback function
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adcHandle)
{

	adc_resultCount++;

	if (adc_resultCount > 2)
	{

		adc_resultReadyFlag = 1;

	}

}


/* UNUSED
uint8_t * adc_get_results(void)
{

	adc_result[0] = HAL_ADC_GetValue(&hadc1);
	adc_result[1] = HAL_ADC_GetValue(&hadc2);
	adc_result[2] = HAL_ADC_GetValue(&hadc3);
	adc_result[3] = HAL_ADC_GetValue(&hadc4);

	return adc_result;

}
*/
