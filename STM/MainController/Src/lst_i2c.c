/*
 * lst_i2c.c
 *
 *  Created on: 2017. dec. 29.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_i2c.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t lst_i2c_master1_tx[LST_I2C_BUFFER1_SIZE] = { 0x00 };
uint8_t lst_i2c_master1_rx[LST_I2C_BUFFER1_SIZE] = { 0x00 };

uint8_t lst_i2c_mem1_rx_cmplt = LST_I2C_MEMRX_NOT_COMPLETE;

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                    I2C handler for RobonAUT 2018 Team LST                  */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the I2C module
 */
void LST_I2C_Init() {

}

/**
 * @brief Reads from a register
 */
void LST_I2C_LSM6DS3_ReadRegister(uint16_t MemAddress, uint8_t * pData, uint16_t Size){
  /* Wait until I2C1 is ready */
  while(hi2c1.State != HAL_I2C_STATE_READY){}

  // TODO:TEST 2018.01.11
  //HAL_I2C_Mem_Read_IT(&hi2c1, LST_I2C_LSM6DS3_ADDRESS, MemAddress, 1, pData, Size);
  HAL_I2C_Mem_Read(&hi2c1, LST_I2C_LSM6DS3_ADDRESS, MemAddress, 1, pData, Size, 0xff);
}

/**
 * @brief Writes to a register
 */
void LST_I2C_LSM6DS3_WriteRegister(uint16_t MemAddress, uint8_t * pData, uint16_t Size){
  /* Wait until I2C1 is ready */
  while(hi2c1.State != HAL_I2C_STATE_READY){}

  HAL_I2C_Mem_Write(&hi2c1, LST_I2C_LSM6DS3_ADDRESS, MemAddress, 1, pData, Size, 1000);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
  if (hi2c->Instance == I2C1)
  {
    /* Set the i2c transfer complete bit to 1 */
    lst_i2c_mem1_rx_cmplt = LST_I2C_MEMRX_COMPLETE;
  }
}
