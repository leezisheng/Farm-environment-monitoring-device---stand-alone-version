/**
  ******************************************************************************
  * File Name          : key.c
  * Description        : This file provides code for the configuration
  *                      of STM32 connect esp8266 by uart1
  ******************************************************************************
 */
 /* Includes ------------------------------------------------------------------*/
#include "Connect.h"
#include "usart.h"
 
/* External function declaration----------------------------------------------*/


/* Private macro definitions--------------------------------------------------*/


/* Global variable------------------------------------------------------------*/

/* -------------Serial port 1 related variables---------------- */ 
/* Serial port 1 indicates that receiving is complete */
__IO ITStatus Uart1_Rx_Flag = RESET;
/* Serial port 1 interrupts receiving flag bit */
__IO ITStatus Uart1Ready = RESET;
/* USART1 stores an array of received data */
uint8_t UART1_Rx_Buf[MAX_REC_LENGTH] = {0};
/* USART1 accepts a data counter */
uint8_t UART1_Rx_cnt = 0;   
/* USART1 receives the data cache array */
uint8_t UART1_temp[REC_LENGTH] = {0};   

/* Static function definition-------------------------------------------------*/



/* Function definition--------------------------------------------------------*/

/** 
* @description: Enable serial port 1 to receive messages
* @param {void} 
* @return {int} ret: cmd uart1_it success , return OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t Cmd_UART1_IT(void)
{
    uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	if(HAL_UART_Receive_IT(&huart1,(uint8_t *)UART1_temp,REC_LENGTH)!=HAL_OK)
	{
		ret = (uint8_t)OPERATION_ERROR;
	}
	return ret;
}
 
/** 
* @description: Serial port 1 interrupts callback function
* @param {UART_HandleTypeDef} *huart: Serial port to call the callback function
* @return {void} 
* @author: leeqingshui 
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance==USART1)
  {
    UART1_Rx_Buf[UART1_Rx_cnt] = UART1_temp[0];
    UART1_Rx_cnt++;
	/* 0xa == \n, newline character */
    if(0x0a == UART1_temp[0])
    {
      Uart1_Rx_Flag = SET;
    }
    HAL_UART_Receive_IT(&huart1,(uint8_t *)UART1_temp,REC_LENGTH);
  }
}

/** 
* @description: "Serial yes-man" function, send what you receive.  
                If the serial port data is received, it's sent and the array, counters, 
	            flags are restored to their original state.  
* @param {void} 
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t UART1_Transmit_Rx(void)
{
	uint8_t ret = (uint8_t)OPERATION_FALSE;
	if(Uart1_Rx_Flag)
    {
	  /* Send Received data */
      HAL_UART_Transmit(&huart1,UART1_Rx_Buf,UART1_Rx_cnt,0x10);    
	  
	  /* Variable reset */
      for(int i = 0;i<UART1_Rx_cnt;i++)
	  {
		UART1_Rx_Buf[i] = 0;
	  }
      UART1_Rx_cnt = 0;
      Uart1_Rx_Flag = 0;
	  
	  
	  ret = (uint8_t)OPERATION_SUCCESS;
    }   
	return ret;
}
 
 
 

