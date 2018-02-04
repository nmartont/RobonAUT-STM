/*
 * lst_fastlap.h
 *
 *  Created on: 2017. dec. 26.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_FASTLAP_H_
#define LST_FASTLAP_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"
#include "lst_control.h"
#include "lst_bt.h"

/* Defines -------------------------------------------------------------------*/
/* Modes for the main state machine */
#define LST_FAST_MODE_BT             0
#define LST_FAST_MODE_STOP           1
#define LST_FAST_MODE_LINE_FOLLOW    2
#define LST_FAST_MODE_Q1             3
#define LST_FAST_MODE_Q1_SLOW        4
#define LST_FAST_MODE_Q1_FAST        5
#define LST_FAST_MODE_Q1_START       6
#define LST_FAST_MODE_Q1_ACCEL       7
#define LST_FAST_MODE_Q1_BRAKE       8
#define LST_FAST_MODE_Q1_LOST_LINE   9
#define LST_FAST_MODE_NO_CONTROL     10

// LST_SETTINGS Encoderless 'Fast lap' mode switch
#define LST_FAST_MODE_ENCODERLESS

#define LST_FAST_MODE_SPEED_CONTROL  20

#define LST_FAST_Q1_START_MOTOR_SPEED  500
#define LST_FAST_Q1_START_STEERING_P   20000
#define LST_FAST_Q1_START_STEERING_D   10000
#define LST_FAST_Q1_START_TIME         20

#define LST_FAST_Q1_SLOW_MOTOR_SPEED      295
#define LST_FAST_Q1_SLOW_STEERING_P       17000
#define LST_FAST_Q1_SLOW_STEERING_D       4000
#define LST_FAST_Q1_SLOW_FILTER_THRESHOLD 1 // TODO TEST 2018.01.31. ONLY LST_CONTROL FILTER (was 5)
#define LST_FAST_Q1_SLOW_LINES_THRESHOLD  2

#define LST_FAST_Q1_ACCEL_PLUS_P       -85
#define LST_FAST_Q1_ACCEL_PLUS_D       13
#define LST_FAST_Q1_ACCEL_PLUS_MOTOR   1
#define LST_FAST_Q1_ACCEL_TIME         25

#define LST_FAST_Q1_FAST_MOTOR_SPEED  460
#define LST_FAST_Q1_FAST_STEERING_P   1750
#define LST_FAST_Q1_FAST_STEERING_D   8000
#define LST_FAST_Q1_FAST_TRIPLE_LINES 5 // TODO TEST 2018. 01. 31. Test with a higher value, earlier braking first (was 15)

#define LST_FAST_Q1_BRAKE_STEERING_P  20000
#define LST_FAST_Q1_BRAKE_STEERING_D  10000
#define LST_FAST_Q1_BRAKE_MOTOR       -700

#define LST_FAST_BRAKE_DELAY 5
#define LST_FAST_BRAKE_TIME  40

/* Function prototypes -------------------------------------------------------*/
void LST_Fast_Init();
void LST_Fast_Logic();
void LST_Fast_DIP_Handler_Start();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t lst_fast_mode;

#endif /* LST_FASTLAP_H_ */
