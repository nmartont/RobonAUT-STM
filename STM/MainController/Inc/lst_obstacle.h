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
#include "lst_control.h"

/* Defines -------------------------------------------------------------------*/
#define LST_OBS_STEERING_P 17000
#define LST_OBS_STEERING_D 4000

#define LST_OBS_MODE_BT             0
#define LST_OBS_MODE_LAP            1
#define LST_OBS_MODE_NO_CONTROL     2

#define LST_OBS_LAP_MODE_START      10
#define LST_OBS_LAP_MODE_SEARCH     11
#define LST_OBS_LAP_MODE_DRONE      12
#define LST_OBS_LAP_MODE_CORNER     13
#define LST_OBS_LAP_MODE_CONVOY     14
#define LST_OBS_LAP_MODE_BARREL     15
#define LST_OBS_LAP_MODE_ROUNDABOUT 16
#define LST_OBS_LAP_MODE_TRAINSTOP  17

/* Function prototypes -------------------------------------------------------*/
void LST_Obs_Init();
void LST_Obs_Logic();
void LST_Obs_ResetStateMachine();
void LST_Obs_GamePadHandler();
void LST_Obs_StateMachine();
void LST_Obs_Lap();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

#endif /* LST_OBSTACLE_H_ */
