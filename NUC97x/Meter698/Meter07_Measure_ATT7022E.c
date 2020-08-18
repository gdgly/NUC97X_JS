
//电能表变量(ATT7022E)
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/Meter698/Meter07_Measure_ATT7022E.h"
#include "../../NUC97x/Meter698/Meter07_Energy_ATT7022E.h"
#include "../../NUC97x/Driver/NUC97x_ATT7022.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../../STM32F4xx/Meter698/Meter07_In.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"


#if IC_ATT7022==1//0=没,1=有

void EMUStartStopUerrIerr(void)//起动潜动电压相序错电流相序错
{
//Bit00 PA =1，表示A相失压；=0，A相未失压。
//Bit01 PB =1，表示B相失压；=0，B相未失压。
//Bit02 PC =1，表示C相失压；=0，C相未失压。
//Bit03 Uorder =1，表示电压相序错；=0，电压未错相序。
//Bit04 Iorder =1，表示电流相序错；=0，电流未错相序。
//Bit05 Revp =1，表示至少有一相有功功率为负；=0，所有相有功功率为正。
//Bit06 Revq =1，表示至少有一相无功功率为负；=0，所有相无功功率为正。
//Bit07 上电复位后，IRQ pin信号自动变低，同时SFlag.7置高；当SPI写入数据后，
//IRQ信号自动变高的同时SFlag.7自动变低，即SFlag的bit07 IRQ标志位与硬件
//IRQ信号是完全同步的。
//Bit09 =1，表示A相处于潜动状态；=0，A相处于起动状态。
//Bit10 =1，表示B相处于潜动状态；=0，B相处于起动状态。
//Bit11 =1，表示C相处于潜动状态；=0，C相处于起动状态。
//Bit12 =1，表示合相有功功率为负；=0，合相有功功率为正。
//Bit13 =1，表示合相无功功率为负；=0，合相无功功率为正。
//Bit14 =1，表示同步数据缓存完毕，Sync_En写0清除。
//Bit15 =1，表示同步数据功能系数超出范围，ync_En写0清除。
//Bit17 =1，表示A相基波处于潜动状态；=0，A相基波未潜动。
//Bit18 =1，表示B相基波处于潜动状态；=0，B相基波未潜动。
//Bit19 =1，表示C相基波处于潜动状态；=0，C相基波未潜动。
	u32 i;
	u32 Data;
	
	Data=Security_Read_ATT7022(0x2C);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return;
	}
//起动潜动
	i=(Data>>9)&0x7;//位为1表示空载;b2=有功C,b1=有功B,b0=有功A
	Meter_Ram->NOPLoad=i;
	Meter_Ram->NOQLoad=i;
	Meter_Ram->NOSLoad=i;
//电压相序错
	i=Data&(1<<3);
	if(i)
	{
		Meter_Ram->IPFlags|=0x80;
	}
	else
	{
		Meter_Ram->IPFlags&=~0x80;
	}
//表示电流相序错
	i=Data&(1<<4);
	if(i)
	{
		Meter_Ram->IPFlags|=0x40;
	}
	else
	{
		Meter_Ram->IPFlags&=~0x40;
	}
	
}

u32 Calculate_U(u32 Addr,u32 ADDR_DATA)//电压计算
{
//分相Vrms：24位数据，补码形式
//实际分相电压有效值为：Urms = Vrms/2^13
	u32 Data;
	u64 d64;
	
	Data=Security_Read_ATT7022(Addr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return 0;
	}
	d64=Data;
	d64*=100000;//4BYTE_BCD 5位小数
	d64/=1<<13;
	Data=d64;
#if DEBUG==0//0=发行,1=调试
	if(Data<(((MeterUBhex*10)/100)*10000))
	{//<(10%UN)
		Data=0;
	}
#endif
	Data=hex_bcd(Data);
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,4);
	}
	return Data;
}

u32 Calculate_I(u32 Addr,u32 ADDR_DATA)//电流计算
{
//实际分相电流有效值为：Irms = (Vrms/2^13)/N
//比例系数N定义：额定电流Ib输入到芯片端取样电压为50mV时，对应的电流有效值寄存器值为Vrms，Vrms/2^13约等于60，此时N=60/Ib， Ib=1.5A，N=60/1.5=40，Ib=6A，N=60/6=10
//同理，当输入到芯片端取样电压为25mV时，Vrms/2^13约等于30，Ib=1.5A，N=30/1.5=20，Ib=6A，N=30/6=5。可根据当前Ib电流的实际值，计算N值
	u32 Data;
	float N;
	float Irms;
	
	N=60;
	N*=ADC_Vi;
	N/=0.05;//以芯片端取样电压为50mV时,Vrms/2^13约等于60为准
	N*=100;//MeterIBhex有二位小数
	N/=MeterIBhex;
	
	Data=Security_Read_ATT7022(Addr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return 0;
	}
	Irms=Data;
	Irms*=100000;//4BYTE_BCD 5位小数
	Irms/=1<<13;
	Irms/=N;
	Data=Irms;
#if DEBUG==0//0=发行,1=调试
	#if MeterIMaxhex<=2000//最大电流xx.xx(A)
	if(Data<200)
	{//<2.00MA
		Data=0;
	}
	#else
	if(Data<400)
	{//<4.00MA
		Data=0;
	}
	#endif
#endif
	Data=hex_bcd(Data);
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,4);
	}
	return Data;
}

u32 Calculate_P(u32 Addr,u32 ADDR_DATA)//功率计算
{
//功率寄存器格式定义：
//A/B/C分相功率参数：X
//X：24位数据，补码形式
//如果X>2^23，则XX=X-2^24
//否则XX=X
//实际的A/B/C分相功率参数为：XXX=XX*K（其中K为功率参数系数，所有功率参数共用）
//单位：功率单位是瓦(W)，功率系数K=2.592*10^10/(HFconst*EC*2^23)
	u32 Data;
	float K;
	u32 s;
#if DEBUG==0//0=发行,1=调试
	u64 d64a;
	u64 d64b;
	u32 UAddr;
#endif
	
	K=25920000000;
	K/=HFConst;
	K/=MeterActiveConst;
	K/=1<<23;
	
	Data=Security_Read_ATT7022(Addr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return 0;
	}
	if(Data&(1<<23))
	{
		s=1;//负数
		Data=~Data;
		Data++;
		Data&=0xffffff;
	}
	else
	{
		s=0;//正数
	}
	K*=Data;
	K*=1000;//4BYTE_SBCD 6位小数
	Data=K;
#if DEBUG==0//0=发行,1=调试
	d64a=MeterUBhex;//参比电压xxx.x(V)
	d64b=MeterIBhex;//参比电流xx.xx(A)
	d64a*=d64b;//x.xxxVA
	d64a/=1000;//0.1%IB
	if(Data<=d64a)
	{//<=0.1%IB功率,功率=0
		Data=0;
	}
	if(Data<=(d64a*10))
	{//<=1.0%IB功率,功率方向不变
		s=MRR(ADDR_DATA+3,1);
		s&=0x80;
	}
	switch(ADDR_DATA)
	{//视在功率方向=有功功率方向
		case ADDR_SA:
			s=MRR(ADDR_PA+3,1);
			s&=0x80;
			break;
		case ADDR_SB:
			s=MRR(ADDR_PB+3,1);
			s&=0x80;
			break;
		case ADDR_SC:
			s=MRR(ADDR_PC+3,1);
			s&=0x80;
			break;
	}
	switch(ADDR_DATA)
	{
		case ADDR_PA:
		case ADDR_QA:
		case ADDR_SA:
			UAddr=ADDR_UA;
			break;
		case ADDR_PB:
		case ADDR_QB:
		case ADDR_SB:
			UAddr=ADDR_UB;
			break;
		case ADDR_PC:
		case ADDR_QC:
		case ADDR_SC:
			UAddr=ADDR_UC;
			break;
	}
	UAddr=MRR(UAddr+2,2);
	if(UAddr<((MeterUBhex*10)/100))
	{//电压<(10%UN),功率方向不变
		s=MRR(ADDR_DATA+3,1);
		s&=0x80;
	}
#endif
	Data*=1004;//实际校验发现显示值小0.4%
	Data/=1000;
	Data=hex_bcd(Data);
	if(Data)
	{
		if(s)
		{
			Data|=0x80000000;
		}
	}
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,4);
	}
	return Data;
}

u32 Calculate_PT(u32 Addr,u32 ADDR_DATA)//合相功率计算
{
//功率寄存器格式定义：
//A/B/C合相功率参数：T
//T：24位数据，补码形式
//如果T>2^23，则TT=T-2^24
//否则TT=T
//实际的合相功率参数为：TTT=TT*2*K（其中K为功率参数系数，所有功率参数共用）。
//单位：功率单位是瓦(W)，功率系数K=2.592*10^10/(HFconst*EC*2^23)
	u32 Data;
	float K;
	u32 s;
#if DEBUG==0//0=发行,1=调试
	u64 d64a;
	u64 d64b;
#endif
	
	K=25920000000;
	K/=HFConst;
	K/=MeterActiveConst;
	K/=1<<23;
	K*=2;
	
	Data=Security_Read_ATT7022(Addr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return 0;
	}
	if(Data&(1<<23))
	{
		s=1;//负数
		Data=~Data;
		Data++;
		Data&=0xffffff;
	}
	else
	{
		s=0;//正数
	}
	K*=Data;
	K*=1000;//4BYTE_SBCD 6位小数
	Data=K;
#if DEBUG==0//0=发行,1=调试
	d64a=MeterUBhex;//参比电压xxx.x(V)
	d64b=MeterIBhex;//参比电流xx.xx(A)
	d64a*=d64b;//x.xxxVA
	d64a/=1000;//0.1%IB
	if(Data<=d64a)
	{//<=0.1%IB功率,功率=0
		Data=0;
	}
	if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
	{
		d64a*=(3*10);
	}
	else
	{
		d64a*=(2*10);
	}
	if(Data<=d64a)
	{//<=1.0%IB功率,功率方向不变
		s=MRR(ADDR_DATA+3,1);
		s&=0x80;
	}
	if(ADDR_DATA==ADDR_ST)
	{//视在功率方向=有功功率方向
		s=MRR(ADDR_PT+3,1);
		s&=0x80;
	}
#endif
	Data*=1004;//实际校验发现显示值小0.4%
	Data/=1000;
	Data=hex_bcd(Data);
	if(Data)
	{
		if(s)
		{
			Data|=0x80000000;
		}
	}
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,4);
	}
	return Data;
}

void Quadrant(u32 ADDR_P,u32 ADDR_Q,u32 ADDR_FLAG)//象限计算
{
	u32 i;
	u8 *p8;
	
	i=0;
	p8=(u8 *)(ADDR_P);
	if((p8[3]&0x80)!=0x0)
	{
		i=1;
	}
	p8=(u8 *)(ADDR_Q);
	if((p8[3]&0x80)!=0x0)
	{
		i|=0x2;
	}
	p8=(u8 *)(ADDR_FLAG);
	switch(i)
	{
		case 0://1象限
			p8[0]=1;
		break;
		case 1://2象限
			p8[0]=2;
		break;
		case 2://4象限
			p8[0]=4;
		break;
		case 3://3象限
			p8[0]=3;
		break;
	}
}

u32 Calculate_COS(u32 Addr,u32 ADDR_DATA)//功率因数计算
{
//ATT7022E/26E/28E功率因数寄存器采用补码形式给出，最高位是符号位，功率因数的符号位与有功功率的符号位一致
//PF：24位数据，补码形式
//如果PF>2^23，则PFF=PF-2^24
//否则 PFF=PF
//实际功率因数为：pf=PFF/2^23
	u32 Data;
	u64 d64;
	u32 s;
	
	Data=Security_Read_ATT7022(Addr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return 0;
	}
	if(Data&(1<<23))
	{//负
		s=1;
		Data=~Data;
		Data++;
		Data&=0xffffff;
	}
	else
	{//正
		s=0;
	}
	d64=Data;
	d64*=1000;//2BYTE_SBCD 3位小数
	d64/=1<<23;
	Data=d64;
	Data=hex_bcd(Data);
	if(s)
	{
		Data|=0x8000;
	}
	if(ADDR_DATA)
	{
		MWR(Data,ADDR_DATA,2);
	}
	return Data;
}

u32 Calculate_TEMP(void)//温度计算
{
//温度传感器需要配置寄存器0x31，TPS_En=1开启，TPS_Sel=0 选择PN温度传感器。
//数据格式为TM：24位数据的低8位有效
//如果TM大于128，则TMM=TM-256
//否则 TMM=TM
//外部MCU读取该寄存器的值，按照上述变换之后，再根据下列公式得到真实温度值：
//真实温度TP=TC - 0.726*TMM
//其中TC为校正值，当室温为25度时，进行温度校正得到TC
	
//#define ADDR_TEMP     (ADDR_UDC+2)//表内温度(半字对齐)
//	u16 TEMP;//BCD 1位小数(b15符号位)
	
	u32 Data;
	u32 TC;
	float t;
	
	Data=Security_Read_ATT7022(0x2A);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return 0;
	}
	t=Data;
	if(Data>128)
	{
		t-=256;
	}
	TC=MRR(ADDR_MeterCalibration_IEEPROM_START+10,1);
	t*=0.726;
	t=TC-t;
	t*=10;//1位小数
	if(t>=0)
	{
		TC=t;
		TC=hex_bcd(TC);
		MWR(TC,ADDR_TEMP,2);
		return TC;
	}
	else
	{
		t*=-1;
		TC=t;
		TC=hex_bcd(TC);
		TC|=0x8000;
		MWR(TC,ADDR_TEMP,2);
		return TC;
	}
}

u32 Calculate_Hz(void)//频率计算
{
//电压线频率寄存器采用24位补码形式给出，最高位为符号位，符号位总是为0。
//Freq：24位数据，补码形式
//实际频率为：f=Freq/2^13，单位：赫兹(Hz)。
	u32 Data;
	
	Data=Security_Read_ATT7022(0x1C);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return 0;
	}
	Data*=100;//2BYTE_BCD 2位小数
	Data/=1<<13;
	Data=hex_bcd(Data);
	MWR(Data,ADDR_FREQUENCY,2);
	return Data;
}

u32 Calculate_Angle(u32 Addr,u32 ADDR_DATA)//角度计算
{
//相角寄存器采用补码形式给出，高位是符号位，表示-180°~+180°之间的夹角。
//θ：21位有效数据，补码形式，高3位均为扩展的符号位
//如果θ>=2^20，则α=θ-2^24 否则α=θ
//实际相角为：Pg=(α/2^20)*180度
	u32 Data;
	u32 s;
	
	Data=Security_Read_ATT7022(Addr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
	if(Data==0xffffffff)
	{
		return 0;
	}
	if(Data&(1<<23))
	{//负
		s=1;
		Data=~Data;
		Data++;
		Data&=0xffffff;
	}
	else
	{//正
		s=0;
	}
	Data*=180;
	Data*=10;//2BYTE_BCD 1位小数
	Data/=1<<20;
	if(s)
	{//负
		Data=3600-Data;
	}
	Data=hex_bcd(Data);
	MWR(Data,ADDR_DATA,2);
	return Data;
}

u32 Calculate_AngleUI(u32 ADDR_AngleU,u32 ADDR_AngleI,u32 ADDR_AngleUI)//电压电流夹角
{
	u32 Qu;
	u32 Qi;
	
	Qu=MRR(ADDR_AngleU,2);
	Qi=MRR(ADDR_AngleI,2);
	Qu=bcd_hex(Qu);
	Qi=bcd_hex(Qi);
	if(Qi<Qu)
	{
		Qi+=3600;
	}
	Qi-=Qu;
	Qi=hex_bcd(Qi);
	MWR(Qi,ADDR_AngleUI,2);
	return Qi;
}



void Meter07_Measure(void)//电表计量
{
	u32 i;
	u32 x;
	u32 ReNum;
	u8* p8;
	
	if(Meter_Ram->EMUInit!=5)
	{
		return;
	}
	switch(Meter_Ram->I1SubTask1)//I1子分时任务1,用于变量计算:电压,电流,功率,...等
	{
		case 0://CS比较
			for(ReNum=100;ReNum>0;ReNum--)
			{//有EMC干忧时免误判，注:100次约需50ms
				i=Read_ATT7022(0x3e);
				x=Read_ATT7022(0x5e);
				if((Meter_Ram->SSP1Data0==i)&&(Meter_Ram->SSP1Data1==x))//校表数据校验和
				{
					break;
				}
				if(Pin_Read(PIN_WKUP)==0)
				{//掉电
					 break;
				}
			}
			if(ReNum==0)
			{//CS错
				Meter_Ram->EMUParaCount=0;
				Meter_Ram->EMUInit=4;//0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
				return;
			}
			break;
		case 1:
			EMUStartStopUerrIerr();//起动潜动电压相序错电流相序错
			break;
		case 2://UA计算
			i=Calculate_U(0x0d,ADDR_UA);//电压计算
		#if (USER/100)==5//上海
			Meter07_UAB();//UAB相电压计算
		#endif
			break;
		case 3://UB计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				i=Calculate_U(0x0e,ADDR_UB);//电压计算
			}
			else
			{
				MWR(0,ADDR_UB,4);
			}
		#if (USER/100)==5//上海
			Meter07_UBC();//UBC相电压计算
		#endif
			break;
		case 4://UC计算
			i=Calculate_U(0x0f,ADDR_UC);//电压计算
		#if (USER/100)==5//上海
			Meter07_UCA();//UCA相电压计算
		#endif
			break;
		case 5://PT计算
			i=Calculate_PT(0x04,ADDR_PT);//合相功率计算
			break;
		case 6://PA计算
			i=Calculate_P(0x01,ADDR_PA);//功率计算
			break;
		case 7://PB计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				i=Calculate_P(0x02,ADDR_PB);//功率计算
			}
			else
			{
				MWR(0,ADDR_PB,4);
			}
			break;
		case 8://PC计算
			i=Calculate_P(0x03,ADDR_PC);//功率计算
			break;
		case 9://QT计算
			i=Calculate_PT(0x08,ADDR_QT);//合相功率计算
			Quadrant(ADDR_PT,ADDR_QT,(u32)&Meter_Ram->TQ);//象限计算
			break;
		case 10://QA计算
			i=Calculate_P(0x05,ADDR_QA);//功率计算
			Quadrant(ADDR_PA,ADDR_QA,(u32)&Meter_Ram->AQ);//象限计算
			break;
		case 11://QB计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				i=Calculate_P(0x06,ADDR_QB);//功率计算
			}
			else
			{
				MWR(0,ADDR_QB,4);
			}
			Quadrant(ADDR_PB,ADDR_QB,(u32)&Meter_Ram->BQ);//象限计算
			break;
		case 12://QC计算
			i=Calculate_P(0x07,ADDR_QC);//功率计算
			Quadrant(ADDR_PC,ADDR_QC,(u32)&Meter_Ram->CQ);//象限计算
			break;
		case 13://ST计算
			i=Calculate_PT(0x0C,ADDR_ST);//合相功率计算
			break;
		case 14://SA计算
			i=Calculate_P(0x09,ADDR_SA);//功率计算
			break;
		case 15://SB计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				i=Calculate_P(0x0A,ADDR_SB);//功率计算
			}
			else
			{
				MWR(0,ADDR_SB,4);
			}
			break;
		case 16://SC计算
			i=Calculate_P(0x0B,ADDR_SC);//功率计算
			break;
		case 17://IA计算
			i=Calculate_I(0x10,ADDR_IA);//电流计算
			p8=(u8*)(ADDR_PA);
			i=p8[3]&0x80;
			p8=(u8*)(ADDR_IA);
			p8[3]|=i;
			break;
		case 18://IB计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				i=Calculate_I(0x11,ADDR_IB);//电流计算
				p8=(u8*)(ADDR_PB);
				i=p8[3]&0x80;
				p8=(u8*)(ADDR_IB);
				p8[3]|=i;
			}
			else
			{
				MWR(0,ADDR_IB,4);
			}
			break;
		case 19://IC计算
			i=Calculate_I(0x12,ADDR_IC);//电流计算
			p8=(u8*)(ADDR_PC);
			i=p8[3]&0x80;
			p8=(u8*)(ADDR_IC);
			p8[3]|=i;
			break;
		case 20://IN计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
//				x=MRR(ADDR_IN,4);//原数据
//				i=Calculate_I(0x29,ADDR_IN);//电流计算
//        Data_Valid(x,i,ADDR_IN);//数据有效性判断(EMC干忧)
//				p8=(u8*)(ADDR_PT);
//				i=p8[3]&0x80;
//				p8=(u8*)(ADDR_IN);
//				p8[3]|=i;
				Meter07_In();//零线电流计算
			}
			break;
		case 21://COST计算
			i=Calculate_COS(0x17,ADDR_COST);//功率因数计算
			x=0;
			i=MRR(ADDR_IA,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				x|=1;
			}
			i=MRR(ADDR_IB,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				x|=2;
			}
			i=MRR(ADDR_IC,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				x|=4;
			}
			if(x==7)
			{//三相均无电流
				MWR(0x1000,ADDR_COST,2);
			}
			break;
		case 22://COSA计算
			i=Calculate_COS(0x14,ADDR_COSA);//功率因数计算
			x=0;
			i=MRR(ADDR_UA+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//电压<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IA,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				x=1;
			}
			if(x!=0)
			{
				MWR(0x1000,ADDR_COSA,2);
			}
			break;
		case 23://COSB计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				i=Calculate_COS(0x15,ADDR_COSB);//功率因数计算
				x=0;
				i=MRR(ADDR_UB+2,2);
				i=bcd_hex(i);
				if(i<((MeterUBhex*10)/100))
				{//电压<(10%UN)
					x=1;
				}
				i=MRR(ADDR_IB,4);
				i&=0x7fffffff;
				i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
				i=bcd_hex(i);
				if(i<((MeterIBhex*2)/3))
				{//电流<(1%IB)的2/3
					x=1;
				}
				if(x!=0)
				{
					MWR(0x1000,ADDR_COSB,2);
				}
			}
			break;
		case 24://COSC计算
			i=Calculate_COS(0x16,ADDR_COSC);//功率因数计算
			x=0;
			i=MRR(ADDR_UC+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//电压<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IC,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				x=1;
			}
			if(x!=0)
			{
				MWR(0x1000,ADDR_COSC,2);
			}
			break;
		case 25://温度计算
			i=Calculate_TEMP();//温度计算
			break;
		case 26://频率计算
			i=Calculate_Hz();//频率计算
			break;

		case 27://UB角度计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				i=Calculate_Angle(0x27,ADDR_ANGLE_UB);//角度计算
			}
			i=MRR(ADDR_UB+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//电压<(10%UN)
				MC(0,ADDR_ANGLE_UB,2);
			}
			break;
		case 28://UC角度计算
			i=Calculate_Angle(0x28,ADDR_ANGLE_UC);//角度计算
			i=MRR(ADDR_UC+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//电压<(10%UN)
				MC(0,ADDR_ANGLE_UC,2);
			}
			break;
		case 29://IA角度计算
			i=Calculate_Angle(0x18,ADDR_ANGLE_IA);//角度计算
			i=MRR(ADDR_IA,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				MC(0,ADDR_ANGLE_IA,2);
			}
			break;
		case 30://IB角度计算
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				i=Calculate_Angle(0x19,ADDR_ANGLE_IB);//角度计算
				i=MRR(ADDR_IB,4);
				i&=0x7fffffff;
				i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
				i=bcd_hex(i);
				if(i<((MeterIBhex*2)/3))
				{//电流<(1%IB)的2/3
					MC(0,ADDR_ANGLE_IB,2);
				}
			}
			else
			{
				MC(0,ADDR_ANGLE_IB,2);
			}
			break;
		case 31://IC角度计算
			i=Calculate_Angle(0x1A,ADDR_ANGLE_IC);//角度计算
			i=MRR(ADDR_IC,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				MC(0,ADDR_ANGLE_IC,2);
			}
			break;
		case 32://电压电流夹角
			Calculate_AngleUI(ADDR_ANGLE_UA,ADDR_ANGLE_IA,ADDR_ANGLE_UIA);//电压电流夹角
			Calculate_AngleUI(ADDR_ANGLE_UB,ADDR_ANGLE_IB,ADDR_ANGLE_UIB);//电压电流夹角
			Calculate_AngleUI(ADDR_ANGLE_UC,ADDR_ANGLE_IC,ADDR_ANGLE_UIC);//电压电流夹角
			x=0;
			i=MRR(ADDR_UA+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//电压<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IA,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				x=1;
			}
			if(x!=0)
			{
				MC(0x0,ADDR_ANGLE_UIA,2);
			}
			x=0;
			i=MRR(ADDR_UB+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//电压<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IB,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				x=1;
			}
			if(x!=0)
			{
				MC(0x0,ADDR_ANGLE_UIB,2);
			}
			x=0;
			i=MRR(ADDR_UC+2,2);
			i=bcd_hex(i);
			if(i<((MeterUBhex*10)/100))
			{//电压<(10%UN)
				x=1;
			}
			i=MRR(ADDR_IC,4);
			i&=0x7fffffff;
			i>>=4;//取4位小数,因参比电流xx.xx(A)的1%IB也是4位小数
			i=bcd_hex(i);
			if(i<((MeterIBhex*2)/3))
			{//电流<(1%IB)的2/3
				x=1;
			}
			if(x!=0)
			{
				MC(0x0,ADDR_ANGLE_UIC,2);
			}
			break;
		
		case 33://总正反向有功电能
			Meter07_ActiveEnergy_Total();//合相有功电能
			break;
		case 34://总正反无功电能
			Meter07_ReactiveEnergy_Total();//合相无功电能
			break;
		case 35://A正反向有功电能
			Meter07_ActiveEnergy_Phase(0x1E,1<<0,ADDR_00150000);//分相有功电能,入口:相电能地址
			break;
		case 36://B正反向有功电能
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				Meter07_ActiveEnergy_Phase(0x1F,1<<1,ADDR_00290000);//分相有功电能,入口:相电能地址
			}
			break;
		case 37://C正反向有功电能
			Meter07_ActiveEnergy_Phase(0x20,1<<2,ADDR_003D0000);//分相有功电能,入口:相电能地址
			break;
		case 38://A正反无功电能
			Meter07_ReactiveEnergy_Phase(0x22,1<<4,ADDR_00150000);//分相有功电能,入口:相电能地址
			break;
		case 39://B正反无功电能
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线
			{
				Meter07_ReactiveEnergy_Phase(0x23,1<<5,ADDR_00290000);//分相有功电能,入口:相电能地址
			}
			break;
		case 40://C正反无功电能
			Meter07_ReactiveEnergy_Phase(0x24,1<<6,ADDR_003D0000);//分相有功电能,入口:相电能地址
			break;
		
		
		
		default:
			//注：因速度很快,每个读循环只约XXXms,故加入定时每秒一次循环
			if(Meter_Ram->Read90E36Timer!=0)
			{
				return;
			}
			Meter_Ram->Read90E36Timer=1000/10;//循环读90E36测量数据每10MS减1定时器
			Meter_Ram->I1SubTask1=0;//I1子分时任务1,用于变量计算:电压,电流,功率,...等
			if(Comm_Ram->SPI2Timer==0)//下次参数重写在2秒后(在Meter07_Measure()中检查此值)
			{
			#if (USER%100)!=0
				//参数读写时ReNum=只有2，有EMC干忧时误判而复位7022，电压电量等会瞬变为0
				Meter_Ram->EMUParaCount=0;//计量芯片参数初始化计数
				Meter_Ram->EMUInit=4;//4=参数初始化
			#endif
			}
			return;
	}
	Meter_Ram->I1SubTask1++;//I1子分时任务1,用于变量计算:电压,电流,功率,...等
}


#endif//#if IC_ATT7022==1//0=没,1=有




