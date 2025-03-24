/*
 * bsp_mpu6050.h
 *
 *  Created on: Mar 24, 2025
 *      Author: compro
 */

#ifndef BSP_INC_BSP_MPU6050_H_
#define BSP_INC_BSP_MPU6050_H_

#define MPU6050_SLAVE_ADDRESS    0xD0		/* I2C´Ó»úµØÖ· */

//****************************************
// ¶¨ÒåMPU6050ÄÚ²¿µØÖ·
//****************************************
#define	SMPLRT_DIV		0x19	//ÍÓÂÝÒÇ²ÉÑùÂÊ£¬µäÐÍÖµ£º0x07(125Hz)
#define	CONFIG			0x1A	//µÍÍ¨ÂË²¨ÆµÂÊ£¬µäÐÍÖµ£º0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//ÍÓÂÝÒÇ×Ô¼ì¼°²âÁ¿·¶Î§£¬µäÐÍÖµ£º0x18(²»×Ô¼ì£¬2000deg/s)

#define	ACCEL_CONFIG	0x1C	//¼ÓËÙ¼Æ×Ô¼ì¡¢²âÁ¿·¶Î§¼°¸ßÍ¨ÂË²¨ÆµÂÊ£¬µäÐÍÖµ£º0x01(²»×Ô¼ì£¬2G£¬5Hz)

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//µçÔ´¹ÜÀí£¬µäÐÍÖµ£º0x00(Õý³£ÆôÓÃ)
#define	WHO_AM_I		0x75	//IICµØÖ·¼Ä´æÆ÷(Ä¬ÈÏÊýÖµ0x68£¬Ö»¶Á)

typedef struct
{
	int16_t Accel_X;
	int16_t Accel_Y;
	int16_t Accel_Z;

	int16_t Temp;

	int16_t GYRO_X;
	int16_t GYRO_Y;
	int16_t GYRO_Z;
}MPU6050_T;

extern MPU6050_T g_tMPU6050;

void bsp_InitMPU6050(void);
void MPU6050_WriteByte(uint8_t _ucRegAddr, uint8_t _ucRegData);
uint8_t MPU6050_ReadByte(uint8_t _ucRegAddr);

void MPU6050_ReadData(void);

#endif /* BSP_INC_BSP_MPU6050_H_ */
