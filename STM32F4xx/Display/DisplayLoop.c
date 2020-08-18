
//终端循显
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"

#endif
#include "../Display/DisplayLoop.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayLoop_Warning.h"
#include "../Display/DisplayLoop_Info.h"
#include "../Display/DisplayLoop_ACSample.h"
#include "../Display/DisplayLoop_Self.h"
#include "../DL698/DL698_DisplayLoop_Contrl.h"

#include "../Display/Display.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




void TerminalDisplayLoopCount(void)//终端循显计数
{
	if(TerminalDisplayLoop->ButtonVal&2)
	{//上移
		if(TerminalDisplayLoop->Count)//当前循环显示任务已显屏计数
		{
			TerminalDisplayLoop->Count--;
		}
		else
		{
			TerminalDisplayLoop->Count=60;
			if(TerminalDisplayLoop->Task)//当前循环显示任务
			{
				TerminalDisplayLoop->Task--;
			}
			else
			{
				TerminalDisplayLoop->Task=3;
			}
		}
	}
	else
	{//下移
		if(TerminalDisplayLoop->Count<60)
		{
			TerminalDisplayLoop->Count++;//当前循环显示任务已显屏计数
		}
		else
		{
			TerminalDisplayLoop->Count=0;//当前循环显示任务已显屏计数
			if(TerminalDisplayLoop->Task<3)
			{
				TerminalDisplayLoop->Task++;
			}
			else
			{
				TerminalDisplayLoop->Task=0;
			}
		}
	}
}

#include "../MS/MS.h"
void DisplayLoop(void)//终端循显
{
	u32 i;
	u32 Row;
//	u8 *p8;
//	UARTCtrl_TypeDef * UARTCtrl;
//void Uart_3762_RxData(u32 PORTn);//UART_3762接收数据;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收

//全显定时
	if(Comm_Ram->DisplayAllErrorTimer)//上电全显或系统错误显示秒定时器
	{
		return;
	}
	if(Comm_Ram->ButtonVal&0x23)
	{
		//按键
		TerminalDisplayLoop->ButtonVal=Comm_Ram->ButtonVal;
		Comm_Ram->ButtonVal&=0xdc;//使用后清0
		Comm_Ram->DisplayLoopTimer=60;//循显秒定时器
		
		TerminalDisplayLoopCount();//终端循显计数
		
		if(TerminalDisplayLoop->ButtonVal&0x1)
		{
			DOTMIXPMODE->FlashMode=1;//动画模式:0=无,1=下移,2=上移,3=左移,4=右移,5=...
			DOTMIXPMODE->FlashDelay=1;//动画变化延时(单位10ms),0最快,255最慢
			DOTMIXPMODE->FlashTask=1;//动画任务:0=无,1=开始,2=播放
		}
		if(TerminalDisplayLoop->ButtonVal&0x2)
		{
			DOTMIXPMODE->FlashMode=2;//动画模式:0=无,1=下移,2=上移,3=左移,4=右移,5=...
			DOTMIXPMODE->FlashDelay=1;//动画变化延时(单位10ms),0最快,255最慢
			DOTMIXPMODE->FlashTask=1;//动画任务:0=无,1=开始,2=播放
		}
		if(TerminalDisplayLoop->ButtonVal&0x20)
		{//ESC
			TerminalDisplayLoop->Task=0;//当前循环显示任务
			TerminalDisplayLoop->Count=0;//当前循环显示任务已显屏计数
			if(Comm_Ram->ErrorIC)
			{
				Comm_Ram->DisplayAllErrorTimer=10;//显示错误
			}
//---每当ESC键按下时设置测试用数据---
//模拟电能表主动上报
//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//UARTCtrl->Task=1;//接收完成
//p8rxbuff=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);		
//p8rxbuff[4]=0;//通信模块标识：0表示对集中器的通信模块操作,1表示对载波表的通信模块操作
//p8rxbuff[10]=6;//AFN
//p8rxbuff[11]=0x10;//Fn;
//p8rxbuff[12]=0;//Fn*8;
//p8rxbuff[13+3+1]=0x11;//表地址
//p8rxbuff[13+3+2]=0x11;//表地址
//p8rxbuff[13+3+3]=0x11;//表地址
//p8rxbuff[13+3+4]=0x11;//表地址
//p8rxbuff[13+3+5]=0x11;//表地址
//p8rxbuff[13+3+6]=0x11;//表地址
//p8rxbuff[13+3+9]=19;//长度
//p8rxbuff[13+3+10]=0x01;//DI
//p8rxbuff[13+3+11]=0x15;//DI
//p8rxbuff[13+3+12]=0x00;//DI
//p8rxbuff[13+3+13]=0x04;//DI
//MC(0xFF,((u32)p8rxbuff)+13+3+14,12);//次数变化标志
//p8rxbuff[13+3+26]=0xaa;//
//MC(0x01,((u32)p8rxbuff)+13+3+27,96);//次数
//Uart_3762_RxData(RS485_4PORT);//UART_3762接收数据;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收



//------------------------------------
		}
	}
	else
	{
		//循显定时
		if(Comm_Ram->DisplayLoopTimer==0)
		{
			TerminalDisplayLoop->Count++;//当前循环显示任务已显屏计数
			TerminalDisplayLoop->ButtonVal=0;//下移
			Comm_Ram->DisplayLoopTimer=8;//循显定时值秒
			//工厂状态时循显只显提示
			if(Comm_Ram->Factory==0x55)//工厂状态
			{
				TerminalDisplayLoop->Task=0;//告警信息显示
				TerminalDisplayLoop->Count=0;
				Comm_Ram->BackLightTimer=60;//循显定时值秒
			}
		}
	}
	
	DisplayClr();//清屏
	//图标和时钟
	DisplayLoop_TopICO(0);//顶图标和时钟显示
	DisplayLoop_BottomICO(9);//底图标显示

	Row=1;
	for(i=0;i<(4*60);i++)
	{
		switch(TerminalDisplayLoop->Task)
		{//当前循环显示任务
			case 0://告警信息显示
				Row=DisplayLoop_Warning(Row);//告警信息显示,返回下行
				break;
			case 1://内部交采数据显示
				Row=DisplayLoop_ACSample(Row);//内部交采数据显示,返回下行
				break;
			case 2://控制信息显示
				Row=DisplayLoop_Contrl(Row);//终端控制显示
				break;
			case 3://终端信息
				Row=DisplayLoop_Info(Row);//终端信息显示,返回下行
				break;
		}
		if(Row!=1)
		{
			break;
		}
		else
		{//无显示
			TerminalDisplayLoopCount();//终端循显计数
		}
	}

//-------调试显示---------
//	DisplayClr();//清屏
//	DisplayClrRow(1,8);//清屏幕显示的指定行
	//DisplayString(7,0,0,(u8 *)(ADDR_AFN0CF4+1));//显示字符串
	//DisplayString(8,0,0,(u8 *)(ADDR_AFN0CF4+1+16));//显示字符串
//	MR(ADDR_DATABUFF,ADDR_04010001,6);
//	DisplayRam(7,ADDR_Terminal_Period);//显示中RAM数据(调试用)
	
//	p8=(u8*)ADDR_DATABUFF;
//	p8[0]=UART1Ctrl->Lock;
//	p8[1]=UART1Ctrl->ATTask;
//	p8[2]=UART1Ctrl->Task;
//	p8[3]=FileDownload->res3;
//	p8[4]=0;
//	p8[5]=0;
//	p8[6]=0;
//	DisplayRam(1,ADDR_IA);//显示中RAM数据(调试用)
//	DisplayRam(2,ADDR_IB);//显示中RAM数据(调试用)
//	DisplayRam(3,ADDR_IC);//显示中RAM数据(调试用)
//	DisplayRam(4,ADDR_PA);//显示中RAM数据(调试用)
//	DisplayRam(5,ADDR_PB);//显示中RAM数据(调试用)
//	DisplayRam(6,ADDR_PC);//显示中RAM数据(调试用)
//	DisplayRam(7,ADDR_PT);//显示中RAM数据(调试用)
//	DisplayRam(8,ADDR_Measure+1);//显示中RAM数据(调试用)

//DisplayRam(5,ADDR_UA);//显示中RAM数据(调试用)
//DisplayRam(6,ADDR_UB);//显示中RAM数据(调试用)
//DisplayRam(7,ADDR_UC);//显示中RAM数据(调试用)
//DisplayRam(8,ADDR_VDD5V0);//显示中RAM数据(调试用)

//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//MC(0,ADDR_DATABUFF,16);
//p8=(u8*)ADDR_DATABUFF;
//for(i=0;i<UARTCtrl->NUM_SomeTaskID;i++)
//{
//	p8[i]=UARTCtrl->SomeTaskid[i]>>8;
//}
//DisplayRam(8,ADDR_DATABUFF);//显示中RAM数据(调试用)



//-------调试显示结束------------
}







