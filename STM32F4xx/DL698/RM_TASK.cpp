
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"
#include "../DL698/EVENT_Record.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../Terminal698/Terminal_Init.h"
#include "../DL698/DL698TEST.h"
#include "../DL698/Info_3762.h"

#include "../DL698/DL698_JSON.h"


/*
#define ADDR_6034     ADDR_6032+LENmax_6032//采集任务监控集
//0 采集任务监控单元∷=structure
//{
//2 任务ID       unsigned
//4 任务执行状态  enum
//{
//	未执行（0）
//	执行中（1）
//	已执行（2）
//}，
//6 任务执行开始时间   date_time_s，//用各端口时间值小的
//14 任务执行结束时间   date_time_s，//用各端口时间值大的
//22 采集总数量         long-unsigned，
//25 采集成功数量       long-unsigned，
//28 已发送报文条数     long-unsigned，
//31 已接收报文条数     long-unsigned
//}
//说明：采集成功数量、已发送报文条数、已接收报文条数，每次任务执行先清零。
#define LENper_6034     34
#define NUMmax_6034     NUMmax_6012_2
#define LENmax_6034     (LENper_6034*NUMmax_6034)
*/


//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//优先级:1=首要,2=必要,3=需要,4=可能
//方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案,扩展14=终端事件(不上报),15=终端事件(上报)
//普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集
//普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标,7=相对上月月末23点59分





u32 RM_Addr(u32 PORTn,u16* pAddr)//指针指向的通信地址是否已抄;返回:0=没抄,1=已抄
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	RMokfailLIST_TypeDef *rmOKFAILlist;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;
	if(i>=(RMTASKmax+ExtRMTASKmax))
	{
		return 0;//返回:0=没抄,1=已抄
	}
	rmOKFAILlist=UARTCtrl->rmOKFAILlist[i];
	i=UARTCtrl->NUMrmOK[i];//各任务已采集完成电能表总数
	if(i>NUM_RMmax)
	{
		i=NUM_RMmax;
	}
	Addr0=pAddr[0];
	Addr1=pAddr[1];
	Addr2=pAddr[2];
	while(i--)
	{
		if(Addr0==rmOKFAILlist->Addr0)
		{
			if(Addr1==rmOKFAILlist->Addr1)
			{
				if(Addr2==rmOKFAILlist->Addr2)
				{
					return 1;//返回:0=没抄,1=已抄
				}
			}
		}
		rmOKFAILlist++;
	}
	return 0;
}

u32 RM_MeterCount(u32 PORTn)//电能表计数+1;返回:0=正常,1=任务错
{
	u32 i;
	u32 Nmax;
	u32 id;
	UARTCtrl_TypeDef *UARTCtrl;
	u16* p16Data;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->CONNECT=0;//DL698电能表连接:0-100连接过程序,100=连接,101=
	p16Data=(u16 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data[3]=0;//长度=时标+已采集的CSD
	p16Data+=LEN_RM_DI_CSD/2;
	p16Data[0]=0;//长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
	p16Data[1]=0;//长度=已填写入buff完整CSD的长度(不包括本身2字节)
	
	id=UARTCtrl->TaskID;
	if(id==0)
	{
		return 1;//返回:0=正常,1=任务错
	}
	id>>=8;
	id&=0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return 1;//返回:0=正常,1=任务错
	}
#if RM_HPLC_Concurrency==1//0=否,1=是;宽带载波集中器主动并发抄表
	if(PORTn==RS485_4PORT)
	{
		if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{//是宽带
			//UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//并发处理时当前任务id的原始抄表电能表计数
		}
	}
#endif
	i=UARTCtrl->MeterCount[id];//各任务已抄电能表计数
	i++;
	UARTCtrl->MeterCount[id]=i;
	
	Nmax=NUM_RMmax;
	switch(id)
	{
		case RMTASKnoPOWER://停上电事件固定任务号
			Nmax=PowerEvent->NUMrm;//停上电事件需抄读电表个数
			if(Nmax>NUM_RMmax)
			{
				Nmax=NUM_RMmax;
			}
			if(i>=Nmax)
			{
				i=UARTCtrl->RMLOOP[id];
				if(i!=0xff)
				{//RMTASKmax byte 全部表抄读一遍的遍数计数
					i++;
					UARTCtrl->RMLOOP[id]=i;
				}
			}
			break;
	}
	if(i>=Nmax)
	{
		UARTCtrl->MeterCount[id]=0;//各任务已抄电能表计数
//		i=UARTCtrl->TaskTime[id].StdTimeCount;//每到频率点标准时间计数+1,每执行一次-1
//		if(i)
//		{
//			i--;
//			UARTCtrl->TaskTime[id].StdTimeCount=i;
//		}
	}
#if RM_HPLC_Concurrency==1//0=否,1=是;宽带载波集中器主动并发抄表
	if(PORTn==RS485_4PORT)
	{
		if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
		{//是宽带
			UARTCtrl->SourMeterCount_id=UARTCtrl->MeterCount[id];//并发处理时当前任务id的原始抄表电能表计数
		}
	}
#endif
	return 0;//返回:0=正常,1=任务错
}


u16* MeterSetNoToCollectorAddr(u32 MeterSetNo)//取采集档案配置序号的采集器地址;入口:从0开始的序号;返回:采集器地址指针,0=无效
{
	u8* p8;
	
	p8=(u8*)ADDR_6000_SDRAM+(MeterSetNo*LENper_6000);
	p8=Get_Element(p8,3,0,0);//计算元素地址,使用ADDR_128KDATABUFF
	if(p8==0)
	{
		return 0;
	}
	return (u16*)(p8+3);
}

void NUMrm(u32 PORTn)//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
{
	u32 TaskID;
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 id;
	u32 OADPORTn;
	u32 NUMrm;
	u32 filename;
	u8* pMS;
	u8 *p8s;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	ms_Type *ms;
	msFILE_TypeDef *file;
	RecordFileHead_TypeDef* RecordFileHead;//抄表记录文件头
	RMokfailLIST_TypeDef *rmOKFAILlist;

	
	ms=Get_ms();
//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//优先级:1=首要,2=必要,3=需要,4=可能
//方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案,扩展14=终端事件(不上报),15=终端事件(上报)
//普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集,4=补抄
//普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	TaskID=UARTCtrl->TaskID;
	if(TaskID==0)
	{
		return;
	}
	id=(TaskID>>8)&0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return;
	}
	UARTCtrl->NUMrmTOTAL[id]=0;//任务需采集总数
	UARTCtrl->NUMrmOK[id]=0;//各任务已采集完成电能表数
	UARTCtrl->NUMrmBAD[id]=0;//各任务已采集完成的电能表数中因配置变更等原因不再需抄读的数
	//UARTCtrl->NUMrmFAIL[id]=0;//各任务已采集失败电能表数

	
//本端口在档案中配置的仪表总数
	NUMrm=0;//配置的仪表总数
	OADPORTn=PORTntoOAD(PORTn);
	for(i=0;i<NUM_RMmax;i++)
	{
		p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
		if(p8s[0]==DataType_structure)
		{
			//端口
			p8d=Get_Element(p8s+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8d)
			{
				x=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
				if(x==OADPORTn)
				{
					NUMrm++;//配置的仪表总数
				}
			}
		}
	}
	UARTCtrl->NUM_SetMeter=NUMrm;//本端口在档案中配置的仪表总数
//各任务
	switch(id)
	{
		case RMTASKnoPOWER://停上电事件固定任务号
			NUMrm=0;//需采集总数
			OADPORTn=PORTntoOAD(PORTn);
			for(i=0;i<PowerEvent->NUMrm;i++)
			{
				x=PowerEvent->setno[i];
				p8s=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
				//端口
				p8d=Get_Element(p8s+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8d)
				{
					x=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
					if(x==OADPORTn)
					{
						NUMrm++;//需采集总数
					}
				}
			}
			UARTCtrl->NUMrmTOTAL[id]=NUMrm;//任务需采集总数
			return;
	}
	
	i=TaskID&0xff;//方案编号
	switch((TaskID>>24)&0xf)//方案类型
	{
		case 1://普通采集方案
			i*=LENper_6014;
			i+=ADDR_6014_SDRAM;
			p8s=(u8*)i;
			if(p8s[0]!=DataType_structure)
			{
				return;
			}
			pMS=Get_Element(p8s,5,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(pMS==0)
			{
				return;
			}
/*
			//记录列选择CSD, 本函数中DateSaveTI()要用
			p8d=Get_Element(p8s,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8d==0)
			{
				return;
			}
			i=UARTCtrl->RMCSDCount;//抄表CSD计数
			p8d=Get_Element(p8d,i+1,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8d==0)
			{
				return;
			}
			i=(p8d[2]<<8)|(p8d[3]<<0);//采集数据的OI
			UARTCtrl->RMCSD[2]=i>>8;//OI
			UARTCtrl->RMCSD[3]=i;//OI
			switch(i)
			{
				case 0x5002://分钟冻结,采曲线数据,因会补抄上96点故用先用无效时间没相同文件名使已抄数为0,在RM_TxDATA.c中会每一点检查是否已抄
					UARTCtrl->RMCSD[2]=0xff;//无效的采集数据的OI,下DateSaveTI()要用
					UARTCtrl->RMCSD[3]=0xff;//无效的采集数据的OI,下DateSaveTI()要用
					break;
//				case 0x5004://日冻结
//				case 0x5006://月冻结
//				default:
//					break;
			}
*/
/*
			UARTCtrl->RMCSD[2]=0xff;//无效的采集数据的OI,下DateSaveTI()要用
			UARTCtrl->RMCSD[3]=0xff;//无效的采集数据的OI,下DateSaveTI()要用
*/
			break;
		case 2://事件采集方案
			i*=LENper_6016;
			i+=ADDR_6016_SDRAM;
			p8s=(u8*)i;
			if(p8s[0]!=DataType_structure)
			{
				return;
			}
			pMS=Get_Element(p8s,3,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(pMS==0)
			{
				return;
			}
			break;
		case 3://透明方案
#if LENper_6018>LEN_128KDATABUFF
	#error
#endif
//#define ADDR_6018     ADDR_6016+LENmax_6016//透明方案集(初始化值0x00)
//属性2（透明方案）∷=structure//删除时structure的数据单元清为0(NULL)
//{
//	方案编号     unsigned，
//	方案内容集   array 方案内容，
			i*=LENper_6018;
			i+=ADDR_6018;
			MR(ADDR_128KDATABUFF,i,8);
			p8s=(u8*)ADDR_128KDATABUFF;
			if(p8s[0]!=DataType_structure)
			{
				return;
			}
			p8s+=4;
			x=p8s[1];
			if(x&0x80)
			{
				x=(p8s[2]<<8)+p8s[3];
				p8s+=4;
			}
			else
			{
				p8s+=2;
			}
			if(x>NUM_RMmax)
			{
				x=0;
			}
			UARTCtrl->NUMrmTOTAL[id]=x;//任务需采集总数
			//读文件已抄地址列表,原已采集完成数
			i=Get_FileID(PORTn);//UART各任务的FileID
			Comm_Ram->msFILESearchCount=0;
			filename=Get_RecordFileName(i,0,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件计数,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
			if(filename!=0xffffffff)
			{
				if(filename<FILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
				}
				else
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
				}
				y=file->NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
				if(y>NUM_RMmax)
				{
					y=0;
				}
				i=UARTCtrl->NUMrmFAIL[id];//各任务已采集失败电能表数
				if((y+i)>NUM_RMmax)
				{
					UARTCtrl->NUMrmFAIL[id]=NUM_RMmax-y;//各任务已采集失败电能表数
				}
				UARTCtrl->NUMrmOK[id]=y;//原已采集完成数
				ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
				RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
				rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
				for(i=0;i<y;i++)
				{
					rmOKFAILlist->Addr0=RecordFileHead->Addr0;
					rmOKFAILlist->Addr1=RecordFileHead->Addr1;
					rmOKFAILlist->Addr2=RecordFileHead->Addr2;
					rmOKFAILlist++;
					RecordFileHead++;
				}
			}
			//计算原已采集完成数中的非法数
			p8d=p8s;//指向内容集
			y=UARTCtrl->NUMrmOK[id];//原已采集完成数
			if(y>=NUM_RMmax)
			{
				y=0;
			}
			NUMrm=0;//采集完成数中的非法数
			rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
			while(y--)
			{
				p8s=p8d;//恢复p8s
				//x=内容集数组个数
				for(i=0;i<x;i++)
				{
					OADPORTn=p8s[4]|(p8s[5]<<8);
					if(rmOKFAILlist->Addr0==OADPORTn)
					{
						OADPORTn=p8s[6]|(p8s[7]<<8);
						if(rmOKFAILlist->Addr1==OADPORTn)
						{
							OADPORTn=p8s[8]|(p8s[9]<<8);
							if(rmOKFAILlist->Addr2==OADPORTn)
							{
								break;
							}
						}
					}
					OADPORTn=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					p8s+=OADPORTn;
				}
				if(i>=x)
				{//没找到相同地址
					NUMrm++;//采集完成数中的非法数
				}
				rmOKFAILlist++;
			}
			UARTCtrl->NUMrmBAD[id]=NUMrm;//采集完成数中的非法数
			//删除已采集失败数中的非法数
			rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
			rmOKFAILlist--;
			y=UARTCtrl->NUMrmFAIL[id];//原已采集失败数
			if(y>=NUM_RMmax)
			{
				y=0;
			}
			for(i=0;i<y;i++)
			{
				x=AddrToMeterSetNo((u16*)rmOKFAILlist);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
				if(x<NUM_RMmax)
				{
					p8s=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
					if(p8s[0]==DataType_structure)
					{
						p8d=Get_Element(p8s+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
						if(p8d)
						{
							n=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
							if(n==OADPORTn)
							{
								x=Compare_SETNO_MS((u8 *)ADDR_6000_SDRAM,x,pMS);//比较配置序号是否符合MS集合,入口:pADDR_6000已在RAM,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
								if(x)
								{
									rmOKFAILlist--;
									continue;
								}
							}
						}
					}
				}
				#if (LEN_RMokfailLIST*NUM_RMmax)>LEN_128KDATABUFF
					#error
				#endif
				MR(ADDR_128KDATABUFF,((u32)rmOKFAILlist)-((y-(i+1))*sizeof(RMokfailLIST_TypeDef)),(y-(i+1))*sizeof(RMokfailLIST_TypeDef));
				MW(ADDR_128KDATABUFF,(u32)rmOKFAILlist-((y-(i+1))*sizeof(RMokfailLIST_TypeDef))+sizeof(RMokfailLIST_TypeDef),(y-(i+1))*sizeof(RMokfailLIST_TypeDef));
				y--;
				rmOKFAILlist--;
			}
			UARTCtrl->NUMrmFAIL[id]=y;//原已采集失败数
			return;
		case 4://上报方案
		case 5://脚本方案
		default:
			break;
	}
	i=(TaskID>>24)&0xf;
	if(i<=2)
	{//1=普通采集方案,2=事件采集方案;(3=透明方案上已列表)
		pMS++;//指向数据类型后的字节
		NUMrm=0;//需采集总数
		OADPORTn=PORTntoOAD(PORTn);
		p8s=(u8*)ADDR_6000_SDRAM;
		for(i=0;i<NUMmax_6000;i++)
		{
			if(p8s[0]==DataType_structure)
			{
				p8d=Get_Element(p8s+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8d)
				{
					x=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
					if(x==OADPORTn)
					{
						x=Compare_SETNO_MS((u8*)ADDR_6000_SDRAM,i,pMS);//比较配置序号是否符合MS集合,入口:pADDR_6000已在RAM,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
						if(x)
						{
							NUMrm++;//需采集总数
						}
					}
				}
			}
			p8s+=LENper_6000;
		}
		UARTCtrl->NUMrmTOTAL[id]=NUMrm;//2byte任务需采集总数
		//读文件已抄地址列表,原已采集完成数
		Comm_Ram->msFILESearchCount=0;
		i=(TaskID>>24)&0xf;
		if(i==1)
		{//1=普通采集方案
			DateSaveTI(PORTn,UARTCtrl->date_time_s);//普通采集方案的采集存储时标;返回:pOUT=date_time_s
			i=Get_FileID(PORTn);//UART各任务的FileID
			filename=Get_RecordFileName(i,0xffffffff,UARTCtrl->date_time_s);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件计数,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
		}
		else
		{//2=事件采集方案
			DateSaveTI(PORTn,UARTCtrl->date_time_s);//普通采集方案的采集存储时标;返回:pOUT=date_time_s
			i=Get_FileID(PORTn);//UART各任务的FileID
			filename=Get_RecordFileName(i,0,UARTCtrl->date_time_s);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件计数,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
		}
		if(filename!=0xffffffff)
		{
			if(filename<FILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			y=file->NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
			if(y>NUM_RMmax)
			{
				y=0;
			}
			i=UARTCtrl->NUMrmFAIL[id];//各任务已采集失败电能表数
			if((y+i)>NUM_RMmax)
			{
				UARTCtrl->NUMrmFAIL[id]=NUM_RMmax-y;//各任务已采集失败电能表数
			}
			UARTCtrl->NUMrmOK[id]=y;//原已采集完成数
			ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
			rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
			for(i=0;i<y;i++)
			{
				rmOKFAILlist->Addr0=RecordFileHead->Addr0;
				rmOKFAILlist->Addr1=RecordFileHead->Addr1;
				rmOKFAILlist->Addr2=RecordFileHead->Addr2;
				rmOKFAILlist++;
				RecordFileHead++;
			}
		}
		NUMrm=0;//采集完成数中的非法数
		//计算原已采集完成数中的非法数
		y=UARTCtrl->NUMrmOK[id];//原已采集完成数
		if(y>NUM_RMmax)
		{
			y=0;
		}
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
		for(i=0;i<y;i++)
		{
			x=AddrToMeterSetNo((u16*)rmOKFAILlist);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
			if(x<NUM_RMmax)
			{
				p8s=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
				if(p8s[0]==DataType_structure)
				{
					p8d=Get_Element(p8s+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8d)
					{
						n=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
						if(n==OADPORTn)
						{
							x=Compare_SETNO_MS((u8 *)ADDR_6000_SDRAM,x,pMS);//比较配置序号是否符合MS集合,入口:pADDR_6000已在RAM,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
							if(x)
							{
								rmOKFAILlist++;
								continue;
							}
						}
					}
				}
			}
			rmOKFAILlist++;
			NUMrm++;//采集完成数中的非法数
		}
		UARTCtrl->NUMrmBAD[id]=NUMrm;//非法数
		//删除已采集失败数中的非法数
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
		rmOKFAILlist--;
		y=UARTCtrl->NUMrmFAIL[id];//原已采集失败数
		if(y>=NUM_RMmax)
		{
			y=0;
		}
		for(i=0;i<y;i++)
		{
			x=AddrToMeterSetNo((u16*)rmOKFAILlist);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
			if(x<NUM_RMmax)
			{
				p8s=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
				if(p8s[0]==DataType_structure)
				{
					p8d=Get_Element(p8s+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8d)
					{
						n=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
						if(n==OADPORTn)
						{
							x=Compare_SETNO_MS((u8 *)ADDR_6000_SDRAM,x,pMS);//比较配置序号是否符合MS集合,入口:pADDR_6000已在RAM,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
							if(x)
							{
								rmOKFAILlist--;
								continue;
							}
						}
					}
				}
			}
			#if (LEN_RMokfailLIST*NUM_RMmax)>LEN_128KDATABUFF
				#error
			#endif
			MR(ADDR_128KDATABUFF,((u32)rmOKFAILlist)-((y-(i+1))*sizeof(RMokfailLIST_TypeDef)),(y-(i+1))*sizeof(RMokfailLIST_TypeDef));
			MW(ADDR_128KDATABUFF,(u32)rmOKFAILlist-((y-(i+1))*sizeof(RMokfailLIST_TypeDef))+sizeof(RMokfailLIST_TypeDef),(y-(i+1))*sizeof(RMokfailLIST_TypeDef));
			y--;
			rmOKFAILlist--;
		}
		UARTCtrl->NUMrmFAIL[id]=y;//原已采集失败数
	}
	
//FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注
//FileID采集类型 3=按时标间隔采集(曲线数据)
//TaskID采集类型 4=补抄
	i=Get_FileID(PORTn);//UART各任务的FileID
	if((((i>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
	{//原已采集完成数=0;抄曲线和补抄(日冻结和曲线)需抄多个点,在发送管理时识别各点是否已抄
		UARTCtrl->NUMrmOK[id]=0;//原已采集完成数
	}
	
}

void RM_OK_FAIL(u32 PORTn,u32 OK)//抄表成功失败地址记录:入口:OK=0失败，=1成功,2=成功但数据为空
{
	u32 i;
	u32 x;
	u32 n;
	u32 m;
	u32 id;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	EventSour_typedef* EventSour;
	RMokfailLIST_TypeDef *rmOKFAILlist;
	UARTCtrl_TypeDef *UARTCtrl;
	
	
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
	if(OK==1)
	{//成功
		n=UARTCtrl->NUMrmOK[id];
		if(n>=NUM_RMmax)
		{
			return;
		}
		//查原是否成功记录
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
		for(i=0;i<n;i++)
		{
			if(rmOKFAILlist->Addr0==Addr0)
			{
				if(rmOKFAILlist->Addr1==Addr1)
				{
					if(rmOKFAILlist->Addr2==Addr2)
					{
						break;
					}
				}
			}
			rmOKFAILlist++;
		}
		if(i>=n)
		{//没
			rmOKFAILlist->Addr0=Addr0;
			rmOKFAILlist->Addr1=Addr1;
			rmOKFAILlist->Addr2=Addr2;
			n++;
			UARTCtrl->NUMrmOK[id]=n;
			//查原是否失败记录
			m=UARTCtrl->NUMrmFAIL[id];
			if((n+m)>NUM_RMmax)
			{
				m=NUM_RMmax-n;//各任务已采集失败电能表数
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
							break;
						}
					}
				}
				rmOKFAILlist--;
			}
			if(i<m)
			{//有
				#if (LEN_RMokfailLIST*NUM_RMmax)>LEN_128KDATABUFF
					#error
				#endif
				MR(ADDR_128KDATABUFF,((u32)rmOKFAILlist)-((m-(i+1))*sizeof(RMokfailLIST_TypeDef)),(m-(i+1))*sizeof(RMokfailLIST_TypeDef));
				MW(ADDR_128KDATABUFF,(u32)rmOKFAILlist-((m-(i+1))*sizeof(RMokfailLIST_TypeDef))+sizeof(RMokfailLIST_TypeDef),(m-(i+1))*sizeof(RMokfailLIST_TypeDef));
				m--;
				UARTCtrl->NUMrmFAIL[id]=m;
			}
		}
		//终端抄表失败事件
		if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
		{
			i=MRR(ADDR_310F_6+5,1);//关联采集任务号
			if(i==id)
			{
				EventSour=GET_EventSour((u16*)UARTCtrl->RMAddr);//得到事件原值数据指针，入口:抄表地址, 返回:数据指针,0=错误
				if(EventSour)
				{
					EventSour->Re=0;//抄表失败(310F)重试轮次
					if((EventSour->Occur&1)==1)//b0=抄表失败(310F)
					{
						EventSour->Occur&=~1;//b0=抄表失败(310F)
						p8=(u8*)ADDR_128KDATABUFF+(64*1024);
						p8[0]=0;//事件发生前正向有功总电能=空
						p8[1024]=0;//事件结束后正向有功总电能=空
						p8+=512;
						p8[0]=DataType_TSA;//产生源
						p8[1]=7;
						p8[2]=5;
						p8[3]=Addr0;
						p8[4]=Addr0>>8;
						p8[5]=Addr1;
						p8[6]=Addr1>>8;
						p8[7]=Addr2;
						p8[8]=Addr2>>8;
						Event_OAD(0x310F0200,p8,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					}
				}
			}
		}
		//RS485设备故障事件记录恢复
		if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
		{
			if(UARTCtrl->EVENT_FAIL==1)//设备故障事件记录(485或载波通道),0=空或恢复,1=产生
			{
				UARTCtrl->EVENT_FAIL=0;//设备故障事件记录(485或载波通道),0=空或恢复,1=产生
				p8=(u8*)ADDR_128KDATABUFF+(64*1024);
				p8[0]=0;//事件发生前正向有功总电能=空
				p8[1024]=0;//事件结束后正向有功总电能=空
				p8+=512;
				p8[0]=DataType_enum;//产生源
				p8[1]=3;//485抄表故障    （3）
				Event_OAD(0x310A0200,p8,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			}
		}
	}
	else
	{//失败或空数据
		//查原是否成功记录
		n=UARTCtrl->NUMrmOK[id];
		if(n>=NUM_RMmax)
		{
			return;
		}
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
		for(i=0;i<n;i++)
		{
			if(rmOKFAILlist->Addr0==Addr0)
			{
				if(rmOKFAILlist->Addr1==Addr1)
				{
					if(rmOKFAILlist->Addr2==Addr2)
					{
						return;
					}
				}
			}
			rmOKFAILlist++;
		}
		//查原是否失败记录
		m=UARTCtrl->NUMrmFAIL[id];
		if((n+m)>NUM_RMmax)
		{
			m=NUM_RMmax-n;//各任务已采集失败电能表数
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
						rmOKFAILlist->ReRMCount=UARTCtrl->ReRMCount;
						rmOKFAILlist->SpaceData=OK;//0=失败,1=,2=抄表正常但数据为空标志失败(台体测试采集监控计为抄表失败)
						break;
					}
				}
			}
			rmOKFAILlist--;
		}
		if(i>=m)
		{//没
			if((n+m)>=NUM_RMmax)
			{
				return;
			}
			rmOKFAILlist->Addr0=Addr0;
			rmOKFAILlist->Addr1=Addr1;
			rmOKFAILlist->Addr2=Addr2;
			rmOKFAILlist->ReRMCount=UARTCtrl->ReRMCount;
			rmOKFAILlist->SpaceData=OK;//0=失败,1=,2=抄表正常但数据为空标志失败(台体测试采集监控计为抄表失败)
			m++;
			UARTCtrl->NUMrmFAIL[id]=m;
			//终端抄表失败事件
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{
				i=MRR(ADDR_310F_6+3,3);//重试轮次,关联采集任务号
				if((i>>16)==id)
				{
					i&=0xff;//参数中的重试轮次
					EventSour=GET_EventSour((u16*)UARTCtrl->RMAddr);//得到事件原值数据指针，入口:抄表地址, 返回:数据指针,0=错误
					if(EventSour)
					{
						if(EventSour->Re>=i)
						{
							if((EventSour->Occur&1)==0)//b0=抄表失败(310F)
							{
								EventSour->Occur|=1;//b0=抄表失败(310F)
								p8=(u8*)ADDR_128KDATABUFF+(64*1024);
								p8[0]=0;//事件发生前正向有功总电能=空
								p8[1024]=0;//事件结束后正向有功总电能=空
								p8+=512;
								p8[0]=DataType_TSA;//产生源
								p8[1]=7;
								p8[2]=5;
								p8[3]=Addr0;
								p8[4]=Addr0>>8;
								p8[5]=Addr1;
								p8[6]=Addr1>>8;
								p8[7]=Addr2;
								p8[8]=Addr2>>8;
								Event_OAD(0x310F0200,p8,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
							}
						}
						if(EventSour->Re!=0xff)//抄表失败(310F)重试轮次
						{
							EventSour->Re++;//抄表失败(310F)重试轮次
						}
					}
				}
			}
		}
		//RS485设备故障事件记录产生
		if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
		{
			if(UARTCtrl->EVENT_FAIL==0)//设备故障事件记录(485或载波通道),0=空或恢复,1=产生
			{
			#if LEN_DATABUFF<NUM_RMmax
				#error
			#endif
				MC(0,ADDR_DATABUFF,NUM_RMmax);
				p8=(u8*)ADDR_DATABUFF;
				n=0;//总有失败表数
				for(id=1;id<RMTASKmax;id++)
				{
					//统计失败记录的各表数
					m=UARTCtrl->NUMrmFAIL[id];
					if(m>NUM_RMmax)
					{
						m=NUM_RMmax;
					}
					rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
					rmOKFAILlist--;
					for(i=0;i<m;i++)
					{
						x=AddrToMeterSetNo((u16*)rmOKFAILlist);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
						if(x<NUM_RMmax)
						{
							if(p8[x]==0)
							{
								p8[x]=1;
								n++;//总有失败表数+1
							}
						}
						rmOKFAILlist--;
					}
				}
				if(n>=UARTCtrl->NUM_SetMeter)//本端口在档案中配置的仪表总数
				{
					UARTCtrl->EVENT_FAIL=1;//设备故障事件记录(485或载波通道),0=空或恢复,1=产生
					p8=(u8*)ADDR_128KDATABUFF+(64*1024);
					p8[0]=0;//事件发生前正向有功总电能=空
					p8[1024]=0;//事件结束后正向有功总电能=空
					p8+=512;
					p8[0]=DataType_enum;//产生源
					p8[1]=3;//485抄表故障    （3）
					Event_OAD(0x310A0200,p8,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
		}
	}
}

void RM_ADDR_Display(u32 PORTn)//抄表地址显示
{
	u32 i;
	u32 x;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8=(u8*)ADDR_DATABUFF;
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
	{
		CopyString((u8 *)"485     ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		if(PORTn==RS485_1PORT)
		{
			i=1;
		}
		else
		{
			i=2;
		}
		p8[3]=i;
	}
	else
	{
		if(PORTn==ACSAMPLEPORT)
		{//交采
			CopyString((u8 *)"交采     ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		else
		{
			CopyString((u8 *)"载波     ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
	}
//广播校时任务
	if((UARTCtrl->BroadcastTimeTask&0x7f)>=2)//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
	{//正在执行广播校时
		CopyString((u8 *)"广播校时",(u8*)ADDR_DATABUFF+6);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		p8[14]=0;
		WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
		return;
	}
//任务号	
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;
	i=hex_bcd(i);
	if(i<0x10)
	{
		p8[5]=i+0x30;
	}
	else
	{
		if(i<0x100)
		{
			p8[5]=(i>>4)+0x30;
			p8[6]=(i&0xf)+0x30;
		}
		else
		{
			p8[5]=((i>>8)&0xf)+0x30;
			p8[6]=((i>>4)&0xf)+0x30;
			p8[7]=(i&0xf)+0x30;
		}
	}
//电能表主动上报任务
	if(UARTCtrl->AutoReportTask>=2)//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
	{
		CopyString((u8 *)"上报",(u8*)ADDR_DATABUFF+4);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
	}
//12位地址
	p8+=8;
	for(i=0;i<3;i++)
	{
		x=UARTCtrl->RMAddr[i];
		if(((x>>4)&0xf)<10)
		{
			p8[0]=((x>>4)&0xf)+0x30;
		}
		else
		{	
			p8[0]=((x>>4)&0xf)+55;
		}
		p8++;
		if((x&0xf)<10)
		{
			p8[0]=(x&0xf)+0x30;
		}
		else
		{
			p8[0]=(x&0xf)+55;
		}
		p8++;
		x>>=8;
		if(((x>>4)&0xf)<10)
		{
			p8[0]=((x>>4)&0xf)+0x30;
		}
		else
		{	
			p8[0]=((x>>4)&0xf)+55;
		}
		p8++;
		if((x&0xf)<10)
		{
			p8[0]=(x&0xf)+0x30;
		}
		else
		{
			p8[0]=(x&0xf)+55;
		}
		p8++;
	}
	p8[0]=0;
	WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
}

void PROXY_ADDR_Display(u32 PORTn)//代理地址显示
{
	u32 i;
	u32 x;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlProxy;
	
	if(PORTn==ACSAMPLEPORT)
	{//交采
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8=(u8*)ADDR_DATABUFF;
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
	{
		CopyString((u8 *)"485  转  ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		if(PORTn==RS485_1PORT)
		{
			i=1;
		}
		else
		{
			i=2;
		}
		p8[3]=i;
	}
	else
	{
		CopyString((u8 *)"载波 转  ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
	}
	UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(UARTCtrl->ProxyPORTn);
	p8+=8;
	for(i=0;i<3;i++)
	{
		x=UARTCtrlProxy->RMAddr[i];
		if(((x>>4)&0xf)<10)
		{
			p8[0]=((x>>4)&0xf)+0x30;
		}
		else
		{	
			p8[0]=((x>>4)&0xf)+55;
		}
		p8++;
		if((x&0xf)<10)
		{
			p8[0]=(x&0xf)+0x30;
		}
		else
		{
			p8[0]=(x&0xf)+55;
		}
		p8++;
		x>>=8;
		if(((x>>4)&0xf)<10)
		{
			p8[0]=((x>>4)&0xf)+0x30;
		}
		else
		{	
			p8[0]=((x>>4)&0xf)+55;
		}
		p8++;
		if((x&0xf)<10)
		{
			p8[0]=(x&0xf)+0x30;
		}
		else
		{
			p8[0]=(x&0xf)+55;
		}
		p8++;
	}
	p8[0]=0;
	WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
}



void RM_NUM_Display(u32 PORTn)//抄表数量显示
{
	u32 i;
	u32 x;
	u32 id;
	u8* p8;
	u32 TaskID;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//id=UARTCtrl->LastTaskID;//最近上次有效采集任务ID,用于显示等
	id=UARTCtrl->TaskID;
	id>>=8;
	id&=0xff;
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
	{
		//CopyString((u8 *)"485x____成____败____",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		CopyString((u8 *)"485x____ 成功____",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		p8=(u8*)(ADDR_DATABUFF);
		if(PORTn==RS485_1PORT)
		{
			i=1;
		}
		else
		{
			i=2;
		}
		p8[3]=i;
	}
	else
	{
		if(PORTn==ACSAMPLEPORT)
		{//交采
			CopyString((u8 *)"交采____ 成功____",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
		else
		{
			//CopyString((u8 *)"载波____成____败____",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			CopyString((u8 *)"载波____ 成功____",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
		}
	}
	//总数
	p8=(u8*)(ADDR_DATABUFF+4+3);
	p8[0]='0';
	i=UARTCtrl->NUMrmTOTAL[id];
	i=hex_bcd(i);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	//成功数
	p8=(u8*)(ADDR_DATABUFF+13+3);
	p8[0]='0';
	TaskID=UARTCtrl->TaskID;
	i=Get_FileID(PORTn);//UART各任务的FileID
	if((((i>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
	{//原已采集完成数=0;抄曲线和补抄(日冻结和曲线)需抄多个点,在发送管理时识别各点是否已抄
		//UARTCtrl->NUMrmOK[id]=0;//原已采集完成数
		i=UARTCtrl->NUMrmOK[id];
	}
	else
	{
		i=UARTCtrl->NUMrmOK[id];
		x=UARTCtrl->NUMrmBAD[id];
		if(i>=x)
		{
			i-=x;
		}
		else
		{
			i=0;
		}
	}
	i=hex_bcd(i);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	p8=(u8*)(ADDR_DATABUFF+17);
	p8[0]=0;
	//失败数
/*
	p8=(u8*)(ADDR_DATABUFF+16+3);
	p8[0]='0';
	p8[1]=0;
	i=UARTCtrl->NUMrmFAIL[id];
	i=hex_bcd(i);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
*/
	if(PORTn==RS485_4PORT)
	{//载波口
		WarningOccur(100,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
	}
	else
	{
		WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
	}
}


void RM_Monitor_Init(u32 ID)//采集任务监控初始化;入口:ID=任务号
{
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl_1;
	UARTCtrl_TypeDef *UARTCtrl_2;
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	UARTCtrl_TypeDef *UARTCtrl_4;
#endif
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	UARTCtrl_1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	UARTCtrl_2=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	UARTCtrl_4=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
#endif
	if(UARTCtrl_1->TaskTime[ID].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行
	{
		return;
	}
	if(UARTCtrl_2->TaskTime[ID].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行
	{
		return;
	}
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(UARTCtrl_4->TaskTime[ID].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行
	{
		return;
	}
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
		if(UARTCtrl_4->TaskTime[ID].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行
		{
			return;
		}
	}
#endif
	UARTCtrl_1->TaskTime[ID].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行
	UARTCtrl_2->TaskTime[ID].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	UARTCtrl_4->TaskTime[ID].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行
#endif
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
//0 采集任务监控单元∷=structure
	p8[0]=DataType_structure;
	p8[1]=8;
//{
//2 任务ID       unsigned
	p8[2]=DataType_unsigned;
	p8[3]=ID;
//4 任务执行状态  enum
//{
//	未执行（0）
//	执行中（1）
//	已执行（2）
//}，
	p8[4]=DataType_enum;
	p8[5]=0;
//6 任务执行开始时间   date_time_s，//用各端口时间值小的
	p8[6]=DataType_date_time_s;
	p8[7]=0xff;
	p8[8]=0xff;
	p8[9]=0xff;
	p8[10]=0xff;
	p8[11]=0xff;
	p8[12]=0xff;
	p8[13]=0xff;
//14 任务执行结束时间   date_time_s，//用各端口时间值大的
	p8[14]=DataType_date_time_s;
	p8[15]=0xff;
	p8[16]=0xff;
	p8[17]=0xff;
	p8[18]=0xff;
	p8[19]=0xff;
	p8[20]=0xff;
	p8[21]=0xff;
//22 采集总数量         long-unsigned，
	p8[22]=DataType_long_unsigned;
	p8[23]=0;
	p8[24]=0;
//25 采集成功数量       long-unsigned，
	p8[25]=DataType_long_unsigned;
	p8[26]=0;
	p8[27]=0;
//28 已发送报文条数     long-unsigned，
	p8[28]=DataType_long_unsigned;
	p8[29]=0;
	p8[30]=0;
//31 已接收报文条数     long-unsigned
	p8[31]=DataType_long_unsigned;
	p8[32]=0;
	p8[33]=0;
//}
}

void RM_Monitor_Init_PORTn(u32 PORTn,u32 ID)//采集任务监控初始化(按端口号);入口:PORTn=端口号,ID=任务号
{
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl_1;
	UARTCtrl_TypeDef *UARTCtrl_2;
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	UARTCtrl_TypeDef *UARTCtrl_4;
#endif
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	UARTCtrl_1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	UARTCtrl_2=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	UARTCtrl_4=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
#endif
	if(UARTCtrl_1->TaskTime[ID].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行
	{
		return;
	}
	if(UARTCtrl_2->TaskTime[ID].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行
	{
		return;
	}
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(UARTCtrl_4->TaskTime[ID].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行
	{
		return;
	}
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
		if(UARTCtrl_4->TaskTime[ID].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行
		{
			return;
		}
	}
#endif
	if(PORTn==RS485_1PORT)
	{
		UARTCtrl_1->TaskTime[ID].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行
	}
	if(PORTn==RS485_2PORT)
	{
		UARTCtrl_2->TaskTime[ID].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行
	}
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(PORTn==RS485_4PORT)
	{
		UARTCtrl_4->TaskTime[ID].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行
	}
#endif
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
//0 采集任务监控单元∷=structure
	p8[0]=DataType_structure;
	p8[1]=8;
//{
//2 任务ID       unsigned
	p8[2]=DataType_unsigned;
	p8[3]=ID;
//4 任务执行状态  enum
//{
//	未执行（0）
//	执行中（1）
//	已执行（2）
//}，
	p8[4]=DataType_enum;
	p8[5]=0;
//6 任务执行开始时间   date_time_s，//用各端口时间值小的
	p8[6]=DataType_date_time_s;
	p8[7]=0xff;
	p8[8]=0xff;
	p8[9]=0xff;
	p8[10]=0xff;
	p8[11]=0xff;
	p8[12]=0xff;
	p8[13]=0xff;
//14 任务执行结束时间   date_time_s，//用各端口时间值大的
	p8[14]=DataType_date_time_s;
	p8[15]=0xff;
	p8[16]=0xff;
	p8[17]=0xff;
	p8[18]=0xff;
	p8[19]=0xff;
	p8[20]=0xff;
	p8[21]=0xff;
//22 采集总数量         long-unsigned，
	p8[22]=DataType_long_unsigned;
	p8[23]=0;
	p8[24]=0;
//25 采集成功数量       long-unsigned，
	p8[25]=DataType_long_unsigned;
	p8[26]=0;
	p8[27]=0;
//28 已发送报文条数     long-unsigned，
	p8[28]=DataType_long_unsigned;
	p8[29]=0;
	p8[30]=0;
//31 已接收报文条数     long-unsigned
	p8[31]=DataType_long_unsigned;
	p8[32]=0;
	p8[33]=0;
//}
}



void RM_Monitor(u32 ID,u32 NUM_Tx,u32 NUM_Rx)//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数,NUM_Rx=接收条数
{
	u32 i;
	u32 x;
	u32 y;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl_1;
	UARTCtrl_TypeDef *UARTCtrl_2;
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	UARTCtrl_TypeDef *UARTCtrl_4;
#endif
	u32 NUM_OK;
	u32 SourTaskID;
	u32 TaskID;
	u32 FileID;
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
	i=MRR(ADDR_6012_2_SDRAM+(ID*LENper_6012_2),1);
	if(i!=DataType_structure)
	{
		p8[0]=0;//删除时structure的数据单元清为0(NULL)
		return;
	}
	UARTCtrl_1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	if(UARTCtrl_1->Function==0)//口功能 0=终端维护口,!=0终端抄表口
	{
		UARTCtrl_1->TaskTime[ID].Run=2;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	}
	UARTCtrl_2=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
	if(UARTCtrl_2->Function==0)//口功能 0=终端维护口,!=0终端抄表口
	{
		UARTCtrl_2->TaskTime[ID].Run=2;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	}
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	UARTCtrl_4=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
#endif
//0 采集任务监控单元∷=structure
	p8[0]=DataType_structure;
	p8[1]=8;
//{
//2 任务ID       unsigned
	p8[2]=DataType_unsigned;
	p8[3]=ID;
//4 任务执行状态  enum
//{
//	未执行（0）
//	执行中（1）
//	已执行（2）
//}，
	p8[4]=DataType_enum;
	p8[5]=0;
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if((UARTCtrl_1->TaskTime[ID].Run==1)||(UARTCtrl_2->TaskTime[ID].Run==1)||(UARTCtrl_4->TaskTime[ID].Run==1))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	{
		p8[5]=1;
	}
	else
	{
		if((UARTCtrl_1->TaskTime[ID].Run==2)&&(UARTCtrl_2->TaskTime[ID].Run==2)&&(UARTCtrl_4->TaskTime[ID].Run==2))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
		{
			p8[5]=2;
		}
	}
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if((UARTCtrl_1->TaskTime[ID].Run==1)||(UARTCtrl_2->TaskTime[ID].Run==1))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	{
		p8[5]=1;
	}
	else
	{
		if((UARTCtrl_1->TaskTime[ID].Run==2)&&(UARTCtrl_2->TaskTime[ID].Run==2))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
		{
			p8[5]=2;
		}
	}
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
		if((UARTCtrl_1->TaskTime[ID].Run==1)||(UARTCtrl_2->TaskTime[ID].Run==1)||(UARTCtrl_4->TaskTime[ID].Run==1))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
		{
			p8[5]=1;
		}
		else
		{
			if((UARTCtrl_1->TaskTime[ID].Run==2)&&(UARTCtrl_2->TaskTime[ID].Run==2)&&(UARTCtrl_4->TaskTime[ID].Run==2))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
			{
				p8[5]=2;
			}
		}
	}
	else
	{
		if((UARTCtrl_1->TaskTime[ID].Run==1)||(UARTCtrl_2->TaskTime[ID].Run==1))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
		{
			p8[5]=1;
		}
		else
		{
			if((UARTCtrl_1->TaskTime[ID].Run==2)&&(UARTCtrl_2->TaskTime[ID].Run==2))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
			{
				p8[5]=2;
			}
		}
	}
#endif
//6 任务执行开始时间   date_time_s，//用各端口时间值小的
	p8[6]=DataType_date_time_s;
	p8[7]=0xff;
	p8[8]=0xff;
	p8[9]=0xff;
	p8[10]=0xff;
	p8[11]=0xff;
	p8[12]=0xff;
	p8[13]=0xff;
//	if(UARTCtrl_1->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//	{
		i=UARTCtrl_1->TaskTime[ID].StartTime[2];
		if((i!=0)&&(i!=0xff))
		{
			MR((u32)p8+7,(u32)&UARTCtrl_1->TaskTime[ID].StartTime,7);
		}
//	}
//	if(UARTCtrl_2->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//	{
		i=UARTCtrl_2->TaskTime[ID].StartTime[2];
		if((i!=0)&&(i!=0xff))
		{
			i=Compare_DL698DataString(p8+7,(u8 *)&UARTCtrl_2->TaskTime[ID].StartTime,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i==2)
			{//p1<p2
				MR((u32)p8+7,(u32)&UARTCtrl_2->TaskTime[ID].StartTime,7);
			}
		}
//	}
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//	if(UARTCtrl_4->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//	{
		i=UARTCtrl_4->TaskTime[ID].StartTime[2];
		if((i!=0)&&(i!=0xff))
		{
			i=Compare_DL698DataString(p8+7,(u8 *)&UARTCtrl_4->TaskTime[ID].StartTime,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i==2)
			{//p1<p2
				MR((u32)p8+7,(u32)&UARTCtrl_4->TaskTime[ID].StartTime,7);
			}
		}
//	}
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
//		if(UARTCtrl_4->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//		{
			i=UARTCtrl_4->TaskTime[ID].StartTime[2];
			if((i!=0)&&(i!=0xff))
			{
				i=Compare_DL698DataString(p8+7,(u8 *)&UARTCtrl_4->TaskTime[ID].StartTime,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				if(i==2)
				{//p1<p2
					MR((u32)p8+7,(u32)&UARTCtrl_4->TaskTime[ID].StartTime,7);
				}
			}
//		}
	}
#endif
//14 任务执行结束时间   date_time_s，//用各端口时间值大的
	p8[14]=DataType_date_time_s;
	p8[15]=0;
	p8[16]=0;
	p8[17]=0;
	p8[18]=0;
	p8[19]=0;
	p8[20]=0;
	p8[21]=0;
//	if(UARTCtrl_1->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//	{
		i=UARTCtrl_1->TaskTime[ID].EndTime[2];
		if((i!=0)&&(i!=0xff))
		{
			MR((u32)p8+15,(u32)&UARTCtrl_1->TaskTime[ID].EndTime,7);
		}
//	}
//	if(UARTCtrl_2->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//	{
		i=UARTCtrl_2->TaskTime[ID].EndTime[2];
		if((i!=0)&&(i!=0xff))
		{
			i=Compare_DL698DataString(p8+15,(u8 *)&UARTCtrl_2->TaskTime[ID].EndTime,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i==2)
			{//p1<p2
				MR((u32)p8+15,(u32)&UARTCtrl_2->TaskTime[ID].EndTime,7);
			}
		}
//	}
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//	if(UARTCtrl_4->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//	{
		i=UARTCtrl_4->TaskTime[ID].EndTime[2];
		if((i!=0)&&(i!=0xff))
		{
			i=Compare_DL698DataString(p8+15,(u8 *)&UARTCtrl_4->TaskTime[ID].EndTime,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i==2)
			{//p1<p2
				MR((u32)p8+15,(u32)&UARTCtrl_4->TaskTime[ID].EndTime,7);
			}
		}
//	}
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
//		if(UARTCtrl_4->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//		{
			i=UARTCtrl_4->TaskTime[ID].EndTime[2];
			if((i!=0)&&(i!=0xff))
			{
				i=Compare_DL698DataString(p8+15,(u8 *)&UARTCtrl_4->TaskTime[ID].EndTime,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				if(i==2)
				{//p1<p2
					MR((u32)p8+15,(u32)&UARTCtrl_4->TaskTime[ID].EndTime,7);
				}
			}
//		}
	}
#endif
//22 采集总数量         long-unsigned，
	p8[22]=DataType_long_unsigned;
//	p8[23]=0;
//	p8[24]=0;
	i=0;
//	if(UARTCtrl_1->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//	{
		i=UARTCtrl_1->NUMrmTOTAL[ID];
//	}
//	if(UARTCtrl_2->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
//	{
		i+=UARTCtrl_2->NUMrmTOTAL[ID];
//	}
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//	if(UARTCtrl_4->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行
//	{
		i+=UARTCtrl_4->NUMrmTOTAL[ID];
//	}
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
		if(UARTCtrl_4->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行
		{
			i+=UARTCtrl_4->NUMrmTOTAL[ID];
		}
	}
#endif
	p8[23]=i>>8;
	p8[24]=i;
//25 采集成功数量       long-unsigned，
	p8[25]=DataType_long_unsigned;
//	p8[26]=0;
//	p8[27]=0;
	i=0;
	if(UARTCtrl_1->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	{
		SourTaskID=UARTCtrl_1->TaskID;
		TaskID=IDtoTaskID(ID);//入口:任务号id;出口：TaskID
		UARTCtrl_1->TaskID=TaskID;
		FileID=Get_FileID(RS485_1PORT);//UART各任务的FileID
		UARTCtrl_1->TaskID=SourTaskID;
		if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
		{//原已采集完成数=0;抄曲线和补抄(日冻结和曲线)需抄多个点,在发送管理时识别各点是否已抄
			//UARTCtrl->NUMrmOK[id]=0;//原已采集完成数
			x=UARTCtrl_1->NUMrmOK[ID];
		}
		else
		{
			x=UARTCtrl_1->NUMrmOK[ID];
			y=UARTCtrl_1->NUMrmBAD[ID];
			if(x>=y)
			{
				x-=y;
			}
			else
			{
				x=0;
			}
		}
		i=x;
	}
	if(UARTCtrl_2->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	{
		SourTaskID=UARTCtrl_2->TaskID;
		TaskID=IDtoTaskID(ID);//入口:任务号id;出口：TaskID
		UARTCtrl_2->TaskID=TaskID;
		FileID=Get_FileID(RS485_2PORT);//UART各任务的FileID
		UARTCtrl_2->TaskID=SourTaskID;
		if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
		{//原已采集完成数=0;抄曲线和补抄(日冻结和曲线)需抄多个点,在发送管理时识别各点是否已抄
			//UARTCtrl->NUMrmOK[id]=0;//原已采集完成数
			x=UARTCtrl_2->NUMrmOK[ID];
		}
		else
		{
			x=UARTCtrl_2->NUMrmOK[ID];
			y=UARTCtrl_2->NUMrmBAD[ID];
			if(x>=y)
			{
				x-=y;
			}
			else
			{
				x=0;
			}
		}
		i+=x;
	}
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(UARTCtrl_4->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	{
		SourTaskID=UARTCtrl_4->TaskID;
		TaskID=IDtoTaskID(ID);//入口:任务号id;出口：TaskID
		UARTCtrl_4->TaskID=TaskID;
		FileID=Get_FileID(RS485_4PORT);//UART各任务的FileID
		UARTCtrl_4->TaskID=SourTaskID;
		if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
		{//原已采集完成数=0;抄曲线和补抄(日冻结和曲线)需抄多个点,在发送管理时识别各点是否已抄
			//UARTCtrl->NUMrmOK[id]=0;//原已采集完成数
			x=UARTCtrl_4->NUMrmOK[ID];
		}
		else
		{
			x=UARTCtrl_4->NUMrmOK[ID];
			y=UARTCtrl_4->NUMrmBAD[ID];
			if(x>=y)
			{
				x-=y;
			}
			else
			{
				x=0;
			}
		}
		i+=x;
	}
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
		if(UARTCtrl_4->TaskTime[ID].Run)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
		{
			SourTaskID=UARTCtrl_4->TaskID;
			TaskID=IDtoTaskID(ID);//入口:任务号id;出口：TaskID
			UARTCtrl_4->TaskID=TaskID;
			FileID=Get_FileID(RS485_4PORT);//UART各任务的FileID
			UARTCtrl_4->TaskID=SourTaskID;
			if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
			{//原已采集完成数=0;抄曲线和补抄(日冻结和曲线)需抄多个点,在发送管理时识别各点是否已抄
				//UARTCtrl->NUMrmOK[id]=0;//原已采集完成数
				x=UARTCtrl_4->NUMrmOK[ID];
			}
			else
			{
				x=UARTCtrl_4->NUMrmOK[ID];
				y=UARTCtrl_4->NUMrmBAD[ID];
				if(x>=y)
				{
					x-=y;
				}
				else
				{
					x=0;
				}
			}
			i+=x;
		}
	}
#endif
	p8[26]=i>>8;
	p8[27]=i;
	NUM_OK=i;
//28 已发送报文条数     long-unsigned，
	p8[28]=DataType_long_unsigned;
	i=p8[29]<<8;
	i|=p8[30];
	i+=NUM_Tx;
	if(i<NUM_OK)
	{//采集成功数不为0，而发送和接收帧数=0,在抄表成功后有代理等其他任务插入后TaskID=0，认为重新执行调用了采集任务监控初始化
		//准确条数=成功数*抄表命令数
		i=NUM_OK;//至少为成功数
	}
	p8[29]=i>>8;
	p8[30]=i;
//31 已接收报文条数     long-unsigned
	p8[31]=DataType_long_unsigned;
	i=p8[32]<<8;
	i|=p8[33];
	i+=NUM_Rx;
	if(i<NUM_OK)
	{//采集成功数不为0，而发送和接收帧数=0,在抄表成功后有代理等其他任务插入后TaskID=0，认为重新执行调用了采集任务监控初始化
		//准确条数=成功数*抄表命令数
		i=NUM_OK;//至少为成功数
	}
	p8[32]=i>>8;
	p8[33]=i;
}

void RM_Monitor_TxNUM(u32 ID,u32 NUM_Tx)//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数
{
	u32 i;
	u8* p8;
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
	i=p8[29]<<8;
	i|=p8[30];
	i+=NUM_Tx;
	p8[29]=i>>8;
	p8[30]=i;
}

void RM_Monitor_RxNUM(u32 ID,u32 NUM_Rx)//采集任务监控;入口:ID=任务号,NUM_Rx=接收条数
{
	u32 i;
	u8* p8;
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
	i=p8[32]<<8;
	i|=p8[33];
	i+=NUM_Rx;
	p8[32]=i>>8;
	p8[33]=i;
}

void RM_State_SetUpDate(void)//采集状态,档案变更时更新(对应配置序号)
{
	u32 i;
	u32 x;
	u8* p8s;
	u8* p8d;

	p8s=(u8*)(ADDR_6000_SDRAM);
	p8d=(u8*)(ADDR_6032);
	for(i=0;i<NUM_RMmax;i++)
	{
		if(p8s[0])
		{
			if(p8d[0])
			{
				for(x=0;x<6;x++)
				{
					if(p8s[10+x]!=p8d[5+x])
					{
						break;
					}
				}
				if(x<6)
				{
					p8d[0]=0;//无效
				}
			}
		}
		else
		{
			p8d[0]=0;
		}
		p8s+=LENper_6000;
		p8d+=LENper_6032;
	}
}

void RM_State_RMUpDate(u32 PORTn,u32 OK,u32 SetNo,u32 Class,u32 Phase)//采集状态,每次抄表后更新:入口:OK=0失败=1成功,OK=1时Class=中继级别,Phase=相位
{
	u32 i;
	u32 offset;
#if ((USER/100)==9)//河南		
	u32 j;
	u8* p8s1;
#endif
	u8* p8d;

	p8d=(u8*)(ADDR_6032);
	p8d+=SetNo*LENper_6032;
	if(p8d[0]==0)
	{
		if(OK==0)
		{
			return;
		}
//一个采集状态∷=structure 
		p8d[0]=DataType_structure;
		p8d[1]=8;
//{ 
//1.通信地址              TSA， 
		p8d[2]=DataType_TSA;
		p8d[3]=7;
		p8d[4]=5;
		MR((u32)p8d+5,ADDR_6000_SDRAM+(SetNo*LENper_6000)+10,6);
		offset=11;
//2.中继级别              unsigned， 
		p8d[offset]=DataType_unsigned;
		p8d[offset+1]=Class;
		offset+=2;
//3.中继地址              TSA， 
		p8d[offset]=0;
		offset+=1;
//4.端口                  OAD， 
		p8d[offset]=DataType_OAD;
		i=PORTntoOAD(PORTn);//通信口号转为OAD值
		p8d[offset+1]=i>>24;
		p8d[offset+2]=(i>>16)&0xff;
		p8d[offset+3]=(i>>8)&0xff;
		p8d[offset+4]=i;
		offset+=5;
//5.最后一次采集成功时间  date_time_s， 
		p8d[offset]=DataType_date_time_s;
		Get_date_time_s(p8d+offset+1);//得到date_time_s共7字节
		offset+=8;
//6.采集失败次数          unsigned， 
		p8d[offset]=DataType_unsigned;
		p8d[offset+1]=0;
		offset+=2;
//7.相位 enum{未知（0），A 相（1），B 相（2），C 相（3）}，
		p8d[offset]=DataType_enum;
#if ((USER/100)==9)//河南	
		Data_Inverse((u32)p8d+5,6); // 转成比较参数
		p8s1=(u8*)ADDR_AFN10F31;
		for(j=0;j<NUM_RMmax;j++)
		{
			if(Compare_DL698DataString(p8s1 + 2 + 8 * j ,p8d+5, 6) == 0)
			{
				break;
			}
		}
		Data_Inverse((u32)p8d+5,6);	// 再次换转
		Phase = 0;
		if(j < NUM_RMmax)
		{
			if(p8s1[ 2+ 8*j + 6]&0x01)
			{
				Phase = 1;
			}
			
			if(p8s1[ 2+ 8*j + 6]&0x02)
			{
				Phase = 2;
			}
	
			if(p8s1[ 2+ 8*j + 6]&0x04)
			{
				Phase = 3;
			}
		}			
#endif
		p8d[offset+1]=Phase;
		offset+=2;
//8.相序异常 enum{正常（0），LN 互易（1），逆相序（2）} 
		p8d[offset]=DataType_enum;
		p8d[offset+1]=0;
//}
		return;
	}
	if(OK==0)
	{//失败
		//6.采集失败次数          unsigned， 
		p8d=Get_Element(p8d,6,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		if(p8d)
		{
			if(p8d[1]!=0xff)
			{
				p8d[1]++;
			}
		}
	}
	else
	{//成功
		//2.中继级别              unsigned， 
		p8d[12]=Class;
		//5.最后一次采集成功时间  date_time_s， 
		p8d=Get_Element(p8d,5,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		if(p8d)
		{
			Get_date_time_s(p8d+1);//得到date_time_s共7字节
			//7.相位 enum{未知（0），A 相（1），B 相（2），C 相（3）}，
			p8d+=10;
			p8d[1]=Phase;
		}
	}
}
/*
void RM_State_UpDate(u32 PORTn,u32 SetNo,u32 Phase,u32 Err)//采集状态更新:入口:SetNo=配置号,Phase=相位,Err=线路异常
{
	u32 i;
	u32 offset;
	u8* p8d;
	
	p8d=(u8*)(ADDR_6032);
	p8d+=SetNo*LENper_6032;
	if(p8d[0]==0)
	{
//一个采集状态∷=structure 
		p8d[0]=DataType_structure;
		p8d[1]=8;
//{ 
//1.通信地址              TSA， 
		p8d[2]=DataType_TSA;
		p8d[3]=7;
		p8d[4]=5;
		MR((u32)p8d+5,ADDR_6000_SDRAM+(SetNo*LENper_6000)+10,6);
		offset=11;
//2.中继级别              unsigned， 
		p8d[offset]=DataType_unsigned;
		p8d[offset+1]=0;
		offset+=2;
//3.中继地址              TSA， 
		p8d[offset]=0;
		offset+=1;
//4.端口                  OAD， 
		p8d[offset]=DataType_OAD;
		i=PORTntoOAD(PORTn);//通信口号转为OAD值
		p8d[offset+1]=i>>24;
		p8d[offset+2]=(i>>16)&0xff;
		p8d[offset+3]=(i>>8)&0xff;
		p8d[offset+4]=i;
		offset+=5;
//5.最后一次采集成功时间  date_time_s， 
		p8d[offset]=DataType_date_time_s;
		MC(0xff,(u32)p8d+offset+1,7);
		offset+=8;
//6.采集失败次数          unsigned， 
		p8d[offset]=DataType_unsigned;
		p8d[offset+1]=0;
		offset+=2;
//7.相位 enum{未知（0），A 相（1），B 相（2），C 相（3）}，
		p8d[offset]=DataType_enum;
		p8d[offset+1]=Phase;
		offset+=2;
//8.相序异常 enum{正常（0），LN 互易（1），逆相序（2）} 
		p8d[offset]=DataType_enum;
		p8d[offset+1]=Err;
//}
		return;
	}
//7.相位 enum{未知（0），A 相（1），B 相（2），C 相（3）}，
	p8d=Get_Element(p8d,7,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
	if(p8d)
	{
		p8d[1]=Phase;
//8.相序异常 enum{正常（0），LN 互易（1），逆相序（2）} 
		p8d[3]=Err;
	}
}

void RM_State_ReadRouterAllUpDate(u32 PORTn)//采集状态,读全部路由信息更新
{
	u32 i;
	u32 n;
	u32 StartNo;
	u32 SetNo;
	u32 Phase;
	u32 Err;
	u32 NUMmax;
	u8* p8;
	
	StartNo=0;//开始序号
	while(1)
	{
		UART_3762_AFN03F101(PORTn,StartNo,15,(u8*)ADDR_128KDATABUFF);//(河南)查询节点台区信息(只暂停路由,没恢复);入口:StartNo=节点起始序号首帧从0开始,Num=节点数量(最大支持15个),p8Buff=数据放置缓冲
//台区节点总数量	BIN	2
//本次上报节点数量n(n<=15)	BIN	1
//节点1地址	BCD	6
//b7	b6	b5-3	b2-1	b0	BS	1
//保留	线路异常	相序	台区状态	表档案		
//节点1台区地址	BCD	6
//…
		p8=(u8*)ADDR_128KDATABUFF;
		NUMmax=p8[0]|(p8[1]<<8);//台区节点总数量
		n=p8[2];//本次上报节点数量
		p8+=3;
		for(i=0;i<n;i++)
		{
			MR(ADDR_DATABUFF,(u32)p8,6);
			Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
			StartNo=AddrToMeterSetNo((u16*)ADDR_DATABUFF);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
			if(StartNo<NUM_RMmax)
			{//找到序号
				Phase=p8[6];
				Err=Phase;
				Phase>>=3;
				Phase&=0x03;
				switch(Phase)
				{
					case 1:
						Phase=1;//相位 enum{未知（0），A 相（1），B 相（2），C 相（3）}
						break;
					case 2:
						Phase=2;
						break;
					case 4:
						Phase=3;
						break;
					default:
						Phase=0;
						break;
				}
				Err>>=6;//相序异常 enum{正常（0），LN 互易（1），逆相序（2）} 
				Err&=1;
				if(Err)
				{
					Err=2;//相序异常 enum{正常（0），LN 互易（1），逆相序（2）} 
				}
				RM_State_UpDate(RS485_4PORT,SetNo,Phase,Err);//采集状态更新:入口:SetNo=配置号,Phase=相位,Err=线路异常
			}
			p8+=13;
		}
		StartNo+=n;
		if(StartNo>=NUMmax)
		{
			break;
		}
	}
}
*/


u32 DateSaveTI(u32 PORTn,u8* pOUT)//普通采集方案的采集存储时标;返回:时标类型1=任务开始时间,2=相对当日0点0分,3=...,6=;pOUT=date_time_s
{
	u32 i;
	u32 x;
	u32 id;
	u32 YYMD;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//b31-b28优先级:1=首要,2=必要,3=需要,4=可能
//b27-b24方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//b23-b20普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集
//b19-b16普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标
	id=UARTCtrl->TaskID;//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	switch((id>>16)&0xf)//普通采集方案的存储时标
	{
		default://(使用任务开始时间,抄表失败地址存储时需要存储时标)
		case 0://未定义(使用任务开始时间)
		case 1://任务开始时间
			if(pOUT)
			{
				i=id;
				i>>=20;
				i&=0xf;
				switch(i)//普通采集方案的采集类型
				{
					default:
					//case 0://采集当前数据,
					//case 1://采集上第N次,
					//case 2://按冻结时标采集,
					//case 3://按时标间隔采集
						id>>=8;
						id&=0xff;//任务号
						if(id<(RMTASKmax+ExtRMTASKmax))
						{
							for(i=0;i<7;i++)
							{
								pOUT[i]=UARTCtrl->TaskTime[id].StdTime[i];
							}
						}
						break;
					case 4://RetryMetering 补抄
						id>>=8;
						id&=0xff;//任务号
						YYMD=(UARTCtrl->TaskTime[id].StdTime[0]<<24)+(UARTCtrl->TaskTime[id].StdTime[1]<<16)+(UARTCtrl->TaskTime[id].StdTime[2]<<8)+UARTCtrl->TaskTime[id].StdTime[3];
						i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
						while(i--)
						{
							YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
						}
						pOUT[0]=YYMD>>24;
						pOUT[1]=YYMD>>16;
						pOUT[2]=YYMD>>8;
						pOUT[3]=YYMD;
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						break;
				}
			}
			return 1;
		case 2://相对当日0点0分
			i=(id>>8)&0xff;//任务号
			i=MRR(ADDR_6012_2_SDRAM+(i*LENper_6012_2)+5,3);
			YYMD=(i&0xff00)+(i>>16);
			i&=0xff;
			switch(i)//采集频率
			{
				case 0://秒      （0），
					break;
				case 1://分      （1），
					YYMD*=60;
					break;
				case 2://时      （2），
					YYMD*=60*60;
					break;
				case 3://日      （3），
					YYMD*=60*60*24;
				case 4://月      （4），
				case 5://年      （5）
				default:
					YYMD*=60*60*24;
					break;
			}
#if((USER/100)==0||(USER/100)==15)//电科院测试
			if(YYMD<(60*60*24))
			{
				//采集间隔小于1日,存储时标用任务开始时间(注:电科院测试软件采集事件等监控数据用相对当日0点0分）
				if(pOUT)
				{
					id=(UARTCtrl->TaskID>>8)&0xff;
					if(id<(RMTASKmax+ExtRMTASKmax))
					{
						for(i=0;i<7;i++)
						{
							pOUT[i]=UARTCtrl->TaskTime[id].StartTime[i];
						}
					}
				}
				return 1;
			}
#endif
			if(pOUT)
			{
				i=UARTCtrl->TaskID;
				i>>=20;
				i&=0xf;
				switch(i)//普通采集方案的采集类型
				{
					default:
					//case 0://采集当前数据,
					//case 1://采集上第N次,
					//case 2://按冻结时标采集,
					//case 3://按时标间隔采集
						for(i=0;i<4;i++)
						{
							pOUT[i]=Comm_Ram->DL698YMDHMS[i];
						}
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						break;
					case 4://RetryMetering 补抄
						YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
						i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
						while(i--)
						{
							YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
						}
						pOUT[0]=YYMD>>24;
						pOUT[1]=YYMD>>16;
						pOUT[2]=YYMD>>8;
						pOUT[3]=YYMD;
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						break;
				}
			}
			return 2;
		case 3://相对上日23点59分
			if(pOUT)
			{
				YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
				i=UARTCtrl->TaskID;
				i>>=20;
				i&=0xf;
				switch(i)//普通采集方案的采集类型
				{
					default:
					//case 0://采集当前数据,
					//case 1://采集上第N次,
					//case 2://按冻结时标采集,
					//case 3://按时标间隔采集
						YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
						break;
					case 4://RetryMetering 补抄
						YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
						i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
						while(i--)
						{
							YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
						}
						break;
				}
				pOUT[0]=YYMD>>24;
				pOUT[1]=YYMD>>16;
				pOUT[2]=YYMD>>8;
				pOUT[3]=YYMD>>0;
				pOUT[4]=23;
				pOUT[5]=59;
				pOUT[6]=0;
			}
			return 3;
		case 4://相对上日0点0分
			if(pOUT)
			{
				YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
				i=UARTCtrl->TaskID;
				i>>=20;
				i&=0xf;
				switch(i)//普通采集方案的采集类型
				{
					default:
					//case 0://采集当前数据,
					//case 1://采集上第N次,
					//case 2://按冻结时标采集,
					//case 3://按时标间隔采集
						YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
						break;
					case 4://RetryMetering 补抄
						YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
						i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
						while(i--)
						{
							YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
						}
						break;
				}
				pOUT[0]=YYMD>>24;
				pOUT[1]=YYMD>>16;
				pOUT[2]=YYMD>>8;
				pOUT[3]=YYMD>>0;
				pOUT[4]=0;
				pOUT[5]=0;
				pOUT[6]=0;
			}
			return 4;
		case 5://相对当月1日0点0分
			if(pOUT)
			{
				for(i=0;i<3;i++)
				{
					pOUT[i]=Comm_Ram->DL698YMDHMS[i];
				}
				pOUT[3]=1;
				pOUT[4]=0;
				pOUT[5]=0;
				pOUT[6]=0;
			}
			return 5;
		case 6://数据冻结时标
//b31-b28优先级:1=首要,2=必要,3=需要,4=可能
//b27-b24方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//b23-b20普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集
//b19-b16普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标
//b15-b8任务号
//b7-b0方案编号
			if(pOUT)
			{
				i=UARTCtrl->TaskID&0xff;//方案号
				p8=(u8*)(u8*)ADDR_6014_SDRAM+(i*LENper_6014);
				p8=Get_Element(p8,4,1,0);//计算元素地址,使用ADDR_128KDATABUFF
				i=0;
				if(p8)
				{
					i=(p8[0]<<24)|(p8[1]<<16)|(p8[2]<<8)|p8[3];
				}
				//i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//采集数据的OI
				switch(i)
				{
					case 0x5B015002://分钟冻结(采曲线数据)
						i=UARTCtrl->TaskID;
						i>>=20;
						i&=0xf;
						switch(i)//普通采集方案的采集类型
						{
							default:
							case 0://采集当前数据,
								id>>=8;
								id&=0xff;//任务号
								if(id<(RMTASKmax+ExtRMTASKmax))
								{
									for(i=0;i<7;i++)
									{
										pOUT[i]=UARTCtrl->TaskTime[id].StdTime[i];//用任务开始标准时间
									}
								}
								break;
							case 1://采集上第N次,
								//上1次到上10次
								id>>=8;
								id&=0xff;//任务号
								if(id>=(RMTASKmax+ExtRMTASKmax))
								{
									break;
								}
								for(i=0;i<7;i++)
								{
									pOUT[i]=UARTCtrl->TaskTime[id].StdTime[i];//任务开始标准时间
								}
								p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
								x=(p8[6]<<8)+p8[7];//取频率值
								if(x==0)
								{
									x=1;//最小值=1
								}
								switch(p8[5])//执行频率   TI
								{
									case 0://秒      （0），
										break;
									case 1://分      （1），
										x*=60;
										break;
									case 2://时      （2），
										x*=60*60;
										break;
									case 3://日      （3），
										x*=60*60*24;
										break;
									case 4://月      （4），
									case 5://年      （5）
										x*=60*60*24*31;
										break;
								}
								i=UARTCtrl->RM_DI_N;//采集上第N次
								if(i)
								{
									i--;
								}
								x*=i;
								pYYMDHMS_SubS_hex(pOUT,x);//年年月日时分秒减S秒,返回减S秒后的年年月日时分秒
								break;
							case 2://按冻结时标采集,
								id>>=8;
								id&=0xff;//任务号
								if(id<(RMTASKmax+ExtRMTASKmax))
								{
									for(i=0;i<7;i++)
									{
										pOUT[i]=UARTCtrl->TaskTime[id].StdTime[i];//用任务开始标准时间
									}
								}
								break;
							case 3://按时标间隔采集
								id>>=8;
								id&=0xff;//任务号
								if(id>=(RMTASKmax+ExtRMTASKmax))
								{
									break;
								}
							#if (USER/100)==9//河南,5分钟电压曲线
								if(PORTn==ACSAMPLEPORT)
								{//交采曲线
									MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
									return 6;
								}
							#endif
								switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
								{
									case 1://DL/T645-1997（1）
										//总是用实时数据代，用标准任务开始时间
										MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
										return 6;
									case 2://DL/T645-2007（2）
										switch(UARTCtrl->RM_DI)
										{
											case 0x06000001://全部6类负荷曲线
											case 0x06010001://1类负荷曲线(电压、电流、频率)
											case 0x06020001://2类负荷曲线(有无功功率)
											case 0x06030001://3类负荷曲线(功率因数)
											case 0x06040001://4类负荷曲线(正反有功,組合无功1和2总电能)
											case 0x06050001://5类负荷曲线(1-4象限无功总电能)
											case 0x06060001://6类负荷曲线(当前有功需量,当前无功需量)
												break;
											default:
												//是用实时数据代，用标准任务开始时间
												MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
												return 6;
										}
										break;
									case 3://DL/T698.45（3）
										break;
									case ProtocolNO_ABB://ABB表通信协议编号
									case ProtocolNO_Landis://Landis表通信协议编号(IEC1107 模式C)
										//总是用实时数据代，用标准任务开始时间
										MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
										return 6;
									default:
										break;
								}
								//按任务执行间隔时间和采集方案采集间隔时间计算
								//任务执行间隔时间
								p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
								i=pTItoSECOND(p8+5);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
								if(PORTn==RS485_4PORT)
								{//是载波口
								#if (CurveTaskTImax)
									if(i<CurveTaskTImax)//(6*60*60)//曲线抄表任务执行频率最大值(秒),此值决定按采集方案采集间隔时间可向前补抄的曲线点数
									{
										i=CurveTaskTImax;
									}
								#endif
								}
								//采集方案采集间隔时间
								x=pTItoSECOND(UARTCtrl->RM_DI_TI);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
								
								//#if (USER/100)==17
								//if(UARTCtrl->RM_DI_TI_Count==0)
								//{
									//UARTCtrl->RM_DI_TI_Count=1;
								//}
								//x*=(UARTCtrl->RM_DI_TI_Count-1);
								//#else
								//x*=UARTCtrl->RM_DI_TI_Count;
								//#endif
								x*=UARTCtrl->RM_DI_TI_Count;
								if(i>=x)
								{
									i-=x;
								}
								else
								{
									i=0;
								}
								MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
								pYYMDHMS_SubS_hex(pOUT,i);//年年月日时分秒减S秒,返回减S秒后的年年月日时分秒
								break;
							case 4://RetryMetering 补抄
								id>>=8;
								id&=0xff;//任务号
								if(id>=(RMTASKmax+ExtRMTASKmax))
								{
									break;
								}
								MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
								i=pTItoSECOND(UARTCtrl->RM_DI_TI);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
								//#if (USER/100)==17
								//if(UARTCtrl->RM_DI_TI_Count==0)
								//{
									//UARTCtrl->RM_DI_TI_Count=1;
								//}
								//i*=(UARTCtrl->RM_DI_TI_Count-1);
								//#else
								//i*=UARTCtrl->RM_DI_TI_Count;
								//#endif
								i*=UARTCtrl->RM_DI_TI_Count;
								pYYMDHMS_SubS_hex(pOUT,i);//年年月日时分秒减S秒,返回减S秒后的年年月日时分秒
								return 6;
						}
						break;
					case  0x5B015004://日冻结
						i=id;
						i>>=20;
						i&=0xf;
						switch(i)//普通采集方案的采集类型
						{
							default:
							case 0://采集当前数据,
							case 1://采集上第N次,
							case 2://按冻结时标采集,
							case 3://按时标间隔采集
								for(i=0;i<4;i++)
								{
									pOUT[i]=Comm_Ram->DL698YMDHMS[i];
								}
								pOUT[4]=0;
								pOUT[5]=0;
								pOUT[6]=0;
								break;
							case 4://RetryMetering 补抄
								YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
								i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
								while(i--)
								{
									YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
								}
								pOUT[0]=YYMD>>24;
								pOUT[1]=YYMD>>16;
								pOUT[2]=YYMD>>8;
								pOUT[3]=YYMD;
								pOUT[4]=0;
								pOUT[5]=0;
								pOUT[6]=0;
								break;
						}
						break;
					case 0x5B015006://月冻结
						for(i=0;i<4;i++)
						{
							pOUT[i]=Comm_Ram->DL698YMDHMS[i];
						}
						pOUT[3]=0x01;//固定为1日
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						break;
					default:
						for(i=0;i<4;i++)
						{
							pOUT[i]=Comm_Ram->DL698YMDHMS[i];
						}
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						id=UARTCtrl->TaskID;
						id>>=8;
						id&=0xff;//任务号
						p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
						switch(p8[5])//执行频率   TI
						{
							case 0://秒      （0），
								break;
							case 1://分      （1），
								break;
							case 2://时      （2），
								pOUT[4]=Comm_Ram->DL698YMDHMS[4];//时
								break;
							case 3://日      （3），
								break;
							case 4://月      （4），
								pOUT[3]=0x01;//固定为1日
								break;
							case 5://年      （5）
								break;
						}
						break;
				}
			}
			return 6;
		case 7://相对上月月末23点59分
			if(pOUT)
			{
				YYMD=(Comm_Ram->DL698YMDHMS[0]<<16)+(Comm_Ram->DL698YMDHMS[1]<<8)+(Comm_Ram->DL698YMDHMS[2]<<0);
				YYMD=YYM_SubM_hex(YYMD,1);//年年月减m月,返回减m月后的年年月
				pOUT[0]=YYMD>>16;
				pOUT[1]=YYMD>>8;
				pOUT[2]=YYMD>>0;
				YYMD=YYM_D_hex(YYMD);//取指定年月的天数
				pOUT[3]=YYMD;
				pOUT[4]=23;
				pOUT[5]=59;
				pOUT[6]=0;
			}
			return 7;
	}
}

void RM_UpDate(void)//抄表参数更新;出口:判断,读采集档案配置表从flash到sdram,置位各PORTn各任务需重计算总需抄表数成功数等
{
	u32 i;
	u32 x;
	u32 n;
	u32 m;
	u32 PORTs;
	u32 PORTd;
	u32 UpDate;
	u8* p8s;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	if(Terminal_Ram->SET6000==0x55)//采集档案配置表有设置标志:0=没设置,0x55=设置
	{
		Terminal_Ram->SET6000=0;//采集档案配置表有设置标志:0=没设置,0x55=设置
		UpDate=0;//变化标志
		n=0;
		while(n<NUMmax_6000)
		{
			m=LEN_128KDATABUFF/LENper_6000;
			if(m>(NUMmax_6000-n))
			{
				m=NUMmax_6000-n;
			}
			MR(ADDR_128KDATABUFF,ADDR_6000,m*LENper_6000);
			for(i=0;i<m;i++)
			{
				//删除
				p8s=(u8*)ADDR_128KDATABUFF+(i*LENper_6000);
				p8d=(u8*)ADDR_6000_SDRAM+((n+i)*LENper_6000);
				if(p8s[0]!=p8d[0])
				{
					break;
				}
				//地址
				p8s+=8;
				p8d+=8;
				//x=Compare_octet_string(p8s,p8d);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				Compare_TSA_string(p8s,p8d);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				if(x)
				{
					break;
				}
				//端口
				x=p8s[0];
				x++;
				x+=5;
				p8s+=x;
				p8d+=x;
				PORTs=(p8s[0]<<24)|(p8s[1]<<16)|(p8s[2]<<8)|p8s[3];
				PORTd=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
				if(PORTs!=PORTd)
				{
					//0xF2090201://RS485_4通信口号(集中器载波,专变控制模块)
					break;
				}
			}
			if(i<m)
			{
				UpDate=1;//变化标志
				break;
			}
			n+=m;
		}
		if(UpDate)//变化标志
		{
			MR(ADDR_6000_SDRAM,ADDR_6000,LENmax_6000);
			Terminal_Ram->SETTask=0xff;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->SetChange6000=0x55;//采集档案配置表有变化标志:0=没变化,0x55=变化
			Terminal_Ram->RouterSynDelay_S_Timer=10;//路由档案同步延时执行秒定时器
			RM_State_SetUpDate();//采集状态,档案变更时更新(对应配置序号)
			
		}
		return;
	}
	if(Terminal_Ram->FLASHtoSDRAM!=Comm_Ram->DL698YMDHMS[4])//参数FLASHtoSDRAM上次进行的实时钟时(或其他)
	{
		Terminal_Ram->FLASHtoSDRAM=Comm_Ram->DL698YMDHMS[4];//参数FLASHtoSDRAM上次进行的实时钟时(或其他)
		FLASHtoSDRAM();//参数FLASHtoSDRAM
	}
}








