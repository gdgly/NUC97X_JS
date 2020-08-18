
#if MainProtocol==376
#include "../Head/Project.h"
#include "../terminal/Terminal_USER11xx.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_Uart.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#endif

#include "../MS/Ethernet.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/24AA02E48.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"

#ifdef IC_STM32F4xx
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../Device/LAN8720A.h"
#endif
#ifdef IC_NUC9xx
#include "../../NUC97x/NUC97x/NUC97x_MAC.h"
#include "../../NUC97x/Driver/NUC97x_IP101GR.h"
#endif

#include "../MS/MS.h"
#include "../Calculate/ascii.h"


#ifdef LINUX
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
		#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
		MR(ADDR_DATABUFF,ADDR_ETHMAC,6);
		#else
		MR(ADDR_DATABUFF,ADDR_ETHMAC+6,6);
		#endif
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
#endif
#if MainProtocol==698
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
		#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
		MR(ADDR_DATABUFF,ADDR_ETHMAC,6);
		#else
		MR(ADDR_DATABUFF,ADDR_ETHMAC+6,6);
		#endif
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
#if MainProtocol==CSG
u32 Local_ASCII_IP4(void)//填写本地ascii ip,出口:0=没变化,1=变化
{
	u32 f;
/*
	u32 i;
	u32 x;
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

*/
	return f;

}
#endif


void Ethernet_RxTx(void)
{
#if (IC_LAN8720|IC_IP101)==0
	u16 * p16timer;
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	if(p16timer[0]==0)
	{
		#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
			OffE5V0();//关E5V0电源
		#else
			Off2E5V0();//关E5V0电源
		#endif
		p16timer[0]=5000/10;//关闭电源延时5秒
	}
#else
	u32 i;
	u32 x;
	u32 sourMACMIIDR;
	u8 *p8s;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 LEN_UARTnTx;
	u32 LEN_ADDR;
	u32 LEN_TAPDU;
	u32 LEN_TxAPDU;
	u32 LEN_Tx;
	char* p8tx;
	ms_Type * ms;
	struct sockaddr_in addr;
	u16* p16;
	u32 p0;
	
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	
	if(UARTCtrl->LinkTask<101)//101://登录ok
	{
		if(Comm_Ram->CheckLEDTimer<=(1000/10))//3000/10;//上电全部LED检查显示 每10MS减1定时器
		{
			Pin_OutH(PIN_ENET_LINK_LED);//引脚输出低,入口引脚使用名
		}
	}
	ms=Get_ms();
	switch(UARTCtrl->LinkTask)
	{
		case 0://0=延时关机
			if(p16timer[0]==0x0)
			{
				UARTCtrl->LinkTask++;
			}
			return;
		case 1://1=关电源
			ms->link_close(UARTCtrl->TCPSocket);
			ms->link_close(UARTCtrl->UDPSocket);
		#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
			ms->link_system("ifconfig eth0 down");
		#else
			ms->link_system("ifconfig eth1 down");
		#endif
		#if MainProtocol==376
			if(Comm_Ram->TYMDHMS[2]==0)
		#endif
		#if MainProtocol==698
			if(Comm_Ram->DL698YMDHMS[2]==0)
		#endif
		#if MainProtocol==CSG
			if(Comm_Ram->RTCBuff[2]==0)
		#endif
			{//是0时,关电源
			#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
				OffE5V0();//关E5V0电源
			#else
				Off2E5V0();//关E5V0电源
			#endif
				p16timer[0]=5000/10;//关闭电源延时5秒
			}
			else
			{//不是0时,不关电源
				p16timer[0]=2000/10;//延时2秒
			}
			UARTCtrl->LinkTask++;
			return;
		case 2://开电源,ETH系统初始化
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
				#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
					if(Pin_Read(PIN_ENE5V0)==0)//读引脚,入口引脚使用名;返回0或1	
					{
						OnE5V0();//打开E5V0电源
						if(p16timer[0]<(1000/10))
						{
							p16timer[0]=1000/10;//至少1s延时
						}
					}
				#else
					if(Pin_Read(PIN_EN2E5V0)==0)//读引脚,入口引脚使用名;返回0或1	
					{
						On2E5V0();//打开E5V0电源
						if(p16timer[0]<(1000/10))
						{
							p16timer[0]=1000/10;//至少1s延时
						}
					}
				#endif
				}
				return;
			}
		#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
			if(Pin_Read(PIN_ENE5V0)==0)//读引脚,入口引脚使用名;返回0或1	
			{
				OnE5V0();//打开E5V0电源
				p16timer[0]=1000/10;//1s延时
				return;
			}
		#else
			if(Pin_Read(PIN_EN2E5V0)==0)//读引脚,入口引脚使用名;返回0或1	
			{
				On2E5V0();//打开E5V0电源
				p16timer[0]=1000/10;//1s延时
				return;
			}
		#endif
			//ETH系统初始化
		#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
			OnE5V0();//打开E5V0电源
		#else
			On2E5V0();//打开E5V0电源
		#endif
			p16timer[0]=2000/10;//等待电缆RJ45插入2s延时
			Local_ASCII_IP4();//填写本地ascii ip4,出口:0=没变化,1=变化
			UARTCtrl->DHCPTask=0;//DHCP任务号
			UARTCtrl->LinkTask++;
			p8tx=(char*)ADDR_DATABUFF;
		#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
			p8tx=StrCopy(p8tx,"ifconfig eth0 ");
		#else
			p8tx=StrCopy(p8tx,"ifconfig eth1 ");
		#endif
			p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
			p8tx=StrCopy(p8tx," netmask ");
			p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
			p8tx=StrCopy(p8tx," hw ether ");
			p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
			p8tx=StrCopy(p8tx," broadcast ");
			p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
			p8tx=StrCopy(p8tx," up ");
			p8tx[0]=0;
			p8tx=(char*)ADDR_DATABUFF;
			for(i=0;p8tx[i]!=0;i++)
			{
				if(p8tx[i]=='-')
				{
					for(x=i;p8tx[x]!=0;x++)//定义新的变量开始循环赋值 
					{
						p8tx[x]=p8tx[x+1];
						i--;
					}
				}
			}
			ms->link_system((char*)ADDR_DATABUFF);
			return;
		case 3://等待电缆RJ45插入(每2秒检测)
			if(p16timer[0]==0x0)
			{
			#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
				i=Read_ETH_PHYReg(PHYAddr_IP101,2);
				if(i!=0x0243)//IP101GR
			#else
				i=Read_ETH_PHYReg(PHYAddr_LAN8720,2);
				if(i!=7)//LAN8720
			#endif
				{
				#if (USER/100)!=0//不是电科院
					Comm_Ram->ErrorIC|=0x80;//硬件错误B7=LAN8720
					Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
				#endif
					//UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					return;
				}
				p16timer[0]=2000/10;//下次检测定时
			#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
				ms->Disable_os();//暂停os
				while (inpw(REG_EMAC0_MIIDA) & 0x20000);//MDC正忙  wait busy flag clear
				sourMACMIIDR=inpw(REG_EMAC0_MIID);//原值保护				
				i=Read_ETH_PHYReg(PHYAddr_IP101,1);
				outpw(REG_EMAC0_MIID, sourMACMIIDR);//原值恢复
				ms->Enable_os();//允许os
			#else
				ms->Disable_os();//暂停os
				while (inpw(REG_EMAC1_MIIDA) & 0x20000);//MDC正忙  wait busy flag clear
				sourMACMIIDR=inpw(REG_EMAC1_MIID);//原值保护				
				i=Read_ETH_PHYReg(PHYAddr_LAN8720,1);
				outpw(REG_EMAC1_MIID, sourMACMIIDR);//原值恢复
				ms->Enable_os();//允许os
			#endif
				if((i&0x24)==0x24)//bit2 link is up,bit5 auto-negotiate process completed
				{//RJ45插入同时自动自检测完成(Link status and Auto-negotiation complete)
					if(Terminal_Ram->ENETSEL>=4)//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
					{
						WarningCancel(ENETPORT);//告警取消,入口告警代码
					}
					Terminal_Ram->ENETSEL=0;//以太网模块选择0=,1=,2=,3=RJ45没插入1次,4=没插入2次,5=没插入5次,6=
				#if MainProtocol==376
					i=MRR(ADDR_AFN04F8,1);
				#endif
				#if MainProtocol==698
					i=MRR(ADDR_4510_2+5,1);//连接方式  enum{TCP（0），UDP（1）}
					i<<=7;
				#endif
				#if MainProtocol==CSG
					//i=MRR(ADDR_AFN04F8,1);
					i=0;
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
					#endif
					#if MainProtocol==698
						p16timer[0]=0;
					#endif
					#if MainProtocol==CSG
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
						WarningOccur(ENETPORT,(u8 *)"以太网电缆没插入");//告警发生,入口告警代码和告警字符串
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
		#if MainProtocol==376
			i=MRR(ADDR_AFN04F7+64,1);
			if(i==0x0)
			{//固定IP连接
				if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
				{
					UARTCtrl->LinkTask=0;
					return;
				}
			}
			else
			{//自动获取IP连接

			}
		#endif
		#if MainProtocol==698
			//IP配置方式enum{DHCP（0），静态（1），PPPoE（2）}
			i=MRR(ADDR_4510_4+3,1);
			if(i==1)
			{//静态（1）
				if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
				{
					p8tx=(char*)ADDR_DATABUFF;
				#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
					p8tx=StrCopy(p8tx,"ifconfig eth0 ");
				#else
					p8tx=StrCopy(p8tx,"ifconfig eth1 ");
				#endif
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
					p8tx=StrCopy(p8tx," netmask ");
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
					p8tx=StrCopy(p8tx," hw ether ");
					p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
					p8tx=StrCopy(p8tx," broadcast ");
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
					p8tx=StrCopy(p8tx," up ");
					p8tx[0]=0;
					p8tx=(char*)ADDR_DATABUFF;
					for(i=0;p8tx[i]!=0;i++)
					{
						if(p8tx[i]=='-')
						{
							for(x=i;p8tx[x]!=0;x++)//定义新的变量开始循环赋值 
							{
								p8tx[x]=p8tx[x+1];
								i--;
							}
						}
					}
					ms->link_system((char*)ADDR_DATABUFF);
					UARTCtrl->LinkTask=0;
					return;
				}
			}
			else
			{
				if(i==0)
				{//{DHCP（0）

				}
				else
				{//PPPoE（2）
					
					
				}
			}
		#endif
		#if MainProtocol==CSG
			i=0;//i=MRR(ADDR_AFN04F7+64,1);
			if(i==0x0)
			{//固定IP连接
				if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
				{
					UARTCtrl->LinkTask=0;
					return;
				}
			}
			else
			{//自动获取IP连接

			}
		#endif
			if (p16timer[0]!=0x0)
			{
				return;
			}
			i=UARTCtrl->LinkSubTask;
			switch(i)//链接子任务
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
				#if MainProtocol==698
					UARTCtrl->ReNewLink=0;//0x55=通信参数变化当前帧发送完成后重新连接
				#endif
				//ifconfig
//					p8tx=(char*)ADDR_DATABUFF;
//				#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
//					p8tx=StrCopy(p8tx,"ifconfig eth0 ");
//				#else
//					p8tx=StrCopy(p8tx,"ifconfig eth1 ");
//				#endif
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
//					p8tx=StrCopy(p8tx," netmask ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
//					p8tx=StrCopy(p8tx," hw ether ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
//					p8tx=StrCopy(p8tx," up ");
//					p8tx[0]=0;
//					ms->link_system((char*)ADDR_DATABUFF);
//				//gw
//					p8tx=(char*)ADDR_DATABUFF;
//					p8tx=StrCopy(p8tx,"route add default gw ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
//					p8tx[0]=0;
//					ms->link_system((char*)ADDR_DATABUFF);
//					p8tx=(char*)ADDR_DATABUFF;
//				#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
//					p8tx=StrCopy(p8tx,"ifconfig eth0 ");
//				#else
//					p8tx=StrCopy(p8tx,"ifconfig eth1 ");
//				#endif
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
//					p8tx=StrCopy(p8tx," netmask ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
//					p8tx=StrCopy(p8tx," hw ether ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
//					p8tx=StrCopy(p8tx," broadcast ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
//					p8tx=StrCopy(p8tx," up ");
//					p8tx[0]=0;
//					p8tx=(char*)ADDR_DATABUFF;
//					for(i=0;p8tx[i]!=0;i++)
//					{
//						if(p8tx[i]=='-')
//						{
//							for(x=i;p8tx[x]!=0;x++)//定义新的变量开始循环赋值 
//							{
//								p8tx[x]=p8tx[x+1];
//								i--;
//							}
//						}
//					}
//					ms->link_system((char*)ADDR_DATABUFF);
					//本机端口号,同时用于掉电时已连接过1次识别
					i=UARTCtrl->PORT;
					i++;
					if((i<SocketPortnMin)||(i>SocketPortnMax))
					{
						i=SocketPortnMin;
					}
					UARTCtrl->PORT=i;
					//主站IP
					if((UARTCtrl->TCPUDP&0x80)==0x0)
					{//TCP
						UARTCtrl->TCPSocket=ms->link_socket(AF_INET,SOCK_STREAM,0);//TCP=SOCK_STREAM ,UDP=SOCK_DGRAM
						addr.sin_family = AF_INET;// 使用 IPv4 协议
						i=UARTCtrl->PORT;
						i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
						addr.sin_port = i;//端口号:倒序
						i=IP_ASCII_HEX(ADDR_IP4ADDR_ascii);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
						addr.sin_addr.s_addr = i;//IP地址，注:字节顺序 "192.168.0.1" = 0x0100A8C0
						i=ms->link_bind(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
						ms->link_printf("1-%d\n",i,0,0);
						if(i!=0)
						{//调用成功时返回0，失败时返回-1
							UARTCtrl->LinkTask=0;
							return;
						}
					#if MainProtocol==376
						if(UARTCtrl->LinkCount<2)
						{//主IP
							MR((u32)&addr.sin_addr.s_addr,ADDR_AFN04F3,4);
							i=MRR(ADDR_AFN04F3+4,2);
							i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
							addr.sin_port = i;//端口号:倒序
						}
						else
						{//备用IP
							MR((u32)&addr.sin_addr.s_addr,ADDR_AFN04F3+6,4);
							i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
							addr.sin_port = i;//端口号:倒序
						}
					#endif
					#if MainProtocol==698
						i=MRR(ADDR_4510_3+1,1);
						if((UARTCtrl->LinkCount<2)||(i<2))
						{//主IP
							i=(u32)Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
							ms->link_printf("3-%d\n",i,0,0);
							if(i)
							{
								MR((u32)&addr.sin_addr.s_addr,i+2,4);
							}
							else
							{
								MWR(192+(168<<8)+(1<<16)+(3<<24),(u32)&addr.sin_addr.s_addr,4);
							}
							i=(u32)Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
							ms->link_printf("4-%d\n",i,0,0);
							if(i)
							{
								i=MRR(i+1,2);
								addr.sin_port = i;//端口号:倒序
							}
							else
							{
								i=1024;
								i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
								addr.sin_port = i;//端口号:倒序
							}
						}
						else
						{//备用IP
							i=(u32)Get_Element((u8*)ADDR_4510_3,2,1,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
							if(i)
							{
								MR((u32)&addr.sin_addr.s_addr,i+2,4);
							}
							else
							{
								MWR(192+(168<<8)+(1<<16)+(3<<24),(u32)&addr.sin_addr.s_addr,4);
							}
							i=(u32)Get_Element((u8*)ADDR_4510_3,2,2,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
							if(i)
							{
								i=MRR(i+1,2);
								addr.sin_port = i;//端口号:倒序
							}
							else
							{
								i=1024;
								i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
								addr.sin_port = i;//端口号:倒序
							}
						}
					#endif
					#if MainProtocol==CSG
						
					/*
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
					*/
					#endif
						ms->link_printf("5-%d\n",i,0,0);
						i=ms->link_connect(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
						ms->link_printf("2-%d\n",i,0,0);
						if(i!=0)
						{//调用成功时返回0，失败时返回-1
							UARTCtrl->LinkCount++;//连接计数+1
							if(UARTCtrl->LinkCount>=3)
							{
								UARTCtrl->LinkCount=0;
							}
							UARTCtrl->LinkTask=0;
							return;
						}
						WarningCancel(ENETPORT);//告警取消,入口告警代码
						p16timer[0]=0;//连接超时定时;Net_Config_TCP.h中重试定义#define TCP_MAX_RETRY 5,#define TCP_RETRY_TOUT 4;5*4=20秒
						UARTCtrl->LinkSubTask=0;//链接子任务
						UARTCtrl->LinkTask=100;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机

					}
					else
					{//UDP
						UARTCtrl->UDPSocket=ms->link_socket(AF_INET,SOCK_DGRAM,0);//TCP=SOCK_STREAM ,UDP=SOCK_DGRAM
						addr.sin_family = AF_INET;// 使用 IPv4 协议
						i=UARTCtrl->PORT;
						i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
						addr.sin_port = i;//端口号:倒序
						i=IP_ASCII_HEX(ADDR_IP4ADDR_ascii);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
						addr.sin_addr.s_addr = i;//IP地址，注:字节顺序 "192.168.0.1" = 0x0100A8C0
						i=ms->link_bind(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
						if(i!=0)
						{//调用成功时返回0，失败时返回-1
							UARTCtrl->LinkCount++;//连接计数+1
							if(UARTCtrl->LinkCount>=3)
							{
								UARTCtrl->LinkCount=0;
							}
							UARTCtrl->LinkTask=0;
							return;
						}
						WarningCancel(ENETPORT);//告警取消,入口告警代码
						p16timer[0]=0;//连接超时定时;Net_Config_TCP.h中重试定义#define TCP_MAX_RETRY 5,#define TCP_RETRY_TOUT 4;5*4=20秒
						UARTCtrl->LinkSubTask=0;//链接子任务
						UARTCtrl->LinkTask=100;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					}
					return;
				default:
					UARTCtrl->LinkSubTask=0;//链接子任务
					return;
			}
			//return;
		case 100://连接ok
			ms->link_printf("3-0\n",0,0,0);
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
			p16timer[0]=2000/10;//关机延时2s(与GPRS模块相同时间)
			return;
		default:
			UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			return;
	}

//连接ok,登录ok
	ms->link_printf("3-1\n",0,0,0);
	if(Terminal_Ram->ENETCheck_S_Timer==0)//4 以太网IP冲突检查和租赁更新秒定时器
	{
		ms->link_printf("3-2\n",0,0,0);
		//经实测暂停os到允许os需24us
		Terminal_Ram->ENETCheck_S_Timer=2;
	#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
		Disable_os();//暂停os
		while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
		{//MDC正忙
		}
		sourMACMIIDR=inpw(REG_EMAC0_MIID);//原值保护
		i=Read_ETH_PHYReg(PHYAddr_IP101,1);
		outpw(REG_EMAC0_MIID, sourMACMIIDR);//原值恢复
		Enable_os();//允许os
	#else
		Disable_os();//暂停os
		while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
		{//MDC正忙
		}
		sourMACMIIDR=inpw(REG_EMAC1_MIID);//原值保护
		i=Read_ETH_PHYReg(PHYAddr_LAN8720,1);
		outpw(REG_EMAC1_MIID, sourMACMIIDR);//原值恢复
		Enable_os();//允许os
	#endif
		if((i&(1<<2))==0)//bit2 link is up,bit5 auto-negotiate process completed
		{//RJ45拨出
			UARTCtrl->LinkTask=2;//等待电缆RJ45插入(每2秒检测)
			return;
		}
	#if MainProtocol==376
		i=MRR(ADDR_AFN04F7+64,1);
		if(i==0x0)
		{//固定IP连接
			if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
			{
				UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				return;
			}
		}
	#endif
	#if MainProtocol==698
		//IP配置方式enum{DHCP（0），静态（1），PPPoE（2）}
		i=MRR(ADDR_4510_4+3,1);
		if(i==1)
		{//静态（1）
			if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
			{
				ms->link_printf("3-3\n",0,0,0);
				p8tx=(char*)ADDR_DATABUFF;
				#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
					p8tx=StrCopy(p8tx,"ifconfig eth0 ");
				#else
					p8tx=StrCopy(p8tx,"ifconfig eth1 ");
				#endif
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
					p8tx=StrCopy(p8tx," netmask ");
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
					p8tx=StrCopy(p8tx," hw ether ");
					p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
					p8tx=StrCopy(p8tx," broadcast ");
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
					p8tx=StrCopy(p8tx," up ");
					p8tx[0]=0;
					p8tx=(char*)ADDR_DATABUFF;
					for(i=0;p8tx[i]!=0;i++)
					{
						if(p8tx[i]=='-')
						{
							for(x=i;p8tx[x]!=0;x++)//定义新的变量开始循环赋值 
							{
								p8tx[x]=p8tx[x+1];
								i--;
							}
						}
					}
					ms->link_system((char*)ADDR_DATABUFF);
				UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				return;
			}
		}	
	#endif
	#if MainProtocol==CSG
		i=0;//i=MRR(ADDR_AFN04F7+64,1);
		if(i==0x0)
		{//固定IP连接
			if(Local_ASCII_IP4())//填写本地ascii ip4,出口:0=没变化,1=变化
			{
				UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				return;
			}
		}
	#endif
	}

	switch(UARTCtrl->Task)
	{
		case 0://接收
			p16=(u16*)Get_ADDR_UARTn(ENETPORT);//得到UART缓冲首地址
			LEN_UARTnTx=Get_LEN_UARTnIntRx(ENETPORT);//得到UART中断接收缓冲长度
			LEN_UARTnTx-=10;
			p0=p16[0];
			if(p0>=LEN_UARTnTx)
			{
				p0=0;
			}
			x=LEN_UARTnTx-p0;//可接收长度
			if((UARTCtrl->TCPUDP&0x80)==0x0)
			{//TCP
				i=ms->link_recv(UARTCtrl->TCPSocket,(void *)((u32)p16+10+p0),x,MSG_DONTWAIT);
			}
			else
			{//UDP
				i=ms->link_recv(UARTCtrl->UDPSocket,(void *)((u32)p16+10+p0),x,MSG_DONTWAIT);
			}
			if(i<=LEN_UARTnTx)
			{
				p0+=i;
				if(p0>=LEN_UARTnTx)
				{
					p0=0;
				}
				p16[0]=p0;
			}
			
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
		#if MainProtocol==376
		case 5://等待多帧数据发送
			#if (USER/100)==11//吉林系
				USER11xx_TxAddr(ENETPORT);//吉林系发送帧终端地址2字节长度变为4字节,并重新计算帧校验和,需发送的帧字节数+2
			#endif
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30秒,等待发送数据给主站时间超时定时
			UARTCtrl->Task++;
		#endif
		#if MainProtocol==698
			LEN_UARTnTx=Get_LEN_UARTnTx(ENETPORT);
			p8tx=(char *)Get_ADDR_UARTnTx(ENETPORT);
			LEN_ADDR=p8tx[4];//取地址字节数
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[1]+(p8tx[2]<<8);//除起始字符和结束字符之外的帧字节数
			LEN_TAPDU-=3+LEN_ADDR+2+2;
			if(LEN_TAPDU<=LENmax_TxAPDU)//链路层分帧发送，每帧最大APDU字节数
			{
			#if LOG_698==1//0=没,1=有
				LOG(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
			#if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
				LOGRS232(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
			#endif
				UARTCtrl->TxCount=0;
				p16timer[0]=30000/10;//30秒,等待发送数据给主站时间超时定时
				UARTCtrl->Task++;
			}
			else
			{
				p8tx=(char *)Get_ADDR_UARTnTx(ENETPORT);
				MW((u32)p8tx,(u32)p8tx+LEN_UARTnTx,UARTCtrl->TxByte);
				UARTCtrl->TxAPDUSEQ=0;//链路层分帧序号计数0-4095
				UARTCtrl->TxAPDUByte=0;//链路层分帧已发送APDU字节数
				UARTCtrl->Task=5;//5=等待链路层分帧数据发送
			}
		#endif
			break;
	#if MainProtocol==698
		case 5://等待多帧数据发送
			LEN_UARTnTx=Get_LEN_UARTnTx(ENETPORT);
			p8tx=(char *)Get_ADDR_UARTnTx(ENETPORT);
			LEN_ADDR=p8tx[4];//取地址字节数
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[LEN_UARTnTx+1]+(p8tx[LEN_UARTnTx+2]<<8);//除起始字符和结束字符之外的帧字节数
			LEN_TAPDU-=3+LEN_ADDR+2+2;//长度域L和控制域C=3byte,帧头校验=2byte,帧校验=2byte
			MR((u32)p8tx,(u32)p8tx+LEN_UARTnTx,4+LEN_ADDR+2);
			p8tx[3]|=(1<<5);//分帧标志位
			LEN_Tx=4+LEN_ADDR+2;
			i=UARTCtrl->TxAPDUByte;
			if(i>=LEN_TAPDU)
			{
				UARTCtrl->RxByte=0;//rx计数=0
				UARTCtrl->Task=0;//0=接收
				return;
			}
			LEN_TxAPDU=LEN_TAPDU-i;
		//分帧格式域定义
		//bit0~bit11：表示分帧传输过程的帧序号，取值范围0~4095，循环使用；
		//bit12~bit13：保留；
		//bit15=0，bit14=0：表示分帧传输数据起始帧；
		//bit15=1，bit14=0：表示分帧传输确认帧（确认帧不包含APDU片段域）；
		//bit15=0，bit14=1：表示分帧传输最后帧；
		//bit15=1，bit14=1：表示分帧传输中间帧。
			x=UARTCtrl->TxAPDUSEQ;//链路层分帧序号计数0-4095
			UARTCtrl->TxAPDUSEQ++;//链路层分帧序号计数0-4095
			x&=0x7ff;
			if(UARTCtrl->TxAPDUByte==0)
			{//第1帧
				LEN_TxAPDU=LENmax_TxAPDU;
			}
			else
			{
				if(LEN_TxAPDU>LENmax_TxAPDU)//链路层分帧发送，每帧最大APDU字节数
				{//中间帧
					x|=0xC000;
					LEN_TxAPDU=LENmax_TxAPDU;
				}
				else
				{//最后帧
					x|=0x4000;
				}
			}
			p8tx[LEN_Tx]=x;
			p8tx[LEN_Tx+1]=x>>8;
			LEN_Tx+=2;
			MR((u32)p8tx+LEN_Tx,(u32)p8tx+LEN_UARTnTx+4+LEN_ADDR+2+UARTCtrl->TxAPDUByte,LEN_TxAPDU);
			UARTCtrl->TxAPDUByte+=LEN_TxAPDU;
			LEN_Tx+=LEN_TxAPDU;
			LEN_Tx+=2+1;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
			Uart_698_HCSFCS((u8*)p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		 #if LOG_68==1//0=没,1=有
			LOG(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		 #endif
		 #if DEBUG_698toRS232==1//0=否,1=是;调试:ENET口收发数据打印到RS232口
			LOGRS232(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		 #endif
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30秒,等待发送数据给主站时间超时定时
			UARTCtrl->Task++;
			break;
	#endif
		case 3://发送单帧数据正在发送
		case 6://多帧数据正在发送
			if(UARTCtrl->TxByte)
			{
				if((UARTCtrl->TCPUDP&0x80)==0x0)
				{//TCP
					x=2048;
					if(x>UARTCtrl->TxByte)
					{
						x=UARTCtrl->TxByte;
					}
					p8s=(u8*)Get_ADDR_UARTnTx(ENETPORT);
					p8s+=UARTCtrl->TxCount;
					i=ms->link_send(UARTCtrl->TCPSocket, (void *)p8s,x, MSG_DONTWAIT);
					if(i==x)
					{
						UARTCtrl->TxByte-=x;
						UARTCtrl->TxCount+=x;
						Terminal_Ram->ENETLED_MS_Timer=50/10;//14 以太网接收发送LED指示MS定时器,每10MS减1定时器
					}
					else
					{
						if(p16timer[0]==0)//60秒,等待发送数据给主站时间超时定时
						{
							UARTCtrl->LinkTask=0;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
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
					p8s=(u8*)Get_ADDR_UARTnTx(ENETPORT);
					p8s+=UARTCtrl->TxCount;
					UARTCtrl->TxByte-=x;
					UARTCtrl->TxCount+=x;
				#if MainProtocol==376
					MR((u32)&addr.sin_addr.s_addr,ADDR_AFN04F3,4);
					i=MRR(ADDR_AFN04F3+4,2);
					i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					addr.sin_port = i;//端口号:倒序	
				#endif
				#if MainProtocol==698
					i=(u32)Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						MR((u32)&addr.sin_addr.s_addr,i+2,4);
					}
					else
					{
						MWR(192+(168<<8)+(1<<16)+(3<<24),(u32)&addr.sin_addr.s_addr,4);
					}
					i=(u32)Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);
						addr.sin_port = i;//端口号:倒序	
					}
					else
					{
						i=1024;
						i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
						addr.sin_port = i;//端口号:倒序	
					}
				#endif
				#if MainProtocol==CSG
				/*
					MR((u32)&addr->addr[0],ADDR_AFN04F3,4);
					MR((u32)&addr->port,ADDR_AFN04F3+4,2);
				*/
				#endif
					ms->link_sendto(UARTCtrl->UDPSocket, (void *)p8s,x, MSG_DONTWAIT,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
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
				#if MainProtocol==376
					UARTCtrl->Task=4;//4=多帧数据续接收完成
				#else
					UARTCtrl->Task=7;//7=等待发送器空转为接收
				#endif
				}
			}
			break;
	}
#endif//#if (IC_LAN8720|IC_IP101)==0
}


#endif













