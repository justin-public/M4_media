/*
 * bsp_touch.h
 *
 *  Created on: Mar 17, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_TOUCH_H_
#define BSP_INC_BSP_TOUCH_H_

#define CALIB_POINT_COUNT	2		/* 2 = 2µãÐ£×¼£» 4 = ËÄµãÐ£×¼ */

#define TOUCH_FIFO_SIZE		20
typedef struct
{
	/* 2µãÐ£×¼ ºÍ 4µãÐ£×¼ */
	uint16_t usAdcX1;	/* ×óÉÏ½Ç */
	uint16_t usAdcY1;

	uint16_t usAdcX2;	/* ÓÒÏÂ½Ç */
	uint16_t usAdcY2;

	uint16_t usAdcX3;	/* ×óÏÂ½Ç */
	uint16_t usAdcY3;

	uint16_t usAdcX4;	/* ÓÒÉÏ½Ç */
	uint16_t usAdcY4;

	uint16_t XYChange;	/* X, Y ÊÇ·ñ½»»»  */

	uint16_t usMaxAdc;	/* ´¥Ãþ°å×î´óADCÖµ£¬ÓÃÓÚÓÐÐ§µãÅÐ¶Ï. ×îÐ¡ADC = 0  */
	uint16_t usAdcNowX;
	uint16_t usAdcNowY;

	uint8_t Enable;		/* ´¥Ãþ¼ì²âÊ¹ÄÜ±êÖ¾ */

	uint8_t Event[TOUCH_FIFO_SIZE];	/* ´¥ÃþÊÂ¼þ */
	int16_t XBuf[TOUCH_FIFO_SIZE];	/* ´¥Ãþ×ø±ê»º³åÇø */
	int16_t YBuf[TOUCH_FIFO_SIZE];	/* ´¥Ãþ×ø±ê»º³åÇø */
	uint8_t Read;					/* »º³åÇø¶ÁÖ¸Õë */
	uint8_t Write;					/* »º³åÇøÐ´Ö¸Õë */
}TOUCH_T;

/* ´¥ÃþÊÂ¼þ */
enum
{
	TOUCH_NONE = 0,		/* ÎÞ´¥Ãþ */
	TOUCH_DOWN = 1,		/* °´ÏÂ */
	TOUCH_MOVE = 2,		/* ÒÆ¶¯ */
	TOUCH_RELEASE = 3	/* ÊÍ·Å */
};

/* ¹©Íâ²¿µ÷ÓÃµÄº¯ÊýÉùÃ÷ */
void TOUCH_InitHard(void);
void TOUCH_Calibration(void);

uint16_t TOUCH_ReadAdcX(void);
uint16_t TOUCH_ReadAdcY(void);

int16_t TOUCH_GetX(void);
int16_t TOUCH_GetY(void);

uint8_t TOUCH_GetKey(int16_t *_pX, int16_t *_pY);
void TOUCH_CelarFIFO(void);

uint8_t TOUCH_InRect(uint16_t _usX, uint16_t _usY,
uint16_t _usRectX, uint16_t _usRectY, uint16_t _usRectH, uint16_t _usRectW);

void TOUCH_Scan(void);
int32_t TOUCH_Abs(int32_t x);

extern TOUCH_T g_tTP;

#endif /* BSP_INC_BSP_TOUCH_H_ */
