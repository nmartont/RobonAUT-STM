/*
 * lst_tests.c
 *
 *  Created on: 2017. nov. 17.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include <lst_test.h>

#ifdef LST_CONFIG_TEST
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#ifdef LST_CONFIG_CHANGE_BT_BAUD
uint8_t buffer_baud[33] = "AT+AB ChangeDefaultBaud 2000000\r\n";
#endif

uint8_t buffer_test_error_string[9] = "TestError";

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                  FreeRTOS tests for RobonAUT 2018 Team LST                 */
/******************************************************************************/

/**
* @brief This task starts the MainController tests.
*/
void LST_Test_Start(void const * argument)
{
	/* Start a test */
	osThreadDef(LST_Test_PWM, LST_Test_PWM, osPriorityNormal, 0, 128);
	lst_test_BtTestHandle = osThreadCreate(osThread(LST_Test_PWM), NULL);

	/* Exit starter task */
	osThreadTerminate(lst_test_StartTestHandle);
}

/**
* @brief This task tests the UART communication.
*/
void LST_Test_UART(void const * argument)
{
  /* Infinite loop */
  for(;;)
  {
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&lst_uart_buffer_tx, 8);
		osDelay(100);
		osDelay(100);

		lst_uart_buffer_tx[0] += 1;
//		lst_uart_buffer_tx[1] += 1;
//		lst_uart_buffer_tx[2] += 1;
//		lst_uart_buffer_tx[3] += 1;
//		lst_uart_buffer_tx[4] += 1;
//		lst_uart_buffer_tx[5] += 1;
//		lst_uart_buffer_tx[6] += 1;
//		lst_uart_buffer_tx[7] += 1;
  }
}

/**
* @brief This task tests the SPI communication.
*/
void LST_Test_SPI(void const * argument)
{
	/* Get 8 bytes from Master */
	// Important: the slave needs to call this function before the master initially starts sending data.
	HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&lst_spi_slave_tx, (uint8_t *)&lst_spi_slave_rx, 8);

  /* Infinite loop */
	while(1)
	{
		/* Synchronization between Master and Slave */
		HAL_SPI_TransmitReceive_IT(&hspi3, (uint8_t *)&lst_spi_master_tx, (uint8_t *)&lst_spi_master_rx, 8);
		osDelay(1);
		osDelay(1);

		lst_spi_master_tx[0] += 1;
		lst_spi_master_tx[1] += 1;
		lst_spi_master_tx[2] += 1;
		lst_spi_master_tx[3] += 1;
		lst_spi_master_tx[4] += 1;
		lst_spi_master_tx[5] += 1;
		lst_spi_master_tx[6] += 1;
		lst_spi_master_tx[7] += 1;
  }
}

/**
* @brief This task tests the Timer.
*/
void LST_Test_TIM(void const * argument)
{
	HAL_TIM_Base_Start_IT(&htim8);
	// HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);

	/* Infinite loop */
	while(1)
	{
		// tim_val = __HAL_TIM_GET_COUNTER(&htim6);
		osDelay(1);
	}
}

/**
* @brief This task tests the Bluetooth.
*/
void LST_Test_BT(void const * argument)
{

#ifdef LST_CONFIG_CHANGE_BT_BAUD
	/* Send BT baud change command */
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&buffer_baud, 33);
#endif

	LST_BT_Send_StatusOk();
	LST_BT_Send_StatusError((uint8_t *)&buffer_test_error_string[0], 9);
	LST_BT_Send_StatusRequest();
	LST_BT_Send_VarList();

	/* Infinite loop */
	while(1)
	{
		// LST_BT_Send_VarValues();
		osDelay(10);
	}
}

/**
* @brief This task tests the ADC.
*/
void LST_Test_ADC(void const * argument)
{

	/* Infinite loop */
	while(1)
	{
		HAL_ADC_Start_IT(&hadc3);
		osDelay(1);
	}
}

/**
* @brief Test for PWM
*/
void LST_Test_PWM(void const * argument)
{
	LST_BT_Send_StatusOk();
	LST_BT_Send_VarList();

	/* Infinite loop */
	while(1)
	{
		int16_t steering = 4575;
		int16_t motor = 4575;

		int16_t new_st = lst_bt_gamepad_values[LST_GAMEPAD_AXIS_LX] - LST_GAMEPAD_AXIS_MIDDLE;
		new_st = new_st / -21.487f;
		steering += new_st;

		int16_t new_motor = lst_bt_gamepad_values[LST_GAMEPAD_AXIS_RY] - LST_GAMEPAD_AXIS_MIDDLE;
		new_motor = new_motor / -60.0f;
		motor += new_motor;

		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, steering);
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, motor);
		if(lst_bt_send_diagdata_flag) LST_BT_Send_VarValues();
		osDelay(10);

/*		for(int i=4500; i<=5100; i = i+10){

			__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, i);
			osDelay(50);
		}
		osDelay(1500);

		for(int i=5300; i>=4400; i = i-10){

			__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, i);
			osDelay(50);
		}
		for(int i=4400; i<=4500; i = i+10){

			__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, i);
			osDelay(50);
		}

		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 4500);
			osDelay(2000);

//Backward

		for(int i=4500; i>=3900; i = i-10){

			__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, i);
			osDelay(50);
		}
			osDelay(1500);

		for(int i=3900; i<=4600; i = i+10){

			__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, i);
			osDelay(50);
		}
		for(int i=4600; i>=4500; i = i-10){

			__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, i);
			osDelay(50);
		}
		osDelay(2000);
*/
	}

}

#endif /* LST_CONFIG_TEST */
