/*
 * function_adc.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_FUNCTION_ADC_H_
#define LST_FUNCTION_ADC_H_

// Includes
#include "lst_constants.h"
#ifdef LST_NUCLEO_TEST
	#include "stm32f4xx_hal.h"
#else
	#include "stm32f3xx_hal.h"
#endif

// External variables
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
#ifdef LST_NUCLEO_TEST
	// NUCLEO has 3 ADCs
#else
	extern ADC_HandleTypeDef hadc4;
#endif

// Local variables
uint8_t lst_adc_resultCount;
uint8_t lst_adc_resultReadyFlag;
uint8_t lst_adc_result[4];

// Function declarations

void lst_adc_start_conversions(void);
/*
 * @Description
 * 	Starts all four (Nucleo: 3) ADC conversions
 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adcHandle);
/*
 * @Description
 * 	HAL callback function, when a conversion completes
 */

#endif /* FUNCTION_ADC_H_ */
