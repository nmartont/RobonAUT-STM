/*
 * lst_utils.c
 *
 *  Created on: 2017. nov. 22.
 *      Author: nmartont
 */
/* Includes ------------------------------------------------------------------*/
#include "lst_utils.h"

/******************************************************************************/
/*                  Utility functions for RobonAUT 2018 Team LST              */
/******************************************************************************/

/**
* @brief Copies len number of bytes from pSrc to pDest
*/
void memoryCopy(uint8_t *pDest, uint8_t *pSrc, uint8_t len){
	uint8_t i;
	// Manually copy the data
	for ( i = 0; i < len; i++ )
	{
		// Copy data from source to destination
		*pDest++ = *pSrc++;
	}
}
