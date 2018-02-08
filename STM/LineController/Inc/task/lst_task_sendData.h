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

// Watchdog and reset variables, constants TODO document and TEST
uint16_t lst_spiWatchdog;
#define LST_SPI_WATCHDOG_TIMEOUT  40 //50
#define LST_SPI_INIT_DELAY_MS			10

// Functions

/**
 * \brief Handles sending the line data to the mainController.
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

/**
 * \brief Initializes DRDY GPIO output and busy flag.
 */
void lst_sendData_init(void);

#endif /* TASK_LST_TASK_SENDDATA_H_ */
