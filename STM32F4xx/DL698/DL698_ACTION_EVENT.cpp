

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_EVENT.h"
#include "../DL698/EVENT_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB_EVENT.h"





u32 ACTION_EVENT(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 x;
	u32 OAD;
	u32 LenRx;
	u32 Method;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_EVENTLIB_TypeDef *pLib;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OAD=(p8rx[0]<<8)+p8rx[1];
	Method=p8rx[2];
	p8rx+=4;
	
	LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
	LenRx>>=8;
	pLib=Get_DL698_EVENT_pLib(OAD<<16);//得到EVENT_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
	switch(Method)
	{
		case 1://方法1：复位
			CLR_EventRecord(OAD);//清相应OI的事件记录
			return (LenRx<<16);
		case 3://3．触发一次记录
			UARTCtrl->DAR=6;//对象不存在           （6）
			return (LenRx<<16);
		case 4://4．添加一个事件关联对象属性
			if(pLib==0)
			{
				UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
				return (LenRx<<16);
			}
			if(pLib->Addr_EventOAD)
			{
				OAD=(p8rx[1]<<24)+(p8rx[2]<<16)+(p8rx[3]<<8)+p8rx[4];
				MR(ADDR_128KDATABUFF,pLib->Addr_EventOAD+4,LENmax_EventOAD);
				p8=(u8*)ADDR_128KDATABUFF;
				n=p8[1];//数组数
				if(n>NUMmax_EventOAD)
				{
					n=0;
				}
				//找原相同OAD
				p8+=3;
				for(i=0;i<n;i++)
				{
					x=(p8[0]<<24)+(p8[1]<<16)+(p8[2]<<8)+p8[3];
					if(x==OAD)
					{
						break;
					}
					p8+=5;
				}
				if(i>=n)
				{//没找到
					if(n>=NUMmax_EventOAD)
					{
						UARTCtrl->DAR=8;//越界                 （8）
						return (LenRx<<16);
					}
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=n+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(5*n),5);
					n++;
					MW(ADDR_128KDATABUFF,pLib->Addr_EventOAD+4,2+(5*n));
				}
			}
			return (LenRx<<16);
		case 5://5．删除一个事件关联对象属性
			if(pLib==0)
			{
				UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
				return (LenRx<<16);
			}
			if(pLib->Addr_EventOAD)
			{
				OAD=(p8rx[1]<<24)+(p8rx[2]<<16)+(p8rx[3]<<8)+p8rx[4];
				MR(ADDR_128KDATABUFF,pLib->Addr_EventOAD+4,LENmax_EventOAD);
				p8=(u8*)ADDR_128KDATABUFF;
				n=p8[1];//数组数
				if(n>NUMmax_EventOAD)
				{
					n=0;
				}
				//找原相同OAD
				p8+=3;
				for(i=0;i<n;i++)
				{
					x=(p8[0]<<24)+(p8[1]<<16)+(p8[2]<<8)+p8[3];
					if(x==OAD)
					{
						break;
					}
					p8+=5;
				}
				if(i>=n)
				{//没找到
					return (LenRx<<16);
				}
				//找到
				MR(ADDR_128KDATABUFF+2+(5*i),ADDR_128KDATABUFF+2+(5*i)+5,LENmax_EventOAD);
				p8=(u8*)ADDR_128KDATABUFF;
				n--;
				p8[0]=DataType_array;
				p8[1]=n;
				MW(ADDR_128KDATABUFF,pLib->Addr_EventOAD+4,2+(5*n));
			}
			return (LenRx<<16);
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return (LenRx<<16);
	}
}






























