
//�ն˵�Դ�½�
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Head/QCSG_Hal.h"
#include "../QCSG_Head/QCSG_AlarmEvent.h"

#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"
#include "../QCSG_Uart/Terminal_ReadMeter_ACSample.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_PWR.h"
#include "../STM32F4xx/STM32F4xx_IO.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"

#include "../MS/Data_Save.h"
#include "../GPRS/GPRS_QCSG.h"
#include "Terminal_Task.h"
#include "Terminal_PowerDown.h"

#ifdef IC_STM32F4xx
#include "../STM32F4xx/STM32F4xx_MAC.h"
#endif
#ifdef IC_NUC9xx
#include "../../NUC97x/NUC97x/NUC97x_MAC.h"
#endif


void Terminal_PowerDown(void)		// 2=��������ת��ع���
{
	u32 PORTn;
  	UARTCtrl_TypeDef * UARTCtrl;
	RealTimeStruct RealTime;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	PORTn = 0;
	
	if(UART1Ctrl->LinkTask >= 101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		PORTn = GPRSPORT;
	}
	else
	{
		UART1Ctrl->LinkTask = 1;	// 1=���ܵ�Դ
	}
	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if(UARTCtrl->LinkTask >= 101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		PORTn = ENETPORT;
	}
	else
	{
		UARTCtrl->LinkTask = 1;		// 1=���ܵ�Դ
	}

	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);	
	switch(pTerminalRam->PowerDownTask)//��Դ�����������
	{
		case 0://�����¼�����
			DisplayStringAutoUp(0,9,(u8*)"\x0""����ͣ��");		// ��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			pCommRam->BattWork_S_timer = 180;					// 180;//4 ��ع����ػ��붨ʱ��
			pTerminalRam->PowerDownTask++;						// ��Դ�����������
			break;
		case 1://��鵱ǰ�Ƿ�����
			if(PORTn==0)
			{
				UART1Ctrl->LinkTask=1;							// 1=�ص�Դ
				UART1Ctrl->PORT=1;								// �����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				UnInit_GPRS_QCSG();								// GPRSģ������Ϊ��������
				OffR5V0();										//��R5V0��Դ
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
				pTerminalRam->PowerDownTask=50;//��Դ�����������.���ϱ�
			}
			else
			{
				if(PORTn==GPRSPORT)		
				{												// GPRS ���ߣ���̫�������ߣ���ر���̫����Դ
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
					UARTCtrl->LinkTask=1;//1=�ص�Դ
					UARTCtrl->PORT=1;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				#ifdef IC_STM32F4xx
					//UnInit_GPRS_QCSG();								// GPRSģ������Ϊ��������
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
				{												// GPRS �����ߣ�̫�������ߣ���ر� GPRS ��Դ
					UART1Ctrl->LinkTask=1;						// 1=�ص�Դ
					UART1Ctrl->PORT=1;							// �����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
					UnInit_GPRS_QCSG();							// GPRSģ������Ϊ��������
					OffR5V0();									// ��R5V0��Դ
				}
				pCommRam->BattWork_S_timer = 60;				// ���� 1 ���ӵ���澯��ʱ����
				pTerminalRam->PowerDownTask++;					// ��Դ�����������
			}
			break;
		case 2://�ڶ�ʱ�ڵȴ��ϱ�
			if(pCommRam->BattWork_S_timer == 0)					// ��ع����ػ��붨ʱ��
			{
				pTerminalRam->PowerDownTask++;					// ��Դ�����������
				HAL_GetSystemTime(&RealTime);
				GenerateTerminalPowerUpAndDownAlarm(&RealTime, 0);
				pTSysCtrl->Flag |= TERMINAL_POWER_DOWN_DONE_FLAG;
				pCommRam->BattWork_S_timer = 120;				// �����ϱ��澯����ʱ��Ϊ 1����
			}
			break;
		case 3:
			
#if (TEST_ON == 1)//̨�����ʱ���ٱ�������3����
			if(pCommRam->BattWork_S_timer == 0)
			{
				pTerminalRam->PowerDownTask++;
			}
#else
			if((pTSysCtrl->LatelyAlarmNum == 0) || (pCommRam->BattWork_S_timer == 0))
			{
				pTerminalRam->PowerDownTask++;
			}
#endif
			break;
		case 4://���˳���¼
			DisplayStringAutoUp(0,9,(u8*)"\x0""ͣ���˳���¼");	// ��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol

#if (TEST_ON == 1)
			pCommRam->BattWork_S_timer = 15;
#else 
			pCommRam->BattWork_S_timer = 120;					// �ȴ������˳���¼��ʱ //err20180228-9 �ϵ�ʱ��15��Ϊ120
#endif
			UARTCtrl->LinkTask = 102;
			pTerminalRam->ENETHeartBeat_S_Timer=0;				// 2 ��̫�������붨ʱ��
			pTerminalRam->GPRSHeartBeat_S_Timer=0;				// 5 GPRS�����붨ʱ��
			pTerminalRam->PowerDownTask++;						// ��Դ�����������
			break;
		case 5://�ȴ��˳���¼
			if((UARTCtrl->LinkTask == 103)||(pCommRam->BattWork_S_timer==0))//��ع����ػ��붨ʱ��
			{
				pTerminalRam->PowerDownTask++;					// ��Դ�����������
			}
			break;
		case 6://�ػ�
			UART1Ctrl->LinkTask=104;							// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			UARTCtrl->LinkTask=104;								// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			pTerminalRam->PowerDownTask++;						// ��Դ�����������
			break;
		case 7://����
			if(pCommRam->BattWork_S_timer==0)					//	��ع����ػ��붨ʱ��
			{
				Battery_Data_Save();							// ��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();								// ��ȵ�Դ�½�ģʽ
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			if((UART1Ctrl->LinkTask==2)||(UARTCtrl->LinkTask==2))//���ܵ�Դ(�ȴ��ػ���ʱ)
			{
				Battery_Data_Save();							// ��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();								// ��ȵ�Դ�½�ģʽ
			}
			break;
		
		case 50://���ϱ�
			DisplayStringAutoUp(0,9,(u8*)"\x0""�����߲��ϱ�");	// ��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			//pCommRam->BattWork_S_timer = 5;						// 4 ��ع����ػ��붨ʱ�� //err20180228-9 ȥ������
			pTerminalRam->PowerDownTask++;						// ��Դ�����������
			break;
		case 51://�ȴ���ʱ��ػ�
			if(pCommRam->BattWork_S_timer==0)					// ��ع����ػ��붨ʱ��
			{
				Battery_Data_Save();						// ��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();							// ��ȵ�Դ�½�ģʽ
			}
			break;	
		default:
			pTerminalRam->PowerDownTask=0;						// ��Դ�����������
			break;
	}
}





//err20190304    II�ͼ��������紦��
void JZQ_II_Terminal_PowerDown(void)		// 2=��������ת��ع���
{
	u32 PORTn;
  	UARTCtrl_TypeDef * UARTCtrl;
	RealTimeStruct RealTime;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	PORTn = 0;

	
	//II�ͼ��������ߵƺ��ź�ǿ�����ƴ���
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(GPRSPORT);
	if(UARTCtrl->LinkTask == 101)
	{
		Terminal_Ram->GPRS_ONLINELED_MS_Timer = 2000/10;
		if(Terminal_Ram ->Signal == 1)
			{
				Terminal_Ram ->GPRS_WEAK_SINGLE_LED_MS_Timer = 2000/10;
			} 
			if((Terminal_Ram ->Signal == 2)||(Terminal_Ram ->Signal == 3))
			{
				Terminal_Ram ->GPRS_WEAK_SINGLE_LED_MS_Timer = 2000/10;
				Terminal_Ram ->GPRS_STRONG_SINGLE_LED_MS_Timer = 2000/10;
			}
			if(Terminal_Ram ->Signal == 4)
			{
				Terminal_Ram ->GPRS_STRONG_SINGLE_LED_MS_Timer = 2000/10;
			}	
	}
	
	if(UART1Ctrl->LinkTask >= 101)//+��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		PORTn = GPRSPORT;
	}
	else
	{
		UART1Ctrl->LinkTask = 1;	// 1=���ܵ�Դ
	}
//	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
//	if(UARTCtrl->LinkTask >= 101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
//	{
//		PORTn = ENETPORT;
//	}
//	else
//	{
//		UARTCtrl->LinkTask = 1;		// 1=���ܵ�Դ
//	}

	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);	
	switch(pTerminalRam->PowerDownTask)//��Դ�����������
	{
		case 0://�����¼�����
			//DisplayStringAutoUp(0,9,(u8*)"\x0""����ͣ��");		// ��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			//pCommRam->BattWork_S_timer = 180;					// 180;//4 ��ع����ػ��붨ʱ��
			pTerminalRam->PowerDownTask++;						// ��Դ�����������
			break;
		case 1://��鵱ǰ�Ƿ�����
			if(PORTn==0)
			{
				UART1Ctrl->LinkTask=1;							// 1=�ص�Դ
				UART1Ctrl->PORT=1;								// �����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				UnInit_GPRS_QCSG();								// GPRSģ������Ϊ��������
				OffR5V0();										//��R5V0��Դ
				//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				//UARTCtrl->LinkTask=1;							// 1=�ص�Դ
				//UARTCtrl->PORT=1;								// �����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				UnInit_STM32F4xx_MAC();
				OffE5V0();//��E5V0��Դ
				pTerminalRam->PowerDownTask = 50;				// ��Դ�����������.���ϱ�
			}
			else
			{
				if(PORTn==GPRSPORT)		
				{												// GPRS ���ߣ���̫�������ߣ���ر���̫����Դ
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
					UARTCtrl->LinkTask=1;						// 1=�ص�Դ
					UARTCtrl->PORT=1;							// �����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
					UnInit_STM32F4xx_MAC();
					OffE5V0();									// ��E5V0��Դ
				}
				//else
				//{												// GPRS �����ߣ�̫�������ߣ���ر� GPRS ��Դ
				//	UART1Ctrl->LinkTask=1;						// 1=�ص�Դ
				//	UART1Ctrl->PORT=1;							// �����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				//	UnInit_GPRS_QCSG();							// GPRSģ������Ϊ��������
				//	OffR5V0();									// ��R5V0��Դ
				//}
				pCommRam->BattWork_S_timer = 5;				// ���� 1 ���ӵ���澯��ʱ����
				pTerminalRam->PowerDownTask++;					// ��Դ�����������
			}
			break;
		case 2://�ڶ�ʱ�ڵȴ��ϱ�
			if(pCommRam->BattWork_S_timer == 0)					// ��ع����ػ��붨ʱ��
			{
				pTerminalRam->PowerDownTask++;					// ��Դ�����������
				HAL_GetSystemTime(&RealTime);
				GenerateTerminalPowerUpAndDownAlarm(&RealTime, 0);
				pTSysCtrl->Flag |= TERMINAL_POWER_DOWN_DONE_FLAG;
				pCommRam->BattWork_S_timer = 60;				// �����ϱ��澯����ʱ��Ϊ 1����
			}
			break;
		case 3:
			
#if (TEST_ON == 1)//̨�����ʱ���ٱ�������3����
			if(pCommRam->BattWork_S_timer == 0)
			{
				pTerminalRam->PowerDownTask++;
			}
#else
			if((pTSysCtrl->LatelyAlarmNum == 0) || (pCommRam->BattWork_S_timer == 0))
			{
				pTerminalRam->PowerDownTask++;
			}
#endif
			break;
		case 4://���˳���¼
			DisplayStringAutoUp(0,9,(u8*)"\x0""ͣ���˳���¼");	// ��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol

#if (TEST_ON == 1)
			pCommRam->BattWork_S_timer = 15;
#else 
			pCommRam->BattWork_S_timer = 120;					// �ȴ������˳���¼��ʱ //err20180228-9 �ϵ�ʱ��15��Ϊ120
#endif
			UART1Ctrl->LinkTask = 102;
			//pTerminalRam->ENETHeartBeat_S_Timer=0;				// 2 ��̫�������붨ʱ��
			pTerminalRam->GPRSHeartBeat_S_Timer=0;				// 5 GPRS�����붨ʱ��
			pTerminalRam->PowerDownTask++;						// ��Դ�����������
			break;
		case 5://�ȴ��˳���¼
			if((UART1Ctrl->LinkTask == 103)||(pCommRam->BattWork_S_timer==0))//��ع����ػ��붨ʱ��
			{
				pTerminalRam->PowerDownTask++;					// ��Դ�����������
			}
			break;
		case 6://�ػ�
			UART1Ctrl->LinkTask=104;							// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			//UARTCtrl->LinkTask=104;								// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			pTerminalRam->PowerDownTask++;						// ��Դ�����������
			break;
		case 7://����
			if(pCommRam->BattWork_S_timer==0)					//	��ع����ػ��붨ʱ��
			{
				Battery_Data_Save();							// ��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();								// ��ȵ�Դ�½�ģʽ
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			if((UART1Ctrl->LinkTask==2)||(UARTCtrl->LinkTask==2))//���ܵ�Դ(�ȴ��ػ���ʱ)
			{
				Battery_Data_Save();							// ��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();								// ��ȵ�Դ�½�ģʽ
			}
			break;
		
		case 50://���ϱ�
			DisplayStringAutoUp(0,9,(u8*)"\x0""�����߲��ϱ�");	// ��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
			pCommRam->BattWork_S_timer = 0;						// 4 ��ع����ػ��붨ʱ�� //err20180228-9 ȥ������
			pTerminalRam->PowerDownTask++;						// ��Դ�����������
			break;
		case 51://�ȴ���ʱ��ػ�
			if(pCommRam->BattWork_S_timer==0)					// ��ع����ػ��붨ʱ��
			{
				Battery_Data_Save();						// ��ع���ʱ�ػ�ǰ���ݱ���
				DeepPowerDown();							// ��ȵ�Դ�½�ģʽ
			}
			break;	
		default:
			pTerminalRam->PowerDownTask=0;						// ��Դ�����������
			break;
	}
}







