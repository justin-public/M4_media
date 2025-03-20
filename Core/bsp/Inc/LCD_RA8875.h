/*
 * LCD_RA8875.h
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */

#ifndef BSP_INC_LCD_RA8875_H_
#define BSP_INC_LCD_RA8875_H_

enum
{
	RA_FONT_16 = 0,		/* RA8875 ×ÖÌå 16µãÕó */
	RA_FONT_24 = 1,		/* RA8875 ×ÖÌå 24µãÕó */
	RA_FONT_32 = 2		/* RA8875 ×ÖÌå 32µãÕó */
};

/* ÎÄ×Ö·Å´ó²ÎÊý */
enum
{
	RA_SIZE_X1 = 0,		/* Ô­Ê¼´óÐ¡ */
	RA_SIZE_X2 = 1,		/* ·Å´ó2±¶ */
	RA_SIZE_X3 = 2,		/* ·Å´ó3±¶ */
	RA_SIZE_X4 = 3		/* ·Å´ó4±¶ */
};

/* ¿É¹©Íâ²¿Ä£¿éµ÷ÓÃµÄº¯Êý */
void RA8875_InitHard(void);
uint16_t RA8875_ReadID(void);
uint8_t RA8875_IsBusy(void);
void RA8875_Layer1Visable(void);
void RA8875_Layer2Visable(void);
void RA8875_DispOn(void);
void RA8875_DispOff(void);
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY);
void RA8875_SetFrontColor(uint16_t _usColor);
void RA8875_SetBackColor(uint16_t _usColor);
void RA8875_SetFont(uint8_t _ucFontType, uint8_t _ucLineSpace, uint8_t _ucCharSpace);
void RA8875_SetTextZoom(uint8_t _ucHSize, uint8_t _ucVSize);
void RA8875_DispAscii(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_ClrScr(uint16_t _usColor);
void RA8875_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void RA8875_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void RA8875_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_FillCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_QuitWinMode(void);
void RA8875_CtrlGPO(uint8_t _pin, uint8_t _value);
void RA8875_SetBackLight(uint8_t _bright);

void RA8875_TouchInit(void);
uint16_t RA8875_TouchReadX(void);
uint16_t RA8875_TouchReadY(void);

void RA8875_InitSPI(void);
void RA8875_Sleep(void);

/* ÏÂÃæ3¸öº¯ÊýÓÃÓÚÖ±½ÓÐ´ÏÔ´æ£¬±ÈÈçÉãÏñÍ·Ö±½ÓDMAµ½ÏÔÊ¾Æ÷. ²»Ö§³ÖSPI½Ó¿ÚÄ£Ê½ */
void RA8875_StartDirectDraw(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_QuitDirectDraw(void);

void TOUCH_Poll(void);

#endif /* BSP_INC_LCD_RA8875_H_ */
