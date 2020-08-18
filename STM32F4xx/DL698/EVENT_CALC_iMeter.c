
//内部交采事件记录文件
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/EVENT_CALC_iMeter.h"
#include "../DL698/EVENT_Record.h"

#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"



void EVENT_CALC_300F_3010(void)//电压电流逆相序事件
{
	u32 lastOccur;
	
	if(((Meter_Ram->IPFlags>>7)&0x1)!=((Meter_Ram->IPFlags>>3)&0x1))
	{//电压逆相序变化
		Meter_Ram->VIP_S_Timer=30;//电压逆相序秒定时器;电科院测试时为30，就用30不从FLASH中取值仅为省时间
		Meter_Ram->IPFlags&=0xf7;
		Meter_Ram->IPFlags|=(((Meter_Ram->IPFlags>>7)&0x1)<<3);
	}
	else
	{
		if(Meter_Ram->VIP_S_Timer==0x0)
		{
			Meter_Ram->VIP_S_Timer=30;//2;//电压逆相序秒定时器,每2秒运行进入
			Meter_Ram->IPFlags&=0xdf;
			Meter_Ram->IPFlags|=(((Meter_Ram->IPFlags>>3)&0x1)<<5);//b7=电压逆相序,b6=电流逆相序,b5=电压逆相序确定,b4=电流逆相序确定,b3=电压逆相序瞬时,b2=电流逆相序瞬时,b1=, b0=
			lastOccur=GET_LastEventOccurRecovery(0x300f0200,0);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
			if(Meter_Ram->IPFlags&(1<<5))
			{//产生
				if(lastOccur==0)
				{
					Event_OAD(0x300f0200,0,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
			else
			{//恢复
				if(lastOccur)
				{
					Event_OAD(0x300f0200,0,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
		}
	}
	if(((Meter_Ram->IPFlags>>6)&0x1)!=((Meter_Ram->IPFlags>>2)&0x1))
	{//电流逆相序变化
		Meter_Ram->IIP_S_Timer=60;//电流逆相序秒定时器
		Meter_Ram->IPFlags&=0xfb;
		Meter_Ram->IPFlags|=(((Meter_Ram->IPFlags>>6)&0x1)<<2);
	}
	else
	{
		if(Meter_Ram->IIP_S_Timer==0x0)
		{
			Meter_Ram->IIP_S_Timer=60;//2;//电流逆相序秒定时器,每2秒运行进入
			Meter_Ram->IPFlags&=0xef;
			Meter_Ram->IPFlags|=(((Meter_Ram->IPFlags>>2)&0x1)<<4);//b7=电压逆相序,b6=电流逆相序,b5=电压逆相序确定,b4=电流逆相序确定,b3=电压逆相序瞬时,b2=电流逆相序瞬时,b1=, b0=
			lastOccur=GET_LastEventOccurRecovery(0x30100200,0);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
			if(Meter_Ram->IPFlags&(1<<4))
			{//产生
				if(lastOccur==0)
				{
					Event_OAD(0x30100200,0,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
			else
			{//恢复
				if(lastOccur)
				{
					Event_OAD(0x30100200,0,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
		}
	}
}

void EventCurrentvalue_NUM(u8* pADDR,u32 Phase)//事件当前值记录表发生次数+1，入口:pADDR=记录表地址,Phase=相(b0=A相,b1=B相,b2=C相)
{
	u32 i;
//当前值记录表∷=array structure
//{
//	事件发生次数 double-long-unsigned，
//	事件累计时间 double-long-unsigned 
//}	
	pADDR[0]=DataType_array;
	pADDR[1]=4;
	//总
	pADDR[2]=DataType_structure;
	pADDR[3]=2;
	pADDR[4]=DataType_double_long_unsigned;
	if(Phase)
	{
		i=(pADDR[5]<<24)|(pADDR[6]<<16)|(pADDR[7]<<8)|pADDR[8];
		i++;
		pADDR[5]=i>>24;
		pADDR[6]=i>>16;
		pADDR[7]=i>>8;
		pADDR[8]=i;
	}
	pADDR[9]=DataType_double_long_unsigned;
	//A相
	pADDR[14]=DataType_structure;
	pADDR[15]=2;
	pADDR[16]=DataType_double_long_unsigned;
	if(Phase&1)
	{
		i=(pADDR[17]<<24)|(pADDR[18]<<16)|(pADDR[19]<<8)|pADDR[20];
		i++;
		pADDR[17]=i>>24;
		pADDR[18]=i>>16;
		pADDR[19]=i>>8;
		pADDR[20]=i;
	}
	pADDR[21]=DataType_double_long_unsigned;
	//B相
	pADDR[26]=DataType_structure;
	pADDR[27]=2;
	pADDR[28]=DataType_double_long_unsigned;
	if(Phase&2)
	{
		i=(pADDR[29]<<24)|(pADDR[30]<<16)|(pADDR[31]<<8)|pADDR[32];
		i++;
		pADDR[29]=i>>24;
		pADDR[30]=i>>16;
		pADDR[31]=i>>8;
		pADDR[32]=i;
	}
	pADDR[33]=DataType_double_long_unsigned;
	//C相
	pADDR[38]=DataType_structure;
	pADDR[39]=2;
	pADDR[40]=DataType_double_long_unsigned;
	if(Phase&4)
	{
		i=(pADDR[41]<<24)|(pADDR[42]<<16)|(pADDR[43]<<8)|pADDR[44];
		i++;
		pADDR[41]=i>>24;
		pADDR[42]=i>>16;
		pADDR[43]=i>>8;
		pADDR[44]=i;
	}
	pADDR[45]=DataType_double_long_unsigned;
}

void EventCurrentvalue_TIME(u8* pADDR,u32 Phase,u32 s)//事件当前值记录表累计时间，入口:pADDR=记录表地址,Phase=相(b0=A相,b1=B相,b2=C相),s=时间秒
{
	u32 i;
//当前值记录表∷=array structure
//{
//	事件发生次数 double-long-unsigned，
//	事件累计时间 double-long-unsigned 
//}	
	pADDR[0]=DataType_array;
	pADDR[1]=4;
	//总
	pADDR[2]=DataType_structure;
	pADDR[3]=2;
	pADDR[4]=DataType_double_long_unsigned;
	pADDR[9]=DataType_double_long_unsigned;
	if(Phase)
	{
		i=(pADDR[10]<<24)|(pADDR[11]<<16)|(pADDR[12]<<8)|pADDR[13];
		i+=s;
		pADDR[10]=i>>24;
		pADDR[11]=i>>16;
		pADDR[12]=i>>8;
		pADDR[13]=i;
	}
	//A相
	pADDR[14]=DataType_structure;
	pADDR[15]=2;
	pADDR[16]=DataType_double_long_unsigned;
	pADDR[21]=DataType_double_long_unsigned;
	if(Phase&1)
	{
		i=(pADDR[22]<<24)|(pADDR[23]<<16)|(pADDR[24]<<8)|pADDR[25];
		i+=s;
		pADDR[22]=i>>24;
		pADDR[23]=i>>16;
		pADDR[24]=i>>8;
		pADDR[25]=i;
	}
	//B相
	pADDR[26]=DataType_structure;
	pADDR[27]=2;
	pADDR[28]=DataType_double_long_unsigned;
	pADDR[33]=DataType_double_long_unsigned;
	if(Phase&2)
	{
		i=(pADDR[34]<<24)|(pADDR[35]<<16)|(pADDR[36]<<8)|pADDR[37];
		i+=s;
		pADDR[34]=i>>24;
		pADDR[35]=i>>16;
		pADDR[36]=i>>8;
		pADDR[37]=i;
	}
	//C相
	pADDR[38]=DataType_structure;
	pADDR[39]=2;
	pADDR[40]=DataType_double_long_unsigned;
	pADDR[45]=DataType_double_long_unsigned;
	if(Phase&4)
	{
		i=(pADDR[46]<<24)|(pADDR[47]<<16)|(pADDR[48]<<8)|pADDR[49];
		i+=s;
		pADDR[46]=i>>24;
		pADDR[47]=i>>16;
		pADDR[48]=i>>8;
		pADDR[49]=i;
	}
}


void EVENT_CALC_3003(void)//断相标志和事件(标准事件)
{
	u32 i;
	u32 f;
	u32 us;
	u32 is;
	u32 delay;
	u8* p8;
	u32 lastOccur;
	
	
	if(Meter_Ram->ALP_S_Timer&1)
	{
		return;//每2秒1次，减小运行次数
	}
	i=MRR(ADDR_3003+1,1);//有效标识
	if(i==0)
	{//无效
		Meter_Ram->ALP_S_Timer=59;//奇数秒下次不进行运行
		Meter_Ram->LPFlags=0;
		return;
	}
	
	f=0;//清断相标志
//#define ADDR_3003_5     ADDR_3003+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  电压触发上限  long-unsigned（单位：V，换算：-1），
//  电流触发上限  double-long（单位：A，换算：-4），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
	//读断相参数
	MR(ADDR_DATABUFF,ADDR_3003_5,LENmax_3003_5);
	p8=(u8*)ADDR_DATABUFF;
	us=(p8[3]<<8)|p8[4];
	is=(p8[6]<<24)|(p8[7]<<16)|(p8[8]<<8)|p8[9];
	us=hex_bcd(us);
	is=hex_bcd(is);
	delay=p8[11];
	if(delay==0x0)
	{
		delay=60;
	}
//A
	i=MRR(ADDR_UA+2,2);
	if(i<=us)
	{
		i=MRR(ADDR_IA,4);
		i&=0x7fffffff;
		i>>=4;
		if(i<=is)
		{
			f|=1;//A当前断相
		}
	}
//B
	if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
	{
		i=MRR(ADDR_UB+2,2);
		if(i<=us)
		{
			i=MRR(ADDR_IB,4);
			i&=0x7fffffff;
			i>>=4;
			if(i<=is)
			{
				f|=2;//B当前断相
			}
		}
	}
//C
	i=MRR(ADDR_UC+2,2);
	if(i<=us)
	{
		i=MRR(ADDR_IC,4);
		i&=0x7fffffff;
		i>>=4;
		if(i<=is)
		{
			f|=4;//C当前断相
		}
	}

//A判定
	i=f&0x01;
	if(i!=(Meter_Ram->LPFlags&0x01))
	{//A断相变化
		Meter_Ram->LPFlags&=0xfe;
		Meter_Ram->LPFlags|=i;
		Meter_Ram->ALP_S_Timer=delay;
	}
	else
	{
		if(Meter_Ram->ALP_S_Timer==0x0)
		{
			Meter_Ram->LPFlags&=0xef;
			Meter_Ram->LPFlags|=(i<<4);
			Meter_Ram->ALP_S_Timer=delay;//每x秒运行进入
			lastOccur=GET_LastEventOccurRecovery(0x30030700,0);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
			if(i)
			{//产生
				if(lastOccur==0)
				{
				#if (USER/100)!=15//安徽
					Event_OAD(0x30030600,0,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				#endif	
					Event_OAD(0x30030700,0,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					EventCurrentvalue_NUM((u8*)ADDR_EventCurrentvalue_3003,1<<0);//事件当前值记录表发生次数+1，入口:pADDR=记录表地址,Phase=相(b0=A相,b1=B相,b2=C相)
				}
			}
			else
			{//恢复
				if(lastOccur)
				{
				#if (USER/100)!=15//安徽
					Event_OAD(0x30030600,0,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				#endif	
					Event_OAD(0x30030700,0,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
			
		}
	}
//B判定
	i=f&0x02;
	if(i!=(Meter_Ram->LPFlags&0x02))
	{//B断相变化
		Meter_Ram->LPFlags&=0xfd;
		Meter_Ram->LPFlags|=i;
		Meter_Ram->BLP_S_Timer=delay;
	}
	else
	{
		if(Meter_Ram->BLP_S_Timer==0x0)
		{
			Meter_Ram->LPFlags&=0xdf;
			Meter_Ram->LPFlags|=(i<<4);
			Meter_Ram->BLP_S_Timer=delay;//每x秒运行进入
			lastOccur=GET_LastEventOccurRecovery(0x30030800,0);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
			if(i)
			{//产生
				if(lastOccur==0)
				{
				#if (USER/100)!=15//安徽
					Event_OAD(0x30030600,0,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				#endif	
					Event_OAD(0x30030800,0,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					EventCurrentvalue_NUM((u8*)ADDR_EventCurrentvalue_3003,1<<1);//事件当前值记录表发生次数+1，入口:pADDR=记录表地址,Phase=相(b0=A相,b1=B相,b2=C相)
				}
			}
			else
			{//恢复
				if(lastOccur)
				{
				#if (USER/100)!=15//安徽
					Event_OAD(0x30030600,0,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				#endif
					Event_OAD(0x30030800,0,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
			
		}
	}
//C判定
	i=f&0x04;
	if(i!=(Meter_Ram->LPFlags&0x04))
	{//C断相变化
		Meter_Ram->LPFlags&=0xfb;
		Meter_Ram->LPFlags|=i;
		Meter_Ram->CLP_S_Timer=delay;
	}
	else
	{
		if(Meter_Ram->CLP_S_Timer==0x0)
		{
			Meter_Ram->LPFlags&=0xbf;
			Meter_Ram->LPFlags|=(i<<4);
			Meter_Ram->CLP_S_Timer=delay;//每x秒运行进入
			lastOccur=GET_LastEventOccurRecovery(0x30030900,0);//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
			if(i)
			{//产生
				if(lastOccur==0)
				{
				#if (USER/100)!=15//安徽
					Event_OAD(0x30030600,0,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				#endif
					Event_OAD(0x30030900,0,1,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					EventCurrentvalue_NUM((u8*)ADDR_EventCurrentvalue_3003,1<<2);//事件当前值记录表发生次数+1，入口:pADDR=记录表地址,Phase=相(b0=A相,b1=B相,b2=C相)
				}
			}
			else
			{//恢复
				if(lastOccur)
				{
				#if (USER/100)!=15//安徽
					Event_OAD(0x30030600,0,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				#endif
					Event_OAD(0x30030900,0,0,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			}
		}
	}
	
}































