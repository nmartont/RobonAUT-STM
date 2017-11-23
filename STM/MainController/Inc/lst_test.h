/*
 * lst_tasks.h
 *
 *  Created on: 2017. nov. 1.
 *      Author: nmartont
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LST_TEST_H_
#define LST_TEST_H_

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "lst_timer.h"
#include "lst_spi.h"
#include "lst_uart.h"
#include "lst_adc.h"
#include "lst_bt.h"

#ifdef LST_CONFIG_TEST

/* Defines -------------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void LST_Test_Start(void const * argument);
void LST_Test_UART(void const * argument);
void LST_Test_SPI(void const * argument);
void LST_Test_TIM(void const * argument);
void LST_Test_BT(void const * argument);
void LST_Test_ADC(void const * argument);
void LST_Test_PWM(void const * argument);

/* Private variables ---------------------------------------------------------*/
osThreadId lst_test_StartTestHandle;
osThreadId lst_test_UartTestHandle;
osThreadId lst_test_TimerTestHandle;
osThreadId lst_test_SpiTestHandle;
osThreadId lst_test_BtTestHandle;
osThreadId lst_test_AdcTestHandle;
osThreadId lst_test_PwmTestHandle;

#endif /* LST_CONFIG_TEST */
#endif /* LST_TEST_H_ */
