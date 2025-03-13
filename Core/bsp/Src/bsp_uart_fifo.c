/*
 * bsp_uart_fifo.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "bsp.h"

/* ¶¨ÒåÃ¿¸ö´®¿Ú½á¹¹Ìå±äÁ¿ */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* ·¢ËÍ»º³åÇø */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ½ÓÊÕ»º³åÇø */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* ·¢ËÍ»º³åÇø */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ½ÓÊÕ»º³åÇø */
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* ·¢ËÍ»º³åÇø */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* ½ÓÊÕ»º³åÇø */
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* ·¢ËÍ»º³åÇø */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* ½ÓÊÕ»º³åÇø */
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* ·¢ËÍ»º³åÇø */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* ½ÓÊÕ»º³åÇø */
#endif

#if UART6_FIFO_EN == 1
	static UART_T g_tUart6;
	static uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* ·¢ËÍ»º³åÇø */
	static uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* ½ÓÊÕ»º³åÇø */
#endif

static void UartVarInit(void);

static void InitHardUart(void);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_T *_pUart);
static void ConfigUartNVIC(void);

UART_HandleTypeDef USART_InitStructure1;
UART_HandleTypeDef USART_InitStructure2;
UART_HandleTypeDef USART_InitStructure3;
UART_HandleTypeDef USART_InitStructure6;

/*
*********************************************************************************************************
*	Func name: bsp_InitUart
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* ±ØÐëÏÈ³õÊ¼»¯È«¾Ö±äÁ¿,ÔÙÅäÖÃÓ²¼þ */

	InitHardUart();		/* ÅäÖÃ´®¿ÚµÄÓ²¼þ²ÎÊý(²¨ÌØÂÊµÈ) */

	ConfigUartNVIC();	/* ÅäÖÃ´®¿ÚÖÐ¶Ï */
}

/*
*********************************************************************************************************
*	Func name: USART3_SendBefor
*********************************************************************************************************
*/
void USART3_SendBefor(void)
{
	RS485_TX_EN();	/* ÇÐ»»RS485ÊÕ·¢Ð¾Æ¬Îª·¢ËÍÄ£Ê½ */
}


/*
*********************************************************************************************************
*	Func name: USART3_SendOver
*********************************************************************************************************
*/
void USART3_SendOver(void)
{
	RS485_RX_EN();	/* ÇÐ»»RS485ÊÕ·¢Ð¾Æ¬Îª½ÓÊÕÄ£Ê½ */
}

/*
*********************************************************************************************************
*	Func name: UartVarInit
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = USART1;						/* STM32 ´®¿ÚÉè±¸ */
	g_tUart1.pTxBuf = g_TxBuf1;					/* ·¢ËÍ»º³åÇøÖ¸Õë */
	g_tUart1.pRxBuf = g_RxBuf1;					/* ½ÓÊÕ»º³åÇøÖ¸Õë */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;	/* ·¢ËÍ»º³åÇø´óÐ¡ */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* ½ÓÊÕ»º³åÇø´óÐ¡ */
	g_tUart1.usTxWrite = 0;						/* ·¢ËÍFIFOÐ´Ë÷Òý */
	g_tUart1.usTxRead = 0;						/* ·¢ËÍFIFO¶ÁË÷Òý */
	g_tUart1.usRxWrite = 0;						/* ½ÓÊÕFIFOÐ´Ë÷Òý */
	g_tUart1.usRxRead = 0;						/* ½ÓÊÕFIFO¶ÁË÷Òý */
	g_tUart1.usRxCount = 0;						/* ½ÓÊÕµ½µÄÐÂÊý¾Ý¸öÊý */
	g_tUart1.usTxCount = 0;						/* ´ý·¢ËÍµÄÊý¾Ý¸öÊý */
	g_tUart1.SendBefor = 0;						/* ·¢ËÍÊý¾ÝÇ°µÄ»Øµ÷º¯Êý */
	g_tUart1.SendOver = 0;						/* ·¢ËÍÍê±ÏºóµÄ»Øµ÷º¯Êý */
	g_tUart1.ReciveNew = 0;						/* ½ÓÊÕµ½ÐÂÊý¾ÝºóµÄ»Øµ÷º¯Êý */
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						/* STM32 ´®¿ÚÉè±¸ */
	g_tUart2.pTxBuf = g_TxBuf2;					/* ·¢ËÍ»º³åÇøÖ¸Õë */
	g_tUart2.pRxBuf = g_RxBuf2;					/* ½ÓÊÕ»º³åÇøÖ¸Õë */
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;	/* ·¢ËÍ»º³åÇø´óÐ¡ */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* ½ÓÊÕ»º³åÇø´óÐ¡ */
	g_tUart2.usTxWrite = 0;						/* ·¢ËÍFIFOÐ´Ë÷Òý */
	g_tUart2.usTxRead = 0;						/* ·¢ËÍFIFO¶ÁË÷Òý */
	g_tUart2.usRxWrite = 0;						/* ½ÓÊÕFIFOÐ´Ë÷Òý */
	g_tUart2.usRxRead = 0;						/* ½ÓÊÕFIFO¶ÁË÷Òý */
	g_tUart2.usRxCount = 0;						/* ½ÓÊÕµ½µÄÐÂÊý¾Ý¸öÊý */
	g_tUart2.usTxCount = 0;						/* ´ý·¢ËÍµÄÊý¾Ý¸öÊý */
	g_tUart2.SendBefor = 0;						/* ·¢ËÍÊý¾ÝÇ°µÄ»Øµ÷º¯Êý */
	g_tUart2.SendOver = 0;						/* ·¢ËÍÍê±ÏºóµÄ»Øµ÷º¯Êý */
	g_tUart2.ReciveNew = 0;						/* ½ÓÊÕµ½ÐÂÊý¾ÝºóµÄ»Øµ÷º¯Êý */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						/* STM32 ´®¿ÚÉè±¸ */
	g_tUart3.pTxBuf = g_TxBuf3;					/* ·¢ËÍ»º³åÇøÖ¸Õë */
	g_tUart3.pRxBuf = g_RxBuf3;					/* ½ÓÊÕ»º³åÇøÖ¸Õë */
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;	/* ·¢ËÍ»º³åÇø´óÐ¡ */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* ½ÓÊÕ»º³åÇø´óÐ¡ */
	g_tUart3.usTxWrite = 0;						/* ·¢ËÍFIFOÐ´Ë÷Òý */
	g_tUart3.usTxRead = 0;						/* ·¢ËÍFIFO¶ÁË÷Òý */
	g_tUart3.usRxWrite = 0;						/* ½ÓÊÕFIFOÐ´Ë÷Òý */
	g_tUart3.usRxRead = 0;						/* ½ÓÊÕFIFO¶ÁË÷Òý */
	g_tUart3.usRxCount = 0;						/* ½ÓÊÕµ½µÄÐÂÊý¾Ý¸öÊý */
	g_tUart3.usTxCount = 0;						/* ´ý·¢ËÍµÄÊý¾Ý¸öÊý */
	g_tUart3.SendBefor = USART3_SendBefor;		/* ·¢ËÍÊý¾ÝÇ°µÄ»Øµ÷º¯Êý */
	g_tUart3.SendOver = USART3_SendOver;		/* ·¢ËÍÍê±ÏºóµÄ»Øµ÷º¯Êý */
	g_tUart3.ReciveNew = 0;						/* ½ÓÊÕµ½ÐÂÊý¾ÝºóµÄ»Øµ÷º¯Êý */
#endif

#if UART4_FIFO_EN == 1
	g_tUart4.uart = UART4;						/* STM32 ´®¿ÚÉè±¸ */
	g_tUart4.pTxBuf = g_TxBuf4;					/* ·¢ËÍ»º³åÇøÖ¸Õë */
	g_tUart4.pRxBuf = g_RxBuf4;					/* ½ÓÊÕ»º³åÇøÖ¸Õë */
	g_tUart4.usTxBufSize = UART4_TX_BUF_SIZE;	/* ·¢ËÍ»º³åÇø´óÐ¡ */
	g_tUart4.usRxBufSize = UART4_RX_BUF_SIZE;	/* ½ÓÊÕ»º³åÇø´óÐ¡ */
	g_tUart4.usTxWrite = 0;						/* ·¢ËÍFIFOÐ´Ë÷Òý */
	g_tUart4.usTxRead = 0;						/* ·¢ËÍFIFO¶ÁË÷Òý */
	g_tUart4.usRxWrite = 0;						/* ½ÓÊÕFIFOÐ´Ë÷Òý */
	g_tUart4.usRxRead = 0;						/* ½ÓÊÕFIFO¶ÁË÷Òý */
	g_tUart4.usRxCount = 0;						/* ½ÓÊÕµ½µÄÐÂÊý¾Ý¸öÊý */
	g_tUart4.usTxCount = 0;						/* ´ý·¢ËÍµÄÊý¾Ý¸öÊý */
	g_tUart4.SendBefor = 0;						/* ·¢ËÍÊý¾ÝÇ°µÄ»Øµ÷º¯Êý */
	g_tUart4.SendOver = 0;						/* ·¢ËÍÍê±ÏºóµÄ»Øµ÷º¯Êý */
	g_tUart4.ReciveNew = 0;						/* ½ÓÊÕµ½ÐÂÊý¾ÝºóµÄ»Øµ÷º¯Êý */
#endif

#if UART5_FIFO_EN == 1
	g_tUart5.uart = UART5;						/* STM32 ´®¿ÚÉè±¸ */
	g_tUart5.pTxBuf = g_TxBuf5;					/* ·¢ËÍ»º³åÇøÖ¸Õë */
	g_tUart5.pRxBuf = g_RxBuf5;					/* ½ÓÊÕ»º³åÇøÖ¸Õë */
	g_tUart5.usTxBufSize = UART5_TX_BUF_SIZE;	/* ·¢ËÍ»º³åÇø´óÐ¡ */
	g_tUart5.usRxBufSize = UART5_RX_BUF_SIZE;	/* ½ÓÊÕ»º³åÇø´óÐ¡ */
	g_tUart5.usTxWrite = 0;						/* ·¢ËÍFIFOÐ´Ë÷Òý */
	g_tUart5.usTxRead = 0;						/* ·¢ËÍFIFO¶ÁË÷Òý */
	g_tUart5.usRxWrite = 0;						/* ½ÓÊÕFIFOÐ´Ë÷Òý */
	g_tUart5.usRxRead = 0;						/* ½ÓÊÕFIFO¶ÁË÷Òý */
	g_tUart5.usRxCount = 0;						/* ½ÓÊÕµ½µÄÐÂÊý¾Ý¸öÊý */
	g_tUart5.usTxCount = 0;						/* ´ý·¢ËÍµÄÊý¾Ý¸öÊý */
	g_tUart5.SendBefor = 0;						/* ·¢ËÍÊý¾ÝÇ°µÄ»Øµ÷º¯Êý */
	g_tUart5.SendOver = 0;						/* ·¢ËÍÍê±ÏºóµÄ»Øµ÷º¯Êý */
	g_tUart5.ReciveNew = 0;						/* ½ÓÊÕµ½ÐÂÊý¾ÝºóµÄ»Øµ÷º¯Êý */
#endif


#if UART6_FIFO_EN == 1
	g_tUart6.uart = USART6;						/* STM32 ´®¿ÚÉè±¸ */
	g_tUart6.pTxBuf = g_TxBuf6;					/* ·¢ËÍ»º³åÇøÖ¸Õë */
	g_tUart6.pRxBuf = g_RxBuf6;					/* ½ÓÊÕ»º³åÇøÖ¸Õë */
	g_tUart6.usTxBufSize = UART6_TX_BUF_SIZE;	/* ·¢ËÍ»º³åÇø´óÐ¡ */
	g_tUart6.usRxBufSize = UART6_RX_BUF_SIZE;	/* ½ÓÊÕ»º³åÇø´óÐ¡ */
	g_tUart6.usTxWrite = 0;						/* ·¢ËÍFIFOÐ´Ë÷Òý */
	g_tUart6.usTxRead = 0;						/* ·¢ËÍFIFO¶ÁË÷Òý */
	g_tUart6.usRxWrite = 0;						/* ½ÓÊÕFIFOÐ´Ë÷Òý */
	g_tUart6.usRxRead = 0;						/* ½ÓÊÕFIFO¶ÁË÷Òý */
	g_tUart6.usRxCount = 0;						/* ½ÓÊÕµ½µÄÐÂÊý¾Ý¸öÊý */
	g_tUart6.usTxCount = 0;						/* ´ý·¢ËÍµÄÊý¾Ý¸öÊý */
	g_tUart6.SendBefor = 0;						/* ·¢ËÍÊý¾ÝÇ°µÄ»Øµ÷º¯Êý */
	g_tUart6.SendOver = 0;						/* ·¢ËÍÍê±ÏºóµÄ»Øµ÷º¯Êý */
	g_tUart6.ReciveNew = 0;						/* ½ÓÊÕµ½ÐÂÊý¾ÝºóµÄ»Øµ÷º¯Êý */
#endif
}

/*
*********************************************************************************************************
*	Func name: InitHardUart
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//UART_HandleTypeDef USART_InitStructure1;
	//UART_HandleTypeDef USART_InitStructure2;
	//UART_HandleTypeDef USART_InitStructure3;
	//UART_HandleTypeDef USART_InitStructure6;

#if UART1_FIFO_EN == 1		/* TX = PA9   RX = PA10  TX = PB6   RX = PB7*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
#if	1
	// TX
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	// RX
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
	USART_InitStructure1.Instance = USART1;
	USART_InitStructure1.Init.BaudRate = UART1_BAUD;
	USART_InitStructure1.Init.WordLength = UART_WORDLENGTH_8B;
	USART_InitStructure1.Init.StopBits = UART_STOPBITS_1;
	USART_InitStructure1.Init.Parity = UART_PARITY_NONE;
	USART_InitStructure1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART_InitStructure1.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&USART_InitStructure1);

	__HAL_UART_ENABLE_IT(&USART_InitStructure1,UART_IT_RXNE);
	__HAL_UART_ENABLE(&USART_InitStructure1);
	__HAL_UART_CLEAR_FLAG(&USART_InitStructure1, UART_FLAG_TC);
#endif

#if UART2_FIFO_EN == 1		/* TX = PD5   RX = PD6 TX = PA2 RX = PA3  */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	// TX
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
	// RX
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure2.Instance = USART2;
	USART_InitStructure2.Init.BaudRate = UART2_BAUD;
	USART_InitStructure2.Init.WordLength = UART_WORDLENGTH_8B;
	USART_InitStructure2.Init.StopBits = UART_STOPBITS_1;
	USART_InitStructure2.Init.Parity = UART_PARITY_NONE;
	USART_InitStructure2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART_InitStructure2.Init.Mode = UART_MODE_RX;
	HAL_UART_Init(&USART_InitStructure2);

	__HAL_UART_ENABLE_IT(&USART_InitStructure2,UART_IT_RXNE);
	__HAL_UART_ENABLE(&USART_InitStructure2);
	__HAL_UART_CLEAR_FLAG(&USART_InitStructure2, UART_FLAG_TC);
#endif
#if 0
#if UART3_FIFO_EN == 1
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_USART3_CLK_ENABLE();

		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStructure.Pin = PIN_RS485_TXEN;
		HAL_GPIO_Init(PORT_RS485_TXEN, &GPIO_InitStructure);
	}
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();

	// USART3 TX
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	// USART3 RX
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure3.Instance = USART3;
	USART_InitStructure3.Init.BaudRate = UART3_BAUD;
	USART_InitStructure3.Init.WordLength = UART_WORDLENGTH_8B;
	USART_InitStructure3.Init.StopBits = UART_STOPBITS_1;
	USART_InitStructure3.Init.Parity = UART_PARITY_NONE;
	USART_InitStructure3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART_InitStructure3.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&USART_InitStructure3);

	__HAL_UART_ENABLE_IT(&USART_InitStructure3,UART_IT_RXNE);
	__HAL_UART_ENABLE(&USART_InitStructure3);
	__HAL_UART_CLEAR_FLAG(&USART_InitStructure3, UART_FLAG_TC);
#endif

#if UART6_FIFO_EN == 1
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_USART6_CLK_ENABLE();

	/* PG14/USART6_TX */
	GPIO_InitStructure.Pin = GPIO_PIN_14;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* PC7/USART6_RX */
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* PG8/USART6_RTS */
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* PG15/USART6_CTS */
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	USART_InitStructure6.Instance = USART6;
	USART_InitStructure6.Init.BaudRate = UART6_BAUD;
	USART_InitStructure6.Init.WordLength = UART_WORDLENGTH_8B;
	USART_InitStructure6.Init.StopBits = UART_STOPBITS_1;
	USART_InitStructure6.Init.Parity = UART_PARITY_NONE;
	USART_InitStructure6.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
	USART_InitStructure6.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&USART_InitStructure6);

	__HAL_UART_ENABLE_IT(&USART_InitStructure6,UART_IT_RXNE);
	__HAL_UART_ENABLE(&USART_InitStructure6);
	__HAL_UART_CLEAR_FLAG(&USART_InitStructure6, UART_FLAG_TC);
#endif
#endif
}

static void ConfigUartNVIC(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

#if UART1_FIFO_EN == 1
	  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(USART1_IRQn);
#endif

#if UART2_FIFO_EN == 1
	  HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	  HAL_NVIC_EnableIRQ(USART2_IRQn);
#endif

#if UART3_FIFO_EN == 1
	  HAL_NVIC_SetPriority(USART3_IRQn, 0, 2);
	  HAL_NVIC_EnableIRQ(USART3_IRQn);
#endif

#if UART4_FIFO_EN == 1
	  HAL_NVIC_SetPriority(UART4_IRQn, 0, 3);
	  HAL_NVIC_EnableIRQ(UART4_IRQn);
#endif

#if UART5_FIFO_EN == 1
	  HAL_NVIC_SetPriority(UART5_IRQn, 0, 4);
	  HAL_NVIC_EnableIRQ(UART5_IRQn);
#endif

#if UART6_FIFO_EN == 1
	  HAL_NVIC_SetPriority(USART6_IRQn, 0, 5);
	  HAL_NVIC_EnableIRQ(USART6_IRQn);
#endif
}

/**
  * @brief  UART1을 통해 데이터를 송신하는 함수
  * @param  data: 송신할 데이터 버퍼의 포인터
  * @param  size: 송신할 데이터 크기
  * @retval HAL 상태 (HAL_OK, HAL_ERROR, HAL_BUSY, HAL_TIMEOUT)
 */
HAL_StatusTypeDef UART1_Transmit(uint8_t *data, uint16_t size)
{
  HAL_StatusTypeDef status;

  // 폴링 방식으로 데이터 송신
  status = HAL_UART_Transmit(&USART_InitStructure1, data, size, 1000); // 타임아웃 1000ms

  // 송신 완료 플래그 클리어
  __HAL_UART_CLEAR_FLAG(&USART_InitStructure1, UART_FLAG_TC);

  return status;
}

/**
  * @brief  UART1을 통해 데이터를 비동기적으로 송신하는 함수
  * @param  data: 송신할 데이터 버퍼의 포인터
  * @param  size: 송신할 데이터 크기
  * @retval HAL 상태 (HAL_OK, HAL_ERROR, HAL_BUSY, HAL_TIMEOUT)
*/
HAL_StatusTypeDef UART1_Transmit_IT(uint8_t *data, uint16_t size)
{
  HAL_StatusTypeDef status;

  // 인터럽트 방식으로 데이터 송신
  status = HAL_UART_Transmit_IT(&USART_InitStructure1, data, size);

  return status;
}

/**
  * @brief  UART1을 통해 문자열을 송신하는 함수
  * @param  str: 송신할 문자열
  * @retval HAL 상태 (HAL_OK, HAL_ERROR, HAL_BUSY, HAL_TIMEOUT)
  */
HAL_StatusTypeDef UART1_Transmit_String(char *str)
{
  return UART1_Transmit((uint8_t*)str, strlen(str));
}

















