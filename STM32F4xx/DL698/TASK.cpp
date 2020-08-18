

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"
#include "../DL698/SearchMeter_Record.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"
#include "../Terminal698/Terminal_PowerDown.h"
#include "../DL698/RM_3762_RouterActive.h"

#include "../DL698/DL698_JSON.h"


//Comm_Ram->DL698YMDHMS[10];//年(先高字节后低字节),月,日,时,分,秒,百分秒,周
//#define ADDR_6012_2     ADDR_6002_127+LENmax_6002_127//任务配置表(初始化值0x00)
//0 属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
// {
//2 	任务ID    unsigned，
//4 	执行频率   TI，
//8 	方案类型   enum
//  	{
//  		1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//		}，
//10	方案编号    unsigned，
//12	开始时间    date_time_s，
//20	结束时间    date_time_s，
//28	延时        TI，
//32	执行优先级  enum{自扩展最高(0),首要（1），必要（2），需要（3），可能（4）}，
//34	状态        enum{正常（1），停用（2）}，
//36	任务开始前脚本id   long-unsigned，
//39	任务完成后脚本id   long-unsigned，
//42	任务运行时段       structure，
//		{
//44  	类型  enum
//			{
//       前闭后开    （0），
//       前开后闭    （1），
//       前闭后闭    （2），
//       前开后开    （3）
//			}，
//46		时段表  array 时段
//48		时段∷=structure
//			{
//50			起始小时  unsigned，
//				起始分钟  unsigned，
//				结束小时  unsigned，
//				结束分钟  unsigned
//			}
//		}
//}
//当方案类型为脚本时，方案编号为脚本id

//#define LENper_6012_2     128
//#define NUMmax_6012_2     16
//#define LENmax_6012_2     (LENper_6012_2*NUMmax_6012_2)




//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//b31-b28优先级:0=自扩展最高,1=首要,2=必要,3=需要,4=可能
//b27-b24方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//b23-b20普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集,4=补抄
//b19-b16普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标,7=相对上月月末23点59分
//b15-b8任务号
//b7-b0方案编号


void CLR_RMCount(u32 PORTn)//清0抄表发送帧各计数值
{
	UARTCtrl_TypeDef *UARTCtrl;
	u32 i;
	u16* p16timer;
	
	i=i;
	p16timer=p16timer;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中
	UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
	UARTCtrl->RMCSDCount=0;//抄表CSD计数
	UARTCtrl->RMCSDSubCount=0;//抄表CSD子计数
	UARTCtrl->RMROADCount=0;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
	UARTCtrl->RMROADCountOK=0;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
	UARTCtrl->ReRMCount=0;//抄表重发计数
	UARTCtrl->RM_OKFAIL=0;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
	UARTCtrl->RM_DI_TI_Count=0;//按时标间隔采集时的间隔计数
	UARTCtrl->RMOADROAD=0;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND
	
	UARTCtrl->RMAddr[0]=0;
	UARTCtrl->RMAddr[1]=0;
	UARTCtrl->RMAddr[2]=0;
#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	if(PORTn==RS485_4PORT)
	{
		ClrALL_RMListRMCount();//清全部抄表计数相关数据(载波口);入口:SetNo=档案号从0开始
	#if (USER/100)==0//电科院测试
		i=UARTCtrl->TaskID;
		i>>=8;
		i&=0xff;
		UARTCtrl->MeterCount[i]=0;//变更电能表计数
		Terminal_Router->NUM_Enable_Concurrency=5;//宽带载波当前还允许并发帧数
		p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
		p16timer[0]=5000/10;//下帧启动发送延时,5秒后
	#else
		Terminal_Router->NUM_Enable_Concurrency=5;//宽带载波当前还允许并发帧数
	#endif
	}
#endif
}

u32 Check_OtherTask(u32 PORTn)//检查是否有其他任务；返回:0=没,!=0有
{
	u32 i;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//其他任务
	i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
	i|=UARTCtrl->BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
	i|=UARTCtrl->AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
	i|=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
	return i;
}

void TASK(u32 PORTn,u32 RM)//任务;入口:RM=0上报任务,RM=1采集任务
{
	u32 i;
	u32 x;
	u32 s;
	u32 id;
	u32 TaskID;
	u32 SourTaskID;
	u32 FileID;
	u32 ti;
	u32 HM1;
	u32 HM2;
	u32 NUM_TaskID;//当前任务数
	u8 *p8s;
	u8 *p8d;
	u16 *p16;
	u32* p32;
	UARTCtrl_TypeDef *UARTCtrl;
	u64 d64;
	u32 n;
	RMokfailLIST_TypeDef *rmOKFAILlist;
#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
	u32 NUM_SomeTaskID;
	u32 TaskChange;
	u32 TH1TaskID;//同类中的第一个任务
#endif
		
	
#if (8+(NUMmax_6012_2*4))>LEN_DATABUFF
	#error
#endif
//ADDR_DATABUFF缓冲
//7byte YYMDHMS 下频率标准开始时间点
//1byte
//7byte YYMDHMS 下频率标准开始时间点+延时
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(RM)
	{//抄表任务
		if(PORTn!=ACSAMPLEPORT)
		{//不是交采
			//停电事件任务
			//if(PowerEvent->task<=10)//10=抄电能表停电记录
			if(PowerEvent->task==10)//10=抄电能表停电记录
			{
				if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run!=2)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
				{//停电事件任务没完成
					TaskID=(RMTASKnoPOWER<<8)+1;
					UARTCtrl->TaskID=TaskID;
					UARTCtrl->NUM_SomeTaskID=1;//同类采集任务的id号的列表个数
					UARTCtrl->SomeTaskid[0]=TaskID;//RMTASKmax byte 同类采集任务的b15-b8任务号,b7-b0方案编号
					if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==0)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
					{
//						if(PowerEvent->task<10)//10=抄电能表记录
//						{
//							UARTCtrl->TaskID=0;//上电,等待电源达到恢复电压值
//							return;
//						}
						PowerEvent->TimeErr=0;//有抄读的电能表停电事件停电或上电时间不完整
						UARTCtrl->TaskTime[RMTASKnoPOWER].Run=1;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
						UARTCtrl->NUMrmFAIL[RMTASKnoPOWER]=0;//清0抄表失败数
						CLR_RMCount(PORTn);//清0抄表发送帧各计数值
						UARTCtrl->MeterCount[RMTASKnoPOWER]=0;//各任务已抄电能表计数
						UARTCtrl->RMLOOP[RMTASKnoPOWER]=0;//RMTASKmax byte 全部表抄读一遍的遍数计数
						RM_Meter_Event3106();//停/上电事件需抄读电表的配置序号(从0开始)
						NUMrm(PORTn);//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
					}
					else
					{
						if(UARTCtrl->RMLOOP[RMTASKnoPOWER]>=1)//RMTASKmax byte 全部表抄读一遍的遍数计数
						{//抄完
							if(PowerEvent->TimeErr)//有抄读的电能表停上电事件未完整产生
							{//有
								UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
								return;
							}
							UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
							UARTCtrl->TaskID=0;
							return;
						}
					}
					return;
				}
			}
			//其他任务
			i=Check_OtherTask(PORTn);//检查是否有其他任务；返回:0=没,!=0有
			if(i)
			{//有其他任务
				return;
			}
			//广播校时任务开始时间
			if(UARTCtrl->LastBroadcastRTCD2!=Comm_Ram->DL698YMDHMS[3])
			{//日变化
				MR(ADDR_DATABUFF,ADDR_4204_2,LENmax_4204_2);
				p8s=(u8*)ADDR_DATABUFF;
				if(p8s[7]==1)
				{//启用
					i=(Comm_Ram->DL698YMDHMS[4]<<16)|(Comm_Ram->DL698YMDHMS[5]<<8)|Comm_Ram->DL698YMDHMS[6];
					x=(p8s[3]<<16)|(p8s[4]<<8)|p8s[5];
					if((x&0xff)==0xff)
					{
						i&=0xffff00;
						x&=0xffff00;
					}
					if((x&0xff00)==0xff00)
					{
						i&=0xff00ff;
						x&=0xff00ff;
					}
					if((x&0xff0000)==0xff0000)
					{
						i&=0x00ffff;
						x&=0x00ffff;
					}
					if(i>=x)
					{//到启动时间
						UARTCtrl->LastBroadcastRTCD2=Comm_Ram->DL698YMDHMS[3];//上次执行广播校时-2的实时钟日
						UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+2;
						UARTCtrl->BroadcastTimeTask=1;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
						return;
					}
				}
			}
			//每天周期搜表任务开始时间
			//#define ADDR_6002_8     ADDR_6000+LENmax_6000//搜表参数
			//属性8∷=structure
			//{
			//	是否启用每天周期搜表   bool，
			//	自动更新采集档案       bool，
			//	是否产生搜表相关事件   bool，
			//	清空搜表结果选项       enum
			//	{
			//		不清空             （0），
			//		每天周期搜表前清空 （1），
			//		每次搜表前清空     （2）
			//	}
			//}
			//#define LENmax_6002_8     10
			//#define ADDR_6002_9     ADDR_6002_8+LENmax_6002_8//每天周期搜表参数配置
			//属性9（每天周期搜表参数配置）∷=array 定时搜表参数
			//定时搜表参数∷=structure
			//{
			//   开始时间         time，
			//   搜表时长（min）  long-unsigned
			//}
			//#define LENmax_6002_9     (4*16)
			if(UARTCtrl->LastAutoSearchRTCD!=Comm_Ram->DL698YMDHMS[3])//上次自动搜表实时钟日
			{//日变化
			#if (LENmax_6002_8+LENmax_6002_9)>LEN_DATABUFF
				#error
			#endif
				MR(ADDR_DATABUFF,ADDR_6002_8,LENmax_6002_8+LENmax_6002_9);
				p8s=(u8*)ADDR_DATABUFF;
				i=p8s[3];
			#if EnRS485AutoSearchMeter==0//0=否,1=是;允许RS485口自动搜表
				if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
				{
					i=0;
				}
			#endif
				if(i==1)
				{//启用每天周期搜表
					p8s=(u8*)ADDR_DATABUFF+LENmax_6002_8;
					s=p8s[1];//数组数
					if(s>1)
					{
						s=1;//暂只支持一个时段
					}
					p8s+=2;
					while(s--)
					{
						i=(Comm_Ram->DL698YMDHMS[4]<<16)|(Comm_Ram->DL698YMDHMS[5]<<8)|Comm_Ram->DL698YMDHMS[6];
						x=(p8s[3]<<16)|(p8s[4]<<8)|p8s[5];
						if((x&0xff)==0xff)
						{
							i&=0xffff00;
							x&=0xffff00;
						}
						if((x&0xff00)==0xff00)
						{
							i&=0xff00ff;
							x&=0xff00ff;
						}
						if((x&0xff0000)==0xff0000)
						{
							i&=0x00ffff;
							x&=0x00ffff;
						}
						if(i>=x)
						{//到启动时间
							UARTCtrl->LastAutoSearchRTCD=Comm_Ram->DL698YMDHMS[3];//上次自动搜表实时钟日
							i=(p8s[7]<<8)|p8s[8];
							UARTCtrl->AutoSearchTime=i;//自动搜表持续时间分，0表示不限时间直至搜表结束
							if(i==0)
							{
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										i=0;
										break;
									case RS485_2PORT://RS485-2
										i=1;
										break;
									default:
										i=255;
										break;
								}
								if(i<=2)
								{
									MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*i),7);
								}
								i=65535;
							}
							Terminal_Ram->AutoSearch_M_Timer=i;//2 自动搜表允许时间分定时器
							UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+4;//方案号4=每天周期搜表
							UARTCtrl->AutoSearchTask=1;//自动搜表任务:0=空,1=等待,2...过程
							p8s=(u8*)ADDR_DATABUFF;
							//if(p8s[9]==2)
							if(p8s[9])
							{//每天周期搜表前清空或每次搜表前清空
								DEL_SearchMeter_Record(3);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
								DEL_SearchMeter_Record(4);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
							}
							return;
						}
						p8s+=9;
					}
				}
			}
			//
			
		}
		p32=(u32*)(ADDR_RMTaskID+4);//当前待执行的抄表任务列表
	}
	else
	{
		p32=(u32*)(ADDR_REPORTTaskID+4);//当前待执行的上报任务列表
	}
	
	NUM_TaskID=0;//当前任务数
	for(id=0;id<NUMmax_6012_2;id++)
	{
		switch(id)
		{
			case RMTASKnoPOWER://停上电事件固定任务号

				continue;
		}
		p8s=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
		if(p8s[0]!=DataType_structure)
		{//删除
			p16=(u16*)&UARTCtrl->TaskTime[id];
			for(i=0;i<(LEN_UARTnTask/2);i++)
			{
				p16[0]=0;
			}
			continue;
		}
		if(p8s[35]!=1)
		{//状态        enum{正常（1），停用（2）}
			p16=(u16*)&UARTCtrl->TaskTime[id];
			for(i=0;i<(LEN_UARTnTask/2);i++)
			{
				p16[0]=0;
			}
			continue;
		}
		d64=YYMDHMS_Sub_hex64(p8s+13,(u8*)&Comm_Ram->DL698YMDHMS);//hex64位时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
		if(d64>>32)
		{//时钟未到设置的开始时间
			continue;
		}
		s=d64;
		MR(ADDR_DATABUFF,(u32)p8s+13,7);//取设置的开始时间
		ti=(p8s[6]<<8)+p8s[7];//取频率值
		if(ti==0)
		{
			ti=1;//最小值=1
		}
		switch(p8s[5])
		{
			case 0://秒      （0），
				break;
			case 1://分      （1），
				ti*=60;
				break;
			case 2://时      （2），
				ti*=60*60;
				break;
			case 3://日      （3），
				ti*=60*60*24;
				break;
			case 4://月      （4），
				x=(p8s[13]<<8)+p8s[14];//开始时间的年
				i=(Comm_Ram->DL698YMDHMS[0]<<8)+Comm_Ram->DL698YMDHMS[1];//当前时间的年
				i-=x;
				i*=12;
				i+=Comm_Ram->DL698YMDHMS[2];//当前时间的月
				i-=p8s[15];//开始时间的月
				i/=ti;//月化整频率值
				i*=ti;
				pYYM_AddM_hex((u8*)ADDR_DATABUFF,i);//年年月加m月,返回加m月后的年年月
				break;
			case 5://年      （5）
				x=(p8s[13]<<8)+p8s[14];//开始时间的年
				i=(Comm_Ram->DL698YMDHMS[0]<<8)+Comm_Ram->DL698YMDHMS[1];//当前时间的年
				i-=x;
				i/=ti;//年化整频率值
				i*=ti;
				i+=x;
				p8d=(u8*)ADDR_DATABUFF;
				p8d[0]=i>>8;
				p8d[1]=i;
			default:
				continue;
		}
		if(p8s[5]<=3)
		{
			if(PORTn==RS485_4PORT)
			{//是载波口
			#if (CurveTaskTImin)
				if(ti<CurveTaskTImin)//(60*60)//曲线抄表任务执行频率最小值(秒)
				{
					ti=CurveTaskTImin;//曲线抄表任务执行频率最小值(秒)
				}
			#endif
			}
			s/=ti;//秒化整频率值
			s*=ti;
			pYYMDHMS_AddS_hex((u8*)ADDR_DATABUFF,s);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
		}
		//当前时钟 比较 下频率标准开始时间点
		i=Compare_DL698DataString((u8*)&Comm_Ram->DL698YMDHMS,(u8*)ADDR_DATABUFF,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
		if(i<=1)
		{//当前时钟 >= 下频率标准开始时间点
			//日时段表
			s=(Comm_Ram->DL698YMDHMS[4]<<8)+Comm_Ram->DL698YMDHMS[5];//当前时钟时分
//			#if (USER%100)!=0//不是台体测试
//				//当前时钟时分>=23点50分或<0点10分，不抄表
//				if((s>=((23<<8)+50))||(s<10))
//				{
//					continue;
//				}
//			#endif
			x=p8s[47];//时段表数
			if(x)
			{//时段表数！=0，无时段表作为有效
				p8d=p8s+50;
				for(i=0;i<x;i++)
				{
					HM1=(p8d[1]<<8)+p8d[3];//时段开始时分
					HM2=(p8d[5]<<8)+p8d[7];//时段结束时分
					if(p8s[45]==0)
					{//前闭后开    （0）
						if((s>=HM1)&&(s<HM2))
						{
							break;
						}
					}
					if(p8s[45]==1)
					{//前开后闭    （1）
						if((s>HM1)&&(s<=HM2))
						{
							break;
						}
					}
					if(p8s[45]==2)
					{//前闭后闭    （2）
						if((s>=HM1)&&(s<=HM2))
						{
							break;
						}
					}
					if(p8s[45]==3)
					{//前开后开    （3）
						if((s>HM1)&&(s<HM2))
						{
							break;
						}
					}	
					p8d+=10;
				}
				if(i>=x)
				{//全部时段无效
					continue;
				}
			}
	//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	//优先级:0=自扩展最高,1=首要,2=必要,3=需要,4=可能
	//方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
	//普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集,4=RetryMetering 补抄
	//普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标,7=相对上月月末23点59分
			TaskID=p8s[33];//优先级
			if(TaskID==0)
			{//电科院互换性测试时优先级设为0
				TaskID=1;//优先级,0=固定用于抄掉上电事件
			}
			if(TaskID>15)
			{
				TaskID=15;
			}
			TaskID<<=4;
			x=p8s[9];//方案类型(1-5)
			if(RM)
			{//抄表任务
				if(x>=4)
				{
					continue;
				}
			}
			else
			{//上报任务
				if(x!=4)
				{
					continue;
				}
			}
			x&=0xf;
			TaskID|=x;
			switch(x)//方案类型
			{
				case 1://普通采集方案 （1）
					//普通方案时采集类型
					TaskID<<=4;
					x=p8s[11];//方案编号0-127
					p8d=(u8*)ADDR_6014_SDRAM+(x*LENper_6014);
				#if (USER/100)==17//江苏系
					if((p8d[10]&0xff)==0xf0)//按采集器方式点名方式采集
					{
						TaskID|=0x05;
						if(PORTn==ACSAMPLEPORT)
							continue;
					}
					else
						TaskID|=p8d[10]&0xf;
				#else
					TaskID|=p8d[10]&0xf;
				#endif
					//TaskID|=p8d[10]&0xf;
					//普通方案时存储时标
					TaskID<<=4;
					p8d=Get_Element(p8d,6,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8d)
					{
						TaskID|=p8d[1]&0xf;
					}
					break;
				case 2://事件采集方案 （2）
					if(PORTn==ACSAMPLEPORT)
					{//交采口不抄事件
						continue;
					}
					//事件采集方案时采集类型
					TaskID<<=4;
					x=p8s[11];//方案编号0-127
					p8d=(u8*)ADDR_6016_SDRAM+(x*LENper_6016);
					x=p8d[7]&0xf;
					switch(x)
					{
						case 0://周期采集事件数据
							break;
						case 1://根据通知采集所有事件数据
							continue;
						case 2://根据通知采集指定事件数据
						#if (USER%100)==0//电科院测试
							continue;
						#else
							break;
						#endif
						case 3://直接存储通知事件数据
						default:
							continue;
					}
					TaskID|=x;
					//事件采集方案时存储时标=0
					TaskID<<=4;
					break;
				//csae 3://透明方案     （3）
				//case 4://上报方案     （4）
				//case 5://脚本方案     （5）
				default:
					//采集类型=0
					//存储时标=0
					TaskID<<=8;
					break;
			}
			TaskID<<=8;
			TaskID|=id;//任务号
			TaskID<<=8;
			x=p8s[11];//方案编号0-127
			TaskID|=x;//方案编号
		#if CurveTaskTImin!=0//曲线抄表任务执行频率最小值(秒)
			if(PORTn==RS485_4PORT)
			{//是载波口
//				if(((TaskID>>16)&0xfff)==0x136)
//				{//曲线抄读任务
//					TaskID&=0x0fffffff;//优先级:0=自扩展最高
//				}
				p8d=(u8*)(u8*)ADDR_6014_SDRAM+(x*LENper_6014);
				p8d=Get_Element(p8d,4,1,0);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8d)
				{
					i=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
					if(i==0x5B015002)//分钟冻结(曲线)
					{
						TaskID&=0x0fffffff;//优先级:0=自扩展最高
					}
				}
			}
		#endif
			
			i=Compare_DL698DataString((u8*)&UARTCtrl->TaskTime[id].StdTime,(u8*)ADDR_DATABUFF,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i==0)
			{//标准时间点相同
				if(UARTCtrl->TaskTime[id].Run==2)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
				{
					continue;
				}
			}
			else
			{//不相同
				//执行状态置为0,使相关计数清0及路由重启,不管原状态情况
				UARTCtrl->TaskTime[id].Run=0;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
				MW(ADDR_DATABUFF,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
				MW(ADDR_DATABUFF,(u32)&UARTCtrl->TaskTime[id].StartTime,7);
				for(i=0;i<7;i++)
				{
					UARTCtrl->TaskTime[id].EndTime[i]=0xff;//任务执行结束时间YYMDHMS
				}
				if(RM)
				{
					UARTCtrl->RMLOOP[id]=0;//全部表抄读一遍的遍数计数,任务ID号数组
					UARTCtrl->NUMrmFAIL[id]=0;//清0抄表失败数
					SourTaskID=UARTCtrl->TaskID;
					UARTCtrl->TaskID=TaskID;
					NUMrm(PORTn);//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
					UARTCtrl->TaskID=SourTaskID;
				}
			}
			//当前时钟 比较 下频率标准开始时间点+延时
			ti=(p8s[30]<<8)+p8s[31];//取延时
			switch(p8s[29])
			{
				case 0://秒      （0），
					break;
				case 1://分      （1），
					ti*=60;
					break;
				case 2://时      （2），
					ti*=60*60;
					break;
				default:
					ti=0;
					break;
			}
			MW((u32)&UARTCtrl->TaskTime[id].StdTime,ADDR_DATABUFF+8,7);
			pYYMDHMS_AddS_hex((u8*)ADDR_DATABUFF+8,ti);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
			i=Compare_DL698DataString((u8*)&Comm_Ram->DL698YMDHMS,(u8*)ADDR_DATABUFF+8,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i<=1)
			{//当前时钟 >= 下频率标准开始时间点+延时
				p32[NUM_TaskID]=TaskID;
				NUM_TaskID++;//有任务id数
			}
		}
	}
	
	p32--;
	p32[0]=NUM_TaskID;//有效Task个数
	if(NUM_TaskID==0)
	{//无任务
		if(RM)
		{//抄表
			UARTCtrl->NUM_SomeTaskID=0;//同类采集任务的id号的列表个数
			UARTCtrl->SomeTaskid[0]=0;
		}
		UARTCtrl->TaskID=0;//值0表示无任务
		return;
	}
	p32++;
	
//查任务完成
	if(RM)
	{
		for(n=0;n<NUM_TaskID;n++)
		{
			TaskID=p32[n];
			id=TaskID>>8;
			id&=0xff;
			if(id<NUMmax_6012_2)
			{
				if(UARTCtrl->TaskTime[id].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
				{
					i=UARTCtrl->NUMrmOK[id];//已采集完成电能表总数
					x=UARTCtrl->NUMrmBAD[id];//已采集完成的电能表数中因配置变更等原因不再需抄读的数
					s=UARTCtrl->NUMrmTOTAL[id];//任务需采集电能表总数
					SourTaskID=UARTCtrl->TaskID;
					UARTCtrl->TaskID=TaskID;
					FileID=Get_FileID(PORTn);
					UARTCtrl->TaskID=SourTaskID;
					if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
					{//原已采集完成数=0;抄曲线和补抄(日冻结和曲线)需抄多个点,在发送管理时识别各点是否已抄
						//UARTCtrl->NUMrmOK[id]=0;//原已采集完成数
					}
					else
					{
						if(i>=x)
						{
							i-=x;
						}
					}
					if(i>=s)
					{//OK数>=需抄数
						UARTCtrl->TaskTime[id].Run=2;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
						for(i=0;i<7;i++)
						{
							UARTCtrl->TaskTime[id].EndTime[i]=Comm_Ram->DL698YMDHMS[i];//任务执行结束时间YYMDHMS
						}
						for(i=n;i<(NUM_TaskID-1);i++)
						{
							HM2=p32[i+1];
							p32[i]=HM2;
						}
						NUM_TaskID--;//任务id数
					}
					
				}
			}
		}
	}
	
#if (USER%100)!=0//不是电科院测试
//时更新,查有>=12时至少已抄过1遍后置为4=抄日冻结执行未完成的任务,在下列时间点补抄
	if(RM)
	{
		i=Comm_Ram->DL698YMDHMS[4];
		if(UARTCtrl->RTCh!=i)//实时钟更新标志原始时
		{
			UARTCtrl->RTCh=i;//实时钟更新标志原始时
			switch(i)
			{
				case 15:
				case 18:
				case 21:
					for(i=0;i<NUM_TaskID;i++)
					{
						HM1=p32[i];
						id=HM1>>8;
						id&=0xff;
						if(id<NUMmax_6012_2)
						{
							if(UARTCtrl->TaskTime[id].Run==4)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
							{
								UARTCtrl->TaskTime[id].Run=3;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
							}
						}
					}
					break;
			}
		}
	}
#endif
	
//排序,将优先高的排最前
	while(NUM_TaskID>=2)
	{//有任务id数>=2
		//按优先级排序
		while(1)
		{
			x=0;//排序没变化标志
			for(i=0;i<(NUM_TaskID-1);i++)
			{
				HM1=p32[i];
				HM2=p32[i+1];
				s=HM1;
				ti=HM2;
				if(RM)
				{
					id=HM1>>8;
					id&=0xff;
					if(id<NUMmax_6012_2)
					{
						if(UARTCtrl->TaskTime[id].Run>=4)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
						{
							s|=0xf0000000;//4=抄日冻结执行未完成,5=执行未完成,置优先级为最低
						}
					}
					id=HM2>>8;
					id&=0xff;
					if(id<NUMmax_6012_2)
					{
						if(UARTCtrl->TaskTime[id].Run>=4)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
						{
							ti|=0xf0000000;//4=抄日冻结执行未完成,5=执行未完成,置优先级为最低
						}
					}
				}
				if((ti&0xfff00000)<(s&0xfff00000))
				{
					p32[i]=HM2;
					p32[i+1]=HM1;
					x=1;//排序变化标志
				}
			}
			if(x==0)
			{
				break;
			}
		}
		//相同优先级抄读遍数小的优先
		if(RM)
		{
			while(1)
			{
				x=0;//排序没变化标志
				for(i=0;i<(NUM_TaskID-1);i++)
				{
					HM1=p32[i];
					s=HM1>>28;
					HM2=p32[i+1];
					ti=HM2>>28;
					id=HM1>>8;
					id&=0xff;
					if(id<NUMmax_6012_2)
					{
						if(UARTCtrl->TaskTime[id].Run>=4)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
						{
							s|=0xf;//4=抄日冻结执行未完成,5=执行未完成,置优先级为最低
						}
					}
					id=HM2>>8;
					id&=0xff;
					if(id<NUMmax_6012_2)
					{
						if(UARTCtrl->TaskTime[id].Run>=4)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
						{
							ti|=0xf;//4=抄日冻结执行未完成,5=执行未完成,置优先级为最低
						}
					}
					if(s==ti)
					{//相同优先级
						id=HM1>>8;
						id&=0xff;
						s=UARTCtrl->RMLOOP[id];
						id=HM2>>8;
						id&=0xff;
						ti=UARTCtrl->RMLOOP[id];
						if(ti<s)
						{
							p32[i]=HM2;
							p32[i+1]=HM1;
							x=1;//排序变化标志
						}
					}
				}
				if(x==0)
				{
					break;
				}
			}
		}
		//任务完成与暂停
		if(RM)
		{
			TaskID=p32[0];
			id=TaskID>>8;
			id&=0xff;
			if(id<NUMmax_6012_2)
			{
				if(UARTCtrl->TaskTime[id].Run==1)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
				{
					i=UARTCtrl->NUMrmOK[id];//已采集完成电能表总数
					x=UARTCtrl->NUMrmBAD[id];//已采集完成的电能表数中因配置变更等原因不再需抄读的数
					s=UARTCtrl->NUMrmTOTAL[id];//任务需采集电能表总数
					n=UARTCtrl->NUMrmFAIL[id];//已采集失败电能表总数

					SourTaskID=UARTCtrl->TaskID;
					UARTCtrl->TaskID=TaskID;
					FileID=Get_FileID(PORTn);
					UARTCtrl->TaskID=SourTaskID;
					if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
					{//原已采集完成数=0;抄曲线和补抄(日冻结和曲线)需抄多个点,在发送管理时识别各点是否已抄
						//UARTCtrl->NUMrmOK[id]=0;//原已采集完成数
					}
					else
					{
						if(i>=x)
						{
							i-=x;
						}
					}
					HM1=0;//抄表正常但数据为空计为失败数(台体测试采集监控计为抄表失败)
					rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
					rmOKFAILlist--;
					for(HM2=0;HM2<n;HM2++)
					{
						if(rmOKFAILlist->SpaceData==2)
						{//0=失败,1=,2=抄表正常但数据为空标志失败(台体测试采集监控计为抄表失败)
							HM1++;
						}
						rmOKFAILlist--;
					}
					if((i+HM1)>=s)
					{//(OK数+正常抄表但数据为空计为失败数)>=需抄数
						UARTCtrl->TaskTime[id].Run=2;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
						for(i=0;i<7;i++)
						{
							UARTCtrl->TaskTime[id].EndTime[i]=Comm_Ram->DL698YMDHMS[i];//任务执行结束时间YYMDHMS
						}
						for(i=0;i<(NUM_TaskID-1);i++)
						{
							HM2=p32[i+1];
							p32[i]=HM2;
						}
						NUM_TaskID--;
						continue;
					}
//			#if (USER%100)!=0//不是电科院
					i+=n;
					if(i>=s)
					{//(失败+OK数)>=需抄数;已抄一遍
						if(PORTn==RS485_4PORT)
						{//载波口；485口不检查重发次数因正常返回但无数据时台体测试采集监控不能算抄表成功计数
							//检查重发次数>=3
							HM1=UARTCtrl->NUMrmFAIL[id];//2*RMTASKmax byte各任务已采集失败电能表总数
							if(HM1>NUM_RMmax)
							{
								HM1=0;
							}
							rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
							rmOKFAILlist--;
							for(i=0;i<HM1;i++)
							{
								if(rmOKFAILlist->ReRMCount<2)//发送次数<2
								//if(rmOKFAILlist->ReRMCount<5)//发送次数<5
								{//发送次数<2
									break;
								}
								rmOKFAILlist--;
							}
							if(i<HM1)
							{//有重发次数<3,没完成抄一遍
								break;
							}
						}
						//
						UARTCtrl->NUMrmFAIL[id]=0;//2*RMTASKmax byte各任务已采集失败电能表总数
						i=UARTCtrl->RMLOOP[id];
						if(i!=0xff)
						{//RMTASKmax byte 全部表抄读一遍的遍数计数
							i++;
							UARTCtrl->RMLOOP[id]=i;
						}
						if(i==0xff)
						{//相同优先级,已抄遍数均为0xFF时,已抄遍数清0
							HM1=p32[0];
							HM1>>=28;
							for(i=1;i<NUM_TaskID;i++)
							{
								HM2=p32[i];
								if(HM1==(HM2>>28))
								{
									id=HM2>>8;
									id&=0xff;
									if(id<NUMmax_6012_2)
									{
										if(UARTCtrl->RMLOOP[id]!=0xff)
										{
											break;
										}
									}
								}
							}
							if(i>=NUM_TaskID)
							{//均为0xff
								for(i=0;i<NUM_TaskID;i++)
								{
									HM2=p32[i];
									if(HM1==(HM2>>28))
									{
										id=HM2>>8;
										id&=0xff;
										if(id<NUMmax_6012_2)
										{
											UARTCtrl->RMLOOP[id]=0;
										}
									}
								}
							}
						}
						HM1=p32[0];
						id=HM1>>8;
						id&=0xff;
						if(id<NUMmax_6012_2)
						{
							if(PORTn==RS485_4PORT)
							{//载波口
							#if RMovertimeACKfail==1//载波路由主动请求抄表大于连续次失败时:0=继续发抄表帧,1=返回抄表失败(跳表)
								Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
							#endif
								HM1>>=28;//执行优先级  enum{自扩展最高（0）、首要（1），必要（2），需要（3），可能（4）}
							#if (USER/100)==9//河南
								#define priority
								if(HM1==1)//首要(日冻结)
							#endif
								
							#if (USER/100)==15//安徽
								#define priority
								if(HM1==2)//(日冻结)
							#endif
								
							#ifndef priority
								if(HM1==1)//首要(日冻结)
							#endif
								{
								#if (USER%100)!=0//不是电科院测试
									i=Comm_Ram->DL698YMDHMS[4];//时
									if(i>=14)
								#endif
									{//时>=14点
										UARTCtrl->TaskTime[id].Run=4;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
										for(i=0;i<(NUM_TaskID-1);i++)
										{
											HM2=p32[i+1];
											p32[i]=HM2;
										}
										NUM_TaskID--;//任务id数
										continue;
									}
								}
								else
								{//非首要任务(日冻结)
									UARTCtrl->TaskTime[id].Run=5;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
									for(i=0;i<(NUM_TaskID-1);i++)
									{
										HM2=p32[i+1];
										p32[i]=HM2;
									}
									NUM_TaskID--;//任务id数
									continue;
								}
							}
							else
							{//485口
								UARTCtrl->TaskTime[id].Run=5;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
								for(i=0;i<(NUM_TaskID-1);i++)
								{
									HM2=p32[i+1];
									p32[i]=HM2;
								}
								NUM_TaskID--;//任务id数
								continue;
							}
						}
						break;
					}
					else
					{//没完成抄一遍

					}
//			#endif
				}
			}
		}
		break;
	}

	if(NUM_TaskID==0)
	{
		p32[0]=0;
	}
	
//采集上第N次,采集时标间隔
	id=p32[0];
	if(RM)
	{//抄表任务
		i=id;
		i>>=24;
		i&=0xf;
		if(i==1)
		{//普通采集方案
			UARTCtrl->RM_DI_N=1;//采集上第N次
			UARTCtrl->RM_DI_TI_Count=0;//按时标间隔采集时的间隔计数
			i=id&0xff;
			p8d=(u8*)ADDR_6014_SDRAM+(i*LENper_6014)+12;
			i=(id>>20)&0xf;
			switch(i)//采集类型
			{
				default:
				case 0://0=采集当前数据
					break;
				case 1://1=采集上第N次
					
					UARTCtrl->RM_DI_N=p8d[0];//采集上第N次
					break;
				case 2://2=按冻结时标采集
					break;
				case 3://3=按时标间隔采集
					UARTCtrl->RM_DI_TI[0]=p8d[0];//按时标间隔采集
					UARTCtrl->RM_DI_TI[1]=p8d[1];//按时标间隔采集
					UARTCtrl->RM_DI_TI[2]=p8d[2];//按时标间隔采集
					break;
				case 4://4=RetryMetering 补抄
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2				 NULL	     按冻结时标采集
//3	       TI	       按时标间隔采集
//4        RetryMetering 补抄
//RetryMetering::=structure
//{
//	数据时标间隔 TI
//	补抄周期(上N次) long-unsigned
//}
					p8d+=2;
					UARTCtrl->RM_DI_TI[0]=p8d[0];//数据时标间隔 TI
					UARTCtrl->RM_DI_TI[1]=p8d[1];//数据时标间隔 TI
					UARTCtrl->RM_DI_TI[2]=p8d[2];//数据时标间隔 TI
					n=(p8d[4]<<8)|p8d[5];//补抄周期(上N次) long-unsigned
					UARTCtrl->RM_DI_N=n;//采集上第N次
				//补抄曲线:按任务执行间隔时间和采集方案采集间隔时间计算采集上第N次
					i=id&0xff;//方案号
					p8d=(u8*)(u8*)ADDR_6014_SDRAM+(i*LENper_6014);
					p8d=Get_Element(p8d,4,1,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8d)
					{
						i=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
						if(i==0x5B015002)//分钟冻结(曲线)
						{
							i=id;
							i>>=8;
							i&=0xff;//任务号
							//任务执行间隔时间
							x=ADDR_6012_2_SDRAM+(i*LENper_6012_2);
							i=pTItoSECOND((u8*)x+5);//TI转为秒值,出口:hex秒
							//采集方案采集间隔时间
							x=pTItoSECOND(UARTCtrl->RM_DI_TI);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
							i/=x;
							n*=i;
							n++;//电科测试招读8个点从22:00-23:59,但补抄时是从00:00上减8个点即22:15-00:00，故需多1点
							UARTCtrl->RM_DI_TI_Count=1;//按时标间隔采集时的间隔计数
							UARTCtrl->RM_DI_N=n;//采集上第N次
						}
					}
					break;
			}
		}
	}
	
//同类抄表任务列表
#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
	if(RM)
	{
		NUM_SomeTaskID=0;//不能按同类抄
		i=id;
		i>>=24;
		i&=0xf;//方案类型
		switch(i)
		{
			case 1://普通采集方案
				NUM_SomeTaskID=1;//可按同类抄
				break;
			case 2://事件采集方案
				break;
			default:
				break;
		}
		if(NUM_SomeTaskID==0)
		{//不能按同类抄
			NUM_SomeTaskID=1;
			UARTCtrl->NUM_SomeTaskID=1;//同类采集任务的id号的列表个数
			UARTCtrl->SomeTaskid[0]=id;//RMTASKmax byte 同类采集任务的b15-b8任务号,b7-b0方案编号
		}
		else
		{//可按同类抄
			NUM_SomeTaskID=0;//同类个数
			HM1=p32[0];
			HM1>>=16;//去b15-b8任务号,b7-b0方案编号
			for(i=0;i<NUM_TaskID;i++)
			{
				HM2=p32[i];
				HM2>>=16;//去b15-b8任务号,b7-b0方案编号
				if(HM1==HM2)
				{
					HM2=p32[i];
					UARTCtrl->SomeTaskid[NUM_SomeTaskID]=HM2;//RMTASKmax byte 同类采集任务的b15-b8任务号,b7-b0方案编号
					NUM_SomeTaskID++;
				}
			}
			UARTCtrl->NUM_SomeTaskID=NUM_SomeTaskID;//同类采集任务的id号的列表个数
		}
	}
#endif
	
//任务执行状态
#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
	if(RM)
	{//抄表任务
		TaskChange=0;//任务变化标志
		TH1TaskID=id;//同类中的第一个任务
		for(n=0;n<NUM_SomeTaskID;n++)
		{
			x=UARTCtrl->SomeTaskid[n];
			UARTCtrl->TaskID=(id&0xffff0000)|x;//RMTASKmax byte 当前同类采集任务的id号列表
			s=UARTCtrl->NUM_LastSomeTaskID;//上最近次同类采集任务的id号的列表个数
			for(i=0;i<s;i++)
			{
				if(x==UARTCtrl->LastSomeTaskid[i])
				{//上次有相同任务号
					break;
				}
			}
			if(i<s)
			{//任务号相同
				SourTaskID=UARTCtrl->TaskID;//原任务TaskID
			}
			else
			{
				SourTaskID=0;
			}
			i=(x>>8)&0xff;
			if(i>=NUMmax_6012_2)
			{
				return;
			}
			if((UARTCtrl->TaskTime[i].Run==1)||(UARTCtrl->TaskTime[i].Run==3)||(UARTCtrl->TaskTime[i].Run==4)||(UARTCtrl->TaskTime[i].Run==5))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
			{//原执行中
				UARTCtrl->TaskTime[i].Run=1;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
				s=0;//设置变化标志
				if(SourTaskID!=UARTCtrl->TaskID)//不比较优先级
				{//任务切换
					s=1;//设置变化标志
					TaskChange=1;//任务变化标志
				#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(PORTn==RS485_4PORT)
					{
						Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
					}
				#endif
				}
				x=Terminal_Ram->SETTask;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
				if(x&(1<<PORTn))
				{
					s=1;//设置变化标志
				#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(PORTn==RS485_4PORT)
					{
						Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
					}
				#endif
				}
				if(s)
				{//设置变化
					//UARTCtrl->NUMrmFAIL[i]=0;//清0抄表失败数
					CLR_RMCount(PORTn);//清0抄表发送帧各计数值
					NUMrm(PORTn);//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
				}
			}
			else
			{//重新执行
				UARTCtrl->RMLOOP[i]=0;//全部表抄读一遍的遍数计数,任务ID号数组
				UARTCtrl->NUMrmFAIL[i]=0;//清0抄表失败数
				NUMrm(PORTn);//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
				HM2=UARTCtrl->NUMrmOK[i];
				HM1=UARTCtrl->NUMrmBAD[i];
				if(HM2>=HM1)
				{
					HM2-=HM1;
				}
				HM1=UARTCtrl->NUMrmTOTAL[i];
				if(HM2>=HM1)
				{//OK数>=需抄数
					//不需抄表(需抄数=0);或停在当前任务插入其他任务置TaskID=0后返回,不进行采集任务监控初始化
					UARTCtrl->TaskTime[i].Run=2;//注:在RM_Monitor_Init(i)后置为=1;任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
					TH1TaskID=0;//同类中的第一个任务
				}
				else
				{
					TaskChange=1;//任务变化标志
					UARTCtrl->TaskTime[i].Run=1;//注:在RM_Monitor_Init(i)后置为=1;任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
					CLR_RMCount(PORTn);//清0抄表发送帧各计数值
				#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(PORTn==RS485_4PORT)
					{
						if((id>>28)!=0)
						{//不是,优先级=0;上电启抄,每小时曲线等用点抄
							Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
						}
					}
				#endif
					MW((u32)&Comm_Ram->DL698YMDHMS[0],(u32)&UARTCtrl->TaskTime[i].StartTime,7);//任务执行开始时间
				//#if (USER/100)==0//电科院测试
				#if ((USER/100)==0||(USER/100)==15)
					RM_Monitor_Init(i);//采集任务监控初始化;入口:ID=任务号
				#else
					RM_Monitor_Init_PORTn(PORTn,i);//采集任务监控初始化(按端口号);入口:PORTn=端口号,ID=任务号
				#endif
				}
			}
		}
		x=Terminal_Ram->SETTask;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
		x&=~(1<<PORTn);
		Terminal_Ram->SETTask=x;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
		
	//其他执行中任务Run==1暂停标志Run=3
		for(x=0;x<RMTASKmax;x++)
		{
			for(n=0;n<NUM_SomeTaskID;n++)
			{
				s=UARTCtrl->SomeTaskid[n];//RMTASKmax*2 byte 当前同类采集任务的b15-b8任务号,b7-b0方案编号
				s>>=8;
				if(x==s)
				{
					break;
				}
			}
			if(n<NUM_SomeTaskID)
			{//任务号相同
				continue;
			}
			if(UARTCtrl->TaskTime[x].Run==1)
			{
				UARTCtrl->TaskTime[x].Run=3;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
			}
		}
		
	//存为上最近次任务
		if(TaskChange)//任务变化标志
		{
			for(n=0;n<NUM_SomeTaskID;n++)
			{
				UARTCtrl->LastSomeTaskid[n]=UARTCtrl->SomeTaskid[n];
			}
			UARTCtrl->NUM_LastSomeTaskID=NUM_SomeTaskID;
		}
	//先置同类中的第一个任务
		UARTCtrl->TaskID=TH1TaskID;//同类中的第一个任务
		
	}
	else
	{//上报任务
#endif
		//以下为原同类任务更改前程序，保留作为上报任务以备参考用
		//UARTCtrl->LastTaskID=id;//最近次有效采集任务ID,用于显示等
		i=(id>>8)&0xff;
		if(i>=NUMmax_6012_2)
		{
			return;
		}
		if((UARTCtrl->TaskTime[i].Run==1)||(UARTCtrl->TaskTime[i].Run==3)||(UARTCtrl->TaskTime[i].Run==4)||(UARTCtrl->TaskTime[i].Run==5))//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
		{//原执行中
			UARTCtrl->TaskTime[i].Run=1;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
			s=0;//设置变化标志
			if((id<<4)==(UARTCtrl->TaskID<<4))//不比较优先级
			{//任务没变
				x=Terminal_Ram->SETTask;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
				if(x&(1<<PORTn))
				{
					s=1;//设置变化标志
				}
			}
			else
			{//任务切换
				UARTCtrl->TaskID=id;
				s=1;//设置变化标志
			}
			if(s)
			{//设置变化
				if(RM)
				{//抄表任务
					//UARTCtrl->NUMrmFAIL[i]=0;//清0抄表失败数
					CLR_RMCount(PORTn);//清0抄表发送帧各计数值
					NUMrm(PORTn);//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
				#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(PORTn==RS485_4PORT)
					{
						Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
					}
				#endif
				}
				else
				{//上报任务
					UARTCtrl->NUMrmTOTAL[i]=NUM_RMmax;//任务需采集总数
					UARTCtrl->NUMrmOK[i]=0;//各任务已采集完成电能表数
					UARTCtrl->NUMrmFAIL[i]=0;//各任务已采集失败电能表数
					UARTCtrl->NUMrmBAD[i]=0;//各任务已采集完成的电能表数中因配置变更等原因不再需抄读的数
				}
				if(RM)
				{
					x=Terminal_Ram->SETTask;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
					x&=~(1<<PORTn);
					Terminal_Ram->SETTask=x;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
				}
			}
		}
		else
		{//重新执行
			UARTCtrl->RMLOOP[i]=0;//全部表抄读一遍的遍数计数,任务ID号数组
			UARTCtrl->TaskID=id;
			if(RM)
			{//抄表任务
				UARTCtrl->NUMrmFAIL[i]=0;//清0抄表失败数
				//CLR_RMCount(PORTn);//清0抄表发送帧各计数值
				NUMrm(PORTn);//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
				HM2=UARTCtrl->NUMrmOK[i];
				HM1=UARTCtrl->NUMrmBAD[i];
				if(HM2>=HM1)
				{
					HM2-=HM1;
				}
				HM1=UARTCtrl->NUMrmTOTAL[i];
				if(HM2>=HM1)
				{//OK数>=需抄数
					//不需抄表;或可能插入其他任务置TaskID=0后返回,不进行采集任务监控初始化
					UARTCtrl->TaskTime[i].Run=2;//注:在RM_Monitor_Init(i)后置为=1;任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
					UARTCtrl->TaskID=0;
				}
				else
				{
					UARTCtrl->TaskTime[i].Run=1;//注:在RM_Monitor_Init(i)后置为=1;任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
					CLR_RMCount(PORTn);//清0抄表发送帧各计数值
				#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(PORTn==RS485_4PORT)
					{
						Terminal_Router->RouterCtrl=1;//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
					}
				#endif
					MW((u32)&Comm_Ram->DL698YMDHMS[0],(u32)&UARTCtrl->TaskTime[i].StartTime,7);//任务执行开始时间
				//#if (USER/100)==0//电科院测试
				#if ((USER/100)==0||(USER/100)==15)
					RM_Monitor_Init(i);//采集任务监控初始化;入口:ID=任务号
				#else
					RM_Monitor_Init_PORTn(PORTn,i);//采集任务监控初始化(按端口号);入口:PORTn=端口号,ID=任务号
				#endif
				}
			}
			else
			{//上报任务
				MW((u32)&Comm_Ram->DL698YMDHMS[0],(u32)&UARTCtrl->TaskTime[i].StartTime,7);//任务执行开始时间
	//			UARTCtrl->NUMrmTOTAL[i]=0;//任务需采集总数;代替文件搜索计数;
	//			UARTCtrl->NUMrmOK[i]=0;//各任务已采集完成电能表数;代替上报任中已经上报的列选择个数计数
	//			UARTCtrl->NUMrmFAIL[i]=0;//各任务已采集失败电能表数;代替
	//			UARTCtrl->NUMrmBAD[i]=0;//各任务已采集完成的电能表数中因配置变更等原因不再需抄读的数;代替
				UARTCtrl->TaskTime[i].Run=1;//注:在RM_Monitor_Init(i)后置为=1;任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
			}
			if(RM)
			{
				x=Terminal_Ram->SETTask;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
				x&=~(1<<PORTn);
				Terminal_Ram->SETTask=x;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
			}
		}

	//其他执行中任务Run==1暂停标志Run=3
		if(RM)
		{
			for(x=0;x<RMTASKmax;x++)
			{
				if(x==i)
				{
					continue;
				}
				if(UARTCtrl->TaskTime[x].Run==1)
				{
					UARTCtrl->TaskTime[x].Run=3;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=抄日冻结执行未完成,5=执行未完成
				}
			}
		}
#if RMSOMETASK==1//0=否,1=是;同类抄表任务归为同个任务抄表,不重启路由
	}
#endif
}



