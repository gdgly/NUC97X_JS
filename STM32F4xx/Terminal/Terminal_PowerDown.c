
//�ն˵�Դ�½�
#include "Project.h"
#include "Terminal_PowerDown.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_MAC.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../Device/GPRS_3763.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"

#include "Terminal_Uart_Manage.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0D_CongealData.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"
#include "../Terminal/Terminal_ReadMeter_ACSample.h"
#include "Terminal_ReadMeter_RS485.h"
#include "Terminal_Uart_3762_Manage.h"
#include "Terminal_ReadMeter_3762.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "../MS/Data_Save.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"



	#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
#include "Terminal_Uart_AutoAFN0AF299.h"
	#endif


#if ((Project/100)<2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=ͨ��ģ��
void Terminal_PowerDown(void)//2=��������ת��ع���
{
}
#else
void Terminal_PowerDown(void)//2=��������ת��ع���
{
	//u32 i;
	u32 PORTn;
	u32 *p32;
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
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Ram->PowerDownTask)//��Դ�����������
	{
		case 0://�����¼�����
			Terminal_Ram->PowerTask=0;//ͣ���¼�����
			while(1)
			{
				p32=(u32*)(ADDR_HourCongeal_AutoRW);
				if(p32[3]==0)
				{//R2����,ֵ=0��ʾ�������
					p32=(u32*)(ADDR_Class2Congeal_AutoRW);
					if(p32[3]==0)
					{//R2����,ֵ=0��ʾ�������
					#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
						if(MRR(ADDR_ERC14SOUR,1)!=0)//ERC14�ϵ��¼��Ѽ�¼��־
					#else
						if(Terminal_Ram->PowerNote!=0)//�ϵ��¼���¼:0=û��¼,!=0�Ѽ�¼;
					#endif
						{
							ERC14_Event_Start(0xee);//�ն�ͣ/�ϵ��¼�(ͣ��);���:�ϵ��¼ʱ�¼�����ָ�����1��־,0=����1,0xee=���1
							break;
						}
						else
						{
							break;
						}
					}
				}
				HourCongeal_AutoRW();//Сʱ�����Զ�IO��д����0,��ɺ�������
	      Class2Congeal_AutoRW();//��2�����Զ�IO��д����0,��ɺ�������
				WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
			}
			Comm_Ram->BattWork_S_timer=60;//4 ��ع����ػ��붨ʱ��
			Terminal_Ram->PowerDownTask++;//��Դ�����������
			break;
		case 1://��鵱ǰ�Ƿ�����
			if(PORTn==0)
			{
				UART1Ctrl->LinkTask=1;//1=�ص�Դ
				UART1Ctrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				UnInit_GPRS_3763();//GPRSģ������Ϊ��������
				OffR5V0();//��R5V0��Դ
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				UARTCtrl->LinkTask=1;//1=�ص�Դ
				UARTCtrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				UnInit_STM32F4xx_MAC();
				OffE5V0();//��E5V0��Դ
				Terminal_Ram->PowerDownTask=50;//��Դ�����������.���ϱ�
			}
			else
			{
				Terminal_Ram->PowerDownTask++;//��Դ�����������
			}
			break;
		case 2://�ڶ�ʱ�ڵȴ��ϱ�
			if(Terminal_Ram->PowerDownSubTask==0)//��Դ�������������
			{
				//�رշ����ӿڵ�Դ
				Terminal_Ram->PowerDownSubTask=1;//��Դ�������������
				if(PORTn==GPRSPORT)
				{
			#if ((USER/100)==5)&&(Project==300)
			//�Ϻ�ר��SIMCOM 4Gģ��ʹ����3.3V��Դ�ʲ���
					
			#else
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
					UARTCtrl->LinkTask=1;//1=�ص�Դ
					UARTCtrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
					UnInit_STM32F4xx_MAC();
					OffE5V0();//��E5V0��Դ
			#endif
				}
				else
				{
					UART1Ctrl->LinkTask=1;//1=�ص�Դ
					UART1Ctrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
					UnInit_GPRS_3763();//GPRSģ������Ϊ��������
					OffR5V0();//��R5V0��Դ
				}
			}
			Terminal_Uart_Manage();//Terminal_Uart����
			Terminal_AFN0CF2_Computer();//�ն���1����F2,�ն�����ʱ��
		  if(Comm_Ram->DisplayScanTimer==0x0)
		  {//������ʾ���
		    Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
				DisplayClr();//����
				DisplayLoop_TopICO(0);//��ͼ���ʱ����ʾ
				DisplayLoop_BottomICO(9);//��ͼ����ʾ
				DisplayString(1,0,0,(u8 *)"����ͣ�������ϱ�...");//��ʾ�ַ���
  			LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
			}
    	if(Pin_Read(PIN_WKUP))
    	{
			RESET:
				//RAM������Ч
				p32=(u32*)((ADDR_Meter_SaveRam_Start)+LEN_SaveRam-8);
				p32[0]=0;
				p32[1]=0;
				//���ŵ�Ӳ����λ��־�Բ����¼�
				RTC->BKP0R&=0xffff00ff;//B8-B15 0x55=WWDT���Ź���λ��־,����ֵΪ���ŵȸ�λ
				__disable_irq();
				while(IWDG->SR!=0)
				{
				}
				IWDG->KR=0x5555;
				IWDG->PR=0;//4 ��Ƶ
				IWDG->RLR=1;
				IWDG->KR=0xAAAA;
				IWDG->KR=0xCCCC;
				while(1);
			}
			if(Comm_Ram->BattWork_S_timer==0)//��ع����ػ��붨ʱ��
			{
				Terminal_Ram->PowerDownTask++;//��Դ�����������
			}
			break;
		case 3://���˳���¼
			Comm_Ram->BattWork_S_timer=10;//�ȴ������˳���¼��ʱ
			Terminal_Uart_Manage();//Terminal_Uart����
			UARTCtrl->LinkTask=102;
			Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 ��̫�������붨ʱ��
			Terminal_Ram->GPRSHeartBeat_S_Timer=0;//5 GPRS�����붨ʱ��
			Terminal_Ram->PowerDownTask++;//��Դ�����������
			break;
		case 4://�ȴ��˳���¼
			Terminal_Uart_Manage();//Terminal_Uart����
		  if(Comm_Ram->DisplayScanTimer==0x0)
		  {//������ʾ���
		    Comm_Ram->DisplayScanTimer=1000/10;
				DisplayClr();//����
				DisplayLoop_TopICO(0);//��ͼ���ʱ����ʾ
				DisplayLoop_BottomICO(9);//��ͼ����ʾ
				DisplayString(1,0,0,(u8 *)"����ͣ���˳���¼...");//��ʾ�ַ���
  			LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
			}
    	if(Pin_Read(PIN_WKUP)!=0)
    	{
				goto RESET;//����ʹ�ܿ��Ź���λ
			}
			if(Comm_Ram->BattWork_S_timer==0)//��ع����ػ��붨ʱ��
			{
				Battery_Data_Save();//��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
			}
			if(UARTCtrl->LinkTask==103)
			{
				Terminal_Ram->PowerDownTask++;//��Դ�����������
			}
			break;
		case 5://�ػ�
			UART1Ctrl->LinkTask=104;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			UARTCtrl->LinkTask=104;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			Terminal_Ram->PowerDownTask++;//��Դ�����������
			break;
		case 6://����
			Terminal_Uart_Manage();//Terminal_Uart����
    	if(Pin_Read(PIN_WKUP))
    	{
				goto RESET;//����ʹ�ܿ��Ź���λ
			}
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
	    Comm_Ram->DisplayScanTimer=1000/10;
			DisplayClr();//����
			DisplayString(1,0,0,(u8 *)"����ͣ��");//��ʾ�ַ���
			LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
			Comm_Ram->BattWork_S_timer=5;//4 ��ع����ػ��붨ʱ��
			Terminal_Ram->PowerDownTask++;//��Դ�����������
			break;
		case 51://�ȴ���ʱ��ػ�
    	if(Pin_Read(PIN_WKUP))
    	{
				goto RESET;//����ʹ�ܿ��Ź���λ
			}
			if(Comm_Ram->ButtonVal&0x3f)//����״̬(ʹ�ú���0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=����,B8=���,B9=�˸�
    	{
				goto RESET;//����ʹ�ܿ��Ź���λ
			}
			if(Comm_Ram->BattWork_S_timer==0)//��ع����ػ��붨ʱ��
			{
				Battery_Data_Save();//��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
			}
			break;
			
		default:
			Terminal_Ram->PowerDownTask=0;//��Դ�����������
			break;
	}
}

#endif//#if ((Project/100)<2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=ͨ��ģ��




#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
u32 VoltageComp(u32 V)//�����ѹ�Ƚ�,����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
{
	u32 i;
	u16 *p16;
	GetACSample(0x0201ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
	p16=(u16 *)(Get_ADDR_UARTnTx(ACSAMPLEPORT)+14);
	for(i=0;i<3;i++)
	{
		if(p16[i]>=V)
		{
			break;
		}
	}
	if(i>=3)
	{
		return 0;
	}
	return 1;
}

u32 EventTimerComp(void)//ͣ���¼�ʱ��Ƚ�;����0=ʱ����ͬ,1=��ͬ���б�û��,2=��ͬ��ȫ�����ѳ�
{
	u32 i;
	u32 x;
	u32 z;
	u32 t1;
	u32 t2;
	u8 *p8;
	u16 *p16;
	
	p8=(u8*)(ADDR_DATABUFF);
	p16=(u16*)(ADDR_F246PnList);//Ҫ�����ĵ����¼����Pn�б�
	z=0;//�ѳ�����
	for(i=0;i<F246MaxPn;i++)
	{
		if(p16[i]==0)
		{//����
			z++;//�ѳ�����
		}
		else
		{//Ҫ��
			MR(ADDR_DATABUFF,ADDR_AFN0CF246_Sec+3+(15*i),6);//������ʱ��
			if(p8[1]==0xee)
			{//û����
				continue;
			}
			MR(ADDR_DATABUFF+6,ADDR_AFN0CF246_Sec+3+(15*i)+6,6);//����ϵ�ʱ��
			t1=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
			if(t1!=0)
			{//��ʼʱ��Ƿ�
				continue;
			}
			t1=Check_YMDHMS(MRR(ADDR_DATABUFF+6,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
			if(t1!=0)
			{//����ʱ��Ƿ�
				continue;
			}
			z++;//�ѳ�����
			
			x=MRR(ADDR_AFN04F98+4,2);//ͣ���¼���ֹʱ��ƫ����ֵ	BIN	2	��λΪ����
			x*=60;
			t1=TimeDifference(ADDR_ERC14SOUR+16,ADDR_DATABUFF);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
			if(t1>>31)
			{
				t1=~t1;
				t1++;
			}
			if(t1<x)
			{
				t1=TimeDifference(ADDR_ERC14SOUR+22,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(t1>>31)
				{
					t1=~t1;
					t1++;
				}
				if(t1<x)
				{
					t1=TimeDifference(ADDR_ERC14SOUR+16,ADDR_ERC14SOUR+22);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
					t2=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����
					if(t2>>31)
					{//��ʼʱ��-����ʱ�䵹�÷Ƿ�
						continue;
					}
					t1-=t2;
					if(t1>>31)
					{
						t1=~t1;
						t1++;
					}
					x=MRR(ADDR_AFN04F98+6,2);//ͣ���¼�ʱ������ƫ����ֵ	BIN	2	��λΪ��
					if(t1<x)
					{
						return 0;//��Чͣ��
					}
				}
			}
		}
	}
	if(z<F246MaxPn)
	{//ûȫ������
		return 1;//1=��ͬ���б�û��
	}
	return 2;//2=��ͬ��ȫ�����ѳ�
}

void ERC14_Event(void)//�ն�ͣ/�ϵ��¼�
{
	u32 i;
	u32 x;
	u16 *p16;
	UARTCtrl_TypeDef * UARTCtrl;
#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
	u32 *p32;
#endif
	
#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
	//�ն˶�ʱ��λ
	i=Terminal_Ram->PowerTask;//ͣ���¼�����
	if((i==2)||(i>=99))
	{
		x=MRR(ADDR_TERMINAL,2);//ȡ�ն˵�ַ
		i=(x%7);//����
		if(i==Comm_Ram->TYMDHMS[6])//��ǰ��0-6
		{
			i=(x%40);//ÿ5����Ϊ��λ
			i*=5;
			x=i/60;
			x+=20;//��ÿ��20:00-23:20ʱ���
			i%=60;
			x=hex_bcd(x);
			i=hex_bcd(i);
			x<<=8;
			x|=i;
			if(Comm_Ram->TYMDHMS[0]<=0x05)
			{//��ǰ��ֵ��0-5��
				i=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);
				if(i==x)
				{//ʱ����ͬ
					p32=(u32*)(ADDR_HourCongeal_AutoRW);
					while(p32[3]!=0)
					{//R2����,ֵ=0��ʾ�������
						HourCongeal_AutoRW();//Сʱ�����Զ�IO��д����0,��ɺ�������
						WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
					}
					p32=(u32*)(ADDR_Class2Congeal_AutoRW);
					while(p32[3]!=0)
					{//R2����,ֵ=0��ʾ�������
						Class2Congeal_AutoRW();//��2�����Զ�IO��д����0,��ɺ�������
						WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
					}
					SoftDelayMS(5000);//���ms��ʱ
					WWDT_RESET();//����ʹ�ܿ��Ź�
				}
			}
		}
	}
#endif
	
	i=((RTC->BKP0R>>8)&0xff);//B8-B15 0x55=WWDT���Ź���λ��־,����ֵΪ���ŵȸ�λ
	if(i!=0x55)
	{//���ŵ�Ӳ����λ
		switch(Terminal_Ram->PowerTask)//ͣ���¼�����
		{
			case 0://���絽�ϵ�
				i=MRR(ADDR_AFN04F98+8,2);//�ն�ͣ�緢����ѹ��ֵ
				i=VoltageComp(i);//�����ѹ�Ƚ�,����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
				if(i!=0)
				{
					if(Terminal_Ram->Power_S_Timer==0)//38 ͣ���¼��붨ʱ��
					{
						//�峭���ĵ��ͣ�ϵ��¼
						MC(0xee,ADDR_AFN0CF246_Sec,15*F246MaxPn);//���1�ε����¼(��ȷ����)
						//RS485-1��ʼ
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
						ClrRMTaskCompletes_RS485(RS485_1PORT,0,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
						UARTCtrl->RMTaskC=0;//��ǰ���������0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						UARTCtrl->RMTaskFlags=1;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
						UARTCtrl->Task=0;
						UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
						UARTCtrl->FnCount=0;//485����Fn����
						UARTCtrl->RMCount=0;//485�����������
						UARTCtrl->ReRMCount=0;//485���������ط�����
						UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
						UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
						UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
						UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����
						UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
						UARTCtrl->BroadcastTime=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						//RS485-2��ʼ
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
						ClrRMTaskCompletes_RS485(RS485_2PORT,0,1);//��˿ڳ���������ɱ�־;���:PORTn=�˿ں�,RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
						UARTCtrl->RMTaskC=0;//��ǰ���������0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						UARTCtrl->RMTaskFlags=1;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
						UARTCtrl->Task=0;
						UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
						UARTCtrl->FnCount=0;//485����Fn����
						UARTCtrl->RMCount=0;//485�����������
						UARTCtrl->ReRMCount=0;//485���������ط�����
						UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
						UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
						UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
						UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����
						UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
						UARTCtrl->BroadcastTime=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						//RS485-4(�ز�)��ʼ
				#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
						ClrRMTaskComplete_3762(0,1);//��˿ڳ���������ɱ�־;RMTask=�����,All=0ֻ��ʧ��,=1��ȫ��
						UARTCtrl->RMTaskC=0;//��ǰ���������0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						UARTCtrl->RMTaskFlags=1;//��ǰ�����������ݷ���(ͬ�����):B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
						//UARTCtrl->Task=0;
						UARTCtrl->MeterNo=0;//485������ܱ�/��������װ�����(���ֶ���)
						UARTCtrl->FnCount=0;//485����Fn����
						UARTCtrl->RMCount=0;//485�����������
						UARTCtrl->ReRMCount=0;//485���������ط�����
						UARTCtrl->RMError=0;//485�������ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
						UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
						UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
						UARTCtrl->AutoSearchTask=0;//�Զ��ѱ�����
						UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
						UARTCtrl->BroadcastTime=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						for(i=0;i<MaxRS485AddCarrierPn;i++)
						{
							Terminal_Note->List[i].FnCount=0;//����Fn����
							Terminal_Note->List[i].RMCount=0;//�������ݱ�ʶ����
							Terminal_Note->List[i].ReRMCount=0;//�ط�����
							Terminal_Note->List[i].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
						}
						if(Terminal_Router->RouterInitTask>=200)//·�ɳ�ʼ��
						{
							Terminal_Router->RouterInitTask=0;//·�ɳ�ʼ��
						}
						UARTCtrl->RMTaskN|=0x2;//ÿ������
				#endif
						i=MRR(ADDR_AFN04F97+2,1);//ͣ���¼�����ʱ����ֵ	BIN	1	��λΪ����
						i*=60;
						p16=(u16*)(&Terminal_Ram->RS4851RMF246_S_Timer);//25 RS485-1ͣ���¼�����ʱ����ֵ�붨ʱ��(�����ϳ���ʱ�����ֿ�)
						p16[0]=i;//RS485-1
						p16[1]=i;//RS485-2
						p16[2]=i;//RS485-3
				#if (USER/100)==0//���Ժ����
						//�ز�ͨ������������
						p16[3]=0;//RS485-4(�ز�)
				#else
						p16[3]=i;//RS485-4(�ز�)
				#endif
						i=MRR(ADDR_AFN04F98+10,2);//�ն�ͣ��ָ���ѹ��ֵ
						i=VoltageComp(i);//�����ѹ�Ƚ�,����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
						if(i!=0)
						{
						#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
							MR(ADDR_ERC14SOUR+16,ADDR_POWER_YMDHMS,6);//ERC14�ϵ��¼��ͣ��ʱʱ��:������ʱ����
							if(MRR(ADDR_AFN04F297_Ext,1)!=0)
							{//�ն˾�ȷ��ʱ���ؿ���
								if(MRR(ADDR_ERC14SOUR+22,6)==0)
								{
									MR(ADDR_ERC14SOUR+22,ADDR_TYMDHMS,6);//ERC14�ϵ��¼���ϵ�ʱʱ��:������ʱ����
								}
								if(Terminal_Ram->RMDelay_S_Timer==0)//�ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
								{
									if((UART1Ctrl->LinkTask!=101)&&(UART10Ctrl->LinkTask!=101))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
									{
										Terminal_Ram->PowerTask=1;//ͣ���¼�����
									}
								}
								else
								{
									if(UART1Ctrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
									{
										//������վʱ��
										Terminal_AutoAFN0AF299(GPRSPORT);//�������㽭��չAFN0AF299�ն�������վʱ��
									}
									if(UART10Ctrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
									{
										//������վʱ��
										Terminal_AutoAFN0AF299(ENETPORT);//�������㽭��չAFN0AF299�ն�������վʱ��
									}
								}
							}
							else
							{
								MR(ADDR_ERC14SOUR+22,ADDR_TYMDHMS,6);//ERC14�ϵ��¼���ϵ�ʱʱ��:������ʱ����
								Terminal_Ram->PowerTask=1;//ͣ���¼�����
							}
						#else
							MR(ADDR_ERC14SOUR+16,ADDR_POWER_YMDHMS,6);//ERC14�ϵ��¼��ͣ��ʱʱ��:������ʱ����
							MR(ADDR_ERC14SOUR+22,ADDR_TYMDHMS,6);//ERC14�ϵ��¼���ϵ�ʱʱ��:������ʱ����
							Terminal_Ram->PowerTask=1;//ͣ���¼�����
						#endif
						}
						else
						{
							Terminal_Ram->PowerTask=99;//ͣ���¼�����;99=�ȴ���ѹ���ָ�ֵ
						}
					}
					break;
				}
				Terminal_Ram->Power_S_Timer=5;//38 ͣ���¼��붨ʱ��
			#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
				if(MRR(ADDR_AFN04F297_Ext,1)==0)
				{//�ն˾�ȷ��ʱ���عر�
					Terminal_Ram->RMDelay_S_Timer=10;//10 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
				}
			#else
				Terminal_Ram->RMDelay_S_Timer=10;//10 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
			#endif
				break;
			case 1://����ͣ��ʱ��
				x=Check_YMDHMS(MRR(ADDR_ERC14SOUR+16,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
				if(x==0)
				{
					x=Check_YMDHMS(MRR(ADDR_ERC14SOUR+22,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
					if(x==0)
					{
						i=TimeDifference(ADDR_ERC14SOUR+16,ADDR_ERC14SOUR+22);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
						if((i>>31)==0)
						{
							x=MRR(ADDR_AFN04F98,2);//ͣ��ʱ����С��Ч�����
							x*=60;
							if(i>=x)
							{//������С��Ч�����
								x=MRR(ADDR_AFN04F98+2,2);//ͣ��ʱ�������Ч���	BIN	2	��λΪ����
								x*=60;
								if(i<x)
								{//С��ͣ��ʱ�������Ч���
									Terminal_Ram->F246PnListUpDate=0xFF;//Ҫ�����ĵ����¼����Pn�б��Ѹ��±�־,0=Ҫ����,!=0�Ѹ���
									F246PnList();//Ҫ������F246�����¼����Pn�б�
								#if ((USER/100)==7)//�û���ʾ���㽭	
									ERC14_Event_End(1);//�ն�ͣ/�ϵ��¼�(�ϵ�);���:���Ա�־
									Terminal_Ram->PowerNote=0xff;//�ϵ��¼
								#endif
									Terminal_Ram->PowerTask=10;//ͣ���¼�����
									break;
								}
							}
						}
					}
				}
				//D0=����쳣,D1=�ȶ���Ч
				ERC14_Event_End(0);//�ն�ͣ/�ϵ��¼�(�ϵ�);���:���Ա�־
				Terminal_Ram->PowerNote=0xff;//�ϵ��¼
				Terminal_Ram->PowerTask=100;//ͣ���¼�����
				break;
			case 2://�е絽ͣ��
				i=MRR(ADDR_AFN04F98+8,2);//�ն�ͣ�緢����ѹ��ֵ
				i=VoltageComp(i);//�����ѹ�Ƚ�,����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
				if(i!=0)
				{
					Terminal_Ram->PowerTask=100;//ͣ���¼�����
					break;
				}
				if(Terminal_Ram->Power_S_Timer==0)//38 ͣ���¼��붨ʱ��
				{//ͣ��
					ERC14_Event_Start(0);//�ն�ͣ/�ϵ��¼�(ͣ��);���:�ϵ��¼ʱ�¼�����ָ�����1��־,0=����1,0xee=���1
				
				#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
				  //���ز�·�ɵ�Դ
					i=				0|//B3-B0:���ù���ѡ��AF(0-15)
							 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
							 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
							 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
							 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
							(0<<11);//B11  :��Ϊ���ʱ��0=���0,1=���1	
					Pin_Configure(PIN_UART4_RX,i);//RXD����,��������
					Pin_Configure(PIN_UART4_TX,i);//TXD����,��������
					Pin_Configure(PIN_PLC_RST,i);//PLC_RST,��������
					Pin_Configure(PIN_PLC_PWR,i);//PLC_PWR,��������
					Pin_Configure(PIN_PLC_ID,i);//PLC_ID,��������
					Terminal_Router->RouterInitTask=0;//·�ɳ�ʼ��
				#endif
					Terminal_Ram->PowerNote=0;//�ϵ��¼
					Terminal_Ram->PowerTask=0;//ͣ���¼�����
				}
				break;
				
			//ERC14��Դ״̬<10����
			case 10://10=�ȴ�������ɱȶԵ��ܱ�
				i=MRR(ADDR_AFN04F98+10,2);//�ն�ͣ��ָ���ѹ��ֵ
				i=VoltageComp(i);//�����ѹ�Ƚ�,����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
				if(i==0)
				{
					Terminal_Ram->PowerTask=99;//�ȴ���ѹ���ָ�ֵ
					break;
				}
				i=EventTimerComp();//ͣ���¼�ʱ��Ƚ�;����0=ʱ����ͬ,1=��ͬ���б�û��,2=��ͬ��ȫ�����ѳ�
				if(i==0)
				{
					ERC14_Event_End(3);//�ն�ͣ/�ϵ��¼�(�ϵ�);���:���Ա�־
					Terminal_Ram->PowerNote=0xff;//�ϵ��¼
					Terminal_Ram->PowerTask=100;//ͣ���¼�����
					break;
				}
				if(i==1)
				{//û���
					//485-1
					if((Comm_Ram->Factory==0x55)||(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)==0))//0x55=����ģʽ
					{
					}
					else
					{
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
						if(UARTCtrl->RMTaskC==0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						{
							break;//û����ϵ糭��
						}
					}
					//485-2
					if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)==0)
					{
					}
					else
					{
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
						if(UARTCtrl->RMTaskC==0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						{
							break;//û����ϵ糭��
						}
					}
					//�ز�
				#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(Terminal_Router->RouterInitTask<200)
					{
						break;
					}
					if(Terminal_Router->NoteNum!=0)
					{//�ڵ��Ϊ��
						UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
						if(UARTCtrl->RMTaskC==0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
						{
							break;//û����ϵ糭��
						}
					}
				#endif
				}
				//��Чͣ��
			#if ((USER/100)==7)//�û���ʾ���㽭	
				ERC14_Event_End(0x81);//�ն�ͣ/�ϵ��¼�(�ϵ�);���:���Ա�־,�㽭��Ҫ�¼������Ӳ��ϱ���־����ʶ��(D8λ)
			#else
				ERC14_Event_End(1);//�ն�ͣ/�ϵ��¼�(�ϵ�);���:���Ա�־
			#endif
				Terminal_Ram->PowerNote=0xff;//�ϵ��¼
				Terminal_Ram->PowerTask=100;//ͣ���¼�����
				break;
				
				
			case 99://�ȴ���ѹ���ָ�ֵ
				i=MRR(ADDR_AFN04F98+10,2);//�ն�ͣ��ָ���ѹ��ֵ
				i=VoltageComp(i);//�����ѹ�Ƚ�,����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
				if(i!=0)
				{
					if(Terminal_Ram->Power_S_Timer==0)//38 ͣ���¼��붨ʱ��
					{
						Terminal_Ram->RMDelay_S_Timer=10;//10 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
						Terminal_Ram->PowerTask=0;//ͣ���¼�����
					}
					break;
				}
				Terminal_Ram->Power_S_Timer=5;//38 ͣ���¼��붨ʱ��
				break;
			case 100://ͣ���ж�
				i=MRR(ADDR_AFN04F98+8,2);//�ն�ͣ�緢����ѹ��ֵ
				i=VoltageComp(i);//�����ѹ�Ƚ�,����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
				if(i==0)
				{//ͣ��
					Terminal_Ram->Power_S_Timer=5;//38 ͣ���¼��붨ʱ��
					Terminal_Ram->PowerTask=2;//ͣ���¼�����
				}
				break;
			default:
				Terminal_Ram->PowerTask=100;//ͣ���¼�����
				break;
		}
	}
	else
	{//���Ź���λ
		i=MRR(ADDR_AFN04F98+10,2);//�ն�ͣ��ָ���ѹ��ֵ
		i=VoltageComp(i);//�����ѹ�Ƚ�,����:0=��С�ڲα�ֵ,1=����һ����ڲα�ֵ
		if(i!=0)
		{
			//ͣ���¼�����ʱ����ֵ��ʱ��=0��,=������
			
			//RS485-1��ʼ
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
			//RS485-2��ʼ
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
			UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->RMFlag=1;//0=��������ͣ,1=���������
	#endif
	
			RTC->BKP0R&=0xffff00ff;//B8-B15 0x55=WWDT���Ź���λ��־,����ֵΪ���ŵȸ�λ
			Terminal_Ram->PowerNote=0xff;//�ϵ��¼���¼:0=û��¼,!=0�Ѽ�¼;
			Terminal_Ram->PowerTask=100;//ͣ���¼�����
			return;
		}
		Terminal_Ram->Power_S_Timer=5;//38 ͣ���¼��붨ʱ��
		Terminal_Ram->RMDelay_S_Timer=10;//10 �ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
	}
}


#endif




//#define ADDR_AFN04F97     ADDR_AFN0CF3+32//ͣ�����ݲɼ����ò���P0
//ͣ�����ݲɼ���־	BS8	1	
//ͣ���¼�����ʱ����	BIN	1	��λΪСʱ
//ͣ���¼�����ʱ����ֵ	BIN	1	��λΪ����
//��Ҫ��ȡͣ���¼����ܱ����n	BIN	2	
//�������1	BIN	2	��1����Ҫ�����ĵ��ܱ�
//����	����	����	����
//�������n	BIN	2	��n����Ҫ�����ĵ��ܱ�
//#define ADDR_AFN04F98     ADDR_AFN04F97+5+(2*MaxRS485AddCarrierPn)//ͣ���¼������ֵ����p0
//ͣ��ʱ����С��Ч���	BIN	2	��λΪ����
//ͣ��ʱ�������Ч���	BIN	2	��λΪ����
//ͣ���¼���ֹʱ��ƫ����ֵ	BIN	2	��λΪ����
//ͣ���¼�ʱ������ƫ����ֵ	BIN	2	��λΪ��
//�ն�ͣ�緢����ѹ��ֵ	����¼A.7	2	V
//�ն�ͣ��ָ���ѹ��ֵ	����¼A.7	2	V
//ͣ���¼������ϱ���־λ	BS8	1	










