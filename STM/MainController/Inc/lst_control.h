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
#define LST_CONTROL_MODE_Q1             3
#define LST_CONTROL_MODE_Q1_SLOW        4
#define LST_CONTROL_MODE_Q1_FAST        5
#define LST_CONTROL_MODE_Q1_START       6
#define LST_CONTROL_MODE_Q1_ACCEL       7
#define LST_CONTROL_MODE_Q1_BRAKE       8

#define LST_CONTROL_MODE_LINE_SLOW      10
#define LST_CONTROL_MODE_LINE_FAST      11

#define LST_CONTROL_LINESENSOR_THRESHOLD 130
#define LST_CONTROL_STEERING_P           1540
#define LST_CONTROL_STEERING_D           18400

#define LST_CONTROL_BT_STEERING_DENUM -21.487f
#define LST_CONTROL_BT_MOTOR_DENUM    -60.0f

#define LST_CONTROL_STEERING_DENUM    21.487f

#define LST_CONTROL_Q1_START_MOTOR_SPEED  500
#define LST_CONTROL_Q1_START_STEERING_P   20000
#define LST_CONTROL_Q1_START_STEERING_D   10000
#define LST_CONTROL_Q1_START_TIME         15

#define LST_CONTROL_Q1_SLOW_MOTOR_SPEED      275
#define LST_CONTROL_Q1_SLOW_STEERING_P    	 20000
#define LST_CONTROL_Q1_SLOW_STEERING_D    	 10000
#define LST_CONTROL_Q1_SLOW_FILTER_THRESHOLD 5
#define LST_CONTROL_Q1_SLOW_LINES_THRESHOLD  2

#define LST_CONTROL_Q1_ACCEL_PLUS_P				-85
#define LST_CONTROL_Q1_ACCEL_PLUS_D				13
#define LST_CONTROL_Q1_ACCEL_PLUS_MOTOR		1
#define LST_CONTROL_Q1_ACCEL_TIME					200

#define LST_CONTROL_Q1_FAST_MOTOR_SPEED  375
#define LST_CONTROL_Q1_FAST_STEERING_P   3000
#define LST_CONTROL_Q1_FAST_STEERING_D   12600
#define LST_CONTROL_Q1_FAST_TRIPLE_LINES 20

#define LST_CONTROL_Q1_BRAKE_STEERING_P  20000
#define LST_CONTROL_Q1_BRAKE_STEERING_D  10000
#define LST_CONTROL_Q1_BRAKE_MOTOR		   -700
#define LST_CONTROL_BRAKE_DELAY	5
#define LST_CONTROL_BRAKE_TIME  40

/* Function prototypes -------------------------------------------------------*/
void LST_Control_Init();
void LST_Control();
void LST_Control_Resolve_Line_Mode();
void LST_Control_Select_Mode();
void LST_Control_Q1();
int16_t LST_Control_Motor_BT();
int16_t LST_Control_Servo_BT();
float LST_Control_GetLinePosition();
int32_t LST_Control_SteeringController();
void LST_Control_Reset_State_Machine();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t lst_control_mode;

#endif /* LST_CONTROL_H_ */
