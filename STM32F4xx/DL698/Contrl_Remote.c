
//遥控
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../DL698/EVENT_Record.h"

#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"



#if ((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void Terminal_Remote_Control(void)//终端控制-遥控跳闸
{
	u32 i;
	u32 x;
	u32 n;
	u32 OAD;
	u8 * p8;
	u8 *p8s;
	u8 * p8d;
	u16 * p16timer;
	
	p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
	p8d=(u8 *)(ADDR_Terminal_Remote_State);
	if(p8[1]!=0x0)
	{//保电
		for(i=0;i<NUMmax_CR;i++)
		{
			p8d[0]=0;//控制状态=0
			p8d+=Terminal_Ctrl_Pn_Len;
		}
		return;
	}
	MR(ADDR_128KDATABUFF+(64*1024),ADDR_8000_129,LENmax_8000_129);//遥控-方法129：跳闸（参数）
	//MR(ADDR_128KDATABUFF+(64*1024)+1024,ADDR_8000_130 ,LENmax_8000_130);//遥控-方法130：合闸（参数）
//#define ADDR_8000_5     ADDR_8000_4+LENmax_8000_4//遥控-属性4属性5（命令状态，只读)
//属性5（命令状态，只读)∷=bit-string(SIZE(8))
//继电器命令状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控跳闸命令输出状态，置“1”：跳闸命令；置“0”：合闸命令。
//#define LENmax_8000_5     3
	p8=(u8*)ADDR_8000_5;
	p8[0]=DataType_bit_string;
	p8[1]=NUMmax_CR;
	x=0;
	p8s=(u8 *)(ADDR_128KDATABUFF+(64*1024));
	n=p8s[1];
	if(n>NUMmax_CR)
	{
		n=NUMmax_CR;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OAD=(p8s[3]<<24)|(p8s[4]<<16)|(p8s[5]<<8)|p8s[6];//继电器OAD
		if(OAD!=0)
		{//跳闸命令
			x|=1<<(7-i);
		}
		p8s+=LENper_8000_129;
	}
	p8[2]=x;
//
	p16timer=(u16*)((u32)(&Terminal_Ram->Remote1_S_Timer));
	for(i=0;i<NUMmax_CR;i++)
	{
		p8s=(u8 *)(ADDR_128KDATABUFF+(64*1024));
		n=p8s[1];
		if(n>NUMmax_CR)
		{
			n=NUMmax_CR;
		}
		p8s+=2;
		for(x=0;x<n;x++)
		{
			OAD=(p8s[3]<<24)|(p8s[4]<<16)|(p8s[5]<<8)|p8s[6];//继电器OAD
			if((OAD&0xff)==(i+1))
			{
				break;
			}
			p8s+=LENper_8000_129;
		}
		if(x<n)
		{//找到对应继电器OAD
			x=(p8s[10]<<8)|p8s[11];//限电时间    long-unsigned（单位：分钟，换算：0；值为0表示永久限电）
			if(x!=0x0)
			{//有限电时间
				x=MRR(ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+22+(i*2),2);//读存贮器数据到寄存器,Byte<=8
				if(x==0x0)
				{//控制时间定时到
					p8d[0]=0;
					p8d+=Terminal_Ctrl_Pn_Len;
					continue;
				}
			}
			switch(p8d[0])
			{
				case 0://无控制或上电
					p8d[0]=1;
					break;
				case 1://投入
					x=p8s[8];//告警延时unsigned（单位：分钟，换算：0）
					x*=60;
					if(x==0x0)
					{//立即跳闸
						p8d[0]=101;
					}
					else
					{//告警延时
						p16timer[i]=x;
						p8d[0]=100;
					}
					break;
				case 100://报警
					if(p16timer[i]==0x0)
					{//报警定时到
						p8d[0]=101;
					}
					break;
				case 101://跳闸准备
					//跳闸时功率
					MR((u32)p8d+8,ADDR_230x_3+1,8);//总加组1
					//2分钟定时
					p16timer[i]=120;
					p8d[0]=102;
					//Terminal_Ctrl_Count(3);//终端跳闸统计计数;入口ci=0月电控跳闸,=1购电控跳闸,=2功控跳闸,3=遥控跳闸
					break;
				case 102://跳闸
					if(p16timer[i]==0x0)
					{//2分钟时间到
						//跳闸后2分钟总加有功功率long64(单位：W，换算-1)
						MR((u32)p8d+16,ADDR_230x_3+1,8);//总加组1
						//事件记录
						Event_31150200(p8s+2,1);//遥控事件记录(控后2分钟),入口:pOAD=事件发生源OAD（继电器单元）,occur=0恢复=1产生=2无	 
						p8d[0]=103;
					}
					break;
				case 103://跳闸2分钟后
					break;
				default:
					p8d[0]=0;//控制状态=0
					break;
			}
		}
		else
		{//没找到对应继电器OAD
			p8d[0]=0;
		}
		p8d+=Terminal_Ctrl_Pn_Len;
	}

//#define ADDR_8000_3     ADDR_230x_17+LENmax_230x_17//遥控-属性3（继电器输出状态，只读)
//属性3（继电器输出状态，只读)∷=bit-string(SIZE(8))
//继电器输出状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控跳闸输出状态，置“1”：跳闸状态；置“0”：合闸状态。
//#define LENmax_8000_3     3
	p8d=(u8 *)(ADDR_Terminal_Remote_State);
	p8=(u8*)ADDR_8000_3;
	p8[0]=DataType_bit_string;
	p8[1]=NUMmax_CR;
	x=0;
	for(i=0;i<NUMmax_CR;i++)
	{
		if(p8d[0]>=102)
		{//跳闸
			x|=1<<(7-i);
		}
		p8d+=Terminal_Ctrl_Pn_Len;
	}
	p8[2]=x;
//#define ADDR_8000_4     ADDR_8000_3+LENmax_8000_3//遥控-属性4(告警状态，只读)
//属性4(告警状态，只读)∷=bit-string(SIZE(8))
//告警状态：bit0～bit7分别按顺序对位表示1～8号继电器遥控告警输出状态，置“1”：处于告警状态；置“0”：未处于告警状态。
//#define LENmax_8000_4     3
	p8d=(u8 *)(ADDR_Terminal_Remote_State);
	p8=(u8*)ADDR_8000_4;
	p8[0]=DataType_bit_string;
	p8[1]=NUMmax_CR;
	x=0;
	for(i=0;i<NUMmax_CR;i++)
	{
		if(p8d[0]==100)
		{//报警
			x|=1<<(7-i);
		}
		p8d+=Terminal_Ctrl_Pn_Len;
	}
	p8[2]=x;
	
}

#endif






























