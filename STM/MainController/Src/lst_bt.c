/*
 * lst_bt.c
 *
 *  Created on: 2017. nov. 17.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_bt.h"

/* Defines -------------------------------------------------------------------*/
#ifdef LST_CONFIG_LINECONTROLLER_VERBOSE_DATA
#define LST_BT_VARLIST_FASTLAP_DATALEN  250
#define LST_BT_VARLIST_OBSTACLE_DATALEN 298
#else
#define LST_BT_VARLIST_FASTLAP_DATALEN  86
#define LST_BT_VARLIST_OBSTACLE_DATALEN 86
#endif

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

/* Diagnostic control variables */
uint8_t lst_bt_diag_mode = LST_BT_DIAG_MODE_FASTLAP;

#ifdef LST_CONFIG_LINECONTROLLER_VERBOSE_DATA
static const uint8_t buffer_varlist_fastlap[LST_BT_VARLIST_FASTLAP_DATALEN] = {
    0x02, 'S', 'p', LST_BT_VARTYPE_INT16,      // Speed from encoder
    0x01, 'P', LST_BT_VARTYPE_UINT16, // Steering P
    0x01, 'D', LST_BT_VARTYPE_UINT16, // Steering D
    0x01, 'S', LST_BT_VARTYPE_INT16,  // Steering command
    0x01, 'M', LST_BT_VARTYPE_INT16,  // Motor command
    0x03, 'L', 'n', 'o', LST_BT_VARTYPE_UINT8, // Line number
    0x04, 'M', 'o', 'd', 'e', LST_BT_VARTYPE_UINT8, // Fast lap mode
    0x02, 'G', 'x',  LST_BT_VARTYPE_INT16,      // GyroX
    0x02, 'G', 'y', LST_BT_VARTYPE_INT16,      // GyroY
    0x02, 'G', 'z', LST_BT_VARTYPE_INT16,      // GyroZ
    0x02, 'A', 'x', LST_BT_VARTYPE_INT16,      // AccelX
    0x02, 'A', 'y', LST_BT_VARTYPE_INT16,      // AccelY
    0x02, 'A', 'z', LST_BT_VARTYPE_INT16,      // AccelZ
    0x02, 'S', '1', LST_BT_VARTYPE_UINT16,     // Sharp1
    0x02, 'S', '2', LST_BT_VARTYPE_UINT16,     // Sharp2
    0x02, 'S', '3', LST_BT_VARTYPE_UINT16,     // Sharp3
		0x04, 'D', 'S', 'T', 'M', LST_BT_VARTYPE_UINT16, // Distance MSBS
		0x04, 'D', 'S', 'T', 'L', LST_BT_VARTYPE_UINT16, // Distance LSBS
    0x02, 'F', 'F', LST_BT_VARTYPE_UINT8, // 0xFF control byte
    0x01, 'L', LST_BT_VARTYPE_UINT8,      // Line number from SPI
    0x02, 'L', '1', LST_BT_VARTYPE_INT16,// Line position
    0x02, 'L', '2', LST_BT_VARTYPE_UINT16,// Line position repeated
    0x03, 'V', '0', '0', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '1', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '2', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '3', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '4', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '5', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '6', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '7', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '8', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '9', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '0', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '1', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '2', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '3', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '4', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '5', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '6', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '7', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '8', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '9', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '0', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '1', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '2', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '3', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '4', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '5', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '6', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '7', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '8', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '9', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '3', '0', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '3', '1', LST_BT_VARTYPE_UINT8,
};

static const uint8_t buffer_varlist_obstacle[LST_BT_VARLIST_OBSTACLE_DATALEN] = {
    0x02, 'S', 'p', LST_BT_VARTYPE_INT16,      // Speed from encoder
    0x01, 'P', LST_BT_VARTYPE_UINT16, // Steering P
    0x01, 'D', LST_BT_VARTYPE_UINT16, // Steering D
    0x01, 'S', LST_BT_VARTYPE_INT16,  // Steering command
    0x01, 'M', LST_BT_VARTYPE_INT16,  // Motor command
    0x03, 'L', 'n', 'o', LST_BT_VARTYPE_UINT8, // Line number
		0x04, 'o', 'M', 'O', 'D', LST_BT_VARTYPE_UINT8, // Obstacle mode
		0x04, 'o', 'L', 'A', 'P', LST_BT_VARTYPE_UINT8, // Obstacle lap mode
		0x04, 'o', 'S', 'R', 'C', LST_BT_VARTYPE_UINT8, // Obstacle search mode
		0x04, 'o', 'C', 'O', 'R', LST_BT_VARTYPE_UINT8,
		0x04, 'o', 'T', 'R', 'A', LST_BT_VARTYPE_UINT8,
		0x04, 'o', 'D', 'R', 'O', LST_BT_VARTYPE_UINT8,
		0x04, 'o', 'C', 'O', 'N', LST_BT_VARTYPE_UINT8,
		0x04, 'o', 'R', 'O', 'U', LST_BT_VARTYPE_UINT8,
		0x04, 'o', 'B', 'A', 'R', LST_BT_VARTYPE_UINT8,
    0x02, 'G', 'x',  LST_BT_VARTYPE_INT16,      // GyroX
    0x02, 'G', 'y', LST_BT_VARTYPE_INT16,      // GyroY
    0x02, 'G', 'z', LST_BT_VARTYPE_INT16,      // GyroZ
    0x02, 'A', 'x', LST_BT_VARTYPE_INT16,      // AccelX
    0x02, 'A', 'y', LST_BT_VARTYPE_INT16,      // AccelY
    0x02, 'A', 'z', LST_BT_VARTYPE_INT16,      // AccelZ
    0x02, 'S', '1', LST_BT_VARTYPE_UINT16,     // Sharp1
    0x02, 'S', '2', LST_BT_VARTYPE_UINT16,     // Sharp2
    0x02, 'S', '3', LST_BT_VARTYPE_UINT16,     // Sharp3
		0x04, 'D', 'S', 'T', 'M', LST_BT_VARTYPE_INT16, // Distance MSBS
		0x04, 'D', 'S', 'T', 'L', LST_BT_VARTYPE_UINT16, // Distance LSBS
    0x02, 'F', 'F', LST_BT_VARTYPE_UINT8, // 0xFF control byte
    0x01, 'L', LST_BT_VARTYPE_UINT8,      // Line number from SPI
    0x02, 'L', '1', LST_BT_VARTYPE_UINT16,// Line position
    0x02, 'L', '2', LST_BT_VARTYPE_UINT16,// Line position repeated
    0x03, 'V', '0', '0', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '1', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '2', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '3', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '4', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '5', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '6', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '7', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '8', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '0', '9', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '0', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '1', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '2', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '3', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '4', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '5', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '6', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '7', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '8', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '1', '9', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '0', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '1', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '2', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '3', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '4', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '5', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '6', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '7', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '8', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '2', '9', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '3', '0', LST_BT_VARTYPE_UINT8,
    0x03, 'V', '3', '1', LST_BT_VARTYPE_UINT8
};
#else
static const uint8_t buffer_varlist_fastlap[LST_BT_VARLIST_FASTLAP_DATALEN] = {
    0x02, 'S', 'p', LST_BT_VARTYPE_INT16,      // Speed from encoder
    0x01, 'P', LST_BT_VARTYPE_UINT16, // Steering P
    0x01, 'D', LST_BT_VARTYPE_UINT16, // Steering D
    0x01, 'S', LST_BT_VARTYPE_INT16,  // Steering command
    0x01, 'M', LST_BT_VARTYPE_INT16,  // Motor command
    0x03, 'L', 'n', 'o', LST_BT_VARTYPE_UINT8, // Line number
    0x04, 'M', 'o', 'd', 'e', LST_BT_VARTYPE_UINT8, // Fast lap mode
    0x02, 'G', 'x',  LST_BT_VARTYPE_INT16,      // GyroX
    0x02, 'G', 'y', LST_BT_VARTYPE_INT16,      // GyroY
    0x02, 'G', 'z', LST_BT_VARTYPE_INT16,      // GyroZ
    0x02, 'A', 'x', LST_BT_VARTYPE_INT16,      // AccelX
    0x02, 'A', 'y', LST_BT_VARTYPE_INT16,      // AccelY
    0x02, 'A', 'z', LST_BT_VARTYPE_INT16,      // AccelZ
    0x02, 'S', '1', LST_BT_VARTYPE_UINT16,     // Sharp1
    0x02, 'S', '2', LST_BT_VARTYPE_UINT16,     // Sharp2
    0x02, 'S', '3', LST_BT_VARTYPE_UINT16,     // Sharp3
		0x04, 'D', 'S', 'T', 'M', LST_BT_VARTYPE_INT16, // Distance MSBS
		0x04, 'D', 'S', 'T', 'L', LST_BT_VARTYPE_UINT16, // Distance LSBS
    0x02, 'F', 'F', LST_BT_VARTYPE_UINT8, // 0xFF control byte
    0x01, 'L', LST_BT_VARTYPE_UINT8,      // Line number from SPI
    0x02, 'L', '1', LST_BT_VARTYPE_UINT16,// Line position
    0x02, 'L', '2', LST_BT_VARTYPE_UINT16,// Line position repeated
    };
static const uint8_t buffer_varlist_obstacle[LST_BT_VARLIST_OBSTACLE_DATALEN] = {
    0x02, 'S', 'p', LST_BT_VARTYPE_INT16,      // Speed from encoder
    0x01, 'P', LST_BT_VARTYPE_UINT16, // Steering P
    0x01, 'D', LST_BT_VARTYPE_UINT16, // Steering D
    0x01, 'S', LST_BT_VARTYPE_INT16,  // Steering command
    0x01, 'M', LST_BT_VARTYPE_INT16,  // Motor command
    0x03, 'L', 'n', 'o', LST_BT_VARTYPE_UINT8, // Line number
    0x04, 'M', 'o', 'd', 'e', LST_BT_VARTYPE_UINT8, // Obstacle lap mode
    0x02, 'G', 'x',  LST_BT_VARTYPE_INT16,      // GyroX
    0x02, 'G', 'y', LST_BT_VARTYPE_INT16,      // GyroY
    0x02, 'G', 'z', LST_BT_VARTYPE_INT16,      // GyroZ
    0x02, 'A', 'x', LST_BT_VARTYPE_INT16,      // AccelX
    0x02, 'A', 'y', LST_BT_VARTYPE_INT16,      // AccelY
    0x02, 'A', 'z', LST_BT_VARTYPE_INT16,      // AccelZ
    0x02, 'S', '1', LST_BT_VARTYPE_UINT16,     // Sharp1
    0x02, 'S', '2', LST_BT_VARTYPE_UINT16,     // Sharp2
    0x02, 'S', '3', LST_BT_VARTYPE_UINT16,     // Sharp3
		0x04, 'D', 'S', 'T', 'M', LST_BT_VARTYPE_INT16, // Distance MSBS
		0x04, 'D', 'S', 'T', 'L', LST_BT_VARTYPE_UINT16, // Distance LSBS
    0x02, 'F', 'F', LST_BT_VARTYPE_UINT8, // 0xFF control byte
    0x01, 'L', LST_BT_VARTYPE_UINT8,      // Line number from SPI
    0x02, 'L', '1', LST_BT_VARTYPE_UINT16,// Line position
    0x02, 'L', '2', LST_BT_VARTYPE_UINT16,// Line position repeated
    };
#endif

/* Function prototypes ------------------------------------------------------ */
inline static void LST_BT_Gamepad_Default_Values();
inline static void LST_BT_Process_BT_Message();
inline static void LST_BT_Process_Config_Message();
static void LST_BT_Error_Handler();

/******************************************************************************/
/*                 BlueTooth handling for RobonAUT 2018 Team LST              */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the BT part of the software
 */
void LST_BT_Init() {
  LST_BT_Gamepad_Default_Values();
}

inline static void LST_BT_Gamepad_Default_Values() {
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
    LST_BT_Process_BT_Message();
    /* Reset counter */
    buffer_rx_cntr = 0;
  }
  /* Check for message end byte for config messages from the BT module */
  else if (lst_uart_buffer_uart2[0] == LST_BT_CONF_MSG_END2) { // Last byte of the message is '\n'
    if (buffer_rx_cntr > 0) {
      if (buffer_rx[buffer_rx_cntr - 1] == LST_BT_CONF_MSG_END1) { // Second to last byte of the message is '\r'
        LST_BT_Process_Config_Message();
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
inline static void LST_BT_Process_BT_Message() {
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
inline static void LST_BT_Process_Config_Message() {
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
    LST_BT_Gamepad_Default_Values();
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
    LST_Utils_Memory_Copy((uint16_t *) &lst_uart_buffer_tx[1], (uint16_t *) &error_msg[0],
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
  
  /* Handle FASTLAP/OBSTACLE modes */
  if(lst_bt_diag_mode == LST_BT_DIAG_MODE_FASTLAP){
    /* Copy source buffer to TX buffer */
    LST_Utils_Memory_Copy((uint16_t *) &lst_uart_buffer_tx[1], (uint16_t *) &buffer_varlist_fastlap,
        LST_BT_VARLIST_FASTLAP_DATALEN);

    /* Put message end character at the end of the message */
    lst_uart_buffer_tx[1 + LST_BT_VARLIST_FASTLAP_DATALEN] = LST_BT_MESSAGE_END;

    /* Send UART message */
    LST_UART_BT_Send_Bytes(2 + LST_BT_VARLIST_FASTLAP_DATALEN);
  }else if(lst_bt_diag_mode == LST_BT_DIAG_MODE_OBSTACLE){
    /* Copy source buffer to TX buffer */
    LST_Utils_Memory_Copy((uint16_t *) &lst_uart_buffer_tx[1], (uint16_t *) &buffer_varlist_obstacle,
        LST_BT_VARLIST_OBSTACLE_DATALEN);

    /* Put message end character at the end of the message */
    lst_uart_buffer_tx[1 + LST_BT_VARLIST_OBSTACLE_DATALEN] = LST_BT_MESSAGE_END;

    /* Send UART message */
    LST_UART_BT_Send_Bytes(2 + LST_BT_VARLIST_OBSTACLE_DATALEN);
  }
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
  
  /* Send P, D, Motor, Steering */
  int16_t temp = lst_encoder_speed;
  int16_t distance_msb = (lst_encoder_distance_um >> 16);
  int16_t distance_lsb = (lst_encoder_distance_um & 0xffff);

  // BT 0xff fix
  if (distance_msb == -1) distance_msb = -10000;
  if (distance_lsb == -1) distance_lsb = -2;
  if(temp<0){
    // Speed can't be a small negative number because of BT 0xFF bit
    temp = temp*-1 + 10000;
  }
  lst_uart_buffer_tx[1]=temp & 0xff;
  lst_uart_buffer_tx[2]=(temp >> 8);
  lst_uart_buffer_tx[3]=lst_control_steeringP & 0xff;
  lst_uart_buffer_tx[4]=(lst_control_steeringP >> 8);
  lst_uart_buffer_tx[5]=lst_control_steeringD & 0xff;
  lst_uart_buffer_tx[6]=(lst_control_steeringD >> 8);
  lst_uart_buffer_tx[7]=lst_control_steering & 0xff;
  lst_uart_buffer_tx[8]=(lst_control_steering >> 8);
  lst_uart_buffer_tx[9]=lst_control_motor & 0xff;
  lst_uart_buffer_tx[10]=(lst_control_motor >> 8);
  lst_uart_buffer_tx[11]=lst_control_line_no;
  /* Handle FASTLAP/OBSTACLE modes */
  uint8_t extra_bytes = 0;
  if(lst_bt_diag_mode == LST_BT_DIAG_MODE_FASTLAP){
    extra_bytes = 19;
    lst_uart_buffer_tx[12]=lst_fast_q1_mode;
    lst_uart_buffer_tx[13]=lst_i2c_master1_rx[0];
    lst_uart_buffer_tx[14]=lst_i2c_master1_rx[1];
    lst_uart_buffer_tx[15]=lst_i2c_master1_rx[2];
    lst_uart_buffer_tx[16]=lst_i2c_master1_rx[3];
    lst_uart_buffer_tx[17]=lst_i2c_master1_rx[4];
    lst_uart_buffer_tx[18]=lst_i2c_master1_rx[5];
    lst_uart_buffer_tx[19]=lst_i2c_master1_rx[6];
    lst_uart_buffer_tx[20]=lst_i2c_master1_rx[7];
    lst_uart_buffer_tx[21]=lst_i2c_master1_rx[8];
    lst_uart_buffer_tx[22]=lst_i2c_master1_rx[9];
    lst_uart_buffer_tx[23]=lst_i2c_master1_rx[10];
    lst_uart_buffer_tx[24]=lst_i2c_master1_rx[11];
    lst_uart_buffer_tx[25]=lst_adc_sharp_result[0] & 0xff;
    lst_uart_buffer_tx[26]=(lst_adc_sharp_result[0] >> 8);
    lst_uart_buffer_tx[27]=lst_adc_sharp_result[1] & 0xff;
    lst_uart_buffer_tx[28]=(lst_adc_sharp_result[1] >> 8);
    lst_uart_buffer_tx[29]=lst_adc_sharp_result[2] & 0xff;
    lst_uart_buffer_tx[30]=(lst_adc_sharp_result[2] >> 8);
    lst_uart_buffer_tx[31]=(distance_msb & 0xff);
		lst_uart_buffer_tx[32]=(distance_msb >> 8);
		lst_uart_buffer_tx[33]=(distance_lsb & 0xff);
		lst_uart_buffer_tx[34]=(distance_lsb >> 8);
  }else if(lst_bt_diag_mode == LST_BT_DIAG_MODE_OBSTACLE){
    extra_bytes = 31;
    lst_uart_buffer_tx[12]=lst_obs_mode;
    lst_uart_buffer_tx[13]=lst_obs_lap_mode;
    lst_uart_buffer_tx[14]=lst_obs_search_mode;
    lst_uart_buffer_tx[15]=lst_obs_corner_stage;
    lst_uart_buffer_tx[16]=lst_obs_train_stage;
    lst_uart_buffer_tx[17]=lst_obs_drone_stage;
    lst_uart_buffer_tx[18]=lst_obs_convoy_stage;
    lst_uart_buffer_tx[19]=lst_obs_roundabout_stage;
    lst_uart_buffer_tx[20]=lst_obs_barrel_stage;
    lst_uart_buffer_tx[21]=lst_i2c_master1_rx[0];
    lst_uart_buffer_tx[22]=lst_i2c_master1_rx[1];
    lst_uart_buffer_tx[23]=lst_i2c_master1_rx[2];
    lst_uart_buffer_tx[24]=lst_i2c_master1_rx[3];
    lst_uart_buffer_tx[25]=lst_i2c_master1_rx[4];
    lst_uart_buffer_tx[26]=lst_i2c_master1_rx[5];
    lst_uart_buffer_tx[27]=lst_i2c_master1_rx[6];
    lst_uart_buffer_tx[28]=lst_i2c_master1_rx[7];
    lst_uart_buffer_tx[29]=lst_i2c_master1_rx[8];
    lst_uart_buffer_tx[30]=lst_i2c_master1_rx[9];
    lst_uart_buffer_tx[31]=lst_i2c_master1_rx[10];
    lst_uart_buffer_tx[32]=lst_i2c_master1_rx[11];
    /*
    lst_uart_buffer_tx[25]=lst_adc_sharp_result[0] & 0xff;
    lst_uart_buffer_tx[26]=(lst_adc_sharp_result[0] >> 8);
    lst_uart_buffer_tx[27]=lst_adc_sharp_result[1] & 0xff;
    lst_uart_buffer_tx[28]=(lst_adc_sharp_result[1] >> 8);
    lst_uart_buffer_tx[29]=lst_adc_sharp_result[2] & 0xff;
    lst_uart_buffer_tx[30]=(lst_adc_sharp_result[2] >> 8);
     */
    lst_uart_buffer_tx[33]=(LST_Sharp_GetLeftDistance_mm() & 0xff);
    lst_uart_buffer_tx[34]=(LST_Sharp_GetLeftDistance_mm() >> 8);
    lst_uart_buffer_tx[35]=LST_Sharp_GetFrontDistance_mm() & 0xff;
    lst_uart_buffer_tx[36]=(LST_Sharp_GetFrontDistance_mm() >> 8);
    lst_uart_buffer_tx[37]=LST_Sharp_GetRightDistance_mm() & 0xff;
    lst_uart_buffer_tx[38]=(LST_Sharp_GetRightDistance_mm() >> 8);
    lst_uart_buffer_tx[39]=(distance_msb & 0xff);
		lst_uart_buffer_tx[40]=(distance_msb >> 8);
		lst_uart_buffer_tx[41]=(distance_lsb & 0xff);
		lst_uart_buffer_tx[42]=(distance_lsb >> 8);
  }

  /* Copy source buffer to TX buffer */
  LST_Utils_Memory_Copy((uint16_t *) &lst_uart_buffer_tx[12 + extra_bytes], (uint16_t *) &lst_spi_master1_rx,
      LST_SPI_BUFFER1_SIZE);
  
  /* Put message end character at the end of the message */
  lst_uart_buffer_tx[12 + extra_bytes + LST_SPI_BUFFER1_SIZE] = LST_BT_MESSAGE_END;
  
  /* Send UART message */
  LST_UART_BT_Send_Bytes(13 + extra_bytes + LST_SPI_BUFFER1_SIZE);
}

/**
 * @brief BT request handler task
 */
void LST_BT_RequestHandler(){
  /* Error handling for the BT module */
  LST_BT_Error_Handler();

  /* Send computer responses if there is a request */
  if (lst_bt_send_status_flag)
    LST_BT_Send_StatusOk();
  if (lst_bt_send_varlist_flag)
    LST_BT_Send_VarList();
}

/**
 * @brief Error handler for the BT module
 */
static void LST_BT_Error_Handler(){
  /* If the UART2 Rx request didn't return HAL_OK, repeat request */
  if(lst_uart_uart2_rx_status != HAL_OK){
    LST_UART_Receive_Byte_UART2();
  }
}
