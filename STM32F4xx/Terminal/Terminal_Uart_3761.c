
//UART376.1
#include "Project.h"
#include "../Terminal/Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"
#include "../Terminal/Terminal_USER11xx.h"


void Uart_3761_RxTx(u32 PORTn)//UART_3761接收发送
{
	u32 i;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)
	{
		case 0://0=正在接收
			//组地址和广播地址时级连转发结束
			i=UARTCtrl->CascadingBroadcast;//0=空,级联组或广播地址转发的端口号(RS485_1PORT,RS485_2PORT,RS485_3PORT)
			if(i!=0)
			{
				UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(i);
				if(UARTCtrlRS485->Lock==0x55)
				{
					if(UARTCtrlRS485->TxByte==0)//发送字节数(半字对齐)
					{
						UARTCtrl->CascadingBroadcast=0;//0=空,级联组或广播地址转发的端口号(RS485_1PORT,RS485_2PORT,RS485_3PORT)
						UARTCtrlRS485->Lock=0;
					}
				}
			}
			//接收
			i=Uart_3761_Rx(PORTn);//UART_3761接收;返回0没完成,1完成,2超时
			if(i==1)
			{
				UARTCtrl->Task=1;
			}
			break;
		case 1://1=接收完成
			UARTCtrl->Task=0;//0=接收
			UARTCtrl->AddrType=0;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			UARTCtrl->TxFrameCount=0;//寄存多帧数据时已发送帧计数
			UARTCtrl->MulFrameGPCR=0;//多帧时通用计数寄存器
			UARTCtrl->LockFlags&=(~(7<<4));//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动1类定时任务,B5=主动2类定时任务,B6=召读1类定时任务,B7
			UARTCtrl->SubTask=0;//子任务(临时操作路由模块任务计数,在每帧接收OK时初始化为0)
			//接收-发送延时
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS或短信通道
				p16timer[0]=0;//延时0MS发送
			}
			else
			{//RS232,RS485
			#if (USER/100)==0//电科院测试
				p16timer[0]=200/10;//20/10;//延时?MS发送(两帧之间的线路空闲间隔最少需33位)
			#else
				p16timer[0]=50/10;//20/10;//延时?MS发送(两帧之间的线路空闲间隔最少需33位)
			#endif
			}
			if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
			{//通信模块
				//清心跳定时(正确收到任何帧,清心跳定时)
				i=MRR(ADDR_AFN04F1+5,1);
				if(i==0x0)
				{
					i=1;//最小1分
				}
				i*=60;
				if((UARTCtrl->TCPUDP&0x30)==0x20)//原工作模式(同F8设置字)B7=0TCP=1UDP,B6=,B5-B4=0混合模式=1客户机模式=2服务器模式,B3-B0=其它
				{//是服务器模式
					i+=60;//加60秒可能的主站心跳发出定时误差
				}
				if(PORTn==GPRSPORT)
				{
					Terminal_Ram->GPRSHeartBeat_S_Timer=i;//GPRS心跳秒定时器
					if(UARTCtrl->HostACK<=1)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
					{
						i=MRR(ADDR_AFN04F8+4,1);
						if(i==0x0)
						{
							i=1;//最小被动激活模式连续无通信自动断线时间1分
						}
						i*=60;
						Terminal_Ram->GPRSAutoDiscon_S_Timer=i;//GPRS被动激活模式连续无通信自动断线秒定时器
					}
				}
				if(PORTn==ENETPORT)
				{
					Terminal_Ram->ENETHeartBeat_S_Timer=i;//以太网心跳秒定时器
				}
				//清自动保电允许与主站连续无通信时间分定时器
				i=MRR(ADDR_AFN04F58,1);
				if(i==0)
				{
					i=1;//最小1
				}
				i*=60;//时化为分
				Terminal_Ram->DisconAutoProtect_M_Timer=i;//1 自动保电允许与主站连续无通信时间分定时器
			}
			if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
			{
				Comm_Ram->BattWork_S_timer=60;//4 电池工作关机秒定时器
			}
		#if (USER/100)==11//吉林系
			i=USER11xx_RxAddr(PORTn);//吉林系接收帧终端地址比较,4字节长度变为标准帧的2字节;返回:0=地址不同,1=地址相同
			if(i==0)
			{
				UARTCtrl->RxByte=0;//rx计数=0
				break;
			}
		#endif
			
		#if LOG_1376_1==1//0=没,1=有
			LOG(13761,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
			Uart_3761_Data(PORTn);//UART_3761数据
			UARTCtrl->RxByte=0;//rx计数=0
			break;
		case 2://2=等待单帧数据发送
//			if(UARTCtrl->AddrType>1)//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
//			{
//				UARTCtrl->AddrType=0;//接收帧的地址类型:0=单地址,1=组地址,2=广播地址
//				UARTCtrl->Task=0;
//				break;
//			}
			if(p16timer[0]==0x0)
			{
			#if (USER/100)==11//吉林系
				USER11xx_TxAddr(PORTn);//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2
			#endif
			#if LOG_1376_1==1//0=没,1=有
				LOG(13761,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
				UARTCtrl->Task=3;//正在发送标志
				Start_UARTn_Tx(PORTn);//启动UART口发送
			}
			else
			{
				if(p16timer[0]>(200/10))
				{//数据转发,返回err时没重置定时值!
					p16timer[0]=200/10;
				}
			}
			break;
		case 3://3=单帧数据正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{			
				UARTCtrl->Task=0;//0=正在接收
				UARTCtrl->RxByte=0;//rx计数=0
				p16timer[0]=0;//超时定时=0;接收超时定时从接收FIFO中有数据时开始定时,平时=0以便运行其他任务
			}
			break;
			
		case 4://4=多帧数据续接收完成
			//接收-发送延时
			if((PORTn==ENETPORT)||(PORTn==GPRSPORT)||(PORTn==SMSPORT))
			{//ENET,GPRS或短信通道
				p16timer[0]=0;//延时0MS发送
			}
			else
			{//RS232,RS485
				p16timer[0]=20/10;//延时?MS发送(两帧之间的线路空闲间隔最少需33位)
			}
			Uart_3761_Data(PORTn);//UART_3761数据
			UARTCtrl->RxByte=0;//rx计数=0
			break;
		case 5://5=等待多帧数据发送
			if(p16timer[0]==0x0)
			{
			#if (USER/100)==11//吉林系
				USER11xx_TxAddr(PORTn);//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2
			#endif
			#if LOG_1376_1==1//0=没,1=有
				LOG(13761,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
				UARTCtrl->Task=6;//正在发送标志
				Start_UARTn_Tx(PORTn);//启动UART口发送
			}
			else
			{
				if(p16timer[0]>(20/10))
				{//数据转发,返回err时没重置定时值!
					p16timer[0]=20/10;
				}
			}
			break;
		case 6://6=多帧数据正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				UARTCtrl->RxByte=0;//rx计数=0
				p16timer[0]=0;//超时定时=0;接收超时定时从接收FIFO中有数据时开始定时,平时=0以便运行其他任务
				
				UARTCtrl->Task=4;//4=多帧数据续接收完成
			}
			break;
		case 7://7=等待发送器空转为接收
			UARTCtrl->Task=0;//0=正在接收
			UARTCtrl->RxByte=0;//rx计数=0
			p16timer[0]=0;//超时定时=0;接收超时定时从接收FIFO中有数据时开始定时,平时=0以便运行其他任务
			break;
		case 8://8=等待通过其它口数据转发
		case 9://9=等待下帧启动发送延时
		case 10://10=正在通过其它口发送数据
		case 11://11=正在通过其它口接收数据
			Forwarding(PORTn);//数据转发处理过程
			break;
		case 20://20=级联接收完成
		case 21://21=启动转发级联数据
		case 22://22=正在转发级联数据
		case 23://22=接收级联数据
			Cascading(PORTn);//级联处理过程
			break;
		default:
			UARTCtrl->Task=0;//0=正在接收
			break;
	}
}



void Uart_3761_Data(u32 PORTn)//UART_376数据
{
	u32 i;
	u32 x;
	u16 * p16;
	u8 * p8rxbuff;
	u8 * p8txbuff;
  UARTCtrl_TypeDef * UARTCtrl;
#if (((Project/100)==2)||((Project/100)==4))//集中器,公变终端
	u32 RS485PORT;
	u8 * p8RS485txbuff;
  UARTCtrl_TypeDef * UARTCtrlRS485;
#endif
#if ((Extern_Uart_AFN255|Extern_Uart_AFN04|Extern_Uart_AFN05)!=0)//外部程序:0=I2,1=E1,2=E2,3=	
	u32 *p32;
#endif

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	if(p8rxbuff[6]&0x80)
	{//非主站下行帧
		return;
	}
	//保存帧序列域SEQ
	UARTCtrl->RxSEQ=p8rxbuff[13];
	//COPY 14+16字节数据头到发送区,作为发送头
	MW((u32)p8rxbuff,(u32)p8txbuff,14+16);

	//返回帧SEQ
	p8txbuff[13]|=0x60;//返回帧暂为单帧TpV,FIR,FIN,CON;PSEQ|RSEQ
	p8txbuff[13]&=0xef;//返回帧CON暂=0
	//返回帧控制码
	p8txbuff[6]=(p8txbuff[6]&0xf)+0x80;//控制码DIR=1上行,启动标志位PRM=0,要求访问位ACD暂=0;功能码不变

	if((p8rxbuff[1]+(p8rxbuff[2]<<8))<12)
	{
		return;//用户区数据长度<12byte错
	}
	
	//地址比较
	MR(ADDR_DATABUFF,ADDR_AREACODE,4);//读行政区域码,终端地址
	i=MRR(ADDR_DATABUFF,4);
	if(i==0xffffffff)
	{
		goto AddrOK;//终端地址设为广播地址,不比较
	}
#if (USER/100)==11//吉林系
	if(Comm_Ram->Factory==0x55)//0x55=工厂状态
	{
#endif
		if(Comm_Ram->Factory==0x55)//0x55=工厂状态
		{
			if(p8rxbuff[12]==255)
			{//AFN=255
				if((p8rxbuff[16]+(p8rxbuff[17]<<8))==0xc10)
				{//Fn=101写终端地址等
					goto AddrOK;
				}
			}
		}
	#if (USER/100)==10//河北万能地址比较和广播地址
		i=MRR(((u32)p8rxbuff)+7,4);
		if(i==0x44332211)
		{
			goto AddrOK;
		}
		if(i==0xffffffff)
		{//广播地址
			//剔除投入
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//剔除投入,只有对时命令继续有效,其他全部无效
				if(p8rxbuff[12]!=5)
				{//AFN!=5
					return;
				}
				if((p8rxbuff[14]+(p8rxbuff[15]<<8))!=0)
				{//Pn!=0
					return;
				}
				if((p8rxbuff[16]+(p8rxbuff[17]<<8))!=0x340)
				{//Fn!=31对时命令
					return;
				}
			}
			UARTCtrl->AddrType=3;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			goto AddrOK;
		}
	#endif
	#if ((USER/100)==12)||((USER/100)==13)//四川、黑龙江广播地址
		i=MRR(((u32)p8rxbuff)+7,4);
		if(i==0xffffffff)
		{//广播地址
			//剔除投入
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//剔除投入,只有对时命令继续有效,其他全部无效
				if(p8rxbuff[12]!=5)
				{//AFN!=5
					return;
				}
				if((p8rxbuff[14]+(p8rxbuff[15]<<8))!=0)
				{//Pn!=0
					return;
				}
				if((p8rxbuff[16]+(p8rxbuff[17]<<8))!=0x340)
				{//Fn!=31对时命令
					return;
				}
			}
			UARTCtrl->AddrType=3;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			goto AddrOK;
		}
	#endif
		i=p8rxbuff[9]+(p8rxbuff[10]<<8);//地址码
		if(i==0xffff)
		{//广播地址
			//if(p8rxbuff[12]!=0x0F)//应用层功能码AFN
			{//不是文件传输
				if((p8rxbuff[11]&0x1)==0x0)
				{//不是组地址标志错
					return;
				}
				//剔除投入
				i=MRR(ADDR_AFN05F28,1);
				if(i==0x55)
				{//剔除投入,只有对时命令继续有效,其他全部无效
					if(p8rxbuff[12]!=5)
					{//AFN!=5
						return;
					}
					if((p8rxbuff[14]+(p8rxbuff[15]<<8))!=0)
					{//Pn!=0
						return;
					}
					if((p8rxbuff[16]+(p8rxbuff[17]<<8))!=0x340)
					{//Fn!=31对时命令
						return;
					}
				}
				//行政区域码比较
				if((p8rxbuff[7]+p8rxbuff[8])!=0x0)
				{//注:行政区域码=0时自定义认为广播区域码用于终端地址通信设置
					if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,2)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
					{//行政区域码错
						return;
					}
				}
				UARTCtrl->AddrType=3;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			}
		}
		else
		{//非广播地址
			if((p8rxbuff[11]&0x1)==0x0)
			{//单地址比较
				//本终端行政区域码和地址比较
				if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,4)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				{//不是本终端行政区域码和地址
					i=MRR(ADDR_AFN04F37,1);//终端级联通信参数
					if(i!=0x0)
					{//有级联口
						i=MRR(ADDR_AFN04F37+6,1);
						if((i&0x80)==0x0)
						{//是主动级联口
							x=i&0x7;//被级联终端个数
							MR(ADDR_DATABUFF+4,ADDR_AFN04F37+7,(4*x));//终端级联通信参数
							for(i=0;i<x;i++)
							{
								if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF+4+(4*i),4)==0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
								{
									break;
								}
							}
							if(i!=x)
							{//找到被级联终端相同行政区域码和地址
								UARTCtrl->Task=20;//20=级联接收完成
								UARTCtrl->CascadingAddrType=1;//级联数据及地址类型0=无级联数据,1=单地址,2=组地址,3=广播地址
								UARTCtrl->CascadingReTxCount=0;//级联重发计数
							}
						}
					}
					return;
				}
			}
			else
			{//地址为组地址
				//剔除投入
				i=MRR(ADDR_AFN05F28,1);
				if(i==0x55)
				{//剔除投入,只有对时命令继续有效,其他全部无效
					if(p8rxbuff[12]!=5)
					{//AFN!=5
						return;
					}
					if((p8rxbuff[14]+(p8rxbuff[15]<<8))!=0)
					{//Pn!=0
						return;
					}
					if((p8rxbuff[16]+(p8rxbuff[17]<<8))!=0x340)
					{//Fn!=31对时命令
						return;
					}
				}
				//行政区域码比较
				if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,2)!=0x0)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				{//行政区域码错
					return;
				}
				p16=(u16 *)(ADDR_DATABUFF);
				p16[0]=p8rxbuff[9]+(p8rxbuff[10]<<8);//地址码
				if(p16[0]==0)
				{//组地址=0为无效组地址
					return;
				}
				MR(ADDR_DATABUFF+2,ADDR_AFN04F6,16);
				for(i=0;i<8;i++)
				{
					if(p16[0]==p16[1+i])
					{
						break;
					}
				}
				if(i==8)
				{//组地址不相同
					return;
				}
				UARTCtrl->AddrType=2;//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			}
		}
#if (USER/100)==11//吉林系
	}
#endif

AddrOK:
#if (((Project/100)==2)||((Project/100)==4))//集中器,公变终端
	//组地址和广播地址时级连转发
	if(UARTCtrl->AddrType>1)//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
	{
		//主动级联口
		i=MRR(ADDR_AFN04F37+6,1);//级联/被级联标志
		if((i&0x80)==0)
		{//主动级联
			i=MRR(ADDR_AFN04F37,1);//终端级联通信端口号 BIN  1 
			RS485PORT=0;
			switch(i)
			{
				case 2://RS485-1
					if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)!=0))
					{//不是终端维护
						RS485PORT=RS485_1PORT;
					}
					break;
				case 3://RS485-2
					if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)!=0)
					{//不是终端维护
						RS485PORT=RS485_2PORT;
					}
					break;
				case 4://RS485-3
					RS485PORT=RS485_3PORT;
					break;
			}
			if(RS485PORT!=0)
			{
				UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485PORT);
				if((UARTCtrlRS485->Lock==0x55)||(UARTCtrlRS485->Task!=0))//主动级联巡测任务:0=空,1=启动发送,2=正在发送,3=接收返回,100=等待上传
				{
					UARTCtrl->Task=1;//保持1=接收完成
					return;
				}
				UARTCtrl->CascadingBroadcast=RS485PORT;//0=空,级联组或广播地址转发的端口号(RS485_1PORT,RS485_2PORT,RS485_3PORT)
				UARTCtrlRS485->Lock=0x55;//0x55=通信功能正在被调用禁止其他程序再次调用
				p8RS485txbuff=(u8 *)Get_ADDR_UARTnTx(RS485PORT);
				i=p8rxbuff[1]+(p8rxbuff[2]<<8);//用户数据区长度
				i+=8;
				MW((u32)p8rxbuff,(u32)p8RS485txbuff,i);
				p8RS485txbuff[1]=p8RS485txbuff[3];//长度字节恢复
				p8RS485txbuff[2]=p8RS485txbuff[4];
				UARTCtrlRS485->TxByte=i;//需发送字节数
				//Terminal_Ram->CD485TxLED_MS_Timer=100/10;//12 级联485发送LED指示MS定时器,每10MS减1定时器
				if(RS485PORT==RS485_1PORT)
				{
					Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;
				}
				if(RS485PORT==RS485_2PORT)
				{
					Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;
				}
				UARTCtrlRS485->BpsCtrl=MRR(ADDR_AFN04F37+1,1);
				Init_UARTn(RS485PORT);//初始化UART口,入口通信控制字已填入
				Start_UARTn_Tx(RS485PORT);//启动UART口发送
			}
		}
	}
#endif
	//本终端实地址填入发送区
	MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);
	p8txbuff[11]&=0xfe;//单地址标志
	UARTCtrl->Lock=0x55;//禁止其他程序再次调用
	switch(p8rxbuff[12])//应用层功能码AFN
	{
		case 0://确认/否认
			Uart_AFN00(PORTn);//确认/否认
			break;
		case 1://复位
			Uart_AFN01(PORTn);//复位
			break;
		case 2://链路接口检测
			Uart_AFN02(PORTn);//链路接口检测
			break;
		case 3://中继站命令
			break;
		case 4://设置参数
		#if Extern_Uart_AFN04==0//外部程序:0=I2,1=E1,2=E2,3=;参数设置和查询
			Uart_AFN04(PORTn);//参数设置
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[108])(PORTn);//参数设置
			}
		#endif
			break;
		case 5://控制命令
		#if Extern_Uart_AFN05==0//外部程序:0=I2,1=E1,2=E2,3=;控制命令
			Uart_AFN05(PORTn);//控制命令
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[109])(PORTn);//控制命令
			}
		#endif
			break;
		case 6://身份认证及密钥协商
			Uart_AFN06(PORTn);//身份认证及密钥协商
			break;
		case 7://备用
			break;
		case 8://请求被级联终端主动上报
			Uart_AFN08(PORTn);//请求被级联终端主动上报
			break;
		case 9://请求终端配置
		#if Extern_Uart_AFN09==0//外部程序:0=I2,1=E1,2=E2,3=;请求终端配置
			Uart_AFN09(PORTn);//请求终端配置
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[111])(PORTn);//请求终端配置
			}
		#endif
			break;
		case 10://查询参数
		#if Extern_Uart_AFN04==0//外部程序:0=I2,1=E1,2=E2,3=;参数设置和查询
			Uart_AFN0A(PORTn);//参数查询
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[110])(PORTn);//参数查询
			}
		#endif
			break;
		case 11://请求任务数据
			Uart_AFN0B(PORTn);//请求任务数据
			break;
		case 12://请求1类数据(实时数据)
		#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
			x=p8rxbuff[14]+(p8rxbuff[15]<<8)+(p8rxbuff[16]<<16)+(p8rxbuff[17]<<24);
			switch(x)
			{
				case 0x19010000://AFN0CF201	P0 
					Uart_AFN0CF201(PORTn);//上海集中器扩展AFN0CF201读取最近一次抄读失败表个数P0
					break;
				case 0x19020000://AFN0CF202	P0
					Uart_AFN0CF202(PORTn); //上海集中器扩展AFN0CF202读取最近一次抄读电能表失败表信息P0
					break;
				default:
					Uart_AFN0C(PORTn);//类1数据
					break;
			}
		#elif ((USER/100)==12)//四川用户
			x=p8rxbuff[16]+(p8rxbuff[17]<<8);
			switch(x)
			{
				case 0x0140://AFN0CF15	P0 
					Uart_AFN0CF15(PORTn);//扩展读取SIM卡信号强度
					break;
				case 0x2601://AFN0CF305	P0 
					Uart_AFN0CF305(PORTn);//透明任务执行状态
					break;
				case 0x2602://AFN0CF306	Pn 
					Uart_AFN0CF306(PORTn);//透明任务执行结果
					break;
				case 0xFE80://AFN0CF2040	P0 
					Uart_AFN0CF2040(PORTn);//扩展读取远程模块信号强度
					break;
				default:
					Uart_AFN0C(PORTn);//类1数据
					break;
			}
		#else
			Uart_AFN0C(PORTn);//类1数据
		#endif
			break;
		case 13://请求2类数据(历史数据)
		#if (USER/100)==8//用户标识：通用
			if((p8rxbuff[16]==0x02)&&(p8rxbuff[17]==0x28))
			{//AFN0DF322
				Uart_AFN0DF322(PORTn);//辽宁电表对时后误差情况统计明细
			}
			else
			{
				if((p8rxbuff[16]==0x01)&&(p8rxbuff[17]==0x28))
				{//AFN0DF321
					Uart_AFN0DF321(PORTn);//辽宁电表对时后误差情况统计明细
				}
				else
				{
					Uart_AFN0D(PORTn);//类2数据
				}
			}
		#else
			#if (USER/100)==6//用户标识：湖南
				if(((p8rxbuff[16]==0x80)&&(p8rxbuff[17]==0x31))||((p8rxbuff[16]==0x01)&&(p8rxbuff[17]==0x32))||((p8rxbuff[16]==0x02)&&(p8rxbuff[17]==0x32)))
				{//AFN0DF400、F401、F402
					Uart_AFN0DF40x(PORTn);
				}
				else
				{
					Uart_AFN0D(PORTn);//类2数据
				}
			#else
				Uart_AFN0D(PORTn);//类2数据
			#endif
		#endif
			break;
		case 14://请求3类数据(事件数据)
			Uart_AFN0E(PORTn);//类3数据(事件)
			break;
		case 15://文件传输
			Uart_AFN0F(PORTn);//文件传输
			break;
		case 16://数据转发
			Uart_AFN10(PORTn);//数据转发
			break;
			
		case 255://工厂测试(扩展)
		#if Extern_Uart_AFN255==0//外部程序:0=I2,1=E1,2=E2,3=
			Uart_AFN255(PORTn);//工厂测试(扩展)
		#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//软件模块错误标志(置位表示错误):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
  			p32=(u32 *)(ADDR_E1_ENTRY);
  			(*(void(*)())p32[100])(PORTn);//工厂测试(扩展)
			}
		#endif
			break;
		default:
			break;
	}
	UARTCtrl->Lock=0;//解除禁止其他程序再次调用
}











