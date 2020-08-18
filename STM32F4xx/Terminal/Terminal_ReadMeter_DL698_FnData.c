
//终端抄表698.45表规约接收数据填入Fn数据缓冲
#include "Project.h"
#include "Terminal_ReadMeter_DL698_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "../Terminal/DataConversion_SH698.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "../terminal/Terminal_AFN0E_Event.h"

//#include <stdarg.h>


#if (USER/100)==5//上海
extern const u32 DL698_RMLib[];


void WriteReportWord(u32 PORTn,u32 rxbuff)//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
{
#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
	u32 y;
	u32 MeterNo;
	u8 *p8s;
	u64 CommAddr;

	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1;
			break;
		case RS485_1PORT://RS485-1
			PORTn=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			PORTn=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			PORTn=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			PORTn=31;
			break;
		default:
			return;
	}
	CommAddr=MRR(rxbuff+5,6);
	MeterNo=Check_AFN04F10_Addr(PORTn,CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
	if(MeterNo==0)
	{
		return;//没找到
	}
	MeterNo--;


	p8s=(u8*)(rxbuff);
	y=p8s[29];
	
	MWR(PORTn,ADDR_DATABUFF,1);//1byte 端口号
	MR(ADDR_DATABUFF+1,rxbuff+5,6);//6BYTE 通信地址
	MWR(0,ADDR_DATABUFF+7,1);//发生次数长度=0

	MWR(18,ADDR_DATABUFF+8,2);//2byte长度
	
	MWR(0x040005FF,ADDR_DATABUFF+32,4);//数据标识
	MWR(y,ADDR_DATABUFF+36,1);//状态字

	Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
#endif
}

void Terminal_ReadMeter_DL698_TxData(u32 PORTn,u8 *p8RMLib)//终端DL698表规约抄表发送数据
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	u8* p8esamrxbuff;
	u32 index = 0;
	u32 fcs;
	//test_log_printf();
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8esamrxbuff=(u8 *)(ADDR_UART7Rx);
	p8txbuff[index++]=0x68;
	index+=2;//留2字节放长度
	if(UARTCtrl->Last_Block_No!=0xffffffff)
		p8txbuff[index++]=0x63;//控制码  63  e3
	else
		p8txbuff[index++]=0x43;//控制码
	p8txbuff[index++]=0x05;//用F10里的表地址，默认是6字节
	//取通信地址
	MR(((u32)p8txbuff)+index,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	index+=6;
	p8txbuff[index++]=0x10;//表示不关注客户机地址
	index+=2;//留2字节放HCS  (下标12 13)

	if(UARTCtrl->Last_Block_No!=0xffffffff)//发分帧确认帧
	{
		p8txbuff[index++]=(UARTCtrl->Last_Block_No+0x8000)&0xff;
		p8txbuff[index++]=((UARTCtrl->Last_Block_No+0x8000)>>8)&0xff;
	}
	else
	{
		p8txbuff[index++]=0x10;//安全传输
		p8txbuff[index++]=0x00;//明文
		index++;//1字节放长度 octet_string的长度	[16  ] 
		p8txbuff[index++]=0x05;
		//命令码
		i=(u32)p8RMLib;
		i+=10+(5*UARTCtrl->RMCount);

		if(((MRR(i,4)&0xFF00FFFF)==0x30000200)||((MRR(i,4)&0xFF00FFFF)==0x30000700)||((MRR(i,4)&0xFF00FFFF)==0x30000800)||((MRR(i,4)&0xFF00FFFF)==0x30000900))//读事件记录
		{
			p8txbuff[index++]=0x03;
			p8txbuff[index++]=0x03;
			MR(((u32)p8txbuff)+index,i,4);
			Data_Inverse(((u32)p8txbuff)+index,4);
			index+=4;
			p8txbuff[index++]=0x09;//RSD 方法一
			p8txbuff[index++]=0x01;//上一次
			p8txbuff[index++]=0x00; //RCSD的个数,0 = 不选 = 全选
			//p8txbuff[index++]=0x01; //RCSD的个数
			//p8txbuff[index++]=0x00;//类型是OAD
			
			//p8txbuff[index++]=0x20;
			//p8txbuff[index++]=0x1e;
			//p8txbuff[index++]=0x02;
			//p8txbuff[index++]=0x00;
		}
		else if(((p8RMLib[4]&0x08)!=0)||((p8RMLib[4]&0x20)!=0))//数据分类:B5=月冻结,
		{
			p8txbuff[index++]=0x03;
			p8txbuff[index++]=0x03;
			p8txbuff[index++]=0x50;
			if((p8RMLib[4]&0x08)!=0)
				p8txbuff[index++]=0x04;
			else
				p8txbuff[index++]=0x06;
			p8txbuff[index++]=0x02;
			p8txbuff[index++]=0x00;
			p8txbuff[index++]=0x01;//RSD 方法一
			p8txbuff[index++]=0x20;//0x20210200  数据冻结时间的OAD
			p8txbuff[index++]=0x21;
			p8txbuff[index++]=0x02;
			p8txbuff[index++]=0x00;
			index+=Get_698DateTime_st(p8txbuff+index,p8RMLib[4]);
			p8txbuff[index++]=0x01; //RCSD的个数
			p8txbuff[index++]=0x00;//类型是OAD
			MR(((u32)p8txbuff)+index,i,4);
			Data_Inverse(((u32)p8txbuff)+index,4);
			index+=4;
		}
		
		else //数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2
		{
			p8txbuff[index++]=0x01;
			p8txbuff[index++]=0x01;
			MR(((u32)p8txbuff)+index,i,4);
			Data_Inverse(((u32)p8txbuff)+index,4);
			index+=4;
		}
		//没有时间标签
		p8txbuff[index++]=0x00;
		//补明文长度
		p8txbuff[16]=index-16-1;
		
		p8txbuff[index++]=0x01;//数据验证信息	随机数			[1]  RN，
		p8txbuff[index++]=0x10;//RN 长度
		i=TESAM_CMD_TxRx(0x801A1000,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误 读随机数
		//i=TESAM_CMD_TxRx(0x800E0002,0,0);//老ESAM读序列号
		//i=TESAM_CMD_TxRx(0x80040010,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
		//i=TESAM_CMD_TxRx(0x80360002,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误,读新ESAM的序列号

		if(i)//因为是明文+RN的方式，实际上是不验证的，随机数可以不是随机数
		{
			MC(0xee,(u32)(p8txbuff+index),16);//存储器清0
		}
		else
		{
			MR((u32)(p8txbuff+index),((u32)p8esamrxbuff)+5,16);
			Data_Inverse((u32)(p8txbuff+index),16);//数据倒序(高低字节调换),入口Le>=2
		}
		
		index+=16;

	}
	


//补长度
	p8txbuff[1]=(index+1)&0xff;
	p8txbuff[2]=((index+1)>>8)&0x3f;

	fcs=Get_16bitfcs_code(p8txbuff+1,11)^0xffff;
	p8txbuff[12]=fcs&0xff;
	p8txbuff[13]=(fcs>>8)&0xff;

	//HCS计算
	fcs=Get_16bitfcs_code(p8txbuff+1,index-1)^0xffff;
	p8txbuff[index++]=fcs&0xff;
	p8txbuff[index++]=(fcs>>8)&0xff;
	p8txbuff[index++]=0x16;
	UARTCtrl->TxByte=index;//TX计数(需发送字节数)
}

 

void ReadMeter_DL698_FnData(u32 PORTn,u8 *p8RMLib)//698.45规约表抄表接收数据填入Fn数据缓冲
{
	ReadMeter_DL698_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//698.45规约表抄表接收数据填入Fn数据缓冲
}

void ReadMeter_DL698_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//698.45规约表抄表接收数据填入Fn数据缓冲
{
		
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 YM=0;
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;
	u32 index;
	u32 Pn;
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

	i=p8RMLib[0];
	//打印测试
	//Log_Printf_Level(LOG_LEVEL_DEBUG,"log test di=%x\n",0x05060101);
	//Log_Dumphex_Level(LOG_LEVEL_DEBUG,"dddd",ADDR_AFN04F10,10);
	//Add_Log_Date_Head(ADDR_DATABUFF);
	//sprintf((char*)(ADDR_DATABUFF+17),"i=%d\n",0x05060101);
	//Write_Log_To_File(ADDR_DATABUFF,strlen((u8*)ADDR_DATABUFF));
	//测试结束
	switch(i)
	{
		case 3://F3 周期抄读电能表事件
			if(UARTCtrl->RMCount==75)
			{
				if(p8rxbuff[24]!=1)//回的不是数据
					break;
				if(p8rxbuff[25]!=1)//回的数据类型不是ARRAY
					break;
				if(p8rxbuff[26]!=7)//有7组状态字
					break;
				if(p8rxbuff[29]&0x0c)// 停电抄表电池欠压   时钟电池欠压
				{
					y=0;//上报标志：0=不上报，1=上报
					//主动上报状态字bit3时钟电池电压低
					//电表运行状态字1 bit2
					i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+3,1);
					if(p8rxbuff[29]&0x04)
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
					if(p8rxbuff[29]&0x08)
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
					{
						WriteReportWord(PORTn,((u32)p8rxbuff));
					}
				}
				else
				{
					break;
				}
			}
			
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				break;
			}
			Pn--;
			if((UARTCtrl->RMCount%3)==0)//抄表命令计数
			{//读次数
				z=UARTCtrl->RMCount;
				UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
				if(p8rxbuff[24]!=1)//回的不是数据
					break;
				z=0;
				switch(UARTCtrl->RMCount)
				{
					case 6:
					case 15:
					case 24:
					case 33:
					case 42:
					case 51:	
						i=MRR(((u32)p8rxbuff)+30,4);//double_long-unsigned//当前次数
						i=Change_Data(i,4);
						YM=MRR(((u32)p8rxbuff)+35,4);//double_long-unsigned//持续时间
						YM=Change_Data(YM,4);
						z=1;
						break;
					case 9:
					case 18:
					case 27:
					case 36:
					case 45:
					case 54:
						i=MRR(((u32)p8rxbuff)+42,4);//double_long-unsigned//当前次数
						i=Change_Data(i,4);
						YM=MRR(((u32)p8rxbuff)+47,4);//double_long-unsigned//持续时间
						YM=Change_Data(YM,4);
						z=2;
						break;
					case 12:
					case 21:
					case 30:
					case 39:
					case 48:
					case 57:	
						i=MRR(((u32)p8rxbuff)+54,4);//double_long-unsigned//当前次数
						i=Change_Data(i,4);
						YM=MRR(((u32)p8rxbuff)+59,4);//double_long-unsigned//持续时间
						YM=Change_Data(YM,4);
						z=3;
						break;
					default:
						i=MRR(((u32)p8rxbuff)+26,2);//long-unsigned//当前次数
						i=Change_Data(i,2);
						YM=0;
						z=0;
						break;
				}
				if(z!=0)////总累计时间
				{
					if(MRR((u32)p8FnData,2)==0xeeee)
					{//第1帧标志
						//长度
						p8FnData[0]=7;//2byte 事件数据长度
						p8FnData[1]=0;
						//数据
						y=MRR(((u32)p8rxbuff)+20,4);//获得OAD
						y=Change_Data(y,4);
						y=Get_LastTimeDi_From_OAD(y,z);
						MWR(y,((u32)p8rxbuff)+512-7,4);//DI
						MWR(YM,((u32)p8rxbuff)+512-3,3);//持续时间
					}
				}
				x=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),3);//原次数(初始化值0xff)
				if((x==0xffffff)||(i<x))
				{//初始化值,或当前次数<原次数可能换表
					MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),4);//新增次数=0
//终端数据初始化后,当抄读到开表盖（次数不为0）、时钟电池欠压（发生状态）、停电抄表电池欠压（发生状态）时，终端需立即上报事件（总次数及对应内容）。
					//x=0;//-----------------------------------------------
					y=MRR(((u32)p8rxbuff)+20,4);//获得OAD
					y=Change_Data(y,4);
					if(y==0x301B0400)
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
				//x=1;//-----------------------------------------------
				i|=x<<24;
				MWR(i,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),4);//新增次数
			}
			else
			{//读1-10次事件
				//存事件
				if((UARTCtrl->RMCount%3)==2)
				{
					i=MRR((u32)p8FnData+2,2);
					if(i==0xeeee)
					{//第1帧标志
						//长度
						if(p8rxbuff[16]&0x80)
						{
							if((p8rxbuff[16]&0x7f)==2)
							{
								YM=MRR(((u32)p8rxbuff)+22,4);
								YM=Change_Data(YM,4);
								i=Get_Di_From_OAD(YM);
								MWR(i,((u32)p8rxbuff)+512,4);
								i=Change_Form_698_to_376(YM,((u32)p8rxbuff)+512+4,((u32)p8rxbuff)+26);
								p8FnData[2]=i+4;//2byte 事件数据长度
								p8FnData[3]=(i+4)>>8;
							}
							else if((p8rxbuff[16]&0x7f)==1)							
							{
								YM=MRR(((u32)p8rxbuff)+21,4);
								YM=Change_Data(YM,4);
								i=Get_Di_From_OAD(YM);
								MWR(i,((u32)p8rxbuff)+512,4);
								i=Change_Form_698_to_376(YM,((u32)p8rxbuff)+512+4,((u32)p8rxbuff)+25);
								p8FnData[2]=i+4;//2byte 事件数据长度
								p8FnData[3]=(i+4)>>8;
							}
							else
							{
								break;
							}
						}
						else
						{
							YM=MRR(((u32)p8rxbuff)+20,4);
							YM=Change_Data(YM,4);
							i=Get_Di_From_OAD(YM);
							MWR(i,((u32)p8rxbuff)+512,4);
							i=Change_Form_698_to_376(YM,((u32)p8rxbuff)+512+4,((u32)p8rxbuff)+24);
							p8FnData[2]=i+4;//2byte 事件数据长度
							p8FnData[3]=(i+4)>>8;
						}
					}
				}

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
					MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+5,6);//6BYTE 通信地址
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
					
					i=Get_EventNumDi_From_OAD(YM);
					MWR(i,ADDR_DATABUFF+10,4);
					
					//取当前次数值
					i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/3)),3);
					i=hex_bcd(i);
					MWR(i,ADDR_DATABUFF+14,3);
					
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
					p8FnData[2]=0xee;//第1帧数据标志
					p8FnData[3]=0xee;
					if(i!=0)
					{//次数没到0,继续读本帧
						UARTCtrl->RMCount--;//抄表命令计数-1
					}
				}
				return;
			}
		case 27://   F27 电能表日历时钟、编程次数及其最近一次操作时间
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表日历时钟YYMMDDWWHhmmss
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					DateTimeStoDCB(p8rxbuff+25,(u8 *)(ADDR_DATABUFF));
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					p8[3]=i;
					p8[5]+=(p8[3]<<5);
					MW(ADDR_DATABUFF,((u32)p8FnData)+6,3);
					MW((u32)(p8+4),((u32)(p8FnData))+6+3,3);
					break;
				case 1://电池工作时间 4字节
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					if(p8rxbuff[25]!=6)//数据类型不是6
						break;
					YM=MRR(((u32)p8rxbuff)+26,4);//double-long-unsigned
					YM=Change_Data(YM,4);
					YM=hex_bcd(YM);
					MWR(YM,((u32)(p8FnData))+6+6,4);
					break;
				case 2://编程总次数   3字节
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					if(p8rxbuff[25]!=0x12)//数据类型不是6
						break;
					p8FnData+=6+6+4;
					p8rxbuff+=25;
					goto DEAL_LONG_UNSIGNED;
				case 3://最近一次编程发生时刻 6字节
					p8FnData+=6+6+4+3;
					if(p8rxbuff[16]&0x80)
					{	
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
					goto DEAL_DATE_TIME;
				case 4://电表清零总次数 3字节
					p8FnData+=6+6+4+3+6;
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					if(p8rxbuff[25]!=0x12)//数据类型不是6
						break;
					p8rxbuff+=25;
					goto DEAL_LONG_UNSIGNED;
				case 5://最近一次清零发生时刻 6字节
					p8FnData+=6+6+4+3+6+3;
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
					goto DEAL_DATE_TIME;
				case 6://电表清零总次数 3字节
					p8FnData+=6+6+4+3+6+3+6;
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					if(p8rxbuff[25]!=0x12)//数据类型不是6
						break;
					p8rxbuff+=25;
					goto DEAL_LONG_UNSIGNED;
				case 7://最近一次需量清零发生时刻6字节
					p8FnData+=6+6+4+3+6+3+6+3;
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
					goto DEAL_DATE_TIME;
				case 8://需量清零总次数	3字节
					p8FnData+=6+6+4+3+6+3+6+3+6;
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					if(p8rxbuff[25]!=0x12)//数据类型不是6
						break;
					p8rxbuff+=25;
					goto DEAL_LONG_UNSIGNED;
				case 9://最近一次事件清零发生时刻6字节
					p8FnData+=6+6+4+3+6+3+6+3+6+3;
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
					goto DEAL_DATE_TIME;
				case 10://校时总次数	3字节
					p8FnData+=6+6+4+3+6+3+6+3+6+3+6;
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					if(p8rxbuff[25]!=0x12)//数据类型不是6
						break;
					p8rxbuff+=25;
DEAL_LONG_UNSIGNED:
					YM=MRR((u32)(p8rxbuff+1),2);//long-unsigned
					YM=Change_Data(YM,2);
					YM=hex_bcd(YM);
					MWR(YM,((u32)p8FnData),3);
					break;
				case 11://最近一次校时发生时刻(用校时后时间)
					p8FnData+=6+6+4+3+6+3+6+3+6+3+6+3;
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							p8rxbuff+=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						p8rxbuff+=24;
					}
					index=Get_OAD_Data_Position(p8rxbuff,0x201e0200,1);
					if(index==0)
						break;
					else
						p8rxbuff+=index;
DEAL_DATE_TIME:
					DateTimeStoDCB(p8rxbuff,(u8 *)(ADDR_DATABUFF));
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					p8[3]=i;
					p8[5]+=(p8[3]<<5);
					MW(ADDR_DATABUFF,((u32)p8FnData),3);
					MW((u32)(p8+4),(u32)(p8FnData+3),3);
					break;

			}
			break;
//16 F129 当前正向有功电能示值（总、费率1～M）
		case 129:
		case 131:
			UARTCtrl->RMError&=(~0x02);//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			if(p8rxbuff[24]==0)//回的不是数据
				break;
			if((p8rxbuff[24]==1)&&(p8rxbuff[25]!=1))//回的是数据，但是数据类型不是ARRAY
				break;
			p8rxbuff+=26;
			p8FnData+=6;
MPDIRPOW://正向有功--5字节
			x=p8rxbuff[0];//费率数
			p8FnData[0]=x-1;
			p8FnData+=1;
			p8rxbuff+=1;
			for(i=0;i<x;i++)
			{
				YM=MRR((u32)(p8rxbuff+1+i*5),4);//double-long-unsigned
				YM=Change_Data(YM,4);
				YM=hex_bcd(YM);
				p8FnData[i*5]=0;
				MWR(YM,((u32)(p8FnData+1+i*5)),4);
			}

			break;			
		case 130:
		case 132:
		case 133:
		case 134:
		case 135:
		case 136:
			if(p8rxbuff[24]==0)//回的不是数据
				break;
			if((p8rxbuff[24]==1)&&(p8rxbuff[25]!=1))//回的是数据，但是数据类型不是ARRAY
				break;
			p8rxbuff+=26;
			p8FnData+=6;
MPDIRVAR://正向无功---4字节
			
			x=p8rxbuff[0];//费率数
			p8FnData[0]=x-1;
			p8FnData+=1;
			p8rxbuff+=1;
			for(i=0;i<x;i++)
			{
				YM=MRR((u32)(p8rxbuff+1+i*5),4);//double-long-unsigned
				YM=Change_Data(YM,4);
				YM=hex_bcd(YM);
				MWR(YM,((u32)(p8FnData+i*4)),4);
			}

			break;
		
			
//49 F162 电能表日历时钟
		case 162:
			//MW(ADDR_TYMDHMS+1,(u32)(p8FnData+1),5);//填入抄表时间
			MW(ADDR_AFN0CF2,((u32)p8FnData)+1,6);//填入抄表时间
			p8rxbuff+=25;
			DateTimeStoDCB(p8rxbuff,(u8 *)(ADDR_DATABUFF));
			p8=(u8 *)ADDR_DATABUFF;
			p8[3]=0;
			YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
			i=p8[3];
			if(i==0x0)
			{
				i=7;
			}
			p8[3]=i;
			p8[5]+=(p8[3]<<5);

			p8FnData+=7;
			MW(ADDR_DATABUFF,((u32)p8FnData),3);
			MW((u32)(p8+4),(u32)(p8FnData+3),3);


			break;
		case 165://开表盖，开端钮盖
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//本序号的电能表/交流采样装置删除
				break;
			}
			
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表尾盖打开次数
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					YM=MRR(((u32)p8rxbuff)+26,2);//long-unsigned
					YM=Change_Data(YM,2);
					YM=hex_bcd(YM);
					MWR(YM,((u32)p8FnData)+6+3+6,3);
					MC(0,((u32)p8FnData)+64,1);//清尾盖打开次数变化标志
					if(Pn<=ERC38MaxPn)
					{
						i=MRR(ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR)+3,3);//原次数
						MWR(YM,ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR)+3,3);//更新原次数
						if(i!=0xeeeeee)
						{
							if(i!=YM)
							{//次数变化
								MC(0x55,((u32)p8FnData)+64,1);//尾盖打开次数变化标志
							}
						}
					}
					break;
				case 1://最近一次尾盖打开时间
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							index=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						index=24;
					}
					i=Get_OAD_Data_Position(p8rxbuff+index,0x201e0200,1);
					if(i==0)
						break;
					index+=i;
					
					DateTimeStoDCB(p8rxbuff+index,(u8 *)(ADDR_DATABUFF));
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					p8[3]=i;
					p8[5]+=(p8[3]<<5);
					MW(ADDR_DATABUFF,((u32)p8FnData)+6+3+6+3,3);
					MW((u32)(p8+4),((u32)p8FnData)+6+3+6+3+3,3);
					
					x=MRR(((u32)p8FnData)+64,1);//尾盖打开次数变化标志
					if(x==0x55)
					{//打开次数变化标志
					#if ERC38==1
						ERC38_Event(Pn,((u32)p8FnData)+6+3+6);//电能表开端钮盒事件记录;入口:Pn测量点号(从1开始)
					#endif
					}
					break;
				case 2://电能表开表盖打开次数
					if(p8rxbuff[24]!=1)//回的不是数据
						break;
					YM=MRR(((u32)p8rxbuff)+26,2);//long-unsigned
					YM=Change_Data(YM,2);
					YM=hex_bcd(YM);
					MWR(YM,((u32)p8FnData)+6+3+6+3+6,3);
					MC(0,((u32)p8FnData)+64,1);//清尾盖打开次数变化标志
					if(Pn<=ERC38MaxPn)
					{
						i=MRR(ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR),3);//原次数
						MWR(YM,ADDR_ERC37_38_SOUR+((Pn-1)*LEN_ERC37_38_SOUR),3);//更新原次数
						if(i!=0xeeeeee)
						{
							if(i!=YM)
							{//次数变化
								MC(0x55,((u32)p8FnData)+64,1);//尾盖打开次数变化标志
							}
						}
					}
					break;
				case 3:
					if(p8rxbuff[16]&0x80)
					{
						if((p8rxbuff[16]&0x7f)==2)
						{
							p8rxbuff+=26;
						}
						else if((p8rxbuff[16]&0x7f)==1)
						{
							index=25;
						}
						else
						{
							break;
						}
					}
					else
					{
						index=24;
					}
					i=Get_OAD_Data_Position(p8rxbuff+index,0x201e0200,1);
					if(i==0)
						break;
					index+=i;
					DateTimeStoDCB(p8rxbuff+index,(u8 *)(ADDR_DATABUFF));
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					p8[3]=i;
					p8[5]+=(p8[3]<<5);
					MW(ADDR_DATABUFF,((u32)p8FnData)+6+3+6+3+6+3,3);
					MW((u32)(p8+4),((u32)p8FnData)+6+3+6+3+6+3+3,3);
					
					x=MRR(((u32)p8FnData)+64,1);//尾盖打开次数变化标志
					if(x==0x55)
					{//打开次数变化标志
					#if ERC37==1
						ERC37_Event(Pn,((u32)p8FnData)+6+3+6+3+6);//电能表开端钮盒事件记录;入口:Pn测量点号(从1开始)
					#endif
					}
					break;
			}
			break;
//扩展3 集中抄表日冻结电能示值（总、费率1～M）
		case 0xE3:
			if(p8rxbuff[30]!=1)//回的不是数据
				break;
			if(p8rxbuff[31]!=1)//数据条数不是1
				break;
			if(p8rxbuff[32]!=1)//数据类型不是 ARRAY
				break;
			p8rxbuff+=33;
			x=MRR(ADDR_TYMDHMS+3,3);
			MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
			p8FnData[0]=1;//有数据填入标志
			switch(UARTCtrl->RMCount)
			{
				case 0://F161
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF161_LastDate_Source,5);//抄表时间
					p8FnData+=6+Office_AFN0DF161_LastDate_Source+5;
					goto MPDIRPOW;
				case 1://F162
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source,5);//抄表时间
					p8FnData+=6+Office_AFN0DF162_LastDate_Source+5;
					goto MPDIRVAR;
				case 2://F163
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF163_LastDate_Source,5);//抄表时间
					p8FnData+=6+Office_AFN0DF163_LastDate_Source+5;
					goto MPDIRPOW;
				case 3://F164
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source,5);//抄表时间
					p8FnData+=6+Office_AFN0DF164_LastDate_Source+5;
					goto MPDIRVAR;
				case 4://F165
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source,5);//抄表时间
					p8FnData+=6+Office_AFN0DF165_LastDate_Source+5;
					goto MPDIRVAR;
				case 5://F166
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source,5);//抄表时间
					p8FnData+=6+Office_AFN0DF166_LastDate_Source+5;
					goto MPDIRVAR;
				case 6://F167
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source,5);//抄表时间
					p8FnData+=6+Office_AFN0DF167_LastDate_Source+5;
					goto MPDIRVAR;
				case 7://F168
					MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source,5);//抄表时间
					p8FnData+=6+Office_AFN0DF168_LastDate_Source+5;
					goto MPDIRVAR;
				default:
					return;
			}
//			break;

//扩展11 集中抄表日冻结正向有功最大需量及发生时间（总、费率1～M）
//扩展12 集中抄表日冻结反向有功最大需量及发生时间（总、费率1～M）
		case 149:
		case 151:
		case 0xEB:
		case 0xEC:
			if(p8rxbuff[30]!=1)//回的不是数据
				break;
			if(p8rxbuff[31]!=1)//数据条数不是1
				break;
			if(p8rxbuff[32]!=1)//数据类型不是 ARRAY
				break;
			p8rxbuff+=33;

			p8FnData[0]=1;//有数据填入标志
			MW(ADDR_TYMDHMS+1,((u32)p8FnData)+1,5);//抄表时间
			p8FnData+=6;			

			index=0;
			p8FnData[0]=p8rxbuff[index];//费率
			x=p8FnData[0];
			index++;
			p8FnData+=1;
			for(i=0;i<x;i++)
			{
				index+=3;//去掉 02 02这2字节 和  数据类型06
				YM=MRR((u32)(p8rxbuff+index),4);//double-long-unsigned
				YM=Change_Data(YM,4);
				YM=hex_bcd(YM);	
				index+=4;//需量处理完了
				
				DateTimeStoDCB(p8rxbuff+index,(u8 *)(ADDR_DATABUFF));
				p8=(u8 *)ADDR_DATABUFF;
				index+=8;//需量发生时间处理完了

				MWR(YM,(u32)(p8FnData+7*i),3);
				MW((u32)(p8+1),(u32)(p8FnData+7*i+3),2);
				MW((u32)(p8+4),(u32)(p8FnData+7*i+5),2);
			}
			break;
		case 137:
		case 139:
			if(p8rxbuff[30]!=1)//回的不是数据
				break;
			if(p8rxbuff[31]!=1)//数据条数不是1
				break;
			if(p8rxbuff[32]!=1)//数据类型不是 ARRAY
				break;
			p8rxbuff+=33;
			
			goto	MPDIRPOW;

		case 138:
		case 140:
		case 141:
		case 142:
		case 143:
		case 144:
			if(p8rxbuff[30]!=1)//回的不是数据
				break;
			if(p8rxbuff[31]!=1)//数据条数不是1
				break;
			if(p8rxbuff[32]!=1)//数据类型不是 ARRAY
				break;
			p8rxbuff+=33;
			
			goto	MPDIRVAR;
		default:
			break;
	}
}
#endif





