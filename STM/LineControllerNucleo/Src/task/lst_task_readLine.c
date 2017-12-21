/*
 * task_readLine.c
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include <task/lst_task_readLine.h>

uint8_t lst_tcrt_values_temp[32] = {0x00};

void lst_readLine(void)
{

	for (uint8_t i=0; i<8; i++)
	{

		// Read 4 sensors at a time
		lst_read_segment(i);

	}

	// Gány
	for (uint8_t i=0; i<32; i++)
	{
		// Read 4 sensors at a time
		lst_tcrt_values_temp[i] = lst_tcrt_values[i];
	}

	lst_tcrt_values[0] = lst_tcrt_values_temp[28];
	lst_tcrt_values[1] = lst_tcrt_values_temp[29];
	lst_tcrt_values[2] = lst_tcrt_values_temp[30];
	lst_tcrt_values[3] = lst_tcrt_values_temp[31];
	lst_tcrt_values[4] = lst_tcrt_values_temp[24];
	lst_tcrt_values[5] = lst_tcrt_values_temp[25];
	lst_tcrt_values[6] = lst_tcrt_values_temp[26];
	lst_tcrt_values[7] = lst_tcrt_values_temp[27];

	lst_tcrt_values[24] = lst_tcrt_values_temp[4];
	lst_tcrt_values[25] = lst_tcrt_values_temp[5];
	lst_tcrt_values[26] = lst_tcrt_values_temp[6];
	lst_tcrt_values[27] = lst_tcrt_values_temp[7];
	lst_tcrt_values[28] = lst_tcrt_values_temp[0];
	lst_tcrt_values[29] = lst_tcrt_values_temp[1];
	lst_tcrt_values[30] = lst_tcrt_values_temp[2];
	lst_tcrt_values[31] = lst_tcrt_values_temp[3];
}

void lst_read_segment(uint8_t segment_id)
{

	// Write LED (TCRT) drivers
	lst_spi_write_ledSegment(segment_id);

	// Latch LED (TCRT) drivers
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

void lst_readLine_init()
{

	// Write 0 to all LED (TCRT) drivers
	lst_spi_clear_ledSegment();

	// Latch LED (TCRT) drivers
	lst_gpio_latch_ledSegment();

}
