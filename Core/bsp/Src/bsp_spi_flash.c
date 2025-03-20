/*
 * bsp_spi_flash.c
 *
 *  Created on: Mar 18, 2025
 *      Author: compro
 */
#include "bsp.h"

SPI_HandleTypeDef  hspi1;

//#define SPI_FLASH			SPI3
#define SPI_FLASH			SPI1

//#define ENABLE_SPI_RCC() 	__HAL_RCC_SPI3_CLK_ENABLE()
#define ENABLE_SPI_RCC() 	__HAL_RCC_SPI1_CLK_ENABLE()

#define SPI_BAUD			SPI_BAUDRATEPRESCALER_4

#define SF_CS_GPIO			GPIOF
#define SF_CS_PIN			GPIO_PIN_8

#define SF_CS_LOW() HAL_GPIO_WritePin(SF_CS_GPIO, SF_CS_PIN, GPIO_PIN_RESET)

#define SF_CS_HIGH() HAL_GPIO_WritePin(SF_CS_GPIO, SF_CS_PIN, GPIO_PIN_SET)

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
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;      // SPI1(GPIO_AF5_SPI1) , SPI3 (GPIO_AF6_SPI3)
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

	SF_CS_HIGH();
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	hspi1.Instance = SPI_FLASH;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_SPI_DISABLE(&hspi1);  // SPI 비활성화
	__HAL_SPI_ENABLE(&hspi1);   // SPI 활성화

	sf_ReadInfo();

	SF_CS_LOW();
	sf_SendByte(CMD_DISWR);
	SF_CS_HIGH();
	sf_WaitForWriteEnd();
	sf_WriteStatus(0);
}

void sf_ReadInfo(void)
{
	g_tSF.ChipID = sf_ReadID();
	//char buffer[20];
	//sprintf(buffer, "CHIPID: 0x%08lX\r\n", g_tSF.ChipID);
	//UART1_Transmit_String(buffer);
	//HAL_Delay(1000);
	switch (g_tSF.ChipID)
	{
		case SST25VF016B_ID:
			strcpy(g_tSF.ChipName, "SST25VF016B");
			UART1_Transmit_String(g_tSF.ChipName);
			g_tSF.TotalSize = 2 * 1024 * 1024;	/* ×ÜÈÝÁ¿ = 2M */
			g_tSF.PageSize = 4 * 1024;			/* Ò³Ãæ´óÐ¡ = 4K */
		break;
		case MX25L1606E_ID:
			strcpy(g_tSF.ChipName, "MX25L1606E");
			UART1_Transmit_String(g_tSF.ChipName);
			g_tSF.TotalSize = 2 * 1024 * 1024;	/* ×ÜÈÝÁ¿ = 2M */
			g_tSF.PageSize = 4 * 1024;			/* Ò³Ãæ´óÐ¡ = 4K */
		break;
		case W25Q64BV_ID:
			strcpy(g_tSF.ChipName, "W25Q64BV");
			UART1_Transmit_String(g_tSF.ChipName);
			g_tSF.TotalSize = 8 * 1024 * 1024;	/* ×ÜÈÝÁ¿ = 8M */
			g_tSF.PageSize = 4 * 1024;			/* Ò³Ãæ´óÐ¡ = 4K */
		break;
		default:
			strcpy(g_tSF.ChipName, "Unknow Flash");
			UART1_Transmit_String(g_tSF.ChipName);
			g_tSF.TotalSize = 2 * 1024 * 1024;
			g_tSF.PageSize = 4 * 1024;
		break;
	}
}

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

#if 0
static uint8_t sf_SendByte(uint8_t _ucValue)
{
	uint8_t rxData = 0;

	// TXE 플래그 확인 (전송 버퍼가 비어있는지)
	while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET);

	// 데이터 전송
	*((__IO uint8_t *)&hspi1.Instance->DR) = _ucValue;

	// RXNE 플래그 확인 (수신 버퍼에 데이터가 있는지)
	while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET);

	// 데이터 수신
	rxData = *((__IO uint8_t *)&hspi1.Instance->DR);

	return rxData;
}
#endif

static uint8_t sf_SendByte(uint8_t _ucValue)
{
	uint8_t rxData = 0;

	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(&hspi1, &_ucValue, &rxData, 1, HAL_MAX_DELAY);

	if (status != HAL_OK){
		// 에러 처리
	    // 예: Error_Handler();
	}
	return rxData;
}

/*
*********************************************************************************************************
*	Func name: sf_WaitForWriteEnd
*********************************************************************************************************
*/
static void sf_WaitForWriteEnd(void)
{
	SF_CS_LOW();
	sf_SendByte(CMD_RDSR);
	while((sf_SendByte(DUMMY_BYTE) & WIP_FLAG) == SET);
	SF_CS_HIGH();
}

/*
*********************************************************************************************************
* Func name: sf_WriteStatus
*********************************************************************************************************
*/
static void sf_WriteStatus(uint8_t _ucValue)
{
	if (g_tSF.ChipID == SST25VF016B_ID)
	{
		SF_CS_LOW();									/* Ê¹ÄÜÆ¬Ñ¡ */
		sf_SendByte(CMD_EWRSR);							/* ·¢ËÍÃüÁî£¬ ÔÊÐíÐ´×´Ì¬¼Ä´æÆ÷ */
		SF_CS_HIGH();									/* ½ûÄÜÆ¬Ñ¡ */

		/* µÚ2²½£ºÔÙÐ´×´Ì¬¼Ä´æÆ÷ */
		SF_CS_LOW();									/* Ê¹ÄÜÆ¬Ñ¡ */
		sf_SendByte(CMD_WRSR);							/* ·¢ËÍÃüÁî£¬ Ð´×´Ì¬¼Ä´æÆ÷ */
		sf_SendByte(_ucValue);							/* ·¢ËÍÊý¾Ý£º×´Ì¬¼Ä´æÆ÷µÄÖµ */
		SF_CS_HIGH();
	}
	else
	{
		SF_CS_LOW();									/* Ê¹ÄÜÆ¬Ñ¡ */
		sf_SendByte(CMD_WRSR);							/* ·¢ËÍÃüÁî£¬ Ð´×´Ì¬¼Ä´æÆ÷ */
		sf_SendByte(_ucValue);							/* ·¢ËÍÊý¾Ý£º×´Ì¬¼Ä´æÆ÷µÄÖµ */
		SF_CS_HIGH();									/* ½ûÄÜÆ¬Ñ¡ */
	}
}


















































