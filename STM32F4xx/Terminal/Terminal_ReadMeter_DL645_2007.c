
//终端抄表RS485口_DL645_2007
#include "Project.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "Terminal_ReadMeter_CountAndFlag.h"
#include "KeyUser.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_Fn.h"

#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_Event.h"



extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];


void Terminal_ReadMeter_DL645_2007(u32 PORTn)//终端抄表RS485口_DL645_2007
{
	u32 i;
	u32 x;
	u32 y;
#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	u32 z;
#endif
	u32 Fn;
	u32 Pn;
	u8 * p8RMLib;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	u8 * p8FnData;
	u32 PORT485;
	u32 ClassNo;
	u64 YMDHM;
#if Extern_ReadMeter_Fn_YesNo!=0//外部程序:0=I2,1=E1,按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数,Protocol(1=1997,其他=2007)
	u32 *p32;
#endif
#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
	u8 *p8;
#endif

//	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	
	
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			if(UARTCtrl->BatchTask>=1)//转发批量任务:0=空,1=等待口空闲启动,2...过程
			{
				return;
			}
/*
#if RMM_RS485_PassageZero==1//RS485通道抄表过0点;0=不暂停,1=暂停
			i=MRR(ADDR_TYMDHMS+1,2);
			if((i>=0x2355)||(i<RMM_StartHM))
			{//RS485口在每日0点附近暂停抄表
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						PORT485=1;//RS485PORT=1;
						break;
					case RS485_2PORT://RS485-2
						PORT485=2;//RS485PORT=2;
						break;
					case RS485_3PORT://RS485-3
						PORT485=3;//RS485PORT=3;
						break;
					default:
						return;
				}
				MC(0,ADDR_DATABUFF,21);
				CopyString((u8 *)"485 停抄等待时钟过0",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				MWR(PORT485,ADDR_DATABUFF+3,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
				return;
			}
#endif
*/
			if(p16timer[0]==0x0)
			{//下帧启动发送延时
				UARTCtrl->Task=3;//启动发送
			}
			break;
		case 1://1=完成1帧发收
			UARTCtrl->Task=0;//空闲1次
			break;
		case 2://2=接收超时(或错误)
			UARTCtrl->Task=0;//空闲1次
			if(UARTCtrl->ReRMCount<2)
			{
				UARTCtrl->ReRMCount+=1;//抄表命令重发计数+1
			}
			else
			{
				UARTCtrl->RMCount+=1;//抄表命令计数+1
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				Fn=MRR(DL645_2007_RMLib[UARTCtrl->FnCount],1);
				if(Fn==3)
				{
					UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
					UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
					UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
				}
				if(Fn!=0xE1)
				{
					UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
				}
				switch(Fn)
				{
					default:
						UARTCtrl->RMError|=1;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
						break;
					case 129://F129 当前正向有功电能示值
					case 28://电表运行状态字及其变位标志
					case 0xE3://扩展3 集中抄表日冻结正反有无功,4个象限无功电能示值（总、费率1～M）
						UARTCtrl->RMError|=2;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
						break;
					case 0xE1://扩展1 ERC8电能表参数变更事件用数据
						i=UARTCtrl->RMCount;
						if(i<2)//抄表命令计数
						{//第1套第1-2日时段表
							UARTCtrl->RMCount=2;
							break;
						}
						if(i<4)//抄表命令计数
						{//第2套第1-2日时段表
							UARTCtrl->RMCount=4;
						}
						return;
				#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
					case 2://读电能表主动上报事件
						//清事件标志,免死循环
						MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
						return;
				#endif
				}
			}
			break;
		case 3://3=启动发送
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
			{//本序号的电能表/交流采样装置删除
				goto END_Fn;//不抄
			}
			if(AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]>=2)//抄表任务完成标志:0=没完成,1=完成但至少有1个数据标识抄读失败,2=成功完成,3=不需抄
			{
				goto END_Fn;//已成功抄表
			}
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					PORT485=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					PORT485=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					PORT485=3;//RS485PORT=3;
					break;
				default://内部交流采样或载波口
					return;
			}
#if (((Project/100)==2)||((Project/100)==5))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			i=MRR(ADDR_AFN04F33+((14+(4*24))*PORT485)+1,1);//终端抄表运行参数设置
			if((i&0x02)!=0x0)
			{//D1置"1"要求终端只抄重点表，置"0"要求终端抄所有表
				y=MRR(ADDR_AFN04F35,1);//台区集中抄表重点户设置
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				for(i=0;i<y;i++)
				{
					if(MRR(ADDR_AFN04F35+1+(2*i),2)==UARTCtrl->MeterNo)
					{
						break;
					}
				}
				if(i==y)
				{//不是重点户
					goto END_Fn;//不抄
				}
			}
			i=MRR(ADDR_AFN04F30+(Pn-1),1);//台区集中抄表停抄/投抄设置(测量点号)
			if(i!=0)
			{//停抄
				goto END_Fn;//不抄
			}
#endif
			if(UARTCtrl->FnCount>=Get_RM_DL645_2007_MaxFn())
			{//Fn计数>=最大Fn个数,本表抄表完成
				if(UARTCtrl->RMError==0)
				{//抄表成功
				#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
				#else
					if(UARTCtrl->RMMeterEvent==0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
					{
						AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
					}
				#endif
					ReadMeterNote_OK(PORTn,Pn,0);//集中抄表电表抄读信息.成功
					DateRMFlags(Pn,0);//日抄表标志;入口:f=0成功,=1失败
				}
				else
				{//抄表失败
				#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
				#else
					if(UARTCtrl->RMMeterEvent==0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
					{
						AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
					}
				#endif
					ReadMeterNote_Fail(PORTn,Pn);//集中抄表电表抄读信息.失败
					DateRMFlags(Pn,1);//日抄表标志;入口:f=0成功,=1失败
				}
	END_Fn:
				UARTCtrl->Task=0;//0=空闲
				UARTCtrl->FnCount=0;//Fn计数=0
				UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
				UARTCtrl->RMCount=0;//抄表命令计数=0
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
				UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
				UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
				UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				UARTCtrl->RMError=0;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
				break;
			}
			if(DL645_2007_RMLib[UARTCtrl->FnCount]==0x0)
			{//本Fn无抄表
	NEXT_Fn:
				UARTCtrl->Task=1;//完成1帧发收
				UARTCtrl->FnCount+=1;//Fn计数+1
				UARTCtrl->RMCount=0;//抄表命令计数=0
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				UARTCtrl->RMError&=0xfe;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				
	#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
	#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
				if((UARTCtrl->RMMeterEvent&1)==0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
				{//0=否
					UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
					UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
					UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
					if(UARTCtrl->RMTaskC!=0)//当前抄表任务号0-3:0=上电任务,1=每日任务,2=每周任务,3=抄停电记录,(MaxRMTask-1)=循抄任务
					{//不是启抄
						i=Check_Meter_Event_Save();//检查电能表事件存贮空间;返回0=存储滿(没上报),1-255允许还可存储的条数
						if(i>=30)
						{//空间可存>=30条,每口10条
							i=Check_AutoReportWord(PORTn);//检查电能表主动上报状态字;返回:0=无,!=0 B31-B16为配置序号(1开始),B15-B0为要读取的主动上报状态字位号(0-95)
							i>>=16;
							if(i!=0)
							{//有主动上报需要读
								UARTCtrl->RMMeterEvent|=1;//正在抄读电能表事件标志;b0=主动上报事件,b1=
								UARTCtrl->MeterNoBackupOfAutoReport=UARTCtrl->MeterNo;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
								UARTCtrl->FnCountBackupOfAutoReport=UARTCtrl->FnCount;//485抄表Fn计数(原常规抄表的Fn计数寄存)
								UARTCtrl->MeterNo=i;
								UARTCtrl->FnCount=2;
							}
						}
						if((UARTCtrl->RMMeterEvent&1)==0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
						{
							p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
							if(p8RMLib[0]==2)//F2
							{
								UARTCtrl->FnCount++;//Fn计数+1;非主动事件跳过不抄
							}
						}
					}
				}
				else
				{//是
					UARTCtrl->RMMeterEvent&=0xfe;//正在抄读电能表事件标志;b0=主动上报事件,b1=
					UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485抄表电能表/交流采样装置序号(半字对齐)(原常规抄表的装置序号寄存)
					UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfAutoReport;//485抄表Fn计数(原常规抄表的Fn计数寄存)
				}
	#endif
	#endif
				break;
			}
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//本Fn不是RS485抄表
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];

			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
		#if Extern_ReadMeter_Fn_YesNo==0//外部程序:0=I2,1=E1,按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数,Protocol(1=1997,其他=2007)	
			y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,30);//按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数
		#else
			y=0;
			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				p32=(u32 *)(ADDR_E1_ENTRY);
				y=(*(u32(*)())p32[73])((u32)PORTn,(u32)ClassNo,(u32)Pn,(u8*)p8RMLib,(u32)UARTCtrl->RMCount,(u32)30);//按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数
			}
		#endif
	#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
	#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
			if(UARTCtrl->RMMeterEvent!=0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
			{
				y=1;//抄读主动上报总是允许
			}
	#endif
	#endif
			if(Fn==26)
			{
			#if RMM_AFN0CF26==0//AFN0CF26断相数据抄表;0=抄电表断相记录数据,1=抄电表失压记录数据
				if(UARTCtrl->RMCount==12)//抄表数据标识计数
			#endif
			#if RMM_AFN0CF26==1//AFN0CF26断相数据抄表;0=抄电表断相记录数据,1=抄电表失压记录数据
				if(UARTCtrl->RMCount==16)//抄表数据标识计数
			#endif
				{//抄表命令计数16以上是兼容命令,只有不成功时才抄
					for(i=0;i<(1+61);i++)
					{
						if(p8FnData[i]!=0xee)
						{
							break;
						}
					}
					if(i!=(1+61))
					{//有抄成功
						y=0;//不抄
						UARTCtrl->RMCount=100;//抄表数据标识计数
					}
				}
			}
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			if(y!=0)
			{
			//扩展14 补抄上2日日冻结数据
			//扩展15 补抄上3日日冻结数据
				if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
				{
					y=0;//没完成1个循环抄表时不抄
					x=MRR(ADDR_TYMDHMS+3,3);//取当前年月日
					x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
					if(p8RMLib[0]==0xEE)
					{
						y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
					}
					else
					{
						x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
						y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
					}
					y=MRR(y,3);//取上?日补抄时标
					if(x==y)
					{//己抄
						goto NEXT_Fn;
					}
					x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
					i=GetDateCongealNo(x);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
					if(i!=0xffffffff)
					{//找到
						z=UARTCtrl->RMCount;//抄表命令计数
						if((z>0)&&(z<9))
						{
							z+=160;//抄表命令计数1-8对应Fn=161-168
							z=GetClass2DataOfficeAddr(z,Pn);//取类2数据存贮偏移地址,返回0表示没找到
							if(z!=0)
							{//找到
								y=MRR(ADDR_TYMDHMS+3,3);//取当前年月日
								z=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+z+2,3);//取原抄表时间年月日
								if((z!=0xeeeeee)&&(z!=0xffffff))
								{
									if((z>x)&&(z<=y))
									{//原抄表时间>上2日时标,同时<=当前时间
										goto NEXT_Fn;//己抄
									}
								}
							}
						}
					}
					while(1)
					{
						i=UARTCtrl->RMCount;//抄表数据标识计数
						y=1;//抄表数据标识计数=0和9总是要抄
						if(i>9)
						{
							break;
						}
						if((i>0)&&(i<9))
						{
							i+=160;//要抄的Fn=161-168
							y=Class2Data_Fn(ClassNo,i);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持		
						}
						if(y==0)
						{
							UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
							UARTCtrl->RMCount++;//抄表数据标识计数
						}
						else
						{
							break;
						}
					}
					if(UARTCtrl->RMCount>=p8RMLib[1])
					{//抄表数据标识计数>=Fn需发命令数;本Fn抄表完成
						if(p8RMLib[1]>UARTCtrl->RMCountErr)
						{//不是全部标识全部抄读失败
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
						}
						goto NEXT_Fn;
					}
				}
			}
		#endif
			if(y!=0)
			{
				if((p8RMLib[0]==0xE3)||(p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
				{//抄上1-3日日冻结数据
				#if ERC39==1
					if(p8RMLib[0]==0xEF)
					{
						if(UARTCtrl->RMCount<=1)
						{
							ERC39_Event(Pn);//补抄失败事件记录
						}
					}
				#endif
				#if (USER/100)==4//用户标识:陕西
					if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
					{
						i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//电能费率个数
						if(i==1)
						{//抄上2-3日日冻结数据
							y=0;//陕西农网表无日冻结
						}
					}
				#endif
					if(UARTCtrl->RMCount==9)
					{//再次确认日冻结时间YYMMDDhhmm
						//省略不抄,因抄表在过0点后延时进行,在此判断不修改抄表库简化设计
						y=0;
					}
				}
			}
			if(y==0x0)
			{//本命令不抄
				if(p8RMLib[1]==0x0)
				{//本Fn抄表命令数=0
					goto NEXT_Fn;
				}
				if(UARTCtrl->RMCount==0)//抄表数据标识计数
				{//抄表数据标识计数=0,清Fn数据缓冲
					UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
					MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
				UARTCtrl->RMCount++;//抄表数据标识计数
				UARTCtrl->ReRMCount=0;//重发计数
				if(UARTCtrl->RMCount>=p8RMLib[1])
				{//抄表命令计数>=Fn需发命令数;本Fn抄表完成
				#if (RMM_RS485_FailData_0xEE==0)//RS485抄表失败数据填0xee;0=否,1=是
					if(p8RMLib[1]>UARTCtrl->RMCountErr)
					{//不是全部标识不抄
						goto OK_Fn;
					}
					goto NEXT_Fn;
				#else
					goto OK_Fn;
				#endif
				}
				return;
			}

			if(p8RMLib[1]==0x0)
			{//本Fn抄表命令数=0
			#if AFN0CF33MaxPn==0
				goto NEXT_Fn;
			#else
				if((p8RMLib[4]&0x01)!=0)//数据分类:B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				{//1类数据,实时
					switch(Fn)
					{
					//F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					//F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					//F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					//F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						case 33:
						case 34:
						case 35:
						case 36:
					//F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					//F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					//F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					//F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M�12�
						case 37:
						case 38:
						case 39:
						case 40:
							break;
						default:
							goto NEXT_Fn;
					}
					ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
					goto NEXT_Fn;
				}
			#endif
			}

			if(UARTCtrl->RMCount==0x0)
			{//抄表命令计数=0,清Fn数据缓冲
				UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//抄表命令计数>=Fn需发命令数;本Fn抄表完成
			OK_Fn:
				if(Fn==129)//F129 当前正向有功电能示值
				{
					if((UARTCtrl->RMError&0x2)==0)//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
					{
				#if (RMM_RS485_FailData_0xEE==1)//RS485抄表失败数据填0xee;0=否,1=是
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				#else
						if(p8RMLib[1]>UARTCtrl->RMCountErr)
						{//不是全部标识都没抄
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
						}
				#endif
						RM_CountAndFlags(PORTn,UARTCtrl->MeterNo,0);//电能表抄表成功失败计数和标志,入口UART缓冲地址,电能表序号,标志f=0成功,f=1失败
						#if ERC31==1
						ERC31_Event(UARTCtrl->MeterNo,Pn,0);//终端485抄表失败事件,入口测量点号,f=1产生=0恢复
						#endif
						MC(0,ADDR_ERC21_Flag,1);//清ERC21 485抄表故障事件记录标志
					}
					else
					{
						RM_CountAndFlags(PORTn,UARTCtrl->MeterNo,1);//电能表抄表成功失败计数和标志,入口UART缓冲地址,电能表序号,标志f=0成功,f=1失败
						#if ERC31==1
						ERC31_Event(UARTCtrl->MeterNo,Pn,1);//终端485抄表失败事件,入口测量点号,f=1产生=0恢复
						#endif
						#if ERC21==1
						ERC21_Event();//ERC21 485抄表故障事件
						#endif
				#if (RMM_RS485_FailData_0xEE==1)//RS485抄表失败数据填0xee;0=否,1=是
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
				#else
						ClrERC29SOUR(Pn);//集中抄表失败清ERC29电能表飞走事件原始数据
				#endif
					}
					goto NEXT_Fn;
				}
		#if (RMM_RS485_FailData_0xEE==1)//RS485抄表失败数据填0xee;0=否,1=是
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
		#else
				if(UARTCtrl->RMError==0)//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				{
					if(p8RMLib[1]>UARTCtrl->RMCountErr)
					{//不是全部标识都没抄
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
					}
				}
		#endif
				goto NEXT_Fn;
			}
			if(UARTCtrl->RMError!=0x0)
			{//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
		#if (RMM_RS485_FailData_0xEE==1)//RS485抄表失败数据填0xee;0=否,1=是
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
		#endif
				//不是转到NEXT_Fn免下个Fn不抄表反而置抄表为成功标志
			#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
				AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
			#else
				if(UARTCtrl->RMMeterEvent==0)//正在抄读电能表事件标志;b0=主动上报事件,b1=
				{
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
				}
			#endif
				ReadMeterNote_Fail(PORTn,Pn);//集中抄表电表抄读信息.失败
				DateRMFlags(Pn,1);//日抄表标志;入口:f=0成功,=1失败
				goto END_Fn;//不是转到NEXT_Fn免下个Fn不抄表反而置抄表为成功标志
			}

			//扩展13 上1-24整点电表冻结的正反有功电能示值
			if(p8RMLib[0]==0xED)
			{
				//Fn101正有功电能示值曲线配置
				y=Class2Data_Fn(ClassNo,101);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				//Fn103反有功电能示值曲线配置
				y|=Class2Data_Fn(ClassNo,103);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				if(y==0)
				{
					goto NEXT_Fn;
				}
				if(UARTCtrl->RMCount==0)
				{//仅当命令计数=0时列表,减少运行时间
					KeyUserPnList();//集中抄表重点户曲线冻结Pn列表
				}
				i=MRR(ADDR_TYMDHMS+2,4);//取当前年月日时
				YMDHM=YMDH_SubH(i,UARTCtrl->RMCount);//年月日时减h时,返回减h时后的年月日时
				YMDHM<<=8;//分=0
				i=CheckCurveCongeal(YMDHM,101,Pn);//检查曲线冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
				if(i!=0)
				{//已冻结或没找到冻结号
					UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
					UARTCtrl->RMCount++;//抄表数据标识计数+1
					UARTCtrl->ReRMCount=0;//重发计数
					return;
				}
			}
	#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
		//EventProject==0
			Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//终端DL645-2007抄表发送数据
		#if (USER/100)==4//用户标识:陕西
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//电能费率个数
			if((i==1)&&(p8RMLib[0]==0xE3))
			{//变换日冻结数据标识
				switch(UARTCtrl->RMCount)
				{
					default://日冻结时间YYMMDDhhmm
						UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
						UARTCtrl->RMCount++;//抄表数据标识计数+1
						UARTCtrl->ReRMCount=0;//重发计数
						return;
					case 1://上1次日正向有功电能示值（总、费率1～M）
						i=0x00010000;//当前正向有功电能示值（总、费率1～M）
						break;
				}
				MWR(i,((u32)p8txbuff)+4+10,4);
				//DI加0x33
				for(i=0;i<4;i++)
				{
					p8txbuff[4+10+i]+=0x33;
				}
				//cs
				p8txbuff[4+14]=0;
				for(i=0;i<14;i++)
				{
					p8txbuff[4+14]+=p8txbuff[4+i];
				}
			}
		#endif
			Fn=p8RMLib[0];
			if((Fn>=0xe4)&&(Fn<=0xe9))
			{
			#if (((Project/100)==2)&&((USER/100)==5))//上海系集中器
				//补当前到当日0点曲线点
				YMDHM=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
				YMDHM&=0xffffffff00;
				i=UARTCtrl->RMCount*60;//每60分间隔
				YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
				i=Fn;
				i-=0xe4;
				i<<=16;
				i+=0x06000001;
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//终端DL645-2007抄表发送负荷曲线数据
			#else
				//补当前到上日0点曲线点
				YMDHM=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
				YMDHM=YMDHM_Int15(YMDHM);//年月日时分化为整15分,如00时18分化为00时15分
				i=UARTCtrl->RMCount*15;//每15分间隔
				YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
				i=Fn;
				i-=0xe4;
				i<<=16;
				i+=0x06000001;
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//终端DL645-2007抄表发送负荷曲线数据
			#endif
			}
	#else//#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
		//EventProject==1
			if(UARTCtrl->SubsequentFrames==0)//0=无读后续帧,1-255=DL645读续帧的帧号
			{//非后续帧
				Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//终端DL645-2007抄表发送数据
			#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
				if(p8RMLib[0]==2)//F2 电能表主动上报事件
				{
					switch(UARTCtrl->RMCount)
					{
						case 0:
						case 1://读主动上报状态字(仅当鼎信模块时)
							UARTCtrl->RMCount++;
							return;
						case (2+(96*2))://复位主动上报状态字
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							MWR(YMDHM,((u32)p8txbuff)+4+1,6);
							p8txbuff[4+8]=0x14;
							p8txbuff[4+9]=24;
							MWR(0x04001503,((u32)p8txbuff)+4+10,4);
							MC(0,((u32)p8txbuff)+4+14,8);//密码,操作者代码
							MR(((u32)p8txbuff)+4+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//复位主动上报状态字的位标志
							for(i=0;i<12;i++)
							{//取反标志位
								x=p8txbuff[4+22+i];
								x=~x;
								p8txbuff[4+22+i]=x;
							}
							DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
							UARTCtrl->TxByte=4+36;//发送字节数
							break;
						case (2+(96*2)+1)://读主动上报状态字
							break;
						default://2-97读次数和事件
							//取DI	
							x=(u32)p8RMLib;
							x+=10+(5*UARTCtrl->RMCount);
							x=MRR(x,4);
							if(x==0)
							{//保留
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-2)/2),1);
								UARTCtrl->RMCount++;
								if(UARTCtrl->RMCount&1)
								{
									UARTCtrl->RMCount++;
								}
								UARTCtrl->ReRMCount=0;
								return;
							}
							i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-2)/2),1);
							if(i==0)
							{//不需读
								UARTCtrl->RMCount++;
								if(UARTCtrl->RMCount&1)
								{
									UARTCtrl->RMCount++;
								}
								UARTCtrl->ReRMCount=0;
								return;
							}
							if(UARTCtrl->RMCount&1)
							{//读1-10次事件
								if(i>10)
								{
									i=10;
								}
								i--;
								x+=i;
								YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
								i=Get_ADDR_UARTnTx(PORTn);
								Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
							}
							MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//第1帧标志
							break;
					}
				}
			#endif//#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
				if(p8RMLib[0]==3)//F3 周期抄读电能表事件
				{
				#if ((USER/100)==5)//上海用户
					switch(UARTCtrl->RMCount)
					{
					#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
						case ((60)*3)://读主动上报状态字
							break;
						case ((60)*3+1)://复位主动上报状态字
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							MWR(YMDHM,((u32)p8txbuff)+4+1,6);
							p8txbuff[4+8]=0x14;
							p8txbuff[4+9]=24;
							MWR(0x04001503,((u32)p8txbuff)+4+10,4);
							MC(0,((u32)p8txbuff)+4+14,8);//密码,操作者代码
							MR(((u32)p8txbuff)+4+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//复位主动上报状态字的位标志

							for(i=0;i<12;i++)
							{
								p8txbuff[4+22+i]=0xff;
							}
							//取反标志位bit5、bit7
							x=p8txbuff[4+22];
							x=(~(x&0xa0));
							p8txbuff[4+22]=x;
							DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
							UARTCtrl->TxByte=4+36;//发送字节数
							break;
						case ((60)*3+3)://读电表运行状态字
							break;
					#endif
						default:
							i=UARTCtrl->RMCount;
							if((i%3)!=0)
							{//读1-10次事件
								i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/3))+3,1);//新增次数(初始化值0xff)
								if(i==0xff)
								{
									i=0;
								}
								if(i==0)
								{//不需读
									UARTCtrl->RMCount++;
									UARTCtrl->ReRMCount=0;
									return;
								}
								if(i>10)
								{
									i=10;
								}
								i--;
								//取DI	
								x=(u32)p8RMLib;
								x+=10+(5*UARTCtrl->RMCount);
								x=MRR(x,4);
								x+=i;
								YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
								i=Get_ADDR_UARTnTx(PORTn);
								Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn)+((UARTCtrl->RMCount%3)-1)*2,2);//第1帧标志
							}
							break;
					}
				#else//#if ((USER/100)==5)//上海用户
				//!!!2015-7-29周期事件抄读修改
					if(UARTCtrl->RMDI==0xffffffff)//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
					{//UARTCtrl->RMDI无效
						i=UARTCtrl->RMCount;
						if((i&1)!=0)
						{//读1-10次事件
							i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//新增次数(初始化值0xff)
							if(i==0xff)
							{
								i=0;
							}
							if(i==0)
							{//不需读
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;
								return;
							}
							if(i>10)
							{
								i=10;
							}
							i--;
							//取DI	
							x=(u32)p8RMLib;
							x+=10+(5*UARTCtrl->RMCount);
							x=MRR(x,4);
							x+=i;
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
							MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//第1帧标志
						}
					}
					else
					{//UARTCtrl->RMDI有效
						i=UARTCtrl->RMCount;
						if((i&1)==0)
						{//读次数(非事件类数据)
							x=UARTCtrl->RMDI;//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
							if(UARTCtrl->RMDICount==0)//周期事件抄读已抄DI计数
							{
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//第1帧标志
							}
						}
						else
						{//读1-10次事件,只指定的数据标识
							i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//新增次数(初始化值0xff)
							if(i==0xff)
							{
								i=0;
							}
							if(i==0)
							{//不需读
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;
								return;
							}
							if(i>10)
							{
								i=10;
							}
							i--;
							//取DI	
							x=UARTCtrl->RMDI;
							x&=0xffffff00;
							x|=0x01;
							x+=i;
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
							if(UARTCtrl->RMDICount==0)//周期事件抄读已抄DI计数
							{
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//第1帧标志
							}
						}
					}
				#endif
				}
			#if (USER/100)==4//用户标识:陕西
				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//电能费率个数
				if((i==1)&&(p8RMLib[0]==0xE3))
				{//变换日冻结数据标识
					switch(UARTCtrl->RMCount)
					{
						default://日冻结时间YYMMDDhhmm
							UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
							UARTCtrl->RMCount++;//抄表数据标识计数+1
							UARTCtrl->ReRMCount=0;//重发计数
							return;
						case 1://上1次日正向有功电能示值（总、费率1～M）
							i=0x00010000;//当前正向有功电能示值（总、费率1～M）
							break;
					}
					MWR(i,((u32)p8txbuff)+4+10,4);
					//加0x33
					for(i=0;i<4;i++)
					{
						p8txbuff[4+10+i]+=0x33;
					}
					//cs
					p8txbuff[4+14]=0;
					for(i=0;i<14;i++)
					{
						p8txbuff[4+14]+=p8txbuff[4+i];
					}
				}
			#endif
				Fn=p8RMLib[0];
				if((Fn>=0xe4)&&(Fn<=0xe9))
				{
				#if (((Project/100)==2)&&((USER/100)==5))//上海系集中器
					//补当前到当日0点曲线点
					YMDHM=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
					YMDHM&=0xffffffff00;
					i=UARTCtrl->RMCount*60;//每60分间隔
					YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
					i=Fn;
					i-=0xe4;
					i<<=16;
					i+=0x06000001;
					PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//终端DL645-2007抄表发送负荷曲线数据
				#else
					//补当前到上日0点曲线点
					YMDHM=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
					YMDHM=YMDHM_Int15(YMDHM);//年月日时分化为整15分,如00时18分化为00时15分
					i=UARTCtrl->RMCount*15;//每15分间隔
					YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
					i=Fn;
					i-=0xe4;
					i<<=16;
					i+=0x06000001;
					PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//终端DL645-2007抄表发送负荷曲线数据
				#endif
				}
			}
			else
			{//是后续帧
				p8txbuff[4+8]=0x12;
				p8txbuff[4+9]=5;
				p8txbuff[4+14]=UARTCtrl->SubsequentFrames+0x33;
				p8txbuff[4+15]=0;//cs
				for(i=0;i<15;i++)
				{
					p8txbuff[4+15]+=p8txbuff[4+i];
				}
				p8txbuff[4+16]=0x16;
				UARTCtrl->TxByte=4+17;//发送字节数
			}
	#endif//#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
	
			//通信速率
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//默认
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}
			//启动
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
			//显示抄表地址
			switch(UARTCtrl->RMTaskC)
			{
				case 0://上电
					CopyString((u8 *)"485 启抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 1://日抄
					if(UARTCtrl->DateReRMCount<=1)//每日任务(包括补抄任务)的执行总次数;0=空(上电或日更新),1-255=总次数计数
					{
						CopyString((u8 *)"485 日抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					}
					else
					{
						CopyString((u8 *)"485 补抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					}
					break;
				case 2://周抄
					CopyString((u8 *)"485 时抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					//CopyString((u8 *)"485 周抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 3://定抄
					CopyString((u8 *)"485 定抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case (MaxRMTask-2)://周期抄读电能表事件
					switch(UARTCtrl->RMEventTaskC)//当前抄读电能表事件任务:0=空,1-3=1-3级
					{
						case 1:
							CopyString((u8 *)"485  1级",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 2:
							CopyString((u8 *)"485  2级",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 3:
							CopyString((u8 *)"485  3级",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
					}
					break;
				default://循抄
					CopyString((u8 *)"485 循抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
			}
		#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			if(UARTCtrl->RMMeterEvent&1)//正在抄读电能表事件标志;b0=主动上报事件,b1=
			{
				CopyString((u8 *)"485 上报",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			}
		#endif
			MWR(PORT485,ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8txbuff[10-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case 4://4=正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				UARTCtrl->Task=5;
				p16timer[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
			}
			break;
		case 5://5=正在接收
			i=DL645_Rx(PORTn);//DL645帧接收;返回0没完成,1完成,2超时
			switch(i)
			{
				case 0://0没完成
					return;
				case 1://1完成
					break;
				default://2超时
					UARTCtrl->Task=2;//2=接收超时(或错误)
					return;
			}	
			
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
			p16timer[0]=30/10;//下帧启动发送延时20MS后
			
	#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			if(UARTCtrl->SubsequentFrames!=0)//0=无读后续帧,1-255=DL645读续帧的帧号
			{//是后续帧
				if((p8RMLib[0]<2)||(p8RMLib[0]>3))
				{//不是读主动上报事件和周期读电能表事件
					//减0x33
					for(i=0;i<p8rxbuff[9];i++)
					{
						p8rxbuff[10+i]-=0x33;
					}
					i=MRR(((u32)p8rxbuff)+10,4);//返回的数据标识
					if(i==0x04001501)
					{//是收到主动上报帧
						if((UARTCtrl->RMCount+1)>=p8RMLib[1])//抄表命令计数
						{//只记1次,否则当主动上报无事件计数时每Fn中有多个抄表命令时会记多次.
							WriteAutoReportWord(PORTn,((u32)p8rxbuff));//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
						}
						UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
						UARTCtrl->Task=0;//空闲1次
						UARTCtrl->ReRMCount=0;//抄表命令重发计数
						return;
					}
					if(p8rxbuff[8]&0x20)
					{//有后续帧
						UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
					}
					else
					{//无后续帧
						UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
					}
					UARTCtrl->Task=0;//空闲1次
					UARTCtrl->ReRMCount=0;//抄表命令重发计数
					return;
				}
			}
			if(p8rxbuff[8]&0x20)
			{//有后续帧
			#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
				UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
			#else
				if((p8RMLib[0]==2)||(p8RMLib[0]==3))
				{//载波只有主动抄事件或周期读电能表事件时才允许后续帧
					UARTCtrl->SubsequentFrames++;//0=无读后续帧,1-255=DL645读续帧的帧号
				}
			#endif
			}
			else
			{//无后续帧
				UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
			}
	#endif
			//比较发收地址、控制码、数据标识、减0x33
			i=DL645_2007_RxData(PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错

	#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			if(i==3)
			{//3数据标识错
				//错误时没减0x33
				for(x=0;x<p8rxbuff[9];x++)
				{
					p8rxbuff[10+x]-=0x33;
				}
				x=MRR(((u32)p8rxbuff)+10,4);
				if((p8RMLib[0]==2)&&((UARTCtrl->RMCount==(2+(96*2)))||(x==0x04001501)))
				{//F2 电能表主动上报事件的复位主动上报状态字回答或后续帧
					i=0;//正确
				}
				if((p8RMLib[0]==3)&&(x==0x04001501))
				{//F3 周期读电能表事件的主动上报状态字后续帧
					i=0;//正确
				}
			#if ((USER/100)==5)//上海用户
			#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
				if((p8RMLib[0]==3)&&(UARTCtrl->RMCount==((60)*3)+1))
				{//F3 周期读电能表事件的复位主动上报状态字回答
					i=0;//正确
				}
			#endif
			#endif
			}
	#endif
			if(i!=0)
			{
				if(i==2)
				{//从站异常回答
					UARTCtrl->Task=1;//完成1帧发收
					UARTCtrl->RMCount+=1;//抄表命令计数+1
					UARTCtrl->ReRMCount=0;//抄表命令重发计数
					UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
					break;
				}
				//地址或数据标识错,电科院测试软件模拟表会停止但接收数据正常,恢复模拟表后会将收到的帧同时发出
				//继续接收
				p16timer[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=正在接收
				break;
			}
			//返回长度
			if(p8rxbuff[9]==4)
			{//电科院测试软件当事件记录无数据时,只返回4字节数据标识,同从站异常回答
				UARTCtrl->Task=1;//完成1帧发收
				UARTCtrl->RMCount+=1;//抄表命令计数+1
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填入
				break;
			}
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			Fn=p8RMLib[0];
			if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
			{//不是读电表曲线
				p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
			}
			else
			{//是读电表曲线
				p8RMLib+=10+4;
			}
	#if EventProject==0//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
			if(p8RMLib[0]!=0x0)
	#else
			if((p8RMLib[0]!=0x0)&&(p8rxbuff[8]!=0x92)&&(p8rxbuff[8]!=0xB2))
			//应返回长度不为0 同时 不是后续帧
	#endif
			{
			//!!!2015-7-29周期事件抄读修改
				if((Fn==3)&&(UARTCtrl->RMDI!=0xffffffff))
				{
					goto SKIP;//周期事件抄读非库中命令时无返回长度可比较
				}
				if(p8rxbuff[9]!=(p8RMLib[0]+4))
				{
				#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
					if(p8RMLib[0]==2)
					{//F2 电能表主动上报事件
						if(p8rxbuff[9]>=4)
						{
							goto SKIP;//不比较长度
						}
					}
				#endif
					if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
					{//块命令后可能有0xaa
						UARTCtrl->Task=0;//空闲1次
						UARTCtrl->RMCount+=1;//抄表命令计数+1
						UARTCtrl->ReRMCount=0;//抄表命令重发计数
						UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数,作为失败免全没抄时空数据填�
						break;
					}
				}
			}
SKIP:
			UARTCtrl->Task=1;//完成1帧发收
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲
			
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			Fn=p8RMLib[0];
			//读电表1-6类负荷曲线
			if((Fn>=0xe4)&&(Fn<=0xea))
			{
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				UARTCtrl->RMCountErr=255;//Fn内有抄读失败或不抄的数据标识计数=最大值免最次存储数据
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			//扩展13 上1-24整点电表冻结的正反有功电能示值
			if(Fn==0xED)
			{
				YMDHM=MRR(((u32)p8FnData)+6+1,4);//取电表返回的冻结时标年月日时
				YMDHM<<=8;
				AddCurveCongeal(YMDHM,101,Pn,((u32)p8FnData)+6+1+4,4);//补曲线冻结点数据
				AddCurveCongeal(YMDHM,103,Pn,((u32)p8FnData)+6+1+4+4,4);//补曲线冻结点数据
			#if (USER/100)==11//吉林系
				//吉林台体测试有整半点冻结数据
				YMDHM=YMDHM_SubM(YMDHM,30);//年月日时分减m分,返回减m分后的年月日时分
				AddCurveCongeal(YMDHM,101,Pn,((u32)p8FnData)+6+1+4,4);//补曲线冻结点数据
			#endif
			}
#if (USER/100)==11//吉林系
			if(Fn==0xFB)
			{
				YMDHM=MRR(((u32)p8FnData)+6+1,4);//取电表的冻结时标年月日时
				YMDHM<<=8;
				AddCurveCongeal(YMDHM,89,Pn,((u32)p8FnData)+6+1+4,2);//补曲线冻结点数据
				AddCurveCongeal(YMDHM+0x15,89,Pn,((u32)p8FnData)+6+1+4+2,2);//补曲线冻结点数据
				AddCurveCongeal(YMDHM+0x30,89,Pn,((u32)p8FnData)+6+1+4+2+2,2);//补曲线冻结点数据
				AddCurveCongeal(YMDHM+0x45,89,Pn,((u32)p8FnData)+6+1+4+2+2+2,2);//补曲线冻结点数据
			}
			if(Fn==0xFA)
			{
				if(UARTCtrl->RMCount==2)
				{
					i=MRR(ADDR_TYMDHMS+4,2);//取当前时间年月
					i=YM_Sub1M(i);//年月减1月,返回减1后的年月
					AddMonthCongeal(i,35,Pn,((u32)p8FnData)+6+1,93);//补月冻结点数据
				}
			}
#endif
	#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
			if((Fn==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))
			{
		 		MR(ADDR_DATABUFF,((u32)p8FnData)+1+6,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				p8=(u8 *)(ADDR_DATABUFF);
				if(p8[0]!=0xee)
				{
					p8[4]&=0x1f;//去加在月字节上的星期
					p8[10]&=0x1f;//去加在月字节上的星期
					i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//年月日时分秒合法检查,返回0合法,1非法
					if(i==0)
					{//电能表时钟合法
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								if((i&0x02)==0)
								{
									i|=0x02;
								}
								break;
							case RS485_2PORT://RS485-2
								if((i&0x04)==0)
								{
									i|=0x04;
								}
								break;
						}
						MWR(i,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
						if((i>>31)!=0x0)
						{//负
							i=~i;
							i++;
						}
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								x=0;
								break;
							case RS485_2PORT://RS485-2
								x=1;
								break;
						}
						x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
						if(x<4)
						{
							MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
							MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//电能表对时前的误差
							MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//电能表对时后的误差
							MWR(2,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//电能表对时后的结果
						}
						if(x==4)
						{
							x=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//电能表对时前的误差
							if(x>MRR(ADDR_AFN04F298_Ext+7,1))
							{
								MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
								MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//电能表对时后的误差
								if(i<=MRR(ADDR_AFN04F298_Ext+7,1))
								{//成功
									MWR(1,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//电能表对时后的结果
								}
								else
								{//失败
									MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//电能表对时后的结果
								}
								//ERC51
								p8=(u8 *)ADDR_DATABUFF;
								//电能表对时前的误差
								MWR(x,ADDR_DATABUFF+9,2);
								//电能表对时后的误差
								MWR(i,ADDR_DATABUFF+11,2);
								p8[13]=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);
								ERC51_Event(Pn+(1<<15),ADDR_DATABUFF);//校时记录事件;入口:Pn测量点号(从0开始)
							#if (USER/100)==8//用户标识：通用
								p8=(u8 *)ADDR_TYMDHMS;
								MR((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1),3);
								x=DataComp((u32)ADDR_DATABUFF,(u32)ADDR_TYMDHMS+3,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
								if(x==0)
								{
									x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);
									if(x==0xffffff)
									{
										MWR(i,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);//寄存器直接写到存储器(<=8字节)(无擦除)
									}
								}
								//时间超差 5min 测量点数量
								if(i>300)
								{
									x=MRR(ADDR_AFN0DF321_Ext_Source,2);
									x++;
									MWR(x,ADDR_AFN0DF321_Ext_Source,2);
								}
								//超过时间阈值未超差5min测量点数量
								if((i>MRR(ADDR_AFN04F298_Ext+7,1))&&(i<300))
								{
									x=MRR(ADDR_AFN0DF321_Ext_Source+2,2);
									x++;
									MWR(x,ADDR_AFN0DF321_Ext_Source+2,2);
								}
							#endif
							}
							x=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),1);//电能表对时次数
							if(x<31)
							{
								x++;
							}
							MWR(x,ADDR_DATABUFF,1);
							MR(ADDR_DATABUFF+1,ADDR_AFN0CF250_Ext1+10*(Pn-1),10);
							if(x>=31)
							{
								x=30;
							}
							MR(ADDR_DATABUFF+11,ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1)+1,10*x);
							MR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),ADDR_DATABUFF,1+10*x+10);
						}
					}
				}
			}
	#endif
			UARTCtrl->RMCount+=1;//抄表命令计数+1
			UARTCtrl->ReRMCount=0;//抄表命令重发计数=0
			break;
	}
}


void Terminal_ReadMeter_DL645_2007_Fn(u32 PORTn,u8 *p8RMLib)//仅抄1个Fn>=129数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
{
	u32 i;
	u32 x;
	u32 Pn;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	u64 YMDHM;
	
//	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			break;
		case 1://1=完成1帧发收
			UARTCtrl->Task=3;
			break;
		case 2://2=接收超时(或错误)
			if(UARTCtrl->ReRMCount<1)
			{
				UARTCtrl->ReRMCount+=1;//抄表命令重发计数+1
			}
			else
			{
				UARTCtrl->RMCount+=1;//抄表命令计数+1
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
			}
			UARTCtrl->Task=3;
			break;
		case 3://3=启动发送
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//本序号的电能表/交流采样装置删除
			END_Fn:
				UARTCtrl->Task=0;//空闲
				UARTCtrl->RMCount=0;//抄表命令计数=0
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				break;
			}
			if((p8RMLib[3]&0x2)==0x0)
			{//本Fn不是RS485抄表
				goto END_Fn;
			}
//			Fn=p8RMLib[0];

			if(p8RMLib[1]==0x0)
			{//本Fn抄表命令数=0
				goto END_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//抄表命令计数=0,清Fn数据缓冲
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//抄表命令计数>=Fn需发命令数;本Fn抄表完成
				if(p8RMLib[0]==28)
				{//F28 电表运行状态字及其变位标志
					ReadMeter_SaveFnData(PORTn,p8RMLib[0],Pn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				}
				goto END_Fn;
			}
			
			//读数据帧
			if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
			{//不是读电表曲线
				Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//终端DL645-2007抄表发送数据
			}
			else
			{//读电表曲线
				i=MRR(((u32)p8RMLib)+10,4);
				YMDHM=MRR((u32)&UARTCtrl->RMCommandYMDHM[0],5);//曲线的时标年月日时分
			#if (((Project/100)==2)&&((USER/100)==5))//上海系集中器
				YMDHM&=0xffffffff00;
			#endif
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//终端DL645-2007抄表发送负荷曲线数据
			}
			
			//通信速率
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//默认2400
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}
			//启动
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
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					Pn=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					Pn=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					Pn=3;//RS485PORT=3;
					break;
				default:
					Pn=0;//RS485PORT=0;
					break;
			}
			//显示抄表地址
			p8rxbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			CopyString((u8 *)"485 实抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			MWR(Pn,ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8rxbuff[10-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case 4://4=正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				UARTCtrl->Task=5;
				p16timer[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
			}
			break;
		case 5://5=正在接收
			i=DL645_Rx(PORTn);//DL645帧接收;返回0没完成,1完成,2超时
			switch(i)
			{
				case 0://0没完成
					return;
				case 1://1完成
					break;
				default://2超时
					UARTCtrl->Task=2;//2=接收超时(或错误)
					return;
			}	
			
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
			p16timer[0]=30/10;//下帧启动发送延时20MS后

			//比较发收地址、控制码、数据标识、减0x33
			i=DL645_2007_RxData(PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
			if(i!=0)
			{
				if(i==2)
				{//从站异常回答
					UARTCtrl->Task=1;//完成1帧发收
					UARTCtrl->RMCount+=1;//抄表命令计数+1
					UARTCtrl->ReRMCount=0;//抄表命令重发计数
					break;
				}
				//地址或数据标识错,电科院测试软件模拟表会停止但接收数据正常,恢复模拟表后会将收到的帧同时发出
				//继续接收
				p16timer[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=正在接收
				break;
			}
	
			//返回长度
			p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
			if(p8RMLib[0]!=0x0)
			{
				if(p8rxbuff[9]!=(p8RMLib[0]+4))
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
					{//块命令后可能有0xaa
						if(p8rxbuff[9]==4)
						{//电科院测试软件当事件记录无数据时,只返回4字节数据标识,同从站异常回答
							UARTCtrl->Task=1;//完成1帧发收
							UARTCtrl->RMCount+=1;//抄表命令计数+1
							UARTCtrl->ReRMCount=0;//抄表命令重发计数
						}
						else
						{
							UARTCtrl->Task=2;//2=接收超时(或错误)
						}
						break;
					}
				}
			}
			p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
			UARTCtrl->Task=1;//完成1帧发收
			
			ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007抄表接收数据填入Fn数据缓冲
			UARTCtrl->RMCount+=1;//抄表命令计数+1
			UARTCtrl->ReRMCount=0;//抄表命令重发计数=0
			break;
	}
}











