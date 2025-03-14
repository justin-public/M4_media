/*
 * bsp_eeprom_24xx.h
 *
 *  Created on: Mar 14, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_EEPROM_24XX_H_
#define BSP_INC_BSP_EEPROM_24XX_H_

//#define AT24C02
#define AT24C128

#ifdef AT24C02
	#define EE_MODEL_NAME		"AT24C02"
	#define EE_DEV_ADDR			0xA0		/* Éè±¸µØÖ· */
	#define EE_PAGE_SIZE		8			/* Ò³Ãæ´óÐ¡(×Ö½Ú) */
	#define EE_SIZE				256			/* ×ÜÈÝÁ¿(×Ö½Ú) */
	#define EE_ADDR_BYTES		1			/* µØÖ·×Ö½Ú¸öÊý */
#endif

#ifdef AT24C128
	#define EE_MODEL_NAME		"AT24C128"
	#define EE_DEV_ADDR			0xA0		/* Éè±¸µØÖ· */
	#define EE_PAGE_SIZE		64			/* Ò³Ãæ´óÐ¡(×Ö½Ú) */
	#define EE_SIZE				(16*1024)	/* ×ÜÈÝÁ¿(×Ö½Ú) */
	#define EE_ADDR_BYTES		2			/* µØÖ·×Ö½Ú¸öÊý */
#endif

uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);

#endif /* BSP_INC_BSP_EEPROM_24XX_H_ */
