
//uart��1����
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "Terminal_ReadMeter_3762.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_AFN0C_RealTime.h"
#include "Terminal_Uart_TimerTask.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "Terminal_ReadMeter_SaveFnData.h"



extern Terminal_Class1DataLib_TypeDef Terminal_Class1DataLib[];

void Uart_AFN0C(u32 PORTn)//��1����
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Fn;
	u32 Pn;
	u32 DIlen;
	u32 RXDIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
  u32 LEN_UARTnTx;
	u32 SourDI;//���ηֽ�ǰ��DI
	u32 ALLPn;//ȫ����Ч�����־��0=��,1=��
	u32 Tariff;

	u8 * p8s;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u32 * p32;
	UARTCtrl_TypeDef * UARTCtrl;	
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	u32 Addr;
	UARTCtrl_TypeDef * UARTCtrlRouter;
#endif
#if (((USER/100)<=1)||(ReadF129Above==1))
	u16 *p16;
#endif
#if (((USER/100)==13)||((USER/100)==11))//������  ���� 
	u64 YMDHMS;
#endif


	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ
  LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0c;//AFN(Ϊ��ʱ�ϱ�֡��д��
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
		{
			if(DIlen<(12+6))
			{
	err:
				//ȫ������
				p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
				Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
				p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				return;
			}
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
			if(i!=0x0)
			{
				return;//�����ñ���
			}
		}
	}

	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//�û������ݱ�ʶ����
	if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
	{
		if((p8rxbuff[13]&0x80)!=0x0)
		{//��Tp
			DIlen-=(8+6);
		}
		else
		{
			DIlen-=(8);
		}
		if(DIlen<4)
		{
			goto err;
		}
	}
	else
	{
		DIlen=UARTCtrl->DIlen;//���ݱ�ʶ���ȼĴ���
	}
//����(����վ������11������2�����ݣ�����Ӧ�ò��������ݵ���·����)
	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
	{
		UARTCtrl->DIOffset=0;//�Ĵ��֡����ʱ��ǰ���ڴ������֡�����ݱ�ʶDI�ĵ�ַƫ��(���ֶ���)
		CopyDI(PORTn,RXDIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�;�����ݱ�ʶΪȫ����Ч��������Ŀ��DI��2�ֽ�Pn������Ϊ��Hex��1��ʼ
	}
	RXDIaddr+=UARTCtrl->DIOffset;
	RXaddr=RXDIaddr+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=TXDIaddr+4;
	TXlen=0;
	f=0;//������Ч���������־;0=��,1=��
	MC(0,TXDIaddr,4);
	i=MRR((u32)(&UARTCtrl->SourDI),2);
	if(i!=0x00ff)
	{//�����ݱ�ʶΪȫ����Ч������
		ALLPn=0;//ȫ����Ч�����־��0=��,1=��
	}
	else
	{
		ALLPn=1;//ȫ����Ч�����־��0=��,1=��
	}
	while(DIlen>=4)
	{
		SourDI=MRR((u32)(&UARTCtrl->DestDI),4);//���ηֽ�ǰ��DI
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		if(i==0x0)//���ݱ�ʶ�ֽ�ΪFnPn
		{
			RXDIaddr=RXaddr;
			UARTCtrl->DIOffset=RXDIaddr-(Get_ADDR_UARTnRx(PORTn)+14);//�Ĵ��֡����ʱ��ǰ���ڴ������֡�����ݱ�ʶDI�ĵ�ַƫ��(���ֶ���)
			CopyDI(PORTn,RXDIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�;�����ݱ�ʶΪȫ����Ч��������Ŀ��DI��2�ֽ�Pn������Ϊ��Hex��1��ʼ
			i=MRR((u32)(&UARTCtrl->SourDI),2);
			if(i!=0x00ff)
			{//�����ݱ�ʶΪȫ����Ч������
				ALLPn=0;//ȫ����Ч�����־��0=��,1=��
			}
			else
			{
				ALLPn=1;//ȫ����Ч�����־��0=��,1=��
			}
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)//������Ч���������־;0=��,1=��
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						goto MulTx;//����-���к���֡
					}
				}
				f=0;//������Ч���������־;0=��,1=��
			}
			MC(0,TXDIaddr,4);
			continue;
		}
		Fn=(i>>16);
		Pn=i&0xffff;

		if(ALLPn!=0x0)
		{//�����ݱ�ʶΪȫ����Ч������,����ʱ���ݱ�ʶ���������
			i=Terminal_Class1DataLib[Fn].PnMax;
			if(Fn==246)
			{//��ǰ�����¼����(Pnӳ��)
				i=MaxRS485AddCarrierPn;
			}
		#if AFN0CF33MaxPn==0
			//����Fn������������ϳ�
			switch(Fn)
			{
				case 33://F33������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
				case 34://F34�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
				case 35://F35������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				case 36://F36�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				case 37://F37���£���һ�����գ�������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
				case 38://F38���£���һ�����գ������㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12
				case 39://F39���£���һ�����գ�������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				case 40://F40���£���һ�����գ������㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12
					i=AFN0CF129MaxPn;
			}
		#endif
			if(Pn>i)
			{//ȫ��Pn��
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			switch(Terminal_Class1DataLib[Fn].PnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
			{
				//case p0Type://�ն���Ϣ��
				//	break;
				case pnType://�������
					if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
					{
						continue;
					}
					break;
				case GnType://�ܼ����
					if(GroupTotalSet(Pn)==0)//��Ч�ܼ���ż��;����0=��Ч,1=��Ч
					{
						continue;
					}
					break;
				//case CnType://�������������
				//case DnType://ֱ��ģ������
				//case RnType://�����ִκ�
				//case TnType://�����
				//case EnType;//�¼���
				//	break;
			}
			if(f!=0x0)//������Ч���������־;0=��,1=��
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if(((TXlen+4)+14+8+2)>LEN_UARTnTx)
				{
					if(DIlen>=4)
					{
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
						goto MulTx;//����-���к���֡
					}
				}
				f=0;//������Ч���������־;0=��,1=��
				MC(0,TXDIaddr,4);
			}
		}
		else
		{
			switch(Terminal_Class1DataLib[Fn].PnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
			{
				//case p0Type://�ն���Ϣ��
				//	break;
				case pnType://�������
					if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
					{
						continue;
					}
					break;
				case GnType://�ܼ����
					if(GroupTotalSet(Pn)==0)//��Ч�ܼ���ż��;����0=��Ч,1=��Ч
					{
						continue;
					}
					break;
				//case CnType://�������������
				//case DnType://ֱ��ģ������
				//case RnType://�����ִκ�
				//case TnType://�����
				//case EnType;//�¼���
				//	break;
			}
		}

#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		switch(Fn)
		{
//��ʱ���嶨��RXBUFF+128��ʼ
//2BYTE Pn
//1BYTE 
//1BYTE �м�·�ɼ���
//90BYTE(6*15) �м�1-15��ַ
//2BYTE ���ݳ���(�ֽ�)

//1BYTE ��д�˿ں�(31)
//1BYTE ��ǰ�Ѷ��м�·�ɼ���
//1BYTE ��1���м�·�ɼ���
//6*n   ��1���м��м�1-n��ַ
//...
//1BYTE ��N���м�·�ɼ���
//6*n   ��N���м��м�1-n��ַ
			case 169://���г����м�·����Ϣ
				if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
				{//������Ŵ�
					goto err;
				}
				Addr=ADDR_AFN04F10+2;//�ұ�������ŵ�ͨ�ŵ�ַ
				for(i=0;i<AFN04F10MaxPn;i++)
				{
					f=MRR(Addr,2);
					if(f==Pn)
					{
						break;
					}
					Addr+=LEN_AFN04F10_Pn;
				}
				if(i==AFN04F10MaxPn)
				{//û�ҵ�
					goto err;
				}
				f=MRR(Addr+2,1);
				f&=0x1f;
				p8rxbuff[128+4+(6*15)+2]=f;//�����ն�ͨ�Ŷ˿ں�
				if(f!=31)
				{//�����ز��˿�
					p8rxbuff[128+4+(6*15)+2+1]=0;//�м�·�ɼ���=0
					i=((u32)p8rxbuff)+128+4+(6*15)+2;//���ݵ�ַ
					x=2;//Pn���ݳ���
					goto Fn169;						
				}
				if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
				{//·������ʼ��δ���,������
					goto err;
				}
				
				//��·����Ϣ
				if((Comm_Ram->RouterType==1)||(Comm_Ram->RouterType==5))//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
				{//1=��̩����·��
					p8rxbuff[128+4+(6*15)+2+1]=0;//�м�·�ɼ���=0
					i=((u32)p8rxbuff)+128+4+(6*15)+2;//���ݵ�ַ
					x=2;//Pn���ݳ���
					goto Fn169;
				}
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ
				UARTCtrlRouter=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);//�õ�UART��������ַ
				p32=(u32 *)(ADDR_MS_ENTRY);
				while((UARTCtrlRouter->Lock==0x55)||(UARTCtrlRouter->Task!=0))
				{//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���,0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					(*(void(*)())p32[115])();//I0_Main_Return;63 ����������
					if(UARTCtrlRouter->Lock==0x55)
					{
						goto err;
					}
				}
				UARTCtrlRouter->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
				//��ͣ·��
				Terminal_Router->StopFlags|=1<<3;//��ͣ·�ɱ�־:b0=,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
				Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
				while(Terminal_Router->RouterCtrl!=0)
				{
					(*(void(*)())p32[115])();//I0_Main_Return;63 ����������
					if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					{
						UARTCtrlRouter->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						goto err;
					}
				}
				//��ָ���ڵ����һ���м�·����Ϣ
				MR(Get_ADDR_UARTnTx(RS485_4PORT)+128,Addr+4,6);
				Uart_3762_TxData(RS485_4PORT,0x10,3,6);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
				while(UARTCtrlRouter->Task!=1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				{
					if(UARTCtrlRouter->Task<=2)
					{//0=����,2=���ճ�ʱ(�����)
						//�ָ�·��
						Terminal_Router->StopFlags&=~(1<<3);//��ͣ·�ɱ�־:b0=,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
						UARTCtrlRouter->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						goto err;
					}
					(*(void(*)())p32[115])();//I0_Main_Return;63 ����������
				}
				Addr=Get_ADDR_UARTnRx(RS485_4PORT);
				i=MRR(Addr+10,3);
				if(i!=0x410)
				{
					//�ָ�·��
					Terminal_Router->StopFlags&=~(1<<3);//��ͣ·�ɱ�־:b0=,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
					Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
					UARTCtrlRouter->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
					goto err;
				}
				else
				{//AFN=10H,Fn=3
					x=MRR(Addr+10+3,1);
					if(x>15)
					{
						x=15;//���15��
					}
					if(Comm_Ram->RouterType==2)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=
					{//���ŷ��ص��м�·����Ϣ�������ڵ��ַ
						if(x!=0)
						{
							x--;//ȥ���ڵ��ַ
						}
						p8rxbuff[128+3]=x;//�м�·�ɼ���
						for(i=0;i<x;i++)
						{
							MR(((u32)p8rxbuff)+128+4+(6*i),Addr+10+3+1+8+(8*i),6);
						}
						p8rxbuff[128+4+(6*15)]=2;//���ݳ���(�ֽ�)
						p8rxbuff[128+4+(6*15)+1]=0;//���ݳ���(�ֽ�)
						p8rxbuff[128+4+(6*15)+3]=0;//��ǰ�Ѷ��м�·�ɼ���
					}
					else
					{//
						p8rxbuff[128+3]=x;//�м�·�ɼ���
						for(i=0;i<x;i++)
						{
							MR(((u32)p8rxbuff)+128+4+(6*i),Addr+10+3+1+(8*i),6);
						}
						p8rxbuff[128+4+(6*15)]=2;//���ݳ���(�ֽ�)
						p8rxbuff[128+4+(6*15)+1]=0;//���ݳ���(�ֽ�)
						p8rxbuff[128+4+(6*15)+3]=0;//��ǰ�Ѷ��м�·�ɼ���
					}
				}
				//�����м���Ϣ
				while(p8rxbuff[128+4+(6*15)+3]<p8rxbuff[128+3])
				{//��ǰ�Ѷ��м�·�ɼ���<�м�·�ɼ���
					x=p8rxbuff[128+4+(6*15)+3];//��ǰ�Ѷ��м�·�ɼ���
					Fn=p8rxbuff[128+4+(6*15)];//���ݳ���(�ֽ�)
					Fn|=p8rxbuff[128+4+(6*15)+1]<<8;//���ݳ���(�ֽ�)
					MR(Get_ADDR_UARTnTx(RS485_4PORT)+128,((u32)p8rxbuff)+128+4+(6*x),6);
					Uart_3762_TxData(RS485_4PORT,0x10,3,6);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					while(UARTCtrlRouter->Task!=1)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
						if(UARTCtrlRouter->Task<=2)
						{//0=����,2=���ճ�ʱ(�����)
							//�ָ�·��
							Terminal_Router->StopFlags&=~(1<<3);//��ͣ·�ɱ�־:b0=,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
							Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
							UARTCtrlRouter->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
							goto err;
						}
						(*(void(*)())p32[115])();//I0_Main_Return;63 ����������
					}
					i=MRR(Addr+10,3);
					if(i!=0x410)
					{
						//�ָ�·��
						Terminal_Router->StopFlags&=~(1<<3);//��ͣ·�ɱ�־:b0=,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
						Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
						UARTCtrlRouter->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						goto err;
					}
					else
					{//AFN=10H,Fn=3
						Pn=MRR(Addr+10+3,1);
						if(Pn>15)
						{
							Pn=15;//���15��
						}
						p8rxbuff[128+4+(6*15)+2+Fn]=1+Pn;//�м�·�ɼ���
						MR(((u32)p8rxbuff)+128+4+(6*15)+2+Fn+1,((u32)p8rxbuff)+128+4+(6*x),6);
						for(i=0;i<Pn;i++)
						{
							MR(((u32)p8rxbuff)+128+4+(6*15)+2+Fn+1+6+(6*i),Addr+10+3+1+(8*i),6);
						}
						Fn+=1+6+(6*Pn);
						p8rxbuff[128+4+(6*15)]=Fn;//���ݳ���(�ֽ�)
						p8rxbuff[128+4+(6*15)+1]=Fn>>8;//���ݳ���(�ֽ�)
						x++;;//��ǰ�Ѷ��м�·�ɼ���+1
						p8rxbuff[128+4+(6*15)+3]=x;
					}
				}
				//�ָ�·��
				Terminal_Router->StopFlags&=~(1<<3);//��ͣ·�ɱ�־:b0=,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
				Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
				UARTCtrlRouter->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
				i=((u32)p8rxbuff)+128+4+(6*15)+2;//���ݵ�ַ
				x=p8rxbuff[128+4+(6*15)];//���ݳ���(�ֽ�)
				x|=p8rxbuff[128+4+(6*15)+1]<<8;//���ݳ���(�ֽ�)
				goto Fn169;
		}
#endif

		i=GetClass1DataAddr(Fn,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
		if(i==0x0)
		{
		#if AFN0CF33MaxPn!=0
			continue;
		#else
			switch(Fn)
			{
				case 33://F33 ��ǰ������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					Class1_F33_F34_F37_F38(Pn,ADDR_DATABUFF,129,130,133,136);//����������ϳ�F33_F34_F37_F38
					break;
				case 34://F34 ��ǰ������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					Class1_F33_F34_F37_F38(Pn,ADDR_DATABUFF,131,132,134,135);//����������ϳ�F33_F34_F37_F38
					break;
				case 35://F35 ����������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					Class1_F35_F36_F39_F40(Pn,ADDR_DATABUFF,145,146);//����������ϳ�F35_F36_F39_F40
					break;
				case 36://F36 ���·�����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					Class1_F35_F36_F39_F40(Pn,ADDR_DATABUFF,147,148);//����������ϳ�F35_F36_F39_F40
					break;
				case 37://F37 ���£���һ�����գ�������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					Class1_F33_F34_F37_F38(Pn,ADDR_DATABUFF,137,138,141,144);//����������ϳ�F33_F34_F37_F38
					break;
				case 38://F38 ���£���һ�����գ�������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					Class1_F33_F34_F37_F38(Pn,ADDR_DATABUFF,139,140,142,143);//����������ϳ�F33_F34_F37_F38
					break;
				case 39://F39 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					Class1_F35_F36_F39_F40(Pn,ADDR_DATABUFF,149,150);//����������ϳ�F35_F36_F39_F40
					break;
				case 40://F40 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					Class1_F35_F36_F39_F40(Pn,ADDR_DATABUFF,151,152);//����������ϳ�F35_F36_F39_F40
					break;
				case 246://��ǰ�����¼����(������ӳ��)
					MC(0xee,ADDR_DATABUFF,1+Terminal_Class1DataLib[Fn].PnLen);
					break;
				default:
					continue;
			}
			i=ADDR_DATABUFF+1;//���ݵ�ַ
			p8s=(u8 *)i;
			p32=(u32 *)(ADDR_Terminal_ClassDataLen);
			//���¼��㳤��
		#if DefaultTariff==0//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
			Tariff=0;
		#else
			#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
				Tariff=Get_SetTariff(Pn);//ȡAFN04F10�����õķ�����
			#else
				Tariff=TMaxTariff;
			#endif
		#endif
			switch(Fn)
			{
				case 33://F33������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
				case 34://F34�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
				case 37://F37���£���һ�����գ�������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
				case 38://F38���£���һ�����գ������㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12)
			#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
					if(p8s[5]!=Tariff)
					{
						p8s[5]=Tariff;
					}
			#else
					if(p8s[5]==0xee)
					{
						p8s[5]=Tariff;
					}
			#endif
					p32[0]=6+(17*(p8s[5]+1));
					break;
				case 35://F35������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				case 36://F36�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				case 39://F39���£���һ�����գ�������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				case 40://F40���£���һ�����գ������㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12
			#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
					if(p8s[5]!=Tariff)
					{
						p8s[5]=Tariff;
					}
			#else
					if(p8s[5]==0xee)
					{
						p8s[5]=Tariff;
					}
			#endif
					p32[0]=6+(14*(p8s[5]+1));
					break;
				case 246://��ǰ�����¼����(������ӳ��)
					p32[0]=Terminal_Class1DataLib[Fn].PnLen;
					break;
				default:
					continue;
			}
		#endif
		}
		p32=(u32 *)(ADDR_Terminal_ClassDataLen);
		x=p32[0];
		if(x==0x0)
		{
			continue;
		}
		
		if(Fn==13)
		{//F13���������ĵ����Ϣ
			//���㷢�ͻ��廹������ĵ����Ϣ����
			if((TXlen+4+14+8+2+(4+13))>LEN_UARTnTx)
			{
				if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
				{
					TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
				}
				MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
				goto MulTx;//����-���к���֡
			}
			FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
			x=LEN_UARTnTx-(TXlen+4+14+8+2+4);
			x/=13;//�ɷ��͵����Ϣ����
			y=MRR(ADDR_AFN0CF13,2);//�������ĵ��������
			if(y>MaxRS485AddCarrierPn)
			{
				y=MaxRS485AddCarrierPn;
			}
				
			i=y-UARTCtrl->MulFrameGPCR;//��֡ʱͨ�ü����Ĵ���(��AFN0CF13��,������F13�ѷ������Ϣ����,��F13��ɺ���0)
			if(x>i)
			{
				x=i;
			}
			z=ADDR_AFN0CF13+2+(LEN_PerMeterAFN0CF13*UARTCtrl->MulFrameGPCR);
			//x=��ǰҪ����ĵ����Ϣ����
			//y=�������ĵ��������
			//z=ADDR_AFN0CF13��ַ
			MWR(y,TXaddr,2);//�ѵ����������m	BIN	2
			MWR(x,TXaddr+2,2);//��֡�����������n	BIN	2
			TXaddr+=4;
			TXlen+=4;
			for(i=0;i<x;i++)
			{
				MR(TXaddr,z+(LEN_PerMeterAFN0CF13*i),13);//�������Ϣ
				TXaddr+=13;
				TXlen+=13;
			}
			UARTCtrl->MulFrameGPCR+=x;//��֡ʱͨ�ü����Ĵ���(��AFN0CF13��,������F13�ѷ������Ϣ����,��F13��ɺ���0)
			if(UARTCtrl->MulFrameGPCR>=y)
			{
				UARTCtrl->MulFrameGPCR=0;//��֡ʱͨ�ü����Ĵ���(��AFN0CF13��,������F13�ѷ������Ϣ����,��F13��ɺ���0)
				goto FnSelectEnd;
			}
			if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
			{
				TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
			}
			MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
			goto MulTx;//����-���к���֡
		}
	#if (USER/100)==6//�û�:����ϵ��չ,�Զ��ѵ���δ֪���
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		if(Fn==171)
		{//F171
			//���㷢�ͻ��廹������ĵ����Ϣ����
			if((TXlen+4+14+8+2+(4+(6+14)))>LEN_UARTnTx)
			{
				if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
				{
					TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
				}
				MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
				goto MulTx;//����-���к���֡
			}
			x=LEN_UARTnTx-(TXlen+4+14+8+2+(4+6));
			x/=14;//�ɷ��͵����Ϣ����
			
			y=MRR(ADDR_AFN0CF13,2);//�������ĵ��������
			if(y>MaxRS485AddCarrierPn)
			{
				y=MaxRS485AddCarrierPn;
			}
			MWR(y,TXaddr,2);//�ѵ����������m	BIN	2
			z=MRR(RXaddr,2);//���ζ�ȡ��ʼ���(��1��ʼ)
			if(z==0)
			{
				z=1;
			}
			z--;
			MWR(z+1,RXaddr,2);//���ζ�ȡ��ʼ���(��1��ʼ)
			MWR(z+1,TXaddr+2,2);//��ʼ���
			Addr=ADDR_AFN0CF13+2+(LEN_PerMeterAFN0CF13*z);
			if(z>y)
			{
				z=y;
			}
			y-=z;//�������ĵ��������
			z=MRR(RXaddr+2,2);//���ζ�ȡ���������0=��ʾ��ȡȫ��
			if(z==0)
			{
				if(UARTCtrl->MulFrameGPCR==0)//����֡��־:0=û,1=����
				{
					z=y;
				}
			}
			if(z>y)
			{
				z=y;
			}
			MWR(z,RXaddr+2,2);//���ζ�ȡ���������
			if(z>x)
			{
				UARTCtrl->MulFrameGPCR=1;//�к���֡��־:0=û,1=����
				i=z-x;
				MWR(i,RXaddr+2,2);//�´ζ�ȡ���������
				i=MRR(RXaddr,2);//�´ζ�ȡ��ʼ���(��1��ʼ)
				i+=x;
				MWR(i,RXaddr,2);//�´ζ�ȡ��ʼ���(��1��ʼ)
				z=x;
			}
			else
			{
				UARTCtrl->MulFrameGPCR=0;//�к���֡��־:0=û,1=����
			}
			MWR(z,TXaddr+4,2);//��֡�����������n	BIN	2
			//UARTCtrl->MulFrameGPCR=�к���֡��־:0=û,1=����
			//x=��ǰ������ĵ����Ϣ����
			//y=�������ĵ��������
			//A=���ζ�ȡ��ʼ���(��1��ʼ)
			//z=���ζ�ȡ���������0=��ʾ��ȡȫ��
			TXaddr+=6;
			TXlen+=6;
			for(i=0;i<z;i++)
			{
				MR(TXaddr,Addr,7);//���ܱ��ַ,ͨ��Э������
				MR(TXaddr+7,Addr+14,1);//�����λ
				y=MRR(Addr+13,1);//�˿ں�
				if(y==31)
				{
					MR(TXaddr+8,(u32)&Terminal_Router->RouterAddr,6);
				}
				else
				{
					MC(0,TXaddr+8,6);
				}
				Addr+=LEN_PerMeterAFN0CF13;
				TXaddr+=14;
				TXlen+=14;
			}
			if(UARTCtrl->MulFrameGPCR==0)//�к���֡��־:0=û,1=����
			{
				RXaddr+=4;
				goto FnSelectEnd;
			}
			if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
			{
				TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
			}
			MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
			goto MulTx;//����-���к���֡
		}
	#endif
	#endif
		if((Fn>=81)&&(Fn<=121))
		{//Сʱ��������
			if(PnHourCongeal(Pn)==0)//������Сʱ������Ч,����=0��Ч,1=��Ч
			{
				continue;
			}
			//2�ֽڶ���ʱ��,�����ܶ�
			if((i>ADDR_HOURCONGEAL_START)&&(i<ADDR_HOURCONGEAL_END))
			{//��EFLSH��
				p8s=(u8 *)(ADDR_HOURCONGEAL_START);
			}
			else
			{//��ESRAM��
				p8s=(u8 *)(ADDR_HOURCONGEAL_BUFF_START);
			}
			if(p8s[1]==0x0)
			{//Сʱ�����ܶ�=0(������)
				continue;
			}
			if((TXlen+4+14+8+2+(2+x))>LEN_UARTnTx)
			{
				if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
				{
					TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
				}
				MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
				goto MulTx;//����-���к���֡
			}
      
			MR(TXaddr,(u32)p8s,2);
	#if SoftBug1==1//0=��������,1==376��վ�������BUG.Сʱ�����ܶ�
			MC(MaxHourCongeal,TXaddr+1,1);
	#endif
			//X�ֽڶ�������
			MR(TXaddr+2,i,x);
		#if Uart3761Check0xEE==1//0xEE���ݼ�飺0=�����;1=���,��ȫ0xEE���ط���
			if(Terminal_Class1DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
			{
				if(CheckUartData_0xEE(TXaddr+2,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
				{
					continue;
				}
			}
		#endif
			TXlen+=2+x;
			TXaddr+=2+x;
		}
		else
		{
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
Fn169:
#endif
			if((TXlen+4+14+8+2+(x))>LEN_UARTnTx)
			{
				if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
				{
					TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
				}
				MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
				goto MulTx;//����-���к���֡
			}
			
//AFN0CF129�Ժ�Ҫʵʱ����*****************************************************************
#if ReadF129Above==1//�ٶ�1��F129��������:0=��ԭ�洢����,1=ʵʱ��ȡ�����ϴ�
	#if (USER/100)==6//����
			if((Fn>=129)||(Fn==27)||(Fn==28))//F169���г����м�·����Ϣר��ʵʱ����,F170���г���������Ϣ
	#else
		#if (USER/100)==11//����ϵ
			if((Fn>=129)||(Fn==28))//F169���г����м�·����Ϣר��ʵʱ����,F170���г���������Ϣ
		#else
			if(Fn>=129)//F169���г����м�·����Ϣר��ʵʱ����,F170���г���������Ϣ
		#endif
	#endif
			{
				switch(Fn)
				{
					case 169://���г����м�·����Ϣ	�������
					case 170://���г���������Ϣ	�������
						break;
					case 179://���յ�ѹͳ������
					case 180://���µ�ѹͳ������
					#if (USER/100)==11//����
						//���ְ��ָ���Ѹ�Ϊָ��RAM,���ڲ�������ר�ټ������ݵ�ַ
						if(Check_ACSamplePn(Pn))//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
						{//���ڲ�����
							if((Pn<=MaxRS485Pn)&&(Pn!=0))
							{//�ڲ����ɲ������<=MaxRS485Pn
								if(Fn==179)
								{//F179
									i=ADDR_AFN0DF27_SOUR+((66+27)*(Pn-1));
								}
								else
								{//F180
									i=ADDR_AFN0DF35_SOUR+((66+27)*(Pn-1));
								}
							}
						}
					#endif
						break;
				#if (USER/100)==5//�Ϻ�
					case 191://�ն˵�ǰ�ߵ�ѹ
					case 241://	��ǰ��¼��վ��Ϣ�����һ�Σ�
						break;
					case 242://	���յ�¼��վ��Ϣ
					case 243://	�����ź�ǿ��
						i=ADDR_DATABUFF;
						break;
				#endif
					case 245://	�ն��¼���������
					case 246://	��ǰ�����¼����(������ӳ��)
					case 247://	��ǰ�ն˵��״̬
						break;
				#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
					case 249://����ʱ����
						break;
					case 250://����ʱͳ������
						x=1+10*MRR(i,1);
						break;
				#endif
				#if (USER/100)==3//�û���ʶ������
					case 253://�ֳ��ź���Ϣ
					case 254://����ͨ��ģ����Ϣ
						break;
				#endif
				#if (USER/100)==11//����
					case 201://�ֳ��ź���Ϣ
						break;
				#endif
					default:
						if((UARTCtrl->LockFlags&0x70)==0)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=����1�ඨʱ����,B5=����2�ඨʱ����,B6=�ٶ�1�ඨʱ����,B7
						{
						#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
							if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
						#else
							if(Terminal_Ram->PowerTask>10)//ͣ���¼�����
						#endif
							{//3=��������
								y=ReadMeter_RealTime(PORTn,Fn,Pn);//ʵʱ��ȡ1������;����0=û��,!=0������FnDataBuff��ַ
								if(y!=0)//ʵʱ��ȡ1������;����0=û��,!=0������FnDataBuff��ַ
								{//��ʵʱ��ȡ
									if(Fn!=28)//F28 �������״̬�ּ����λ��־
									{
										i=y;//���ݵ�ַ
									}
								}
								else
								{//û��ȡ
									y=Check_AFN04F10_Pn(Pn);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
									if(y==0)
									{//����0��
										i=ADDR_DATABUFF;
										MC(0xee,ADDR_DATABUFF,Terminal_Class1DataLib[Fn].PnLen);
									}
									else
									{//
										y=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+4,2);//ͨ�����ʼ��˿ں�,ͨ��Э������
										if(((y&=0x1f)==1)||((y>>8)==2))
										{//�˿ں���=1��Э��=2,���ڲ�����
											//����ȡ��ǰ��������
										}
										else
										{//�ⲿ�������
											i=ADDR_DATABUFF;
											MC(0xee,ADDR_DATABUFF,Terminal_Class1DataLib[Fn].PnLen);
										}
									}
								}
								if(ALLPn!=0)//ȫ����Ч�����־;0=��,1=��
								{
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
									DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
									if(p16[0]!=p16[1])
									{//�ѽ��յ���֡,����ȫ����Ч�����ʵʱ��ȡ֡
										UARTCtrl->Task=0;//0=����
										UARTCtrl->RxByte=0;//rx����=0
										return;
									}
								}
								//��ʵʱ��ȡ���޸ķ��ͻ��峤����������ÿֻ��
								LEN_UARTnTx=0;//��ʵʱ��ȡ,���޸ķ��ͻ��峤��=0(ģ�ⷢ�ͻ��������ж�֡����)
								//����ʱ��
								switch(Fn)
								{
									case 49://F49 ��ǰ��ѹ��������λ��
									case 0xF1://��չ17 ��ǰ��ѹ��������λ��(��չ�����ն��ڲ�����)
										Fn=49;
										break;
									case 57://F57 ��ǰA��B��C�����ѹ������2��N��г����Чֵ
									case 58://F58 ��ǰA��B��C�����ѹ������2��N��г��������
									case 162://F162 ���ܱ�����ʱ��
										break;
									default:
									#if Uart3761Check0xEE==0//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
										#if (((USER/100)==13)||((USER/100)==11))//������  ���� 
											if(PORTn==ACSAMPLEPORT) 
											{//���������������ն��᳭��ʱ��Ϊ23:59��Ҫ��Ϊ00:00�Ժ�  (�޸Ľ��ɿڵĳ���ʱ����Ϊ23:59�����޸�Ϊ��һ�յ�00:00�� 

													YMDHMS=MRR(ADDR_TYMDHMS,6); 
													if((YMDHMS&0xFFFFFF)>0x235950) 
													{ 
															YMDHMS=YMDHMS_AddS(YMDHMS,10);//+10s 
													} 
													YMDHMS=(YMDHMS>>8); 
												 MWR(YMDHMS,i,5);     
											} 
											else 
											{ 
												 MW(ADDR_TYMDHMS+1,i,5);//���볭��ʱ�� 
											} 
										#else 
										 MW(ADDR_TYMDHMS+1,i,5);//���볭��ʱ�� 
										#endif 
									#else
										p8s=(u8 *)i;
										for(y=0;y<Terminal_Class1DataLib[Fn].PnLen;y++)
										{
											if(p8s[y]!=0xee)
											{
												break;
											}
										}
										if(y!=Terminal_Class1DataLib[Fn].PnLen)
										{//��ʵʱ��ȡ,���볭��ʱ��(�Ƿ񷵻ط���ֻ�п�ʼ�ĳ���ʱ��5�ֽ�)
										#if (((USER/100)==13)||((USER/100)==11))//������  ���� 
											if(PORTn==ACSAMPLEPORT) 
											{//���������������ն��᳭��ʱ��Ϊ23:59��Ҫ��Ϊ00:00�Ժ�  (�޸Ľ��ɿڵĳ���ʱ����Ϊ23:59�����޸�Ϊ��һ�յ�00:00�� 

													YMDHMS=MRR(ADDR_TYMDHMS,6); 
													if((YMDHMS&0xFFFFFF)>0x235950) 
													{ 
															YMDHMS=YMDHMS_AddS(YMDHMS,10);//+10s 
													} 
													YMDHMS=(YMDHMS>>8); 
												 MWR(YMDHMS,i,5);     
											} 
											else 
											{ 
												 MW(ADDR_TYMDHMS+1,i,5);//���볭��ʱ�� 
											} 
										#else 
										 MW(ADDR_TYMDHMS+1,i,5);//���볭��ʱ�� 
										#endif 
										}
									#endif
										break;
								}
								//������Ч���ݳ���,�������ȱ��ʱ����Ҳ��
							#if DefaultTariff==0//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
								Tariff=0;
							#else
								#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
									Tariff=Get_SetTariff(Pn);//ȡAFN04F10�����õķ�����
								#else
									Tariff=TMaxTariff;
								#endif
							#endif								
								p8s=(u8 *)i;
								switch(Fn)
								{
									case 129://F129	��ǰ�����й�����ʾֵ���ܡ�����1��M��	�������
									case 131://F131	��ǰ�����й�����ʾֵ���ܡ�����1��M��	�������
									case 137://F137	���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��	�������
									case 139://F139	���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��	�������
									case 153://F153	��һʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
									case 154://F154	�ڶ�ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
									case 155://F155	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
									case 156://F156	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
									case 157://F157	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
									case 158://F158	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
									case 159://F159	����ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
									case 160://F160	�ڰ�ʱ�����������й�����ʾֵ���ܡ�����1��M��	�������
									case 177://F177 ��ǰ����й�����ʾֵ���ܡ�����1~M��
									case 178://F178 ��1 ����������й�����ʾֵ���ܡ�����1~M��
								#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
										if(p8s[5]!=Tariff)
										{
											p8s[5]=Tariff;
										}
								#else
										if(p8s[5]==0xee)
										{
											p8s[5]=Tariff;
										}
								#endif
										x=6+(5*(p8s[5]+1));
										break;
									case 130://F130	��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��	�������
									case 132://F132	��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��	�������
									case 133://F133	��ǰһ�����޹�����ʾֵ���ܡ�����1��M��	�������
									case 134://F134	��ǰ�������޹�����ʾֵ���ܡ�����1��M��	�������
									case 135://F135	��ǰ�������޹�����ʾֵ���ܡ�����1��M��	�������
									case 136://F136	��ǰ�������޹�����ʾֵ���ܡ�����1��M��	�������
									case 138://F138	���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��	�������
									case 140://F140	���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��	�������
									case 141://F141	���£���һ�����գ�һ�����޹�����ʾֵ���ܡ�����1��M��	�������
									case 142://F142	���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��	�������
									case 143://F143	���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��	�������
									case 144://F144	���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��	�������
								#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
										if(p8s[5]!=Tariff)
										{
											p8s[5]=Tariff;
										}
								#else
										if(p8s[5]==0xee)
										{
											p8s[5]=Tariff;
										}
								#endif
										x=6+(4*(p8s[5]+1));
										break;
									case 145://F145	���������й��������������ʱ�䣨�ܡ�����1��M��	�������
									case 146://F146	���������޹��������������ʱ�䣨�ܡ�����1��M��	�������
									case 147://F147	���·����й��������������ʱ�䣨�ܡ�����1��M��	�������
									case 148://F148	���·����޹��������������ʱ�䣨�ܡ�����1��M��	�������
									case 149://F149	���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��	�������
									case 150://F150	���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��	�������
									case 151://F151	���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��	�������
									case 152://F152	���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��	�������
								#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
										if(p8s[5]!=Tariff)
										{
											p8s[5]=Tariff;
										}
								#else
										if(p8s[5]==0xee)
										{
											p8s[5]=Tariff;
										}
								#endif
										x=6+(7*(p8s[5]+1));
										break;
									case 168://F168	���ܱ������Ϣ	�������
								#if DefaultTariff==1//������ʱ��Ĭ�Ϸ�����:0=������Ϊ0,1=��AFN04F10�����õķ�����,2=������TMaxTariff
										if(p8s[5]!=Tariff)
										{
											p8s[5]=Tariff;
										}
								#else
										if(p8s[5]==0xee)
										{
											p8s[5]=Tariff;
										}
								#endif
										x=6+(10*(bcd_hex(p8s[5]+1)));
										break;
								}
							}
						}
						break;
				}
			}
#endif
//****************************************************************************************

			MR(TXaddr,i,x);
		#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
			if(Terminal_Class1DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
			{
				if(CheckUartData_0xEE(TXaddr,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
				{
					LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
					continue;
				}
			}
		#endif
			TXlen+=x;
			TXaddr+=x;
		}
	FnSelectEnd:
		FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
		f=1;//������Ч���������־;0=��,1=��

		switch(Fn)
		{//�����ݺ���������ݵ�Fn
			case 9://�ն�״̬������λ��־
				MC(0,ADDR_AFN0CF9+1,1);
				break;
			case 12://���������·���ؽ���״̬����λ��־
				MC(0,ADDR_AFN0CF12+1,1);
				break;
			case 28://�������״̬�ֱ�λ��־
				MC(0,ADDR_AFN0CF28+5+(33*(Pn-1)),14);
				break;
		}
	}
	
	if(TXlen==0x0)
	{
		if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
		{
			goto err;
		}
	}
	TXlen+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	if(UARTCtrl->TxFrameCount!=0)//�Ĵ��֡����ʱ�ѷ���֡����
	{
		p8txbuff[13]&=0x9f;//����֡Ϊ��֡�м�֡TpV,FIR,FIN,CON;PSEQ|RSEQ
		p8txbuff[13]|=0x20;//����֡��Ϊ��֡����֡TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//�Ĵ��֡����ʱ�ѷ���֡����
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ,�ݵȴ���֡���ݷ���
	Uart_376_CON(PORTn);//������ʱ����CON��־λ�͹�����
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
	//������֡���TSEQ+1(��Ϊ��ʱ�����ϱ�ʱ��Ҫ)
	if(UARTCtrl->LockFlags&0x30)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=����1�ඨʱ����,B5=����2�ඨʱ����,B6=�ٶ�1�ඨʱ����,B7
	{
		UARTCtrl->TxSEQ=p8rxbuff[13]&0x0f;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
		UARTCtrl->TxSEQ+=1;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
	}
	return;
MulTx://����-���к���֡
	if(TXlen<=4)
	{
//		if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
//		{
			goto err;
//		}
	}
	TXlen+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	p8txbuff[13]&=0x9f;//����֡Ϊ��֡�м�֡TpV,FIR,FIN,CON;PSEQ|RSEQ
	if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
	{
		p8txbuff[13]|=0x40;//����֡Ϊ��֡��ʼ֡TpV,FIR,FIN,CON;PSEQ|RSEQ
	}
	UARTCtrl->TxFrameCount++;//�Ĵ��֡����ʱ�ѷ���֡����
	UARTCtrl->DIlen=DIlen;//���ݱ�ʶ���ȼĴ���
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ,�ݵȴ���֡���ݷ���
	Uart_376_CON(PORTn);//������ʱ����CON��־λ�͹�����
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
	//����Ӧ֡���RSEQ+1
	i=p8rxbuff[13];
	x=i;
	i&=0xf0;
	x++;
	x&=0x0f;
	i|=x;
	p8rxbuff[13]=i;
	UARTCtrl->Task=5;//5=�ȴ���֡���ݷ���
	return;
}


#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
void Uart_AFN0CF201(u32 PORTn)//�Ϻ���������չAFN0CF201��ȡ���һ�γ���ʧ�ܱ����P0
{
	u32 i;
	u32 x;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0c;//AFN
//�û������ݱ�ʶ����
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(DIlen<(12+6))
		{
err:
			//ȫ������
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
			Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
		if(i!=0x0)
		{
			return;//�����ñ���
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	TXlen=12+2;
	//����
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	//��AFN0CF170���г���������Ϣ�м������һ�γ���ʧ�ܱ����
	x=0;
	for(i=0;i<MaxRS485AddCarrierPn;i++)
	{
		p8=(u8 *)(ADDR_AFN0CF170+(18*i));
		if(p8[4]==0)
		{//���һ�γ���ɹ�/ʧ�ܱ�־��1/0��ʾ�ɹ�/ʧ�ܡ�
			x++;
		}
	}
	p8txbuff[18]=x;
	p8txbuff[19]=(x>>8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}

void Uart_AFN0CF202(u32 PORTn)//�Ϻ���������չAFN0CF202��ȡ���һ�γ������ܱ�ʧ�ܱ���ϢP0
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 TXlen;
	u32 DIlen;
	u32 n;//ÿ֡����ͳ��ո���
	UARTCtrl_TypeDef * UARTCtrl;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8;

	if(PORTn==IRPORT)
	{//�����
		n=10;//ÿ֡����ͳ��ո���
	}
	else
	{
		n=50;//ÿ֡����ͳ��ո���
	}

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0c;//AFN
//�û������ݱ�ʶ����
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
	{
	//Tpʱ���ǩ��Ч�ж�
		if((p8rxbuff[13]&0x80)!=0x0)
		{//��Tp
			if(DIlen<(12+4+6))
			{
	err:
				//ȫ������
				p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
				Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				return;
			}
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
			if(i!=0x0)
			{
				return;//�����ñ���
			}
		}
		else
		{
			if(DIlen<(12+4))
			{
				goto err;
			}
		}
		//��ʼ���
		x=p8rxbuff[18]+(p8rxbuff[19]<<8);
		//�������
		y=p8rxbuff[20]+(p8rxbuff[21]<<8);
		if(x>y)
		{//��ʼ���>�������
			goto err;
		}

		x=0;
		for(y=0;y<MaxRS485AddCarrierPn;y++)
		{
			p8=(u8 *)(ADDR_AFN0CF170+(18*y));
			if(p8[4]==0)
			{//���һ�γ���ɹ�/ʧ�ܱ�־��1/0��ʾ�ɹ�/ʧ�ܡ�
				x++;
			}
		}
		if(x<(p8rxbuff[18]+(p8rxbuff[19]<<8)))
		{
			x=0;
			goto TX;
		}
		else
		{
			//������
			y=(p8rxbuff[20]+(p8rxbuff[21]<<8))-(p8rxbuff[18]+(p8rxbuff[19]<<8))+1;
			//����>=�������ҳ�����<=һ֡���͸���������<������������<=һ֡���͸���
			if(((x>=y)&&(y<=n)) || ((x<y)&&(x<=n)))	
			{//��֡����
				//��AFN0CF170���г���������Ϣ�м������x~y�γ���ʧ�ܱ���Ϣ
				x=0;
				for(i=0;i<MaxRS485AddCarrierPn;i++)
				{
					p8=(u8 *)(ADDR_AFN0CF170+(18*i));
					if(p8[4]==0)
					{//���һ�γ���ɹ�/ʧ�ܱ�־��1/0��ʾ�ɹ�/ʧ�ܡ�
						if(x==(p8rxbuff[18]+(p8rxbuff[19]<<8)))
						{
							break;
						}
						x++;
					}
				}
				x=0;
				for(y=i;y<MaxRS485AddCarrierPn;y++)
				{
					p8=(u8 *)(ADDR_AFN0CF170+(18*y));
					if(p8[4]!=0)
					{//���һ�γ���ɹ�/ʧ�ܱ�־��1/0��ʾ�ɹ�/ʧ�ܡ�
						continue;
					}
					//������Ϣ���
					p8txbuff[18*x+20]=x;
					p8txbuff[18*x+21]=(x>>8);
					z=Check_AFN04F10_Pn(y+1);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
					//������š�ͨ�����ʼ��˿ںš�ͨ��Э�����͡�ͨ�ŵ�ַ
					MR((u32)p8txbuff+18*x+22,ADDR_AFN04F10+LEN_AFN04F10_Pn*(z-1)+2,10);
					//�����������
					MR((u32)p8txbuff+18*x+22+10,ADDR_AFN04F10+LEN_AFN04F10_Pn*(z-1)+2+8,6);
					x++;
					if(x==((p8rxbuff[20]+(p8rxbuff[21]<<8))-(p8rxbuff[18]+(p8rxbuff[19]<<8))+1))
					{
						break;
					}
				}
TX:
				//���ո���
				p8txbuff[18]=x;
				p8txbuff[19]=(x>>8);
				//����
				TXlen=12+2+18*x;
				p8txbuff[1]=TXlen;
				p8txbuff[2]=(TXlen>>8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				return;
			}
			else
			{//��֡����
				goto MulTX;
			}
		}
	}
MulTX://��֡����
	//��ʼ���
	x=(p8rxbuff[18]+(p8rxbuff[19]<<8))+n*UARTCtrl->TxFrameCount;
	//�������
	y=p8rxbuff[20]+(p8rxbuff[21]<<8);
	if(x>y)
	{//��ʼ���>�������
		//��֡����֡
		p8txbuff[13]|=0x20;
		UARTCtrl->TxFrameCount=0;
		//���ո���
		x=0;
		goto TX;
	}
//SEQ��D6D5��11��֡��10��֡��1֡�к���֡��00��֡�м�֡��01��֡����֡
	//��֡�м�֡
	p8txbuff[13]&=0x9f;
	if(UARTCtrl->TxFrameCount==0)
	{//��֡��1֡
		p8txbuff[13]|=0x40;
	}
	//��AFN0CF170���г���������Ϣ�м������x~y�γ���ʧ�ܱ���Ϣ
	x=0;
	for(i=0;i<MaxRS485AddCarrierPn;i++)
	{
		p8=(u8 *)(ADDR_AFN0CF170+(18*i));
		if(p8[4]==0)
		{//���һ�γ���ɹ�/ʧ�ܱ�־��1/0��ʾ�ɹ�/ʧ�ܡ�
			if(x==((p8rxbuff[18]+(p8rxbuff[19]<<8))+n*UARTCtrl->TxFrameCount))
			{
				break;
			}
			x++;
		}
	}
	if(i>=MaxRS485AddCarrierPn)
	{//����ʼ���~�������֮��δ�ҵ�ʧ�ܱ�
		//��֡����֡
		p8txbuff[13]|=0x20;
		UARTCtrl->TxFrameCount=0;
		//���ո���
		x=0;
		goto TX;
	}
	x=0;
	for(y=i;y<MaxRS485AddCarrierPn;y++)
	{
		p8=(u8 *)(ADDR_AFN0CF170+(18*y));
		if(p8[4]!=0)
		{//���һ�γ���ɹ�/ʧ�ܱ�־��1/0��ʾ�ɹ�/ʧ�ܡ�
			continue;
		}
		//������Ϣ���
		p8txbuff[18*x+20]=(x+n*UARTCtrl->TxFrameCount);
		p8txbuff[18*x+21]=((x+n*UARTCtrl->TxFrameCount)>>8);
		z=Check_AFN04F10_Pn(y+1);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		//������š�ͨ�����ʼ��˿ںš�ͨ��Э�����͡�ͨ�ŵ�ַ
		MR((u32)p8txbuff+18*x+22,ADDR_AFN04F10+LEN_AFN04F10_Pn*(z-1)+2,10);
		//�����������
		MR((u32)p8txbuff+18*x+22+10,ADDR_AFN04F10+LEN_AFN04F10_Pn*(z-1)+2+8,6);
		x++;
		if(x==n)
		{
			break;
		}
		if(x==((p8rxbuff[20]+(p8rxbuff[21]<<8))-(p8rxbuff[18]+(p8rxbuff[19]<<8)+n*UARTCtrl->TxFrameCount)+1))
		{
			break;
		}
	}

	//���ո���
	p8txbuff[18]=x;
	p8txbuff[19]=(x>>8);
	//����
	TXlen=12+2+18*x;
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ

	UARTCtrl->TxFrameCount++;
	//����Ӧ֡���RSEQ+1
	i=p8rxbuff[13];
	x=i;
	i&=0xf0;
	x++;
	x&=0x0f;
	i|=x;
	p8rxbuff[13]=i;
	UARTCtrl->Task=5;//5=�ȴ���֡���ݷ���
	return;
}

#endif

#if (USER/100)==12//�Ĵ��û�
void Uart_AFN0CF15(u32 PORTn)//�Ĵ���չ��ȡSIM���ź�ǿ��
{
	u32 i;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0c;//AFN
//�û������ݱ�ʶ����
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(DIlen<(12+6))
		{
err:
			//ȫ������
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
			Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
		if(i!=0x0)
		{
			return;//�����ñ���
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	TXlen=12+2;
	//����
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	i=Terminal_Ram->SignaldBm;//�ź�ǿ��0-31
	if(i==0x99)
	{
		i=0xeeee;//0x99δ֪���޷���ȡ
	}
	else
	{
		i=bcd_hex(i);//8λѹ��BCD��תΪHEX
		if(i>31)
		{
			i=31;
		}
		i*=2;
		i=113-i;
		i=hex_bcd(i);
		i|=0x1000;
	}

	p8txbuff[18]=i;
	p8txbuff[19]=(i>>8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}
void Uart_AFN0CF2040(u32 PORTn)//2016��������б��Ĵ���չ��ȡԶ��ģ���ź�ǿ��P0
{
	u32 i;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u64 z;
	
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0c;//AFN
//�û������ݱ�ʶ����
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(DIlen<(12+6))
		{
err:
			//ȫ������
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
			Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
		if(i!=0x0)
		{
			return;//�����ñ���
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	TXlen=12+27;
	//����
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	z=MRR(ADDR_AFN0CF2040+1,6);
	if(z==0)//���绰�����޷���ȡʱȫΪE��EE EE EE EE EE EE��
	{
		MC(0xee,ADDR_AFN0CF2040+1,6);
	}
	MW(ADDR_AFN0CF2040,((u32)p8txbuff)+18,27);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}

void Uart_AFN0CF305(u32 PORTn)//͸������ִ��״̬P0 201703
{
	u32 i;
	u32 x;
	u32 byte;
	u32 bin;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8* p8;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0c;//AFN
//�û������ݱ�ʶ����
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(DIlen<(12+6))
		{
err:
			//ȫ������
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
			Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
		if(i!=0x0)
		{
			return;//�����ñ���
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
	TXlen=12+255;
	//����
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
//---------------------------��1008���������ִ��״̬��Ϊ126���ֽ�
	MR(ADDR_DATABUFF,ADDR_AFN0CF305,LEN_AFN0CF305);
	p8=(u8*)ADDR_DATABUFF;
	byte=0;
	i=0;
	while(i<LEN_AFN0CF305)
	{
		x=0;
		for(bin=0;bin<8;bin++)
		{
			if(p8[i]>=0xFE)
			{
				x|=1<<bin;
			}
			i++;
		}
		p8[byte]=x;
		byte++;
	}
	#if (LEN_AFN0CF305/8)>255
		#error
	#endif
	MC(0xFF,ADDR_DATABUFF+(LEN_AFN0CF305/8),255-(LEN_AFN0CF305/8));
//---------------------------
	MW(ADDR_DATABUFF,((u32)p8txbuff)+18,255);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}

void Uart_AFN0CF306(u32 PORTn)//͸������ִ�н��Pn 201703
{
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 LEN_data;
	u32 Pn;
	u32 TXlen;
	u32 DIlen;
	u32 TXaddr;
	u32 LEN_UARTnTx;
//	u32 RXDIaddr;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0c;//AFN
//�û������ݱ�ʶ����
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(DIlen<(12+6))
		{
err:
			//ȫ������
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
			Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
		if(i!=0x0)
		{
			return;//�����ñ���
		}
	}
	else
	{
		if(DIlen<12)
		{
			goto err;
		}
	}
//�û������ݱ�ʶ�Ͳ������ݳ���
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
//	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	TXaddr=(u32)p8txbuff+14;
	TXlen=8;
	while(DIlen>4)
	{
		LEN_data=0;
		if((TXlen+4+14+8+2+9)>LEN_UARTnTx)
		{
			goto err;
		}
		MR(TXaddr,(u32)p8rxbuff+14,4);
		TXaddr+=4;
		TXlen+=4;
		CopyDI(PORTn,(u32)p8rxbuff+14);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�;�����ݱ�ʶΪȫ����Ч��������Ŀ��DI��2�ֽ�Pn������Ϊ��Hex��1��ʼ
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		Pn=i&0xffff;
		if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
		{
			goto err;
		}
		#if (LEN_AFN0CF306>LEN_DATABUFF)
			#error LEN_AFN0CF306 Over
		#endif
//----------------------------------���͸������ͬʱ��͸������ִ�н��		
			i=MRR(ADDR_AFN0CF305+(Pn-1),1);
			if(i==0xFE)
			{
				MC(0,ADDR_DATABUFF,LEN_AFN0CF306);
			}
			else
			{
				MR(ADDR_DATABUFF,ADDR_AFN0CF306+(Pn-1)*LEN_AFN0CF306,LEN_AFN0CF306);
			}
//----------------------------------		
//		MR(ADDR_DATABUFF,ADDR_AFN0CF306+(Pn-1)*LEN_AFN0CF306,LEN_AFN0CF306);
		i=MRR(ADDR_AFN10F306+(Pn-1)*LEN_AFN10F306,2);//ȡ������
		if(i==0)
		{//����������������ʱ�������š����ʱ�̡���֡���ĸ�����Ϊ00
			MC(0,TXaddr,9);
			TXlen+=9;
			LEN_data=p8rxbuff[18];//��ѯ����
			LEN_data+=5;
		}
		else
		{
			if((TXlen+4+14+8+2+9)>LEN_UARTnTx)
			{
				goto err;
			}
			p8txbuff=(u8*)TXaddr;
			MWR(i,TXaddr,2);//������
			TXaddr+=2;
			TXlen+=2;
			MW(ADDR_DATABUFF+2,TXaddr,6);//�������ʱ��
			TXaddr+=6+1;
			TXlen+=6+1;
			j=0;//��֡���ĸ���
			x=p8rxbuff[18];//��ѯ����
			LEN_data+=x+5;
			if(x==0)
			{//=0��ѯ���������б���ִ�н��
				for(i=0;i<15;i++)
				{
					if((TXlen+14+2+169)>LEN_UARTnTx)
					{
						goto err;
					}
					x=MRR(ADDR_DATABUFF+9+i*169,1);//�������i
					if(x!=0)
					{
						y=MRR(ADDR_DATABUFF+9+i*169+2,1);//����i����
						if(y>166)
						{//���ĳ��ȳ���166�ֽ�
							goto err;
						}
						MR(TXaddr,ADDR_DATABUFF+9+(x-1)*169,(y+3));
						TXaddr+=(y+3);
						TXlen+=(y+3);
						j++;//��֡���ĸ���
					}
				}
				p8txbuff[8]=j;//��֡���ĸ���
			}
			else
			{//!=0��ѯָ�����ģ�����ѯ�������������m
				i=MRR(ADDR_AFN10F306+(Pn-1)*LEN_AFN10F306+5,1);//ȡ����������
				if(x>i)
				{
					x=i;
				}
				for(i=0;i<x;i++)
				{
					if((TXlen+14+2+169)>LEN_UARTnTx)
					{
						goto err;
					}
					y=MRR(((u32)p8rxbuff)+18+1+i,1);//�������i
					if((MRR(ADDR_DATABUFF+9+(y-1)*169,1))==0)//�������
					{
						break;
					}
					z=MRR(ADDR_DATABUFF+9+(y-1)*169+2,1);//����i����
					if(z>166)
					{//���ĳ��ȳ���166�ֽ�
						goto err;
					}
					MR(TXaddr,ADDR_DATABUFF+9+(y-1)*169,(z+3));
					TXaddr+=(z+3);
					TXlen+=(z+3);
					j++;//��֡���ĸ���
				}
				p8txbuff[8]=j;//��֡���ĸ���
			}
		}
		p8rxbuff+=LEN_data;
		if(DIlen>LEN_data)
		{
			DIlen-=LEN_data;
		}
		else
		{
			DIlen=0;
		}
	}
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}
#endif



