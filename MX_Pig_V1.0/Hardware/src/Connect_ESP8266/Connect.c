/**
  ******************************************************************************
  * File Name          : key.c
  * Description        : This file provides code for the configuration
  *                      of STM32 connect esp8266 by uart1
  ******************************************************************************
 */
 /* Includes --------------------------------------------------------------------------------------------------------------------*/
#include "Connect.h"
#include "usart.h"
#include "tim.h"
/* Stdarg is the C standard function library header. 
   Stdarg is simplified from Standard (standard) arguments to 
   allow functions to accept non-quantitative arguments. 
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
/* External function declaration-------------------------------------------------------------------------------------------------*/




/* Static function declaration---------------------------------------------------------------------------------------------------*/
static char *itoa( int value, char *string, int radix );




/* Private macro definitions----------------------------------------------------------------------------------------------------*/




/* Global variable--------------------------------------------------------------------------------------------------------------*/

/* -------------Serial port 1 related variables---------------- */ 
/* Serial port 1 indicates that receiving is complete */
__IO ITStatus UART1_Rx_Flag = RESET;
/* Serial port 1 interrupts receiving flag bit */
__IO ITStatus Uart1Ready = RESET;
/* USART1 stores an array of received data */
uint8_t UART1_Rx_Buf[MAX_REC_LENGTH] = {0};
/* USART1 accepts a data counter */
uint8_t UART1_Rx_cnt = 0;   
/* USART1 receives the data cache array */
uint8_t UART1_temp[REC_LENGTH] = {0};   

/* -------------Receive and send data related------------------ */ 
/* Data frame structure */
struct STRUCT_USART_Fram ESP8266_Fram_Record_Struct = { 0 };

/* Static function definition---------------------------------------------------------------------------------------------------*/



/* Function definition----------------------------------------------------------------------------------------------------------*/


 


/** 
* @description: Enable serial port interruption
* @param {void} 
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t Cmd_TIM3_IT(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	if(HAL_TIM_Base_Start_IT(&htim3)!=HAL_OK)
    {
		ret = (uint8_t)OPERATION_ERROR;
	}
    return ret;
}

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

uint8_t USART1_printf(const char* Data, ...)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	const char *s;
	int d;   
    char buf[16];

    va_list ap;
    va_start(ap, Data);
	
	uint8_t temp_r = 0x0d;
	uint8_t temp_n = 0x0a;

    while (*Data != 0)  
    {                                         
        if (*Data == 0x5c)  
        {                                     
            switch ( *++Data )
            {
                case 'r':                                     
                if(HAL_UART_Transmit (&huart1, (uint8_t *)&temp_r, (uint16_t)1 , 1000)!=HAL_OK)
				{
					ret = (uint8_t)OPERATION_ERROR;
					return ret ;
				}
                Data ++;
                break;

                case 'n':                                  
                if(HAL_UART_Transmit (&huart1, (uint8_t *)&temp_n, (uint16_t)1 , 1000)!=HAL_OK)
				{
					ret = (uint8_t)OPERATION_ERROR;
					return ret ;
				}
                Data ++;
                break;

                default:
                Data ++;
                break;
            }            
        }

        else if ( * Data == '%')
        {                                     
            switch ( *++Data )
            {               
                case 's':                                         
                s = va_arg(ap, const char *);
                for ( ; *s; s++) 
                {
					if(HAL_UART_Transmit (&huart1, (uint8_t *)s, (uint16_t)1 , 1000)!=HAL_OK)
					{
						ret = (uint8_t)OPERATION_ERROR;
						return ret ;
					}
                }
                Data++;
                break;

                case 'd':           
                    
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++) 
                {
                    if(HAL_UART_Transmit (&huart1, (uint8_t *)s, (uint16_t)1 , 1000)!=HAL_OK)
					{
						ret = (uint8_t)OPERATION_ERROR;
						return ret ;
					}
                }
                     Data++;
                     break;
                default:
                     Data++;
                     break;
            }        
        }
        else 
        {
			if(HAL_UART_Transmit (&huart1, (uint8_t *)Data++, (uint16_t)1 , 1000)!=HAL_OK)
			{
				ret = (uint8_t)OPERATION_ERROR;
				return ret ;
			}
		}
    }
	return ret ;
}



/** 
* @description: Convert an integer to a string,itoa ( integer to array )
*               Converts an int integer to a string and stores the value in an array string
* @param {int}     value   :  integer
* @param {char *}  string  :  An array of characters stored after conversion
* @param {int}     radix   :  Convert base numbers, such as 2,8,10,16, etc
* @return {char}:  Pointer to a character array
* @author: leeqingshui 
*/
static char *itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;

    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */
 
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
      UART1_Rx_Flag = SET;
    }
    HAL_UART_Receive_IT(&huart1,(uint8_t *)UART1_temp,REC_LENGTH);
  }
}







