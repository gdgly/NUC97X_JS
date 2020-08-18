
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_6000.h"
#include "../DL698/SearchMeter_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../DL698/TASK.h"
#include "../Terminal698/Terminal_Init.h"


u32 ACTION_Add(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
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
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2[];//事件采集方案
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_601C[];
		if((pLib==DL698_SETLIB_6016_2)||(pLib==DL698_SETLIB_601C))
		{
			//上报方案6016,601C有data数据类型，不比较数据类型
			i=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		}
		else
		{
			i=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			if((i&0xff)==0)
			{
				UARTCtrl->DAR=7;//类型不匹配           （7）
				return 0;
			}
			i>>=8;
		}
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


u32 ACTION_Add_6000(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 LenRx;
	u8* p8;
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
		//检查波特率
		p8=Get_Element(p8rx+5,2,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		if(p8)
		{
			if((p8[1]>10)&&(p8[1]!=255))
			{
				UARTCtrl->DAR=255;
				return 0;
			}
		}

		//检查规约类型
	#if (USER%100)==0//台体测试
		p8=Get_Element(p8rx+5,3,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		if(p8)
		{
			if(p8[1]>4)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
		}
	#endif
		//检查接线方式
		p8=Get_Element(p8rx+5,8,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		if(p8)
		{
			if(p8[1]>3)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
		}
		//检查单元数据长度
		i=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
		if((i&0xff)==0)
		{
			UARTCtrl->DAR=7;//类型不匹配           （7）
			return 0;
		}
		i>>=8;
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
		//检查配置序号
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
			UARTCtrl->DAR=8;//越界                 （8）
			return (LenRx<<16)+0;
		}
		MW((u32)p8rx,pLib->Addr+(n*pLib->LENper),i);//nand
		//MW((u32)p8rx,ADDR_6000_SDRAM+(n*pLib->LENper),i);//sdram
		p8rx+=i;
	}
	return (LenRx<<16)+0;
}

u32 ADDR_6000_Check(void)//ADDR_6000档案电能表地址重复检查;返回:0=没重复,1=有重复,并自动删除重复序号
{
	u32 i;
	u32 n;
	u32 m;
	u32 f;
	u8* p8s;
	u8* p8d;
	u8* buff;
	ms_Type *ms;
	
	f=0;
	ms=Get_ms();
	p8s=(u8*)ms->ms_malloc(LENmax_6000);//动态分配我的内存堆
	ms->ms_free(p8s);//释放我的内存堆分配
	if(p8s)
	{
		buff=p8s;
		MR((u32)p8s,ADDR_6000,LENmax_6000);
		for(n=0;n<NUM_RMmax;n++)
		{
			if(p8s[0]==DataType_structure)
			{
				p8d=buff;
				for(m=0;m<NUM_RMmax;m++)
				{
					if(p8d[0]==DataType_structure)
					{
						for(i=0;i<6;i++)
						{
							if(p8s[10+i]!=p8d[10+i])
							{
								break;
							}
						}
						if(i>=6)
						{//重复
							if(n!=m)
							{//不是同一序号
								p8d[0]=0;//sdram
								MC(0,ADDR_6000+(m*LENper_6000),1);//nand
								f=1;
							}
						}
					}
					p8d+=LENper_6000;
				}
			}
			p8s+=LENper_6000;
		}
	}
	return f;
}



u32 ACTION_Del(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
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

//采集档案配置表
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_0[];
u32 ACTION_6000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
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
	
	pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_6000_2_0;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：Add（采集档案配置单元）添加一个采集档案配置单元，配置单元的信息中基本信息必须是完整的。
			if(p8rx[4]!=DataType_structure)
			{
				LenRx= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				UARTCtrl->DAR=255;//数据访问结果
				return (LenRx<<16)+LenTx;
			}
			i=ACTION_Add_6000((DL698_SETLIB_TypeDef *)DL698_SETLIB_6000_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
		#if (USER%100)!=0
			x=ADDR_6000_Check();//ADDR_6000档案电能表地址重复检查;返回:0=没重复,1=有重复,并自动删除重复序号
			if(x)
			{
				UARTCtrl->DAR=255;//数据访问结果
			}
		#endif
			return i;
		case 128://方法128：AddBatch（array 采集档案配置单元）批量添加采集档案配置单元。
			i=ACTION_Add_6000((DL698_SETLIB_TypeDef *)DL698_SETLIB_6000_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
		#if (USER%100)!=0
			x=ADDR_6000_Check();//ADDR_6000档案电能表地址重复检查;返回:0=没重复,1=有重复,并自动删除重复序号
			if(x)
			{
				UARTCtrl->DAR=255;//数据访问结果
			}
		#endif
			return i;
		case 129://方法129：Update（配置序号，基本信息）更新配置单元的基本信息对象
			Method=(p8rx[4+3]<<8)+p8rx[4+4];//取配置序号
			if(Method>=NUM_RMmax)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+LenTx;
			}
			if(LEN_RxAPDU<(4+5))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx=5;
			p8rx+=4+5;
			LEN_RxAPDU-=4+5;
			LenRx=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			if((LenRx&0xff)==0)
			{
				UARTCtrl->DAR=7;//类型不匹配           （7）
				return 0;
			}
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			//原长度
			i=Get_DL698DataLen((u8*)(ADDR_6000+(Method*LENper_6000)+5),(Attribute_TypeDef *)pLib->pDataType,2,LENper_6000);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if((i>LENper_6000)||(i<2))
			{
				i=2;
			}
			//后续数据长度1
			Array=Get_DL698DataLen((u8*)(ADDR_6000+(Method*LENper_6000)+5+i),(Attribute_TypeDef *)pLib->pDataType,11,LENper_6000);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			Array>>=8;
			if((Array>LENper_6000)||(Array<2))
			{
				Array=2;
			}
			//后续数据长度2
			x=Get_DL698DataLen((u8*)(ADDR_6000+(Method*LENper_6000)+5+i+Array),(Attribute_TypeDef *)pLib->pDataType,16,LENper_6000);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			x>>=8;
			if((x>LENper_6000)||(x<2))
			{
				x=2;
			}
			x+=Array;
			if((LenRx+x)>LENper_6000)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+LenTx;
			}
			//新数据
			MR(ADDR_128KDATABUFF,(u32)p8rx,LenRx);
			//原后续数据
			MR(ADDR_128KDATABUFF+LenRx,ADDR_6000+(Method*LENper_6000)+5+i,x);
			//写
			MW(ADDR_128KDATABUFF,ADDR_6000+(Method*LENper_6000)+5,LenRx+x);
			return (LenRx<<16)+LenTx;
		case 130://方法130：Update（配置序号，扩展信息，附属信息）更新配置单元的扩展信息以及附属信息，对象为NULL表示不更新。
			Method=(p8rx[4+3]<<8)+p8rx[4+4];//取配置序号
			if(Method>=NUM_RMmax)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+LenTx;
			}
			if(LEN_RxAPDU<(4+5))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx=5;
			p8rx+=4+5;
			LEN_RxAPDU-=4+5;
	//扩展信息
			if(p8rx[0]==0)
			{//对象为NULL表示不更新
				if(LEN_RxAPDU<1)
				{
					UARTCtrl->DAR=9;//数据块不可用         （9）
					return 0;
				}
				p8rx+=1;
				LenRx=1;
				LEN_RxAPDU-=1;
			}
			else
			{
				LenRx=Get_DL698DataLen(p8rx,0,11,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				if((LenRx&0xff)==0)
				{
					UARTCtrl->DAR=7;//类型不匹配           （7）
					return 0;
				}
				LenRx>>=8;
				if(LEN_RxAPDU<LenRx)
				{
					UARTCtrl->DAR=9;//数据块不可用         （9）
					return 0;
				}
				//原数据偏移
				Array=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+5,0,2,LENper_6000);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				Array>>=8;
				if((Array>LENper_6000)||(Array<2))
				{
					Array=2;
				}
				Array+=5;
				//原长度
				i=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+Array,0,11,LENper_6000);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				i>>=8;
				if((i>LENper_6000)||(i<2))
				{
					i=2;
				}
				//后继数据长度
				x=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+Array+i,0,16,LENper_6000);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				if((x>LENper_6000)||(x<2))
				{
					x=2;
				}
				if((Array+LenRx+x)>LENper_6000)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+LenTx;
				}
				//新数据
				MR(ADDR_128KDATABUFF,(u32)p8rx,LenRx);
				//原后续数据
				MR(ADDR_128KDATABUFF+LenRx,ADDR_6000+(Method*LENper_6000)+Array+i,x);
				//写
				MW(ADDR_128KDATABUFF,ADDR_6000+(Method*LENper_6000)+Array,LenRx+x);
				p8rx+=LenRx;
				LEN_RxAPDU-=LenRx;
			}
		//附属信息
			if(p8rx[0]==0)
			{//对象为NULL表示不更新
				if(LEN_RxAPDU<1)
				{
					UARTCtrl->DAR=9;//数据块不可用         （9）
					return 0;
				}
				p8rx+=1;
				LenRx=1;
				LEN_RxAPDU-=1;
			}
			else
			{
				LenRx=Get_DL698DataLen(p8rx,0,16,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				if((LenRx&0xff)==0)
				{
					UARTCtrl->DAR=7;//类型不匹配           （7）
					return 0;
				}
				LenRx>>=8;
				if(LEN_RxAPDU<LenRx)
				{
					UARTCtrl->DAR=9;//数据块不可用         （9）
					return 0;
				}
				//原数据偏移
				Array=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+5,0,2,LENper_6000);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				Array>>=8;
				if((Array>LENper_6000)||(Array<2))
				{
					Array=2;
				}
				Array+=5;
				i=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+Array,0,11,LENper_6000);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				i>>=8;
				if((i>LENper_6000)||(i<2))
				{
					i=2;
				}
				Array+=i;
				if((Array+LenRx)>LENper_6000)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+LenTx;
				}
				//写
				MW((u32)p8rx,ADDR_6000+(Method*LENper_6000)+Array,LenRx);
				p8rx+=LenRx;
				LEN_RxAPDU-=LenRx;
			}
			return (LenRx<<16)+LenTx;
		case 131://方法131：Delete（配置序号）删除配置单元，通过配置序号删除
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6000_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 132://方法132：Delete（基本信息）删除配置单元，通过基本信息对象删除
			p8rx+=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<(4+LenRx))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			x=0;//总计数
			while(1)
			{
				if(x>=NUM_RMmax)
				{
					break;
				}
				MR(ADDR_128KDATABUFF,ADDR_6000+(x*LENper_6000),LEN_128KDATABUFF);
				p8=(u8*)ADDR_128KDATABUFF+5;
				for(i=0;i<(LEN_128KDATABUFF/LENper_6000);i++)
				{
					if(x>=NUM_RMmax)
					{
						break;
					}
				//比较
					for(Array=0;Array<LenRx;Array++)
					{
						if(p8rx[Array]!=p8[Array])
						{
							break;
						}
					}
					if(Array<LenRx)
					{
						p8+=LENper_6000;
						x++;
						continue;
					}
				//清配置
					MC(0,ADDR_6000+(x*LENper_6000),1);//删除时structure的数据单元清为0(NULL)
					p8+=LENper_6000;
					x++;
				}
			}
			return (LenRx<<16)+LenTx;
		case 133://方法133：Delete（通信地址， 端口号）删除配置单元，通过通信地址及端口删除
			//structure
			//{
			//	TSA
			//	OAD
			//}
			p8rx+=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<(4+LenRx))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			x=0;//总计数
			while(1)
			{
				if(x>=NUM_RMmax)
				{
					break;
				}
				MR(ADDR_128KDATABUFF,ADDR_6000+(x*LENper_6000),LEN_128KDATABUFF);
				p8=(u8*)ADDR_128KDATABUFF;
				for(i=0;i<(LEN_128KDATABUFF/LENper_6000);i++)
				{
					if(x>=NUM_RMmax)
					{
						break;
					}
				//比较TSA
					Method=2;//接收数据偏移
					Array=7;//FLASH数据偏移
					if(p8rx[Method]==DataType_NULL)
					{
						Method++;//接收数据偏移
						Array+=p8[Array]+1;//FLASH数据偏移
						Array+=4;
					}
					else
					{
						if(p8rx[Method]==DataType_TSA)
						{
							Method++;//接收数据偏移
							Array++;//FLASH数据偏移
							LenTx=p8rx[Method]+1;//比较字节数
							while(LenTx)
							{
								if(p8rx[Method]!=p8[Array])
								{
									break;
								}
								LenTx--;
								Method++;//接收数据偏移
								Array++;//FLASH数据偏移
							}
							if(LenTx)
							{//不相同
								p8+=LENper_6000;
								x++;
								continue;
							}
							Array+=4;//FLASH数据偏移
						}
						else
						{
							UARTCtrl->DAR=9;//数据块不可用         （9）
							return 0;
						}
					}
				//比较OAD
					if(p8rx[Method]!=DataType_NULL)
					{
						if(p8rx[Method]!=DataType_OAD)
						{
							UARTCtrl->DAR=9;//数据块不可用         （9）
							return 0;
						}
						LenTx=4;
						while(LenTx)
						{
							LenTx--;
							Method++;//接收数据偏移
							Array++;//FLASH数据偏移
							if(p8rx[Method]!=p8[Array])
							{//不相同
								break;
							}
						}
						if(LenTx)
						{//不相同
							p8+=LENper_6000;
							x++;
							continue;
						}
					}
					//清配置
					MC(0,ADDR_6000+(x*LENper_6000),1);//删除时structure的数据单元清为0(NULL)
					p8+=LENper_6000;
					x++;
				}
			}
			LenTx=0;
			return (LenRx<<16)+LenTx;
		case 134://方法134：Clear()清空采集档案配置表
			if(p8rx[4]!=0)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			MC(0,ADDR_6000,LENper_6000*NUM_RMmax);
			LenRx=1;
			p8rx+=4+1;
			LEN_RxAPDU-=4+1;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//搜表
u32 ACTION_6002(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	LenTx=0;
	LenRx=0;
	p8rx+=4;
	switch(i)
	{
		case 127://方法127：实时启动搜表（搜表时长）
//			MW((u32)p8rx,ADDR_6002_127,3);
//			LenTx=MRR(ADDR_6002_8+9,1);
//			if(LenTx==2)
//			{//每次搜表前清空
//				DEL_SearchMeter_Record(3);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
//				DEL_SearchMeter_Record(4);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
//			}
			LenRx=(p8rx[1]<<8)|p8rx[2];
			if(LenRx==0)
			{//自动搜表持续时间分，0表示不限时间直至搜表结束
				Terminal_Ram->AutoSearch_M_Timer=65535;//2 自动搜表允许时间分定时器
			}
			else
			{
				Terminal_Ram->AutoSearch_M_Timer=LenRx;//2 自动搜表允许时间分定时器
			}
			
		#if EnRS485AutoSearchMeter==1//0=否,1=是;允许RS485口自动搜表
			MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*0),7);//RS485-1初始搜表地址
			MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*1),7);//RS485-2初始搜表地址
			
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			UARTCtrl->AutoSearchTime=LenRx;//自动搜表持续时间分，0表示不限时间直至搜表结束
			UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+3;//方案号3=实时启动搜表
			UARTCtrl->AutoSearchTask=1;//自动搜表任务:0=空,1=等待,2...过程
		
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
			UARTCtrl->AutoSearchTime=LenRx;//自动搜表持续时间分，0表示不限时间直至搜表结束
			UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+3;//方案号3=实时启动搜表
			UARTCtrl->AutoSearchTask=1;//自动搜表任务:0=空,1=等待,2...过程
		#endif
			
		#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->AutoSearchTime=LenRx;//自动搜表持续时间分，0表示不限时间直至搜表结束
			UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+3;//方案号3=实时启动搜表
			UARTCtrl->AutoSearchTask=1;//自动搜表任务:0=空,1=等待,2...过程
		#endif
			MR(ADDR_DATABUFF,ADDR_6002_8,LENmax_6002_8);//搜表参数
			p8=(u8*)ADDR_DATABUFF;
			if(p8[9]==2)
			{//每次搜表前清空
				DEL_SearchMeter_Record(3);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
				DEL_SearchMeter_Record(4);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
			}
		
			LenTx=0;
			LenRx=3;
			return (LenRx<<16)+LenTx;
		case 128://方法128：清空搜表结果()
			DEL_SearchMeter_Record(3);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
			return (LenRx<<16)+LenTx;
		case 129://方法129：清空跨台区搜表结果()
			DEL_SearchMeter_Record(4);//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}



//任务配置表
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_0[];
u32 ACTION_6012(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 filename;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 3://清空记录表
			//删除记录文件
			ms=Get_ms();
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
			{
				if(filename==recordFILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
				}
				if(file->BLOCK_ENTRY)
				{
					//文件分类:0=普通采集方案数据记录
					if((file->FILEID&0xff)==0)
					{
						//删除
						i=filename;
						if(i>=recordFILENAMEmax_FLASH)
						{
							i-=recordFILENAMEmax_FLASH;
							i+=FILENAMEmax_FLASH;
						}
						ms->msfile_Delete(i);
					}
				}
				file++;
			}
			LenRx=1;
			return (LenRx<<16)+LenTx;
		case 127://方法127：Add（array 任务配置单元）添加或更新一组任务配置单元。
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_6012_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 128://方法128：Delete（array 任务ID）删除一组配置单元。
		#if (USER/100)==0
		//一致性测试软件bug
			if(p8rx[4+3]==1)
			{
				p8rx[4+3]=10;
			}
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6012_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		#else
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6012_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		#endif
		case 129://方法129：Clear()清空任务配置表。
			MC(0,ADDR_6012_2,LENper_6012_2*NUMmax_6012_2);
//		#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//			//电科院并发抄表测试需要，清空任务需立即执行
//			FLASHtoSDRAM();//参数FLASHtoSDRAM
//			TASK(RS485_4PORT,1);//任务;入口:RM=0上报任务,RM=1采集任务
//		#endif
			LenRx=1;
			return (LenRx<<16)+LenTx;
		case 130://方法130：Update(任务ID，状态)更新任务状态。
			//::==DataType_structure
			//{
				//unsigned
				//emun
			//}
		#if (USER/100)==0
		//一致性测试软件bug
			Method=p8rx[4+3];//取任务ID
			if(Method==10)
			{
				MWR(10,ADDR_6012_2+(1*LENper_6012_2)+3,1);//任务号1变为10
				MWR(2,ADDR_6012_2+(1*LENper_6012_2)+35,1);//状态1变为2
			}
			else
			{
				if(Method<NUMmax_6012_2)
				{
					i=p8rx[4+5];
					MWR(i,ADDR_6012_2+(Method*LENper_6012_2)+35,1);
				}
			}
		#else
			Method=p8rx[4+3];//取任务ID
			if(Method<NUMmax_6012_2)
			{
				i=p8rx[4+5];
				MWR(i,ADDR_6012_2+(Method*LENper_6012_2)+35,1);
			}
		#endif
			LenRx=4;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//普通采集方案集
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_0[];
u32 ACTION_6014(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 Method;
	u32 array;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8;
	u32 Len0;
	u32 Len1;
	u32 Len2;
	
	x=x;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：Add（array 普通采集方案）添加或更新一组普通采集方案。
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_6014_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 128://方法128：Delete（array 方案编号）删除一组普通采集方案。
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6014_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 129://方法129：Clear()清空普通采集方案集。
			MC(0,ADDR_6014,LENper_6014*NUMmax_6014);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		case 130://方法130：Set_CSD（方案编号，array CSD）重置方案的记录列选择。
//structure
//{
//unsigned 方案编号
//array CSD
//}
			p8rx+=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			if((i&0xff)==0)
			{
				UARTCtrl->DAR=7;//类型不匹配           （7）
				return 0;
			}
			i>>=8;
			if(LEN_RxAPDU<(i+4))
			{
				UARTCtrl->DAR=9;//数据块不可用（9）
				return 0;
			}
			LenRx=i;
			array=1;
			while(array--)
			{
				Method=p8rx[3];//取方案编号
				if(Method>=NUMmax_6014)
				{
					UARTCtrl->DAR=8;//越界（8）
					break;
				}
				else
				{
				#if LENper_6014>LEN_DATABUFF
					#error
				#endif
					MR(ADDR_DATABUFF,ADDR_6014+(Method*LENper_6014),LENper_6014);
					p8=Get_Element((u8 *)ADDR_DATABUFF,4,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
					if(p8==0)
					{
						UARTCtrl->DAR=255;
						break;
					}
					//原CSD前数据长度
					Len0=(u32)p8-ADDR_DATABUFF;
					//原CSD数据长度
					Len1=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					//原后续数据长度
					Len2=Get_DL698DataLen_S(p8+Len1,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					Len2+=2;
					//新CSD数据长度
					i=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
					if((Len0+i+Len2)<=LENper_6014)
					{
						MW((u32)p8rx+4,ADDR_6014+(Method*LENper_6014)+Len0,i);
						//写回原后续数据
						MW(ADDR_DATABUFF+Len0+Len1,ADDR_6014+(Method*LENper_6014)+Len0+i,Len2);
					}
					else
					{
						UARTCtrl->DAR=8;//越界（8）
						break;
					}
				}
				i=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				p8rx+=i;
			}
			return (LenRx<<16)+LenTx;
	#if (USER/100)==9//河南扩展
		case 200:
//方法200：Set_MS（参数）
//参数∷=structure
//{
//  方案编号      unsigned，
//	电能表集合    MS
//}
//重置方案的电能表集合。
			p8rx+=4;
			Method=p8rx[3];//取方案号
			if(Method>=NUMmax_6014)
			{
				UARTCtrl->DAR=8;//越界（8）
				return 0;
			}
		#if LENper_6014>LEN_DATABUFF
			#error
		#endif
			MR(ADDR_DATABUFF,ADDR_6014+(Method*LENper_6014),LENper_6014);
			//计算电能表集合MS后的存储时标选择 enum
			p8=Get_Element((u8 *)ADDR_DATABUFF,6,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8==0)
			{
				UARTCtrl->DAR=9;//数据块不可用 （9）
				return 0;
			}
			x=p8[0]|(p8[1]<<8);//保护原数据
			//计算电能表集合MS开始地址
			p8=Get_Element((u8 *)ADDR_DATABUFF,5,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8==0)
			{
				UARTCtrl->DAR=9;//数据块不可用（9）
				return 0;
			}
			LenRx=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			if(((u32)p8+LenRx+2)>(ADDR_DATABUFF+LEN_DATABUFF))
			{
				UARTCtrl->DAR=9;//数据块不可用（9）
				return 0;
			}
			MW((u32)p8rx+4,(u32)p8,LenRx);
			p8+=LenRx;
			p8[0]=x;//恢复原数据
			p8[1]=x>>8;
			p8+=2;
			if((u32)p8>=ADDR_DATABUFF)
			{
				i=(u32)p8-ADDR_DATABUFF;
				if(i>LENper_6014)
				{
					UARTCtrl->DAR=9;//数据块不可用（9）
					return 0;
				}
				MW(ADDR_DATABUFF,ADDR_6014+(Method*LENper_6014),i);
			}
			LenRx+=4;
			return (LenRx<<16)+LenTx;
	#endif
		default:
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
	}
}

//事件采集方案集
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2[];
u32 ACTION_6016(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：Add（array 事件采集方案）添加或更新一组事件采集方案。
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_6016_2,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 128://方法128：Delete（array 方案编号）删除一组事件采集方案。
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6016_2,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 129://方法129：Clear()清空事件采集方案集。
			MC(0,ADDR_6016,LENper_6016*NUMmax_6016);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		case 130://方法130：UpdateReportFlag(方案编号，上报标识)上报标识∷=bool
//structure
//{
//unsigned 方案编号
//bool
//}
			if(LEN_RxAPDU<(4+6))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx=6;
			Method=p8rx[4+3];//取方案编号
			if(Method>=NUMmax_6016)
			{
				UARTCtrl->DAR=8;//越界                 （8）
			}
			else
			{
				pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_6016_2;
				i=Get_DL698DataLen((u8*)ADDR_6016+(Method*LENper_6016),(Attribute_TypeDef *)pLib->pDataType,0,LENper_6016);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				i>>=8;
				if(i>5)
				{
					i-=5;
					MW((u32)p8rx+4+4,ADDR_6016+(Method*LENper_6016)+i,2);
				}
			}
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//透明方案集
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6018[];
u32 ACTION_6018(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：Add（透明方案）添加更新一个透明方案或添加一组方案内容。
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_6018,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 128://方法128：AddMeterFrame（方案编号，通信地址，方案控制标志，方案报文集）添加一组报文。
			UARTCtrl->DAR=6;//对象不存在           （6）
			return LenTx+(LenRx<<16);
		case 129://方法129：Delete（方案编号，array 通信地址）删除一个方案的一组方案内容。
			UARTCtrl->DAR=6;//对象不存在           （6）
			return (LenRx<<16)+LenTx;
		case 130://方法130：Delete（array 方案编号）删除一组透明方案。
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6018,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 131://方法131：Clear()清空透明方案集。
			MC(0,ADDR_6018,LENper_6018*NUMmax_6018);
			//同时清空结果集
			//MC(0,ADDR_601A,LENper_601A*NUMmax_601A);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//上报方案集
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_601C[];
u32 ACTION_601C(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：Add（array 上报方案）添加或更新一组上报方案。
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_601C,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法增加一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 128://方法128：Delete（array 方案编号）删除一组上报方案。
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_601C,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法删除一个或一组设置数据;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return i;
		case 129://方法129：Clear()清空上报方案集。
			MC(0,ADDR_601C,LENper_601C*NUMmax_601C);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//采集规则库
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_601E[];
u32 Search_LIB601E_OAD(u32 OAD)//从采集规则库中搜索OAD的配置序号(从0开始),返回值>=NUMmax_601E表示没找到
{
	u32 i;
	u32 x;
	u32 n;
	u8 *p8;
	
	n=0;
	while(1)
	{
		if(n>=NUMmax_601E)
		{
			return n;
		}
		MR(ADDR_128KDATABUFF,ADDR_601E+(n*LENper_601E),LEN_128KDATABUFF);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<(LEN_128KDATABUFF/LENper_601E);i++)
		{
			if(n>=NUMmax_601E)
			{
				return n;
			}
			if(p8[0]==DataType_structure)
			{//删除时structure的数据单元清为0(NULL)
				x=(p8[3]<<24)+(p8[4]<<16)+(p8[5]<<8)+p8[6];//取CSD
				if(x==OAD)
				{
					return n;
				}
			}
			n++;
			p8+=LENper_601E;
		}
	}
}
u32 Search_LIB601E_SPACE(void)//从采集规则库中搜索第1个无配置的序号(从0开始),返回值>=NUMmax_601E表示没找到
{
	u32 i;
	u32 n;
	u8 *p8;
	
	n=0;
	while(1)
	{
		if(n>=NUMmax_601E)
		{
			return n;
		}
		MR(ADDR_128KDATABUFF,ADDR_601E+(n*LENper_601E),LEN_128KDATABUFF);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<(LEN_128KDATABUFF/LENper_601E);i++)
		{
			if(n>=NUMmax_601E)
			{
				return n;
			}
			if(p8[0]==0)
			{//删除时structure的数据单元清为0(NULL)
				return n;
			}
			n++;
			p8+=LENper_601E;
		}
	}
}
u32 ACTION_601E(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_601E;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：Add（array 采集规则）添加或更新一组采集规则。
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
				i=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				if((i&0xff)==0)
				{
					UARTCtrl->DAR=7;//类型不匹配           （7）
					return 0;
				}
				i>>=8;
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
				n=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];//取OAD
				n=Search_LIB601E_OAD(n);//从采集规则库中搜索OAD的配置序号(从0开始),返回值>=NUMmax_601E表示没找到
				if(n>=NUMmax_601E)
				{
					n=Search_LIB601E_SPACE();//从采集规则库中搜索第1个无配置的序号(从0开始),返回值>=NUMmax_601E表示没找到
					if(n>=NUMmax_601E)
					{
						UARTCtrl->DAR=8;//越界                 （8）
						return (LenRx<<16)+0;
					}
				}
				MW((u32)p8rx,pLib->Addr+(n*pLib->LENper),i);
			}
			return (LenRx<<16)+0;
		case 128://方法128：Delete(array CSD)删除一组采集规则。
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
				i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				if((i&0xff)==0)
				{
					UARTCtrl->DAR=7;//类型不匹配           （7）
					return 0;
				}
				i>>=8;
				if(LEN_RxAPDU<i)
				{
					UARTCtrl->DAR=9;//数据块不可用         （9）
					return 0;
				}
				n=(p8rx[2]<<24)+(p8rx[3]<<16)+(p8rx[4]<<8)+p8rx[5];//取OAD
				n=Search_LIB601E_OAD(n);//从采集规则库中搜索OAD的配置序号(从0开始),返回值>=NUMmax_601E表示没找到
				if(n<NUMmax_601E)
				{
					MC(0,pLib->Addr+(n*pLib->LENper),1);
				}
				LenRx+=i;
				p8rx+=i;
			}
			return (LenRx<<16)+0;
		case 129://方法129：Clear()清空采集规则库。
			MC(0,ADDR_601E,LENper_601E*NUMmax_601E);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}




























