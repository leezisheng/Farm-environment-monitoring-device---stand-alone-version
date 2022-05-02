/**
  ******************************************************************************
  * File Name          : BUZZER.h
  * Description        : This file contains all the functions prototypes for
  *                      the BUZZER
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUZZER_H
#define __BUZZER_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Common macro definitions---------------------------------------------------*/
#define BUZZER_Pin 			GPIO_PIN_5
#define BUZZER_GPIO_Port 	GPIOA

/* Data structure declaration-------------------------------------------------*/

/* Function declaration-------------------------------------------------------*/
/* Control the buzzer tick and LED2 blink */
void Buzzer_Work(uint8_t times , uint8_t blinktime);


#ifdef __cplusplus
}
#endif
#endif /*__BUZZER_H*/
