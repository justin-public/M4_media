/*
 * bsp.c
 *
 *  Created on: Mar 12, 2025
 *      Author: compro
 */
#include "bsp.h"

void bsp_Init(void)
{
	bsp_InitUart();
	bsp_InitLed();
	bsp_InitKey();

	bsp_InitExtSRAM();

	bsp_InitI2C();
	bsp_InitTimer();

	//bsp_DelayMS(20);
	HAL_Delay(20);
}

extern void SaveScreenToBmp(uint16_t _index);
void bsp_Idle(void)
{
	if (bsp_GetKey2() == SYS_DOWN_K1K2)
	{
		static uint16_t s_file_no = 0;
		SaveScreenToBmp(s_file_no++);
	}
}
