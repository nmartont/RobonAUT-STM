/*
 * lst_fastlap.c
 *
 *  Created on: 2017. dec. 26.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_fastlap.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t lst_fast_mode = LST_FAST_MODE_BT;

uint8_t cntr_brake                   = 0;
uint16_t cntr_temp                    = 0;
int16_t iiii = 250;

float lst_fast_q1_accel_plus_p       = 0.0f;
float lst_fast_q1_accel_plus_d       = 0.0f;
float lst_fast_q1_accel_plus_motor   = 0.0f;
float lst_fast_motor_float           = 0.0f;
uint8_t cntr_q1_accel                = 0;
uint8_t cntr_q1_start                = 0;
uint8_t cntr_q1_brake                = 0;
uint8_t cntr_q1_fast_triple_line     = 0;
uint8_t flag_q1_slow_triple_search   = 1;
uint8_t cntr_q1_slow_triple          = 0;
uint8_t cntr_q1_slow_single          = 0;
uint8_t cntr_q1_slow_dotted_lines    = 0;
uint8_t lst_fast_q1_mode             = LST_FAST_MODE_Q1_START;

/* External variables --------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void LST_Fast_Reset_State_Machine();
static void LST_Fast_Q1_Logic();
static void LST_Fast_Gamepad_Handler();
static void LST_Fast_State_Machine();

/******************************************************************************/
/*                  Fast lap handler for RobonAUT 2018 Team LST               */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Fast lap module
 */
void LST_Fast_Init() {
  /* Calculate values to add to the parameters during acceleration */
  lst_fast_q1_accel_plus_p     = (float)(LST_FAST_Q1_FAST_STEERING_P  - LST_FAST_Q1_SLOW_STEERING_P) /(float)(LST_FAST_Q1_ACCEL_TIME);
  lst_fast_q1_accel_plus_d     = (float)(LST_FAST_Q1_FAST_STEERING_D  - LST_FAST_Q1_SLOW_STEERING_D) /(float)(LST_FAST_Q1_ACCEL_TIME);
  lst_fast_q1_accel_plus_motor = (float)(LST_FAST_Q1_FAST_MOTOR_SPEED - LST_FAST_Q1_SLOW_MOTOR_SPEED)/(float)(LST_FAST_Q1_ACCEL_TIME);
}

/**
 * @brief Main handler logic of the fast lap
 */
void LST_Fast_Logic(){

  /* Get line data, sensor data, etc */
  LST_Control_Commons();

  /* Switch between control modes based on the GamePad */
  LST_Fast_Gamepad_Handler();

  /* Main state machine for the Fast Lap mode (eg. BT control, lap mode, etc) */
  LST_Fast_State_Machine();

  /* Set the control of the servo and motor */
  LST_Control_ServoAndMotor();
}

/**
 * @brief Resets the state machine
 */
static void LST_Fast_Reset_State_Machine(){
//  cntr_lost_lines              = 0;
  cntr_brake                   = 0;
  lst_fast_motor_float         = LST_FAST_Q1_SLOW_MOTOR_SPEED;
  cntr_q1_fast_triple_line     = 0;
  cntr_q1_start                = 0;
  cntr_q1_brake                = 0;
  cntr_q1_accel                = 0;
  flag_q1_slow_triple_search   = 1;
//  lst_fast_line_no_input       = 0;
  cntr_q1_slow_triple          = 0;
  cntr_q1_slow_single          = 0;
  cntr_q1_slow_dotted_lines    = 0;
  lst_fast_q1_mode             = LST_FAST_MODE_Q1_START;
}

/**
 * @brief Main state machine of the fast lap mode
 */
static void LST_Fast_State_Machine(){
  switch(lst_fast_mode){
  case LST_FAST_MODE_BT:
    LST_Fast_Reset_State_Machine();

    lst_control_steering = LST_Control_Servo_BT();
    lst_control_motor = LST_Control_Motor_BT();
    break;
  case LST_FAST_MODE_SPEED_CONTROL:
    LST_Fast_Reset_State_Machine();

    if(cntr_temp<800){
      cntr_temp++;
      lst_control_motor = iiii;
    }
    else if(cntr_temp<1300){
      cntr_temp++;
      lst_control_motor = 0;
    }
    else{
      iiii = iiii+50;
      cntr_temp = 0;
    }

    lst_control_steering = LST_Control_Servo_BT();
    break;
  case LST_FAST_MODE_LINE_FOLLOW:
    LST_Fast_Reset_State_Machine();

    /* Set acceleration from GamePad */
    lst_control_motor = LST_Control_Motor_BT();
    /* Get line position from the data */
    lst_control_steering = LST_Control_SteeringController(0);
    break;
  case LST_FAST_MODE_Q1:
    /* Q1 logic */
    LST_Fast_Q1_Logic();
    /* Controlled steering*/
    lst_control_steering = LST_Control_SteeringController(0);
    break;
  case LST_FAST_MODE_STOP:
    /* Set steering to 0 */
    lst_control_steering = 0;

    /* Satufék */
    if(cntr_brake<LST_FAST_BRAKE_DELAY){
      lst_control_motor = LST_FAST_Q1_BRAKE_MOTOR;
      cntr_brake++;
    }else if(cntr_brake < 2*LST_FAST_BRAKE_DELAY){
      lst_control_motor = 0;
      cntr_brake++;
    }else if(cntr_brake < 2*LST_FAST_BRAKE_DELAY + LST_FAST_BRAKE_TIME){
      lst_control_motor = LST_FAST_Q1_BRAKE_MOTOR;
      cntr_brake++;
    }else{
      // Wait for reset...
      lst_control_motor = 0;
    }
    break;
  case LST_FAST_MODE_NO_CONTROL:
    /* Leave values on default */
  default:
    /* Leave values at default */
    LST_Fast_Reset_State_Machine();
    break;
  }
}

/**
 * @brief Handles Q1 logic
 */
static void LST_Fast_Q1_Logic(){
  // ToDo Érzéketlenségi idõsáv a triplavonal kereséshez, pl 200 ticks
  switch(lst_fast_q1_mode){
  case LST_FAST_MODE_Q1_START:
    /* Set controller values */
    lst_control_steeringP = LST_FAST_Q1_START_STEERING_P;
    lst_control_steeringD = LST_FAST_Q1_START_STEERING_D;

    /* Accelerate to the desired starting speed */
    if(cntr_q1_start < LST_FAST_Q1_START_TIME){
      lst_control_motor = LST_FAST_Q1_START_MOTOR_SPEED;
      cntr_q1_start++;
    }
    else{
      lst_fast_q1_mode = LST_FAST_MODE_Q1_SLOW;
      cntr_q1_start = 0;
    }
    break;
  case LST_FAST_MODE_Q1_SLOW:
    /* Set controller values */
    lst_control_steeringP = LST_FAST_Q1_SLOW_STEERING_P;
    lst_control_steeringD = LST_FAST_Q1_SLOW_STEERING_D;

    /* Set motor value */
    lst_control_motor = LST_FAST_Q1_SLOW_MOTOR_SPEED;

    /* Check for dotted lines */
    // Check for triple lines
    if(flag_q1_slow_triple_search == 1){
      if (lst_control_line_no < 2) cntr_q1_slow_triple = 0;
      if (lst_control_line_no >= 2) cntr_q1_slow_triple++;
      if(cntr_q1_slow_triple > LST_FAST_Q1_SLOW_FILTER_THRESHOLD){
        flag_q1_slow_triple_search = 0;
        cntr_q1_slow_triple = 0;
      }
    }
    // Check for single lines
    else{
      if (lst_control_line_no > 1) cntr_q1_slow_single = 0;
      if (lst_control_line_no == 1) cntr_q1_slow_single++;
      if (cntr_q1_slow_single > LST_FAST_Q1_SLOW_FILTER_THRESHOLD){
        flag_q1_slow_triple_search = 1;
        cntr_q1_slow_single = 0;
        cntr_q1_slow_dotted_lines++;
      }
    }

    /* Check for total number of dotted lines */
    if(cntr_q1_slow_dotted_lines > LST_FAST_Q1_SLOW_LINES_THRESHOLD){
      lst_fast_q1_mode = LST_FAST_MODE_Q1_ACCEL;
      cntr_q1_slow_dotted_lines = 0;
    }
    break;
  case LST_FAST_MODE_Q1_ACCEL:
    /* Linearly increase everything from slow to fast */
    if(cntr_q1_accel < LST_FAST_Q1_ACCEL_TIME){
      lst_control_steeringP  += lst_fast_q1_accel_plus_p;
      lst_control_steeringD  += lst_fast_q1_accel_plus_d;
      lst_fast_motor_float   += lst_fast_q1_accel_plus_motor;
      lst_control_motor       = lst_fast_motor_float;

      cntr_q1_accel++;
    }
    else{
      lst_fast_motor_float = LST_FAST_Q1_SLOW_MOTOR_SPEED;
      lst_fast_q1_mode = LST_FAST_MODE_Q1_FAST;
      cntr_q1_accel = 0;
    }
    break;
  case LST_FAST_MODE_Q1_FAST:
    /* Set controller values */
    lst_control_steeringP = LST_FAST_Q1_FAST_STEERING_P;
    lst_control_steeringD = LST_FAST_Q1_FAST_STEERING_D;

    /* Set motor value */
    lst_control_motor = LST_FAST_Q1_FAST_MOTOR_SPEED;

    /* Check for triple lines */
    if (lst_control_line_no < 2) cntr_q1_fast_triple_line = 0;
    if (lst_control_line_no >= 2) cntr_q1_fast_triple_line++;
    if(cntr_q1_fast_triple_line>LST_FAST_Q1_FAST_TRIPLE_LINES){
      /* Switch to braking */
      lst_fast_q1_mode = LST_FAST_MODE_Q1_BRAKE;
      cntr_q1_fast_triple_line = 0;
    }
    break;
  case LST_FAST_MODE_Q1_BRAKE:
    /* Set controller values */
    lst_control_steeringP = LST_FAST_Q1_BRAKE_STEERING_P;
    lst_control_steeringD = LST_FAST_Q1_BRAKE_STEERING_D;

    /* Satufék */
    if(cntr_q1_brake<LST_FAST_BRAKE_DELAY){
      lst_control_motor = LST_FAST_Q1_BRAKE_MOTOR;
      cntr_q1_brake++;
    }else if(cntr_q1_brake < 2*LST_FAST_BRAKE_DELAY){
      lst_control_motor = 0;
      cntr_q1_brake++;
    }else if(cntr_q1_brake < 2*LST_FAST_BRAKE_DELAY + 5*LST_FAST_BRAKE_DELAY){
      lst_control_motor = LST_FAST_Q1_BRAKE_MOTOR;
      cntr_q1_brake++;
    }else if(cntr_q1_brake < 2*LST_FAST_BRAKE_DELAY + 6*LST_FAST_BRAKE_DELAY){
      lst_control_motor = 0;
      cntr_q1_brake++;
    }else if(cntr_q1_brake < 2*LST_FAST_BRAKE_DELAY + 9*LST_FAST_BRAKE_DELAY){
      lst_control_motor = LST_FAST_Q1_BRAKE_MOTOR;
      cntr_q1_brake++;
    }else if(cntr_q1_brake < 2*LST_FAST_BRAKE_DELAY + 10*LST_FAST_BRAKE_DELAY){
      lst_control_motor = 0;
      cntr_q1_brake++;
    }else if(cntr_q1_brake < 2*LST_FAST_BRAKE_DELAY + 11*LST_FAST_BRAKE_DELAY){
      lst_control_motor = LST_FAST_Q1_BRAKE_MOTOR;
      cntr_q1_brake++;
//    }
//    else if(cntr_q1_brake < 2*LST_FAST_BRAKE_DELAY + 14*LST_FAST_BRAKE_DELAY){
//      lst_fast_motor = 0;
//      cntr_q1_brake++;
//    }else if(cntr_q1_brake < 2*LST_FAST_BRAKE_DELAY + 16*LST_FAST_BRAKE_DELAY){
//        lst_fast_motor = LST_FAST_Q1_BRAKE_MOTOR;
//        cntr_q1_brake++;
    }else{
      lst_fast_q1_mode = LST_FAST_MODE_Q1_SLOW;
      cntr_q1_brake = 0;
    }
    break;
  }
}

/**
 * @brief Switches between different modes of control based on the GamePad
 */
static void LST_Fast_Gamepad_Handler(){
  /* Switch between BT/automatic mode */
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_A] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_fast_mode = LST_FAST_MODE_BT;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_B] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_fast_mode = LST_FAST_MODE_LINE_FOLLOW;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_fast_mode = LST_FAST_MODE_SPEED_CONTROL;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_R1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_fast_mode = LST_FAST_MODE_Q1_FAST;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L2] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_fast_mode = LST_FAST_MODE_Q1;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_Y] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_fast_mode = LST_FAST_MODE_NO_CONTROL;
  }

  /* Change control parameters with DPad */
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_NORTH){
    lst_control_steeringD += 20;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_SOUTH){
    lst_control_steeringD -= 20;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_WEST){
    lst_control_steeringP -= 20;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_EAST){
    lst_control_steeringP += 20;
  }
}
