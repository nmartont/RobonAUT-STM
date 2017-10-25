/*
 * task_readLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "task_readLine.h"

uint8_t * read_line(void)
{

	for (uint8_t i=0; i<8; i++)
	{

		// Read 4 sensors at a time
		read_segment(i);

	}

	return values;

}

void read_segment(uint8_t segment_id)
{

	// Set LED drivers
	// TODO

	// SET MUXes
	// TODO

	// ADCs: start conversion
	adc_start_conversions();

	// ADCs: wait for conversion
	while (!adc_resultReadyFlag)
	{

		// Wait

	}

	// ADCs: read results
	for (uint8_t i=0; i<4; i++)
	{

		values[i * 8 + segment_id] = adc_result[i];

	}

}
