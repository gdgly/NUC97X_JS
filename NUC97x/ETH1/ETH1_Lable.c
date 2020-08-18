
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../../NUc97x/OS/API/netAPI.h"
#include "../../../NUc97x/ETH1/net1API.h"



void _0(void);
void ETH1_sys_check_timeouts(void);//ETH1定时

const eth1_Type eth1_Lable=
{
#if RJ45SELECT==0//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	_0,
#else
//
	ETH1_sys_check_timeouts,
//
	netInitialize,
	netifInitialize,
	//TCP
	netTCP_GetSocket,
	netTCP_ReleaseSocket,
	netTCP_Listen,
	netTCP_Connect,
	netTCP_Close,
	netTCP_Abort,
	netTCP_GetMaxSegmentSize,
	netTCP_SendReady,
	netTCP_Send,
	//UDP
	netUDP_GetSocket,
	netUDP_ReleaseSocket,
	netUDP_Open,
	netUDP_Close,
	netUDP_Send,
	//DHCP
	netDHCP_Enable,
	netDHCP_Disable,
#endif
};









