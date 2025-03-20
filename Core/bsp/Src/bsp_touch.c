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
[1] Alphonse STM32-X2 development board + 3.0 inch display module, the touch chip on the display module is TSC2046 or its compatible chip
PA8 --> TP_CS
PD3 --> TP_BUSY
PA5 --> TP_SCK
PA6 --> TP_MISO
PA7 --> TP_MOSI
PC7 --> TP_PEN_INT
[2] Alphonse STM32 development board + 4.3 inch or 7 inch display module (with built-in RA8875 chip)
RA8875 has a built-in touch screen interface, so it can be accessed directly through FSMC to the relevant RA8875 registers.
This program does not use the touch pen interrupt function. In the 1ms periodic Systick interrupt service routine,
the touch ADC values are sampled and filtered.
After the touch pen is pressed for more than 40ms, it begins to collect ADC values (collecting once every 1ms, continuously for 10 times),
then sorts these 10 samples, discards the first 2 samples, and calculates the average value of the middle section samples.
The sampling uses a 2-point calibration method. After calibration, the ADC values of the 2 calibration points are saved.
During actual operation, the screen coordinates are calculated based on the 2-point linear equation.
The calibration parameters have a save interface.
This program is mainly used for demonstration and does not implement the save function.
You can modify the TOUCH_SaveParam() and TOUCH_LoadParam() functions to implement the save function yourself.

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

//static void TOUCH_WaitRelease(void);

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
	TOUCH_LoadParam();
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
*	Func name: CalTwoPoint
*********************************************************************************************************
*/
static int32_t CalTwoPoint(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x)
{
	return y1 + ((int32_t)(y2 - y1) * (x - x1)) / (x2 - x1);
}

/*
*********************************************************************************************************
* Func name: TOUCH_TransX
*********************************************************************************************************
*/
static int16_t TOUCH_TransX(uint16_t _usAdcX, uint16_t _usAdcY)
{
#if CALIB_POINT_COUNT == 2
	uint16_t x;
	int32_t y;

	if (g_tTP.XYChange == 0)
	{
		x = _usAdcX;
		if (x == 0)
		{
			y = 0;
		}
		else
		{
			y = CalTwoPoint(g_tTP.usAdcX1, TP_X1, g_tTP.usAdcX2, TP_X2, x);
		}
	}
	else
	{
		x = _usAdcY;
		if (x == 0)
		{
			y = 0;
		}
		else
		{
			y = CalTwoPoint(g_tTP.usAdcY1, TP_X1, g_tTP.usAdcY2, TP_X2, x);
		}
	}
	return y;
#endif
}

/*
*********************************************************************************************************
*	Func name: TOUCH_TransY
*********************************************************************************************************
*/
static int16_t TOUCH_TransY(uint16_t _usAdcX, uint16_t _usAdcY)
{
#if CALIB_POINT_COUNT == 2
	int32_t x;
	int32_t y;

	if (g_tTP.XYChange == 0)
	{
		x = _usAdcY;
		if (x == 0)
		{
			y = 0;
		}
		else
		{
			y = CalTwoPoint(g_tTP.usAdcY1, TP_Y1, g_tTP.usAdcY2, TP_Y2, x);
		}
	}
	else
	{
		x = _usAdcX;
		if (x == 0)
		{
			y = 0;
		}
		else
		{
			y = CalTwoPoint(g_tTP.usAdcX1, TP_Y1, g_tTP.usAdcX2, TP_Y2, x);
		}
	}
	return y;
#else
	int32_t x, x1, x2;
	int32_t y;

	if (g_tTP.XYChange == 0)	/* X Y ×ø±ê²»½»»» */
	{
		x = _usAdcY;

		/* ¸ù¾Ý X ADC ÊµÊ±¼ÆËãÖ±Ïß·½³ÌµÄ²Î¿¼µãx1, x2
		if  _usAdcX = usAdcX1 then  È¡µã = (AdcY1, TP_Y1, AdcY3, TP_Y3, _usAdcX)
		if  _usAdcX = usAdcX2 then  È¡µã = (AdcY4, TP_Y4, AdcY2, TP_Y2, _usAdcX)

		ÆäÖÐ TP_Y1 = TP_Y4;  TP_Y3 = TP_Y2 , ÕâÊÇ³ÌÐòÉè¶¨µÄÐ£×¼Î»ÖÃµÄÏñËØ×ø±ê, ÊÇ¹Ì¶¨µÄ¡£
		ÎÒÃÇ½öÐèÒª¶¯Ì¬¼ÆËã¶ÔµÚ1¸öºÍµÚ3¸ö²ÎÊý¡£Í¬Ñù²ÉÓÃ2µãÖ±Ïß·½³Ì¼ÆËã¡£
		*/
		x1 = CalTwoPoint(g_tTP.usAdcX1, g_tTP.usAdcY1, g_tTP.usAdcX2,  g_tTP.usAdcY4, _usAdcX);
		x2 = CalTwoPoint(g_tTP.usAdcX1, g_tTP.usAdcY3, g_tTP.usAdcX2,  g_tTP.usAdcY2, _usAdcX);
	}
	else						/* X Y ×ø±ê½»»» */
	{
		x = _usAdcX;
		/* ¸ù¾Ý X ADC ÊµÊ±¼ÆËãÖ±Ïß·½³ÌµÄ²Î¿¼µãx1, x2
		if  _usAdcY = usAdcY1 then  È¡µã = (AdcX1, TP_Y1, AdcX3, TP_Y3, _usAdcY)
		if  _usAdcY = usAdcY2 then  È¡µã = (AdcX4, TP_Y4, AdcX2, TP_Y2, _usAdcY)
		ÆäÖÐ TP_Y1 = TP_Y3;  TP_Y4 = TP_Y2 , ÕâÊÇ³ÌÐòÉè¶¨µÄÐ£×¼Î»ÖÃµÄÏñËØ×ø±ê, ÊÇ¹Ì¶¨µÄ¡£
		ÎÒÃÇ½öÐèÒª¶¯Ì¬¼ÆËã¶ÔµÚ1¸öºÍµÚ3¸ö²ÎÊý¡£Í¬Ñù²ÉÓÃ2µãÖ±Ïß·½³Ì¼ÆËã¡£
		*/
		x1 = CalTwoPoint(g_tTP.usAdcY1, g_tTP.usAdcX1, g_tTP.usAdcY2,  g_tTP.usAdcX4, _usAdcY);
		x2 = CalTwoPoint(g_tTP.usAdcY1, g_tTP.usAdcX3, g_tTP.usAdcY2,  g_tTP.usAdcX2, _usAdcY);
	}

	if (x == 0)
	{
		y = 0;
	}
	else
	{
		/* ¸ù¾Ý2µãÖ±Ïß·½³Ì£¬¼ÆËã×ø±ê */
		y = CalTwoPoint(x1, TP_Y1, x2, TP_Y2, x);
	}
	return y;
#endif
}

/*
*********************************************************************************************************
* Func name: TOUCH_PutKey
*********************************************************************************************************
*/
void TOUCH_PutKey(uint8_t _ucEvent, uint16_t _usX, uint16_t _usY)
{
	g_tTP.Event[g_tTP.Write] = _ucEvent;
	g_tTP.XBuf[g_tTP.Write] = TOUCH_TransX(_usX, _usY);
	g_tTP.YBuf[g_tTP.Write] = TOUCH_TransY(_usX, _usY);

	if (++g_tTP.Write  >= TOUCH_FIFO_SIZE)
	{
		g_tTP.Write = 0;
	}
}

/*
*********************************************************************************************************
* Func name: TOUCH_MoveValid
*********************************************************************************************************
*/
uint8_t TOUCH_MoveValid(uint16_t _usX1, uint16_t _usY1, uint16_t _usX2, uint16_t _usY2)
{
	int16_t iX, iY;
	static uint8_t s_invalid_count = 0;

	iX = TOUCH_Abs(_usX1 - _usX2);
	iY = TOUCH_Abs(_usY1 - _usY2);

	if ((iX < 25) && (iY < 25))
	{
		s_invalid_count = 0;
		return 1;
	}
	else
	{
		if (++s_invalid_count >= 3)
		{
			s_invalid_count = 0;
			return 1;
		}
		return 0;
	}
}

/*
*********************************************************************************************************
*	Func name: TOUCH_Scan
*********************************************************************************************************
*/
void TOUCH_Scan(void)
{
	uint16_t usAdcX;
	uint16_t usAdcY;
	static uint16_t s_usXBuf[SAMPLE_COUNT];
	static uint16_t s_usYBuf[SAMPLE_COUNT];
	static uint8_t s_ucPos = 0;
	static uint8_t s_count = 0;
	static uint8_t s_down = 0;
	static uint16_t s_usSaveAdcX, s_usSaveAdcY;

	if (g_tTP.Enable == 0)
	{
		return;
	}
	if (g_ChipID == IC_8875)
	{
		if (RA8875_IsBusy())
		{
			return;
		}
		usAdcX = RA8875_TouchReadX();
		usAdcY = RA8875_TouchReadY();
	}
	else
	{
		//usAdcX = TSC2046_ReadAdc(ADC_CH_X);
		//usAdcY = TSC2046_ReadAdc(ADC_CH_Y);
	}
	if (TOUCH_PressValid(usAdcX, usAdcY))
	{
		if (s_count >= 30)
		{
			s_usXBuf[s_ucPos] = usAdcX;
			s_usYBuf[s_ucPos] = usAdcY;

			if (++s_ucPos >= SAMPLE_COUNT)
			{
				s_ucPos = 0;

				g_tTP.usAdcNowX = TOUCH_DataFilter(s_usXBuf, SAMPLE_COUNT);
				g_tTP.usAdcNowY = TOUCH_DataFilter(s_usYBuf, SAMPLE_COUNT);

				if (s_down == 0)
				{
					s_down = 1;
					TOUCH_PutKey(TOUCH_DOWN, g_tTP.usAdcNowX, g_tTP.usAdcNowY);

					s_usSaveAdcX = g_tTP.usAdcNowX;
					s_usSaveAdcY = g_tTP.usAdcNowY;
				}
				else
				{
					if (TOUCH_MoveValid(s_usSaveAdcX, s_usSaveAdcY, g_tTP.usAdcNowX, g_tTP.usAdcNowY))
					{
						TOUCH_PutKey(TOUCH_MOVE, g_tTP.usAdcNowX, g_tTP.usAdcNowY);

						s_usSaveAdcX = g_tTP.usAdcNowX;
						s_usSaveAdcY = g_tTP.usAdcNowY;
					}
					else
					{
						g_tTP.usAdcNowX = 0;
					}
				}
			}
		}
		else
		{
			s_count++;
		}
	}
	else
	{
		if (s_count > 0)
		{
			if (--s_count == 0)
			{
				TOUCH_PutKey(TOUCH_RELEASE, s_usSaveAdcX, s_usSaveAdcY);

				g_tTP.usAdcNowX = 0;
				g_tTP.usAdcNowY = 0;

				s_count = 0;
				s_down = 0;
			}
		}
		s_ucPos = 0;
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

	TOUCH_CelarFIFO();

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
	//__set_PRIMASK(1);  		/* ¹ØÖÐ¶Ï */
	__enable_irq();
	g_tTP.Write = g_tTP.Read;
	//__set_PRIMASK(0);  		/* ¿ªÖÐ¶Ï */
	__disable_irq();
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
void TOUCH_WaitRelease(void)
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
*  Func name: TOUCH_DataFilter
*********************************************************************************************************
*/
static uint16_t TOUCH_DataFilter(uint16_t *_pBuf, uint8_t _ucCount)
{
	uint8_t flag;
	uint8_t i;
	uint16_t usTemp;
	uint32_t uiSum;

	do
	{
		flag = 0;
		for (i = 0; i < _ucCount - 1; i++)
		{
			if (_pBuf[i] > _pBuf[i+1])
			{
				usTemp = _pBuf[i + 1];
				_pBuf[i+1] = _pBuf[i];
				_pBuf[i] = usTemp;
				flag = 1;
			}
		}
	}while(flag);

	uiSum = 0;
	for (i = 0; i < _ucCount / 3; i++)
	{
		uiSum += _pBuf[_ucCount / 3 + i];
	}
	usTemp = uiSum / (_ucCount / 3);
	return usTemp;
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


























