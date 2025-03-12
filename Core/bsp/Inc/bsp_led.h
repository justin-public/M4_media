/*
 * bsp_led.h
 *
 *  Created on: Mar 12, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_LED_H_
#define BSP_INC_BSP_LED_H_

/* ¹©Íâ²¿µ÷ÓÃµÄº¯ÊýÉùÃ÷ */
void bsp_InitLed(void);
void bsp_LedOn(uint8_t _no);
void bsp_LedOff(uint8_t _no);
void bsp_LedToggle(uint8_t _no);
uint8_t bsp_IsLedOn(uint8_t _no);

#endif /* BSP_INC_BSP_LED_H_ */
