
#ifndef BSP_H_
#define BSP_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

// LED

#define LED_GREEN		(uint16_t) 1<<5

#define LED_PORT	GPIOA //GPIOD

void LedInit(uint16_t ledPos);
void LedToggle(uint16_t ledPos);

// BUTTON

#define BUTTON_PORT		GPIOC
#define BUTTON_TOGGLE	(uint16_t) 1<<13

void ButtonInit();
void ButtonPressed();

// TIMER

void TimerInit();
extern TIM_HandleTypeDef Tim4Handle;

#endif
