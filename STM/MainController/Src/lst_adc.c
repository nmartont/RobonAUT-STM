/*
 * lst_adc.c
 *
 *  Created on: 2017. nov. 16.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_adc.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t cntr_adc = 0;

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                    ADC handler  for RobonAUT 2018 Team LST                 */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the ADC part of the software
 */
void LST_ADC_Init() {
  
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* adcHandle) {
  cntr_adc++;
}
