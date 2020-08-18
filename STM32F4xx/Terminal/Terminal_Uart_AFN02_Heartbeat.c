
//链路接口检测
#include "Project.h"
#include "Terminal_Uart_AFN02_Heartbeat.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"



const u8 Heartberat_Example[]=
{
	0x68,
	12,0,
	12,0,
	0x68,
	0xc9,//控制码(DIR=1上行,启动标志位PRM=1,功能码9=请求∕响应帧	链路测试)
	0,0,//行政区划码
	0,0,//终端地址
	0,//启动站地址(终端启动发送帧的MSA应为零)
	2,//AFN=2
	0x70,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ
	0,0,//Pn=0
	0x04,0,//Fn=1登入,2退出,3心跳
	0,//cs
	0x16,
};


void Heartberat(u32 PORTn,u32 ADDR_HeartberatTimer)//主动心跳(链路接口检测)
{
	u32 i;
	u8 * p8txbuff;
	u16 * p16;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 UARTxTimerAddr;
	
	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		{
			return;
		}
	}
	UARTxTimerAddr=Get_ADDR_UARTnTimer(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	p16=(u16 *)(ADDR_HeartberatTimer);//心跳发出间隔秒定时器
	if(p16[0]==0x0)
	{
		if(UARTCtrl->Task==0x0)
		{//接收
			p16=(u16 *)(UARTxTimerAddr);//UART定时器MS
			if(p16[0]==0x0)
			{
				if((UARTCtrl->LinkTask>=100)&&(UARTCtrl->LinkTask<=102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{//连接
						if((UARTCtrl->TCPUDP&0x30)==0x20)//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
						{//服务器模式,在设定(心跳周期+60秒)内没收到正确数据帧为断线
							if(PORTn==GPRSPORT)
							{
								i=Terminal_Ram->GPRSHeartBeat_S_Timer;//GPRS心跳秒定时器
							}
							if(PORTn==ENETPORT)
							{
								i=Terminal_Ram->ENETHeartBeat_S_Timer;//以太网心跳秒定时器
							}
							if(i==0)
							{
								UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							}
							
							if(UARTCtrl->LinkTask==102)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							{
								UARTCtrl->LinkTask=103;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							}
							return;
						}
						
						if(UARTCtrl->HostACK>7)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
						{//值错误
							UARTCtrl->HostACK=0;
						}
						if(UARTCtrl->HostACK>=4)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
						{//有其他主动上报等待确认返回
							return;
						}
						UARTCtrl->HeartberatCount+=1;//心跳发出计数,收到确认帧清0
						if(UARTCtrl->HeartberatCount>3)
						{//连续3次没收到确认帧
							UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
							if(UARTCtrl->LinkTask==102)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							{
								UARTCtrl->LinkTask=103;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							}
							else
							{
								UARTCtrl->LinkTask=0;//重新初始化
								if(PORTn==GPRSPORT)
								{
									WarningOccur(PORTn,(u8 *)"远程无线主站无响应");//告警发生,入口告警代码和告警字符串
								}
								if(PORTn==ENETPORT)
								{
									WarningOccur(PORTn,(u8 *)"以太网主站无响应");//告警发生,入口告警代码和告警字符串
								}
							}
						}
						else
						{
							UARTCtrl->Task=2;//2=等待单帧数据发送
							UARTCtrl->TxByte=20;//需发送字节数
							p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
							MR((u32)p8txbuff,(u32)Heartberat_Example,18);
							MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//行政码,终端地址
							p8txbuff[13]|=(UARTCtrl->TxSEQ&0xf);//启动帧序号PSEQ
							UARTCtrl->TxSEQ+=1;//启动帧序号PSEQ+1
							UARTCtrl->HostACK=3;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
							switch(UARTCtrl->LinkTask)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							{
								default://心跳
									MR(((u32)p8txbuff)+18,ADDR_AFN0CF2,6);//终端时钟
									p8txbuff[1]=12+6;
									UARTCtrl->TxByte=20+6;//需发送字节数
									break;
								case 100://登入
									p8txbuff[16]=1;//登入
									UARTCtrl->HostACK=1;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
									break;
								case 102://退出
									p8txbuff[16]=2;//退出
									UARTCtrl->HostACK=2;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
									UARTCtrl->HeartberatCount=100;//退出时心跳发出计数设为100,若返回定时内没确认回答则立即重新初始化关闭模块连接
									break;
							}
							Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
							p16=(u16 *)(ADDR_HeartberatTimer);//心跳发出间隔秒定时器
							i=MRR(ADDR_AFN04F1+2,2);
							i&=0xfff;
							if(i<WaitHeartberatACKMinTimer)
							{
								i=WaitHeartberatACKMinTimer;//心跳发出等待主站确认最小定时秒
							}
							if(i>WaitHeartberatACKMaxTimer)
							{
								i=WaitHeartberatACKMaxTimer;//心跳发出等待主站确认最大定时秒
							}
							p16[0]=i;//等心跳返回定时秒
						}

				}
			}
		}
	}
}


void Uart_AFN02(u32 PORTn)//链路接口检测
{
	u32 i;
	u8 * p8rxbuff;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;

  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+11;//响应功能码 11
	if((p8txbuff[16]==0x01)||(p8txbuff[16]==0x04))//Fn=1登入,2退出,3心跳
	{//登入或心跳
		if(UARTCtrl->LinkTask==100)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
		{
			UARTCtrl->LinkTask=101;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
		}
	}
	if(p8txbuff[16]==0x02)//Fn=1登入,2退出,3心跳
	{//退出
		UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	}
	Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,数据单元标识
	i=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}






