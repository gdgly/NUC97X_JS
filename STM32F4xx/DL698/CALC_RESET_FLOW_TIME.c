

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CALC_RESET_FLOW_TIME.h"
#include "../DL698/EVENT_Record.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"



void FLOW_Count(u32 PORTn)//通信流量计数
{
	u32 i;
	u8 *p8;
	u32 Byte;
	UARTCtrl_TypeDef * UARTCtrl;
	
//#define ADDR_2200     ADDR_TerminalRealTimer+2+(TREALMTIMER_NUM*2)//通信流量
//数值∷=structure
//{
//当日通信流量  double-long-unsigned，
//当月通信流量  double-long-unsigned
//}
//单位：byte，换算：0
//#define LENmax_2200     12
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Byte=UARTCtrl->RxTxByte;
	p8=(u8*)ADDR_2200;
	if((Byte)||(p8[0]!=DataType_structure))
	{
		UARTCtrl->RxTxByte=0;
		p8[0]=DataType_structure;
		p8[1]=2;
//当日通信流量
		p8[2]=DataType_double_long_unsigned;
		i=(p8[3]<<24)+(p8[4]<<16)+(p8[5]<<8)+p8[6];
		i+=Byte;
		p8[3]=i>>24;
		p8[4]=i>>16;
		p8[5]=i>>8;
		p8[6]=i;
//当月通信流量
		p8[7]=DataType_double_long_unsigned;
		i=(p8[8]<<24)+(p8[9]<<16)+(p8[10]<<8)+p8[11];
		i+=Byte;
		p8[8]=i>>24;
		p8[9]=i>>16;
		p8[10]=i>>8;
		p8[11]=i;
		//超限事件
		Byte=MRR(ADDR_3110_6+3,4);//通信流量门限  double-long-unsigned（单位：byte）
		if(Byte!=0)
		{
			Byte=R_Inverse(Byte,4);//寄存器数据倒序(高低字节调换)
			if(i>=Byte)
			{//超限
				p8=(u8*)ADDR_2200_EventRecordFags;//通信流量超限事件已记录标志,月更新时清0
				if(p8[0]==0)
				{//本月没记录
					p8[0]=1;
					Event_OAD(0x31100200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
		}
	}
}

void POWER_Time(void)//供电时间
{
	u32 i;
	u8 *p8;
	
//#define ADDR_2203     ADDR_2200+LENmax_2200//供电时间
//数值∷=structure
//{
//日供电累计时间  double-long-unsigned，
//月供电累计时间  double-long-unsigned
//}
//单位：分钟，换算：0
//1byte 上次计算供电时间的实时钟分
//#define LENmax_2203     13
	p8=(u8*)ADDR_2203;
	i=Comm_Ram->DL698YMDHMS[5];//年(先高字节后低字节),月,日,时,分,秒,百分秒,周
	if(p8[12]==i)
	{
		return;
	}
	p8[12]=i;
	if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		return;
	}
	p8[0]=DataType_structure;
	p8[1]=2;
//日供电累计时间  double-long-unsigned，
	p8[2]=DataType_double_long_unsigned;
	i=(p8[3]<<24)+(p8[4]<<16)+(p8[5]<<8)+p8[6];
	i++;
	p8[3]=i>>24;
	p8[4]=i>>16;
	p8[5]=i>>8;
	p8[6]=i;
//月供电累计时间  double-long-unsigned
	p8[7]=DataType_double_long_unsigned;
	i=(p8[8]<<24)+(p8[9]<<16)+(p8[10]<<8)+p8[11];
	i++;
	p8[8]=i>>24;
	p8[9]=i>>16;
	p8[10]=i>>8;
	p8[11]=i;
}

void RESET_Count(void)//终端复位次数
{
	u32 i;
	u8 *p8;
	
//#define ADDR_2204     ADDR_2203+LENmax_2203//复位累计次数
//数值∷=structure
//{
//日复位累计次数  long-unsigned，
//月复位累计次数  long-unsigned
//}
//#define LENmax_2204     8
	p8=(u8*)ADDR_2204;
	p8[0]=DataType_structure;
	p8[1]=2;
//日复位累计次数
	p8[2]=DataType_long_unsigned;
	i=(p8[3]<<8)+p8[4];
	i++;
	p8[3]=i>>8;
	p8[4]=i;
//月复位累计次数
	p8[5]=DataType_long_unsigned;
	i=(p8[6]<<8)+p8[7];
	i++;
	p8[6]=i>>8;
	p8[7]=i;
}

void CLR_RESET_FLOW_TIME(void)//清0终端复位次数_通信流量_供电时间
{
//通信流量计数
	MC(0,ADDR_2200+3,4);
	MC(0,ADDR_2200+8,4);
//通信流量超限事件已记录标志
	MC(0,ADDR_2200_EventRecordFags,1);//通信流量超限事件已记录标志,月更新时清0
//供电时间
	MC(0,ADDR_2203+3,4);
	MC(0,ADDR_2203+8,4);
//复位累计次数
	MC(0,ADDR_2204+3,2);
	MC(0,ADDR_2204+6,2);
	
}
















