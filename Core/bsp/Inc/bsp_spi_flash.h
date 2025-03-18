/*
 * bsp_spi_flash.h
 *
 *  Created on: Mar 18, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_SPI_FLASH_H_
#define BSP_INC_BSP_SPI_FLASH_H_

#define SF_MAX_PAGE_SIZE	(4 * 1024)

/* ¶¨Òå´®ÐÐFlash ID */
enum
{
	SST25VF016B_ID = 0xBF2541,
	MX25L1606E_ID  = 0xC22015,
	W25Q64BV_ID    = 0xEF4017
};

typedef struct
{
	uint32_t ChipID;		/* Ð¾Æ¬ID */
	char ChipName[16];		/* Ð¾Æ¬ÐÍºÅ×Ö·û´®£¬Ö÷ÒªÓÃÓÚÏÔÊ¾ */
	uint32_t TotalSize;		/* ×ÜÈÝÁ¿ */
	uint16_t PageSize;		/* Ò³Ãæ´óÐ¡ */
}SFLASH_T;

void bsp_InitSFlash(void);
uint32_t sf_ReadID(void);
void sf_EraseChip(void);
void sf_EraseSector(uint32_t _uiSectorAddr);
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize);
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize);
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);

extern SFLASH_T g_tSF;

#endif /* BSP_INC_BSP_SPI_FLASH_H_ */
