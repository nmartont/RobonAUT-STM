/*
 * lst_lineControllerNucleo.c
 *
 *  Created on: 31 Oct 2017
 *      Author: Balazs
 */

#include "lst_lineController.h"

void lst_entry_point(void)
{

	lst_main_function();

	//test_timer();

	//test_LedDrivers();

	//debug_driveAddress();

}

void lst_main_function(void)
{

	// TODO:move - init first data transfer
	lst_spiCompleted = 1;
	HAL_GPIO_WritePin(SPI1_DRDY_GPIO_Port, SPI1_DRDY_Pin, 0);

	while (1)
	{

		lst_readLine();

		//lst_evaluate_line();

		lst_sendData();

		lst_displayLine(LST_LINEDISPLAY_RAW_THRESHOLD);

	}

}



