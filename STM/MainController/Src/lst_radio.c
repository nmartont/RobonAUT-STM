/*
 * lst_radio.c
 *
 *  Created on: 2017. nov. 23.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_radio.h"

/* Private define ------------------------------------------------------------*/
uint8_t lst_radio_msg_5_received = LST_RADIO_MSG_NOT_RECEIVED;
uint8_t lst_radio_msg_4_received = LST_RADIO_MSG_NOT_RECEIVED;
uint8_t lst_radio_msg_3_received = LST_RADIO_MSG_NOT_RECEIVED;
uint8_t lst_radio_msg_2_received = LST_RADIO_MSG_NOT_RECEIVED;
uint8_t lst_radio_msg_1_received = LST_RADIO_MSG_NOT_RECEIVED;
uint8_t lst_radio_msg_0_received = LST_RADIO_MSG_NOT_RECEIVED;

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                   Radio handler  for RobonAUT 2018 Team LST                */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Radio part of the software
 */
void LST_Radio_Init() {
  
}

/**
 * @brief Processes a byte from the Radio module
 */
void LST_Radio_Process_Incoming_Byte() {
  if (lst_uart_buffer_uart1[0] == LST_RADIO_MSG_5)
    lst_radio_msg_5_received = LST_RADIO_MSG_RECEIVED;
  else if (lst_uart_buffer_uart1[0] == LST_RADIO_MSG_4)
    lst_radio_msg_4_received = LST_RADIO_MSG_RECEIVED;
  else if (lst_uart_buffer_uart1[0] == LST_RADIO_MSG_3)
    lst_radio_msg_3_received = LST_RADIO_MSG_RECEIVED;
  else if (lst_uart_buffer_uart1[0] == LST_RADIO_MSG_2)
    lst_radio_msg_2_received = LST_RADIO_MSG_RECEIVED;
  else if (lst_uart_buffer_uart1[0] == LST_RADIO_MSG_1)
    lst_radio_msg_1_received = LST_RADIO_MSG_RECEIVED;
  else if (lst_uart_buffer_uart1[0] == LST_RADIO_MSG_0)
    lst_radio_msg_0_received = LST_RADIO_MSG_RECEIVED;
}
