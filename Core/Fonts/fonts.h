/*										
*********************************************************************************************************
*	                                  
*	模块名称 : 字库模块
*	文件名称 : fonts.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __FONTS_H_
#define __FONTS_H_

extern unsigned char const g_Ascii16[];

extern unsigned char const g_Hz16[];

extern unsigned char const g_Ascii12[];

extern unsigned char const g_Hz12[];

//#define USE_SMALL_FONT			/* 定义此行表示使用小字库， 这个宏只在bsp_tft+lcd.c中使用 */

/* 0x64000000 是NOR Flash的首地址 */
#define HZK12_ADDR (0x64000000 + 0x0)		/* 12点阵汉字库地址 */
#define HZK16_ADDR (0x64000000 + 0x2C9D0)	/* 16点阵汉字库地址 */

#endif
