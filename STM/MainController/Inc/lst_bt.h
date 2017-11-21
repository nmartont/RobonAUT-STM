/*
 * lst_bt.h
 *
 *  Created on: 2017. nov. 17.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_BT_H_
#define LST_BT_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"
#include "main.h"

/* Defines -------------------------------------------------------------------*/
/* General BT defines */
#define BT_MESSAGE_END 0xFF

/* Key mapping for Ipega BT Controller */
/* Face Buttons */
#define GAMEPAD_BUTTONA 			48
#define GAMEPAD_BUTTONB 			49
#define GAMEPAD_BUTTONX 			51
#define GAMEPAD_BUTTONY				52
/* Analog Axes */
#define GAMEPAD_AXISLX 				0
#define GAMEPAD_AXISLY 				4
#define GAMEPAD_AXISRX 				8
#define GAMEPAD_AXISRY 				20
/* Triggers */
#define GAMEPAD_TRIGGERL1 		54
#define GAMEPAD_TRIGGERR1 		55
#define GAMEPAD_TRIGGERL2 		56
#define GAMEPAD_TRIGGERR2 		57
/* Misc buttons */
#define GAMEPAD_BUTTONSTART 	59
#define GAMEPAD_BUTTONSELECT 	58
#define GAMEPAD_BUTTONLSTICK 	61
#define GAMEPAD_BUTTONRSTICK 	62
/* D-Pad */
#define GAMEPAD_DPAD 32

/* Button states */
#define GAMEPAD_BUTTON_STATE_PRESSED	128
#define GAMEPAD_BUTTON_STATE_RELEASED 0
#define GAMEPAD_DPAD_RELEASED 				1
#define GAMEPAD_DPAD_NORTH 						0
#define GAMEPAD_DPAD_NORTHEAST 				4500
#define GAMEPAD_DPAD_EAST 						9000
#define GAMEPAD_DPAD_SOUTHEAST 				13500
#define GAMEPAD_DPAD_SOUTH 						18000
#define GAMEPAD_DPAD_SOUTHWEST 				22500
#define GAMEPAD_DPAD_WEST 						27000
#define GAMEPAD_DPAD_NORTHWEST 				31500

/* Button values */
#define GAMEPAD_AXIS_MAX 			0xFFFF
#define GAMEPAD_AXIS_MIDDLE 	0x8000

/* Function prototypes -------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

#endif /* LST_BT_H_ */
