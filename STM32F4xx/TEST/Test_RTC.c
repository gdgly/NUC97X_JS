

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_TIM.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../Device/RTC.h"
#include "../Calculate/Calculate.h"
#include "../Device/IO_I2C.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"


void DS3231_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231д
void DS3231_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231��
void RX8025T_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//RX8025Tд
void RX8025T_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//RX8025T��
void Test_WriteRTC(u32 ADDR_BUFF)//д�ⲿӲʵʱ��
{
#if IC_DS3231==1
  DS3231_Write(ADDR_BUFF,0,7);//DS3231д
#endif
#if IC_RX8025T==1//0=û,1=��
	u8 * p8;
	p8=(u8 *)(ADDR_BUFF);
	p8[3]=1<<p8[3];
	RX8025T_Write(ADDR_BUFF,0,7);//RX8025Tд
#endif
}
void Test_ReadRTC(u32 ADDR_BUFF)//���ⲿӲʵʱ�ӵ�ָ�������ַ(Լ668us,cclk=96MHz)
{
#if IC_DS3231==1
	 u8 *p8s;
	DS3231_Read(ADDR_BUFF,0,7);//DS3231��
	p8s=(u8 *)(ADDR_BUFF);
	p8s[3]&=0x7;
	if(p8s[3]==7)
	{
		p8s[3]=0;
	}
	p8s[5]&=0x1f;
#endif
#if IC_RX8025T==1//0=û,1=��
	u32 i;
	u8 *p8s;
	RX8025T_Read(ADDR_BUFF,0,7);//RX8025T��
	p8s=(u8 *)(ADDR_BUFF);
	i=p8s[3];
	i=BitLoca(i);//�����1����λΪ1��λ��(0-32)
	if(i)
	{
		i--;
	}
	p8s[3]=i;
#endif
}

void Test_DS3231_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte,u32 ms)//DS3231��
{
	u32 ReNum;
	u32 i;
	u8 * p8;
	p8=(u8 *)(ADDR_RAM);
	for(ReNum=0;ReNum<2;ReNum++)
	{
		//InitI2c2();//I2C2��ʼ��
		if(IOI2C_Start(PIN_SCL2,PIN_SDA2)==0x0)//IO��I2C����,����0=��ȷ,1=����
		{
			if(IOI2C_Write(PIN_SCL2,PIN_SDA2,0xD0)==0x0)//IO��I2C�ֽ�д,����0=��ȷ,1=����
			{
				if(IOI2C_Write(PIN_SCL2,PIN_SDA2,ADDR_Drive)==0x0)//IO��I2C�ֽ�д,����0=��ȷ,1=����
				{
					if(IOI2C_ReStart(PIN_SCL2,PIN_SDA2)==0x0)//IO��I2C������,����0=��ȷ,1=����
					{
						if(IOI2C_Write(PIN_SCL2,PIN_SDA2,0xD1)==0x0)//IO��I2C�ֽ�д,����0=��ȷ,1=����
						{
							SoftDelayMS(ms);//���ms��ʱ
							for(i=0;i<(Byte-1);i++)
							{
								p8[i]=IOI2C_ReadAck(PIN_SCL2,PIN_SDA2);//IO��I2C�ֽڶ�ACK
							}
							p8[i]=IOI2C_ReadNoAck(PIN_SCL2,PIN_SDA2);//IO��I2C�ֽڶ�NOACK
              Comm_Ram->ErrorIC&=~(1<<4);//Ӳ�������־(��λ):B4=RTC
							IOI2C_Stop(PIN_SCL2,PIN_SDA2);//IO��I2Cֹͣ
							return;
						}
					}
				}
			}
		}
		IOI2C_Stop(PIN_SCL2,PIN_SDA2);//IO��I2Cֹͣ
    Comm_Ram->ErrorIC|=(1<<4);//Ӳ�������־(��λ):B4=RTC
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
	}
}
void Test_RX8025T_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte,u32 ms)//RX8025T��
{
	u32 ReNum;
	u32 i;
	u8 * p8;
	p8=(u8 *)(ADDR_RAM);
	for(ReNum=0;ReNum<2;ReNum++)
	{
		//InitI2c2();//I2C2��ʼ��
		if(IOI2C_Start(PIN_SCL2,PIN_SDA2)==0x0)//IO��I2C����,����0=��ȷ,1=����
		{
			if(IOI2C_Write(PIN_SCL2,PIN_SDA2,0x64)==0x0)//IO��I2C�ֽ�д,����0=��ȷ,1=����
			{
				if(IOI2C_Write(PIN_SCL2,PIN_SDA2,ADDR_Drive)==0x0)//IO��I2C�ֽ�д,����0=��ȷ,1=����
				{
					if(IOI2C_ReStart(PIN_SCL2,PIN_SDA2)==0x0)//IO��I2C������,����0=��ȷ,1=����
					{
						if(IOI2C_Write(PIN_SCL2,PIN_SDA2,0x65)==0x0)//IO��I2C�ֽ�д,����0=��ȷ,1=����
						{
							SoftDelayMS(ms);//���ms��ʱ
							for(i=0;i<(Byte-1);i++)
							{
								p8[i]=IOI2C_ReadAck(PIN_SCL2,PIN_SDA2);//IO��I2C�ֽڶ�ACK
							}
							p8[i]=IOI2C_ReadNoAck(PIN_SCL2,PIN_SDA2);//IO��I2C�ֽڶ�NOACK
              Comm_Ram->ErrorIC&=~(1<<4);//Ӳ�������־(��λ):B4=RTC
							IOI2C_Stop(PIN_SCL2,PIN_SDA2);//IO��I2Cֹͣ
							return;
						}
					}
				}
			}
		}
		IOI2C_Stop(PIN_SCL2,PIN_SDA2);//IO��I2Cֹͣ
    Comm_Ram->ErrorIC|=(1<<4);//Ӳ�������־(��λ):B4=RTC
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
	}
}





u32 Test_RTC(u32 ADDR_BASE)
{
	u32 i;
	u32 ReNum;
	u8 *p8;
	u32 RWErr;//��д:0=��ȷ,1=����
	u32 UpErr;//��λ����:0=��ȷ,1=����
	u32 tRST;//I2C���߸�λms
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	__enable_irq();//�����ж�
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
#if IC_DS3231==1//0=û,1=��
	DisplayStringAutoUp(0,9,(u8*)"\x0""DS3231");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
#if IC_RX8025T==1//0=û,1=��
	DisplayStringAutoUp(0,9,(u8*)"\x0""RX8025T");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
//д���Ƚ�
	ReNum=2;
	while(ReNum--)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[0]=0x59;
		p8[1]=0x59;
		p8[2]=0x23;
		p8[3]=6;
		p8[4]=0x31;
		p8[5]=0x12;
		p8[6]=0x99;
		Test_WriteRTC(ADDR_DATABUFF);//д�ⲿӲʵʱ��
		Test_ReadRTC(ADDR_DATABUFF+7);//���ⲿӲʵʱ�ӵ�ָ�������ַ
		for(i=0;i<7;i++)
		{
			if(p8[i]!=p8[7+i])
			{
				if(i==3)
				{//��
					if(p8[7+i]==6)
					{
						continue;
					}
				}
				RWErr=1;//��д:0=��ȷ,1=����
				TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
				break;
			}
		}
		if(i>=7)
		{
			RWErr=0;//��д:0=��ȷ,1=����
			break;
		}
	}
//��λ����
	Comm_Ram->GPMSTimer=1500/10;//18 ͨ��(������UART11) ÿ10MS��1��ʱ��
	UpErr=1;//��λ����:0=��ȷ,1=����
	while(Comm_Ram->GPMSTimer)
	{
		Test_ReadRTC(ADDR_DATABUFF+1);//���ⲿӲʵʱ�ӵ�ָ�������ַ
		p8=(u8*)ADDR_DATABUFF+1;
		if(p8[6]==0)
		{
			if(p8[5]==1)
			{
				if(p8[4]==1)
				{
					if(p8[3]==0)
					{
						if(p8[2]==0)
						{
							if(p8[1]==0)
							{
								if((p8[0]==0)||(p8[0]==1))
								{
									UpErr=0;//����:0=��ȷ,1=����
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	if(UpErr)//��λ����:0=��ȷ,1=����
	{
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
//I2C���߸�λms
	tRST=950;//I2C���߸�λms
	p8=(u8*)ADDR_DATABUFF+8;
	MC(0xff,ADDR_DATABUFF+8,7);
#if IC_DS3231==1//0=û,1=��
	Test_DS3231_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231��
#endif
#if IC_RX8025T==1//0=û,1=��
	Test_RX8025T_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231��
#endif
	i=1;//����
	if(((p8[6]<<16)+(p8[5]<<8)+p8[4])==0x000101)
	{
		i=0;//��ȷ
	}
	if(i==0)
	{//950msʱ����ȷ
		for(i=0;i<6;i++)
		{
			tRST+=10;//I2C���߸�λms
			MC(0xff,ADDR_DATABUFF+8,7);
		#if IC_DS3231==1//0=û,1=��
			Test_DS3231_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231��
		#endif
		#if IC_RX8025T==1//0=û,1=��
			Test_RX8025T_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231��
		#endif
			if(((p8[6]<<16)+(p8[5]<<8)+p8[4])!=0x000101)
			{
				break;
			}
		}
	}
	else
	{//950msʱ����
		for(i=0;i<6;i++)
		{
			tRST-=10;//I2C���߸�λms
			MC(0xff,ADDR_DATABUFF+8,7);
		#if IC_DS3231==1//0=û,1=��
			Test_DS3231_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231��
		#endif
		#if IC_RX8025T==1//0=û,1=��
			Test_RX8025T_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231��
		#endif
			if(((p8[6]<<16)+(p8[5]<<8)+p8[4])==0x000101)
			{
				break;
			}
		}
	}
	if((tRST<900)||(tRST>1000))
	{
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
	
//�洢����
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=RWErr;
	p8[1]=UpErr;
	p8[2]=tRST;
	p8[3]=tRST>>8;
	p8[4]=0;//1Hz���(0=OK,1=ERROR)
	p8[5]=0;//32kHz���(0=OK,1=ERROR)
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_RTC_Characteristics),6);
	
	if(TestResult==0)//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
	}
	
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=0x00;
	p8[1]=0x00;
	p8[2]=0x12;
	p8[3]=2;
	p8[4]=0x01;
	p8[5]=0x10;
	p8[6]=0x19;
	Test_WriteRTC(ADDR_DATABUFF);//д�ⲿӲʵʱ��
	return TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
}







