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
#include "lst_utils.h"
#include "lst_uart.h"
#include "lst_spi.h"

/* Defines -------------------------------------------------------------------*/
/* Buffer defines */
#define LST_BT_RX_BUFFER_SIZE 128

/* BT Defines ----------------------------------------------------------------*/
#define LST_BT_MESSAGE_END 0xFF
#define LST_BT_CONF_MSG_END1 '\r'
#define LST_BT_CONF_MSG_END2 '\n'
#define LST_BT_CONF_MSG_START1 'A'
#define LST_BT_CONF_MSG_START2 'T'
#define LST_BT_CONF_MSG_START3 '-'
#define LST_BT_CONF_MSG_START4 'A'
#define LST_BT_CONF_MSG_START5 'B'
#define LST_BT_CONF_MSG_START6 ' '

#define LST_BT_CONF_MSG_CONNECTION1   'C'
#define LST_BT_CONF_MSG_CONNECTION2   'o'
#define LST_BT_CONF_MSG_CONNECTION3   'n'
#define LST_BT_CONF_MSG_CONNECTION4   'n'
#define LST_BT_CONF_MSG_CONNECTION5   'e'
#define LST_BT_CONF_MSG_CONNECTION6   'c'
#define LST_BT_CONF_MSG_CONNECTION7   't'
#define LST_BT_CONF_MSG_CONNECTION8   'i'
#define LST_BT_CONF_MSG_CONNECTION9   'o'
#define LST_BT_CONF_MSG_CONNECTION10  'n'
#define LST_BT_CONF_MSG_CONNECTIONUP1 'U'
#define LST_BT_CONF_MSG_CONNECTIONUP2 'p'
#define LST_BT_CONF_MSG_CONNECTIONDOWN1 'D'
#define LST_BT_CONF_MSG_CONNECTIONDOWN2 'o'
#define LST_BT_CONF_MSG_CONNECTIONDOWN3 'w'
#define LST_BT_CONF_MSG_CONNECTIONDOWN4 'n'

#define LST_BT_CONNECTION_UP     1
#define LST_BT_CONNECTION_DOWN   0

/* BT Message Types */
#define LST_BT_MSGTYPE_STATUSERROR  0
#define LST_BT_MSGTYPE_STATUSOK     1
#define LST_BT_MSGTYPE_STATUSREQ    2
#define LST_BT_MSGTYPE_VARLIST      3
#define LST_BT_MSGTYPE_VARVALUES    4
#define LST_BT_MSGTYPE_VARLISTREQ   5
#define LST_BT_MSGTYPE_MONITORSTART 6
#define LST_BT_MSGTYPE_MONITORSTOP  7
#define LST_BT_MSGTYPE_BTINPUT      8
/* BT Variable Types */
#define LST_BT_VARTYPE_UINT8    0
#define LST_BT_VARTYPE_INT8     1
#define LST_BT_VARTYPE_UINT16   2
#define LST_BT_VARTYPE_INT16    3
#define LST_BT_VARTYPE_UINT32   4
#define LST_BT_VARTYPE_INT32    5
/* BT Status Defines */
#define LST_BT_STATUS_OK        0
#define LST_BT_STATUS_ERROR     1

/* GamePad Defines ---------------------------------------------------------- */
#define LST_GAMEPAD_ARRAY_SIZE      64
/* Key mapping for Ipega BT Controller */
/* Face Buttons */
#define LST_GAMEPAD_BUTTON_A        49
#define LST_GAMEPAD_BUTTON_B        50
#define LST_GAMEPAD_BUTTON_X        48
#define LST_GAMEPAD_BUTTON_Y        51
/* Analog Axes */
#define LST_GAMEPAD_AXIS_LX         0
#define LST_GAMEPAD_AXIS_LY         4
#define LST_GAMEPAD_AXIS_RX         8
#define LST_GAMEPAD_AXIS_RY         20
/* Triggers */
#define LST_GAMEPAD_TRIGGER_L1       52
#define LST_GAMEPAD_TRIGGER_R1       53
#define LST_GAMEPAD_TRIGGER_L2       54
#define LST_GAMEPAD_TRIGGER_R2       55
/* Misc buttons */
#define LST_GAMEPAD_BUTTON_START    57
#define LST_GAMEPAD_BUTTON_SELECT   56
#define LST_GAMEPAD_BUTTON_LSTICK   58
#define LST_GAMEPAD_BUTTON_RSTICK   59
/* D-Pad */
#define LST_GAMEPAD_DPAD            32

/* Button states */
#define LST_GAMEPAD_BUTTON_STATE_PRESSED  128
#define LST_GAMEPAD_BUTTON_STATE_RELEASED 0
#define LST_GAMEPAD_DPAD_RELEASED         1
#define LST_GAMEPAD_DPAD_NORTH            0
#define LST_GAMEPAD_DPAD_NORTHEAST        4500
#define LST_GAMEPAD_DPAD_EAST             9000
#define LST_GAMEPAD_DPAD_SOUTHEAST        13500
#define LST_GAMEPAD_DPAD_SOUTH            18000
#define LST_GAMEPAD_DPAD_SOUTHWEST        22500
#define LST_GAMEPAD_DPAD_WEST             27000
#define LST_GAMEPAD_DPAD_NORTHWEST        31500

/* Button values */
#define LST_GAMEPAD_AXIS_MAX       0xFFFF
#define LST_GAMEPAD_AXIS_MIDDLE    0x8000

/* Function prototypes -------------------------------------------------------*/
void LST_BT_Init();
void LST_BT_Process_Incoming_Byte();
void LST_BT_Send_StatusOk();
void LST_BT_Send_StatusError(uint8_t *error_msg, uint8_t error_msg_len);
void LST_BT_Send_StatusRequest();
void LST_BT_Send_VarList();
void LST_BT_Send_VarValues();
void LST_BT_ErrorHandler();

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern uint16_t lst_bt_gamepad_values[LST_GAMEPAD_ARRAY_SIZE];
extern uint8_t lst_bt_connection_status;
extern uint8_t lst_bt_pc_status;
extern uint8_t lst_bt_stm_status;
extern uint8_t lst_bt_send_status_flag;
extern uint8_t lst_bt_send_varlist_flag;
extern uint8_t lst_bt_send_diagdata_flag;

extern uint16_t lst_control_steeringP;
extern uint16_t lst_control_steeringD;

extern int16_t lst_control_steering;
extern int16_t lst_control_motor;
extern int16_t lst_control_steering_offset;

extern uint8_t lst_control_line_no;
extern uint8_t lst_control_q1_mode;

extern int16_t lst_control_speed;

#endif /* LST_BT_H_ */
