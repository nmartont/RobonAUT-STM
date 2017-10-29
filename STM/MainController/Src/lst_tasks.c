/*
 * lst_tasks.c
 *
 *  Created on: 2017. okt. 26.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define SPI_SLAVE_SYNBYTE     0x53
#define SPI_MASTER_SYNBYTE    0xAC

/* Private variables ---------------------------------------------------------*/
uint8_t buffer_tx[8] = {65, 66, 67, 68, 69, 70, 71, 72};
uint8_t pAddrcmd[8] = {0x00};

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern uint8_t buffer_uart2[8];

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

extern uint8_t spi_master_tx[8];
extern uint8_t spi_master_rx[8];
extern uint8_t spi_slave_tx[8];
extern uint8_t spi_slave_rx[8];

/* Function prototypes -------------------------------------------------------*/
void LST_Task_UART_Test(void const * argument);
void LST_Task_SPI_Test(void const * argument);

/******************************************************************************/
/*                  FreeRTOS tasks for RobonAUT 2018 Team LST                 */
/******************************************************************************/

/**
* @brief This task tests the UART communication.
*/
void LST_Task_UART_Test(void const * argument)
{
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&buffer_uart2, 1);

  /* Infinite loop */
  for(;;)
  {
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&buffer_tx, 8);
		osDelay(100);
		osDelay(100);

		buffer_tx[0] += 1;
		buffer_tx[1] += 1;
		buffer_tx[2] += 1;
		buffer_tx[3] += 1;
		buffer_tx[4] += 1;
		buffer_tx[5] += 1;
		buffer_tx[6] += 1;
		buffer_tx[7] += 1;
  }
}

/**
* @brief This task tests the SPI communication.
*/
void LST_Task_SPI_Test(void const * argument)
{
	/* Get 8 bytes from Master */
	// Important: the slave needs to call this function before the master initially starts sending data.
	HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&spi_slave_tx, (uint8_t *)&spi_slave_rx, 8);

  /* Infinite loop */
	while(1)
	{
		/* Synchronization between Master and Slave */
		HAL_SPI_TransmitReceive_IT(&hspi3, (uint8_t *)&spi_master_tx, (uint8_t *)&spi_master_rx, 8);
		osDelay(1);
		osDelay(1);

		spi_master_tx[0] += 1;
		spi_master_tx[1] += 1;
		spi_master_tx[2] += 1;
		spi_master_tx[3] += 1;
		spi_master_tx[4] += 1;
		spi_master_tx[5] += 1;
		spi_master_tx[6] += 1;
		spi_master_tx[7] += 1;
  }
}
