/*
 * lst_dip.h
 *
 *  Created on: 23 Jan 2018
 *      Author: Balazs
 */

#ifndef LST_DIP_H_
#define LST_DIP_H_

#include "main.h"
#include "stm32f4xx_hal.h"

void LST_DIP_SetMux(uint8_t position);
void LST_DIP_Read(uint8_t position);

uint8_t lst_dip_settings[8];

/**
 * @brief Maps the MUX addresses to the DIP switch positions
 */
static const uint8_t lst_dip_map[8] = {5, 6, 7, 4, 3, 0, 2, 1};

#endif /* LST_DIP_H_ */
