
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"





u32 RM_TxDATA_Landis(u32 PORTn,u8 *p8CSD,u8 *p8tx)//Landis(IEC1107 模式C)抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
{
	u32 i;
	u32 n;
	u32 x;
	u32 LEN_DATA;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_DATA=0;
	switch(UARTCtrl->CONNECT)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
	{
		default://698规约连接和关闭过程
			UARTCtrl->CONNECT=0;
			return 0;
		case 1://300bps握手
			//在300Bd 下，发 “/?电表地址！CR LF” 命令
			p8tx[0]='/';
			p8tx[1]='?';
			LEN_DATA=2;
			for(i=1;i<3;i++)
			{
				x=UARTCtrl->RMAddr[i];
				for(n=0;n<2;n++)
				{
					p8tx[LEN_DATA]=((x>>4)&0xf)+0x30;
					LEN_DATA++;
					p8tx[LEN_DATA]=((x>>0)&0xf)+0x30;
					LEN_DATA++;
					x>>=8;
				}
			}
			p8tx[LEN_DATA]='!';
			p8tx[LEN_DATA+1]=0x0d;
			p8tx[LEN_DATA+2]=0x0a;
			LEN_DATA+=3;
			UARTCtrl->BpsCtrl=//通信控制字(同规约2007)(半字对齐)
							//D8-D5位表示波特率,O～7依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
							//D4位表示停止位位数0/1分别表示1/2个停止位
				(1<<3)|//D3位表示有无校验位0/1分别表示无/有
							//D2位表示偶／奇校验位0/1分别表示偶／奇校验
				2;		//D1-D0 数据位数0～3分别表示5-8位
			return LEN_DATA;
		case 101://抄表中
			i=UARTCtrl->TaskID;
			switch((i>>8)&0xff)//任务号
			{
				case RMTASKnoPOWER://停上电事件固定任务号
					return 0;
				default:
					break;
			}
			switch((i>>24)&0xf)//方案类型
			{
				case 1://普通采集方案
					switch((UARTCtrl->TaskID>>20)&0xf)//采集类型
					{
						default:
						case 1://1=采集上第N次
							return 0;
						case 0://0=采集当前数据
						case 2://2=按冻结时标采集
							//发 “ACK 0 x 0 CR LF” 命令
							p8tx[0]=6;//ACK
							p8tx[1]='0';
							i=UARTCtrl->BpsCtrl;
							i>>=0xf;
							i&=0x03;
							switch(i)
							{
								default:
								case 0://300
									p8tx[2]='0';
									break;
								case 1://600
									p8tx[2]='1';
									break;
								case 2://1200
									p8tx[2]='2';
									break;
								case 3://2400
									p8tx[2]='3';
									break;
								case 4://4800
									p8tx[2]='4';
									break;
								case 6://9600
									p8tx[2]='5';
									break;
							}
							p8tx[3]='0';
							p8tx[4]=0x0d;
							p8tx[5]=0x0a;
							LEN_DATA=6;
							break;
						case 3://3=按时标间隔采集
							//采曲线数据,按任务执行间隔时间和采集方案采集间隔时间计算
							//发 “ACK 0 x 0 CR LF” 命令
							p8tx[0]=6;//ACK
							p8tx[1]='0';
							i=UARTCtrl->BpsCtrl;
							i>>=0xf;
							i&=0x03;
							switch(i)
							{
								default:
								case 0://300
									p8tx[2]='0';
									break;
								case 1://600
									p8tx[2]='1';
									break;
								case 2://1200
									p8tx[2]='2';
									break;
								case 3://2400
									p8tx[2]='3';
									break;
								case 4://4800
									p8tx[2]='4';
									break;
								case 6://9600
									p8tx[2]='5';
									break;
							}
							p8tx[3]='0';
							p8tx[4]=0x0d;
							p8tx[5]=0x0a;
							LEN_DATA=6;
							break;
						case 4://4=RetryMetering 补抄(类同,2=按冻结时标采集)
							return 0;
					}
					break;
				case 2://事件采集方案
				case 3://透明方案
					return 0;
				default:
					return 0;
			}
			return LEN_DATA;
		case 102://关闭连接
			//
			return 0;
	}
}












