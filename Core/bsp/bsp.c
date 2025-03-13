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

	bsp_InitI2C();

	bsp_InitTimer();

	//bsp_DelayMS(20);
	HAL_Delay(20);
}

void bsp_Idle(void)
{

}
