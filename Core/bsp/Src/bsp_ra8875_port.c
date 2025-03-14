/*
 * bsp_ra8875_port.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "bsp.h"

#ifdef RA_SOFT_8080_8		/* Èí¼þÄ£Äâ8080×ÜÏß£¬8 bit */
	#error "STM32_V5 dont's surport soft 8080"
#endif

#ifdef RA_SOFT_SPI			/* Èí¼þÄ£SPI×ÜÏß */
	#error "STM32_V5 dont's surport soft SPI"
#endif

#ifdef RA_HARD_SPI	/* Ó²¼þ SPI ½çÃæ (ÐèÒª¸Ä±äRA8875ÆÁÉÏµÄ2¸öµç×èÎ»ÖÃ) */
	/*
	¡¾1¡¿°²¸»À³STM32-V5¿ª·¢°å,STM32F407IGT6
		PD3/LCD_BUSY		--- ´¥ÃþÐ¾Æ¬Ã¦       £¨RA8875ÆÁÊÇRA8875Ð¾Æ¬µÄÃ¦ÐÅºÅ)
		PF6/LCD_PWM			--- LCD±³¹âPWM¿ØÖÆ  £¨RA8875ÆÁÎÞÐè´Ë½Å£¬±³¹âÓÉRA8875¿ØÖÆ)

		PI10/TP_NCS			--- ´¥ÃþÐ¾Æ¬µÄÆ¬Ñ¡		(RA8875ÆÁÎÞÐèSPI½Ó¿Ú´¥ÃþÐ¾Æ¬£©
		PB3/SPI3_SCK		--- ´¥ÃþÐ¾Æ¬SPIÊ±ÖÓ		(RA8875ÆÁÎÞÐèSPI½Ó¿Ú´¥ÃþÐ¾Æ¬£©
		PB4/SPI3_MISO		--- ´¥ÃþÐ¾Æ¬SPIÊý¾ÝÏßMISO(RA8875ÆÁÎÞÐèSPI½Ó¿Ú´¥ÃþÐ¾Æ¬£©
		PB5/SPI3_MOSI		--- ´¥ÃþÐ¾Æ¬SPIÊý¾ÝÏßMOSI(RA8875ÆÁÎÞÐèSPI½Ó¿Ú´¥ÃþÐ¾Æ¬£©

		PC3/TP_INT			--- ´¥ÃþÐ¾Æ¬ÖÐ¶Ï £¨¶ÔÓÚRA8875ÆÁ£¬ÊÇRA8875Êä³öµÄÖÐ¶Ï)
	*/
	#define RA8875_CS_0()	GPIOI->BSRRH = GPIO_Pin_10
	#define RA8875_CS_1()	GPIOI->BSRRL = GPIO_Pin_10

	#define SPI_WRITE_DATA	0x00
	#define SPI_READ_DATA	0x40
	#define SPI_WRITE_CMD	0x80
	#define SPI_READ_STATUS	0xC0

	static uint8_t SPI_ShiftByte(uint8_t _ucByte);
#endif
#if 0
#ifdef RA_HARD_8080_16		/* 8080Ó²¼þ×ÜÏß £¨°²¸»À³RA8875ÆÁÈ±Ê¡Ä£Ê½£© */
	/*
		°²¸»À³STM32-V5¿ª·¢°å½ÓÏß·½·¨£º

		PD0/FSMC_D2
		PD1/FSMC_D3
		PD4/FSMC_NOE		--- ¶Á¿ØÖÆÐÅºÅ£¬OE = Output Enable £¬ N ±íÊ¾µÍÓÐÐ§
		PD5/FSMC_NWE		--- Ð´¿ØÖÆÐÅºÅ£¬WE = Output Enable £¬ N ±íÊ¾µÍÓÐÐ§
		PD8/FSMC_D13
		PD9/FSMC_D14
		PD10/FSMC_D15
		PD13/FSMC_A18		--- µØÖ· RS
		PD14/FSMC_D0
		PD15/FSMC_D1

		PE4/FSMC_A20		--- ºÍÖ÷Æ¬Ñ¡Ò»ÆðÒëÂë
		PE5/FSMC_A21		--- ºÍÖ÷Æ¬Ñ¡Ò»ÆðÒëÂë
		PE7/FSMC_D4
		PE8/FSMC_D5
		PE9/FSMC_D6
		PE10/FSMC_D7
		PE11/FSMC_D8
		PE12/FSMC_D9
		PE13/FSMC_D10
		PE14/FSMC_D11
		PE15/FSMC_D12

		PG12/FSMC_NE4		--- Ö÷Æ¬Ñ¡£¨TFT, OLED ºÍ AD7606£©

		PC3/TP_INT			--- ´¥ÃþÐ¾Æ¬ÖÐ¶Ï £¨¶ÔÓÚRA8875ÆÁ£¬ÊÇRA8875Êä³öµÄÖÐ¶Ï)

		---- ÏÂÃæÊÇ TFT LCD½Ó¿ÚÆäËûÐÅºÅ £¨FSMCÄ£Ê½²»Ê¹ÓÃ£©----
		PD3/LCD_BUSY		--- ´¥ÃþÐ¾Æ¬Ã¦       £¨RA8875ÆÁÊÇRA8875Ð¾Æ¬µÄÃ¦ÐÅºÅ)
		PF6/LCD_PWM			--- LCD±³¹âPWM¿ØÖÆ  £¨RA8875ÆÁÎÞÐè´Ë½Å£¬±³¹âÓÉRA8875¿ØÖÆ)

		PI10/TP_NCS			--- ´¥ÃþÐ¾Æ¬µÄÆ¬Ñ¡		(RA8875ÆÁÎÞÐèSPI½Ó¿Ú´¥ÃþÐ¾Æ¬£©
		PB3/SPI3_SCK		--- ´¥ÃþÐ¾Æ¬SPIÊ±ÖÓ		(RA8875ÆÁÎÞÐèSPI½Ó¿Ú´¥ÃþÐ¾Æ¬£©
		PB4/SPI3_MISO		--- ´¥ÃþÐ¾Æ¬SPIÊý¾ÝÏßMISO(RA8875ÆÁÎÞÐèSPI½Ó¿Ú´¥ÃþÐ¾Æ¬£©
		PB5/SPI3_MOSI		--- ´¥ÃþÐ¾Æ¬SPIÊý¾ÝÏßMOSI(RA8875ÆÁÎÞÐèSPI½Ó¿Ú´¥ÃþÐ¾Æ¬£©
	*/
	#define RA8875_BASE		((uint32_t)(0x6C000000 | 0x00000000))

	#define RA8875_REG		*(__IO uint16_t *)(RA8875_BASE +  (1 << (18 + 1)))	/* FSMC 16Î»×ÜÏßÄ£Ê½ÏÂ£¬FSMC_A18¿ÚÏß¶ÔÓ¦ÎïÀíµØÖ·A19 */
	#define RA8875_RAM		*(__IO uint16_t *)(RA8875_BASE)

	#define RA8875_RAM_ADDR		RA8875_BASE
#endif
#endif


/*
*********************************************************************************************************
*	Func name: RA8875_WriteCmd
*********************************************************************************************************
*/
void RA8875_WriteCmd(uint8_t _ucRegAddr)
{
#ifdef RA_HARD_SPI	/* Ó²¼þSPI½Ó¿Ú */
	RA8875_CS_0();
	SPI_ShiftByte(SPI_WRITE_CMD);
	SPI_ShiftByte(_ucRegAddr);
	RA8875_CS_1();
#endif

#ifdef RA_HARD_8080_16			/* 8080Ó²¼þ×ÜÏß */
	RA8875_REG = _ucRegAddr;	/* ÉèÖÃ¼Ä´æÆ÷µØÖ· */
#endif
}

/*
*********************************************************************************************************
*	Func name: RA8875_ReadData
*********************************************************************************************************
*/
uint8_t RA8875_ReadData(void)
{
#ifdef RA_HARD_SPI	/* Ó²¼þSPI½Ó¿Ú */
	uint16_t value;

	RA8875_CS_0();
	SPI_ShiftByte(SPI_READ_DATA);
	value = SPI_ShiftByte(0x00);
	RA8875_CS_1();

	return value;
#endif

#ifdef RA_HARD_8080_16			/* 8080Ó²¼þ×ÜÏß */
	uint8_t value;

	value = RA8875_RAM;		/* ¶ÁÈ¡¼Ä´æÆ÷Öµ */

	return value;
#endif
}

/*
*********************************************************************************************************
*	Func name: RA8875_WriteData
*********************************************************************************************************
*/
void RA8875_WriteData(uint8_t _ucRegValue)
{
#ifdef RA_HARD_SPI	/* Ó²¼þSPI½Ó¿Ú */
	RA8875_CS_0();
	SPI_ShiftByte(SPI_WRITE_DATA);
	SPI_ShiftByte(_ucRegValue);
	RA8875_CS_1();
#endif

#ifdef RA_HARD_8080_16			/* 8080Ó²¼þ×ÜÏß */
	RA8875_RAM = _ucRegValue;	/* ÉèÖÃ¼Ä´æÆ÷µØÖ· */
#endif
}

/*
*********************************************************************************************************
*	Func name: RA8875_ReadStatus
*********************************************************************************************************
*/
uint8_t RA8875_ReadStatus(void)
{
#ifdef RA_HARD_SPI	/* Ó²¼þSPI½Ó¿Ú */
	uint8_t value;

	RA8875_CS_0();
	SPI_ShiftByte(SPI_READ_STATUS);
	value = SPI_ShiftByte(0x00);
	RA8875_CS_1();

	return value;
#endif

#ifdef RA_HARD_8080_16			/* 8080Ó²¼þ×ÜÏß */
	uint8_t value;

	value = RA8875_REG;

	return value;
#endif
}

/*
*********************************************************************************************************
*	Func name: RA8875_Delaly1us
*********************************************************************************************************
*/
void RA8875_Delaly1us(void)
{
	uint8_t i;
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	Func name: RA8875_Delaly1ms
*********************************************************************************************************
*/
void RA8875_Delaly1ms(void)
{
	uint16_t i;
	for (i = 0; i < 5000; i++);
}



