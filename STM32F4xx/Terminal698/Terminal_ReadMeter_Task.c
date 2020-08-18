
//抄表任务
#include "AllHead.h"
#include "Terminal_ReadMeter_Task.h"
#include "../LPC177x8x/lpc177x8x_UART.h"
#include "../Calculate/Calculate.h"
#include "../Driver/MEMRW.h"



void ClrRMTaskCompletePn(u32 Pn)//清指定Pn的抄表任务完成标志;入口:Pn
{
	u32 i;
	for(i=0;i<MaxRMTask;i++)
	{
		AllRMList->Pn[Pn].RMCompleteS[i]=0;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
	}
}

void ClrRMTaskCompletes(u32 PORTn,u32 RMTask)//清端口抄表任务完成标志;入口:PORTn=端口号,RMTask=任务号
{
	u32 i;
	u32 Pn;
	u32 RS485PORT;
#if IC_SST39VF6401B==0//0=没,1=有
	u32 Addr;
#else
	u8 *p8;
#endif

	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=2;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=3;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=4;
			break;
		case RS485_4PORT://RS485-4(载波)
			RS485PORT=31;
			break;
		default:
			return;
	}
#if IC_SST39VF6401B==0//0=没,1=有
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//通信速率及端口号
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//通信端口号相同
			Pn=MRR(Addr,2);//所属测量点号 BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//所属测量点号!=0x0
				AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
#else
	p8=(u8*)(ADDR_AFN04F10+2);
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=p8[2];//通信速率及端口号
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//通信端口号相同
			Pn=p8[0]+(p8[1]<<8);//所属测量点号 BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//所属测量点号!=0x0
				AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
#endif
}



