

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../DL698/DL698_Uart.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"


#include "../DL698/DL698_JSON.h"



u32 RM_TxDATA(u32 PORTn,u8 *p8CSD,u8 *p8tx)//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
{
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	{
		case 1://DL/T645-1997
			return RM_TxDATA_DL1997(PORTn,p8CSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
		case 2://DL/T645-2007
			return RM_TxDATA_DL2007(PORTn,p8CSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
		case 3://DL/T698.45
			return RM_TxDATA_DL698(PORTn,p8CSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
		case 4://CJ/T 188-2004
			return 0;//RM_TxDATA_CJ188(PORTn,p8CSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
		case ProtocolNO_ABB://ABB
			return RM_TxDATA_ABB(PORTn,p8CSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
		case ProtocolNO_Landis://Landis(IEC1107 模式C)
			return RM_TxDATA_Landis(PORTn,p8CSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
		default://未知 (0)
			return 0;//返回:组帧后总长度字节数

	}
}

u32 RM_TxDATA_Manage(u32 PORTn)//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
{
	u32 i;
	u32 x;
	u32 y;
	u8* p8tx;
	u8* p8rx;
	u16* p16;
	u8* pCSD;
	UARTCtrl_TypeDef * UARTCtrl;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	u16* p16timer;
#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
	u32 n;
	u32 NUM_SomeTaskID;
#endif
	u32 SetNo;
	
	SetNo=SetNo;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
//	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	UARTCtrl->Task=0;//空闲1次
	UARTCtrl->TxByte=0;
	
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;//任务号
	SetNo=UARTCtrl->MeterCount[i];

	switch(UARTCtrl->CONNECT)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
	{
		default://698规约连接和关闭过程
			UARTCtrl->CONNECT=0;
			return 4;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
		case 1://连接过程1
		case 2://连接过程2
			switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
			{
				default:
				case 1://DL/T645-1997
				case 2://DL/T645-2007
					UARTCtrl->CONNECT=100;
					return 4;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case 3://DL/T698.45
					UARTCtrl->CONNECT=100;
					return 4;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case 4://CJ/T 188-2004
					UARTCtrl->CONNECT=100;
					return 4;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case ProtocolNO_ABB://ABB
					x=RM_TxDATA(PORTn,pCSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
					if(x)
					{
						UARTCtrl->TxByte=x;//发送字节数
						return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
					}
					return 0;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case ProtocolNO_Landis://Landis(IEC1107 模式C)
					x=RM_TxDATA(PORTn,pCSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
					if(x)
					{
						UARTCtrl->TxByte=x;//发送字节数
						return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
					}
					return 0;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
			}
		case 0://0=空
			i=UARTCtrl->TaskID;
			if(i==0)
			{
				if(PORTn!=ACSAMPLEPORT)
				{//不是交采
					RM_NUM_Display(PORTn);//抄表数量显示
					if(PORTn==RS485_4PORT)
					{
						WarningOccur(PORTn,(u8 *)"载波 当前任务完成");//告警发生,入口告警代码和告警字符串
					}
				}
		#if DEBUG==1//0=发行,1=调试
				else
				{//是交采
					RM_NUM_Display(PORTn);//抄表数量显示
				}
		#endif
				return 4;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
			}
			i>>=8;
			i&=0xff;
			if(i>=(RMTASKmax+ExtRMTASKmax))
			{
				return 4;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
			}
			if(PORTn==RS485_4PORT)
			{//载波
				RM_NUM_Display(PORTn);//抄表数量显示
			}
			for(x=0;x<100;x++)
			{
			#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
				if(UARTCtrl->RMMeterCountAuto==1)//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
				{
					if(SetNo>=NUM_RMmax)
					{//错
						return 3;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
					}
					NUM_SomeTaskID=UARTCtrl->NUM_SomeTaskID;//当前同类采集任务的id号的列表个数
					n=0;
					y=0;
					for(n=0;n<NUM_SomeTaskID;n++)
					{
						y=UARTCtrl->TaskID;
						y&=0xffff0000;
						y|=UARTCtrl->SomeTaskid[n];//RMTASKmax*2 byte 当前同类采集任务的b15-b8任务号,b7-b0方案编号
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
				}
				else
				{
					y=RM_Meter(PORTn);//当前电能表计数是否要抄;入口:UARTCtrl->MeterCount[],UARTCtrl->TaskID;返回:0=不抄,1=要抄,2=完成,同时填写通信地址,波特率,抄表规约类型,
				}
			#else//#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
				y=RM_Meter(PORTn);//当前电能表计数是否要抄;入口:UARTCtrl->MeterCount[],UARTCtrl->TaskID;返回:0=不抄,1=要抄,2=完成,同时填写通信地址,波特率,抄表规约类型,
			#endif
				if(y==1)
				{//要抄
					i=UARTCtrl->TaskID;//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
					if(((i>>20)&0xf)==4)
					{//4=RetryMetering 补抄
						if(((i>>24)&0xf)==1)
						{//普通采集方案
							i=UARTCtrl->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
							if(((i!=2)&&(i!=3))||(PORTn==ACSAMPLEPORT))//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
							{//不补抄除645-2007和DL698.45外的表;也不补抄交采
								RM_OK_FAIL(PORTn,1);//抄表成功失败地址记录:入口:OK=0失败，=1成功,2=成功但数据为空
								y=0;//0=不抄
							}
						}
					}
				}
				if(y==1)
				{//要抄
					break;
				}
			#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
				if(PORTn==RS485_4PORT)
				{//载波口
					if(UARTCtrl->RMMeterCountAuto!=0)//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
					{//路由主导的周期抄表模式
						if(y==2)
						{
							return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
						}
						return 0;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
					}
				}
			#endif
				y=0;
				if(UARTCtrl->RMMeterCountAuto!=0)//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
				{
					break;
				}
				i=RM_MeterCount(PORTn);//电能表计数+1;返回:0=正常,1=任务错
				if(i)
				{
					return 0;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				}
			}
			if(y==0)
			{
				return 0;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
			}
			
			UARTCtrl->CONNECT=1;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中
			UARTCtrl->ReRMCount=0;//抄表重发计数
		#if(USER/100)==17//江苏系
			if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
			//if(((UARTCtrl->TaskID>>20)&0xf)==5)
				UARTCtrl->RMprotocol=3;
		#endif
			switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
			{
				case 1://DL/T645-1997
				case 2://DL/T645-2007
				case 3://DL/T698.45
					break;
				case 4://CJ/T 188-2004
					break;
				case ProtocolNO_ABB://ABB
					x=RM_TxDATA(PORTn,pCSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
					if(x)
					{
						UARTCtrl->TxByte=x;//发送字节数
					}
					return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case ProtocolNO_Landis://Landis(IEC1107 模式C)
					x=RM_TxDATA(PORTn,pCSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
					if(x)
					{
						UARTCtrl->TxByte=x;//发送字节数
					}
					return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
			}
		case 100://100=连接
			UARTCtrl->CONNECT=101;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中
			UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
			UARTCtrl->RMCSDCount=0;//抄表CSD计数
			UARTCtrl->RMCSDSubCount=0;//抄表CSD子计数
			UARTCtrl->RMROADCount=0;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
			UARTCtrl->RMROADCountOK=0;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
			UARTCtrl->ReRMCount=0;//抄表重发计数
			UARTCtrl->RM_OKFAIL=0;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
			UARTCtrl->RM_DI_TI_Count=0;//按时标间隔采集时的间隔计数
			UARTCtrl->RM_Rx_date_40020200[0]=0;//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
			RM_RxDATA_START(PORTn);//抄表接收数据处理-开始
			break;
		case 101://101=抄表中
			i=i;
			break;
		case 102://102=关闭连接
			switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
			{
				default:
				case 1://DL/T645-1997
				case 2://DL/T645-2007
					//下电能表
					RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
					return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case 3://DL/T698.45
					//下电能表
					RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
					return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case 4://CJ/T 188-2004
					//下电能表
					RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
					return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case ProtocolNO_ABB://ABB
					x=RM_TxDATA(PORTn,pCSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
					if(x)
					{
						UARTCtrl->TxByte=x;//发送字节数
					}
					return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				case ProtocolNO_Landis://Landis(IEC1107 模式C)
					x=RM_TxDATA(PORTn,pCSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
					if(x)
					{
						UARTCtrl->TxByte=x;//发送字节数
					}
					return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
			}
	}
	
#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
	if(UARTCtrl->RMMeterCountAuto==1)//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
	{
		NUM_SomeTaskID=UARTCtrl->NUM_SomeTaskID;//当前同类采集任务的id号的列表个数
		n=0;
		y=0;
		for(n=0;n<NUM_SomeTaskID;n++)
		{
			y=UARTCtrl->TaskID;
			y&=0xffff0000;
			y|=UARTCtrl->SomeTaskid[n];//RMTASKmax*2 byte 当前同类采集任务的b15-b8任务号,b7-b0方案编号
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
			return 3;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
		}
	}
#endif
	while(1)
	{
		pCSD=RM_CSD(PORTn);//取抄表数据标识CSD;返回:0=无效,1=CSD在URATCtrl,其他值=透明抄读帧数据指针
		if(pCSD!=0)
		{//有效
			if((u32)pCSD==1)
			{//普通采集方案,事件采集方案
				pCSD=(u8*)&UARTCtrl->RMCSD;//1=CSD在URATCtrl
				switch((UARTCtrl->TaskID>>20)&0xf)//采集类型
				{
					default:
						break;
					case 0://0=采集当前数据
					case 1://1=采集上第N次
					case 2://2=按冻结时标采集
					case 3://3=按时标间隔采集
						i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//采集数据的OI
						switch(i)
						{//
							case 0x5002://分钟冻结
								//采曲线数据,按任务执行间隔时间和采集方案采集间隔时间计算
								DateSaveTI(PORTn,UARTCtrl->date_time_s);//普通采集方案的采集存储时标;返回:pOUT=date_time_s
								FileID=Get_FileID(PORTn);//UART各任务的FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0保留
								//FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注
								Comm_Ram->msFILESearchCount=0;
								filename=Get_RecordFileName(FileID,0xffffffff,UARTCtrl->date_time_s);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
								if(filename!=0xffffffff)
								{//找到
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
										y=NUM_RMmax;
									}
									ms=Get_ms();
									ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
									RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
									Addr0=UARTCtrl->RMAddr[0];
									Addr1=UARTCtrl->RMAddr[1];
									Addr2=UARTCtrl->RMAddr[2];
									for(i=0;i<y;i++)
									{
										if(RecordFileHead->Addr0==Addr0)
										{
											if(RecordFileHead->Addr1==Addr1)
											{
												if(RecordFileHead->Addr2==Addr2)
												{
													pCSD=0;//已抄
													x=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
													x&=0xf0;
													x|=1;
													UARTCtrl->RM_OKFAIL=x;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
													break;
												}
											}
										}
										RecordFileHead++;
									}
								}
								break;
							default:
								break;
						}
						break;
					case 4://4=RetryMetering 补抄(类同,2=按冻结时标采集)
						i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//采集数据的OI
						switch(i)
						{//
							case 0x5002://分钟冻结
							case 0x5004://日冻结
								DateSaveTI(PORTn,UARTCtrl->date_time_s);//普通采集方案的采集存储时标;返回:pOUT=date_time_s
								FileID=Get_FileID(PORTn);//UART各任务的FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0保留
								//FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注
								Comm_Ram->msFILESearchCount=0;
								filename=Get_RecordFileName(FileID,0xffffffff,UARTCtrl->date_time_s);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
								if(filename!=0xffffffff)
								{//找到
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
										y=NUM_RMmax;
									}
									ms=Get_ms();
									ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
									RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
									Addr0=UARTCtrl->RMAddr[0];
									Addr1=UARTCtrl->RMAddr[1];
									Addr2=UARTCtrl->RMAddr[2];
									for(i=0;i<y;i++)
									{
										if(RecordFileHead->Addr0==Addr0)
										{
											if(RecordFileHead->Addr1==Addr1)
											{
												if(RecordFileHead->Addr2==Addr2)
												{
													pCSD=0;//已抄
													x=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
													x&=0xf0;
													x|=1;
													UARTCtrl->RM_OKFAIL=x;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
													break;
												}
											}
										}
										RecordFileHead++;
									}
								}
								break;
							default:
								pCSD=0;//已抄
								break;
						}
						break;
				}
			//帧数据
				if(pCSD!=0)
				{
				//交采
					if(PORTn==ACSAMPLEPORT)
					{//交采
						p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
						x=RM_TxDATA(PORTn,pCSD,p8rx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数						
						if(x)
						{
							p8rx[1]=0xaa;
							p8rx[2]=0xaa;
							p8rx[3]=0xaa;
							p8rx[4]=0xaa;
							p8rx[5]=0xaa;
							p8rx[6]=0xaa;
							UARTCtrl->TxByte=x;//发送字节数
							#ifdef DEBUG_JSON
							for(i=0;i<x;i++)
								myprintf("%02x ",p8rx[i],0,0);
							myprintf("\n",0,0,0);
							#endif
							return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
						}
					}
				//RS485
					if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
					{//RS485
						switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
						{
							case 1://DL/T645-1997
							case 2://DL/T645-2007
							case 3://DL/T698.45
								x=RM_TxDATA(PORTn,pCSD,p8tx+4);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
								if(x)
								{
									p8tx[0]=0xfe;
									p8tx[1]=0xfe;
									p8tx[2]=0xfe;
									p8tx[3]=0xfe;
									x+=4;
									UARTCtrl->TxByte=x;//发送字节数
									#ifdef DEBUG_JSON
									for(i=0;i<x;i++)
										myprintf("%02x ",p8tx[i],0,0);
									myprintf("\n",0,0,0);
									#endif
									return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
								}
								break;
							default:
								x=RM_TxDATA(PORTn,pCSD,p8tx);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
								if(x)
								{
									UARTCtrl->TxByte=x;//发送字节数
									return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
								}
								break;
						}
					}
				//载波
					if(PORTn==RS485_4PORT)
					{//载波
						switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
						{
							case 1://DL/T645-1997
							case 2://DL/T645-2007
							case 3://DL/T698.45
								if(Comm_Ram->RouterType==10)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能,8=钱龙,9=华瑞,10=新开普
								{//新开普强制集中器主动抄表,同时不加0xfe
									x=RM_TxDATA(PORTn,pCSD,p8tx+128);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
									if(x)
									{
										UARTCtrl->TxByte=x;//发送字节数
										return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
									}
									break;
								}
								x=RM_TxDATA(PORTn,pCSD,p8tx+128+4);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
								//myprintf("[%s:%d]:载波组帧完成字节数%d\n",(u32)&__func__,(u32)__LINE__,x);
								if(x)
								{//发送字节数
									p8tx[128]=0xfe;
									p8tx[129]=0xfe;
									p8tx[130]=0xfe;
									p8tx[131]=0xfe;
									x+=4;
									UARTCtrl->TxByte=x;//发送字节数
									//for(x=0;x<UARTCtrl->TxByte;x++)
										//myprintf("%02x ",p8tx[x],0,0);
									//myprintf("\n",0,0,0);
									return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
								}
								break;
							default:
								x=RM_TxDATA(PORTn,pCSD,p8tx+128);//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数 
								if(x)
								{
									UARTCtrl->TxByte=x;//发送字节数
									return 2;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
								}
								break;
						}
					}
				}
			}
			else
			{//透明方案
				
				
				
			}
		}
		i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
		i++;
		UARTCtrl->RMCSDSubCount=i;
		x=UARTCtrl->RMCSDSubCountMax;
		if(i>=x)
		{
			//1个完整的CSD的数据
			i=RM_One_CSD(PORTn);//检查抄表是否完成1个完整的CSD的数据;返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127
			if(i)
			{
				switch(i)
				{
					case 1:
						i=CopyString((u8 *)"缓冲1长度不够",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case 2:
						i=CopyString((u8 *)"缓冲2长度不够",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case 3:
						i=CopyString((u8 *)"缓冲中有关长度值错",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case 4:
						i=CopyString((u8 *)"无对应接口类号",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case 5:
						i=CopyString((u8 *)"数组数大于127",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					case 6:
						i=CopyString((u8 *)"抄表计数大于最大数",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
				}
				p8=(u8*)ADDR_DATABUFF;
				p8[i]=0;
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=1000/10;//下帧启动发送延时,以显示错误码
			}
			
			UARTCtrl->RMCSDSubCount=0;//抄表CSD子计数
			i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数
			i++;
			UARTCtrl->RMROADCount=i;
			x=UARTCtrl->RMROADCountMax;
			if(i>=x)
			{
				UARTCtrl->RMROADCount=0;//抄表CSD的ROAD内OAD计数
				UARTCtrl->RMROADCountOK=0;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
				i=UARTCtrl->RMCSDCount;//抄表CSD计数
				i++;
				UARTCtrl->RMCSDCount=i;
				x=UARTCtrl->RMCSDCountMax;
				if(i>=x)
				{//本表抄表完成
					RecordFileHead=(RecordFileHead_TypeDef*)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
					p16=(u16 *)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
					p16+=(LEN_RM_DI_CSD)/2;//已填写入buff完整CSD的长度
					i=p16[1];
					#if(USER/100)==17
					i=(UARTCtrl->TaskID>>20)&0xf;
					if(i==4||i==3)//补抄//按采集时标采集的分钟曲线
					{
						i=1;
					}else
					{
						i=p16[1];
					}
					#endif
					if(i)
					{//有数据填入
						//写记录
						RM_RxDATA_END(PORTn);//抄表接收数据处理-结束
						RM_RecordFile(PORTn,(u8*)RecordFileHead);//抄表记录
						//清抄表成功判断数据
						p16[1]=0;
						x=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
						x&=0xf0;
						x|=1;
						UARTCtrl->RM_OKFAIL=x;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
					}
					else
					{//无数据填入
						//清抄表成功判断数据
						p16[1]=0;
						x=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
						if((x&0xf)==0)//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
						{
							x|=2;
							UARTCtrl->RM_OKFAIL=x;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
						}
					}
					switch((UARTCtrl->TaskID>>20)&0xf)//采集类型
					{
						default:
						case 0://0=采集当前数据
						case 1://1=采集上第N次
						case 2://2=按冻结时标采集
						case 3://3=按时标间隔采集
							i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//采集数据的OI
							switch(i)
							{
								case 0x5002://分钟冻结(采曲线数据)
									i=UARTCtrl->TaskID;
									i>>=20;
									i&=0xf;
									switch(i)//普通采集方案的采集类型
									{
										default:
										case 0://采集当前数据,
										case 1://采集上第N次,
										case 2://按冻结时标采集,
											break;
										case 3://按时标间隔采集(曲线)
											UARTCtrl->RM_DI_TI_Count++;//按时标间隔采集时的间隔计数
											//按任务执行间隔时间和采集方案采集间隔时间计算
											i=UARTCtrl->TaskID;
											i>>=8;
											i&=0xff;//任务号
											//任务执行间隔时间
											x=ADDR_6012_2_SDRAM+(i*LENper_6012_2);
											i=pTItoSECOND((u8*)x+5);//TI转为秒值,出口:hex秒
											if(PORTn==RS485_4PORT)
											{//是载波口
											#if (CurveTaskTImax)
												if(i<CurveTaskTImax)//(24*60*60)//曲线抄表任务执行频率最大值(秒),此值决定按采集方案采集间隔时间可向前补抄的曲线点数
												{
													i=CurveTaskTImax;
												}
											#endif
											}
											//采集方案采集间隔时间
											x=pTItoSECOND(UARTCtrl->RM_DI_TI);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
											//#if (USER/100)==17
											//if(UARTCtrl->RM_DI_TI_Count>0)
											//{
												//x*=(UARTCtrl->RM_DI_TI_Count-1);
											//}
											//#else
											//x*=UARTCtrl->RM_DI_TI_Count;
											//#endif
											x*=UARTCtrl->RM_DI_TI_Count;
											if(x<i)
											{
												UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
												UARTCtrl->RMCSDCount=0;//抄表CSD计数
												UARTCtrl->RMCSDSubCount=0;//抄表CSD子计数
												UARTCtrl->RMROADCount=0;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
												UARTCtrl->RMROADCountOK=0;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
												UARTCtrl->ReRMCount=0;//抄表重发计数
												x=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
												if((x>>4)!=2)
												{
													if((x&0xf)==2)
													{
														x=0x20;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
													}
												}
												x&=0xf0;
												UARTCtrl->RM_OKFAIL=x;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
												RM_RxDATA_START(PORTn);//抄表接收数据处理-开始
												continue;
											}
											//按采集间隔的点数抄完
											x=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
											x>>=4;
											if(x!=2)
											{
												x=1;
											}
											UARTCtrl->RM_OKFAIL=x;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
											break;
										case 4://RetryMetering 补抄
											break;
									}
									break;
								//========国网台体改动==========
								case 0x5006://月冻结
								case 0x5004://日冻结
									UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
									UARTCtrl->RMCSDCount=0;//抄表CSD计数
									UARTCtrl->RMCSDSubCount=0;//抄表CSD子计数
									UARTCtrl->RMROADCount=0;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
									UARTCtrl->RMROADCountOK=0;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
									UARTCtrl->ReRMCount=0;//抄表重发计数
									break;
								default:
									break;
							}
							break;
						case 4://4=RetryMetering 补抄(类同,2=按冻结时标采集)
							switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
							{
								default:
									break;
								case 2://DL/T645-2007
								case 3://DL/T698.45
									UARTCtrl->RM_DI_TI_Count++;//按时标间隔采集时的间隔计数
									//#if(USER/100)==17
									//if((UARTCtrl->RM_DI_TI_Count-1)<UARTCtrl->RM_DI_N)
									//#else
									//if(UARTCtrl->RM_DI_TI_Count<UARTCtrl->RM_DI_N)
									//#endif
									if(UARTCtrl->RM_DI_TI_Count<UARTCtrl->RM_DI_N)
									{
										UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
										UARTCtrl->RMCSDCount=0;//抄表CSD计数
										UARTCtrl->RMCSDSubCount=0;//抄表CSD子计数
										UARTCtrl->RMROADCount=0;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
										UARTCtrl->RMROADCountOK=0;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
										UARTCtrl->ReRMCount=0;//抄表重发计数
										x=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
										if((x>>4)!=2)
										{
											if((x&0xf)==2)
											{
												x=0x20;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
											}
										}
										x&=0xf0;
										UARTCtrl->RM_OKFAIL=x;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
										RM_RxDATA_START(PORTn);//抄表接收数据处理-开始
										continue;
									}
									//按采集的点数抄完
									x=UARTCtrl->RM_OKFAIL;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
									x>>=4;
									if(x!=2)
									{
										x=1;
									}
									UARTCtrl->RM_OKFAIL=x;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
									break;
							}
							break;
					}
					
					RM_OK_FAIL(PORTn,UARTCtrl->RM_OKFAIL);//抄表成功失败地址记录:入口:OK=0失败，=1成功,2=成功但无数据
					
					//发关闭连接帧
					switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
					{
						default:
						case 1://DL/T645-1997
						case 2://DL/T645-2007
							//下电能表
							RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
							return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
						case 3://DL/T698.45
							//下电能表
							RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
							return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
						case 4://CJ/T 188-2004
							//下电能表
							RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
							return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
						case ProtocolNO_ABB://ABB
							//下电能表
							RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
							return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
						case ProtocolNO_Landis://Landis
							//下电能表
							RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
							return 1;//返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
					}
				
				}
			}
		}
	}
}











