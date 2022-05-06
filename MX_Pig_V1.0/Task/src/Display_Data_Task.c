/**
  ******************************************************************************
  * File Name          : Display_Data_Task.c
  * Description        : This file provides code for the configuration
  *                      of Display_Data_Task.
  * Task Introduction:
  *
  * OLED display task (Display_Data_Task) : Display and update the collected sensor information and network connection data on 
  *	the OLED screen. The data comes from two message queues, Internet_Status_Queue and Sensor_Data_Queue. The main peripherals used 
  *	include: 1.3-inch OLED display, relevant driver layer file in OLed.c /.h
  * 
  * One structure is used here: OLED_Display_struct holds gas concentration data and network state data
  *
  * Data is mainly obtained from Sensor_Data_Queue and Internet_Status_Queue
  *
  * The functions of this file include: display data, display operation status, and task functions
  ******************************************************************************
 */
  
/* Includes ------------------------------------------------------------------*/
#include "Display_Data_Task.h"
#include "OLED.h"
#include "UART_Printf.h"
#include <math.h>

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
/* External function declaration----------------------------------------------*/

/* Private macro definitions--------------------------------------------------*/

/* Global variable------------------------------------------------------------*/

/* OLED display structure */
Display_Struct OLED_Display_struct = {0};
/*
Because the DTH11 module is damaged, temperature and humidity data cannot be obtained. Therefore, 
the simulation value of temperature and humidity data is generated using random number seeds
*/
static uint32_t rand = 100;
/* Whether the device is successfully connected to alibaba Cloud Iot platform: it is determined that 
   the device is operated independently and the data is displayed locally; Or upload the data to the cloud 
*/
extern uint8_t is_connect_iot;
/* Handle to the gas sensor data message queue */
extern QueueHandle_t Sensor_Data_Queue;
/* Handle to the network status message queue */
extern QueueHandle_t Internet_Status_Queue;

/* Static function definition-------------------------------------------------*/

/* Function definition--------------------------------------------------------*/

/** 
* @description: Displays initialization information
* @param  {void     } 
* @return {uint8_t  } : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t Display_Operation_Status(uint8_t operation_name[] ,uint8_t status)
{
	OLED_CLS();
	
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	ret = OLED_ShowStr(10, 1, operation_name, 1) && ret;
	
	if(status == (uint8_t)OPERATION_SUCCESS)
	{
		ret = OLED_ShowStr(30, 6, "success", 1) && ret;
	}
	else
	{
		ret = OLED_ShowStr(30, 6, "fail", 1) && ret;
	}
	
	return ret;
}

/** 
* @description: Displays the data collected by the sensor and network information
* @param  {void     } 
* @return {uint8_t  } : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t Display_Data_Status(void)
{
	OLED_CLS();
	
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	ret = OLED_ShowStr(2,0,"CO2(ppm):",1)	&&
		  OLED_ShowStr(2,1,"CO(ppm):", 1)	&&
		  OLED_ShowStr(2,2,"H2S(ppm):",1)	&&
		  OLED_ShowStr(2,3,"NH3(ppm):",1)	&&
	      OLED_ShowStr(2,4,"Humi:(%):",1)	&&
		  OLED_ShowStr(2, 5,"Temp(C):",1)	&&
		  OLED_ShowStr(2,6,"Internet:",1)	&&
	      OLED_ShowStr(2,7,"MQTT:",1);
	
	/*
	Because the DTH11 module is damaged, temperature and humidity data cannot be obtained. Therefore, 
	the simulation value of temperature and humidity data is generated using random number seeds
	
	If DTH11 is working, remove this code
	*/
	rand = rand+(uint32_t)20000*sin(0.2*rand)+(uint32_t)10000*cos(0.1*rand)+(uint32_t)10000*cos(rand*rand*0.1);
	rand = (uint32_t)rand/4000;
	if(rand>63355)rand=0;
	
	OLED_Display_struct.sensor_info_struct.Temp = 24+rand;
	OLED_Display_struct.sensor_info_struct.Humi = 35+rand;
	
	ret = OLED_ShowNum(100,0 ,(uint32_t)OLED_Display_struct.sensor_info_struct.CO2_CC/10   ,2,1)                  &&
		  OLED_ShowNum(110,0 ,(uint32_t)OLED_Display_struct.sensor_info_struct.CO2_CC%10   ,2,1)                  &&
		  OLED_ShowNum(100,1 ,(uint32_t)OLED_Display_struct.sensor_info_struct.CO_CC/10    ,2,1)	              &&
		  OLED_ShowNum(110,1 ,(uint32_t)OLED_Display_struct.sensor_info_struct.CO_CC%10    ,2,1)	              &&
		  OLED_ShowNum(100,2 ,(uint32_t)OLED_Display_struct.sensor_info_struct.H2S_CC/10   ,2,1)	              &&
		  OLED_ShowNum(110,2 ,(uint32_t)OLED_Display_struct.sensor_info_struct.H2S_CC%10   ,2,1)	              &&
		  OLED_ShowNum(100,3 ,(uint32_t)OLED_Display_struct.sensor_info_struct.NH3_CC/10   ,2,1)	              &&
		  OLED_ShowNum(110,3 ,(uint32_t)OLED_Display_struct.sensor_info_struct.NH3_CC%10   ,2,1)	              &&
	      OLED_ShowNum(100,4 ,(uint32_t)OLED_Display_struct.sensor_info_struct.Humi/10     ,2,1)	              &&
		  OLED_ShowNum(110,4 ,(uint32_t)OLED_Display_struct.sensor_info_struct.Humi%10     ,2,1)	              &&
		  OLED_ShowNum(100,5 ,(uint32_t)OLED_Display_struct.sensor_info_struct.Temp/10     ,2,1)	              &&
		  OLED_ShowNum(110,5 ,(uint32_t)OLED_Display_struct.sensor_info_struct.Temp%10     ,2,1)	              &&
	      OLED_ShowNum(100,6 ,(uint32_t)OLED_Display_struct.internect_status_struct.Internet_Link_Status ,2,1)	  &&
	      OLED_ShowNum(100,7 ,(uint32_t)OLED_Display_struct.internect_status_struct.MQTTS_Link_tatus     ,2,1)    &&
		  ret;
	
	return ret;
}

/** 
* @description: Displays the data task function
* @param  {void*  } 
* @return {void   } 
* @author: leeqingshui 
*/
void Display_Data_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;
	
	while(1)
	{
	    /* Receiving sensor data */
		xReturn = xQueuePeek( 	Sensor_Data_Queue,  
							    &OLED_Display_struct.sensor_info_struct,     
                                500); 
		/* Receiving sensor data succeeded */
		if(pdPASS == xReturn)
			printf("Display task : Receiving sensor data succeeded\r\n");
		
		/* 
		If the cloud platform is successfully connected, create a task to connect to the cloud platform. 
		Otherwise, the standalone mode will be started and the data will be displayed on the local OLED screen
		*/
		if(is_connect_iot)
		{
	    /* Receiving network status */
		xReturn = xQueuePeek( 	Internet_Status_Queue,  
							    &OLED_Display_struct.internect_status_struct,     
                                500); 
		/* Receiving network status succeeded */
		if(pdPASS == xReturn)
			printf("Display task : network status succeeded\r\n");
	    }
		
	    /* Update the data */
		Display_Data_Status();
		
		printf("Displays the data task Running\r\n");
	    vTaskDelay(1000);
	}
}
