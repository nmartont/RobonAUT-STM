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
  osThreadDef(LST_Task_BT_Request_Handler, LST_Task_BT_Request_Handler, osPriorityLow, 0, 256);
  lst_task_BTRequestHandlerTaskHandle = osThreadCreate(osThread(LST_Task_BT_Request_Handler), NULL);

  /* Start Q1 */
  osThreadDef(LST_Task_FastLap, LST_Task_FastLap, osPriorityNormal, 0, 1024);
  lst_task_Q1TaskHandle = osThreadCreate(osThread(LST_Task_FastLap), NULL);

  /* Exit starter task */
  osThreadTerminate(lst_task_TaskStartHandle);
}

/**
 * @brief Task for Q1
 */
void LST_Task_FastLap(void const * argument) {
  /* Send Status and VarList to the PC */
  LST_BT_Send_StatusOk();
  LST_BT_Send_VarList();

  /* Record starting timestamp */
  TickType_t xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  while (1) {
    /* Get line data from LineController */
#ifdef LST_CONFIG_UART_LINE_COM
    LST_UART_ReceiveLineControllerData();
#else
    LST_SPI_ReceiveLineControllerData();
#endif

    /* ToDo ADC conversion, I2C, other sensor data */
    /* Calculate speed from encoder */
    LST_TIM_CalculateSpeed();

    /* Wait for the end of the LineController transaction */
#ifdef LST_CONFIG_UART_LINE_COM
    LST_UART_WaitForLineControllerData();
#else
    LST_SPI_WaitForLineControllerData();
#endif

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
