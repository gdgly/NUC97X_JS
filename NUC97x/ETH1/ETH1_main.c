
#if MainProtocol==376
#include "../../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/OS/OS.h"


void _0(void)
{
}

void ETH1_main(void)
{
	while(1);
}


os_Type *Get_os(void)//�õ�os���ַ
{
	u32 *p32;
	p32=(u32*)(ADDR_OS_ENTRY);
	return (os_Type *)p32[130];
}

INT32 sysStartTimer(INT32 nTimeNo, UINT32 uTicksPerSecond, INT32 nOpMode)
{
	os_Type * os;
	os=Get_os();
	return os->sysStartTimer(nTimeNo, uTicksPerSecond, nOpMode);
}

INT32 sysSetTimerEvent(INT32 nTimeNo, UINT32 uTimeTick, PVOID pvFun)
{
	os_Type * os;
	os=Get_os();
	return os->sysSetTimerEvent(nTimeNo, uTimeTick, pvFun);
}

UINT32 sysGetTicks(INT32 nTimeNo)
{
	os_Type * os;
	os=Get_os();
	return os->sysGetTicks(nTimeNo);
}

extern unsigned char net1InitializeFlag;//net0��ʼ����־:0=û,1=��ʼ��
void sys_check_timeouts(void);
void ETH1_sys_check_timeouts(void)//ETH1��ʱ
{

	if(net1InitializeFlag==1)//net0��ʼ����־:0=û,1=��ʼ��
	{
		sys_check_timeouts();  // All network traffic is handled in interrupt handler
	}
}


