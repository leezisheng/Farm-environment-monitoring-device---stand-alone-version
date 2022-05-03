/**
  ******************************************************************************
  * File Name          : DTH11.h
  * Description        : This file contains all the functions prototypes for
  *                      the DTH11
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DTH11_H
#define __DTH11_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Common macro definitions---------------------------------------------------*/

/* For delay function adjustment , system clock is 54Mhz */
#define DELAY_COEFFICIENT   	( 5500000 )
/* Manipulate GPIO macro definition */
#define DTH11_Pin 				GPIO_PIN_2
#define DTH11_GPIO_Port 		GPIOC
/* Operations related to DTH11 ports */
#define DHT11_PIN_SET   		HAL_GPIO_WritePin(DTH11_GPIO_Port,DTH11_Pin,GPIO_PIN_SET)                                         
#define DHT11_PIN_RESET 		HAL_GPIO_WritePin(DTH11_GPIO_Port,DTH11_Pin,GPIO_PIN_RESET)                                     
#define DHT11_READ_IO    		HAL_GPIO_ReadPin(DTH11_GPIO_Port,DTH11_Pin) 

/* Data structure declaration-------------------------------------------------*/

/* Function declaration-------------------------------------------------------*/

/* DTH11 initialization */
uint8_t DHT11_Init (void);
/* Read temperature and humidity data once */
uint8_t DHT11_ReadData(uint8_t *ptemp , uint8_t *phumi);
/* Tests whether the microsecond delay is accurate */
void Test_Delay_us(uint32_t us);
/* Get the temperature value */
uint8_t DTH11_GetTemp(void);
/* Obtain humidity value */
uint8_t DTH11_GetHumi(void);

#ifdef __cplusplus
}
#endif
#endif /*__DTH11_H*/

