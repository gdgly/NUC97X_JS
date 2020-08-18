
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"


#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"




u32 RM_TxDATA_DL1997(u32 PORTn,u8 *p8CSD,u8 *p8tx)//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
{
	u32 i;
	u32 DI;
	u32 LEN_DATA;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx[0]=0x68;
	p8tx[1]=UARTCtrl->RMAddr[2]>>8;
	p8tx[2]=UARTCtrl->RMAddr[2];
	p8tx[3]=UARTCtrl->RMAddr[1]>>8;
	p8tx[4]=UARTCtrl->RMAddr[1];
	p8tx[5]=UARTCtrl->RMAddr[0]>>8;
	p8tx[6]=UARTCtrl->RMAddr[0];
	p8tx[7]=0x68;
	p8tx[8]=0x01;
	LEN_DATA=2;
	DI=UARTCtrl->RM_DI;//DL/T645-2007/1997当前抄表数据标识
	i=UARTCtrl->TaskID;
	if(((i>>24)&0xf)==1)
	{//普通采集方案
		i>>=20;
		i&=0xf;
		switch(i)//采集类型
		{
			case 0://0=采集当前数据
				break;
			case 1://1=采集上第N次
				break;
			case 2://2=按冻结时标采集
				break;
			case 3://3=按时标间隔采集
				//采曲线数据
				break;
			case 4://RetryMetering 补抄
				DI=0xffff;//97表不发补抄,意外进入发送时发错误的DI
				break;
		}
	}
	p8tx[9]=LEN_DATA;
	for(i=0;i<2;i++)
	{
		p8tx[10+i]=DI;
		DI>>=8;
	}
	//加0x33
	for(i=0;i<LEN_DATA;i++)
	{
		p8tx[10+i]+=0x33;
	}
	//cs
	p8tx[10+LEN_DATA]=0;
	for(i=0;i<(10+LEN_DATA);i++)
	{
		p8tx[10+LEN_DATA]+=p8tx[i];
	}
	p8tx[10+LEN_DATA+1]=0x16;
	return 10+LEN_DATA+2;
}

















