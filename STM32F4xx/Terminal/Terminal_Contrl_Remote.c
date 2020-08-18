
//终端控制-遥控跳闸
#include "Project.h"
#include "Terminal_Contrl_Remote.h"
#include "../device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"





#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
void Terminal_Remote_Control(void)//终端控制-遥控跳闸
{
}
#else
void Terminal_Remote_Control(void)//终端控制-遥控跳闸
{
	u32 i;
	u32 Pn;
	u8 * p8;
	u8 *p8s;
	u8 * p8d;
	u16 * p16timer;
	
	
	p8=(u8 *)(ADDR_Terminal_Protect);//终端保电,0=不保电,1=命令保电,2=自动保电
	p8s=(u8 *)(ADDR_AFN05F1);
	p8d=(u8 *)(ADDR_Terminal_Remote_State);

	for(Pn=0;Pn<MaxControlRound;Pn++)
	{
		if(p8[0]!=0x0)
		{//保电
NoCtrl://不控制
			p8d[0]=0;//控制状态=0
			goto NextPn;
		}
		if(MRR(((u32)p8s),1)==0x55)
		{//刚投入
			i=MRR(((u32)p8s)+1,1)&0xf;//限电时间0.5h
			i*=30;
			MWR(i,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+22+(Pn*2),2);//写寄存器数据到存贮器,Byte<=8
			MC(0x5a,ADDR_AFN05F1+(2*Pn),1);//0x55->0x5a
			p8d[0]=1;//控制状态=1
			goto NextPn;
		}
		if(MRR(((u32)p8s),1)!=0x5a)
		{//解除
			goto NoCtrl;//不控制
		}
		if((MRR(((u32)p8s)+1,1)&0xf)!=0x0)
		{//有限电时间
			i=MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+22+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
			if(i==0x0)
			{//控制时间定时到
				MC(0x0,ADDR_AFN05F1+(2*Pn),1);//清0x5a->0
				goto NoCtrl;//不控制
			}
		}
		p16timer=(u16 *)((u32)(&Terminal_Ram->Remote1_S_Timer));
		switch(p8d[0])
		{
			case 0://无控制或上电
				p8d[0]=1;
				break;
			case 1://投入
				i=(MRR(((u32)p8s)+1,1)>>4)&0xf;
				i*=60;
				if(i==0x0)
				{//立即跳闸
					p8d[0]=101;
				}
				else
				{//告警延时
					p16timer[Pn]=i;
					p8d[0]=100;
				}
				break;
			case 100://报警
				if(p16timer[Pn]==0x0)
				{//报警定时到
					p8d[0]=101;
				}
				break;
			case 101://跳闸准备
				//跳闸时功率
				i=MRR(ADDR_AFN0CF17,2);
				MWR(i,((u32)p8d)+2,2);//写寄存器数据到存贮器,Byte<=8
				//2分钟定时
				p16timer[Pn]=120;
				p8d[0]=102;
				Terminal_Ctrl_Count(3);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
				break;
			case 102://跳闸
				if(p16timer[Pn]==0x0)
				{//2分钟时间到
					//事件记录
					i=MRR(ADDR_AFN0CF17,2);//跳闸2分后功率
					ERC5_Event(Pn,MRR(((u32)p8d)+2,2),i);//遥控跳闸记录;入口n=跳闸轮次(0-7),kw1=跳闸时功率(总加功率),kw2=跳闸后2分钟的功率(总加功率)
					p8d[0]=103;
				}
				break;
			case 103://跳闸2分钟后
				break;
			default:
				p8d[0]=0;//控制状态=0
				break;
		}
		
NextPn:
		p8s+=2;
		p8d+=Terminal_Ctrl_Pn_Len;
	}
}
#endif




