
//自动搜表RS485
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/AutoSearchMeter.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../DL698/RM_TxDATA.h"
#include "../Device/MEMRW.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/SearchMeter_Record.h"
#include "../DL698/DL698_Uart.h"



void AutoSearchMeter_DL645_2007_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
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

void AutoSearchMeter_DL645_1997_TxData(u32 BuffAddr,u32 DI,u64 CommAddr)//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
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

void AutoSearchMeter_DL698_TxData(u32 BuffAddr,u32 OAD,u64 CommAddr)//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
{
	u32 i;
	u8* p8tx;
	
	p8tx=(u8*)BuffAddr;
//起始字符（68H）  1Byte
	p8tx[0]=0x68;
//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
//控制域C  1Byte
	p8tx[3]=0x43;
//地址域A
	MWR(CommAddr,BuffAddr+5,6);
	for(i=0;i<6;i++)
	{
		if(p8tx[5+i]==0xaa)
		{
			break;
		}
	}
	if(i<6)
	{
		p8tx[4]=0x45;
	}
	else
	{
		p8tx[4]=0x05;
	}
//客户机地址CA  1Byte
	p8tx[11]=0;
//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
//APDU
	p8tx[14]=5;//GET-Request
	p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
	i=p8tx[16];//PIID
	i++;
	i&=0x3f;
	p8tx[16]=i;
	//OAD
	OAD=R_Inverse(OAD,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	for(i=0;i<4;i++)
	{
		p8tx[17+i]=OAD&0xff;
		OAD>>=8;
	}
//时间标签
	p8tx[21]=0;
//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
//结束字符（16H）
	p8tx[1]=23;
	p8tx[2]=0;
	Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	//共25byte
}


//属性2（所有搜表结果）∷=array 一个搜表结果
//一个搜表结果∷=structure
//{
//  通信地址         TSA，
//  所属采集器地址   TSA，
//	规约类型  enum
//	{
//  	未知          （0），
//		DL/T645-1997  （1），
//		DL/T645-2007  （2），
//		DL/T698.45    （3），
//		CJ/T 188-2004 （4）
//	}，
//	相位             enum{未知（0），A（1），B（2），C（3）}，
//		信号品质         unsigned，
//	搜到的时间       date_time_s，
//	搜到的附加信息   array附加信息
//}
//附加信息∷=structure
//{
//	对象属性描述  OAD，
//	属性值        Data
//}

//属性5（跨台区搜表结果）∷=array 一个跨台区结果
//一个跨台区结果∷=structure
//{
//	通信地址       TSA，
//	主节点地址     TSA，
//	变更时间       date_time_s
//}

#if (USER/100)==0//电科院测试
void AutoSearchMeter_RS485(u32 PORTn)//自动搜表任务-rs485:0=空,1=等待,2...过程
{
	UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	UARTCtrl->TaskID=0;
	UARTCtrl->AutoSearchTask=0;//自动搜表任务:0=空,1=等待,2...过程
}
#else
void AutoSearchMeter_RS485(u32 PORTn)//自动搜表任务-rs485:0=空,1=等待,2...过程
{
	u32 i;
	u32 x;
	u32 RS485PORT;
	u8 *p8;
	u8 * p8tx;
	u8 * p8rx;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16fifo;
	u16 *p16timer;
	u32 ProtocolBps;
	u64 MeterAddr;
	
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	if(Terminal_Ram->AutoSearch_M_Timer==0)//2 自动搜表允许时间分定时器
	{
		UARTCtrl->TaskID=0;
		UARTCtrl->AutoSearchTask=0;//自动搜表任务:0=空,1=等待,2...过程
		return;
	}
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=0;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=1;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=2;
			break;
		default:
			return;
	}
	ProtocolBps=MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT)+6,1);
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			if(p16timer[0]==0x0)
			{//下帧启动发送延时
				UARTCtrl->Task=3;//启动发送
			}
			break;
		case 3://3=启动发送
			MC(0xfe,(u32)p8tx,4);
			i=ProtocolBps>>4;//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
			switch(i)
			{
				case 0://DL645-2007
				case 3:
					AutoSearchMeter_DL645_2007_TxData(((u32)p8tx)+4,0x00010000,MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6));//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
					UARTCtrl->TxByte=4+16;//TX计数(需发送字节数)
					UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//默认2400
					break;
				case 1://DL698
				case 4:
					MeterAddr=MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					//MeterAddr=R_Inverse(MeterAddr,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					AutoSearchMeter_DL698_TxData(((u32)p8tx)+4,0x40010200,MeterAddr);//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
					UARTCtrl->TxByte=4+25;//TX计数(需发送字节数)
					UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//默认9600
					break;
				case 2://DL645-1997
				case 5:
					AutoSearchMeter_DL645_1997_TxData(((u32)p8tx)+4,0x9010,MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6));//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
					UARTCtrl->TxByte=4+14;//TX计数(需发送字节数)
					UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//默认1200
					break;
				default:
					MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
					return;
			}
			//p16fifo[0]=0;
			//p16fifo[1]=0;
			//p16fifo[2]=0;
			UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;
			RS485_TxLED(PORTn);//RS485 LED发送指示灯
			CopyString((u8 *)"485x正在自动搜表",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			MWR(RS485PORT+1,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case 4://4=正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				UARTCtrl->Task=5;
				p16timer[0]=1000/10;//1000ms接收定时
			}
			break;
		case 5://5=正在接收
			DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
			if(p16fifo[0]!=p16fifo[2])
			{//有字节收到
				p16fifo[2]=p16fifo[0];
				p16timer[0]=500/10;//500ms定时
				if(PORTn==RS485_1PORT)
				{
					Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
				}
				if(PORTn==RS485_2PORT)
				{
					Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
				}
				break;
			}
			if(p16timer[0]!=0)
			{//等待全部响应表发送完成
				break;
			}
			i=ProtocolBps>>4;//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
			switch(i)
			{
				case 0://DL645-2007
				case 3:
					i=RM_Rx_DL645(PORTn);//UART_645接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
					break;
				case 1://DL698
				case 4:
					i=Uart_698_Rx(PORTn);//UART_698接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
					break;
				case 2://DL645-1997
				case 5:
					i=RM_Rx_DL645(PORTn);//UART_645接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
					break;
				default:
					MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
					UARTCtrl->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					return;
			}
			switch(i)
			{
				case 0://0没完成
					return;
				case 1://1完成
					UARTCtrl->Task=0;//空闲1次
					i=ProtocolBps>>4;//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
					switch(i)
					{
						case 0://DL645-2007
						case 3:
						case 2://DL645-1997
						case 5:
							if((p8rx[8]&0x80)==0x0)
							{//不是从动帧
								return;//重发
							}
							break;
						case 1://DL698
						case 4:
							if((p8rx[3]&0x80)==0x0)
							{//不是从动帧
								return;//重发
							}
							break;
						default:
							MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
							UARTCtrl->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							return;
					}
					if(p16fifo[0]!=p16fifo[1])
					{
						goto Over;//有多佘字节接收作为超时
					}
					for(i=5;i<11;i++)
					{
						if(p8tx[i]==0xaa)
						{
							break;
						}
					}
					if(i<11)
					{//发送地址中含有0xaa,用返回来的地址验证
						i=ProtocolBps>>4;//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
						switch(i)
						{
							case 0://DL645-2007
							case 3:
								AutoSearchMeter_DL645_2007_TxData(((u32)p8tx)+4,0x00010000,MRR(((u32)p8rx)+1,6));//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
								UARTCtrl->TxByte=4+16;//TX计数(需发送字节数)
								break;
							case 1://DL698
							case 4:
								MeterAddr=MRR(((u32)p8rx)+5,6);
								AutoSearchMeter_DL698_TxData(((u32)p8tx)+4,0x40010200,MeterAddr);//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
								UARTCtrl->TxByte=4+25;//TX计数(需发送字节数)
								break;
							case 2://DL645-1997
							case 5:
								AutoSearchMeter_DL645_1997_TxData(((u32)p8tx)+4,0x9010,MRR(((u32)p8rx)+1,6));//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
								UARTCtrl->TxByte=4+14;//TX计数(需发送字节数)
								break;
							default:
								MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
								UARTCtrl->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								return;
						}
						UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
						Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
						Start_UARTn_Tx(PORTn);//启动UART口发送
						UARTCtrl->Task=4;
						RS485_TxLED(PORTn);//RS485 LED发送指示灯
						return;
					}
					i=ProtocolBps>>4;//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
					switch(i)
					{
						case 0://DL645-2007
						case 3:
							i=DataComp(((u32)p8rx)+1,((u32)p8tx)+4+1,6);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
							if(i==0)
							{//地址相同
								MeterAddr=MRR(((u32)p8rx)+1,6);
								SearchMeter_Record(PORTn,3,MeterAddr,2,0x0,ProtocolBps&0xf,0);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
							}
							break;
						case 1://DL698
						case 4:
							i=DataComp(((u32)p8rx)+5,((u32)p8tx)+4+5,6);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
							if(i==0)
							{//地址相同
								MeterAddr=MRR(((u32)p8rx)+5,6);
								SearchMeter_Record(PORTn,3,MeterAddr,3,0x0,ProtocolBps&0xf,0);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
							}
							break;
						case 2://DL645-1997
						case 5:
							i=DataComp(((u32)p8rx)+1,((u32)p8tx)+4+1,6);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
							if(i==0)
							{//地址相同
								MeterAddr=MRR(((u32)p8rx)+1,6);
								SearchMeter_Record(PORTn,3,MeterAddr,1,0x0,ProtocolBps&0xf,0);//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
							}
							break;
						default:
							MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
							UARTCtrl->Task=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							return;
					}
					if(i==0)
					{//地址相同
						//下搜表地址
						if((ProtocolBps>>4)<3)//B4-B7:0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
						{//快速
							MR(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
							p8=(u8*)(ADDR_DATABUFF);
							for(i=0;i<6;i++)
							{
								if(p8[i]==0xaa)
								{
									break;
								}
							}
							if(i!=0)
							{
								i--;
							}
							while(1)
							{
								x=p8[i];
								if(x==0x99)
								{
									if(i!=0)
									{
										p8[i]=0xaa;
										i--;
										continue;
									}
									else
									{
										switch(ProtocolBps>>4)
										{
										//0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
											case 0://原DL645-2007快速
												MWR(0x16aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												break;
											case 1://原DL698快速
												MWR(0x22aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												break;
											default://原DL645-1997快速
												MWR(0x33aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												break;
										}
										return;
									}
								}
								break;
							}
							x=bcd_hex(x);
							x++;
							x=hex_bcd(x);
							p8[i]=x;
							MW(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
							break;
						}
					}
				default://2超时
				Over://有多佘字节接收作为超时
					UARTCtrl->Task=0;//空闲1次
					MR(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<6;i++)
					{
						if(p8[i]==0xaa)
						{
							break;
						}
					}
					if(i!=0)
					{
						i--;
					}
					if(p16fifo[0]==0)
					{//没收到任何字节
						while(1)
						{
							x=p8[i];
							if(x==0x99)
							{
								if(i==0)
								{//地址回0
									switch(ProtocolBps>>4)
									{
										case 0://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
											MWR(0x16aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 1://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
											MWR(0x22aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 2://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
											MWR(0x33aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 3://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
											MWR(0x46aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 4://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
											MWR(0x52aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 5://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
										default:
											if(UARTCtrl->AutoSearchTime==0)//自动搜表持续时间分，0表示不限时间直至搜表结束
											{
												UARTCtrl->TaskID=0;
												UARTCtrl->AutoSearchTask=0;//自动搜表任务:0=空,1=等待,2...过程
												return;
											}
											MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											MC(0,Get_ADDR_UARTn(PORTn),6);//请fifo指针
											return;
									}
								}
								else
								{
									p8[i]=0xaa;
									i--;
								}
							}
							else
							{
								break;
							}
						}
						x=bcd_hex(x);
						x++;
						x=hex_bcd(x);
						p8[i]=x;
					}
					else
					{//有收到字节
						if(i<5)
						{
							i++;
							p8[i]=0;
						}
						else
						{
							while(1)
							{
								x=p8[i];
								if(x==0x99)
								{
									if(i!=0)
									{
										p8[i]=0xaa;
										i--;
										continue;
									}
									else
									{
										switch(ProtocolBps>>4)
										{
											case 0://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
												MWR(0x16aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 1://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
												MWR(0x22aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 2://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
												MWR(0x33aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 3://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
												MWR(0x46aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 4://原0=DL645-2007快速,1=DL698快速,2=DL645-1997快速,3=DL645-2007准确,4=DL698准确,5=DL645-1997准确
												MWR(0x52aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 5://原0=DL645-2007快速,1=DL645-1997快速,2=DL698快速,3=DL645-2007准确,4=DL645-1997准确,5=DL698准确
											default:
												if(UARTCtrl->AutoSearchTime==0)//自动搜表持续时间分，0表示不限时间直至搜表结束
												{
													UARTCtrl->TaskID=0;
													UARTCtrl->AutoSearchTask=0;//自动搜表任务:0=空,1=等待,2...过程
													return;
												}
												MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												MC(0,Get_ADDR_UARTn(PORTn),6);//请fifo指针
												return;
										}
									}
								}
								break;
							}
							x=bcd_hex(x);
							x++;
							x=hex_bcd(x);
							p8[i]=x;
						}
					}
					MW(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					return;
			}
//			break;
	}
}
#endif




