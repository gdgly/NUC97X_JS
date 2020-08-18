
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"
#include "../DL698/RM_TxRxDATA_ABB.h"




u32 ABB_CRC(u8* p8, u32 nByte) //ABB表CRC计算
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

u32 ABB_Password(u32 key, u32 pword) //根据密匙key与远程通讯口令来计算加密口令字
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



u32 Calculate_ABBkey(u32 PORTn, u8 *p8) //计算ABB通讯密码
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
    x = AddrToMeterSetNo(pAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效

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
//根据p8CSD,返回要抄的ABB类。
//返回abb_class,如果abb_class 为0xEE,则没有找到要读的类。
u32 transform_OOP_into_ABB(u8 *p8CSD, u32 PORTn)
{

    u32 i, OI, abb_class;
    UARTCtrl_TypeDef * UARTCtrl;
    UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
    i = UARTCtrl->RMCSD[1];;//当前正在抄的CSD(有数据类型)
    if(i == 0)
    {   //OAD
        OI  = UARTCtrl->RMCSD[2]  << 24; //当前正在抄的CSD
        OI |= UARTCtrl->RMCSD[2 + 1] << 16;
        OI |= UARTCtrl->RMCSD[2 + 2] << 8 ;
        OI |= UARTCtrl->RMCSD[2 + 3]    ;
    }
    else
    {   //ROAD
        i = UARTCtrl->RMROADCount; //抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
        i *= 4;
        OI  = UARTCtrl->RMCSD[7 + i]  << 24; //当前正在抄的CSD
        OI |= UARTCtrl->RMCSD[7 + i + 1] << 16;
        OI |= UARTCtrl->RMCSD[7 + i + 2] << 8 ;
        OI |= UARTCtrl->RMCSD[7 + i + 3]    ;
        UARTCtrl->RMROADCountMax = UARTCtrl->RMCSD[6];
    }
    switch(OI)
    {
    case 0x20210200:            // 冻结时间，冻结时间用本地时间来代替
    case 0x40000200:            // 日期时间
        abb_class = 9;
        break;
    case 0x00100200:            //2 正向有功电能
    case 0x00100201:            //  正向有功总电能
    case 0x00200200:            //3 反向有功电能
    case 0x00200201:            //  反向有功总电能
    case 0x00300200:            //4 组合无功1电能
    case 0x00400200:            //  组合无功2电能
    case 0x10100200:            //10 正向有功最大需量及发生时间
        abb_class = 11;
        break;
    default:
        abb_class = 0xEE;
        break;

    }
    return abb_class;

}
u32 RM_TxDATA_ABB_Class(u32 Class, u32 PORTn) //读ABB表不同的类
{
    u32 LEN_DATA = 0;
    u8 *p8;
    p8 = (u8 *)Get_ADDR_UARTnTx(PORTn);
    switch(Class)//
    {
    case 0://读类0电量小数点
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
    case 2://读类2计费数据配置，这里设置了要读数据的长度40和偏移地址50
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
    case 9://读类9状态1，时间等参数
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
    case 11://读类11当前计费数据
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
//如果ABB中没有698的数据标识，应该“LEN_DATA = 0”，
//但是为了进入“Err=RM_RxDATA_ABB(PORTn)”数据接收处理的流程，此处 “LEN_DATA = 1;”
//实际上不会去发送数据
        LEN_DATA = 1;
        break;
    }
    return LEN_DATA;
}
u32 RM_TxDATA_ABB(u32 PORTn, u8 *p8CSD, u8 *p8tx) //抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
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
    UARTCtrl->BpsCtrl = //通信控制字(同规约2007)(半字对齐)
        (2 << 5) | //D8-D5位表示波特率,O～7依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
        (0 << 4) | //D4位表示停止位位数0/1分别表示1/2个停止位
        (0 << 3) | //D3位表示有无校验位0/1分别表示无/有
        //D2位表示偶／奇校验位0/1分别表示偶／奇校验
        3;        //D1-D0 数据位数0～3分别表示5-8位
    //1个起始位,8个数据位，1个停止位，无校验 1200波特率
    switch(UARTCtrl->CONNECT)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
    {
    case 1://300bps握手 //{0x02,0x18,0x06,0x00,0x01,0x01,0x98,0x9e};
        p8 += ABB_SUB_OFFSET;
        switch(p8[0])
        {
        case 0:
        case 1:
            MC(0, (u32)p8, LEN_UART2Rx / 2);
            p8[0] = 1;

            p8tx[0] = 0x02;
            p8tx[1] = 0x18;
            p8tx[2] = 0x06;//0x01 密码检测，0x02 设置时间；0x06 握手
            p8tx[3] = 0x00;
            p8tx[4] = 0x01;
            LEN_DATA = 5;
            x = UARTCtrl->RMAddr[2];   //如果地址是1234 5678 9012，RMAddr[0]=0x3412, RMAddr[1]=0x7856,RMAddr[2]=0x1290,
            //ABB表的地址是16进制，并非BCD码，即48应发送30
            p8tx[LEN_DATA] = (x & 0x0F) * 100;
            p8tx[LEN_DATA] += ((x & 0xF000) >> 12) * 10;
            p8tx[LEN_DATA] += (x & 0x0F00) >> 8;
            LEN_DATA++;
            CRCCode = ABB_CRC(p8tx, LEN_DATA);
            p8tx[LEN_DATA] = (CRCCode >> 8) & 0x00FF;
            p8tx[LEN_DATA + 1] =  CRCCode & 0x00FF;
            LEN_DATA += 2;//8个字节
            break;
        case 2://连接密码校验
            p8tx[0] = 0x02;
            p8tx[1] = 0x18;
            p8tx[2] = 0x01; //0x01 密码检测，0x02 设置时间；0x06 握手
            p8tx[3] = 0x00;
            p8tx[4] = 0x04;
            x = Calculate_ABBkey(PORTn, p8);//计算ABB远程通讯密钥
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
        case 3: //读类0，（电能量小数位）
            abb_class = 0;
            LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
            break;
        case 4: //读类2，（计费数据配置）
            abb_class = 2;
            LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
            break;
        case 5: //读类9时间
            abb_class = 9;
            LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
            break;
        case 6: //读类11实时数据电量
            if(UARTCtrl->RM_SubsequentFrames == 0) //0=无读后续帧,1-n=读续帧的帧号
            {
                abb_class = 11;
                LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
            }
            else
            {   //后续帧“02 81 E7 CB ”
                p8tx[0] = 0x02;
                p8tx[1] = 0x81;
                p8tx[2] = 0xE7; //0x01 密码检测，0x02 设置时间；0x06 握手
                p8tx[3] = 0xCB;
                LEN_DATA = 4;
            }
            break;

        }
        return LEN_DATA;
    case 101://抄表中
        i = UARTCtrl->TaskID;
        switch((i >> 8) & 0xff) //任务号
        {
        case RMTASKnoPOWER://停上电事件固定任务号
            return 0;
        default:
            break;
        }
        i = UARTCtrl->TaskID;
        switch((i >> 24) & 0xf) //方案类型
        {
        case 1://普通采集方案
            switch((UARTCtrl->TaskID >> 20) & 0xf) //采集类型
            {
            default:
            case 1://1=采集上第N次
                return 0;
            case 0://0=采集当前数据
            case 2://2=按冻结时标采集
            case 3://3=按时标间隔采集
                //ABB表的数据在预连接5和6已经读过来了，此处只是为了能进入到接收处理阶段
                abb_class = transform_OOP_into_ABB(p8CSD, PORTn);
                LEN_DATA = RM_TxDATA_ABB_Class(abb_class, PORTn);
                break;
            case 4://4=RetryMetering 补抄(类同,2=按冻结时标采集)
                break;
            }
            break;
        case 2://事件采集方案
        case 3://透明方案
            return 0;
        default:
            return 0;
        }
        return LEN_DATA;
    case 102://关闭连接
        p8tx[0] = 0x02;
        p8tx[1] = 0x80;
        p8tx[3] = 0xF7;
        p8tx[3] = 0xEA;
        LEN_DATA = 4;
        return LEN_DATA;
    //return 0;
    default://ABB规约连接和关闭过程
        UARTCtrl->CONNECT = 0;
        return 0;
    }
}


/*
    struct OOP_into_ABB
    {
       u32   OOP_OI;     //要抄的CSD
       u8    CLASS;      //对应的ABB表类
    };

   struct OOP_into_ABB  R_T_data[38] =
   {
       0x40000200,9 ,// 日期时间
       0x00100200,11,//2 正向有功电能
       0x00100201,11,//  正向有功总电能
       0x00110200,0xEE,//  A相正向有功
       0x00120200,0xEE,//  B相正向有功
       0x00130200,0xEE,//  C相正向有功
       0x00200200,11,//3 反向有功电能
       0x00200201,11,//  反向有功总电能
       0x00300200,11,//4 组合无功1电能
       0x00400200,11,//  组合无功2电能
       0x00500200,0xEE,//6 一象限无功电能
       0x00500201,0xEE,// 一象限无功总电能
       0x00600200,0xEE,//7 二象限无功电能
       0x00600201,0xEE,// 二象限无功总电能
       0x00700200,0xEE,//8 三象限无功电能
       0x00700201,0xEE,// 三象限无功总电能
       0x00800200,0xEE,//9 四象限无功电能
       0x00800201,0xEE,// 四象限无功总电能
//       0x10100200,11,//10 正向有功最大需量及发生时间
       0x20000200,0xEE,// 电压
       0x20000201,0xEE,// A相电压
       0x20010200,0xEE,// 电流
       0x20010201,0xEE,// A相电流
       0x20010400,0xEE,// 零线电流（698表需要配置）
       0x20040200,0xEE,// 有功功率
       0x20040201,0xEE,// 总有功功率
       0x20050200,0xEE,// 无功功率
       0x20060200,0xEE,// 视在功率
       0x200A0200,0xEE,// 功率因素
       0x200a0200,0xEE,// 功率因素
       0x20210200,0xEE,//1 冻结时标
       0x21310201,0xEE,//11 A相电压合格率
       0x21310202,0xEE,// A相电压合格率
       0x21320201,0xEE,//12 B相电压合格率
       0x21320202,0xEE,// B相电压合格率
       0x21330201,0xEE,//13 C相电压合格率
       0x21330202,0xEE,// C相电压合格率
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
   if(n == i) //没有查找到对应的类
   {
       abb_class = 0xEE;
   }
*/










