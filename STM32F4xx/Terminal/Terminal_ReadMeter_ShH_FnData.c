
//终端抄表上海表规约接收数据填入Fn数据缓冲
#include "Project.h"
#include "Terminal_ReadMeter_ShH_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




#if (USER/100)==5//上海
extern const u32 ShH_RMLib[];
void Terminal_ReadMeter_ShH_TxData(u32 PORTn,u8 *p8RMLib)//终端上海表规约抄表发送数据
{
/*
	上海规约帧格式：
	前导字符	FE  //0
	帧起始符	68H //1+0
	地址域	    A0  //1+1
				A1  //1+2
				A2  //1+3
				A3  //1+4	   制造厂代码，1字节ASCII码，如P，则该抄表帧里为50H，主站下发给终端为80H(BCD码)
				A4  //1+5
				A5  //1+6
	帧起始符	68H //1+7
	命令码		C   //1+8
	数据域长度	L   //1+9	   十六进制
	数据域		DATA//1+10	   数据域+33H处理
	纵向校验码	CS	//1+10+L
	帧结束符	16H	//1+11+L
*/
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[0]=0xFE;
	p8txbuff[1+0]=0x68;
	//取通信地址
	MR(((u32)p8txbuff)+1+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
//	p8txbuff[1+4]=bcd_hex(p8txbuff[1+4]);//8位压缩BCD数转为HEX
	//上海规约表的通信地址A5=0xff发送
	p8txbuff[1+6]=0xff;

	p8txbuff[1+7]=0x68;
	//命令码
	i=(u32)p8RMLib;
	i+=10+(2*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+1+8,i,1);
	p8txbuff[1+8]|=0x80;

	p8txbuff[1+9]=0;
	//CS计算
	p8txbuff[1+10]=0;
	for(i=0;i<10;i++)
	{
		p8txbuff[1+10]+=p8txbuff[1+i];
	}
	p8txbuff[1+11]=0x16;
	UARTCtrl->TxByte=1+12;//TX计数(需发送字节数)
}

void ReadMeter_ShH_FnData(u32 PORTn,u8 *p8RMLib)//上海规约表抄表接收数据填入Fn数据缓冲
{
	ReadMeter_ShH_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//上海规约表抄表接收数据填入Fn数据缓冲
}

void ReadMeter_ShH_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//上海规约表抄表接收数据填入Fn数据缓冲
{
/*
	针对上海表，集中器档案中费率数为2时，集中器费率1对应上海表费率平，集中器费率2对应上海表费率谷；
	集中器档案中费率数为4时，集中器费率1～4分别对应无效数据、上海表费率平、无效数据、上海表费率谷。
	此费率顺序对一类和二类数据均有要求。
*/			
	u32 i;
	u32 x;
	u32 YM;
	u32 YM1;
	u64 d64a;
	u64 d64b;
	
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

		
	//i=MRR(ShH_RMLib[UARTCtrl->FnCount],1);
	i=p8RMLib[0];
	switch(i)
	{
//3 F27 电能表日历时钟、编程次数及其最近一次操作时间
		case 27:
			p8FnData+=6;
			p8rxbuff+=10;
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0://电能表日历时钟YYMMDDWWHhmmss
					d64a=MRR(((u32)p8rxbuff),6);
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData),6);
					break;
				//电池工作时间 4字节
				//编程总次数   3字节
				//最近一次编程发生时刻 6字节
				//电表清零总次数 3字节
				//最近一次清零发生时刻 6字节
				//需量清零总次数	3字节
//				case 1://最近一次需量清零发生时刻
//					p8FnData+=3+3+4+3+6+3+6+3;
//					p8FnData[0]=0;
//					MW(((u32)p8rxbuff)+2+4,((u32)p8FnData)+1,4);
//					p8FnData[5]=0;
//					break;
//				case 2://最近一次事件清零发生时刻
				case 1://最近一次事件清零发生时刻
					p8FnData+=3+3+4+3+6+3+6+3+6+3;
					d64a=MRR(((u32)p8rxbuff),5);
					d64a<<=8;
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					p8=(u8 *)ADDR_DATABUFF;
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData),6);
					break;
//				case 3://最近一次校时发生时刻(用校时后时间)
				case 2://最近一次校时发生时刻(用校时后时间)
					p8FnData+=3+3+4+3+6+3+6+3+6+3+6+3;
					d64a=MRR(((u32)p8rxbuff),6);
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					p8=(u8 *)ADDR_DATABUFF;
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData),6);
					break;
			}
			break;
//16 F129 当前正向有功电能示值（总、费率1～M）
		case 129:
			UARTCtrl->RMError&=(~0x02);//本电能表/交流采样装置序号通信失败标志,B0=失败,B1=F129失败
			p8FnData+=6;
			p8rxbuff+=10;
AFN0DF161:
			p8=(u8 *)(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18);
			x=p8[0];//费率数
			if(x==4)
			{
				//费率数
				p8FnData[0]=x;
				//总
				p8FnData[1]=0;
				MW(((u32)p8rxbuff),(u32)p8FnData+2,4);
				//费率2
				p8FnData[11]=0;
				MW(((u32)p8rxbuff)+4,(u32)p8FnData+12,4);
				//费率4
				p8FnData[21]=0;
				MW(((u32)p8rxbuff)+8,(u32)p8FnData+22,4);
			}
			else
			{//费率2
				//费率数
				p8FnData[0]=2;
				//总
				p8FnData[1]=0;
				MW(((u32)p8rxbuff),(u32)p8FnData+2,4);
				//费率1
				p8FnData[6]=0;
				MW(((u32)p8rxbuff)+4,(u32)p8FnData+7,4);
				//费率2
				p8FnData[11]=0;
				MW(((u32)p8rxbuff)+8,(u32)p8FnData+12,4);
			}
			break;
//24 F137 上月（上一结算日）正向有功电能示值（总、费率1～M）
		case 137:
			p8FnData+=6;
			p8rxbuff+=10;
			p8=(u8 *)(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18);
			x=p8[0];//费率数
			//上1月正向有功数据
			YM=MRR(ADDR_TYMDHMS+4,2);//当前时间年月
			for(i=0;i<3;i++)
			{
				YM1=MRR(((u32)p8rxbuff)+3+i*17,2);//上n结算时间年月
				if(YM==YM1)
				{//当前时间年月等于上n结算时间年月
					break;
				}
			}
			if(x==4)
			{
				//费率数
				p8FnData[0]=x;
				if(i!=3)
				{
					//总
					p8FnData[1]=0;
					MW(((u32)p8rxbuff)+5+i*17,(u32)p8FnData+2,4);
					//费率2
					p8FnData[11]=0;
					MW(((u32)p8rxbuff)+4+5+i*17,(u32)p8FnData+12,4);
					//费率4
					p8FnData[21]=0;
					MW(((u32)p8rxbuff)+8+5+i*17,(u32)p8FnData+22,4);
				}
			}
			else
			{//费率2
				//费率数
				p8FnData[0]=2;
				if(i!=3)
				{
					//总
					p8FnData[1]=0;
					MW(((u32)p8rxbuff)+5+i*17,(u32)p8FnData+2,4);
					//费率1
					p8FnData[6]=0;
					MW(((u32)p8rxbuff)+4+5+i*17,(u32)p8FnData+7,4);
					//费率2
					p8FnData[11]=0;
					MW(((u32)p8rxbuff)+8+5+i*17,(u32)p8FnData+12,4);
				}
			}
			//上2月正向有功数据
			YM=YM_Sub1M(YM);//年月减1月,返回减1后的年月
			for(i=0;i<3;i++)
			{
				YM1=MRR(((u32)p8rxbuff)+3+i*17,2);//上n结算时间年月
				if(YM==YM1)
				{//当前时间年月减1月等于上n结算时间年月
					break;
				}
			}
			if(x==4)
			{
				//费率数
				p8FnData[26+5]=x;
				if(i!=3)
				{
					//总
					p8FnData[27+5]=0;
					MW(((u32)p8rxbuff)+5+i*17,(u32)p8FnData+28+5,4);
					//费率2
					p8FnData[37+5]=0;
					MW(((u32)p8rxbuff)+4+5+i*17,(u32)p8FnData+38+5,4);
					//费率4
					p8FnData[47+5]=0;
					MW(((u32)p8rxbuff)+8+5+i*17,(u32)p8FnData+48+5,4);
				}
			}
			else
			{//费率2
				//费率数
				p8FnData[26+5]=2;
				if(i!=3)
				{
					//总
					p8FnData[27+5]=0;
					MW(((u32)p8rxbuff)+5+i*17,(u32)p8FnData+28+5,4);
					//费率1
					p8FnData[32+5]=0;
					MW(((u32)p8rxbuff)+4+5+i*17,(u32)p8FnData+33+5,4);
					//费率2
					p8FnData[37+5]=0;
					MW(((u32)p8rxbuff)+8+5+i*17,(u32)p8FnData+38+5,4);
				}
			}
			break;
//32 F145 当月正向有功最大需量及发生时间（总、费率1～M）
//		case 145:
//			p8FnData+=6;
//			p8rxbuff+=10;
//			//费率数
//			p8FnData[0]=0;
//			//总需量
//			x=MRR(((u32)p8rxbuff),2);
//			x<<=4;
//			MWR(x,(u32)p8FnData+1,3);
//			//总时间
//			MW(((u32)p8rxbuff)+2,(u32)p8FnData+4,4);
//			break;
//49 F162 电能表日历时钟
		case 162:
			//星期计算
			d64a=MRR(((u32)p8rxbuff)+10,6);
			MWR(d64a,ADDR_DATABUFF,3);
			MWR(d64a>>24,ADDR_DATABUFF+4,3);
			p8=(u8 *)ADDR_DATABUFF;
			p8[3]=0;
			YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
			i=p8[3];
			if(i==0x0)
			{
				i=7;
			}
			i<<=5;
			d64b=i;
			d64b<<=32;
			d64a|=d64b;
			MWR(d64a,((u32)p8FnData)+1+6,6);

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
//50 F166 电能表参数修改次数及时间
		case 166:
			p8FnData+=6;
			p8rxbuff+=10;
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				//校时总次数 3字节
				case 0://电能表最近一次时钟修改前和后时间
					//校时前时间
					d64a=MRR(((u32)p8rxbuff)+6,6);
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData)+3,6);
					//校时后时间
					d64a=MRR(((u32)p8rxbuff),6);
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData)+9,6);
					break;
				//电能表时段参数修改次数  3字节
				case 1://最近一次电能表时段参数修改时间
					d64a=MRR(((u32)p8rxbuff),5);
					p8=(u8 *)ADDR_DATABUFF;
					p8[0]=0;
					MWR(d64a,ADDR_DATABUFF+1,2);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//指定地址缓冲中的年月日周时分秒计算周
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData)+18,6);
					break;
			}
			break;
//扩展3 集中抄表日冻结电能示值（总、费率1～M）
		case 0xE3:
			x=MRR(ADDR_TYMDHMS+3,3);
			MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//数据时标
			p8FnData[0]=1;//有数据填入标志
			MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF161_LastDate_Source,5);//抄表时间
			p8rxbuff+=10;
			p8FnData+=6+Office_AFN0DF161_LastDate_Source+5;
			goto AFN0DF161;
	}
}
#endif














