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
  LST_BT_Send_StatusOk();
  LST_BT_Send_VarList();

  /* Infinite loop */
  while (1) {
    /* Get line data from LineController */
    LST_SPI_ReceiveLineControllerData();
    LST_SPI_WaitForLineControllerData();

    /* Switch between GamePad and Controller */
    if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_A] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
      lst_control_mode = LST_CONTROL_MODE_BT;
    }
    if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_B] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
      lst_control_mode = LST_CONTROL_MODE_Q1;
    }

    /* Handle PWM controls */
    int16_t steering = 4575;
    int16_t motor = 4575;
    float temp1 = 0;

    switch(lst_control_mode){
    case LST_CONTROL_MODE_BT:
      temp1 = lst_bt_gamepad_values[LST_GAMEPAD_AXIS_LX]
          - LST_GAMEPAD_AXIS_MIDDLE;
      temp1 = temp1 / -21.487f;
      steering += temp1;

      temp1 = lst_bt_gamepad_values[LST_GAMEPAD_AXIS_RY]
          - LST_GAMEPAD_AXIS_MIDDLE;
      temp1 = temp1 / -60.0f;
      motor += temp1;
      break;
    case LST_CONTROL_MODE_Q1:
      /* Set acceleration from GamePad */
      temp1 = lst_bt_gamepad_values[LST_GAMEPAD_AXIS_RY]
          - LST_GAMEPAD_AXIS_MIDDLE;
      temp1 = temp1 / -60.0f;
      motor += temp1;

      /* Get line position from the data */
      steering = LST_Control_SteeringController();
      break;
    }

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, steering);
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, motor);

    /* Send diag data via BT */
    if (lst_bt_send_diagdata_flag)
      LST_BT_Send_VarValues();

    osDelay(10);
    }
}

/**
 * @brief This task handles the BT requests
 */
void LST_Task_BT_Request_Handler(void const * argument) {
  /* Infinite loop */
  while (1) {
    if (lst_bt_send_status_flag)
      LST_BT_Send_StatusOk(); // ToDo send Error status as well
    if (lst_bt_send_varlist_flag)
      LST_BT_Send_VarList();
    osDelay(500);
  }
}
