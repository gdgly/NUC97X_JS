
//�ն˵�Դ�½�
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../DL698/DL698_DataDef.h"
#include "Terminal_PowerDown.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_IO.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../Device/GPRS_3763.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"

#include "Terminal698_Uart_Manage.h"
#include "../Terminal/Terminal_ReadMeter_ACSample.h"

#include "../MS/Data_Save.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/EVENT_Record.h"

#ifdef IC_STM32F4xx
#include "../STM32F4xx/STM32F4xx_MAC.h"
#endif
#ifdef IC_NUC9xx
#include "../../NUC97x/NUC97x/NUC97x_MAC.h"
#endif




#if ((Project/100)<2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=ͨ��ģ��
void Terminal_PowerDown(void)//2=��������ת��ع���
{
}
#else
void Terminal_PowerDown(void)//2=��������ת��ع���
{
//	u32 i;
	u32 PORTn;
//	u32 *p32;
  UARTCtrl_TypeDef * UARTCtrl;
	
	PORTn=0;
	if(UART1Ctrl->LinkTask>=101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		PORTn=GPRSPORT;
	}
	else
	{
		UART1Ctrl->LinkTask=1;//1=���ܵ�Դ
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if(UARTCtrl->LinkTask>=101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		PORTn=ENETPORT;
	}
	else
	{
		UARTCtrl->LinkTask=1;//1=���ܵ�Դ
	}
//---test---
	//PORTn=ENETPORT;
//---
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Ram->PowerDownTask)//��Դ�����������
	{
		case 0://�����¼�����
			//������ʾ���
			DisplayStringAutoUp(0,9,(u8*)"\x0""����ͣ��");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			Comm_Ram->BattWork_S_timer=90;//60;//4 ��ع����ػ��붨ʱ��
			Terminal_Ram->ENETHeartBeat_S_Timer=120;//2 ��̫�������붨ʱ��//�յ���������֡������ʱ60��
			Terminal_Ram->GPRSHeartBeat_S_Timer=120;//5 GPRS�����붨ʱ��//�յ���������֡������ʱ60��
			Terminal_Ram->PowerDownTask++;//��Դ�����������
			break;
		case 1://��鵱ǰ�Ƿ�����
		#if TEST_DISPVCAP==1//0��Ч,1=����;��ʾ�������ݵ�س�ŵ�ʱ��
			PORTn=GPRSPORT;
		#endif
			if(PORTn==0)
			{
				UART1Ctrl->LinkTask=1;//1=�ص�Դ
				UART1Ctrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				UnInit_GPRS_3763();//GPRSģ������Ϊ��������
				OffR5V0();//��R5V0��Դ
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				UARTCtrl->LinkTask=1;//1=�ص�Դ
				UARTCtrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
			#ifdef IC_STM32F4xx
				UnInit_STM32F4xx_MAC();
				OffE5V0();//��E5V0��Դ
			#endif
			#ifdef IC_NUC9xx
				UnInit_NUC97x_MAC0();
				UnInit_NUC97x_MAC1();
				OffE5V0();//��E5V0��Դ
				Off2E5V0();//��E5V0��Դ
			#endif
				Terminal_Ram->PowerDownTask=50;//��Դ�����������.���ϱ�
			}
			else
			{
				if(PORTn==GPRSPORT)
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
					UARTCtrl->LinkTask=1;//1=�ص�Դ
					UARTCtrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				#ifdef IC_STM32F4xx
					UnInit_STM32F4xx_MAC();
					OffE5V0();//��E5V0��Դ
				#endif
				#ifdef IC_NUC9xx
					UnInit_NUC97x_MAC0();
					UnInit_NUC97x_MAC1();
					OffE5V0();//��E5V0��Դ
					Off2E5V0();//��E5V0��Դ
				#endif
				}
				else
				{
					UART1Ctrl->LinkTask=1;//1=�ص�Դ
					UART1Ctrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
					UnInit_GPRS_3763();//GPRSģ������Ϊ��������
					OffR5V0();//��R5V0��Դ
				}
				//DisplayStringAutoUp(0,9,(u8*)"\x0""ͣ���¼��ϱ�");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				Terminal_Ram->PowerDownTask++;//��Դ�����������
			}
			break;
		case 2://�ڶ�ʱ�ڵȴ��ϱ�
			if(Comm_Ram->BattWork_S_timer==0)//��ع����ػ��붨ʱ��
			{
				Terminal_Ram->PowerDownTask++;//��Դ�����������
			}
			break;
		case 3://���˳���¼
			DisplayStringAutoUp(0,9,(u8*)"\x0""ͣ���˳���¼");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			Comm_Ram->BattWork_S_timer=10;//�ȴ����˳���¼��ʱ
			UARTCtrl->LinkTask=102;
			Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 ��̫�������붨ʱ��
			Terminal_Ram->GPRSHeartBeat_S_Timer=0;//5 GPRS�����붨ʱ��
			Terminal_Ram->PowerDownTask++;//��Դ�����������
			break;
		case 4://�ȴ��˳���¼
			if((UARTCtrl->LinkTask==103)||(Comm_Ram->BattWork_S_timer==0))//��ع����ػ��붨ʱ��
			{
				Terminal_Ram->PowerDownTask++;//��Դ�����������
			}
			break;
		case 5://�ػ�
			UART1Ctrl->LinkTask=104;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			UARTCtrl->LinkTask=104;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			Comm_Ram->BattWork_S_timer=5;//�ȴ�ģ��ػ���ʱ
			Terminal_Ram->PowerDownTask++;//��Դ�����������
			break;
		case 6://����
			if(Comm_Ram->BattWork_S_timer==0)//��ع����ػ��붨ʱ��
			{
				Battery_Data_Save();//��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			if((UART1Ctrl->LinkTask==2)||(UARTCtrl->LinkTask==2))//���ܵ�Դ(�ȴ��ػ���ʱ)
			{
				Battery_Data_Save();//��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
			}
			break;
		
		case 50://���ϱ�
			//������ʾ���
			DisplayStringAutoUp(0,9,(u8*)"\x0""�����߲��ϱ�");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			Comm_Ram->BattWork_S_timer=5;//4 ��ع����ػ��붨ʱ��
			Terminal_Ram->PowerDownTask++;//��Դ�����������
			break;
		case 51://�ȴ���ʱ��ػ�
			if(Comm_Ram->BattWork_S_timer==0)//��ع����ػ��붨ʱ��
			{
				if(PowerEvent->task<100)
				{//ͣ���¼��Ѽ�¼���
					Battery_Data_Save();//��ع���ʱ�ػ�ǰ���ݱ���
					DeepPowerDown();//��ȵ�Դ�½�ģʽ
				}
			}
			break;
			
		default:
			Terminal_Ram->PowerDownTask=0;//��Դ�����������
			break;
	}
}

#endif//#if ((Project/100)<2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=ͨ��ģ��




//����6�����ò�������=structure
//{
//ͣ�����ݲɼ����ò���  structure
//{
//�ɼ���־  bit-string(SIZE(8))��
//ͣ���¼�����ʱ������Сʱ�� unsigned ��
//ͣ���¼�����ʱ����ֵ�����ӣ� unsigned��
//��Ҫ��ȡͣ���¼����ܱ�       array TSA
//}��
//ͣ���¼������ֵ����  structure
//{
//ͣ��ʱ����С��Ч��������ӣ� long-unsigned��
//ͣ��ʱ�������Ч��������ӣ� long-unsigned��
//ͣ���¼���ֹʱ��ƫ����ֵ�����ӣ� long-unsigned��
//ͣ���¼�ʱ������ƫ����ֵ�����ӣ� long-unsigned��
//ͣ�緢����ѹ��ֵ  long-unsigned����λ��V�����㣺-1����
//ͣ��ָ���ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
//}
//}
//�ɼ���־��
//bit0���á�1����Ч���á�0����Ч��
//bit1���á�1�����ѡ������㣬�á�0��ֻ�ɼ����õĲ����㡣
//�¼�����Դ��=NULL
void RM_Meter_Event3106(void)//ͣ/�ϵ��¼��賭�������������(��0��ʼ)
{
	u32 i;
	u32 j;
	u32 n;
	u32 x;
	u8* p8;
	
	MR(ADDR_128KDATABUFF,ADDR_3106_6,LENmax_3106_6);
	p8=(u8*)ADDR_128KDATABUFF;
	i=p8[6];//�ɼ���־  bit-string(SIZE(8))
	if((i&0x80)==0)
	{//��Ч
		PowerEvent->NUMrm=0;//ͣ/�ϵ��¼��賭��������
		return;
	}
	j=0;//��Ч����
	if((i&0x40)==0)
	{//ֻ�ɼ����õĲ�����
		n=p8[12];//TSA��
		if(n>NUMmax_Event3106RM)
		{
			n=NUMmax_Event3106RM;
		}
		p8+=16;
		for(i=0;i<n;i++)
		{
			x=AddrToMeterSetNo((u16*)p8);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
			if(x<NUM_RMmax)
			{
				PowerEvent->setno[j]=x;//ͣ/�ϵ��¼��賭�������������(��0��ʼ)
				j++;
			}
			p8+=9;
		}
	}
	else
	{//���ѡ�������
		//��ѡRS485��
		for(i=1;i<NUM_RMmax;i++)
		{
			if(j>=NUMmax_Event3106RM)
			{
				break;
			}
			p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
			if(p8[0]==DataType_structure)
			{
				p8+=5;
				//��Լ����
				p8=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8==0)
				{
					continue;
				}
				x=p8[1];
				if((x==2)||(x==3))
				{//DL/T645-2007��2��,DL/T698.45��3��
					//�˿�
					p8+=2;
					x=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
					x=OADtoPORTn(x);
					if((x==RS485_1PORT)||(x==RS485_2PORT))
					{
						PowerEvent->setno[j]=i;//ͣ/�ϵ��¼��賭�������������(��0��ʼ)
						j++;
					}
				}
			}
		}
		//����ѡ����ѡ�ز���
		if(j==0)
		{
			for(i=1;i<NUM_RMmax;i++)
			{
				if(j>=NUMmax_Event3106RM)
				{
					break;
				}
				p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
				if(p8[0]==DataType_structure)
				{
					p8+=5;
					//��Լ����
					p8=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8==0)
					{
						continue;
					}
					x=p8[1];
					if((x==2)||(x==3))
					{//DL/T645-2007��2��,DL/T698.45��3��
						//�˿�
						p8+=2;
						x=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
						x=OADtoPORTn(x);
						if(x==RS485_4PORT)
						{
							PowerEvent->setno[j]=i;//ͣ/�ϵ��¼��賭�������������(��0��ʼ)
							j++;
						}
					}
				}
			}
		}
	}
	PowerEvent->NUMrm=j;//ͣ/�ϵ��¼��賭��������
}

u32 CompVoltage(u32 V)//�Ƚ�ͣ���ѹ��ֵ,���:V=0�ָ���ѹ��ֵ,V=1������ѹ��ֵ;����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
{
	u32 i;
	u8* p8;
	u16 *p16;
//ͣ���¼������ֵ����  structure
//{
//ͣ��ʱ����С��Ч��������ӣ� long-unsigned��
//ͣ��ʱ�������Ч��������ӣ� long-unsigned��
//ͣ���¼���ֹʱ��ƫ����ֵ�����ӣ� long-unsigned��
//ͣ���¼�ʱ������ƫ����ֵ�����ӣ� long-unsigned��
//ͣ�緢����ѹ��ֵ  long-unsigned����λ��V�����㣺-1����
//ͣ��ָ���ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
//}
	MR(ADDR_128KDATABUFF,ADDR_3106_6,LENmax_3106_6);
	p8=(u8*)ADDR_128KDATABUFF;
	p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
	if(p8==0)
	{
		return 1;
	}
	if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		MC(0,ADDR_UA,3*4);//�彻�ɵ�ѹֵ
	}
	if(V==0)
	{
		i=(p8[18]<<8)|p8[19];//ͣ��ָ���ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
	}
	else
	{
		i=(p8[15]<<8)|p8[16];//ͣ�緢����ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
	}
	V=hex_bcd(i);
	for(i=0;i<3;i++)
	{
		p16=(u16 *)(ADDR_UA+2+(i*4));
		if(p16[0]>=V)
		{
			return 1;
		}
	}
	return 0;
}


void PowerEventTimerComp(u8* pEVENT)//ͣ�ϵ��¼�ʱ��Ƚ�,���:pEVENTָ��698��ʽ�¼��Ŀ�ʼ,����:PowerEvent->bit
{
	u32 x;
	u32 t1;
	u32 t2;
	u8* p8;
	
	if(PowerEvent->bit==0xc0)//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
	{
		return;
	}
//ͣ/�ϵ��¼���¼��Ԫ��=structure
//{
//  2 �¼���¼���  double-long-unsigned��
//  7 �¼�����ʱ��  date_time_s��
//  15 �¼�����ʱ��  date_time_s��
//...
//}
	x=Check_pYYMDHMS_hex(pEVENT+8);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
	if(x)
	{
		PowerEvent->TimeErr=1;//�г����ĵ��ܱ�ͣ���¼�ͣ����ϵ�ʱ�䲻����
		return;
	}
	x=Check_pYYMDHMS_hex(pEVENT+16);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
	if(x)
	{
		PowerEvent->TimeErr=1;//�г����ĵ��ܱ�ͣ���¼�ͣ����ϵ�ʱ�䲻����
		return;
	}
	MR(ADDR_128KDATABUFF,ADDR_3106_6,LENmax_3106_6);
	p8=(u8*)ADDR_128KDATABUFF;
	p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
	if(p8==0)
	{
		return;
	}
	x=(p8[9]<<8)|p8[10];//ͣ���¼���ֹʱ��ƫ����ֵ�����ӣ� long-unsigned
	x*=60;
	t1=YYMDHMS_Sub_hex(PowerEvent->Tstarttime,pEVENT+8);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC2=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
	if(t1>>31)
	{
		t1=~t1;
		t1++;
	}
	if(t1<x)
	{
		t1=YYMDHMS_Sub_hex(PowerEvent->Tstoptime,pEVENT+16);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC2=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
		if(t1>>31)
		{
			t1=~t1;
			t1++;
		}
		if(t1<x)
		{
			t1=YYMDHMS_Sub_hex(PowerEvent->Tstarttime,PowerEvent->Tstoptime);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
			t2=YYMDHMS_Sub_hex(pEVENT+8,pEVENT+16);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
			if((t2>>31)==0)
			{
				t1-=t2;
				if(t1>>31)
				{
					t1=~t1;
					t1++;
				}
				x=(p8[12]<<8)|p8[13];//ͣ���¼�ʱ������ƫ����ֵ�����ӣ� long-unsigned
				x*=60;
				if(t1<=x)
				{
					PowerEvent->bit|=0x40;//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
				}
			}
		}
	}
}

//ͣ���¼������ֵ����  structure
//{
//ͣ��ʱ����С��Ч��������ӣ� long-unsigned��
//ͣ��ʱ�������Ч��������ӣ� long-unsigned��
//ͣ���¼���ֹʱ��ƫ����ֵ�����ӣ� long-unsigned��
//ͣ���¼�ʱ������ƫ����ֵ�����ӣ� long-unsigned��
//ͣ�緢����ѹ��ֵ  long-unsigned����λ��V�����㣺-1����
//ͣ��ָ���ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
//}
void Event_Power(void)//�ն�ͣ/�ϵ��¼�
{
	u32 i;
//	u32 x;
	u32 t;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(PowerEvent->RTCS==Comm_Ram->DL698YMDHMS[6])//�ϴμ���ʱRTC����
	{
		return;
	}
	
//---TEST---
//LCDDataOut();//���ݲ�����Զˢ����,�������������LCD��
//---TEST END---
	
	PowerEvent->RTCS=Comm_Ram->DL698YMDHMS[6];//�ϴμ���ʱRTC����
	switch(PowerEvent->task)
	{
		case 0://�ն˸�λ
			MR((u32)&PowerEvent->Tstarttime,ADDR_POWER_YMDHMS,7);//�����¼��ж��������ʱʱ��
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
			UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	#endif
	#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	#endif
			Terminal_Ram->Power_S_Timer=10;//38 ͣ���¼��붨ʱ��,ͬʱ��ͣ�絽��ʼ����ʱ;ע:���Ժ��������ϵ������ϱ��ȴ�ʱ��60��
			PowerEvent->task++;
			break;
		case 1://����ָ��ȶ���ʱ
			i=CompVoltage(0);//�Ƚ�ͣ���ѹ��ֵ,���:V=0�ָ���ѹ��ֵ,V=1������ѹ��ֵ;����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
			if(i)
			{
				if(Terminal_Ram->Power_S_Timer==0)//38 ͣ���¼��붨ʱ��
				{
					MW(ADDR_DL698YMDHMS,(u32)&PowerEvent->Tstoptime,7);//����ָ�ʱ��
				#ifdef IC_STM32F4xx
					if(RTC->BKP19R==0x12345678)//ͣ�����¼��ж����ϵ��־;���Ź���λ�Զ�����=0x12345678,����ֵΪ�ϵ�
					{//���Ź���λ
						RTC->BKP19R=0;//ͣ�����¼��ж����ϵ��־;���Ź���λ�Զ�����=0x12345678,����ֵΪ�ϵ�
						PowerEvent->task=100;
						break;
					}
					RTC->BKP19R=0;//ͣ�����¼��ж����ϵ��־;���Ź���λ�Զ�����=0x12345678,����ֵΪ�ϵ�
				#endif
				#ifdef IC_NUC9xx
					p8=(u8*)(ADDR_REG_SYS_RSTSTS);
					if(p8[0]&(1<<5))
					{//���Ź���λ
						p8[0]&=~(1<<5);
						PowerEvent->task=100;
						break;
					}
					p8[0]&=~(1<<5);
				#endif
					i=Check_pYYMDHMS_hex(PowerEvent->Tstarttime);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
					if(i==0)
					{
						i=Check_pYYMDHMS_hex(PowerEvent->Tstoptime);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
						if(i==0)
						{
							t=YYMDHMS_Sub_hex(PowerEvent->Tstarttime,PowerEvent->Tstoptime);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC2=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
							if((t>>31)==0)
							{
								MR(ADDR_128KDATABUFF,ADDR_3106_6,LENmax_3106_6);
								p8=(u8*)ADDR_128KDATABUFF;
								p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
								if(p8)
								{
									i=(p8[3]<<8)|p8[4];//ͣ��ʱ����С��Ч��������ӣ� long-unsigned
									i*=60;
									if(t>=i)
									{
										i=(p8[6]<<8)|p8[7];//ͣ��ʱ�������Ч��������ӣ� long-unsigned
										i*=60;
										if(t<i)
										{
											PowerEvent->bit=0x80;//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
											Terminal_Ram->Power_S_Timer=10;//��ʱ�����ܱ��¼(��)
											PowerEvent->task++;
											break;
										}
									}
								}
							}
						}
					}
					//��Чͣ���¼�
					PowerEvent->bit=0;//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
					Terminal_Ram->Power_S_Timer=0;//��ͣ���¼���ʱ
					PowerEvent->task=20;//д�ϵ��¼���¼
				}
				break;
			}
			Terminal_Ram->Power_S_Timer=10;//38 ͣ���¼��붨ʱ��;ע:���Ժ��������ϵ������ϱ��ȴ�ʱ��60��
			break;
		case 2://�����ܱ��¼��ʱ�ȴ�
			i=CompVoltage(0);//�Ƚ�ͣ���ѹ��ֵ,���:V=0�ָ���ѹ��ֵ,V=1������ѹ��ֵ;����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
			if(i==0)
			{
				PowerEvent->task=0;
				break;
			}
			if(Terminal_Ram->Power_S_Timer==0)//38 ͣ���¼��붨ʱ��
			{
				RM_Meter_Event3106();//ͣ/�ϵ��¼��賭�������������(��0��ʼ)
			#if (USER/100)==0//���Ժ����
				Terminal_Ram->Power_S_Timer=30;//��ͣ���¼���ʱ(��);ע:���Ժ��������ϵ������ϱ��ȴ�ʱ��60��
			#else
				if(PowerEvent->NUMrm==0)//ͣ/�ϵ��¼��賭��������
				{
					Terminal_Ram->Power_S_Timer=0;//��ͣ���¼���ʱ(��);ע:���Ժ��������ϵ������ϱ��ȴ�ʱ��60��
				}
				else
				{
					Terminal_Ram->Power_S_Timer=120;//��ͣ���¼���ʱ(��);ע:���Ժ��������ϵ������ϱ��ȴ�ʱ��180��
				}
			#endif
//				t=PowerEvent->NUMrm;//ͣ/�ϵ��¼��賭��������
//				for(i=0;i<t;i++)
//				{
//					p8=(u8*)ADDR_6000_SDRAM+(PowerEvent->setno[i]*LENper_6000);
//					p8=Get_Element(p8+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
//					if(p8)
//					{
//						x=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
//						if(x==0xF2090201)
//						{
//							//���ز���ѡ��
//							Terminal_Ram->Power_S_Timer=300;//��ͣ���¼���ʱ(��);ע:���Ժ��������ϵ������ϱ��ȴ�ʱ��60��
//							break;
//						}
//					}
//				}
				PowerEvent->task=10;//�����ܱ��¼
			}
			break;
		
		case 10://�����ܱ��¼
			i=CompVoltage(0);//�Ƚ�ͣ���ѹ��ֵ,���:V=0�ָ���ѹ��ֵ,V=1������ѹ��ֵ;����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
			if(i==0)
			{
				PowerEvent->task=0;
				break;
			}
			if(PowerEvent->bit&0x40)//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
			{//�¼���Ч
				Terminal_Ram->Power_S_Timer=0;
			}
			if(Terminal_Ram->Power_S_Timer==0)
			{
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
				UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
				UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;
			#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
				UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;
			#endif
			#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
				UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;
			#endif
				PowerEvent->task=20;//д�ϵ��¼���¼
				break;
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==2)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
			{
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
				if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==2)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
				{
				#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==2)
					{
				#endif
				#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==2)
					{
				#endif
						
						PowerEvent->task=20;//д�ϵ��¼���¼
						break;
						
				#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=		
					}
				#endif
				#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=		
					}
				#endif
				}
			}
			break;
			
		case 20://д�ϵ��¼���¼
			i=CompVoltage(0);//�Ƚ�ͣ���ѹ��ֵ,���:V=0�ָ���ѹ��ֵ,V=1������ѹ��ֵ;����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
			if(i==0)
			{
				PowerEvent->task=0;
				break;
			}
			
//			Comm_Ram->DisplayAllErrorTimer=5;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��			
//			DisplayStringAutoUp(0,9,(u8*)"\x0""�ϵ��¼���¼");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			
			if((PowerEvent->bit&0x80)==0)//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
			{
				PowerEvent->bit=0;//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
			}
			Event_31060200(0,0);//�ն�ͣ/�ϵ��¼���¼;���:f=���Ա�־,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			PowerEvent->task=100;
			break;
		
		case 100://��Դ����
			i=CompVoltage(1);//�Ƚ�ͣ���ѹ��ֵ,���:V=0�ָ���ѹ��ֵ,V=1������ѹ��ֵ;����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
			if(i==0)
			{
				Terminal_Ram->Power_S_Timer=10;//38 ͣ���¼��붨ʱ��;ע:���Ժ����������������ϱ��ȴ�ʱ��20��
				PowerEvent->task++;
			}
			MW(ADDR_DL698YMDHMS,ADDR_POWER_YMDHMS,7);//�����¼��ж��������ʱʱ��
			break;
		case 101://�������
			i=CompVoltage(1);//�Ƚ�ͣ���ѹ��ֵ,���:V=0�ָ���ѹ��ֵ,V=1������ѹ��ֵ;����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
			if(i)
			{
				PowerEvent->task=100;
			}
			else
			{
				if(Terminal_Ram->Power_S_Timer==0)//38 ͣ���¼��붨ʱ��
				{
					MW(ADDR_DL698YMDHMS,ADDR_POWER_YMDHMS,7);//�����¼��ж��������ʱʱ��
					MW(ADDR_DL698YMDHMS,(u32)&PowerEvent->Tstarttime,7);//�������ʱ��
					MC(0xff,(u32)&PowerEvent->Tstoptime,7);//����ָ�ʱ��
					PowerEvent->bit=0x80;//b7=�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������ж�,b6=�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���ж�
					Event_31060200(0,1);//�ն�ͣ/�ϵ��¼���¼;���:f=���Ա�־,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					if(Comm_Ram->DataSave==2)//���ݱ���:0=��,1=�����е糬��10�������豣��,2=�����ѱ���
					{//�����ѱ���
						Battery_Data_Save();//��ع���ʱ�ػ�ǰ���ݱ���
					}
					PowerEvent->task=0;
					
//					Comm_Ram->DisplayAllErrorTimer=5;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��			
//					DisplayStringAutoUp(0,9,(u8*)"\x0""ͣ���¼���¼");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				}
			}
			break;
		default:
			PowerEvent->task=0;
			break;
	}
}

//��ѹ�����½�����
//1.���ѹ * 65%;��ʱ�ȴ� 90 �� ;��ȷ��δ�����ն�ͣ�ϵ��¼�
//2.���ѹ * 55% ��ʱ�ȴ� 90 ��  ��ȷ�������ն�ͣ�ϵ��¼�
//3.���ѹ * 75% ��ʱ�ȴ� 90 ��  ��ȷ��δ�����ն�ͣ�ϵ��¼�
//4.���ѹ * 85% ��ʱ�ȴ� 90 ��  ��ȷ�������ն�ͣ�ϵ��¼�
//5.���ѹ * 100% ��ʱ�ȴ� 10 �� �����ն������ϱ�
//6.���ѹ * 40% ��ʱ�ȴ� 20 ��  ��ȷ���ն˷����ϱ�
//7.���ѹ * 100% ��ʱ�ȴ� 60 �� ��ȷ���ն˷����ϱ�
//8.���ѹ * 22%  ��ʱ�ȴ� 90 ��  ����ģ���,�����¼�
//9.���ѹ * 100% ��ʱ�ȴ� 180 ��  �¼���¼������ȷ��[2]  last 2��ȷ�����Ա�ʶΪ [ ��������Ч ];last 1��ȷ�����Ա�ʶΪ [ ��������Ч ]
//10.���ѹ * 22% ��ʱ�ȴ� 90 �� ����ģ�������¼�
//11.���ѹ * 100% ��ʱ�ȴ� 180 �� last 2��ȷ�����Ա�ʶΪ [ ��������Ч ];last 1��ȷ�����Ա�ʶΪ [ ��������Ч ]
//12.���ѹ * 22% ��ʱ�ȴ� 90 �� ����ģ�������¼�
//13.���ѹ * 100% ��ʱ�ȴ� 180 �� last 2��ȷ�����Ա�ʶΪ [ ��������Ч ];last 1��ȷ�����Ա�ʶΪ [ ��������Ч ]























