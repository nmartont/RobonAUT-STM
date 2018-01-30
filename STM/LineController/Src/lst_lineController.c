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

}

void lst_main_function(void)
{

	lst_init_functions();

	while (1)
	{

		// Measure read_line() time on scope !! MEASURED: 35ms !!!!!!!!!!!!!!!!!!

		//HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 1);

		lst_readLine();

		//HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 0);




		lst_evaluate_line();



		// WARNING !!! LINE DATA CYCLE TIME TEST ONLY !!!
		// WARNING !!! LINE DATA CYCLE TIME TEST ONLY !!!
		// WARNING !!! LINE DATA CYCLE TIME TEST ONLY !!!
		// WARNING !!! LINE DATA CYCLE TIME TEST ONLY !!!
		// WARNING !!! LINE DATA CYCLE TIME TEST ONLY !!!

		lst_sendData();

		lst_displayLine(LST_LINEDISPLAY_RAW_THRESHOLD);



	}

}

void lst_init_functions()
{

	lst_sendData_init();

	lst_readLine_init();

}


