/*
 * bsp_i2c_gpio.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "bsp.h"

// HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SDA_PIN, GPIO_PIN_SET)

#define GPIO_PORT_I2C	GPIOH
#define I2C_SCL_PIN		GPIO_PIN_4
#define I2C_SDA_PIN		GPIO_PIN_5

#define I2C_SCL_1()  HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SCL_PIN, GPIO_PIN_SET)				/* SCL = 1 */
#define I2C_SCL_0()  HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SCL_PIN, GPIO_PIN_RESET)				/* SCL = 0 */

#define I2C_SDA_1()  HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SDA_PIN, GPIO_PIN_SET)				/* SDA = 1 */
#define I2C_SDA_0()  HAL_GPIO_WritePin(GPIO_PORT_I2C, I2C_SDA_PIN, GPIO_PIN_RESET)				/* SDA = 0 */

#define I2C_SDA_READ()  (HAL_GPIO_ReadPin(GPIO_PORT_I2C, I2C_SDA_PIN) == GPIO_PIN_SET)  /* Read SDA pin state */
#define I2C_SCL_READ()  (HAL_GPIO_ReadPin(GPIO_PORT_I2C, I2C_SCL_PIN) == GPIO_PIN_SET)  /* Read SCL pin state */


/*
*********************************************************************************************************
*	Func name: i2c_Delay
*********************************************************************************************************
*/
void i2c_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 30; i++);
}

/*
*********************************************************************************************************
*	Func name: i2c_Start
*********************************************************************************************************
*/
void i2c_Start(void)
{
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	Func name: i2c_SendByte
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // ÊÍ·Å×ÜÏß
		}
		_ucByte <<= 1;	/* ×óÒÆÒ»¸öbit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	Func name: i2c_ReadByte
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	value = 0;

	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	Func name: i2c_WaitAck
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPUÊÍ·ÅSDA×ÜÏß */
	i2c_Delay();
	I2C_SCL_1();	/* CPUÇý¶¯SCL = 1, ´ËÊ±Æ÷¼þ»á·µ»ØACKÓ¦´ð */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU¶ÁÈ¡SDA¿ÚÏß×´Ì¬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	Func name: i2c_Ack
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPUÇý¶¯SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU²úÉú1¸öÊ±ÖÓ */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPUÊÍ·ÅSDA×ÜÏß */
}

/*
*********************************************************************************************************
*	Func name: i2c_NAck
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPUÇý¶¯SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU²úÉú1¸öÊ±ÖÓ */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}


/*
*********************************************************************************************************
*	Func name: i2c_Start
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	Func name: bsp_InitI2C
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOH_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStructure.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	HAL_GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	i2c_Stop();
}
