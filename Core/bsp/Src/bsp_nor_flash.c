/*
 * bsp_nor_flash.c
 *
 *  Created on: Mar 18, 2025
 *      Author: compro
 */
#include "bsp.h"

#define ADDR_SHIFT(A) 	(NOR_FLASH_ADDR + (2 * (A)))
#define NOR_WRITE(Address, Data)  (*(__IO uint16_t *)(Address) = (Data))

/* ÅÐÃ¦Ê±µÄÖ´ÐÐÓï¾äÑ­»·´ÎÊý */
#define BlockErase_Timeout    ((uint32_t)0x00A00000)
#define ChipErase_Timeout     ((uint32_t)0x30000000)
#define Program_Timeout       ((uint32_t)0x00001400)

/* PD6 ÊÇNOR FlashÊä³öµ½STM32µÄÃ¦ÐÅºÅ, Í¨¹ýGPIO²éÑ¯·½Ê½ÅÐÃ¦ */
#define NOR_IS_BUSY()	(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == RESET)

static void NOR_QuitToReadStatus(void);
static uint8_t NOR_GetStatus(uint32_t Timeout);


/*
*********************************************************************************************************
*	Func name: bsp_InitNorFlash
*********************************************************************************************************
*/
void bsp_InitNorFlash(void)
{
	SRAM_HandleTypeDef hnorsram;
	FSMC_NORSRAM_TimingTypeDef  timingWrite;
	FSMC_NORSRAM_TimingTypeDef  timingRead;
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	__HAL_RCC_FSMC_CLK_ENABLE();

	/* NOR Flash µÄ GPIO £º
			PD0/FSMC_D2
			PD1/FSMC_D3
			PD4/FSMC_NOE
			PD5/FSMC_NWE

			PD6/FSMC_NWAIT		- Ã¦ÐÅºÅ£¬ÅäÖÃÎªGPIO£¬ÊäÈëÄ£Ê½£¬Í¨¹ýÈí¼þ²éÑ¯·½Ê½ÅÐÃ¦

			PD8/FSMC_D13
			PD9/FSMC_D14
			PD10/FSMC_D15
			PD11/FSMC_CLE/FSMC_A16
			PD12/FSMC_ALE/FSMC_A17
			PD13/FSMC_A18
			PD14/FSMC_D0
			PD15/FSMC_D1

			PE3/FSMC_A19
			PE4/FSMC_A20
			PE5/FSMC_A21
			PE6/FSMC_A22
			PE7/FSMC_D4
			PE8/FSMC_D5
			PE9/FSMC_D6
			PE10/FSMC_D7
			PE11/FSMC_D8
			PE12/FSMC_D9
			PE13/FSMC_D10
			PE14/FSMC_D11
			PE15/FSMC_D12

			PF0/FSMC_A0
			PF1/FSMC_A1
			PF2/FSMC_A2
			PF3/FSMC_A3
			PF4/FSMC_A4
			PF5/FSMC_A5
			PF12/FSMC_A6
			PF13/FSMC_A7
			PF14/FSMC_A8
			PF15/FSMC_A9

			PG0/FSMC_A10
			PG1/FSMC_A11
			PG2/FSMC_A12
			PG3/FSMC_A13
			PG4/FSMC_A14
			PG5/FSMC_A15
			PG9/FSMC_NE2	- Æ¬Ñ¡ÐÅºÅ
	*/
	/* GPIOD configuration */
	GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_4  | GPIO_PIN_5  |
		                    GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 |
		                    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* GPIOE configuration */
	GPIO_InitStructure.Pin = GPIO_PIN_3 |
		                    GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6 | GPIO_PIN_7 |
		                    GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11|
		                    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* GPIOF configuration */
	GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
		                    GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_12 | GPIO_PIN_13 |
		                    GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* GPIOG configuration */
	GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3 |
		                    GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_9;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* PD6 ×÷ÎªÃ¦ÐÅºÅ, ÅäÖÃÎªGPIOÊäÈëÄ£Ê½£¬Èí¼þ²éÑ¯ */
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*-- FSMC Configuration ------------------------------------------------------*/
	timingWrite.AddressSetupTime = 0x06;
	timingWrite.AddressHoldTime = 0x01;
	timingWrite.DataSetupTime = 0x0C;
	timingWrite.BusTurnAroundDuration = 0x00;
	timingWrite.CLKDivision = 0x00;
	timingWrite.DataLatency = 0x00;
	timingWrite.AccessMode = FSMC_ACCESS_MODE_B;

	timingRead.AddressSetupTime = 0x06;
	timingRead.AddressHoldTime = 0x01;
	timingRead.DataSetupTime = 0x0C;
	timingRead.BusTurnAroundDuration = 0x00;
	timingRead.CLKDivision = 0x00;
	timingRead.DataLatency = 0x00;
	timingRead.AccessMode = FSMC_ACCESS_MODE_B;

	hnorsram.Instance = FSMC_NORSRAM_DEVICE;
	hnorsram.Init.NSBank = FSMC_NORSRAM_BANK2;
	hnorsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hnorsram.Init.MemoryType = FSMC_MEMORY_TYPE_NOR;
	hnorsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hnorsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hnorsram.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hnorsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hnorsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hnorsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;     // ??
	hnorsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hnorsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hnorsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hnorsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;

	HAL_SRAM_Init(&hnorsram, &timingRead, &timingWrite);
}







































