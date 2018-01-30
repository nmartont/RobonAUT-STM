/*
 * lst_sharp.h
 *
 *  Created on: 30 Jan 2018
 *      Author: Balazs
 */

#ifndef LST_SHARP_H_
#define LST_SHARP_H_

// TODO Refactor according to naming conventions

// Includes

#include "stm32f4xx_hal.h"
#include "lst_adc.h"

/* Sensor characteristic constants */

#define LST_Sharp_ShortRangeTable_Length 25
#define LST_Sharp_LongRangeTable_Length 19

// Type: 0 - short range, 1 - long range
static const uint8_t LST_Sharp_LeftSensorType = 0;
static const uint8_t LST_Sharp_FrontSensorType = 1;
static const uint8_t LST_Sharp_RightSensorType = 0;

// ADC values
static const uint16_t LST_Sharp_ADC_min = 0; // TODO
static const uint16_t LST_Sharp_ADC_max = 1200; // TODO

// ADC correction values
static const float LST_Sharp_Multiplier = 800;

// Sensor value-distance conversion tables: 0:value 1:distance
static const float LST_Sharp_ShortRangeTable_Base
		[2][LST_Sharp_ShortRangeTable_Length] =
{

		{30, 35, 40, 50, 60, 70, 80, 90, 100, 110, 120,
		 130, 140, 150, 160, 180, 200, 220, 240, 260, 280,
		 300, 320, 340, 380},
		{3.05, 2.95, 2.75, 2.33, 2.02, 1.76, 1.55, 1.4, 1.26, 1.15, 1.07,
		 0.98, 0.92, 0.87, 0.81, 0.75, 0.66, 0.6, 0.55, 0.51, 0.47,
		 0.44, 0.4, 0.38, 0.35}

};

static const float LST_Sharp_LongRangeTable_Base
		[2][LST_Sharp_LongRangeTable_Length] =
{

		{150, 200, 250, 300, 350, 400, 450, 500, 550, 600,
		 650, 700, 800, 900, 1000, 1100, 1200, 1300, 1400},
		{2.75, 2.5, 2.25, 2, 1.75, 1.5, 1.375, 1.25, 1.15, 1.05,
		 0.97, 0.9, 0.82, 0.75, 0.68, 0.6, 0.55, 0.5, 0.47}

};

// Corrected tables
uint16_t LST_Sharp_ShortRangeTable[2][25];
uint16_t LST_Sharp_LongRangeTable[2][19];

// Functions
uint16_t LST_Sharp_GetLeftDistance();
uint16_t LST_Sharp_GetFrontDistance();
uint16_t LST_Sharp_GetRightDistance();

uint16_t LST_Sharp_GetRawLeftDistance();
uint16_t LST_Sharp_GetRawFrontDistance();
uint16_t LST_Sharp_GetRawRightDistance();

// TODO not implemented yet
uint16_t LST_Sharp_GetValueFromShortDistance(uint16_t distance);
// TODO not implemented yet
uint16_t LST_Sharp_GetValueFromLongDistance(uint16_t distance);

uint16_t LST_Sharp_ConvertLongValueToDistance(uint16_t value);
uint16_t LST_Sharp_ConvertShortValueToDistance(uint16_t value);

void LST_Sharp_Init();

#endif /* LST_SHARP_H_ */
