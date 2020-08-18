

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Info_3762.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../MS/MS.h"
#include "../DL698/EVENT_Record.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"


#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

u8* UART_3762_AFNxFn(u32 AFN,u32 Fn)//��ѯ��������Ϣ,����ز�Ƶ����ŵ�(�Զ���ͣ�ͻָ�·��);���: AFN,Fn;����:���ݵ�Ԫָ��
{
	u32 i;
	u8* p8rx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
	#ifndef LINUX
	u32 *p32;
	#else
	ms_Type *ms;//�õ�ms���ַ
	#endif
	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//���У�
	//�����ݵ�Ԫ
//���У�
	//������ڵ�������n BIN 1
	//���ڵ������ʶ��  BIN 3
	//���ڵ����ڵ��ַ  BIN 6
	//�ھӽڵ�1�����ʶ��  BIN 3
	//����
	//�ھӽڵ�n�����ʶ��  BIN 3
	MC(0,(u32)p8rx,16);//���߳̿���ʹ���˷��û�����
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		if((AFN==0x03)&&(Fn==16))
		{//���Ժ�����Բ���bug,��ѯ����ز�Ƶ�����ʧ��ʱ����ֱֵ��������ֵ
			MR((u32)p8rx,ADDR_F209_9+1,1);//���� 9������ز�Ƶ����ţ��� =unsigned
		}
		return p8rx;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����

	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(RS485_4PORT);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			MC(0,(u32)p8rx,16);//���߳̿���ʹ���˷��û�����
			if((AFN==0x03)&&(Fn==16))
			{//���Ժ�����Բ���bug,��ѯ����ز�Ƶ�����ʧ��ʱ����ֱֵ��������ֵ
				MR((u32)p8rx,ADDR_F209_9+1,1);//���� 9������ز�Ƶ����ţ��� =unsigned
			}
			return p8rx;
		}
	}
	p16timer[0]=6;//6��
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4)||(UARTCtrlPLC->Task==5))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(UARTCtrlPLC->Task==5)
		{
			if(p16timer[0]==0)
			{
				break;
			}
		}
	}
	
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		Uart_3762_TxData(RS485_4PORT,AFN,Fn,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn			
				if(i==((AFN<<8)|Fn))//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					//·�ɻָ�
					Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
					Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
					return p8rx+13;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	MC(0,(u32)p8rx,16);//���߳̿���ʹ���˷��û�����
	if((AFN==0x03)&&(Fn==16))
	{//���Ժ�����Բ���bug,��ѯ����ز�Ƶ�����ʧ��ʱ����ֱֵ��������ֵ
		MR((u32)p8rx,ADDR_F209_9+1,1);//���� 9������ز�Ƶ����ţ��� =unsigned
	}
	//·�ɻָ�
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
	Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
	return p8rx;
}

void UART_3762_AFN10F21(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//��ѯ����������Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
	#ifndef LINUX
	u32 *p32;
	#else
	ms_Type *ms;//�õ�ms���ַ
	#endif
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//���У�
	//�ڵ���ʼ���	BIN	2
	//�ڵ�����n	BIN	1
//���У�
	//�ڵ�������	BIN	2
	//�ڵ���ʼ���	BIN	2
	//����Ӧ��Ľڵ�����n	BIN	1
	//�ڵ�1��ַ	BIN	6
	//�ڵ�1����ڵ���Ϣ	BIN	5
	//����	����	����
	//�ڵ�n��ַ	BIN	6
	//�ڵ�n����ڵ���Ϣ	BIN	5
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,21,3);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x1015)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=21
					i=p8rx[13+4];//����Ӧ��Ľڵ�����n	BIN	1
					i*=11;
					i+=5;
				#if ((255*11)+5)>LEN_DATABUFF
					#error "��p8Buff=ADDR_DATABUFFʱ"
				#endif
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
}

void UART_3762_AFN10F31(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//��ѯ��λ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type *ms;//�õ�ms���ַ
#endif
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//���У�
	//�ڵ���ʼ���	BIN	2
	//�ڵ�����n	BIN	1
//���У�
	//�ڵ�������	BIN	2
	//�ڵ���ʼ���	BIN	2
	//����Ӧ��Ľڵ�����n	BIN	1
	//�ڵ�1��ַ	BIN	6
	//�ڵ�1��λ��Ϣ	BIN	2
	//����	����	����
	//�ڵ�n��ַ	BIN	6
	//�ڵ�n��λ��Ϣ	BIN	2
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,31,3);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x101F)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=31
					i=p8rx[13+4];//����Ӧ��Ľڵ�����n	BIN	1
					i*=8;
					i+=5;
				#if ((255*8)+5)>LEN_DATABUFF
					#error "��p8Buff=ADDR_DATABUFFʱ"
				#endif
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
}

void UART_3762_AFN10F112(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//��ѯ����ز�оƬ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//���У�
	//�ڵ���ʼ���	BIN	2
	//�ڵ�����n	BIN	1
//���У�
//�ڵ������� BIN 2
//�ڵ���ʼ��� BIN 2
//����Ӧ��Ľڵ�����n BIN 1
//�ڵ�1��ַ BIN 6
//�ڵ�1�豸���� BIN 1
//�ڵ�1оƬID��Ϣ BIN 24
//�ڵ�1оƬ����汾��Ϣ BCD 2
//...
//�ڵ�n��ַ BIN 6
//�ڵ�n�豸���� BIN 1
//�ڵ�nоƬID��Ϣ BIN 24
//�ڵ�nоƬ����汾��Ϣ BCD 2
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,112,3);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x1070)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=112
					i=p8rx[13+4];//����Ӧ��Ľڵ�����n	BIN	1
					i*=33;
					i+=5;
					//#error "��p8Buff=ADDR_DATABUFFʱ"
					if(((u32)p8Buff>=ADDR_DATABUFF)&&((u32)p8Buff<(ADDR_DATABUFF+LEN_DATABUFF)))
					{
						if(i>((ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff))
						{
							i=(ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff;
						}
					}
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
}


void UART_3762_AFN10F104(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//��ѯ����ز�оƬ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//���У�
	//�ڵ���ʼ���	BIN	2
	//�ڵ�����n	BIN	1
//���У�
//�ڵ������� BIN 2
//�ڵ���ʼ��� BIN 2
//����Ӧ��Ľڵ�����n BIN 1
//�ڵ�1��ַ BIN 6
//�ڵ�1�豸���� BIN 1
//�ڵ�1оƬID��Ϣ BIN 24
//�ڵ�1оƬ����汾��Ϣ BCD 2
//...
//�ڵ�n��ַ BIN 6
//�ڵ�n�豸���� BIN 1
//�ڵ�nоƬID��Ϣ BIN 24
//�ڵ�nоƬ����汾��Ϣ BCD 2
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,104,3);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x1068)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=104
					i=p8rx[13+2];//����Ӧ��Ľڵ�����n	BIN	1
					i*=15;
					i+=3;
					//#error "��p8Buff=ADDR_DATABUFFʱ"
					if(((u32)p8Buff>=ADDR_DATABUFF)&&((u32)p8Buff<(ADDR_DATABUFF+LEN_DATABUFF)))
					{
						if(i>((ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff))
						{
							i=(ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff;
						}
					}
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
}

void UART_3762_AFN5F16(u32 PORTn,u8* data)//��������ʶ�𿪹� 0���ر� 1����
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		p8tx[128]=data[0];	// ����HPLCͨ��Ƶ��
		Uart_3762_TxData(RS485_4PORT,0x05,16,1);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)//ȷ��֡
				{
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
}



void UART_3762_AFN5F200(u32 PORTn,u8* data)//�ܾ��ڵ��ϱ�ʹ�ܱ�־����ֹ��0����ʹ�ܣ�1��
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=10;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		ms->link_printf("A-%x\n",ReNum,0,0);
		p8tx[128]=data[0];	// ����/��ֹ�ܾ��ڵ���Ϣ�ϱ�
		Uart_3762_TxData(RS485_4PORT,0x05,200,1);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)//ȷ��֡
				{
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
}
void UART_3762_AFN5F6(u32 PORTn,u32 EnableFlag)//��������ʶ�𿪹� 0���ر� 1����
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
		#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
		#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
		#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		if((EnableFlag&0x01) == 1)
			p8tx[128]=0x01;	// ʹ��̨��ʶ��
		else
			p8tx[128]=0;	// �ر�̨��ʶ��
		Uart_3762_TxData(RS485_4PORT,0x05,6,1);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0001)//ȷ��֡
				{
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
}

void UART_3762_AFN10F7(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//��ѯ����ز�оƬ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
{
	u32 i;
	u8  n;
	u16 Len;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif	
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//���У�
	//�ڵ���ʼ���	BIN	2
	//�ڵ�����n	BIN	1
//���У�
//�ڵ������� BIN 2
//����Ӧ��Ľڵ�����n BIN 1

//�ӽڵ�1��ַ	BCD 		     6 
//�ӽڵ�1�ڵ�����BS				 1 
//�ӽڵ�1ģ�鳧�̴���ASCII 			 2 
//�ӽڵ�1�ڵ�ģ��ID�ų���BIN			 1 
//�ӽڵ�1�ڵ�ģ��ID�Ÿ�ʽBIN			 1 
//�ӽڵ�1�ڵ�ģ��ID��				 50
//...
//�ӽڵ�N��ַ	BCD 		     6 
//�ӽڵ�N�ڵ�����BS				 1 
//�ӽڵ�Nģ�鳧�̴���ASCII 			 2 
//�ӽڵ�N�ڵ�ģ��ID�ų���BIN			 1 
//�ӽڵ�N�ڵ�ģ��ID�Ÿ�ʽBIN			 1 
//�ӽڵ�N�ڵ�ģ��ID��				 50

	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
#ifndef LINUX
			p32=(u32*)ADDR_MS_ENTRY;
			(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
#else
			ms=(ms_Type *)Get_ms();
			ms->Main_Thread();
#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x10,7,3);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x1007)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=7
					n=p8rx[13+2];//����Ӧ��Ľڵ�����n	BIN	1 �䳤���ݼ�����
					i=0;
					i+=3;
					while(n)
					{
						Len=p8rx[i+13+9];
						i+=(Len+11);
						n-=1;
					}
					//#error "��p8Buff=ADDR_DATABUFFʱ"
					if(((u32)p8Buff>=ADDR_DATABUFF)&&((u32)p8Buff<(ADDR_DATABUFF+LEN_DATABUFF)))
					{
						if(i>((ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff))
						{
							i=(ADDR_DATABUFF+LEN_DATABUFF)-(u32)p8Buff;
						}
					}
					MR((u32)p8Buff,(u32)(p8rx+13),i);
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
}


void READ_AFN3F12_TO_ADDR_AFN3F12(void)//��ѯ�������ڵ�ͨ��ģ��ID��Ϣ
{
	u32 n;	
	u8* p8;
	u8* p8s;
	u32 offset;
	u8* p8sAFN3F12;
	u8	pOccurSour[5];		  // ����¼�����Դ    OAD
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN3F12******************\n", 0, 0, 0);
	p8=UART_3762_AFNxFn(0x03,12);//��ѯ�������ڵ�ͨ��ģ�� ID ��Ϣ
	//ģ�鳧�̴��� ASCII 2
	//ģ�� ID ���� BIN 1
	//ģ�� ID ��ʽ BIN 1
	//ģ�� ID ��Ӧģ�� ID ��ʽ M
	n=p8[2];
	p8sAFN3F12=(u8*)ADDR_AFN3F12;
	if(IsAllAByte(p8sAFN3F12, 0, 4)==0)
	{
		if(Compare_DL698DataString(p8sAFN3F12+4, p8+4, n))//�����ȡ�ı������ڵ�ͨ��ģ��ID��Ϣ�ı��ˣ�����ģ��ID�ű���¼��е��ز�ģ����
		{
			p8s=((u8*)ADDR_DATABUFF + 3072);
			//ģ���Ӧ��ͨ�ŵ�ַ octet-string
			offset = 0;
			p8s[offset]=DataType_octet_string;
			p8s[offset+1]=6;
			MR((u32)p8s+offset+2,ADDR_4001+2,6);

			offset+=8;
			//���ǰ��ģ�������� visible-string�� 
			p8s[offset++]=DataType_visible_string;
			p8s[offset++]= (105 + 2*n); //128 �ֽ�ģ��ID��Ϣ
			
			//	�ӽڵ�������	  visible-string
			//type=
			p8s[offset++] = 0x74;
			p8s[offset++] = 0x79;
			p8s[offset++] = 0x70;
			p8s[offset++] = 0x65;
			p8s[offset++] = 0x3d;
	
			//ģ������ PLCд��
			p8s[offset++] = 0x50;
			p8s[offset++] = 0x4c;
			p8s[offset++] = 0x43;
	
			// ;
			p8s[offset++] = 0x3b;
	
			//mfrs=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x73;
			p8s[offset++] = 0x3d;
			
			// оƬ���̴���
			p8s[offset++] = p8sAFN3F12[1];
			p8s[offset++] = p8sAFN3F12[0];
	
			//;
			p8s[offset++] = 0x3b; 
			
			//idformat=
			p8s[offset++] = 0x69; 
			p8s[offset++] = 0x64; 
			p8s[offset++] = 0x66; 
			p8s[offset++] = 0x6f; 
			p8s[offset++] = 0x72; 
			p8s[offset++] = 0x6d; 
			p8s[offset++] = 0x61; 
			p8s[offset++] = 0x74; 
			p8s[offset++] = 0x3d; 
	
			// bcd bin ascll 
			p8s[offset++] = 0x32;
	
			// ��
			p8s[offset++] = 0x3b;
	
			// id =
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x3d;
	
			Data_Inverse((u32)ADDR_AFN10F112_old+9,24);
			HexToASCII((u8*)ADDR_AFN10F112_old+9,&p8s[offset],24);
			Data_Inverse((u32)ADDR_AFN10F112_old+9,24); // RAM������Ҫ��λ
			offset+=48;
			
			// ��
			p8s[offset++] = 0x3b;	
	
			// mmfrs=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x73;
			p8s[offset++] = 0x3d;
	
			//	ģ�鳧�̴���
			p8s[offset++] = p8sAFN3F12[1];
			p8s[offset++] = p8sAFN3F12[0];
	
			// ��
			p8s[offset++] = 0x3b;
	
			// midformat=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x6f;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x61;
			p8s[offset++] = 0x74;
			p8s[offset++] = 0x3d;
	
			// 02
			p8s[offset++] = 0x31;
	
			// ;
			p8s[offset++] = 0x3b;
			
			//mid =
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x3d;
	
			// ģ��ID��Ϣ		
			Data_Inverse((u32)p8sAFN3F12+4,n);
			HexToASCII(p8sAFN3F12+4,&p8s[offset],n);
			Data_Inverse((u32)p8sAFN3F12+4,n);	// RAM������Ҫ��λ
			offset+=(2*n);
	
	//������ģ�������� visible-string
			p8s[offset++]=DataType_visible_string;
			p8s[offset++]= (105 + 2*n); //128 �ֽ�ģ��ID��Ϣ
	
			
			//	�ӽڵ�������	  visible-string
			//type=
			p8s[offset++] = 0x74;
			p8s[offset++] = 0x79;
			p8s[offset++] = 0x70;
			p8s[offset++] = 0x65;
			p8s[offset++] = 0x3d;
	
			//ģ������ PLCд��
			p8s[offset++] = 0x50;
			p8s[offset++] = 0x4c;
			p8s[offset++] = 0x43;
	
			// ;
			p8s[offset++] = 0x3b;
	
			//mfrs=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x73;
			p8s[offset++] = 0x3d;
			
			// оƬ���̴���
			p8s[offset++] = p8[1];
			p8s[offset++] = p8[0];
	
			//;
			p8s[offset++] = 0x3b; 
			
			//idformat=
			p8s[offset++] = 0x69; 
			p8s[offset++] = 0x64; 
			p8s[offset++] = 0x66; 
			p8s[offset++] = 0x6f; 
			p8s[offset++] = 0x72; 
			p8s[offset++] = 0x6d; 
			p8s[offset++] = 0x61; 
			p8s[offset++] = 0x74; 
			p8s[offset++] = 0x3d; 
	
			// bcd bin ascll 
			p8s[offset++] = 0x32;
	
			// ��
			p8s[offset++] = 0x3b;
	
			// id =
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x3d;
	
			Data_Inverse((u32)ADDR_AFN10F112+9,24);
			HexToASCII((u8*)ADDR_AFN10F112+9,&p8s[offset],24);
			Data_Inverse((u32)ADDR_AFN10F112+9,24); // RAM������Ҫ��λ
			offset+=48;
			
			// ��
			p8s[offset++] = 0x3b;	
	
			// mmfrs=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x73;
			p8s[offset++] = 0x3d;
	
			//	ģ�鳧�̴���
			p8s[offset++] = p8[1];
			p8s[offset++] = p8[0];
	
			// ��
			p8s[offset++] = 0x3b;
	
			// midformat=
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x66;
			p8s[offset++] = 0x6f;
			p8s[offset++] = 0x72;
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x61;
			p8s[offset++] = 0x74;
			p8s[offset++] = 0x3d;
	
			// 02
			p8s[offset++] = 0x31;
	
			// ;
			p8s[offset++] = 0x3b;
			
			//mid =
			p8s[offset++] = 0x6d;
			p8s[offset++] = 0x69;
			p8s[offset++] = 0x64;
			p8s[offset++] = 0x3d;
	
			// ģ��ID��Ϣ		
			Data_Inverse((u32)p8+4,n);
			HexToASCII(p8+4,&p8s[offset],n);
			Data_Inverse((u32)p8+4,n);	// RAM������Ҫ��λ
			offset+=(2*n);
			
			pOccurSour[0]=DataType_OAD;
			pOccurSour[1]=0xF2;
			pOccurSour[2]=0x09;
			pOccurSour[3]=0x02;
			pOccurSour[4]=0x01;
			Event_30300200(p8s,pOccurSour,offset);//��̨�����ܱ��¼���¼;���:pDATA=һ����̨�����;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			ms->link_printf("\n********************************************\n", 0, 0, 0);
			ms->link_printf("***************CcoIdChang***********\n", 0, 0, 0);
			ms->link_printf("**********************************************\n", 0, 0, 0);
		}
	}
	MR(ADDR_AFN3F12,(u32)p8,4+n);
}
void READ_AFN10F21_TO_ADDR_AFN10F21(void)//��ѯȫ������������Ϣ(�Զ���ͣ�ͻָ�·��);����:������ERAM��ADDR_AFN10F21
{
	u32 n;
	u32 m;
	u32 NUMmax;
	u8* p8s;
	u8* p8d;
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F21******************\n", 0, 0, 0);
//2byte �ڵ�������
//6byte �ڵ�1��ַ
//5byte �ڵ�1����������Ϣ
//...
//6byte �ڵ�n��ַ
//5byte �ڵ�n����������Ϣ
	n=0;
	p8d=(u8*)ADDR_AFN10F21+2;
	while(1)
	{
		UART_3762_AFN10F21(ACSAMPLEPORT,n+1,32,(u8*)ADDR_DATABUFF);//��ѯ����������Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
		p8s=(u8*)ADDR_DATABUFF;
	//���У�
		//�ڵ�������	BIN	2
		//�ڵ���ʼ���	BIN	2
		//����Ӧ��Ľڵ�����n	BIN	1
		//�ڵ�1��ַ	BIN	6
		//�ڵ�1����ڵ���Ϣ	BIN	5
		//����	����	����
		//�ڵ�n��ַ	BIN	6
		//�ڵ�n����ڵ���Ϣ	BIN	5
		NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
		m=p8s[4];//����Ӧ��Ľڵ�����
		if(m==0)
		{
			break;
		}
		p8s+=5;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}
		MR((u32)p8d+(11*n),(u32)p8s,11*m);
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}
	p8d=(u8*)ADDR_AFN10F21;
	p8d[0]=n;
	p8d[1]=n>>8;
	//·�ɻָ�
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
	Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
}

void READ_AFN10F31_TO_ADDR_AFN10F31(void)//��ѯ��λ��Ϣ
{
	u32 n;
	u32 m;
	u32 NUMmax;
	u8* p8s;
	u8* p8d;
	
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F31******************\n", 0, 0, 0);

//���У�
	//�ڵ���ʼ���	BIN	2
	//�ڵ�����n	BIN	1
//���У�
	//�ڵ�������	BIN	2
	//�ڵ���ʼ���	BIN	2
	//����Ӧ��Ľڵ�����n	BIN	1
	//�ڵ�1��ַ	BIN	6
	//�ڵ�1��λ��Ϣ	BIN	2
	//����	����	����
	//�ڵ�n��ַ	BIN	6
	//�ڵ�n��λ��Ϣ	BIN	2
	n=0;
	p8d=(u8*)ADDR_AFN10F31+2;
	while(1)
	{
		UART_3762_AFN10F31(ACSAMPLEPORT,n+1,15,(u8*)ADDR_DATABUFF);//��ѯ����ز�оƬ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
		p8s=(u8*)ADDR_DATABUFF;

		NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
		m=p8s[4];//����Ӧ��Ľڵ�����
		if(m==0)
		{
			break;
		}
		p8s+=5;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}
		MR((u32)p8d+(8*n),(u32)p8s,8*m);
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}
	p8d=(u8*)ADDR_AFN10F31;
	p8d[0]=n;
	p8d[1]=n>>8;
	//·�ɻָ�
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
	Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
}


void READ_AFN10F111_TO_ADDR_AFN10F111(void)//��ѯȫ����������Ϣ(�Զ���ͣ�ͻָ�·��);����:������ERAM��ADDR_AFN10F111
{
	u32 n;
	u8* p8s;
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F101******************\n", 0, 0, 0);
	p8s=UART_3762_AFNxFn(0x10,111);//��ѯ��������Ϣ,����ز�Ƶ����ŵ�(�Զ���ͣ�ͻָ�·��);���: AFN,Fn;����:���ݵ�Ԫָ��
	//������ڵ�������n BIN 1
	//���ڵ������ʶ��  BIN 3
	//���ڵ����ڵ��ַ  BIN 6
	//�ھӽڵ�1�����ʶ��  BIN 3
	//����
	//�ھӽڵ�n�����ʶ��  BIN 3
	n=p8s[0];
	n*=3;
	n+=10;
	MR(ADDR_AFN10F111,(u32)p8s,n);
}

void READ_AFN10F112_TO_ADDR_AFN10F112(void)//��ѯȫ������ز�оƬ��Ϣ(�Զ���ͣ�ͻָ�·��);����:������ERAM��ADDR_AFN10F112
{
	u32 n;
	u32 m;
	u32 NUMmax;
	u8* p8s;
	u8* p8d;
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F112******************\n", 0, 0, 0);

//�ڵ������� BIN 2
//�ڵ�1��ַ BIN 6
//�ڵ�1�豸���� BIN 1
//�ڵ�1оƬID��Ϣ BIN 24
//�ڵ�1оƬ����汾��Ϣ BIN 2
//...
//�ڵ�n��ַ BIN 6
//�ڵ�n�豸���� BIN 1
//�ڵ�nоƬID��Ϣ BIN 24
//�ڵ�nоƬ����汾��Ϣ BIN 2
	n=0;
	p8d=(u8*)ADDR_AFN10F112+2;
	MR(ADDR_AFN10F112_old,ADDR_AFN10F112, 2 + (33 * NUM_RMmax));
	while(1)
	{
		UART_3762_AFN10F112(ACSAMPLEPORT,n+1,32,(u8*)ADDR_DATABUFF);//��ѯ����ز�оƬ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
		p8s=(u8*)ADDR_DATABUFF;
	//���У�
		//�ڵ������� BIN 2
		//�ڵ���ʼ��� BIN 2
		//����Ӧ��Ľڵ�����n BIN 1
		//�ڵ�1��ַ BIN 6
		//�ڵ�1�豸���� BIN 1
		//�ڵ�1оƬID��Ϣ BIN 24
		//�ڵ�1оƬ����汾��Ϣ BCD 2
		NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
		m=p8s[4];//����Ӧ��Ľڵ�����
		if(m==0)
		{
			break;
		}
		p8s+=5;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}
		MR((u32)p8d+(33*n),(u32)p8s,33*m);
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}
	p8d=(u8*)ADDR_AFN10F112;
	p8d[0]=n;
	p8d[1]=n>>8;
	
	myprintf("======================ADDR_AFN10F112-S==========\n",0,0,0);
	for(n=0;n<100;n++)
		myprintf("%02x ", p8d[n], 0, 0);
	myprintf("\n",0,0,0);
	myprintf("======================ADDR_AFN10F112-E==========\n",0,0,0);
	//·�ɻָ�
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
	Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
}

void READ_AFN10F104_TO_ADDR_AFN10F104(void)//��ѯȫ������ز�оƬ��Ϣ(�Զ���ͣ�ͻָ�·��);����:������ERAM��ADDR_AFN10F112
{
	u32 n;
	u32 m;
	u32 NUMmax;
	u8* p8s;
	u8* p8d;

	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf("\n*****************AFN10F104******************\n", 0, 0, 0);

	//�ڵ������� BIN 2
	//�ڵ�1��ַ BIN 6
	//�ڵ�1����汾 BIN 2
	//�ڵ�1����汾���� BIN 3
	//�ڵ�1ģ�鳧�̴��� BIN 2
	//�ڵ�1оƬ���� BIN 2
	//...
	//�ڵ�n��ַ BIN 6
	//�ڵ�n����汾 BIN 2
	//�ڵ�n����汾���� BIN 3
	//�ڵ�nģ�鳧�̴��� BIN 2
	//�ڵ�nоƬ���� BIN 2

	n=0;
	
	MR(ADDR_AFN10F104_old, ADDR_AFN10F104,2 + (15 * NUM_RMmax));	// ��������ǰ��ģ��ID��Ϣ�������¼�����
	p8d=(u8*)ADDR_AFN10F104 + 2;
	while(1)
	{
		UART_3762_AFN10F104(ACSAMPLEPORT,n+1,15,(u8*)ADDR_DATABUFF);//��ѯ����ز�оƬ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
		p8s=(u8*)ADDR_DATABUFF;
		NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
		m=p8s[2];//����Ӧ��Ľڵ�����
		if(m==0)
		{
			break;
		}
		p8s+=3;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}
		MR((u32)p8d+(15*n),(u32)p8s,15*m);
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}
	p8d=(u8*)ADDR_AFN10F104;
	p8d[0]=n;
	p8d[1]=n>>8;
	//·�ɻָ�
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
	Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
}

void READ_AFN10F7_TO_ADDR_AFN10F7(void)//��ѯȫ������ز�оƬ��Ϣ(�Զ���ͣ�ͻָ�·��);����:������ERAM��ADDR_AFN10F112
{
	u32 n;
	u32 m;
	u32 M;
	u32 k;
	u32 w,x,y,z;
	u32 Minus;
	u32 NUMmax;
	u32 addr;
	u32 Len;
	u32 i,j;
	u32 offset;
	u8	pOccurSour[5];		  // ����¼�����Դ    OAD
	u8* p8s;
	u8* p8s1;
	u8* p8s2;
	u8* p8s3;
	u8* p8s4;
	u8* p8s5;
	u8* p8d;
	ms_Type *ms;
	ms=Get_ms();

	
	ms->link_printf("\n*****************AFN10F7******************\n", 0, 0, 0);

	//�ڵ������� BIN					 2
	
	//�ӽڵ�1��ַ  BCD 				 6 
	//�ӽڵ�1�ڵ�����BS  				 1 
	//�ӽڵ�1ģ�鳧�̴���ASCII  			 2 
	//�ӽڵ�1�ڵ�ģ��ID�ų���BIN  		 1 
	//�ӽڵ�1�ڵ�ģ��ID�Ÿ�ʽBIN       	 1 
	//�ӽڵ�1�ڵ�ģ��ID��				 50

	//...
	//�ӽڵ�N��ַ  BCD 				 6 
	//�ӽڵ�N�ڵ�����BS  				 1 
	//�ӽڵ�Nģ�鳧�̴���ASCII  			 2 
	//�ӽڵ�N�ڵ�ģ��ID�ų���BIN  		 1 
	//�ӽڵ�N�ڵ�ģ��ID�Ÿ�ʽBIN       	 1 
	//�ӽڵ�N�ڵ�ģ��ID��				 50
	
	n=0;
	p8d=(u8*)ADDR_AFN10F7 + 2;
	MR(ADDR_AFN10F7_old,ADDR_AFN10F7,2 + (61 * NUM_RMmax));
	p8s1=(u8*)ADDR_AFN10F7_old + 2;
	while(1)
	{
		MR(ADDR_DATABUFF + 1024 + 512,(ADDR_AFN10F7 + 2 + n* 61),  15 * 61);	// ��������ǰ��ģ��ID��Ϣ�������¼�����
		UART_3762_AFN10F7(ACSAMPLEPORT,n+1,15,(u8*)ADDR_DATABUFF);//��ѯ����ز�оƬ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
		p8s=(u8*)ADDR_DATABUFF;
	//���У�
		//�ڵ������� BIN					 2
		//����Ӧ�������					 1
		//�ӽڵ�1��ַ	BCD 			 6 
		//�ӽڵ�1�ڵ�����BS				 1 
		//�ӽڵ�1ģ�鳧�̴���ASCII 			 2 
		//�ӽڵ�1�ڵ�ģ��ID�ų���BIN			 1 
		//�ӽڵ�1�ڵ�ģ��ID�Ÿ�ʽBIN			 1 
		//�ӽڵ�1�ڵ�ģ��ID��				 M(<=50)

		NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
		m=p8s[2];//����Ӧ��Ľڵ�����
		if(m==0)
		{
			break;
		}
		p8s+=3;
		if(((n+m)>NUMmax)||((n+m)>NUM_RMmax))
		{
			break;
		}

		// ��һ��
		k = m;
		addr=0;
		if( k == 1)
		{
			MR((u32)p8d+(61*n),(u32)p8s,61);	// �ӽڵ㿪ʼ 
		}
		else
		{
			while(k)
			{
				Len=p8s[addr+9];
				Minus = (m-k);
				MR((u32)(p8d+(61*(n + Minus))),(u32)(p8s + addr),61);
				addr+=(Len+11);
				k-=1;
				if(k < 1)
				{
					break;
				}
			} 
		}
		n+=m;
		if((n>=NUMmax)||(n>=NUM_RMmax))
		{
			break;
		}
	}

#if 1		
	// ���ݳ�ʼ����һ�� ��Ӧ�ò���ģ��ID����¼�
	if(IsAllAByte(p8s1, 0, 9) == 0)
	{
		// ����ģ��ID����¼�
		for(i=0; i<n; i++)
		{
			// ���ǰģ��ID��̬�䳤����
			M = p8s1[(61 * i) + 9];
			if(M>50)
			{
				continue;
			}

			// û�г���ģ��ID��Ϣ���ݣ�Ҳ�������¼�
			if(IsAllAByte(p8s1 + 61 * i + 11, 0, M) == 1)
			{
				continue;
			}

			// û�г���ģ��ID��Ϣ���ݣ�Ҳ�������¼�
			if(IsAllAByte(p8d + 61 * i + 11, 0, M) == 1)
			{
				continue;
			}

			// ��ַȫ0����
			if(IsAllAByte(p8s1 + (61 * i), 0, 6) == 1)
			{
				continue;
			}

			// ��ַȫ0����
			if(IsAllAByte(p8d + (61 * i), 0, 6) == 1)
			{
				continue;
			}
#if 1		
			// �ն������б��ǰ�õ������
			for(j=0;j<NUM_RMmax;j++)
			{
				if(Compare_DL698DataString(((u8*)ADDR_NoteList+(j * LEN_NoteList)),p8s1 + (61 * i), 6) == 0)
				{
					break;
				}
			}
			if(j == NUM_RMmax)
			{
				continue;
			}	

			// �ն������б����õ������
			for(j=0;j<NUM_RMmax;j++)
			{
				if(Compare_DL698DataString(((u8*)ADDR_NoteList+(j * LEN_NoteList)),p8d + (61 * i), 6) == 0)
				{
					break;
				}
			}
			if(j == NUM_RMmax)
			{
				continue;
			}
#endif

			// �ҵ����ǰ����ַ ��Ӧ��ģ��ID�洢λ�� 
			for(x=0;x<NUM_RMmax;x++)
			{
				if(Compare_DL698DataString(p8d + 61 * i ,p8s1+61*x, 6) == 0)
				{
					break;
				}
			}

			if(x == NUM_RMmax)
			{
				continue;
			}
			
			// ģ��ID�������
			if(Compare_DL698DataString(p8d + (61 * i), p8s1 + (61 * x), 11 + M))
			{
				//ģ���Ӧ��ͨ�ŵ�ַ octet-string
				p8s=(u8*)ADDR_DATABUFF+1024;
				offset = 0;
				p8s[offset]=DataType_octet_string;
				p8s[offset+1]=6;
				MR((u32)p8s+offset+2,(u32)p8d+(61 * i),6);
				p8s2=(u8*)ADDR_AFN10F112;
				p8s3= (u8*)ADDR_AFN10F104_old;
				p8s4=(u8*)ADDR_AFN10F112_old;
				p8s5= (u8*)ADDR_AFN10F104;
				for(k=0;k<NUM_RMmax;k++)
				{
					if(Compare_DL698DataString(p8s2 + 2 + 33 * k ,p8s+offset+2, 6) == 0)
					{
						break;
					}
				}
				for(z=0;z<NUM_RMmax;z++)
				{
					if(Compare_DL698DataString(p8s4 + 2 + 33 * z ,p8s+offset+2, 6) == 0)
					{
						break;
					}
				}
				
				for(y=0;y<NUM_RMmax;y++)
				{
					if(Compare_DL698DataString(p8s3 + 2 + 15 * y ,p8s+offset+2, 6) == 0)
					{
						break;
					}
				}

				for(w=0;w<NUM_RMmax;w++)
				{
					if(Compare_DL698DataString(p8s5 + 2 + 15 * w ,p8s+offset+2, 6) == 0)
					{
						break;
					}
				}

				
				Data_Inverse((u32)p8s+offset+2,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				offset+=8;
	
				//���ǰ��ģ�������� visible-string�� 
				p8s[offset++]=DataType_visible_string;
				p8s[offset++]= (103 + 2*M); //128 �ֽ�ģ��ID��Ϣ
				
				//	�ӽڵ�������	  visible-string
				//type=
				p8s[offset++] = 0x74;
				p8s[offset++] = 0x79;
				p8s[offset++] = 0x70;
				p8s[offset++] = 0x65;
				p8s[offset++] = 0x3d;
	
				
				//HexToASCII(p8s4 + (33 * z) + 6 + 2,&p8s[offset],1);
				p8s[offset++] = p8s4[(33 * z) + 6 + 2] + 0x30;
	
				// ;
				p8s[offset++] = 0x3b;
	
				//mfrs=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x73;
				p8s[offset++] = 0x3d;
				
				// оƬ���̴���
				p8s[offset++] = p8s3[14+ 2 + 15*y];
				p8s[offset++] = p8s3[13+ 2 + 15*y];
	
				//;
				p8s[offset++] = 0x3b; 
				
				//idformat=
				p8s[offset++] = 0x69; 
				p8s[offset++] = 0x64; 
				p8s[offset++] = 0x66; 
				p8s[offset++] = 0x6f; 
				p8s[offset++] = 0x72; 
				p8s[offset++] = 0x6d; 
				p8s[offset++] = 0x61; 
				p8s[offset++] = 0x74; 
				p8s[offset++] = 0x3d; 
	
				// bcd bin ascll 
				p8s[offset++] = 0x32;
	
				// ��
				p8s[offset++] = 0x3b;
	
				// id =
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x3d;

				Data_Inverse((u32)p8s4 + (33 * z) + 7 + 2,24);
				HexToASCII(p8s4 + (33 * z) + 7 + 2,&p8s[offset],24);
				Data_Inverse((u32)p8s4 + (33 * z) + 7 + 2,24);	// RAM������Ҫ��λ
				offset+=48;
				
				// ��
				p8s[offset++] = 0x3b;	
	
				// mmfrs=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x73;
				p8s[offset++] = 0x3d;
	
				//	ģ�鳧�̴���
				p8s[offset++]= p8s3[12+ 2 + 15*y];
				p8s[offset++]= p8s3[11+ 2 + 15*y];
	
				// ��
				p8s[offset++] = 0x3b;
	
				// midformat=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x6f;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x61;
				p8s[offset++] = 0x74;
				p8s[offset++] = 0x3d;
	
				// 02
				p8s[offset++] = 0x31;
	
				// ;
				p8s[offset++] = 0x3b;
				
				//mid =
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x3d;
	
				// ģ��ID��Ϣ		
				Data_Inverse((u32)p8s1+11+x*61,M);
				HexToASCII(&p8s1[11+x*61],&p8s[offset],M);
				Data_Inverse((u32)p8s1+11+x*61,M);	// RAM������Ҫ��λ
				offset+=(2*M);
	
		//������ģ�������� visible-string
				// ģ��ID��̬�䳤����
				M = p8d[(61 * i) + 9];
				if(M>50)
				{
					M=50;
				}
				p8s[offset++]=DataType_visible_string;
				p8s[offset++]= (103 + 2*M); //128 �ֽ�ģ��ID��Ϣ
	
				
				//128 �ֽ�ģ��ID��Ϣ
				//128 �ֽ�ģ��ID��Ϣ
				//type=
				p8s[offset++] = 0x74;
				p8s[offset++] = 0x79;
				p8s[offset++] = 0x70;
				p8s[offset++] = 0x65;
				p8s[offset++] = 0x3d;
	
				//HexToASCII(&p8s2[2] + (33 * k) + 6,&p8s[offset],1);
				//offset+=2;
				p8s[offset++] = p8s2[2 + (33 * k) + 6]+0x30;
	
				// ;
				p8s[offset++] = 0x3b;
	
				//mfrs=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x73;
				p8s[offset++] = 0x3d;
				
				// оƬ���̴���
				p8s[offset++] = p8s5[14+ 2 + 15*w];
				p8s[offset++] = p8s5[13+ 2 + 15*w];

				//;
				p8s[offset++] = 0x3b; 
				
				//idformat=
				p8s[offset++] = 0x69; 
				p8s[offset++] = 0x64; 
				p8s[offset++] = 0x66; 
				p8s[offset++] = 0x6f; 
				p8s[offset++] = 0x72; 
				p8s[offset++] = 0x6d; 
				p8s[offset++] = 0x61; 
				p8s[offset++] = 0x74; 
				p8s[offset++] = 0x3d; 
	
				// bcd bin ascll
				p8s[offset++] = 0x32;
	
				// ��
				p8s[offset++] = 0x3b;
	
				// id =
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x3d;
				Data_Inverse((u32)p8s2 + 2 + (33 * k) + 7,24);
				HexToASCII(p8s2 + 2 + (33 * k) + 7,&p8s[offset],24);
				Data_Inverse((u32)p8s2 + 2 + (33 * k) + 7,24);
				
				offset+=48;
				
				// ��
				p8s[offset++] = 0x3b;	
	
				// mmfrs=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x73;
				p8s[offset++] = 0x3d;
	
				//	ģ�鳧�̴���
				p8s[offset++]= p8s5[12+ 2 + 15*w];
				p8s[offset++]= p8s5[11+ 2 + 15*w];

	
				// ��
				p8s[offset++] = 0x3b;
	
				// midformat=
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x66;
				p8s[offset++] = 0x6f;
				p8s[offset++] = 0x72;
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x61;
				p8s[offset++] = 0x74;
				p8s[offset++] = 0x3d;
	
				// 02
				p8s[offset++] = 0x31;
	
				// ;
				p8s[offset++] = 0x3b;
				
				//mid =
				p8s[offset++] = 0x6d;
				p8s[offset++] = 0x69;
				p8s[offset++] = 0x64;
				p8s[offset++] = 0x3d;
	
				// ģ��ID��Ϣ		
				Data_Inverse((u32)p8d+11+i*61,M);
				HexToASCII(&p8d[11+i*61],&p8s[offset],M);
				Data_Inverse((u32)p8d+11+i*61,M);
				offset+=(2*M);
				pOccurSour[0]=DataType_OAD;
				pOccurSour[1]=0xF2;
				pOccurSour[2]=0x09;
				pOccurSour[3]=0x05;
				pOccurSour[4]=0x00;
				Event_30300200(p8s,pOccurSour,offset);//��̨�����ܱ��¼���¼;���:pDATA=һ����̨�����;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				ms->link_printf("\n********************************************\n", 0, 0, 0);
				ms->link_printf("*******************NetIDChang*****************\n", 0, 0, 0);
			}
		}
	}
#endif

	p8d=(u8*)ADDR_AFN10F7;
	p8d[0]=n;
	p8d[1]=n>>8;
	//·�ɻָ�
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
	Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
}

void READ_Info(void)//����ز���F209_6���õ�TI���ڶ�����������Ϣ,��������Ϣ��(����Ҫ���ڶ�ȡʱ�����ö�ȡ��TI����=0)
{
	u32 i;
	u32 t;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	ms_Type *ms;
	ms=Get_ms();
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	if(UARTCtrl->Lock_InfoTask==0x55)//��·������������Ϣ������:0x55=����
	{
		return;
	}
	if(Terminal_Router->RouterRunMode_1!=2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	{//�ǿ���������ز�
		return;
	}
	if((Terminal_Router->RUNState&1)==0)//��ǰ·������״̬(��ģ���ж�ȡֵ):b2=�ϱ��¼���־,b1=������־,b0=·����ɱ�־
	{//��·����ɱ�־
		return;
	}
	MR(ADDR_DATABUFF,ADDR_F209_6+1,3);//�������ڣ�ָ�ӽڵ�����б�����������Ϣ��������Ϣ�ȸ�������
	t=pTItoSECOND((u8 *)ADDR_DATABUFF);//TIתΪ��ֵ,���:pTI����������,����:hex��
	if(t==0)
	{//����TIû����,��=0
		return;
	}
	i=YYMDHMS_Sub_hex((u8*)ADDR_Read_Info_Time,(u8*)ADDR_DL698YMDHMS);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
	if(i==0)
	{
		p8=(u8*)ADDR_Read_Info_Time;
		if(p8[2]|p8[3])
		{//�ϴμ����ʱ�ӷ�ȫ�� 0
			return;
		}
	}
	
	if((i==0)||(i>=t))
	{
		ms->link_printf("======================���ڸ��¿�ʼ==========\n",0,0,0);
		MW(ADDR_DL698YMDHMS,ADDR_Read_Info_Time,7);
		UARTCtrl->Lock_InfoTask=0x55;//��·������������Ϣ������:0x55=����
		READ_AFN10F21_TO_ADDR_AFN10F21();//��ѯȫ������������Ϣ(�Զ���ͣ�ͻָ�·��);����:������ERAM��ADDR_AFN10F21
		READ_AFN10F111_TO_ADDR_AFN10F111();//��ѯȫ����������Ϣ(�Զ���ͣ�ͻָ�·��);����:������ERAM��ADDR_AFN10F111
		READ_AFN10F112_TO_ADDR_AFN10F112();//��ѯȫ������ز�оƬ��Ϣ(�Զ���ͣ�ͻָ�·��);����:������ERAM��ADDR_AFN10F112
		READ_AFN10F104_TO_ADDR_AFN10F104();//��ѯ�ӽڵ��оƬ��Ϣ
		READ_AFN10F31_TO_ADDR_AFN10F31();//��ѯ��λ��Ϣ
		READ_AFN3F12_TO_ADDR_AFN3F12();//��ѯ�������ڵ�ͨ��ģ��ID��Ϣ
		READ_AFN10F7_TO_ADDR_AFN10F7();//��ѯ�ӽڵ�ID����Ϣ,������F10F112֮��
		ms->link_printf("======================���ڸ��½���==========\n",0,0,0);
		UARTCtrl->Lock_InfoTask=0;//��·������������Ϣ������:0x55=����
	}
}


void UART_3762_AFN03F101(u32 PORTn,u32 StartNo,u32 Num,u8* p8Buff)//(����)��ѯ�ڵ�̨����Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���ʼ�����֡��0��ʼ,Num=�ڵ�����(���֧��15��),p8Buff=���ݷ��û���
{
	u32 i;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 ReNum;
	UARTCtrl_TypeDef * UARTCtrlPLC;
#ifndef LINUX
	u32 *p32;
#else
	ms_Type* ms;
#endif
	
	UARTCtrlPLC=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
	p8tx=(u8 *)Get_ADDR_UARTnTx(RS485_4PORT);
	p8rx=(u8 *)Get_ADDR_UARTnRx(RS485_4PORT);

//���У�
	//�ڵ���ʼ���	BIN	2
	//�ڵ�����n	BIN	1
//���У�
	//̨���ڵ�������	BIN	2
	//�����ϱ��ڵ�����n(n<=15)	BIN	1
	//�ڵ�1��ַ	BCD	6
	//b7	b6	b5-3	b2-1	b0	BS	1
	//����	��·�쳣	����	̨��״̬	����		
	//�ڵ�1̨����ַ	BCD	6
	//��	��	��
	//a)	������0��ʾ���Ǽ������·��ı�����1��ʾ�Ǽ������·��ı�����
	//b)	̨��״̬��0��ʾ�ڵ�̨����ַ��ȷ����1��ʾ�ڵ�̨����ַ�ͱ���������ͬ�����ڱ�̨����2��ʾ�ڵ�̨����ַ�ͱ���������ַ����ͬ��
	//c)	���򣺽ڵ����ڵ�Դ�����0��ʾ��ȷ������1���α�ʾ��1��2��3�ࣨ001����A�ࣻ010����B�ࣻ100�ñ�C�ࣻ111�������ࣩ��
	//d)	��·�쳣���ӽڵ���·�쳣��ǣ��á�1��ʱ����ʾ�ӽڵ���·���쳣�������ܱ�Ϊ���๩��ʱ����·�쳣��ʾ����߷��ӣ������ܱ�Ϊ���๩��ʱ����·�쳣��ʾ������
	//e)	�ڵ�̨����ַ���ڵ��ַ������̨����ַ��������̨����ַ������ȷ��ȫ��F��
	MC(0,(u32)p8Buff,5);
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//ģ�鲦��
		return;
	}
	UARTCtrlPLC->InfoTask=1;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	p16timer=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16timer[0]=60;//60��
	while(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
		if(p16timer[0]==0)
		{
			UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
			MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
			return;
		}
	}
	while((UARTCtrlPLC->Task==3)||(UARTCtrlPLC->Task==4))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
	}
	UARTCtrlPLC->Task=0;
//��ͣ·��
//Terminal_Router->StopFlags;//��ͣ·�ɱ�־:(�ֶ���)
	//b0=�����0��ͣ���¼�,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
	//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
	//b16=·���������,b17=�������ϱ��¼�,b18=,b19=,b20=,b21=,b22=,b23=
	if((Terminal_Router->StopFlags&(1<<3))==0)
	{
		Terminal_Router->StopFlags|=(1<<3);//��ͣ·�ɱ�־
		Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
		while(Terminal_Router->RouterCtrl)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
		{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
		}
	}
//����Ϣ
	UARTCtrlPLC->InfoTask=2;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	ReNum=3;//�ظ�����
	while(ReNum--)
	{
		p8tx[128]=StartNo;
		p8tx[129]=StartNo>>8;
		p8tx[130]=Num;
		Uart_3762_TxData(RS485_4PORT,0x03,101,3);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
		while(1)
		{
			while(UARTCtrlPLC->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
			}
			if(UARTCtrlPLC->Task==1)
			{
				i=p8rx[1]+(p8rx[2]<<8);
				if(i<=16)
				{//���ϻش�
					break;
				}
				i=Get_Uart_3762_RxData_AFN_Fn(RS485_4PORT);//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				if(i==0x0365)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
				{//AFN=10H,Fn=101
					i=p8rx[13+2];//����Ӧ��Ľڵ�����n	BIN	1
					i*=13;
					i+=3;
				#if ((255*13)+3)>LEN_DATABUFF
					#error "��p8Buff=ADDR_DATABUFFʱ "
				#endif
					MR((u32)p8Buff,(u32)p8rx+13,i);
					UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
					UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
					return;
				}
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(RS485_4PORT);
				p16timer[0]=1000/10;//��ʱ��ʱ
				UARTCtrlPLC->RxByte=0;
				UARTCtrlPLC->Task=5;//5=���ڽ���
			}
			else
			{
				break;
			}
		}
	}
	UARTCtrlPLC->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	UARTCtrlPLC->InfoTask=0;//��·������������Ϣ������:0=��,1=�ȴ�,2...����
	MC(0,(u32)p8Buff,5);//���߳̿���ʹ���˷��û�����
}


#endif










