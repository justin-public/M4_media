/*
 * hart_test.c
 *
 *  Created on: Mar 17, 2025
 *      Author: compro
 */
#include "bsp.h"			/* printfº¯Êý¶¨ÏòÊä³öµ½´®¿Ú£¬ËùÒÔ±ØÐë°üº¬Õâ¸öÎÄ¼þ */
#include "hard_test.h"

#define BUTTON_RET_H	32
#define BUTTON_RET_W	60
#define	BUTTON_RET_X	(g_LcdWidth - BUTTON_RET_W - 4)
#define	BUTTON_RET_Y	(g_LcdHeight - BUTTON_RET_H - 4)

/*
*********************************************************************************************************
*	Func name: HardInfo
*********************************************************************************************************
*/
void HardInfo(void)
{
	uint8_t ucKeyCode;		/* °´¼ü´úÂë */
	uint8_t ucTouch;		/* ´¥ÃþÊÂ¼þ */
	uint8_t fRefresh;		/* Ë¢ÆÁÇëÇó±êÖ¾,1±íÊ¾ÐèÒªË¢ÐÂ */
	FONT_T tFont, tFontBtn;	/* ¶¨ÒåÒ»¸ö×ÖÌå½á¹¹Ìå±äÁ¿£¬ÓÃÓÚÉèÖÃ×ÖÌå²ÎÊý */
	char buf[128];
	uint16_t x, y;
	uint16_t usLineCap = 18;

	int16_t tpX, tpY;
	BUTTON_T tBtn;

	LCD_ClrScr(CL_BLUE);  	/* ÇåÆÁ£¬±³¾°À¶É« */

	{
		tFont.FontCode = FC_ST_16;	/* ×ÖÌå´úÂë 16µãÕó */
		tFont.FrontColor = CL_WHITE;	/* ×ÖÌåÑÕÉ« */
		tFont.BackColor = CL_BLUE;	/* ÎÄ×Ö±³¾°ÑÕÉ« */
		tFont.Space = 0;				/* ÎÄ×Ö¼ä¾à£¬µ¥Î» = ÏñËØ */

		/* °´Å¥×ÖÌå */
		tFontBtn.FontCode = FC_ST_16;
		tFontBtn.BackColor = CL_MASK;	/* Í¸Ã÷É« */
		tFontBtn.FrontColor = CL_BLACK;
		tFontBtn.Space = 0;
	}
	x = 5;
	y = 3;
	LCD_DispStr(x, y, "°²¸»À³STM32-V5¿ª·¢°åÓ²¼þÐÅÏ¢", &tFont);			/* ÔÚ(8,3)×ø±ê´¦ÏÔÊ¾Ò»´®ºº×Ö */
	y += usLineCap;
}
