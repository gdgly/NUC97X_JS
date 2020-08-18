
//�ն�ѭ��
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"

#endif
#include "../Display/DisplayLoop.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayLoop_Warning.h"
#include "../Display/DisplayLoop_Info.h"
#include "../Display/DisplayLoop_ACSample.h"
#include "../Display/DisplayLoop_Self.h"
#include "../DL698/DL698_DisplayLoop_Contrl.h"

#include "../Display/Display.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




void TerminalDisplayLoopCount(void)//�ն�ѭ�Լ���
{
	if(TerminalDisplayLoop->ButtonVal&2)
	{//����
		if(TerminalDisplayLoop->Count)//��ǰѭ����ʾ��������������
		{
			TerminalDisplayLoop->Count--;
		}
		else
		{
			TerminalDisplayLoop->Count=60;
			if(TerminalDisplayLoop->Task)//��ǰѭ����ʾ����
			{
				TerminalDisplayLoop->Task--;
			}
			else
			{
				TerminalDisplayLoop->Task=3;
			}
		}
	}
	else
	{//����
		if(TerminalDisplayLoop->Count<60)
		{
			TerminalDisplayLoop->Count++;//��ǰѭ����ʾ��������������
		}
		else
		{
			TerminalDisplayLoop->Count=0;//��ǰѭ����ʾ��������������
			if(TerminalDisplayLoop->Task<3)
			{
				TerminalDisplayLoop->Task++;
			}
			else
			{
				TerminalDisplayLoop->Task=0;
			}
		}
	}
}

#include "../MS/MS.h"
void DisplayLoop(void)//�ն�ѭ��
{
	u32 i;
	u32 Row;
//	u8 *p8;
//	UARTCtrl_TypeDef * UARTCtrl;
//void Uart_3762_RxData(u32 PORTn);//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���

//ȫ�Զ�ʱ
	if(Comm_Ram->DisplayAllErrorTimer)//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
	{
		return;
	}
	if(Comm_Ram->ButtonVal&0x23)
	{
		//����
		TerminalDisplayLoop->ButtonVal=Comm_Ram->ButtonVal;
		Comm_Ram->ButtonVal&=0xdc;//ʹ�ú���0
		Comm_Ram->DisplayLoopTimer=60;//ѭ���붨ʱ��
		
		TerminalDisplayLoopCount();//�ն�ѭ�Լ���
		
		if(TerminalDisplayLoop->ButtonVal&0x1)
		{
			DOTMIXPMODE->FlashMode=1;//����ģʽ:0=��,1=����,2=����,3=����,4=����,5=...
			DOTMIXPMODE->FlashDelay=1;//�����仯��ʱ(��λ10ms),0���,255����
			DOTMIXPMODE->FlashTask=1;//��������:0=��,1=��ʼ,2=����
		}
		if(TerminalDisplayLoop->ButtonVal&0x2)
		{
			DOTMIXPMODE->FlashMode=2;//����ģʽ:0=��,1=����,2=����,3=����,4=����,5=...
			DOTMIXPMODE->FlashDelay=1;//�����仯��ʱ(��λ10ms),0���,255����
			DOTMIXPMODE->FlashTask=1;//��������:0=��,1=��ʼ,2=����
		}
		if(TerminalDisplayLoop->ButtonVal&0x20)
		{//ESC
			TerminalDisplayLoop->Task=0;//��ǰѭ����ʾ����
			TerminalDisplayLoop->Count=0;//��ǰѭ����ʾ��������������
			if(Comm_Ram->ErrorIC)
			{
				Comm_Ram->DisplayAllErrorTimer=10;//��ʾ����
			}
//---ÿ��ESC������ʱ���ò���������---
//ģ����ܱ������ϱ�
//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//UARTCtrl->Task=1;//�������
//p8rxbuff=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);		
//p8rxbuff[4]=0;//ͨ��ģ���ʶ��0��ʾ�Լ�������ͨ��ģ�����,1��ʾ���ز����ͨ��ģ�����
//p8rxbuff[10]=6;//AFN
//p8rxbuff[11]=0x10;//Fn;
//p8rxbuff[12]=0;//Fn*8;
//p8rxbuff[13+3+1]=0x11;//���ַ
//p8rxbuff[13+3+2]=0x11;//���ַ
//p8rxbuff[13+3+3]=0x11;//���ַ
//p8rxbuff[13+3+4]=0x11;//���ַ
//p8rxbuff[13+3+5]=0x11;//���ַ
//p8rxbuff[13+3+6]=0x11;//���ַ
//p8rxbuff[13+3+9]=19;//����
//p8rxbuff[13+3+10]=0x01;//DI
//p8rxbuff[13+3+11]=0x15;//DI
//p8rxbuff[13+3+12]=0x00;//DI
//p8rxbuff[13+3+13]=0x04;//DI
//MC(0xFF,((u32)p8rxbuff)+13+3+14,12);//�����仯��־
//p8rxbuff[13+3+26]=0xaa;//
//MC(0x01,((u32)p8rxbuff)+13+3+27,96);//����
//Uart_3762_RxData(RS485_4PORT);//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���



//------------------------------------
		}
	}
	else
	{
		//ѭ�Զ�ʱ
		if(Comm_Ram->DisplayLoopTimer==0)
		{
			TerminalDisplayLoop->Count++;//��ǰѭ����ʾ��������������
			TerminalDisplayLoop->ButtonVal=0;//����
			Comm_Ram->DisplayLoopTimer=8;//ѭ�Զ�ʱֵ��
			//����״̬ʱѭ��ֻ����ʾ
			if(Comm_Ram->Factory==0x55)//����״̬
			{
				TerminalDisplayLoop->Task=0;//�澯��Ϣ��ʾ
				TerminalDisplayLoop->Count=0;
				Comm_Ram->BackLightTimer=60;//ѭ�Զ�ʱֵ��
			}
		}
	}
	
	DisplayClr();//����
	//ͼ���ʱ��
	DisplayLoop_TopICO(0);//��ͼ���ʱ����ʾ
	DisplayLoop_BottomICO(9);//��ͼ����ʾ

	Row=1;
	for(i=0;i<(4*60);i++)
	{
		switch(TerminalDisplayLoop->Task)
		{//��ǰѭ����ʾ����
			case 0://�澯��Ϣ��ʾ
				Row=DisplayLoop_Warning(Row);//�澯��Ϣ��ʾ,��������
				break;
			case 1://�ڲ�����������ʾ
				Row=DisplayLoop_ACSample(Row);//�ڲ�����������ʾ,��������
				break;
			case 2://������Ϣ��ʾ
				Row=DisplayLoop_Contrl(Row);//�ն˿�����ʾ
				break;
			case 3://�ն���Ϣ
				Row=DisplayLoop_Info(Row);//�ն���Ϣ��ʾ,��������
				break;
		}
		if(Row!=1)
		{
			break;
		}
		else
		{//����ʾ
			TerminalDisplayLoopCount();//�ն�ѭ�Լ���
		}
	}

//-------������ʾ---------
//	DisplayClr();//����
//	DisplayClrRow(1,8);//����Ļ��ʾ��ָ����
	//DisplayString(7,0,0,(u8 *)(ADDR_AFN0CF4+1));//��ʾ�ַ���
	//DisplayString(8,0,0,(u8 *)(ADDR_AFN0CF4+1+16));//��ʾ�ַ���
//	MR(ADDR_DATABUFF,ADDR_04010001,6);
//	DisplayRam(7,ADDR_Terminal_Period);//��ʾ��RAM����(������)
	
//	p8=(u8*)ADDR_DATABUFF;
//	p8[0]=UART1Ctrl->Lock;
//	p8[1]=UART1Ctrl->ATTask;
//	p8[2]=UART1Ctrl->Task;
//	p8[3]=FileDownload->res3;
//	p8[4]=0;
//	p8[5]=0;
//	p8[6]=0;
//	DisplayRam(1,ADDR_IA);//��ʾ��RAM����(������)
//	DisplayRam(2,ADDR_IB);//��ʾ��RAM����(������)
//	DisplayRam(3,ADDR_IC);//��ʾ��RAM����(������)
//	DisplayRam(4,ADDR_PA);//��ʾ��RAM����(������)
//	DisplayRam(5,ADDR_PB);//��ʾ��RAM����(������)
//	DisplayRam(6,ADDR_PC);//��ʾ��RAM����(������)
//	DisplayRam(7,ADDR_PT);//��ʾ��RAM����(������)
//	DisplayRam(8,ADDR_Measure+1);//��ʾ��RAM����(������)

//DisplayRam(5,ADDR_UA);//��ʾ��RAM����(������)
//DisplayRam(6,ADDR_UB);//��ʾ��RAM����(������)
//DisplayRam(7,ADDR_UC);//��ʾ��RAM����(������)
//DisplayRam(8,ADDR_VDD5V0);//��ʾ��RAM����(������)

//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//MC(0,ADDR_DATABUFF,16);
//p8=(u8*)ADDR_DATABUFF;
//for(i=0;i<UARTCtrl->NUM_SomeTaskID;i++)
//{
//	p8[i]=UARTCtrl->SomeTaskid[i]>>8;
//}
//DisplayRam(8,ADDR_DATABUFF);//��ʾ��RAM����(������)



//-------������ʾ����------------
}







