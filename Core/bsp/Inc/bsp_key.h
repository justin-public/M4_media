/*
 * bsp_key.h
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_KEY_H_
#define BSP_INC_BSP_KEY_H_

#define KEY_COUNT    10	   					/* °´¼ü¸öÊý, 8¸ö¶ÀÁ¢½¨ + 2¸ö×éºÏ¼ü */

/* ¸ù¾ÝÓ¦ÓÃ³ÌÐòµÄ¹¦ÄÜÖØÃüÃû°´¼üºê */
#define KEY_DOWN_K1		KEY_1_DOWN
#define KEY_UP_K1		KEY_1_UP
#define KEY_LONG_K1		KEY_1_LONG

#define KEY_DOWN_K2		KEY_2_DOWN
#define KEY_UP_K2		KEY_2_UP
#define KEY_LONG_K2		KEY_2_LONG

#define KEY_DOWN_K3		KEY_3_DOWN
#define KEY_UP_K3		KEY_3_UP
#define KEY_LONG_K3		KEY_3_LONG

#define JOY_DOWN_U		KEY_4_DOWN		/* ÉÏ */
#define JOY_UP_U		KEY_4_UP
#define JOY_LONG_U		KEY_4_LONG

#define JOY_DOWN_D		KEY_5_DOWN		/* ÏÂ */
#define JOY_UP_D		KEY_5_UP
#define JOY_LONG_D		KEY_5_LONG

#define JOY_DOWN_L		KEY_6_DOWN		/* ×ó */
#define JOY_UP_L		KEY_6_UP
#define JOY_LONG_L		KEY_6_LONG

#define JOY_DOWN_R		KEY_7_DOWN		/* ÓÒ */
#define JOY_UP_R		KEY_7_UP
#define JOY_LONG_R		KEY_7_LONG

#define JOY_DOWN_OK		KEY_8_DOWN		/* ok */
#define JOY_UP_OK		KEY_8_UP
#define JOY_LONG_OK		KEY_8_LONG

#define SYS_DOWN_K1K2	KEY_9_DOWN		/* K1 K2 ×éºÏ¼ü */
#define SYS_UP_K1K2	    KEY_9_UP
#define SYS_LONG_K1K2	KEY_9_LONG

#define SYS_DOWN_K2K3	KEY_10_DOWN		/* K2 K3 ×éºÏ¼ü */
#define SYS_UP_K2K3  	KEY_10_UP
#define SYS_LONG_K2K3	KEY_10_LONG

/* °´¼üID, Ö÷ÒªÓÃÓÚbsp_KeyState()º¯ÊýµÄÈë¿Ú²ÎÊý */
typedef enum
{
	KID_K1 = 0,
	KID_K2,
	KID_K3,
	KID_JOY_U,
	KID_JOY_D,
	KID_JOY_L,
	KID_JOY_R,
	KID_JOY_OK
}KEY_ID_E;

/*
	°´¼üÂË²¨Ê±¼ä50ms, µ¥Î»10ms¡£
	Ö»ÓÐÁ¬Ðø¼ì²âµ½50ms×´Ì¬²»±ä²ÅÈÏÎªÓÐÐ§£¬°üÀ¨µ¯ÆðºÍ°´ÏÂÁ½ÖÖÊÂ¼þ
	¼´Ê¹°´¼üµçÂ·²»×öÓ²¼þÂË²¨£¬¸ÃÂË²¨»úÖÆÒ²¿ÉÒÔ±£Ö¤¿É¿¿µØ¼ì²âµ½°´¼üÊÂ¼þ
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* µ¥Î»10ms£¬ ³ÖÐø1Ãë£¬ÈÏÎª³¤°´ÊÂ¼þ */

/*
	Ã¿¸ö°´¼ü¶ÔÓ¦1¸öÈ«¾ÖµÄ½á¹¹Ìå±äÁ¿¡£
*/
typedef struct
{
	/* ÏÂÃæÊÇÒ»¸öº¯ÊýÖ¸Õë£¬Ö¸ÏòÅÐ¶Ï°´¼üÊÖ·ñ°´ÏÂµÄº¯Êý */
	uint8_t (*IsKeyDownFunc)(void); /* °´¼ü°´ÏÂµÄÅÐ¶Ïº¯Êý,1±íÊ¾°´ÏÂ */

	uint8_t  Count;			/* ÂË²¨Æ÷¼ÆÊýÆ÷ */
	uint16_t LongCount;		/* ³¤°´¼ÆÊýÆ÷ */
	uint16_t LongTime;		/* °´¼ü°´ÏÂ³ÖÐøÊ±¼ä, 0±íÊ¾²»¼ì²â³¤°´ */
	uint8_t  State;			/* °´¼üµ±Ç°×´Ì¬£¨°´ÏÂ»¹ÊÇµ¯Æð£© */
	uint8_t  RepeatSpeed;	/* Á¬Ðø°´¼üÖÜÆÚ */
	uint8_t  RepeatCount;	/* Á¬Ðø°´¼ü¼ÆÊýÆ÷ */
}KEY_T;

/*
	¶¨Òå¼üÖµ´úÂë, ±ØÐë°´ÈçÏÂ´ÎÐò¶¨Ê±Ã¿¸ö¼üµÄ°´ÏÂ¡¢µ¯ÆðºÍ³¤°´ÊÂ¼þ

	ÍÆ¼öÊ¹ÓÃenum, ²»ÓÃ#define£¬Ô­Òò£º
	(1) ±ãÓÚÐÂÔö¼üÖµ,·½±ãµ÷ÕûË³Ðò£¬Ê¹´úÂë¿´ÆðÀ´Êæ·þµã
	(2) ±àÒëÆ÷¿É°ïÎÒÃÇ±ÜÃâ¼üÖµÖØ¸´¡£
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ±íÊ¾°´¼üÊÂ¼þ */

	KEY_1_DOWN,				/* 1¼ü°´ÏÂ */
	KEY_1_UP,				/* 1¼üµ¯Æð */
	KEY_1_LONG,				/* 1¼ü³¤°´ */

	KEY_2_DOWN,				/* 2¼ü°´ÏÂ */
	KEY_2_UP,				/* 2¼üµ¯Æð */
	KEY_2_LONG,				/* 2¼ü³¤°´ */

	KEY_3_DOWN,				/* 3¼ü°´ÏÂ */
	KEY_3_UP,				/* 3¼üµ¯Æð */
	KEY_3_LONG,				/* 3¼ü³¤°´ */

	KEY_4_DOWN,				/* 4¼ü°´ÏÂ */
	KEY_4_UP,				/* 4¼üµ¯Æð */
	KEY_4_LONG,				/* 4¼ü³¤°´ */

	KEY_5_DOWN,				/* 5¼ü°´ÏÂ */
	KEY_5_UP,				/* 5¼üµ¯Æð */
	KEY_5_LONG,				/* 5¼ü³¤°´ */

	KEY_6_DOWN,				/* 6¼ü°´ÏÂ */
	KEY_6_UP,				/* 6¼üµ¯Æð */
	KEY_6_LONG,				/* 6¼ü³¤°´ */

	KEY_7_DOWN,				/* 7¼ü°´ÏÂ */
	KEY_7_UP,				/* 7¼üµ¯Æð */
	KEY_7_LONG,				/* 7¼ü³¤°´ */

	KEY_8_DOWN,				/* 8¼ü°´ÏÂ */
	KEY_8_UP,				/* 8¼üµ¯Æð */
	KEY_8_LONG,				/* 8¼ü³¤°´ */

	/* ×éºÏ¼ü */
	KEY_9_DOWN,				/* 9¼ü°´ÏÂ */
	KEY_9_UP,				/* 9¼üµ¯Æð */
	KEY_9_LONG,				/* 9¼ü³¤°´ */

	KEY_10_DOWN,			/* 10¼ü°´ÏÂ */
	KEY_10_UP,				/* 10¼üµ¯Æð */
	KEY_10_LONG,			/* 10¼ü³¤°´ */
}KEY_ENUM;

/* °´¼üFIFOÓÃµ½±äÁ¿ */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ¼üÖµ»º³åÇø */
	uint8_t Read;					/* »º³åÇø¶ÁÖ¸Õë1 */
	uint8_t Write;					/* »º³åÇøÐ´Ö¸Õë */
	uint8_t Read2;					/* »º³åÇø¶ÁÖ¸Õë2 */
}KEY_FIFO_T;

/* ¹©Íâ²¿µ÷ÓÃµÄº¯ÊýÉùÃ÷ */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);

#endif /* BSP_INC_BSP_KEY_H_ */
