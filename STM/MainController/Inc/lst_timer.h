/*
 * lst_timer.h
 *
 *  Created on: 2017. nov. 1.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_TIMER_H_
#define LST_TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"

/* Defines -------------------------------------------------------------------*/
#define LST_TIM_RCPWM_MIDDLE 4575
#define LST_TIM_RCPWM_MIN    -1525
#define LST_TIM_RCPWM_MAX    1525

#define LST_TIM_ENCODER_MAX                50000
#define LST_TIM_ENCODER_OVERFLOW_THRESHOLD 10000

/* Function prototypes -------------------------------------------------------*/
void LST_Timer_Init();
void LST_TIM_SetMotorRcPwm(int16_t motor);
void LST_TIM_SetServoRcPwm(int16_t servo);
int16_t LST_TIM_CalculateSpeed();

// LST_SETTINGS Motor PWM
static const uint32_t counterLimit = 4499;

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim8;

#endif /* LST_TIMER_H_ */
