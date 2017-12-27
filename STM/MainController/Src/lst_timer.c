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
uint16_t enc_cnt      = 0;
uint16_t enc_cnt_last = 0;

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
  LST_TIM_SetMotorRcPwm(0);
  
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  LST_TIM_SetServoRcPwm(0);

  /* Start Encoder */
  HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
}

/**
 * @brief Sets the RC PWM signal for the DC Motor
 */
void LST_TIM_SetMotorRcPwm(int16_t motor){
  /* Min/Max value */
  if (motor < LST_TIM_RCPWM_MIN) motor = LST_TIM_RCPWM_MIN;
  if (motor > LST_TIM_RCPWM_MAX) motor = LST_TIM_RCPWM_MAX;

  /* Set timer value */
  __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, LST_TIM_RCPWM_MIDDLE + motor);
}

/**
 * @brief Sets the RC PWM signal for the Servo
 */
void LST_TIM_SetServoRcPwm(int16_t servo){
  /* Min/Max value */
  if (servo < LST_TIM_RCPWM_MIN) servo = LST_TIM_RCPWM_MIN;
  if (servo > LST_TIM_RCPWM_MAX) servo = LST_TIM_RCPWM_MAX;

  /* Set timer value */
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, LST_TIM_RCPWM_MIDDLE + servo);
}

/**
 * @brief Function calculates vehicle speed
 */
uint16_t LST_TIM_CalculateSpeed(){
  uint16_t temp = htim3.Instance->CNT;
  uint16_t speed = temp - enc_cnt_last;
  // ToDo what if overflow
  enc_cnt_last = temp;

  return speed;
}

/**
 * @brief Function that handles the timer elapsed callback
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  cntr++;
}
