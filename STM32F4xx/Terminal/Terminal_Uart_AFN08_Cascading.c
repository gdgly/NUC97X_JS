
//终端级连
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_Uart_Cascading.h"
#include "Terminal_Uart_AutoEvent.h"
#include "Terminal_Uart_TimerTask.h"





void Uart_AFN08(u32 PORTn)//请求被级联终端主动上报
{
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	
	i=MRR(ADDR_AFN04F37,1);//终端级联通信参数
	if(i==0x0)
	{//没级联口
err:
		//全部否认
		Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		i=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息
		Uart_376_CS(Get_ADDR_UARTnTx(PORTn));//和校验计算,同时加结束0x16;修改长度字节的标志位
	}
	else
	{//有级联口
		i=MRR(ADDR_AFN04F37+6,1);//终端级联通信参数
		if((i&0x80)==0x0)
		{//是主动级联口
			Terminal_Ram->Cascading_LoopChack_S_Timer=0;//级联巡测周期秒定时器
		}
		else
		{//是被动级联口
			//主动上报重要事件
			Terminal_AutoEvent(PORTn);//主动上报重要事件
			if(UARTCtrl->Task==2)
			{
				if(UARTCtrl->TxByte!=0)//发送字节数(半字对齐)
				{
					return;
				}
				else
				{
					UARTCtrl->Task=0;
				}
			}
			//定时任务
			for(i=0;i<(2*MaxTimerTask);i++)
			{
				Terminal_TimerTask(PORTn);//定时任务
				if(UARTCtrl->Task==2)
				{
					if(UARTCtrl->TxByte!=0)//发送字节数(半字对齐)
					{
						return;
					}
					else
					{
						UARTCtrl->Task=0;
					}
				}
			}
			goto err;
		}
	}
	
	
}


















