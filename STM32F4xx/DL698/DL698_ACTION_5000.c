
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_5000.h"
#include "../DL698/DL698_SETLIB.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"






u32 ACTION_5000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 OAD;
	u32 array;
	u32 newarray;
	u32 LenRx;
	u32 LenTx;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OAD=(p8rx[0]<<24)|(p8rx[1]<<16);
	OAD|=0x0300;//关联对象属性表为属性3
	pLib=Get_DL698_SET_pLib(OAD);//得到SET_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
		return 0;
	}
	OAD=p8rx[2];//方法
#if LENmax_50xx>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,pLib->Addr,LENmax_50xx);
	p8=(u8*)ADDR_DATABUFF;
	if(p8[0]==DataType_array)
	{
		array=p8[1];//原有数组数
		if(array>127)
		{
			array=0;
			p8[1]=0;
		}
	}
	else
	{
		p8[0]=DataType_array;
		array=0;
		p8[1]=0;
	}
	LenTx=0;
	p8rx+=4;
	switch(OAD)
	{
//		case 1://1．复位
//		case 2://2．执行
//		case 3://3．触发一次冻结
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
		case 4://4．添加一个冻结对象属性
//冻结对象∷=structure
//{
//  冻结周期            long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度            long-unsigned
//}
			LenRx=13;
			OAD=(p8rx[6]<<24)|(p8rx[7]<<16)|(p8rx[8]<<8)|p8rx[9];
			//查原是否存在
			p8+=2;
			for(i=0;i<array;i++)
			{
				x=(p8[6]<<24)|(p8[7]<<16)|(p8[8]<<8)|p8[9];
				if(x==OAD)
				{
					break;
				}
				p8+=13;
			}
			x=((u32)p8-ADDR_DATABUFF);
			if((x>(LEN_DATABUFF-13))||((x+13)>pLib->LENmax))
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+LenTx;
			}
			MR((u32)p8,(u32)p8rx,13);
			if((i>=array)&&(array<127))
			{
				p8=(u8*)ADDR_DATABUFF;
				p8[1]++;//数组数+1
			}
			MW(ADDR_DATABUFF,pLib->Addr,pLib->LENmax);
			return (LenRx<<16)+LenTx;
		case 5://5．删除一个冻结对象属性
			//07 01 07 50 04 05 00  51 21 31 02 01 00 
			LenRx=5;
			OAD=(p8rx[1]<<24)|(p8rx[2]<<16)|(p8rx[3]<<8)|p8rx[4];
			//查原是否存在
			p8+=2;
			for(i=0;i<array;i++)
			{
				x=(p8[6]<<24)|(p8[7]<<16)|(p8[8]<<8)|p8[9];
				if(x==OAD)
				{
					break;
				}
				p8+=13;
			}
			if(i<array)
			{//原存在
				MR((u32)p8,(u32)p8+13,(array-i)*13);
				p8=(u8*)ADDR_DATABUFF;
				p8[1]--;//数组数-1
			}
			MW(ADDR_DATABUFF,pLib->Addr,pLib->LENmax);
			return (LenRx<<16)+LenTx;
		case 7://7．批量添加冻结对象属性
			//批量添加冻结对象属性（array 冻结对象）
			//07 01 0D 50 04 07 00 
			//01 03 
			//02 03 12 00 01 51 21 31 02 01 12 00 20 
			//02 03 12 00 01 51 21 32 02 01 12 00 20 
			//02 03 12 00 01 51 21 33 02 01 12 00 20 00 
			newarray=p8rx[1];//添加数
			LenRx=2+(13*newarray);
			p8rx+=2;
			while(newarray--)
			{
				OAD=(p8rx[6]<<24)|(p8rx[7]<<16)|(p8rx[8]<<8)|p8rx[9];
				//查原是否存在
				p8=(u8*)ADDR_DATABUFF+2;
				for(i=0;i<array;i++)
				{
					x=(p8[6]<<24)|(p8[7]<<16)|(p8[8]<<8)|p8[9];
					if(x==OAD)
					{
						break;
					}
					p8+=13;
				}
				x=((u32)p8-ADDR_DATABUFF);
				if((x>(LEN_DATABUFF-13))||((x+13)>pLib->LENmax))
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+LenTx;
				}
				MR((u32)p8,(u32)p8rx,13);
				if((i>=array)&&(array<127))
				{
					p8=(u8*)ADDR_DATABUFF;
					p8[1]++;//数组数+1
					array++;
				}
				p8rx+=13;
			}
			MW(ADDR_DATABUFF,pLib->Addr,pLib->LENmax);
			return (LenRx<<16)+LenTx;
		case 8://8．清除关联对象属性表
			//参数∷=NULL
			LenRx=0;
			p8[0]=DataType_array;
			p8[1]=0;
			MW((u32)p8,pLib->Addr,2);
			return (LenRx<<16)+LenTx;
	}
}



























