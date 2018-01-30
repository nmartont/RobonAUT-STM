/*
 * function_adc.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include <function/lst_function_adc.h>

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
	lst_adc_nucleoConversion();
#endif

}

void lst_adc_nucleoConversion(void)
{

#ifdef LST_NUCLEO_TEST
	HAL_ADC_ConfigChannel(&hadc1, &adc1_config_B);

	// Random ADC waiting time
	lst_timer1_delay_timClk(500);

	HAL_ADC_Start(&hadc1);

	// Random ADC waiting time
	lst_timer1_delay_timClk(500);

	lst_adc_result[3] = HAL_ADC_GetValue(&hadc1);

	HAL_ADC_ConfigChannel(&hadc1, &adc1_config_A);

	// Random ADC waiting time
	lst_timer1_delay_timClk(500);

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

void lst_adc_init()
{

#ifdef LST_NUCLEO_TEST
	adc1_config_A.Channel = ADC_CHANNEL_11;
	adc1_config_A.Rank = 1;
	adc1_config_A.SamplingTime = ADC_SAMPLETIME_3CYCLES;

	adc1_config_B.Channel = ADC_CHANNEL_14;
	adc1_config_B.Rank = 1;
	adc1_config_B.SamplingTime = ADC_SAMPLETIME_3CYCLES;
#endif

}
