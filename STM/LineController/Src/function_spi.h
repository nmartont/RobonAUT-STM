/*
 * function_spi.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#ifndef FUNCTION_SPI_H_
#define FUNCTION_SPI_H_
#endif /* FUNCTION_SPI_H_ */

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_spi.h"

extern SPI_HandleTypeDef hspi1; // inter-STM
extern SPI_HandleTypeDef hspi2; // TCRT LED driver

uint8_t spi_interSTM_writeCompleteFlag;
uint8_t spi_ledDriver_writeCompleteFlag;
