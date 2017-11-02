/*
 * lst_tasks.c
 *
 *  Created on: 2017. okt. 26.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_tasks.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                  FreeRTOS tasks for RobonAUT 2018 Team LST                 */
/******************************************************************************/

/**
* @brief This task tests the UART communication.
*/
void LST_Tasks_UART_Test(void const * argument)
{
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&lst_uart_buffer_uart2, 1);

  /* Infinite loop */
  for(;;)
  {
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&lst_uart_buffer_tx, 8);
		osDelay(100);
		osDelay(100);

		lst_uart_buffer_tx[0] += 1;
		lst_uart_buffer_tx[1] += 1;
		lst_uart_buffer_tx[2] += 1;
		lst_uart_buffer_tx[3] += 1;
		lst_uart_buffer_tx[4] += 1;
		lst_uart_buffer_tx[5] += 1;
		lst_uart_buffer_tx[6] += 1;
		lst_uart_buffer_tx[7] += 1;
  }
}

/**
* @brief This task tests the SPI communication.
*/
void LST_Tasks_SPI_Test(void const * argument)
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
void LST_Tasks_TIM_Test(void const * argument)
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
void LST_Tasks_BT_Test(void const * argument)
{
	/* Turn on Bluetooth power */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	/* Receive a byte on UART2 */
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&lst_uart_buffer_uart2, 1);

	/* Infinite loop */
	while(1)
	{
		/* Transmit data to PC */
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&lst_uart_buffer_tx, 3);
		osDelay(100);
	}
}
