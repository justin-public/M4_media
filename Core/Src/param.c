/*
 * param.c
 *
 *  Created on: Mar 14, 2025
 *      Author: compro
 */
#include "bsp.h"
#include "param.h"

PARAM_T g_tParam;

void LoadParam(void)
{
#ifdef PARAM_SAVE_TO_FLASH
	/* ¶ÁÈ¡CPU FlashÖÐµÄ²ÎÊý */
	bsp_ReadCpuFlash(PARAM_ADDR, (uint8_t *)&g_tParam, sizeof(PARAM_T));
#endif

#ifdef PARAM_SAVE_TO_EEPROM
	/* ¶ÁÈ¡EEPROMÖÐµÄ²ÎÊý */
	ee_ReadBytes((uint8_t *)&g_tParam, PARAM_ADDR, sizeof(PARAM_T));
#endif

	/* Ìî³äÈ±Ê¡²ÎÊý */
	if (g_tParam.ParamVer != PARAM_VER)
	{
		g_tParam.ParamVer = PARAM_VER;

		g_tParam.ucBackLight = 200;

		/* uip  ±¾»úIPµØÖ· */
		g_tParam.uip_ip[0] = 192;
		g_tParam.uip_ip[1] = 168;
		g_tParam.uip_ip[2] = 1;
		g_tParam.uip_ip[3] = 11;

		/* uip  ×ÓÍøÑÚÂë */
		g_tParam.uip_net_mask[0] = 255;
		g_tParam.uip_net_mask[1] = 255;
		g_tParam.uip_net_mask[2] = 255;
		g_tParam.uip_net_mask[3] = 0;

		/* uip  Ä¬ÈÏÍø¹Ø */
		g_tParam.uip_net_gate[0] = 192;
		g_tParam.uip_net_gate[1] = 168;
		g_tParam.uip_net_gate[2] = 1;
		g_tParam.uip_net_gate[3] = 1;

		/* lwip  ±¾»úIPµØÖ· */
		g_tParam.lwip_ip[0] = 192;
		g_tParam.lwip_ip[1] = 168;
		g_tParam.lwip_ip[2] = 1;
		g_tParam.lwip_ip[3] = 12;

		/* lwip  ×ÓÍøÑÚÂë */
		g_tParam.lwip_net_mask[0] = 255;
		g_tParam.lwip_net_mask[1] = 255;
		g_tParam.lwip_net_mask[2] = 255;
		g_tParam.lwip_net_mask[3] = 0;

		/* lwip  Ä¬ÈÏÍø¹Ø */
		g_tParam.lwip_net_gate[0] = 192;
		g_tParam.lwip_net_gate[1] = 168;
		g_tParam.lwip_net_gate[2] = 1;
		g_tParam.lwip_net_gate[3] = 1;

		g_tParam.ucRadioMode     = 0;				/* 1 = AM »ò 0 = FM */
		g_tParam.ucRadioListType = 0;				/* µçÌ¨ÁÐ±íÀàÐÍ¡£0ÎäººµØÇø»ò1È«¹ú */
		g_tParam.ucIndexFM       = 0;				/* µ±Ç°FMµçÌ¨Ë÷Òý */
		g_tParam.ucIndexAM       = 0;				/* µ±Ç°µçÌ¨Ë÷Òý */
		g_tParam.ucRadioVolume   = 36;				/* ÒôÁ¿ */
		g_tParam.ucSpkOutEn      = 1;				/* ÑïÉùÆ÷Êä³öÊ¹ÄÜ */

		SaveParam();							/* ½«ÐÂ²ÎÊýÐ´ÈëFlash */
	}
}

void SaveParam(void)
{
#ifdef PARAM_SAVE_TO_FLASH
	bsp_WriteCpuFlash(PARAM_ADDR, (unsigned char *)&g_tParam, sizeof(PARAM_T));
#endif

#ifdef PARAM_SAVE_TO_EEPROM
	ee_WriteBytes((uint8_t *)&g_tParam, PARAM_ADDR, sizeof(PARAM_T));
#endif
}

