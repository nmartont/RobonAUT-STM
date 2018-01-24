/*
 * lst_dip.c
 *
 *  Created on: 23 Jan 2018
 *      Author: Balazs
 */

#include "lst_dip.h"

void LST_DIP_ReadSwitches()
{

	// Read the 8 switches consecutively
	for (uint8_t i=0; i<8; i++)
	{

		LST_DIP_SetMux(i);

		// Wait for MUX output rise
		vTaskDelay(MUX_DELAY_OSTICK);

		LST_DIP_Read(i);

	}

	// Wait between DIP switch scans
	vTaskDelay(READ_INTERVAL_OSTICK);

}

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
