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
float linePos = 0;
float linePosSum = 0;
float linePosOld = 0;

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
  int16_t str_cntrl_result = 4575;

  linePosOld = linePos;
  linePos = LST_Control_GetLinePosition(); /* 1525 --- -1525 */
  linePosSum += linePos;

  /* PID Controller */
  /* Error signal */
  int16_t error_signal = LST_CONTROLLER_STEERING_P*linePos +
      LST_CONTROLLER_STEERING_I*linePosSum;

  str_cntrl_result = 4575 - error_signal;

  /* Max/Min */
  if (str_cntrl_result < 3050) str_cntrl_result = 3050;
  if (str_cntrl_result > 6100) str_cntrl_result = 6100;

  return str_cntrl_result;
}
