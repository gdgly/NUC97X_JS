
//引脚定义
#ifndef Pin_H
#define Pin_H


//0(A)
#define PIN_WKUP     (0<<8)+0
#define PIN_ETH_REF_CLK     (0<<8)+1
#define PIN_ETH_MDIO     (0<<8)+2
#define PIN_SPACE_PA3     (0<<8)+3//ADC123_IN3
#define PIN_DAC1     (0<<8)+4
#define PIN_SPACE_PA5     (0<<8)+5//ADC12_IN4
#define PIN_ADC12_IN6     (0<<8)+6
#define PIN_ETH_GRS_DV     (0<<8)+7
#define PIN_PLC_PWR     (0<<8)+8
#define PIN_USART1_TX   (0<<8)+9
#define PIN_USART1_RX   (0<<8)+10
#define PIN_USB_FS_DM    (0<<8)+11
#define PIN_USB_FS_DP    (0<<8)+12
#define PIN_SWDIO     (0<<8)+13//PA13 SW debug port
#define PIN_SWCLK     (0<<8)+14//PA14 SW debug port
#define PIN_SPI1_NSS     (0<<8)+15
//1(B)
#define PIN_URTC   (1<<8)+0
#define PIN_UBACKUP   (1<<8)+1
#define PIN_BOOT1   (1<<8)+2
#define PIN_SPI1_SCK   (1<<8)+3
#define PIN_SPI1_MISO   (1<<8)+4
#define PIN_SPI1_MOSI   (1<<8)+5
#define PIN_ENET_ACT_LED   (1<<8)+6
#define PIN_GPRS_ID0   (1<<8)+7
#define PIN_GPRS_ID1   (1<<8)+8
#define PIN_ENTESAM   (1<<8)+9
#define PIN_USART3_TX   (1<<8)+10
#define PIN_USART3_RX   (1<<8)+11
#define PIN_USB_HS_ID   (1<<8)+12
#define PIN_USB_HS_VBUS   (1<<8)+13
#define PIN_USB_HS_DM   (1<<8)+14
#define PIN_USB_HS_DP   (1<<8)+15
//2(C)
#define PIN_FMC_SDNWE   (2<<8)+0
#define PIN_ETH_MDC   (2<<8)+1
#define PIN_UGPRS   (2<<8)+2
#define PIN_ADC123_IN13   (2<<8)+3
#define PIN_ETH_RXD0   (2<<8)+4
#define PIN_ETH_RXD1   (2<<8)+5
#define PIN_USART6_TX   (2<<8)+6
#define PIN_USART6_RX   (2<<8)+7
#define PIN_RST_GPRS     (2<<8)+8
#if (HVER==0x400)//硬件版本号4.00
#define PIN_SPACE_PC9    (2<<8)+9
#endif
#if (HVER==0x410)//硬件版本号4.10
#define PIN_ON_GPRS     (2<<8)+9
#endif
#define PIN_UART4_RX   (2<<8)+10
#define PIN_UART4_TX   (2<<8)+11
#define PIN_UART5_TX   (2<<8)+12
#define PIN_RTC_TAMP1   (2<<8)+13
#define PIN_OSC32_IN   (2<<8)+14
#define PIN_OSC32_OUT   (2<<8)+15
//3(D)
#define PIN_FMC_D2   (3<<8)+0
#define PIN_FMC_D3   (3<<8)+1
#define PIN_UART5_RX   (3<<8)+2
#define PIN_UART5_OE   (3<<8)+3
#define PIN_FMC_NOE   (3<<8)+4
#define PIN_FMC_NWE   (3<<8)+5
#define PIN_SPACE_PD6     (3<<8)+6//PIN_NAND_WAIT
#define PIN_FACTORY     (3<<8)+7
#define PIN_FMC_D13   (3<<8)+8
#define PIN_FMC_D14   (3<<8)+9
#define PIN_FMC_D15   (3<<8)+10
#define PIN_FMC_CLE   (3<<8)+11
#define PIN_FMC_ALE   (3<<8)+12
#define PIN_ENLED3V3   (3<<8)+13
#define PIN_FMC_D0   (3<<8)+14
#define PIN_FMC_D1   (3<<8)+15
//4(E)
#define PIN_FMC_NBL0   (4<<8)+0
#define PIN_FMC_NBL1   (4<<8)+1
#define PIN_SWIN1   (4<<8)+2
#define PIN_SWIN2     (4<<8)+3
#define PIN_SWIN3     (4<<8)+4
#define PIN_SWIN4     (4<<8)+5
#define PIN_DOOR_IN   (4<<8)+6
#define PIN_FMC_D4   (4<<8)+7
#define PIN_FMC_D5   (4<<8)+8
#define PIN_FMC_D6   (4<<8)+9
#define PIN_FMC_D7   (4<<8)+10
#define PIN_FMC_D8   (4<<8)+11
#define PIN_FMC_D9   (4<<8)+12
#define PIN_FMC_D10   (4<<8)+13
#define PIN_FMC_D11   (4<<8)+14
#define PIN_FMC_D12   (4<<8)+15
//5(F)
#define PIN_FMC_A0   (5<<8)+0
#define PIN_FMC_A1   (5<<8)+1
#define PIN_FMC_A2   (5<<8)+2
#define PIN_FMC_A3   (5<<8)+3
#define PIN_FMC_A4   (5<<8)+4
#define PIN_FMC_A5   (5<<8)+5
#define PIN_UART7_RX   (5<<8)+6
#define PIN_UART7_TX   (5<<8)+7
#define PIN_ENIO3V3   (5<<8)+8
#define PIN_NEXTI     (5<<8)+9
#define PIN_NEXTD     (5<<8)+10
#define PIN_FMC_SDNRAS   (5<<8)+11
#define PIN_FMC_A6   (5<<8)+12
#define PIN_FMC_A7   (5<<8)+13
#define PIN_FMC_A8   (5<<8)+14
#define PIN_FMC_A9   (5<<8)+15
//6(G)
#define PIN_FMC_A10     (6<<8)+0
#define PIN_FMC_A11     (6<<8)+1
#define PIN_FMC_A12     (6<<8)+2
#define PIN_SPACE_PG3    (6<<8)+3//PIN_FMC_A13
#define PIN_FMC_BA0     (6<<8)+4
#define PIN_FMC_BA1     (6<<8)+5
#define PIN_PLC_ID     (6<<8)+6
#define PIN_PLC_RST     (6<<8)+7
#define PIN_FMC_SDCLK     (6<<8)+8
#define PIN_FMC_NCE3   (6<<8)+9
#define PIN_FMC_NE3   (6<<8)+10
#define PIN_ETH_TX_EN   (6<<8)+11
#define PIN_BUZZ   (6<<8)+12
#define PIN_ETH_TXD0   (6<<8)+13
#define PIN_ETH_TXD1   (6<<8)+14
#define PIN_FMC_SDNCAS   (6<<8)+15
//7(H)
#define PIN_OSC_IN     (7<<8)+0
#define PIN_OSC_OUT     (7<<8)+1
#define PIN_FMC_SDCKE0     (7<<8)+2
#define PIN_FMC_SDNE0     (7<<8)+3
#define PIN_SCL2     (7<<8)+4
#define PIN_SDA2     (7<<8)+5
#define PIN_KHZ38     (7<<8)+6
#define PIN_ENBACKLIGHT   (7<<8)+7
#define PIN_ATTR_LED     (7<<8)+8
#define PIN_ENHOT     (7<<8)+9
#define PIN_ETH_RST   (7<<8)+10
#define PIN_ENET_LINK_LED  (7<<8)+11
#define PIN_ENR5V0   (7<<8)+12
#define PIN_ENUSB5V0   (7<<8)+13
#define PIN_EMURST   (7<<8)+14
#define PIN_ENA5V0   (7<<8)+15

//8(I)
#define PIN_SPI2_NSS   	(8<<8)+0
#define PIN_SPI2_SCK   	(8<<8)+1
#define PIN_SPI2_MISO   	(8<<8)+2
#define PIN_SPI2_MOSI   	(8<<8)+3
#define PIN_ENE5V0   	(8<<8)+4
#define PIN_SPACE_PI5     (8<<8)+5//GPRS_ID2
#define PIN_SPACE_PI6     (8<<8)+6//GPRS_ID3
#define PIN_SPACE_PI7     (8<<8)+7//GPRS_ID4
#define PIN_SPACE_PI8     (8<<8)+8
#define PIN_ENTER     (8<<8)+9
#define PIN_LCD_RST   (8<<8)+10
#define PIN_UART7_OE   (8<<8)+11
#define PIN_LEFT     (8<<8)+12
#define PIN_RIGHT     (8<<8)+13
#define PIN_RUNLED   (8<<8)+14
#define PIN_ESC       (8<<8)+15
//9(J)
#if (HVER==0x400)//硬件版本号4.00
#define PIN_RS485R2_LED     (9<<8)+0
#define PIN_RS485T2_LED     (9<<8)+1
#define PIN_FUNIMP        (9<<8)+2
#define PIN_SPACE_PJ3     (9<<8)+3
#define PIN_SPACE_PJ4     (9<<8)+4
#define PIN_SPACE_PJ5     (9<<8)+5
#define PIN_SPACE_PJ6     (9<<8)+6
#define PIN_SPACE_PJ7     (9<<8)+7
#define PIN_SPACE_PJ8     (9<<8)+8
#define PIN_SPACE_PJ9     (9<<8)+9
#define PIN_SPACE_PJ10     (9<<8)+10
#define PIN_SPACE_PJ11     (9<<8)+11
#define PIN_ON_GPRS     (9<<8)+12
#define PIN_SPACE_PJ13     (9<<8)+13
#define PIN_SPACE_PJ14     (9<<8)+14
#define PIN_SPACE_PJ15     (9<<8)+15//PIN_CTR_EN
#endif
#if (HVER==0x410)//硬件版本号4.10
#define PIN_RS485R2_LED     ((9<<8)+0)
#define PIN_RS485T2_LED     ((9<<8)+1)
#define PIN_FUNIMP        ((9<<8)+2)
#define PIN_LCD_A0     ((9<<8)+3)
#define PIN_LCD_D0     ((9<<8)+4)
#define PIN_LCD_D1     ((9<<8)+5)
#define PIN_LCD_D2     ((9<<8)+6)
#define PIN_LCD_D3     ((9<<8)+7)
#define PIN_LCD_D4     ((9<<8)+8)
#define PIN_LCD_D5     ((9<<8)+9)
#define PIN_LCD_D6     ((9<<8)+10)
#define PIN_LCD_D7     ((9<<8)+11)
#define PIN_LCD_CS     ((9<<8)+12)
#define PIN_LCD_WR     ((9<<8)+13)
#define PIN_LCD_RD     ((9<<8)+14)
#define PIN_SPACE_PJ15     (9<<8)+15//PIN_CTR_EN
#endif

//10(K)
#define PIN_SPACE_PK0     (10<<8)+0
#define PIN_SPACE_PK1     (10<<8)+1
#define PIN_SPACE_PK2     (10<<8)+2
#define PIN_SPACE_PK3     (10<<8)+3
#define PIN_SPACE_PK4     (10<<8)+4
#define PIN_SPACE_PK5     (10<<8)+5
#define PIN_RS485R1_LED     (10<<8)+6
#define PIN_RS485T1_LED     (10<<8)+7
#define PIN_SPACE_PK8     (10<<8)+8
#define PIN_SPACE_PK9     (10<<8)+9
#define PIN_SPACE_PK10     (10<<8)+10
#define PIN_SPACE_PK11     (10<<8)+11
#define PIN_SPACE_PK12     (10<<8)+12
#define PIN_SPACE_PK13     (10<<8)+13
#define PIN_SPACE_PK14     (10<<8)+14
#define PIN_SPACE_PK15     (10<<8)+15

#endif





