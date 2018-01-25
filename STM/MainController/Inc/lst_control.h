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
#include "lst_inertial.h"
#include "lst_utils.h"

/* Defines -------------------------------------------------------------------*/
/* Control frequency for the FreeRTOS tasks */
#define LST_CONTROL_REPEAT_TICKS 10  // 100Hz

/* Default P and D values for the steering */
#define LST_CONTROL_STEERING_P           1540
#define LST_CONTROL_STEERING_P_DIVIDER   16384.0f
#define LST_CONTROL_STEERING_D           18400
#define LST_CONTROL_STEERING_D_DIVIDER   1630.0f

/* Default values for the speed PID controller */
#define LST_CONTROL_SPEED_P           12000
#define LST_CONTROL_SPEED_P_DIVIDER   1638.40f
#define LST_CONTROL_SPEED_D           5000
#define LST_CONTROL_SPEED_D_DIVIDER   16384.0f
#define LST_CONTROL_SPEED_I           300
#define LST_CONTROL_SPEED_I_DIVIDER   16384.0f

/* Float denums for dividing steering and servo control values */
#define LST_CONTROL_BT_STEERING_DENUM -21.487f
#define LST_CONTROL_BT_MOTOR_DENUM    -60.0f
#define LST_CONTROL_STEERING_DENUM    21.487f
#define LST_CONTROL_SPEED_DENUM       1.0f

/* Defines for lost line detection */
#define LST_CONTROL_LOST_LINES_THRESHOLD       20
#define LST_CONTROL_NEW_LINE_FILTER_THRESHOLD  4

/* Interpolation */
#define LST_CONTROL_PD_INTERPOL_POINTS  5

// LST_SETTINGS Servo invert

// Invert steering directions
#define LST_INVERT_STEERING

/* Function prototypes -------------------------------------------------------*/
void LST_Control_Init();
void LST_Control_Commons();
int16_t LST_Control_Motor_BT();
int16_t LST_Control_Servo_BT();
int32_t LST_Control_SteeringController(uint8_t use_interpolation);
int32_t LST_Control_SpeedController(int16_t reference);
void LST_Control_ServoAndMotor();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint32_t lst_control_time_cntr;
extern uint8_t  lst_control_line_lost_flag;
extern uint16_t lst_control_steeringP;
extern uint16_t lst_control_steeringD;
extern int16_t  lst_control_steering;
extern uint16_t lst_control_speedP;
extern uint16_t lst_control_speedD;
extern uint16_t lst_control_speedI;
extern int16_t  lst_control_motor;
extern float    lst_control_speed_encoder;
extern int16_t  lst_control_steering_offset;
extern uint8_t  lst_control_line_no;

#endif /* LST_CONTROL_H_ */
