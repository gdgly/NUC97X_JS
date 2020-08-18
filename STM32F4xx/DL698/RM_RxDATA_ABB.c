
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

//const u8 CONNECT_1[]=//��key
//{
//    0x02,0xFF,0x08,0x13,0xA1,0x08,0x00,0x00,0x10,0x62,0x41,0x15,0x86,0x4A,0xE1,
//};
//const u8 CONNECT_2[]=//Զ�̵�¼������֤����
//{
//    0x02 ,0x18 ,0x00 ,0xA3 ,0x82 ,0x23,
//};
//const u8 CONNECT_3[]=//��������С��λ
//{
//    0x02 ,0x05 ,0x00 ,0xA3 ,0xA8
//,0x00 ,0x02 ,0x00 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x0F ,0x0F
//,0x03 ,0x03
//,0xFF ,0x00 ,0x01 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00
//,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
//,0x00 ,0x00 ,0x92 ,0x95 ,0xC6,
//};
//const u8 CONNECT_4[]=//��11����������
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

u32 RM_Rx_ABB(u32 PORTn)//UART_645����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
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

    DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
    p0 = p16fifo[0]; //�жϽ���ָ��
    p1 = p16fifo[1]; //��ǰ�Ѷ�ָ��
    if((p0 >= (LEN_UARTnIntRx - 10)) || (p1 >= (LEN_UARTnIntRx - 10)))
    {
        p16fifo[0] = 0; //�жϽ���ָ��
        p16fifo[1] = 0; //��ǰ�Ѷ�ָ��
        p16fifo[2] = 0; //��Լ���ָ��
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
    	switch(UARTCtrl->CONNECT)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
    	{
            case 1://300bps����
            switch(p8[0])//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
    	{
    		case 1://300bps����
                MR((u32)p8rx,(u32)CONNECT_1,sizeof(CONNECT_1));
    			UARTCtrl->RxByte=sizeof(CONNECT_1);
    			return 1;//1=����OK
            case 2://��������У��
                MR((u32)p8rx,(u32)CONNECT_2,sizeof(CONNECT_2));
    			UARTCtrl->RxByte=sizeof(CONNECT_2);
    			return 1;//1=����OK
            case 3://����0����������С��λ��
                MR((u32)p8rx,(u32)CONNECT_3,sizeof(CONNECT_3));
    			UARTCtrl->RxByte=sizeof(CONNECT_3);
    			return 1;//1=����OK
            case 4://����2�����Ʒ��������ã�
                MR((u32)p8rx,(u32)CONNECT_4,sizeof(CONNECT_4));
    			UARTCtrl->RxByte=sizeof(CONNECT_4);
    			return 1;//1=����OK
            case 5://����9����ʱ�䣩
                MR((u32)p8rx,(u32)CONNECT_101_CLASS9_1,sizeof(CONNECT_101_CLASS9_1));
    			UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS9_1);
    			return 1;//1=����OK
            case 6://����11����������
                MR((u32)p8rx,(u32)CONNECT_101_CLASS9_1,sizeof(CONNECT_101_CLASS9_1));
    			UARTCtrl->RxByte=sizeof(CONNECT_101_CLASS9_1);
               if(UARTCtrl->RM_SubsequentFrames==0)//0=�޶�����֡,1-n=����֡��֡��
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
                return 1;//1=����OK
            }
           case 101://101=������
               return 1;
    		default:
    			return 1;
    	}

    //---TEST END---
    */

    RxByte = UARTCtrl->RxByte;
    if(p0 != p1)
    {   //�����ݽ������ó�ʱ��ʱ
        p16timer[0] = 500 / 10; //�ֽڼ䳬ʱ��ʱ
        if(PORTn == RS485_1PORT)
        {
            Terminal_Ram->RS485_1_RxLED_MS_Timer = 20 / 10; //11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
        }
        if(PORTn == RS485_2PORT)
        {
            Terminal_Ram->RS485_2_RxLED_MS_Timer = 20 / 10; //11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
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
            if(i != 0x02) //�ж���ʼ��02
            {
                RxByte = 0;
            }
            continue;
        }

        switch(p8[0])
        {
        case 0:
        case 1: //���ֱ���
            RxByte_Expect = 15;
            break;
        case 2: //����У��
            RxByte_Expect = 6;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            if((RxByte == 2) && (p8rxbuff[1] != 0x05) && (p8rxbuff[1] != 0x81))
            {   //�ж��������Ƿ���ȷ
                RxByte = 0;
                continue;
            }
            if(RxByte < 3)
            {
                continue;
            }
            RxByte_Expect = 6; //p8rxbuff[2]�������ͽ��յ����ֽڳ�����6,
            if(p8rxbuff[2] == 0x00)
            {   //ACK��ȷ
                if(RxByte >= 5)//�յ�֡�����ݳ���
                {
                    i = p8rxbuff[4] & 0x7F;
                    RxByte_Expect = i + 7;
                }
            }
            break;
        }
        if(RxByte >= RxByte_Expect)//�������ݳ����㹻��CRCУ���ж�
        {
            Pn = ABB_CRC((u8 *)&p8rxbuff[0], (RxByte_Expect - 2));
            i = (p8rxbuff[RxByte_Expect - 2] << 8) + (p8rxbuff[RxByte_Expect - 1]);
            if(Pn == i)
            {   //У����ȷ
                p16fifo[1] = p1; //��ǰ�Ѷ�ָ��
                p16fifo[2] = p1; //���ָ��==��ָ��,���ʡʱ
                UARTCtrl->RxByte = RxByte;
                return 1;
            }
            else
            {   //У�����
                RxByte = 0;
            }
        }
    }

    p16fifo[1] = p1;
    UARTCtrl->RxByte = RxByte;
    if(p16timer[0] == 0x0)
    {   //��ʱ
        return 2;//2=���ճ�ʱ
    }
    return 0;
}

u32 RM_RxDATA_ABB_CHECK(u32 PORTn)//Landis֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
{
    return 0;
}
//p8rx:������ʼ��ַ��Block����11block��������ݣ�Rate:�ܼ��ƽ�ȷ���;
//Offset:����ڵ�����ƫ����,length:���ݳ���
//���أ�bit63���Ƿ��ҵ���bit62-bit0���������ݡ�
u64 RM_RxDATA_ABB_Electric_Power(u8* p8rx, u32 Block, u8 Rate, u8 Offset, u8 Length) //
{
    u8 i;
    u32 Data;
    u64 Data64[5] = {0};
    u8* p8;
//��ʼ---�洢��ͨ��ͨ���ڶ���������
//2���ֽ�  ��11������ʼ�Ļ���ƫ�������Ѿ��̶�Ϊ16�ˣ�����ʱû��
//2���ֽ�  ��11�����Ѿ�д������ĳ��ȣ�����֡p[4]���ۼ�,16������
//4���ֽ�  KEY�Ļ��壬4���ֽڵĳ���
//6���ֽ�  ����������С���㣬��11��BLOCK1 ��BLOCK2��BLOCK3 ��BLOCK4������
//��16���ֽ�  �洢������
    p8 = p8rx + ABB_SUB_OFFSET + ABB_CLASS11_BLOCK1; //��ȡ���ݵ�BLOCK����ֵ��ַ
    Data = (*(p8) << 24) + (*(p8 + 1) << 16) + (*(p8 + 2) << 8) + (*(p8 + 3));
    for(i = 0; i < 4; i++) //��ȡ���ջ��������ƫ����i
    {
        if(Block == (Data & 0xFF))
        {
            i = 84 * (3 - i);
            break;
        }
        Data >>= 8;
    }
    if(i == 4)
    {   //û���ҵ���Ӧ�ĵ��������
        return Data64[0] = 0;
    }
    p8 = p8rx + ABB_SUB_OFFSET + ABB_RM_CLASS11_OFFSET + i; //��֡�����׵�ַ+i(ƫ��)
    for(i = 0; i < Length; i++) //ɸѡ����������Ч���ֽڣ����硰04 44 24 85 56 00 00��ѡ�� ��04 44 24 85 56��
    {
        Data64[2] = ((Data64[2] << 8) + * (p8 + Offset + i)); //�壻Data1��8���ֽڣ���Ϊ�˷�ֹ�������
        Data64[3] = ((Data64[3] << 8) + * (p8 + 21 + Offset + i)); //ƽ
        Data64[4] = ((Data64[4] << 8) + * (p8 + 42 + Offset + i)); //��
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
//�������뺯��
//��������BCD���һλ��������10(BCD������4λ)
u64 Round_up_or_down(u64 n)
{
    if((n & 0x0F) >= 5) //ĩλ����5���һ
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
//UART�������ݻ��嶨��
//#define LEN_RM_DI_CSD     512
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)

//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)

//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�����ݳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�����ݳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������)(ROADʱ��д���ܹ�����OAD��������!!!)

u32 RM_RxDATA_ABB(u32 PORTn)//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
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
    p8rx = (u8 *)Get_ADDR_UARTnRx(PORTn); ////�õ�UART���ջ����ַ

    LEN_RxDataBuff = Get_LEN_UARTnFnDataBuff(PORTn); //�õ�UART�������ݻ��峤��
    LEN_RxDataBuff -= (LEN_RM_DI_CSD + 4);
    p8Data = (u8 *)Get_ADDR_UARTnFnDataBuff(PORTn); //�õ�UART�������ݻ����ַ
    p16Data = (u16*)(p8Data + LEN_RM_DI_CSD);
    LEN_Data1 = p16Data[1];
    if(LEN_RxDataBuff < LEN_Data1)
    {   //��
        return 3;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
    }
    LEN_RxDataBuff -= LEN_Data1;
    p8Data += LEN_RM_DI_CSD + 4 + LEN_Data1;
    p8Array = (u8*)p8Data;
    if(UARTCtrl->RMCSDSubCount == 0) //����CSD�Ӽ���(��DL698����ʱ)
    {
        p16Data[0] = 0; //����д��buff����û����һ��CSD�����ݳ���(����������2�ֽ�)
    }
    LEN_Data0 = p16Data[0];
    if(LEN_RxDataBuff < LEN_Data0)
    {   //��
        return 1;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
    }
    LEN_RxDataBuff -= LEN_Data0;
    p8 = p8rx + LEN_UART2Rx + LEN_UART2Tx; //�����յ��ĺ���֡���飬�ŵ��ڶ���������,ǰ4���ֽ�������ʾһЩ��־
    switch(UARTCtrl->CONNECT)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
    {
    case 1://300bps����
        switch(p8[0])
        {
        case 0:
        case 1:
            MR((u32)(p8 + ABB_KEY), (u32)(p8rx + 9), 4); //�洢����
            p8[0] = 2;
            return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
        case 2://��������У��
            p8[0] = 3;
            return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
        case 3://����0����������С��λ��
            MR((u32)(p8 + ABB_POWER_POINT), (u32)(p8rx + 16), 2); //����С��λ������С��λ
            p8[0] = 4;
            return 0;
        case 4://����2�����Ʒ��������ã�
            MR((u32)(p8 + ABB_CLASS11_BLOCK1), (u32)(p8rx + 8), 2); //��11��BLOCK1 ����11��BLOCK2
            MR((u32)(p8 + ABB_CLASS11_BLOCK3), (u32)(p8rx + 8 + 2 + 29), 2); //��11��BLOCK3 ����11��BLOCK4
            p8[0] = 5;
            return 0;
        case 5://����9��ʱ��
            i = p8rx[4] & 0x7F; //���ڴ����֡�ĳ���
            MR((u32)(p8 + ABB_RM_CLASS9_OFFSET), (u32)(p8rx + 5), i); //ʱ��洢��RS485��RAM����
            p8[0] = 6;
            return 0;
        case 6://����11ʵʱ����
            i = p8rx[4] & 0x7F; //���ڴ����֡�ĳ���
            offset = (p8[ABB_LEN_CLASS11] << 8) + (p8[ABB_LEN_CLASS11 + 1]);
            MR((u32)(p8 + ABB_RM_CLASS11_OFFSET + offset), (u32)(p8rx + 5), i); //�洢��RS485��RAM����
            i += offset;
            if(i > ABB_LENMAX_POWER_CLASS11)
            {   //������յ��ĵ������ݳ����Ѿ���������ֵ��0x016E���ˣ�˵�����̻����ݳ�����ȥ����ֻ���ˣ�����������Ҳ������
                UARTCtrl->CONNECT = 102;
                p8[0] = 0;
            }
            p8[ABB_LEN_CLASS11] = (i >> 8) & 0xFF;
            p8[ABB_LEN_CLASS11 + 1] =  i & 0xFF;
            if((p8rx[4] & 0x80) == 0) //bit7Ϊ0�к���֡
            {
                UARTCtrl->RM_SubsequentFrames = 1; //�к���֡
            }
            else
            {
                UARTCtrl->RM_SubsequentFrames = 0; //�޺���֡
                UARTCtrl->CONNECT = 100;
                p8[0] = 0;
            }
            return 0;
        }
    case 101://101=������

        break;
    case 102://�ر�����
        return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
    default://698��Լ���Ӻ͹رչ���
        UARTCtrl->CONNECT = 0; //DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
        return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
    }

//---����֡---

//---����֡����---


//---���߿�ʼ---

//---���߽���---


//---�¼��࿪ʼ---


//---�¼������---

    i = UARTCtrl->RMCSD[1]; //��ǰ���ڳ���CSD(����������)
    if(i == 0)
    {   //OAD
        OI = UARTCtrl->RMCSD[2] << 8; //��ǰ���ڳ���CSD
        OI |= UARTCtrl->RMCSD[3];
        oad = (UARTCtrl->RMCSD[4] << 8) + (UARTCtrl->RMCSD[5]);
    }
    else
    {   //ROAD
        i = UARTCtrl->RMROADCount; //����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
        i *= 4;
        OI = UARTCtrl->RMCSD[7 + i] << 8; //��ǰ���ڳ���CSD
        OI |= UARTCtrl->RMCSD[7 + i + 1];
        oad = (UARTCtrl->RMCSD[7 + i + 2] << 8) + (UARTCtrl->RMCSD[7 + i + 3]);
    }
    i = OI_IC(OI); //�õ��ӿ����;���:�����ʶOI
    switch(i)
    {
    case 1://�������ӿ���,���ӿ�������ṩ�洢����������Ϣ
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //���峤�Ȳ���
                return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
        }
        switch((oad & 0xFF00) >> 8)
        {
        case 2://�ܼ����ʵ����������=array ������
            //��������
            //CHOICE
            //{
            //double-long-unsigned   [6]��
            //double-long            [5]
            //}
            switch(OI)
            {
            case 0x0010://���й�
                n = 0x80;//ABB����е������й��ı�ʶ
                break;
            case 0x0020://���й�
                n = 0x40; //ABB����еķ����й��ı�ʶ
                break;
            case 0x0030://����޹�1(���޹�)
                n = 0x03;   //ABB����е�����޹�1�ı�ʶ
                break;
            case 0x0040://����޹�2(���޹�)
                n = 0x0C; //ABB����е�����޹�2(���޹�)�ı�ʶ
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
//                return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                break;
            }

            oad &= 0xFF;
            if(0 == oad)
            {   //������ܵ�����ѭ��5��
                offset = 5;
                if(LEN_RxDataBuff < (5 * 5))
                {   //���峤�Ȳ���
                    return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                }
                p8Array[0] = DataType_array;
                p8Array[1] = offset;
                LEN_Data0 = 2;
                LEN_RxDataBuff -= 2;
                p8Data += LEN_Data0;
            }
            else
            {   //����Ƿַ��ʵ�����ѭ��1��
                offset = oad ;
                oad -= 1;
                if(LEN_RxDataBuff < 5)
                {   //���峤�Ȳ���
                    return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                }
            }
            for(i = oad; i < offset; i++)
            {
                switch(i)
                {
                case 0://��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                case 1://��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                case 2://��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                case 3://ƽ
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                case 4://��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 0, 7); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                }
                p8[0] = (Data1 >> 63) & (0x01); //��һλ��ʾ��û���ҵ�
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
                    p8Data[0] = 0; //��
                    p8Data += 1;
                    LEN_Data0 += 1;
                }
            }

            p16Data[0] = LEN_Data0;
            break;
//				case 4://�߾����ܼ����ʵ����������=array �߾��ȵ�����
//				//�߾��ȵ�������
//				//CHOICE
//				//{
//				//long64-unsigned        [21]��
//				//long64                 [20]
//				//}

//					break;
        default:
            return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
        }
        break;
    case 2://��������ӿ���,���ӿ�������ṩ�洢�����������Ϣ
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //���峤�Ȳ���
                return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
        }

        switch((oad & 0xFF00) >> 8) //����
        {
        case 2://�ܼ�����������������=array �������������ʱ��
            //�������������ʱ���=structure
            //{
            //  �������ֵ CHOICE��
            //  ����ʱ��   date_time_s
            //}
            //�������ֵ ��
            //CHOICE
            //{
            //double-long            [5]��
            //double-long-unsigned   [6]
            //}
            switch(OI)
            {
            case 0x1010://���й��������
                n = 0x80;//ABB����е������й��ı�ʶ
                break;
            case 0x1020://���й��������
                n = 0x40;//ABB����е������й��ı�ʶ
                break;
            case 0x1030://����޹�1(���޹�)�������
                n = 0x03;//ABB����е������й��ı�ʶ
                break;
            case 0x1040://����޹�2(���޹�)�������
                n = 0x0C;//ABB����е������й��ı�ʶ
                break;
            default:
                n = 0x00;
                break;
            }

            oad &= 0xFF;
            if(0 == oad)
            {   //������ܵ�����ѭ��5��
                offset = 5;
                if(LEN_RxDataBuff < (13 * 5))
                {   //���峤�Ȳ���
                    return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                }
                p8Array[0] = DataType_array;
                p8Array[1] = offset;
                LEN_Data0 = 2;
                LEN_RxDataBuff -= 2;
                p8Data += LEN_Data0;
            }
            else
            {   //����Ƿַ��ʵ�����ѭ��1��
                offset = oad ;
                oad -= 1;
                if(LEN_RxDataBuff < 13)
                {   //���峤�Ȳ���
                    return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                }
            }
            for(i = oad; i < offset; i++)
            {
                switch(i)
                {
                case 0://��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                case 1://��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                case 2://��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                case 3://ƽ
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                case 4://��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 7, 3); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    break;
                }
                p8[0] = (Data1 >> 63) & (0x01); //��һλ��ʾ��û���ҵ�
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
                    //����ʱ��
                    Data1 = RM_RxDATA_ABB_Electric_Power(p8rx, n, i, 10, 5); //0x01<<32λ��ʾ��û���ҵ�����32λ��16���Ƶĵ�������
                    p8[0] = Data1 & ((u64)0x01 << 63); //��һλ��ʾ��û���ҵ�
                    if(p8)
                    {
                        Data1 -= ((u64)0x01 << 63);
                        p8Data[0] = DataType_date_time_s;
                        Data = Data1 & ((u64)0xFF << 32);
                        Data = bcd_hex(Data);
                        Data += 2000;
                        p8Data[1] = Data >> 8; //��
                        p8Data[2] = Data;
                        Data = Data1 & (0x7F << 24);
                        Data = bcd_hex(Data);
                        p8Data[3] = Data; //��
                        Data = Data1 & (0xFF << 16);
                        Data = bcd_hex(Data);
                        p8Data[4] = Data; //��
                        Data = Data1 & (0xFF << 8);
                        Data = bcd_hex(Data);
                        p8Data[5] = Data; //ʱ
                        Data = Data1 & (0xFF);
                        Data = bcd_hex(Data);
                        p8Data[6] = Data; //��
                        p8Data[7] = 0;   //��
                        p8Data += 8;
                        LEN_Data0 += 2 + 5 + 8;
                    }
                    else
                    {
                        p8Data[0] = 0; //��
                        p8Data += 1;
                        LEN_Data0 += 1;
                    }
                }
                else
                {
                    p8Data[0] = 0; //��
                    p8Data += 1;
                    LEN_Data0 += 1;
                }
            }
            p16Data[0] = LEN_Data0;
            break;
        default:
            return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
        }

        break;
    case 3://��������ӿ���,���ӿ�������ṩ�洢��ѹ����������ǵȷ����������Ϣ
        //������ֵ���=array ������ֵ
        //��ֵ�鰴A�ࡢB�ࡢC�ࡢN��˳�����У����У���������N�ߣ����⣬�����߷�ʽΪ����ʱ��A��B��C�����ΪA�ࣨĳһ�ࣩ��
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //���峤�Ȳ���
                return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
        }
        switch((oad & 0xFF00) >> 8) //����
        {
        case 2://������ֵ��
            switch(OI)
            {
            case 0x2000://��ѹ
            case 0x2001://����
            case 0x2002://��ѹ���
            case 0x2003://��ѹ�������
            case 0x200B://��ѹ����ʧ���
            case 0x200C://��������ʧ���
                break;
            default:
                if(LEN_Data0 == 0)
                {
                    if(LEN_RxDataBuff < 2)
                    {   //���峤�Ȳ���
                        return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                    }
                    p8Array[0] = DataType_NULL;
                    LEN_Data0 = 1;
                    LEN_RxDataBuff -= 1;
                }
                p16Data[0] = LEN_Data0;
                return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }

            oad &= 0xFF;
            if(0 == oad)
            {   //
                offset = 3;
                if(LEN_RxDataBuff < (3 * 3))
                {   //���峤�Ȳ���
                    return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
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
                {   //���峤�Ȳ���
                    return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                }
            }
            for(i = oad; i < offset; i++)
            {
                p8Data[0] = DataType_NULL; //��
                p8Data += 1;
                LEN_Data0 += 1;
            }
            p16Data[0] = LEN_Data0;
            break;

        case 4:
            switch(OI)
            {
            case 0x2001://���ߵ���
                p8Data[0] = DataType_NULL; //��
                LEN_Data0 += 1;
                p16Data[0] = LEN_Data0;
                break;
            default:
                return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
        default:
            return 0;
        }
//			break;
    case 4://���ʽӿ���,���ӿ�������ṩ�洢���ʡ�������������Ϣ
        //�ܼ�������ֵ���=array �ܻ������ֵ
        //��ֵ�鰴�ܡ�A�ࡢB�ࡢC��˳�����У������߷�ʽΪ����ʱ��Ϊ�ܡ�A�ࣨĳһ�ࣩ
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //���峤�Ȳ���
                return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
        }
        switch((oad & 0xFF00) >> 8) //����
        {
        case 2://�ܼ�������ֵ��
            switch(OI)
            {
            case 0x2004://�й�����
            case 0x2005://�޹�����
            case 0x2006://���ڹ���
            case 0x2007://һ����ƽ���й�����
            case 0x2008://һ����ƽ���޹�����
            case 0x2009://һ����ƽ�����ڹ���
            case 0x200A://��������
                break;
            default:
                if(LEN_Data0 == 0)
                {
                    if(LEN_RxDataBuff < 2)
                    {   //���峤�Ȳ���
                        return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                    }
                    p8Array[0] = DataType_NULL;
                    LEN_Data0 = 1;
                    LEN_RxDataBuff -= 1;
                }
                p16Data[0] = LEN_Data0;
                return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
            oad &= 0xFF;
            if(0 == oad)
            {   //
                offset = 4;
                if(LEN_RxDataBuff < (5 * 4))
                {   //���峤�Ȳ���
                    return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
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
                {   //���峤�Ȳ���
                    return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                }
            }
            for(i = oad; i < offset; i++)
            {
                p8Data[0] = 0; //��
                p8Data += 1;
                LEN_Data0 += 1;
            }

            p16Data[0] = LEN_Data0;
        default:
            return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
        }
//		case 5://г�������ӿ���,���ӿ�������ṩ�洢г����������Ϣ
//			break;
    case 6://���ݱ����ӿ���,���ӿ�������ṩ�洢����ֵ�������ֵ��Ԫ��ص�״ֵ̬��Ϣ
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //���峤�Ȳ���
                return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
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
            case 0x2014://�������״̬��1-7
            case 0x202D://����ǰ��͸֧���  �������ͣ�double-long-unsigned����λ��Ԫ�����㣺-2
            default:
                if(LEN_Data0 == 0)
                {
                    if(LEN_RxDataBuff < 2)
                    {   //���峤�Ȳ���
                        return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
                    }
                    p8Array[0] = DataType_NULL;
                    LEN_Data0 = 1;
                    LEN_RxDataBuff -= 1;
                }
                p16Data[0] = LEN_Data0;
                return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
        default:
            return 0;
        }
//		case 7://�¼�����ӿ���,���ӿ�������ṩ���á��洢�¼���¼����Ϣ
//			break;
    case 8://���������ӿ���,���ӿ�������ṩ�洢�ն˵ĸ��ֲ�������Ϣ
        switch(OI)
        {
        case 0x2021://���ݶ���ʱ��
            if(LEN_RxDataBuff < 8)
            {   //���峤�Ȳ���
                return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
            p8Data += LEN_Data0;
            p8Array = (u8*)p8Data;
            p8Data[0] = DataType_date_time_s;
            Get_date_time_s(p8Data + 1); //�õ�date_time_s��7�ֽ�
            p8Data += 8;
            LEN_Data0 += 8;
            LEN_RxDataBuff -= 8;
            p16Data[0] = LEN_Data0;
            break;
        case 0x4000://����ʱ��
            if(LEN_RxDataBuff < 8)
            {   //���峤�Ȳ���
                return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
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
                i = Check_YMDHMS(Data1); //BCD���ڼ��
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
                    p8Data[1] = Data >> 8; //��
                    p8Data[2] = Data;
                    Data = p8[1];
                    Data = bcd_hex(Data);
                    p8Data[3] = Data; //��
                    Data = p8[2];
                    Data = bcd_hex(Data);
                    p8Data[4] = Data; //��
                    Data = p8[3];
                    Data = bcd_hex(Data);
                    p8Data[5] = Data; //ʱ
                    Data = p8[4];
                    Data = bcd_hex(Data);
                    p8Data[6] = Data; //��
                    Data = p8[5];
                    Data = bcd_hex(Data);
                    p8Data[7] = Data; //��
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
        case 0x202C://����ǰ��Ǯ���ļ� ʣ����	double-long-unsigned����λ��Ԫ�����㣺-2���� �������	double-long-unsigned
            break;
        }
        break;
//		case 9://�������ݽӿ���,���ӿ�������ṩ���á��洢�������ݼ��������Ϣ
//			break;
//		case 10://�ɼ���ؽӿ���
//			break;
//		case 11://���Ͻӿ���,���ӿ�������ṩ���á��洢�ն˲ɼ����ݼ��������Ϣ
//			break;
//		case 12://��������ӿ���
//			break;
//		case 13://���ɿ��ƶ���ӿ���,���ӿ�������ṩ���ɹ����йصĿ��ƹ���
//			break;
//		case 14://����ͳ�ƽӿ���,���ӿ�������ṩ�������ò����ж���ͳ����Խ����ص���Ϣ
//			break;
//		case 15://�ۼ�ƽ���ӿ���,���ӿ�������ṩ����ͬ�������Ե���ֵ�����ۼӡ�ƽ�������㹦��
//			break;
//		case 16://��ֵ���߽ӿ���,���ӿ�������ṩ�ɼ������������Сֵ���䷢��ʱ��
//			break;
//		case 17://��ʾ�ӿ���,���ӿ�������ṩ���ն���ʾ������ӡ��ص���Ϣ
//			break;
//		case 18://�ļ�����ӿ���,���ӿ�������ṩ�ն�ʵ���ϴ��������ļ��Ĺ���
//			break;
//		case 19://�豸����ӿ���,
//			break;
//		case 20://Ӧ�����ӽӿ���
//			break;
//		case 21://ESAM�ӿ���
//			break;
//		case 22://��������豸�ӿ���
//			break;
//		case 23://�ܼ���ӿ���
//			break;
//		case 24://�����¼�����ӿ���,���ӿ�������ṩ���á��洢�����¼���¼����Ϣ
//			break;
//		case 25://���߹���ͨ�Žӿ���
//			break;
//		case 26://��̫��ͨ�Žӿ���
//			break;
    default:
        if(LEN_Data0 == 0)
        {
            if(LEN_RxDataBuff < 2)
            {   //���峤�Ȳ���
                return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
            }
            p8Array[0] = DataType_NULL;
            LEN_Data0 = 1;
            LEN_RxDataBuff -= 1;
        }
        p16Data[0] = LEN_Data0;
        break;
        //���Կ�δ�����OIֵ
//        i = OI_IC(OI); //�õ��ӿ����;���:�����ʶOI
//        if(i)
//        {
//            i = i;
//        }
//        return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
    }
    return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
}




















