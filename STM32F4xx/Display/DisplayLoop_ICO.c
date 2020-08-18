
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Display/DisplayLoop_ICO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
//#include "../I2_terminal/Terminal_SwitchIn.h"
//#include "../I2_terminal/Terminal_Contrl_Out.h"
#include "../Display/Display.h"
#include "../Calculate/Calculate.h"
#include "../Device/IC_TESAM.h"




u32 DisplayLoop_TopICO(u32 Row)//顶图标和时钟显示
{
#if MainProtocol==376
	u32 i;
	u8 *p8;
  u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
//#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
//	u32 x;
//#endif

  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(0*2));
//无线信号和连接
	p16[0]=0;
	p16[1]=0;
	p16[2]=0;
	p16[3]=0;
	p16[4]=0;
	i=Terminal_Ram->Signal;
	if(i!=0)
	{
		p16[0]=2;//'天线'
		switch(i)
		{
			case 1:
				p16[1]=3;//'信号强度1'
				break;
			case 2:
				p16[1]=4;//'信号强度2'
				break;
			case 3:
				p16[1]=4;//'信号强度2'
				p16[2]=5;//'信号强度3'
				break;
			case 4:
				p16[1]=4;//'信号强度2'
				p16[2]=6;//'信号强度4'
				break;
		}
	}
//GPRS连接
	if((UART1Ctrl->LinkTask==101)||(UART1Ctrl->LinkTask==102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
	{
	#if (USER/100)==5//上海
		switch(Comm_Ram->CSPN)
		{
			case 0://0=空
				break;
			case 1://1=中国移动通信-----china mobile  CMCC
				p16[3]=7;//'G'
				p16[4]=8;
				break;
			default://2=中国联通通讯-----china unicom  CUCC
							//3=中国电信  ------CHINA TELECOM  CTCC
							//255=未知
				p16[3]=9;//'C'
				p16[4]=10;
				break;
		}
	#else
		if(Terminal_Ram->MODULSEL==1)//无线模块选择0=,1=华为CDMA MC323,2=GPRS标准3763,3=模块盒没插入
		{
			p16[3]=9;//'C'
			p16[4]=10;
		}
		else
		{
//			if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//远程无线模块类型位标志:b0=GSM网络支持GPRS,b1=WCDMA网络,b2=TD-SCDMA网络,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=表示扩展一个字节
//			{
//				p16[3]=7;//'G'
//				p16[4]=8;
//			}
//			else
//			{
//				p16[3]=9;//'C'
//				p16[4]=10;
//			}
			#if (USER/100)==0//电科院测试
				p16[3]=69;//'有框4G'
				p16[4]=70;
		//		p16[3]=7;//'有框G'
		//		p16[4]=8;
			#else
				i=Comm_Ram->RemoteWirelessModuleSYSINFO;
				switch(i)//远程无线模块当前注册的网络:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
				{
					case 4:
						p16[3]=69;//'有框4G'
						p16[4]=70;
						break;
					case 3:
						p16[3]=67;//'有框3G'
						p16[4]=68;
						break;
					default:
						p16[3]=65;//'有框2G'
						p16[4]=66;
						break;
				}
			#endif
		}
	#endif
	}
	
//以太网连接
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if((UARTCtrl->LinkTask==101)||(UARTCtrl->LinkTask==102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
	{
	#if (USER/100)==5//上海
		p16[3]=30;//30 'E'
		p16[4]=31;
	#else
		p16[3]=11;//11 'L'
		p16[4]=12;
	#endif
	}
	

	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(5*2));
#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=通信模块
//保电状态
	p8=(u8 *)(ADDR_Terminal_Protect);
	if(p8[0]!=0)
	{
		p16[0]=13;//保电
		p16[1]=14;
	}
#endif
	
//文件下载(与保电符号同一位置优先显示)
/*
	i=MRR(ADDR_AFN0FF1,1);//取下载文件标识
	if((i!=0)&&(i<16))
	{
		i=MRR(ADDR_AFN0FF1+2,1);//取文件指令
		if(i!=1)
		{//不是FTP 方式下装
			p16[0]=15;//'升级'
			p16[1]=16;
		}
	}
*/

//异常告警指示，表示终端或测量点有异常情况。当终端发生异常时，该标志将以1Hz的频率闪烁显示
//#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
//	p8=(u8 *)(ADDR_TYMDHMS);
//	if(p8[0]&0x1)
//	{
//		i=Terminal_Contrl_Round();//终端当前设定的控制轮次(功控合电控);返回B0-B7位标志
//		p8=(u8 *)(ADDR_ContrlModeData+2);//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
//		x=0;
//		if((p8[0]&0x3)==0x3)
//		{
//			x=1;
//		}
//		if(((p8[0]>>2)&0x3)==0x3)
//		{
//			x|=2;
//		}
//		i&=x;
//		if(i!=0x0)
//		{
//			p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(7*2));
//			p16[0]=17;// '告警'
//			p16[1]=18;
//		}
//	}
//#endif

//尖峰平谷
//#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
////终端尖峰平谷
//	p8=(u8 *)(ADDR_Terminal_Period);
//#else
	p8=(u8 *)(ADDR_METER07_CTRL_SRAM);
//#endif
#if (USER/100)==5//是上海
	switch(p8[0])
	{
		case 1:
			DisplayString(Row,10,0,(u8 *)"峰");//显示字符串
			break;
		case 2:
			DisplayString(Row,10,0,(u8 *)"平");//显示字符串
			break;
		case 3:
			DisplayString(Row,10,0,(u8 *)"尖");//显示字符串
			break;
		case 4:
			DisplayString(Row,10,0,(u8 *)"谷");//显示字符串
			break;
	}
#else//不是上海
	switch(p8[0])
	{
		case 1:
			DisplayString(Row,10,0,(u8 *)"尖");//显示字符串
			break;
		case 2:
			DisplayString(Row,10,0,(u8 *)"峰");//显示字符串
			break;
		case 3:
			DisplayString(Row,10,0,(u8 *)"平");//显示字符串
			break;
		case 4:
			DisplayString(Row,10,0,(u8 *)"谷");//显示字符串
			break;
	}
#endif	
	
//冻结(与尖峰平谷符号同一位置优先显示)
	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		p16=(u16 *)(ADDR_Class2Congeal_AutoRW+12);//类2数据冻结自动写EFLASH控制地址(字对齐)
		i=p16[0]|p16[1];//原类2数据冻结AUTOIOW没完成
		p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(10*2));
		if(((Terminal_Ram->CongealFlags&0x1f)!=0x1f)||(i!=0))//在实时钟分更新时清0;正在冻结标志(0正在冻结,1冻结完成);B0=小时冻结,B1=曲线冻结,B2=日冻结,B3=抄表日冻结,B4=月冻结
		{
			p16[0]=28;// '磁盘图'
			p16[1]=29;
		}
	}
	
//当前时钟
	p8=(u8 *)(ADDR_RTCBUFF);
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(12*2));
	p16[0]=((p8[2]>>4)&0xf)+0x30+0x2000;
	p16[1]=((p8[2]>>0)&0xf)+0x30+0x2000;
	p16[2]=':'+0x2000;
	p16[3]=((p8[1]>>4)&0xf)+0x30+0x2000;
	p16[4]=((p8[1]>>0)&0xf)+0x30+0x2000;
	p16[5]=':'+0x2000;
	p16[6]=((p8[0]>>4)&0xf)+0x30+0x2000;
	p16[7]=((p8[0]>>0)&0xf)+0x30+0x2000;
//
	Row++;
	return Row;
#endif
	
#if MainProtocol==698
	u32 i;
	u8 *p8;
  u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;

  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(0*2));
//无线信号和连接
	p16[0]=0;
	p16[1]=0;
	p16[2]=0;
	p16[3]=0;
	p16[4]=0;
	i=Terminal_Ram->Signal;
	if(i!=0)
	{
		p16[0]=2;//'天线'
		switch(i)
		{
			case 1:
				p16[1]=3;//'信号强度1'
				break;
			case 2:
				p16[1]=4;//'信号强度2'
				break;
			case 3:
				p16[1]=4;//'信号强度2'
				p16[2]=5;//'信号强度3'
				break;
			case 4:
				p16[1]=4;//'信号强度2'
				p16[2]=6;//'信号强度4'
				break;
		}
	}
//GPRS连接
	if((UART1Ctrl->LinkTask==101)||(UART1Ctrl->LinkTask==102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
	{
	#if (USER/100)==0//电科院测试
		p16[3]=69;//'有框4G'
		p16[4]=70;
//		p16[3]=7;//'有框G'
//		p16[4]=8;
	#else
		switch(Comm_Ram->RemoteWirelessModuleSYSINFO)//远程无线模块当前注册的网络:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
		{
			case 4:
				p16[3]=69;//'有框4G'
				p16[4]=70;
				break;
			case 3:
				p16[3]=67;//'有框3G'
				p16[4]=68;
				break;
			default:
				p16[3]=65;//'有框2G'
				p16[4]=66;
				break;
		}
	#endif
	}
	
//以太网连接
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if((UARTCtrl->LinkTask==101)||(UARTCtrl->LinkTask==102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
	{
	#if (USER/100)==5//上海
		p16[5]=30;//30 'E'
		p16[6]=31;
	#else
		p16[5]=11;//11 'L'
		p16[6]=12;
	#endif
	}
	
#if ((Project/100)==3)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=通信模块
//保电状态
	p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
	if(p8[1]!=0)
	{
		p16[7]=13;//保电
		p16[8]=14;
	}
#endif
#if ((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=通信模块
//保电状态
	if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
	{
		p8=(u8 *)(ADDR_8001_2);//保电-属性2（保电状态，只读）
		if(p8[1]!=0)
		{
			p16[7]=13;//保电
			p16[8]=14;
		}
	}
#endif
	
//尖峰平谷
	p8=(u8 *)(ADDR_METER07_CTRL_SRAM);
#if (USER/100)==5//是上海
	switch(p8[0])
	{
		case 1:
			DisplayString(Row,10,0,(u8 *)"峰");//显示字符串
			break;
		case 2:
			DisplayString(Row,10,0,(u8 *)"平");//显示字符串
			break;
		case 3:
			DisplayString(Row,10,0,(u8 *)"尖");//显示字符串
			break;
		case 4:
			DisplayString(Row,10,0,(u8 *)"谷");//显示字符串
			break;
	}
#else//不是上海
	switch(p8[0])
	{
		case 1:
			DisplayString(Row,10,0,(u8 *)"尖");//显示字符串
			break;
		case 2:
			DisplayString(Row,10,0,(u8 *)"峰");//显示字符串
			break;
		case 3:
			DisplayString(Row,10,0,(u8 *)"平");//显示字符串
			break;
		case 4:
			DisplayString(Row,10,0,(u8 *)"谷");//显示字符串
			break;
	}
#endif	
//异常告警指示,优先尖峰平谷显示
//表示终端或测量点有异常情况。当终端发生异常时，该标志将以1Hz的频率闪烁显示
//#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
//	p8=(u8 *)(ADDR_TYMDHMS);
//	if(p8[0]&0x1)
//	{
//		i=Terminal_Contrl_Round();//终端当前设定的控制轮次(功控合电控);返回B0-B7位标志
//		p8=(u8 *)(ADDR_ContrlModeData+2);//从控制模块输入的断线标志字节0:b0=回路1常开,b1=回路1常闭,b2=回路2常开,b3回路2常闭;(1断线,0未断线)
//		x=0;
//		if((p8[0]&0x3)==0x3)
//		{
//			x=1;
//		}
//		if(((p8[0]>>2)&0x3)==0x3)
//		{
//			x|=2;
//		}
//		i&=x;
//		if(i!=0x0)
//		{
//			p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(10*2));
//			p16[0]=17;// '告警'
//			p16[1]=18;
//		}
//	}
//#endif
	
//当前时钟
	p8=(u8 *)(ADDR_RTCBUFF);
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(12*2));
	p16[0]=((p8[2]>>4)&0xf)+0x30+0x2000;
	p16[1]=((p8[2]>>0)&0xf)+0x30+0x2000;
	p16[2]=':'+0x2000;
	p16[3]=((p8[1]>>4)&0xf)+0x30+0x2000;
	p16[4]=((p8[1]>>0)&0xf)+0x30+0x2000;
	p16[5]=':'+0x2000;
	p16[6]=((p8[0]>>4)&0xf)+0x30+0x2000;
	p16[7]=((p8[0]>>0)&0xf)+0x30+0x2000;
//
	Row++;
	return Row;
#endif
}

u32 DisplayLoop_BottomICO(u32 Row)//底图标显示
{
	u32 i;
	u32 x;
  u16 *p16;
#if (USER/100)!=5//上海
  UARTCtrl_TypeDef * UARTCtrl;
#endif
#if (USER/100)==15//安徽
	u8 *p8;
	u8 *p8time;
#endif

#if (USER/100)==5//上海
//UA UB UC
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4);
	if(Meter_Ram->IPFlags&0x80)
	{//电压逆相序
		DisplayString(Row,4,0,(u8 *)"Uc Ub Ua");//显示字符串
		if(Meter_Ram->LPFlags&0x7)
		{
			DisplayString(Row,0,0,(u8 *)"断相");//显示字符串
			if(Meter_Ram->LPFlags&0x1)
			{//A
				p16[10]|=0X8000;
				p16[11]|=0X8000;
			}
			if(Meter_Ram->LPFlags&0x2)
			{//B
				p16[7]|=0X8000;
				p16[8]|=0X8000;
			}
			if(Meter_Ram->LPFlags&0x4)
			{//C
				p16[4]|=0X8000;
				p16[5]|=0X8000;
			}
		}
		if(Meter_Ram->LVFlags&0x7)
		{
			DisplayString(Row,0,0,(u8 *)"失压");//显示字符串
			if(Meter_Ram->LVFlags&0x1)
			{//A
				p16[10]|=0X8000;
				p16[11]|=0X8000;
			}
			if(Meter_Ram->LVFlags&0x2)
			{//B
				p16[7]|=0X8000;
				p16[8]|=0X8000;
			}
			if(Meter_Ram->LVFlags&0x4)
			{//C
				p16[4]|=0X8000;
				p16[5]|=0X8000;
			}
		}
	}
	else
	{
		DisplayString(Row,4,0,(u8 *)"Ua Ub Uc");//显示字符串
		if(Meter_Ram->LPFlags&0x7)
		{
			DisplayString(Row,0,0,(u8 *)"断相");//显示字符串
			if(Meter_Ram->LPFlags&0x1)
			{//A
				p16[4]|=0X8000;
				p16[5]|=0X8000;
			}
			if(Meter_Ram->LPFlags&0x2)
			{//B
				p16[7]|=0X8000;
				p16[8]|=0X8000;
			}
			if(Meter_Ram->LPFlags&0x4)
			{//C
				p16[10]|=0X8000;
				p16[11]|=0X8000;
			}
		}
		if(Meter_Ram->LVFlags&0x7)
		{
			DisplayString(Row,0,0,(u8 *)"失压");//显示字符串
			if(Meter_Ram->LVFlags&0x1)
			{//A
				p16[4]|=0X8000;
				p16[5]|=0X8000;
			}
			if(Meter_Ram->LVFlags&0x2)
			{//B
				p16[7]|=0X8000;
				p16[8]|=0X8000;
			}
			if(Meter_Ram->LVFlags&0x4)
			{//C
				p16[10]|=0X8000;
				p16[11]|=0X8000;
			}
		}
	}
#else//#if (USER/100)==5//上海
//终端抄表状态
	i=DOTMIXPMODE->Ln9Code;//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=打开串口,2=检测通信模块,3=检测SIM卡,4=网络注册,5=获取信号,6=开始拨号,7=登录主站,8=登录主站成功
	switch(i)
	{
	#if LCDLn9Code!=0//屏幕底部(Ln=9)显示内容代号:0=终端抄表状态,1=GPRS连接状态
		case 1:
			DisplayString(Row,0,0,(u8 *)"打开串口");//显示字符串
			break;
		case 2:
			DisplayString(Row,0,0,(u8 *)"检测通信模块");//显示字符串
			break;
		case 3:
			DisplayString(Row,0,0,(u8 *)"检测SIM卡");//显示字符串
			break;
		case 4:
			DisplayString(Row,0,0,(u8 *)"网络注册");//显示字符串
			break;
		case 5:
			DisplayString(Row,0,0,(u8 *)"获取信号");//显示字符串
			break;
		case 6:
			DisplayString(Row,0,0,(u8 *)"开始拨号");//显示字符串
			break;
		case 7:
			DisplayString(Row,0,0,(u8 *)"登录主站");//显示字符串
			break;
		case 8:
			DisplayString(Row,0,0,(u8 *)"登录主站成功");//显示字符串
			break;
	#endif
		
		default:
			if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
			{
		#if MainProtocol==376
				//376
				i=0;
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
				if((UARTCtrl->RMFlag!=0)&&(UARTCtrl->FnEnd!=0))//0=抄表间隔暂停,1=抄表进行中;485抄表Fn计数结束标志,0=结束,1=没结束
				{
					i=1;
				}
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
				if((UARTCtrl->RMFlag!=0)&&(UARTCtrl->FnEnd!=0))//0=抄表间隔暂停,1=抄表进行中;485抄表Fn计数结束标志,0=结束,1=没结束
				{
					i=1;
				}
				if(i!=0)
				{
			#if RMM_RS485_PassageZero==1//RS485通道抄表过0点;0=不暂停,1=暂停
					x=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);
					if((x>=0x2355)||(x<0x0005))
					{//RS485口在每日0点附近暂停抄表
						i=0;
					}
			#endif
				}
			#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
				if(Terminal_Router->RouterRunMode_4==1)//路由运行模式_周期抄表模式： 01表示通信模块仅支持集中器主导的周期抄表模式，10表示通信模块仅支持路由主导的周期抄表模式；11表示通信模块对两种抄表模式都支持；00保留
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					if((UARTCtrl->RMFlag!=0)&&(UARTCtrl->FnEnd!=0))//0=抄表间隔暂停,1=抄表进行中;485抄表Fn计数结束标志,0=结束,1=没结束
					{
						i=1;
					}
				}
				else
				{
					//暂停路由标志:
					//b0=抄表间隔定时,b1=抄重点户,b2=数据转发,b3=UART读路由信息,b4=广播校时,b5=广播冻结,b6=通断电控制,b7=实时抄读
					//b8=档案同步,b9=批量对时,b10=命令暂停抄表,b11=不允许自动抄表,b12=非抄表时段,b13=自动搜表,b14=等待时钟过0,b15=正在冻结
					//b16=路由软件更新,b17=,b18=,b19=,b20=,b21=,b22=,b23=
					if(((Terminal_Router->StopFlags&0xFFFFFF7D)==0)&&(Terminal_Router->RouterInitTask>=200)&&(Terminal_Router->NoteNum!=0))
					{
						i=1;
					}
				}
			#endif
		#else
			//698
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
				i=UARTCtrl->TaskID;
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
				i|=UARTCtrl->TaskID;
			#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
				i|=UARTCtrl->TaskID;
			#endif
			#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
				if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					i|=UARTCtrl->TaskID;
				}
			#endif
				i>>=8;
		#endif
				if(i!=0)
				{
					DisplayString(Row,0,0,(u8 *)"终端正在抄表...");//显示字符串
				}
			}
			break;
	}
#endif//#if (USER/100)==5//上海
	
//设置键
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(14*2));
#if KEY_SET==1//设置按键0=没,1=有
	#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
	if(MRR(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '编程按键'
		p16[1]=24;
	}
	#else
	if(MRR(ADDR_TerminalFMTimer+12,2))
	{
		p16[0]=23;// '编程按键'
		p16[1]=24;
	}
	#endif
#endif
//公钥

	
#if (USER/100)==15//安徽
	p8=(u8*)ADDR_UART7Rx;
	MR(ADDR_DATABUFF,ADDR_MYMDHMS,6);
	p8time=(u8*)ADDR_DATABUFF;
	if((p8time[0]%2)==0)
	{
		TESAM_CMD_TxRx(0x803600FF,0,0);
		if(p8[17]==0)//公钥
		{
			p16[0]=103;
			p16[1]=104;
		}
	}	
#endif
//工厂模式(优先于设置键)
	if(Comm_Ram->Factory==0x55)//工厂状态
	{
		p16[0]=25;// '工厂模式'
		p16[1]=26;
		

	}
//时钟电池欠压
	if(Comm_Ram->URTC<0x200)
	{//小于2.00V
		if(Comm_Ram->RTCBuff[0]&1)
		{
	  	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(16*2));
			p16[0]=21;//'电池欠压'
			p16[1]=22;
		}
	}
//可充电电池
	x=Comm_Ram->UBACKUP;
	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		x=bcd_hex(x);
		if(x>=30)
		{
			x-=30;//电网供电正在充电-0.30V
		}
		else
		{
			x=0;
		}
		x=hex_bcd(x);
	}
	i=38;//38'电池0格图'
	if(x>=RechargeableBatteryGrid1)
	{
  	i=40;//40'电池1格图'
	}
	if(x>=RechargeableBatteryGrid2)
	{
  	i=42;//42'电池2格图'
	}
	if(x>=RechargeableBatteryGrid3)
	{
  	i=44;//44'电池3格图'
	}
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(18*2));
	p16[0]=i;
	p16[1]=i+1;

	Row++;
	return Row;
}
/*
u32 DisplayLoop_BottomICO(u32 Row)//底图标显示
{
	u32 i;
	u32 x;
  u16 *p16;
	
//状态量输入变位或未变位
	x=MRR(ADDR_AFN0CF9+1,1);//D0～D7按位对应1～8路状态量的变位CD，置"O"：自前次遥信传送后无状态变化；置"1"：自前次遥信传送后至少有一次状态变化
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(4*2));
	//for(i=0;i<Get_SwitchIn_Route();i++)
	for(i=0;i<8;i++)
	{
		if((x&(1<<i))==0)
		{
			p16[0]='-'+0x2000;
		}
		else
		{
			p16[0]=27;//上下箭头
		}
		p16++;
	}
//时钟电池欠压
	if(Comm_Ram->URTC<0x200)
	{//小于2.00V
		if(Comm_Ram->YMDHMS[0]&1)
		{
	  	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(14*2));
			p16[0]=21;//'电池欠压'
			p16[1]=22;
		}
	}
//可充电电池
	x=Comm_Ram->UBACKUP;
	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		x=bcd_hex(x);
		if(x>=30)
		{
			x-=30;//电网供电正在充电-0.30V
		}
		else
		{
			x=0;
		}
		x=hex_bcd(x);
	}
	i=38;//38'电池0格图'
	if(x>=RechargeableBatteryGrid1)
	{
  	i=40;//40'电池1格图'
	}
	if(x>=RechargeableBatteryGrid2)
	{
  	i=42;//42'电池2格图'
	}
	if(x>=RechargeableBatteryGrid3)
	{
  	i=44;//44'电池3格图'
	}
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(16*2));
	p16[0]=i;
	p16[1]=i+1;
//设置键
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(18*2));
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
	if(MRR(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '编程按键'
		p16[1]=24;
	}
#else
	if(MRR(ADDR_TerminalFMTimer+12,2))
	{
		p16[0]=23;// '编程按键'
		p16[1]=24;
	}
#endif
//工厂模式(优先于设置键)
	if(Comm_Ram->Factory==0x55)//工厂状态
	{
		p16[0]=25;// '工厂模式'
		p16[1]=26;
	}
	Row++;
	return Row;
}
*/



