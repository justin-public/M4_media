/*
 * LCD_SPFD5420.h
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */

#ifndef BSP_INC_LCD_SPFD5420_H_
#define BSP_INC_LCD_SPFD5420_H_

#include "stm32f4xx.h"

uint16_t SPFD5420_ReadID(void);
void SPFD5420_InitHard(void);
void SPFD5420_DispOn(void);
void SPFD5420_DispOff(void);
void SPFD5420_ClrScr(uint16_t _usColor);
void SPFD5420_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t SPFD5420_GetPixel(uint16_t _usX, uint16_t _usY);
void SPFD5420_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void SPFD5420_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void SPFD5420_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor);
void SPFD5420_DrawHTransLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor);
void SPFD5420_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void SPFD5420_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void SPFD5420_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);

void SPFD5420_SetBackLight(uint8_t _bright);

#endif /* BSP_INC_LCD_SPFD5420_H_ */
