
#include "AllHead.h"
#include "Terminal_ReadMeter_Fn.h"
#include "../Calculate/Calculate.h"
#include "../Driver/MEMRW.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "../LPC177x8x/lpc177x8x_UART.h"
#include "KeyUser.h"



#if Project==200//集中器
#if (USER/100)==2//山东系



#define RMM_TimerTask     0//0=不抄,1=自动抄读定时任务配置的相关数据
#define RMM_Event     0//0=不抄,1=自动抄读事件记录配置的相关数据


#if RMM_TimerTask==1//0=不抄,1=自动抄读定时任务配置的相关数据
u32 CompFnPn(u32 DI,u32 Fn,u32 Pn)//分解DI为Fn,Pn且比较;返回0=不同,1=有相同;使用ADDR_DATABUFF
{
	u32 FnPn;
	u8 *p8;
	u32 *p32;
	p8=(u8*)(ADDR_DATABUFF);
	p32=(u32*)(ADDR_DATABUFF);
	
	p32[0]=DI;
	//Pn
	DI&=0xffff;
	if(DI!=0x00ff)
	{//不是全部有效测量点
		if(p8[1]==0x0)
		{
			return 0;//Pn=0;
		}
		else
		{
			if(p8[0]==0x0)
			{
				return 0;//Pn=0;
			}
			else
			{
				while(1)
				{
					FnPn=BitLoca(p8[0]);//计算第1个置位为1的位置(0-32)
					DI=p8[0];
					DI<<=1;
					DI&=(~(1<<FnPn));
					DI>>=1;
					p8[0]=DI;
					FnPn+=((p8[1]-1)*8);
					if(FnPn==Pn)
					{
						break;
					}
					if(DI==0)
					{//分解完
						return 0;
					}
				}
			}
		}
	}
	//Fn
	if(p8[2]==0x0)
	{
		return 0;//Fn=0;
	}
	else
	{
		while(1)
		{
			FnPn=BitLoca(p8[2]);//计算第1个置位为1的位置(0-32)
			DI=p8[2];
			DI<<=1;
			DI&=(~(1<<FnPn));
			DI>>=1;
			p8[2]=DI;
			FnPn+=(p8[3]*8);
			if(FnPn==Fn)
			{
				return 1;
			}
			if(DI==0)
			{//分解完
				return 0;
			}
		}
	}
}
u32 ReadMeter_Fn_Class1TimerTask(u32 Fn,u32 Pn)//FnPn在1类数据定时任务,返回:0=没,1=有;使用 ADDR_DATABUFF
{
	u32 i;
	u32 Task;
	u32 MaxDINum;
	u32 DI;
	
	for(Task=0;Task<AFN04F65MaxPn;Task++)
	{
		i=MRR(ADDR_AFN04F67+Task,1);//定时发送1类数据任务启动／停止设置(任务号)
		if(i!=0x55)
		{//置"55H"：启动；置"AAH"停止；其它数值无效
			continue;
		}
		MaxDINum=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Task)+8,1);
		if(MaxDINum>MaxTimerTaskDI)
		{
			MaxDINum=MaxTimerTaskDI;
		}
		for(i=0;i<MaxDINum;i++)
		{
			DI=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Task)+9+(4*i),4);//取DI
			DI=CompFnPn(DI,Fn,Pn);//分解DI为Fn,Pn且比较;返回0=不同,1=有相同;使用ADDR_DATABUFF
			if(DI!=0)
			{
				return 1;
			}
		}
	}
	return 0;
}
u32 ReadMeter_Fn_Class2TimerTask(u32 Fn,u32 Pn)//FnPn在2类数据定时任务,返回:0=没,1=有
{
	u32 i;
	u32 Task;
	u32 MaxDINum;
	u32 DI;
	
	for(Task=0;Task<AFN04F66MaxPn;Task++)
	{
		i=MRR(ADDR_AFN04F68+Task,1);//定时发送2类数据任务启动／停止设置(任务号)
		if(i!=0x55)
		{//置"55H"：启动；置"AAH"停止；其它数值无效
			continue;
		}
		MaxDINum=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Task)+8,1);
		if(MaxDINum>MaxTimerTaskDI)
		{
			MaxDINum=MaxTimerTaskDI;
		}
		for(i=0;i<MaxDINum;i++)
		{
			DI=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Task)+9+(4*i),4);//取DI
			DI=CompFnPn(DI,Fn,Pn);//分解DI为Fn,Pn且比较;返回0=不同,1=有相同;使用ADDR_DATABUFF
			if(DI!=0)
			{
				return 1;
			}
		}
	}
	return 0;
}
#endif

u32 ReadMeter_Fn_Com(u32 Fn,u32 Pn,u32 ClassNo)
{
	u32 i;
	u32 y;
	
	y=0;
	i=GetClass2SourceDataAddr(Fn,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(i!=0)
	{
		MR(ADDR_DATABUFF,i+2,3);//取原抄表时间年月日
		i=DataComp(ADDR_DATABUFF,ADDR_TYMDHMS+3,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		if(i!=0)
		{
			y|=Class2Data_Fn(ClassNo,Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
		#if RMM_TimerTask==1//0=不抄,1=自动抄读定时任务配置的相关数据
			y|=ReadMeter_Fn_Class2TimerTask(Fn,Pn);//FnPn在2类数据定时任务,返回:0=没,1=有;使用 ADDR_DATABUFF
		#endif
		}
	}
	return y;
}





u32 ReadMeter_Fn_YesNo(u32 PORTn,u32 ClassNo,u32 Pn,u8 * p8RMLib,u32 RMCount,u32 Protocol)//按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数,Protocol(1=1997,其他=2007)
{
	u32 i;
	u32 y;
	u32 y1;
	u32 Fn;
	u64 d64a;
	u64 d64b;
  UARTCtrl_TypeDef * UARTCtrl;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	y=0;
	y1=0;
	
	if(PORTn!=RS485_4PORT)//RS485_4(载波)
	{//是485口
	#if RMM_RS485_PassageZero==1//RS485通道抄表过0点;0=不暂停,1=暂停
		i=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);
		if((i>=RMM_EndHM)||(i<RMM_StartHM))
		{//RS485口在每日0点附近暂停抄表
			if(p8RMLib[4]&((1<<6)+(1<<5)+(1<<3)))//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
			{//2类数据B6=补抄上2-3日冻结,B5=月冻结,B3=日冻结
				return 0;//不抄日月冻结数据
			}
		}
	#endif
	}
	switch(UARTCtrl->RMTaskC)
	{
		case 0://0=上电任务
		#if PowerOnRMTask==0//电源开抄表任务;0=没有,1=有
			#error PowerOnRMTask=0
		#endif
			if((p8RMLib[0]==129)||(p8RMLib[0]==33))
			{
				return 1;
			}
			return 0;
		case 1://1=每日任务
		#if DateRMTask==0//每日抄表任务;0=没有,1=有
			#error DateRMTask=0
		#endif
			if(Protocol!=1)
			{//DL645-2007
				if(p8RMLib[0]==0xE3)
				{//日冻结正反有无功,4个象限无功电能示值（总、费率1～M）
					Fn=GetClass2SourceDataAddr(161,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
					if(Fn!=0)
					{
		//				i=MRR(Fn-3,3);//取原抄表年月日
		//				Fn=MRR(ADDR_TYMDHMS+3,3);//取当前时间年月日
		//				if(i!=Fn)
		//				{//年月日不同
							if((RMCount>0)&&(RMCount<9))
							{
								if((RMCount==1)||(RMCount==3))
								{//正或反有功
									Fn=160+RMCount;//抄表命令计数1-8对应Fn=161-168
									y|=ReadMeter_Fn_Com(Fn,Pn,ClassNo);
								}
								else
								{
									y=0;
								}
							}
							else
							{//抄表命令计数=0或9,上1次日冻结时间YYMMDDhhmm
							#if RMM_CompCongealTime==1//读电能表冻结数据比较冻结时间:0=不比较,1=要比较
								y|=ReadMeter_Fn_Com(161,Pn,ClassNo);
								y|=ReadMeter_Fn_Com(162,Pn,ClassNo);
								y|=ReadMeter_Fn_Com(163,Pn,ClassNo);
								y|=ReadMeter_Fn_Com(164,Pn,ClassNo);
								y|=ReadMeter_Fn_Com(165,Pn,ClassNo);
								y|=ReadMeter_Fn_Com(166,Pn,ClassNo);
								y|=ReadMeter_Fn_Com(167,Pn,ClassNo);
								y|=ReadMeter_Fn_Com(168,Pn,ClassNo);
							#endif
							}
		//				}
					}
					return y;
				}
			}
			else
			{//DL645-1997
				if((p8RMLib[0]==129)||(p8RMLib[0]==131))
				{
					return 1;
				}
			}
			return 0;
		case 2://2=每周任务
		#if WeekRMTask==1//每周抄表任务;0=没有,1=有
			#error WeekRMTask=1
		#endif
			return 0;
		case 3://3=定时抄读停电数据任务
		#if TimerRMTask==1//每周抄表任务;0=没有,1=有
			#error TimerRMTask=1
		#endif
			return 0;
		case (MaxRMTask-1)://循抄任务
			break;
		default:
			return 0;
	}
	if(p8RMLib[0]==1)
	{//抄读重点户曲线(仅对无曲线冻结的单相表)
		return 0;//专在任务4抄
	}
	switch(PORTn)
	{
		case RS485_1PORT://RS485_1
		case RS485_2PORT://RS485_2
		case RS485_3PORT://RS485_3
	#if RMM_RS485==0//RS485通道抄表模式:0=循抄全部数据,1=按大小类号设置抄读数据
			if((p8RMLib[4]&0x20)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
			{//2类数据B5=月冻结
				y1=1;//全抄标志
				break;
			}
			if((p8RMLib[4]&0x04)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
			{//2类数据,曲线
				y=1;
				break;
			}
			return 1;//返回0=不抄,1=要抄
	#else
			if(ClassNo==0)
			{
				if((p8RMLib[4]&0x20)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				{//2类数据B5=月冻结
					y1=1;//全抄标志
					break;
				}
				if((p8RMLib[4]&0x04)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				{//2类数据,曲线
					y=1;
					break;
				}
				return 1;
			}
			break;
	#endif
		case RS485_4PORT://RS485_4(载波)
			if(ClassNo==0)
			{
				if((p8RMLib[4]&0x20)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				{//2类数据B5=月冻结
					y1=1;//全抄标志
					break;
				}
				if((p8RMLib[4]&0x04)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				{//2类数据,曲线
					y=1;
					break;
				}
				return 1;
			}
			break;
	}
	//1类数据
	if((p8RMLib[4]&0x01)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//1类数据,实时
		Fn=p8RMLib[0];//取实时Fn
		if(Fn==246)
		{//F246当前掉电记录数据,在定时任务中专抄
			return 0;
		}
//		if((Fn!=25)&&(Fn!=26))
//		{//F25,F26已在用周任务替代的每是时任务中抄
			y|=Class1Data_Fn(ClassNo,Fn);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持		
		#if RMM_TimerTask==1//0=不抄,1=自动抄读定时任务配置的相关数据
			y|=ReadMeter_Fn_Class1TimerTask(Fn,Pn);//FnPn在1类数据定时任务,返回:0=没,1=有;使用 ADDR_DATABUFF
		#endif
//		}
		//1类实时数据与2类曲线配置相关
	#if (RMM_RS485_IntegralCongeal|RMM_Carrier_IntegralCongeal)==0//抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
		if(y==0)
		{
			if(UARTCtrl->RMCount==0)
			{//仅当命令计数=0时列表,减少运行时间
				KeyUserPnList();//集中抄表重点户曲线冻结Pn列表
			}
			i=KeyUserPnSet(Pn);//重点户有效测量点号检查;返回0=无效,1=有效;使用重点户曲线冻结Pn列表ADDR_KeyUserPnList
			if(i!=0)
			{//是重点户
				//F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
				if(Fn==25)
				{
					if(Protocol==1)
					{//DL/T645-1997
						i=MRR((u32)&p8RMLib[10+(3*RMCount)],2);//取DI
						switch(i)
						{
						//F81-84测量点总,A,B,C相有功功率曲线
							case 0xb630://总有功功率
								y|=Class2Data_Fn(ClassNo,81);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb631://A有功功率
								y|=Class2Data_Fn(ClassNo,82);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb632://B有功功率
								y|=Class2Data_Fn(ClassNo,83);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb633://C有功功率
								y|=Class2Data_Fn(ClassNo,84);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
						//F85-88测量点总,A,B,C相无功功率曲线
							case 0xb640://总无功功率
								y|=Class2Data_Fn(ClassNo,85);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb641://A无功功率
								y|=Class2Data_Fn(ClassNo,86);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb642://B无功功率
								y|=Class2Data_Fn(ClassNo,87);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb643://C无功功率
								y|=Class2Data_Fn(ClassNo,88);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							//F105-108测量点总,A,B,C相功率因数曲线
							case 0xb650://总功率因数
								y|=Class2Data_Fn(ClassNo,105);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb651://A功率因数
								y|=Class2Data_Fn(ClassNo,106);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb652://B功率因数
								y|=Class2Data_Fn(ClassNo,107);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb653://C功率因数
								y|=Class2Data_Fn(ClassNo,108);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							//F89-91测量点A,B,C相电压曲线	
							case 0xb611://A相电压
								y|=Class2Data_Fn(ClassNo,89);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb612://B相电压
								y|=Class2Data_Fn(ClassNo,90);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xb613://C相电压
								y|=Class2Data_Fn(ClassNo,91);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							//F92-94测量点A,B,C相电流曲线
							case 0xB621://A相电流
								y|=Class2Data_Fn(ClassNo,92);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xB622://B相电流
								y|=Class2Data_Fn(ClassNo,93);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0xB623://C相电流
								y|=Class2Data_Fn(ClassNo,94);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
						}
					}
					else
					{//DL/T645-2007
						i=MRR((u32)&p8RMLib[10+(5*RMCount)],4);//取DI
						switch(i)
						{
							case 0x0203ff00://总,A,B,C有功功率
								//F81-84测量点总,A,B,C相有功功率曲线
								y|=Class2Data_Fn(ClassNo,81);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,82);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,83);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,84);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0x0204ff00://总,A,B,C无功功率
								//F85-88测量点总,A,B,C相无功功率曲线
								y|=Class2Data_Fn(ClassNo,85);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,86);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,87);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,88);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0x0206ff00://总,A,B,C功率因数
								//F105-108测量点总,A,B,C相功率因数曲线
								y|=Class2Data_Fn(ClassNo,105);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,106);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,107);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,108);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0x0201ff00://A,B,C相电压
								//F89-91测量点A,B,C相电压曲线
								y|=Class2Data_Fn(ClassNo,89);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,90);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,91);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0x0202ff00://A,B,C相电流
								//F92-94测量点A,B,C相电流曲线
								y|=Class2Data_Fn(ClassNo,92);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,93);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								y|=Class2Data_Fn(ClassNo,94);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0x02800001://零序电流
								//F95测量点零序电流曲线
								y|=Class2Data_Fn(ClassNo,95);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
								break;
							case 0x0205ff00://总,A,B,C视在功率
								break;
						}
					}
				}
				//F129 当前正向有功电能示值(已抄小时冻结)
				//F130 当前正向无功（组合无功1）电能示值
				if(Fn==130)
				{
					//F102测量点正向无功总电能示值曲线
					y|=Class2Data_Fn(ClassNo,102);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				}
				//F131 当前反向有功电能示值(已抄小时冻结)
				//F132 当前反向无功（组合无功2）电能示值
				if(Fn==132)
				{
					//F104测量点反向无功总电能示值曲线
					y|=Class2Data_Fn(ClassNo,104);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				}
				//F133 当前一象限无功电能示值
				if(Fn==133)
				{
					//F145测量点1象限无功总电能示值曲线
					y|=Class2Data_Fn(ClassNo,145);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				}
				//F134 当前二象限无功电能示值
				if(Fn==134)
				{
					//F147测量点2象限无功总电能示值曲线
					y|=Class2Data_Fn(ClassNo,147);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				}
				//F135 当前三象限无功电能示值
				if(Fn==135)
				{
					//F148测量点3象限无功总电能示值曲线
					y|=Class2Data_Fn(ClassNo,148);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				}
				//F136 当前四象限无功电能示值
				if(Fn==136)
				{
					//F146测量点4象限无功总电能示值曲线
					y|=Class2Data_Fn(ClassNo,146);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
				}
			}
		}
	#endif
	}
	//2类数据
	if((p8RMLib[4]&0x10)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//2类数据,抄表日冻结
		Fn=p8RMLib[8];//抄表日冻结Fn
		y|=Class2Data_Fn(ClassNo,Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持		
	#if RMM_TimerTask==1//0=不抄,1=自动抄读定时任务配置的相关数据
		y|=ReadMeter_Fn_Class2TimerTask(Fn,Pn);//FnPn在2类数据定时任务,返回:0=没,1=有;使用 ADDR_DATABUFF
	#endif
	}
	if((p8RMLib[4]&0x04)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//2类数据,曲线
		//当前库中设为曲线有:扩展上1-24整点电表冻结的正反有功电能示值或负荷曲线
		#if RMM_RS485_IntegralCongeal==1//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
			y=1;//已在抄表中判F101,F103是否配置
		#endif
		#if RMM_RS485_IntegralCongeal==2//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
			Fn=p8RMLib[0];//取实时Fn
			if(Fn==0xE4)
			{//RS485用读全部6类负荷曲线
				if(RMCount==0)
				{//仅当命令计数=0时列表,减少运行时间
					KeyUserPnList();//集中抄表重点户曲线冻结Pn列表
				}
				i=KeyUserPnSet(Pn);//重点户有效测量点号检查;返回0=无效,1=有效;使用重点户曲线冻结Pn列表ADDR_KeyUserPnList
				if(i!=0)
				{//是重点户
					//补当前时间到上日0点的x点曲线冻结号
					i=Comm_Ram->TYMDHMS[1];//当前RTC分
					i=bcd_hex(i);
					i/=15;
					Fn=Comm_Ram->TYMDHMS[2];//当前RTC时
					Fn=bcd_hex(Fn);
					Fn*=4;//每小时4点
					i+=Fn;
					i+=96+1;//昨天96点,0点算1点
					Fn=p8RMLib[0];//取实时Fn
					if(RMCount<i)
					{
						d64a=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
						d64a=YMDHM_Int15(d64a);//年月日时分化为整15分,如00时18分化为00时15分
						i=RMCount*15;//每15分间隔
						d64a=YMDHM_SubM(d64a,i);//年月日时分减m分,返回减m分后的年月日时分
						i=CheckCurveCongeal(d64a,101,Pn);//检查曲线冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
						if(i==0)
						{
							return 1;
						}
					}
				}
				return 0;
			}
			if((Fn>=0xE5)&&(Fn<=0xE9))
			{//载波用1-5类负荷曲线
			#if RMM_Carrier_CurveTime==15//载波通道读电表曲线的整冻结间隔时间分;15分或60分
				//补当前时间到上日0点的x点曲线冻结号
				i=Comm_Ram->TYMDHMS[1];//当前RTC分
				i=bcd_hex(i);
				i/=15;
				Fn=Comm_Ram->TYMDHMS[2];//当前RTC时
				Fn=bcd_hex(Fn);
				Fn*=4;//每小时4点
				i+=Fn;
				i+=96+1;//昨天96点,0点算1点
				Fn=p8RMLib[0];//取实时Fn
				if(RMCount>=i)
				{
					return 0;
				}
			#endif
			#if RMM_Carrier_CurveTime==60//载波通道读电表曲线的整冻结间隔时间分;15分或60分
				//补当前时间到上日0点的x点整点曲线冻结号
				i=Comm_Ram->TYMDHMS[2];//当前RTC时
				i=bcd_hex(i);
				i+=24+1;//昨天24点,0点算1点
				if(RMCount>=i)
				{
					return 0;
				}
			#endif
				if(RMCount==0)
				{//仅当命令计数=0时列表,减少运行时间
					KeyUserPnList();//集中抄表重点户曲线冻结Pn列表
				}
				i=KeyUserPnSet(Pn);//重点户有效测量点号检查;返回0=无效,1=有效;使用重点户曲线冻结Pn列表ADDR_KeyUserPnList
				if(i!=0)
				{//是重点户
					switch(Fn)
					{
						case 0xe5://1类负荷曲线(电压、电流、频率)
							Fn=89;//测量点A相电压曲线
							break;
						case 0xe6://2类负荷曲线(有无功功率)
							Fn=81;//测量点有功功率曲线
							break;
						case 0xe7://3类负荷曲线(功率因数)
							Fn=105;//测量点总功率因数
							break;
						case 0xe8://4类负荷曲线(正反有功,組合无功1和2总电能)
							Fn=101;//测量点正向有功总电能示值
							break;
						case 0xe9://5类负荷曲线(1-4象限无功总电能)
							Fn=145;//测量点1象限无功总电能示值曲线
							break;
					}
					y|=Class2Data_Fn(ClassNo,Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
					if(y!=0)
					{//有配置
						d64a=MRR(ADDR_TYMDHMS+1,5);//取当前时间年月日时分
					#if RMM_Carrier_CurveTime==15//载波通道读电表曲线的整冻结间隔时间分;15分或60分
						d64a=YMDHM_Int15(d64a);//年月日时分化为整15分,如00时18分化为00时15分
						i=RMCount*15;//每15分间隔
					#else
						d64a&=0xffffffff00;
						i=RMCount*60;//每60分间隔
					#endif
						d64a=YMDHM_SubM(d64a,i);//年月日时分减m分,返回减m分后的年月日时分
						i=CheckCurveCongeal(d64a,Fn,Pn);//检查曲线冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
						if(i==0)
						{
							return 1;
						}
					}
				}
				return 0;
			}
		#endif
	}
	if((p8RMLib[4]&0x08)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//2类数据B3=日冻结
		Fn=p8RMLib[7];//取日冻结Fn
		if(p8RMLib[0]==0xE3)
		{//集中抄表日冻结正反有无功,4个象限无功电能示值（总、费率1～M）
			Fn=GetClass2SourceDataAddr(161,Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			if(Fn!=0)
			{
//				i=MRR(Fn-3,3);//取原抄表年月日
//				Fn=MRR(ADDR_TYMDHMS+3,3);//取当前时间年月日
//				if(i!=Fn)
//				{//年月日不同
					if((RMCount>0)&&(RMCount<9))
					{
						if((RMCount!=1)&&(RMCount!=3))
						{//正反有功已在任务1中抄
							Fn=160+RMCount;//抄表命令计数1-8对应Fn=161-168
							y|=ReadMeter_Fn_Com(Fn,Pn,ClassNo);
						}
					}
					else
					{//抄表命令计数=0或9,上1次日冻结时间YYMMDDhhmm
					#if RMM_CompCongealTime==1//读电能表冻结数据比较冻结时间:0=不比较,1=要比较
						y|=ReadMeter_Fn_Com(161,Pn,ClassNo);
						y|=ReadMeter_Fn_Com(162,Pn,ClassNo);
						y|=ReadMeter_Fn_Com(163,Pn,ClassNo);
						y|=ReadMeter_Fn_Com(164,Pn,ClassNo);
						y|=ReadMeter_Fn_Com(165,Pn,ClassNo);
						y|=ReadMeter_Fn_Com(166,Pn,ClassNo);
						y|=ReadMeter_Fn_Com(167,Pn,ClassNo);
						y|=ReadMeter_Fn_Com(168,Pn,ClassNo);
					#endif
					}
//				}
			}
		}
		else
		{
			y|=ReadMeter_Fn_Com(Fn,Pn,ClassNo);
		}
	}
	if((p8RMLib[4]&0x20)!=0)//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
	{//2类数据B5=月冻结
		if(p8RMLib[0]==165)//F165 电能表开关操作次数及时间
		{//月冻结标志同时又是ERC37开表盖ERC38开端钮盒事件数据
			if(y1==1)//全抄标志
			{
				d64a=MRR(ADDR_AFN04F9,8);//事件记录有效标志位 BS64 8
				d64b=1;
				d64b<<=(37-1);
				d64b&=d64a;
				if(d64b!=0)
				{
					return 1;
				}
				d64b=1;
				d64b<<=(38-1);
				d64b&=d64a;
				if(d64b!=0)
				{
					return 1;
				}
			}
		}
		if(p8RMLib[0]==166)//F166 电能表参数修改次数及时间
		{//月冻结标志同时又是ERC41直接读电能表对时事件记录
			if(y1==1)//全抄标志
			{
				d64a=MRR(ADDR_AFN04F9,8);//事件记录有效标志位 BS64 8
				d64b=1;
				d64b<<=(41-1);
				d64b&=d64a;
				if(d64b!=0)
				{
					return 1;
				}
			}
		}
		Fn=p8RMLib[9];//取月冻结Fn
		i=y1;
		i|=Class2Data_Fn(ClassNo,Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
	#if RMM_TimerTask==1//0=不抄,1=自动抄读定时任务配置的相关数据
		i|=ReadMeter_Fn_Class2TimerTask(Fn,Pn);//FnPn在2类数据定时任务,返回:0=没,1=有;使用 ADDR_DATABUFF
	#endif
		if(i!=0)
		{//有配置
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//补抄上2月数据
				i=Comm_Ram->TYMDHMS[4]+(Comm_Ram->TYMDHMS[5]<<8);
				i=YM_Sub1M(i);//年月减1月,返回减1后的年月
				i=YM_Sub1M(i);//年月减1月,返回减1后的年月
				i=CheckMonthCongeal(i,Fn,Pn);//检查月冻结;返回:0=数据没冻结,1=数据已冻结,2=没找到有效冻结
				if(i==0)
				{//0=数据没冻结
					y|=1;
				}
			}
			else
			{
				i=GetClass1DataAddr(p8RMLib[0],Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
				if(i!=0)
				{
					if((p8RMLib[0]==165)||(p8RMLib[0]==166)||(p8RMLib[0]==167))
					{//F165 电能表开关操作次数及时间,F166 电能表参数修改次数及时间,F167 电能表购、用电信息
						return 1;//月冻结用实时数据,总是要实抄
					}
					i=DataComp(i+3,ADDR_TYMDHMS+4,2);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
					if(i!=0)
					{//0=相等
						y|=1;
					}
				}
			}
		}
	}
	if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
	{//补抄上2-3日冻结(日冻结正反有无功,4个象限无功电能示值（总、费率1～M）)
		y=0;
		if((RMCount>0)&&(RMCount<9))
		{
			Fn=160+RMCount;//抄表命令计数1-8对应Fn=161-168
			y|=Class2Data_Fn(ClassNo,Fn);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
		}
		else
		{//抄表命令计数=0或9,上1次日冻结时间YYMMDDhhmm
		#if RMM_CompCongealTime==1//读电能表冻结数据比较冻结时间:0=不比较,1=要比较
			y|=Class2Data_Fn(ClassNo,161);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			y|=Class2Data_Fn(ClassNo,162);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			y|=Class2Data_Fn(ClassNo,163);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			y|=Class2Data_Fn(ClassNo,164);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			y|=Class2Data_Fn(ClassNo,165);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			y|=Class2Data_Fn(ClassNo,166);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			y|=Class2Data_Fn(ClassNo,167);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			y|=Class2Data_Fn(ClassNo,168);//2类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
		#endif
		}
	}
	
	Fn=p8RMLib[0];
	d64a=MRR(ADDR_AFN04F9,8);//事件记录有效标志位 BS64 8
	//ERC8电能表参数变更事件用数据
	if(Fn==0xE1)
	{
		d64b=1;
		d64b<<=(8-1);
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
	//ERC13电表故障信息事件用数据
	if(Fn==0xE2)
	{
		d64b=1;
		d64b<<=(13-1);
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
	//ERC40读电能表恒定磁场干扰总次数
	if(Fn==0xF0)
	{
		d64b=1;
		d64b<<=(40-1);//ERC40磁场异常事件记录
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
#if (USER/100)==2//用户标识:山东版
	//扩展 ERC9电流回路异常事件用数据
	if(Fn==0xF2)
	{
		d64b=1;
		d64b<<=(9-1);//ERC9电流回路异常事件
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
	//扩展 ERC10电压回路异常事件用数据
	if(Fn==0xF3)
	{
		d64b=1;
		d64b<<=(10-1);//ERC10电压回路异常事件
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
	//扩展 ERC29电能表飞走事件,ERC28电能表超差事件用数据
	if(Fn==0xF4)
	{
		d64b=3;
		d64b<<=(28-1);//ERC28,ERC29
		d64b&=d64a;
		if(d64b!=0)
		{
			i=Class1Data_Fn(ClassNo,25);//1类数据Fn配置,入口ClassNo的D7-D4大类号D3-D0小类号,Fn;返回0=不支持,1=支持
			if(i==0)
			{//没抄F25(有功功率)
				y=1;
			}
		}
	}
#endif

#if RMM_Event==1//0=不抄,1=自动抄读事件记录配置的相关数据
	//ERC8电能表参数变更事件
	//ERC11相序异常
	//ERC12电能表时间超差
	//ERC13电表故障信息事件
	//ERC27电能表示度下降
	//ERC33电能表运行状态字变位事件
	//ERC37电能表开表盖事件
	//ERC38电能表开端盖事件
	//ERC40直接读电能表磁场异常事件记录
	//ERC41电能表被非终端对时事件记录
	switch(Fn)
	{
//		case 27://F27 电能表日历时钟、编程次数及其最近一次操作时间
//			goto RMERC1;
		case 28://F28 电表运行状态字及其变位标志
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					i=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					i=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					i=3;//RS485PORT=3;
					break;
				default://RS485-4(载波)
					i=30;//RS485PORT=4
					break;
			}
			i=MRR(ADDR_AFN04F33+((14+(4*24))*i)+1,1);//终端抄表运行参数设置,端口31
			if(i&0x20)
			{//2)	D5置"1"要求终端抄读"电表状态字"，置"0"不要求
				y=1;
			}
			d64b=1;
			d64b<<=(11-1);//ERC11相序异常
			d64b&=d64a;
			if(d64b!=0)
			{
				y=1;
			}
			d64b=1;
			d64b<<=(33-1);//ERC33电能表运行状态字变位事件
			d64b&=d64a;
			if(d64b!=0)
			{
				y=1;
			}
			break;
		case 165://F165 电能表开关操作次数及时间
			d64b=3;
			d64b<<=(37-1);//ERC37电能表开表盖事件,ERC38电能表开端盖事件
			goto RMERC2;
		case 129://F129 当前正向有功电能示值（总、费率1～M）
			d64b=1;
			d64b<<=(27-1);//ERC27电能表示度下降
			goto RMERC2;
		case 162://F162 电能表日历时钟
			d64b=1;
			d64b<<=(12-1);//ERC12电能表时间超差
			goto RMERC2;
		case 166://F166 电能表参数修改次数及时间
			d64b=1;
			d64b<<=(41-1);//ERC41电能表被非终端对时事件记录
			goto RMERC2;
		case 0xe1://扩展1 ERC8电能表参数变更事件用数据
			d64b=1;
			d64b<<=(8-1);//ERC8电能表参数变更事件
			goto RMERC2;
		case 0xe2://扩展2 ERC13电表故障信息事件用数据
			d64b=1;
			d64b<<=(13-1);//ERC13电表故障信息事件
		RMERC2:
			d64b&=d64a;
			if(d64b!=0)
			{
				y=1;
			}
			break;
	}
#endif
	
	return y;
}






#endif
#endif

