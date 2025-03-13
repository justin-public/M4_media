/*
 * LCD_SPFD5420.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "bsp_tft_lcd.h"

#define SPFD5420_BASE       ((uint32_t)(0x60000000 | 0x00000000))

#define SPFD5420_REG		*(__IO uint16_t *)(SPFD5420_BASE)
#define SPFD5420_RAM		*(__IO uint16_t *)(SPFD5420_BASE + (1 << 19))

static __IO uint8_t s_RGBChgEn = 0;		/* RGB×ª»»Ê¹ÄÜ, 4001ÆÁÐ´ÏÔ´æºó¶Á»áµÄRGB¸ñÊ½ºÍÐ´ÈëµÄ²»Í¬ */

static void SPFD5420_Delaly10ms(void);
static void SPFD5420_WriteReg(__IO uint16_t _usAddr, uint16_t _usValue);
static uint16_t SPFD5420_ReadReg(__IO uint16_t _usAddr);
static void Init_5420_4001(void);
static void Init_61509(void);
static void SPFD5420_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
static void SPFD5420_QuitWinMode(void);
static void SPFD5420_SetCursor(uint16_t _usX, uint16_t _usY);
static uint16_t SPFD5420_BGR2RGB(uint16_t _usRGB);


/*
*********************************************************************************************************
*	Func name: SPFD5420_ReadID
*********************************************************************************************************
*/
uint16_t SPFD5420_ReadID(void)
{
	return SPFD5420_ReadReg(0x0000);
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: SPFD5420_ReadReg
*	¹¦ÄÜËµÃ÷: ¶ÁÈ¡LCD¿ØÖÆÆ÷µÄ¼Ä´æÆ÷µÄÖµ¡£
*	ÐÎ    ²Î:
*			SPFD5420_Reg £º¼Ä´æÆ÷µØÖ·;
*			SPFD5420_RegValue : ¼Ä´æÆ÷Öµ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
static uint16_t SPFD5420_ReadReg(__IO uint16_t _usAddr)
{
	/* Write 16-bit Index (then Read Reg) */
	SPFD5420_REG = _usAddr;
	/* Read 16-bit Reg */
	return (SPFD5420_RAM);
}
