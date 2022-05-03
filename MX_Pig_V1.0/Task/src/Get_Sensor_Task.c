/**
  ******************************************************************************
  * File Name          : Get_Sensor_Task.c
  * Description        : This file provides code for the configuration
  *                      of Get_Sensor_Task.
  ******************************************************************************
 */
  
/* Includes ------------------------------------------------------------------*/
#include "Get_Sensor_Task.h"
#include "ADC_Operation.h"
#include "DTH11.h"
#include "UART_Printf.h"
/* External function declaration----------------------------------------------*/

/* Private macro definitions--------------------------------------------------*/

/* Global variable------------------------------------------------------------*/
SENSOR_Information_Struct Sensor_Info_Struct = {0};

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






