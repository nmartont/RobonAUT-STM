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
#include "lst_radio.h"
#include "control/lst_movement.h"
#include "control/lst_steering.h"
#include "control/lst_distance.h"
#include "control/lst_movement.h"
#include "lst_sharp.h"

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

/* Private variables ---------------------------------------------------------*/

// Obs_Corner
uint8_t lst_obs_corner_stage;
#define LST_OBS_COR_STAGE_APPROACH						0
#define LST_OBS_COR_STAGE_PASSEDJUNCTION 			1
#define LST_OBS_COR_STAGE_CURVEDLINEFOUND 		2
#define LST_OBS_COR_STAGE_BACKING_FIRST				3
#define LST_OBS_COR_STAGE_BACKING_SECOND			4
#define LST_OBS_COR_STAGE_BACKING_THIRD				5
#define LST_OBS_COR_STAGE_EXIT								6
#define LST_OBS_COR_STEERINGLOCK							1000 // Steering turned left

/* External variables --------------------------------------------------------*/

extern uint8_t  lst_control_line_no;

uint8_t lst_temp;

#endif /* LST_OBSTACLE_H_ */
