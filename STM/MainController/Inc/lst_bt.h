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
/* BT Defines ----------------------------------------------------------------*/
#define LST_BT_MESSAGE_END 0xFF
/* BT Message Types */
#define LST_BT_MSGTYPE_STATUSERROR	0
#define LST_BT_MSGTYPE_STATUSOK			1
#define LST_BT_MSGTYPE_STATUSREQ		2
#define LST_BT_MSGTYPE_VARLIST			3
#define LST_BT_MSGTYPE_VARVALUES		4
#define LST_BT_MSGTYPE_VARLISTREQ		5
#define LST_BT_MSGTYPE_MONITORSTART	6
#define LST_BT_MSGTYPE_MONITORSTOP	7
#define LST_BT_MSGTYPE_BTINPUT			8
/* BT Variable Types */
#define LST_BT_VARTYPE_UINT8		0
#define LST_BT_VARTYPE_INT8			1
#define LST_BT_VARTYPE_UINT16		2
#define LST_BT_VARTYPE_INT16		3
#define LST_BT_VARTYPE_UINT32		4
#define LST_BT_VARTYPE_INT32		5
/* BT Status Defines */
#define LST_BT_STATUS_OK				0
#define LST_BT_STATUS_ERROR			1

/* GamePad Defines ---------------------------------------------------------- */
/* Key mapping for Ipega BT Controller */
/* Face Buttons */
#define LST_GAMEPAD_BUTTON_A 				48
#define LST_GAMEPAD_BUTTON_B 				49
#define LST_GAMEPAD_BUTTON_X 				51
#define LST_GAMEPAD_BUTTON_Y				52
/* Analog Axes */
#define LST_GAMEPAD_AXIS_LX 				0
#define LST_GAMEPAD_AXIS_LY 				4
#define LST_GAMEPAD_AXIS_RX 				8
#define LST_GAMEPAD_AXIS_RY 				20
/* Triggers */
#define LST_GAMEPAD_TRIGGER_L1 			54
#define LST_GAMEPAD_TRIGGER_R1 			55
#define LST_GAMEPAD_TRIGGER_L2 			56
#define LST_GAMEPAD_TRIGGER_R2 			57
/* Misc buttons */
#define LST_GAMEPAD_BUTTON_START 		59
#define LST_GAMEPAD_BUTTON_SELECT 	58
#define LST_GAMEPAD_BUTTON_LSTICK 	61
#define LST_GAMEPAD_BUTTON_RSTICK 	62
/* D-Pad */
#define GAMEPAD_DPAD 								32

/* Button states */
#define LST_GAMEPAD_BUTTON_STATE_PRESSED	128
#define LST_GAMEPAD_BUTTON_STATE_RELEASED 0
#define LST_GAMEPAD_DPAD_RELEASED 				1
#define LST_GAMEPAD_DPAD_NORTH 						0
#define LST_GAMEPAD_DPAD_NORTHEAST 				4500
#define LST_GAMEPAD_DPAD_EAST 						9000
#define LST_GAMEPAD_DPAD_SOUTHEAST 				13500
#define LST_GAMEPAD_DPAD_SOUTH 						18000
#define LST_GAMEPAD_DPAD_SOUTHWEST 				22500
#define LST_GAMEPAD_DPAD_WEST 						27000
#define LST_GAMEPAD_DPAD_NORTHWEST 				31500

/* Button values */
#define LST_GAMEPAD_AXIS_MAX 			0xFFFF
#define LST_GAMEPAD_AXIS_MIDDLE 	0x8000

/* Function prototypes -------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

#endif /* LST_BT_H_ */
