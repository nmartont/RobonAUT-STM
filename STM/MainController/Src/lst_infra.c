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
NEC nec;

/* External variables --------------------------------------------------------*/

/* Private functions -------------------------------------------------------- */
void myNecDecodedCallback(uint16_t address, uint8_t cmd);
void myNecErrorCallback();
void myNecRepeatCallback();

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
  nec.timerHandle = &htim8;

  nec.timerChannel = TIM_CHANNEL_3;
  nec.timerChannelActive = HAL_TIM_ACTIVE_CHANNEL_3;

  nec.timingBitBoundary = 1680; // ToDo what??
  nec.timingAgcBoundary = 12500;// ToDo what??
  nec.type = NEC_NOT_EXTENDED;  // ToDo what??

  nec.NEC_DecodedCallback = myNecDecodedCallback;
  nec.NEC_ErrorCallback = myNecErrorCallback;
  nec.NEC_RepeatCallback = myNecRepeatCallback;

  // TODO:TEST 2018. 01. 25.
  nec.timingAgcBoundary = 0;

  NEC_Read(&nec);
}

void myNecDecodedCallback(uint16_t address, uint8_t cmd) {
  /*
  char buff[100];
  snprintf(buff, 100, "A:%d\tC:%d\n", address, cmd);
  HAL_UART_Transmit_DMA(&huart3, (uint8_t*) buff, strlen(buff));
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
  HAL_Delay(10);
  NEC_Read(&nec);
  */
}

void myNecErrorCallback() {
  /*
  char* msg = "Error!\n";
  HAL_UART_Transmit_DMA(&huart3, (uint8_t*) msg, strlen(msg));
  HAL_Delay(10);
  NEC_Read(&nec);
  */
}

void myNecRepeatCallback() {
  /*
  char* msg = "Repeat!\n";
  HAL_UART_Transmit_DMA(&huart3, (uint8_t*) msg, strlen(msg));
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
  HAL_Delay(10);
  NEC_Read(&nec);
  */
}


/**
 * @brief Function that handles the timer capture callback
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

	// TODO:TEST 2018. 01. 25.
	LST_test_counter++;

  if (htim == &htim8) {
      NEC_TIM_IC_CaptureCallback(&nec);
  }
}
