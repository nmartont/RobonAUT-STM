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
uint8_t lst_task_mode = LST_TASK_MODE_FASTLAP;

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                  FreeRTOS tasks for RobonAUT 2018 Team LST                 */
/******************************************************************************/

/**
 * @brief This task starts the MainController tasks.
 */
void LST_Task_Start(void const * argument) {
  /* Send Status to the PC */
  LST_BT_Send_StatusOk();

  /* Start BT request handler */
  osThreadDef(LST_Task_BT_RequestHandler, LST_Task_BT_RequestHandler, osPriorityLow, 0, 256);
  lst_task_BTRequestHandlerTaskHandle = osThreadCreate(osThread(LST_Task_BT_RequestHandler), NULL);

  /* Start inertial sensor task */
  osThreadDef(LST_Task_Inertial, LST_Task_Inertial, osPriorityNormal, 0, 256);
  lst_task_InertialTaskHandle = osThreadCreate(osThread(LST_Task_Inertial), NULL);

  /* ToDo Determine which mode to start based on the switches on the car */
  lst_bt_diag_mode = LST_BT_DIAG_MODE_FASTLAP;
  if (lst_task_mode == LST_TASK_MODE_FASTLAP){
    /* Start Fast Lap mode */
    osThreadDef(LST_Task_FastLap, LST_Task_FastLap, osPriorityNormal, 0, 1024);
    lst_task_MainTaskHandle = osThreadCreate(osThread(LST_Task_FastLap), NULL);
  }
  else if (lst_task_mode == LST_TASK_MODE_OBSTACLE){
    /* Start Obstacle Lap mode */
    osThreadDef(LST_Task_Obstacle, LST_Task_Obstacle, osPriorityNormal, 0, 1024);
    lst_task_MainTaskHandle = osThreadCreate(osThread(LST_Task_Obstacle), NULL);
  }

  /* Exit starter task */
  osThreadTerminate(lst_task_TaskStartHandle);
}

/**
 * @brief Task for Fast Lap
 */
void LST_Task_FastLap(void const * argument) {
  /* Init Fast mode */
  LST_Fast_Init();

  /* Send VarList to the PC */
  LST_BT_Send_VarList();

  /* Record starting timestamp */
  TickType_t xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  while (1) {
    LST_Fast_Logic();

    /* Send diagnostic data via BT */
    if (lst_bt_send_diagdata_flag)
      LST_BT_Send_VarValues();

    /* Wait for the next cycle */
    vTaskDelayUntil(&xLastWakeTime, LST_CONTROL_REPEAT_TICKS);
  }
}

/**
 * @brief Task for Obstacle Lap
 */
void LST_Task_Obstacle(void const * argument) {
  /* Init Obstacle mode */
  LST_Obs_Init();

  /* Send VarList to the PC */
  LST_BT_Send_VarList();

  /* Record starting timestamp */
  TickType_t xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  while (1) {
    LST_Obs_Logic();

    /* Send diagnostic data via BT */
    if (lst_bt_send_diagdata_flag)
      LST_BT_Send_VarValues();

    /* Wait for the next cycle */
    vTaskDelayUntil(&xLastWakeTime, LST_CONTROL_REPEAT_TICKS);
  }
}

/**
 * @brief This task handles the Inertial module
 */
void LST_Task_Inertial(void const * argument) {
  /* Record starting timestamp */
  TickType_t xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  while (1) {
    if(lst_inertial_ready == LST_INERTIAL_READY){
      LST_Inertial_GetSensorData();
    }

    /* Wait for the next cycle */
    vTaskDelayUntil(&xLastWakeTime, LST_TASK_INERTIAL_TASK_REPEAT_TICKS);
  }
}

/**
 * @brief This task handles the BT requests
 */
void LST_Task_BT_RequestHandler(void const * argument) {
  /* Record starting timestamp */
  TickType_t xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  while (1) {
    LST_BT_RequestHandler();

    /* Wait for the next cycle */
    vTaskDelayUntil(&xLastWakeTime, LST_TASK_BT_TASK_REPEAT_TICKS);
  }
}
