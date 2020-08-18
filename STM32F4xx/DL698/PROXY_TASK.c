

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/PROXY_TASK.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"


void PROXY_TASK_RS485(u32 PORTn)//代理任务RS485
{
	u32 i;
	u32 SourProtocol;
	u8* p8tx;
	u8* p8rx;
	u16* p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
	switch(i)
	{
		default:
			UARTCtrl->ProxyTxRxByte=0;
			UARTCtrl->ProxyTask=0;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			break;
		case 1://有代理
			UARTCtrl->ProxyTask=3;
			break;
		case 2://超时
			UARTCtrl->ProxyTxRxByte=0;
			UARTCtrl->ProxyTask=0;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			break;
		case 3://启动发送
			if(p16timer[0]!=0)//下帧启动发送延时
			{
				return;
			}
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			p8rx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
			i=UARTCtrl->ProxyTxRxByte;
			if(i>LEN_UART2Tx)
			{
				i=LEN_UART2Tx;
			}
			MR((u32)p8tx,(u32)p8rx,i);
			UARTCtrl->TxByte=i;
			i=UARTCtrl->BpsCtrl;
			UARTCtrl->BpsCtrl=UARTCtrl->ProxyBpsCtrl;//代理通信控制字(同规约2007)(半字对齐)
		#if DEBUG_RS485_1toRS232==1//0=否,1=是;调试:RS485_1口收发数据打印到RS232口
			if(PORTn==RS485_1PORT)
			{
				LOGRS232(UARTCtrl->ProxyRMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
		#if DEBUG_RS485_2toRS232==1//0=否,1=是;调试:RS485_2口收发数据打印到RS232口
			if(PORTn==RS485_2PORT)
			{
				LOGRS232(UARTCtrl->ProxyRMprotocol,1,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
			}
		#endif
			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
			Start_UARTn_Tx(PORTn);//启动UART口发送
			
			UARTCtrl->BpsCtrl=i;
			RS485_TxLED(PORTn);//RS485 LED发送指示灯
			PROXY_ADDR_Display(PORTn);//代理地址显示
			UARTCtrl->ProxyTask++;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			break;
		case 4://正在发送
			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
			{
				p16timer[0]=RMRS485OverTimer/10;//DL645读表超时定时值ms
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
				UARTCtrl->ProxyTask++;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			}
			break;
		case 5://正在接收
			SourProtocol=UARTCtrl->RMprotocol;
			UARTCtrl->RMprotocol=UARTCtrl->ProxyRMprotocol;
			i=RM_Rx(PORTn);//抄表接收帧;入口:PORTn;返回:0=正在接收,1=完成1帧接收,2=接收超时
			switch(i)
			{
				case 0://0没完成
					break;
				case 1://1完成
					p16timer[0]=30/10;//下帧启动发送延时20MS后
					p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
					p8tx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
					i=UARTCtrl->RxByte;
					if(i>LEN_UART2Rx)
					{
						i=LEN_UART2Rx;
					}
					MR((u32)p8tx,(u32)p8rx,i);
					UARTCtrl->ProxyTxRxByte=i;
					UARTCtrl->ProxyTask=0;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				#if DEBUG_RS485_1toRS232==1//0=否,1=是;调试:RS485_1口收发数据打印到RS232口
					if(PORTn==RS485_1PORT)
					{
						LOGRS232(UARTCtrl->ProxyRMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
					}
				#endif
				#if DEBUG_RS485_2toRS232==1//0=否,1=是;调试:RS485_2口收发数据打印到RS232口
					if(PORTn==RS485_2PORT)
					{
						LOGRS232(UARTCtrl->ProxyRMprotocol,0,PORTn,(u8 *)Get_ADDR_UARTnRx(PORTn),UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0
					}
				#endif
					break;
				default://2超时
					UARTCtrl->ProxyTxRxByte=0;
					UARTCtrl->ProxyTask=0;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
					break;
			}
			UARTCtrl->RMprotocol=SourProtocol;
			break;
	}
}

#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
void PROXY_TASK_3762(u32 PORTn)//代理任务376.2
{
	u32 i;
	u32 x;
	u32 Len;
	u16 * p16timer;
	u8 * p8rx;
	u8* p8tx;
  UARTCtrl_TypeDef* UARTCtrl;
	UARTCtrl_TypeDef* UARTCtrlProxy;
#if SEL_RouterRunMode3ADDR==1//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
	u8 * p8;
	u64 d64;
#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
	if(i)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
	{
		UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(UARTCtrl->ProxyPORTn);
		switch(i)
		{
			default:
				UARTCtrl->ProxyTxRxByte=0;
				UARTCtrl->ProxyTask=0;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				break;
			case 1://有代理
				UARTCtrl->ProxyTask=3;
				break;
			case 2://超时
				Terminal_Router->StopFlags&=~(1<<2);//恢复路由
				Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				UARTCtrl->ProxyTxRxByte=0;
				UARTCtrl->ProxyTask=0;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				break;
			case 3://暂停路由
				if(Terminal_Router->RouterCtrl!=0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				{
					break;
				}
				Terminal_Router->StopFlags|=(1<<2);//暂停路由标志
				Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				UARTCtrl->ProxyTask++;
				break;
			case 4://暂停路由接收
				if(Terminal_Router->RouterCtrl==0)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				{
					//清空接收FIFO
					while(1)
					{
						i=UART_3762_Rx(PORTn);//UART 3762接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
						if(i==1)
						{//1=完成1帧接收
//							Uart_3762_RxData(PORTn);//UART_3762接收数据;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
//							return;
							continue;
						}
						break;
					}
					UARTCtrl->ProxyTask++;
				}
				break;
			case 5://路由转发数据
				p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
				if(UARTCtrlProxy->RMprotocol==128)//通信协议自动识别:返回:0=未知,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
				{//转发376.2帧
					Len=UARTCtrl->ProxyTxRxByte;
					if(Len>LEN_UART2Tx)
					{
						Len=LEN_UART2Tx;
					}
					p8rx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
					MR((u32)p8tx,(u32)p8rx,Len);//转发内容
					UARTCtrl->TxByte=Len;//发送字节数(半字对齐)
				}
				else
				{//自组376.2帧
					p8tx[0]=0x68;
					if(Terminal_Router->RouterRunMode_1==2)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
					{
						p8tx[3]=0x43;//控制码;集中式路由载波通信
					}
					else
					{
						if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
						{
							p8tx[3]=0x4a;//控制码;微功率无线通信
						}
						else
						{
							p8tx[3]=0x41;//控制码;集中式路由载波通信
						}
					}
					p8tx[4]=4;//通信模块标识=1
					p8tx[5]=0;
					p8tx[6]=100;//预计应答字节数
					p8tx[7]=0;
					p8tx[8]=0;
					UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
					p8tx[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
					MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
					MR(((u32)p8tx)+16,(u32)&UARTCtrlProxy->RMAddr,6);//从节点地址(目的地址)
					Data_Inverse(((u32)p8tx)+16,6);//数据倒序(高低字节调换),入口Le>=2
					p8tx[22]=0x13;//AFN=13H
					p8tx[23]=1;//Fn=1
					p8tx[24]=0;
					//p8tx[25]=0;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
					if(UARTCtrlProxy->RMprotocol==128)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
					{
						p8tx[25]=0;//0透明传输
					}
					else
					{
					#if (USER%100)==0//电科院
						p8tx[25]=UARTCtrlProxy->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
					#else
						//p8tx[25]=0;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
						switch(UARTCtrlProxy->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
						{
							case 1://DL/T645-1997（1）
							case 2://DL/T645-2007（2）
								p8tx[25]=UARTCtrlProxy->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
								break;
							case 3://DL/T698.45（3）
								if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能,8=钱龙
								{//鼎信无698暂用透明
									p8tx[25]=0;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
								}
								else
								{
									p8tx[25]=UARTCtrlProxy->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
								}
								break;
							case 4://CJ/T 188-2004（4）
							default://
								p8tx[25]=UARTCtrlProxy->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）,128=DL376.2
								break;
						}
					#endif
					}
					p8tx[26]=0;//通信延时相关标志
					p8tx[27]=0;//附属节点数=0
					i=UARTCtrl->ProxyTxRxByte;
					if(i>(LEN_UART2Tx-30))
					{
						i=(LEN_UART2Tx-30);
					}
					p8tx[28]=i;//DL645报文长度(前导0xfe不加入)
					p8rx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
					MR(((u32)p8tx)+29,(u32)p8rx,i);//转发内容(前导0xfe不加入)
					Len=29+i+2;//需总发送字节数
					p8tx[1]=Len;
					p8tx[2]=Len>>8;
				#if SEL_RouterRunMode3ADDR==0//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
					
				#else
					if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
					{//微功率无线,档案地址用采集器地址
						x=AddrToMeterSetNo((u16*)&UARTCtrlProxy->RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
						p8=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
						p8=Get_Element(p8,3,0,0);//计算元素地址
						if((x>=NUM_RMmax)||(p8==0))
						{//没找到配置序号
							Terminal_Router->StopFlags&=~(1<<2);//恢复路由
							Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
							UARTCtrl->ProxyTxRxByte=0;
							UARTCtrl->ProxyTask=0;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
							break;
						}
						//d64=MRR((u32)p8+5,6);//取配置采集器地址
						d64=Get_TSA_Value(p8+3);//得到TSA的不大于6字节通信地址,数据顺序不变,入口:p8=TSA类型后的字节，若无效值则=0
						if((d64!=0xeeeeeeeeeeee)&&(d64!=0))
						{//采集器地址有效
							d64=R_Inverse(d64,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
							MWR(d64,((u32)p8tx)+16,6);//从节点地址(目的地址)
						}
					}
				#endif
					Uart_3762_CS((u32)p8tx);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					UARTCtrl->TxByte=Len;//发送字节数(半字对齐)
				}
				
			#if LOG_1376_2==1
				LOG(13762,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			#if DEBUG_RS485_4toRS232==1//0=否,1=是;调试:RS485_4(载波)口收发数据打印到RS232口
				LOGRS232(13762,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
				
				UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
				Start_UARTn_Tx(PORTn);//启动UART口发送
				
				UARTCtrl->ProxyTask++;
				PROXY_ADDR_Display(PORTn);//代理地址显示
				break;
			case 6://正在发送
				if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
				{
					p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
					p16timer[0]=UARTCtrl->TxToRxTimerOver;//发送后接收超时定时值ms/10(半字对齐)
					UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
					UARTCtrl->ProxyTask++;
				}
				break;
			case 7://接收
				i=UART_3762_Rx(PORTn);//UART 3762接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
				switch(i)
				{
					default:
						break;
					case 2:
						UARTCtrl->ProxyTask=2;//超时
						break;
					case 1:
						UARTCtrl->Task=0;//UART_3762_Rx()中如果完成1帧接收UARTCtrl->Task=1，故需清0；其他不会变化UARTCtrl->Task值
						p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
						p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
						//帧序号比较，电科院测试软件开始时不响应,测试开始后会响应上一帧数据
						if(p8rx[9]!=p8tx[9])
						{
							UARTCtrl->ProxyTask=7;//接收
							UARTCtrl->RxByte=0;//接收字节数(半字对齐)
							p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
							p16timer[0]=30000/10;//注:电科院测试软件等待90秒
							return;
						}
					#if LOG_1376_2==1
						LOG(13762,0,PORTn,p8rx,UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
					#if DEBUG_RS485_4toRS232==1//0=否,1=是;调试:RS485_4(载波)口收发数据打印到RS232口
						LOGRS232(13762,0,PORTn,p8rx,UARTCtrl->RxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
						if(UARTCtrlProxy->RMprotocol==128)//通信协议自动识别:返回:0=未知,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
						{//转发376.2帧
							Len=UARTCtrl->RxByte;
							p8tx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
							MW((u32)p8rx,(u32)p8tx,Len);
						}
						else
						{//自组376.2帧
							if(UARTCtrl->RxByte<=16)
							{//否认回答
								UARTCtrl->ProxyTask=2;//超时
								break;
							}
							Len=p8rx[28];//返回645帧总长度
							i=MRR(((u32)p8rx)+22,3);
							if(i!=0x113)
							{//AFN=13H,Fn=1
								UARTCtrl->ProxyTask=7;//接收
								UARTCtrl->RxByte=0;//接收字节数(半字对齐)
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								if(i==0x0200)
								{//AFN=00,Fn=2直接否认帧(微功率无线有此情况)
									p16timer[0]=6000/10;//6秒超时时间
								}
								else
								{
									//p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
									p16timer[0]=30000/10;//注:电科院测试软件等待90秒
								}
								return;
							}
							//查当前接收前导0xfe个数
							for(x=0;x<14;x++)
							{
								if(p8rx[29+x]!=0xfe)
								{
									break;
								}
							}
							if(Len>x)
							{
								Len-=x;
							}
							//地址比较
/*
					#if (USER/100)!=0//电科院测试不比较地址,因南瑞模块有bug
							MR(ADDR_DATABUFF,(u32)&UARTCtrlProxy->RMAddr,6);
							Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
						#if SEL_RouterRunMode3ADDR==0//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
						#else
							if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{//微功率无线,档案地址用采集器地址
								x=AddrToMeterSetNo((u16*)&UARTCtrlProxy->RMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
								p8=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
								p8=Get_Element(p8,3,0,0);//计算元素地址
								if((x>=NUM_RMmax)||(p8==0))
								{//没找到配置序号
									UARTCtrl->ProxyTask=7;//接收
									UARTCtrl->RxByte=0;//接收字节数(半字对齐)
									p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
									p16timer[0]=6000/10;//6秒超时时间
									break;
								}
								MR(ADDR_DATABUFF,(u32)p8+5,6);
								Data_Inverse(ADDR_DATABUFF,6);//数据倒序(高低字节调换),入口Le>=2
							}
						#endif
							x=DataComp(((u32)p8rx)+10,ADDR_DATABUFF,6);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
							if(x!=0)
							{//不相同
								UARTCtrl->ProxyTask=7;//接收
								UARTCtrl->RxByte=0;//接收字节数(半字对齐)
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								if(i==0x0200)
								{//AFN=00,Fn=2直接否认帧(微功率无线有此情况)
									p16timer[0]=6000/10;//6秒超时时间
								}
								else
								{
									p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
								}
								return;
							}
					#endif
*/
							p8tx=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);
							MW(((u32)p8rx)+28+1+x,(u32)p8tx,Len);
						}
						
						Terminal_Router->StopFlags&=~(1<<2);//恢复路由
						Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
						UARTCtrl->ProxyTxRxByte=Len;
						UARTCtrl->ProxyTask=0;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
						break;
				}
				break;
		}
	}
}
#endif










