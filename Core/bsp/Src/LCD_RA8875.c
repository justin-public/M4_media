/*
 * LCD_RA8875.c
 *
 *  Created on: Mar 13, 2025
 *      Author: compro
 */
#include "bsp.h"
#include "bsp_ra8875_port.h"

static __IO uint16_t s_usTouchX, s_usTouchY;	/* ´¥ÃþÆÁADCÖµ */
static __IO uint8_t s_ucRA8875Busy = 0;			/* Ã¦±êÖ¾, ÆäËûÄ£¿éÍ¨¹ý RA8875_IsBusy()º¯ÊýÅÐ¶ÏÃ¦ */

/* ±£´æµ±Ç°ÏÔÊ¾´°¿ÚµÄÎ»ÖÃºÍ´óÐ¡£¬Õâ¼¸¸ö±äÁ¿ÓÉ RA8875_SetDispWin() ½øÐÐÉèÖÃ */
static uint16_t s_WinX = 0;
static uint16_t s_WinY = 0;
static uint16_t s_WinHeight = 272;
static uint16_t s_WinWidth = 480;

static void RA8875_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue);
static uint8_t RA8875_ReadReg(uint8_t _ucRegAddr);

static void RA8875_SetCursor(uint16_t _usX, uint16_t _usY);
static void RA8875_SetReadCursor(uint16_t _usX, uint16_t _usY);
static void RA8875_SetTextCursor(uint16_t _usX, uint16_t _usY);

static void BTE_SetTarBlock(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t _ucLayer);
static void BTE_SetOperateCode(uint8_t _ucOperate);
static void BTE_Start(void);
static void BTE_Wait(void);

/*
*********************************************************************************************************
*	Func name: RA8875_ReadID
*********************************************************************************************************
*/
uint16_t RA8875_ReadID(void)
{
	return RA8875_ReadReg(0x00);
}

/*
*********************************************************************************************************
*	Func name: RA8875_WriteReg
*********************************************************************************************************
*/
static uint8_t RA8875_ReadReg(uint8_t _ucRegAddr)
{
	RA8875_WriteCmd(_ucRegAddr);
	return RA8875_ReadData();
}

/*
*********************************************************************************************************
*	Func name: RA8875_WriteReg
*********************************************************************************************************
*/
static void RA8875_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue)
{
	s_ucRA8875Busy = 1;

	RA8875_WriteCmd(_ucRegAddr);
	RA8875_WriteData(_ucRegValue);

	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	Func name: RA8875_InitHard
*********************************************************************************************************
*/
void RA8875_InitHard(void)
{
	uint8_t ucGPIX;

#ifdef IF_SPI_EN	   /* ËÄÏßSPI½Ó¿ÚÄ£Ê½ */
	RA8875_InitSPI();
#endif

	/* ¶ÁÈ¡ RA8875 Ð¾Æ¬¶îGPIXÒý½ÅµÄµçÆ½×´Ì¬£»1±íÊ¾4.3´çÆÁ£»0±íÊ¾7´çÆÁ
	¡¾±¸×¢¡¿ÕâÊÇ°²¸»À³µç×ÓÎªÁË±ãÓÚÍ³Ò»²âÊÔÀý³Ì£¬ÔÚLCDÄ£¿éÉÏ×öµÄÓ²¼þ±êÊ¶¡£ÔÚ×ö²úÆ·Ê±£¬²»±Ø×Ô¶¯Ê¶±ð¡£
	*/
	ucGPIX = RA8875_ReadReg(0xC7);

	if (ucGPIX == 1)	/* 	GPIX = 1 ±íÊ¾ 4.3 ´çÆÁ 480x272 */
	{
		/* ³õÊ¼»¯PLL.  ¾§ÕñÆµÂÊÎª25M */
		RA8875_WriteCmd(0x88);
		RA8875_Delaly1us();		/* ÑÓ³Ù1us */
		RA8875_WriteData(10);	/* PLLDIVM [7] = 0 ;  PLLDIVN [4:0] = 10 */

	    RA8875_Delaly1ms();

		RA8875_WriteCmd(0x89);
		RA8875_Delaly1us();		/* ÑÓ³Ù1us */
		RA8875_WriteData(2);		/* PLLDIVK[2:0] = 2, ³ýÒÔ4 */

		/* RA8875 µÄÄÚ²¿ÏµÍ³ÆµÂÊ (SYS_CLK) ÊÇ½áºÏÕñµ´µçÂ·¼°PLL µçÂ·Ëù²úÉú£¬ÆµÂÊ¼ÆËã¹«Ê½ÈçÏÂ :
		  SYS_CLK = FIN * ( PLLDIVN [4:0] +1 ) / ((PLLDIVM+1 ) * ( 2^PLLDIVK [2:0] ))
		          = 25M * (10 + 1) / ((0 + 1) * (2 ^ 2))
				  = 68.75MHz
		*/

		/* REG[88h]»òREG[89h]±»Éè¶¨ºó£¬Îª±£Ö¤PLL Êä³öÎÈ¶¨£¬ÐëµÈ´ýÒ»¶Î¡¸ËøÆµÊ±¼ä¡¹(< 100us)¡£*/
	    RA8875_Delaly1ms();

		/*
			ÅäÖÃÏµÍ³¿ØÖÆ¼Ä´æÆ÷¡£ ÖÐÎÄpdf µÚ18Ò³:

			bit3:2 É«²ÊÉî¶ÈÉè¶¨ (Color Depth Setting)
				00b : 8-bpp µÄÍ¨ÓÃTFT ½Ó¿Ú£¬ i.e. 256 É«¡£
				1xb : 16-bpp µÄÍ¨ÓÃTFT ½Ó¿Ú£¬ i.e. 65K É«¡£	 ¡¾Ñ¡Õâ¸ö¡¿

			bit1:0 MCUIF Ñ¡Ôñ
				00b : 8-Î»MCU ½Ó¿Ú¡£
				1xb : 16-Î»MCU ½Ó¿Ú¡£ ¡¾Ñ¡Õâ¸ö¡¿
		*/
		#ifdef IF_SPI_EN
			RA8875_WriteReg(0x10, (1 <<3 ) | (0 << 1));	/* SPI½Ó¿ÚÊ±£¬ÅäÖÃ8Î»£¬65KÉ« */
		#else
			RA8875_WriteReg(0x10, (1 <<3 ) | (1 << 1));	/* ÅäÖÃ16Î»MCU²¢¿Ú£¬65KÉ« */
		#endif

		/* REG[04h] Pixel Clock Setting Register   PCSR
			bit7  PCLK Inversion
				0 : PDAT ÊÇÔÚPCLK ÕýÔµÉÏÉý (Rising Edge) Ê±±»È¡Ñù¡£
				1 : PDAT ÊÇÔÚPCLK ¸ºÔµÏÂ½µ (Falling Edge) Ê±±»È¡Ñù¡£
			bit1:0 PCLK ÆµÂÊÖÜÆÚÉè¶¨
				Pixel Clock  PCLK ÆµÂÊÖÜÆÚÉè¶¨¡£
				00b: PCLK ÆµÂÊÖÜÆÚ= ÏµÍ³ÆµÂÊÖÜÆÚ¡£
				01b: PCLK ÆµÂÊÖÜÆÚ= 2 ±¶µÄÏµÍ³ÆµÂÊÖÜÆÚ¡£
				10b: PCLK ÆµÂÊÖÜÆÚ= 4 ±¶µÄÏµÍ³ÆµÂÊÖÜÆÚ¡£
				11b: PCLK ÆµÂÊÖÜÆÚ= 8 ±¶µÄÏµÍ³ÆµÂÊÖÜÆÚ¡£
		*/
	    RA8875_WriteReg(0x04, 0x82);    /* ÉèÖÃPCLK·´Ïà */
	    RA8875_Delaly1ms();

	    //Horizontal set
	    //HDWR//Horizontal Display Width Setting Bit[6:0]
	 	//Horizontal display width(pixels) = (HDWR + 1)*8
	    RA8875_WriteReg(0x14, 0x3B);
	    RA8875_WriteReg(0x15, 0x00);

	    //HNDR//Horizontal Non-Display Period Bit[4:0]
	    //Horizontal Non-Display Period (pixels) = (HNDR + 1)*8
		RA8875_WriteReg(0x16, 0x01);

	    //HSTR//HSYNC Start Position[4:0]
	    //HSYNC Start Position(PCLK) = (HSTR + 1)*8
		RA8875_WriteReg(0x17, 0x00);

	    //HPWR//HSYNC Polarity ,The period width of HSYNC.
	    //HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8
		RA8875_WriteReg(0x18, 0x05);

	    //Vertical set
	    //VDHR0 //Vertical Display Height Bit [7:0]
	    //Vertical pixels = VDHR + 1
		RA8875_WriteReg(0x19, 0x0F);

	    //VDHR1 //Vertical Display Height Bit [8]
	    //Vertical pixels = VDHR + 1
		RA8875_WriteReg(0x1A, 0x01);

	    //VNDR0 //Vertical Non-Display Period Bit [7:0]
	    //Vertical Non-Display area = (VNDR + 1)
		RA8875_WriteReg(0x1B, 0x02);

	    //VNDR1 //Vertical Non-Display Period Bit [8]
	    //Vertical Non-Display area = (VNDR + 1)
		RA8875_WriteReg(0x1C, 0x00);

	    //VSTR0 //VSYNC Start Position[7:0]
	    //VSYNC Start Position(PCLK) = (VSTR + 1)
		RA8875_WriteReg(0x1D, 0x07);

	    //VSTR1 //VSYNC Start Position[8]
	    //VSYNC Start Position(PCLK) = (VSTR + 1)
		RA8875_WriteReg(0x1E, 0x00);

	    //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	    //VSYNC Pulse Width(PCLK) = (VPWR + 1)
	    RA8875_WriteReg(0x1F, 0x09);


	    g_LcdHeight = LCD_43_HEIGHT;
		g_LcdWidth = LCD_43_WIDTH;
	}
	else	/* GPIX = 0 ±íÊ¾7´çÆÁ(800x480) */
	{
	    g_LcdHeight = LCD_70_HEIGHT;
		g_LcdWidth = LCD_70_WIDTH;

		/* ³õÊ¼»¯PLL.  ¾§ÕñÆµÂÊÎª25M */
		RA8875_WriteCmd(0x88);
		RA8875_Delaly1us();		/* ÑÓ³Ù1us */
		RA8875_WriteData(12);	/* PLLDIVM [7] = 0 ;  PLLDIVN [4:0] = 10 */

	    RA8875_Delaly1ms();

		RA8875_WriteCmd(0x89);
		RA8875_Delaly1us();		/* ÑÓ³Ù1us */
		RA8875_WriteData(2);	/* PLLDIVK[2:0] = 2, ³ýÒÔ4 */

		/* RA8875 µÄÄÚ²¿ÏµÍ³ÆµÂÊ (SYS_CLK) ÊÇ½áºÏÕñµ´µçÂ·¼°PLL µçÂ·Ëù²úÉú£¬ÆµÂÊ¼ÆËã¹«Ê½ÈçÏÂ :
		  SYS_CLK = FIN * ( PLLDIVN [4:0] +1 ) / ((PLLDIVM+1 ) * ( 2^PLLDIVK [2:0] ))
		          = 25M * (12 + 1) / ((0 + 1) * (2 ^ 2))
				  = 81.25MHz
		*/

		/* REG[88h]»òREG[89h]±»Éè¶¨ºó£¬Îª±£Ö¤PLL Êä³öÎÈ¶¨£¬ÐëµÈ´ýÒ»¶Î¡¸ËøÆµÊ±¼ä¡¹(< 100us)¡£*/
	    RA8875_Delaly1ms();

		/*
			ÅäÖÃÏµÍ³¿ØÖÆ¼Ä´æÆ÷¡£ ÖÐÎÄpdf µÚ18Ò³:

			bit3:2 É«²ÊÉî¶ÈÉè¶¨ (Color Depth Setting)
				00b : 8-bpp µÄÍ¨ÓÃTFT ½Ó¿Ú£¬ i.e. 256 É«¡£
				1xb : 16-bpp µÄÍ¨ÓÃTFT ½Ó¿Ú£¬ i.e. 65K É«¡£	 ¡¾Ñ¡Õâ¸ö¡¿

			bit1:0 MCUIF Ñ¡Ôñ
				00b : 8-Î»MCU ½Ó¿Ú¡£
				1xb : 16-Î»MCU ½Ó¿Ú¡£ ¡¾Ñ¡Õâ¸ö¡¿
		*/
		#ifdef IF_SPI_EN
			RA8875_WriteReg(0x10, (1 <<3 ) | (0 << 1));	/* SPI½Ó¿ÚÊ±£¬ÅäÖÃ8Î»£¬65KÉ« */
		#else
			RA8875_WriteReg(0x10, (1 <<3 ) | (1 << 1));	/* ÅäÖÃ16Î»MCU²¢¿Ú£¬65KÉ« */
		#endif

		/* REG[04h] Pixel Clock Setting Register (PCSR)
			bit7  PCLK Inversion
				0 : PDAT ÊÇÔÚPCLK ÕýÔµÉÏÉý (Rising Edge) Ê±±»È¡Ñù¡£
				1 : PDAT ÊÇÔÚPCLK ¸ºÔµÏÂ½µ (Falling Edge) Ê±±»È¡Ñù¡£
			bit1:0 PCLK ÆµÂÊÖÜÆÚÉè¶¨
				Pixel Clock ,PCLK ÆµÂÊÖÜÆÚÉè¶¨¡£
				00b: PCLK ÆµÂÊÖÜÆÚ= ÏµÍ³ÆµÂÊÖÜÆÚ¡£
				01b: PCLK ÆµÂÊÖÜÆÚ= 2 ±¶µÄÏµÍ³ÆµÂÊÖÜÆÚ¡£
				10b: PCLK ÆµÂÊÖÜÆÚ= 4 ±¶µÄÏµÍ³ÆµÂÊÖÜÆÚ¡£
				11b: PCLK ÆµÂÊÖÜÆÚ= 8 ±¶µÄÏµÍ³ÆµÂÊÖÜÆÚ¡£
		*/
	    RA8875_WriteReg(0x04, 0x81);
	    RA8875_Delaly1ms();

	#if 1
		/* OTD9960 & OTA7001 ÉèÖÃ */
		RA8875_WriteReg(0x14, 0x63);
		RA8875_WriteReg(0x15, 0x00);
		RA8875_WriteReg(0x16, 0x03);
		RA8875_WriteReg(0x17, 0x03);
		RA8875_WriteReg(0x18, 0x0B);
		RA8875_WriteReg(0x19, 0xDF);
		RA8875_WriteReg(0x1A, 0x01);
		RA8875_WriteReg(0x1B, 0x1F);
		RA8875_WriteReg(0x1C, 0x00);
		RA8875_WriteReg(0x1D, 0x16);
		RA8875_WriteReg(0x1E, 0x00);
		RA8875_WriteReg(0x1F, 0x01);

	#else	/* AT070TN92  setting */
	    //Horizontal set
	    //HDWR//Horizontal Display Width Setting Bit[6:0]
	 	//Horizontal display width(pixels) = (HDWR + 1)*8
	    RA8875_WriteReg(0x14, 0x4F);
	    RA8875_WriteReg(0x15, 0x05);

	    //HNDR//Horizontal Non-Display Period Bit[4:0]
	    //Horizontal Non-Display Period (pixels) = (HNDR + 1)*8
		RA8875_WriteReg(0x16, 0x0F);

	    //HSTR//HSYNC Start Position[4:0]
	    //HSYNC Start Position(PCLK) = (HSTR + 1)*8
		RA8875_WriteReg(0x17, 0x01);

	    //HPWR//HSYNC Polarity ,The period width of HSYNC.
	    //HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8
		RA8875_WriteReg(0x18, 0x00);

	    //Vertical set
	    //VDHR0 //Vertical Display Height Bit [7:0]
	    //Vertical pixels = VDHR + 1
		RA8875_WriteReg(0x19, 0xDF);

	    //VDHR1 //Vertical Display Height Bit [8]
	    //Vertical pixels = VDHR + 1
		RA8875_WriteReg(0x1A, 0x01);

	    //VNDR0 //Vertical Non-Display Period Bit [7:0]
	    //Vertical Non-Display area = (VNDR + 1)
		RA8875_WriteReg(0x1B, 0x0A);

	    //VNDR1 //Vertical Non-Display Period Bit [8]
	    //Vertical Non-Display area = (VNDR + 1)
		RA8875_WriteReg(0x1C, 0x00);

	    //VSTR0 //VSYNC Start Position[7:0]
	    //VSYNC Start Position(PCLK) = (VSTR + 1)
		RA8875_WriteReg(0x1D, 0x0E);

	    //VSTR1 //VSYNC Start Position[8]
	    //VSYNC Start Position(PCLK) = (VSTR + 1)
		RA8875_WriteReg(0x1E, 0x00);

	    //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	    //VSYNC Pulse Width(PCLK) = (VPWR + 1)
	    RA8875_WriteReg(0x1F, 0x01);
	#endif
	}

	/* ÉèÖÃTFTÃæ°åµÄ DISP  Òý½ÅÎª¸ß£¬Ê¹ÄÜÃæ°å. °²¸»À³TFTÄ£¿éµÄDISPÒý½ÅÁ¬½Óµ½RA8875Ð¾Æ¬µÄGP0X½Å */
	RA8875_WriteReg(0xC7, 0x01);	/* DISP = 1 */

	/* LCD ÏÔÊ¾/¹Ø±ÕÑ¶ºÅ (LCD Display on) */
	RA8875_WriteReg(0x01, 0x80);

	/* 	REG[40h] Memory Write Control Register 0 (MWCR0)

		Bit 7	ÏÔÊ¾Ä£Ê½Éè¶¨
			0 : »æÍ¼Ä£Ê½¡£
			1 : ÎÄ×ÖÄ£Ê½¡£

		Bit 6	ÎÄ×ÖÐ´Èë¹â±ê/ÄÚ´æÐ´Èë¹â±êÉè¶¨
			0 : Éè¶¨ÎÄ×Ö/ÄÚ´æÐ´Èë¹â±êÎª²»ÏÔÊ¾¡£
			1 : Éè¶¨ÎÄ×Ö/ÄÚ´æÐ´Èë¹â±êÎªÏÔÊ¾¡£

		Bit 5	ÎÄ×ÖÐ´Èë¹â±ê/ÄÚ´æÐ´Èë¹â±êÉÁË¸Éè¶¨
			0 : ÓÎ±ê²»ÉÁË¸¡£
			1 : ÓÎ±êÉÁË¸¡£

		Bit 4   NA

		Bit 3-2  »æÍ¼Ä£Ê½Ê±µÄÄÚ´æÐ´Èë·½Ïò
			00b : ×ó -> ÓÒ£¬È»ºóÉÏ -> ÏÂ¡£
			01b : ÓÒ -> ×ó£¬È»ºóÉÏ -> ÏÂ¡£
			10b : ÉÏ -> ÏÂ£¬È»ºó×ó -> ÓÒ¡£
			11b : ÏÂ -> ÉÏ£¬È»ºó×ó -> ÓÒ¡£

		Bit 1 	ÄÚ´æÐ´Èë¹â±ê×Ô¶¯Ôö¼Ó¹¦ÄÜÉè¶¨
			0 : µ±ÄÚ´æÐ´ÈëÊ±¹â±êÎ»ÖÃ×Ô¶¯¼ÓÒ»¡£
			1 : µ±ÄÚ´æÐ´ÈëÊ±¹â±êÎ»ÖÃ²»»á×Ô¶¯¼ÓÒ»¡£

		Bit 0 ÄÚ´æ¶ÁÈ¡¹â±ê×Ô¶¯Ôö¼Ó¹¦ÄÜÉè¶¨
			0 : µ±ÄÚ´æ¶ÁÈ¡Ê±¹â±êÎ»ÖÃ×Ô¶¯¼ÓÒ»¡£
			1 : µ±ÄÚ´æ¶ÁÈ¡Ê±¹â±êÎ»ÖÃ²»»á×Ô¶¯¼ÓÒ»¡£
	*/
	RA8875_WriteReg(0x40, 0x00);	/* Ñ¡Ôñ»æÍ¼Ä£Ê½ */


	/* 	REG[41h] Memory Write Control Register1 (MWCR1)
		Ð´ÈëÄ¿µÄÎ»ÖÃ£¬Ñ¡ÔñÍ¼²ã1
	*/
	RA8875_WriteReg(0x41, 0x00);	/* Ñ¡Ôñ»æÍ¼Ä£Ê½, Ä¿µÄÎªCGRAM */

	RA8875_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);

#ifdef IF_SPI_EN
	RA8875_HighSpeedSPI();		/* ÅäÖÃÍê±Ïºó£¬ÇÐ»»SPIµ½¸ßËÙÄ£Ê½ */
#endif
}

/*
*********************************************************************************************************
*	Func name: RA8875_SetDispWin
*********************************************************************************************************
*/
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth)
{

	uint16_t usTemp;

	/* ×ø±êÏµÍ³Ê¾ÒâÍ¼£º £¨ºáÆÁ£©
			 -----------------------------
			|(0,0)                        |
			|     --------->              |
			|         |                   |
			|         |                   |
			|         |                   |
			|         V                   |
			|     --------->              |
			|                    (479,271)|
			 -----------------------------

		×óÉÏ½ÇÊÇ×ø±êÁãµã, É¨Ãè·½Ïò£¬ÏÈ´Ó×óµ½ÓÒ£¬ÔÙ´ÓÉÏµ½ÏÂ¡£

		Èç¹ûÐèÒª×öÊúÆÁ·½Ê½£¬ÄãÐèÒª½øÐÐÎïÀí×ø±êºÍÂß¼­×ø±êµÄ×ª»»
	*/

	RA8875_WriteReg(0x30, _usX);
    RA8875_WriteReg(0x31, _usX >> 8);

	RA8875_WriteReg(0x32, _usY);
    RA8875_WriteReg(0x33, _usY >> 8);

	usTemp = _usWidth + _usX - 1;
	RA8875_WriteReg(0x34, usTemp);
    RA8875_WriteReg(0x35, usTemp >> 8);

	usTemp = _usHeight + _usY - 1;
	RA8875_WriteReg(0x36, usTemp);
    RA8875_WriteReg(0x37, usTemp >> 8);

	RA8875_SetCursor(_usX, _usY);

	/* ±£´æµ±Ç°´°¿ÚÐÅÏ¢£¬Ìá¸ßÒÔºóµ¥É«Ìî³ä²Ù×÷µÄÐ§ÂÊ.
	ÁíÍâÒ»ÖÖ×ö·¨ÊÇÍ¨¹ý¶ÁÈ¡0x30-0x37¼Ä´æÆ÷»ñµÃµ±Ç°´°¿Ú£¬²»¹ýÐ§ÂÊ½ÏµÍ */
	s_WinX = _usX;
	s_WinY = _usY;
	s_WinHeight = _usHeight;
	s_WinWidth = _usWidth;
}

/*
*********************************************************************************************************
*	Funa name: RA8875_SetCursor
*********************************************************************************************************
*/
static void RA8875_SetCursor(uint16_t _usX, uint16_t _usY)
{
	/* ÉèÖÃÄÚ´æÐ´¹â±êµÄ×ø±ê ¡¾×¢Òâ0x80-83 ÊÇ¹â±êÍ¼ÐÎµÄ×ø±ê¡¿ */
	RA8875_WriteReg(0x46, _usX);
	RA8875_WriteReg(0x47, _usX >> 8);
	RA8875_WriteReg(0x48, _usY);
	RA8875_WriteReg(0x49, _usY >> 8);
}


/*
*********************************************************************************************************
*	Func name: RA8875_ClrScr
*********************************************************************************************************
*/
void RA8875_ClrScr(uint16_t _usColor)
{
	BTE_SetTarBlock(s_WinX, s_WinY, s_WinHeight, s_WinWidth, 0);	/* ÉèÖÃBTEÎ»ÖÃºÍ¿í¶È¸ß¶ÈÒÔ¼°Ä¿±êÍ¼²ã£¨0»ò1£© */
	BTE_SetOperateCode(0x0C);		/* Éè¶¨BTE ²Ù×÷ÂëºÍ¹âÕ¤ÔËËãÂë  REG[51h] Bit[3:0] = 0Ch */
	RA8875_SetFrontColor(_usColor);	/* ÉèÖÃBTEÇ°¾°É« */
	BTE_Start();					/* ¿ªÆôBTE ¹¦ÄÜ */
	BTE_Wait();
}

/*
*********************************************************************************************************
*	Func name: RA8875_PutPixel
*********************************************************************************************************
*/
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	s_ucRA8875Busy = 1;

	RA8875_WriteCmd(0x46); RA8875_WriteData(_usX);
	RA8875_WriteCmd(0x47); RA8875_WriteData(_usX >> 8);
	RA8875_WriteCmd(0x48); RA8875_WriteData(_usY);
	RA8875_WriteCmd(0x49); RA8875_WriteData(_usY >> 8);

	RA8875_WriteCmd(0x02); 		/* ÓÃÓÚÉè¶¨RA8875 ½øÈëÄÚ´æ(DDRAM»òCGRAM)¶ÁÈ¡/Ð´ÈëÄ£Ê½ */
	RA8875_WriteData16(_usColor);

	s_ucRA8875Busy = 0;
}


/*
*********************************************************************************************************
*	Func name: BTE_SetTarBlock
*********************************************************************************************************
*/
static void BTE_SetTarBlock(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t _ucLayer)
{
	/* ÉèÖÃÆðµã×ø±ê */
	RA8875_WriteReg(0x58, _usX);
	RA8875_WriteReg(0x59, _usX >> 8);

	RA8875_WriteReg(0x5A, _usY);
	if (_ucLayer == 0)	/* Í¼²ã2 */
	{
		RA8875_WriteReg(0x5B, _usY >> 8);
	}
	else
	{
		RA8875_WriteReg(0x5B, (1 << 7) | (_usY >> 8));	/* Bit7 ±íÊ¾Í¼²ã£¬ 0 Í¼²ã1£» 1 Í¼²ã2*/
	}

	/* ÉèÖÃÇø¿é¿í¶È */
	RA8875_WriteReg(0x5C, _usWidth);
	RA8875_WriteReg(0x5D, _usWidth >> 8);

	/* ÉèÖÃÇø¿é¸ß¶È */
	RA8875_WriteReg(0x5E, _usHeight);
	RA8875_WriteReg(0x5F, _usHeight >> 8);
}


/*
*********************************************************************************************************
*	Func name: BTE_SetOperateCode
*********************************************************************************************************
*/
static void BTE_SetOperateCode(uint8_t _ucOperate)
{
	RA8875_WriteReg(0x51, _ucOperate);
}

/*
*********************************************************************************************************
* Func name: RA8875_SetFrontColor
*********************************************************************************************************
*/
void RA8875_SetFrontColor(uint16_t _usColor)
{
	RA8875_WriteReg(0x63, (_usColor & 0xF800) >> 11);	/* R5  */
	RA8875_WriteReg(0x64, (_usColor & 0x07E0) >> 5);	/* G6 */
	RA8875_WriteReg(0x65, (_usColor & 0x001F));			/* B5 */
}

/*
*********************************************************************************************************
*	Func name: RA8875_DrawBMP
*********************************************************************************************************
*/
void RA8875_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	uint32_t index = 0;
	const uint16_t *p;

	RA8875_SetDispWin(_usX, _usY, _usHeight, _usWidth);

	s_ucRA8875Busy = 1;

	RA8875_WriteCmd(0x02); 		

	p = _ptr;
	for (index = 0; index < _usHeight * _usWidth; index++)
	{
		RA8875_WriteData16(*p++);
	}
	s_ucRA8875Busy = 0;

	RA8875_QuitWinMode();
}

/*
*********************************************************************************************************
*	Funa name: RA8875_QuitWinMode
*********************************************************************************************************
*/
void RA8875_QuitWinMode(void)
{
	RA8875_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);
}

/*
*********************************************************************************************************
*	Func name: RA8875_WriteData16
*********************************************************************************************************
*/
void RA8875_WriteData16(uint16_t _usRGB)
{
#ifdef RA_HARD_SPI	/* Ó²¼þSPI½Ó¿Ú */
	RA8875_CS_0();
	SPI_ShiftByte(SPI_WRITE_DATA);
	SPI_ShiftByte(_usRGB >> 8);
	RA8875_CS_1();

	RA8875_CS_0();
	SPI_ShiftByte(SPI_WRITE_DATA);
	SPI_ShiftByte(_usRGB);
	RA8875_CS_1();
#endif

#ifdef RA_HARD_8080_16
	RA8875_RAM = _usRGB;
#endif	
}


/*
*********************************************************************************************************
*	Func name: BTE_Start
*********************************************************************************************************
*/
static void BTE_Start(void)
{
	s_ucRA8875Busy = 1;
	/* RA8875_WriteReg(0x50, 0x80);  ²»ÄÜÊ¹ÓÃÕâ¸öº¯Êý£¬ÒòÎªÄÚ²¿ÒÑ¾­²Ù×÷ÁË s_ucRA8875Busy ±êÖ¾ */
	RA8875_WriteCmd(0x50);	/* ÉèÖÃ¼Ä´æÆ÷µØÖ· */
	RA8875_WriteData(0x80);	/* Ð´Èë¼Ä´æÆ÷Öµ */
}


/*
*********************************************************************************************************
*	Func name: BTE_Wait
*********************************************************************************************************
*/
static void BTE_Wait(void)
{
	while ((RA8875_ReadStatus() & 0x40) == 0x40);
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: RA8875_SetBackLight
*	¹¦ÄÜËµÃ÷: ÅäÖÃRA8875Ð¾Æ¬µÄPWM1Ïà¹Ø¼Ä´æÆ÷£¬¿ØÖÆLCD±³¹â
*	ÐÎ    ²Î:  _bright ÁÁ¶È£¬0ÊÇÃð£¬255ÊÇ×îÁÁ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void RA8875_SetBackLight(uint8_t _bright)
{
	if (_bright == 0)
	{
		/* ¹Ø±ÕPWM, PWM1Òý½ÅÈ±Ê¡Êä³öLOW  */
		RA8875_WriteReg(0x8A, 0 << 6);
	}
	else if (_bright == BRIGHT_MAX)	/* ×î´óÁÁ¶È */
	{
		/* ¹Ø±ÕPWM, PWM1Òý½ÅÈ±Ê¡Êä³öHIGH */
		RA8875_WriteReg(0x8A, 1 << 6);
	}
	else
	{
		/* Ê¹ÄÜPWM1, ½øÐÐÕ¼¿Õ±Èµ÷½Ú */

		/* 	REG[8Ah] PWM1 Control Register (P1CR)

			Bit7 Âö²¨¿í¶Èµ÷±ä (PWM1) Éè¶¨
				0 : ¹Ø±Õ£¬´Ë×´Ì¬ÏÂ£¬PWM1 Êä³ö×¼Î»ÒÀÕÕ´Ë»º´æÆ÷Bit6 ¾ö¶¨¡£
				1 : ¿ªÆô¡£

			Bit6 PWM1 ¹Ø±ÕÊ±µÄ×¼Î»
				0 : µ±PWM ¹Ø±Õ»òÓÚË¯ÃßÄ£Ê½Ê±£¬PWM1 Êä³öÎª¡±Low¡± ×´Ì¬¡£
				1 : µ±PWM ¹Ø±Õ»òÓÚË¯ÃßÄ£Ê½Ê±£¬PWM1 Êä³öÎª¡±High¡± ×´Ì¬¡£

			Bit5 ±£Áô

			Bit4 PWM1 ¹¦ÄÜÑ¡Ôñ
				0 : PWM1 ¹¦ÄÜ¡£
				1 : PWM1 ¹Ì¶¨Êä³öÒ»ÆµÂÊÎªÍâ²¿¾§ÌåÕñµ´Æ÷Clock (Fin) ÆµÂÊ 1 /16 µÄClock

			Bit3-0  PWM1 µçÂ·µÄÆµÂÊÀ´Ô´Ñ¡ÔñPWM_CLK	¡¾²»ÊÇPWMÊä³öÆµÂÊ¡¿
				0000b : SYS_CLK / 1   , 1000b : SYS_CLK / 256
				0001b : SYS_CLK / 2   , 1001b : SYS_CLK / 512
				0010b : SYS_CLK / 4   , 1010b : SYS_CLK / 1024
				0011b : SYS_CLK / 8   , 1011b : SYS_CLK / 2048
				0100b : SYS_CLK / 16  , 1100b : SYS_CLK / 4096
				0101b : SYS_CLK / 32  , 1101b : SYS_CLK / 8192
				0110b : SYS_CLK / 64  , 1110b : SYS_CLK / 16384
				0111b : SYS_CLK / 128 , 1111b : SYS_CLK / 32768

				¡°SYS_CLK¡± ´ú±íÏµÍ³ÆµÂÊ£¬ ÀýÈçSYS_CLK Îª20MHz£¬ µ±Bit[3:0] =0001b Ê±£¬PWM1 ÆµÂÊÀ´Ô´Îª10MHz¡£
				Êµ¼ÊÊä³öµÄPWMÆµÂÊ»¹ÐèÒª³ýÒÔ 256£¬Ö§³Ö8Î»µÄ·Ö±æÂÊ¡£

				°²¸»À³µÄ4.3´ç(480*272)Ä£¿é£¬SYS_CLK =  68.75MHz
				°²¸»À³µÄ7.0´ç(800*480)Ä£¿é£¬SYS_CLK =  81.25MHz

				ÎªÁË±ÜÃâÒôÆµÔëÉù£¬PWMÆµÂÊ¿ÉÒÔÑ¡Ôñ
				£¨1£© µÍÆµ100Hz
				£¨2£© ¸ßÓÚ 20KHz

				±ÈÈç£¬Bit3-0Îª0011bÊ± SYS_CLK / 8£¬
					4.3´ç Êä³öµÄPWMÆµÂÊÎª (68.75MHz / 8) / 256 = 33.56KHz
					7´ç Êä³öµÄPWMÆµÂÊÎª (81.25MHz / 8) / 256 = 39.67KHz
		*/

		// RA8875_WriteReg(0x8A, (1 << 7) | 3);   5´çºÍ7´çÐÂÆÁ¿ÉÒÔÓÃ 3 £¬¸ßÆµPWM, 4.3´ç²»ÐÐ
		RA8875_WriteReg(0x8A, (1 << 7) | 12);

		/* REG[8Bh] PWM1 Duty Cycle Register (P1DCR) */
		RA8875_WriteReg(0x8B, _bright);
	}
}

/*
*********************************************************************************************************
*	Func name: RA8875_GetPixel
*********************************************************************************************************
*/
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;

	RA8875_WriteReg(0x40, (1 << 0));

	RA8875_SetReadCursor(_usX, _usY);	/* ÉèÖÃ¶ÁÈ¡¹â±êÎ»ÖÃ */

	s_ucRA8875Busy = 1;

	RA8875_WriteCmd(0x02);
	usRGB = RA8875_ReadData16();	/* µÚ1´Î¶ÁÈ¡Êý¾Ý¶ªÆú */
	usRGB = RA8875_ReadData16();
	usRGB = RA8875_ReadData16();

	s_ucRA8875Busy = 0;

	return usRGB;
}

/*
*********************************************************************************************************
*	Func name: RA8875_SetCursor
*********************************************************************************************************
*/
static void RA8875_SetReadCursor(uint16_t _usX, uint16_t _usY)
{
	RA8875_WriteReg(0x4A, _usX);
	RA8875_WriteReg(0x4B, _usX >> 8);
	RA8875_WriteReg(0x4C, _usY);
	RA8875_WriteReg(0x4D, _usY >> 8);
}

/*
*********************************************************************************************************
*	Func name: RA8875_ReadData16
*********************************************************************************************************
*/
uint16_t RA8875_ReadData16(void)
{
#ifdef RA_HARD_SPI	/* Ó²¼þSPI½Ó¿Ú */
	uint16_t value;

	RA8875_CS_0();
	SPI_ShiftByte(SPI_READ_DATA);
	value = SPI_ShiftByte(0x00);
	value <<= 8;
	value += SPI_ShiftByte(0x00);
	RA8875_CS_1();

	return value;
#endif

#ifdef RA_HARD_8080_16			/* 8080Ó²¼þ×ÜÏß */
	uint16_t value;

	value = RA8875_RAM;		/* ¶ÁÈ¡¼Ä´æÆ÷Öµ */

	return value;
#endif
}

/*
*********************************************************************************************************
*	Func name: RA8875_TouchInit
*********************************************************************************************************
*/
void RA8875_TouchInit(void)
{
	// 터치 인터럽트 활성화 (0xF0 레지스터의 비트 2 설정)
	RA8875_WriteReg(0xF0, (1 << 2));

	// TPCR1 설정: 자동 모드, 내부 참조 전압, 디바운스 활성화
	RA8875_WriteReg(0x71, (0 << 6) | (0 << 5) | (1 << 2));

	// TPCR0 설정: 터치 활성화, ADC 샘플링 = 4096, X축 데이터 획득 모드
	RA8875_WriteReg(0x70, (1 << 7) | (3 << 4) | (0 << 3) | (2 << 0));

}

void TOUCH_Poll(void) {
    uint8_t touched;
    uint16_t x, y;

    // 터치 상태 및 X좌표 읽기 (TPCR0 모드가 2이므로 X축 데이터가 저장됨)
    touched = RA8875_ReadReg(0x72) & 0x01;

    char debug_str[50];
    sprintf(debug_str, "Touch: %d\r\n", touched);
    UART1_Transmit_String(debug_str);

    if (touched) {
        // X 좌표 읽기
        uint8_t x_h = RA8875_ReadReg(0x72) & 0x03; // 상위 2비트
        uint8_t x_l = RA8875_ReadReg(0x74);        // 하위 8비트
        x = (x_h << 8) | x_l;

        // Y 좌표를 읽기 위해 모드 변경
        RA8875_WriteReg(0x70, (1 << 7) | (3 << 4) | (0 << 3) | (3 << 0)); // Y축 데이터 획득 모드로 변경

        // Y 좌표 읽기
        uint8_t y_h = RA8875_ReadReg(0x73) & 0x03; // 상위 2비트
        uint8_t y_l = RA8875_ReadReg(0x74);        // 하위 8비트
        y = (y_h << 8) | y_l;

        // 다시 X축 모드로 돌아가기
        RA8875_WriteReg(0x70, (1 << 7) | (3 << 4) | (0 << 3) | (2 << 0));

        char pos_str[50];
        sprintf(pos_str, "X: %d, Y: %d\r\n", x, y);
        UART1_Transmit_String(pos_str);
    }

    char debug_str2[50];
    sprintf(debug_str2, "TPCR: 0x%02X\r\n", RA8875_ReadReg(0x72));
    UART1_Transmit_String(debug_str2);

    //HAL_Delay(100); // 폴링 간격
    bsp_DelayMS(100);
}



/*
*********************************************************************************************************
* Func name: RA8875_DrawRect
*********************************************************************************************************
*/
void RA8875_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	RA8875_WriteReg(0x91, _usX);
	RA8875_WriteReg(0x92, _usX >> 8);
	RA8875_WriteReg(0x93, _usY);
	RA8875_WriteReg(0x94, _usY >> 8);

	/* ÉèÖÃÖÕµã×ø±ê */
	RA8875_WriteReg(0x95, _usX + _usWidth - 1);
	RA8875_WriteReg(0x96, (_usX + _usWidth - 1) >> 8);
	RA8875_WriteReg(0x97, _usY + _usHeight - 1);
	RA8875_WriteReg(0x98, (_usY + _usHeight - 1) >> 8);

	RA8875_SetFrontColor(_usColor);	/* ÉèÖÃÑÕÉ« */

	s_ucRA8875Busy = 1;
	//	RA8875_WriteReg(0x90, (1 << 7) | (0 << 5) | (1 << 4) | (0 << 0));	/* ¿ªÊ¼»­¾ØÐÎ  */
	RA8875_WriteCmd(0x90);
	RA8875_WriteData((1 << 7) | (0 << 5) | (1 << 4) | (0 << 0));
	//	while (RA8875_ReadReg(0x90) & (1 << 7));							/* µÈ´ý½áÊø */
	RA8875_WaitBusy();
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
* Func name: RA8875_WaitBusy
*********************************************************************************************************
*/
void RA8875_WaitBusy(void)
{
	while ((RA8875_ReadStatus() & 0x80) == 0x80);
}

/*
*********************************************************************************************************
*	Func name: RA8875_DrawCircle
*********************************************************************************************************
*/
void RA8875_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	if (_usRadius > 255)
	{
		return;
	}
	RA8875_WriteReg(0x99, _usX);
	RA8875_WriteReg(0x9A, _usX >> 8);
	RA8875_WriteReg(0x9B, _usY);
	RA8875_WriteReg(0x9C, _usY >> 8);

	RA8875_WriteReg(0x9D, _usRadius);	/* ÉèÖÃÔ²µÄ°ë¾¶ */

	RA8875_SetFrontColor(_usColor);	/* ÉèÖÃÑÕÉ« */

	s_ucRA8875Busy = 1;
	//	RA8875_WriteReg(0x90, (1 << 6) | (0 << 5));				/* ¿ªÊ¼»­Ô², ²»Ìî³ä  */
	RA8875_WriteCmd(0x90);
	RA8875_WriteData( (1 << 6) | (0 << 5));
	//	while (RA8875_ReadReg(0x90) & (1 << 6));				/* µÈ´ý½áÊø */
	RA8875_WaitBusy();
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*  Func name: RA8875_IsBusy
*********************************************************************************************************
*/
uint8_t RA8875_IsBusy(void)
{
	if (s_ucRA8875Busy == 0)
	{
		return 0;
	}
	return 1;
}

/*
*********************************************************************************************************
* Func name: RA8875_TouchReadX
*********************************************************************************************************
*/
uint16_t RA8875_TouchReadX(void)
{
	uint16_t usAdc;
	uint8_t ucRegValue;
	uint8_t ucReg74;

	ucRegValue = RA8875_ReadReg(0xF1);
	if (ucRegValue & (1 << 2))
	{
		ucReg74 = RA8875_ReadReg(0x74);
		usAdc = RA8875_ReadReg(0x72);
		usAdc <<= 2;
		usAdc += (ucReg74 & 0x03);

		s_usTouchX = usAdc;

		usAdc = RA8875_ReadReg(0x73);	/* Bit9-2 */
		usAdc <<= 2;
		usAdc += ((ucReg74 & 0x0C) >> 2);

		s_usTouchY = usAdc;
	}
	else
	{
		s_usTouchX = 0;
		s_usTouchY = 0;
	}

	RA8875_WriteCmd(0xF1);
	RA8875_WriteData(1 << 2);

	return s_usTouchX;
}

/*
*********************************************************************************************************
* Func name: RA8875_TouchReadY
*********************************************************************************************************
*/
uint16_t RA8875_TouchReadY(void)
{
	return s_usTouchY;
}

/*
*********************************************************************************************************
* Func name: RA8875_FillRect
*********************************************************************************************************
*/
void RA8875_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	RA8875_WriteReg(0x91, _usX);
	RA8875_WriteReg(0x92, _usX >> 8);
	RA8875_WriteReg(0x93, _usY);
	RA8875_WriteReg(0x94, _usY >> 8);

	RA8875_WriteReg(0x95, _usX + _usWidth - 1);
	RA8875_WriteReg(0x96, (_usX + _usWidth - 1) >> 8);
	RA8875_WriteReg(0x97, _usY + _usHeight - 1);
	RA8875_WriteReg(0x98, (_usY + _usHeight - 1) >> 8);

	RA8875_SetFrontColor(_usColor);	/* ÉèÖÃÑÕÉ« */

	s_ucRA8875Busy = 1;
	//	RA8875_WriteReg(0x90, (1 << 7) | (1 << 5) | (1 << 4) | (0 << 0));	/* ¿ªÊ¼Ìî³ä¾ØÐÎ  */
	RA8875_WriteCmd(0x90);
	RA8875_WriteData((1 << 7) | (1 << 5) | (1 << 4) | (0 << 0));
	//	while (RA8875_ReadReg(0x90) & (1 << 7));							/* µÈ´ý½áÊø */
	RA8875_WaitBusy();
	s_ucRA8875Busy = 0;
}


































































