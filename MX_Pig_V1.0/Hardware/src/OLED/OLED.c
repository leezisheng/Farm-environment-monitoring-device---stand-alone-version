/**
  ******************************************************************************
  * File Name          : Display.c
  * Description        : This file provides code for the configuration
  *                      Display information on an Oled
  ******************************************************************************
 */
  
/* Includes ------------------------------------------------------------------*/
#include "OLED.h"
#include "i2c.h"

/* Font header file */
#include "ASCII.h"

/* External function declaration----------------------------------------------*/


/* Private macro definitions--------------------------------------------------*/


/* Global variable------------------------------------------------------------*/


/* Static function definition-------------------------------------------------*/
static uint32_t oled_pow(uint8_t m,uint8_t n);

/* Function definition--------------------------------------------------------*/

/* --------------------Basic function function definition-------------------- */

/** 
* @description: Write command function
* @param {uint8_t} I2C_Command
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t WriteCmd(uint8_t I2C_Command)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;

	if(HAL_I2C_Mem_Write(&hi2c1,OLED0561_ADD,COM,I2C_MEMADD_SIZE_8BIT,&I2C_Command,1,1000) != HAL_OK)
	{
		ret = (uint8_t)OPERATION_ERROR;
	}

	return ret;
}

/** 
* @description: Write data function
* @param {uint8_t} I2C_Command
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t WriteDat(uint8_t I2C_Data)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;

	if( HAL_I2C_Mem_Write(&hi2c1,OLED0561_ADD,DAT,I2C_MEMADD_SIZE_8BIT,&I2C_Data,1,1000) != HAL_OK)
	{
		ret = (uint8_t)OPERATION_ERROR;
	}

	return ret;
}

/* --------------------Oled function definition-------------------- */

/** 
* @description: Oled initialization function
* @param {void} 
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t OLED_Init(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;

	HAL_Delay(500); //--The delay cannot be removed
	
	ret = 	WriteCmd(0xAE)		&& 	//--display off
			WriteCmd(0x20)		&&	//--Set Memory Addressing Mode	
			WriteCmd(0x10)   	&&	//--00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
			WriteCmd(0xb0)		&&	//--Set Page Start Address for Page Addressing Mode,0-7
			WriteCmd(0xc8)		&&	//--Set COM Output Scan Direction
			WriteCmd(0x00)		&&	//---set low column address
			WriteCmd(0x10)		&&	//---set high column address
			WriteCmd(0x40)		&& 	//--set start line address
			WriteCmd(0x81)		&&  //--set contrast control register
			WriteCmd(0xff)		&& 	//--Brightness control:0x00~0xff
			WriteCmd(0xa1)		&& 	//--set segment re-map 0 to 127
			WriteCmd(0xa6)		&& 	//--set normal display
			WriteCmd(0xa8)		&& 	//--set multiplex ratio(1 to 64)
			WriteCmd(0x3F)		&& 	//
			WriteCmd(0xa4)		&& 	//--0xa4,Output follows RAM content;0xa5,Output ignores RAM content
			WriteCmd(0xd3)		&& 	//--set display offset
			WriteCmd(0x00)		&& 	//--not offset
			WriteCmd(0xd5)		&& 	//--set display clock divide ratio/oscillator frequency
			WriteCmd(0xf0)		&& 	//--set divide ratio
			WriteCmd(0xd9)		&& 	//--set pre-charge period
			WriteCmd(0x22)		&& 	//
			WriteCmd(0xda)		&& 	//--set com pins hardware configuration
			WriteCmd(0x12)		&& 	
			WriteCmd(0xdb)		&& 	//--set vcomh
			WriteCmd(0x20)		&& 	//--0x20,0.77xVcc
			WriteCmd(0x8d)		&& 	//--set DC-DC enable
			WriteCmd(0x14)		&& 	//
			WriteCmd(0xaf);		    //--turn on oled panel
			
	return ret;
}

/** 
* @description: Set the starting point coordinates
* @param {uint8_t} x : The x-coordinate of our starting point
* @param {uint8_t} y : The y-coordinate of our starting point
* @return {uint8_t}:if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t OLED_SetPos(uint8_t x, uint8_t y)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;

	ret = WriteCmd(0xb0+y) 					&&
		  WriteCmd(((x&0xf0)>>4)|0x10)		&&
		  WriteCmd((x&0x0f)|0x01);

	return ret;
}

/** 
* @description: Full screen OLED filling
* @param  {uint8_t} fill_Data : Data that needs to be populated
* @return {uint8_t}           : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t OLED_Fill(uint8_t fill_Data)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	unsigned char m,n;
	
	for(m=0;m<8;m++)
	{
		ret =	WriteCmd(0xb0+m)  &&	//page0-page1
				WriteCmd(0x00)    &&	//low column start address
				WriteCmd(0x10);			//high column start address
		for(n=0;n<128;n++)
			{
				ret = WriteDat(fill_Data) && ret;
			}
		if ( ret != (uint8_t)OPERATION_SUCCESS ) 
			return ret;
	}

	return ret;
}

/** 
* @description: Clear the screen
* @param  {void   } 
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t OLED_CLS(void)
{
	return OLED_Fill(0x00);
}

/** 
* @description: Turn on the OLED screen
* @param  {void   } 
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t OLED_ON(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	ret =	WriteCmd(0X8D)	&&	// 	Set charge pump
			WriteCmd(0X14)	&&	//	Open charge pump
			WriteCmd(0XAF);		//	OLED awaken
	
	return ret;
}

/** 
* @description: OLED closed
* @param  {void   } 
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t OLED_OFF(void)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	ret =	WriteCmd(0X8D)	&&	// 	Set charge pump
			WriteCmd(0X10)	&&	//	close charge pump
			WriteCmd(0XAE);		//	OLED dormancy
	
	return ret;
}

/** 
* @description: Display ASCII characters, 6*8 and 8*16 are optional
* @param  {uint8_t  } x         ：The x-coordinate of our starting point ， x:0~127
* @param  {uint8_t  } y         ：The y-coordinate of our starting point ， y:0~7
* @param  {uint8_t *} ch        ：The string to display
* @param  {uint8_t  } TextSize  ：Character size ， 1:6*8 ; 2:8*16
* @return {uint8_t  }           : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/

uint8_t OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	unsigned char c = 0,i = 0,j = 0;

	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				
				if(x > 126)
				{
					x = 0;
					y++;
				}
				
				ret = OLED_SetPos(x,y) && ret;
				
				for(i=0;i<6;i++)
					ret = WriteDat(F6x8[c][i]) && ret;
				
				x += 6;
				j++;
			}
			
		}break;
		
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				
				if(x > 120)
				{
					x = 0;
					y++;
				}
				
				ret = OLED_SetPos(x,y) && ret;
				
				for(i=0;i<8;i++)
					ret = WriteDat(F8X16[c*16+i]) && ret;
				
				ret = OLED_SetPos(x,y+1) && ret;
				
				for(i=0;i<8;i++)
					ret = WriteDat(F8X16[c*16+i+8]) && ret;
				
				x += 8;
				j++;
			}
		}break;
		
		default:
			ret = (uint8_t)OPERATION_FALSE;
		break;
	
	}

	return ret;
}

/** 
* @description: OLED display character
* @param  {uint8_t} x		  : The x-coordinate of our starting point ， x:0~127
* @param  {uint8_t} y		  : The y-coordinate of our starting point ， y:0~7
* @param  {uint8_t} chr		  : The string to display
* @param  {uint8_t} Char_Size : Character size ， 1:6*8 ; 2:8*16
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	unsigned char c=0,i=0;	
	
	// get the offset value
	c=chr - ' ';
	
	if(x>128-1)
	{
		x=0;
		y=y+2;
	}
	
	switch( Char_Size )
	{
		case 1 :	
		{
			ret = OLED_SetPos(x,y) && ret;	
			
			for(i=0;i<8;i++)
				ret = WriteDat(F8X16[c*16+i]) && ret;
			
			ret =OLED_SetPos(x,y+1) && ret;
			
			for(i=0;i<8;i++)
				ret = WriteDat(F8X16[c*16+i+8]) && ret;
		}break;
		
		case 2 :
		{
			ret = OLED_SetPos(x,y) && ret;
			
			for(i=0;i<6;i++)
				ret = WriteDat(F6x8[c][i]) && ret;
		}
		break;
		
		default:
			ret = (uint8_t)OPERATION_FALSE;
		break;
		
	}

	return ret;
}

/** 
* @description: Display 2 numbers
* @param  {uint8_t}  x	     	: The x-coordinate of our starting point ， x:0~127
* @param  {uint8_t}  y			: The y-coordinate of our starting point ， y:0~7
* @param  {uint32_t} num	 	: Two digits
* @param  {uint8_t}  len     	: Digital digits
* @param  {uint8_t}  Char_Size  : font size , 1:6*8 ; 2:8*16
* @return {uint8_t} : if success,return (uint8_t)OPERATION_SUCCESS
* @author: leeqingshui 
*/
uint8_t OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Char_Size )
{
	uint8_t ret = (uint8_t)OPERATION_SUCCESS;
	
	uint32_t t , temp;
	uint8_t  enshow = 0;	
	
	for(t=0;t<len;t++)
	{
		temp = ( num / oled_pow(10, len-t-1) ) % 10;
		
		if( enshow == 0&& t<(len-1) )
		{
			if( temp == 0 )
			{
				ret = OLED_ShowChar( x + ( Char_Size/2 )*t, y, ' ', Char_Size) && ret;
					continue;
			}
			else 
				enshow=1;  
		}
		
	 	ret = OLED_ShowChar(x+(Char_Size/2)*t,y,temp+'0',Char_Size) && ret; 
	}
	
	return ret;
}

/* --------------------static function definition-------------------- */

static uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	
	while(n--)
	{
		result *= m ;  
	}
	
	return result;
}	




