/*
 * task_readLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include <task/lst_task_readLine.h>

void lst_readLine(void)
{

	for (uint8_t i=0; i<8; i++)
	{

		// Read 4 sensors at a time
		lst_read_segment(i);

	}

}

void lst_read_segment(uint8_t segment_id)
{

	// Write LED (TCRT) drivers
	lst_spi_write_ledSegment(segment_id);

	// Latch LED (TCRT) drivers
	lst_gpio_latch_ledSegment();

	// Set MUXes
	lst_gpio_write_muxes(segment_id);

	// TIMER OK: 60us CHECKED
	//HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 1);

	// Wait for MUX and LED rise time
	lst_timer1_delay_microSeconds(60);

	//HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 0);

	//HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 1); // WARNING ADC READ TIME MEASURED 4ms!

	// Start ADC conversions
	lst_adc_convert();

	//HAL_GPIO_WritePin(SPI_STM_DRDY_GPIO_Port, SPI_STM_DRDY_Pin, 0);

	// Read ADC results
	for (uint8_t i=0; i<4; i++)
	{

		lst_tcrt_values[lst_result_control[segment_id][i]]
										= lst_adc_result[i];

	}

}

void lst_readLine_init()
{

	// Write 0 to all LED (TCRT) drivers
	lst_spi_clear_ledSegment();

	// Latch LED (TCRT) drivers
	lst_gpio_latch_ledSegment();

}
