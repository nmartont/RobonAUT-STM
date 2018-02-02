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
#define LST_OBS_LAP_MODE_END        18

// Obs_Search
#define LST_OBS_SEARCH_MODE_BEGIN           0
#define LST_OBS_SEARCH_MODE_LONGLINE        1
#define LST_OBS_SEARCH_MODE_ROUNDABOUT      2
#define LST_OBS_SEARCH_MODE_BARREL          3
#define LST_OBS_SEARCH_MODE_DRONE           4
#define LST_OBS_SEARCH_MODE_CONVOY          5
#define LST_OBS_SEARCH_MODE_CORNER          6
#define LST_OBS_SEARCH_MODE_FOUND           10


#define LST_OBS_SEARCH_LINE_THRESHOLD                   4
#define LST_OBS_SEARCH_LINE_ERROR_THRESHOLD             100
#define LST_OBS_SEARCH_LONG_LINE_THRESHOLD              20
#define LST_OBS_SEARCH_ONE_LINE_BETWEEN_LONGS_THRESHOLD 1
#define LST_OBS_SEARCH_SHARP_THRESHOLD                  10
#define LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD         300
#define LST_OBS_SEARCH_LED_THRESHOLD                    110
#define LST_OBS_SEARCH_LONG_LINE_SIZE_THRESHOLD         6 //9


/* Function prototypes -------------------------------------------------------*/
void LST_Obs_Init();
void LST_Obs_Logic();

/* Private variables ---------------------------------------------------------*/

/* Obs_Corner */

// Stages
uint8_t lst_obs_corner_stage;
#define LST_OBS_COR_STAGE_APPROACH						0
#define LST_OBS_COR_STAGE_PASSEDJUNCTION 			1
#define LST_OBS_COR_STAGE_CURVEDLINEFOUND 		2
#define LST_OBS_COR_STAGE_BACKING_FIRST				3
#define LST_OBS_COR_STAGE_BACKING_SECOND			4
#define LST_OBS_COR_STAGE_BACKING_THIRD				5
#define LST_OBS_COR_STAGE_OUTGOING						6
#define LST_OBS_COR_STAGE_ALIGNMENT						7
#define LST_OBS_COR_STAGE_EXIT								8

// Steering
#define LST_OBS_COR_STEERINGLOCK							1100 // Steering turned left
#define LST_OBS_COR_RIGHT_LOCK								-500
#define LST_OBS_COR_LEFT_LOCK									500

// Sharp
uint16_t lst_obs_cor_rightSharp_previous;
#define LST_OBS_COR_SHARP_DIST_WALL						280

/* Obs_Trainstop */

// Stages
uint8_t lst_obs_train_stage;
#define LST_OBS_TRA_STAGE_PREPARE						0
#define LST_OBS_TRA_STAGE_APPROACH					1
#define LST_OBS_TRA_STAGE_WATCH							2
#define LST_OBS_TRA_STAGE_WAIT							3
#define LST_OBS_TRA_STAGE_COUNT							4
#define LST_OBS_TRA_STAGE_CROSS							5
#define LST_OBS_TRA_STAGE_CROSSING					6
#define LST_OBS_TRA_STAGE_CROSSED						7
#define LST_OBS_TRA_STAGE_EXIT							8
uint8_t lst_obs_train_repeatedCrossing;

// Sharp
#define LST_OBS_TRA_SHARP_DIST_CAR					500

// Timing
int16_t lst_obs_tra_lastCarTimer;
#define LST_OBS_TRA_LASTCARTIMER_PERIOD			300 // T = 10ms -> 3s

int16_t lst_obs_tra_crossingTimer;
#define LST_OBS_TRA_CROSSINGTIMER_PERIOD		60

/* External variables --------------------------------------------------------*/

extern uint8_t  lst_control_line_no;

uint8_t lst_temp;

#endif /* LST_OBSTACLE_H_ */
