
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_3762_iActive.h"
#include "../DL698/RM_3762_RouterActive.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"

#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
void Find_Next_CollectAddr(u32 PORTn,u32 id)
{
	u32 i;
	#ifdef DEBUG_JSON
	u32 z;
	#endif
	u32 SETNO;
	u8 *pADDR_6000;
	UARTCtrl_TypeDef *UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	pADDR_6000=(u8 *)(ADDR_6000_SDRAM);
	SETNO=UARTCtrl->MeterCount[id];

	pADDR_6000+=SETNO*LENper_6000;
	for(i=SETNO;i<NUM_RMmax;i++)
	{
		if(pADDR_6000[0]!=DataType_structure)
		{
			pADDR_6000+=LENper_6000;
			continue;
		}
		break;
	}
	if(i>=NUM_RMmax)
	{
		pADDR_6000=(u8 *)(ADDR_6000_SDRAM);
		i=0;
		for(;i<SETNO;i++)
		{
			if(pADDR_6000[0]!=DataType_structure)
			{
				pADDR_6000+=LENper_6000;
				continue;
			}
			break;
		}
		if(i>=SETNO)
		{
			return;
		}
	}
	UARTCtrl->MeterCount[id]=i;
#ifdef DEBUG_JSON
	myprintf("[%s:%d]:成功找到了档案中下一只表的位置:%d\n",(u32)&__func__,(u32)__LINE__,i);
	pADDR_6000+=10;
	for(z=0;z<6;z++)
	{	
		myprintf("%02x",pADDR_6000[z],0,0);
	}
	myprintf("\n",0,0,0);
#endif
}

u32 Get_ListNo(u8* p8)//得到并发列表号;入口:抄表地址;返回:列表号0-(NUM_RMLISTmax-1)(>=NUM_RMLISTmax表示无效)
{
	u32 ListNo;
	u32 ListTask;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u32 i;
//	u16 *pAddr;
	u32 x;
	u64 d64a;
	u64 d64b;
	
	Addr0=p8[0]|(p8[1]<<8);
	Addr1=p8[2]|(p8[3]<<8);
	Addr2=p8[4]|(p8[5]<<8);
	
	
	for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
	{
		ListTask=RMLIST->List[ListNo].TASK;////0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
		if((ListTask>=1)&&(ListTask<=6))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
		{//有效
			if(Addr0!=RMLIST->List[ListNo].RMAddr[0])
			{
				continue;
			}
			if(Addr1!=RMLIST->List[ListNo].RMAddr[1])
			{
				continue;
			}
			if(Addr2!=RMLIST->List[ListNo].RMAddr[2])
			{
				continue;
			}
			return ListNo;
		}
	}
	
	x=Terminal_Router->NoteNum;//已知总从节点数

	d64a=MRR((u32)p8,6);
	for(i=0;i<x;i++)
	{
		d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
		d64b=R_Inverse(d64b,6);
		if(d64a==d64b)
		{
			d64b=MRR((u32)&NoteList->List[i].Addr,6);
			d64b=R_Inverse(d64b,6);
			MWR(d64b,(u32)p8,6);
			Addr0=p8[0]|(p8[1]<<8);
			Addr1=p8[2]|(p8[3]<<8);
			Addr2=p8[4]|(p8[5]<<8);
			break;//原已存在
		}
	}
//	p8=(u8*)ADDR_Meter_Collector;
//	for(i=0;i<NUM_RMmax;i++)
//	{
//		Data_Inverse((u32)p8+6,6);
//		pAddr=(u16*)(p8+6);
//		if(pAddr[0]==Addr0)
//		{
//			if(pAddr[1]==Addr1)
//			{
//				if(pAddr[2]==Addr2)
//				{
//					Data_Inverse((u32)p8,6);
//					Addr0=p8[0]|(p8[1]<<8);
//					Addr1=p8[2]|(p8[3]<<8);
//					Addr2=p8[4]|(p8[5]<<8);
//					Data_Inverse((u32)p8,6);
//					break;
//				}
//			}
//		}
//		Data_Inverse((u32)p8+6,6);
//		p8+=12;
//	}
	
	for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
	{
		ListTask=RMLIST->List[ListNo].TASK;////0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
		if((ListTask>=1)&&(ListTask<=6))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
		{//有效
			if(Addr0!=RMLIST->List[ListNo].RMAddr[0])
			{
				continue;
			}
			if(Addr1!=RMLIST->List[ListNo].RMAddr[1])
			{
				continue;
			}
			if(Addr2!=RMLIST->List[ListNo].RMAddr[2])
			{
				continue;
			}
			return ListNo;
		}
	}
	return ListNo;
}

u32 AddrComp(u8* p8)
{
	u64 d64a;
	u64 d64b;
	int i;
	
	d64a=MRR((u32)p8,6);
	d64a=R_Inverse(d64a,6);
	for(i=0;i<NUM_RMmax;i++)
	{
		d64b=MRR((u32)&NoteList->List[i].Addr,6);
		if((d64a==d64b))//&&(NoteList->List[i].user==0))
			return 0;
		if(d64b==0)
			break;
	}
	return 1;
}

u32 CompListRMAddr(u8* p8)//抄表地址与缓冲中有效中的抄表地址比较是否有相同;入口:p8指向地址(数据类型后);返回:列表号(从0开始,>=NUM_RMLISTmax表示没找到)
{
	u32 ListNo;
	u32 ListTask;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	
	Addr0=p8[0]|(p8[1]<<8);
	Addr1=p8[2]|(p8[3]<<8);
	Addr2=p8[4]|(p8[5]<<8);
	for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
	{
		ListTask=RMLIST->List[ListNo].TASK;////0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
		if((ListTask>=1)&&(ListTask<=6))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
		{//有效
			if(Addr0!=RMLIST->List[ListNo].RMAddr[0])
			{
				continue;
			}
			if(Addr1!=RMLIST->List[ListNo].RMAddr[1])
			{
				continue;
			}
			if(Addr2!=RMLIST->List[ListNo].RMAddr[2])
			{
				continue;
			}
			return ListNo;
		}
	}
	return ListNo;
}

void IncAndCheckTxSEQ(u32 PORTn)//发送帧序号+1,同时检查帧序号是否与缓冲中等待接收的帧序号是否有相同，若有相同则再加1
{
	u32 i;
	u32 ListNo;
	u32 ListTask;
	u32 TxSEQ;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	TxSEQ=UARTCtrl->TxSEQ;
	for(i=0;i<256;i++)
	{
		TxSEQ++;
		TxSEQ&=0xff;
		for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
		{
			ListTask=RMLIST->List[ListNo].TASK;////0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
			if((ListTask>=3)&&(ListTask<=6))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
			{//有效
				if(TxSEQ==RMLIST->List[ListNo].TxSEQ)
				{
					break;
				}
			}
		}
		if(ListNo>=NUM_RMLISTmax)
		{
			UARTCtrl->TxSEQ=TxSEQ;
			return;
		}
	}
	UARTCtrl->TxSEQ=TxSEQ;
}

void ConcurrencyRx(u32 PORTn)//并发接收与超时定时,超时状态
{
	u32 i;
	u32 ListTask;
	u32 ListNo;
	u16* p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
	{
		ListTask=RMLIST->List[ListNo].TASK;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
		if(ListTask==5)
		{//正在接收
			i=RMLIST->List[ListNo].TIMER_S;//并发启动发送后等待接收秒分定时
			if(i==0)
			{
				RMLIST->List[ListNo].TASK=2;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
				if(Terminal_Router->NUM_Enable_Concurrency<5)//宽带载波当前还允许并发帧数
				{
					Terminal_Router->NUM_Enable_Concurrency++;//宽带载波当前还允许并发帧数
				}
			}
			else
			{
				UARTCtrl->Task=5;//总状态正在接收
				i*=1000/10;
				if(i>p16timer[0])
				{
					p16timer[0]=i;
				}
			}
		}
	}
	
}

void Uart_3762_AFNF1F1_Tx(u32 PORTn,u32 Protocol,u32 ADDR_Data,u32 LEN_Data,u32 LEN_ESC,u32 DelayFlag)//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
{
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	p8tx[0]=0x68;
	p8tx[1]=(29+LEN_Data+2);//需总发送字节数
	p8tx[2]=(29+LEN_Data+2)>>8;
	if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	{
		p8tx[3]=0x43;//控制码;集中式路由载波通信
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{
			p8tx[3]=0x4a;//控制码;微功率无线通信
		}
		else
		{
			p8tx[3]=0x41;//控制码;集中式路由载波通信
		}
	}
	p8tx[4]=4;//通信模块标识=1
	p8tx[5]=0;
	p8tx[6]=LEN_ESC;//预计应答字节数
	p8tx[7]=0;
	p8tx[8]=0;
	//UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	IncAndCheckTxSEQ(PORTn);//发送帧序号+1,同时检查帧序号是否与缓冲中等待接收的帧序号是否有相同，若有相同则再加1
	p8tx[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
	//MR(((u32)p8tx)+16,ADDR_Data+1,6);//从节点地址(目的地址)
	d64a=UARTCtrl->RMAddr[2];
	d64a<<=16;
	d64a|=UARTCtrl->RMAddr[1];
	d64a<<=16;
	d64a|=UARTCtrl->RMAddr[0];
	d64a=R_Inverse(d64a,6);
	MWR(d64a,((u32)p8tx)+16,6);//从节点地址(目的地址)
	
	p8tx[22]=0xF1;//AFN=F1H
	p8tx[23]=0x1;//Fn=1
	p8tx[24]=0;
	p8tx[25]=Protocol;//2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
	p8tx[26]=0;//保留
	p8tx[27]=LEN_Data;//报文长度
	p8tx[28]=LEN_Data>>8;
	MR(((u32)p8tx)+29,ADDR_Data,LEN_Data);//标准645帧

	Uart_3762_CS((u32)p8tx);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
}

void RM_3762_iActive_HPLC(u32 PORTn)//集中器主动抄表(宽带载波并发抄表)
{
	u32 i;
	u32 x;
	u32 ListNo;
	u32 ListTask;
	u32 id;
  UARTCtrl_TypeDef * UARTCtrl;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 SetNo;
	u32 Info;
#if SEL_RouterRunMode3ADDR==1//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
	u8* pCSD;
	u64 d64a;
#endif
	u32 y;
	u64 d64b;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
		case 0://0=空闲
			UARTCtrl->Task=6;//3;//启动发送
			break;
		case 2://2=接收超时(或错误)
			UARTCtrl->Task=0;//空闲1次
			break;
		case 3://3=启动发送
			break;
		case 5://5=正在接收
			if(UARTCtrl->RxByte!=0)
			{
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				if(p16timer[0]>(1000/10))
				{
					p16timer[0]=1000/10;
				}
				return;
			}
		case 6://6=抄表发送(同RS485的3=启动发送)
			//代理任务
			i=0;
			if(UARTCtrl->ProxyTask>=1)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			{
				i=1;
			}
			//广播校时任务
			if((UARTCtrl->BroadcastTimeTask&0x7f)>=1)//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
			{
				i=1;
			}
			//自动搜表任务
			if(UARTCtrl->AutoSearchTask>=1)//自动搜表任务:0=空,1=等待,2...过程
			{
				i=1;
			}
			//电能表主动上报任务
			if(UARTCtrl->AutoReportTask>=1)//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
			{
				i=1;
			}
			ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
			//有其他任务等待,停止并发
//			if(UARTCtrl->Task!=5)
//			{
				if(i)
				{
					UARTCtrl->Task=0;//空闲
					return;
				}
//			}
			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
			i=p16timer[0];
			if(i!=0)
			{
			#if (USER/100)==0//电科院测试
				if(i>(5000/10))//下帧启动发送延时
				{
					p16timer[0]=5000/10;
				}
//				if(i>(4000/10))
//				{
//					//读状态
//					Uart_3762_TxData(PORTn,0x10,4,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
//				}
			#else
				//if(i>(1000/10))//下帧启动发送延时
				//{
					//p16timer[0]=1000/10;
				//}
			#endif
				break;
			}
			UARTCtrl->RMMeterCountAuto=0;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
			if(UARTCtrl->RTCS_TASK!=Comm_Ram->DL698YMDHMS[6])//实时钟更新标志原始秒(Task运算)
			{
				UARTCtrl->RTCS_TASK=Comm_Ram->DL698YMDHMS[6];//实时钟更新标志原始秒(Task运算)
				TASK(PORTn,1);//采集任务;TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
			}
			i=UARTCtrl->TaskID;
			if(i==0)//如果当前没有任务在跑
			{
				p16timer[0]=1000/10;//下次计算TASK()延时;注：376.2每接收到1帧会置30/10值
				UARTCtrl->Task=5;//正在接收
				RM_NUM_Display(PORTn);//抄表数量显示
				WarningOccur(PORTn,(u8 *)"载波 当前任务完成");//告警发生,入口告警代码和告警字符串
				return;
			}
			//缓冲列表任务
			for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
			{
				ListTask=RMLIST->List[ListNo].TASK;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
				if(ListTask<=2)
				{//0=空闲,1=完成1帧发收,2=接收超时(或错误)
					break;
				}
			}
			if(ListNo>=NUM_RMLISTmax)
			{//没找到空或完成1帧发收或接收超时
				//查停用
				for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
				{
					ListTask=RMLIST->List[ListNo].TASK;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
					if(ListTask==7)
					{
						ListTask=0;
						RMLIST->List[ListNo].TASK=0;//该表任务从0开始
						break;
					}
				}
				if(ListNo>=NUM_RMLISTmax)
				{//没停用
					UARTCtrl->Task=5;//正在接收
					return;
				}
			}
			i=Terminal_Router->NUM_Enable_Concurrency;//宽带载波当前还允许并发帧数
			if(i==0)
			{
				for(i=0;i<NUM_RMLISTmax;i++)
				{
					if(RMLIST->List[i].TASK==5)
					{//有在等待接收
						UARTCtrl->Task=5;//正在接收
						return;
					}
				}
				Terminal_Router->NUM_Enable_Concurrency=5;//宽带载波当前还允许并发帧数
			}
			
			id=UARTCtrl->TaskID;
			id>>=8;
			id&=0xff;
			UARTCtrl->SourMeterCount_id=UARTCtrl->MeterCount[id];//并发处理时当前任务id的原始抄表电能表计数
			switch(ListTask)
			{
				case 0://0=空闲
					Clr_RMListRMCount(ListNo);//清抄表计数相关数据;入口:ListNo=列表号从0开始
					MC(0xFF,(u32)&UARTCtrl->RMAddr,6);
					break;
				case 1://1=完成1帧发收
					break;
				case 2://2=接收超时(或错误)
					i=AddrToMeterSetNoJS((u16*)&RMLIST->List[ListNo].RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
					//i=AddrToMeterSetNo((u16*)&RMLIST->List[ListNo].RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
					
					i=AddrToMeterSetNoJS((u16*)&UARTCtrl->RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
					if(i>=NUM_RMmax)
					{//无效
						Clr_RMListRMCount(ListNo);//清抄表计数相关数据;入口:ListNo=列表号从0开始
						MC(0xFF,(u32)&UARTCtrl->RMAddr,6);
					}
					else
					{
						UARTCtrl->MeterCount[id]=i;//变更电能表计数
						MR((u32)&UARTCtrl->RMAddr,(u32)&RMLIST->List[ListNo].RMAddr,6);
					}
					break;
			}
			Load_RMCount_NoCompAddr(PORTn,ListNo);//得到抄表计数相关数据(不比较地址);入口:ListNo=列表号从0开始
			UARTCtrl->PIID=RMLIST->List[ListNo].PIID;//PIID的值在当前就改变(一开始从0开始)
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			if(UARTCtrl->ReRMCount>=3)//抄表重发计数
			{
				RM_OK_FAIL(PORTn,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
				UARTCtrl-> ReRMCount=0;//抄表重发计数
				UARTCtrl->RM_OKFAIL=0;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
				//下电能表
				RMLIST->List[ListNo].TASK=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
				Clr_RMListRMCount(ListNo);//清抄表计数相关数据;入口:ListNo=列表号从0开始 
				MC(0xFF,(u32)&UARTCtrl->RMAddr,6);
				Load_RMCount_NoCompAddr(PORTn,ListNo);//得到抄表计数相关数据(不比较地址);入口:ListNo=列表号从0开始
				UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
				RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
				Find_Next_CollectAddr(PORTn,id);
			}

			if(UARTCtrl->RM_SubsequentFrames==0)//0=无读后续帧,1-n=读续帧的帧号
			{//非后续帧
				UARTCtrl->RMMeterCountAuto=0;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
			#if(USER/100)==17//江苏系
				if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
				//if(((UARTCtrl->TaskID>>20)&0xf)==5)
					UARTCtrl->RMprotocol=3;
			#endif
				i=RM_TxDATA_Manage(PORTn);//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				//注:RM_TxDATA_Manage(PORTn)会清UARTCtrl-> ReRMCount=0
				//Get_ReRMCount(PORTn);//得到抄表重发计数,取入到UARTCtrl->ReRMCount
				UARTCtrl->ReRMCount++;//抄表重发计数
				
				if(UARTCtrl->TxByte==0)
				{//不需抄
					RMLIST->List[ListNo].TASK=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
					//注:调用Save_RMCount（）会将成功表又置为失败!!!
					//Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
					UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
					ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
					UARTCtrl->Task=5;//正在接收
					//myprintf("[%s:%d]:载波发送的字节为0,不需要抄读\n",(u32)&__func__,(u32)__LINE__,0);
					return;
				}
				i=CompListRMAddr((u8*)&UARTCtrl->RMAddr);//抄表地址与缓冲中有效中的抄表地址比较是否有相同;入口:p8指向地址(数据类型后);返回:列表号(从0开始,>=NUM_RMLISTmax表示没找到)
				if(Terminal_Router->MinuteCollect==1)//如果载波支持分钟采集就不比较这一项
				{
					i=NUM_RMLISTmax;
				}
				if((i<NUM_RMLISTmax)&&(i!=ListNo))
				{//当前地址原有列表号已存在,且当前的列表号和原列表号不同
					RMLIST->List[ListNo].TASK=7;//7=停用(为了能执行下个列表号)
					UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
					ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
					RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
					UARTCtrl->Task=5;//正在接收
					return;
				}
				MW((u32)&UARTCtrl->RMAddr,(u32)&RMLIST->List[ListNo].RMAddr,6);
				if(UARTCtrl->RMprotocol==3)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
				{
					i=100;//预计应答字节数
				}
				else
				{
					i=0;//预计应答字节数
				}
			#if (USER/100)==0//电科院测试验
				Uart_3762_AFNF1F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			#else
				if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能,8=钱龙
				{//鼎信无698暂用透明
					if(UARTCtrl->RMprotocol==3)
					{
						Uart_3762_AFNF1F1_Tx(PORTn,0,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
					}
					else
					{
						Uart_3762_AFNF1F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
					}
				}
				else
				{
					Uart_3762_AFNF1F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
				}
			#endif
			#if(USER/100)==17//江苏系
				i=UARTCtrl->TaskID;
				i>>=8;
				i&=0xff;
				if(i==0x22)
				{
					if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
					{
						switch(i)
						{
							case RMTASKnoPOWER://停上电事件固定任务号
								i=UARTCtrl->MeterCount[i];
								i=PowerEvent->setno[i];
								break;
							default:
								i=UARTCtrl->MeterCount[i];
								break;
						}
						if(i>=NUM_RMmax)
						{
							UARTCtrl->Task=5;//正在接收
							return;
						}
						x=Terminal_Router->NoteNum;//已知总从节点数
						d64a=MRR((u32)p8tx+16,6);
						for(y=0;y<x;y++)
						{
							d64b=MRR((u32)&NoteList->List[y].Addr,6);
							if(d64a==d64b)
							{
								d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
								d64b=R_Inverse(d64b,6);
								MWR(d64b,((u32)p8tx)+16,6);//从节点地址(目的地址)
								Uart_3762_CS((u32)p8tx);
								break;//原已存在
							}
						}
					}
				}
			#endif
			#if SEL_RouterRunMode3ADDR==1//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
				if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
				{//微功率无线,目标地址用采集器地址
					i=UARTCtrl->TaskID;
					i>>=8;
					i&=0xff;
					switch(i)
					{
						case RMTASKnoPOWER://停上电事件固定任务号
							i=UARTCtrl->MeterCount[i];
							i=PowerEvent->setno[i];
							break;
						default:
							i=UARTCtrl->MeterCount[i];
							break;
					}
					if(i>=NUM_RMmax)
					{
						UARTCtrl->Task=5;//正在接收
						return;
					}
					d64a=0;
					pCSD=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
					pCSD=Get_Element(pCSD,3,0,0);//计算元素地址
					if(pCSD)
					{//采集器可取
						d64a=MRR((u32)pCSD+5,6);//取配置采集器地址(目的地址)
						d64a=R_Inverse(d64a,6);
					}
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8tx)+16,6);//从节点地址(目的地址)
						Uart_3762_CS((u32)p8tx);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					}
				}
			#endif
				//Terminal_Router->NUM_Enable_Concurrency--;//宽带载波当前还允许并发帧数
			}
			else
			{//是后续帧
				UARTCtrl-> ReRMCount++;//抄表重发计数
				//p8tx[0]=0x68;
				p8tx[1]=(29+17+2);//需总发送字节数
				p8tx[2]=(29+17+2)>>8;
				//p8tx[3]=0x41;//控制码为0x41
				//p8tx[4]=4;//通信模块标识=1
				//p8tx[5]=0;
				p8tx[6]=0;//预计应答字节数
				//p8tx[7]=0;
				//p8tx[8]=0;
				//UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
				IncAndCheckTxSEQ(PORTn);//发送帧序号+1,同时检查帧序号是否与缓冲中等待接收的帧序号是否有相同，若有相同则再加1
				p8tx[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
				//MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
				//MR(((u32)p8tx)+16,ADDR_Data+1,6);//从节点地址(目的地址)
				//p8tx[22]=0x13;//AFN=13H
				//p8tx[23]=1;//Fn=1
				//p8tx[24]=0;
				//p8tx[25]=2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
				//p8tx[26]=0;//保留
				p8tx[27]=17;//DL645报文长度
				p8tx[28]=0;
				p8tx[29+8]=0x12;
				p8tx[29+9]=5;
				p8tx[29+14]=UARTCtrl->RM_SubsequentFrames+0x33;
				p8tx[29+15]=0;//cs
				for(i=0;i<15;i++)
				{
					p8tx[29+15]+=p8tx[29+i];
				}
				p8tx[29+16]=0x16;
				Uart_3762_CS((u32)p8tx);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
				UARTCtrl->TxByte=29+17+2;//发送字节数(半字对齐)
			}
			UARTCtrl->TxByte=p8tx[1]+(p8tx[2]<<8);//发送字节数(半字对齐)
			i=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;//启动发送
			RM_ADDR_Display(PORTn);//抄表地址显示
			//Comm_Ram->DisplayScanTimer=0;//立即显示
			i=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
			i>>=8;
			i&=0xff;
			RM_Monitor_TxNUM(i,1);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数

			Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
			UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
			RMLIST->List[ListNo].TASK=5;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
			//i=Terminal_Router->MaxOverTime;
			RMLIST->List[ListNo].TIMER_S=120;//并发启动发送后等待接收分秒定时
			RMLIST->List[ListNo].TxSEQ=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
			break;
		case 1://1=接收完成
			UARTCtrl->Task=0;//空闲1次
			//myprintf("[%s:%d]:接受完成之后的处理\n",(u32)&__func__,(u32)__LINE__,0);
			UARTCtrl->RxByte=0;
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//载波路由器死机秒定时值
			
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			
			i=p8rx[1]+(p8rx[2]<<8);
			if(i<=16)
			{//确认否认
				i=p8rx[10]+(p8rx[11]<<8)+(p8rx[12]<<16);
				if(i==0x0200)
				{//否认回答
					i=p8rx[9];//SEQ
					for(ListNo=0;ListNo<NUM_RMLISTmax;ListNo++)
					{
						if(RMLIST->List[ListNo].TxSEQ==i)
						{
							break;
						}
					}
					if(ListNo<NUM_RMLISTmax)
					{//找到
						Load_RMCount_NoCompAddr(PORTn,ListNo);//得到抄表计数相关数据(不比较地址);入口:ListNo=列表号从0开始
						if(UARTCtrl->ReRMCount)//连继重发次数计数
						{
							UARTCtrl->ReRMCount--;//连继重发次数计数
						}
						i=p8rx[13];
						switch(i)
						{
					//CCO端回复否认帧给集中器,有以下可能原因:
							case 109://0x6d
							case 110://0x6e
					//a）集中器下发的抄表报文的并发个数超过CCO允许的最大值，错误原因为109（超过最大并发数）;  
					//b）每个并发读表帧中可以承载多条电表协议报文，若集中器下发报文中的电表协议报文条数超过允许的最大值，CCO返回否认帧，错误原因为110（超过单个Q/GDW 1376.2帧最大允许的电表协议报文条数）; 
								Terminal_Router->NUM_Enable_Concurrency=0;//宽带载波当前还允许并发帧数
								RMLIST->List[ListNo].TASK=1;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
								break;
							case 111://0x6f
								//RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
								//Find_Next_CollectAddr(PORTn,id);
					//c）集中器抄表时，不允许并发给同一块电表多条抄表报文，即集中器下发第二条抄表报文的前提是要等到第一条抄表报文的回复。否则，CCO将返回否认帧，错误原因为111（该表正在抄读中）; 
								//RMLIST->List[ListNo].TASK=7;//7=停用(为了能执行下个列表号)
								if(Terminal_Router->MinuteCollect==1)
								{
									UARTCtrl->Task=5;//载波接受一次
								}
								RMLIST->List[ListNo].TASK=5;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
								//RMLIST->List[ListNo].TIMER_S=240;//并发启动发送后等待接收秒定时
								break;
							case 9://载波在做别的事情
							default:
					//d）当CCO端组网以及路由优化还未完成时，不允许抄表，如果此时集中器发送读表帧给CCO时，CCO会返回否认帧，错误原因为9（CCO模块忙，不允许抄表）。 
								RMLIST->List[ListNo].TASK=1;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
								break;
						}
						Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
					}
				}
				ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态	
				return;
			}
		
			id=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
			id>>=8;
			id&=0xff;
			RM_Monitor_RxNUM(id,1);//采集任务监控;入口:ID=任务号,NUM_Rx=接收条数
			ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态

			if(Terminal_Router->MinuteCollect==0)//不是分钟采集才会去判断这一项
			{
				MR((u32)&UARTCtrl->RMAddr,((u32)p8rx)+10,6);//从节点地址(目的地址)
				Data_Inverse((u32)&UARTCtrl->RMAddr,6);//数据倒序(高低字节调换),入口Le>=2
			}
			ListNo=Get_ListNo((u8*)&UARTCtrl->RMAddr);//得到并发列表号;入口:抄表地址;返回:列表号0-(NUM_RMLISTmax-1)(>=NUM_RMLISTmax表示无效)
			if(ListNo>=NUM_RMLISTmax)
			{
				ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
				return;
			}
			Load_RMCount_NoCompAddr(PORTn,ListNo);//得到抄表计数相关数据(不比较地址);入口:ListNo=列表号从0开始
			
			i=p8rx[22]+(p8rx[23]<<8)+(p8rx[24]<<16);
			if(i!=0x1F1)
			{//AFN=F1H,Fn=1
				//继续接收
				Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
				ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
				return;
			}
			
			x=p8rx[26]+(p8rx[27]<<8);//返回数据域长度
			if(x==0)
			{//接收数据长度=0,没收到帧也返回长度0
				RMLIST->List[ListNo].TASK=1;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
				Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
				ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
				//i=UARTCtrl->MeterCount[id];
				//i++;
				//UARTCtrl->MeterCount[id]=i;
				//Find_Next_CollectAddr(PORTn,id);
				return;
			}
			Info=p8rx[4]+(p8rx[5]<<8)+(p8rx[6]<<16)+(p8rx[7]<<24);
			MR((u32)p8rx,((u32)p8rx)+28,x);
			for(i=0;i<x;i++)
			{//去返回帧的前导0xfe
				if(p8rx[i]!=0xfe)
				{
					if(i!=0)
					{
						MR((u32)p8rx,((u32)p8rx)+i,x-i);
					}
					break;
				}
			}
			if(Terminal_Router->MinuteCollect==1)
			{
				i=0;//抄表组帧的地址有问题先强制通过
			}else
			{	
				i=RM_RxDATA_CHECK(PORTn);//帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			if((i==1)||(i==3))
			{//1地址错,3数据标识错
				//继续接收
				Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
				ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
				return;
			}
			if(i==4)
			{//4主动上报(已处理)
				//继续接收
				Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
				ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
				return;
			}
			UARTCtrl->RM_Rx_date_NULL=i;//抄表接收返回:0=有数据,!=0无数据(空数据)
			if(i==2)
			{//2控制码错(无数据)
//				UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
//				UARTCtrl->ReRMCount=0;//抄表重发计数
//				return;
			#if (USER/100)==15//安徽
				//安徽:针对三相表需要支持06000001和061000FF等块抄读
				if(UARTCtrl->RM_DI==0x06000001)
				{
					//继续接收
					Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
					ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
					return;
				}
			#endif
				//曲线等冻结数据当指定时间点无数据冻结时写入空数据避免补抄时重新抄读
				i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
				x=(UARTCtrl->TaskID>>20)&0xf;//采集类型
				if(!((i==0x5002)||(x==4)))
				{//不是抄分钟冻结(采曲线数据)和补抄(2=事件采集方案时会填入空数据而上报)
					UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
					UARTCtrl->ReRMCount=0;//抄表重发计数
					RMLIST->List[ListNo].TASK=1;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送),7=停用(为了能执行下个列表号)
					Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
					ConcurrencyRx(PORTn);//并发接收与超时定时,超时状态
					return;
				}
			}
			UARTCtrl->SourMeterCount_id=UARTCtrl->MeterCount[id];//并发处理时当前任务id的原始抄表电能表计数
		
		#if (USER/100)==17//江苏系
			if(id==0x22)
			{
				if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
				//if(((UARTCtrl->TaskID>>20)&0xf)==5)
					SetNo=AddrToMeterSetNoJS((u16*)&RMLIST->List[ListNo].RMAddr);
				else
					SetNo=AddrToMeterSetNo((u16*)&RMLIST->List[ListNo].RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效

			}
			else
				SetNo=AddrToMeterSetNo((u16*)&RMLIST->List[ListNo].RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
		#else
			SetNo=AddrToMeterSetNo((u16*)&RMLIST->List[ListNo].RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
		#endif	
			
		
			//UARTCtrl->MeterCount[id]=SetNo;
			
//		#if(USER/100)==17//江苏系
//			pMeterAddrNo=MeterSetAddrNo((u16*)&RMLIST->List[ListNo].RMAddr);
//			
//		#endif
			RM_RxDATA(PORTn);//抄表接收数据处理
			RM_State_RMUpDate(PORTn,1,SetNo,(Info>>4)&0xf,(Info>>16)&0xf);//采集状态,每次抄表后更新:入口:OK=0失败=1成功,OK=1时Class=中继级别,Phase=相位
			UARTCtrl->ReRMCount=0;//抄表重发计数
		#if(USER/100)==17//江苏系
			if(UARTCtrl->res23==5)//0=无读后续帧,1-n=读续帧的帧号
			{//无后续帧
				RM_OK_FAIL(PORTn,1);//抄表成功失败地址记录:入口:OK=0失败，=1成功
				UARTCtrl-> ReRMCount=0;//抄表重发计数
				UARTCtrl->RM_OKFAIL=1;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
				//下电能表
				RMLIST->List[ListNo].TASK=0;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
				Clr_RMListRMCount(ListNo);//清抄表计数相关数据;入口:ListNo=列表号从0开始 
				Load_RMCount_NoCompAddr(PORTn,ListNo);//得到抄表计数相关数据(不比较地址);入口:ListNo=列表号从0开始
				UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
				RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
				Find_Next_CollectAddr(PORTn,id);//
				UARTCtrl->res23=0;
			}
			else
			{
				UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
				RMLIST->List[ListNo].TASK=1;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
				Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
				UARTCtrl->res23=3;//发后续的8806
				//UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
			}
		#else
			UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
			RMLIST->List[ListNo].TASK=1;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			Save_RMCount(PORTn,ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
			UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
		#endif
			
			RMLIST->List[ListNo].res23=UARTCtrl->res23;
//			RMLIST->List[ListNo].MinuteCollect=UARTCtrl->MinuteCollect;
			RMLIST->List[ListNo].PIID=UARTCtrl->PIID;
			

	}
}

#endif


