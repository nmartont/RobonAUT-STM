/*
 * lst_dip.h
 *
 *  Created on: 23 Jan 2018
 *      Author: Balazs
 */

/**
 * @author bszakaly
 */
#ifndef LST_DIP_H_
#define LST_DIP_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"

// LST_SETTINGS DIP SWITCH
static const uint8_t READ_AT_STARTUP_ONLY = 0;
static const uint16_t READ_INTERVAL_OSTICK = 100;
static const uint16_t MUX_DELAY_OSTICK = 1;

/**
 * @brief Read all switch values into lst_dip_settings.
 */
void LST_DIP_ReadSwitches();

/**
 * @brief Connect a switch to the digital input.
 * @param position The position of the switch set to be read.
 */
void LST_DIP_SetMux(uint8_t position);

/**
 * @brief Read the MUX output
 * @param position The position of the switch being read.
 */
void LST_DIP_Read(uint8_t position);

/**
 * @var lst_dip_settings Stores the read DIP switch values.
 */
uint8_t lst_dip_settings[8];

/**
 * @brief Maps the MUX addresses to the DIP switch positions
 */
static const uint8_t lst_dip_map[8] = {5, 6, 7, 4, 3, 0, 2, 1};

#endif /* LST_DIP_H_ */
