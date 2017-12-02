/*
 * lst_bt.c
 *
 *  Created on: 2017. nov. 17.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_bt.h"

/* Defines -------------------------------------------------------------------*/
#ifdef LST_CONFIG_LINECONTROLLER_DEBUG_DATA
#define LST_BT_VARLIST_DATALEN 245
#else
#define LST_BT_VARLIST_DATALEN 245
#endif

#define LST_BT_VARVALUES_DATALEN 14
/* Private variables ---------------------------------------------------------*/
/* Buffer for receiving bytes */
uint8_t buffer_rx[LST_BT_RX_BUFFER_SIZE] = { 0x00 };

/* Counter for buffer */
uint8_t buffer_rx_cntr = 0;

/* Array for gamepad values */
uint16_t lst_bt_gamepad_values[LST_GAMEPAD_ARRAY_SIZE] = { 0x00 };

/* BT Control Flags */
uint8_t lst_bt_connection_status = LST_BT_CONNECTION_DOWN;
uint8_t lst_bt_pc_status = LST_BT_STATUS_OK;
uint8_t lst_bt_stm_status = LST_BT_STATUS_OK;
uint8_t lst_bt_send_status_flag = 0;
uint8_t lst_bt_send_varlist_flag = 0;
uint8_t lst_bt_send_diagdata_flag = 0;

#ifdef LST_CONFIG_LINECONTROLLER_DEBUG_DATA
uint8_t buffer_varlist[LST_BT_VARLIST_DATALEN] = {
    0x05, 'L', 'i', 'n', 'e', '1', LST_BT_VARTYPE_INT16,
    0x05, 'L', 'i', 'n', 'e', '2', LST_BT_VARTYPE_INT16,
    0x05, 'L', 'i', 'n', 'e', '3', LST_BT_VARTYPE_INT16,
    0x05, 'V', 'a', 'l', '0', '0', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '1', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '2', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '3', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '4', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '5', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '6', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '7', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '8', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '0', '9', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '0', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '1', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '2', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '3', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '4', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '5', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '6', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '7', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '8', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '1', '9', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '0', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '1', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '2', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '3', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '4', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '5', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '6', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '7', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '8', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '2', '9', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '3', '0', LST_BT_VARTYPE_UINT8,
    0x05, 'V', 'a', 'l', '3', '1', LST_BT_VARTYPE_UINT8,};
#else
uint8_t buffer_varlist[LST_BT_VARLIST_DATALEN] = {
  0x05, 'L', 'i', 'n', 'e', '1', LST_BT_VARTYPE_INT16,
  0x05, 'L', 'i', 'n', 'e', '2', LST_BT_VARTYPE_INT16,
  0x05, 'L', 'i', 'n', 'e', '3', LST_BT_VARTYPE_INT16,
  0x05, 'V', 'a', 'l', '0', '0', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '1', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '2', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '3', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '4', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '5', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '6', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '7', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '8', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '0', '9', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '0', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '1', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '2', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '3', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '4', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '5', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '6', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '7', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '8', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '1', '9', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '0', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '1', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '2', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '3', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '4', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '5', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '6', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '7', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '8', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '2', '9', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '3', '0', LST_BT_VARTYPE_UINT8,
  0x05, 'V', 'a', 'l', '3', '1', LST_BT_VARTYPE_UINT8,};
#endif

/* Function prototypes ------------------------------------------------------ */
inline void gamepad_default_values();
inline void process_bt_message();
inline void process_config_message();

/******************************************************************************/
/*                 BlueTooth handling for RobonAUT 2018 Team LST              */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the BT part of the software
 */
void LST_BT_Init() {
  gamepad_default_values();
}

inline void gamepad_default_values() {
  uint8_t cntr = 0;
  for (cntr = 0; cntr < LST_GAMEPAD_ARRAY_SIZE; cntr++) {
    lst_bt_gamepad_values[cntr] = LST_GAMEPAD_BUTTON_STATE_RELEASED;
  }
  
  /* Default value of GAMEPAD_DPAD is 1 */
  lst_bt_gamepad_values[LST_GAMEPAD_DPAD] = LST_GAMEPAD_DPAD_RELEASED;
  /* Default value of axes */
  lst_bt_gamepad_values[LST_GAMEPAD_AXIS_LX] = LST_GAMEPAD_AXIS_MIDDLE;
  lst_bt_gamepad_values[LST_GAMEPAD_AXIS_LY] = LST_GAMEPAD_AXIS_MIDDLE;
  lst_bt_gamepad_values[LST_GAMEPAD_AXIS_RX] = LST_GAMEPAD_AXIS_MIDDLE;
  lst_bt_gamepad_values[LST_GAMEPAD_AXIS_RY] = LST_GAMEPAD_AXIS_MIDDLE;
}

/* Data processing functions -------------------------------------------------*/
/**
 * @brief Processes a byte from the BT module
 */
void LST_BT_Process_Incoming_Byte() {
  /* Check for message end byte */
  if (lst_uart_buffer_uart2[0] == LST_BT_MESSAGE_END) {
    /* Process message */
    process_bt_message();
    /* Reset counter */
    buffer_rx_cntr = 0;
  }
  /* Check for message end byte for config messages from the BT module */
  else if (lst_uart_buffer_uart2[0] == LST_BT_CONF_MSG_END2) { // Last byte of the message is '\n'
    if (buffer_rx_cntr > 0) {
      if (buffer_rx[buffer_rx_cntr - 1] == LST_BT_CONF_MSG_END1) { // Second to last byte of the message is '\r'
        process_config_message();
        /* Reset counter */
        buffer_rx_cntr = 0;
      }
    }
  } else {
    /* Store byte in buffer */
    buffer_rx[buffer_rx_cntr] = lst_uart_buffer_uart2[0];
    /* Increment buffer counter */
    if (buffer_rx_cntr == LST_BT_RX_BUFFER_SIZE - 1) {
      buffer_rx_cntr = 0;
    } else {
      buffer_rx_cntr++;
    }
  }
}

/**
 * @brief Processes a BT message from the computer
 */
inline void process_bt_message() {
  /* Check if counter is greater than zero */
  if (buffer_rx_cntr == 0)
    return;
  
  /* Switch case for msg type */
  switch (buffer_rx[0]) {
  case LST_BT_MSGTYPE_STATUSREQ:
    /* Message counter value should be 1 */
    if (buffer_rx_cntr != 1) return;
    /* Do stuff */
    lst_bt_send_status_flag = 1;
    break;
  case LST_BT_MSGTYPE_STATUSOK:
    /* Message counter value should be 1 */
    if (buffer_rx_cntr != 1) return;
    /* Do stuff */
    lst_bt_pc_status = LST_BT_STATUS_OK;
    break;
  case LST_BT_MSGTYPE_STATUSERROR:
    /* Message counter value should greater than 2 */
    if (buffer_rx_cntr < 2) return;
    /* Do stuff */
    lst_bt_pc_status = LST_BT_STATUS_ERROR;
    break;
  case LST_BT_MSGTYPE_VARLISTREQ:
    /* Message counter value should be 1 */
    if (buffer_rx_cntr != 1) return;
    /* Do stuff */
    lst_bt_send_varlist_flag = 1;
    break;
  case LST_BT_MSGTYPE_MONITORSTART:
    /* Message counter value should be 1 */
    if (buffer_rx_cntr != 1) return;
    /* Do stuff */
    lst_bt_send_diagdata_flag = 1;
    break;
  case LST_BT_MSGTYPE_MONITORSTOP:
    /* Message counter value should be 1 */
    if (buffer_rx_cntr != 1) return;
    /* Do stuff */
    lst_bt_send_diagdata_flag = 0;
    break;
  case LST_BT_MSGTYPE_BTINPUT:
    /* Message counter value should be 4 */
    if (buffer_rx_cntr != 4) return;
    /* We cannot get 0xFF: replace 0xFE for 0xFF so that the max value can be properly set */
    if (buffer_rx[3] == 0xFE)
      buffer_rx[3] = 0xFF;
    if (buffer_rx[2] == 0xFE)
      buffer_rx[2] = 0xFF;
    /* Store value */
    lst_bt_gamepad_values[buffer_rx[1]] = (buffer_rx[3] << 8) | (buffer_rx[2]);
    break;
  default:
    return;
  }
}

/**
 * @brief Processes a BT config message from the BT module
 */
inline void process_config_message() {
  /* Check if counter is greater than 8 */
  if (buffer_rx_cntr < 8)
    return;
  
  /* Check if the start of the message is the appropriate characters */
  if (buffer_rx[0] != LST_BT_CONF_MSG_START1) return;
  if (buffer_rx[1] != LST_BT_CONF_MSG_START2) return;
  if (buffer_rx[2] != LST_BT_CONF_MSG_START3) return;
  if (buffer_rx[3] != LST_BT_CONF_MSG_START4) return;
  if (buffer_rx[4] != LST_BT_CONF_MSG_START5) return;
  if (buffer_rx[5] != LST_BT_CONF_MSG_START6) return;
  
  /* Do the processing */
  /* Messages we care about:
   * ConnectionUp[...]
   * ConnectionDown[...]
   * */
  if (buffer_rx[6] != LST_BT_CONF_MSG_CONNECTION1)   return;
  if (buffer_rx[7] != LST_BT_CONF_MSG_CONNECTION2)   return;
  if (buffer_rx[8] != LST_BT_CONF_MSG_CONNECTION3)   return;
  if (buffer_rx[9] != LST_BT_CONF_MSG_CONNECTION4)   return;
  if (buffer_rx[10] != LST_BT_CONF_MSG_CONNECTION5)  return;
  if (buffer_rx[11] != LST_BT_CONF_MSG_CONNECTION6)  return;
  if (buffer_rx[12] != LST_BT_CONF_MSG_CONNECTION7)  return;
  if (buffer_rx[13] != LST_BT_CONF_MSG_CONNECTION8)  return;
  if (buffer_rx[14] != LST_BT_CONF_MSG_CONNECTION9)  return;
  if (buffer_rx[15] != LST_BT_CONF_MSG_CONNECTION10) return;
  
  /* Determine if Up or Down */
  if (buffer_rx[16] == LST_BT_CONF_MSG_CONNECTIONUP1
      && buffer_rx[17] == LST_BT_CONF_MSG_CONNECTIONUP2) {
    /* Do the thing */
    lst_bt_connection_status = LST_BT_CONNECTION_UP;
    return;
  }
  if (buffer_rx[16] == LST_BT_CONF_MSG_CONNECTIONDOWN1
      && buffer_rx[17] == LST_BT_CONF_MSG_CONNECTIONDOWN2
      && buffer_rx[18] == LST_BT_CONF_MSG_CONNECTIONDOWN3
      && buffer_rx[19] == LST_BT_CONF_MSG_CONNECTIONDOWN4) {
    /* Do the thing */
    lst_bt_connection_status = LST_BT_CONNECTION_DOWN;
    gamepad_default_values();
    return;
  }
}

/* Message sender functions --------------------------------------------------*/
/**
 * @brief Sends an UART BT StatusOK message
 */
void LST_BT_Send_StatusOk() {
  lst_bt_send_status_flag = 0;
  
  /* Wait until other sender functions are not using the BT */
  while (lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT) {
  }
  lst_uart_uart2_txcplt = LST_UART_TX_NOTCPLT;
  
  /* Put message type at the start of the message */
  lst_uart_buffer_tx[0] = LST_BT_MSGTYPE_STATUSOK;
  
  /* Put message end character at the end of the message */
  lst_uart_buffer_tx[1] = LST_BT_MESSAGE_END;
  
  /* Send UART message */
  LST_UART_BT_Send_Bytes(2);
}

/**
 * @brief Sends an UART BT StatusError message
 */
void LST_BT_Send_StatusError(uint8_t *error_msg, uint8_t error_msg_len) {
  lst_bt_send_status_flag = 0;
  
  /* Wait until UART2 is ready */
  while (lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT) {
  }
  lst_uart_uart2_txcplt = LST_UART_TX_NOTCPLT;
  
  /* Put message type at the start of the message */
  lst_uart_buffer_tx[0] = LST_BT_MSGTYPE_STATUSERROR;
  
  /* Copy source buffer to TX buffer */
  if (error_msg_len > 0) {
    memoryCopy((uint8_t *) &lst_uart_buffer_tx[1], (uint8_t *) &error_msg[0],
        error_msg_len);
  }
  
  /* Put message end character at the end of the message */
  lst_uart_buffer_tx[1 + error_msg_len] = LST_BT_MESSAGE_END;
  
  /* Send UART message */
  LST_UART_BT_Send_Bytes(2 + error_msg_len);
}

/**
 * @brief Sends an UART BT StatusRequest message
 */
void LST_BT_Send_StatusRequest() {
  /* Wait until UART2 is ready */
  while (lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT) {
  }
  lst_uart_uart2_txcplt = LST_UART_TX_NOTCPLT;
  
  /* Put message type at the start of the message */
  lst_uart_buffer_tx[0] = LST_BT_MSGTYPE_STATUSREQ;
  
  /* Put message end character at the end of the message */
  lst_uart_buffer_tx[1] = LST_BT_MESSAGE_END;
  
  /* Send UART message */
  LST_UART_BT_Send_Bytes(2);
}

/**
 * @brief Sends an UART BT Variable List message
 */
void LST_BT_Send_VarList() {
  lst_bt_send_varlist_flag = 0;
  
  /* Wait until UART2 is ready */
  while (lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT) {
  }
  lst_uart_uart2_txcplt = LST_UART_TX_NOTCPLT;
  
  /* Put message type at the start of the message */
  lst_uart_buffer_tx[0] = LST_BT_MSGTYPE_VARLIST;
  
  /* Copy source buffer to TX buffer */
  memoryCopy((uint8_t *) &lst_uart_buffer_tx[1], (uint8_t *) &buffer_varlist,
      LST_BT_VARLIST_DATALEN);
  
  /* Put message end character at the end of the message */
  lst_uart_buffer_tx[1 + LST_BT_VARLIST_DATALEN] = LST_BT_MESSAGE_END;
  
  /* Send UART message */
  LST_UART_BT_Send_Bytes(2 + LST_BT_VARLIST_DATALEN);
}

/**
 * @brief Sends an UART BT Variable Values message
 */
void LST_BT_Send_VarValues() {
  /* Wait until UART2 is ready */
  while (lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT) {
  }
  
  lst_uart_uart2_txcplt = LST_UART_TX_NOTCPLT;

  /* Put message type at the start of the message */
  lst_uart_buffer_tx[0] = LST_BT_MSGTYPE_VARVALUES;
  
  /* Copy source buffer to TX buffer */
  memoryCopy((uint8_t *) &lst_uart_buffer_tx[1], (uint8_t *) &lst_spi_master1_rx,
      LST_SPI_BUFFER1_SIZE);
  
  /* Put message end character at the end of the message */
  lst_uart_buffer_tx[1 + LST_SPI_BUFFER1_SIZE] = LST_BT_MESSAGE_END;
  
  /* Send UART message */
  LST_UART_BT_Send_Bytes(2 + LST_SPI_BUFFER1_SIZE);
}

/**
 * @brief Error handler for the BT module
 */
void LST_BT_ErrorHandler(){
  /* If the UART2 Rx request didn't return HAL_OK, repeat request */
  if(lst_uart_uart2_rx_status != HAL_OK){
    LST_UART_Receive_Byte_UART2();
  }
}
