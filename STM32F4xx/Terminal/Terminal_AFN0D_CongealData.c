
//终端类2数据计算
#include "Project.h"
#include "Terminal_AFN0D_CongealData.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0D_SourceData.h"

#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "KeyUser.h"




extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);



void Init_Class2Data_UpPower(void)//类2数据初始化(上电)
{
//计算用原始数据初始化
	
	//曲线冻结用电能量原始数据初始化
	MC(0xee,ADDR_AFNOD_SOUR_Start,(ADDR_AFNOD_SOUR_End)-(ADDR_AFNOD_SOUR_Start));
}

void Init_Class2Data_Command(void)//类2数据初始化(命令,全部清除)
{
	u32 * p32;
	
//清原冻结
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//清冻结自动IOW
	p32[3]=0;
//日,抄表日,月,曲线冻结数据无效
	MC(0x0,ADDR_TCongealCount,LEN_TCongealCount);
	MC(0x0,ADDR_PnCongealCount,LEN_PnCongealCount*MaxRS485AddCarrierPn);
	Init_Class2Data_UpPower();//类2数据初始化(上电)
	
#if (USER/100)==5//上海
	MC(0xff,ADDR_AFN0CF242_Next,6*AFN0CF242MaxCount);//当日登录主站信息
	MC(0xff,ADDR_AFN0CF243,24);//当日信号强度
	MC(0,ADDR_CURVECONGEAL_ExtCount,7);//清0扩展曲线计数
#endif

#if (USER/100)==11//吉林
	//电压统计数据掉电RAM中的数据,因库中已改指向RAM
	MC(0,ADDR_AFN0DF27_SOUR,(66+27)*AFN0DF27MaxPn);
	MC(0,ADDR_AFN0DF35_SOUR,(66+27)*AFN0DF35MaxPn);
#endif
}

void Class2Congeal_AutoRW(void)//类2冻结自动IO读写或清0,完成后结果处理
{
	u32 i;
	u32 Count;
	u8 * p8;
	u32 * p32;

	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);
	if(p32[3]!=0x0)
	{
		//i=Get_LEN_FLASHSectorWrite(p32[2]);//得到FLASH当前地址扇区可写字节长度
		i=(128*1024)-(p32[2]&0x1ffff);//当前块能写字节
		if(i>p32[3])
		{
			i=p32[3];
		}
		p8=(u8 *)(ADDR_Class2Congeal_AutoRW);
		switch(p8[0])
		{
			case 0://IOCLR
				MC(p8[1],p32[2],i);
				break;
			case 1://IOR
				MR(p32[1],p32[2],i);
				break;
			case 2://IOW
				MW(p32[1],p32[2],i);
				break;
			default:
				p32[3]=0;
				return;
		}
		p32[1]+=i;
		p32[2]+=i;
		p32[3]-=i;
		if(p32[3]==0x0)
		{//长度=0,完成后处理
			//计数+1,清正在自动写冻结标志
			switch(p8[2])//数据标记0=曲线冻结,1=日冻结,2=抄表日冻结,3=月冻结,4=日冻结更新,5=补抄上2日,6=补抄上3日
			{
				case 0://0=曲线冻结
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],5);//冻结有效(写冻结时标)
					if(Terminal_Ram->CurveCongealTask==0)//曲线冻结任务0-24(当点和上1-24点)
					{
						MW(ADDR_Class2Congeal_AutoRW+20,ADDR_TCongealCount+9,5);//写上次冻结时标
					}
					Count=MRR(ADDR_TCongealCount+7,2);
					if(p32[4]==(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*(Count&0x7fff))))
					{//当前冻结地址是计数后最后,计数+1
						PnCurveCongealCount();//测量点的曲线冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
						i=Count&0x8000;
						Count&=0x7fff;
						Count+=1;
						if(Count>=MaxCurveCongeal)
						{
							Count=0;
							i=0x8000;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+7,2);
					}
					Terminal_Ram->CurveCongealTask++;//曲线冻结任务0-24(当时,上1-24时)
					break;
				case 1://1=日冻结
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],3);//冻结有效(写冻结时标)
					Count=MRR(ADDR_TCongealCount+14,1);
					if(p32[4]==(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*(Count&0x7f))))
					{//当前冻结地址是计数后最后,计数+1
						PnDateCongealCount();//测量点的日冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxDateCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+14,1);
					}
					MW(ADDR_TYMDHMS+3,ADDR_TCongealCount+16,3);//写上次冻结时标
					Terminal_Ram->CongealFlags|=4;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
					
					MC(0x0,ADDR_AFN0DF27_PAR,18*AFN0DF27MaxPn);//日平均电压累加计数(日冻结后清0)
					break;
				case 2://2=抄表日冻结
					//按抄日和时分冻结
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],3);//冻结有效(写冻结时标)
				/*
					//按抄表日未24点冻结
					i=YMD_Sub1D(MRR(ADDR_TYMDHMS+3,3));//年月日减1日,返回减1后的年月日
					MWR(i,p32[4],3);//冻结有效(写冻结时标)
				*/
					Count=MRR(ADDR_TCongealCount+21,1);
					if(p32[4]==(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*(Count&0x7f))))
					{//当前冻结地址是计数后最后,计数+1
						PnRMDateCongealCount();//测量点的抄表日冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxRMDateCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+21,1);
					}
					MW(ADDR_TYMDHMS+1,ADDR_TCongealCount+23,5);//写上次冻结时标
					Terminal_Ram->CongealFlags|=8;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
					break;
				case 3://3=月冻结
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],2);//冻结有效(写冻结时标)
					Count=MRR(ADDR_TCongealCount+28,1);
					if(p32[4]==(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*(Count&0x7f))))
					{//当前冻结地址是计数后最后,计数+1
						PnMonthCongealCount();//测量点的月冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxMonthCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+28,1);
					}
					MW(ADDR_TYMDHMS+4,ADDR_TCongealCount+30,2);//写上次冻结时标
					Terminal_Ram->CongealFlags|=0x10;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
					
					MC(0x0,ADDR_AFN0DF35_PAR,18*AFN0DF27MaxPn);//月平均电压累加计数(月冻结后清0)
					break;
				case 4://4=日冻结更新(上日抄到数据补冻为上上日冻结)
				case 5://5=补抄上2日
				case 6://6=补抄上3日
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],3);//冻结有效(写冻结时标)
					Count=MRR(ADDR_TCongealCount+14,1);
					if(p32[4]==(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*(Count&0x7f))))
					{//当前冻结地址是计数后最后,计数+1
						PnDateCongealCount();//测量点的日冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxDateCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+14,1);
					}
					break;
				case 7://7=月冻结更新(上月抄到数据补冻为上上月冻结)
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],2);//冻结有效(写冻结时标)
					Count=MRR(ADDR_TCongealCount+28,1);
					if(p32[4]==(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*(Count&0x7f))))
					{//当前冻结地址是计数后最后,计数+1
						PnMonthCongealCount();//测量点的月冻结计数(清0电能表配置变更时的冻结计数值);在冻结后写当前冻结计数前执行免错过计数
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxMonthCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+28,1);
					}
					break;
			}
		}
	}
}


void PointkWhInc_KeyUser(u32 SourAddr,u32 CurrAddr,u32 MapPn,u32 Pn)//重点户冻结区间测量点有功电能量计算,结果值在DATABUFF
{
	u64 d64a;
	u64 d64b;
	u8 * p8;
	//读每次冻结开始时值
	MR(ADDR_DATABUFF,SourAddr+(5*MapPn),5);
	//读当前值
	MR(ADDR_DATABUFF+5,CurrAddr+6+((6+(5*(1+TMaxTariff)))*Pn),5);
	//存当前值为开始时值
	MW(ADDR_DATABUFF+5,SourAddr+(5*MapPn),5);

	p8=(u8 *)(ADDR_DATABUFF);
	if(p8[0]==0xee)
	{
		return;
	}
	if(p8[5]==0xee)
	{
		MC(0xee,ADDR_DATABUFF,5);
		return;
	}
	d64a=MRR(ADDR_DATABUFF,5);//非对齐字节装入到字,入口数据地址,装入字节数
	d64b=MRR(ADDR_DATABUFF+5,5);//非对齐字节装入到字,入口数据地址,装入字节数
	d64a=bcd16_hex(d64a);
	d64b=bcd16_hex(d64b);
	if(d64b<d64a)
	{//可能测量点值回0
		d64b+=10000000000;
	}
	d64b-=d64a;
	d64b=hex_bcd16(d64b);
	MWR(d64b,ADDR_DATABUFF,4);//字存到非对齐字节,入口数据地址,存入字节数
}

void PointkvarhInc_KeyUser(u32 SourAddr,u32 CurrAddr,u32 MapPn,u32 Pn)//重点户冻结区间无功电能量计算,结果值在DATABUFF
{
	u32 i;
	u32 * p32;
	//读每次冻结开始时值
	MR(ADDR_DATABUFF,SourAddr+(4*MapPn),4);
	//读当前值
	MR(ADDR_DATABUFF+4,CurrAddr+6+((6+(4*(1+TMaxTariff)))*Pn),4);
	//存当前值为开始时值
	MW(ADDR_DATABUFF+4,SourAddr+(4*MapPn),4);

	p32=(u32 *)(ADDR_DATABUFF);
	i=0xeeeeeeee;
	if(p32[0]==i)
	{
		return;
	}
	if(p32[1]==i)
	{
		p32[0]=i;
		return;
	}
	p32[0]=bcd_hex(p32[0]);
	p32[1]=bcd_hex(p32[1]);
	if(p32[1]<p32[0])
	{//可能测量点值回0
		p32[1]+=100000000;
	}
	p32[1]-=p32[0];
	p32[1]=hex_bcd(p32[1]);
	p32[0]=p32[1]<<8;//2位小数到4位小数
}

void AFN0C_Full_CurveCongeal_Buff(u32 Addr_CongealBuff)//AFN0C实时数据填入曲线冻结缓冲
{
	u32 Fn;
	u32 Pn;
	u32 Class2Data_MaxFn;
	u32 i;
	u32 MapPn;
	u32 PORT;
	u16 *pn16;
//	u32 *p32;


	PnVSPORTProtocolList(ADDR_PnVSSetList);//测量点号Pn对应AFN04F10配置端口号和通信协议列表,无配置且非脉冲口默认端口号0通信协议为30(DL645-2007)
	pn16=(u16 *)(ADDR_PnVSSetList);
	KeyUserPnList();//集中抄表重点户曲线冻结Pn列表

	
	MC(0xFF,Addr_CongealBuff,5);//冻结时标暂为0XFF
	Class2Data_MaxFn=Get_Class2Data_MaxFn();
	for(Fn=0;Fn<Class2Data_MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==3)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
			{
				if(Terminal_Class2DataLib[Fn].PnType==0x0)
				{//p0
					if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
					{//原始地址=0,无此数据
						MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
				}
				else
				{//pn
					if(Terminal_Class2DataLib[Fn].PnMax!=0x0)
					{
						for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
						{
							switch(Fn)
							{
								case 75://总加组有功电能量曲线
									TotalAddEnergyInc_Computer(ADDR_AFN0DF75_SOUR,ADDR_AFN0CF19,Pn);//冻结区间总加电能量计算,结果值在DATABUFF
									break;
								case 76://总加组无功电能量曲线
									TotalAddEnergyInc_Computer(ADDR_AFN0DF76_SOUR,ADDR_AFN0CF20,Pn);//冻结区间总加电能量计算,结果值在DATABUFF
									break;
								case 97://测量点正向有功总电能量曲线
								#if AFN0DF97MaxPn==0
									continue;
								#else
									MapPn=KeyUserMapPn(Pn+1);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
									if(MapPn!=0)
									{
										MapPn--;
										PointkWhInc_KeyUser(ADDR_AFN0DF97_SOUR,ADDR_AFN0CF129,MapPn,Pn);//重点户冻结区间测量点有功电能量计算,结果值在DATABUFF
									}
									break;
								#endif
								case 98://测量点正向无功总电能量曲线
								#if AFN0DF98MaxPn==0
									continue;
								#else
									MapPn=KeyUserMapPn(Pn+1);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
									if(MapPn!=0)
									{
										MapPn--;
										PointkvarhInc_KeyUser(ADDR_AFN0DF98_SOUR,ADDR_AFN0CF130,MapPn,Pn);//重点户冻结区间无功电能量计算,结果值在DATABUFF
									}
									break;
								#endif
								case 99://测量点反向有功总电能量曲线
								#if AFN0DF99MaxPn==0
									continue;
								#else
									MapPn=KeyUserMapPn(Pn+1);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
									if(MapPn!=0)
									{
										MapPn--;
										PointkWhInc_KeyUser(ADDR_AFN0DF99_SOUR,ADDR_AFN0CF131,MapPn,Pn);//重点户冻结区间测量点有功电能量计算,结果值在DATABUFF
									}
									break;
								#endif
								case 100://测量点反向无功总电能量曲线
								#if AFN0DF100MaxPn==0
									continue;
								#else
									MapPn=KeyUserMapPn(Pn+1);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
									if(MapPn!=0)
									{
										MapPn--;
										PointkvarhInc_KeyUser(ADDR_AFN0DF100_SOUR,ADDR_AFN0CF132,MapPn,Pn);//重点户冻结区间无功电能量计算,结果值在DATABUFF
									}
									break;
								#endif
							}
							if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
							{//原始地址=0,无此数据
								MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								if(Terminal_Class2DataLib[Fn].SourAddr==ADDR_DATABUFF)
								{//原始数据地址=ADDR_DATABUFF
									if(Terminal_Class2DataLib[Fn].PnType!=pnType)
									{//非测量点类
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
									}
									else
									{//测量点类
										MapPn=KeyUserMapPn(Pn+1);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
										if(MapPn!=0)
										{
											MapPn--;
											MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										}
									}
								}
								else
								{//原始数据地址!=ADDR_DATABUFF
									if(Terminal_Class2DataLib[Fn].PnType!=pnType)
									{//非测量点类
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
									}
									else
									{//测量点类
										MapPn=KeyUserMapPn(Pn+1);//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
										if(MapPn!=0)
										{
											MapPn--;
											i=Check_AFN04F11_Pn(Pn+1);//检查终端脉冲配置参数指定的测量点号;返回0无,1有
											if(i!=0)
											{//是脉冲口
												MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
											}
											else
											{//不是脉冲口
												PORT=pn16[Pn+1]&0x1f;//通信端口号
												i=pn16[Pn+1]>>8;//通信协议
											#if (((Project/100)==2)&&(USER==600))//是湖南集中器(RS485的总表需15分实时冻结,益阳当前安装的总表曲线冻结间隔为60分)
												if(PORT!=31)//是内部交流采样(2)或RS485口
											#else
												if((i<30)||(PORT==1))//是内部交流采样(2)或DL/T645-1997(1)
											#endif	
												{
													MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
												}
												else
												{//是DL/T645-2007(30)或串行接口连接窄带低压载波(31)
													if(PORT!=31)
													{//485口
										#if RMM_RS485_IntegralCongeal==0//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线			
														//不补抄整点数据,总是用实时数据冻
														MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
														continue;
										#endif
										#if RMM_RS485_IntegralCongeal==1//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线			
														//补抄整点数据
														switch(Fn)
														{
															default:
																MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
																break;
															case 101://测量点正向有功总电能示值
															case 103://测量点反向有功总电能示值
														#if (USER/100)==11//吉林系
															//case 89://测量点A相电压曲线
														#endif
																//MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].PnLen);
																break;
														}
														continue;
										#endif
										#if RMM_RS485_IntegralCongeal==2//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线			
														//补抄曲线数据
														if(((Fn>=81)&&(Fn<=95))||((Fn>=101)&&(Fn<=108))||((Fn>=145)&&(Fn<=148)))
														{
															//总,A,B,C有功功率;总,A,B,C无功功率;A,B,C相电压;A,B,C相电流
															//正反向有无功总电能示值;A,B,C功率因素
															//4个象限无功电能示值
															//MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].PnLen);
														}
														else
														{
															MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
														}
														continue;
										#endif
													}
													if(PORT==31)
													{//是载波口
										#if RMM_Carrier_IntegralCongeal==0//载波通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
														//不补抄整点数据,总是用实时数据冻
														MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
														continue;
										#endif
										#if RMM_Carrier_IntegralCongeal==1//载波通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
														//补抄整点数据
														switch(Fn)
														{
															default:
																MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
																break;
															case 101://测量点正向有功总电能示值
															case 103://测量点反向有功总电能示值
														#if (USER/100)==11//吉林系
															///case 89://测量点A相电压曲线
														#endif
																//MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].PnLen);
																break;
														}
														continue;
										#endif
										#if RMM_Carrier_IntegralCongeal==2//载波通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
														//补抄曲线数据
														if(((Fn>=81)&&(Fn<=95))||((Fn>=101)&&(Fn<=108))||((Fn>=145)&&(Fn<=148)))
														{
															//总,A,B,C有功功率;总,A,B,C无功功率;A,B,C相电压;A,B,C相电流
															//正反向有无功总电能示值;A,B,C功率因素
															//4个象限无功电能示值
															//MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].PnLen);
														}
														else
														{
															MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
														}
														continue;
										#endif
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


void Terminal_CurveCongeal(void)//曲线冻结
{
	u32 i;
	u8 * p8;
	u32 * p32;
	u64 YMDHM;
	u64 YMDHMsour;
	u32 AddPoint;
//#if(RMM_RS485_IntegralCongeal!=0)//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
	u32 SatrtCongealNo;
	u32 CongealCount;
//#endif

	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
	if(p32[3]!=0x0)
	{//原类2数据冻结AUTOIOW没完成
		return;
	}
	if(Terminal_Ram->CongealFlags&2)//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	{
//	#if(RMM_RS485_IntegralCongeal==0)//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
//		AddPoint=AddPoint;
//		return;
//	#endif
	
#if (RMM_RS485_IntegralCongeal==1)//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
	#if IC_SDRAM
	  //有128KDATABUFF
	  #if ((LEN_CURVECONGEAL*2)>(128*1024))
		//补当前时间到上日0点的x点整点曲线冻结号
		AddPoint=Comm_Ram->TYMDHMS[2];//当前RTC时
		AddPoint=bcd_hex(AddPoint);
		AddPoint+=24+1;//昨天24点,0点算1点
		i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
		if(i>=AddPoint)
		{
			return;
		}
		i*=60;
		YMDHM=MRR(ADDR_TYMDHMS+2,4);
		YMDHM<<=8;
		YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
		CreateCurveCongealNo(YMDHM);//创建曲线冻结号;返回:0=没创建(原已存在),1=创建
		Terminal_Ram->CurveCongealCheckCount++;//曲线冻结检查计数
		return;
	  #else
		//补当前时间到上日0点的x点整点曲线冻结号
		AddPoint=Comm_Ram->TYMDHMS[2];//当前RTC时
		AddPoint=bcd_hex(AddPoint);
 #if (USER/100)==11//吉林系
		AddPoint*=4;
		AddPoint+=96+1;//昨天96点+当前1个整点
 #else
		AddPoint+=24+1;//昨天24点,0点算1点
 #endif
		i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
		if(i>=AddPoint)
		{
			return;
		}
		KeyUserPnList();//重点户曲线冻结Pn列表
		p32=(u32*)(ADDR_128KDATABUFF);
		for(i=0;i<(LEN_128KDATABUFF/4);i++)
		{
			p32[i]=0xffffffff;
		}
		SatrtCongealNo=MRR(ADDR_TCongealCount+7,2);
		if((SatrtCongealNo&0x7fff)>=MaxCurveCongeal)
		{
			MWR(SatrtCongealNo&0x8000,ADDR_TCongealCount+7,2);
		}
		SatrtCongealNo&=0x7fff;//起始冻结号
		CongealCount=0;//连续冻结个数
		while(Terminal_Ram->CurveCongealCheckCount<AddPoint)
		{//曲线冻结检查计数<需补点数
			i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
			Terminal_Ram->CurveCongealCheckCount++;//曲线冻结检查计数
 #if (USER/100)==11//吉林系
			i*=15;
 #else
			i*=60;
 #endif
			YMDHM=MRR(ADDR_TYMDHMS+2,4);
			YMDHM<<=8;
			YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
			i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
			if(i!=0xffffffff)
			{//原有相同时标
				continue;
			}
			MWR(YMDHM,ADDR_128KDATABUFF+(LEN_CURVECONGEAL*CongealCount),5);//冻结有效(冻结时标)
			MR(ADDR_128KDATABUFF+(LEN_CURVECONGEAL*CongealCount)+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
			CongealCount++;//连续冻结个数
			if(((LEN_CURVECONGEAL*(CongealCount+1))>LEN_128KDATABUFF)||((SatrtCongealNo+CongealCount)>=MaxCurveCongeal))
			{//缓冲满或冻结存储回0
				break;
			}
		}
		if(CongealCount!=0)//连续冻结个数
		{
			MW(ADDR_128KDATABUFF,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*SatrtCongealNo),LEN_CURVECONGEAL*CongealCount);
			SatrtCongealNo+=CongealCount;//起始冻结号
			if(SatrtCongealNo>=MaxCurveCongeal)
			{
				SatrtCongealNo=0x8000;
			}
			i=MRR(ADDR_TCongealCount+7,2);
			i&=0x8000;//原满标志
			SatrtCongealNo|=i;
			MWR(SatrtCongealNo,ADDR_TCongealCount+7,2);
		}
		return;
	  #endif
	#else
	 	//无128KDATABUFF(专变)
	  #if ((LEN_CURVECONGEAL*2)>LEN_DATABUFF)
		//补当前时间到上日0点的x点整点曲线冻结号
		AddPoint=Comm_Ram->TYMDHMS[2];//当前RTC时
		AddPoint=bcd_hex(AddPoint);
		AddPoint+=24+1;//昨天24点,0点算1点
		i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
		if(i>=AddPoint)
		{
			return;
		}
		i*=60;
		YMDHM=MRR(ADDR_TYMDHMS+2,4);
		YMDHM<<=8;
		YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
		CreateCurveCongealNo(YMDHM);//创建曲线冻结号;返回:0=没创建(原已存在),1=创建
		Terminal_Ram->CurveCongealCheckCount++;//曲线冻结检查计数
		return;
	  #else
		//补当前时间到上日0点的x点整点曲线冻结号
		AddPoint=Comm_Ram->TYMDHMS[2];//当前RTC时
		AddPoint=bcd_hex(AddPoint);
		AddPoint+=24+1;//昨天24点,0点算1点
		i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
		if(i>=AddPoint)
		{
			return;
		}
		KeyUserPnList();//重点户曲线冻结Pn列表
		p32=(u32*)(ADDR_DATABUFF);
		for(i=0;i<(LEN_DATABUFF/4);i++)
		{
			p32[i]=0xffffffff;
		}
		SatrtCongealNo=MRR(ADDR_TCongealCount+7,2);
		if((SatrtCongealNo&0x7fff)>=MaxCurveCongeal)
		{
			MWR(SatrtCongealNo&0x8000,ADDR_TCongealCount+7,2);
		}
		SatrtCongealNo&=0x7fff;//起始冻结号
		CongealCount=0;//连续冻结个数
		while(Terminal_Ram->CurveCongealCheckCount<AddPoint)
		{//曲线冻结检查计数<需补点数
			i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
			Terminal_Ram->CurveCongealCheckCount++;//曲线冻结检查计数
			i*=60;
			YMDHM=MRR(ADDR_TYMDHMS+2,4);
			YMDHM<<=8;
			YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
			i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
			if(i!=0xffffffff)
			{//原有相同时标
				continue;
			}
			MWR(YMDHM,ADDR_DATABUFF+(LEN_CURVECONGEAL*CongealCount),5);//冻结有效(冻结时标)
			MR(ADDR_DATABUFF+(LEN_CURVECONGEAL*CongealCount)+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
			CongealCount++;//连续冻结个数
			if(((LEN_CURVECONGEAL*(CongealCount+1))>LEN_DATABUFF)||((SatrtCongealNo+CongealCount)>=MaxCurveCongeal))
			{//缓冲满或冻结存储回0
				break;
			}
		}
		if(CongealCount!=0)//连续冻结个数
		{
			MW(ADDR_DATABUFF,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*SatrtCongealNo),LEN_CURVECONGEAL*CongealCount);
			SatrtCongealNo+=CongealCount;//起始冻结号
			if(SatrtCongealNo>=MaxCurveCongeal)
			{
				SatrtCongealNo=0x8000;
			}
			i=MRR(ADDR_TCongealCount+7,2);
			i&=0x8000;//原满标志
			SatrtCongealNo|=i;
			MWR(SatrtCongealNo,ADDR_TCongealCount+7,2);
		}
		return;
	  #endif
	#endif
#endif//#if(RMM_RS485_IntegralCongeal==1)//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
	
#if ((RMM_RS485_IntegralCongeal==0)||(RMM_RS485_IntegralCongeal==2))//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
	#if IC_SDRAM
	  //有128KDATABUFF
	  #if ((LEN_CURVECONGEAL*2)>LEN_128KDATABUFF)
		//补当前时间到上日0点的x点曲线冻结号
		i=Comm_Ram->TYMDHMS[1];//当前RTC分
		i=bcd_hex(i);
		i/=15;
		AddPoint=Comm_Ram->TYMDHMS[2];//当前RTC时
		AddPoint=bcd_hex(AddPoint);
		AddPoint*=4;//每小时4点
		AddPoint+=i;
		AddPoint+=96+1;//昨天96点,0点算1点
		i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
		if(i>=AddPoint)
		{
			return;
		}
		i*=15;
		YMDHM=CurveCongeal_YMDHM();//当前标准曲线冻结时间YMDHM
		YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
		CreateCurveCongealNo(YMDHM);//创建曲线冻结号;返回:0=没创建(原已存在),1=创建
		Terminal_Ram->CurveCongealCheckCount++;//曲线冻结检查计数
		return;
	  #else
		//补当前时间到上日0点的x点曲线冻结号
		i=Comm_Ram->TYMDHMS[1];//当前RTC分
		i=bcd_hex(i);
		i/=15;
		AddPoint=Comm_Ram->TYMDHMS[2];//当前RTC时
		AddPoint=bcd_hex(AddPoint);
		AddPoint*=4;//每小时4点
		AddPoint+=i;
		AddPoint+=96+1;//昨天96点,0点算1点
		i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
		if(i>=AddPoint)
		{
			return;
		}
		KeyUserPnList();//重点户曲线冻结Pn列表
		p32=(u32*)(ADDR_128KDATABUFF);
		for(i=0;i<(LEN_128KDATABUFF/4);i++)
		{
			p32[i]=0xffffffff;
		}
		SatrtCongealNo=MRR(ADDR_TCongealCount+7,2);
		if((SatrtCongealNo&0x7fff)>=MaxCurveCongeal)
		{
			MWR(SatrtCongealNo&0x8000,ADDR_TCongealCount+7,2);
		}
		SatrtCongealNo&=0x7fff;//起始冻结号
		CongealCount=0;//连续冻结个数
		
		while(Terminal_Ram->CurveCongealCheckCount<AddPoint)
		{//曲线冻结检查计数<需补点数
			i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
			Terminal_Ram->CurveCongealCheckCount++;//曲线冻结检查计数
			i*=15;
			YMDHM=CurveCongeal_YMDHM();//当前标准曲线冻结时间YMDHM
			YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
			i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
			if(i!=0xffffffff)
			{//原有相同时标
				continue;
			}
			MWR(YMDHM,ADDR_128KDATABUFF+(LEN_CURVECONGEAL*CongealCount),5);//冻结有效(冻结时标)
			MR(ADDR_128KDATABUFF+(LEN_CURVECONGEAL*CongealCount)+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
			CongealCount++;//连续冻结个数
			if(((LEN_CURVECONGEAL*(CongealCount+1))>LEN_128KDATABUFF)||((SatrtCongealNo+CongealCount)>=MaxCurveCongeal))
			{//缓冲满或冻结存储回0
				break;
			}
		}
		if(CongealCount!=0)//连续冻结个数
		{
			MW(ADDR_128KDATABUFF,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*SatrtCongealNo),LEN_CURVECONGEAL*CongealCount);
			SatrtCongealNo+=CongealCount;//起始冻结号
			if(SatrtCongealNo>=MaxCurveCongeal)
			{
				SatrtCongealNo=0x8000;
			}
			i=MRR(ADDR_TCongealCount+7,2);
			i&=0x8000;//原满标志
			SatrtCongealNo|=i;
			MWR(SatrtCongealNo,ADDR_TCongealCount+7,2);
		}
		return;
	  #endif
	#else
	 	//无128KDATABUFF(专变)
	  #if ((LEN_CURVECONGEAL*2)>LEN_DATABUFF)
		//补当前时间到上日0点的x点曲线冻结号
		i=Comm_Ram->TYMDHMS[1];//当前RTC分
		i=bcd_hex(i);
		i/=15;
		AddPoint=Comm_Ram->TYMDHMS[2];//当前RTC时
		AddPoint=bcd_hex(AddPoint);
		AddPoint*=4;//每小时4点
		AddPoint+=i;
		AddPoint+=1;//0点算1点
		i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
		if(i>=AddPoint)
		{
			return;
		}
		i*=15;
		YMDHM=CurveCongeal_YMDHM();//当前标准曲线冻结时间YMDHM
		YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
		CreateCurveCongealNo(YMDHM);//创建曲线冻结号;返回:0=没创建(原已存在),1=创建
		Terminal_Ram->CurveCongealCheckCount++;//曲线冻结检查计数
		return;
	  #else
		//补当前时间到上日0点的x点曲线冻结号
		i=Comm_Ram->TYMDHMS[1];//当前RTC分
		i=bcd_hex(i);
		i/=15;
		AddPoint=Comm_Ram->TYMDHMS[2];//当前RTC时
		AddPoint=bcd_hex(AddPoint);
		AddPoint*=4;//每小时4点
		AddPoint+=i;
		AddPoint+=96+1;//昨天96点,0点算1点
		i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
		if(i>=AddPoint)
		{
			return;
		}
		KeyUserPnList();//重点户曲线冻结Pn列表
		p32=(u32*)(ADDR_DATABUFF);
		for(i=0;i<(LEN_DATABUFF/4);i++)
		{
			p32[i]=0xffffffff;
		}
		SatrtCongealNo=MRR(ADDR_TCongealCount+7,2);
		if((SatrtCongealNo&0x7fff)>=MaxCurveCongeal)
		{
			MWR(SatrtCongealNo&0x8000,ADDR_TCongealCount+7,2);
		}
		SatrtCongealNo&=0x7fff;//起始冻结号
		CongealCount=0;//连续冻结个数
		while(Terminal_Ram->CurveCongealCheckCount<AddPoint)
		{//曲线冻结检查计数<需补点数
			i=Terminal_Ram->CurveCongealCheckCount;//曲线冻结检查计数
			Terminal_Ram->CurveCongealCheckCount++;//曲线冻结检查计数
			i*=15;
			YMDHM=CurveCongeal_YMDHM();//当前标准曲线冻结时间YMDHM
			YMDHM=YMDHM_SubM(YMDHM,i);//年月日时分减m分,返回减m分后的年月日时分
			i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
			if(i!=0xffffffff)
			{//原有相同时标
				continue;
			}
			MWR(YMDHM,ADDR_DATABUFF+(LEN_CURVECONGEAL*CongealCount),5);//冻结有效(冻结时标)
			MR(ADDR_DATABUFF+(LEN_CURVECONGEAL*CongealCount)+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
			CongealCount++;//连续冻结个数
			if(((LEN_CURVECONGEAL*(CongealCount+1))>LEN_DATABUFF)||((SatrtCongealNo+CongealCount)>=MaxCurveCongeal))
			{//缓冲满或冻结存储回0
				break;
			}
		}
		if(CongealCount!=0)//连续冻结个数
		{
			MW(ADDR_DATABUFF,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*SatrtCongealNo),LEN_CURVECONGEAL*CongealCount);
			SatrtCongealNo+=CongealCount;//起始冻结号
			if(SatrtCongealNo>=MaxCurveCongeal)
			{
				SatrtCongealNo=0x8000;
			}
			i=MRR(ADDR_TCongealCount+7,2);
			i&=0x8000;//原满标志
			SatrtCongealNo|=i;
			MWR(SatrtCongealNo,ADDR_TCongealCount+7,2);
		}
		return;
	  #endif
	#endif//#if IC_SDRAM
#endif//#if(RMM_RS485_IntegralCongeal==2)//RS485通道抄表抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
	}
	Terminal_Ram->CongealFlags|=0x2;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	if(Terminal_Ram->CurveCongealTask==0)//曲线冻结任务0-24(当点和上1-24点)
	{
		YMDHMsour=MRR(ADDR_TCongealCount+9,5);
		YMDHM=CurveCongeal_YMDHM();//当前标准曲线冻结时间YMDHM
		if(YMDHM==YMDHMsour)
		{//已冻结
			return;
		}
	}
	Terminal_Ram->CurveCongealCheckCount=0;//曲线冻结检查计数
	//冻结
	if(Terminal_Ram->CurveCongealTask>24)//曲线冻结任务0-24(当点和上1-24点)
	{
		Terminal_Ram->CurveCongealTask=0;//曲线冻结任务0-24(当点和上1-24点)
		Terminal_Ram->CongealFlags|=2;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
		return;
	}
	Terminal_Ram->CongealFlags&=0xfd;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	if(Terminal_Ram->CurveCongealTask==0)//曲线冻结任务0-24(当点和上1-24点)
	{//当前点冻结
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_CURVECONGEAL);
		MC(0xee,ADDR_AFN0D_Congeal_Buff_Start+ADDR_CURVECONGEALFLAG,1);//冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
		AFN0C_Full_CurveCongeal_Buff(ADDR_AFN0D_Congeal_Buff_Start);//AFN0C实时数据填入曲线冻结缓冲
		i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
		if(i==0xffffffff)
		{//原冻结没相同时标
			MR(ADDR_DATABUFF,ADDR_TCongealCount+7,2);
			p8=(u8 *)(ADDR_DATABUFF);
			i=(p8[0]+(p8[1]<<8))&0x7fff;
			if(i>=MaxCurveCongeal)
			{
				i=(p8[0]+(p8[1]<<8))&0x8000;
				MWR(i,ADDR_TCongealCount+7,2);
				i=0;
			}
		}
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
		p32[0]=2;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i);
		p32[3]=LEN_CURVECONGEAL;
		p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
		MWR(YMDHM,ADDR_Class2Congeal_AutoRW+20,5);//冻结有效(冻结时标)
		//冻结后数据处理(电能量原始数据在计算时已存入)

		return;
	}

	Terminal_Ram->CurveCongealTask=100;//曲线冻结任务0-24(当时,上1-24时)
}


void AFN0C_Full_DateCongeal_Buff(u32 Addr_CongealBuff)//AFN0C实时数据填入日冻结缓冲
{
	u32 Fn;
	u32 Pn;
	u32 Class2Data_MaxFn;
/*
	u16 *pn16;
	PnVSSetnoList(ADDR_PnVSSetList);//测量点号Pn对应AFN04F10配置序号列表
	pn16=(u16 *)(ADDR_PnVSSetList);
*/
	MC(0xFF,Addr_CongealBuff,3);//冻结时标暂为0XFF
	Class2Data_MaxFn=Get_Class2Data_MaxFn();
	for(Fn=0;Fn<Class2Data_MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==0)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
			{
				if(Terminal_Class2DataLib[Fn].PnType==0x0)
				{//p0
					if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
					{//原始地址=0,无此数据
						MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
				}
				else
				{//pn
					if(Terminal_Class2DataLib[Fn].PnMax!=0x0)
					{
						if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
						{//原始地址=0,无此数据
							MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							switch(Fn)
							{//暂无下列统计数据
								case 121://F121 A相谐波越限日统计数据
								case 122://F122 B相谐波越限日统计数据
								case 123://F123 C相谐波越限日统计数据
									MC(0xee,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
									for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
									{
										MC(19,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),1);
									}
									break;
							}
						}
						else
						{
							for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
							{
								switch(Fn)
								{
								#if (USER/100)==11//吉林
									case 27://测量点日电压统计数据
										//吉林电压统计数据除交采测量点外用读取的电表数据
										if(Pn<AFN0DF27MaxPn_ReadMeter)
										{
											if(Check_ACSamplePn(Pn+1))//有效交采测量点号检查;返回0=无效,1=有效
											{
												MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_AFN0DF27_SOUR+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
												break;
											}
										}
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										break;
								#endif
									case 153://F153 日冻结分相正向有功电能示值
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//终端抄表时间
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5,5);//A相正向有功电能示值
										MR(ADDR_DATABUFF+10,ADDR_AFN0CF31+(59*Pn)+5+18,5);//B相正向有功电能示值
										MR(ADDR_DATABUFF+15,ADDR_AFN0CF31+(59*Pn)+5+18+18,5);//C相正向有功电能示值
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 154://F154 日冻结分相正向无功电能示值
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//终端抄表时间
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+10,4);//A相正向无功电能示值
										MR(ADDR_DATABUFF+9,ADDR_AFN0CF31+(59*Pn)+5+18+10,4);//B相正向无功电能示值
										MR(ADDR_DATABUFF+13,ADDR_AFN0CF31+(59*Pn)+5+18+18+10,4);//C相正向无功电能示值
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 155://F155 日冻结分相反向有功电能示值
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//终端抄表时间
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+5,5);//A相反向有功电能示值
										MR(ADDR_DATABUFF+10,ADDR_AFN0CF31+(59*Pn)+5+18+5,5);//B相反向有功电能示值
										MR(ADDR_DATABUFF+15,ADDR_AFN0CF31+(59*Pn)+5+18+18+5,5);//C相反向有功电能示值
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 156://F156 日冻结分相反向无功电能示值
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//终端抄表时间
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+14,4);//A相反向无功电能示值
										MR(ADDR_DATABUFF+9,ADDR_AFN0CF31+(59*Pn)+5+18+14,4);//B相反向无功电能示值
										MR(ADDR_DATABUFF+13,ADDR_AFN0CF31+(59*Pn)+5+18+18+14,4);//C相反向无功电能示值
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									default:
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										break;
								}
							}
						}
					}
				}
			}
		}
	}	
}

void Terminal_DateCongeal(void)//日冻结
{
	u32 i;
	u32 Fn;
	u8 * p8;
	u32 * p32;
	u32 x;
	#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	u32 y;
	u32 CongealFlag;//冻结标志:0=没,1=原已有
	#endif

	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
	if(p32[3]!=0x0)
	{//原类2数据冻结AUTOIOW没完成
		return;
	}
	if(Terminal_Ram->CongealFlags&4)
	{
	#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
		return;
	#endif
	#if RMM_DateCongeal_0xEE==1//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
		//补日冻结号
		i=Terminal_Ram->DateCongealCheckCount;//日冻冻结检查计数
		if(i>=3)
		{
			return;
		}
		x=MRR(ADDR_TYMDHMS+3,3);//当前年月日
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		for(;i!=0;i--)
		{
			x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		}
		CreateDateCongealNo(x);//创建日冻结号;返回:返回:原已存在或新创建的冻结号
		Terminal_Ram->DateCongealCheckCount++;//日冻冻结检查计数
		return;
	#endif
	}
	Terminal_Ram->CongealFlags|=0x4;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	i=MRR(ADDR_TCongealCount+16,3);
	if(MRR(ADDR_TYMDHMS+3,3)==i)
	{//已冻结
		return;
	}
	Terminal_Ram->DateCongealCheckCount=0;//日冻冻结检查计数
	//冻结
	Terminal_Ram->CongealFlags&=0xfb;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	if((Terminal_Ram->CongealFlags&0x3)!=0x3)
	{//小时冻结和曲线冻结没完成
		return;
	}
	
	#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	//日冻结更新(上日抄表数据补冻为上上日冻结)
	i=MRR(ADDR_TYMDHMS+3,3);
	x=MRR(ADDR_TCongealCount+32,3);//日冻结更新的年月日(上日抄表数据补冻为上上日冻结)
	if(i!=x)
	{
		MWR(i,ADDR_TCongealCount+32,3);//日冻结更新的年月日(上日抄表数据补冻为上上日冻结)
		i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
		i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
		i=GetDateCongealNo(i);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
		if((i!=0xffffffff)&&(i<MaxDateCongeal))
		{//有上上日冻结
			MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//读原上上日冻结数据
		}
		else
		{
			MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//清冻结缓冲
			i=MRR(ADDR_TCongealCount+14,1);
			i&=0x7f;
			if(i>=MaxDateCongeal)
			{
				i=MRR(ADDR_TCongealCount+14,1);
				i&=0x80;
				MWR(i,ADDR_TCongealCount+14,1);
				i=0;
			}
		}
		x=MRR(ADDR_TYMDHMS+3,3);//有效抄表时间
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,0,0,x,0);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee

		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,3);//冻结时标暂无效
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
		p32[0]=0x40002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
		p32[3]=LEN_DATECONGEAL;
		p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
		i=MRR(ADDR_TYMDHMS+3,3);
		i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
		i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
		MWR(i,ADDR_Class2Congeal_AutoRW+20,3);//冻结有效(冻结时标)
		return;
	}
	//补抄上2日冻结
	x=MRR(ADDR_TYMDHMS+3,3);
	i=MRR(ADDR_TCongealCount+35,3);//日冻结更新的年月日
	if(i!=x)
	{
		MWR(x,ADDR_TCongealCount+35,3);//日冻结更新的年月日
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		y=x;//电表冻结的时标
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		i=GetDateCongealNo(x);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
		if((i!=0xffffffff)&&(i<MaxDateCongeal))
		{//有上2日冻结,且冻结号有效
			CongealFlag=1;//冻结标志:0=没,1=原已有
			MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//读原冻结数据
		}
		else
		{//没冻结号或无效
			CongealFlag=0;//冻结标志:0=没,1=原已有
			MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//清冻结缓冲
			i=MRR(ADDR_TCongealCount+14,1);
			i&=0x7f;
			if(i>=MaxDateCongeal)
			{
				i=MRR(ADDR_TCongealCount+14,1);
				i&=0x80;
				MWR(i,ADDR_TCongealCount+14,1);
				i=0;
			}
		}
		x=LastSource_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,y);//上2-3日抄表原始数据填入冻结缓冲;入口:LastYMD=电表冻结时标;返回:0=没补抄填入,1=有
		if((CongealFlag==0)||(x!=0))//冻结标志:0=没,1=原已有
		{
			MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,3);//冻结时标暂无效
			p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
			p32[0]=0x50002;
			p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
			p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
			p32[3]=LEN_DATECONGEAL;
			p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
			i=MRR(ADDR_TYMDHMS+3,3);
			i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
			i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
			i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
			MWR(i,ADDR_Class2Congeal_AutoRW+20,3);//冻结有效(冻结时标)
			return;
		}
	}
	//补抄上3日冻结
	x=MRR(ADDR_TYMDHMS+3,3);
	i=MRR(ADDR_TCongealCount+38,3);//日冻结更新的年月日
	if(i!=x)
	{
		MWR(x,ADDR_TCongealCount+38,3);//日冻结更新的年月日
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		y=x;//电表冻结的时标
		x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
		i=GetDateCongealNo(x);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
		if((i!=0xffffffff)&&(i<MaxDateCongeal))
		{//有上3日冻结,且冻结号有效
			CongealFlag=1;//冻结标志:0=没,1=原已有
			MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//读原冻结数据
		}
		else
		{//没冻结号或无效
			CongealFlag=0;//冻结标志:0=没,1=原已有
			MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//清冻结缓冲
			i=MRR(ADDR_TCongealCount+14,1);
			i&=0x7f;
			if(i>=MaxDateCongeal)
			{
				i=MRR(ADDR_TCongealCount+14,1);
				i&=0x80;
				MWR(i,ADDR_TCongealCount+14,1);
				i=0;
			}
		}
		x=LastSource_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,y);//上2-3日抄表原始数据填入冻结缓冲;入口:LastYMD=电表冻结时标;返回:0=没补抄填入,1=有
		if((CongealFlag==0)||(x!=0))//冻结标志:0=没,1=原已有
		{
			MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,3);//冻结时标暂无效
			p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
			p32[0]=0x60002;
			p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
			p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
			p32[3]=LEN_DATECONGEAL;
			p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
			i=MRR(ADDR_TYMDHMS+3,3);
			i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
			i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
			i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
			i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
			MWR(i,ADDR_Class2Congeal_AutoRW+20,3);//冻结有效(冻结时标)
			return;
		}
	}
	#endif
	
	//上日冻结
	MC(0xee,ADDR_AFN0D_Congeal_Buff_Start+ADDR_DATECONGEALFLAG,1);//冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
	AFN0C_Full_DateCongeal_Buff(ADDR_AFN0D_Congeal_Buff_Start);//AFN0C实时数据填入日冻结缓冲
	x=MRR(ADDR_TYMDHMS+3,3);//有效抄表时间
	#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
	Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,0,0,0,0);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
	#else
	Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,0,0,x,1);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
	#endif

	i=MRR(ADDR_TYMDHMS+3,3);
	i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
	i=GetDateCongealNo(i);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{//原冻结没相同时标
		MR(ADDR_DATABUFF,ADDR_TCongealCount+14,1);
		p8=(u8 *)(ADDR_DATABUFF);
		i=p8[0]&0x7f;
		if(i>=MaxDateCongeal)
		{
			i=p8[0]&0x80;
			MWR(i,ADDR_TCongealCount+14,1);
			i=0;
		}
	}
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
	p32[0]=0x10002;
	p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
	p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
	p32[3]=LEN_DATECONGEAL;
	p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
	i=MRR(ADDR_TYMDHMS+3,3);
	i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
	MWR(i,ADDR_Class2Congeal_AutoRW+20,3);//冻结有效(冻结时标)

	//冻结后数据处理
	for(Fn=0;Fn<Get_Class2Data_MaxFn();Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==0)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
			{
				switch(Terminal_Class2DataLib[Fn].SourInit)//冻结后原始数据处理:0=不变,1=清为0,2=清为0xee,3=置初值
				{
					case 0://0=不变
						break;
					case 1://1=清为0
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//原始数据地址有效
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
							#if AFN0DF5MaxPn==0
								switch(Fn)
								{
									case 5://正向有功电能量（总、费率1～M）
									case 6://正向无功电能量（总、费率1～M）
									case 7://反向有功电能量（总、费率1～M）
									case 8://反向无功电能量（总、费率1～M）
										MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*AFN0CF41MaxPn);
										break;
									default:
										MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
										break;
								}
							#else
								MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							#endif
							}
						}
						break;
					case 2://2=清为0xee
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//原始数据地址有效
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0xee,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								MC(0xee,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							}
						}
						break;
					case 3://3=置初值
						break;
					case 4://4=清为0xff
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//原始数据地址有效
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0xff,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								MC(0xff,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							}
						}
						break;
				}
			}
		}
	}
	//同时清0小时冻结电能量计算用原始数据
	MC(0x0,ADDR_AFN0CF83_SOUR,4*AFN0CF83MaxPn);
	MC(0x0,ADDR_AFN0CF84_SOUR,4*AFN0CF84MaxPn);
	//同时清0曲线冻结电能量计算用原始数据
	MC(0x0,ADDR_AFN0DF75_SOUR,4*AFN0DF75MaxPn);
	MC(0x0,ADDR_AFN0DF76_SOUR,4*AFN0DF76MaxPn);
	
#if IC_SDRAM
	//上次RAM保存到FLASH的实时钟时(上电清为0,日月冻结后清为0xff使立即进行保存)
	MC(0xff,ADDR_HOURFLASH_RAM_START,1);
	//清0日冻结A 相电压谐波电压含有率、谐波电流95%概率值全部原始数据
	MC(0,ADDR_AFN0DF124_SOUR,LEN_AFN0DF124_SOUR*AFN0DF124MaxPn);
	//清0日冻结B 相电压谐波电压含有率、谐波电流95%概率值全部原始数据
	MC(0,ADDR_AFN0DF125_SOUR,LEN_AFN0DF125_SOUR*AFN0DF125MaxPn);
	//清0日冻结C 相电压谐波电压含有率、谐波电流95%概率值全部原始数据
	MC(0,ADDR_AFN0DF126_SOUR,LEN_AFN0DF126_SOUR*AFN0DF126MaxPn);
	//清0日冻结三相电压、电流不平衡度95%概率值全部原始数据
	MC(0,ADDR_AFN0DF211_SOUR,LEN_AFN0DF211_SOUR*AFN0DF211MaxPn);
#endif

#if (USER/100)==5//用户标识:上海系
	MC(0xff,ADDR_AFN0CF242_Next,6*AFN0CF242MaxCount);//当日登录主站信息
	MC(0xff,ADDR_AFN0CF243,24);//当日信号强度
#endif
#if (USER/100)==6//用户标识:湖南系
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		//过零点清除AFN0CF25,F26
		if(Comm_Ram->TYMDHMS[3]!=0x01)
		{//每月1日在月冻结后清
			MC(0xEE,ADDR_AFN0CF25,67*AFN0CF25MaxPn);
			MC(0xEE,ADDR_AFN0CF26,61*AFN0CF26MaxPn);
		}
	#endif
#endif
#if (USER/100)==11//吉林
	//清0掉电保存中的日电压统计,因库中已改指向RAM
	MC(0,ADDR_AFN0DF27_SOUR,(66+27)*AFN0DF27MaxPn);
#endif
}

void AFN0C_Full_RMDateCongeal_Buff(u32 Addr_CongealBuff,u32 ADDR_CongealFlags)//AFN0C实时数据填入抄表日冻结缓冲;入口:测量点分端口号冻结标志已计算
{
	u32 Fn;
	u32 Pn;
	u32 MaxFn;
	u32 i;
	u8 *p8f;
	
	p8f=(u8*)ADDR_CongealFlags;//测量点分端口需冻结标志
	MC(0xff,Addr_CongealBuff,3);//冻结时标暂为0xff
	MaxFn=Get_Class2Data_MaxFn();
	for(Fn=0;Fn<MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==1)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
			{
				if(Terminal_Class2DataLib[Fn].PnType==0x0)
				{//P0
					if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
					{//原始地址=0,无此数据
						MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
				}
				else
				{//Pn
					if(Terminal_Class2DataLib[Fn].PnMax!=0x0)
					{
						if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
						{//原始地址=0,无此数据
							MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
						}
						else
						{
							for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
							{
								if(Terminal_Class2DataLib[Fn].PnType==pnType)//测量点号类
								{
									i=p8f[Pn/8];
									i&=1<<(Pn%8);
									if(i==0)
									{//分端口本测量点冻结标志=0
										continue;
									}
								}
							#if RMDateCongealSour==0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
								MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
							#else
								MC(0xff,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
							#endif
							}
						}
					}
				}
			}
		}
	}
}

void RMDateCongealFlags(u32 PORTf,u32 ADDR_CongealFlags)//抄表日冻结各测量点号分端口冻结标志;入口:端口标志B1-B31分别表示端口1-端口31,存放缓冲;出口:B0-Bn分别表示测量点号1-(n+1),位0=不冻结,位1=冻结
{
	u32 i;
	u32 Meter;
	u32 Pn;
	u8 *p8d;
	u32 Addr;

	p8d=(u8*)(ADDR_CongealFlags);
	MC(0xff,ADDR_CongealFlags,(AFN04F10MaxPn+7)/8);//初始为全部点冻结
	#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
		#error
	#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	Addr=ADDR_128KDATABUFF+2;
	//Addr=ADDR_AFN04F10+2;
	for(Meter=0;Meter<AFN04F10MaxPn;Meter++)
	{
		Pn=MRR(Addr,2);
		if((Pn<=AFN04F10MaxPn)&&(Pn!=0))
		{//Pn有效
			Pn--;
			i=MRR(Addr+2,1);
			i&=0x1f;
			i=1<<i;
			i&=PORTf;
			if(i==0)
			{//本端口不冻结
				i=1<<(Pn%8);
				i=~i;
				p8d[Pn/8]&=i;
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
}

void Terminal_RMDateCongeal(void)//抄表日冻结
{
	u32 i;
	u32 x;
	u32 hm;
	u32 d;
	u32 PORT;
	u8 * p8;
	u32 * p32;
	
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
	if(p32[3]!=0x0)
	{//原类2数据冻结AUTOIOW没完成
		return;
	}
	if(Terminal_Ram->CongealFlags&8)
	{
		return;
	}
	Terminal_Ram->CongealFlags|=0x8;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结

//按抄表日和时分冻结
	PORT=0;//冻结端口标志B1-B31分别表示端口1-端口31
	p8=(u8 *)(ADDR_TYMDHMS);
	d=p8[3];
	d=bcd_hex(d);
	d=1<<d;
	hm=p8[1]+(p8[2]<<8);//当前时分
	//通信端口1 交流采样接口通信
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*0),4);
	x<<=1;
	if(d&x)
	{//抄表日
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*0),2);
		if(hm==x)
		{//到抄表日时分
			PORT|=1<<1;
		}
	}
	//通信端口2 RS485-1
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*1),4);
	x<<=1;
	if(d&x)
	{//抄表日
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*1),2);
		if(hm==x)
		{//到抄表日时分
			PORT|=1<<2;
		}
	}
	//通信端口3 RS485-2
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*2),4);
	x<<=1;
	if(d&x)
	{//抄表日
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*2),2);
		if(hm==x)
		{//到抄表日时分
			PORT|=1<<3;
		}
	}
	//通信端口4 RS485-3
#if (Project/100)==5//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*3),4);
	x<<=1;
	if(d&x)
	{//抄表日
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*3),2);
		if(hm==x)
		{//到抄表日时分
			PORT|=1<<4;
		}
	}
#endif
	//通信端口31 载波抄表
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*30),4);
	x<<=1;
	if(d&x)
	{//抄表日
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*30),2);
		if(hm==x)
		{//到抄表日时分
			PORT|=0x80000000;
		}
	}
#endif
	if(PORT==0x0)
	{//没端口要抄表日冻结
		return;
	}

	if(MRR(ADDR_TYMDHMS+1,5)==MRR(ADDR_TCongealCount+23,5))
	{//已冻结
		return;
	}
	//冻结
//Congeal:
	Terminal_Ram->CongealFlags&=0xf7;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	i=GetRMDateCongealNo(MRR(ADDR_TYMDHMS+3,3));//得到抄表日冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{//原冻结没相同时标
		MR(ADDR_DATABUFF,ADDR_TCongealCount+21,1);
		p8=(u8 *)(ADDR_DATABUFF);
		i=p8[0]&0x7f;
		if(i>=MaxRMDateCongeal)
		{
			i=p8[0]&0x80;
			MWR(i,ADDR_TCongealCount+21,1);
			i=0;
		}
	#if RMDateCongealSour==0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
		//清缓冲为0xee
		MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_RMDATECONGEAL);
	#else
		//清缓冲为0xff
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_RMDATECONGEAL);
	#endif
	}
	else
	{//原冻结有相同时标
		//读原冻结到缓冲
		MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),LEN_RMDATECONGEAL);
	}
	
	RMDateCongealFlags(PORT,ADDR_DATABUFF);//抄表日冻结各测量点号分端口冻结标志;入口:端口标志B1-B31分别表示端口1-端口31,存放缓冲;出口:B0-Bn分别表示测量点号1-(n+1),位0=不冻结,位1=冻结
#if RMDateCongealSour==0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值
	MC(0xee,ADDR_AFN0D_Congeal_Buff_Start+ADDR_RMDATECONGEALFLAG,1);//冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
#else
	MC(0xff,ADDR_AFN0D_Congeal_Buff_Start+ADDR_RMDATECONGEALFLAG,1);//冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
#endif
	AFN0C_Full_RMDateCongeal_Buff(ADDR_AFN0D_Congeal_Buff_Start,ADDR_DATABUFF);//AFN0C实时数据填入抄表日冻结缓冲;入口:测量点分端口号冻结标志已计算
//	//x=MRR(ADDR_TYMDHMS+3,3);//有效抄表时间
//	//Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,1,PORT,0,0);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee

	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
	p32[0]=0x20002;
	p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
	p32[2]=ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i);
	p32[3]=LEN_RMDATECONGEAL;
	p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
	MW(ADDR_TYMDHMS+3,ADDR_Class2Congeal_AutoRW+20,3);//冻结有效(冻结时标)
	//冻结后数据处理

}

void AFN0C_Full_MonthCongeal_Buff(u32 Addr_CongealBuff)//AFN0C实时数据填入月冻结缓冲
{
	u32 Fn;
	u32 Pn;
	u32 Class2Data_MaxFn;
/*
	u16 *pn16;
	PnVSSetnoList(ADDR_PnVSSetList);//测量点号Pn对应AFN04F10配置序号列表
	pn16=(u16 *)(ADDR_PnVSSetList);
*/
	MC(0xff,Addr_CongealBuff,2);//冻结时标暂为0xff
	Class2Data_MaxFn=Get_Class2Data_MaxFn();
	for(Fn=0;Fn<Class2Data_MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==2)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
			{
				if(Terminal_Class2DataLib[Fn].PnType==0x0)
				{//p0
					if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
					{//原始地址=0,无此数据
						MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
				}
				else
				{//pn
					if(Terminal_Class2DataLib[Fn].PnMax!=0x0)
					{
						if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
						{//原始地址=0,无此数据
							MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
						}
						else
						{
							for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
							{
								switch(Fn)
								{
								#if (USER/100)==11//吉林
									case 35://测量点月电压统计数据
										//吉林电压统计数据除交采测量点外用读取的电表数据
										if(Pn<AFN0DF35MaxPn)
										{
											if(Check_ACSamplePn(Pn+1))//有效交采测量点号检查;返回0=无效,1=有效
											{
												MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_AFN0DF35_SOUR+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
												break;
											}
										}
//										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										break;
								#endif
									case 157://F157 月冻结分相正向有功电能示值
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//终端抄表时间
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5,5);//A相正向有功电能示值
										MR(ADDR_DATABUFF+10,ADDR_AFN0CF31+(59*Pn)+5+18,5);//B相正向有功电能示值
										MR(ADDR_DATABUFF+15,ADDR_AFN0CF31+(59*Pn)+5+18+18,5);//C相正向有功电能示值
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 158://F158 月冻结分相正向无功电能示值
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//终端抄表时间
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+10,4);//A相正向无功电能示值
										MR(ADDR_DATABUFF+9,ADDR_AFN0CF31+(59*Pn)+5+18+10,4);//B相正向无功电能示值
										MR(ADDR_DATABUFF+13,ADDR_AFN0CF31+(59*Pn)+5+18+18+10,4);//C相正向无功电能示值
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 159://F159 月冻结分相反向有功电能示值
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//终端抄表时间
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+5,5);//A相反向有功电能示值
										MR(ADDR_DATABUFF+10,ADDR_AFN0CF31+(59*Pn)+5+18+5,5);//B相反向有功电能示值
										MR(ADDR_DATABUFF+15,ADDR_AFN0CF31+(59*Pn)+5+18+18+5,5);//C相反向有功电能示值
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 160://F160 月冻结分相反向无功电能示值
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//终端抄表时间
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+14,4);//A相反向无功电能示值
										MR(ADDR_DATABUFF+9,ADDR_AFN0CF31+(59*Pn)+5+18+14,4);//B相反向无功电能示值
										MR(ADDR_DATABUFF+13,ADDR_AFN0CF31+(59*Pn)+5+18+18+14,4);//C相反向无功电能示值
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 216://F216 测量点电能表结算信息,比原始数据AFN0CF168多了5字节故障电量
										MC(0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].SourPnLen);
										break;
									default:
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										break;
								}
							}
						}
					}	
				}
			}
		}
	}
}

void Terminal_MonthCongeal(void)//月冻结
{
	u32 i;
	u32 Fn;
	u8 * p8;
	u32 * p32;
	
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
	if(p32[3]!=0x0)
	{//原类2数据冻结AUTOIOW没完成
		return;
	}
	if(Terminal_Ram->CongealFlags&0x10)
	{
	#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
		return;
	#endif
	#if RMM_MonthCongeal_0xEE==1//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
		//补月冻结号
		i=Terminal_Ram->MonthCongealCheckCount;//月冻冻结检查计数
		if(i>=2)
		{
			return;
		}
		Fn=MRR(ADDR_TYMDHMS+4,2);//当前年月日
		Fn=YM_Sub1M(Fn);//年月减1月,返回减1后的年月
		for(;i!=0;i--)
		{
			Fn=YM_Sub1M(Fn);//年月减1月,返回减1后的年月
		}
		CreateMonthCongealNo(Fn);//创建月冻结号;返回:返回:原已存在或新创建的冻结号
		Terminal_Ram->MonthCongealCheckCount++;//月冻冻结检查计数
		return;
	#endif
	}
	Terminal_Ram->CongealFlags|=0x10;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	i=MRR(ADDR_TCongealCount+30,2);
	if(MRR(ADDR_TYMDHMS+4,2)==i)
	{//已冻结
		return;
	}
	Terminal_Ram->MonthCongealCheckCount=0;//月冻冻结检查计数
	Terminal_Ram->CongealFlags&=~0x10;//B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
	if((Terminal_Ram->CongealFlags&0x7)!=0x7)
	{//小时冻结,曲线冻结,日冻结没完成
		return;
	}
	
	#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
	//月冻结更新(上月抄表数据补冻为上上月冻结)
	i=MRR(ADDR_TYMDHMS+4,2);
	Fn=MRR(ADDR_TCongealCount+19,2);//上月抄表数据补冻为上上月冻结的年月
	if(i!=Fn)
	{
		MWR(i,ADDR_TCongealCount+19,2);//上月抄表数据补冻为上上月冻结的年月
		i=YM_Sub1M(i);//年月减1月,返回减1后的年月
		i=YM_Sub1M(i);//年月减1月,返回减1后的年月
		i=GetMonthCongealNo(i);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
		if((i!=0xffffffff)&&(i<MaxMonthCongeal))
		{//有上上月冻结
			MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),LEN_MONTHCONGEAL);//读原上上月冻结数据
		}
		else
		{
			MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_MONTHCONGEAL);//清冻结缓冲
			i=MRR(ADDR_TCongealCount+28,1);
			i&=0x7f;
			if(i>=MaxMonthCongeal)
			{
				i=MRR(ADDR_TCongealCount+28,1);
				i&=0x80;
				MWR(i,ADDR_TCongealCount+28,1);
				i=0;
			}
		}
		Fn=MRR(ADDR_TYMDHMS+4,2);//有效抄表时间
		Fn=YM_Sub1M(Fn);//年月减1月,返回减1后的年月
		Fn<<=8;
		Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,2,0,Fn,0);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
		
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,2);//冻结时标暂无效
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
		p32[0]=0x70002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i);
		p32[3]=LEN_MONTHCONGEAL;
		p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
		i=MRR(ADDR_TYMDHMS+4,2);
		i=YM_Sub1M(i);//年月减1月,返回减1后的年月
		i=YM_Sub1M(i);//年月减1月,返回减1后的年月
		MWR(i,ADDR_Class2Congeal_AutoRW+20,2);//冻结有效(冻结时标)
		return;
	}
	#endif

	//上月冻结
	MC(0xee,ADDR_AFN0D_Congeal_Buff_Start+ADDR_MONTHCONGEALFLAG,1);//冻结标志:0xEE正常冻结,0xFF补冻结(只有电能表测量点类的数据会补,其他数据为无效,用于非BCD的数据也标识为无效)
	AFN0C_Full_MonthCongeal_Buff(ADDR_AFN0D_Congeal_Buff_Start);//AFN0C实时数据填入月冻结缓冲
	i=MRR(ADDR_TYMDHMS+4,2);//有效抄表时间
	i=YM_Sub1M(i);//年月减1月,返回减1后的年月
	i<<=8;
	#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
	Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,2,0,0,0);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
	#else
	Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,2,0,i,1);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
	#endif


	i=MRR(ADDR_TYMDHMS+4,2);
	i=YM_Sub1M(i);//年月减1月,返回减1后的年月
	i=GetMonthCongealNo(i);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
	if(i==0xffffffff)
	{//原冻结没相同时标
		MR(ADDR_DATABUFF,ADDR_TCongealCount+28,1);
		p8=(u8 *)(ADDR_DATABUFF);
		i=p8[0]&0x7f;
		if(i>=MaxMonthCongeal)
		{
			i=p8[0]&0x80;
			MWR(i,ADDR_TCongealCount+28,1);
			i=0;
		}
	}
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//类2数据冻结自动写EFLASH控制地址(字对齐)
	p32[0]=0x30002;
	p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
	p32[2]=ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i);
	p32[3]=LEN_MONTHCONGEAL;
	p32[4]=p32[2];//冻结开始地址(时标地址),同初始值R1
	i=MRR(ADDR_TYMDHMS+4,2);
	i=YM_Sub1M(i);//年月减1月,返回减1后的年月
	MWR(i,ADDR_Class2Congeal_AutoRW+20,2);//冻结有效(冻结时标)

	//冻结后数据处理
	for(Fn=0;Fn<Get_Class2Data_MaxFn();Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==2)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
			{
				switch(Terminal_Class2DataLib[Fn].SourInit)//冻结后原始数据处理:0=不变,1=清为0,2=清为0xee,3=置初值
				{
					case 0://0=不变
						break;
					case 1://1=清为0
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//原始数据地址有效
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
							#if AFN0DF21MaxPn==0
								switch(Fn)
								{
									case 21://正向有功电能量（总、费率1～M）
									case 22://正向无功电能量（总、费率1～M）
									case 23://反向有功电能量（总、费率1～M）
									case 24://反向无功电能量（总、费率1～M）
										MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*AFN0CF45MaxPn);
										break;
									default:
										MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
										break;
								}
							#else
								MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							#endif
							}
						}
						break;
					case 2://2=清为0xee
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//原始数据地址有效
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0xee,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								MC(0xee,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							}
						}
						break;
					case 3://3=置初值
						break;
					case 4://2=清为0xff
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//原始数据地址有效
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0xff,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								MC(0xff,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							}
						}
						break;
				}
			}
		}
	}
	
#if IC_SDRAM
	//上次RAM保存到FLASH的实时钟时(上电清为0,日月冻结后清为0xff使立即进行保存)
	MC(0xff,ADDR_HOURFLASH_RAM_START,1);
	//清0月冻结三相电压、电流不平衡度95%概率值全部原始数据
	MC(0,ADDR_AFN0DF212_SOUR,LEN_AFN0DF212_SOUR*AFN0DF212MaxPn);
#endif

#if (USER/100)==6//用户标识:湖南系
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		//过零点清除AFN0CF25,F26
		MC(0xEE,ADDR_AFN0CF25,67*AFN0CF25MaxPn);
		MC(0xEE,ADDR_AFN0CF26,61*AFN0CF26MaxPn);
	#endif
#endif
#if (USER/100)==11//吉林
	//清0掉电保存中的月电压统计,因库中AFN0DF35MaxPn改为AFN0DF35MaxPn_ReadMeter与实际掉电保存的ram空间不符,不清0
	MC(0,ADDR_AFN0DF35_SOUR,(66+27)*AFN0DF35MaxPn);
#endif
}









void Terminal_Class2Data_Computer(void)//终端类2数据计算
{
	u32 i;
	u32 x;
	u32 y;
	u16 *p16;
	
	
	if(DataComp(((u32)Terminal_Ram->LastCongealTime),ADDR_TYMDHMS+1,5))//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
	{//年月日时分更新
		if((Terminal_Ram->CongealFlags&0x1f)==0)//在实时钟分更新时清0;正在冻结标志(0正在冻结,1冻结完成);B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
		{
			MR((u32)Terminal_Ram->LastCongealTime,ADDR_TYMDHMS+1,5);//上次冻结时的实时钟分寄存
		}
		else
		{
			if((Terminal_Ram->CongealFlags&0x1f)==0x1f)//在实时钟分更新时清0;正在冻结标志(0正在冻结,1冻结完成);B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
			{
				Terminal_Ram->CongealFlags=0;//在实时钟分更新时清0;正在冻结标志(0正在冻结,1冻结完成);B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
				MR((u32)Terminal_Ram->LastCongealTime,ADDR_TYMDHMS+1,5);//上次冻结时的实时钟分寄存
			}
		}
	}
	switch(Terminal_Ram->I2SUBTask2)
	{
		case 0:
			x=MRR(ADDR_TCongealCount+7,2);
			p16=(u16*)(ADDR_CurveCongealList);
			y=p16[0];
			if(x!=y)
			{
				if((x&0x7fff)>=MaxCurveCongeal)
				{
					x=0;
					MWR(x,ADDR_TCongealCount+7,2);
				}
				if((y&0x7fff)>=MaxCurveCongeal)
				{
					y=0;
				}
				if((x&0x8000)==0)
				{
					if(y&0x8000)
					{
						y=0;//x回0
					}
				}
				i=100;
				while(i--)
				{
					if(x==y)
					{
						break;
					}
					MR(ADDR_CurveCongealList+2+((y&0x7fff)*6),ADDR_CURVECONGEAL_START+((y&0x7fff)*LEN_CURVECONGEAL),5);
					y++;
					if((y&0x7fff)>=MaxCurveCongeal)
					{
						y=0x8000;
					}
				}
				p16[0]=y;
				return;
			}
			Terminal_CurveCongeal();//曲线冻结
			break;
		case 1:
			Terminal_DateCongeal();//日冻结
			break;
		case 2:
			Terminal_RMDateCongeal();//抄表日冻结
			break;
		case 3:
			Terminal_MonthCongeal();//月冻结
			break;
		default:
			Terminal_Ram->I2SUBTask2=0;
			return;
	}
	Terminal_Ram->I2SUBTask2+=1;
}










