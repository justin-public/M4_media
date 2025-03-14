/*
 * bsp_eeprom_24xx.c
 *
 *  Created on: Mar 14, 2025
 *      Author: compro
 */
#include "bsp.h"

/*
*********************************************************************************************************
*	Func name: ee_ReadBytes
*********************************************************************************************************
*/
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;

	i2c_Start();

	i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* ´Ë´¦ÊÇÐ´Ö¸Áî */

	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}
	if (EE_ADDR_BYTES == 1)
	{
		i2c_SendByte((uint8_t)_usAddress);
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROMÆ÷¼þÎÞÓ¦´ð */
		}
	}
	else
	{
		i2c_SendByte(_usAddress >> 8);
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROMÆ÷¼þÎÞÓ¦´ð */
		}
		i2c_SendByte(_usAddress);
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROMÆ÷¼þÎÞÓ¦´ð */
		}
	}
	i2c_Start();
	i2c_SendByte(EE_DEV_ADDR | I2C_RD);

	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROMÆ÷¼þÎÞÓ¦´ð */
	}
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* ¶Á1¸ö×Ö½Ú */

		if (i != _usSize - 1)
		{
			i2c_Ack();
		}
		else
		{
			i2c_NAck();	/* ×îºó1¸ö×Ö½Ú¶ÁÍêºó£¬CPU²úÉúNACKÐÅºÅ(Çý¶¯SDA = 1) */
		}
	}
	i2c_Stop();
	return 1;
cmd_fail:
	i2c_Stop();
	return 0;
}


/*
*********************************************************************************************************
*	Func name: ee_WriteBytes
*********************************************************************************************************
*/
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;

	usAddr = _usAddress;
	for (i = 0; i < _usSize; i++)
	{

		if ((i == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
		{
			i2c_Stop();

			for (m = 0; m < 1000; m++)
			{
				i2c_Start();

				i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* ´Ë´¦ÊÇÐ´Ö¸Áî */

				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROMÆ÷¼þÐ´³¬Ê± */
			}
			if (EE_ADDR_BYTES == 1)
			{
				i2c_SendByte((uint8_t)usAddr);
				if (i2c_WaitAck() != 0)
				{
					goto cmd_fail;
				}
			}
			else
			{
				i2c_SendByte(usAddr >> 8);
				if (i2c_WaitAck() != 0)
				{
					goto cmd_fail;	/* EEPROMÆ÷¼þÎÞÓ¦´ð */
				}
				i2c_SendByte(usAddr);
				if (i2c_WaitAck() != 0)
				{
					goto cmd_fail;	/* EEPROMÆ÷¼þÎÞÓ¦´ð */
				}
			}
		}
		/* µÚ6²½£º¿ªÊ¼Ð´ÈëÊý¾Ý */
		i2c_SendByte(_pWriteBuf[i]);
		/* µÚ7²½£º·¢ËÍACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROMÆ÷¼þÎÞÓ¦´ð */
		}
		usAddr++;	/* µØÖ·Ôö1 */
	}
	/* ÃüÁîÖ´ÐÐ³É¹¦£¬·¢ËÍI2C×ÜÏßÍ£Ö¹ÐÅºÅ */
	i2c_Stop();
	return 1;
cmd_fail: /* ÃüÁîÖ´ÐÐÊ§°Üºó£¬ÇÐ¼Ç·¢ËÍÍ£Ö¹ÐÅºÅ£¬±ÜÃâÓ°ÏìI2C×ÜÏßÉÏÆäËûÉè±¸ */
	/* ·¢ËÍI2C×ÜÏßÍ£Ö¹ÐÅºÅ */
	i2c_Stop();
	return 0;
}
