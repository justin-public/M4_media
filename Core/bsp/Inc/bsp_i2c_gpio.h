/*
 * bsp_i2c_gpio.h
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */

#ifndef BSP_BSP_I2C_GPIO_H_
#define BSP_BSP_I2C_GPIO_H_

#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

void bsp_InitI2C(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Delay(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif /* BSP_BSP_I2C_GPIO_H_ */
