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
uint8_t cntr_adc_sharp = 0;
uint16_t lst_adc_sharp_result[3] = {0x00};

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                    ADC handler for RobonAUT 2018 Team LST                  */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the ADC part of the software
 */
void LST_ADC_Init() {
  
}

/**
 * @brief Starts the Sharp ADC conversion
 */
void LST_ADC_StartSharpADC(){
  cntr_adc_sharp = 0;
  /* Start all 3 ADCs */
  while(hadc1.State != HAL_ADC_STATE_READY){}
  HAL_ADC_Start_IT(&hadc1);
  while(hadc2.State != HAL_ADC_STATE_READY){}
  HAL_ADC_Start_IT(&hadc2);
  while(hadc3.State != HAL_ADC_STATE_READY){}
  HAL_ADC_Start_IT(&hadc3);
}

/**
 * @brief Waits for the Sharp ADC conversion
 */
void LST_ADC_WaitForSharpADC(){
  /* Wait for all three Sharp ADCs to complete */
  while(cntr_adc_sharp<3){}

  /* Get results */
  lst_adc_sharp_result[0] = HAL_ADC_GetValue(&hadc1);
  lst_adc_sharp_result[1] = HAL_ADC_GetValue(&hadc2);
  lst_adc_sharp_result[2] = HAL_ADC_GetValue(&hadc3);
}

/**
 * @brief Triggered when the ADC conversion is done
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  if (hadc->Instance == ADC1){
    cntr_adc_sharp++;
  }
  if (hadc->Instance == ADC2){
    cntr_adc_sharp++;
  }
  if (hadc->Instance == ADC3){
    cntr_adc_sharp++;
  }
}
