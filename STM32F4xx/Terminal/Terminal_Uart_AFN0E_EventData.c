
//uart类3数据(事件)
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"






void Uart_AFN0E(u32 PORTn)//类3数据(事件)
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
	u32 Addr;
  u32 LEN_UARTnTx;
  u64 d64a;
  u64 d64b;
	
	u8 * p8s;
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;

  LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
	if(DIlen<6)
	{
		goto err;
	}
//数据(启动站功能码11（请求2级数据）用于应用层请求数据的链路传输)
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	RXaddr=Get_ADDR_UARTnRx(PORTn)+14+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=Get_ADDR_UARTnTx(PORTn)+14+4;
	TXlen=0;
	f=0;//有无有效数据填入标志
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	p8d=(u8 *)(TXDIaddr);
	p8d[0]=0;
	p8d[1]=0;
	p8d[2]=0;
	p8d[3]=0;
	d64a=MRR(ADDR_AFN0CF8,8);//F8终端事件标志状态p0
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
//DIerr:
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if((TXlen+14+8+2)>LEN_UARTnTx)
				{
					goto err;
				}
				f=0;
			}
			CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
			p8d=(u8 *)(TXDIaddr);
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
			p8d[3]=0;
		}
		else
		{
			Fn=(i>>16);
			Pn=i&0xffff;
			if(Pn!=0x0)
			{
				DIlen-=2;
				RXaddr+=2;
				continue;	
			}
			switch(Fn)
			{
				case 1://请求重要事件
					Addr=ADDR_AFN0EF1;
					break;
				case 2://请求一般事件
					Addr=ADDR_AFN0EF2;
					break;
				case 3://分类事件
					p8s=(u8 *)(RXaddr);
					p8d=(u8 *)(TXaddr);
					i=p8s[0];//请求事件代码ERC
					i--;
					if(i>(MaxERC-1))
					{//ERC代码错
						DIlen-=2;
						RXaddr+=2;
						continue;
					}
					Addr=ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)));
					i=p8s[1];//请求最近发生的事件数量n
					if(i==0)
					{//请求该事件代码ERC的所有事件
						i=MaxClassEventNum;
					}
					x=MRR(Addr,2);//取已存事件记录数
					Addr+=2;
					if(x>MaxClassEventNum)
					{
						x=MaxClassEventNum;
					}
					if(i>x)
					{
						i=x;
					}
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8d[0]=i;//本帧报文传送的事件数量n
					x-=i;//开始传送记录指针
					TXlen+=1;
					TXaddr+=1;
					for(y=0;y<i;y++)
					{
						f=MRR(Addr+(x*LEN_TERMINAL_EVENT)+1,1);//取事件长度
						f+=2;
						if((TXlen+4+14+8+2+f)>LEN_UARTnTx)
						{
							goto err;
						}
						MR(TXaddr,Addr+(x*LEN_TERMINAL_EVENT),f);
						TXlen+=f;
						TXaddr+=f;
						x++;
					}
					DIlen-=2;
					RXaddr+=2;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					f=1;
					continue;

				default:
					DIlen-=2;
					RXaddr+=2;
					continue;
			}
			MR(TXaddr,ADDR_AFN0CF7_FM,2);//先读2字节EC计数
			p8s=(u8 *)(RXaddr);
			p8d=(u8 *)(TXaddr+2);
			p8d[0]=p8s[0];//起始指针
			p8d[1]=p8d[0];//当前结束指针=起始指针
			TXlen+=4;
			TXaddr+=4;
			while(p8d[1]!=p8s[1])
			{//当前结束指针!=请求结束指针
				x=Addr+(p8d[1]*LEN_TERMINAL_EVENT);
				//取ERC号,计算已读标志
				i=MRR(x,1);
				i-=1;
				if(i<=63)
				{//ERC合法
					d64b=1;
					d64b<<=i;
					d64b=~d64b;
					d64a&=d64b;
				}
				//取事件
				i=MRR(x+1,1);
				if(i==0x0)
				{//事件记录长度Le=0,无事件
					if(TXlen==4)
					{//无事件读入,只有2字节EC计数2字节指针
						p8d[0]+=1;//起始指针+1
						p8d[1]=p8d[0];//当前结束指针=起始指针
						continue;
					}
					else
					{
						break;
					}
				}
				i+=2;
				if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
				{
					goto err;
				}
				MR(TXaddr,x,i);
				TXlen+=i;
				TXaddr+=i;
				p8d[1]+=1;
			}
			
			DIlen-=2;
			RXaddr+=2;
			if(TXlen==4)
			{//无事件读入,只有2字节EC计数2字节指针
				TXlen-=4;
				TXaddr-=4;
				continue;
			}
			FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
			f=1;

			switch(Fn)
			{//读数据后清相关数据的Fn
				case 1://请求重要事件
					Addr=ADDR_AFN0EF1;
					if(PORTn==GPRSPORT)
					{
						MWR(0,ADDR_ADCFlags,1);//重要事件等待访问0=没,1=有
					}
		#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
					if(PORTn==ENETPORT)
					{
						MWR(0,ADDR_ADCFlags+1,1);//重要事件等待访问0=没,1=有
					}
		#else
					if(PORTn==ENETPORT)
					{
						MWR(0,ADDR_ADCFlags,1);//重要事件等待访问0=没,1=有
					}
		#endif			
					if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
					{
						MWR(0,ADDR_ADCFlags+2,1);//重要事件等待访问0=没,1=有
					}
					break;
			}
		}
	}
	if(TXlen==0x0)
	{
		goto err;
	}
	MWR(d64a,ADDR_AFN0CF8,8);//F8终端事件标志状态p0
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位

}










