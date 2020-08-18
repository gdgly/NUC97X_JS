
//终端抄表DL645_2007接收数据填入Fn数据缓冲
#include "Project.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_CountAndFlag.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"

#include "../terminal/Terminal_USER2xx.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_Event.h"



extern const u32 DL645_2007_RMLib[];
void Terminal_ReadMeter_DL645_2007_TxData(u32 PORTn,u8 *p8RMLib)//终端DL645-2007抄表发送数据
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	MC(0xfe,(u32)p8txbuff,4);
	p8txbuff[4+0]=0x68;
	//取通信地址
	if(PORTn==ACSAMPLEPORT)
	{//是内部交采口
		MC(0xaa,((u32)p8txbuff)+4+1,6);
	}
	else
	{
		MR(((u32)p8txbuff)+4+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	}
	p8txbuff[4+7]=0x68;
	p8txbuff[4+8]=0x11;
	p8txbuff[4+9]=0x4;
	//取DI	
	//i=DL645_2007_RMLib[UARTCtrl->FnCount];
	i=(u32)p8RMLib;
	i+=10+(5*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+4+10,i,4);
	//加0x33
	for(i=0;i<4;i++)
	{
		p8txbuff[4+10+i]+=0x33;
	}
	p8txbuff[4+15]=0x16;
	//CS计算
	p8txbuff[4+14]=0;
	for(i=0;i<14;i++)
	{
		p8txbuff[4+14]+=p8txbuff[4+i];
	}
	UARTCtrl->TxByte=4+16;//TX计数(需发送字节数)
}

void PORTn_ReadMeter_DL645_2007_TxCurveData(u32 PORTn,u32 DI,u64 YMDHM)//DL645-2007读负荷曲线数据帧
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	MC(0xfe,(u32)p8txbuff,4);
	p8txbuff[4+0]=0x68;
	//取通信地址
	if(PORTn==ACSAMPLEPORT)
	{//是内部交采口
		MC(0xaa,((u32)p8txbuff)+4+1,6);
	}
	else
	{
		MR(((u32)p8txbuff)+4+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	}
	p8txbuff[4+7]=0x68;
	p8txbuff[4+8]=0x11;
	p8txbuff[4+9]=10;
	MWR(DI,((u32)p8txbuff)+4+10,4);
	MWR(1,((u32)p8txbuff)+4+10+4,1);//读1个点
	MWR(YMDHM,((u32)p8txbuff)+4+10+4+1,5);
	//加0x33
	for(i=0;i<10;i++)
	{
		p8txbuff[4+10+i]+=0x33;
	}
	//CS计算
	p8txbuff[4+20]=0;
	for(i=0;i<20;i++)
	{
		p8txbuff[4+20]+=p8txbuff[4+i];
	}
	p8txbuff[4+21]=0x16;
	UARTCtrl->TxByte=4+22;//TX计数(需发送字节数)
}

void Change0xFFTo0xEE(u32 Addr,u32 Byte)//搜索0xFF变为0xee
{
	u32 i;
	u8 *p8;
	
	if(Byte>(256-16))
	{
		return;
	}
	p8=(u8*)Addr;
	for(i=0;i<Byte;i++)
	{
		if(p8[i]==0xff)
		{
			p8[i]=0xee;
		}
	}
}

void ReadMeter_DL645_2007_FnData(u32 PORTn,u8 *p8RMLib)//DL645_2007抄表接收数据填入Fn数据缓冲
{
	ReadMeter_DL645_2007_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲
}

void ReadMeter_DL645_2007_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//DL645_2007抄表接收数据填入Fn数据缓冲
{
	u32 i;
	u32 x;
	u32 y;
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
	#if ((USER/100)!=5)//非上海用户
	u32 z;
	#endif
#endif
#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
	u32 RMCount;
#endif
	u32 Pn;
	u8 * p8;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;
	u64 d64a;
	u64 d64b;
#if (((USER/100)==5)&&((Project/100)==2))//上海集中器
	u32 j;
#endif
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;
	
	//i=MRR(DL645_2007_RMLib[UARTCtrl->FnCount],1);
	i=p8RMLib[0];
	switch(i)
	{
		case 0://通用
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0:
					x=p8rxbuff[9];//取数据长度
					if(x<=4)
					{
						break;
					}
					x-=4;
					i=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表Fn数据缓冲长度
					i-=6;
					if(x>i)
					{
						x=i;
					}
					MR(((u32)p8FnData)+6,((u32)p8rxbuff)+14,x);
					p8FnData[0]=x;//已有长度
					break;
				default:
					x=p8rxbuff[9];//取数据长度
					if(x<=4)
					{
						break;
					}
					x-=4;
					i=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表Fn数据缓冲长度
					i-=6;
					if((x+p8FnData[0])>i)
					{
						x=i-p8FnData[0];
					}
					MR(((u32)p8FnData)+6+p8FnData[0],((u32)p8rxbuff)+14,x);
					p8FnData[0]+=x;//已有长度
					break;
			}
			break;
#if HourTask==1//重点户曲线抄读(仅对无曲线冻结的单相表)
		case 1://重点户曲线(仅对无曲线冻结的单相表)
//负荷记录起始码：A0H，A0H（或 E0H，E0H：本数据块不正确），2 字节； 
//负荷记录字节数：1 字节（十六进制）； 
//负荷记录存储时间：年、月、日、时、分,  5 字节；
//电压、电流、频率：17 字节； 
//块分隔码：AAH，1 字节； 
//有、无功功率：24 字节； 
//块分隔码：AAH，1 字节； 
//功率因数：8 字节； 
//块分隔码：AAH，1 字节； 
//有、无功总电能：16 字节； 
//块分隔码：AAH，1 字节； 
//四象限无功总电能：16 字节； 
//块分隔码：AAH，1 字节； 
//当前需量：6 字节； 
//块分隔码：AAH，1 字节；
//负荷记录累加校验码：1 字节（从第一个 A0H 开始到最后一个数据块结束码）；
//负荷记录结束码：E5H,1 字节。
			if(p8FnData[6]==0xee)
			{
				MC(0xff,((u32)p8FnData)+7+4,103);//没抄到的数据填0xff以备还可补抄
			}
			p8FnData[6]=107;
			p8FnData[7+4+0]=0xa0;
			p8FnData[7+4+1]=0xa0;
			p8FnData[7+4+3]=0;//分总是为0
			MR(((u32)p8FnData)+7+4+4,ADDR_TYMDHMS+2,4);//曲线点时标
			p8FnData[7+4+8+17]=0xaa;
			p8FnData[7+4+8+18+24]=0xaa;
			p8FnData[7+4+8+18+25+8]=0xaa;
			p8FnData[7+4+8+18+25+9+16]=0xaa;
			p8FnData[7+4+8+18+25+9+17+16]=0xaa;
			p8FnData[7+4+8+18+25+9+17+17+6]=0xaa;
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://A相电压
					MR(((u32)p8FnData)+7+4+8,((u32)p8rxbuff)+14,2);
					break;
				case 1://当前正向有功总电能示值
					MR(((u32)p8FnData)+7+4+8+18+25+9,((u32)p8rxbuff)+14,4);
					break;
				case 2://当前反向有功总电能示值
					MR(((u32)p8FnData)+7+4+8+18+25+9+4,((u32)p8rxbuff)+14,4);
					break;
				case 3://A相电流
					MR(((u32)p8FnData)+7+4+8+6,((u32)p8rxbuff)+14,3);
					break;
			}
			break;
#endif

#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
	#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
//F2 电能表主动上报事件
		case 2:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0:
					break;
				case (2+(96*2))://复位主动上报状态字
					//不读后续帧
					UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
					//正确响应后清需复位的位
					MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//复位主动上报状态字的位标志
					break;
				case 1://读主动上报状态字(仅当鼎信模块时)
				case (2+(96*2)+1)://读主动上报状态字
					//不读后续帧
					UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
					WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
					break;
				default://读次数和各事件
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
					{
						break;
					}
					Pn--;
					i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
					RMCount=DL645_2007_DI_To_RMCount(i);//主动上报抄读电能表的事件次数或1-10次记录的数据标识转为与周期抄读相同标识的的抄表计数;返回0xffff(65535)表示没找到
					if((UARTCtrl->RMCount&1)==0)//抄表命令计数
					{//读次数
						//不读后续帧
						UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
						if(i==0x04001501)
						{//是收到主动上报帧
							//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
						}
						else
						{
							if(RMCount==0xffff)
							{
								//清事件标志,免死循环
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
								return;
							}
							x=MRR(((u32)p8rxbuff)+14,3);//当前次数
							if(x==0xffffff)
							{//有些表空数据返回0xff
								x=0;
							}
							x=bcd_hex(x);
							//抄读了主动上报,周期抄读数置为0
							MWR(x,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2)),4);//新增次数=0
							//后跟多于3字节的数据保存
							i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
							switch(i)
							{
								case 0x03120000://41 B70需量超限次数的反向有功或4象限无功次数
									for(i=1;i<6;i++)
									{
										z=MRR(((u32)p8rxbuff)+14+(i*3),3);//当前次数
										if(z==0xffffff)
										{//有些表空数据返回0xff
											z=0;
										}
										z=bcd_hex(z);
										x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2))+(i*4),3);//原次数(初始化值0xff)
										if((x==0xffffff)||(z<x))
										{//初始化值,或当前次数<原次数可能换表
											MWR(z,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2))+(i*4),4);//新增次数=0
											continue;
										}
										y=z-x;//新增次数
										x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2))+(i*4)+3,1);//原次数(初始化值0xff)
										if(x==0xff)
										{
											x=0;
										}
										x+=y;
										if(x>10)
										{
											x=10;
										}
										z|=x<<24;
										MWR(z,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2))+(i*4),4);//新增
									}
									return;
								case 0x03060000://39 B68辅助电源失电次数后的总累计时间
									MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
									break;
								case 0x03050000://45 B74全失压次数后的总累计时间
									MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
									break;
							}
						}	
					}
					else
					{//读1-10次事件
						UARTCtrl->RMCount--;//抄表命令计数-1,加1在启动发送时检查本事件发生次数读完后进行
		#if ((USER/100)==5)//上海用户
					//存事件
						i=MRR((u32)p8FnData,2);
						if(i==0xeeee)
						{//第1帧标志或抄表计数不相同
							//长度
							i=p8rxbuff[9];
							p8FnData[0]=i;//2byte 事件数据长度
							p8FnData[1]=0;
							//数据
							MR(((u32)p8rxbuff)+512,((u32)p8rxbuff)+10,i);//数据标识和数据
						}
						else
						{//是后续帧
							if((p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2))
							{
								i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
								if(i==0x04001501)
								{//是收到主动上报帧
									i=MRR(((u32)p8rxbuff)+512,4);//取寄存的数据标识
									RMCount=DL645_2007_DI_To_RMCount(i);//主动上报抄读电能表的事件次数或1-10次记录的数据标识转为与周期抄读相同标识的的抄表计数;返回0xffff(65535)表示没找到
									//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
								}
								else
								{
									//长度
									x=p8FnData[0]+(p8FnData[1]<<8);//已有长度
									i=p8rxbuff[9];//当前长度
									if(i>=5)
									{
										i-=5;//续帧有1字节seq
									}
									else
									{
										i=0;
									}
									//还可填入长度,用UART4Rx最小2048字节做比较
									y=Get_LEN_UARTnRx(PORTn);//得到UART接收缓冲长度
									if(y>=512)
									{
										y-=512;
										if(y>=x)
										{
											y-=x;
										}
										else
										{
											y=0;
										}
									}
									else
									{
										y=0;
									}
									if(i>y)
									{
										i=y;
									}
									i+=x;
									p8FnData[0]=i;//2byte 事件数据长度
									p8FnData[1]=i>>8;
									//数据
									i-=x;
									MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+14,i);//256-8byte 数据标识和数据
								}
							}
						}
		#else//#if ((USER/100)==5)//上海用户
		//不是上海的电能表主动上报事件
					//存事件
						i=MRR((u32)p8FnData,2);
						if(i==0xeeee)
						{//第1帧标志或抄表计数不相同
							//长度
							i=p8rxbuff[9];
							i++;
							p8FnData[0]=i+1;//2byte 总事件数据长度
							p8FnData[1]=0;
							//数据
							p8rxbuff[512]=1;//总数据标识个数
							MR(((u32)p8rxbuff)+512+1,((u32)p8rxbuff)+9,i);//1Byte长度,nByte数据标识和数据
						}
						else
						{//是后续帧
							if((p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2))
							{
								i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
								if(i==0x04001501)
								{//是收到主动上报帧
									i=MRR(((u32)p8rxbuff)+512+2,4);//取寄存的数据标识
									RMCount=DL645_2007_DI_To_RMCount(i);//主动上报抄读电能表的事件次数或1-10次记录的数据标识转为与周期抄读相同标识的的抄表计数;返回0xffff(65535)表示没找到
									//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
								}
								else
								{
									//长度
									x=p8FnData[0]+(p8FnData[1]<<8);//已有长度
									i=p8rxbuff[9];//当前长度
									if(i>=5)
									{
										i-=5;//续帧有1字节seq
									}
									else
									{
										i=0;
									}
									//还可填入长度,用UART4Rx最小2048字节做比较
									y=Get_LEN_UARTnRx(PORTn);//得到UART接收缓冲长度
									if(y>=512)
									{
										y-=512;
										if(y>=x)
										{
											y-=x;
										}
										else
										{
											y=0;
										}
									}
									else
									{
										y=0;
									}
									if(i>y)
									{
										i=y;
									}
									i+=x;
									p8FnData[0]=i;//2byte 事件数据长度
									p8FnData[1]=i>>8;
									//数据
									i-=x;
									MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+14,i);//256-8byte 数据标识和数据
									//修改最后数据标识帧的长度
									if(p8rxbuff[512]>=1)
									{//已有数据标识个数>1
										//计算前已有完整数据的总长度
										p8rxbuff[512]--;
										x=1;//1字节总标识个数
										for(y=0;y<p8rxbuff[512];y++)
										{
											x+=p8rxbuff[512+x]+1;
										}
										p8rxbuff[512]++;
										p8rxbuff[512+x]+=i;
									}
								}
							}
						}
		#endif
						
						if((p8rxbuff[8]&0x20)==0)
						{//无后续帧
							//写
							switch(PORTn)
							{
								case ACSAMPLEPORT:
									i=1;
									break;
								case RS485_1PORT://RS485-1
									i=2;//RS485PORT=1;
									break;
								case RS485_2PORT://RS485-2
									i=3;//RS485PORT=2;
									break;
								case RS485_3PORT://RS485-3
									i=4;//RS485PORT=3;
									break;
								case RS485_4PORT://RS485-4
									i=31;
									break;
								default:
									i=0;
									break;
							}
							MWR(i,ADDR_DATABUFF,1);//1byte 端口号
							MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE 通信地址
							MWR(7,ADDR_DATABUFF+7,1);//发生次数长度
							//取读次数的DI	
							i=(u32)p8RMLib;
							i+=10+(5*UARTCtrl->RMCount);//UARTCtrl->RMCount上已-1
							MR(ADDR_DATABUFF+10,i,4);
							//取当前次数值
							if(RMCount==0xffff)
							{
								//清事件标志,免死循环
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
								return;
							}
							i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2)),3);
							i=hex_bcd(i);
							MWR(i,ADDR_DATABUFF+14,3);
							//次数数据标识多于3字节的
							i=MRR(((u32)p8rxbuff)+512+2,4);//取寄存的数据标识
							switch(i)
							{
								case 0x03120101://B70 需量超限
									MWR(22,ADDR_DATABUFF+7,1);//发生次数长度
									for(i=1;i<6;i++)
									{
										x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(RMCount/2)+(i*4)),3);
										x=hex_bcd(x);
										MWR(x,ADDR_DATABUFF+14+(i*3),3);
									}
									break;
								case 0x03060001://B68辅助电源失电
									MWR(10,ADDR_DATABUFF+7,1);//发生次数长度
									MR(ADDR_DATABUFF+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
									break;
								case 0x03050001://B74全失压
									MWR(10,ADDR_DATABUFF+7,1);//发生次数长度
									MR(ADDR_DATABUFF+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
									break;
							}
							//
							i=p8FnData[0]+(p8FnData[1]<<8);
							MWR(i,ADDR_DATABUFF+8,2);//2byte长度
							MR(ADDR_DATABUFF+32,((u32)p8rxbuff)+512,i);//数据
						#if ((USER/100)==10)//河北用户
						//开端纽盖事件河北终端只需上报第一个数据内容开盖次数，第二个开盖事件记录不需要。
							i=MRR(((u32)p8rxbuff)+512+2,4);//取寄存的数据标识
							switch(i)
							{
								default:
									Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
									break;
								//开端钮盖事件新增后只上报一次
								case 0x03300E02:
								case 0x03300E03:
								case 0x03300E04:
								case 0x03300E05:
								case 0x03300E06:
								case 0x03300E07:
								case 0x03300E08:
								case 0x03300E09:
								case 0x03300E0A:
									break;
							}
						#else
							Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
						#endif
							//需读计数-1
							i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-1)/2),1);//UARTCtrl->RMCount上已-1
							if(i>10)
							{
								i=10;
							}
							if(i!=0)
							{
								i--;
							}
							MWR(i,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-1)/2),1);//UARTCtrl->RMCount上已-1
							//清缓冲
							p8FnData[0]=0xee;//第1帧数据标志
							p8FnData[1]=0xee;
						}
					}
					break;
			}
			break;
	#endif//#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
//F3 周期抄读电能表事件
		case 3:
	#if ((USER/100)==5)//上海用户
		#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
			if(UARTCtrl->RMCount==((60)*3))
			{//读主动上报状态字bit5,bit7
				//不读后续帧
				UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
				WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
				break;
			}
			if(UARTCtrl->RMCount==((60)*3+1))
			{//复位主动上报状态字bit5,bit7
				//不读后续帧
				UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
				//正确响应后清需复位的位
//				MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//复位主动上报状态字的位标志
				i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,1);
				i&=0x5f;
				MC(i,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,1);
				break;
			}

			if(UARTCtrl->RMCount==((60)*3+3))
			{//读电表运行状态字
				//不读后续帧
				UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号

				if((p8rxbuff[9]!=(14+4))&&(p8rxbuff[9]!=(18+4)))
				{//0x040005ff老07规约长度是14，新07规约长度是18
					break;
				}
				x=MRR(((u32)p8rxbuff)+14,1);
				y=0;//上报标志：0=不上报，1=上报

				//主动上报状态字bit3时钟电池电压低
				//电表运行状态字1 bit2
				i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+3,1);
				if(x&0x04)
				{
					if(i==0xff)
					{//是第1次读状态字
						y=1;//上报标志：0=不上报，1=上报
					}
					else
					{
						if(i==0)
						{
							y=1;//上报标志：0=不上报，1=上报
						}
					}

					MWR(1,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+3,1);
				}
				else
				{
					MWR(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+3,1);
				}

				//主动上报状态字bit8停电抄表电池欠压
				//电表运行状态字1 bit3
				i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+8,1);
				if(x&0x08)
				{
					if(i==0xff)
					{//是第1次读状态字
						y=1;//上报标志：0=不上报，1=上报
					}
					else
					{
						if(i==0)
						{
							y=1;//上报标志：0=不上报，1=上报
						}
					}
					MWR(1,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+8,1);
				}
				else
				{
					MWR(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+8,1);
				}

				if(y==1)
				{//上报
					switch(PORTn)
					{
						case ACSAMPLEPORT:
							i=1;
							break;
						case RS485_1PORT://RS485-1
							i=2;//RS485PORT=1;
							break;
						case RS485_2PORT://RS485-2
							i=3;//RS485PORT=2;
							break;
						case RS485_3PORT://RS485-3
							i=4;//RS485PORT=3;
							break;
						case RS485_4PORT://RS485-4
							i=31;
							break;
						default:
							return;
					}
					MWR(i,ADDR_DATABUFF,1);//1byte 端口号
					MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE 通信地址
					MWR(0,ADDR_DATABUFF+7,1);//发生次数长度=0
			
					x=p8rxbuff[9];
					MWR(x,ADDR_DATABUFF+8,2);//2byte长度
					MR(ADDR_DATABUFF+32,((u32)p8rxbuff)+10,x);//数据
					Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
				}
				break;
			}
		#endif//#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				break;
			}
			Pn--;
			if((UARTCtrl->RMCount%3)==0)//抄表命令计数
			{//读次数
				UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
				i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
				if(i==0x04001501)
				{//是收到主动上报帧
					//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
				}
				else
				{
					i=MRR(((u32)p8rxbuff)+14,3);//当前次数
					if(i==0xffffff)
					{//有些表空数据返回0xff
						i=0;
					}
					i=bcd_hex(i);
					x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),3);//原次数(初始化值0xff)
					if((x==0xffffff)||(i<x))
					{//初始化值,或当前次数<原次数可能换表
						MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),4);//新增次数=0
//终端数据初始化后,当抄读到开表盖（次数不为0）、时钟电池欠压（发生状态）、停电抄表电池欠压（发生状态）时，终端需立即上报事件（总次数及对应内容）。
						y=MRR(((u32)p8rxbuff)+10,4);
						if(y==0x03300D00)
						{//开表盖次数
							if(x==0xffffff)
							{//初始化值
								if(i==0)
								{
									break;
								}
								x=0;
							}
							else
							{//当前次数<原次数可能换表
								break;
							}
						}
						else
						{
							break;
						}
					}
					y=i-x;//新增次数
					x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3))+3,1);//原次数(初始化值0xff)
					if(x==0xff)
					{
						x=0;
					}
					x+=y;
					if(x>10)
					{
						x=10;
					}
					//上海要求新增事件只上报最近一次
					if(x>1)
					{
						x=1;
					}
					i|=x<<24;
					MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),4);//新增次数
					//后跟多于3字节的数据保存
					i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
					switch(i)
					{
						case 0x03060000://39 B68辅助电源失电次数后的总累计时间
							MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
							break;
						case 0x03050000://44 B74全失压次数后的总累计时间
							MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
							break;
					}
				}
			}
			else
			{//读1-10次事件
				//存事件
				if((UARTCtrl->RMCount%3)==1)
				{//总累计时间
					i=MRR((u32)p8FnData,2);
					if(i==0xeeee)
					{//第1帧标志
						//长度
						i=p8rxbuff[9];
						p8FnData[0]=i;//2byte 事件数据长度
						p8FnData[1]=0;
						//数据
						MR(((u32)p8rxbuff)+512-7,((u32)p8rxbuff)+10,i);//数据标识和数据
					}
					
				}
				if((UARTCtrl->RMCount%3)==2)
				{
					i=MRR((u32)p8FnData+2,2);
					if(i==0xeeee)
					{//第1帧标志
						//长度
						i=p8rxbuff[9];
						p8FnData[2]=i;//2byte 事件数据长度
						p8FnData[3]=0;
						//数据
						MR(((u32)p8rxbuff)+512,((u32)p8rxbuff)+10,i);//数据标识和数据
					}
					else
					{//是后续帧
						if((p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2))
						{
							i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
							if(i==0x04001501)
							{//是收到主动上报帧
								//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
							}
							else
							{
								//长度
								x=p8FnData[2]+(p8FnData[3]<<8);//已有长度
								i=p8rxbuff[9];//当前长度
								if(i>=5)
								{
									i-=5;//续帧有1字节seq
								}
								else
								{
									i=0;
								}
								//还可填入长度,用UART4Rx最小2048字节做比较
								y=Get_LEN_UARTnRx(PORTn);//得到UART接收缓冲长度
								if(y>=512)
								{
									y-=512;
									if(y>=x)
									{
										y-=x;
									}
									else
									{
										y=0;
									}
								}
								else
								{
									y=0;
								}
								if(i>y)
								{
									i=y;
								}
								i+=x;
								p8FnData[2]=i;//2byte 事件数据长度
								p8FnData[3]=i>>8;
								//数据
								i-=x;
								MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+14,i);//数据标识和数据
							}
						}
					}
				}

				if((p8rxbuff[8]&0x20)==0)
				{//无后续帧
					if((UARTCtrl->RMCount%3)==2)
					{
						//写
						switch(PORTn)
						{
							case ACSAMPLEPORT:
								i=1;
								break;
							case RS485_1PORT://RS485-1
								i=2;//RS485PORT=1;
								break;
							case RS485_2PORT://RS485-2
								i=3;//RS485PORT=2;
								break;
							case RS485_3PORT://RS485-3
								i=4;//RS485PORT=3;
								break;
							case RS485_4PORT://RS485-4
								i=31;
								break;
							default:
								i=0;
								break;
						}
						MWR(i,ADDR_DATABUFF,1);//1byte 端口号
						MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE 通信地址
						i=p8FnData[0]+(p8FnData[1]<<8);
						if(i==0xeeee)
						{
							i=(u32)p8RMLib;
							i+=10+(5*(UARTCtrl->RMCount-2))+4;
							MR(ADDR_DATABUFF+7,i,1);
							p8=(u8 *)ADDR_DATABUFF+7;
							p8[0]+=4;
						}
						else
						{
							MWR(14,ADDR_DATABUFF+7,1);//发生次数长度
						}
						//取读次数的DI	
						i=(u32)p8RMLib;
						i+=10+(5*(UARTCtrl->RMCount-2));
						MR(ADDR_DATABUFF+10,i,4);
						//取当前次数值
						i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),3);
						i=hex_bcd(i);
						MWR(i,ADDR_DATABUFF+14,3);
						//后跟多于3字节的数据
						i=MRR(ADDR_DATABUFF+10,4);
						switch(i)
						{
							case 0x03060000://39 B68辅助电源失电次数后的总累计时间
								MR(ADDR_DATABUFF+17,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
								break;
							case 0x03050000://44 B74全失压次数后的总累计时间
								MR(ADDR_DATABUFF+17,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
								break;
						}
						i=p8FnData[0]+(p8FnData[1]<<8);
						if(i!=0xeeee)
						{
							//读总累计时间的DI
							//总累计时间值
							MR(ADDR_DATABUFF+17,((u32)p8rxbuff)+512-7,7);//数据
						}
						//
						i=p8FnData[2]+(p8FnData[3]<<8);
						MWR(i,ADDR_DATABUFF+8,2);//2byte长度
						MR(ADDR_DATABUFF+32,((u32)p8rxbuff)+512,i);//数据
						Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
						//需读计数-1
						i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3))+3,1);//新增次数(初始化值0xff)
						if(i==0xff)
						{
							i=0;
						}
						if(i>10)
						{
							i=10;
						}
						if(i!=0)
						{
							i--;
						}
						MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3))+3,1);//新增次数(初始化值0xff)
						//清缓冲
						p8FnData[0]=0xee;//第1帧数据标志
						p8FnData[1]=0xee;
						if(i!=0)
						{//次数没到0,继续读本帧
							UARTCtrl->RMCount--;//抄表命令计数-1
						}
					}
					return;
				}
				if((p8rxbuff[8]&0x20)!=0)
				{//有后续帧
					UARTCtrl->RMCount--;//抄表命令计数-1
				}
			}
	#else//#if ((USER/100)==5)//上海用户
	//不是上海用户的电能表事件采集
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				break;
			}
			Pn--;
			if(((UARTCtrl->RMCount&1)==0)&&(UARTCtrl->RMDI==0xffffffff))
			{//读次数
				UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
				i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
				if(i==0x04001501)
				{//是收到主动上报帧
					//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
				}
				else
				{
					i=MRR(((u32)p8rxbuff)+14,3);//当前次数
					if(i==0xffffff)
					{//有些表空数据返回0xff
						i=0;
					}
					i=bcd_hex(i);
					x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2)),3);//原次数(初始化值0xff)
					if((x==0xffffff)||(i<x))
					{//初始化值,或当前次数<原次数可能换表
						MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2)),4);//新增次数=0
						break;
					}
					y=i-x;//新增次数
					x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+3,1);//原次数(初始化值0xff)
					if(x==0xff)
					{
						x=0;
					}
					x+=y;
					if(x>10)
					{
						x=10;
					}
					i|=x<<24;
					MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2)),4);//新增次数
					//后跟多于3字节的数据保存
					i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
					switch(i)
					{
						case 0x03120000://41 B70需量超限次数的反向有功或4象限无功次数
							for(i=1;i<6;i++)
							{
								z=MRR(((u32)p8rxbuff)+14+(i*3),3);//当前次数
								if(z==0xffffff)
								{//有些表空数据返回0xff
									z=0;
								}
								z=bcd_hex(z);
								x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+(i*4),3);//原次数(初始化值0xff)
								if((x==0xffffff)||(z<x))
								{//初始化值,或当前次数<原次数可能换表
									MWR(z,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+(i*4),4);//新增次数=0
									continue;
								}
								y=z-x;//新增次数
								x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+(i*4)+3,1);//原次数(初始化值0xff)
								if(x==0xff)
								{
									x=0;
								}
								x+=y;
								if(x>10)
								{
									x=10;
								}
								z|=x<<24;
								MWR(z,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+(i*4),4);//新增
							}
							return;
						case 0x03060000://39 B68辅助电源失电次数后的总累计时间
							MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
							break;
						case 0x03050000://45 B74全失压次数后的总累计时间
							MW(((u32)p8rxbuff)+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
							break;
					}
				}
			}
			else
			{//读1-10次事件,或非事件类数据
				i=MRR((u32)p8FnData,2);
				if(i==0xeeee)
				{//第1帧标志
					//长度
					i=p8rxbuff[9];
					i++;
					p8FnData[0]=i+1;//2byte 总事件数据长度
					p8FnData[1]=0;
					//数据
					p8rxbuff[512]=1;//总数据标识个数
					MR(((u32)p8rxbuff)+512+1,((u32)p8rxbuff)+9,i);//1Byte长度,nByte数据标识和数据
				}
				else
				{//是后续帧或第n开始帧
					i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
					if(i==0x04001501)
					{//是收到主动上报帧
						//WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
					}
					else
					{//是后续帧或第n开始帧
						if((p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2))
						{//是后续帧
							//长度
							x=p8FnData[0]+(p8FnData[1]<<8);//已有长度
							i=p8rxbuff[9];//当前长度
							if((x+i)>(LEN_MeterEvent-10-22))
							{//总长度>FLASH能存储的长度,先存以前读取的
								if(p8rxbuff[512]>1)
								{//已有数据标识个数>1
									//计算前已有完整数据的总长度
									p8rxbuff[512]--;
									x=1;//1字节总标识个数
									for(y=0;y<p8rxbuff[512];y++)
									{
										x+=p8rxbuff[512+x]+1;
									}
									p8FnData[0]=x;//2byte 总事件数据长度
									p8FnData[1]=x>>8;
									Meter_Event_WriteBuff(PORTn,Pn,Addr_FnDataBuff,p8RMLib);//读取的电能表事件写入缓冲ADDR_DATABUFF
									Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
									//移动最后未完整帧到开始
									y=p8rxbuff[512+x]+1;
									MR(((u32)p8rxbuff)+512+1,((u32)p8rxbuff)+x,y);
									x=y+1;//已有长度
									p8rxbuff[512]=1;//总数据标识个数
								}
							}
							if(i>=5)
							{
								i-=5;//续帧有1字节seq
							}
							else
							{
								i=0;
							}
							//还可填入长度,用UART4Rx最小2048字节做比较
							y=Get_LEN_UARTnRx(PORTn);//得到UART接收缓冲长度
							if(y>=512)
							{
								y-=512;
								if(y>=x)
								{
									y-=x;
								}
								else
								{
									y=0;
								}
							}
							else
							{
								y=0;
							}
							if(i>y)
							{
								i=y;
							}
							i+=x;
							p8FnData[0]=i;//2byte 事件数据长度
							p8FnData[1]=i>>8;
							//数据
							i-=x;
							MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+14,i);//数据标识和数据
							//修改最后数据标识帧的长度
							if(p8rxbuff[512]>=1)
							{//已有数据标识个数>1
								//计算前已有完整数据的总长度
								p8rxbuff[512]--;
								x=1;//1字节标识个数
								for(y=0;y<p8rxbuff[512];y++)
								{
									x+=p8rxbuff[512+x]+1;
								}
								p8rxbuff[512]++;
								p8rxbuff[512+x]+=i;
							}
						}
						else
						{//第n开始帧
							//长度
							x=p8FnData[0]+(p8FnData[1]<<8);//已有长度
							i=p8rxbuff[9];//当前长度
							i++;
							if((x+i)>(LEN_MeterEvent-10-22))
							{//总长度>FLASH能存储的长度,先存以前读取的
								Meter_Event_WriteBuff(PORTn,Pn,Addr_FnDataBuff,p8RMLib);//读取的电能表事件写入缓冲ADDR_DATABUFF
								//事件数据FLASH能存储的长度不够的当前只有非事件类的很多个标识设置的情况
								Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
								p8rxbuff[512]=0;//总数据标识个数
								x=1;//已有长度(1字节总数据标识个数)
							}
							//还可填入长度,用UART4Rx最小2048字节做比较
							y=Get_LEN_UARTnRx(PORTn);//得到UART接收缓冲长度
							if(y>=512)
							{
								y-=512;
								if(y>=x)
								{
									y-=x;
								}
								else
								{
									y=0;
								}
							}
							else
							{
								y=0;
							}
							if(i>y)
							{
								i=y;
							}
							i+=x;
							p8FnData[0]=i;//2byte 事件数据长度
							p8FnData[1]=i>>8;
							//数据
							i-=x;
							p8rxbuff[512]+=1;//总数据标识个数
							MR(((u32)p8rxbuff)+512+x,((u32)p8rxbuff)+9,i);//1Byte长度,nByte数据标识和数据
						}
					}
				}
				if((p8rxbuff[8]&0x20)!=0)
				{//有后续帧
					UARTCtrl->RMCount--;//抄表命令计数-1
					return;
				}
				//无后续帧
				
				//已抄DI计数
				UARTCtrl->SearchDICount++;//周期事件抄读AFN04F106设置搜索计数
				UARTCtrl->RMDICount++;//周期事件抄读已抄DI计数
				if(UARTCtrl->RMDICount<UARTCtrl->TotalDICount)
				{//已抄DI计数<总需抄数
					UARTCtrl->RMCount--;//抄表命令计数-1
					return;
				}
				
				//存事件
				Meter_Event_WriteBuff(PORTn,Pn,Addr_FnDataBuff,p8RMLib);//读取的电能表事件写入缓冲ADDR_DATABUFF
			#if ((USER/100)==10)//河北用户
			//河北终端只需上报第一个数据内容开盖次数，第二个开盖事件记录不需要。
				i=MRR(((u32)p8rxbuff)+512+2,4);//取寄存的数据标识
				switch(i)
				{
					default:
						Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
						break;
					//开端钮盖事件新增后只上报一次
					case 0x03300E02:
					case 0x03300E03:
					case 0x03300E04:
					case 0x03300E05:
					case 0x03300E06:
					case 0x03300E07:
					case 0x03300E08:
					case 0x03300E09:
					case 0x03300E0A:
						break;
				}
			#else//#if ((USER/100)==10)//河北用户
				Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
			#endif
				//需读发生次数-1
				i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+3,1);//新增次数(初始化值0xff)
				if(i==0xff)
				{
					i=0;
				}
				if(i>10)
				{
					i=10;
				}
				if(i!=0)
				{
					i--;
				}
				MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2))+3,1);//新增次数(初始化值0xff)
				//清缓冲
				p8FnData[0]=0xee;//第1帧数据标志
				p8FnData[1]=0xee;
				
				UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
				UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
				UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
			}
	#endif//#if ((USER/100)==5)//上海用户
			break;
#endif//#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则


//1 F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
		case 25:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			y=MRR(ADDR_AFN04F25+((Pn-1)*11)+10,1);//取电源接线方式 BS8  1
			y&=0x3;//D0～D1编码表示测量点电源接线方式，数值1～3依次表示三相三线、三相四线、单相表
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总,A,B,C有功功率
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+6,6);//搜索0xFF变为0xee
					x=p8rxbuff[9];//取数据长度
					x-=4;//4字节DI标识
					if(x>12)
					{
						x=12;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5,x);
					if(y==1)
					{//三相三线
						MC(0xee,((u32)p8FnData)+1+5+6,3);
					}
#if (USER/100)==2//用户标识:山东版
					i=MRR(((u32)p8rxbuff)+14,3);
					DayAveragePower(Pn,i,1);//日平均正有功功率计算;入口Pn从1开始,Power=有功功率(BCD),Count=本次计算次数
#endif
					break;
				case 1://总,A,B,C无功功率
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+6,6);//搜索0xFF变为0xee
					x=p8rxbuff[9];//取数据长度
					x-=4;//4字节DI标识
					if(x>12)
					{
						x=12;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12,x);
					if(y==1)
					{//三相三线
						MC(0xee,((u32)p8FnData)+1+5+12+6,3);
					}
					break;
				case 2://总,A,B,C功率因数
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+4,4);//搜索0xFF变为0xee
					x=p8rxbuff[9];//取数据长度
					x-=4;//4字节DI标识
					if(x>8)
					{
						x=8;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12,x);
					if(y==1)
					{//三相三线
						MC(0xee,((u32)p8FnData)+1+5+12+12+4,2);
					}
					break;
				case 3://A,B,C相电压
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+2,4);//搜索0xFF变为0xee
					x=p8rxbuff[9];//取数据长度
					x-=4;//4字节DI标识
					if(x>6)
					{
						x=6;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12+8,x);
					if(y==1)
					{//三相三线
						MC(0xee,((u32)p8FnData)+1+5+12+12+8+2,2);
					}
					break;
				case 4://A,B,C相电流
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+3,6);//搜索0xFF变为0xee
					x=p8rxbuff[9];//取数据长度
					x-=4;//4字节DI标识
					if(x>9)
					{
						x=9;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12+8+6,x);
					if(y==1)
					{//三相三线
						MC(0xee,((u32)p8FnData)+1+5+12+12+8+6+3,3);
					}
					break;
				case 5://零序电流			
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12+8+6+9,3);
					break;
				case 6://总,A,B,C视在功率
					Change0xFFTo0xEE(((u32)p8rxbuff)+14+6,6);//搜索0xFF变为0xee
					x=p8rxbuff[9];//取数据长度
					x-=4;//4字节DI标识
					if(x>12)
					{
						x=12;
					}
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+12+12+8+6+9+3,x);
					if(y==1)
					{//三相三线
						MC(0xee,((u32)p8FnData)+1+5+12+12+8+6+9+3+6,3);
					}
					break;
			}
			break;
/*
//1 F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
		case 25:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总有功功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://A有功功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 2://B有功功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 3://C有功功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 4://总无功功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12,3);
					break;
				case 5://A无功功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12+3,3);
					break;
				case 6://B无功功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12+6,3);
					break;
				case 7://C无功功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12+9,3);
					break;
				case 8://总功率因数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+24,2);
					break;
				case 9://A功率因数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+24+2,2);
					break;
				case 10://B功率因数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+24+4,2);
					break;
				case 11://C功率因数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+24+6,2);
					break;
				case 12://A相电压
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+32,2);
					break;
				case 13://B相电压
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+32+2,2);
					break;
				case 14://C相电压
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+32+4,2);
					break;
				case 15://A相电流
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+38,3);
					break;
				case 16://B相电流
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+38+3,3);
					break;
				case 17://C相电流
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+38+6,3);
					break;
				case 18://零序电流
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+47,3);
					break;
				case 19://总视在功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50,3);
					break;
				case 20://A视在功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50+3,3);
					break;
				case 21://B视在功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50+6,3);
					break;
				case 22://C视在功率
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50+9,3);
					break;
			}
			break;
*/
//2 F26 A、B、C三相断相统计数据及最近一次断相记录
#if RMM_AFN0CF26==0//AFN0CF26断相数据抄表;0=抄电表断相记录数据,1=抄电表失压记录数据
		//抄电表断相记录数据
		case 26:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://A相断相次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 1://A相断相时间累计值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+15,3);
					break;
				case 2://B相断相次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 3://B相断相时间累计值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18,3);
					break;
				case 4://C相断相次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 5://C相断相时间累计值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+21,3);
					//总断相次数计算
					x=MRR(((u32)p8FnData)+6+3,3);
					if(x!=0xeeeeee)
					{
						i=bcd_hex(x);
						x=MRR(((u32)p8FnData)+6+6,3);
						if(x!=0xeeeeee)
						{
							i+=bcd_hex(x);
							x=MRR(((u32)p8FnData)+6+9,3);
							if(x!=0xeeeeee)
							{
								i+=bcd_hex(x);
								i=hex_bcd(i);
								MWR(i,((u32)p8FnData)+6,3);
							}
						}
					}
					//时间累计值计算
					x=MRR(((u32)p8FnData)+6+15,3);
					if(x!=0xeeeeee)
					{
						i=bcd_hex(x);
						x=MRR(((u32)p8FnData)+6+18,3);
						if(x!=0xeeeeee)
						{
							i+=bcd_hex(x);
							x=MRR(((u32)p8FnData)+6+21,3);
							if(x!=0xeeeeee)
							{
								i+=bcd_hex(x);
								i=hex_bcd(i);
								MWR(i,((u32)p8FnData)+6+12,3);
							}
						}
					}
					break;
				case 6://A相最近断相起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+56,6);//暂存A最近断相起始时刻YMDHMS
					break;
				case 7://A相最近断相结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+44,4);
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+56+12,6);//暂存A最近断相结束时刻YMDHMS
					break;
				case 8://B相最近断相起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+56+6,6);//暂存B最近断相起始时刻YMDHMS
					break;
				case 9://B相最近断相结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+48,4);
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+56+18,6);//暂存B最近断相结束时刻YMDHMS
					break;
				case 10://C相最近断相起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					//最近断相起始时刻计算
					d64a=MRR(((u32)p8rxbuff)+14,6);
					if(d64a!=0xeeeeeeeeeeee)
					{
						d64b=MRR(((u32)p8FnData)+6+56,6);
						if(d64b!=0xeeeeeeeeeeee)
						{
							if(d64b>d64a)
							{
								d64a=d64b;
							}
							d64b=MRR(((u32)p8FnData)+6+56+6,6);
							if(d64b!=0xeeeeeeeeeeee)
							{
								if(d64b>d64a)
								{
									d64a=d64b;
								}
								d64a>>=8;
								MWR(d64a,((u32)p8FnData)+6+24,4);
							}
						}
					}
					break;
				case 11://C相最近断相结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+52,4);
					//最近断相结束时刻计算
					d64a=MRR(((u32)p8rxbuff)+14,6);
					if(d64a!=0xeeeeeeeeeeee)
					{
						d64b=MRR(((u32)p8FnData)+6+56+12,6);
						if(d64b!=0xeeeeeeeeeeee)
						{
							if(d64b>d64a)
							{
								d64a=d64b;
							}
							d64b=MRR(((u32)p8FnData)+6+56+18,6);
							if(d64b!=0xeeeeeeeeeeee)
							{
								if(d64b>d64a)
								{
									d64a=d64b;
								}
								d64a>>=8;
								MWR(d64a,((u32)p8FnData)+6+40,4);
							}
						}
					}
					break;
				//当以上命令不成功时为兼容用以下命令读
				case (12+0)://A,B,C相断相次数,累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					MW(((u32)p8rxbuff)+14+3,((u32)p8FnData)+6+15,3);
					MW(((u32)p8rxbuff)+14+6,((u32)p8FnData)+6+6,3);
					MW(((u32)p8rxbuff)+14+9,((u32)p8FnData)+6+18,3);
					MW(((u32)p8rxbuff)+14+12,((u32)p8FnData)+6+9,3);
					MW(((u32)p8rxbuff)+14+15,((u32)p8FnData)+6+21,3);
					break;
				case (12+1)://A相最近断相起始时刻,A相最近断相结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+44,4);
					break;
				case (12+2)://B相最近断相起始时刻,A相最近断相结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+48,4);
					break;
				case (12+3)://C相最近断相起始时刻,A相最近断相结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+52,4);
					break;
			}
			break;
#endif
#if RMM_AFN0CF26==1//AFN0CF26断相数据抄表;0=抄电表断相记录数据,1=抄电表失压记录数据
//	#if (USER/100)!=6//不是湖南系
		//抄电表失压记录数据
		case 26:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://A失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 2://B失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 3://C失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 4://总失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12,3);
					break;
				case 5://A失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+15,3);
					break;
				case 6://B失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18,3);
					break;
				case 7://C失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+21,3);
					break;
				case 8://总最近失压起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+24,4);
					break;
				case 9://A最近失压起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					break;
				case 10://B最近失压起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					break;
				case 11://C最近失压起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					break;
				case 12://总最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+40,4);
					break;
				case 13://A最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+44,4);
					break;
				case 14://B最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+48,4);
					break;
				case 15://C最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+52,4);
					break;
				//当以上命令不成功时为兼容用以下命令读
				case 16://A,B,C相失压次数,累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					MW(((u32)p8rxbuff)+14+3,((u32)p8FnData)+6+15,3);
					MW(((u32)p8rxbuff)+14+6,((u32)p8FnData)+6+6,3);
					MW(((u32)p8rxbuff)+14+9,((u32)p8FnData)+6+18,3);
					MW(((u32)p8rxbuff)+14+12,((u32)p8FnData)+6+9,3);
					MW(((u32)p8rxbuff)+14+15,((u32)p8FnData)+6+21,3);
					break;
				case 17://A相最近失压起始时刻,A相最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+44,4);
					break;
				case 18://B相最近失压起始时刻,B相最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+48,4);
					break;
				case 19://C相最近失压起始时刻,c相最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+52,4);
					break;
			}
			break;
/*
	#endif
	#if (USER/100)==6//是湖南系
		//抄电表失压记录数据
		case 26:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://A失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 2://B失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 3://C失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 4://总失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+12,3);
					break;
				case 5://A失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+15,3);
					break;
				case 6://B失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18,3);
					break;
				case 7://C失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+21,3);
					break;
				case 8://总最近失压起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+24,4);
					break;
				case 9://A最近失压起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					break;
				case 10://B最近失压起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					break;
				case 11://C最近失压起始时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					break;
				case 12://总最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+40,4);
					break;
				case 13://A最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+44,4);
					break;
				case 14://B最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+48,4);
					break;
				case 15://C最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+52,4);
					break;
				case 16://A失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,3);
					break;
				case 17://B失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,3);
					break;
				case 18://C失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+9,3);
					break;
				case 19://A失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+15,3);
					break;
				case 20://B失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18,3);
					break;
				case 21://C失压累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+21,3);
					break;
				case 22://A相最近失压起始时刻,A相最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+28,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+44,4);
					break;
				case 23://B相最近失压起始时刻,B相最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+32,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+48,4);
					break;
				case 24://C相最近失压起始时刻,c相最近失压结束时刻
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+36,4);
					MW(((u32)p8rxbuff)+14+6+1,((u32)p8FnData)+6+52,4);
					break;
			}
			break;
	#endif
*/
#endif
		
//3 F27 电能表日历时钟、编程次数及其最近一次操作时间
		case 27:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表日历时钟Hhmmss
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5,3);
					break;
				case 1://电能表日历时钟YYMMDDWW
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+1+5+3,3);
					//星期
					i=p8rxbuff[14];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					p8FnData[1+5+3+1]|=i;
					break;
				case 2://电池工作时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3,4);
					break;
				case 3://编程总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4,3);
					break;
				case 4://最近一次编程发生时刻
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3,6);
					break;
				case 5://电表清零总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6,3);
					break;
				case 6://最近一次清零发生时刻
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3,6);
					break;
				case 7://需量清零总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6,3);
					break;
				case 8://最近一次清零发生时刻
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3,6);
					break;
				case 9://事件清零总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3+6,3);
					break;
				case 10://最近一次清零发生时刻
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3+6+3,6);
					break;
				case 11://校时总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3+6+3+6,3);
					break;
				case 12://最近一次校时发生时刻(用校时后时间)
					MW(((u32)p8rxbuff)+14+4+6,((u32)p8FnData)+1+5+3+3+4+3+6+3+6+3+6+3+6+3,6);
					break;
			}
			break;
//4 F28 电表运行状态字及其变位标志
		case 28:
			MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+5+14,14);
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//本序号的电能表/交流采样装置删除
				break;
			}
			x=p8FnData[1+5+14+12];
			x&=0x3;
			if(x!=0x0)
			{//电压或电流逆相序
				x=1;
			}
			ERC11_Event(Pn,x);//相序异常,入口:Pn测量点号(从1开始),f=0恢复=1产生
			
			break;
//5 F29 当前铜损、铁损有功总电能示值
//6 F30 上一结算日铜损、铁损有功总电能示值
		case 29:
		case 30:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0:
					p8FnData[6]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+1,4);
					break;
				case 1:
					p8FnData[6+5]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+6,4);
					break;
			}
			break;
//7 F31 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
//8 F32 上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
		case 31:
		case 32:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://当前A相正向有功电能示值
					p8FnData[6]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+1,4);
					break;
				case 1://当前A相反向有功电能示值
					p8FnData[6+5]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+5+1,4);
					break;
				case 2://当前A相组合无功1电能示值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+10,4);
					break;
				case 3://当前A相组合无功2电能示值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+14,4);
					break;
				case 4://当前B相正向有功电能示值
					p8FnData[6+18]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+18+1,4);
					break;
				case 5://当前B相反向有功电能示值
					p8FnData[6+23]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+23+1,4);
					break;
				case 6://当前B相组合无功1电能示值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+28,4);
					break;
				case 7://当前B相组合无功2电能示值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+32,4);
					break;
				case 8://当前C相正向有功电能示值
					p8FnData[6+36]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+36+1,4);
					break;
				case 9://当前C相反向有功电能示值
					p8FnData[6+41]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+41+1,4);
					break;
				case 10://当前C相组合无功1电能示值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+46,4);
					break;
				case 11://当前C相组合无功2电能示值
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+50,4);
					break;
			}
			break;

//9 F49 当前电压、电流相位角
		case 49:
			//Uab/Ua相位角	见附录A.5	度	2
			MWR(0,((u32)p8FnData)+1,2);
			//Ub相位角	见附录A.5	度	2
			MWR(0x1200,((u32)p8FnData)+1+2,2);
			//Ucb/Uc相位角	见附录A.5	度	2
			MWR(0x2400,((u32)p8FnData)+1+4,2);
			//Ia相位角	见附录A.5	度	2
			MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+6,2);
			//Ib相位角	见附录A.5	度	2
			i=MRR(((u32)p8rxbuff)+14+2,2);
			i=bcd_hex(i);
			i+=1200;
			if(i>=3600)
			{
				i-=3600;
			}
			i=hex_bcd(i);
			MWR(i,((u32)p8FnData)+1+8,2);
			//Ic相位角	见附录A.5	度	2
			i=MRR(((u32)p8rxbuff)+14+4,2);
			i=bcd_hex(i);
			i+=2400;
			if(i>=3600)
			{
				i-=3600;
			}
			i=hex_bcd(i);
			MWR(i,((u32)p8FnData)+1+10,2);
			break;
//扩展17 当前电压、电流相位角(扩展用于终端内部交采)
		case 0xF1:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://当前电压、电流相位角
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1,12);
					break;
			}
			break;
//10 F57 当前A、B、C三相电压、电流2～N次谐波有效值
		case 57:
			i=UARTCtrl->RMCount;//抄表命令计数
			if(i<6)
			{
				p8FnData[1]=19;
				MW(((u32)p8rxbuff)+14+2,((u32)p8FnData)+2+(36*i),36);
			}
			break;
//11 F58 当前A、B、C三相电压、电流2～N次谐波含有率
		case 58:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电压波形失真度数据块
					p8FnData[1]=19;
					for(i=0;i<3;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+(2*i),2);
						x>>=4;//2位小数删去1位
						MWR(x,((u32)p8FnData)+2+(38*i),2);
					}
					break;
				case 1://A 相电压谐波含量数据块
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2位小数删去1位
						MWR(x,((u32)p8FnData)+2+2+(2*i),2);
					}
					break;
				case 2://B 相电压谐波含量数据块
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2位小数删去1位
						MWR(x,((u32)p8FnData)+2+38+2+(2*i),2);
					}
					break;
				case 3://C 相电压谐波含量数据块
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2位小数删去1位
						MWR(x,((u32)p8FnData)+2+38+38+2+(2*i),2);
					}
					break;
				case 4://A 相电流谐波含量数据块
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2位小数删去1位
						MWR(x,((u32)p8FnData)+2+38+38+38+(2*i),2);
					}
					break;
				case 5://B 相电流谐波含量数据块
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2位小数删去1位
						MWR(x,((u32)p8FnData)+2+38+38+38+36+(2*i),2);
					}
					break;
				case 6://C 相电流谐波含量数据块
					for(i=0;i<18;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+2+(2*i),2);
						x>>=4;//2位小数删去1位
						MWR(x,((u32)p8FnData)+2+38+38+38+36+36+(2*i),2);
					}
					break;
			}
			break;
//12 F65 当前电容器投切状态
//13 F66 当前电容器累计补偿投入时间和次数
//14 F67 当日、当月电容器累计补偿的无功电能量
//15 F73 直流模拟量当前数据


//16 F129 当前正向有功电能示值（总、费率1～M）
//24 F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
//扩展0xD1 上2月正向有功电能示值（总、费率1～M）
		case 129:
		case 137:
		case 0xd1:
			//if(i==129)
			//{
				UARTCtrl->RMError&=(~0x02);//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			//}
			#if (((USER/100)==5)&&((Project/100)==2))//上海集中器
				switch(UARTCtrl->RMCount)//抄表命令计数
				{

					case 0://组合有功 ,大小类号（5,1）或（5,2）抄读组合有功
		 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
						switch(i)
						{
							case 0x51:
							case 0x52:
								goto F129DATA;
							case 0x02:
							case 0x62:
							case 0x71:
							case 0x72:
								break;
							default://其他按大类号5小类号1处理
								goto F129DATA;
						}
						break;
					case 1://正向有功，大小类号（0,2）（6,2）（7,1）或（7,2）抄读正向有功
		 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
						switch(i)
						{
							case 0x51:
							case 0x52:
								break;
							case 0x02:
							case 0x62:
							case 0x71:
							case 0x72:
								goto F129DATA;
							default://其他按大类号5小类号1处理
								break;
						}
						break;
				}
			#else
				switch(UARTCtrl->RMCount)//抄表命令计数
				{
					case 0://块命令抄
						//有块命令跳过下步只抄总电量
						UARTCtrl->RMCount=1;//抄表命令计数+1
						goto F129DATA;
					case 1://只抄总电量
					/*
						p8FnData[6]=1;//费率数=1
						p8FnData+=7;
						p8rxbuff+=14;
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						MW((u32)p8FnData,((u32)p8FnData)+5,5);//费率1数据=总
						break;
					*/
						goto F129DATA;
				}
			#endif
			break;
//18 F131 当前反向有功电能示值（总、费率1～M）
//26 F139 上月（上一结算日）反向有功电能示值（总、费率1～M）
//40 F153 第一时区冻结正向有功电能示值（总、费率1～M）
//41 F154 第二时区冻结正向有功电能示值（总、费率1～M）
//42 F155 第三时区冻结正向有功电能示值（总、费率1～M）
//43 F156 第四时区冻结正向有功电能示值（总、费率1～M）
//44 F157 第五时区冻结正向有功电能示值（总、费率1～M）
//45 F158 第六时区冻结正向有功电能示值（总、费率1～M）
//46 F159 第七时区冻结正向有功电能示值（总、费率1～M）
//47 F160 第八时区冻结正向有功电能示值（总、费率1～M）
// F177 当前组合有功电能示值（总、费率1~M）
// F178 上1 结算日组合有功电能示值（总、费率1~M）
//扩展0xD3 上2月反向有功电能示值（总、费率1～M）
		case 131:
		case 139:
		case 153:
		case 154:
		case 155:
		case 156:
		case 157:
		case 158:
		case 159:
		case 160:
		case 177:
		case 178:
		case 0xd3:
F129DATA:
			x=p8rxbuff[9];
			if(x>=8)
			{
				x-=8;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				p8FnData+=7;
				p8rxbuff+=14;
				for(i=0;i<=x;i++)
				{
					p8FnData[0+(5*i)]=0;
					p8FnData[1+(5*i)]=p8rxbuff[0];
					p8FnData[2+(5*i)]=p8rxbuff[1];
					p8FnData[3+(5*i)]=p8rxbuff[2];
					p8FnData[4+(5*i)]=p8rxbuff[3];
					p8rxbuff+=4;
				}
				if(x==0)
				{//费率数=0改为1
					p8FnData-=7;
					p8FnData[6]=1;//费率数=1
					p8FnData+=7;
					MW((u32)p8FnData,((u32)p8FnData)+5,5);//费率1数据=总
				}
			}
			break;
//17 F130 当前正向无功（组合无功1）电能示值（总、费率1～M）
//19 F132 当前反向无功（组合无功2）电能示值（总、费率1～M）
//25 F138 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
//27 F140 上月（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
//扩展0xD2 上2月正向无功（组合无功1）电能示值（总、费率1～M）
//扩展0xD4 上2月反向无功（组合无功2）电能示值（总、费率1～M）
		case 130:
		case 132:
		case 138:
		case 140:
		case 0xd2:
		case 0xd4:
			x=p8rxbuff[9];
			if(x>=8)
			{
				x-=8;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				Pn=(x+1)*4;
				p8FnData+=7;
				p8rxbuff+=14;
				for(i=0;i<Pn;i++)
				{
					p8FnData[i]=p8rxbuff[i];
				}
//				//去组合无功最高位符号
//				for(i=0;i<(x+1);i++)
//				{
//					p8FnData[(4*i)+3]&=0x7f;
//				}
				if(x==0)
				{//费率数=0改为1
					p8FnData-=7;
					p8FnData[6]=1;//费率数=1
					p8FnData+=7;
					MW((u32)p8FnData,((u32)p8FnData)+4,4);//费率1数据=总
				}
			}
			break;
//20 F133 当前一象限无功电能示值（总、费率1～M）
//21 F134 当前二象限无功电能示值（总、费率1～M）
//22 F135 当前三象限无功电能示值（总、费率1～M）
//23 F136 当前四象限无功电能示值（总、费率1～M）
//28 F141 上月（上一结算日）一象限无功电能示值（总、费率1～M）
//29 F142 上月（上一结算日）二象限无功电能示值（总、费率1～M）
//30 F143 上月（上一结算日）三象限无功电能示值（总、费率1～M）
//31 F144 上月（上一结算日）四象限无功电能示值（总、费率1～M）
//扩展0xD5 上2月一象限无功电能示值（总、费率1～M）
//扩展0xD6 上2月二象限无功电能示值（总、费率1～M）
//扩展0xD7 上2月三象限无功电能示值（总、费率1～M）
//扩展0xD8 上2月四象限无功电能示值（总、费率1～M）
		case 133:
		case 134:
		case 135:
		case 136:
		case 141:
		case 142:
		case 143:
		case 144:
		case 0xd5:
		case 0xd6:
		case 0xd7:
		case 0xd8:
//F130DATA:
			x=p8rxbuff[9];
			if(x>=8)
			{
				x-=8;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				Pn=(x+1)*4;
				p8FnData+=7;
				p8rxbuff+=14;
				for(i=0;i<Pn;i++)
				{
					p8FnData[i]=p8rxbuff[i];
				}
				if(x==0)
				{//费率数=0改为1
					p8FnData-=7;
					p8FnData[6]=1;//费率数=1
					p8FnData+=7;
					MW((u32)p8FnData,((u32)p8FnData)+4,4);//费率1数据=总
				}
			}
			break;
//32 F145 当月正向有功最大需量及发生时间（总、费率1～M）
//33 F146 当月正向无功最大需量及发生时间（总、费率1～M）
//34 F147 当月反向有功最大需量及发生时间（总、费率1～M）
//35 F148 当月反向无功最大需量及发生时间（总、费率1～M）
//36 F149 上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
//37 F150 上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
//38 F151 上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
//39 F152 上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
//扩展0xD9 上2月正向有功最大需量及发生时间（总、费率1～M）
//扩展0xDA 上2月正向无功最大需量及发生时间（总、费率1～M）
//扩展0xDB 上2月反向有功最大需量及发生时间（总、费率1～M）
//扩展0xDC 上2月反向无功最大需量及发生时间（总、费率1～M）
		case 145:
		case 146:
		case 147:
		case 148:
		case 149:
		case 150:
		case 151:
		case 152:
		case 0xd9:
		case 0xda:
		case 0xdb:
		case 0xdc:
F145DATA:
			x=p8rxbuff[9];
			if(x>=4)
			{
				Change0xFFTo0xEE(((u32)p8rxbuff)+14,x-4);//搜索0xFF变为0xee
			}
			if(x>=12)
			{
				x-=12;
				x/=8;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				p8FnData+=7;
				p8rxbuff+=14;
				for(i=0;i<=x;i++)
				{
					for(Pn=0;Pn<7;Pn++)
					{
						p8FnData[Pn+(7*i)]=p8rxbuff[Pn];
					}
					p8rxbuff+=8;
				}
				if(x==0)
				{//费率数=0改为1
					p8FnData-=7;
					p8FnData[6]=1;//费率数=1
					p8FnData+=7;
					MW((u32)p8FnData,((u32)p8FnData)+7,7);//费率1数据=总
				}
			}
			break;
//48 F161 电能表远程控制通断电状态及记录
		case 161:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表通断电状态
					i=MRR(((u32)p8rxbuff)+14,1);
					i&=0x10;//B4=继电器状态(0通，1断)
					if(i==0)
					{
						p8FnData[6]=0x11;//0x11表示"通电"
					}
					else
					{
						p8FnData[6]=0;//0表示"断电"
					}
					break;
				case 1://上1-10次合闸操作者代码(本地自动跳合闸时，跳、合闸记录中的操作者代码为FFFFFFFF)
					for(i=0;i<10;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+(4*i),4);
						if(x!=0xffffffff)
						{
							break;
						}
					}
					if(i<10)
					{//有最近次合闸操作
						p8FnData[32]=i;
					}
					break;
				case 2://上1-10次合闸发生时刻
					i=p8FnData[32];
					if(i!=0xee)
					{//有最近次合闸操作
						MW(((u32)p8rxbuff)+14+(6*i),((u32)p8FnData)+6+1,6);
					}
					break;
				case 3://上1-10次跳闸操作者代码
					for(i=0;i<10;i++)
					{
						x=MRR(((u32)p8rxbuff)+14+(4*i),4);
						if(x!=0xffffffff)
						{
							break;
						}
					}
					if(i<10)
					{//有最近次合闸操作
						p8FnData[33]=i;
					}
					break;
				case 4://上1-10次跳闸发生时刻
					i=p8FnData[33];
					if(i!=0xee)
					{//有最近次合闸操作
						MW(((u32)p8rxbuff)+14+(6*i),((u32)p8FnData)+6+1+6,6);
					}
					break;
			}
			break;
//49 F162 电能表日历时钟
		case 162:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表日历时钟Hhmmss				
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1+6,3);
					//精确的抄表时间,年月日时分秒(星期)
					if(UARTCtrl->TransferDelay==0)
					{//线路上传输延时时间秒
						MW(ADDR_AFN0CF2,((u32)p8FnData)+1,6);//填入抄表时间
					}
					else
					{
						d64a=MRR(ADDR_TYMDHMS,6);
						d64a=YMDHMS_SubS(d64a,UARTCtrl->TransferDelay);//年月日时分秒减S秒,返回减S秒后的年月日时分秒
						//星期计算
						MWR(d64a,((u32)p8rxbuff)+14,3);
						MWR(d64a>>24,((u32)p8rxbuff)+14+4,3);
						YMDCalculateW(((u32)p8rxbuff)+14);//指定地址缓冲中的年月日周时分秒计算周
						i=p8rxbuff[14+3];
						if(i==0x0)
						{
							i=7;//星期日期=7
						}
						i<<=5;
						d64b=i;
						d64b<<=32;
						d64a|=d64b;
						MWR(d64a,((u32)p8FnData)+1,6);//填入抄表时间
					}
					break;
				case 1://电能表日历时钟YYMMDDWW
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+1+6+3,3);
					//星期
					i=p8rxbuff[14];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					p8FnData[1+6+3+1]|=i;
					break;
			}
			break;

//49 F165 电能表开关操作次数及时间
		case 165:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//本序号的电能表/交流采样装置删除
				break;
			}
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://编程总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://最近一次编程发生时刻
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3,6);
					break;
				case 2://电能表尾盖打开次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+6,3);
					MC(0,((u32)p8FnData)+64,1);//清尾盖打开次数变化标志
					if(Pn<=ERC38MaxPn)
					{
						i=MRR(ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR)+3,3);//原次数
						x=MRR(((u32)p8rxbuff)+14,3);//当前次数
						MWR(x,ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR)+3,3);//更新原次数
						if(i!=0xeeeeee)
						{
							if(i!=x)
							{//次数变化
								MC(0x55,((u32)p8FnData)+64,1);//尾盖打开次数变化标志
							}
						}
					}
					break;
				case 3://最近一次尾盖打开时间
					x=MRR(((u32)p8FnData)+64,1);//尾盖打开次数变化标志
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+6+3,60);
					if(x==0x55)
					{//打开次数变化标志
					#if ERC38==1
						ERC38_Event(Pn,((u32)p8FnData)+6+3+6);//电能表开端钮盒事件记录;入口:Pn测量点号(从1开始)
					#endif
					}
					break;
				case 4://电能表开表盖打开次数(仅用于事件记录)
					if(Pn<=ERC37MaxPn)
					{
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+6+3+6,3);
						MC(0,((u32)p8FnData)+64,1);//清开表盖打开次数变化标志
						i=MRR(ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR),3);//原次数
						x=MRR(((u32)p8rxbuff)+14,3);//当前次数
						MWR(x,ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR),3);//更新原次数
						if(i!=0xeeeeee)
						{
							if(i!=x)
							{//次数变化
								MC(0x55,((u32)p8FnData)+64,1);//开表盖打开次数变化标志
								break;
							}
						}
					}
					UARTCtrl->RMCount++;//跳过下步
					break;
				case 5://最近一次开表盖记录(仅用于事件记录)
					if(Pn<=ERC37MaxPn)
					{
						x=MRR(((u32)p8FnData)+64,1);//开表盖打开次数变化标志
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+6+3+6+3,60);
						if(x==0x55)
						{//打开次数变化标志
						#if ERC37==1
							ERC37_Event(Pn,((u32)p8FnData)+6+3+6+3+6);//电能表开表盖事件记录;入口:Pn测量点号(从1开始)
						#endif
						}
					}
					break;
			}
			break;
//50 F166 电能表参数修改次数及时间
		case 166:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://校时总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					break;
				case 1://电能表最近一次时钟修改前和后时间
					MW(((u32)p8rxbuff)+14+4,((u32)p8FnData)+6+3,12);
					break;
				case 2://电能表时段参数修改次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+12,3);
					break;
				case 3://最近一次电能表时段参数修改时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+3+12+3,6);
					break;
			}
			break;
//51 F167 电能表购、用电信息
		case 167:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://上1次购电后总购电费次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,2);
					break;
				case 1://（当前）剩余金额
					p8FnData[6+2]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+1,4);
					break;
				case 2://上1次购电后累计购电金额
					p8FnData[6+2+5]=0;
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+1,4);
					break;
				case 3://上1次购电后总购电量次数
					i=MRR(((u32)p8FnData)+6,2);
					if((i==0)||(i==0xeeee))
					{//若购电费次数=0或空
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,2);
					}
					break;
				case 4://（当前）剩余电量
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5,4);
					break;
				case 5://（当前）透支电量
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5+4,4);
					break;
				case 6://上1次购电后累计购电量
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5+4+4,4);
					break;
				case 7://赊欠门限电量(透支电量限值)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5+4+4+4,4);
					break;
				case 8://报警电量(报警电量1)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+2+5+5+4+4+4+4,4);
					break;
			}
			break;
//52 F168 电能表结算信息
		case 168:
			break;
			
#if ERC8==1		
//0xE1 扩展1 ERC8电能表参数变更事件用数据
		case 0xE1:
			if(p8FnData[ERC8_SOUR_Pn_LEN]==0xee)
			{
				MC(0,((u32)p8FnData),10);//清前10字节第1,2套第1-8日时段表抄标志和CRC值
				p8FnData[ERC8_SOUR_Pn_LEN]=0;
			}
			i=UARTCtrl->RMCount;
			if(i<4)//抄表命令计数
			{//第1,2套第1-2日时段表
				p8FnData[0]|=1<<i;
				x=MRR(((u32)p8FnData)+1,4);//读存贮器数据到寄存器,Byte<=8
				Pn=p8rxbuff[9];
				if(Pn>4)
				{
					Pn-=4;
					Pn/=3;
					for(i=0;i<Pn;i++)
					{
						if(i<8)
						{
							x+=MRR(((u32)p8rxbuff)+14+(3*i),3)<<i;//读存贮器数据到寄存器,Byte<=8
						}
						else
						{
							x+=(~MRR(((u32)p8rxbuff)+14+(3*i),3))<<(i-8);//读存贮器数据到寄存器,Byte<=8
						}
					}
				}
				MWR(x,((u32)p8FnData)+1,4);//写寄存器数据到存贮器,Byte<=8
				break;
			}
			switch(i)//抄表命令计数
			{
				case 4://每月第1结算日
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10,2);
					break;
				case 5://每月第2结算日
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+2,2);
					break;
				case 6://每月第3结算日
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+4,2);
					break;
				case 7://电表无功常数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+3,3);
					break;
				case 8://电流互感器变比
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+6,3);
					break;
				case 9://电压互感器变比
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+6+3,3);
					break;
				case 10://电表有功常数(互感器变比有些表可能没有，为正确跳过下2项，故读有功常数在此)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6,3);
					if(PORTn==ACSAMPLEPORT)
					{//内部交采口
						goto Fn0xE1_END;
					}
					if(PORTn!=RS485_4PORT)
					{//RS485口
	#if RMM_RS485==0//RS485通道抄表模式:0=循抄全部数据,1=按大小类号设置抄读数据
						goto Fn0xE1_END;
	#endif
					}
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
					i=Class1Data_Fn(i,27);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					if(i==0)
					{//Fn=27没抄表
						break;
					}
				Fn0xE1_END:
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					i=GetClass1DataAddr(27,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
					if(i==0x0)
					{
						break;
					}
					//取最近一次编程发生时刻
					MR(((u32)p8FnData)+10+6+6+6,i+18,6);
					//取需量清零总次数
					MR(((u32)p8FnData)+10+6+6+6+6,i+33,3);
					if(Pn==0x0)
					{//本序号的电能表/交流采样装置删除
						break;
					}
					ERC8_Event(Pn-1,(u32)p8FnData);//电能表参数变更,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
					UARTCtrl->RMCount+=2;//跳过下2项抄表
					break;
				case 11://最近一次编程发生时刻(当Fn=27不抄时)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+6+6,6);
					break;
				case 12://需量清零总次数(当Fn=27不抄时)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+6+6+6,3);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if(Pn==0x0)
					{//本序号的电能表/交流采样装置删除
						break;
					}
					ERC8_Event(Pn-1,(u32)p8FnData);//电能表参数变更,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
					break;
			}
			break;
#endif		
#if ERC13==1
//0xE2 扩展2 ERC13电表故障信息事件用数据
		case 0xE2:
			if(p8FnData[ERC13_SOUR_Pn_LEN]==0xee)
			{
				MC(0,((u32)p8FnData)+6,4);//清断相次数
				p8FnData[ERC13_SOUR_Pn_LEN]=0;
			}
			i=UARTCtrl->RMCount;
			if(i<3)//抄表命令计数
			{//断相次数
				p8FnData[6]|=1<<i;
				x=MRR(((u32)p8FnData)+7,3);//读存贮器数据到寄存器,Byte<=8
				x+=MRR(((u32)p8rxbuff)+14,3);//读存贮器数据到寄存器,Byte<=8
				MWR(x,((u32)p8FnData)+7,3);//写寄存器数据到存贮器,Byte<=8
				break;
			}
			switch(i)
			{
				case 3://失压总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10,3);
					break;
				case 4://掉电总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+3,3);
					if(PORTn==ACSAMPLEPORT)
					{//内部交采口
						goto Fn0xE2_END;
					}
					if(PORTn!=RS485_4PORT)
					{//RS485口
	#if RMM_RS485==0//RS485通道抄表模式:0=循抄全部数据,1=按大小类号设置抄读数据
						goto Fn0xE2_END;
	#endif
					}
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
					x=Class1Data_Fn(i,27);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					if(x==0)
					{//Fn=27没抄表
						break;
					}
					x=Class1Data_Fn(i,28);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					if(x==0)
					{//Fn=28没抄表
						Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
						if(Pn==0x0)
						{//本序号的电能表/交流采样装置删除
							break;
						}
						i=GetClass1DataAddr(27,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
						if(i!=0x0)
						{
							//电能表编程次数							
							MR(((u32)p8FnData),i+15,3);
							//最大需量清零次数
							MR(((u32)p8FnData)+3,i+33,3);	
						}
						UARTCtrl->RMCount+=2;//跳过2项抄表
						break;
					}
				Fn0xE2_END:
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if(Pn==0x0)
					{//本序号的电能表/交流采样装置删除
						break;
					}
					i=GetClass1DataAddr(27,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
					if(i!=0x0)
					{
						//电能表编程次数							
						MR(((u32)p8FnData),i+15,3);
						//最大需量清零次数
						MR(((u32)p8FnData)+3,i+33,3);	
					}
					i=GetClass1DataAddr(28,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
					if(i!=0x0)
					{
						//电表运行状态字1电池欠压
						p8=(u8 *)(i+19);
						i=p8[0];
					#if (USER/100)==5//上海;只要求时钟电池
						i&=0x04;//b3=停电抄表电池,b2=时钟电池
					#else
						i&=0x0c;//b3=停电抄表电池,b2=时钟电池
					#endif
						if(i!=0)
						{
							i=1;
						}
						p8FnData[16]=i;
					}
					ERC13_Event(Pn-1,(u32)p8FnData);//电表故障信息,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
					UARTCtrl->RMCount+=3;//跳过下3项抄表
					break;
				case 5://编程总次数(当Fn=27不抄时)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData),3);
					break;
				case 6://需量清零总次数(当Fn=27不抄时)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+3,3);
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
					x=Class1Data_Fn(i,28);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					if(x==0)
					{//Fn=28没抄表
						break;
					}
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if(Pn==0x0)
					{//本序号的电能表/交流采样装置删除
						break;
					}
					i=GetClass1DataAddr(28,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
					if(i!=0x0)
					{
						//电表运行状态字1电池欠压
						p8=(u8 *)(i+19);
						i=p8[0];
						i&=0x0c;//b3=停电抄表电池,b2=时钟电池
						if(i!=0)
						{
							i=1;
						}
						p8FnData[16]=i;
					}
					ERC13_Event(Pn-1,(u32)p8FnData);//电表故障信息,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
					UARTCtrl->RMCount+=1;//跳过下1项抄表
					break;
				case 7://电表运行状态字1(当Fn=28不抄时)
					i=p8rxbuff[14];
					i&=0x0c;//b3=停电抄表电池,b2=时钟电池
					if(i!=0)
					{
						i=1;
					}
					p8FnData[16]=i;
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if(Pn==0x0)
					{//本序号的电能表/交流采样装置删除
						break;
					}
					ERC13_Event(Pn-1,(u32)p8FnData);//电表故障信息,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
					break;
			}
			break;
#endif	
//扩展3 集中抄表日冻结电能示值（总、费率1～M）
		case 0xE3:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://日冻结时间YYMMDD
					//注:不比对时标时用于冻结的时标在Terminal_AFN0D_CongealData_Fn.c冻结时会确定
					x=MRR(((u32)p8rxbuff)+14+2,3);
			#if (USER/100)==5//上海
				#if (RMM_CompCongealTime!=0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					i=MRR(ADDR_TYMDHMS+3,3);
					if(i!=x)
					{//YMD不同
						UARTCtrl->RMCount=100;//跳不抄下面
						return;
					}
				#endif
			#endif
					MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
					return;
				case 9://再次确认日冻结时间YYMMDD(已修改为不抄)
				/*
				#if (RMM_CompCongealTime!=0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					x=MRR(((u32)p8rxbuff)+14+2,3);
					i=MRR(ADDR_TYMDHMS+3,3);
					if(i!=x)
					{//YMD不同
						p8FnData[0]=0xee;//数据有效标志
					}
				#endif
				*/
					return;
			}
			goto DateAddCom;
//扩展 RS485用读全部6类负荷曲线
//扩展 载波用读1类负荷曲线(电压、电流、频率)
//扩展 载波用读2类负荷曲线(有无功功率)
//扩展 载波用读3类负荷曲线(功率因数)
//扩展 载波用读4类负荷曲线(正反有功,組合无功1和2总电能)
//扩展 载波用读5类负荷曲线(1-4象限无功总电能)
//扩展 载波用读6类负荷曲线(当前有功需量,当前无功需量)
		case 0xE4://扩展 RS485用读全部6类负荷曲线
		case 0xE5://扩展 载波用读1类负荷曲线(电压、电流、频率)
		case 0xE6://扩展 载波用读2类负荷曲线(有无功功率)
		case 0xE7://扩展 载波用读3类负荷曲线(功率因数)
		case 0xE8://扩展 载波用读4类负荷曲线(正反有功,組合无功1和2总电能)
		case 0xE9://扩展 载波用读5类负荷曲线(1-4象限无功总电能)
		case 0xEA://扩展 载波用读6类负荷曲线(当前有功需量,当前无功需量)
			i=p8rxbuff[9];
			i+=1;
			x=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表Fn数据缓冲长度
			if(x>=(6+i))
			{
				MR(((u32)p8FnData)+6,((u32)p8rxbuff)+9,i);
			}
			p8FnData[0]=1;//数据有效标志
			switch(p8RMLib[0])
			{
				case 0xE6://扩展 载波用读2类负荷曲线(有无功功率)
					if(p8rxbuff[14+8]!=0xAA)
					{//前面无分隔符AA
						p8FnData[6]+=1;
						p8FnData[7+4+8]=0xAA;
						MR(((u32)p8FnData)+7+4+8+1,((u32)p8rxbuff)+14+8,25);
					}
					break;
				case 0xE7://扩展 载波用读3类负荷曲线(功率因数)
					if(p8rxbuff[14+8]!=0xAA)
					{//前面无分隔符AA,AA
						p8FnData[6]+=2;
						p8FnData[7+4+8]=0xAA;
						p8FnData[7+4+9]=0xAA;
						MR(((u32)p8FnData)+7+4+8+2,((u32)p8rxbuff)+14+8,9);
					}
					break;
				case 0xE8://扩展 载波用读4类负荷曲线(正反有功,組合无功1和2总电能)
					if(p8rxbuff[14+8]!=0xAA)
					{//前面无分隔符AA,AA,AA
						p8FnData[6]+=3;
						p8FnData[7+4+8]=0xAA;
						p8FnData[7+4+9]=0xAA;
						p8FnData[7+4+10]=0xAA;
						MR(((u32)p8FnData)+7+4+8+3,((u32)p8rxbuff)+14+8,17);
					#if (((USER/100)==5)&&((Project/100)==2))//上海集中器
		 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
						switch(i)
						{
							case 0x51:
							case 0x52:
						D51E8://大类号5小类号1，所有正向有功数据取电能表的组合有功（若无组合有功，取正向+反向数据）
								d64b=MRR(((u32)p8FnData)+7+4+8+3+4,4);
								for(y=0;y<8;y++)
								{
									x=(d64b>>(y*4));
									x&=0xf;
									if(x>9)
									{
										break;
									}
								}
								if(y!=8)
								{
									d64b=0;
								}
								else
								{
									d64b=bcd16_hex(d64b);
								}
								d64a=MRR(((u32)p8FnData)+7+4+8+3,4);
								for(y=0;y<8;y++)
								{
									x=(d64a>>(y*4));
									x&=0xf;
									if(x>9)
									{
										break;
									}
								}
								if(y!=8)
								{
									d64a=0;
								}
								else
								{
									d64a=bcd16_hex(d64a);
								}
								d64a+=d64b;
								d64a=hex_bcd16(d64a);
								MWR(d64a,((u32)p8FnData)+7+4+8+3,4);
								break;
							case 0x02:
							case 0x62:
							case 0x71:
							case 0x72:
								break;
							default://其他按大类号5小类号1处理
								goto D51E8;
						}
					#endif

					}
					break;
				case 0xE9://扩展 载波用读5类负荷曲线(1-4象限无功总电能)
					if(p8rxbuff[14+8]!=0xAA)
					{//前面无分隔符AA,AA,AA,AA
						p8FnData[6]+=4;
						p8FnData[7+4+8]=0xAA;
						p8FnData[7+4+9]=0xAA;
						p8FnData[7+4+10]=0xAA;
						p8FnData[7+4+11]=0xAA;
						MR(((u32)p8FnData)+7+4+8+4,((u32)p8rxbuff)+14+8,17);
					}
					break;
				case 0xEA://扩展 载波用读6类负荷曲线(当前有功需量,当前无功需量)
					if(p8rxbuff[14+8]!=0xAA)
					{//前面无分隔符AA,AA,AA,AA,AA
						p8FnData[6]+=5;
						p8FnData[7+4+8]=0xAA;
						p8FnData[7+4+9]=0xAA;
						p8FnData[7+4+10]=0xAA;
						p8FnData[7+4+11]=0xAA;
						p8FnData[7+4+12]=0xAA;
						MR(((u32)p8FnData)+7+4+8+5,((u32)p8rxbuff)+14+8,7);
					}
					break;
			}
			break;
//扩展11 集中抄表日冻结正向有功最大需量及发生时间（总、费率1～M）
//扩展12 集中抄表日冻结反向有功最大需量及发生时间（总、费率1～M）
		case 0xEB:
		case 0xEC:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://日冻结电能示值
		#if (RMM_CompCongealTime==0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					p8FnData[0]=1;//数据有效标志
					UARTCtrl->RMCount=1;//抄表命令计数+1
		#else
					p8FnData[0]=0;//有数据标志
		#endif
					goto F145DATA;
				case 1://（上1次）日冻结时间YYMMDDhhmm
					i=MRR(((u32)p8rxbuff)+14+2,3);
					x=MRR(ADDR_TYMDHMS+3,3);
					if(i==x)
					{//YMD同
						if(p8FnData[0]!=0xee)
						{
							p8FnData[0]=1;//数据有效标志
						}
					}
					break;
			}
			break;
//75 扩展13 上1-24整点电表冻结的正反有功电能示值
		case 0xED:
			//FnDataAddr+6=抄表计数(0-23即上1-24点),FnDataAddr+7开始12字节数据
			x=p8rxbuff[9];
			if(x>=14)
			{
				p8FnData[6]=UARTCtrl->RMCount;//抄表计数(0-23即上1-24点)
				MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+6+1,4);
				MW(((u32)p8rxbuff)+14+5+1,((u32)p8FnData)+6+1+4,4);
				if(x>=19)
				{//有反向
					MW(((u32)p8rxbuff)+14+5+1+4+1,((u32)p8FnData)+6+1+4+4,4);
				}
			}
			break;
//76 扩展14 补抄上2日日冻结数据
		case 0xEE:
	#if(LEN_UART2FnDataBuff<(6+LEN_LastDate_Source_PerPn))
		#error "RS485-1 LEN_UART2FnDataBuff<(6+LEN_LastDate_Source_PerPn)"
	#endif
	#if(LEN_UART3FnDataBuff<(6+LEN_LastDate_Source_PerPn))
		#error "RS485-2 LEN_UART3FnDataBuff<(6+LEN_LastDate_Source_PerPn)"
	#endif
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#if(LEN_UART4FnDataBuff<(6+LEN_LastDate_Source_PerPn))
		#error "载波 LEN_UART4FnDataBuff<(6+LEN_LastDate_Source_PerPn)"
	#endif
#endif
#if (Project/100)==5//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	#if(LEN_UART5FnDataBuff<(6+LEN_LastDate_Source_PerPn))
		#error "RS485-3 LEN_UART5FnDataBuff<(6+LEN_LastDate_Source_PerPn)"
	#endif
#endif
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://日冻结时间YYMMDD
					//注:不比对时标时用于冻结的时标在Terminal_AFN0D_CongealData_Fn.c冻结时会确定
					x=MRR(((u32)p8rxbuff)+14+2,3);
			#if (USER/100)==5//上海
				#if (RMM_CompCongealTime!=0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					i=MRR(ADDR_TYMDHMS+3,3);
					i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
					if(i!=x)
					{//YMD不同
						UARTCtrl->RMCount=100;//跳,不抄下面
						UARTCtrl->RMError|=2;//B0=失败,B1=F129失败;上海要求当日冻结时标错误时AFN0CF11集中抄表信息不能计抄表成功
						return;
					}
				#endif
			#endif
					MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
					return;
				case 9://再次确认日冻结时间YYMMDD(已修改为不抄)
				/*
				#if (RMM_CompCongealTime!=0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					x=MRR(((u32)p8rxbuff)+14+2,3);
					i=MRR(ADDR_TYMDHMS+3,3);
					i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
					if(i!=x)
					{//YMD不同
						p8FnData[0]=0xee;//数据有效标志
					}
				#endif
				*/
					return;
			}
	DateAddCom:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 1://F161 正向有功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF161_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=x;
						p8FnData+=6+Office_AFN0DF161_LastDate_Source+6;
						p8rxbuff+=14;
						for(i=0;i<=x;i++)
						{
							p8FnData[0+(5*i)]=0;
							p8FnData[1+(5*i)]=p8rxbuff[0];
							p8FnData[2+(5*i)]=p8rxbuff[1];
							p8FnData[3+(5*i)]=p8rxbuff[2];
							p8FnData[4+(5*i)]=p8rxbuff[3];
							p8rxbuff+=4;
						}
					#if (USER/100)==4//用户标识:陕西
						//农网表只有总电量,费率数填4
						if(x==0)
						{
							x=TMaxTariff;
							p8FnData-=6+Office_AFN0DF161_LastDate_Source+6;
							p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=TMaxTariff;//费率数=4
						}
					#endif
						if(x==0)
						{//费率数=0改为1
							p8FnData-=6+Office_AFN0DF161_LastDate_Source+6;
							p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=1;//费率数=1
							p8FnData+=6+Office_AFN0DF161_LastDate_Source+6;
							MW((u32)p8FnData,((u32)p8FnData)+5,5);//费率1数据=总
						}
					}
					break;
				case 2://F162 正向无功（组合无功1）电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF162_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source+6,4*(x+1));
					}
					break;
				case 3://F163 反向有功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF163_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF163_LastDate_Source+5]=x;
						p8FnData+=6+Office_AFN0DF163_LastDate_Source+6;
						p8rxbuff+=14;
						for(i=0;i<=x;i++)
						{
							p8FnData[0+(5*i)]=0;
							p8FnData[1+(5*i)]=p8rxbuff[0];
							p8FnData[2+(5*i)]=p8rxbuff[1];
							p8FnData[3+(5*i)]=p8rxbuff[2];
							p8FnData[4+(5*i)]=p8rxbuff[3];
							p8rxbuff+=4;
						}
						if(x==0)
						{//费率数=0改为1
							p8FnData-=6+Office_AFN0DF163_LastDate_Source+6;
							p8FnData[6+Office_AFN0DF163_LastDate_Source+5]=1;//费率数=1
							p8FnData+=6+Office_AFN0DF163_LastDate_Source+6;
							MW((u32)p8FnData,((u32)p8FnData)+5,5);//费率1数据=总
						}
					}
				#if (((USER/100)==5)&&((Project/100)==2))//上海集中器
	 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
					switch(i)
					{
						case 0x51:
						case 0x52:
					D51F161://大类号5小类号1，所有正向有功数据取电能表的组合有功（若无组合有功，取正向+反向数据）
							p8FnData=(u8 *)Addr_FnDataBuff;
							for(i=0;i<=x;i++)
							{
								d64b=MRR(((u32)p8FnData)+(6+Office_AFN0DF161_LastDate_Source+6)+(5*i),5);
								for(y=0;y<10;y++)
								{
									j=(d64b>>(y*4));
									j&=0xf;
									if(j>9)
									{
										break;
									}
								}
								if(y!=10)
								{
									d64b=0;
								}
								else
								{
									d64b=bcd16_hex(d64b);
								}
								d64a=MRR(((u32)p8FnData)+(6+Office_AFN0DF163_LastDate_Source+6)+(5*i),5);
								for(y=0;y<10;y++)
								{
									j=(d64a>>(y*4));
									j&=0xf;
									if(j>9)
									{
										break;
									}
								}
								if(y!=10)
								{
									d64a=0;
								}
								else
								{
									d64a=bcd16_hex(d64a);
								}
								d64a+=d64b;
								d64a=hex_bcd16(d64a);
								MWR(d64a,((u32)p8FnData)+(6+Office_AFN0DF161_LastDate_Source+6)+(5*i),5);
							}
							break;
						case 0x02:
						case 0x62:
						case 0x71:
						case 0x72:
							break;
						default://其他按大类号5小类号1处理
							goto D51F161;
					}
				#endif
					break;
				case 4://F164 反向无功（组合无功2）电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF164_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source+6,4*(x+1));
					}
					break;
				case 5://F165 1象限无功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF165_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source+6,4*(x+1));
					}
					break;
				case 6://F166 2象限无功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF166_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source+6,4*(x+1));
					}
					break;
				case 7://F167 3象限无功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF167_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source+6,4*(x+1));
					}
					break;
				case 8://F168 4象限无功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=8)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=8;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF168_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source+6,4*(x+1));
					}
					break;
			#if (((USER/100)==5)&&(Project==300))//上海专变
				case 10://F185 正向有功最大需量及发生时间
					x=p8rxbuff[9];
					if(x>=4)
					{
						Change0xFFTo0xEE(((u32)p8rxbuff)+14,x-4);//搜索0xFF变为0xee
					}
					if(x>=(4+8))
					{
						p8FnData[0]=1;//有数据填入标志
						x-=(4+8);
						x/=8;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF185_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF185_LastDate_Source+5]=x;//费率数
						p8FnData+=6+Office_AFN0DF185_LastDate_Source+6;
						p8rxbuff+=14;
						for(i=0;i<=x;i++)
						{
							for(Pn=0;Pn<7;Pn++)
							{
								p8FnData[Pn+(7*i)]=p8rxbuff[Pn];
							}
							p8rxbuff+=8;
						}
						if(x==0)
						{//费率数=0改为1
							p8FnData-=7;
							p8FnData[6]=1;//费率数=1
							p8FnData+=7;
							MW((u32)p8FnData,((u32)p8FnData)+7,7);//费率1数据=总
						}
					}
					break;
				case 11://F187 反向有功最大需量及发生时间
					x=p8rxbuff[9];
					if(x>=4)
					{
						Change0xFFTo0xEE(((u32)p8rxbuff)+14,x-4);//搜索0xFF变为0xee
					}
					if(x>=(4+8))
					{
						p8FnData[0]=1;//有数据填入标志
						x-=(4+8);
						x/=8;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF187_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF187_LastDate_Source+5]=x;//费率数
						p8FnData+=6+Office_AFN0DF187_LastDate_Source+6;
						p8rxbuff+=14;
						for(i=0;i<=x;i++)
						{
							for(Pn=0;Pn<7;Pn++)
							{
								p8FnData[Pn+(7*i)]=p8rxbuff[Pn];
							}
							p8rxbuff+=8;
						}
						if(x==0)
						{//费率数=0改为1
							p8FnData-=7;
							p8FnData[6]=1;//费率数=1
							p8FnData+=7;
							MW((u32)p8FnData,((u32)p8FnData)+7,7);//费率1数据=总
						}
					}
					break;
			#endif
			}
			break;
//77 扩展15 补抄上3日日冻结数据
		case 0xEF:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://日冻结时间YYMMDD
					//注:不比对时标时用于冻结的时标在Terminal_AFN0D_CongealData_Fn.c冻结时会确定
					x=MRR(((u32)p8rxbuff)+14+2,3);
			#if (USER/100)==5//上海
				#if (RMM_CompCongealTime!=0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					i=MRR(ADDR_TYMDHMS+3,3);
					i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
					i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
					if(i!=x)
					{//YMD不同
						UARTCtrl->RMCount=100;//跳,不抄下面
						return;
					}
				#endif
			#endif
					MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
					return;
				case 9://再次确认日冻结时间YYMMDDhhmm(已修改为不抄)
				/*
				#if (RMM_CompCongealTime!=0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					x=MRR(((u32)p8rxbuff)+14+2,3);
					i=MRR(ADDR_TYMDHMS+3,3);
					i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
					i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
					if(i!=x)
					{//YMD不同
						p8FnData[0]=0xee;//数据有效标志
					}
				#endif
				*/
					return;
			}
			goto DateAddCom;
			
#if ERC40==1
//扩展16 恒定磁场干扰总次数
		case 0xF0:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>ERC40MaxPn))
			{//Pn错
				UARTCtrl->RMCount++;//跳过下1项抄表
				break;
			}
			Pn--;
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://
					i=MRR(((u32)p8rxbuff)+14,3);//当前总次数
					x=MRR(ADDR_ERC40_SOUR+1+(LEN_ERC40_SOUR*Pn)+2,3);
					if(i!=0xeeeeee)
					{
						MWR(i,ADDR_ERC40_SOUR+1+(LEN_ERC40_SOUR*Pn)+2,3);
					}
					if((i!=0xeeeeee)&&(x!=0xeeeeee))
					{
						if(i!=x)
						{
							p8FnData[0]=1;//数据有效标志
							break;//抄记录
						}
					}
					UARTCtrl->RMCount++;//跳过下1项抄表
					break;
				case 1://
					if(p8FnData[0]!=1)//数据有效标志
					{
						break;
					}
					MW(((u32)p8rxbuff)+1,((u32)p8FnData)+6,6);//6字节设备地址
					MW(ADDR_ERC40_SOUR+1+(LEN_ERC40_SOUR*Pn)+2,((u32)p8FnData)+12,3);//总次数
					MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+15,5);//发生时间YMDHM
					ERC40_Meter_Event((u32)p8FnData,Pn+1);//ERC40直接读电能表磁场异常事件记录;入口:Fn数据首地址,测量点号(1开始)
					break;
			}
			break;
#endif
	#if (USER/100)==2//用户标识:山东版
		case 0xF2://扩展 ERC9电流回路异常事件用数据
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://A相断流总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData),3);
					break;
				case 1://B相断流总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+3,3);
					break;
				case 2://C相断流总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0x0)||(Pn>ERC9MaxPn))
					{//Pn错
						UARTCtrl->RMCount+=3;//跳过下3项抄表
						break;
					}
					x=0;//b0-b2分别表示A,B,C相次数变化标志
					for(i=0;i<3;i++)
					{
						d64a=MRR(((u32)p8FnData)+(i*3),3);
						d64b=MRR(ADDR_ERC9_SOUR+((Pn-1)*LEN_ERC9_SOUR_Pn)+1+(i*3),3);
						if(d64a!=0xeeeeee)
						{
							if(d64b==0xeeeeee)
							{
								MWR(d64a,ADDR_ERC9_SOUR+((Pn-1)*LEN_ERC9_SOUR_Pn)+1+(i*3),3);
							}
							else
							{
								if(d64a!=d64b)
								{
									x|=1<<i;
								}
							}
						}
					}
					if(x==0)
					{//没变化,不进行事件记录
						UARTCtrl->RMCount+=3;//跳过下3项抄表
					}
					x|=1<<6;//D6～D7编码表示电流回路的异常类型，取值1～3依次表示短路、开路、反向
					p8FnData[9]=x;
					p8FnData[0]=1;//数据有要效标志
					break;
				case 3://A,B,C相电压(当事件产生时实时读)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10,6);
					break;
				case 4://A,B,C相电流(当事件产生时实时读)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6,9);
					break;
				case 5://(当前)正向有功总电能(当事件产生时实时读)
					if(p8FnData[0]!=1)//数据有要效标志
					{
						break;
					}
					p8FnData[10+6+9]=0;//5字节电量
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+10+6+9+1,4);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0x0)||(Pn>ERC9MaxPn))
					{//Pn错
						break;
					}
					//更新成功读取的次数
					for(i=0;i<3;i++)
					{
						d64a=MRR(((u32)p8FnData)+(i*3),3);
						if(d64a!=0xeeeeee)
						{
							MWR(d64a,ADDR_ERC9_SOUR+((Pn-1)*LEN_ERC9_SOUR_Pn)+1+(i*3),3);
						}
					}
					p8FnData[0]=9;//ERC
					p8FnData[1]=28;//长度Le
					MR(((u32)p8FnData)+2,ADDR_TYMDHMS+1,5);//分时日月年
					p8FnData[7]=Pn;
					p8FnData[8]=(Pn>>8)|(1<<7);
					ERC_Event_Save(((u32)p8FnData));//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
					break;
			}
			break;
		#endif
		#if (((USER/100)==2)||((USER/100)==5))//用户标识:山东,上海
		case 0xF3://扩展 ERC10电压回路异常事件用数据
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>ERC10MaxPn))
			{//Pn错
				break;
			}
			Pn--;
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://全失压总次数,累计时间
					MW(((u32)p8rxbuff)+14,((u32)p8FnData),3);
				#if (USER/100)==5//用户标识:上海
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+401,6);
				#endif
					break;
				case 1://A相失压次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+3,3);
				#if (USER/100)==5//用户标识:上海
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+8,3);
				#endif
					break;
				case 2://B相失压次数	见附录A.10	次	3
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6,3);
				#if (USER/100)==5//用户标识:上海
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+11,3);
				#endif
					break;
				case 3://C相失压次数	见附录A.10	次	3
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+9,3);
				#if (USER/100)==5//用户标识:上海
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+14,3);
				#endif
					UARTCtrl->RMCount++;//能抄到新命令不抄老命令
					break;
				case 4://A,B,C失压总次数累计时间(老命令当上命令抄不到时抄)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+3,3);
					MW(((u32)p8rxbuff)+14+6,((u32)p8FnData)+6,3);
					MW(((u32)p8rxbuff)+14+12,((u32)p8FnData)+9,3);
				#if (USER/100)==5//用户标识:上海
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+8,3);
					MW(((u32)p8rxbuff)+14+6,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+11,3);
					MW(((u32)p8rxbuff)+14+12,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+14,3);
				#endif
					break;
				case 5://A断相次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+12,3);
					break;
				case 6://B断相次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+15,3);
					break;
				case 7://c断相次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+18,3);
					UARTCtrl->RMCount++;//能抄到新命令不抄老命令
					goto Fn0xF3_NEXT;
				case 8://A,B,C断相总次数累计时间(老命令当上命令抄不到时抄)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+12,3);
					MW(((u32)p8rxbuff)+14+6,((u32)p8FnData)+15,3);
					MW(((u32)p8rxbuff)+14+12,((u32)p8FnData)+18,3);
				Fn0xF3_NEXT:
					x=0;//b0-b2分别表示A,B,C相次数变化标志
					for(i=0;i<7;i++)
					{
						d64a=MRR(((u32)p8FnData)+(i*3),3);
						d64b=MRR(ADDR_ERC10_SOUR+(Pn*LEN_ERC10_SOUR_Pn)+2+(i*3),3);
						if(d64a!=0xeeeeee)
						{
							#if (USER/100)==5//用户标识:上海  只要测量点1的ERC10
								if(Pn==0)
								{
									MWR(d64a,ADDR_ERC10_SOUR+(Pn*LEN_ERC10_SOUR_Pn)+2+(i*3),3);
								}
							#else
								MWR(d64a,ADDR_ERC10_SOUR+(Pn*LEN_ERC10_SOUR_Pn)+2+(i*3),3);
							#endif
						}
						if((d64a!=0xeeeeee)&&(d64b!=0xeeeeee))
						{
							if(d64a!=d64b)
							{
								if(i==0)
								{//全失压
									x|=0x07;
									if((x&0xf0)==0)
									{
										x|=2<<6;//D6～D7编码表示电压回路的异常类型，取值1～2依次表示断相、失压，值0、3为备用
									}
								}
								else
								{
									if(i<4)
									{//失压
										x|=1<<(i-1);
										if((x&0xf0)==0)
										{
											x|=2<<6;//D6～D7编码表示电压回路的异常类型，取值1～2依次表示断相、失压，值0、3为备用
										}
									}
									else
									{//断相
										x|=1<<(i-4);
										if((x&0xf0)==0)
										{
											x|=1<<6;//D6～D7编码表示电压回路的异常类型，取值1～2依次表示断相、失压，值0、3为备用
										}
									}
								}
							}
						}
					}
					if(x==0)
					{//没变化,不进行事件记录
						UARTCtrl->RMCount+=3;//跳过下3项抄表
					}
					p8FnData[15]=x;
					p8FnData[0]=1;//数据有要效标志
					break;
				case 9://A,B,C相电压(当事件产生时实时读)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+16,6);
					break;
				case 10://A,B,C相电流(当事件产生时实时读)
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+16+6,9);
					break;
				case 11://(当前)正向有功总电能(当事件产生时实时读)
					if(p8FnData[0]!=1)//数据有要效标志
					{
						break;
					}
					p8FnData[16+6+9]=0;//5字节电量
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+16+6+9+1,4);
					p8FnData[6]=10;//ERC
					p8FnData[7]=28;//长度Le
					MR(((u32)p8FnData)+8,ADDR_TYMDHMS+1,5);//分时日月年
					Pn++;
					p8FnData[13]=Pn;
					p8FnData[14]=(Pn>>8)|(1<<7);
					ERC_Event_Save(((u32)p8FnData)+6);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
					break;
			}
			break;
	#endif
	#if (USER/100)==2//用户标识:山东版
		case 0xF4://扩展 ERC29电能表飞走事件,ERC28电能表超差事件用数据
			i=MRR(((u32)p8rxbuff)+14,3);
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			DayAveragePower(Pn,i,1);//日平均正有功功率计算;入口Pn从1开始,Power=有功功率(BCD),Count=本次计算次数
			break;
	#endif
		case 0xF6://扩展 F246当前掉电记录数据
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			Pn=F246MapPn(Pn);//F246掉电记录数据映射Pn;返回0表示无效
			if(Pn==0)
			{
				break;
			}
			Pn--;
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总次数
					MW(((u32)p8rxbuff)+14,((u32)p8FnData)+1,3);
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF246_Sec+(15*Pn),3);
					break;
				default://上2-10次
					i=UARTCtrl->RMCount;
					if((i>=1)&&(i<=10))
					{
						i--;
						for(x=0;x<12;x++)
						{//0xff转为0xee
							if(p8rxbuff[14+x]==0xff)
							{
								p8rxbuff[14+x]=0xee;
							}
						}
						MW(((u32)p8rxbuff)+14+1,((u32)p8FnData)+4+(10*i),5);
						MW(((u32)p8rxbuff)+14+7,((u32)p8FnData)+4+(10*i)+5,5);
						if(i==0)
						{//是第1次
							MW(((u32)p8rxbuff)+14,ADDR_AFN0CF246_Sec+3+(15*Pn),12);
						}
					}
					break;
			}
			break;
	#if (USER/100)==5//用户标识:上海
		case 0xF7://ERC68电能表负荷开关误动作事件记录数据
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
			{//Pn错
				break;
			}
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总次数
					i=MRR(ADDR_ERC68_SOUR+(3*(Pn-1)),3);//原次数
					x=MRR(((u32)p8rxbuff)+14,3);//当前次数
					if(x!=0xeeeeee)
					{
						MWR(x,ADDR_ERC68_SOUR+(3*(Pn-1)),3);//更新原次数
					}
					if((i!=0xeeeeee)&&(x!=0xeeeeee))
					{
						if(i!=x)
						{//次数变化
							p8FnData[0]=1;//有效比较总次数标志
							break;
						}
					}
					UARTCtrl->RMCount++;//抄表命令计数
					break;
				case 1://(上1次)负荷开关误动作记录
					if(p8FnData[0]!=1)//有效比较总次数标志
					{
						break;
					}
					p8FnData[0]=68;//ERC=68	BIN	1
					p8FnData[1]=39;//长度Le	BIN	1
					MR(((u32)p8FnData)+2,ADDR_TYMDHMS+1,5);//分时日月年//发生时间：分时日月年	见附录A.15	5
					p8FnData[7]=Pn;//D15～D12：备用	D11～D0：pn（测量点号1～2048）	BIN	2
					p8FnData[8]=Pn>>8;
					MR(((u32)p8FnData)+9,ADDR_ERC68_SOUR+(3*(Pn-1)),3);//负荷开关误动作总次数	见附录A.10	3
					//发生时刻	见附录A.1	6
					//结束时刻	见附录A.1	6
					//负荷开关误动作后状态	BIN	1
					//负荷开关误动作发生时刻正向有功总电能	见附录A.11	4
					//负荷开关误动作发生时刻反向有功总电能	见附录A.11	4
					//负荷开关误动作结束时刻正向有功总电能	见附录A.11	4
					//负荷开关误动作结束时刻反向有功总电能	见附录A.11	4
					MR(((u32)p8FnData)+12,((u32)p8rxbuff)+14,29);
					ERC_Event_Save((u32)p8FnData);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
					break;
			}
			break;
	
		case 0xF8://显示用失压全失压数据
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
			{//Pn错
				break;
			}
			Pn--;
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总失压次数	见附录A.10	次	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+5,3);
					break;
				case 1://失压时间累计值	见附录A.10	min	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+17,3);
					break;
				case 2://A相失压时间累计值	见附录A.10	min	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+20,3);
					break;
				case 3://B相失压时间累计值	见附录A.10	min	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+23,3);
					break;
				case 4://C相失压时间累计值	见附录A.10	min	3
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+26,3);
					break;
				case 5://最近1次失压起始时刻	秒分时日月年	6
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+29,6);
					break;
				case 6://最近1次失压结束时刻	秒分时日月年	6
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+35,6);
					break;
				case 7://A相最近1-10次失压起始时刻	秒分时日月年	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+41,60);
					break;
				case 8://A相最近1-10次失压结束时刻	秒分时日月年	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+101,60);
					break;
				case 9://B相最近1-10次失压起始时刻	秒分时日月年	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+161,60);
					break;
				case 10://B相最近1-10次失压结束时刻	秒分时日月年	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+221,60);
					break;
				case 11://C相最近1-10次失压起始时刻	秒分时日月年	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+281,60);
					break;
				case 12://C相最近1-10次失压结束时刻	秒分时日月年	6*10
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+341,60);
					break;
				case 13://（上1次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+407,15);
					break;
				case 14://（上2次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+422,15);
					break;
				case 15://（上3次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+437,15);
					break;
				case 16://（上4次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+452,15);
					break;
				case 17://（上5次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+467,15);
					break;
				case 18://（上6次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+482,15);
					break;
				case 19://（上7次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+497,15);
					break;
				case 20://（上8次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+512,15);
					break;
				case 21://（上9次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+527,15);
					break;
				case 22://（上10次）全失压发生时刻，电流值，结束时刻
					MW(((u32)p8rxbuff)+14,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+542,15);
					break;
				case 23://电压不平衡总次数，总累计时间(ERC17用户)					
					i=MRR(ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+557,3);//原次数
					x=MRR(((u32)p8rxbuff)+14,3);//当前次数
					if(x!=0xeeeeee)
					{
						MWR(x,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+557,3);
					}
					if((i!=0xeeeeee)&&(x!=0xeeeeee))
					{
						if(i!=x)
						{
							p8FnData[0]=1;//有效比较总次数标志
							break;//抄事件记录
						}
					}
					UARTCtrl->RMCount+=3;//不抄下命令
					break;
				case 24://电压最大不平衡率(ERC17用)
					if(p8FnData[0]!=1)//数据有效
					{
						break;
					}
					x=MRR(((u32)p8rxbuff)+14,3);//最大不平衡率
					x>>=4;
					p8FnData=(u8 *)(ADDR_DATABUFF);
					p8FnData[0]=17;//ERC
					p8FnData[1]=27;//长度Le
					MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
					Pn++;
					p8FnData[7]=Pn;
					p8FnData[8]=(Pn>>8)|(1<<7);
					p8FnData[9]=0x1;
					p8FnData[10]=x;
					p8FnData[11]=x>>8;
					p8FnData[12]=0xee;
					p8FnData[13]=0xee;
					MC(0xee,ADDR_DATABUFF+14,15);
					ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
					UARTCtrl->RMCount+=2;//不抄下命令
					break;
				case 25://(老命令)电压不平衡总次数，总累计时间(ERC17用)
					i=MRR(ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+557,3);//原次数
					x=MRR(((u32)p8rxbuff)+14,3);//当前次数
					if(x!=0xeeeeee)
					{
						MWR(x,ADDR_AFN0CF26_Ext+(LEN_AFN0CF26_Ext*Pn)+557,3);
					}
					if((i!=0xeeeeee)&&(x!=0xeeeeee))
					{
						if(i!=x)
						{
							p8FnData[0]=1;//有效比较总次数标志
							break;//抄事件记录
						}
					}
					UARTCtrl->RMCount++;//不抄下命令
					break;
				case 26://电压最大不平衡率(ERC17用)
					if(p8FnData[0]!=1)//有效比较总次数标志
					{
						break;
					}
					x=MRR(((u32)p8rxbuff)+14+12,2);//最大不平衡率
					x>>=4;
					p8FnData=(u8 *)(ADDR_DATABUFF);
					p8FnData[0]=17;//ERC
					p8FnData[1]=27;//长度Le
					MR(ADDR_DATABUFF+2,((u32)p8rxbuff)+14+1,5);//分时日月年
					Pn++;
					p8FnData[7]=Pn;
					p8FnData[8]=(Pn>>8)|(1<<7);
					p8FnData[9]=0x1;
					p8FnData[10]=x;
					p8FnData[11]=x>>8;
					p8FnData[12]=0xee;
					p8FnData[13]=0xee;
					MC(0xee,ADDR_DATABUFF+14,15);
					ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
					break;
			}
			break;
	#endif
	#if (USER/100)==11//吉林系
		case 0xF9://电表读取的日电压合格率统计数据
		case 0xFA://电表读取的月电压合格率统计数据
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0)
			{
				break;
			}
			Pn--;
			if(p8RMLib[0]==0xf9)
			{
				x=ADDR_AFN0DF27_SOUR_ReadMeter+((66+27)*Pn);
			}
			else
			{
				x=((u32)p8FnData)+6+1;
				if(UARTCtrl->RMCount==0)
				{
					MC(0,x,93);
				}
			}
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://A相电压合格率统计数据
				//相电压监测时间
					
				//相电压合格率 
					y=MRR(((u32)p8rxbuff)+14+3,2);
					y<<=8;
					MWR(y,x+66+6,3);
				//相电压超限率
				
				//相电压超上限时间
					y=MRR(((u32)p8rxbuff)+14+7,3);
					y=bcd_hex(y);
					MWR(y,x+4,2);
				//相电压超下限时间
					y=MRR(((u32)p8rxbuff)+14+10,3);
					y=bcd_hex(y);
					MWR(y,x+6,2);
				//相最高电压
				//相最高电压出现时间
					MW(((u32)p8rxbuff)+14+13,x+30,5);
				//相最低电压
				//相最低电压出现时间
					MW(((u32)p8rxbuff)+14+19,x+35,5);

				//相电压合格时间 ：相电压监测时间- 相电压超上限时间-相电压超下限时间
					y=MRR(((u32)p8rxbuff)+14,3);
					y=bcd_hex(y);
					i=MRR(((u32)p8rxbuff)+14+7,3);
					i=bcd_hex(i);
					y-=i;
					i=MRR(((u32)p8rxbuff)+14+10,3);
					i=bcd_hex(i);
					y-=i;
					MWR(y,x+8,2);
				//相越上限率
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+7,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
					    MWR(d64b,x+66,3);		
					}
				//相越下限率
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+3,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+10,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+3,3);					
					}
					break;
				case 1://B相电压合格率统计数据
				//相电压监测时间
					
				//相电压合格率 
					y=MRR(((u32)p8rxbuff)+14+3,2);
					y<<=8;
					MWR(y,x+66+9+6,3);
				//相电压超限率
				
				//相电压超上限时间
					y=MRR(((u32)p8rxbuff)+14+7,3);
					y=bcd_hex(y);
					MWR(y,x+14,2);
				//相电压超下限时间
					y=MRR(((u32)p8rxbuff)+14+10,3);
					y=bcd_hex(y);
					MWR(y,x+16,2);
				//相最高电压
				//相最高电压出现时间
					MW(((u32)p8rxbuff)+14+13,x+40,5);
				//相最低电压
				//相最低电压出现时间
					MW(((u32)p8rxbuff)+14+19,x+45,5);

				//相电压合格时间 ：相电压监测时间- 相电压超上限时间-相电压超下限时间
					y=MRR(((u32)p8rxbuff)+14,3);
					y=bcd_hex(y);
					i=MRR(((u32)p8rxbuff)+14+7,3);
					i=bcd_hex(i);
					y-=i;
					i=MRR(((u32)p8rxbuff)+14+10,3);
					i=bcd_hex(i);
					y-=i;
					MWR(y,x+18,2);
				//相越上限率
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+9,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+7,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+9,3);				
					}
				//相越下限率
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+12,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+10,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+12,3);					
					}
					break;
				case 2://C相电压合格率统计数据
				//相电压监测时间
					
				//相电压合格率 
					y=MRR(((u32)p8rxbuff)+14+3,2);
					y<<=8;
					MWR(y,x+66+18+6,3);
				//相电压超限率
				
				//相电压超上限时间
					y=MRR(((u32)p8rxbuff)+14+7,3);
					y=bcd_hex(y);
					MWR(y,x+24,2);
				//相电压超下限时间
					y=MRR(((u32)p8rxbuff)+14+10,3);
					y=bcd_hex(y);
					MWR(y,x+26,2);
				//相最高电压
				//相最高电压出现时间
					MW(((u32)p8rxbuff)+14+13,x+50,5);
				//相最低电压
				//相最低电压出现时间
					MW(((u32)p8rxbuff)+14+19,x+55,5);
				//相电压合格时间 ：相电压监测时间- 相电压超上限时间-相电压超下限时间
					y=MRR(((u32)p8rxbuff)+14,3);
					y=bcd_hex(y);
					i=MRR(((u32)p8rxbuff)+14+7,3);
					i=bcd_hex(i);
					y-=i;
					i=MRR(((u32)p8rxbuff)+14+10,3);
					i=bcd_hex(i);
					y-=i;
					MWR(y,x+28,2);
				//相越上限率
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+18,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+7,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+18,3);
					}
				//相越下限率
					y=MRR(((u32)p8rxbuff)+14,3);
					if(y==0)
					{
					    MWR(0,x+66+18,3);		
					}
					else
					{
						y=bcd_hex(y);
						i=MRR(((u32)p8rxbuff)+14+10,3);
						i=bcd_hex(i);
					    d64b=i*100000;
					    d64a=y;
					    d64b/=d64a;
					    d64b=hex_bcd(d64b);
						MWR(d64b,x+66+18+3,3);
					}
					break;
			}
			break;
		case 0xFB://A相电压曲线
			x=p8rxbuff[9];
			if((x==(4+8))||(x==(4+24)))
			{
				i=MRR(ADDR_TYMDHMS+3,3);//取当前时间年月日
				i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
				i<<=8;
				i=YMDH_AddH(i,UARTCtrl->RMCount);//年月日时加h时,返回加h时后的年月日时
				MWR(i,((u32)p8FnData)+6+1,4);
				MW(((u32)p8rxbuff)+14,((u32)p8FnData)+6+1+4,8);
			}
			break;
	#endif
	}
}













