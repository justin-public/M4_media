/*
 * bsp_tft_lcd.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "bsp.h"
#include "fonts.h"

/* ÏÂÃæ3¸ö±äÁ¿£¬Ö÷ÒªÓÃÓÚÊ¹³ÌÐòÍ¬Ê±Ö§³Ö²»Í¬µÄÆÁ */
uint16_t g_ChipID = IC_4001;		/* Çý¶¯Ð¾Æ¬ID */
uint16_t g_LcdHeight = 240;			/* ÏÔÊ¾ÆÁ·Ö±æÂÊ-¸ß¶È */
uint16_t g_LcdWidth = 400;			/* ÏÔÊ¾ÆÁ·Ö±æÂÊ-¿í¶È */
uint8_t s_ucBright;					/* ±³¹âÁÁ¶È²ÎÊý */

static void LCD_CtrlLinesConfig(void);
static void LCD_FSMCConfig(void);

/*
*********************************************************************************************************
*	Func name: LCD_InitHard
*********************************************************************************************************
*/
void LCD_InitHard(void)
{
	uint16_t id;

	LCD_CtrlLinesConfig();

	LCD_FSMCConfig();

	HAL_Delay(20);

	id = SPFD5420_ReadID();
	if ((id == 0x5420) || (id ==  0xB509) || (id == 0x5520))
	{
		//SPFD5420_InitHard();
	}
	else
	{
		if (RA8875_ReadID() == 0x75)
		{
			//UART1_Transmit_String("LCD Controller is RA8875 or compatible\r\n");
			g_ChipID = IC_8875;
			RA8875_InitHard();
		}
		else
		{
			g_ChipID = IC_4001;
			g_LcdHeight = LCD_30_HEIGHT;
			g_LcdWidth = LCD_30_WIDTH;
		}
	}
	LCD_ClrScr(CL_BLUE);
	LCD_SetBackLight(BRIGHT_DEFAULT);
}


static void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_FSMC_CLK_ENABLE();

	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |
		                     GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 |
		                     GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOD,&GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
		                     GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |
							 GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_13;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOD,&GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_12;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);
}

static void LCD_FSMCConfig(void)
{
	//FSMC_NORSRAM_InitTypeDef  init;
	SRAM_HandleTypeDef hsram;
	FSMC_NORSRAM_TimingTypeDef  timingWrite;
	FSMC_NORSRAM_TimingTypeDef  timingRead;

	timingWrite.AddressSetupTime = 4;
	timingWrite.AddressHoldTime = 0;
	timingWrite.DataSetupTime = 6;
	timingWrite.BusTurnAroundDuration = 1;
	timingWrite.CLKDivision = 0;
	timingWrite.DataLatency = 0;
	timingWrite.AccessMode = FSMC_ACCESS_MODE_A;

	timingRead.AddressSetupTime = 4;
	timingRead.AddressHoldTime = 0;
	timingRead.DataSetupTime = 8;
	timingRead.BusTurnAroundDuration = 0;
	timingRead.CLKDivision = 0;
	timingRead.DataLatency = 0;
	timingRead.AccessMode = FSMC_ACCESS_MODE_A;

	hsram.Instance = FSMC_NORSRAM_DEVICE;
	hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	hsram.Init.NSBank = FSMC_NORSRAM_BANK4;
	hsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hsram.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	//hsram.Init.ContinuousClock = FSMC_CONTINUOUS_CLOCK_SYNC_ONLY; /* 새로운 파라미터 */

	HAL_SRAM_Init(&hsram, &timingRead, &timingWrite);
}

/*
*********************************************************************************************************
*	Func name: LCD_ClrScr
*********************************************************************************************************
*/
void LCD_ClrScr(uint16_t _usColor)
{
	if (g_ChipID == IC_8875)	/* RA8875 ·ÖÖ§ */
	{
		//UART1_Transmit_String("LCD Controller is RA8875 color\r\n");
		RA8875_ClrScr(_usColor);
	}
	else	/* 5420£¬4001£¬61509 ·ÖÖ§ */
	{
		UART1_Transmit_String("LCD Controller is SFD5420 color\r\n");
		//SPFD5420_ClrScr(_usColor);
	}
}
/*
*********************************************************************************************************
* 	Func name: LCD_SetBackLight
*********************************************************************************************************
*/
void LCD_SetBackLight(uint8_t _bright)
{
	s_ucBright =  _bright;	/* ±£´æ±³¹âÖµ */

	if (g_ChipID == IC_8875)
	{
		RA8875_SetBackLight(_bright);
	}
	else
	{
		//SPFD5420_SetBackLight(_bright);
	}
}

/*
*********************************************************************************************************
*	Func name: LCD_DrawBMP
*********************************************************************************************************
*/
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_DrawBMP(_usX, _usY, _usHeight, _usWidth, _ptr);
	}
	else
	{
		//SPFD5420_DrawBMP(_usX, _usY, _usHeight, _usWidth, _ptr);
	}
}

/*
*********************************************************************************************************
*	Func name: LCD_DispStr
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint8_t buf[24 * 24 / 8];	/* ×î´óÖ§³Ö24µãÕóºº×Ö */
	uint8_t width;
	uint16_t m;
	uint8_t font_width,font_height, font_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;

#ifdef USE_SMALL_FONT
	const uint8_t *pHzDot;
#else
	uint32_t AddrHZK;
#endif
	if (_tFont->FontCode == FC_ST_12)
	{
		font_height = 12;
		font_width = 12;
		font_bytes = 24;
		pAscDot = g_Ascii12;

		#ifdef USE_SMALL_FONT
			pHzDot = g_Hz12;
		#else
			AddrHZK = HZK12_ADDR;
		#endif
		}
		else
		{
			font_height = 16;
			font_width = 16;
			font_bytes = 32;
			pAscDot = g_Ascii16;

		#ifdef USE_SMALL_FONT
			pHzDot = g_Hz16;
		#else
			AddrHZK = HZK16_ADDR;
		#endif
		}

		while (*_ptr != 0)
		{
			code1 = *_ptr;
			if (code1 < 0x80)
			{
				memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
				width = font_width / 2;
			}
			else
			{
				code2 = *++_ptr;
				if (code2 == 0)
				{
					break;
				}
				#ifdef USE_SMALL_FONT
					m = 0;
					while(1)
					{
						address = m * (font_bytes + 2);
						m++;
						if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
						{
							address += 2;
							memcpy(buf, &pHzDot[address], font_bytes);
							break;
						}
						else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
						{
							/* ×Ö¿âËÑË÷Íê±Ï£¬Î´ÕÒµ½£¬ÔòÌî³äÈ«FF */
							memset(buf, 0xFF, font_bytes);
							break;
						}
					}
				#else	/* ÓÃÈ«×Ö¿â */
					/* ´Ë´¦ÐèÒª¸ù¾Ý×Ö¿âÎÄ¼þ´æ·ÅÎ»ÖÃ½øÐÐÐÞ¸Ä */
					if (code1 >=0xA1 && code1 <= 0xA9 && code2 >=0xA1)
					{
						address = ((code1 - 0xA1) * 94 + (code2 - 0xA1)) * font_bytes + AddrHZK;
					}
					else if (code1 >=0xB0 && code1 <= 0xF7 && code2 >=0xA1)
					{
						address = ((code1 - 0xB0) * 94 + (code2 - 0xA1) + 846) * font_bytes + AddrHZK;
					}
					memcpy(buf, (const uint8_t *)address, font_bytes);
				#endif
					width = font_width;
			}
			y = _usY;
			/* ¿ªÊ¼Ë¢LCD */
			for (m = 0; m < font_height; m++)	/* ×Ö·û¸ß¶È */
			{
				x = _usX;
				for (i = 0; i < width; i++)	/* ×Ö·û¿í¶È */
				{
					if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
					{
						LCD_PutPixel(x, y, _tFont->FrontColor);	/* ÉèÖÃÏñËØÑÕÉ«ÎªÎÄ×ÖÉ« */
					}
					else
					{
						if (_tFont->BackColor != CL_MASK)	/* Í¸Ã÷É« */
						{
							LCD_PutPixel(x, y, _tFont->BackColor);	/* ÉèÖÃÏñËØÑÕÉ«ÎªÎÄ×Ö±³¾°É« */
						}
					}
					x++;
				}
				y++;
			}

			if (_tFont->Space > 0)
			{
				/* Èç¹ûÎÄ×Öµ×É«°´_tFont->usBackColor£¬²¢ÇÒ×Ö¼ä¾à´óÓÚµãÕóµÄ¿í¶È£¬ÄÇÃ´ÐèÒªÔÚÎÄ×ÖÖ®¼äÌî³ä(ÔÝÊ±Î´ÊµÏÖ) */
			}
			_usX += width + _tFont->Space;	/* ÁÐµØÖ·µÝÔö */
			_ptr++;			/* Ö¸ÏòÏÂÒ»¸ö×Ö·û */
		}
}

/*
*********************************************************************************************************
*	Func name: LCD_PutPixel
*********************************************************************************************************
*/
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_PutPixel(_usX, _usY, _usColor);
	}
	else
	{
		//SPFD5420_PutPixel(_usX, _usY, _usColor);
	}
}




























