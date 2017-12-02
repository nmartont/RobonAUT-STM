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
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Timer part of the software
 */
void LST_Timer_Init() {
  /* Start PWMs */
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, LST_TIM_RCPWM_MIDDLE);
  
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, LST_TIM_RCPWM_MIDDLE);
}

/**
 * @brief Sets the RC PWM signal for the DC Motor
 */
void LST_TIM_SetMotorRcPwm(int16_t motor){
  /* Min/Max value */
  if (motor < LST_TIM_RCPWM_MIN) motor = 3050;
  if (motor > LST_TIM_RCPWM_MAX) motor = 6100;

  /* Set timer value */
  __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, motor);
}

/**
 * @brief Sets the RC PWM signal for the Servo
 */
void LST_TIM_SetServoRcPwm(int16_t servo){
  /* Min/Max value */
  if (servo < LST_TIM_RCPWM_MIN) servo = 3050;
  if (servo > LST_TIM_RCPWM_MAX) servo = 6100;

  /* Set timer value */
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, servo);
}

/**
 * @brief Function that handles the timer elapsed callback
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  cntr++;
}
