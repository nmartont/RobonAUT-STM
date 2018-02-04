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
uint8_t lst_infra_is_available = LST_INFRA_NOT_AVAILABLE;

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



uint8_t LST_Infra_Get_Direction(){
  return LST_INFRA_DIR_LEFT; // ToDo
}

uint8_t LST_Infra_Get_Exit(){
  return LST_INFRA_EXIT_ONE; // ToDo
}
