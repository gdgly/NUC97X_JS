
//请求任务数据
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"


extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
//#define Class2Data_MaxFn	218//类2数据最大Fn
u32 Get_Class2Data_MaxFn(void);








void Uart_AFN0B(u32 PORTn)//请求任务数据
{
	u32 i;
	u32 x;
	u32 y;
	u32 t;
	u32 Fn;
	u32 Pn;
	u32 DIlen;
	u32 DIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 SourFn;
	u32 ALLPn;//全部有效量点标志，0=否,1=是
  u32 LEN_UARTnTx;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//得到UART控制区地址
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
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
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	TXaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXlen=0;
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	i=MRR((u32)(&UARTCtrl->SourDI),2);
	if(i!=0x00ff)
	{//本数据标识为全部有效测量点
		ALLPn=0;//全部有效量点标志，0=否,1=是
	}
	else
	{
		ALLPn=1;//全部有效量点标志，0=否,1=是
	}
	SourFn=0;
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
			switch(SourFn)
			{
				case 1:
					DIlen-=4;
					DIaddr+=4;
					break;
				case 2:
					if(ALLPn==0x0)
					{//本数据标识为全部有效测量点,发送时数据标识不进行组合
						DIlen-=4;
						DIaddr+=4;
					}
					else
					{
						DIlen-=9;
						DIaddr+=9;
					}
					break;
				default:
					goto err;
			}
			CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分�

			i=MRR((u32)(&UARTCtrl->SourDI),2);
			if(i!=0x00ff)
			{//本数据标识为全部有效测量点
				ALLPn=0;//全部有效量点标志，0=否,1=是
			}
			else
			{
				ALLPn=1;//全部有效量点标志，0=否,1=是
			}
			continue;
		}
		Fn=(i>>16);
		Pn=i&0xffff;
		SourFn=Fn;
		if(Pn==0x0)
		{
			goto err;
		}
		if(Pn>MaxTimerTask)
		{//全部Pn完
			if(ALLPn!=0x0)
			{//本数据标识为全部有效测量点,发送时数据标识不进行组合
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			goto err;
		}
		Pn--;
		switch(Fn)
		{
			case 1://请求定时上报1类数据任务
				i=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Pn)+8,1);//
				i*=4;
				if((TXlen+32+i)>LEN_UARTnTx)
				{
					goto err;
				}
				MR(TXaddr,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Pn)+9,i);
				TXaddr+=i;
				TXlen+=i;
				break;
			case 2://请求定时上报2类数据任务
				if(DIlen<9)
				{
					goto err;
				}
				x=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+8,1);//
				for(i=0;i<x;i++)
				{
					y=BitLoca(MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i)+2,1));
					y+=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i)+3,1)*8;
					if((y>=Get_Class2Data_MaxFn())||(y==0x0))
					{//Fn无效时不装入,因时标长度不可知
						if(ALLPn==0)//全部有效量点标志，0=否,1=是
						{
							DIlen-=5;
							DIaddr+=5;
						}
						continue;
					}
					if((TXlen+32+4)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i),4);//取DI
					TXaddr+=4;
					TXlen+=4;
					switch(Terminal_Class2DataLib[y].CongealType)//冻结类型:0=日冻结,1=抄表日冻结,2=月冻结,3=曲线
					{
						case 0://日冻结
							if((TXlen+32+3)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+2,TXaddr,3);
							TXaddr+=3;
							TXlen+=3;
							break;
						case 1://抄表日冻结
							if((TXlen+32+3)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+2,TXaddr,3);
							TXaddr+=3;
							TXlen+=3;
							break;
						case 2://月冻结(上报上1月)
							if((TXlen+32+2)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+3,TXaddr,2);
							TXaddr+=2;
							TXlen+=2;
							break;
						case 3://曲线(上报上1日整日点数)
/*
#define CurveCongealTime     15//曲线冻结间隔时间(密度)0=不冻结;15=15,30,45,0;30=30,0;60=0;5=5,10,15...0;1=1,2,3...0
曲线抽取数据倍率R：取值范围1～96
如被抽取的数据的冻结密度m=2，即每30分钟冻结一个值，那么当R=2时，表示按60分钟抽取，R=1时，表示仍按30分钟抽取
数据冻结密度m			
0	不冻结	无
1	15	15分、30分、45分、0分
2	30	30分、0分；
3	60	0分
254	5	5分、10分、15分......、0分
255	1	1分、2分、3分、......0分
数据点数n
*/
							if((TXlen+32+7)>LEN_UARTnTx)
							{
								goto err;
							}
							MR(TXaddr,DIaddr+4,5);
							//曲线冻结间隔时间*抽取数据倍率
							t=CurveCongealTime*MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+7,1);
							switch(t)
							{
								case 15:
									y=1;//要读取曲线的数据密度m
									break;
								case 30:
									y=2;//要读取曲线的数据密度m
									break;
								case 60:
									y=3;//要读取曲线的数据密度m
									break;
								case 5:
									y=254;//要读取曲线的数据密度m
									break;
								case 1:
									y=255;//要读取曲线的数据密度m
									break;
								default:
									t=CurveCongealTime;
									switch(t)
									{
										case 15:
											y=1;//要读取曲线的数据密度m
											break;
										case 30:
											y=2;//要读取曲线的数据密度m
											break;
										case 60:
											y=3;//要读取曲线的数据密度m
											break;
										case 5:
											y=254;//要读取曲线的数据密度m
											break;
										case 1:
											y=255;//要读取曲线的数据密度m
											break;
									}
									break;
							}
							MC(y,TXaddr+5,1);//数据冻结密度m
							t=(24*60)/t;//整1天点数
							MC(t,TXaddr+6,1);//数据冻结密度m
							TXaddr+=7;
							TXlen+=7;
							break;
					}
				}
				if(ALLPn==0)//全部有效量点标志，0=否,1=是
				{
					DIlen-=5;
					DIaddr+=5;
				}
				break;
			default:
				goto err;
		}
	}
	if(TXlen==0)
	{
		goto err;
	}
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		i=((u32)p8rxbuff)+6+DIlen-6;
		MR(TXaddr,i,6);
		TXaddr+=6;
		TXlen+=6;
	}
	TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	TXlen+=8;
	i=Get_LEN_UARTnRx(PORTn);
	if(TXlen>i)
	{
		goto err;
	}
	MR((u32)p8rxbuff,(u32)p8txbuff,TXlen+8);
	if(Fn==1)
	{
		UARTCtrl->LockFlags|=0x40;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动1类定时任务,B5=主动2类定时任务,B6=召读1类定时任务,B7
		Uart_AFN0C(PORTn);//类1数据
	}
	else
	{
		Uart_AFN0D(PORTn);//类2数据
	}
}







