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

// External variables

extern uint8_t lst_tcrt_values[32];

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
