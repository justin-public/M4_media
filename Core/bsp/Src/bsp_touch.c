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

static void TOUCH_DispPoint(uint8_t _ucIndex);

static void TOUCH_WaitRelease(void);

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

/*
*********************************************************************************************************
* Func name: TOUCH_Calibration
*********************************************************************************************************
*/
void TOUCH_Calibration(void)
{
	uint16_t usAdcX;
	uint16_t usAdcY;
	uint8_t usCount;
	uint8_t i;
	uint32_t n;

	TOUCH_CelarFIFO();		/* Çå³ýÎÞÐ§µÄ´¥ÃþÊÂ¼þ */

	for (i = 0; i < CALIB_POINT_COUNT; i++)
	{
		TOUCH_DispPoint(i);		/* ÏÔÊ¾Ð£×¼µã */

		TOUCH_WaitRelease(); 	/* µÈ´ý´¥±ÊÊÍ·Å */

		usCount = 0;
		for (n = 0; n < 500; n++)
		{
			usAdcX = TOUCH_ReadAdcX();
			usAdcY = TOUCH_ReadAdcY();

			if (TOUCH_PressValid(usAdcX, usAdcY))
			{
				if (++usCount > 5)
				{
					/* °´Ñ¹ÓÐÐ§, ±£´æÐ£×¼µãADC²ÉÑùÖµ */
					if (i == 0)
					{
						g_tTP.usAdcX1 = usAdcX;
						g_tTP.usAdcY1 = usAdcY;
					}
					else if (i == 1)
					{
						g_tTP.usAdcX2 = usAdcX;
						g_tTP.usAdcY2 = usAdcY;
					}
					else if (i == 2)
					{
						g_tTP.usAdcX3 = usAdcX;
						g_tTP.usAdcY3 = usAdcY;
					}
					else
					{
						g_tTP.usAdcX4 = usAdcX;
						g_tTP.usAdcY4 = usAdcY;
					}
					break;
				}
			}
			else
			{
				usCount = 0;
			}
			bsp_DelayMS(10);
		}
		if (n == 500)
		{
			return;
		}
	}
	TOUCH_WaitRelease(); 	/* µÈ´ý´¥±ÊÊÍ·Å */

	/* Ê¶±ð´¥ÃþµÄ X, Y ºÍ ÏÔÊ¾Ãæ°åµÄ X£¬Y ÊÇ·ñÐèÒª½»»» */
	g_tTP.XYChange = 0;		/* 1±íÊ¾X YÐèÒª½»»» */
	if (LCD_GetHeight() < LCD_GetWidth())
	{
		if (TOUCH_Abs(g_tTP.usAdcX1 - g_tTP.usAdcX2) < TOUCH_Abs(g_tTP.usAdcY1 - g_tTP.usAdcY2))
		{
			g_tTP.XYChange = 1;
		}
	}
	else
	{
		if (TOUCH_Abs(g_tTP.usAdcX1 - g_tTP.usAdcX2) > TOUCH_Abs(g_tTP.usAdcY1 - g_tTP.usAdcY2))
		{
			g_tTP.XYChange = 1;
		}
	}
	TOUCH_SaveParam();
}

/*
*********************************************************************************************************
* Func name: TOUCH_CelarFIFO
*********************************************************************************************************
*/
void TOUCH_CelarFIFO(void)
{
	__set_PRIMASK(1);  		/* ¹ØÖÐ¶Ï */
	g_tTP.Write = g_tTP.Read;
	__set_PRIMASK(0);  		/* ¿ªÖÐ¶Ï */
}

/*
*********************************************************************************************************
* Func name: TOUCH_DispPoint1
*********************************************************************************************************
*/
static void TOUCH_DispPoint(uint8_t _ucIndex)
{
	FONT_T tFont16;
	{
		tFont16.FontCode = FC_ST_16;	/* ×ÖÌå´úÂë 16µãÕó */
		tFont16.FrontColor = CL_WHITE;		/* ×ÖÌåÑÕÉ« 0 »ò 1 */
		tFont16.BackColor = CL_BLUE;	/* ÎÄ×Ö±³¾°ÑÕÉ« */
		tFont16.Space = 0;			/* ÎÄ×Ö¼ä¾à£¬µ¥Î» = ÏñËØ */
	}
	if (_ucIndex == 0)
	{
		LCD_ClrScr(CL_BLUE);
		LCD_DrawRect(0, 0, LCD_GetHeight(), LCD_GetWidth(), CL_WHITE);
		LCD_DrawRect(2, 2, LCD_GetHeight() - 4, LCD_GetWidth() - 4, CL_YELLOW);

		LCD_DispStr(50, 10, "Calibration", &tFont16);

		LCD_DrawCircle(TP_X1, TP_Y1, 6, CL_WHITE);
	}
	else if (_ucIndex == 1)
	{
		LCD_DrawCircle(TP_X1, TP_Y1, 6, CL_BLUE);			/* ²Á³ýµÚ1¸öµã */

		LCD_DrawCircle(TP_X2, TP_Y2, 6, CL_WHITE);
	}
	else if (_ucIndex == 2)
	{
		LCD_DrawCircle(TP_X2, TP_Y2, 6, CL_BLUE);			/* ²Á³ýµÚ2¸öµã */

		LCD_DrawCircle(TP_X3, TP_Y3, 6, CL_WHITE);
	}
	else
	{
		LCD_DrawCircle(TP_X3, TP_Y3, 6, CL_BLUE);			/* ²Á³ýµÚ3¸öµã */

		LCD_DrawCircle(TP_X4, TP_Y4, 6, CL_WHITE);
	}
}

/*
*********************************************************************************************************
* Func name: TOUCH_WaitRelease
*********************************************************************************************************
*/
static void TOUCH_WaitRelease(void)
{
	uint8_t usCount = 0;

	for (;;)
	{
		if (TOUCH_PressValid(TOUCH_ReadAdcX(), TOUCH_ReadAdcY()) == 0)
		{
			if (++usCount > 5)
			{
				break;
			}
		}
		else
		{
			usCount = 0;
		}
		bsp_DelayMS(10);
	}
}


/*
*********************************************************************************************************
*	º¯ Êý Ãû: TOUCH_PressValid
*	¹¦ÄÜËµÃ÷: ÅÐ¶Ï°´Ñ¹ÊÇ·ñÓÐÐ§£¬¸ù¾ÝX, YµÄADCÖµ½øÐÐ´óÖÂÅÐ¶Ï
*	ÐÎ    ²Î:  ÎÞ
*	·µ »Ø Öµ: 1 ±íÊ¾ÓÐÐ§£» 0 ±íÊ¾ÎÞÐ§
*********************************************************************************************************
*/
static uint8_t	TOUCH_PressValid(uint16_t _usX, uint16_t _usY)
{
	if ((_usX <= ADC_VALID_OFFSET) || (_usY <= ADC_VALID_OFFSET)
			|| (_usX >= g_tTP.usMaxAdc - ADC_VALID_OFFSET)
			|| (_usY >= g_tTP.usMaxAdc - ADC_VALID_OFFSET))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
*********************************************************************************************************
*	Func name: TOUCH_ReadAdcX
*********************************************************************************************************
*/
uint16_t TOUCH_ReadAdcX(void)
{
	uint16_t usAdc;

	__set_PRIMASK(1);  		/* ¹ØÖÐ¶Ï */
	usAdc = g_tTP.usAdcNowX;
	__set_PRIMASK(0);  		/* ¿ªÖÐ¶Ï */

	return usAdc;
}

/*
*********************************************************************************************************
*	Func name: TOUCH_ReadAdcY
*********************************************************************************************************
*/
uint16_t TOUCH_ReadAdcY(void)
{
	uint16_t usAdc;

	__set_PRIMASK(1);  		/* ¹ØÖÐ¶Ï */
	usAdc = g_tTP.usAdcNowY;
	__set_PRIMASK(0);  		/* ¿ªÖÐ¶Ï */

	return usAdc;
}

/*
*********************************************************************************************************
* Func name: TOUCH_Abs
*********************************************************************************************************
*/
int32_t TOUCH_Abs(int32_t x)
{
	if (x >= 0)
	{
		return x;
	}
	else
	{
		return -x;
	}
}

/*
*********************************************************************************************************
*	Func name: TOUCH_SaveParam
*********************************************************************************************************
*/
extern void SaveParam(void);
static void TOUCH_SaveParam(void)
{
#if 1
	g_tParam.usAdcX1 = g_tTP.usAdcX1;
	g_tParam.usAdcY1 = g_tTP.usAdcY1;
	g_tParam.usAdcX2 = g_tTP.usAdcX2;
	g_tParam.usAdcY2 = g_tTP.usAdcY2;
	g_tParam.usAdcX3 = g_tTP.usAdcX3;
	g_tParam.usAdcY3 = g_tTP.usAdcY3;
	g_tParam.usAdcX4 = g_tTP.usAdcX4;
	g_tParam.usAdcY4 = g_tTP.usAdcY4;
	g_tParam.XYChange = g_tTP.XYChange;

	SaveParam();	/* ½«²ÎÊýÐ´ÈëFlash */
#endif
}


























