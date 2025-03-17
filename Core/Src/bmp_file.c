/*
 * bmp_file.c
 *
 *  Created on: Mar 14, 2025
 *      Author: compro
 */
#include "ff.h"
#include "bmp_file.h"

__packed typedef struct
{
	/* 14×Ö½Ú Î»Í¼ÎÄ¼þÍ· */
	uint16_t bfType;		/* 2×Ö½Ú Î»Í¼Àà±ð£¬ ÔÚWindowsÖÐ£¬´Ë×Ö¶ÎµÄÖµ×ÜÎª'BM'*/
	uint32_t bfSize;		/* bmpÎÄ¼þ´óÐ¡ 4×Ö½Ú */
	uint16_t bfReserved1;	/* ±£Áô£¬Ã¿×Ö½ÚÒÔ"00"ÌîÐ´ 2×Ö½Ú */
	uint16_t bfReserved2; 	/* Í¬ÉÏ 2×Ö½Ú */
	uint32_t bfOffBits;		/* ¼ÇÂ¼Í¼ÏñÊý¾ÝÇøµÄÆðÊ¼Î»ÖÃ(Í¼ÏóÊý¾ÝÏà¶ÔÓÚÎÄ¼þÍ·×Ö½ÚµÄÆ«ÒÆÁ¿)¡£ 4×Ö½Ú */

	/* 40×Ö½Ú Î»Í¼ÐÅÏ¢Í· */
	uint32_t biSize;		/* 4×Ö½Ú ±¾½á¹¹µÄ´óÐ¡£¬ÔÚWindowsÖÐ£¬×ÜÎª28h£¬40×Ö½Ú */
	int32_t  biWidth;		/* 4×Ö½Ú BMPÍ¼ÏñµÄ¿í¶È£¬µ¥Î»ÏñËØ */
	int32_t  biHeight;		/* 4×Ö½Ú BMPÍ¼ÏñµÄ¸ß¶È£¬µ¥Î»ÏñËØ */
	uint16_t biPlanes;		/* 2×Ö½Ú Ä¿±êÉè±¸µÄ¼¶±ð(É«²ÊÆ½ÃæÊý)£¬¹Ì¶¨Îª1 */
	uint16_t biBitCount;	/* 2×Ö½Ú BMPÍ¼ÏñµÄÉ«Éî£¬¼´Ò»¸öÏñËØÓÃ¶àÉÙÎ»±íÊ¾¡£³£¼ûµÄÓÐ 1 4 8 16 24 32 */
	uint32_t biCompression;	/* 4×Ö½Ú Ñ¹ËõÀàÐÍ£¬0(²»Ñ¹Ëõ), 1(BI_RLE8), 2(BI_RLE4) */
	uint32_t biSizeImage;	/* 4×Ö½Ú ±íÊ¾Î»Í¼Êý¾ÝÇøÓòµÄ´óÐ¡ÒÔ×Ö½ÚÎªµ¥Î» */
	int32_t  biXPelsPerMeter;	/* 4×Ö½Ú ÓÃÏóËØ/Ã×±íÊ¾µÄË®Æ½·Ö±æÂÊ */
	int32_t  biYPelsPerMeter;	/* 4×Ö½Ú ÓÃÏóËØ/Ã×±íÊ¾µÄ´¹Ö±·Ö±æÂÊ */
	uint32_t biClrUsed;			/* 4×Ö½Ú Êµ¼ÊÊ¹ÓÃÉ«²ÊÊýÄ¿£¬ÔòÓÉÎ»Êý¶¨ */
	uint32_t biClrImportant;	/* 4×Ö½Ú Í¼ÏñÖÐÖØÒªµÄÉ«²ÊÊýÄ¿¡£0±íÊ¾µ÷É«°åÄÚËùÓÐµÄÑÕÉ«¶¼ÊÇÖØÒªµÄ  */
}BMP_T;

/*
*********************************************************************************************************
*	Func name: MakeBmpHeader
*********************************************************************************************************
*/
static void MakeBmpHeader(__packed BMP_T *_pBmp, uint16_t _Width, uint16_t _Height)
{
	/* 14×Ö½Ú Î»Í¼ÎÄ¼þÍ· */
	_pBmp->bfType = 0x4D42;	/* 2×Ö½Ú Î»Í¼Àà±ð£¬ ÔÚWindowsÖÐ£¬´Ë×Ö¶ÎµÄÖµ×ÜÎª'BM'*/
	_pBmp->bfSize = (_Height * _Width * 3) + 54;	/* bmpÎÄ¼þ´óÐ¡ 4×Ö½Ú */
	_pBmp->bfReserved1 = 0;	/* ±£Áô£¬Ã¿×Ö½ÚÒÔ"00"ÌîÐ´ 2×Ö½Ú */
	_pBmp->bfReserved2 = 0; /* Í¬ÉÏ 2×Ö½Ú */
	_pBmp->bfOffBits = 54;	/* ¼ÇÂ¼Í¼ÏñÊý¾ÝÇøµÄÆðÊ¼Î»ÖÃ(Í¼ÏóÊý¾ÝÏà¶ÔÓÚÎÄ¼þÍ·×Ö½ÚµÄÆ«ÒÆÁ¿)¡£ 4×Ö½Ú */

	/* 40×Ö½Ú Î»Í¼ÐÅÏ¢Í· */
	_pBmp->biSize = 40;		/* 4×Ö½Ú ±¾½á¹¹µÄ´óÐ¡£¬ÔÚWindowsÖÐ£¬×ÜÎª28h£¬40×Ö½Ú */
	_pBmp->biWidth = _Width;/* 4×Ö½Ú BMPÍ¼ÏñµÄ¿í¶È£¬µ¥Î»ÏñËØ */
	_pBmp->biHeight = _Height;		/* 4×Ö½Ú BMPÍ¼ÏñµÄ¸ß¶È£¬µ¥Î»ÏñËØ */
	_pBmp->biPlanes = 1;			/* 2×Ö½Ú Ä¿±êÉè±¸µÄ¼¶±ð(É«²ÊÆ½ÃæÊý)£¬¹Ì¶¨Îª1 */
	_pBmp->biBitCount = 24;			/* 2×Ö½Ú BMPÍ¼ÏñµÄÉ«Éî£¬¼´Ò»¸öÏñËØÓÃ¶àÉÙÎ»±íÊ¾¡£³£¼ûµÄÓÐ 1 4 8 16 24 32 */
	_pBmp->biCompression = 0;		/* 4×Ö½Ú Ñ¹ËõÀàÐÍ£¬0(²»Ñ¹Ëõ), 1(BI_RLE8), 2(BI_RLE4) */
	_pBmp->biSizeImage = (_Height * _Width * 3);	/* 4×Ö½Ú ±íÊ¾Î»Í¼Êý¾ÝÇøÓòµÄ´óÐ¡ÒÔ×Ö½ÚÎªµ¥Î» */
	_pBmp->biXPelsPerMeter = 0;	/* 4×Ö½Ú ÓÃÏóËØ/Ã×±íÊ¾µÄË®Æ½·Ö±æÂÊ */
	_pBmp->biYPelsPerMeter = 0;	/* 4×Ö½Ú ÓÃÏóËØ/Ã×±íÊ¾µÄ´¹Ö±·Ö±æÂÊ */
	_pBmp->biClrUsed = 0;		/* 4×Ö½Ú Êµ¼ÊÊ¹ÓÃÉ«²ÊÊýÄ¿£¬0ÔòÓÉÎ»Êý¶¨ */
	_pBmp->biClrImportant = 0;	/* 4×Ö½Ú Í¼ÏñÖÐÖØÒªµÄÉ«²ÊÊýÄ¿¡£0±íÊ¾µ÷É«°åÄÚËùÓÐµÄÑÕÉ«¶¼ÊÇÖØÒªµÄ  */
}


/*
*********************************************************************************************************
*	Funa name: SaveScreenToBmp
*********************************************************************************************************
*/
void SaveScreenToBmp(uint16_t _index)
{
	__packed BMP_T tBmpHead;
	uint8_t LineData[800 * 3]; 	/* °´×î´ó 800ÏñËØ¿í¶È·ÖÅä»º³åÇø */
	uint16_t i,j;
	uint16_t pixel;

	/* ±¾º¯ÊýÊ¹ÓÃµÄ¾Ö²¿±äÁ¿Õ¼ÓÃ½Ï¶à£¬ÇëÐÞ¸ÄÆô¶¯ÎÄ¼þ£¬±£Ö¤¶ÑÕ»¿Õ¼ä¹»ÓÃ */
	FRESULT result;
	FATFS fs;
	FIL file;
	uint32_t bw;
	char path[32];
	uint8_t ucVolume = FS_SD;

	bsp_LedOn(1);

	MakeBmpHeader(&tBmpHead, LCD_GetWidth(), LCD_GetHeight());	/* ¹¹½¨BMPÎÄ¼þÍ· */
}
