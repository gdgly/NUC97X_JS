
//ERC事件
#include "Project.h"
#include "Terminal_AFN0E_Event.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "../Terminal/DataConversion_SH698.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_Uart_AFN04_Paramter.h"
#include "../terminal/Terminal_USER5xx.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"



extern const u32 DL645_2007_RMLib[];

void Init_Class3Data_UpPower(void)//类3数据(事件计算）初始化(上电)
{
	u32 i;
	u8 * p8;

#if ERC22_TNum!=0
	u32 Pn;
	u16 * p16;
//ERC22差动越限事件
	//清总加电量,原差动组超限标志不清
	MC(0,ADDR_ERC22_TotalEnergy,(ADDR_ERC22_SOUR)-(ADDR_ERC22_TotalEnergy));
	//初始化区间定时
	p16=(u16 *)(ADDR_ERC22_Timer);
	for(Pn=0;Pn<ERC22_TNum;Pn++)
	{
    i=MRR(ADDR_AFN04F15+(Pn*9)+3,1);//取参与差动的电能量的时间区间及对比方法标志
		i&=3;
		switch(i)
		{
			case 0:
				i=60*60;
				break;
			case 1:
				i=30*60;
				break;
			default:
				i=15*60;
				break;
		}
		p16[Pn]=i;
	}
#endif
//ERC9电流回路异常事件
	MC(0xee,ADDR_ERC9_SOUR,LEN_ERC9_SOUR_Pn*ERC9MaxPn);
	p8=(u8 *)(ADDR_ERC9_SOUR);
	for(i=0;i<ERC9MaxPn;i++)
	{
		p8[0]=0;
		p8+=LEN_ERC9_SOUR_Pn;
	}
//ERC10电压回路异常事件
	MC(0xee,ADDR_ERC10_SOUR,LEN_ERC10_SOUR_Pn*ERC10MaxPn);
	p8=(u8 *)(ADDR_ERC10_SOUR);
	for(i=0;i<ERC10MaxPn;i++)
	{
		p8[0]=0;
		p8[1]=0;
		p8+=LEN_ERC10_SOUR_Pn;
	}
//ERC29电能表飞走,超差事件
	//清定时开始时电量值,恢复产生标志不清
	p8=(u8 *)(ADDR_ERC29_SOUR);
	for(i=0;i<ERC29MaxPn;i++)
	{
		p8[0]=0xee;
		p8[19]=0xee;
		p8+=LEN_ERC29_SOUR;
	}
//ERC30电能表停走事件
	//清定时开始时电量值,恢复产生标志不清
	p8=(u8 *)(ADDR_ERC30_SOUR);
	for(i=0;i<ERC30MaxPn;i++)
	{
		p8[0]=0xee;
		p8+=LEN_ERC30_SOUR;
	}
//ERC37ERC38电能表开表盖端盖事件记录原始数据
	MC(0xee,ADDR_ERC37_38_SOUR,LEN_ERC37_38_SOUR*ERC37MaxPn);
//ERC40磁场异常事件记录
	//清发生总次数为无效
	p8=(u8 *)(ADDR_ERC40_SOUR+1);
	for(i=0;i<ERC40MaxPn;i++)
	{
		p8[2]=0xee;
		p8[3]=0xee;
		p8[4]=0xee;
		p8+=LEN_ERC40_SOUR;
	}
#if (USER/100)==5//用户标识:上海
	//清电能表负荷开关误动作为0xee
	MC(0xee,ADDR_ERC68_SOUR,3*MaxRS485AddCarrierPn);
	//显示用失压全失压数据(上电时清为0xee)
	MC(0xee,ADDR_AFN0CF26_Ext,LEN_AFN0CF26_Ext*MaxRS485AddCarrierPn);
	//ERC41直接读电能表对时事件记录
	MC(0xee,ADDR_ERC41_SOUR,LEN_ERC41_SOUR*MaxRS485AddCarrierPn);
#endif
}
void Init_Class3Data_Command(void)//类3数据(事件)初始化(命令,全部清除)
{
	//事件计数和事件标志状态
	MC(0x0,ADDR_AFN0CF7_FM,8);
  MC(0x0,ADDR_AFN0CF8,8);
	//重要事件等待访问标志
	MC(0x0,ADDR_ADCFlags,3);
	//事件记录
	MC(0x0,ADDR_AFN0EF1,LEN_TERMINAL_EVENT*256);
	MC(0x0,ADDR_AFN0EF2,LEN_TERMINAL_EVENT*256);
	MC(0x0,ADDR_AFN0EF3,MaxERC*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)));
	//抄表标志
	MC(0x0,ADDR_ERC21_Flag,1);//ERC21 485抄表故障事件记录标志
	MC(0x0,ADDR_RM_CountFlags+32,MaxRS485AddCarrierPn/4);//每表用2位原电能表抄表标志(0=成功,1=失败->成功,2=成功->失败,3=失败)
	//电能表事件计数
	MC(0x0,ADDR_MeterEventCount_FM,8);
	//电能表主动上报状态字
	MC(0x0,ADDR_AutoReportWord,LEN_AutoReportWord*MaxRS485AddCarrierPn);
	//电能表各事件原发生次数
	MC(0xff,ADDR_MeterEventCount,LEN_MeterEventCount*MaxRS485AddCarrierPn);

	Init_Class3Data_UpPower();//类3数据(事件计算)初始化(上电)
}

void Init_Class3Data(u32 Pn)//类3数据(事件)初始化(指定的测量点数据,Pn=1~n)
{
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	Pn--;
	//电能表主动上报状态字
	MC(0,ADDR_AutoReportWord+(LEN_AutoReportWord*Pn),LEN_AutoReportWord);
	//电能表各事件原发生次数
	MC(0xff,ADDR_MeterEventCount+(LEN_MeterEventCount*Pn),LEN_MeterEventCount);
}

u32 ERC_Event_Set(u32 EventNo)//检查事件记录设置,返回0=不记录,1=记录
{
	u64 d64;
	u64 d64b;
	
	EventNo--;
	if(EventNo>63)
	{//ERC非法
		return 0;
	}
	d64=1;
	d64<<=EventNo;
	d64b=MRR(ADDR_AFN04F9,8);
#if (USER/100)==2//用户标识:山东版;ERC14终端停/上电事件终端停电时应立即向主站上报停电事件(无论终端的"终端停/上电事件"是否设置为重要事件)
	d64b|=1<<(14-1);
#endif
	if((d64&d64b)==0x0)
	{//不需记录
		return 0;
	}
	return 1;
}

u32 ERC_Event_Save_Re(u32 BuffAddr)//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
{
	u32 i;
	u32 x;
	u64 d64;
	u64 d64b;
	u8 * p8s;

	p8s=(u8 *)(BuffAddr);
	i=p8s[0];
	i-=1;
	if(i>63)
	{//ERC非法
	#if (USER/100)==5//用户标识:上海系
		if(i==(68-1))
		{
			d64=0;
			goto save;//ERC68需记录为重要事件
		}
	#endif
		return 0;
	}
	d64=1;
	d64<<=i;
	d64b=MRR(ADDR_AFN04F9,8);
#if (USER/100)==2//用户标识:山东版;ERC14终端停/上电事件终端停电时应立即向主站上报停电事件(无论终端的"终端停/上电事件"是否设置为重要事件)
	d64b|=1<<(14-1);
#endif
	if((d64&d64b)==0x0)
	{//不需记录
		return 0;
	}
	d64b=MRR(ADDR_AFN04F9+8,8);
#if (USER/100)==2//用户标识:山东版;ERC14终端停/上电事件终端停电时应立即向主站上报停电事件(无论终端的"终端停/上电事件"是否设置为重要事件)
	d64b|=1<<(14-1);
#endif
	if((d64&d64b)!=0x0)
	{//重要事件
	#if (USER/100)==5//用户标识:上海系
	save://ERC68需记录为重要事件
	#endif
		i=MRR(ADDR_AFN0CF7_FM,1);//重要事件计数
		if(p8s[0]!=14)
		{//不是停上电事件
			MW(BuffAddr,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),p8s[1]+2);
		}
		else
		{//是停上电事件,多少1个字节上电记录标志
		#if (USER/100)==7//用户标识:浙江系
			MW(BuffAddr,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),p8s[1]+2+2);
		#else
			MW(BuffAddr,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),p8s[1]+2+1);
		#endif
		}
		//重要事件计数+1
		i+=1;
    MWR(i,ADDR_AFN0CF7_FM,1);
		//重要事件等待访问标志ADC=1
		MWR(0x010101,ADDR_ADCFlags,3);
		
		if(MRR(ADDR_AFN05F29,1)==0x55)
		{//允许终端主动上报
			i=MRR(ADDR_AFN0CF7_FM+2,1);//GPRS当前还没上传被确认重要事件计数+1
			if(i!=0xff)
			{
				i+=1;
				MWR(i,ADDR_AFN0CF7_FM+2,1);
			}
			i=MRR(ADDR_AFN0CF7_FM+4,1);//以太网当前还没上传被确认重要事件计数+1
			if(i!=0xff)
			{
				i+=1;
				MWR(i,ADDR_AFN0CF7_FM+4,1);
			}
		#if RS232AutoRePort==1//RS232口主动上报0=禁止,1=允许
			i=MRR(ADDR_AFN0CF7_FM+6,1);//RS232当前还没上传被确认重要事件计数+1
			if(i!=0xff)
			{
				i+=1;
				MWR(i,ADDR_AFN0CF7_FM+6,1);
			}
		#endif
		}
		else
		{//禁止终端主动上报
			MC(0,ADDR_AFN0CF7_FM+2,6);//清当前还没上传被确认重要事件计数,当前有?个重要事件主动上传需确认
		}
	}
	else
	{//一般事件
		i=MRR(ADDR_AFN0CF7_FM+1,1);//一般事件计数
		if(p8s[0]!=14)
		{//不是停上电事件
			MW(BuffAddr,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),p8s[1]+2);
		}
		else
		{//是停上电事件,多少1个字节上电记录标志
			MW(BuffAddr,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),p8s[1]+2+1);
		}
		i+=1;//一般事件计数+1
		MWR(i,ADDR_AFN0CF7_FM+1,1);
	}
	//AFN0CF8事件标志
	d64b=MRR(ADDR_AFN0CF8,8);
	d64b|=d64;
	MWR(d64b,ADDR_AFN0CF8,8);
	
	d64=0;
		//重要事件标志
	for(i=0;i<256;i++)
	{
		x=MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),1);//ERC号
		x-=1;
		if(x<=63)
		{
			d64b=1;
			d64b<<=x;
			d64|=d64b;
		}
	}
		//一般事件标志
	for(i=0;i<256;i++)
	{
		x=MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),1);//ERC号
		x-=1;
		if(x<=63)
		{
			d64b=1;
			d64b<<=x;
			d64|=d64b;
		}
	}
	d64b=MRR(ADDR_AFN0CF8,8);
	d64b&=d64;
	MWR(d64b,ADDR_AFN0CF8,8);
	
	PerERC_Event_Save(BuffAddr);//分类ERC事件记录
	return 1;
}


#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则

u32 Get_ADDR_UARTnRMEvent(u32 PORTn)//得到UART抄读电能表事件栈结构地址
{
	switch(PORTn)
	{
		case RS485_1PORT:
			return ADDR_UARTRMEvent_4851;
		case RS485_2PORT:
			return ADDR_UARTRMEvent_4852;
		case RS485_3PORT:
			return ADDR_UARTRMEvent_4853;
		case RS485_4PORT://(载波)
			return ADDR_UARTRMEvent_4854;
	}
	return 0;
}

u32 Check_Meter_Event_Save(void)//检查电能表事件存贮空间;返回0=存储滿(没上报),1-255允许还可存储的条数
{
	u32 x;
	u32 y;
	
	x=MRR(ADDR_MeterEventCount_FM+2,4);
	y=x;
	x=(x&0xff)+((x>>8)&0xff);//GPRS当前还没上传被确认电能表事件计数+当前有?个电能表事件主动上传需确认
	y>>=16;
	y=(y&0xff)+((y>>8)&0xff);//以太网当前还没上传被确认电能表事件计数+当前有?个电能表事件主动上传需确认
	if(x>y)
	{
		x=y;
	}
	if(x>=255)
	{
		return 0;
	}
	x=255-x;
	return x;
}

void Meter_Event_WriteBuff(u32 PORTn,u32 Pn,u32 Addr_FnDataBuff,u8 *p8RMLib)//读取的电能表事件写入缓冲ADDR_DATABUFF
{
	u32 i;
	u32 x;
	u32 y;
	
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

	switch(PORTn)
	{
		case ACSAMPLEPORT:
			i=1;
			break;
		case RS485_1PORT://RS485-1
			i=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			i=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			i=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			i=31;
			break;
		default:
			i=0;
			break;
	}
	MWR(i,ADDR_DATABUFF,1);//1byte 端口号
	MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE 通信地址
	if(((UARTCtrl->RMCount&1)==0)&&(UARTCtrl->RMDI!=0xffffffff))
	{//非事件类数据
		MWR(0,ADDR_DATABUFF+7,1);//发生次数长度=0
	}
	else
	{//事件
		MWR(7,ADDR_DATABUFF+7,1);//发生次数长度
		//取读次数的DI	
		i=(u32)p8RMLib;
		i+=10+(5*(UARTCtrl->RMCount-1));
		MR(ADDR_DATABUFF+10,i,4);
		//取当前次数值
		i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2)),3);
		i=hex_bcd(i);
		MWR(i,ADDR_DATABUFF+14,3);
		//次数数据标识多于3字节的
		i=MRR(((u32)p8rxbuff)+512+2,4);//取寄存的数据标识
		switch(i)
		{
			case 0x03120101://B70需量超限(正向有功)
			case 0x03120201://B70需量超限(反向有功)
			case 0x03120301://B70需量超限(1象限无功)
			case 0x03120401://B70需量超限(2象限无功)
			case 0x03120501://B70需量超限(3象限无功)
			case 0x03120601://B70需量超限(4象限无功)
				MWR(22,ADDR_DATABUFF+7,1);//发生次数长度
				i>>=8;
				i&=0xff;
				i--;
				x=i;
				for(i=0;i<6;i++)
				{
					y=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*((UARTCtrl->RMCount/2)-x))+(4*i),3);
					y=hex_bcd(y);
					MWR(y,ADDR_DATABUFF+14+(i*3),3);
				}
				break;
			case 0x03060001://B68辅助电源失电
				MWR(10,ADDR_DATABUFF+7,1);//发生次数长度
				MR(ADDR_DATABUFF+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
				break;
			case 0x03050001://B74全失压
				MWR(10,ADDR_DATABUFF+7,1);//发生次数长度
				MR(ADDR_DATABUFF+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
				break;
		}
	}
	//
	i=p8FnData[0]+(p8FnData[1]<<8);
	MWR(i,ADDR_DATABUFF+8,2);//2byte长度
	MR(ADDR_DATABUFF+32,((u32)p8rxbuff)+512,i);//数据
}

void Meter_Event_Save(u32 Addr_Buff)//直接存贮读取的电能表事件
{	
	u32 i;
	
	i=MRR(ADDR_MeterEventCount_FM,1);//重要事件计数
	MW(Addr_Buff,ADDR_MeterEvent+(LEN_MeterEvent*i),LEN_MeterEvent);
	//事件计数+1
	i+=1;
  MWR(i,ADDR_MeterEventCount_FM,1);
	
	i=MRR(ADDR_MeterEventCount_FM+2,1);//GPRS当前还没上传被确认重要事件计数+1
	if(i!=0xff)
	{
		i+=1;
		MWR(i,ADDR_MeterEventCount_FM+2,1);
	}
	i=MRR(ADDR_MeterEventCount_FM+4,1);//以太网当前还没上传被确认重要事件计数+1
	if(i!=0xff)
	{
		i+=1;
		MWR(i,ADDR_MeterEventCount_FM+4,1);
	}
#if RS232AutoRePort==1//RS232口主动上报0=禁止,1=允许
	i=MRR(ADDR_MeterEventCount_FM+6,1);//RS232当前还没上传被确认重要事件计数+1
	if(i!=0xff)
	{
		i+=1;
		MWR(i,ADDR_MeterEventCount_FM+6,1);
	}
#endif
}

void WriteAutoReportWord(u32 PORTn,u32 rxbuff)//写电能表主动上报状态字;入口:端口号(1-31),通信地址,自动上报状态字地址
{
#if ((USER/100)==5)//上海用户
#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
	u32 i;
	u32 y;
	u32 z;
	u32 f;
	u32 MeterNo;
	u8 *p8s;
	u8 *p8d;
	u64 CommAddr;
	
	f=0;//没对应事件标志
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1;
			break;
		case RS485_1PORT://RS485-1
			PORTn=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			PORTn=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			PORTn=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			PORTn=31;
			break;
		default:
			return;
	}
	CommAddr=MRR(rxbuff+1,6);
	MeterNo=Check_AFN04F10_Addr(PORTn,CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
	if(MeterNo==0)
	{
		return;//没找到
	}
	MeterNo--;
	//当前自动上报状态字转为按位相同偏移的新增次数存储
	MR(ADDR_DATABUFF,ADDR_AutoReportWord+(MeterNo*LEN_AutoReportWord),96);
	for(i=0;i<96;i++)
	{
		if((i!=3)&&(i!=8))
		{//bit3时钟电池电压低、bit8停电抄表电池欠压不清
			MC(0,ADDR_DATABUFF+i,1);
		}
	}
	p8s=(u8*)(rxbuff);
	p8d=(u8*)(ADDR_DATABUFF);
	if(p8s[9]<=(4+12+2))
	{//长度错或老格式(只有10字节状态字)
		return;
	}
	y=p8s[14];
	if(y&0x20)
	{//bit5
		y=1;//没发生次数的为0xff
		f=1;//没对应事件标志
		p8d[5]=y;
	}
	if(y&0x80)
	{//bit7
		y=1;//没发生次数的为0xff
		f=1;//没对应事件标志
		p8d[7]=y;
	}
	//数据或
	z=0;
	for(i=0;i<96;i++)
	{
		z|=p8d[i];
	}
	p8d[i]=z;
	//12byte 主动上报事件的位标志寄存,用于发复位命令
	MR(ADDR_DATABUFF+96+1,rxbuff+14,12);
	//写
	MW(ADDR_DATABUFF,ADDR_AutoReportWord+(MeterNo*LEN_AutoReportWord),96+1+12);
	//没对应事件写主动上报状态字作为事件
	if(f!=0)
	{
		MWR(PORTn,ADDR_DATABUFF,1);//1byte 端口号
		MR(ADDR_DATABUFF+1,rxbuff+1,6);//6BYTE 通信地址
		MWR(0,ADDR_DATABUFF+7,1);//发生次数长度=0
		//
		i=p8s[9];
		MWR(i,ADDR_DATABUFF+8,2);//2byte长度
		MR(ADDR_DATABUFF+32,rxbuff+10,i);//数据
		Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
	}
#endif
#endif

#if EnMeterAutoAutoReport==1//允许电能表主动上报事件:0=不允许,1=允许
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 f;
	u32 MeterNo;
	u8 *p8s;
	u8 *p8d;
	u64 CommAddr;
	
	f=0;//没对应事件标志
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1;
			break;
		case RS485_1PORT://RS485-1
			PORTn=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			PORTn=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			PORTn=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			PORTn=31;
			break;
		default:
			return;
	}
	CommAddr=MRR(rxbuff+1,6);
	MeterNo=Check_AFN04F10_Addr(PORTn,CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
	if(MeterNo==0)
	{
		return;//没找到
	}
	MeterNo--;
	//当前自动上报状态字转为按位相同偏移的新增次数存储
	MC(0,ADDR_DATABUFF,96);
	z=0;//己有置位计数
	p8s=(u8*)(rxbuff);
	p8d=(u8*)(ADDR_DATABUFF);
	if(p8s[9]<=(4+12+2))
	{//长度错或老格式(只有10字节状态字)
		return;
	}
	for(i=0;i<12;i++)
	{
		for(x=0;x<8;x++)
		{
			y=p8s[14+i];
			y&=(1<<x);
			if(y!=0)
			{//有新增次数
				y=p8s[14+12+1+z];
				if(y==0xff)
				{
					y=10;//没发生次数的为0xff
				}
				if((((i*8)+x)>=1)&&(((i*8)+x)<=9))
				{//B1-B9没对应事件
					y=1;//没发生次数的为0xff
					f=1;//没对应事件标志
				}
				p8d[(i*8)+x]=y;
				z++;//己有置位计数
			}
		}
	}
	//数据或
	z=0;
	for(i=0;i<96;i++)
	{
		z|=p8d[i];
	}
	p8d[i]=z;
	//12byte 主动上报事件的位标志寄存,用于发复位命令
	MR(ADDR_DATABUFF+96+1,rxbuff+14,12);
	//写
	MW(ADDR_DATABUFF,ADDR_AutoReportWord+(MeterNo*LEN_AutoReportWord),96+1+12);
	//没对应事件写主动上报状态字作为事件
	if(f!=0)
	{
		MWR(PORTn,ADDR_DATABUFF,1);//1byte 端口号
		MR(ADDR_DATABUFF+1,rxbuff+1,6);//6BYTE 通信地址
		MWR(0,ADDR_DATABUFF+7,1);//发生次数长度=0
		//
		i=p8s[9];
		MWR(i,ADDR_DATABUFF+8,2);//2byte长度
		MR(ADDR_DATABUFF+32,rxbuff+10,i);//数据
		Meter_Event_Save(ADDR_DATABUFF);//直接存贮读取的电能表事件
	}
#endif
}
u32 Check_AutoReportWord(u32 PORTn)//检查电能表主动上报状态字;返回:0=无,!=0 B31-B16为配置序号(1开始),B15-B0为要读取的主动上报状态字位号(0-95)
{
#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
	return 0;
#else
	u32 x;
	u8 *p8s;
	u8 *p8d;
	u32 Meter;
	u32 port;
	
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			port=1;
			break;
		case RS485_1PORT://RS485-1
			port=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			port=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			port=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			port=31;
			break;
		default:
			port=0;
			break;
	}
//ADDR_AFN04F10读入内存
	#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
		#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
	#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	p8s=(u8*)(ADDR_128KDATABUFF);
	for(Meter=0;Meter<AFN04F10MaxPn;Meter++)
	{
		x=p8s[2]+(p8s[3]<<8);
		if(x!=0)
		{//测量点有效
			x=p8s[4];
			x&=0x1f;
			if(x==port)
			{//端口号相同
				//p8d=(u8*)(ADDR_AutoReportWord+(Meter*LEN_AutoReportWord));
			#if LEN_AutoReportWord>LEN_DATABUFF
				#error LEN_AutoReportWord>LEN_DATABUFF
			#endif
				MR(ADDR_DATABUFF,ADDR_AutoReportWord+(Meter*LEN_AutoReportWord),LEN_AutoReportWord);
				p8d=(u8*)(ADDR_DATABUFF);
				if(p8d[96]!=0)
				{//数据或的值!=0
					for(x=0;x<96;x++)
					{
						if(p8d[x]!=0)
						{
							return (((Meter+1)<<16)+x);
						}
					}
					//全0,清数据或值字节
					//MC(0,((u32)p8d)+96,1);
					MC(0,ADDR_AutoReportWord+(Meter*LEN_AutoReportWord)+96,1);
				}
			}
		}
		p8s+=LEN_AFN04F10_Pn;
	}
	return 0;
#endif
}

u32 RMEvene_YesNo(u32 PORTn,u32 Pn,u8 * p8RMLib,u32 RMCount,u32 Protocol)//按分类分级事件抄读的Fn是否要抄;返回0=不抄,1=要抄;Pn,p8RMLib抄表库指针,RMCount抄表计数,Protocol(1=1997,其他=2007)
{
#if ((USER/100)==5)//上海
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(Protocol==1)
	{
		return 0;
	}
	if(p8RMLib[0]!=3)
	{
		return 0;
	}
	if(Pn==0)
	{
		return 0;
	}
	Pn--;
	if((RMCount%3)==0)
	{//读次数
	#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
MeterEvent://若抄主动上报状态字，则发送复位主动上报状态字
	#endif
		//取DI	
		x=(u32)p8RMLib;
		x+=10+(5*RMCount);
		x=MRR(x,4);
		if(Protocol==3)
		{
			x=Get_Di_From_OAD(x);
		}
		if(x==0)
		{//保留
			return 0;
		}
		x>>=8;//只比较高3字节,使读次数和读事件命令相同
	#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
//		//2014年12月26日供货要求
//		//数据分级归类参数根据测量点小类号默认：单相表为1，三相表为2
//		y=Check_AFN04F10_Pn(Pn+1);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
//		i=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+26,1);//取大小类号
//		switch(i)
//		{
//			case 0x51://单相低压用户
//			case 0x71://单相分布式发电用户
//				i=1;
//				break;
//			case 0x52://三相低压用户
//			case 0x72://三相分布式发电用户
//			case 0x02://专变用户
//			case 0x62://公用配变考核计量点
//				i=2;
//				break;
//			default://其他按大类号5小类号1处理
//				i=1;
//				break;
//		}

//智能电能表及采集终端事件记录采集规则20150122.doc
//				表2 根据大小类默认配置事件分级表
//			F105参数预设表（参数区初始化后需保持默认配置）
//终端类型	   专变终端	                         集中器
//抄表端口	   485端口	               485端口	                          载波端口
//用户类型		总表	         总表	        户表	                    户表
//电能表类型	   三相表	        三相表  	三相表  单相表             三相表  单相表
//大小类号  						  		 								0-2
//			不区分大小类		 0-2		 5-2	 其他				6-2		其他
//								 6-2		 7-2						5-2
//																		7-2
//F106类别标识	1类		 	 	 1类		 2类	  3类				4类		 5类
//注：F106类别数据项中存在09版智能电能表不支持的部分，终端需根据电能表回复命令否定加以区分。

		//取端口号
		y=Check_AFN04F10_Pn(Pn+1);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
		i=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+4,1);
		i&=0x1f;
		if((i==2) || (i==3))
		{//RS485-I口、RS485-II口
			//取大小类号
			i=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+26,1);
			switch(i)
			{
				case 0x02://专变用户
				case 0x62://公用配变考核计量点
					i=1;//取分级类号
					break;
				case 0x52://三相低压用户
				case 0x72://三相分布式发电用户
					i=2;//取分级类号
					break;
				default://其他
					i=3;//取分级类号
					break;
			}
		}
		else
		{
			if(i==31)
			{//载波口
				//取大小类号
				i=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+26,1);
				switch(i)
				{
					case 0x02://专变用户
					case 0x62://公用配变考核计量点
					case 0x52://三相低压用户
					case 0x72://三相分布式发电用户
						i=4;//取分级类号
						break;
					default://其他
						i=5;//取分级类号
						break;
				}
			}
			else
			{
				return 0;
			}
		}
	#else//#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
		//p8=(u8*)(ADDR_AFN04F105+Pn);
		//i=p8[0];//取分级类号
		i=MRR(ADDR_AFN04F105+Pn,1);//取分级类号
	#endif
		if((i==0)||(i>MaxMeterEventClass))//最大电能表事件分级类号
		{
			return 0;
		}
		i--;
	#if (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
		#error (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
	#endif
		//p8=(u8*)(ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)));
		MR(ADDR_DATABUFF,ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)),(5*NUM_AFN04F106_PerC));
		p8=(u8*)(ADDR_DATABUFF);
		for(i=0;i<NUM_AFN04F106_PerC;i++)
		{
			y=p8[4];
			if(y!=0)
			{//电能表数据等级BIN 1 (0表示删除)
				if(y==UARTCtrl->RMEventTaskC)
				{//分级同
					y=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);
					y>>=8;//只比较高3字节,使读次数和读事件命令相同
					if(((x&0xff)==0xff)||((y&0xff)==0xff))
					{//块命令
						if((x>>8)==(y>>8))
						{//数据标识同
							return 1;
						}
					}
					else
					{
						if(y==x)
						{//数据标识同
							return 1;
						}
					}
				}
			}
			p8+=5;
		}
	}
	else
	{//读1-10次事件
	#if EnMeterAutoAutoReport==0//允许电能表主动上报事件:0=不允许,1=允许
		if(RMCount==((60)*3+1))
		{//复位主动上报状态字
			//若抄主动上报状态字，则发送复位主动上报状态字
			RMCount=((60)*3);
			goto MeterEvent;
		}
	#endif
		//取DI
		x=(u32)p8RMLib;
		x+=10+(5*RMCount);
		x=MRR(x,4);
		if(Protocol==3)
		{
			x=Get_Di_From_OAD(x);
		}
		if(x==0)
		{//保留
			return 0;
		}
		p8=(u8*)(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount));
		i=RMCount;
		i/=3;
		i*=4;
		p8+=i;
		i=MRR(((u32)p8)+3,1);
		if((i!=0)&&(i!=0xff))
		//if((p8[3]!=0)&&(p8[3]!=0xff))
		{//新增次数hex!=0同时!=0xff
			return 1;
		}
	}
#else//#if ((USER/100)==5)//上海
//不是上海
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(Protocol==1)
	{
		return 0;
	}
	if(p8RMLib[0]!=3)
	{
		return 0;
	}
	if(Pn==0)
	{
		return 0;
	}
	Pn--;
	if((RMCount&1)==0)
	{//读次数
		//取DI	
		x=(u32)p8RMLib;
		x+=10+(5*RMCount);
		x=MRR(x,4);
		if(x==0)
		{//保留
			return 0;
		}
		if(x==0xffffffff)
		{//DI=0XFFFFFFFF其他非事件类的数据在此定时抄读
			x=Search_AFN04F106_NoEvene_DI(PORTn,Pn);//按分类分级事件搜索AFN04F106设置中非事件类抄读;返回0=没找到,1=找到
			return x;
		}
		//
		UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
		UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
		UARTCtrl->RMDI=0xffffffff;//无效

		//取分级类号
//		p8=(u8*)(ADDR_AFN04F105+Pn);
//		i=p8[0];
		i=MRR(ADDR_AFN04F105+Pn,1);
		if((i==0)||(i>MaxMeterEventClass))//最大电能表事件分级类号
		{
			return 0;
		}
		i--;
	#if (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
		#error (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
	#endif
		//p8=(u8*)(ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)));
		MR(ADDR_DATABUFF,ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)),(5*NUM_AFN04F106_PerC));
		p8=(u8*)(ADDR_DATABUFF);
		for(i=0;i<NUM_AFN04F106_PerC;i++)
		{
			y=p8[4];
			if(y!=0)
			{//电能表数据等级BIN 1 (0表示删除)
				if(y==UARTCtrl->RMEventTaskC)
				{//分级同
					y=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);
					if(y==x)
					{//数据标识同
						return 1;
					}
				}
			}
			p8+=5;
		}
	}
	else
	{//读1-10次事件
		p8=(u8*)(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount));
		i=RMCount;
		i/=2;
		i*=4;
		p8+=i;
		//if((p8[3]!=0)&&(p8[3]!=0xff))
		i=MRR(((u32)p8)+3,1);
		if((i!=0)&&(i!=0xff))
		{//新增次数hex!=0同时!=0xff
			Search_AFN04F106_Evene_DI(PORTn,Pn,RMCount);//按分类分级事件的当前事件抄读DI搜索AFN04F106设置中非发生次数的事件类抄读DI,返回:有设置UARTCtrl->RMDI=DI,没设置UARTCtrl->RMDI=0xffffffff
			return 1;
		}
	}
#endif
	return 0;
}

void Search_AFN04F106_Evene_DI(u32 PORTn,u32 Pn,u32 RMCount)//按分类分级事件的当前事件抄读DI搜索AFN04F106设置中非发生次数的事件类抄读DI,返回:有设置UARTCtrl->RMDI=DI,没设置UARTCtrl->RMDI=0xffffffff
{
	u32 i;
	u32 x;
	u32 DI;
	u32 NumDI;
	u32 AND;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->RMDI=0xffffffff;//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识

	//取DI
	x=DL645_2007_RMLib[3];
	x+=10+(5*RMCount);
	DI=MRR(x,4);
	if(DI==0)
	{//保留
		return;
	}
	if((DI&0xff00)==0xff00)
	{
		AND=0xffff0000;//与数据
	}
	else
	{
		AND=0xffffff00;//与数据
	}
	DI&=AND;
	NumDI=MRR(x-5,4);
	
	//p8=(u8*)(ADDR_AFN04F105+Pn);
	//i=p8[0];//取分级类号
	i=MRR(ADDR_AFN04F105+Pn,1);//取分级类号
	if((i==0)||(i>MaxMeterEventClass))//最大电能表事件分级类号
	{
		return;
	}
	i--;
	//p8=(u8*)(ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)));
	#if (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
		#error (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
	#endif
	MR(ADDR_DATABUFF,ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)),(5*NUM_AFN04F106_PerC));
	p8=(u8*)(ADDR_DATABUFF);
	i=UARTCtrl->SearchDICount;//周期事件抄读AFN04F106设置搜索计数
	if(i==0)
	{//搜索计数=0,同时统计总个数
		UARTCtrl->TotalDICount=0;//周期事件抄读AFN04F106设置中搜索到的DI总数(在SearchDICount=0时计算)
		for(;i<NUM_AFN04F106_PerC;i++)
		{
			x=p8[4];
			if(x!=0)
			{//电能表数据等级BIN 1 (0表示删除)
				if(x==UARTCtrl->RMEventTaskC)
				{//分级同
					x=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//取DI
					if(x!=NumDI)
					{//不是读次数DI
						x&=AND;
						if(DI==x)
						{//相同
							UARTCtrl->TotalDICount++;//周期事件抄读AFN04F106设置中搜索到的DI总数(在SearchDICount=0时计算)
							if(UARTCtrl->RMDI==0xffffffff)
							{
								UARTCtrl->SearchDICount=i;//周期事件抄读AFN04F106设置搜索计数
								x=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//取DI
								UARTCtrl->RMDI=x;//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
							}
						}
					}
				}
			}
			p8+=5;
		}
		if(UARTCtrl->RMDI==0xffffffff)
		{
			UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
			UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
		}
		return;
	}
	p8+=(i*5);
	for(;i<NUM_AFN04F106_PerC;i++)
	{
		x=p8[4];
		if(x!=0)
		{//电能表数据等级BIN 1 (0表示删除)
			if(x==UARTCtrl->RMEventTaskC)
			{//分级同
				x=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//取DI
				if(x!=NumDI)
				{//不是读次数DI
					x&=AND;
					if(DI==x)
					{//相同
						UARTCtrl->SearchDICount=i;//周期事件抄读AFN04F106设置搜索计数
						x=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//取DI
						UARTCtrl->RMDI=x;//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
						return;
					}
				}
			}
		}
		p8+=5;
	}
	UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
	UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
}
//事件抄读数据标识规律:
//1.次数DI的格式为xxxxxx00,则事件记录DI的格式为xxxxxx01,事件无分项抄读命令,无FF
//2.次数DI的格式为xxxxxx01,则事件记录DI的格式为xxxxFF01,,事件有分项抄读命令,有FF
u32 Search_AFN04F106_NoEvene_DI(u32 PORTn,u32 Pn)//按分类分级事件搜索AFN04F106设置中非事件类抄读;返回0=没找到,1=找到
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 DI;
	u8 *p8;
	u8 * p8RMLib;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->RMDI=0xffffffff;//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
	//p8=(u8*)(ADDR_AFN04F105+Pn);
	//i=p8[0];//取分级类号
	i=MRR(ADDR_AFN04F105+Pn,1);//取分级类号
	if((i==0)||(i>MaxMeterEventClass))//最大电能表事件分级类号
	{
		return 0;
	}
	i--;
	//p8=(u8*)(ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)));
	#if (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
		#error (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
	#endif
	MR(ADDR_DATABUFF,ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)),(5*NUM_AFN04F106_PerC));
	p8=(u8*)(ADDR_DATABUFF);
	i=UARTCtrl->SearchDICount;//周期事件抄读AFN04F106设置搜索计数
	if(i==0)
	{//搜索计数=0,同时统计总个数
		UARTCtrl->TotalDICount=0;//周期事件抄读AFN04F106设置中搜索到的DI总数(在SearchDICount=0时计算)
		for(;i<NUM_AFN04F106_PerC;i++)
		{
			x=p8[4];
			if(x!=0)
			{//电能表数据等级BIN 1 (0表示删除)
				if(x==UARTCtrl->RMEventTaskC)
				{//分级同
					DI=p8[2]+(p8[3]<<8);//取DI高2字节
					p8RMLib=(u8*)DL645_2007_RMLib[3];
					z=p8RMLib[1];//通信标识个数
					p8RMLib+=10;
					for(y=0;y<z;y++)
					{
						x=p8RMLib[2]+(p8RMLib[3]<<8);//取库中DI高2字节
						if(DI==x)
						{//相同
							break;
						}
						p8RMLib+=5;
					}
					if(y>=z)
					{//库中没相同,即非事件类抄读
						UARTCtrl->TotalDICount++;//周期事件抄读AFN04F106设置中搜索到的DI总数(在SearchDICount=0时计算)
						if(UARTCtrl->RMDI==0xffffffff)
						{
							UARTCtrl->SearchDICount=i;//周期事件抄读AFN04F106设置搜索计数
							DI=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//取DI
							UARTCtrl->RMDI=DI;//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
						}
					}
				}
			}
			p8+=5;
		}
		if(UARTCtrl->RMDI==0xffffffff)
		{
			UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
			UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
			return 0;
		}
		return 1;
	}
	p8+=(i*5);
	for(;i<NUM_AFN04F106_PerC;i++)
	{
		x=p8[4];
		if(x!=0)
		{//电能表数据等级BIN 1 (0表示删除)
			if(x==UARTCtrl->RMEventTaskC)
			{//分级同
				DI=p8[2]+(p8[3]<<8);//取DI高2字节
				p8RMLib=(u8*)DL645_2007_RMLib[3];
				z=p8RMLib[1];//通信标识个数
				p8RMLib+=10;
				for(y=0;y<z;y++)
				{
					x=p8RMLib[2]+(p8RMLib[3]<<8);//取库中DI高2字节
					if(DI==x)
					{//相同
						break;
					}
					p8RMLib+=5;
				}
				if(y>=z)
				{//库中没相同,即非事件类抄读
					UARTCtrl->SearchDICount=i;//周期事件抄读AFN04F106设置搜索计数
					DI=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//取DI
					UARTCtrl->RMDI=DI;//抄表数据标识(用于周期事件抄读),0xffffffff表示无效,其他值表示要抄的数据标识不是用库中的块抄标识和非事件类的抄读数据标识
					return 1;//要抄
				}
			}
		}
		p8+=5;
	}
	UARTCtrl->SearchDICount=0;//周期事件抄读AFN04F106设置搜索计数
	UARTCtrl->RMDICount=0;//周期事件抄读已抄DI计数
	return 0;//没要抄
}

#endif


#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
u32 ERC_Event_Save(u32 BuffAddr)//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
{
	u32 i;
	//若没记录上电事件则先记上电事件免冲掉原掉电事件
	if(MRR(ADDR_ERC14SOUR,1)==0)//ERC14事件记录标志(上电)
	{
		MWR(0x14,ADDR_ERC14SOUR,1);//ERC14事件记录标志
		if(MRR(ADDR_UpFlags,4)!=0x12345678)//4BYTE 启动标志:0x12345678为看门狗复位,否则为引脚等硬件复位
		{
			ERC14_Event_End();//终端停/上电事件(上电),用专用事件数据缓冲
		}
	}
	//记录事件
	i=ERC_Event_Save_Re(BuffAddr);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	return i;
}
#endif
#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
u32 ERC_Event_Save(u32 BuffAddr)//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
{
	u32 i;
	u32 x;

	//若没记录上电事件则先记上电事件免冲掉原掉电事件
	if(Terminal_Ram->PowerNote==0)//上电事件记录:0=没记录,!=0已记录;
	{
	//先查重要事件停电事件记录
		for(i=0;i<256;i++)
		{
			if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),1)==14)
			{//ERC=14,是停/上电事件
				if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+8,6)==0xeeeeeeeeeeee)
				{//上电记录标志=0xee
					MC(0,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+8+5,1);//上电记录标志=0
					x=MRR(ADDR_AFN0CF7_FM,1);
					if(i==x)
					{
						x++;
						MWR(x,ADDR_AFN0CF7_FM,1);
					}
				}
			}
		}
	//再查一般事件停电事件记录
		for(i=0;i<256;i++)
		{
			if(MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),1)==14)
			{//ERC=14,是停/上电事件
				if(MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i)+8,6)==0xeeeeeeeeeeee)
				{//上电记录标志=0xee
					MC(0,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i)+8+5,1);//上电记录标志=0
					x=MRR(ADDR_AFN0CF7_FM+1,1);
					if(i==x)
					{
						x++;
						MWR(x,ADDR_AFN0CF7_FM+1,1);
					}
				}
			}
		}
	}
	//记录事件
	i=ERC_Event_Save_Re(BuffAddr);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	return i;
}
#endif



extern  u8 ADDR_AFN09F1[];//终端版本信息
#if ERC1==1
void ERC1_Event(u32 f)//数据初始化和版本变更记录;f=1数据初始化,f=2版本变更,f=3数据初始化同时版本变更
{
	u8 * p8;

	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=1;//ERC
	p8[1]=14;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=f;
	MR(ADDR_DATABUFF+8,ADDR_AFN0CF1+12,4);
	if((f&0x2)==0x2)
	{//版本变更
		MR(ADDR_DATABUFF+12,((u32)ADDR_AFN09F1)+12,4);
	}
	else
	{//没版本变更
		MR(ADDR_DATABUFF+12,ADDR_AFN0CF1+12,4);
	}
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC2==1
void ERC2_Event(u32 f)//参数丢失记录;f=1终端参数丢失,f=2测量点参数丢失,f=3终端参数丢失同时测量点参数丢失
{
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=2;//ERC
	p8[1]=6;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=f;
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC3==1
void ERC3_Event(u32 rxbuff)//参数变更记录,入口接收缓冲首址
{
	u32 i;
	u32 x;
	u8 * p8;
	u8 * p8rxbuff;
	
	p8rxbuff=(u8 *)(rxbuff);
	i=0;
	while(i!=p8rxbuff[0])
	{
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=3;//ERC
		p8[1]=6;//长度Le(暂值,最小10)
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
		p8[7]=p8rxbuff[11]>>1;//启动站地址
		x=0;
		while((8+(4*x))<=LEN_TERMINAL_EVENT)
		{
			if(i==p8rxbuff[0])
			{
				break;
			}
			MR(ADDR_DATABUFF+8+(4*x),rxbuff+14+(4*i),4);
			i++;
			x++;
			p8[1]+=4;
		}
		ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	}
}
#endif

#if ERC4==1
void ERC4_Event(u32 sc,u32 ss)//状态量变位记录;入口sc=状态变位,ss=变位后状态
{
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=4;//ERC
	p8[1]=7;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=sc;
	p8[8]=ss;
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC5==1
void ERC5_Event(u32 n,u32 kw1,u32 kw2)//遥控跳闸记录;入口n=跳闸轮次(0-7),kw1=跳闸时功率(总加功率),kw2=跳闸后2分钟的功率(总加功率)
{
	u64 d64;
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=5;//ERC
	p8[1]=10;//长度Le
	//跳闸时间为当前记录时间减2分
	d64=MRR(ADDR_TYMDHMS+1,5);//分时日月年
	d64=YMDHM_SubM(d64,2);//年月日时分减m分,返回减m分后的年月日时分
	MWR(d64,ADDR_DATABUFF+2,5);
	p8[7]=1<<n;
	p8[8]=kw1;
	p8[9]=kw1>>8;
	p8[10]=kw2;
	p8[11]=kw2>>8;
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC6==1
void ERC6_Event(u32 Pn,u32 Bin,u32 Type,u32 kW1,u32 kW2,u32 kW3)//功控跳闸记录;入口Pn总加组号(从1开始),Bin跳闸轮次,Type功控类别,kw1跳闸前功率,kw2跳闸后2分钟功率,kw3跳闸时功率定值
{
	u32 i;
	u32 x;
	u64 d64;
	u8 * p8;
	
	if(Bin==0x0)
	{
		return;
	}
	x=0x80;
	for(i=0;i<8;i++)
	{
		if((x&Bin)!=0x0)
		{
			break;
		}
		x>>=1;
	}
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=6;//ERC
	p8[1]=14;//长度Le
	//跳闸时间为当前记录时间减2分
	d64=MRR(ADDR_TYMDHMS+1,5);//分时日月年
	d64=YMDHM_SubM(d64,2);//年月日时分减m分,返回减m分后的年月日时分
	MWR(d64,ADDR_DATABUFF+2,5);
	p8[7]=Pn;
	p8[8]=x;
	p8[9]=Type;
	p8[10]=kW1;
	p8[11]=kW1>>8;
	p8[12]=kW2;
	p8[13]=kW2>>8;
	p8[14]=kW3;
	p8[15]=kW3>>8;
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC7==1
void ERC7_Event(u32 Pn,u32 Bin,u32 Type,u32 kWh1,u32 kWh2)//电控跳闸记录;入口Pn总加组号(从1开始),Bin跳闸轮次,Type电控类别,kwh1跳闸时电能量,kwh2跳闸时电能量定值
{
	u32 i;
	u32 x;
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=7;//ERC
	p8[1]=16;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;

	x=0;
	for(i=0;i<MaxControlRound;i++)
	{
		x<<=1;
		x|=1;
	}
	
	p8[8]=Bin&x;
	p8[9]=Type;
	MWR(kWh1,ADDR_DATABUFF+10,4);
	MWR(kWh2,ADDR_DATABUFF+14,4);
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC8==1
void ERC8_Event(u32 Pn,u32 FnDataBuff)//电能表参数变更,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
{
	u32 i;
	u32 f;
	u8 * p8;
	u8 * p8FnData;
	
	if(Pn>=ERC8MaxPn)
	{
		return;
	}
	p8=(u8 *)(ADDR_ERC8_SOUR+(ERC8_SOUR_Pn_LEN*Pn));
	p8FnData=(u8 *)(FnDataBuff);
	f=0;
	//电能表费率时段变化
	if(p8[0]==p8FnData[0])
	{
		if(DataComp(((u32)p8)+1,((u32)p8FnData)+1,4)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		{
			f|=1;
		}
	}
	if(p8[5]==p8FnData[5])
	{
		if(DataComp(((u32)p8)+6,((u32)p8FnData)+6,4)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		{
			f|=1;
		}
	}
	//电能表抄表日更改
	for(i=0;i<3;i++)
	{
		if(p8[10+(2*i)]!=0xee)
		{
			if(p8FnData[10+(2*i)]!=0xee)
			{
				if(DataComp(((u32)p8)+10+(2*i),((u32)p8FnData)+10+(2*i),2)!=0x0)
				{
					f|=4;
				}
			}
		}
	}
	//电能表脉冲常数更改
	for(i=0;i<2;i++)
	{
		if(p8[10+6+(3*i)]!=0xee)
		{
			if(p8FnData[10+6+(3*i)]!=0xee)
			{
				if(DataComp(((u32)p8)+10+6+(3*i),((u32)p8FnData)+10+6+(3*i),3)!=0x0)
				{
					f|=8;
				}
			}
		}
	}
	//电能表的互感器倍率更改
	for(i=0;i<2;i++)
	{
		if(p8[10+6+6+(3*i)]!=0xee)
		{
			if(p8FnData[10+6+6+(3*i)]!=0xee)
			{
				if(DataComp(((u32)p8)+10+6+6+(3*i),((u32)p8FnData)+10+6+6+(3*i),3)!=0x0)
				{
					f|=0x10;
				}
			}
		}
	}
	//电能表编程时间更改
	if(p8[10+6+6+6]!=0xee)
	{
		if(p8FnData[10+6+6+6]!=0xee)
		{
			if(DataComp(((u32)p8)+10+6+6+6,((u32)p8FnData)+10+6+6+6,6)!=0x0)
			{
				f|=0x2;
			}
		}
	}
	//电能表最大需量清零
	if(p8[10+6+6+6+6]!=0xee)
	{
		if(p8FnData[10+6+6+6+6]!=0xee)
		{
			//if(DataComp(((u32)p8)+10+6+6+6+6,((u32)p8FnData)+10+6+6+6+6,6)!=0x0)
			if(DataComp(((u32)p8)+10+6+6+6+6,((u32)p8FnData)+10+6+6+6+6,3)!=0x0)
			{
				f|=0x20;
			}
		}
	}
	MW((u32)p8FnData,(u32)p8,ERC8_SOUR_Pn_LEN);
	if(f!=0x0)
	{
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=8;//ERC
		p8[1]=8;//长度Le
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
		Pn++;
		p8[7]=Pn;
		p8[8]=Pn>>8;
		p8[9]=f;
		ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	}
}
#endif

#if ERC9==1
void ERC9_Event_Write(u32 Pn,u32 s,u32 f)//电流异常写,入口:Pn测量点号(从1开始),s=1开始=0恢复,f=异常标志
{
	u32 i;
	u8 * p8;

	if(ERC_Event_Set(9)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8=(u8 *)(ADDR_ERC9_SOUR+((Pn-1)*LEN_ERC9_SOUR_Pn));
		p8[0]=0;
		return;
	}
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=9;//ERC
	p8[1]=28;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(s<<7);
	p8[9]=f;
	i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	MR(ADDR_DATABUFF+10,i+37,15);
	i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	MR(ADDR_DATABUFF+25,i+6,5);
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}

void ERC9_3_Event(u32 Pn)//电流回路异常.反向,入口:Pn测量点号(从1开始)
{
	u32 i;
	u32 is;//额定电流
	u32 Line;//接线方式
	u32 ia;
	u32 ib;
	u32 ic;
	u32 f;
	u8 * p8s;
	u8 * p8d;
/*
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
//电压回路异常、电流回路异常、相序异常、电压/电流不平衡越限、电压越限、电流越限等6个事件，终端不要求对使用DL/T645-2007的测量点生成
	i=Check_ACSamplePn(Pn);//有效交采测量点号检查;返回0=无效,1=有效
	if(i==0)
	{
		return;
	}
#endif
*/
	i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		p8d=(u8 *)(i);
		if(Pn<=ERC9MaxPn)
		{//Pn<=测量点基本参数(测量点号)
			Pn--;
			p8d+=43;
			is=MRR(ADDR_AFN04F25+(11*Pn)+6,1);//读额定电流0.0A
			is<<=8;//000.000A
			is=bcd_hex(is);
			is*=5;
			is/=100;//5%IB
			is=hex_bcd(is);
			Line=MRR(ADDR_AFN04F25+(11*Pn)+15,1);//读接线方式
			Line&=3;
			ia=MRR((u32)p8d,3);//取A电流
			ia&=0x7fffff;
			ib=MRR(((u32)p8d)+3,3);//取B电流
			ib&=0x7fffff;
			ic=MRR(((u32)p8d)+6,3);//取C电流
			ic&=0x7fffff;
	
			p8s=(u8 *)(ADDR_ERC9_SOUR+(Pn*LEN_ERC9_SOUR_Pn));
			f=0xc0;//反向恢复标志=0
			//A恢复
			if(p8d[0]!=0xee)
			{
				if(ia>=is)
				{//IA>=is
					if((p8d[2]&0x80)==0x0)
					{
						if((p8s[0]&0x3)==0x3)
						{
							f|=1;
							p8s[0]&=0xfc;
						}
					}
				}
			}
			//B恢复
			if(Line!=0x1)
			{//不是三线
				if(p8d[3]!=0xee)
				{
					if(ib>=is)
					{//IB>=is
						if((p8d[5]&0x80)==0x0)
						{
							if((p8s[0]&0xc)==0xc)
							{
								f|=2;
								p8s[0]&=0xf3;
							}
						}
					}
				}
			}
			//C恢复
			if(p8d[6]!=0xee)
			{
				if(ic>=is)
				{//IC>=is
					if((p8d[8]&0x80)==0x0)
					{
						if((p8s[0]&0x30)==0x30)
						{
							f|=4;
							p8s[0]&=0xcf;
						}
					}
				}
			}
			if((f&0x07)!=0x0)
			{//有恢复
				ERC9_Event_Write(Pn+1,0,f);//电流异常写,入口:Pn测量点号(从1开始),s=1开始=0恢复,f=异常标志
			}
			f=0xc0;//反向起始标志=0
			//A产生
			if(p8d[0]!=0xee)
			{
				if(ia>=is)
				{//IA>=is
					if((p8d[2]&0x80)!=0x0)
					{
						if((p8s[0]&0x3)==0x0)
						{
							f|=1;
							p8s[0]|=0x3;
						}
					}
				}
			}
			//B产生
			if(Line!=0x1)
			{//不是三线
				if(p8d[3]!=0xee)
				{
					if(ib>=is)
					{//IB>=is
						if((p8d[5]&0x80)!=0x0)
						{
							if((p8s[0]&0xc)==0x0)
							{
								f|=2;
								p8s[0]|=0xc;
							}
						}
					}
				}
			}
			//C产生
			if(p8d[6]!=0xee)
			{
				if(ic>=is)
				{//IC>=is
					if((p8d[8]&0x80)!=0x0)
					{
						if((p8s[0]&0x30)==0x0)
						{
							f|=4;
							p8s[0]|=0x30;
						}
					}
				}
			}
			if((f&0x07)!=0x0)
			{//有产生
				ERC9_Event_Write(Pn+1,1,f);//电流异常写,入口:Pn测量点号(从1开始),s=1开始=0恢复,f=异常标志
			}
			
		}
	}
}
#endif

#if ERC10==1
void ERC10_Event_Write(u32 Pn,u32 s,u32 f)//电压异常写,入口:Pn测量点号(从1开始),s=1开始=0恢复,f=异常标志
{
	u32 i;
	u8 * p8;
	if(ERC_Event_Set(10)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8=(u8 *)(ADDR_ERC10_SOUR+(LEN_ERC10_SOUR_Pn*(Pn-1)));
		p8[0]=0;
		return;
	}
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=10;//ERC
	p8[1]=28;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(s<<7);
	p8[9]=f;
	i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	MR(ADDR_DATABUFF+10,i+37,15);
	i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	MR(ADDR_DATABUFF+25,i+6,5);
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}

#if (((USER/100)==2)||((USER/100)==5))//用户标识:山东,上海
void ERC10_Event(void)//电压回路异常(断相失压)
{
}
#else
void ERC10_Event(void)//电压回路异常(断相失压)
{
	u32 i;
	u32 up;//断相门限
	u32 ul;//失压门限
	u32 is;//额定电流
	u32 Line;//接线方式
	u32 ua;
	u32 ub;
	u32 uc;
	u32 ia;
	u32 ib;
	u32 ic;
	u32 fui;//电压电流数据有效标志
	u32 f1;//断相标志
	u32 f2;//失压标志	

	u8 * p8s;
	u8 * p8d;
	u16 * p16;

	u32 Pn;
	Pn=Terminal_Ram->I2SUBTask4_15;//ERC10_Event();//电压回路异常(断相、失压)
	if(Pn>AFN04F25MaxPn)
	{
		Pn=1;
	}
/*
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
//电压回路异常、电流回路异常、相序异常、电压/电流不平衡越限、电压越限、电流越限等6个事件，终端不要求对使用DL/T645-2007的测量点生成
	i=Check_ACSamplePn(Pn);//有效交采测量点号检查;返回0=无效,1=有效
	if(i==0)
	{
		return;
	}
#endif
*/
	i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		if(Pn<=AFN04F25MaxPn)
		{//Pn<=测量点基本参数(测量点号)
			if(Pn<=AFN04F26MaxPn)
			{//Pn<=测量点限值参数(测量点号)
				Pn--;
				up=MRR(ADDR_AFN04F26+(57*Pn)+4,2);//读断相门限
				ul=MRR(ADDR_AFN04F25+(11*Pn)+4,2);//读额定电压000.0V计算失压门限
				ul=bcd_hex(ul);
				ul*=60;//60%UN
				ul/=100;
				ul=hex_bcd(ul);
				
				is=MRR(ADDR_AFN04F25+(11*Pn)+6,1);//读额定电流0.0A
				is<<=8;//000.000A
				is=bcd_hex(is);
				is*=5;
				is/=100;//5%IB
				is=hex_bcd(is);
				Line=MRR(ADDR_AFN04F25+(11*Pn)+15,1);//读接线方式
				Line&=3;
				ua=MRR(i+37,2);//取A电压
				ub=MRR(i+39,2);//取B电压
				uc=MRR(i+41,2);//取C电压
				ia=MRR(i+43,3);//取A电流
				ia&=0x7fffff;
				ib=MRR(i+46,3);//取B电流
				ib&=0x7fffff;
				ic=MRR(i+49,3);//取C电流
				ic&=0x7fffff;
				p8s=(u8 *)(i);
				p8s+=37;
				fui=0;//电压电流数据有效标志
				f1=0;//断相标志
				//A相
				if(p8s[0]!=0xee)
				{
					if(p8s[6]!=0xee)
					{
						fui|=1;//电压电流数据有效标志
						if(ua<up)
						{
							if(ia<is)
							{
								f1|=1;//断相标志
							}
						}
					}
				}
				//B相
				if(Line!=0x1)
				{//不是三线
					if(p8s[2]!=0xee)
					{
						if(p8s[9]!=0xee)
						{
							fui|=2;//电压电流数据有效标志
							if(ub<up)
							{
								if(ib<is)
								{
									f1|=2;//断相标志
								}
							}
						}
					}
				}
				//C相
				if(p8s[4]!=0xee)
				{
					if(p8s[12]!=0xee)
					{
						fui|=4;//电压电流数据有效标志
						if(uc<up)
						{
							if(ic<is)
							{
								f1|=4;//断相标志
							}
						}
					}
				}
				f2=0;//失压标志	
				//A相
				if(p8s[0]!=0xee)
				{
					if(p8s[6]!=0xee)
					{
						fui|=1;//电压电流数据有效标志
						if(ua<ul)
						{
							if(ia>=is)
							{
								f2|=1;//失压标志	
							}
						}
					}
				}
				//B相
				if(Line!=0x1)
				{//不是三线
					if(p8s[2]!=0xee)
					{
						if(p8s[9]!=0xee)
						{
							fui|=2;//电压电流数据有效标志
							if(ub<ul)
							{
								if(ib>=is)
								{
									f2|=2;//失压标志
								}
							}
						}
					}
				}
				//C相
				if(p8s[4]!=0xee)
				{
					if(p8s[12]!=0xee)
					{
						fui|=4;//电压电流数据有效标志
						if(uc<ul)
						{
							if(ic>=is)
							{
								f2|=4;//失压标志
							}
						}
					}
				}

				p8d=(u8 *)(ADDR_ERC10_SOUR+(LEN_ERC10_SOUR_Pn*Pn));
				p16=(u16 *)(ADDR_ERC10_Timer);//ERC10断相失压事件延时秒定时器
				i=f1|(f2<<4);
				if(i==p8d[1])
				{
					if(p16[Pn]==0x0)
					{//延时秒定时=0
						//断相恢复
						i=~f1;
						i&=0x7;
						i&=fui;
						if((i&0x1)!=0x0)
						{
							if((p8d[0]&0x3)!=0x1)
							{
								i&=0xfe;
							}
						}
						if((i&0x2)!=0x0)
						{
							if((p8d[0]&0x0c)!=0x4)
							{
								i&=0xfd;
							}
						}
						if((i&0x4)!=0x0)
						{
							if((p8d[0]&0x30)!=0x10)
							{
								i&=0xfb;
							}
						}
						if(i!=0x0)
						{
							i|=1<<6;
							ERC10_Event_Write(Pn+1,0,i);//电压异常写,入口:Pn测量点号(从1开始),s=1开始=0恢复,f=异常标志
						}
						//失压恢复
						i=~f2;
						i&=0x7;
						i&=fui;
						if((i&0x1)!=0x0)
						{
							if((p8d[0]&0x3)!=0x2)
							{
								i&=0xfe;
							}
						}
						if((i&0x2)!=0x0)
						{
							if((p8d[0]&0x0c)!=0x8)
							{
								i&=0xfd;
							}
						}
						if((i&0x4)!=0x0)
						{
							if((p8d[0]&0x30)!=0x20)
							{
								i&=0xfb;
							}
						}
						if(i!=0x0)
						{
							i|=2<<6;
							ERC10_Event_Write(Pn+1,0,i);//电压异常写,入口:Pn测量点号(从1开始),s=1开始=0恢复,f=异常标志
						}
						//断相产生
						i=f1;
						if((i&0x1)!=0x0)
						{
							if((p8d[0]&0x3)==0x1)
							{
								i&=0xfe;
							}
						}
						if((i&0x2)!=0x0)
						{
							if((p8d[0]&0xc)==0x4)
							{
								i&=0xfd;
							}
						}
						if((i&0x4)!=0x0)
						{
							if((p8d[0]&0x30)==0x10)
							{
								i&=0xfb;
							}
						}
						if(i!=0x0)
						{
							i|=1<<6;
							ERC10_Event_Write(Pn+1,1,i);//电压异常写,入口:Pn测量点号(从1开始),s=1开始=0恢复,f=异常标志
						}
						//失压产生
						i=f2;
						if((i&0x1)!=0x0)
						{
							if((p8d[0]&0x3)==0x2)
							{
								i&=0xfe;
							}
						}
						if((i&0x2)!=0x0)
						{
							if((p8d[0]&0xc)==0x8)
							{
								i&=0xfd;
							}
						}
						if((i&0x4)!=0x0)
						{
							if((p8d[0]&0x30)==0x20)
							{
								i&=0xfb;
							}
						}
						if(i!=0x0)
						{
							i|=2<<6;
							ERC10_Event_Write(Pn+1,1,i);//电压异常写,入口:Pn测量点号(从1开始),s=1开始=0恢复,f=异常标志
						}
						//存当前标志
						i=0;
						if((f1&1)!=0x0)
						{
							i|=1;
						}
						else
						{
							if((f2&1)!=0x0)
							{
								i|=2;
							}
						}
						if((f1&2)!=0x0)
						{
							i|=4;
						}
						else
						{
							if((f2&2)!=0x0)
							{
								i|=8;
							}
						}
						if((f1&4)!=0x0)
						{
							i|=0x10;
						}
						else
						{
							if((f2&4)!=0x0)
							{
								i|=0x20;
							}
						}
						p8d[0]=i;
					}
				}
				else
				{
					p8d[1]=i;
					i=MRR(ADDR_AFN04F26+(57*Pn)+56,1);//连续失压时间限值 BIN min 1 
					i*=60;
					p16[Pn]=i;//稳定延时值S
				}
				Pn++;
			}
		}
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_15=Pn;
}
#endif
#endif

#if ERC11==1
void ERC11_Event(u32 Pn,u32 f)//相序异常,入口:Pn测量点号(从1开始),f=0恢复=1产生
{
	u32 i;
	u32 x;
	u8 * p8;
/*
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
//电压回路异常、电流回路异常、相序异常、电压/电流不平衡越限、电压越限、电流越限等6个事件，终端不要求对使用DL/T645-2007的测量点生成
	i=Check_ACSamplePn(Pn);//有效交采测量点号检查;返回0=无效,1=有效
	if(i==0)
	{
		return;
	}
#endif
*/
	i=GetClass1DataAddr(49,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		Pn--;
		p8=(u8 *)(ADDR_ERC11_SOUR);
		if(ERC_Event_Set(11)==0)//检查事件记录设置,返回0=不记录,1=记录
		{
			i=1<<Pn%8;
			x=p8[Pn/8];
			x&=~i;
			p8[Pn/8]=x;
			return;
		}
		i=p8[Pn/8];
		i>>=Pn%8;
		i&=1;
		if((i^f)!=0x0)
		{//变化
			i=1<<Pn%8;
			x=p8[Pn/8];
			x&=~i;
			i=f;
			i<<=Pn%8;
			x|=i;
			p8[Pn/8]=x;
			Pn++;
			p8=(u8 *)(ADDR_DATABUFF);
			p8[0]=11;//ERC
			p8[1]=24;//长度Le
			MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
			p8[7]=Pn;
			p8[8]=(Pn>>8)|(f<<7);
			i=GetClass1DataAddr(49,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
			MR(ADDR_DATABUFF+9,i,12);
			i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
			MR(ADDR_DATABUFF+21,i+6,5);
			ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
		}
	}
}
#endif

#if ERC12==1
void ERC12_Event(u32 Pn)//电能表时间超差,入口:Pn测量点号(从1开始)
{
	u32 i;
	u32 x;
	u32 f;
	u8 * p8;
#if (USER/100)==5//上海
	u32 HM;
#endif
	
	
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
#if (USER/100)==5//上海
//核对频率：0-每日核对一次；1-每周核对一次；2-每月核对一次；3-每轮抄表核对一次。（默认3）
//核对开始日期：每日核对一次为0；每周核对为1-7；每月核对为1-31。（默认0，核对频率为3时不判断此参数）
//核对开始小时：0-23，表示0点到23点。（默认4，核对频率为3时不判断此参数）
//核对开始分钟：0-59，表示0分到59分。（默认5，核对频率为3时不判断此参数）
	i=MRR(ADDR_AFN04F241,4);//核对频率
	x=i&0xff;
	HM=i>>24;//开始分
	HM=hex_bcd(HM);
	f=(i>>16)&0xff;
	f=hex_bcd(f);
	f<<=8;
	HM|=f;
	switch(x)
	{
		case 0://每日核对一次
			x=MRR(ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			f=MRR(ADDR_TYMDHMS+3,3);
			if(x==f)
			{
				return;//本日已校
			}
			f=MRR(ADDR_TYMDHMS+1,2);
			if(f<HM)
			{
				return;//没到开始分
			}
			MW(ADDR_TYMDHMS+3,ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			break;
		case 1://每周核对一次
			x=(i>>8)&0xff;
			f=Comm_Ram->TYMDHMS[6];//当前周0-6
			if(f==0)
			{
				f=7;
			}
			if(x!=f)
			{//周不同
				return;
			}
			x=MRR(ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			f=MRR(ADDR_TYMDHMS+3,3);
			if(x==f)
			{
				return;//本日已校
			}
			f=MRR(ADDR_TYMDHMS+1,2);
			if(f<HM)
			{
				return;//没到开始时分
			}
			MW(ADDR_TYMDHMS+3,ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			break;
		case 2://每月核对一次
			x=(i>>8)&0xff;
			f=Comm_Ram->TYMDHMS[3];//当前日
			f=bcd_hex(f);
			if(x!=f)
			{//日不同
				return;
			}
			x=MRR(ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			f=MRR(ADDR_TYMDHMS+3,3);
			if(x==f)
			{
				return;//本月已校
			}
			f=MRR(ADDR_TYMDHMS+1,2);
			if(f<HM)
			{
				return;//没到开始时分
			}
			MW(ADDR_TYMDHMS+3,ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			break;
	}
#endif
	i=GetClass1DataAddr(162,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		MR(ADDR_DATABUFF,i+6,6);
		MR(ADDR_DATABUFF+6,i,6);
		p8=(u8 *)(ADDR_DATABUFF);
		if(p8[0]==0xee)
		{//没抄表
			return;
		}
		i=MRR(ADDR_DATABUFF+1,2);//取电能表时钟时分
		if(i==0x2359)
		{//电能表时钟时分=23时59分,不比较,因时分秒和年月日为二次读取
			return;
		}
		p8[4]&=0x1f;//去加在月字节上的星期
		p8[10]&=0x1f;//去加在月字节上的星期
		i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//年月日时分秒合法检查,返回0合法,1非法
		if(i==0)
		{//电能表时钟合法
			i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
			if((i>>31)!=0x0)
			{//负
				i=~i;
				i++;
			}
		#if (USER/100)==5//上海
			x=MRR(ADDR_AFN04F241+4,1);//报警误差间隔分钟	BIN	1
			x*=60;
		#else
			x=MRR(ADDR_AFN04F59+3,2);//电能表校时阈值	BIN	s	2
		#endif
			f=0;
			if(x!=0)
			{//事件判断阀值为0时,不做判断
				if(i>=x)
				{
					f=1;
				}
			}
		}
		else
		{//电能表时钟非法
			f=1;
		}
		Pn--;
		p8=(u8 *)(ADDR_ERC12_SOUR);
		if(ERC_Event_Set(12)==0)//检查事件记录设置,返回0=不记录,1=记录
		{
			i=1<<Pn%8;
			x=p8[Pn/8];
			x&=~i;
			p8[Pn/8]=x;
			return;
		}
		i=p8[Pn/8];
		i>>=Pn%8;
		i&=1;
		if((i^f)!=0x0)
		{//变化
			i=1<<Pn%8;
			x=p8[Pn/8];
			x&=~i;
			i=f;
			i<<=Pn%8;
			x|=i;
			p8[Pn/8]=x;
			Pn++;
			p8=(u8 *)(ADDR_DATABUFF);
			p8[0]=12;//ERC
			p8[1]=7;//长度Le
			MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
			p8[7]=Pn;
			p8[8]=(Pn>>8)|(f<<7);
			ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
		#if ((USER/100)==5)//上海用户
			//上海扩展ERC61：电能表时钟偏差监测
			p8=(u8 *)(ADDR_DATABUFF);
			p8[0]=61;//ERC
			p8[1]=14;//长度Le
			i=GetClass1DataAddr(162,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
			MR(ADDR_DATABUFF+2,i,12);
			p8[14]=Pn;
			p8[15]=(Pn>>8)|(f<<7);
			ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
		#endif
		}
	}
}
#endif

#if ERC13==1
void ERC13_Event(u32 Pn,u32 FnDataBuff)//电表故障信息,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
{
	u32 i;
	u32 f;
	u8 * p8;
	u8 * p8FnData;
	
	if(Pn>=ERC13MaxPn)
	{
		return;
	}
	if(ERC_Event_Set(13)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		MC(0xee,ADDR_ERC13_SOUR+(ERC13_SOUR_Pn_LEN*Pn),ERC13_SOUR_Pn_LEN);
		return;
	}
	
	p8=(u8 *)(ADDR_ERC13_SOUR+(ERC13_SOUR_Pn_LEN*Pn));
	p8FnData=(u8 *)(FnDataBuff);
	f=0;
#if (USER/100)!=5//不是上海
	//电能表编程次数或最大需量清零次数发生变化
	if(p8[0]!=0xee)
	{
		if(p8FnData[0]!=0xee)
		{
			if(DataComp(((u32)p8),((u32)p8FnData),3)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			{
				f|=1;
			}
		}
	}
	if(p8[3]!=0xee)
	{
		if(p8FnData[3]!=0xee)
		{
			if(DataComp(((u32)p8)+3,((u32)p8FnData)+3,3)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			{
				f|=1;
			}
		}
	}
	//电能表断相次数变化
	if(p8[6]==p8FnData[6])
	{
		if(DataComp(((u32)p8)+7,((u32)p8FnData)+7,3)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		{
			f|=2;
		}
	}
	//电能表失压次数变化
	if(p8[10]!=0xee)
	{
		if(p8FnData[10]!=0xee)
		{
			if(DataComp(((u32)p8)+10,((u32)p8FnData)+10,3)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			{
				f|=4;
			}
		}
	}
	//电能表停电次数变化
	if(p8[13]!=0xee)
	{
		if(p8FnData[13]!=0xee)
		{
			if(DataComp(((u32)p8)+13,((u32)p8FnData)+13,3)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
			{
				f|=8;
			}
		}
	}
#endif
	//电能表电池欠压
	i=p8FnData[16];
	if(i!=0xee)
	{
		if(i==0x1)
		{//上电时若电池欠压无论如何上报一次
			if(i!=p8[16])
			{
				f|=0x10;
			}
		}
		else
		{
			if(p8[16]!=0xee)
			{
				if(i!=p8[16])
				{
					f|=0x10;
				}
			}
		}
	}
	MW((u32)p8FnData,(u32)p8,ERC13_SOUR_Pn_LEN);
	if(f!=0x0)
	{
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=13;//ERC
		p8[1]=8;//长度Le
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
		Pn++;
		p8[7]=Pn;
		if((f&0x10)!=0x0)
		{//电池欠压有产生恢复,其他只有产生
			if(i==0x0)
			{//电能表电池欠压恢复
				p8[8]=Pn>>8;
				p8[9]=0x10;
				ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				f&=0x0f;
				if(f==0x0)
				{
					return;
				}
			}
		}
		p8[8]=(Pn>>8)|0x80;
		p8[9]=f;
		ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	}
}
#endif

#if ERC14==1
#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
#if (USER/100)!=2//用户标识:不是山东版
void ERC14_Event_Start(void)//终端停/上电事件(停电)
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=14;//ERC
	p8[1]=10;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//分时日月年
	MC(0xee,ADDR_DATABUFF+7,6);//分时日月年,上电记录标志=0xee
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
void ERC14_Event_End(void)//终端停/上电事件(上电),用专用事件数据缓冲
{
	u32 i;
	u32 x;
	u8 * p8;
	
//先查重要事件停电事件记录
	for(i=0;i<256;i++)
	{
		if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),1)==14)
		{//ERC=14,是停/上电事件
			if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+7,6)==0xeeeeeeeeeeee)
			{//上电记录标志=0xee
				MC(0,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+7+5,1);//上电记录标志=0
				x=MRR(ADDR_AFN0CF7_FM,1);
				if(i==x)
				{
					x++;
					MWR(x,ADDR_AFN0CF7_FM,1);
				}
				MR(ADDR_ERC14SOUR,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),7);
				MW(ADDR_TYMDHMS+1,ADDR_ERC14SOUR+7,5);
				ERC_Event_Save_Re(ADDR_ERC14SOUR);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				return;
			}
		}
	}
	
//再查一般事件停电事件记录
	for(i=0;i<256;i++)
	{
		if(MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),1)==14)
		{//ERC=14,是停/上电事件
			if(MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i)+7,6)==0xeeeeeeeeeeee)
			{//上电记录标志=0xee
				MC(0,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i)+7+5,1);//上电记录标志=0
				x=MRR(ADDR_AFN0CF7_FM+1,1);
				if(i==x)
				{
					x++;
					MWR(x,ADDR_AFN0CF7_FM+1,1);
				}
				MR(ADDR_ERC14SOUR,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),7);
				MW(ADDR_TYMDHMS+1,ADDR_ERC14SOUR+7,5);
				ERC_Event_Save_Re(ADDR_ERC14SOUR);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				return;
			}
		}
	}
//原没停电事件记录,补停电记录
	p8=(u8 *)(ADDR_ERC14SOUR);
	p8[0]=14;//ERC
	p8[1]=10;//长度Le
	//停电时:分时日月年
	MC(0xee,ADDR_ERC14SOUR+7,5);//上电时间：分时日月年
	MC(0,ADDR_ERC14SOUR+12,1);//上电记录标志=0,停电时记为0xee,上电记录时清为0,用于记录指针加1识别
	ERC_Event_Save_Re(ADDR_ERC14SOUR);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
//原没停电事件记录,再记录上电事件
	//停电时:分时日月年
	MR(ADDR_ERC14SOUR+7,ADDR_TYMDHMS+1,5);//分时日月年
	ERC_Event_Save_Re(ADDR_ERC14SOUR);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#else//#if (USER/100)!=2//用户标识:不是山东版
//山东版
void ERC14_Event_Start(void)//终端停/上电事件(停电),用专用事件数据缓冲
{
	u8 * p8;
	p8=(u8 *)(ADDR_ERC14SOUR);
	p8[0]=14;//ERC
	p8[1]=10;//长度Le
	MR(ADDR_ERC14SOUR+2,ADDR_TFM_YMDHMS+1,5);//停电时:分时日月年
	MC(0xee,ADDR_ERC14SOUR+2+10,1);//1BYTE 停电时记为0xee,上电记录时清为0,用于记录指针加1识别
	ERC_Event_Save_Re(ADDR_ERC14SOUR);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
void ERC14_Event_End(void)//终端停/上电事件(上电),用专用事件数据缓冲
{
	u32 i;
	u32 x;
	u8 * p8;
	p8=(u8 *)(ADDR_ERC14SOUR);
	p8[0]=14;//ERC
	p8[1]=10;//长度Le
	//停电时:分时日月年
	MR(ADDR_ERC14SOUR+7,ADDR_TYMDHMS+1,5);//分时日月年
	MC(0,ADDR_ERC14SOUR+2+10,1);//1BYTE 停电时记为0xee,上电记录时清为0,用于记录指针加1识别
//先查重要事件停电事件记录
	for(i=0;i<256;i++)
	{
		if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),1)==14)
		{//ERC=14,是停/上电事件
			if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+7+5,1)==0xee)
			{//上电记录标志=0xee
				MC(0,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+7+5,1);//1BYTE 停电时记为0xee,上电记录时清为0,用于记录指针加1识别
				x=MRR(ADDR_AFN0CF7_FM,1);
				if(i==x)
				{
					x++;
					MWR(x,ADDR_AFN0CF7_FM,1);
				}
			}
		}
	}
	ERC_Event_Save_Re(ADDR_ERC14SOUR);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif//#if (USER/100)!=2//用户标识:不是山东版
#endif//#if NewERC14==0//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知

#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
void ERC14_Event_Start(u32 f)//终端停/上电事件(停电);入口:上电记录时事件记数指针需加1标志,0=不加1,0xee=需加1
{
	u8 * p8;
#if (USER/100)==11//吉林
	u64 YMDHMS;
#endif

	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=14;//ERC
	p8[1]=11;//长度Le
	p8[2]=1;//D0=正常,D1=无效
#if (USER/100)==11//吉林
	//吉林测试时停电和上电时间各加5秒能过台体
	YMDHMS=MRR(ADDR_POWER_YMDHMS,6);
	YMDHMS=YMDHMS_AddS(YMDHMS,5);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
	YMDHMS>>=8;
	MWR(YMDHMS,ADDR_DATABUFF+3,5);
#else
	MR(ADDR_DATABUFF+3,ADDR_POWER_YMDHMS+1,5);//ERC14事件判定掉电前的时钟
#endif
	MC(0xee,ADDR_DATABUFF+8,5);//分时日月年
	p8[13]=f;//上电记录标志
	ERC_Event_Save_Re(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
void ERC14_Event_End(u32 f)//终端停/上电事件(上电);入口:属性标志
{
	u8 * p8;
#if (USER/100)==11//吉林
	u64 YMDHMS;
#endif

	p8=(u8 *)(ADDR_ERC14SOUR+2);
	p8[0]=14;//ERC
	p8[1]=11;//长度Le
    p8[2]=f;//属性
#if (USER/100)==7//浙江，增加一个字节用以判断是否上报
    p8[2]&=0x03;//属性   
#endif
 #if (USER/100)==11//吉林
	//吉林测试时停电和上电时间各加5秒能过台体
	YMDHMS=MRR(ADDR_ERC14SOUR+16,6);
	YMDHMS=YMDHMS_AddS(YMDHMS,5);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
	YMDHMS>>=8;
	MWR(YMDHMS,((u32)p8)+3,5);
	YMDHMS=MRR(ADDR_ERC14SOUR+22,6);
	YMDHMS=YMDHMS_AddS(YMDHMS,5);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
	YMDHMS>>=8;
	MWR(YMDHMS,((u32)p8)+8,5);
 #else
	MR(((u32)p8)+3,ADDR_ERC14SOUR+17,5);//停电时:分时日月年
	MR(((u32)p8)+8,ADDR_ERC14SOUR+23,5);//上电时间：分时日月年
 #endif
	p8[13]=0;//上电记录标志=0,停电时记为0xee,上电记录时清为0,用于记录指针加1识别
#if (USER/100)==7//浙江，增加一个字节用以判断是否上报
    p8[14]=(f&0x80);
#endif
	ERC_Event_Save(ADDR_ERC14SOUR+2);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif//#if NewERC14==1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
#endif//#if ERC14==1






#if ERC15==1//ERC15谐波越限告警
//已包含在统计中
#endif

#if ERC16==1//直流模拟量越限
void ERC16_Event(u32 Pn,u32 ss,u32 f,u32 dc)//直流模拟量越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=越限标志,dc=当前直流模拟量
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=16;//ERC
	p8[1]=9;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//分时日月年
	p8[7]=Pn|(ss<<7);
	p8[8]=f;
	p8[9]=dc;
	p8[10]=dc>>8;
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC17==1
	#if (USER/100)==5//上海
void ERC17_Event(u32 Pn,u32 ss,u32 f,u32 unu,u32 uni)//电压/电流不平衡度越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,unu=压不平衡度,uni=电流不平衡度
{
}
	#else
void ERC17_Event(u32 Pn,u32 ss,u32 f,u32 unu,u32 uni)//电压/电流不平衡度越限,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,unu=压不平衡度,uni=电流不平衡度
{
	u32 i;
	u8 * p8;
/*
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
//电压回路异常、电流回路异常、相序异常、电压/电流不平衡越限、电压越限、电流越限等6个事件，终端不要求对使用DL/T645-2007的测量点生成
	i=Check_ACSamplePn(Pn);//有效交采测量点号检查;返回0=无效,1=有效
	if(i==0)
	{
		return;
	}
#endif
*/
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=17;//ERC
	p8[1]=27;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(ss<<7);
	p8[9]=f;
	p8[10]=unu;
	p8[11]=unu>>8;
	p8[12]=uni;
	p8[13]=uni>>8;
	i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		MR(ADDR_DATABUFF+14,i+37,15);
		ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	}
}
	#endif
#endif

#if ERC18==1

#endif

#if ERC19==1
void ERC19_Event(u32 Pn,u32 RxAddr,u32 kWh1,u32 kWh2)//购电参数设置记录
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=19;//ERC
	p8[1]=31;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	MR(ADDR_DATABUFF+8,RxAddr,17);//购电参数
	MWR(kWh1,ADDR_DATABUFF+25,4);//本次购电前剩余电能量（费）
	MWR(kWh2,ADDR_DATABUFF+29,4);//本次购电后剩余电能量（费）
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC20==1
void ERC20_Event(u64 PWL,u64 PWH,u32 MSA)//消息认证错误记录,入口:PWH_PWL消息认证码,MSA=启动站地址
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=20;//ERC
	p8[1]=22;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	MWR(PWL,ADDR_DATABUFF+7,8);//写寄存器数据到存贮器,Byte<=8
	MWR(PWH,ADDR_DATABUFF+7+8,8);//写寄存器数据到存贮器,Byte<=8
	p8[23]=MSA;
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif


#if ERC21==1
void ERC21_Event(void)//ERC21 485抄表故障事件
{
	u32 i;
	u32 x;
	u32 f;
	u32 MeterNo;
	u32 Addr;
	u8 * p8;

	f=1;//产生
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		x=MRR(Addr,2);//所属测量点号 BIN 2 
		if(x!=0)
		{//测量点号有效
			x=MRR(Addr+2,1);//通信速率及端口号 BIN 1 
			x&=0x1f;
			if((x>=2)&&(x<=4))
			{//是RS485-1-2-3
				MeterNo=i;
				p8=(u8 *)(ADDR_RM_CountFlags+32+(MeterNo/4));
				MeterNo%=4;
				MeterNo<<=1;
				if(((p8[0]>>MeterNo)&3)<=1)
				{//0=成功,1=失败->成功,2=成功->失败,3=失败
					f=0;
				}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	if(f==1)
	{//全部失败,记录485抄表故障
		i=MRR(ADDR_ERC21_Flag,1);//ERC21 485抄表故障事件记录标志
		if(i==0)
		{//没记录
			p8=(u8 *)(ADDR_DATABUFF);
			p8[0]=21;//ERC
			p8[1]=6;//长度Le
			MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
			p8[7]=4;//4=485抄表故障
			i=ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
			MWR(i,ADDR_ERC21_Flag,1);//ERC21 485抄表故障事件记录标志
		}
	}
}
#endif

#if ERC22==1
void ERC22_Event(void)//有功总电能量差动越限计算和事件记录
{
#if MaxDiffGroup!=0
	u32 i;
	u32 Pn;//差动组号
	u32 x;//对比的总加组号
	u32 y;//参照总加组号
	u32 absolute;//绝对值
	u32 relative;//相对值
	u32 f;
	u32 Len;//事件数据长度
	u64 a64;
	u64 b64;
	
	u8 * p8;
	u8 * p8s;
	u16 * p16;
	
	Pn=Terminal_Ram->ERC22Task;
	if(Pn>=MaxDiffGroup)
	{
		Pn=0;
		Terminal_Ram->ERC22Task=0;
	}
	p16=(u16 *)(ADDR_ERC22_Timer);//ERC22差动越限事件秒定时器
	if(p16[Pn]==0x0)
	{//区间定时到
		i=MRR(ADDR_AFN04F15+(Pn*9)+3,1);//取参与差动的电能量的时间区间及对比方法标志
		i&=3;
		switch(i)
		{
			case 0:
				i=60*60;
				break;
			case 1:
				i=30*60;
				break;
			default:
				i=15*60;
				break;
		}
		p16[Pn]=i;//下区间定时S
		
		x=MRR(ADDR_AFN04F15+(Pn*9)+1,1);//对比的总加组号
		if((x!=0x0)&&(x<=MaxTotalAddGroup))
		{
			x--;
			y=MRR(ADDR_AFN04F15+(Pn*9)+2,1);//参照总加组号
			if((y!=0x0)&&(y<=MaxTotalAddGroup))
			{
				y--;
				//取对比总加电量
				i=MRR(ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn)+(x*4),4);//读存贮器数据到寄存器
				a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
				//取参照总加电量
				i=MRR(ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn)+(y*4),4);//读存贮器数据到寄存器
				b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
				//绝对偏差值
				a64-=b64;
				if((a64>>63)!=0x0)
				{//负
					a64=(~a64)+1;
				}
				absolute=kWh_Data03(a64);
				//相对偏差值
				relative=(a64*100)/b64;
				//比较
				f=0;
				i=MRR(ADDR_AFN04F15+(Pn*9)+3,1);//取参与差动的电能量的时间区间及对比方法标志
				i>>=7;
				if(i==0x0)
				{//相对对比
					i=MRR(ADDR_AFN04F15+(Pn*9)+4,1);//取差动越限相对偏差值
					if(i==0)
					{//比对值=0不比较
						p8=(u8 *)(ADDR_ERC22_SOUR);//原差动组超限标志,0=没超,1=超限
						p8[Pn]=0;
					}
					else
					{
						if(relative>=i)
						{
							f=1;
						}
					}
				}
				else
				{//绝对对比
					i=MRR(ADDR_AFN04F15+(Pn*9)+5,4);//取差动越限绝对偏差值
					i&=0xefffffff;//去符号,总是用正值
					if(i==0)
					{//比对值=0不比较
						p8=(u8 *)(ADDR_ERC22_SOUR);//原差动组超限标志,0=没超,1=超限
						p8[Pn]=0;
					}
					else
					{
						b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
						if(a64>=b64)
						{
							f=1;
						}
					}
				}
				p8=(u8 *)(ADDR_ERC22_SOUR);//原差动组超限标志,0=没超,1=超限
				if((f^p8[Pn])!=0x0)
				{//变化
					p8[Pn]=f;
					p8=(u8 *)(ADDR_DATABUFF);
					p8[0]=22;//ERC
					MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//分时日月年
					p8[7]=(Pn+1)|(f<<7);//D7：起/止标志	D6=0	D5～D0：电能量差动组号	BIN	1
					MR(ADDR_DATABUFF+8,ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn)+(x*4),4);//越限时对比总加组有功总电能量	见附录A.3	4
					MR(ADDR_DATABUFF+12,ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn)+(y*4),4);//越限时参照总加组有功总电能量	见附录A.3	4
					p8[16]=relative;//越限时差动越限相对偏差值	BIN	1
					MWR(absolute,ADDR_DATABUFF+17,4);//越限时差动越限绝对偏差值	见附录A.3	4
					p8s=(u8 *)(ADDR_AFN04F14+((2+64)*x));//对比总加组配置
					f=MRR(((u32)(p8s))+1,1);//对比的总加组测量点数量n（1≤n≤64）	BIN	1
					p8[21]=f;
					p8s+=2;
					Len=22;//总长度已22字节
					for(i=0;i<f;i++)
					{//对比总加组各测量点有功总电能示值
						if((Len+5)<=LEN_TERMINAL_EVENT)//终端事件记录固定长度
						{
							x=GetClass1DataAddr(129,(MRR(((u32)(p8s))+i,1)&0x3f)+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
							if(x!=0x0)
							{
								MR(ADDR_DATABUFF+Len,x+6,5);
								Len+=5;
							}
						}
					}
					if(Len<LEN_TERMINAL_EVENT)
					{
						p8s=(u8 *)(ADDR_AFN04F14+((2+64)*y));//参照总加组配置
						f=MRR(((u32)(p8s))+1,1);//对比的总加组测量点数量n（1≤n≤64）	BIN	1
						p8[Len]=f;
						Len++;
						p8s+=2;
						for(i=0;i<f;i++)
						{//对比总加组各测量点有功总电能示值
							if((Len+5)<=LEN_TERMINAL_EVENT)//终端事件记录固定长度
							{
								x=GetClass1DataAddr(129,(MRR(((u32)(p8s))+i,1)&0x3f)+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
								if(x!=0x0)
								{
									MR(ADDR_DATABUFF+Len,x+6,5);
									Len+=5;
								}
							}
						}
					}
					p8[1]=Len-2;//长度Le
					ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				}
			}
		}
		//清总加电量
		MC(0,ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn),4*MaxTotalAddGroup);
	}
	
	if(ERC_Event_Set(22)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8=(u8 *)(ADDR_ERC22_SOUR);//原差动组超限标志,0=没超,1=超限
		p8[Pn]=0;
	}
	Pn++;
	Terminal_Ram->ERC22Task=Pn;
#endif
}
#endif

#if ERC23==1
void ERC23_Event(u32 Pn,u32 Bin,u32 Type,u32 kWh1,u32 kWh2)//电控告警记录;入口Pn总加组号,Bin跳闸轮次,Type电控类别,kwh1告警时电能量,kwh2告警时电能量定值
{
	u32 i;
	u32 x;
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=23;//ERC
	p8[1]=16;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	x=0;
	for(i=0;i<MaxControlRound;i++)
	{
		x<<=1;
		x|=1;
	}
	p8[8]=Bin&x;
	p8[9]=Type;
	MWR(kWh1,ADDR_DATABUFF+10,4);
	MWR(kWh2,ADDR_DATABUFF+14,4);
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC24==1
void ERC24_Event(u32 Pn,u32 ss,u32 f)//电压越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
{
	u32 i;
	u8 * p8;
/*
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
//电压回路异常、电流回路异常、相序异常、电压/电流不平衡越限、电压越限、电流越限等6个事件，终端不要求对使用DL/T645-2007的测量点生成
	i=Check_ACSamplePn(Pn);//有效交采测量点号检查;返回0=无效,1=有效
	if(i==0)
	{
		return;
	}
#endif
*/
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=24;//ERC
	p8[1]=14;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(ss<<7);
	p8[9]=f;
	i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		MR(ADDR_DATABUFF+10,i+37,6);
		ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	}
}
#endif

#if ERC25==1
void ERC25_Event(u32 Pn,u32 ss,u32 f)//电流越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志
{
	u32 i;
	u8 * p8;
/*
#if EventProject==1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
//电压回路异常、电流回路异常、相序异常、电压/电流不平衡越限、电压越限、电流越限等6个事件，终端不要求对使用DL/T645-2007的测量点生成
	i=Check_ACSamplePn(Pn);//有效交采测量点号检查;返回0=无效,1=有效
	if(i==0)
	{
		return;
	}
#endif
*/
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=25;//ERC
	p8[1]=17;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(ss<<7);
	p8[9]=f;
	i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		MR(ADDR_DATABUFF+10,i+43,9);
		ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	}
}
#endif

#if ERC26==1
void ERC26_Event(u32 Pn,u32 ss,u32 f,u32 kva1,u32 kva2)//视在功率越限记录,入口:Pn测量点号(从1开始),ss=0恢复=1产生,f=异常标志,kva1=发生时的视在功率,kva2=发生时的视在功率限值
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=26;//ERC
	p8[1]=14;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(ss<<7);
	p8[9]=f;
	MWR(kva1,ADDR_DATABUFF+10,3);
	MWR(kva2,ADDR_DATABUFF+13,3);
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC27==1
void ERC27_Event(u32 Pn,u32 FnDataBuff)//电能表示度下降记录(在类1数据存贮前检查事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
{
	u32 i;
	u8 * p8;
	
	i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		p8=(u8 *)(i);
		if(p8[6]!=0xee)
		{
			p8=(u8 *)(FnDataBuff);
			if(p8[7]!=0xee)
			{
				i=DataComp(i+6,FnDataBuff+7,5);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
	#if (USER/100)==2//用户标识:山东版有事件恢复
				if(i==0)
				{//返回0=相等
					return;
				}
				if(i==0x1)
				{//1=Data1>Data2
					i=1;//产生
				}
				else
				{
					i=0;//恢复
				}
				p8=(u8*)(ADDR_ERC27_SOUR+((Pn-1)*LEN_ERC27_SOUR));
				if(i==p8[0])
				{//没变化
					return;
				}
				p8[0]=i;
				p8=(u8 *)(ADDR_DATABUFF+2048);//在类1数据存贮前检查事件,不能用DATABUFF缓冲,因交采数据也放在DATABUFF缓冲
				p8[0]=27;//ERC
				p8[1]=17;//长度Le
				MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
				p8[7]=Pn;
				p8[8]=(Pn>>8)|(i<<7);
				i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
				MR(ADDR_DATABUFF+2048+9,i+6,5);
				MR(ADDR_DATABUFF+2048+14,FnDataBuff+7,5);
				ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	#else
				if(i==0x1)
				{//1=Data1>Data2
					p8=(u8 *)(ADDR_DATABUFF+2048);//在类1数据存贮前检查事件,不能用DATABUFF缓冲,因交采数据也放在DATABUFF缓冲
					p8[0]=27;//ERC
					p8[1]=17;//长度Le
					MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
					p8[7]=Pn;
					p8[8]=(Pn>>8)|(0x80);
					i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
					MR(ADDR_DATABUFF+2048+9,i+6,5);
					MR(ADDR_DATABUFF+2048+14,FnDataBuff+7,5);
					ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				}
	#endif
			}
		}
	}
}
#endif



void ClrERC29SOUR(u32 Pn)//集中抄表失败清ERC29电能表飞走,超差事件原始数据
{
	if(Pn<=ERC29MaxPn)
	{
		Pn--;
		MC(0xee,ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*Pn),1);//判断时只用1字节
	}
}

#if ERC29==1
//用功率计算实际发生电量方法
void ERC29_Event(u32 Pn,u32 FnDataBuff)//电能表飞走,超差(在类1数据存贮前检查事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
{
	#if (USER/100)!=2//用户标识:不是山东版,山东版有专用的按日判断事件
	u32 i;
	u32 x;
	u32 y;
	u32 t;
	u32 Errf;
	u32 f;
	double f64;
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	
	if((Pn<=ERC29MaxPn)&&(Pn!=0))
	{
		p8s=(u8 *)(ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)));
		i=ERC_Event_Set(29);//检查事件记录设置,返回0=不记录,1=记录
		if(i==0)//检查事件记录设置,返回0=不记录,1=记录
		{
			p8s[5]=0;
		}
		x=ERC_Event_Set(28);//检查事件记录设置,返回0=不记录,1=记录
		if(x==0)//检查事件记录设置,返回0=不记录,1=记录
		{
			p8s[6]=0;
		}
		i|=x;
		if(i==0)
		{
			p8s[0]=0xee;
			return;
		}
		i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
		if(i==0)
		{
			return;
		}
		p8d=(u8 *)(i);
		if(p8d[6]!=0xee)
		{//有开始电量值
			if(p8s[0]!=0xee)
			{//有开始时间秒值
				p8=(u8*)(FnDataBuff);
				if(p8[6]!=0xee)
				{//有当前电能值
					MR(ADDR_DATABUFF,ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)),1);//上次抄表时间秒
					MR(ADDR_DATABUFF+1,i,5);//上次抄表时间分时日月年
					MW(ADDR_TYMDHMS,ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)),5);//更新上次抄表时间秒分时日月
					MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);//当前时间
					t=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
					if((t>>31)==0)
					{//差值为正
						i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
						if(i==0)
						{
							return;
						}
						i=MRR(i+5,3);//取总有功功率xx.xxxx KW
						if(i!=0xeeeeee)
						{//总有功功率有效
							if(i&0x800000)
							{//功率为负,反向
								i=0;
							}
							i=bcd_hex(i);
							f64=i;
							f64*=t;
							f64/=3600;//kws变为kwh,4位小数
							t=(u32)f64;//t=应走电量kwh,4位小数
							if(t<=100)
							{//应走电量kwh<=0.0100时不比较
								return;
							}
							x=MRR(((u32)p8d)+5+1,4);//读原示值xxxx.xxxx
							y=MRR(FnDataBuff+6+1,4);//读当前示值xxxx.xxxx
							x=bcd_hex(x);
							y=bcd_hex(y);
							if(y>=x)
							{//当前示值>=原示值
								i=y-x;//i=示值计算的电量
								y=i*10;
								y/=t;//示值计算的电量/实际应走电量=x.x
								y=hex_bcd(y);
								
								f=0;//飞走标志
								Errf=0;//超差标志
								x=MRR(ADDR_AFN04F59+1,1);//取电能表飞走阈值	数据格式22		l
								if(x==0)
								{//阈值=0
									x=~x;
								}
								if(y>x)
								{//(示值计算的电量/实际发发电量)>电能表飞走阈值,即判为电能表飞走
									f=1;//飞走标志
									p8s[5]&=0x0f;//清恢复延时计数
									p8s[6]|=0xf0;//超差恢复计数(马上恢复)
									if(p8s[6]&0x01)
									{//原超差已产生,则继续
										Errf=1;//超差标志
									}
								}
								else
								{
									x=MRR(ADDR_AFN04F59,1);//取电能表超差阈值	数据格式22		l
									if(x==0)
									{//阈值=0
										x=~x;
									}
									if(y>x)
									{//(示值计算的电量/实际发发电量)>电能表超差阈值,即判为电能表超差
										Errf=1;//超差标志
										p8s[6]&=0x0f;//清恢复延时计数
										p8s[5]|=0xf0;//飞走恢复计数(马上恢复)
									}
//									else
//									{//负超差
//										if(t>=i)
//										{
//											i=t-i;
//											y=i*10;
//											y/=t;//示值计算的电量/实际应走电量=x.x
//											y=hex_bcd(y);
//											if(y>x)
//											{//(示值计算的电量/实际发发电量)>电能表超差阈值,即判为电能表超差
//												Errf=1;//超差标志
//												p8s[6]&=0x0f;//清恢复延时计数
//												p8s[5]|=0xf0;//飞走恢复计数(马上恢复)
//											}
//										}
//									}
								}
							//记录
								//超差恢复记录
								if(Errf==0)
								{
									if((p8s[6]&0xf0)<0x30)
									{//恢复延时计数(376.1测试软件问题恢复事件需延时2抄表周期后产生)
										//2016.7又改成延时3抄表周期后产生
										p8s[6]+=0x10;
									}
									else
									{
										x=p8s[6]&0x1;
										if(x!=0)
										{
											if((x^Errf)!=0x0)
											{//变化
												p8s[6]&=0xfe;
												p8s[6]|=Errf;
												p8=(u8 *)(ADDR_DATABUFF+2048);
												p8[0]=28;//ERC
												p8[1]=18;//长度Le
												MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
												p8[7]=Pn;
												p8[8]=(Pn>>8)|(Errf<<7);
												MR(ADDR_DATABUFF+2048+9,((u32)p8d)+5+1,5);//超差发生时对应正向有功电能示值
												MR(ADDR_DATABUFF+2048+14,FnDataBuff+6+1,5);//超差发生时正向有功电能示值
												MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59,1);//取电能表超差阈值	数据格式22		l
												ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
											}
										}
									}
								}
								//飞走恢复记录
								if(f==0)
								{
									if((p8s[5]&0xf0)<0x20)
									{//恢复延时计数(376.1测试软件问题恢复事件需延时2抄表周期后产生)
										p8s[5]+=0x10;
									}	
									else
									{
										x=p8s[5]&0x1;
										if(x!=0)
										{
											if((x^f)!=0x0)
											{//变化
												p8s[5]&=0xfe;
												p8s[5]|=f;
												p8=(u8 *)(ADDR_DATABUFF+2048);
												p8[0]=29;//ERC
												p8[1]=18;//长度Le
												MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
												p8[7]=Pn;
												p8[8]=(Pn>>8)|(f<<7);
												MR(ADDR_DATABUFF+2048+9,((u32)p8d)+5+1,5);//飞走发生前正向有功电能示值
												MR(ADDR_DATABUFF+2048+14,FnDataBuff+6+1,5);//飞走发生后正向有功电能示值
												MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59+1,1);//取电能表飞走阈值	数据格式22		l
												ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
											}
										}
									}
								}
								//超差产生记录
								x=p8s[6]&0x1;
								if(x==0)
								{
									if((x^Errf)!=0x0)
									{//变化
										p8s[6]&=0xfe;
										p8s[6]|=Errf;
										p8=(u8 *)(ADDR_DATABUFF+2048);
										p8[0]=28;//ERC
										p8[1]=18;//长度Le
										MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
										p8[7]=Pn;
										p8[8]=(Pn>>8)|(Errf<<7);
										MR(ADDR_DATABUFF+2048+9,((u32)p8d)+5+1,5);//超差发生时对应正向有功电能示值
										MR(ADDR_DATABUFF+2048+14,FnDataBuff+6+1,5);//超差发生时正向有功电能示值
										MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59,1);//取电能表超差阈值	数据格式22		l
										ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
									}
								}
								//飞走产生记录
								x=p8s[5]&0x1;
								if(x==0)
								{
									if((x^f)!=0x0)
									{//变化
										p8s[5]&=0xfe;
										p8s[5]|=f;
										p8=(u8 *)(ADDR_DATABUFF+2048);
										p8[0]=29;//ERC
										p8[1]=18;//长度Le
										MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
										p8[7]=Pn;
										p8[8]=(Pn>>8)|(f<<7);
										MR(ADDR_DATABUFF+2048+9,((u32)p8d)+5+1,5);//飞走发生前正向有功电能示值
										MR(ADDR_DATABUFF+2048+14,FnDataBuff+6+1,5);//飞走发生后正向有功电能示值
										MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59+1,1);//取电能表飞走阈值	数据格式22		l
										ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
									}
								}
								
							}
						}
					}
				}
			}
		}
		MW(ADDR_TYMDHMS,ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)),5);//更新上次抄表时间秒分时日月
	}
	#endif
}
#endif

#if ERC30==1
void ERC30_Event(u32 Pn)//电能表停走事件,入口:Pn=测量点号(1开始)
{
	u32 i;
//	u32 x;
//	u32 Ib;
	u32 f;
	u32 t;//停走定时区间秒
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16;
	double f64;
	
	p16=(u16 *)(ADDR_ERC30_Timer);
	if((Pn<=ERC30MaxPn)&&(Pn!=0))
	{
		p8s=(u8 *)(ADDR_ERC30_SOUR+(LEN_ERC30_SOUR*(Pn-1)));
		if(ERC_Event_Set(30)==0)//检查事件记录设置,返回0=不记录,1=记录
		{
			p8s[0]=0xee;
			p8s[5]=0;
			return;
		}
		t=MRR(ADDR_AFN04F59+2,1);//电能表停走阈值	BIN	15min	l
		if(t==0x0)
		{//停走阈值=0
			p8s[0]=0xee;
			p8s[5]=0;
			return;
		}
		t*=(15*60);
		if(t>0xffff)
		{
			t=0xffff;
		}
		i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
		i+=6;
		p8d=(u8 *)(i);
		if(p8s[0]==0xee)
		{//无开始值
			MR((u32)p8s,(u32)p8d,5);
			p16[Pn-1]=t;
		}
		else
		{//有开始值
			if(p8d[0]!=0xee)
			{//当前有值
				f=DataComp((u32)p8s,(u32)p8d,5);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				if(f!=0x0)
				{//不停走
					f=0;//恢复
					MR((u32)p8s,(u32)p8d,5);
					p16[Pn-1]=t;//重新开始定时
				}
				else
				{
					//功率计算定时内应走电量>=0.04
					i=GetClass1DataAddr(25,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
					if(i==0)
					{
						return;
					}
					i=MRR(i+5,3);//取总有功功率xx.xxxx KW
					if(i!=0xeeeeee)
					{//总有功功率有效
						if(i&0x800000)
						{//功率为负,反向
							i=0;
						}
						i=bcd_hex(i);
						f64=i;
						f64*=t;
						f64/=3600;//kws变为kwh,4位小数
						i=(u32)f64;//t=应走电量kwh,4位小数
						if(i<400)
						{//应走电量kwh<0.0400时不比较
							p16[Pn-1]=t;//重新开始定时
							return;
						}
					}
/*
					Ib=MRR(ADDR_AFN04F25+(11*Pn)+6,1);//取额定电流(BCD x.x)
					if(Ib==0)
					{
						Ib=~Ib;
					}
					x=GetClass1DataAddr(25,Pn+1);//取类1数据存贮地址,返回0表示没找到,有效数据长度放Terminal_ClassDataLen_ADDR
					if(x!=0)
					{
						x+=5+24+14+1;
						for(i=0;i<3;i++)
						{
							t=MRR(x+(3*i),2);//xxx.xxx
							if(t!=0xeeee)
							{
								t&=0x7fff;//去电流符号位
								if(t>=Ib)
								{
									f=1;//产生
									break;
								}
							}
						}
					}
*/
					if(p16[Pn-1]==0x0)
					{//停走阈值定时到
						f=1;//产生
						p16[Pn-1]=t;//重新开始定时
					}
					else
					{
						return;
					}
				}
				t=MRR(ADDR_AFN04F59+2,1);//电能表停走阈值	BIN	15min	
				if((p8s[5]^f)!=0x0)
				{//停走变化
					p8s[5]=f;
					p8=(u8 *)(ADDR_DATABUFF);
					p8[0]=30;//ERC
					p8[1]=13;//长度Le
					MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
					p8[7]=Pn;
					p8[8]=(Pn>>8)|(f<<7);
					MR(ADDR_DATABUFF+9,(u32)p8s,5);
					p8[14]=t;
					ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				}
			}
		}
	}
}
#endif


#if ERC31==1
void ERC31_Event(u32 MeterNo,u32 Pn,u32 f)//终端485抄表失败事件,入口测量点号,f=1产生=0恢复
{
	u8 * p8;
	
	if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	MeterNo--;
	p8=(u8 *)(ADDR_RM_CountFlags+32+(MeterNo/4));
	MeterNo%=4;
	MeterNo<<=1;
	if(f==1)
	{//f=1产生
		if(((p8[0]>>MeterNo)&0x3)!=2)
		{
			return;
		}
#if (USER/100)==11//吉林系
#else
		p8=(u8 *)(ADDR_AFN0CF129+((6+(5*(1+TMaxTariff)))*(Pn-1)));
		if(p8[6]==0xee)
		{//无最近一次抄表成功正向有功总电能示值	
			return;
		}
#endif
	}
	else
	{//恢复
		if(((p8[0]>>MeterNo)&0x3)!=1)
		{
			return;
		}
	}

//ERC31抄表失败事件
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=31;//ERC
	p8[1]=21;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(f<<7);
	Pn--;
	MR(ADDR_DATABUFF+9,ADDR_AFN0CF129+((6+(5*(1+TMaxTariff)))*Pn),5);//最近一次抄表成功时间：分时日月年
	MR(ADDR_DATABUFF+14,ADDR_AFN0CF129+((6+(5*(1+TMaxTariff)))*Pn)+6,5);//最近一次抄表成功正向有功总电能示值	
	MR(ADDR_DATABUFF+19,ADDR_AFN0CF130+((6+(4*(1+TMaxTariff)))*Pn)+6,4);//最近一次抄表成功正向无功总电能示值
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC32==1
void ERC32_Event(u32 n1,u32 n2)//通信流量超门限事件,n1=当月已发生的通信流量,n2=月通信流量门限
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=32;//ERC
	p8[1]=13;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	MWR(n1,ADDR_DATABUFF+7,4);//字存到非对齐字节,入口数据地址,存入字节数
	MWR(n2,ADDR_DATABUFF+11,4);//字存到非对齐字节,入口数据地址,存入字节数
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC33==1
void ERC33_Event(u32 Pn,u32 FnDataBuff)//电能表运行状态字变位事件(在类1数据存贮前写事件),入口Pn=测量点号(1开始),Fn抄表数据缓冲地址
{
	u8 * p8;
	
 #if (USER/100)==0//电科院送检版
	if(Check_ACSamplePn(Pn)==1)//有效交采测量点号检查;返回0=无效,1=有效
	{
		return;//交采测量点不记录
	}
 #endif
	p8=(u8 *)(ADDR_DATABUFF+2048);//在类1数据存贮前写查事件,不能用DATABUFF缓冲,因交采数据也放在DATABUFF缓冲
	p8[0]=33;//ERC
	p8[1]=35;//长度Le
	MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8);
	MR(ADDR_DATABUFF+2048+9,FnDataBuff+6,28);
	ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

#if ERC34==1

#endif

#if ERC35==1
void ERC35_Event(u32 Port,u32 n,u32 DataAddr)//发现未知电表事件;入口Port=端口号,n=未知电表数,DataAddr=未知电表通信地址等每表8字节数据存放的地址
{
	u32 i;
	u8 * p8;

	while(n!=0x0)
	{
		i=((LEN_TERMINAL_EVENT)-9)/8; //64 用终端事件记录固定长度计算记录能存的未知电表数
		if(n<i)
		{
			i=n;
		}
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=35;//ERC
		p8[1]=7+(i*8);//长度Le
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
		p8[7]=Port;//端口号
		p8[8]=i;//电表数
		MR(ADDR_DATABUFF+9,DataAddr,i*8);
		ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
		n-=i;
		DataAddr+=(i*8);
	}
}
#endif

#if ERC36==1
void ERC36_Event(u32 CD,u32 ST)//控制输出回路开关接入状态量变位记录;入口：CD=变位,ST=状态
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=36;//ERC
	p8[1]=7;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=CD;//状态变位标志
	p8[8]=ST;//变位后状态
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录	
}
#endif

#if ERC37==1
void ERC37_Event(u32 Pn,u32 ADDR_DATA)//电能表开表盖事件记录;入口:Pn测量点号(从1开始)
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=37;//ERC
	p8[1]=70;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8);
	MR(ADDR_DATABUFF+9,ADDR_DATA,63);
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录	
}
#endif

#if ERC38==1
void ERC38_Event(u32 Pn,u32 ADDR_DATA)//电能表开端钮盒事件记录;入口:Pn测量点号(从1开始)
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=38;//ERC
	p8[1]=70;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=(Pn>>8);
	MR(ADDR_DATABUFF+9,ADDR_DATA,63);
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录	
}
#endif

#if ERC39==1
/*
void ERC39_Event(u32 Fn,u32 Pn,u32 f)//补抄失败事件记录;入口:当前抄读的FnPn,f=0抄表成功,f=1补抄失败
{
	u32 i;
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=39;//ERC
	p8[1]=21;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=Pn>>8;
	p8[8]|=(f<<7);//产生恢复标志
	MC(0xee,ADDR_DATABUFF+9,14);
	switch(Fn)
	{
		case 129://F129 当前正向有功电能示值
		//最近一次抄表成功时间：分时日月年	见附录A.15	5
		//最近一次抄表成功正向有功总电能示值	见附录A.14	5
			i=GetClass1DataAddr(129,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				MR(ADDR_DATABUFF+9,i,5);
				MR(ADDR_DATABUFF+14,i+6,5);
			}
		//最近一次抄表成功正向无功总电能示值	见附录A.11	4
			i=GetClass1DataAddr(130,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				MR(ADDR_DATABUFF+19,i+6,4);
			}
			break;
		//case 162://F162 电能表日历时钟
		case 0xe3://扩展3 集中抄表日冻结正向有功电能示值（总、费率1～M）
		//最近一次抄表成功时间：分时日月年	见附录A.15	5
		//最近一次抄表成功正向有功总电能示值	见附录A.14	5
		//最近一次抄表成功正向无功总电能示值	见附录A.11	4
			return;
		default:
			return;
	}
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
*/
void ERC39_Event(u32 Pn)//补抄失败事件记录
{
	u32 i;
	u32 x;
	u32 y;
	u32 YMD;
	u8 *p8;
	u32 OfficeAddr;
	
	i=MRR(ADDR_TCongealCount+14,2);
	if(i<4)
	{
		return;//日冻结数不到4天
	}
	p8=(u8*)(ADDR_ERC39_SOUR);//1byte ERC39补抄失败事件记录标志:0=空(上电或日更新),1=失败记录,2=成功记录
	if(p8[Pn-1]==1)
	{
		return;
	}
	if(ERC_Event_Set(39)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8[Pn-1]=0;
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(161,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	//查最近3日是否有抄表
	YMD=MRR(ADDR_TYMDHMS+3,3);
	YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
	for(i=0;i<3;i++)
	{
		YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
		x=GetDateCongealNo(YMD);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
		if(x!=0xffffffff)
		{//找到
			x=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,1);
			if((x!=0xff)&&(x!=0xee))
			{
				return;//在上3日内有抄表
			}
		}
	}
	//查最近是成功抄表
	for(i=0;i<MaxDateCongeal;i++)
	{
		YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
		x=GetDateCongealNo(YMD);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
		if(x!=0xffffffff)
		{//找到
			y=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,1);
			if((y!=0xff)&&(y!=0xee))
			{
				break;
			}
		}
	}
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=39;//ERC
	p8[1]=21;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	p8[7]=Pn;
	p8[8]=Pn>>8;
	p8[8]|=(1<<7);//产生恢复标志
	MC(0xee,ADDR_DATABUFF+9,14);
	if(i<MaxDateCongeal)
	{
		MR(ADDR_DATABUFF+9,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr,5);//最近一次抄表成功时间：分时日月年	见附录A.15	5
		Data_0xFFto0xEE(ADDR_DATABUFF+9,5);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
		MR(ADDR_DATABUFF+14,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,5);//最近一次抄表成功正向有功总电能示值	见附录A.14	5
		Data_0xFFto0xEE(ADDR_DATABUFF+14,5);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
		OfficeAddr=GetClass2DataOfficeAddr(162,Pn);//取类2数据存贮偏移地址,返回0表示没找到
		MR(ADDR_DATABUFF+19,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,4);//最近一次抄表成功正向无功总电能示值	见附录A.11	4
		Data_0xFFto0xEE(ADDR_DATABUFF+19,4);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
	}
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	p8=(u8*)(ADDR_ERC39_SOUR);//1byte ERC39补抄失败事件记录标志:0=空(上电或日更新),1=失败记录,2=成功记录
	p8[Pn-1]=1;
}

void ERC39_Event_Cancel(u32 Pn)//补抄失败事件恢复
{
	u32 i;
	u32 x;
	u32 y;
	u32 YMD;
	u8 *p8;
	u32 OfficeAddr;

	p8=(u8*)(ADDR_ERC39_SOUR);//1byte ERC39补抄失败事件记录标志:0=空(上电或日更新),1=失败记录,2=成功记录
	if(p8[Pn-1]!=1)
	{
		return;
	}
	if(ERC_Event_Set(39)==0)//检查事件记录设置,返回0=不记录,1=记录
	{
		p8[Pn-1]=0;
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(161,Pn);//取类2数据存贮偏移地址,返回0表示没找到
	//查最近3日是否有抄表
	YMD=MRR(ADDR_TYMDHMS+3,3);
	YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
	for(i=0;i<3;i++)
	{
		YMD=YMD_Sub1D(YMD);//年月日减1日,返回减1后的年月日
		x=GetDateCongealNo(YMD);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
		if(x!=0xffffffff)
		{//找到
			y=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,1);
			if((y!=0xff)&&(y!=0xee))
			{
				//在上3日内有抄表
				p8[Pn-1]=2;//2=成功
				p8=(u8 *)(ADDR_DATABUFF);
				p8[0]=39;//ERC
				p8[1]=21;//长度Le
				MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
				p8[7]=Pn;
				p8[8]=Pn>>8;
				MR(ADDR_DATABUFF+9,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr,5);//最近一次抄表成功时间：分时日月年	见附录A.15	5
				MR(ADDR_DATABUFF+14,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,5);//最近一次抄表成功正向有功总电能示值	见附录A.14	5
				MC(0xee,ADDR_DATABUFF+19,4);//最近一次抄表成功正向无功总电能示值	见附录A.11	4
				ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
				return;
			}
		}
	}
}
#endif

#if ERC40==1
void ERC40_Meter_Event(u32 FnDataAddr,u32 Pn)//ERC40直接读电能表磁场异常事件记录;入口:Fn数据首地址,测量点号(1开始)
{
	u8 *p8;
	if(Pn<=ERC40MaxPn)
	{
		Pn--;
		p8=(u8 *)(ADDR_DATABUFF+2048);
		p8[0]=40;//ERC
		p8[1]=15;//长度Le
		MR(ADDR_DATABUFF+2048+2,FnDataAddr+15,5);//分时日月年
		Pn++;
		p8[7]=Pn;
		p8[8]=(Pn>>8)|(1<<7);
		p8[9]=2;//设备类型：2=电能表
		MR(ADDR_DATABUFF+2048+10,FnDataAddr+6,6);//设备地址
		p8[16]=2;//磁场异常类型:2=强磁场
		ERC_Event_Save(ADDR_DATABUFF+2048);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
	}
}
#endif

#if ERC41==1
void ERC41_Event(u32 ADDR_BUFF)//对时事件记录;入口:ADDR_BUFF的第2字节开始分别为2BYTE测量点号,6BYTE对时前时间,6BYTE对时后时间
{
	u8 * p8;
	p8=(u8 *)(ADDR_BUFF);
	p8[0]=41;//ERC
	p8[1]=14;//长度Le
	ERC_Event_Save(ADDR_BUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#if (USER/100)==5//上海
void ERC41_Meter_Event(u32 FnDataAddr,u32 Pn)//ERC41直接读电能表对时事件记录;入口:Fn数据首地址,测量点号(1开始)
{
	u32 x;
	u32 y;

	if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	//在每日15:00的抄表周期结束后比对校时次数生成记录
	x=MRR(ADDR_TYMDHMS,3);//当前时分秒
	if(x<0x150000)
	{
		return;
	}
	x=MRR(ADDR_TYMDHMS+3,2);//当前月日
	y=MRR(ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*(Pn-1)),2);//原比对月日
	if(x==y)
	{
		return;
	}
	MWR(x,ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*(Pn-1)),2);
	
	y=MRR(FnDataAddr+6,3);//当前电能表时钟修改次数
	if(y!=0xeeeeee)
	{
		x=MRR(ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*(Pn-1))+2,3);//原电能表时钟修改次数
		MWR(y,ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*(Pn-1))+2,3);//更新原电能表时钟修改次数
		if(x!=0xeeeeee)
		{
			if(y!=0)
			{//当前电能表时钟修改次数!=0
				if(x!=y)
				{//次数变化
					MWR(Pn,ADDR_DATABUFF+2,2);
					MR(ADDR_DATABUFF+4,FnDataAddr+6+3,12);
					ERC41_Event(ADDR_DATABUFF);//对时事件记录;入口:ADDR_BUFF的第2字节开始分别为2BYTE测量点号,6BYTE对时前时间,6BYTE对时后时间
				}
			}
		}
	}
}
#else//#if (USER/100)==5//上海
void ERC41_Meter_Event(u32 FnDataAddr,u32 Pn)//ERC41直接读电能表对时事件记录;入口:Fn数据首地址,测量点号(1开始)
{
	u32 i;
	u32 x;
	u32 y;

	i=GetClass1DataAddr(166,Pn);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
	if(i!=0)
	{
		x=MRR(i+5,3);//原电能表时钟修改次数
		if(x!=0xeeeeee)
		{
			y=MRR(FnDataAddr+6,3);//当前电能表时钟修改次数
			if(y!=0xeeeeee)
			{
				if(y!=0)
				{//当前电能表时钟修改次数!=0
					if(x!=y)
					{//次数变化
						MWR(Pn,ADDR_DATABUFF+2,2);
						MR(ADDR_DATABUFF+4,FnDataAddr+6+3,12);
						ERC41_Event(ADDR_DATABUFF);//对时事件记录;入口:ADDR_BUFF的第2字节开始分别为2BYTE测量点号,6BYTE对时前时间,6BYTE对时后时间
					}
				}
			}
		}
	}
}
#endif
#endif


#if ERC45==1
void ERC45_Event(void)//终端电池失效事件
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=45;//ERC
	p8[1]=5;//长度Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	ERC_Event_Save(ADDR_DATABUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif

void PerERC_Event_Save(u32 BuffAddr)//分类ERC事件记录
{
	u32 i;
	u32 x;
	u8 * p8s;
	
	p8s=(u8 *)(BuffAddr);
	i=p8s[0];
	i-=1;
	if(i>(MaxERC-1))
	{
		return;
	}
	MR(ADDR_DATABUFF_ERAM,ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT))),2);
	x=MRR(ADDR_DATABUFF_ERAM,2);
	if(x<MaxClassEventNum)
	{
		MR(ADDR_DATABUFF_ERAM+2,ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)))+2,MaxClassEventNum*LEN_TERMINAL_EVENT);
	}
	else
	{
		MR(ADDR_DATABUFF_ERAM+2,ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)))+2+LEN_TERMINAL_EVENT,(MaxClassEventNum-1)*LEN_TERMINAL_EVENT);
	}
	x++;
	if(x>MaxClassEventNum)
	{
		x=MaxClassEventNum;
	}
	MWR(x,ADDR_DATABUFF_ERAM,2);
	x--;
	MR(ADDR_DATABUFF_ERAM+2+(LEN_TERMINAL_EVENT*x),BuffAddr,LEN_TERMINAL_EVENT);
	MW(ADDR_DATABUFF_ERAM,ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT))),2+(MaxClassEventNum*LEN_TERMINAL_EVENT));
}

#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
void ERC51_Event(u32 Pn,u32 ADDR_BUFF)//校时记录事件;入口:Pn测量点号(从0开始)
{
	u8 * p8;
	p8=(u8 *)(ADDR_BUFF);
	p8[0]=51;//ERC
	p8[1]=12;//长度Le
	MR(ADDR_BUFF+2,ADDR_TYMDHMS+1,5);//分时日月年
	MR(ADDR_AFN04F299_Ext+28+1+4*25+4+1,ADDR_TYMDHMS,6);
	p8[7]=Pn;
	p8[8]=(Pn>>8);
	ERC_Event_Save(ADDR_BUFF);//类3数据(事件)存贮,自动识别重要或一般;返回0没记录,1记录
}
#endif


