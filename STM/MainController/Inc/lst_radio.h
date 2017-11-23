/*
 * lst_radio.h
 *
 *  Created on: 2017. nov. 23.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_RADIO_H_
#define LST_RADIO_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void LST_Radio_Process_Incoming_Byte();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

#endif /* LST_RADIO_H_ */
