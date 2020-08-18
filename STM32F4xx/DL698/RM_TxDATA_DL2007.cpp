

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"



void DL645_68H_Add33H_CS_16H(u8* p8tx)//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
{
	u32 i;
	u32 x;

	//起始符
	p8tx[0]=0x68;
	p8tx[7]=0x68;
	//加0x33
	x=p8tx[9];
	for(i=0;i<x;i++)
	{
		p8tx[10+i]+=0x33;
	}
	//结尾0x16
	p8tx[10+x+1]=0x16;
	//cs
	x+=10;
	p8tx[x]=0;
	for(i=0;i<x;i++)
	{
		p8tx[x]+=p8tx[i];
	}
}

u32 RM_TxDATA_DL2007_DI(u32 PORTn,u32 DI)//抄表发送帧DI按上n次更改
{
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	if(((i>>24)&0xf)==1)
	{//普通采集方案
		i>>=20;
		i&=0xf;
		switch(i)//采集类型
		{
			default:
			case 0://0=采集当前数据
				break;
			case 1://1=采集上第N次
				i=UARTCtrl->RM_DI_N;//采集上第N次
				if(i)
				{
					i--;
				}
				DI+=i;
				break;
			case 2://2=按冻结时标采集
				break;
			case 3://3=按时标间隔采集
				////采曲线数据,按任务执行间隔时间和采集方案采集间隔时间计算
				break;
			case 4://RetryMetering 补抄
				i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
				i++;
				DI&=0xffffff00;
				DI|=i;
				break;
		}
	}
	return DI;
}

u32 RM_TxDATA_DL2007(u32 PORTn,u8 *p8CSD,u8 *p8tx)//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
{
	u32 i;
	u32 DI;
	u32 LEN_DATA;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->RM_SubsequentFrames==0)//0=无读后续帧,1-n=读续帧的帧号
	{//非后续帧
		p8tx[0]=0x68;
		p8tx[1]=UARTCtrl->RMAddr[2]>>8;
		p8tx[2]=UARTCtrl->RMAddr[2];
		p8tx[3]=UARTCtrl->RMAddr[1]>>8;
		p8tx[4]=UARTCtrl->RMAddr[1];
		p8tx[5]=UARTCtrl->RMAddr[0]>>8;
		p8tx[6]=UARTCtrl->RMAddr[0];
		p8tx[7]=0x68;
		p8tx[8]=0x11;
		
		LEN_DATA=4;
		DI=UARTCtrl->RM_DI;//DL/T645-2007/1997当前抄表数据标识
	#if (USER/100)==15//安徽
		//安徽:针对三相表需要支持06000001和061000FF等块抄读
		if(DI==0x06000001)
		{
			if(UARTCtrl->ReRMCount>=1)//抄表重发计数
			{
				DI=0x060000FF;//全部6类负荷曲线(安徽)
				UARTCtrl->RM_DI=DI;//DL/T645-2007/1997当前抄表数据标识
			}
		}
	#endif
		switch(DI)
		{
		#if (USER/100)==15//安徽
			case 0x060000FF://全部6类负荷曲线(安徽)
		#endif
			case 0x06000001://全部6类负荷曲线
			case 0x06010001://1类负荷曲线(电压、电流、频率)
			case 0x06020001://2类负荷曲线(有无功功率)
			case 0x06030001://3类负荷曲线(功率因数)
			case 0x06040001://4类负荷曲线(正反有功,組合无功1和2总电能)
			case 0x06050001://5类负荷曲线(1-4象限无功总电能)
			case 0x06060001://6类负荷曲线(当前有功需量,当前无功需量)
				//采曲线数据,按任务执行间隔时间和采集方案采集间隔时间计算
				DateSaveTI(PORTn,p8tx+32);//普通采集方案的采集存储时标;返回:pOUT=date_time_s
				p8tx[10+4+0]=1;//读1个点
				i=p8tx[37];//分
				i=hex_bcd(i);
				p8tx[10+4+1]=i;
				i=p8tx[36];//时
				i=hex_bcd(i);
				p8tx[10+4+2]=i;
				i=p8tx[35];//日
				i=hex_bcd(i);
				p8tx[10+4+3]=i;
				i=p8tx[34];//月
				i=hex_bcd(i);
				p8tx[10+4+4]=i;
				i=(p8tx[32]<<8)+p8tx[33];//年
				i-=2000;
				i=hex_bcd(i);
				p8tx[10+4+5]=i;
				LEN_DATA+=6;
				break;
			default:
				DI=RM_TxDATA_DL2007_DI(PORTn,DI);//抄表发送帧DI按上n次更改
				break;
		}
		p8tx[9]=LEN_DATA;
		for(i=0;i<4;i++)
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
	else
	{//后续帧
		p8tx[8]=0x12;
		p8tx[9]=5;
		p8tx[14]=UARTCtrl->RM_SubsequentFrames+0x33;
		p8tx[15]=0;//cs
		for(i=0;i<15;i++)
		{
			p8tx[15]+=p8tx[i];
		}
		p8tx[16]=0x16;
		return 17;//发送字节数
	}
}

//645-07广播校时
//68 99 99 99 99 99 99 68 08 06 84 55 43 38 39 4A 4B 16  
//645广播单地址校时
//地址为 11 11 11 11 11 11
//68 11 11 11 11 11 11 68 08 06 58 73 43 38 39 4A 0D 16 

u32 BroadcastTime_TxDATA_DL645(u8 *p8tx)//DL645广播校时发送帧;返回:组帧后总长度字节数
{
	u32 i;
	
	p8tx[0]=0x68;
	p8tx[1]=0x99;
	p8tx[2]=0x99;
	p8tx[3]=0x99;
	p8tx[4]=0x99;
	p8tx[5]=0x99;
	p8tx[6]=0x99;
	p8tx[7]=0x68;
	p8tx[8]=0x08;
	p8tx[9]=0x06;
	MR((u32)p8tx+10,ADDR_MYMDHMS,6);
	//加0x33
	for(i=0;i<6;i++)
	{
		p8tx[10+i]+=0x33;
	}
	//cs
	p8tx[10+6]=0;
	for(i=0;i<(10+6);i++)
	{
		p8tx[10+6]+=p8tx[i];
	}
	p8tx[10+6+1]=0x16;
	return 10+6+2;
}




