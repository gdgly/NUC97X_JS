

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_3762_iActive.h"
#include "../DL698/Uart_3762_RxTx.h"
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


#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
void RM_3762_iActive(u32 PORTn)//集中器主动抄表
{
	u32 i;
	u32 y;
	u32 Info;
	u32 f;
  UARTCtrl_TypeDef * UARTCtrl;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 SetNo;
#if SEL_RouterRunMode3ADDR==1//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
	u8* pCSD;
	u64 d64a;
#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送(同RS485的3=启动发送)
	{
		case 0://0=空闲
			UARTCtrl->Task=6;//3;//启动发送
			break;
		case 2://2=接收超时(或错误)
			UARTCtrl->Task=0;//空闲1次
			break;
		case 3://3=启动发送
			break;
		case 6://6=抄表发送(同RS485的3=启动发送)
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			i=p16timer[0];
			if(i!=0)
			{
				if(i>(30/10))//下帧启动发送延时
				{
					p16timer[0]=30/10;
				}
				break;
			}
			p16timer[0]=30/10;//下帧启动发送延时
			UARTCtrl->RMMeterCountAuto=0;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
			i=UARTCtrl->TaskID;
			if(i==0)
			{
				p16timer[0]=1000/10;//下次计算TASK()在1秒后,否则会影响菜单速度
				UARTCtrl->Task=5;//正在接收
				RM_NUM_Display(PORTn);//抄表数量显示
				WarningOccur(PORTn,(u8 *)"载波 当前任务完成");//告警发生,入口告警代码和告警字符串
				if(UARTCtrl->RTCS_TASK!=Comm_Ram->DL698YMDHMS[6])//实时钟更新标志原始秒(Task运算)
				{
					UARTCtrl->RTCS_TASK=Comm_Ram->DL698YMDHMS[6];//实时钟更新标志原始秒(Task运算)
					TASK(PORTn,1);//采集任务;TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
				}
				return;
			}
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			if(UARTCtrl->ReRMCount>=2)//抄表重发计数
			{
				RM_OK_FAIL(PORTn,0);//抄表成功失败地址记录:入口:OK=0失败，=1成功
				UARTCtrl-> ReRMCount=0;//抄表重发计数
				UARTCtrl->RM_OKFAIL=0;//抄表成功失败标志;=0失败，=1成功,2=成功但无数据
				//下电能表
				RM_MeterCount(PORTn);//电能表计数+1;返回:0=没抄完,1=抄完
				TASK(PORTn,1);//采集任务;TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
				return;
			}
			if(UARTCtrl->RM_SubsequentFrames==0)//0=无读后续帧,1-n=读续帧的帧号
			{//非后续帧
				UARTCtrl->RMMeterCountAuto=0;//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
				i=RM_TxDATA_Manage(PORTn);//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				//注:RM_TxDATA_Manage(PORTn)会清UARTCtrl-> ReRMCount=0
				UARTCtrl-> ReRMCount++;//抄表重发计数
				if(UARTCtrl->TxByte==0)
				{
					i=UARTCtrl->RM_DI_TI_Count;//保护
					TASK(PORTn,1);//采集任务;TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
					if(UARTCtrl->CONNECT!=0)//DL698连接:0=空,1=启动连接,2=，100=连接,101=抄表中,102=关闭连接
					{
						UARTCtrl->RM_DI_TI_Count=i;//恢复
					}
					return;
				}
				if(UARTCtrl->RMprotocol==3)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
				{
					i=100;//预计应答字节数
				}
				else
				{
					i=0;//预计应答字节数
				}
			#if (USER/100)==0//电科院测试验
				Uart_3762_AFN13F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
			#else
				if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能,8=钱龙
				{//鼎信无698暂用透明
					if(UARTCtrl->RMprotocol==3)
					{
						Uart_3762_AFN13F1_Tx(PORTn,0,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
					}
					else
					{
						Uart_3762_AFN13F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
					}
				}
				else
				{
					Uart_3762_AFN13F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2监控从节点数据填入发送区;入口:端口,通信协议(1=1997,2=2007),标准645帧地址,645帧长度,预计应答字节数,通信延时相关标志
				}
			#endif
			#if SEL_RouterRunMode3ADDR==1//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
				if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
				{//微功率无线,目标地址用采集器地址
					i=UARTCtrl->TaskID;
					i>>=8;
					i&=0xff;
					switch(i)
					{
						case RMTASKnoPOWER://停上电事件固定任务号
							i=UARTCtrl->MeterCount[i];
							i=PowerEvent->setno[i];
							break;
						default:
							i=UARTCtrl->MeterCount[i];
							break;
					}
					if(i>=NUM_RMmax)
					{
						return;
					}
					d64a=0;
					pCSD=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
					pCSD=Get_Element(pCSD,3,0,0);//计算元素地址
					if(pCSD)
					{//采集器可取
						d64a=MRR((u32)pCSD+5,6);//取配置采集器地址(目的地址)
						d64a=R_Inverse(d64a,6);
					}
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8tx)+16,6);//从节点地址(目的地址)
						Uart_3762_CS((u32)p8tx);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
					}
				}
			#endif
			}
			else
			{//是后续帧
				UARTCtrl-> ReRMCount++;//抄表重发计数
				//p8tx[0]=0x68;
				p8tx[1]=(29+17+2);//需总发送字节数
				p8tx[2]=(29+17+2)>>8;
				//p8tx[3]=0x41;//控制码为0x41
				//p8tx[4]=4;//通信模块标识=1
				//p8tx[5]=0;
				p8tx[6]=0;//预计应答字节数
				//p8tx[7]=0;
				//p8tx[8]=0;
				UARTCtrl->TxSEQ++;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
				p8tx[9]=UARTCtrl->TxSEQ;//启动帧序列域SEQ(每次加1后发送(非发送后加1),等待响应时不变,以便比较)
				//MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//主节点地址(源地址)
				//MR(((u32)p8tx)+16,ADDR_Data+1,6);//从节点地址(目的地址)
				//p8tx[22]=0x13;//AFN=13H
				//p8tx[23]=1;//Fn=1
				//p8tx[24]=0;
				//p8tx[25]=2;//转发数据标识类型=0透明传输,1=DL645-1997,2=DL645-2007
				//p8tx[26]=0;//通信延时相关标志
				//p8tx[27]=0;//附属节点数=0
				p8tx[28]=17;//DL645报文长度
				p8tx[29+8]=0x12;
				p8tx[29+9]=5;
				p8tx[29+14]=UARTCtrl->RM_SubsequentFrames+0x33;
				p8tx[29+15]=0;//cs
				for(i=0;i<15;i++)
				{
					p8tx[29+15]+=p8tx[29+i];
				}
				p8tx[29+16]=0x16;
				Uart_3762_CS((u32)p8tx);//和校验计算并写入(同时写入结束0x16)和比较,返回0=正确1=错误,入口接收或发送缓冲
				UARTCtrl->TxByte=31+p8tx[28];//发送字节数(半字对齐)
			}
			if(UARTCtrl->TxByte==0)
			{
				return;
			}
			UARTCtrl->TxByte=31+p8tx[28];//发送字节数(半字对齐)
			i=Terminal_Router->MaxOverTime*(1000/10);//从节点监控最大超时时间秒
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;//启动发送
			RM_ADDR_Display(PORTn);//抄表地址显示
			i=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
			i>>=8;
			i&=0xff;
			RM_Monitor_TxNUM(i,1);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数
			break;
		case 1://1=接收完成
			i=UARTCtrl->TaskID;//b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
			i>>=8;
			i&=0xff;
			RM_Monitor_RxNUM(i,1);//采集任务监控;入口:ID=任务号,NUM_Rx=接收条数
			SetNo=UARTCtrl->MeterCount[i];//2*RMTASKmax byte各任务已抄电能表计数,主动上报时用于文件中已上报电能表计数
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			p16timer[0]=30/10;//下帧启动发送延时20MS后
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//载波路由器死机秒定时值
		
			UARTCtrl->Task=0;//空闲1次
			i=p8rx[1]+(p8rx[2]<<8);
			if(i<=16)
			{//否认回答
				return;
			}
			i=p8rx[22]+(p8rx[23]<<8)+(p8rx[24]<<16);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			#if (USER/100)==0//电科院测试
				p16timer[0]=1000/10;
			#else
				p16timer[0]=30000/10;
			#endif
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=正在接收
				return;
			}
			y=p8rx[28];//返回数据域长度
			if(y==0)
			{//接收数据长度=0
				return;
			}
			i=p8rx[25]+(p8rx[26]<<8);//线路上传输延时时间秒
			UARTCtrl->TransferDelay=i;
			
			Info=p8rx[4]+(p8rx[5]<<8)+(p8rx[6]<<16)+(p8rx[7]<<24);
			f=p8rx[8];//鼎信模块用暂存主动上报标志
			f=f;
			MR((u32)p8rx,((u32)p8rx)+28+1,y);
			for(i=0;i<y;i++)
			{//去返回帧的前导0xfe
				if(p8rx[i]!=0xfe)
				{
					if(i!=0)
					{
						MR((u32)p8rx,((u32)p8rx)+i,y-i);
					}
					break;
				}
			}
			i=RM_RxDATA_CHECK(PORTn);//帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			if((i==1)||(i==3))
			{//1地址错,3数据标识错
			#if (USER/100)==0//电科院测试
				UARTCtrl->Task=2;//2=接收超时(或错误)
			#else
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=30000/10;
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=正在接收
			#endif
				return;
			}
			if(i==4)
			{//4主动上报(已处理)
			#if (USER/100)==0//电科院测试
				
			#else
				//继续接收
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=30000/10;
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=正在接收
			#endif
				return;
			}
			UARTCtrl->RM_Rx_date_NULL=i;//抄表接收返回:0=有数据,!=0无数据(空数据)
			if(i==2)
			{//2控制码错(无数据)
//				UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
//				UARTCtrl->ReRMCount=0;//抄表重发计数
//				return;
			#if (USER/100)==15//安徽
				//安徽:针对三相表需要支持06000001和061000FF等块抄读
				if(UARTCtrl->RM_DI==0x06000001)
				{
					UARTCtrl->Task=2;//2=接收超时(或错误)
					return;
				}
			#endif
				//曲线等冻结数据当指定时间点无数据冻结时写入空数据避免补抄时重新抄读
				i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
				Info=(UARTCtrl->TaskID>>20)&0xf;//采集类型
				if(!((i==0x5002)||(Info==4)))
				{//不是抄分钟冻结(采曲线数据)和补抄(2=事件采集方案时会填入空数据而上报)
					UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
					UARTCtrl->ReRMCount=0;//抄表重发计数
					return;
				}
			}
			RM_RxDATA(PORTn);//抄表接收数据处理
			RM_State_RMUpDate(PORTn,1,SetNo,(Info>>4)&0xf,(Info>>16)&0xf);//采集状态,每次抄表后更新:入口:OK=0失败=1成功,OK=1时Class=中继级别,Phase=相位
			
			UARTCtrl->ReRMCount=0;//抄表重发计数
			UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
	}
}

#endif


