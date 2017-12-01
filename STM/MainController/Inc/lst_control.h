/*
 * lst_control.h
 *
 *  Created on: 2017. nov. 28.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_CONTROL_H_
#define LST_CONTROL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"
#include "lst_spi.h"

/* Defines -------------------------------------------------------------------*/
#define LST_CONTROL_MODE_BT 0
#define LST_CONTROL_MODE_Q1 1

#define LST_CONTROL_LINESENSOR_TRESHOLD 130
#define LST_CONTROLLER_STEERING_P       1.25f
#define LST_CONTROLLER_STEERING_I       0.01f

/* Function prototypes -------------------------------------------------------*/
void LST_Control_Init();
float LST_Control_GetLinePosition();
int16_t LST_Control_SteeringController();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t lst_control_mode;

#endif /* LST_CONTROL_H_ */
