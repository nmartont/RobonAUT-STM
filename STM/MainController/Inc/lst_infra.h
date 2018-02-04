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
#include "lst_timer.h"

/* Defines -------------------------------------------------------------------*/
#define LST_INFRA_NOT_AVAILABLE   0
#define LST_INFRA_VAILABLE        1

#define LST_INFRA_DIR_LEFT        0
#define LST_INFRA_DIR_RIGHT       1

#define LST_INFRA_EXIT_ONE        1
#define LST_INFRA_EXIT_TWO        2
#define LST_INFRA_EXIT_THREE      3

/* Function prototypes -------------------------------------------------------*/
void LST_Infra_Init();
uint8_t LST_Infra_Get_Direction();
uint8_t LST_Infra_Get_Exit();
void RC5Reset();
uint8_t RC5CurrentBit(uint8_t edge, uint8_t lastBit, uint32_t pulse);

/* External variables --------------------------------------------------------*/
extern uint8_t lst_infra_is_available;

extern uint32_t inputCapture;
extern uint32_t ICValue1;
extern uint32_t ICValue2;
extern uint32_t highPulse;
extern uint32_t lowPulse;
extern uint32_t pulse;
extern uint32_t infraT;
extern uint32_t infraTolerance;
extern uint8_t RC5BitsBuffer[20];
extern uint8_t RC5BitsCounter;
extern uint8_t RC5Bit;
extern uint8_t RC5LastBit;
extern uint16_t RC5Word;
extern uint8_t infraAddress;
extern uint8_t infraCommand;
extern double infraPulses[100];
extern uint8_t infraPulseCounter;

#endif /* LST_INFRA_H_ */
