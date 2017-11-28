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

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void LST_Task_Start(void const * argument);
void LST_Task_Q1(void const * argument);
void LST_Task_BT_Request_Handler(void const * argument);

/* Private variables ---------------------------------------------------------*/
osThreadId lst_task_StartTaskHandle;
osThreadId lst_task_TaskStartHandle;
osThreadId lst_task_Q1TaskHandle;
osThreadId lst_task_BTRequestHandlerTaskHandle;

#endif /* LST_TASK_H_ */
