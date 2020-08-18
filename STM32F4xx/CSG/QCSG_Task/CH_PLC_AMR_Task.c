/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:CH_PLC_AMR_Task.c
��  ��:zhaoyun
��  ��:1.00
��  ��:20170812
��  ��:����PLC�ز�ģ�鳭�������ļ�
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../QCSG_Head/QCSG_Hal.h"

#include "Terminal_Task.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Head/CH_PLC_AMR_Task.h" 
#include "../QCSG_Head/QCSG_AlarmEvent.h"
#include "../TEST/LOG.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//68 1A 00 60 11 11 11 11 11 11 22 22 22 22 22 22 00 01 01 00 01 E8 01 01 7F 16
#if ((Project/100)==2 || ((Project/100)==9))

#define PLC_SEARCH_BPS	(0x0440)
/* Private constants ---------------------------------------------------------*/

void PLC_ModuleManage(u32 PORTn)
{
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	switch(PLCManage->State)
	{
		case PLC_POWER_UP_INIT:	
			PLC_PowerUpInit(PORTn, PLCManage);
			break;
		case PLC_RESET_INIT:
			PLC_ResetInit(PORTn, PLCManage);
			break;
		case PLC_MODLE_RECOGNIZE:
			PLC_ModleRecognizeProcess(PORTn, PLCManage);
			break;
		case PLC_FILE_SYNC:
			PLC_ModleFileSync(PORTn, PLCManage);
			break;
		case PLC_FILE_ADD:
			PLC_ModleFileAddNode(PORTn, PLCManage);
			break;
		case PLC_AMR_STATE:
			PLC_ModleAMR_Task(PORTn, PLCManage);
			break;
		case PLC_CHECK_MODLE: //�ݴ����Ƽ���ز�ģ��
			PLC_CheckModle(PORTn,PLCManage);
			break;
		case PLC_YBWHZ_TEST: //err20190403 �ز�ģ�������
			PLC_YBWHZ_Test(PORTn,PLCManage);
			break;
		case PLC_MODLE_ERROR:		
			break;
		default:
			PLCManage->State = PLC_POWER_UP_INIT;
			break;
	}

	PLC_RT_AMR_ReqCheck(PORTn, PLCManage);
	
	QCSG_LocalModuleFrameProcess(PORTn, PLCManage);
}

void PLC_RT_AMR_ReqCheck(u32 PORTn, PLCManageStruct* PLCManage)
{
	u16* pAckTimer;
	u32 CurTicks = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	AMRCtrlStruct* AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);

	if((AMRCtrl->Flag & AMR_FLAG_PAUSE) != 0)
	{
		AMRCtrl->Flag &= ~AMR_FLAG_PAUSE;
		if(PLCManage->State == PLC_AMR_STATE)
		{
			pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
			if(AMRCtrl->State <= AMR_Pause)
			{
				if(AMRCtrl->State != AMR_Pause)
				{
					AMRCtrl->OldState = AMRCtrl->State;
					AMRCtrl->State = AMR_Pause;	
				}
				pAckTimer[0] = AMR_PAUSE_MAX_TIME;
				Task->Flag |= RT_AMR_CH_READY;
			}		
		}		
	}

	if(PLCManage->State != PLC_AMR_STATE) return;

	if(Pin_Read(PIN_PLC_ID) == 0)
	{
		PLCManage->UnPlugCnt = 0;
	}
	else
	{
		PLCManage->UnPlugCnt++;
		if(PLCManage->UnPlugCnt < 20) return;

		PLCManage->UnPlugCnt = 0;
		WarningOccur(PORTn,(u8 *)"�ز�ģ��û����");	//�澯����,��ڸ澯����͸澯�ַ���
		PLCManage->State = PLC_POWER_UP_INIT;

		if(((AMRCtrl->AMR_ID & (AMR_DAY_FREEZ_ID | AMR_DAY_FREEZ_PATCH_ID | AMR_HOUR_FREEZ_ID)) != 0) && (AMRCtrl->State != AMR_Idle))
		{
			if((AMRCtrl->State == AMR_Tx) || (AMRCtrl->State == AMR_WaitRx))
			{
				AMRCtrl->OldState = AMR_Tx;
			}
			AMRCtrl->State = AMR_Resume;
		}
		else
		{
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
		}
	}	

	CurTicks = HAL_GetSystemTick();
	if((CurTicks - PLCManage->LastRcvTicks) > 30000) // 5���� 5*60*1000/10 
	{
		PLCManage->State = PLC_CHECK_MODLE;//PLC_RESET_INIT;
		AMRCtrl->OldState = AMRCtrl->State;
		pTerminalRouter->Old_RouterInitTask = pTerminalRouter->RouterInitTask;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;		
	}
}
void PLC_PowerUpInit(u32 PORTn, PLCManageStruct* PLCManage)
{
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	//Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	switch(pTerminalRouter->RouterInitTask)
	{
		case 0:
			PLCModuleGPIOInit(PORTn);
			break;
		case 1:
			PLCModulePowerUpInit(PORTn);
			break;
		case 2:
			PLCManage->State = PLC_RESET_INIT;
			PLCManage->UnPlugCnt = 0;
			pTerminalRouter->RouterInitTask = 0;
		default:
			pTerminalRouter->RouterInitTask = 0;
			break;
	}
}

void PLCModuleGPIOInit(u32 PORTn)
{
	u16 * p16timer;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	//Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	if(GetTerminalPower_S_TimerValue() > 0) return;
	if(pTerminalRam->RouterCheck_S_Timer != 0) return;
	if(pTerminalRam->RouterAck_S_Timer != 0) return;
	
	p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);
	Pin_Configure(PIN_UART4_RX,//RXD����,����
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Pin_Configure(PIN_UART4_TX,//TXD����,����
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Pin_Configure(PIN_PLC_RST,//��λ�������0
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	//i = pComm_Ram->MYMDHMS[1] + (pComm_Ram->MYMDHMS[2] << 8);//ʱ��
	Pin_Configure(PIN_PLC_PWR,//��Դ����������� 0�� ����Դ
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				 (0<<11));	//B11  :��Ϊ���ʱ��0=���0,1=���1	

	Pin_Configure(PIN_PLC_ID,//ģ���������,��������
						0|//B3-B0:���ù���ѡ��AF(0-15)
				 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
				 
	p16timer[0] = 2000/10;//��ʱ(����Դ)
	pTerminalRouter->RouterInitTask++;
	
}


void PLCModulePowerUpInit(u32 PORTn)
{
	u16 * p16timer;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterCheck_S_Timer != 0) return;
	p16timer = (u16 *)Get_ADDR_UARTnTimer(PORTn);
	if(p16timer[0] != 0) return;
	
	if(Pin_Read(PIN_PLC_ID) == 0)
	{// �ز�ģ���Ѳ���
		Pin_Configure(PIN_PLC_RST,//��λ�������0
							0|//B3-B0:���ù���ѡ��AF(0-15)
					 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1		
		p16timer[0] = 1000/10;
		pTerminalRouter->RouterInitTask++;
	}
	else
	{
		WarningOccur(PORTn,(u8 *)"�ز�ģ��û����");	//�澯����,��ڸ澯����͸澯�ַ���
		pTerminalRam->RouterCheck_S_Timer = 3;		//�´μ��ģ�������ʱ: 3S
	}
}


void PLC_ResetInit(u32 PORTn, PLCManageStruct* PLCManage)
{
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	switch(pTerminalRouter->RouterInitTask)
	{
		case 0:
			PLCModuleResetInit(PORTn);
			break;
		case 1:
			PLCModuleResetRelease(PORTn);
			break;
		case 2:
			pTerminalRam->RouterAck_S_Timer = PLC_RECOGNIZE_TIME_OUT;		//20S,�ȴ�·�ɿ����Զ��ϱ�֡
			pTerminalRouter->RouterInitTask++;			
			break;
		case 3:
			if(pTerminalRam->RouterAck_S_Timer == 0)
			{
				PLCManage->State = PLC_MODLE_RECOGNIZE;
				pTerminalRouter->RouterInitTask = 0;
			}
			break;
		default:
			pTerminalRouter->RouterInitTask = 0;
			break;
	}
}


void PLCModuleResetInit(u32 PORTn)
{
	u16 * p16timer;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	p16timer = (u16 *)Get_ADDR_UARTnTimer(PORTn);
	if(p16timer[0]==0)
	{
		Pin_Configure(PIN_PLC_RST,//��λ�������0
								0|//B3-B0:���ù���ѡ��AF(0-15)
					 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
		p16timer[0] = 200/10;//ģ�����г�ʼ����ʱ
		pTerminalRouter->RouterInitTask++;
		//HAL_BuzzerON(BUZZ_MODE_RW_PARA, 20);//err20180806 ������
	}
}

void PLCModuleResetRelease(u32 PORTn)
{
	u16 * p16timer;
	UARTCtrl_TypeDef* UARTCtrl;	
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	p16timer = (u16 *)Get_ADDR_UARTnTimer(PORTn);
	if(p16timer[0] == 0)
	{
		p16timer = (u16 *)Get_ADDR_UARTn(PORTn);
		p16timer[0] = 0;			// �жϽ���ָ��
		p16timer[1] = 0;			// ��ǰ�Ѷ�ָ��
		UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
		UARTCtrl->Function = 1;		// �ڹ��� 0=�ն�ά����,!=0�ն˳�����
		UARTCtrl->Task = 0;			// UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
		UARTCtrl->RxByte = 0;		// RX����(�ѽ����ֽ���)=0
		pTerminalRouter->RUNState = 0;			// ��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
		pTerminalRouter->StopFlags = 0;			// ��ͣ·�ɱ�־
		pTerminalRouter->RouterCtrlTask = 0;	// ·���������������;��ɺ��Զ��ص�0
		pTerminalRouter->RouterCtrl = 0;		// ·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
		UARTCtrl->BpsCtrl = BPS_9600;			// 9600,8e1
		Init_UARTn(PORTn);						// ��ʼ��UART��,���ͨ�ſ�����������
		Pin_OutH(PIN_PLC_RST);					// ���������,�������ʹ����
		pTerminalRouter->RouterInitTask++;		// ·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	}
}


void PLC_ModleRecognizeProcess(u32 PORTn, PLCManageStruct* PLCManage)
{
	u8 Temp[SIZE_8];
	CommunicationParaStruct CommPara;
	QCSGLocalFrameStruct LocalFrame;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;

	switch(pTerminalRouter->RouterInitTask)
	{		
		case PLC_RECOGNIZE_READ_MODLE_INFO:					// ���Ͳ�ѯģ������ģʽ��Ϣ
			PLC_TxReadRunMode(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_RECOGNIZE_WAIT_READ_MODLE_INFO_ACK:		// �ȴ�ģ��ظ�����ģʽ��Ϣ						
			if((PLCManage->Flag & PLC_FLAG_READ_MODLE_INFO_DONE) != 0)
			{
				PLCManage->Flag &= ~PLC_FLAG_READ_MODLE_INFO_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
				MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
				ConvertTerminalAddrToBCD(CommPara.TerminalAddr, CommPara.TerminalLocationCode, Temp);
				
				if(MemCompare(pTerminalRouter->RouterAddr, Temp, METER_ADDR_SIZE) == 0)
				{// PLC ģ���ַ���ն˵�ַ��ȣ���ģ��ʶ����ɣ��������� PLC ģ�鵵��ͬ���׶�
					MR((u32)PLCManage->TerminalAddr, (u32)pTerminalRouter->RouterAddr, METER_ADDR_SIZE);
					pTerminalRouter->RouterInitTask = PLC_RECOGNIZE_DONE;
					pTerminalRouter->RetryCnt = 0;				
				}
				else
				{// PLC ģ���ַ���ն˵�ַ���ȣ�������ʼ������
					pTerminalRouter->RouterInitTask++;
					pTerminalRouter->RetryCnt = 0;
				}
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_RECOGNIZE_INIT_FILE:		// �·���ʼ������
			PLC_TxInitFile(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_RECOGNIZE_WAIT_INIT_FILE_ACK:			
			if((PLCManage->Flag & PLC_FLAG_INIT_FILE_DONE) != 0)
			{// PLC ģ�鵵����ʼ����ɣ����������ʼ��
				PLCManage->Flag &= ~PLC_FLAG_INIT_FILE_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
			}	
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_RECOGNIZE_INIT_TASK:				// ���������ʼ��
			PLC_TxInitTask(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_RECOGNIZE_WAIT_INIT_TASK_ACK:		
			if((PLCManage->Flag & PLC_FLAG_INIT_TASK_DONE) != 0)
			{// PLC ģ�������ʼ����ɣ���������ģ�����ڵ��ַ
				PLCManage->Flag &= ~PLC_FLAG_INIT_TASK_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{ // ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_RECOGNIZE_SET_MODLE_ADDR:			// �����������ڵ��ַ
			PLC_TxSetMasterNodeAddr(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_RECOGNIZE_WAIT_SET_MODLE_ADDR_ACK:			
			if((PLCManage->Flag & PLC_FLAG_SET_MODLE_ADDR_DONE) != 0)
			{// PLC ģ���֪������ɣ������ѯ���ڵ��ַ����
				PLCManage->Flag &= ~PLC_FLAG_SET_MODLE_ADDR_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{ // ��ʱ���ط�����ģ���ַ
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_RECOGNIZE_READ_MODLE_ADDR:			// ���Ͳ�ѯ���ڵ��ַ
			PLC_TxReadMasterNodeAddr(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_RECOGNIZE_WAIT_READ_MODLE_ADDR_ACK:		
			if((PLCManage->Flag & PLC_FLAG_READ_MODLE_ADDR_DONE) != 0)
			{// PLC ģ���ַ��ȡ���
				PLCManage->Flag &= ~PLC_FLAG_READ_MODLE_ADDR_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
				MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
				ConvertTerminalAddrToBCD(CommPara.TerminalAddr, CommPara.TerminalLocationCode, Temp);
				if(MemCompare(pTerminalRouter->RouterAddr, Temp, METER_ADDR_SIZE) == 0)
				{// PLC ģ���ַ���ն˵�ַ��ȣ���ģ��ʶ����ɣ��������� PLC ģ�鵵��ͬ���׶�
					MR((u32)PLCManage->TerminalAddr, (u32)pTerminalRouter->RouterAddr, METER_ADDR_SIZE);
					pTerminalRouter->RouterInitTask++;
					pTerminalRouter->RetryCnt = 0;
				}
				else
				{// PLC ģ���ַ���ն˵�ַ���ȣ�������ʼ������
					pTerminalRouter->RouterInitTask = PLC_RECOGNIZE_INIT_FILE;
					pTerminalRouter->RetryCnt = 0;
					pTerminalRam->RouterAck_S_Timer = 3;
				}	
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{ // ��ʱ���ط���ѯ PLC ģ�����ڵ��ַ
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_RECOGNIZE_DONE:
			//DisplayRouterVer(PORTn);
			PLCManage->State = PLC_FILE_SYNC;
			pTerminalRouter->RouterInitTask = 0;
			pTerminalRouter->RetryCnt = 0;
			break;
		default:
			pTerminalRouter->RouterInitTask = 0;
			break;
	}
}

ResultEnum PLC_TxReadRunMode(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u16* p16;
	u16 NextBps = 0;
	u32 FrameLen = 0, AddrTxBuf = 0;
	UARTCtrl_TypeDef* UARTCtrl;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));					
		pTerminalRam->RouterAck_S_Timer = PLC_RECOGNIZE_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_03;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8000302;
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_INIT;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_RECOGNIZE_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
		return NO_ERROR;
	}

	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	NextBps = GetNextBPS(UARTCtrl->BpsCtrl, PLC_SEARCH_BPS);
	if(NextBps != 0xFFFF)
	{
		UARTCtrl->BpsCtrl = NextBps;
		UARTCtrl->Function = 1;
		UARTCtrl->Task = 0;			
		UARTCtrl->RxByte = 0;
		Init_UARTn(PORTn);
		p16 = (u16 *)Get_ADDR_UARTn(PORTn);
		p16[0] = 0;			// �жϽ���ָ��
		p16[1] = 0;			// ��ǰ�Ѷ�ָ��
		pTerminalRouter->RetryCnt = 0;
	}
	else
	{
		if(pTerminalRouter->RouterResetCnt < PLC_RESET_MAX_CNT)
		{
			PLCManage->State = PLC_RESET_INIT;
			pTerminalRouter->RouterResetCnt++;		
			pTerminalRouter->RouterInitTask = 0;
			pTerminalRouter->RetryCnt = 0;
		}
		else
		{
			PLCManage->State = PLC_MODLE_ERROR;
			WarningOccur(PORTn,(u8 *)"�ز�ģ���޷�ʶ��");
		}
	}
	return NO_ERROR;
}

ResultEnum PLC_TxInitFile(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_RECOGNIZE_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_01;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020102;
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_INIT;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_RECOGNIZE_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{		
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}
	return NO_ERROR;	
}

ResultEnum PLC_TxInitTask(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));		
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_01;//��ʼ��ģ��
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020103;//��ʼ������
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_INIT;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_RECOGNIZE_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
			pTerminalRam->RouterAck_S_Timer = PLC_WAIT_ACK_TIME_OUT;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

ResultEnum PLC_TxSetMasterNodeAddr(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u8 Temp[8];
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	CommunicationParaStruct CommPara;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
		MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		ConvertTerminalAddrToBCD(CommPara.TerminalAddr, CommPara.TerminalLocationCode, Temp);
		pTerminalRam->RouterAck_S_Timer = PLC_RECOGNIZE_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_04;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020401;
		LocalFrame->DataLen = METER_ADDR_SIZE;
		MR((u32)LocalFrame->Data, (u32)Temp, METER_ADDR_SIZE);

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_INIT;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_RECOGNIZE_TIME_OUT;
		UartTxReq.DataLen = (u16)FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}
	return NO_ERROR;	
}

ResultEnum PLC_TxReadMasterNodeAddr(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_RECOGNIZE_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_03;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8000303;
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_INIT;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_RECOGNIZE_TIME_OUT;
		UartTxReq.DataLen = (u16)FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}
	return NO_ERROR;	
}


void PLC_ModleFileSync(u32 PORTn, PLCManageStruct* PLCManage)
{
	u16 Index = 0, CurReadNodeNum = 0;
	QCSGLocalFrameStruct LocalFrame;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	u8* MeterAddrStart = (u8*)ADDR_PLC_AMR_DataBuf;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;

	switch(pTerminalRouter->RouterInitTask)
	{
		case PLC_FILE_SYNC_READ_NODE_NUM:
			WarningOccur(PLC_PORT, (u8*)"�ز�����ͬ������...");
			PLC_FileSyncTxReadNodeNum(PORTn, PLCManage, &LocalFrame);
			break; 	
		case PLC_FILE_SYNC_WAIT_READ_NODE_NUM_ACK:			
			if((PLCManage->Flag & PLC_FLAG_READ_NODE_NUM_DONE) != 0)
			{// PLC ģ���ȡ�ӽڵ�������ɣ����������ʼ��
				PLCManage->Flag &= ~PLC_FLAG_READ_NODE_NUM_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				if(MeterFileInfo->PLCMeters != pTerminalRouter->TotalNodeNum)
				{// ģ��ӽڵ��������ն˲�һ�£�������ʼ����������
					pTerminalRouter->RouterInitTask++;
					pTerminalRouter->RetryCnt = 0;
				}
				else
				{// ģ��ӽڵ��������ն�һ�£�������ѯ�ӽڵ���Ϣ����
				
					//#if(TEST_ON == 1)//err20180626 ��ʱȥ��
					//	pTerminalRouter->RouterInitTask++;
					//	pTerminalRouter->RetryCnt = 0;
					//#else
						pTerminalRouter->RouterInitTask = PLC_FILE_SYNC_READ_NODE_INFO;
						pTerminalRouter->RetryCnt = 0;
						pTerminalRouter->CurReadNodeNum = 0;
						pTerminalRouter->ReadNodeNum = 0;
					//#endif	
				}	
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_FILE_SYNC_INIT_FILE:		// �·���ʼ������
			PLC_TxInitFile(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_FILE_SYNC_WAIT_INIT_FILE_ACK:		
			if((PLCManage->Flag & PLC_FLAG_INIT_FILE_DONE) != 0)
			{// PLC ģ�鵵����ʼ����ɣ��������Ӵӽڵ㲽��
				PLCManage->Flag &= ~PLC_FLAG_INIT_FILE_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
				pTerminalRouter->AddNodeNum = 0;
				pTerminalRouter->CurAddNodeNum = 0;
				pTerminalRouter->TempCount = 0;
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_FILE_SYNC_ADD_NODE:		// �·����Ӵӽڵ�
			PLC_FileSyncTxAddNode(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_FILE_SYNC_WAIT_ADD_NODE_ACK:			
			if((PLCManage->Flag & PLC_FLAG_ADD_NODE_DONE) != 0)
			{// PLC �������Ӵӽڵ���Ϣ��ɣ�
				PLCManage->Flag &= ~PLC_FLAG_ADD_NODE_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				pTerminalRouter->AddNodeNum += pTerminalRouter->CurAddNodeNum;
				if(pTerminalRouter->AddNodeNum < MeterFileInfo->PLCMeters)
				{// �����ӵĴӽڵ�С���ܽڵ�������������
					pTerminalRouter->RouterInitTask--;
					pTerminalRouter->RetryCnt = 0;
				}
				else
				{// ��ʣ��ӽڵ㣬������ѯ�ӽڵ���Ϣ
					//pTerminalRouter->RouterInitTask++;
					pTerminalRouter->RouterInitTask = PLC_FILE_SYNC_READ_NODE_NUM;//err20180626 ̨����� ���Ӵӽڵ���ɺ����²�ѯ�ӽڵ�����
					pTerminalRouter->CurReadNodeNum = 0;
					pTerminalRouter->ReadNodeNum = 0;
					pTerminalRouter->RetryCnt = 0;
				}
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_FILE_SYNC_READ_NODE_INFO:		// �·���ѯ�ӽڵ���Ϣ
			PLC_FileSyncTxReadNodeInfo(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_FILE_SYNC_WAIT_READ_NODE_INFO_ACK:
			if((PLCManage->Flag & PLC_FLAG_READ_NODE_INFO_DONE) != 0)
			{// PLC ���ζ�ȡ�ӽڵ���Ϣ��ɣ�
				PLCManage->Flag &= ~PLC_FLAG_READ_NODE_INFO_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				CurReadNodeNum = pTerminalRouter->CurReadNodeNum;
				Index = CompareMeterAddrInNodeList(MeterAddrStart, CurReadNodeNum, NodeList, MAX_PN_NUM);
				if(Index < CurReadNodeNum)
				{// �ȶԴӽڵ��ַʱ����ѯ���� PLC �ز����б��д��ڲ���ƥ��ĵ�ַ����ص���ʼ���������������Ӵӽڵ�
					pTerminalRouter->RouterInitTask = PLC_FILE_SYNC_INIT_FILE;
					pTerminalRouter->RetryCnt = 0;
				}
				else
				{// ���δӽڵ���Ϣ�ȶԳɹ������ж��Ƿ���ʣ��ӽڵ���Ҫ�ȶ�
					PLC_FileSyncSetOkFlag(MeterAddrStart, CurReadNodeNum, NodeList);
					pTerminalRouter->ReadNodeNum += CurReadNodeNum;
					if((pTerminalRouter->ReadNodeNum < MeterFileInfo->PLCMeters) && (CurReadNodeNum != 0))
					{
						pTerminalRouter->RouterInitTask--;
						pTerminalRouter->RetryCnt = 0;
					}
					else
					{// ��ʣ��ӽڵ���Ҫ�ȶԣ�������ʼ��������
						pTerminalRouter->RouterInitTask++;
						pTerminalRouter->RetryCnt = 0;
					}
				}
			}	
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{// ��ʱ���ط���ѯ�ӽڵ���Ϣ
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_FILE_SYNC_INIT_TASK:		// ���������ʼ��
			PLC_TxInitTask(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_FILE_SYNC_WAIT_INIT_TASK_ACK:			
			if((PLCManage->Flag & PLC_FLAG_INIT_TASK_DONE) != 0)
			{// PLC ģ�������ʼ����ɣ���������ģ�鵵��ͬ�����
				PLCManage->Flag &= ~PLC_FLAG_INIT_TASK_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{ // ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;			
		case PLC_FILE_SYNC_DONE:
			if(pTerminalRam->RouterAck_S_Timer == 0)
			{// ģ�鵵����ɣ����볭������״̬���ȴ���������
				WarningCancel(PLC_PORT);
				PLCManage->State = PLC_AMR_STATE;
				MeterFileInfo->PLCMeterSyncFlag = 0;
				pTerminalRouter->RouterInitTask = 0;
				pTerminalRouter->RetryCnt = 0;
			}		
			break;
		default:
			pTerminalRouter->RouterInitTask = 0;
			break;
		
	}
}

void PLC_FileSyncSetOkFlag(u8* MeterList, u32 MeterListNum, NodeListStruct* NodeList)
{
	u32 i = 0, Index = 0;
	for(i = 0; i < MeterListNum; i++)
	{
		Index = FindMeterInNodeList(&MeterList[i*METER_ADDR_SIZE], NodeList, MAX_PN_NUM);
		if(Index < MAX_PN_NUM)
		{
			NodeList[Index].Flag |= NODE_FLAG_SYNC_TO_PLC_OK;
			NodeList[Index].Flag &= ~NODE_FLAG_NEED_FILE_SYNC;
		}
	}
}

ResultEnum PLC_FileSyncTxReadNodeNum(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_03;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8000305;
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_FILE_SYNC;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

ResultEnum PLC_FileSyncTxAddNode(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u16 i = 0, TempAddNodeNum = 0;
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_04;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020402;
		LocalFrame->DataLen = 0;
		if(MeterFileInfo->PLCMeters > pTerminalRouter->AddNodeNum)
		{
			if((MeterFileInfo->PLCMeters - pTerminalRouter->AddNodeNum) > pTerminalRouter->MaxRWNodeNum)
			{
				TempAddNodeNum = pTerminalRouter->MaxRWNodeNum;
			}
			else
			{
				TempAddNodeNum = MeterFileInfo->PLCMeters - pTerminalRouter->AddNodeNum;
			}
		}
		else
		{
			TempAddNodeNum = 0;
		}

		if(TempAddNodeNum == 0)
		{
			pTerminalRouter->RouterInitTask = PLC_FILE_SYNC_READ_NODE_INFO;
			return ERROR_WRONG_ACCESS;
		}

		LocalFrame->DataLen++;
		for(i = 0; i < TempAddNodeNum; i++)
		{
			pTerminalRouter->TempCount = FindPortNextMeterInNodeList(PORT_ID_PLC, NodeList, pTerminalRouter->TempCount, MAX_PN_NUM);
			if(pTerminalRouter->TempCount >= MAX_PN_NUM) break;
			MR((u32)&LocalFrame->Data[LocalFrame->DataLen], (u32)NodeList[pTerminalRouter->TempCount].Addr, METER_ADDR_SIZE);
			pTerminalRouter->TempCount++;
			LocalFrame->DataLen += METER_ADDR_SIZE;
		}

		pTerminalRouter->CurAddNodeNum = i;
		LocalFrame->Data[0] = pTerminalRouter->CurAddNodeNum;
		
		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_FILE_SYNC;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}
	return NO_ERROR;
}

ResultEnum PLC_FileSyncTxReadNodeInfo(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_03;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8030306;
		LocalFrame->DataLen = 0;
		LocalFrame->Data[LocalFrame->DataLen++] = pTerminalRouter->ReadNodeNum;
		LocalFrame->Data[LocalFrame->DataLen++] = pTerminalRouter->ReadNodeNum >> 8;
		LocalFrame->Data[LocalFrame->DataLen++] = pTerminalRouter->MaxRWNodeNum;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_FILE_SYNC;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}


ResultEnum PLC_FileAddNodeTxAddNode(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u16 i = 0, TempAddNodeNum = 0;
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_04;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020402;
		LocalFrame->DataLen = 0;

		LocalFrame->DataLen++;
		for(i = pTerminalRouter->TempCount; i < MAX_PN_NUM; i++)
		{// pTerminalRouter->TempCount�ڽ������ӽڵ�״̬֮ǰ�������㣬Ŀ�����ڶ�����ӽڵ�ʱ������ÿ�ζ���ͷ��ʼ����
			if(TempAddNodeNum >= pTerminalRouter->MaxRWNodeNum) break;
			if((NodeList[i].PortID == PORT_ID_PLC) && ((NodeList[i].Flag & NODE_FLAG_NEED_FILE_SYNC) != 0))
			{
				MR((u32)&LocalFrame->Data[LocalFrame->DataLen], (u32)NodeList[i].Addr, METER_ADDR_SIZE);
				LocalFrame->DataLen += METER_ADDR_SIZE;
				TempAddNodeNum++;
			}			
		}
		if(TempAddNodeNum == 0)
		{
			pTerminalRouter->RouterInitTask = PLC_FILE_ADD_DONE;
			return ERROR_WRONG_ACCESS;
		}
		pTerminalRouter->TempCount = i;
		LocalFrame->Data[0] = TempAddNodeNum;
		
		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_ADD_NODE;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}
	return NO_ERROR;
}

void PLC_ModleFileAddNode(u32 PORTn, PLCManageStruct* PLCManage)
{
	u16 i = 0;
	QCSGLocalFrameStruct LocalFrame;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;

	switch(pTerminalRouter->RouterInitTask)
	{
		case PLC_FILE_ADD_ADD_NODE:		// �·����Ӵӽڵ�
			PLC_FileAddNodeTxAddNode(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_FILE_ADD_WAIT_ADD_NODE_ACK:			
			if((PLCManage->Flag & PLC_FLAG_ADD_NODE_DONE) != 0)
			{// PLC �������Ӵӽڵ���Ϣ��ɣ�
				PLCManage->Flag &= ~PLC_FLAG_ADD_NODE_DONE;
				pTerminalRam->RouterAck_S_Timer = 0;
				for(i = 0; i < pTerminalRouter->TempCount; i++)
				{
					if((NodeList[i].PortID == PORT_ID_PLC) && 
					   ((NodeList[i].Flag & NODE_FLAG_NEED_FILE_SYNC) != 0))
					{
						NodeList[i].Flag &= ~NODE_FLAG_NEED_FILE_SYNC;
						NodeList[i].Flag |= NODE_FLAG_SYNC_TO_PLC_OK;
					}			
				}
				for(i = pTerminalRouter->TempCount; i < MAX_PN_NUM; i++)
				{
					if((NodeList[i].PortID == PORT_ID_PLC) && 
					   ((NodeList[i].Flag & NODE_FLAG_NEED_FILE_SYNC) != 0))
					{
						break;
					}			
				}
				if(i < MAX_PN_NUM)
				{
					pTerminalRouter->RouterInitTask--;
					pTerminalRouter->RetryCnt = 0;
				}
				else
				{
					pTerminalRouter->RouterInitTask++;
					pTerminalRouter->RetryCnt = 0;
				}
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{// ��ʱ���ط����ӽڵ�
					pTerminalRouter->RouterInitTask--;
					pTerminalRouter->TempCount = 0;
				}
			}
			break;
		case PLC_FILE_ADD_DONE:
			if(pTerminalRam->RouterAck_S_Timer == 0)
			{// ģ�鵵����ɣ����볭������״̬���ȴ���������
				PLCManage->State = PLC_AMR_STATE;
				MeterFileInfo->PLCMeterSyncFlag = 0;
				pTerminalRouter->RouterInitTask = 0;
				pTerminalRouter->RetryCnt = 0;
				pTerminalRouter->TempCount = 0;
			}		
			break;
		default:
			pTerminalRouter->RouterInitTask = 0;
			break;
		
	}
}



void PLC_ModleAMR_Task(u32 PORTn, PLCManageStruct* PLCManage)
{
	RealTimeStruct RealTime;	
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	AMRCtrlStruct* AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	
	HAL_GetSystemTime(&RealTime);
	if(RealTime.u8Minute == 0x00)
	{
		AMRCtrl->Flag |= AMR_FLAG_TIME_CROSS_HOUR;
	}
	if((MeterFileInfo->PLCMeters == 0) && ((RealTime.u8Second & 0x07) == 0) && 
	   (AMRCtrl->State > AMR_Idle) && (AMRCtrl->State < AMR_Pause))
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
	}
	
	switch(AMRCtrl->State)
	{
		case AMR_Idle:
			PLC_AMR_IdleProcess(PORTn, AMRCtrl, PLCManage, &RealTime);
			break;
		case AMR_Tx:
			PLC_AMR_TxProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_WaitRx:
			PLC_AMR_WaitRxProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_Pause:
			PLC_AMR_PauseProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_RealTimeTx:
			PLC_AMR_RealTimeTxProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_RealTimeWaitRx:
			PLC_AMR_WaitRealTimeRxProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_Resume:
			PLC_AMR_ResumeProcess(PORTn, AMRCtrl, PLCManage);
			break;
		default:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
	}
}


ResultEnum PLC_AMR_IdleProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, RealTimeStruct* RealTime)
{
	u16* pAckTimer;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;

	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if((pAckTimer == NULL) || (pAckTimer[0] != 0)) return ERROR_WRONG_TIME;
	
	if(MeterFileInfo->PLCMeterSyncFlag == METER_FILE_SYNC_TO_PLC_FLAG)
	{
		PLCManage->State = PLC_FILE_SYNC;
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
		pTerminalRouter->TempCount = 0;
		return NO_ERROR;
	}
	
	if(AMRCtrl == NULL) return ERROR_POINTER_NULL;
	if(PLCManage == NULL) return ERROR_POINTER_NULL;
	if(RealTime == NULL) return ERROR_POINTER_NULL;

	if(AMRCtrl->LastCheckTime.u8Minute == RealTime->u8Minute) return ERROR_WRONG_TIME;
	DisplayRouterVer(PORTn);

#if(TEST_ON == 0)//err20180711 ̨�����ʱ���ղ�ͬ����������ֹӰ��ģ�����������
	if(AMRCtrl->LastCheckTime.u8Day != RealTime->u8Day)
	{
		PLCManage->State = PLC_FILE_SYNC;
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
		pTerminalRouter->TempCount = 0;

		AMRCtrl->LastCheckTime.u8Second = RealTime->u8Second;
		AMRCtrl->LastCheckTime.u8Minute = RealTime->u8Minute;
		AMRCtrl->LastCheckTime.u8Hour   = RealTime->u8Hour;
		AMRCtrl->LastCheckTime.u8Day    = RealTime->u8Day;
		AMRCtrl->LastCheckTime.u8Month  = RealTime->u8Month;
		AMRCtrl->LastCheckTime.u8Year   = RealTime->u8Year;
		AMRCtrl->LastCheckTime.u8Week   = RealTime->u8Week;
		AMRCtrl->LastCheckTime.u8Sync   = RealTime->u8Sync;
		return NO_ERROR;
	}
#endif 
	AMRCtrl->LastCheckTime.u8Second = RealTime->u8Second;
	AMRCtrl->LastCheckTime.u8Minute = RealTime->u8Minute;
	AMRCtrl->LastCheckTime.u8Hour   = RealTime->u8Hour;
	AMRCtrl->LastCheckTime.u8Day    = RealTime->u8Day;
	AMRCtrl->LastCheckTime.u8Month  = RealTime->u8Month;
	AMRCtrl->LastCheckTime.u8Year   = RealTime->u8Year;
	AMRCtrl->LastCheckTime.u8Week   = RealTime->u8Week;
	AMRCtrl->LastCheckTime.u8Sync   = RealTime->u8Sync;

	if(MeterFileInfo->PLCMeters == 0) return ERROR_DestAddrNotExist;	
	AMRCtrl->TotalMeters = MeterFileInfo->PLCMeters;
	
	AMRCtrl->AMR_ID = 0;
	if((RealTime->u8Year != AMRCtrl->LastDayFrozenTime.u8Year) || 
	   (RealTime->u8Month != AMRCtrl->LastDayFrozenTime.u8Month))
	{
		AMRCtrl->MonthFreezFailMeters = 0;
		AMRCtrl->MonthFreezOkMeters = 0;
		MeterFileInfo->AMRMonthFreezFailMeters = 0;
	}
	
	if((RealTime->u8Year != AMRCtrl->LastDayFrozenTime.u8Year) || 
	   (RealTime->u8Month != AMRCtrl->LastDayFrozenTime.u8Month) || 
	   (RealTime->u8Day != AMRCtrl->LastDayFrozenTime.u8Day))
	{
		#if (TEST_ON == 0)
			if(RealTime->u8Minute > 0x04)
		#endif
		{
			AMRCtrl->AMR_ID |= AMR_DAY_FREEZ_ID;
			AMRCtrl->DayFreezFailMeters = 0;
			AMRCtrl->DayFreezOkMeters = 0;
			MeterFileInfo->AMRDayFreezFailMeters = 0;
			MC(0, (u32)ADDR_MeterDayFreezFailList, LEN_MeterDayFreezFailList);
		}	
	}

	if((RealTime->u8Year != AMRCtrl->LastHourFrozenTime.u8Year) || 
	   (RealTime->u8Month != AMRCtrl->LastHourFrozenTime.u8Month) || 
	   (RealTime->u8Day != AMRCtrl->LastHourFrozenTime.u8Day) || 
	   (RealTime->u8Hour != AMRCtrl->LastHourFrozenTime.u8Hour))
	{
		#if (TEST_ON == 0)
		if(RealTime->u8Minute > 0x04)
		#endif
		{
			AMRCtrl->AMR_ID |= AMR_HOUR_FREEZ_ID;
			AMRCtrl->HourFreezFailMeters = 0;
			AMRCtrl->HourFreezOkMeters = 0;
			if(AMRCtrl->DayFreezFailMeters != 0)
			{
				AMRCtrl->AMR_ID |= AMR_DAY_FREEZ_PATCH_ID;
			}
		}	
	}

	AMRTaskManageCheckNormalTask(PORTn, AMRCtrl, RealTime);
	AMRTaskManageCheckRelayTask(PORTn, AMRCtrl, RealTime);
	CheckNormalTasktoExecute(PORTn, AMRCtrl, RealTime);
	CheckRelayTasktoExecute(PORTn, AMRCtrl, RealTime);
	CheckAlarmTasktoExecute(PORTn, AMRCtrl, RealTime);
	if(AMRCtrl->AMR_ID == 0) return ERROR_NO_AMR_TASK;

	if((AMRCtrl->AMR_ID & (AMR_DAY_FREEZ_ID | AMR_DAY_FREEZ_PATCH_ID)) != 0)
	{
		AMRCtrl->LastDayFrozenTime.u8Second = RealTime->u8Second;
		AMRCtrl->LastDayFrozenTime.u8Minute = RealTime->u8Minute;
		AMRCtrl->LastDayFrozenTime.u8Hour   = RealTime->u8Hour;
		AMRCtrl->LastDayFrozenTime.u8Day    = RealTime->u8Day;
		AMRCtrl->LastDayFrozenTime.u8Month  = RealTime->u8Month;
		AMRCtrl->LastDayFrozenTime.u8Year   = RealTime->u8Year;
		AMRCtrl->LastDayFrozenTime.u8Week   = RealTime->u8Week;
		AMRCtrl->LastDayFrozenTime.u8Sync   = RealTime->u8Sync;
	}

	if((AMRCtrl->AMR_ID & AMR_HOUR_FREEZ_ID) != 0)
	{
		AMRCtrl->LastHourFrozenTime.u8Second = RealTime->u8Second;
		AMRCtrl->LastHourFrozenTime.u8Minute = RealTime->u8Minute;
		AMRCtrl->LastHourFrozenTime.u8Hour   = RealTime->u8Hour;
		AMRCtrl->LastHourFrozenTime.u8Day    = RealTime->u8Day;
		AMRCtrl->LastHourFrozenTime.u8Month  = RealTime->u8Month;
		AMRCtrl->LastHourFrozenTime.u8Year   = RealTime->u8Year;
		AMRCtrl->LastHourFrozenTime.u8Week   = RealTime->u8Week;
		AMRCtrl->LastHourFrozenTime.u8Sync   = RealTime->u8Sync;
	}

	AMRCtrl->AMR_ID_Step = 0x0001;
	if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->AMR_ID = 0;
		return ERROR_NO_AMR_TASK;
	}

	while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->AMR_ID_Step <<= 1;
		if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
		{
			return ERROR_NO_AMR_TASK;
		}
	}
	
	while(Get_NextAMR_NW_DI(AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->MeterIndex++;
		while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
		{
			AMRCtrl->AMR_ID_Step <<= 1;
			if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
			{
				return ERROR_NO_AMR_TASK;
			}
		}
	}

	while(Get_NextAMR_645_DI(AMRCtrl) != NO_ERROR)
	{
		AMRCtrl->CurNW_DI_Step++;
		while(Get_NextAMR_NW_DI(AMRCtrl) != NO_ERROR)
		{
			AMRCtrl->MeterIndex++;
			while(Get_NextMeter(PORTn, AMRCtrl) != NO_ERROR)
			{
				AMRCtrl->AMR_ID_Step <<= 1;
				if(Get_NextAMR_ID(AMRCtrl) != NO_ERROR)
				{
					return ERROR_NO_AMR_TASK;
				}
			}
		}
	}
	
	AMRCtrl->TryCnt = 0;
	AMRCtrl->State = AMR_Tx;
	AMRCtrl->FailMeters = 0;
	AMRCtrl->SuccessMeters = 0;
	AMRCtrl->PLCTaskIDAckNum = 0;
	AMRCtrl->PLCTaskIDAddNum = 0;
	PLCManage->DataLenInDataBuf = 0;
	PLCManage->ReportDataLen = 0;
	pTerminalRouter->RouterInitTask = 0;
	pTerminalRouter->RetryCnt = 0;

	if((AMRCtrl->AMR_ID_Step & (AMR_DAY_FREEZ_ID | AMR_HOUR_FREEZ_ID)) != 0)
	{
		AMR_NUM_Display(PORTn, AMRCtrl->TotalMeters, AMRCtrl->SuccessMeters, AMRCtrl->FailMeters);
	}
	return NO_ERROR;
	
}


ResultEnum PLC_AMR_TxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	ResultEnum Result = NO_ERROR;
	switch(AMRCtrl->AMR_ID_Step)
	{
		case AMR_DAY_FREEZ_ID:
			Result = PLC_AMR_AddTaskProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_DAY_FREEZ_PATCH_ID:
			Result = PLC_AMR_AddTaskProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_HOUR_FREEZ_ID:
			Result = PLC_AMR_AddTaskProcess(PORTn, AMRCtrl, PLCManage);
			break;	
		case AMR_NORMAL_TASK_ID:
			Result = PLC_AMR_AddTaskProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_RELAY_TASK_ID:
			Result = PLC_AMR_AddTaskProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_ALARM_TASK_ID:
			Result = PLC_AMR_AddTaskProcess(PORTn, AMRCtrl, PLCManage);
			break;
		case AMR_EVENT_TASK_ID:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
		default:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
	}
	return Result;
}

ResultEnum PLC_AMR_AddTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	u16* pPortAckTimer;
	QCSGLocalFrameStruct LocalFrame;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	ResultEnum Result = NO_ERROR;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	switch(pTerminalRouter->RouterInitTask)
	{
		case PLC_ADD_TASK_STOP_TASK:
			Result = PLC_AMR_ADD_TASK_TxStopTask(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_ADD_TASK_STOP_TASK_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_STOP_TASK_DONE) != 0)//ģ��ظ�ȷ�ϴ�����
			{// PLC  ����������ֹͣ����
				PLCManage->Flag &= ~PLC_FLAG_STOP_TASK_DONE;
				pPortAckTimer[0] = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط�ֹͣ����
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_ADD_TASK_REMAIN_TASK:
			Result = PLC_TxCheckRemainTask(PORTn, PLCManage, &LocalFrame);
			break;
		/*case PLC_ADD_TASK_REMAIN_TASK_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_INIT_TASK_DONE) != 0)
			{// PLC ������ʼ���������
				PLCManage->Flag &= ~PLC_FLAG_INIT_TASK_DONE;
				pPortAckTimer[0] = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
				//AMRCtrl->PLCTaskIDAckNum = 0;
				//AMRCtrl->PLCTaskIDAddNum = 0;
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
*/
			
		case PLC_ADD_TASK_INIT:
			Result = PLC_TxInitTask(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_ADD_TASK_INIT_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_INIT_TASK_DONE) != 0)
			{// PLC ������ʼ���������
				PLCManage->Flag &= ~PLC_FLAG_INIT_TASK_DONE;
				pPortAckTimer[0] = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
				//AMRCtrl->PLCTaskIDAckNum = 0;
				//AMRCtrl->PLCTaskIDAddNum = 0;
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_ADD_TASK_ADD_ONE:
			if(AMRCtrl->AMR_ID_Step == AMR_RELAY_TASK_ID)
			{
				Result = PLC_AMR_ADD_OneRelayTask(PORTn, AMRCtrl, PLCManage, &LocalFrame);
			}
			else
			{
				Result = PLC_AMR_ADD_OneTask(PORTn, AMRCtrl, PLCManage, &LocalFrame);
			}
			break;
		case PLC_ADD_TASK_ADD_ONE_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_ADD_ONE_TASK_DONE) != 0)
			{// PLC ��������������ɣ�
				PLCManage->Flag &= ~PLC_FLAG_ADD_ONE_TASK_DONE;
				pPortAckTimer[0] = 0;
				if(AMRCtrl->AMR_ID_Step < AMR_NORMAL_TASK_ID)
				{
					PLCManage->AMR_Data[AMRCtrl->CurNW_DI_Step].NW_DI = AMRCtrl->CurNW_DI;
					PLCManage->AMR_Data[AMRCtrl->CurNW_DI_Step].TblIndex = AMRCtrl->CurNW_DI_TblIndex;
					PLCManage->AMR_Data[AMRCtrl->CurNW_DI_Step].DataLen = 0;				
					PLCManage->AMR_Data[AMRCtrl->CurNW_DI_Step].DI645Num++;
					PLCManage->AMR_Data[AMRCtrl->CurNW_DI_Step].TaskIDNum++;
					PLCManage->AMR_Data[AMRCtrl->CurNW_DI_Step].TaskIDList[AMRCtrl->Cur645_DI_Step] = AMRCtrl->PLCTaskIDAddNum;
				}
				else if(AMRCtrl->AMR_ID_Step == AMR_NORMAL_TASK_ID)
				{
					AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].NW_DI = AMRCtrl->CurNW_DI;
					AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].TblIndex = AMRCtrl->CurNW_DI_TblIndex;
					AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].DataLen = 0;
					AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].DI645Num++;
					AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].TaskIDNum++;
					AMRCtrl->NormalTask->DataList[AMRCtrl->CurNW_DI_Step].TaskIDList[AMRCtrl->Cur645_DI_Step] = AMRCtrl->PLCTaskIDAddNum;
				}
				/*else if(AMRCtrl->AMR_ID_Step == AMR_ALARM_TASK_ID)
				{
					//err20190220 �޸��ز���������
					u8 Pos=0,Bit=0;
					Pos = AMRCtrl->PLCTaskIDAddNum >> 3;
					Bit = AMRCtrl->PLCTaskIDAddNum & 0x07;	
					AMRCtrl->PLCTaskIDFlagList[Pos] |= 1 << Bit;	
				}*/
				else
				{
				}
				
				AMRCtrl->PLCTaskIDAddNum++;
				Result = PLC_AMR_ADD_TASK_SwitchToNextTaskID(PORTn, AMRCtrl, PLCManage);
				if(Result == NO_ERROR)//����������Ҫ�����򷵻���һ��������������
				{
					pTerminalRouter->RouterInitTask--;
					pTerminalRouter->RetryCnt = 0;
				}
				else
				{
					pTerminalRouter->RouterInitTask++;
					pTerminalRouter->RetryCnt = 0;
				}
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط���������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_ADD_TASK_START_TASK:
			Result = PLC_AMR_ADD_TASK_TxStartTask(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_ADD_TASK_START_TASK_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_START_TASK_DONE) != 0)
			{// PLC ��������������ɣ�
				PLCManage->Flag &= ~PLC_FLAG_START_TASK_DONE;
				pPortAckTimer[0] = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_ADD_TASK_DONE:
			if(pPortAckTimer[0] == 0)
			{
				AMRCtrl->State = AMR_WaitRx;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
				pPortAckTimer[0] = PLCManage->AMRTimeOut;
				AMR_ADDR_Display(PORTn, AMRCtrl->Addr);
			}
			break;
		default:
			if(pPortAckTimer[0] == 0) pTerminalRouter->RouterInitTask++;	
			// �˴����� pTerminalRouter->RouterInitTask ֱ���� 0������Ϊ�л�������ַǰ�ո���ģ�鷢����
			// ���һ�����������ϱ���ȷ����Ϣ����Ҫ�ȴ�ȷ����Ϣ������ϣ��������д˲����������Ϊ����
			// ����ĵ�һ�����贮�ڱ�ռ�ã������ó�ʱʱ��Ϊ 3 S��ʱ��Ƚϳ����˷�ʱ�䡣
		break;
	}
	
	return Result;
}

ResultEnum PLC_AMR_ADD_TASK_TxStopTask(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	u16* pPortAckTimer;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer[0] != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_02;//��������
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020209;//��ͣ����
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
			pPortAckTimer[0] = PLC_WAIT_ACK_TIME_OUT;
		}	
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

ResultEnum PLC_AMR_ADD_OneTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	DLT645FrameStruct DLT645Frame;
	UartTxReqStruct UartTxReq;
	u16* pPortAckTimer;
	u32 FrameLen = 0, AddrTxBuf = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer[0] != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pPortAckTimer[0] = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT;
		MR((u32)LocalFrame->SrcAddr, (u32)PLCManage->TerminalAddr, METER_ADDR_SIZE);
		MR((u32)LocalFrame->DstAddr, (u32)AMRCtrl->Addr, METER_ADDR_SIZE);
		LocalFrame->AFN = LOCAL_FRAME_AFN_02;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020201;
		LocalFrame->DataLen = 0;
		LocalFrame->Data[LocalFrame->DataLen++] = AMRCtrl->PLCTaskIDAddNum;
		LocalFrame->Data[LocalFrame->DataLen++] = AMRCtrl->PLCTaskIDAddNum >> 8;
		LocalFrame->Data[LocalFrame->DataLen++] = PLC_TASK_LEVEL_3 | PLC_TASK_RESP_BIT;

		if(AMRCtrl->PMType == METER_TYPE_SINGLE)
		{
			PLCManage->AMRTimeOut = PLC_AMR_SIGLE_TIME_OUT;
		}
		else
		{
			PLCManage->AMRTimeOut = PLC_AMR_MULTI_TIME_OUT;
		}
		LocalFrame->Data[LocalFrame->DataLen++] = PLCManage->AMRTimeOut;
		LocalFrame->Data[LocalFrame->DataLen++] = PLCManage->AMRTimeOut >> 8;
		MC(0, (u32)&DLT645Frame, sizeof(DLT645FrameStruct));
		BuildMeterAMR645FrameStruct(&DLT645Frame, AMRCtrl);
		LocalFrame->Data[LocalFrame->DataLen] = Build645Frame(&DLT645Frame, &LocalFrame->Data[LocalFrame->DataLen + 1], NO_FE_IN_645);
		LocalFrame->DataLen += LocalFrame->Data[LocalFrame->DataLen] + 1;
		
		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}	
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

ResultEnum PLC_AMR_ADD_TASK_SwitchToNextTaskID(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	AMRCtrl->Cur645_DI_Step++;
	if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR) return NO_ERROR;

	AMRCtrl->CurNW_DI_Step++;
	while(Get_NextAMR_NW_DI(AMRCtrl) == NO_ERROR)
	{
		if(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
		{
			return NO_ERROR;
		}
		AMRCtrl->CurNW_DI_Step++;
	}

	return ERROR_WRONG_ACCESS;
}

ResultEnum PLC_AMR_ADD_TASK_TxStartTask(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	u16* pPortAckTimer;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer[0] != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pPortAckTimer[0] = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_02;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020208;
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}	
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

/*
ResultEnum PLC_AMR_ADD_OneNormalTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	DLT645FrameStruct DLT645Frame;
	UartTxReqStruct UartTxReq;
	u16* pPortAckTimer;
	u32 FrameLen = 0, AddrTxBuf = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer[0] != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pPortAckTimer[0] = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT;
		MR((u32)LocalFrame->SrcAddr, (u32)PLCManage->TerminalAddr, METER_ADDR_SIZE);
		MR((u32)LocalFrame->DstAddr, (u32)AMRCtrl->Addr, METER_ADDR_SIZE);
		LocalFrame->AFN = LOCAL_FRAME_AFN_02;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020201;
		LocalFrame->DataLen = 0;
		LocalFrame->Data[LocalFrame->DataLen++] = AMRCtrl->PLCTaskIDAddNum;
		LocalFrame->Data[LocalFrame->DataLen++] = AMRCtrl->PLCTaskIDAddNum >> 8;
		LocalFrame->Data[LocalFrame->DataLen++] = PLC_TASK_LEVEL_3 | PLC_TASK_RESP_BIT;

		if(AMRCtrl->PMType == METER_TYPE_SINGLE)
		{
			PLCManage->AMRTimeOut = PLC_AMR_SIGLE_TIME_OUT;
		}
		else
		{
			PLCManage->AMRTimeOut = PLC_AMR_MULTI_TIME_OUT;
		}
		LocalFrame->Data[LocalFrame->DataLen++] = PLCManage->AMRTimeOut;
		LocalFrame->Data[LocalFrame->DataLen++] = PLCManage->AMRTimeOut >> 8;
		MC(0, (u32)&DLT645Frame, sizeof(DLT645FrameStruct));
		BuildMeterAMR645FrameStruct(&DLT645Frame, AMRCtrl);
		LocalFrame->Data[LocalFrame->DataLen] = Build645Frame(&DLT645Frame, &LocalFrame->Data[LocalFrame->DataLen + 1], NO_FE_IN_645);
		LocalFrame->DataLen += LocalFrame->Data[LocalFrame->DataLen] + 1;	
		
		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}	
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}
*/

ResultEnum PLC_AMR_ADD_OneRelayTask(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	UartTxReqStruct UartTxReq;
	RelayTaskParaStruct RelayTaskPara;
	u16* pPortAckTimer;
	u32 FrameLen = 0, AddrTxBuf = 0, CurTaskID = 0, Addr = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer[0] != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));		
		MC(0, (u32)&RelayTaskPara, sizeof(RelayTaskParaStruct));
		
		CurTaskID = AMRCtrl->RelayTask->CurTaskID;
		Addr = ADDR_RelayTaskPara + CurTaskID*sizeof(RelayTaskParaStruct);
		MR((u32)&RelayTaskPara, Addr, sizeof(RelayTaskParaStruct));

		for(Addr = 0; Addr < RelayTaskPara.Length; Addr++)
		{
			if(RelayTaskPara.Frame[Addr] == DLT_645_HEAD) break;
		}

		MR((u32)AMRCtrl->Addr, (u32)&RelayTaskPara.Frame[Addr + 1], METER_ADDR_SIZE);
		PLCManage->AMRTimeOut = PLC_AMR_MULTI_TIME_OUT;
		pPortAckTimer[0] = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT;
		MR((u32)LocalFrame->SrcAddr, (u32)PLCManage->TerminalAddr, METER_ADDR_SIZE);
		MR((u32)LocalFrame->DstAddr, (u32)AMRCtrl->Addr, METER_ADDR_SIZE);
		LocalFrame->AFN = LOCAL_FRAME_AFN_02;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020201;
		LocalFrame->DataLen = 0;
		LocalFrame->Data[LocalFrame->DataLen++] = AMRCtrl->PLCTaskIDAddNum;
		LocalFrame->Data[LocalFrame->DataLen++] = AMRCtrl->PLCTaskIDAddNum >> 8;
		LocalFrame->Data[LocalFrame->DataLen++] = PLC_TASK_LEVEL_3 | PLC_TASK_RESP_BIT;
		LocalFrame->Data[LocalFrame->DataLen++] = PLCManage->AMRTimeOut;
		LocalFrame->Data[LocalFrame->DataLen++] = PLCManage->AMRTimeOut >> 8;
		LocalFrame->Data[LocalFrame->DataLen++] = RelayTaskPara.Length;
		MR((u32)&LocalFrame->Data[LocalFrame->DataLen], (u32)RelayTaskPara.Frame, RelayTaskPara.Length);
		LocalFrame->DataLen += RelayTaskPara.Length;	
		
		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}	
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

ResultEnum PLC_AMR_WaitRxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	DLT645FrameStruct DLT645Frame;
	u16* pAckTimer;
	ResultEnum Result = NO_ERROR;
	u8* DataBuf = 0; 
	
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pAckTimer[0] == 0)
	{
		Result = PLC_AMR_RxTimeOutProcess(PORTn, AMRCtrl, PLCManage);
		return Result;
	}

	if(PLCManage->DataLenInDataBuf == 0) return ERROR_WRONG_LENGTH;

	DataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	if(DataBuf == NULL) return ERROR_POINTER_NULL;

	//err20190221 �������κ��Ƿ����ڳ���ʱ�л�����Ҫ�ܾ�
	
//pAckTimer[0] = PLCManage->AMRTimeOut;	//���ڳ�����ʱ100s
	Result = DLT645FrameDcompress(&DLT645Frame, DataBuf, PLCManage->DataLenInDataBuf);
	PLCManage->DataLenInDataBuf = 0;
	if(Result != NO_ERROR) return Result;
	
	switch(AMRCtrl->AMR_ID_Step)
	{
		case AMR_DAY_FREEZ_ID:
			Result = PLC_AMR_RxDayFreezProcess(PORTn, AMRCtrl, &DLT645Frame, PLCManage);
			break;
		case AMR_DAY_FREEZ_PATCH_ID:
			Result = PLC_AMR_RxDayFreezProcess(PORTn, AMRCtrl, &DLT645Frame, PLCManage);
			break;
		case AMR_HOUR_FREEZ_ID:
			Result = PLC_AMR_RxHourFreezProcess(PORTn, AMRCtrl, &DLT645Frame, PLCManage);
			break;
		case AMR_NORMAL_TASK_ID:
			Result = PLC_AMR_RxNormalTaskProcess(PORTn, AMRCtrl, &DLT645Frame, PLCManage);
			break;
		case AMR_RELAY_TASK_ID:
			Result = PLC_AMR_RxRelayTaskProcess(PORTn, AMRCtrl, &DLT645Frame, PLCManage);
			break;
		case AMR_ALARM_TASK_ID:
			Result = PLC_AMR_RxAlarmTaskProcess(PORTn, AMRCtrl, &DLT645Frame, PLCManage);
			break;
		case AMR_EVENT_TASK_ID:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
		default:
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
			break;
	}
	return Result;
}


ResultEnum PLC_AMR_RxTimeOutProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	RealTimeYYStruct YYTime;
	ResultEnum Result = NO_ERROR;
	u8* AMRDataBuf = NULL;
	u8 NW_DI_Num = 0;
	u16 Index = 0, OffSet = 0;
	u32 DIDataLenPos = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	u8* DayFreezFailMeterList = (u8*)ADDR_MeterDayFreezFailList;

	///////////////////////////ybwhz����///////////////////////////////
	if(YBWHZ_TEST)
	{
		u8 *p8;
		u8 count = 0;
		MC(0, ADDR_YBWHZ_DATABUFF, 64); 
		p8=(u8*)(ADDR_YBWHZ_DATABUFF);
		p8[count++] = 'R';
		p8[count++] = 'x';
		p8[count++] = 'T';
		p8[count++] = 'i';
		p8[count++] = 'm';
		p8[count++] = 'e';
		p8[count++] = 'O';
		p8[count++] = 'u';
		p8[count++] = 't';
		p8[count++] = ':';
		switch(AMRCtrl->AMR_ID_Step)
		{
			case AMR_DAY_FREEZ_ID:
				p8[count++] = 'D';
				p8[count++] = 'A';
				p8[count++] = 'Y';
				break;
			case AMR_DAY_FREEZ_PATCH_ID:
				p8[count++] = 'D';
				p8[count++] = 'A';
				p8[count++] = 'P';
				break;
			case AMR_HOUR_FREEZ_ID:
				p8[count++] = 'C';
				p8[count++] = 'U';
				p8[count++] = 'V';
				break;
			case AMR_ALARM_TASK_ID:
				p8[count++] = 'A';
				p8[count++] = 'M';
				p8[count++] = 'R';
				break;
			default: 
				p8[count++] = 'O';
				p8[count++] = 'T';
				p8[count++] = 'H';
				break;
		}
		p8[count++] = '#';

		QCSG_LogToRS232Port((u32)PORTn,ADDR_YBWHZ_DATABUFF,count);
	}
	////////////////////////////ybwhz����/////////////////////////////



	
	if(AMRCtrl->AMR_ID_Step == AMR_DAY_FREEZ_ID)
	{// ��¼�ն��᳭��ʧ�ܣ����浽����ʧ���б��ȴ��´����㲹��
		OffSet = LEN_ListForOneMeter * MeterFileInfo->AMRDayFreezFailMeters;
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[0];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[1];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[2];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[3];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[4];
		DayFreezFailMeterList[OffSet++] = AMRCtrl->Addr[5];
		AMRCtrl->DayFreezFailMeters++;
		MeterFileInfo->AMRDayFreezFailMeters++;

		if(AMRCtrl->LastDayFrozenTime.u8Day == 0x01)
		{// ��¼�ն��᳭��ʧ�ܣ����浽�¶��᳭��ʧ���б��ȴ��´����㲹��
			AMRCtrl->MonthFreezFailMeters++;
			MeterFileInfo->AMRMonthFreezFailMeters++;
		}
		NodeList[AMRCtrl->MeterIndex].FailCnt++;

		//err20190214 ���������ʧ��һ��ֻ�ϱ�һ��
		if(AMRCtrl->DayFreezOkMeters + AMRCtrl->DayFreezFailMeters == AMRCtrl->TotalMeters)
			GenerateMeterAMRFailAlarm_E200003D(AMRCtrl->MeterIndex);
		//GenerateMeterAMRFailAlarm_E200003D(AMRCtrl->MeterIndex);
	}
	
	AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	if((AMRCtrl->AMR_ID_Step == AMR_HOUR_FREEZ_ID) && (AMRDataBuf != NULL))
	{
		for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->HourFreezNwDINum; NW_DI_Num++)
		{
			if(PLCManage->AMR_Data[NW_DI_Num].DataLen == 0) continue;
			//Index = FindNwDIFrom_AFN0C_0D_Tbl(PLCManage->PLC_AMR_Data[NW_DI_Num].NW_DI);
			Index = PLCManage->AMR_Data[NW_DI_Num].TblIndex;
			if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
			
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 8;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 16;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 24;
			DIDataLenPos = AMRCtrl->AMRTotalDataLen;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0;
			AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0;
			MR((u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], (u32)PLCManage->AMR_Data[NW_DI_Num].Data, PLCManage->AMR_Data[NW_DI_Num].DataLen);
			AMRCtrl->AMRTotalDataLen += PLCManage->AMR_Data[NW_DI_Num].DataLen;

			if((PLCManage->AMR_Data[NW_DI_Num].NW_DI == 0x0201FF00) || (PLCManage->AMR_Data[NW_DI_Num].NW_DI == 0x0202FF00))
			{
				Index = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].u16Dlen;
				for(; PLCManage->AMR_Data[NW_DI_Num].DataLen < Index; PLCManage->AMR_Data[NW_DI_Num].DataLen++)
				{
					AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0xFF;
				}
			}
			AMRDataBuf[DIDataLenPos]     = PLCManage->AMR_Data[NW_DI_Num].DataLen;
			AMRDataBuf[DIDataLenPos + 1] = PLCManage->AMR_Data[NW_DI_Num].DataLen >> 8;
		}
		
		if(AMRCtrl->AMRTotalDataLen != 0)
		{
			RealTimeToYYTime(&AMRCtrl->LastHourFrozenTime, &YYTime);
			YYTime.Minute = 0;
			YYTime.Ack = 0;
			YYTime.Report = 0;
			NAND_WriteMeterAMRData(AMR_CurveFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
		}
		else
		{
			AMRCtrl->HourFreezFailMeters++;
		}
	}

	if((AMRCtrl->AMR_ID_Step == AMR_NORMAL_TASK_ID) && (AMRDataBuf != NULL))
	{
		StoreNormalTaskAMRData(PORTn, AMRCtrl);
	}

	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
		SwitchMeterClearAMRInfo(AMRCtrl);
		MC(0, (u32)PLCManage->AMR_Data, sizeof(NwDIAMRDataStruct) * SIZE_12);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);

		#if(TEST_ON == 1)
			MeterFileInfo->PLCMeterSyncFlag = METER_FILE_SYNC_TO_PLC_FLAG;
		#endif
	}
	else
	{

	}
	
	return Result;
}

ResultEnum PLC_AMR_RxDayFreezProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage)
{
	u16* pAckTimer; 
	RealTimeYYStruct YYTime;
	RealTimeStruct RealTime;
	AMRFileHeadStruct FileHead;
	ResultEnum Result = NO_ERROR;
	u8 DI_Size = 0, NW_DI_Num = 0;
	u16 Index = 0, OffSet = 0, TaskIDNum = 0, DataLen = 0;
	u32 Cur645_DI = 0;
	u8* AMRDataBuf = NULL;
	u8* PnData = (u8*)(ADDR_DATABUFF);
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	//MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
	u8* DayFreezFailMeterList = (u8*)ADDR_MeterDayFreezFailList;	

	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	if(GetProtocolTypeByCtrlCode(Frame->CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;
	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)//��վ��ȷӦ��
	{
		Cur645_DI = Get645FrameDataID(Frame, AMRCtrl->Protocol);
		for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->DayFreezNwDINum; NW_DI_Num++)
		{
			//Index = FindNwDIFrom_AFN0D_Tbl(PLCManage->AMR_Data[NW_DI_Num].NW_DI);
			Index = PLCManage->AMR_Data[NW_DI_Num].TblIndex;
			if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
			AMRCtrl->CurNW_DI_TblIndex = Index;
			AMRCtrl->Cur645_DI_Step = 0;
			while(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
			{
				if(Cur645_DI == AMRCtrl->Cur645_DI) break;
				AMRCtrl->Cur645_DI_Step++;
			}
			if(Cur645_DI == AMRCtrl->Cur645_DI) break;	
		}
		if(NW_DI_Num >= AMRCtrl->DayFreezNwDINum) return ERROR_InvalidData;//������ڱ�����

		DI_Size = DLT_645_07_ID_SIZE;
		if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			DI_Size = DLT_645_97_ID_SIZE;
		}
		if(Frame->DataLen > DI_Size)
		{
			DataLen = Frame->DataLen - DI_Size;
			Index = PLCManage->AMR_Data[NW_DI_Num].DataLen;
			MR((u32)&PLCManage->AMR_Data[NW_DI_Num].Data[Index], (u32)&Frame->Data[DI_Size], DataLen);
			PLCManage->AMR_Data[NW_DI_Num].DataLen += DataLen;
		}
	}
	else //if((Frame->CtrlCode & DLT_645_ERROR_BIT) != 0)
	{	
		for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->DayFreezNwDINum; NW_DI_Num++)
		{
			OffSet = 0;
			Index = PLCManage->AMR_Data[NW_DI_Num].TblIndex;
			if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
			for(TaskIDNum = 0; TaskIDNum < PLCManage->AMR_Data[NW_DI_Num].TaskIDNum; TaskIDNum++)
			{
				if(PLCManage->ReportTaskID == PLCManage->AMR_Data[NW_DI_Num].TaskIDList[TaskIDNum])
				{
					if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
					{
						DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[TaskIDNum].u8BlDataLen;
					}
					else
					{
						DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[TaskIDNum].u8BlDataLen;
					}
					break;
				}
				if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
				{
					OffSet += gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[TaskIDNum].u8BlDataLen;
				}
				else
				{
					OffSet += gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[TaskIDNum].u8BlDataLen;
				}
			}
			if(TaskIDNum < PLCManage->AMR_Data[NW_DI_Num].TaskIDNum) break;
		}
		if(NW_DI_Num >= AMRCtrl->DayFreezNwDINum) return ERROR_InvalidData;
		
		MC(0xFF, (u32)&PLCManage->AMR_Data[NW_DI_Num].Data[OffSet], DataLen);
		if((OffSet + DataLen) >= PLCManage->AMR_Data[NW_DI_Num].DataLen)
		{
			PLCManage->AMR_Data[NW_DI_Num].DataLen = OffSet + DataLen;
		}
	}

	AMRCtrl->PLCTaskIDAckNum++;
	///////////////////////////ybwhz����///////////////////////////////
	if(YBWHZ_TEST)
	{
		u8 *p8;
		u8 count = 0;
		MC(0, ADDR_YBWHZ_DATABUFF, 64);	
		p8=(u8*)(ADDR_YBWHZ_DATABUFF);
		p8[count++] = 'D';
		p8[count++] = 'A';
		p8[count++] = 'Y';
		p8[count++] = ':';
		
		p8[count++] = AMRCtrl->MeterIndex/10%10 +'0';
		p8[count++] = AMRCtrl->MeterIndex/1%10 +'0';
		p8[count++] = '*';
		p8[count++] = AMRCtrl->PLCTaskIDAckNum/10%10 +'0';
		p8[count++] = AMRCtrl->PLCTaskIDAckNum/1%10 +'0';
		p8[count++] = '/';
		p8[count++] = AMRCtrl->PLCTaskIDAddNum/10%10 +'0';
		p8[count++] = AMRCtrl->PLCTaskIDAddNum/1%10 +'0';
		p8[count++] = ' ';

		p8[count++] = PLCManage->ReportTaskID/10%10 +'0';
		p8[count++] = PLCManage->ReportTaskID/1%10 +'0';
		p8[count++] = '#';


		QCSG_LogToRS232Port((u32)PORTn,ADDR_YBWHZ_DATABUFF,count);
	}
////////////////////////////ybwhz����/////////////////////////////
	if(AMRCtrl->PLCTaskIDAckNum < AMRCtrl->PLCTaskIDAddNum) return ERROR_WRONG_LENGTH;
	if(AMRCtrl->AMR_ID_Step == AMR_DAY_FREEZ_ID)
	{
		AMRCtrl->DayFreezOkMeters++;
		AMRCtrl->SuccessMeters++;
		NodeList[AMRCtrl->MeterIndex].FailCnt = 0;
		AMR_NUM_Display(PORTn, AMRCtrl->TotalMeters, AMRCtrl->SuccessMeters, AMRCtrl->FailMeters);
		//err20190214 ���������ʧ��һ��ֻ�ϱ�һ��
		if(AMRCtrl->DayFreezOkMeters + AMRCtrl->DayFreezFailMeters == AMRCtrl->TotalMeters)
			GenerateMeterAMRFailAlarm_E200003D(AMRCtrl->MeterIndex);
	}
	
	AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	if(AMRDataBuf == NULL) return ERROR_POINTER_NULL;
	
	for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->DayFreezNwDINum; NW_DI_Num++)
	{
		if(PLCManage->AMR_Data[NW_DI_Num].DataLen == 0) continue;
		//Index = FindNwDIFrom_AFN0D_Tbl(PLCManage->PLC_AMR_Data[NW_DI_Num].NW_DI);
		Index = PLCManage->AMR_Data[NW_DI_Num].TblIndex;
		if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
		
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 8;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 16;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 24;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].DataLen;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].DataLen >> 8;
		MR((u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], (u32)PLCManage->AMR_Data[NW_DI_Num].Data, PLCManage->AMR_Data[NW_DI_Num].DataLen);
		AMRCtrl->AMRTotalDataLen += PLCManage->AMR_Data[NW_DI_Num].DataLen;
		
		if((AMRCtrl->PMType == METER_TYPE_MULTI) && (NW_DI_Num == (METER_DAY_M_DI_NUM - 1)) && (AMRCtrl->AMRTotalDataLen != 0))
		{
			HAL_GetSystemTime(&RealTime);
			RealTimeToYYTime(&RealTime, &YYTime);
			//RealTimeToYYTime(&AMRCtrl->LastDayFrozenTime, &YYTime);
			YYTime.Ack = 0;
			YYTime.Report = 0;
			NAND_WriteMeterAMRData(AMR_DailyFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
		}
	}
	
	if(AMRCtrl->AMRTotalDataLen != 0)
	{
		if(AMRCtrl->PMType == METER_TYPE_SINGLE)
		{
			HAL_GetSystemTime(&RealTime);
			RealTimeToYYTime(&RealTime, &YYTime);
			//RealTimeToYYTime(&AMRCtrl->LastDayFrozenTime, &YYTime);
			YYTime.Ack = 0;
			YYTime.Report = 0;
			NAND_WriteMeterAMRData(AMR_DailyFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);	
		}	
		if(YYTime.Day == 0x01)
		{
			NAND_WriteMeterAMRData(AMR_MonthFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
			if(AMRCtrl->MonthFreezOkMeters < AMRCtrl->TotalMeters) AMRCtrl->MonthFreezOkMeters++;
		}
		else
		{
			Result = NAND_ReadMeterAMRData(AMR_MonthFreeze, AMRCtrl->Addr, &FileHead, AMRCtrl->CurNW_DI, &YYTime, PnData, &Cur645_DI);
			if(Result != NO_ERROR)
			{
				NAND_WriteMeterAMRData(AMR_MonthFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
				if(AMRCtrl->MonthFreezOkMeters < AMRCtrl->TotalMeters) AMRCtrl->MonthFreezOkMeters++;
			}
		}
	}

	if(AMRCtrl->AMR_ID_Step == AMR_DAY_FREEZ_PATCH_ID)
	{
		if(AMRCtrl->DayFreezFailMeters > 0) AMRCtrl->DayFreezFailMeters--;
		//if(MeterFileInfo->AMRDayFreezFailMeters > 0) MeterFileInfo->AMRDayFreezFailMeters--;	
		Index = FindMeterInNodeList(AMRCtrl->Addr, NodeList, MAX_PN_NUM);
		NodeList[Index].FailCnt = 0;
		MC(0, (u32)&DayFreezFailMeterList[AMRCtrl->MeterIndex * METER_ADDR_SIZE], METER_ADDR_SIZE);
	}
	
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	pAckTimer[0] = 0;
	MC(0, (u32)PLCManage->AMR_Data, sizeof(NwDIAMRDataStruct) * SIZE_12);
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
		SwitchMeterClearAMRInfo(AMRCtrl);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}

ResultEnum PLC_AMR_RxHourFreezProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage)
{
	RealTimeYYStruct YYTime;
	u16* pAckTimer; 
	
	ResultEnum Result = NO_ERROR;
	u8 DI_Size = 0, NW_DI_Num = 0;
	u16 Index = 0, DataLen = 0, OffSet = 0, TaskIDNum = 0;
	u32 Cur645_DI = 0, DIDataLenPos = 0;
	u8* AMRDataBuf = NULL;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	
	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;	
	if(GetProtocolTypeByCtrlCode(Frame->CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;
	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)	//����Ӧ��
	{
		Cur645_DI = Get645FrameDataID(Frame, AMRCtrl->Protocol);
		for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->HourFreezNwDINum; NW_DI_Num++)
		{
			//Index = FindNwDIFrom_AFN0D_Tbl(PLCManage->PLC_AMR_Data[NW_DI_Num].NW_DI);
			Index = PLCManage->AMR_Data[NW_DI_Num].TblIndex;
			if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
			AMRCtrl->CurNW_DI_TblIndex = Index;
			AMRCtrl->Cur645_DI_Step = 0;
			OffSet = 0;
			while(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
			{
				if(Cur645_DI == AMRCtrl->Cur645_DI) break;
				
				if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
				{
					OffSet += gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[AMRCtrl->Cur645_DI_Step].u8BlDataLen;
				}
				else
				{
					OffSet += gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[AMRCtrl->Cur645_DI_Step].u8BlDataLen;
				}
				AMRCtrl->Cur645_DI_Step++;
			}
			if(Cur645_DI == AMRCtrl->Cur645_DI) break;	
		}
		if(NW_DI_Num >= AMRCtrl->HourFreezNwDINum) return ERROR_InvalidData;

		AMRCtrl->CurNW_DI = PLCManage->AMR_Data[NW_DI_Num].NW_DI;
		DI_Size = DLT_645_07_ID_SIZE;
		if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97) DI_Size = DLT_645_97_ID_SIZE;
		if(Frame->DataLen > DI_Size)
		{
			if(AMRCtrl->CurNW_DI == 0x0202FF00 || AMRCtrl->CurNW_DI == 0x0201FF00)	//�������ʾֵ��������Ҫת��
			{
				AMRCtrl->Cur645DataLen = 0;
				Convert645ToNWAMRData(Frame, AMRCtrl->CurNW_DI_TblIndex, AMRCtrl->Cur645_DI_Step, AMRCtrl->Protocol, AMRCtrl->Cur645Data, &AMRCtrl->Cur645DataLen);
				if(AMRCtrl->Cur645DataLen != 0)
				{
					MR((u32)&PLCManage->AMR_Data[NW_DI_Num].Data[OffSet], (u32)AMRCtrl->Cur645Data, AMRCtrl->Cur645DataLen);
					if((OffSet + AMRCtrl->Cur645DataLen) >= PLCManage->AMR_Data[NW_DI_Num].DataLen)
					{
						PLCManage->AMR_Data[NW_DI_Num].DataLen = OffSet + AMRCtrl->Cur645DataLen;
					}
				}
			}
			else
			{
				DataLen = Frame->DataLen - DI_Size;
				MR((u32)&PLCManage->AMR_Data[NW_DI_Num].Data[OffSet], (u32)&Frame->Data[DI_Size], DataLen);
				if((OffSet + DataLen) >= PLCManage->AMR_Data[NW_DI_Num].DataLen)
				{
					PLCManage->AMR_Data[NW_DI_Num].DataLen = OffSet + DataLen;
				}
			}
		}
	}	
	else		//�쳣Ӧ��
	{
		for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->HourFreezNwDINum; NW_DI_Num++)
		{
			OffSet = 0;
			Index = PLCManage->AMR_Data[NW_DI_Num].TblIndex;
			if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
			for(TaskIDNum = 0; TaskIDNum < PLCManage->AMR_Data[NW_DI_Num].TaskIDNum; TaskIDNum++)
			{
				if(PLCManage->ReportTaskID == PLCManage->AMR_Data[NW_DI_Num].TaskIDList[TaskIDNum])
				{
					if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
					{
						DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[TaskIDNum].u8BlDataLen;
					}
					else
					{
						DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[TaskIDNum].u8BlDataLen;
					}
					break;
				}
				if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
				{
					OffSet += gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[TaskIDNum].u8BlDataLen;
				}
				else
				{
					OffSet += gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[TaskIDNum].u8BlDataLen;
				}
			}
			if(TaskIDNum < PLCManage->AMR_Data[NW_DI_Num].TaskIDNum) break;
		}
		if(NW_DI_Num >= AMRCtrl->HourFreezNwDINum) return ERROR_InvalidData;
		
		MC(0xFF, (u32)&PLCManage->AMR_Data[NW_DI_Num].Data[OffSet], DataLen);
		if((OffSet + DataLen) >= PLCManage->AMR_Data[NW_DI_Num].DataLen)
		{
			PLCManage->AMR_Data[NW_DI_Num].DataLen = OffSet + DataLen;
		}
	}

	AMRCtrl->PLCTaskIDAckNum++;
	///////////////////////////ybwhz����///////////////////////////////
		if(YBWHZ_TEST)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64); 
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'C';
			p8[count++] = 'U';
			p8[count++] = 'V';
			p8[count++] = ':';
			
			p8[count++] = AMRCtrl->MeterIndex/10%10 +'0';
			p8[count++] = AMRCtrl->MeterIndex/1%10 +'0';
			p8[count++] = '*';
			p8[count++] = AMRCtrl->PLCTaskIDAckNum/10%10 +'0';
			p8[count++] = AMRCtrl->PLCTaskIDAckNum/1%10 +'0';
			p8[count++] = '/';
			p8[count++] = AMRCtrl->PLCTaskIDAddNum/10%10 +'0';
			p8[count++] = AMRCtrl->PLCTaskIDAddNum/1%10 +'0';
			p8[count++] = ' ';
	
			p8[count++] = PLCManage->ReportTaskID/10%10 +'0';
			p8[count++] = PLCManage->ReportTaskID/1%10 +'0';
			p8[count++] = '#';
	
	
			QCSG_LogToRS232Port((u32)PORTn,ADDR_YBWHZ_DATABUFF,count);
		}
	////////////////////////////ybwhz����/////////////////////////////

	
	//��һ����������������ٱ���
	if(AMRCtrl->PLCTaskIDAckNum < AMRCtrl->PLCTaskIDAddNum) return ERROR_WRONG_LENGTH;


	AMRCtrl->HourFreezOkMeters++;
	AMRCtrl->SuccessMeters++;
	NodeList[AMRCtrl->MeterIndex].FailCnt = 0;
	AMR_NUM_Display(PORTn, AMRCtrl->TotalMeters, AMRCtrl->SuccessMeters, AMRCtrl->FailMeters);
	AMRDataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	if(AMRDataBuf == NULL) return ERROR_POINTER_NULL;
	
	for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->HourFreezNwDINum; NW_DI_Num++)
	{
		if(PLCManage->AMR_Data[NW_DI_Num].DataLen == 0) continue;
		//Index = FindNwDIFrom_AFN0C_0D_Tbl(PLCManage->PLC_AMR_Data[NW_DI_Num].NW_DI);
		Index = PLCManage->AMR_Data[NW_DI_Num].TblIndex;
		if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
		
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 8;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 16;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = PLCManage->AMR_Data[NW_DI_Num].NW_DI >> 24;
		DIDataLenPos = AMRCtrl->AMRTotalDataLen;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0;
		AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0;
		MR((u32)&AMRDataBuf[AMRCtrl->AMRTotalDataLen], (u32)PLCManage->AMR_Data[NW_DI_Num].Data, PLCManage->AMR_Data[NW_DI_Num].DataLen);
		AMRCtrl->AMRTotalDataLen += PLCManage->AMR_Data[NW_DI_Num].DataLen;

		if((PLCManage->AMR_Data[NW_DI_Num].NW_DI == 0x0201FF00) || (PLCManage->AMR_Data[NW_DI_Num].NW_DI == 0x0202FF00))
		{
			Index = gc_stQCSGAFN0C_0D_DLT645_Table[AMRCtrl->CurNW_DI_TblIndex].u16Dlen;
			for(; PLCManage->AMR_Data[NW_DI_Num].DataLen < Index; PLCManage->AMR_Data[NW_DI_Num].DataLen++)
			{
				AMRDataBuf[AMRCtrl->AMRTotalDataLen++] = 0xFF;
			}
		}
		AMRDataBuf[DIDataLenPos]     = PLCManage->AMR_Data[NW_DI_Num].DataLen;
		AMRDataBuf[DIDataLenPos + 1] = PLCManage->AMR_Data[NW_DI_Num].DataLen >> 8;
	}
	
	if(AMRCtrl->AMRTotalDataLen != 0)
	{
		RealTimeToYYTime(&AMRCtrl->LastHourFrozenTime, &YYTime);
		YYTime.Minute = 0x00;
		YYTime.Ack = 0;
		YYTime.Report = 0;
		NAND_WriteMeterCurveData(AMR_CurveFreeze, AMRCtrl->Addr, AMRCtrl->PMType, AMRCtrl->CurNW_DI, &YYTime, (u8*)AMRDataBuf, &AMRCtrl->AMRTotalDataLen);
	}

	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	pAckTimer[0] = 0;
	MC(0, (u32)PLCManage->AMR_Data, sizeof(NwDIAMRDataStruct) * SIZE_12);	
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
		SwitchMeterClearAMRInfo(AMRCtrl);	
	}
	else
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	
	return Result;
}


ResultEnum PLC_AMR_RxNormalTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage)
{
	u16* pAckTimer;
	ResultEnum Result = NO_ERROR;
	u16 Index = 0, NW_DI_Num = 0, OffSet = 0;
	u32 Cur645_DI = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	if(GetProtocolTypeByCtrlCode(Frame->CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;

	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)//��վ��ȷӦ��
	{
		Cur645_DI = Get645FrameDataID(Frame, AMRCtrl->Protocol);
		for(NW_DI_Num = 0; NW_DI_Num < AMRCtrl->NormalTask->DINum; NW_DI_Num++)
		{
			//Index = FindNwDIFrom_AFN0C_0D_Tbl(AMRCtrl->NormalTask->DataList[NW_DI_Num].NW_DI);
			Index = AMRCtrl->NormalTask->DataList[NW_DI_Num].TblIndex;
			if(Index >= MAX_AFN0C_0D_IDENT_NUM) continue;
			AMRCtrl->CurNW_DI_TblIndex = Index;
			AMRCtrl->Cur645_DI_Step = 0;
			OffSet = 0;
			while(Get_NextAMR_645_DI(AMRCtrl) == NO_ERROR)
			{
				if(Cur645_DI == AMRCtrl->Cur645_DI) break;
				
				if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
				{
					OffSet += gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[AMRCtrl->Cur645_DI_Step].u8BlDataLen;
				}
				else
				{
					OffSet += gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[AMRCtrl->Cur645_DI_Step].u8BlDataLen;
				}
				AMRCtrl->Cur645_DI_Step++;
			}
			if(Cur645_DI == AMRCtrl->Cur645_DI) break;	
		}
		if(NW_DI_Num >= AMRCtrl->NormalTask->DINum) return ERROR_InvalidData;

		AMRCtrl->Cur645DataLen = 0;
		Convert645ToNWAMRData(Frame, AMRCtrl->CurNW_DI_TblIndex, AMRCtrl->Cur645_DI_Step, AMRCtrl->Protocol, AMRCtrl->Cur645Data, &AMRCtrl->Cur645DataLen);

		if(AMRCtrl->Cur645DataLen != 0)
		{
			MR((u32)&AMRCtrl->NormalTask->DataList[NW_DI_Num].Data[OffSet], (u32)AMRCtrl->Cur645Data, AMRCtrl->Cur645DataLen);
			if((OffSet + AMRCtrl->Cur645DataLen) >= AMRCtrl->NormalTask->DataList[NW_DI_Num].DataLen)
			{
				AMRCtrl->NormalTask->DataList[NW_DI_Num].DataLen = OffSet + AMRCtrl->Cur645DataLen;
			}
		}
	}

	AMRCtrl->PLCTaskIDAckNum++;
	///////////////////////////ybwhz����///////////////////////////////
		if(YBWHZ_TEST)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64); 
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'N';
			p8[count++] = 'R';
			p8[count++] = 'M';
			p8[count++] = ':';
			
			p8[count++] = AMRCtrl->MeterIndex/10%10 +'0';
			p8[count++] = AMRCtrl->MeterIndex/1%10 +'0';
			p8[count++] = '*';
			p8[count++] = AMRCtrl->PLCTaskIDAckNum/10%10 +'0';
			p8[count++] = AMRCtrl->PLCTaskIDAckNum/1%10 +'0';
			p8[count++] = '/';
			p8[count++] = AMRCtrl->PLCTaskIDAddNum/10%10 +'0';
			p8[count++] = AMRCtrl->PLCTaskIDAddNum/1%10 +'0';
			p8[count++] = ' ';
	
			p8[count++] = PLCManage->ReportTaskID/10%10 +'0';
			p8[count++] = PLCManage->ReportTaskID/1%10 +'0';
			p8[count++] = '#';
	
	
			QCSG_LogToRS232Port((u32)PORTn,ADDR_YBWHZ_DATABUFF,count);
		}
	////////////////////////////ybwhz����/////////////////////////////

	
	if(AMRCtrl->PLCTaskIDAckNum < AMRCtrl->PLCTaskIDAddNum) return ERROR_WRONG_LENGTH;
	
	StoreNormalTaskAMRData(PORTn, AMRCtrl);

	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	pAckTimer[0] = 0;
	
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
		SwitchMeterClearAMRInfo(AMRCtrl);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}


ResultEnum PLC_AMR_RxRelayTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage)
{
	u16* pAckTimer;
	RealTimeYYStruct YYTime;
	RealTimeStruct RealTime;
	ResultEnum Result = NO_ERROR;
	u32 RelayTaskID = 0, DataLen = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	DataLen = Build645Frame(Frame, AMRCtrl->Cur645Data, NO_FE_IN_645);

	if(DataLen != 0)
	{
		HAL_GetSystemTime(&RealTime);
		RealTimeToYYTime(&RealTime, &YYTime);
		YYTime.Ack = 0;
		YYTime.Report = 0;
		RelayTaskID = 0xE0000401 + AMRCtrl->RelayTask->CurTaskID;
		NAND_WriteRelayTask(AMRCtrl->RelayTask->RelayType,RelayTaskID, &YYTime, AMRCtrl->Cur645Data, &DataLen);
	}

	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	pAckTimer[0] = 0;
	
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
		SwitchMeterClearAMRInfo(AMRCtrl);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
	
	return Result;
}

ResultEnum PLC_AMR_RxAlarmTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, DLT645FrameStruct* Frame, PLCManageStruct* PLCManage)
{
	u16* pAckTimer;
	ResultEnum Result = NO_ERROR;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	AlarmAMRDataStruct* AlarmAMRData = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	AlarmAMRData += AMRCtrl->MeterIndex;


//if(PLCManage->ReportTaskID < RT_AMR_TASK_ID_START) 
{
	if(MemCompare(Frame->Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	
	if(GetProtocolTypeByCtrlCode(Frame->CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;

	if((Frame->CtrlCode & DLT_645_ERROR_BIT) == 0)
	{
		if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			StoreAlarmTaskAMRData_97(PORTn, AMRCtrl, Frame);
		}
		else
		{
			StoreAlarmTaskAMRData_07(PORTn, AMRCtrl, Frame);
		}
	}

	/*{
	//err20190220 �޸��ز���������
		u8 Pos=0,Bit=0;
		Pos = AMRCtrl->PLCTaskIDAddNum >> 3;
		Bit = AMRCtrl->PLCTaskIDAddNum & 0x07;	
		AMRCtrl->PLCTaskIDFlagList[Pos] |= 1 << Bit;
	}*/
	
	AMRCtrl->PLCTaskIDAckNum++;
	
	///////////////////////////ybwhz����///////////////////////////////
	if(YBWHZ_TEST)
	{
		u8 *p8;
		u8 count = 0;
		MC(0, ADDR_YBWHZ_DATABUFF, 64);	
		p8=(u8*)(ADDR_YBWHZ_DATABUFF);
		p8[count++] = 'A';
		p8[count++] = 'M';
		p8[count++] = 'R';
		p8[count++] = ':';
		
		p8[count++] = AMRCtrl->MeterIndex/10%10 +'0';
		p8[count++] = AMRCtrl->MeterIndex/1%10 +'0';
		p8[count++] = '*';
		p8[count++] = AMRCtrl->PLCTaskIDAckNum/10%10 +'0';
		p8[count++] = AMRCtrl->PLCTaskIDAckNum/1%10 +'0';
		p8[count++] = '/';
		p8[count++] = AMRCtrl->PLCTaskIDAddNum/10%10 +'0';
		p8[count++] = AMRCtrl->PLCTaskIDAddNum/1%10 +'0';
		p8[count++] = ' ';

		p8[count++] = PLCManage->ReportTaskID/10%10 +'0';
		p8[count++] = PLCManage->ReportTaskID/1%10 +'0';
		p8[count++] = '#';


		QCSG_LogToRS232Port((u32)PORTn,ADDR_YBWHZ_DATABUFF,count);
	}
////////////////////////////ybwhz����/////////////////////////////

	if(AMRCtrl->PLCTaskIDAckNum < AMRCtrl->PLCTaskIDAddNum) return ERROR_WRONG_LENGTH;
		//���ڳ�������δ�ϱ���������ȴ�
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	pAckTimer[0] = 0;
	
	AlarmAMRData->State[1] = 0x01;	//�����Ѹ���
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
		SwitchMeterClearAMRInfo(AMRCtrl);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{

	}
}
	return Result;
}


ResultEnum PLC_AMR_PauseProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	u16* pAckTimer;
	ResultEnum Result = NO_ERROR;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pAckTimer[0] == 0)
	{
		AMRCtrl->State = AMR_Resume;
		return Result;
	}

	if((AMRCtrl->Flag & AMR_FLAG_RT_AMR_START) == 0) return Result;
	AMRCtrl->Flag &= ~AMR_FLAG_RT_AMR_START;

	PLCManage->Flag = 0;
	if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
	{
		AMRCtrl->State = AMR_RealTimeTx;
		pAckTimer[0] = 0;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_STOP_TASK;
		//if(Task->RequestCnt == 0x01) pAckTimer[0] = RT_AMR_PLC_FIRST_TIME_DELAY;
		//pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
	}
	else if(Task->SrcDataType == DATA_TYPE_FRAME)
	{
		AMRCtrl->State = AMR_RealTimeTx;
		pAckTimer[0] = 0;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_STOP_TASK;
		//if(Task->RequestCnt == 0x01) pAckTimer[0] = RT_AMR_PLC_FIRST_TIME_DELAY;
		//pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
	}
	else if(Task->SrcDataType == DATA_TYPE_RELAY)
	{
		AMRCtrl->State = AMR_RealTimeTx;
		pAckTimer[0] = 0;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_STOP_TASK;
		//if(Task->RequestCnt == 0x01) pAckTimer[0] = RT_AMR_PLC_FIRST_TIME_DELAY;
		//pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
	}
	else //if(Task->SrcDataType == DATA_TYPE_STRUCT)
	{
		if(Task->Protocol == DLT_645_PROTOCOL_07)
		{
			Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].stDLT64507Resolve[Task->Cur645_DI_Step].u32Ident;
		}
		else //if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
		{
			Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].stDLT64597Resolve[Task->Cur645_DI_Step].u32Ident;
		}
		if(Task->Cur645_DI == 0xFFFFFFFF)
		{	
			Task->Flag |= RT_AMR_DATA_READY;
			AMRCtrl->State = AMR_Pause;
			pAckTimer[0] = AMR_PAUSE_MAX_TIME;
		}
		else
		{
			AMRCtrl->State = AMR_RealTimeTx;
			pAckTimer[0] = 0;
			pTerminalRouter->RouterInitTask = PLC_ADD_TASK_STOP_TASK;
			//if(Task->RequestCnt == 0x01) pAckTimer[0] = RT_AMR_PLC_FIRST_TIME_DELAY;
			//pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
			// �˴����� pTerminalRouter->RouterInitTask ֱ���� 0������Ϊ�л�������ַǰ���ܸո���ģ�鷢����
			// ���һ�����������ϱ���ȷ����Ϣ����Ҫ�ȴ�ȷ����Ϣ������ϣ��������д˲����������Ϊ����
			// ����ĵ�һ�����贮�ڱ�ռ�ã������ó�ʱʱ��Ϊ 3 S��ʱ��Ƚϳ����˷�ʱ�䡣
		}
	}
	
	return Result;
}

ResultEnum PLC_AMR_RealTimeTxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	u16* pPortAckTimer;
	QCSGLocalFrameStruct LocalFrame;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	ResultEnum Result = NO_ERROR;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	switch(pTerminalRouter->RouterInitTask)
	{
		case PLC_ADD_TASK_STOP_TASK:
			Result = PLC_AMR_ADD_TASK_TxStopTask(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_ADD_TASK_STOP_TASK_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_STOP_TASK_DONE) != 0)
			{// PLC  ����������ֹͣ����
				PLCManage->Flag &= ~PLC_FLAG_STOP_TASK_DONE;
				pPortAckTimer[0] = 0;
				//err20190221 �ı����ڳ���ʱ�����ʵʱ���̣����ٽ��г�ʼ��
				if((AMRCtrl->OldState == AMR_WaitRx)&& (AMRCtrl->AMR_ID_Step == AMR_ALARM_TASK_ID))
				{
					Task->PLCTaskIDAckNum = RT_AMR_TASK_ID_START;
					Task->PLCTaskIDAddNum = RT_AMR_TASK_ID_START;
					pTerminalRouter->RouterInitTask = PLC_ADD_TASK_ADD_ONE;
					//pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE;//��ʱ��ͣ�۲�����ģ��״̬
				}
				else
					pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط�ֹͣ����
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_ADD_TASK_INIT:
			Result = PLC_TxInitTask(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_ADD_TASK_INIT_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_INIT_TASK_DONE) != 0)
			{// PLC ������ʼ���������
				PLCManage->Flag &= ~PLC_FLAG_INIT_TASK_DONE;
				pPortAckTimer[0] = 3;
				if(AMRCtrl->OldState == AMR_Idle) pPortAckTimer[0] = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
				Task->PLCTaskIDAckNum = RT_AMR_TASK_ID_START;
				Task->PLCTaskIDAddNum = RT_AMR_TASK_ID_START;
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_ADD_TASK_ADD_ONE:
			Result = PLC_AMR_RealTimeAddTaskProcess(PORTn, AMRCtrl, PLCManage, &LocalFrame);
			break;
		case PLC_ADD_TASK_ADD_ONE_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_ADD_ONE_TASK_DONE) != 0)
			{// PLC ��������������ɣ�
				PLCManage->Flag &= ~PLC_FLAG_ADD_ONE_TASK_DONE;
				pPortAckTimer[0] = 0;
				Task->PLCTaskIDAddNum++;
				if(Task->SrcDataType == DATA_TYPE_STRUCT)
				{
					Task->Cur645_DI_Step++;
					if(Task->Protocol == DLT_645_PROTOCOL_07)
					{
						Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].stDLT64507Resolve[Task->Cur645_DI_Step].u32Ident;
					}
					else //if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
					{
						Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].stDLT64597Resolve[Task->Cur645_DI_Step].u32Ident;
					}
					if(Task->Cur645_DI == 0xFFFFFFFF)
					{
						Result = ERROR_NO_AMR_TASK;
					}
					else
					{
						Result = NO_ERROR;
					}
				}
				else
				{
					Result = ERROR_NO_AMR_TASK;
				}
				if(Result == NO_ERROR)
				{
					pTerminalRouter->RouterInitTask--;
					pTerminalRouter->RetryCnt = 0;					
				}
				else
				{
					pTerminalRouter->RouterInitTask++;
					pTerminalRouter->RetryCnt = 0;
				}
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط���������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_ADD_TASK_START_TASK:
			Result = PLC_AMR_ADD_TASK_TxStartTask(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_ADD_TASK_START_TASK_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_START_TASK_DONE) != 0)
			{// PLC ��������������ɣ�
				PLCManage->Flag &= ~PLC_FLAG_START_TASK_DONE;
				PLCManage->ReportTaskID = 0;
				pPortAckTimer[0] = 0;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
			}
			else
			{
				if(pPortAckTimer[0] == 0)
				{// ��ʱ���ط���ʼ������
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		case PLC_ADD_TASK_DONE:
			if(pPortAckTimer[0] == 0)
			{
				AMRCtrl->State = AMR_RealTimeWaitRx;
				pTerminalRouter->RouterInitTask++;
				pTerminalRouter->RetryCnt = 0;
				pPortAckTimer[0] = PLCManage->AMRTimeOut;
				if(Task->SrcDataType == DATA_TYPE_RELAY)
				{
					RT_AMR_Info_Display(PORTn, Task->Addr, RT_AMR_INFO_CODE_RELAY);
				}
				if(Task->SrcDataType == DATA_TYPE_STRUCT)
				{
					RT_AMR_Info_Display(PORTn, Task->Addr, RT_AMR_INFO_CODE_RT);
				}

				if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
				{
					Task->Flag |= RT_AMR_DATA_READY;//err20190315 �ز��㲥����ִ����ɾ�Ӧ�������ֹ�ȴ����á�
					RT_AMR_Info_Display(PORTn, Task->Addr, RT_AMR_INFO_CODE_BCAST);
				}	

				if(Task->SrcDataType == DATA_TYPE_FRAME)
				{
					RT_AMR_Info_Display(PORTn, Task->Addr, RT_AMR_INFO_CODE_CONTROL);
				}
			}
			break;
		default:
			if(pPortAckTimer[0] == 0) pTerminalRouter->RouterInitTask++;
			// �˴����� pTerminalRouter->RouterInitTask ֱ���� 0������Ϊ�л�������ַǰ�ո���ģ�鷢����
			// ���һ�����������ϱ���ȷ����Ϣ����Ҫ�ȴ�ȷ����Ϣ������ϣ��������д˲����������Ϊ����
			// ����ĵ�һ�����贮�ڱ�ռ�ã������ó�ʱʱ��Ϊ 3 S��ʱ��Ƚϳ����˷�ʱ�䡣
			break;
	}
	
	return Result;
}
ResultEnum PLC_AMR_RealTimeAddTaskProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u16* pPortAckTimer;
	UartTxReqStruct UartTxReq;
	DLT645FrameStruct DLT645Frame;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	u32 TxBuf = 0, TxLen = 0;

	pPortAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pPortAckTimer[0] != 0) return ERROR_WRONG_TIME;
	
	MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));
	TxBuf = GetUartPortTxBuf(PORTn);
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{	
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pPortAckTimer[0] = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT;
		MR((u32)LocalFrame->SrcAddr, (u32)PLCManage->TerminalAddr, METER_ADDR_SIZE);
		MR((u32)LocalFrame->DstAddr, (u32)Task->Addr, METER_ADDR_SIZE);
		LocalFrame->AFN = LOCAL_FRAME_AFN_02;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8020201;
		LocalFrame->DataLen = 0;
		LocalFrame->Data[LocalFrame->DataLen++] = Task->PLCTaskIDAddNum;
		LocalFrame->Data[LocalFrame->DataLen++] = Task->PLCTaskIDAddNum >> 8;
		if((Task->SrcDataType == DATA_TYPE_RELAY) || (Task->SrcDataType == DATA_TYPE_STRUCT) || (Task->SrcDataType == DATA_TYPE_FRAME))//err20180623 控制命令也需要回�?
		{
			LocalFrame->Data[LocalFrame->DataLen++] = PLC_TASK_LEVEL_0 | PLC_TASK_RESP_BIT;
		}
		else
		{
			LocalFrame->Data[LocalFrame->DataLen++] = PLC_TASK_LEVEL_0;//err20180623 广播校时不需要回�?
		}
		
		PLCManage->AMRTimeOut = PLC_AMR_RT_TIME_OUT;
		
		if(Task->SrcDataType == DATA_TYPE_RELAY)
		{//err20190311 �м�ת�����õĳ�ʱʱ�������С��ʱʱ��ʱ�������õ�ʱ����㳬ʱ��
			if(Task->RelayTimeOut > PLC_AMR_RT_TIME_OUT)
				PLCManage->AMRTimeOut = Task->RelayTimeOut;
		}
		
		LocalFrame->Data[LocalFrame->DataLen++] = PLCManage->AMRTimeOut;
		LocalFrame->Data[LocalFrame->DataLen++] = PLCManage->AMRTimeOut >> 8;
		
		MC(0, (u32)&DLT645Frame, sizeof(DLT645FrameStruct));		
		if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
		{
			LocalFrame->Data[LocalFrame->DataLen++] = Task->CurPnDataLen;
			MR((u32)&LocalFrame->Data[LocalFrame->DataLen], (u32)&Task->SrcData[Task->SrcDataPos], Task->CurPnDataLen);
			LocalFrame->DataLen += (u8)Task->CurPnDataLen;
			UartTxReq.NeedAck = 0x00;
			PLCManage->AMRTimeOut = 30;
		}
		else if(Task->SrcDataType == DATA_TYPE_FRAME)
		{
			LocalFrame->Data[LocalFrame->DataLen++] = Task->CurPnDataLen;
			MR((u32)&LocalFrame->Data[LocalFrame->DataLen], (u32)&Task->SrcData[Task->SrcDataPos], Task->CurPnDataLen);	
			LocalFrame->DataLen += (u8)Task->CurPnDataLen;
			UartTxReq.NeedAck = 0x00;
			PLCManage->AMRTimeOut = 30;
		}
		else if(Task->SrcDataType == DATA_TYPE_RELAY)
		{
			LocalFrame->Data[LocalFrame->DataLen++] = Task->CurPnDataLen;
			MR((u32)&LocalFrame->Data[LocalFrame->DataLen], (u32)&Task->SrcData[Task->SrcDataPos], Task->CurPnDataLen);	
			LocalFrame->DataLen += (u8)Task->CurPnDataLen;
			UartTxReq.NeedAck = 0x01;
		}
		else //if(Task->SrcDataType == DATA_TYPE_STRUCT)
		{
			MC(0, (u32)&DLT645Frame, sizeof(DLT645FrameStruct));
			MR((u32)DLT645Frame.Addr, (u32)Task->Addr, METER_ADDR_SIZE);
			if(Task->Protocol == DLT_645_PROTOCOL_07)
			{
				DLT645Frame.CtrlCode = DLT_07_READ;
				DLT645Frame.DataLen = DLT_645_07_ID_SIZE;
				DLT645Frame.Data[0] = Task->Cur645_DI;
				DLT645Frame.Data[1] = Task->Cur645_DI >> 8;
				DLT645Frame.Data[2] = Task->Cur645_DI >> 16;
				DLT645Frame.Data[3] = Task->Cur645_DI >> 24;
			}
			else //if(Task->Protocol == DLT_645_PROTOCOL_97)
			{
				DLT645Frame.CtrlCode = DLT_97_READ;
				DLT645Frame.DataLen = DLT_645_97_ID_SIZE;
				DLT645Frame.Data[0] = Task->Cur645_DI;
				DLT645Frame.Data[1] = Task->Cur645_DI >> 8;		
			}
			LocalFrame->Data[LocalFrame->DataLen] = Build645Frame(&DLT645Frame, &LocalFrame->Data[LocalFrame->DataLen + 1], NO_FE_IN_645);
			LocalFrame->DataLen += LocalFrame->Data[LocalFrame->DataLen] + 1;
			UartTxReq.NeedAck = 0x01;
		}	
		
		TxLen = BuildLocalModuleFrame((u8*)TxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = TxLen;
		UartTxReq.DataBuf = (u8*)TxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}	
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}
	return NO_ERROR;
}

ResultEnum PLC_AMR_WaitRealTimeRxProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	DLT645FrameStruct DLT645Frame;
	u16* pAckTimer;
	
	ResultEnum Result = NO_ERROR;
	RT_AMRCtrlStruct* Task = (RT_AMRCtrlStruct*)ADDR_RealTimeAMRTaskCtrl;
	u32 DataLen = 0, i = 0, LeftLen = 0, Index= 0, Cur645_DI = 0;
	u16 OffSet = 0, TempOffSet = 0;
	u8* DataBuf = NULL; 

	AlarmAMRDataStruct* AlarmAMRData = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;


	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pAckTimer[0] == 0)
	{
		AMRCtrl->State = AMR_Pause;
		pAckTimer[0] = AMR_PAUSE_MAX_TIME;
		Task->Flag |= RT_AMR_DATA_READY;
		return Result;
	}

	if(PLCManage->DataLenInDataBuf == 0) return ERROR_WRONG_LENGTH;	
	if(PLCManage->ReportTaskID < RT_AMR_TASK_ID_START) 
	{
		//err20190308 �˴���Ϊ������΢��������ģ�飬�·�ʵʱ�������������ϱ�һ֡���ڳ����������ݡ���Ҫ���մ������򽫶�ʧ��
		if((AMRCtrl->OldState == AMR_WaitRx)&& (AMRCtrl->AMR_ID_Step == AMR_ALARM_TASK_ID)&& (AMRCtrl->PLCTaskIDAckNum < AMRCtrl->PLCTaskIDAddNum))
		{//���ڳ�����û�г��� ��������ڳ�������
			DataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
			if(DataBuf == NULL) return ERROR_POINTER_NULL;

			Result = DLT645FrameDcompress(&DLT645Frame, DataBuf, PLCManage->DataLenInDataBuf);
			PLCManage->DataLenInDataBuf = 0;
			if(Result != NO_ERROR) return Result;

			if(MemCompare(DLT645Frame.Addr, AMRCtrl->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
	
			if(GetProtocolTypeByCtrlCode(DLT645Frame.CtrlCode & DLT_645_CODE_MASK) != AMRCtrl->Protocol) return ERROR_InvalidData;

			if((DLT645Frame.CtrlCode & DLT_645_ERROR_BIT) == 0)
			{
				if(AMRCtrl->Protocol == DLT_645_PROTOCOL_97)
				{
					StoreAlarmTaskAMRData_97(PORTn, AMRCtrl, &DLT645Frame);
				}
				else
				{
					StoreAlarmTaskAMRData_07(PORTn, AMRCtrl, &DLT645Frame);
				}
			}
			AMRCtrl->PLCTaskIDAckNum++;
			///////////////////////////ybwhz����///////////////////////////////
				if(YBWHZ_TEST)
				{
					u8 *p8;
					u8 count = 0;
					MC(0, ADDR_YBWHZ_DATABUFF, 64); 
					p8=(u8*)(ADDR_YBWHZ_DATABUFF);
					p8[count++] = 'A';
					p8[count++] = 'M';
					p8[count++] = 'R';
					p8[count++] = 'T';
					
					p8[count++] = AMRCtrl->MeterIndex/10%10 +'0';
					p8[count++] = AMRCtrl->MeterIndex/1%10 +'0';
					p8[count++] = '*';
					p8[count++] = AMRCtrl->PLCTaskIDAckNum/10%10 +'0';
					p8[count++] = AMRCtrl->PLCTaskIDAckNum/1%10 +'0';
					p8[count++] = '/';
					p8[count++] = AMRCtrl->PLCTaskIDAddNum/10%10 +'0';
					p8[count++] = AMRCtrl->PLCTaskIDAddNum/1%10 +'0';
					p8[count++] = ' ';
			
					p8[count++] = PLCManage->ReportTaskID/10%10 +'0';
					p8[count++] = PLCManage->ReportTaskID/1%10 +'0';
					p8[count++] = '#';
			
			
					QCSG_LogToRS232Port((u32)PORTn,ADDR_YBWHZ_DATABUFF,count);
				}
			////////////////////////////ybwhz����/////////////////////////////
			if(AMRCtrl->PLCTaskIDAckNum < AMRCtrl->PLCTaskIDAddNum) return ERROR_WRONG_LENGTH;
					//���ڳ�������δ�ϱ���������ȴ�
			pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
			pAckTimer[0] = 30;//����ȴ�ʵʱ����������
			
			AlarmAMRData->State[1] = 0x01;	//�����Ѹ���
			Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
			if(Result == NO_ERROR)
			{
				AMRCtrl->OldState = AMR_Tx;
				pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
				SwitchMeterClearAMRInfo(AMRCtrl);
			}
			else if(Result == ERROR_NO_AMR_TASK)
			{
				AMRCtrl->OldState = AMR_Idle;
				AMRCtrl->AMR_ID = 0;
				WarningCancel(PORTn);
			}
			else
			{
			
			}	
		}
		PLCManage->DataLenInDataBuf = 0;
		return ERROR_WRONG_ACCESS;	
	}
	DataBuf = (u8*)Get_AMR_CH_DataBufAddr(PORTn);
	if(DataBuf == NULL) return ERROR_POINTER_NULL;
	DataLen = PLCManage->DataLenInDataBuf;
	PLCManage->DataLenInDataBuf = 0;
	
	Task->PLCTaskIDAckNum++;
	///////////////////////////ybwhz����///////////////////////////////
		if(YBWHZ_TEST)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64); 
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'R';
			p8[count++] = 'T';
			p8[count++] = ' ';
			p8[count++] = ':';
			
			p8[count++] = SwitchPnToNum(Task->CurPn)/10%10 +'0';
			p8[count++] = SwitchPnToNum(Task->CurPn)/1%10 +'0';
			p8[count++] = '*';
			p8[count++] = Task->PLCTaskIDAckNum/10%10 +'0';
			p8[count++] = Task->PLCTaskIDAckNum/1%10 +'0';
			p8[count++] = '/';
			p8[count++] = Task->PLCTaskIDAddNum/10%10 +'0';
			p8[count++] = Task->PLCTaskIDAddNum/1%10 +'0';
			p8[count++] = ' ';
	
			p8[count++] = PLCManage->ReportTaskID/10%10 +'0';
			p8[count++] = PLCManage->ReportTaskID/1%10 +'0';
			p8[count++] = '#';
	
	
			QCSG_LogToRS232Port((u32)PORTn,ADDR_YBWHZ_DATABUFF,count);
		}
	////////////////////////////ybwhz����/////////////////////////////

	
	pAckTimer[0] = PLCManage->AMRTimeOut;
	Result = DLT645FrameDcompress(&DLT645Frame, DataBuf, DataLen);
	if(Result != NO_ERROR) return Result;

	if(Task->SrcDataType == DATA_TYPE_STRUCT)
	{
		if(MemCompare(DLT645Frame.Addr, Task->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
		if(GetProtocolTypeByCtrlCode(DLT645Frame.CtrlCode & DLT_645_CODE_MASK) != Task->Protocol) return ERROR_InvalidData;

		Task->Cur645_DI = 0;
		Task->Cur645_DI_Step = 0;
		Index = Task->CurNW_DI_TblIndex;		
		if((DLT645Frame.CtrlCode & DLT_645_ERROR_BIT) == 0)
		{		
			Cur645_DI = Get645FrameDataID(&DLT645Frame, Task->Protocol);
			while(Task->Cur645_DI != 0xFFFFFFFF)
			{
				if(Task->Protocol == DLT_645_PROTOCOL_97)
				{
					Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[Task->Cur645_DI_Step].u32Ident;
					TempOffSet = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[Task->Cur645_DI_Step].u8BlDataLen;
				}
				else
				{	
					Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[Task->Cur645_DI_Step].u32Ident;
					TempOffSet = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[Task->Cur645_DI_Step].u8BlDataLen;
				}
				if(Cur645_DI == Task->Cur645_DI) break;
				OffSet += TempOffSet;
				Task->Cur645_DI_Step++;
			}
			if(Cur645_DI != Task->Cur645_DI) return ERROR_InvalidData;	
		}
		else
		{
			while((Task->Cur645_DI != 0xFFFFFFFF) && (Task->Cur645_DI_Step < (PLCManage->ReportTaskID - RT_AMR_TASK_ID_START)))
			{
				if(Task->Protocol == DLT_645_PROTOCOL_97)
				{
					Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[Task->Cur645_DI_Step].u32Ident;
					TempOffSet = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64597Resolve[Task->Cur645_DI_Step].u8BlDataLen;
				}
				else
				{	
					Task->Cur645_DI = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[Task->Cur645_DI_Step].u32Ident;
					TempOffSet = gc_stQCSGAFN0C_0D_DLT645_Table[Index].stDLT64507Resolve[Task->Cur645_DI_Step].u8BlDataLen;
				}
				OffSet += TempOffSet;
				Task->Cur645_DI_Step++;
			}
			Task->Cur645_DI_Step = PLCManage->ReportTaskID - RT_AMR_TASK_ID_START;
		}
		
		AMRCtrl->Cur645DataLen = 0;
		Result = Convert645ToNWAMRData(&DLT645Frame, Index, Task->Cur645_DI_Step, Task->Protocol, AMRCtrl->Cur645Data, &AMRCtrl->Cur645DataLen);	

		OffSet += sizeof(QCSG_PnListStruct);
		MR((u32)&Task->AckData[OffSet], (u32)AMRCtrl->Cur645Data, AMRCtrl->Cur645DataLen);
		if((OffSet + AMRCtrl->Cur645DataLen) >= Task->AckDataLen)
		{
			Task->AckDataLen = OffSet + AMRCtrl->Cur645DataLen;
		}
		
		if(Task->PLCTaskIDAckNum >= Task->PLCTaskIDAddNum)
		{
			if((Task->Protocol == DLT_645_PROTOCOL_97) && (Task->AckDataLen > sizeof(QCSG_PnListStruct)))
			{
				DataLen = gc_stQCSGAFN0C_0D_DLT645_Table[Task->CurNW_DI_TblIndex].u16Dlen;		
				if((u16)DataLen > (Task->AckDataLen - sizeof(QCSG_PnListStruct)))
				{	
					LeftLen = DataLen - (Task->AckDataLen - sizeof(QCSG_PnListStruct));
					for(i = 0; i < LeftLen; i++)
					{
						Task->AckData[Task->AckDataLen++] = 0xFF;
					}
				}
			}
		}
	}
	else if(Task->SrcDataType == DATA_TYPE_RELAY)
	{
		if(MemCompare(DLT645Frame.Addr, Task->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
		if(PLCManage->ReportTaskID != (Task->PLCTaskIDAddNum - 1))  return ERROR_WRONG_ACCESS;
		Task->AckData[Task->AckDataLen++] = Task->RelayType;
		Task->AckData[Task->AckDataLen++] = DataLen;
		MR((u32)&Task->AckData[Task->AckDataLen], (u32)DataBuf, DataLen);
		Task->AckDataLen += DataLen;
	}
	else if(Task->SrcDataType == DATA_TYPE_FRAME)
	{
		if(MemCompare(DLT645Frame.Addr, Task->Addr, METER_ADDR_SIZE) != 0) return ERROR_WRONG_ADDR;
		if(PLCManage->ReportTaskID != (Task->PLCTaskIDAddNum - 1))  return ERROR_WRONG_ACCESS;
		MR((u32)&Task->AckData[Task->AckDataLen], (u32)DataBuf, DataLen);
		Task->AckDataLen += DataLen;
	}
	else //if(Task->SrcDataType == DATA_TYPE_BCAST_CMD)
	{
		
	}

	if(Task->PLCTaskIDAckNum >= Task->PLCTaskIDAddNum)
	{//ʵʱ�����Ѿ�ȫ���ظ���
	if((AMRCtrl->OldState == AMR_WaitRx)&& (AMRCtrl->AMR_ID_Step == AMR_ALARM_TASK_ID)&& (AMRCtrl->PLCTaskIDAckNum < AMRCtrl->PLCTaskIDAddNum))
	{
		//���ڳ�����û�г��� ����ת�������ڳ�������
		Task->Flag |= RT_AMR_DATA_READY;
		AMRCtrl->State = AMR_WaitRx;
		pAckTimer[0] = PLCManage->AMRTimeOut;
	}
	else
		{
		Task->Flag |= RT_AMR_DATA_READY;
		AMRCtrl->State = AMR_Pause;
		pAckTimer[0] = AMR_PAUSE_MAX_TIME;
		}
	}

	return Result;
}



ResultEnum PLC_AMR_ResumeProcess(u32 PORTn, AMRCtrlStruct* AMRCtrl, PLCManageStruct* PLCManage)
{
	u16* pAckTimer;
	ResultEnum Result = NO_ERROR;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	if(pAckTimer[0] != 0) return ERROR_WRONG_TIME;

if((AMRCtrl->OldState == AMR_WaitRx)&& (AMRCtrl->AMR_ID_Step == AMR_ALARM_TASK_ID))
{
	AMRCtrl->State = AMR_WaitRx;
}
else
{
	AMRCtrl->State = AMRCtrl->OldState;
	if(AMRCtrl->OldState != AMR_Idle)
	{
		AMRCtrl->MeterIndex--;
		if(AMRCtrl->AMR_ID_Step == AMR_NORMAL_TASK_ID)
		{
			AMRCtrl->NormalTask->PnListIndex--;
		}
		else if(AMRCtrl->AMR_ID_Step == AMR_RELAY_TASK_ID)
		{
			AMRCtrl->RelayTask->CurTaskID--;
		}
		else 
		{

		}
		Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
		if(Result == NO_ERROR)
		{
			AMRCtrl->State = AMR_Tx;
			pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
			MC(0, (u32)PLCManage->AMR_Data, sizeof(NwDIAMRDataStruct) * SIZE_12);
			SwitchMeterClearAMRInfo(AMRCtrl);
		}
		else
		{
			AMRCtrl->State = AMR_Idle;
			AMRCtrl->AMR_ID = 0;
		}
	}
	else
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
	}
	WarningCancel(PORTn);
}
	return Result;
}


//�ݴ�����
void PLC_CheckModle(u32 PORTn, PLCManageStruct* PLCManage)
{
	u16* pAckTimer;
	QCSGLocalFrameStruct LocalFrame;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	AMRCtrlStruct* AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);

	switch(pTerminalRouter->RouterInitTask)
	{
		case PLC_CHECK_MODLE_TASK_NUM:
			PLC_TxCheckUnfinishTaskNum(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_CHECK_MODLE_TASK_NUM_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_READ_UNFINISH_TASK_DONE) != 0)
			{
				PLCManage->Flag &= ~PLC_FLAG_READ_UNFINISH_TASK_DONE;
				if((AMRCtrl->PLCTaskIDAddNum - AMRCtrl->PLCTaskIDAckNum) != pTerminalRouter->UnfinishTaskNum)
				{// ģ��δ������������ն˲�һ�£�������ʼ��������
					//pTerminalRouter->RouterInitTask++;
					PLCManage->State = PLC_AMR_STATE;
					AMRCtrl->State = AMR_Tx;
					pAckTimer[0] = 0;
					pTerminalRouter->RouterInitTask = PLC_ADD_TASK_STOP_TASK;
					pTerminalRouter->RetryCnt = 0;
				}
				else
				{// ģ��δ������������ն�һ�£���
					//pTerminalRouter->RouterInitTask = PLC_FILE_SYNC_READ_NODE_INFO;
					PLCManage->State = PLC_AMR_STATE;
					AMRCtrl->State = AMRCtrl->OldState;
					pTerminalRouter->RouterInitTask = pTerminalRouter->Old_RouterInitTask;
					pTerminalRouter->RetryCnt = 0;
				}
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		default:
			pTerminalRouter->RouterInitTask = 0;
			break;
		
	}
}



//err20190403 �ز�ģ�����
void PLC_YBWHZ_Test(u32 PORTn, PLCManageStruct* PLCManage)
{
//	u16* pAckTimer;
	QCSGLocalFrameStruct LocalFrame;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	AMRCtrlStruct* AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	//pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);

	switch(pTerminalRouter->RouterInitTask)
	{
		case PLC_YBWHZ_TEST_NUM1:
			PLC_Tx_YBWHZ_Test(PORTn, PLCManage, &LocalFrame);
			break;
		case PLC_YBWHZ_TEST_NUM1_WAIT_ACK:
			if((PLCManage->Flag & PLC_FLAG_YBWHZ_TEST_DONE) != 0)
			{
				PLCManage->Flag &= ~PLC_FLAG_YBWHZ_TEST_DONE;
				pTerminalRouter->RouterInitTask++;
				PLCManage->State = PLC_AMR_STATE;
				AMRCtrl->State = AMRCtrl->OldState;
				pTerminalRouter->RouterInitTask = pTerminalRouter->Old_RouterInitTask;
				pTerminalRouter->RetryCnt = 0;
				/*if((AMRCtrl->PLCTaskIDAddNum - AMRCtrl->PLCTaskIDAckNum) != pTerminalRouter->UnfinishTaskNum)
				{// ģ��δ������������ն˲�һ�£�������ʼ��������
					//pTerminalRouter->RouterInitTask++;
					PLCManage->State = PLC_AMR_STATE;
					AMRCtrl->State = AMR_Tx;
					pAckTimer[0] = 0;
					pTerminalRouter->RouterInitTask = PLC_ADD_TASK_STOP_TASK;
					pTerminalRouter->RetryCnt = 0;
				}
				else
				{// ģ��δ������������ն�һ�£���
					//pTerminalRouter->RouterInitTask = PLC_FILE_SYNC_READ_NODE_INFO;
					PLCManage->State = PLC_AMR_STATE;
					AMRCtrl->State = AMRCtrl->OldState;
					pTerminalRouter->RouterInitTask = pTerminalRouter->Old_RouterInitTask;
					pTerminalRouter->RetryCnt = 0;
				}*/
			}
			else
			{
				if(pTerminalRam->RouterAck_S_Timer == 0)
				{
					pTerminalRouter->RouterInitTask--;
				}
			}
			break;
		default:
			pTerminalRouter->RouterInitTask = 0;
			break;
		
	}
	
}


ResultEnum PLC_TxCheckUnfinishTaskNum(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_RECOGNIZE_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_02;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8000203;
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;//���޸�
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_RECOGNIZE_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

ResultEnum PLC_TxCheckRemainTask(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_WAIT_ACK_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_02;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8000206;
		LocalFrame->DataLen = 0;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;//���޸�
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_WAIT_ACK_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		//PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_INIT;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

ResultEnum PLC_Tx_YBWHZ_Test(u32 PORTn, PLCManageStruct* PLCManage, QCSGLocalFrameStruct* LocalFrame)
{
	u32 FrameLen = 0, AddrTxBuf = 0;
	UartTxReqStruct UartTxReq;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if(pTerminalRam->RouterAck_S_Timer != 0) return ERROR_WRONG_TIME;
	
	if(pTerminalRouter->RetryCnt < PLC_CMD_MAX_CNT)
	{
		MC(0, (u32)LocalFrame, sizeof(QCSGLocalFrameStruct));
		pTerminalRam->RouterAck_S_Timer = PLC_RECOGNIZE_TIME_OUT;
		LocalFrame->FC = LOCAL_FRAME_PRM_BIT;
		LocalFrame->AFN = LOCAL_FRAME_AFN_03;
		LocalFrame->SEQ = PLCManage->SEQ++;
		LocalFrame->DI = 0xE8030311;//0xE8000203;
		LocalFrame->DataLen = 0;
		LocalFrame->Data[LocalFrame->DataLen++] = (1);
		LocalFrame->Data[LocalFrame->DataLen++] = (1) >> 8;
		LocalFrame->Data[LocalFrame->DataLen++] = 3;

		AddrTxBuf = GetUartPortTxBuf(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		MC(0, (u32)&UartTxReq, sizeof(UartTxReqStruct));				
		UartTxReq.SrcTask = TASK_ID_PLC_AMR;//���޸�
		UartTxReq.NeedAck = 1;
		UartTxReq.FrameType = FRAME_TYPE_QCSG_LOCAL;
		UartTxReq.RetryCnt = 0;
		UartTxReq.BpsCtrl = 0;
		UartTxReq.TimeOut = PLC_RECOGNIZE_TIME_OUT;
		UartTxReq.DataLen = FrameLen;
		UartTxReq.DataBuf = (u8*)AddrTxBuf;
		if(UartPortTxReqNew(PORTn, &UartTxReq) == NO_ERROR)
		{
			pTerminalRouter->RouterInitTask++;
			pTerminalRouter->RetryCnt++;
		}
	}
	else
	{
		PLCManage->State = PLC_RESET_INIT;
		pTerminalRouter->RouterResetCnt = 0;		
		pTerminalRouter->RouterInitTask = 0;
		pTerminalRouter->RetryCnt = 0;
	}	
	return NO_ERROR;
}

ResultEnum QCSG_LocalModuleFrameProcess(u32 PORTn, PLCManageStruct* PLCManage)
{	
	QCSGLocalFrameStruct LocalFrame;
	UartTaskCtrlStruct* pUTask;
	UartRxChCtrlStruct* UartRxChCtrl;
	ResultEnum Result = NO_ERROR;
	
	//u32 FrameLen = 0;
	//u8 DataType = 0, FrameType = 0;
	//u8* FrameBuf = (u8*)GetUartPortRxBuf(PORTn, DATA_TYPE_FRAME);
	//Result = UartPortRxReq(PORTn, FrameBuf, &FrameLen, &FrameType, &DataType);
	//if(Result != NO_ERROR) return Result;
	//if(FrameType != FRAME_TYPE_QCSG_LOCAL) return ERROR_WRONG_PROTOCOL;
	
	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);
	if(UartRxChCtrl == NULL) return ERROR_POINTER_NULL;
	if(UartRxChCtrl->Empty == BUFFER_EMPTY) return ERROR_BUF_EMPTY;
	if(UartRxChCtrl->FrameBuf == NULL) return ERROR_POINTER_NULL;	
	pUTask = (UartTaskCtrlStruct*)GetUartTaskCtrl(PORTn);
	if(pUTask == NULL) return ERROR_POINTER_NULL;
	pUTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
	UartRxChCtrl->Empty = BUFFER_EMPTY;
	if(UartRxChCtrl->FrameType != FRAME_TYPE_QCSG_LOCAL) return ERROR_WRONG_PROTOCOL;	

	MC(0, (u32)&LocalFrame, sizeof(QCSGLocalFrameStruct));
	Result = QCSG_LocalFrameDecompress(UartRxChCtrl->FrameBuf, (u32)UartRxChCtrl->FrameLen, &LocalFrame);
	if(Result != NO_ERROR) return Result;
	#if LOG_1376_2==1//0=û,1=��
		LOG(13762,0,PLC_PORT,UartRxChCtrl->FrameBuf, UartRxChCtrl->FrameLen,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
	#endif

	PLCManage->LastRcvTicks = HAL_GetSystemTick();
	switch(LocalFrame.AFN)
	{
		case 0x00:
			LocalFrame_AFN00_Process(PORTn, &LocalFrame);
			break;
		case 0x01:
			LocalFrame_AFN01_Process(PORTn, &LocalFrame);
			break;
		case 0x02:
			LocalFrame_AFN02_Process(PORTn, &LocalFrame);
			break;
		case 0x03:
			LocalFrame_AFN03_Process(PORTn, &LocalFrame);
			break;
		case 0x04:
			LocalFrame_AFN04_Process(PORTn, &LocalFrame);
			break;
		case 0x05:
			LocalFrame_AFN05_Process(PORTn, &LocalFrame);
			break;
		case 0x06:
			LocalFrame_AFN06_Process(PORTn, &LocalFrame);
			break;
		case 0x07:
			LocalFrame_AFN07_Process(PORTn, &LocalFrame);
			break;
		default:
			break;
		
	}
	return NO_ERROR;
}



ResultEnum LocalFrame_AFN00_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	if(LocalFrame->DI == 0xE8010002)
	{
		return ERROR_WRONG_ACCESS;
	}
	
	switch(PLCManage->State)
	{
		case PLC_MODLE_RECOGNIZE:
			if(pTerminalRouter->RouterInitTask == PLC_RECOGNIZE_WAIT_INIT_FILE_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_INIT_FILE_DONE;
			}
			if(pTerminalRouter->RouterInitTask == PLC_RECOGNIZE_WAIT_INIT_TASK_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_INIT_TASK_DONE;
			}
			if(pTerminalRouter->RouterInitTask == PLC_RECOGNIZE_WAIT_SET_MODLE_ADDR_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_SET_MODLE_ADDR_DONE;
			}
			break;
		case PLC_FILE_SYNC:
			if(pTerminalRouter->RouterInitTask == PLC_FILE_SYNC_WAIT_INIT_FILE_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_INIT_FILE_DONE;
			}
			if(pTerminalRouter->RouterInitTask == PLC_FILE_SYNC_WAIT_ADD_NODE_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_ADD_NODE_DONE;
			}
			if(pTerminalRouter->RouterInitTask == PLC_FILE_SYNC_WAIT_INIT_TASK_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_INIT_TASK_DONE;
			}
			break;
		case PLC_FILE_ADD:
			if(pTerminalRouter->RouterInitTask == PLC_FILE_ADD_WAIT_ADD_NODE_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_ADD_NODE_DONE;
			}
			break;
		case PLC_AMR_STATE:
			if(pTerminalRouter->RouterInitTask == PLC_ADD_TASK_STOP_TASK_WAIT_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_STOP_TASK_DONE;
			}
			if(pTerminalRouter->RouterInitTask == PLC_ADD_TASK_INIT_WAIT_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_INIT_TASK_DONE;
			}
			if(pTerminalRouter->RouterInitTask == PLC_ADD_TASK_ADD_ONE_WAIT_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_ADD_ONE_TASK_DONE;
			}
			if(pTerminalRouter->RouterInitTask == PLC_ADD_TASK_START_TASK_WAIT_ACK)
			{
				PLCManage->Flag |= PLC_FLAG_START_TASK_DONE;
			}
			break;
	}
	return NO_ERROR;
}

ResultEnum LocalFrame_AFN01_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	
	return NO_ERROR;
}


ResultEnum LocalFrame_AFN02_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	switch(LocalFrame->DI)
	{
		case 0xE8000203:
			LocalFrame_AFN02_E8000203_Process(PORTn, LocalFrame);
			break;
		case 0xE8000206:
			LocalFrame_AFN02_E8000206_Process(PORTn, LocalFrame);
			break;
	}
	return NO_ERROR;
}
/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=02, E8040204,���ز�ѯδ���������
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: nw
��дʱ ��; 2018-7-4
*****************************************************************************************************/
ResultEnum LocalFrame_AFN02_E8000203_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	u16 Index = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;

	pTerminalRouter->UnfinishTaskNum = LocalFrame->Data[Index++];
	pTerminalRouter->UnfinishTaskNum += LocalFrame->Data[Index++] << 8;

	PLCManage->Flag |= PLC_FLAG_READ_UNFINISH_TASK_DONE;

	return NO_ERROR;		
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=02, E8000206,��ѯʣ��ɷ���������
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: nw
��дʱ ��; 2018-7-4
*****************************************************************************************************/
ResultEnum LocalFrame_AFN02_E8000206_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	u16 Index = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	pTerminalRouter->ResDistribTaskNum = LocalFrame->Data[Index++];
	pTerminalRouter->ResDistribTaskNum += LocalFrame->Data[Index++] << 8;

	return NO_ERROR;		
}

ResultEnum LocalFrame_AFN03_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	switch(LocalFrame->DI)
	{
		case 0xE8000301:
			LocalFrame_AFN03_E8000301_Process(PORTn, LocalFrame);
			break;
		case 0xE8000302:
			LocalFrame_AFN03_E8000302_Process(PORTn, LocalFrame);
			break;
		case 0xE8000303:
			LocalFrame_AFN03_E8000303_Process(PORTn, LocalFrame);
			break;
		case 0xE8030304:
			break;
		case 0xE8000305:
			LocalFrame_AFN03_E8000305_Process(PORTn, LocalFrame);
			break;
		case 0xE8030306:
			break;
		case 0xE8000307:
			LocalFrame_AFN03_E8000307_Process(PORTn, LocalFrame);
			break;
		case 0xE8030308:
			break;
		case 0xE8040304:
			LocalFrame_AFN03_E8040304_Process(PORTn, LocalFrame);
			break;
		case 0xE8040306:
			LocalFrame_AFN03_E8040306_Process(PORTn, LocalFrame);
			break;
		case 0xE8040308:
			LocalFrame_AFN03_E8040308_Process(PORTn, LocalFrame);
			break;
		case 0xE8040311:
			LocalFrame_AFN03_E8040321_Process(PORTn, LocalFrame);//���ز�ѯ����������Ϣ
			break;
		case 0xE8040312:
			LocalFrame_AFN03_E8040321_Process(PORTn, LocalFrame);//���ز�ѯ������λ��̨����Ϣ
			break;
		case 0xE8040321:
			LocalFrame_AFN03_E8040321_Process(PORTn, LocalFrame);//����������ѯ�����ڵ�汾��Ϣ
			break;
	}
	return NO_ERROR;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8000301,��ѯ����ͨ��ģ�鳧�̴�����汾��Ϣ
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8000301_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	u16 Index = 0;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	pTerminalRouter->RouterMFCode[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterMFCode[1] = LocalFrame->Data[Index++];
	
	pTerminalRouter->RouterICCode[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterICCode[1] = LocalFrame->Data[Index++];

	pTerminalRouter->RouterVerDate[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterVerDate[1] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterVerDate[2] = LocalFrame->Data[Index++];

	pTerminalRouter->RouterVer[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterVer[1] = LocalFrame->Data[Index++];

	return NO_ERROR;		
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8000302,��ѯ����ͨ��ģ������ģʽ��Ϣ
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8000302_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	//UARTCtrl_TypeDef* UARTCtrl;
	UartTaskCtrlStruct* UartTask;
	
	u16 Index = 0, FrameLen = 0;
	u32 AddrTxBuf = 0;	
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	
	pTerminalRouter->RouterRunMode_1 = LocalFrame->Data[Index++];
	pTerminalRouter->MaxTxByte = LocalFrame->Data[Index++];
	pTerminalRouter->MaxTxByte += LocalFrame->Data[Index++] << 8;
	pTerminalRouter->MaxFileByte = LocalFrame->Data[Index++];
	pTerminalRouter->MaxFileByte += LocalFrame->Data[Index++] << 8;
	pTerminalRouter->MaxUpDateTime = LocalFrame->Data[Index++];

	MR((u32)pTerminalRouter->RouterAddr, (u32)&LocalFrame->Data[Index], METER_ADDR_SIZE);
	Index += METER_ADDR_SIZE;
	pTerminalRouter->MaxNodeNum = LocalFrame->Data[Index++];
	pTerminalRouter->MaxNodeNum += LocalFrame->Data[Index++] << 8;
	pTerminalRouter->TotalNodeNum = LocalFrame->Data[Index++];
	pTerminalRouter->TotalNodeNum += LocalFrame->Data[Index++] << 8;
	pTerminalRouter->MaxRWNodeNum = LocalFrame->Data[Index++];
	pTerminalRouter->MaxRWNodeNum += LocalFrame->Data[Index++] << 8;
	
	pTerminalRouter->RouterDate[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterDate[1] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterDate[2] = LocalFrame->Data[Index++];

	pTerminalRouter->RouterMFCode[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterMFCode[1] = LocalFrame->Data[Index++];
	
	pTerminalRouter->RouterICCode[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterICCode[1] = LocalFrame->Data[Index++];

	pTerminalRouter->RouterVerDate[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterVerDate[1] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterVerDate[2] = LocalFrame->Data[Index++];

	pTerminalRouter->RouterVer[0] = LocalFrame->Data[Index++];
	pTerminalRouter->RouterVer[1] = LocalFrame->Data[Index++];

	if(PLCManage->State == PLC_RESET_INIT)
	{
		LocalFrame->FC  &= ~(LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_DIR_BIT);
		LocalFrame->AFN = LOCAL_FRAME_AFN_00;		
		LocalFrame->DI  = 0xE8010001;
		LocalFrame->DataLen = 2;
		LocalFrame->Data[0] = 0x00;
		LocalFrame->Data[1] = 0x00;
		
		//UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
		UartTask = GetUartTaskCtrl(PORTn);
		AddrTxBuf = Get_ADDR_UARTnTx(PORTn);
		FrameLen = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
		//UARTCtrl->TxByte = FrameLen;
		UartTask->AckLen = FrameLen;
		UartTask->Flag |= UART_FLAG_TX_ACK;	
		pTerminalRam->RouterAck_S_Timer = 2;
	}
	else
	{
		PLCManage->Flag |= PLC_FLAG_READ_MODLE_INFO_DONE;
	}	
	return NO_ERROR;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8000303,��ѯ����ͨ��ģ�����ڵ��ַ
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8000303_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	MR((u32)pTerminalRouter->RouterAddr, (u32)LocalFrame->Data, METER_ADDR_SIZE);
	PLCManage->Flag |= PLC_FLAG_READ_MODLE_ADDR_DONE;

	return NO_ERROR;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8000305,��ѯ����ͨ��ģ��ӽڵ�����
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8000305_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	MR((u32)&pTerminalRouter->TotalNodeNum, (u32)LocalFrame->Data, 2);//err20180228-16 ԭ����:MR((u32)pTerminalRouter->TotalNodeNum, (u32)LocalFrame->Data, 2);//δȡ��ַ�����������ݶ���ַ����
	PLCManage->Flag |= PLC_FLAG_READ_NODE_NUM_DONE;

	return NO_ERROR;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8000307,��ѯ�ӽڵ�����ע�����
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8000307_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
/*	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	MR((u32)&pTerminalRouter->TotalNodeNum, (u32)LocalFrame->Data, 2);
	PLCManage->Flag |= PLC_FLAG_READ_NODE_NUM_DONE;
*/
	return NO_ERROR;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8040304,���ز�ѯͨ����ʱʱ��
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8040304_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
/*	u16 TotalNodeNum = 0, Index = 0;
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	TotalNodeNum = LocalFrame->Data[Index++];
	TotalNodeNum += LocalFrame->Data[Index++] << 8;

	if(TotalNodeNum != pTerminalRouter->TotalNodeNum) return ERROR_InvalidData;
	pTerminalRouter->CurReadNodeNum = LocalFrame->Data[Index++];
	
	MR((u32)ADDR_ReadPLCNodeBuf, (u32)&LocalFrame->Data[Index], pTerminalRouter->CurReadNodeNum * METER_ADDR_SIZE);
	PLCManage->Flag |= PLC_FLAG_READ_NODE_INFO_DONE;
*/
	return NO_ERROR;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8040306,���ز�ѯ�Ĵӽڵ���Ϣ
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8040306_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	u16 TotalNodeNum = 0, Index = 0;
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	TotalNodeNum = LocalFrame->Data[Index++];
	TotalNodeNum += LocalFrame->Data[Index++] << 8;

	pTerminalRouter->TotalNodeNum = TotalNodeNum;
	pTerminalRouter->CurReadNodeNum = LocalFrame->Data[Index++];
	
	MR((u32)ADDR_PLC_AMR_DataBuf, (u32)&LocalFrame->Data[Index], pTerminalRouter->CurReadNodeNum * METER_ADDR_SIZE);
	PLCManage->Flag |= PLC_FLAG_READ_NODE_INFO_DONE;

	return NO_ERROR;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8040308,���ز�ѯ�ӽڵ�ĸ��ڵ�
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8040308_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
/*	u16 TotalNodeNum = 0, Index = 0;
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;

	TotalNodeNum = LocalFrame->Data[Index++];
	TotalNodeNum += LocalFrame->Data[Index++] << 8;

	if(TotalNodeNum != pTerminalRouter->TotalNodeNum) return ERROR_InvalidData;
	pTerminalRouter->CurReadNodeNum = LocalFrame->Data[Index++];
	
	MR((u32)ADDR_ReadPLCNodeBuf, (u32)&LocalFrame->Data[Index], pTerminalRouter->CurReadNodeNum * METER_ADDR_SIZE);
	PLCManage->Flag |= PLC_FLAG_READ_NODE_INFO_DONE;
*/
	return NO_ERROR;
}


/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8040311,���ز�ѯ����������Ϣ
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: ybwhz
��дʱ ��; 2019-4-4
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8040311_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{

	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	PLCManage->Flag |= PLC_FLAG_YBWHZ_TEST_DONE;

	return NO_ERROR;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8040312,���ز�ѯ������λ��̨����Ϣ
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: ybwhz
��дʱ ��; 2019-4-4
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8040312_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{

	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	PLCManage->Flag |= PLC_FLAG_YBWHZ_TEST_DONE;

	return NO_ERROR;
}


/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=03, E8040321,����������ѯ�����ڵ�汾��Ϣ
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: ybwhz
��дʱ ��; 2019-4-3
��ע:�����Զ��������ز��ݲ�֧�ָ����ݱ�ʶ
*****************************************************************************************************/
ResultEnum LocalFrame_AFN03_E8040321_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{

	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	PLCManage->Flag |= PLC_FLAG_YBWHZ_TEST_DONE;

	return NO_ERROR;
}



ResultEnum LocalFrame_AFN04_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	switch(LocalFrame->DI)
	{
		case 0xE8020401:
			break;
		case 0xE8020402:
			break;
		case 0xE8020403:
			break;
		case 0xE8020404:
			break;
		case 0xE8020405:
			break;
		case 0xE8020406:
			break;
	}
	return NO_ERROR;
}


ResultEnum LocalFrame_AFN05_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	ResultEnum Result = NO_ERROR;
	switch(LocalFrame->DI)
	{
		case 0xE8050501://�ϱ���������
			Result = LocalFrame_AFN05_E8050501_Process(PORTn, LocalFrame);
			break;
		case 0xE8050502://�ϱ��ӽڵ��¼�
			Result = LocalFrame_AFN05_E8050502_Process(PORTn, LocalFrame);
			break;
		case 0xE8050503://�ϱ��ӽڵ���Ϣ
			Result = LocalFrame_AFN05_E8050503_Process(PORTn, LocalFrame);
			break;
		case 0xE8050504://�ϱ��ӽڵ�����ע�����
			Result = LocalFrame_AFN05_E8050504_Process(PORTn, LocalFrame);
			break;
		case 0xE8050505://�ϱ�����״̬
			Result = LocalFrame_AFN05_E8050505_Process(PORTn, LocalFrame);
			break;
		default:
			Result = ERROR_NoSuchDataID;
			break;
	}
	return Result;
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=05, E8050501��PLC ģ���ϱ���������
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN05_E8050501_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	UartTaskCtrlStruct* UartTask;
	AMRCtrlStruct* AMRCtrl;
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	u8* PLCDataBuf = (u8*)ADDR_PLC_AMR_DataBuf;
	u16 Index = 0;
	u32 AddrTxBuf = 0;

	//AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	//if((AMRCtrl->State == AMR_Pause) || (AMRCtrl->State == AMR_RealTimeTx)) return ERROR_WRONG_ACCESS;

	//err20190308 ��ʵʱ�������ڷ���ʱ����Ӧ���ϱ�֡�����򽫶�ʧ֡��(΢��������)
	AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	if((AMRCtrl->State == AMR_Pause) || (AMRCtrl->State == AMR_RealTimeTx)) return ERROR_WRONG_ACCESS;

	MR((u32)PLCManage->SrcAddr, (u32)LocalFrame->SrcAddr, METER_ADDR_SIZE);
	MR((u32)PLCManage->DstAddr, (u32)LocalFrame->DstAddr, METER_ADDR_SIZE);

	PLCManage->ReportTaskID = LocalFrame->Data[Index++];
	PLCManage->ReportTaskID += LocalFrame->Data[Index++] << 8;
	PLCManage->ReportDataLen = LocalFrame->Data[Index++];

	PLCManage->DataLenInDataBuf = PLCManage->ReportDataLen;
	MR((u32)PLCDataBuf, (u32)&LocalFrame->Data[Index], PLCManage->ReportDataLen);

	LocalFrame->FC  &= ~(LOCAL_FRAME_DIR_BIT | LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT);
	LocalFrame->AFN = LOCAL_FRAME_AFN_00;		
	LocalFrame->DI  = 0xE8010001;
	LocalFrame->DataLen = 2;
	LocalFrame->Data[0] = 0x00;
	LocalFrame->Data[1] = 0x00;
	
	UartTask = GetUartTaskCtrl(PORTn);
	AddrTxBuf = Get_ADDR_UARTnTx(PORTn);
	Index = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
	//UARTCtrl->TxByte = Index;
	UartTask->AckLen = Index;
	UartTask->Flag |= UART_FLAG_TX_ACK;	

	return NO_ERROR;		
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=05, E8050502��PLC ģ���ϱ��ӽڵ��¼�
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN05_E8050502_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	//UARTCtrl_TypeDef* UARTCtrl;
	UartTaskCtrlStruct* UartTask;
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	u8* PLCDataBuf = (u8*)ADDR_PLC_AMR_DataBuf;
	u16 Index = 0;
	u32 AddrTxBuf = 0;

	MR((u32)PLCManage->SrcAddr, (u32)LocalFrame->SrcAddr, METER_ADDR_SIZE);
	MR((u32)PLCManage->DstAddr, (u32)LocalFrame->DstAddr, METER_ADDR_SIZE);

	PLCManage->ReportDataLen = LocalFrame->Data[Index++];
	PLCManage->DataLenInDataBuf = PLCManage->ReportDataLen;
	MR((u32)PLCDataBuf, (u32)&LocalFrame->Data[Index], PLCManage->ReportDataLen);

	LocalFrame->FC  &= ~(LOCAL_FRAME_DIR_BIT | LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT);
	LocalFrame->AFN = LOCAL_FRAME_AFN_00;		
	LocalFrame->DI  = 0xE8010001;
	LocalFrame->DataLen = 2;
	LocalFrame->Data[0] = 0x00;
	LocalFrame->Data[1] = 0x00;
	
	//UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UartTask = GetUartTaskCtrl(PORTn);
	AddrTxBuf = Get_ADDR_UARTnTx(PORTn);
	Index = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
	//UARTCtrl->TxByte = Index;
	UartTask->AckLen = Index;
	UartTask->Flag |= UART_FLAG_TX_ACK;	

	return NO_ERROR;		
}

/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=05, E8050503��PLC ģ���ϱ��ӽڵ���Ϣ
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN05_E8050503_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	//UARTCtrl_TypeDef* UARTCtrl;
	UartTaskCtrlStruct* UartTask;
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	u8* PLCDataBuf = (u8*)ADDR_PLC_AMR_DataBuf;
	u16 Index = 0;
	u32 AddrTxBuf = 0;

	MR((u32)PLCManage->SrcAddr, (u32)LocalFrame->SrcAddr, METER_ADDR_SIZE);
	MR((u32)PLCManage->DstAddr, (u32)LocalFrame->DstAddr, METER_ADDR_SIZE);

	PLCManage->ReportDataLen = LocalFrame->Data[Index++];

	PLCManage->DataLenInDataBuf = PLCManage->ReportDataLen;
	MR((u32)PLCDataBuf, (u32)&LocalFrame->Data[Index], PLCManage->ReportDataLen);

	LocalFrame->FC  &= ~(LOCAL_FRAME_DIR_BIT | LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT);
	LocalFrame->AFN = LOCAL_FRAME_AFN_00;		
	LocalFrame->DI  = 0xE8010001;
	LocalFrame->DataLen = 2;
	LocalFrame->Data[0] = 0x00;
	LocalFrame->Data[1] = 0x00;
	
	//UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UartTask = GetUartTaskCtrl(PORTn);
	AddrTxBuf = Get_ADDR_UARTnTx(PORTn);
	Index = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
	//UARTCtrl->TxByte = Index;
	UartTask->AckLen = Index;
	UartTask->Flag |= UART_FLAG_TX_ACK;	

	return NO_ERROR;		
}


/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=05, E8050504��PLC ģ���ϱ��ӽڵ�����ע�����
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
*****************************************************************************************************/
ResultEnum LocalFrame_AFN05_E8050504_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	//UARTCtrl_TypeDef* UARTCtrl;
	UartTaskCtrlStruct* UartTask;
	u16 Index = 0;
	u32 AddrTxBuf = 0;
/*
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	u8* PLCDataBuf = (u8*)ADDR_PLC_DataBuf;

	MR((u32)PLCManage->SrcAddr, (u32)LocalFrame->SrcAddr, METER_ADDR_SIZE);
	MR((u32)PLCManage->DstAddr, (u32)LocalFrame->DstAddr, METER_ADDR_SIZE);

	PLCManage->ReportDataLen = LocalFrame->Data[Index++];

	PLCManage->DataLenInDataBuf = PLCManage->ReportDataLen;
	MR((u32)PLCDataBuf, (u32)&LocalFrame->Data[Index], PLCManage->ReportDataLen);
*/
	LocalFrame->FC  &= ~(LOCAL_FRAME_DIR_BIT | LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT);
	LocalFrame->AFN = LOCAL_FRAME_AFN_00;		
	LocalFrame->DI  = 0xE8010001;
	LocalFrame->DataLen = 2;
	LocalFrame->Data[0] = 0x00;
	LocalFrame->Data[1] = 0x00;
	
	//UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UartTask = GetUartTaskCtrl(PORTn);
	AddrTxBuf = Get_ADDR_UARTnTx(PORTn);
	Index = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
	//UARTCtrl->TxByte = Index;
	UartTask->AckLen = Index;
	UartTask->Flag |= UART_FLAG_TX_ACK;	

	return NO_ERROR;		
}


/*****************************************************************************************************
��     ��: 	�������� PLC Э��д����AFN=05, E8050505��PLC ģ���ϱ�����״̬
��ڲ� ��: 	PORTn: ֡Դ�˿ں�
			LocalFrame: �����Դ����ָ֡��
��     ��: ���ز����Ƿ�ɹ�
��     д: zhaoyun
��дʱ ��; 2017-8-10
//ybwhz �ϱ�����״̬ 01:�������ܱ�δ��Ӧ  FF:��������(����ʱ)
*****************************************************************************************************/
ResultEnum LocalFrame_AFN05_E8050505_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{

	//UARTCtrl_TypeDef* UARTCtrl;
	UartTaskCtrlStruct* UartTask;
	u16 Index = 0;
	u32 AddrTxBuf = 0;
//̨�����
	//Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
//	AMRCtrlStruct* AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PORTn);
	//AlarmAMRDataStruct* AlarmAMRData = (AlarmAMRDataStruct*)ADDR_AlarmTaskAMRDataBuf_New;
	//PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	//u16* pAckTimer;
	//pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
//̨�����
/*	
	PLCManageStruct* PLCManage = (PLCManageStruct*)ADDR_PLCManageCtrl;
	u8* PLCDataBuf = (u8*)ADDR_PLC_DataBuf;
	

	MR((u32)PLCManage->SrcAddr, (u32)LocalFrame->SrcAddr, METER_ADDR_SIZE);
	MR((u32)PLCManage->DstAddr, (u32)LocalFrame->DstAddr, METER_ADDR_SIZE);

	PLCManage->ReportDataLen = LocalFrame->Data[Index++];

	PLCManage->DataLenInDataBuf = PLCManage->ReportDataLen;
	MR((u32)PLCDataBuf, (u32)&LocalFrame->Data[Index], PLCManage->ReportDataLen);
*/
		//err20190222 ��ʱȥ��������������δӦ��ʱ���������ʱʱ����ж�OUTTIME������һ����
		//��ȷ����̨���Ƿ����Ӱ�죿
	//pAckTimer[0] = PLCManage->AMRTimeOut;//̨����� δ�ӱ�ʱ�ظ����������� ��ֹ�Զ�ͬ������

	LocalFrame->FC  &= ~(LOCAL_FRAME_DIR_BIT | LOCAL_FRAME_PRM_BIT | LOCAL_FRAME_ADR_BIT);
	LocalFrame->AFN = LOCAL_FRAME_AFN_00;		
	LocalFrame->DI  = 0xE8010001;
	LocalFrame->DataLen = 2;
	LocalFrame->Data[0] = 0x00;
	LocalFrame->Data[1] = 0x00;
	
	//UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UartTask = GetUartTaskCtrl(PORTn);
	AddrTxBuf = Get_ADDR_UARTnTx(PORTn);
	Index = BuildLocalModuleFrame((u8*)AddrTxBuf, LocalFrame);
	//UARTCtrl->TxByte = Index;
	UartTask->AckLen = Index;
	UartTask->Flag |= UART_FLAG_TX_ACK;	

	/*AMRCtrl->PLCTaskIDAckNumErr++;
	if((AMRCtrl->PLCTaskIDAckNum + AMRCtrl->PLCTaskIDAckNumErr)< AMRCtrl->PLCTaskIDAddNum) return ERROR_WRONG_LENGTH;
		//���ڳ�������δ�ϱ���������ȴ�
	pAckTimer = (u16*)GetUartPortAckTimerAddr(PORTn);
	pAckTimer[0] = 0;
	
	//AlarmAMRData->State[1] = 0x01;	//�����Ѹ���
	Result = AMR_SwitchToNextMeter(PORTn, AMRCtrl);
	if(Result == NO_ERROR)
	{
		AMRCtrl->State = AMR_Tx;
		pTerminalRouter->RouterInitTask = PLC_ADD_TASK_DONE + 1;
		SwitchMeterClearAMRInfo(AMRCtrl);
	}
	else if(Result == ERROR_NO_AMR_TASK)
	{
		AMRCtrl->State = AMR_Idle;
		AMRCtrl->AMR_ID = 0;
		WarningCancel(PORTn);
	}
	else
	{
	
	}
	*/


	return NO_ERROR;		
}

ResultEnum LocalFrame_AFN06_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	switch(LocalFrame->DI)
	{
		case 0xE8060601:
			
			break;	
		default:
			break;
	}
	return NO_ERROR;
}

ResultEnum LocalFrame_AFN07_Process(u32 PORTn, QCSGLocalFrameStruct* LocalFrame)
{
	switch(LocalFrame->DI)
	{
		case 0xE8020701:
			break;
		case 0xE8020702:
			break;
		case 0xE8000703:
			break;
		case 0xE8000704:
			break;
		case 0xE8030705:
			break;
		case 0xE8040705:
			break;
		default:
			break;
	}
	return NO_ERROR;
}




void ConvertTerminalAddrToBCD(u32 TerminalAddr, u32 TerminalLocation, u8* BCDAddr)
{
/*	u8 BCD = 0;
	BCD = (TerminalAddr /10) % 10;
	BCD <<= 8;
	BCD += TerminalAddr % 10;	
	BCDAddr[0] = BCD;

	BCD = (TerminalAddr /1000) % 10;
	BCD <<= 8;
	BCD += (TerminalAddr /100) % 10;;	
	BCDAddr[1] = BCD;

	BCD = (TerminalAddr /100000) % 10;
	BCD <<= 8;
	BCD += (TerminalAddr /10000) % 10;;	
	BCDAddr[2] = BCD;

	BCD = (TerminalAddr /10000000) % 10;
	BCD <<= 8;
	BCD += (TerminalAddr /1000000) % 10;;	
	BCDAddr[3] = BCD;

	BCDAddr[4] = (u8)TerminalLocation;
	BCDAddr[5] = (u8)(TerminalLocation >> 8);
*/
	BCDAddr[0] = TerminalAddr;
	BCDAddr[1] = TerminalAddr >> 8;
	BCDAddr[2] = TerminalAddr >> 16;

	BCDAddr[3] = TerminalLocation;
	BCDAddr[4] = TerminalLocation >> 8;
	BCDAddr[5] = TerminalLocation >> 16;
}


void DisplayRouterVer(u32 PORTn)
{
	u32 i;
	u8*p8;
	
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	i = pTerminalRouter->RouterMFCode[0];
	i += pTerminalRouter->RouterMFCode[1] << 8;
	switch((u16)i)											//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
	{
		case (('5'<<8)+'0'):
			CopyString((u8 *)"��̩�ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('T'<<8)+'C'):
			CopyString((u8 *)"�����ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('E'<<8)+'S'):
			CopyString((u8 *)"�����ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('C'<<8)+'X'):
			CopyString((u8 *)"�����ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('0'<<8)+'1'):
			CopyString((u8 *)"��˹��  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('L'<<8)+'H'):
			CopyString((u8 *)"����΢  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('N'<<8)+'X'):
			CopyString((u8 *)"Ѷ���ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('Q'<<8)+'L'):
			CopyString((u8 *)"Ǯ���ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('R'<<8)+'H'):
			CopyString((u8 *)"�����ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;			
		case (('M'<<8)+'X'):
			CopyString((u8 *)"��ϣ�ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('F'<<8)+'C'):
			CopyString((u8 *)"��Ѷ��  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('N'<<8)+'W'):
			CopyString((u8 *)"�з��ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
		case (('S'<<8)+'R'):
			CopyString((u8 *)"ɣ���ز�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			break;
			
		default:
			WarningOccur(PORTn,(u8 *)"δ֪�ز�");//�澯����,��ڸ澯����͸澯�ַ���
			return;
	}
	p8=(u8 *)(ADDR_DATABUFF+8);
	p8[0]=(pTerminalRouter->RouterVerDate[2]>>4)+0x30;
	p8[1]=(pTerminalRouter->RouterVerDate[2]&0xf)+0x30;
	p8[2]=(pTerminalRouter->RouterVerDate[1]>>4)+0x30;
	p8[3]=(pTerminalRouter->RouterVerDate[1]&0xf)+0x30;
	p8[4]=(pTerminalRouter->RouterVerDate[0]>>4)+0x30;
	p8[5]=(pTerminalRouter->RouterVerDate[0]&0xf)+0x30;
	p8[6]='V';
	i=(pTerminalRouter->RouterVer[1]>>4);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[7]=i;
	i=(pTerminalRouter->RouterVer[1]&0xf);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[8]=i;
	i=(pTerminalRouter->RouterVer[0]>>4);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[9]=i;
	i=(pTerminalRouter->RouterVer[0]&0xf);
	if(i>=10)
	{
		i+=55;
	}
	else
	{
		i+=0x30;
	}
	p8[10]=i;
	p8[11]=0;
	WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
}


ResultEnum QCSG_LocalFrameDecompress(u8* FrameBuf, u16 FrameLen, QCSGLocalFrameStruct* Frame)
{
	u16 Len = 0, Index = 0;

	if(FrameBuf == NULL) return ERROR_POINTER_NULL;
	Index++;
	Len = FrameBuf[Index++];
	Len += FrameBuf[Index++] << 8;
	
	if(FrameLen != Len) return ERROR_WRONG_LENGTH;
	if(Len > QCSG_MAX_FRAME_LEN) return ERROR_WRONG_LENGTH;

	Frame->DataLen = Len - LOCAL_FRAME_FIX_LEN;
	Frame->FC = FrameBuf[Index++];
	if((Frame->FC & LOCAL_FRAME_ADR_BIT) != 0)
	{		
		MR((u32)Frame->SrcAddr, (u32)&FrameBuf[Index], METER_ADDR_SIZE);
		Index += 6;
		Frame->DataLen -= METER_ADDR_SIZE;
		MR((u32)Frame->DstAddr, (u32)&FrameBuf[Index], METER_ADDR_SIZE);
		Index += 6;
		Frame->DataLen -= METER_ADDR_SIZE;
	}

	Frame->AFN = FrameBuf[Index++];
	Frame->SEQ = FrameBuf[Index++];	
	MR((u32)&Frame->DI, (u32)&FrameBuf[Index], 4);
	Index += 4;
	Frame->DataLen -= 6;
	
	if(Frame->DataLen > 0)
	{
		MR((u32)Frame->Data, (u32)&FrameBuf[Index], Frame->DataLen);
	}
	return NO_ERROR;
}

u32 BuildLocalModuleFrame(u8* FrameBuf, QCSGLocalFrameStruct* Frame)
{
	u32 Index = 0, CSStartPos = 0, CSLen = 0;
	FrameBuf[Index++] = LOCAL_FRAME_HEAD;
	Index += 2;

	CSStartPos = Index;
	FrameBuf[Index++] = Frame->FC;
	
	if((Frame->FC & LOCAL_FRAME_ADR_BIT) != 0)
	{
		MR((u32)&FrameBuf[Index], (u32)Frame->SrcAddr, METER_ADDR_SIZE);
		Index += METER_ADDR_SIZE;
		MR((u32)&FrameBuf[Index], (u32)Frame->DstAddr, METER_ADDR_SIZE);
		Index += METER_ADDR_SIZE;
	}
	FrameBuf[Index++] = Frame->AFN;
	FrameBuf[Index++] = Frame->SEQ;
	FrameBuf[Index++] = Frame->DI;
	FrameBuf[Index++] = Frame->DI >> 8;
	FrameBuf[Index++] = Frame->DI >> 16;
	FrameBuf[Index++] = Frame->DI >> 24;

	if(Frame->DataLen > 0)
	{
		MR((u32)&FrameBuf[Index], (u32)Frame->Data, Frame->DataLen);
		Index += Frame->DataLen;
	}

	CSLen = Index - CSStartPos;
	FrameBuf[Index++] = (u8)SumCaculation((u8*)&FrameBuf[CSStartPos], CSLen);
	FrameBuf[Index++] = LOCAL_FRAME_TAIL;

	FrameBuf[1] = Index;
	FrameBuf[2] = Index >> 8;
	return Index;
}



u16 CompareMeterAddrInNodeList(u8* MeterAddrStart, u16 MeterNum, NodeListStruct* NodeList, u16 NodeListNum)
{
	u16 i = 0, Index = 0;;
	for(i = 0; i < MeterNum; i++)
	{
		Index = FindMeterInNodeList(&MeterAddrStart[i * METER_ADDR_SIZE], NodeList, NodeListNum);
		if(Index >= NodeListNum) break;
	}
	return i;
}

u16 GetNextBPS(u16 CurBps, u16 BpsSelectBit)
{
	u32 i = 0;
	for(i = 0; i < MAX_BAUD_NUM; i++)
	{
		if(CurBps == UartBPS[i]) break;
	}
	i++;
	for(; i < MAX_BAUD_NUM; i++)
	{
		if((BpsSelectBit & (1 << i)) != 0) break;
	}
	
	if(i >= MAX_BAUD_NUM) return 0xFFFF;

	return UartBPS[i];
}



#endif
/**************************************** (C) COPYRIGHT zhaoyun **********************************END OF FILE****/

