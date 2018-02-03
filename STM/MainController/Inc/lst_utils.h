/*
 * lst_utils.h
 *
 *  Created on: 2017. nov. 22.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_UTILS_H_
#define LST_UTILS_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"

/* Defines -------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* 1D interpolation structure definition */
struct table_pd_interpol {
    uint8_t speed_length;

    float *speed_values;
    float *p_values;
    float *d_values;
};

/* Function prototypes -------------------------------------------------------*/
void LST_Utils_Memory_Copy(uint16_t *pDest, uint16_t *pSrc, uint16_t len);
float* LST_Utils_Interpolate_Table_PD(struct table_pd_interpol *table, float x);

/* External variables --------------------------------------------------------*/

#endif /* LST_UTILS_H_ */
