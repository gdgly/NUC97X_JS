
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../NUC97x/NUC97x_MAC.h"
#include "../../NUC97x/Driver/NUC97x_IP101GR.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"
#include "../OS/OS.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_UART.h"



#define TEST_ETH_BYTE     1472//def=1440,Max=1472

int32_t tcp_cb_client(int32_t socket, netTCP_Event event, NET_ADDR *addr,uint8_t *buf, uint32_t len)
{
	return (0);
}
uint32_t test_udp_callback(int32_t socket, const NET_ADDR *addr, const uint8_t *buf, uint32_t len)
{
  // This function is called when UDP data is received 
  // Process received data from 'buf' 
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	if(UARTCtrl->LinkSubTask==6)//链接子任务
	{
		//case 6://ETH死机检查等待接收
		for(i=0;i<TEST_ETH_BYTE;i++)
		{
			if(buf[i]!=(i&0xff))
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
  return (0);
}

u32 Test_ETH0(u32 ADDR_BASE)//ETH0 and IP101
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	u32 i;
	u32 reg;
	u8* p8;
	u16* p16timer;
	os_Type *os;
	NET_ADDR netaddr;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 ReNum;
	u32 TestResult_IP101;//总测试结果0=合格,1=不合格,0xff=无结论
	u32 TestResult_ETH;
	u32 socket;
	
	os=Get_os();
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""IP101");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	
	Init_NUC97x_MAC();
	Init_IP101();
	TestResult_IP101=0;//总测试结果0=合格,1=不合格,0xff=无结论
	for(i=0;i<500;i++)
	{
		while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
		{//MDC正忙
		}
		reg=Read_ETH_PHYReg(PHYAddr_IP101,2);
		if(reg==0x0243)//IP101GR
		{
			break;
		}
		SoftDelayMS(10);//软件ms延时
	}
	if(i>=500)
	{
		TestResult_IP101=1;//总测试结果0=合格,1=不合格,0xff=无结论
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
		MC(1,ADDR_BASE+OFFSET_LAN8720A_Characteristics,1);
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
		MC(0,ADDR_BASE+OFFSET_LAN8720A_Characteristics,1);
	}
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""Ethernet");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	TestResult_ETH=1;
	ReNum=2;
	while(ReNum&&(TestResult_ETH==1))
	{
		ReNum--;
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		for(i=0;i<100;i++)
		{
			while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
			{//MDC正忙
			}
			reg=Read_ETH_PHYReg(PHYAddr_IP101,1);
			if((reg&0x24)==0x24)//bit2 link is up,bit5 auto-negotiate process completed
			{//RJ45插入
				break;
			}
			SoftDelayMS(10);//软件ms延时
		}
		if(i>=100)
		{
			TestResult_ETH=1;//总测试结果0=合格,1=不合格,0xff=无结论
			continue;
		}

	//本机地址
		p8=(u8*)ADDR_IP4GATEWAY_ascii;
		p8[0]='1';
		p8[1]='9';
		p8[2]='2';
		p8[3]='.';
		p8[4]='1';
		p8[5]='6';
		p8[6]='8';
		p8[7]='.';
		p8[8]='0';
		p8[9]='.';
		p8[10]='1';
		p8[11]=0;
		p8=(u8*)ADDR_IP4ADDR_ascii;
		p8[0]='1';
		p8[1]='9';
		p8[2]='2';
		p8[3]='.';
		p8[4]='1';
		p8[5]='6';
		p8[6]='8';
		p8[7]='.';
		p8[8]='0';
		p8[9]='.';
		p8[10]='2';
		p8[11]=0;
		p8=(u8*)ADDR_IP4MASK_ascii;
		p8[0]='2';
		p8[1]='5';
		p8[2]='5';
		p8[3]='.';
		p8[4]='2';
		p8[5]='5';
		p8[6]='5';
		p8[7]='.';
		p8[8]='2';
		p8[9]='5';
		p8[10]='5';
		p8[11]='.';
		p8[12]='0';
		p8[13]=0;
		
		os->netInitialize();//每次重启OS后只能调用初始化1次,否则死机
		os->netifInitialize();//netif_init
		
		socket=os->netTCP_GetSocket((netTCP_cb_t)tcp_cb_client);
		os->netTCP_Connect(socket, &netaddr,1234);
		os->netTCP_Listen(socket, 1234);
		
		UARTCtrl->UDPSocket=os->netUDP_GetSocket((netUDP_cb_t)test_udp_callback);
		os->netUDP_Open(UARTCtrl->UDPSocket,1234);
		
		netaddr.addr_type=NET_ADDR_IP4;
	//	netaddr.addr[0]=192;
	//	netaddr.addr[1]=168;
	//	netaddr.addr[2]=0;
	//	netaddr.addr[3]=83;
	//	netaddr.port=1234;
		netaddr.addr[0]=255;
		netaddr.addr[1]=255;
		netaddr.addr[2]=255;
		netaddr.addr[3]=255;
		netaddr.port=1234;
		
		p8=(u8*)ADDR_DATABUFF;
		for(i=0;i<TEST_ETH_BYTE;i++)
		{
			p8[i]=i;
		}
		UARTCtrl->LinkSubTask=6;//链接子任务
		os->netUDP_Send(UARTCtrl->UDPSocket, (NET_ADDR *)&netaddr,p8,TEST_ETH_BYTE);
		p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
		p16timer[0]=1000/10;//超时定时
		while(1)
		{
			WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
			if(UARTCtrl->LinkSubTask==7)//链接子任务
			{
				TestResult_ETH=0;
				break;
			}
			if(p16timer[0]==0)
			{//接收超时
				TestResult_ETH=1;
				break;
			}
		}
	}
	if(TestResult_ETH==0)
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	MC(TestResult_ETH,ADDR_BASE+OFFSET_UART_Characteristics+3,1);
	return TestResult_IP101|TestResult_ETH;
#else
	return 0;
#endif
}

u32 Test_ETH1(u32 ADDR_BASE)//ETH1 and LAN8720
{
#if RJ45SELECT!=0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	u32 i;
	u32 reg;
	u8* p8;
	u16* p16timer;
	os_Type *os;
	NET_ADDR netaddr;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 ReNum;
	u32 TestResult_LAN8720;//总测试结果0=合格,1=不合格,0xff=无结论
	u32 TestResult_ETH;
	u32 socket;
	
	os=Get_os();
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""LAN8720");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	Init_NUC97x_MAC1();
	Init_LAN8720();
	TestResult_LAN8720=0;//总测试结果0=合格,1=不合格,0xff=无结论
	for(i=0;i<500;i++)
	{
		while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
		{//MDC正忙
		}
		reg=Read_ETH1_PHYReg(PHYAddr_LAN8720,2);
		if(reg==0x0243)//IP101GR
		{
			break;
		}
		SoftDelayMS(10);//软件ms延时
	}
	if(i>=100)
	{
		TestResult_LAN8720=1;//总测试结果0=合格,1=不合格,0xff=无结论
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
		MC(1,ADDR_BASE+OFFSET_LAN8720A_Characteristics,1);
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
		MC(0,ADDR_BASE+OFFSET_LAN8720A_Characteristics,1);
	}
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""Ethernet 1");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	TestResult_ETH=1;
	ReNum=2;
	while(ReNum&&(TestResult_ETH==1))
	{
		ReNum--;
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		for(i=0;i<100;i++)
		{
			while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
			{//MDC正忙
			}
			reg=Read_ETH1_PHYReg(PHYAddr_LAN8720,1);
			if((reg&0x24)==0x24)//bit2 link is up,bit5 auto-negotiate process completed
			{//RJ45插入
				break;
			}
			SoftDelayMS(10);//软件ms延时
		}
		if(i>=100)
		{
			TestResult_ETH=1;//总测试结果0=合格,1=不合格,0xff=无结论
			continue;
		}

	//本机地址
		p8=(u8*)ADDR_IP4GATEWAY_ascii;
		p8[0]='1';
		p8[1]='9';
		p8[2]='2';
		p8[3]='.';
		p8[4]='1';
		p8[5]='6';
		p8[6]='8';
		p8[7]='.';
		p8[8]='0';
		p8[9]='.';
		p8[10]='1';
		p8[11]=0;
		p8=(u8*)ADDR_IP4ADDR_ascii;
		p8[0]='1';
		p8[1]='9';
		p8[2]='2';
		p8[3]='.';
		p8[4]='1';
		p8[5]='6';
		p8[6]='8';
		p8[7]='.';
		p8[8]='0';
		p8[9]='.';
		p8[10]='2';
		p8[11]=0;
		p8=(u8*)ADDR_IP4MASK_ascii;
		p8[0]='2';
		p8[1]='5';
		p8[2]='5';
		p8[3]='.';
		p8[4]='2';
		p8[5]='5';
		p8[6]='5';
		p8[7]='.';
		p8[8]='2';
		p8[9]='5';
		p8[10]='5';
		p8[11]='.';
		p8[12]='0';
		p8[13]=0;
		
		os->netInitialize1();//每次重启OS后只能调用初始化1次,否则死机
		os->netifInitialize1();//netif_init
		
		socket=os->netTCP_GetSocket1((netTCP_cb_t)tcp_cb_client);
		os->netTCP_Connect1(socket, &netaddr,1234);
		os->netTCP_Listen1(socket, 1234);
		
		UARTCtrl->UDPSocket=os->netUDP_GetSocket1((netUDP_cb_t)test_udp_callback);
		os->netUDP_Open1(UARTCtrl->UDPSocket,1234);
		
		netaddr.addr_type=NET_ADDR_IP4;
	//	netaddr.addr[0]=192;
	//	netaddr.addr[1]=168;
	//	netaddr.addr[2]=0;
	//	netaddr.addr[3]=83;
	//	netaddr.port=1234;
		netaddr.addr[0]=255;
		netaddr.addr[1]=255;
		netaddr.addr[2]=255;
		netaddr.addr[3]=255;
		netaddr.port=1234;
		
		p8=(u8*)ADDR_DATABUFF;
		for(i=0;i<TEST_ETH_BYTE;i++)
		{
			p8[i]=i;
		}
		UARTCtrl->LinkSubTask=6;//链接子任务
		os->netUDP_Send1(UARTCtrl->UDPSocket, (NET_ADDR *)&netaddr,p8,TEST_ETH_BYTE);
		p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
		p16timer[0]=1000/10;//超时定时
		while(1)
		{
			WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
			if(UARTCtrl->LinkSubTask==7)//链接子任务
			{
				TestResult_ETH=0;
				break;
			}
			if(p16timer[0]==0)
			{//接收超时
				TestResult_ETH=1;
				break;
			}
		}
	}
	if(TestResult_ETH==0)
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	MC(TestResult_ETH,ADDR_BASE+OFFSET_UART_Characteristics+3,1);
	return TestResult_LAN8720|TestResult_ETH;
#else
	return 0;
#endif
}



u32 Test_ETH(u32 ADDR_BASE)//ETH and IP101,LAN8720
{
	u32 TestResult;
	TestResult=Test_ETH0(ADDR_BASE);//ETH0 and IP101
	TestResult|=Test_ETH1(ADDR_BASE);//ETH1 and LAN8720
	return TestResult;
}


/*
u32 Test_ETH(u32 ADDR_BASE)
{
#define RX_DESCRIPTOR_NUM     4
#define TX_DESCRIPTOR_NUM     4
#define PACKET_BUFFER_SIZE    2048
// Frame Descriptor's Owner bit
#define OWNERSHIP_EMAC 0x80000000  // 1 = EMAC
//#define OWNERSHIP_CPU 0x7fffffff  // 0 = CPU

// Rx Frame Descriptor Status
#define RXFD_RXGD    0x00100000  // Receiving Good Packet Received
#define RXFD_RTSAS   0x00800000  // RX Time Stamp Available 

// Tx Frame Descriptor's Control bits
#define TXFD_TTSEN    0x08    // Tx Time Stamp Enable
#define TXFD_INTEN    0x04    // Interrupt Enable
#define TXFD_CRCAPP   0x02    // Append CRC
#define TXFD_PADEN    0x01    // Padding Enable

// Tx Frame Descriptor Status
#define TXFD_TXCP    0x00080000  // Transmission Completion
#define TXFD_TTSAS   0x08000000  // TX Time Stamp Available
struct eth_descriptor
{
    unsigned int  status1;
    unsigned char *buf;
    unsigned int  status2;
    struct eth_descriptor *next;
};
#ifdef __ICCARM__
#pragma data_alignment=4
static struct eth_descriptor rx_desc[RX_DESCRIPTOR_NUM];
static struct eth_descriptor tx_desc[TX_DESCRIPTOR_NUM];
#else
static struct eth_descriptor rx_desc[RX_DESCRIPTOR_NUM] __attribute__ ((aligned(4)));
static struct eth_descriptor tx_desc[TX_DESCRIPTOR_NUM] __attribute__ ((aligned(4)));
#endif
//static struct eth_descriptor volatile *cur_tx_desc_ptr, *cur_rx_desc_ptr, *fin_tx_desc_ptr;
static u8 rx_buf[RX_DESCRIPTOR_NUM][PACKET_BUFFER_SIZE];
static u8 tx_buf[TX_DESCRIPTOR_NUM][PACKET_BUFFER_SIZE];
	
	u32 i;
	u32 reg;
	u8* p8;
//	u32* p32;
	os_Type *os;
	u32 ReNum;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论

	os=Get_os();
	Disable_os();//暂停os
	DisplayStringAutoUp(0,9,(u8*)"\x0""Ethernet");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
	ReNum=2;
	while(ReNum&&(TestResult==1))
	{
		ReNum--;
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		Init_NUC97x_MAC();
		Init_IP101();
		for(i=0;i<100;i++)
		{
			reg=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,2);
			if(reg==0x0243)//IP101GR
			{
				break;
			}
			SoftDelayMS(10);//软件ms延时
		}
		if(i>=100)
		{
			TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
			continue;
		}
		for(i=0;i<100;i++)
		{
			reg=Read_ETH_PHYReg(PHYAddr_IP101,1);
			if(reg&(1<<2))//bit2 link is up,bit5 auto-negotiate process completed
			{//RJ45插入
				break;
			}
			SoftDelayMS(10);//软件ms延时
		}
		if(i>=100)
		{
			TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
			continue;
		}
		os->netInitialize();//每次重启OS后只能调用初始化1次,否则死机
		os->netifInitialize();//netif_init
//EMACn_RXDLSA must be configured by software before the bit RXON (EMACn_MCMDR[0]) is enabled.
//EMACn_TXDLSA must be configured by software before the bit TXON (EMACn_MCMDR[8]) is enabled
		outpw(REG_EMAC0_MCMDR,inpw(REG_EMAC0_MCMDR)&0xfffffefe);
//ETH发送描述符表
    for(i = 0; i < TX_DESCRIPTOR_NUM; i++)
		{
			tx_desc[i].status1 = TXFD_PADEN | TXFD_CRCAPP;// | TXFD_INTEN;
			tx_desc[i].buf = (unsigned char *)((UINT)(&tx_buf[i][0]) | 0x80000000);
			tx_desc[i].status2 = 0;
			tx_desc[i].next = (struct eth_descriptor *)((UINT)(&tx_desc[(i + 1) % TX_DESCRIPTOR_NUM]) | 0x80000000);
		}
    outpw(REG_EMAC0_TXDLSA, (unsigned int)&tx_desc[0] | 0x80000000);
//ETH接收描述符表
    for(i = 0; i < RX_DESCRIPTOR_NUM; i++)
		{
			rx_desc[i].status1 = OWNERSHIP_EMAC;
			rx_desc[i].buf = (unsigned char *)((UINT)(&rx_buf[i][0]) | 0x80000000);
			rx_desc[i].status2 = 0;
			rx_desc[i].next = (struct eth_descriptor *)((UINT)(&rx_desc[(i + 1) % RX_DESCRIPTOR_NUM]) | 0x80000000);
		}
    outpw(REG_EMAC0_RXDLSA, (unsigned int)&rx_desc[0] | 0x80000000);
		//清接收
		for(i=0;i<2048;i++)
		{
			rx_buf[0][i]=0;
		}
		//写发送
	//IEEE 802.3-2002 标准规定，MAC 块使用 MAC 子层和可选 MAC 控制子层 (10/100 Mbit/s)
	//为使用 CSMA/CD MAC 的数据通信系统指定了两个帧格式:基本 MAC 帧格式,标记 MAC 帧格式（扩展了基本 MAC 帧格式）

	//7BYTE 报头0x55 0x55 0x55 0x55 0x55 0x55 0x55(硬件自动生成加入)
	//1BYTE SDF 0xD5(硬件自动生成加入)
	//6BYTE 目标地址(需相同设置的MAC地址)
		MW(ADDR_ETHMAC,(u32)&tx_buf,6);
	//6BYTE 源地址(用于测试时可任意地址)
		MC(0xff,((u32)&tx_buf)+6,6);
	//4BYTE QTag前辍(带标记的MAC帧)
		//2BYTE 长度/类型=802.1QTagType
		//2BYTE 标记控制信息
	//2BYTE MAC客户端长度/类型:<=1500表示 802.3 帧的后续数据字段中所包含的 MAC 客户端数据字节的数量（长度解析）;>0x6000则该字段表示与以太网帧相关的 MAC 客户端协议的种类（类型解析）,=0x8100 802.1Q变量协议类型
		tx_buf[0][12]=1024>>8;//高字节
		tx_buf[0][13]=1024&0xff;//低字节
	//46-1500BYTE 数据和 PAD 字段
		for(i=0;i<1024;i++)
		{
			tx_buf[0][14+i]=i;
		}
	//4BYTE CRC (CRC 计算基于下列字段：源地址、目标地址、QTag 前缀、长度/类型、LLC 数据和 PAD（即，除报头、SFD 字段以外的所有字段）
		
//设置长度Set length of the packet to TXDES2[15:0]
		tx_desc[0].status2=1024+14;
//Set TXDES0[31] to 1
		tx_desc[0].status1|=0x80000000;
//允许TX,RX
		outpw(REG_EMAC0_MCMDR,inpw(REG_EMAC0_MCMDR)&0x101);
//Write EMAC_RSDR
		outpw(REG_EMAC0_RSDR,0);
//填寫任意值進EMAC_TSDR 寄存器, 即可起始傳送狀態機, 開始傳送封包
		outpw(REG_EMAC0_TSDR,0);
//接收
		Comm_Ram->GPMSTimer=1000/10;
		while(1)
		{
			if((rx_desc[0].status1&0xffff)>=(1024+14))
			{
				p8=(u8*)(&rx_buf+14);
				for(i=0;i<1024;i++)
				{
					if(p8[i]!=(i&0xff))
					{
						break;
					}
				}
				if(i>=1024)
				{
					TestResult=0;//OK
					break;
				}
			}
			if(Comm_Ram->GPMSTimer==0)
			{
				break;
			}
		}
		
	}
	outpw(REG_EMAC0_MCMDR,inpw(REG_EMAC0_MCMDR)&0xfffffefe);
	Enable_os();//允许os
	if(TestResult==0)
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	MC(TestResult,ADDR_BASE+OFFSET_UART_Characteristics+3,1);
	return TestResult;
}
*/










