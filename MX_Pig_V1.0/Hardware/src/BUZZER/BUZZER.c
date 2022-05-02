/**
  ******************************************************************************
  * File Name          : BUZZER.c
  * Description        : This file provides code for the configuration
  *                      of BUZZER.
  ******************************************************************************
 */
  
/* Includes ------------------------------------------------------------------*/
#include "BUZZER.h"

/* External function declaration----------------------------------------------*/


/* Private macro definitions--------------------------------------------------*/


/* Global variable------------------------------------------------------------*/


/* Static function definition-------------------------------------------------*/


/* Function definition--------------------------------------------------------*/

/** 
* @description: Buzzer function
* @param  {uint8_t} times 	  : The number of times the buzzer sounded
* @param  {uint8_t} blinktime : LED2 Blink interval ,  in milliseconds
* @return {void}
* @author: leeqingshui 
*/
void Buzzer_Work(uint8_t times , uint8_t blinktime)
{
	while(times>0)
	{
      HAL_GPIO_TogglePin(BUZZER_GPIO_Port,BUZZER_Pin);
	  HAL_Delay(blinktime);
	  times--;
	}
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
}














