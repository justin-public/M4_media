/*
 * bsp_sdio_sd.c
 *
 *  Created on: Mar 15, 2025
 *      Author: compro
 */
#include "bsp_sdio_sd.h"

SD_Error SD_Init(void)
{
	__IO SD_Error errorstatus = SD_OK;

	SDIO_Interrupts_Config();
}

/*
*********************************************************************************************************
*	Func name: SDIO_Interrupts_Config
*********************************************************************************************************
*/
void SDIO_Interrupts_Config(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);

	HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SDIO_IRQn);

	/* Enable the SDIO DMA interrupt */
	HAL_NVIC_SetPriority(SD_SDIO_DMA_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(SD_SDIO_DMA_IRQn);
}
