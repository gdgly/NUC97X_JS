
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"
#include "../DL698/RM_TxRxDATA_ABB.h"




u32 ABB_CRC(u8* p8, u32 nByte) //ABB��CRC����
{
    u32 i;
    u32 j;
    u32 iTemp;
    u32 flag;

    iTemp = 0;
    for(i = 0; i < nByte; i++)
    {
        iTemp ^= (p8[i] << 8);
        for(j = 0; j < 8; j++)
        {
            flag = iTemp & 0x8000;
            iTemp <<= 1;
            if(flag)
            {
                iTemp ^= 0x1021;
            }
        }
    }
    iTemp &= 0xffff;
    return iTemp;
}

u32 ABB_Password(u32 key, u32 pword) //�����ܳ�key��Զ��ͨѶ������������ܿ�����
{
    s32 i; // loop index
    s32 j, k = 0; // used to simulate rotate
    union
    {
        unsigned long key ; // encryption key
        struct
        {   /* broken into bytes*/
            unsigned char byta, bytb, bytc, bytd;
        } parts;
    } val;
    val.key = key;

// Add an arbitrary number to the key just for fun.
    val.key += 0xab41;
// Generate a four bit checksum to be used as loop index.
    i = val.parts.byta + val.parts.bytb + val.parts.bytc + val.parts.bytd;
    i = i & 0x0f;
    while(i >= 0)
    {
// Set 'j' to the value of the high bit before shifting.
//Simulates carry flag. */
        if(val.parts.bytd >= 0x80)
        {
            j = 1;
        }
        else
        {
            j = 0;
        }
// Shift the key. Add in the carry flag from the previous loop. */
        val.key = val.key << 1;
        val.key += k;
        k = j;
// Apply the key to the password. */
        pword ^= val.key;
        i--;
    }
    return pword;
}



u32 Calculate_ABBkey(u32 PORTn, u8 *p8) //����ABBͨѶ����
{
    u32 returnedKey, CalculatedKey, RemotePwd;
    u16 i, x;
    u8 * p8_pwd;
    UARTCtrl_TypeDef * UARTCtrl;
    u16 pAddr[3];

    UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
    pAddr[0] = UARTCtrl->RMAddr[0];
    pAddr[1] = UARTCtrl->RMAddr[1];
    pAddr[2] = UARTCtrl->RMAddr[2];
    x = AddrToMeterSetNo(pAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч

    p8_pwd = (u8*)ADDR_6000_SDRAM ;
    p8_pwd += (x * LENper_6000) + 26;
    x = p8_pwd[0];
    RemotePwd = 0;
    for(i = 1; i <= x; i++)
    {
        RemotePwd = (RemotePwd << 8) + p8_pwd[i];
    }
    p8 += ABB_KEY;
    returnedKey = ((*(p8 + 0) << 24) & 0xff000000) + ((*(p8 + 1) << 16) & 0x00ff0000)
                  + ((*(p8 + 2) << 8)  & 0x0000ff00) + ((*(p8 + 3))     & 0x000000ff);
    CalculatedKey = ABB_Password(returnedKey, RemotePwd);
    return CalculatedKey;

}
//����p8CSD,����Ҫ����ABB�ࡣ
//����abb_class,���abb_class Ϊ0xEE,��û���ҵ�Ҫ�����ࡣ
u32 transform_OOP_into_ABB(u8 *p8CSD, u32 PORTn)
{

    u32 i, OI, abb_class;
    UARTCtrl_TypeDef * UARTCtrl;
    UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
    i = UARTCtrl->RMCSD[1];;//��ǰ���ڳ���CSD(����������)
    if(i == 0)
    {   //OAD
        OI  = UARTCtrl->RMCSD[2]  << 24; //��ǰ���ڳ���CSD
        OI |= UARTCtrl->RMCSD[2 + 1] << 16;
        OI |= UARTCtrl->RMCSD[2 + 2] << 8 ;
        OI |= UARTCtrl->RMCSD[2 + 3]    ;
    }
    else
    {   //ROAD
        i = UARTCtrl->RMROADCount; //����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
        i *= 4;
        OI  = UARTCtrl->RMCSD[7 + i]  << 24; //��ǰ���ڳ���CSD
        OI |= UARTCtrl->RMCSD[7 + i + 1] << 16;
        OI |= UARTCtrl->RMCSD[7 + i + 2] << 8 ;
        OI |= UARTCtrl->RMCSD[7 + i + 3]    ;
        UARTCtrl->RMROADCountMax = UARTCtrl->RMCSD[6];
    }
    switch(OI)
    {
    case 0x20210200:            // ����ʱ�䣬����ʱ���ñ���ʱ��������
    case 0x40000200:            // ����ʱ��
        abb_class = 9;
        break;
    case 0x00100200:            //2 �����й�����
    case 0x00100201:            //  �����й��ܵ���
    case 0x00200200:            //3 �����й�����
    case 0x00200201:            //  �����й��ܵ���
    case 0x00300200:            //4 ����޹�1����
    case 0x00400200:            //  ����޹�2����
    case 0x10100200:            //10 �����й��������������ʱ��
        abb_class = 11;
        break;
    default:
        abb_class = 0xEE;
        break;

    }
    return abb_class;

}
u32 RM_TxDATA_ABB_Class(u32 Class, u32 PORTn) //��ABB��ͬ����
{
    u32 LEN_DATA = 0;
    u8 *p8;
    p8 = (u8 *)Get_ADDR_UARTnTx(PORTn);
    switch(Class)//
    {
    case 0://����0����С����
        p8[0] = 0x02;
        p8[1] = 0x05;
        p8[2] = 0x00;
        p8[3] = 0x00;
        p8[4] = 0x00;
        p8[5] = 0x00;
        p8[6] = 0x00;
        p8[7] = 0x00;
        p8[8] = 0xF6;
        p8[9] = 0x01;
        LEN_DATA = 10;
        break;
    case 2://����2�Ʒ��������ã�����������Ҫ�����ݵĳ���40��ƫ�Ƶ�ַ50
        p8[0] = 0x02;
        p8[1] = 0x05;
        p8[2] = 0x00;
        p8[3] = 0x00;
        p8[4] = 0x28;
        p8[5] = 0x00;
        p8[6] = 0x32;
        p8[7] = 0x02;
        p8[8] = 0x07;
        p8[9] = 0x39;
        LEN_DATA = 10;
        break;
    case 9://����9״̬1��ʱ��Ȳ���
        p8[0] = 0x02;
        p8[1] = 0x05;
        p8[2] = 0x00;
        p8[3] = 0x00;
        p8[4] = 0x00;
        p8[5] = 0x00;
        p8[6] = 0x00;
        p8[7] = 0x09;
        p8[8] = 0x67;
        p8[9] = 0x28;
        LEN_DATA = 10;
        break;
    case 11://����11��ǰ�Ʒ�����
        p8[0] = 0x02;
        p8[1] = 0x05;
        p8[2] = 0x00;
        p8[3] = 0x00;
        p8[4] = 0x00;
        p8[5] = 0x00;
        p8[6] = 0x00;
        p8[7] = 0x0B;
        p8[8] = 0x47;
        p8[9] = 0x6A;
        LEN_DATA = 10;
        break;
    default:
//���ABB��û��698�����ݱ�ʶ��Ӧ�á�LEN_DATA = 0����
//����Ϊ�˽��롰Err=RM_RxDATA_ABB(PORTn)�����ݽ��մ�������̣��˴� ��LEN_DATA = 1;��
//ʵ���ϲ���ȥ��������
        LEN_DATA = 1;
        break;
    }
    return LEN_DATA;
}
u32 RM_TxDATA_ABB(u32 PORTn, u8 *p8CSD, u8 *p8tx) //������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
{
    u32 i;
    u32 x;
    u32 LEN_DATA, CRCCode;
    u8 abb_class;
    u8 *p8;
    UARTCtrl_TypeDef * UARTCtrl;

    UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
    p8tx = (u8 *)Get_ADDR_UARTnTx(PORTn);
    p8 = (u8 *)Get_ADDR_UARTnRx(PORTn);
    LEN_DATA = 0;
    UARTCtrl->BpsCtrl = //ͨ�ſ�����(ͬ��Լ2007)(���ֶ���)
        (2 << 5) | //D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
        (0 << 4) | //D4λ��ʾֹͣλλ��0/1�ֱ��ʾ1/2��ֹͣλ
        (0 << 3) | //D3λ��ʾ����У��λ0/1�ֱ��ʾ��/��
        //D2λ��ʾż����У��λ0/1�ֱ��ʾż����У��
        3;        //D1-D0 ����λ��0��3�ֱ��ʾ5-8λ
    //1����ʼλ,8������λ��1��ֹͣλ����У�� 1200������
    switch(UARTCtrl->CONNECT)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
    {
    case 1://300bps���� //{0x02,0x18,0x06,0x00,0x01,0x01,0x98,0x9e};
        p8 += ABB_SUB_OFFSET;
        switch(p8[0])
        {
        case 0:
        case 1:
            MC(0, (u32)p8, LEN_UART2Rx / 2);
            p8[0] = 1;

            p8tx[0] = 0x02;
            p8tx[1] = 0x18;
            p8tx[2] = 0x06;//0x01 �����⣬0x02 ����ʱ�䣻0x06 ����
            p8tx[3] = 0x00;
            p8tx[4] = 0x01;
            LEN_DATA = 5;
            x = UARTCtrl->RMAddr[2];   //�����ַ��1234 5678 9012��RMAddr[0]=0x3412, RMAddr[1]=0x7856,RMAddr[2]=0x1290,
            //ABB��ĵ�ַ��16���ƣ�����BCD�룬��48Ӧ����30
            p8tx[LEN_DATA] = (x & 0x0F) * 100;
            p8tx[LEN_DATA] += ((x & 0xF000) >> 12) * 10;
            p8tx[LEN_DATA] += (x & 0x0F00) >> 8;
            LEN_DATA++;
            CRCCode = ABB_CRC(p8tx, LEN_DATA);
            p8tx[LEN_DATA] = (CRCCode >> 8) & 0x00FF;
            p8tx[LEN_DATA + 1] =  CRCCode & 0x00FF;
            LEN_DATA += 2;//8���ֽ�
            break;
        case 2://��������У��
            p8tx[0] = 0x02;
            p8tx[1] = 0x18;
            p8tx[2] = 0x01; //0x01 �����⣬0x02 ����ʱ�䣻0x06 ����
            p8tx[3] = 0x00;
            p8tx[4] = 0x04;
            x = Calculate_ABBkey(PORTn, p8);//����ABBԶ��ͨѶ��Կ
            p8tx[5] = (x >> 24) & 0x00000000ff;
            p8tx[6] = (x >> 16) & 0x00000000ff;
            p8tx[7] = (x >> 8)  & 0x00000000ff;
            p8tx[8] = (x)     & 0x00000000ff;
            LEN_DATA = 9;
            CRCCode = ABB_CRC(p8tx, LEN_DATA);
            p8tx[LEN_DATA] = (CRCCode >> 8) & 0x00FF;
            p8tx[LEN_DATA + 1] =  CRCCode & 0x00FF;
            LEN_DATA += 2;
            break;
        case 3: //����0����������С��λ��
            abb_class = 0;
            LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
            break;
        case 4: //����2�����Ʒ��������ã�
            abb_class = 2;
            LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
            break;
        case 5: //����9ʱ��
            abb_class = 9;
            LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
            break;
        case 6: //����11ʵʱ���ݵ���
            if(UARTCtrl->RM_SubsequentFrames == 0) //0=�޶�����֡,1-n=����֡��֡��
            {
                abb_class = 11;
                LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
            }
            else
            {   //����֡��02 81 E7 CB ��
                p8tx[0] = 0x02;
                p8tx[1] = 0x81;
                p8tx[2] = 0xE7; //0x01 �����⣬0x02 ����ʱ�䣻0x06 ����
                p8tx[3] = 0xCB;
                LEN_DATA = 4;
            }
            break;

        }
        return LEN_DATA;
    case 101://������
        i = UARTCtrl->TaskID;
        switch((i >> 8) & 0xff) //�����
        {
        case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
            return 0;
        default:
            break;
        }
        i = UARTCtrl->TaskID;
        switch((i >> 24) & 0xf) //��������
        {
        case 1://��ͨ�ɼ�����
            switch((UARTCtrl->TaskID >> 20) & 0xf) //�ɼ�����
            {
            default:
            case 1://1=�ɼ��ϵ�N��
                return 0;
            case 0://0=�ɼ���ǰ����
            case 2://2=������ʱ��ɼ�
            case 3://3=��ʱ�����ɼ�
                //ABB���������Ԥ����5��6�Ѿ��������ˣ��˴�ֻ��Ϊ���ܽ��뵽���մ���׶�
                abb_class = transform_OOP_into_ABB(p8CSD, PORTn);
                LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
                break;
            case 4://4=RetryMetering ����(��ͬ,2=������ʱ��ɼ�)
                break;
            }
            break;
        case 2://�¼��ɼ�����
        case 3://͸������
            return 0;
        default:
            return 0;
        }
        return LEN_DATA;
    case 102://�ر�����
        p8tx[0] = 0x02;
        p8tx[1] = 0x80;
        p8tx[3] = 0xF7;
        p8tx[3] = 0xEA;
        LEN_DATA = 4;
        return LEN_DATA;
    //return 0;
    default://ABB��Լ���Ӻ͹رչ���
        UARTCtrl->CONNECT = 0;
        return 0;
    }
}


/*
    struct OOP_into_ABB
    {
       u32   OOP_OI;     //Ҫ����CSD
       u8    CLASS;      //��Ӧ��ABB����
    };

   struct OOP_into_ABB  R_T_data[38] =
   {
       0x40000200,9 ,// ����ʱ��
       0x00100200,11,//2 �����й�����
       0x00100201,11,//  �����й��ܵ���
       0x00110200,0xEE,//  A�������й�
       0x00120200,0xEE,//  B�������й�
       0x00130200,0xEE,//  C�������й�
       0x00200200,11,//3 �����й�����
       0x00200201,11,//  �����й��ܵ���
       0x00300200,11,//4 ����޹�1����
       0x00400200,11,//  ����޹�2����
       0x00500200,0xEE,//6 һ�����޹�����
       0x00500201,0xEE,// һ�����޹��ܵ���
       0x00600200,0xEE,//7 �������޹�����
       0x00600201,0xEE,// �������޹��ܵ���
       0x00700200,0xEE,//8 �������޹�����
       0x00700201,0xEE,// �������޹��ܵ���
       0x00800200,0xEE,//9 �������޹�����
       0x00800201,0xEE,// �������޹��ܵ���
//       0x10100200,11,//10 �����й��������������ʱ��
       0x20000200,0xEE,// ��ѹ
       0x20000201,0xEE,// A���ѹ
       0x20010200,0xEE,// ����
       0x20010201,0xEE,// A�����
       0x20010400,0xEE,// ���ߵ�����698����Ҫ���ã�
       0x20040200,0xEE,// �й�����
       0x20040201,0xEE,// ���й�����
       0x20050200,0xEE,// �޹�����
       0x20060200,0xEE,// ���ڹ���
       0x200A0200,0xEE,// ��������
       0x200a0200,0xEE,// ��������
       0x20210200,0xEE,//1 ����ʱ��
       0x21310201,0xEE,//11 A���ѹ�ϸ���
       0x21310202,0xEE,// A���ѹ�ϸ���
       0x21320201,0xEE,//12 B���ѹ�ϸ���
       0x21320202,0xEE,// B���ѹ�ϸ���
       0x21330201,0xEE,//13 C���ѹ�ϸ���
       0x21330202,0xEE,// C���ѹ�ϸ���
   };

   n=sizeof(R_T_data)/sizeof(R_T_data[0]);
   for(i=0; i<n; i++)
   {
       if(OI == R_T_data[i].OOP_OI)
       {
           abb_class = (u32)R_T_data[i].CLASS;
           break;
       }
   }
   if(n == i) //û�в��ҵ���Ӧ����
   {
       abb_class = 0xEE;
   }
*/










