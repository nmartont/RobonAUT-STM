/*
 * lst_obstacle.c
 *
 *  Created on: 2017. dec. 26.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_obstacle.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t lst_obs_mode = LST_OBS_MODE_BT;
uint8_t lst_obs_lap_mode = LST_OBS_LAP_MODE_START;

/* External variables --------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void LST_Obs_ResetStateMachine();
static void LST_Obs_GamePadHandler();
static void LST_Obs_StateMachine();
static void LST_Obs_Lap();
static void LST_Obs_Search();
static void LST_Obs_Drone();
static void LST_Obs_Corner();
static void LST_Obs_Convoy();
static void LST_Obs_Barrel();
static void LST_Obs_Roundabout();
static void LST_Obs_Trainstop();

/******************************************************************************/
/*                Obstacle lap handler for RobonAUT 2018 Team LST             */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Obstacle lap module
 */
void LST_Obs_Init() {
  lst_control_steeringP = LST_OBS_STEERING_P;
  lst_control_steeringD = LST_OBS_STEERING_D;

  // INIT TODO TEST
  lst_temp = 0;

}

/**
 * @brief Main handler logic of the obstacle lap
 */
void LST_Obs_Logic(){
  /* Get line data, sensor data, etc */
  LST_Control_Commons();

  /* Switch between control modes based on the GamePad */
  LST_Obs_GamePadHandler();

  /* Main state machine for the Obstacle Lap mode (eg. BT control, lap mode, etc) */
  LST_Obs_StateMachine();

  /* Set the control of the servo and motor */
  LST_Control_ServoAndMotor();
}

/**
 * @brief Main state machine of the obstacle lap mode
 */
static void LST_Obs_StateMachine(){
  switch(lst_obs_mode){
  case LST_OBS_MODE_BT:
    LST_Obs_ResetStateMachine();

    lst_control_steering = LST_Control_Servo_BT();
    lst_control_motor = LST_Control_Motor_BT();
    break;
  case LST_OBS_MODE_LAP:

    /* Q1 logic */

  	// Periodic controls
  	LST_Movement_Set();
  	LST_Steering_Set();

  	// Obstacle logic
    LST_Obs_Lap();

    break;
  case LST_OBS_MODE_NO_CONTROL:
    /* Leave values on default */
  default:
    /* Leave values at default */
    LST_Obs_ResetStateMachine();
    break;
  }
}

/**
 * @brief The state machine of the lap mode
 */
static void LST_Obs_Lap(){
  switch(lst_obs_lap_mode){
  case LST_OBS_LAP_MODE_START:
    /* Return if the radio hasn't received the start message */
    if(lst_radio_msg_0_received != LST_RADIO_MSG_RECEIVED) return;

    /* Switch to SEARCH mode */
    lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;
    break;
  case LST_OBS_LAP_MODE_SEARCH:
    LST_Obs_Search();
    break;
  case LST_OBS_LAP_MODE_DRONE:
    LST_Obs_Drone();
    break;
  case LST_OBS_LAP_MODE_CORNER:
    LST_Obs_Corner();
    break;
  case LST_OBS_LAP_MODE_CONVOY:
    LST_Obs_Convoy();
    break;
  case LST_OBS_LAP_MODE_BARREL:
    LST_Obs_Barrel();
    break;
  case LST_OBS_LAP_MODE_ROUNDABOUT:
    LST_Obs_Roundabout();
    break;
  case LST_OBS_LAP_MODE_TRAINSTOP:
    LST_Obs_Trainstop();
    break;
  default:
    break;
  }
}

/**
 * @brief Mode for searching obstacles
 */
static void LST_Obs_Search(){
  // ToDo

	LST_Movement_Stop();

}

static void LST_Obs_Drone(){
  // ToDo TEST 2018. 02. 01.

	LST_Steering_Follow();

	if (LST_Sharp_GetFrontDistance_mm() > 300)
	{

		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

	}
	else
	{

		LST_Movement_Stop();

	}

}

/**
 * @brief Mode for the corner
 */
static void LST_Obs_Corner(){

	switch (lst_obs_corner_stage)
	{

	case LST_OBS_COR_STAGE_APPROACH:

		LST_Steering_Follow();

		// Slow speed
		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		// Jump to next stage if the Line disappears (in the center of the junction)
		if (lst_control_line_no < 1) lst_obs_corner_stage =
				LST_OBS_COR_STAGE_PASSEDJUNCTION;

		break;

	case LST_OBS_COR_STAGE_PASSEDJUNCTION:

		LST_Steering_Follow();

		// Slow down
		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		// Jump
		if (lst_control_line_no > 1)
			{

			// Advance a certain distance to prepare for backing up
			// (start the measurement)
			//LST_Distance_Measure_mm(300); // NEXT STAGE A VERSION !!!

			// Next stage
			lst_obs_corner_stage =
					LST_OBS_COR_STAGE_CURVEDLINEFOUND;

			}


		break;

	case LST_OBS_COR_STAGE_CURVEDLINEFOUND:

		// Lock because curved line interferes
		LST_Steering_Lock(0);

		/* A VERSION
		// Continue ahead until the distance is reached
		if (!LST_Distance_Measure_mm(0))
		{

			LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		}
		else
		{

			LST_Movement_Stop(); // Not really needed

			// Next stage
			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING;

		}
		*/

		// B VERSION - move until wall ends

		LST_Movement_Move(LST_MOVEMENT_FB_SLOWEST);

		if (LST_Sharp_GetRightDistance_mm() > 200)
		{

			LST_Movement_Stop(); // Not really needed

			// Next stage
			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING_FIRST;

			LST_Distance_Measure_mm(-800);

		}

		break;


	// STAGE: back up until the wall to the right is lost
	case LST_OBS_COR_STAGE_BACKING_FIRST:

		LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK);

		//LST_Movement_Move(LST_MOVEMENT_FB_BACKING_SLOWEST);
		LST_Movement_Move_Encoderless(LST_MOVEMENT_BACKING_SLOW);

		// Back up a bit to lose the wall to the right
		if (LST_Distance_Measure_mm(0))
		{

			// Next stage
			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING_SECOND;

			// Init var for next
			lst_obs_cor_rightSharp_previous = 10000;

		}

		break;

	// STAGE: Back up until the right Sharp distance is increasing again
	case LST_OBS_COR_STAGE_BACKING_SECOND:

		LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK);

		//LST_Movement_Move(LST_MOVEMENT_FB_BACKING_SLOWEST);
		LST_Movement_Move_Encoderless(LST_MOVEMENT_BACKING_SLOW);

		if (LST_Sharp_GetRightDistance_mm() > lst_obs_cor_rightSharp_previous)
		{

			lst_obs_corner_stage = LST_OBS_COR_STAGE_BACKING_THIRD;
			LST_Distance_Measure_mm(-150);

		}
		else
		{

			lst_obs_cor_rightSharp_previous = LST_Sharp_GetRightDistance_mm();

		}

		break;

	// STAGE: Back up a bit more to align better
	case LST_OBS_COR_STAGE_BACKING_THIRD:

		LST_Steering_Lock(LST_OBS_COR_STEERINGLOCK);

		// Back up a bit to lose the wall to the right
		if (LST_Distance_Measure_mm(0))
		{

			// Next stage
			lst_obs_corner_stage = lst_obs_corner_stage =
					LST_OBS_COR_STAGE_OUTGOING;

		}

		break;

	case LST_OBS_COR_STAGE_OUTGOING:

		LST_Steering_Follow();

		// Slow speed
		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		if ((LST_Sharp_GetLeftDistance_mm() < LST_OBS_COR_SHARP_DIST_WALL) &&
				(LST_Sharp_GetRightDistance_mm() < LST_OBS_COR_SHARP_DIST_WALL))
		{

			// Between outgoing walls, heading for exit
			lst_obs_corner_stage =
								LST_OBS_COR_STAGE_ALIGNMENT;

		}

		break;

	case LST_OBS_COR_STAGE_ALIGNMENT:


		// Align with steering
		if (LST_Sharp_GetLeftDistance_mm() > LST_Sharp_GetRightDistance_mm())
		{

			LST_Steering_Lock(LST_OBS_COR_LEFT_LOCK);

		}
		else
		{

			LST_Steering_Lock(LST_OBS_COR_RIGHT_LOCK);

		}

		// Slow speed
		LST_Movement_Move(LST_MOVEMENT_FB_SLOW);

		// No need for alignment
		if (lst_control_line_no > 0)
		{

			lst_obs_corner_stage =
					LST_OBS_COR_STAGE_EXIT;

		}

		break;

	case LST_OBS_COR_STAGE_EXIT:

		// Search mode
		lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;

		break;

	}

}

/**
 * @brief Mode for the convoy
 */
static void LST_Obs_Convoy(){
  // ToDo TEST 2018. 02. 01.

	LST_Movement_Move(60);

}

/**
 * @brief Mode for the barrel
 */
static void LST_Obs_Barrel(){
  // ToDo

  // TODO TEST

	LST_Steering_Lock(0);

	if (!lst_temp)
		if (!LST_Distance_Measure_mm(-300))
		{

			LST_Movement_Move_Encoderless(LST_MOVEMENT_BACKING_SLOW);

		}
		else
		{

			//LST_Movement_Stop();
			lst_temp = 1;

		}
	else
	{

		LST_Movement_Stop();

	}

}

/**
 * @brief Mode for the roundabout
 */
static void LST_Obs_Roundabout(){
	// ToDo TEST 2018. 02. 02.

		LST_Movement_Move_Encoderless(-400);

		// lst_control_motor(-100);
}

/**
 * @brief Mode for the train stop
 */
static void LST_Obs_Trainstop(){

	// TODO TEST 2018. 02. 02

	switch (lst_obs_train_stage)
	{

	case LST_OBS_TRA_STAGE_APPROACH:

		break;

	case LST_OBS_TRA_STAGE_WAIT_FIRST:

		break;

	case LST_OBS_TRA_STAGE_CROSS_FIRST:

		break;

	case LST_OBS_TRA_STAGE_WAIT_SECOND:

		break;

	case LST_OBS_TRA_STAGE_CROSS_SECOND:

		break;

	}

}

/**
 * @brief Resets the state machine
 */
static void LST_Obs_ResetStateMachine(){
  lst_obs_lap_mode             = LST_OBS_LAP_MODE_START;
  lst_obs_corner_stage = LST_OBS_COR_STAGE_APPROACH;
  // ToDo reset other variables
}

/**
 * @brief Switches between different modes of control based on the GamePad
 */
static void LST_Obs_GamePadHandler(){
  /* Switch between BT/automatic mode */
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_A] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_mode = LST_OBS_MODE_BT;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_B] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_mode = LST_OBS_MODE_LAP;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_Y] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_mode = LST_OBS_MODE_NO_CONTROL;
  }

  /* Change control parameters with DPad and triggers */
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_NORTH){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_START;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_SOUTH){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_WEST){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_DRONE;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_EAST){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_CORNER;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_CONVOY;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L2] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_BARREL;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_R1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_ROUNDABOUT;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_R2] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_obs_lap_mode = LST_OBS_LAP_MODE_TRAINSTOP;
  }
}
