/*
 * lst_inertial.h
 *
 *  Created on: 2017. dec. 29.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_INERTIAL_H_
#define LST_INERTIAL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"
#include "lst_i2c.h"

/* Defines -------------------------------------------------------------------*/
#define LST_INERTIAL_WHO_AM_I_VALUE 0x69

#define LST_INERTIAL_NOT_READY 0
#define LST_INERTIAL_READY     1

#define LST_INERTIAL_ACCEL_RANGE 16
#define LST_INERTIAL_GYRO_RANGE  2000

/* Function prototypes -------------------------------------------------------*/
void LST_Inertial_Init();
void LST_Inertial_GetSensorData();
float LST_Inertial_CalcAccel(int16_t input);
float LST_Inertial_CalcGyro(int16_t input);

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern float lst_inertial_sensor_data[6];

#endif /* LST_INERTIAL_H_ */
