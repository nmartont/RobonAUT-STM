/*
 * task_readLine.h
 *
 *  Created on: 25 Oct 2017
 *      Author: Balazs
 */

#include "function_adc.h"
#include "function_gpio.h"
#include "function_spi.h"

uint8_t values[32];

void read_line(void);
void read_segment(uint8_t segment_id);
