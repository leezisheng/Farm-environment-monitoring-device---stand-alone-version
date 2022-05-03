/**
  ******************************************************************************
  * File Name          : Connect_Iot.h
  * Description        : This file contains all the functions prototypes for
  *                      the Connect_Iot.c
  *
  * Triplet of devices used for testing : 
  *	{
  *	  "ProductKey": "h5fb4XQhOoD",
  *	  "DeviceName": "test_0",
  *	  "DeviceSecret": "e6d45170fa27bfebbed1d86324362a81"
  *	}
  *
  * MQTT parameter :
  * 				IP 					: h5fb4XQhOoD.iot-as-mqtt.cn-shanghai.aliyuncs.com:1883
  *					ClientIld 			: 123456|securemode=3,signmethod=hmacsha1|
  *					Username			: test_0&h5fb4XQhOoD
  *					Password			: B210DD9F749288AE7F8A485F0ABA0C0C05677A91
  *					generate ClientIld 	: 123456deviceNametest_0productKeyh5fb4XQhOoD
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONNECT_IOT_TASK_H
#define __CONNECT_IOT_TASK_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Common macro definitions---------------------------------------------------*/

/* Serial port 1 macro definitions */

/* WIFI Parameter Setting */
/* The name of the WIFI  */
#define WIFI_NAME               		"nova7"
/* WIFI password */
#define WIFI_KEY    					"1234567890"

/* SNTP server macro definition */
#define SNTP_SERVER_0					"cn.ntp.org.cn"
#define SNTP_SERVER_1					"ntp.sjtu.edu.cn"
#define SNTP_SERVER_2					"us.pool.ntp.org"

/* MQTT protocol parameters */
/* generate Client ID */
#define MQTT_User_client_id   			"123456|securemode=3\\,signmethod=hmacsha1|"  
/* Username */
#define MQTT_Username					"test_0&h5fb4XQhOoD"
/* Password */
#define MQTT_Password					"B210DD9F749288AE7F8A485F0ABA0C0C05677A91" 
/* Server IP Address */
#define MQTT_Server_IP     				"h5fb4XQhOoD.iot-as-mqtt.cn-shanghai.aliyuncs.com"  
/* Server port number */
#define MQTT_Server_PORT   				1883    
/* Subscribe topic */
#define MQTT_Server_Topic  				"topic"    
/* Client ID */
#define CLIENT_ID						"123456"

/* TCP protocol Parameters */
/* Server IP Address */
#define User_ESP8266_TCPServer_IP		"iot-as-mqtt.cn-shanghai.aliyuncs.com" 
/* Server port number */
#define User_ESP8266_TCPServer_PORT   	"8888" 

/* The equipment parameters:Device triad */
#define DEVICE_NAME      				 "test_0"  
#define DEVICE_SECRET     				"e6d45170fa27bfebbed1d86324362a81"
#define PRODUCT_KEY 					"h5fb4XQhOoD"


/* Data structure declaration-------------------------------------------------*/



/* Function declaration-------------------------------------------------------*/
/* Use at-MQTT instructions to connect ali Cloud platform */
uint8_t Connct_aliyun_iot(void);




#ifdef __cplusplus
}
#endif
#endif /*__CONNECT_IOT_TASK_H*/

