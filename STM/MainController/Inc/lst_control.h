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
#include "lst_adc.h"

/* Defines -------------------------------------------------------------------*/
/* Control frequency for the FreeRTOS tasks */
#define LST_CONTROL_REPEAT_TICKS 10  // 100Hz

/* Default P and D values for the steering */
#define LST_CONTROL_STEERING_P           1540
#define LST_CONTROL_STEERING_D           18400

/* Float denums for dividing steering and servo control values */
#define LST_CONTROL_BT_STEERING_DENUM -21.487f
#define LST_CONTROL_BT_MOTOR_DENUM    -60.0f
#define LST_CONTROL_STEERING_DENUM    21.487f

/* Defines for lost line detection */
#define LST_CONTROL_LOST_LINES_THRESHOLD       20
#define LST_CONTROL_NEW_LINE_FILTER_THRESHOLD  4

/* Function prototypes -------------------------------------------------------*/
void LST_Control_Init();
void LST_Control_Commons();
void LST_Control_Resolve_Line();
uint8_t LST_Control_Check_Lost_Line();
int16_t LST_Control_Motor_BT();
int16_t LST_Control_Servo_BT();
float LST_Control_GetLinePosition();
int32_t LST_Control_SteeringController();
void LST_Control_ServoAndMotor();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t lst_control_line_lost_flag;

#endif /* LST_CONTROL_H_ */
