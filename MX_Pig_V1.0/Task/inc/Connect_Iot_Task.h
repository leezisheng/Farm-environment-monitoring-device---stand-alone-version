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
#include "Connect_Iot_Task.h"
/* Common macro definitions---------------------------------------------------*/

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
/* Event reporting */
#define EVENT_ID                         "NULL"
/* Service invocation */
#define SERVER_ID                        "NULL"

/* TCP protocol Parameters */
/* Server IP Address */
#define User_ESP8266_TCPServer_IP		"iot-as-mqtt.cn-shanghai.aliyuncs.com" 
/* Server port number */
#define User_ESP8266_TCPServer_PORT   	"8888" 

/* The equipment parameters:Device triad */
#define DEVICE_NAME      				 "test_0"  
#define DEVICE_SECRET     				"e6d45170fa27bfebbed1d86324362a81"
#define PRODUCT_KEY 					"h5fb4XQhOoD"

/* MQTT server parameters */
/* Property setting macro definition : /sys/h5fb4XQhOoD/${deviceName}/thing/service/property/set */
#define PROPERTY_SET					"/sys/h5fb4XQhOoD/"##DEVICE_NAME##"/thing/service/property/set"

/* 	Property to report macro definition : 
	/sys/h5fb4XQhOoD/${deviceName}/thing/event/property/post 
	/sys/h5fb4XQhOoD/${deviceName}/thing/event/property/post_reply
*/
#define PROPERTY_POST					"/sys/h5fb4XQhOoD/"##DEVICE_NAME##"/thing/event/property/post"
#define PROPERTY_POST_REPLY				"/sys/h5fb4XQhOoD/"##DEVICE_NAME##"/thing/event/property/post_reply"

/*	Event reporting macro definition
	/sys/h5fb4XQhOoD/${deviceName}/thing/event/${tsl.event.identifier}/post
	/sys/h5fb4XQhOoD/${deviceName}/thing/event/${tsl.event.identifier}/post_reply
*/
#define EVENT_POST						"/sys/h5fb4XQhOoD/"##DEVICE_NAME##"/thing/event/"##EVENT_ID##"/post"
#define EVENT_POST_REPLY				"/sys/h5fb4XQhOoD/"##DEVICE_NAME##"/thing/event/"##EVENT_ID##"/post_reply"

/*	Service invocation macro definition
	/sys/h5fb4XQhOoD/${deviceName}/thing/service/${tsl.service.identifier}
	/sys/h5fb4XQhOoD/${deviceName}/thing/service/${tsl.service.identifier}_reply
*/
#define SERVER_POST						"/sys/h5fb4XQhOoD/"##DEVICE_NAME##"/thing/service/"SERVER_ID
#define SERVER_POST_REPLY				"/sys/h5fb4XQhOoD/"##DEVICE_NAME##"/thing/service/"SERVER_ID"_reply"

/* Data structure declaration-------------------------------------------------*/

/* Network and server connection status */
typedef struct 
{
	uint8_t Internet_Link_Status;
	uint8_t MQTTS_Link_tatus;
}Internat_Connect_Information_Struct;

/* Function declaration-------------------------------------------------------*/
/* Use at-MQTT instructions to connect ali Cloud platform */
uint8_t Connct_aliyun_iot(void);
/* Gets the network connection status */
uint8_t Get_InternetLink_Status(void);
/* Gets the MQTT connection server status */
uint8_t Get_ServerLink_Status(void);
/* Upload data to the server */
uint8_t UploadData_To_Server(void);
/* Fixed heartbeat packets are sent to the server */
uint8_t Send_Heart_Server(void);

#ifdef __cplusplus
}
#endif
#endif /*__CONNECT_IOT_TASK_H*/

