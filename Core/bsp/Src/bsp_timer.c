/*
 * bsp_timer.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "bsp.h"

/* Õâ2¸öÈ«¾Ö±äÁ¿×ªÓÃÓÚ bsp_DelayMS() º¯Êý */
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* ¶¨ÓÚÈí¼þ¶¨Ê±Æ÷½á¹¹Ìå±äÁ¿ */
static SOFT_TMR s_tTmr[TMR_COUNT];

/*
	È«¾ÖÔËÐÐÊ±¼ä£¬µ¥Î»1ms
	×î³¤¿ÉÒÔ±íÊ¾ 24.85Ìì£¬Èç¹ûÄãµÄ²úÆ·Á¬ÐøÔËÐÐÊ±¼ä³¬¹ýÕâ¸öÊý£¬Ôò±ØÐë¿¼ÂÇÒç³öÎÊÌâ
*/
__IO int32_t g_iRunTime = 0;

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);

/*
*********************************************************************************************************
*	Func name: bsp_InitTimer
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;
	}
	SysTick_Config(SystemCoreClock / 1000);
}

/*
*********************************************************************************************************
*	Func name: bsp_DelayMS
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	if (n == 0)
	{
		//return;
	}
	else if (n == 1)
	{
		n = 2;
	}

	DISABLE_INT();

	s_uiDelayCount = n;
	s_ucTimeOutFlag = 0;

	ENABLE_INT();

	while (1)
	{
		bsp_Idle();

		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}


