
//控制模块通信
#include "Project.h"
#include "Terminal_Contrl_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"



#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void Terminal_Contrl_Uart_RxTx(u32 PORTn)//控制模块通信RxTx
{
	u32 i;
	u32 RxByte;
	u32 LEN_UARTnIntRx;
	u32 LEN_UARTnRx;
	u32 p0;
	u32 p1;
	u8 *p8;
	u16 *p16;
//	u32 *p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
//初始化
	switch(UARTCtrl->LinkTask)
	{
		case 0://上电或复位
			UARTCtrl->Task=0;//控制模块接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			i=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);//时分
			Pin_Configure(PIN_CTR_EN,//关控制模块输出允许
									0|//B3-B0:复用功能选择AF(0-15)
						 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
						 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
						(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
			if(i<0x0010)
			{//是小于0时10分,关电源
				Pin_Configure(PIN_PLC_PWR,
										0|//B3-B0:复用功能选择AF(0-15)
							 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
							 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
							 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
							 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
							(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
			}
			else
			{//不关电源
				Pin_Configure(PIN_PLC_PWR,
										0|//B3-B0:复用功能选择AF(0-15)
							 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
							 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
							 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
							 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
							(1<<11));//B11  :当为输出时：0=输出0,1=输出1	
			}
			p16timer[0]=3000/10;//复位延时
			UARTCtrl->LinkTask=1;
			return;
		case 1://复位解除
			if(p16timer[0]==0)
			{
				Pin_OutH(PIN_PLC_PWR);//引脚输出高,入口引脚使用名
				p16timer[0]=2000/10;//模块自行初始化延时
				UARTCtrl->BpsCtrl=0xb+(7<<5);//19200,8e1
				Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
				UARTCtrl->LinkTask=2;
			}
			return;
		case 2://模块自行初始化延时
			if(p16timer[0]==0)
			{
				UARTCtrl->LinkTask=3;
			}
			return;
	}
	
//发送接收
	switch(UARTCtrl->Task)//控制模块接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		case 0://空闲
		case 1://完成1帧发收
		case 2://接收超时(或错误)
			break;
		case 3://启动发送
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;//正在发送标志
			return;
		case 4://正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				UARTCtrl->RxByte=0;//rx计数=0
				p16timer[0]=1000/10;//超时定时
				UARTCtrl->Task=5;//0=正在接收
			}
			return;
		case 5://正在接收
			DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
			p16=(u16 *)Get_ADDR_UARTn(PORTn);
			p8=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
			LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
			LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
			p0 = p16[0];//中断接收指针
			p1 = p16[1];//当前已读指针
			if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
			{
				p16[0]=0;//中断接收指针
				p16[1]=0;//当前已读指针
				return;
			}
			if(p0!=p1)
			{//有数据接收,重置接收超时定时
				//被动接收超时定时从接收FIFO中有数据时开始定时,平时=0以便运行其他任务
				p16timer[0]=500/10;
			}
			RxByte=UARTCtrl->RxByte;
			while(p0 != p1)
			{//接收数据
				if(RxByte >= LEN_UARTnRx)
				{
					RxByte = 0;
				}
				i = p8[p1];
				p1 ++;
				if(p1 >= (LEN_UARTnIntRx-10))
				{
					p1 = 0;
				}
				p8rxbuff[RxByte] = i;
				RxByte ++;
				
				if(RxByte == 1)
				{
					if(i != 0x68)
					{
						RxByte = 0;
						continue;
					}
				}
				else
				{
					if(RxByte >= 7)
					{
						if(i == 0x16)
						{//接收完成
							//和校验计算
							//i=RAMCRC16((u32)p8rxbuff,4);//RAM中数据CRC16计算(硬件算法)
							i=RAMCRC16_Soft((u32)p8rxbuff,4);//RAM中数据CRC16计算(软件算法)
							if(i != (p8rxbuff[4]+(p8rxbuff[5]<<8)))
							{//和校验错误
								RxByte = 0;
								continue;
							}
							//和校验正确
							p16[1] = p1;//当前已读指针
							UARTCtrl->RxByte = RxByte;
							UARTCtrl->Task = 1;
							return;
						}
						else
						{//结束符不正确
							RxByte = 0;
							continue;
						}
					}
				}
			}
			p16[1] = p1;
			if(p16timer[0] == 0x0)
			{//超时
				UARTCtrl->Task=2;
				break;
			}
			else
			{
				UARTCtrl->RxByte = RxByte;
				return;
			}
		default:
			UARTCtrl->Task=0;//控制模块接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			return;
	}
	
//控制通信任务
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	switch(UARTCtrl->SubTask)
	{
		case 0://写控制命令
			if(p16timer[0]!=0x0)
			{
				break;//延时约500ms(接收字节后字节间超时值),免不间断老发送
			}
			p8txbuff[1]=0xA3;//控制命令帧
			p8=(u8*)ADDR_ContrlUARTData;//控制模块输出输入数据
			p8txbuff[2]=p8[0];
			p8txbuff[3]=p8[1];
		TX:
			p8txbuff[0]=0x68;
			//i=RAMCRC16((u32)p8txbuff,4);//RAM中数据CRC16计算(硬件算法)
			i=RAMCRC16_Soft((u32)p8txbuff,4);//RAM中数据CRC16计算(软件算法)
			p8txbuff[4]=i;
			p8txbuff[5]=i>>8;
			p8txbuff[6]=0x16;
			UARTCtrl->TxByte=7;//发送字节数(半字对齐)
			UARTCtrl->Task=3;//控制模块接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->SubTask++;
			break;
		case 1:
			if(UARTCtrl->Task==2)
			{//接收超时(或错误)
				WarningOccur(PORTn,(u8 *)"没检测到控制模块");//告警发生,入口告警代码和告警字符串
				//控制模块没有插入时，F12提示控制输出回路接入状态,	需要改成不接入状态
				p8=(u8*)ADDR_ContrlUARTData;//控制模块输出输入数据
				p8[2]=0xff;//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
				UARTCtrl->LinkTask=0;//关电源
				UARTCtrl->SubTask=0;
				return;
			}
			WarningCancel(PORTn);//告警取消,入口告警代码
			UARTCtrl->SubTask++;
			break;
		case 2://读控制命令
			p8txbuff[1]=0xA4;//读控制命令
			p8txbuff[2]=0;
			p8txbuff[3]=0;
			goto TX;
		case 3:
			if(UARTCtrl->Task==1)
			{//正确接收
				p8=(u8*)ADDR_ContrlUARTData;//控制模块输出输入数据
				if((p8[0]==p8rxbuff[2])&&(p8[1]==p8rxbuff[3]))
				{//写入数据正确
					Pin_Configure(PIN_CTR_EN,//控制模块输出允许
											0|//B3-B0:复用功能选择AF(0-15)
								 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
								 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
								 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
								 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
								(1<<11));//B11  :当为输出时：0=输出0,1=输出1	
				}
			}
			UARTCtrl->SubTask++;
			break;
		case 4://读告警状态
			p8txbuff[1]=0xA5;//读取告警状态帧
			p8txbuff[2]=0;
			p8txbuff[3]=0;
			goto TX;
		case 5:
			if(UARTCtrl->Task==1)
			{//正确接收
				p8=(u8*)ADDR_ContrlUARTData;//控制模块输出输入数据
				p8[2]=p8rxbuff[2];
				p8[3]=p8rxbuff[3];
			}
			UARTCtrl->SubTask++;
			break;
		case 6://读控制模块硬件错误信息
			p8txbuff[1]=0x50;//读帧
			goto TX;
		case 7://
			if(UARTCtrl->Task==1)
			{//正确接收
				i=p8rxbuff[2]|p8rxbuff[3];
				if(i)
				{
					if((Comm_Ram->ErrorIC&0x80000000)==0)
					{
						Comm_Ram->ErrorIC|=0x80000000;//硬件错误标志:(置位表示错误)
						Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
					}
				}
				else
				{
					Comm_Ram->ErrorIC&=0x7fffffff;//硬件错误标志:(置位表示错误)
				}
			}
			UARTCtrl->SubTask++;
			break;
		case 8://软件更新
//			p32=(u32*)(ADDR_DATABUFF);
//  		MR(ADDR_DATABUFF,ADDR_Download_ENTRY+(4*64),8);//读开始结束地址
//			if(p32[0]==0x00002000)
//			{//是I0入口地址
//				RxByte=p32[1]-p32[0];
//				if((p32[1]>p32[0])&&(RxByte<=0x1000))
//				{
//	        MR(ADDR_DATABUFF+8,ADDR_Download_ENTRY+(RxByte-8),8);//存储器读
//	        if(p32[3]==0xA55A5AA5)
//	        {
//						//起动文件传输
//						RxByte+=255;
//						RxByte/=256;//每帧固定传256BYTE
//						UARTCtrl->RxSEQ=RxByte;//总需发送帧数
//						UARTCtrl->TxSEQ=0;//已发帧数
//						UARTCtrl->SubTask++;
//						return;
//					}
//				}
//			}
			UARTCtrl->SubTask=0;
			break;
		case 9://文件发送帧
			p8txbuff[0]=0x68;
			p8txbuff[1]=0x51;//升级程序帧
			p8txbuff[2]=UARTCtrl->TxSEQ;//已发帧数
			p8txbuff[3]=0;
			MR(((u32)p8txbuff+4),ADDR_Download_ENTRY+(256*UARTCtrl->TxSEQ),256);
			//i=RAMCRC16((u32)p8txbuff,4+256);//RAM中数据CRC16计算(硬件算法)
			i=RAMCRC16_Soft((u32)p8txbuff,4+256);//RAM中数据CRC16计算(软件算法)
			p8txbuff[4+256]=i;
			p8txbuff[5+256]=i>>8;
			p8txbuff[6+256]=0x16;
			UARTCtrl->TxByte=7+256;//发送字节数(半字对齐)
			UARTCtrl->Task=3;//控制模块接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			UARTCtrl->SubTask++;
			break;
		case 10://文件发送返回帧
			if(UARTCtrl->Task==2)
			{//接收超时(或错误)
				UARTCtrl->SubTask=0;
				return;
			}
			if(UARTCtrl->Task==1)
			{//正确接收
				UARTCtrl->TxSEQ++;
				if(UARTCtrl->TxSEQ>=UARTCtrl->RxSEQ)
				{//发完
					MC(0,ADDR_Download_ENTRY+(4*64),8);//清缓存区
					UARTCtrl->SubTask=0;
					return;
				}
				UARTCtrl->SubTask--;//发下帧
			}
			break;
	}

}

#endif
