/*
 * bsp_timer.h
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_TIMER_H_
#define BSP_INC_BSP_TIMER_H_

/*
	ÔÚ´Ë¶¨ÒåÈô¸É¸öÈí¼þ¶¨Ê±Æ÷È«¾Ö±äÁ¿
	×¢Òâ£¬±ØÐëÔö¼Ó__IO ¼´ volatile£¬ÒòÎªÕâ¸ö±äÁ¿ÔÚÖÐ¶ÏºÍÖ÷³ÌÐòÖÐÍ¬Ê±±»·ÃÎÊ£¬ÓÐ¿ÉÄÜÔì³É±àÒëÆ÷´íÎóÓÅ»¯¡£
*/
#define TMR_COUNT	4		/* Èí¼þ¶¨Ê±Æ÷µÄ¸öÊý £¨¶¨Ê±Æ÷ID·¶Î§ 0 - 3) */

/* ¶¨Ê±Æ÷½á¹¹Ìå£¬³ÉÔ±±äÁ¿±ØÐëÊÇ volatile, ·ñÔòC±àÒëÆ÷ÓÅ»¯Ê±¿ÉÄÜÓÐÎÊÌâ */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* Ò»´Î¹¤×÷Ä£Ê½ */
	TMR_AUTO_MODE = 1		/* ×Ô¶¯¶¨Ê±¹¤×÷Ä£Ê½ */
}TMR_MODE_E;

/* ¶¨Ê±Æ÷½á¹¹Ìå£¬³ÉÔ±±äÁ¿±ØÐëÊÇ volatile, ·ñÔòC±àÒëÆ÷ÓÅ»¯Ê±¿ÉÄÜÓÐÎÊÌâ */
typedef struct
{
	volatile uint8_t Mode;		/* ¼ÆÊýÆ÷Ä£Ê½£¬1´ÎÐÔ */
	volatile uint8_t Flag;		/* ¶¨Ê±µ½´ï±êÖ¾  */
	volatile uint32_t Count;	/* ¼ÆÊýÆ÷ */
	volatile uint32_t PreLoad;	/* ¼ÆÊýÆ÷Ô¤×°Öµ */
}SOFT_TMR;

/* Ìá¹©¸øÆäËûCÎÄ¼þµ÷ÓÃµÄº¯Êý */
void bsp_InitTimer(void);
void bsp_DelayMS(uint32_t n);
void bsp_DelayUS(uint32_t n);
void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);
uint8_t bsp_CheckTimer(uint8_t _id);
int32_t bsp_GetRunTime(void);

#endif /* BSP_INC_BSP_TIMER_H_ */
