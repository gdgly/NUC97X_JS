

//用户标识:吉林系
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#endif
#include "../terminal/Terminal_USER11xx.h"
#include "../terminal/Terminal_Uart_3761.h"
#include "../terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"



#if MainProtocol==376
#if (USER/100)==11//吉林系

u32 USER11xx_RxAddr(u32 PORTn)//吉林系接收帧终端地址比较,4字节长度变为标准帧的2字节;返回:0=地址不同,1=地址相同
{
	u32 i;
	u32 x;
	u16 * p16;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	
	if(Comm_Ram->Factory==0x55)//0x55=工厂状态
	{
		return 1;
	}
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);

//吉林增加万能通信地址
		i=MRR(((u32)p8rxbuff)+7,4);
		if(i==0xffffffff)
		{//广播地址
			//剔除投入
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//剔除投入,只有对时命令继续有效,其他全部无效
				if(p8rxbuff[12+2]!=5)
				{//AFN!=5
					return 0;
				}
				if((p8rxbuff[14+2]+(p8rxbuff[15+2]<<8))!=0)
				{//Pn!=0
					return 0;
				}
				if((p8rxbuff[16+2]+(p8rxbuff[17+2]<<8))!=0x340)
				{//Fn!=31对时命令
					return 0;
				}
			}
			UARTCtrl->AddrType=3;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			goto AddrjlOK;
		}
//吉林增加万能通信地址

	MR(ADDR_DATABUFF,ADDR_AREACODE,2);//读行政区域码
	MR(ADDR_DATABUFF+2,ADDR_04000401,4);//读表地址作为终端地址
	i=p8rxbuff[9]+(p8rxbuff[10]<<8);//地址码
	if(i==0xffff)
	{//广播地址
		if(p8rxbuff[12+2]!=15)//应用层功能码AFN
		{//不是文件传输
			if((p8rxbuff[11+2]&0x1)==0x0)
			{//不是组地址标志错
				return 0;
			}
			//剔除投入
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//剔除投入,只有对时命令继续有效,其他全部无效
				if(p8rxbuff[12+2]!=5)
				{//AFN!=5
					return 0;
				}
				if((p8rxbuff[14+2]+(p8rxbuff[15+2]<<8))!=0)
				{//Pn!=0
					return 0;
				}
				if((p8rxbuff[16+2]+(p8rxbuff[17+2]<<8))!=0x340)
				{//Fn!=31对时命令
					return 0;
				}
			}
			//行政区域码比较
			if((p8rxbuff[7]+p8rxbuff[8])!=0x0)
			{//注:行政区域码=0时自定义认为广播区域码用于终端地址通信设置
				if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,2)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				{//行政区域码错
					return 0;
				}
			}
			UARTCtrl->AddrType=3;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
		}
	}
	else
	{//非广播地址
		if((p8rxbuff[11+2]&0x1)==0x0)
		{//单地址比较
			//本终端行政区域码和地址比较
			if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,6)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			{//不是本终端行政区域码和地址
				return 0;
			}
		}
		else
		{//地址为组地址
			//剔除投入
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//剔除投入,只有对时命令继续有效,其他全部无效
				if(p8rxbuff[12+2]!=5)
				{//AFN!=5
					return 0;
				}
				if((p8rxbuff[14+2]+(p8rxbuff[15+2]<<8))!=0)
				{//Pn!=0
					return 0;
				}
				if((p8rxbuff[16+2]+(p8rxbuff[17+2]<<8))!=0x340)
				{//Fn!=31对时命令
					return 0;
				}
			}
			//行政区域码比较
			if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,2)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			{//行政区域码错
				return 0;
			}
			p16=(u16 *)(ADDR_DATABUFF);
			p16[0]=p8rxbuff[9]+(p8rxbuff[10]<<8);//地址码
			if(p16[0]==0)
			{//组地址=0为无效组地址
				return 0;
			}
			MR(ADDR_DATABUFF+2,ADDR_AFN04F6,16);
			for(i=0;i<8;i++)
			{
				if(p16[0]==p16[1+i])
				{
					break;
				}
			}
			if(i==8)
			{//组地址不相同
				return 0;
			}
			UARTCtrl->AddrType=2;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
		}
	}
	//移动接收帧终端地址6字节长度为标准帧的4字节长度
AddrjlOK:	
	x=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(x<5)
	{
		return 0;
	}
	i=x-2;
	p8rxbuff[1]=i;
	p8rxbuff[2]=i>>8;
	x-=5;//需移动的字节数
	for(i=0;i<x;i++)
	{
		p8rxbuff[6+5+i]=p8rxbuff[6+5+2+i];
	}
	return 1;
}

//	0x68,
//	12,0,
//	12,0,
//	0x68,
//	0xc9,//控制码(DIR=1上行,启动标志位PRM=1,功能码9=请求∕响应帧	链路测试)
//	0,0,//行政区划码
//	0,0,//终端地址

//	0,//启动站地址(终端启动发送帧的MSA应为零)
//	2,//AFN=2
//	0x70,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ
//	0,0,//Pn=0
//	0x04,0,//Fn=1登入,2退出,3心跳
//	0,//cs
//	0x16,


void USER11xx_TxAddr(u32 PORTn)//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2
{
	u32 i;
	u32 x;
	u32 len;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	if(Comm_Ram->Factory==0x55)//0x55=工厂状态
	{
		return;
	}
	x=p8txbuff[1]+(p8txbuff[2]<<8);//用户数据区长度;每帧字符数为用户数据长度L+8

	x>>=2;
	len=x;
	if(x<5)
	{
		return;
	}
	x-=5;//需移动的字节数
	for(i=0;i<x;i++)
	{
		p8txbuff[(5+len+2)-i]=p8txbuff[(5+len)-i];
	}
	MR(((u32)p8txbuff)+9,ADDR_04000401,4);//读表地址作为终端地址
	len+=2;
	p8txbuff[1]=len;
	p8txbuff[2]=len>>8;
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	UARTCtrl->TxByte+=2;//需发送字节数
}


#endif
#endif
