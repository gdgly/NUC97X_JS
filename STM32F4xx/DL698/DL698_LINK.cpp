

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_LINK.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"

#include "../DL698/DL698_Uart.h"
#include "../Device/RTC.h"
#include "../Device/ATNo.h"
#include "../Device/GPRS_3763.h"
#include "../MS/Ethernet.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"

//预连接请求的数据类型（LINK-Request）
//LINK-Request∷=SEQUENCE
//{
//  服务序号-优先级-ACD  PIID-ACD
//  请求类型  ENUMERATED
//{
//    建立连接 （0），
//    心跳     （1），
//    断开连接 （2）
//}，
//  心跳周期  long-unsigned，
//  请求时间   date_time
//}

void LINK_request(u32 PORTn,u32 ADDR_HeartberatTimer)
{
	u32 i;
	u32 LEN_SA;//服务器地址SA的长度
	u32 LEN_Tx;//总发送长度
	u8 * p8tx;
	u16 * p16;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 UARTxTimerAddr;
	u16 * p16timer;
	
	UARTxTimerAddr=Get_ADDR_UARTnTimer(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	p16=(u16 *)(ADDR_HeartberatTimer);//心跳发出间隔秒定时器
	if(p16[0]==0x0)
	{
		if(UARTCtrl->Task==0x0)
		{//接收
			p16=(u16 *)(UARTxTimerAddr);//UART定时器MS
			if(p16[0]==0x0)
			{
				if((UARTCtrl->LinkTask>=100)&&(UARTCtrl->LinkTask<=102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{//连接
						if((UARTCtrl->TCPUDP&0x30)==0x20)//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
						{//服务器模式,在设定(心跳周期+60秒)内没收到正确数据帧为断线
							if(PORTn==GPRSPORT)
							{
								i=Terminal_Ram->GPRSHeartBeat_S_Timer;//GPRS心跳秒定时器
							}
							if(PORTn==ENETPORT)
							{
								i=Terminal_Ram->ENETHeartBeat_S_Timer;//以太网心跳秒定时器
							}
							if(i==0)
							{
								UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							}
							
							if(UARTCtrl->LinkTask==102)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							{
								UARTCtrl->LinkTask=103;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							}
							return;
						}
						
						if(UARTCtrl->HostACK>8)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
						{//值错误
							UARTCtrl->HostACK=0;
						}
						if(UARTCtrl->HostACK>=4)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
						{//有其他主动上报等待确认返回
							return;
						}
						UARTCtrl->HeartberatCount+=1;//心跳发出计数,收到确认帧清0
						if(UARTCtrl->HeartberatCount>3)
						{//连续3次没收到确认帧
							UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
							if(UARTCtrl->LinkTask==102)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							{
								UARTCtrl->LinkTask=103;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							}
							else
							{
								UARTCtrl->LinkTask=0;//重新初始化
								if(PORTn==GPRSPORT)
								{
									WarningOccur(PORTn,(u8 *)"远程无线主站无响应");//告警发生,入口告警代码和告警字符串
								}
								if(PORTn==ENETPORT)
								{
									WarningOccur(PORTn,(u8 *)"以太网主站无响应");//告警发生,入口告警代码和告警字符串
								}
							}
						}
						else
						{
							UARTCtrl->Task=2;//2=等待单帧数据发送
							p8tx=(u8 *)(Get_ADDR_UARTnTx(PORTn));
							LEN_Tx=Uart_698_TxHead(p8tx,0x81,0);//Uart698帧头填入发送缓冲区,入口:C=控制域,CA=客户机地址;返回:帧头长度
							p8tx[LEN_Tx]=0x01;//[1] LINK-Request
							p8tx[LEN_Tx+1]=0x0;//PIID-ACD
							p8tx[LEN_Tx+2]=1,//请求类型：心跳（1）
							LEN_Tx+=3;
						// 心跳周期  long-unsigned，
							if(PORTn==GPRSPORT)
							{
								i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
							}
							else
							{
								i=(u32)Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
							}
							if(i==0)
							{
								i=120;
								i=(i>>8)+((i&0xff)<<8);
							}
							else
							{
								i=MRR(i+1,2);
							}
							p8tx[LEN_Tx]=i;
							p8tx[LEN_Tx+1]=i>>8;
							
							LEN_Tx+=2;
//请求时间  date_time
//date_time∷=SEQUENCE
//{
//  year          long-unsigned，
//  month         unsigned，
//  day_of_month  unsigned，
							
//  day_of_week   unsigned，
							
//  hour          unsigned，
//  minute        unsigned，
//  second        unsigned，
//  milliseconds  long-unsigned
//}
							ReadRTCtoDataType();//读RTC并转为各数据类型后填入到RAM缓冲
							p8tx[LEN_Tx+0]=Comm_Ram->DL698YMDHMS[0];//年(2B),月,日,时,分,秒,百分秒,周
							p8tx[LEN_Tx+1]=Comm_Ram->DL698YMDHMS[1];
							p8tx[LEN_Tx+2]=Comm_Ram->DL698YMDHMS[2];
							p8tx[LEN_Tx+3]=Comm_Ram->DL698YMDHMS[3];
							p8tx[LEN_Tx+4]=Comm_Ram->DL698YMDHMS[9];
							p8tx[LEN_Tx+5]=Comm_Ram->DL698YMDHMS[4];
							p8tx[LEN_Tx+6]=Comm_Ram->DL698YMDHMS[5];
							p8tx[LEN_Tx+7]=Comm_Ram->DL698YMDHMS[6];
							i=Read_STM32F4xx_RTC_SS();//读STM32F4xx内部RTC的亚秒值计算成ms
							p8tx[LEN_Tx+8]=i>>8;
							p8tx[LEN_Tx+9]=i;
							LEN_Tx+=10;
							//2byte 帧校验
							LEN_Tx+=1;//2;
							p8tx[1]=LEN_Tx;
							p8tx[2]=LEN_Tx>>8;
							UARTCtrl->TxByte=LEN_Tx+2;
							LEN_SA=p8tx[4];//取地址字节数
							LEN_SA&=0x0f;
							LEN_SA++;
							switch(UARTCtrl->LinkTask)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							{
								default://心跳
									UARTCtrl->HostACK=3;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
									p8tx[LEN_SA+8+1]=0x0;//PIID-ACD
									p8tx[LEN_SA+8+2]=1;//请求类型：心跳（1）
									break;
								case 100://登入
									UARTCtrl->HostACK=1;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
									p8tx[LEN_SA+8+1]=0x0;//PIID-ACD
									p8tx[LEN_SA+8+2]=0;//请求类型：建立连接（0）
									break;
								case 102://退出
									UARTCtrl->HostACK=2;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
									UARTCtrl->HeartberatCount=100;//退出时心跳发出计数设为100,若返回定时内没确认回答则立即重新初始化关闭模块连接
									p8tx[LEN_SA+8+1]=0x0;//PIID-ACD
									p8tx[LEN_SA+8+2]=2;//请求类型：退出连接（2）
									break;
							}
							Uart_698_HCSFCS(p8tx);//整帧校验计算,同时加结束0x16;返回0=正确，1=错误
							p16=(u16 *)(ADDR_HeartberatTimer);//心跳发出间隔秒定时器
							if(PORTn==GPRSPORT)
							{
								i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
							}
							else
							{
								i=(u32)Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//计算元素地址,使用ADDR_128K_DATABUFF
							}
							if(i==0)
							{
								i=120;
							}
							else
							{
								i=MRR(i+1,2);
								i=(i>>8)+((i&0xff)<<8);
							}
							if(i<WaitHeartberatACKMinTimer)
							{
								i=WaitHeartberatACKMinTimer;//心跳发出等待主站确认最小定时秒
							}
							if(i>WaitHeartberatACKMaxTimer)
							{
								i=WaitHeartberatACKMaxTimer;//心跳发出等待主站确认最大定时秒
							}
							p16[0]=i;//等心跳返回定时
							
 							p16timer=(u16 *)(Get_ADDR_UARTnTimer(PORTn));
							if(PORTn==GPRSPORT)
							{
							 //复制GPRS_3763中的等待单帧数据发送
							#if LOG_698==1//0=没,1=有
								LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
							#endif
								Terminal_Ram->CopyTxByte=UART1Ctrl->TxByte;//发送字节数
								UART1Ctrl->TxCount=0;//已发送计数
								Terminal_Ram->CopyTxCount=0;//已发送计数
								Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//2分,GPRS发送数据等待超时秒定时器
#if PPP_ENABLE==1
               //Do nothing
#else
								UART1Ctrl->ATTask=1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
								UART1Ctrl->ATCount=AT_MYNETWRITE;//发送数据
								UART1Ctrl->ATWriteTask=0;//AT数据发送任务:0=发命令,1=发数据
#endif
								UART1Ctrl->Task++;
							//为心跳时间准确度立即发送
								GPRS_3763_RxTx();//远程无线GPRS模块接收发送

							}
							if(PORTn==ENETPORT)
							{
							//复制Ethernet中的等待单帧数据发送
							#if LOG_698==1//0=没,1=有
								LOG(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
							#endif
								UARTCtrl->TxCount=0;
								p16timer[0]=30000/10;//30秒,等待发送数据给主站时间超时定时
								UARTCtrl->Task++;
							//为心跳时间准确度立即发送
								Ethernet_RxTx();
							}
						}

				}
			}
		}
	}
}

//预连接响应的数据类型（LINK-Response）
//LINK-Response∷=SEQUENCE
//{
//  服务序号-优先级  PIID，
//  结果             Result，
//  请求时间         date_time，
//  收到时间         date_time，
//  响应时间         date_time
//}
//Result∷=bit-string（SIZE（8））
//{
//  时钟可信标志	（0），
//  保留bit6   		（1），
//  保留bit5     	（2），
//  保留bit4     	（3），
//  保留bit3     	（4），
//  结果bit2     	（5），
//  结果bit1     	（6），
//  结果bit0     	（7）
//}
//时钟可信标志——用于表示响应方的时钟是否可信（准确），0：不可信，1：可信。结果bit0…结果bit2——二进制编码表示：0：成功，1：地址重复，2：非法设备，3：容量不足，其它值：保留。

void LINK_Response(u32 PORTn)
{
//	u8 * p8rx;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//  p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	
	switch(UARTCtrl->LinkTask)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
	{
		default://心跳
			UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0			
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
			break;
		case 100://登入
			UARTCtrl->LinkTask=101;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
			UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0			
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
			#if (USER/100)==9//河南
			if(PORTn==GPRSPORT)
			{
				DOTMIXPMODE->Ln9Code=8;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
			}
			#endif
			break;
		case 102://退出
			UARTCtrl->LinkTask=103;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
			UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0			
			UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
			break;
	}
}






