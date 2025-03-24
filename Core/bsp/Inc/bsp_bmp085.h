/*
 * bsp_bmp085.h
 *
 *  Created on: Mar 24, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_BMP085_H_
#define BSP_INC_BSP_BMP085_H_

#define BMP085_SLAVE_ADDRESS    0xEE		/* I2C´Ó»úµØÖ· */

typedef struct
{
	/* ÓÃÓÚ±£´æÐ¾Æ¬ÄÚ²¿EEPROMµÄÐ£×¼²ÎÊý */
	int16_t AC1;
	int16_t AC2;
	int16_t AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;
	int16_t B1;
	int16_t B2;
	int16_t MB;
	int16_t MC;
	int16_t MD;

	uint8_t OSS;	/* ¹ý²ÉÑùÖµ£¬¿ÉÓÐÓÃ»§×Ô¼ºÉè¶¨ */

	/* ÏÂÃæ2¸öµ¥ÔªÓÃÓÚ´æ·Å¼ÆËãµÄÕæÊµÖµ */
	int32_t Temp;	/* ÎÂ¶ÈÖµ£¬ µ¥Î» 0.1ÉãÊÏ¶È */
	int32_t Press;	/* Ñ¹Á¦Öµ£¬ µ¥Î» Pa */
}BMP085_T;

extern BMP085_T g_tBMP085;

void bsp_InitBMP085(void);
void BMP085_ReadTempPress(void);

#endif /* BSP_INC_BSP_BMP085_H_ */
