
#include "Project.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_FMC.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"

#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayError.h"
#include "../Display/DisplayFTP.h"
#include "../Display/DisplayLoop.h"

#include "../Device/MEMRW.h"
#include "Terminal_PowerDown.h"
#include "Terminal_Init.h"
#include "Terminal_Timer.h"
#include "Terminal_Uart_Manage.h"
#include "Terminal_Uart_3761.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0D_CongealData.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Statistic.h"
#include "Terminal_Contrl_Manage.h"
#include "Average_Voltage.h"
#include "../Test/Test_RAM.h"



uint32_t SystemCoreClock = 16000000;//��stm32f4xx_hal_rcc.c�ļ�����ʱ��Ҫ,linkʱ����Ҫ
void _0(void)
{
}
void ERC14_Event(void);//�ն�ͣ/�ϵ��¼�

void RunLED(void)//����LEDָʾ��
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	
	i=0;
	if((UART1Ctrl->LinkTask==101)||(UART1Ctrl->LinkTask==102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
	{
		i=1;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if((UARTCtrl->LinkTask==101)||(UARTCtrl->LinkTask==102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
	{
		i=1;
	}
	if(i==0)
	{
		Pin_OutL(PIN_RUNLED);
	}
	else
	{
		if(Comm_Ram->TYMDHMS[0]&0x1)
		{
			Pin_OutH(PIN_RUNLED);
		}
		else
		{
			Pin_OutL(PIN_RUNLED);
		}
	}
}


void Terminal_Task(void)//I2��ʱ����(�����е�)
{
//	u32 i;
	u8 *p8;
	u32 *p32;
	
	p8=(u8*)(ADDR_I2Task);//I2���������(ԭ��SDRAM,��SDRAM�������޷�����������SDRAM�������Ŀ��ܹʸķ���IRAM)
  switch(p8[0])
  {
    case 0:
      MW(ADDR_TYMDHMS,ADDR_TFM_YMDHMS,6);//����ǰʱ��
		#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
			if((Terminal_Ram->PowerTask==10)||(Terminal_Ram->PowerTask==100))//ͣ���¼�����
			{//ERC14��Դ״̬<10����
				MW(ADDR_TYMDHMS,ADDR_POWER_YMDHMS,6);//ERC14�¼��ж�����ǰ��ʱ��
			}
		#endif
			Terminal_FMTimer();//���籣����ͷֶ�ʱ��
			Terminal_Timer();//�ն˷��ж���ͷֶ�ʱ��
			RunLED();//����LEDָʾ��
      break;
		case 1://�ն�ͣ�ϵ��¼�
	#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
			if(Terminal_Ram->RMDelay_S_Timer==0)//�ϵ����ʱ�����붨ʱ��(ͬʱ�����ϵ��¼��ɼ�¼��ʱ)
			{
				if(MRR(ADDR_ERC14SOUR,1)==0)//ERC14�¼���¼��־(�ϵ�)
				{
					MWR(0x14,ADDR_ERC14SOUR,1);//ERC14�¼���¼��־
					if((RTC->BKP0R>>8)&0xff)!=0x55)//B8-B15 0x55=WWDT���Ź���λ��־,����ֵΪ���ŵȸ�λ
					{
						ERC14_Event_End();//�ն�ͣ/�ϵ��¼�(�ϵ�),��ר���¼����ݻ���
					}
				}
			}
	#endif
	#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
			ERC14_Event();//�ն�ͣ/�ϵ��¼�
	#endif
			break;
    case 2:
      Terminal_Init_Command();//�ն˳�ʼ������
      break;
    case 3:
			Terminal_Class1Data_Computer();//�ն���1���ݼ���
			break;
		case 4:
			HourCongeal_AutoRW();//Сʱ�����Զ�IO��д����0,��ɺ�������
			break;
		case 5:
			Terminal_Class2Data_Computer();//�ն���2���ݼ���
			break;
		case 6:
			Class2Congeal_AutoRW();//��2�����Զ�IO��д����0,��ɺ�������
			break;
		case 7:
			Terminal_Statistic();//�ն�ͳ�Ƽ���,����������
			break;
		case 8:
			Terminal_Contrl_Manage();//�ն˿��ƹ���(����������)
			break;
		case 9:
			Average_Voltage();//�ڲ�����1����ƽ����ѹ��������߶���
			break;
		case 10:
			ERC10_Event();//��ѹ��·�쳣(���ࡢʧѹ)	
			break;

    default:
			//SDRAM�������
			if(RAM_Test(ADDR_128KDATABUFF,256))//RAM����;����:0=��ȷ,1=����
			{
				Comm_Ram->DisplayAllErrorTimer=3;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
				DisplayStringAutoUp(0,9,(u8*)"\x0""SDRAM ERROR");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				Comm_Ram->GPMSTimer=1000/10;//18 ͨ��(������UART11) ÿ10MS��1��ʱ��
				while(1)
				{
					SDRAM_FMC_Init();
					if(RAM_Test(ADDR_128KDATABUFF,128*1024)==0)//RAM����;����:0=��ȷ,1=����
					{
						break;
					}
					if(Comm_Ram->GPMSTimer==0)//18 ͨ��(������UART11) ÿ10MS��1��ʱ��
					{
						__disable_irq();
						p32=(u32*)(ADDR_Meter_SaveRam_Start+LEN_SaveRam-8);
						p32[0]=0x12345678;
						p32[1]=RAM_CRC32(ADDR_Meter_SaveRam_Start,LEN_SaveRam-4);//RAM������CRC32����
						RTC->BKP0R&=0xffff00ff;//B8-B15 0x55=WWDT���Ź���λ��־,����ֵΪ���ŵȸ�λ
						RTC->BKP0R|=0x5500;
						//	RCC->CSR|=1;//LSI RC ��������
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
				}
			}
/*			
			//nand���
			p32=(u32*)(ADDR_128KDATABUFF);
//			for(i=0;i<(128*1024/4);i++)
//			{
//				p32[i]=i;
//			}
//			MW(ADDR_128KDATABUFF,ADDR_Download_ENTRY,128*1024);
			MR(ADDR_128KDATABUFF,ADDR_Download_ENTRY,128*1024);
			for(i=0;i<(128*1024/4);i++)
			{
				if(p32[i]!=i)
				{
					break;
				}
			}
			if(i<(128*1024/4))
			{
				Comm_Ram->DisplayAllErrorTimer=3;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
				DisplayStringAutoUp(0,9,(u8*)"\x0""NAND ERROR");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			}
*/
			//
      p8[0]=0;
      return;
  }
  p8[0]++;
}


__asm void Call_E2(u32 r0)//����E2�˵�,���R0=PC��ַ
{
	PUSH {LR}
	LDR LR,=ADDR_SystemStack
	STR SP,[LR]
	CMP R0,#0
	BNE Call_E2_1
	LDR R0,=ADDR_Menu_ENTRY//E2��ڵ�ַ
	LDR R0,[R0,#(4*1)];
Call_E2_1
	LDR LR,=Call_E2_End
	BX R0
Call_E2_End
	LDR LR,=ADDR_SystemStack
	MOV R0,#0
	STR R0,[LR]
	POP {PC}
	ALIGN
}

void Terminal_DisplayTask(void)//I2��ʾ
{
	//�˵����ն�ѭ��
#if (IC_UC1698|IC_ST75161)//0=û,1=��
	u32 i;
  u32 * p32;
#if (USER/100)==5//�Ϻ�
	u8 *p8;
#endif

  if(MenuCount->Count==0)//�˵���ջ����
  {//�˵���ջ����=0
    if((Comm_Ram->ButtonVal&0x10)==0)
    {//��ȷ�ϼ�����,ѭ��
		  if(Comm_Ram->DisplayScanTimer==0x0)
		  {
				if(((RTC->BKP0R&0xff)>=2)&&((RTC->BKP0R&0xff)<=4))//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
				{//USB�ļ���ɱ�־��Ч
					Comm_Ram->DisplayAllErrorTimer=0;
					Comm_Ram->DisplayScanTimer=5000/10;
					DOTMIXPMODE->Task=1;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
					DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
				}
				else
				{
					i=MRR(ADDR_FTPFileFlags+4,1);
		      if((i<(MaxFileNum+3))&&(Comm_Ram->DisplayAllErrorTimer!=0))
					{//FTP�ļ�������Ч
						DOTMIXPMODE->Task=1;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
						DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
						DisplayFTP();//FTP�ļ�������ʾ
					}
					else
					{
						if((i==(MaxFileNum+2))&&(Comm_Ram->DisplayAllErrorTimer==0))
						{
							MWR(MaxFileNum+3,ADDR_FTPFileFlags+4,1);//FTP�ļ���������ʾ�����־
						}
					#if ErrorAutoDisplay==1//0=����,1=��ֹ;ϵͳ�����Զ�������ʾ
						if(Comm_Ram->Factory!=0x55)//0x55=����״̬
						{//���ǹ���״̬
							if(Comm_Ram->ErrorIC)
							{
								Comm_Ram->ErrorIC=0;
								Comm_Ram->DisplayAllErrorTimer=0;
							}
						}
					#endif
						if((Comm_Ram->ErrorIC)&&(Comm_Ram->DisplayAllErrorTimer))
						{//������ʾ
							DOTMIXPMODE->Task=2;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
							DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
							DisplayError();//Ӳ�������ģ�������ʾ
						}
						else
						{
						//ѭ����ʾ
							DOTMIXPMODE->Task=4;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
							DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
							DisplayLoop();//�ն�ѭ��
						}
					}
				}
				//������ʾ���
				if(DOTMIXPMODE->Task==0)
				{//�ϵ���Ļ
					Comm_Ram->DisplayScanTimer=30/10;
				}
				else
				{//������Ļ
		    	Comm_Ram->DisplayScanTimer=1100/10;
				}
  			LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
		  }
    }
    else
    {//ȷ�ϼ�����,�����˵�
			DOTMIXPMODE->Task=3;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
			DOTMIXPMODE->Mix=2;//2;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
      Comm_Ram->ButtonVal&=~0x10;//ȷ�ϼ�ʹ�ú����
      if(FileDownload->File&0x10)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
      {
        MenuCount->Count=1;
        p32=(u32 *)(ADDR_Menu_ENTRY);
        Stack->MenuStack[MenuCount->Count].PC=p32[1];
        Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
        Stack->MenuStack[MenuCount->Count].MoveRow=0;//�˵���������(����ʱ��Ϊ0,����ʱ����)
        Stack->MenuStack[MenuCount->Count].EditRow=1;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
        Stack->MenuStack[MenuCount->Count].EditCol=0;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
        Call_E2(0);//����E2�˵�
      }
			//������ʾ���
	    Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
			LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
    }
  }
  else
  {//�˵���ջ����!=0
		if(FileDownload->File&0x10)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
		{
			Call_E2(Stack->MenuStack[MenuCount->Count].PC);//����E2�˵�
		}
	  if(Comm_Ram->DisplayScanTimer==0x0)
	  {
		#if (USER/100)==5//�Ϻ�
			p8=(u8*)(ADDR_MenuICO);//1byte �˵�����ʾ���к͵���ͼ�� 0=��,1=��
			if(p8[0]!=0)
			{
				DisplayClrRow(0,0);//����Ļ��ʾ��ָ����
				DisplayClrRow(9,9);//����Ļ��ʾ��ָ����
				DisplayLoop_TopICO(0);//��ͼ���ʱ����ʾ
				DisplayLoop_BottomICO(9);//��ͼ����ʾ
			}
		#endif
			//������ʾ���
		  Comm_Ram->DisplayScanTimer=1000/10;//1100/10;
			LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
		}
    if(Comm_Ram->MenuOutTimer==0)
    {//�˵��˳� 10MS��ʱ��
      MenuCount->Count=0;
    }
  }
	
#endif//#if (IC_UC1698|IC_ST75161)//0=û,1=��
}


void Terminal_Main_PowerUp(void)//I2�����е�
{
/*
	u16 *p16;
	p16=(u16*)(ADDR_RUNSPEEDTEST);//ѭ�������ٶȲ���(���ֶ���)
	__disable_irq();
	p16[0]++;
	__enable_irq();
*/
/*
//�Բ�����
	if(Comm_Ram->SelfTestCommand==0x55555555)//�Բ�������0x55555555����1(Ӳ��),0xAAAAAAAA����2(FLASH),0x5555AAAA����3(FLASH��������)
	{
	#if((IC_SST39VF6401B+IC_S29GLxxx)!=0)
		if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[70])();//����E1
		}
	#else
		SelfTest1();//�ն��Բ�1(Ӳ������)
	#endif
		Terminal_Uart_Manage();//Terminal_Uart����
		Uart_3761_RxTx(RS485_1PORT);//UART_3761���շ���
		Uart_3761_RxTx(RS485_2PORT);//UART_3761���շ���
		if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
		{
			Uart_3761_RxTx(RS485_3PORT);//UART_3761���շ���
		}
		return;
	}
	if(Comm_Ram->SelfTestCommand==0xAAAAAAAA)//�Բ�������0x55555555����1(Ӳ��),0xAAAAAAAA����2(FLASH),0x5555AAAA����3(FLASH��������)
	{
	#if((IC_SST39VF6401B+IC_S29GLxxx)!=0)
		if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[71])();//����E1
		}	
	#else
		SelfTest2();//�ն��Բ�2(FLASH)
	#endif
		Terminal_Uart_Manage();//Terminal_Uart����
		Uart_3761_RxTx(RS485_1PORT);//UART_3761���շ���
		Uart_3761_RxTx(RS485_2PORT);//UART_3761���շ���
		if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
		{
			Uart_3761_RxTx(RS485_3PORT);//UART_3761���շ���
		}
		return;
	}
	if(Comm_Ram->SelfTestCommand==0x5555AAAA)//�Բ�������0x55555555����1(Ӳ��),0xAAAAAAAA����2(FLASH),0x5555AAAA����3(FLASH��������)
	{
	#if((IC_SST39VF6401B+IC_S29GLxxx)!=0)
		if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[72])();//����E1
		}	
	#else
		SelfTest3();//�ն��Բ�3(FLASH��������)
	#endif
		Terminal_Uart_Manage();//Terminal_Uart����
		Uart_3761_RxTx(RS485_1PORT);//UART_3761���շ���
		Uart_3761_RxTx(RS485_2PORT);//UART_3761���շ���
		if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)
		{
			Uart_3761_RxTx(RS485_3PORT);//UART_3761���շ���
		}
		return;
	}
*/
	
//��ʾ
	Terminal_DisplayTask();//I2��ʾ
//UART
	Terminal_Uart_Manage();//Terminal_Uart����
//��ʱ����
	Terminal_Task();//I2��ʱ����(�����е�)
	
}

void Terminal_Main_Battery(void)//I2��ع���
{
#if ((Project/100)<2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=ͨ��ģ��
	return;
#else
	if(Comm_Ram->BattWork_S_timer==0)//4 ��ع����ػ��붨ʱ��
	{
		DeepPowerDown();//��ȵ�Դ�½�ģʽ
	}
	else
	{
		Terminal_DisplayTask();//I2��ʾ
		if((RTC->BKP0R&0xff)<=1)//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
		{//��USB����
			SCB->SCR = 0x0;//SLEEPDEEP=0
			__WFI();
		}
  	if(Pin_Read(PIN_WKUP)!=0)
  	{
			WWDT_RESET();//����ʹ�ܿ��Ź���λ
		}
	}		
#endif
}



void Terminal_Main(void)
{
	
#if ((Project/100)!=0)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  switch(Comm_Ram->POWER)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
  {
	#if ((Project/100)>=2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
    case 0://0=��ع���
			if(Comm_Ram->UBACKUP<RechargeableBatteryStopMin)//�ɳ���ֹͣ�������ѹ4.40V
			{
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
			}
      Terminal_Main_Battery();//I2��ع���
      break;
		case 1://1=��������ת��ع���������
			DeepPowerDown();//��ȵ�Դ�½�ģʽ
			break;
	#endif
    case 2://2=��������ת��ع���
			if(Comm_Ram->UBACKUP<RechargeableBatteryStartMin)//�ɳ�������������С��ѹ4.80V
			{
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
				return;
			}
			Terminal_PowerDown();//2=��������ת��ع���
			break;
    case 3://3=��������
      Terminal_Main_PowerUp();//I2�����е�
      break;
  }
#endif

}
