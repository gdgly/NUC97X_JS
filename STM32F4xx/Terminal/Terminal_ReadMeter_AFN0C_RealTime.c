
//实时抄表1类数据
#include "Project.h"
#include "Terminal_ReadMeter_AFN0C_RealTime.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"

#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_DL645_1997_Lib.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "Terminal_ReadMeter_DL645_1997.h"
#include "Terminal_ReadMeter_3762_Std.h"

#include "Terminal_ReadMeter_WSH41_Lib.h"
#include "Terminal_ReadMeter_WSH41.h"
#include "Terminal_ReadMeter_ShH_Lib.h"
#include "Terminal_ReadMeter_ShH.h"

#if ((Project/100)==3)&&((USER/100)==12)//四川专变
	#include "Terminal_ReadMeter_HX_6E_Lib.h"	 
	#include "Terminal_ReadMeter_HX_6E.h"		 
	extern const u32 HX_6E_RMLib[];
#endif
#include "Terminal_ReadMeter_DL698_Lib.h"
#include "Terminal_ReadMeter_DL698.h"



extern const u32 DL645_2007_RMLib[];
extern const u32 DL645_1997_RMLib[];
extern const u32 WSH41_RMLib[];
extern const u32 ShH_RMLib[];
extern const u32 DL698_RMLib[];
u32 Wait_ReadMeter_RealTime(u32 PORTn,u32 PORTRS485,u32 Fn,u32 Protocol)//等待1个Fn数据实时抄读完成
{
  UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
	u16 *p16;
#endif

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
	p32=(u32 *)(ADDR_MS_ENTRY);
	
	switch(Protocol)
	{
		case 1://DL/T645-1997(1)
			UARTCtrl->FnCount=Get_RM_DL645_1997_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	#if ((Project/100)==3)&&((USER/100)==12)//四川专变
		case 4:
			UARTCtrl->FnCount=Get_RM_HX_6E_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;			
	#endif
	#if (USER/100)==10//河北专用
		case 7://威胜规约(河北)
			UARTCtrl->FnCount=Get_RM_WSH41_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	#endif
	#if (USER/100)==5//上海专用
		case 21://上海规约
			UARTCtrl->FnCount=Get_RM_ShH_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
		case 3://DL698规约
			UARTCtrl->FnCount=Get_RM_DL698_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	#endif
		default://其他都认为DL/T645-2007
			UARTCtrl->FnCount=Get_RM_DL645_2007_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	}
	if(UARTCtrl->FnCount==0xff)
	{
		return 0;//没得到Fn值的相应抄表计数号
	}
	switch(PORTRS485)
	{
		case RS485_1PORT:
		case RS485_2PORT:
		case RS485_3PORT:
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->Task=3;//3=启动发送
			while(UARTCtrl->Task!=0)
			{
				switch(Protocol)
				{
					case 1://DL/T645-1997(1)
					#if Extern_Terminal_ReadMeter_DL645_1997_Fn==0//外部程序:0=I2,1=E1,2=E2,3=;仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
						Terminal_ReadMeter_DL645_1997_Fn(PORTRS485,(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount]);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
					#else
						if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
						{
		    			p32=(u32 *)(ADDR_E1_ENTRY);
		    			(*(void(*)())p32[102])(PORTRS485,(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount]);//102 仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
							p32=(u32 *)(ADDR_I0_ENTRY);
						}
						else
						{
							return 0;
						}
					#endif
						break;
				#if ((Project/100)==3)&&((USER/100)==12)//四川专变
					case 4:	
						Terminal_ReadMeter_HX_6E_Fn(PORTRS485,(u8 *)HX_6E_RMLib[UARTCtrl->FnCount]);
						break;
				#endif
				#if (USER/100)==10//河北专用
					case 7://威胜规约(河北)
						Terminal_ReadMeter_WSH41_Fn(PORTRS485,(u8 *)WSH41_RMLib[UARTCtrl->FnCount]);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
						break;
				#endif
				#if (USER/100)==5//上海专用
					case 21://上海规约
					#if Extern_Terminal_ReadMeter_ShH_Fn==0//外部程序:0=I2,1=E1,2=E2,3=;仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
						Terminal_ReadMeter_ShH_Fn(PORTRS485,(u8 *)ShH_RMLib[UARTCtrl->FnCount]);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
					#else
						if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
						{
		    			p32=(u32 *)(ADDR_E1_ENTRY);
		    			(*(void(*)())p32[113])(PORTRS485,(u8 *)ShH_RMLib[UARTCtrl->FnCount]);//113 仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
							p32=(u32 *)(ADDR_I0_ENTRY);
						}
						else
						{
							return 0;
						}
					#endif
						break;
					case 3://698规约
						Terminal_ReadMeter_DL698_Fn(PORTRS485,(u8 *)DL698_RMLib[UARTCtrl->FnCount]);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
						break;
				#endif
					default://其他都认为DL/T645-2007
						Terminal_ReadMeter_DL645_2007_Fn(PORTRS485,(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount]);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
						break;
				}
				
				(*(void(*)())p32[115])();//调用I0_Main_Return;63 开环主任务
			#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
				if(Terminal_Ram->PowerTask<=10)//停电事件任务
				{
					return 0;
				}
			#endif
			}
			break;
		case RS485_4PORT://载波
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			Terminal_Router->RouterRealTimeTask=1;//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
			while(Terminal_Router->RouterRealTimeTask!=0)
			{
				if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
				{//路由器不存在,或错误等
					Terminal_Router->RouterRealTimeTask=0;//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
					return 0;
				}
				if(Terminal_Router->RouterCtrl==0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				{
					switch(Protocol)
					{
						case 1://DL/T645-1997(1)
							RealTimeReadMeter_3762_Fn_Std(PORTRS485,(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount]);//仅抄1个Fn数据填写到FnDataBuff;RouterRealTimeTask=0=空闲(本Fn抄读完成)
							break;
					#if (USER/100)==5//上海专用
						case 21://上海规约
							RealTimeReadMeter_3762_Fn_Std(PORTRS485,(u8 *)ShH_RMLib[UARTCtrl->FnCount]);//仅抄1个Fn数据填写到FnDataBuff;RouterRealTimeTask=0=空闲(本Fn抄读完成)
							break;
					#endif
						default://DL/T645-2007(30),串行接口连接窄带低压载波(31)
							RealTimeReadMeter_3762_Fn_Std(PORTRS485,(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount]);//仅抄1个Fn数据填写到FnDataBuff;RouterRealTimeTask=0=空闲(本Fn抄读完成)
							break;
					}
					if(UARTCtrl->Task==2)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
					#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
						p16=(u16 *)Get_ADDR_UARTn(PORTn);
						DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
						if(p16[0]!=p16[1])
						{//已接收到下帧
							if(Terminal_Router->StopFlags&(1<<7))//b7=实时抄读
							{
								Terminal_Router->StopFlags&=~(1<<7);//恢复路由
								Terminal_Router->RouterCtrl=3;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
							}
							Terminal_Router->RouterRealTimeTask=0;//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
							return 0;//0=没读
						}
					#endif
					}
				}
				(*(void(*)())p32[115])();//调用I0_Main_Return;63 开环主任务
			#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
				if(Terminal_Ram->PowerTask<=10)//停电事件任务
				{
					Terminal_Router->RouterRealTimeTask=0;//载波Fn>=129的实时抄读任务;0=空闲,1=暂停路由,2=发送,3=接收
					return 0;
				}
			#endif
			}
	#endif		
			break;
	}
	return 1;
}

u32 ReadMeter_RealTime(u32 PORTn,u32 Fn,u32 Pn)//实时读取1类数据;返回0=没读,!=0读且是FnDataBuff地址
{
	u32 i;
	u32 MeterNo;//电能表配置序号
	u32 PORTRS485;
	u32 Protocol;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
	u16 *p16;
#endif

	if(Pn==0)
	{
		return 0;//0=没读
	}
	MeterNo=Check_AFN04F10_Pn(Pn);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
	if(MeterNo==0)
	{
		return 0;//0=没读
	}
	i=MRR(ADDR_AFN04F10+((MeterNo-1)*LEN_AFN04F10_Pn)+4,1);//通信速率及端口号
	i&=0x1f;
	switch(i)
	{
		case 2://485-1
			PORTRS485=RS485_1PORT;
			break;
		case 3://485-2
			PORTRS485=RS485_2PORT;
			break;
		case 4://485-3
			PORTRS485=RS485_3PORT;
			break;
		case 31://载波
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			{
				PORTRS485=RS485_4PORT;
			}
			else
			{
				return 0;
			}
			break;
	#else
			return 0;
	#endif	
		default:
			return 0;//0=没读
	}
	Protocol=MRR(ADDR_AFN04F10+((MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1 
	switch(Protocol)
	{
		case 1://DL/T645-1997(1)
		#if Extern_Terminal_ReadMeter_DL645_1997_Fn==1//外部程序:0=I2,1=E1,2=E2,3=;仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
			if((Comm_Ram->SoftModule&(1<<4))!=0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				return 0;//0=没读
			}
		#endif
			i=Get_RM_DL645_1997_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	#if ((Project/100)==3)&&((USER/100)==12)//四川专变
		case 4:
			i=Get_RM_HX_6E_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	#endif
	#if (USER/100)==10//河北专用
		case 7://威胜规约(河北)
			i=Get_RM_WSH41_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	#endif
	#if (USER/100)==5//上海专用
		case 21://上海规约
		#if Extern_Terminal_ReadMeter_ShH_Fn==1//外部程序:0=I2,1=E1,2=E2,3=;仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
			if((Comm_Ram->SoftModule&(1<<4))!=0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				return 0;//0=没读
			}
		#endif
			i=Get_RM_ShH_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
		case 3://698规约
			i=Get_RM_DL698_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	#endif
		default://其他都认为DL/T645-2007
			i=Get_RM_DL645_2007_FnCount(Fn);//得到Fn值的相应抄表计数号;返回0xffffffff表示没找到
			break;
	}

	if(i==0xffffffff)
	{//没得到Fn值的相应抄表计数号
		return 0;//0=没读
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
	p32=(u32 *)(ADDR_MS_ENTRY);
#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
	p16=(u16 *)Get_ADDR_UARTn(PORTn);
	DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
	if(p16[0]!=p16[1])
	{//已接收到下帧
		return 0;//0=没读
	}
#endif
	while((UARTCtrl->Lock==0x55)||((UARTCtrl->Task!=0)&&(UARTCtrl->Task!=2)))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{//间隔暂停时,等待接收模块的主动上报会进入超时状态
		(*(void(*)())p32[115])();//调用I0_Main_Return;63 开环主任务
		if(PORTRS485==RS485_4PORT)
		{
			if(UARTCtrl->Lock==0x55)
			{
				return 0;//0=没读
			}
		}
	#if MulRealRead==1//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧
		DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
		if(p16[0]!=p16[1])
		{//已接收到下帧
			return 0;//0=没读
		}
	#endif
	#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
		if(Terminal_Ram->PowerTask<=10)//停电事件任务
		{
			return 0;
		}
	#endif
	}
	//
	UARTCtrl->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
	UARTCtrl->MeterNoBackup=UARTCtrl->MeterNo;//485抄表电能表/交流采样装置序号(半字对齐)(集中抄表插入抄读实时数据时备份)
	UARTCtrl->MeterNo=MeterNo;
	UARTCtrl->FnCountBackup=UARTCtrl->FnCount;//485抄表Fn计数(插入抄读实时数据时备份)
	UARTCtrl->RMCountBackup=UARTCtrl->RMCount;//485抄表命令计数(插入抄读实时数据时备份)
	
	i=Wait_ReadMeter_RealTime(PORTn,PORTRS485,Fn,Protocol);//等待1个Fn数据实时抄读完成
	
	UARTCtrl->MeterNo=UARTCtrl->MeterNoBackup;//485抄表电能表/交流采样装置序号(半字对齐)(插入抄读实时数据时备份)
	UARTCtrl->FnCount=UARTCtrl->FnCountBackup;//485抄表Fn计数(插入抄读实时数据时备份)
	UARTCtrl->RMCount=UARTCtrl->RMCountBackup;//485抄表命令计数(插入抄读实时数据时备份)
	UARTCtrl->SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
	if((UARTCtrl->FnCount<2)||(UARTCtrl->FnCount>3))
	{//原不是抄电能表事件
		UARTCtrl->RMCount=0;//485抄表命令计数
	}
	UARTCtrl->ReRMCount=0;//485抄表命令重发计数
	UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
	UARTCtrl->Task=0;
	if(i==0)
	{
		return 0;
	}
	return (Get_ADDR_UARTnFnDataBuff(PORTRS485)+1);//!=0读
}




