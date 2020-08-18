#include "../Hcsg/ProjectCSG.h"
#include "../Hcsg/IRAMCSG.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
//#include "../I2_terminal/Terminal_SwitchIn.h"
//#include "../I2_terminal/Terminal_Contrl_Out.h"
#include "../Display/Display.h"
#include "../Calculate/Calculate.h"




u32 DisplayLoop_TopICO(u32 Row)//顶图标和时钟显示
{
	u32 i,j,k,x,y,m,n;
	u8 *p8;
  u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
#if (Project/100)==3 ||(Project/100)==9    //南网负控
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
#endif

	
	//无线信号和连接
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(0*2));
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
	if((UART1Ctrl->LinkTask==101)||(UART1Ctrl->LinkTask==102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
	{
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
			if(Comm_Ram->RemoteWirelessModuleType == (1 << 0))//2G GSM/GPRS模式
			{
				p16[3]=65;//有框2G
				p16[4]=66;
			}
			else if(Comm_Ram->RemoteWirelessModuleType == (1 << 1))//3G WCDMA模式
			{
				p16[3]=67;//有框3G
				p16[4]=68;
			}
			else if(Comm_Ram->RemoteWirelessModuleType == (1 << 5))//4G LTE模式
			{
				p16[3]=69;//有框4G
				p16[4]=70;
			}
			else
			{
				p16[3]=9;//'C'
				p16[4]=10;
			}
		}
	}
	
//以太网连接
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if((UARTCtrl->LinkTask==101)||(UARTCtrl->LinkTask==102))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录
	{
		p16[3]=11;//11 'L'
		p16[4]=12;
	}



//异常告警指示，表示终端或测量点有异常情况。当终端发生异常时，该标志将以1Hz的频率闪烁显示
	p8=(u8 *)(ADDR_MYMDHMS);
	y=TerminalGetLastAlarmDI();
	k=(y>>24)&0x000000FF;               //E2
	j=y&0x00FFFF00;                     //0000
	m=(y>>4)&0x0000000F;
	n=y&0x0000000F;
	if((k==0xE2)&&(j==0)&&(m<=4))
	{		
		if((p8[0]%2)==1)
		{	
			p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(5*2));
			p16[0]=17;// '告警'
			p16[1]=18;
		}
		if((p8[0]%2)==0)
		{
			p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(5*2));
			p16[0]=0x2030 + m;
			if(n==0x0A)
			{
				p16[1]=0x2000 + 'A';
			}
			else if(n==0x0B)
			{
				p16[1]=0x2000 + 'B';
			}
			else if(n==0x0C)
			{
				p16[1]=0x2000 + 'C';
			}
			else if(n==0x0D)
			{
				p16[1]=0x2000 + 'D';
			}
			else if(n==0x0E)
			{
				p16[1]=0x2000 + 'E';
			}
			else if(n==0x0F)
			{
				p16[1]=0x2000 + 'F';
			}
			else
			{
				p16[1]=0x2030 + n;
			}	
		}
	}


//轮显或查询显示的测量点号	
//默认显示测量点号0
#if (Project/100)==3 || (Project/100)==9      //南网负控
	p16=(u16 *)(ADDR_STRINGBUFF+(0*84)+4+(8*2));
	i = hex_bcd(Pn_Lcd_Data->u16Pn);
	if(Pn_Lcd_Data->u16Pn==0xFFFF)              //测量点不存在时，测量点号为0xFFFF
	{
		p16[0]=0x2030; 
		p16[1]=0x2030;
		p16[2]=0x2030; 
		p16[3]=0x2030;
	}
	else
	{
		p16[0]=0x2000 + (((i >> 12) & 0x000F) + 0x30);
		p16[1]=0x2000 + (((i >> 8) & 0x000F) + 0x30);
		p16[2]=0x2000 + (((i >> 4) & 0x000F) + 0x30);
		p16[3]=0x2000 + ((i & 0x000F) + 0x30);
	}
#endif	


//可充电电池
	x=Comm_Ram->UBACKUP;
	if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		x=bcd_hex(x);
		if(x>=20)
		{
			x-=20;//电网供电正在充电-0.30V
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
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(12*2));
	p16[0]=i;
	p16[1]=i+1;

	
//当前时钟
	p8=(u8 *)(ADDR_RTCBUFF);
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(15*2));
	p16[0]=((p8[2]>>4)&0xf)+0x30+0x2000;
	p16[1]=((p8[2]>>0)&0xf)+0x30+0x2000;
	p16[2]=':'+0x2000;
	p16[3]=((p8[1]>>4)&0xf)+0x30+0x2000;
	p16[4]=((p8[1]>>0)&0xf)+0x30+0x2000;
//	p16[5]=':'+0x2000;
//	p16[6]=((p8[0]>>4)&0xf)+0x30+0x2000;
//	p16[7]=((p8[0]>>0)&0xf)+0x30+0x2000;
//
	Row++;
	return Row;
}





u32 DisplayLoop_BottomICO(u32 Row)//底图标显示
{
  u16 *p16;
	#if (Project/100)==3 || (Project/100)==9
		u64 Databuff = 0;
		QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485I = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485II = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485III = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
	#endif
	
	#if (Project/100)==3 || (Project/100)==9
	if((pstRDMeter_Ctrl_RS485I->u8Rdm_Sem != 0)||(pstRDMeter_Ctrl_RS485II->u8Rdm_Sem != 0)||(pstRDMeter_Ctrl_RS485III->u8Rdm_Sem != 0))
	{
		DisplayString(Row,0,0,(u8 *)"终端正在抄表..");//显示字符串
	}
	else
	{
		DisplayString(Row,0,0,(u8 *)"              ");//显示字符串
	}
	#endif	

	
	#if (Project/100)==2
		MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
		TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
		if((MeterFileInfo->TotalMeters > 0)&&((pTSysCtrl->Flag & TERMINAL_READ_METER_ON_FLAG) != 0))
		{
			DisplayString(Row,0,0,(u8 *)"终端正在抄表..");//显示字符串
		}
		else
		{
			DisplayString(Row,0,0,(u8 *)"              ");//显示字符串
		}
	#endif	




//设置键
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(12*2));
#if KEY_SET==1//设置按键0=没,1=有
	#if ((Project/100)<5) || ((Project/100)==9)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
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
	  		p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(14*2));
			p16[0]=21;//'电池欠压'
			p16[1]=22;
		}
	}


//失压标志
#if (Project/100)==3 || (Project/100)==9
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(16*2));          //失压标志
	Pn_Lcd_Data->u16Res1 |= LCD_RT_BOTTOM_ICO;									
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Terminal040005FF[6],6);	
	if(Databuff == 0xFFFFFFFFFFFF )
	{
		p16[0]=71;//不失压
		p16[1]=72;
	}
	else
	{
		Databuff &= 0x100010001;
		if(Databuff == 0x000000001)
		{//A相失压
			p16[0]=77;
			p16[1]=78;
		}
		if(Databuff == 0x000010000)
		{//B相失压
			p16[0]=75;
			p16[1]=76;
		}
		if(Databuff == 0x100000000)
		{//C相失压
			p16[0]=73;
			p16[1]=74;
		}
		if(Databuff == 0x000010001)
		{//AB相失压
			p16[0]=83;
			p16[1]=84;
		}
		if(Databuff == 0x100010000)
		{//BC相失压
			p16[0]=79;
			p16[1]=80;
		}
		if(Databuff == 0x100000001)
		{//AC相失压
			p16[0]=81;
			p16[1]=82;
		}
		if(Databuff == 0x00)
		{//不失压
			p16[0]=71;
			p16[1]=72;
		}
		if(Databuff == 0x100010001)
		{//全失压
			p16[0]=85;
			p16[1]=86;
		}
	}
	
	
	//失流标志
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(18*2));          //失流标志
	Pn_Lcd_Data->u16Res1 |= LCD_RT_BOTTOM_ICO;									
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Terminal040005FF[6],6);
	if(Databuff == 0xFFFFFFFFFFFF )
	{
		p16[0]=87;//不失流
		p16[1]=88;
	}
	else
	{	
		Databuff &= 0x800080008;
		if(Databuff == 0x000000008)
		{//A相失流
			p16[0]=93;
			p16[1]=94;
		}
		if(Databuff == 0x000080000)
		{//B相失流
			p16[0]=89;
			p16[1]=90;
		}
		if(Databuff == 0x800000000)
		{//C相失流
			p16[0]=91;
			p16[1]=92;
		}
		if(Databuff == 0x000080008)
		{//AB相失流
			p16[0]=99;
			p16[1]=100;
		}
		if(Databuff == 0x800080000)
		{//BC相失流
			p16[0]=95;
			p16[1]=96;
		}
		if(Databuff == 0x800000008)
		{//AC相失流
			p16[0]=97;
			p16[1]=98;
		}
		if(Databuff == 0x00)
		{//不失流
			p16[0]=87;
			p16[1]=88;
		}
		if(Databuff == 0x800080008)
		{//全失流
			p16[0]=101;
			p16[1]=102;
		}
	}
#endif	
	Row++;
	return Row;
}



