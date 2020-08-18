
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_TxDATA.h"


#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/RM_TxRxDATA_ABB.h"

//const u8 CONNECT_1[]=//读key
//{
//    0x02,0xFF,0x08,0x13,0xA1,0x08,0x00,0x00,0x10,0x62,0x41,0x15,0x86,0x4A,0xE1,
//};
//const u8 CONNECT_2[]=//远程登录口令验证返回
//{
//    0x02 ,0x18 ,0x00 ,0xA3 ,0x82 ,0x23,
//};
//const u8 CONNECT_3[]=//电量需量小数位
//{
//    0x02 ,0x05 ,0x00 ,0xA3 ,0xA8
//,0x00 ,0x02 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x0F ,0x0F
//,0x03 ,0x03
//,0xFF ,0x00 ,0x01 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00
//,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
//,0x00 ,0x00 ,0x92 ,0x95 ,0xC6,
//};
//const u8 CONNECT_4[]=//类11的配置属性
//{
//    0x02 ,0x05 ,0x00 ,0xA3 ,0xA8
//,0x0C ,0xE3 ,0x81
//,0x80
//,0x40
//,0x00 ,0x80 ,0x0C ,0x1C ,0x05 ,0x00 ,0xE4 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
//,0x00 ,0x80 ,0x00 ,0x0C ,0x00 ,0x00 ,0x80 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
//,0x00 ,0x00 ,0x00 ,0x00 ,0x00
//,0x03
//,0x0C
//,0x00 ,0x00 ,0x00 ,0x00 ,0x61 ,0x8E
//};
//const u8 CONNECT_101_CLASS9_1[]=//
//{
//0x02 ,0x05 ,0x00 ,0xA3 ,0xB0 ,0xA1 ,0x08 ,0x00 ,0x00 ,0x08,
//0x78 ,0x87 ,0x50 ,0x00 ,0x20 ,0x84 ,0x02 ,0x18 ,0x04 ,0x29,
//0x17 ,0x59 ,0x08 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x08,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x03 ,0x37 ,0x26 ,0x00 ,0x34,
//0x18 ,0x04 ,0x19 ,0x04 ,0x06 ,0x22 ,0x18 ,0x04 ,0x27 ,0x00,
//0x24 ,0x05 ,0x44 ,0x53 ,0x84,
//};
//const u8 CONNECT_101_CLASS11_1[]=//
//{
//    0x02,0x05 ,0x00 ,0xA3 ,0x2A ,0x04 ,0x44 ,0x24 ,0x85 ,0x56 ,0x00 ,0x00 ,0x00,
//0x00,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x02 ,0x95 ,0x31 ,0x00 ,0x00 ,0x00,
//0x04,0x59 ,0x27 ,0x50 ,0x16 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00,0x00 ,0x03 ,0x47 ,0x62 ,0x00 ,0x00 ,0x00 ,0xDF ,0x74 ,
//};
//const u8 CONNECT_101_CLASS11_2[]=//
//{
//0x02 ,0x81 ,0x00 ,0xA3 ,0x2A ,0x04 ,0x44 ,0x24 ,0x85 ,0x56,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x02 ,0x95 ,0x31 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x03 ,0x05 ,0x86 ,0x00 ,0x00 ,0x00 ,0x37 ,0xD4 ,
//};
//const u8 CONNECT_101_CLASS11_3[]=//
//{
//0x02 ,0x81 ,0x00 ,0xA3 ,0x2A ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x22, 0x9A ,

//};
//const u8 CONNECT_101_CLASS11_4[]=//
//{
//0x02 ,0x81 ,0x00 ,0xA3 ,0x2A ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x22, 0x9A ,
//};
//const u8 CONNECT_101_CLASS11_5[]=//
//{
//0x02 ,0x81 ,0x00 ,0xA3 ,0x2A ,0x01 ,0x15 ,0x04 ,0x97 ,0x20 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x04 ,0x71 ,0x00,
//0x00 ,0x00 ,0x00 ,0x84 ,0x85 ,0x67 ,0x20 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x11 ,0x94 ,0x00 ,0x00 ,0x00 ,0xB9, 0x2C ,
//};
//const u8 CONNECT_101_CLASS11_6[]=//
//{
//0x02 ,0x81 ,0x00 ,0xA3 ,0x2A ,0x01 ,0x15 ,0x04 ,0x97 ,0x20 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x04 ,0x71 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x98 ,0x24 ,0x00 ,0x00 ,0x00 ,0xFD, 0x98,
//
//};
//const u8 CONNECT_101_CLASS11_7[]=//
//{
//0x02 ,0x81 ,0x00 ,0xA3 ,0x2A ,0x00 ,0x04 ,0x87 ,0x96 ,0x04 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x17 ,0x39 ,0x00,
//0x00 ,0x00 ,0x00 ,0x01 ,0x33 ,0x95 ,0x48 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x16 ,0x36 ,0x00 ,0x00 ,0x00 ,0x15, 0xC8 ,
//};
//const u8 CONNECT_101_CLASS11_8[]=//
//{
//0x02 ,0x81 ,0x00 ,0xA3 ,0x2A ,0x00 ,0x04 ,0x87 ,0x96 ,0x04 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x17 ,0x39 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x19 ,0x10 ,0x00 ,0x00 ,0x00 ,0x9D, 0xD0,

//};
//const u8 CONNECT_101_CLASS11_9[]=
//{
//0x02 ,0x81 ,0x00 ,0xA3 ,0x9E ,0x00 ,0x07 ,0x07 ,0x96 ,0x24 ,0x00 ,0x00,
//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
//0x00 ,0x00 ,0x03 ,0x05 ,0x24 ,0x89 ,0x00 ,0x00 ,0x00 ,0x10 ,0x00 ,0x9B, 0x62  ,

//};

u32 RM_Rx_ABB(u32 PORTn)//UART_645接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
{
    u32 i;
    u32 Pn;
    u32 p0;
    u32 p1;
    u32 RxByte;

    u8 * p8fifo;
    u16 * p16fifo;
    u8 * p8rxbuff;
    u8 * p8;
    u16 * p16timer;
    UARTCtrl_TypeDef * UARTCtrl;
    u32 RxByte_Expect = 0;

    u32 LEN_UARTnIntRx = Get_LEN_UARTnIntRx(PORTn);
    u32 LEN_UARTnRx = Get_LEN_UARTnRx(PORTn);
    UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
    p8rxbuff = (u8 *)Get_ADDR_UARTnRx(PORTn);
    p8fifo = (u8 *)(Get_ADDR_UARTn(PORTn) + 10);
    p16fifo = (u16 *)Get_ADDR_UARTn(PORTn);
    p16timer = (u16 *)Get_ADDR_UARTnTimer(PORTn);

    DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
    p0 = p16fifo[0]; //中断接收指针
    p1 = p16fifo[1]; //当前已读指针
    if((p0 >= (LEN_UARTnIntRx - 10)) || (p1 >= (LEN_UARTnIntRx - 10)))
    {
        p16fifo[0] = 0; //中断接收指针
        p16fifo[1] = 0; //当前已读指针
        p16fifo[2] = 0; //规约检查指针
        p16fifo[3] = 0;
        p16fifo[4] = 0;
        return 0;
    }

    if(UARTCtrl->CONNECT == 101)
    {
        return 1;
    }
    /*
    //---TEST---
        p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
        p8 = p8rx+ABB_SUB_OFFSET;
    	switch(UARTCtrl->CONNECT)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
    	{
            case 1://300bps握手
            switch(p8[0])//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
    	{
    		case 1://300bps握手
                MR((u32)p8rx,(u32)CONNECT_1,sizeof(CONNECT_1));
    			UARTCtrl->RxByte=sizeof(CONNECT_1);
    			return 1;//1=接收OK
            case 2://连接密码校验
                MR((u32)p8rx,(u32)CONNECT_2,sizeof(CONNECT_2));
    			UARTCtrl->RxByte=sizeof(CONNECT_2);
    			return 1;//1=接收OK
            case 3://读类0，（电能量小数位）
                MR((u32)p8rx,(u32)CONNECT_3,sizeof(CONNECT_3));
    			UARTCtrl->RxByte=sizeof(CONNECT_3);
    			return 1;//1=接收OK
            case 4://读类2，（计费数据配置）
                MR((u32)p8rx,(u32)CONNECT_4,sizeof(CONNECT_4));
    			UARTCtrl->RxByte=sizeof(CONNECT_4);
    			return 1;//1=接收OK
            case 5://读类9，（时间）
                MR((u32)p8rx,(u32)CONNECT_101_CLASS9_1,sizeof(CONNECT_101_CLASS9_1));
    			UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS9_1);
    			return 1;//1=接收OK
            case 6://读类11，（电量）
                MR((u32)p8rx,(u32)CONNECT_101_CLASS9_1,sizeof(CONNECT_101_CLASS9_1));
    			UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS9_1);
               if(UARTCtrl->RM_SubsequentFrames==0)//0=无读后续帧,1-n=读续帧的帧号
    	        {
                        MR((u32)p8rx,(u32)CONNECT_101_CLASS11_1,sizeof(CONNECT_101_CLASS11_1));
    			        UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_1);
                }
                else
                {
                    i  = (*(p8rx+ABB_SUB_OFFSET+ABB_LEN_CLASS11)<<8);
                    i += (*(p8rx+ABB_SUB_OFFSET+ABB_LEN_CLASS11+1));
                    i=i/42;
                    switch(i)
                    {
                        case 0:
                            break;
                        case 1:
                            MR((u32)p8rx,(u32)CONNECT_101_CLASS11_2,sizeof(CONNECT_101_CLASS11_2));
    			            UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_2);
                            break;
                        case 2:
                            MR((u32)p8rx,(u32)CONNECT_101_CLASS11_3,sizeof(CONNECT_101_CLASS11_3));
    			            UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_3);
                            break;
                        case 3:
                            MR((u32)p8rx,(u32)CONNECT_101_CLASS11_4,sizeof(CONNECT_101_CLASS11_4));
    			            UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_4);
                            break;
                        case 4:
                            MR((u32)p8rx,(u32)CONNECT_101_CLASS11_5,sizeof(CONNECT_101_CLASS11_5));
    			            UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_5);
                            break;
                        case 5:
                            MR((u32)p8rx,(u32)CONNECT_101_CLASS11_6,sizeof(CONNECT_101_CLASS11_6));
    			            UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_6);
                            break;
                        case 6:
                            MR((u32)p8rx,(u32)CONNECT_101_CLASS11_7,sizeof(CONNECT_101_CLASS11_7));
    			            UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_7);
                            break;
                        case 7:
                            MR((u32)p8rx,(u32)CONNECT_101_CLASS11_8,sizeof(CONNECT_101_CLASS11_8));
    			            UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_8);
                            break;
                        case 8:
                            MR((u32)p8rx,(u32)CONNECT_101_CLASS11_9,sizeof(CONNECT_101_CLASS11_9));
    			            UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS11_9);
                            break;
                        default:
                            break;
                    }
                }
                return 1;//1=接收OK
            }
           case 101://101=抄表中
               return 1;
    		default:
    			return 1;
    	}

    //---TEST END---
    */

    RxByte = UARTCtrl->RxByte;
    if(p0 != p1)
    {   //有数据接收重置超时定时
        p16timer[0] = 500 / 10; //字节间超时定时
        if(PORTn == RS485_1PORT)
        {
            Terminal_Ram->RS485_1_RxLED_MS_Timer = 20 / 10; //11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
        }
        if(PORTn == RS485_2PORT)
        {
            Terminal_Ram->RS485_2_RxLED_MS_Timer = 20 / 10; //11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
        }
    }
    p8 = p8rxbuff + ABB_SUB_OFFSET;
    while(p0 != p1)
    {
        if(RxByte >= LEN_UARTnRx)
        {
            RxByte = 0;
        }
        i = p8fifo[p1];
        p1++;
        if(p1 >= (LEN_UARTnIntRx - 10))
        {
            p1 = 0;
        }
        p8rxbuff[RxByte] = i;
        RxByte++;
        if(RxByte == 1)
        {
            if(i != 0x02) //判断起始符02
            {
                RxByte = 0;
            }
            continue;
        }

        switch(p8[0])
        {
        case 0:
        case 1: //握手报文
            RxByte_Expect = 15;
            break;
        case 2: //密码校验
            RxByte_Expect = 6;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            if((RxByte == 2) && (p8rxbuff[1] != 0x05) && (p8rxbuff[1] != 0x81))
            {   //判断命令字是否正确
                RxByte = 0;
                continue;
            }
            if(RxByte < 3)
            {
                continue;
            }
            RxByte_Expect = 6; //p8rxbuff[2]错误和最低接收到的字节长度是6,
            if(p8rxbuff[2] == 0x00)
            {   //ACK正确
                if(RxByte >= 5)//收到帧的数据长度
                {
                    i = p8rxbuff[4] & 0x7F;
                    RxByte_Expect = i + 7;
                }
            }
            break;
        }
        if(RxByte >= RxByte_Expect)//接收数据长度足够，CRC校验判断
        {
            Pn = ABB_CRC((u8 *)&p8rxbuff[0], (RxByte_Expect - 2));
            i = (p8rxbuff[RxByte_Expect - 2] << 8) + (p8rxbuff[RxByte_Expect - 1]);
            if(Pn == i)
            {   //校验正确
                p16fifo[1] = p1; //当前已读指针
                p16fifo[2] = p1; //检查指针==读指针,免检省时
                UARTCtrl->RxByte = RxByte;
                return 1;
            }
            else
            {   //校验错误
                RxByte = 0;
            }
        }
    }

    p16fifo[1] = p1;
    UARTCtrl->RxByte = RxByte;
    if(p16timer[0] == 0x0)
    {   //超时
        return 2;//2=接收超时
    }
    return 0;
}

u32 RM_RxDATA_ABB_CHECK(u32 PORTn)//Landis帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
{
    return 0;
}
//p8rx:数据起始地址，Block：类11block代表的数据，Rate:总尖峰平谷费率;
//Offset:相对于电量的偏移量,length:数据长度
//返回：bit63：是否找到；bit62-bit0：电量数据。
u64 RM_RxDATA_ABB_Electric_Power(u8* p8rx, u32 Block, u8 Rate, u8 Offset, u8 Length) //
{
    u8 i;
    u32 Data;
    u64 Data64[5] = {0};
    u8* p8;
//起始---存储于通信通道第二个缓冲区
//2个字节  类11数据起始的缓冲偏移量，已经固定为16了，故暂时没有
//2个字节  类11数据已经写进缓冲的长度，接收帧p[4]的累加,16进制数
//4个字节  KEY的缓冲，4个字节的长度
//6个字节  电量和需量小数点，类11中BLOCK1 ，BLOCK2，BLOCK3 ，BLOCK4的数据
//第16个字节  存储的数据
    p8 = p8rx + ABB_SUB_OFFSET + ABB_CLASS11_BLOCK1; //获取数据的BLOCK的数值地址
    Data = (*(p8) << 24) + (*(p8 + 1) << 16) + (*(p8 + 2) << 8) + (*(p8 + 3));
    for(i = 0; i < 4; i++) //获取接收缓存电量的偏移量i
    {
        if(Block == (Data & 0xFF))
        {
            i = 84 * (3 - i);
            break;
        }
        Data >>= 8;
    }
    if(i == 4)
    {   //没有找到对应的电量数据项，
        return Data64[0] = 0;
    }
    p8 = p8rx + ABB_SUB_OFFSET + ABB_RM_CLASS11_OFFSET + i; //分帧数据首地址+i(偏移)
    for(i = 0; i < Length; i++) //筛选出电量中有效的字节，例如“04 44 24 85 56 00 00”选出 “04 44 24 85 56”
    {
        Data64[2] = ((Data64[2] << 8) + * (p8 + Offset + i)); //峰；Data1有8个字节，是为了防止左移溢出
        Data64[3] = ((Data64[3] << 8) + * (p8 + 21 + Offset + i)); //平
        Data64[4] = ((Data64[4] << 8) + * (p8 + 42 + Offset + i)); //谷
        Data64[1] = ((Data64[1] << 8) + * (p8 + 63 + Offset + i));
    }
    Data64[0]  = bcd16_hex(Data64[1]);
    Data64[0] += bcd16_hex(Data64[2]);
    Data64[0] += bcd16_hex(Data64[3]);
    Data64[0] += bcd16_hex(Data64[4]);
    Data64[0]  = hex_bcd16(Data64[0]);
    Data64[Rate] |= ((u64)0x01 << 63);
    return Data64[Rate];
}
//四舍五入函数
//四舍五入BCD最后一位，并除以10(BCD码左移4位)
u64 Round_up_or_down(u64 n)
{
    if((n & 0x0F) >= 5) //末位大于5则进一
    {
        n >>= 4;
        n = bcd16_hex(n);
        n += 1;
        n = hex_bcd16(n);
    }
    else
    {
        n >>= 4;
    }
    return n;
}
//UART抄表数据缓冲定义
//#define LEN_RM_DI_CSD     512
//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)

//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)

//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的数据长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的数据长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型)(ROAD时不写入总关联的OAD个数数组!!!)

u32 RM_RxDATA_ABB(u32 PORTn)//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
{
    u32 i;
    u32 n;
    u32 offset;
    u32 OI, oad;
    u32 Data;
    u64 Data1;
    u32 LEN_RxDataBuff;
    u32 LEN_Data0;
    u32 LEN_Data1;
    u8* p8Array;
    u8* p8Data;
    u16* p16Data;
    u8* p8rx;
    UARTCtrl_TypeDef * UARTCtrl;
    u8* p8;

    UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
    p8rx = (u8 *)Get_ADDR_UARTnRx(PORTn); ////得到UART接收缓冲地址

    LEN_RxDataBuff = Get_LEN_UARTnFnDataBuff(PORTn); //得到UART抄表数据缓冲长度
    LEN_RxDataBuff -= (LEN_RM_DI_CSD + 4);
    p8Data = (u8 *)Get_ADDR_UARTnFnDataBuff(PORTn); //得到UART抄表数据缓冲地址
    p16Data = (u16*)(p8Data + LEN_RM_DI_CSD);
    LEN_Data1 = p16Data[1];
    if(LEN_RxDataBuff < LEN_Data1)
    {   //错
        return 3;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
    }
    LEN_RxDataBuff -= LEN_Data1;
    p8Data += LEN_RM_DI_CSD + 4 + LEN_Data1;
    p8Array = (u8*)p8Data;
    if(UARTCtrl->RMCSDSubCount == 0) //抄表CSD子计数(非DL698抄表时)
    {
        p16Data[0] = 0; //已填写入buff但还没完整一个CSD的数据长度(不包括本身2字节)
    }
    LEN_Data0 = p16Data[0];
    if(LEN_RxDataBuff < LEN_Data0)
    {   //错
        return 1;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
    }
    LEN_RxDataBuff -= LEN_Data0;
    p8 = p8rx + LEN_UART2Rx + LEN_UART2Tx; //将接收到的后续帧数组，放到第二个缓冲区,前4个字节用来表示一些标志
    switch(UARTCtrl->CONNECT)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
    {
    case 1://300bps握手
        switch(p8[0])
        {
        case 0:
        case 1:
            MR((u32)(p8 + ABB_KEY), (u32)(p8rx + 9), 4); //存储密码
            p8[0] = 2;
            return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
        case 2://连接密码校验
            p8[0] = 3;
            return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
        case 3://读类0，（电能量小数位）
            MR((u32)(p8 + ABB_POWER_POINT), (u32)(p8rx + 16), 2); //电量小数位和需量小数位
            p8[0] = 4;
            return 0;
        case 4://读类2，（计费数据配置）
            MR((u32)(p8 + ABB_CLASS11_BLOCK1), (u32)(p8rx + 8), 2); //类11中BLOCK1 和类11中BLOCK2
            MR((u32)(p8 + ABB_CLASS11_BLOCK3), (u32)(p8rx + 8 + 2 + 29), 2); //类11中BLOCK3 和类11中BLOCK4
            p8[0] = 5;
            return 0;
        case 5://读类9，时间
            i = p8rx[4] & 0x7F; //正在处理的帧的长度
            MR((u32)(p8 + ABB_RM_CLASS9_OFFSET), (u32)(p8rx + 5), i); //时间存储到RS485的RAM区间
            p8[0] = 6;
            return 0;
        case 6://读类11实时电量
            i = p8rx[4] & 0x7F; //正在处理的帧的长度
            offset = (p8[ABB_LEN_CLASS11] << 8) + (p8[ABB_LEN_CLASS11 + 1]);
            MR((u32)(p8 + ABB_RM_CLASS11_OFFSET + offset), (u32)(p8rx + 5), i); //存储到RS485的RAM区间
            i += offset;
            if(i > ABB_LENMAX_POWER_CLASS11)
            {   //如果接收到的电量数据长度已经大于理论值（0x016E）了，说明流程或数据出错，不去抄这只表了，抄到的数据也不处理
                UARTCtrl->CONNECT = 102;
                p8[0] = 0;
            }
            p8[ABB_LEN_CLASS11] = (i >> 8) & 0xFF;
            p8[ABB_LEN_CLASS11 + 1] =  i & 0xFF;
            if((p8rx[4] & 0x80) == 0) //bit7为0有后续帧
            {
                UARTCtrl->RM_SubsequentFrames = 1; //有后续帧
            }
            else
            {
                UARTCtrl->RM_SubsequentFrames = 0; //无后续帧
                UARTCtrl->CONNECT = 100;
                p8[0] = 0;
            }
            return 0;
        }
    case 101://101=抄表中

        break;
    case 102://关闭连接
        return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
    default://698规约连接和关闭过程
        UARTCtrl->CONNECT = 0; //DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
        return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
    }

//---后续帧---

//---后续帧结束---


//---曲线开始---

//---曲线结束---


//---事件类开始---


//---事件类结束---

    i = UARTCtrl->RMCSD[1]; //当前正在抄的CSD(有数据类型)
    if(i == 0)
    {   //OAD
        OI = UARTCtrl->RMCSD[2] << 8; //当前正在抄的CSD
        OI |= UARTCtrl->RMCSD[3];
        oad = (UARTCtrl->RMCSD[4] << 8) + (UARTCtrl->RMCSD[5]);
    }
    else
    {   //ROAD
        i = UARTCtrl->RMROADCount; //抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
        i *= 4;
        OI = UARTCtrl->RMCSD[7 + i] << 8; //当前正在抄的CSD
        OI |= UARTCtrl->RMCSD[7 + i + 1];
        oad = (UARTCtrl->RMCSD[7 + i + 2] << 8) + (UARTCtrl->RMCSD[7 + i + 3]);
    }
    i = OI_IC(OI); //得到接口类号;入口:对象标识OI
    switch(i)
    {
    case 1://电能量接口类,本接口类对象提供存储电能量类信息
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //缓冲长度不够
                return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
        }
        switch((oad & 0xFF00) >> 8)
        {
        case 2://总及费率电能量数组∷=array 电能量
            //电能量：
            //CHOICE
            //{
            //double-long-unsigned   [6]，
            //double-long            [5]
            //}
            switch(OI)
            {
            case 0x0010://正有功
                n = 0x80;//ABB电表中的正向有功的标识
                break;
            case 0x0020://反有功
                n = 0x40; //ABB电表中的反向有功的标识
                break;
            case 0x0030://组合无功1(正无功)
                n = 0x03;   //ABB电表中的组合无功1的标识
                break;
            case 0x0040://组合无功2(反无功)
                n = 0x0C; //ABB电表中的组合无功2(反无功)的标识
                break;
            case 0x0050:
            case 0x0060:
            case 0x0070:
            case 0x0080:
            default:
                n = 0x00;
//                p8Array[0] = DataType_NULL;
//                LEN_Data0 = 1;
//                LEN_RxDataBuff -= 1;
//                p16Data[0] = LEN_Data0;
//                return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                break;
            }

            oad &= 0xFF;
            if(0 == oad)
            {   //如果是总电量则循环5次
                offset = 5;
                if(LEN_RxDataBuff < (5 * 5))
                {   //缓冲长度不够
                    return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                }
                p8Array[0] = DataType_array;
                p8Array[1] = offset;
                LEN_Data0 = 2;
                LEN_RxDataBuff -= 2;
                p8Data += LEN_Data0;
            }
            else
            {   //如果是分费率电量则循环1次
                offset = oad ;
                oad -= 1;
                if(LEN_RxDataBuff < 5)
                {   //缓冲长度不够
                    return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                }
            }
            for(i = oad; i < offset; i++)
            {
                switch(i)
                {
                case 0://总
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                case 1://尖
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                case 2://峰
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                case 3://平
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                case 4://谷
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                }
                p8[0] = (Data1 >> 63) & (0x01); //这一位表示有没有找到
                if(p8[0])
                {
                    Data1 -= ((u64)0x01 << 63);
                }
                if((p8[0]) && (Data1 != 0))
                {
                    Data1 >>= 24;
                    Data1 = Round_up_or_down(Data1);
                    Data1 = bcd16_hex (Data1);
                    p8Data[0] = DataType_double_long_unsigned;
                    p8Data[1] = Data1 >> 24;
                    p8Data[2] = Data1 >> 16;
                    p8Data[3] = Data1 >> 8;
                    p8Data[4] = Data1 >> 0;
                    p8Data += 5;
                    LEN_Data0 += 5;
                }
                else
                {
                    p8Data[0] = 0; //空
                    p8Data += 1;
                    LEN_Data0 += 1;
                }
            }

            p16Data[0] = LEN_Data0;
            break;
//				case 4://高精度总及费率电能量数组∷=array 高精度电能量
//				//高精度电能量：
//				//CHOICE
//				//{
//				//long64-unsigned        [21]，
//				//long64                 [20]
//				//}

//					break;
        default:
            return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
        }
        break;
    case 2://最大需量接口类,本接口类对象提供存储最大需量类信息
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //缓冲长度不够
                return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
        }

        switch((oad & 0xFF00) >> 8) //属性
        {
        case 2://总及费率最大需量数组∷=array 最大需量及发生时间
            //最大需量及发生时间∷=structure
            //{
            //  最大需量值 CHOICE，
            //  发生时间   date_time_s
            //}
            //最大需量值 ：
            //CHOICE
            //{
            //double-long            [5]，
            //double-long-unsigned   [6]
            //}
            switch(OI)
            {
            case 0x1010://正有功最大需量
                n = 0x80;//ABB电表中的正向有功的标识
                break;
            case 0x1020://反有功最大需量
                n = 0x40;//ABB电表中的正向有功的标识
                break;
            case 0x1030://组合无功1(正无功)最大需量
                n = 0x03;//ABB电表中的正向有功的标识
                break;
            case 0x1040://组合无功2(反无功)最大需量
                n = 0x0C;//ABB电表中的正向有功的标识
                break;
            default:
                n = 0x00;
                break;
            }

            oad &= 0xFF;
            if(0 == oad)
            {   //如果是总电量则循环5次
                offset = 5;
                if(LEN_RxDataBuff < (13 * 5))
                {   //缓冲长度不够
                    return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                }
                p8Array[0] = DataType_array;
                p8Array[1] = offset;
                LEN_Data0 = 2;
                LEN_RxDataBuff -= 2;
                p8Data += LEN_Data0;
            }
            else
            {   //如果是分费率电量则循环1次
                offset = oad ;
                oad -= 1;
                if(LEN_RxDataBuff < 13)
                {   //缓冲长度不够
                    return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                }
            }
            for(i = oad; i < offset; i++)
            {
                switch(i)
                {
                case 0://总
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                case 1://尖
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                case 2://峰
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                case 3://平
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                case 4://谷
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    break;
                }
                p8[0] = (Data1 >> 63) & (0x01); //这一位表示有没有找到
                if(p8[0])
                {
                    Data1 -= ((u64)0x01 << 63);
                }
                if((p8[0]) && (Data1 != 0))
                {
                    Data1 = Round_up_or_down(Data1);
                    Data1 = bcd16_hex (Data1);
                    p8Data[0] = DataType_structure;
                    p8Data[1] = 2;
                    p8Data += 2;
                    p8Data[0] = DataType_double_long_unsigned;
                    p8Data[1] = Data1 >> 24;
                    p8Data[2] = Data1 >> 16;
                    p8Data[3] = Data1 >> 8;
                    p8Data[4] = Data1 >> 0;
                    p8Data += 5;
                    //发生时间
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 10, 5); //0x01<<32位表示有没有找到，后32位是16进制的电量数据
                    p8[0] = Data1 & ((u64)0x01 << 63); //这一位表示有没有找到
                    if(p8)
                    {
                        Data1 -= ((u64)0x01 << 63);
                        p8Data[0] = DataType_date_time_s;
                        Data = Data1 & ((u64)0xFF << 32);
                        Data = bcd_hex(Data);
                        Data += 2000;
                        p8Data[1] = Data >> 8; //年
                        p8Data[2] = Data;
                        Data = Data1 & (0x7F << 24);
                        Data = bcd_hex(Data);
                        p8Data[3] = Data; //月
                        Data = Data1 & (0xFF << 16);
                        Data = bcd_hex(Data);
                        p8Data[4] = Data; //日
                        Data = Data1 & (0xFF << 8);
                        Data = bcd_hex(Data);
                        p8Data[5] = Data; //时
                        Data = Data1 & (0xFF);
                        Data = bcd_hex(Data);
                        p8Data[6] = Data; //分
                        p8Data[7] = 0;   //秒
                        p8Data += 8;
                        LEN_Data0 += 2 + 5 + 8;
                    }
                    else
                    {
                        p8Data[0] = 0; //空
                        p8Data += 1;
                        LEN_Data0 += 1;
                    }
                }
                else
                {
                    p8Data[0] = 0; //空
                    p8Data += 1;
                    LEN_Data0 += 1;
                }
            }
            p16Data[0] = LEN_Data0;
            break;
        default:
            return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
        }

        break;
    case 3://分相变量接口类,本接口类对象提供存储电压、电流、相角等分相变量类信息
        //分相数值组∷=array 分相数值
        //数值组按A相、B相、C相、N线顺序排列，其中：仅电流有N线，另外，当接线方式为单相时，A、B、C三相改为A相（某一相）。
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //缓冲长度不够
                return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
        }
        switch((oad & 0xFF00) >> 8) //属性
        {
        case 2://分相数值组
            switch(OI)
            {
            case 0x2000://电压
            case 0x2001://电流
            case 0x2002://电压相角
            case 0x2003://电压电流相角
            case 0x200B://电压波形失真度
            case 0x200C://电流波形失真度
                break;
            default:
                if(LEN_Data0 == 0)
                {
                    if(LEN_RxDataBuff < 2)
                    {   //缓冲长度不够
                        return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                    }
                    p8Array[0] = DataType_NULL;
                    LEN_Data0 = 1;
                    LEN_RxDataBuff -= 1;
                }
                p16Data[0] = LEN_Data0;
                return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }

            oad &= 0xFF;
            if(0 == oad)
            {   //
                offset = 3;
                if(LEN_RxDataBuff < (3 * 3))
                {   //缓冲长度不够
                    return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                }
                p8Array[0] = DataType_array;
                p8Array[1] = offset;
                LEN_Data0 = 2;
                LEN_RxDataBuff -= 2;
                p8Data += LEN_Data0;
            }
            else
            {   //
                offset = oad ;
                oad -= 1;
                if(LEN_RxDataBuff < 3)
                {   //缓冲长度不够
                    return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                }
            }
            for(i = oad; i < offset; i++)
            {
                p8Data[0] = DataType_NULL; //空
                p8Data += 1;
                LEN_Data0 += 1;
            }
            p16Data[0] = LEN_Data0;
            break;

        case 4:
            switch(OI)
            {
            case 0x2001://零线电流
                p8Data[0] = DataType_NULL; //空
                LEN_Data0 += 1;
                p16Data[0] = LEN_Data0;
                break;
            default:
                return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
        default:
            return 0;
        }
//			break;
    case 4://功率接口类,本接口类对象提供存储功率、功率因数等信息
        //总及分相数值组∷=array 总或分相数值
        //数值组按总、A相、B相、C相顺序排列，当接线方式为单相时，为总、A相（某一相）
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //缓冲长度不够
                return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
        }
        switch((oad & 0xFF00) >> 8) //属性
        {
        case 2://总及分相数值组
            switch(OI)
            {
            case 0x2004://有功功率
            case 0x2005://无功功率
            case 0x2006://视在功率
            case 0x2007://一分钟平均有功功率
            case 0x2008://一分钟平均无功功率
            case 0x2009://一分钟平均视在功率
            case 0x200A://功率因数
                break;
            default:
                if(LEN_Data0 == 0)
                {
                    if(LEN_RxDataBuff < 2)
                    {   //缓冲长度不够
                        return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                    }
                    p8Array[0] = DataType_NULL;
                    LEN_Data0 = 1;
                    LEN_RxDataBuff -= 1;
                }
                p16Data[0] = LEN_Data0;
                return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
            oad &= 0xFF;
            if(0 == oad)
            {   //
                offset = 4;
                if(LEN_RxDataBuff < (5 * 4))
                {   //缓冲长度不够
                    return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                }
                p8Array[0] = DataType_array;
                p8Array[1] = offset;
                LEN_Data0 = 2;
                LEN_RxDataBuff -= 2;
                p8Data += LEN_Data0;
            }
            else
            {   //
                offset = oad ;
                oad -= 1;
                if(LEN_RxDataBuff < 5)
                {   //缓冲长度不够
                    return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                }
            }
            for(i = oad; i < offset; i++)
            {
                p8Data[0] = 0; //空
                p8Data += 1;
                LEN_Data0 += 1;
            }

            p16Data[0] = LEN_Data0;
        default:
            return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
        }
//		case 5://谐波变量接口类,本接口类对象提供存储谐波变量类信息
//			break;
    case 6://数据变量接口类,本接口类对象提供存储过程值或与过程值单元相关的状态值信息
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //缓冲长度不够
                return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
        }
        switch((oad & 0xFF00) >> 8)
        {
        case 2:
            switch(OI)
            {
            case 0x2131:
            case 0x2132:
            case 0x2133:
            case 0x2014://电表运行状态字1-7
            case 0x202D://（当前）透支金额  数据类型：double-long-unsigned，单位：元，换算：-2
            default:
                if(LEN_Data0 == 0)
                {
                    if(LEN_RxDataBuff < 2)
                    {   //缓冲长度不够
                        return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
                    }
                    p8Array[0] = DataType_NULL;
                    LEN_Data0 = 1;
                    LEN_RxDataBuff -= 1;
                }
                p16Data[0] = LEN_Data0;
                return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
        default:
            return 0;
        }
//		case 7://事件对象接口类,本接口类对象提供配置、存储事件记录类信息
//			break;
    case 8://参数变量接口类,本接口类对象提供存储终端的各种参数类信息
        switch(OI)
        {
        case 0x2021://数据冻结时间
            if(LEN_RxDataBuff < 8)
            {   //缓冲长度不够
                return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
            p8Data += LEN_Data0;
            p8Array = (u8*)p8Data;
            p8Data[0] = DataType_date_time_s;
            Get_date_time_s(p8Data + 1); //得到date_time_s共7字节
            p8Data += 8;
            LEN_Data0 += 8;
            LEN_RxDataBuff -= 8;
            p16Data[0] = LEN_Data0;
            break;
        case 0x4000://日期时间
            if(LEN_RxDataBuff < 8)
            {   //缓冲长度不够
                return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
            p8Data += LEN_Data0;
            p8 = p8rx + ABB_SUB_OFFSET + ABB_RM_CLASS9_OFFSET + 27;
            if(p8)
            {
                for(i=0;i<6;i++)
                {
                    Data1 <<= 8;
                    Data1 += p8[i];
                }
                i = Check_YMDHMS(Data1); //BCD日期检查
                if(i==1)
                {
                    p8Array[0] = DataType_NULL;
                    LEN_Data0 = 1;
                    LEN_RxDataBuff -= 1;
                }
                else
                {
                    p8Data[0] = DataType_date_time_s;
                    Data = p8[0];
                    Data = bcd_hex(Data);
                    Data += 2000;
                    p8Data[1] = Data >> 8; //年
                    p8Data[2] = Data;
                    Data = p8[1];
                    Data = bcd_hex(Data);
                    p8Data[3] = Data; //月
                    Data = p8[2];
                    Data = bcd_hex(Data);
                    p8Data[4] = Data; //日
                    Data = p8[3];
                    Data = bcd_hex(Data);
                    p8Data[5] = Data; //时
                    Data = p8[4];
                    Data = bcd_hex(Data);
                    p8Data[6] = Data; //分
                    Data = p8[5];
                    Data = bcd_hex(Data);
                    p8Data[7] = Data; //秒
                    LEN_Data0 += 8;
                    LEN_RxDataBuff -= 6;
                }
            }
            else
            {
                p8Array[0] = DataType_NULL;
                LEN_Data0 = 1;
                LEN_RxDataBuff -= 1;
            }
            p16Data[0] = LEN_Data0;
            break;
        case 0x202C://（当前）钱包文件 剩余金额	double-long-unsigned（单位：元，换算：-2）， 购电次数	double-long-unsigned
            break;
        }
        break;
//		case 9://冻结数据接口类,本接口类对象提供配置、存储冻结数据及其相关信息
//			break;
//		case 10://采集监控接口类
//			break;
//		case 11://集合接口类,本接口类对象提供配置、存储终端采集数据及其相关信息
//			break;
//		case 12://脉冲计量接口类
//			break;
//		case 13://负荷控制对象接口类,本接口类对象提供负荷管理有关的控制功能
//			break;
//		case 14://区间统计接口类,本接口类对象提供依据配置参数判定、统计与越限相关的信息
//			break;
//		case 15://累加平均接口类,本接口类对象提供对相同物理属性的数值进行累加、平均的运算功能
//			break;
//		case 16://极值工具接口类,本接口类对象提供采集或生成最大、最小值及其发生时间
//			break;
//		case 17://显示接口类,本接口类对象提供与终端显示或对外打印相关的信息
//			break;
//		case 18://文件传输接口类,本接口类对象提供终端实现上传和下载文件的功能
//			break;
//		case 19://设备管理接口类,
//			break;
//		case 20://应用连接接口类
//			break;
//		case 21://ESAM接口类
//			break;
//		case 22://输入输出设备接口类
//			break;
//		case 23://总加组接口类
//			break;
//		case 24://分项事件对象接口类,本接口类对象提供配置、存储分项事件记录类信息
//			break;
//		case 25://无线公网通信接口类
//			break;
//		case 26://以太网通信接口类
//			break;
    default:
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //缓冲长度不够
                return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
            }
            p8Array[0] = DataType_NULL;
            LEN_Data0 = 1;
            LEN_RxDataBuff -= 1;
        }
        p16Data[0] = LEN_Data0;
        break;
        //测试看未处理的OI值
//        i = OI_IC(OI); //得到接口类号;入口:对象标识OI
//        if(i)
//        {
//            i = i;
//        }
//        return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
    }
    return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
}




















