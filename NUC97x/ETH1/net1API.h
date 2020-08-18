
#ifndef net1API_H
#define net1API_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "../../NUc97x/OS/API/netAPI.h"

typedef struct
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
void (*_0) (void);
#else
//timeouts
void (*ETH1_sys_check_timeouts) (void);
	
//  ==== Network System API ====
netStatus (*netInitialize) (void);
void (*netifInitialize) (void);//eth0 netif init
//  ==== TCP Socket API ====
int32_t   (*netTCP_GetSocket) (netTCP_cb_t cb_func);
netStatus (*netTCP_ReleaseSocket) (int32_t socket);
netStatus (*netTCP_Listen) (int32_t socket, uint16_t port);
netStatus (*netTCP_Connect) (int32_t socket, const NET_ADDR *addr, uint16_t local_port);
netStatus (*netTCP_Close) (int32_t socket);
netStatus (*netTCP_Abort) (int32_t socket);
uint32_t  (*netTCP_GetMaxSegmentSize) (int32_t socket);
//uint8_t  *(*netTCP_GetBuffer) (uint32_t size);
bool      (*netTCP_SendReady) (int32_t socket);
netStatus (*netTCP_Send) (int32_t socket, uint8_t *buf, uint32_t len);
//netTCP_State (*netTCP_GetState) (int32_t socket);
//netStatus (*netTCP_ResetReceiveWindow) (int32_t socket);
//netStatus (*netTCP_SetOption )(int32_t socket, netTCP_Option option, uint32_t val);
//uint16_t  (*netTCP_GetLocalPort) (int32_t socket);
//netStatus (*netTCP_GetPeer) (int32_t socket, NET_ADDR *addr, uint32_t addr_len);
//uint32_t  (*netTCP_GetTimer) (int32_t socket);
//  ==== UDP Socket API ====
int32_t   (*netUDP_GetSocket) (netUDP_cb_t cb_func);
netStatus (*netUDP_ReleaseSocket) (int32_t socket);
netStatus (*netUDP_Open) (int32_t socket, uint16_t port);
netStatus (*netUDP_Close) (int32_t socket);
//uint8_t  *(*netUDP_GetBuffer) (uint32_t size);
netStatus (*netUDP_Send) (int32_t socket, const NET_ADDR *addr, uint8_t *buf, uint32_t len);
//netStatus (*netUDP_SetOption) (int32_t socket, netUDP_Option option, uint32_t val);
//uint16_t  (*netUDP_GetLocalPort) (int32_t socket);

//====THCP API===
netStatus (*netDHCP_Enable) (uint32_t if_num);
netStatus (*netDHCP_Disable) (uint32_t if_num);
#endif
}const eth1_Type;

eth1_Type* Get_eth1(void);

#ifdef __cplusplus
 }
#endif 
#endif
