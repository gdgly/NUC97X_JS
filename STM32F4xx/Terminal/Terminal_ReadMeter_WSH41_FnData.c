
//终端抄表威胜规约4.1版本接收数据填入Fn数据缓冲
#include "Project.h"
#include "Terminal_ReadMeter_WSH41_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_CountAndFlag.h"



#if (USER/100)==10//河北
extern const u32 WSH41_RMLib[];
void Terminal_ReadMeter_WSH41_TxData(u32 PORTn,u8 *p8RMLib)//终端威胜规约4.1版本抄表发送
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	//取通信地址
	MR(((u32)p8txbuff),ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	
	i=p8txbuff[1]&0xf;
	i<<=8;
	i+=p8txbuff[0];
	//表地址转换
	p8txbuff[0]=bcd_hex(i);//8位压缩BCD数转为HEX
	//命令标志字55H
	p8txbuff[1]=0x55;
	
	//取DI
	i=(u32)p8RMLib;
	i+=10+(3*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+2,i,2);
	
	//取数据长度
	i+=2;
	MR(((u32)p8txbuff)+4,i,1);
	
	//CS计算
	p8txbuff[5]=0;
	for(i=0;i<5;i++)
	{
	p8txbuff[5]+=p8txbuff[i];
	}
	//结束码
	p8txbuff[6]=0x0D;
	UARTCtrl->TxByte=7;//TX计数(需发送字节数)
}

void ReadMeter_WSH41_FnData(u32 PORTn,u8 *p8RMLib)//威胜规约4.1版本抄表接收数据填入Fn数据缓冲
{
	ReadMeter_WSH41_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//威胜规约4.1版本抄表接收数据填入Fn数据缓冲
}

void ReadMeter_WSH41_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//威胜规约4.1版本抄表接收数据填入Fn数据缓冲
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u64 d64a;
	u64 d64b;
	
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=((u8 *)Get_ADDR_UARTnRx(PORTn))+4;
	p8FnData=(u8 *)Addr_FnDataBuff;
		
	i=p8RMLib[0];
	switch(i)
	{
//1 F25 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
		case 25:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			y=MRR(ADDR_AFN04F25+((Pn-1)*11)+10,1);//取电源接线方式 BS8  1
			y&=0x3;//D0～D1编码表示测量点电源接线方式，数值1～3依次表示三相三线、三相四线、单相表
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://总有功功率		
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约3字节（0.1W），1376.1规约3字节4位小数（kW）
					MWR(y,((u32)p8FnData)+6,3);
					break;
				case 1://A有功功率
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约3字节（0.1W），1376.1规约3字节4位小数（kW）
					MWR(y,((u32)p8FnData)+6+3,3);
					break;
				case 2://B有功功率
					if(y==1)
					{//三相三线
						break;
					}
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约3字节（0.1W），1376.1规约3字节4位小数（kW）
					MWR(y,((u32)p8FnData)+6+6,3);
					break;
				case 3://C有功功率
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约3字节（0.1W），1376.1规约3字节4位小数（kW）
					MWR(y,((u32)p8FnData)+6+9,3);
					break;
				case 4://总无功功率
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约3字节（0.1var），1376.1规约3字节4位小数（kvar）
					MWR(y,((u32)p8FnData)+6+12,3);
					break;
				case 5://A无功功率
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约3字节（0.1var），1376.1规约3字节4位小数（kvar）
					MWR(y,((u32)p8FnData)+6+15,3);
					break;
				case 6://B无功功率
					if(y==1)
					{//三相三线
						break;
					}
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约3字节（0.1var），1376.1规约3字节4位小数（kvar）
					MWR(y,((u32)p8FnData)+6+18,3);
					break;
				case 7://C无功功率
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约3字节（0.1var），1376.1规约3字节4位小数（kvar）
					MWR(y,((u32)p8FnData)+6+21,3);
					break;
				case 8://总功率因数
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约2字节（0.001），1376.1规约2字节1位小数（%）
					MWR(y,((u32)p8FnData)+6+24,2);
					break;
				case 9://A功率因数
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约2字节（0.001），1376.1规约2字节1位小数（%）
					MWR(y,((u32)p8FnData)+6+26,2);
					break;
				case 10://B功率因数
					if(y==1)
					{//三相三线
						break;
					}
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//威胜规约2字节（0.001），1376.1规约2字节1位小数（%）
					MWR(y,((u32)p8FnData)+6+28,2);
					break;
				case 11://C功率因数
					d64a=MRR((u32)p8rxbuff+5,2);
					d64a=hex_bcd16(d64a);//HEX数转为16位压缩BCD数
					//威胜规约2字节（0.001），1376.1规约2字节1位小数（%）
					MWR(d64a,((u32)p8FnData)+6+30,2);
					//总,A,B,C视在功率(用有功功率/功率因数计算)
					for(i=0;i<4;i++)
					{
						x=MRR(((u32)p8FnData)+6+(3*i),3);//取有功功率
						if(x!=0xeeeeee)
						{
							x&=0x7fffff;//去符号位
							x=bcd_hex(x);
							d64a=x;
							d64a*=1000;//功率因数有3位小数
							x=MRR(((u32)p8FnData)+6+24+(2*i),2);//取功率因数
							if(x!=0xeeee)
							{
								x&=0x7fff;//去符号位
								x=bcd_hex(x);
								d64a/=x;
								x=d64a;
								x=hex_bcd(x);
								MWR(x,((u32)p8FnData)+6+50+(3*i),3);
							}
						}
					}
					break;
				case 12://A相电压
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//电压 威胜规约2字节（0.01V），1376.1规约2字节1位小数（V）
					y>>=4;
					MWR(y,((u32)p8FnData)+6+32,2);
					break;
				case 13://B相电压
					if(y==1)
					{//三相三线
						break;
					}
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//电压 威胜规约2字节（0.01V），1376.1规约2字节1位小数（V）
					y>>=4;
					MWR(y,((u32)p8FnData)+6+34,2);
					break;
				case 14://C相电压
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//电压 威胜规约2字节（0.01V），1376.1规约2字节1位小数（V）
					y>>=4;
					MWR(y,((u32)p8FnData)+6+36,2);
					break;
				case 15://A相电流
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//电流 威胜规约2字节（0.01A），1376.1规约3字节3位小数（A）
					y<<=4;
					MWR(y,((u32)p8FnData)+6+38,3);
					break;
				case 16://B相电流
					if(y==1)
					{//三相三线
						break;
					}
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//电流 威胜规约2字节（0.01A），1376.1规约3字节3位小数（A）
					y<<=4;
					MWR(y,((u32)p8FnData)+6+41,3);
					break;
				case 17://C相电流
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					//电流 威胜规约2字节（0.01A），1376.1规约3字节3位小数（A）
					y<<=4;
					MWR(y,((u32)p8FnData)+6+44,3);
					break;
				//零序电流

				case 18://有功极性0-3位依次代表总、A、B、C，0为正1为负
					x=MRR((u32)p8rxbuff+5,1);
//					if((x&0x1)!=0x0)
//					{//总
//						if(p8FnData[6+2]!=0xee)
//						{
//							p8FnData[6+2]|=0x80;
//						}
//					}
//					if((x&0x2)!=0x0)
//					{//A
//						if(p8FnData[6+5]!=0xee)
//						{
//							p8FnData[6+5]|=0x80;
//						}
//					}
//					if((x&0x4)!=0x0)
//					{//B
//						if(p8FnData[6+8]!=0xee)
//						{
//							p8FnData[6+8]|=0x80;
//						}
//					}
//					if((x&0x8)!=0x0)
//					{//C
//						if(p8FnData[6+11]!=0xee)
//						{
//							p8FnData[6+11]|=0x80;
//						}
//					}
					for(i=0;i<4;i++)
					{
						if(p8FnData[6+2+i*3]!=0xee)
						{
							if(x&0x1)
							{
								p8FnData[6+2+i*3]|=0x80;
							}
						}
						x>>=1;
					}
					break;
				case 19://无功极性0-3位依次代表总、A、B、C，0为正1为负
					x=MRR((u32)p8rxbuff+5,1);
					for(i=0;i<4;i++)
					{
						if(p8FnData[6+14+i*3]!=0xee)
						{
							if(x&0x1)
							{
								p8FnData[6+14+i*3]|=0x80;
							}
						}
						x>>=1;
					}
					break;
			}
			break;
//2 F26 A、B、C三相断相统计数据及最近一次断相记录
		case 26:
			break;
//3 F27 电能表日历时钟、编程次数及其最近一次操作时间
		case 27:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表日历时钟YYMMDDHhmm
					// 威胜规约5字节（依次接收为年（后两位）月日时分），1376.1规约6字节（秒分时日月年）
					p8=(u8*)ADDR_DATABUFF;
					y=MRR((u32)p8rxbuff+5,1);
					p8[6]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+1,1);
					p8[5]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+2,1);
					p8[4]=hex_bcd(y);//HEX数转为8位压缩BCD数
					p8[3]=0;
					y=MRR((u32)p8rxbuff+5+3,1);
					p8[2]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+4,1);
					p8[1]=hex_bcd(y);//HEX数转为8位压缩BCD数
					p8[0]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					MW((u32)p8,((u32)p8FnData)+6,3);
					MW(((u32)p8)+4,((u32)p8FnData)+6+3,3);
					//星期
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					p8FnData[6+3+1]|=i;
					break;
				case 1://电池工作时间
					y=MRR((u32)p8rxbuff+5,4);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					// 威胜规约4字节（分），1376.1规约4字节（分）
					MWR(y,((u32)p8FnData)+6+6,4);
					break;
				case 2://编程总次数
					y=MRR((u32)p8rxbuff+5+12,1);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					// 威胜规约1字节，1376.1规约3字节
 					MWR(y,((u32)p8FnData)+6+10,3);
					//最大需量清零次数
					y=MRR((u32)p8rxbuff+5,1);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					// 威胜规约1字节，1376.1规约3字节
 					MWR(y,((u32)p8FnData)+6+28,3);					
					break;
				case 3://最近一次编程发生时刻
					p8FnData[6+13]=0;
					// 威胜规约4字节（月日时分），1376.1规约6字节（秒分时日月年）
					y=MRR((u32)p8rxbuff+5+3,1);
					p8FnData[6+14]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+2,1);
					p8FnData[6+15]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+1,1);
					p8FnData[6+16]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5,1);
					p8FnData[6+17]=hex_bcd(y);//HEX数转为8位压缩BCD数
					p8FnData[6+18]=0;
					break;
					//电表清零总次数
					//最近一次电表清零发生时刻
				case 4://最近一次最大需量清零发生时刻
					p8FnData[6+31]=0;
					// 威胜规约4字节（月日时分），1376.1规约6字节（秒分时日月年）
					y=MRR((u32)p8rxbuff+5+3,1);
					p8FnData[6+32]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+2,1);
					p8FnData[6+33]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+1,1);
					p8FnData[6+34]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5,1);
					p8FnData[6+35]=hex_bcd(y);//HEX数转为8位压缩BCD数
					p8FnData[6+36]=0;
					break;
					//事件清零总次数
					//最近一次事件清零发生时刻
					//校时总次数
					//最近一次校时发生时刻
			}
			break;
//4 F28 电表运行状态字及其变位标志
		case 28:
			break;
//5 F29 当前铜损、铁损有功总电能示值
//6 F30 上一结算日铜损、铁损有功总电能示值
		case 29:
		case 30:
			break;
//7 F31 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
//8 F32 上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
		case 31:
		case 32:
			break;

//9 F49 当前电压、电流相位角
//10 F57 当前A、B、C三相电压、电流2～N次谐波有效值
//11 F58 当前A、B、C三相电压、电流2～N次谐波含有率
//12 F65 当前电容器投切状态
//13 F66 当前电容器累计补偿投入时间和次数
//14 F67 当日、当月电容器累计补偿的无功电能量
//15 F73 直流模拟量当前数据


//16 F129 当前正向有功电能示值（总、费率1～M）
		case 129:
			UARTCtrl->RMError&=(~0x02);//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
//18 F131 当前反向有功电能示值（总、费率1～M）
		case 131:
			d64a=MRR((u32)p8rxbuff+5,4);
			d64a=hex_bcd16(d64a);//HEX数转为16位压缩BCD数
			//威胜规约4字节（Wh），1376.1规约5字节4位小数（kWh）
			d64a<<=4;
			MWR(d64a,((u32)p8FnData)+6+1+5*UARTCtrl->RMCount,5);
			p8FnData[6]=4;   //费率数给固定值
			break;
//24 F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
//26 F139 上月（上一结算日）反向有功电能示值（总、费率1～M）
		case 139:
		case 137:
			d64a=MRR((u32)p8rxbuff+5,3);
			d64a=hex_bcd16(d64a);//HEX数转为16位压缩BCD数
			//威胜规约3字节（Wh），1376.1规约5字节4位小数（kWh）
			d64a<<=4;
			MWR(d64a,((u32)p8FnData)+6+1+5*UARTCtrl->RMCount,5);
			p8FnData[6]=4;   //费率数给固定值
			break;
//40 F153 第一时区冻结正向有功电能示值（总、费率1～M）
//41 F154 第二时区冻结正向有功电能示值（总、费率1～M）
//42 F155 第三时区冻结正向有功电能示值（总、费率1～M）
//43 F156 第四时区冻结正向有功电能示值（总、费率1～M）
//44 F157 第五时区冻结正向有功电能示值（总、费率1～M）
//45 F158 第六时区冻结正向有功电能示值（总、费率1～M）
//46 F159 第七时区冻结正向有功电能示值（总、费率1～M）
//47 F160 第八时区冻结正向有功电能示值（总、费率1～M）
		case 153:
		case 154:
		case 155:
		case 156:
		case 157:
		case 158:
		case 159:
		case 160:
			break;
//17 F130 当前正向无功（组合无功1）电能示值（总、费率1～M）
//19 F132 当前反向无功（组合无功2）电能示值（总、费率1～M）
//25 F138 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
//27 F140 上月（上一结算日）反向无功（组合无功2）电能示值（总、费率1～M）
		case 130:
		case 132:
		case 138:
		case 140:
			break;
//20 F133 当前一象限无功电能示值（总、费率1～M）
//21 F134 当前二象限无功电能示值（总、费率1～M）
//22 F135 当前三象限无功电能示值（总、费率1～M）
//23 F136 当前四象限无功电能示值（总、费率1～M）
		case 133:
		case 134:
		case 135:
		case 136:
			d64a=MRR((u32)p8rxbuff+5,4);
			d64a=hex_bcd16(d64a);//HEX数转为16位压缩BCD数
			//威胜规约4字节（varh），1376.1规约4字节2位小数（kvarh）
			d64a>>=4;
			MWR(d64a,((u32)p8FnData)+6+1+4*UARTCtrl->RMCount,4);
			p8FnData[6]=4;   //费率数给固定值4
			break;
//28 F141 上月（上一结算日）一象限无功电能示值（总、费率1～M）
//29 F142 上月（上一结算日）二象限无功电能示值（总、费率1～M）
//30 F143 上月（上一结算日）三象限无功电能示值（总、费率1～M）
//31 F144 上月（上一结算日）四象限无功电能示值（总、费率1～M）
		case 141:
		case 142:
		case 143:
		case 144:
			y=MRR((u32)p8rxbuff+5,3);
			y=hex_bcd(y);//HEX数转为8位压缩BCD数
			//威胜规约3字节（varh），1376.1规约4字节2位小数（kvarh）
			y>>=4;
			MWR(y,((u32)p8FnData)+6+1+4*UARTCtrl->RMCount,4);
			p8FnData[6]=4;   //费率数给固定值4
			break;
//32 F145 当月正向有功最大需量及发生时间（总、费率1～M）
//34 F147 当月反向有功最大需量及发生时间（总、费率1～M）
//36 F149 上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
//38 F151 上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
		case 145:
		case 147:
		case 149:
		case 151:
			if((UARTCtrl->RMCount%2)==0)
			{			
				// 威胜规约2字节（0.1W），1376.1规约3字节4位小数（KW）
				y=MRR((u32)p8rxbuff+5,2);
				y=hex_bcd(y);//HEX数转为8位压缩BCD数
				MWR(y,((u32)p8FnData)+6+1+7*(UARTCtrl->RMCount/2),3);
			}
			else
			{
				// 威胜规约4字节（月日时分），1376.1规约4字节（分时日月）
				y=MRR((u32)p8rxbuff+5+3,1);
				p8FnData[6+1+3+7*(UARTCtrl->RMCount/2)]=hex_bcd(y);//HEX数转为8位压缩BCD数
				y=MRR((u32)p8rxbuff+5+2,1);
				p8FnData[6+1+4+7*(UARTCtrl->RMCount/2)]=hex_bcd(y);//HEX数转为8位压缩BCD数
				y=MRR((u32)p8rxbuff+5+1,1);
				p8FnData[6+1+5+7*(UARTCtrl->RMCount/2)]=hex_bcd(y);//HEX数转为8位压缩BCD数
				y=MRR((u32)p8rxbuff+5,1);
				p8FnData[6+1+6+7*(UARTCtrl->RMCount/2)]=hex_bcd(y);//HEX数转为8位压缩BCD数
			}
			p8FnData[6]=4;   //费率数给固定值
			break;
//33 F146 当月正向无功最大需量及发生时间（总、费率1～M）
//35 F148 当月反向无功最大需量及发生时间（总、费率1～M）
//37 F150 上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
//39 F152 上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
		case 146:
		case 148:
		case 150:
		case 152:
			break;
//48 F161 电能表远程控制通断电状态及记录
		case 161:
			break;
//49 F162 电能表日历时钟
		case 162:
			// 威胜规约5字节（依次接收为年（后两位）月日时分），1376.1规约6字节（秒分时日月年）
			p8=(u8*)ADDR_DATABUFF;
			y=MRR((u32)p8rxbuff+5,1);
			p8[6]=hex_bcd(y);//HEX数转为8位压缩BCD数
			y=MRR((u32)p8rxbuff+5+1,1);
			p8[5]=hex_bcd(y);//HEX数转为8位压缩BCD数
			y=MRR((u32)p8rxbuff+5+2,1);
			p8[4]=hex_bcd(y);//HEX数转为8位压缩BCD数
			p8[3]=0;
			y=MRR((u32)p8rxbuff+5+3,1);
			p8[2]=hex_bcd(y);//HEX数转为8位压缩BCD数
			y=MRR((u32)p8rxbuff+5+4,1);
			p8[1]=hex_bcd(y);//HEX数转为8位压缩BCD数
			p8[0]=0;
			YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
			MW((u32)p8,((u32)p8FnData)+7,3);
			MW(((u32)p8)+4,((u32)p8FnData)+7+3,3);
			//星期
			i=p8[3];
			if(i==0x0)
			{
				i=7;
			}
			i<<=5;
			p8FnData[7+3+1]|=i;
			//精确的抄表时间,年月日时分秒(星期)
			if(UARTCtrl->TransferDelay==0)
			{//线路上传输延时时间秒
				MW(ADDR_AFN0CF2,((u32)p8FnData)+1,6);//填入抄表时间
			}
			else
			{
				d64a=MRR(ADDR_TYMDHMS,6);
				d64a=YMDHMS_SubS(d64a,UARTCtrl->TransferDelay);//年月日时分秒减S秒,返回减S秒后的年月日时分秒
				//星期计算
				MWR(d64a,((u32)p8rxbuff)+14,3);
				MWR(d64a>>24,((u32)p8rxbuff)+14+4,3);
				YMDCalculateW(((u32)p8rxbuff)+14);//指定地址缓冲中的年月日周时分秒计算周
				i=p8rxbuff[14+3];
				if(i==0x0)
				{
					i=7;
				}
				i<<=5;
				d64b=i;
				d64b<<=32;
				d64a|=d64b;
				MWR(d64a,((u32)p8FnData)+1,6);//填入抄表时间
			}
			break;
//49 F165 电能表开关操作次数及时间
		case 165:
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://编程总次数
					y=MRR((u32)p8rxbuff+5+12,1);
					y=hex_bcd(y);//HEX数转为8位压缩BCD数
					// 威胜规约1字节，1376.1规约3字节
 					MWR(y,((u32)p8FnData)+6,3);
					break;
				case 1://最近一次编程发生时刻
					p8FnData[6+3]=0;
					// 威胜规约4字节（月日时分），1376.1规约6字节（秒分时日月年）
					y=MRR((u32)p8rxbuff+5+3,1);
					p8FnData[6+4]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+2,1);
					p8FnData[6+5]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5+1,1);
					p8FnData[6+6]=hex_bcd(y);//HEX数转为8位压缩BCD数
					y=MRR((u32)p8rxbuff+5,1);
					p8FnData[6+7]=hex_bcd(y);//HEX数转为8位压缩BCD数
					p8FnData[6+8]=0;
					break;
			}
			break;
			
//50 F166 电能表参数修改次数及时间
		case 166:
//51 F167 电能表购、用电信息
		case 167:
//52 F168 电能表结算信息
		case 168:
			break;
//		
//#if ERC8==1		
//		case 0xE1:
//0xE1 扩展1 ERC8电能表参数变更事件用数据
//			switch(UARTCtrl->RMCount)//抄表命令计数
//			{
//				case 0://最近一次编程时间(当Fn=27不抄时)
//					p8FnData[6]=0;
//					// 威胜规约4字节（月日时分），1376.1规约6字节（秒分时日月年）
//					y=MRR((u32)p8rxbuff+5+3,1);
//					p8FnData[6+1]=hex_bcd(y);//HEX数转为8位压缩BCD数
//					y=MRR((u32)p8rxbuff+5+2,1);
//					p8FnData[6+2]=hex_bcd(y);//HEX数转为8位压缩BCD数
//					y=MRR((u32)p8rxbuff+5+1,1);
//					p8FnData[6+3]=hex_bcd(y);//HEX数转为8位压缩BCD数
//					y=MRR((u32)p8rxbuff+5,1);
//					p8FnData[6+4]=hex_bcd(y);//HEX数转为8位压缩BCD数
//					p8FnData[6+5]=0;					
//					break;
//				case 1://最大需量清零次数(当Fn=27不抄时)
//					y=MRR((u32)p8rxbuff+5,1);
//					y=hex_bcd(y);//HEX数转为8位压缩BCD数
//					// 威胜规约1字节，1376.1规约3字节
// 					MWR(y,((u32)p8FnData)+6+6,3);									
//					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
//					if(Pn==0x0)
//					{//本序号的电能表/交流采样装置删除
//						break;
//					}
//					ERC8_Event(Pn-1,(u32)p8FnData);//电能表参数变更,入口Pn=测量点号(0开始),Fn抄表数据缓冲地址
//					break;
//			}
//			break;
//#endif
#if ERC13==1
//0xE2 扩展2 ERC13电表故障信息事件用数据
		case 0xE2:
			//编程次数(当Fn=27不抄时)和最大需量清零次数(当Fn=27不抄时)
			y=MRR((u32)p8rxbuff+5+12,1);
			y=hex_bcd(y);//HEX数转为8位压缩BCD数
			// 威胜规约1字节，1376.1规约3字节
			MWR(y,((u32)p8FnData)+6,3);
			//最大需量清零次数
			y=MRR((u32)p8rxbuff+5,1);
			y=hex_bcd(y);//HEX数转为8位压缩BCD数
			// 威胜规约1字节，1376.1规约3字节
			MWR(y,((u32)p8FnData)+6+3,3);					
			break;

#endif
	}
}

#endif












