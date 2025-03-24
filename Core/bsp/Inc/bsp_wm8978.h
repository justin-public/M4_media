/*
 * bsp_wm8978.h
 *
 *  Created on: Mar 24, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_WM8978_H_
#define BSP_INC_BSP_WM8978_H_

#define WM8978_SLAVE_ADDRESS    0x34	/* WM8978 I2C´Ó»úµØÖ· */

/* WM8978 ÒôÆµÊäÈëÍ¨µÀ¿ØÖÆÑ¡Ïî, ¿ÉÒÔÑ¡Ôñ¶àÂ·£¬±ÈÈç MIC_LEFT_ON | LINE_ON */
typedef enum
{
	IN_PATH_OFF		= 0x00,	/* ÎÞÊäÈë */
	MIC_LEFT_ON 	= 0x01,	/* LIN,LIP½Å£¬MIC×óÉùµÀ£¨½Ó°åÔØßäÍ·£© */
	MIC_RIGHT_ON	= 0x02,	/* RIN,RIP½Å£¬MICÓÒÉùµÀ£¨½ÓGPRSÄ£¿éÒôÆµÊä³ö£© */
	LINE_ON			= 0x04, /* L2,R2 Á¢ÌåÉùÊäÈë */
	AUX_ON			= 0x08,	/* AUXL,AUXR Á¢ÌåÉùÊäÈë */
	DAC_ON			= 0x10,	/* I2SÊý¾ÝDAC (CPU²úÉúÒôÆµÐÅºÅ) */
	ADC_ON			= 0x20	/* ÊäÈëµÄÒôÆµÀ¡ÈëWM8978ÄÚ²¿ADC £¨I2SÂ¼Òô) */
}IN_PATH_E;

/* WM8978 ÒôÆµÊä³öÍ¨µÀ¿ØÖÆÑ¡Ïî, ¿ÉÒÔÑ¡Ôñ¶àÂ· */
typedef enum
{
	OUT_PATH_OFF	= 0x00,	/* ÎÞÊä³ö */
	EAR_LEFT_ON 	= 0x01,	/* LOUT1 ¶ú»ú×óÉùµÀ */
	EAR_RIGHT_ON	= 0x02,	/* ROUT1 ¶ú»úÓÒÉùµÀ */
	SPK_ON			= 0x04,	/* LOUT2ºÍROUT2·´ÏàÊä³öµ¥ÉùµÀ,½ÓÑïÉùÆ÷ */
	OUT3_4_ON		= 0x08,	/* OUT3 ºÍ OUT4 Êä³öµ¥ÉùµÀÒôÆµ£¬ ½ÓGSMÄ£¿éµÄÒôÆµÊäÈë */
}OUT_PATH_E;

/* ¶¨Òå×î´óÒôÁ¿ */
#define VOLUME_MAX		63		/* ×î´óÒôÁ¿ */
#define VOLUME_STEP		1		/* ÒôÁ¿µ÷½Ú²½³¤ */

/* ¶¨Òå×î´óMICÔöÒæ */
#define GAIN_MAX		63		/* ×î´óÔöÒæ */
#define GAIN_STEP		1		/* ÔöÒæ²½³¤ */


/* ¹©Íâ²¿ÒýÓÃµÄº¯ÊýÉùÃ÷ */
uint8_t wm8978_Init(void);

void wm8978_CfgAudioIF(uint16_t _usStandard, uint8_t _ucWordLen);
void wm8978_OutMute(uint8_t _ucMute);
void wm8978_PowerDown(void);

void wm8978_CfgAudioPath(uint16_t _InPath, uint16_t _OutPath);

void wm8978_SetMicGain(uint8_t _ucGain);
void wm8978_SetLineGain(uint8_t _ucGain);
void wm8978_SetSpkVolume(uint8_t _ucVolume);
void wm8978_SetEarVolume(uint8_t _ucVolume);
uint8_t wm8978_ReadEarVolume(void);
uint8_t wm8978_ReadSpkVolume(void);

void wm8978_NotchFilter(uint16_t _NFA0, uint16_t _NFA1);

void I2S_CODEC_Init(void);
void I2S_StartPlay(uint16_t _usStandard, uint16_t _usWordLen,  uint32_t _uiAudioFreq);
void I2S_StartRecord(uint16_t _usStandard, uint16_t _usWordLen,  uint32_t _uiAudioFreq);
void I2S_Stop(void);

#endif /* BSP_INC_BSP_WM8978_H_ */
