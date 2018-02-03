/*
 * lst_infra.c
 *
 *  Created on: 2018. jan. 23.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_infra.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private functions -------------------------------------------------------- */

// TODO:TEST 2018. 01. 25.
uint32_t LST_test_counter = 0;

/******************************************************************************/
/*                   Infra handler for RobonAUT 2018 Team LST                 */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Infra module
 */
void LST_Infra_Init() {

}

/**
 * @brief Function that handles the timer capture callback
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim8) {

  }
}
