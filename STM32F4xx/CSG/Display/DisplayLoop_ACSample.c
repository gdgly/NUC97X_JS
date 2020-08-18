

#include "../Hcsg/ProjectCSG.h"

#include "../Display/DisplayLoop_ACSample.h"
#include "../Display/Display.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
//#include "../Terminal/Terminal_ReadMeter_ACSample.h"



void Display_I(u32 Row,u32 ADDR_DATA)//电流显示
{
	u32 i;
	u16 *p16;
	i=MRR(ADDR_DATA,3);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*6));
	if(i&0x800000)
	{
		i&=0x7fffff;
		p16[0]='-'+0x2000;
	}
	p16++;
	DisplayData_OffZero(i,6,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
}
void Display_P(u32 Row,u32 ADDR_DATA)//功率显示
{
	u32 i;
	u16 *p16;
	i=MRR(ADDR_DATA,3);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*6));
	if(i&0x800000)
	{
		i&=0x7fffff;
		p16[0]='-'+0x2000;
	}
	p16++;
	DisplayData_OffZero(i,6,4,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
}
void Display_C(u32 Row,u32 ADDR_DATA)//功率因数显示
{
	u32 i;
	u16 *p16;
	i=MRR(ADDR_DATA,2);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*10));
	if(i&0x8000)
	{
		i&=0x7fff;
		p16[0]='-'+0x2000;
	}
	p16++;
	DisplayData_OffZero(i,4,3,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
}
void Display_E(u32 Row,u32 ADDR_DATA)//电能显示
{
	u32 i;
	u16 *p16;
	i=MRR(ADDR_DATA,4);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*8));
	DisplayData_OffZero(i,8,2,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
}
void Display_YMDHM(u32 Row,u32 ADDR_DATA)//最大需量发生时间显示
{
	u8 *p8;
	u16 *p16;
	p8=(u8*)(ADDR_DATA);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*6));
	p16[0]=(p8[4]>>4)+0x30+0x2000;
	p16[1]=(p8[4]&0xf)+0x30+0x2000;
	p16[2]='.'+0x2000;
	p16[3]=(p8[3]>>4)+0x30+0x2000;
	p16[4]=(p8[3]&0xf)+0x30+0x2000;
	p16[5]='.'+0x2000;
	p16[6]=(p8[2]>>4)+0x30+0x2000;
	p16[7]=(p8[2]&0xf)+0x30+0x2000;
	p16[8]=' '+0x2000;
	p16[9]=(p8[1]>>4)+0x30+0x2000;
	p16[10]=(p8[1]&0xf)+0x30+0x2000;
	p16[11]=':'+0x2000;
	p16[12]=(p8[0]>>4)+0x30+0x2000;
	p16[13]=(p8[0]&0xf)+0x30+0x2000;
}




u32 DisplayLoop_ACSample(u32 Row)//内部交采数据显示,返回下行
{
	u32 i;
#if (DLT500_REPORT == 0)
	u32 x;
	u16 *p16;
#endif
	u32 ADDR_DATA;
	ADDR_DATA=Get_ADDR_UARTnTx(ACSAMPLEPORT)+14;
	
#if (DLT500_REPORT == 1)
	if(TerminalDisplayLoop->Count == 0)
	{//配变取DLT500报告轮显只显示电压
			GetACSample(0x0201ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row+2,0,2,(u8 *)
						"电压(V):\n\r"
						"  Ua/Uab =\n\r"
						"  Ub/Uac =\n\r"
						"  Uc/Ubc =\n\r"
						);//显示菜单字符串
			Row++;
			for(i=0;i<3;i++)
			{
				DisplayData_OffZero(MRR(ADDR_DATA+(2*i),2),4,1,(u16 *)(ADDR_STRINGBUFF+(84*(Row+2+i))+4+(2*11)));//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			}
			Row+=3;
	}
#else
	switch(TerminalDisplayLoop->Count)
	{
		case 0://三相电压、三相电流
			//电压
			GetACSample(0x0201ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"电压(V):\n\r"
																			"  Ua/Uab =\n\r"
																			"  Ub/Uac =\n\r"
																			"  Uc/Ubc =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<3;i++)
			{
				DisplayData_OffZero(MRR(ADDR_DATA+(2*i),2),4,1,(u16 *)(ADDR_STRINGBUFF+(84*(Row+i))+4+(2*11)));//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			}
			Row+=3;
			//电流
			GetACSample(0x0202ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"电流(A):\n\r"
																			"  Ia =\n\r"
																			"  Ib =\n\r"
																			"  Ic =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<3;i++)
			{
				Display_I(Row+i,ADDR_DATA+(3*i));//电流显示
			}
			Row+=3;
			break;
		case 1://有功功率
			GetACSample(0x0203ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"有功功率(kW):\n\r"
																			"  P总=\n\r"
																			"  Pa =\n\r"
																			"  Pb =\n\r"
																			"  Pc =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<4;i++)
			{
				Display_P(Row+i,ADDR_DATA+(3*i));//功率显示
			}
			Row+=4;
			break;
		case 2://无功功率
			GetACSample(0x0204ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"无功功率(kvar):\n\r"
																			"  Q总=\n\r"
																			"  Qa =\n\r"
																			"  Qb =\n\r"
																			"  Qc =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<4;i++)
			{
				Display_P(Row+i,ADDR_DATA+(3*i));//功率显示
			}
			Row+=4;
			break;
		case 3://功率因数
			GetACSample(0x0206ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"功率因数:\n\r"
																			"  COS 总=\n\r"
																			"  COS a =\n\r"
																			"  COS b =\n\r"
																			"  COS c =\n\r"
																			);//显示菜单字符串
			p16=(u16 *)(ADDR_STRINGBUFF+(84*(Row+1))+4+(2*5));
			p16[0]=237+0x2000;
			p16+=84/2;
			p16[0]=237+0x2000;
			p16+=84/2;
			p16[0]=237+0x2000;
			p16+=84/2;
			p16[0]=237+0x2000;
			Row++;
			for(i=0;i<4;i++)
			{
				Display_C(Row+i,ADDR_DATA+(2*i));//功率因数显示
			}
			Row+=4;
			break;
		case 4://总及各费率正向有功电量
			GetACSample(0x0001ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"正向有功电量(kWh):\n\r"
																			"  总 =\n\r"
																			"  尖 =\n\r"
																			"  峰 =\n\r"
																			"  平 =\n\r"
																			"  谷 =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<5;i++)
			{
				Display_E(Row+i,ADDR_DATA+(4*i));//电能显示
			}
			Row+=5;
			break;
		case 5://总及各费率反向有功电量
			GetACSample(0x0002ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"反向有功电量(kWh):\n\r"
																			"  总 =\n\r"
																			"  尖 =\n\r"
																			"  峰 =\n\r"
																			"  平 =\n\r"
																			"  谷 =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<5;i++)
			{
				Display_E(Row+i,ADDR_DATA+(4*i));//电能显示
			}
			Row+=5;
			break;
		case 6://总及各费率組合无功1电量
			GetACSample(0x0003ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"组合无功1电量(kvarh)\n\r"
																			"  总 =\n\r"
																			"  尖 =\n\r"
																			"  峰 =\n\r"
																			"  平 =\n\r"
																			"  谷 =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<5;i++)
			{
				x=MRR(ADDR_DATA+(4*i)+3,1);
				if(x&0x80)
				{//负
					x&=0x7f;
					MWR(x,ADDR_DATA+(4*i)+3,1);
					p16=(u16 *)(ADDR_STRINGBUFF+(84*(Row+i))+4+(2*8));
					p16[0]='-'+0x2000;
					p16++;
					x=MRR(ADDR_DATA+(4*i),4);
					DisplayData_OffZero(x,8,2,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				}
				else
				{
					Display_E(Row+i,ADDR_DATA+(4*i));//电能显示
				}
			}
			Row+=5;
			break;
		case 7://总及各费率組合无功2电量
			GetACSample(0x0004ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"组合无功2电量(kvarh)\n\r"
																			"  总 =\n\r"
																			"  尖 =\n\r"
																			"  峰 =\n\r"
																			"  平 =\n\r"
																			"  谷 =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<5;i++)
			{
				x=MRR(ADDR_DATA+(4*i)+3,1);
				if(x&0x80)
				{//负
					x&=0x7f;
					MWR(x,ADDR_DATA+(4*i)+3,1);
					p16=(u16 *)(ADDR_STRINGBUFF+(84*(Row+i))+4+(2*8));
					p16[0]='-'+0x2000;
					p16++;
					x=MRR(ADDR_DATA+(4*i),4);
					DisplayData_OffZero(x,8,2,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				}
				else
				{
					Display_E(Row+i,ADDR_DATA+(4*i));//电能显示
				}
			}
			Row+=5;
			break;
		case 8://有功最大需量
			GetACSample(0x0101ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"正向有功最大需量(kW)\n\r"
																			"  总 =\n\r"
																			"  尖 =\n\r"
																			"  峰 =\n\r"
																			"  平 =\n\r"
																			"  谷 =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<5;i++)
			{
				Display_P(Row+i,ADDR_DATA+(8*i));//功率显示
			}
			Row+=5;
			break;
		case 9://有功最大需量发生时间
			GetACSample(0x0101ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
			DisplayMenuString(Row,0,2,(u8 *)
																			"正向有功最大需量时间\n\r"
																			" 总 =\n\r"
																			" 尖 =\n\r"
																			" 峰 =\n\r"
																			" 平 =\n\r"
																			" 谷 =\n\r"
																			);//显示菜单字符串
			Row++;
			for(i=0;i<5;i++)
			{
				Display_YMDHM(Row+i,ADDR_DATA+3+(8*i));//最大需量发生时间显示
			}
			Row+=5;
			break;
	}
#endif
	return Row;
}




















