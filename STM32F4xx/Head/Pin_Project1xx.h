
//专变终端引脚定义
#ifndef Pin_H
#define Pin_H

#define PIN_U2_OE          0//
#define PIN_U3_OE          0//
//P0
#define PIN_TXD3            (0<<5)//串行通讯口3发送
#define PIN_RXD3            (0<<5)+1//串行通讯口3接收
#define PIN_TXD0            (0<<5)+2//串行通讯口0发送
#define PIN_RXD0            (0<<5)+3//串行通讯口0接收
#define PIN_RXD5            (0<<5)+4//串行通讯口5接收
#define PIN_RST7858         (0<<5)+5//计量芯片复位信号
#define PIN_NSS1            (0<<5)+6//
#define PIN_SCK1            (0<<5)+7//SPI1口时钟信号
#define PIN_MISO1           (0<<5)+8//
#define PIN_MOSI1           (0<<5)+9//
#define PIN_TXD2            (0<<5)+10//串行通讯口2发送
#define PIN_RXD2            (0<<5)+11//串行通讯口2接收
#define PIN_NEXTI           (0<<5)+12//
#define PIN_NEXTD           (0<<5)+13//
#define PIN_TESAMIO         (0<<5)+14//
#define PIN_TXD1_MOSI2      (0<<5)+15//串行通讯口1发送
#define PIN_RXD1_MISO2      (0<<5)+16//串行通讯口1接收
#define PIN_EVENT_RST       (0<<5)+17//
#define PIN_DCD1_STA        (0<<5)+18//
#define PIN_DSR1            (0<<5)+19//
#define PIN_DTR1_NSS2_SET   (0<<5)+20//
#define PIN_RI1_SCK2_EVENT  (0<<5)+21//
#define PIN_SET             (0<<5)+22//
#define PIN_BV1             (0<<5)+23//
#define PIN_BV2             (0<<5)+24//
#define PIN_BV3             (0<<5)+25//
#define PIN_BUZZ            (0<<5)+26//
#define PIN_SDA0            (0<<5)+27//
#define PIN_SCL0            (0<<5)+28//
#define PIN_OPEN1           (0<<5)+29//
#define PIN_OPEN2           (0<<5)+30//
#define PIN_USB2_D          (0<<5)+31//




//P1
#define PIN_SCK2            (1<<5)//
#define PIN_MOSI2           (1<<5)+1//
//#define PIN_ENET_TXD2       (1<<5)+2//
//#define PIN_ENET_TXD3       (1<<5)+3//
#define PIN_MISO2           (1<<5)+4//
#define PIN_P208_TXD5       (1<<5)+5//
#define PIN_TESAMPWR        (1<<5)+6//
//#define PIN_ENET-COL        (1<<5)+7//
#define PIN_NSS2            (1<<5)+8//
#define PIN_ENET_LINK_LED   (1<<5)+9//
#define PIN_ENET_ACT_LED    (1<<5)+10//
//#define PIN_ENET_RXD2       (1<<5)+11//
#define PIN_A26             (1<<5)+12//
//#define PIN_ENET_RX_DV      (1<<5)+13//
#define PIN_TRIP_LED1  (1<<5)+14//
#define PIN_ENA5V0          (1<<5)+15//
#define PIN_ENR5V0          (1<<5)+16//
#define PIN_ENHOT           (1<<5)+17//
#define PIN_ENE5V0          (1<<5)+18//

#define PIN_ENET_RST        (1<<5)+19//
#define PIN_SCK0            (1<<5)+20//
#define PIN_NSS0            (1<<5)+21//
#define PIN_KHZ38           (1<<5)+22//
#define PIN_MISO0           (1<<5)+23//
#define PIN_MOSI0           (1<<5)+24//
#define PIN_TRIP1           (1<<5)+25//
#define PIN_TRIP2           (1<<5)+26//
#define PIN_FUNIMP          (1<<5)+27//
#define PIN_TRIP_ATTR       (1<<5)+28//
#define PIN_FACTORY         (1<<5)+29//
#define PIN_BV4             (1<<5)+30//
#define PIN_BV5             (1<<5)+31//




//P2
#define PIN_ENUSB5V0       (2<<5)//
#define PIN_RXD6           (2<<5)+1//
#define PIN_LCDDCLK        (2<<5)+2//
#define PIN_CARDRST        (2<<5)+3//
#define PIN_CARDCLK        (2<<5)+4//
#define PIN_CARDIO         (2<<5)+5//
#define PIN_CARDDOOR       (2<<5)+6//
#define PIN_MESAMRST       (2<<5)+7//
#define PIN_MESAMIO        (2<<5)+8//
#define PIN_PLCPWR         (2<<5)+9//
#define PIN_ISP            (2<<5)+10//
#define PIN_PWR_DOWN       (2<<5)+11//
#define PIN_MODULSEL0      (2<<5)+12//
#define PIN_MODULSEL1      (2<<5)+13//
#define PIN_CS2            (2<<5)+14//
#define PIN_CS3            (2<<5)+15//
#define PIN_CAS            (2<<5)+16//
#define PIN_RAS            (2<<5)+17//
#define PIN_CLK0           (2<<5)+18//
#define PIN_RIGHT          (2<<5)+19//
#define PIN_DYCS0          (2<<5)+20//
#define PIN_P208_TRIP3     (2<<5)+21//
#define PIN_P208_TRIP4     (2<<5)+22//
#define PIN_LEFT           (2<<5)+23//
#define PIN_CKE0           (2<<5)+24//
#define PIN_ENTER          (2<<5)+25//
#define PIN_ESC            (2<<5)+26//
#define PIN_P208_MODUL_RESET (2<<5)+27//
#define PIN_DQM0           (2<<5)+28//
#define PIN_DQM1           (2<<5)+29//
#define PIN_P208_TRIP_LED3      (2<<5)+30//
#define PIN_P208_TRIP_LED2      (2<<5)+31//




//P3
#define PIN_D0      (3<<5)+0
#define PIN_D1      (3<<5)+1
#define PIN_D2      (3<<5)+2
#define PIN_D3      (3<<5)+3
#define PIN_D4      (3<<5)+4
#define PIN_D5      (3<<5)+5
#define PIN_D6      (3<<5)+6
#define PIN_D7      (3<<5)+7
#define PIN_D8      (3<<5)+8
#define PIN_D9      (3<<5)+9
#define PIN_D10      (3<<5)+10
#define PIN_D11      (3<<5)+11
#define PIN_D12      (3<<5)+12
#define PIN_D13      (3<<5)+13
#define PIN_D14      (3<<5)+14
#define PIN_D15      (3<<5)+15
//      (3<<5)+16//
#define PIN_P208_DOOR       (3<<5)+17//
#define PIN_P208_TXD6       (3<<5)+18//
#define PIN_P208_SIN1       (3<<5)+19//
#define PIN_P208_SIN2       (3<<5)+20//
#define PIN_P208_SIN3       (3<<5)+21//
#define PIN_P208_SIN4       (3<<5)+22//
#define PIN_TESAMRST       (3<<5)+23//
#define PIN_ESAMCLK        (3<<5)+24//
#define PIN_ENLED3V3       (3<<5)+25//
#define PIN_ENIO3V3        (3<<5)+26//
#define PIN_P208_SIN5       (3<<5)+27//
#define PIN_P208_SIN6       (3<<5)+28//
#define PIN_P208_LED6           (3<<5)+29//
#define PIN_P208_LED5           (3<<5)+30//
#define PIN_P208_TRIP_LED4      (3<<5)+31//



//P4
#define PIN_MOSI3_SIO0     (4<<5)+0
#define PIN_MISO3_SIO1     (4<<5)+1
#define PIN_WP3_SIO2     (4<<5)+2
#define PIN_HOLD3_SIO3   (4<<5)+3
#define PIN_NSS3     (4<<5)+4
#define PIN_SCK3     (4<<5)+5

#define PIN_A0      (4<<5)+0
#define PIN_A1      (4<<5)+1
#define PIN_A2      (4<<5)+2
#define PIN_A3      (4<<5)+3
#define PIN_A4      (4<<5)+4
#define PIN_A5      (4<<5)+5
#define PIN_A6      (4<<5)+6
#define PIN_A7      (4<<5)+7
#define PIN_A8      (4<<5)+8
#define PIN_A9      (4<<5)+9
#define PIN_A10      (4<<5)+10
#define PIN_A11      (4<<5)+11
#define PIN_A12      (4<<5)+12
#define PIN_A13      (4<<5)+13
#define PIN_A14      (4<<5)+14
#define PIN_A15      (4<<5)+15
#define PIN_A16      (4<<5)+16
#define PIN_A17      (4<<5)+17
#define PIN_A18      (4<<5)+18
#define PIN_A19      (4<<5)+19
#define PIN_A20      (4<<5)+20
#define PIN_A21      (4<<5)+21
#define PIN_A22      (4<<5)+22
#define PIN_A23      (4<<5)+23
#define PIN_OE             (4<<5)+24//
#define PIN_WE             (4<<5)+25//
#define PIN_BLS0           (4<<5)+26//
#define PIN_BLS1           (4<<5)+27//
#define PIN_ATTR_LED       (4<<5)+28//
#define PIN_ENBACKLIGHT    (4<<5)+29//
#define PIN_CS0            (4<<5)+30//
#define PIN_CS1            (4<<5)+31//





//P5
#define PIN_POWER          (5<<5)+0//
#define PIN_A25            (5<<5)+1//
#define PIN_ON_RST         (5<<5)+2//此脚bug,输出时无高电平输出
#define PIN_RXD4           (5<<5)+3//串行通讯口4接收
#define PIN_TXD4           (5<<5)+4//串行通讯口4发送


#endif





