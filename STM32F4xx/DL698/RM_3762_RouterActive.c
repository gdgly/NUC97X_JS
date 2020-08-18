

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
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






#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void Uart_3762_AFN14F1_Tx(u32 PORTn,u32 RMFlag)//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
{
	u32 x;
	u8 * p8rx;
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;

	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	{
		p8tx[3]=0x03;//控制码;集中式路由载波通信
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{
			p8tx[3]=0x0a;//控制码;微功率无线通信
		}
		else
		{
			p8tx[3]=0x01;//控制码;集中式路由载波通信
		}
	}
	p8tx[4]=4;//通信模块标识=1
	MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
	MR(((u32)p8tx)+16,((u32)p8rx)+14,6);//从节点地址(目的地址)
	p8tx[22]=0x14;//AFN=0x14
	p8tx[23]=0x01;//Fn=1
	p8tx[24]=0x00;//Fn=1
	p8tx[25]=RMFlag;//抄读标志=0抄读失败,=1抄读成功,2=抄读
	p8tx[26]=0;//通信延时相关性标志,0=无关,1=有关
	p8tx[27]=0;//DL645帧长度=0
	p8tx[28]=0;//附属节点数量N=0
	x=29+2;//需总发送字节数
	p8tx[1]=x;
	p8tx[2]=x>>8;
	UARTCtrl->TxByte=x;//发送字节数(半字对齐)

//	switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=
//	{
//		default://其他
//		case 2://鼎信
//			break;
//		case 3://东软
//			p8tx[4]=0x0;//通信模块标识=0
//			MR(((u32)p8tx)+10,((u32)p8tx)+22,7);
//			x=17+2;//需总发送字节数
//			p8tx[1]=x;
//			p8tx[2]=x>>8;
//			UARTCtrl->TxByte=x;//发送字节数(半字对齐)
//			break;
//	}

	Uart_3762_CS((u32)p8tx);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
	UARTCtrl->Task=3;//等待发送			
	UARTCtrl->TxToRxTimerOver=0;//1000/10//发送后接收超时定时值=0,使能很快超时而转空闲以便数据转发
}

void Get_ReRMCount(u32 PORTn)//得到抄表重发计数,取入到UARTCtrl->ReRMCount
{
	u32 i;
	u32 id;
	u32 m;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	RMokfailLIST_TypeDef *rmOKFAILlist;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->ReRMCount=0;//连续重发次数计数
	id=UARTCtrl->TaskID;
	if(id==0)
	{
		return;
	}
	id>>=8;
	id&=0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return;
	}
	Addr0=UARTCtrl->RMAddr[0];
	Addr1=UARTCtrl->RMAddr[1];
	Addr2=UARTCtrl->RMAddr[2];
	//查原是否失败记录
	m=UARTCtrl->NUMrmFAIL[id];//各任务已采集失败电能表数
	if(m>NUM_RMmax)
	{
		m=NUM_RMmax;
	}
	rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
	rmOKFAILlist--;
	for(i=0;i<m;i++)
	{
		if(rmOKFAILlist->Addr0==Addr0)
		{
			if(rmOKFAILlist->Addr1==Addr1)
			{
				if(rmOKFAILlist->Addr2==Addr2)
				{
					UARTCtrl->ReRMCount=rmOKFAILlist->ReRMCount;//连继重发次数计数
					return;
				}
			}
		}
		rmOKFAILlist--;
	}
}

void Save_ReRMCount(u32 PORTn)//存贮抄表重发计数,将UARTCtrl->ReRMCount写入到失败列表
{
	u32 i;
	u32 id;
	u32 n;
	u32 m;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	RMokfailLIST_TypeDef *rmOKFAILlist;
  UARTCtrl_TypeDef* UARTCtrl;

	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	id=UARTCtrl->TaskID;
	if(id==0)
	{
		return;
	}
	id>>=8;
	id&=0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return;
	}
	Addr0=UARTCtrl->RMAddr[0];
	Addr1=UARTCtrl->RMAddr[1];
	Addr2=UARTCtrl->RMAddr[2];
	n=UARTCtrl->NUMrmOK[id];
	if(n>=NUM_RMmax)
	{
		return;
	}
	m=UARTCtrl->NUMrmFAIL[id];//各任务已采集失败电能表数
	if((n+m)>NUM_RMmax)
	{
		m=NUM_RMmax-n;//各任务已采集失败电能表数
	}
	
	
////---test 抄表失败计数列表----
//	
//	if(UARTCtrl->ReRMCount==1)
//	{
//		id=id;
//	}
//	p8=(u8*)ADDR_Event_Sour;
//	rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
//	rmOKFAILlist--;
//	for(i=0;i<m;i++)
//	{
//		p8[0]=rmOKFAILlist->ReRMCount;
//		p8++;
//		rmOKFAILlist--;
//	}
////----test end------------
	
	
	
	//查原是否失败记录
	rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
	rmOKFAILlist--;
	for(i=0;i<m;i++)
	{
		if(rmOKFAILlist->Addr0==Addr0)
		{
			if(rmOKFAILlist->Addr1==Addr1)
			{
				if(rmOKFAILlist->Addr2==Addr2)
				{
					rmOKFAILlist->ReRMCount=UARTCtrl->ReRMCount;//连继重发次数计数
					return;
				}
			}
		}
		rmOKFAILlist--;
	}
	m++;
	if((n+m)>NUM_RMmax)
	{
		return;
	}
	UARTCtrl->NUMrmFAIL[id]=m;//各任务已采集失败电能表数
	rmOKFAILlist->Addr0=Addr0;
	rmOKFAILlist->Addr1=Addr1;
	rmOKFAILlist->Addr2=Addr2;
	rmOKFAILlist->ReRMCount=UARTCtrl->ReRMCount;//连继重发次数计数

}

void Clr_RMListRMCount(u32 ListNo)//清抄表计数相关数据;入口:ListNo=列表号从0开始
{
	RMLIST->List[ListNo].CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
	RMLIST->List[ListNo].RMprotocol=0;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	RMLIST->List[ListNo].RMCSDCount=0;//抄表CSD计数
	RMLIST->List[ListNo].RMROADCount=0;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
	RMLIST->List[ListNo].RMCSDSubCount=0;//抄表CSD子计数(非DL698抄表时)
	RMLIST->List[ListNo].RMCSDCountMax=0;//抄表CSD最大个数
	RMLIST->List[ListNo].RMROADCountMax=0;//抄表CSD的ROAD内有关联的OAD最大个数(非DL698抄表规约库设定)
	RMLIST->List[ListNo].RMCSDSubCountMax=0;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
	RMLIST->List[ListNo].RMROADCountOK=0;//抄表写文件数据缓冲中ROAD时已填入关联的OAD数据个数计数
	RMLIST->List[ListNo].TASK=0;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	RMLIST->List[ListNo].TIMER_S=0;//并发启动发送后等待接收秒定时
	//TxSEQ;//并发启动发送帧序号
	RMLIST->List[ListNo].RM_OKFAIL=0;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
	RMLIST->List[ListNo].RM_DI_CHOICE=0;//抄表DI选择:0=主DI,1=备用DI
	RMLIST->List[ListNo].RM_DI=0;//DL/T645-2007/1997当前抄表数据标识
	RMLIST->List[ListNo].RM_DI_TI_Count=0;//按时标间隔采集时的间隔计数
	RMLIST->List[ListNo].RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
	RMLIST->List[ListNo].RMOADROAD=0;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND

//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)

//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型,全部数据为单独顺序排列无总数组)
	RMLIST->List[ListNo].FnDataBuff[6]=0;//长度=时标+已采集的CSD(不包括本身2字节)
	RMLIST->List[ListNo].FnDataBuff[7]=0;
	RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+0]=0;//长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
	RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+1]=0;
	RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+2]=0;//长度=已填写入buff完整CSD的长度(不包括本身2字节)
	RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+3]=0;
}


void ClrALL_RMListRMCount(void)//清全部抄表计数相关数据(载波口)
{
	u32 i;
	for(i=0;i<NUM_RMLISTmax;i++)
	{
		Clr_RMListRMCount(i);//清抄表计数相关数据;入口:ListNo=列表号从0开始
	}
}

#if NUM_RMLISTmax<NUM_RMmax//载波路由主动抄表时用的最大抄表能缓存个数(抄表计数、已抄数据填入等)
u32 Get_RMListNO_Tx(u16* p16Addr)//得到当前抄表地址的列表号0-(NUM_RMLISTmax-1),当原没列表时自动分配列表号并初始化相关计数值
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	
	Addr0=p16Addr[0];
	Addr1=p16Addr[1];
	Addr2=p16Addr[2];
	for(i=0;i<NUM_RMLISTmax;i++)
	{
		if(Addr0==RMLIST->List[i].RMAddr[0])
		{
			if(Addr1==RMLIST->List[i].RMAddr[1])
			{
				if(Addr2==RMLIST->List[i].RMAddr[2])
				{
					return i;
				}
			}
		}
	}
	i=Terminal_Router->RMListCount;//抄表地址的列表号自动分配计数0-(NUM_RMLISTmax-1)
	if(i>=NUM_RMLISTmax)
	{
		i=0;
	}
	Terminal_Router->RMListCount=i+1;//抄表地址的列表号自动分配计数0-(NUM_RMLISTmax-1)
	Clr_RMListRMCount(i);//清抄表计数相关数据;入口:ListNo=列表号从0开始
	RMLIST->List[i].RMAddr[0]=Addr0;
	RMLIST->List[i].RMAddr[1]=Addr1;
	RMLIST->List[i].RMAddr[2]=Addr2;
	return i;
}
u32 Get_RMListNO_Rx(u16* p16Addr)//得到当前抄表地址的列表号0-(NUM_RMLISTmax-1),当原没列表时返回:NUM_RMLISTmax表示无效
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	
	Addr0=p16Addr[0];
	Addr1=p16Addr[1];
	Addr2=p16Addr[2];
	for(i=0;i<NUM_RMLISTmax;i++)
	{
		if(Addr0==RMLIST->List[i].RMAddr[0])
		{
			if(Addr1==RMLIST->List[i].RMAddr[1])
			{
				if(Addr2==RMLIST->List[i].RMAddr[2])
				{
					return i;
				}
			}
		}
	}
	return NUM_RMLISTmax;//返回:NUM_RMLISTmax表示无效
}
#endif


void Load_RMCount_NoCompAddr(u32 PORTn,u32 ListNo)//得到抄表计数相关数据(不比较地址);入口:ListNo=列表号从0开始
{
	u32 i;
	u32 x;
	u8* p8;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->CONNECT=RMLIST->List[ListNo].CONNECT;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
	UARTCtrl->RMprotocol=RMLIST->List[ListNo].RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	UARTCtrl->RMCSDCount=RMLIST->List[ListNo].RMCSDCount;//抄表CSD计数
	UARTCtrl->RMROADCount=RMLIST->List[ListNo].RMROADCount;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
	UARTCtrl->RMCSDSubCount=RMLIST->List[ListNo].RMCSDSubCount;//抄表CSD子计数(非DL698抄表时)
	UARTCtrl->RMCSDCountMax=RMLIST->List[ListNo].RMCSDCountMax;//抄表CSD最大个数
	UARTCtrl->RMROADCountMax=RMLIST->List[ListNo].RMROADCountMax;//抄表CSD的ROAD内有关联的OAD最大个数(非DL698抄表规约库设定)
	UARTCtrl->RMCSDSubCountMax=RMLIST->List[ListNo].RMCSDSubCountMax;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
	UARTCtrl->RMROADCountOK=RMLIST->List[ListNo].RMROADCountOK;//抄表写文件数据缓冲中ROAD时已填入关联的OAD数据个数计数
//	RMLIST->List[ListNo].TASK=0;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
//	RMLIST->List[ListNo].TIMER_S=0;//并发启动发送后等待接收秒定时
	Get_ReRMCount(PORTn);//得到抄表重发计数,取入到UARTCtrl->ReRMCount
	UARTCtrl->RM_OKFAIL=RMLIST->List[ListNo].RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
	UARTCtrl->RM_DI_CHOICE=RMLIST->List[ListNo].RM_DI_CHOICE;//抄表DI选择:0=主DI,1=备用DI
	UARTCtrl->RM_DI=RMLIST->List[ListNo].RM_DI;//DL/T645-2007/1997当前抄表数据标识
	UARTCtrl->RM_DI_TI_Count=RMLIST->List[ListNo].RM_DI_TI_Count;//按时标间隔采集时的间隔计数
	UARTCtrl->RM_SubsequentFrames=RMLIST->List[ListNo].RM_SubsequentFrames;//0=无读后续帧,1-n=读续帧的帧号
	UARTCtrl->RMOADROAD=RMLIST->List[ListNo].RMOADROAD;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND
	UARTCtrl->res23=RMLIST->List[ListNo].res23;
	//UARTCtrl->MinuteCollect=RMLIST->List[ListNo].MinuteCollect;

	for(i=0;i<8;i++)
	{
		UARTCtrl->RM_Rx_date_time_s[i]=RMLIST->List[ListNo].RM_Rx_date_time_s[i];//抄表接收到的数据时标(无数据类型),7byte,仅为对齐定义8byte
	}
	for(i=0;i<10;i++)
	{
		UARTCtrl->RM_Rx_date_40020200[i]=RMLIST->List[ListNo].RM_Rx_date_40020200[i];//抄表接收到的数据事件次暂存及用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
	}
	for(i=0;i<16;i++)
	{
		UARTCtrl->Rand[i]=RMLIST->List[ListNo].Rand[i];//抄表安全模式时发送给电表的16字节随机数
	}
	for(i=0;i<LEN_RMCSD;i++)
	{
		UARTCtrl->RMCSD[i]=RMLIST->List[ListNo].RMCSD[i];//128-x byte 当前正在抄的CSD(有数据类型)
	}
	
//FnData
	//i=Get_ADDR_UARTnFnDataBuff(PORTn);
	//MR(i,(u32)&RMLIST->List[ListNo].FnDataBuff,LEN_UART4FnDataBuff);
//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)
	p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);
	i=RMLIST->List[ListNo].FnDataBuff[6];//长度=时标+已采集的CSD(不包括本身2字节)
	i|=RMLIST->List[ListNo].FnDataBuff[7]<<8;
	i+=8;
	if(i>LEN_RM_DI_CSD)
	{
		i=LEN_RM_DI_CSD;
	}
	MR((u32)p8,(u32)&RMLIST->List[ListNo].FnDataBuff,i);
//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型,全部数据为单独顺序排列无总数组)
	i=RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD];
	i|=RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+1]<<8;
	x=RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+2];
	x|=RMLIST->List[ListNo].FnDataBuff[LEN_RM_DI_CSD+3]<<8;
	i+=x;
	i+=4;
	if(i>(LEN_UART4FnDataBuff-LEN_RM_DI_CSD))
	{
		i=LEN_UART4FnDataBuff-LEN_RM_DI_CSD;
	}
	MR((u32)p8+LEN_RM_DI_CSD,(u32)&RMLIST->List[ListNo].FnDataBuff+LEN_RM_DI_CSD,i);
}

void Load_RMCount_CompAddr(u32 PORTn,u32 ListNo)//得到抄表计数相关数据(比较地址);入口:ListNo=列表号从0开始
{
	u32 i;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	for(i=0;i<3;i++)
	{
		if(UARTCtrl->RMAddr[i]!=RMLIST->List[ListNo].RMAddr[i])
		{//地址变化
			Clr_RMListRMCount(ListNo);//清抄表计数相关数据;入口:ListNo=列表号从0开始
			for(i=0;i<3;i++)
			{
				RMLIST->List[ListNo].RMAddr[i]=UARTCtrl->RMAddr[i];
			}
			break;
		}
	}
	Load_RMCount_NoCompAddr(PORTn,ListNo);//得到抄表计数相关数据(不比较地址);入口:ListNo=列表号从0开始
}

void Save_RMCount(u32 PORTn,u32 ListNo)//保存抄表计数相关数据;入口:ListNo=列表号从0开始
{
	u32 i;
	u32 x;
	u8* p8;
  UARTCtrl_TypeDef* UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	RMLIST->List[ListNo].CONNECT=UARTCtrl->CONNECT;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
	RMLIST->List[ListNo].RMprotocol=UARTCtrl->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	RMLIST->List[ListNo].RMCSDCount=UARTCtrl->RMCSDCount;//抄表CSD计数
	RMLIST->List[ListNo].RMROADCount=UARTCtrl->RMROADCount;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
	RMLIST->List[ListNo].RMCSDSubCount=UARTCtrl->RMCSDSubCount;//抄表CSD子计数(非DL698抄表时)
	RMLIST->List[ListNo].RMCSDCountMax=UARTCtrl->RMCSDCountMax;//抄表CSD最大个数
	RMLIST->List[ListNo].RMROADCountMax=UARTCtrl->RMROADCountMax;//抄表CSD的ROAD内有关联的OAD最大个数(非DL698抄表规约库设定)
	RMLIST->List[ListNo].RMCSDSubCountMax=UARTCtrl->RMCSDSubCountMax;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
	RMLIST->List[ListNo].RMROADCountOK=UARTCtrl->RMROADCountOK;//抄表写文件数据缓冲中ROAD时已填入关联的OAD数据个数计数
//	RMLIST->List[ListNo].TASK=0;//并发任务:0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
//	RMLIST->List[ListNo].TIMER_S=0;//并发启动发送后等待接收秒定时
	Save_ReRMCount(PORTn);//存贮抄表重发计数,将UARTCtrl->ReRMCount写入到失败列表
	RMLIST->List[ListNo].RM_OKFAIL=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
	RMLIST->List[ListNo].RM_DI_CHOICE=UARTCtrl->RM_DI_CHOICE;//抄表DI选择:0=主DI,1=备用DI
	RMLIST->List[ListNo].RM_DI=UARTCtrl->RM_DI;//DL/T645-2007/1997当前抄表数据标识
	RMLIST->List[ListNo].RM_DI_TI_Count=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
	RMLIST->List[ListNo].RM_SubsequentFrames=UARTCtrl->RM_SubsequentFrames;//0=无读后续帧,1-n=读续帧的帧号
	RMLIST->List[ListNo].RMOADROAD=UARTCtrl->RMOADROAD;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND

	for(i=0;i<8;i++)
	{
		RMLIST->List[ListNo].RM_Rx_date_time_s[i]=UARTCtrl->RM_Rx_date_time_s[i];//抄表接收到的数据时标(无数据类型),7byte,仅为对齐定义8byte
	}
	for(i=0;i<10;i++)
	{
		RMLIST->List[ListNo].RM_Rx_date_40020200[i]=UARTCtrl->RM_Rx_date_40020200[i];//抄表接收到的数据事件次暂存及用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
	}
	for(i=0;i<16;i++)
	{
		RMLIST->List[ListNo].Rand[i]=UARTCtrl->Rand[i];//抄表安全模式时发送给电表的16字节随机数
	}
	for(i=0;i<LEN_RMCSD;i++)
	{
		RMLIST->List[ListNo].RMCSD[i]=UARTCtrl->RMCSD[i];//128-x byte 当前正在抄的CSD(有数据类型)
	}
	
//FnData
	//i=Get_ADDR_UARTnFnDataBuff(PORTn);
	//MW(i,(u32)&RMLIST->List[ListNo].FnDataBuff,LEN_UART4FnDataBuff);
//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)
	p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);
	i=p8[6];//长度=时标+已采集的CSD(不包括本身2字节)
	i|=p8[7]<<8;
	i+=8;
	if(i>LEN_RM_DI_CSD)
	{
		i=LEN_RM_DI_CSD;
	}
	MW((u32)p8,(u32)&RMLIST->List[ListNo].FnDataBuff,i);
//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型,全部数据为单独顺序排列无总数组)
	i=p8[LEN_RM_DI_CSD];
	i|=p8[LEN_RM_DI_CSD+1]<<8;
	x=p8[LEN_RM_DI_CSD+2];
	x|=p8[LEN_RM_DI_CSD+3]<<8;
	i+=x;
	i+=4;
	if(i>(LEN_UART4FnDataBuff-LEN_RM_DI_CSD))
	{
		i=LEN_UART4FnDataBuff-LEN_RM_DI_CSD;
	}
	MW((u32)p8+LEN_RM_DI_CSD,(u32)&RMLIST->List[ListNo].FnDataBuff+LEN_RM_DI_CSD,i);
}




void RM_3762_RouterActive(u32 PORTn)//载波抄表(路由模块主导的周期抄表模式)
{
	u32 i;
	u32 LEN_DATA;
	u32 Info;
	u8* p8rx;
	u8* p8tx;
	u64 d64a;
//	u64 d64b;
	u16* p16timer;
	u32 SetNo;
	u32 RMListNO;
  UARTCtrl_TypeDef* UARTCtrl;
	u32 NUM_SomeTaskID;
	u32 y;
	
	NUM_SomeTaskID=NUM_SomeTaskID;
	y=y;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		case 0://0=空闲
			UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
			UARTCtrl->Task=5;//正在接收
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			p16timer[0]=1000/10;//超时定时
			break;
		case 2://2=接收超时(或错误)
			UARTCtrl->Task=0;//空闲1次
			if(Terminal_Ram->RouterTASK_S_Timer==0)//36 载波路由主动抄表发送后到接收超时时调用TASK()任务计算秒定时器
			{
				Terminal_Ram->RouterTASK_S_Timer=10;//36 载波路由主动抄表发送后到接收超时时调用TASK()任务计算秒定时器
				UARTCtrl->RMMeterCountAuto=1;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
				TASK(PORTn,1);//采集任务;TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号;返回:0表示无采集任务
				i=UARTCtrl->TaskID;
				if(i==0)
				{
					RM_NUM_Display(PORTn);//抄表数量显示
					WarningOccur(PORTn,(u8 *)"载波 当前任务完成");//告警发生,入口告警代码和告警字符串
					return;
				}
				if((i>>28)==0)//优先级=0;上电启抄,每小时曲线等用点抄
				{
					return;
				}
				if((Terminal_Router->RUNState&2)==0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
				{
					if(Terminal_Router->RouterCtrl==0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
					{
						Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
						Terminal_Router->RouterInitTask=201;//延时路由恢复
					}
				}
			}
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 载波路由器死机秒定时器
			{
				Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			break;
		case 1://1=接收完成
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//载波路由器死机秒定时值
			i=p8rx[4];//读通信模块标识
			i&=0x4;
			if(i==0x0)
			{//集中器通信模块
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
				switch(i)
				{
					case 0x1401://AFN=0x14,Fn=1;路由器发送请求抄表
						Terminal_Ram->RouterTASK_S_Timer=10;//36 载波路由主动抄表发送后到接收超时时调用TASK()任务计算秒定时器
						if(Terminal_Router->RouterCtrl!=0)//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
						{
							UARTCtrl->Task=0;//空闲时进入其他任务
							return;
						}
						i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						if(i)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						{
							UARTCtrl->Task=0;//空闲时进入其他任务
							return;
						}
						//广播校时
						i=UARTCtrl->BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
						if((i&0x7f)==1)
						{
							UARTCtrl->Task=0;//空闲时进入其他任务
							return;
						}
						//自动搜表
						i=UARTCtrl->AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
						if(i==1)
						{
							UARTCtrl->Task=0;//空闲时进入其他任务
							return;
						}
						//电能表主动上报
						i=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
						if(i==1)
						{
							UARTCtrl->Task=0;//空闲时进入其他任务
							return;
						}
						MR((u32)p8tx,(u32)p8rx,10);//复制10字节接收头到发送头
						d64a=MRR(((u32)p8rx)+14,6);//取从节点地址
						d64a=R_Inverse(d64a,6);
						MWR(d64a,(u32)&UARTCtrl->RMAddr,6);//帧返回地址
						SetNo=AddrToMeterSetNoJS((u16*)&UARTCtrl->RMAddr);
						//SetNo=AddrToMeterSetNo((u16*)&UARTCtrl->RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
						if(SetNo>=NUM_RMmax)
						{//没找到,直接返回抄读成功
							Uart_3762_AFN14F1_Tx(PORTn,1);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
							return;
						}
					#if NUM_RMLISTmax<NUM_RMmax//载波路由主动抄表时用的最大抄表能缓存个数(抄表计数、已抄数据填入等)
						RMListNO=Get_RMListNO_Tx((u16*)&UARTCtrl->RMAddr);//得到当前抄表地址的列表号0-(NUM_RMLISTmax-1),当原没列表时自动分配列表号并初始化相关计数值
					#else
						RMListNO=SetNo;
					#endif
						
						Load_RMCount_CompAddr(PORTn,RMListNO);//得到抄表计数相关数据(比较地址);入口:ListNo=列表号从0开始
						
						UARTCtrl->RMMeterCountAuto=1;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
						i=UARTCtrl->RM_DI_TI_Count;//保护
						TASK(PORTn,1);//采集任务;TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号;返回:0表示无采集任务
						if(Terminal_Router->RouterCtrl==1)//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
						{
							UARTCtrl->Task=0;//0=空闲
							return;
						}
						MWR(d64a,(u32)&UARTCtrl->RMAddr,6);//帧返回地址
						if(UARTCtrl->CONNECT!=0)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
						{
							UARTCtrl->RM_DI_TI_Count=i;//恢复
						}
						
						i=UARTCtrl->TaskID;
						if(i==0)
						{
							UARTCtrl->Task=0;//空闲
							RM_NUM_Display(PORTn);//抄表数量显示
							WarningOccur(PORTn,(u8 *)"载波 当前任务完成");//告警发生,入口告警代码和告警字符串
							return;
						}
						if((i>>28)==0)//优先级=0;上电启抄,每小时曲线等用点抄
						{
							return;
						}
						i>>=8;
						i&=0xff;
						if(i>=(RMTASKmax+ExtRMTASKmax))
						{
							UARTCtrl->Task=0;
							return;
						}
	
						UARTCtrl->MeterCount[i]=SetNo;//2*RMTASKmax byte各任务已抄电能表计数,主动上报时用于文件中已上报电能表计数
						
					#if RMovertimeACKfail==0//载波路由主动请求抄表大于连续次失败时:0=继续发抄表帧,1=返回抄表失败(跳表)
						i=UARTCtrl->ReRMCount;
						if(i>=1)//抄表重发计数;路由主动时每表请求2次后可能就自动换表了,当重复次数>=2时永不会失败!!!
						{
							if(i==0xff)
							{
								UARTCtrl->ReRMCount=0xfe;
							}
							RM_OK_FAIL(PORTn,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
//							if(UARTCtrl->ReRMCount>=5)//抄表重发计数;路由主动时每表请求2次后可能就自动换表了,当重复次数>=2时永不会失败!!!
//							{
//								//Save_RMCount(PORTn,RMListNO);//保存抄表计数相关数据;入口:SetNo=档案号从0开始
//								Uart_3762_AFN14F1_Tx(PORTn,0);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
//								UARTCtrl->RMAddr[0]=0xffff;
//								UARTCtrl->RMAddr[1]=0xffff;
//								UARTCtrl->RMAddr[2]=0xffff;
//								return;
//							}
						}
					#else
						if(UARTCtrl->ReRMCount>=1)//抄表重发计数;路由主动时每表请求2次后可能就自动换表了,当重复次数>=2时永不会失败!!!
						{
							RM_OK_FAIL(PORTn,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
							if(UARTCtrl->ReRMCount>=3)//抄表重发计数;路由主动时每表请求2次后可能就自动换表了,当重复次数>=2时永不会失败!!!
							{
								//Save_RMCount(PORTn,RMListNO);//保存抄表计数相关数据;入口:SetNo=档案号从0开始
								Uart_3762_AFN14F1_Tx(PORTn,0);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
								UARTCtrl->RMAddr[0]=0xffff;
								UARTCtrl->RMAddr[1]=0xffff;
								UARTCtrl->RMAddr[2]=0xffff;
								return;
							}
						}
					#endif
/*
						if(UARTCtrl->ReRMCount>=2)//抄表重发计数;路由主动时每表请求2次后可能就自动换表了,当重复次数>=2时永不会失败!!!
						{
						#if RMovertimeACKfail==0//载波路由主动请求抄表大于连续次失败时:0=继续发抄表帧,1=返回抄表失败(跳表)
							if(UARTCtrl->ReRMCount==0xff)
							{
								UARTCtrl->ReRMCount=0xfe;
							}
							RM_OK_FAIL(PORTn,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
						#else
							//下列清注意要同void CLR_RMCount(u32 PORTn)//清0抄表发送帧各计数值
							UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中
							UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
							UARTCtrl->RMCSDCount=0;//抄表CSD计数
							UARTCtrl->RMCSDSubCount=0;//抄表CSD子计数
							UARTCtrl->RMROADCount=0;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
							UARTCtrl->RMROADCountOK=0;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
							UARTCtrl->ReRMCount=0;//抄表重发计数
							UARTCtrl->RMErr=0;//抄表错误标志
							UARTCtrl->RM_DI_TI_Count=0;//按时标间隔采集时的间隔计数
							UARTCtrl->RMOADROAD=0;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND
							
							RM_OK_FAIL(PORTn,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
							Save_RMCount(PORTn,RMListNO);//保存抄表计数相关数据;入口:SetNo=档案号从0开始
							Uart_3762_AFN14F1_Tx(PORTn,0);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
							UARTCtrl->RMAddr[0]=0;
							UARTCtrl->RMAddr[1]=0;
							UARTCtrl->RMAddr[2]=0;
							return;
						#endif
						}
*/
						UARTCtrl->RMMeterCountAuto=1;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
						i=RM_TxDATA_Manage(PORTn);//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
						//注:RM_TxDATA_Manage(PORTn)会清UARTCtrl-> ReRMCount=0!!!
						Get_ReRMCount(PORTn);//得到抄表重发计数,取入到UARTCtrl->ReRMCount
						UARTCtrl->ReRMCount++;//抄表重发计数
						if(i==2)
						{//
							if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{
								p8tx[3]=0x03;//控制码;集中式路由载波通信
							}
							else
							{
								if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
								{
									p8tx[3]=0x0a;//控制码;微功率无线通信
								}
								else
								{
									p8tx[3]=0x01;//控制码;集中式路由载波通信
								}
							}
							p8tx[4]=4;//通信模块标识=1
							if(UARTCtrl->RMprotocol==3)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
							{
								p8tx[6]=100;//预计应答字节数
							}
							else
							{
								p8tx[6]=0;//预计应答字节数
							}
							MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
							d64a=UARTCtrl->RMAddr[2];
							d64a<<=16;
							d64a|=UARTCtrl->RMAddr[1];
							d64a<<=16;
							d64a|=UARTCtrl->RMAddr[0];
							d64a=R_Inverse(d64a,6);
							MWR(d64a,((u32)p8tx)+16,6);//从节点地址(目的地址)
							p8tx[22]=0x14;//AFN=0x14
							p8tx[23]=0x01;//Fn=1
							p8tx[24]=0x00;//Fn=1
							p8tx[25]=2;//抄读标志=2
							p8tx[26]=0;//通信延时相关性标志,0=无关,1=有关
							i=UARTCtrl->TxByte;
							p8tx[27]=i;//DL645帧长度
							MR((u32)p8tx+28,(u32)p8tx+128,i);
							p8tx[28+i]=0;//附属节点数量N=0
							i+=29+2;//需总发送字节数
							p8tx[1]=i;
							p8tx[2]=i>>8;
							UARTCtrl->TxByte=i;//发送字节数(半字对齐)

							Uart_3762_CS((u32)p8tx);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
							UARTCtrl->Task=3;//等待发送
							UARTCtrl->TxToRxTimerOver=1000/10;//发送后接收超时定时值ms/10(半字对齐)
							Terminal_Ram->RouterTASK_S_Timer=60;//36 载波路由主动抄表发送后到接收超时时调用TASK()任务计算秒定时器
							
							RM_ADDR_Display(PORTn);//抄表地址显示
							i=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
							i>>=8;
							i&=0xff;
							RM_Monitor_TxNUM(i,1);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数
							
							Save_RMCount(PORTn,RMListNO);//得到抄表计数相关数据;入口:SetNo=档案号从0开始
						}
						else
						{//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
							if(i<=1)
							{
								Uart_3762_AFN14F1_Tx(PORTn,1);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
								//成功后清缓冲中的抄表地址;免下次抄表时找到数据抄表计数没清0
								RMLIST->List[RMListNO].RMAddr[0]=0xffff;
								RMLIST->List[RMListNO].RMAddr[1]=0xffff;
								RMLIST->List[RMListNO].RMAddr[2]=0xffff;
							}
							else
							{//>=3;3=失败,4=任务暂停
								Uart_3762_AFN14F1_Tx(PORTn,0);//路由器请求抄表返回帧;入口:RMFlag抄读标志=0抄读失败,=1抄读成功,2=抄读
							}
							RM_NUM_Display(PORTn);//抄表数量显示
						}
						break;
					case 0x0602://AFN=0x06,Fn=2;路由器发送上报抄表数据
						//东软等不并发的上报抄表数据从此返回
						MR((u32)p8tx,(u32)p8rx,10);//复制10字节接收头到发送头
						Info=p8rx[4]+(p8rx[5]<<8)+(p8rx[6]<<16)+(p8rx[7]<<24);//取6字节376.2返回的信息域
						i=p8rx[16]+(p8rx[17]<<8);//线路上传输延时时间秒
						UARTCtrl->TransferDelay=i;
						LEN_DATA=p8rx[18];//读DL645帧长度
						MR(((u32)p8rx),((u32)p8rx)+19,LEN_DATA);//取645帧
						goto RMESC;//抄表返回
					case 0x1402://AFN=0x14,Fn=2;路由请求集中器时钟
						MR((u32)p8tx,(u32)p8rx,10);//复制10字节接收头到发送头
						MW(ADDR_MYMDHMS,((u32)p8tx)+128,6);
						Uart_3762_TxData(PORTn,0x14,2,6);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
						break;
					//case 0x1404://AFN=0x14,Fn=3;请求依通信延时修正通信数据
						//break;
					default:
						UARTCtrl->RxByte=0;//接收字节数(半字对齐)
						UARTCtrl->Task=5;//正在接收
						break;
				}
			}
			else
			{//电能表通信模块
				i=p8rx[22]+(p8rx[23]<<8)+(p8rx[24]<<16);
				switch(i)
				{
					case 0x000206://AFN=0x06,Fn=2;路由器发送上报抄表数据
						//鼎信等并发的上报抄表数据从此返回
						MR((u32)p8tx,(u32)p8rx,10);//复制10字节接收头到发送头
						Info=p8rx[4]+(p8rx[5]<<8)+(p8rx[6]<<16)+(p8rx[7]<<24);
						i=p8rx[28]+(p8rx[29]<<8);//线路上传输延时时间秒
						UARTCtrl->TransferDelay=i;
						LEN_DATA=p8rx[30];//读DL645帧长度
						d64a=MRR(((u32)p8rx)+10,6);//取从节点地址
						d64a=R_Inverse(d64a,6);
						MWR(d64a,(u32)&UARTCtrl->RMAddr,6);//帧返回地址
						MR(((u32)p8rx),((u32)p8rx)+31,LEN_DATA);//取645帧
					
				RMESC://抄表返回
						SetNo=AddrToMeterSetNoJS((u16*)&UARTCtrl->RMAddr);
						//SetNo=AddrToMeterSetNo((u16*)&UARTCtrl->RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
						if(SetNo>=NUM_RMmax)
						{//没找到,直接返回抄读成功
							goto RXESC;
						}
					#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
						NUM_SomeTaskID=UARTCtrl->NUM_SomeTaskID;//当前同类采集任务的id号的列表个数
						y=0;
						for(i=0;i<NUM_SomeTaskID;i++)
						{
							y=UARTCtrl->TaskID;
							y&=0xffff0000;
							y|=UARTCtrl->SomeTaskid[i];//RMTASKmax*2 byte 当前同类采集任务的b15-b8任务号,b7-b0方案编号
							UARTCtrl->TaskID=y;
							y>>=8;
							y&=0xff;//任务号
							if(y>=(RMTASKmax+ExtRMTASKmax))
							{
								continue;
							}
							UARTCtrl->MeterCount[y]=SetNo;//2*RMTASKmax byte各任务已抄电能表计数,主动上报时用于文件中已上报电能表计数
							y=RM_Meter(PORTn);//当前电能表计数是否要抄;入口:UARTCtrl->MeterCount[],UARTCtrl->TaskID;返回:0=不抄,1=要抄,2=完成,同时填写通信地址,波特率,抄表规约类型,
							if(y)
							{
								break;
							}
						}
						if(y==0)
						{//没找到
							goto RXESC;
						}
					#endif
						i=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
						i>>=8;
						i&=0xff;
						if(i>=(RMTASKmax+ExtRMTASKmax))
						{
							goto RXESC;
						}
						RM_Monitor_RxNUM(i,1);//采集任务监控;入口:ID=任务号,NUM_Rx=接收条数
						UARTCtrl->MeterCount[i]=SetNo;//2*RMTASKmax byte各任务已抄电能表计数,主动上报时用于文件中已上报电能表计数
						
					#if NUM_RMLISTmax<NUM_RMmax//载波路由主动抄表时用的最大抄表能缓存个数(抄表计数、已抄数据填入等)
						RMListNO=Get_RMListNO_Rx((u16*)&UARTCtrl->RMAddr);//得到当前抄表地址的列表号0-(NUM_RMLISTmax-1),当原没列表时返回:NUM_RMLISTmax表示无效
						if(RMListNO>=NUM_RMLISTmax)
						{//没找到
							goto RXESC;
						}
					#else
						RMListNO=SetNo;
					#endif
						
						Load_RMCount_CompAddr(PORTn,RMListNO);//得到抄表计数相关数据(比较地址);入口:ListNo=列表号从0开始
						for(i=0;i<LEN_DATA;i++)
						{//去返回645帧的前导0xfe
							if(p8rx[i]!=0xfe)
							{
								if(i!=0)
								{
									MR((u32)p8rx,((u32)p8rx)+i,LEN_DATA-i);
								}
								break;
							}
						}
						i=RM_RxDATA_CHECK(PORTn);//帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
						if((i==1)||(i==3))
						{//1地址错,3数据标识错
							//UARTCtrl->Task=2;//2=接收超时(或错误)
							Save_RMCount(PORTn,RMListNO);//得到抄表计数相关数据;入口:SetNo=档案号从0开始
							goto RXESC;
						}
						if(i==4)
						{//4主动上报(已处理)
							Save_RMCount(PORTn,RMListNO);//得到抄表计数相关数据;入口:SetNo=档案号从0开始
							goto RXESC;
						}
						UARTCtrl->RM_Rx_date_NULL=i;//抄表接收返回:0=有数据,!=0无数据(空数据)
						if(i==2)
						{//2控制码错(无数据)
//							UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
//							UARTCtrl->ReRMCount=0;//抄表重发计数
//							Save_RMCount(PORTn,RMListNO);//得到抄表计数相关数据;入口:SetNo=档案号从0开始
//							goto RXESC;
						#if (USER/100)==15//安徽
							//安徽:针对三相表需要支持06000001和061000FF等块抄读
							if(UARTCtrl->RM_DI==0x06000001)
							{
								//UARTCtrl->Task=2;//2=接收超时(或错误)
								Save_RMCount(PORTn,RMListNO);//得到抄表计数相关数据;入口:SetNo=档案号从0开始
								goto RXESC;
							}
						#endif
							//曲线等冻结数据当指定时间点无数据冻结时写入空数据避免补抄时重新抄读
							i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
							Info=(UARTCtrl->TaskID>>20)&0xf;//采集类型
							if(!((i==0x5002)||(Info==4)))
							{//不是抄分钟冻结(采曲线数据)和补抄(2=事件采集方案时会填入空数据而上报)
								UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
								UARTCtrl->ReRMCount=0;//抄表重发计数
								Save_RMCount(PORTn,RMListNO);//得到抄表计数相关数据;入口:SetNo=档案号从0开始
								goto RXESC;
							}
						}
						RM_RxDATA(PORTn);//抄表接收数据处理
						RM_State_RMUpDate(PORTn,1,SetNo,(Info>>4)&0xf,(Info>>16)&0xf);//采集状态,每次抄表后更新:入口:OK=0失败=1成功,OK=1时Class=中继级别,Phase=相位
						
						UARTCtrl->ReRMCount=0;//抄表重发计数
						UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
						Save_RMCount(PORTn,RMListNO);//得到抄表计数相关数据;入口:SetNo=档案号从0开始
						//发确认回答上报抄表数据
					RXESC:
						MWR(0xffffffff,((u32)p8tx)+128,6);
						Uart_3762_TxData(PORTn,0,1,6);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
						if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
						{
							p8tx[3]=0x03;//控制码;集中式路由载波通信
						}
						else
						{
							if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{
								p8tx[3]=0x0a;//控制码;微功率无线通信
							}
							else
							{
								p8tx[3]=0x01;//控制码;集中式路由载波通信
							}
						}
						Uart_3762_CS((u32)p8tx);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
						break;
					default:
						UARTCtrl->RxByte=0;//接收字节数(半字对齐)
						UARTCtrl->Task=5;//正在接收
						break;
				}
			}
			return;
	}
}
#endif


