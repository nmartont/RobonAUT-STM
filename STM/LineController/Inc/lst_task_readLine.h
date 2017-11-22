/*
 * task_readLine.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#ifndef LST_TASK_READLINE_H_
#define LST_TASK_READLINE_H_

// Includes

#include "lst_function_adc.h"
#include "lst_function_gpio.h"
#include "lst_function_spi.h"

// Constants

static const uint8_t lst_ledDriver_control[8][4] =
		{
				/*
				{4, 64, 4, 64},
				{2, 32, 2, 32},
				{1, 16, 1, 16},
				{8, 128, 8, 128},
				{16, 1, 16, 1},
				{128, 8, 128, 8},
				{32, 2, 32, 2},
				{64, 4, 64, 4}
				*/

				/*
				// Reversed byte and bit order
				{4, 64, 4, 64},
				{2, 32, 2, 32},
				{8, 128, 8, 128},
				{1, 16, 1, 16},
				{128, 8, 128, 8},
				{16, 1, 16, 1},
				{32, 2, 32, 2},
				{64, 4, 64, 4}
				*/
				{2, 32, 2, 32},
				{4, 64, 4, 64},
				{8, 128, 8, 128},
				{1, 16, 1, 16},
				{128, 8, 128, 8},
				{16, 1, 16, 1},
				{64, 4, 64, 4},
				{32, 2, 32, 2}

		};
/*
 * @Description
 * 	Stores the bytes to be shifted to the Led Driver in
 * 	the eight consecutive TCRT read cycles. In each cycle,
 * 	four bytes are shifted to control all 32 LEDs.
 */

static const uint8_t lst_result_control[8][4] =
	{
			{2, 14, 18, 30},
			{1, 13, 17, 29},
			{0, 12, 16, 28},
			{3, 15, 19, 31},
			{4, 8, 20, 24},
			{7, 11, 23, 27},
			{5, 9, 21, 25},
			{6, 10, 22, 26}
	};
/*
 * @Description
 * 	Stores the TCRT position data; shows where the ADC values
 * 	have to be read into in the consecutive TCRT read cycles.
 */

static const uint8_t lst_mux_control[8] =
	{0, 1, 2, 3, 4, 5, 6, 7};
/*
 * @Description
 * 	Stores the MUX addresses applied in the eight consecutive
 * 	TCRT read cycles.
 */

// Local variables

uint8_t lst_tcrt_values[32];
/*
 * @Description
 * 	Stores the sensor output values converted by the ADCs.
 */

// Function declarations

void read_line(void);
/*
 * @Description
 *	Calls read_segment on the 8 LED segments. Acquired values
 *	are stored in values[32].
 */

void read_segment(uint8_t segment_id);
/*
 * @Description
 *	Acquires the 4 output values of a LED segment.
 */

#endif /* LST_TASK_READLINE_H_ */
