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

// Infravevõ változói
uint32_t inputCapture = 0;
uint32_t ICValue1 = 0;
uint32_t ICValue2 = 0;
uint32_t highPulse = 0;
uint32_t lowPulse = 0;
uint32_t pulse = 0;
uint32_t infraT = 900;
uint32_t infraTolerance = 150;
uint8_t RC5BitsBuffer[20];
uint8_t RC5BitsCounter = 0;
uint8_t RC5Bit = 0;
uint8_t RC5LastBit = 0;
uint16_t RC5Word = 0;
uint8_t infraAddress = 0;
uint8_t infraCommand = 0;
double infraPulses[100];
uint8_t infraPulseCounter = 0;

/******************************************************************************/
/*                   Infra handler for RobonAUT 2018 Team LST                 */
/******************************************************************************/
/* Initialization function ---------------------------------------------------*/
/**
 * @brief Initializes the Infra module
 */
void LST_Infra_Init() {
  HAL_TIM_Base_Start_IT(&htim8);  //infra számlálójának indítása
}

// nii ja naa
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM8) //counter timeout: sokáig nem jött él
  {
    if (infraPulses[10] > 500 || infraPulses[10] < -500)
    {
      //feldolgozás jön ide
      for (int i = 0; i < 100; i++)
      {
        if (infraPulses[i] > 0.0)               //felfutó él
        {
          pulse = infraPulses[i];
          RC5Bit = RC5CurrentBit(1, RC5LastBit, pulse);
          if (RC5Bit == 0)  //logikai 0 érkezett
          {
            RC5BitsBuffer[RC5BitsCounter] = 0;
            RC5BitsCounter++;
            RC5LastBit = 0;
          }
          if (RC5Bit == 1)  //logikai 1 érkezett
          {
            RC5BitsBuffer[RC5BitsCounter] = 1;
            RC5BitsCounter++;
            RC5LastBit = 1;
          }
          if (RC5Bit == 3)  //invalid bit
          {
            RC5Reset();
          }
        }
        else                        //lefutó él
        {
          pulse = - 1.0 * infraPulses[i];
          RC5Bit = RC5CurrentBit(0, RC5LastBit, pulse);
          if (RC5Bit == 0)  //logikai 0 érkezett
          {
            RC5BitsBuffer[RC5BitsCounter] = 0;
            RC5BitsCounter++;
            RC5LastBit = 0;
          }
          if (RC5Bit == 1)  //logikai 1 érkezett
          {
            RC5BitsBuffer[RC5BitsCounter] = 1;
            RC5BitsCounter++;
            RC5LastBit = 1;
          }
          if (RC5Bit == 3)  //invalid bit
          {
            RC5Reset();
          }
          if (RC5Bit == 4)  //rossz pulzushossz: kezdõ 1-es
          {
            RC5BitsBuffer[0] = 1;
            RC5BitsCounter = 1;
            RC5LastBit = 1;
          }
        }

        if (RC5BitsCounter == 14)             //kiolvastuk az utolsó bitet is
        {
          RC5Word = 0;
          for (int i = 0; i < 14; i++)
            RC5Word += (RC5BitsBuffer[13 - i] << i);
          infraAddress = (RC5Word>>6) & 0x1F;
          if (infraAddress == 0x0D){
            lst_infra_is_available = LST_INFRA_AVAILABLE;
            infraCommand = RC5Word & 0x3F;
          }
          RC5Reset();
        }
      }
    }
    RC5Reset();
  }
}

// elupoletaja
uint8_t RC5CurrentBit(uint8_t edge, uint8_t lastBit, uint32_t pulse)
{
  if (edge) //felfutó él
  {
    if (lastBit == 1)
    {
      if (pulse >= infraT - infraTolerance && pulse <= infraT + infraTolerance)
      {
        return 2; //NaN
      }
      if (pulse >= 2 * infraT - infraTolerance && pulse <= 2 * infraT + infraTolerance)
      {
        return 0; //logikai 0
      }
    }
    if (lastBit == 0)
    {
      if (pulse >= infraT - infraTolerance && pulse <= infraT + infraTolerance)
      {
        return 0; //logikai 0
      }
      if (pulse >= 2 * infraT - infraTolerance && pulse <= 2 * infraT + infraTolerance)
      {
        return 3; //invalid
      }
    }
  }
  else    //lefutó él
  {
    if (lastBit == 1)
    {
      if (pulse >= infraT - infraTolerance && pulse <= infraT + infraTolerance)
      {
        return 1; //logikai 1
      }
      if (pulse >= 2 * infraT - infraTolerance && pulse <= 2 * infraT + infraTolerance)
      {
        return 3; //invalid
      }
    }
    if (lastBit == 0)
    {
      if (pulse >= infraT - infraTolerance && pulse <= infraT + infraTolerance)
      {
        return 2; //NaN
      }
      if (pulse >= 2 * infraT - infraTolerance && pulse <= 2 * infraT + infraTolerance)
      {
        return 1; //logikai 1
      }
    }
  }
  return 4; //rossz pulzushossz
}

// linn on minu
void RC5Reset()
{
  RC5BitsCounter = 0;
  RC5LastBit = 0;
  for (int i = 0; i < 20; i++)
    RC5BitsBuffer[i] = 0;
  for (int i = 0; i < 100; i++)
    infraPulses[i] = 0;
  infraPulseCounter = 0;
}

uint8_t LST_Infra_Get_Direction(){
  if (infraCommand == 0x01 ||
      infraCommand == 0x02 ||
      infraCommand == 0x03)
    return LST_INFRA_DIR_RIGHT;

  return LST_INFRA_DIR_LEFT;
}

uint8_t LST_Infra_Get_Exit(){
  if (infraCommand == 0x01 ||
      infraCommand == 0x04)
    return LST_INFRA_EXIT_ONE;

  if (infraCommand == 0x02 ||
      infraCommand == 0x05)
    return LST_INFRA_EXIT_TWO;

  return LST_INFRA_EXIT_THREE;
}
