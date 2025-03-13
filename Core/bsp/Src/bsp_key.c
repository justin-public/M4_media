/*
 * bsp_key.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "bsp.h"

/*
	¸Ã³ÌÐòÊÊÓÃÓÚ°²¸»À³STM32-X4¡¢STM32-V5¿ª·¢°å

	Èç¹ûÓÃÓÚÆäËüÓ²¼þ£¬ÇëÐÞ¸ÄGPIO¶¨ÒåºÍ IsKeyDown1 - IsKeyDown8 º¯Êý

	Èç¹ûÓÃ»§µÄ°´¼ü¸öÊýÐ¡ÓÚ8¸ö£¬Äã¿ÉÒÔ½«¶àÓàµÄ°´¼üÈ«²¿¶¨ÒåÎªºÍµÚ1¸ö°´¼üÒ»Ñù£¬²¢²»Ó°Ïì³ÌÐò¹¦ÄÜ
	#define KEY_COUNT    8	  Õâ¸öÔÚ bsp_key.h ÎÄ¼þÖÐ¶¨Òå
*/

#ifdef STM32_X3		/* °²¸»À³ STM32-X4 ¿ª·¢°å */
	/*
		°²¸»À³STM32-X °´¼ü¿ÚÏß·ÖÅä£º
			K1¼ü       : PC13 (µÍµçÆ½±íÊ¾°´ÏÂ)
			K2¼ü       : PC0  (µÍµçÆ½±íÊ¾°´ÏÂ)
			K3¼ü       : PC1  (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËUP¼ü   : PC2  (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËDOWN¼ü : PC3  (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËLEFT¼ü : PC4  (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËRIGHT¼ü: PA0  (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËOK¼ü   : PA1  (µÍµçÆ½±íÊ¾°´ÏÂ)
	*/
	#define RCC_ALL_KEY 	(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC)	/* °´¼ü¿Ú¶ÔÓ¦µÄRCCÊ±ÖÓ */

	#define GPIO_PORT_K1    GPIOC
	#define GPIO_PIN_K1	    GPIO_Pin_13

	#define GPIO_PORT_K2    GPIOC
	#define GPIO_PIN_K2	    GPIO_Pin_0

	#define GPIO_PORT_K3    GPIOC
	#define GPIO_PIN_K3	    GPIO_Pin_1

	#define GPIO_PORT_K4    GPIOC
	#define GPIO_PIN_K4	    GPIO_Pin_3

	#define GPIO_PORT_K5    GPIOC
	#define GPIO_PIN_K5	    GPIO_Pin_4

	#define GPIO_PORT_K6    GPIOC
	#define GPIO_PIN_K6	    GPIO_Pin_2

	#define GPIO_PORT_K7    GPIOA
	#define GPIO_PIN_K7	    GPIO_Pin_1

	#define GPIO_PORT_K8    GPIOA
	#define GPIO_PIN_K8	    GPIO_Pin_0

#else	/* STM32_V5 */
	/*
		°²¸»À³STM32-V5 °´¼ü¿ÚÏß·ÖÅä£º
			K1 ¼ü      : PI8   (µÍµçÆ½±íÊ¾°´ÏÂ)
			K2 ¼ü      : PC13  (µÍµçÆ½±íÊ¾°´ÏÂ)
			K3 ¼ü      : PI11  (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËUP¼ü   : PH2   (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËDOWN¼ü : PH3   (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËLEFT¼ü : PF11  (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËRIGHT¼ü: PG7   (µÍµçÆ½±íÊ¾°´ÏÂ)
			Ò¡¸ËOK¼ü   : PH15  (µÍµçÆ½±íÊ¾°´ÏÂ)
	*/

	/* °´¼ü¿Ú¶ÔÓ¦µÄRCCÊ±ÖÓ */
	#define RCC_ALL_KEY 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG)

	#define GPIO_PORT_K1    GPIOI
	#define GPIO_PIN_K1	    GPIO_PIN_8

	#define GPIO_PORT_K2    GPIOC
	#define GPIO_PIN_K2	    GPIO_PIN_13

	#define GPIO_PORT_K3    GPIOI
	#define GPIO_PIN_K3	    GPIO_PIN_11

	#define GPIO_PORT_K4    GPIOH
	#define GPIO_PIN_K4	    GPIO_PIN_2

	#define GPIO_PORT_K5    GPIOH
	#define GPIO_PIN_K5	    GPIO_PIN_3

	#define GPIO_PORT_K6    GPIOF
	#define GPIO_PIN_K6	    GPIO_PIN_11

	#define GPIO_PORT_K7    GPIOG
	#define GPIO_PIN_K7	    GPIO_PIN_7

	#define GPIO_PORT_K8    GPIOH
	#define GPIO_PIN_K8	    GPIO_PIN_15
#endif

static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		/* °´¼üFIFO±äÁ¿,½á¹¹Ìå */

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

/*
*********************************************************************************************************
*	º¯ Êý Ãû: IsKeyDownX
*	¹¦ÄÜËµÃ÷: ÅÐ¶Ï°´¼üÊÇ·ñ°´ÏÂ
*	ÐÎ    ²Î: ÎÞ
*	·µ »Ø Öµ: ·µ»ØÖµ1 ±íÊ¾°´ÏÂ£¬0±íÊ¾Î´°´ÏÂ
*********************************************************************************************************
*/
#ifdef STM32_X3		/* °²¸»À³ STM32-X3 ¿ª·¢°å */
	static uint8_t IsKeyDown1(void) {if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown2(void) {if ((GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown3(void) {if ((GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown4(void) {if ((GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown5(void) {if ((GPIO_PORT_K5->IDR & GPIO_PIN_K5) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown6(void) {if ((GPIO_PORT_K6->IDR & GPIO_PIN_K6) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown7(void) {if ((GPIO_PORT_K7->IDR & GPIO_PIN_K7) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown8(void) {if ((GPIO_PORT_K8->IDR & GPIO_PIN_K8) == 0) return 1;else return 0;}
#else				/* °²¸»À³ STM32-V5 ¿ª·¢°å */
	static uint8_t IsKeyDown1(void) {if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown2(void) {if ((GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown3(void) {if ((GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown4(void) {if ((GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown5(void) {if ((GPIO_PORT_K5->IDR & GPIO_PIN_K5) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown6(void) {if ((GPIO_PORT_K6->IDR & GPIO_PIN_K6) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown7(void) {if ((GPIO_PORT_K7->IDR & GPIO_PIN_K7) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown8(void) {if ((GPIO_PORT_K8->IDR & GPIO_PIN_K8) == 0) return 1;else return 0;}
#endif
	static uint8_t IsKeyDown9(void) {if (IsKeyDown1() && IsKeyDown2()) return 1;else return 0;}
	static uint8_t IsKeyDown10(void) {if (IsKeyDown1() && IsKeyDown2()) return 1;else return 0;}



/*
*********************************************************************************************************
* Func name: bsp_InitKey
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();
	bsp_InitKeyHard();
}

/*
*********************************************************************************************************
*	Func name: bsp_InitKeyVar
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* ¶Ô°´¼üFIFO¶ÁÐ´Ö¸ÕëÇåÁã */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* ¸øÃ¿¸ö°´¼ü½á¹¹Ìå³ÉÔ±±äÁ¿¸³Ò»×éÈ±Ê¡Öµ */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* ³¤°´Ê±¼ä 0 ±íÊ¾²»¼ì²â³¤°´¼üÊÂ¼þ */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* ¼ÆÊýÆ÷ÉèÖÃÎªÂË²¨Ê±¼äµÄÒ»°ë */
		s_tBtn[i].State = 0;							/* °´¼üÈ±Ê¡×´Ì¬£¬0ÎªÎ´°´ÏÂ */
		//s_tBtn[i].KeyCodeDown = 3 * i + 1;				/* °´¼ü°´ÏÂµÄ¼üÖµ´úÂë */
		//s_tBtn[i].KeyCodeUp   = 3 * i + 2;				/* °´¼üµ¯ÆðµÄ¼üÖµ´úÂë */
		//s_tBtn[i].KeyCodeLong = 3 * i + 3;				/* °´¼ü±»³ÖÐø°´ÏÂµÄ¼üÖµ´úÂë */
		s_tBtn[i].RepeatSpeed = 0;						/* °´¼üÁ¬·¢µÄËÙ¶È£¬0±íÊ¾²»Ö§³ÖÁ¬·¢ */
		s_tBtn[i].RepeatCount = 0;						/* Á¬·¢¼ÆÊýÆ÷ */
	}
	/* Èç¹ûÐèÒªµ¥¶À¸ü¸ÄÄ³¸ö°´¼üµÄ²ÎÊý£¬¿ÉÒÔÔÚ´Ëµ¥¶ÀÖØÐÂ¸³Öµ */
	/* ±ÈÈç£¬ÎÒÃÇÏ£Íû°´¼ü1°´ÏÂ³¬¹ý1Ãëºó£¬×Ô¶¯ÖØ·¢ÏàÍ¬¼üÖµ */
	s_tBtn[KID_JOY_U].LongTime = 100;
	s_tBtn[KID_JOY_U].RepeatSpeed = 5;	/* Ã¿¸ô50ms×Ô¶¯·¢ËÍ¼üÖµ */

	s_tBtn[KID_JOY_D].LongTime = 100;
	s_tBtn[KID_JOY_D].RepeatSpeed = 5;	/* Ã¿¸ô50ms×Ô¶¯·¢ËÍ¼üÖµ */

	s_tBtn[KID_JOY_L].LongTime = 100;
	s_tBtn[KID_JOY_L].RepeatSpeed = 5;	/* Ã¿¸ô50ms×Ô¶¯·¢ËÍ¼üÖµ */

	s_tBtn[KID_JOY_R].LongTime = 100;
	s_tBtn[KID_JOY_R].RepeatSpeed = 5;	/* Ã¿¸ô50ms×Ô¶¯·¢ËÍ¼üÖµ */

	/* ÅÐ¶Ï°´¼ü°´ÏÂµÄº¯Êý */
	s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc = IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc = IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc = IsKeyDown4;
	s_tBtn[4].IsKeyDownFunc = IsKeyDown5;
	s_tBtn[5].IsKeyDownFunc = IsKeyDown6;
	s_tBtn[6].IsKeyDownFunc = IsKeyDown7;
	s_tBtn[7].IsKeyDownFunc = IsKeyDown8;

	/* ×éºÏ¼ü */
	s_tBtn[8].IsKeyDownFunc = IsKeyDown9;
	s_tBtn[9].IsKeyDownFunc = IsKeyDown10;
}

/*
*********************************************************************************************************
* Func name: bsp_InitKeyHard
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// #define RCC_ALL_KEY 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG)
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();


	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pin = GPIO_PIN_K1;
	HAL_GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K2;
	HAL_GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K3;
	HAL_GPIO_Init(GPIO_PORT_K3, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K4;
	HAL_GPIO_Init(GPIO_PORT_K4, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K5;
	HAL_GPIO_Init(GPIO_PORT_K5, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K6;
	HAL_GPIO_Init(GPIO_PORT_K6, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K7;
	HAL_GPIO_Init(GPIO_PORT_K7, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K8;
	HAL_GPIO_Init(GPIO_PORT_K8, &GPIO_InitStructure);
}






































