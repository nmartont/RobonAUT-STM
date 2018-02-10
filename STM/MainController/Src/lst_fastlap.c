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

/* Section counters */
uint8_t lst_fast_done_laps             = 0;
int8_t  lst_fast_started_fast_sections = -1; // -1 because the race starts with a fast section
uint8_t lst_fast_started_slow_sections = 0;

int16_t lst_fast_brake_delay = LST_BRAKE_Q1_DELAY_LAP1;
int16_t lst_fast_brake_speed = LST_BRAKE_Q1_SPEED_LAP1;

/* Motor control variables */
int16_t lst_fast_slow_speed            = LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP1;
int16_t lst_fast_fast_speed            = LST_FAST_Q1_FAST_MOTOR_SPEED_LAP1;

#ifdef LST_FAST_MODE_STEERING_INTERPOLATED
uint8_t lst_fast_steering_interpol = 1;
#else
uint8_t lst_fast_steering_interpol = 0;
#endif

uint8_t cntr_brake                   = 0;
uint16_t cntr_temp                   = 0;
uint16_t cntr_temp_2                 = 0;
int16_t iiii = 250;

uint8_t lst_fast_follow_do_brake     = 0;
uint8_t lst_fast_follow_cntr         = 0;
uint8_t lst_fast_follow_cntr2        = 0;

uint8_t lst_fast_line_pattern_insensitivity = 0;

float lst_fast_q1_accel_plus_p       = 0.0f;
float lst_fast_q1_accel_plus_d       = 0.0f;
float lst_fast_q1_accel_plus_motor   = 0.0f;

float lst_fast_motor_float           = 0.0f;

uint8_t cntr_q1                      = 0;
uint8_t cntr_q1_accel                = 0;
uint8_t cntr_q1_start                = 0;
int16_t cntr_q1_brake                = 0;
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
static void LST_Fast_Q1_Lap_Control();

/******************************************************************************/
/*                  Fast lap handler for RobonAUT 2018 Team LST               */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Fast lap module
 */
void LST_Fast_Init() {
  lst_movement_sharp_speed_max = LST_FAST_Q1_SLOW_FOLLOW_SPEED_MAX;

  /* Calculate values to add to the parameters during acceleration */
  lst_fast_q1_accel_plus_p     = (float)(LST_FAST_Q1_FAST_STEERING_P  - LST_FAST_Q1_SLOW_STEERING_P) /(float)(LST_FAST_Q1_ACCEL_TIME);
  lst_fast_q1_accel_plus_d     = (float)(LST_FAST_Q1_FAST_STEERING_D  - LST_FAST_Q1_SLOW_STEERING_D) /(float)(LST_FAST_Q1_ACCEL_TIME);
  lst_fast_q1_accel_plus_motor = (float)(LST_FAST_Q1_FAST_MOTOR_SPEED_LAP1 - LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP1)/(float)(LST_FAST_Q1_ACCEL_TIME);
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
  lst_fast_motor_float         = LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP1;
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
  lst_fast_done_laps             = 0;
  lst_fast_started_fast_sections = -1; // -1 because the race starts with a fast section
  lst_fast_started_slow_sections = 0;
  lst_fast_slow_speed            = LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP1;
  lst_fast_fast_speed            = LST_FAST_Q1_FAST_MOTOR_SPEED_LAP1;
  cntr_temp                      = 0;
  cntr_temp_2                    = 0;
  lst_fast_follow_do_brake       = 0;
  lst_fast_follow_cntr           = 0;
  lst_fast_follow_cntr2          = 0;
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
    /* Map control for speed setting and such */
    LST_Fast_Q1_Lap_Control();

    /* Q1 logic */
    LST_Fast_Q1_Logic();

    // Periodic controls
    LST_Movement_Set_Fast();
    LST_Steering_Set();
    break;

  case LST_FAST_MODE_STOP:
    /* Set steering to 0 */
    lst_control_steering = 0;

    /* Satufék */
    if(cntr_brake<LST_FAST_BRAKE_DELAY){
      LST_Movement_Move_Encoderless(LST_FAST_Q1_BRAKE_MOTOR);
      cntr_brake++;

      LST_Movement_Set_Fast();
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

  // ToDo Érzéketlenségi idõsáv a triplavonal kereséshez, pl 200 ticks

/**
 * @brief Handles Q1 logic
 */
static void LST_Fast_Q1_Logic(){
  // Line follow all the way
  LST_Steering_Follow(lst_fast_steering_interpol);

  // ToDo TEMP!!!!
  /* If lost line, switch to SATUFÉK mode */
//  if (lst_control_line_lost_flag){
//    lst_fast_mode = LST_FAST_MODE_STOP;
//    return;
//  }

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
		  // ToDo temp comment
//			/* Accelerate to the desired starting speed */
//			if(cntr_q1_start < LST_FAST_Q1_START_TIME){
//			  LST_Movement_Move_Encoderless(LST_FAST_Q1_START_MOTOR_SPEED);
//				cntr_q1_start++;
//			}
//			else{
			  // Approach safety car
				lst_fast_q1_mode = LST_FAST_MODE_Q1_APPROACH;
//
//				/* Set motor value */
//				LST_Movement_Move_Encoderless(LST_FAST_Q1_APPROACH_MOTOR_SPEED);
//
//				cntr_q1_start = 0;
//			}

// Speed control handles motor soft start
#else

			lst_fast_q1_mode = LST_FAST_MODE_Q1_APPROACH;
			cntr_q1_start = 0;

#endif
			break;

    // Approach safety car, stop
		case LST_FAST_MODE_Q1_APPROACH:
		  if(!lst_fast_steering_interpol){
        /* Set controller values */
        lst_control_steeringP = LST_FAST_Q1_APPROACH_STEERING_P;
        lst_control_steeringD = LST_FAST_Q1_APPROACH_STEERING_D;
      }

		  /* Set motor value */
#ifdef LST_FAST_MODE_ENCODERLESS
		  // ToDo temp!!
		  LST_Movement_Move(50);
#else
		  LST_Movement_Move(LST_FAST_Q1_APPROACH_MOTOR_SPEED);
#endif

      /* Check if safety car is within like 40cm */
      if(LST_Sharp_GetFrontDistance() > LST_FAST_Q1_APPROACH_DIST){ // 400mm
        cntr_q1++;

        if(cntr_q1 > LST_FAST_Q1_APPROACH_MIN){
          cntr_q1 = 0;
          lst_fast_q1_mode = LST_FAST_MODE_Q1_WAIT_FOR_FOLLOW;
          // Start in fast mode TODO TEST 2018. 02. 09.
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
      if(LST_Sharp_GetFrontDistance() < LST_FAST_Q1_WAIT_FOLLOW_DIST){ // 600mm
        cntr_q1++;

        if(cntr_q1 > LST_FAST_Q1_WAIT_MIN){
          cntr_q1 = 0;
          lst_fast_line_pattern_insensitivity = 1;
          lst_fast_q1_mode = LST_FAST_MODE_Q1_FOLLOW;

          // Fast follow start
          // TODO 2018.02.08. Init for follow
          lst_movement_sharp_speed_max = LST_FAST_Q1_FAST_FOLLOW_SPEED_MAX;
          if(!lst_fast_steering_interpol){
            lst_control_steeringP = LST_FAST_Q1_FOLLOW_FAST_STEERING_P;
            lst_control_steeringD = LST_FAST_Q1_FOLLOW_FAST_STEERING_D;
          }

        }
      }
      else{
        cntr_q1 = 0;
      }

		  break;

    // Follow safety car, also check if we are good to go for the laps.
    // ToDo TEST!!
		case LST_FAST_MODE_Q1_FOLLOW:

		  /* Sense Start gate */
		  if ((LST_Sharp_GetLeftDistance() > 150)
		  		&& (LST_Sharp_GetRightDistance() > 150))
		  {
		  	lst_fast_startDetected++;

		  	if(lst_fast_startDetected > 5){
		  	  lst_fast_startDetected = 0;
		  	  lst_fast_q1_mode = LST_FAST_MODE_Q1_ACCEL;
          cntr_q1_follow_dotted_lines = 0;
          return;
		  	}
		  }else{
		    lst_fast_startDetected = 0;
		  }

		  /* Emergency braking */
		  if(!lst_fast_follow_do_brake &&
		      LST_Sharp_GetFrontDistance() > LST_FAST_Q1_FOLLOW_TOO_CLOSE_DIST)
		  {
		    lst_fast_follow_do_brake = 1;
		    lst_fast_follow_cntr     = 0;
		    lst_fast_follow_cntr2    = 0;
		  }

		  if(lst_fast_follow_do_brake){
		    lst_fast_follow_cntr++;

		    /* Check if speed is low enough */
		    if(lst_encoder_speed < LST_FAST_Q1_FOLLOW_BRAKE_MIN_SPEED){ // 20 encoder speed
		      lst_fast_follow_cntr2++;
		    }else{
		      lst_fast_follow_cntr2 = 0;
		    }

		    if(lst_fast_follow_cntr < LST_FAST_Q1_FOLLOW_BRAKE_TIME &&
		       lst_fast_follow_cntr2 < 2){ // TODO THRESHOLD OK?
		      LST_Movement_Move_Encoderless(LST_BRAKE_Q1_SPEED);
		    }
		    else{
		      lst_fast_follow_do_brake = 0;
		    }
		  }
		  else{
		    LST_Movement_Move_Sharp(LST_FAST_Q1_FOLLOW_DIST);
		  }

		  /* Pattern detection */
		  if(lst_fast_line_pattern_insensitivity){
		    if(LST_Distance_Measure_mm(LST_FAST_INSNESITIVITY_DIST)){
		      lst_fast_line_pattern_insensitivity = 0;
		    }
		  }else{
        /* Sense slow and fast patterns, limit speed accordingly */
        // sense 1+ lines
        if(lst_control_line_no > 1){
          cntr_temp ++;

          if(cntr_temp_2 > 0){
            cntr_temp = 0;
            cntr_temp_2 = 0;

            /*FAST pattern*/
            lst_fast_line_pattern_insensitivity = 1;
            lst_movement_sharp_speed_max =
                LST_FAST_Q1_FAST_FOLLOW_SPEED_MAX;  // ToDo encoder speed

            // TODO 2018.02.08. Steering controls set
            if(!lst_fast_steering_interpol){
              lst_control_steeringP = LST_FAST_Q1_FOLLOW_FAST_STEERING_P;
              lst_control_steeringD = LST_FAST_Q1_FOLLOW_FAST_STEERING_D;
            }
            return;
          }

          cntr_temp_2 = 0;

          if(cntr_temp > 30){
            // if long line, decrease max speed
            cntr_temp = 0;
            cntr_temp_2 = 0;

            lst_fast_line_pattern_insensitivity = 1;
            lst_movement_sharp_speed_max =
                LST_FAST_Q1_SLOW_FOLLOW_SPEED_MAX;  // ToDo encoder speed

            // TODO 2018.02.08. Steering controls set
            if(!lst_fast_steering_interpol){
              lst_control_steeringP = LST_FAST_Q1_FOLLOW_SLOW_STEERING_P;
              lst_control_steeringD = LST_FAST_Q1_FOLLOW_SLOW_STEERING_D;
            }
            return;
          }
        }else if(lst_control_line_no == 1 && cntr_temp > 2){
          // short lines
          cntr_temp_2 ++; // number of found 1 lines

          if(cntr_temp_2 > 15){
            // faulty, we wanna see some 3 lines again
            cntr_temp_2 = 0;
          }
        }else{
          // counter reset
          cntr_temp = 0;
          cntr_temp_2 = 0;
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
      LST_Movement_Move_Encoderless(lst_fast_slow_speed);
#else
      LST_Movement_Move(lst_fast_slow_speed);
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
			/*if(cntr_q1_accel < LST_FAST_Q1_ACCEL_TIME){
			  if(!lst_fast_steering_interpol){
          lst_control_steeringP  += lst_fast_q1_accel_plus_p;
          lst_control_steeringD  += lst_fast_q1_accel_plus_d;
			  }
				lst_fast_motor_float   += lst_fast_q1_accel_plus_motor;
				LST_Movement_Move_Encoderless((int16_t)lst_fast_motor_float);

				cntr_q1_accel++;
			}
			else{*/
				lst_fast_motor_float = lst_fast_slow_speed;
				lst_fast_q1_mode = LST_FAST_MODE_Q1_FAST;
				lst_fast_started_fast_sections++;
				cntr_q1_accel = 0;
			/*
			}
			*/

#else // Jump to high speed immediately, motor control handles acceleration slope

			lst_fast_q1_mode = LST_FAST_MODE_Q1_FAST;
			lst_fast_started_fast_sections++;

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
      LST_Movement_Move_Encoderless(lst_fast_fast_speed);
#else
      LST_Movement_Move(lst_fast_fast_speed);
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
          cntr_q1_brake = lst_fast_brake_delay;
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
			/* Satufék */
		  /* Brake v2 */
		  if (cntr_q1_brake >= 0)
		  {

		  	LST_Movement_Move_Encoderless(lst_fast_brake_speed);
		  	cntr_q1_brake--;

		  }
		  else
		  {

		  	lst_fast_q1_mode = LST_FAST_MODE_Q1_SLOW;

		  }

#else // Jump to low speed immediately, motor control handles deceleration slope
			lst_fast_q1_mode = LST_FAST_MODE_Q1_SLOW;
			lst_fast_started_slow_sections++;
#endif

			lst_fast_line_pattern_insensitivity = 1;
			break;

		case LST_FAST_MODE_Q1_END:
		  if(!lst_fast_steering_interpol){
        /* Set controller values */
        lst_control_steeringP = LST_FAST_Q1_END_STEERING_P;
        lst_control_steeringD = LST_FAST_Q1_END_STEERING_D;
      }

		  /* Measure about 4 meters, if it's done, switch to Q1_START and SATUFÉK mode */
		  if(LST_Distance_Measure_mm(LST_FAST_END_DIST)){ // 3 meters
		    lst_fast_q1_mode = LST_FAST_MODE_Q1_START;
		    lst_fast_mode = LST_FAST_MODE_STOP;
      }
		  else{
		    /* Switch speed to super fast */
#ifdef LST_FAST_MODE_ENCODERLESS
		    LST_Movement_Move_Encoderless(LST_FAST_Q1_FINISH_MOTOR_SPEED);
#else
		    LST_Movement_Move(LST_FAST_Q1_FINISH_MOTOR_SPEED);
#endif
		  }

		  break;

  }
}

static void LST_Fast_Q1_Lap_Control(){
  /* Check fast lap count */
  if(lst_fast_started_fast_sections == 4){ // full lap
    lst_fast_started_fast_sections = 0;
    lst_fast_done_laps++;
  }

#ifdef LST_FAST_THREE_LAPS
  switch(lst_fast_done_laps){
  case 0: // first lap is next
    lst_fast_brake_speed= LST_BRAKE_Q1_SPEED_LAP1;
    lst_fast_brake_delay = LST_BRAKE_Q1_DELAY_LAP1;
    lst_fast_slow_speed = LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP1;
    lst_fast_fast_speed = LST_FAST_Q1_FAST_MOTOR_SPEED_LAP1;
    break;
  case 1: // second lap is next
    lst_fast_brake_speed= LST_BRAKE_Q1_SPEED_LAP2;
    lst_fast_brake_delay = LST_BRAKE_Q1_DELAY_LAP2;
    lst_fast_slow_speed = LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP2;
    lst_fast_fast_speed = LST_FAST_Q1_FAST_MOTOR_SPEED_LAP2;
    break;
  case 2: // third lap is next
    lst_fast_brake_speed= LST_BRAKE_Q1_SPEED_LAP3;
    lst_fast_brake_delay = LST_BRAKE_Q1_DELAY_LAP3;
    lst_fast_slow_speed = LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP3;
    lst_fast_fast_speed = LST_FAST_Q1_FAST_MOTOR_SPEED_LAP3;
    break;
  case 3: // last lap finished, time for finish-line
  default:
    /* Get to End mode */
    //lst_fast_q1_mode = LST_FAST_MODE_Q1_END;
  	lst_fast_q1_mode = LST_FAST_MODE_Q1_START;
		lst_fast_mode = LST_FAST_MODE_STOP;
    break;
  }
#else
  switch(lst_fast_done_laps){
  case 0: // first lap is next
    lst_fast_slow_speed = LST_FAST_Q1_SLOW_MOTOR_SPEED_LAP1;
    lst_fast_fast_speed = LST_FAST_Q1_FAST_MOTOR_SPEED_LAP1;
    break;
  case 1: // Finish
  default:
    /* Get to End mode */
  	//lst_fast_q1_mode = LST_FAST_MODE_Q1_END;
  	lst_fast_q1_mode = LST_FAST_MODE_Q1_START;
		lst_fast_mode = LST_FAST_MODE_STOP;
    break;
  }
#endif
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

  /* ToDo test */
  if(HAL_GPIO_ReadPin(ALT_START_GPIO_Port, ALT_START_Pin) == GPIO_PIN_SET){
    lst_fast_q1_mode = LST_FAST_MODE_Q1_SLOW;
  }
  else{
    lst_fast_q1_mode = LST_FAST_MODE_Q1_START;
  }
}
