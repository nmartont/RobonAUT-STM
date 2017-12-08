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
 * @brief This task starts the MainController tasks.
 */
void LST_Task_Start(void const * argument) {
  /* Start BT request handler */
  osThreadDef(LST_Task_BT_Request_Handler, LST_Task_BT_Request_Handler, osPriorityLow, 0, 128);
  lst_task_BTRequestHandlerTaskHandle = osThreadCreate(osThread(LST_Task_BT_Request_Handler), NULL);

  /* Start Q1 */
  osThreadDef(LST_Task_Q1, LST_Task_Q1, osPriorityNormal, 0, 128);
  lst_task_Q1TaskHandle = osThreadCreate(osThread(LST_Task_Q1), NULL);

  /* Exit starter task */
  osThreadTerminate(lst_task_TaskStartHandle);
}

/**
 * @brief Task for Q1
 */
void LST_Task_Q1(void const * argument) {
  /* Send Status and VarList to the PC */
  LST_BT_Send_StatusOk();
  LST_BT_Send_VarList();

  /* Record starting timestamp */
  TickType_t xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  while (1) {
    /* Get line data from LineController */
    LST_SPI_ReceiveLineControllerData();

    /* ToDo ADC conversion, I2C, other sensor data */

    /* Wait for the end of the SPI transaction */
    LST_SPI_WaitForLineControllerData();

    /* ToDo Check for 0xFF control byte at the first byte of the SPI Rx buffer */
    /* ToDo Handle SPI Rx data in a separate module */

    /* Hand over control to the Controls module */
    LST_Control();

    /* Send diag data via BT */
    if (lst_bt_send_diagdata_flag)
      LST_BT_Send_VarValues();

    /* Wait for the next cycle */
    vTaskDelayUntil(&xLastWakeTime, LST_TASK_Q1_TASK_REPEAT_TICKS);
  }
}

/**
 * @brief This task handles the BT requests
 */
void LST_Task_BT_Request_Handler(void const * argument) {
  /* Record starting timestamp */
  TickType_t xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  while (1) {
    /* Error handling for the BT module */
    LST_BT_ErrorHandler();

    if (lst_bt_send_status_flag)
      LST_BT_Send_StatusOk(); // ToDo send Error status as well?
    if (lst_bt_send_varlist_flag)
      LST_BT_Send_VarList();

    /* Wait for the next cycle */
    vTaskDelayUntil(&xLastWakeTime, LST_TASK_BT_TASK_REPEAT_TICKS);
  }
}
