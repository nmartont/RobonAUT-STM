/*
 * lst_encoder.c
 *
 *  Created on: 31 Jan 2018
 *      Author: Balazs
 */

#include "tasks/lst_encoder.h"

float lst_control_speed_array[LST_ENCODER_SPEED_FILTER_ORDER] = {0.0f};
float lst_encoder_dist_meas_total = 0.0f;
uint8_t lst_encoder_dist_meas_state = LST_ENCODER_DIST_MEAS_NOT_STARTED;

void LST_Encoder_CalculateSpeed(){
  // ToDo test

  // TODO NB!!!! If control freq changes, CHANGE THE REPEAT TICKS CONSTANT TOO!
  float sp = -LST_ENCODER_MULTIPLIER * (float)LST_TIM_CalculateSpeed();
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

uint8_t LST_Encoder_MeasureDistance(float dist_mm){
  // Returns true if the distance is completed, false if not.
  // Distance calculation start on the first call when there is no ongoing measurement.
  if(lst_encoder_dist_meas_state != LST_ENCODER_DIST_MEAS_ONGOING){
    // start measurement
    lst_encoder_dist_meas_state = LST_ENCODER_DIST_MEAS_ONGOING;
    lst_encoder_dist_meas_total = 0.0f;
  }
  else{
    // If the measurement is ongoing
    // See if the distance is reached
    if(dist_mm > 0){
      if(dist_mm <= lst_encoder_dist_meas_total){
        lst_encoder_dist_meas_state = LST_ENCODER_DIST_MEAS_FINISHED;
      }
    }
    else{
      if(dist_mm >= lst_encoder_dist_meas_total){
        lst_encoder_dist_meas_state = LST_ENCODER_DIST_MEAS_FINISHED;
      }
    }
  }
  if(lst_encoder_dist_meas_state != LST_ENCODER_DIST_MEAS_FINISHED){
    // Advance distance
    lst_encoder_dist_meas_total += (float)lst_control_speed_encoder * LST_ENCODER_INCR_MM;
  }

  return lst_encoder_dist_meas_state;
}
