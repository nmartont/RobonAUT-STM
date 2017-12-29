/*
 * lst_i2c.h
 *
 *  Created on: 2017. dec. 29.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_I2C_H_
#define LST_I2C_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"
#include "LSM6DS3.h"

/* Defines -------------------------------------------------------------------*/
#define LST_I2C_LSM6DS3_ADDRESS 0x6A
#define LST_I2C_BUFFER1_SIZE    16

/* Function prototypes -------------------------------------------------------*/
void LST_I2C_Init();
void LST_I2C_LSM6DS3_ReadRegister(uint16_t MemAddress, uint8_t * pData, uint16_t Size);
void LST_I2C_LSM6DS3_WriteRegister(uint16_t MemAddress, uint8_t * pData, uint16_t Size);
/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;

extern uint8_t lst_i2c_master1_tx[LST_I2C_BUFFER1_SIZE];
extern uint8_t lst_i2c_master1_rx[LST_I2C_BUFFER1_SIZE];

#endif /* LST_I2C_H_ */
