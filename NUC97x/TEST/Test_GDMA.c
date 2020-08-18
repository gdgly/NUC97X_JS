
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../NUC97x/NUC97x_DMA.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_CRC.h"




u32 Test_GDMA(u32 ADDR_BASE)//����GDMA;���:Ҫ���ԵĿ��ַ;����:0=��ȷ,1=����
{
	u32 i;
	u32* p32s;
	u32* p32d;
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""GDMA");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol

	p32d=(u32*)(ADDR_msFILEmanage+LEN_msFILEmanage);
	for(i=0;i<(LEN_msFILEmanage/4);i++)
	{
		p32d[i]=0;
	}
	p32s=(u32*)(ADDR_msFILEmanage+LEN_msFILEmanage-8);
	p32s[0]=0x12345678;
	p32s[1]=RAM_CRC32(ADDR_msFILEmanage,LEN_msFILEmanage-4);//RAM������CRC32����
	MW_DMA(ADDR_msFILEmanage,ADDR_msFILEmanage+LEN_msFILEmanage,LEN_msFILEmanage);
	//�ȴ�DMA���
	p32s=(u32*)(REG_GDMA_CTL0);
	while(p32s[0]&1);
	//
	p32s=(u32*)(ADDR_msFILEmanage);
	for(i=0;i<(LEN_msFILEmanage/4);i++)
	{
		if(p32s[i]!=p32d[i])
		{
			DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
			return 1;
		}
	}
	DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	return 0;
}














