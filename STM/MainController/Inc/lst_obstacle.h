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
#include "lst_dip.h"
#include "stdlib.h" // abs function

/* Defines -------------------------------------------------------------------*/
// #define LST_OBS_MODE_STEERING_INTERPOLATED

#define LST_OBS_STEERING_P 17000
#define LST_OBS_STEERING_D 4000

#define LST_OBS_MODE_BT             0
#define LST_OBS_MODE_LAP            1
#define LST_OBS_MODE_NO_CONTROL     2

#define LST_OBS_LAP_MODE_START      10
#define LST_OBS_LAP_MODE_STARTER		19
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
void LST_Obs_DIP_Handler_Start();

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
#define LST_OBS_SEARCH_LINE_ERROR_THRESHOLD             20 // 100
#define LST_OBS_SEARCH_LONG_LINE_THRESHOLD              20
#define LST_OBS_SEARCH_ONE_LINE_BETWEEN_LONGS_THRESHOLD 1
#define LST_OBS_SEARCH_SHARP_THRESHOLD                  30 //50
#define LST_OBS_SEARCH_SHARP_DISTANCE_THRESHOLD         352 // 300 S!
#define LST_OBS_SEARCH_LED_THRESHOLD                    150 //110
#define LST_OBS_SEARCH_LONG_LINE_SIZE_THRESHOLD         7 //6

/* Obs_Start */

// Stages
uint8_t lst_obs_starter_stage;
#define LST_OBS_STA_STAGE_PREPARE							0
#define LST_OBS_STA_STAGE_FORWARD							1
#define LST_OBS_STA_STAGE_EXIT								2

// Timing
int16_t lst_obs_starter_forwardTimer;
#define LST_OBS_STA_FORWARDTIMER_PERIOD				250

/* Obs_Drone */

// Stages
uint8_t lst_obs_drone_stage;
#define LST_OBS_DRO_STAGE_APPROACH						0
#define LST_OBS_DRO_STAGE_WATCH								1
#define LST_OBS_DRO_STAGE_WAIT								2
#define LST_OBS_DRO_STAGE_EXIT								3

// Sharp
#define LST_OBS_DRO_SHARP_DIST_DRONE_IN				1600
#define LST_OBS_DRO_SHARP_DIST_DRONE_OUT			1200

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
#define LST_OBS_COR_STEERINGLOCK_LEFT					1000 // 900 1100
#define LST_OBS_COR_STEERINGLOCK_RIGHT				-900
#define LST_OBS_COR_RIGHT_LOCK								-500
#define LST_OBS_COR_LEFT_LOCK									500

// Sharp
uint16_t lst_obs_cor_backingSharp_previous;
#define LST_OBS_COR_SHARP_DIST_WALL						304 // S!340 // 280 was too low
#define LST_OBS_COR_SHARP_FAR_WALL						352 // S!300
#define LST_OBS_COR_SHARP_BACKING_WALL				304 // S!340
//#define LST_OBS_COR_SHARP_ALIGN_RAWDISTANCE		600

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
#define LST_OBS_TRA_STAGE_LEAVECROSSING			8
#define LST_OBS_TRA_STAGE_EXIT							9
uint8_t lst_obs_train_repeatedCrossing;

// Sharp
#define LST_OBS_TRA_SHARP_DIST_CAR					600 // S!900 //500

// Timing
int16_t lst_obs_train_lastCarTimer;
#define LST_OBS_TRA_LASTCARTIMER_PERIOD			300 // T = 10ms -> 3s
int16_t lst_obs_train_leaveTimer;
#define LST_OBS_TRA_LEAVETIMER_PERIOD				300

int16_t lst_obs_train_crossingTimer;
#define LST_OBS_TRA_CROSSINGTIMER_PERIOD		60

/* Obs_Convoy */

// Stages
uint8_t lst_obs_convoy_stage;
#define LST_OBS_CON_STAGE_APPROACH			0 // does nothing
#define LST_OBS_CON_STAGE_CLOSEINONE		1
#define LST_OBS_CON_STAGE_CLOSEINTWO		2
#define LST_OBS_CON_STAGE_WATCH					3
//#define LST_OBS_CON_STAGE_WAIT					4
#define LST_OBS_CON_STAGE_COUNT					5
#define LST_OBS_CON_STAGE_TURNIN				6
#define LST_OBS_CON_STAGE_FINDIN				7
#define LST_OBS_CON_STAGE_ATTACH				8
#define LST_OBS_CON_STAGE_FOLLOW				9 // Bang-bang type control
#define LST_OBS_CON_STAGE_TURNOUT				10
#define LST_OBS_CON_STAGE_FINDOUT				11
#define LST_OBS_CON_STAGE_LEAVELINE			12 // Leave that perpendicular line which fucks up the search mode
#define LST_OBS_CON_STAGE_EXIT					13

// Direction control
uint8_t lst_obs_convoy_wallDirection; // Set in 'LST_Obs_Search_Sharp_Detection'
#define LST_OBS_CON_WALLDIRECTION_LEFT	0
#define LST_OBS_CON_WALLDIRECTION_RIGHT	1

// Sharp
#define LST_OBS_CON_SHARP_DIST_CAR			528 // S!200 //300 // 350 // Small-range sensor!
#define LST_OBS_CON_SHARP_FOLLOW_LOW		1200 // 1600
#define LST_OBS_CON_SHARP_FOLLOW_HIGH		700 // 1000
uint8_t lst_obs_convoy_follow_state;

// Timing
int16_t lst_obs_convoy_lastCarTimer;
#define LST_OBS_CON_LASTCARTIMER_PERIOD	200
int16_t lst_obs_convoy_turnTimer;
#define LST_OBS_CON_TURNTIMER_PERIOD		40 //60 same // 80, was too sharp angle
int16_t lst_obs_convoy_attachTimer;
#define LST_OBS_CON_ATTACHTIMER_PERIOD	400 //50 Attach until curve is reached
int16_t lst_obs_convoy_closeInTimer;
#define LST_OBS_CON_CLOSEINTIMER_PERIOD	60
#define LST_OBS_CON_CLOSEINTIMER_ADD		20

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
#define LST_OBS_BRL_STAGE_BRAKE						5
#define LST_OBS_BRL_STAGE_BACKUP					6
#define LST_OBS_BRL_STAGE_FWRIGHT					7
#define LST_OBS_BRL_STAGE_BWRIGHT					8
#define LST_OBS_BRL_STAGE_FWLEFT					9
#define LST_OBS_BRL_STAGE_BWLEFT					10
#define LST_OBS_BRL_STAGE_FOUNDLINE				11
#define LST_OBS_BRL_STAGE_GIVEUP					12
#define LST_OBS_BRL_STAGE_EXIT						13


// Timing
int16_t lst_obs_barrel_approachTimer;
#define LST_OBS_BRL_APPROACHTIMER_PERIOD			300 // 200
int16_t lst_obs_barrel_rampSafetyTimer;
#define LST_OBS_BRL_RAMPSAFETYTIMER_PERIOD		130
int16_t lst_obs_barrel_tubeSafetyTimer;
#define LST_OBS_BRL_TUBESAFETYTIMER_PERIOD		200
int16_t lst_obs_barrel_brakeTimer;
#define LST_OBS_BRL_BRAKETIMER_PERIOD					80
int16_t lst_obs_barrel_moveTimer;
#define LST_OBS_BRL_BACKUPTIMER_PERIOD				140
#define LST_OBS_BRL_FWRIGHTTIMER_PERIOD				100
#define LST_OBS_BRL_BWRIGHTTIMER_PERIOD				100
#define LST_OBS_BRL_FWLEFTTIMER_PERIOD				100
#define LST_OBS_BRL_BWLEFTTIMER_PERIOD				100
int16_t lst_obs_barrel_exitTimer;
#define LST_OBS_BRL_ALIGNTIMER_PERIOD					200
#define LST_OBS_BRL_EXITTIMER_PERIOD					100

// Sharp
#define LST_OBS_BRL_SHARP_INTHETUBE						290 // S!370

// Steering
#define LST_OBS_BRL_STEERING_RIGHT						-800
#define LST_OBS_BRL_STEERING_LEFT							800

// Line count
uint16_t lst_obs_barrel_foundCounter;
#define LST_OBS_BRL_FOUNDCOUNTER_PERIOD				10

/* Obs_Roundabout */
uint8_t lst_obs_roundabout_stage;
uint16_t lst_obs_roundabout_cntr;

#define LST_OBS_RND_STAGE_APPROACH        0
#define LST_OBS_RND_STAGE_FIRST_TURN      1
#define LST_OBS_RND_STAGE_TRAVEL          2
#define LST_OBS_RND_STAGE_LAST_TURN       3
#define LST_OBS_RND_STAGE_ALIGN						4
#define LST_OBS_RND_STAGE_FINISH          5
#define LST_OBS_RND_STAGE_EXIT						6

#define LST_OBS_RND_SHARP_DIST_APPROACH   840 // S!600 //900
#define LST_OBS_RND_INFRA_ERROR_MAX       200 // FixMe NMT

#define LST_OBS_RND_FIRST_LEFT_TURN_VALUE  1000
#define LST_OBS_RND_FIRST_RIGHT_TURN_VALUE -1000
#define LST_OBS_RND_LAST_LEFT_TURN_VALUE   1000
#define LST_OBS_RND_LAST_RIGHT_TURN_VALUE  -1000

#define LST_OBS_RND_FIRST_TURN_DISTANCE   350

#define LST_OBS_RND_TRAVEL_DISTANCE_ONE   200 // 300 ?? // 500 way too long
#define LST_OBS_RND_TRAVEL_DISTANCE_TWO   1600 // 1800
#define LST_OBS_RND_TRAVEL_DISTANCE_THREE 3000 //3200

#define LST_OBS_RND_FINISH_DISTANCE     400

#define LST_OBS_RND_SHARP_SIDE_DIST     1400 //600 // Raw shortSharp value

// Timing
int16_t lst_obs_roundabout_turnInTimer;
#define LST_OBS_RND_TURNINTIMER_PERIOD	160 //180
int16_t lst_obs_roundabout_turnOutTimer;
#define LST_OBS_RND_TURNOUTTIMER_PERIOD	50

/* Obs End */
#define LST_OBS_END_DISTANCE_MM             300

/* External variables --------------------------------------------------------*/

extern uint8_t  lst_control_line_no;

uint8_t lst_temp;

#endif /* LST_OBSTACLE_H_ */
