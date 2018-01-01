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
void LST_Obs_StateMachine(){
  switch(lst_obs_mode){
  case LST_OBS_MODE_BT:
    LST_Obs_ResetStateMachine();

    lst_control_steering = LST_Control_Servo_BT();
    lst_control_motor = LST_Control_Motor_BT();
    break;
  case LST_OBS_MODE_LAP:
    /* Q1 logic */
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
void LST_Obs_Lap(){
  switch(lst_obs_lap_mode){
  case LST_OBS_LAP_MODE_START:
    /* Return if the radio hasn't received the start message */
    if(lst_radio_msg_0_received != LST_RADIO_MSG_RECEIVED) return;

    /* Switch to SEARCH mode */
    lst_obs_lap_mode = LST_OBS_LAP_MODE_SEARCH;
    break;
  case LST_OBS_LAP_MODE_SEARCH:
    break;
  case LST_OBS_LAP_MODE_DRONE:
    break;
  case LST_OBS_LAP_MODE_CORNER:
    break;
  case LST_OBS_LAP_MODE_CONVOY:
    break;
  case LST_OBS_LAP_MODE_BARREL:
    break;
  case LST_OBS_LAP_MODE_ROUNDABOUT:
    break;
  case LST_OBS_LAP_MODE_TRAINSTOP:
    break;
  default:
    break;
  }
}

/**
 * @brief Resets the state machine
 */
void LST_Obs_ResetStateMachine(){
  lst_obs_lap_mode             = LST_OBS_LAP_MODE_START;
  // ToDo reset other variables
}

/**
 * @brief Switches between different modes of control based on the GamePad
 */
void LST_Obs_GamePadHandler(){
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
