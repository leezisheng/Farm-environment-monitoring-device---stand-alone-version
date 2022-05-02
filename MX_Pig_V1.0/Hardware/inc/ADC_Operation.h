/**
  ***********************************************************************************************************************
  * File Name          : UART_Printf.h
  * Description        : This file contains all the functions prototypes for
  *                      GetGas by ADC1 
  *
  *                      The voltage values of channel 4, channel 6, channel 7, channel 8, 
  *                      internal reference voltage and temperature sensor in ADC1 are obtained  
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
  ***********************************************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_Get_Gas_H
#define __ADC_Get_Gas_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Common macro definitions---------------------------------------------------*/
#define VDD_APPLI                      ((uint32_t) 3300)   /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_12BITS                   ((uint32_t) 4095)   /* Max value with a full range of 12 bits */
#define USERBUTTON_CLICK_COUNT_MAX     ((uint32_t)    4)   /* Maximum value of variable "UserButtonClickCount" */

#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)    5)   /* Size of array containing ADC converted values: set to ADC sequencer number of ranks converted, to have a rank in each address */

#define INTERNAL_TEMPSENSOR_V25        ((int32_t)1430)         /* Internal temperature sensor, parameter V25 (unit: mV). Refer to device datasheet for min/typ/max values. */
#define INTERNAL_TEMPSENSOR_AVGSLOPE   ((int32_t)4300)         /* Internal temperature sensor, parameter Avg_Slope (unit: uV/DegCelsius). Refer to device datasheet for min/typ/max values. */                

/**
  * @brief  Computation of temperature (unit: degree Celsius) from the internal
  *         temperature sensor measurement by ADC.
  *         Computation is using temperature sensor standard parameters (refer
  *         to device datasheet).
  *         Computation formula:
  *         Temperature = (VTS - V25)/Avg_Slope + 25
  *         with VTS = temperature sensor voltage
  *              Avg_Slope = temperature sensor slope (unit: uV/DegCelsius)
  *              V25 = temperature sensor @25degC and Vdda 3.3V (unit: mV)
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param TS_ADC_DATA: Temperature sensor digital value measured by ADC
  * @retval None
  */
#define COMPUTATION_TEMPERATURE_STD_PARAMS(TS_ADC_DATA)                        \
  ((((int32_t)(INTERNAL_TEMPSENSOR_V25 - (((TS_ADC_DATA) * VDD_APPLI) / RANGE_12BITS)   \
     ) * 1000                                                                  \
    ) / INTERNAL_TEMPSENSOR_AVGSLOPE                                           \
   ) + 25                                                                      \
  )

/**
  * @brief  Computation of voltage (unit: mV) from ADC measurement digital
  *         value on range 12 bits.
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param ADC_DATA: Digital value measured by ADC
  * @retval None
  */
#define COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(ADC_DATA)                        \
  ( (ADC_DATA) * VDD_APPLI / RANGE_12BITS)

/* Gas sensor output voltage and concentration fitting curve default macro parameters */

/* Ozone gas coefficient macro definition */
#define OZONE_COEFFICIENT_4 173.2
#define OZONE_COEFFICIENT_3 -2223.1
#define OZONE_COEFFICIENT_2 10611
#define OZONE_COEFFICIENT_1 -22446
#define OZONE_COEFFICIENT_0 17964
#define OZONE_DIVISOR       6700

/* carbon dioxide gas coefficient macro definition */
#define CO2_COEFFICIENT_4 156.6
#define CO2_COEFFICIENT_3 -3223.1
#define CO2_COEFFICIENT_2 40600
#define CO2_COEFFICIENT_1 -38470
#define CO2_COEFFICIENT_0 47964
#define CO2_DIVISOR       80

/* carbonic oxide gas coefficient macro definition */
#define CO_COEFFICIENT_4 11.7
#define CO_COEFFICIENT_3 -763.7
#define CO_COEFFICIENT_2 650
#define CO_COEFFICIENT_1 -3638
#define CO_COEFFICIENT_0 18809
#define CO_DIVISOR       180

/* hydrogen sulfide gas coefficient macro definition */
#define H2S_COEFFICIENT_4 17.21
#define H2S_COEFFICIENT_3 -220.2
#define H2S_COEFFICIENT_2 10.611
#define H2S_COEFFICIENT_1 -235.46
#define H2S_COEFFICIENT_0 1964
#define H2S_DIVISOR       1000

/* Voltage and temperature offset items */
#define VOLTAGE_OFFSET    4.0

/* Gas sensor output voltage and concentration fitting curve default macro function */

/* Ozone concentration calculation macro function */
#define OZONE_CONCENTRATION_CAL(x) (((x)*(x)*(x)*(x)*(OZONE_COEFFICIENT_4)+(x)*(x)*(x)*(OZONE_COEFFICIENT_3)+(x)*(x)*(OZONE_COEFFICIENT_2)+(x)*(OZONE_COEFFICIENT_1)+OZONE_COEFFICIENT_0)/OZONE_DIVISOR)

/* carbon dioxide concentration calculation macro function */
#define CO2_CONCENTRATION_CAL(x) (((x)*(x)*(x)*(x)*(CO2_COEFFICIENT_4)+(x)*(x)*(x)*(CO2_COEFFICIENT_3)+(x)*(x)*(CO2_COEFFICIENT_2)+(x)*(CO2_COEFFICIENT_1)+CO2_COEFFICIENT_0)/CO2_DIVISOR)

/* carbonic oxide concentration calculation macro function */
#define CO_CONCENTRATION_CAL(x) (((x)*(x)*(x)*(x)*(CO_COEFFICIENT_4)+(x)*(x)*(x)*(CO_COEFFICIENT_3)+(x)*(x)*(CO_COEFFICIENT_2)+(x)*(CO_COEFFICIENT_1)+CO_COEFFICIENT_0)/CO_DIVISOR)

/* hydrogen sulfide concentration calculation macro function */
#define H2S_CONCENTRATION_CAL(x) (((x)*(x)*(x)*(x)*(H2S_COEFFICIENT_4)+(x)*(x)*(x)*(H2S_COEFFICIENT_3)+(x)*(x)*(H2S_COEFFICIENT_2)+(x)*(H2S_COEFFICIENT_1)+H2S_COEFFICIENT_0)/H2S_DIVISOR)
  
  
  
/* Extern Variable------------------------------------------------------------*/
extern __IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];
extern uint8_t         ubSequenceCompleted;



/* Data structure declaration-------------------------------------------------*/



/* Function declaration-------------------------------------------------------*/
int32_t  ADC_Get_Gas(void);
float    ADC_Get_Ozone(void);
float    ADC_Get_CO2(void);
float    ADC_Get_CO(void);
float    ADC_Get_H2S(void);
float    ADC_Get_Voltage(void);

#ifdef __cplusplus
}
#endif
#endif /* __ADC_Get_Gas_H */

