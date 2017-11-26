/*
 * task_readLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include <task/lst_task_readLine.h>

void lst_read_line(void)
{

	for (uint8_t i=0; i<8; i++)
	{

		// Read 4 sensors at a time
		lst_read_segment(i);

	}

}

void lst_read_segment(uint8_t segment_id)
{

	// Write LED drivers
	lst_spi_write_ledSegment(segment_id);

	// Latch LED drivers
	lst_gpio_latch_ledSegment();

	// Set MUXes
	lst_gpio_write_muxes(segment_id);

	// Wait for MUX and LED rise time
	lst_timer1_delay_microSeconds(60);

	// Start ADC conversions
	lst_adc_convert();

	// Read ADC results
	for (uint8_t i=0; i<4; i++)
	{

		lst_tcrt_values[lst_result_control[segment_id][i]]
										= lst_adc_result[i];

	}

}
