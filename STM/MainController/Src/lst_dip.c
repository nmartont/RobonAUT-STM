/*
 * lst_dip.c
 *
 *  Created on: 23 Jan 2018
 *      Author: Balazs
 */

#include "lst_dip.h"

uint8_t lst_dip_read_once = 0;

void LST_DIP_SetMux(uint8_t position)
{

	HAL_GPIO_WritePin(ADDR0_GPIO_Port, ADDR0_Pin, (position & 1));
	HAL_GPIO_WritePin(ADDR1_GPIO_Port, ADDR1_Pin, (position & 2));
	HAL_GPIO_WritePin(ADDR2_GPIO_Port, ADDR2_Pin, (position & 4));

}

void LST_DIP_Read(uint8_t position)
{

	lst_dip_settings[lst_dip_map[position]] =
			!HAL_GPIO_ReadPin(DIP_GPIO_Port, DIP_Pin);

}

void LST_DIP_ReadAll(){
  lst_dip_read_once = 1;

  for (uint8_t i=0; i<8; i++){
    LST_DIP_SetMux(i);

    /* 200 ticks delay for the MUX */
    volatile uint16_t vol_cntr = 0;
    for(vol_cntr=0; vol_cntr<200; ){
      vol_cntr++;
    }

    LST_DIP_Read(i);

  }
}
