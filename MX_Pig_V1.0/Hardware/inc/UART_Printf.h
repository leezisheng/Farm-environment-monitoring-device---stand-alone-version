/**
  ******************************************************************************
  * File Name          : UART_Printf.h
  * Description        : This file contains all the functions prototypes for
  *                      the UART_Printf
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_Printf_H
#define __UART_Printf_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Common macro definitions---------------------------------------------------*/

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Data structure declaration-------------------------------------------------*/

/* Function declaration-------------------------------------------------------*/


#ifdef __cplusplus
}
#endif
#endif /* __UART_Printf_H */

