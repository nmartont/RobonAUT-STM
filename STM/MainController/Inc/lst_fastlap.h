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
#include "lst_sharp.h"
#include "tasks/lst_encoder.h"

// TODO TEMP

uint8_t lst_fast_startDetected;

/* Defines -------------------------------------------------------------------*/
/* Modes for the main state machine */
#define LST_FAST_MODE_BT             0
#define LST_FAST_MODE_STOP           1
#define LST_FAST_MODE_LINE_FOLLOW    2
#define LST_FAST_MODE_Q1             3
#define LST_FAST_MODE_NO_CONTROL     4

#define LST_FAST_MODE_Q1_START           5
#define LST_FAST_MODE_Q1_APPROACH        6
#define LST_FAST_MODE_Q1_WAIT_FOR_FOLLOW 7
#define LST_FAST_MODE_Q1_FOLLOW          8
#define LST_FAST_MODE_Q1_START_LAPS      9
#define LST_FAST_MODE_Q1_SLOW            10
#define LST_FAST_MODE_Q1_FAST            11
#define LST_FAST_MODE_Q1_ACCEL           12
#define LST_FAST_MODE_Q1_BRAKE           13
#define LST_FAST_MODE_Q1_END             14

// LST_SETTINGS Encoderless 'Fast lap' mode switch
//#define LST_FAST_MODE_ENCODERLESS
// LST_SETTINGS Interpolated steering controller parameters
// #define LST_FAST_MODE_STEERING_INTERPOLATED

// LST_SETTINGS Three laps switch for the fast lap mode
#define LST_FAST_THREE_LAPS

#define LST_FAST_MODE_SPEED_CONTROL  20

#define LST_FAST_INSNESITIVITY_DIST  1000 // 1 meter

// Both use encoder TODO 2018.02.10. TEST
#define LST_FAST_Q1_SLOW_FOLLOW_SPEED_MAX			 125 // 95  // 270 // 290 //280 // ToDo NMT ENCODER
#define LST_FAST_Q1_FAST_FOLLOW_SPEED_MAX			 175

/* Speed values */
#ifdef LST_FAST_MODE_ENCODERLESS
//#define LST_FAST_Q1_APPROACH_MOTOR_SPEED       300 // 200 -> does not start
 // 310 // ToDo NMT ENCODER

#define LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP1      295
#define LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP2      310
#define LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP3      330
#define LST_FAST_Q1_FAST_MOTOR_SPEED_LAP1      460
#define LST_FAST_Q1_FAST_MOTOR_SPEED_LAP2      500
#define LST_FAST_Q1_FAST_MOTOR_SPEED_LAP3      550

//#define LST_FAST_Q1_START_MOTOR_SPEED          350
#define LST_FAST_Q1_FINISH_MOTOR_SPEED         550 // ToDo increase
#else
// ToDo calibrate these values
#define LST_FAST_Q1_APPROACH_MOTOR_SPEED       50
#define LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP1      150
#define LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP2      150
#define LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP3      150
#define LST_FAST_Q1_FAST_MOTOR_SPEED_LAP1      320
#define LST_FAST_Q1_FAST_MOTOR_SPEED_LAP2      360
#define LST_FAST_Q1_FAST_MOTOR_SPEED_LAP3      400

#define LST_FAST_Q1_START_MOTOR_SPEED          130
#define LST_FAST_Q1_FINISH_MOTOR_SPEED         250 // ToDo increase
#endif

#ifdef LST_FAST_MODE_ENCODERLESS
#define LST_BRAKE_Q1_DELAY_LAP1			100
#define LST_BRAKE_Q1_DELAY_LAP2     150
#define LST_BRAKE_Q1_DELAY_LAP3     200
#else
#define LST_BRAKE_Q1_DELAY_LAP1			80
#define LST_BRAKE_Q1_DELAY_LAP2     100
#define LST_BRAKE_Q1_DELAY_LAP3     120
#endif

#define LST_BRAKE_Q1_SPEED          -800
#define LST_BRAKE_Q1_SPEED_LAP1			-800
#define LST_BRAKE_Q1_SPEED_LAP2     -800
#define LST_BRAKE_Q1_SPEED_LAP3     -800

/* Steering values */
#define LST_FAST_Q1_SLOW_STEERING_P       17000
#define LST_FAST_Q1_SLOW_STEERING_D       4000
#define LST_FAST_Q1_APPROACH_STEERING_P   17000
#define LST_FAST_Q1_APPROACH_STEERING_D   4000

#define LST_FAST_Q1_FOLLOW_FAST_STEERING_P     	5000
#define LST_FAST_Q1_FOLLOW_FAST_STEERING_D     	13000
#define LST_FAST_Q1_FOLLOW_SLOW_STEERING_P			18000
#define LST_FAST_Q1_FOLLOW_SLOW_STEERING_D			4000

#define LST_FAST_Q1_FAST_STEERING_P       1750
#define LST_FAST_Q1_FAST_STEERING_D       8000
#define LST_FAST_Q1_START_STEERING_P      20000
#define LST_FAST_Q1_START_STEERING_D      10000
#define LST_FAST_Q1_BRAKE_STEERING_P      20000
#define LST_FAST_Q1_BRAKE_STEERING_D      10000
#define LST_FAST_Q1_END_STEERING_P        1750 // ToDo change
#define LST_FAST_Q1_END_STEERING_D        8000

#define LST_FAST_Q1_SLOW_STEERING_P_LAP1 17000
#define LST_FAST_Q1_SLOW_STEERING_D_LAP1 4000
#define LST_FAST_Q1_FAST_STEERING_P_LAP1 1750
#define LST_FAST_Q1_FAST_STEERING_D_LAP1 8000

#define LST_FAST_Q1_SLOW_STEERING_P_LAP2 15000
#define LST_FAST_Q1_SLOW_STEERING_D_LAP2 5000
#define LST_FAST_Q1_FAST_STEERING_P_LAP2 1500
#define LST_FAST_Q1_FAST_STEERING_D_LAP2 9000

#define LST_FAST_Q1_SLOW_STEERING_P_LAP3 13500
#define LST_FAST_Q1_SLOW_STEERING_D_LAP3 5500
#define LST_FAST_Q1_FAST_STEERING_P_LAP3 1350
#define LST_FAST_Q1_FAST_STEERING_D_LAP3 10000

/* Start defines */
#define LST_FAST_Q1_START_TIME         5

/* Approach defines */
#define LST_FAST_Q1_APPROACH_DIST   1000 // ADC value (12bits)
#define LST_FAST_Q1_APPROACH_MIN    5   // consecutive Sharp measurements

/* Wait for follow defines */
#define LST_FAST_Q1_WAIT_FOLLOW_DIST  800 // ADC value (12bits)
#define LST_FAST_Q1_WAIT_MIN          30  // 300ms

/* Follow defines */
#define LST_FAST_Q1_FOLLOW_DIST               1200 //800 // ADC value (12bits)
#define LST_FAST_Q1_FOLLOW_END_DIST           100 // ADC value (12bits)
#define LST_FAST_Q1_FOLLOW_FILTER_THRESHOLD   1
#define LST_FAST_Q1_FOLLOW_LINES_THRESHOLD    2
#define LST_FAST_Q1_FOLLOW_TOO_CLOSE_DIST     1600
#define LST_FAST_Q1_FOLLOW_BRAKE_TIME         55 // 50
#define LST_FAST_Q1_FOLLOW_ACCEL_TIME         50
#define LST_FAST_Q1_FOLLOW_BRAKE_MIN_SPEED    20

/* Slow mode defines */
#define LST_FAST_Q1_SLOW_FILTER_THRESHOLD 1 // TODO TEST 2018.01.31. ONLY LST_CONTROL FILTER (was 5)
#define LST_FAST_Q1_SLOW_LINES_THRESHOLD  2

/* Fast mode defines */
#define LST_FAST_Q1_FAST_TRIPLE_LINES 5 // TODO TEST 2018. 01. 31. Test with a higher value, earlier braking first (was 15)

/* Brake mode defines */
#define LST_FAST_BRAKE_DELAY           120
#define LST_FAST_BRAKE_TIME            30
#define LST_FAST_Q1_BRAKE_MOTOR       -700

/* Acceleration defines */
#define LST_FAST_Q1_ACCEL_PLUS_P       -85
#define LST_FAST_Q1_ACCEL_PLUS_D       13
#define LST_FAST_Q1_ACCEL_PLUS_MOTOR   1
#define LST_FAST_Q1_ACCEL_TIME         25

/* End defines */
#define LST_FAST_END_DIST              3000 // mm

/* Function prototypes -------------------------------------------------------*/
void LST_Fast_Init();
void LST_Fast_Logic();
void LST_Fast_DIP_Handler_Start();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t lst_fast_mode;

#endif /* LST_FASTLAP_H_ */
