
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_3762.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "../DL698/RM_3762_RouterActive.h"
#include "../DL698/RM_3762_iActive.h"





#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void DisplayRouterVer(u32 PORTn)//显示路由厂家和版本号
{
	u32 i;
	u8*p8;
	switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
	{
		case 1:
			CopyString((u8 *)"正泰载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 2:
			CopyString((u8 *)"鼎信载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 3:
			CopyString((u8 *)"东软载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 4:
			CopyString((u8 *)"晓程载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 5:
			CopyString((u8 *)"瑞斯康  ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 6:
			CopyString((u8 *)"力合微  ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 7:
			CopyString((u8 *)"讯能载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 8:
			CopyString((u8 *)"钱龙载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 9:
			CopyString((u8 *)"华瑞载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
			
		case 101:
			CopyString((u8 *)"麦希载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 102:
			CopyString((u8 *)"友讯达  ",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 103:
			CopyString((u8 *)"有方载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
		case 104:
			CopyString((u8 *)"桑锐载波",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			break;
			
		default:
			//WarningOccur(PORTn,(u8 *)"未知载波");//告警发生,入口告警代码和告警字符串
			return;
	}
	p8=(u8 *)(ADDR_DATABUFF+8);
	p8[0]=(Terminal_Router->RouterVer[2]>>4)+0x30;
	p8[1]=(Terminal_Router->RouterVer[2]&0xf)+0x30;
	p8[2]=(Terminal_Router->RouterVer[1]>>4)+0x30;
	p8[3]=(Terminal_Router->RouterVer[1]&0xf)+0x30;
	p8[4]=(Terminal_Router->RouterVer[0]>>4)+0x30;
	p8[5]=(Terminal_Router->RouterVer[0]&0xf)+0x30;
	p8[6]='V';
	i=(Terminal_Router->RouterVer[4]>>4);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[7]=i;
	i=(Terminal_Router->RouterVer[4]&0xf);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[8]=i;
	i=(Terminal_Router->RouterVer[3]>>4);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[9]=i;
	i=(Terminal_Router->RouterVer[3]&0xf);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[10]=i;
	p8[11]=0;
	//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
}


void Terminal_ReadMeter_3762(u32 PORTn)//终端抄表Q/GDW376.2
{
	u32 i;
	u64 d64a;
	u8 * p8rx;
	u8 * p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;
//	u32 * p32;
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	if(UARTCtrl->Task>6)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送
	{//容错
		UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收,6=抄表发送
	}
	switch(Terminal_Router->RouterInitTask)
	{//模块状态
		case 0://上电或复位(关电源)
//			Pin_Configure(PIN_UART4_RX,//RXD输入,下拉
//									0|//B3-B0:复用功能选择AF(0-15)
//						 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
//						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
//						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
//						 (2<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
//						(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
//			Pin_Configure(PIN_UART4_TX,//TXD输入,下拉
//									0|//B3-B0:复用功能选择AF(0-15)
//						 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
//						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
//						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
//						 (2<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
//						(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
			Pin_Configure(PIN_PLC_RST,//复位引脚输出0
									0|//B3-B0:复用功能选择AF(0-15)
						 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
						 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
						(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
			i=Comm_Ram->MYMDHMS[1]+(Comm_Ram->MYMDHMS[2]<<8);//时分
			if(i<0x0010)
			{//是小于0时10分,关电源
				Pin_Configure(PIN_PLC_PWR,//复位引脚输出0
										0|//B3-B0:复用功能选择AF(0-15)
							 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
							 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
							 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
							 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
							(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
				p16timer[0]=2000/10;//延时(关电源)
			}
			else
			{//不关电源
				p16timer[0]=0;//关闭电源延时秒
			}
			Pin_Configure(PIN_PLC_ID,//模块插入引脚,输入上拉
									0|//B3-B0:复用功能选择AF(0-15)
						 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
						 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
						(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
			if(Pin_Read(PIN_PLC_ID)==0)
			{
				Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			else
			{
				//WarningOccur(PORTn,(u8 *)"载波模块没插入");//告警发生,入口告警代码和告警字符串
			}
			break;
		case 1://开电源
			if(p16timer[0]==0)
			{
				Pin_Configure(PIN_PLC_RST,//复位引脚输出1
										0|//B3-B0:复用功能选择AF(0-15)
							 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
							 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
							 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
							 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
							(1<<11));//B11  :当为输出时：0=输出0,1=输出1		
				Pin_Configure(PIN_PLC_PWR,//复位引脚输出0
										0|//B3-B0:复用功能选择AF(0-15)
							 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
							 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
							 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
							 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
							(1<<11));//B11  :当为输出时：0=输出0,1=输出1	
				p16timer[0]=1000/10;//模块自行初始化延时
				Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			break;
		case 2://复位
			if(p16timer[0]==0)
			{
				Pin_Configure(PIN_PLC_RST,//复位引脚输出0
										0|//B3-B0:复用功能选择AF(0-15)
							 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
							 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
							 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
							 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
							(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
				p16timer[0]=500/10;//模块自行初始化延时
				Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			break;
		case 3://复位解除
			if(p16timer[0]==0)
			{
				p16timer=(u16 *)Get_ADDR_UARTn(PORTn);
				p16timer[0]=0;//中断接收指针
				p16timer[1]=0;//当前已读指针
				UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
//				UARTCtrl->LockFlags&=0xfd;//通信功能被禁止标志:B0=保留,B1=正在自动搜表,B2=本日已自动搜表,B3=保留,B4=主动定时任务1类数据,B5=主动定时任务2类数据,B6=,B7=保留
//				UARTCtrl->RMMeterEvent=0;//正在抄读电能表事件标志;b0=主动上报事件,b1=
				UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-255=DL645读续帧的帧号
				Terminal_Router->RUNState=0;//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
				Terminal_Router->StopFlags=0;//暂停路由标志
				Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
				Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
				UARTCtrl->BpsCtrl=0xb+(6<<5);//9600,8e1
				Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
				Pin_OutH(PIN_PLC_RST);//引脚输出高,入口引脚使用名
				Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			break;
		case 4://模块自行初始化延时
		#if (USER/100)==0
			Terminal_Ram->RouterCheck_S_Timer=60;//60S,等待路由开机自动上报帧
		#else
			Terminal_Ram->RouterCheck_S_Timer=10;//10S,等待路由开机自动上报帧
		#endif
			Terminal_Router->RouterInitSubTask=0;//子任务
			Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
		case 5://等待路由开机自动上报帧
			switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				case 1://1=完成1帧发收
					Terminal_Router->RouterInitSubTask=0;//子任务
					Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
				case 0://0=空闲
				case 2://2=接收超时(或错误)
					if(Terminal_Ram->RouterCheck_S_Timer!=0)//24 载波路由器死机秒定时器
					{
						UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
						break;
					}
					Terminal_Router->RouterInitSubTask=0;//子任务
					Terminal_Router->RouterInitTask+=2;//不回确认帧
					break;
			}
			break;
		case 6://回确认帧(主动上报厂商代码的响应,发确认帧停止)
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
					i=p8rx[13];
					Terminal_Router->RouterRunMode_1=i&0x0f;//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
					MWR(0xffffffff,((u32)p8tx)+128,6);
					Uart_3762_TxData(PORTn,0,1,6);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://等待完成
					if((UARTCtrl->Task==3)||(UARTCtrl->Task==4))//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						break;
					}
					if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x030A)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
					{//正确收到AFN3F10上报帧
						Terminal_Router->RouterInitTask+=2;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
						Terminal_Router->RouterInitSubTask=3;//子任务
						UARTCtrl->Task=1;//1=完成1帧发收
						break;
					}
					Terminal_Router->RouterInitSubTask=0;//子任务
					Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 7://
			//东软模块复位后响应第1帧需3抄左右,故加入3抄延时
			p16timer[0]=3000/10;//发送后接收超时定时值ms/10(半字对齐)
			Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
		case 8://读本地模块运行模式
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
					if(p16timer[0]==0)//发送后接收超时定时值ms/10(半字对齐)
					{
						Terminal_Router->RouterInitSubTask++;
					}
					break;
				case 1://
					if(UARTCtrl->Task<=2)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						if(UARTCtrl->Task!=2)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						{//上电后可能已接到多帧上报,收完多帧
							UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
							UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
						}
						else
						{
							Terminal_Router->RouterInitSubTask++;
						}
					}
					break;
				case 2:
					Uart_3762_TxData(PORTn,3,10,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask++;
					break;
				case 3:
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)!=0x030A)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								break;
							}
//							Comm_Ram->SelfTestFlags|=(1<<16);//B16=载波模块(自动)
							UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
							i=p8rx[13+30]+(p8rx[13+31]<<8);
							switch(i)
							{
							//电力线
								case (('5'<<8)+'0')://正泰
									Comm_Ram->RouterType=1;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
									break;
								case (('T'<<8)+'C')://鼎信
									Comm_Ram->RouterType=2;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
									break;
								case (('E'<<8)+'S')://东软
									Comm_Ram->RouterType=3;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
									break;
								case (('C'<<8)+'X')://晓晨
									Comm_Ram->RouterType=4;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
									break;
								case (('0'<<8)+'1')://瑞斯康
									Comm_Ram->RouterType=5;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
									break;
								case (('L'<<8)+'H')://力合微
									Comm_Ram->RouterType=6;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
									break;
								case (('N'<<8)+'X')://讯能
									Comm_Ram->RouterType=7;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能
									break;
								case (('Q'<<8)+'L')://钱龙
									Comm_Ram->RouterType=8;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能
									break;
								case (('R'<<8)+'H')://华瑞
									Comm_Ram->RouterType=9;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能
									break;
									
							//小无线
								case (('M'<<8)+'X')://麦希
									Comm_Ram->RouterType=101;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=;100=保留,101=麦希,102=友讯达,103=有方
									break;
								case (('F'<<8)+'C')://友讯达
									Comm_Ram->RouterType=102;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=;100=保留,101=麦希,102=友讯达,103=有方
									break;
								case (('N'<<8)+'W')://有方
									Comm_Ram->RouterType=103;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=;100=保留,101=麦希,102=友讯达,103=有方
									break;
								case (('S'<<8)+'R')://桑锐
									Comm_Ram->RouterType=104;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=;100=保留,101=麦希,102=友讯达,103=有方
									break;
									
								default:
									Comm_Ram->RouterType=0;//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=
									break;
							}
							
							i=p8rx[13];
							Terminal_Router->RouterRunMode_1=i&0x0f;//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							Terminal_Router->RouterRunMode_2=(i>>4)&0x1;//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
							if(Comm_Ram->RouterType==7)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能
							{//讯能此项有误
								Terminal_Router->RouterRunMode_2=0;//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
							}
							Terminal_Router->RouterRunMode_3=(i>>5)&0x1;//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
							Terminal_Router->RouterRunMode_4=(i>>6)&0x3;//路由运行模式_周期抄表模式：01表示通信模块仅支持集中器主导的周期抄表模式，10表示通信模块仅支持路由主导的周期抄表模式；11表示通信模块对两种抄表模式都支持；00保留
							i=p8rx[13+1];
							Terminal_Router->RouterRunMode_5=i&0x07;//路由运行模式_传输延时参数支持：由D2、D1、D0等三个位表示，分别代表广播、从节点监控、路由主动抄表等三种命令支持向集中器提供传输延时参数的情况，1表示支持提供传输延时参数，0表示不支持
							Terminal_Router->RouterRunMode_6=(i>>3)&0x3;//路由运行模式_失败节点切换发起方式：D0置1表示通信模块自主切换待抄节点，D1置1表示集中器发起通知通信模块切换待抄节点
							Terminal_Router->RouterRunMode_7=(i>>5)&0x1;//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
							Terminal_Router->RouterRunMode_8=(i>>6)&0x3;//路由运行模式_广播命令信道执行方式：0表示执行广播命令不需要信道标识，1表示广播命令要根据报文中的信道标识逐个来发送，其它取值保留
							i=p8rx[13+6];
							Terminal_Router->MaxOverTime=i;//从节点监控最大超时时间秒
							
							i=p8rx[13+7]+(p8rx[13+8]<<8);
							Terminal_Router->MaxBroadcastOverTime=i;//最大广播超时时间秒
							i=p8rx[13+9]+(p8rx[13+10]<<8);
							Terminal_Router->MaxTxByte=i;//最大支持报文长度(半字对齐)
							i=p8rx[13+11]+(p8rx[13+12]<<8);
							Terminal_Router->MaxFileByte=i;//文件传输支持的最大单个数据包长度(半字对齐)
							i=p8rx[13+13];
							Terminal_Router->MaxUpDateTime=i;//升级操作最大等待时间秒(半字对齐)
							i=p8rx[13+20]+(p8rx[13+21]<<8);
							if(Comm_Ram->RouterType==102)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=;100=保留,101=麦希,102=友讯达,103=有方
							{//友讯达支持的最大从节点数-1
								i--;
							}
							Terminal_Router->MaxNoteNum=i;//支持的最大从节点数(半字对齐)
							MR((u32)&Terminal_Router->RouterCODE,((u32)p8rx)+13+30,4);//厂商代码 ASCII 2,芯片代码 ASCII 2
							Data_Inverse((u32)&Terminal_Router->RouterCODE,2);//数据倒序(高低字节调换),入口Le>=2
							Data_Inverse(((u32)&Terminal_Router->RouterCODE)+2,2);//数据倒序(高低字节调换),入口Le>=2
							MR((u32)&Terminal_Router->RouterVer,((u32)p8rx)+13+34,5);//路由器版本号,分别为:3BYTE日月年,2BYTE版本

							DisplayRouterVer(PORTn);//显示路由厂家和版本号
							Terminal_Router->RouterInitReCount=0;//命令重发计数
							Terminal_Router->RouterInitSubTask=0;//子任务
							Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->RouterInitReCount<2)
							{
								Terminal_Router->RouterInitReCount++;
								Uart_3762_TxData(PORTn,3,10,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
								return;
							}
							//Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							
							Comm_Ram->GPMSTimer=0;//测试定时到
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 9://读主节点地址
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://发送
					Uart_3762_TxData(PORTn,0x03,4,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask=1;
					break;
				case 1://接收
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)!=0x0304)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=03H,Fn=4
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								break;
							}
							d64a=MRR(ADDR_4001+2,6);
							d64a=R_Inverse(d64a,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
							MWR(d64a,(u32)&Terminal_Router->RouterAddr,6);
							i=DataComp((u32)&Terminal_Router->RouterAddr,((u32)p8rx)+13,6);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
							if(i==0)
							{//地址相同跳过设置
								Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							}
							Terminal_Router->RouterInitReCount=0;//命令重发计数
							Terminal_Router->RouterInitSubTask=0;//子任务
							Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->RouterInitReCount<2)
							{
								Terminal_Router->RouterInitReCount++;
								Uart_3762_TxData(PORTn,0x03,4,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
								return;
							}
							Terminal_Router->RouterInitReCount=0;//命令重发计数
							Terminal_Router->RouterInitSubTask=0;//子任务
							Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 10://设置主节点地址(行政区域码+终端地址)
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
					d64a=MRR(ADDR_4001+2,6);
					d64a=R_Inverse(d64a,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					MWR(d64a,(u32)&Terminal_Router->RouterAddr,6);
				
					MR(((u32)p8tx)+128,(u32)&Terminal_Router->RouterAddr,6);
					Uart_3762_TxData(PORTn,5,1,6);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask=1;
					break;
				case 1:
					i=Uart_3762_RxData_AFN00_F1(PORTn,5,1,6);//UART_3762接收AFN00F1确认数据,入口AFN,Fn,LEN_Data接收失败重发用参数;返回0未确认1确认2失败
					if(i==2)
					{//设置主节点地址失败
						Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 11://
			Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
		case 12://参数初始化(清档案信息,恢复至出厂设置)
/*
			i=MRR(ADDR_AFN01Fx+5,1);//复位命令的Fn值:1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
			if(i>=3)
			{
				switch(Terminal_Router->RouterInitSubTask)
				{
					case 0://发送
						Uart_3762_TxData(PORTn,0x01,2,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
						Terminal_Router->RouterInitSubTask=1;
						break;
					case 1://接收
						Uart_3762_RxData_AFN00_F1(PORTn,0x01,2,0);//UART_3762接收AFN00F1确认数据,入口AFN,Fn,LEN_Data接收失败重发用参数;返回0未确认1确认2失败
						//只发3次若失败则放弃转到下1步
						break;
					default:
						Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
						break;
				}
			}
			else
			{
				Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
*/
			Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
		case 13://数据初始化(节点侦听信息)
/*
			i=MRR(ADDR_AFN01Fx+5,1);//复位命令的Fn值:1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
			if(i>=2)
			{
				switch(Terminal_Router->RouterInitSubTask)
				{
					case 0:
						Uart_3762_TxData(PORTn,0x01,3,0);
						Terminal_Router->RouterInitSubTask=1;
						break;
					case 1:
						Uart_3762_RxData_AFN00_F1(PORTn,0x01,3,0);//UART_3762接收AFN00F1确认数据,入口AFN,Fn,LEN_Data接收失败重发用参数;返回0未确认1确认2失败
						//只发3次若失败则放弃转到下1步
						break;
					default:
						Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
						break;
				}
			}
			else
			{
				Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
*/
			Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
		case 14://设初始档案同步标志
			//MC(0,ADDR_AFN01Fx+5,1);//清复位命令的Fn值
		
			UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			Terminal_Router->RouterSynTask=1;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
			Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
		case 15://初始档案同步过程
			Terminal_RouterSyn(PORTn);//路由器档案同步
			//显示电表配置同步数量
			i=CopyString((u8 *)"档案同步",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			i+=HEX_ASCII(MRR((u32)&Terminal_Router->NoteNum,2),ADDR_DATABUFF+i);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
			MWR(0,ADDR_DATABUFF+i,1);
			//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			if(Terminal_Router->RouterSynTask==0)//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
			{
				Terminal_Router->RouterInitReCount=0;//命令重发计数
				Terminal_Router->RouterInitSubTask=0;//子任务
				Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			break;
		case 16://读无线信道组
			Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
		/*
			if(Terminal_Router->RouterRunMode_1!=3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{
				Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			else
			{
				switch(Terminal_Router->RouterInitSubTask)
				{
					case 0:
						Uart_3762_TxData(PORTn,0x03,8,0);
						Terminal_Router->RouterInitSubTask++;
						break;
					case 1:
						switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						{
							case 1://1=完成1帧发收
								if(Get_Uart_3762_RxData_AFN_Fn(PORTn)!=0x0308)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
								{
									UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
									UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
									p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
									break;
								}
								UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
								i=p8rx[13];
								Terminal_Router->RadioChannel=i;//无线信道组:0～63，64～253：保留，254：自动选择，255：保持不变
								i=p8rx[14];
								Terminal_Router->RadioPower=i;//无线主节点发射功率:00：最高发射功率，01：次高发射功率，02：次低发射功率，03：最低发射功率，04～254：保留，255：保持不变
								Terminal_Router->RouterInitSubTask=0;//子任务
								Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
								break;
							case 0://0=空闲
							case 2://2=接收超时(或错误)
								Terminal_Router->RouterInitSubTask=0;//子任务
								Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
								break;
						}
						break;
					default:
						Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
						break;
				}
			}
		*/
			break;
		case 17://
//			if(Terminal_Ram->RMDelay_S_Timer!=0x0)//10 上电后延时抄表秒定时器
//			{
//				CopyString((u8 *)"载波等待上电",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
//				MWR(0,ADDR_DATABUFF+12,1);
//				//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
//				return;
//			}
			Terminal_Router->RouterInitTask=100;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;

			
		case 100://重启路由
			Terminal_Router->RouterCtrl=1;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
			Terminal_Router->RouterInitTask++;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
		case 101:
			Terminal_Router->RouterInitTask=200;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;

		
		case 200://>=200抄表
		//路由软件更新
/*
			p32=(u32*)(ADDR_E4_ENTRY);
			if((p32[64]==ADDR_E4_ENTRY)||(Terminal_Router->UpDateTask!=0))//软件更新任务
			{
				UpDateRouter(PORTn);//路由软件更新
				break;
			}
*/
		//路由器档案同步
			Terminal_RouterSyn(PORTn);//路由器档案同步
			if(Terminal_Router->RouterSynTask>=2)//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
			{//正在同步或同步刚正将标志设为退出恢复路由时
				//抄表暂停间隔时间清0命有表加入时启动抄表
				if(UARTCtrl->TaskID==0)//0=抄表间隔暂停,1=抄表进行中
				{
//					p16timer=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
//					p16timer[0]=0;
				}
				//显示电表配置同步数量
				i=CopyString((u8 *)"档案同步",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				i+=HEX_ASCII(MRR((u32)&Terminal_Router->NoteNum,2),ADDR_DATABUFF+i);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
				MWR(0,ADDR_DATABUFF+i,1);
				//WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
				return;
			}
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//0=空闲,2=超时
			{
				if((UARTCtrl->SetChange6000==0x55)&&(Terminal_Ram->RouterSynDelay_S_Timer==0))//采集档案配置表有变化标志:0=没变化,0x55=变化
				{
					UARTCtrl->SetChange6000=0;//采集档案配置表有变化标志:0=没变化,0x55=变化
					Terminal_Router->RouterSynTask=1;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
					break;
				}
			}
		//抄表
			if(Terminal_Router->NoteNum==0)
			{//节点表空
				UARTCtrl->Task=0;//0=空闲,以便其他任务
				DisplayRouterVer(PORTn);//显示路由厂家和版本号
				return;
			}
			if((Terminal_Router->RouterRunMode_4!=1)&&((Terminal_Router->StopFlags&(1<<17))==0))//路由运行模式_周期抄表模式:01表示通信模块仅支持集中器主导的周期抄表模式，10表示通信模块仅支持路由主导的周期抄表模式；11表示通信模块对两种抄表模式都支持；00保留
			{//支持路由主导的周期抄表模式
				i=UARTCtrl->TaskID;
				i>>=8;
				i&=0xff;
				if(i==1)
				{//约定任务号1路由主动抄,其他任务为点抄
					//路由抄读
					if((Terminal_Router->RUNState&2)==0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
					{
						
//						if(Terminal_Router->StopFlags==0)//暂停路由标志
//						{
							Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
							Terminal_Router->RouterInitTask=201;//延时路由恢复
							break;
//						}
					}
					//RM_3762_RouterActive(PORTn);//载波抄表(路由模块主导的周期抄表模式)
				}
				else
				{//约定任务号1路由主动抄,其他任务为点抄
					if((Terminal_Router->RUNState&2)!=0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
					{
						Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
						break;
					}
					//RM_3762_iActive(PORTn);//载波抄表(集中器主导的周期抄表模式)
				}
			}
			else
			{//01表示通信模块仅支持集中器主导的周期抄表模式;或主动上报
			#if (USER/100)!=0//不是电科院送检
				if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
				{//微功率无线,需路由学习完成后才能抄表,否则直接返回否认
					if((Terminal_Router->RUNState&1)==0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
					{
						if(Terminal_Router->NoteNumOver==0)//AFN04F10配置的从节点数>模块支持的最大从节点数;0=否,1=是
						{
							////WarningOccur(PORTn,(u8 *)"正在无线组网");//告警发生,入口告警代码和告警字符串
						}
						else
						{
							////WarningOccur(PORTn,(u8 *)"从节点数超限!");//告警发生,入口告警代码和告警字符串
						}
						p16timer=(u16 *)&Terminal_Ram->Trip3_MS_Timer;//终端跳闸3 10MS定时器
						if(p16timer[0]==0)
						{
							p16timer[0]=10000/10;//下次读在10秒后
							Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
						}
						break;
					}
				}
			#endif
				//RM_3762_iActive(PORTn);//载波抄表(集中器主导的周期抄表模式)
			}
			break;
		case 201://延时路由恢复
			if(Terminal_Ram->RouterCtrl_MS_Timer==0)//16 路由控制恢复延时 10MS定时器
			{
				Terminal_Router->RouterCtrl=5;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
				Terminal_Router->RouterInitTask=202;//路由恢复检查
			}
			//WarningOccur(PORTn,(u8 *)"载波路由恢复延时");//告警发生,入口告警代码和告警字符串
			switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				case 0://0=空闲
					UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
					p16timer[0]=100/10;//发送后接收超时定时值ms/10(半字对齐)
					UARTCtrl->Task=5;//接收
					break;
				case 1://1=完成1帧发收
				case 2://2=接收超时(或错误)
					UARTCtrl->Task=0;
					break;
			}
			break;
		case 202://路由恢复检查
			if((Terminal_Router->RUNState&2)==0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
			{
				Terminal_Router->RouterInitTask=0;//恢复失败
			}
			else
			{
				Terminal_Router->RouterInitTask=200;//恢复成功
			}
			break;
			
		default:
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
	}
}


void Terminal_RouterSyn(u32 PORTn)//路由器档案同步
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u64 d64a;
	u64 d64b;
	u8* p8rx;
	u8* p8tx;
	UARTCtrl_TypeDef * UARTCtrl;
	u16* p16timer;
	u64 d64c;
	u8 *p8s;
	u8* p8d;
	
	d64c=d64c;
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(Terminal_Router->RouterSynTask)//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
	{
		default:
			Terminal_Router->RouterSynTask=0;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
			break;
		case 1://1=等待启动
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
				{
					Terminal_Router->StopFlags|=(1<<8);//暂停路由
					Terminal_Router->RouterCtrl=2;//路由器控制命令0=空,1=重启,2=暂停,3=恢复,4=检测;完成后自动回到0
				}
				if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
				{//小无线,同步后总是置学习完成标志=0
					Terminal_Router->RUNState=0;//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
				}
			#if (USER/100)==0//电科院测试
				Terminal_Router->NoteNum=0;//电科院测试总是从0开始同步
			#endif
				Terminal_Router->RouterInitSubTask=0;//子任务
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
			}
			break;
		case 2://查询从节点数量
		#if (USER/100)==0//电科院测试
			if(Terminal_Router->RouterRunMode_2==0)//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
			{
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
				break;
			}
		#else
			if(Terminal_Router->RouterRunMode_3==0)//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
			{//0表示不需要下发测量点信息
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
				break;
			}
		#endif
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://发送
					Uart_3762_TxData(PORTn,0x10,1,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://接收
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=10H,Fn=1
								x=p8rx[13]+(p8rx[14]<<8);
								if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
								{//微功率无线,总已知节点数=0
									Terminal_Router->NoteNum=0;//总已知节点数总是从0开始
								}
								else
								{//电力线载波
									if(Terminal_Router->NoteNum!=x)
									{
										Terminal_Router->NoteNum=0;//已知节点数
									}
								}
								Terminal_Router->ModeNoteNum=x;//从模块读取的模块返回从节点数(半字对齐)
								Terminal_Router->RouterInitReCount=0;//命令重发计数
								Terminal_Router->RouterInitSubTask=0;//子任务
								Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							}
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->RouterInitReCount<2)//命令重发计数
							{
								Terminal_Router->RouterInitReCount++;//命令重发计数
								Terminal_Router->RouterInitSubTask=0;//子任务=发送
							}
							else
							{
								Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 3://读取从节点信息
		#if (USER/100)==0//电科院测试
			if(Terminal_Router->RouterRunMode_2==0)//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
			{
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
				break;
			}
		#else
			if(Terminal_Router->RouterRunMode_3==0)//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
			{//0表示不需要下发测量点信息
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
				break;
			}
		#endif
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://清当前路由器中从节点数
					if(Terminal_Router->ModeNoteNum==0)//从模块读取的模块返回从节点数(半字对齐)
					{
						Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
						return;
					}
					Terminal_Router->TempCount=1;//删除或添加节点数计数(序号从1开始)
					Terminal_Router->ReadNoteNum=0;//档案同步时从模块已读取的节点数(半字对齐)
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://发送
					x=Terminal_Router->TempCount;
					MWR(x,((u32)p8tx)+128,2);//从节点起始序号
					p8tx[128+2]=10;//本次读10个
					Uart_3762_TxData(PORTn,0x10,2,3);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 2://接收
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
							y=0;
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1002)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=10H,Fn=2
								x=Terminal_Router->NoteNum;//已知总从节点数
								y=MRR(((u32)p8rx)+13+2,1);//本次读入从节点数
								Terminal_Router->TempCount+=y;//下开始序号
								Terminal_Router->ReadNoteNum+=y;//档案同步时从模块已读取的节点数(半字对齐)
								for(z=0;z<y;z++)
								{
									if((x+1)<=NUM_RMmax)
									{
										d64a=MRR(((u32)p8rx)+13+3+(z*8),6);
										for(i=0;i<x;i++)
										{
											d64b=MRR((u32)&NoteList->List[i].Addr,6);
											if(d64a==d64b)
											{
												break;//原已存在
											}
										}
										if(i==x)
										{//原没存在
											if(Terminal_Router->RouterRunMode_1!=3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
											{//电力线载波表
												MC(0,(u32)&NoteList->List[x],LEN_NoteList);
												MWR(d64a,(u32)&NoteList->List[x].Addr,6);
												x++;
												Terminal_Router->NoteNum=x;//当前路由器中总从节点数
											}
											else
											{//微功率无线,档案地址用采集器地址
												for(i=0;i<x;i++)
												{
													d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
													if(d64a==d64b)
													{
														break;//原已存在
													}
												}
												if(i==x)
												{//原没存在
													MC(0,(u32)&NoteList->List[x],LEN_NoteList);
													MWR(d64a,(u32)&NoteList->List[x].Addr,6);
													MWR(d64a,(u32)&NoteList->List[x].CollectorAddr,6);//采集器地址
													x++;
													Terminal_Router->NoteNum=x;//当前路由器中总从节点数
												}
											}
										}
									}
								}
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								return;
							}
							if((Terminal_Router->TempCount<=NUM_RMmax)&&(Terminal_Router->ReadNoteNum<Terminal_Router->ModeNoteNum))
							{//继续读
								if(y==0)
								{
									Terminal_Router->TempCount+=10;//下开始序号从下10个开始
								}
								Terminal_Router->RouterInitSubTask=1;
							}
							else
							{//读结束
								Terminal_Router->RouterInitReCount=0;//命令重发计数
								Terminal_Router->RouterInitSubTask=0;//子任务
								Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
							}
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->RouterInitReCount<2)//命令重发计数
							{
								Terminal_Router->RouterInitReCount++;//命令重发计数
								Terminal_Router->RouterInitSubTask=1;//子任务=发送
							}
							else
							{
								Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 4://删除从节点
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://清删除或添加节点数计数
					Terminal_Router->TempCount=0;//删除或添加节点数计数
					p8tx[128]=0;//有要删除的节点数
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://查删除
					for(i=0;i<100;i++)
					{
						if(Terminal_Router->TempCount>=Terminal_Router->NoteNum)
						{
							if(p8tx[128]==0)//有要删除的节点数
							{//没要删除
								Terminal_Router->RouterInitSubTask=0;//子任务
								Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
								return;
							}
							else
							{//有删除
								//即发送,免其它任务影响p8tx+128后的数据
								Terminal_Router->RouterInitReCount=0;//命令重发计数
								Terminal_Router->RouterInitSubTask=2;//子任务
								return;
							}
						}
						d64a=MRR((u32)&NoteList->List[Terminal_Router->TempCount].Addr,6);
						p8s=(u8*)ADDR_6000_SDRAM;
						for(x=0;x<NUM_RMmax;x++)
						{
							if(p8s[0]==DataType_structure)
							{//有效
								p8d=p8s+5;
								p8d=Get_Element(p8d,4,0,0);//计算元素地址
								if(p8d)
								{
									z=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
									z=OADtoPORTn(z);
									if(z==PORTn)
									{//端口号相同
										if(Terminal_Router->RouterRunMode_1!=3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
										{//电力线载波
											d64b=MRR((u32)p8s+9,6);//取配置通信地址
											d64b=R_Inverse(d64b,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
											if(d64a==d64b)
											{//地址相同
												break;
											}
										}
										else
										{//微功率无线,档案地址用采集器地址
											d64b=MRR((u32)p8s+9,6);
											d64b=R_Inverse(d64b,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
											if(d64a==d64b)
											{//与通信地址相同
												p8d=Get_Element(p8s,3,0,0);//计算元素地址
												if(p8d==0)
												{//无采集器可取
													break;
												}
												d64b=MRR((u32)p8d+4,6);//取配置采集器地址
												d64b=R_Inverse(d64b,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
												d64c=MRR((u32)&NoteList->List[Terminal_Router->TempCount].CollectorAddr,6);
												if(((d64b==0xeeeeeeeeeeee)||(d64b==0))||(d64b==d64c))
												{//无采集器或采集器地址也相同
													break;
												}
											}
											else
											{
												p8d=Get_Element(p8s,3,0,0);//计算元素地址
												if(p8d)
												{
													d64c=MRR((u32)p8d+4,6);//取配置采集器地址
													d64c=R_Inverse(d64c,6);
													if(d64a==d64c)
													{//采集器地址相同
														for(y=0;y<Terminal_Router->NoteNum;y++)
														{
															d64c=MRR((u32)&NoteList->List[y].Addr,6);//列表中通信地址
															if(d64b==d64c)
															{
																break;
															}
														}
														if(y>=Terminal_Router->NoteNum)
														{//相同采集器地址的电表通信地址还无列表
															MWR(d64b,(u32)&NoteList->List[Terminal_Router->TempCount].Addr,6);//通信地址
															break;
														}
													}
												}
											}
										}
									}
								}
							}
							p8s+=LENper_6000;
						}
						if(x==NUM_RMmax)
						{//删除
							if(Terminal_Router->RouterRunMode_1!=3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
							{//电力线载波
								x=p8tx[128];//有要删除的节点数
								MR(((u32)p8tx)+128+1+(x*6),(u32)&NoteList->List[Terminal_Router->TempCount].Addr,6);
								x++;//有要删除的节点数+1
								p8tx[128]=x;//有要删除的节点数
								if(x>=10)
								{
									//即发送,免其它任务影响p8tx+128后的数据
									Terminal_Router->RouterInitReCount=0;//命令重发计数
									Terminal_Router->RouterInitSubTask=2;//子任务
									return;
								}
							}
							else
							{
								d64b=MRR((u32)&NoteList->List[Terminal_Router->TempCount].CollectorAddr,6);//取要删除的采集器地址
								if((d64b==0xeeeeeeeeeeee)||(d64b==0))
								{//无采集器地址
									x=p8tx[128];//有要删除的节点数
									MR(((u32)p8tx)+128+1+(x*6),(u32)&NoteList->List[Terminal_Router->TempCount].Addr,6);
									x++;//有要删除的节点数+1
									p8tx[128]=x;//有要删除的节点数
									if(x>=10)
									{
										//即发送,免其它任务影响p8tx+128后的数据
										Terminal_Router->RouterInitReCount=0;//命令重发计数
										Terminal_Router->RouterInitSubTask=2;//子任务
										return;
									}
								}
								else
								{//采集器地址有效
									z=0;//清相同采集器地址计数
									for(y=0;y<Terminal_Router->NoteNum;y++)
									{
										d64c=MRR((u32)&NoteList->List[y].CollectorAddr,6);//列表中通信地址
										if(d64b==d64c)
										{
											z++;
											if(z>=2)
											{
												break;
											}
										}
									}
									if(z>=2)
									{//仅删除列表,不发送到模块
										if(Terminal_Router->NoteNum!=0)
										{
											z=Terminal_Router->TempCount;
											y=(Terminal_Router->NoteNum-(z+1))*LEN_NoteList;//从地址表中删去
											MR(ADDR_NoteList+(z*LEN_NoteList),ADDR_NoteList+((z+1)*LEN_NoteList),y);
											Terminal_Router->NoteNum--;//当前路由器中总从节点数
											Terminal_Router->TempCount=0;//从0开始重新搜索
											return;
										}
									}
									else
									{
										x=p8tx[128];//有要删除的节点数
										MR(((u32)p8tx)+128+1+(x*6),(u32)&NoteList->List[Terminal_Router->TempCount].CollectorAddr,6);
										x++;//有要删除的节点数+1
										p8tx[128]=x;//有要删除的节点数
										if(x>=10)
										{
											//即发送,免其它任务影响p8tx+128后的数据
											Terminal_Router->RouterInitReCount=0;//命令重发计数
											Terminal_Router->RouterInitSubTask=2;//子任务
											return;
										}
									}
								}
							}
						}
						Terminal_Router->TempCount++;
					}
					break;
				case 2://删除发送
				#if (USER/100)==0//电科院测试
					if(Terminal_Router->RouterRunMode_2==0)//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
					{
						UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
						goto DELACK;
					}
				#else
					if(Terminal_Router->RouterRunMode_3==0)//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
					{//0表示不需要下发测量点信息
						UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
						goto DELACK;
					}
				#endif
					x=p8tx[128];//有要删除的节点数
					Uart_3762_TxData(PORTn,0x11,2,1+(x*6));//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 3://删除接收
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//回答确认,删除
						DELACK:
								x=p8tx[128];//有要删除的节点数
								if(x>=10)
								{
									x=10;
								}
								for(i=0;i<x;i++)
								{
									d64a=MRR(((u32)p8tx)+128+1+(i*6),6);
									y=Terminal_Router->NoteNum;//当前路由器中总从节点数
									for(z=0;z<y;z++)
									{
										d64b=MRR((u32)&NoteList->List[z].Addr,6);
										if(d64a==d64b)
										{//找到要删去的地址
											if(Terminal_Router->NoteNum!=0)
											{
												y=(Terminal_Router->NoteNum-(z+1))*LEN_NoteList;//从地址表中删去
												MR(ADDR_NoteList+(z*LEN_NoteList),ADDR_NoteList+((z+1)*LEN_NoteList),y);
												Terminal_Router->NoteNum--;//当前路由器中总从节点数
												Terminal_Router->ModeNoteNum--;//从模块读取的模块返回从节点数(半字对齐)
											}
											break;
										}
										if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
										{//小无线
											d64b=MRR((u32)&NoteList->List[z].CollectorAddr,6);
											if(d64a==d64b)
											{//找到要删去的地址
												if(Terminal_Router->NoteNum!=0)
												{
													y=(Terminal_Router->NoteNum-(z+1))*LEN_NoteList;//从地址表中删去
													MR(ADDR_NoteList+(z*LEN_NoteList),ADDR_NoteList+((z+1)*LEN_NoteList),y);
													Terminal_Router->NoteNum--;//当前路由器中总从节点数
													Terminal_Router->ModeNoteNum--;//从模块读取的模块返回从节点数(半字对齐)
												}
												break;
											}
										}
									}
								}
								p8tx[128]=0;//要删除数
								Terminal_Router->TempCount=0;//从0开始重新搜索
								Terminal_Router->RouterInitSubTask=1;//子任务
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							}
							return;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->RouterInitReCount<2)
							{
								Terminal_Router->RouterInitReCount++;
								Terminal_Router->RouterInitSubTask=2;//子任务
							}
							else
							{
								Terminal_Router->NoteNum=0;//当前路由器中已知总从节点数
								Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;	
		case 5://添加从节点
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://清删除或添加节点数计数
					Terminal_Router->NoteNumOver=0;//AFN04F10配置的从节点数>模块支持的最大从节点数;0=否,1=是
					Terminal_Router->TempCount=0;//删除或添加节点数计数
					p8tx[128]=0;//要添加数
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://查添加
					for(i=0;i<100;i++)
					{
						if(Terminal_Router->TempCount>=NUM_RMmax)
						{
							if(p8tx[128]==0)//有要删除的节点数
							{//没要添加
								Terminal_Router->RouterInitSubTask=0;//子任务
								Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
								return;
							}
							else
							{//有添加
								//即发送,免其它任务影响p8tx+128后的数据
								Terminal_Router->RouterInitReCount=0;//命令重发计数
								Terminal_Router->RouterInitSubTask=2;//子任务
								return;
							}
						}
						p8s=(u8*)ADDR_6000_SDRAM+(LENper_6000*Terminal_Router->TempCount);
						if(p8s[0]==DataType_structure)
						{//有效
							p8d=p8s+5;
							p8d=Get_Element(p8d,4,0,0);//计算元素地址
							if(p8d)
							{
								z=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
								z=OADtoPORTn(z);
								if(z==PORTn)
								{//端口号相同
									d64a=MRR((u32)p8s+9,6);//取配置通信地址
									d64a=R_Inverse(d64a,6);
									d64b=MRR((u32)&Terminal_Router->RouterAddr,6);//主节点地址
									if(d64a!=d64b)
									{//电表地址!=主节点地址;有效
										if(d64a!=0xeeeeeeeeeeee)
										{//电表地址!=0xee;有效
											for(y=0;y<Terminal_Router->NoteNum;y++)
											{
												d64c=MRR((u32)&NoteList->List[y].Addr,6);
												if(d64a==d64c)
												{
													break;
												}
											}
											if(y==Terminal_Router->NoteNum)
											{//添加
												if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
												{//微功率无线,档案地址用采集器地址
													p8d=Get_Element(p8s,3,0,0);//计算元素地址
													if(p8d)
													{
														d64b=MRR((u32)p8d+4,6);//取配置采集器地址
														d64b=R_Inverse(d64b,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
														if((d64b!=0xeeeeeeeeeeee)&&(d64b!=0))
														{//采集器地址有效
															for(y=0;y<Terminal_Router->NoteNum;y++)
															{
																d64c=MRR((u32)&NoteList->List[y].CollectorAddr,6);
																if(d64c==d64b)
																{
																	break;
																}
															}
															if(y!=Terminal_Router->NoteNum)
															{//采集器地址原已存在;仅增加列表,不写入模块
															AddNoTx:
																if(Terminal_Router->NoteNum<NUM_RMmax)
																{
																	MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
																	MWR(d64a,(u32)&NoteList->List[Terminal_Router->NoteNum].Addr,6);
																	MWR(d64b,(u32)&NoteList->List[Terminal_Router->NoteNum].CollectorAddr,6);
																	Terminal_Router->NoteNum++;
																}
																Terminal_Router->TempCount++;
																continue;
															}
															d64a=d64b;//用采集器地址发给模块
														}
													}
												}
												x=p8tx[128];//有要添加的节点数
												MWR(d64a,((u32)p8tx)+128+1+(x*7),6);
												
//	规约类型  enum
//	{
//  	未知 (0)，
//		DL/T645-1997（1），
//		DL/T645-2007（2），
//		DL/T698.45（3），
//		CJ/T 188-2004（4）
//	}
												p8d=p8s+5;
												p8d=Get_Element(p8d,3,0,0);//计算元素地址
												if(p8d==0)
												{
													y=0;
												}
												else
												{
													y=p8d[1];
												}
												switch(y)
												{
													case 1://DL/T645-1997（1）
														p8tx[128+1+(x*7)+6]=1;//通信协议类型
														break;
													case 2://DL/T645-2007（2）
														p8tx[128+1+(x*7)+6]=2;//通信协议类型
														break;
													case 3://DL/T698.45（3）
														if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能,8=钱龙
														{//鼎信无698暂用透明
															p8tx[128+1+(x*7)+6]=0;//通信协议类型
														}
														else
														{
															p8tx[128+1+(x*7)+6]=3;//通信协议类型
														}
														break;
													case 4://CJ/T 188-2004（4）
													default://
														p8tx[128+1+(x*7)+6]=2;//通信协议类型
														break;
												}
												y=10;
												if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
												{//微功率无线,档案地址用采集器地址
													y=1;
													if(Terminal_Router->ModeNoteNum>=Terminal_Router->MaxNoteNum)
													{//模块中已有从节点数>=模块支持的最大从节点数
														//仅增加列表,不写入模块
														Terminal_Router->NoteNumOver=1;//AFN04F10配置的从节点数>模块支持的最大从节点数;0=否,1=是
														goto AddNoTx;
													}
												}
												x++;//有要添加的节点数+1
												p8tx[128]=x;//有要添加的节点数
												if(x>=y)
												{
													//即发送,免其它任务影响p8tx+128后的数据
													Terminal_Router->RouterInitReCount=0;//命令重发计数
													Terminal_Router->RouterInitSubTask=2;//子任务
													return;
												}
											}
										}
									}
								}
							}
						}
						Terminal_Router->TempCount++;
					}
					break;
				case 2://添加发送
				#if (USER/100)==0//电科院测试
					if(Terminal_Router->RouterRunMode_2==0)//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
					{
						UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
						goto ADDACK;
					}
				#else
					if(Terminal_Router->RouterRunMode_3==0)//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
					{//0表示不需要下发测量点信息
						UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
						goto ADDACK;
					}
				#endif
					x=p8tx[128];//有要添加的节点数
					Uart_3762_TxData(PORTn,0x11,1,1+(7*x));//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 3://添加接收
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==((0<<8)+1))//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//回答确认,添加
						ADDACK:
								x=p8tx[128];//有要添加的节点数
								if(x>=10)
								{
									x=10;
								}
								for(i=0;i<x;i++)
								{
									d64a=MRR(((u32)p8tx)+128+1+(i*7),6);
									for(z=0;z<NUM_RMmax;z++)
									{
										p8s=(u8*)ADDR_6000_SDRAM+(z*LENper_6000);
										if(p8s[0]==DataType_structure)
										{//测量点有效
											p8d=p8s+5;
											p8d=Get_Element(p8d,4,0,0);//计算元素地址
											if(p8d)
											{
												y=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
												y=OADtoPORTn(y);
												if(y==PORTn)
												{//端口号相同
													d64b=MRR((u32)p8s+9,6);//取配置通信地址
													d64b=R_Inverse(d64b,6);
													if(d64a==d64b)
													{//找到要添加的04参数配置地址
														if(Terminal_Router->NoteNum<NUM_RMmax)
														{
															MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
															MWR(d64a,(u32)&NoteList->List[Terminal_Router->NoteNum].Addr,6);
															if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
															{//微功率无线,档案地址用采集器地址
																p8d=Get_Element(p8s,3,0,0);//计算元素地址
																if(p8d)
																{
																	d64b=MRR((u32)p8d+4,6);//取配置采集器地址
																	d64b=R_Inverse(d64b,6);
																	MWR(d64b,(u32)&NoteList->List[Terminal_Router->NoteNum].CollectorAddr,6);
																}
															}
															Terminal_Router->NoteNum++;
															Terminal_Router->ModeNoteNum++;//从模块读取的模块返回从节点数(半字对齐)
														}
														break;
													}
													else
													{
														if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
														{//微功率无线,档案地址用采集器地址
															p8d=Get_Element(p8s,3,0,0);//计算元素地址
															if(p8d)
															{
																d64b=MRR((u32)p8d+4,6);//取配置采集器地址
																d64b=R_Inverse(d64b,6);
																if(d64a==d64b)
																{//找到要添加的04参数配置地址
																	if(Terminal_Router->NoteNum<NUM_RMmax)
																	{
																		MC(0,ADDR_NoteList+(Terminal_Router->NoteNum*LEN_NoteList),LEN_NoteList);
																		d64a=MRR((u32)p8s+9,6);//取配置通信地址
																		d64a=R_Inverse(d64a,6);
																		MWR(d64a,(u32)&NoteList->List[Terminal_Router->NoteNum].Addr,6);
																		MWR(d64b,(u32)&NoteList->List[Terminal_Router->NoteNum].CollectorAddr,6);
																		Terminal_Router->NoteNum++;
																		Terminal_Router->ModeNoteNum++;//从模块读取的模块返回从节点数(半字对齐)
																	}
																	break;
																}
															}
														}
													}
												}
											}
										}
									}
								}
								p8tx[128]=0;//要添加数
								Terminal_Router->RouterInitSubTask=1;//子任务
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							}
							return;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->RouterInitReCount<2)
							{
								Terminal_Router->RouterInitReCount++;
								Terminal_Router->RouterInitSubTask=2;//子任务
							}
							else
							{
								Terminal_Router->NoteNum=0;//当前路由器中已知总从节点数
								Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 6://查询从节点数量
		#if (USER/100)==0//电科院测试
			if(Terminal_Router->RouterRunMode_2==0)//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
			{
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
				break;
			}
		#else
			if(Terminal_Router->RouterRunMode_3==0)//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
			{//0表示不需要下发测量点信息
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
				break;
			}
		#endif
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://发送
					Uart_3762_TxData(PORTn,0x10,1,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://接收
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=10H,Fn=1
								x=p8rx[13]+(p8rx[14]<<8);
								if(Terminal_Router->RouterRunMode_1==3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
								{//微功率无线
									y=Terminal_Router->ModeNoteNum;
									if(y!=x)
									{//模块节点数!=读取的节点数
										Terminal_Router->NoteNum=0;//同步错误
										Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
										return;
									}
								}
								else
								{//电力线载波
									if(Terminal_Router->NoteNum!=x)
									{//总已知节点数!=模块读取的节点数
										Terminal_Router->NoteNum=0;//同步错误
										Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
										return;
									}
								}
								Terminal_Router->ModeNoteNum=x;//从模块读取的模块返回从节点数(半字对齐)
								Terminal_Router->RouterInitReCount=0;//命令重发计数
								Terminal_Router->RouterInitSubTask=0;//子任务
								Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							}
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->RouterInitReCount<2)//命令重发计数
							{
								Terminal_Router->RouterInitReCount++;//命令重发计数
								Terminal_Router->RouterInitSubTask=0;//子任务=发送
							}
							else
							{
								Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 7://读取从节点信息
		#if (USER/100)==0//电科院测试
			if(Terminal_Router->RouterRunMode_2==0)//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
			{
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
				break;
			}
		#else
			if(Terminal_Router->RouterRunMode_3==0)//路由运行模式_测量点信息模式：0表示不需要下发测量点信息；1表示需要下发测量点信息
			{//0表示不需要下发测量点信息
				Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
				break;
			}
		#endif
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0://清当前路由器中从节点数
					if(Terminal_Router->ModeNoteNum==0)//从模块读取的模块返回从节点数(半字对齐)
					{
						Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
						return;
					}
					Terminal_Router->TempCount=1;//删除或添加节点数计数(序号从1开始)
					Terminal_Router->ReadNoteNum=0;//档案同步时从模块已读取的节点数(半字对齐)
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 1://发送
					x=Terminal_Router->TempCount;
					MWR(x,((u32)p8tx)+128,2);//从节点起始序号
					p8tx[128+2]=10;//本次读10个
					Uart_3762_TxData(PORTn,0x10,2,3);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterInitSubTask++;//子任务
					break;
				case 2://接收
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//0=空闲,其他转发等插入操作
							y=0;
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1002)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{//AFN=10H,Fn=2
								x=Terminal_Router->NoteNum;//已知总从节点数
								y=MRR(((u32)p8rx)+13+2,1);//本次读入从节点数
								Terminal_Router->TempCount+=y;
								Terminal_Router->ReadNoteNum+=y;//档案同步时从模块已读取的节点数(半字对齐)
								for(z=0;z<y;z++)
								{
									if((x+1)<=NUM_RMmax)
									{
										d64a=MRR(((u32)p8rx)+13+3+(z*8),6);
										for(i=0;i<x;i++)
										{
											d64b=MRR((u32)&NoteList->List[i].Addr,6);
											if(d64a==d64b)
											{
												break;//原已存在
											}
										}
										if(i==x)
										{//原没存在
											if(Terminal_Router->RouterRunMode_1!=3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
											{
												//添加册除错误
												Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
												return;
											}
											else
											{//微功率无线,档案地址用采集器地址
												for(i=0;i<x;i++)
												{
													d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
													if(d64a==d64b)
													{
														break;//原已存在
													}
												}
												if(i==x)
												{//原没存在
													//添加册除错误
													Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
													return;
												}
											}
										}
									}
								}
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								return;
							}
							if((Terminal_Router->TempCount<=NUM_RMmax)&&(Terminal_Router->ReadNoteNum<Terminal_Router->ModeNoteNum))
							{//继续读
								if(y==0)
								{
									Terminal_Router->TempCount+=10;//下开始序号从下10个开始
								}
								Terminal_Router->RouterInitSubTask=1;
							}
							else
							{//读结束
								Terminal_Router->RouterInitReCount=0;//命令重发计数
								Terminal_Router->RouterInitSubTask=0;//子任务
								Terminal_Router->RouterSynTask++;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
							}
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->RouterInitReCount<2)//命令重发计数
							{
								Terminal_Router->RouterInitReCount++;//命令重发计数
								Terminal_Router->RouterInitSubTask=1;//子任务=发送
							}
							else
							{
								Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 8://恢复路由
			if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			{
				Terminal_Router->StopFlags&=~(1<<8);//恢复路由
				Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
			}
			Terminal_Router->RouterSynTask=0;//档案同步任务;0=空闲,1=等待启动,2-n同步过程完成后回0
			break;
	}
}




void Terminal_RouterCtrl(u32 PORTn)//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
{
	u32 i;
	u8 *p8rx;
	u16 *p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Terminal_Router->RouterCtrl==0)//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if((UARTCtrl->Task==3)||(UARTCtrl->Task==4))//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
	{
		return;//正在发送(主动上报时可能有这种情况,但不很确定,加上再说)
	}
	//if(Terminal_Router->RouterRunMode_2==0)//路由运行模式_路由管理方式：0表示本地通信模块无路由管理功能，仅实现通信的调制解调功能，1表示本地通信模块带有路由管理功能
	if(Terminal_Router->RouterRunMode_4==1)//路由运行模式_周期抄表模式：01表示通信模块仅支持集中器主导的周期抄表模式，10表示通信模块仅支持路由主导的周期抄表模式；11表示通信模块对两种抄表模式都支持；00保留
	{
		if((Terminal_Router->RouterCtrl!=4)&&(Terminal_Router->RouterCtrl!=6))//路由器控制命令0=空,1=重启,2=暂停,3=恢复延时,4=读状态,5=恢复,6=启动组网;完成后自动回到0
		{
			if(Terminal_Router->RouterCtrl==1)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
			{
				Terminal_Router->StopFlags=0;//暂停路由标志
			}
			UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			Terminal_Ram->RouterCheck_S_Timer=(10*60);//载波路由器死机秒定时值
			Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
			Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
			return;
		}
	}

	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	switch(Terminal_Router->RouterCtrl)//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
	{//路由控制
		case 0://空
			break;
		case 1://重启
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//载波路由器死机秒定时值
			switch(Terminal_Router->RouterCtrlTask)//路由器控制任务计数;完成后自动回到0
			{
				case 0:
					//重启在过整点冻结后进行,仅电科院测试需要,过整点重启后路由请求数据时要求6秒内响应,但当前整点冻结时间约需3秒
					i=MRR(ADDR_MYMDHMS,2);
					if(i<0x0003)
					{//过整点后时钟分秒<00分03秒或正在冻结
						break;
					}
					Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
					Uart_3762_TxData(PORTn,0x12,1,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterCtrlTask++;//路由器控制任务计数;完成后自动回到0
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rx[4]&0x4)==0))//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
								Terminal_Router->StopFlags=0;//暂停路由标志
								Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
								//下条仅为电科院测试去了读状态而增加的
								Terminal_Router->RUNState|=2;//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
								break;
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							}
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
							{
								Terminal_Router->ReRouterCtrl++;
								Uart_3762_TxData(PORTn,0x12,1,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
							}
							else
							{
								Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
								Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
								//东软路由配置数=0时重启和恢复错误
								if(Terminal_Router->NoteNum!=0)
								{//节点表!=0
									Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
								}
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 2://暂停
			switch(Terminal_Router->RouterCtrlTask)//路由器控制任务计数;完成后自动回到0
			{
				case 0:
					if(Terminal_Router->RouterInitTask>=200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					{
						if((Terminal_Router->RUNState&2)==0)//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
						{//本就是暂停
							Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
							Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
							break;
						}
					}
					Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
					Uart_3762_TxData(PORTn,0x12,2,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterCtrlTask++;//路由器控制任务计数;完成后自动回到0
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rx[4]&0x4)==0))//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
								Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
								//下条仅为电科院测试去了读状态而增加的
								Terminal_Router->RUNState&=0xfd;//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
								break;
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							}
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
							{
								Terminal_Router->ReRouterCtrl++;
								Uart_3762_TxData(PORTn,0x12,2,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
							}
							else
							{
								Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
								Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 3://延时恢复
			if(Terminal_Router->StopFlags!=0)//暂停路由标志
			{//还有其它需暂停
				Terminal_Ram->RouterCtrl_MS_Timer=0;//5000/100;//16 路由控制恢复延时 10MS定时器
			}
			else
			{
			#if (USER/100)==0
				Terminal_Ram->RouterCtrl_MS_Timer=0;//5000/100;//16 路由控制恢复延时 10MS定时器
			#else
				if(Comm_Ram->RouterType==2)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
				{//鼎信100s
					Terminal_Ram->RouterCtrl_MS_Timer=100000/10;//16 路由控制恢复延时 10MS定时器
				}
				else
				{//其他30s
					Terminal_Ram->RouterCtrl_MS_Timer=30000/10;//16 路由控制恢复延时 10MS定时器
				}
			#endif
			}
			UARTCtrl->Task=0;//置为0=空闭,以便马上再行转发等
			Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
			Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
			//下条置当前路由运行状态=0仅为麦希路由时需要,因暂停路由后读状态仍为运行需恢复动作再次读取状态以判断学习状态
			Terminal_Router->RUNState=0;//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
			break;
		case 5://立即恢复
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//载波路由器死机秒定时值
			switch(Terminal_Router->RouterCtrlTask)//路由器控制任务计数;完成后自动回到0
			{
				case 0:
					if(Terminal_Router->StopFlags!=0)//暂停路由标志
					{//还有其它需暂停
						Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
						break;
					}
					Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
					Uart_3762_TxData(PORTn,0x12,3,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterCtrlTask++;//路由器控制任务计数;完成后自动回到0
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rx[4]&0x4)==0))//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
								Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
								//下条仅为电科院测试去了读状态而增加的
								Terminal_Router->RUNState|=2;//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
								break;
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							}
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
							{
								Terminal_Router->ReRouterCtrl++;
								Uart_3762_TxData(PORTn,0x12,3,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
							}
							else
							{
								Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
								Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
								//东软路由配置数=0时重启和恢复错误
								if(Terminal_Router->NoteNum!=0)
								{//节点表!=0
									Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
								}
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		case 4://读状态
			if(Terminal_Router->RouterRunMode_1!=3)//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
			{//电力线
				Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
				Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
			}
			else
			{//小无线
				switch(Terminal_Router->RouterCtrlTask)//路由器控制任务计数;完成后自动回到0
				{
					case 0:
						Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
						Uart_3762_TxData(PORTn,0x10,4,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
						Terminal_Router->RouterCtrlTask++;//路由器控制任务计数;完成后自动回到0
						break;
					case 1:
						switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
						{
							case 1://1=完成1帧发收
								UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1004)&&((p8rx[4]&0x4)==0))//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
								{
									//取当前运行状态
									i=p8rx[13];
									Terminal_Router->RUNState=i;//当前路由运行状态(从模块中读取值):b2=上报事件标志,b1=工作标志,b0=路由完成标志
									//
									Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
									Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
									break;
								}
								else
								{
									p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
									p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
									UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
									UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
								}
								break;
							case 0://0=空闲
							case 2://2=接收超时(或错误)
								i=2;
								if(Comm_Ram->RouterType==102)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=;100=保留,101=麦希,102=友讯达,103=有方
								{//友讯达在组网时会不响应
									i=20;
								}
								if(Terminal_Router->ReRouterCtrl<i)//路由器控制重发计数
								{
									Terminal_Router->ReRouterCtrl++;
									Uart_3762_TxData(PORTn,0x10,4,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
								}
								else
								{
									Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
									Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
								}
								break;
						}
						break;
					default:
						Terminal_Router->RouterCtrl=0;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
						Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
						break;
				}
			}
			break;
		case 6://启动组网
			switch(Terminal_Router->RouterCtrlTask)//路由器控制任务计数;完成后自动回到0
			{
				case 0:
					Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
					Uart_3762_TxData(PORTn,0x11,102,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
					Terminal_Router->RouterCtrlTask++;//路由器控制任务计数;完成后自动回到0
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
					{
						case 1://1=完成1帧发收
							UARTCtrl->Task=0;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							if((Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)&&((p8rx[4]&0x4)==0))//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
								Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
								break;
							}
							else
							{
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
								UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
								UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							}
							break;
						case 0://0=空闲
						case 2://2=接收超时(或错误)
							if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
							{
								Terminal_Router->ReRouterCtrl++;
								Uart_3762_TxData(PORTn,0x11,102,0);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
							}
							else
							{
								Terminal_Router->RouterCtrlTask=0;//路由器控制任务计数;完成后自动回到0
								Terminal_Router->RouterCtrl=4;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
							}
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
					break;
			}
			break;
		default:
			Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			break;
	}
}

void RouterBroadcastTime(u32 PORTn)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
{
	u32 i;
	u32 x;
	u64 d64;
	u8 *p8rx;
	u8 *p8tx;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Terminal_Router->RouterInitTask<200)//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
	{
		return;
	}
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(UARTCtrl->BroadcastTimeTask)//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
	{
		default:
			break;
		case 3://3=命令读
			if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				Terminal_Router->StopFlags|=(1<<4);//暂停路由
				Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复;完成后自动回到0
				UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				//WarningOccur(PORTn,(u8 *)"载波准备广播校时");//告警发生,入口告警代码和告警字符串
			}
			break;
		case 4://等待1分
			p16timer[0]=60000/10;
			Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
			UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			break;
		case 5://通信延时相关广播通信时长查询
			if(p16timer[0]==0)
			{
				p8tx+=128;
				p8tx[0]=2;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
				p8tx[1]=22;//报文长度
				MC(0xfe,((u32)p8tx)+2,4);
				p8tx[6]=0x68;
				MC(0x99,((u32)p8tx)+7,6);
				p8tx[13]=0x68;
				p8tx[14]=0x08;
				p8tx[15]=0x06;
				MR(((u32)p8tx)+16,ADDR_MYMDHMS,6);
				for(i=0;i<6;i++)
				{
					p8tx[16+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8tx[6+i];
				}
				p8tx[22]=x;
				p8tx[23]=0x16;
				Uart_3762_TxData(PORTn,0x03,9,24);//376.2发送数据填入发送区;入口时若有信息域目的地址数据单元等则已填入且数据单元暂固定填入发送缓冲区+128地址
				UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0	
				//WarningOccur(PORTn,(u8 *)"载波正在广播校时");//告警发生,入口告警代码和告警字符串
			}
			break;
		case 6://发广播校时
			if(UARTCtrl->Task>=3)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			i=MRR(((u32)p8rx)+10,3);
			if((i!=0x010103)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
				{
					Terminal_Router->ReRouterCtrl++;
					UARTCtrl->BroadcastTimeTask--;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				else
				{
					UARTCtrl->BroadcastTimeTask=10;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
				}
				break;
			}
			i=MRR(((u32)p8rx)+13,2);//广播通信延迟时间秒
			p8tx+=128;
			p8tx[0]=2;//控制字,0=透明传输，1=645-1997，2=645-2007，3=相位识别
			p8tx[1]=22;//报文长度
			MC(0xfe,((u32)p8tx)+2,4);
			p8tx[6]=0x68;
			MC(0x99,((u32)p8tx)+7,6);
			p8tx[13]=0x68;
			p8tx[14]=0x08;
			p8tx[15]=0x06;
			d64=MRR(ADDR_MYMDHMS,6);
			d64=YMDHMS_AddS(d64,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
			MWR(d64,((u32)p8tx)+16,6);
			for(i=0;i<6;i++)
			{
				p8tx[16+i]+=0x33;
			}
			x=0;
			for(i=0;i<16;i++)
			{
				x+=p8tx[6+i];
			}
			p8tx[22]=x;
			p8tx[23]=0x16;
			Uart_3762_TxData(PORTn,0x05,3,24);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			Terminal_Router->ReRouterCtrl=0;//路由器控制重发计数
			if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
			{
				Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//最大广播超时时间秒(半字对齐)
			}
			UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0	
			break;
		case 7://接收确认帧
			if(Terminal_Router->RouterRunMode_7==0)//路由运行模式_广播命令确认方式：0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文，1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
			{//0表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文
				switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				{
					case 1://1=完成1帧发收
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
							UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
						}
						break;
					case 0://0=空闲
					case 2://2=接收超时(或错误)
						if(Terminal_Ram->RouterCheck_S_Timer==0)//最大广播超时时间秒(半字对齐)
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
							UARTCtrl->Task=5;//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
							p16timer[0]=1000/10;//发送后接收超时定时值ms/10(半字对齐)
						}
						break;
				}
			}
			else
			{//1表示广播命令在本地信道执行广播通信之前就返回确认报文，需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
				switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				{
					case 1://1=完成1帧发收
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							//取等待时间
							i=p8rx[17];
							i+=p8rx[18]<<8;
							i*=1000/10;
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16timer[0]=i;
							UARTCtrl->BroadcastTimeTask++;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							break;
						}
						else
						{
							if(Terminal_Router->ReRouterCtrl<2)//路由器控制重发计数
							{
								Terminal_Router->ReRouterCtrl++;
								UARTCtrl->BroadcastTimeTask--;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							}
							else
							{
								UARTCtrl->BroadcastTimeTask=10;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
							}
						}
						break;
					case 0://0=空闲
					case 2://2=接收超时(或错误)
						UARTCtrl->BroadcastTimeTask=10;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
						break;
				}
			}
			break;
		case 8://等待广播延时
			if(p16timer[0]==0)
			{
				UARTCtrl->BroadcastTimeTask=10;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
			}
			break;
//		case 9:
//			break;
		case 10:
			Terminal_Router->StopFlags&=~(1<<4);//恢复路由
			Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态,5=真恢复;完成后自动回到0
			UARTCtrl->BroadcastTimeTask=0;//广播校时0=空,1=,2=,3=命令,4=过程...,完成后自动回到0
		#if ((USER/100)==5)//上海
			i=MRR(ADDR_AFN05F201,1);
			if(i&(1<<3))
			{
				i&=~(1<<3);
				MWR(i,ADDR_AFN05F201,1);
			}
		#endif
			break;
	}
}



#endif
#endif
