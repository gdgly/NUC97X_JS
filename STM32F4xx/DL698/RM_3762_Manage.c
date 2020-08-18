

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_3762_Manage.h"
#include "../DL698/RM_3762.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/AutoSearchMeter.h"
#include "../DL698/MeterAutoReport.h"
#include "../DL698/Info_3762.h"


#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
void RM_3762_Manage(void)//载波3762抄表管理
{
	u32 x;
	u32 y;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	
//日更新
	if((UARTCtrl->Task==0)&&(UARTCtrl->Lock!=0x55))//0=空闲
	{
		x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//原月日
		y=Comm_Ram->MYMDHMS[3]+(Comm_Ram->MYMDHMS[4]<<8);//当前月日
		if(x!=y)
		{
			UARTCtrl->RTCD[0]=y;
			UARTCtrl->RTCD[1]=y>>8;
	#if (USER/100)==0//电科院测试
			//电科院测试过日不重初始化,免通信次数测试时返回否认,同时循抄标志不清0而继续原来循抄
			Terminal_Router->RouterCtrl=1;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
	#else
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	#endif
		}
	}
	
//路由模块
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//模块拨出
		Terminal_Router->NoteNum=0;//已知总从节点数
		Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	}
	x=Uart_3762_RxTx(RS485_4PORT);//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收;返回:0=正常返回,1=代理返回
	Terminal_RouterCtrl(RS485_4PORT);//路由器控制;重启,暂停,恢复,检测
	if(Terminal_Router->RouterCtrl!=0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
	{
		return;
	}
//
	
	if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
		//其他任务
		
		//代理任务
		if(UARTCtrl->ProxyTask>=2)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
		{
			if(UARTCtrl->Task==6)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
				UARTCtrl->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			}
			return;
		}
		//广播校时任务
		if((UARTCtrl->BroadcastTimeTask&0x7f)>=2)//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
		{
			if(UARTCtrl->Task==6)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
				UARTCtrl->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			}
			BroadcastTime_3762(RS485_4PORT);//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			return;
		}
		//自动搜表任务
		if(UARTCtrl->AutoSearchTask>=2)//自动搜表任务:0=空,1=等待,2...过程
		{
			if(UARTCtrl->Task==6)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
				UARTCtrl->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			}
			p8=(u8*)ADDR_6002_10;
			p8[0]=DataType_enum;
			p8[1]=1;//空闲（0），搜表中（1） 
			AutoSearchMeter_3762(RS485_4PORT);//自动搜表任务-3762:0=空,1=等待,2...过程
			return;
		}
		else
		{
			p8=(u8*)ADDR_6002_10;
			p8[0]=DataType_enum;
			p8[1]=0;//空闲（0），搜表中（1） 
		}
		//电能表主动上报任务
		if(UARTCtrl->AutoReportTask>=2)//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
		{
			if(UARTCtrl->Task==6)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
				UARTCtrl->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			}
			MeterAutoReport_3762(RS485_4PORT);//电能表主动上报任务3762:0=空,1=有主动上报等待读取,2...抄读中
			return;
		}
	//读信息
		READ_Info();//宽带载波按F209_6设置的TI周期读网络拓扑信息,多网络信息等(不需要周期读取时可设置读取的TI周期=0)
		if(UARTCtrl->InfoTask>=2)//读路由网络拓扑信息等任务:0=空,1=等待,2...过程
		{
			if(UARTCtrl->Task==6)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			{
				UARTCtrl->Task=5;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
			}
			return;
		}
		
	}
//抄表
	if(x)
	{//0=正常返回,1=代理返回
		//代理返回下循环后进入抄表免中间插入抄表帧
		return;
	}
	RM_3762(RS485_4PORT);//终端抄表Q/GDW376.2
	
}
#endif



