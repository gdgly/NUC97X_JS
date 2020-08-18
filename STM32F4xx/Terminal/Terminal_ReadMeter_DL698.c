//终端抄表RS485口_上海规约
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"

#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Terminal/DataConversion_SH698.h"

#include "Terminal_AFN0E_Event.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "Terminal_ReadMeter_CountAndFlag.h"
#include "KeyUser.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_Fn.h"

#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_ReadMeter_DL698_FnData.h"
#include "Terminal_ReadMeter_DL698_Lib.h"
#include "../MS/MS.h"


#if (USER/100)==5//不是上海

#define		CTRL_OFFSET			3		//控制字在报文里的偏移量
#define		AF_OFFSET			4		//地址标识在报文里的偏移
#define		SA_OFFSET			5		//服务器地址在报文里的偏移
#define		SA_LEN(af)		(((af) & 0x0f) + 1)		//SA的长度
#define		APDU_OFFSET(data)	(8 + SA_LEN(data[AF_OFFSET]))					//APDU首字节在报文里的偏移


extern const u32 DL698_RMLib[];


u32 DL698_Rx(u32 PORTn)//698规约帧接收;返回0没完成,1完成,2超时
{

	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	//u32 sa_len;
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
			if(RxByte>5)
			{
				if(RxByte==((p8rxbuff[4]&0X0F)+1+8))//帧头接收完了，可以校验HCS
				{
					Pn=Get_16bitfcs_code(p8rxbuff+1, RxByte-3)^0xffff;
					if(Pn!=(p8rxbuff[RxByte-2]+(p8rxbuff[RxByte-1]<<8)))
					{//HCS校验错误
						RxByte=0;
						continue;					
					}
				}
				if(RxByte==(p8rxbuff[1]+((p8rxbuff[2]&0x3f)<<8)+2))//整帧接收完了，可以校验FCS
				{
					if(i==0x16)//结束位必须是0X16
					{
						Pn=Get_16bitfcs_code(p8rxbuff+1,RxByte-4)^0xffff;
						if(Pn==(p8rxbuff[RxByte-3]+(p8rxbuff[RxByte-2]<<8)))
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
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//超时
		return 2;//2=接收超时
	}
	return 0;
}


u32 DL698_RxData(u32 PORTn)//698规约帧接收数据,比较发收地址、命令码;返回0正确,1地址错,2命令码错
{
	u32 i;
	u32 sa_len;
	u32 apdu_offset;
	u32 split_frame;//分帧格式
	u32 fragment_len;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u32 block_no;//分帧当前帧序号
	// u32 last_block_no=0xffff;//分帧上一帧帧序号

	u8* p_save_frame;//保存分帧最后一帧之前的帧的apdu
	// u32 save_fame_len=0;//保存分帧的帧长度
	ms_Type* ms;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	ms=Get_ms();
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	//通信地址比较
	//CA不用比较
	sa_len=SA_LEN(p8rxbuff[4]);//,(p8rxbuff[4]&0x0f)+1;
	for(i=0;i<sa_len;i++)
	{
		if(p8rxbuff[5+i]!=p8txbuff[5+i])
		{
			break;
		}
	}
	if(i!=sa_len)
	{
		return 1;//返回0正确,1地址错,2命令码错
	}

	apdu_offset=APDU_OFFSET(p8rxbuff);//apdu在帧里的首字节
	if((p8rxbuff[3]&0x20)!=0)//有分帧
	{
		split_frame=MRR(((u32)p8rxbuff)+apdu_offset,2);//分帧格式
		block_no=split_frame&0xfff;//当前帧序号
		fragment_len=UARTCtrl->RxByte-13-SA_LEN(p8rxbuff[4]);//APDU除去分帧格式之后的长度
		if(UARTCtrl->Last_Block_No==block_no)//连续两帧同一序号，是错误的
		{
			if(UARTCtrl->Addr_Split_Frame!=0)
				ms->ms_free((u8*)UARTCtrl->Addr_Split_Frame);//释放我的内存堆分配
			UARTCtrl->Addr_Split_Frame=0;
			UARTCtrl->Save_Split_Frame_len=0;
			UARTCtrl->Last_Block_No=0xffffffff;
			return 1;
		}
		else
		{
			if(UARTCtrl->Last_Block_No!=0xffffffff)//已经不是第一帧了,需要判断序号
			{
				if(block_no!=UARTCtrl->Last_Block_No+1)//前后帧次序不对，错误
				{
					if(UARTCtrl->Addr_Split_Frame!=0)
						ms->ms_free((u8*)UARTCtrl->Addr_Split_Frame);//释放我的内存堆分配
					UARTCtrl->Addr_Split_Frame=0;
					UARTCtrl->Save_Split_Frame_len=0;
					UARTCtrl->Last_Block_No=0xffffffff;
					return 1;
				}
			}
			//到这就说明这帧是正确的分帧的其中一帧，把帧里的APDU部分拷贝到p_save_frame里
			UARTCtrl->Last_Block_No=block_no;//备份帧序号
			if(UARTCtrl->Addr_Split_Frame==0)//还没分配个空间
			{
				p_save_frame=ms->ms_malloc(8*1024);//动态分配我的内存堆
				UARTCtrl->Addr_Split_Frame=(u32)p_save_frame;
				if(UARTCtrl->Addr_Split_Frame==0)//分配空间失败
				{	
					return 1;
				}
				UARTCtrl->Save_Split_Frame_len=0;
			}
			if(block_no==0)//第一帧时，备份帧头
			{
				MR(UARTCtrl->Addr_Split_Frame+UARTCtrl->Save_Split_Frame_len,(u32)p8rxbuff,14);
				UARTCtrl->Save_Split_Frame_len+=14;
			}
			//备份分帧的APDU部分
			MR(UARTCtrl->Addr_Split_Frame+UARTCtrl->Save_Split_Frame_len,((u32)p8rxbuff)+apdu_offset+2,fragment_len);
			UARTCtrl->Save_Split_Frame_len+=fragment_len;

			if((split_frame&0xc000)==0x4000)//分帧最后一帧了
			{
				MR((u32)p8rxbuff,UARTCtrl->Addr_Split_Frame,UARTCtrl->Save_Split_Frame_len);
				if(UARTCtrl->Addr_Split_Frame!=0)
					ms->ms_free((u8*)UARTCtrl->Addr_Split_Frame);//释放我的内存堆分配
				UARTCtrl->Addr_Split_Frame=0;
				UARTCtrl->Last_Block_No=0xffffffff;
				UARTCtrl->Save_Split_Frame_len=0;
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else//不是分帧的时候，去清分帧需要用到的参数，为分帧做准备
	{
		UARTCtrl->Last_Block_No=0xffffffff;
		if(UARTCtrl->Addr_Split_Frame!=0)
		{
			ms->ms_free((u8*)UARTCtrl->Addr_Split_Frame);//释放我的内存堆分配
			UARTCtrl->Addr_Split_Frame=0;
		}
		UARTCtrl->Save_Split_Frame_len=0;
	}
	return 0;//返回0正确,1地址错,2命令码错
}

void Terminal_ReadMeter_DL698(u32 PORTn)//终端抄表RS485口_上海规约
{
	u32 i;
	u32 x;
	u32 Fn;
	u32 Pn;
	u8 * p8RMLib;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	u8 * p8FnData;
	u32 PORT485;
	u32 ClassNo;

	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	u32 y;


  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
				Fn=MRR(DL698_RMLib[UARTCtrl->FnCount],1);
				if(Fn!=0xe1)
				{
					UARTCtrl->RMCountErr++;//Fn内有抄读失败或不抄的数据标识计数
				}
				if(Fn==3)
				{
					UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
					UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
					UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
				}
				switch(Fn)
				{
					default:
						UARTCtrl->RMError|=1;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
						break;
					case 129://F129 当前正向有功电能示值
						UARTCtrl->RMError|=2;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
						break;
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
				default:
					return;
			}
#if (((Project/100)==2)||((Project/100)==5))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块,700=
			i=MRR(ADDR_AFN04F33+((14+(4*24))*PORT485)+1,1);//终端抄表运行参数设置,端口31
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
			if(UARTCtrl->FnCount>=Get_RM_DL698_MaxFn())
			{//Fn计数>=最大Fn个数,本表抄表完成
				if(UARTCtrl->RMError==0)
				{//抄表成功
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
					ReadMeterNote_OK(PORTn,Pn,0);//集中抄表电表抄读信息.成功
					DateRMFlags(Pn,0);//日抄表标志;入口:f=0成功,=1失败
				}
				else
				{//抄表失败
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//寄存抄表成功失败标志:0=空,1=至少有1个数据标识抄读失败,2=成功
					ReadMeterNote_Fail(PORTn,Pn);//集中抄表电表抄读信息.失败
					DateRMFlags(Pn,1);//日抄表标志;入口:f=0成功,=1失败
				}
END_Fn:
				UARTCtrl->Task=0;//0=空闲
				UARTCtrl->FnEnd=0;//485抄表Fn计数结束标志,0=结束,1=没结束
				UARTCtrl->FnCount=0;//Fn计数=0
				UARTCtrl->RMCount=0;//抄表命令计数=0
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
				UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
				UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
				UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				UARTCtrl->RMError=0;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				break;
			}
			if(DL698_RMLib[UARTCtrl->FnCount]==0x0)
			{//本Fn无抄表
NEXT_Fn:
				UARTCtrl->Task=1;//完成1帧发收
				UARTCtrl->FnCount+=1;//Fn计数+1
				UARTCtrl->RMCount=0;//抄表命令计数=0
				UARTCtrl->ReRMCount=0;//抄表命令重发计数
				UARTCtrl->RMCountErr=0;//Fn内有抄读失败或不抄的数据标识计数
				UARTCtrl->RMError&=0xfe;//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
				UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
				UARTCtrl->RMDI=0xffffffff;//周期事件抄读的数据标识,0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
				
				break;	
			}
			p8RMLib=(u8 *)DL698_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//本Fn不是RS485抄表
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号			
			y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,3);//按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数
			i=UARTCtrl->RMCount;
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
				#if(RMM_RS485_FailData_0xEE==0)//RS485抄表失败数据填0xee;0=否,1=是
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
				goto NEXT_Fn;
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
				#if(RMM_RS485_FailData_0xEE==1)//RS485抄表失败数据填0xee;0=否,1=是
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
				#if(RMM_RS485_FailData_0xEE==1)//RS485抄表失败数据填0xee;0=否,1=是
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
				#else
						ClrERC29SOUR(Pn);//集中抄表失败清ERC29电能表飞走事件原始数据
				#endif
					}
					goto NEXT_Fn;
				}
		#if(RMM_RS485_FailData_0xEE==1)//RS485抄表失败数据填0xee;0=否,1=是
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
		#if(RMM_RS485_FailData_0xEE==1)//RS485抄表失败数据填0xee;0=否,1=是
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
		#endif
				if(p8RMLib[1]>UARTCtrl->RMCountErr)
				{//不是全部标识都没抄
					ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
				}
//				goto NEXT_Fn;//上海规约单相表没有0x2A最近一次需量清零发生时刻，不返回任何数据帧，不跳到下一个Fn避免F27没有数据。
			}

			Terminal_ReadMeter_DL698_TxData(PORTn,p8RMLib);//终端上海规约抄表发送数据

			if(UARTCtrl->TxByte==0)//组帧失败,Terminal_ReadMeter_DL698_TxData_TESAM里有可能失败
				goto NEXT_Fn;
				
			//通信速率
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//默认1200
				UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}

			//启动
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
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
					CopyString((u8 *)"485 周抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 3://定抄
					CopyString((u8 *)"485 定抄",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 5://事件
					CopyString((u8 *)"485 事件",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
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
			p8rxbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			MWR(PORT485,ADDR_DATABUFF+3,1);
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
			i=DL698_Rx(PORTn);//698.45规约帧接收;返回0没完成,1完成,2超时
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
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
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
			
			//比较发收地址、命令码、减0x33
			i=DL698_RxData(PORTn);//698.45规约帧接收数据,比较发收地址、命令码、减0x33;返回0正确,1地址错,2命令码错
			if(i!=0)
			{
				UARTCtrl->Task=2;//2=接收超时(或错误)
				break;
			}
			
			UARTCtrl->Task=1;//完成1帧发收
			p8RMLib=(u8 *)DL698_RMLib[UARTCtrl->FnCount];
			ReadMeter_DL698_FnData(PORTn,p8RMLib);//698.45规约抄表接收数据填入Fn数据缓冲
			UARTCtrl->RMCount+=1;//抄表命令计数+1
			i=UARTCtrl->RMCount;
			UARTCtrl->ReRMCount=0;//抄表命令重发计数=0
			break;
	}
}


void Terminal_ReadMeter_DL698_Fn(u32 PORTn,u8 *p8RMLib)//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
{
	u32 i;
	u32 x;
	u32 Pn;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	Log_Dumphex_Level(LOG_LEVEL_DEBUG,"in Terminal_ReadMeter_DL698_Fn",0,0);	
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
//				ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//存贮抄表Fn数据
				goto END_Fn;
			}
			
			Terminal_ReadMeter_DL698_TxData(PORTn,p8RMLib);//终端上海规约抄表发送数据
			
			//通信速率
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//默认1200
				UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}

			//启动
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
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
				x=p8rxbuff[7-i];
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
			i=DL698_Rx(PORTn);//上海规约帧接收;返回0没完成,1完成,2超时
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
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
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
			
			//比较发收地址、命令码、减0x33
			i=DL698_RxData(PORTn);//上海规约帧接收数据,比较发收地址、命令码、减0x33;返回0正确,1地址错,2命令码错
			if(i!=0)
			{
				UARTCtrl->Task=2;//2=接收超时(或错误)
				break;
			}
			UARTCtrl->Task=1;//完成1帧发收
			
			ReadMeter_DL698_FnData(PORTn,p8RMLib);//上海规约抄表接收数据填入Fn数据缓冲
			UARTCtrl->RMCount+=1;//抄表命令计数+1
			UARTCtrl->ReRMCount=0;//抄表命令重发计数=0
			break;
	}
}

#endif
