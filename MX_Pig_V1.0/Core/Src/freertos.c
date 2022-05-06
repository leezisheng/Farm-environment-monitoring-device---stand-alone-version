/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* The basic function used by the task */
#include "Display_Data_Task.h"
#include "Get_Sensor_Task.h"
#include "Connect_Iot_Task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PM */

/* A message queue for receiving and sending sensor data */
/* Queue length: The maximum number of messages that the queue can contain */
#define  SENSOR_QUEUE_LEN    	1  
/* Message size: The size of each message in the queue, and the number of bytes for the corresponding data type of the message */
#define  SENSOR_QUEUE_SIZE   	sizeof(SENSOR_Information_Struct)  
/* A message queue for receiving and sending network connection status */
#define  INTERNET_QUEUE_LEN    	1  
#define  INTERNET_QUEUE_SIZE   	sizeof(Internat_Connect_Information_Struct)  
	
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Whether the device is successfully connected to alibaba Cloud Iot platform: it is determined that 
   the device is operated independently and the data is displayed locally; Or upload the data to the cloud 
*/
extern uint8_t is_connect_iot;

/*--------------------------Task handle declaration---------------------------*/

/* handle to Create tasks, and tasks for kernel objects */
osThreadId AppTaskCreate_Handle = NULL;
/* handle to Displays the data task */
osThreadId DisplayTask_TaskHandle = NULL;
/* handle to get sensor data task */
osThreadId GetSensorDataTask_TaskHandle = NULL;
/* handle to  connect iot task */
osThreadId ConnectIotTask_TaskHandle = NULL;

/*----------------------Message queue handle declaration----------------------*/

/* Handle to the gas sensor data message queue */
QueueHandle_t Sensor_Data_Queue =NULL;
/* Handle to the network status message queue */
QueueHandle_t Internet_Status_Queue =NULL;

/* USER CODE END Variables */
osThreadId Null_TaskHandle = NULL;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/*----------------------------User task function------------------------------*/

/* Create tasks, and tasks for kernel objects */
static void AppTaskCreate(void);
/* connect iot task function */
extern void Connect_Iot_Task(void* parameter);
/* get sensor data task function */
extern void Get_Sensor_Task(void* parameter);
/* Displays the data task function */
extern void Display_Data_Task(void* parameter);

/* USER CODE END FunctionPrototypes */
void Null_Test_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
   /* USER CODE BEGIN Init */
  
   /* Define a create message return quantity */
   BaseType_t xReturn = pdPASS;
   /* USER CODE END Init */

   /* USER CODE BEGIN RTOS_MUTEX */
   /* add mutexes, ... */
   /* USER CODE END RTOS_MUTEX */

   /* USER CODE BEGIN RTOS_SEMAPHORES */
   /* add semaphores, ... */
   /* USER CODE END RTOS_SEMAPHORES */

   /* USER CODE BEGIN RTOS_TIMERS */
   /* start timers, add new ones, ... */
   /* USER CODE END RTOS_TIMERS */

   /* USER CODE BEGIN RTOS_QUEUES */
   /* add queues, ... */
   /* USER CODE END RTOS_QUEUES */

   /* Create the thread(s) */
   /* definition and creation of Null_Task */
	osThreadDef(Null_Task, Null_Test_Task, osPriorityLow, 0, 128);
	Null_TaskHandle = osThreadCreate(osThread(Null_Task), NULL);

   /* USER CODE BEGIN RTOS_THREADS */
  
   /* This task is automatically destroyed after it is created and other tasks are completed */
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  
						 (const char*    )"AppTaskCreate",
						 (uint16_t       )512, 
					 	 (void*          )NULL,
						 (UBaseType_t    )1, 
						 (TaskHandle_t*  )&AppTaskCreate_Handle);
						 
    if(pdPASS == xReturn)
		printf("create appcreate task success\r\n");
   /* add threads, ... */
   /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Null_Test_Task */
/**
  * @brief  Function implementing the Null_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Null_Test_Task */
void Null_Test_Task(void const * argument)
{
  /* USER CODE BEGIN Null_Test_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Null_Test_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/** 
* @description: This task is automatically destroyed after it is created and other tasks are completed
* @param  {void*  } 
* @return {void   } 
* @author: leeqingshui 
*/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;
  
  /* Entry critical region */
  taskENTER_CRITICAL();           
  
  /* 
	If the cloud platform is successfully connected, create a task to connect to the cloud platform. 
	Otherwise, the standalone mode will be started and the data will be displayed on the local OLED screen
  */
  if(is_connect_iot)
  {
  /* Create Connect_Iot_Task task 

	 BaseType_t xTaskCreate(
							TaskFunction_t pxTaskCode,            // Task entry function
							const char * const pcName,            // The task name
							const uint16_t usStackDepth,          // Task stack size
							void * const pvParameters,            // Task entry function parameters
							UBaseType_t uxPriority,       		  // Task priority
							TaskHandle_t * const pxCreatedTask    // Task control block pointer
						   )
  */
  xReturn = xTaskCreate((TaskFunction_t )Connect_Iot_Task, 
                        (const char*    )"Connect_Iot_Task",
                        (uint16_t       )512,   
                        (void*          )NULL,	
                        (UBaseType_t    )2,	   
                        (TaskHandle_t*  )&ConnectIotTask_TaskHandle);
  if(pdPASS == xReturn)
    printf("create connect iot task success \r\n");
  }
  
  /* Create Get_Sensor_Task task */
  xReturn = xTaskCreate((TaskFunction_t )Get_Sensor_Task, 
                        (const char*    )"Get_Sensor_Task",
                        (uint16_t       )512,   
                        (void*          )NULL,	
                        (UBaseType_t    )2,	    
                        (TaskHandle_t*  )&GetSensorDataTask_TaskHandle);
  if(pdPASS == xReturn)
    printf("create get sensor data task success\r\n");
  
  /* Create Display_Data_Task task */
  xReturn = xTaskCreate((TaskFunction_t )Display_Data_Task, 
                        (const char*    )"Display_Data_Task",
                        (uint16_t       )1024,   
                        (void*          )NULL,	
                        (UBaseType_t    )3,	    
                        (TaskHandle_t*  )&DisplayTask_TaskHandle);
  if(pdPASS == xReturn)
    printf("create get sensor data task success\r\n");
  
  /* Create a message queue that delivers sensor data */
  Sensor_Data_Queue = xQueueCreate((UBaseType_t ) SENSOR_QUEUE_LEN,
                                   (UBaseType_t ) SENSOR_QUEUE_SIZE);
  if(NULL != Sensor_Data_Queue)
    printf("Succeeded in creating the sensor data message queue\r\n");
  
  /* 
	If the cloud platform is successfully connected, create a task to connect to the cloud platform. 
	Otherwise, the standalone mode will be started and the data will be displayed on the local OLED screen
  */
  if(is_connect_iot)
  {
  /* Create a message queue that delivers network connection status */
  Internet_Status_Queue = xQueueCreate((UBaseType_t ) INTERNET_QUEUE_LEN,
                                       (UBaseType_t ) INTERNET_QUEUE_SIZE);
  if(NULL != Internet_Status_Queue)
    printf("Succeeded in creating the internet status message queue\r\n");
  }
  
  /* Create and delete yourself after completing other tasks */
  vTaskDelete(AppTaskCreate_Handle); 
  
  /* Exit critical region */
  taskEXIT_CRITICAL();     
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
