
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/Device/IC_TESAM.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SPI.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/Device/MEMRW.h"


#if IC_TESAM==0//�ն���ESAM,0==û,1==��
void TESAM_RXTX(void)//TESAM���շ���
{
}
u32 TESAM_CMD_TxRx(u32 ClaInsP1P2,u32 Lc,u32 ADDR_DATA)//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
{
	return 0;
}
#else//#if IC_TESAM==0//�ն���ESAM,0==û,1==��

void TESAM_RXTX(void)//TESAM���շ���(��׼SPI0��)
{
  u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	u16 * p16timer;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	u32 RxByte;
	
	p16timer=(u16 *)(ADDR_UART7Timer);
	switch(UART7Ctrl->Task)
	{
		case 0://�ϵ��λ(�ص�Դ)
			//SPI����Ϊ��������
			UnInit_SPI1();//����SPI1��ʼ��(TESAM),SPI��Ϊ��������
			//�ص�Դ
			Pin_Configure(PIN_ENTESAM,//��������
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(1<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
			p16timer[0]=50/10;//��ʱ50ms
			while(p16timer[0]);
			UART7Ctrl->Task++;
			//break;
		case 1://����Դ
			//ESAMоƬ�ϵ縴λ�ڼ䣬SSN���ű��ָߵ�ƽ�� MISO���ű�������״̬��MOSI��������͵�ƽ
			Pin_Configure(PIN_ENTESAM,//��������
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (1<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
			Pin_Configure(PIN_SPI0_SS1,//��������
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
			p16timer[0]=50/10;//��ʱ50ms
			while(p16timer[0]);
			UART7Ctrl->Task++;
			//break;
		case 2://����Դ��ʱ
			UART7Ctrl->Task=100;
			break;
			
    case 10://��������(�ȷ�7�ֽ� 0x55 CLA INS P1 P2 Len1 Len2)
			Init_SPI1();//SPI1��ʼ��(TESAM)
			SoftDelayUS(10);//��SSN�øߺ�T-ESAM������͹���״̬���ߵ�ƽ����ʱ������10us����
			//CS=0
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=2;
			SoftDelayUS(50);//�ӿ��豸��SSN�ٴ��õͺ���ȴ�50us����ʱ����ܿ�ʼ��������
			//�ȷ�7�ֽ�(�ȷ��� MSB)
			p32TxData=(u32*)(REG_SPI0_TX0);
			p32ctrl=(u32*)(REG_SPI0_CNTRL);
		  p8=(u8 *)(ADDR_UART7Tx);
			for(i=0;i<7;i++)
			{
				p32TxData[0]=p8[i];
				//p32ctrl[0]|=1;
				p32ctrl[0]=0x80000000|(8<<3)|(1<<1)|1;
				while(p32ctrl[0]&1);
			#if MainProtocol==698
				//SoftDelayUS(1);//���������ֽ�֮����ʱ�����15us
			#else
				//SoftDelayUS(20);//���������ֽ�֮����ʱ�����15us
			#endif
			}
			x=(p8[5]<<8)+p8[6];//Lc
			//�ٷ��������ݺ�LRC1
			x+=1;
			SoftDelayUS(100);//�ӿ��豸����������Len2����Ҫ��100��sʱ�������ٷ���DATA
		  p8=(u8 *)(ADDR_UART7Tx+7);
			for(i=0;i<x;i++)
			{
				p32TxData[0]=p8[i];
				//p32ctrl[0]|=1;
				p32ctrl[0]=0x80000000|(8<<3)|(1<<1)|1;
				while(p32ctrl[0]&1);
			#if MainProtocol==698
				//SoftDelayUS(1);//���������ֽ�֮����ʱ�����15us
			#else
				//SoftDelayUS(20);//���������ֽ�֮����ʱ�����15us
			#endif
			}
			//CS=1
			SoftDelayUS(5);
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=0;
			UART7Ctrl->Task++;
			//break;
		case 11://��������
			//Init_SPI1();//SPI1��ʼ��(TESAM)
			SoftDelayUS(10);//��SSN�øߺ�T-ESAM������͹���״̬���ߵ�ƽ����ʱ������10us����
			//CS=0
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=2;
			SoftDelayUS(50);//�ӿ��豸��SSN�ٴ��õͺ���ȴ�50us����ʱ����ܿ�ʼ��������
			MC(0,ADDR_UART7Rx,LEN_UART7Rx);//����ջ���
			p16timer[0]=3000/10;//�ȴ���1��0x55���ݷ��ؽ��ն�ʱ
			UART7Ctrl->Task++;
			//break;
		case 12://���ڽ���
			RxByte=0;
			p32TxData=(u32*)(REG_SPI0_TX0);
			p32TxData[0]=0;
			p32RxData=(u32*)(REG_SPI0_RX0);
			p32ctrl=(u32*)(REG_SPI0_CNTRL);
			p8=(u8 *)(ADDR_UART7Rx);
			while(1)
			{
				if(RxByte==0)
				{
					if(p16timer[0]==0)
					{
						//CS=1
						p32ctrl=(u32*)(REG_SPI0_SSR);
						p32ctrl[0]=0;
						UART7Ctrl->Task=101;//101=���մ���
						return;
					}
				}
				//p32ctrl[0]|=1;
				p32ctrl[0]=0x80000000|(8<<3)|(1<<1)|1;
				while(p32ctrl[0]&1);
				if(RxByte>=LEN_UART7Rx)
				{
					UART7Ctrl->RxByte=RxByte;//�����ֽ���
					SoftDelayUS(5);//ÿ�� SPI ͨѶ��������ȴ�һ��ʱ��ſ��Խ� SSN ���ߣ��õȴ�ʱ�� Tcsd>3us 
					//CS=1
					p32ctrl=(u32*)(REG_SPI0_SSR);
					p32ctrl[0]=0;
					UART7Ctrl->Task++;//���ս���
					return;
				}
				i=p32RxData[0];
				p8[RxByte]=i;
				if(RxByte==0)
				{
					if((i&0xff)==0x55)
					{
						RxByte=1;//�����ֽ���
					#if MainProtocol==698
						//SoftDelayUS(1);//���������ֽ�֮����ʱ�����15us
					#else
						//SoftDelayUS(20);//���������ֽ�֮����ʱ�����15us
					#endif
					}
					else
					{
						SoftDelayUS(20);//��ѯ0x55,�ü��ʱTq>15us
					}
				}
				else
				{
					RxByte++;
					if(RxByte>=6)
					{
						i=(p8[3]<<8)+p8[4];
						i+=6;
						if(RxByte>=i)
						{
							UART7Ctrl->RxByte=RxByte;//�����ֽ���
							SoftDelayUS(5);//ÿ�� SPI ͨѶ��������ȴ�һ��ʱ��ſ��Խ� SSN ���ߣ��õȴ�ʱ�� Tcsd>3us 
							//CS=1
							p32ctrl=(u32*)(REG_SPI0_SSR);
							p32ctrl[0]=0;
							UART7Ctrl->Task++;//���ս���
							return;
						}
					}
				#if MainProtocol==698
					//SoftDelayUS(1);//���������ֽ�֮����ʱ�����15us
				#else
					//SoftDelayUS(20);//���������ֽ�֮����ʱ�����15us
				#endif
				}
			}
    case 13://���ս���
			//LRC2�ļ��㷽������SW1 SW2 Len1 Len2 DATA���ݣ�ÿ���ֽڵ����ֵ����ȡ����
			p8=(u8 *)(ADDR_UART7Rx+1);
			x=(p8[2]<<8)+p8[3];
			x+=3;
			y=p8[0];
			for(i=0;i<x;i++)
			{
				y^=p8[1+i];
			}
			y=(~y);
			y&=0xff;
			if(y==p8[1+x])
			{
				UART7Ctrl->Task=100;//100=�������
			}
			else
			{
				UART7Ctrl->Task=101;//101=���մ���
			}
      break;
     
		case 100://�������
      break;
		case 101://���մ���
			break;
		default:
      UART7Ctrl->Task=0;//�ϵ��λ(�ص�Դ)
			break;
	}
}

u32 TESAM_CMD_TxRx(u32 ClaInsP1P2,u32 Lc,u32 ADDR_DATA)//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
{
//ClaInsP1P2��APDU����4�ֽ��ױ�ֱ�ΪCla(ָ�����),Ins(ָ������е�ָ�����),P1(����1),P2(����2)
//Lc(P3)��APDU����������ֶ���(����)���ֵ��ֽ���
	u32 i;
	u32 x;
	u8 * p8;
//  u32 *p32;
  
	if(UART7Ctrl->Lock==0x55)
	{
    if(UART7Ctrl->Task==100)
    {
      UART7Ctrl->Task=101;
    }
		return 1;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	if(Lc>(LEN_UART7Tx-8))
	{//���ͻ��峤�Ȳ���
		UART7Ctrl->Task=101;
		return 1;
	}  
  UART7Ctrl->Lock=0x55;
  while(UART7Ctrl->Task<100)
  {
    TESAM_RXTX();//TESAM���շ���
//    p32=(u32 *)ADDR_MS_ENTRY;
//    (*(void(*)())p32[115])();//115 ���߳�
  }
  p8=(u8 *)(ADDR_UART7Tx);
	p8[0]=0x55;
	p8[1]=ClaInsP1P2>>24;
	p8[2]=ClaInsP1P2>>16;
	p8[3]=ClaInsP1P2>>8;
	p8[4]=ClaInsP1P2>>0;
	p8[5]=Lc>>8;
	p8[6]=Lc;
	if(ADDR_DATA!=(ADDR_UART7Tx+7))
	{
		MR(ADDR_UART7Tx+7,ADDR_DATA,Lc);
	}
	//LRC1�ļ��㷽������CLA INS P1 P2 Len1 Len2 DATA���ݣ�ÿ���ֽڵ����ֵ����ȡ��
	x=p8[1];
	for(i=0;i<(5+Lc);i++)
	{
		x^=p8[2+i];
	}
	x=(~x);
	p8[7+Lc]=x;
	
	UART7Ctrl->Task=10;//�ȴ����ͺ����
  while(UART7Ctrl->Task<100)
  {
    TESAM_RXTX();//TESAM���շ���
//    p32=(u32 *)ADDR_MS_ENTRY;
//    (*(void(*)())p32[115])();//115 ���߳�
  }
  UART7Ctrl->Lock=0;
	
	p8=(u8 *)(ADDR_UART7Rx);
	i=(p8[1]<<8)+p8[2];
	if((UART7Ctrl->Task!=100)||(i!=0x9000))
	{
		return 1;
	}
	return 0;
}

#endif//#if IC_TESAM==0//�ն���ESAM,0==û,1==��



