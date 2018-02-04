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
#include "lst_infra.h"
#include "stdlib.h" // abs function

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

/* Function prototypes -------------------------------------------------------*/
void LST_Obs_Init();
void LST_Obs_Logic();

/* Private variables ---------------------------------------------------------*/

/* Obs Search */
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
#define LST_OBS_SEARCH_SHARP_THRESHOLD                  30 //50
#define LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD         300
#define LST_OBS_SEARCH_LED_THRESHOLD                    150 //110
#define LST_OBS_SEARCH_LONG_LINE_SIZE_THRESHOLD         7 //6

/* Obs_Drone */

// Stages
uint8_t lst_obs_drone_stage;
#define LST_OBS_DRO_STAGE_APPROACH						0
#define LST_OBS_DRO_STAGE_WATCH								1
#define LST_OBS_DRO_STAGE_WAIT								2
#define LST_OBS_DRO_STAGE_EXIT								3

// Sharp
#define LST_OBS_DRO_SHARP_DIST_DRONE_IN				300
#define LST_OBS_DRO_SHARP_DIST_DRONE_OUT			400

// Timing
int16_t lst_obs_drone_takeoffTimer;
#define LST_OBS_DRO_TAKEOFFTIMER_PERIOD				100

/* Obs_Corner */

// Stages
uint8_t lst_obs_corner_stage;
#define LST_OBS_COR_STAGE_APPROACH						0
#define LST_OBS_COR_STAGE_PASSEDJUNCTION 			1
#define LST_OBS_COR_STAGE_GETNEARWALL					2
#define LST_OBS_COR_STAGE_CURVEDLINEFOUND 		3
#define LST_OBS_COR_STAGE_BACKING_FIRST				4
#define LST_OBS_COR_STAGE_BACKING_SECOND			5
#define LST_OBS_COR_STAGE_OUTGOING						6
#define LST_OBS_COR_STAGE_CENTER							7
#define LST_OBS_COR_STAGE_ALIGNMENT						8
#define LST_OBS_COR_STAGE_EXIT								9

// Direction control
uint8_t lst_obs_corner_directionControl;
#define LST_OBS_COR_DIR_LEFT									0
#define LST_OBS_COR_DIR_RIGHT									1

// Steering
#define LST_OBS_COR_STEERINGLOCK_LEFT					1100
#define LST_OBS_COR_STEERINGLOCK_RIGHT				-900
#define LST_OBS_COR_RIGHT_LOCK								-500
#define LST_OBS_COR_LEFT_LOCK									500

// Sharp
uint16_t lst_obs_cor_backingSharp_previous;
#define LST_OBS_COR_SHARP_DIST_WALL						340 // 280 was too low
#define LST_OBS_COR_SHARP_FAR_WALL						300
#define LST_OBS_COR_SHARP_BACKING_WALL				340

// Distance
#define LST_OBS_COR_BACKING_DISTANCE					-900 // was -800, found corner...

// Timing
int16_t lst_obs_corner_junctionTimer;
#define LST_OBS_COR_JUNCTIONTIMER_PERIOD			50

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
int16_t lst_obs_train_lastCarTimer;
#define LST_OBS_TRA_LASTCARTIMER_PERIOD			300 // T = 10ms -> 3s

int16_t lst_obs_train_crossingTimer;
#define LST_OBS_TRA_CROSSINGTIMER_PERIOD		60

/* Obs_Convoy */

// Stages
uint8_t lst_obs_convoy_stage;
#define LST_OBS_CON_STAGE_APPROACH			0 // does nothing
#define LST_OBS_CON_STAGE_WATCH					1
#define LST_OBS_CON_STAGE_WAIT					2
#define LST_OBS_CON_STAGE_COUNT					3
#define LST_OBS_CON_STAGE_TURNIN				4
#define LST_OBS_CON_STAGE_FINDIN				5
#define LST_OBS_CON_STAGE_ATTACH				6
#define LST_OBS_CON_STAGE_FOLLOW				7 // Bang-bang type control
#define LST_OBS_CON_STAGE_TURNOUT				8
#define LST_OBS_CON_STAGE_FINDOUT				9
#define LST_OBS_CON_STAGE_LEAVELINE			10 // Leave that perpendicular line which fucks up the search mode
#define LST_OBS_CON_STAGE_EXIT					11

// Direction control
uint8_t lst_obs_convoy_wallDirection; // Set in 'LST_Obs_Search_Sharp_Detection'
#define LST_OBS_CON_WALLDIRECTION_LEFT	0
#define LST_OBS_CON_WALLDIRECTION_RIGHT	1

// Sharp
#define LST_OBS_CON_SHARP_DIST_CAR			350 // Small-range sensor!
#define LST_OBS_CON_SHARP_FOLLOW_LOW		300
#define LST_OBS_CON_SHARP_FOLLOW_HIGH		500
uint8_t lst_obs_convoy_follow_state;

// Timing
int16_t lst_obs_convoy_lastCarTimer;
#define LST_OBS_CON_LASTCARTIMER_PERIOD	200
int16_t lst_obs_convoy_turnTimer;
#define LST_OBS_CON_TURNTIMER_PERIOD		80 //50
int16_t lst_obs_convoy_attachTimer;
#define LST_OBS_CON_ATTACHTIMER_PERIOD	400 //50 Attach until curve is reached

// Steering
#define LST_OBS_CON_STEERINGLOCK_RIGHT	-1000
#define LST_OBS_CON_STEERINGLOCK_LEFT 	1000// TODO two-directional

// Steering watch
#define LST_OBS_CON_STEERING_P 12000
#define LST_OBS_CON_STEERING_D 4000
#define LST_OBS_CON_STEERINGWATCH_HIGH	600
#define LST_OBS_CON_STEERINGWATCH_LOW		300
uint8_t lst_obs_con_steeringCount;
uint8_t lst_obs_con_steeringState;
uint8_t lst_obs_con_steeringHigh;
uint8_t lst_obs_con_steeringLow;
#define LST_OBS_CON_STEERING_THRESHOLD	10
#define LST_OBS_CON_STEERING_COUNT			4 //3

// Distance
#define LST_OBS_CON_DISTANCE_LEAVELINE	1200

/* Obs_Barrel */

// Stages
uint8_t lst_obs_barrel_stage;
#define LST_OBS_BRL_STAGE_APPROACH				0
#define LST_OBS_BRL_STAGE_APPROACHRAMP		1
#define LST_OBS_BRL_STAGE_ONTHERAMP				2
#define LST_OBS_BRL_STAGE_INTHETUBE				3
#define LST_OBS_BRL_STAGE_OUTGOING				4
#define LST_OBS_BRL_STAGE_EXIT						5

// Timing
int16_t lst_obs_barrel_approachTimer;
#define LST_OBS_BRL_APPROACHTIMER_PERIOD	200 // 2s
int16_t lst_obs_barrel_rampTimer;
#define LST_OBS_BRL_RAMPTIMER_PERIOD			50 // 0.5s

/* Obs_Roundabout */
uint8_t lst_obs_roundabout_stage;
uint8_t lst_obs_roundabout_cntr;

#define LST_OBS_RND_STAGE_APPROACH        0
#define LST_OBS_RND_STAGE_FIRST_TURN      1
#define LST_OBS_RND_STAGE_TRAVEL          2
#define LST_OBS_RND_STAGE_LAST_TURN       3
#define LST_OBS_RND_STAGE_FINISH          4

#define LST_OBS_RND_SHARP_DIST_APPROACH   400
#define LST_OBS_RND_INFRA_ERROR_MAX       200

#define LST_OBS_RND_FIRST_LEFT_TURN_VALUE  -1000 // ToDo is the sign correct??
#define LST_OBS_RND_FIRST_RIGHT_TURN_VALUE 1000
#define LST_OBS_RND_LAST_LEFT_TURN_VALUE   -1000
#define LST_OBS_RND_LAST_RIGHT_TURN_VALUE  1000

#define LST_OBS_RND_FIRST_TURN_DISTANCE   300

#define LST_OBS_RND_TRAVEL_DISTANCE_ONE   500
#define LST_OBS_RND_TRAVEL_DISTANCE_TWO   750
#define LST_OBS_RND_TRAVEL_DISTANCE_THREE 1000

#define LST_OBS_RND_FINISH_DISTANCE     400

#define LST_OBS_RND_SHARP_SIDE_DIST     600

/* Obs End */
#define LST_OBS_END_DISTANCE_MM             300

/* External variables --------------------------------------------------------*/

extern uint8_t  lst_control_line_no;

uint8_t lst_temp;

#endif /* LST_OBSTACLE_H_ */
