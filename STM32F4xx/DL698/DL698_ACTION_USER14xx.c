
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_USER14xx.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"




#if (USER/100)==14

u32 ACTION_Add_USER14xx(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 LenRx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(p8rx[4+0]==DataType_array)
	{//添加一组
		Array=p8rx[4+1];
		if(Array&0x80)
		{
			Array&=0x7f;
			if(LEN_RxAPDU<=(4+2+Array))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx=2+Array;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2+Array;
			i=Array;
			Array=0;
			while(i--)
			{
				Array<<=8;
				Array|=p8rx[0];
				p8rx+=1;
			}
		}
		else
		{
			if(LEN_RxAPDU<=(4+2))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx=2;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2;
		}
	}
	else
	{//添加更新一个
		if(LEN_RxAPDU<=(4+2))
		{
			UARTCtrl->DAR=9;//数据块不可用         （9）
			return 0;
		}
		Array=1;
		LenRx=0;
		p8rx+=4;
		LEN_RxAPDU-=4;
	}
	while(Array--)
	{
		i=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		if(LEN_RxAPDU<i)
		{
			UARTCtrl->DAR=9;//数据块不可用         （9）
			return 0;
		}
		LenRx+=i;
		if(i>pLib->LENper)
		{
			UARTCtrl->DAR=8;//越界                 （8）
			return (LenRx<<16)+0;
		}
		
		if(p8rx[2]==DataType_unsigned)
		{
			n=p8rx[3];//取配置序号
		}
		else
		{
			n=(p8rx[3]<<8)+p8rx[4];//取配置序号
		}
		if(n>=(pLib->LENmax/pLib->LENper))
		{
			UARTCtrl->DAR=8;//越界（8）
			return (LenRx<<16)+0;
		}
		MW((u32)p8rx,pLib->Addr+(n*pLib->LENper),i);
		p8rx+=i;
	}
	return (LenRx<<16)+0;
}

u32 ACTION_Del_USER14xx(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 LenRx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(p8rx[4+0]==DataType_array)
	{//删除一组
		Array=p8rx[4+1];
		if(Array&0x80)
		{
			Array&=0x7f;
			if(LEN_RxAPDU<=(4+2+Array))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx=2+Array;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2+Array;
			i=Array;
			Array=0;
			while(i--)
			{
				Array<<=8;
				Array|=p8rx[0];
				p8rx+=1;
			}
		}
		else
		{
			if(LEN_RxAPDU<=(4+2))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx=2;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2;
		}
	}
	else
	{//删除一个
		if(LEN_RxAPDU<=(4+2))
		{
			UARTCtrl->DAR=9;//数据块不可用         （9）
			return 0;
		}
		Array=1;
		LenRx=0;
		p8rx+=4;
		LEN_RxAPDU-=4;
	}
	while(Array--)
	{
		if(LEN_RxAPDU<2)
		{
			UARTCtrl->DAR=9;//数据块不可用         （9）
			return 0;
		}
		if(p8rx[0]==DataType_unsigned)
		{
			n=p8rx[1];//取配置序号
			p8rx+=2;
			LenRx+=2;
		}
		else
		{
			n=(p8rx[1]<<8)+p8rx[2];//取配置序号
			p8rx+=3;
			LenRx+=3;
		}
		if(n<(pLib->LENmax/pLib->LENper))
		{
			MC(0,pLib->Addr+(n*pLib->LENper),1);
		}
	}
	return (LenRx<<16)+0;
}





extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6E11[];
//全局透明任务方案集
u32 ACTION_6E110200(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{

	u32 i;
	u32 x;
	u32 Method;
	u32 Array;
	u32 LenRx;
	u32 LenTx;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
//属性2（配置表）∷=array全局透明任务方案单元(对应任务方案参数有变更的应立即按新参数开始任务调度，没有变更的不应重新执行任务)
	pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_6E11;
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://Add（array 透明任务方案单元）;添加或更新一组透明任务方案单元，方案参数有变化的应清除其数据记录，重新开始任务调度
			i=ACTION_Add_USER14xx(pLib,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 128://Delete（array 透明任务方案号） 按任务方案号删除一组透明任务方案单元、具体任务配置及其数据记录
			i=ACTION_Del_USER14xx(pLib,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 129://Clear (参数) 参数∷=NULL
			//清空透明任务方案配置表
			MC(0,ADDR_6E11,LENmax_6E11);
			//清空具体任务配置
			MC(0,ADDR_6E13,LENmax_6E13);
			//清空及其数据记录
			
			//
			LenRx=1;
			p8rx+=4+1;
			LEN_RxAPDU-=4+1;
			return (LenRx<<16)+LenTx;
		case 130://方法130：Update（array 参数）
//参数∷=structure 
//{
//透明任务方案号  long-unsigned，
//执行优先级      unsigned
//}
//按新的策略更新任务状态。0删除该方案及其具体任务和数据，254为暂停任务，255为重启任务（任务方案优先级不变，执行清除当前周期任务记录，重新开始执行）或触发一次冻结
			
			
			return i;
		default:
			UARTCtrl->DAR=6;//对象不存在 6
			return 0;
	}
}

extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6E13[];
//全局透明具体任务集
u32 ACTION_6E13(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{

	u32 i;
	u32 x;
	u32 Method;
	u32 Array;
	u32 LenRx;
	u32 LenTx;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
//属性2（配置表）∷=array全局透明任务方案单元(对应任务方案参数有变更的应立即按新参数开始任务调度，没有变更的不应重新执行任务)
	pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_6E13;
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://Add（array 全局透明具体任务单元）
			i=ACTION_Add_USER14xx(pLib,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 128://Delete（array 参数）
//参数∷=structure
//{
//  透明任务方案号long-unsigned，
//  通信地址集合   MS
//}
//按透明任务方案号和地址删除一组透明具体任务
			
			return i;
		case 129://Delete（array透明任务方案号）
//按透明任务方案号删除一组透明具体任务
			return (LenRx<<16)+LenTx;
		case 130://Clear(参数) 参数∷=NULL 清空透明具体任务集
			
			
			return i;
		default:
			UARTCtrl->DAR=6;//对象不存在 6
			return 0;
	}
}







#endif//#if (USER/100)==14






















