
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SET_6000.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_SETLIB.h"



u32 SET_OAD_6000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU)//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
{
	u32 OI;
	u32 Attribute;
	u32 Element;
	u32 Array;
	u32 Len;
	DL698_SETLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OI=(p8rx[0]<<8)+p8rx[1];
	Attribute=p8rx[2]&0x1f;
	Element=p8rx[3];
	p8rx+=4;
	
	Len=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
	Len>>=8;
	pLib=Get_DL698_SET_pLib((OI<<16)+(Attribute<<8));//得到EVENT_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
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
	if(Attribute)
	{//单个属性
		if(Element==0)
		{//全部元素
			MC(0,pLib->Addr,pLib->LENmax);
			Array=p8rx[1];//数组数
			p8rx+=2;
			if(LEN_RxAPDU<(4+2))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LEN_RxAPDU-=(4+2);
			while(Array--)
			{
				Attribute=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
				if(pLib->pDataType)
				{
					if((Attribute&0xff)==0)
					{
						UARTCtrl->DAR=7;//类型不匹配           （7）
						return Len;
					}
				}
				Attribute>>=8;
				if(LEN_RxAPDU<Attribute)
				{
					UARTCtrl->DAR=9;//数据块不可用         （9）
					return Len;
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
				if(Attribute>pLib->LENmax)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return Len;
				}
				if(p8rx[2]==DataType_unsigned)
				{
					Element=p8rx[3];//取配置序号
				}
				else
				{
					Element=(p8rx[3]<<8)+p8rx[4];//取配置序号
				}
				if(Element>=(pLib->LENmax/pLib->LENper))
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return Len;
				}
				MW((u32)p8rx,pLib->Addr+(Element*pLib->LENper),Attribute);
				p8rx+=Attribute;
			}
			UARTCtrl->DAR=0;//数据访问结果;成功
			return Len;
		}
		else
		{//单个元素
			UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
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




