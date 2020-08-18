

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/RM_RS485.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../Device/MEMRW.h"
#include "../DL698/RM_TASK.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_Uart.h"




void BroadcastTime_RS485(u32 PORTn)//广播校时-rs485;完成后置UARTCtrl->BroadcastTimeTask=0
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8tx;
	u16 * p16timer;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(UARTCtrl->BroadcastTimeTask)//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
	{
		default:
			UARTCtrl->TaskID=0;
			UARTCtrl->BroadcastTimeTask=0;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
		case 1:
			UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
		case 2:
			RM_ADDR_Display(PORTn);//抄表地址显示
			p16timer[0]=1000/10;//延时1秒后进行
			UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
		case 3://DL698 9600发送
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->BpsCtrl=(6<<5)+0x0b;
			i=BroadcastTime_TxDATA_DL698(p8tx);//DL698广播校时发送帧;返回:组帧后总长度字节数
			UARTCtrl->TxByte=i;//发送字节数
			//启动
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;//正在发送
			RS485_TxLED(PORTn);//RS485 LED发送指示灯
			UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
		case 4:
			RS485_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(UARTCtrl->Task<3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				p16timer[0]=1000/10;//延时1秒后进行
				UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			}
			break;
		case 5://DL645 1200发送
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			i=BroadcastTime_TxDATA_DL645(p8tx);//DL645广播校时发送帧;返回:组帧后总长度字节数
			UARTCtrl->TxByte=i;//发送字节数
			//启动
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;//正在发送
			RS485_TxLED(PORTn);//RS485 LED发送指示灯
			UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
		case 6:
			RS485_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(UARTCtrl->Task<3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				p16timer[0]=1000/10;//延时1秒后进行
				UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			}
			break;
		case 7://DL645 2400发送
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->BpsCtrl=(3<<5)+0x0b;
			i=BroadcastTime_TxDATA_DL645(p8tx);//DL645广播校时发送帧;返回:组帧后总长度字节数
			UARTCtrl->TxByte=i;//发送字节数
			//启动
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;//正在发送
			RS485_TxLED(PORTn);//RS485 LED发送指示灯
			UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
		case 8:
			RS485_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(UARTCtrl->Task<3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				p16timer[0]=1000/10;//延时1秒后进行
				UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			}
			break;
		case 9:
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->TaskID=0;
			UARTCtrl->BroadcastTimeTask=0;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
	}
}






#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void BroadcastTime_3762(u32 PORTn)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
{
	u32 i;
	u32 x;
	u64 d64;
	u8 *p8rx;
	u8 *p8tx;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef *UARTCtrlProxy;
	u8* p8Data;	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(UARTCtrl->BroadcastTimeTask)//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
	{
		default:
			UARTCtrl->TaskID=0;
			UARTCtrl->BroadcastTimeTask=0;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
		case 1:
			UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待,2-127校时-2过程;0x81=校时-3等待,0x82-0xFF校时-3过程
			break;
		case 2://下帧发送延时
//			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//集中器时用于终端收到路由模块数据帧后到发送延时定时器
//			if(p16timer[0]!=0)//下帧启动发送延时
//			{
//				return;
//			}
			UARTCtrl->BroadcastTimeTask++;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
			break;
		case 3://暂停路由
			Terminal_Router->StopFlags|=(1<<4);//暂停路由
			Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
		
			if(UARTCtrl->BroadcastTimeProxyTask==0)//代理透明转发广播校时任务:0=空,1=有代理
			{
				UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			else
			{//是透明转发命令下发的广播校时
				UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(UARTCtrl->ProxyPORTn);
				if(UARTCtrlProxy->RMprotocol==3)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
				{//DL698
					UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				else
				{//
					UARTCtrl->BroadcastTimeTask=10;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
			}
			break;
		case 4://等待1分
//			MC(0,ADDR_DATABUFF+18,4);
//			CopyString((u8 *)"载波准备698广播校时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
//			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			p16timer[0]=0;//60000/10;
			Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
			UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
		case 5://通信延时相关广播通信时长查询
			if(p16timer[0]==0)
			{
				MC(0,ADDR_DATABUFF+18,4);
				CopyString((u8 *)"载波发送698广播校时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
				p8tx+=128;
				p8tx[0]=0;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
				//p8tx[1]=32;//报文长度
				//MC(0xfe,((u32)p8tx)+2,4);
				//p8tx+=6;
				p8tx[1]=32-4;//报文长度
				
				p8tx+=6-4;
			//起始字符（68H）  1Byte
				p8tx[0]=0x68;
			//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
			//控制域C  1Byte
				p8tx[3]=0x43;
			//地址域A
				p8tx[4]=0xC0;
				p8tx[5]=0xAA;
			//客户机地址CA  1Byte
				p8tx[6]=0;
			//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
			//APDU
				p8tx[9]=0x07;
				p8tx[10]=1;
				p8tx[11]=0;//PIID
				p8tx[12]=0x40;//OAD
				p8tx[13]=0x00;
				p8tx[14]=0x7F;
				p8tx[15]=0;
				p8tx[16]=DataType_date_time_s;
				if(UARTCtrl->BroadcastTimeProxyTask==0)//代理透明转发广播校时任务:0=空,1=有代理
				{
					MR((u32)p8tx+17,ADDR_DL698YMDHMS,7);
				}
				else
				{
					p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);//得到UART抄表数据缓冲地址
					MR((u32)p8tx+17,(u32)p8Data+17,7);
				}
				i=24;
			//时间标签
				p8tx[i]=0;
			//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
			//结束字符（16H）
				i+=4;
				
				i-=2;
				p8tx[1]=i;
				p8tx[2]=i>>8;
				Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
				//i+=2+6;
				i+=2+6-4;
				Uart_3762_TxData(PORTn,0x03,9,i);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
				UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			break;
		case 6://发广播校时
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=MRR(((u32)p8rx)+10,3);
			if((i!=0x010103)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
				{
					Terminal_Router->ReRouterCtrl++;
					UARTCtrl->BroadcastTimeTask--;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				else
				{
					UARTCtrl->BroadcastTimeTask=100;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				break;
			}
			x=MRR(((u32)p8rx)+13,2);//广播通信延迟时间秒
			//发DL698广播校时
			p8tx+=128;
			p8tx[0]=0;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
			//p8tx[1]=32;//报文长度
			//MC(0xfe,((u32)p8tx)+2,4);
			//p8tx+=6;
			
			p8tx[1]=32-4;//报文长度
			p8tx+=6-4;
		//起始字符（68H）  1Byte
			p8tx[0]=0x68;
		//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
		//控制域C  1Byte
			p8tx[3]=0x43;
		//地址域A
			p8tx[4]=0xC0;
			p8tx[5]=0xAA;
		//客户机地址CA  1Byte
			p8tx[6]=0;
		//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
		//APDU
			p8tx[9]=0x07;
			p8tx[10]=1;
			p8tx[11]=0;//PIID
			p8tx[12]=0x40;//OAD
			p8tx[13]=0x00;
			p8tx[14]=0x7F;
			p8tx[15]=0;
			p8tx[16]=DataType_date_time_s;
			if(UARTCtrl->BroadcastTimeProxyTask==0)//代理透明转发广播校时任务:0=空,1=有代理
			{
				MR((u32)p8tx+17,ADDR_DL698YMDHMS,7);
			}
			else
			{
				p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);//得到UART抄表数据缓冲地址
				MR((u32)p8tx+17,(u32)p8Data+17,7);
			}
			pYYMDHMS_AddS_hex(p8tx+17,x);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
			
			i=24;
		//时间标签
			p8tx[i]=0;
		//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
		//结束字符（16H）
			i+=4;
			
			i-=2;
			p8tx[1]=i;
			p8tx[2]=i>>8;
			Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
			//i+=2+6;
			i+=2+6-4;
			Uart_3762_TxData(PORTn,0x05,3,i);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			//Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
			if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
			{
				Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//最大广播超时时间秒(半字对齐)
			}
			UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
		case 7://接收确认帧
			if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
			{//0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文
				switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				{
					case 1://1=完成1帧发收
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
							UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
						}
						break;
					case 0://0=空闲
					case 2://2=接收超时(或错误)
						if(Terminal_Ram->RouterCheck_S_Timer==0)//最大广播超时时间秒(半字对齐)
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
							UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
						}
						break;
				}
			}
			else
			{//1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
				switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				{
					case 1://1=完成1帧发收
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							//取等待时间
							i=p8rx[17];
							i+=p8rx[18]<<8;
							i*=1000/10;
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16timer[0]=i;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							break;
						}
						else
						{
							if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
							{
								Terminal_Router->ReRouterCtrl++;
								UARTCtrl->BroadcastTimeTask--;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							}
							else
							{
								UARTCtrl->BroadcastTimeTask=100;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							}
						}
						break;
					case 0://0=空闲
					case 2://2=接收超时(或错误)
						UARTCtrl->BroadcastTimeTask=100;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						break;
				}
			}
			break;
		case 8://等待广播延时
			if(p16timer[0]==0)
			{
				if(UARTCtrl->BroadcastTimeProxyTask==0)//代理透明转发广播校时任务:0=空,1=有代理
				{
					UARTCtrl->BroadcastTimeTask=10;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				else
				{//透明转发命令下发的广播校时
					UARTCtrl->BroadcastTimeTask=100;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
			}
			break;
//		case 9:
//			break;
		case 10:
//			MC(0,ADDR_DATABUFF+18,4);
//			CopyString((u8 *)"载波准备645广播校时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
//			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			p16timer[0]=0;//5000/10;
			Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
			UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
		case 11://通信延时相关广播通信时长查询
			if(p16timer[0]==0)
			{
				MC(0,ADDR_DATABUFF+18,4);
				CopyString((u8 *)"载波发送645广播校时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
				p8tx+=128;
				p8tx[0]=2;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
				//p8tx[1]=22;//报文长度
				//MC(0xfe,((u32)p8tx)+2,4);
				p8tx[1]=22-4;//报文长度
				p8tx-=4;
				p8tx[6]=0x68;
				MC(0x99,((u32)p8tx)+7,6);
				p8tx[13]=0x68;
				p8tx[14]=0x08;
				p8tx[15]=0x06;

				if(UARTCtrl->BroadcastTimeProxyTask==0)//代理透明转发广播校时任务:0=空,1=有代理
				{
					MR((u32)p8tx+16,ADDR_MYMDHMS,6);
				}
				else
				{//是透明转发命令下发的广播校时
					p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);//得到UART抄表数据缓冲地址
					MR((u32)p8tx+16,(u32)p8Data+10,6);
					for(i=0;i<6;i++)
					{
						p8tx[16+i]-=0x33;
					}
				}
				
				for(i=0;i<6;i++)
				{
					p8tx[16+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8tx[6+i];
				}
				p8tx[22]=x;
				p8tx[23]=0x16;
				//Uart_3762_TxData(PORTn,0x03,9,24);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
				Uart_3762_TxData(PORTn,0x03,9,20);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
				UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			break;
		case 12://发广播校时
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=MRR(((u32)p8rx)+10,3);
			if((i!=0x010103)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
				{
					Terminal_Router->ReRouterCtrl++;
					UARTCtrl->BroadcastTimeTask--;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				else
				{
					UARTCtrl->BroadcastTimeTask=100;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				break;
			}
			x=MRR(((u32)p8rx)+13,2);//广播通信延迟时间秒
			//发DL645广播校时
			p8tx+=128;
			p8tx[0]=2;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
			//p8tx[1]=22;//报文长度
			//MC(0xfe,((u32)p8tx)+2,4);
			p8tx[1]=22-4;//报文长度
			p8tx-=4;
			p8tx[6]=0x68;
			MC(0x99,((u32)p8tx)+7,6);
			p8tx[13]=0x68;
			p8tx[14]=0x08;
			p8tx[15]=0x06;
			
			if(UARTCtrl->BroadcastTimeProxyTask==0)//代理透明转发广播校时任务:0=空,1=有代理
			{
				MR((u32)p8tx+16,ADDR_MYMDHMS,6);
			}
			else
			{//是透明转发命令下发的广播校时
				p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);//得到UART抄表数据缓冲地址
				MR((u32)p8tx+16,(u32)p8Data+10,6);
				for(i=0;i<6;i++)
				{
					p8tx[16+i]-=0x33;
				}
			}
			d64=MRR(((u32)p8tx)+16,6);
			d64=YMDHMS_AddS(d64,x);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
			MWR(d64,((u32)p8tx)+16,6);
			
			for(i=0;i<6;i++)
			{
				p8tx[16+i]+=0x33;
			}
			x=0;
			for(i=0;i<16;i++)
			{
				x+=p8tx[6+i];
			}
			p8tx[22]=x;
			p8tx[23]=0x16;
			//Uart_3762_TxData(PORTn,0x05,3,24);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			Uart_3762_TxData(PORTn,0x05,3,20);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
			{
				Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//最大广播超时时间秒(半字对齐)
			}
			UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0	
			break;
		case 13://接收确认帧
			if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
			{//0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文
				switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				{
					case 1://1=完成1帧发收
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
							UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
						}
						break;
					case 0://0=空闲
					case 2://2=接收超时(或错误)
						if(Terminal_Ram->RouterCheck_S_Timer==0)//最大广播超时时间秒(半字对齐)
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
							UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
						}
						break;
				}
			}
			else
			{//1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
				switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				{
					case 1://1=完成1帧发收
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							//取等待时间
							i=p8rx[17];
							i+=p8rx[18]<<8;
							i*=1000/10;
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16timer[0]=i;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							break;
						}
						else
						{
							if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
							{
								Terminal_Router->ReRouterCtrl++;
								UARTCtrl->BroadcastTimeTask--;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							}
							else
							{
								UARTCtrl->BroadcastTimeTask=100;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							}
						}
						break;
					case 0://0=空闲
					case 2://2=接收超时(或错误)
						UARTCtrl->BroadcastTimeTask=100;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						break;
				}
			}
			break;
		case 14://等待广播延时
			if(p16timer[0]==0)
			{
				UARTCtrl->BroadcastTimeTask=100;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			break;
			
		case 100:
			Terminal_Router->StopFlags&=~(1<<4);//恢复路由
			//Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
			UARTCtrl->TaskID=0;
			UARTCtrl->BroadcastTimeTask=0;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			UARTCtrl->BroadcastTimeProxyTask=0;//代理透明转发广播校时任务:0=空,1=有代理
			break;
	}
}


#endif





















