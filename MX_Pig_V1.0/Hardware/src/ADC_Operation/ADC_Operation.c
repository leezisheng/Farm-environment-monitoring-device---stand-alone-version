/**
  ******************************************************************************
  * File Name          : key.c
  * Description        : This file provides code for the configuration
  *                      of GetGas by ADC1 .
  *
  * ADC Parameter Settings :
  *
  * 	ADCs_Common_Settings:
  *	    	            Mode             Independent mode
  *	
  * 	ADC_Settings:
  * 					Data Alignment   						Right alignment
  *                	    Scan Conversion  						Mode Enabled
  * 					Continuous Conversion Mode 				Disabled
  * 					Discontinuous Conversion Mode 			Enabled 
  * 					Number Of Discontinuous Conversions 	1
  *
  *     ADC_Regular_ConversionMode:
  *     				Enable Regular Conversions 				Enable
  * 					Number Of Conversion 					6
  * 					External Trigger Conversion 			Source Regular Conversion launched by software
  * 
  *     Rank 1~6:
  * 	Channel 4	Channel 6	Channel 7	Channel 8	Channel Temperature Sensor	Channel Vrefint
  * 	Sampling Time:	71.5 Cycles
  *
  * 	ADC_Injected_ConversionMode:
  * 					Enable Injected Conversions 			Disable
  *
  *		WatchDog:
  * 					Enable Analog WatchDog Mode 			false
  *
  * DMA configuration:
  * 	DMA request		ADC1
  * 	Stream			DMA1_Channel1
  * 	Direction		Peripheral To Memory
  * 	Priority		High
  *
  * 	ADC1: DMA1_Channel1 DMA request Settings:
  *						Mode: 									Circular 
  *			     		Peripheral Increment: 					Disable
  *						Memory Increment: 						Enable 
  *						Peripheral Data Width: 					Half Word
  *						Memory Data Width: 						Half Word
  *
  *  NVIC configuration:										Preenmption Priority
  * 	DMA1 channel1 global interrupt 							6
  *		ADC1 and ADC2 global interrupts							5
  *
  ******************************************************************************
 */
 
/* Includes ------------------------------------------------------------------*/
#include "ADC_Operation.h"
#include "adc.h"
#include "dma.h"

/* External function declaration----------------------------------------------*/


/* Private macro definitions--------------------------------------------------*/


/* Global variable------------------------------------------------------------*/

/* Variable containing ADC conversions results */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];

/* Variables for ADC conversions results computation to physical values */
static uint16_t   uhADCChannel_4_ToDAC_mVolt = 0;
static uint16_t   uhADCChannel_6_ToDAC_mVolt = 0;
static uint16_t   uhADCChannel_7_ToDAC_mVolt = 0;
static uint16_t   uhADCChannel_8_ToDAC_mVolt = 0;
static int32_t    wTemperature_DegreeCelsius = 0;
static uint16_t   uhVrefInt_mVolt = 0;

/* Variable to report ADC sequencer status */
uint8_t         ubSequenceCompleted = RESET;     /* Set when all ranks of the sequence have been converted */

/* Static function definition-------------------------------------------------*/

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
  /* Report to main program that ADC sequencer has reached its end */
  ubSequenceCompleted = SET;
}

/* Function definition--------------------------------------------------------*/
int32_t ADC_Get_Gas(void)
{
	int32_t ret= (int32_t)OPERATION_SUCCESS;
	
	HAL_ADC_Start(&hadc1);
	
	/* Wait for conversion completion before conditional check hereafter */
    HAL_ADC_PollForConversion(&hadc1, 1);
	
    if (ubSequenceCompleted == RESET)
    {
      /* ADC acquisition not completed, LED off */	
      HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_RESET);
	  ret= (int32_t)OPERATION_FALSE;
    }
	
	else
	{
	  /* ADC acquisition completed, LED light turned on */
	  HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET);
      
	  uhADCChannel_4_ToDAC_mVolt    = COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(aADCxConvertedValues[0]);
      uhADCChannel_6_ToDAC_mVolt    = COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(aADCxConvertedValues[1]);
	  uhADCChannel_7_ToDAC_mVolt    = COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(aADCxConvertedValues[2]);
	  uhADCChannel_8_ToDAC_mVolt    = COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(aADCxConvertedValues[3]);
      wTemperature_DegreeCelsius    = COMPUTATION_TEMPERATURE_STD_PARAMS(aADCxConvertedValues[4]);
	  uhVrefInt_mVolt               = COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(aADCxConvertedValues[5]);
	
	  ubSequenceCompleted = RESET;
	  ret= (int32_t)OPERATION_SUCCESS;
	}
	
	return ret;
}

float ADC_Get_Ozone(void)
{
	return (float)(OZONE_CONCENTRATION_CAL((float)uhADCChannel_4_ToDAC_mVolt/1000));
}

float ADC_Get_CO2(void)
{
	return (float)(CO2_CONCENTRATION_CAL((float)uhADCChannel_6_ToDAC_mVolt/1000));
}

float ADC_Get_CO(void)
{
	return (float)(CO_CONCENTRATION_CAL((float)uhADCChannel_7_ToDAC_mVolt/1000));
}

float ADC_Get_H2S(void)
{
	return (float)(H2S_CONCENTRATION_CAL((float)uhADCChannel_8_ToDAC_mVolt/1000));
}

float ADC_Get_Voltage(void)
{
	return (float)(uhVrefInt_mVolt)+VOLTAGE_OFFSET;
}

float ADC_Get_Temp(void)
{
	return wTemperature_DegreeCelsius+TEMP_OFFSET;
}


