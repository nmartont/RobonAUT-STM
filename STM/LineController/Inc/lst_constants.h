/*
 * lst_constants.h
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_CONSTANTS_H_
#define LST_CONSTANTS_H_

// Test constants

/**
 * \warning
 * 	Define if code is run on NUCLEO, comment out if run on STM32F3
 */
//#define LST_NUCLEO_TEST
#define LST_NUCLEO_LINECONTROLLER

/**
 * \brief
 * 	Hardware Abstraction Layer Library of the appropriate controller family
 */
#ifdef LST_NUCLEO_TEST
	#include "stm32f4xx_hal.h"
#else
	#include "stm32f3xx_hal.h"
#endif

// Line evaluation algorithm selection

// TODO:expand explanation
/**
 * \brief
 * 	Line detection algorithm selection.
 */
#define LST_LINEEVAL_BASIC
//#define LST_LINEEVAL_ADVANCED

// Line evaluation parameters - basic

/**
 * \brief
 *  Multiplier value used in simple summing line position calculation.
 */
#define LST_LINEEVAL_BASIC_MULTIPLIER 2048

/**
 * \brief
 * 	If line sensing is displayed based on raw sensor input data
 * 	(not based on the evaluation algorithm result) this value
 * 	determines if a feedback LED is to be lit or not.
 */
#define LST_LINEDISPLAY_RAW_THRESHOLD 140

#define LST_SIMPLEEVAL_LOW_THRESHOLD 40
#define LST_SIMPLEEVAL_HIGH_THRESHOLD 140

// STM constants

/**
 * @Warning
 * 	Timer functions only work correctly below TIM1 frequencies of 64MHz
 */
#define LST_F_TIM1 64000000UL


// Hardware signal hold constants

/**
 * \brief
 * 	Number of TIM1 clock cycles to wait while the LATCH pin
 * 	of the TLC5928 LED driver is held high to enable output
 */
#define LST_TLC5928_LATCH_HOLD_CLK 2

/**
 * \brief The waiting time after MUX addressing in CPU clock cycles.
 */
#define LST_CD74HC4051_OUTRISE_HOLD_CLK 100

// Other constants

/**
 * \brief The size of the inter-STM SPI package.
 */
#define LST_SPI_SIZE 38

/**
 * \brief The size of the inter-STM SPI transmit buffer in bytes.
 * \warning Cannot be higher than LST_SPI_SIZE.
 */
#define LST_SPI_TXBUF_SIZE 38

/**
 * \brief
 *  The first byte of the sensor output data in the inter-STM SPI
 *  transmit buffer.
 * \warning
 *  Cannot be higher than LST_SPI_SIZE.
 */
#define LST_SPI_TXBUF_VALUE_START 6

/**
 * \brief
 *  The size of the inter-STM SPI receive buffer in bytes.
 */
#define LST_SPI_RXBUF_SIZE 1

// TODO:needed?
#define LST_SENDDATA_MAX 65534

/**
 * \brief Timeout for the TCRT LED driver SPI communication in
 *  milliseconds.
 */
#define LST_SPI_TXRX_TIMEOUT 50 // msec

#endif /* LST_CONSTANTS_H_ */
