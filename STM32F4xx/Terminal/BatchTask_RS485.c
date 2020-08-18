

//数据转发批量任务-RS485
#include "Project.h"
#include "BatchTask_RS485.h"
#include "../terminal/Terminal_Uart_3761.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "../terminal/Terminal_ReadMeter_DL645.h"
#include "../terminal/Terminal_AFN0E_Event.h"
#include "../terminal/Terminal_AFN0C_RealTimeData_Fn.h"
#include "../Display/Warning.h"


void BatchTask_RS485_SETRTC(u32 PORTn)//数据转发批量任务-RS485-时钟对时
{
	u32 i;
	u32 x;
	u32 PORT485;
	u64 d64a;
	u64 d64b;
	u64 Random;
	u64 Key;
	
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
#if Extern_BatchTask_RS485==0//外部程序:0=I2,1=E1,2=E2,3=
	if(UARTCtrl->BatchTaskLock==0x55)//0x55=转发批量任务正在被调用禁止其他程序再次调用
	{
		return;
	}
	i=MRR(ADDR_AFN05F30+1,1);//终端对电能表对时功能的开启与关闭
	if(i!=1)
	{//0=关闭，1=开启，其它值关闭
		if(UARTCtrl->BatchTask>=2)//转发批量任务:0=空,1=等待口空闲启动,2...过程
		{
			Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			if(UARTCtrl->Task<=2)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				UARTCtrl->Task=0;//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
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
	
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			PORT485=2;
			break;
		case RS485_2PORT://RS485-2
			PORT485=3;
			break;
		case RS485_3PORT://RS485-3
			PORT485=4;
			break;
		default:
			PORT485=32;
			return;
	}
	switch(UARTCtrl->BatchTask)//转发批量任务:0=空,1=等待口空闲启动,2...过程
	{
		case 0://空
#if Extern_BatchTask_RS485==0//外部程序:0=I2,1=E1,2=E2,3=
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
#endif
			break;
		case 1://等待口空闲启动
			x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//取任务类型1(对时任务)配置电表数量n
			if(UARTCtrl->BatchMeterCount>=x)
			{//转发批量任务计数>=配置电表数量
				UARTCtrl->BatchTask=0;//转发批量任务:0=空,1=等待口空闲启动,2...过程
				return;
			}
			i=MRR(ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);
			if(i==0)
			{//0=没完成,1=完成,其它值=执行错误
				d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
				i=Check_AFN04F10_Addr(PORT485,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
				if(i)
				{//有相同地址的配置
					if(UARTCtrl->Lock!=0x55)//0x55=通信功能正在被调用禁止其他程序再次调用
					{
						if(UARTCtrl->Task==0)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						{
							UART7Ctrl->BatchTaskLock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
							UARTCtrl->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
							UARTCtrl->ReRMCount=0;//485抄表命令重发计数
							p16timer[0]=50/10;//下帧启动发送延时50MS后
							UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
						}
					}
					return;
				}
			}
			UARTCtrl->BatchMeterCount++;//转发批量任务电表计数
			break;
	NEXT://空闲1次以便主站其他任务,然后等待下只表对时
			UARTCtrl->BatchMeterCount++;//转发批量任务电表计数
			UARTCtrl->BatchTask=1;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			UART7Ctrl->BatchTaskLock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
			UARTCtrl->Task=0;//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
			UARTCtrl->RMCount=0;//485抄表命令计数
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
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
			MC(0xfe,((u32)p8txbuff),4);
			Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x04000402,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
			UARTCtrl->TxByte=4+16;//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//发送后接收超时定时值ms/10(半字对齐)
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
		Tx:
			//通信速率
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			i=Check_AFN04F10_Addr(PORT485,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
			if(i==0)
			{
				goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
			}
			i=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//默认2400
				UARTCtrl->BpsCtrl=(3<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}
			UARTCtrl->Task=3;//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收

			//显示对时地址
			CopyString((u8 *)"485x对时",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			MWR((PORT485-1),ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8txbuff[5+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			break;
		case 5://读表号接收
			if(UARTCtrl->Task>=3)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->Task!=1)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{//超时
				goto ReTx3;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
			if(i!=0)
			{//错误
				if((i==1)||(i==3))
				{//地址或数据标识错,电科院测试软件模拟表会停止但接收数据正常,恢复模拟表后会将收到的帧同时发出
					//继续接收
					p16timer[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
					UARTCtrl->RxByte=0;
					UARTCtrl->Task=5;//5=正在接收
					break;
				}
			ReTx3:
				if(UARTCtrl->ReRMCount<2)//重发计数
				{//电科院测试台此时会设模拟表,增加重发次数以等待模拟表
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=4+16;//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//发送后接收超时定时值ms/10(半字对齐)
					goto Tx;
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
			MC(0xfe,(u32)p8txbuff,4);
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
			UARTCtrl->TxByte=4+10+32+2;//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=10000/10;//发送后接收超时定时值ms/10(半字对齐)
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			goto Tx;
		case 7://	身份认证接收
			if(UARTCtrl->Task>=3)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
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
			if(UARTCtrl->Task!=1)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{//超时
				goto ReTx7;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
			if(i!=0)
			{//错误
				if((i==1)||(i==3))
				{//地址或数据标识错,电科院测试软件模拟表会停止但接收数据正常,恢复模拟表后会将收到的帧同时发出
					//继续接收
					p16timer[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
					UARTCtrl->RxByte=0;
					UARTCtrl->Task=5;//5=正在接收
					break;
				}
			ReTx7:
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=4+10+32+2;//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=10000/10;//发送后接收超时定时值ms/10(半字对齐)
					goto Tx;
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
		case 8://对时发送
			p8txbuff[0]=1;//任务类型
			MWR(0x050032000507,((u32)p8txbuff)+1,6);
			Data_Inverse(((u32)p8txbuff)+1,6);//数据倒序(高低字节调换),入口Le>=2
			MR(((u32)p8txbuff)+7,ADDR_TYMDHMS,3);//终端当前时间YMDWHMS
			MR(((u32)p8txbuff)+7+3,ADDR_TYMDHMS+6,1);//终端当前时间YMDWHMS
			MR(((u32)p8txbuff)+7+4,ADDR_TYMDHMS+3,3);//终端当前时间YMDWHMS
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
			UARTCtrl->TxByte=4+10+32+2;//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=10000/10;//发送后接收超时定时值ms/10(半字对齐)
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			goto Tx;
		case 9://对时接收
			if(UARTCtrl->Task>=3)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->Task!=1)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{//超时
				goto ReTx9;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
			if((i!=0)&&(i!=3))
			{//错误
			ReTx9:
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=4+10+32+2;//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=10000/10;//发送后接收超时定时值ms/10(半字对齐)
					goto Tx;
				}
				else
				{
					MWR(2,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//对时完成标志:0=没完成,1=完成,其它值=执行错误
					goto NEXT;//空闲1次以便主站其他任务,然后等待下只表对时
				}
			}
			MWR(1,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//对时完成标志:0=没完成,1=完成,其它值=执行错误
			UARTCtrl->ReRMCount=0;//485抄表命令重发计数
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			p16timer[0]=1000/10;//延时等待电表生成校时记录
			break;
		case 10://延时等待电表生成校时记录
			if(p16timer[0]==0)
			{
				UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			}
			break;
		case 11://读上1次校时记录
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			MC(0xfe,((u32)p8txbuff),4);
			Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x03300401,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
			UARTCtrl->TxByte=4+16;//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//发送后接收超时定时值ms/10(半字对齐)
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			goto Tx;
		case 12://读上1次校时记录接收
			if(UARTCtrl->Task>=3)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->Task!=1)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{//超时
				goto ReTx12;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
			if(i!=0)
			{//错误
				if((i==1)||(i==3))
				{//地址或数据标识错,电科院测试软件模拟表会停止但接收数据正常,恢复模拟表后会将收到的帧同时发出
					//继续接收
					p16timer[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
					UARTCtrl->RxByte=0;
					UARTCtrl->Task=5;//5=正在接收
					break;
				}
			ReTx12:
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=4+16;//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//发送后接收超时定时值ms/10(半字对齐)
					goto Tx;
				}
				else
				{
					MC(0xee,((u32)p8rxbuff)+14+4,12);//对时前后电表时间为空
				}
			}
			//1类数据F166 对时前时间对时后时间
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			i=Check_AFN04F10_Addr(PORT485,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
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
		case 13://读电表校时总次数
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			MC(0xfe,((u32)p8txbuff),4);
			Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x03300400,d64a);//DL645-2007读数据帧,入口:要填入的缓冲地址,数据标识,通信地址
			UARTCtrl->TxByte=4+16;//发送字节数(半字对齐)
			UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//发送后接收超时定时值ms/10(半字对齐)
			UARTCtrl->BatchTask++;//转发批量任务:0=空,1=等待口空闲启动,2...过程
			goto Tx;
		case 14://读电表校时总次数接收
			if(UARTCtrl->Task>=3)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				break;
			}
			if(UARTCtrl->Task!=1)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{//超时
				goto ReTx14;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
			if(i!=0)
			{//错误
				if((i==1)||(i==3))
				{//地址或数据标识错,电科院测试软件模拟表会停止但接收数据正常,恢复模拟表后会将收到的帧同时发出
					//继续接收
					p16timer[0]=ReadDL645OverTimer/10;//DL645读表超时定时值ms
					UARTCtrl->RxByte=0;
					UARTCtrl->Task=5;//5=正在接收
					break;
				}
			ReTx14:
				if(UARTCtrl->ReRMCount<1)//重发计数
				{
					UARTCtrl->ReRMCount++;//重发计数+1
					UARTCtrl->TxByte=4+16;//发送字节数(半字对齐)
					UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//发送后接收超时定时值ms/10(半字对齐)
					goto Tx;
				}
				else
				{
					MC(0xee,((u32)p8rxbuff)+14,3);//电表校时总次数为空
				}
			}
			//1类数据F166 电表校时总次数
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//取表通信地址
			i=Check_AFN04F10_Addr(PORT485,d64a);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
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











