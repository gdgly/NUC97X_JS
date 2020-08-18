
//����
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../Driver/Button.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../../STM32F4xx/OS/OS.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"
#include "../NUC97x/NUC97x_RTC.h"



void ButtonInit(void)//�����ϵ��ʼ��
{
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	Pin_Configure(PIN_NEXTD,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Pin_Configure(PIN_NEXTI,0);
	Pin_Configure(PIN_LEFT,0);//��������
	Pin_Configure(PIN_RIGHT,0);//��������
	Pin_Configure(PIN_ENTER,0);//��������
	Pin_Configure(PIN_ESC,0);//��������
	Pin_Configure(PIN_FACTORY,0);//��������
//	Pin_Configure(PIN_SET,0);//��������
#endif
#if ((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	Pin_Configure(PIN_FACTORY,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
#endif
}

void UnButtonPin(void)//��������Ϊ���0
{
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	Pin_Configure(PIN_NEXTD,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Pin_Configure(PIN_NEXTI,1<<4);
	Pin_Configure(PIN_LEFT,1<<4);//��������
	Pin_Configure(PIN_RIGHT,1<<4);//��������
	Pin_Configure(PIN_ENTER,1<<4);//��������
	Pin_Configure(PIN_ESC,1<<4);//��������
	Pin_Configure(PIN_FACTORY,1<<4);//��������
//	Pin_Configure(PIN_SET,0);//��������
#endif
#if ((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	Pin_Configure(PIN_FACTORY,//��������
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
#endif
}


void Button(void)//��������
{
	u32 i;
	u32 x;
	os_Type *os;
	
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	
  i=Pin_Read(PIN_NEXTD);//������,�������ʹ����;����0��1
  i|=Pin_Read(PIN_NEXTI)<<1;//������,�������ʹ����;����0��1
  i|=Pin_Read(PIN_LEFT)<<2;//������,�������ʹ����;����0��1
  i|=Pin_Read(PIN_RIGHT)<<3;//������,�������ʹ����;����0��1
  i|=Pin_Read(PIN_ENTER)<<4;//������,�������ʹ����;����0��1
  i|=Pin_Read(PIN_ESC)<<5;//������,�������ʹ����;����0��1
//  i|=Pin_Read(PIN_SET)<<6;//������,�������ʹ����;����0��1
	x=Pin_Read(PIN_FACTORY);//������,�������ʹ����;����0��1
	x=~x;
	x&=1;
	i|=x<<7;//������,�������ʹ����;����0��1
	
//	i|=Pin_Read(PIN_OPEN1)<<8;//������,�������ʹ����;����0��1
//	i|=Pin_Read(PIN_OPEN2)<<9;//������,�������ʹ����;����0��1
  //i=~i;
	//i&=0x3ff;
#endif
#if ((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	i=0;
	x=Pin_Read(PIN_FACTORY);//������,�������ʹ����;����0��1
	x=~x;
	x&=1;
	i|=x<<7;//������,�������ʹ����;����0��1
#endif

  if(Comm_Ram->ButtonCurrent!=i)
  {
		x=Comm_Ram->ButtonCurrent;
    Comm_Ram->ButtonCurrent=i;
    Comm_Ram->ButtonTimer=50/10;//50ms��ʱ
    Comm_Ram->ButtonKeepTimer=2900/10;//3s��ʱ,��100MS����3��ʱ��ʾˢ��
    Comm_Ram->MenuOutTimer=60000/10;//�˵��˳�60s��ʱ
		//�����Բ��־
		x^=i;//�����仯λ
		i=~i;
		x&=i;//�����Ӱ���(0)���طſ�(1)��ΪOK
		Comm_Ram->TESTKEY|=x;
    return;
  }
  if(Comm_Ram->ButtonTimer==0x0)
  {
    Comm_Ram->ButtonVal=i;
    Comm_Ram->ButtonTimer=0xffff;//655�������
/*
    if(Comm_Ram->SelfTestCommand==0x55555555)
    {//�ն��Բ�1(Ӳ������)
			if((i&0x3ff)!=0x0)
			{
	      BUZZ_Generate(1);//��������,��ڷ�����
	      Comm_Ram->BUZZCTRL[8]=1;//��������������/��ֹ,=0��ֹ,!=0����
			}
    }
*/
    if((i&0x7f)!=0x0)
    {//�м�����������ʾɨ��
//      Comm_Ram->DisplayScanTimer=0;//��ʾɨ��MS��ʱ��
//			Comm_Ram->BackLightTimer=60;//�������ʾ�붨ʱ��
			Comm_Ram->DisplayAllErrorTimer=0;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
			Comm_Ram->BattWork_S_timer=30;//��ع����ػ��붨ʱ��
	#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
			Meter_Ram->ButtonVal=Comm_Ram->ButtonVal;
      Meter_Ram->DisplayScanTimer=0;//��ʾɨ��MS��ʱ��
			Meter_Ram->BackLightTimer=60;//�������ʾ�붨ʱ��
	#endif
	#if ((Project/100)!=0)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
      Comm_Ram->DisplayScanTimer=0;//��ʾɨ��MS��ʱ��
			Comm_Ram->BackLightTimer=60;//�������ʾ�붨ʱ��
	#endif
    }
	#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
	  //���ܱ������ü�
	  if((Comm_Ram->ButtonVal&0x40)!=0)
	  {
	    if(MRR(ADDR_MeterFMTimer,2)==0)
	    {
	      MWR(240,ADDR_MeterFMTimer,2);//240�ֶ�ʱ
	      MC(1,ADDR_03300000+3,1);//���ü����º��1��������ݱ�־
	    }
	    else
	    {
	      MWR(0,ADDR_MeterFMTimer,2);//�嶨ʱ
	    }
	  }
	#else
		//#if ((Project/100)>=5)
	  //�ն������ü�
	  if((Comm_Ram->ButtonVal&0x40)!=0)
	  {
	    if(MRR(ADDR_TerminalFMTimer+12,2)==0)
	    {
	      MWR(240,ADDR_TerminalFMTimer+12,2);//240�ֶ�ʱ
	      //MC(1,ADDR_03300000+3,1);//���ü����º��1��������ݱ�־
	    }
	    else
	    {
	      MWR(0,ADDR_TerminalFMTimer+12,2);//�嶨ʱ
	    }
	  }
	#endif//#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
	  //����ģʽ
		if((Comm_Ram->ButtonVal&0x80)!=0)
		{
			MWR(31*24*60,ADDR_MeterFMTimer+14,2);//31*24*60�ֶ�ʱ
			MWR(31*24*60,ADDR_TerminalFMTimer+14,2);//31*24*60�ֶ�ʱ
		}
  }
	//��������
/*
  if(Comm_Ram->ButtonKeepTimer==0x0)
  {
    Comm_Ram->ButtonKeep=Comm_Ram->ButtonVal;
    Comm_Ram->ButtonKeepTimer=0xffff;//655�������
		if(Comm_Ram->ButtonVal&0x40)
		{//���ü����±���3S�幤��ģʽ��ʱ
			MC(0,ADDR_MeterFMTimer+14,2);
			MC(0,ADDR_TerminalFMTimer+14,2);
		}
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
		Meter_Ram->ButtonKeep=Comm_Ram->ButtonVal;
#endif
  }
*/
  if(Comm_Ram->ButtonKeepTimer==0x0)
  {
    Comm_Ram->ButtonKeepTimer=1000/10;//3s��ʱ,��100MS����3��ʱ��ʾˢ��
		if(Comm_Ram->ButtonCurrent&0x20)
		{//ESC�����±���3S�幤��ģʽ��ʱ
			MC(0,ADDR_MeterFMTimer+14,2);
			MC(0,ADDR_TerminalFMTimer+14,2);
		#if MainProtocol==698
			if(Comm_Ram->Factory==0x55)//����״̬
			{//����״̬�˳�ʱ����ǰͨ�ŵ�ַ����Ϊ����ͨ�ŵ�ַ
				MR(ADDR_DATABUFF,ADDR_4001,LENmax_4001_Factory);
				MW(ADDR_DATABUFF,ADDR_4001_Factory,LENmax_4001_Factory);
			}
		#endif
			//��yaffs�ļ�
			if(Comm_Ram->MS_Task!=0)
			{//os������
				//ME(ADDR_EFLASH_Start,NAND_FILE_BLOCK_COUNT*128*1024);//FLASH�����
				//WWDT_RESET();//����ʹ�ܿ��Ź���λ
				os=Get_os();
				os->yaffs_unlinkdir("user/TEST/");
				os->yaffs_unlinkdir("user/Update/");
				//os->yaffs_unlinkdir("user/Program Files/");
				
			}
		}
	}
	
	#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
	if(MRR(ADDR_MeterFMTimer+14,2)==0)
	{
		Comm_Ram->Factory=0x0;//����״̬
	}
	else
	{
		Comm_Ram->Factory=0x55;//����״̬
	}
	#else
	if(MRR(ADDR_TerminalFMTimer+14,2)==0)
	{
		Comm_Ram->Factory=0x0;//����״̬
	}
	else
	{
		Comm_Ram->Factory=0x55;//����״̬
	}
	#endif
	
}












