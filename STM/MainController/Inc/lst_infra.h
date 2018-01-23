/*
 * lst_infra.h
 *
 *  Created on: 2018. jan. 23.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_INFRA_H_
#define LST_INFRA_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"
#include "NEC_Decode.h"
#include "lst_timer.h"

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void LST_Infra_Init();

/* External variables --------------------------------------------------------*/
extern NEC nec;

#endif /* LST_INFRA_H_ */
