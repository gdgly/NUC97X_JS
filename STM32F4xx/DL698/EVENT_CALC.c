
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/EVENT_Record.h"
#include "../DL698/EVENT_CALC.h"
#include "../DL698/RM_Record.h"
#include "../DL698/RM_TASK.h"
#include "../Device/MEMRW.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../Calculate/Calculate.h"




u8* Get_RMOADDATA(u32 OAD,u8* pDATA)//得到抄表数据中指定的OAD数据指针,入口:pDATA=抄表数据;返回:0=没找到
{
	u32 i;
	u32 x;
	u32 Offset;
	u32 Len;
	u32 n;
	u32 OADcount;
	u8* pData;
	
	
	switch(OAD)
	{
		case 0x60400200://采集启动时标
			return pDATA;
		case 0x60410200://采集成功时标
			return pDATA+7;
		case 0x60420200://采集存储时标
			return pDATA+14;
	}
	//计算记录数据开始指针
	Offset=21;
	n=pDATA[Offset+1];
	Offset+=2;
	for(i=0;i<n;i++)
	{
		if(pDATA[Offset+1]==0)
		{//OAD
			Offset+=6;
		}
		else
		{//ROAD
			x=pDATA[Offset+6];//关联OAD个数
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	pData=pDATA+Offset;
//电能表数据块区,长度按实际写入数
//0 7byte 采集启动时标  date_time_s(无数据类型)
//7 7byte 采集成功时标  date_time_s(无数据类型)
//14 7byte 采集存储时标  date_time_s(无数据类型)
//21  采集的CSD  array CSD(有数据类型)
//n  采集的数据(有数据类型)
	//找相同OAD在记录中的OAD计数
	Offset=21+2;
	OADcount=0;//相同OAD在记录中的OAD计数
	for(i=0;i<n;i++)
	{
		x=(pDATA[Offset+2]<<24)+(pDATA[Offset+3]<<16)+(pDATA[Offset+4]<<8)+pDATA[Offset+5];
		if(OAD==x)
		{
			break;
		}
		if(pDATA[Offset+1]==0)
		{//OAD
			OADcount++;
			Offset+=6;
		}
		else
		{//ROAD
			x=pDATA[Offset+5];//关联OAD个数
			OADcount+=x;
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	if(i>=n)
	{//没找到
		return 0;
	}
	Len=0;//数据偏移
	while(OADcount--)
	{
		Len+=Get_DL698DataLen_S(pData+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
	}
	return pData+Len;
}









u32 EVENT_CALC_3105(u32 PORTn,u8* pDATA)//抄表数据相关的事件-电能表时钟超差事件;返回:0=非事件关联的id,1=是事件关联的id
{
	u32 i;
	u32 x;
	u32 lastOccur;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	x=UARTCtrl->TaskID;
	x>>=8;
	x&=0xff;//任务号
	i=MRR(ADDR_3105_6+3,4);
	if(((i>>24)&0xff)!=x)
	{
		return 0;//关联采集任务号不同
	}
	i&=0xffff;//异常判别阈值  long-unsigned（单位：秒）
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换)
#if ((USER%100)!=0 &&(USER/100)!=15)//不是电科院台体测试
	x=(Comm_Ram->DL698YMDHMS[4]<<8)|Comm_Ram->DL698YMDHMS[5];//当时钟时分
	if((x>=((23<<8)|55))||(x<5))
	{//终端时钟时分>=23点55分或<0点5分,不判断时钟超差事件
		return 1;//返回:0=非事件关联的id,1=是事件关联的id
	}
#endif
	pDATA=Get_RMOADDATA(0x40000200,pDATA);//得到抄表数据中指定的OAD或ROAD数据指针,入口:pOAD无数据类型,pDATA=抄表数据;返回:0=没找到
	if(pDATA)
	{
		if(pDATA[0]==DataType_date_time_s)
		{
			x=YYMDHMS_Sub_hex(pDATA+1,Comm_Ram->DL698YMDHMS);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
			if(x>>31)
			{
				x=~x;
				x++;
			}
			UARTCtrl->TSA[0]=DataType_TSA;
			UARTCtrl->TSA[1]=7;
			UARTCtrl->TSA[2]=5;
			lastOccur=GET_LastEventOccurRecovery(0x31050200,UARTCtrl->TSA);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
			if(x>=i)
			{//产生
				if(lastOccur==0)
				{
					Event_31050200(UARTCtrl->TSA,1,pDATA);//终端停/上电事件记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
			else
			{//恢复
				if(lastOccur)
				{
					Event_31050200(UARTCtrl->TSA,0,pDATA);//终端停/上电事件记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
		}
	}
	return 1;//返回:0=非事件关联的id,1=是事件关联的id
}


u32 EVENT_CALC_310B_310C_310D_310E(u32 PORTn,u8* pDATA)//抄表数据相关的事件-电能表示度下降事件,电能量超差事件,电能表飞走事件,电能表停走事件
{
	u32 i;
	u32 id;
	u32 eventid;//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
	u32 threshold_310C;//超差阈值  double-long-unsigned（单位：%，无换算）
	u32 threshold_310D;//飞走阈值  double-long-unsigned（单位：%，无换算）
	u32 ti;//停走阈值 TI
	u8* pKWHc;//当前抄表电能示值
	u8* pKWHs;//上次抄表电能示值
	u8* pTIMEs;//上次抄表成功时间
	u32 t;//2次抄表时间差秒
	u64 d64a;
	u64 d64b;
	u32 lastOccur;
	EventSour_typedef* EventSour;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 x;
//	u8* pKWc;//当前抄表功率 double-long，单位：W，换算：-1
//	double f64;
	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	id=UARTCtrl->TaskID;
	id>>=8;
	id&=0xff;//任务号
#if (LENmax_310B_6+4+LENmax_EventOAD+LENmax_310C_6+4+LENmax_EventOAD+LENmax_310D_6+4+LENmax_EventOAD+LENmax_310E_6)>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_310B_6,LENmax_310B_6+4+LENmax_EventOAD+LENmax_310C_6+4+LENmax_EventOAD+LENmax_310D_6+4+LENmax_EventOAD+LENmax_310E_6);
	eventid=0;//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
	pKWHs=(u8*)ADDR_DATABUFF;
	//电能表示度下降事件参数
	i=pKWHs[3];
	if(i==id)
	{
		eventid|=0x01;//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
	}
	//电能量超差事件参数
	pKWHs+=LENmax_310B_6+4+LENmax_EventOAD;
	threshold_310C=(pKWHs[3]<<24)|(pKWHs[4]<<16)|(pKWHs[5]<<8)|(pKWHs[6]<<0);//超差阈值  double-long-unsigned（单位：%，无换算）
	i=pKWHs[8];
	if(i==id)
	{
		if(PORTn!=RS485_4PORT)
		{//不是载波口,载波口不产生此事件因采集间隔时间不准可能误报
			eventid|=0x02;//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
		}
	}
	//电能表飞走事件参数
	pKWHs+=LENmax_310C_6+4+LENmax_EventOAD;
	threshold_310D=(pKWHs[3]<<24)|(pKWHs[4]<<16)|(pKWHs[5]<<8)|(pKWHs[6]<<0);//飞走阈值  double-long-unsigned（单位：%，无换算）
	i=pKWHs[8];
	if(i==id)
	{
		if(PORTn!=RS485_4PORT)
		{//不是载波口,载波口不产生此事件因采集间隔时间不准可能误报
			eventid|=0x04;//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
		}
	}
	//电能表停走事件参数
	pKWHs+=LENmax_310D_6+4+LENmax_EventOAD;
	ti=pTItoSECOND(pKWHs+3);//停走阈值 TI//TI转为秒值,入口:pTI无数据类型;出口:hex秒
	i=pKWHs[7];
	if(i==id)
	{
		eventid|=0x08;//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
	}
	if(eventid==0)
	{
		return 0;//关联采集任务号不同
	}
	
	if(eventid&8)//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
	{
		i=AddrToMeterSetNo((u16*)&UARTCtrl->RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
		if(i>=NUM_RMmax)
		{
			EventSour=0;//无效指针
		}
		else
		{
			EventSour=(EventSour_typedef*)(ADDR_Event_Sour+(i*sizeof(EventSour_typedef)));
			for(i=0;i<3;i++)
			{
				if(EventSour->RMAddr[i]!=UARTCtrl->RMAddr[i])
				{//地址不同
					for(i=0;i<3;i++)
					{
						EventSour->RMAddr[i]=UARTCtrl->RMAddr[i];
					}
					EventSour->TimeCount=0;//电能表停走时间计时(秒)
					break;
				}
			}
		}
	}
	
	i=Get_LastRecordFileData(id,(u16*)&UARTCtrl->RMAddr);//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF数据长度，0=没找到
	if(i==0)
	{
		return 1;//没找到原记录
	}
//事件缓冲使用约定
//ADDR_DATABUFF事件编辑存放
//ADDR_128KDATABUFF  32KBYTE 保留给DL698_DataDef
//ADDR_128KDATABUFF+32K 32KBYTE 事件参数,关联对象属性表等
//ADDR_128KDATABUFF+64K 32KBYTE 产生事件用临时数据
//ADDR_128KDATABUFF+96K 32KBYTE 事件中各项数据的相应OAD顺序表array OAD
	if(i>(32*1024))
	{
		return 1;//保留给DL698_DataDef长度超出
	}
	pKWHc=Get_RMOADDATA(0x00100200,pDATA);//得到抄表数据中指定的OAD或ROAD数据指针,入口:pOAD无数据类型,pDATA=抄表数据;返回:0=没找到
	if(pKWHc)
	{
		pKWHs=Get_RMOADDATA(0x00100200,(u8*)ADDR_128KDATABUFF);//得到抄表数据中指定的OAD或ROAD数据指针,入口:pOAD无数据类型,pDATA=抄表数据;返回:0=没找到
		if(pKWHs)
		{
			//发生源
			UARTCtrl->TSA[0]=DataType_TSA;
			UARTCtrl->TSA[1]=7;
			UARTCtrl->TSA[2]=5;
			//00102201事件发生前正向有功总电能
			//00106201事件结束前正向有功总电能
			MR(ADDR_128KDATABUFF+(64*1024),(u32)pKWHs,2+((TMaxTariff+1)*9));
			//00104201事件发生后正向有功总电能
			//00108201事件结束后正向有功总电能
			MR(ADDR_128KDATABUFF+(64*1024)+1024,(u32)pKWHc,2+((TMaxTariff+1)*9));
			//00202201事件发生前反向有功总电能
			//00206201事件结束前反向有功总电能
			pKWHs=(u8*)ADDR_128KDATABUFF+(64*1024)+128;
			pKWHs[0]=0;
			pKWHs=Get_RMOADDATA(0x00200200,(u8*)ADDR_128KDATABUFF);//得到抄表数据中指定的OAD或ROAD数据指针,入口:pOAD无数据类型,pDATA=抄表数据;返回:0=没找到
			if(pKWHs)
			{
				MR(ADDR_128KDATABUFF+(64*1024)+128,(u32)pKWHs,2+((TMaxTariff+1)*9));
			}
			//00204201事件发生后反向有功总电能
			//00208201事件结束后反向有功总电能
			pKWHs=(u8*)ADDR_128KDATABUFF+(64*1024)+1024+128;
			pKWHs[0]=0;
			pKWHs=Get_RMOADDATA(0x00200200,pDATA);//得到抄表数据中指定的OAD或ROAD数据指针,入口:pOAD无数据类型,pDATA=抄表数据;返回:0=没找到
			if(pKWHs)
			{
				MR(ADDR_128KDATABUFF+(64*1024)+1024+128,(u32)pKWHs,2+((TMaxTariff+1)*9));
			}
			
			pKWHs=(u8*)ADDR_128KDATABUFF+(64*1024);//ADDR_128KDATABUFF  32KBYTE 保留给DL698_DataDef 可能变化
			t=0;//2次抄表时间差秒
			pTIMEs=Get_RMOADDATA(0x60410200,(u8*)ADDR_128KDATABUFF);//上次抄表成功时间
			if(pTIMEs)
			{
				t=YYMDHMS_Sub_hex(pTIMEs,(u8*)ADDR_DL698YMDHMS);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
				if(t>>31)
				{//负
					t=0;
				}
			}
			
			MC(0,ADDR_DATABUFF,16);
			if(pKWHc[2]!=DataType_double_long_unsigned)
			{
				if(pKWHc[2]!=DataType_long64_unsigned)
				{
					return 1;
				}
				MR(ADDR_DATABUFF,(u32)pKWHc+3,8);
			}
			else
			{
				MR(ADDR_DATABUFF+4,(u32)pKWHc+3,4);
			}
			if(pKWHs[2]!=DataType_double_long_unsigned)
			{
				if(pKWHs[2]!=DataType_long64_unsigned)
				{
					return 1;
				}
				MR(ADDR_DATABUFF+8,(u32)pKWHs+3,8);
			}
			else
			{
				MR(ADDR_DATABUFF+12,(u32)pKWHs+3,4);
			}
			d64a=MRR(ADDR_DATABUFF,8);//当前示值
			d64b=MRR(ADDR_DATABUFF+8,8);//上次示值
			d64a=R_Inverse(d64a,8);
			d64b=R_Inverse(d64b,8);
		//电能表示度下降事件
			if(eventid&1)//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
			{
				if(d64a<d64b)
				{
					Event_OAD(0x310B0200,UARTCtrl->TSA,2,1);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					return 1;
				}
			}
			
			if(t==0)
			{
				return 1;//时间差=0
			}
			d64a-=d64b;//电能差
		//电能表停走事件
			if(eventid&8)//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
			{
				if(EventSour)//有效指针
				{
					lastOccur=GET_LastEventOccurRecovery(0x310E0200,UARTCtrl->TSA);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
					if(d64a==0)//电能差
					{//停走
						t+=EventSour->TimeCount;//电能表停走时间计时(秒)
						EventSour->TimeCount=t;//电能表停走时间计时(秒)
						if(ti)
						{//停走阈值不是0
							if(t>=ti)
							{//大于停走阈值 TI
								if(lastOccur==0)
								{
									Event_OAD(0x310E0200,UARTCtrl->TSA,1,1);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
								}
							}
						}
						return 1;
					}
					else
					{//没停走
						EventSour->TimeCount=0;//电能表停走时间计时(秒)
						if(lastOccur)
						{
							Event_OAD(0x310E0200,UARTCtrl->TSA,0,0);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
						}
					}
				}
			}

			if((PORTn!=RS485_1PORT)&&(PORTn!=RS485_2PORT))
			{
				return 1;//不是485口
			}
/*
			pKWc=Get_RMOADDATA(0x20040200,pDATA);//得到抄表数据中指定的OAD或ROAD数据指针,入口:pOAD无数据类型,pDATA=抄表数据;返回:0=没找到
			if(pKWc==0)
			{
				return 1;
			}
			i=(pKWc[2]<<8)|pKWc[3];
			if(i&0x8000)
			{//功率为负,反向
				return 1;
			}
			f64=i;
			f64*=t;
			f64/=3600;//kws变为kwh,4位小数
			x=(u32)f64;//t=应走电量kwh,4位小数
			if(x<=100)
			{//应走电量kwh<=0.0100时不比较
				return 1;
			}
			i=d64a;
			i*=100;
			i/=x;
*/
			i=d64a;//电能差
			i*=10;
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{//飞走、超差事件只开通485口
			//电能表飞走事件
				if(eventid&4)//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
				{
					lastOccur=GET_LastEventOccurRecovery(0x310D0200,UARTCtrl->TSA);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
					if(i>=threshold_310D)//飞走阈值  double-long-unsigned（单位：%，无换算）
					{
						if(lastOccur==0)
						{
							Event_OAD(0x310D0200,UARTCtrl->TSA,1,1);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
						}
						return 1;
					}
					else
					{
						if(lastOccur)
						{
							Event_OAD(0x310D0200,UARTCtrl->TSA,0,0);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
						}
					}
				}
			//电能量超差事件
				if(eventid&2)//事件ID:b0=示度下降,b1=超差,b2=飞走,b3=停走
				{
					lastOccur=GET_LastEventOccurRecovery(0x310C0200,UARTCtrl->TSA);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
					if(i>=threshold_310C)//超差阈值  double-long-unsigned（单位：%，无换算）
					{
						if(lastOccur==0)
						{
							Event_OAD(0x310C0200,UARTCtrl->TSA,1,1);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
						}
						return 1;
					}
					else
					{
						if(lastOccur)
						{
							Event_OAD(0x310C0200,UARTCtrl->TSA,0,0);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
						}
					}
				}
			}
			
		}
	}
	return 1;//返回:0=非事件关联的id,1=是事件关联的id
}





u32 EVENT_CALC_311C(u32 PORTn,u8* pRecord)//抄表数据相关的事件-电能表数据变更监控记录;返回:0=非事件关联的id,1=是事件关联的id
{
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 m;
	u8* pRecord1;
	u32 LEN_CSD;
	u32 CSDoffset;
	u32 DATAoffset;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	n=UARTCtrl->TaskID;
	n>>=8;
	n&=0xff;//任务号
	i=MRR(ADDR_311C_6+3,1);
	if(i!=n)
	{//关联采集任务号不同
		return 0;//返回:0=非事件关联的id,1=是事件关联的id
	}
	i=Get_LastRecordFileData(n,(u16*)UARTCtrl->RMAddr);//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF的数据长度，0=没找到
	if(i==0)
	{
		return 1;//返回:0=非事件关联的id,1=是事件关联的id
	}
	pRecord1=(u8*)ADDR_128KDATABUFF;
	pRecord+=21;
	pRecord1+=21;
	//计算记录数据CSD长度
	n=pRecord[1];//CSD个数
	LEN_CSD=2;
	for(i=0;i<n;i++)
	{
		if(pRecord[LEN_CSD+1]==0)
		{//OAD
			LEN_CSD+=6;
		}
		else
		{//ROAD
			x=pRecord[LEN_CSD+6];//关联OAD个数
			x*=4;
			x+=7;
			LEN_CSD+=x;
		}
	}
	n=pRecord[1];//CSD个数
	x=pRecord1[1];//CSD个数
	if(x!=n)
	{
		return 1;//返回:0=非事件关联的id,1=是事件关联的id
	}
	CSDoffset=2;
	DATAoffset=0;
	for(m=0;m<n;m++)
	{
		//比较CSD是否相同
		if(pRecord[CSDoffset+1]==0)
		{//OAD
			x=6;
		}
		else
		{//ROAD
			x=pRecord[CSDoffset+6];//关联OAD个数
			x*=4;
			x+=7;
		}
		for(i=0;i<x;i++)
		{
			if(pRecord[CSDoffset+i]!=pRecord1[CSDoffset+i])
			{//CSD不同不比较
				return 1;
			}
		}
		//比较DATA是否相同
		y=Get_DL698DataLen_S(pRecord+LEN_CSD+DATAoffset,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		for(i=0;i<y;i++)
		{
			if(pRecord[LEN_CSD+DATAoffset+i]!=pRecord1[LEN_CSD+DATAoffset+i])
			{//DATA不同
				Event_311C0200(PORTn,pRecord+CSDoffset,pRecord1+LEN_CSD+DATAoffset,pRecord+LEN_CSD+DATAoffset);//电能表数据变更监控记录,入口:pCSD=监控的CSD,pDATA1=变更前数据,pDATA2=变更后数据
				break;
			}
		}
		CSDoffset+=x;
		DATAoffset+=y;
	}
	return 1;//返回:0=非事件关联的id,1=是事件关联的id
}









u32 Event_RMDATA(u32 PORTn,u8* pDATA)//抄表数据相关的事件;入口:pDATA=抄表数据,返回:0=非事件关联的id,1=是事件关联的id
{
	u32 EventID;
	
	EventID=0;
//3105 电能表时钟超差事件
	EventID|=EVENT_CALC_3105(PORTn,pDATA);//抄表数据相关的事件-电能表时钟超差事件;返回:0=非事件关联的id,1=是事件关联的id
//310B,310C,310D,310E 电能量超差事件,电能表飞走事件,电能表停走事件
	EventID|=EVENT_CALC_310B_310C_310D_310E(PORTn,pDATA);//抄表数据相关的事件-电能表示度下降事件,电能量超差事件,电能表飞走事件,电能表停走事件;返回:0=非事件关联的id,1=是事件关联的id
//311C
	EventID|=EVENT_CALC_311C(PORTn,pDATA);//抄表数据相关的事件-电能表数据变更监控记录;返回:0=非事件关联的id,1=是事件关联的id
	
	
	
	
	
	return EventID;
}













