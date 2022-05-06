/**
  ******************************************************************************
  * File Name          : key.c
  * Description        : This file provides code for the configuration
  *                      of UART3_Printf.
  *
  * This file includes the printf function redirection for serial ports 3 and 1
  ******************************************************************************
 */
 
/* Includes ------------------------------------------------------------------*/
#include "UART_Printf.h"
#include "usart.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
/* External function declaration----------------------------------------------*/

/* Private macro definitions--------------------------------------------------*/

/* Global variable------------------------------------------------------------*/

/* Static function definition-------------------------------------------------*/
static char *itoa( int value, char *string, int radix );

/* Function definition--------------------------------------------------------*/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/** 
* @description: the printf function for serial port 1 redirection
*
*               When using c language programming, usually is to determine the number of parameters, 
*               the forms of function in the call, I will, in turn,
*               all the actual parameters and the corresponding formal parameters are given, 
*               but in some cases hope function can according to the need to identify the number of parameters, 
*               such as printf, the scanf function, such as the c compiler provides a series of macro deal with this situation, 
*               These macros include Va_start, va_arg and va_end, etc.
*
*               In ANSI standard form, a function with a variable number of arguments is prototyped as:
*               type funcname(type para1,...) ;
*               The ellipsis is part of the function form. 
*               Type is the type of the function return value and the formal parameter.
*
*               Va is a variable argument.
*
*               The three macros in the standard C library are used only to determine the memory address of each argument 
*               in the variable argument list. The compiler does not know the actual number of arguments:
*               1.Set flags in fixed arguments -- the printf function is determined with the first fixed string argument;
*               2.In advance set a special end mark, that is to say, more input a variable parameter, call the last variable 
*               parameter value is set to this special value, 
*               in the function body according to the value of the end of the parameter to judge whether;
*
* @param {const char*} Data : String constant
* @param {void}        ...  : Format the sent data
* @return {int} ret: if success , return OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t USART1_printf(const char* Data, ...)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	const char *s;
	int d;   
    char buf[16];

	/* 
	typedef struct __va_list { void *__ap; } va_list; 
	
	This variable is a pointer to the address of the parameter. 
	The value of the parameter can only be obtained by combining the parameter type with the address of the parameter. 
	Va_list is defined as char*. Some machines also define void*
	
	*/
    va_list ap;
	
	/*
	#define va_start(ap, parmN) __va_start(ap, parmN)
	
	Va_start (va_list,type) refers to the first variable argument. 
	Va_start is defined as &v + _INTSIZEOF(v), where &v is the starting address of the last fixed argument.
	*/
    va_start(ap, Data);
	
	uint8_t temp_r = 0x0d;
	uint8_t temp_n = 0x0a;

    while (*Data != 0)  // Determine if it is the end
    {                                         
        if (*Data == 0x5c)  // Determine if it is "/"
        {                                     
            switch ( *++Data ) // Determine if it is "/"
            {
                case 'r': // Check whether it is a carriage return character                                    
                if(HAL_UART_Transmit (&huart1, (uint8_t *)&temp_r, (uint16_t)1 , 1000)!=HAL_OK)
				{
					ret = (uint8_t)OPERATION_ERROR;
					return ret ;
				}
                Data ++;
                break;

                case 'n':   // Check whether it is a carriage return character                              
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

        else if ( * Data == '%')  // Determines whether it is a format symbol
        {                                     
            switch ( *++Data )
            {               
                case 's':  // Check whether it is a character type   
                
                /* 
				#define va_arg(ap, type) __va_arg(ap, type)
				
				Va_arg (va_list, type) is used to obtain the value of the specified parameter type and 
				make va_list point to the starting address of a parameter, equivalent to a stack operation
				
				This macro does two things:
				(1) Use the type name entered by the user to cast the parameter address to obtain the value required by the user
				(2) Calculate the actual size of this parameter, and move the pointer to the end of this parameter, that is, 
				the first address of the next parameter, for subsequent processing.
				
				*/				
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

                case 'd':  // Checks whether it is a decimal symbol         
                    
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
	/* Clears the argument list. The collimated argument pointer arg_ptr is invalid. */
	va_end (ap);
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

