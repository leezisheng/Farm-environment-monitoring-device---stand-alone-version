/**
  ******************************************************************************
  * File Name          : Display_Data.h
  * Description        : This file contains all the functions prototypes for
  *                      the Display_Data.c
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_DATA_TASK_H
#define __DISPLAY_DATA_TASK_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Connect_Iot_Task.h"
#include "Get_Sensor_Task.h"
/* Common macro definitions---------------------------------------------------*/

/* Data structure declaration-------------------------------------------------*/

/* OLED display structure */
typedef struct 
{
	SENSOR_Information_Struct 				sensor_info_struct;
	Internat_Connect_Information_Struct		internect_status_struct;
}Display_Struct;

/* Function declaration-------------------------------------------------------*/
/* OLED displays operating status */
uint8_t Display_Operation_Status(uint8_t operation_name[] ,uint8_t status);
/* OLED displays data information */
uint8_t Display_Data_Status(void);
/* OLED displays data task function */
void Display_Data_Task(void* parameter);

#ifdef __cplusplus
}
#endif
#endif /*__DISPLAY_DATA_TASK_H*/

