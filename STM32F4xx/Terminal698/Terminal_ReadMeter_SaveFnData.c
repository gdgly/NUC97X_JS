
//终端抄表数据存贮
#include "Project.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "KeyUser.h"

#include "../terminal/Terminal_USER2xx.h"




extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];



void ReadMeter_SaveFnData(u32 PORTn,u32 Fn,u32 Pn,u32 FnDataAddr,u8 *p8RMLib)//存贮抄表Fn数据;若只存储实时数据u8 *p8RMLib=0
{
	u32 i;
	u32 x;
	u8 * p8s;
	u8 * p8d;
	u16 *p16;
	u8 * p8FnData;

	if(Pn==0x0)
	{
		return;
	}
	p8FnData=(u8*)FnDataAddr;
	
	SaveClass2Data(PORTn,Pn,FnDataAddr,p8RMLib);//存贮类2测量点抄表数据
#if ERC39==1
	if((p8RMLib[0]==0xE3)||(p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
	{//抄上1-3日日冻结数据
		ERC39_Event_Cancel(Pn);//补抄失败事件恢复
	}
#endif
	if(Fn==0xF1)//扩展17 当前电压、电流相位角(扩展用于终端内部交采)
	{
		Fn=49;
	}
	if(Pn>GetClass1Data_PnMax(Fn))
	{
		switch(Fn)
		{
			case 246://当前掉电记录数据(测量点映射)
				break;
			default:
				return;
		}
	}
	switch(Fn)
	{
		//case 0://F0 通用
		//case 1://F1 重点户曲线(仅对无曲线冻结的单相表)
		case 2://F2 电能表主动上报事件
		case 3://F3 周期读电能表事件
			return;
		case 49://F49 当前电压、电流相位角
		case 57://F57 当前A、B、C三相电压、电流2～N次谐波有效值
		case 58://F58 当前A、B、C三相电压、电流2～N次谐波含有率
		case 162://F162 电能表日历时钟
		case 246://F246(0xF6) 当前掉电记录数据(测量点映射)
			break;
		default:
			MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//填入抄表时间
			break;
	}
	p8s=(u8 *)(FnDataAddr);
	switch(Fn)
	{
//1 F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
//2 F26 A、B、C三相断相统计数据及最近一次断相记录
//3 F27 电能表日历时钟、编程次数及其最近一次操作时间
		case 28://4 F28 电表运行状态字及其变位标志
			i=GetClass1DataAddr(Fn,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
			if(i!=0x0)
			{
				p8s=(u8 *)(i);
				MC(0,FnDataAddr+6,14);//清0当前变位标志
				if(p8s[0]!=0xee)
				{//不是第1次读状态字
					p8s+=5;
					for(i=0;i<14;i++)
					{
						if(p8s[14+i]!=0xee)
						{
							break;
						}
					}
					if(i!=14)
					{//原状态字数据不为0xee
						p8d=(u8 *)(FnDataAddr+6);
						for(i=0;i<14;i++)
						{
							if(p8d[14+i]!=0xee)
							{
								break;
							}
						}
						if(i!=14)
						{//当前状态字数据不为0xee
							//当前变位标志
							for(i=0;i<14;i++)
							{
								p8d[i]=(p8s[14+i]^p8d[14+i]);
							}
							x=0;
							for(i=0;i<14;i++)
							{
								x|=p8d[i];
							}
							if(x!=0x0)
							{//当前有变位
							#if ERC33==1
								ERC33_Event(Pn,FnDataAddr);//电能表运行状态字变位事件(在类1数据存贮前写事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
							#endif
							}
							//逻辑或原变位标志(在读类1数据F28时清0)
							for(i=0;i<14;i++)
							{
								p8d[i]|=p8s[i];
							}
						}
					}
				}
			}
			break;
//5 F29 当前铜损、铁损有功总电能示值
//6 F30 上一结算日铜损、铁损有功总电能示值
//7 F31 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
//8 F32 上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
//F33 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
		case 33://由F129-F144抄表完成后组合得到(仅为费率M≤12的保留选用)
			Class1_F33_F34_F37_F38(Pn,FnDataAddr,129,130,133,136);//抄表数据组合成F33_F34_F37_F38
			break;
//F34 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
		case 34://由F129-F144抄表完成后组合得到(仅为费率M≤12的保留选用)
			Class1_F33_F34_F37_F38(Pn,FnDataAddr,131,132,134,135);//抄表数据组合成F33_F34_F37_F38
			break;
//F35 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 35://由F129-F144抄表完成后组合得到(仅为费率M≤12的保留选用)
			Class1_F35_F36_F39_F40(Pn,FnDataAddr,145,146);//抄表数据组合成F35_F36_F39_F40
			break;
//F36 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 36://由F129-F144抄表完成后组合得到(仅为费率M≤12的保留选用)
			Class1_F35_F36_F39_F40(Pn,FnDataAddr,147,148);//抄表数据组合成F35_F36_F39_F40
			break;
//F37 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
		case 37://由F129-F144抄表完成后组合得到(仅为费率M≤12的保留选用)
			Class1_F33_F34_F37_F38(Pn,FnDataAddr,137,138,141,144);//抄表数据组合成F33_F34_F37_F38
			break;
//F38 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
		case 38://由F129-F144抄表完成后组合得到(仅为费率M≤12的保留选用)
			Class1_F33_F34_F37_F38(Pn,FnDataAddr,139,140,142,143);//抄表数据组合成F33_F34_F37_F38
			break;
//F39 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 39://由F129-F144抄表完成后组合得到(仅为费率M≤12的保留选用)
			Class1_F35_F36_F39_F40(Pn,FnDataAddr,149,150);//抄表数据组合成F35_F36_F39_F40
			break;
//F40 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
		case 40://由F129-F144抄表完成后组合得到(仅为费率M≤12的保留选用)
			Class1_F35_F36_F39_F40(Pn,FnDataAddr,151,152);//抄表数据组合成F35_F36_F39_F40
			break;

//9 F49 当前电压、电流相位角
//10 F57 当前A、B、C三相电压、电流2～N次谐波有效值
//11 F58 当前A、B、C三相电压、电流2～N次谐波含有率
//12 F65 当前电容器投切状态
//13 F66 当前电容器累计补偿投入时间和次数
//14 F67 当日、当月电容器累计补偿的无功电能量
//15 F73 直流模拟量当前数据

//16 F129 当前正向有功电能示值（总、费率1～M）
//17 F130 当前正向无功（组合无功1）电能示值（总、费率1～M）
//18 F131 当前反向有功电能示值（总、费率1～M）
//19 F132 当前反向无功（组合无功2）电能示值（总、费率1～M）
//20 F133 当前一象限无功电能示值（总、费率1～M）
//21 F134 当前二象限无功电能示值（总、费率1～M）
//22 F135 当前三象限无功电能示值（总、费率1～M）
//23 F136 当前四象限无功电能示值（总、费率1～M）
//24 F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
//25 F138 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
//26 F139 上月（上一结算日）反向有功电能示值（总、费率1～M）
//27 F140 上月（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
//28 F141 上月（上一结算日）一象限无功电能示值（总、费率1～M）
//29 F142 上月（上一结算日）二象限无功电能示值（总、费率1～M）
//30 F143 上月（上一结算日）三象限无功电能示值（总、费率1～M）
//31 F144 上月（上一结算日）四象限无功电能示值（总、费率1～M）

//32 F145 当月正向有功最大需量及发生时间（总、费率1～M）
//33 F146 当月正向无功最大需量及发生时间（总、费率1～M）
//34 F147 当月反向有功最大需量及发生时间（总、费率1～M）
//35 F148 当月反向无功最大需量及发生时间（总、费率1～M）
		case 145:
		case 146:
		case 147:
		case 148:
//#if SoftBug3==0//抄表上月最大需量0=比较需量产生的月,1=不比较需量产生的月
//			p8s=(u8 *)(ADDR_TYMDHMS);
//			p8d=(u8 *)(FnDataAddr);
//			if(p8s[4]!=p8d[6+7])
//			{//最大需量发生时间非终端时钟当前月
//				return;
//			}
//#endif
			p8s=(u8 *)(FnDataAddr+7);
			if(p8s[0]==0xee)
			{//当前抄表数据空
				return;
			}
			i=GetClass1DataAddr(Fn,Pn);//取类1测量点抄表数据存贮地址,返回0表示没找到
			p8d=(u8 *)(i+6);
			if(p8d[0]!=0xee)
			{
				if(MRR((u32)p8s,3)<MRR((u32)p8d,3))
				{//当前抄表最大需量值<原最大需量值
					return;
				}
			}
			break;
//36 F149 上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
//37 F150 上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
//38 F151 上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
//39 F152 上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
		case 149:
		case 150:
		case 151:
		case 152:
#if SoftBug3==0//抄表上月最大需量0=比较需量产生的月,1=不比较需量产生的月
			p8s=(u8 *)(ADDR_TYMDHMS);
			i=p8s[4];
			i=bcd_hex(i);
			i-=1;
			if(i==0x0)
			{
				i=12;
			}
			i=hex_bcd(i);
			p8d=(u8 *)(FnDataAddr);
			if(i!=p8d[6+7])
			{//最大需量发生时间非终端时钟上月
				return;
			}
#endif
			break;
//F153 第一时区冻结正向有功电能示值（总、费率1～M）
//F154 第二时区冻结正向有功电能示值（总、费率1～M）
//F155 第三时区冻结正向有功电能示值（总、费率1～M）
//F156 第四时区冻结正向有功电能示值（总、费率1～M）
//F157 第五时区冻结正向有功电能示值（总、费率1～M）
//F158 第六时区冻结正向有功电能示值（总、费率1～M）
//F159 第七时区冻结正向有功电能示值（总、费率1～M）
//F160 第八时区冻结正向有功电能示值（总、费率1～M）

//F161 电能表远程控制通断电状态及记录
//F162 电能表日历时钟
		case 162://电能表日历时钟
			if((p8FnData[1+6]==0xee)||(p8FnData[1+6+3]==0xee))
			{//至少有1个命令没抄
				return;
			}
			i=MRR(FnDataAddr+1+6,3);//读取的电能表时钟时分秒
			if(PORTn==RS485_4PORT)
			{//载波口
				if(i>=0x235500)
				{
					return;//在电能表0点附近读取的时间不算,免分开读取时发生日更新
				}
				//抄第1个HMS和抄第2个YMD的相隔时间
				MR(FnDataAddr+1+12,FnDataAddr+1,6);
				p8FnData[1+12+4]&=0x1f;//去月字节的星期
				i=TimeDifference(FnDataAddr+1+12,ADDR_TYMDHMS);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(i>=240)
				{//2个命令的相隔时间>=4分
					return;
				}
			}
			else
			{//交采或485口
				if(i>=0x235900)
				{
					return;//在电能表0点附近读取的时间不算,免分开读取时发生日更新
				}
			}
			break;
//F165 电能表开关操作次数及时间
//F166 电能表参数修改次数及时间
//F167 电能表购、用电信息
//F168 电能表结算信息

	}
	
//事件(存贮前)
	switch(Fn)
	{
		case 129://当前正向有功电能示值（总、费率1～M）
		#if ERC27==1
			ERC27_Event(Pn,FnDataAddr);//电能表示度下降记录(在类1数据存贮前检查事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
		#endif
			if(PORTn!=RS485_4PORT)
			{//不是载波口
			#if ((ERC28==1)||(ERC29==1))
				ERC29_Event(Pn,FnDataAddr);//电能表飞走,超差(在类1数据存贮前检查事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
			#endif
			}
			//ERC29_Event(Pn,FnDataAddr);//电能表飞走,超差(在类1数据存贮前检查事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
			break;
		case 166://F166 电能表参数修改次数及时间
			ERC41_Meter_Event(FnDataAddr,Pn);//ERC41直接读电能表对时事件记录;入口:Fn数据首地址,测量点号(1开始)
			break;
	}
	
	SaveClass1Data(Fn,Pn,FnDataAddr+1);//存贮类1测量点抄表数据

	
//事件(存贮后)
	switch(Fn)
	{
		case 25://F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
			ERC9_3_Event(Pn);//电流回路异常.反向,入口:Pn测量点号(从1开始)
			//ERC10_Event(Pn);//电压回路异常(断相、失压),入口:Pn测量点号(从1开始)
			break;
		case 162://F162 电能表日历时钟
			ERC12_Event(Pn);//电能表时间超差,入口:Pn测量点号(从1开始)
			break;
		case 129://当前正向有功电能示值（总、费率1～M）
			if(PORTn!=RS485_4PORT)
			{//不是载波口
			#if ERC30==1
				ERC30_Event(Pn);//电能表停走事件,入口:Pn=测量点号(1开始)
			#endif
			}
			//ERC30_Event(Pn);//电能表停走事件,入口:Pn=测量点号(1开始)
			
			if((PORTn==ACSAMPLEPORT)||(FnDataAddr==(ADDR_DATABUFF+(256+4-6-1))))
			{//是内部交采口或脉冲输入口
				if(Pn>ERC29MaxPn)
				{
					break;
				}
				p16=(u16 *)(ADDR_ERC29_Timer);//ERC29电能表飞走秒定时器(当前仅用于内部交采定时)
				if(p16[Pn-1]!=0)
				{
					break;
				}
				i=MRR(ADDR_AFN04F33+9,1);//端口1(内部交采)抄表间隔时间
				if(i==0)
				{//没设置用端口2(rs485-1)
					i=MRR(ADDR_AFN04F33+14+(4*24)+9,1);//抄表间隔时间
				}
				i*=60;
				p16[Pn-1]=i;
			}
			break;
		
	}
	
}






