/*
 * bsp_led.c
 *
 *  Created on: Mar 12, 2025
 *      Author: compro
 */
#include "bsp.h"

#ifdef STM32_X3		/* °²¸»À³ STM32-X4 ¿ª·¢°å */
	/*
		°²¸»À³STM32-X4 LED¿ÚÏß·ÖÅä£º
			LED1       : PE2 (µÍµçÆ½µãÁÁ£¬¸ßµçÆ½Ï¨Ãð)
			LED2       : PE3 (µÍµçÆ½µãÁÁ£¬¸ßµçÆ½Ï¨Ãð)
			LED3       : PE4 (µÍµçÆ½µãÁÁ£¬¸ßµçÆ½Ï¨Ãð)
			LED4       : PE5 (µÍµçÆ½µãÁÁ£¬¸ßµçÆ½Ï¨Ãð)
	*/
	#define RCC_ALL_LED 	RCC_AHB1Periph_GPIOE	/* °´¼ü¿Ú¶ÔÓ¦µÄRCCÊ±ÖÓ */

	#define GPIO_PORT_LED1  GPIOE
	#define GPIO_PIN_LED1	GPIO_Pin_2

	#define GPIO_PORT_LED2  GPIOE
	#define GPIO_PIN_LED2	GPIO_Pin_3

	#define GPIO_PORT_LED3  GPIOE
	#define GPIO_PIN_LED3	GPIO_Pin_4

	#define GPIO_PORT_LED4  GPIOE
	#define GPIO_PIN_LED4	GPIO_Pin_5

#else	/* STM32_F4 */
	/*
		°²¸»À³STM32-V5 ¿ª·¢°åLED¿ÚÏß·ÖÅä£º
			LD1     : PI10/TP_NCS          (µÍµçÆ½µãÁÁ£¬¸ßµçÆ½Ï¨Ãð)
			LD2     : PF7/NRF24L01_CSN     (µÍµçÆ½µãÁÁ£¬¸ßµçÆ½Ï¨Ãð)
			LD3     : PF8/SF_CS            (µÍµçÆ½µãÁÁ£¬¸ßµçÆ½Ï¨Ãð)
			LD4     : PC2/NRF905_CSN/VS1053_XCS  (µÍµçÆ½µãÁÁ£¬¸ßµçÆ½Ï¨Ãð)
	*/

	/* °´¼ü¿Ú¶ÔÓ¦µÄRCCÊ±ÖÓ */
	#define RCC_ALL_LED 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOI)

	#define GPIO_PORT_LED1  GPIOI
	#define GPIO_PIN_LED1	GPIO_PIN_10

	#define GPIO_PORT_LED2  GPIOF
	#define GPIO_PIN_LED2	GPIO_PIN_7

	#define GPIO_PORT_LED3  GPIOF
	#define GPIO_PIN_LED3	GPIO_PIN_8

	#define GPIO_PORT_LED4  GPIOC
	#define GPIO_PIN_LED4	GPIO_PIN_2
#endif

/*
*********************************************************************************************************
*	Func name: bsp_InitLed
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// #define RCC_ALL_LED 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOI)
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	bsp_LedOff(1);
	bsp_LedOff(2);
	bsp_LedOff(3);
	bsp_LedOff(4);

	GPIO_InitStructure.Pin = GPIO_PIN_LED1;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_LED2|GPIO_PIN_LED3;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_LED4;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	Func name: bsp_LedOn
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_LED1, GPIO_PIN_RESET);
	}
	else if (_no == 1)
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_LED2, GPIO_PIN_RESET);
	}
	else if (_no == 2)
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_LED3, GPIO_PIN_RESET);
	}
	else if (_no == 3)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_LED4, GPIO_PIN_RESET);
	}
}

/*
*********************************************************************************************************
*	Func name: bsp_LedOff
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_LED1, GPIO_PIN_SET);
	}
	else if (_no == 1)
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_LED2, GPIO_PIN_SET);
	}
	else if (_no == 2)
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_LED3, GPIO_PIN_SET);
	}
	else if (_no == 3)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_LED4, GPIO_PIN_SET);
	}
}

