/*
 * bsp_touch.c
 *
 *  Created on: Mar 17, 2025
 *      Author: compro
 */
#include "stm32f4xx.h"
#include <stdio.h>

#include "bsp_touch.h"
#include "bsp_tft_lcd.h"
#include "LCD_RA8875.h"
#include "bsp_timer.h"

#include "param.h"

/*
¡¾1¡¿°²¸»À³STM32-X2¿ª·¢°å + 3.0´çÏÔÊ¾Ä£¿é£¬ ÏÔÊ¾Ä£¿éÉÏµÄ´¥ÃþÐ¾Æ¬Îª TSC2046»òÆä¼æÈÝÐ¾Æ¬
	PA8   --> TP_CS
	PD3   --> TP_BUSY
	PA5   --> TP_SCK
	PA6   --> TP_MISO
	PA7   --> TP_MOSI
	PC7   --> TP_PEN_INT

¡¾2¡¿°²¸»À³STM32¿ª·¢°å + 4.3´ç»ò7´çÏÔÊ¾Ä£¿é£¨ÄÚÖÃRA8875Ð¾Æ¬)
	RA8875ÄÚÖÃ´¥ÃþÆÁ½Ó¿Ú£¬Òò´ËÖ±½ÓÍ¨¹ýFSMC·ÃÎÊRA8875Ïà¹Ø¼Ä´æÆ÷¼´¿É¡£


	±¾³ÌÐòÎ´Ê¹ÓÃ´¥±ÊÖÐ¶Ï¹¦ÄÜ¡£ÔÚ1msÖÜÆÚµÄ Systick¶¨Ê±ÖÐ¶Ï·þÎñ³ÌÐòÖÐ¶Ô´¥ÃþADCÖµ½øÐÐ²ÉÑùºÍ
	ÂË²¨´¦Àí¡£µ±´¥±Ê°´ÏÂ³¬¹ý40msºó£¬¿ªÊ¼²É¼¯ADCÖµ£¨Ã¿1ms²É¼¯1´Î£¬Á¬Ðø²É¼¯10´Î£©£¬È»ºó¶Ô
	Õâ10¸öÑù±¾½øÐÐÅÅÐò£¬¶ªÆú2Í·µÄÑù±¾£¬¶ÔÖÐ¼äÒ»¶ÎÑù±¾½øÐÐÇóºÍ²¢¼ÆËãÆ½¾ùÖµ¡£

	²ÉÑù2µãÐ£×¼·½·¨£¬Ð£×¼ºó±£´æ2¸öÐ£×¼µãµÄADCÖµ£¬Êµ¼Ê¹¤×÷Ê±£¬¸ù¾Ý2µãÖ±Ïß·½³Ì¼ÆËãÆÁÄ»×ø±ê¡£
	Ð£×¼²ÎÊýÓÐ±£´æ½Ó¿Ú£¬±¾³ÌÐòÖ÷ÒªÓÃÓÚÑÝÊ¾£¬Î´×ö±£´æ¹¦ÄÜ¡£
	´ó¼Ò¿ÉÒÔ×Ô¼ºÐÞ¸Ä  TOUCH_SaveParam() ºÍ TOUCH_LoadParam() Á½¸öº¯ÊýÊµÏÖ±£´æ¹¦ÄÜ¡£

*/

/* TSC2046 Æ¬Ñ¡ */
#define TSC2046_CS_1()	GPIOA->BSRRL = 	GPIO_Pin_8
#define TSC2046_CS_0()	GPIOA->BSRRH = 	GPIO_Pin_8

/* TSC2046 ÄÚ²¿ADCÍ¨µÀºÅ */
#define ADC_CH_X	1		/* XÍ¨µÀ£¬²âÁ¿XÎ»ÖÃ */
#define ADC_CH_Y	5		/* YÍ¨µÀ£¬²âÁ¿YÎ»ÖÃ */

#define SAMPLE_COUNT	10	/* Ã¿´ÎÁ¬Ðø²É¼¯¶àÉÙ¸öÑù±¾ */

/*
´¥ÃþÆÁÐ£×¼µãÏà¶ÔÆÁÄ»ÏñËØËÄ½ÇµÄÆ«ÒÆÏñËØ
µÚ1¸öµã £º x1 = CALIB_OFFSET, y1 = CALIB_OFFSET
µÚ2¸öµã £º x2 = LCD_GetWidth() - CALIB_OFFSET, y2 = LCD_GetHeight() - CALIB_OFFSET
*/
#define CALIB_OFFSET	20
#define TP_X1	CALIB_OFFSET
#define TP_Y1	CALIB_OFFSET

#define TP_X2	(LCD_GetWidth() - CALIB_OFFSET)
#define TP_Y2	(LCD_GetHeight() - CALIB_OFFSET)

#define TP_X3	CALIB_OFFSET
#define TP_Y3	(LCD_GetHeight() - CALIB_OFFSET)

#define TP_X4	(LCD_GetWidth() - CALIB_OFFSET)
#define TP_Y4	CALIB_OFFSET

/* ÓÐÐ§ADCÖµµÄÅÐ¶ÏÃÅÏÞ. Ì«½Ó½üADCÁÙ½çÖµµÄ×ø±êÈÏÎªÎÞÐ§ */
#define ADC_VALID_OFFSET	2

//#define WaitTPReady() while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5))
#define WaitTPReady() {}

/* ´¥ÆÁÄ£¿éÓÃµ½µÄÈ«¾Ö±äÁ¿ */
TOUCH_T g_tTP;

static void TSC2046_InitHard(void);
static uint16_t TSC2046_ReadAdc(uint8_t _ucCh);
static uint8_t	TOUCH_PressValid(uint16_t _usX, uint16_t _usY);
static uint16_t TOUCH_DataFilter(uint16_t *_pBuf, uint8_t _ucCount);
static void TOUCH_LoadParam(void);
static void TOUCH_SaveParam(void);
static int32_t CalTwoPoint(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x);
static int16_t TOUCH_TransX(uint16_t _usAdcX, uint16_t _usAdcY);
static int16_t TOUCH_TransY(uint16_t _usAdcX, uint16_t _usAdcY);
int32_t TOUCH_Abs(int32_t x);

/*
*********************************************************************************************************
*	º¯ Êý Ãû: bsp_InitTouch
*	¹¦ÄÜËµÃ÷: ÅäÖÃSTM32ºÍ´¥ÃþÏà¹ØµÄ¿ÚÏß£¬Ê¹ÄÜÓ²¼þSPI1, Æ¬Ñ¡ÓÉÈí¼þ¿ØÖÆ
*	ÐÎ    ²Î:  ÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void TOUCH_InitHard(void)
{
	g_tTP.Enable = 0;
	if (g_ChipID == IC_8875)
	{
		RA8875_TouchInit();
		g_tTP.usMaxAdc = 1023;	/* 10Î»ADC */
	}
	else
	{
		//TSC2046_InitHard();
		//g_tTP.usMaxAdc = 4095;	/* 12Î»ADC */
	}
	TOUCH_LoadParam();	/* ¶ÁÈ¡Ð£×¼²ÎÊý */
	g_tTP.Write = g_tTP.Read = 0;
	g_tTP.Enable = 1;
}

/*
*********************************************************************************************************
*	Func name: TOUCH_LoadParam
*********************************************************************************************************
*/
extern void LoadParam(void);
static void TOUCH_LoadParam(void)
{
#if 1
	LoadParam();	/* ´ÓFlashÖÐ¶ÁÈ¡²ÎÊý */

	g_tTP.usAdcX1 = g_tParam.usAdcX1;
	g_tTP.usAdcY1 = g_tParam.usAdcY1;
	g_tTP.usAdcX2 = g_tParam.usAdcX2;
	g_tTP.usAdcY2 = g_tParam.usAdcY2;
	g_tTP.usAdcX3 = g_tParam.usAdcX3;
	g_tTP.usAdcY3 = g_tParam.usAdcY3;
	g_tTP.usAdcX4 = g_tParam.usAdcX4;
	g_tTP.usAdcY4 = g_tParam.usAdcY4;
	g_tTP.XYChange = g_tParam.XYChange;

#endif
}

/*
*********************************************************************************************************
*	Func name: TOUCH_GetKey
*********************************************************************************************************
*/
uint8_t TOUCH_GetKey(int16_t *_pX, int16_t *_pY)
{
	uint8_t ret;

	if (g_tTP.Read == g_tTP.Write)
	{
		return TOUCH_NONE;
	}
	else
	{
		ret = g_tTP.Event[g_tTP.Read];
		*_pX = g_tTP.XBuf[g_tTP.Read];
		*_pY = g_tTP.YBuf[g_tTP.Read];

		if (++g_tTP.Read >= TOUCH_FIFO_SIZE)
		{
			g_tTP.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	Func name: TOUCH_InRect
*********************************************************************************************************
*/
uint8_t TOUCH_InRect(uint16_t _usX, uint16_t _usY,
	uint16_t _usRectX, uint16_t _usRectY, uint16_t _usRectH, uint16_t _usRectW)
{
	if ((_usX > _usRectX) && (_usX < _usRectX + _usRectW)
		&& (_usY > _usRectY) && (_usY < _usRectY + _usRectH))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}




















