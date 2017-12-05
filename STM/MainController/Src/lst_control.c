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

uint16_t mode_cntr = 0;

/******************************************************************************/
/*                Controller handling for RobonAUT 2018 Team LST              */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Control part of the software
 */
void LST_Control_Init() {

}

/**
 * @brief Handles control of the car
 */
void LST_Control(){
  /* Switch between control modes based on the GamePad */
  LST_Control_Select_Mode();

  /* Handle PWM controls */
  lst_control_steering = 0;
  lst_control_motor = 0;

  switch(lst_control_mode){
  case LST_CONTROL_MODE_BT:
    // ToDo temp
    mode_cntr = 0;

    lst_control_steering = LST_Control_Servo_BT();
    lst_control_motor = LST_Control_Motor_BT();
    break;
  case LST_CONTROL_MODE_LINE_FOLLOW:
    // ToDo temp
    mode_cntr = 0;

    /* Set acceleration from GamePad */
    lst_control_motor = LST_Control_Motor_BT();
    /* Get line position from the data */
    lst_control_steering = LST_Control_SteeringController();
    break;
  case LST_CONTROL_MODE_Q1_SLOW:
    // ToDo temp
    mode_cntr = 0;

    /* Set acceleration */
//    if(mode_cntr < 15){
//      lst_control_motor = LST_CONTROL_Q1_SLOW_MOTOR_SPEED_1;
//      mode_cntr++;
//    }
//    else{
//      lst_control_motor = LST_CONTROL_Q1_SLOW_MOTOR_SPEED;
//    }

    lst_control_motor = LST_CONTROL_Q1_SLOW_MOTOR_SPEED;
    /* Set up controller parameters */
    lst_control_steeringP = LST_CONTROL_Q1_SLOW_P;
    lst_control_steeringD = LST_CONTROL_Q1_SLOW_D;
    /* Get line position from the data */
    lst_control_steering = LST_Control_SteeringController();
    break;
  case LST_CONTROL_MODE_Q1_FAST:
    if(mode_cntr<5){
      lst_control_motor = -700;
    }else if(mode_cntr<5){
      lst_control_motor = 0;
    }else if(mode_cntr<50){
      lst_control_motor = -700;
    }else{
      lst_control_motor = 0;
    }
    mode_cntr++;
    /* Set acceleration */
    // lst_control_motor = LST_CONTROL_Q1_FAST_MOTOR_SPEED;
    /* Set up controller parameters */
    lst_control_steeringP = LST_CONTROL_Q1_FAST_P;
    lst_control_steeringD = LST_CONTROL_Q1_FAST_D;
    /* Get line position from the data */
    // lst_control_steering = LST_Control_SteeringController();
    lst_control_steering = LST_Control_Servo_BT();
    break;
  case LST_CONTROL_MODE_Q1:
    /* ToDo Q1 mode switching */
    break;
  case LST_CONTROL_MODE_STOP:
  default:
    /* Leave values at default */
    // ToDo temp
    mode_cntr = 0;
    break;
  }

  LST_TIM_SetServoRcPwm(lst_control_steering);
  LST_TIM_SetMotorRcPwm(lst_control_motor);
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
    lst_control_steeringD += 5;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_SOUTH){
    lst_control_steeringD -= 5;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_WEST){
    lst_control_steeringP -= 20;
  }
  if(lst_bt_gamepad_values[LST_GAMEPAD_DPAD] == LST_GAMEPAD_DPAD_EAST){
    lst_control_steeringP += 20;
  }

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
    if(lst_spi_master1_rx[getlp_cntr + 6]>LST_CONTROL_LINESENSOR_TRESHOLD){
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
int16_t LST_Control_SteeringController(){
  int16_t str_cntrl_result = 0;

  lst_control_linePosOld = lst_control_linePos;
  // lst_control_linePos = LST_Control_GetLinePosition(); /* 1525 --- -1525 */
  // lst_control_linePosSum += lst_control_linePos;
  uint16_t line = (lst_spi_master1_rx[1] << 8) | (lst_spi_master1_rx[0]);
  lst_control_linePos = (line - 0x8000) / 21.487f;

  /*    PD Controller    */
  /* Divide PD parameters into float */
  float floatP = lst_control_steeringP / 16384.0f;
  float floatD = lst_control_steeringD / 32768.0f;
  /* Reference is always 0 (middle of line sensor) */
  int16_t reference = 0;  // ToDo: maybe change reference based on angle of steering

  /* Error signal */
  lst_control_errorSignalOld = lst_control_errorSignal;
  lst_control_errorSignal = lst_control_linePos - reference;

  /* System input */
  int16_t system_input = floatP*lst_control_errorSignal +
      floatD*(lst_control_errorSignal - lst_control_errorSignalOld);

  str_cntrl_result = -system_input;

  /* Max/Min */
  if (str_cntrl_result < LST_TIM_RCPWM_MIN) str_cntrl_result = LST_TIM_RCPWM_MIN;
  if (str_cntrl_result > LST_TIM_RCPWM_MAX) str_cntrl_result = LST_TIM_RCPWM_MAX;

  return str_cntrl_result;
}
