
#include "../Hcsg/ProjectCSG.h"

#include "../MS/Ethernet.h"
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/LAN8720A.h"
#include "../Device/24AA02E48.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"
#include "../Display/Warning.h"
#include "../OS/OS.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"



int32_t tcp_cb_client(int32_t socket, netTCP_Event event, NET_ADDR *addr,uint8_t *buf, uint32_t len)
{
	u32 i;
	u32 x;
	u32 p0;
	u32 Max_LEN_BUFF;
	u8 *p8;
	u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
	
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	
  switch (event) 
	{
    case netTCP_EventConnect:
      // Connect request received
			i=i;
      break;
 
    case netTCP_EventEstablished:
      // Connection established
			p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
			p16[0]=0;
			UARTCtrl->LinkSubTask=2;//链接子任务
      break;
 
    case netTCP_EventClosed:
      // Connection was properly closed
			p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
			p16[0]=0;
			UARTCtrl->LinkTask=99;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			UARTCtrl->LinkSubTask=3;//链接子任务
      break;
 
    case netTCP_EventAborted:
      // Connection is for some reason aborted
			p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
			p16[0]=0;
			UARTCtrl->LinkSubTask=4;//链接子任务
      break;
 
    case netTCP_EventACK:
      // Previously sent data acknowledged
      break;
 
    case netTCP_EventData:
      // Data received
			p16=(u16*)Get_ADDR_UARTn(ENETPORT);//得到UART缓冲首地址
			Max_LEN_BUFF=Get_LEN_UARTnIntRx(ENETPORT);//得到UART中断接收缓冲长度
			Max_LEN_BUFF-=10;
			p0=p16[0];
			if(p0>=Max_LEN_BUFF)
			{
				p0=0;
			}
			p8=(u8*)((u32)p16)+10;
			while(len)
			{
				x=Max_LEN_BUFF-p0;//当前缓冲到满还可填入字节数
				if(x>len)
				{
					x=len;
				}
				for(i=0;i<x;i++)
				{
					p8[p0]=buf[i];
					p0++;
				}
				buf+=x;
				len-=x;
				if(p0>=Max_LEN_BUFF)
				{
					p0=0;
				}
			}
			p16[0]=p0;
      break;
  }
  return (0);
}
uint32_t udp_callback(int32_t socket, const NET_ADDR *addr, const uint8_t *buf, uint32_t len)
{
  // This function is called when UDP data is received 
  // Process received data from 'buf' 
	u32 i;
	u32 x;
	u32 p0;
	u32 Max_LEN_BUFF;
	u8 *p8;
	u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	if(UARTCtrl->LinkSubTask==6)//链接子任务
	{
		//case 6://ETH死机检查等待接收
		for(i=0;i<16;i++)
		{
			if(buf[i]!=i)
			{
				break;
			}
		}
		if(i>=16)
		{
			UARTCtrl->LinkSubTask++;//接收OK
		}
		return (0);
	}
	p16=(u16*)Get_ADDR_UARTn(ENETPORT);//得到UART缓冲首地址
	Max_LEN_BUFF=Get_LEN_UARTnIntRx(ENETPORT);//得到UART中断接收缓冲长度
	Max_LEN_BUFF-=10;
	p0=p16[0];
	if(p0>=Max_LEN_BUFF)
	{
		p0=0;
	}
	p8=(u8*)((u32)p16)+10;
	while(len)
	{
		x=Max_LEN_BUFF-p0;//当前缓冲到满还可填入字节数
		if(x>len)
		{
			x=len;
		}
		for(i=0;i<x;i++)
		{
			p8[p0]=buf[i];
			p0++;
		}
		buf+=x;
		len-=x;
		if(p0>=Max_LEN_BUFF)
		{
			p0=0;
		}
	}
	p16[0]=p0;
  return (0);
}
uint32_t tcp_cb_server (int32_t socket, netTCP_Event event,const NET_ADDR *addr, const uint8_t *buf, uint32_t len)
{   
	switch (event)
	{    
		case netTCP_EventConnect:
			// Connect request received 
			// Example
//			if (addr->addr_type == NET_ADDR_IP4)
//			{ // IPv4 client
//				if (addr->addr[0] == 192  &&
//						addr->addr[1] == 168  &&
//						addr->addr[2] == 0    &&
//						addr->addr[3] == 1)
//				{// Accept connection from client at 192.168.0.1 
//					return (1);
//				}
//			}
//			else
//			{// IPv6 client
//				const uint8_t ip6_addr[NET_ADDR_IP6_LEN] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//																										0x1c, 0x30, 0x6c, 0xff, 0xfe, 0xa2, 0x45, 0x5e };
//				if (memcmp (addr->addr, ip6_addr, NET_ADDR_IP6_LEN) == 0)
//				{// Accept connection from client at [fe80::1c30:6cff:fea2:455e]
//					return (1);
//				}
//			}
//			// Deny connection.
//			return (0);

			return (1);
		case netTCP_EventEstablished:
			// Connection established
			break;
		case netTCP_EventClosed:
			// Connection was properly closed
			break;
		case netTCP_EventAborted:
			// Connection is for some reason aborted
			break;
		case netTCP_EventACK:
			// Previously sent data acknowledged
			break;
		case netTCP_EventData:
			// Data received

			break;
	} 
	return (0);
}

/*
u32 IP4_ASCII(u8 *hex,u8 *ascii)//填写ip4 ascii,出口:0=没变化,1=变化
{
	u32 i;
	u32 x;
	u32 f;
	
	f=0;//0=没变化
	for(x=0;x<4;x++)
	{
		i=*hex;
		i=hex_bcd(i);
		if(i<0x10)
		{
			if(ascii[0]!=(i+0x30))
			{
				f=1;
				ascii[0]=i+0x30;
			}
			if(x<3)
			{//不是第4字节
				if(ascii[1]!='.')
				{
					f=1;
					ascii[1]='.';
				}
			}
			ascii+=2;
			hex++;
			continue;
		}
		if(i<0x100)
		{
			if(ascii[0]!=((i>>4)+0x30))
			{
				f=1;
				ascii[0]=(i>>4)+0x30;
			}
			if(ascii[1]!=((i&0xf)+0x30))
			{
				f=1;
				ascii[1]=(i&0xf)+0x30;
			}
			if(x<3)
			{//不是第4字节
				if(ascii[2]!='.')
				{
					f=1;
					ascii[2]='.';
				}
			}
			ascii+=3;
			hex++;
			continue;
		}
		if(ascii[0]!=((i>>8)+0x30))
		{
			f=1;
			ascii[0]=(i>>8)+0x30;
		}
		if(ascii[1]!=(((i>>4)&0xf)+0x30))
		{
			f=1;
			ascii[1]=((i>>4)&0xf)+0x30;
		}
		if(ascii[2]!=((i&0xf)+0x30))
		{
			f=1;
			ascii[2]=(i&0xf)+0x30;
		}
		if(x<3)
		{//不是第4字节
			if(ascii[3]!='.')
			{
				f=1;
				ascii[3]='.';
			}
		}
		ascii+=4;
		hex++;
	}
	ascii--;
	ascii[0]=0;
	
	return f;
}
#if MainProtocol==376
u32 Local_ASCII_IP4(void)//填写本地ascii ip,出口:0=没变化,1=变化
{
	u32 i;
	u32 x;
	u32 f;
	u8 *p8s;
	u8 *p8d;
	
	f=0;//0=没变化
//MAC ascii
 #if IC_24AAxxx==1
	Read_24AA02E48(ADDR_DATABUFF,0xFA,6);//24AAxxx读
	MW(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #else
	MR(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #endif
	p8s=(u8*)ADDR_DATABUFF;
	p8d=(u8*)ADDR_MACADDR_ascii;
	for(i=0;i<6;i++)
	{
		if((p8s[i]>>4)<=9)
		{
			x=(p8s[i]>>4)+0x30;
		}
		else
		{
			x=(p8s[i]>>4)+55;
		}
		if(p8d[0]!=x)
		{
			f=1;//1=变化
			p8d[0]=x;
		}
		if((p8s[i]&0xf)<=9)
		{
			x=(p8s[i]&0xf)+0x30;
		}
		else
		{
			x=(p8s[i]&0xf)+55;
		}
		if(p8d[1]!=x)
		{
			f=1;//1=变化
			p8d[1]=x;
		}
		p8d[2]='-';
		p8d+=3;
	}
	p8d--;
	p8d[0]=0;
//本机IP4 ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4ADDR_ascii);//填写ip4 ascii,出口:0=没变化,1=变化
//本机IP4子网掩码 ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7+4,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4MASK_ascii);//填写ip4 ascii,出口:0=没变化,1=变化
//本机IP4网关IP ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7+8,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4GATEWAY_ascii);//填写ip4 ascii,出口:0=没变化,1=变化
//本机IP4 PRIMARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4PRIMARY_DNS_ascii);//填写ip4 ascii,出口:0=没变化,1=变化
//本机IP4 SECONDARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4SECONDARY_DNS_ascii);//填写ip4 ascii,出口:0=没变化,1=变化

	return f;
}
#else//#if MainProtocol==376
u32 Local_ASCII_IP4(void)//填写本地ascii ip,出口:0=没变化,1=变化
{
	u32 i;
	u32 x;
	u32 f;
	u8 *p8s;
	u8 *p8d;
	
	f=0;//0=没变化
//MAC ascii
 #if IC_24AAxxx==1
	Read_24AA02E48(ADDR_DATABUFF,0xFA,6);//24AAxxx读
	//MW(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #else
	MR(ADDR_DATABUFF,ADDR_4510_5+2,6);
 #endif
	p8s=(u8*)ADDR_DATABUFF;
	p8d=(u8*)ADDR_MACADDR_ascii;
	for(i=0;i<6;i++)
	{
		if((p8s[i]>>4)<=9)
		{
			x=(p8s[i]>>4)+0x30;
		}
		else
		{
			x=(p8s[i]>>4)+55;
		}
		if(p8d[0]!=x)
		{
			f=1;//1=变化
			p8d[0]=x;
		}
		if((p8s[i]&0xf)<=9)
		{
			x=(p8s[i]&0xf)+0x30;
		}
		else
		{
			x=(p8s[i]&0xf)+55;
		}
		if(p8d[1]!=x)
		{
			f=1;//1=变化
			p8d[1]=x;
		}
		p8d[2]='-';
		p8d+=3;
	}
	p8d--;
	p8d[0]=0;
//本机IP4 ascii
	MR(ADDR_DATABUFF,ADDR_4510_4+6,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4ADDR_ascii);//填写ip4 ascii,出口:0=没变化,1=变化
//本机IP4子网掩码 ascii
	i=(u32)Get_Element((u8*)ADDR_4510_4,3,0,LENmax_4510_4);//计算元素地址,使用ADDR_128K_DATABUFF
	if(i)
	{
		MR(ADDR_DATABUFF,i+2,4);
	}
	else
	{
		MWR(255+(255<<8)+(255<<16)+(0<<24),ADDR_DATABUFF,4);
	}
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4MASK_ascii);//填写ip4 ascii,出口:0=没变化,1=变化
//本机IP4网关IP ascii
	i=(u32)Get_Element((u8*)ADDR_4510_4,4,0,LENmax_4510_4);//计算元素地址,使用ADDR_128K_DATABUFF
	if(i)
	{
		MR(ADDR_DATABUFF,i+2,4);
	}
	else
	{
		MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	}
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4GATEWAY_ascii);//填写ip4 ascii,出口:0=没变化,1=变化
//本机IP4 PRIMARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4PRIMARY_DNS_ascii);//填写ip4 ascii,出口:0=没变化,1=变化
//本机IP4 SECONDARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4SECONDARY_DNS_ascii);//填写ip4 ascii,出口:0=没变化,1=变化

	return f;
}
#endif

void Ethernet_RxTx(void)
{
	u32 i;
	u32 x;
	u32 sourMACMIIDR;
	u8 *p8s;
	u8 *p8d;
	u16 * p16timer;
	os_Type *os;
	NET_ADDR *addr;
  UARTCtrl_TypeDef * UARTCtrl;
	netStatus ns;
	
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	
	if(UARTCtrl->LinkTask<101)//101://登录ok
	{
		if(Comm_Ram->CheckLEDTimer<=(1000/10))//3000/10;//上电全部LED检查显示 每10MS减1定时器
		{
			Pin_OutH(PIN_ENET_LINK_LED);//引脚输出低,入口引脚使用名
		}
	}
	switch(UARTCtrl->LinkTask)
	{
		case 0://0=延时关机
			if(p16timer[0]==0x0)
			{
				UARTCtrl->LinkTask++;
			}
			return;
		case 1://1=关电源
			UnInit_STM32F4xx_MAC();
		#if MainProtocol==376
			if(Comm_Ram->TYMDHMS[2]==0)
		#else
			if(Comm_Ram->DL698YMDHMS[2]==0)
		#endif
			{//是0时,关电源
				OffE5V0();//关E5V0电源
				p16timer[0]=5000/10;//关闭电源延时5秒
			}
			else
			{//不是0时,不关电源
				p16timer[0]=2000/10;//延时2秒
			}
			UARTCtrl->LinkTask++;
			return;
		case 2://开电源,ETH系统初始化
			if(Comm_Ram->After_Init_OS&(1<<4))//OS重启后使用标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
			{
				Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
				return;
			}
			Terminal_Ram->ENETSEL=0;//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
			if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
			{
				//本机端口号,同时用于掉电时已连接过1次识别
				if(UARTCtrl->PORT==0)
				{
					UARTCtrl->PORT=1;
				}
				else
				{
					return;//电池供电时连接1次后停止
				}
			}
			if(UARTCtrl->LinkCtrl!=1)//连接控制0=不连接,1=连接
			{
				return;
			}
			//开电源
			if(p16timer[0]!=0x0)
			{
				if(p16timer[0]<(2000/10))
				{
					if(Pin_Read(PIN_ENE5V0)==0)//读引脚,入口引脚使用名;返回0或1	
					{
						OnE5V0();//打开E5V0电源
						if(p16timer[0]<(1000/10))
						{
							p16timer[0]=1000/10;//至少1s延时
						}
					}
				}
				return;
			}
			if(Pin_Read(PIN_ENE5V0)==0)//读引脚,入口引脚使用名;返回0或1	
			{
				OnE5V0();//打开E5V0电源
				p16timer[0]=1000/10;//1s延时
				return;
			}
			//ETH系统初始化
			Init_STM32F4xx_MAC();
			Init_LAN8720A();
			p16timer[0]=2000/10;//等待电缆RJ45插入2s延时
			i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,2);
			if(i!=7)
			{
				Comm_Ram->ErrorIC|=0x80;//硬件错误B7=LAN8720
				Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
				UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				return;
			}
			Local_ASCII_IP4();//填写本地ascii ip4,出口:0=没变化,1=变化
			
			os=Get_os();
			os->netInitialize();//每次重启OS后只能调用初始化1次,否则死机
			Comm_Ram->After_Init_OS|=(1<<4);//OS重启后使用标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
			UARTCtrl->DHCPTask=0;//DHCP任务号
			UARTCtrl->TCPSocket=os->netTCP_GetSocket((netTCP_cb_t)tcp_cb_client);
			if(UARTCtrl->TCPSocket<0)
			{
				Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
				return;
			}
			UARTCtrl->UDPSocket=os->netUDP_GetSocket((netUDP_cb_t)udp_callback);
			if(UARTCtrl->UDPSocket<0)
			{
				Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
				return;
			}
			UARTCtrl->LinkTask++;
			return;
		case 3://等待电缆RJ45插入(每2秒检测)
			if(p16timer[0]==0x0)
			{
				p16timer[0]=2000/10;//下次检测定时
				Disable_os();//暂停os
				while(ETH->MACMIIAR&1)
				{//MDC正忙
				}
				sourMACMIIDR=ETH->MACMIIDR;//原值保护
				i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,1);
				ETH->MACMIIDR=sourMACMIIDR;//原值恢复
				Enable_os();//允许os
				if(i&(1<<2))//bit2 link is up,bit5 auto-negotiate process completed
				{//RJ45插入
					if(Terminal_Ram->ENETSEL>=4)//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
					{
						WarningCancel(ENETPORT);//告警取消,入口告警代码
					}
					Terminal_Ram->ENETSEL=0;//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
				#if MainProtocol==376
					i=MRR(ADDR_AFN04F8,1);
				#else
					i=MRR(ADDR_4510_2+5,1);//连接方式  enum{TCP（0），UDP（1）}
					i<<=7;
				#endif
					UARTCtrl->TCPUDP=i;
					if(Comm_Ram->Factory==0x55)
					{//工厂模式时登录不加随机延时方便工厂生产测试
						p16timer[0]=0;//5000/10;//0;//至小延时5秒,滨江新大楼发现<=3秒内不能连接
					}
					else
					{
					#if MainProtocol==376
						i=MRR(ADDR_TERMINAL,1);//终端地址的低1字节作为随机数
						i&=0x0f;
						i*=100;
						if(i<500)
						{
							i+=500;//至小延时5秒,滨江新大楼发现<=3秒内不能连接
						}
						p16timer[0]=i;
					#else//#if MainProtocol==376
						p16timer[0]=0;
					#endif
					}
					UARTCtrl->LinkSubTask=0;//链接子任务
					UARTCtrl->LinkTask++;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				}
				else
				{
					if(Terminal_Ram->ENETSEL<3)//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
					{
						Terminal_Ram->ENETSEL=3;//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
					}
					else
					{//第二次判断
						if(Terminal_Ram->ENETSEL<5)//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
						{
							Terminal_Ram->ENETSEL++;//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
						}
						WarningOccur(ENETPORT,(u8 *)"以太网电缆被拨出");//告警发生,入口告警代码和告警字符串
					}
				}
			}
			return;
		case 4://
		case 5://
		case 6://
			UARTCtrl->LinkTask++;
			return;
		case 7://连接随机延时1-30秒
			if(p16timer[0]==0x0)
			{
				UARTCtrl->LinkTask=99;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;
		case 99://连接
			os=Get_os();
		#if MainProtocol==376
			i=MRR(ADDR_AFN04F7+64,1);
			if(i==0x0)
			{//固定IP连接
				if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
				{
					Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
					return;
				}
			}
			else
			{//自动获取IP连接
				switch(UARTCtrl->DHCPTask)//DHCP任务号
				{
					case 0://启动DHCP
						ns=os->netDHCP_Enable(0);
						if(ns!=netOK)
						{
							Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
							return;
						}
						WarningOccur(ENETPORT,(u8 *)"以太网正在自动获取IP");//告警发生,入口告警代码和告警字符串
						p16timer[0]=60000/10;//等待超时60s
						UARTCtrl->DHCPTask=1;//DHCP任务号
						return;
					case 1://等待DHCP
						if(p16timer[0]==0)
						{//超时
							Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
						}
						return;
					case 2://DPCH完成
						p16timer[0]=0;
						UARTCtrl->DHCPTask=3;//DHCP任务号
						break;
					case 3://连接
						break;
					default:
						UARTCtrl->DHCPTask=0;//DHCP任务号
						return;
				}
			}
		#else//#if MainProtocol==376
			//IP配置方式enum{DHCP（0），静态（1），PPPoE（2）}
			i=MRR(ADDR_4510_4+3,1);
			if(i==1)
			{//静态（1）
				if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
				{
					Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
					return;
				}
			}
			else
			{
				if(i==0)
				{//{DHCP（0）
					switch(UARTCtrl->DHCPTask)//DHCP任务号
					{
						case 0://启动DHCP
							ns=os->netDHCP_Enable(0);
							if(ns!=netOK)
							{
								Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
								return;
							}
							WarningOccur(ENETPORT,(u8 *)"以太网正在自动获取IP");//告警发生,入口告警代码和告警字符串
							p16timer[0]=60000/10;//等待超时60s
							UARTCtrl->DHCPTask=1;//DHCP任务号
							return;
						case 1://等待DHCP
							if(p16timer[0]==0)
							{//超时
								Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
							}
							return;
						case 2://DPCH完成
							p16timer[0]=0;
							UARTCtrl->DHCPTask=3;//DHCP任务号
							break;
						case 3://连接
							break;
						default:
							UARTCtrl->DHCPTask=0;//DHCP任务号
							return;
					}
				}
				else
				{//PPPoE（2）
					
					
				}
			}
		#endif
			if (p16timer[0]!=0x0)
			{
				return;
			}
			switch(UARTCtrl->LinkSubTask)//链接子任务
			{
				case 0://发起连接
					UARTCtrl->Task=0;//0=正在接收
					UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
					UARTCtrl->RxByte=0;//接收字节数=0
					UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
					UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//心跳发出秒定时器
					UARTCtrl->LinkTask2=0;//通道2链接任务(FTP命令);0-99=过程,100=建立连接
					UARTCtrl->Task2=0;//通道2任务(FTP命令)
					
					//本机端口号,同时用于掉电时已连接过1次识别
					i=UARTCtrl->PORT;
					i++;
					if((i<SocketPortnMin)||(i>SocketPortnMax))
					{
						i=SocketPortnMin;
					}
					UARTCtrl->PORT=i;
					//主站IP
					addr=(NET_ADDR *)&UARTCtrl->NET_ADDR;
					if((UARTCtrl->TCPUDP&0x80)==0x0)
					{//TCP
						addr->addr_type = NET_ADDR_IP4;
					#if MainProtocol==376
						if(UARTCtrl->LinkCount<2)
						{//主IP
							MR((u32)&addr->addr[0],ADDR_AFN04F3,4);
							MR((u32)&addr->port,ADDR_AFN04F3+4,2);
						}
						else
						{//备用IP
							MR((u32)&addr->addr[0],ADDR_AFN04F3+6,4);
							MR((u32)&addr->port,ADDR_AFN04F3+6+4,2);
						}
					#else
						i=MRR(ADDR_4510_3+1,1);
						if((UARTCtrl->LinkCount<2)||(i<2))
						{//主IP
							i=(u32)Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
							if(i)
							{
								MR((u32)&addr->addr[0],i+2,4);
							}
							else
							{
								MWR(192+(168<<8)+(3<<16)+(1<<24),(u32)&addr->addr[0],4);
							}
							i=(u32)Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
							if(i)
							{
								i=MRR(i+1,2);
								i=(i>>8)+((i&0xff)<<8);
								addr->port=i;
							}
							else
							{
								addr->port=1024;
							}
						}
						else
						{//备用IP
							i=(u32)Get_Element((u8*)ADDR_4510_3,1,3,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
							if(i)
							{
								MR((u32)&addr->addr[0],i+2,4);
							}
							else
							{
								MWR(192+(168<<8)+(3<<16)+(1<<24),(u32)&addr->addr[0],4);
							}
							i=(u32)Get_Element((u8*)ADDR_4510_3,1,4,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
							if(i)
							{
								i=MRR(i+1,2);
								i=(i>>8)+((i&0xff)<<8);
								addr->port=i;
							}
							else
							{
								addr->port=1024;
							}
						}
					#endif
						os->netTCP_Close(UARTCtrl->TCPSocket);
						ns=os->netTCP_Connect(UARTCtrl->TCPSocket,addr,UARTCtrl->PORT);
						if(ns!=netOK)
						{
							Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
							return;
						}
						p16timer[0]=30000/10;//连接超时定时;Net_Config_TCP.h中重试定义#define TCP_MAX_RETRY 5,#define TCP_RETRY_TOUT 4;5*4=20秒
						UARTCtrl->LinkSubTask=1;//链接子任务
					}
					else
					{//UDP
						os->netUDP_Close(UARTCtrl->UDPSocket);
						ns=os->netUDP_Open(UARTCtrl->UDPSocket,UARTCtrl->PORT);
						if(ns!=netOK)
						{
							Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
							return;
						}
						WarningCancel(ENETPORT);//告警取消,入口告警代码
						p16timer[0]=0;//连接超时定时;Net_Config_TCP.h中重试定义#define TCP_MAX_RETRY 5,#define TCP_RETRY_TOUT 4;5*4=20秒
						UARTCtrl->LinkSubTask=0;//链接子任务
						UARTCtrl->LinkTask=100;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					}
					return;
				case 1://等待连接
					if(p16timer[0]==0x0)
					{//连接超时
						if(UARTCtrl->LinkSubTask==1)//链接子任务没在tcp_cb_client()变化,tcp_cb_client()事件会设置LinkSubTask同时设置p16timer[0]=0
						{
							UARTCtrl->LinkCount++;//连接计数+1
							if(UARTCtrl->LinkCount>=3)
							{
								UARTCtrl->LinkCount=0;
								//ETH死机检查
								UARTCtrl->LinkSubTask=5;//链接子任务5=RJ45插入,ETH死机检查
								return;
							}
							UARTCtrl->LinkSubTask=0;//链接子任务
						}
					}
					return;
				case 2://case netTCP_EventEstablished:
					//WarningOccur(ENETPORT,(u8 *)"以太网链接建立");//告警发生,入口告警代码和告警字符串
					WarningCancel(ENETPORT);//告警取消,入口告警代码
					p16timer[0]=0;
					UARTCtrl->LinkSubTask=0;//链接子任务
					UARTCtrl->LinkTask=100;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					return;
				case 3://case netTCP_EventClosed:
					WarningOccur(ENETPORT,(u8 *)"以太网链接被断开");//告警发生,入口告警代码和告警字符串
					p16timer[0]=1000/10;//1秒后重连
					UARTCtrl->LinkSubTask=0;//链接子任务
					return;
				case 4://case netTCP_EventAborted:
//						if(UARTCtrl->LinkCount<2)
//						{//主IP
//							WarningOccur(ENETPORT,(u8 *)"以太网链接主IP失败");//告警发生,入口告警代码和告警字符串
//						}
//						else
//						{//备用IP
//							WarningOccur(ENETPORT,(u8 *)"以太网链接备用IP失败");//告警发生,入口告警代码和告警字符串
//						}
					WarningOccur(ENETPORT,(u8 *)"以太网链接主站失败");//告警发生,入口告警代码和告警字符串
					UARTCtrl->LinkSubTask=1;//链接子任务
					return;
					
				case 5://RJ45插入,ETH死机检查
					Disable_os();//暂停os
					while(ETH->MACMIIAR&1)
					{//MDC正忙
					}
					sourMACMIIDR=ETH->MACMIIDR;//原值保护
					i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,1);
					if((i&(1<<2))==0)//bit2 link is up,bit5 auto-negotiate process completed
					{//RJ45拨出
						ETH->MACMIIDR=sourMACMIIDR;//原值恢复
						Enable_os();//允许os
						UARTCtrl->LinkTask=3;//等待电缆RJ45插入(每2秒检测)
						return;
					}
					i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,0);
					Write_ETH_PHYReg(PHYAddr_LAN8720A_0,0,i|(1<<14));//bit14 loopback mode
					ETH->MACMIIDR=sourMACMIIDR;//原值恢复
					Enable_os();//允许os
					
					addr=(NET_ADDR *)&UARTCtrl->NET_ADDR;
					addr->addr_type = NET_ADDR_IP4;
					addr->addr[0] =255;//广播地址,不能用本机地址测试
					addr->addr[1] =255;
					addr->addr[2] =255;
					addr->addr[3] =255;
					addr->port =1024;
					ns=os->netUDP_Open(UARTCtrl->UDPSocket,1024);
					if(ns)
					{
						ns=ns;
					}
					p8s=os->netUDP_GetBuffer(16);
					if((u32)p8s==0)
					{
						Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
						return;
					}
					for(i=0;i<16;i++)
					{
						p8s[i]=i;
					}
					ns=os->netUDP_Send(UARTCtrl->UDPSocket,addr,p8s,16);
					if(ns)
					{
						ns=ns;
					}
					p16timer[0]=1000/10;//超时定时
					UARTCtrl->LinkSubTask=6;//链接子任务
					return;
				case 6://ETH死机检查等待接收
					if(p16timer[0]==0x0)
					{//接收超时
						WarningOccur(ENETPORT,(u8 *)"以太网内循接测试失败");//告警发生,入口告警代码和告警字符串
						Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
					}
					return;
				case 7://接收OK
					os->netUDP_Close(UARTCtrl->UDPSocket);
					Disable_os();//暂停os
					while(ETH->MACMIIAR&1)
					{//MDC正忙
					}
					sourMACMIIDR=ETH->MACMIIDR;//原值保护
					i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,0);
					i&=~(1<<14);
					Write_ETH_PHYReg(PHYAddr_LAN8720A_0,0,i);//bit14 loopback mode
					ETH->MACMIIDR=sourMACMIIDR;//原值恢复
					Enable_os();//允许os
					UARTCtrl->LinkSubTask=0;//链接子任务
					return;
				default:
					UARTCtrl->LinkSubTask=0;//链接子任务
					return;
			}
			//return;
		case 100://连接ok
			break;
		case 101://登录ok
			Pin_OutL(PIN_ENET_LINK_LED);//引脚输出低,入口引脚使用名
			break;
		case 102://发退出登录
			break;
		case 103://退出登录
			if(Comm_Ram->POWER==3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
			{//电网有电则重新登录,电网掉电则保持
				UARTCtrl->LinkTask=3;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			}
			return;
		case 104://关机
			Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
			p16timer[0]=2000/10;//关机延时2s(与GPRS模块相同时间)
			return;
		default:
			UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			return;
	}

//连接ok,登录ok

	if(Terminal_Ram->ENETCheck_S_Timer==0)//4 以太网IP冲突检查和租赁更新秒定时器
	{
		//经实测暂停os到允许os需24us
		Terminal_Ram->ENETCheck_S_Timer=2;
		Disable_os();//暂停os
		while(ETH->MACMIIAR&1)
		{//MDC正忙
		}
		sourMACMIIDR=ETH->MACMIIDR;//原值保护
		i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,1);
		ETH->MACMIIDR=sourMACMIIDR;//原值恢复
		Enable_os();//允许os
		if((i&(1<<2))==0)//bit2 link is up,bit5 auto-negotiate process completed
		{//RJ45拨出
			UARTCtrl->LinkTask=3;//等待电缆RJ45插入(每2秒检测)
			return;
		}
	#if MainProtocol==376
		i=MRR(ADDR_AFN04F7+64,1);
		if(i==0x0)
		{//固定IP连接
			if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
			{
				Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
				return;
			}
		}	
	#else
		//IP配置方式enum{DHCP（0），静态（1），PPPoE（2）}
		i=MRR(ADDR_4510_4+3,1);
		if(i==1)
		{//静态（1）
			if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
			{
				Comm_Ram->Init_OS|=1<<4;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
				return;
			}
		}	
	#endif
	}

	switch(UARTCtrl->Task)
	{
		case 0://接收
			//在线控制
			if(UARTCtrl->LinkCtrl==0)//连接控制0=不连接,1=连接
			{
				if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{
					UARTCtrl->LinkTask=102;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					UARTCtrl->HeartberatCount=0;//心跳发出计数,收到确认帧清0
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 以太网心跳秒定时器
					return;
				}
			}
			break;
		case 2://等待单帧数据发送
		case 5://等待多帧数据发送
		#if (USER/100)==11//吉林系
			USER11xx_TxAddr(PORTn);//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2
		#endif
		#if LOG_1376_1==1//0=没,1=有
			LOG(13761,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		#endif
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30秒,等待发送数据给主站时间超时定时
			UARTCtrl->Task++;
			break;
		case 3://发送单帧数据正在发送
		case 6://多帧数据正在发送
			if(UARTCtrl->TxByte)
			{
				os=Get_os();
				if((UARTCtrl->TCPUDP&0x80)==0x0)
				{//TCP
					if(os->netTCP_SendReady(UARTCtrl->TCPSocket))
					{//准备好发送
						x=os->netTCP_GetMaxSegmentSize(UARTCtrl->TCPSocket);
						if(x>UARTCtrl->TxByte)
						{
							x=UARTCtrl->TxByte;
						}
						p8d=os->netTCP_GetBuffer(x);
						if((u32)p8d==0)
						{
							if(p16timer[0]==0)//60秒,等待发送数据给主站时间超时定时
							{
								UARTCtrl->LinkTask=3;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
							}
							break;
						}
						p8s=(u8*)Get_ADDR_UARTnTx(ENETPORT);
						p8s+=UARTCtrl->TxCount;
						for(i=0;i<x;i++)
						{
							p8d[i]=p8s[i];
						}
						UARTCtrl->TxByte-=x;
						UARTCtrl->TxCount+=x;
						os->netTCP_Send(UARTCtrl->TCPSocket,p8d,x);
						Terminal_Ram->ENETLED_MS_Timer=50/10;//14 以太网接收发送LED指示MS定时器,每10MS减1定时器
					}
					else
					{
						if(p16timer[0]==0)//60秒,等待发送数据给主站时间超时定时
						{
							UARTCtrl->LinkTask=3;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
						}
					}
				}
				else
				{//UDP
					x=1440;
					if(x>UARTCtrl->TxByte)
					{
						x=UARTCtrl->TxByte;
					}
					p8d=os->netUDP_GetBuffer(x);
					if((u32)p8d==0)
					{
						if(p16timer[0]==0)//60秒,等待发送数据给主站时间超时定时
						{
							UARTCtrl->LinkTask=3;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
						}
						break;
					}
					p8s=(u8*)Get_ADDR_UARTnTx(ENETPORT);
					p8s+=UARTCtrl->TxCount;
					for(i=0;i<x;i++)
					{
						p8d[i]=p8s[i];
					}
					UARTCtrl->TxByte-=x;
					UARTCtrl->TxCount+=x;
					addr=(NET_ADDR *)&UARTCtrl->NET_ADDR;
					addr->addr_type = NET_ADDR_IP4;
				#if MainProtocol==376
					MR((u32)&addr->addr[0],ADDR_AFN04F3,4);
					MR((u32)&addr->port,ADDR_AFN04F3+4,2);
				#else
					i=(u32)Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						MR((u32)&addr->addr[0],i+2,4);
					}
					else
					{
						MWR(192+(168<<8)+(3<<16)+(1<<24),(u32)&addr->addr[0],4);
					}
					i=(u32)Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);
						i=(i>>8)+((i&0xff)<<8);
						addr->port=i;
					}
					else
					{
						addr->port=1024;
					}
				#endif
					os->netUDP_Send(UARTCtrl->UDPSocket,addr,p8d,x);
					Terminal_Ram->ENETLED_MS_Timer=50/10;//14 以太网接收发送LED指示MS定时器,每10MS减1定时器
				}
			}
			else
			{
				if(UARTCtrl->Task==3)
				{
					UARTCtrl->Task=7;//7=等待发送器空转为接收
				}
				else
				{
					UARTCtrl->Task=4;//4=多帧数据续接收完成
				}
			}
			break;
	}
}

*/


















