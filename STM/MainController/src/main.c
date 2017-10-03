/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "bsp.h"

#include "freeRTOS.h"
#include "task.h"

void Task1(void *p) {

	while (1) {

		LedToggle(LED_GREEN);
		vTaskDelay(100);

	}

}

int main(void) {

	HAL_Init();
	LedInit(LED_GREEN);
	ButtonInit();
	//TimerInit();

	// FreeRTOS
	xTaskCreate(Task1, "TASK1", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();

	// xTaskStartScheduler never returns
	// Remove FreeRTOS to proceed to this point (base program with loop)

	for(;;) {

		HAL_Delay(1000);

		LedToggle(LED_GREEN);

	}

}

// Added: void EXTI0_IRQHandler(void) to stm32f4xx_it.c, also included bsp.h
