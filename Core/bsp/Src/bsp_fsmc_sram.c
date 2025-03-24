/*
 * bsp_fsmc_sram.c
 *
 *  Created on: Mar 17, 2025
 *      Author: compro
 */
#include "bsp.h"

/*
*********************************************************************************************************
*	func name: bsp_InitExtSRAM
*	SRAM_HandleTypeDef , FSMC_NORSRAM_TimingTypeDef
*********************************************************************************************************
*/
#if 1
void bsp_InitExtSRAM(void)
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

	/* SRAM µÄ GPIO £º
		PD0/FSMC_D2
		PD1/FSMC_D3
		PD4/FSMC_NOE
		PD5/FSMC_NWE
		PD8/FSMC_D13
		PD9/FSMC_D14
		PD10/FSMC_D15
		PD11/FSMC_A16
		PD12/FSMC_A17
		PD13/FSMC_A18
		PD14/FSMC_D0
		PD15/FSMC_D1

		PE0/FSMC_NBL0
		PE1/FSMC_NBL1
		PE3/FSMC_A19
		PE4/FSMC_A20	-- ²ÎÓëÆ¬Ñ¡µÄÒëÂë
		PE5/FSMC_A21	-- ²ÎÓëÆ¬Ñ¡µÄÒëÂë
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
		PG10/FSMC_NE3	--- Æ¬Ñ¡Ö÷ÐÅºÅ
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
	GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_3 |
		                    GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_7 |
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
		                    GPIO_PIN_4  | GPIO_PIN_5  |GPIO_PIN_10;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	/*-- FSMC Configuration ------------------------------------------------------*/
	timingWrite.AddressSetupTime = 3;
	timingWrite.AddressHoldTime = 0;
	timingWrite.DataSetupTime = 2;
	timingWrite.BusTurnAroundDuration = 1;
	timingWrite.CLKDivision = 0;
	timingWrite.DataLatency = 0;
	timingWrite.AccessMode = FSMC_ACCESS_MODE_A;

	timingRead.AddressSetupTime = 3;
	timingRead.AddressHoldTime = 0;
	timingRead.DataSetupTime = 2;
	timingRead.BusTurnAroundDuration = 1;
	timingRead.CLKDivision = 0;
	timingRead.DataLatency = 0;
	timingRead.AccessMode = FSMC_ACCESS_MODE_A;

	hnorsram.Instance = FSMC_NORSRAM_DEVICE;
	hnorsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	hnorsram.Init.NSBank = FSMC_NORSRAM_BANK3;
	hnorsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hnorsram.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	hnorsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hnorsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hnorsram.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hnorsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hnorsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hnorsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hnorsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hnorsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hnorsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hnorsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;

	HAL_SRAM_Init(&hnorsram, &timingRead, &timingWrite);
}
#endif

#if 0
void bsp_InitExtSRAM(void)
{
	FSMC_NORSRAM_InitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAM_TimingTypeDef  p;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clocks */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/* Enable FSMC clock */
	__HAL_RCC_FSMC_CLK_ENABLE();

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
	GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_3  | GPIO_PIN_4  |
	                          GPIO_PIN_5  | GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  |
	                          GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
	                          GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* GPIOF configuration */
	GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
	                          GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_12 | GPIO_PIN_13 |
	                          GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* GPIOG configuration */
	GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
	                          GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_10;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	p.AddressSetupTime = 3;
	p.AddressHoldTime = 0;
    p.DataSetupTime = 2;
	p.BusTurnAroundDuration = 1;
	p.CLKDivision = 0;
	p.DataLatency = 0;
	p.AccessMode = FSMC_ACCESS_MODE_A;


	// FSMC_NORSRAM_BANK1
	// FSMC_NORSRAM_BANK3 ???
	FSMC_NORSRAMInitStructure.NSBank = FSMC_NORSRAM_BANK3;
	FSMC_NORSRAMInitStructure.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	FSMC_NORSRAMInitStructure.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	FSMC_NORSRAMInitStructure.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	FSMC_NORSRAMInitStructure.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	FSMC_NORSRAMInitStructure.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	FSMC_NORSRAMInitStructure.WrapMode = FSMC_WRAP_MODE_DISABLE;
	FSMC_NORSRAMInitStructure.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	FSMC_NORSRAMInitStructure.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	FSMC_NORSRAMInitStructure.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	FSMC_NORSRAMInitStructure.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	FSMC_NORSRAMInitStructure.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	FSMC_NORSRAMInitStructure.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	FSMC_NORSRAMInitStructure.ContinuousClock = FSMC_CONTINUOUS_CLOCK_SYNC_ONLY;
	FSMC_NORSRAMInitStructure.WriteFifo = FSMC_WRITE_BURST_ENABLE;
	FSMC_NORSRAMInitStructure.PageSize = FSMC_PAGE_SIZE_NONE;

	FSMC_NORSRAM_Init(FSMC_NORSRAM_DEVICE, &FSMC_NORSRAMInitStructure);
	FSMC_NORSRAM_Timing_Init(FSMC_NORSRAM_DEVICE, &p, FSMC_NORSRAM_BANK3);    // FSMC_NORSRAM_BANK3

	//FSMC_NORSRAM_EnableBank(FSMC_NORSRAM_BANK3);
}
#endif

uint8_t bsp_TestExtSRAM(void)
{
	uint32_t i;
	uint32_t *pSRAM;
	uint8_t *pBytes;
	uint32_t err;
	const uint8_t ByteBuf[4] = {0x55, 0xA5, 0x5A, 0xAA};

	pSRAM = (uint32_t *)EXT_SRAM_ADDR;
	for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
	{
		*pSRAM++ = i;
	}

	err = 0;
	pSRAM = (uint32_t *)EXT_SRAM_ADDR;
	for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
	{
		if (*pSRAM++ != i)
		{
			err++;
		}
	}

	if (err >  0)
	{
		return  (4 * err);
	}

	pSRAM = (uint32_t *)EXT_SRAM_ADDR;
	for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
	{
		*pSRAM = ~*pSRAM;
		pSRAM++;
	}

	err = 0;
	pSRAM = (uint32_t *)EXT_SRAM_ADDR;
	for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
	{
		if (*pSRAM++ != (~i))
		{
			err++;
		}
	}

	if (err >  0)
	{
		return (4 * err);
	}

	pBytes = (uint8_t *)EXT_SRAM_ADDR;
	for (i = 0; i < sizeof(ByteBuf); i++)
	{
		*pBytes++ = ByteBuf[i];
	}

	err = 0;
	pBytes = (uint8_t *)EXT_SRAM_ADDR;
	for (i = 0; i < sizeof(ByteBuf); i++)
	{
		if (*pBytes++ != ByteBuf[i])
		{
			err++;
		}
	}
	if (err >  0)
	{
		return err;
	}
	return 0;
}
























