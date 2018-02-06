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

#ifdef LST_FAST_MODE_STEERING_INTERPOLATED
uint8_t lst_fast_steering_interpol = 1;
#else
uint8_t lst_fast_steering_interpol = 0;
#endif

uint8_t cntr_brake                   = 0;
uint16_t cntr_temp                   = 0;
int16_t iiii = 250;

uint8_t lst_fast_line_pattern_insensitivity = 0;

float lst_fast_q1_accel_plus_p       = 0.0f;
float lst_fast_q1_accel_plus_d       = 0.0f;
float lst_fast_q1_accel_plus_motor   = 0.0f;

float lst_fast_motor_float           = 0.0f;

uint8_t cntr_q1                      = 0;
uint8_t cntr_q1_accel                = 0;
uint8_t cntr_q1_start                = 0;
uint8_t cntr_q1_brake                = 0;
uint8_t cntr_q1_fast_triple_line     = 0;
uint8_t flag_q1_slow_triple_search   = 1;
uint8_t cntr_q1_slow_triple          = 0;
uint8_t cntr_q1_slow_single          = 0;
uint8_t cntr_q1_slow_dotted_lines    = 0;
uint8_t flag_q1_follow_triple_search = 1;
uint8_t cntr_q1_follow_triple        = 0;
uint8_t cntr_q1_follow_single        = 0;
uint8_t cntr_q1_follow_dotted_lines  = 0;
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
  cntr_q1                      = 0;
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
  flag_q1_follow_triple_search = 1;
  cntr_q1_follow_triple        = 0;
  cntr_q1_follow_single        = 0;
  cntr_q1_follow_dotted_lines  = 0;
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

    /* ToDo temp comment, accidentally doing this measurement is bad
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
    */

    lst_control_steering = LST_Control_Servo_BT();
    break;

  case LST_FAST_MODE_LINE_FOLLOW:
    LST_Fast_Reset_State_Machine();

    /* Set acceleration from GamePad */
    lst_control_motor = LST_Control_Motor_BT();
    /* Get line position from the data */
    LST_Steering_Follow(lst_fast_steering_interpol);
    LST_Steering_Set();
    break;

  case LST_FAST_MODE_Q1:
    /* Q1 logic */
    LST_Fast_Q1_Logic();

    // Periodic controls
    LST_Movement_Set();
    LST_Steering_Set();
    break;

  case LST_FAST_MODE_STOP:
    /* Set steering to 0 */
    lst_control_steering = 0;

    /* Satuf�k */
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

  // ToDo �rz�ketlens�gi id�s�v a triplavonal keres�shez, pl 200 ticks

/**
 * @brief Handles Q1 logic
 */
static void LST_Fast_Q1_Logic(){
  // Line follow all the way
  LST_Steering_Follow(lst_fast_steering_interpol);

  // ToDo TEMP!!!!
  /* If lost line, switch to SATUF�K mode */
  if (lst_control_line_lost_flag){
    lst_fast_mode = LST_FAST_MODE_STOP;
    return;
  }

  switch(lst_fast_q1_mode){
  	// Fast lap start (from a stopped position)
		case LST_FAST_MODE_Q1_START:

		  if(!lst_fast_steering_interpol){
        /* Set controller values */
        lst_control_steeringP = LST_FAST_Q1_START_STEERING_P;
        lst_control_steeringD = LST_FAST_Q1_START_STEERING_D;
		  }

// Linear increase of motor control signal
#ifdef LST_FAST_MODE_ENCODERLESS

			/* Accelerate to the desired starting speed */
			if(cntr_q1_start < LST_FAST_Q1_START_TIME){
			  LST_Movement_Move_Encoderless(LST_FAST_Q1_START_MOTOR_SPEED);
				cntr_q1_start++;
			}
			else{
			  // Approach safety car
				lst_fast_q1_mode = LST_FAST_MODE_Q1_APPROACH;

				/* Set motor value */
				LST_Movement_Move_Encoderless(LST_FAST_Q1_SLOW_MOTOR_SPEED);

				cntr_q1_start = 0;
			}

// Speed control handles motor soft start
#else

			lst_fast_q1_mode = LST_FAST_MODE_Q1_APPROACH;
			cntr_q1_start = 0;

#endif
			break;

    // Approach safety car, stop
		case LST_FAST_MODE_Q1_APPROACH:
		  /* Set motor value */
#ifdef LST_FAST_MODE_ENCODERLESS
		  LST_Movement_Move_Encoderless(LST_FAST_Q1_SLOW_MOTOR_SPEED);
#else
		  LST_Movement_Move(LST_FAST_Q1_SLOW_MOTOR_SPEED);
#endif

      /* Check if safety car is within like 40cm */
      if(LST_Sharp_GetRawFrontDistance() < LST_FAST_Q1_APPROACH_DIST){ // 400mm
        cntr_q1++;

        if(cntr_q1 > LST_FAST_Q1_APPROACH_MIN){
          cntr_q1 = 0;
          lst_fast_q1_mode = LST_FAST_MODE_Q1_WAIT_FOR_FOLLOW;
        }
      }
      else{
        cntr_q1 = 0;
      }

		  break;

		// Wait for the safety car to start
		case LST_FAST_MODE_Q1_WAIT_FOR_FOLLOW:
		  LST_Movement_Stop();

		  /* Check if safety car is within like 60cm */
      if(LST_Sharp_GetRawFrontDistance() > LST_FAST_Q1_WAIT_FOLLOW_DIST){ // 600mm
        cntr_q1++;

        if(cntr_q1 > LST_FAST_Q1_WAIT_MIN){
          cntr_q1 = 0;
          lst_fast_line_pattern_insensitivity = 1;
          lst_fast_q1_mode = LST_FAST_MODE_Q1_FOLLOW;
        }
      }
      else{
        cntr_q1 = 0;
      }

		  break;

    // Follow safety car, also check if we are good to go for the laps.
    // ToDo TEST!!
		case LST_FAST_MODE_Q1_FOLLOW:
		  LST_Movement_Move_Sharp(LST_FAST_Q1_FOLLOW_DIST);

      // Check if we need to switch to Race mode
		  // Search for speedup lines, and also no car in front
		  if(lst_fast_line_pattern_insensitivity){
		    if(LST_Distance_Measure_mm(LST_FAST_INSNESITIVITY_DIST)){
		      lst_fast_line_pattern_insensitivity = 0;
		    }
		  }else{
		    if(LST_Sharp_GetRawFrontDistance() < LST_FAST_Q1_FOLLOW_END_DIST){
		      if(flag_q1_follow_triple_search == 1){
            if (lst_control_line_no < 2) cntr_q1_follow_triple = 0;
            if (lst_control_line_no >= 2) cntr_q1_follow_triple++;
            if(cntr_q1_follow_triple > LST_FAST_Q1_FOLLOW_FILTER_THRESHOLD){
              flag_q1_follow_triple_search = 0;
              cntr_q1_follow_triple = 0;
            }
		      }

          // Check for single lines
          else{
            if (lst_control_line_no > 1) cntr_q1_follow_single = 0;
            if (lst_control_line_no == 1) cntr_q1_follow_single++;
            if (cntr_q1_follow_single > LST_FAST_Q1_FOLLOW_FILTER_THRESHOLD){
              flag_q1_follow_triple_search = 1;
              cntr_q1_follow_single = 0;
              cntr_q1_follow_dotted_lines++;
            }
          }

          /* Check for total number of dotted lines */ // Dotted lines found, jump to ACCEL mode
          if(cntr_q1_follow_dotted_lines > LST_FAST_Q1_FOLLOW_LINES_THRESHOLD){
            lst_fast_q1_mode = LST_FAST_MODE_Q1_ACCEL;
            cntr_q1_follow_dotted_lines = 0;
          }
		    }
		  }
		  break;

		case LST_FAST_MODE_Q1_SLOW:
		  if(!lst_fast_steering_interpol){
        /* Set controller values */
        lst_control_steeringP = LST_FAST_Q1_SLOW_STEERING_P;
        lst_control_steeringD = LST_FAST_Q1_SLOW_STEERING_D;
		  }

			/* Set motor value */
#ifdef LST_FAST_MODE_ENCODERLESS
      LST_Movement_Move_Encoderless(LST_FAST_Q1_SLOW_MOTOR_SPEED);
#else
      LST_Movement_Move(LST_FAST_Q1_SLOW_MOTOR_SPEED);
#endif

			/* Check for dotted lines */
			// Check for triple lines
      if(lst_fast_line_pattern_insensitivity){
        if(LST_Distance_Measure_mm(LST_FAST_INSNESITIVITY_DIST)){
          lst_fast_line_pattern_insensitivity = 0;
        }
      }else{
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

        /* Check for total number of dotted lines */ // Dotted lines found, jump to ACCEL mode
        if(cntr_q1_slow_dotted_lines > LST_FAST_Q1_SLOW_LINES_THRESHOLD){
          lst_fast_q1_mode = LST_FAST_MODE_Q1_ACCEL;
          cntr_q1_slow_dotted_lines = 0;
        }
      }
			break;

		case LST_FAST_MODE_Q1_ACCEL:

#ifdef LST_FAST_MODE_ENCODERLESS

			/* Linearly increase everything from slow to fast */
			if(cntr_q1_accel < LST_FAST_Q1_ACCEL_TIME){
				lst_control_steeringP  += lst_fast_q1_accel_plus_p;
				lst_control_steeringD  += lst_fast_q1_accel_plus_d;
				lst_fast_motor_float   += lst_fast_q1_accel_plus_motor;
				LST_Movement_Move_Encoderless(lst_fast_motor_float);

				cntr_q1_accel++;
			}
			else{
				lst_fast_motor_float = LST_FAST_Q1_SLOW_MOTOR_SPEED;
				lst_fast_q1_mode = LST_FAST_MODE_Q1_FAST;
				cntr_q1_accel = 0;
			}

#else // Jump to high speed immediately, motor control handles acceleration slope

			lst_fast_q1_mode = LST_FAST_MODE_Q1_FAST;

#endif

			lst_fast_line_pattern_insensitivity = 1;
			break;

		case LST_FAST_MODE_Q1_FAST:
		  if(!lst_fast_steering_interpol){
        /* Set controller values */
        lst_control_steeringP = LST_FAST_Q1_FAST_STEERING_P;
        lst_control_steeringD = LST_FAST_Q1_FAST_STEERING_D;
		  }

			/* Set motor value */
#ifdef LST_FAST_MODE_ENCODERLESS
      LST_Movement_Move_Encoderless(LST_FAST_Q1_FAST_MOTOR_SPEED);
#else
      LST_Movement_Move(LST_FAST_Q1_FAST_MOTOR_SPEED);
#endif

      if(lst_fast_line_pattern_insensitivity){
        if(LST_Distance_Measure_mm(LST_FAST_INSNESITIVITY_DIST)){
          lst_fast_line_pattern_insensitivity = 0;
        }
      }else{
        /* Check for triple lines */
        if (lst_control_line_no < 2) cntr_q1_fast_triple_line = 0;
        if (lst_control_line_no >= 2) cntr_q1_fast_triple_line++;
        if(cntr_q1_fast_triple_line>LST_FAST_Q1_FAST_TRIPLE_LINES){
          /* Switch to braking */
          lst_fast_q1_mode = LST_FAST_MODE_Q1_BRAKE;
          cntr_q1_fast_triple_line = 0;
        }
      }
			break;

		case LST_FAST_MODE_Q1_BRAKE:
		  if(!lst_fast_steering_interpol){
        /* Set controller values */
        lst_control_steeringP = LST_FAST_Q1_BRAKE_STEERING_P;
        lst_control_steeringD = LST_FAST_Q1_BRAKE_STEERING_D;
		  }

#ifdef LST_FAST_MODE_ENCODERLESS

			/* Satuf�k */
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

#else // Jump to low speed immediately, motor control handles deceleration slope

			lst_fast_q1_mode = LST_FAST_MODE_Q1_SLOW;

#endif

			lst_fast_line_pattern_insensitivity = 1;
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
    lst_fast_q1_mode = LST_FAST_MODE_Q1_START;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_R1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_fast_q1_mode = LST_FAST_MODE_Q1_SLOW;
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

/**
 * @brief DIP handler at the start
 */
void LST_Fast_DIP_Handler_Start(){
  if(lst_dip_settings[1] == 1){
    lst_fast_mode = LST_FAST_MODE_BT;
  }
  else{
    lst_fast_mode = LST_FAST_MODE_Q1;
  }

  if(lst_dip_settings[2] == 1){
    lst_fast_q1_mode = LST_FAST_MODE_Q1_SLOW;
  }
  else{
    lst_fast_q1_mode = LST_FAST_MODE_Q1_START;
  }
}
