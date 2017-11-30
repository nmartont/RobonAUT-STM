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
#include "lst_uart.h"

/* Defines -------------------------------------------------------------------*/
#define LST_RADIO_MSG_5 '5'
#define LST_RADIO_MSG_4 '4'
#define LST_RADIO_MSG_3 '3'
#define LST_RADIO_MSG_2 '2'
#define LST_RADIO_MSG_1 '1'
#define LST_RADIO_MSG_0 '0'

#define LST_RADIO_MSG_NOT_RECEIVED  0
#define LST_RADIO_MSG_RECEIVED      1

/* Function prototypes -------------------------------------------------------*/
void LST_Radio_Init();
void LST_Radio_Process_Incoming_Byte();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint8_t lst_radio_msg_5_received;
extern uint8_t lst_radio_msg_4_received;
extern uint8_t lst_radio_msg_3_received;
extern uint8_t lst_radio_msg_2_received;
extern uint8_t lst_radio_msg_1_received;
extern uint8_t lst_radio_msg_0_received;

#endif /* LST_RADIO_H_ */
