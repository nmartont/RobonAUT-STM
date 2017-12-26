/*
 * lst_fastlap.c
 *
 *  Created on: 2017. dec. 26.
 *      Author: nmartont
 */

/* Includes ------------------------------------------------------------------*/
#include "lst_fastlap.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*                  Fast lap handler for RobonAUT 2018 Team LST               */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Fast lap module
 */
void LST_Fast_Init() {

}

/**
 * @brief Main handler logic of the fast lap
 */
void LST_Fast_Logic(){
  /* Get line data from LineController */
#ifdef LST_CONFIG_UART_LINE_COM
  LST_UART_ReceiveLineControllerData();
#else
  LST_SPI_ReceiveLineControllerData();
#endif

  /* ToDo ADC conversion, I2C, other sensor data */
  // ToDo normalize speed by LST_TASK_FASTLAP_REPEAT_TICKS
  /* Calculate speed from encoder */
  LST_TIM_CalculateSpeed();

  /* Wait for the end of the LineController transaction */
#ifdef LST_CONFIG_UART_LINE_COM
  LST_UART_WaitForLineControllerData();
#else
  LST_SPI_WaitForLineControllerData();
#endif

  /* ToDo Check for 0xFF control byte at the first byte of the SPI Rx buffer */
  /* ToDo Handle SPI Rx data in a separate module */

  /* Hand over control to the Controls module */
  /* ToDo refactor */
  LST_Control();
}
