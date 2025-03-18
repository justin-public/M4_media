/*
 * bsp_spi_flash.c
 *
 *  Created on: Mar 18, 2025
 *      Author: compro
 */
#include "bsp.h"

SPI_HandleTypeDef  hspi;

/*
	STM32F4XX Ê±ÖÓ¼ÆËã.
		HCLK = 168M
		PCLK1 = HCLK / 4 = 42M
		PCLK2 = HCLK / 2 = 84M

		SPI2¡¢SPI3 ÔÚ PCLK1, Ê±ÖÓ42M
		SPI1       ÔÚ PCLK2, Ê±ÖÓ84M

		STM32F4 Ö§³ÖµÄ×î´óSPIÊ±ÖÓÎª 37.5 Mbits/S, Òò´ËÐèÒª·ÖÆµ¡£
*/

#ifdef STM32_X3		/* °²¸»À³ STM32-X3 ¿ª·¢°å */
	/*
		°²¸»À³STM32-X3 ¿ÚÏß·ÖÅä£º ´®ÐÐFlashÐÍºÅÎª W25Q64BVSSIG (80MHz)
		PB12 = CS
		PB13 = SCK
		PB14 = MISO
		PB15 = MOSI

		STM32Ó²¼þSPI½Ó¿Ú = SPI2
	*/
	#define SPI_FLASH			SPI2

	#define ENABLE_SPI_RCC() 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI2, ENABLE)

	#define SPI_BAUD			SPI_BaudRatePrescaler_1		/* Ñ¡Ôñ2·ÖÆµÊ±, SCKÊ±ÖÓ = 21M */

	/* Æ¬Ñ¡¿ÚÏßÖÃµÍÑ¡ÖÐ  */
	#define SF_CS_LOW()			GPIOB->BSRRH = GPIO_Pin_12

	/* Æ¬Ñ¡¿ÚÏßÖÃ¸ß²»Ñ¡ÖÐ */
	#define SF_CS_HIGH()		GPIOB->BSRRL = GPIO_Pin_12
#else
	/*
	Alinx STM32-V5 development board port assignment: Serial Flash model is W25Q64BVSSIG (80MHz)
	PB3/SPI3_SCK/SPI1_SCK
	PB4/SPI3_MISO/SPI1_MISO
	PB5/SPI3_MOSI/SPI1_MOSI
	PF8/SF_CS

	STM32 hardware SPI interface = SPI3 or SPI1

	Since the clock source of SPI1 is 84MHz and the clock source of SPI3 is 42MHz, SPI1 is selected in software to achieve faster speed.
	*/
	//#define SPI_FLASH			SPI3
	#define SPI_FLASH			SPI1

	//#define ENABLE_SPI_RCC() 	__HAL_RCC_SPI3_CLK_ENABLE()
	#define ENABLE_SPI_RCC() 	__HAL_RCC_SPI1_CLK_ENABLE()

	/*
		¡¾SPIÊ±ÖÓ×î¿ìÊÇ2·ÖÆµ£¬²»Ö§³Ö²»·ÖÆµ¡¿
		Èç¹ûÊÇSPI1£¬2·ÖÆµÊ±SCKÊ±ÖÓ = 42M£¬4·ÖÆµÊ±SCKÊ±ÖÓ = 21M
		Èç¹ûÊÇSPI3, 2·ÖÆµÊ±SCKÊ±ÖÓ = 21M
	*/
	#define SPI_BAUD			SPI_BAUDRATEPRESCALER_4

	/* Æ¬Ñ¡GPIO¶Ë¿Ú  */
	#define SF_CS_GPIO			GPIOF
	#define SF_CS_PIN			GPIO_PIN_8
#endif

/* Æ¬Ñ¡¿ÚÏßÖÃµÍÑ¡ÖÐ  */
//#define SF_CS_LOW()       SF_CS_GPIO->BSRRH = SF_CS_PIN
#define SF_CS_LOW() HAL_GPIO_WritePin(SF_CS_GPIO, SF_CS_PIN, GPIO_PIN_SET)
/* Æ¬Ñ¡¿ÚÏßÖÃ¸ß²»Ñ¡ÖÐ */
//#define SF_CS_HIGH()      SF_CS_GPIO->BSRRL = SF_CS_PIN
#define SF_CS_HIGH() HAL_GPIO_WritePin(SF_CS_GPIO, SF_CS_PIN, GPIO_PIN_RESET)

#define CMD_AAI       0xAD  	/* AAI Á¬Ðø±à³ÌÖ¸Áî(FOR SST25VF016B) */
#define CMD_DISWR	  0x04		/* ½ûÖ¹Ð´, ÍË³öAAI×´Ì¬ */
#define CMD_EWRSR	  0x50		/* ÔÊÐíÐ´×´Ì¬¼Ä´æÆ÷µÄÃüÁî */
#define CMD_WRSR      0x01  	/* Ð´×´Ì¬¼Ä´æÆ÷ÃüÁî */
#define CMD_WREN      0x06		/* Ð´Ê¹ÄÜÃüÁî */
#define CMD_READ      0x03  	/* ¶ÁÊý¾ÝÇøÃüÁî */
#define CMD_RDSR      0x05		/* ¶Á×´Ì¬¼Ä´æÆ÷ÃüÁî */
#define CMD_RDID      0x9F		/* ¶ÁÆ÷¼þIDÃüÁî */
#define CMD_SE        0x20		/* ²Á³ýÉÈÇøÃüÁî */
#define CMD_BE        0xC7		/* ÅúÁ¿²Á³ýÃüÁî */
#define DUMMY_BYTE    0xA5		/* ÑÆÃüÁî£¬¿ÉÒÔÎªÈÎÒâÖµ£¬ÓÃÓÚ¶Á²Ù×÷ */

#define WIP_FLAG      0x01		/* ×´Ì¬¼Ä´æÆ÷ÖÐµÄÕýÔÚ±à³Ì±êÖ¾£¨WIP) */

SFLASH_T g_tSF;

void sf_ReadInfo(void);
static uint8_t sf_SendByte(uint8_t _ucValue);
static void sf_WriteEnable(void);
static void sf_WriteStatus(uint8_t _ucValue);
static void sf_WaitForWriteEnd(void);
static uint8_t sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _uiLen);
static uint8_t sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize);
static uint8_t sf_AutoWritePage(uint8_t *_ucpSrc, uint32_t _uiWrAddr, uint16_t _usWrLen);

static void bsp_CfgSPIForSFlash(void);

static uint8_t s_spiBuf[4*1024];	/* ÓÃÓÚÐ´º¯Êý£¬ÏÈ¶Á³öÕû¸öpage£¬ÐÞ¸Ä»º³åÇøºó£¬ÔÙÕû¸öpage»ØÐ´ */

/*
*********************************************************************************************************
* Func name: bsp_InitSpiFlash
*********************************************************************************************************
*/
void bsp_InitSFlash(void)
{
#ifdef STM32_X3		/* °²¸»À³ STM32-X3 ¿ª·¢°å */
	/*
		°²¸»À³STM32-X4 ¿ÚÏß·ÖÅä£º ´®ÐÐFlashÐÍºÅÎª W25Q64BVSSIG (80MHz)
		PB12 = CS
		PB13 = SCK
		PB14 = MISO
		PB15 = MOSI

		STM32Ó²¼þSPI½Ó¿Ú = SPI2
	*/
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* Ê¹ÄÜGPIO Ê±ÖÓ */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* ÅäÖÃ SCK, MISO ¡¢ MOSI Îª¸´ÓÃ¹¦ÄÜ */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* ÅäÖÃÆ¬Ñ¡¿ÚÏßÎªÍÆÍìÊä³öÄ£Ê½ */
		SF_CS_HIGH();		/* Æ¬Ñ¡ÖÃ¸ß£¬²»Ñ¡ÖÐ */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
#else
	/*
		STM32-V5 Development Board Port Assignment: The Serial Flash model is W25Q64BVSSIG (80MHz)
		PB3/SPI3_SCK
		PB4/SPI3_MISO
		PB5/SPI3_MOSI
		PF8/SF_CS
		STM32 hardware SPI interface = SPI3
	*/
	{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
	GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;      // SPI1(GPIO_AF5_SPI1) , SPI3 (GPIO_AF6_SPI3)
	GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);

	SF_CS_HIGH();		/* Æ¬Ñ¡ÖÃ¸ß£¬²»Ñ¡ÖÐ */
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOF,&GPIO_InitStructure);
	}
#endif

	bsp_CfgSPIForSFlash();

	sf_ReadInfo();				/* ×Ô¶¯Ê¶±ðÐ¾Æ¬ÐÍºÅ */
#if 1
	SF_CS_LOW();				/* Èí¼þ·½Ê½£¬Ê¹ÄÜ´®ÐÐFlashÆ¬Ñ¡ */
	sf_SendByte(CMD_DISWR);		/* ·¢ËÍ½ûÖ¹Ð´ÈëµÄÃüÁî,¼´Ê¹ÄÜÈí¼þÐ´±£»¤ */
	SF_CS_HIGH();				/* Èí¼þ·½Ê½£¬½ûÄÜ´®ÐÐFlashÆ¬Ñ¡ */

	sf_WaitForWriteEnd();		/* µÈ´ý´®ÐÐFlashÄÚ²¿²Ù×÷Íê³É */

	sf_WriteStatus(0);			/* ½â³ýËùÓÐBLOCKµÄÐ´±£»¤ */
#endif
}

/*
*********************************************************************************************************
*	Func name: bsp_CfgSPIForSFlash
*********************************************************************************************************
*/
static void bsp_CfgSPIForSFlash(void)
{
	SPI_HandleTypeDef  hspi;

	ENABLE_SPI_RCC();

	hspi.Instance = SPI_FLASH;
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.Direction = SPI_DIRECTION_2LINES;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi.Init.NSS = SPI_NSS_SOFT;

	hspi.Init.BaudRatePrescaler = SPI_BAUD;
	hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	// add
	hspi.Init.TIMode = SPI_TIMODE_DISABLE;                				/* TI mode: disabled */
	hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;              // SPI_CRCCALCULATION_ENABLE , SPI_CRCCALCULATION_DISABLE

	hspi.Init.CRCPolynomial = 7;

	/* Initialize SPI */
	HAL_SPI_Init(&hspi);

	/* Enable SPI */
	//__HAL_SPI_ENABLE(&hspi);
}

/*
*********************************************************************************************************
*	Func name: sf_ReadInfo
*********************************************************************************************************
*/
void sf_ReadInfo(void)
{
	{
		g_tSF.ChipID = sf_ReadID();	/* Ð¾Æ¬ID */

		char buffer[20];
		sprintf(buffer, "ChipID: 0x%08lX\r\n", g_tSF.ChipID);
		UART1_Transmit_String(buffer);
		HAL_Delay(300);

		switch (g_tSF.ChipID)
		{
			case SST25VF016B_ID:
				strcpy(g_tSF.ChipName, "SST25VF016B");
				g_tSF.TotalSize = 2 * 1024 * 1024;	/* ×ÜÈÝÁ¿ = 2M */
				g_tSF.PageSize = 4 * 1024;			/* Ò³Ãæ´óÐ¡ = 4K */
			break;

			case MX25L1606E_ID:
				strcpy(g_tSF.ChipName, "MX25L1606E");
				g_tSF.TotalSize = 2 * 1024 * 1024;	/* ×ÜÈÝÁ¿ = 2M */
				g_tSF.PageSize = 4 * 1024;			/* Ò³Ãæ´óÐ¡ = 4K */
			break;

			case W25Q64BV_ID:
				strcpy(g_tSF.ChipName, "W25Q64BV");
				g_tSF.TotalSize = 8 * 1024 * 1024;	/* ×ÜÈÝÁ¿ = 8M */
				g_tSF.PageSize = 4 * 1024;			/* Ò³Ãæ´óÐ¡ = 4K */
			break;

			default:
				strcpy(g_tSF.ChipName, "Unknow Flash");
				UART1_Transmit_String("Unknow Flash");
				g_tSF.TotalSize = 2 * 1024 * 1024;
				g_tSF.PageSize = 4 * 1024;
			break;
		}
	}
}

/*
*********************************************************************************************************
*	Func name: sf_ReadID
*********************************************************************************************************
*/
uint32_t sf_ReadID(void)
{
	uint32_t uiID;
	uint8_t id1, id2, id3;

	SF_CS_LOW();
	sf_SendByte(CMD_RDID);
	id1 = sf_SendByte(DUMMY_BYTE);					/* ¶ÁIDµÄµÚ1¸ö×Ö½Ú */
	id2 = sf_SendByte(DUMMY_BYTE);					/* ¶ÁIDµÄµÚ2¸ö×Ö½Ú */
	id3 = sf_SendByte(DUMMY_BYTE);					/* ¶ÁIDµÄµÚ2¸ö×Ö½Ú */
	SF_CS_HIGH();

	uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;

	return uiID;
}

/*
*********************************************************************************************************
*	Func name: sf_SendByte
*********************************************************************************************************
*/
static uint8_t sf_SendByte(uint8_t _ucValue)
{
#if 0
	uint8_t rxData = 0;

	while (__HAL_SPI_GET_FLAG(&hspi, SPI_FLAG_TXE) == RESET);

	*((__IO uint8_t *)&hspi.Instance->DR) = _ucValue;

	while (__HAL_SPI_GET_FLAG(&hspi, SPI_FLAG_RXNE) == RESET);

	rxData = *((__IO uint8_t *)&hspi.Instance->DR);

	return rxData;
#endif
	uint8_t rxData = 0;
	HAL_StatusTypeDef status;

	status = HAL_SPI_TransmitReceive(&hspi, &_ucValue, &rxData, 1, 100);   // HAL_MAX_DELAY

	if (status != HAL_OK)
	{
		// 에러 처리 (필요시 구현)
	}
}

/*
*********************************************************************************************************
*	Func name: sf_WaitForWriteEnd
*********************************************************************************************************
*/
static void sf_WaitForWriteEnd(void)
{
	SF_CS_LOW();									/* Ê¹ÄÜÆ¬Ñ¡ */
	sf_SendByte(CMD_RDSR);							/* ·¢ËÍÃüÁî£¬ ¶Á×´Ì¬¼Ä´æÆ÷ */
	while((sf_SendByte(DUMMY_BYTE) & WIP_FLAG) == SET);	/* ÅÐ¶Ï×´Ì¬¼Ä´æÆ÷µÄÃ¦±êÖ¾Î» */
	SF_CS_HIGH();									/* ½ûÄÜÆ¬Ñ¡ */
}

/*
*********************************************************************************************************
*	Func name: sf_WriteStatus
*********************************************************************************************************
*/
static void sf_WriteStatus(uint8_t _ucValue)
{
	if (g_tSF.ChipID == SST25VF016B_ID)
	{
		/* µÚ1²½£ºÏÈÊ¹ÄÜÐ´×´Ì¬¼Ä´æÆ÷ */
		SF_CS_LOW();									/* Ê¹ÄÜÆ¬Ñ¡ */
		sf_SendByte(CMD_EWRSR);							/* ·¢ËÍÃüÁî£¬ ÔÊÐíÐ´×´Ì¬¼Ä´æÆ÷ */
		SF_CS_HIGH();									/* ½ûÄÜÆ¬Ñ¡ */

		/* µÚ2²½£ºÔÙÐ´×´Ì¬¼Ä´æÆ÷ */
		SF_CS_LOW();									/* Ê¹ÄÜÆ¬Ñ¡ */
		sf_SendByte(CMD_WRSR);							/* ·¢ËÍÃüÁî£¬ Ð´×´Ì¬¼Ä´æÆ÷ */
		sf_SendByte(_ucValue);							/* ·¢ËÍÊý¾Ý£º×´Ì¬¼Ä´æÆ÷µÄÖµ */
		SF_CS_HIGH();									/* ½ûÄÜÆ¬Ñ¡ */
	}
	else
	{
		SF_CS_LOW();									/* Ê¹ÄÜÆ¬Ñ¡ */
		sf_SendByte(CMD_WRSR);							/* ·¢ËÍÃüÁî£¬ Ð´×´Ì¬¼Ä´æÆ÷ */
		sf_SendByte(_ucValue);							/* ·¢ËÍÊý¾Ý£º×´Ì¬¼Ä´æÆ÷µÄÖµ */
		SF_CS_HIGH();									/* ½ûÄÜÆ¬Ñ¡ */
	}
}






































