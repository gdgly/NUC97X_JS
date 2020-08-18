
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_PROXY.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/TASK.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"

#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../Calculate/Calculate.h"
#include "../Device/IC_TESAM.h"


//PROXY-Request∷=CHOICE
//{
//代理读取若干个服务器的若干个对象属性请求             [1] ProxyGetRequestList，
//代理读取一个服务器的一个记录型对象属性请求           [2] ProxyGetRequestRecord，
//代理设置若干个服务器的若干个对象属性请求             [3] ProxySetRequestList，
//代理设置后读取若干个服务器的若干个对象属性请求       [4] ProxySetThenGetRequestList，
//代理操作若干个服务器的若干个对象方法请求             [5] ProxyActionRequestList，
//代理操作后读取若干个服务器的若干个对象方法和属性请求 [6] ProxyActionThenGetRequestList，
//代理透明转发命令请求                                 [7] ProxyTransCommandRequest
//}
//PROXY-Response∷=CHOICE
//{
//代理读取若干个服务器的若干个对象属性响应             [1] ProxyGetResponseList，
//代理读取一个服务器的一个记录型对象属性响应           [2] ProxyGetResponseRecord，
//代理设置若干个服务器的若干个对象属性响应             [3] ProxySetResponseList，
//代理设置后读取若干个服务器的若干个对象属性响应       [4] ProxySetThenGetResponseList，
//代理操作若干个服务器的若干个对象方法响应             [5] ProxyActionResponseList，
//代理操作后读取若干个服务器的若干个对象方法和属性响应 [6] ProxyActionThenGetResponseList，
//代理透明转发命令响应                                 [7] ProxyTransCommandResponse
//}


#define NUMmax_ReTx     2//最大代理重发次数

void PROXY_Request(u32 PORTn)
{
	u32 i;
	u32 x;
	u32 LEN_SA;//服务器地址SA的长度
	u32 TI;//时间标签:0=无,1=有
	u32 LEN_RxAPDU;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//最大还可发送的字节数
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	ms_Type *ms;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息
	
	u32 offset;
	u32 NUM_TSA;
	u32 NUM_OAD;
	u32 SETno;
	u32 ProxyPORTn;
	UARTCtrl_TypeDef * UARTCtrlProxy;
	u8* p8Data;
	u8* p8;
	u16 *p16;
	u32 NUM_ReTx;
	u8* p8OAD;
	u32 Err;
	u32 LEN_RSD;
	u32 LEN_RCSD;
	u32 CA;//客户机地址CA
#ifndef LINUX
	u32 *p32;
#endif

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
#else
	//客户机接收缓冲长度
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//不为0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//除起始字符和结束字符之外的帧字节数

	CA=p8rx[11];//客户机地址CA
	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	if(UARTCtrl->pTxData)//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
	{
		ms=Get_ms();
		ms->ms_free(UARTCtrl->pTxData);
		UARTCtrl->pTxData=0;
	}
	UARTCtrl->PIID=p8rx[LEN_SA+8+2];//PIID
	switch(p8rx[LEN_SA+8+1])
	{
		case 1://代理读取若干个服务器的若干个对象属性请求             [1] ProxyGetRequestList，
//ProxyGetRequestList∷=SEQUENCE
//{
//  服务序号-优先级          PIID，
//  整个代理请求的超时时间   long-unsigned，
//  代理若干个服务器的对象属性读取  SEQUENCE OF
//  {
//    一个目标服务器地址         TSA，
//    代理一个服务器的超时时间   long-unsigned，
//    若干个对象属性描述符       SEQUENCE OF OAD
//  }
//}
//68 45 00 43 05 01 00 00 00 00 00 10 44 E1 
//09 01 07 
//00 64 整个代理请求的超时时间   long-unsigned
//01 代理若干个服务器的对象属性读取  SEQUENCE OF
//07 05 22 22 22 22 22 22 一个目标服务器地址         TSA
//00 00 代理一个服务器的超时时间   long-unsigned
//09 若干个对象属性描述符       SEQUENCE OF OAD
//00 10 02 00 OAD1
//00 20 02 00 OAD2
//00 00 02 00 OAD3
//00 30 02 01 OAD4
//00 40 02 01 OAD5
//00 50 02 00 OAD6
//00 60 02 00 OAD7
//00 70 02 00 OAD8
//00 80 02 00 OAD9
//00 92 C5 16 
//ProxyGetResponseList∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  代理若干个服务器的读取结果  SEQUENCE OF
//  {
//		一个目标服务器地址  TSA，
//		若干个对象属性描述符及其结果  SEQUENCE OF
//		{
//      对象属性描述符  OAD，
//      及其读取结果    Get-Result
//		}
//  }
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=137;//代理响应        	[137]	PROXY-Response
			p8tx[1]=1;//[1] ProxyGetResponseList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//整个代理请求的超时时间
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//得到UART秒定时器地址
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//读取服务器个数
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// 一个目标服务器地址         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA字节数
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
				Err=1;//置错误
				if(SETno<NUM_RMmax)
				{//找到配置序号
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//配置有效
						p8=Get_Element(p8+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
						if(p8)
						{//找到端口号
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OAD值转为通信口号
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if ((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//口功能 0=终端维护口,!=0终端抄表口
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//自动搜表任务:0=空,1=等待,2...过程
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//发起代理的端口号
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
										if(p8)
										{//波特率
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://自适应
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA字节数
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//发送缓冲不够
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//代理一个服务器的超时时间
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//代理1个服务器的超时时间秒
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//当前需发送OAD个数及数值指针
				NUM_OAD=p8rx[offset];
				offset+=1;
			//组发送帧
		#if ((USER%100)==0)||((USER/100)==9)//电科院,河南
		//明文读取
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//起始字符（68H）  1Byte
				p8Data[0]=0x68;
			//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
			//控制域C  1Byte
				p8Data[3]=0x43;
			//地址域A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//客户机地址CA  1Byte
				p8Data[11]=CA;
			//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
			//APDU
				p8Data[14]=5;//GET-Request
				p8Data[15]=2;//读取若干个对象属性请求           [2] GetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				i=4*NUM_OAD;
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//时间标签
				p8Data[i]=0;
			//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
			//结束字符（16H）
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		#else
		//明文+数据验证码读取
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//起始字符（68H）  1Byte
				p8Data[0]=0x68;
			//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
			//控制域C  1Byte
				p8Data[3]=0x43;
			//地址域A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//客户机地址CA  1Byte
				p8Data[11]=CA;
			//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
				p8Data[14]=0x10;
				p8Data[15]=0;//明文应用数据单元 [0] 密文应用数据单元 [1]
				//APDU长度;octet-string
			//APDU
				p8Data[14+3]=5;//GET-Request
				p8Data[15+3]=2;//读取若干个对象属性请求           [2] GetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16+3]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17+3]=NUM_OAD;
				i=4*NUM_OAD;
				MR((u32)p8Data+18+3,(u32)p8rx+offset,i);
				offset+=i;
				i+=18+3;
			//时间标签
				p8Data[i]=0;
				i++;
				p8Data[16]=i-17;//APDU长度;octet-string
			//随机数
				p8Data[i]=1;//随机数 [1]  RN
				p8Data[i+1]=16;//长度
				i+=2;
				//16byte随机数
				x=TESAM_CMD_TxRx(0x80040010,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(x)
				{
					UART7Ctrl->Task=0;//关电源重启
				}
				MR((u32)p8Data+i,ADDR_UART7Rx+5,16);
				MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//抄表安全模式时发送给电表的16字节随机数
				i+=16;
			//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
			//结束字符（16H）
				i+=3;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		#endif

				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//交采口
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
						if(UARTCtrlProxy->Task==2)//等待单帧数据发送
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//其他口
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						NUM_ReTx=0;//代理重发次数
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
						while(1)
						{
					#ifndef LINUX
						p32=(u32*)ADDR_MS_ENTRY;		
						(*(void(*)())p32[115])();//Main_Thread; 115://主线程
					#else
						ms=(ms_Type *)Get_ms();
						ms->Main_Thread();
					#endif
							
							if(UARTCtrlProxy->ProxyTask==0)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//无数据接收
									if(NUM_ReTx<NUMmax_ReTx)//代理重发次数
									{
										NUM_ReTx++;//代理重发次数
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
										continue;
									}
								}
								break;
							}
						}
					}
				//代理返回数据
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//无数据接收
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(6*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD+(i*4),4);
							p8tx[LEN_TxAPDU+4]=0;
							p8tx[LEN_TxAPDU+5]=33;//请求超时 
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
					}
					else
					{
						p8Data-=4;//前有p8Data+=4;
					#if ((USER%100)==0)||((USER/100)==9)//电科院,河南
						//明文读取
					#else
						//明文+数据验证码读取
						i=p8Data[1]+(p8Data[2]<<8);
						if(i>=(14+3))
						{
							i-=(14+3);
						}
						if((p8Data[16]&0x80)==0)
						{
							MR((u32)p8Data+14,(u32)p8Data+14+3,i);
						}
						else
						{
							if((p8Data[16]&0x7f)==1)
							{//1byte长度
								MR((u32)p8Data+14,(u32)p8Data+14+4,i);
							}
							else
							{//2byte长度
								MR((u32)p8Data+14,(u32)p8Data+14+5,i);
							}
						}
					#endif
						NUM_OAD=p8Data[17];//返回OAD个数
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
							if(p8Data[i+4]==0)
							{//0=返回错误
								if(LENmax_TxSPACE<6)
								{
									LEN_TxAPDU=0;//发送缓冲不够
									return;
								}
								MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,6);
								i+=6;
								LEN_TxAPDU+=6;
								LENmax_TxSPACE-=6;
							}
							else
							{//1=返回data
								x=Get_DL698DataLen_S(p8Data+i+5,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
								x+=5;
								if(LENmax_TxSPACE<x)
								{
									LEN_TxAPDU=0;//发送缓冲不够
									return;
								}
								MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
								i+=x;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
						}
					}
				}
				else
				{//Err=1
					NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
					if(LENmax_TxSPACE<(1+(6*NUM_OAD)))
					{
						LEN_TxAPDU=0;//发送缓冲不够
						break;
					}
					p8tx[LEN_TxAPDU]=NUM_OAD;
					LEN_TxAPDU+=1;
					LENmax_TxSPACE-=1;
					p8OAD++;
					for(i=0;i<NUM_OAD;i++)
					{
						MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD+(i*4),4);
						p8tx[LEN_TxAPDU+4]=0;
						p8tx[LEN_TxAPDU+5]=255;//其他错误
						LEN_TxAPDU+=6;
						LENmax_TxSPACE-=6;
					}
				}
				p8tx[3]++;//服务器个数+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 2://代理读取一个服务器的一个记录型对象属性请求           [2] ProxyGetRequestRecord，
//ProxyGetRequestRecord∷=SEQUENCE
//{
//  服务序号-优先级     PIID，
//  代理请求的超时时间  long-unsigned，
//  目标服务器地址      TSA，
//  对象属性描述符      OAD，
//  记录选择描述符      RSD，
//  记录列选择描述符    RCSD
//}
//68 2E 00 43 05 04 00 00 00 00 00 10 71 7C 09 02 0B 
//00 1E  代理请求的超时时间  long-unsigned
//07 05 00 00 00 00 00 01  TSA
//50 04 02 00  OAD
//09 01  RSD
//02 00 00 10 02 00 00 00 10 02 00  RCSD
//00 31 07 16 
//ProxyGetResponseRecord∷=SEQUENCE
//{
//  服务序号-优先级-ACD          PIID-ACD，
//  目标服务器地址               TSA，
//  一个记录型对象属性及其结果   A-ResultRecord
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=137;//代理响应        	[137]	PROXY-Response
			p8tx[1]=2;//[2] ProxyGetRequestRecord
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//整个代理请求的超时时间
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//得到UART秒定时器地址
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			// 一个目标服务器地址         TSA
			p16=(u16*)(ADDR_DATABUFF);
			p16[0]=0;
			p16[1]=0;
			p16[2]=0;
			i=p8rx[offset+1];//TSA字节数
			i++;
			MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
			UARTCtrl->RMAddr[0]=p16[0];
			UARTCtrl->RMAddr[1]=p16[1];
			UARTCtrl->RMAddr[2]=p16[2];
			
			SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
			Err=1;//置错误
			if(SETno<NUM_RMmax)
			{//找到配置序号
				p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
				if(p8[0]==DataType_structure)
				{//配置有效
					p8=Get_Element(p8+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//找到端口号
						ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
						ProxyPORTn=OADtoPORTn(ProxyPORTn);//OAD值转为通信口号
						if(ProxyPORTn==ACSAMPLEPORT)
						{
							UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
							Err=0;
						}
					#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
						if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
					#endif
					#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
						if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
					#endif
					#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
						if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
					#endif
					#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
						if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
					#endif
						{
							UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
							if(UARTCtrlProxy->Function)//口功能 0=终端维护口,!=0终端抄表口
							{
								if(UARTCtrlProxy->AutoSearchTask==0)//自动搜表任务:0=空,1=等待,2...过程
								{
									UARTCtrlProxy->ProxyPORTn=PORTn;//发起代理的端口号
									p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
									p8=Get_Element(p8+5,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
									if(p8)
									{//波特率
										i=p8[1];
										switch(i)
										{
											case 0://300
											case 1://600
											case 2://1200
											case 3://2400
											case 4://4800
											case 5://7200
											case 6://9600
											case 7://19200
											case 8://38400
											case 9://57600
											case 10://115200
												UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
												break;
											case 255://自适应
											default:
												UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
												break;
										}
										UARTCtrlProxy->ProxyRMprotocol=p8[3];//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
									}
									Err=0;
								}
							}
						}
					}
				}
			}
			i=p8rx[offset];//TSA字节数
			i++;
			if(LENmax_TxSPACE<i)
			{
				LEN_TxAPDU=0;//发送缓冲不够
				break;
			}
			MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
			offset+=i;
			LEN_TxAPDU+=i;
			LENmax_TxSPACE-=i;
			//代理一个服务器的超时时间
			if(Err==0)
			{
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//得到UART秒定时器地址
				i=p16[0];
				UARTCtrlProxy->ProxyOverTime=i;//代理1个服务器的超时时间秒
			}
			//OAD
			p8OAD=p8rx+offset;//当前需发送OAD个数及数值指针
		//组发送帧
		#if ((USER%100)==0)||((USER/100)==9)//电科院,河南
		//明文读取
			p8Data[0]=0xfe;
			p8Data[1]=0xfe;
			p8Data[2]=0xfe;
			p8Data[3]=0xfe;
			p8Data+=4;
		//起始字符（68H）  1Byte
			p8Data[0]=0x68;
		//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
		//控制域C  1Byte
			p8Data[3]=0x43;
		//地址域A
			p8Data[4]=5;
			p8Data[5]=UARTCtrl->RMAddr[2]>>8;
			p8Data[6]=UARTCtrl->RMAddr[2];
			p8Data[7]=UARTCtrl->RMAddr[1]>>8;
			p8Data[8]=UARTCtrl->RMAddr[1];
			p8Data[9]=UARTCtrl->RMAddr[0]>>8;
			p8Data[10]=UARTCtrl->RMAddr[0];
		//客户机地址CA  1Byte
			p8Data[11]=CA;
		//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
		//APDU
			p8Data[14]=5;//GET-Request
			p8Data[15]=3;//读取一个记录型对象属性请求       [3] GetRequestRecord
			UARTCtrl->PIID++;
			p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
			i=DataLen_RSD_NoMS(p8rx+offset+4,0,0);//内联的MS长度不计
			i>>=8;
			i--;//无数据类型
			LEN_RSD=i;
			LEN_RCSD=p8rx[offset+4+i];//RCSD个数
			LEN_RCSD*=5;
			LEN_RCSD+=1;
			i=4+LEN_RSD+LEN_RCSD;
			MR((u32)p8Data+17,(u32)p8rx+offset,i);
			offset+=i;
			i+=17;
		//时间标签
			p8Data[i]=0;
		//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
		//结束字符（16H）
			i+=4;
			i-=2;
			p8Data[1]=i;
			p8Data[2]=i>>8;
			Uart_698_HCSFCS(p8Data);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		#else
		//明文+数据验证码读取
			p8Data[0]=0xfe;
			p8Data[1]=0xfe;
			p8Data[2]=0xfe;
			p8Data[3]=0xfe;
			p8Data+=4;
		//起始字符（68H）  1Byte
			p8Data[0]=0x68;
		//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
		//控制域C  1Byte
			p8Data[3]=0x43;
		//地址域A
			p8Data[4]=5;
			p8Data[5]=UARTCtrl->RMAddr[2]>>8;
			p8Data[6]=UARTCtrl->RMAddr[2];
			p8Data[7]=UARTCtrl->RMAddr[1]>>8;
			p8Data[8]=UARTCtrl->RMAddr[1];
			p8Data[9]=UARTCtrl->RMAddr[0]>>8;
			p8Data[10]=UARTCtrl->RMAddr[0];
		//客户机地址CA  1Byte
			p8Data[11]=CA;
		//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
			p8Data[14]=0x10;
			p8Data[15]=0;//明文应用数据单元 [0] 密文应用数据单元 [1]
			//APDU长度;octet-string
		//APDU
			p8Data[14+3]=5;//GET-Request
			p8Data[15+3]=3;//读取一个记录型对象属性请求       [3] GetRequestRecord
			UARTCtrl->PIID++;
			p8Data[16+3]=UARTCtrl->PIID&0x3f;//PIID
			i=DataLen_RSD_NoMS(p8rx+offset+4,0,0);//内联的MS长度不计
			i>>=8;
			i--;//无数据类型
			LEN_RSD=i;
			LEN_RCSD=p8rx[offset+4+i];//RCSD个数
			LEN_RCSD*=5;
			LEN_RCSD+=1;
			i=4+LEN_RSD+LEN_RCSD;
			MR((u32)p8Data+17+3,(u32)p8rx+offset,i);
			offset+=i;
			i+=17+3;
		//时间标签
			p8Data[i]=0;
			i++;
			p8Data[16]=i-17;//APDU长度;octet-string
		//随机数
			p8Data[i]=1;//随机数 [1]  RN
			p8Data[i+1]=16;//长度
			i+=2;
			//16byte随机数
			x=TESAM_CMD_TxRx(0x80040010,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(x)
			{
				UART7Ctrl->Task=0;//关电源重启
			}
			MR((u32)p8Data+i,ADDR_UART7Rx+5,16);
			MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//抄表安全模式时发送给电表的16字节随机数
			i+=16;
		//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
		//结束字符（16H）
			i+=3;
			i-=2;
			p8Data[1]=i;
			p8Data[2]=i>>8;
			Uart_698_HCSFCS(p8Data);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		#endif
		
			if(Err==0)
			{
				if(ProxyPORTn==ACSAMPLEPORT)
				{//交采口
					x=Get_ADDR_UARTnRx(ProxyPORTn);
					MW((u32)p8Data,x,i+2);
					Uart_698_Data(ProxyPORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
					if(UARTCtrlProxy->Task==2)//等待单帧数据发送
					{
						UARTCtrlProxy->Task=0;
						x=UARTCtrlProxy->TxByte;
						UARTCtrlProxy->TxByte=0;
						UARTCtrlProxy->ProxyTxRxByte=x;
						MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
					}
					else
					{
						UARTCtrlProxy->ProxyTxRxByte=0;
					}
				}
				else
				{//其他口
					UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
					UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
					NUM_ReTx=0;//代理重发次数
					p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
					while(1)
					{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
						
						if(UARTCtrlProxy->ProxyTask==0)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						{
							if(UARTCtrlProxy->ProxyTxRxByte==0)
							{//无数据接收
								if(NUM_ReTx<NUMmax_ReTx)//代理重发次数
								{
									NUM_ReTx++;//代理重发次数
									UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
									UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
									continue;
								}
							}
							break;
						}
					}
				}
			//代理返回数据
				if(UARTCtrlProxy->ProxyTxRxByte==0)
				{//无数据接收
					if(LENmax_TxSPACE<(4+LEN_RCSD+2))
					{
						LEN_TxAPDU=0;//发送缓冲不够
						break;
					}
					MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);//OAD
					MR((u32)p8tx+LEN_TxAPDU+4,(u32)p8OAD+4+LEN_RSD,LEN_RCSD);//RCSD
					LEN_TxAPDU+=4+LEN_RCSD;
					p8tx[LEN_TxAPDU+0]=0;
					p8tx[LEN_TxAPDU+1]=33;//请求超时 
					LEN_TxAPDU+=2;
					LENmax_TxSPACE-=4+LEN_RCSD+2;
				}
				else
				{
					p8Data-=4;//前有p8Data+=4;
				#if ((USER%100)==0)||((USER/100)==9)//电科院,河南
					//明文读取
				#else
					//明文+数据验证码读取
					i=p8Data[1]+(p8Data[2]<<8);
					if(i>=(14+3))
					{
						i-=(14+3);
					}
					if((p8Data[16]&0x80)==0)
					{
						MR((u32)p8Data+14,(u32)p8Data+14+3,i);
					}
					else
					{
						if((p8Data[16]&0x7f)==1)
						{//1byte长度
							MR((u32)p8Data+14,(u32)p8Data+14+4,i);
						}
						else
						{//2byte长度
							MR((u32)p8Data+14,(u32)p8Data+14+5,i);
						}
					}
				#endif
					i=17;
					NUM_OAD=p8Data[17+4];//RCSD个数
					LEN_RCSD=5*NUM_OAD;
					LEN_RCSD+=1;
					if(p8Data[17+4+LEN_RCSD]==0)
					{//0=返回错误
						if(LENmax_TxSPACE<(4+LEN_RCSD+2))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							return;
						}
						MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+17,4+LEN_RCSD+2);
						LEN_TxAPDU+=4+LEN_RCSD+2;
						LENmax_TxSPACE-=4+LEN_RCSD+2;
					}
					else
					{//1=返回data
						x=p8Data[17+4+LEN_RCSD+1];//M条记录
						LEN_RCSD+=4+2;
						while(x--)
						{
							for(i=0;i<NUM_OAD;i++)//RCSD个数
							{
								offset=Get_DL698DataLen_S(p8Data+17+LEN_RCSD,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
								LEN_RCSD+=offset;
							}
						}
						if(LENmax_TxSPACE<LEN_RCSD)
						{
							LEN_TxAPDU=0;//发送缓冲不够
							return;
						}
						MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+17,LEN_RCSD);
						LEN_TxAPDU+=LEN_RCSD;
						LENmax_TxSPACE-=LEN_RCSD;
					}
				}
			}
			else
			{//Err=1
					if(LENmax_TxSPACE<(4+LEN_RCSD+2))
					{
						LEN_TxAPDU=0;//发送缓冲不够
						break;
					}
					MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);//OAD
					MR((u32)p8tx+LEN_TxAPDU+4,(u32)p8OAD+4+LEN_RSD,LEN_RCSD);//RCSD
					LEN_TxAPDU+=4+LEN_RCSD;
					p8tx[LEN_TxAPDU+0]=0;
					p8tx[LEN_TxAPDU+1]=255;//其他错误
					LEN_TxAPDU+=2;
					LENmax_TxSPACE-=4+LEN_RCSD+2;
			}
			break;
		case 3://代理设置若干个服务器的若干个对象属性请求             [3] ProxySetRequestList，
//ProxySetRequestList∷=SEQUENCE
//{
//  服务序号-优先级         PIID，
//  整个代理请求的超时时间  long-unsigned，
//  代理若干个服务器的对象属性设置  SEQUENCE OF
//  {
//    一个目标服务器地址        TSA，
//    代理一个服务器的超时时间  long-unsigned，
//    若干个对象属性描述符及其数据  SEQUENCE OF
//    {
//      对象属性描述符  OAD，
//      及其数据        Data
//    }
//  }
//}		
//68 2D 00 43 05 04 00 00 00 00 00 10 C2 82 09 03 0A 
//00 1E  整个代理请求的超时时间  long-unsigned
//01   代理若干个服务器的对象属性设置  SEQUENCE OF
//07 05 00 00 00 00 00 01  一个目标服务器地址        TSA
//00 00  代理一个服务器的超时时间  long-unsigned
//01  若干个对象属性描述符及其数据  SEQUENCE OF
//40 00 02 00 对象属性描述符  OAD
//1C 07 E1 07 02 0D 22 02  及其数据        Data
//00 E9 6B 16 
//ProxySetResponseList∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  代理若干个服务器的读取结果  SEQUENCE OF
//  {
//一个目标服务器地址  TSA，
//若干个对象属性描述符及其结果  SEQUENCE OF
//{
//      对象属性描述符  OAD，
//      及其设置结果    DAR
//}
//  }
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=137;//代理响应        	[137]	PROXY-Response
			p8tx[1]=3;// [3] ProxySetRequestList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//整个代理请求的超时时间
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//得到UART秒定时器地址
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//服务器个数
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// 一个目标服务器地址         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA字节数
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
				Err=1;//置错误
				if(SETno<NUM_RMmax)
				{//找到配置序号
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//配置有效
						p8=Get_Element(p8+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
						if(p8)
						{//找到端口号
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OAD值转为通信口号
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//口功能 0=终端维护口,!=0终端抄表口
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//自动搜表任务:0=空,1=等待,2...过程
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//发起代理的端口号
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
										if(p8)
										{//波特率
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://自适应
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA字节数
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//发送缓冲不够
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//代理一个服务器的超时时间
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//代理1个服务器的超时时间秒
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//当前需发送OAD个数及数值指针
				NUM_OAD=p8rx[offset];
				offset+=1;
			//组发送帧
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//起始字符（68H）  1Byte
				p8Data[0]=0x68;
			//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
			//控制域C  1Byte
				p8Data[3]=0x43;
			//地址域A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//客户机地址CA  1Byte
				p8Data[11]=CA;
			//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
			//APDU
				p8Data[14]=6;//设置请求 			 [6]		SET-Request
				p8Data[15]=2;//设置若干个对象属性请求     	[2] SetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				
				i=0;
				for(x=0;x<NUM_OAD;x++)
				{
					LEN_RSD=Get_DL698DataLen_S(p8rx+offset+i+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
					i+=4+LEN_RSD;
				}
				if(i>(LEN_UART2Tx-(4+18+4)))
				{
					LEN_TxAPDU=0;
					break;
				}
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//时间标签
				p8Data[i]=0;
			//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
			//结束字符（16H）
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
					
				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//交采口
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
						if(UARTCtrlProxy->Task==2)//等待单帧数据发送
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//其他口
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						NUM_ReTx=0;//代理重发次数
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
						while(1)
						{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
							if(UARTCtrlProxy->ProxyTask==0)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//无数据接收
									if(NUM_ReTx<NUMmax_ReTx)//代理重发次数
									{
										NUM_ReTx++;//代理重发次数
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
										continue;
									}
								}
								break;
							}
						}
					}
				//代理返回数据
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//无数据接收
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(5*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=33;//请求超时 
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//得到DL698数据类型长度(包括:类型字节和数据)
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
						}
					}
					else
					{
						p8Data-=4;//前有p8Data+=4;
						NUM_OAD=p8Data[17];//返回OAD个数
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
							if(LENmax_TxSPACE<5)
							{
								LEN_TxAPDU=0;//发送缓冲不够
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,5);//4字节OAD+DAR
							i+=5;
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
						}
					}
				}
				else
				{//Err=1
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(5*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=255;//其他错误
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//得到DL698数据类型长度(包括:类型字节和数据)
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
						}
				}
				p8tx[3]++;//服务器个数+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 4://代理设置后读取若干个服务器的若干个对象属性请求       [4] ProxySetThenGetRequestList，
//ProxySetThenGetRequestList∷=SEQUENCE
//{
//  服务序号-优先级          PIID，
//  整个代理请求的超时时间   long-unsigned，
//  代理若干个服务器的对象属性设置后读取  SEQUENCE OF
//  {
//    一个目标服务器地址          TSA，
//    代理一个服务器的超时时间    long-unsigned，
//    若干个对象属性的设置后读取  SEQUENCE OF
//   {
//      设置的对象属性描述符   OAD，
//      及其设置数值           Data，
//      读取的对象属性描述符   OAD，
//      及其延时读取时间       unsigned
//    }
//  }
//}
//68 32 00 43 05 04 00 00 00 00 00 10 CB EF 09 04 0B 
//00 1E 整个代理请求的超时时间   long-unsigned
//01  代理若干个服务器的对象属性设置后读取  SEQUENCE OF
//07 05 00 00 00 00 00 01 一个目标服务器地址          TSA
//00 00  代理一个服务器的超时时间    long-unsigned
//01  若干个对象属性的设置后读取  SEQUENCE OF
//40 00 02 00  设置的对象属性描述符   OAD
//1C 07 E1 07 02 0E 32 1A 及其设置数值           Data
//40 00 02 00  读取的对象属性描述符   OAD
//00  及其延时读取时间       unsigned
//00 98 CC 16 
//ProxySetThenGetResponseList∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  代理若干个服务器的设置后读取结果  SEQUENCE OF
//  {
//一个目标服务器地址  TSA，
//若干个对象属性设置后读取结果  SEQUENCE OF
//{
//      设置的对象属性描述符  OAD，
//      及其设置结果          DAR，
//一个对象属性及其结果  A-ResultNormal 
//}
//  }
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=137;//代理响应        	[137]	PROXY-Response
			p8tx[1]=4;// [4] ProxySetThenGetRequestList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//整个代理请求的超时时间
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//得到UART秒定时器地址
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//服务器个数
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// 一个目标服务器地址         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA字节数
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
				Err=1;//置错误
				if(SETno<NUM_RMmax)
				{//找到配置序号
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//配置有效
						p8=Get_Element(p8+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
						if(p8)
						{//找到端口号
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OAD值转为通信口号
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//口功能 0=终端维护口,!=0终端抄表口
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//自动搜表任务:0=空,1=等待,2...过程
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//发起代理的端口号
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
										if(p8)
										{//波特率
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://自适应
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA字节数
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//发送缓冲不够
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//代理一个服务器的超时时间
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//代理1个服务器的超时时间秒
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//当前需发送OAD个数及数值指针
				NUM_OAD=p8rx[offset];
				offset+=1;
			//组发送帧
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//起始字符（68H）  1Byte
				p8Data[0]=0x68;
			//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
			//控制域C  1Byte
				p8Data[3]=0x43;
			//地址域A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//客户机地址CA  1Byte
				p8Data[11]=CA;
			//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
			//APDU
				p8Data[14]=6;//设置请求 			 [6]		SET-Request
				p8Data[15]=3;//[3] SetThenGetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				
				i=0;
				for(x=0;x<NUM_OAD;x++)
				{
// 若干个设置后读取对象属性  SEQUENCE OF
// {
//		一个设置的对象属性   OAD，
//		数据                 Data，
//		一个读取的对象属性   OAD，
//		延时读取时间         unsigned
// }
					LEN_RSD=Get_DL698DataLen_S(p8rx+offset+i+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
					i+=4+LEN_RSD+5;
				}
				if(i>(LEN_UART2Tx-(4+18+4)))
				{
					LEN_TxAPDU=0;
					break;
				}
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//时间标签
				p8Data[i]=0;
			//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
			//结束字符（16H）
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
					
				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//交采口
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
						if(UARTCtrlProxy->Task==2)//等待单帧数据发送
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//其他口
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						NUM_ReTx=0;//代理重发次数
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
						while(1)
						{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
							if(UARTCtrlProxy->ProxyTask==0)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//无数据接收
									if(NUM_ReTx<NUMmax_ReTx)//代理重发次数
									{
										NUM_ReTx++;//代理重发次数
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
										continue;
									}
								}
								break;
							}
						}
					}
				//代理返回数据
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//无数据接收
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(11*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=33;//请求超时 
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//得到DL698数据类型长度(包括:类型字节和数据)
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=0;//0=错误
							p8tx[LEN_TxAPDU+5]=33;//请求超时 
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
					}
					else
					{//有数据接收
						p8Data-=4;//前有p8Data+=4;
						NUM_OAD=p8Data[17];//返回OAD个数
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
//      设置的对象属性描述符  OAD，
//      及其设置结果          DAR，
							if(LENmax_TxSPACE<5)
							{
								LEN_TxAPDU=0;//发送缓冲不够
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,5);//4字节OAD+DAR
							i+=5;
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
//一个对象属性及其结果  A-ResultNormal 
							if(p8Data[i+4]==0)
							{//返回错误
								x=6;//4字节OAD+2字节DAR
							}
							else
							{//返回数据
								x=Get_DL698DataLen_S(p8Data+i+4+1,0);//得到DL698数据类型长度(包括:类型字节和数据)
								x+=4+1;
							}
							if(LENmax_TxSPACE<x)
							{
								LEN_TxAPDU=0;//发送缓冲不够
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
							i+=x;
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
						}
					}
				}
				else
				{//Err=1
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(11*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=255;//其他错误
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//得到DL698数据类型长度(包括:类型字节和数据)
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=0;//0=错误
							p8tx[LEN_TxAPDU+5]=255;//其他错误
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
				}
				p8tx[3]++;//服务器个数+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 5://代理操作若干个服务器的若干个对象方法请求             [5] ProxyActionRequestList，
//ProxyActionRequestList∷=SEQUENCE
//{
//  服务序号-优先级         PIID，
//  整个代理请求的超时时间  long-unsigned，
//  代理若干个服务器的对象方法操作  SEQUENCE OF
//  {
//    一个目标服务器地址            TSA，
//    代理一个服务器的超时时间      long-unsigned，
//    若干个对象方法描述符及其参数  SEQUENCE OF
//    {
//      对象方法描述符  OMD，
//      及其方法参数    Data
//    }
//  }
//}
//68 26 00 43 05 04 00 00 00 00 00 10 DB C0 09 05 0B 
//00 1E 整个代理请求的超时时间  long-unsigned
//01 代理若干个服务器的对象方法操作  SEQUENCE OF
//07 05 00 00 00 00 00 01 一个目标服务器地址            TSA
//00 00 代理一个服务器的超时时间      long-unsigned
//01 若干个对象方法描述符及其参数  SEQUENCE OF
//F3 00 04 00 对象方法描述符  OMD
//00 及其方法参数    Data
//00 6D FB 16 
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=137;//代理响应        	[137]	PROXY-Response
			p8tx[1]=5;// [5] ProxyActionRequestList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//整个代理请求的超时时间
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//得到UART秒定时器地址
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//服务器个数
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// 一个目标服务器地址         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA字节数
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
				Err=1;//置错误
				if(SETno<NUM_RMmax)
				{//找到配置序号
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//配置有效
						p8=Get_Element(p8+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
						if(p8)
						{//找到端口号
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OAD值转为通信口号
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//口功能 0=终端维护口,!=0终端抄表口
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//自动搜表任务:0=空,1=等待,2...过程
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//发起代理的端口号
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
										if(p8)
										{//波特率
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://自适应
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA字节数
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//发送缓冲不够
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//代理一个服务器的超时时间
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//代理1个服务器的超时时间秒
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//当前需发送OAD个数及数值指针
				NUM_OAD=p8rx[offset];
				offset+=1;
			//组发送帧
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//起始字符（68H）  1Byte
				p8Data[0]=0x68;
			//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
			//控制域C  1Byte
				p8Data[3]=0x43;
			//地址域A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//客户机地址CA  1Byte
				p8Data[11]=CA;
			//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
			//APDU
				p8Data[14]=7;//操作请求 			 [7]		ACTION-Request
				p8Data[15]=2;//操作若干个对象方法请求                  	     [2] ActionRequestList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				
				i=0;
				for(x=0;x<NUM_OAD;x++)
				{
					LEN_RSD=Get_DL698DataLen_S(p8rx+offset+i+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
					i+=4+LEN_RSD;
				}
				if(i>(LEN_UART2Tx-(4+18+4)))
				{
					LEN_TxAPDU=0;
					break;
				}
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//时间标签
				p8Data[i]=0;
			//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
			//结束字符（16H）
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
					
				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//交采口
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
						if(UARTCtrlProxy->Task==2)//等待单帧数据发送
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//其他口
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						NUM_ReTx=0;//代理重发次数
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
						while(1)
						{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
							if(UARTCtrlProxy->ProxyTask==0)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//无数据接收
									if(NUM_ReTx<NUMmax_ReTx)//代理重发次数
									{
										NUM_ReTx++;//代理重发次数
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
										continue;
									}
								}
								break;
							}
						}
					}
				//代理返回数据
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//无数据接收
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(6*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=33;//请求超时 
							p8tx[LEN_TxAPDU+5]=0;//操作返回数据Data  OPTIONAL
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//得到DL698数据类型长度(包括:类型字节和数据)
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
					}
					else
					{
						p8Data-=4;//前有p8Data+=4;
						NUM_OAD=p8Data[17];//返回OAD个数
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
							if(LENmax_TxSPACE<5)
							{
								LEN_TxAPDU=0;//发送缓冲不够
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,5);//4字节OAD+DAR
							i+=5;
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
							//操作返回数据Data  OPTIONAL
//							if(p8Data[i]==0)
//							{
//								x=1;
//							}
//							else
//							{
//								x=Get_DL698DataLen_S(p8Data+i+1,0);//得到DL698数据类型长度(包括:类型字节和数据)
//								x+=1;
//							}
							x=Get_DL698DataLen_S(p8Data+i,0);//得到DL698数据类型长度(包括:类型字节和数据)
							
							
							if(LENmax_TxSPACE<x)
							{
								LEN_TxAPDU=0;//发送缓冲不够
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
							i+=x;
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
						}
					}
				}
				else
				{//Err=1
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(6*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=255;//其他错误
							p8tx[LEN_TxAPDU+5]=0;//操作返回数据Data  OPTIONAL
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//得到DL698数据类型长度(包括:类型字节和数据)
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
				}
				p8tx[3]++;//服务器个数+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 6://代理操作后读取若干个服务器的若干个对象方法和属性请求 [6] ProxyActionThenGetRequestList，
//ProxyActionThenGetRequestList∷=SEQUENCE
//{
//  服务序号-优先级               PIID，
//  整个代理请求的超时时间        long-unsigned，
//  代理若干个服务器的操作后读取  SEQUENCE OF
//  {
//    一个目标服务器地址          TSA，
//    代理一个服务器的超时时间    long-unsigned，
//    若干个对象方法及属性的操作后读取  SEQUENCE OF
//    {
//      操作的对象方法描述符   OMD，
//      及其方法参数           Data，
//      读取的对象属性描述符   OAD，
//      及其延时读取时间       unsigned
//    }
//  }
//}
//68 2B 00 43 05 04 00 00 00 00 00 10 B5 77 09 06 0B 
//00 1E 整个代理请求的超时时间        long-unsigned
//01 代理若干个服务器的操作后读取  SEQUENCE OF
//07 05 00 00 00 00 00 01 一个目标服务器地址          TSA
//00 00 代理一个服务器的超时时间    long-unsigned
//01 若干个对象方法及属性的操作后读取  SEQUENCE OF
//F3 00 04 00 操作的对象方法描述符   OMD
//00 及其方法参数           Data
//40 00 02 00 读取的对象属性描述符   OAD
//3C 及其延时读取时间       unsigned
//00 F3 77 16 
//ActionThenGetResponseNormalList∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  操作若干个对象方法后读取属性的结果  SEQUENCE OF
//  {
//一个设置的对象方法描述符   OMD，
//     操作执行结果               DAR，
//     操作返回数据               Data OPTIONAL，
//一个对象属性及其结果        A-ResultNormal 
//}
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=137;//代理响应        	[137]	PROXY-Response
			p8tx[1]=6;//[6] ProxyActionThenGetRequestList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//整个代理请求的超时时间
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//得到UART秒定时器地址
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//服务器个数
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// 一个目标服务器地址         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA字节数
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
				Err=1;//置错误
				if(SETno<NUM_RMmax)
				{//找到配置序号
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//配置有效
						p8=Get_Element(p8+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
						if(p8)
						{//找到端口号
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OAD值转为通信口号
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//口功能 0=终端维护口,!=0终端抄表口
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//自动搜表任务:0=空,1=等待,2...过程
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//发起代理的端口号
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
										if(p8)
										{//波特率
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://自适应
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA字节数
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//发送缓冲不够
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//代理一个服务器的超时时间
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//代理1个服务器的超时时间秒
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//当前需发送OAD个数及数值指针
				NUM_OAD=p8rx[offset];
				offset+=1;
			//组发送帧
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//起始字符（68H）  1Byte
				p8Data[0]=0x68;
			//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
			//控制域C  1Byte
				p8Data[3]=0x43;
			//地址域A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//客户机地址CA  1Byte
				p8Data[11]=CA;
			//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
			//APDU
				p8Data[14]=7;//操作请求 			 [7]		ACTION-Request
				p8Data[15]=3;//[3] ActionThenGetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				
				i=0;
				for(x=0;x<NUM_OAD;x++)
				{
//      操作的对象方法描述符   OMD，
//      及其方法参数           Data，
//      读取的对象属性描述符   OAD，
//      及其延时读取时间       unsigned
					LEN_RSD=Get_DL698DataLen_S(p8rx+offset+i+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
					i+=4+LEN_RSD+5;
				}
				if(i>(LEN_UART2Tx-(4+18+4)))
				{
					LEN_TxAPDU=0;
					break;
				}
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//时间标签
				p8Data[i]=0;
			//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
			//结束字符（16H）
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
					
				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//交采口
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
						if(UARTCtrlProxy->Task==2)//等待单帧数据发送
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//其他口
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						NUM_ReTx=0;//代理重发次数
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
						while(1)
						{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
							if(UARTCtrlProxy->ProxyTask==0)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//无数据接收
									if(NUM_ReTx<NUMmax_ReTx)//代理重发次数
									{
										NUM_ReTx++;//代理重发次数
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
										continue;
									}
								}
								break;
							}
						}
					}
				//代理返回数据
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//无数据接收
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(12*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=33;//请求超时 
							p8tx[LEN_TxAPDU+5]=0;//操作返回数据          Data  OPTIONAL
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//得到DL698数据类型长度(包括:类型字节和数据)
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=0;//0=错误
							p8tx[LEN_TxAPDU+5]=33;//请求超时 
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
					}
					else
					{
						p8Data-=4;//前有p8Data+=4;
						NUM_OAD=p8Data[17];//返回OAD个数
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
//  一个对象方法描述符    OMD，
//  操作执行结果          DAR，
							if(LENmax_TxSPACE<5)
							{
								LEN_TxAPDU=0;//发送缓冲不够
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,5);//4字节OAD+DAR
							i+=5;
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
//  操作返回数据          Data  OPTIONAL
//							if(p8Data[i]==0)
//							{
//								x=1;
//							}
//							else
//							{
//								x=Get_DL698DataLen_S(p8Data+i+1,0);//得到DL698数据类型长度(包括:类型字节和数据)
//								x+=1;
//							}
							x=Get_DL698DataLen_S(p8Data+i,0);//得到DL698数据类型长度(包括:类型字节和数据)
							
							if(LENmax_TxSPACE<x)
							{
								LEN_TxAPDU=0;//发送缓冲不够
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
							i+=x;
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
//一个对象属性及其结果  A-ResultNormal 
							if(p8Data[i+4]==0)
							{//返回错误
								x=6;//4字节OAD+2字节DAR
							}
							else
							{//返回数据
								x=Get_DL698DataLen_S(p8Data+i+4+1,0);//得到DL698数据类型长度(包括:类型字节和数据)
								x+=4+1;
							}
							if(LENmax_TxSPACE<x)
							{
								LEN_TxAPDU=0;//发送缓冲不够
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
							i+=x;
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
						}
					}
				}
				else
				{//Err=1
						NUM_OAD=p8OAD[0];//当前需发送OAD个数及数值指针
						if(LENmax_TxSPACE<(1+(12*NUM_OAD)))
						{
							LEN_TxAPDU=0;//发送缓冲不够
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=255;//其他错误
							p8tx[LEN_TxAPDU+5]=0;//操作返回数据          Data  OPTIONAL
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//得到DL698数据类型长度(包括:类型字节和数据)
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=0;//0=错误
							p8tx[LEN_TxAPDU+5]=255;//其他错误
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
				}
				p8tx[3]++;//服务器个数+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//整个代理请求的超时时间
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 7://代理透明转发命令请求                                 [7] ProxyTransCommandRequest
//ProxyTransCommandRequest∷=SEQUENCE
//{
//  服务序号-优先级               PIID，
//  数据转发端口                  OAD，
//  端口通信控制块                COMDCB，
//  接收等待报文超时时间（秒）   long-unsigned，
//  接收等待字节超时时间（毫秒） long-unsigned，
//  透明转发命令                 octet-string
//}
//68 3A 00 43 05 01 00 00 00 00 00 10 C2 A3 
//09 07 08 
//F2 01 02 01 数据转发端口                  OAD
//COMDCB∷=SEQUENCE
//{
//	波特率    ENUMERATED
//	{
//		300bps（0），   600bps（1），     1200bps（2），
//		2400bps（3），  4800bps（4），    7200bps（5），
//		9600bps（6），  19200bps（7），   38400bps（8），
//		57600bps（9）， 115200bps（10）， 自适应（255）
//	}，
//	校验位  ENUMERATED {无校验（0），奇校验（1），偶校验（2）}，
//	数据位  ENUMERATED {5（5），6（6），7（7），8（8）}，
//	停止位  ENUMERATED {1（1），2（2）}，
//	流控    ENUMERATED {无(0)，硬件(1)，软件(2)}
//}
//03 COMDCB_波特率
//02 COMDCB_校验位
//08 COMDCB_数据位
//01 COMDCB_停止位
//00 COMDCB_流控
//00 0A 接收等待报文超时时间（秒）   long-unsigned
//00 0A 接收等待字节超时时间（毫秒） long-unsigned
//19 68 17 00 43 05 22 22 22 22 22 22 10 C1 D6 05 01 07 00 10 02 00 00 04 01 16  透明转发命令 octet-string
//00 C4 75 16 

//ProxyTransCommandResponse∷=SEQUENCE
//{
//  服务序号-优先级-ACD      PIID-ACD，  
//  数据转发端口             OAD，
//  透明转发命令返回结果     TransResult
//}
//TransResult∷=CHOICE
//{
//  错误信息      [0] DAR，
//  返回数据      [1] octet-string
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=137;//代理响应        	[137]	PROXY-Response
			p8tx[1]=7;//[7] ProxyTransCommandResponse
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//数据转发端口                  OAD，
			ProxyPORTn=(p8rx[offset]<<24)+(p8rx[offset+1]<<16)+(p8rx[offset+2]<<8)+p8rx[offset+3];//端口OAD
			MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,4);
			offset+=4;
			LEN_TxAPDU+=4;
			LENmax_TxSPACE-=4;
			ProxyPORTn=OADtoPORTn(ProxyPORTn);//OAD值转为通信口号
		#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if((ProxyPORTn!=ACSAMPLEPORT)&&(ProxyPORTn!=RS485_1PORT)&&(ProxyPORTn!=RS485_2PORT)&&(ProxyPORTn!=RS485_4PORT))
		#endif
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if((ProxyPORTn!=ACSAMPLEPORT)&&(ProxyPORTn!=RS485_1PORT)&&(ProxyPORTn!=RS485_2PORT))
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if((ProxyPORTn!=ACSAMPLEPORT)&&(ProxyPORTn!=RS485_1PORT)&&(ProxyPORTn!=RS485_2PORT)&&(ProxyPORTn!=RS485_4PORT))
		#endif
		#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
		#endif
			{
				p8tx[LEN_TxAPDU]=0;//错误信息      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//其它
				LEN_TxAPDU+=2;
				break;
			}
			UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
			if(UARTCtrlProxy->Function==0)//口功能 0=终端维护口,!=0终端抄表口
			{
				p8tx[LEN_TxAPDU]=0;//错误信息      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//其它
				LEN_TxAPDU+=2;
				break;
			}
			if(UARTCtrlProxy->AutoSearchTask)//自动搜表任务:0=空,1=等待,2...过程
			{
				p8tx[LEN_TxAPDU]=0;//错误信息      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//其它
				LEN_TxAPDU+=2;
				break;
			}
			UARTCtrlProxy->ProxyPORTn=PORTn;//发起代理的端口号
			//D8-D5位表示波特率,O～7依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			//波特率
			i=p8rx[offset];
			switch(i)
			{
				case 0://300
				case 1://600
				case 2://1200
				case 3://2400
				case 4://4800
				case 5://7200
				case 6://9600
				case 7://19200
				case 8://38400
				case 9://57600
				case 10://115200
					x=(0xb+(i<<5));
					break;
				case 255://自适应
				default:
					x=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
					break;
			}
			x&=0xffe0;
			//D3位表示有无校验位0/1分别表示无/有
			//D2位表示偶／奇校验位0/1分别表示偶／奇校验
			//	校验位  ENUMERATED {无校验（0），奇校验（1），偶校验（2）}，
			i=p8rx[offset+1];
			switch(i)
			{
				case 0://无校验
					break;
				case 1://奇校验
					x|=0x0c;
					break;
				default://偶校验
					x|=0x08;
					break;
			}
			//D1-D0 数据位数0～3分别表示5-8位
			//	数据位  ENUMERATED {5（5），6（6），7（7），8（8）}，
			i=p8rx[offset+2];
			i-=5;
			i&=0x03;
			x|=i;
			//D4位表示停止位位数0/1分别表示1/2个停止位
			//	停止位  ENUMERATED {1（1），2（2）}，
			i=p8rx[offset+3];
			if(i==2)
			{
				x|=0x10;
			}
			//	流控    ENUMERATED {无(0)，硬件(1)，软件(2)}
			//仅电科院一致性测试时需要判别
			if(p8rx[offset+4]>2)
			{
				p8tx[LEN_TxAPDU]=0;//错误信息      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//其它
				LEN_TxAPDU+=2;
				break;
			}
			offset+=5;
			UARTCtrlProxy->ProxyBpsCtrl=x;
			UARTCtrlProxy->ProxyRMprotocol=0;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
			//接收等待报文超时时间（秒）   long-unsigned
			//接收等待字节超时时间（毫秒） long-unsigned
			offset+=4;
			//透明转发命令 octet-string
			i=p8rx[offset];
			if((i&0x80)==0)
			{
				offset+=1;
			}
			else
			{
				i&=0x7f;
				if(i==1)
				{
					i=p8rx[offset+1];
					offset+=2;
				}
				else
				{
					i=(p8rx[offset+1]<<8)|p8rx[offset+2];
					offset+=3;
				}
			}
			//if(i>LEN_UART2FnDataBuff)
			if(i>LEN_UART2Tx)
			{
				p8tx[LEN_TxAPDU]=0;//错误信息      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//其它
				LEN_TxAPDU+=2;
				break;
			}
			//目的地址
			UARTCtrl->RMAddr[0]=0;//显示地址时用,载波口时目的地址用
			UARTCtrl->RMAddr[1]=0;//显示地址时用,载波口时目的地址用
			UARTCtrl->RMAddr[2]=0;//显示地址时用,载波口时目的地址用
			//查开始的0xfe个数
			for(x=0;x<i;x++)
			{
				if(p8rx[offset+x]!=0xfe)
				{
					break;
				}
			}
			TI=FrameProtocol(p8rx+offset+x,i-x);//通信协议自动识别:返回:0=未知,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
			UARTCtrl->RMprotocol=TI;//通信协议自动识别:返回:0=未知,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
			switch(TI)
			{
				case 1://DL/T645-1997
				case 2://DL/T645-2007
					MR((u32)&UARTCtrl->RMAddr,(u32)p8rx+offset+x+1,6);
					Data_Inverse((u32)&UARTCtrl->RMAddr,6);//数据倒序(高低字节调换),入口Le>=2
				#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(ProxyPORTn==RS485_4PORT)
					{
						if((UARTCtrl->RMAddr[0]==0x9999)&&(UARTCtrl->RMAddr[1]==0x9999)&&(UARTCtrl->RMAddr[2]==0x9999))
						{//广播校时
							if(p8rx[offset+x+8]==8)
							{//控制码=8
								//其他任务
								TI=Check_OtherTask(ProxyPORTn);//检查是否有其他任务；返回:0=没,!=0有
								if(TI==0)
								{//无其他任务
									MR((u32)p8Data,(u32)p8rx+offset+x,i-x);
									UARTCtrlProxy->BroadcastTimeProxyTask=1;//代理透明转发广播校时任务:0=空,1=有代理
									UARTCtrlProxy->TaskID=(RMTASKnoPOWER<<8)+2;
									UARTCtrlProxy->BroadcastTimeTask=1;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
									while(1)
									{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
										if(UARTCtrlProxy->BroadcastTimeProxyTask==0)//代理透明转发广播校时任务:0=空,1=有代理
										{
											break;
										}
									}
								}
								//代理返回数据
								i=0;
								if((i+5)>LENmax_TxSPACE)
								{
									LEN_TxAPDU=0;
									break;
								}
								p8tx[LEN_TxAPDU]=1;//返回数据      [1] octet-string
								p8tx[LEN_TxAPDU+1]=0;
								LEN_TxAPDU+=2;
								TI=0;
								Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
								return;
							}
						}
					}
				#endif
					break;
				case 3://DL/T698.45
					MR((u32)&UARTCtrl->RMAddr,(u32)p8rx+offset+x+5,6);
					Data_Inverse((u32)&UARTCtrl->RMAddr,6);//数据倒序(高低字节调换),入口Le>=2
				#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					if(ProxyPORTn==RS485_4PORT)
					{
						if((p8rx[offset+x+4]==0xC0)&&(p8rx[offset+x+5]==0xAA)&&(p8rx[offset+x+9]==7))
						{//广播校时
							TI=p8rx[offset+x+12]<<24;
							TI|=p8rx[offset+x+13]<<16;
							TI|=p8rx[offset+x+14]<<8;
							TI|=p8rx[offset+x+15]<<0;
							if(TI==0x40007f00)
							{
								//其他任务
								TI=Check_OtherTask(ProxyPORTn);//检查是否有其他任务；返回:0=没,!=0有
								if(TI==0)
								{//无其他任务
									MR((u32)p8Data,(u32)p8rx+offset+x,i-x);
									UARTCtrlProxy->BroadcastTimeProxyTask=1;//代理透明转发广播校时任务:0=空,1=有代理
									UARTCtrlProxy->TaskID=(RMTASKnoPOWER<<8)+2;
									UARTCtrlProxy->BroadcastTimeTask=1;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
									while(1)
									{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
										if(UARTCtrlProxy->BroadcastTimeProxyTask==0)//代理透明转发广播校时任务:0=空,1=有代理
										{
											break;
										}
									}
								}
								//代理返回数据
								i=0;
								if((i+5)>LENmax_TxSPACE)
								{
									LEN_TxAPDU=0;
									break;
								}
								p8tx[LEN_TxAPDU]=1;//返回数据      [1] octet-string
								p8tx[LEN_TxAPDU+1]=0;
								LEN_TxAPDU+=2;
								TI=0;
								Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
								return;
							}
						}
					}
				#endif
					break;
				case 128://DL376.2
					UARTCtrl->RMAddr[0]=0xFFFF;//显示地址时用,载波口时目的地址用
					UARTCtrl->RMAddr[1]=0xFFFF;//显示地址时用,载波口时目的地址用
					UARTCtrl->RMAddr[2]=0xFFFF;//显示地址时用,载波口时目的地址用
					break;
				
				case 4://CJ/T 188-2004
				default:
					if(ProxyPORTn==RS485_4PORT)
					{
						//无目的地址无法转
						p8tx[LEN_TxAPDU]=0;//错误信息      [0] DAR
						p8tx[LEN_TxAPDU+1]=255;//其他错误 
						LEN_TxAPDU+=2;
						Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
						return;
					}
					UARTCtrl->RMAddr[0]=0xEEEE;//显示地址时用,载波口时目的地址用
					UARTCtrl->RMAddr[1]=0xEEEE;//显示地址时用,载波口时目的地址用
					UARTCtrl->RMAddr[2]=0xEEEE;//显示地址时用,载波口时目的地址用
					break;
			}
			
			if(ProxyPORTn==ACSAMPLEPORT)
			{//交采口
				if(UARTCtrl->RMprotocol==3)//通信协议自动识别:返回:0=未知,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
				{
					TI=Get_ADDR_UARTnRx(ProxyPORTn);
					MW((u32)p8rx+offset+x,TI,i-x);
					Uart_698_Data(ProxyPORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
					if(UARTCtrlProxy->Task==2)//等待单帧数据发送
					{
						UARTCtrlProxy->Task=0;
						x=UARTCtrlProxy->TxByte;
						UARTCtrlProxy->TxByte=0;
						UARTCtrlProxy->ProxyTxRxByte=x;
						MR((u32)p8Data,Get_ADDR_UARTnTx(ProxyPORTn),x);
					}
					else
					{
						UARTCtrlProxy->ProxyTxRxByte=0;
					}
				}
				else
				{
					UARTCtrlProxy->ProxyTxRxByte=0;
				}
			}
			else
			{//其他口
				MR((u32)p8Data,(u32)p8rx+offset,i);
				UARTCtrlProxy->ProxyTxRxByte=i;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
				UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				
				NUM_ReTx=0;//代理重发次数
				while(1)
				{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://主线程
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
					if(UARTCtrlProxy->ProxyTask==0)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
					{
						if(UARTCtrlProxy->ProxyTxRxByte==0)
						{//无数据接收
							if(NUM_ReTx<NUMmax_ReTx)
							{
								NUM_ReTx++;//代理重发次数
								UARTCtrlProxy->ProxyTxRxByte=i;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
								UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
								continue;
							}
						}
						break;
					}
				}
			}
			
			//代理返回数据
			i=UARTCtrlProxy->ProxyTxRxByte;
			if(i==0)
			{
				p8tx[LEN_TxAPDU]=0;//错误信息      [0] DAR
				p8tx[LEN_TxAPDU+1]=33;//请求超时 
				LEN_TxAPDU+=2;
				break;
			}
			if((i+5)>LENmax_TxSPACE)
			{
				LEN_TxAPDU=0;
				break;
			}
			p8tx[LEN_TxAPDU]=1;//返回数据      [1] octet-string
			if(i<=127)
			{
				p8tx[LEN_TxAPDU+1]=i;
				LEN_TxAPDU+=2;
			}
			else
			{
				p8tx[LEN_TxAPDU+1]=0x82;
				p8tx[LEN_TxAPDU+2]=i>>8;
				p8tx[LEN_TxAPDU+3]=i;
				LEN_TxAPDU+=4;
			}
			MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data,i);
			LEN_TxAPDU+=i;
			break;
		default:
			return;
	}
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
}































