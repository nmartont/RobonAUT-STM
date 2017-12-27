/*
 * lst_adc.h
 *
 *  Created on: 2017. nov. 16.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_ADC_H_
#define LST_ADC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void LST_ADC_Init();
void LST_ADC_StartSharpADC();
void LST_ADC_WaitForSharpADC();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

extern uint16_t lst_adc_sharp_result[3];

#endif /* LST_ADC_H_ */
