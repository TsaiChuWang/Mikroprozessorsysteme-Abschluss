/******************************************************************************
 * @file     main.c
 * @version  V2.00
 * $Revision: 3 $
 * $Date: 15/04/20 2:56p $
 * @brief
 *           Implement SPI Master loop back transfer.
 *           This sample code needs to connect SPI2_MISO0 pin and SPI2_MOSI0 pin together.
 *           It will compare the received data with transmitted data.
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "NUC100Series.h"

#define PLL_CLOCK           50000000

#define TRUE 1

// Reference : https://www.analog.com/media/en/technical-documentation/data-sheets/adxl345.pdf
#define OFSX        0x1E   /* X-axis offset */
#define OFSY        0x1F   /* Y-axis offset */
#define OFSZ        0x20   /* Z-axis offset */
#define BW_RATE     0x2C   /* Data rate and power mode control */
#define POWER_CTL   0x2D   /* Power-saving features control */
#define INT_ENABLE  0x2E   /* Interrupt enable control */
#define DATA_FORMAT 0x31   /* Data format control */
#define DATAX0      0x32   /* X-Axis Data 0 */
#define DATAX1		  0x33   /* X-Axis Data 1 */
#define DATAY0		  0x34   /* Y-Axis Data 0 */
#define DATAY1      0x35   /* Y-Axis Data 1 */
#define DATAZ0      0x36   /* Z-Axis Data 0 */
#define DATAZ1      0x37   /* Z-Axis Data 1 */ 
#define FIFO_CTL    0x38   /* FIFO control */

/* Function prototype declaration */
void SYS_Init(void);  /* Initialization of the system , change the setting of SPI */
void SPI_Init(void);  /* SPI initialization */
void SPI_write(SPI_T * spi, uint8_t slav_addr, uint8_t data); /* Write data to specified address */
uint8_t SPI_read(SPI_T *spi, uint8_t func_addr); /* Read data from specified address */
float AXIS_DATA_READ(int tag);  /* Return the ADXL value with parpmeter */

/* ------------- */
/* Main function */
/* ------------- */
int main(void)
{
     /* Unlock protected registers */
    SYS_UnlockReg();
    /* Init system, peripheral clock and multi-function I/O */
    SYS_Init();
    /* Lock protected registers */
    SYS_LockReg();
    /* Configure UART0: 115200, 8-bit word, no parity bit, 1 stop bit. */
    UART_Open(UART0, 115200);
    /* Init SPI */
    SPI_Init();
	  printf("Start obtaining data from ADXL345 \n");
		
   SPI_write(SPI2, DATA_FORMAT, 0x0B); //set to full resolution mode and Set the range of G +-16G
   SPI_write(SPI2, BW_RATE, 0x0F); //set bandwidth and output data speed
   SPI_write(SPI2, POWER_CTL, 0x08); //set ADXL345 to measurement mode
   SPI_write(SPI2, INT_ENABLE, 0x80); //set D7 DATA_READY is 1
	 SPI_write(SPI2, FIFO_CTL, 0x80); //set FIFO mode is stream
   SPI_write(SPI2, OFSX, 0x05);	
   SPI_write(SPI2, OFSY, 0x05);
   SPI_write(SPI2, OFSZ, 0x05);
	 
	 printf("check 0x00 : %x\n",SPI_read(SPI2,0x00)); 
	 
    while(TRUE)
    {		
        printf("X: %8.2f  g   Y: %8.2f g    Z: %8.2f g ck:%x\n", AXIS_DATA_READ(0), AXIS_DATA_READ(1), AXIS_DATA_READ(2),SPI_read(SPI2,0x00));
        CLK_SysTickDelay(500000ul);
    }

}

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC 22.1184 MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);
    
    /* Enable external 12 MHz XTAL */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock rate as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Select HXT as the clock source of UART0 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

    /* Select HCLK as the clock source of SPI0 */
    CLK_SetModuleClock(SPI2_MODULE, CLK_CLKSEL1_SPI2_S_HCLK, MODULE_NoMsk);

    /* Enable UART peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);
    /* Enable SPI0 peripheral clock */
    CLK_EnableModuleClock(SPI2_MODULE);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CyclesPerUs automatically. */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PB multi-function pins for UART0 RXD and TXD */
     /* Set PB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFP = SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD;

    /* Setup SPI2 multi-function pins */
    SYS->GPD_MFP = SYS_GPD_MFP_PD0_SPI2_SS0 | SYS_GPD_MFP_PD1_SPI2_CLK | SYS_GPD_MFP_PD2_SPI2_MISO0 | SYS_GPD_MFP_PD3_SPI2_MOSI0;
    SYS->ALT_MFP = SYS_ALT_MFP_PD0_SPI2_SS0 | SYS_ALT_MFP_PD1_SPI2_CLK | SYS_ALT_MFP_PD2_SPI2_MISO0 | SYS_ALT_MFP_PD3_SPI2_MOSI0;
}

void SPI_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init SPI      :  .\NUC100_120BSP_v3.00.002\NUC100_120BSPv3.00.002\Library\StdDriver\src                                                                                         */
    /*---------------------------------------------------------------------------------------------------------*/
    SPI_Open(SPI2, SPI_MASTER, SPI_MODE_3, 8, 1000000);
    /*---------------------------------------------------------------------------------------------------------*/
    /* SPI2 : The pointer of the specified SPI module. */
    /* SPI_MASTER : u32MasterSlave Decides the SPI module is operating in master mode */
    /* SPI_MODE_3 : Decides the transfer timing, The clock line (SCK) rests at a high state when idle. */
    /* 8 : Decides the data width of a SPI transaction, 8-bit transaction */
    /* 1000000 :  SPI clock rate = 1 MHz */
    /*---------------------------------------------------------------------------------------------------------*/
}
void SPI_write(SPI_T *spi, uint8_t func_addr, uint8_t data){
    SPI_WRITE_TX0(spi, func_addr);  /* Write to TX register */
    SPI_SET_SS0_LOW(spi); /* set cs low : Disable automatic slave selection function and set SPIn_SS0 pin to low state. Only available in Master mode. */
    SPI_TRIGGER(spi); /* Trigger SPI data transfer : SPI controller will trigger the data transfer automatically after user write to SPI_TX0/1 register.*/
    while(SPI_IS_BUSY(spi));  /* Check SPI0 busy status */
    SPI_WRITE_TX0(spi, data); 
    SPI_TRIGGER(spi);
    while(SPI_IS_BUSY(spi));
    SPI_SET_SS0_HIGH(spi);  /* Disable automatic slave selection function and set SPIn_SS1 pin to high state. Only available in Master mode */
}
uint8_t SPI_read(SPI_T *spi, uint8_t func_addr){
    uint8_t data; /* Data from specified address */
    SPI_WRITE_TX0(spi, 0x80 | func_addr);
    //*set cs low */
    SPI_SET_SS0_LOW(spi);
    /* Trigger SPI data transfer */
    SPI_TRIGGER(spi);
  	/* Check SPI0 busy status */
    while(SPI_IS_BUSY(spi));

		/*wait data recived*/
    SPI_TRIGGER(spi);
  	/* Check SPI0 busy status */
    while(SPI_IS_BUSY(spi));
    /* Read received data */
		data = (uint8_t)SPI_READ_RX0(spi); 
    //*set cs high */
    SPI_SET_SS0_HIGH(spi);                   
		return data;
}
float AXIS_DATA_READ(int tag){
	uint8_t d0, d1;
	int8_t offset;
	int16_t output;
	switch(tag){
		case 0:
			d0 = SPI_read(SPI2,DATAX0);
			d1 = SPI_read(SPI2,DATAX1);
		  offset = SPI_read(SPI2,OFSX);
		  output = (d1<<8) | d0;
		break;
		case 1:
			d0 = SPI_read(SPI2,DATAY0);
			d1 = SPI_read(SPI2,DATAY1);
		  offset = SPI_read(SPI2,OFSY);
		  output = (d1<<8) | d0;
		break;
		case 2:
			d0 = SPI_read(SPI2,DATAZ0);
			d1 = SPI_read(SPI2,DATAZ1);
		  offset = SPI_read(SPI2,OFSZ);
		  output = (d1<<8) | d0;
		break;
	}
	return ((output+offset)/(256.0+offset));
}
/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
