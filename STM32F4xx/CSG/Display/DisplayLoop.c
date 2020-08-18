
//终端循显
#include "../Hcsg/ProjectCSG.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayLoop_Warning.h"
#include "../Display/DisplayLoop_Info.h"
#include "../Display/DisplayLoop_ACSample.h"
#include "../Display/DisplayLoop_Contrl.h"
#include "../Display/DisplayLoop_Self.h"
#include "../Display/DisplayLoop.h"
#include "../Display/Display.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"





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


void DisplayLoop(void)//终端循显
{
	u32 i;
	u32 Row;
//	u8 * p8rxbuff;
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
		{//ESC 取消键
		#if ((Project/100)==2)
			TerminalDisplayLoop->Task=4;//当前循环显示任务         //取消键按下时南网I型集中器显示告警信息，方便调试
		#else
			TerminalDisplayLoop->Task=0;//当前循环显示任务
		#endif
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

//

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
				#if ((Project/100)==3)
					#if (DLT500_REPORT == 0)
						Row=DisplayLoop_Warning(Row);//告警信息显示,返回下行
					#endif
				#endif
				break;
			case 1://内部交采数据显示
				#if ((Project/100)==3)||((Project/100)==9)
					Row=DisplayLoop_ACSample(Row);//内部交采数据显示,返回下行
				#endif
				#if ((Project/100)==2)
					NWJZQDisplayDefaultInterface();			
				#endif					
				break;
			case 2://控制信息显示
//			#if ((Project/100)==3)
//				Row=DisplayLoop_Contrl(Row);//终端控制显示
//			#endif
				break;
			case 3://终端信息
//			#if ((Project/100)==3)
//				Row=DisplayLoop_Info(Row);//终端信息显示,返回下行
//			#endif
				break;		
		#if ((Project/100)==2)
			case 4://告警信息显示
				Row=DisplayLoop_Warning(Row);//告警信息显示,返回下行	
				break;
		#endif
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
	
//调试显示
//	DisplayClr();//清屏
//	DisplayClrRow(1,8);//清屏幕显示的指定行
	//DisplayString(7,0,0,(u8 *)(ADDR_AFN0CF4+1));//显示字符串
	//DisplayString(8,0,0,(u8 *)(ADDR_AFN0CF4+1+16));//显示字符串
	
//	DisplayRam(8,ADDR_BKPSDRAM);//显示中RAM数据(调试用)
//	DisplayRam(8,ADDR_F203_2+4);//显示中RAM数据(调试用)
	
//调试显示结束
}


void NWJZQDisplayDefaultInterface(void)//南网集中器显示默认界面
{
	u8 *p;
	u8 *p8;
	u32 i;
	u32 Ln = 0;
	
		
	DisplayString(3,2,0,(u8 *)"中国南方电网公司");//显示字符串	
	//日期
	p8=(u8 *)ADDR_MYMDHMS;
	p=(u8 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
	p[0*2]=0x32;
	p[1*2]=0x30;
	p[2*2]=(p8[5]>>4)+0x30;//年
	p[3*2]=(p8[5]&0xf)+0x30;
	p[4*2]='-';
	p[5*2]=(p8[4]>>4)+0x30;//月
	p[6*2]=(p8[4]&0xf)+0x30;
	p[7*2]='-';
	p[8*2]=(p8[3]>>4)+0x30;//日
	p[9*2]=(p8[3]&0xf)+0x30;
	for(i=0;i<=9;i++)
	{
		p[i*2+1]=0x20;
	}
	//时间
	p=(u8 *)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
	p[0*2]=(p8[2]>>4)+0x30;//时
	p[1*2]=(p8[2]&0xf)+0x30;
	p[2*2]=':';
	p[3*2]=(p8[1]>>4)+0x30;//分
	p[4*2]=(p8[1]&0xf)+0x30;
	p[5*2]=':';
	p[6*2]=(p8[0]>>4)+0x30;//秒
	p[7*2]=(p8[0]&0xf)+0x30;
	for(i=0;i<=7;i++)
	{
		p[i*2+1]=0x20;
	}
}






