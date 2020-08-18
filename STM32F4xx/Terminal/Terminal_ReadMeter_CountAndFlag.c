
//终端抄表计数和成功失败标志
#include "Project.h"
#include "Terminal_ReadMeter_CountAndFlag.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0E_Event.h"
#include "../Calculate/Calculate.h"




void RM_CountAndFlags(u32 PORTn,u32 MeterNo,u32 f)//电能表抄表成功失败计数和标志,入口UART缓冲地址,电能表序号,标志f=0成功,f=1失败
{
	u32 i;
	u8 * p8;
	u16 * p16;
	
	//每表用2位原电能表抄表标志(0=成功,1=失败->成功,2=成功->失败,3=失败)
	if((MeterNo!=0x0)&&(MeterNo<=MaxRS485AddCarrierPn))
	{
		MeterNo--;
		p8=(u8 *)(ADDR_RM_CountFlags+32+(MeterNo/4));
		MeterNo%=4;
		MeterNo<<=1;
		i=3;
		i<<=MeterNo;
		
		if(ERC_Event_Set(31)==0)//检查事件记录设置,返回0=不记录,1=记录
		{
			if(f==0x0)
			{//成功
					p8[0]&=~i;
					i=1;
					i<<=MeterNo;
					p8[0]|=i;
			}
			else
			{//失败
					p8[0]&=~i;
					i=2;
					i<<=MeterNo;
					p8[0]|=i;
			}
		}
		else
		{
			if(f==0x0)
			{//成功
				if(((p8[0]>>MeterNo)&3)<=1)
				{//原也成功
					p8[0]&=~i;
				}
				else
				{//原失败,失败->成功
					p8[0]&=~i;
					i=1;
					i<<=MeterNo;
					p8[0]|=i;
				}
			}
			else
			{//失败
				if(((p8[0]>>MeterNo)&3)>=2)
				{//原也失败
					p8[0]|=i;
				}
				else
				{//原成功,成功->失败
					p8[0]&=~i;
					i=2;
					i<<=MeterNo;
					p8[0]|=i;
				}
			}
		}	
			
		//成功失败计数
		p16=(u16 *)(ADDR_RM_CountFlags);
		switch(PORTn)
		{
			case RS485_1PORT:
				i=0;
				break;
			case RS485_2PORT:
				i=2;
				break;
			case RS485_3PORT:
				i=4;
				break;
			case RS485_4PORT:
				i=6;
				break;
			default:
				return;
		}
		if(f==0x0)
		{
			p16[i]++;//成功计数
		}
		else
		{
			p16[i+1]++;//失败计数
		}
	}
}


