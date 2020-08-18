
//终端抄表DL645_1997接收数据填入Fn数据缓冲
#include "Project.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_CountAndFlag.h"




extern const u32 DL645_1997_RMLib[];
void Terminal_ReadMeter_DL645_1997_TxData(u32 PORTn,u8 *p8RMLib)//终端DL645-1997抄表发送数据
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	MC(0xfe,(u32)p8txbuff,4);
	p8txbuff[4+0]=0x68;
	//取通信地址
	MR(((u32)p8txbuff)+4+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	p8txbuff[4+7]=0x68;
	p8txbuff[4+8]=0x01;
	p8txbuff[4+9]=0x2;
	//取DI
	//i=DL645_1997_RMLib[UARTCtrl->FnCount];
	i=(u32)p8RMLib;
	i+=10+(3*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+4+10,i,2);
	//加0x33
	for(i=0;i<2;i++)
	{
		p8txbuff[4+10+i]+=0x33;
	}
	//CS计算
	p8txbuff[4+12]=0;
	for(i=0;i<12;i++)
	{
		p8txbuff[4+12]+=p8txbuff[4+i];
	}
	p8txbuff[4+13]=0x16;
	UARTCtrl->TxByte=4+14;//TX计数(需发送字节数)
}

void ReadMeter_DL645_1997_FnData(u32 PORTn,u8 *p8RMLib)//DL645_1997抄表接收数据填入Fn数据缓冲
{
	ReadMeter_DL645_1997_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//DL645_1997抄表接收数据填入Fn数据缓冲
}

void ReadMeter_DL645_1997_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//DL645_1997抄表接收数据填入Fn数据缓冲
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u64 d64a;
	u64 d64b;

	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

		
	//i=MRR(DL645_1997_RMLib[UARTCtrl->FnCount],1);
	i=p8RMLib[0];
	switch(i)
	{
#if (USER/100)==7//浙江用户
	#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
		#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
//F2 电能表主动上报事件
		case 2:
		//不读后续帧
			UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
			//清事件标志,免死循环
			MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+(UARTCtrl->RMCount/2),1);
			i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),4);
			if(i==0)
			{
				MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
			}
		//存事件
			i=MRR((u32)p8FnData,2);
			if(i==0xeeee)
			{//第1帧标志或抄表计数不相同
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
				MC(0,ADDR_DATABUFF,512);
				p8=(u8 *)ADDR_DATABUFF;
				MWR(i,ADDR_DATABUFF,1);//1byte 端口号
				MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE 通信地址

				i=(u32)p8RMLib;
				i+=10+(3*UARTCtrl->RMCount)+2;
				MR(ADDR_DATABUFF+7,i,1);
				MR(ADDR_DATABUFF+10,((u32)p8rxbuff)+10,2);//数据标识
				p8[12]=0;
				p8[13]=0;
				MR(ADDR_DATABUFF+14,((u32)p8rxbuff)+12,p8[7]);//数据
				p8[7]+=4;//总长度

				Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
			}
			break;
		#endif
	#endif
#endif
//1 F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
		case 25:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			y=MRR(ADDR_AFN04F25+((Pn-1)*11)+10,1);//取电源接线方式 BS8  1
			y&=0x3;//D0～D1编码表示测量点电源接线方式，数值1～3依次表示三相三线、三相四线、单相表
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总有功功率
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6,3);
					i=GetClass1DataAddr(28,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
					if(i!=0x0)
					{
						p8=(u8 *)(i);
						if(p8[6]!=0xee)
						{
							if((p8[6]&0x10)!=0x0)
							{//有功功率方向负
								p8FnData[6+2]|=0x80;
							}
						}
					}
					break;
				case 1://A有功功率
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+3,3);
					break;
				case 2://B有功功率
					if(y==1)
					{//三相三线
						break;
					}
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+6,3);
					break;
				case 3://C有功功率
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+9,3);
					break;
				case 4://总无功功率
					p8FnData[6+12]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+13,2);
					i=GetClass1DataAddr(28,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
					if(i!=0x0)
					{
						p8=(u8 *)(i);
						if(p8[6]!=0xee)
						{
							if((p8[6]&0x20)!=0x0)
							{//无功功率方向负
								p8FnData[6+14]|=0x80;
							}
						}
					}
					break;
				case 5://A无功功率
					p8FnData[6+15]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+16,2);
					break;
				case 6://B无功功率
					if(y==1)
					{//三相三线
						break;
					}
					p8FnData[6+18]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+19,2);
					break;
				case 7://C无功功率
					p8FnData[6+21]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+22,2);
					break;
				case 8://总功率因数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+24,2);
					break;
				case 9://A功率因数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+26,2);
					break;
				case 10://B功率因数
					if(y==1)
					{//三相三线
						break;
					}
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+28,2);
					break;
				case 11://C功率因数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+30,2);
					//总,A,B,C视在功率(用有功功率/功率因数计算)
					for(i=0;i<4;i++)
					{
						x=MRR(((u32)p8FnData)+6+(3*i),3);//取有功功率
						if(x!=0xeeeeee)
						{
							x&=0x7fffff;//去符号位
							x=bcd_hex(x);
							d64a=x;
							d64a*=1000;//功率因数有3位小数
							x=MRR(((u32)p8FnData)+6+24+(2*i),2);//取功率因数
							if(x!=0xeeee)
							{
								x&=0x7fff;//去符号位
								x=bcd_hex(x);
								d64a/=x;
								x=d64a;
								x=hex_bcd(x);
								MWR(x,((u32)p8FnData)+6+50+(3*i),3);
							}
						}
					}
					break;
				case 12://A相电压
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+32]=i;
					p8FnData[6+33]=i>>8;
					break;
				case 13://B相电压
					if(y==1)
					{//三相三线
						break;
					}
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+34]=i;
					p8FnData[6+35]=i>>8;
					break;
				case 14://C相电压
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+36]=i;
					p8FnData[6+37]=i>>8;
					break;
				case 15://A相电流
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+38]=i;
					p8FnData[6+39]=i>>8;
					p8FnData[6+40]=i>>16;
					break;
				case 16://B相电流
					if(y==1)
					{//三相三线
						break;
					}
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+41]=i;
					p8FnData[6+42]=i>>8;
					p8FnData[6+43]=i>>16;
					break;
				case 17://C相电流
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					p8FnData[6+44]=i;
					p8FnData[6+45]=i>>8;
					p8FnData[6+46]=i>>16;
					break;
				//零序电流
			}
			break;
//2 F26 A、B、C三相断相统计数据及最近一次断相记录
		case 26:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总断相次数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6,2);
					p8FnData[6+2]=0;
					break;
				case 1://A断相次数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+3,2);
					p8FnData[6+3+2]=0;
					break;
				case 2://B断相次数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+6,2);
					p8FnData[6+6+2]=0;
					break;
				case 3://C断相次数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+9,2);
					p8FnData[6+9+2]=0;
					break;
				case 4://总断相时间累计值
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+12,3);
					break;
				case 5://A断相时间累计值
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+15,3);
					break;
				case 6://B断相时间累计值
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+18,3);
					break;
				case 7://C断相时间累计值
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+21,3);
					break;
				case 8://总断相起始时刻
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+24,4);
					break;
				case 9://A断相起始时刻
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+28,4);
					break;
				case 10://B断相起始时刻
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+32,4);
					break;
				case 11://C断相起始时刻
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+36,4);
					break;
				case 12://总断相结束时刻
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+40,4);
					break;
				case 13://A断相结束时刻
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+44,4);
					break;
				case 14://B断相结束时刻
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+48,4);
					break;
				case 15://C断相结束时刻
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+52,4);
					break;
			}
			break;
//3 F27 电能表日历时钟、编程次数及其最近一次操作时间
		case 27:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表日历时钟Hhmmss
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6,3);
					break;
				case 1://电能表日历时钟YYMMDDWW
					MW(((u32)p8rxbuff)+12+1,((u32)p8FnData)+6+3,3);
					//星期
					i=p8rxbuff[12];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					p8FnData[6+3+1]|=i;
					break;
				case 2://电池工作时间
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+6,3);
					p8FnData[6+9]=0;
					break;
				case 3://编程总次数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+10,2);
					p8FnData[6+12]=0;
					break;
				case 4://最近一次编程发生时刻
					p8FnData[6+13]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+14,4);
					p8FnData[6+18]=0;
					break;
					//电表清零总次数
					//最近一次电表清零发生时刻
				case 5://最大需量清零次数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+28,2);
					p8FnData[6+30]=0;
					break;
				case 6://最近一次最大需量清零发生时刻
					p8FnData[6+31]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+32,4);
					p8FnData[6+36]=0;
					break;
					//事件清零总次数
					//最近一次事件清零发生时刻
					//校时总次数
					//最近一次校时发生时刻
			}
			break;
//4 F28 电表运行状态字及其变位标志
		case 28:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电表运行状态字
					for(i=0;i<14;i++)
					{
						p8FnData[6+14+i]=0;
					}
					p8FnData[6+14]=p8rxbuff[12];
					break;
				case 1://电网状态字
					if((p8rxbuff[12]&0x1)!=0x0)
					{//A断电
						p8FnData[6+14+6]|=0x80;
					}
					if((p8rxbuff[12]&0x10)!=0x0)
					{//A过压
						p8FnData[6+14+6]|=0x04;
					}
					if((p8rxbuff[12]&0x2)!=0x0)
					{//B断电
						p8FnData[6+14+8]|=0x80;
					}
					if((p8rxbuff[12]&0x20)!=0x0)
					{//B过压
						p8FnData[6+14+8]|=0x04;
					}
					if((p8rxbuff[12]&0x4)!=0x0)
					{//C断电
						p8FnData[6+14+10]|=0x80;
					}
					if((p8rxbuff[12]&0x40)!=0x0)
					{//C过压
						p8FnData[6+14+10]|=0x04;
					}
					break;
			}
			break;
//5 F29 当前铜损、铁损有功总电能示值
//6 F30 上一结算日铜损、铁损有功总电能示值
		case 29:
		case 30:
			break;
//7 F31 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
//8 F32 上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
		case 31:
		case 32:
			break;

//9 F49 当前电压、电流相位角
//10 F57 当前A、B、C三相电压、电流2～N次谐波有效值
//11 F58 当前A、B、C三相电压、电流2～N次谐波含有率
//12 F65 当前电容器投切状态
//13 F66 当前电容器累计补偿投入时间和次数
//14 F67 当日、当月电容器累计补偿的无功电能量
//15 F73 直流模拟量当前数据


//16 F129 当前正向有功电能示值（总、费率1～M）
//24 F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
		case 129:
		case 137:
			//if(i==129)
			//{
				UARTCtrl->RMError&=(~0x02);//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			//}
			#if (((USER/100)==5)&&((Project/100)==2))//上海集中器
				switch(UARTCtrl->RMCount)//抄表命令计数
				{
					case 0://正向有功
						x=p8rxbuff[9];
						if(x>=6)
						{
							x-=6;
							x/=4;
							if(x>TMaxTariff)
							{
								x=TMaxTariff;
							}
							p8FnData[6]=x;
							p8FnData+=7;
							p8rxbuff+=12;
							for(i=0;i<=x;i++)
							{
								p8FnData[0+(5*i)]=0;
								p8FnData[1+(5*i)]=p8rxbuff[0];
								p8FnData[2+(5*i)]=p8rxbuff[1];
								p8FnData[3+(5*i)]=p8rxbuff[2];
								p8FnData[4+(5*i)]=p8rxbuff[3];
								p8rxbuff+=4;
							}
						}
						break;
					case 1://反向有功加正向有功
		 				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
						switch(i)
						{
							case 0x51:
							case 0x52:
						C51F129://大类号5小类号1，所有正向有功数据取电能表的组合有功（若无组合有功，取正向+反向数据）
								x=p8rxbuff[9];
								if(x>=6)
								{
									x-=6;
									x/=4;
									if(x>TMaxTariff)
									{
										x=TMaxTariff;
									}
									p8FnData[6]=x;
									p8FnData+=7;
									p8rxbuff+=12;
									for(i=0;i<=x;i++)
									{
										p8FnData[60+(5*i)]=0;
										p8FnData[61+(5*i)]=p8rxbuff[0];
										p8FnData[62+(5*i)]=p8rxbuff[1];
										p8FnData[63+(5*i)]=p8rxbuff[2];
										p8FnData[64+(5*i)]=p8rxbuff[3];
										p8rxbuff+=4;
									}
									for(i=0;i<=x;i++)
									{
										d64a=MRR(((u32)p8FnData)+(5*i),5);
										d64a=bcd16_hex(d64a);
										d64b=MRR(((u32)p8FnData)+60+(5*i),5);
	 									d64b=bcd16_hex(d64b);
										d64a+=d64b;
										d64a=hex_bcd16(d64a);
										MWR(d64a,((u32)p8FnData)+(5*i),5);
									}
								}
								break;														
							case 0x02:
							case 0x62:
							case 0x71:
							case 0x72:
								break;
							default://其他按大类号5小类号1处理
								goto C51F129;
						}
						break;
				}										
			#else
				switch(UARTCtrl->RMCount)//抄表命令计数
				{
					case 0://块命令抄
						//有块命令跳过只抄总电量
						UARTCtrl->RMCount=1;//抄表命令计数+1
						goto F129DATA;
					case 1://只抄总电量
					
						p8FnData[6]=1;//费率数=1
						p8FnData+=7;
						p8rxbuff+=12;
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						MW((u32)p8FnData,((u32)p8FnData)+5,5);//费率1数据=总
						break;
					
						//goto F129DATA;			
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
	#if (((USER/100)!=5)||((Project/100)!=2))//非上海集中器
F129DATA:
	#endif
			x=p8rxbuff[9];
			if(x>=6)
			{
				x-=6;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				p8FnData+=7;
				p8rxbuff+=12;
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
//20 F133 当前一象限无功电能示值（总、费率1～M）
//21 F134 当前二象限无功电能示值（总、费率1～M）
//22 F135 当前三象限无功电能示值（总、费率1～M）
//23 F136 当前四象限无功电能示值（总、费率1～M）
//25 F138 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
//27 F140 上月（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
//28 F141 上月（上一结算日）一象限无功电能示值（总、费率1～M）
//29 F142 上月（上一结算日）二象限无功电能示值（总、费率1～M）
//30 F143 上月（上一结算日）三象限无功电能示值（总、费率1～M）
//31 F144 上月（上一结算日）四象限无功电能示值（总、费率1～M）
		case 130:
		case 132:
		case 133:
		case 134:
		case 135:
		case 136:
		case 138:
		case 140:
		case 141:
		case 142:
		case 143:
		case 144:
			x=p8rxbuff[9];
			if(x>=6)
			{
				x-=6;
				x/=4;
				if(x>TMaxTariff)
				{
					x=TMaxTariff;
				}
				p8FnData[6]=x;
				Pn=(x+1)*4;
				p8FnData+=7;
				p8rxbuff+=12;
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
		case 145:
		case 146:
		case 147:
		case 148:
		case 149:
		case 150:
		case 151:
		case 152:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://最大需量
					x=p8rxbuff[9];
					if(x>=5)
					{
						x-=5;
						x/=3;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						p8FnData[6]=x;
						p8FnData+=7;
						p8rxbuff+=12;
						for(i=0;i<=x;i++)
						{
							for(Pn=0;Pn<3;Pn++)
							{
								p8FnData[Pn+(7*i)]=p8rxbuff[Pn];
							}
							p8rxbuff+=3;
						}
						if(x==0)
						{//费率数=0改为1
							p8FnData-=7;
							p8FnData[6]=1;//费率数=1
							p8FnData+=7;
							MW((u32)p8FnData,((u32)p8FnData)+7,3);//费率1数据=总
						}
					}
					break;
				case 1://发生时间
					x=p8rxbuff[9];
					if(x>=6)
					{
						x-=6;
						x/=4;
						if(x>p8FnData[6])
						{
							break;
						}
						p8FnData+=7;
						p8rxbuff+=12;
						for(i=0;i<=x;i++)
						{
							for(Pn=0;Pn<4;Pn++)
							{
								p8FnData[3+Pn+(i*7)]=p8rxbuff[Pn];
							}
							p8rxbuff+=4;
						}
						if(x==0)
						{//费率数=0改为1
							p8FnData-=7;
							p8FnData[6]=1;//费率数=1
							p8FnData+=7;
							MW(((u32)p8FnData)+3,((u32)p8FnData)+7+3,4);//费率1数据=总
						}
					}
					break;
			}
			break;
//48 F161 电能表远程控制通断电状态及记录
		case 161:
			break;
//49 F162 电能表日历时钟
		case 162:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表日历时钟Hhmmss
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+1+6,3);
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
							i=7;
						}
						i<<=5;
						d64b=i;
						d64b<<=32;
						d64a|=d64b;
						MWR(d64a,((u32)p8FnData)+1,6);//填入抄表时间
					}
					break;
				case 1://电能表日历时钟YYMMDDWW
					MW(((u32)p8rxbuff)+12+1,((u32)p8FnData)+1+6+3,3);
					//星期
					i=p8rxbuff[12];
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
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://编程总次数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6,2);
					p8FnData[8]=0;
					break;
				case 1://最近一次编程发生时刻
					p8FnData[6+3]=0;
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+4,4);
					p8FnData[6+8]=0;
					break;
			}
			break;
			
//50 F166 电能表参数修改次数及时间
		case 166:
			
//51 F167 电能表购、用电信息
		case 167:
			
//52 F168 电能表结算信息
		case 168:
			
			break;
		
#if ERC8==1		
		case 0xE1:
//0xE1 扩展1 ERC8电能表参数变更事件用数据
			if(p8FnData[ERC8_SOUR_Pn_LEN]==0xee)
			{
				MC(0,((u32)p8FnData),10);//清前10字节第1,2套第1-8日时段表抄标志和CRC值
				p8FnData[ERC8_SOUR_Pn_LEN]=0;
			}
			i=UARTCtrl->RMCount;
			if(i<8)//抄表命令计数
			{//第1日时段表1-8
				p8FnData[0]|=1<<i;
				x=MRR(((u32)p8FnData)+1,4);//读存贮器数据到寄存器,Byte<=8
				x+=MRR(((u32)p8rxbuff)+14,3)<<i;
				MWR(x,((u32)p8FnData)+1,4);//写寄存器数据到存贮器,Byte<=8
				break;
			}
			if(i<14)//抄表命令计数
			{//第1日时段表9-14
				i-=8;
				p8FnData[5]|=1<<i;
				x=MRR(((u32)p8FnData)+6,4);//读存贮器数据到寄存器,Byte<=8
				x+=MRR(((u32)p8rxbuff)+14,3)<<i;
				MWR(x,((u32)p8FnData)+6,4);//写寄存器数据到存贮器,Byte<=8
				break;
			}
			switch(i)//抄表命令计数
			{
				case 14://电表有功常数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6,3);
					break;
				case 15://电表无功常数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6+3,3);
					break;
				case 16://每月第1结算日(无功常数有些表可能没有，为正确跳过下2项，故读结算日在此)
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10,2);
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
	#if RMM_RS485==0//RS485通道抄表模式:0=循抄全部数据,1=按大小类号设置抄读数据
				Fn0xE1_END:
	#endif
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
				case 17://最近一次编程时间(当Fn=27不抄时)
					MC(0,((u32)p8FnData)+10+6+6+6,6);
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6+6+6+1,4);
					break;
				case 18://最大需量清零次数(当Fn=27不抄时)
					MC(0,((u32)p8FnData)+10+6+6+6+6,3);
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6+6+6+6,2);
					
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
			switch(UARTCtrl->RMCount)
			{
				case 0://总断相次数
					p8FnData[6]=1;//
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+7,2);
					p8FnData[9]=0;
					
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
				case 1://编程次数(当Fn=27不抄时)
					MW(((u32)p8rxbuff)+12,((u32)p8FnData),2);
					p8FnData[2]=0;
					break;
				case 2://最大需量清零次数(当Fn=27不抄时)
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+3,2);
					p8FnData[5]=0;
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
				case 3://电表运行状态字(当Fn=28不抄时)
					i=p8rxbuff[12];
					i&=0x04;//b2=电池欠压
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
#endif
//扩展3 集中抄表日冻结电能示值（总、费率1～M）
		case 0xE3:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表日历时钟(年月日周)
					//注:不比对时标时用于冻结的时标在Terminal_AFN0D_CongealData_Fn.c冻结时会确定
					x=MRR(ADDR_TYMDHMS+3,3);
			/*
			#if (RMM_CompCongealTime!=0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					i=MRR(((u32)p8rxbuff)+12+1,3);
					if(i!=x)
					{//YMD不同
						UARTCtrl->RMCount=100;//跳不抄下面
						return;
					}
			#endif
			*/
					MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
					return;
				case 1://F161 正向有功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF161_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=x;
						p8FnData+=6+Office_AFN0DF161_LastDate_Source+6;
						p8rxbuff+=12;
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
							p8FnData-=6+Office_AFN0DF161_LastDate_Source+6;
							p8FnData[6+Office_AFN0DF161_LastDate_Source+5]=1;//费率数=1
							p8FnData+=6+Office_AFN0DF161_LastDate_Source+6;
							MW((u32)p8FnData,((u32)p8FnData)+5,5);//费率1数据=总
						}
					}
					break;
				case 2://F162 正向无功（组合无功1）电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF162_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF162_LastDate_Source+6,4*(x+1));
					}
					break;
				case 3://F163 反向有功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF163_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF163_LastDate_Source+5]=x;
						p8FnData+=6+Office_AFN0DF163_LastDate_Source+6;
						p8rxbuff+=12;
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
					break;
				case 4://F164 反向无功（组合无功2）电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF164_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF164_LastDate_Source+6,4*(x+1));
					}
					break;
				case 5://F165 1象限无功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF165_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF165_LastDate_Source+6,4*(x+1));
					}
					break;
				case 6://F166 2象限无功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF166_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF166_LastDate_Source+6,4*(x+1));
					}
					break;
				case 7://F167 3象限无功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF167_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF167_LastDate_Source+6,4*(x+1));
					}
					break;
				case 8://F168 4象限无功电能示值（总、费率1～M）
					x=p8rxbuff[9];
					if(x>=6)
					{
						p8FnData[0]=1;//有数据填入标志
						x-=6;
						x/=4;
						if(x>TMaxTariff)
						{
							x=TMaxTariff;
						}
						MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source,5);//抄表时间
						p8FnData[6+Office_AFN0DF168_LastDate_Source+5]=x;//费率数
						MW(((u32)p8rxbuff)+12,((u32)p8FnData)+6+Office_AFN0DF168_LastDate_Source+6,4*(x+1));
					}
					break;
				case 9://再次确认电能表日历时钟(年月日周)(已修改为不抄)
			#if (RMM_CompCongealTime!=0)//读电能表日冻结数据比较冻结时间:0=不比较,1=要比较
					i=MRR(((u32)p8rxbuff)+12+1,3);
					x=MRR(ADDR_TYMDHMS+3,3);
					if(i!=x)
					{//YMD不同
						p8FnData[0]=0xee;//数据有效标志
					}
			#endif
					return;
			}

#if (USER/100)==2//用户标识:山东版
//扩展 ERC10电压回路异常事件用数据
		case 0xF3:
			switch(UARTCtrl->RMCount)
			{
				case 0://总断相次数
					MW(((u32)p8rxbuff)+12,((u32)p8FnData),2);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0x0)||(Pn>ERC10MaxPn))
					{//Pn错
						UARTCtrl->RMCount+=7;//跳过下7项抄表
						break;
					}
					i=MRR(((u32)p8FnData),2);
					x=MRR(ADDR_ERC10_SOUR+((Pn-1)*LEN_ERC10_SOUR_Pn)+5,2);
					if((x==0xeeee)||(i==x))
					{
						MWR(i,ADDR_ERC10_SOUR+((Pn-1)*LEN_ERC10_SOUR_Pn)+5,2);
						UARTCtrl->RMCount+=7;//跳过下7项抄表
						break;
					}
					break;
				case 1://A相电压(当事件产生时实时读)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+10,2);
					break;
				case 2://B相电压(当事件产生时实时读)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+12,2);
					break;
				case 3://C相电压(当事件产生时实时读)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+14,2);
					break;
				case 4://A相电流(当事件产生时实时读)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+16,3);
					break;
				case 5://B相电流(当事件产生时实时读)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+19,3);
					break;
				case 6://C相电流(当事件产生时实时读)
					i=p8rxbuff[12]+(p8rxbuff[13]<<8);
					i<<=4;
					MWR(i,((u32)p8FnData)+22,3);
					break;
				case 7://当前正向有功示值(当事件产生时实时读)
					p8FnData[10+6+9]=0;//5字节电量
					MW(((u32)p8rxbuff)+12,((u32)p8FnData)+10+6+9+1,4);
					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
					if((Pn==0x0)||(Pn>ERC10MaxPn))
					{//Pn错
						break;
					}
					i=MRR(((u32)p8FnData),2);//更新成功读取的次数
					MWR(i,ADDR_ERC10_SOUR+((Pn-1)*LEN_ERC10_SOUR_Pn)+5,2);
					
					p8FnData[0]=10;//ERC
					p8FnData[1]=28;//长度Le
					MR(((u32)p8FnData)+2,ADDR_TYMDHMS+1,5);//分时日月年
					p8FnData[7]=Pn;
					p8FnData[8]=(Pn>>8)|(1<<7);
					p8FnData[9]=0x07+(1<<6);//D6～D7编码表示电压回路的异常类型，取值1～2依次表示断相、失压，值0、3为备用
					ERC_Event_Save(((u32)p8FnData));//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
					break;
			}
			break;
#endif
	}
}















