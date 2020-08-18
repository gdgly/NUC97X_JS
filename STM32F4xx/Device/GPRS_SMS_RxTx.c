
#if MainProtocol==376
#include "../Head/Project.h"
#include "../Terminal/Terminal_Uart_3761.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_Uart.h"
#endif
#include "GPRS_SMS_RxTx.h"
#include "../Device/GPRS_3763.h"
#include "../Device/ATNo.h"
#include "..//Terminal/ATString.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"

#include "../STM32F4xx/STM32F4xx_SoftDelay.h"



u32 LEN_TelephoneNumber(u32 Addr)//电话号码有效位数,入口电话号存贮地址
{
	u32 i;
	u32 x;
	u8 * p8;
	
	MR(ADDR_DATABUFF,Addr,8);
	i=8;//8字节设置
	x=0;//号码位数=0
	p8=(u8 *)(ADDR_DATABUFF);
	if(p8[0]==0x86)
	{
		i--;
		p8++;
	}
	for(;i!=0x0;i--)
	{
		if((p8[0]>>4)==0xf)
		{//号码未
			break;
		}
		x++;
		if((p8[0]&0xf)==0xf)
		{//号码未
			break;
		}
		x++;
		p8++;
	}
	return x;
}

u8 * SMS_BCD_ASCII(u32 BCD,u8 *p8)//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元;返回下个存贮指针p8
{
	u32 i;
	i=BCD>>4;
	if(i<10)
	{
		i+=0x30;
	}
	else
	{
		i+=55;
	}
	p8[0]=i;
	i=BCD&0xf;
	if(i<10)
	{
		i+=0x30;
	}
	else
	{
		i+=55;
	}
	p8[1]=i;
	p8+=2;
	return(p8);
}

u32 SMS_ASCII_BCD(u32 Addr,u32 BCDByte)//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8s;
	u8 *p8d;
	p8s=(u8*)Addr;
	p8d=p8s;
	y=0;
	for(i=0;i<BCDByte;i++)
	{
		y<<=8;
		x=p8s[0];
		if((x>=0x30)&&(x<=0x39))
		{
			x-=0x30;
		}
		else
		{
			x-=(0x41-10);
		}
		y|=x<<4;
		x=p8s[1];
		if((x>=0x30)&&(x<=0x39))
		{
			x-=0x30;
		}
		else
		{
			x-=(0x41-10);
		}
		y|=x;
		p8d[i]=y;
		p8s+=2;
	}
	if(y>=0x100)
	{
		y=0;
	}
	return y;
}


u32 GPRS_SMS_Tx(void)//GPRS_SMS_Tx发送;返回发送字节数
{
	u32 i;
	u32 x;
	u32 y;
	u32 TxByte;
	u8 *p8txbuff;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrlSMS;

	p8txbuff=(u8*)(ADDR_ATTx);
	UARTCtrlSMS=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(SMSPORT);
	TxByte=0;
	switch(UART1Ctrl->SubTask)//子任务
	{
		default:
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->SubTask++;//子任务
			break;
	//发送
		case 0://发SMS的AT命令AT+CMGS(PDU格式)
			if(UART1Ctrl->Task!=0)
			{
				GPRS_3763_NextATCount();//下1个AT
				return 0;
			}
//以下仅测试时主动发送用
//			UARTCtrlSMS->Task=2;
//			UARTCtrlSMS->TxByte=2;
			if((UARTCtrlSMS->Task==2)||(UARTCtrlSMS->Task==5))
			{//2=等待单帧数据发送,5=等待多帧数据发送
				UARTCtrlSMS->Task++;
				UARTCtrlSMS->TxCount=0;//已发送计数(半字对齐)
			}
			if((UARTCtrlSMS->Task==3)||(UARTCtrlSMS->Task==6))
			{//2=单帧数据正在发送,5=多帧数据正在发送
				x=UARTCtrlSMS->TxByte;//发送字节数(半字对齐)
				if(x==0)
				{//发完
					if(UARTCtrlSMS->Task==3)
					{//单帧发送完
						UARTCtrlSMS->Task=0;//0=正在接收
					}
					else
					{//多帧发送完
						UARTCtrlSMS->Task=4;//4=多帧数据续接收完成
						//SMS的帧计数+1
						UARTCtrlSMS->SMSTxSEQ++;
						UARTCtrlSMS->SMSTxSEQ&=0xfff;
					#if MainProtocol==376
						Uart_3761_RxTx(SMSPORT);//UART_3761接收发送
					#endif
					#if MainProtocol==698
						Uart_698_RxTx(SMSPORT);//UART_3761接收发送
					#endif
					}
				}
				else
				{
					TxByte=CopyString((u8 *)"AT+CMGS=",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				#if SMSEQ2Byte==1//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
					x+=2;//数据通信2BYTE接口
				#endif
					
				#if MainProtocol==376	
					y=LEN_TelephoneNumber(ADDR_AFN04F4);//电话号码有效位数,入口电话号存贮地址
				#endif
				#if MainProtocol==698
					p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//计算元素地址,使用ADDR_128K_DATABUFF
					if(p8)
					{
						y=p8[1];
						if(((p8[2]<<8)+p8[3])==(('8'<<8)+'6'))
						{
							if(y>=2)
							{
								y-=2;
							}
							else
							{
								y=0;
							}
						}
					}
					else
					{
						y=0;
					}
				#endif
					if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
					{//GSM网络支持GPRS
						if(x>140)
						{
							x=140;//短信发送最大用户数据长度
						}
						y++;//奇数号码偶数字节数
						y>>=1;//每2位1字节
						y+=1;//国际电话头1字节
						x+=y;//
						x+=8;//加PDU头字节数,总是不计SCA短信中心号码长度在内
					}
					else
					{//中兴CDMA MC8332
						if(x>140)
						{
							x=140;//短信发送最大用户数据长度
						}
						x+=y;
						x+=16;//加PDU头字节数
					}
					y=hex_bcd(x);
					TxByte+=BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8txbuff[TxByte]='\r';
					TxByte++;
					return TxByte;
				}
			}
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->SubTask=10;//子任务
			break;
		case 1://发SMS的数据
			if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			{//GSM网络支持GPRS
			//SMS-SUBMIT 发送PDU格式,每1字节的HEX均为发送2字节ASCII码
			//1BYTE SMSC地址信息的长度
			//1BYTE SMSC地址格式(TON/NPI)
			//M_BYTE SMSC地址
				//PDU数据长度从以下开始计算,以上SCA短信中心号码不计在内
			//1BYTE 基本参数(TP-MTI/VFP=11)
			//1BYTE 消息基准值(TP-MR)
			//1BYTE 目的号码位数
			//1BYTE 目的号码格式(TON/NPI)
			//M_BYTE 目的号码
			//1BYTE Protocol identifier (TP-PID) 协议
			//1BYTE Data coding scheme (TP-DCS)数据编码
			//1BYTE Validity period有效时间(TP-VP),0xa7=24小时,0x00=5分钟
			//1BYTE User Data Length (TP-UDL)用户数据长度
			// <=140BYTE TP_UD 数据

				//1 SCA短信中心号码
				//1.1 SMSC地址信息的长度(包括1字节SMSC地址格式,n字节SMSC地址)
			#if MainProtocol==376
				y=LEN_TelephoneNumber(ADDR_AFN04F4+8);//电话号码有效位数,入口电话号存贮地址
				y++;//奇数号码偶数字节数
				y>>=1;//每2位1字节
				if(y==0)
				{//没设置，自动用卡上的短信中心号码
					p8txbuff=SMS_BCD_ASCII(0x0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				}
				else
				{//设置
					x=y+2;//地址格式1字节,国际电话头1字节
					x=hex_bcd(x);
					p8txbuff=SMS_BCD_ASCII(x,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//1.2 SMSC地址格式(TON/NPI=91)
					p8txbuff=SMS_BCD_ASCII(0x91,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//1.3 SMSC地址(补’F’凑成偶数个)
						//国际区位码
					p8txbuff=SMS_BCD_ASCII(0x86,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
					p8txbuff-=2;
					i=p8txbuff[0];
					p8txbuff[0]=p8txbuff[1];
					p8txbuff[1]=i;
					p8txbuff+=2;
						//号码
					MR(ADDR_DATABUFF,ADDR_AFN04F4+8,8);
					p8=(u8 *)(ADDR_DATABUFF);
					if(p8[0]==0x86)
					{
						p8++;
					}
					for(i=0;i<y;i++)
					{
						p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
						p8txbuff-=2;
						x=p8txbuff[0];
						p8txbuff[0]=p8txbuff[1];
						p8txbuff[1]=x;
						p8txbuff+=2;
					}
				}
			#endif
			#if MainProtocol==698
				MR(ADDR_DATABUFF,ADDR_4500_4+3,17);//短信中心号码
				p8=(u8 *)(ADDR_DATABUFF);
				y=p8[0];
				p8++;
				if(((p8[0]<<8)+p8[1])==(('8'<<8)+'6'))
				{
					if(y>=2)
					{
						p8+=2;
						y-=2;
					}
					else
					{
						y=0;
					}
				}
				y++;//奇数号码偶数字节数
				y>>=1;//每2位1字节
				if(y==0)
				{//没设置，自动用卡上的短信中心号码
					p8txbuff=SMS_BCD_ASCII(0x0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				}
				else
				{//设置
					x=y+2;//地址格式1字节,国际电话头1字节
					x=hex_bcd(x);
					p8txbuff=SMS_BCD_ASCII(x,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//1.2 SMSC地址格式(TON/NPI=91)
					p8txbuff=SMS_BCD_ASCII(0x91,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//1.3 SMSC地址(补’F’凑成偶数个)
						//国际区位码
					p8txbuff=SMS_BCD_ASCII(0x86,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
					p8txbuff-=2;
					i=p8txbuff[0];
					p8txbuff[0]=p8txbuff[1];
					p8txbuff[1]=i;
					p8txbuff+=2;
						//号码
					y*=2;
					for(i=0;i<y;i++)
					{
						p8txbuff[0]=p8[0];
						p8txbuff++;
						p8++;
					}
				}
			#endif
				//2 基本参数(TP-MTI/VFP=11)
				//TP-RP=0(B7),TP-UDHI=0(B6),TP-SRR=0(B5),TP-VPF=10(B4B3),TP-RD=0(B2),TP-MTI=01(B1B0)
				p8txbuff=SMS_BCD_ASCII(0x11,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//3 消息基准值(TP-MR)
				//Message reference field (TP-MR),0-255短信计数,通常设为0
				p8txbuff=SMS_BCD_ASCII(0x0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//4 目的电话号码地址
				//Destination Address (TP-da)
				//4.1 号码位数(共?个十进制数,不包括91和’F’)
			#if MainProtocol==376
				y=LEN_TelephoneNumber(ADDR_AFN04F4);//电话号码有效位数,入口电话号存贮地址
			#endif
			#if MainProtocol==698
				p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//计算元素地址,使用ADDR_128K_DATABUFF
				if(p8)
				{
					y=p8[1];
					p8+=2;
					if(((p8[0]<<8)+p8[1])==(('8'<<8)+'6'))
					{
						if(y>=2)
						{
							p8+=2;
							y-=2;
						}
						else
						{
							y=0;
						}
					}
				}
				else
				{
					y=0;
				}
			#endif
					//加2位国际区位码
				p8txbuff=SMS_BCD_ASCII(y+2,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				y++;//奇数号码偶数字节数
				y>>=1;//每2位1字节
				//4.2 目标地址格式(TON/NPI=91)
				p8txbuff=SMS_BCD_ASCII(0x91,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//4.3 目标地址(TP-DA)(补’F’凑成偶数个)
					//国际区位码
				p8txbuff=SMS_BCD_ASCII(0x86,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				p8txbuff-=2;
				i=p8txbuff[0];
				p8txbuff[0]=p8txbuff[1];
				p8txbuff[1]=i;
				p8txbuff+=2;
					//号码
		#if MainProtocol==376
				MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
				p8=(u8 *)(ADDR_DATABUFF);
				if(p8[0]==0x86)
				{
					p8++;
				}
				for(i=0;i<y;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
					p8txbuff-=2;
					x=p8txbuff[0];
					p8txbuff[0]=p8txbuff[1];
					p8txbuff[1]=x;
					p8txbuff+=2;
				}
		#endif
		#if MainProtocol==698
				y*=2;
				for(i=0;i<y;i++)
				{
					p8txbuff[0]=p8[0];
					p8txbuff++;
					p8++;
				}
		#endif
				//5 Protocol identifier协议标识(TP-PID),00=是普通GSM类型,点到点方式
				p8txbuff=SMS_BCD_ASCII(0x0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//6 Data coding scheme (TP-DCS)数据编码;0X00GSM编码,0X04=8bit数据编码,0X08=UCS2 16BIT(UNICODE),0XC0未定义
				p8txbuff=SMS_BCD_ASCII(0x04,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//7 Validity period有效时间(TP-VP)
					//00 – 8F (VF+1)*5 分钟从 5 分钟间隔到 12 个小时
					//90 – A7 12 小时 + (VF – 143)*30 分钟
					//A8 – C4 (VP – 166) * 1 天
					//C5 – FF (VP – 192) * 1 周
				p8txbuff=SMS_BCD_ASCII(0x00,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//8 User data length用户信息长度(TP-UDL)
				x=UARTCtrlSMS->TxByte;//发送字节数(半字对齐)
			#if SMSEQ2Byte==1//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
				x+=2;//数据通信2BYTE接口
			#endif
				if(x>140)
				{
					x=140;//短信发送最大用户数据长度
				}
			#if SMSEQ2Byte==0//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
				UARTCtrlSMS->TxByte-=x;//还需发送字节数
			#else
				UARTCtrlSMS->TxByte-=(x-2);//还需发送字节数
			#endif
				if(UARTCtrlSMS->TxByte==0x0)
				{//最后条SMS标志
					UARTCtrlSMS->SMSTxSEQ|=0x8000;//短信发送接口;B0-B11发送帧计数,B12-B14帧内短信条数计数,B15帧内未条短信标志
				}
				//User data length (TP-UDL)
				p8txbuff=SMS_BCD_ASCII(x,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				//User Data (TP-UD)用户数据
			#if SMSEQ2Byte==1//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
				p8txbuff=SMS_BCD_ASCII(UARTCtrlSMS->SMSTxSEQ,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				p8txbuff=SMS_BCD_ASCII(UARTCtrlSMS->SMSTxSEQ>>8,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				x-=2;
			#endif
				p8=(u8 *)Get_ADDR_UARTnTx(SMSPORT);
				p8+=UARTCtrlSMS->TxCount;//已发送计数(半字对齐)
				UARTCtrlSMS->TxCount+=x;//下条发送指针
				for(i=0;i<x;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				}
				p8txbuff[0]=26;//<ctrl-Z> (IRA 26)
				p8txbuff++;
				TxByte=((u32)p8txbuff)-(ADDR_ATTx);
				//帧内SMS计数+1
				UARTCtrlSMS->SMSTxSEQ+=0x1000;
				UARTCtrlSMS->SMSTxSEQ&=0x7fff;
			}
			else
			{//中兴CDMA MC8332
			//<callerID_length>：1 byte，范围0-20，用来限制<caller_ID>
		#if MainProtocol==376
				y=LEN_TelephoneNumber(ADDR_AFN04F4);//电话号码有效位数,入口电话号存贮地址
				p8txbuff=SMS_BCD_ASCII(y,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<Caller_ID>： 0-20 byte，范围 +,*, #, 0-9,当长度为0时，可以不用写。
				MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
				p8=(u8 *)(ADDR_DATABUFF);
				i=0;
				if(p8[i]==0x86)
				{
					i++;
				}
				x=0;
				while(x<y)
				{
					p8txbuff=SMS_BCD_ASCII((p8[i]>>4)+0x30,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
					x++;
					if(x>=y)
					{
						break;
					}
					p8txbuff=SMS_BCD_ASCII((p8[i]&0xf)+0x30,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
					x++;
					i++;
				}
			//<Time>： 6 byte，范围Year： 0-99；Month ：1-12；Day ：1-31；Hour：0-23； Minute：0-59；Second：0-59，Day的范围是不固定的，要根据年和月确定。
				//是可以省略的，PDU模式下可以使用FFFFFFFFFFFF替换。
				MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
				Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
				for(i=0;i<6;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				}
		#endif
		#if MainProtocol==698
				p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//计算元素地址,使用ADDR_128K_DATABUFF;出口:地址指向元素的类型字节,0=错误
				if(p8)
				{
					y=p8[1];
					p8+=2;
					if(((p8[0]<<8)+p8[1])==(('8'<<8)+'6'))
					{
						if(y>=2)
						{
							p8+=2;
							y-=2;
						}
						else
						{
							y=0;
						}
					}
				}
				else
				{
					y=0;
				}
				p8txbuff=SMS_BCD_ASCII(y,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<Caller_ID>： 0-20 byte，范围 +,*, #, 0-9,当长度为0时，可以不用写。
				i=0;
				while(x<y)
				{
					p8txbuff[0]=p8[i];
					p8txbuff++;
					i++;
				}
			//<Time>： 6 byte，范围Year： 0-99；Month ：1-12；Day ：1-31；Hour：0-23； Minute：0-59；Second：0-59，Day的范围是不固定的，要根据年和月确定。
				//是可以省略的，PDU模式下可以使用FFFFFFFFFFFF替换。
				MR(ADDR_DATABUFF,ADDR_DL698YMDHMS+1,6);
				for(i=0;i<6;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				}
		#endif
			//<Lang> ：1 byte，范围0-7，可以省略，省略时用FF替换
				//0 UNSPECIFIED 
				//1 ENGLISH 
				//2 FRENCH 
				//3 SPANISH 
				//4 JAPANESE 
				//5 KOREAN 
				//6 CHINESE 
				//7 HEBREW
				p8txbuff=SMS_BCD_ASCII(1,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<Format>：1 byte，范围0, 1, 6
				//0 GSM 7-BIT 
				//1 ASCII 
				//6 UNICODE
				p8txbuff=SMS_BCD_ASCII(1,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<Prt>：1 byte，范围0-3,可以省略，省略时用FF替换
				//0 NORMAL 
				//1 INTERACTIVE 
				//2 URGENT 
				//3 EMERGENCY
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<Prv>：1 byte，范围0-3,可以省略，省略时用FF替换
				//0 NORMAL 
				//1 RESTRICTED 
				//2 CONFIDENTIAL 
				//3 SECRET
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<Ack>：1 byte，范围0-1
				//0 NEED ACK 
				//1 NO NEED ACK
				p8txbuff=SMS_BCD_ASCII(1,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<Type>：1 byte，范围0，输入模式下只能为0。其它的值都不支持。
				//0 PDU
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<Stat>：1 byte ，范围 0-3，(存储和发送短信时，该值只能设置为0或1)
				//0 MSG RECEIVED UNREAD 
				//1 MSG RECEIVED READ 
				//2 MSG STORED UNSEND 
				//3 MSG STORED SEND
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<udh_length>：1 byte ，范围0, 6, 7，用来限制<udh>
				p8txbuff=SMS_BCD_ASCII(0,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<udh>： 6-7 byte 当长度为0时，可以不用写。
				//例如：06050003120201 其中06是长度；050003为固定字段；12字段为index，收发双方通过该字段标识不同的长短信；02为长短信分包数量，01为当前短信序号.

			//<msg_length> ：1 byte ，范围0-160，<format>为0和1时，最大值是160，为6时，最大值是140
				x=UARTCtrlSMS->TxByte;//发送字节数(半字对齐)
			#if SMSEQ2Byte==1//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
				x+=2;//数据通信2BYTE接口
			#endif
				if(x>140)
				{
					x=140;//短信发送最大用户数据长度
				}
			#if SMSEQ2Byte==0//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
				UARTCtrlSMS->TxByte-=x;//还需发送字节数
			#else
				UARTCtrlSMS->TxByte-=(x-2);//还需发送字节数
			#endif
				if(UARTCtrlSMS->TxByte==0x0)
				{//最后条SMS标志
					UARTCtrlSMS->SMSTxSEQ|=0x8000;//短信发送接口;B0-B11发送帧计数,B12-B14帧内短信条数计数,B15帧内未条短信标志
				}
				p8txbuff=SMS_BCD_ASCII(x,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
			//<msg> 0-160 byte，当长度为0时，可以不用写。
			#if SMSEQ2Byte==1//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
				p8txbuff=SMS_BCD_ASCII(UARTCtrlSMS->SMSTxSEQ,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				p8txbuff=SMS_BCD_ASCII(UARTCtrlSMS->SMSTxSEQ>>8,p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				x-=2;
			#endif
				p8=(u8 *)Get_ADDR_UARTnTx(SMSPORT);
				p8+=UARTCtrlSMS->TxCount;//已发送计数(半字对齐)
				UARTCtrlSMS->TxCount+=x;//下条发送指针
				for(i=0;i<x;i++)
				{
					p8txbuff=SMS_BCD_ASCII(p8[i],p8txbuff);//短信用1字节BCD转为2字节ASCII码并存在p8指向的存贮单元,返回下个存贮指针p8
				}
				p8txbuff[0]=26;//<ctrl-Z> (IRA 26)
				p8txbuff++;
				TxByte=((u32)p8txbuff)-(ADDR_ATTx);
				//帧内SMS计数+1
				UARTCtrlSMS->SMSTxSEQ+=0x1000;
				UARTCtrlSMS->SMSTxSEQ&=0x7fff;
			}
			break;
	//接收
		case 10://读短信(已接收但未读消息)
			if(UART1Ctrl->Task!=0)
			{
				GPRS_3763_NextATCount();//下1个AT
				return 0;
			}
			if(UARTCtrlSMS->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//在线检查,每秒检查1次(GPRS模块：读信号和打开查询)
			{
				UARTCtrlSMS->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];
			#if SM_CMGF==0//短信模式:0=PDU模式,1=文本模式
				TxByte=CopyString((u8 *)"AT+CMGL=4\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			#else
				TxByte=CopyString((u8 *)"AT+CMGL=\"ALL\"\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			#endif
			}
			else
			{
				GPRS_3763_NextATCount();//下1个AT
			}
			break;
	//删除
		case 20://全部删除存储器中的已读短信
		case 30://全部删除存储器中的已读短信后硬件复位
		case 40://全部删除存储器中的已读短信后关模块
		#if SM_CMGF==0//短信模式:0=PDU模式,1=文本模式
			TxByte=CopyString((u8 *)"AT+CMGD=1,4\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
		#else
			TxByte=CopyString((u8 *)"AT+CMGD=1,4\r",p8txbuff);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
		#endif
			break;
	}
	return TxByte;
}

void GPRS_SMS_Rx(void)//GPRS_SMS_Rx接收
{
	u32 i;
	u32 x;
	u32 y;
	u32 Addr;
	u16 *p16fifo;
	u32 ReadFlag;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrlSMS;
//	u32 *p32;
#if SM_CMGF==0//短信模式:0=PDU模式,1=文本模式
	u32 LEN_UARTnIntRx;
	u8 *p8d;
	u16 *p16;
#endif
#if (USER/100)==6//用户标识:湖南
	u32 z;
#endif
	
	UARTCtrlSMS=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(SMSPORT);
	p16fifo=(u16*)(ADDR_UART1IntATRx);
	switch(UART1Ctrl->SubTask)//子任务
	{
		default:
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->SubTask=0;//子任务
			break;
	//发送
		case 0://发SMS的AT命令AT+CMGS(PDU格式)
			if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
			{//GSM网络支持GPRS
				Addr=ATRxSearch((u32)((u8*)">"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr==0)
				{
					if(Terminal_Ram->AT_MS_Timer!=0)
					{//接收没完成
						return;
					}
				}
			}
			else
			{//中兴CDMA MC8332
			}
			UART1Ctrl->SubTask++;//子任务
			UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			return;
		case 1://发SMS的数据
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->SubTask=0;//子任务
			break;
	//接收
		case 10://读短信(已接收但未读消息)
		#if SM_CMGF==0//短信模式:0=PDU模式,1=文本模式
			//PDU返回+CMGL:<index>,<stat>,[<alpha>],<length><CR><LF><pdu><CR><LF>+CMGL:....
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			ReadFlag=0;//有读标志
			while(1)
			{
				Addr=ATRxSearch((u32)((u8*)"+CMGL:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr==0)
				{//没找到SMS头'+CMGL:'
				ReadEnd:
					if(ReadFlag==0)//有读标志
					{//没
						if(UARTCtrlSMS->Task==0)
						{//没收到完整1帧
						#ifdef DL698
							Uart_698_RxTx(SMSPORT);//UART_3761接收发送
						#else
							Uart_3761_RxTx(SMSPORT);//UART_3761接收发送
						#endif
							if(UARTCtrlSMS->Task==1)
							{//收到完整1帧
							#ifdef DL698
								Uart_698_RxTx(SMSPORT);//UART_3761接收发送
							#else
								Uart_3761_RxTx(SMSPORT);//UART_3761接收发送
							#endif
								UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
								UART1Ctrl->SubTask=0;//子任务
								return;
							}
						}
						GPRS_3763_NextATCount();//下1个AT
					}
					else
					{//有
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
						UART1Ctrl->SubTask=20;//全部删除存储器中的已读短信
					}
					break;
				}
				//有正确SMS头
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				ReadFlag=1;//有读标志
				//找<pdu>地址,前有<CR><LF>
				Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr==0)
				{//没找到<pdu>地址
					goto ReadEnd;
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				
				if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
				{//GSM网络支持GPRS
				//SMS-DELIVER 接收PDU格式,每1字节的HEX均为接收2字节ASCII码
				//SCA
				//1BYTE SCA长度
				//1BYTE SCA号码类型
				//N_BYTE SCA号码
				//TPDU
				//1BYTE TP-MTI and more friends
				//1BYTE 发送源号码位数
				//1BYTE 发送源号码类型
				//M_BYTE 发送源号码
				//1BYTE Protocol identifier (TP-PID) 协议
				//1BYTE Data coding scheme (TP-DCS)数据编码
				//7BYTE The service centre time stamp (TP-SCTS)发送时间
				//年低年高
				//月低月高
				//日低日高
				//时低时高
				//分低分高
				//秒低秒高
				//时区
				//1BYTE User Data Length (TP-UDL)用户数据长度
				// <=140BYTE TP_UD 数据
					//取SCA字节长度
					LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
					i=SMS_ASCII_BCD(ADDR_DATABUFF,1);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
					i<<=1;//每字节2个ASCII码
					Addr+=2+i;
					Addr+=2;//1BYTE TP-MTI and more friends
					//取发送源号码位数
					LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
					x=SMS_ASCII_BCD(ADDR_DATABUFF,1);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
					Addr+=2+2;//1BYTE号码位数,1BYTE号码类型,指向号码地址
					if((x<2)||(x>16))
					{//号码长度错,最小2位(当前11位),最大16位
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
						continue;
					}
					//号码比较(主站手机号码)
			#if MainProtocol==376
					MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
					p8=(u8 *)(ADDR_DATABUFF);
					if(p8[0]==0x86)
					{
						MR(ADDR_DATABUFF,ADDR_AFN04F4+1,7);///国际字头86不比较
					}
					//设置BCD号码转为SMS格式的ASCII码
					for(i=0;i<8;i++)
					{
						y=p8[i]&0xf;
						if(y<10)
						{
							p8[8+(i*2)]=y+0x30;
						}
						else
						{
							p8[8+(i*2)]=y+55;
						}
						y=(p8[i]>>4)&0xf;
						if(y<10)
						{
							p8[8+(i*2)+1]=y+0x30;
						}
						else
						{
							p8[8+(i*2)+1]=y+55;
						}
					}
			#else
				p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//计算元素地址,使用ADDR_128K_DATABUFF
				if(p8)
				{
					i=p8[1];//电话号码有效位数
					p8+=2;
					if(((p8[0]<<8)+p8[1])!=(('8'<<8)+'6'))
					{
						MR(ADDR_DATABUFF,(u32)p8,16);
					}
					else
					{
						MR(ADDR_DATABUFF,(u32)p8+2,16);
					}
				}
				else
				{
					MC(0,ADDR_DATABUFF,16);
				}
				p8=(u8 *)(ADDR_DATABUFF);
			#endif	
					LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
					i=SMS_ASCII_BCD(ADDR_DATABUFF,1);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
					i=((i>>4)&0xf)+((i&0xf)<<4);//高低位交换
					if(i==0x86)
					{//国除字头86不比较
						Addr+=2;
						x-=2;
					}
					LineATIntRx(Addr,ADDR_DATABUFF+8+16,16);//取AT中断接收数据线性放在缓冲中
					if((x&1)&&(x!=0))
					{//奇数位,最后1位,取后1字节,因2字节ASCII组成BCD关系
						p8[8+(x-1)]=p8[8+x];
						p8[8+16+(x-1)]=p8[8+16+x];
					}
					for(i=0;i<x;i++)
					{
						if(p8[8+i]!=p8[8+16+i])
						{
							break;
						}
					}
					if(i!=x)
					{//号码不相同
					#if MainProtocol==376
						i=LEN_TelephoneNumber(ADDR_AFN04F4);//电话号码有效位数,入口电话号存贮地址
						if(i!=0)
						{//主站号码已设置
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
							continue;
						}
					#else
						p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//计算元素地址,使用ADDR_128K_DATABUFF
						if(p8)
						{
							i=p8[1];//电话号码有效位数
							p8+=2;
							if(((p8[0]<<8)+p8[1])==(('8'<<8)+'6'))
							{
								if(i>2)
								{
									i-=2;
								}
								else
								{
									i=0;
								}
							}
						}
						else
						{
							i=0;
						}
						if(i!=0)
						{//主站号码已设置
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
							continue;
						}
					#endif
					}
					x++;//奇数号码时偶数字节
					x>>=1;
					x<<=1;//号码位数=ASCII字节数
					Addr+=x;
					Addr+=(2*9);
					//取PDU长度=i
					LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
					i=SMS_ASCII_BCD(ADDR_DATABUFF,1);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
					Addr+=2;
					LineATIntRx(Addr,ADDR_DATABUFF,2*i);//取AT中断接收数据线性放在缓冲中
					SMS_ASCII_BCD(ADDR_DATABUFF,i);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
					p8=(u8 *)(ADDR_DATABUFF);
					p8[i]=0;//数据结束加0
					Addr+=(2*i);
				}
				else
				{//中兴CDMA MC8332
				//<callerID_length>：1 byte，范围0-20，用来限制<caller_ID>
					LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
					x=SMS_ASCII_BCD(ADDR_DATABUFF,1);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
					Addr+=2+2;//长度ASCII的2字节+'"'ASCII的2字节
				//<Caller_ID>： 0-20 byte，范围 +,*, #, 0-9,当长度为0时，可以不用写。
			#if MainProtocol==376
					i=LEN_TelephoneNumber(ADDR_AFN04F4);//电话号码有效位数,入口电话号存贮地址
					if(i!=0)
					{
						//号码比较(主站手机号码)
						MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
						p8=(u8 *)(ADDR_DATABUFF);
						//设置BCD号码转为SMS格式的ASCII码
						for(i=0;i<8;i++)
						{
							y=(p8[i]>>4)&0xf;
							if(y<10)
							{
								p8[8+(i*2)]=y+0x30;
							}
							else
							{
								p8[8+(i*2)]=y+55;
							}
							y=p8[i]&0xf;
							if(y<10)
							{
								p8[8+(i*2)+1]=y+0x30;
							}
							else
							{
								p8[8+(i*2)+1]=y+55;
							}
						}
						LineATIntRx(Addr,ADDR_DATABUFF+32,2*x);//取AT中断接收数据线性放在缓冲中
						SMS_ASCII_BCD(ADDR_DATABUFF+32,x);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
						i=LEN_TelephoneNumber(ADDR_AFN04F4);//电话号码有效位数,入口电话号存贮地址
						i=WinDataComp(ADDR_DATABUFF+8,ADDR_DATABUFF+32,i,x);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
						if(i==0)
						{//主站号码已设置
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
							continue;
						}
					}
				#else
					p8=Get_Element((u8*)ADDR_4500_4,2,1,LENmax_4500_4);//计算元素地址,使用ADDR_128K_DATABUFF
					if(p8)
					{
						i=p8[1];//电话号码有效位数
						p8+=2;
						if(((p8[0]<<8)+p8[1])!=(('8'<<8)+'6'))
						{
							MR(ADDR_DATABUFF,(u32)p8,16);
						}
						else
						{
							MR(ADDR_DATABUFF,(u32)p8+2,16);
							if(i>2)
							{
								i-=2;
							}
							else
							{
								i=0;
							}
						}
						LineATIntRx(Addr,ADDR_DATABUFF+32,2*x);//取AT中断接收数据线性放在缓冲中
						SMS_ASCII_BCD(ADDR_DATABUFF+32,x);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
						i=WinDataComp(ADDR_DATABUFF+8,ADDR_DATABUFF+32,i,x);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
						if(i==0)
						{//主站号码已设置
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
							continue;
						}
					}
				#endif
					Addr+=(2*x)+2;//号ASCII的2*x字节+'"'ASCII的2字节
				//<Time>： 6 byte，范围Year： 0-99；Month ：1-12；Day ：1-31；Hour：0-23； Minute：0-59；Second：0-59，Day的范围是不固定的，要根据年和月确定。
					//是可以省略的，PDU模式下可以使用FFFFFFFFFFFF替换。
				//<Lang> ：1 byte，范围0-7，可以省略，省略时用FF替换
					//0 UNSPECIFIED 
					//1 ENGLISH 
					//2 FRENCH 
					//3 SPANISH 
					//4 JAPANESE 
					//5 KOREAN 
					//6 CHINESE 
					//7 HEBREW
				//<Format>：1 byte，范围0, 1, 6
					//0 GSM 7-BIT 
					//1 ASCII 
					//6 UNICODE
				//<Prt>：1 byte，范围0-3,可以省略，省略时用FF替换
					//0 NORMAL 
					//1 INTERACTIVE 
					//2 URGENT 
					//3 EMERGENCY
				//<Prv>：1 byte，范围0-3,可以省略，省略时用FF替换
					//0 NORMAL 
					//1 RESTRICTED 
					//2 CONFIDENTIAL 
					//3 SECRET
				//<Ack>：1 byte，范围0-1
					//0 NEED ACK 
					//1 NO NEED ACK
				//<Type>：1 byte，范围0，输入模式下只能为0。其它的值都不支持。
					//0 PDU
				//<Stat>：1 byte ，范围 0-3，(存储和发送短信时，该值只能设置为0或1)
					//0 MSG RECEIVED UNREAD 
					//1 MSG RECEIVED READ 
					//2 MSG STORED UNSEND 
					//3 MSG STORED SEND
				//<udh_length>：1 byte ，范围0, 6, 7，用来限制<udh>
				//<udh>： 6-7 byte 当长度为0时，可以不用写。
					//例如：06050003120201 其中06是长度；050003为固定字段；12字段为index，收发双方通过该字段标识不同的长短信；02为长短信分包数量，01为当前短信序号.
					Addr+=(2*14);
				//<msg_length> ：1 byte ，范围0-160，<format>为0和1时，最大值是160，为6时，最大值是140
					LineATIntRx(Addr,ADDR_DATABUFF,2);//取AT中断接收数据线性放在缓冲中
					i=SMS_ASCII_BCD(ADDR_DATABUFF,1);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
					Addr+=2;
					LineATIntRx(Addr,ADDR_DATABUFF,2*i);//取AT中断接收数据线性放在缓冲中
					SMS_ASCII_BCD(ADDR_DATABUFF,i);//短信用2字节ASCII转为1字节BCD码,并存在存贮单元
					p8=(u8 *)(ADDR_DATABUFF);
					p8[i]=0;//数据结束加0
					Addr+=(2*i);
				}
				
			#if SMSEQ2Byte==1//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
				if(i<=2)
				{//短信传输接口有2字节
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
					continue;
				}
				x=p8[0]+(p8[1]<<8);
				UARTCtrlSMS->SMSRxSEQ=x;
				UARTCtrlSMS->SMSTxSEQ=x&0xfff;
				MR(ADDR_DATABUFF,ADDR_DATABUFF+2,i);
				i-=2;
			#endif	
				p16=(u16*)Get_ADDR_UARTn(SMSPORT);//得到UART缓冲首地址
				y=p16[0];
				p8d=(u8*)(Get_ADDR_UARTn(SMSPORT)+10);//得到UART缓冲首地址
				LEN_UARTnIntRx=Get_LEN_UARTnIntRx(SMSPORT)-10;//得到UART中断接收缓冲长度
				for(x=0;x<i;x++)
				{
					while(y>=LEN_UARTnIntRx)
					{
						y-=LEN_UARTnIntRx;
					}
					p8d[y]=p8[x];
					y++;
				}
				p16[0]=y;
				if(UARTCtrlSMS->Task==0)
				{//没收到完整1帧
				#ifdef DL698
					Uart_698_RxTx(SMSPORT);//UART_3761接收发送
				#else
					Uart_3761_RxTx(SMSPORT);//UART_3761接收发送
				#endif
				}
				if(UARTCtrlSMS->Task==1)
				{//收到完整1帧,放弃后继短信免短信SEQ重写
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->SubTask=20;//全部删除存储器中的已读短信
					break;
				}
			}
			if(UARTCtrlSMS->Task==1)
			{//收到完整1帧
			#ifdef DL698
				Uart_698_RxTx(SMSPORT);//UART_3761接收发送
			#else
				Uart_3761_RxTx(SMSPORT);//UART_3761接收发送
			#endif
			}
		#endif//#if SM_CMGF==0//短信模式:0=PDU模式,1=文本模式
		#if SM_CMGF==1//短信模式:0=PDU模式,1=文本模式
			//文本模式返回+CMGL :<index>,<stat>,<da/oa>,<lang>,<encod><CR><LF><data>(接收/发送的短信)
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			ReadFlag=0;//有读标志:D0位=有读标志,D1位=复位,D2位=关模块
			while(1)
			{
				Addr=ATRxSearch((u32)((u8*)"+CMGL:"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr==0)
				{//没找到SMS头'+CMGL:'
					if(ReadFlag==0)//有读标志:D0位=有读标志,D1位=复位,D2位=关模块
					{//没
						GPRS_3763_NextATCount();//下1个AT
					}
					else
					{//有
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
						UART1Ctrl->SubTask=20;//删除短信
						if(ReadFlag&4)//有读标志:D0位=有读标志,D1位=复位,D2位=关模块
						{
							UART1Ctrl->SubTask=40;//全部删除存储器中的已读短信后关模块
						}
						if(ReadFlag&2)//有读标志:D0位=有读标志,D1位=复位,D2位=关模块
						{
							UART1Ctrl->SubTask=30;//子任务;全部删除存储器中的已读短信后硬件复位
						}
					}
					break;
				}
				//有正确SMS头
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				ReadFlag|=1;//有读标志:D0位=有读标志,D1位=复位,D2位=关模块
				
				//找<da/oa>地址,前有<index>,<stat>,<da/oa>
				Addr=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr==0)
				{//没找到
					continue;
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				Addr=ATRxSearch((u32)((u8*)","));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr==0)
				{//没找到
					continue;
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				
				x=LEN_TelephoneNumber(ADDR_AFN04F4);//电话号码有效位数,入口电话号存贮地址
				if(x!=0)
				{
					//号码比较(主站手机号码)
					MR(ADDR_DATABUFF,ADDR_AFN04F4,8);
					p8=(u8 *)(ADDR_DATABUFF);
					//设置BCD号码转为SMS格式的ASCII码
					for(i=0;i<8;i++)
					{
						y=(p8[i]>>4)&0xf;
						if(y<10)
						{
							p8[8+(i*2)]=y+0x30;
						}
						else
						{
							p8[8+(i*2)]=y+55;
						}
						y=p8[i]&0xf;
						if(y<10)
						{
							p8[8+(i*2)+1]=y+0x30;
						}
						else
						{
							p8[8+(i*2)+1]=y+55;
						}
					}
					p8[8+x]=0;//比较字符串结束
					x=ATRxSearch(ADDR_DATABUFF+8);//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
					if(x==0)
					{//没找到
						continue;
					}
				}
				//找<Data>开始地址,前有<CR><LF>
				Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr==0)
				{//没找到
					continue;
				}
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				//找<Data>结束地址,后有<CR><LF>
				i=ATRxSearch((u32)((u8*)"\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(i==0)
				{//没找到
					continue;
				}
				p16fifo[1]=i-(ADDR_UART1IntATRx+10);//修改已读指针
				while(i<Addr)
				{
					i+=LEN_UART1IntATRx-10;
				}
				i-=Addr;//文本长度
				if(i<2)
				{
					continue;
				}
				i-=2;
				if(i>LEN_DATABUFF)
				{
					i=LEN_DATABUFF;
				}
				LineATIntRx(Addr,ADDR_DATABUFF,i);//取AT中断接收数据线性放在缓冲中
				p8=(u8 *)(ADDR_DATABUFF);
				p8[i]=0;//文本结束加0
				
		#if (USER/100)==6//用户标识:湖南
				//远程硬件初始化功能，即终端收到指定短信命令后自动硬件初始化
				x=WinDataComp((u32)((u8*)"cssdl 0"),ADDR_DATABUFF,7,i);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
				if(x!=0)
				{
					ReadFlag|=2;//有读标志:D0位=有读标志,D1位=复位,D2位=关模块
				}
				//GPRS远程主站切换功能，即终端收到指定短信命令后，更改原终端的通讯参数，并重新连接主站，完成主站切换的功能
				x=WinDataComp((u32)((u8*)"cssdl 1 "),ADDR_DATABUFF,8,i);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
				if(x!=0)
				{
					y=IP_ASCII_HEX(x+7);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
					if(y!=0)
					{
						MWR(y,ADDR_DATABUFF+2048,4);//写ip
						y=i-((x-ADDR_DATABUFF)+8);//剩余没找字节数
						x=WinDataComp((u32)((u8*)":"),x+8+7,1,y);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
						if(x!=0)
						{
							y=ASCII_HEX(x+1);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
							MWR(y,ADDR_DATABUFF+2048+4,2);//写端口
							MW(ADDR_DATABUFF+2048,ADDR_DATABUFF+2048+6,6);//备用
							y=i-((x-ADDR_DATABUFF)+1);//剩余没找字节数
							x=WinDataComp((u32)((u8*)":"),x+1,1,y);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
							if(x!=0)
							{
								y=i-((x-ADDR_DATABUFF));//剩余没找字节数,多1个最后加了0
								//y=WinDataComp((u32)((u8*)"\x00"),x+1,1,y);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
								p8=(u8 *)(x+1);
								for(z=0;z<y;z++)
								{
									if(p8[z]<0x21)
									{//找到
										if(z<=16)
										{
											x++;
											MC(0,ADDR_DATABUFF+2048+12,16);//不够16字节补0
											MW(x,ADDR_DATABUFF+2048+12,z);//写APN
											MW(ADDR_DATABUFF+2048,ADDR_AFN04F3,28);//写入AFN04F3参数
											ReadFlag|=4;//有读标志:D0位=有读标志,D1位=复位,D2位=关模块
											break;
										}
									}
								}
							}
						}
					}
				}
				//CDMA远程主站切换功能，即终端收到指定短信命令后，更改原终端的CDMA通讯参数，并重新连接主站
				x=WinDataComp((u32)((u8*)"cssdl 2 "),ADDR_DATABUFF,8,i);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
				if(x!=0)
				{
					y=IP_ASCII_HEX(x+7);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
					if(y!=0)
					{
						MWR(y,ADDR_DATABUFF+2048,4);//写ip
						y=i-((x-ADDR_DATABUFF)+8);//剩余没找字节数
						x=WinDataComp((u32)((u8*)":"),x+8+7,1,y);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
						if(x!=0)
						{
							y=ASCII_HEX(x+1);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
							MWR(y,ADDR_DATABUFF+2048+4,2);//写端口
							MW(ADDR_DATABUFF+2048,ADDR_DATABUFF+2048+6,6);//备用
							y=i-((x-ADDR_DATABUFF)+1);//剩余没找字节数
							x=WinDataComp((u32)((u8*)":"),x+1,1,y);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
							if(x!=0)
							{
								y=i-((x-ADDR_DATABUFF)+1);//剩余没找字节数
								y=WinDataComp((u32)((u8*)":"),x+1,1,y);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
								if(y!=0)
								{
									x++;
									MC(0,ADDR_DATABUFF+2048+32,32);//不够字节补0
									MW(x,ADDR_DATABUFF+2048+32,y-x);
									x=y;
									y=i-((x-ADDR_DATABUFF));//剩余没找字节数,多1个最后加了0
									//y=WinDataComp((u32)((u8*)"\x00"),x+1,1,y);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
									p8=(u8 *)(x+1);
									for(z=0;z<y;z++)
									{
										if(p8[z]<0x30)
										{//找到
											if(z<=32)
											{
												x++;
												MC(0,ADDR_DATABUFF+2048+64,32);//不够字节补0
												MW(x,ADDR_DATABUFF+2048+64,z);
												MW(ADDR_DATABUFF+2048,ADDR_AFN04F3,12);//写入AFN04F3参数
												MW(ADDR_DATABUFF+2048+32,ADDR_AFN04F16,64);//写入AFN04F16参数
			
												ReadFlag|=4;//有读标志:D0位=有读标志,D1位=复位,D2位=关模块
												break;
											}
										}
									}
								}
							}
						}
					}
				}
		#endif//#if (USER/100)==6//用户标识:湖南
		
			}
		#endif//#if SM_CMGF==1//短信模式:0=PDU模式,1=文本模式
			break;
	//删除
		case 20://全部删除存储器中的已读短信
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			if(UARTCtrlSMS->Task!=0)
			{//收到完整1帧
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
			}
			else
			{
				GPRS_3763_NextATCount();//下1个AT
			}
			break;
		case 30://全部删除存储器中的已读短信后硬件复位
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收

//			p32=(u32*)(ADDR_HourCongeal_AutoRW);
//			while(p32[3]!=0)
//			{//R2长度,值=0表示操作完成
//				HourCongeal_AutoRW();//小时冻结自动IO读写或清0,完成后结果处理
//				WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗
//			}
//			p32=(u32*)(ADDR_Class2Congeal_AutoRW);
//			while(p32[3]!=0)
//			{//R2长度,值=0表示操作完成
//				Class2Congeal_AutoRW();//类2冻结自动IO读写或清0,完成后结果处理
//				WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗
//			}

			SoftDelayMS(2000);//软件ms延时
			WWDT_RESET();//立即使能看门狗复位
			return;
		case 40://全部删除存储器中的已读短信后关模块
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			UART1Ctrl->ATCount=AT_MYPOWEROFF;//关机
			break;
	}
	Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
	if(Addr!=0)
	{
		p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
	}
}


	
