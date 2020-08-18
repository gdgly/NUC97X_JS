

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../DL698/PROXY_TASK.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/AutoSearchMeter.h"
#include "../TEST/LOG.h"


#include "../DL698/DL698_JSON.h"


void RS485_TxLED(u32 PORTn)//RS485 LED发送指示灯
{
	u32 i;
	u32 x;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//LED发送指示灯
	x=UARTCtrl->TxByte;
	x*=(11*1000);//ms
	i=UARTCtrl->BpsCtrl;
	i>>=5;
	i&=0xf;
	switch(i)
	{
		case 0://0=300,
			x/=300;
			break;
		case 1://1=600,
			x/=600;
			break;
		case 2://2=1200,
			x/=1200;
			break;
		case 3://3=2400,
			x/=2400;
			break;
		case 4://4=4800,
			x/=4800;
			break;
		case 5://5=7200,
			x/=7200;
			break;
		case 6://6=9600,
			x/=9600;
			break;
		case 7://7=19200
		default:
			x/=19200;
			break;
	}
	x/=10;
	if(x<2)
	{
		x=2;
	}
	if(PORTn==RS485_1PORT)
	{
		Terminal_Ram->RS485_1_TxLED_MS_Timer=x;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
	}
	if(PORTn==RS485_2PORT)
	{
		Terminal_Ram->RS485_2_TxLED_MS_Timer=x;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
	}
}


void RS485_RxTx(u32 PORTn)//RS485接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
{
	u32 i;
  UARTCtrl_TypeDef* UARTCtrl;
	u16* p16timer;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			break;
		case 1://1=完成1帧发收
			break;
		case 2://2=接收超时(或错误)
			break;
		case 3://3=启动发送
			if(p16timer[0]!=0)
			{
				break;
			}
		#if DEBUG_RS485_1toRS232==1//0=否,1=是;调试:RS485_1口收发数据打印到RS232口
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=否,1=是;调试:RS485_2口收发数据打印到RS232口
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
			
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			UARTCtrl->Task=4;
			RS485_TxLED(PORTn);//RS485 LED发送指示灯
			break;
		case 4://4=正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				//p16timer[0]=UARTCtrl->TxToRxTimerOver;//发送后接收超时定时值ms/10(半字对齐)
				p16timer[0]=RMRS485OverTimer/10;//DL645读表超时定时值ms
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->Task=5;
			}
			break;
		case 5://5=正在接收
			i=RM_Rx(PORTn);//抄表接收帧;入口:PORTn;返回:0=正在接收,1=完成1帧接收,2=接收超时
			switch(i)
			{
				case 0://正在接收
					return;
				case 1://完成1帧接收
					UARTCtrl->Task=1;
					p16timer[0]=30/10;//下帧发送延时
				
		#if DEBUG_RS485_1toRS232==1//0=否,1=是;调试:RS485_1口收发数据打印到RS232口
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=否,1=是;调试:RS485_2口收发数据打印到RS232口
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
					break;
				default://接收超时
					UARTCtrl->Task=2;
					return;
			}
			break;
	}
}


void RM_RS485(u32 PORTn)//RS485抄表
{
	u32 i;
	u32 x;
	//u8* p8tx;
	//u8* p8rx;
	u16* p16timer;
	u8* pCSD;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
	#if(USER/100)==15//安徽
	u8* p8;
	#endif

	x=x;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	if(PORTn!=ACSAMPLEPORT)
	{//不是交采
		UARTCtrl->PowerUpDealy=1;
		if(UARTCtrl->PowerUpDealy==0)//上电延时:0=没延时,!=0已到延时
		{
			p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
			if(p16timer[0]!=0x0)//10 上电后延时抄表秒定时器
			{
				CopyString((u8 *)"485 等待上电",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				pCSD=(u8*)ADDR_DATABUFF;
				if(PORTn==RS485_1PORT)
				{
					i=1;
				}
				else
				{
					i=2;
				}
				pCSD[3]=i;
				pCSD[12]=0;
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
				return;
			}
			UARTCtrl->PowerUpDealy=1;//上电延时:0=没延时,!=0已到延时
		}
	}

	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=空闲
			if(PORTn!=ACSAMPLEPORT)
			{//不是交采
				//空闲时进入其他任务
				i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				if(i)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				{
					PROXY_TASK_RS485(PORTn);//代理任务RS485
					return;
				}
				i=UARTCtrl->BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
				if((i&0x7f)==1)
				{
					i++;
					UARTCtrl->BroadcastTimeTask=i;
					return;
				}
				i=UARTCtrl->AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
				if(i==1)
				{
					i++;
					UARTCtrl->AutoSearchTask=i;//自动搜表任务:0=空,1=等待,2...过程
					return;
				}
				i=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
				if(i==1)
				{
					i++;
					UARTCtrl->AutoReportTask=i;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
					return;
				}
			}
			UARTCtrl->Task=3;//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			break;
		case 2://2=接收超时(或错误)
			UARTCtrl->Task=0;//空闲1次
			break;
		case 3://3=启动发送
			if(p16timer[0]!=0)//下帧启动发送延时
			{
				return;
			}
			//p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			//p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			if(UARTCtrl->ReRMCount>=3)//抄表重发计数
			{
				RM_OK_FAIL(PORTn,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
				UARTCtrl->ReRMCount=0;//抄表重发计数
				UARTCtrl->RM_OKFAIL=0;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
				UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
				//下电能表
				RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
			}
			UARTCtrl->RMMeterCountAuto=0;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
			
			if(UARTCtrl->CONNECT==0)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
			{
				TASK(PORTn,1);//采集任务;TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号;返回:0表示无采集任务
				i=UARTCtrl->TaskID;
				if(i==0)
				{
					p16timer[0]=2000/10;//下次计算TASK()在1秒后,否则会影响菜单速度
					UARTCtrl->Task=0;//空闲1次
					if(PORTn!=ACSAMPLEPORT)
					{//不是交采
						RM_NUM_Display(PORTn);//抄表数量显示
						if(PORTn==RS485_4PORT)
						{
							WarningOccur(PORTn,(u8 *)"载波 当前任务完成");//告警发生,入口告警代码和告警字符串
						}
					}
			#if DEBUG==1//0=发行,1=调试
					else
					{//是交采
						RM_NUM_Display(PORTn);//抄表数量显示
					}
			#endif
					return;
				}
			}
			i=RM_TxDATA_Manage(PORTn);//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
			if(UARTCtrl->TxByte==0)
			{
				UARTCtrl->ReRMCount=0;//抄表重发计数
				return;
			}
		#if DEBUG_RS485_1toRS232==1//0=否,1=是;调试:RS485_1口收发数据打印到RS232口
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=否,1=是;调试:RS485_2口收发数据打印到RS232口
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
			
			UARTCtrl->ReRMCount++;//抄表重发计数
			if(PORTn==ACSAMPLEPORT)
			{//交采
			#if iMeterProtocol==2//内部交采抄表规约:2=DL/T645-2007，3=DL/T698.45
			//内部交采抄表规约:2=DL/T645-2007
				//	DataType_CSD,1,//0=OAD,1=ROAD
				//	0x50,0x02,0x02,0x00,//OAD
				//	2,//关联个数
				//	0x20,0x21,0x02,0x00,//1 冻结时标
				//	0x20,0x00,0x02,0x00,//2 电压
					i=UARTCtrl->RMCSD[1];
					if(i==1)
					{//1=ROAD
						i=UARTCtrl->RMCSD[2];
						i<<=8;
						i|=UARTCtrl->RMCSD[3];
						i<<=8;
						i|=UARTCtrl->RMCSD[4];
						i<<=8;
						i|=UARTCtrl->RMCSD[5];
						if(i==0x50020200)
						{//分钟冻结(内部交采曲线)
						#if (USER/100)==9//河南,5分钟电压曲线
							if(UARTCtrl->RM_DI_TI_Count!=0)//按时标间隔采集时的间隔计数
							{//交采只抄当前一个点
								UARTCtrl->ReRMCount=0;//抄表重发计数
								UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
								UARTCtrl->Task=0;//空闲1次
								return;
							}
							UARTCtrl->RM_DI_TI_Count=0xF0;//交采只抄当前一个点
							p8rx[8]=0x91;//控制码
							p8rx[9]=36;//数据长度
							UARTCtrl->RM_DI=0x06010001;
							p8rx[10+0]=0x01;//DI
							p8rx[10+1]=0x00;
							p8rx[10+2]=0x01;
							p8rx[10+3]=0x06;
						//负荷记录起始码：A0H，A0H（或 E0H，E0H：本数据块不正确），2 字节； 
							p8rx[10+4]=0xA0;
							p8rx[10+5]=0xA0;
						//负荷记录字节数：1 字节（十六进制）； 
							p8rx[10+6]=36-7-2;
						//负荷记录存储时间：年、月、日、时、分,  5 字节；
							i=Comm_Ram->DL698YMDHMS[5];
							i/=5;
							i*=5;
							i=hex_bcd(i);
							p8rx[10+7]=i;
							p8rx[10+8]=Comm_Ram->MYMDHMS[2];
							p8rx[10+9]=Comm_Ram->MYMDHMS[3];
							p8rx[10+10]=Comm_Ram->MYMDHMS[4];
							p8rx[10+11]=Comm_Ram->MYMDHMS[5];
							
						//电压、电流、频率：17 字节； 
							p8tx=(u8*)ADDR_UA+2;//A电压
							p8rx[10+12]=p8tx[0];
							p8rx[10+13]=p8tx[1];
							p8tx=(u8*)ADDR_UB+2;//B电压
							p8rx[10+14]=p8tx[0];
							p8rx[10+15]=p8tx[1];
							p8tx=(u8*)ADDR_UC+2;//C电压
							p8rx[10+16]=p8tx[0];
							p8rx[10+17]=p8tx[1];
							p8tx=(u8*)ADDR_IA+1;//A电流
							p8rx[10+18]=p8tx[0];
							p8rx[10+19]=p8tx[1];
							p8rx[10+20]=p8tx[2];
							p8tx=(u8*)ADDR_IB+1;//B电流
							p8rx[10+21]=p8tx[0];
							p8rx[10+22]=p8tx[1];
							p8rx[10+23]=p8tx[2];
							p8tx=(u8*)ADDR_IC+1;//C电流
							p8rx[10+24]=p8tx[0];
							p8rx[10+25]=p8tx[1];
							p8rx[10+26]=p8tx[2];
							p8tx=(u8*)ADDR_FREQUENCY;//频率
							p8rx[10+27]=p8tx[0];
							p8rx[10+28]=p8tx[1];
						//块分隔码：AAH，1 字节； 
							p8rx[10+29]=0xaa;
						//有、无功功率：24 字节； 
						//块分隔码：AAH，1 字节； 
							p8rx[10+30]=0xaa;
						//功率因数：8 字节； 
						//块分隔码：AAH，1 字节； 
							p8rx[10+31]=0xaa;
						//有、无功总电能：16 字节； 
						//块分隔码：AAH，1 字节； 
							p8rx[10+32]=0xaa;
						//四象限无功总电能：16 字节；
						//块分隔码：AAH，1 字节； 
							p8rx[10+33]=0xaa;
						//当前需量：6 字节； 
						//块分隔码：AAH，1 字节；
							p8rx[10+34]=0xaa;
						//负荷记录累加校验码：1 字节（从第一个 A0H 开始到最后一个数据块结束码）；
							p8rx[10+35]=0;
						//负荷记录结束码：E5H,1 字节。
							p8rx[10+35]=0xE5;
							
						//数据加0x33
							for(i=0;i<36;i++)
							{
								p8rx[10+i]+=0x33;
							}
							UARTCtrl->Task=1;//1=接收完成
						#if DEBUG==1//0=发行,1=调试
							RM_ADDR_Display(ACSAMPLEPORT);//抄表地址显示
						#endif
							return;
						#endif
							
						}
					}
					
					UARTCtrl->Task=1;//1=接收完成
					UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
					p32=(u32 *)(ADDR_Meter_ENTRY);
					/*
					#ifdef DEBUG_JSON
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					myprintf("[%s:%d]:交采发送\n",(u32)__func__,(u32)__LINE__,0);
					for(i=0;i<50;i++)
					{	
						myprintf("%02x ",p8rx[i],0,0);
					}
					myprintf("\n",0,0,0);
					#endif
					*/
					(*(void(*)())p32[130])((u32)PORTn);//调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送
					/*
					#ifdef DEBUG_JSON
					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					myprintf("[%s:%d]交采接受\n",(u32)__func__,(u32)__LINE__,0);
					for(i=0;i<50;i++)
					{	
						myprintf("%02x ",p8tx[i],0,0);
					}
					myprintf("\n",0,0,0);
					#endif
					*/
					UARTCtrl->Task=1;//1=接收完成
			#else
			//内部交采抄表规约:3=DL/T698.45
				UARTCtrl->Task=0;//0=接收
				UARTCtrl->TxByte=0;
				//Uart_698_Data(PORTn);//UART_698数据
					Uart_698_Data(PORTn,0);//UART_698数据
				if(UARTCtrl->Task==2)
				{//2=等待单帧数据发送
					MR((u32)p8rx,(u32)p8tx,UARTCtrl->TxByte);
					UARTCtrl->Task=1;//1=接收完成
				}
			#endif
			}
			else
			{//RS485
				Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
				Start_UARTn_Tx(PORTn);//启动UART口发送
				UARTCtrl->Task=4;
			}
			
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{//RS485
				RS485_TxLED(PORTn);//RS485 LED发送指示灯
				RM_ADDR_Display(PORTn);//抄表地址显示
				i=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
				i>>=8;
				i&=0xff;
				RM_Monitor_TxNUM(i,1);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数
			}
	#if DEBUG==1//0=发行,1=调试
			if(PORTn==ACSAMPLEPORT)
			{
				RM_ADDR_Display(PORTn);//抄表地址显示
			}
	#endif
			break;
		case 4://4=正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				UARTCtrl->Task=5;
				p16timer[0]=RMRS485OverTimer/10;//DL645读表超时定时值ms
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
			}
			break;
		case 5://5=正在接收
			i=RM_Rx(PORTn);//抄表接收帧;入口:PORTn;返回:0=正在接收,1=完成1帧接收,2=接收超时
			switch(i)
			{
				case 0://0没完成
					return;
				case 1://1完成
					UARTCtrl->Task=0;
					break;
				default://2超时
					UARTCtrl->Task=2;//2=接收超时(或错误)
					return;
			}
			p16timer[0]=30/10;//下帧启动发送延时20MS后
		case 1://1=接收完成
		#if DEBUG_RS485_1toRS232==1//0=否,1=是;调试:RS485_1口收发数据打印到RS232口
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=否,1=是;调试:RS485_2口收发数据打印到RS232口
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->RMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{//RS485
				i=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
				i>>=8;
				i&=0xff;
				RM_Monitor_RxNUM(i,1);//采集任务监控;入口:ID=任务号,NUM_Rx=接收条数
			}
			UARTCtrl->Task=0;//空闲1次
			i=RM_RxDATA_CHECK(PORTn);//帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回:0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			if((i==1)||(i==3))
			{//1地址错,3数据标识错
				UARTCtrl->Task=2;//2=接收超时(或错误)
				return;
			}
			if(i==4)
			{//4主动上报(已处理)
				return;
			}
			UARTCtrl->RM_Rx_date_NULL=i;//抄表接收返回:0=有数据,!=0无数据(空数据)
			if(i==2)
			{//2控制码错(无数据)
			#if(USER/100)==17
				//曲线等冻结数据当指定时间点无数据冻结时写入空数据避免补抄时重新抄读
				i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
				x=(UARTCtrl->TaskID>>20)&0xf;//采集类型
				if((i==0x5002)&&(x==2))//按冻结时标采集的分钟数据
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:接收到台体报文是空数据的时候\n",(u32)__func__,(u32)__LINE__,0);
					#endif
					UARTCtrl->ReRMCount=0;//抄表重发计数
					UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
					return;
				}
			#endif
			#if (USER/100)==15//安徽
				//安徽:针对三相表需要支持06000001和061000FF等块抄读
				if(UARTCtrl->RM_DI==0x06000001)
				{
					UARTCtrl->Task=2;//2=接收超时(或错误)
					return;
				}
			#endif
			#if (USER&100)==0//电科院和省电科院测试
//---
				//台体测试采集监控不能算抄表成功计数
				UARTCtrl->ReRMCount=0;//抄表重发计数
				UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
				return;
//---
			#else
//---
				//曲线等冻结数据当指定时间点无数据冻结时写入空数据避免补抄时重新抄读
				i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
				x=(UARTCtrl->TaskID>>20)&0xf;//采集类型
				if(!((i==0x5002)||(x==4)))
				{//不是抄分钟冻结(采曲线数据)和补抄(2=事件采集方案时会填入空数据而上报)
					UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
					UARTCtrl->ReRMCount=0;//抄表重发计数
					return;
				}
			#if (USER/100)==15//安徽
				if((i==0x5002)&&(x==2))
				{
					i=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
					i>>=8;
					i&=0xff;
					p8=(u8*)ADDR_DATABUFF;
					Get_date_time_s(p8);
					i=YYMDHMS_Sub_hex(UARTCtrl->TaskTime[i].StdTime,p8);
					if(i<=600)//十分钟内如果回的都是空数据，会一直召读，超过十分钟以后，将空数据保存。
					{
						UARTCtrl->ReRMCount=0;//抄表重发计数
						UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
						return;
					}				
				}
			#endif
				
//---
			#endif
			}
			RM_RxDATA(PORTn);//抄表接收数据处理

			UARTCtrl->ReRMCount=0;//抄表重发计数
			if(UARTCtrl->RM_SubsequentFrames==0)//0=无读后续帧,1-n=读续帧的帧号
			{//无后续帧
				UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
			}
			return;
	
	}
}






