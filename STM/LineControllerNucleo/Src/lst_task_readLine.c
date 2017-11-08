/*
 * task_readLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "lst_task_readLine.h"

void read_line(void)
{

	for (uint8_t i=0; i<8; i++)
	{

		// Read 4 sensors at a time
		read_segment(i);

	}

}

void read_segment(uint8_t segment_id)
{

	// Write LED drivers
	lst_spi_write_ledSegment(segment_id);

	// Latch LED drivers
	lst_gpio_latch_ledSegment();

	// Set MUXes
	lst_gpio_write_muxes(segment_id);

	// Start ADC conversions
	lst_adc_convert();

	// Read ADC results
	for (uint8_t i=0; i<4; i++)
	{

		lst_tcrt_values[i * 8 + segment_id] = lst_adc_result[i];

	}

}
