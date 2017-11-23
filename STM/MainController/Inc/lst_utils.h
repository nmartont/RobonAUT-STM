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

/* Function prototypes -------------------------------------------------------*/
void memoryCopy(uint8_t *pDest, uint8_t *pSrc, uint8_t len);

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

#endif /* LST_UTILS_H_ */
