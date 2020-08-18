
//NUC975引脚定义
#ifndef Pin_H
#define Pin_H

//0(A)
#define PIN_RS485T1_LED     ((0<<8)+0)//0000 = PA.0.0010 = LCD_DATA0.Others = PA.0.PWNON_SET0
#define PIN_RS485R1_LED     ((0<<8)+1)//0000 = PA.1.0010 = LCD_DATA1.Others = PA.1.PWNON_SET1
#define PIN_GPRSTX_LED     ((0<<8)+2)//0000 = PA.2.0010 = LCD_DATA2.Others = PA.2.PWNON_SET2
#define PIN_GPRSRX_LED     ((0<<8)+3)//0000 = PA.3.0010 = LCD_DATA3.Others = PA.3.PWNON_SET3
#define PIN_ATTR_LED     ((0<<8)+4)//0000 = PA.4.0010 = LCD_DATA4.Others = PA.4.PWNON_SET4
#define PIN_LED_ONLINE     ((0<<8)+5)//0000 = PA.5.0010 = LCD_DATA5.Others = PA.5.PWNON_SET5
#define PIN_GPRS_PCTRL     ((0<<8)+6)//0000 = PA.6.0010 = LCD_DATA6.Others = PA.6.PWNON_SET6
#define PIN_ENW5V0     ((0<<8)+7)//0000 = PA.7.0010 = LCD_DATA7.Others = PA.7.PWNON_SET7
#define PIN_ENE5V0     ((0<<8)+8)//0000 = PA.8.0010 = LCD_DATA8.Others = PA.8.PWNON_SET8
#define PIN_ENR5V0     ((0<<8)+9)//0000 = PA.9.0010 = LCD_DATA9.Others = PA.9.PWNON_SET9

//1(B)
#define PIN_SPI0_SS1     ((1<<8)+0)//0000 = PB.0.0101 = NAND_nCS1.1001 = UR5_TXD.1011 = SPI0_SS1.1101 = ETMR1_TGL.Others = PB.0.
#define PIN_STRONG_SINGLE     ((1<<8)+1)//#define PIN_SPI1_SS1     ((1<<8)+1)//0000 = PB.1.0101 = NAND_RDY1.1001 = UR5_RXD.1011 = SPI1_SS1.1101 = ETMR1_CAP.Others = PB.1.

#define PIN_SPI0_SS0     ((1<<8)+6)//0000 = PB.6.1011 = SPI0_SS0.Others = PB.6.
#define PIN_SPI0_CLK     ((1<<8)+7)//0000 = PB.7.1011 = SPI0_CLK.Others = PB.7.
#define PIN_SPI0_MOSI     ((1<<8)+8)//0000 = PB.8.1011 = SPI0_DATAO (SPI0_DATA0).Others = PB.8.
#define PIN_SPI0_MISO     ((1<<8)+9)//0000 = PB.9.1011 = SPI0_DATAI (SPI0_DATA1).Others = PB.9.
#define PIN_SPACE_PB10     ((1<<8)+10)//0000 = PB.10.1001 = UR10_TXD.1011 = SPI0_DATA2.1100 = CAN0_RX.Others = PB.10.
#define PIN_SPACE_PB11     ((1<<8)+11)//0000 = PB.11.1001 = UR10_RXD.1011 = SPI0_DATA3.1100 = CAN0_TX.Others = PB.11.
#define PIN_SPACE_PB12     ((1<<8)+12)//0000 = PB.12.1001 = UR10_TXD.1011 = SPI1_SS0.Others = PB.12.
#define PIN_SPACE_PB13     ((1<<8)+13)//0000 = PB.13.1001 = UR10_RXD.1011 = SPI1_CLK.Others = PB.13.

//2(C)
#define PIN_UART_TXD4     ((2<<8)+10)//0000 = PC.10.0101 = NAND_CLE.1001 = UR4_TXD.1101 = ETMR2_TGL.Others = PC.10.
#define PIN_UART_RXD4     ((2<<8)+11)//0000 = PC.11.0101 = NAND_nWE.1001 = UR4_RXD.1101 = ETMR2_CAP.Others = PC.11.
#define PIN_WSTASE0     ((2<<8)+12)//0000 = PC.12.0101 = NAND_nRE.1001 = UR4_RTS.1101 = ETMR3_TGL.Others = PC.12.
#define PIN_WRST     ((2<<8)+13)//0000 = PC.13.0101 = NAND_RDY0.1001 = UR4_CTS.1101 = ETMR3_CAP.1111 = CLK_OUT.Others = PC.13.

//3(D)
#define PIN_FACTORY     ((3<<8)+0)//0000 = PD.0.0110 = SD0_CMD.Others = PD.0.
#define PIN_SWIN1      ((3<<8)+1)//0000 = PD.1.0110 = SD0_CLK.Others = PD.1.
#define PIN_ENTESAM     ((3<<8)+2)//0000 = PD.2.0110 = SD0_DATA0.Others = PD.2.
#define PIN_RST_GPRS     ((3<<8)+3)//0000 = PD.3.0110 = SD0_DATA1.Others = PD.3.
#define PIN_PWREN     ((3<<8)+4)//0000 = PD.4.0110 = SD0_DATA2.Others = PD.4.
#define PIN_WKUP     ((3<<8)+5)//0000 = PD.5.0110 = SD0_DATA3.Others = PD.5.
#define PIN_ETH_RST     ((3<<8)+6)//0000 = PD.6.0110 = SD0_nCD.Others = PD.6.

#define PIN_RUNLED     ((3<<8)+14)//0000 = PD.14.0010 = LCD_DATA22.1001 = UR9_TXD.1101 = PWM2.1110 = EBI_nOE.Others = PD.14.
#define PIN_KHZ38     ((3<<8)+15)//0000 = PD.15.0010 = LCD_DATA23.1001 = UR9_RXD.1101 = PWM3.1110 = EBI_nWAIT.Others = PD.15.

//4(E)
#define PIN_UART_TXD0     ((4<<8)+0)//0000 = PE.0.1001 = UR0_TXD.Others = PE.0.
#define PIN_UART_RXD0     ((4<<8)+1)//0000 = PE.1.1001 = UR0_RXD.Others = PE.1.
#define PIN_UART_TXD1     ((4<<8)+2)//0000 = PE.2.0001 = RMII1_MDC.0110 = SD1_CMD.1001 = UR1_TXD.Others = PE.2.
#define PIN_UART_RXD1     ((4<<8)+3)//0000 = PE.3.0001 = RMII1_MDIO.0110 = SD1_CLK.1001 = UR1_RXD.Others = PE.3.
#define PIN_UART_TR1     ((4<<8)+4)//0000 = PE.4.0001 = RMII1_TXDATA0.0110 = SD1_DATA0.1001 = UR1_RTS.Others = PE.4.
#define PIN_WEAK_SINGLE    ((4<<8)+5)//0000 = PE.5.0001 = RMII1_TXDATA1.0110 = SD1_DATA1.1001 = UR1_CTS.1111 = CLK_OUT.Others = PE.5.

//5(F)
#define PIN_ETH_MDC     ((5<<8)+0)//0000 = PF.0.0001 = RMII0_MDC.Others = PF.0.
#define PIN_ETH_MDIO     ((5<<8)+1)//0000 = PF.1.0001 = RMII0_MDIO.Others = PF.1.
#define PIN_ETH_TXD0     ((5<<8)+2)//0000 = PF.2.0001 = RMII0_TXDATA0.Others = PF.2.
#define PIN_ETH_TXD1     ((5<<8)+3)//0000 = PF.3.0001 = RMII0_TXDATA1.Others = PF.3.
#define PIN_ETH_TX_EN     ((5<<8)+4)//0000 = PF.4.0001 = RMII0_TXEN.Others = PF.4.
#define PIN_ETH_REF_CLK     ((5<<8)+5)//0000 = PF.5.0001 = RMII0_REFCLK.Others = PF.5.
#define PIN_ETH_RXD0     ((5<<8)+6)//0000 = PF.6.0001 = RMII0_RXDATA0.Others = PF.6.
#define PIN_ETH_RXD1     ((5<<8)+7)//0000 = PF.7.0001 = RMII0_RXDATA1.Others = PF.7.
#define PIN_ETH_GRS_DV     ((5<<8)+8)//0000 = PF.8.0001 = RMII0_CRSDV.Others = PF.8.
#define PIN_ETH_RDERR     ((5<<8)+9)//0000 = PF.9.0001 = RMII0_RXERR.Others = PF.9.
#define PIN_SPACE_PF10     ((5<<8)+10)//0000 = PF.10.0111 = USBH_PPWR.Others = PF.10.
#define PIN_UART_TXD2     ((5<<8)+11)//0000 = PF.11.1001 = UR2_TXD.1101 = ETMR2_TGL.1111 = EINT0.Others = PF.11.
#define PIN_UART_RXD2     ((5<<8)+12)//0000 = PF.12.1001 = UR2_RXD.1101 = ETMR2_CAP.1111 = EINT1.Others = PF.12.
#define PIN_UART_TR2     ((5<<8)+13)//0000 = PF.13.1001 = UR2_RTS.1101 = ETMR3_TGL.1111 = EINT2.Others = PF.13.
#define PIN_SPACE_PF14     ((5<<8)+14)//0000 = PF.14.1001 = UR2_CTS.1101 = ETMR3_CAP.1111 = EINT3.Others = PF.14.


//6(G) 0-14
#define PIN_SCL2     ((6<<8)+0)//0000 = PG.0.1000 = I2C0_SCL.Others = PG.0.
#define PIN_SDA2     ((6<<8)+1)//0000 = PG.1.1000 = I2C0_SDA.Others = PG.1.

#define PIN_GPRS_ID1     ((6<<8)+10)//0000 = PG.10.1000 = I2S_SYSCLK.1010 = SMC0_RST.Others = PG.10.
#define PIN_UART_TXD6     ((6<<8)+11)//0000 = PG.11.1000 = I2S_DATAO.1001 = UR6_TXD.1010 = SMC0_CLK.Others = PG.11.
#define PIN_UART_RXD6     ((6<<8)+12)//0000 = PG.12.1000 = I2S_DATAI.1001 = UR6_RXD.1010 = SMC0_DATA.Others = PG.12.
#define PIN_GPRS_ID0     ((6<<8)+13)//0000 = PG.13.1000 = I2S_BITCLK.1001 = UR6_RTS.1010 = SMC0_PWR.Others = PG.13.
#define PIN_ON_GPRS     ((6<<8)+14)//0000 = PG.14.1000 = I2S_WS.1001 = UR6_CTS.1010 = SMC0_CD.1111 = CLK_OUT.Others = PG.14.

//7(H)
#define PIN_USB0_VBUS     ((7<<8)+0)//0000 = PH.0.0111 = USBD_VBUSVLD.1111 = EINT0.Others = PH.0.
#define PIN_SPACE_PH1     ((7<<8)+1)//0000 = PH.1.0111 = USBH_OVRCUR.1111 = EINT1.Others = PH.1.

#define PIN_UART_TXD8     ((7<<8)+12)//0000 = PH.12.0110 = SD1_nCD.1001 = UR8_TXD.1011 = SPI0_SS1.1110 = EBI_ADDR8.Others = PH.12.
#define PIN_UART_RXD8     ((7<<8)+13)//0000 = PH.13.0110 = SD1_nPWR.1001 = UR8_RXD.1011 = SPI1_SS1.1110 = EBI_ADDR9.Others = PH.13.

//8(I)
#define PIN_NAND_nCS0     ((8<<8)+1)//0000 = PI.1.0101 = NAND_nCS0.1001 = UR7_TXD.1110 = EBI_DATA1.1111 = EINT6.Others = PI.1.
#define PIN_NAND_nWP     ((8<<8)+2)//0000 = PI.2.0101 = NAND_nWP.1001 = UR7_RXD.1110 = EBI_DATA2.1111 = EINT7.Others = PI.2.
#define PIN_NAND_ALE     ((8<<8)+3)//0000 = PI.3.0011 = VCAP_CLKO.0101 = NAND_ALE.1000 = I2C1_SCL.1100 = CAN0_RX.1101 = RTC_TICK.1110 = EBI_DATA3.Others = PI.3.
#define PIN_NAND_CLE     ((8<<8)+4)//0000 = PI.4.0011 = VCAP_PCLK.0101 = NAND_CLE.1000 = I2C1_SDA.1100 = CAN0_TX.1110 = EBI_DATA4.Others = PI.4.
#define PIN_NAND_nWE     ((8<<8)+5)//0000 = PI.5.0011 = VCAP_HSYNC.0100 = SD1_CMD.0101 = NAND_nWE.0110 = eMMC_CMD.1001 = UR1_TXD.1011 = SPI1_SS0.1110 = EBI_DATA5.Others = PI.5.
#define PIN_NAND_nRE     ((8<<8)+6)//0000 = PI.6.0011 = VCAP_VSYNC.0100 = SD1_CLK.0101 = NAND_nRE.0110 = eMMC_CLK.1001 = UR1_RXD.1010 = SMC1_RST.1011 = SPI1_CLK.1110 = EBI_DATA6.Others = PI.6.
#define PIN_NAND_RDY0     ((8<<8)+7)//0000 = PI.7.0011 = VCAP_FIELD.0100 = SD1_DATA0.0101 = NAND_RDY0.0110 = eMMC_DATA3.1001 = UR1_RTS.1010 = SMC1_CLK.1011 = SPI1_DATAO (SPI1_DATA0).1110 = EBI_DATA7.Others = PI.7.
#define PIN_NAND_DATA0     ((8<<8)+8)//0000 = PI.8.0011 = VCAP_DATA0.0100 = SD1_DATA1.0101 = NAND_DATA0.0110 = eMMC_DATA0.1001 = UR1_CTS.1010 = SMC1_DATA.1011 = SPI1_DATAI (SPI1_DATA1).1110 = EBI_DATA8.Others = PI.8.
#define PIN_NAND_DATA1     ((8<<8)+9)//0000 = PI.9.0011 = VCAP_DATA1.0100 = SD1_DATA2.0101 = NAND_DATA1.0110 = eMMC_DATA1.1001 = UR4_TXD.1010 = SMC1_PWR.1110 = EBI_DATA9.Others = PI.9.
#define PIN_NAND_DATA2     ((8<<8)+10)//0000 = PI.10.0011 = VCAP_DATA2.0100 = SD1_DATA3.0101 = NAND_DATA2.0110 = eMMC_DATA2.1001 = UR4_RXD.1010 = SMC1_CD.1110 = EBI_DATA10.Others = PI.10.
#define PIN_NAND_DATA3     ((8<<8)+11)//0000 = PI.11.0011 = VCAP_DATA3.0101 = NAND_DATA3.1010 = SMC0_RST.1110 = EBI_DATA11.Others = PI.11.
#define PIN_NAND_DATA4     ((8<<8)+12)//0000 = PI.12.0011 = VCAP_DATA4.0100 = SD1_nCD.0101 = NAND_DATA4.1001 = UR8_TXD.1010 = SMC0_CLK.1110 = EBI_DATA12.Others = PI.12.
#define PIN_NAND_DATA5     ((8<<8)+13)//0000 = PI.13.0011 = VCAP_DATA5.0100 = SD1_nPWR.0101 = NAND_DATA5.1001 = UR8_RXD.1010 = SMC0_DATA.1110 = EBI_DATA13.Others = PI.13.
#define PIN_NAND_DATA6   ((8<<8)+14)//0000 = PI.14.0011 = VCAP_DATA6.0101 = NAND_DATA6.1001 = UR8_RTS.1010 = SMC0_PWR.1110 = EBI_DATA14.Others = PI.14.
#define PIN_NAND_DATA7    ((8<<8)+15)//0000 = PI.15.0011 = VCAP_DATA7.0101 = NAND_DATA7.1001 = UR8_CTS.1010 = SMC0_CD.1110 = EBI_DATA15.1111 = CLK_OUT.Others = PI.15.

//9(J) 0-4
	//0000 = PJ.0.1111 = JTAG_TCK.Others = PJ.0.
	//0000 = PJ.1.1111 = JTAG_TMS.Others = PJ.1.
	//0000 = PJ.2.1111 = JTAG_TDI.Others = PJ.2.
	//0000 = PJ.3.1111 = JTAG_TDO.Others = PJ.3.
	//0000 = PJ.4.1111 = JTAG_nTRST.Others = PJ.4.

//非GPIO类功能引脚
//USB0
	//PIN_USB0_ID
	//PIN_USB0_VBUS
	//PIN_USB0_DM
	//PIN_USB0_DP
//USB1
	//PIN_USB1_DM
	//PIN_USB1_DP
//RTC
	//PIN_PWREN(仅NUC972,NUC975需用GPIO定义)
	
//ADC4-ADC7
	//ADC4	any	(YM ADC analog input.If used in touch screen, it should connect to the negative end of Y axis.If used in 5-wire touch screen, it could connect to lowerleft electrode)
	//ADC5	any	(YP ADC analog input.If used in touch screen, it should connect to the positive end of Y axis.If used in 5-wire touch screen, it could connect to upper-right electrode)
	//ADC6	any	(XM ADC analog input.If used in touch screen, it should connect to the negative end of X axis.If used in 5-wire touch screen, it could connect to lowerright electrode)
	//ADC7	any	(XP ADC analog input.If used in 4-wire touch screen, it should connect to the positive end of X axis.If used in 5-wire touch screen, it could connect to upper-left electrode)



#endif





