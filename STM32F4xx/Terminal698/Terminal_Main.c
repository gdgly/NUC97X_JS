
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../MS/MS.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_FMC.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../STM32F4xx/STM32F4xx_ExtMem.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"

#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayError.h"
#include "../Display/DisplayFTP.h"
#include "../Display/DisplayLoop.h"

#include "../Device/MEMRW.h"
#include "Terminal_PowerDown.h"
#include "Terminal_Init.h"
#include "Terminal_Timer.h"
#include "Terminal698_Uart_Manage.h"

#include "../DL698/RM_TASK.h"
#include "../DL698/CALC_SwitchIn.h"
#include "../DL698/CALC_RESET_FLOW_TIME.h"
#include "../DL698/CONGEAL_Record.h"
#include "../DL698/CALC_Voltage_Qualification_Rate.h"

#include "../DL698/EVENT_CALC.h"
#include "../DL698/EVENT_CALC_iMeter.h"
#include "../DL698/CALC_impIn.h"
#include "../DL698/CALC_TAG.h"
#include "../DL698/Contrl_Manage.h"
#include "../TEST/LOG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"

#include "../DL698/DL698TEST.h"
#include "../Test/Test_RAM.h"
#include "../MS/Data_Save.h"
#include "../DL698/DL698_JSON.h"

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
		if(Comm_Ram->RTCBuff[0]&0x1)
		{
			Pin_OutH(PIN_RUNLED);
		}
		else
		{
			Pin_OutL(PIN_RUNLED);
		}
	}
}

void ACTION_RESET(void)//��λ�������
{
	u32 PORTn;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(Comm_Ram->RESET!=0x55)//0x55=������(����ʹ�ÿ��Ź���λ)
	{
		return;
	}
	PORTn=Comm_Ram->ACTION_RESET_PORTn;//������λ��ͨ�Ŷ˿ں�
	switch(PORTn)
	{
		case RS232PORT://0//RS232
		case GPRSPORT://1//GPRS
		case RS485_1PORT://2//RS485_1ͨ�ſں�
		case RS485_2PORT://3//RS485-2ͨ�ſں�
		case RS485_3PORT://5//RS485_3ͨ�ſں�(WI-FI)
		case IRPORT://6//����ͨ�ſں�
		case ENETPORT://10//��̫��ͨ�ſں�
		case SMSPORT://11//����ͨ�ſں�
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
			if(UARTCtrl->Task!=0)
			{//0=���ڽ���
				return;
			}
		#ifdef IC_NUC9xx
			//��ʱ�ȴ�����ֽڷ���
			if(Comm_Ram->WaitUartCompleteMSTimer!=0)
			{
				return;
			}
		#endif
	}
	//��ʱ�ȴ�����ֽڷ���
	Comm_Ram->GPMSTimer=1000/10;//18 ͨ��(������UART11) ÿ10MS��1��ʱ��
	while(Comm_Ram->GPMSTimer)
	{
	}
	WWDT_RESET();//����ʹ�ܿ��Ź���λ
	
}




void Terminal_Task(void)//I2��ʱ����(�����е�)
{
	u8 *p8;
	u32 *p32;
	//u32 i;
	
	p32=p32;
	p8=(u8*)(ADDR_I2Task);//I2���������(ԭ��SDRAM,��SDRAM�������޷�����������SDRAM�������Ŀ��ܹʸķ���IRAM)
  switch(p8[0])
  {
    case 0:
      MW(ADDR_MYMDHMS,ADDR_TFM_YMDHMS,6);//����ǰʱ��
			Terminal_FMTimer();//���籣����ͷֶ�ʱ��
			Terminal_Timer();//�ն˷��ж���ͷֶ�ʱ��
			RunLED();//����LEDָʾ��
      break;
		case 1://�ն�ͣ�ϵ��¼�
			Event_Power();//�ն�ͣ/�ϵ��¼�
			break;
    case 2:
			RM_UpDate();//�����������;����:�ж�,���ɼ��������ñ��flash��sdram,��λ��PORTn���������ؼ������賭�����ɹ�����
      break;
    case 3:
			ACTION_RESET();//��λ�������
			CALC_SwitchIn();//����������
			FLOW_Count(GPRSPORT);//ͨ����������
			//FLOW_Count(ENETPORT);//ͨ����������
			POWER_Time();//����ʱ��
			break;
		case 4:
			CONGEAL_Record();//����
			break;
		case 5:
			CALC_Voltage_Qualification_Rate();//��ѹ�ϸ��ʼ���
			break;
		case 6:
			EVENT_CALC_300F_3010();//��ѹ�����������¼�
			EVENT_CALC_3003();//�����־���¼�(�����¼�)
			
			break;
		case 7:
		#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
			Terminal_impIn_Energy();//�ն���������-����
			Terminal_impIn_Demand();//�ն���������-���ʺ��������
			Update_Group_Total_Data_240x();//�����ܼ��������,���ñ���������
		#endif
		#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
			if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{
				Terminal_impIn_Energy();//�ն���������-����
				Terminal_impIn_Demand();//�ն���������-���ʺ��������
				Update_Group_Total_Data_240x();//�����ܼ��������,���ñ���������
			}
		#endif
			break;
		case 8:
			Terminal_Contrl_Manage();//�ն˿��ƹ���(����������)
			break;
		case 9:
			if(Terminal_Router->MinuteCollect!=0)
			{
				Terminal_PackJSON();
			}
			break;
		case 10:	
			//TaskReport_JsonFile(GPRSPORT);
			//TaskReport_JsonFile(ENETPORT);
			break;

    default:
#ifdef IC_STM32F4xx
	#if DEBUG==1
			//TEST
		#if RUN_DL698TEST_1==1//0��Ч,1=����;ȫ��������Ϊ�ڲ����ɲ������ݴ洢������
			DL698TEST_1();//ȫ��������Ϊ�ڲ����ɲ������ݴ洢������
		#endif
		#if RUN_DL698TEST_2==1//0��Ч,1=����;����601E��Լ��
			DL698TEST_2();//����601E��Լ��
		#endif
		#if RUN_DL698TEST_USER==1//0��Ч,1=����;�û������
			DL698TEST_USER();//�û������
		#endif
			//
		
			//SDRAM�������
			if(RAM_Test(ADDR_128KDATABUFF,1024))//RAM����;����:0=��ȷ,1=����
			{
				Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
				DisplayStringAutoUp(0,9,(u8*)"\x0""SDRAM���ݴ���!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	#else
			if(RAM_Test(ADDR_128KDATABUFF,64))//RAM����;����:0=��ȷ,1=����
			{
	#endif
				Comm_Ram->GPMSTimer=1000/10;//18 ͨ��(������UART11) ÿ10MS��1��ʱ��
				while(1)
				{
					Init_ExtMem();//sdram��ʼ��
					SDRAM_FMC_Init();
					if(RAM_Test(ADDR_128KDATABUFF,64)==0)//RAM����;����:0=��ȷ,1=����
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
						RTC->BKP19R=0x12345678;//ͣ�����¼��ж����ϵ��־;���Ź���λ�Զ�����=0x12345678,����ֵΪ�ϵ�
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
#endif
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
//	u32 i;
  u32 * p32;
#if (USER/100)==5//�Ϻ�
	u8 *p8;
#endif

//ms_Type * ms;
//ms=Get_ms();
	
	
  if(MenuCount->Count==0)//�˵���ջ����
  {//�˵���ջ����=0
    if((Comm_Ram->ButtonVal&0x10)==0)
    {//��ȷ�ϼ�����,ѭ��
		  if(Comm_Ram->DisplayScanTimer==0x0)
		  {
			#ifdef IC_STM32F4xx
				if(((RTC->BKP0R&0xff)>=2)&&((RTC->BKP0R&0xff)<=4))//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
			#endif
			#ifdef IC_NUC9xx
				if((FileDownload->USB0Task>=2)&&(FileDownload->USB0Task<=4))//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
			#endif
				{//USB�ļ���ɱ�־��Ч
					Comm_Ram->DisplayAllErrorTimer=0;
					Comm_Ram->DisplayScanTimer=5000/10;
					DOTMIXPMODE->Task=1;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
					DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
				}
				else
				{
//					i=MRR(ADDR_FTPFileFlags+4,1);
//		      if((i<(MaxFileNum+3))&&(Comm_Ram->DisplayAllErrorTimer!=0))
//					{//FTP�ļ�������Ч
//						DOTMIXPMODE->Task=1;//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
//						DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
//						DisplayFTP();//FTP�ļ�������ʾ
//					}
//					else
					{
//						if((i==(MaxFileNum+2))&&(Comm_Ram->DisplayAllErrorTimer==0))
//						{
//							MWR(MaxFileNum+3,ADDR_FTPFileFlags+4,1);//FTP�ļ���������ʾ�����־
//						}
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
		#if (Project/100==2)&&(USER/100)==17//�Ϻ�
				DisplayClrRow(0,0);//����Ļ��ʾ��ָ����
				DisplayClrRow(9,9);//����Ļ��ʾ��ָ����
				DisplayLoop_TopICO(0);//��ͼ���ʱ����ʾ
				DisplayLoop_BottomICO(9);//��ͼ����ʾ
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
//��ʾ
	Terminal_DisplayTask();//I2��ʾ
//UART
	Terminal_Uart_Manage();//Terminal_Uart����

//��ʱ����
	Terminal_Task();//I2��ʱ����(�����е�)
}

void Terminal_Main_Battery(void)//I2��ع���
{
#if ((Project/100)<2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=ͨ��ģ��
	return;
#else
	u32 i;
	if(Comm_Ram->BattWork_S_timer==0)//4 ��ع����ػ��붨ʱ��
	{
		DeepPowerDown();//��ȵ�Դ�½�ģʽ
	}
	else
	{
		Terminal_DisplayTask();//I2��ʾ
	#ifdef IC_STM32F4xx
		if((RTC->BKP0R&0xff)<=1)//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
		{//��USB����
			SCB->SCR = 0x0;//SLEEPDEEP=0
			__WFI();
		}
	#endif
		if(Pin_Read(PIN_WKUP)!=0)
		{
			//1���Դ�仯�˲�
			for(i=0;i<1000;i++)
			{
				SoftDelayMS(1);//���ms��ʱ
				if(Pin_Read(PIN_WKUP)==0)
				{
					return;
				}
			}
			WWDT_RESET();//����ʹ�ܿ��Ź���λ
		}
	}		
#endif
}
/*
const u32 JSON_ALIGN32_List[]=
{
	(u32)&JSON->g_Filesize,
	
};
	
const u32 JSON_ALIGN16_List[]=
{
	(u32)&JSON->CurFrzMetCount,
	(u32)&JSON->CurRealMetCount,
	(u32)&JSON->g_Block_Number,
	(u32)&JSON->g_Need_report_Block,
	(u32)&JSON->res2,
};

u32 Check_ALIGN32(u32* pList,u32 NUM)//���4�ֽڶ���:����:0=��ȷ,1=����
{
	u32 i;
	u32 x;
	
	for(i=0;i<NUM;i++)
	{
		x=pList[i];
		if(x&3)
		{
			return 1;
		}
	}
	return 0;
}
u32 Check_ALIGN16(u32* pList,u32 NUM)//���2�ֽڶ���:����:0=��ȷ,1=����
{
	u32 i;
	u32 x;
	
	for(i=0;i<NUM;i++)
	{
		x=pList[i];
		if(x&1)
		{
			return 1;
		}
	}
	return 0;
}
*/
void Terminal_Main(void)
{
	u32 i;

//---linux test----	
/*
	i=MRR(ADDR_DATABUFF,1);
	i++;
	MC(i,ADDR_DATABUFF,256);

	return;
*/	
//---linux test end----	
	
	
	i=i;
//---TEST---
//LOG(698,0,RS232PORT,(u8 *)ADDR_DATABUFF,16,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
//LOG(13762,0,RS232PORT,(u8 *)ADDR_DATABUFF,16,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
//LOG(13763,0,RS232PORT,(u8 *)ADDR_DATABUFF,16,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��

//	ABB_Password(0x12345678,0x90123456);//�����ܳ�key��Զ��ͨѶ������������ܿ�����
	
//i=ADDR_6014+(9*512);
//if(i)
//{
//	i=i;
//}
//i=ADDR_6000;
//if(i)
//{
//	i=i;
//}
//DL698_SETINIT(7,0);
//	for(i=1;i<=NUM_RMmax;i++)
//	{
//		MWR(i,ADDR_DATABUFF,6);
//		MeterAutoReport_3011((u8*)ADDR_DATABUFF,1);
//	}
//	for(i=1;i<=NUM_RMmax;i++)
//	{
//		MWR(i,ADDR_DATABUFF,6);
//		MeterAutoReport_3011((u8*)ADDR_DATABUFF,1);
//	}
//	i=5;
//	MWR(i,ADDR_DATABUFF,6);
//	MeterAutoReport_3011((u8*)ADDR_DATABUFF,2);
	
//	Pin_Configure(PIN_ADC123_IN13,//��������
//							0|//B3-B0:���ù���ѡ��AF(0-15)
//				 (3<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
//				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
//				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
//				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
//				(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
//	i=ADCx(13,3,1,0);//ADC��ѹ����,���:Channel=ADCͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ;����:����ֵ(0xffffffff��ʾת��ʧ��)
//	MWR(i,ADDR_VDD5V0,2);
	
//	ms_Type *ms;
//	
//	ms=Get_ms();
//	ms->msfile_format();
//i=FILENAMEmax_FLASH;
//if(i)
//{
//	i=i;
//}
//void RM_OK_FAIL(u32 PORTn,u32 OK);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
//	i=ADDR_128KDATABUFF;
//	if(i)
//	{
//		i=i;
//	}
//	UARTCtrl_TypeDef *UARTCtrl;
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
//	UARTCtrl->TaskID=1;
//	UARTCtrl->RMAddr[0]=1;
//	RM_OK_FAIL(RS485_2PORT,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
//	UARTCtrl->RMAddr[0]=2;
//	RM_OK_FAIL(RS485_2PORT,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
//	UARTCtrl->RMAddr[0]=3;
//	RM_OK_FAIL(RS485_2PORT,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
//	UARTCtrl->RMAddr[0]=2;
//	RM_OK_FAIL(RS485_2PORT,1);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
//	
	
//	recordFILE_write(0,(u8*)filedata,sizeof(filedata));//д��¼�ļ�,���:ID=b31-b24�����ȵ�ǰ��ִ�е�ID��,b23-b16��������,b15-b8�������,b7-b0�ļ�����;����:д���ֽ���
//	u32 i;
//	RMokfailLIST_TypeDef rmOKFAILlist[4][8];
//	u8* p8;
//	

//	i=ADDR_Terminal_InDemandimp;
//	if(i)
//	{
//		i=i;
//	}
//	
//	MR(ADDR_128KDATABUFF,ADDR_6000,LENper_6000);
//	for(i=1;i<NUMmax_6000;i++)
//	{
//		MW(ADDR_128KDATABUFF,ADDR_6000+(i*LENper_6000),LENper_6000);
//	}

//	Event_Standard(0x3104,0,2);//��׼�¼���¼
//	Event_Standard(0x3104,0,2);//��׼�¼���¼
//	Get_EventRecord_OAD(ACSAMPLEPORT,0x31040201,(u8 *)ADDR_DATABUFF,1024*4);//���¼���¼;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR

//	Event_Standard(0x3114,0,2);//��׼�¼���¼;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
//	ADDR_Check();//ADDR_6000�������ܱ��ַ�ظ����;����:0=û�ظ�,1=���ظ�,���Զ�ɾ���ظ����
//---TEST END---
	
#if ((Project/100)!=0)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��
  switch(Comm_Ram->POWER)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
  {
	#if ((Project/100)>=2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��
    case 0://0=��ع���
			if(Comm_Ram->UBACKUP<RechargeableBatteryStopMin)//�ɳ���ֹͣ�������ѹ4.40V
			{
			#ifdef IC_NUC9xx
				if(SuperCap_Voltage()<SuperCapBatteryStopMin)//�������ݵ�ѹADC����;����:BCD��ʽ2λС��
				{
					DeepPowerDown();//��ȵ�Դ�½�ģʽ
				}
			#else
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
			#endif
			}
      Terminal_Main_Battery();//I2��ع���
      break;
		case 1://1=��������ת��ع���������
			//DeepPowerDown();//��ȵ�Դ�½�ģʽ
			WWDT_RESET();//����ʹ�ܿ��Ź���λ(�ϵ�ʱ����PA3=1 ON ofter Dog power,���򲻿�ʹ��WDT)
			break;
	#endif
    case 2://2=��������ת��ع���
			//if(Comm_Ram->UBACKUP<RechargeableBatteryStartMin)//�ɳ�������������С��ѹ4.80V
			if(Comm_Ram->UBACKUP<RechargeableBatteryStopMin)//�ɳ���ֹͣ�������ѹ4.40V
			{
			#if HVER>=0x420//Ӳ���汾��:4.20;���ӳ������ݵ�ѹADC����
				OffEMU();//��EMU(��λ)
				OffA5V0();//��A5V0��Դ
				Meter_Ram->EMUInit=0;//����оƬ��ʼ����־:0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
			 #ifdef IC_STM32F4xx
				VDD5V0_Voltage();//VDD5V0��ѹADC����
				if(Comm_Ram->VDD5V0<SuperCapBatteryStopMin)//�������ݵ�ѹADC����;����:BCD��ʽ2λС��
			 #endif
			 #ifdef IC_NUC9xx
				i=SuperCap_Voltage();
				if((i<SuperCapBatteryStopMin)||(Comm_Ram->Factory==0x55))//�������ݵ�ѹADC����;����:BCD��ʽ2λС��
			 #endif
				{
					Battery_Data_Save();//��ع���ʱ�ػ�ǰ���ݱ���
					DeepPowerDown();//��ȵ�Դ�½�ģʽ
					return;
				}
			#else
				Battery_Data_Save();//��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
				return;
			#endif
			}
			if(Comm_Ram->DataSave==0)//���ݱ���:0=��,1=�����е糬��10�������豣��,2=�����ѱ���
			{
				DeepPowerDown();//��ȵ�Դ�½�ģʽ
				return;
			}
			Terminal_PowerDown();//2=��������ת��ع���
			Terminal_Uart_Manage();//Terminal_Uart����
			Terminal_Timer();//�ն˷��ж���ͷֶ�ʱ��
			RunLED();//����LEDָʾ��
			Event_Power();//�ն�ͣ/�ϵ��¼�
			break;
    case 3://3=��������
      Terminal_Main_PowerUp();//I2�����е�
      break;
  }
#endif

}






