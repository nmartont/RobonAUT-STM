/*
 * lst_control.h
 *
 *  Created on: 2017. nov. 28.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_CONTROL_H_
#define LST_CONTROL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"
#include "lst_spi.h"
#include "lst_bt.h"
#include "lst_timer.h"

/* Defines -------------------------------------------------------------------*/
#define LST_CONTROL_MODE_BT             0
#define LST_CONTROL_MODE_STOP           1
#define LST_CONTROL_MODE_LINE_FOLLOW    2
#define LST_CONTROL_MODE_Q1_SLOW        3
#define LST_CONTROL_MODE_Q1_FAST        4
#define LST_CONTROL_MODE_Q1             5

#define LST_CONTROL_LINESENSOR_TRESHOLD 130
#define LST_CONTROL_STEERING_P       0x8000
#define LST_CONTROL_STEERING_D       328

#define LST_CONTROL_BT_STEERING_DENUM -21.487f
#define LST_CONTROL_BT_MOTOR_DENUM    -60.0f

#define LST_CONTROL_Q1_SLOW_MOTOR_SPEED   275
#define LST_CONTROL_Q1_SLOW_MOTOR_SPEED_1 500
#define LST_CONTROL_Q1_SLOW_P             20000
#define LST_CONTROL_Q1_SLOW_D             5000

#define LST_CONTROL_Q1_FAST_MOTOR_SPEED 350
#define LST_CONTROL_Q1_FAST_P           6000
#define LST_CONTROL_Q1_FAST_D           1000

/* Function prototypes -------------------------------------------------------*/
void LST_Control_Init();
void LST_Control();
void LST_Control_Select_Mode();
int16_t LST_Control_Motor_BT();
int16_t LST_Control_Servo_BT();
float LST_Control_GetLinePosition();
int16_t LST_Control_SteeringController();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t lst_control_mode;

#endif /* LST_CONTROL_H_ */
