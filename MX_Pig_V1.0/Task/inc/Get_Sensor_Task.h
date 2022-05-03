/**
  ******************************************************************************
  * File Name          : Get_Sensor_Task.h
  * Description        : This file contains all the functions prototypes for
  *                      the Get_Sensor_Task.c
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GET_SENSOR_TASK_H
#define __GET_SENSOR_TASK_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Common macro definitions---------------------------------------------------*/

/* Data structure declaration-------------------------------------------------*/
/* Sensor information structure */
typedef struct 
{
	/* Gas concentration */
	float NH3_CC;
	float H2S_CC;
	float CO_CC;
	float CO2_CC;
	
	/* Equipment voltage */
	float Device_V;
	
	/* Indoor temperature and humidity */
	uint8_t Temp;
	uint8_t Humi;
	
}SENSOR_Information_Struct;

/* Function declaration-------------------------------------------------------*/
/* Update the value of the sensor obtained */
uint8_t Get_Sensor_Info(void);


#ifdef __cplusplus
}
#endif
#endif /*__GET_SENSOR_TASK_H*/

