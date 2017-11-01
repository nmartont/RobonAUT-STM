/*
 * lst_timer.c
 *
 *  Created on: 2017. okt. 31.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_timer.h"

/* Private variables ---------------------------------------------------------*/
uint8_t cntr = 0;

/******************************************************************************/
/*                   Timer handling for RobonAUT 2018 Team LST                */
/******************************************************************************/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
	cntr++;
}
