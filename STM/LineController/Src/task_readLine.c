/*
 * task_readLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "task_readLine.h"

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
	spi_write_ledSegment(segment_id);

	// Wait for SPI transaction complete flag
	while (!spi_ledDriver_writeCompleteFlag)
	{

		// Wait

	}

	spi_reset_ledDriver_writeCompleteFlag();

	// Latch LED drivers
	gpio_latch_ledSegment();

	// SET MUXes
	// TODO

	// ADCs: start conversion
	adc_start_conversions();

	// ADCs: wait for conversion
	while (!adc_resultReadyFlag)
	{

		// Wait

	}

	adc_reset_resultReadyFlag();

	// ADCs: read results
	for (uint8_t i=0; i<4; i++)
	{

		values[i * 8 + segment_id] = adc_result[i];

	}

}
