/*
 * lst_tasks.h
 *
 *  Created on: 2017. nov. 1.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_TASKS_H_
#define LST_TASKS_H_

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "lst_timer.h"
#include "lst_spi.h"
#include "lst_uart.h"

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void LST_Task_UART_Test(void const * argument);
void LST_Task_SPI_Test(void const * argument);
void LST_Task_TIM_Test(void const * argument);

/* Private variables ---------------------------------------------------------*/
osThreadId lstDefaultTaskHandle;
osThreadId lstTaskUartTestHandle;

#endif /* LST_TASKS_H_ */
