/**
  ******************************************************************************
  * File Name          : Display.h
  * Description        : This file contains all the functions prototypes for
  *                      Display information on Oled
  *
  *					     Reference blog links:
  *                      https://blog.csdn.net/qq_47877230/article/details/109007571
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_H
#define __DISPLAY_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "numtype.h"
#include "main.h"

/* Common macro definitions---------------------------------------------------*/

/* I2C address of OLED (do not modify) */
#define OLED0561_ADD	0x78 
/* OLED instruction (no modification) */
#define COM				0x00
/* OLED data (no modification) */
#define DAT 			0x40 

/* Data structure declaration-------------------------------------------------*/

/* Function declaration-------------------------------------------------------*/

/* Basic function function declaration */
/* Write command function */
uint8_t WriteCmd(uint8_t I2C_Command);
/* Write data function */
uint8_t WriteDat(uint8_t I2C_Data);

/* Oled function declaration */

/* Oled initialization function */
uint8_t OLED_Init(void);
/* Set the starting point coordinates */
uint8_t OLED_SetPos(uint8_t x, uint8_t y);
/* Full screen OLED filling */
uint8_t OLED_Fill(uint8_t fill_Data);
/* Clear the screen */
uint8_t OLED_CLS(void);
/* Turn on the OLED screen */
uint8_t OLED_ON(void);
/* OLED closed */
uint8_t OLED_OFF(void);
/* OLED display string */
uint8_t OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize);
/* OLED display character */
uint8_t OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
/* Display 2 numbers */
uint8_t OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Char_Size );
	

#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_H */
