

#include "Project.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"



void Uart_DL645_RxTx(u32 PORTn)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			break;
		case 1://1=完成1帧发收
			break;
		case 2://2=接收超时(或错误)
			break;
		case 3://3=启动发送
			if(p16timer[0]!=0)
			{
				break;
			}
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
			}
#endif
			break;
		case 4://4=正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				p16timer[0]=UARTCtrl->TxToRxTimerOver;//发送后接收超时定时值ms/10(半字对齐)
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;
			}
			break;
		case 5://5=正在接收
			i=DL645_Rx(PORTn);//DL645帧接收;返回0没完成,1完成,2超时
			switch(i)
			{
				case 0://0没完成
					return;
				case 1://1完成
					UARTCtrl->Task=1;
					break;
				default://2超时
					UARTCtrl->Task=2;
					return;
			}
			p16timer[0]=50/10;//下帧启动发送延时50MS后
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
			}
#endif
			break;
	}
}

void DL645_68H_Add33H_CS_16H(u32 TxBuffAddr)//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
{
	u32 i;
	u32 x;
	u8 * p8;
	p8=(u8 *)(TxBuffAddr);
	//起始符
	p8[0]=0x68;
	p8[7]=0x68;
	//加0x33
	x=p8[9];
	for(i=0;i<x;i++)
	{
		p8[10+i]+=0x33;
	}
	//结尾0x16
	p8[10+x+1]=0x16;
	//cs
	x+=10;
	p8[x]=0;
	for(i=0;i<x;i++)
	{
		p8[x]+=p8[i];
	}
}


void Read_DL645_2007_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x11;
	p8[9]=0x4;
	MWR(DI,BuffAddr+10,4);
	//加0x33
	for(i=0;i<4;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[14]=0;
	for(i=0;i<14;i++)
	{
		p8[14]+=p8[i];
	}
	p8[15]=0x16;
}

void Read_DL645_2007_TxData_SubsequentFrames(u32 BuffAddr,u32 DI,u64 CommAddr,u32 Frames)//DL645-2007读后续数据帧,入口:要填入的缓冲地址,数据标识,通信地址,后续帧号
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x12;
	p8[9]=5;
	MWR(DI,BuffAddr+10,4);
	p8[14]=Frames;
	//加0x33
	for(i=0;i<5;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[15]=0;
	for(i=0;i<15;i++)
	{
		p8[15]+=p8[i];
	}
	p8[16]=0x16;
}


void ReadMeter_DL645_2007_TxCurveData(u32 BuffAddr,u32 DI,u64 CommAddr,u64 YMDHM)//DL645-2007读负荷曲线数据帧
{
	u32 i;
	u8 * p8txbuff;
	
	p8txbuff=(u8 *)BuffAddr;
	p8txbuff[0]=0x68;
	MWR(CommAddr,((u32)p8txbuff)+1,6);
	p8txbuff[7]=0x68;
	p8txbuff[8]=0x11;
	p8txbuff[9]=10;
	MWR(DI,((u32)p8txbuff)+10,4);
	MWR(1,((u32)p8txbuff)+10+4,1);//读1个点
	MWR(YMDHM,((u32)p8txbuff)+10+4+1,5);
	//加0x33
	for(i=0;i<10;i++)
	{
		p8txbuff[10+i]+=0x33;
	}
	//CS计算
	p8txbuff[20]=0;
	for(i=0;i<20;i++)
	{
		p8txbuff[20]+=p8txbuff[i];
	}
	p8txbuff[21]=0x16;
}

void Read_DL645_1997_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x01;
	p8[9]=0x2;
	MWR(DI,BuffAddr+10,2);
	//加0x33
	for(i=0;i<2;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[12]=0;
	for(i=0;i<12;i++)
	{
		p8[12]+=p8[i];
	}
	p8[13]=0x16;
}

void Read_DL645_1997_TxData_SubsequentFrames(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-1997读后续数据帧,入口:要填入的缓冲地址,数据标识,通信地址
{
	u32 i;
	u8 * p8;
	p8=(u8 *)(BuffAddr);
	p8[0]=0x68;
	MWR(CommAddr,BuffAddr+1,6);
	p8[7]=0x68;
	p8[8]=0x02;
	p8[9]=0x2;
	MWR(DI,BuffAddr+10,2);
	//加0x33
	for(i=0;i<2;i++)
	{
		p8[10+i]+=0x33;
	}
	//cs
	p8[12]=0;
	for(i=0;i<12;i++)
	{
		p8[12]+=p8[i];
	}
	p8[13]=0x16;
}

u32 DL645_Rx(u32 PORTn)//DL645帧接收;返回0没完成,1完成,2超时
{
	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
	p0=p16fifo[0];//中断接收指针
	p1=p16fifo[1];//当前已读指针
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//中断接收指针
		p16fifo[1]=0;//当前已读指针
		p16fifo[2]=0;//规约检查指针
		p16fifo[3]=0;
		p16fifo[4]=0;
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//有数据接收重置超时定时
		p16timer[0]=500/10;//字节间超时定时
	}
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-10))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;
		if(RxByte==1)
		{
			if(i!=0x68)
			{
				RxByte=0;
				continue;
			}
		}
		else
		{
			if(RxByte==8)
			{
				if(i!=0x68)
				{
					RxByte=0;
					continue;
				}
			}
			else
			{
				if(RxByte>=12)
				{
					if(RxByte>=(p8rxbuff[9]+12))
					{
						if(i==0x16)
						{//接收完成
							//和校验计算
							Pn=0;
							for(i=0;i<(p8rxbuff[9]+10);i++)
							{
								Pn+=p8rxbuff[i];
							}
							Pn&=0xff;
							if(Pn==p8rxbuff[i])
							{//和校验正确
								p16fifo[1]=p1;//当前已读指针
								p16fifo[2]=p1;//检查指针==读指针,免检省时
								UARTCtrl->RxByte=RxByte;
								return 1;
							}
							else
							{//和校验错误
								RxByte=0;
								continue;
							}
						}
						else
						{
							RxByte=0;
							continue;
						}
					}
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//超时
		return 2;//2=接收超时
	}
	return 0;
}

u32 DL645_2007_RxData(u32 PORTn)//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
{
	u32 i;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	//通信地址比较
	for(i=0;i<6;i++)
	{
		if(p8txbuff[4+1+i]==0xaa)
		{
			continue;
		}
		if(p8rxbuff[1+i]!=p8txbuff[4+1+i])
		{
			break;
		}
	}
	if(i!=6)
	{
		return 1;//返回0正确,1地址错,2控制码错,3数据标识错
	}
	//控制码
	if((p8rxbuff[8]&0x40)!=0x0)
	{//从站异常回答
		return 2;//返回0正确,1地址错,2控制码错,3数据标识错
	}
	//DI比较
	for(i=0;i<4;i++)
	{
		if(p8rxbuff[10+i]!=p8txbuff[4+10+i])
		{
			break;
		}
	}
	if(i!=4)
	{
		return 3;//返回0正确,1地址错,2控制码错,3数据标识错
	}
	//减0x33
	for(i=0;i<p8rxbuff[9];i++)
	{
		p8rxbuff[10+i]-=0x33;
	}
	return 0;//返回0正确,1地址错,2控制码错,3数据标识错
}

u32 DL645_1997_RxData(u32 PORTn)//DL645_1997帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
{
	u32 i;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	//通信地址比较
	for(i=0;i<6;i++)
	{
		if(p8txbuff[4+1+i]!=0x99)
		{
			break;
		}
	}
	if(i!=6)
	{//地址999999999999不比较
		for(i=0;i<6;i++)
		{
			if(p8rxbuff[1+i]!=p8txbuff[4+1+i])
			{
				break;
			}
		}
		if(i!=6)
		{
			return 1;//返回0正确,1地址错,2控制码错,3数据标识错
		}
	}
	//控制码
	if((p8rxbuff[8]&0x40)!=0x0)
	{//从站异常回答
		return 2;//返回0正确,1地址错,2控制码错,3数据标识错
	}
	//DI比较
	for(i=0;i<2;i++)
	{
		if(p8rxbuff[10+i]!=p8txbuff[4+10+i])
		{
			break;
		}
	}
	if(i!=2)
	{
		return 3;//返回0正确,1地址错,2控制码错,3数据标识错
	}
	//减0x33
	for(i=0;i<p8rxbuff[9];i++)
	{
		p8rxbuff[10+i]-=0x33;
	}
	return 0;//返回0正确,1地址错,2控制码错,3数据标识错
}

void Terminal_BroadcastTime_DL645(u32 PORTn)//DL645广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u16 * p16timer;
#if ((USER/100)==5)//上海
	u32 x;
	u64 d64;
#endif

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(UARTCtrl->BroadcastTime)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
	{
		default:
			UARTCtrl->BroadcastTime=0;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
		case 3:
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					i=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					i=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					i=3;//RS485PORT=3;
					break;
				default:
					i=0;//RS485PORT=0;
					break;
			}
			CopyString((u8 *)"口x 正在广播校时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			MWR(i+0x30,ADDR_DATABUFF+2,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			p16timer[0]=1000/10;//延时1秒后进行
			UARTCtrl->BroadcastTime++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
		case 4://1200发送
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->BpsCtrl=(2<<5)+0x0b;
		TX:
			MC(0xfe,((u32)p8txbuff),4);
			MC(0x99,((u32)p8txbuff)+5,6);
			p8txbuff+=4;
			p8txbuff[0]=0x68;
			p8txbuff[7]=0x68;
			p8txbuff[8]=0x08;
			p8txbuff[9]=6;
		#if ((USER/100)==5)//上海
			i=MRR(ADDR_AFN05F201,1);
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					x=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					x=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					x=3;//RS485PORT=3;
					break;
			}
			if(i&(1<<(x-1)))
			{
				d64=MRR(ADDR_TYMDHMS,6);
				i=MRR(ADDR_AFN05F201+7,2);
				if(i&0x1000)
				{//负值
					i&=0xfff;
					i=bcd_hex(i);
					d64=YMDHMS_SubS(d64,i);//年月日时分秒减S秒,返回减S秒后的年月日时分秒
				}
				else
				{//正值
					i=bcd_hex(i);
					d64=YMDHMS_AddS(d64,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
				}
				MWR(d64,((u32)p8txbuff)+10,6);
			}
			else
			{
				MR(((u32)p8txbuff)+10,ADDR_TYMDHMS,6);
			}
		#else
			MR(((u32)p8txbuff)+10,ADDR_TYMDHMS,6);
		#endif
			for(i=0;i<6;i++)
			{
				p8txbuff[10+i]+=0x33;
			}
			p8txbuff[16]=0;
			p8txbuff[17]=0x16;
			for(i=0;i<16;i++)
			{
				p8txbuff[16]+=p8txbuff[i];
			}
			UARTCtrl->TxByte=22;//发送字节数(半字对齐)
			
			//启动
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;//正在发送
			
			UARTCtrl->BroadcastTime++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
			}
#endif
			break;
		case 5:
			Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(UARTCtrl->Task<3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				UARTCtrl->BroadcastTime++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			break;
		case 6://2400发送
			UARTCtrl->BpsCtrl=(3<<5)+0x0b;
			goto TX;
		case 7:
	#if ((USER/100)==5)//上海
			Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(UARTCtrl->Task<3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				UARTCtrl->BroadcastTime++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			break;
		case 8:
			//上海智能表波特率为4800
			//4800发送
			UARTCtrl->BpsCtrl=(4<<5)+0x0b;
			goto TX;
		case 9:
	#endif
			Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(UARTCtrl->Task<3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				UARTCtrl->BroadcastTime=0;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			#if ((USER/100)==5)//上海
				i=MRR(ADDR_AFN05F201,1);
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						x=1;//RS485PORT=1;
						break;
					case RS485_2PORT://RS485-2
						x=2;//RS485PORT=2;
						break;
					case RS485_3PORT://RS485-3
						x=3;//RS485PORT=3;
						break;
				}
				if(i&(1<<(x-1)))
				{
					i&=~(1<<(x-1));
					MWR(i,ADDR_AFN05F201,1);
				}
			#endif
			}
			break;
	}
}


