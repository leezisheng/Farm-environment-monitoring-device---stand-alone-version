/**
  ******************************************************************************
  * File Name          : key.c
  * Description        : This file provides code for the configuration
  *                      of STM32 connect esp8266 by uart1
  ******************************************************************************
 */
 /* Includes --------------------------------------------------------------------------------------------------------------------*/
#include "ESP8266.h"
#include "usart.h"
#include "tim.h"
#include "UART_Printf.h"
/* Stdarg is the C standard function library header. 
   Stdarg is simplified from Standard (standard) arguments to 
   allow functions to accept non-quantitative arguments. 
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
/* External function declaration-------------------------------------------------------------------------------------------------*/

/* External variable-------------------------------------------------------------------------------------------------------------*/

/* Static function declaration---------------------------------------------------------------------------------------------------*/
static uint8_t Cmd_UART1_IT(void);
static uint8_t Cmd_TIM3_IT(void);

/* Private macro definitions----------------------------------------------------------------------------------------------------*/

/* Global variable--------------------------------------------------------------------------------------------------------------*/

/* -------------Receive and send data related------------------ */ 
/* Data frame structure */
struct STRUCT_USART_Fram ESP8266_Fram_Record_Struct = { 0 };
/* Serial port receiving area, data cache */
uint8_t RxBuffer=0; 

/* Function definition----------------------------------------------------------------------------------------------------------*/

/*++++++++++++++++++++++++++++++++++++++++++++++ESP8266 related function functions++++++++++++++++++++++++++++++++++++++++++++++*/

/** 
* @description: ESP8266 initialization,include :
*               ESP8266 restart function
*               Enable serial port interruption
*               Enable serial port interruption
* @param {void} 
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_Init(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	if(ESP8266_Rst()!=(uint8_t)OPERATION_SUCCESS)
    {
	  ret = (uint8_t)OPERATION_ERROR;
	}
	
	// Enable the working port for high level
	ESP8266_CH_PD_Pin_SetH;
	
	if(Cmd_TIM3_IT()!=(uint8_t)OPERATION_SUCCESS)
    {
	  ret = (uint8_t)OPERATION_ERROR;
	}
	
	if(Cmd_UART1_IT()!=(uint8_t)OPERATION_SUCCESS)
    {
	  ret = (uint8_t)OPERATION_ERROR;
	}
	return ret;
}

/** 
* @description: Set the time domain and SNTP server
* @param  {uint32_t} timezone : Time domain, range: [-11,13]; If SNTP is enabled, this parameter is mandatory. Otherwise, no need to fill in
*                               SNTP is enabled by default
* @param  {char *}   server_0 : The first SNTP server is optional
* @param  {char *}   server_1 : The second SNTP server is optional
* @param  {char *}   server_2 : The third SNTP server is optional
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
__weak uint8_t ESP8266_Set_SNTP(uint32_t timezone , char * server_0 , char * server_1 , char * server_2)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cCmd [120];
	
	/*
	AT+CIPSNTPCFG=<enable>[,<timezone>][,<SNTP server0>,<SNTP	server1>,<SNTP	server2>]
	*/
	sprintf (cCmd, "AT+CIPSNTPCFG=1,%d,\"%s\",\"%s\",\"%s\"", timezone, server_0, server_1, server_2);
	
	if(ESP8266_Send_AT_Cmd (cCmd, "OK", NULL, 2000) != (uint8_t)OPERATION_SUCCESS)
	{
		ret = (uint8_t)OPERATION_ERROR;
	}

	return ret;
}

/** 
* @description: ESP8266 restart function
* @param {void} 
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_Rst(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
    ESP8266_RST_Pin_SetL;
    HAL_Delay(500); 
    ESP8266_RST_Pin_SetH;
	return ret;
}

/** 
* @description: Send the AT command to the ESP8266 module
*               In Data Frame structure, the update of received Data is carried out in serial port interrupt
* @param {char *  } cmd  : Instructions to be sent
* @param {char *  } ack1 : Expected reply signal 1, If NULL, no reply is required
* @param {char *  } ack2 : Expected reply signal 2, If NULL, no reply is required
*                          The logical relation between the two being OR
* @param {uint32_t} time : The time to wait for a response
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,uint32_t time)
{
    uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	// Receives new packets again
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
	ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 0;
	
    if(ESP8266_USART("%s\r\n", cmd)!=(uint8_t)OPERATION_SUCCESS)
	{
		ret = (uint8_t)OPERATION_ERROR;
	}
	// There is no need to receive data
	if(ack1==0&&ack2==0)     
    {
		return ret;
    }
	// Delay the serial port reception
	HAL_Delay(1000+time);

	ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0';
	
    printf("%s\r\n",ESP8266_Fram_Record_Struct .Data_RX_BUF);
	
	//strstr(s1,s2) : Checks if S2 is part of S1, returns the bit if it is, and false otherwise  
	if(ack1!=0&&ack2!=0)
    {
        ret =((uint32_t) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack1 ) || (uint32_t) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack2 ) );
    }
	else if( ack1 != 0 )  
        ret = ((uint32_t) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack1 ) );

    else
        ret = ((uint32_t) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack2 ) );

	if (ret>0)
		ret = (uint8_t)OPERATION_SUCCESS;
	else
		ret = (uint8_t)OPERATION_ERROR;
	
	return ret;
}

/** 
* @description: Send AT command to restore factory Settings and erase the saved parameters
* @param {void}
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_AT_RESTORE(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
    char count=0;
    HAL_Delay(1000); 
    while(count < 10)
    {
        if(ESP8266_Send_AT_Cmd("AT+RESTORE","OK",NULL,500)) 
        {
            printf("RESTORE OK\r\n");
            return ret;
        }
        ++ count;
		ret = (uint8_t)OPERATION_FALSE;
    }
	return ret;
}

/** 
* @description: Select the ESP8266 working mode
* @param {ENUM_Net_ModeTypeDef} enumMode : ESP8266 working mode
*        STA       : Station (client mode) : similar to mobile phones, small ai audio and other devices (Client)  
*        AP        : AP (Access point mode) : Similar to a router (server), AP can be accessed by other devices  
*		 STA_AP    : Station +AP (Client +AP mode)
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/

uint8_t ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)
{
    switch ( enumMode )
    {
        case STA:
            return ESP8266_Send_AT_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 

        case AP:
            return ESP8266_Send_AT_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 

        case STA_AP:
            return ESP8266_Send_AT_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 

        default:
           return (uint8_t)OPERATION_FALSE;
    }       
}

/** 
* @description: Connect to external WIFI  
* @param {char *} pSSID     : WIFI name
* @param {char *} pPassWord : WIFI key
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_JoinAP( char * pSSID, char * pPassWord)
{
    char cCmd [120];
	/*
	Sprintf is almost the same as printf in usage, except that it is printed to a different destination. 
	The former is printed to a string, while the latter is printed directly on the command line  
	
	Sprintf is a variable parameter function defined as follows:
		int sprintf( char *buffer, const char *format [, argument] … );
	
	Printf and sprintf both use format strings to specify the format of the string, use format descriptors starting with % to 
	occupy a space inside the format string, and provide corresponding variables in the following argument list.  
	Eventually the function replaces the specifier with a variable at the corresponding position, producing the desired string.  
	
	Sprintf are often used to : 
	1. Formatted numeric string:
		One of the most common uses of Sprintf is to print integers into strings, 
		so spritNF can replace ITOA (converting an integer toa string) in most cases.
	2. Controls the printing format of floating-point numbers
	3. Connection string
	
	*/
    sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
    return ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 5000 );
}

/** 
* @description: Whether to use multi-link mode in transparent transport
* @param {FunctionalState} enumEnUnvarnishTx : Whether to use multi-link mode , DISABLE or ENABLE
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_Enable_MultipleId (FunctionalState enumEnUnvarnishTx )
{
    char cStr [20];

    sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );

    return ESP8266_Send_AT_Cmd ( cStr, "OK", 0, 500 );
}

/** 
* @description: The ESP8266 connects to the server over TCP or UDP 
* @param {ENUM_NetPro_TypeDef} enumE   : Network type of the connection, TCP or UDP
*
*                                        Generally speaking, TCP corresponds to applications with high reliability requirements, 
*                                        while UDP corresponds to applications with low reliability requirements and low transmission economy.
*                                        
*										 TCP is a connection-oriented protocol. Before sending and receiving data, a reliable connection must be 
*                                        established with the other party. The three-way handshake for establishing a connection and the four-way 
*                                        handshake for disconnecting a connection lay a reliable foundation for data transmission. 
*                                        UDP is a connectionless protocol. Before data transmission, the source end and the terminal do not 
*                                        establish a connection. The sender sends data to the network as quickly as possible, and the receiver 
*                                        reads message segments from the message queue.
* @param {char *}              ip      : Server IP Address
* @param {char *}              ComNum  : Server port number
*
*                                        In network technology, Port roughly has two meanings: one is the physical Port, for example, the interface 
*                                        used by ADSL Modem, hub, switch and router to connect other network devices, such as RJ-45 Port, SC Port 
*                                        and so on. The second is the logical port, generally refers to the TCP/IP protocol port, port number range 
*                                        from 0 to 65535, such as port 80 for web browsing service, port 21 for FTP service and so on. What we are 
*                                        going to introduce here is a logical port
*
*                                        What are ports for? As we know, a host with an IP address can provide many services, such as Web services, 
*                                        FTP services, SMTP services, and so on. These services can be realized by using only one IP address. So 
*                                        how do hosts differentiate between different network services? Obviously, you can't rely on IP addresses 
*                                        alone, because the relationship between IP addresses and network services is one-to-many. In fact, 
*                                        IP address + port number is used to distinguish different services.
*                                     
* 									     Servers are generally identified by well-known port numbers. For example, for each TCP/IP implementation, 
*                                        the TCP port number of the FTP server is 21, the TCP port number of each Telnet server is 23, and the UDP 
*                                        port number of each TFTP(Simple File Transfer Protocol) server is 69. Any TCP/IP implementation provides 
*                                        services with well-known port numbers between 1 and 1023. These famous port number assigned by the Internet, 
*                                        organizations (InternetAssignedNumbersAuthority, IANA) to manage.
* @param {ENUM_ID_NO_TypeDef}  id      : Device Connection number
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cStr [100] = { 0 }, cCmd [120];
	
	switch (  enumE )
    {
        case enumTCP:
          sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
          break;

        case enumUDP:
          sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
          break;

        default: 
		  ret = (uint8_t)OPERATION_ERROR;
          break;
    }
	
	if ( id < 5 )
		/*
	    AT+CIPSTART - Establishes TCP, UDP transport or SSL connections
	
	    TCP single connection (AT+CIPMUX=0) : AT+CIPSTART=<type>,<remote IP>,<remote	port>[,<TCP	keep alive>] 
	    TCP multi-connection (AT+CIPMUX=1)  : AT+CIPSTART=<link	ID>,<type>,<remote IP>,<remote port>[,<TCP	keep alive>]
	
	    Parameter description:
	    <link	ID>        ：Network connection ID (0 to 4), used for multiple connections
	    <type>             ：string argument, connection type, "TCP", "UDP" or "SSL"
	    <remote	IP>        : String parameter, remote IP address
	    [<TCP keep alive>] ：0        : disables the TCP keep-alive function
	                         1 to 7200: detection time, expressed in 1s
	
	    example            : AT+CIPSTART="TCP","iot.espressif.cn",8000
	                         AT+CIPSTART="TCP","192.168.101.110",1000
	    */
        sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

    else
        sprintf ( cCmd, "AT+CIPSTART=%s", cStr );
	
	ret = ESP8266_Send_AT_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
	
    return ret;
}

/** 
* @description: The transparent transmission function was enabled 
* @param {void} 
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_UnvarnishSend (void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	/*
	AT+CIPMODE=<mode>
	
	<mode>：0: common transmission mode
	        1: transparent transmission mode, which supports only the TCP single connection and UDP fixed communication on the peer end
	*/
	ret = ESP8266_Send_AT_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 );
	
	if(ret!=(uint8_t)OPERATION_SUCCESS)
	{
		return ret;
	}
	
	/*
	AT+CIPSEND - Sends data , In pass-through mode, start sending data
	1. Single connection :(+CIPMUX=0)
		AT+CIPSEND=<length>
	2. Multiple connections :(+CIPMUX=1)
		AT+CIPSEND=<link ID>,<length>
	3. For UDP transmission, you can set the remote IP address and remote IP port
		AT+CIPSEND=[<link	ID>,]<length> [,<remote	IP>,<remote	port>]
	*/
	ret = ESP8266_Send_AT_Cmd( "AT+CIPSEND", "OK", ">", 500 );
	
	return ret;
}

/** 
* @description: Controls ESP8266 to send strings 
* @param   {FunctionalState}     enumEnUnvarnishTx : Whether to enable the transparent transmission mode , DISABLE or ENABLE
* @param   {char *}              pStr              : String to be sent
* @param   {uint32_t}            ulStrLength       : The length of the string to be sent
* @param   {ENUM_ID_NO_TypeDef}  ucId              : Equipment connection number
* @return  {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cStr [20];
	
	if ( enumEnUnvarnishTx == ENABLE)
	{
		ret = ESP8266_USART ( "%s", pStr );
	}
	else 
	{
		if ( ucId < 5 )
        {
			sprintf( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );
		}
		else
        {
		    sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		}
		
	    ESP8266_Send_AT_Cmd ( cStr, "> ", 0, 1000 );
		
		ret = ESP8266_Send_AT_Cmd ( pStr, "SEND OK", 0, 1000 );
	}


	return ret;
}

/** 
* @description: ESP8266 exits transparent transmission mode
* @param   {void}  
* @return  {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_ExitUnvarnishSend ( void )
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	HAL_Delay(1000);
	
	ret = ESP8266_USART( "+++" );
	
	HAL_Delay(500);
	
	return ret;
}

/** 
* @description: The ESP8266 detects the network connection status
* @param   {void}  
* @return  {uint8_t}:
*          0        : Failed to obtain status
*          2        : Get an IP address
*          3        : Establish a connection
*          4        : Lost connection
* @author: leeqingshui 
*/
uint8_t ESP8266_Get_LinkStatus ( void )
{
	uint8_t ret = (uint8_t)0;
	
	/* AT+CIPSTATUS - Queries network connection information
	AT+CIPSTATUS : <link ID>,<type>,<remote	IP>,<remote	port>,<local port>,<tetype>
	*/
	if ( ESP8266_Send_AT_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:2\r\n" ) )
            return (uint8_t)2;

        else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:3\r\n" ) )
			return (uint8_t)3;

        else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:4\r\n" ) )
            return (uint8_t)4;       
	}

	return ret;
}

/** 
* @description: ESP8266 exits transparent transmission mode
* @param   {uint8_t } LinkID      : Connection ID. Currently, only 0 is supported
* @param   {uint8_t } scheme      : Connection mode: select MQTT over TCP and set it to 1
*                                   1: MQTT over TCP 
*									2: MQTT over TLS(no certificate verify)
*									3: MQTT over TLS(verify server certificate)
*									4: MQTT over TLS(provide client certificate)
*									5: MQTT over TLS(verify server certificate and provide client certificate)
*									6: MQTT over WebSocket(based on TCP)
*									7: MQTT over WebSocket Secure(based on TLS, no certificate verify)
*									8: MQTT over WebSocket Secure(based on TLS, verify server certificate)
*									9: MQTT over WebSocket Secure(based on TLS, provide client certificate)
*									10: MQTT over WebSocket Secure(based on TLS, verify server certificate and provide client certificate)
* @param   {char *  } pClient_Id  : The corresponding MQTT client ID is used to identify the client. The maximum length of the ID is 256 bytes
*                                   Set NULL
* @param   {char *  } pUserName   : Username used to log in to MQTT Broker, maximum 64 bytes
* @param   {char *  } PassWord    : Password for logging into MQTT Broker, up to 64 bytes
* @param   {uint8_t } cert_key_ID : Certificate ID. Currently, one cert certificate is supported. The parameter is 0
* @param   {uint8_t } CA_ID       : CA ID. Currently, one SET of CA certificates is supported
* @param   {uint32_t} uint32_t    : Resource path, maximum 32 bytes
* @return  {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
__weak uint8_t ESP8266_MQTTUSERCFG( char * pClient_Id, char * pUserName,char * PassWord)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cCmd [120];

	sprintf ( cCmd, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"", pClient_Id,pUserName,PassWord );
    
	/*
	AT+MQTTUSERCFG=<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">
	AT+MQTTUSERCFG=0,1,"NULL","test_0&h5fb4XQhOoD","B210DD9F749288AE7F8A485F0ABA0C0C05677A91",0,0,""
	*/
	ret =  ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 500 );

	return ret;
}

/** 
* @description: Set the MQTT client ID : Generated by aliyun Iot platform configuration tool
* @param   {char *  } pClient_Id  : The corresponding MQTT client ID is used to identify the client. The maximum length of the ID is 256 bytes
* @return  {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
__weak uint8_t ESP8266_MQTTSETCLIENTID( char * p_Generate_Client_Id )
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cCmd [120];

	sprintf ( cCmd, "AT+MQTTCLIENTID=0,\"%s\"", p_Generate_Client_Id );
    
	ret =  ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 500 );

	return ret;
}

/** 
* @description: Connect to the specified MQTT server
* @param   {char *     } Ip        : IP address
* @param   {uint32_t   } Num       : Port number, usually 1883
* @param   {uint8_t    } LinkID    : Connection ID. Currently, only 0 is supported
* @param   {uint8_t    } reconnect : Whether to reconnect MQTT. If the value is set to 1, memory resources are consumed
* @return  {uint8_t    }           : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
__weak uint8_t ESP8266_MQTTCONN( char * Ip, uint32_t  Num)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cCmd [120];
	
	/* 
	AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
	
	Parameters : 
      
	LinkID: Currently, 0 is supported
	Host: connects to the MQTT Broker domain name. Maximum 128 bytes
	Port: Connects to the MQTT Broker port. The maximum value is 65535
	Reconnect: Whether to reconnect MQTT. If the value is set to 1, it consumes a lot of memory resources
	*/
	sprintf ( cCmd,"AT+MQTTCONN=0,\"%s\",%d,0", Ip,Num);
	
	ret = ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 500 );
	
    return ret;
}

/** 
* @description: Subscribe to MQTT topics that are specified for connection, and you can repeatedly define different topics
* @param   {char *   } topic  : Subscribe to the topic name
* @param   {uint32_t } Qos    : Publish quality of Service , Generally, the value is 0. Set it to 1
* @param   {uint8_t  } LinkID : Connection ID. Currently, only 0 is supported
* @return  {uint8_t  }        : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
__weak uint8_t ESP8266_MQTTSUB(char * Topic)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cCmd [120];
	/*
	AT+MQTTSUB=<LinkID>,<"topic">,<qos>

	When an MQTT message for the topic subscription is received, the message content is printed in the following format :
	
	+MQTTSUBRECV:<LinkID>,<"topic">,<data_length>,data
	
	If you SUBSCRIBE to an ALREADY subscribed topic, still SUBSCRIBE unconditionally to the MQTT broker, and the Log port prints ALREADY SUBSCRIBE
	
	*/
	sprintf ( cCmd, "AT+MQTTSUB=0,\"%s\",1",Topic );
	
	ret = ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 500 );

    return ret;
}

/** 
* @description: Publish data on LinkID via topic, where data is a string message
* @param   {char *   } Topic  : Subscribe to the topic name
* @param   {char *   } temp   : String information
* @param   {uint8_t  } LinkID : Connection ID. Currently, only 0 is supported
* @param   {uint32_t } Qos    : Publish quality of Service , Generally, the value is 0. Set it to 1
* @param   {uint32_t } Retain : Reserving the message flag bit
* @return  {uint8_t  }        : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
__weak uint8_t ESP8266_MQTTPUB( char * Topic,char *temp)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cCmd [120];
	
	/*
	AT+MQTTPUB=<LinkID>,<"topic">,<"data">,<qos>,<retain>

	Parameters : 
	
	
	LinkID: Currently, 0 is supported
	Topic: Publish topic, up to 64 bytes
	Data: publish a message. Data cannot contain \0. Make sure that the entire AT+MQTTPUB does not exceed the maximum length limit of the AT instruction
	Qos: publishes the quality of service. The value can be 0,1, or 2. The default value is 0
	Retain: release retain
	
	*/
    sprintf (cCmd, "AT+MQTTPUB=0,\"%s\",\"%s\",1,0", Topic ,temp);
	
	ret = ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 1000 ) ;
	
	return ret;
}

/** 
* @description: Close the connection of the MQTT Client to LinkID and release the internal resources 
* @param   {void   } 
* @param   {uint8_t  } LinkID : Connection ID. Currently, only 0 is supported
* @return  {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
__weak uint8_t ESP8266_MQTTCLEAN(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	char cCmd [120];
	
	/*
	AT+MQTTCLEAN=<LinkID>
	*/
	sprintf ( cCmd, "AT+MQTTCLEAN=0");
	
	ret = ESP8266_Send_AT_Cmd( cCmd, "OK", NULL, 500 );
	
	return ret;
}

/** 
* @description: ESP8266 Sends a character string 
* @param   {char *   } Topic  : Subscribe to the topic name
* @param   {char *   } temp   : String information
* @return  {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
__weak uint8_t MQTT_SendString(char * pTopic,char *temp)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	ret = ESP8266_MQTTPUB(pTopic,temp);
	
	HAL_Delay(1000);
	
	return ret;
}

/** 
* @description: ESP8266 detects the MQTT connection status
* @param   {void}  
* @return  {uint8_t}:
*          0        : The connection is not initialized
*          2        : Has been set MQTTCONNCFG
*          3        : Connection disconnected
*          4        : Established connection
*          5        : Connected, but not subscribed to topic
*          6        : Already connected and subscribed to topic
* @author: leeqingshui 
*/
uint8_t ESP8266_Get_MQTTStatus ( void )
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;

	/*
	AT+MQTTCONN?
	
	State: indicates the current status of the MQTT. The status is described as follows:
		0: the connection is not initialized
		1: MQTTUSERCFG has been set
		2: MQTTCONNCFG has been configured
		3: The connection is disconnected
		4: The connection has been established
		5: Connected but not subscribed to topic
		6: Connected and subscribed to topic
	*/
	if ( ESP8266_Send_AT_Cmd( "AT+MQTTCONN?", "OK", 0, 500 ) )
	{
		if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:0\r\n" ) )
            return (uint8_t)0;

        else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:2\r\n" ) )
			return (uint8_t)2;

        else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:3\r\n" ) )
            return (uint8_t)3;       
		 
		else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:4\r\n" ) )
			return (uint8_t)4;

        else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:5\r\n" ) )
            return (uint8_t)5; 
		
		else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:6\r\n" ) )
			return (uint8_t)6;	
	}

	return ret;
}

/** 
* @description: The MQTT sends heartbeat packets to maintain the connection
* @param   {void}  
* @return  {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t ESP8266_Send_Heart( void )
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;

	ESP8266_USART("%d%d",0xc0,0x00);
	
	return ret;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++Static functions++++++++++++++++++++++++++++++++++++++++++++++++++*/

/** 
* @description: Enable serial port interruption
* @param {void} 
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
static uint8_t Cmd_TIM3_IT(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	if(HAL_TIM_Base_Start_IT(&htim3)!=HAL_OK)
    {
		ret = (uint8_t)OPERATION_ERROR;
	}
	
    return ret;
}


/** 
* @description: Enable serial port 1 IT to receive messages:
*				1. Enable serial port receive interrupt
*				2. Enable serial port idle interrupt
* @param {void} 
* @return {int} ret: cmd uart1_it success , return OPERATION_SUCCESS
* @author: leeqingshui 
*/
static uint8_t Cmd_UART1_IT(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	/* Enable serial port receive interrupt, generated once every received byte */
	if(HAL_UART_Receive_IT(&huart1,&RxBuffer,1)!=HAL_OK)
	{
		ret = (uint8_t)OPERATION_ERROR;
	}
	
	/* Enable serial port idle interrupt, generated once every data frame received */
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE); 
	
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

  }
}









