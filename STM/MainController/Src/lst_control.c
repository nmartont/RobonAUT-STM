/*
 * lst_control.c
 *
 *  Created on: 2017. nov. 28.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_control.h"

/* Private variables ---------------------------------------------------------*/
uint8_t lst_control_mode = LST_CONTROL_MODE_BT;

float lst_control_q1_accel_plus_p    = 0.0f;
float lst_control_q1_accel_plus_d    = 0.0f;
float lst_control_q1_accel_plus_motor= 0.0f;
float lst_control_motor_float 		   = 0.0f;
uint8_t cntr_q1_accel								 = 0;
uint8_t cntr_q1_start         			 = 0;
uint8_t cntr_q1_brake								 = 0;
uint8_t cntr_q1_fast_triple_line     = 0;
uint8_t flag_q1_slow_triple_search   = 1;
uint8_t cntr_q1_slow_triple          = 0;
uint8_t cntr_q1_slow_single          = 0;
uint8_t cntr_q1_slow_dotted_lines 	 = 0;
uint8_t lst_control_line_no          = 0;
uint8_t lst_control_line_no_input    = 0;
uint8_t lst_control_q1_mode          = LST_CONTROL_MODE_Q1_START;
uint8_t lst_control_line_no_array[8] = { 0x00 };

/* Line positions for PID controller */
float lst_control_linePos =     0.0f;
float lst_control_linePosOld =  0.0f;
// float lst_control_linePosSum =  0.0f;

/* Controller parameters */
uint16_t lst_control_steeringP = LST_CONTROL_STEERING_P;
uint16_t lst_control_steeringD = LST_CONTROL_STEERING_D;

/* Other controller variables */
int16_t lst_control_errorSignal = 0;
int16_t lst_control_errorSignalOld = 0;

int16_t lst_control_steering = 0;
int16_t lst_control_motor = 0;
int16_t lst_control_steering_offset = 0; // -150

uint16_t mode_cntr = 0;

/******************************************************************************/
/*                Controller handling for RobonAUT 2018 Team LST              */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Control part of the software
 */
void LST_Control_Init() {
	/* Calculate values to add to the parameters during acceleration */
	lst_control_q1_accel_plus_p     = (float)(LST_CONTROL_Q1_FAST_STEERING_P  - LST_CONTROL_Q1_SLOW_STEERING_P) /(float)(LST_CONTROL_Q1_ACCEL_TIME);
	lst_control_q1_accel_plus_d     = (float)(LST_CONTROL_Q1_FAST_STEERING_D  - LST_CONTROL_Q1_SLOW_STEERING_D) /(float)(LST_CONTROL_Q1_ACCEL_TIME);
	lst_control_q1_accel_plus_motor = (float)(LST_CONTROL_Q1_FAST_MOTOR_SPEED - LST_CONTROL_Q1_SLOW_MOTOR_SPEED)/(float)(LST_CONTROL_Q1_ACCEL_TIME);
}

/**
 * @brief Handles control of the car
 */
void LST_Control(){
  /* Switch between control modes based on the GamePad */
  LST_Control_Select_Mode();

  /* Number of lines */
  LST_Control_Resolve_Line_Mode();

  /* Handle PWM controls */
  lst_control_steering = 0;
  lst_control_motor = 0;

  switch(lst_control_mode){
  case LST_CONTROL_MODE_BT:
    LST_Control_Reset_State_Machine();

    lst_control_steering = LST_Control_Servo_BT();
    lst_control_motor = LST_Control_Motor_BT();
    break;
  case LST_CONTROL_MODE_LINE_FOLLOW:
    LST_Control_Reset_State_Machine();

    /* Set acceleration from GamePad */
    lst_control_motor = LST_Control_Motor_BT();
    /* Get line position from the data */
    lst_control_steering = LST_Control_SteeringController();
    break;
  case LST_CONTROL_MODE_Q1:
  	LST_Control_Q1();
    break;
  case LST_CONTROL_MODE_STOP:
  default:
    /* Leave values at default */
    LST_Control_Reset_State_Machine();
    break;
  }

  LST_TIM_SetServoRcPwm(lst_control_steering + lst_control_steering_offset);
  LST_TIM_SetMotorRcPwm(lst_control_motor);
}

void LST_Control_Q1(){
	switch(lst_control_q1_mode){
	case LST_CONTROL_MODE_Q1_START:
		/* Set controller values */
		lst_control_steeringP = LST_CONTROL_Q1_START_STEERING_P;
		lst_control_steeringD = LST_CONTROL_Q1_START_STEERING_D;

		/* Accelerate to the desired starting speed */
		if(cntr_q1_start < LST_CONTROL_Q1_START_TIME){
			lst_control_motor = LST_CONTROL_Q1_START_MOTOR_SPEED;
			cntr_q1_start++;
		}
		else{
			lst_control_q1_mode = LST_CONTROL_MODE_Q1_SLOW;
			cntr_q1_start = 0;
		}
		break;
	case LST_CONTROL_MODE_Q1_SLOW:
		/* Set controller values */
		lst_control_steeringP = LST_CONTROL_Q1_SLOW_STEERING_P;
		lst_control_steeringD = LST_CONTROL_Q1_SLOW_STEERING_D;

		/* Set motor value */
		lst_control_motor = LST_CONTROL_Q1_SLOW_MOTOR_SPEED;

		/* Check for dotted lines */
		// Check for triple lines
		if(flag_q1_slow_triple_search == 1){
			if (lst_control_line_no < 3) cntr_q1_slow_triple = 0;
			if (lst_control_line_no == 3) cntr_q1_slow_triple++;
			if(cntr_q1_slow_triple > LST_CONTROL_Q1_SLOW_FILTER_THRESHOLD){
				flag_q1_slow_triple_search = 0;
				cntr_q1_slow_triple = 0;
			}
		}
		// Check for single lines
		else{
			if (lst_control_line_no > 1) cntr_q1_slow_single = 0;
			if (lst_control_line_no == 1) cntr_q1_slow_single++;
			if (cntr_q1_slow_single > LST_CONTROL_Q1_SLOW_FILTER_THRESHOLD){
				flag_q1_slow_triple_search = 1;
				cntr_q1_slow_single = 0;
				cntr_q1_slow_dotted_lines++;
			}
		}

		/* Check for total number of dotted lines */
		if(cntr_q1_slow_dotted_lines > LST_CONTROL_Q1_SLOW_LINES_THRESHOLD){
			lst_control_q1_mode = LST_CONTROL_MODE_Q1_ACCEL;
			cntr_q1_slow_dotted_lines = 0;
		}
		break;
	case LST_CONTROL_MODE_Q1_ACCEL:
		/* Linearly increase everything from slow to fast */
		if(cntr_q1_accel < LST_CONTROL_Q1_ACCEL_TIME){
			lst_control_steeringP  += lst_control_q1_accel_plus_p;
			lst_control_steeringD  += lst_control_q1_accel_plus_d;
			lst_control_motor_float+= lst_control_q1_accel_plus_motor;
			lst_control_motor       = lst_control_motor_float;

			cntr_q1_accel++;
		}
		else{
			lst_control_motor_float = LST_CONTROL_Q1_SLOW_MOTOR_SPEED;
			lst_control_q1_mode = LST_CONTROL_MODE_Q1_FAST;
			cntr_q1_accel = 0;
		}
		break;
	case LST_CONTROL_MODE_Q1_FAST:
		/* Set controller values */
		lst_control_steeringP = LST_CONTROL_Q1_FAST_STEERING_P;
		lst_control_steeringD = LST_CONTROL_Q1_FAST_STEERING_D;

		/* Set motor value */
		lst_control_motor = LST_CONTROL_Q1_FAST_MOTOR_SPEED;

		/* Check for triple lines */
		if (lst_control_line_no < 3) cntr_q1_fast_triple_line = 0;
		if (lst_control_line_no == 3) cntr_q1_fast_triple_line++;
		if(cntr_q1_fast_triple_line>LST_CONTROL_Q1_FAST_TRIPLE_LINES){
			/* Switch to braking */
			lst_control_q1_mode = LST_CONTROL_MODE_Q1_BRAKE;
			cntr_q1_fast_triple_line = 0;
		}
		break;
	case LST_CONTROL_MODE_Q1_BRAKE:
		/* Set controller values */
		lst_control_steeringP = LST_CONTROL_Q1_BRAKE_STEERING_P;
		lst_control_steeringD = LST_CONTROL_Q1_BRAKE_STEERING_D;

		/* Satufék */
		if(cntr_q1_brake<LST_CONTROL_BRAKE_DELAY){
			lst_control_motor = LST_CONTROL_Q1_BRAKE_MOTOR;
			cntr_q1_brake++;
		}else if(cntr_q1_brake < 2*LST_CONTROL_BRAKE_DELAY){
			lst_control_motor = 0;
			cntr_q1_brake++;
		}else if(cntr_q1_brake < 2*LST_CONTROL_BRAKE_DELAY + LST_CONTROL_BRAKE_TIME){
			lst_control_motor = LST_CONTROL_Q1_BRAKE_MOTOR;
			cntr_q1_brake++;
		}else{
			lst_control_q1_mode = LST_CONTROL_MODE_Q1_SLOW;
			cntr_q1_brake = 0;
		}
		break;
	}
}

void LST_Control_Reset_State_Machine(){
	lst_control_motor_float      = LST_CONTROL_Q1_SLOW_MOTOR_SPEED;
	cntr_q1_fast_triple_line     = 0;
	cntr_q1_start 							 = 0;
	cntr_q1_brake 							 = 0;
	cntr_q1_accel								 = 0;
	flag_q1_slow_triple_search   = 1;
  lst_control_line_no          = 0;
  lst_control_line_no_input    = 0;
  cntr_q1_slow_triple 			   = 0;
	cntr_q1_slow_single 				 = 0;
	cntr_q1_slow_dotted_lines 	 = 0;
  lst_control_q1_mode          = LST_CONTROL_MODE_Q1_START;
}

/**
 * @brief Resolves line mode
 */
void LST_Control_Resolve_Line_Mode(){
  // Get lumber of lines from Line Controller
  lst_control_line_no_input = lst_spi_master1_rx[1] >> 1;

  // Line values array
  uint8_t cccntr = 0;
  for(cccntr=0; cccntr<4; cccntr++){
    lst_control_line_no_array[cccntr+1] = lst_control_line_no_array[cccntr];
  }
  lst_control_line_no_array[0] = lst_control_line_no_input;


  // Check if all values are the same
  // ToDo Doesn't work as expected but good enough
  uint8_t temp1 = lst_control_line_no_array[0];
  uint8_t flag1 = 1;
  for(cccntr=1; cccntr<4; cccntr++){
    if(lst_control_line_no_array[cccntr] != temp1){
      flag1 = 0;
    }
  }

  // If all values are the same, change internal line counter
  if(flag1 == 1){
    lst_control_line_no = temp1;
  }
}

/**
 * @brief BT control of the servo
 */
int16_t LST_Control_Servo_BT(){
  int16_t steering = 0;

  float temp1 = 0;
  temp1 = lst_bt_gamepad_values[LST_GAMEPAD_AXIS_LX]
      - LST_GAMEPAD_AXIS_MIDDLE;
  temp1 = temp1 / LST_CONTROL_BT_STEERING_DENUM;
  steering += temp1;

  return steering;
}

/**
 * @brief BT control of the motor
 */
int16_t LST_Control_Motor_BT(){
  int16_t motor = 0;

  float temp1 = 0;
  temp1 = lst_bt_gamepad_values[LST_GAMEPAD_AXIS_RY]
      - LST_GAMEPAD_AXIS_MIDDLE;
  temp1 = temp1 / LST_CONTROL_BT_MOTOR_DENUM;
  motor += temp1;

  return motor;
}

/**
 * @brief Switches between different modes of control
 */
void LST_Control_Select_Mode(){
  /* Switch between BT/automatic mode */
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_A] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_control_mode = LST_CONTROL_MODE_BT;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_B] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_control_mode = LST_CONTROL_MODE_LINE_FOLLOW;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_control_mode = LST_CONTROL_MODE_Q1_SLOW;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_R1] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_control_mode = LST_CONTROL_MODE_Q1_FAST;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L2] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_control_mode = LST_CONTROL_MODE_Q1;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_BUTTON_Y] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
    lst_control_mode = LST_CONTROL_MODE_STOP;
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

  /* Steering offset */
//  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_L2] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
//    lst_control_steering_offset += 1;
//  }
//  if(lst_bt_gamepad_values[LST_GAMEPAD_TRIGGER_R2] == LST_GAMEPAD_BUTTON_STATE_PRESSED){
//    lst_control_steering_offset -= 1;
//  }

  if(lst_control_steeringP<0) lst_control_steeringP = 0;
  if(lst_control_steeringD<0) lst_control_steeringD = 0;
}

/**
 * @brief temp Calculates the line position
 */
float LST_Control_GetLinePosition() {
  uint8_t getlp_cntr = 0;
  uint16_t numerator = 16;
  uint16_t denumerator = 1;
  float line_result = 0.0f;

  for(getlp_cntr=0; getlp_cntr<32; getlp_cntr++){
    if(lst_spi_master1_rx[getlp_cntr + 6]>LST_CONTROL_LINESENSOR_THRESHOLD){
      numerator += lst_spi_master1_rx[getlp_cntr + 6]*getlp_cntr;
      denumerator += lst_spi_master1_rx[getlp_cntr + 6];
    }
  }

  line_result = numerator/denumerator;
  line_result = (line_result-16)*95.3;

  return line_result;
}

/**
 * @brief PI controller for the steering
 */
int32_t LST_Control_SteeringController(){
  int32_t str_cntrl_result = 0;

  lst_control_linePosOld = lst_control_linePos;
  // lst_control_linePosSum += lst_control_linePos;
  uint16_t line = (lst_spi_master1_rx[3] << 8) | (lst_spi_master1_rx[2]);
  lst_control_linePos = (line - 0x8000);

  /*    PD Controller    */
  /* Divide PD parameters into float */
  float floatP = lst_control_steeringP / 16384.0f;
  float floatD = lst_control_steeringD / 1630.0f;
  /* Reference is always 0 (middle of line sensor) */
  int16_t reference = 0;

  /* Error signal */
  lst_control_errorSignalOld = lst_control_errorSignal;
  lst_control_errorSignal = lst_control_linePos - reference;

  /* System input */
  int32_t system_input = floatP*lst_control_errorSignal +
      floatD*(lst_control_errorSignal - lst_control_errorSignalOld);

  str_cntrl_result = -system_input / LST_CONTROL_STEERING_DENUM;

  /* Max/Min */
  if (str_cntrl_result < LST_TIM_RCPWM_MIN) str_cntrl_result = LST_TIM_RCPWM_MIN;
  if (str_cntrl_result > LST_TIM_RCPWM_MAX) str_cntrl_result = LST_TIM_RCPWM_MAX;

  return str_cntrl_result;
}
