/*
 * bsp_ra8875_port.h
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_RA8875_PORT_H_
#define BSP_INC_BSP_RA8875_PORT_H_

//#define RA_SOFT_SPI	   	/* Èí¼þSPI½Ó¿ÚÄ£Ê½ */
//#define RA_HARD_SPI	   	/* Ó²¼þSPI½Ó¿ÚÄ£Ê½ */
//#define RA_SOFT_8080_8	/* Èí¼þÄ£Äâ8080½Ó¿Ú,8bit */
#define RA_HARD_8080_16	/* Ó²¼þ8080½Ó¿Ú,16bit */

void RA8875_Delaly1us(void);
void RA8875_Delaly1ms(void);
uint16_t RA8875_ReadID(void);
void RA8875_WriteCmd(uint8_t _ucRegAddr);
void RA8875_WriteData(uint8_t _ucRegValue);
uint8_t RA8875_ReadData(void);
void RA8875_WriteData16(uint16_t _usRGB);
uint16_t RA8875_ReadData16(void);
uint8_t RA8875_ReadStatus(void);
uint32_t RA8875_GetDispMemAddr(void);

#ifdef RA_HARD_SPI	   /* ËÄÏßSPI½Ó¿ÚÄ£Ê½ */
	void RA8875_InitSPI(void);
	void RA8875_HighSpeedSPI(void);
	void RA8875_LowSpeedSPI(void);
#endif

#endif /* BSP_INC_BSP_RA8875_PORT_H_ */
