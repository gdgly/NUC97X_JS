
//告警
#include "../Hcsg/ProjectCSG.h"
#include "../Display/Warning.h"


void WarningOccur(u32 WarningCode,u8 * pStr)//告警发生,入口告警代码和告警字符串
{
	u32 i;
	u8 *p8;
	u16 *p16;
	
	p16=(u16 *)0;
	switch(WarningCode)
	{
		case GPRSPORT:
			p16=(u16 *)(ADDR_WarningStringBuff+(0*84));
			break;
		case ENETPORT:
			p16=(u16 *)(ADDR_WarningStringBuff+(1*84));
			break;
		case RS485_1PORT:
			p16=(u16 *)(ADDR_WarningStringBuff+(2*84));
			break;
		case RS485_2PORT:
			p16=(u16 *)(ADDR_WarningStringBuff+(3*84));
			break;
		case RS485_3PORT:
			p16=(u16 *)(ADDR_WarningStringBuff+(4*84));
			break;
		case PLC_PORT://(载波)
			p16=(u16 *)(ADDR_WarningStringBuff+(5*84));
			break;
	}
	if(p16==0)
	{
		p16=(u16 *)(ADDR_WarningStringBuff+(6*84));
		for(i=6;i<MaxWarningString;i++)
		{
			if(p16[0]==WarningCode)
			{
				break;
			}
			p16+=84/2;
		}
		if(i>=MaxWarningString)
		{
			p16=(u16 *)(ADDR_WarningStringBuff+(6*84));
			for(i=6;i<MaxWarningString;i++)
			{
				if(p16[0]==0)
				{
					break;
				}
				p16+=84/2;
			}
			if(i>=MaxWarningString)
			{
				p16=(u16 *)(ADDR_WarningStringBuff+((MaxWarningString-1)*84));//复盖最后行
			}
		}
	}
	p16[0]=WarningCode;
	p8=(u8 *)(((u32)p16)+4);
	for(i=0;i<80;i++)
	{
		p8[i]=pStr[i];
		if(pStr[i]==0)
		{
			break;
		}
	}
}

void WarningCancel(u32 WarningCode)//告警取消,入口告警代码
{
	u32 i;
	u16 *p16;
	
	p16=(u16 *)(ADDR_WarningStringBuff);
	for(i=0;i<MaxWarningString;i++)
	{
		if(p16[0]==WarningCode)
		{
			p16[0]=0;
		}
		p16+=84/2;
	}
}

void WarningCancelAll(void)//取消全部告警,入口告警代码
{
	u32 i;
	u16 *p16;
	p16=(u16 *)(ADDR_WarningStringBuff);
	for(i=0;i<MaxWarningString;i++)
	{
		p16[0]=0;
		p16+=84/2;
	}
}





