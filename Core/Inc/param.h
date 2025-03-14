/*
 * param.h
 *
 *  Created on: Mar 14, 2025
 *      Author: compro
 */

#ifndef INC_PARAM_H_
#define INC_PARAM_H_

/* ÏÂÃæ2ÐÐºêÖ»ÄÜÑ¡ÔñÆäÒ» */
#define PARAM_SAVE_TO_EEPROM			/* ²ÎÊý´æ´¢µ½Íâ²¿µÄEEPROM (AT24C128) */
//#define PARAM_SAVE_TO_FLASH		/* ²ÎÊý´æ´¢µ½CPUÄÚ²¿Flash */

#ifdef PARAM_SAVE_TO_EEPROM
	#define PARAM_ADDR		0			/* ²ÎÊýÇøµØÖ· */
#endif

#ifdef PARAM_SAVE_TO_FLASH
	#define PARAM_ADDR		ADDR_FLASH_SECTOR_3			/* 0x0800C000 ÖÐ¼äµÄ16KBÉÈÇøÓÃÀ´´æ·Å²ÎÊý */
	//#define PARAM_ADDR	 ADDR_FLASH_SECTOR_11		/* 0x080E0000 Flash×îºó128KÉÈÇøÓÃÀ´´æ·Å²ÎÊý */
#endif

#define PARAM_VER			0x00000102					/* ²ÎÊý°æ±¾ */

/* È«¾Ö²ÎÊý */
typedef struct
{
	uint32_t ParamVer;			/* ²ÎÊýÇø°æ±¾¿ØÖÆ£¨¿ÉÓÃÓÚ³ÌÐòÉý¼¶Ê±£¬¾ö¶¨ÊÇ·ñ¶Ô²ÎÊýÇø½øÐÐÉý¼¶£© */

	/* LCD±³¹âÁÁ¶È */
	uint8_t ucBackLight;

	/* ´¥ÃþÆÁÐ£×¼²ÎÊý */
	uint16_t usAdcX1;	/* ×óÉÏ½Ç */
	uint16_t usAdcY1;
	uint16_t usAdcX2;	/* ÓÒÏÂ½Ç */
	uint16_t usAdcY2;
	uint16_t usAdcX3;	/* ×óÏÂ½Ç */
	uint16_t usAdcY3;
	uint16_t usAdcX4;	/* ÓÒÉÏ½Ç */
	uint16_t usAdcY4;
	uint16_t XYChange;	/* X, Y ÊÇ·ñ½»»»  */

	/* uip ip µØÖ·²ÎÊý */
	uint8_t uip_ip[4];			/* ±¾»úIPµØÖ· */
	uint8_t uip_net_mask[4];	/* ×ÓÍøÑÚÂë */
	uint8_t uip_net_gate[4];	/* Ä¬ÈÏÍø¹Ø */

	/* lwip ip µØÖ·²ÎÊý */
	uint8_t lwip_ip[4];			/* ±¾»úIPµØÖ· */
	uint8_t lwip_net_mask[4];	/* ×ÓÍøÑÚÂë */
	uint8_t lwip_net_gate[4];	/* Ä¬ÈÏÍø¹Ø */

	/* ÊÕÒô»ú²ÎÊý */
	uint8_t ucRadioMode;		/* AM »ò FM */
	uint8_t ucRadioListType;		/* µçÌ¨ÁÐ±íÀàÐÍ¡£ÎäººµØÇø»òÈ«¹ú */
	uint8_t ucIndexFM;			/* µ±Ç°FMµçÌ¨Ë÷Òý */
	uint8_t ucIndexAM;			/* µ±Ç°µçÌ¨Ë÷Òý */
	uint8_t ucRadioVolume;		/* ÒôÁ¿ */
	uint8_t ucSpkOutEn;			/* ÑïÉùÆ÷Êä³öÊ¹ÄÜ */
}
PARAM_T;

extern PARAM_T g_tParam;

void LoadParam(void);
void SaveParam(void);

#endif /* INC_PARAM_H_ */
