/*
 * lst_control.c
 *
 *  Created on: 2017. nov. 28.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_control.h"

/* Private variables ---------------------------------------------------------*/
/* Variables for the lost line detection */
uint8_t lst_control_line_lost_flag = 0;
uint8_t cntr_lost_lines            = 0;

/* Variables for the line number calculation */
uint8_t lst_control_line_no_array[8] = { 0x00 };
uint8_t lst_control_line_no          = 0;
uint8_t lst_control_line_no_input    = 0;

/* Line positions for steering PID controller */
float lst_control_linePos =     0.0f;
float lst_control_linePosOld =  0.0f;
// float lst_control_linePosSum =  0.0f;

/* Steering controller parameters */
uint16_t lst_control_steeringP = LST_CONTROL_STEERING_P;
uint16_t lst_control_steeringD = LST_CONTROL_STEERING_D;

/* Other controller variables */
int16_t lst_control_errorSignal = 0;
int16_t lst_control_errorSignalOld = 0;

/* Motor and steering variables */
int16_t lst_control_steering = 0;
int16_t lst_control_motor = 0;
int16_t lst_control_steering_offset = 0; // -150
float lst_control_speed = 0.0f;

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
 * @brief Runs the common functions of the controls, gets SPI data from
 *        line controller, gets ADC values, gets speed from encoder, etc.
 */
void LST_Control_Commons(){
  /* Get line data from LineController */
#ifdef LST_CONFIG_UART_LINE_COM
  LST_UART_ReceiveLineControllerData();
#else
  LST_SPI_ReceiveLineControllerData();
#endif

  /* ADC conversions */
  LST_ADC_StartSharpADC();

  /* ToDo I2C, other sensor data */

  /* Calculate and normalize speed from encoder */
  lst_control_speed = (float)LST_TIM_CalculateSpeed()/(float)LST_CONTROL_REPEAT_TICKS;

  /* Wait for ADC */
  LST_ADC_WaitForSharpADC();

  /* Wait for the end of the LineController transaction */
#ifdef LST_CONFIG_UART_LINE_COM
  LST_UART_WaitForLineControllerData();
#else
  LST_SPI_WaitForLineControllerData();
#endif

  /* ToDo Check for 0xFF control byte at the first byte of the SPI Rx buffer */
  /* ToDo Handle SPI Rx data in a separate module */

  /* Number of lines */
  LST_Control_Resolve_Line();

  /* Check for lost line */
  lst_control_line_lost_flag = LST_Control_Check_Lost_Line();

  /* Reset the steering and motor control values */
  lst_control_steering = 0;
  lst_control_motor = 0;
}

/**
 * @brief Returns 1 if the line is lost
 */
uint8_t LST_Control_Check_Lost_Line(){
  /* Count lost lines */
  if(lst_control_line_no == 0){
    cntr_lost_lines++;
  }
  else{
    cntr_lost_lines = 0;
  }
  /* If the car left the line, return 1 */
  return cntr_lost_lines > LST_CONTROL_LOST_LINES_THRESHOLD;
}

/**
 * @brief Resolves line mode
 */
void LST_Control_Resolve_Line(){
  // Get line data
  lst_control_linePosOld = lst_control_linePos;
  // lst_control_linePosSum += lst_control_linePos;
  uint16_t line = (lst_spi_master1_rx[3] << 8) | (lst_spi_master1_rx[2]);
  lst_control_linePos = (line - 0x8000);

  // Get lumber of lines from Line Controller
  lst_control_line_no_input = lst_spi_master1_rx[1] >> 1;

  // remember line position if line is lost
  if(lst_control_line_no_input == 0){
    lst_control_linePos = lst_control_linePosOld;
  }

  // Line values array
  uint8_t cccntr = 0;
  for(cccntr=LST_CONTROL_NEW_LINE_FILTER_THRESHOLD - 1; cccntr!=255; cccntr--){
    lst_control_line_no_array[cccntr+1] = lst_control_line_no_array[cccntr];
  }
  lst_control_line_no_array[0] = lst_control_line_no_input;

  // Filter: Check if all values are the same
  uint8_t temp1 = lst_control_line_no_array[0];
  uint8_t flag1 = 1;
  for(cccntr=1; cccntr<LST_CONTROL_NEW_LINE_FILTER_THRESHOLD; cccntr++){
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
 * @brief PI controller for the steering
 */
int32_t LST_Control_SteeringController(){
  int32_t str_cntrl_result = 0;

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

/**
 * @brief Sets the control for the servo and the motor
 */
void LST_Control_ServoAndMotor(){
  LST_TIM_SetServoRcPwm(lst_control_steering + lst_control_steering_offset);
  LST_TIM_SetMotorRcPwm(lst_control_motor);
}
