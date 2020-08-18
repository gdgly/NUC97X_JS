
//数据转发批量任务-载波3762
#include "Project.h"
#include "BatchTask_3762.h"
#include "Terminal_Uart_3761.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_Uart_3762_RxTx.h"
#include "../Display/Warning.h"


#if (Project/100)!=2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
void BatchTask_3762_SETRTC(u32 PORTn)//数据转发批量任务-3762-时钟对时
{
}
#else
void BatchTask_3762_SETRTC(u32 PORTn)//数据转发批量任务-3762-时钟对时
{
	u32 i;
	u32 x;
	u64 d64a;
	u64 d64b;
	u64 Random;
	u64 Key;
	
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

#if Extern_BatchTask_3762==0//外部程序:0=I2,1=E1,2=E2,3=
	if(UARTCtrl->BatchTaskLock==0x55)//0x55=转发批量任务正在被调用禁止其他程序再次调用
	{
		return;
	}
	i=MRR(ADDR_AFN05F30+1,1);//终端对电能表对时功能的开启与关闭
	if(i!=1)
	{//0=关闭，1=开启，其它值关闭
		if(UARTCtrl->BatchTask>=2)//转发批量任务:0=空,1=等待口空闲启动,2...过程
		{
			Uart_DL645_RxTx(PORTn);//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(UARTCtrl->Task<=2)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			}
		}
		else
		{
			UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
		}
		return;
	}
#endif

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(UARTCtrl->BatchTask)//转发批量任务:0=空,1=等待口空闲启动,2...过程
	{
		case 0://空
		
			x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//取任务类型1(对时任务)配置电表数量n
			if(x==0)
			{//配置电表数量n=0
				UARTCtrl->BatchMeterCount=0;//转发批量任务电表计数
				return;
			}
			if(x>MaxRS485AddCarrierPn)
			{//配置电表数量>MaxRS485AddCarrierPn
				x=MaxRS485AddCarrierPn;
			}
			if(UARTCtrl->BatchMeterCount<x)
			{//转发批量任务计数<配置电表数量
				UARTCtrl->BatchTask=1;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			}
		
			break;
		case 1://等待口空闲启动
			x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//取任务类型1(对时任务)配置电表数量n
			if(UARTCtrl->BatchMeterCount>=x)
			{//转发批量任务计数>=配置电表数量				
				UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
				Terminal_Router->StopFlags&=~(1<<9);//恢复路由
				Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				
				return;
			}
			i=MRR(ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);
			if(i==0)
			{//0=没完成,1=完成,其它值=执行错误
				d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
				i=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(i)
				{//有相同地址的配置
					if(UARTCtrl->Lock!=0x55)//0x55=通信功能正在被调用禁止其他程序再次调用
					{
						if(UARTCtrl->Task==0)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						{
							Terminal_Router->StopFlags|=(1<<9);//暂停路由
							Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
							UART7Ctrl->BatchTaskLock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
							UARTCtrl->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
							UARTCtrl->ReRMCount=0;//485抄表命令重发计数
							UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
						}
					}
					return;
				}
			}
			UARTCtrl->BatchMeterCount++;//转发批量任务电表计数
			break;
	NEXT://空闲1次以便主站其他任务,然后等待下只表对时
			UARTCtrl->BatchTask=1;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			UART7Ctrl->BatchTaskLock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
			UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->BatchMeterCount++;//转发批量任务电表计数
			return;
		case 2://等测试台设模拟表延时
			if(UARTCtrl->BatchMeterCount==0)
			{
				p16timer[0]=6000/10;//第1只表开始延时10秒后,仅因等测试台设模拟表
			}
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 3://等测试台设模拟表延时
			if(UARTCtrl->BatchMeterCount==0)
			{
				if(p16timer[0]!=0)//第1只表开始延时10秒后,仅因等测试台设模拟表
				{
					break;
				}
			}
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 4://发送读表号
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			Read_DL645_2007_TxData(ADDR_DATABUFF,0x04000402,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,6,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{//微功率无线,档案地址用采集器地址
				x=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
						Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					}
				}
			}
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			
			UARTCtrl->TxByte=(31+16);//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			//显示对时地址
			CopyString((u8 *)"载波对时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[30+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case 5://读表号接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2监控从节点接收数据帧取出标准645帧;返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
			if(i!=0)
			{//错误
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=(31+16);//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
					UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					return;
				}
				else
				{
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			//正确,保存表号
			MR((u32)&UARTCtrl->BatchMeterNo,((u32)p8rxbuff)+14,6);
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 6://身份认证发送
			//取终端TESAM随机数
			UARTCtrl->BatchTaskLock=0x55;//0x55=转发批量任务正在被调用禁止其他程序再次调用
			i=TESAM_CMD_TxRx(0x801A1000,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			UARTCtrl->BatchTaskLock=0;//0x55=转发批量任务正在被调用禁止其他程序再次调用
			if(i)
			{
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					return;
				}
				else
				{
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			d64a=MRR(ADDR_UART7Rx+5,8);
			d64b=MRR(ADDR_UART7Rx+5+8,8);
			Random=d64a^d64b;
			//取密钥密文
			x=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData,2);//取任务类型0(身份认证任务)配置电表数量n
			//先找单个认证
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),8);//取表通信地址
			for(i=0;i<x;i++)
			{
				d64b=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData+2+(40*i),8);//取表通信地址
				if(d64a==d64b)
				{
					break;
				}
			}
			if(i>=x)
			{//单个认证没找到,再找通用认证
				for(i=0;i<x;i++)
				{
					d64b=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData+2+(40*i),8);//取表通信地址
					if(d64b==0xaaaaaaaaaaaaaaaa)
					{
						break;
					}
				}
				if(i>=x)
				{//通用认证也没找到
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			MR((u32)p8rxbuff,ADDR_AFN10F12+5+LEN_AFN10F12TaskData+2+(40*i)+8,32);//取电表密钥
			Data_Inverse((u32)p8rxbuff,32);//数据倒序(高低字节调换),入口Le>=2
			UARTCtrl->BatchTaskLock=0x55;//0x55=转发批量任务正在被调用禁止其他程序再次调用
			if(d64b!=0xaaaaaaaaaaaaaaaa)
			{//单地址
				x=32+8+8;//发给TESAM数据长度
				//8BYTE表号
				//MWR(d64a,((u32)p8rxbuff)+32,8);//用通信地址
				MW((u32)&UARTCtrl->BatchMeterNo,((u32)p8rxbuff)+32,6);//表号
				MWR(0,((u32)p8rxbuff)+32+6,2);
				Data_Inverse(((u32)p8rxbuff)+32,8);//数据倒序(高低字节调换),入口Le>=2
				//8BYTE随机数
				MWR(Random,((u32)p8rxbuff)+32+8,8);
				i=TESAM_CMD_TxRx(0x841C0001,x,(u32)p8rxbuff);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			}
			else
			{//通用地址
				x=32+2+6+2+6+8;//发给TESAM数据长度
				//2BYTE任务数据
				MR(((u32)p8rxbuff)+32,ADDR_AFN10F12+5,2);
				Data_Inverse(((u32)p8rxbuff)+32,2);//数据倒序(高低字节调换),入口Le>=2
				//6BYTE表号
				//MWR(d64a,((u32)p8rxbuff)+32+2,6);
				MW((u32)&UARTCtrl->BatchMeterNo,((u32)p8rxbuff)+32+2,6);//表号
				Data_Inverse(((u32)p8rxbuff)+32+2,6);//数据倒序(高低字节调换),入口Le>=2
				//2BYTE 0000
				MWR(0,((u32)p8rxbuff)+32+8,2);
				//6BYTE表号
				//MWR(d64a,((u32)p8rxbuff)+32+10,6);
				MW((u32)&UARTCtrl->BatchMeterNo,((u32)p8rxbuff)+32+10,6);//表号
				Data_Inverse(((u32)p8rxbuff)+32+10,6);//数据倒序(高低字节调换),入口Le>=2
				//8BYTE随机数
				MWR(Random,((u32)p8rxbuff)+32+16,8);
				i=TESAM_CMD_TxRx(0x841C0002,x,(u32)p8rxbuff);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			}
			UARTCtrl->BatchTaskLock=0;//0x55=转发批量任务正在被调用禁止其他程序再次调用
			if(i)
			{
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					return;
				}
				else
				{
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			Key=MRR(ADDR_UART7Rx+5,8);
			d64b=MRR(ADDR_UART7Rx+5+8,8);
			Key^=d64b;//密文
			//645报文
			MWR(d64a,((u32)p8txbuff)+4+1,6);
			p8txbuff[4+8]=0x03;//控制码
			p8txbuff[4+9]=4+4+8+8+8;//数据域长度
			//FF000007 78563412 77FDC349F9A7F0FD d29847bb16a776d2 0100785634120000
			MWR(0x070000FF,((u32)p8txbuff)+4+10,4);//数据标识
			MWR(0x12345678,((u32)p8txbuff)+4+10+4,4);//操作者代码
			MWR(Key,((u32)p8txbuff)+4+10+8,8);//密文
			Data_Inverse(((u32)p8txbuff)+4+10+8,8);//数据倒序(高低字节调换),入口Le>=2
			MWR(Random,((u32)p8txbuff)+4+10+16,8);//随机数
			Data_Inverse(((u32)p8txbuff)+4+10+16,8);//数据倒序(高低字节调换),入口Le>=2
			//MWR(d64a,((u32)p8txbuff)+4+10+24,8);//用地址作的分散因子
			MW((u32)&UARTCtrl->BatchMeterNo,((u32)p8txbuff)+4+10+24,6);//分散因子(表号)
			MWR(0,((u32)p8txbuff)+4+10+30,2);
			DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
			MR(ADDR_DATABUFF,((u32)p8txbuff)+4,10+32+2);
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,10+32+2,16+12,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{//微功率无线,档案地址用采集器地址
				x=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
						Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					}
				}
			}
			
			UARTCtrl->TxByte=31+10+32+2;//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 7://	身份认证接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}	
			//找当前对时任务计数号相对身份认证任务计数号,用于执行结果记录
			x=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData,2);//取任务类型0(身份认证任务)配置电表数量n
			if(x>MaxRS485AddCarrierPn)
			{//配置电表数量>MaxRS485AddCarrierPn
				x=MaxRS485AddCarrierPn;
			}
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),8);//取表通信地址
			for(i=0;i<x;i++)
			{
				d64b=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData+2+(40*i),8);//取表通信地址
				if(d64a==d64b)
				{
					break;
				}
			}
			if(i>=x)
			{//没找到
				x=0xffffffff;
			}
			else
			{
				x=i;
			}
			//
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2监控从节点接收数据帧取出标准645帧;返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
			if(i!=0)
			{//错误
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=31+10+32+2;//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
					UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					return;
				}
				else
				{
					if(x!=0xffffffff)
					{//找到当前对时任务计数号相对身份认证任务计数号
						MWR(2,ADDR_AFN10F13+MaxRS485AddCarrierPn+x,1);//对时完成标志:0=没完成,1=完成,其它值=执行错误
					}
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			if(x!=0xffffffff)
			{//找到当前对时任务计数号相对身份认证任务计数号
				MWR(1,ADDR_AFN10F13+x,1);//身份认证完成标志
			}
			MR((u32)&UARTCtrl->Random,((u32)p8rxbuff)+14,4);//电表身份认证时返回的随机数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 8://预计延迟时间
			//645报文
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),8);//取表通信地址
			MWR(d64a,((u32)p8txbuff)+1,6);
			p8txbuff[8]=0x14;//控制码
			p8txbuff[9]=32;//数据域长度
			//0C 01 00 04（数据标识） 98(PA) 000000(P0P1P2) 78563412(操作者代码倒项) +8B 29 73 C3 09 2F 39 9B 29 18 83 4F 04 74 AD 5D（电表对时任务密文倒项） + 03031C8A（MAC倒项）
			MWR(0x0400010C,((u32)p8txbuff)+10,4);//数据标识
			MWR(0x00000098,((u32)p8txbuff)+10+4,4);//密钥等级
			MWR(0x12345678,((u32)p8txbuff)+10+8,4);//操作者代码
			MR(((u32)p8txbuff)+10+12,ADDR_UART7Rx+5+4,16);//电表对时任务密文
			MR(((u32)p8txbuff)+10+28,ADDR_UART7Rx+5+4+16,4);//MAC
			DL645_68H_Add33H_CS_16H(((u32)p8txbuff));//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
			MR(ADDR_DATABUFF,((u32)p8txbuff),10+32+2);
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,10+32+2,12,1);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{//微功率无线,档案地址用采集器地址
				x=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
						Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					}
				}
			}
			
			UARTCtrl->TxByte=31+10+32+2;//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 9://预计延迟时间接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=MRR(((u32)p8rxbuff)+10,3);
			if((i!=0x414)||(UARTCtrl->Task!=1))
			{//AFN=14H,Fn=3
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=31+10+32+2;//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
					UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					return;
				}
				else
				{
					MWR(2,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//对时完成标志:0=没完成,1=完成,其它值=执行错误
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			UARTCtrl->TransferDelay=MRR(((u32)p8rxbuff)+19,2);//预计延迟时间秒
			MR((u32)p8txbuff,(u32)p8rxbuff,10);//复制10字节接收头到发送头
			
			p8txbuff[3]=1;//控制码为0x1
			p8txbuff[4]=0;//通信模块标识=0
			p8txbuff[10]=0x14;//AFN=0x14
			p8txbuff[11]=0x04;//Fn=3
			p8txbuff[12]=0x00;//Fn=3
			p8txbuff[13]=0;//数据长度=0,放弃本报文
			i=14+2;//需总发送字节数
			p8txbuff[1]=i;
			p8txbuff[2]=i>>8;
			UARTCtrl->TxByte=i;//发送字节数(半字对齐)
			Uart_3762_CS((u32)p8txbuff);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲

			UARTCtrl->TxToRxTimerOver=6000/10;//发送后接收超时定时值ms/10(半字对齐)
			UARTCtrl->Task=3;//等待发送
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 10:
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			//对时发送
			p8txbuff[0]=1;//任务类型
			MWR(0x050032000507,((u32)p8txbuff)+1,6);
			Data_Inverse(((u32)p8txbuff)+1,6);//数据倒序(高低字节调换),入口Le>=2
			//终端当前时间YMDWHMS+预计延迟时间秒
			d64a=MRR(ADDR_TYMDHMS,6);
			d64a=YMDHMS_AddS(d64a,UARTCtrl->TransferDelay);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
			MWR(d64a,((u32)p8txbuff)+7,3);
			MWR(d64a>>24,((u32)p8txbuff)+7+4,3);
			//星期计算
			YMDCalculateW(((u32)p8txbuff)+7);//指定地址缓冲中的年月日周时分秒计算周
			
			Data_Inverse(((u32)p8txbuff)+7,7);//数据倒序(高低字节调换),入口Le>=2
			MR(((u32)p8txbuff)+14,ADDR_AFN10F12+LEN_AFN10F12PerTask+5,40);//任务数据
			Data_Inverse(((u32)p8txbuff)+14,40);//数据倒序(高低字节调换),入口Le>=2
			MR(((u32)p8txbuff)+54,ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount)+8,32);//对时任务中电表密钥密文
			Data_Inverse(((u32)p8txbuff)+54,32);//数据倒序(高低字节调换),入口Le>=2
			MR(((u32)p8txbuff)+86,(u32)&UARTCtrl->Random,4);//电表身份认证时返回的随机数
			Data_Inverse(((u32)p8txbuff)+86,4);//数据倒序(高低字节调换),入口Le>=2
			
			UARTCtrl->BatchTaskLock=0x55;//0x55=转发批量任务正在被调用禁止其他程序再次调用
			i=TESAM_CMD_TxRx(0x84180404,0x5A,(u32)p8txbuff);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			UARTCtrl->BatchTaskLock=0;//0x55=转发批量任务正在被调用禁止其他程序再次调用
			if(i)
			{
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					return;
				}
				else
				{
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			//645报文
			MC(0xfe,(u32)p8txbuff,4);
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),8);//取表通信地址
			MWR(d64a,((u32)p8txbuff)+4+1,6);
			p8txbuff[4+8]=0x14;//控制码
			p8txbuff[4+9]=32;//数据域长度
			//0C 01 00 04（数据标识） 98(PA) 000000(P0P1P2) 78563412(操作者代码倒项) +8B 29 73 C3 09 2F 39 9B 29 18 83 4F 04 74 AD 5D（电表对时任务密文倒项） + 03031C8A（MAC倒项）
			MWR(0x0400010C,((u32)p8txbuff)+4+10,4);//数据标识
			MWR(0x00000098,((u32)p8txbuff)+4+10+4,4);//密钥等级
			MWR(0x12345678,((u32)p8txbuff)+4+10+8,4);//操作者代码
			MR(((u32)p8txbuff)+4+10+12,ADDR_UART7Rx+5+4,16);//电表对时任务密文
			Data_Inverse(((u32)p8txbuff)+4+10+12,16);//数据倒序(高低字节调换),入口Le>=2
			MR(((u32)p8txbuff)+4+10+28,ADDR_UART7Rx+5+4+16,4);//MAC
			Data_Inverse(((u32)p8txbuff)+4+10+28,4);//数据倒序(高低字节调换),入口Le>=2
			DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465帧起始符0x68,数据域加0x33,CS,结尾0x16
			MR(ADDR_DATABUFF,((u32)p8txbuff)+4,10+32+2);
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,10+32+2,12,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{//微功率无线,档案地址用采集器地址
				x=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
						Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					}
				}
			}

			UARTCtrl->TxByte=31+10+32+2;//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 11://对时接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2监控从节点接收数据帧取出标准645帧;返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
			if(i!=0)
			{//错误
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=31+10+32+2;//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
					UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					return;
				}
				else
				{
					MWR(2,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//对时完成标志:0=没完成,1=完成,其它值=执行错误
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			//正确
			MWR(1,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//对时完成标志:0=没完成,1=完成,其它值=执行错误
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			//延时等待电表生成校时记录
			Terminal_Ram->Trip4_MS_Timer=1000/10;//6 终端跳闸4 10MS定时器;1200ms,>=1000跳,<1000合
			break;
		case 12://延时等待电表生成校时记录
			if(Terminal_Ram->Trip4_MS_Timer==0)//6 终端跳闸4 10MS定时器;1200ms,>=1000跳,<1000合
			{
				UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			}
			break;
		case 13://读上1次校时记录
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			Read_DL645_2007_TxData(ADDR_DATABUFF,0x03300401,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,23,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{//微功率无线,档案地址用采集器地址
				x=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
						Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					}
				}
			}
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			
			UARTCtrl->TxByte=(31+16);//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			break;
		case 14://读上1次校时记录接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2监控从节点接收数据帧取出标准645帧;返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
			if(i!=0)
			{//错误
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=(31+16);//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
					UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					return;
				}
				else
				{
					MC(0xee,((u32)p8rxbuff)+14+4,12);//对时前后电表时间为空
				}
			}
			//1类数据F166 对时前时间对时后时间
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			i=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
			if(i)
			{//有相同地址的配置
				x=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+2,2);//测量点号
				i=GetClass1DataAddr(166,x);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
				if(i)
				{
					MR(i+8,((u32)p8rxbuff)+14+4,12);//对时前时间,对时后时间
				}
			}
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			break;
		case 15://读电表校时总次数
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			Read_DL645_2007_TxData(ADDR_DATABUFF,0x03300400,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,23,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{//微功率无线,档案地址用采集器地址
				x=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//采集器地址(目的地址)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//从节点地址(目的地址)
						Uart_3762_CS((u32)p8txbuff);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					}
				}
			}
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			
			UARTCtrl->TxByte=(31+16);//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			break;
		case 16://读电表校时总次数接收
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2监控从节点接收数据帧取出标准645帧;返回0正确,1地址错,2控制码错,3,4帧3762错,5超时,255其他错
			if(i!=0)
			{//错误
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=(31+16);//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
					UARTCtrl->Task=3;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					return;
				}
				else
				{
					MC(0xee,((u32)p8rxbuff)+14,3);//电表校时总次数为空
				}
			}
			//1类数据F166 电表校时总次数
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			i=Check_AFN04F10_Addr(31,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
			if(i)
			{//有相同地址的配置
				x=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+2,2);//测量点号
				i=GetClass1DataAddr(166,x);//取类1数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
				if(i)
				{
					MR(i+5,((u32)p8rxbuff)+14,3);//电表校时总次数
					MR(i,ADDR_TYMDHMS+1,5);//修改终端抄表时间
					//ERC41对时事件
					MWR(x,ADDR_DATABUFF+2,2);
					MR(ADDR_DATABUFF+4,i+8,12);//对时前时间,对时后时间
					ERC41_Event(ADDR_DATABUFF);//对时事件记录;入口:ADDR_BUFF的第2字节开始分别为2BYTE测量点号,6BYTE对时前时间,6BYTE对时后时间
				}
			}		
			goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
		default:
			goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
	}
}

#endif










