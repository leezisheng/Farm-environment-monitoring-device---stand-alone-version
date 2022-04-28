/**
  ***********************************************************************************************************************
  * File Name          : UART_Printf.h
  * Description        : This file contains all the functions prototypes for
  *                      Connect to ESP8266 by USART1
  *
  ***********************************************************************************************************************
  */
  
/* Define to prevent recursive inclusion --------------------------------------------------------------------------*/
#ifndef __CONNECT_H
#define __CONNECT_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -------------------------------------------------------------------------------------------------------*/
#include "main.h"
/* Common macro definitions----------------------------------------------------------------------------------------*/

/* Anonymous structure-related compiler options 
   Related to ESP8266_Fram_Record_Struct after
*/
#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/* Serial port 1 macro definitions */
#define REC_LENGTH  1
#define MAX_REC_LENGTH  1024 

/* WIFI Parameter Setting */
/* The name of the WIFI  */
#define WIFI_NAME   "CMCC-y36J"
/* WIFI password */
#define WIFI_KEY    "DE3e5SLL"

/* MQTT protocol parameters */
/* Client ID */
#define MQTT_User_client_id    "123456"  
/* Server IP Address */
#define MQTT_Server_IP     "192.168.31.16"  
/* Server port number */
#define MQTT_Server_PORT   1883    
/* Subscribe topic */
#define MQTT_Server_Topic  "topic"    

/* The equipment parameters:Device triad */
#define DEVICE_NAME       "123456"  
#define DEVICE_SECRET     "123456"
#define PRODUCT_KEY "a1NRzHXlTTp"

/* AT instruction macro definition*/


/* Device-specific macro definitions-------------------------------------------------------------------------------*/

/* Pin macro definition */
/* Pins about the reset function*/
#define ESP8266_RST_Pin GPIO_PIN_12
#define ESP8266_RST_GPIO_Port GPIOB
/* Pins about enabling functions */
#define ESP8266_CH_PD_Pin GPIO_PIN_13
#define ESP8266_CH_PD_GPIO_Port GPIOB

/* Macro defined function-------------------------------------------------------------------------------------------*/

/* A macro function that calculates the length of an array */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#define ESP8266_RST_Pin_SetH       HAL_GPIO_WritePin(ESP8266_RST_GPIO_Port, ESP8266_RST_Pin, GPIO_PIN_SET)
#define ESP8266_RST_Pin_SetL       HAL_GPIO_WritePin(ESP8266_RST_GPIO_Port, ESP8266_RST_Pin, GPIO_PIN_RESET)


#define ESP8266_CH_PD_Pin_SetH     HAL_GPIO_WritePin(ESP8266_CH_PD_GPIO_Port, ESP8266_CH_PD_Pin, GPIO_PIN_SET)
#define ESP8266_CH_PD_Pin_SetL     HAL_GPIO_WritePin(ESP8266_CH_PD_GPIO_Port, ESP8266_CH_PD_Pin, GPIO_PIN_RESET)

  
/* Extern Variable-------------------------------------------------------------------------------------------------*/
/* Serial port 1 indicates that receiving is complete */
extern __IO ITStatus UART1_Rx_Flag;
/* Serial port 1 indicates that receiving is complete */
extern __IO ITStatus Uart1Ready;
/* USART1 stores an array of received data */
extern uint8_t UART1_Rx_Buf[MAX_REC_LENGTH];
/* USART1 accepts a data counter */
extern uint8_t UART1_Rx_cnt;   
/* USART1 receives the data cache array */
extern uint8_t UART1_temp[REC_LENGTH];   


/* Data structure declaration--------------------------------------------------------------------------------------*/
/* Enumeration of esp8266 working modes */
typedef enum
{
    STA,
    AP,
    STA_AP  
}ENUM_Net_ModeTypeDef;

/* Enumeration of network transport layer protocols */
typedef enum{
     enumTCP,
     enumUDP,
} ENUM_NetPro_TypeDef;

/* Enumeration of connection numbers to prevent other computers from accessing the same port */
typedef enum{
    Multiple_ID_0 = 0,
    Multiple_ID_1 = 1,
    Multiple_ID_2 = 2,
    Multiple_ID_3 = 3,
    Multiple_ID_4 = 4,
    Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;

/* Data frame structure definition */
/* !!! An error occurs if you do not add the extern keyword */
extern struct STRUCT_USART_Fram  
{
    char Data_RX_BUF[MAX_REC_LENGTH];
    union 
    {
        __IO uint16_t InfAll;
		/* If you want MDK5 to support anonymous structures, you need to compile the #pragma anon_unions directive */
        struct 
        {
            __IO uint16_t FramLength       :15;                               // 14:0 
            __IO uint16_t FramFinishFlag   :1;                                // 15 
        }InfBit;
    }; 
	
}ESP8266_Fram_Record_Struct;

/* Function declaration--------------------------------------------------------------------------------------------*/

/* Enable serial port interruption */
uint8_t Cmd_UART1_IT(void);

/* Enable timer 3 to interrupt */
uint8_t Cmd_TIM3_IT(void);
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																												
/* Serial port redirection: Serial port 1 only!! */
uint8_t USART1_printf(const char* Data, ...);


#ifdef __cplusplus
}
#endif
#endif /* __CONNECT_H */


