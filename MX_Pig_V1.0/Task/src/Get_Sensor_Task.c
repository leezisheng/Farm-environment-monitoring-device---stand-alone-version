/**
  ******************************************************************************
  * File Name          : Get_Sensor_Task.c
  * Description        : This file provides code for the configuration
  *                      of Get_Sensor_Task.
  *
  * Task Introduction:
  *
  *	Sensor information collection task (Get_Sensor_Task) - Collects temperature and humidity information, ammonia concentration, 
  *	hydrogen sulfide concentration, ozone concentration, and carbon monoxide concentration. The following peripherals are used: 
  *	ADC.C /. H and DTH11. C /. H, and send data to Sensor_Data_Queue message queue
  * 
  * The collected data is stored in the Sensor_Info_Struct structure
  ******************************************************************************
 */
  
/* Includes ------------------------------------------------------------------*/
#include "Get_Sensor_Task.h"
#include "ADC_Operation.h"
#include "DTH11.h"
#include "UART_Printf.h"

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
/* External function declaration----------------------------------------------*/

/* Private macro definitions--------------------------------------------------*/

/* Global variable------------------------------------------------------------*/

/* A structure used to store sensor information */
SENSOR_Information_Struct Sensor_Info_Struct = {0};
/* Whether the device is successfully connected to alibaba Cloud Iot platform: it is determined that 
   the device is operated independently and the data is displayed locally; Or upload the data to the cloud 
*/
extern uint8_t is_connect_iot;
/* Handle to the gas sensor data message queue */
extern QueueHandle_t Sensor_Data_Queue;

/* Static function definition-------------------------------------------------*/

/* Function definition--------------------------------------------------------*/

/** 
* @description: Update the value of the sensor obtained 
* @param  {void} 
* @return {uint8_t } : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t Get_Sensor_Info(void)
{
	printf("The system starts to collect environment data\r\n");
	
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	ret = ADC_Get_Gas() && ret;
	
	Sensor_Info_Struct.NH3_CC = ADC_Get_NH3();
	Sensor_Info_Struct.H2S_CC = ADC_Get_H2S();
	Sensor_Info_Struct.CO_CC  = ADC_Get_CO();
	Sensor_Info_Struct.CO2_CC = ADC_Get_CO2();
	
	Sensor_Info_Struct.Device_V = ADC_Get_Voltage();
	
	/* DTH11 works at an interval of 900ms */
	Sensor_Info_Struct.Temp   = DTH11_GetTemp();
	
	/* DTH11 works at an interval of 900ms */
	Sensor_Info_Struct.Humi   = DTH11_GetHumi();
	
	Sensor_Info_Struct.Temp   = 24;
	Sensor_Info_Struct.Humi   = 31;

	return ret;
}

/** 
* @description: get sensor data task function
* @param  {void*} 
* @return {void } 
* @author: leeqingshui 
*/
void Get_Sensor_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;
	
	while(1)
	{
		/* Update the data */
		Get_Sensor_Info();
		
		/* Sends sensor data to the message queue */
	    xReturn = xQueueSend(	Sensor_Data_Queue, 
								&Sensor_Info_Struct,
								500);       
		/* Sending sensor data succeeded. Procedure */
        if(pdPASS == xReturn)
			printf("Sending sensor data succeeded. Procedure\r\n");
		
		printf("get sensor data task Running\r\n");
		vTaskDelay(3000);
	}
}




