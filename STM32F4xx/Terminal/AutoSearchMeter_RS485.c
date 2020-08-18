
//自动搜表RS485
#include "Project.h"
#include "AutoSearchMeter_RS485.h"
#include "AutoSearchMeter.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"


void AutoSearchMeter_RS485_DL645(u32 PORTn)//自动搜表RS485端口的DL645
{
	u32 i;
	u32 x;
	u32 RS485PORT;
	u8 *p8;
	u8 * p8txbuff;
	u8 * p8rxbuff;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16fifo;
	u16 *p16timer;
	u32 ProtocolBps;

	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=0;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=1;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=2;
			break;
		default:
			return;
	}
	ProtocolBps=MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT)+6,1);
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			if(UARTCtrl->BatchTask>=1)//转发批量任务:0=空,1=等待口空闲启动,2...过程
			{
				return;
			}
			if(p16timer[0]==0x0)
			{//下帧启动发送延时
				UARTCtrl->Task=3;//启动发送
			}
			break;
		case 3://3=启动发送
			if(((ProtocolBps>>4)==0)||((ProtocolBps>>4)==2))//B4-B7:0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
			{//DL645-2007
				Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x00010000,MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6));//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
			}
			else
			{//DL645-1997
				Read_DL645_1997_TxData(((u32)p8txbuff)+4,0x9010,MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6));//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
			}
		TX:
			if(((ProtocolBps>>4)==0)||((ProtocolBps>>4)==2))//B4-B7:0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
			{//DL645-2007
				UARTCtrl->TxByte=4+16;//TX计数(需发送字节数)
				UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//默认2400
			}
			else
			{//DL645-1997
				UARTCtrl->TxByte=4+14;//TX计数(需发送字节数)
				UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//默认1200
			}
			MC(0xfe,(u32)p8txbuff,4);
			//p16fifo[0]=0;
			//p16fifo[1]=0;
			//p16fifo[2]=0;
			UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
			}
#endif
			CopyString((u8 *)"485x正在自动搜表",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			MWR(RS485PORT+1,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case 4://4=正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				UARTCtrl->Task=5;
				p16timer[0]=1000/10;//1000ms接收定时
			}
			break;
		case 5://5=正在接收
			DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
			if(p16fifo[0]!=p16fifo[2])
			{//有字节收到
				p16fifo[2]=p16fifo[0];
				p16timer[0]=500/10;//500ms定时
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
				if(PORTn==RS485_1PORT)
				{
					Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
				}
				if(PORTn==RS485_2PORT)
				{
					Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
				}
#endif
				break;
			}
			if(p16timer[0]!=0)
			{//等待全部发送完成			
				break;
			}
			i=DL645_Rx(PORTn);//DL645帧接收;返回0没完成,1完成,2超时
			//p16timer[0]=50/10;//下帧启动发送延时20MS后
			switch(i)
			{
				case 0://0没完成
					return;
				case 1://1完成
					UARTCtrl->Task=0;//空闲1次
					if((p8rxbuff[8]&0x80)==0x0)
					{//不是从动帧
						return;//重发
					}
					if(p16fifo[0]!=p16fifo[1])
					{
						goto Over;//有多佘字节接收作为超时
					}
					for(i=5;i<11;i++)
					{
						if(p8txbuff[i]==0xaa)
						{
							break;
						}
					}
					if(i<11)
					{//发送地址验证
						if(((ProtocolBps>>4)==0)||((ProtocolBps>>4)==2))//B4-B7:0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
						{//DL645-2007
							Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x00010000,MRR(((u32)p8rxbuff)+1,6));//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
						}
						else
						{//DL645-1997
							Read_DL645_1997_TxData(((u32)p8txbuff)+4,0x9010,MRR(((u32)p8rxbuff)+1,6));//DL645-1997读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
						}
						goto TX;
					}
					if(DataComp(((u32)p8rxbuff)+1,((u32)p8txbuff)+5,6)==0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
					{
						if(((ProtocolBps>>4)==0)||((ProtocolBps>>4)==2))//B4-B7:0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
						{//DL645-2007
							//AutoSetAFN04F10(PORTn,MRR(((u32)p8txbuff)+5,6),30,0xeeeeeeeeeeee,ProtocolBps&0x0f,0);//自动设置AFN04F10的电能表配置
							AutoSetAFN04F10(PORTn,MRR(((u32)p8txbuff)+5,6),30,0x0,ProtocolBps&0x0f,0);//自动设置AFN04F10的电能表配置
						}
						else
						{//DL645-1997
							//AutoSetAFN04F10(PORTn,MRR(((u32)p8txbuff)+5,6),1,0xeeeeeeeeeeee,ProtocolBps&0x0f,0);//自动设置AFN04F10的电能表配置
							AutoSetAFN04F10(PORTn,MRR(((u32)p8txbuff)+5,6),1,0x0,ProtocolBps&0x0f,0);//自动设置AFN04F10的电能表配置
						}
						//下搜表地址
						if((ProtocolBps>>4)<2)//B4-B7:0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
						{
							MR(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
							p8=(u8*)(ADDR_DATABUFF);
							for(i=0;i<6;i++)
							{
								if(p8[i]==0xaa)
								{
									break;
								}
							}
							if(i!=0)
							{
								i--;
							}
							while(1)
							{
								x=p8[i];
								if(x==0x99)
								{
									if(i!=0)
									{
										p8[i]=0xaa;
										i--;
										continue;
									}
									else
									{
										if((ProtocolBps>>4)==0)
										{//原DL645-2007快速
											MWR(0x12aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
										}
										else
										{//原DL645-1997快速
											MWR(0x23aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
										}
										return;
									}
								}
								break;
							}
							x=bcd_hex(x);
							x++;
							x=hex_bcd(x);
							p8[i]=x;
							MW(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
							break;
						}
					}
				default://2超时
				Over://有多佘字节接收作为超时
					UARTCtrl->Task=0;//空闲1次
					MR(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<6;i++)
					{
						if(p8[i]==0xaa)
						{
							break;
						}
					}
					if(i!=0)
					{
						i--;
					}
					if(p16fifo[0]==0)
					{//没收到任何字节
						while(1)
						{
							x=p8[i];
							if(x==0x99)
							{
								if(i==0)
								{//地址回0
									switch(ProtocolBps>>4)
									{
										case 0://原0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
											MWR(0x12aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 1://原0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
											MWR(0x23aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 2://原0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
											MWR(0x32aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 3://原0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
										default:
											MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											if(MRR(ADDR_AFN05F149,1)==0x55)
											{//是F149启动
												if(MRR(ADDR_AFN05F149+2+RS485PORT,1)==0x5a)//电能表通信参数自动维护功能开启与关闭
												{//立即执行
													MWR(0xaa,ADDR_AFN05F149+2+RS485PORT,1);//电能表通信参数自动维护功能开启与关闭
												}
											}
											MC(0,Get_ADDR_UARTn(PORTn),6);//请fifo指针
											UARTCtrl->LockFlags&=0xfd;//通信功能被禁止标志:B0=菜单操作,B1=自动搜表,B2-B7保留
											return;
									}
								}
								else
								{
									p8[i]=0xaa;
									i--;
								}
							}
							else
							{
								break;
							}
						}
						x=bcd_hex(x);
						x++;
						x=hex_bcd(x);
						p8[i]=x;
					}
					else
					{//有收到字节
						if(i<5)
						{
							i++;
							p8[i]=0;
						}
						else
						{
							while(1)
							{
								x=p8[i];
								if(x==0x99)
								{
									if(i!=0)
									{
										p8[i]=0xaa;
										i--;
										continue;
									}
									else
									{
										switch(ProtocolBps>>4)
										{
											case 0://原0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
												MWR(0x12aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 1://原0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
												MWR(0x23aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 2://原0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
												MWR(0x32aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 3://原0=DL645-2007快速,1=DL645-1997快速,2=DL645-2007准确,3=DL645-1997准确
											default:
												MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												if(MRR(ADDR_AFN05F149,1)==0x55)
												{//是F149启动
													if(MRR(ADDR_AFN05F149+2+RS485PORT,1)==0x5a)//电能表通信参数自动维护功能开启与关闭
													{//立即执行
														MWR(0xaa,ADDR_AFN05F149+2+RS485PORT,1);//电能表通信参数自动维护功能开启与关闭
													}
												}
												MC(0,Get_ADDR_UARTn(PORTn),6);//请fifo指针
												UARTCtrl->LockFlags&=0xfd;//通信功能被禁止标志:B0=菜单操作,B1=自动搜表,B2-B7保留
												return;
										}
									}
								}
								break;
							}
							x=bcd_hex(x);
							x++;
							x=hex_bcd(x);
							p8[i]=x;
						}
					}
					MW(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					return;
			}
//			break;
	}
}

