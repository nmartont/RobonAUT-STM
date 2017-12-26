/*
 * lst_obstacle.h
 *
 *  Created on: 2017. dec. 26.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_OBSTACLE_H_
#define LST_OBSTACLE_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void LST_Obs_Init();
void LST_Obs_Logic();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

#endif /* LST_OBSTACLE_H_ */
