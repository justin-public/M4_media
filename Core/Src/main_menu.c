/*
 * main_menu.c
 *
 *  Created on: Mar 14, 2025
 *      Author: compro
 */
#include "bsp.h"			
#include "images.h"
#include "main_menu.h"
#include "main.h"

#define VER_INFO	"STM32-V5(V0.63)"
#define KEY_INFO	"Original K3 System Information, Original K1 Parameter Settings"

#define ICON_LEFT	16
#define ICON_TOP	25

enum
{
	ICON1_X	= ICON_LEFT,
	ICON1_Y	= ICON_TOP,
	ICON1_H	= ICON_HEIGHT,
	ICON1_W	= ICON_WIDTH,

		ICON2_X	= ICON_LEFT + ICON_STEP_X,
		ICON2_Y	= ICON_TOP,
		ICON2_H	= ICON_HEIGHT,
		ICON2_W	= ICON_WIDTH,

			ICON3_X	= ICON_LEFT + 2 * ICON_STEP_X,
			ICON3_Y	= ICON_TOP,
			ICON3_H	= ICON_HEIGHT,
			ICON3_W	= ICON_WIDTH,

				ICON4_X	= ICON_LEFT + 3 * ICON_STEP_X,
				ICON4_Y	= ICON_TOP,
				ICON4_H	= ICON_HEIGHT,
				ICON4_W	= ICON_WIDTH,

					ICON5_X	= ICON_LEFT + 4 * ICON_STEP_X,
					ICON5_Y	= ICON_TOP,
					ICON5_H	= ICON_HEIGHT,
					ICON5_W	= ICON_WIDTH,


						ICON6_X	= ICON_LEFT + 5 * ICON_STEP_X,
						ICON6_Y	= ICON_TOP,
						ICON6_H	= ICON_HEIGHT,
						ICON6_W	= ICON_WIDTH,
	
	ICON7_X	= ICON_LEFT,
	ICON7_Y	= ICON_TOP + ICON_STEP_Y,
	ICON7_H	= ICON_HEIGHT,
	ICON7_W	= ICON_WIDTH,

		ICON8_X	= ICON_LEFT + 1 * ICON_STEP_X,
		ICON8_Y	= ICON_TOP + ICON_STEP_Y,
		ICON8_H	= ICON_HEIGHT,
		ICON8_W	= ICON_WIDTH,

			ICON9_X	= ICON_LEFT + 2 * ICON_STEP_X,
			ICON9_Y	= ICON_TOP + ICON_STEP_Y,
			ICON9_H	= ICON_HEIGHT,
			ICON9_W	= ICON_WIDTH,

				ICON10_X	= ICON_LEFT + 3 * ICON_STEP_X,
				ICON10_Y	= ICON_TOP + ICON_STEP_Y,
				ICON10_H	= ICON_HEIGHT,
				ICON10_W	= ICON_WIDTH,

					ICON11_X	= ICON_LEFT + 4 * ICON_STEP_X,
					ICON11_Y	= ICON_TOP + ICON_STEP_Y,
					ICON11_H	= ICON_HEIGHT,
					ICON11_W	= ICON_WIDTH,	
					
						ICON12_X	= ICON_LEFT + 5 * ICON_STEP_X,
						ICON12_Y	= ICON_TOP + ICON_STEP_Y,
						ICON12_H	= ICON_HEIGHT,
						ICON12_W	= ICON_WIDTH,		

	
	ICON13_X	= ICON_LEFT,
	ICON13_Y	= ICON_TOP + 2 * ICON_STEP_Y,
	ICON13_H	= ICON_HEIGHT,
	ICON13_W	= ICON_WIDTH,

		ICON14_X	= ICON_LEFT + 1 * ICON_STEP_X,
		ICON14_Y	= ICON_TOP + 2 * ICON_STEP_Y,
		ICON14_H	= ICON_HEIGHT,
		ICON14_W	= ICON_WIDTH,

			ICON15_X	= ICON_LEFT + 2 * ICON_STEP_X,
			ICON15_Y	= ICON_TOP + 2 * ICON_STEP_Y,
			ICON15_H	= ICON_HEIGHT,
			ICON15_W	= ICON_WIDTH,

				ICON16_X	= ICON_LEFT + 3 * ICON_STEP_X,
				ICON16_Y	= ICON_TOP + 2 * ICON_STEP_Y,
				ICON16_H	= ICON_HEIGHT,
				ICON16_W	= ICON_WIDTH,

					ICON17_X	= ICON_LEFT + 4 * ICON_STEP_X,
					ICON17_Y	= ICON_TOP + 2 * ICON_STEP_Y,
					ICON17_H	= ICON_HEIGHT,
					ICON17_W	= ICON_WIDTH,	
					
						ICON18_X	= ICON_LEFT + 5 * ICON_STEP_X,
						ICON18_Y	= ICON_TOP + 2 * ICON_STEP_Y,
						ICON18_H	= ICON_HEIGHT,
						ICON18_W	= ICON_WIDTH,	
};


extern const unsigned char acRecorder[48 * 48 * 4];
extern const unsigned char acTape[48 * 48 * 4];
extern const unsigned char acCamera[48 * 48 * 4];
extern const unsigned char acChiLun[48 * 48 * 4];
extern const unsigned char acchujiao[48 * 48 * 4];


#define ICONS_COUNT     18
#if 1
static const ICON_T s_tMainIcons[ICONS_COUNT] =
{	
	{ID_ICON, ICON1_X, ICON1_Y, ICON1_H, ICON1_W, (uint16_t *)achwinfo, "HW info"},
	{ID_ICON, ICON2_X, ICON2_Y, ICON2_H, ICON2_W, (uint16_t *)acLCD, "LCD"},
	{ID_ICON, ICON3_X, ICON3_Y, ICON3_H, ICON3_W, (uint16_t *)acRadio, "Radio"},
	{ID_ICON, ICON4_X, ICON4_Y, ICON4_H, ICON4_W, (uint16_t *)acRecorder, "REC"},
	{ID_ICON, ICON5_X, ICON5_Y, ICON5_H, ICON5_W, (uint16_t *)acchujiao, "MEMS"},
	{ID_ICON, ICON6_X, ICON6_Y, ICON6_H, ICON6_W, (uint16_t *)acweb, "WEB"},
	{ID_ICON, ICON7_X, ICON7_Y, ICON7_H, ICON7_W, (uint16_t *)acGPS, "GPS"},
	{ID_ICON, ICON8_X, ICON8_Y, ICON8_H, ICON8_W, (uint16_t *)acPhone, "GPRS"},
	{ID_ICON, ICON9_X, ICON9_Y, ICON9_H, ICON9_W, (uint16_t *)acMP3, "MP3"},
	{ID_ICON, ICON10_X, ICON10_Y, ICON10_H, ICON10_W, (uint16_t *)acUSBDisk, "USB"},
	{ID_ICON, ICON11_X, ICON11_Y, ICON11_H, ICON11_W, (uint16_t *)acADC, "AD7606"},
	{ID_ICON, ICON12_X, ICON12_Y, ICON12_H, ICON12_W, (uint16_t *)acCamera, "Camera"},
	{ID_ICON, ICON13_X, ICON13_Y, ICON13_H, ICON13_W, (uint16_t *)acChiLun, "OLED"},
	{ID_ICON, ICON14_X, ICON14_Y, ICON14_H, ICON14_W, (uint16_t *)acWireless, "nRF24L01"},
	{ID_ICON, ICON15_X, ICON15_Y, ICON15_H, ICON15_W, (uint16_t *)acWireless, "nRF905"},
	{ID_ICON, ICON16_X, ICON16_Y, ICON16_H, ICON16_W, (uint16_t *)acChiLun, "RS485"},
	{ID_ICON, ICON17_X, ICON17_Y, ICON17_H, ICON17_W, (uint16_t *)acChiLun, "CAN"},
	{ID_ICON, ICON18_X, ICON18_Y, ICON18_H, ICON18_W, (uint16_t *)acChiLun, "RA8875"},
};
#endif

static const uint8_t s_RetStatus[ICONS_COUNT] =
{
	MS_HARDWARE_INFO,	
	MS_TEST_TOUCH,		
	MS_RADIO,			
	MS_WM8978_REC,		
	MS_MEMS,			
	MS_ETH_WEB,			
	
	MS_GPS,				
	MS_GPRS,			
	MS_MP3,				
	MS_MASS_STORAGE,		
	MS_AD7606_TEST,			
	MS_CAMERA,			
	
	MS_OLED,			
	MS_NRF24L01,		
	MS_NRF905,				
	MS_RS485,			
	MS_CAN,				
	MS_RA8875,			
};

#define STYLE_COUNT			7		

typedef struct
{
	uint16_t *pBmp;			
	uint16_t BackColor;		
	uint16_t TextColor;		
}STYLE_T;

/*
	±³¾°Í¼Æ¬£¨RGB565¸ñÊ½£¬´æ·ÅÔÚNOR Flash
	NOR Flash »ùµØÖ· 0x6400 0000

	0x0020 0000   255K×Ö½Ú 480x272_º£Ì².bin  + 0x3FC00
	0x0023 FC00   255K×Ö½Ú 480x272_Ê÷Ò¶.bin
	0x0027 F800   255K×Ö½Ú 480x272_Ë®Öé.bin

	0x002B F400   755K×Ö½Ú 800x480_º£Ì².bin  + 0xBB800
	0x0037 AC00   755K×Ö½Ú 800x480_Ê÷Ò¶.bin
	0x0043 6400   755K×Ö½Ú 800x480_Ë®Öé.bin
*/

static const STYLE_T s_UI480[STYLE_COUNT] =
{
	{0,					CL_WHITE,			CL_BLACK},			
	{0,					RGB(0,78,152),		CL_WHITE},	
	{0,					RGB(185,193,204),	CL_BLACK},		
	{0,					RGB(38,38,38),		CL_WHITE},		
	
	{(uint16_t *)(0x64000000 + 0x00200000), 0,	CL_BLACK},	
	{(uint16_t *)(0x64000000 + 0x0023FC00), 0,	CL_WHITE},	
	{(uint16_t *)(0x64000000 + 0x0027F800), 0,	CL_BLACK},	
};

static const STYLE_T s_UI800[STYLE_COUNT] =
{
	{0,					CL_WHITE,			CL_BLACK},			
	{0,					RGB(0,78,152),		CL_WHITE},	
	{0,					RGB(185,193,204),	CL_BLACK},		
	{0,					RGB(38,38,38),		CL_WHITE},		
	
	{(uint16_t *)(0x64000000 + 0x002BF400), 0,	CL_BLACK},	
	{(uint16_t *)(0x64000000 + 0x0037AC00), 0,	CL_WHITE},	
	{(uint16_t *)(0x64000000 + 0x00436400), 0,	CL_BLACK},	
};
	
static uint16_t s_ColorIndex = 0;	


#define MAIN_TEXT_COLOR		((g_LcdWidth == 480)?s_UI480[s_ColorIndex].TextColor:s_UI800[s_ColorIndex].TextColor)

static void DispBackground(void);

/*
*********************************************************************************************************
*	Func name: MainMenu
*********************************************************************************************************
*/
uint8_t MainMenu(void)
{
	uint8_t ucKeyCode;		
	uint8_t ucTouch;		
	uint8_t fRefresh;		
	FONT_T tFont;			
	FONT_T tIconFont;		

	int16_t tpX, tpY;
	uint16_t i;

	DispBackground();

	{
		tFont.FontCode = FC_ST_16;	/* ×ÖÌå´úÂë 16µãÕó */
		tFont.FrontColor = MAIN_TEXT_COLOR;	/* ×ÖÌåÑÕÉ« */
		tFont.BackColor = CL_MASK;	/* ÎÄ×Ö±³¾°ÑÕÉ« */
		tFont.Space = 0;				/* ÎÄ×Ö¼ä¾à£¬µ¥Î» = ÏñËØ */

		tIconFont.FontCode = FC_ST_12;		/* ×ÖÌå´úÂë 16µãÕó */
		tIconFont.FrontColor = MAIN_TEXT_COLOR;	/* ×ÖÌåÑÕÉ« */
		tIconFont.BackColor = CL_MASK;		/* ÎÄ×Ö±³¾°ÑÕÉ« */
		tIconFont.Space = 1;				/* ÎÄ×Ö¼ä¾à£¬µ¥Î» = ÏñËØ */
	}
	LCD_DispStr(5, 3, VER_INFO, &tFont);			/* ÔÚ(8,3)×ø±ê´¦ÏÔÊ¾Ò»´®ºº×Ö */
	LCD_DispStr(5, g_LcdHeight - 13, KEY_INFO, &tIconFont);			/* ÔÚ(8,3)×ø±ê´¦ÏÔÊ¾Ò»´®ºº×Ö */

	fRefresh = 1;

	while(1)
	{
		bsp_Idle();
		if (fRefresh)
		{
			fRefresh = 0;
			{
				for (i = 0; i < ICONS_COUNT; i++)
				{
					LCD_DrawIcon32(&s_tMainIcons[i], &tIconFont, 0);
				}
			}
		}
		ucTouch = TOUCH_GetKey(&tpX, &tpY);
		if(ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ´¥±Ê°´ÏÂÊÂ¼þ */
				{
					for (i = 0; i < ICONS_COUNT; i++)
					{
						if (TOUCH_InRect(tpX, tpY, s_tMainIcons[i].Left, s_tMainIcons[i].Top,
							s_tMainIcons[i].Height, s_tMainIcons[i].Width))
						{
							LCD_DrawIcon32(&s_tMainIcons[i], &tIconFont, 1);
						}
					}
				}
				break;
				case TOUCH_MOVE:		/* ´¥±ÊÒÆ¶¯ÊÂ¼þ */
					break;
				case TOUCH_RELEASE:
					for (i = 0; i < ICONS_COUNT; i++)
					{
						if (TOUCH_InRect(tpX, tpY, s_tMainIcons[i].Left, s_tMainIcons[i].Top,
							s_tMainIcons[i].Height, s_tMainIcons[i].Width))
						{
							return s_RetStatus[i];
						}
					}
					{
						for (i = 0; i < ICONS_COUNT; i++)
						{
							LCD_DrawIcon32(&s_tMainIcons[i], &tIconFont, 0);
						}
					}
					break;
				default:
					break;
			}
		}
		ucKeyCode = bsp_GetKey();	/* ¶ÁÈ¡¼üÖµ, ÎÞ¼ü°´ÏÂÊ±·µ»Ø KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				case  KEY_DOWN_K3:	/* K3¼ü */
					return MS_CALIBRATION;	/* ½øÈë´¥Ãþ½çÃæ */
					//break;
				case  KEY_DOWN_K1:	/* K1¼ü */
					if (++s_ColorIndex >= STYLE_COUNT)
					{
						s_ColorIndex = 0;
					}
					{
						DispBackground();
						{
							tFont.FontCode = FC_ST_16;	/* ×ÖÌå´úÂë 16µãÕó */
							tFont.FrontColor = MAIN_TEXT_COLOR;	/* ×ÖÌåÑÕÉ« */
							tFont.BackColor = CL_MASK;	/* ÎÄ×Ö±³¾°ÑÕÉ« */
							tFont.Space = 0;				/* ÎÄ×Ö¼ä¾à£¬µ¥Î» = ÏñËØ */

							tIconFont.FontCode = FC_ST_12;		/* ×ÖÌå´úÂë 16µãÕó */
							tIconFont.FrontColor = MAIN_TEXT_COLOR;	/* ×ÖÌåÑÕÉ« */
							tIconFont.BackColor = CL_MASK;		/* ÎÄ×Ö±³¾°ÑÕÉ« */
							tIconFont.Space = 1;				/* ÎÄ×Ö¼ä¾à£¬µ¥Î» = ÏñËØ */
						}
						LCD_DispStr(5, 3, VER_INFO, &tFont);			/* ÔÚ(8,3)×ø±ê´¦ÏÔÊ¾Ò»´®ºº×Ö */
						LCD_DispStr(5, g_LcdHeight - 13, KEY_INFO, &tIconFont);			/* ÔÚ(8,3)×ø±ê´¦ÏÔÊ¾Ò»´®ºº×Ö */
					}
					fRefresh = 1;
					break;
				default:
					break;
			}
		}
	}
}





























/*
*********************************************************************************************************
*	Func name: DispBackground
*********************************************************************************************************
*/
static void DispBackground(void)
{
	if (g_LcdWidth == 480)	/* 480 x 272 */
	{
		if (s_UI480[s_ColorIndex].pBmp == 0)
		{
			LCD_ClrScr(s_UI480[s_ColorIndex].BackColor);  	/* ÇåÆÁ, Ìî³äµ¥É« */
		}
		else
		{
			LCD_DrawBMP(0, 0, g_LcdHeight, g_LcdWidth, s_UI480[s_ColorIndex].pBmp);
		}
	}
	else	/* 800 x 480 */
	{
		if (s_UI800[s_ColorIndex].pBmp == 0)
		{
			LCD_ClrScr(s_UI800[s_ColorIndex].BackColor);  	/* ÇåÆÁ, Ìî³äµ¥É« */
		}
		else
		{
			LCD_DrawBMP(0, 0, g_LcdHeight, g_LcdWidth, s_UI800[s_ColorIndex].pBmp);
		}
	}	
}
