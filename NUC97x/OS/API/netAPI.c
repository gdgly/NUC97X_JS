
#if MainProtocol==376
#include "../../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../../NUc97x/OS/API/netAPI.h"
#include "../../../NUc97x/ETH1/net1API.h"

#include "netif/ethernetif.h"
#include "netif/etharp.h"
#include "lwip/init.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/stats.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/timers.h"
#include "lwip/dhcp.h"

#include "FreeRTOS.h"
#include "task.h"

//#include "../../../STM32F4xx/Calculate/Calculate.h"



extern unsigned char my_mac_addr0[];
extern unsigned char my_mac_addr1[];
extern struct ip_addr gw, ipaddr, netmask;
extern struct netif netif;
unsigned char net0InitializeFlag=0;//net0初始化标志:0=没,1=初始化

eth1_Type* Get_eth1(void)
{
	u32* p32;
	
	p32=(u32*)(ADDR_OS_ENTRY+(4*150));
	return (eth1_Type*)p32[130];
}

u32 bcd_hex(u32 bcd)//8位压缩BCD数转为HEX
{
	u8 i;
	u32 hex;
	hex=0;
	for(i=7;i!=0x0;i--)
	{
		hex+=(bcd>>28)&0xf;
		hex*=10;
		bcd<<=4;
	}
	hex+=(bcd>>28)&0xf;
	return(hex);
}
u8* IP_ASCII_START(u8* pAddr)//找到ip有效字符开始地址,若在256字符内没找到则返回0
{
	u32 i;
	for(i=0;i<256;i++)
	{
		if(((pAddr[0]>='0')&&(pAddr[0]<='9'))||((pAddr[0]>='A')&&(pAddr[0]<='F'))||((pAddr[0]>='a')&&(pAddr[0]<='f')))
		{
			break;
		}
		pAddr++;
	}
	if(i>255)
	{
		return 0;
	}	
	return (pAddr);
}
u8* IP_ASCII_END(u8* pAddr)//找到ip有效字符结束地址,若在256字符内没找到则返回0
{
	u32 i;
	for(i=0;i<256;i++)
	{
		if (!(((pAddr[0]>='0')&&(pAddr[0]<='9'))||((pAddr[0]>='A')&&(pAddr[0]<='F'))||((pAddr[0]>='a')&&(pAddr[0]<='f'))))
		{
			break;
		}
		pAddr++;
	}
	if(i>255)
	{
		return 0;
	}	
	return (pAddr);
}

u32 ASCII_HEX(u8* pASCII)//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
{
	u32 x;

	x=0;
	while(1)
	{
		if((pASCII[0]>='0')&&(pASCII[0]<='9'))
		{
			x<<=4;
			x|=pASCII[0]-'0';
			pASCII++;
			continue;
		}
		else
		{
			if((pASCII[0]>='a')&&(pASCII[0]<='f'))
			{
				x<<=4;
				x|=(pASCII[0]-'a')+10;
				pASCII++;
				continue;
			}
			else
			{
				if((pASCII[0]>='A')&&(pASCII[0]<='F'))
				{
					x<<=4;
					x|=(pASCII[0]-'A')+10;
					pASCII++;
					continue;
				}
				else
				{
					if((pASCII[0]==' ')||(pASCII[0]=='-'))
					{//有效数据开始前的空格
						pASCII++;
						continue;
					}
				}
			}
		}
		break;
	}
	x=bcd_hex(x);
	return x;
}
u32 IP_ASCII_HEX(u8* Addr)//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
{
	u32 ip;

	ip=0;
	//找到第1个ASCII数字0-9的地址
	Addr=IP_ASCII_START(Addr);//找到ip有效字符开始地址,若在256字符内没找到则返回0
	if(Addr==0)
	{
		return 0;
	}
	ip=ASCII_HEX(Addr);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
	Addr=IP_ASCII_END(Addr);//找到ip有效字符结束地址,若在256字符内没找到则返回0
	//找到第2个ASCII数字0-9的地址
	Addr=IP_ASCII_START(Addr);//找到ip有效字符开始地址,若在256字符内没找到则返回0
	if(Addr==0)
	{
		return 0;
	}
	ip+=ASCII_HEX(Addr)<<8;//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
	Addr=IP_ASCII_END(Addr);//找到ip有效字符结束地址,若在256字符内没找到则返回0
	//找到第3个ASCII数字0-9的地址
	Addr=IP_ASCII_START(Addr);//找到ip有效字符开始地址,若在256字符内没找到则返回0
	if(Addr==0)
	{
		return 0;
	}
	ip+=ASCII_HEX(Addr)<<16;//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
	Addr=IP_ASCII_END(Addr);//找到ip有效字符结束地址,若在256字符内没找到则返回0
	//找到第4个ASCII数字0-9的地址
	Addr=IP_ASCII_START(Addr);//找到ip有效字符开始地址,若在256字符内没找到则返回0
	if(Addr==0)
	{
		return 0;
	}
	ip+=ASCII_HEX(Addr)<<24;//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
	return ip;
}

void LwipTask( void *pvParameters )//Lwip线程
{
	while (1)
	{
		if(net0InitializeFlag==1)//net0初始化标志:0=没,1=初始化
		{
			sys_check_timeouts();  // All network traffic is handled in interrupt handler
		}
		taskYIELD();//强制任务切换
	}
}






typedef struct
{
	u32 Count;
	u32 Callback_fn[MEMP_NUM_TCP_PCB];
	u32 pPCB[MEMP_NUM_TCP_PCB];
	NET_ADDR NETADDR[MEMP_NUM_TCP_PCB];
}TCP_Callback_Lable;
typedef struct
{
	u32 Count;
	u32 Callback_fn[MEMP_NUM_UDP_PCB];
	u32 pPCB[MEMP_NUM_UDP_PCB];
	NET_ADDR NETADDR[MEMP_NUM_UDP_PCB];
}UDP_Callback_Lable;

TCP_Callback_Lable tcp_callback_lable;
UDP_Callback_Lable udp_callback_lable;




netStatus netInitialize0(void)//eth0
{
	u32 i;
	u32 ip;
	u8* p8;
	
	

//清回调表
	tcp_callback_lable.Count=0;
	udp_callback_lable.Count=0;

//本机MAC地址
	p8=(u8*)ADDR_ETHMAC;
	for(i=0;i<6;i++)
	{
		my_mac_addr0[i]=p8[i];
	}
//本机地址
	ip=IP_ASCII_HEX((u8*)ADDR_IP4GATEWAY_ascii);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
	if(ip==0)
	{
		IP4_ADDR(&gw, 192, 168, 0, 1);
	}
	else
	{
		IP4_ADDR(&gw, ip&0xff, (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
	}
	ip=IP_ASCII_HEX((u8*)ADDR_IP4ADDR_ascii);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
	if(ip==0)
	{
		IP4_ADDR(&ipaddr, 192, 168, 0, 2);
	}
	else
	{
		IP4_ADDR(&ipaddr, ip&0xff, (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
	}
	ip=IP_ASCII_HEX((u8*)ADDR_IP4MASK_ascii);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
	if(ip==0)
	{
		IP4_ADDR(&netmask, 255, 255, 255, 0);
	}
	else
	{
		IP4_ADDR(&netmask, ip&0xff, (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
	}
	
	return netOK;
}
netStatus netInitialize1(void)//eth1
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netInitialize();
#endif
}
netStatus netInitialize(void)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netInitialize0();//eth0
#else
	return netInitialize1();//eth1
#endif
}



extern struct netif *netif_list;
extern struct netif *netif_default;
extern u8_t netif_num;
extern struct sys_timeo *next_timeout;

void netifInitialize0(void)//netif_init
{
	u32* p32;
	
	sysDisableInterrupt(TMR0_IRQn);//禁止TIM0中断
	//复位 EMAC0
	p32=(u32*)(REG_SYS_AHBIPRST);
	p32[0]=1<<16;
	p32[0]=0;

	netif_list=NULL;
	netif_default=NULL;
	netif_num=0;
	next_timeout=NULL;
	lwip_init();

	netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init0, ethernet_input);
	netif_set_default(&netif);
	netif_set_up(&netif);

// LwIP needs a timer @ 100Hz. To use another timer source, please modify sys_now() in sys_arch.c as well
	sysStartTimer(TIMER0, 100, PERIODIC_MODE);
	sysEnableInterrupt(TMR0_IRQn);//允许TIM0中断
	net0InitializeFlag=1;//net0初始化标志:0=没,1=初始化
}
void netifInitialize1(void)//netif_init
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	eth1->netifInitialize();
#endif
}
void netifInitialize(void)//netif_init
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	netifInitialize0();//netif_init
#else
	netifInitialize1();//netif_init
#endif
}



//---TCP API---
int32_t netTCP_GetSocket0(netTCP_cb_t cb_func)
{
	u32 i;
	eth1_Type* eth1;
	eth1=Get_eth1();
	
	i=tcp_callback_lable.Count;
	if(i>=MEMP_NUM_TCP_PCB)
	{
		return -1;
	}
	tcp_callback_lable.Callback_fn[i]=(u32)cb_func;
	i++;
	tcp_callback_lable.Count=i;
	return i;
}
int32_t netTCP_GetSocket1(netTCP_cb_t cb_func)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return 0;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_GetSocket(cb_func );
#endif
}
int32_t netTCP_GetSocket(netTCP_cb_t cb_func)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_GetSocket0(cb_func);
#else
	return netTCP_GetSocket1(cb_func);
#endif
}

netStatus netTCP_ReleaseSocket0(int32_t socket)
{
	u32 i;
	u32 x;
	struct tcp_pcb *pcb;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_TCP_PCB)||(i>tcp_callback_lable.Count))
	{
		return netError;
	}
	i--;
	pcb=(struct tcp_pcb*)tcp_callback_lable.pPCB[i];
	tcp_abort(pcb);//终止连接,删除pcb控制块
//	tcp_arg(pcb,NULL);
//	tcp_recv(pcb,NULL);
//	tcp_sent(pcb,NULL);
//	tcp_err(pcb,NULL);
//	tcp_poll(pcb,NULL,0);
	
	tcp_callback_lable.Count--;
	for(;i<tcp_callback_lable.Count;i++)
	{
		tcp_callback_lable.Callback_fn[i]=tcp_callback_lable.Callback_fn[i+1];
		tcp_callback_lable.NETADDR[i].addr_type=tcp_callback_lable.NETADDR[i+1].addr_type;
		tcp_callback_lable.NETADDR[i].port=tcp_callback_lable.NETADDR[i+1].port;
		for(x=0;x<NET_ADDR_IP6_LEN;x++)
		{
			tcp_callback_lable.NETADDR[i].addr[x]=tcp_callback_lable.NETADDR[i+1].addr[x];
		}
	}
	return netOK;
}
netStatus netTCP_ReleaseSocket1(int32_t socket)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_ReleaseSocket(socket);
#endif
}
netStatus netTCP_ReleaseSocket(int32_t socket)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_ReleaseSocket0(socket);
#else
	return netTCP_ReleaseSocket1(socket);
#endif
}

err_t os_tcp_Client_recv0(void *arg, struct tcp_pcb *pcb,struct pbuf *p, err_t err)
{
	u32 socket;
	u32 Callback_fn;
//	u32 MaxByte;
	
	for(socket=0;socket<tcp_callback_lable.Count;socket++)
	{
		if(tcp_callback_lable.pPCB[socket]==(u32)pcb)
		{
			Callback_fn=tcp_callback_lable.Callback_fn[socket];
			if(p==NULL)
			{
				//主站服务器主动断开连接
				(*(void(*)())Callback_fn)((int32_t)socket+1,(netTCP_Event)netTCP_EventClosed,(NET_ADDR *)&tcp_callback_lable.NETADDR[socket],(uint8_t *)0,(uint32_t)0);
				return ERR_OK;
			}
			else
			{
			//---TEST---
//				MaxByte=netTCP_GetMaxSegmentSize(socket+1);
//				netTCP_Send(socket+1, p->payload, MaxByte);
//				//tcp_write(pcb,p->payload,p->len,TCP_WRITE_FLAG_COPY);//tcp_write(struct tcp_pcb *pcb, const void *dataptr, u16_t len,u8_t apiflags);
//				MaxByte=netTCP_GetMaxSegmentSize(socket+1);
//				if(MaxByte)
//				{
//					MaxByte=MaxByte;
//				}
			//---TEST END---
				//回调接收数据事件
				(*(void(*)())Callback_fn)((int32_t)socket+1,(netTCP_Event)netTCP_EventData,(NET_ADDR *)&tcp_callback_lable.NETADDR[socket],(uint8_t *)p->payload,(uint32_t)p->len);
				tcp_recved(pcb,p->len);//用于获取接收数据的长度,表示可以获取更多的数据 
				pbuf_free(p);//释放内存
				return ERR_OK;
			}
		}
	}
	return ERR_OK;
}

err_t os_tcp_server_recv0(void *arg, struct tcp_pcb *pcb,struct pbuf *p, err_t err)
{
	u32 socket;
	u32 Callback_fn;
	u32 MaxByte;
	
	for(socket=0;socket<tcp_callback_lable.Count;socket++)
	{
		if(tcp_callback_lable.pPCB[socket]==(u32)pcb)
		{
			Callback_fn=tcp_callback_lable.Callback_fn[socket];
			if(p==NULL)
			{
				//主站服务器主动断开连接
				(*(void(*)())Callback_fn)((int32_t)socket+1,(netTCP_Event)netTCP_EventClosed,(NET_ADDR *)&tcp_callback_lable.NETADDR[socket],(uint8_t *)0,(uint32_t)0);
				return ERR_OK;
			}
			else
			{
			//---TEST---
//				MaxByte=netTCP_GetMaxSegmentSize(socket+1);
//				netTCP_Send(socket+1, p->payload, MaxByte);
//				//tcp_write(pcb,p->payload,p->len,TCP_WRITE_FLAG_COPY);//tcp_write(struct tcp_pcb *pcb, const void *dataptr, u16_t len,u8_t apiflags);
//				MaxByte=netTCP_GetMaxSegmentSize(socket+1);
//				if(MaxByte)
//				{
//					MaxByte=MaxByte;
//				}
			//---TEST END---
				//回调接收数据事件
				(*(void(*)())Callback_fn)((int32_t)socket+1,(netTCP_Event)netTCP_EventData,(NET_ADDR *)&tcp_callback_lable.NETADDR[socket],(uint8_t *)p->payload,(uint32_t)p->len);
				tcp_recved(pcb,p->len);//用于获取接收数据的长度,表示可以获取更多的数据 
				pbuf_free(p);//释放内存
				return ERR_OK;
			}
		}
	}
	return ERR_OK;
}

err_t os_tcp_Client_connected0(void *arg, struct tcp_pcb *pcb, err_t err)
{
	u32 i;
	u32 Callback_fn;
	
	if(err!=ERR_OK)
	{
		return ERR_OK;
	}
	for(i=0;i<tcp_callback_lable.Count;i++)
	{
		if(tcp_callback_lable.pPCB[i]==(u32)pcb)
		{
			//回调连接事件
			Callback_fn=tcp_callback_lable.Callback_fn[i];
			(*(void(*)())Callback_fn)((int32_t)i+1,(netTCP_Event)netTCP_EventEstablished,(NET_ADDR *)&tcp_callback_lable.NETADDR[i],(uint8_t *)0,(uint32_t)0);
			break;
		}
	}
	tcp_recv(pcb,os_tcp_Client_recv0);//指定接收到数据后的回调函数
	return ERR_OK;
}

err_t os_tcp_server_acpt0(void *arg,struct tcp_pcb *pcb, err_t err)
{
	u32 i;
	u32 Callback_fn;
	
	if(err!=ERR_OK)
	{
		return ERR_OK;
	}
	for(i=0;i<tcp_callback_lable.Count;i++)
	{
		if(tcp_callback_lable.pPCB[i]==(u32)pcb)
		{
			//客户端IP地址
			tcp_callback_lable.NETADDR[i].addr[0]=pcb->remote_ip.addr>>24;
			tcp_callback_lable.NETADDR[i].addr[1]=pcb->remote_ip.addr>>16;
			tcp_callback_lable.NETADDR[i].addr[2]=pcb->remote_ip.addr>>8;
			tcp_callback_lable.NETADDR[i].addr[3]=pcb->remote_ip.addr>>0;
			//回调连接事件
			Callback_fn=tcp_callback_lable.Callback_fn[i];
			(*(void(*)())Callback_fn)((int32_t)i+1,(netTCP_Event)netTCP_EventConnect,(NET_ADDR *)&tcp_callback_lable.NETADDR[i],(uint8_t *)0,(uint32_t)0);
			break;
		}
	}
	tcp_recv(pcb, os_tcp_server_recv0);//指定接收到数据后的回调函数
	return ERR_OK;
}

netStatus netTCP_Listen0(int32_t socket, uint16_t port)
{
	u32 i;
	err_t err;
	struct tcp_pcb *pcb, *pcb_listen;

	i=socket;
	if((i==0)||(i>MEMP_NUM_TCP_PCB)||(i>tcp_callback_lable.Count))
	{
		return netError;
	}
	socket--;
//Create a new TCP PCB
	pcb=tcp_new();
	if(pcb==NULL)
	{
		return netError;
	}
	//Bind the PCB to TCP port
	if(tcp_bind(pcb,IP_ADDR_ANY,port) != ERR_OK)
	{
		tcp_abort(pcb);//删除pcb控制块
		return netError;
	}
	tcp_callback_lable.pPCB[socket]=(u32)pcb;
//客户端IP地址
	tcp_callback_lable.NETADDR[socket].addr_type=NET_ADDR_IP4;
	tcp_callback_lable.NETADDR[socket].port=port;
	tcp_callback_lable.NETADDR[socket].addr[0]=0;
	tcp_callback_lable.NETADDR[socket].addr[1]=0;
	tcp_callback_lable.NETADDR[socket].addr[2]=0;
	tcp_callback_lable.NETADDR[socket].addr[3]=0;
	//Change TCP state to LISTEN
	pcb_listen = tcp_listen(pcb);
	//Setup http_accet() function to be called 	when a new connection arrives.
	tcp_accept(pcb_listen, os_tcp_server_acpt0);
	return netOK;
}
netStatus netTCP_Listen1(int32_t socket, uint16_t port)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_Listen(socket, port);
#endif
}
netStatus netTCP_Listen(int32_t socket, uint16_t port)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_Listen0(socket, port);
#else
	return netTCP_Listen1(socket, port);
#endif
}

netStatus netTCP_Connect0(int32_t socket, const NET_ADDR *addr, uint16_t local_port)
{
	u32 i;
	err_t err;
	struct tcp_pcb *pcb;
	struct ip_addr ip4_addr;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_TCP_PCB)||(i>tcp_callback_lable.Count))
	{
		return netError;
	}
	socket--;
	
//远程服务器IP
	IP4_ADDR(&ip4_addr,addr->addr[0],addr->addr[1],addr->addr[2],addr->addr[3]);//远程服务器IP 写进全局变量
//Create a new TCP PCB
	pcb=tcp_new();
	if(pcb==NULL)
	{
		return netError;
	}
//Bind the PCB to TCP port
	if(tcp_bind(pcb,IP_ADDR_ANY,local_port) != ERR_OK)
	{
		tcp_abort(pcb);//删除pcb控制块
		return netError;
	}
	tcp_callback_lable.pPCB[socket]=(u32)pcb;
	tcp_callback_lable.NETADDR[socket].addr_type=addr->addr_type;
	tcp_callback_lable.NETADDR[socket].port=addr->port;
	for(i=0;i<NET_ADDR_IP6_LEN;i++)
	{
		tcp_callback_lable.NETADDR[socket].addr[i]=addr->addr[i];
	}
	err=tcp_connect(pcb,&ip4_addr,addr->port,os_tcp_Client_connected0);	
	if(err!=ERR_OK)
	{
		tcp_abort(pcb);//删除pcb控制块
		return netError;
	}
	
	return netOK;
}
netStatus netTCP_Connect1(int32_t socket, const NET_ADDR *addr, uint16_t local_port)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_Connect(socket,addr, local_port);
#endif
}
netStatus netTCP_Connect(int32_t socket, const NET_ADDR *addr, uint16_t local_port)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_Connect0(socket,addr,local_port);
#else
	return netTCP_Connect1(socket,addr,local_port);
#endif
}

netStatus netTCP_Close0(int32_t socket)
{
	u32 i;
	struct tcp_pcb *pcb;
	err_t err;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_TCP_PCB)||(i>tcp_callback_lable.Count))
	{
		return netError;
	}
	socket--;
	pcb=(struct tcp_pcb*)tcp_callback_lable.pPCB[socket];
	err=tcp_close(pcb);
	
//	tcp_abort(pcb);//删除pcb控制块
	if(err!=ERR_OK)
	{
		return netError;
	}
	return netOK;
}
netStatus netTCP_Close1(int32_t socket)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_Close(socket);
#endif
}
netStatus netTCP_Close(int32_t socket)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_Close0(socket);
#else
	return netTCP_Close1(socket);
#endif
}

netStatus netTCP_Abort0(int32_t socket)
{
	u32 i;
	struct tcp_pcb *pcb;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_TCP_PCB)||(i>tcp_callback_lable.Count))
	{
		return netError;
	}
	socket--;
	pcb=(struct tcp_pcb*)tcp_callback_lable.pPCB[socket];
	tcp_abort(pcb);//删除pcb控制块
	return netOK;
}
netStatus netTCP_Abort1(int32_t socket)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_Abort(socket);
#endif
}
netStatus netTCP_Abort(int32_t socket)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_Abort0(socket);
#else
	return netTCP_Abort1(socket);
#endif
}

uint32_t netTCP_GetMaxSegmentSize0(int32_t socket)
{
	u32 i;
	struct tcp_pcb *pcb;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_TCP_PCB)||(i>tcp_callback_lable.Count))
	{
		return 0;
	}
	socket--;
	pcb=(struct tcp_pcb*)tcp_callback_lable.pPCB[socket];
	i=tcp_sndbuf(pcb);
	return i;
}
uint32_t netTCP_GetMaxSegmentSize1(int32_t socket)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return 0;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_GetMaxSegmentSize(socket);
#endif
}
uint32_t netTCP_GetMaxSegmentSize(int32_t socket)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_GetMaxSegmentSize0(socket);
#else
	return netTCP_GetMaxSegmentSize1(socket);
#endif
}

bool netTCP_SendReady0(int32_t socket)
{
	if(netTCP_GetMaxSegmentSize(socket))
	{
		return 1;
	}
	return 0;
}
bool netTCP_SendReady1(int32_t socket)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return 0;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_SendReady(socket);
#endif
}
bool netTCP_SendReady(int32_t socket)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_SendReady0(socket);
#else
	return netTCP_SendReady1(socket);
#endif
}

netStatus netTCP_Send0(int32_t socket, uint8_t *buf, uint32_t len)
{
	u32 i;
	struct tcp_pcb *pcb;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_TCP_PCB)||(i>tcp_callback_lable.Count))
	{
		return netError;
	}
	socket--;
	pcb=(struct tcp_pcb*)tcp_callback_lable.pPCB[socket];
	
	//tcp_write(struct tcp_pcb *pcb, const void *dataptr, u16_t len,u8_t apiflags);
	tcp_write(pcb,buf,len,TCP_WRITE_FLAG_COPY);
	return netOK;
}
netStatus netTCP_Send1(int32_t socket, uint8_t *buf, uint32_t len)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netTCP_Send(socket, buf, len);
#endif
}
netStatus netTCP_Send(int32_t socket, uint8_t *buf, uint32_t len)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netTCP_Send0(socket, buf, len);
#else
	return netTCP_Send1(socket, buf, len);
#endif
}
//---TCP API END---

//---UDP API---
int32_t netUDP_GetSocket0(netUDP_cb_t cb_func)
{
	u32 i;
	
	i=udp_callback_lable.Count;
	if(i>=MEMP_NUM_UDP_PCB)
	{
		return -1;
	}
	udp_callback_lable.Callback_fn[i]=(u32)cb_func;
	i++;
	udp_callback_lable.Count=i;
	return i;
}
int32_t netUDP_GetSocket1(netUDP_cb_t cb_func)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return 0;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netUDP_GetSocket(cb_func);
#endif
}
int32_t netUDP_GetSocket(netUDP_cb_t cb_func)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netUDP_GetSocket0(cb_func);
#else
	return netUDP_GetSocket1(cb_func);
#endif
}

netStatus netUDP_ReleaseSocket0(int32_t socket)
{
	u32 i;
	u32 x;
	struct udp_pcb *pcb;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_UDP_PCB)||(i>udp_callback_lable.Count))
	{
		return netError;
	}
	i--;
	pcb=(struct udp_pcb*)udp_callback_lable.pPCB[i];
	udp_remove(pcb);//删除pcb控制块
	
	udp_callback_lable.Count--;
	for(;i<udp_callback_lable.Count;i++)
	{
		udp_callback_lable.Callback_fn[i]=udp_callback_lable.Callback_fn[i+1];
		udp_callback_lable.NETADDR[i].addr_type=udp_callback_lable.NETADDR[i+1].addr_type;
		udp_callback_lable.NETADDR[i].port=udp_callback_lable.NETADDR[i+1].port;
		for(x=0;x<NET_ADDR_IP6_LEN;x++)
		{
			udp_callback_lable.NETADDR[i].addr[x]=udp_callback_lable.NETADDR[i+1].addr[x];
		}
	}
	return netOK;
}
netStatus netUDP_ReleaseSocket1(int32_t socket)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netUDP_ReleaseSocket(socket);
#endif
}
netStatus netUDP_ReleaseSocket(int32_t socket)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netUDP_ReleaseSocket0(socket);
#else
	return netUDP_ReleaseSocket1(socket);
#endif
}

void os_udp_recv0(void *arg, struct udp_pcb *pcb, struct pbuf *p,ip_addr_t *addr, u16_t port)
{
	u32 socket;
	u32 Callback_fn;
	NET_ADDR NETADDR;
//	u32 MaxByte;
	
	for(socket=0;socket<udp_callback_lable.Count;socket++)
	{
		if(udp_callback_lable.pPCB[socket]==(u32)pcb)
		{
			Callback_fn=udp_callback_lable.Callback_fn[socket];
			if(p==NULL)
			{
				return;
			}
			else
			{
				NETADDR.addr_type=NET_ADDR_IP4;
				NETADDR.port=port;
				NETADDR.addr[0]=addr->addr>>24;
				NETADDR.addr[1]=addr->addr>>16;
				NETADDR.addr[2]=addr->addr>>8;
				NETADDR.addr[3]=addr->addr>>0;
			//---TEST---
				
//				netUDP_Send(socket+1, (NET_ADDR *)&NETADDR, p->payload, p->len);

			//---TEST END---
				//回调接收数据事件
				(*(void(*)())Callback_fn)((int32_t)socket+1,(NET_ADDR *)&NETADDR,(uint8_t *)p->payload,(uint32_t)p->len);
				//tcp_recved(pcb,p->len);//用于获取接收数据的长度,表示可以获取更多的数据 
				pbuf_free(p);//释放内存
				return;
			}
		}
	}
}

netStatus netUDP_Open0(int32_t socket, uint16_t port)
{
	u32 i;
	err_t err;
	struct udp_pcb *pcb;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_UDP_PCB)||(i>udp_callback_lable.Count))
	{
		return netError;
	}
	socket--;
//Create a new UDP PCB
	pcb=udp_new();
	if(pcb==NULL)
	{
		return netError;
	}
	
//Bind the PCB to UDP port
	err=udp_bind(pcb,IP_ADDR_ANY,port);
	if(err != ERR_OK)
	{
		udp_remove(pcb);//删除pcb控制块
		return netError;
	}
	
	udp_callback_lable.pPCB[socket]=(u32)pcb;
	udp_callback_lable.NETADDR[socket].port=port;
	udp_recv(pcb,os_udp_recv0,NULL);
	return netOK;
}
netStatus netUDP_Open1(int32_t socket, uint16_t port)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netUDP_Open(socket, port);
#endif
}
netStatus netUDP_Open(int32_t socket, uint16_t port)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netUDP_Open0(socket, port);
#else
	return netUDP_Open1(socket, port);
#endif
}

netStatus netUDP_Close0(int32_t socket)
{
	u32 i;
	err_t err;
	struct udp_pcb *pcb;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_UDP_PCB)||(i>udp_callback_lable.Count))
	{
		return netError;
	}
	socket--;
	pcb=(struct udp_pcb*)udp_callback_lable.pPCB[socket];
	udp_disconnect (pcb);
	
//	udp_remove(pcb);//删除pcb控制块
	return netOK;
}
netStatus netUDP_Close1(int32_t socket)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netUDP_Close(socket);
#endif
}
netStatus netUDP_Close(int32_t socket)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netUDP_Close0(socket);
#else
	return netUDP_Close1(socket);
#endif
}

netStatus netUDP_Send0(int32_t socket, const NET_ADDR *addr, uint8_t *buf, uint32_t len)
{
	u32 i;
	err_t err;
	struct udp_pcb *pcb;
	struct pbuf *p;
	ip_addr_t dst_ip;
	
	i=socket;
	if((i==0)||(i>MEMP_NUM_UDP_PCB)||(i>udp_callback_lable.Count))
	{
		return netError;
	}
	socket--;
	pcb=(struct udp_pcb*)udp_callback_lable.pPCB[socket];
//	udp_callback_lable.NETADDR[socket].port=addr->port;
//	udp_callback_lable.NETADDR[socket].addr_type=addr->addr_type;
//	udp_callback_lable.NETADDR[socket].addr[0]=addr->addr[0];
//	udp_callback_lable.NETADDR[socket].addr[1]=addr->addr[1];
//	udp_callback_lable.NETADDR[socket].addr[2]=addr->addr[2];
//	udp_callback_lable.NETADDR[socket].addr[3]=addr->addr[3];
	dst_ip.addr=(addr->addr[3]<<24)|(addr->addr[2]<<16)|(addr->addr[1]<<8)|(addr->addr[0]<<0);
	
//	err=udp_connect(pcb,&dst_ip,addr->port);
//	if(err != ERR_OK)
//	{
//		return netError;
//	}

	p=pbuf_alloc(PBUF_TRANSPORT,len,PBUF_RAM);//struct pbuf *pbuf_alloc(pbuf_layer l, u16_t length, pbuf_type type);
	if(p==NULL)
	{
		return netError;
	}
	p->payload=buf;
	p->tot_len=len;
	p->len=len;
	err=udp_sendto(pcb,p,&dst_ip,addr->port);
//	err=udp_send(pcb,p);
//	udp_disconnect(pcb);
	
	pbuf_free(p);
	if(err != ERR_OK)
	{
		return netError;
	}
	return netOK;
}
netStatus netUDP_Send1(int32_t socket, const NET_ADDR *addr, uint8_t *buf, uint32_t len)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netUDP_Send(socket,addr,buf,len);
#endif
}
netStatus netUDP_Send(int32_t socket, const NET_ADDR *addr, uint8_t *buf, uint32_t len)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netUDP_Send0(socket, addr, buf, len);
#else
	return netUDP_Send1(socket, addr, buf, len);
#endif
}
//---UDP API END---


//--DHCP API---
netStatus netDHCP_Enable0(uint32_t if_num)
{
	dhcp_start(&netif);
	return netOK;
}
netStatus netDHCP_Enable1(uint32_t if_num)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netDHCP_Enable(if_num);
#endif
}
netStatus netDHCP_Enable(uint32_t if_num)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netDHCP_Enable0(if_num);
#else
	return netDHCP_Enable1(if_num);
#endif
}

netStatus netDHCP_Disable0(uint32_t if_num)
{
	dhcp_stop(&netif);
	return netOK;
}
netStatus netDHCP_Disable1(uint32_t if_num)
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netError;
#else
	eth1_Type* eth1;
	eth1=Get_eth1();
	return eth1->netDHCP_Disable(if_num);
#endif
}
netStatus netDHCP_Disable(uint32_t if_num)
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return netDHCP_Disable0(if_num);
#else
	return netDHCP_Disable1(if_num);
#endif
}

//--DHCP API END---



