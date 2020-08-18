
//确认/否认
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"




void Uart_AFN00(u32 PORTn)//确认/否认
{
#if (USER/100)==5//上海
	u32 i;
#endif
  UARTCtrl_TypeDef * UARTCtrl;
//	u8 * p8rxbuff;
//	u8 * p8txbuff;


  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	switch(UARTCtrl->HostACK)//主动发出需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
	{
		case 1://1=登录帧
			UARTCtrl->LinkTask=101;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
			UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0			
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
//			if(PORTn==GPRSPORT)
//			{
//				Comm_Ram->SelfTestFlags|=1<<11;//B11=GPRS登录(自动,但需设置IP等参数)
//			}
//			if(PORTn==ENETPORT)
//			{
//				Comm_Ram->SelfTestFlags|=1<<17;//B17=以太网登录(自动,但需设置IP等参数)
//			}
		#if (USER/100)==5//上海
			//F241	当前登录主站信息（最近一次）
			MW(ADDR_AFN0CF2,ADDR_AFN0CF241,6);
			//月冻结F242	月冻结登录主站次数
			i=MRR(ADDR_AFN0CF242+4,4);
			i++;
			MWR(i,ADDR_AFN0CF242+4,4);
			//月冻结F241	日冻结登录主站次数
			i=MRR(ADDR_AFN0CF242,4);
			i++;
			if(i>AFN0CF242MaxCount)//当日登录主站信息最大记录次数
			{
				i=AFN0CF242MaxCount;
			}
			//F242	当日登录主站信息
			DMW(ADDR_AFN0CF2,ADDR_AFN0CF242_Next+(6*(i-1)),6);
			MWR(i,ADDR_AFN0CF242,4);
		#endif
			break;
		case 2://2=退出登录帧
			UARTCtrl->LinkTask=103;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
			UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0			
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
			break;
		case 3://3=心跳帧
			UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0			
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
			break;
		case 4://4=重要事件
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
			UARTCtrl->HostCount=0;//主动上报重发计数(心跳发出计数另)
			if(PORTn==GPRSPORT)
			{
				MC(0,ADDR_AFN0CF7_FM+3,1);//清当前有?个重要事件主动上传需确认
			}
#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报			
			if(PORTn==ENETPORT)
			{
				MC(0,ADDR_AFN0CF7_FM+5,1);//清当前有?个重要事件主动上传需确认
			}
#else
			if(PORTn==ENETPORT)
			{
				MC(0,ADDR_AFN0CF7_FM+3,1);//清当前有?个重要事件主动上传需确认
			}
#endif			
			if(PORTn==RS232PORT)
			{
				MC(0,ADDR_AFN0CF7_FM+7,1);//清当前有?个重要事件主动上传需确认
			}
			break;
		case 5://5=定时任务
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
			UARTCtrl->HostCount=0;//主动上报重发计数(心跳发出计数另)
			break;
		case 6://6=电能表事件
//		  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
//		  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
//			if((p8txbuff[13]&0xf)!=(p8rxbuff[13]&0xf))
//			{//帧序列域SEQ不同
//				break;
//			}
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
			UARTCtrl->HostCount=0;//主动上报重发计数(心跳发出计数另)
			if(PORTn==GPRSPORT)
			{
				MC(0,ADDR_MeterEventCount_FM+3,1);//清当前有?个重要事件主动上传需确认
			}
#if GPRSENETAutoRePort==0//GPRS和以太网口主动上报0=分开独立上报,1=合并上报			
			if(PORTn==ENETPORT)
			{
				MC(0,ADDR_MeterEventCount_FM+5,1);//清当前有?个重要事件主动上传需确认
			}
#else
			if(PORTn==ENETPORT)
			{
				MC(0,ADDR_MeterEventCount_FM+3,1);//清当前有?个重要事件主动上传需确认
			}
#endif			
			if(PORTn==RS232PORT)
			{
				MC(0,ADDR_MeterEventCount_FM+7,1);//清当前有?个重要事件主动上传需确认
			}
			break;
	}
}




