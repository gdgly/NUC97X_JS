

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SET_EVENT.h"
#include "../DL698/DL698_SET.h"
#include "../DL698/DL698_GET.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"






const u8 DL698_Attribute_EventOAD[]=
{
	3,//类型列表数
//array
	DataType_array,
	1,
	DataType_OAD,
};


u32 SET_OAD_EVENT(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU)//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
{
	u32 OAD;
	u32 Len;
	DL698_EVENTLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
	p8rx+=4;
	
	Len=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
	Len>>=8;
	pLib=Get_DL698_EVENT_pLib(OAD);//得到EVENT_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
		return Len;
	}
	if((pLib->rw&2)==0)//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	{//不可写
		if(Comm_Ram->Factory!=0x55)//工厂状态
		{
			UARTCtrl->DAR=3;//数据访问结果;拒绝读写             （3）
			return Len;
		}
	}
	if(OAD&0xff00)
	{//单个属性
		if((OAD&0xff)==0)
		{//全部元素
			switch(pLib->IC)
			{
				case 7://IC=7 事件对象接口类
					switch((OAD>>8)&0xff)
					{
						case 1://1．逻辑名
						case 2://2．事件记录表
						case 4://4．当前记录数
						case 5://5．最大记录数
						case 7://7．当前值记录表
							UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
							return Len;
						case 9://9．有效标识
							if(p8rx[0]!=DataType_bool)
							{
								UARTCtrl->DAR=7;//类型不匹配           （7）
								return Len;
							}
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//数据块不可用         （9）
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD,2);
							UARTCtrl->DAR=0;//数据访问结果;成功
							return 2;
						case 8://8．上报标识
							if((p8rx[0]!=DataType_enum)&&(p8rx[0]!=DataType_bool))
							{
								UARTCtrl->DAR=7;//类型不匹配           （7）
								return Len;
							}
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//数据块不可用         （9）
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD+2,2);
							UARTCtrl->DAR=0;//数据访问结果;成功
							return 2;
						case 3://3．关联对象属性表
							Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)DL698_Attribute_EventOAD,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							if((Len&0xff)==0)
							{
								UARTCtrl->DAR=7;//类型不匹配           （7）
								return 0;
							}
							Len>>=8;
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//数据块不可用         （9）
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
								return Len;
							}
							if(Len>LENmax_EventOAD)
							{
								UARTCtrl->DAR=8;//越界                 （8）
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD+4,Len);
							UARTCtrl->DAR=0;//数据访问结果;成功
							return Len;
						case 6://6．配置参数
							Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							if(pLib->pDataType)
							{
								if((Len&0xff)==0)
								{
									UARTCtrl->DAR=7;//类型不匹配           （7）
									return 0;
								}
							}
							Len>>=8;
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//数据块不可用         （9）
								return 0;
							}
							if(pLib->Addr==0)
							{
								UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
								return Len;
							}
		/*
							if(Comm_Ram->Factory!=0x55)
							{//不是工厂模式
								if((pLib->rw&0x2)==0)
								{//不允许写
									UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
									return Len;
								}
							}
		*/
							if(Len>pLib->LENmax)
							{
								UARTCtrl->DAR=8;//越界                 （8）
								return Len;
							}
							MW((u32)p8rx,pLib->Addr,Len);
							UARTCtrl->DAR=0;//数据访问结果;成功
							return Len;
						default:
							UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
							return Len;
					}
					//return Len;
				case 24://IC=24 分项事件对象接口类
					switch((OAD>>8)&0xff)
					{
						case 1://1．逻辑名	（static）
						case 3://3．当前记录数	（dyn.）
						case 4://4．最大记录数	（static）
						case 6://6．事件记录表1	（dyn.）
						case 7://7．事件记录表2	（dyn.）
						case 8://8．事件记录表3	（dyn.）
						case 9://9．事件记录表4	（dyn.）
						case 10://10．当前值记录表	（dyn.）
							UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
							return Len;
						case 12://12．有效标识	（static）
							if(p8rx[0]!=DataType_bool)
							{
								UARTCtrl->DAR=7;//类型不匹配           （7）
								return Len;
							}
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//数据块不可用         （9）
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD,2);
							UARTCtrl->DAR=0;//数据访问结果;成功
							return 2;
						case 11://11．上报标识	（static）
							if(p8rx[0]!=DataType_enum)
							{
								UARTCtrl->DAR=7;//类型不匹配           （7）
								return Len;
							}
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//数据块不可用         （9）
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD+2,2);
							UARTCtrl->DAR=0;//数据访问结果;成功
							return 2;
						case 2://2．关联对象属性表	（static）
							Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)DL698_Attribute_EventOAD,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							if((Len&0xff)==0)
							{
								UARTCtrl->DAR=7;//类型不匹配           （7）
								return 0;
							}
							Len>>=8;
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//数据块不可用         （9）
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
								return Len;
							}
							if(Len>LENmax_EventOAD)
							{
								UARTCtrl->DAR=8;//越界                 （8）
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD+4,Len);
							UARTCtrl->DAR=0;//数据访问结果;成功
							return Len;
						case 5://5．配置参数	（static）
							Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							if(pLib->pDataType)
							{
								if((Len&0xff)==0)
								{
									UARTCtrl->DAR=7;//类型不匹配           （7）
									return 0;
								}
							}
							Len>>=8;
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//数据块不可用         （9）
								return 0;
							}
							if(pLib->Addr==0)
							{
								UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
								return Len;
							}
							if(Comm_Ram->Factory!=0x55)
							{//不是工厂模式
								if((pLib->rw&0x2)==0)
								{//不允许写
									UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
									return Len;
								}
							}
							if(Len>pLib->LENmax)
							{
								UARTCtrl->DAR=8;//越界                 （8）
								return Len;
							}
							MW((u32)p8rx,pLib->Addr,Len);
							UARTCtrl->DAR=0;//数据访问结果;成功
							return Len;
						default:
							UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
							return Len;
					}
					//return Len;
				default:
					UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
					return Len;
			}
		}
		else
		{//单个元素
			if(pLib->Addr==0)
			{
				UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
				return Len;
			}
			if(Comm_Ram->Factory!=0x55)
			{//不是工厂模式
				if((pLib->rw&0x2)==0)
				{//不允许写
					UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
					return Len;
				}
			}
			Len=SET_OAD_Element(PORTn,p8rx,(u8*)pLib->Addr,OAD&0xff,pLib->LENmax);//设置一个对象元素;返回:本元素的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
			return Len;
		}
	}
	else
	{//0表示整个对象属性，即对象的所有属性
		UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
		return Len;
	}
//	return 0;
}






































