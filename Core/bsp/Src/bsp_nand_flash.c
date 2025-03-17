/*
 * bsp_nand_flash.c
 *
 *  Created on: Mar 15, 2025
 *      Author: compro
 */
#include "bsp.h"

/* ¶¨ÒåNAND FlashµÄÎïÀíµØÖ·¡£Õâ¸öÊÇÓÉÓ²¼þ¾ö¶¨µÄ */
#define Bank2_NAND_ADDR    ((uint32_t)0x70000000)
#define Bank_NAND_ADDR     Bank2_NAND_ADDR

/* ¶¨Òå²Ù×÷NAND FlashÓÃµ½3¸öºê */
#define NAND_CMD_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA)
#define NAND_ADDR_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA)
#define NAND_DATA_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA)

/* Âß¼­¿éºÅÓ³Éä±í¡£ºÃ¿é×ÜÊýµÄ2%ÓÃÓÚ±¸·ÝÇø£¬Òò´ËÊý×éÎ¬ÊýµÍÓÚ1024¡£ LUT = Look Up Table */
static uint16_t s_usLUT[NAND_BLOCK_COUNT];

static uint16_t s_usValidDataBlockCount;	/* ÓÐÐ§µÄÊý¾Ý¿é¸öÊý */

static uint8_t s_ucTempBuf[NAND_PAGE_TOTAL_SIZE];	/* ´ó»º³åÇø£¬2112×Ö½Ú. ÓÃÓÚ¶Á³ö±È½Ï */

static uint8_t NAND_BuildLUT(void);
static uint8_t FSMC_NAND_GetStatus(void);
static uint16_t NAND_FindFreeBlock (void);
static uint8_t NAND_MarkUsedBlock(uint32_t _ulBlockNo);
static void NAND_MarkBadBlock(uint32_t _ulBlockNo);
static uint16_t NAND_AddrToPhyBlockNo(uint32_t _ulMemAddr);
static uint8_t NAND_IsBufOk(uint8_t *_pBuf, uint32_t _ulLen, uint8_t _ucValue);
uint8_t NAND_WriteToNewBlock(uint32_t _ulPhyPageNo, uint8_t *_pWriteBuf, uint16_t _usOffset, uint16_t _usSize);
static uint8_t NAND_IsFreeBlock(uint32_t _ulBlockNo);

static uint16_t NAND_LBNtoPBN(uint32_t _uiLBN);

//static void FSMC_NAND_Init(void);

/*
*********************************************************************************************************
*	Func name: FSMC_NAND_Init
*********************************************************************************************************
*/
static void BSP_FSMC_NAND_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NAND_HandleTypeDef hnand1;
	FSMC_NAND_PCC_TimingTypeDef ComSpaceTiming = {0};
	FSMC_NAND_PCC_TimingTypeDef AttSpaceTiming = {0};

	/*--NAND Flash GPIOs ÅäÖÃ  ------
	 * PD0/FSMC_D2
	 * PD1/FSMC_D3
	 * PD4/FSMC_NOE
	 * PD5/FSMC_NWE
	 * PD7/FSMC_NCE2
	 * PD11/FSMC_A16
	 * PD12/FSMC_A17
	 * PD14/FSMC_D0
	 * PD15/FSMC_D1
	 *
	 * PE7/FSMC_D4
	 * PE8/FSMC_D5
	 * PE9/FSMC_D6
	 * PE10/FSMC_D7
	 *
	 * PG6/FSMC_INT2	(±¾Àý³ÌÓÃ²éÑ¯·½Ê½ÅÐÃ¦£¬´Ë¿ÚÏß×÷ÎªÆÕÍ¨GPIOÊäÈë¹¦ÄÜÊ¹ÓÃ)
	 */
	 //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	//RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	__HAL_RCC_FSMC_CLK_ENABLE();

	/* GPIOD */
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |
            GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* GPIOE */
	GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	/*  GPIOG, PG6 */
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	/*
	Defines the number of HCLK cycles to setup address before the command assertion for NAND-Flash
	read or write access  to common/Attribute or I/O memory space (depending on the memory space
	timing to be configured).This parameter can be a value between 0 and 0xFF.
	*/
	ComSpaceTiming.SetupTime = 0x1;
	/*
	Defines the minimum number of HCLK cycles to assert the command for NAND-Flash read or write
	access to common/Attribute or I/O memory space (depending on the memory space timing to be
	configured). This parameter can be a number between 0x00 and 0xFF
	*/
	ComSpaceTiming.WaitSetupTime = 0x3;
	/*
	Defines the number of HCLK clock cycles to hold address (and data for write access) after the
	command deassertion for NAND-Flash read or write access to common/Attribute or I/O memory space
	(depending on the memory space timing to be configured).
	This parameter can be a number between 0x00 and 0xFF
	*/
	ComSpaceTiming.HoldSetupTime = 0x2;
	/*
	Defines the number of HCLK clock cycles during which the databus is kept in HiZ after the start
	of a NAND-Flash  write access to common/Attribute or I/O memory space (depending on the memory
	space timing to be configured). This parameter can be a number between 0x00 and 0xFF
	 */
	ComSpaceTiming.HiZSetupTime = 0x1;

	AttSpaceTiming.SetupTime = 0x1;
	AttSpaceTiming.WaitSetupTime = 0x3;
	AttSpaceTiming.HoldSetupTime = 0x2;
	AttSpaceTiming.HiZSetupTime = 0x1;

	hnand1.Instance = FSMC_NAND_DEVICE;
	hnand1.Init.NandBank = FSMC_NAND_BANK2;
	hnand1.Init.Waitfeature = FSMC_NAND_PCC_WAIT_FEATURE_DISABLE;
	hnand1.Init.MemoryDataWidth =  FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
	hnand1.Init.EccComputation = FSMC_NAND_ECC_ENABLE;
	hnand1.Init.ECCPageSize = FSMC_NAND_ECC_PAGE_SIZE_2048BYTE;
	hnand1.Init.TCLRSetupTime = 0x1;
	hnand1.Init.TARSetupTime = 0x1;

	HAL_NAND_Init(&hnand1, &ComSpaceTiming, &AttSpaceTiming);


}


/*
*********************************************************************************************************
* Func name: NAND_Init
*********************************************************************************************************
*/
uint8_t NAND_Init(void)
{
	uint8_t Status;

	BSP_FSMC_NAND_Init();
}
