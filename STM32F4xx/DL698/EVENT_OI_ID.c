
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../Device/MEMRW.h"



/*
typedef __packed struct
{
	u16 OI;//事件OI
	u32 ADDR_FLASH_ID;//关联采集任务号 unsigned 地址(指向类型字节)
}OIID_LIB_TypeDef;


const OIID_LIB_TypeDef   OIID_LIB[]=
{
//电能表时钟超差事件
	0x3105,//OI
	ADDR_3105_6+5,//ADDR_FLASH_ID
//电能表示度下降事件
	0x310B,//OI
	ADDR_310B_6+2,//ADDR_FLASH_ID
//电能量超差事件
	0x310C,//OI
	ADDR_310C_6+7,//ADDR_FLASH_ID
//电能表飞走事件
	0x310D,//OI
	ADDR_310D_6+7,//ADDR_FLASH_ID
//电能表停走事件
	0x310E,//OI
	ADDR_310E_6+6,//ADDR_FLASH_ID
//电能表数据变更监控记录
	0x311C,//OI
	ADDR_311C_6+2,//ADDR_FLASH_ID
	
	
};


void OIID_FLASHtoSDRAM(void)//OI对应的ID任务号FLASHtoSDRAM
{
	u32 i;
	u32 n;//有效的OI对应ID数
	u8* p8;
	
	n=0;//有效的OI对应ID数
	p8=(u8*)ADDR_DATABUFF;
	for(i=0;i<sizeof(OIID_LIB);i++)
	{
		MR(ADDR_DATABUFF,OIID_LIB[i].ADDR_FLASH_ID,2);
		if(p8[0]==DataType_unsigned)
		{
			OIID->OI_ID[n].OI=OIID_LIB[i].OI;
			OIID->OI_ID[n].ID=p8[1];
			n++;
		}
	}
	OIID->NUM=n;

}

u32 Get_OIID(u32 OI)//事件OI关联的采集任务号ID;返回:采集任务号,>=RMTASKmax表示没找到
{
	u32 n;
	
	for(n=0;n<OIID->NUM;n++)
	{
		if(OIID->OI_ID[n].OI==OI)
		{
			return OIID->OI_ID[n].ID;
		}
	}
	return RMTASKmax;
}

*/







