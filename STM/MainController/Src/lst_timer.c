/*
 * lst_timer.c
 *
 *  Created on: 2017. okt. 31.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_timer.h"

/* Private variables ---------------------------------------------------------*/
uint32_t enc_cnt_last = 0;

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
#ifdef LST_CONFIG_CUSTOM_MOTOR_CONTROL
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
  // ToDo set up defaults for custom motor control
#else
  LST_TIM_SetMotorRcPwm(0);
#endif
  
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
int16_t LST_TIM_CalculateSpeed(){
  uint32_t temp = htim3.Instance->CNT;
  int32_t speed = temp - enc_cnt_last;
  enc_cnt_last = temp;

  if(speed < -LST_TIM_ENCODER_OVERFLOW_THRESHOLD){
    speed = speed + LST_TIM_ENCODER_MAX;
  }else if(speed > LST_TIM_ENCODER_OVERFLOW_THRESHOLD){
    speed = speed - LST_TIM_ENCODER_MAX;
  }

  return speed;
}

/**
 * @brief Function that handles the timer elapsed callback
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

}
