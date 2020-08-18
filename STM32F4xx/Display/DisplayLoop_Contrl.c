
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Display/DisplayLoop_Contrl.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"


#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
u32 DisplayLoop_Contrl(u32 Row)//终端控制显示
{
	return Row;
}
#else
//催费告警显示计数号=0
//允许合闸通知显示计数号=1
//解除控制:遥控跳闸,当前功率下浮控,营业报停功控,厂休功控,时段功控,月电控,购电控=2
#define DisplayRemoteCount     (3+MaxControlRound)//遥控跳闸显示计数号
#define DisplayPowerCtrl1Count     (DisplayRemoteCount+MaxTotalAddGroup)//当前功率下浮控显示计数号
#define DisplayPowerCtrl2Count     (DisplayPowerCtrl1Count+MaxTotalAddGroup)//营业报停功控显示计数号
#define DisplayPowerCtrl3Count     (DisplayPowerCtrl2Count+MaxTotalAddGroup)//厂休功控显示计数号
#define DisplayPowerCtrl4Count     (DisplayPowerCtrl3Count+MaxTotalAddGroup)//时段功控显示计数号
#define DisplayEnergyCtrl1Count     (DisplayPowerCtrl4Count+MaxTotalAddGroup)//月电控显示计数号
#define DisplayEnergyCtrl2Count     (DisplayEnergyCtrl1Count+MaxTotalAddGroup)//购电控显示计数号

u32 DisplayLoop_Contrl(u32 Row)//终端控制显示
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 d64;
	u64 a64;
	u64 b64;
	u8 *p8;
	u16 *p16;

//催费告警
	if(TerminalDisplayLoop->Count==0)
	{
		x=MRR(ADDR_AFN05F26,1);
		if(x==0x55)
		{
			DisplayString(Row,0,0,(u8 *)"催费告警!");//显示字符串
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
		}
		return Row;
	}
//允许合闸通知
	if(TerminalDisplayLoop->Count==1)
	{
		p8=(u8 *)(ADDR_Terminal_Total_State);//终端总跳闸
		x=p8[1];
		x&=0x0f;//目前最大只显4轮
		while(x!=0x0)
		{
			i=BitLoca(x);//计算第1个置位为1的位置(0-32)
			DisplayString(Row,0,0,(u8 *)"轮次?合闸允许");//显示字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[4]=i+0x2030;
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			i--;
			i=1<<i;
			i=~i;
			x&=i;
		}
		return Row;
	}

//控制解除
	if(TerminalDisplayLoop->Count==2)
	{
		//遥控跳闸
		x=0;
		for(i=0;i<MaxControlRound;i++)
		{
			x+=MRR(ADDR_AFN05F1+(2*i),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"遥控跳闸当前无控制");//显示字符串
			Row++;
		}
		//当前功率下浮控
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWCtrl1_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"功率下浮控当前无控制");//显示字符串
			Row++;
		}
		//营业报停功控
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWCtrl2_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"报停功控当前无控制");//显示字符串
			Row++;
		}
		//厂休功控
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWCtrl3_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"厂休功控当前无控制");//显示字符串
			Row++;
		}
		//时段功控
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWCtrl4_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"时段功控当前无控制");//显示字符串
			Row++;
		}
		//月电控
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWhCtrl1_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"月电控当前无控制");//显示字符串
			Row++;
		}
		//购电控
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWhCtrl2_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"购电控当前无控制");//显示字符串
			Row++;
		}
		return Row;
	}
	
		
//遥控跳闸
	if(TerminalDisplayLoop->Count<DisplayRemoteCount)
	{
		Pn=TerminalDisplayLoop->Count-(DisplayRemoteCount-MaxControlRound);
		if(MRR(ADDR_AFN05F1+(2*Pn),1)!=0x0)
		{
			DisplayString(Row,0,0,(u8 *)"轮次?遥控跳闸投入");//显示字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[4]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;

			p8=(u8 *)(ADDR_Terminal_Remote_State+(Terminal_Ctrl_Pn_Len*Pn));
			x=p8[0];
			if(x==100)
			{//报警
				DisplayString(Row,0,0,(u8 *)"轮次?遥控跳闸报警");//显示字符串
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[4]=(Pn+1)+0x2030;
			}
			if(x>=101)
			{//跳闸
				DisplayString(Row,0,0,(u8 *)"轮次?遥控跳闸");//显示字符串
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[4]=(Pn+1)+0x2030;
			}

			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
		}
		return Row;
	}
//当前功率下浮控
	if(TerminalDisplayLoop->Count<DisplayPowerCtrl1Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayRemoteCount;
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//投入
			DisplayString(Row,0,0,(u8 *)"总加组?下浮控投入");//显示字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//当前功率
			DisplayString(Row,0,0,(u8 *)"当前功率=");//显示字符串
			i=MRR(ADDR_AFN0CF17+(Pn*2),2);
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//保安定值
			DisplayString(Row,0,0,(u8 *)"保安定值=");//显示字符串
			i=MRR(ADDR_AFN04F17,2);//读存贮器数据到寄存器,Byte<=8
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			if((d64>>63)!=0x0)
			{//负
				d64=(~d64)+1;
			}
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			if(x>2)
			{//定值已计算
				//定值功率
				DisplayString(Row,0,0,(u8 *)"定值功率=");//显示字符串
				i=MRR(ADDR_kWCtrl1_FixedValue+(Pn*2),2);//读存贮器数据到寄存器,Byte<=8
				d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
				d64=hex_bcd16(d64);
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
				//报警或跳闸
				if(x==100)
				{//功率超限报警
					DisplayString(Row,0,0,(u8 *)"功率超限报警");//显示字符串
					DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					Row++;
				}
				else
				{//跳闸
					x=p8[1];
					if(x!=0x0)
					{//有跳闸
						DisplayString(Row,0,0,(u8 *)"跳闸轮次");//显示字符串
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
						for(i=1;i<=8;i++)
						{
							if((x&0x1)!=0x0)
							{
								p16[0]=i+0x2030;
								p16[1]=','+0x2000;
								p16+=2;
							}
							x>>=1;
						}
						p16--;
						p16[0]=0;//删除最后','
						DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
						Row++;
					}
				}
			}
		}
		else
		{
			x=p8[4];
			if(x!=0x0)
			{
				//没投入原因
				DisplayString(Row,0,0,(u8 *)"总加组?下浮控投入");//显示字符串
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[6]=(Pn+1)+0x2030;
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
				DisplayString(Row,0,0,(u8 *)"控制时间定时到");//显示字符串
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
		}
		return Row;
	}
//营业报停功控
	if(TerminalDisplayLoop->Count<DisplayPowerCtrl2Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayPowerCtrl1Count;
		p8=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//投入
			DisplayString(Row,0,0,(u8 *)"总加组?报停控投入");//显示字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//当前功率
			DisplayString(Row,0,0,(u8 *)"当前功率=");//显示字符串
			i=MRR(ADDR_AFN0CF17+(Pn*2),2);
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//保安定值
			DisplayString(Row,0,0,(u8 *)"保安定值=");//显示字符串
			i=MRR(ADDR_AFN04F17,2);//读存贮器数据到寄存器,Byte<=8
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			if((d64>>63)!=0x0)
			{//负
				d64=(~d64)+1;
			}
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//定值功率
			DisplayString(Row,0,0,(u8 *)"定值功率=");//显示字符串
			i=MRR(ADDR_AFN04F44+(Pn*8)+6,2);//读存贮器数据到寄存器,Byte<=8
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//报警或跳闸
			if(x==100)
			{//功率超限报警
				DisplayString(Row,0,0,(u8 *)"功率超限报警");//显示字符串
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
			else
			{//跳闸
				x=p8[1];
				if(x!=0x0)
				{//有跳闸
					DisplayString(Row,0,0,(u8 *)"跳闸轮次");//显示字符串
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//删除最后','
					DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					Row++;
				}
			}
		}
		else
		{
			x=p8[4];
			if(x!=0x0)
			{
				//没投入原因
				DisplayString(Row,0,0,(u8 *)"总加组?报停控投入");//显示字符串
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[6]=(Pn+1)+0x2030;
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
				switch(x)
				{
					case 1:
						DisplayString(Row,0,0,(u8 *)"没到报停起始时间");//显示字符串
						break;
					case 2:
						DisplayString(Row,0,0,(u8 *)"已到报停结束时间");//显示字符串
						break;
					case 3:
						DisplayString(Row,0,0,(u8 *)"有优先级高下浮控");//显示字符串
						break;
					default:
						DisplayString(Row,0,0,(u8 *)"终端原因没投入");//显示字符串
						break;
				}
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
		}
		return Row;
	}
//厂休功控
	if(TerminalDisplayLoop->Count<DisplayPowerCtrl3Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayPowerCtrl2Count;
		p8=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//投入
			DisplayString(Row,0,0,(u8 *)"总加组?厂休控投入");//显示字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//当前功率
			DisplayString(Row,0,0,(u8 *)"当前功率=");//显示字符串
			i=MRR(ADDR_AFN0CF17+(Pn*2),2);
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//保安定值
			DisplayString(Row,0,0,(u8 *)"保安定值=");//显示字符串
			i=MRR(ADDR_AFN04F17,2);//读存贮器数据到寄存器,Byte<=8
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			if((d64>>63)!=0x0)
			{//负
				d64=(~d64)+1;
			}
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//定值功率
			DisplayString(Row,0,0,(u8 *)"定值功率=");//显示字符串
			i=MRR(ADDR_AFN04F42+(Pn*6),2);//读存贮器数据到寄存器,Byte<=8
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//报警或跳闸
			if(x==100)
			{//功率超限报警
				DisplayString(Row,0,0,(u8 *)"功率超限报警");//显示字符串
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
			else
			{//跳闸
				x=p8[1];
				if(x!=0x0)
				{//有跳闸
					DisplayString(Row,0,0,(u8 *)"跳闸轮次");//显示字符串
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//删除最后','
					DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					Row++;
				}
			}
		}
		else
		{
			x=p8[4];
			if(x!=0x0)
			{
				//没投入原因
				DisplayString(Row,0,0,(u8 *)"总加组?厂休控投入");//显示字符串
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[6]=(Pn+1)+0x2030;
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
				switch(x)
				{
					case 1:
						DisplayString(Row,0,0,(u8 *)"时段?不控制");//显示字符串
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p8=(u8 *)(ADDR_Terminal_Period);
						p16[4]=p8[3]+0x2030;
						break;
					case 2:
						DisplayString(Row,0,0,(u8 *)"非厂休日");//显示字符串
						break;
					case 3:
						DisplayString(Row,0,0,(u8 *)"非限电时间");//显示字符串
						break;
					case 4:
						DisplayString(Row,0,0,(u8 *)"有优先级高下浮控");//显示字符串
						break;
					case 5:
						DisplayString(Row,0,0,(u8 *)"有优先级高报停控");//显示字符串
						break;
					default:
						DisplayString(Row,0,0,(u8 *)"终端原因没投入");//显示字符串
						break;
				}
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
		}
		return Row;
	}
//时段功控	
	if(TerminalDisplayLoop->Count<DisplayPowerCtrl4Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayPowerCtrl3Count;
		p8=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//投入
			DisplayString(Row,0,0,(u8 *)"总加组?时段控投入");//显示字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//当前功率
			DisplayString(Row,0,0,(u8 *)"当前功率=");//显示字符串
			i=MRR(ADDR_AFN0CF17+(Pn*2),2);
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//保安定值
			DisplayString(Row,0,0,(u8 *)"保安定值=");//显示字符串
			i=MRR(ADDR_AFN04F17,2);//读存贮器数据到寄存器,Byte<=8
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			if((d64>>63)!=0x0)
			{//负
				d64=(~d64)+1;
			}
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//定值功率
			DisplayString(Row,0,0,(u8 *)"定值功率=");//显示字符串
			i=MRR(((u32)p8)+6,2);//读存贮器数据到寄存器,Byte<=8
			d64=Data02_kW(i);//数据格式02转为单位统一为kw的hex补码,有3位小数
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//报警或跳闸
			if(x==100)
			{//功率超限报警
				DisplayString(Row,0,0,(u8 *)"功率超限报警");//显示字符串
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
			else
			{//跳闸
				x=p8[1];
				if(x!=0x0)
				{//有跳闸
					DisplayString(Row,0,0,(u8 *)"跳闸轮次");//显示字符串
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//删除最后','
					DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					Row++;
				}
			}
		}
		else
		{
			x=p8[4];
			if(x!=0x0)
			{
				//没投入原因
				DisplayString(Row,0,0,(u8 *)"总加组?时段控投入");//显示字符串
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[6]=(Pn+1)+0x2030;
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
				switch(x)
				{
					case 1:
						DisplayString(Row,0,0,(u8 *)"时段?不控制");//显示字符串
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p8=(u8 *)(ADDR_Terminal_Period);
						p16[4]=p8[2]+0x2030;
						break;
					case 2:
						DisplayString(Row,0,0,(u8 *)"功控时段设置超８段");//显示字符串
						break;
					case 3:
						DisplayString(Row,0,0,(u8 *)"时段?没投入");//显示字符串
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p8=(u8 *)(ADDR_Terminal_Period);
						p16[4]=p8[2]+0x2030;
						break;
					case 4:
						DisplayString(Row,0,0,(u8 *)"无效方案号");//显示字符串
						break;
					case 5:
						DisplayString(Row,0,0,(u8 *)"方案?无定值");//显示字符串
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p16[4]=p8[5]+1+0x2030;
						break;
					case 6:
						DisplayString(Row,0,0,(u8 *)"方案?时段?定值没设置");//显示字符串
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p16[4]=p8[5]+1+0x2030;
						p8=(u8 *)(ADDR_Terminal_Period);
						p16[9]=p8[2]+0x2030;
						break;
					case 7:
						DisplayString(Row,0,0,(u8 *)"有优先级高下浮控");//显示字符串
						break;
					case 8:
						DisplayString(Row,0,0,(u8 *)"有优先级高报停控");//显示字符串
						break;
					case 9:
						DisplayString(Row,0,0,(u8 *)"有优先级高厂休控");//显示字符串
						break;
					default:
						DisplayString(Row,0,0,(u8 *)"终端原因没投入");//显示字符串
						break;
				}
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
		}
		return Row;
	}
//月电控
	if(TerminalDisplayLoop->Count<DisplayEnergyCtrl1Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayPowerCtrl4Count;
		p8=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//投入
			DisplayString(Row,0,0,(u8 *)"总加组?月电控投入");//显示字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//当月电量
			DisplayString(Row,0,0,(u8 *)"当月电量=");//显示字符串
			i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);
			x=i&0x0fffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(x,7,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			if((i&0x40000000)==0x0)
			{//kWh
				DispalyStringContinue(p16,(u8 *)"kWh");//显示字符串继续
			}
			else
			{//MWh
				DispalyStringContinue(p16,(u8 *)"MWh");//显示字符串继续
			}
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			//定值电量
			DisplayString(Row,0,0,(u8 *)"电量定值=");//显示字符串
			
			//取月电能量控定值
			i=MRR(ADDR_AFN04F46+(5*Pn),4);//读存贮器数据到寄存器,Byte<=8
			i&=0xefffffff;//去符号位
			a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
			//取终端月电能量控定值浮动系数
			i=MRR(ADDR_AFN04F20,1);//读存贮器数据到寄存器,Byte<=8
			x=i;
			i&=0x7f;
			i=bcd_hex(i);
			b64=(a64*i)/100;
			if((x>>7)==0x0)
			{//S0=0，表示上浮
				a64+=b64;
			}
			else
			{//S0=1，表示下浮
				a64-=b64;
			}
			//当前定值
			i=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
			//i=MRR(((u32)p8)+2,4);
			x=i&0x0fffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(x,7,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			if((i&0x40000000)==0x0)
			{//kWh
				DispalyStringContinue(p16,(u8 *)"kWh");//显示字符串继续
			}
			else
			{//MWh
				DispalyStringContinue(p16,(u8 *)"MWh");//显示字符串继续
			}
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			
			//报警门限
			DisplayString(Row,0,0,(u8 *)"报警门限=");//显示字符串
			i=MRR(ADDR_AFN04F46+(5*Pn),4);//读存贮器数据到寄存器,Byte<=8
			i&=0xefffffff;//去符号位
			a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
			//取报警门限浮动系数
			i=MRR(ADDR_AFN04F46+(5*Pn)+4,1);//读存贮器数据到寄存器,Byte<=8
//			x=i;
//			i&=0x7f;
//			i=bcd_hex(i);
//			b64=(a64*i)/100;
//			if((x>>7)==0x0)
//			{//S0=0，表示上浮
//				a64+=b64;
//			}
//			else
//			{//S0=1，表示下浮
//				a64-=b64;
//			}
			i=bcd_hex(i);
			b64=i;
			a64*=b64;
			a64/=100;
			//门限值
			i=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
			x=i&0x0fffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(x,7,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			if((i&0x40000000)==0x0)
			{//kWh
				DispalyStringContinue(p16,(u8 *)"kWh");//显示字符串继续
			}
			else
			{//MWh
				DispalyStringContinue(p16,(u8 *)"MWh");//显示字符串继续
			}
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			
			//报警或跳闸
			if(p8[0]==100)
			{//电量超限报警
				DisplayString(Row,0,0,(u8 *)"月用电超限报警");//显示字符串
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
			else
			{//跳闸
				x=p8[1];
				if(x!=0x0)
				{//有跳闸
					DisplayString(Row,0,0,(u8 *)"跳闸轮次");//显示字符串
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//删除最后','
					DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					Row++;
				}
			}
		}
		return Row;
	}
//购电控
	if(TerminalDisplayLoop->Count<DisplayEnergyCtrl2Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayEnergyCtrl1Count;
		p8=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//投入
			DisplayString(Row,0,0,(u8 *)"总加组?购电控投入");//显示字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
			Row++;
			if(p8[6]==0x0)//购电类别在剩余电量(费)计算时识别,0=购电量,1=购电费
			{//0=购电量
				//剩余电量
				DisplayString(Row,0,0,(u8 *)"剩余电量=");//显示字符串
				i=MRR(ADDR_AFN0CF23+(4*Pn),4);//读存贮器数据到寄存器,Byte<=8
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				if((i&0x10000000)!=0x0)
				{//负值
					p16[0]='-'+0x2000;
					p16++;
				}
				x=i&0x0fffffff;
				p16=DisplayData_OffZero(x,7,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				if((i&0x40000000)==0x0)
				{//kWh
					DispalyStringContinue(p16,(u8 *)"kWh");//显示字符串继续
				}
				else
				{//MWh
					DispalyStringContinue(p16,(u8 *)"MWh");//显示字符串继续
				}
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
				//跳闸门限
				DisplayString(Row,0,0,(u8 *)"跳闸门限=");//显示字符串
				i=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//读存贮器数据到寄存器,Byte<=8
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				if((i&0x10000000)!=0x0)
				{//负值
					p16[0]='-'+0x2000;
					p16++;
				}
				x=i&0x0fffffff;
				p16=DisplayData_OffZero(x,7,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				if((i&0x40000000)==0x0)
				{//kWh
					DispalyStringContinue(p16,(u8 *)"kWh");//显示字符串继续
				}
				else
				{//MWh
					DispalyStringContinue(p16,(u8 *)"MWh");//显示字符串继续
				}
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
			else
			{//1=购电费
				//剩余电费
				DisplayString(Row,0,0,(u8 *)"剩余电费=");//显示字符串
				i=MRR(ADDR_AFN0CF23+(4*Pn),4);//读存贮器数据到寄存器,Byte<=8
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				if((i&0x10000000)!=0x0)
				{//负值
					p16[0]='-'+0x2000;
					p16++;
				}
				i&=0xefffffff;
				d64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
				d64=hex_bcd16(d64);//HEX数转为16位压缩BCD数
				p16=DisplayData_OffZero(d64,10,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				DispalyStringContinue(p16,(u8 *)"元");//显示字符串继续
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
				//跳闸门限
				DisplayString(Row,0,0,(u8 *)"跳闸门限=");//显示字符串
				i=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//读存贮器数据到寄存器,Byte<=8
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				if((i&0x10000000)!=0x0)
				{//负值
					p16[0]='-'+0x2000;
					p16++;
				}
				i&=0xefffffff;
				d64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
				d64=hex_bcd16(d64);//HEX数转为16位压缩BCD数
				p16=DisplayData_OffZero(d64,10,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				DispalyStringContinue(p16,(u8 *)"元");//显示字符串继续
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
			//报警或跳闸
			if(p8[0]==100)
			{//电量超限报警
				if(p8[6]==0x0)
				{
					DisplayString(Row,0,0,(u8 *)"剩余电量欠限报警");//显示字符串
				}
				else
				{
					DisplayString(Row,0,0,(u8 *)"剩余电费欠限报警");//显示字符串
				}
				DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
				Row++;
			}
			else
			{//跳闸
				x=p8[1];
				if(x!=0x0)
				{//有跳闸
					DisplayString(Row,0,0,(u8 *)"跳闸轮次");//显示字符串
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//删除最后','
					DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
					Row++;
				}
			}
		}
		return Row;
	}

	return Row;
}
#endif//#if ((Project/100)!=3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块




