/*
 * bsp.c
 *
 *  Created on: 22 Sep 2017
 *      Author: Balazs
 */

#include "bsp.h"
#include "main.h"

// Variables
static uint8_t LedEnable;
TIM_HandleTypeDef Tim4Handle;

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;


  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
}

void HAL_MSP_INIT() {

	LED_Init(LED_GREEN); //LED_ALL);

}

void LedInit(uint16_t ledPos) {

	GPIO_InitTypeDef portInit;

	// Parameter check
	/*
	if (!(ledPos & LED_ALL)) {

		assert_param(ledPos); // Shut down uC command if parameter error occurs (??)

	}
	*/

	__GPIOA_CLK_ENABLE();

	portInit.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull mode (other: open drain)
	portInit.Pin = ledPos; //& LED_ALL; // Initialise all LEDs at once
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(LED_PORT, &portInit);

	LedEnable = 1;

}

void LedToggle(uint16_t ledPos) {

	// Parameter check
	/*
		if (!(ledPos & LED_ALL)) {

			assert_param(ledPos); // Shut down uC command if parameter error occurs (??)

		}
		*/
	if (LedEnable)
		HAL_GPIO_TogglePin(LED_PORT, ledPos); // & LED_ALL);

}

// BUTTON INTERRUPT

void ButtonInit() {

	GPIO_InitTypeDef portInit;

	__GPIOC_CLK_ENABLE();

	portInit.Mode = GPIO_MODE_IT_FALLING; // Push-pull mode (other: open drain)
	portInit.Pin = BUTTON_TOGGLE; //& LED_ALL; // Initialise all LEDs at once
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(BUTTON_PORT, &portInit);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

__weak void ButtonPressed() { // __weak function can be redefined

	LedEnable = !LedEnable;

}

// Interrupt handling
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin) {

	if (GPIO_pin & BUTTON_TOGGLE) {

		ButtonPressed();

	}

}

// Timer initialisation
void TimerInit() {

	__TIM4_CLK_ENABLE();

	Tim4Handle.Instance = TIM4;
	Tim4Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim4Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim4Handle.Init.Prescaler = 999;
	Tim4Handle.Init.Period = 41999;

	HAL_TIM_Base_Init(&Tim4Handle);

	HAL_NVIC_SetPriority(TIM4_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_TIM_Base_Start_IT(&Tim4Handle);

}

// Timer callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim) {

	if (htim -> Instance == TIM4) {

		LedToggle(LED_GREEN);

	}

}


