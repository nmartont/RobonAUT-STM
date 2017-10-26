/*
 * lst_spi.c
 *
 *  Created on: 2017. okt. 26.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
uint8_t cntr_spi1 = 0;
uint8_t buffer_spi1[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* External variables --------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

/******************************************************************************/
/*             SPI Communication handling for RobonAUT 2018 Team LST          */
/******************************************************************************/

/**
* @brief This function handles the SPI receive complete callback.
*/
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){
	cntr_spi1++;
	HAL_SPI_Receive_IT(&hspi1, (uint8_t *)&buffer_spi1, 8);
}
