/*
 * lst_bt.c
 *
 *  Created on: 2017. nov. 17.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_bt.h"

/* Private variables ---------------------------------------------------------*/
// ToDo some of these variables are for testing.
#ifdef LST_CONFIG_LINECONTROLLER_DEBUG_DATA
#define LST_BT_VARLIST_DATALEN 24
uint8_t buffer_varlist[LST_BT_VARLIST_DATALEN] = {
		0x03, 0x41, 0x42, 0x43, 0x00,
		0x03, 0x44, 0x45, 0x46, 0x01,
		0x03, 0x47, 0x48, 0x49, 0x02,
		0x01, 0x50, 0x03,
		0x01, 0x51, 0x04,
		0x01, 0x52, 0x05};
#else
#define LST_BT_VARLIST_DATALEN 24
uint8_t buffer_varlist[LST_BT_VARLIST_DATALEN] = {
		0x03, 0x41, 0x42, 0x43, 0x00,
		0x03, 0x44, 0x45, 0x46, 0x01,
		0x03, 0x47, 0x48, 0x49, 0x02,
		0x01, 0x50, 0x03,
		0x01, 0x51, 0x04,
		0x01, 0x52, 0x05};
#endif
#define LST_BT_VARVALUES_DATALEN 14
uint8_t buffer_vars[LST_BT_VARVALUES_DATALEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/******************************************************************************/
/*                 BlueTooth handling for RobonAUT 2018 Team LST              */
/******************************************************************************/

/* Message sender functions --------------------------------------------------*/
/**
* @brief Sends an UART BT StatusOK message
*/
void LST_BT_Send_StatusOk(){
	/* Wait until other sender functions are not using the BT */
	while(lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT){}
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
void LST_BT_Send_StatusError(uint8_t *error_msg, uint8_t error_msg_len){
	/* Wait until UART2 is ready */
	while(lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT){}
	lst_uart_uart2_txcplt = LST_UART_TX_NOTCPLT;

	/* Put message type at the start of the message */
	lst_uart_buffer_tx[0] = LST_BT_MSGTYPE_STATUSERROR;

	/* Copy source buffer to TX buffer */
	if(error_msg_len > 0){
		memoryCopy((uint8_t *)&lst_uart_buffer_tx[1], (uint8_t *)&error_msg, error_msg_len);
	}

	/* Put message end character at the end of the message */
	lst_uart_buffer_tx[1 + error_msg_len] = LST_BT_MESSAGE_END;

	/* Send UART message */
	LST_UART_BT_Send_Bytes(2 + error_msg_len);
}

/**
* @brief Sends an UART BT StatusRequest message
*/
void LST_BT_Send_StatusRequest(){
	/* Wait until UART2 is ready */
	while(lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT){}
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
void LST_BT_Send_VarList(){
	/* Wait until UART2 is ready */
	while(lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT){}
	lst_uart_uart2_txcplt = LST_UART_TX_NOTCPLT;

	/* Put message type at the start of the message */
	lst_uart_buffer_tx[0] = LST_BT_MSGTYPE_VARLIST;

	/* Copy source buffer to TX buffer */
	memoryCopy((uint8_t *)&lst_uart_buffer_tx[1], (uint8_t *)&buffer_varlist, LST_BT_VARLIST_DATALEN);

	/* Put message end character at the end of the message */
	lst_uart_buffer_tx[1 + LST_BT_VARLIST_DATALEN] = LST_BT_MESSAGE_END;

	/* Send UART message */
	LST_UART_BT_Send_Bytes(2 + LST_BT_VARLIST_DATALEN);
}

/**
* @brief Sends an UART BT Variable Values message
*/
void LST_BT_Send_VarValues(){
	/* Wait until UART2 is ready */
	while(lst_uart_uart2_txcplt == LST_UART_TX_NOTCPLT){}

	/* Increment data */
	uint8_t i = 0;
	for(i = 0; i < LST_BT_VARVALUES_DATALEN; i++){
		buffer_vars[i]++;
		if(buffer_vars[i] == 0xFF){
			buffer_vars[i] = 0x00;
		}
	}

	lst_uart_uart2_txcplt = LST_UART_TX_NOTCPLT;

	/* Put message type at the start of the message */
	lst_uart_buffer_tx[0] = LST_BT_MSGTYPE_VARVALUES;

	/* Copy source buffer to TX buffer */
	memoryCopy((uint8_t *)&lst_uart_buffer_tx[1], (uint8_t *)&buffer_vars, LST_BT_VARVALUES_DATALEN);

	/* Put message end character at the end of the message */
	lst_uart_buffer_tx[1 + LST_BT_VARVALUES_DATALEN] = LST_BT_MESSAGE_END;

	/* Send UART message */
	LST_UART_BT_Send_Bytes(2 + LST_BT_VARVALUES_DATALEN);
}
