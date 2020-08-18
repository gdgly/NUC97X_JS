
//uart类2数据
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_Uart_TimerTask.h"
#include "KeyUser.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "Terminal_ReadMeter_AFN0D_RealTime.h"

#include "../Terminal/Terminal_USER5xx.h"





extern Terminal_Class2DataLib_TypeDef Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);

/*
u32 TINUM(u32 PORTn,u32 ALLPn,u32 Fn,u32 Pn)//当前数据标识对应前有时标个数计算;返回：前有时标个数
{
	u32 i;
	u32 x;
	u32 y;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址

	if(ALLPn==0)//全部有效量点标志;0=否,1=是
	{//本数据标识为全部有效测量点
		if(Pn==0)
		{//p0点Fn组合
			i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//计算置位为1的个数(0-32)
		}
		else
		{//非p0点Fn组合
			i=MRR(((u32)(&UARTCtrl->SourDI)),1);//计算同类型Pn组合本Pn前有Pn个数
			x=(Pn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//计算置位为1的个数(0-32)
			x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合个数
			x=BitNum(x);//计算置位为1的个数(0-32)
			i*=x;
			y=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			x&=y;
			x=BitNum(x);//计算置位为1的个数(0-32)
			i+=x;
		}
	}
	else
	{
		i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
		x=(Fn-1)%8;
		x=0xff<<x;
		x=~x;
		i&=x;
		i=BitNum(i);//计算置位为1的个数(0-32)	
	}
	return i;
}	
*/




void Uart_AFN0D(u32 PORTn)//类2数据
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 MapPn;
	u32 RXDIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
	u32 OfficeAddr;
	u32 m;
	u32 FnLen;
  u32 LEN_UARTnTx;
	u32 SourDI;//本次分解前的DI
	u32 TIlen;//时标长度
	u32 TIOffice;//时标偏移
	u32 ALLPn;//全部有效量点标志，0=否,1=是
	u32 PointNum;//曲线最多能填入缓冲区的点数
	u64 YMDHM;
	u64 NextYMDHM;
	u16 *p16;
	u32 DataBuffDate;//当前UART数据缓冲调入冻结数据的数据日期
#if MaxACSampleCurveCongeal!=0//最大交采1分钟冻结曲线点数(可设为0表示不用)
	u32 ACSamplePn;
#endif
#if (USER/100)==11//吉林 
	u32 port;
#endif
#if (USER/100)==12//四川 
	u8 * p8;	
#endif
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	DataBuffDate=0;//当前UART数据缓冲调入冻结数据的数据日期
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址
  LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0d;//AFN(为定时上报帧填写）
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			if(DIlen<(12+6))
			{
	err:
				//全部否认
				p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
				Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
				p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				return;
			}
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
			if(i!=0x0)
			{
				return;//舍弃该报文
			}
		}
	}
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识长度
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		if((p8rxbuff[13]&0x80)!=0x0)
		{//有Tp
			DIlen-=(8+6);
		}
		else
		{
			DIlen-=(8);
		}
		if(DIlen<4)
		{
			goto err;
		}
	}
	else
	{
		DIlen=UARTCtrl->DIlen;//数据标识长度寄存器
	}
//数据(启动站功能码11（请求2级数据）用于应用层请求数据的链路传输)
	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		UARTCtrl->DIOffset=0;//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
		CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
		MW(RXDIaddr+4,(u32)&UARTCtrl->TI,7);//寄存全部测量点时曲线的时标(年月日时分),密度,点数
	}
	RXDIaddr+=UARTCtrl->DIOffset;
	RXaddr=RXDIaddr+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=TXDIaddr+4;
	TXlen=0;
	TIlen=0;//时标长度
	f=0;//有无有效数据填入标志;0=无,1=有
	MC(0,TXDIaddr,4);
	i=MRR((u32)(&UARTCtrl->SourDI),2);
	if(i!=0x00ff)
	{//本数据标识为全部有效测量点
		ALLPn=0;//全部有效量点标志，0=否,1=是
	}
	else
	{
		ALLPn=1;//全部有效量点标志，0=否,1=是
	}
	
#if MaxKeyUser!=0//最大集中抄表重点用户数
	KeyUserPnList();//集中抄表重点户曲线冻结Pn列表
#endif

	while(DIlen>=4)
	{
		SourDI=MRR((u32)(&UARTCtrl->DestDI),4);//本次分解前的DI
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
			if(ALLPn==0)//全部有效量点标志，0=否,1=是
			{
				//减总时标长度,每Fn的每Pn一个时标
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//计算Pn组合个数
				i=BitNum(i);//计算置位为1的个数(0-32)
				if(i==0)
				{
					i=1;//p0点
				}
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算Fn组合个数
				x=BitNum(x);//计算置位为1的个数(0-32)
				i*=x;//总个数
			}
			else
			{//本数据标识为全部有效测量点
				//减总时标长度,每Fn一个时标
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算Fn组合个数
				i=BitNum(i);//计算置位为1的个数(0-32)
			}
			if(TIlen==0)//时标长度
			{//前面还没有一个标识正确过,未知本标识时标长度
				goto err;
			}
		#if(USER/100)==7//浙江用户
			if(UARTCtrl->LockFlags&0x30)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B5-B7保留
			{//主动定时任务
				i=TIlen;//时标长度
			}
			else
			{
				i*=TIlen;//时标长度
			}
		#else
			i*=TIlen;//时标长度
		#endif
			RXaddr+=i;//接收地址+总时标长度
			if(DIlen>=(i+4))
			{
				DIlen-=i;
			}
			else
			{
				goto err;
			}
			RXDIaddr=RXaddr;
			UARTCtrl->DIOffset=RXDIaddr-(Get_ADDR_UARTnRx(PORTn)+14);//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
			CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
			i=MRR((u32)(&UARTCtrl->SourDI),2);
			if(i!=0x00ff)
			{//本数据标识为全部有效测量点
				ALLPn=0;//全部有效量点标志，0=否,1=是
			}
			else
			{
				ALLPn=1;//全部有效量点标志，0=否,1=是
				MW(RXaddr+4,(u32)&UARTCtrl->TI,7);//寄存全部测量点时曲线的时标(年月日时分),密度,点数
			}
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
			}
			MC(0,TXDIaddr,4);
			continue;
		}
		
		Fn=(i>>16);
		Pn=i&0xffff;

	#if AFN0DF1MaxPn!=0
		if(ALLPn!=0)//全部有效量点标志;0=否,1=是
		{
			i=Terminal_Class2DataLib[Fn].PnMax;
			if(Fn==246)
			{//当前掉电记录数据(Pn映射)
				i=MaxRS485AddCarrierPn;
			}
			if(Pn>i)
			{//全部Pn完
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			switch(Terminal_Class2DataLib[Fn].PnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
			{
//				case p0Type://终端信息点
//					break;
				case pnType://测量点号
					if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
				case GnType://总加组号
					if(GroupTotalSet(Pn)==0)//有效总加组号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
//				case CnType://电容器测量点号
//				case DnType://直流模拟量号
//				case RnType://控制轮次号
//				case TnType://任务号
//				case EnType;//事件号
//					break;
			}
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
				MC(0,TXDIaddr,4);
			}
		}
		else
		{
			switch(Terminal_Class2DataLib[Fn].PnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
			{
//				case p0Type://终端信息点
//					break;
				case pnType://测量点号
					if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
				case GnType://总加组号
					if(GroupTotalSet(Pn)==0)//有效总加组号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
//				case CnType://电容器测量点号
//				case DnType://直流模拟量号
//				case RnType://控制轮次号
//				case TnType://任务号
//				case EnType;//事件号
//					break;
			}
		}
	#else//#if AFN0DF1MaxPn!=0
		if(ALLPn!=0)//全部有效量点标志;0=否,1=是
		{
			if((GetClass2DataCongealType(Fn)==3)&&(Terminal_Class2DataLib[Fn].PnType==pnType))//测量点号类的曲线
			{//测量点号类的曲线
				if(Pn>MaxRS485AddCarrierPn)
				{//全部Pn完
					MC(0,(u32)(&UARTCtrl->DestDI),4);
					continue;
				}
			}
			else
			{//
				i=Terminal_Class2DataLib[Fn].PnMax;
				if(Fn==246)
				{//当前掉电记录数据(Pn映射)
					i=MaxRS485AddCarrierPn;
				}
				//以下Fn在集中抄表为节省FLASH将MaxPn设为0,但可由其他分项合成
				switch(Fn)
				{
				//2类数据.日冻结
					case 1://F1测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					case 2://F2测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					case 3://F3测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					case 4://F4测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				#if AFN0DF5MaxPn==0
					case 5://测量点正向有功电能量（总、费率1～M）
					case 6://测量点正向无功电能量（总、费率1～M）
					case 7://测量点反向有功电能量（总、费率1～M）
					case 8://测量点反向无功电能量（总、费率1～M）
				#endif
				//2类数据.抄表日冻结
					case 9://F9测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					case 10://F10测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					case 11://F11测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					case 12://F12测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				//2类数据.月冻结
					case 17://F17测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					case 18://F18测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					case 19://F19测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					case 20://F20测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
				#if AFN0DF21MaxPn==0
					case 21://测量点正向有功电能量（总、费率1～M）
					case 22://测量点正向无功电能量（总、费率1～M）
					case 23://测量点反向有功电能量（总、费率1～M）
					case 24://测量点反向无功电能量（总、费率1～M）
				#endif
				/*
				#if AFN0DF97MaxPn==0
					case 97://正向有功总电能量曲线
					case 98://正向无功总电能量曲线
					case 99://反向有功总电能量曲线
					case 100://反向无功总电能量曲线
				#endif
				*/
						i=AFN0DF161MaxPn;
						break;
				
				}
				if(Pn>i)
				{//全部Pn完
					MC(0,(u32)(&UARTCtrl->DestDI),4);
					continue;
				}
				
			}
			switch(Terminal_Class2DataLib[Fn].PnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
			{
//				case p0Type://终端信息点
//					break;
				case pnType://测量点号
					if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
					{
						continue;
					}
					if((GetClass2DataCongealType(Fn)==3)&&(Terminal_Class2DataLib[Fn].PnType==pnType))//测量点号类的曲线
					{//测量点号类的曲线
						if(KeyUserPnSet(Pn)==0)//重点户有效测量点号检查;返回0=无效,1=有效;使用重点户曲线冻结Pn列表ADDR_KeyUserPnList
						{
							continue;
						}
					}
					break;
				case GnType://总加组号
					if(GroupTotalSet(Pn)==0)//有效总加组号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
//				case CnType://电容器测量点号
//				case DnType://直流模拟量号
//				case RnType://控制轮次号
//				case TnType://任务号
//				case EnType;//事件号
//					break;
			}
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
				MC(0,TXDIaddr,4);
			}
		}
		else
		{
			switch(Terminal_Class2DataLib[Fn].PnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
			{
//				case p0Type://终端信息点
//					break;
				case pnType://测量点号
					if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
					{
						continue;
					}
					if((GetClass2DataCongealType(Fn)==3)&&(Terminal_Class2DataLib[Fn].PnType==pnType))//测量点号类的曲线
					{//测量点号类的曲线
						if(KeyUserPnSet(Pn)==0)//重点户有效测量点号检查;返回0=无效,1=有效;使用重点户曲线冻结Pn列表ADDR_KeyUserPnList
						{
							continue;
						}
					}
					break;
				case GnType://总加组号
					if(GroupTotalSet(Pn)==0)//有效总加组号检查;返回0=无效,1=有效
					{
						continue;
					}
					break;
//				case CnType://电容器测量点号
//				case DnType://直流模拟量号
//				case RnType://控制轮次号
//				case TnType://任务号
//				case EnType;//事件号
//					break;
			}
		}
	#endif



		switch(GetClass2DataCongealType(Fn))//取类2数据冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
		{
			case 0://0=日冻结
			case 1://1=抄表日冻结
				TIlen=3;//时标长度
				break;
			case 2://1=月冻结
				TIlen=2;//时标长度
				break;
			case 3://1=曲线
				TIlen=7;//时标长度
				break;
			default:
				goto err;
		}
		if(ALLPn==0)//全部有效量点标志;0=否,1=是
		{//本数据标识为全部有效测量点
			if(Pn==0)
			{//p0点Fn组合
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//计算置位为1的个数(0-32)
			}
			else
			{//非p0点Fn组合
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//计算同类型Pn组合本Pn前有Pn个数
				x=(Pn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//计算置位为1的个数(0-32)
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合个数
				x=BitNum(x);//计算置位为1的个数(0-32)
				i*=x;
				y=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				x&=y;
				x=BitNum(x);//计算置位为1的个数(0-32)
				i+=x;
			}
		}
		else
		{
			i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//计算置位为1的个数(0-32)
		}
	#if(USER/100)==7//浙江用户
		if(UARTCtrl->LockFlags&0x30)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B5-B7保留
		{//主动定时任务
			TIOffice=0;//时标偏移
		}
		else
		{
			TIOffice=i*TIlen;//时标偏移
		}
	#else
		TIOffice=i*TIlen;//时标偏移
	#endif
		if(DIlen<(TIOffice+TIlen+4))
		{
			DIlen=0;
			continue;
		}
		
	#if AFN0DF1MaxPn!=0
		if(Fn==219)//F219 测量点组合数据曲线
		{//Fn=219测量点组合数据曲线
			if((Pn==0)||(Pn>Terminal_Class2DataLib[Fn].PnMax))
			{
				continue;
			}
		}
		else
		{//非Fn=219测量点组合数据曲线
			OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
			if(OfficeAddr==0x0)
			{
				continue;
			}
		}
	#else
		if(Fn==219)//F219 测量点组合数据曲线
		{//Fn=219测量点组合数据曲线
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				continue;
			}
		}
		else
		{//非Fn=219测量点组合数据曲线
			if((GetClass2DataCongealType(Fn)==3)&&(Terminal_Class2DataLib[Fn].PnType==pnType))//测量点号类的曲线
			{//测量点号类的曲线
				OfficeAddr=GetClass2DataOfficeAddr(Fn,1);//因测量点映射先固定用Pn=1
				if(OfficeAddr==0x0)
				{
					continue;
				}
			}
			else
			{
				OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//取类2数据存贮偏移地址,返回0表示没找到
				if(OfficeAddr==0x0)
				{
					//以下Fn在集中抄表为节省FLASH将MaxPn设为0,但可由其他分项合成
					switch(Fn)
					{
					//2类数据.日冻结
						case 1://F1测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
						case 2://F2测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
						case 3://F3测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						case 4://F4测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					#if AFN0DF5MaxPn==0
						case 5://F5测量点正向有功电能量（总、费率1～M）
						case 6://F6测量点正向无功电能量（总、费率1～M）
						case 7://F7测量点反向有功电能量（总、费率1～M）
						case 8://F8测量点反向无功电能量（总、费率1～M）
					#endif
					//2类数据.抄表日冻结
						case 9://F9测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
						case 10://F10测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
						case 11://F11测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						case 12://F12测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					//2类数据.月冻结
						case 17://F17测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
						case 18://F18测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
						case 19://F19测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						case 20://F20测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					#if AFN0DF21MaxPn==0
						case 21://F21测量点正向有功电能量（总、费率1～M）
						case 22://F22测量点正向无功电能量（总、费率1～M）
						case 23://F23测量点反向有功电能量（总、费率1～M）
						case 24://F24测量点反向无功电能量（总、费率1～M）
					#endif
					/*
					#if AFN0DF97MaxPn==0
						case 97://正向有功总电能量曲线
						case 98://正向无功总电能量曲线
						case 99://反向有功总电能量曲线
						case 100://反向无功总电能量曲线
					#endif
					*/
							break;
				#if (USER/100)==5//上海
						case 235://终端AB线电压曲线（扩展）
							OfficeAddr=ADDR_AFN0DF235;
							break;
						case 236://终端BC线电压曲线（扩展）
							OfficeAddr=ADDR_AFN0DF236;
							break;
						case 237://终端AC线电压曲线（扩展）
							OfficeAddr=ADDR_AFN0DF237;
							break;
				#endif
						default:
							continue;	
					}
				}
			}
		}
	#endif
		
		switch(GetClass2DataCongealType(Fn))//取类2数据冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
		{
			case 0://0=日冻结
			#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
				//上1日
				x=MRR(ADDR_TYMDHMS+3,3);
				x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
				if(MRR(RXaddr+TIOffice,3)==x)
				{//时标=(当前实时钟的上1日)
					i=GetDateCongealNo(x);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
					i=PnDateCongealNo(i,Pn);//测量点日冻结号有效,返回=0xffffffff表示无效(没找到)
					if(i!=0xffffffff)
					{//有上1日冻结
						if(Fn==246)
						{//日冻结掉电记录数据(测量点映射)
							OfficeAddr=GetClass2DataF246OfficeAddr(i,Pn);//取类2数据F246存贮偏移地址,返回0表示没找到
							if(OfficeAddr==0)
							{//没找到
								continue;
							}
						}
						if(MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+ADDR_DATECONGEALFLAG,1)==0xff)
						{//冻结标志是补冻结,只有电表的测量点类会补,其他非电表测量点作无数据避免将0xff转为0xee
							if(Terminal_Class2DataLib[Fn].PnType!=pnType)
							{//不是测量点类
								continue;
							}
						}
						if(DataBuffDate!=x)//当前UART数据缓冲调入冻结数据的数据日期
						{
							DataBuffDate=x;//当前UART数据缓冲调入冻结数据的数据日期
							MR(ADDR_AFN0D_UART_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//读原上上日冻结数据
							x=MRR(ADDR_TYMDHMS+3,3);//有效抄表时间
							Source_Full_CongealBuff(ADDR_AFN0D_UART_Buff_Start,0,0,x,0);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
						}
						Class2_Group(ADDR_AFN0D_UART_Buff_Start,Fn,Pn);//2类数据合成;出口:数据放ADDR_DATABUFF
						
						x=GetClass2DataLen(Fn,Pn,ADDR_AFN0D_UART_Buff_Start+OfficeAddr);//取类2数据长度
						if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
						{
							if(ALLPn==0)//全部有效量点标志;0=否,1=是
							{
								TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
							}
							MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
							goto MulTx;//发送-还有后续帧
						}
						MR(TXaddr,ADDR_AFN0D_UART_Buff_Start,3);//先读3字节冻结时标
						
						//以下Fn在集中抄表为节省FLASH将MaxPn设为0,但可由其他分项合成
						switch(Fn)
						{
						//2类数据.日冻结
							case 1://F1测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
							case 2://F2测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
							case 3://F3测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
							case 4://F4测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
								MR(TXaddr+3,ADDR_DATABUFF,x);//再读x字节冻结数据
								break;
							default:
								MR(TXaddr+3,ADDR_AFN0D_UART_Buff_Start+OfficeAddr,x);//再读x字节冻结数据
								break;
						}
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//是测量点类
							Data_0xFFto0xEE(TXaddr+3,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						}
						//当前取得2类数据的费率数或谐波次数等修正
						i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
						if(i&0x10000)
						{//需修正
							MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
						}
					#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
						{
							if(CheckUartData_0xEE(TXaddr+3,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
							{
								continue;
							}
						}
					#endif
						TXlen+=3+x;
						TXaddr+=3+x;
						break;
					}
					//没找到
					continue;
				}
				//上2日
				x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
				if(MRR(RXaddr+TIOffice,3)==x)
				{//时标=(当前实时钟的上2日)
					i=GetDateCongealNo(x);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
					i=PnDateCongealNo(i,Pn);//测量点日冻结号有效,返回=0xffffffff表示无效(没找到)
					if(i!=0xffffffff)
					{//有上2日冻结
						if(Fn==246)
						{//日冻结掉电记录数据(测量点映射)
							OfficeAddr=GetClass2DataF246OfficeAddr(i,Pn);//取类2数据F246存贮偏移地址,返回0表示没找到
							if(OfficeAddr==0)
							{//没找到
								continue;
							}
						}
						if(MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+ADDR_DATECONGEALFLAG,1)==0xff)
						{//冻结标志是补冻结,只有电表的测量点类会补,其他非电表测量点作无数据避免将0xff转为0xee
							if(Terminal_Class2DataLib[Fn].PnType!=pnType)
							{//不是测量点类
								continue;
							}
						}
						if(DataBuffDate!=x)//当前UART数据缓冲调入冻结数据的数据日期
						{
							DataBuffDate=x;//当前UART数据缓冲调入冻结数据的数据日期
							MR(ADDR_AFN0D_UART_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//读原上上日冻结数据
							x=YMD_Add1D(x);//年月日加1日,返回加1后的年月日
							LastSource_Full_CongealBuff(ADDR_AFN0D_UART_Buff_Start,x);//上2-3日抄表原始数据填入冻结缓冲;入口:LastYMD=电表冻结时标;返回:0=没补抄填入,1=有
						}
						Class2_Group(ADDR_AFN0D_UART_Buff_Start,Fn,Pn);//2类数据合成;出口:数据放ADDR_DATABUFF
						
						x=GetClass2DataLen(Fn,Pn,ADDR_AFN0D_UART_Buff_Start+OfficeAddr);//取类2数据长度
						if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
						{
							if(ALLPn==0)//全部有效量点标志;0=否,1=是
							{
								TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
							}
							MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
							goto MulTx;//发送-还有后续帧
						}
						MR(TXaddr,ADDR_AFN0D_UART_Buff_Start,3);//先读3字节冻结时标
						
						//以下Fn在集中抄表为节省FLASH将MaxPn设为0,但可由其他分项合成
						switch(Fn)
						{
						//2类数据.日冻结
							case 1://F1测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
							case 2://F2测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
							case 3://F3测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
							case 4://F4测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
								MR(TXaddr+3,ADDR_DATABUFF,x);//再读x字节冻结数据
								break;
							default:
								MR(TXaddr+3,ADDR_AFN0D_UART_Buff_Start+OfficeAddr,x);//再读x字节冻结数据
								break;
						}
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//是测量点类
							Data_0xFFto0xEE(TXaddr+3,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						}
						//当前取得2类数据的费率数或谐波次数等修正
						i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
						if(i&0x10000)
						{//需修正
							MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
						}
					#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
						{
							if(CheckUartData_0xEE(TXaddr+3,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
							{
								continue;
							}
						}
					#endif
						TXlen+=3+x;
						TXaddr+=3+x;
						break;
					}
					//没找到
					continue;
				}
				//上3日
				x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
				if(MRR(RXaddr+TIOffice,3)==x)
				{//时标=(当前实时钟的上3日)
					i=GetDateCongealNo(x);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
					i=PnDateCongealNo(i,Pn);//测量点日冻结号有效,返回=0xffffffff表示无效(没找到)
					if(i!=0xffffffff)
					{//有上3日冻结
						if(Fn==246)
						{//日冻结掉电记录数据(测量点映射)
							OfficeAddr=GetClass2DataF246OfficeAddr(i,Pn);//取类2数据F246存贮偏移地址,返回0表示没找到
							if(OfficeAddr==0)
							{//没找到
								continue;
							}
						}
						if(MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+ADDR_DATECONGEALFLAG,1)==0xff)
						{//冻结标志是补冻结,只有电表的测量点类会补,其他非电表测量点作无数据避免将0xff转为0xee
							if(Terminal_Class2DataLib[Fn].PnType!=pnType)
							{//不是测量点类
								continue;
							}
						}
						if(DataBuffDate!=x)//当前UART数据缓冲调入冻结数据的数据日期
						{
							DataBuffDate=x;//当前UART数据缓冲调入冻结数据的数据日期
							MR(ADDR_AFN0D_UART_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//读原上上日冻结数据
							x=YMD_Add1D(x);//年月日加1日,返回加1后的年月日
							LastSource_Full_CongealBuff(ADDR_AFN0D_UART_Buff_Start,x);//上2-3日抄表原始数据填入冻结缓冲;入口:LastYMD=电表冻结时标;返回:0=没补抄填入,1=有
						}
						Class2_Group(ADDR_AFN0D_UART_Buff_Start,Fn,Pn);//2类数据合成;出口:数据放ADDR_DATABUFF
						
						x=GetClass2DataLen(Fn,Pn,ADDR_AFN0D_UART_Buff_Start+OfficeAddr);//取类2数据长度
						if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
						{
							if(ALLPn==0)//全部有效量点标志;0=否,1=是
							{
								TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
							}
							MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
							goto MulTx;//发送-还有后续帧
						}
						MR(TXaddr,ADDR_AFN0D_UART_Buff_Start,3);//先读3字节冻结时标
						
						//以下Fn在集中抄表为节省FLASH将MaxPn设为0,但可由其他分项合成
						switch(Fn)
						{
						//2类数据.日冻结
							case 1://F1测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
							case 2://F2测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
							case 3://F3测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
							case 4://F4测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
								MR(TXaddr+3,ADDR_DATABUFF,x);//再读x字节冻结数据
								break;
							default:
								MR(TXaddr+3,ADDR_AFN0D_UART_Buff_Start+OfficeAddr,x);//再读x字节冻结数据
								break;
						}
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//是测量点类
							Data_0xFFto0xEE(TXaddr+3,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						}
						//当前取得2类数据的费率数或谐波次数等修正
						i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
						if(i&0x10000)
						{//需修正
							MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
						}
					#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
						{
							if(CheckUartData_0xEE(TXaddr+3,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
							{
								continue;
							}
						}
					#endif
						TXlen+=3+x;
						TXaddr+=3+x;
						break;
					}
					//没找到
					continue;
				}
			#endif//#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
				
				//其他日
				DataBuffDate=MRR(RXaddr+TIOffice,3);
				i=DataBuffDate;
			#if UartDateTI==1//日冻结的召读时标0=该日的24时数据,1=该日的0时数据
				i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
			#endif
				i=GetDateCongealNo(i);//得到日冻结相同时标的冻结号,返回=0xffffffff表示没找到
				i=PnDateCongealNo(i,Pn);//测量点日冻结号有效,返回=0xffffffff表示无效(没找到)
				if(i==0xffffffff)
				{//没找到
				#if (USER/100)==5//上海
				NOF246:
					//历史数据丢失应答方式：能用"否定确认"帧应答，统一使用无效数据0xEE应答，时标使用被请求的时标，数据内容填写完整
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//不是测量点类
						switch(Fn)
						{
							case 243://日冻结信号强度
								break;
							default:
								continue;
						}
					}
					MC(0xee,ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
					x=GetClass2DataLen(Fn,Pn,ADDR_DATABUFF);//取类2数据长度
					if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
					{
						if(ALLPn==0)//全部有效量点标志;0=否,1=是
						{
							TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
						}
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
						goto MulTx;//发送-还有后续帧
					}
					MWR(DataBuffDate,TXaddr,3);//3字节冻结时标
					MR(TXaddr+3,ADDR_DATABUFF,x);//再读x字节冻结数据
					//当前取得2类数据的费率数或谐波次数等修正
					i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
					if(i&0x10000)
					{//需修正
						MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
					}
					TXlen+=3+x;
					TXaddr+=3+x;
					break;
				#else//#if (USER/100)==5//上海
					continue;
				#endif
				}
				if(Fn==246)
				{//日冻结掉电记录数据(测量点映射)
					OfficeAddr=GetClass2DataF246OfficeAddr(i,Pn);//取类2数据F246存贮偏移地址,返回0表示没找到
					if(OfficeAddr==0)
					{//没找到
				#if (USER/100)==5//上海
					//历史数据丢失应答方式：能用"否定确认"帧应答，统一使用无效数据0xEE应答，时标使用被请求的时标，数据内容填写完整
						goto NOF246;
				#else
						continue;
				#endif
					}
				}
				if(MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+ADDR_DATECONGEALFLAG,1)==0xff)
				{//冻结标志是补冻结,只有电表的测量点类会补,其他非电表测量点作无数据避免将0xff转为0xee
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//不是测量点类
						continue;
					}
				}
			#if AFN0DF1MaxPn==0
				Class2_Group(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),Fn,Pn);//2类数据合成;出口:数据放ADDR_DATABUFF
			#endif
				x=GetClass2DataLen(Fn,Pn,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr);//取类2数据长度
				if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
				{
					if(ALLPn==0)//全部有效量点标志;0=否,1=是
					{
						TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
					goto MulTx;//发送-还有后续帧
				}
				MWR(DataBuffDate,TXaddr,3);//3字节冻结时标
		
			#if AFN0DF1MaxPn==0
				//以下Fn在集中抄表为节省FLASH将MaxPn设为0,但可由其他分项合成
				switch(Fn)
				{
				//2类数据.日冻结
					case 1://F1测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					case 2://F2测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					case 3://F3测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					case 4://F4测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						MR(TXaddr+3,ADDR_DATABUFF,x);//再读x字节冻结数据
						break;
				#if AFN0DF5MaxPn==0
					case 5://测量点正向有功电能量（总、费率1～M）
						UART_AFN0DF5F6F7F8_Computer(TXaddr+3,DataBuffDate,161,Pn,5);//2类日冻结数据电能量计算;入口：Fn为用于计算的示值Fn
						break;
					case 6://测量点正向无功电能量（总、费率1～M）
						UART_AFN0DF5F6F7F8_Computer(TXaddr+3,DataBuffDate,162,Pn,4);//2类日冻结数据电能量计算;入口：Fn为用于计算的示值Fn
						break;
					case 7://测量点反向有功电能量（总、费率1～M）
						UART_AFN0DF5F6F7F8_Computer(TXaddr+3,DataBuffDate,163,Pn,5);//2类日冻结数据电能量计算;入口：Fn为用于计算的示值Fn
						break;
					case 8://测量点反向无功电能量（总、费率1～M）
						UART_AFN0DF5F6F7F8_Computer(TXaddr+3,DataBuffDate,164,Pn,4);//2类日冻结数据电能量计算;入口：Fn为用于计算的示值Fn
						break;
				#endif
					default:
					#if (USER/100)==12//四川
					if(Fn==245)
					{
							MR(TXaddr+3,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr,x);//再读x字节冻结数据
							p8=(u8*)(TXaddr+3);
							if((p8[4]!=0xEE)&&(p8[4]!=0xFF))
							{
								p8[4]&=0x1f;
								p8[10]&=0x1f;
							}
					}
					else
					{
						MR(TXaddr+3,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr,x);//再读x字节冻结数据
					}
					#else
						MR(TXaddr+3,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr,x);//再读x字节冻结数据
					#endif
						break;
				}
			#else
				MR(TXaddr+3,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr,x);//再读x字节冻结数据
			#endif
				if(Terminal_Class2DataLib[Fn].PnType==pnType)
				{//是测量点类
					Data_0xFFto0xEE(TXaddr+3,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
				}
				//当前取得2类数据的费率数或谐波次数等修正
				i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
				if(i&0x10000)
				{//需修正
					MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
				}
			#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
				if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
				{
					if(CheckUartData_0xEE(TXaddr+3,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
					{
						continue;
					}
				}
			#endif
				TXlen+=3+x;
				TXaddr+=3+x;
				break;
			case 1://1=抄表日冻结
				DataBuffDate=MRR(RXaddr+TIOffice,3);
			#if (USER/100)==11	   //吉林 抄表日冻结时标提前一天
				port=MRR(ADDR_AFN04F10+((Pn-1)*LEN_AFN04F10_Pn)+4,1);
				port&=0x1f;//端口号
				if(port==31)
				{//通信端口31 载波抄表
					i=GetRMDateCongealNo(DataBuffDate);//得到抄表日冻结相同时标的冻结号,返回=0xffffffff表示没找到
				}
				else
				{
					i=DataBuffDate;
					i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
					i=GetRMDateCongealNo(i);//得到抄表日冻结相同时标的冻结号,返回=0xffffffff表示没找到
				}
			#else
				i=GetRMDateCongealNo(DataBuffDate);//得到抄表日冻结相同时标的冻结号,返回=0xffffffff表示没找到
			#endif
				i=PnRMDateCongealNo(i,Pn);//测量点抄表日冻结号有效,返回=0xffffffff表示无效(没找到)
				if(i==0xffffffff)
				{//没找到
				#if (USER/100)==5//上海
					//历史数据丢失应答方式：能用"否定确认"帧应答，统一使用无效数据0xEE应答，时标使用被请求的时标，数据内容填写完整
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//不是测量点类
						continue;
					}
					MC(0xee,ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
					x=GetClass2DataLen(Fn,Pn,ADDR_DATABUFF);//取类2数据长度
					if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
					{
						if(ALLPn==0)//全部有效量点标志;0=否,1=是
						{
							TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
						}
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
						goto MulTx;//发送-还有后续帧
					}
					MWR(DataBuffDate,TXaddr,3);//3字节冻结时标
					MR(TXaddr+3,ADDR_DATABUFF,x);//再读x字节冻结数据
					//当前取得2类数据的费率数或谐波次数等修正
					if(Terminal_Class2DataLib[Fn].PnType==pnType)
					{//是测量点类
						Data_0xFFto0xEE(TXaddr+3,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
					}
					i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
					if(i&0x10000)
					{//需修正
						MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
					}
					TXlen+=3+x;
					TXaddr+=3+x;
					break;
				#else//#if (USER/100)==5//上海
					continue;
				#endif
				}
			/*
				if(MRR(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+ADDR_RMDATECONGEALFLAG,1)==0xff)
				{//冻结标志是补冻结,只有电表的测量点类会补,其他非电表测量点作无数据避免将0xff转为0xee
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//不是测量点类
						continue;
					}
				}
			*/
			#if AFN0DF1MaxPn==0
				switch(Fn)
				{
				//2类数据.抄表日冻结
					case 9://F9测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					case 10://F10测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					case 11://F11测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					case 12://F12测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						Class2_Group(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),Fn,Pn);//2类数据合成;出口:数据放ADDR_DATABUFF
						//抄表日冻结按端口号分开时分冻结，当没冻结时填充数据为0xee
						x=MRR(ADDR_DATABUFF,1);
						if(x==0xee)
						{//无冻结
							continue;
						}
				}
			#else
				//抄表日冻结按端口号分开时分冻结，当没冻结时填充数据为0xee
				x=MRR(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+OfficeAddr,1);
				if(x==0xee)
				{//无冻结
					continue;
				}
			#endif
				x=GetClass2DataLen(Fn,Pn,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+OfficeAddr);//取类2数据长度
				if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
				{
					if(ALLPn==0)//全部有效量点标志;0=否,1=是
					{
						TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
					goto MulTx;//发送-还有后续帧
				}
			#if (USER/100)==11 //吉林
				port=MRR(ADDR_AFN04F10+((Pn-1)*LEN_AFN04F10_Pn)+4,1);
				port&=0x1f;//端口号
				if(port==31)
				{//通信端口31 载波抄表
					MR(TXaddr,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3);//先读3字节冻结时标
				}
				else
				{
					MWR(DataBuffDate,TXaddr,3);//3字节冻结时标
				}
			#else
				MR(TXaddr,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3);//先读3字节冻结时标
			#endif
			#if AFN0DF1MaxPn==0
				switch(Fn)
				{
				//2类数据.抄表日冻结
					case 9://F9测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					case 10://F10测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					case 11://F11测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					case 12://F12测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						MR(TXaddr+3,ADDR_DATABUFF,x);//再读x字节冻结数据
						break;
					default:
						MR(TXaddr+3,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+OfficeAddr,x);//再读x字节冻结数据
						break;
				}
			#else
				MR(TXaddr+3,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+OfficeAddr,x);//再读x字节冻结数据
			#endif
				//当前取得2类数据的费率数或谐波次数等修正
				if(Terminal_Class2DataLib[Fn].PnType==pnType)
				{//是测量点类
					Data_0xFFto0xEE(TXaddr+3,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
				}
				i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
				if(i&0x10000)
				{//需修正
					MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
				}
			#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
				if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
				{
					if(CheckUartData_0xEE(TXaddr+3,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
					{
						continue;
					}
				}
			#endif
				TXlen+=3+x;
				TXaddr+=3+x;
				break;
			case 2://2=月冻结
			#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				//上1月
				x=MRR(ADDR_TYMDHMS+4,2);
				x=YM_Sub1M(x);//年月减1月,返回减1后的年月
				if(MRR(RXaddr+TIOffice,2)==x)
				{//时标=(当前实时钟的上1月)
					if(MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+ADDR_MONTHCONGEALFLAG,1)==0xff)
					{//冻结标志是补冻结,只有电表的测量点类会补,其他非电表测量点作无数据避免将0xff转为0xee
						if(Terminal_Class2DataLib[Fn].PnType!=pnType)
						{//不是测量点类
							continue;
						}
					}
					i=GetMonthCongealNo(x);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
					i=PnMonthCongealNo(i,Pn);//测量点月冻结号有效,返回=0xffffffff表示无效(没找到)
					if(i!=0xffffffff)
					{//有上1月冻结
						if(DataBuffDate!=x)//当前UART数据缓冲调入冻结数据的数据日期
						{
							DataBuffDate=x;//当前UART数据缓冲调入冻结数据的数据日期
							MR(ADDR_AFN0D_UART_Buff_Start,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),LEN_MONTHCONGEAL);//读原上月冻结数据
							x=MRR(ADDR_TYMDHMS+3,3);//有效抄表时间
							Source_Full_CongealBuff(ADDR_AFN0D_UART_Buff_Start,2,0,x,0);//抄表原始数据填入冻结缓冲;冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线;端口B0=空,B1-B31分别表示端口1-31;YMD:有效抄表时间年月日(值0表示不比较);Full0xee!=0表示DL645-2007表无原始数据时填0xee
						}
						Class2_Group(ADDR_AFN0D_UART_Buff_Start,Fn,Pn);//2类数据合成;出口:数据放ADDR_DATABUFF
						
						x=GetClass2DataLen(Fn,Pn,ADDR_AFN0D_UART_Buff_Start+OfficeAddr);//取类2数据长度
						if((TXlen+4+14+8+2+(2+x))>LEN_UARTnTx)
						{
							if(ALLPn==0)//全部有效量点标志;0=否,1=是
							{
								TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
							}
							MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
							goto MulTx;//发送-还有后续帧
						}
						MR(TXaddr,ADDR_AFN0D_UART_Buff_Start,2);//先读2字节冻结时标
						switch(Fn)
						{
						//2类数据.月冻结
							case 17://F17测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
							case 18://F18测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
							case 19://F19测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
							case 20://F20测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
								MR(TXaddr+2,ADDR_DATABUFF,x);//再读x字节冻结数据
								break;
							default:
								MR(TXaddr+2,ADDR_AFN0D_UART_Buff_Start+OfficeAddr,x);//再读x字节冻结数据
								break;
						}
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//是测量点类
							Data_0xFFto0xEE(TXaddr+2,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						}
						//当前取得2类数据的费率数或谐波次数等修正
						i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
						if(i&0x10000)
						{//需修正
							MWR(i&0xff,TXaddr+2+((i>>8)&0xff),1);
						}
					#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
						{
							if(CheckUartData_0xEE(TXaddr+2,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
							{
								continue;
							}
						}
					#endif
						TXlen+=2+x;
						TXaddr+=2+x;
						break;
					}
					//没找到
					continue;
				}
			#endif//#if RMM_MonthCongeal_0xEE==0//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
				//其他月
				DataBuffDate=MRR(RXaddr+TIOffice,2);
				i=DataBuffDate;
			#if UartMonthTI==1//月冻结的召读时标0=该月未的24时数据,1=该月初1日的0时数据
				i=YM_Sub1M(i);//年月减1月,返回减1后的年月
			#endif
				i=GetMonthCongealNo(i);//得到月冻结相同时标的冻结号,返回=0xffffffff表示没找到
				i=PnMonthCongealNo(i,Pn);//测量点月冻结号有效,返回=0xffffffff表示无效(没找到)
				if(i==0xffffffff)
				{//没找到
				#if (USER/100)==5//上海
					//历史数据丢失应答方式：能用"否定确认"帧应答，统一使用无效数据0xEE应答，时标使用被请求的时标，数据内容填写完整
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//不是测量点类
						continue;
					}
					MC(0xee,ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
					x=GetClass2DataLen(Fn,Pn,ADDR_DATABUFF);//取类2数据长度
					if((TXlen+4+14+8+2+(2+x))>LEN_UARTnTx)
					{
						if(ALLPn==0)//全部有效量点标志;0=否,1=是
						{
							TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
						}
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
						goto MulTx;//发送-还有后续帧
					}
					MWR(DataBuffDate,TXaddr,2);//2字节冻结时标
					MR(TXaddr+2,ADDR_DATABUFF,x);//再读x字节冻结数据
					//当前取得2类数据的费率数或谐波次数等修正
					i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
					if(i&0x10000)
					{//需修正
						MWR(i&0xff,TXaddr+2+((i>>8)&0xff),1);
					}
					TXlen+=2+x;
					TXaddr+=2+x;
					break;
				#else//#if (USER/100)==5//上海
					continue;
				#endif
				}
				if(MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+ADDR_MONTHCONGEALFLAG,1)==0xff)
				{//冻结标志是补冻结,只有电表的测量点类会补,其他非电表测量点作无数据避免将0xff转为0xee
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//不是测量点类
						continue;
					}
				}
		#if AFN0DF1MaxPn==0
				Class2_Group(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),Fn,Pn);//2类数据合成;出口:数据放ADDR_DATABUFF
		#endif
				x=GetClass2DataLen(Fn,Pn,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+OfficeAddr);//取类2数据长度
				if((TXlen+4+14+8+2+(2+x))>LEN_UARTnTx)
				{
					if(ALLPn==0)//全部有效量点标志;0=否,1=是
					{
						TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
					goto MulTx;//发送-还有后续帧
				}
				MWR(DataBuffDate,TXaddr,2);//2字节冻结时标
		#if AFN0DF1MaxPn==0
				switch(Fn)
				{
				//2类数据.月冻结
					case 17://F17测量点正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
					case 18://F18测量点反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
					case 19://F19测量点正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
					case 20://F20测量点反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
						MR(TXaddr+2,ADDR_DATABUFF,x);//再读x字节冻结数据
						break;
			#if AFN0DF21MaxPn==0
					case 21://测量点正向有功电能量（总、费率1～M）
						UART_AFN0DF21F22F23F24_Computer(TXaddr+2,DataBuffDate,177,Pn,5);//2类月冻结数据电能量计算;入口：Fn为用于计算的示值Fn
						break;
					case 22://测量点正向无功电能量（总、费率1～M）
						UART_AFN0DF21F22F23F24_Computer(TXaddr+2,DataBuffDate,178,Pn,4);//2类月冻结数据电能量计算;入口：Fn为用于计算的示值Fn
						break;
					case 23://测量点反向有功电能量（总、费率1～M）
						UART_AFN0DF21F22F23F24_Computer(TXaddr+2,DataBuffDate,179,Pn,5);//2类月冻结数据电能量计算;入口：Fn为用于计算的示值Fn
						break;
					case 24://测量点反向无功电能量（总、费率1～M）
						UART_AFN0DF21F22F23F24_Computer(TXaddr+2,DataBuffDate,180,Pn,4);//2类月冻结数据电能量计算;入口：Fn为用于计算的示值Fn
						break;
			#endif
					default:
						MR(TXaddr+2,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+OfficeAddr,x);//再读x字节冻结数据
						break;
				}
		#else
				MR(TXaddr+2,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+OfficeAddr,x);//再读x字节冻结数据
		#endif
				if(Terminal_Class2DataLib[Fn].PnType==pnType)
				{//是测量点类
					Data_0xFFto0xEE(TXaddr+2,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
				}
				//当前取得2类数据的费率数或谐波次数等修正
				i=MRR(ADDR_Terminal_ClassDataLen,3);//1类数据长度寄存器或2类数据费率寄存器
				if(i&0x10000)
				{//需修正
					MWR(i&0xff,TXaddr+2+((i>>8)&0xff),1);
				}
			#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
				if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
				{
					if(CheckUartData_0xEE(TXaddr+2,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
					{
						continue;
					}
				}
			#endif
				TXlen+=2+x;
				TXaddr+=2+x;
				break;
			case 3://3=曲线(起始时间ts：分时日月年 5byte,数据冻结密度m 1byte,数据点数n 1byte)
				//数据冻结密度m
				p8d=(u8 *)(RXaddr+TIOffice);
			#if MaxACSampleCurveCongeal==0//最大交采1分钟冻结曲线点数(可设为0表示不用)
				//ACSamplePn=0;//有效交采电压曲线测量点号检查;返回0=无效,1=有效
				if(p8d[5]==127)
				{//默认交采电压曲线1分,其他15分
					p8d[5]=1;//15 分、30 分、45 分、0 分
				}
			#else
				ACSamplePn=ACSampleVoltageCurvePn(Fn,Pn);//有效交采电压曲线测量点号检查;返回0=无效,1=有效
				if(p8d[5]==127)
				{//默认交采电压曲线1分,其他15分
					if(ACSamplePn)//有效交采电压曲线测量点号0=无效,1=有效
					{
						p8d[5]=255;//1 分、2 分、3 分、......0 分
					}
					else
					{
						p8d[5]=1;//15 分、30 分、45 分、0 分
					}
				}
			#endif
				switch(p8d[5])
				{
					case 1://15 分、30 分、45 分、0 分
						m=15;
						i=p8d[0];
						if(i==0x00)
						{
							break;
						}
						if(i<=0x15)
						{
							p8d[0]=0x15;//分化整
							break;
						}
						if(i<=0x30)
						{
							p8d[0]=0x30;//分化整
							break;
						}
						if(i<=0x45)
						{
							p8d[0]=0x45;//分化整
							break;
						}
						p8d[0]=0x0;//分化整
						YMDHM=MRR(RXaddr+TIOffice,5);
						YMDHM=YMDHM_AddM(YMDHM,60);//年月日时分加m分,返回加m分后的年月日时分
						MWR(YMDHM,RXaddr+TIOffice,5);
						break;
					case 2://30 分、0 分；
						m=30;
						i=p8d[0];
						if(i==0x00)
						{
							break;
						}
						if(i<=0x30)
						{
							p8d[0]=0x30;//分化整
							break;
						}
						p8d[0]=0x0;//分化整
						YMDHM=MRR(RXaddr+TIOffice,5);
						YMDHM=YMDHM_AddM(YMDHM,60);//年月日时分加m分,返回加m分后的年月日时分
						MWR(YMDHM,RXaddr+TIOffice,5);
						break;
					case 3://0 分
						m=60;
						if(p8d[0]==0x00)
						{
							break;
						}
						p8d[0]=0x0;//分化整
						YMDHM=MRR(RXaddr+TIOffice,5);
						YMDHM=YMDHM_AddM(YMDHM,60);//年月日时分加m分,返回加m分后的年月日时分
						MWR(YMDHM,RXaddr+TIOffice,5);
						break;
			#if (USER/100)==5//上海
					case 254://5 分、10 分、15 分......、0 分
						switch(Fn)
						{
							case 235://终端AB线电压曲线（扩展）
							case 236://终端BC线电压曲线（扩展）
							case 237://终端AC线电压曲线（扩展）
								m=5;
								break;
							default:
								goto err;
						}
						break;
			#else
					case 254://5 分、10 分、15 分......、0 分
						m=5;
						i=p8d[0];
						if(i==0x00)
						{
							break;
						}
						if(i<=0x05)
						{
							p8d[0]=0x05;//分化整
							break;
						}
						if(i<=0x10)
						{
							p8d[0]=0x10;//分化整
							break;
						}
						if(i<=0x15)
						{
							p8d[0]=0x15;//分化整
							break;
						}
						if(i<=0x20)
						{
							p8d[0]=0x20;//分化整
							break;
						}
						if(i<=0x25)
						{
							p8d[0]=0x25;//分化整
							break;
						}
						if(i<=0x30)
						{
							p8d[0]=0x30;//分化整
							break;
						}
						if(i<=0x35)
						{
							p8d[0]=0x35;//分化整
							break;
						}
						if(i<=0x40)
						{
							p8d[0]=0x40;//分化整
							break;
						}
						if(i<=0x45)
						{
							p8d[0]=0x45;//分化整
							break;
						}
						if(i<=0x50)
						{
							p8d[0]=0x50;//分化整
							break;
						}
						if(i<=0x55)
						{
							p8d[0]=0x55;//分化整
							break;
						}
						p8d[0]=0x0;//分化整
						YMDHM=MRR(RXaddr+TIOffice,5);
						YMDHM=YMDHM_AddM(YMDHM,60);//年月日时分加m分,返回加m分后的年月日时分
						MWR(YMDHM,RXaddr+TIOffice,5);
						break;
					case 255://1 分、2 分、3 分、......0 分
						m=1;
						break;
			#endif
					default:
						goto err;
				}
				if(m<CurveCongealTime)
				{//要求密度<最小曲线冻结间隔时间
				#if (USER/100)==5//上海
					switch(Fn)
					{
						case 235://终端AB线电压曲线（扩展）
						case 236://终端BC线电压曲线（扩展）
						case 237://终端AC线电压曲线（扩展）
							break;
						default:
							goto err;
					}
				#else
					#if MaxACSampleCurveCongeal!=0//最大交采1分钟冻结曲线点数(可设为0表示不用)
					if(ACSamplePn==0)//有效交采电压曲线测量点号0=无效,1=有效
					{
						goto err;
					}
					#else
					goto err;
					#endif
				#endif
				}
				//Fn数据长度
				FnLen=GetClass2DataLen(Fn,Pn,0);//取类2数据长度
				//开始时间到当前时间最多点数
		#if((USER/100)!=10)//非河北用户
				MC(0,ADDR_DATABUFF,12);
				MR(ADDR_DATABUFF+1,RXaddr+TIOffice,5);
				MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,5);
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
				if(i>>31)
				{//负
					i=0;
				}
				else
				{
					i/=60;//秒化为分
					i/=m;
					i++;
				}
				if(p8d[6]>i)
				{
					p8d[6]=i;
				}
		#endif
				//曲线点过多时分帧
				PointNum=0;//曲线最多能填入缓冲区的点数
				while((TXlen+4+14+8+2+(7+(FnLen*PointNum)))<=LEN_UARTnTx)
				{
					PointNum++;
				}
				if(PointNum!=0)
				{
					PointNum--;
				}
				if(PointNum>p8d[6])
				{
					PointNum=p8d[6];
				}
				if(PointNum==0)
				{
					if(ALLPn==0)//全部有效量点标志;0=否,1=是
					{
						TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
					goto MulTx;//发送-还有后续帧
				}
/*				
				if((TXlen+4+14+8+2+(7+(FnLen*p8d[6])))>LEN_UARTnTx)
				{
					if(ALLPn==0)//全部有效量点标志;0=否,1=是
					{
						TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
					goto MulTx;//发送-还有后续帧
				}
*/
				MR(TXaddr,RXaddr+TIOffice,7);//7字节冻结时标
				MWR(PointNum,TXaddr+6,1);//当前帧点数
				TXlen+=7;
				TXaddr+=7;
				//查找数据
				YMDHM=MRR(RXaddr+TIOffice,5);
				//下帧时标和点数
				if(PointNum<p8d[6])
				{//还有下帧
					NextYMDHM=YMDHM_AddM(YMDHM,m*PointNum);//下帧的开始时标
					MWR(NextYMDHM,RXaddr+TIOffice,5);
					p8d[6]-=PointNum;//下帧的曲线还有点数
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
				}
				else
				{//没下帧,恢复曲线时标,密度,点数
					if(ALLPn!=0)//全部有效量点标志;0=否,1=是
					{
						MR(RXaddr,(u32)&UARTCtrl->TI,7);//寄存全部测量点时曲线的时标(年月日时分),密度,点数
					}
				}
				z=0;//没找到任何数据标志
				for(y=0;y<PointNum;y++)//需读点数
				{
				#if (USER/100)==5//上海
					z=1;//有找到数据标志
					switch(Fn)
					{
						case 235://p0 终端AB线电压曲线（扩展）
						case 236://p0 终端BC线电压曲线（扩展）
						case 237://p0 终端AC线电压曲线（扩展）
							i=GetCurveCongealNo_Ext(YMDHM);//得到扩展曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
							if(i==0xffffffff)
							{//没找到相同时标的冻结号
								MC(0xee,TXaddr,FnLen);
							}
							else
							{
								MR(TXaddr,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*i)+OfficeAddr,FnLen);//再读x字节冻结数据
								Data_0xFFto0xEE(TXaddr,FnLen);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
							}
							TXlen+=FnLen;
							TXaddr+=FnLen;
							YMDHM=YMDHM_AddM(YMDHM,m);//年月日时分加m分,返回加m分后的年月日时分
							continue;
						default:
							i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
							i=PnCurveCongealNo(i,Pn);//测量点冻结号有效,返回=0xffffffff表示无效(没找到)
							break;
					}
				#else//#if (USER/100)==5//上海
					#if MaxACSampleCurveCongeal==0//最大交采1分钟冻结曲线点数(可设为0表示不用)
					i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
					i=PnCurveCongealNo(i,Pn);//测量点冻结号有效,返回=0xffffffff表示无效(没找到)
					#else
					if(ACSamplePn==0)//有效交采电压曲线测量点号0=无效,1=有效
					{
						i=GetCurveCongealNo(YMDHM);//得到曲线冻结相同时标的冻结号,返回=0xffffffff表示没找到
						i=PnCurveCongealNo(i,Pn);//测量点冻结号有效,返回=0xffffffff表示无效(没找到)
					}
					else
					{
						i=GetACSampleCurveCongealAddr(YMDHM,Fn);//得到交采测量点曲线冻结存储地址,返回=0表示没找到
						if(i==0)
						{
							i=~i;
						}
					}
					#endif
				#endif
					if(i==0xffffffff)
					{//没找到相同时标的冻结号
				NextCurve:
						MC(0xee,TXaddr,FnLen);
						TXlen+=FnLen;
						TXaddr+=FnLen;
						YMDHM=YMDHM_AddM(YMDHM,m);//年月日时分加m分,返回加m分后的年月日时分
						continue;
					}
					
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//不是测量点类
						x=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR_CURVECONGEALFLAG,1);
						if(x==0xff)
						{//冻结标志是补冻结,只有电表的测量点类会补,其他非电表测量点作无数据
							goto NextCurve;
						}
					}
					else
					{//是测量点类
					#if MaxACSampleCurveCongeal==0//最大交采1分钟冻结曲线点数(可设为0表示不用)
						MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
						p16=(u16 *)(ADDR_PnVSSetList);
						for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
						{
							if(Pn==p16[MapPn])
							{
								break;
							}
						}
						if(MapPn>=MaxKeyUser)
						{//冻结的测量点列表中没找到相同的测量点号
							goto NextCurve;
						}
						MapPn+=1;
						if(Fn!=219)//非F219 测量点组合数据曲线
						{
							OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找到
							if(OfficeAddr==0)
							{//没找到
								goto NextCurve;
							}
						}
					#else
						if(ACSamplePn==0)//有效交采电压曲线测量点号0=无效,1=有效
						{
							MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
							p16=(u16 *)(ADDR_PnVSSetList);
							for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
							{
								if(Pn==p16[MapPn])
								{
									break;
								}
							}
							if(MapPn>=MaxKeyUser)
							{//冻结的测量点列表中没找到相同的测量点号
								goto NextCurve;
							}
							MapPn+=1;
							if(Fn!=219)//非F219 测量点组合数据曲线
							{
								OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//取曲线映射Pn的类2数据存贮偏移地址,返回0表示没找到
								if(OfficeAddr==0)
								{//没找到
									goto NextCurve;
								}
							}
						}
					#endif
					}
					
				#if Uart3761Check0xEE==0//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
					z=1;//有找到数据标志
				#endif	

					if(Fn!=219)//非F219 测量点组合数据曲线
					{
					#if MaxACSampleCurveCongeal==0//最大交采1分钟冻结曲线点数(可设为0表示不用)
						MR(TXaddr,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,FnLen);//再读x字节冻结数据
					#else
						if(ACSamplePn==0)//有效交采电压曲线测量点号0=无效,1=有效
						{
							MR(TXaddr,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,FnLen);//再读x字节冻结数据
						}
						else
						{
							MR(TXaddr,i,FnLen);//再读x字节冻结数据
						}
					#endif
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//是测量点类
							Data_0xFFto0xEE(TXaddr,FnLen);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
							x=Check_AFN04F11_Pn(Pn);//检查终端脉冲配置参数指定的测量点号;返回0无,1有
							if(x==0)
							{//不是脉冲口,由相应冻结示值计算得到
								switch(Fn)
								{
									case 97://正向有功总电能量曲线
										UART_AFN0DF97F98F99F100_Computer(TXaddr,YMDHM,m,101,Pn,i);//2类曲线数据电能量计算;入口：Fn为用于计算的示值曲线Fn
										break;
									case 98://正向无功总电能量曲线
										UART_AFN0DF97F98F99F100_Computer(TXaddr,YMDHM,m,102,Pn,i);//2类曲线数据电能量计算;入口：Fn为用于计算的示值曲线Fn
										break;
									case 99://反向有功总电能量曲线
										UART_AFN0DF97F98F99F100_Computer(TXaddr,YMDHM,m,103,Pn,i);//2类曲线数据电能量计算;入口：Fn为用于计算的示值曲线Fn
										break;
									case 100://反向无功总电能量曲线
										UART_AFN0DF97F98F99F100_Computer(TXaddr,YMDHM,m,104,Pn,i);//2类曲线数据电能量计算;入口：Fn为用于计算的示值曲线Fn
										break;
								}
							}
						}
					#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=终端信息点,pnType=测量点号,GnType=总加组号,CnType=电容器测量点号,DnType=直流模拟量号,RnType=控制轮次号,TnType=任务号
						{
							if(CheckUartData_0xEE(TXaddr,FnLen)==0)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
							{
								z=1;//有找到数据标志
							}
						}
						else
						{
							z=1;//有找到数据标志
						}
					#endif
					}
					else
					{//F219 测量点组合数据曲线
						//有功功率见附录A.9 kW 3
						OfficeAddr=GetClass2DataOfficeAddr(81,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr,3);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
						//无功功率见附录A.9 kvar 3
						OfficeAddr=GetClass2DataOfficeAddr(85,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+3,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+3,3);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
					#if MaxACSampleCurveCongeal==0//最大交采1分钟冻结曲线点数(可设为0表示不用)
						//A 相电压见附录A.7 V 2
						OfficeAddr=GetClass2DataOfficeAddr(89,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+6,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+6,2);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
						//B 相电压见附录A.7 V 2
						OfficeAddr=GetClass2DataOfficeAddr(90,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+8,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+8,2);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
						//C 相电压见附录A.7 V 2
						OfficeAddr=GetClass2DataOfficeAddr(91,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+10,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+10,2);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
					#else
						x=ACSampleVoltageCurvePn(89,Pn);//有效交采电压曲线测量点号检查;返回0=无效,1=有效
						if(x==0)
						{//不是交采点
							//A 相电压见附录A.7 V 2
							OfficeAddr=GetClass2DataOfficeAddr(89,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
							MR(TXaddr+6,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//再读x字节冻结数据
							x=Data_0xFFto0xEE(TXaddr+6,2);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
							if(x==0)
							{
								z=1;//有找到数据标志
							}
							//B 相电压见附录A.7 V 2
							OfficeAddr=GetClass2DataOfficeAddr(90,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
							MR(TXaddr+8,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//再读x字节冻结数据
							x=Data_0xFFto0xEE(TXaddr+8,2);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
							if(x==0)
							{
								z=1;//有找到数据标志
							}
							//C 相电压见附录A.7 V 2
							OfficeAddr=GetClass2DataOfficeAddr(91,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
							MR(TXaddr+10,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//再读x字节冻结数据
							x=Data_0xFFto0xEE(TXaddr+10,2);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
							if(x==0)
							{
								z=1;//有找到数据标志
							}
						}
						else
						{//交采电压曲线点
							//A 相电压见附录A.7 V 2
							//B 相电压见附录A.7 V 2
							//C 相电压见附录A.7 V 2
							x=GetACSampleCurveCongealAddr(YMDHM,89);//得到交采测量点曲线冻结存储地址,返回=0表示没找到
							if(x==0)
							{
								MC(0xee,TXaddr+6,6);
							}
							else
							{
								MR(TXaddr+6,x,6);//再读x字节冻结数据
								z=1;//有找到数据标志
							}
						}
					#endif//#if MaxACSampleCurveCongeal==0//最大交采1分钟冻结曲线点数(可设为0表示不用)
						//A 相电流见附录A.25 A 3
						OfficeAddr=GetClass2DataOfficeAddr(92,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+12,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+12,3);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
						//B 相电流见附录A.25 A 3
						OfficeAddr=GetClass2DataOfficeAddr(93,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+15,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+15,3);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
						//C 相电流见附录A.25 A 3
						OfficeAddr=GetClass2DataOfficeAddr(94,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+18,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+18,3);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
						//正向有功总电能量示值见附录A.14 kWh 4
						OfficeAddr=GetClass2DataOfficeAddr(101,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+21,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,4);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+21,4);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
						//一象限无功总电能量示值见附录A.14 kvar 4
						OfficeAddr=GetClass2DataOfficeAddr(145,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+25,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,4);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+25,4);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
						//四象限无功总电能量示值见附录A.14 kvar 4
						OfficeAddr=GetClass2DataOfficeAddr(146,MapPn);//取类2数据存贮偏移地址,返回0表示没找到
						MR(TXaddr+29,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,4);//再读x字节冻结数据
						x=Data_0xFFto0xEE(TXaddr+29,4);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
						if(x==0)
						{
							z=1;//有找到数据标志
						}
					}
					TXlen+=FnLen;
					TXaddr+=FnLen;
					YMDHM=YMDHM_AddM(YMDHM,m);//年月日时分加m分,返回加m分后的年月日时分
				}
				if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
				{//首帧
					if(z==0x0)
					{//没找到任何数据
						TXlen-=7+(FnLen*PointNum);
						TXaddr-=7+(FnLen*PointNum);
						continue;
					}
				}
				break;
		}
		FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
		f=1;//有无有效数据填入标志;0=无,1=有
/*
		switch(Fn)
		{//读数据后清相关数据的Fn

		}
*/
	}
	
	if(TXlen==0x0)
	{
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			goto err;
		}
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	if(UARTCtrl->TxFrameCount!=0)//寄存多帧数据时已发送帧计数
	{
		p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
		p8txbuff[13]|=0x20;//返回帧暂为多帧结束帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
	Uart_376_CON(PORTn);//主动定时任务CON标志位和功能码
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	//下启动帧序号TSEQ+1(当为定时主动上报时需要)
	if(UARTCtrl->LockFlags&0x30)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B5-B7保留
	{
		UARTCtrl->TxSEQ=p8rxbuff[13]&0x0f;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
		UARTCtrl->TxSEQ+=1;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
	}
	return;
MulTx://发送-还有后续帧
	if(TXlen<=4)
	{
//		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
//		{
			goto err;
//		}
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		p8txbuff[13]|=0x40;//返回帧为多帧起始帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
	UARTCtrl->DIlen=DIlen;//数据标识长度寄存器
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
	Uart_376_CON(PORTn);//主动定时任务CON标志位和功能码
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	//下响应帧序号RSEQ+1
	i=p8rxbuff[13];
	x=i;
	i&=0xf0;
	x++;
	x&=0x0f;
	i|=x;
	p8rxbuff[13]=i;
	UARTCtrl->Task=5;//5=等待多帧数据发送
#if (USER/100)==7//浙江
	if(UARTCtrl->LockFlags&0x30)//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B5-B7保留
	{//主动定时任务
		if(PORTn==GPRSPORT)
		{
			p16=(u16 *)((u32)&Terminal_Ram->UART_S_Timer);
			p16[0]=10;
//			p16[0]=(UARTCtrl->TxFrameCount)*5;
//			p16[0]=20;
		}
	}
#endif
	return;
}

#if (USER/100)==8//用户标识：通用版
void Uart_AFN0DF321(u32 PORTn)//电表对时后误差情况统计数量
{
	u32 i;
	u32 x;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8;
	u32 Pn;
	u32 RXDIaddr;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0d;//AFN
//用户区数据标识长度
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
	i=DItoFnPn(PORTn);//数据标识分解为FnPn
	Pn=i&0xffff;
	if(Pn!=0)
	{
		goto err;
	}
	x=MRR(RXDIaddr+4,3);
	x=Check_YMD(x);//年月日合法检查,返回0合法,1非法
	if(x==1)
	{
		goto err;
	}
	x=MRR(RXDIaddr+4,3);
	p8=(u8 *)ADDR_DATABUFF;
	x=YMD_Sub1D(x);//年月日减1日,返回减1后的年月日
	p8[0]=x;
	if(x!=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[0]-1),3))
	{
		goto err;
	}
	x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[0]-1)+3,4);
	if(x==0xffffffff)
	{
		x=0xeeeeeeee;
	}
	TXlen=12+7;
	//长度
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	p8txbuff[21]=x;
	p8txbuff[22]=(x>>8);
	p8txbuff[23]=(x>>16);
	p8txbuff[24]=(x>>24);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}
void Uart_AFN0DF322(u32 PORTn)//辽宁电表对时后误差情况统计明细	   辽宁台体测试要召读全部有效测量点数据
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 RXDIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
	u32 LEN_UARTnTx;
	u32 SourDI;//本次分解前的DI
	u32 TIlen;//时标长度
	u32 TIOffice;//时标偏移
	u32 ALLPn;//全部有效量点标志，0=否,1=是
	u32 DataBuffDate;//当前UART数据缓冲调入冻结数据的数据日期
	u32 TimeCom;
	u8 * p8;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	DataBuffDate=0;//当前UART数据缓冲调入冻结数据的数据日期
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0d;//AFN(为定时上报帧填写）
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			if(DIlen<(12+6))
			{
	err:
				//全部否认
				p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
				Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
				p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				return;
			}
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
			if(i!=0x0)
			{
				return;//舍弃该报文
			}
		}
	}
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识长度
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		if((p8rxbuff[13]&0x80)!=0x0)
		{//有Tp
			DIlen-=(8+6);
		}
		else
		{
			DIlen-=(8);
		}
		if(DIlen<4)
		{
			goto err;
		}
	}
	else
	{
		DIlen=UARTCtrl->DIlen;//数据标识长度寄存器
	}
//数据(启动站功能码11（请求2级数据）用于应用层请求数据的链路传输)
	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		UARTCtrl->DIOffset=0;//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
		CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
	}
	RXDIaddr+=UARTCtrl->DIOffset;
	RXaddr=RXDIaddr+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=TXDIaddr+4;
	TXlen=0;
	TIlen=0;//时标长度
	f=0;//有无有效数据填入标志;0=无,1=有
	MC(0,TXDIaddr,4);
	i=MRR((u32)(&UARTCtrl->SourDI),2);
	if(i!=0x00ff)
	{//本数据标识为全部有效测量点
		ALLPn=0;//全部有效量点标志，0=否,1=是
	}
	else
	{
		ALLPn=1;//全部有效量点标志，0=否,1=是
	}
	
	while(DIlen>=4)
	{
		SourDI=MRR((u32)(&UARTCtrl->DestDI),4);//本次分解前的DI
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
			if(ALLPn==0)//全部有效量点标志，0=否,1=是
			{
				//减总时标长度,每Fn的每Pn一个时标
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//计算Pn组合个数
				i=BitNum(i);//计算置位为1的个数(0-32)
				if(i==0)
				{
					i=1;//p0点
				}
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算Fn组合个数
				x=BitNum(x);//计算置位为1的个数(0-32)
				i*=x;//总个数
			}
			else
			{//本数据标识为全部有效测量点
				//减总时标长度,每Fn一个时标
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算Fn组合个数
				i=BitNum(i);//计算置位为1的个数(0-32)
			}
			if(TIlen==0)//时标长度
			{//前面还没有一个标识正确过,未知本标识时标长度
				goto err;
			}
			i*=TIlen;//时标长度
			RXaddr+=i;//接收地址+总时标长度
			if(DIlen>=(i+4))
			{
				DIlen-=i;
			}
			else
			{
				goto err;
			}
			RXDIaddr=RXaddr;
			UARTCtrl->DIOffset=RXDIaddr-(Get_ADDR_UARTnRx(PORTn)+14);//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
			CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
			i=MRR((u32)(&UARTCtrl->SourDI),2);
			if(i!=0x00ff)
			{//本数据标识为全部有效测量点
				ALLPn=0;//全部有效量点标志，0=否,1=是
			}
			else
			{
				ALLPn=1;//全部有效量点标志，0=否,1=是
			}
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
			}
			MC(0,TXDIaddr,4);
			continue;
		}
		Fn=(i>>16);
		Pn=i&0xffff;
		if(ALLPn!=0)//全部有效量点标志;0=否,1=是
		{
			if(Pn>MaxRS485AddCarrierPn)
			{//全部Pn完
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
			{
				continue;
			}
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
				MC(0,TXDIaddr,4);
			}
		}
		else
		{
			if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
			{
				continue;
			}
		}
		TIlen=3;//日冻结时标长度
		if(ALLPn==0)//全部有效量点标志;0=否,1=是
		{//本数据标识为全部有效测量点
			if(Pn==0)
			{//p0点Fn组合
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//计算置位为1的个数(0-32)
			}
			else
			{//非p0点Fn组合
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//计算同类型Pn组合本Pn前有Pn个数
				x=(Pn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//计算置位为1的个数(0-32)
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合个数
				x=BitNum(x);//计算置位为1的个数(0-32)
				i*=x;
				y=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				x&=y;
				x=BitNum(x);//计算置位为1的个数(0-32)
				i+=x;
			}
		}
		else
		{
			i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//计算置位为1的个数(0-32)
		}
		TIOffice=i*TIlen;//时标偏移
		if(DIlen<(TIOffice+TIlen+4))
		{
			DIlen=0;
			continue;
		}
		DataBuffDate=MRR(RXaddr+TIOffice,3);
		x=Check_YMD(DataBuffDate);//年月日合法检查,返回0合法,1非法
		if(x==1)
		{
			goto err;
		}
	#if UartDateTI==1//日冻结的召读时标0=该日的24时数据,1=该日的0时数据
		i=YMD_Sub1D(i);//年月日减1日,返回减1后的年月日
	#endif
		x=3;//取类2数据长度
		if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
		{
			if(ALLPn==0)//全部有效量点标志;0=否,1=是
			{
				TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
			}
			MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
			goto MulTx;//发送-还有后续帧
		}
		MWR(DataBuffDate,TXaddr,3);//3字节冻结时标
		p8=(u8 *)ADDR_TYMDHMS;
		TimeCom=DataComp(TXaddr,(u32)ADDR_TYMDHMS+3,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		if((TimeCom==0)||(TimeCom==1))
		{
			goto err;
		}
		p8=(u8 *)TXaddr;//3字节冻结时标
		TimeCom=DataComp(TXaddr,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[0]-1),3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		if(TimeCom==0)
		{
			MR(TXaddr+3,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[0]-1)+3+4+(Pn-1)*3,3);//再读x字节冻结数据
		}
		Data_0xFFto0xEE(TXaddr+3,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
	#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
			if(CheckUartData_0xEE(TXaddr+3,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
			{
				continue;
			}
	#endif
		TXlen+=3+x;
		TXaddr+=3+x;
		FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
		f=1;//有无有效数据填入标志;0=无,1=有
	}
	if(TXlen==0x0)
	{
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			goto err;
		}
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	if(UARTCtrl->TxFrameCount!=0)//寄存多帧数据时已发送帧计数
	{
		p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
		p8txbuff[13]|=0x20;//返回帧暂为多帧结束帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	return;
MulTx://发送-还有后续帧
	if(TXlen<=4)
	{
			goto err;
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		p8txbuff[13]|=0x40;//返回帧为多帧起始帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
	UARTCtrl->DIlen=DIlen;//数据标识长度寄存器
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	//下响应帧序号RSEQ+1
	i=p8rxbuff[13];
	x=i;
	i&=0xf0;
	x++;
	x&=0x0f;
	i|=x;
	p8rxbuff[13]=i;
	UARTCtrl->Task=5;//5=等待多帧数据发送
	return;
}
#endif

#if (USER/100)==6//用户标识：湖南
void Uart_AFN0DF40x(u32 PORTn)//AFN0DF400、F401、F402
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 RXDIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
	u32 LEN_UARTnTx;
	u32 SourDI;//本次分解前的DI
	u32 TIlen;//时标长度
	u32 TIOffice;//时标偏移
	u32 ALLPn;//全部有效量点标志，0=否,1=是
	u32 DataBuffDate;//当前UART数据缓冲调入冻结数据的数据日期
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 ADDR_Data;
	u32 YMD;
	
	DataBuffDate=0;//当前UART数据缓冲调入冻结数据的数据日期
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	p8txbuff[12]=0x0d;//AFN(为定时上报帧填写）
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			if(DIlen<(12+6))
			{
	err:
				//全部否认
				p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
				Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
				p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				return;
			}
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
			if(i!=0x0)
			{
				return;//舍弃该报文
			}
		}
	}
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识长度
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		if((p8rxbuff[13]&0x80)!=0x0)
		{//有Tp
			DIlen-=(8+6);
		}
		else
		{
			DIlen-=(8);
		}
		if(DIlen<4)
		{
			goto err;
		}
	}
	else
	{
		DIlen=UARTCtrl->DIlen;//数据标识长度寄存器
	}
//数据(启动站功能码11（请求2级数据）用于应用层请求数据的链路传输)
	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		UARTCtrl->DIOffset=0;//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
		CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
	}
	RXDIaddr+=UARTCtrl->DIOffset;
	RXaddr=RXDIaddr+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=TXDIaddr+4;
	TXlen=0;
	TIlen=0;//时标长度
	f=0;//有无有效数据填入标志;0=无,1=有
	MC(0,TXDIaddr,4);
	i=MRR((u32)(&UARTCtrl->SourDI),2);
	if(i!=0x00ff)
	{//本数据标识为全部有效测量点
		ALLPn=0;//全部有效量点标志，0=否,1=是
	}
	else
	{
		ALLPn=1;//全部有效量点标志，0=否,1=是
	}
	
	while(DIlen>=4)
	{
		SourDI=MRR((u32)(&UARTCtrl->DestDI),4);//本次分解前的DI
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
			if(ALLPn==0)//全部有效量点标志，0=否,1=是
			{
				//减总时标长度,每Fn的每Pn一个时标
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//计算Pn组合个数
				i=BitNum(i);//计算置位为1的个数(0-32)
				if(i==0)
				{
					i=1;//p0点
				}
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算Fn组合个数
				x=BitNum(x);//计算置位为1的个数(0-32)
				i*=x;//总个数
			}
			else
			{//本数据标识为全部有效测量点
				//减总时标长度,每Fn一个时标
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算Fn组合个数
				i=BitNum(i);//计算置位为1的个数(0-32)
			}
			if(TIlen==0)//时标长度
			{//前面还没有一个标识正确过,未知本标识时标长度
				goto err;
			}
			i*=TIlen;//时标长度
			RXaddr+=i;//接收地址+总时标长度
			if(DIlen>=(i+4))
			{
				DIlen-=i;
			}
			else
			{
				goto err;
			}
			RXDIaddr=RXaddr;
			UARTCtrl->DIOffset=RXDIaddr-(Get_ADDR_UARTnRx(PORTn)+14);//寄存多帧数据时当前正在处理接收帧中数据标识DI的地址偏移(半字对齐)
			CopyDI(PORTn,RXDIaddr);//COPY数据标识到控制缓冲准备分解;若数据标识为全部有效测量点则目的DI的2字节Pn部分清为从Hex的1开始
			i=MRR((u32)(&UARTCtrl->SourDI),2);
			if(i!=0x00ff)
			{//本数据标识为全部有效测量点
				ALLPn=0;//全部有效量点标志，0=否,1=是
			}
			else
			{
				ALLPn=1;//全部有效量点标志，0=否,1=是
			}
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
			}
			MC(0,TXDIaddr,4);
			continue;
		}
		Fn=(i>>16);
		Pn=i&0xffff;
		if(ALLPn!=0)//全部有效量点标志;0=否,1=是
		{
			if(Pn>MaxRS485AddCarrierPn)
			{//全部Pn完
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
			{
				continue;
			}
			if(f!=0x0)//有无有效数据填入标志;0=无,1=有
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
						goto MulTx;//发送-还有后续帧
					}
				}
				f=0;//有无有效数据填入标志;0=无,1=有
				MC(0,TXDIaddr,4);
			}
		}
		else
		{
			if(PnSet(Pn)==0)//有效测量点号检查;返回0=无效,1=有效
			{
				continue;
			}
		}
		TIlen=3;//日冻结时标长度
		if(ALLPn==0)//全部有效量点标志;0=否,1=是
		{//本数据标识为全部有效测量点
			if(Pn==0)
			{//p0点Fn组合
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//计算置位为1的个数(0-32)
			}
			else
			{//非p0点Fn组合
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//计算同类型Pn组合本Pn前有Pn个数
				x=(Pn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//计算置位为1的个数(0-32)
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合个数
				x=BitNum(x);//计算置位为1的个数(0-32)
				i*=x;
				y=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				x&=y;
				x=BitNum(x);//计算置位为1的个数(0-32)
				i+=x;
			}
		}
		else
		{
			i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//计算同类型Fn组合本Fn前有Fn个数
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//计算置位为1的个数(0-32)
		}
		TIOffice=i*TIlen;//时标偏移
		if(DIlen<(TIOffice+TIlen+4))
		{
			DIlen=0;
			continue;
		}
		DataBuffDate=MRR(RXaddr+TIOffice,3);
		x=Check_YMD(DataBuffDate);//年月日合法检查,返回0合法,1非法
		if(x==1)
		{
			goto err;
		}
		switch(Fn)
		{
			case 400://F400 测量点固定时间点单相电压
				x=8;//取类2数据长度
				break;
			case 401://F401 测量点固定时间点三相电压、电流
				x=72;//取类2数据长度
				break;
			case 402://F402 测量点固定时间点A相、零相电流
				x=24;//取类2数据长度
				break;
			default:
				return;
		}
		if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
		{
			if(ALLPn==0)//全部有效量点标志;0=否,1=是
			{
				TXlen+=4;//不是全部测量点时4字节组合标识;全部测量点时不组合已自动加上
			}
			MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//恢复本次分解前的DI
			goto MulTx;//发送-还有后续帧
		}
		MWR(DataBuffDate,TXaddr,3);//3字节冻结时标
		i=DataComp(TXaddr,(u32)ADDR_TYMDHMS+3,3);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
		if(i==1)
		{
			goto err;
		}
		YMD=MRR((u32)ADDR_TYMDHMS+3,3);
		if(DataBuffDate==YMD)
		{
			ADDR_Data=ADDR_AFN0DF400_Source;
		}
		else
		{
			ADDR_Data=ADDR_AFN0DF40x_Start;
			for(i=0;i<MaxDate_AFN0DF40x;i++)
			{
				YMD=MRR(ADDR_Data,3);
				if(YMD==DataBuffDate)
				{
					break;
				}
				ADDR_Data+=LEN_AFN0DF40x;
			}
			if(i>=3)
			{
				goto err;
			}
			ADDR_Data+=3;
		}
		switch(Fn)
		{
			case 400://F400 测量点固定时间点单相电压
				MR(TXaddr+3,ADDR_Data+((Pn-1)*x),x);//再读x字节冻结数据
				break;
			case 401://F401 测量点固定时间点三相电压、电流
				MR(TXaddr+3,ADDR_Data+(8*AFN0DF400MaxPn)+((Pn-1)*x),x);//再读x字节冻结数据
				break;
			case 402://F402 测量点固定时间点A相、零相电流
				MR(TXaddr+3,ADDR_Data+(80*AFN0DF400MaxPn)+((Pn-1)*x),x);//再读x字节冻结数据
				break;
			default:
				goto err;
		}
		Data_0xFFto0xEE(TXaddr+3,x);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF
//	#if Uart3761Check0xEE==1//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
//			if(CheckUartData_0xEE(TXaddr+3,x)==1)//检查测量点类的UART数据是否全0xee;入口:若Byte>5则Byte改为5字节的抄表时标长度;返回0=不是全0xEE,1=是全0xEE
//			{
//				continue;
//			}
//	#endif
		TXlen+=3+x;
		TXaddr+=3+x;
		FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
		f=1;//有无有效数据填入标志;0=无,1=有
	}
	if(TXlen==0x0)
	{
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			goto err;
		}
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	if(UARTCtrl->TxFrameCount!=0)//寄存多帧数据时已发送帧计数
	{
		p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
		p8txbuff[13]|=0x20;//返回帧暂为多帧结束帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	return;
MulTx://发送-还有后续帧
	if(TXlen<=4)
	{
			goto err;
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
	{
		p8txbuff[13]|=0x40;//返回帧为多帧起始帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
	UARTCtrl->DIlen=DIlen;//数据标识长度寄存器
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	//下响应帧序号RSEQ+1
	i=p8rxbuff[13];
	x=i;
	i&=0xf0;
	x++;
	x&=0x0f;
	i|=x;
	p8rxbuff[13]=i;
	UARTCtrl->Task=5;//5=等待多帧数据发送
	return;
}
#endif
