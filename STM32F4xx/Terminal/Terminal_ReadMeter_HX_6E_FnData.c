
//终端抄表DL645_2007接收数据填入Fn数据缓冲
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "Terminal_ReadMeter_HX_6E_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_CountAndFlag.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"

#include "../terminal/Terminal_USER2xx.h"
#include "Terminal_ReadMeter_HX_6E_Lib.h"
#include "Terminal_ReadMeter_Event.h"
#include "../terminal/airthmetic.h"


#if ((Project/100)==3)&&((USER/100)==12)//四川专变
//红相电表的输入地址规则为前面补3个000后面写地址，
//如果地址不足9个数字则前面补12-sizeof(数字)个0
extern const u32 HX_6E_RMLib[];


void Terminal_ReadMeter_HX_6E_TxData(u32 PORTn,u8 *p8RMLib)//红相电表抄表发送数据
{
	u32 i ,j;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[0]= 0x02;
	p8txbuff[0+1]= 0x45;
	//取通信地址
	if(PORTn==ACSAMPLEPORT)
	{//是内部交采口
		MC(0xaa,((u32)p8txbuff)+1+1,6);
	}
	else
	{
		MR(((u32)p8txbuff)+1+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	}
	if(ChangeAddress(p8txbuff,2) == 0) 
	{
		for(i=0;i<10;i++)
		{
			p8txbuff[i] = 0; //故意破坏一下发送的数据使得电表不能通讯上
		}
		return;//对12个数字进行转换，转换成实际通讯中的地址
	}
// 协议中的源地址，现在自定义地址0x00000001
	p8txbuff[6] =0;
	p8txbuff[7] =0;
	p8txbuff[8] =0;
	p8txbuff[9] =0x01;
	
	p8txbuff[10] = 0;
	p8txbuff[11] = UARTCtrl->TxSEQ;
	if(p8txbuff[11] == 0xff)
	{
	  UARTCtrl->TxSEQ = 0;	 //序列号自己控制
	}
	else 
	{
		UARTCtrl->TxSEQ =  p8txbuff[11] +1;
	}
	//取DI	
	i=(u32)p8RMLib;
	i+=10+(5*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+12,i,4);
	if((p8txbuff[13] == 0)&&(p8txbuff[14] == 0)) 
	{//登入命令
		j =27	;
	}
	else 
	{//读取命令
		j = 15;
	}
	if(j == 27)
	{
		p8txbuff[13] = 0x45;
		p8txbuff[14] = 0x44;
		p8txbuff[15] = 0x4d;
		p8txbuff[16] = 0x49;
		p8txbuff[17] = 0x2c;
		p8txbuff[18] = 0x49;
		p8txbuff[19] = 0x4d;
		p8txbuff[20] = 0x44;
		p8txbuff[21] = 0x45;
		p8txbuff[22] = 0x49;
		p8txbuff[23] = 0x4d;
		p8txbuff[24] = 0x44;
		p8txbuff[25] = 0x45;
		p8txbuff[26] = 0;
	}
	i = (u32)Crc_HX(p8txbuff,j);
	p8txbuff[j] = (u8)(i>>8);
	p8txbuff[j+1] = (u8)(i&0x000000ff);

	i=(u32)CheckHXTxData(p8txbuff,0,(u8)(j+2));	 //这里发送库函数数据类型写小了，主要考虑发送的数据没那么长
	if(i>250)
	{
		return;
	}
	p8txbuff[i] = 0x03;
	UARTCtrl->TxByte=i+1;//TX计数(需发送字节数)
}

void ReadMeter_HX_6E_FnData(u32 PORTn,u8 *p8RMLib)//HX抄表接收数据填入Fn数据缓冲
{
	ReadMeter_HX_6E_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//HX抄表接收数据填入Fn数据缓冲
}

void ReadMeter_HX_6E_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//HX抄表接收数据填入Fn数据缓冲
{
	u32 i;
	u32 x;
	u32 week;
	u64 d64a;
	u64 d64b;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;
	union change{u8 d[4]; float f;}changedata;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

	i=p8RMLib[0];
	switch(i)
	{
		case 129://A14
			if(UARTCtrl->RMCount != 0)
			{
				goto DATAPROCESS1;	//负值也可以通过 但是负数还在下一步骤中直接return
			}
			else
			{//=0是登入命令
				break;
			}
		case 130://A11
		case 131://A14
		case 132://A11
		case 133://A11
		case 134://A11
		case 135://A11	
		case 136://A11	
			goto DATAPROCESS1;	//A11    //负值也可以通过 但是负数还在下一步骤中直接return
		case 145://A23  A17  
		case 146://A23  A17
		case 147://A23  A17
		case 148://A23  A17	
			goto DATAPROCESS2;//A23  A17//A23 负值也可以通过 但是负数还在下一步骤中直接return

		case 25:
			goto DATAPROCESS3;	//A9 A5 A7 A25		 //小心正负值，协议里面可能会出现负值 负值也可以通过
		case 27:
			goto DATAPROCESS4;		     
		case 49:
			goto DATAPROCESS5;	//A5		 //小心正负值，协议里面可能会出现负值 负值也可以通过
		case 162:	
			switch(UARTCtrl->RMCount)//抄表命令计数
			{
				case 0:
					p8FnData[7] = _8HexToBCD(p8rxbuff[17]);//秒
					p8FnData[8] = _8HexToBCD(p8rxbuff[16]);//分
					p8FnData[9] = _8HexToBCD(p8rxbuff[15]);//时
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
						MWR(d64a,((u32)p8rxbuff)+15,3);
						MWR(d64a>>24,((u32)p8rxbuff)+15+4,3);
						YMDCalculateW(((u32)p8rxbuff)+15);//指定地址缓冲中的年月日周时分秒计算周
						i=p8rxbuff[15+3];
						if(i==0x0)
						{
							i=7;//星期日期=7
						}
						i<<=5;
						d64b=i;
						d64b<<=32;
						d64a|=d64b;
						MWR(d64a,((u32)p8FnData)+1,6);//填入抄表时间
					}
					break;
				case 1: 
					p8FnData[10] = _8HexToBCD(p8rxbuff[15]);//日
					p8FnData[11] = _8HexToBCD(p8rxbuff[16]);//月
					p8FnData[12] = _8HexToBCD(p8rxbuff[17]);//年
					week=YMDCalculateWeek((uint32_t)p8rxbuff+15);//指定地址缓冲中的年月日计算周
					if(week==0)
					{
						week=7;
					}
					week<<=5;
					p8rxbuff[16]|=week;
					p8FnData[11] = p8rxbuff[16];//星期-月
					break;
			}
			break;

	DATAPROCESS1:		  // 现在处理数据分类分的不好，都是写的过程中才发现不合理
							  // 等测试通过之后再来修改这里，将类型重新分一下
		x=4;
		if(x>TMaxTariff)
		{
		  x=TMaxTariff;
		}
		p8FnData[6]=x;
		p8FnData+=7;
		p8rxbuff+=15;
		changedata.d[0] = p8rxbuff[3];
		changedata.d[1] = p8rxbuff[2];
		changedata.d[2] = p8rxbuff[1];
		changedata.d[3] = p8rxbuff[0];
		if(changedata.f<0)
		{
			return; //数字负数异常处理
		}
		if(i==129)
		{
			 x = UARTCtrl->RMCount-1;//第1条是登入命令不存储
		}
		else
		{
			 x = UARTCtrl->RMCount;
		}
		if((i==129)||(i==131))
		{//有功电能
			if(	ChangeFloatTo376BCD(changedata.f,p8rxbuff,1,1) == 1)
			{//A14的格式
				p8FnData[0+5*x]=p8rxbuff[0];	  
				p8FnData[1+5*x]=p8rxbuff[1];
				p8FnData[2+5*x]=p8rxbuff[2];
				p8FnData[3+5*x]=p8rxbuff[3];
				p8FnData[4+5*x]=p8rxbuff[4];
			}
		}
		if((i==130)||((i>131)&&(i<=136)))
		{//无功电能
			if(	ChangeFloatTo376BCD(changedata.f,p8rxbuff,2,0) == 1)
			{//A11的格式
				p8FnData[0+4*x]=p8rxbuff[0];	  
				p8FnData[1+4*x]=p8rxbuff[1];
				p8FnData[2+4*x]=p8rxbuff[2];
				p8FnData[3+4*x]=p8rxbuff[3];
			}
		}
	  break;
		
	DATAPROCESS2:
		x=4;
		if(x>TMaxTariff)
		{
			x=TMaxTariff;
		} 
		p8FnData[6] = x;
		p8FnData+= 7;
		p8rxbuff+= 15;
		x = UARTCtrl->RMCount;
		if((x%2)==0)
		{//最大需量
			changedata.d[0] = p8rxbuff[3];
			changedata.d[1] = p8rxbuff[2];
			changedata.d[2] = p8rxbuff[1];
			changedata.d[3] = p8rxbuff[0];
			if(changedata.f<0)
			{
				return; //数字负数异常处理
			}
			if(	ChangeFloatTo376BCD(changedata.f,p8rxbuff,3,1) == 1)
			{//A23的格式
				p8FnData[0+(x/2)*7]=p8rxbuff[0];	  
				p8FnData[1+(x/2)*7]=p8rxbuff[1];
				p8FnData[2+(x/2)*7]=p8rxbuff[2];
			}
		}
		else
		{//最大需量发生时间
			p8FnData[3+(x/2)*7]=_8HexToBCD(p8rxbuff[4]); 	//分 	A17
			p8FnData[4+(x/2)*7]=_8HexToBCD(p8rxbuff[3]);	//时	A17
			p8FnData[5+(x/2)*7]=_8HexToBCD(p8rxbuff[0]);	//日	A17
			p8FnData[6+(x/2)*7]=_8HexToBCD(p8rxbuff[1]);	//月	A17
		}
		break;	
		
	DATAPROCESS3:
		p8FnData+= 6;
		p8rxbuff+= 15;
		x = UARTCtrl->RMCount;
		changedata.d[0] = p8rxbuff[3];
		changedata.d[1] = p8rxbuff[2];
		changedata.d[2] = p8rxbuff[1];
		changedata.d[3] = p8rxbuff[0];
		if(x<=7)
		{//当前总、A、B、C有/无功功率
			if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,4,1) == 1)
			{//A9的格式
				p8FnData[0+x*3]=p8rxbuff[0];	  
				p8FnData[1+x*3]=p8rxbuff[1];
				p8FnData[2+x*3]=p8rxbuff[2];
			}
		}
		if(x == 8)
		{//当前功率因数
			if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,5,2) == 1)
			{//A5的格式
				p8FnData[24]=p8rxbuff[0];	  
				p8FnData[25]=p8rxbuff[1];
			}
		}
		if((x>=9)&&(x<=11))	//空掉3个数据项（ABC功率因数）	  2*3个字节
		{//ABC相电压
			if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,6,1) == 1)
			{//A7的格式
				p8FnData[32+(x-9)*2]=p8rxbuff[0];	  
				p8FnData[33+(x-9)*2]=p8rxbuff[1];
			}
		}
		if((x>=12)&&(x<=14))
		{//ABC相电流
			if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,7,2) == 1)
			{//A25的格式
				p8FnData[38+(x-12)*3]=p8rxbuff[0];	  
				p8FnData[39+(x-12)*3]=p8rxbuff[1];
				p8FnData[40+(x-12)*3]=p8rxbuff[2];
			}
		} 
    break;	
		
	DATAPROCESS4:
		p8FnData+= 6;
		p8rxbuff+= 15;
		x = UARTCtrl->RMCount;
		switch(x)
		{
			case 0:	
				p8FnData[0] = _8HexToBCD(p8rxbuff[2]);//秒
				p8FnData[1] = _8HexToBCD(p8rxbuff[1]);//分
				p8FnData[2] = _8HexToBCD(p8rxbuff[0]);//时
				break;
			case 1: 
				week=YMDCalculateWeek((uint32_t)p8rxbuff);//指定地址缓冲中的年月日计算周
				if(week==0)
				{
					week=7;
				}
				week<<=5;
				p8rxbuff[1]|=week;
				p8FnData[3] = p8rxbuff[0];//日
				p8FnData[4] = p8rxbuff[1];//月
				p8FnData[5] = p8rxbuff[2];//年				
				break;
			case 2:
				if(_32HexToBCD(p8rxbuff,p8FnData,19) == 1)	   //现在是用了MK6的指令，MK3是另外一个指令，现在没样表无法验证
					break;
			case 3:	
				p8FnData[22] = _8HexToBCD(p8rxbuff[5]);
				p8FnData[23] = _8HexToBCD(p8rxbuff[4]);
				p8FnData[24] = _8HexToBCD(p8rxbuff[3]);
				p8FnData[25] = _8HexToBCD(p8rxbuff[0]);
				p8FnData[26] = _8HexToBCD(p8rxbuff[1]);
				p8FnData[27] = _8HexToBCD(p8rxbuff[2]);
				break;
			default:
				break;
		}
		break;
	DATAPROCESS5:
		p8FnData+= 1;
		p8rxbuff+= 15;
		x = UARTCtrl->RMCount;
		changedata.d[0] = p8rxbuff[3];
		changedata.d[1] = p8rxbuff[2];
		changedata.d[2] = p8rxbuff[1];
		changedata.d[3] = p8rxbuff[0];
		if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,5,1) == 1)
		{
			p8FnData[0+x*2]=p8rxbuff[0];	  //A5的格式
			p8FnData[1+x*2]=p8rxbuff[1];
		}
		break;
	}
}
#endif
