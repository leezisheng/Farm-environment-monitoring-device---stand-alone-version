/**
  ***********************************************************************************************************************
  * File Name          : UART_Printf.h
  * Description        : This file contains all the functions prototypes for
  *                      Connect to ESP8266 by USART1
  *
  ***********************************************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONNECT_H
#define __CONNECT_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Common macro definitions---------------------------------------------------*/

/* Serial port 1 macro definitions */
#define REC_LENGTH  1
#define MAX_REC_LENGTH  1024 

/* WIFI Parameter Setting-----------------------------------------------------*/
/* The name of the WIFI  */
#define WIFI_NAME   "CMCC-y36J"
/* WIFI password */
#define WIFI_KEY    "DE3e5SLL"

/* AT instruction macro definition--------------------------------------------*/
/* AT test instruction */
#define AT_TEST       "AT\r\n"
/* AT Restart command */
#define AT_RESET      "AT+RST\r\n"
/* Instruction for setting working mode AT: Set to STA mode */
#define AT_CWMODE     "AT+CWMODE=1\r\n"
/* AT command to connect WIFI */
#define AT_CWJAP      "AT+CWJAP="
/* Setting connection mode AT instruction: Single connection mode */
#define AT_CIPMUX     "AT+CIPMUX=0\r\n"

/* Device-specific macro definitions------------------------------------------*/




/* Macro defined function------------------------------------------------------*/

/* A macro function that calculates the length of an array */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

  
/* Extern Variable------------------------------------------------------------*/
/* Serial port 1 indicates that receiving is complete */
extern __IO ITStatus Uart1_Rx_Flag;
/* Serial port 1 indicates that receiving is complete */
extern __IO ITStatus Uart1Ready;
/* USART1 stores an array of received data */
extern uint8_t UART1_Rx_Buf[MAX_REC_LENGTH];
/* USART1 accepts a data counter */
extern uint8_t UART1_Rx_cnt;   
/* USART1 receives the data cache array */
extern uint8_t UART1_temp[REC_LENGTH];   


/* Data structure declaration-------------------------------------------------*/



/* Function declaration-------------------------------------------------------*/
/* Enable serial port interruption */
uint8_t Cmd_UART1_IT(void);
/* "Serial yes-man" function, send what you receive.  
    If the serial port data is received, it's sent and the array, counters, 
	flags are restored to their original state.   
*/
uint8_t UART1_Transmit_Rx(void);

#ifdef __cplusplus
}
#endif
#endif /* __CONNECT_H */


