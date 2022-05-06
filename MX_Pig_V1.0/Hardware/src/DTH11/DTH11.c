/**
  ******************************************************************************
  * File Name          : DTH11.c
  * Description        : This file provides code for the configuration
  *                      of DTH11.
  *
  * DTH11 is single-bus communication. PC2 port is used to connect to DTH11_Data
  *
  * The functions in this file fall into two broad categories:
  * Initialization function (internal functions used include: control port working mode function, DTH11 reset function, 
  * DTH11 check return signal function, US level delay function)
  *
  * Read data function
  ******************************************************************************
 */
  
/* Includes ------------------------------------------------------------------*/
#include "DTH11.h"
/* External function declaration----------------------------------------------*/

/* Private macro definitions--------------------------------------------------*/

/* Global variable------------------------------------------------------------*/

/* Static function definition-------------------------------------------------*/

/* Imprecise microsecond level delay function */
static void Delay_us(uint32_t us);
/* Set the working mode of the DTH11 port to push-pull output mode */
static void DHT11_IO_OUT (void);
/* Set the working mode of the DTH11 port to input mode */
static void DHT11_IO_IN (void);
/* DHT11 port resets and sends a start signal */
static void DHT11_RST (void);
/* Wait for DHT11 to respond. 0: DHT11 is not detected. 1: DHT11 is successfully received */
static uint8_t Dht11_Check(void);
/* Read a bit from DHT11 */
static uint8_t Dht11_ReadBit(void);
/* Read a byte from DHT11 */
static uint8_t Dht11_ReadByte(void);

/* Function definition--------------------------------------------------------*/

/** 
* @description: Set the working mode of the DTH11 port to push-pull output mode
* @param  {void} 
* @return {void}
* @author: leeqingshui 
*/
static void DHT11_IO_OUT (void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DTH11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DTH11_GPIO_Port, &GPIO_InitStruct);
}

/** 
* @description: Set the working mode of the DTH11 port to input mode
* @param  {void} 
* @return {void}
* @author: leeqingshui 
*/
static void DHT11_IO_IN (void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DTH11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DTH11_GPIO_Port, &GPIO_InitStruct);
}

/** 
* @description: DHT11 port resets and sends a start signal
* @param  {void} 
* @return {void}
* @author: leeqingshui 
*/
static void DHT11_RST (void)
{
	DHT11_IO_OUT();
	DHT11_PIN_RESET;
	// Pull down at least 18ms
	HAL_Delay(20);
	DHT11_PIN_SET;
	// Raise the host by 20 to 40us
	Delay_us(30);
}

/** 
* @description: DHT11 port resets and sends a start signal
* @param  {void} 
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS , else return OPERATION_ERROR Indicates that no DHT11 reply is detected
* @author: leeqingshui 
*/
static uint8_t Dht11_Check(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	uint8_t retry=0;
	
	// Set the working mode of the DTH11 port to input mode
	DHT11_IO_IN();
	
	// DHT11 will drop 40~80us
	while (DHT11_READ_IO && retry<100)
	{
        retry++;
        Delay_us(1);
    }
	if(retry>=100)
		return ret = (uint8_t)OPERATION_ERROR;
	
	
	// DHT11 will be raised again by 40~80us
	while (!DHT11_READ_IO && retry<100)
	{
        retry++;
        Delay_us(1);
    }
	if(retry>=100)
		return ret = (uint8_t)OPERATION_ERROR;
	
	return ret;
}

/** 
* @description: DTH11 initialization 
* @param  {void} 
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS , else return OPERATION_ERROR
* @author: leeqingshui 
*/
uint8_t DHT11_Init (void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	// DHT11 port resets and sends a start signal
	DHT11_RST();

	ret = Dht11_Check(); 
	
	/* The delay after DTH11 initialization is longer than 1s */ 
	HAL_Delay(1000);

	return ret;
}

/** 
* @description: Read a bit from DHT11
* @param  {void} 
* @return {uint8_t} : Data bits, 0 or 1
* @author: leeqingshui 
*/
static uint8_t Dht11_ReadBit(void)
{
	uint8_t retry=0;
	
	while(DHT11_READ_IO && retry<100)
    {
		// Wait to turn low
        retry++;
        Delay_us(1);
    }
    retry=0;
	
	while(!DHT11_READ_IO && retry<100)
    {
		// Wait to turn high
        retry++;
        Delay_us(1);
    }
	
	// Wait for 40us to judge high and low level, i.e. data 1 or 0
	Delay_us(40);
	
	if(DHT11_READ_IO)
        return 1; 
    else 
        return 0;
	
}

/** 
* @description: Read a byte from DHT11
* @param  {void} 
* @return {uint8_t} : Data byte
* @author: leeqingshui 
*/
static uint8_t Dht11_ReadByte(void)
{
    uint8_t i,dat;
    dat=0;
	
    for (i=0;i<8;i++)
    {
        dat<<=1;
        dat|=Dht11_ReadBit();
    }
	
    return dat;
}

/** 
* @description: Read temperature and humidity data once
* @param  {uint8_t} ptemp : Pointer to the temperature and humidity data array , Temperature value (decimal, range :0~50°)
* @param  {uint8_t} phumi : Humidity value (decimal, 20%-90%)
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS , else return OPERATION_ERROR
* @author: leeqingshui 
*/
uint8_t DHT11_ReadData(uint8_t *ptemp , uint8_t *phumi)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;

	uint8_t buf[5];
	uint8_t i;
	
	// The DHT11 port resets and sends a start signal
    DHT11_RST();
	
	if(Dht11_Check()==(uint8_t)OPERATION_SUCCESS)
    { 
        // Wait for DHT11 to respond
        for(i=0;i<5;i++)
        {
			// Read 5 bits of data
            buf[i]=Dht11_ReadByte(); 
        }
		
		// Data verification
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
        {	
			// Put the humidity value into pointer 1
            *phumi=buf[0]; 
			// Put the temperature value into pointer 2
            *ptemp=buf[2]; 
        }
    }
	else 
        return (uint8_t)OPERATION_ERROR;

	return ret;
}


/** 
* @description: Test whether the delay function is accurate
* @param  {uint32_t} us : Microsecond delay
* @return {void}:
* @author: leeqingshui 
*/
void Test_Delay_us(uint32_t us)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	Delay_us(us);
}

/** 
* @description: Implement imprecise microseconds delay
* @param  {uint32_t} us : Microsecond delay
* @return {void}:
* @author: leeqingshui 
*/
static void Delay_us(uint32_t us)
{
	uint32_t delay = (HAL_RCC_GetHCLKFreq() / DELAY_COEFFICIENT * us);
	
    while (delay--)
	{
		;
	}
}

/** 
* @description: Get the temperature value
* @param  {void} 
* @return {uint8_t}: temp value
* @author: leeqingshui 
*/
uint8_t DTH11_GetTemp(void)
{
	uint8_t Temp = 0;
	
	DHT11_ReadData(&Temp,NULL);
	
	return Temp;
}

/** 
* @description: Get the temperature value
* @param  {void} 
* @return {uint8_t}: humidity value
* @author: leeqingshui 
*/
uint8_t DTH11_GetHumi(void)
{
	uint8_t Humi = 0;
	
	DHT11_ReadData(NULL,&Humi);
	
	return Humi;
}

