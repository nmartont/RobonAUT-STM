/*
 * lst_tasks.c
 *
 *  Created on: 2017. okt. 26.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include <lst_task.h>

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                  FreeRTOS tasks for RobonAUT 2018 Team LST                 */
/******************************************************************************/

/**
 * @brief This task starts the MainController tests.
 */
void LST_Task_Start(void const * argument) {
  /* Start the tasks */
  /* TODO */

  /* Exit starter task */
  osThreadTerminate(lst_task_StartTaskHandle);
}
