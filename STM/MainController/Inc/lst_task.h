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

/* Defines -------------------------------------------------------------------*/
// #define LST_TASK_INERTIAL_TASK_REPEAT_TICKS 5000 // 2Hz
#define LST_TASK_BT_TASK_REPEAT_TICKS 200 // 5Hz

#define LST_TASK_MODE_FASTLAP  0
#define LST_TASK_MODE_OBSTACLE 1

/* Function prototypes -------------------------------------------------------*/
void LST_Task_Start(void const * argument);
void LST_Task_FastLap(void const * argument);
void LST_Task_Obstacle(void const * argument);
void LST_Task_BT_RequestHandler(void const * argument);
void LST_Task_DIP_Read(void const * argument);
// void LST_Task_Inertial(void const * argument);

/* Private variables ---------------------------------------------------------*/
osThreadId lst_task_StartTaskHandle;
osThreadId lst_task_TaskStartHandle;
osThreadId lst_task_MainTaskHandle;
osThreadId lst_task_BTRequestHandlerTaskHandle;
osThreadId lst_task_DIPReadHandle;
// osThreadId lst_task_InertialTaskHandle;

#endif /* LST_TASK_H_ */
