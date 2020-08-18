
//实时抄表2类数据
#include "Project.h"
#include "Terminal_ReadMeter_AFN0D_RealTime.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_DL645_1997_Lib.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "Terminal_ReadMeter_DL645_1997.h"
#include "Terminal_ReadMeter_3762_Std.h"
#include "Terminal_AFN0D_CongealData_Fn.h"




void Wait_ReadMeter_AFN0D_RealTime(u32 PORTn,u32 Protocol)//等待1个Fn数据实时抄读完成
{
  UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p32=(u32 *)(ADDR_MS_ENTRY);
	
	switch(PORTn)
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
						Terminal_ReadMeter_DL645_1997_Fn(PORTn,(u8 *)&UARTCtrl->RMLib);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
					#else
						if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
						{
		    			p32=(u32 *)(ADDR_E1_ENTRY);
		    			(*(void(*)())p32[102])(PORTn,(u8 *)&UARTCtrl->RMLib);//102 仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
							p32=(u32 *)(ADDR_I0_ENTRY);
						}
						else
						{
							return;
						}
					#endif
						break;
					case 30://DL/T645-2007(30)
						Terminal_ReadMeter_DL645_2007_Fn(PORTn,(u8 *)&UARTCtrl->RMLib);//仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
						break;
					default:
						return;
				}
				(*(void(*)())p32[115])();//调用I0_Main_Return;63 开环主任务
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
					break;
				}
				if(Terminal_Router->RouterCtrl==0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				{
					RealTimeReadMeter_3762_Fn_Std(PORTn,(u8 *)&UARTCtrl->RMLib);//仅抄1个Fn数据填写到FnDataBuff;RouterRealTimeTask=0=空闲(本Fn抄读完成)
				}
				(*(void(*)())p32[115])();//调用I0_Main_Return;63 开环主任务
			}
	#endif		
			break;
	}
}

u32 ReadMeter_AFN0D_RealTime(u32 Fn,u32 Pn,u64 YMDHM)//实时读取2类数据;返回0=没读,!=0读且已写入FLASH返回是FnDataBuff地址
{
	u32 i;
	u32 di;
	u32 MeterNo;//电能表配置序号
	u32 PORTn;
	u32 Protocol;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
	u32 ReadFlag;
	u8 *p8FnData;
	u32 YMD;
	u32 s;
	
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
			PORTn=RS485_1PORT;
			break;
		case 3://485-2
			PORTn=RS485_2PORT;
			break;
		case 4://485-3
			PORTn=RS485_3PORT;
			break;
		case 31://载波
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			{
				PORTn=RS485_4PORT;
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
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p32=(u32 *)(ADDR_MS_ENTRY);
	while((UARTCtrl->Lock==0x55)||(UARTCtrl->Task!=0))//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		(*(void(*)())p32[115])();//调用I0_Main_Return;63 开环主任务
		if(PORTn==RS485_4PORT)
		{
			if(UARTCtrl->Lock==0x55)
			{
				return 0;//0=没读
			}
		}
	}
	//
	UARTCtrl->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
	UARTCtrl->MeterNoBackup=UARTCtrl->MeterNo;//485抄表电能表/交流采样装置序号(半字对齐)(集中抄表插入抄读实时数据时备份)
	UARTCtrl->MeterNo=MeterNo;
	UARTCtrl->FnCountBackup=UARTCtrl->FnCount;//485抄表Fn计数(集中抄表插入抄读实时数据时备份)
	
	Protocol=MRR(ADDR_AFN04F10+((MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//通信协议类型 BIN 1 
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	ReadFlag=0;//正确读标志
	switch(Protocol)
	{
		case 1://DL/T645-1997(1)
		#if Extern_Terminal_ReadMeter_DL645_1997_Fn==1//外部程序:0=I2,1=E1,2=E2,3=;仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
			if((Comm_Ram->SoftModule&(1<<4))!=0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				break;
			}
		#endif
			break;
		case 30://DL/T645-2007(30)
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		case 31://串行接口连接窄带低压载波(31)
	#endif
		//日冻结
			if((Fn==161)||(Fn==163))
			{//F161日冻结 正向有功电能示值(总,费率1～M);F163日冻结 反向有功电能示值(总,费率1～M)
				//先找相同日冻结时间的上几次冻结(1-n)
				YMDHM>>=16;
				YMD=YMDHM;
				YMD=YMD_Add1D(YMD);//电表冻结的年月日是终端抄读的年月日+1
				MC(0,ADDR_DATABUFF,12);
				MWR(YMD,ADDR_DATABUFF+3,3);
				MR(ADDR_DATABUFF+9,ADDR_TYMDHMS+3,3);
				di=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(di>>31)
				{//负错
					break;
				}
				di/=(24*60*60);//相隔的日数
				if(di>=62)
				{
					break;//电表最多冻结上62次
				}
				//di=理论计算的(上n次)日冻结时间数据标识
				s=2;//0=正,1=负,2=空
				while(1)
				{
					UARTCtrl->RMLib[0]=0;//实时Fn或接收处理类同的Fn
					UARTCtrl->RMLib[1]=1;//通信标识个数
					UARTCtrl->RMLib[2]=0;//数据头,0=终端抄表时间,1=终端抄表时间+费率数
					UARTCtrl->RMLib[3]=0x06;//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
					UARTCtrl->RMLib[4]=1;//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
					MWR(di+0x05060001,(u32)&UARTCtrl->RMLib[10],4);//数据标识
					UARTCtrl->RMLib[14]=5;//返回长度
					Wait_ReadMeter_AFN0D_RealTime(PORTn,Protocol);//等待1个Fn数据实时抄读完成
					i=MRR(((u32)p8FnData)+6+2,3);
					if(i==YMD)
					{
						break;
					}
					if(i==0xeeeeee)
					{//无数据
						if(s==2)
						{//还是第一次
							s=0;
							di=0;
							continue;
						}
						else
						{
							break;
						}
					}
					if(i>YMD)
					{//读到的上次冻结时间>需要冻结时间
						if(s==1)
						{
							break;//原为负向查
						}
						s=0;
						di++;
						if(di>=62)
						{
							break;//电表最多冻结上62次
						}
					}
					else
					{//读到的上次冻结时间<需要冻结时间
						if(s==0)
						{
							break;//原为正向查
						}
						s=1;
						if(di==0)
						{
							break;//电表最多冻结上62次
						}
						di--;
					}
				}
				if(i!=YMD)
				{
					break;//没找到相同冻结时标
				}
				UARTCtrl->RMLib[0]=131;//实时Fn或接收处理类同的Fn;用131不用129
				UARTCtrl->RMLib[1]=1;//通信标识个数
				UARTCtrl->RMLib[2]=1;//数据头,0=终端抄表时间,1=终端抄表时间+费率数
				UARTCtrl->RMLib[3]=0x06;//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
				UARTCtrl->RMLib[4]=1;//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				if(Fn==161)
				{//F161日冻结 正向有功电能示值(总,费率1～M);F163日冻结 反向有功电能示值(总,费率1～M)
					MWR(di+0x05060101,(u32)&UARTCtrl->RMLib[10],4);//数据标识
				}
				if(Fn==163)
				{//F161日冻结 正向有功电能示值(总,费率1～M);F163日冻结 反向有功电能示值(总,费率1～M)
					MWR(di+0x05060201,(u32)&UARTCtrl->RMLib[10],4);//数据标识
				}
				UARTCtrl->RMLib[14]=0;//返回长度
				UARTCtrl->RMLib[15]=20;//当正确返回长度=0时的预计返回字节数
				Wait_ReadMeter_AFN0D_RealTime(PORTn,Protocol);//等待1个Fn数据实时抄读完成
				if(p8FnData[6]!=0xee)
				{//正确读
					ReadFlag=1;//正确读标志
					MR(((u32)p8FnData)+1,ADDR_TYMDHMS+1,5);//抄表时间
					if(Fn==161)
					{//F161日冻结 正向有功电能示值(总,费率1～M);F163日冻结 反向有功电能示值(总,费率1～M)
						AddDateCongeal(YMDHM,161,Pn,((u32)p8FnData)+1,6+(5*(1+MaxTariff)));//补日冻结点数据
					}
					if(Fn==163)
					{//F161日冻结 正向有功电能示值(总,费率1～M);F163日冻结 反向有功电能示值(总,费率1～M)
						AddDateCongeal(YMDHM,163,Pn,((u32)p8FnData)+1,6+(5*(1+MaxTariff)));//补日冻结点数据
					}
				}
				break;
			}
		//月冻结
			if((Fn==177)||(Fn==179))
			{//F161日冻结 正向有功电能示值(总,费率1～M);F163日冻结 反向有功电能示值(总,费率1～M)
				//计算相同月冻结时间的上几次冻结(1-n)
				YMDHM>>=24;//终端抄表的年月
				s=YMDHM;
				i=MRR(ADDR_TYMDHMS+4,2);//当前年月
				for(di=0;di<12;di++)
				{
					if(s==i)
					{
						break;
					}
					i=YM_Sub1M(i);//年月减1月,返回减1后的年月
				}
				if(di>=MaxMonthCongeal)
				{
					break;//电表最多冻结上12次
				}
				UARTCtrl->RMLib[0]=131;//实时Fn或接收处理类同的Fn;用131不用129
				UARTCtrl->RMLib[1]=1;//通信标识个数
				UARTCtrl->RMLib[2]=1;//数据头,0=终端抄表时间,1=终端抄表时间+费率数
				UARTCtrl->RMLib[3]=0x06;//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
				UARTCtrl->RMLib[4]=1;//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				if(Fn==177)
				{//F177月冻结 正向有功电能示值(总,费率1～M);F179月冻结 反向有功电能示值(总,费率1～M)
					MWR(di+0x0001FF00,(u32)&UARTCtrl->RMLib[10],4);//数据标识
				}
				if(Fn==179)
				{//F177月冻结 正向有功电能示值(总,费率1～M);F179月冻结 反向有功电能示值(总,费率1～M)
					MWR(di+0x0002FF00,(u32)&UARTCtrl->RMLib[10],4);//数据标识
				}
				UARTCtrl->RMLib[14]=0;//返回长度
				UARTCtrl->RMLib[15]=20;//当正确返回长度=0时的预计返回字节数
				Wait_ReadMeter_AFN0D_RealTime(PORTn,Protocol);//等待1个Fn数据实时抄读完成
				if(p8FnData[6]!=0xee)
				{//正确读
					ReadFlag=1;//正确读标志
					MR(((u32)p8FnData)+1,ADDR_TYMDHMS+1,5);//抄表时间
					if(Fn==177)
					{//F177月冻结 正向有功电能示值(总,费率1～M);F179月冻结 反向有功电能示值(总,费率1～M)
						AddMonthCongeal(YMDHM,177,Pn,((u32)p8FnData)+1,6+(5*(1+MaxTariff)));//补月冻结点数据
					}
					if(Fn==179)
					{//F177月冻结 正向有功电能示值(总,费率1～M);F179月冻结 反向有功电能示值(总,费率1～M)
						AddMonthCongeal(YMDHM,179,Pn,((u32)p8FnData)+1,6+(5*(1+MaxTariff)));//补月冻结点数据
					}
				}
				break;
			}
		//曲线冻结
			if((Fn==101)||(Fn==103))
			{//F101曲线冻结 正向有功总电能示值;F103曲线冻结 反向有功总电能示值
				UARTCtrl->RMLib[0]=0xe8;//实时Fn或接收处理类同的Fn
				UARTCtrl->RMLib[1]=1;//通信标识个数
				UARTCtrl->RMLib[2]=0;//数据头,0=终端抄表时间,1=终端抄表时间+费率数
				UARTCtrl->RMLib[3]=0x06;//抄表支持:B2=载波抄表,B1=RS485抄表,B0=内部交采抄表
				UARTCtrl->RMLib[4]=0x04;//数据分类:B6=补抄上2-3日冻结,B5=月冻结,B4=抄表日冻结,B3=日冻结,B2=曲线冻结,B1=小时冻结,B0=实时
				MWR(0x06040001,(u32)&UARTCtrl->RMLib[10],4);//数据标识
				UARTCtrl->RMLib[14]=0;//返回长度
				UARTCtrl->RMLib[15]=32;//当正确返回长度=0时的预计返回字节数
				MWR(YMDHM,(u32)&UARTCtrl->RMCommandYMDHM[0],5);//曲线的时标年月日时分
				Wait_ReadMeter_AFN0D_RealTime(PORTn,Protocol);//等待1个Fn数据实时抄读完成
				if(p8FnData[6]!=0xee)
				{//正确读
					ReadFlag=1;//正确读标志
					SaveClass2Data(Pn,(u32)p8FnData,(u8*)&UARTCtrl->RMLib);//存贮类2测量点抄表数据
				}
				break;
			}
			break;
	}

	UARTCtrl->MeterNo=UARTCtrl->MeterNoBackup;//485抄表电能表/交流采样装置序号(半字对齐)(集中抄表插入抄读实时数据时备份)
	UARTCtrl->FnCount=UARTCtrl->FnCountBackup;//485抄表Fn计数(集中抄表插入抄读实时数据时备份)
	UARTCtrl->RMCount=0;//485抄表命令计数
	UARTCtrl->ReRMCount=0;//485抄表命令重发计数
	UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
	UARTCtrl->Task=0;
	
	i=Get_ADDR_UARTnFnDataBuff(PORTn)+1;
	MR(i,ADDR_TYMDHMS+1,5);//填入抄表时间
	if(ReadFlag==0)//正确读标志
	{
		return 0;
	}
	return i;//!=0读
}



