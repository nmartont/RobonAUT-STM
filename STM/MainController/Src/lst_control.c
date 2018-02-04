/*
 * lst_control.c
 *
 *  Created on: 2017. nov. 28.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_control.h"

/* Private variables ---------------------------------------------------------*/
/* Counter for time progression */
uint32_t lst_control_time_cntr = 0;

/* Variables for the lost line detection */
uint8_t lst_control_line_lost_flag = 0;
uint8_t cntr_lost_lines            = 0;

/* Variables for the line number calculation */
uint8_t lst_control_line_no_array[8] = { 0x00 };
uint8_t lst_control_line_no          = 0;
uint8_t lst_control_line_no_input    = 0;

/* Speed filter */
//float lst_control_speed_array[LST_CONTROL_SPEED_FILTER_ORDER] = {0.0f};

/* Line positions for steering PID controller */
float lst_control_linePos =     0.0f;
float lst_control_linePosOld =  0.0f;
// float lst_control_linePosSum =  0.0f;

/* Steering controller parameters */
uint16_t lst_control_steeringP = LST_CONTROL_STEERING_P;
uint16_t lst_control_steeringD = LST_CONTROL_STEERING_D;

/* Speed controller parameters */
uint16_t lst_control_speedP = LST_CONTROL_SPEED_P;
uint16_t lst_control_speedD = LST_CONTROL_SPEED_D;
uint16_t lst_control_speedI = LST_CONTROL_SPEED_I;

int32_t speed_cntrl_result_previous = 0;

/* Other controller variables */
int16_t lst_control_errorSignal_steering = 0;
int16_t lst_control_errorSignalOld_steering = 0;

int16_t lst_control_errorSignal_speed =     0;
int16_t lst_control_errorSignalOld_speed =  0;
int16_t lst_control_referenceOld_speed =    0;
int32_t lst_control_errorSignalSum_speed =  0;

// LST_SETTINGS Servo offset TODO move

/* Motor and steering variables */
// MOVED TO LST_MOVEMENT, LST_STEERING
int16_t lst_control_steering_offset = -115; // negative: left

/* Declare variables for steering controller interpolation */
// ToDo calibrate interpolation for steering controller parameters
static float interpol_Speed[LST_CONTROL_PD_INTERPOL_POINTS] = {0.0, 100.0, 200.0, 300.0, 400.0};
static float interpol_P[LST_CONTROL_PD_INTERPOL_POINTS]     = {5.0, 2.0,     0.5,  0.25,  0.05};
static float interpol_D[LST_CONTROL_PD_INTERPOL_POINTS]     = {0.1, 1.0,     2.0,   3.0,   4.0};

static struct table_pd_interpol steering_pd_table = {
    LST_CONTROL_PD_INTERPOL_POINTS,      /* Number of data points */
    interpol_Speed,                      /* Array of x-coordinates */
    interpol_P,                          /* Array of y-coordinates */
    interpol_D                           /* Array of y-coordinates */
};

/* Private functions ---------------------------------------------------------*/
static void LST_Control_Resolve_Line();
static uint8_t LST_Control_Check_Lost_Line();

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
  /* Increment time counter for time-dependent operations */
  lst_control_time_cntr++;

  /* Get line data from LineController */
#ifdef LST_CONFIG_UART_LINE_COM
  LST_UART_ReceiveLineControllerData();
#else
  LST_SPI_ReceiveLineControllerData();
#endif

  /* Get Inertial sensor data via interrupt routine */

  // TODO:TEST 2018.01.11
  if(lst_inertial_data_ready == 1) //{}
  //vTaskDelay(1);
  LST_Inertial_GetSensorData();

  /* ADC conversions */
  LST_ADC_StartSharpADC();

  /* Calculate and normalize speed from encoder */
  //lst_control_speed_encoder = LST_Control_CalculateSpeed();
  // !! moved to separate task

  /* Wait for ADC */
  LST_ADC_WaitForSharpADC();

  /* Wait for Inertial data */
  // TODO:TEST 2018.01.11
  //if(lst_inertial_data_ready == 1) LST_Inertial_WaitForSensorData();

  /* Wait for the end of the LineController transaction */
#ifdef LST_CONFIG_UART_LINE_COM
  LST_UART_WaitForLineControllerData();
#else
  LST_SPI_WaitForLineControllerData();
#endif

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
static uint8_t LST_Control_Check_Lost_Line(){
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
static void LST_Control_Resolve_Line(){
  /* Check for 0xFF control byte at the first byte of the SPI Rx buffer */
  // ToDo change control byte so that it's not FF, but say 0F
  if(lst_spi_master1_rx[0] < 254){ // FixMe last bit is bugged
    lst_spi_linecontroller_lost = 1;
    // return;
  }
  lst_spi_linecontroller_lost = 0;

  // Get line data
  lst_control_linePosOld = lst_control_linePos;
  // lst_control_linePosSum += lst_control_linePos;
  uint16_t line = (lst_spi_master1_rx[3] << 8) | (lst_spi_master1_rx[2]);
  lst_control_linePos = (line - 0x8000);

  // Get lumber of lines from Line Controller
  lst_control_line_no_input = lst_spi_master1_rx[1] >> 1;

  // Remember line position if line is lost
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
 * @brief PD controller for the steering
 */
int32_t LST_Control_SteeringController(uint8_t use_interpolation){
  int32_t str_cntrl_result = 0;
  float floatP;
  float floatD;

  /*    PD Controller    */
  if(!use_interpolation){
    /* Divide PD parameters into float */
    floatP = (float)lst_control_steeringP / LST_CONTROL_STEERING_P_DIVIDER;
    floatD = (float)lst_control_steeringD / LST_CONTROL_STEERING_D_DIVIDER;
  }else{
    float* interpol_result;
    interpol_result = LST_Utils_Interpolate_Table_PD(&steering_pd_table, lst_encoder_speed);
    floatP = *(interpol_result);
    floatD = *(interpol_result + 1);
    /* Set P and D in memory for debugging */
    lst_control_steeringP = floatP*LST_CONTROL_STEERING_P_DIVIDER;
    lst_control_steeringD = floatD*LST_CONTROL_STEERING_D_DIVIDER;
  }

  /* Reference is always 0 (middle of line sensor) */
  int16_t reference = 0;

  /* Error signal */
  lst_control_errorSignalOld_steering = lst_control_errorSignal_steering;
  lst_control_errorSignal_steering = reference - lst_control_linePos;

  /* System input */
  int32_t system_input =
      floatP*lst_control_errorSignal_steering +
      floatD*(lst_control_errorSignal_steering - lst_control_errorSignalOld_steering);

  str_cntrl_result = system_input / LST_CONTROL_STEERING_DENUM;

  /* Max/Min */
  if (str_cntrl_result < LST_TIM_SERVO_PWM_MIN) str_cntrl_result = LST_TIM_SERVO_PWM_MIN;
  if (str_cntrl_result > LST_TIM_SERVO_PWM_MAX) str_cntrl_result = LST_TIM_SERVO_PWM_MAX;

  return str_cntrl_result;
}

/**
 * @brief P controller for the Sharps
 */
int32_t LST_Control_SteeringControllerSharp(uint8_t sharp_dir, uint16_t dist){
  uint16_t error_signal = 0;
  int32_t str_cntrl_result = 0;

  // ToDo test

  if(sharp_dir == 0){ // left
    error_signal = dist - LST_Sharp_GetLeftDistance_mm();
  }else{              // right
    error_signal = -(dist - LST_Sharp_GetRightDistance_mm());
  }

  /* System input */
  int32_t system_input = LST_CONTROL_SHARP_P*error_signal; // Todo is this good?

  str_cntrl_result = system_input / LST_CONTROL_STEERING_DENUM;

  /* Max/Min */
  if (str_cntrl_result < LST_TIM_SERVO_PWM_MIN) str_cntrl_result = LST_TIM_SERVO_PWM_MIN;
  if (str_cntrl_result > LST_TIM_SERVO_PWM_MAX) str_cntrl_result = LST_TIM_SERVO_PWM_MAX;

  return str_cntrl_result;
}

/**
 * @brief PID controller for the speed
 */
int32_t LST_Control_SpeedController(int16_t reference){
  // ToDo Test rate limiter
  /*    Rate limiter    */
  /* Calculate the rate limit based on control freq */
  float difference =
      LST_CONTROL_TICKS_TIMEBASE*
      (float)(reference - lst_control_referenceOld_speed)/
      (float)LST_CONTROL_SPEED_CONTROL_TICKS;

  if(difference<-LST_CONTROL_MOTOR_RATE_LIMIT){
    reference = lst_control_referenceOld_speed -
            (float)LST_CONTROL_SPEED_CONTROL_TICKS*LST_CONTROL_MOTOR_RATE_LIMIT/
            LST_CONTROL_TICKS_TIMEBASE;
  }else if(difference>LST_CONTROL_MOTOR_RATE_LIMIT){
    reference = lst_control_referenceOld_speed +
            (float)LST_CONTROL_SPEED_CONTROL_TICKS*LST_CONTROL_MOTOR_RATE_LIMIT/
            LST_CONTROL_TICKS_TIMEBASE;
  }
  // TODO NB!!!! If control freq changes, CHANGE THE REPEAT TICKS CONSTANT TOO!

  lst_control_referenceOld_speed = reference;

  int32_t speed_cntrl_result = 0;

  /*    PID Controller    */
  /* Divide PID parameters into float */
  float floatP = lst_control_speedP / LST_CONTROL_SPEED_P_DIVIDER;
  float floatD = lst_control_speedD / LST_CONTROL_SPEED_D_DIVIDER;
#ifndef LST_CONTROL_MOTOR_NOINTEGRATOR
  float floatI = lst_control_speedI / LST_CONTROL_SPEED_I_DIVIDER;
#else
  float floatI = 0;
#endif

  /* Error signal */
  lst_control_errorSignalOld_speed = lst_control_errorSignal_speed;
  lst_control_errorSignal_speed = reference - lst_encoder_speed;

  /* Integrator */
  uint32_t new_i = lst_control_errorSignalSum_speed + lst_control_errorSignal_speed;

  /* System input */
  int32_t system_input =
      floatP*lst_control_errorSignal_speed +
      floatD*(lst_control_errorSignal_speed - lst_control_errorSignalOld_speed) +
      floatI*new_i;

  speed_cntrl_result = system_input / LST_CONTROL_SPEED_DENUM;

  /* Anti wind-up, handle saturation */
  uint8_t int_ok = 1;

  /* Positive saturation? */
  if (speed_cntrl_result > LST_TIM_MOTOR_PWM_MAX)
  {
    /* Clamp the output */
    speed_cntrl_result = LST_TIM_MOTOR_PWM_MAX;

    /* Error is the same sign? Inhibit integration. */
    if (lst_control_errorSignal_speed > 0)
    {
      int_ok = 0;
    }
  }
  /* Repeat for negative sign */
  else if (speed_cntrl_result < LST_TIM_MOTOR_PWM_MIN)
  {
    speed_cntrl_result = LST_TIM_MOTOR_PWM_MIN;

    if (lst_control_errorSignal_speed < 0)
    {
      int_ok = 0;
    }
  }

  /* Update the integrator if allowed. */
  if (int_ok == 1)
  {
    lst_control_errorSignalSum_speed = new_i;
  }

  // Rate limiter to prevent oscillation due to mechanical hysteresis
  // TODO TEST 2018. 02. 01.

  // Limit rise
  if (speed_cntrl_result_previous <
  		(speed_cntrl_result - LST_CONTROL_SPEED_RATELIMIT))
  {

  	speed_cntrl_result = speed_cntrl_result_previous +
  			LST_CONTROL_SPEED_RATELIMIT;

  }

  // Limit fall
  if (speed_cntrl_result_previous >
    		(speed_cntrl_result + LST_CONTROL_SPEED_RATELIMIT))
	{

		speed_cntrl_result = speed_cntrl_result_previous -
				LST_CONTROL_SPEED_RATELIMIT;

	}
/*
  // Apply reverse lock if needed TODO TEST 2018. 02. 02.
  if (lst_control_speed_reverseLock)
  {

  	if (speed_cntrl_result > 0) speed_cntrl_result = 0;

  }
*/
  speed_cntrl_result_previous = speed_cntrl_result;

  return speed_cntrl_result;
}

/**
 * @brief Sets the control for the servo and the motor
 */
void LST_Control_ServoAndMotor(){

// TODO TEST 2018. 01. 25. invert steering
#ifdef LST_INVERT_STEERING
  LST_TIM_SetServoRcPwm(-lst_control_steering + lst_control_steering_offset);
#else
  LST_TIM_SetServoRcPwm(lst_control_steering + lst_control_steering_offset);
#endif

#ifdef LST_CONFIG_CUSTOM_MOTOR_CONTROL
  LST_TIM_SetMotorUnipolPwm(lst_control_motor);
#else
  LST_TIM_SetMotorRcPwm(lst_control_motor);
#endif
}

/*
// TODO PURGE
float LST_Control_CalculateSpeed(){
  // ToDo test

  // TODO NB!!!! If control freq changes, CHANGE THE REPEAT TICKS CONSTANT TOO!
  float sp = -LST_CONTROL_TICKS_TIMEBASE*(float)LST_TIM_CalculateSpeed()
      /(float)LST_CONTROL_SPEED_CALC_TICKS;
  float sum = 0.0f;
  float temp = 0.0f;

  // Speed values array
  uint8_t cccntr = 0;
  for(cccntr=LST_CONTROL_SPEED_FILTER_ORDER - 1; cccntr!=255; cccntr--){
    temp = lst_control_speed_array[cccntr];
    sum = sum + temp;
    lst_control_speed_array[cccntr+1] = temp;
  }
  sum = sum + sp;
  lst_control_speed_array[0] = sp;

  return sum/(float)(LST_CONTROL_SPEED_FILTER_ORDER + 1);
}
*/

// TODO TEMP 2018. 01. 30. functions for task migration

