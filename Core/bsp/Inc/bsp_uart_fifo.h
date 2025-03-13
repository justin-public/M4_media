/*
 * bsp_uart_fifo.h
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_UART_FIFO_H_
#define BSP_INC_BSP_UART_FIFO_H_

/*
	Èç¹ûÐèÒª¸ü¸Ä´®¿Ú¶ÔÓ¦µÄ¹Ü½Å£¬Çë×ÔÐÐÐÞ¸Ä bsp_uart_fifo.cÎÄ¼þÖÐµÄ static void InitHardUart(void)º¯Êý
*/

/* ¶¨ÒåÊ¹ÄÜµÄ´®¿Ú, 0 ±íÊ¾²»Ê¹ÄÜ£¨²»Ôö¼Ó´úÂë´óÐ¡£©£¬ 1±íÊ¾Ê¹ÄÜ */
#ifdef STM32_X3	/* °²¸»À³ STM32-X4 ¿ª·¢°å */
	/* °²¸»À³STM32-X3¿ª·¢°å´®¿Ú·ÖÅä£º
		PA9/USART1_TX	  --- ´òÓ¡µ÷ÊÔ¿Ú
		PA10/USART1_RX
	*/
	#define	UART1_FIFO_EN	1
	#define	UART2_FIFO_EN	0
	#define	UART3_FIFO_EN	0
	#define	UART4_FIFO_EN	0
	#define	UART5_FIFO_EN	0
	#define	UART6_FIFO_EN	0
#else
	/*
		°²¸»À³STM32-V5 ´®¿Ú·ÖÅä£º
		¡¾´®¿Ú1¡¿ RS232 Ð¾Æ¬µÚ1Â·¡£
			PB6/USART1_TX	  --- ´òÓ¡µ÷ÊÔ¿Ú
			PB7/USART1_RX

		¡¾´®¿Ú2¡¿ PA2 ¹Ü½ÅÓÃÓÚÒÔÌ«Íø£» RX¹Ü½ÅÓÃÓÚ½ÓÊÕGPSÐÅºÅ
			PA2/USART2_TX/ETH_MDIO (ÓÃÓÚÒÔÌ«Íø£¬²»×ö´®¿Ú·¢ËÍÓÃ)
			PA3/USART2_RX	;½ÓGPSÄ£¿éÊä³ö

		¡¾´®¿Ú3¡¿ RS485 Í¨ÐÅ - TTL ÌøÏß ºÍ ÅÅÕë
			PB10/USART3_TX
			PB11/USART3_RX

		¡¾´®¿Ú4¡¿ --- ²»×ö´®¿ÚÓÃ¡£
		¡¾´®¿Ú5¡¿ --- ²»×ö´®¿ÚÓÃ¡£

		¡¾´®¿Ú6¡¿--- GPRSÄ£¿é £¨Ó²¼þÁ÷¿Ø£©
			PC6/USART6_TX
			PC7/USART6_RX
			PG8/USART6_RTS
			PG15/USART6_CTS
	*/
	#define	UART1_FIFO_EN	1
	#define	UART2_FIFO_EN	1
	#define	UART3_FIFO_EN	1
	#define	UART4_FIFO_EN	0
	#define	UART5_FIFO_EN	0
	#define	UART6_FIFO_EN	1

	/* RS485Ð¾Æ¬·¢ËÍÊ¹ÄÜGPIO, PB2 */
	#define RCC_RS485_TXEN 	__HAL_RCC_GPIOB_CLK_ENABLE()
	#define PORT_RS485_TXEN  GPIOB
	#define PIN_RS485_TXEN	 GPIO_PIN_2

	#define RS485_RX_EN()	HAL_GPIO_WritePin(PORT_RS485_TXEN, PIN_RS485_TXEN, GPIO_PIN_RESET)
	#define RS485_TX_EN()	HAL_GPIO_WritePin(PORT_RS485_TXEN, PIN_RS485_TXEN, GPIO_PIN_SET)
#endif

/* ¶¨Òå¶Ë¿ÚºÅ */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 »ò  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 »ò PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5	/* USART6, PC6, PC7 */
}COM_PORT_E;

/* ¶¨Òå´®¿Ú²¨ÌØÂÊºÍFIFO»º³åÇø´óÐ¡£¬·ÖÎª·¢ËÍ»º³åÇøºÍ½ÓÊÕ»º³åÇø, Ö§³ÖÈ«Ë«¹¤ */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			115200
	#define UART1_TX_BUF_SIZE	1*1024
	#define UART1_RX_BUF_SIZE	1*1024
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			9600
	#define UART2_TX_BUF_SIZE	1
	#define UART2_RX_BUF_SIZE	2*1024
#endif

#if UART3_FIFO_EN == 1
	#define UART3_BAUD			9600
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif

#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif

#if UART6_FIFO_EN == 1
	#define UART6_BAUD			115200
	#define UART6_TX_BUF_SIZE	1*1024
	#define UART6_RX_BUF_SIZE	1*1024
#endif

/* ´®¿ÚÉè±¸½á¹¹Ìå */
typedef struct
{
	USART_TypeDef *uart;		/* STM32ÄÚ²¿´®¿ÚÉè±¸Ö¸Õë */
	uint8_t *pTxBuf;			/* ·¢ËÍ»º³åÇø */
	uint8_t *pRxBuf;			/* ½ÓÊÕ»º³åÇø */
	uint16_t usTxBufSize;		/* ·¢ËÍ»º³åÇø´óÐ¡ */
	uint16_t usRxBufSize;		/* ½ÓÊÕ»º³åÇø´óÐ¡ */
	uint16_t usTxWrite;			/* ·¢ËÍ»º³åÇøÐ´Ö¸Õë */
	uint16_t usTxRead;			/* ·¢ËÍ»º³åÇø¶ÁÖ¸Õë */
	uint16_t usTxCount;			/* µÈ´ý·¢ËÍµÄÊý¾Ý¸öÊý */

	uint16_t usRxWrite;			/* ½ÓÊÕ»º³åÇøÐ´Ö¸Õë */
	uint16_t usRxRead;			/* ½ÓÊÕ»º³åÇø¶ÁÖ¸Õë */
	uint16_t usRxCount;			/* »¹Î´¶ÁÈ¡µÄÐÂÊý¾Ý¸öÊý */

	void (*SendBefor)(void); 	/* ¿ªÊ¼·¢ËÍÖ®Ç°µÄ»Øµ÷º¯ÊýÖ¸Õë£¨Ö÷ÒªÓÃÓÚRS485ÇÐ»»µ½·¢ËÍÄ£Ê½£© */
	void (*SendOver)(void); 	/* ·¢ËÍÍê±ÏµÄ»Øµ÷º¯ÊýÖ¸Õë£¨Ö÷ÒªÓÃÓÚRS485½«·¢ËÍÄ£Ê½ÇÐ»»Îª½ÓÊÕÄ£Ê½£© */
	void (*ReciveNew)(void);	/* ´®¿ÚÊÕµ½Êý¾ÝµÄ»Øµ÷º¯ÊýÖ¸Õë */
}UART_T;

void bsp_InitUart(void);

void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

void bsp_usart1_tx(char *msg);

HAL_StatusTypeDef UART1_Transmit(uint8_t *data, uint16_t size);
HAL_StatusTypeDef UART1_Transmit_IT(uint8_t *data, uint16_t size);
HAL_StatusTypeDef UART1_Transmit_String(char *str);

#endif /* BSP_INC_BSP_UART_FIFO_H_ */
