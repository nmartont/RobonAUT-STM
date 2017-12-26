/*
 * lst_fastlap.h
 *
 *  Created on: 2017. dec. 26.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_FASTLAP_H_
#define LST_FASTLAP_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"
#include "lst_spi.h"
#include "lst_bt.h"
#include "lst_timer.h"

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void LST_Fast_Init();
void LST_Fast_Logic();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

#endif /* LST_FASTLAP_H_ */
