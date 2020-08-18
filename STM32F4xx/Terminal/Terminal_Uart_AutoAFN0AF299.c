
#include "Project.h"
#include "Terminal_Uart_AutoAFN0AF299.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"


#if ((USER/100)==8 || (USER/100)==7)//通用市场系、浙江版

const u8 AutoAFN0AF299_Example[]=
{//终端请求主站时钟
	0x68,
	40,0,
	40,0,
	0x68,
	0xC0,//控制码(DIR=1上行,启动标志位PRM=1,功能码暂4=发送∕无回答,若需要主站确认的通信服务（CON=1)10=请求∕响应)
	0x71,0x05,//行政区划码
	0x05,0x00,//终端地址
	0x02,//启动站地址(终端启动发送帧的MSA应为零)
	0x0A,//AFN0A
	0x60,//SEQ
	0x00,0x00,//Pn=0
	0x04,0x25,//Fn=299
	0x01,//通道特征:1GPRS 2CDMA 3Ethernet 4phone_line 5PLC
	0x00,0x00,0x22,0x08,0x10,0x14,0x10,0x15,0x20,//终端查询时间T1 2015年10月14日10:08:22，0000ms
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,//主站接收时间T2 年月日时分秒毫秒
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,//主站响应时间T3 年月日时分秒毫秒
	0x20,//CS
	0x16,
};

void Terminal_AutoAFN0AF299(u32 PORTn)//辽宁、浙江扩展AFN0AF299终端请求主站时钟
{
	u32 i;
	u16 * p16;
	u16 * p16ACKTimer;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;

	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
		{
			return;
		}
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	if(UARTCtrl->Task==0x0)
	{//接收
		p16=(u16 *)Get_ADDR_UARTnTimer(PORTn);
		if(p16[0]==0x0)
		{
			if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			{//登入
				if(PORTn==GPRSPORT)
				{
					p16ACKTimer=(u16 *)((u32)&Terminal_Ram->GPRS_WaitHostACK_S_Timer);//6 主动上报等待主站确认秒定时器
				}
				if(PORTn==ENETPORT)
				{
					p16ACKTimer=(u16 *)((u32)&Terminal_Ram->ENET_WaitHostACK_S_Timer);//6 主动上报等待主站确认秒定时器
				}
				if(UARTCtrl->HostACK!=0x0)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=请求主站时钟
				{
					if(UARTCtrl->HostACK!=7)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=请求主站时钟
					{//有其他主动上报等待确认返回
						return;
					}
					if(p16ACKTimer[0]!=0x0)
					{
						return;//等待主站确认秒定时器!=0
					}
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=请求主站时钟
					if(UARTCtrl->HostCount>2)
					{//重发次数到
						UARTCtrl->HostCount=0;
						if(Terminal_Ram->PowerTask==0)//停电事件任务
						{
							Terminal_Ram->RMDelay_S_Timer=10;
							Terminal_Ram->PowerTask=1;//停电事件任务
						}
						else
						{//请求主站3次未响应则本次精确对时停止
							MWR(0,ADDR_AFN04F299_Ext+28,1);
						}
						return;
					}
				}
				//发送
				UARTCtrl->Task=2;//2=等待单帧数据发送
				UARTCtrl->TxByte=48;//需发送字节数
				p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
				MR((u32)p8txbuff,(u32)AutoAFN0AF299_Example,48);
				if(PORTn==GPRSPORT)
				{
					if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
					{//GSM网络支持GPRS
						p8txbuff[18]=1;
					}
					else
					{//CDMA
						p8txbuff[18]=2;
					}
				}
				if(PORTn==ENETPORT)
				{
					p8txbuff[18]=3;
				}
				MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//行政码,终端地址
				MR(((u32)p8txbuff)+21,ADDR_TYMDHMS,6);//终端时钟
				//月份去掉星期
				p8txbuff[25]&=0x1f;

				if(UARTCtrl->HostCount==0)
				{//不是重复帧
					p8txbuff[13]|=UARTCtrl->TxSEQ&0xf;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
					UARTCtrl->ReTxSEQ=UARTCtrl->TxSEQ&0xf;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变时用)
					UARTCtrl->TxSEQ+=1;//启动帧序号PSEQ+1
				}
				else
				{//是重复帧
					p8txbuff[13]|=UARTCtrl->ReTxSEQ;//重发时启动帧序列域SEQ(重复发送时帧序列域SEQ不变寄存)
				}
				UARTCtrl->HostACK=7;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=请求主站时钟
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				UARTCtrl->HostCount++;

				i=MRR(ADDR_AFN04F1+2,2);//终端等待从动站响应的超时时间
				i&=0xfff;
				if(i<WaitHostACKMinTimer)
				{
					i=WaitHostACKMinTimer;//主动上报等待主站确认最小定时秒
				}
				if(i>WaitHostACKMaxTimer)
				{
					i=WaitHostACKMaxTimer;//主动上报等待主站确认最大定时秒
				}
				p16ACKTimer[0]=i;//主动上报等待主站确认秒
			}
		}
	}
}

#endif
