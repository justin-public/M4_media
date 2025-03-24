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
#define NOR_IS_BUSY() (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == GPIO_PIN_RESET)

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
	FSMC_NORSRAM_TimingTypeDef  timing;
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
#if 0
	timing.AddressSetupTime = 0x06;
	timing.AddressHoldTime = 0x01;
	timing.DataSetupTime = 0x0C;
	timing.BusTurnAroundDuration = 0x00;
	timing.CLKDivision = 0x00;
	timing.DataLatency = 0x00;
	timing.AccessMode = FSMC_ACCESS_MODE_B;
#endif
	timing.AddressSetupTime = 6;
	timing.AddressHoldTime = 1;
	timing.DataSetupTime = 12;
	timing.BusTurnAroundDuration = 0;
	timing.CLKDivision = 0;
	timing.DataLatency = 0;
	timing.AccessMode = FSMC_ACCESS_MODE_B;

	hnorsram.Instance = FSMC_NORSRAM_DEVICE;
	hnorsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

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

	HAL_SRAM_Init(&hnorsram, &timing, &timing);
}

/*
*********************************************************************************************************
*	Func Type: NOR_ReadID
*********************************************************************************************************
*/
uint32_t NOR_ReadID(void)
{
	uint32_t uiID;
	uint8_t id1, id2, id3, id4;

	NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x0090);

	id1 = *(__IO uint16_t *) ADDR_SHIFT(0x0000);
	id2 = *(__IO uint16_t *) ADDR_SHIFT(0x0001);
	id3 = *(__IO uint16_t *) ADDR_SHIFT(0x000E);
	id4 = *(__IO uint16_t *) ADDR_SHIFT(0x000F);

	uiID = ((uint32_t)id1 << 24) | ((uint32_t)id2 << 16)  | ((uint32_t)id3 << 8) | id4;

	NOR_WRITE(NOR_FLASH_ADDR, 0x00F0 );

	return uiID;
}

static void NOR_QuitToReadStatus(void)
{
	NOR_WRITE(ADDR_SHIFT(0x00555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x002AA), 0x0055);
	NOR_WRITE(NOR_FLASH_ADDR, 0x00F0 );
}

static uint8_t NOR_GetStatus(uint32_t Timeout)
{
	uint16_t val1 = 0x00;
	uint16_t val2 = 0x00;
	uint8_t status = NOR_ONGOING;
	uint32_t timeout = Timeout;

	while ((!NOR_IS_BUSY()) && (timeout > 0))
	{
		timeout--;
	}

	timeout = Timeout;
	while(NOR_IS_BUSY() && (timeout > 0))
	{
		timeout--;
	}


	while ((Timeout != 0x00) && (status != NOR_SUCCESS))
	{
		Timeout--;

		/* Read DQ6 */
		val1 = *(__IO uint16_t *)(NOR_FLASH_ADDR);
		val2 = *(__IO uint16_t *)(NOR_FLASH_ADDR);

		/* If DQ6 did not toggle between the two reads then return NOR_Success */
		if ((val1 & 0x0040) == (val2 & 0x0040))
		{
			return NOR_SUCCESS;
		}

		/* Read DQ2 */
		if((val1 & 0x0020) != 0x0020)
		{
			status = NOR_ONGOING;
		}

		val1 = *(__IO uint16_t *)(NOR_FLASH_ADDR);
		val2 = *(__IO uint16_t *)(NOR_FLASH_ADDR);

		if((val1 & 0x0040) == (val2 & 0x0040))
		{
			return NOR_SUCCESS;
		}
		else if ((val1 & 0x0020) == 0x0020)
		{
			status = NOR_ERROR;
			NOR_QuitToReadStatus();
		}
	}

	if (Timeout == 0x00)
	{
		status = NOR_TIMEOUT;
		NOR_QuitToReadStatus();
	}

	return (status);
}

uint8_t NOR_EraseChip(void)
{
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x0080);
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x0010);

	return (NOR_GetStatus(ChipErase_Timeout));
}

uint8_t NOR_EraseSector(uint32_t _uiBlockAddr)
{
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x0080);
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
	NOR_WRITE((NOR_FLASH_ADDR + _uiBlockAddr), 0x30);

	return (NOR_GetStatus(BlockErase_Timeout));
}

uint8_t NOR_ReadByte(uint32_t _uiWriteAddr)
{
	uint16_t usHalfWord;

	if (_uiWriteAddr % 2)	/* ÆæÊýµØÖ· */
	{
		usHalfWord = *(uint16_t *)(NOR_FLASH_ADDR + _uiWriteAddr - 1);
		return (usHalfWord >> 8);	/* È¡¸ß8Bit */
	}
	else	/* Å¼ÊýµØÖ· */
	{
		usHalfWord = *(uint16_t *)(NOR_FLASH_ADDR + _uiWriteAddr);
		return usHalfWord;	/* È¡µÍ8Bit */
	}
}

void NOR_ReadBuffer(uint8_t *_pBuf, uint32_t _uiWriteAddr, uint32_t _uiBytes)
{
	uint16_t usHalfWord;
	uint16_t *pNor16;
	uint32_t i;
	uint32_t uiNum;

	uiNum = _uiBytes;

	if (_uiWriteAddr % 2)	/* ÆæÊýµØÖ· */
	{
		usHalfWord = *(uint16_t *)(NOR_FLASH_ADDR + _uiWriteAddr - 1);
		*_pBuf++ = (usHalfWord >> 8);	/* È¡¸ß8Bit */
		uiNum--;
		_uiWriteAddr++;		/* ±äÎªÅ¼Êý */
	}

	/* °´ÕÕË«×Ö½ÚÄ£Ê½Á¬Ðø¶ÁÈ¡NORÊý¾ÝÖÁ»º³åÇø_pBuf */
	pNor16 = (uint16_t *)(NOR_FLASH_ADDR + _uiWriteAddr);
	for (i = 0; i < uiNum / 2; i++)
	{
		usHalfWord = *pNor16++;
		*_pBuf++ = usHalfWord;
		*_pBuf++ = usHalfWord >> 8;
		uiNum -= 2;
	}

	/* ´¦Àí×îºó1¸ö×Ö½Ú */
	if (uiNum == 1)
	{
		*_pBuf++ = *pNor16;
	}
}

uint8_t NOR_WriteHalfWord(uint32_t _uiWriteAddr, uint16_t _usData)
{
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x0555), 0x00A0);
	NOR_WRITE(NOR_FLASH_ADDR + _uiWriteAddr, _usData);

	return (NOR_GetStatus(Program_Timeout));
}

uint8_t NOR_WriteByte(uint32_t _uiWriteAddr, uint8_t _ucByte)
{
	uint16_t usHalfWord;

	if (_uiWriteAddr % 2)	/* ÆæÊýµØÖ· */
	{
		/* ¶Á³ö2×Ö½ÚÊý¾Ý£¬È»ºó¸ÄÐ´¸ß×Ö½Ú£¬Î¬³ÖÒÔÇ°µÄµÍ×Ö½ÚÊý¾Ý²»±ä */
		usHalfWord = *(uint16_t *)(NOR_FLASH_ADDR + _uiWriteAddr - 1);
		usHalfWord &= 0x00FF;
		usHalfWord |= (_ucByte << 8);
	}
	else
	{
		/* ¶ÁÈ¡NORÔ­Ê¼Êý¾Ý£¬±£Áô¸ß×Ö½Ú */
		usHalfWord = *(uint16_t *)(NOR_FLASH_ADDR + _uiWriteAddr);
		usHalfWord &= 0xFF00;
		usHalfWord |= _ucByte;
	}
	return NOR_WriteHalfWord(_uiWriteAddr, usHalfWord);
}

uint8_t NOR_WriteInPage(uint16_t *pBuffer, uint32_t _uiWriteAddr,  uint16_t _usNumHalfword)
{
	uint32_t lastloadedaddress;
	uint32_t currentaddress;
	uint32_t endaddress;

	/* pdf ±í7.7 Ð´Èë»º³åÆ÷±à³Ì

		Ð´Èë»º³åÆ÷±à³ÌÔÊÐíÏµÍ³ÔÚÒ»¸ö±à³Ì²Ù×÷ÖÐÐ´Èë×î¶à32 ¸ö×Ö¡£Óë±ê×¼µÄ¡° ×Ö¡± ±à³ÌËã·¨Ïà±È£¬Õâ¿ÉÒÔÓÐÐ§µØ
		¼Ó¿ì×Ö±à³ÌËÙ¶È¡£
	*/

	if (_usNumHalfword > 32)
	{
		return NOR_ERROR;
	}

	if ((_uiWriteAddr % 2) != 0)
	{
		return NOR_ERROR;
	}

	_uiWriteAddr = _uiWriteAddr / 2;

	currentaddress = _uiWriteAddr;
	endaddress = _uiWriteAddr + _usNumHalfword - 1;
	lastloadedaddress = _uiWriteAddr;

	/* ½âËøÃüÁîÐòÁÐ */
	NOR_WRITE(ADDR_SHIFT(0x00555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);

	/* Write Write Buffer Load Command */
	NOR_WRITE(ADDR_SHIFT(_uiWriteAddr), 0x0025);
	NOR_WRITE(ADDR_SHIFT(_uiWriteAddr), (_usNumHalfword - 1));

	/*  Load Data into NOR Buffer */
	while (currentaddress <= endaddress)
	{
		/* Store last loaded address & data value (for polling) */
		lastloadedaddress = currentaddress;

		NOR_WRITE(ADDR_SHIFT(currentaddress), *pBuffer++);
		currentaddress += 1;
	}

	NOR_WRITE(ADDR_SHIFT(lastloadedaddress), 0x29);

	return (NOR_GetStatus(Program_Timeout));
}


uint8_t NOR_WriteBuffer(uint8_t *_pBuf, uint32_t _uiWriteAddr, uint32_t _uiBytes)
{
	uint16_t usHalfWord;
	uint32_t i;
	uint32_t uiNum;
	uint8_t ucStatus;

	uiNum = _uiBytes;
	/* ´¦ÀíÊ××Ö½Ú */
	if (_uiWriteAddr % 2)	/* ÆæÊýµØÖ· */
	{
		/* ¶Á³ö2×Ö½ÚÊý¾Ý£¬È»ºó¸ÄÐ´¸ß×Ö½Ú£¬Î¬³ÖÒÔÇ°µÄµÍ×Ö½ÚÊý¾Ý²»±ä */
		usHalfWord = *(uint16_t *)(NOR_FLASH_ADDR + _uiWriteAddr - 1);
		usHalfWord &= 0x00FF;
		usHalfWord |= ((*_pBuf++) << 8);

		ucStatus = NOR_WriteHalfWord(_uiWriteAddr - 1, usHalfWord);
		if (ucStatus != NOR_SUCCESS)
		{
			goto err_quit;
		}

		uiNum--;
		_uiWriteAddr++;		/* ±äÎªÅ¼Êý */
	}

	/* °´ÕÕË«×Ö½ÚÄ£Ê½Á¬Ðø±à³ÌNORÊý¾Ý */
	for (i = 0; i < uiNum / 2; i++)
	{
		usHalfWord = *_pBuf++;
		usHalfWord |= ((*_pBuf++) << 8);

		ucStatus = NOR_WriteHalfWord(_uiWriteAddr, usHalfWord);
		if (ucStatus != NOR_SUCCESS)
		{
			goto err_quit;
		}

		_uiWriteAddr += 2;
	}

	/* ´¦Àí×îºó1¸ö×Ö½Ú */
	if (uiNum % 2)
	{
		/* ¶ÁÈ¡NORÔ­Ê¼Êý¾Ý£¬±£Áô¸ß×Ö½Ú */
		usHalfWord = *(uint16_t *)(NOR_FLASH_ADDR + _uiWriteAddr);
		usHalfWord &= 0xFF00;
		usHalfWord |= (*_pBuf++);

		ucStatus = NOR_WriteHalfWord(_uiWriteAddr, usHalfWord);
		if (ucStatus != NOR_SUCCESS)
		{
			goto err_quit;
		}
	}
	ucStatus = NOR_SUCCESS;
err_quit:
	return 	ucStatus;
}













































