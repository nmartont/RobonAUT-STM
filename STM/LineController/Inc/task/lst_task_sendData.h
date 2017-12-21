/*
 * task_sendData.h
 *
 *  Created on: 27 Oct 2017
 *      Author: Balazs
 */

#ifndef TASK_LST_TASK_SENDDATA_H_
#define TASK_LST_TASK_SENDDATA_H_

// Includes

#include <function/lst_function_spi.h>
#include <task/lst_task_readLine.h>
#include <task/lst_task_evaluateLine.h>

// Variables
uint8_t data;

/**
 * \brief
 *  Contains the data to be sent to the mainController.
 */
uint8_t lst_spiData_tx[LST_SPI_SIZE];

/**
 * \brief
 *  Contains the data received from the mainController.
 */
uint8_t lst_spiData_rx[LST_SPI_SIZE];

/**
 * \brief
 *  Set if the SPI TxRx_IT callback returned, thus the inter-STM
 *  communication completed. If not set, the main_function execution
 *  skips the SPI TxRx_IT part.
 */
uint8_t lst_spiCompleted;

// Functions

/**
 * \brief
 *	TODO doxy
 */
void lst_sendData(void);

/**
 * \brief
 *  Initializes the communication with the mainController.
 *  This includes calling an SPI TxRx_IT function and pulling
 *  the DRDY pin high after that.
 */

/**
 * \brief
 *  Initializes the RX and TX buffers to 0 values.
 */
void lst_sendData_initValues(void);

/**
 * \brief
 *  Fills the transmit buffer with the line position and
 *  TCRT sensor output data.
 */
void lst_sendData_fillTxBuffer(void);

/**
 * \brief
 *  Calls the SPI TxRx_IT function and signals the mainController
 *  that it is ready to receive/send.
 */
void lst_sendData_transmitReceive(void);

/**
 * \brief
 *  Called when the inter-STM TxRx operation is complete, enables
 *  next package by setting lst_spiCompleted.
 */
void lst_sendData_TxRxComplete(void);

//TODO:TEST
void lst_spiTest(void);

// TODO:doxy
void lst_sendData_init(void);

#endif /* TASK_LST_TASK_SENDDATA_H_ */
