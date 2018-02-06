/*
 * lst_utils.c
 *
 *  Created on: 2017. nov. 22.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_utils.h"

/* Private functions ---------------------------------------------------------*/
static float interpolate_segment(float x0, float y0, float x1, float y1, float x);

/* Private variables ---------------------------------------------------------*/
float interpolate_return[2];

/******************************************************************************/
/*                  Utility functions for RobonAUT 2018 Team LST              */
/******************************************************************************/

/**
 * @brief Copies len number of bytes from pSrc to pDest
 */
void LST_Utils_Memory_Copy(uint8_t *pDest, uint8_t *pSrc, uint16_t len) {
  uint16_t i;
  // Manually copy the data
  for (i = 0; i < len; i++) {
    // Copy data from source to destination
    *pDest++ = *pSrc++;
  }
}

/**
* Returns the interpolated y-value.
* Saturates to y0 or y1 if x outside interval [x0, x1].
*/
static float interpolate_segment(float x0, float y0, float x1, float y1, float x)
{
    float t;

    if (x <= x0) { return y0; }
    if (x >= x1) { return y1; }

    t =  (x-x0);
    t /= (x1-x0);

    return y0 + t*(y1-y0);
}

/* 1D Table lookup with interpolation */
float* LST_Utils_Interpolate_Table_PD(struct table_pd_interpol *table, float x){
  uint8_t segment;

  /* Check input bounds and saturate if out-of-bounds */
  if (x > (table->speed_values[table->speed_length-1])) {
    /* x-value too large, saturate to max y-value */
    interpolate_return[0] = table->p_values[table->speed_length-1];
    interpolate_return[1] = table->d_values[table->speed_length-1];
    return interpolate_return;
  }
  else if (x < (table->speed_values[0])) {
    /* x-value too small, saturate to min y-value */

    interpolate_return[0] = table->p_values[0];
    interpolate_return[1] = table->d_values[0];
    return interpolate_return;
  }

  /* Find the segment that holds x */
  for (segment = 0; segment<(table->speed_length-1); segment++)
  {
    if ((table->speed_values[segment]   <= x) &&
        (table->speed_values[segment+1] >= x))
    {
      /* Found the correct segment */
      /* Interpolate */
      interpolate_return[0] =  interpolate_segment(
                table->speed_values[segment],   /* x0 */
                table->p_values[segment],       /* y0 */
                table->speed_values[segment+1], /* x1 */
                table->p_values[segment+1],     /* y1 */
                x);                             /* x  */
      interpolate_return[1] =  interpolate_segment(
                table->speed_values[segment],   /* x0 */
                table->d_values[segment],       /* y0 */
                table->speed_values[segment+1], /* x1 */
                table->d_values[segment+1],     /* y1 */
                x);                             /* x  */
      return interpolate_return;
    }
  }

  /* Something with the data was wrong if we get here */
  /* Saturate to the max value */
  interpolate_return[0] = table->p_values[table->speed_length-1];
  interpolate_return[1] = table->d_values[table->speed_length-1];
  return interpolate_return;
}
