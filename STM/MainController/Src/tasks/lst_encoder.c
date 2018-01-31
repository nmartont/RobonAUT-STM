/*
 * lst_encoder.c
 *
 *  Created on: 31 Jan 2018
 *      Author: Balazs
 */

#include "tasks/lst_encoder.h"

float lst_control_speed_array[LST_ENCODER_SPEED_FILTER_ORDER] = {0.0f};

void LST_Encoder_CalculateSpeed(){
  // ToDo test

  // TODO NB!!!! If control freq changes, CHANGE THE REPEAT TICKS CONSTANT TOO!
  float sp = -LST_ENCODER_TICKS_TIMEBASE*(float)LST_TIM_CalculateSpeed()
      /(float)LST_ENCODER_SPEED_CALC_TICKS;
  float sum = 0.0f;
  float temp = 0.0f;

  // Speed values array
  uint8_t cccntr = 0;
  for(cccntr=LST_ENCODER_SPEED_FILTER_ORDER - 1; cccntr!=255; cccntr--){
    temp = lst_control_speed_array[cccntr];
    sum = sum + temp;
    lst_control_speed_array[cccntr+1] = temp;
  }
  sum = sum + sp;
  lst_control_speed_array[0] = sp;

  LST_Control_TEMP_setSpeedEncoder(
  		sum/(float)(LST_ENCODER_SPEED_FILTER_ORDER + 1));
}
