/*
 * lst_tasks.h
 *
 *  Created on: 2017. nov. 1.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_TASK_H_
#define LST_TASK_H_

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "lst_timer.h"
#include "lst_spi.h"
#include "lst_uart.h"
#include "lst_adc.h"
#include "lst_bt.h"
#include "lst_radio.h"
#include "lst_control.h"
#include "lst_i2c.h"
#include "lst_inertial.h"
#include "lst_obstacle.h"
#include "lst_fastlap.h"
#include "lst_dip.h"
#include "lst_infra.h"
#include "lst_sharp.h"
#include "tasks/lst_encoder.h"

/* Defines -------------------------------------------------------------------*/
// #define LST_TASK_INERTIAL_TASK_REPEAT_TICKS 5000
#define LST_TASK_BT_TASK_REPEAT_TICKS  200 // 5Hz
#define LST_TASK_DIP_TASK_REPEAT_TICKS 1000 // 1Hz

#define LST_TASK_MODE_FASTLAP  0
#define LST_TASK_MODE_OBSTACLE 1

// RTOS ticks per milliseconds
#define LST_RTOS_TICKS_PER_MS (configTICK_RATE_HZ / 1000)

// Task cycle time constants
#define LST_TASK_BT_TICKS					(10		* LST_RTOS_TICKS_PER_MS)
#define LST_TASK_DIP_TICKS				(100	* LST_RTOS_TICKS_PER_MS)
#define LST_TASK_LINEREAD_TICKS		(2		* LST_RTOS_TICKS_PER_MS)
//#define LST_TASK_LINEEVAL_TICKS		(1		* LST_RTOS_TICKS_PER_MS)
#define LST_TASK_INERTIAL_TICKS		(20		* LST_RTOS_TICKS_PER_MS)
#define LST_TASK_SHARP_TICKS			(20		* LST_RTOS_TICKS_PER_MS)
#define LST_TASK_ENCODER_TICKS		(2		* LST_RTOS_TICKS_PER_MS)

// Task priority constants (-3:IDLE, +3:REALTIME)
#define LST_TASK_BT_PRIO				0
#define LST_TASK_DIP_PRIO				0
#define LST_TASK_LINEREAD_PRIO	1
#define LST_TASK_LINEEVAL_PRIO	2
#define LST_TASK_INERTIAL_PRIO	0
#define LST_TASK_SHARP_PRIO			0
#define LST_TASK_ENCODER_PRIO		3

// Task stack constants
#define LST_TASK_BT_STACK					256
#define LST_TASK_DIP_STACK				128
#define LST_TASK_LINEREAD_STACK		512
#define LST_TASK_LINEEVAL_STACK		512
#define LST_TASK_INERTIAL_STACK		128
#define LST_TASK_SHARP_STACK			128
#define LST_TASK_ENCODER_STACK		256

/* Function prototypes -------------------------------------------------------*/
void LST_Task_Start(void const * argument);
void LST_Task_FastLap(void const * argument);
void LST_Task_Obstacle(void const * argument);

void LST_Task_BT_Request(void const * argument);
void LST_Task_DIP_Read(void const * argument);
void LST_Task_Line_Read(void const * argument);
void LST_Task_Line_Eval(void const * argument);
void LST_Task_Inertial_Read(void const * argument);
void LST_Task_Sharp_Read(void const * argument);
void LST_Task_Encoder_Read(void const * argument);

/* Private variables ---------------------------------------------------------*/

// Main tasks
osThreadId lst_task_StartTaskHandle;
osThreadId lst_task_TaskStartHandle;
osThreadId lst_task_MainTaskHandle;

// Sensor and control handling tasks

// Low priority | 10ms cycle time
osThreadId lst_task_BTRequest_Handle;

// Normal priority | 100ms cycle time
osThreadId lst_task_DIPRead_Handle;

// Above Normal priority | 2ms cycle time
osThreadId lst_task_LineRead_Handle;

// High priority | waits for LineDataHandle notification
osThreadId lst_task_LineEval_Handle;

// Normal priority | 20ms cycle time
osThreadId lst_task_InertialRead_Handle;

// Normal priority | 20ms cycle time
osThreadId lst_task_SharpRead_Handle;

// Above high priority | 2ms cycle time
// Note: will use vTaskDelayUntil (CHANGE TO TIMER?)
// -> not punctual, but will be OK with high priority
osThreadId lst_task_EncoderRead_Handle;


// Test tasks
osThreadId lst_test_StartTestHandle;
osThreadId lst_test_PwmTestHandle;
// osThreadId lst_task_InertialTaskHandle;

#endif /* LST_TASK_H_ */
