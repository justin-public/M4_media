/*
 * bsp_bh1750.h
 *
 *  Created on: Mar 24, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_BH1750_H_
#define BSP_INC_BSP_BH1750_H_

#define BH1750_SLAVE_ADDRESS    0x46		/* I2C´Ó»úµØÖ· */

/* ²Ù×÷Âë Opercode ¶¨Òå */
enum
{
	BHOP_POWER_DOWN = 0x00,		/* ½øÈëµôµçÄ£Ê½¡£Ð¾Æ¬ÉÏµçºóÈ±Ê¡¾ÍÊÇPowerDownÄ£Ê½ */
	BHOP_POWER_ON = 0x01,		/* ÉÏµç£¬µÈ´ý²âÁ¿ÃüÁî */
	BHOP_RESET = 0x07,			/* ÇåÁãÊý¾Ý¼Ä´æÆ÷ (Power Down Ä£Ê½ÎÞÐ§) */
	BHOP_CON_H_RES  = 0x10,		/* Á¬Ðø¸ß·Ö±æÂÊ²âÁ¿Ä£Ê½  £¨²âÁ¿Ê±¼ä 120ms£© £¨×î´ó 180ms£©*/
	BHOP_CON_H_RES2 = 0x11,		/* Á¬Ðø¸ß·Ö±æÂÊ²âÁ¿Ä£Ê½2 £¨²âÁ¿Ê±¼ä 120ms£©*/
	BHOP_CON_L_RES = 0x13,		/* Á¬ÐøµÍ·Ö±æÂÊ²âÁ¿Ä£Ê½ £¨²âÁ¿Ê±¼ä 16ms£©*/

	BHOP_ONE_H_RES  = 0x20,		/* µ¥´Î¸ß·Ö±æÂÊ²âÁ¿Ä£Ê½ , Ö®ºó×Ô¶¯½øÈëPower Down */
	BHOP_ONE_H_RES2 = 0x21,		/* µ¥´Î¸ß·Ö±æÂÊ²âÁ¿Ä£Ê½2 , Ö®ºó×Ô¶¯½øÈëPower Down  */
	BHOP_ONE_L_RES = 0x23,		/* µ¥´ÎµÍ·Ö±æÂÊ²âÁ¿Ä£Ê½ , Ö®ºó×Ô¶¯½øÈëPower Down  */
};

void bsp_InitBH1750(void);
void BH1750_WriteCmd(uint8_t _ucOpecode);
uint16_t BH1750_ReadData(void);
void BH1750_AdjustSensitivity(uint8_t _ucMTReg);
void BH1750_ChageMode(uint8_t _ucMode);
float BH1750_GetLux(void);

#endif /* BSP_INC_BSP_BH1750_H_ */
