/*
 * lst_inertial.c
 *
 *  Created on: 2017. dec. 29.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_inertial.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t lst_inertial_ready = LST_INERTIAL_NOT_READY;
float lst_inertial_sensor_data[6] = { 0.0f };

/* External variables --------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static float calc_accel(int16_t input);
static float calc_gyro(int16_t input);

/******************************************************************************/
/*            Inertial sensor handler for RobonAUT 2018 Team LST              */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Inertial module
 */
void LST_Inertial_Init() {
  volatile uint16_t cntr = 0;

  /* Check if LSM6DS3 is ready */
  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_WHO_AM_I_REG, (uint8_t *) &lst_i2c_master1_rx, 1);

  if(lst_i2c_master1_rx[0] == LST_INERTIAL_WHO_AM_I_VALUE){
    lst_inertial_ready = LST_INERTIAL_READY;
  }
  else{
    lst_inertial_ready = LST_INERTIAL_NOT_READY;
  }

  /* Setup the accelerometer */
  lst_i2c_master1_tx[0] = 0x00;
  lst_i2c_master1_tx[0] |= LSM6DS3_ACC_GYRO_BW_XL_200Hz;  // First patch in filter bandwidth
  lst_i2c_master1_tx[0] |= LSM6DS3_ACC_GYRO_FS_XL_16g;    // Next, patch in full scale
  lst_i2c_master1_tx[0] |= LSM6DS3_ACC_GYRO_ODR_XL_416Hz; // Lastly, patch in accelerometer ODR
  LST_I2C_LSM6DS3_WriteRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, (uint8_t *) &lst_i2c_master1_tx, 1);

  /* Set the ODR bit */
//  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_CTRL4_C, (uint8_t *) &lst_i2c_master1_rx, 1);
//  lst_i2c_master1_rx[0] &= ~((uint8_t)LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED);
//  LST_I2C_LSM6DS3_WriteRegister(LSM6DS3_ACC_GYRO_CTRL4_C, (uint8_t *) &lst_i2c_master1_rx, 1);

  /* Setup the gyroscope */
  lst_i2c_master1_tx[0] = 0x00;
  lst_i2c_master1_tx[0] |= LSM6DS3_ACC_GYRO_FS_G_2000dps;   //First, patch in full scale
  lst_i2c_master1_tx[0] |= LSM6DS3_ACC_GYRO_ODR_G_416Hz;    //Lastly, patch in gyro ODR
  LST_I2C_LSM6DS3_WriteRegister(LSM6DS3_ACC_GYRO_CTRL2_G, (uint8_t *) &lst_i2c_master1_tx, 1);
}

/**
 * @brief Gets all sensor data from accelerometer and gyroscope, converts to SI.
 */
void LST_Inertial_GetSensorData(){
  // volatile uint16_t cntr = 0;

  /* Get 6DoF inertial sensor data */
  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTX_L_XL, (uint8_t *) &lst_i2c_master1_rx[0], 1);
  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTX_H_XL, (uint8_t *) &lst_i2c_master1_rx[1], 1);

  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTY_L_XL, (uint8_t *) &lst_i2c_master1_rx[2], 1);
  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTY_H_XL, (uint8_t *) &lst_i2c_master1_rx[3], 1);

  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTZ_L_XL, (uint8_t *) &lst_i2c_master1_rx[4], 1);
  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTZ_H_XL, (uint8_t *) &lst_i2c_master1_rx[5], 1);

  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTX_L_G, (uint8_t *) &lst_i2c_master1_rx[6], 1);
  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTX_H_G, (uint8_t *) &lst_i2c_master1_rx[7], 1);

  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTY_L_G, (uint8_t *) &lst_i2c_master1_rx[8], 1);
  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTY_H_G, (uint8_t *) &lst_i2c_master1_rx[9], 1);

  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTZ_L_G, (uint8_t *) &lst_i2c_master1_rx[10], 1);
  LST_I2C_LSM6DS3_ReadRegister(LSM6DS3_ACC_GYRO_OUTZ_H_G, (uint8_t *) &lst_i2c_master1_rx[11], 1);

  /* Convert acceleration data to floats */
  int16_t raw_val = 0;
  for(uint8_t i = 0; i<3; i++){
    raw_val = (lst_i2c_master1_rx[2*i + 1] << 8) | (lst_i2c_master1_rx[2*i]);
    lst_inertial_sensor_data[i] = calc_accel(raw_val);
  }

  /* Convert gyroscope data to floats */
  for(uint8_t i = 3; i<6; i++){
    raw_val = (lst_i2c_master1_rx[2*i + 1] << 8) | (lst_i2c_master1_rx[2*i]);
    lst_inertial_sensor_data[i] = calc_gyro(raw_val);
  }
}

/**
 * @brief Calculates the acceleration value from the raw input.
 */
static float calc_accel(int16_t input){
  float output = (float)input * 0.061f * (LST_INERTIAL_ACCEL_RANGE >> 1) / 1000;
  return output;
}

/**
 * @brief Calculates the gyroscope value from the raw input.
 */
static float calc_gyro(int16_t input){
  uint8_t gyroRangeDivisor = LST_INERTIAL_GYRO_RANGE / 125;
  if ( LST_INERTIAL_GYRO_RANGE == 245 ) {
    gyroRangeDivisor = 2;
  }

  float output = (float)input * 4.375f * (gyroRangeDivisor) / 1000;
  return output;
}
