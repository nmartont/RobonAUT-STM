/*
 * lst_dip.c
 *
 *  Created on: 23 Jan 2018
 *      Author: Balazs
 */

#include "lst_dip.h"

void LST_DIP_SetMux(uint8_t position)
{

	HAL_GPIO_WritePin(ADDR0_GPIO_Port, ADDR0_Pin, (position & 1));
	HAL_GPIO_WritePin(ADDR1_GPIO_Port, ADDR1_Pin, (position & 2));
	HAL_GPIO_WritePin(ADDR2_GPIO_Port, ADDR2_Pin, (position & 4));

}

void LST_DIP_Read(uint8_t position)
{

	lst_dip_settings[lst_dip_map[position]] =
			HAL_GPIO_ReadPin(DIP_GPIO_Port, DIP_Pin);

}
