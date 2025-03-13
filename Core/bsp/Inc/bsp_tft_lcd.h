/*
 * bsp_tft_lcd.h
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_TFT_LCD_H_
#define BSP_INC_BSP_TFT_LCD_H_

/* ¶¨ÒåLCDÏÔÊ¾ÇøÓòµÄ·Ö±æÂÊ */
#define LCD_30_HEIGHT	240		/* 3.0´ç¿íÆÁ ¸ß¶È£¬µ¥Î»£ºÏñËØ */
#define LCD_30_WIDTH	400		/* 3.0´ç¿íÆÁ ¿í¶È£¬µ¥Î»£ºÏñËØ */

#define LCD_43_HEIGHT	272		/* 4.3´ç¿íÆÁ ¸ß¶È£¬µ¥Î»£ºÏñËØ */
#define LCD_43_WIDTH	480		/* 4.3´ç¿íÆÁ ¿í¶È£¬µ¥Î»£ºÏñËØ */

#define LCD_70_HEIGHT	480		/* 7.0´ç¿íÆÁ ¸ß¶È£¬µ¥Î»£ºÏñËØ */
#define LCD_70_WIDTH	800		/* 7.0´ç¿íÆÁ ¿í¶È£¬µ¥Î»£ºÏñËØ */

/* Ö§³ÖµÄÇý¶¯Ð¾Æ¬ID */
enum
{
	IC_5420		= 0x5420,
	IC_4001		= 0x4001,
	IC_61509 	= 0xB509,
	IC_8875 	= 0x0075
};

#define CHIP_STR_5420	"SPFD5420A"
#define CHIP_STR_4001	"OTM4001A"
#define CHIP_STR_61509	"R61509V"
#define CHIP_STR_8875	"RA8875"

/*
	LCD ÑÕÉ«´úÂë£¬CL_ÊÇColorµÄ¼òÐ´
	16BitÓÉ¸ßÎ»ÖÁµÍÎ»£¬ RRRR RGGG GGGB BBBB

	ÏÂÃæµÄRGB ºê½«24Î»µÄRGBÖµ×ª»»Îª16Î»¸ñÊ½¡£
	Æô¶¯windowsµÄ»­±Ê³ÌÐò£¬µã»÷±à¼­ÑÕÉ«£¬Ñ¡Ôñ×Ô¶¨ÒåÑÕÉ«£¬¿ÉÒÔ»ñµÃµÄRGBÖµ¡£

	ÍÆ¼öÊ¹ÓÃÃÔÄãÈ¡É«Æ÷Èí¼þ»ñµÃÄã¿´µ½µÄ½çÃæÑÕÉ«¡£
*/
#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	/* ½«8Î»R,G,B×ª»¯Îª 16Î»RGB565¸ñÊ½ */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)
enum
{
	CL_WHITE        = RGB(255,255,255),	/* °×É« */
	CL_BLACK        = RGB(  0,  0,  0),	/* ºÚÉ« */
	CL_RED          = RGB(255,	0,  0),	/* ºìÉ« */
	CL_GREEN        = RGB(  0,255,  0),	/* ÂÌÉ« */
	CL_BLUE         = RGB(  0,	0,255),	/* À¶É« */
	CL_YELLOW       = RGB(255,255,  0),	/* »ÆÉ« */

	CL_GREY			= RGB( 98, 98, 98), 	/* Éî»ÒÉ« */
	CL_GREY1		= RGB( 150, 150, 150), 	/* Ç³»ÒÉ« */
	CL_GREY2		= RGB( 180, 180, 180), 	/* Ç³»ÒÉ« */
	CL_GREY3		= RGB( 200, 200, 200), 	/* ×îÇ³»ÒÉ« */
	CL_GREY4		= RGB( 230, 230, 230), 	/* ×îÇ³»ÒÉ« */

	CL_BUTTON_GREY	= RGB( 220, 220, 220), /* WINDOWS °´Å¥±íÃæ»ÒÉ« */

	CL_MAGENTA      = 0xF81F,	/* ºì×ÏÉ«£¬ÑóºìÉ« */
	CL_CYAN         = 0x7FFF,	/* À¶ÂÌÉ«£¬ÇàÉ« */

	CL_BLUE1        = RGB(  0,  0, 240),		/* ÉîÀ¶É« */
	CL_BLUE2        = RGB(  0,  0, 128),		/* ÉîÀ¶É« */
	CL_BLUE3        = RGB(  68, 68, 255),		/* Ç³À¶É«1 */
	CL_BLUE4        = RGB(  0, 64, 128),		/* Ç³À¶É«1 */

	/* UI ½çÃæ Windows¿Ø¼þ³£ÓÃÉ« */
	CL_BTN_FACE		= RGB(236, 233, 216),	/* °´Å¥±íÃæÑÕÉ«(»Ò) */
	CL_BOX_BORDER1	= RGB(172, 168,153),	/* ·Ö×é¿òÖ÷ÏßÑÕÉ« */
	CL_BOX_BORDER2	= RGB(255, 255,255),	/* ·Ö×é¿òÒõÓ°ÏßÑÕÉ« */


	CL_MASK			= 0x9999	/* ÑÕÉ«ÑÚÂë£¬ÓÃÓÚÎÄ×Ö±³¾°Í¸Ã÷ */
};

/* ±à¼­¿ò·ç¸ñ */
enum
{
	EDIT_BORDER_COLOR		= CL_BLUE2,		/* ±à¼­¿òËÄ¸ö±ßµÄÑÕÉ« */
	EDIT_BACK_COLOR			= CL_WHITE,			/* ±à¼­¿ò±³¾° */
};

/* °´Å¥·ç¸ñ */
enum
{
	BUTTON_BORDER_COLOR		= CL_BLUE2,			/* °´Å¥ËÄ¸ö±ßµÄÑÕÉ« */
	BUTTON_BORDER1_COLOR	= CL_WHITE,			/* °´Å¥ËÄ¸ö±ßÄÚ²¿2ÁÐµÄÑÕÉ« */
	BUTTON_BORDER2_COLOR	= CL_GREY1,			/* °´Å¥ËÄ¸ö±ßÄÚ²¿2ÁÐµÄÑÕÉ« */
	BUTTON_BACK_COLOR		= CL_GREY3,			/* °´Å¥±³¾° */
	BUTTON_ACTIVE_COLOR		= CL_CYAN,			/* ¼¤»îµÄ°´Å¥±³¾° */
};

/* ´°¿Ú·ç¸ñ */
enum
{
	WIN_BORDER_COLOR	= CL_BLUE4,		/* ´°¿Ú±ß¿ò */
	WIN_TITLE_COLOR		= CL_BLUE3,		/* ´°¿Ú±êÌâÀ¶±³¾°ÑÕÉ« */
	WIN_CAPTION_COLOR	= CL_WHITE,		/* ´°¿Ú±êÌâÀ¸ÎÄ×ÖÑÕÉ« */
	WIN_BODY_COLOR		= CL_GREY2,		/* ´°ÌåÑÕÉ« */
};

/* ¼ì²é¿ò·ç¸ñ */
enum
{
	CHECK_BOX_BORDER_COLOR	= CL_BLUE2,		/* ¼ì²é¿òËÄ¸ö±ßµÄÑÕÉ« */
	CHECK_BOX_BACK_COLOR	= CL_GREY3,		/* ¼ì²é¿ò±³¾° */
	CHECK_BOX_CHECKED_COLOR	= CL_RED,		/* ¼ì²é¿ò´ò¹´µÄÑÕÉ« */

	CHECK_BOX_H			= 24,				/* ¼ì²é¿ò¸ß¶È */
	CHECK_BOX_W			= 24,				/* ¼ì²é¿ò¸ß¶È */
};

/* ×ÖÌå´úÂë */
typedef enum
{
	FC_ST_16 = 0,		/* ËÎÌå15x16µãÕó £¨¿íx¸ß£© */
	FC_ST_12 = 1		/* ËÎÌå12x12µãÕó £¨¿íx¸ß£© */
}FONT_CODE_E;

/* ×ÖÌåÊôÐÔ½á¹¹, ÓÃÓÚLCD_DispStr() */
typedef struct
{
	uint16_t FontCode;	/* ×ÖÌå´úÂë 0 ±íÊ¾16µãÕó */
	uint16_t FrontColor;	/* ×ÖÌåÑÕÉ« */
	uint16_t BackColor;	/* ÎÄ×Ö±³¾°ÑÕÉ«£¬Í¸Ã÷ */
	uint16_t Space;		/* ÎÄ×Ö¼ä¾à£¬µ¥Î» = ÏñËØ */
}FONT_T;

/* ¿Ø¼þID */
typedef enum
{
	ID_ICON		= 1,
	ID_WIN		= 2,
	ID_LABEL	= 3,
	ID_BUTTON	= 4,
	ID_CHECK 	= 5,
	ID_EDIT 	= 6,
	ID_GROUP 	= 7,
}CONTROL_ID_T;

/* Í¼±ê½á¹¹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;		/* ×óÉÏ½ÇX×ø±ê */
	uint16_t Top;		/* ×óÉÏ½ÇY×ø±ê */
	uint16_t Height;	/* Í¼±ê¸ß¶È */
	uint16_t Width;		/* Í¼±ê¿í¶È */
	uint16_t *pBmp;		/* Ö¸ÏòÍ¼±êÍ¼Æ¬Êý¾Ý */
	char  Text[16];	/* Í¼±êÎÄ±¾, ×î¶àÏÔÊ¾5¸öºº×Ö16µãÕó */
}ICON_T;

/* ´°Ìå½á¹¹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;
	char *pCaption;
}WIN_T;

/* ÎÄ±¾±êÇ©½á¹¹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* ×óÉÏ½ÇX×ø±ê */
	uint16_t Top;			/* ×óÉÏ½ÇY×ø±ê */
	uint16_t Height;		/* ¸ß¶È */
	uint16_t Width;			/* ¿í¶È */
	uint16_t MaxLen;		/* ×Ö·û´®³¤¶È */
	FONT_T *Font;			/* ×ÖÌå */
	char  *pCaption;
}LABEL_T;

/* °´Å¥½á¹¹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	/* °´Å¥µÄÑÕÉ«£¬ÓÉµ×²ã×Ô¶¯¹ÜÀí */
	FONT_T *Font;			/* ×ÖÌå */
	char *pCaption;
	uint8_t Focus;			/* ½¹µã */
}BUTTON_T;

/* ±à¼­¿ò½á¹¹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;
	uint16_t Top;
	uint16_t Height;
	uint16_t Width;
	uint16_t Color;
	FONT_T *Font;			/* ×ÖÌå */
	char   *pCaption;
	char Text[32];			/* ÓÃÓÚ´æ·Å±à¼­ÄÚÈÝ */
}EDIT_T;

/* ¼ì²é¿ò CHECK BOX ½á¹¹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* ×óÉÏ½ÇX×ø±ê */
	uint16_t Top;			/* ×óÉÏ½ÇY×ø±ê */
	uint16_t Height;		/* ¸ß¶È */
	uint16_t Width;			/* ¿í¶È */
	uint16_t Color;			/* ÑÕÉ« */
	FONT_T *Font;			/* ×ÖÌå */
	char  *pCaption;
	uint8_t Checked;		/* 1±íÊ¾´ò¹´ */
}CHECK_T;

/* ·Ö×é¿òGROUP BOX ½á¹¹ */
typedef struct
{
	uint8_t id;
	uint16_t Left;			/* ×óÉÏ½ÇX×ø±ê */
	uint16_t Top;			/* ×óÉÏ½ÇY×ø±ê */
	uint16_t Height;		/* ¸ß¶È */
	uint16_t Width;			/* ¿í¶È */
	FONT_T *Font;			/* ×ÖÌå */
	char  *pCaption;
}GROUP_T;

/* ±³¾°¹â¿ØÖÆ */
#define BRIGHT_MAX		255
#define BRIGHT_MIN		0
#define BRIGHT_DEFAULT	200
#define BRIGHT_STEP		5

/* ¿É¹©Íâ²¿Ä£¿éµ÷ÓÃµÄº¯Êý */
void LCD_InitHard(void);
void LCD_GetChipDescribe(char *_str);
uint16_t LCD_GetHeight(void);
uint16_t LCD_GetWidth(void);
void LCD_DispOn(void);
void LCD_DispOff(void);
void LCD_ClrScr(uint16_t _usColor);
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t LCD_GetPixel(uint16_t _usX, uint16_t _usY);
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor);
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void LCD_SetBackLight(uint8_t _bright);
uint8_t LCD_GetBackLight(void);

void LCD_Fill_Rect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);

void LCD_DrawWin(WIN_T *_pWin);
void LCD_DrawIcon(const ICON_T *_tIcon, FONT_T *_tFont, uint8_t _ucFocusMode);
void LCD_DrawEdit(EDIT_T *_pEdit);
void LCD_DrawButton(BUTTON_T *_pBtn);
void LCD_DrawLabel(LABEL_T *_pLabel);
void LCD_DrawCheckBox(CHECK_T *_pCheckBox);
void LCD_DrawGroupBox(GROUP_T *_pBox);

void LCD_DispControl(void *_pControl);

void LCD_DrawIcon32(const ICON_T *_tIcon, FONT_T *_tFont, uint8_t _ucFocusMode);
void LCD_DrawBmp32(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_pBmp);

/* ÏÂÃæ3¸ö±äÁ¿£¬Ö÷ÒªÓÃÓÚÊ¹³ÌÐòÍ¬Ê±Ö§³Ö²»Í¬µÄÆÁ */
extern uint16_t g_ChipID;			/* Çý¶¯Ð¾Æ¬ID */
extern uint16_t g_LcdHeight;		/* ÏÔÊ¾ÆÁ·Ö±æÂÊ-¸ß¶È */
extern uint16_t g_LcdWidth;			/* ÏÔÊ¾ÆÁ·Ö±æÂÊ-¿í¶È */

#endif /* BSP_INC_BSP_TFT_LCD_H_ */
