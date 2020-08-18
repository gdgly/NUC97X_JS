

//Meter_Uart_工厂
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "Meter07_Uart_Factory_ATT7022E.h"
#include "../../STM32F4xx/Meter/Meter07_DL645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../NUC97x/Driver/NUC97x_ATT7022.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"


#if IC_ATT7022==1//0=没,1=有

extern const u16 MeterCalInitLib[];//校表参数初始化库

u32 UI_gain0(u32 Ur,u32 ADDR_PARA)//反向计算gain为0时测量数据
{
//gain=Ur/Urms-1
//Urms=Ur/(gain+1)
	u32 i;
	float Urms;
	float gain;
	i=MRR(ADDR_PARA,2);
	if((i&(1<<15))==0)
	{
		gain=i;
		gain+=1<<15;
		gain/=1<<15;
	}
	else
	{
		i=~i;
		i++;
		i&=0xffff;
		gain=1<<15;
		gain-=i;
		gain/=1<<15;
	}
	Urms=Ur;
	Urms/=gain;
	i=(u32)Urms;
	return i;
}

u32 Calculate_U(u32 Addr,u32 ADDR_DATA);//电压计算
void ADJ_U(u32 PORTn,u32 REGaddr,u32 ADDR_PARA)//电压增益校正
{
//在Ugain=0时，标准表上读出实际输入电压有效值Ur，通过SPI口读出测量电压有效值寄存器的值为DataU
//测量电压有效值Urms=DataU/2^13
//Ugain=Ur/Urms-1
//如果Ugain≥0，则Ugain=INT[Ugain*2^15]
//如果Ugain<0，则Ugain=INT[2^16+ Ugain*2^15]
	
	u32 i;
	float Ur;
	float Um;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	Ur=i;//4位小数
	i=Calculate_U(REGaddr,0);
	i>>=4;//4位小数
	i=bcd_hex(i);
	i=UI_gain0(i,ADDR_PARA);//反向计算gain为0时测量数据
	Um=i;
	
	Ur/=Um;
	Ur-=1;
	if(Ur>=0)
	{
		Ur*=(1<<15);
	}
	else
	{
		Ur*=(1<<15);
		Ur+=(1<<16);
	}
	i=(u32)Ur;
	MWR(i,ADDR_PARA,2);
}

u32 Calculate_I(u32 Addr,u32 ADDR_DATA);//电流计算
void ADJ_I(u32 PORTn,u32 REGaddr,u32 ADDR_PARA)//电流增益校正
{
//在Igain=0时，标准表上读出实际输入电流有效值Ir，通过SPI口读出测量电压有效值寄存器的值为DataI
//测量电压有效值Irms=(DataI/2^13)/N (额定电流对应取样信号为25mV，则N=30/Ib；额定电流对应取样信号为50mV，则N=60/Ib；详见4.2.2有效值寄存器描述)
//Igain=Ir/Irms-1
//如果Igain≥0，则Igain=INT[Igain*2^15]
//如果Igain≤0，则Igain=INT[2^16+ Igain*2^15]
	u32 i;
	float Ir;
	float Im;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	Ir=i;//4位小数
	
	i=Calculate_I(REGaddr,0);
	i>>=4;//4位小数
	i=bcd_hex(i);
	i=UI_gain0(i,ADDR_PARA);//反向计算gain为0时测量数据
	Im=i;
	
	Ir/=Im;
	Ir-=1;
	if(Ir>=0)
	{
		Ir*=(1<<15);
	}
	else
	{
		Ir*=(1<<15);
		Ir+=(1<<16);
	}
	i=(u32)Ir;
	MWR(i,ADDR_PARA,2);
}





u32 ADJ_Calculate_P(u32 Addr,u32 ADDR_DATA)//功率计算
{
//功率寄存器格式定义：
//A/B/C分相功率参数：X
//X：24位数据，补码形式
//如果X>2^23，则XX=X-2^24
//否则XX=X
//实际的A/B/C分相功率参数为：XXX=XX*K（其中K为功率参数系数，所有功率参数共用）
//单位：功率单位是瓦(W)，功率系数K=2.592*10^10/(HFconst*EC*2^23)
	u32 Data;
	//float K;
	double K;
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

void ADJ_P(u32 PORTn,u32 REGaddr,u32 ADDR_P,u32 ADDR_Q,u32 ADDR_S)//功率增益校正
{
//标准表上读出误差为err%
//计算公式：
//	Pgain=-err%/(1+err%)
//如果Pgain>=0，则GP1=INT[Pgain*2^15]
//否则Pgain<0， 则GP1=INT[2^16+Pgain*2^15]
	u32 i;
	float Pr;
	float Pm;
	float err;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	Pr=i;
	i=ADJ_Calculate_P(REGaddr,0);//功率计算
	i&=0x7fffffff;
	i=bcd_hex(i);
	Pm=i;
	Pm/=100;//6位变4位小数
	err=(Pm-Pr)/Pr;
	Pr=(-err)/(1+err);
	if(Pr>=0)
	{
		Pr*=(1<<15);
	}
	else
	{
		Pr*=(1<<15);
		Pr+=(1<<16);
	}
	i=(u32)Pr;
	i+=MRR(ADDR_P,2);
	MWR(i,ADDR_P,2);
	MWR(i,ADDR_Q,2);
	MWR(i,ADDR_S,2);
}

void ADJ_TEMP(u32 PORTn)//温度校验
{
//进行TPS一致性校正，使得TPSData（计量参数0x2A）值在常温（25度）输出为0x00。校正方式：
//直接读取TPSData（计量参数0x2A）在常温（25度）输出值，直接写入Toffset校正寄存器(校表参数0x6B)即可。
	u32 i;
	
	i=Read_ATT7022(0x2A);
	MWR(i,ADDR_EMU_6B,2);
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	i/=10000;//去4位小数
	MWR(i,ADDR_MeterCalibration_IEEPROM_START+10,2);//温度CAL(ATT7022在温度校定时的当前温度值xx度(hex,总是正温度值)
}

void ADJ_Offset(u32 REGaddr,u32 ADDR_EMU_x)
{
//输入信号为0的时候，读取寄存器的值Irms
//计算公式：IrmsOffset = (Irms^2)/ (2^15)。
//在分相有效值增益校正之前，进行ItRmsoffset校正(校表参数0x6A)。
//已知：输入信号为0的时候，读取寄存器的值Itrms
//计算公式：ItRmsoffset = (Irms^2)/ (2^15)
//有效值和功率的更新速率选择，=1表示慢速(1.76Hz)；=0快速(14.4Hz)。
	u32 i;
	
	i=Read_ATT7022(REGaddr);
	i*=i;
	i/=1<<15;
	i+=MRR(ADDR_EMU_x,2);
	MWR(i,ADDR_EMU_x,2);
}

void ADJ_ERR10(u32 PORTn,u32 ADDR_P,u32 ADDR_Q,u32 ADDR_S)//1.0 Ib误差调整
{
//标准表上读出误差为err%
//计算公式：
//	Pgain=-err%/(1+err%)
//如果Pgain>=0，则GP1=INT[Pgain*2^15]
//否则Pgain<0， 则GP1=INT[2^16+Pgain*2^15]
	u32 i;
	float err;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	if(i&0x80000000)
	{
		i=~i;
		i++;
		err=i;
		err*=-1;
	}
	else
	{
		err=i;
	}
	err/=1000000;//输入时4位小数+err%的2位小数
	err=(-err)/(1+err);
	if(err>=0)
	{
		err*=(1<<15);
	}
	else
	{
		err*=(1<<15);
		err+=(1<<16);
	}
	i=(u32)err;
	i+=MRR(ADDR_P,2);
	MWR(i,ADDR_P,2);
	MWR(i,ADDR_Q,2);
	MWR(i,ADDR_S,2);
}

void ADJ_ERR05L(u32 PORTn,u32 ADDR_Q1,u32 ADDR_Q2,u32 ADDR_Q3)//05L Ib误差调整
{
//在功率因数cos(φ)=1时，功率增益已经校正好之后，再进行相位补偿，相位校正在cos(φ)=0.5L时进行校正。
//已知：0.5L处标准表误差读数为err%
//相位补偿公式：
//θ=(-err%)/1.732
//如果θ >= 0，PhSregpq = INT[θ *2 ^15]
//否则θ < 0，PhSregpq = INT[2 ^16 +θ *2 ^15]
	u32 i;
	float err;
	
	i=MRR(Get_ADDR_UARTnRx(PORTn)+12,4);
	if(i&0x80000000)
	{
		i=~i;
		i++;
		err=i;
		err*=-1;
	}
	else
	{
		err=i;
	}
	err/=1000000;//输入时4位小数+err%的2位小数
	err=(-err)/1.732;
	if(err>=0)
	{
		err*=(1<<15);
	}
	else
	{
		err*=(1<<15);
		err+=(1<<16);
	}
	i=(u32)err;
	i+=MRR(ADDR_Q1,2);
	MWR(i,ADDR_Q1,2);
	MWR(i,ADDR_Q2,2);
	MWR(i,ADDR_Q3,2);
}

void ADJ_ERR10Ib5(u32 PORTn,u32 ADDR_P,u32 ADDR_Q,u32 ADDR_PL,u32 ADDR_QL)//5%Ib 1.0 误差调整
{
	
	
	
}




void DL645_07_Factory_ATT7022(u32 PORTn)//Meter07_工厂模式:校验数据写,初始化数据读写
{
	u8 * p8Rx;
	

	if(Comm_Ram->Factory!=0x55)//0x55=工厂状态
	{
		return;
	}
	p8Rx=(u8 *)(Get_ADDR_UARTnRx(PORTn));

//相别：0=合,1=A,2=B,3=C,4=N,5上用于其他工厂设置(初始化数据读写)
	if(p8Rx[10]<=4)
	{//相别<=4
//校验分类:0=空,1=1.0误差,2=0.5L误差,3=,4=,5=,6=,7=时钟日误差,8=晶体频率,9=温度,10=,11=,12=电压设置,13=电流设置,14=零电流值时偏移,15=零电压值时偏移,16=零功率值时偏移,17=起潜动设置,18=校验初始化	
		switch(p8Rx[11])
		{
			case 1://1.0误差
				switch(p8Rx[10])//当前校验相
				{
					case 1://A
						ADJ_ERR10(PORTn,ADDR_EMU_04,ADDR_EMU_07,ADDR_EMU_0A);//1.0 Ib误差调整
						break;
					case 2://B
						ADJ_ERR10(PORTn,ADDR_EMU_05,ADDR_EMU_08,ADDR_EMU_0B);//1.0 Ib误差调整
						break;
					case 3://C
						ADJ_ERR10(PORTn,ADDR_EMU_06,ADDR_EMU_09,ADDR_EMU_0C);//1.0 Ib误差调整
						break;
				}
				break;
			case 2://0.5L误差
				switch(p8Rx[10])//当前校验相
				{
					case 1://A
						ADJ_ERR05L(PORTn,ADDR_EMU_0D,ADDR_EMU_10,ADDR_EMU_61);//05L Ib误差调整
						break;
					case 2://B
						ADJ_ERR05L(PORTn,ADDR_EMU_0E,ADDR_EMU_11,ADDR_EMU_62);//05L Ib误差调整
						break;
					case 3://C
						ADJ_ERR05L(PORTn,ADDR_EMU_0F,ADDR_EMU_12,ADDR_EMU_63);//05L Ib误差调整
						break;
				}
				break;
			case 3://基波1.0误差
				break;
			case 4://5%Ib 1.0 误差调整
				switch(p8Rx[10])//当前校验相
				{
					case 1://A
						
						break;
					case 2://B
						
						break;
					case 3://C
						
						break;
				}
				break;
			
			case 6://直流模拟量校准
				break;
			case 9://温度校验
				ADJ_TEMP(PORTn);//温度校验
				break;
			case 10://有功功率校验
				switch(p8Rx[10])//当前校验相
				{
					case 0://合
						ADJ_P(PORTn,0x01,ADDR_EMU_04,ADDR_EMU_07,ADDR_EMU_0A);//功率增益校正
						ADJ_P(PORTn,0x02,ADDR_EMU_05,ADDR_EMU_08,ADDR_EMU_0B);//功率增益校正
						ADJ_P(PORTn,0x03,ADDR_EMU_06,ADDR_EMU_09,ADDR_EMU_0C);//功率增益校正
						break;
					case 1://A
						ADJ_P(PORTn,0x01,ADDR_EMU_04,ADDR_EMU_07,ADDR_EMU_0A);//功率增益校正
						break;
					case 2://B
						ADJ_P(PORTn,0x02,ADDR_EMU_05,ADDR_EMU_08,ADDR_EMU_0B);//功率增益校正
						break;
					case 3://C
						ADJ_P(PORTn,0x03,ADDR_EMU_06,ADDR_EMU_09,ADDR_EMU_0C);//功率增益校正
						break;
				}
				break;
			
			case 12://电压校验
				switch(p8Rx[10])//当前校验相
				{
					case 0://合
						ADJ_U(PORTn,0x0d,ADDR_EMU_17);//电压增益校正
						ADJ_U(PORTn,0x0e,ADDR_EMU_18);//电压增益校正
						ADJ_U(PORTn,0x0f,ADDR_EMU_19);//电压增益校正
						break;
					case 1://A
						ADJ_U(PORTn,0x0d,ADDR_EMU_17);//电压增益校正
						break;
					case 2://B
						ADJ_U(PORTn,0x0e,ADDR_EMU_18);//电压增益校正
						break;
					case 3://C
						ADJ_U(PORTn,0x0f,ADDR_EMU_19);//电压增益校正
						break;
				}
				break;
			case 13://电流校验
				switch(p8Rx[10])//当前校验相
				{
					case 0://合
						ADJ_I(PORTn,0x10,ADDR_EMU_1A);//电流增益校正
						ADJ_I(PORTn,0x11,ADDR_EMU_1B);//电流增益校正
						ADJ_I(PORTn,0x12,ADDR_EMU_1C);//电流增益校正
						break;
					case 1://A
						ADJ_I(PORTn,0x10,ADDR_EMU_1A);//电流增益校正
						break;
					case 2://B
						ADJ_I(PORTn,0x11,ADDR_EMU_1B);//电流增益校正
						break;
					case 3://C
						ADJ_I(PORTn,0x12,ADDR_EMU_1C);//电流增益校正
						break;
				}
				break;
			case 14://零电流值时偏移
				switch(p8Rx[10])//当前校验相
				{
					case 0://合
						ADJ_Offset(0x10,ADDR_EMU_27);
						ADJ_Offset(0x11,ADDR_EMU_28);
						ADJ_Offset(0x12,ADDR_EMU_29);
						ADJ_Offset(0x13,ADDR_EMU_6A);//电流矢量和(ItRms)
						break;
					case 1://A
						ADJ_Offset(0x10,ADDR_EMU_27);
						break;
					case 2://B
						ADJ_Offset(0x11,ADDR_EMU_28);
						break;
					case 3://C
						ADJ_Offset(0x12,ADDR_EMU_29);
						break;
				}
				break;
			case 15://零电压值时偏移
				switch(p8Rx[10])//当前校验相
				{
					case 1://A
						ADJ_Offset(0x0d,ADDR_EMU_24);
						break;
					case 2://B
						ADJ_Offset(0x0e,ADDR_EMU_25);
						break;
					case 3://C
						ADJ_Offset(0x0f,ADDR_EMU_26);
						break;
				}
				break;
			case 16://

				break;
			case 17://起潜动检查
				
				break;
			case 18://校验初始化
				MW((u32)MeterCalInitLib,ADDR_MeterCalibration_IEEPROM_START,ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START);
				break;
			default:
				return;
		}
		Meter_Ram->EMUParaCount=0;
    Meter_Ram->EMUInit=4;//0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
		DL645_07_WriteOKACK(PORTn);//正确写回答
	}
	else
	{//相别>4
//相别：0=合,1=A,2=B,3=C,4=N,5上用于其他工厂设置(初始化数据读写)	
	
	
	}
}

#endif//#if IC_ATT7022E==1//0=没,1=有






























