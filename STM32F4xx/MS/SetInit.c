

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../MS/SetInit.h"
#include "../Device/MEMRW.h"


typedef __packed struct
{
	u32 DataAddr;//数据开始地址
	u32 DataLen;//数据长度
	u16 LibLen;//下跟数据长度
	u8 InitDI;//初始化标识:0=清0无下跟数据,1=用下跟数据初始化,2=用下跟数据初始化且为通信参数;(当下跟数据长度<初始总长度时则用下跟数据重复初始化)
}SetLib_TypeDef;

void SetInit(u32 ADDR_LIB,u32 ADDR_LIBEnd,u32 Communicate)//参数初始化,入口ADDR_LIB参数初始化库地址,ADDR_LIBEnd库结束地址,Communicate通信参数0=不初始化,1=初始化
{
	u32 i;
	u32 DataAddr;
	u32 DataLen;
	u32 LibLen;
	SetLib_TypeDef *pSetLib;
	
	pSetLib=(SetLib_TypeDef*)(ADDR_DATABUFF);
	MR(ADDR_DATABUFF,ADDR_LIB,4);
	if(pSetLib->DataAddr!=0x5555AAAA)
	{
		return;
	}
	ADDR_LIB+=4;
	while(1)
	{
		if(ADDR_LIB>=ADDR_LIBEnd)
		{
			return;
		}
		MR(ADDR_DATABUFF,ADDR_LIB,11);
		ADDR_LIB+=11;
		DataAddr=pSetLib->DataAddr;
		if(DataAddr==0)
		{
			return;
		}
		DataLen=pSetLib->DataLen;
		LibLen=pSetLib->LibLen;
		if(Communicate==0)
		{//通信参数不初始化
			if(pSetLib->InitDI==2)
			{//当前是通信参数
				ADDR_LIB+=LibLen;
				continue;
			}
		}

		if(pSetLib->InitDI==0)
		{
			MC(0,DataAddr,DataLen);
		}
		else
		{
			MR(ADDR_DATABUFF,ADDR_LIB,LibLen);
			if(DataLen<=LibLen)
			{
				MW(ADDR_DATABUFF,DataAddr,DataLen);
			}
			else
			{
				while(DataLen!=0)
				{
					i=LibLen;
					while(i<DataLen)
					{
						if((i+LibLen)>LEN_DATABUFF)
						{
							break;
						}
						MW(ADDR_DATABUFF,ADDR_DATABUFF+i,LibLen);
						i+=LibLen;
					}
					if(i>DataLen)
					{
						i=DataLen;
					}
					MW(ADDR_DATABUFF,DataAddr,i);
					DataAddr+=i;
					DataLen-=i;
				}
			}
			ADDR_LIB+=LibLen;
		}
	}
}






