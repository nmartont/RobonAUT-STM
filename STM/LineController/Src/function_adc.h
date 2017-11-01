/*
 * function_adc.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#ifndef FUNCTION_ADC_H_
#define FUNCTION_ADC_H_
#endif /* FUNCTION_ADC_H_ */

// ADC HAL functions
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_adc.h"

// ADC handlers
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;

uint8_t adc_resultCount;
uint8_t adc_resultReadyFlag;
uint8_t adc_result[4];

void lst_adc_start_conversions(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adcHandle);
uint8_t * adc_get_results(void);
