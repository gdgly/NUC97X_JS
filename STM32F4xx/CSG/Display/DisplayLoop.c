
//�ն�ѭ��
#include "../Hcsg/ProjectCSG.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../Display/DisplayLoop_Warning.h"
#include "../Display/DisplayLoop_Info.h"
#include "../Display/DisplayLoop_ACSample.h"
#include "../Display/DisplayLoop_Contrl.h"
#include "../Display/DisplayLoop_Self.h"
#include "../Display/DisplayLoop.h"
#include "../Display/Display.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"





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


void DisplayLoop(void)//�ն�ѭ��
{
	u32 i;
	u32 Row;
//	u8 * p8rxbuff;
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
		{//ESC ȡ����
		#if ((Project/100)==2)
			TerminalDisplayLoop->Task=4;//��ǰѭ����ʾ����         //ȡ��������ʱ����I�ͼ�������ʾ�澯��Ϣ���������
		#else
			TerminalDisplayLoop->Task=0;//��ǰѭ����ʾ����
		#endif
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
//p8rxbuff[4]=0;//ͨ��ģ���ʶ��0��ʾ�Լ�������ͨ��ģ�����,1��ʾ���ز�����ͨ��ģ�����
//p8rxbuff[10]=6;//AFN
//p8rxbuff[11]=0x10;//Fn;
//p8rxbuff[12]=0;//Fn*8;
//p8rxbuff[13+3+1]=0x11;//����ַ
//p8rxbuff[13+3+2]=0x11;//����ַ
//p8rxbuff[13+3+3]=0x11;//����ַ
//p8rxbuff[13+3+4]=0x11;//����ַ
//p8rxbuff[13+3+5]=0x11;//����ַ
//p8rxbuff[13+3+6]=0x11;//����ַ
//p8rxbuff[13+3+9]=19;//����
//p8rxbuff[13+3+10]=0x01;//DI
//p8rxbuff[13+3+11]=0x15;//DI
//p8rxbuff[13+3+12]=0x00;//DI
//p8rxbuff[13+3+13]=0x04;//DI
//MC(0xFF,((u32)p8rxbuff)+13+3+14,12);//�����仯��־
//p8rxbuff[13+3+26]=0xaa;//
//MC(0x01,((u32)p8rxbuff)+13+3+27,96);//����
//Uart_3762_RxData(RS485_4PORT);//UART_3762��������;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���

//

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
				#if ((Project/100)==3)
					#if (DLT500_REPORT == 0)
						Row=DisplayLoop_Warning(Row);//�澯��Ϣ��ʾ,��������
					#endif
				#endif
				break;
			case 1://�ڲ�����������ʾ
				#if ((Project/100)==3)||((Project/100)==9)
					Row=DisplayLoop_ACSample(Row);//�ڲ�����������ʾ,��������
				#endif
				#if ((Project/100)==2)
					NWJZQDisplayDefaultInterface();			
				#endif					
				break;
			case 2://������Ϣ��ʾ
//			#if ((Project/100)==3)
//				Row=DisplayLoop_Contrl(Row);//�ն˿�����ʾ
//			#endif
				break;
			case 3://�ն���Ϣ
//			#if ((Project/100)==3)
//				Row=DisplayLoop_Info(Row);//�ն���Ϣ��ʾ,��������
//			#endif
				break;		
		#if ((Project/100)==2)
			case 4://�澯��Ϣ��ʾ
				Row=DisplayLoop_Warning(Row);//�澯��Ϣ��ʾ,��������	
				break;
		#endif
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
	
//������ʾ
//	DisplayClr();//����
//	DisplayClrRow(1,8);//����Ļ��ʾ��ָ����
	//DisplayString(7,0,0,(u8 *)(ADDR_AFN0CF4+1));//��ʾ�ַ���
	//DisplayString(8,0,0,(u8 *)(ADDR_AFN0CF4+1+16));//��ʾ�ַ���
	
//	DisplayRam(8,ADDR_BKPSDRAM);//��ʾ��RAM����(������)
//	DisplayRam(8,ADDR_F203_2+4);//��ʾ��RAM����(������)
	
//������ʾ����
}


void NWJZQDisplayDefaultInterface(void)//������������ʾĬ�Ͻ���
{
	u8 *p;
	u8 *p8;
	u32 i;
	u32 Ln = 0;
	
		
	DisplayString(3,2,0,(u8 *)"�й��Ϸ�������˾");//��ʾ�ַ���	
	//����
	p8=(u8 *)ADDR_MYMDHMS;
	p=(u8 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
	p[0*2]=0x32;
	p[1*2]=0x30;
	p[2*2]=(p8[5]>>4)+0x30;//��
	p[3*2]=(p8[5]&0xf)+0x30;
	p[4*2]='-';
	p[5*2]=(p8[4]>>4)+0x30;//��
	p[6*2]=(p8[4]&0xf)+0x30;
	p[7*2]='-';
	p[8*2]=(p8[3]>>4)+0x30;//��
	p[9*2]=(p8[3]&0xf)+0x30;
	for(i=0;i<=9;i++)
	{
		p[i*2+1]=0x20;
	}
	//ʱ��
	p=(u8 *)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
	p[0*2]=(p8[2]>>4)+0x30;//ʱ
	p[1*2]=(p8[2]&0xf)+0x30;
	p[2*2]=':';
	p[3*2]=(p8[1]>>4)+0x30;//��
	p[4*2]=(p8[1]&0xf)+0x30;
	p[5*2]=':';
	p[6*2]=(p8[0]>>4)+0x30;//��
	p[7*2]=(p8[0]&0xf)+0x30;
	for(i=0;i<=7;i++)
	{
		p[i*2+1]=0x20;
	}
}





