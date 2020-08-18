
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/OS/OS.h"
#include "../../STM32F4xx/MS/MS.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/Calculate/ascii.h"
#include "../Calculate/Calculate.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"


void Test_USBfileRW(void)//����USB�ļ���д�ɿ���
{
	u32 i;
	u32 LEN_File;
	u32* p32;
	os_Type *os;
	ms_Type *ms;
	FATFS fatfs;
	FIL fil;
	FRESULT fresult;
	FRESULT usfile;
	u32 Count;
	u16 *pStrBuff;
	
	if(FileDownload->USB0Task!=5)//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
	{
		return;
	}
	Count=0;
	DisplayClr();//����
	DisplayStringAutoUp(0,9,(u8*)"\x0""USB TEST");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	os=Get_os();
	ms=Get_ms();
	p32=ms->ms_malloc(1024*1024);
	
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
		#ifdef PIN_ESC
		if(Pin_Read(PIN_ESC))//������,�������ʹ����;����0��1
		{
			return;
		}
		#endif
		for(i=0;i<(1024*1024)/4;i++)
		{
			p32[i]=i;
		}
		fresult=os->f_mount(&fatfs,"3:",0);//Mount the default drive
		if(fresult!=FR_OK)
		{
			DisplayStringAutoUp(0,9,(u8*)"\x0""USB: mount fail!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			continue;
		}
		fresult=os->f_chdir("/TEST");
		if(fresult!=FR_OK)
		{
			os->f_mkdir("/TEST");
			os->f_chdir("/TEST");
		}
		usfile=os->f_open(&fil,"TEST.txt",FA_CREATE_ALWAYS|FA_WRITE);
		if(usfile!=FR_OK)
		{
			DisplayStringAutoUp(0,9,(u8*)"\x0""f_open_write");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			continue;
		}
		os->f_WRITE(&fil,(u8* )p32,1024*1024);
		os->f_close(&fil);
		usfile=os->f_open(&fil,"TEST.txt",FA_OPEN_EXISTING|FA_READ);
		if(usfile!=FR_OK)
		{
			DisplayStringAutoUp(0,9,(u8*)"\x0""f_open_read");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			continue;
		}
		LEN_File=fil.fsize;
		if(LEN_File!=(1024*1024))
		{
			os->f_close(&fil);
			DisplayStringAutoUp(0,9,(u8*)"\x0""fil.fsize!=1M");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			continue;
		}
		LEN_File=os->f_READ(&fil,(void *)p32,1024*1024);
		if(LEN_File!=(1024*1024))
		{
			os->f_close(&fil);
			DisplayStringAutoUp(0,9,(u8*)"\x0""LEN_Read!=1M");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			continue;
		}
		os->f_close(&fil);
		for(i=0;i<(1024*1024)/4;i++)
		{
			if(p32[i]!=i)
			{
				DisplayStringAutoUp(0,9,(u8*)"\x0""File Data");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				continue;
			}
		}
		os->f_mount(0,"3:",0);//ȥ����Mount the default drive
		
		Count++;
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row)*84)+4);
		for(i=0;i<8;i++)
		{
			pStrBuff[i]=0;
		}
		i=hex_bcd(Count);
		DisplayData_OffZero(i,8,0,pStrBuff);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
		LCDDataOut();//�������������LCD��
		
	}
}











