
//uart��2����
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_Uart_TimerTask.h"
#include "KeyUser.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "Terminal_ReadMeter_AFN0D_RealTime.h"

#include "../Terminal/Terminal_USER5xx.h"





extern Terminal_Class2DataLib_TypeDef Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);

/*
u32 TINUM(u32 PORTn,u32 ALLPn,u32 Fn,u32 Pn)//��ǰ���ݱ�ʶ��Ӧǰ��ʱ���������;���أ�ǰ��ʱ�����
{
	u32 i;
	u32 x;
	u32 y;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ

	if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
	{//�����ݱ�ʶΪȫ����Ч������
		if(Pn==0)
		{//p0��Fn���
			i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//������λΪ1�ĸ���(0-32)
		}
		else
		{//��p0��Fn���
			i=MRR(((u32)(&UARTCtrl->SourDI)),1);//����ͬ����Pn��ϱ�Pnǰ��Pn����
			x=(Pn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//������λΪ1�ĸ���(0-32)
			x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϸ���
			x=BitNum(x);//������λΪ1�ĸ���(0-32)
			i*=x;
			y=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			x&=y;
			x=BitNum(x);//������λΪ1�ĸ���(0-32)
			i+=x;
		}
	}
	else
	{
		i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
		x=(Fn-1)%8;
		x=0xff<<x;
		x=~x;
		i&=x;
		i=BitNum(i);//������λΪ1�ĸ���(0-32)	
	}
	return i;
}	
*/




void Uart_AFN0D(u32 PORTn)//��2����
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 MapPn;
	u32 RXDIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
	u32 OfficeAddr;
	u32 m;
	u32 FnLen;
  u32 LEN_UARTnTx;
	u32 SourDI;//���ηֽ�ǰ��DI
	u32 TIlen;//ʱ�곤��
	u32 TIOffice;//ʱ��ƫ��
	u32 ALLPn;//ȫ����Ч�����־��0=��,1=��
	u32 PointNum;//������������뻺�����ĵ���
	u64 YMDHM;
	u64 NextYMDHM;
	u16 *p16;
	u32 DataBuffDate;//��ǰUART���ݻ�����붳�����ݵ���������
#if MaxACSampleCurveCongeal!=0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
	u32 ACSamplePn;
#endif
#if (USER/100)==11//���� 
	u32 port;
#endif
#if (USER/100)==12//�Ĵ� 
	u8 * p8;	
#endif
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	DataBuffDate=0;//��ǰUART���ݻ�����붳�����ݵ���������
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ
  LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0d;//AFN(Ϊ��ʱ�ϱ�֡��д��
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
		MW(RXDIaddr+4,(u32)&UARTCtrl->TI,7);//�Ĵ�ȫ��������ʱ���ߵ�ʱ��(������ʱ��),�ܶ�,����
	}
	RXDIaddr+=UARTCtrl->DIOffset;
	RXaddr=RXDIaddr+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=TXDIaddr+4;
	TXlen=0;
	TIlen=0;//ʱ�곤��
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
	
#if MaxKeyUser!=0//����г����ص��û���
	KeyUserPnList();//���г����ص㻧���߶���Pn�б�
#endif

	while(DIlen>=4)
	{
		SourDI=MRR((u32)(&UARTCtrl->DestDI),4);//���ηֽ�ǰ��DI
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		if(i==0x0)//���ݱ�ʶ�ֽ�ΪFnPn
		{
			if(ALLPn==0)//ȫ����Ч�����־��0=��,1=��
			{
				//����ʱ�곤��,ÿFn��ÿPnһ��ʱ��
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//����Pn��ϸ���
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
				if(i==0)
				{
					i=1;//p0��
				}
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����Fn��ϸ���
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i*=x;//�ܸ���
			}
			else
			{//�����ݱ�ʶΪȫ����Ч������
				//����ʱ�곤��,ÿFnһ��ʱ��
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����Fn��ϸ���
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
			}
			if(TIlen==0)//ʱ�곤��
			{//ǰ�滹û��һ����ʶ��ȷ��,δ֪����ʶʱ�곤��
				goto err;
			}
		#if(USER/100)==7//�㽭�û�
			if(UARTCtrl->LockFlags&0x30)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B5-B7����
			{//������ʱ����
				i=TIlen;//ʱ�곤��
			}
			else
			{
				i*=TIlen;//ʱ�곤��
			}
		#else
			i*=TIlen;//ʱ�곤��
		#endif
			RXaddr+=i;//���յ�ַ+��ʱ�곤��
			if(DIlen>=(i+4))
			{
				DIlen-=i;
			}
			else
			{
				goto err;
			}
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
				MW(RXaddr+4,(u32)&UARTCtrl->TI,7);//�Ĵ�ȫ��������ʱ���ߵ�ʱ��(������ʱ��),�ܶ�,����
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

	#if AFN0DF1MaxPn!=0
		if(ALLPn!=0)//ȫ����Ч�����־;0=��,1=��
		{
			i=Terminal_Class2DataLib[Fn].PnMax;
			if(Fn==246)
			{//��ǰ�����¼����(Pnӳ��)
				i=MaxRS485AddCarrierPn;
			}
			if(Pn>i)
			{//ȫ��Pn��
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			switch(Terminal_Class2DataLib[Fn].PnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
			{
//				case p0Type://�ն���Ϣ��
//					break;
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
//				case CnType://�������������
//				case DnType://ֱ��ģ������
//				case RnType://�����ִκ�
//				case TnType://�����
//				case EnType;//�¼���
//					break;
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
			switch(Terminal_Class2DataLib[Fn].PnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
			{
//				case p0Type://�ն���Ϣ��
//					break;
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
//				case CnType://�������������
//				case DnType://ֱ��ģ������
//				case RnType://�����ִκ�
//				case TnType://�����
//				case EnType;//�¼���
//					break;
			}
		}
	#else//#if AFN0DF1MaxPn!=0
		if(ALLPn!=0)//ȫ����Ч�����־;0=��,1=��
		{
			if((GetClass2DataCongealType(Fn)==3)&&(Terminal_Class2DataLib[Fn].PnType==pnType))//��������������
			{//��������������
				if(Pn>MaxRS485AddCarrierPn)
				{//ȫ��Pn��
					MC(0,(u32)(&UARTCtrl->DestDI),4);
					continue;
				}
			}
			else
			{//
				i=Terminal_Class2DataLib[Fn].PnMax;
				if(Fn==246)
				{//��ǰ�����¼����(Pnӳ��)
					i=MaxRS485AddCarrierPn;
				}
				//����Fn�ڼ��г���Ϊ��ʡFLASH��MaxPn��Ϊ0,��������������ϳ�
				switch(Fn)
				{
				//2������.�ն���
					case 1://F1������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 2://F2�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 3://F3������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					case 4://F4�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				#if AFN0DF5MaxPn==0
					case 5://�����������й����������ܡ�����1��M��
					case 6://�����������޹����������ܡ�����1��M��
					case 7://�����㷴���й����������ܡ�����1��M��
					case 8://�����㷴���޹����������ܡ�����1��M��
				#endif
				//2������.�����ն���
					case 9://F9������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 10://F10�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 11://F11������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					case 12://F12�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				//2������.�¶���
					case 17://F17������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 18://F18�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 19://F19������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					case 20://F20�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
				#if AFN0DF21MaxPn==0
					case 21://�����������й����������ܡ�����1��M��
					case 22://�����������޹����������ܡ�����1��M��
					case 23://�����㷴���й����������ܡ�����1��M��
					case 24://�����㷴���޹����������ܡ�����1��M��
				#endif
				/*
				#if AFN0DF97MaxPn==0
					case 97://�����й��ܵ���������
					case 98://�����޹��ܵ���������
					case 99://�����й��ܵ���������
					case 100://�����޹��ܵ���������
				#endif
				*/
						i=AFN0DF161MaxPn;
						break;
				
				}
				if(Pn>i)
				{//ȫ��Pn��
					MC(0,(u32)(&UARTCtrl->DestDI),4);
					continue;
				}
				
			}
			switch(Terminal_Class2DataLib[Fn].PnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
			{
//				case p0Type://�ն���Ϣ��
//					break;
				case pnType://�������
					if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
					{
						continue;
					}
					if((GetClass2DataCongealType(Fn)==3)&&(Terminal_Class2DataLib[Fn].PnType==pnType))//��������������
					{//��������������
						if(KeyUserPnSet(Pn)==0)//�ص㻧��Ч������ż��;����0=��Ч,1=��Ч;ʹ���ص㻧���߶���Pn�б�ADDR_KeyUserPnList
						{
							continue;
						}
					}
					break;
				case GnType://�ܼ����
					if(GroupTotalSet(Pn)==0)//��Ч�ܼ���ż��;����0=��Ч,1=��Ч
					{
						continue;
					}
					break;
//				case CnType://�������������
//				case DnType://ֱ��ģ������
//				case RnType://�����ִκ�
//				case TnType://�����
//				case EnType;//�¼���
//					break;
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
			switch(Terminal_Class2DataLib[Fn].PnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
			{
//				case p0Type://�ն���Ϣ��
//					break;
				case pnType://�������
					if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
					{
						continue;
					}
					if((GetClass2DataCongealType(Fn)==3)&&(Terminal_Class2DataLib[Fn].PnType==pnType))//��������������
					{//��������������
						if(KeyUserPnSet(Pn)==0)//�ص㻧��Ч������ż��;����0=��Ч,1=��Ч;ʹ���ص㻧���߶���Pn�б�ADDR_KeyUserPnList
						{
							continue;
						}
					}
					break;
				case GnType://�ܼ����
					if(GroupTotalSet(Pn)==0)//��Ч�ܼ���ż��;����0=��Ч,1=��Ч
					{
						continue;
					}
					break;
//				case CnType://�������������
//				case DnType://ֱ��ģ������
//				case RnType://�����ִκ�
//				case TnType://�����
//				case EnType;//�¼���
//					break;
			}
		}
	#endif



		switch(GetClass2DataCongealType(Fn))//ȡ��2���ݶ�������:0=�ն���,1=�����ն���,2=�¶���,3=����
		{
			case 0://0=�ն���
			case 1://1=�����ն���
				TIlen=3;//ʱ�곤��
				break;
			case 2://1=�¶���
				TIlen=2;//ʱ�곤��
				break;
			case 3://1=����
				TIlen=7;//ʱ�곤��
				break;
			default:
				goto err;
		}
		if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
		{//�����ݱ�ʶΪȫ����Ч������
			if(Pn==0)
			{//p0��Fn���
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
			}
			else
			{//��p0��Fn���
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//����ͬ����Pn��ϱ�Pnǰ��Pn����
				x=(Pn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϸ���
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i*=x;
				y=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				x&=y;
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i+=x;
			}
		}
		else
		{
			i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//������λΪ1�ĸ���(0-32)
		}
	#if(USER/100)==7//�㽭�û�
		if(UARTCtrl->LockFlags&0x30)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B5-B7����
		{//������ʱ����
			TIOffice=0;//ʱ��ƫ��
		}
		else
		{
			TIOffice=i*TIlen;//ʱ��ƫ��
		}
	#else
		TIOffice=i*TIlen;//ʱ��ƫ��
	#endif
		if(DIlen<(TIOffice+TIlen+4))
		{
			DIlen=0;
			continue;
		}
		
	#if AFN0DF1MaxPn!=0
		if(Fn==219)//F219 �����������������
		{//Fn=219�����������������
			if((Pn==0)||(Pn>Terminal_Class2DataLib[Fn].PnMax))
			{
				continue;
			}
		}
		else
		{//��Fn=219�����������������
			OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
			if(OfficeAddr==0x0)
			{
				continue;
			}
		}
	#else
		if(Fn==219)//F219 �����������������
		{//Fn=219�����������������
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				continue;
			}
		}
		else
		{//��Fn=219�����������������
			if((GetClass2DataCongealType(Fn)==3)&&(Terminal_Class2DataLib[Fn].PnType==pnType))//��������������
			{//��������������
				OfficeAddr=GetClass2DataOfficeAddr(Fn,1);//�������ӳ���ȹ̶���Pn=1
				if(OfficeAddr==0x0)
				{
					continue;
				}
			}
			else
			{
				OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
				if(OfficeAddr==0x0)
				{
					//����Fn�ڼ��г���Ϊ��ʡFLASH��MaxPn��Ϊ0,��������������ϳ�
					switch(Fn)
					{
					//2������.�ն���
						case 1://F1������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						case 2://F2�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						case 3://F3������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						case 4://F4�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					#if AFN0DF5MaxPn==0
						case 5://F5�����������й����������ܡ�����1��M��
						case 6://F6�����������޹����������ܡ�����1��M��
						case 7://F7�����㷴���й����������ܡ�����1��M��
						case 8://F8�����㷴���޹����������ܡ�����1��M��
					#endif
					//2������.�����ն���
						case 9://F9������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						case 10://F10�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						case 11://F11������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						case 12://F12�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					//2������.�¶���
						case 17://F17������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						case 18://F18�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						case 19://F19������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						case 20://F20�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					#if AFN0DF21MaxPn==0
						case 21://F21�����������й����������ܡ�����1��M��
						case 22://F22�����������޹����������ܡ�����1��M��
						case 23://F23�����㷴���й����������ܡ�����1��M��
						case 24://F24�����㷴���޹����������ܡ�����1��M��
					#endif
					/*
					#if AFN0DF97MaxPn==0
						case 97://�����й��ܵ���������
						case 98://�����޹��ܵ���������
						case 99://�����й��ܵ���������
						case 100://�����޹��ܵ���������
					#endif
					*/
							break;
				#if (USER/100)==5//�Ϻ�
						case 235://�ն�AB�ߵ�ѹ���ߣ���չ��
							OfficeAddr=ADDR_AFN0DF235;
							break;
						case 236://�ն�BC�ߵ�ѹ���ߣ���չ��
							OfficeAddr=ADDR_AFN0DF236;
							break;
						case 237://�ն�AC�ߵ�ѹ���ߣ���չ��
							OfficeAddr=ADDR_AFN0DF237;
							break;
				#endif
						default:
							continue;	
					}
				}
			}
		}
	#endif
		
		switch(GetClass2DataCongealType(Fn))//ȡ��2���ݶ�������:0=�ն���,1=�����ն���,2=�¶���,3=����
		{
			case 0://0=�ն���
			#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				//��1��
				x=MRR(ADDR_TYMDHMS+3,3);
				x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
				if(MRR(RXaddr+TIOffice,3)==x)
				{//ʱ��=(��ǰʵʱ�ӵ���1��)
					i=GetDateCongealNo(x);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
					i=PnDateCongealNo(i,Pn);//�������ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
					if(i!=0xffffffff)
					{//����1�ն���
						if(Fn==246)
						{//�ն�������¼����(������ӳ��)
							OfficeAddr=GetClass2DataF246OfficeAddr(i,Pn);//ȡ��2����F246����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
							if(OfficeAddr==0)
							{//û�ҵ�
								continue;
							}
						}
						if(MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+ADDR_DATECONGEALFLAG,1)==0xff)
						{//�����־�ǲ�����,ֻ�е��Ĳ�������Ჹ,�����ǵ��������������ݱ��⽫0xffתΪ0xee
							if(Terminal_Class2DataLib[Fn].PnType!=pnType)
							{//���ǲ�������
								continue;
							}
						}
						if(DataBuffDate!=x)//��ǰUART���ݻ�����붳�����ݵ���������
						{
							DataBuffDate=x;//��ǰUART���ݻ�����붳�����ݵ���������
							MR(ADDR_AFN0D_UART_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//��ԭ�����ն�������
							x=MRR(ADDR_TYMDHMS+3,3);//��Ч����ʱ��
							Source_Full_CongealBuff(ADDR_AFN0D_UART_Buff_Start,0,0,x,0);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
						}
						Class2_Group(ADDR_AFN0D_UART_Buff_Start,Fn,Pn);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
						
						x=GetClass2DataLen(Fn,Pn,ADDR_AFN0D_UART_Buff_Start+OfficeAddr);//ȡ��2���ݳ���
						if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
						{
							if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
							{
								TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
							}
							MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
							goto MulTx;//����-���к���֡
						}
						MR(TXaddr,ADDR_AFN0D_UART_Buff_Start,3);//�ȶ�3�ֽڶ���ʱ��
						
						//����Fn�ڼ��г���Ϊ��ʡFLASH��MaxPn��Ϊ0,��������������ϳ�
						switch(Fn)
						{
						//2������.�ն���
							case 1://F1������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 2://F2�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 3://F3������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
							case 4://F4�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
								MR(TXaddr+3,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
								break;
							default:
								MR(TXaddr+3,ADDR_AFN0D_UART_Buff_Start+OfficeAddr,x);//�ٶ�x�ֽڶ�������
								break;
						}
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//�ǲ�������
							Data_0xFFto0xEE(TXaddr+3,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						}
						//��ǰȡ��2�����ݵķ�������г������������
						i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
						if(i&0x10000)
						{//������
							MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
						}
					#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
						{
							if(CheckUartData_0xEE(TXaddr+3,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
							{
								continue;
							}
						}
					#endif
						TXlen+=3+x;
						TXaddr+=3+x;
						break;
					}
					//û�ҵ�
					continue;
				}
				//��2��
				x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
				if(MRR(RXaddr+TIOffice,3)==x)
				{//ʱ��=(��ǰʵʱ�ӵ���2��)
					i=GetDateCongealNo(x);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
					i=PnDateCongealNo(i,Pn);//�������ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
					if(i!=0xffffffff)
					{//����2�ն���
						if(Fn==246)
						{//�ն�������¼����(������ӳ��)
							OfficeAddr=GetClass2DataF246OfficeAddr(i,Pn);//ȡ��2����F246����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
							if(OfficeAddr==0)
							{//û�ҵ�
								continue;
							}
						}
						if(MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+ADDR_DATECONGEALFLAG,1)==0xff)
						{//�����־�ǲ�����,ֻ�е��Ĳ�������Ჹ,�����ǵ��������������ݱ��⽫0xffתΪ0xee
							if(Terminal_Class2DataLib[Fn].PnType!=pnType)
							{//���ǲ�������
								continue;
							}
						}
						if(DataBuffDate!=x)//��ǰUART���ݻ�����붳�����ݵ���������
						{
							DataBuffDate=x;//��ǰUART���ݻ�����붳�����ݵ���������
							MR(ADDR_AFN0D_UART_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//��ԭ�����ն�������
							x=YMD_Add1D(x);//�����ռ�1��,���ؼ�1���������
							LastSource_Full_CongealBuff(ADDR_AFN0D_UART_Buff_Start,x);//��2-3�ճ���ԭʼ�������붳�Ỻ��;���:LastYMD=�����ʱ��;����:0=û��������,1=��
						}
						Class2_Group(ADDR_AFN0D_UART_Buff_Start,Fn,Pn);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
						
						x=GetClass2DataLen(Fn,Pn,ADDR_AFN0D_UART_Buff_Start+OfficeAddr);//ȡ��2���ݳ���
						if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
						{
							if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
							{
								TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
							}
							MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
							goto MulTx;//����-���к���֡
						}
						MR(TXaddr,ADDR_AFN0D_UART_Buff_Start,3);//�ȶ�3�ֽڶ���ʱ��
						
						//����Fn�ڼ��г���Ϊ��ʡFLASH��MaxPn��Ϊ0,��������������ϳ�
						switch(Fn)
						{
						//2������.�ն���
							case 1://F1������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 2://F2�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 3://F3������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
							case 4://F4�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
								MR(TXaddr+3,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
								break;
							default:
								MR(TXaddr+3,ADDR_AFN0D_UART_Buff_Start+OfficeAddr,x);//�ٶ�x�ֽڶ�������
								break;
						}
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//�ǲ�������
							Data_0xFFto0xEE(TXaddr+3,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						}
						//��ǰȡ��2�����ݵķ�������г������������
						i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
						if(i&0x10000)
						{//������
							MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
						}
					#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
						{
							if(CheckUartData_0xEE(TXaddr+3,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
							{
								continue;
							}
						}
					#endif
						TXlen+=3+x;
						TXaddr+=3+x;
						break;
					}
					//û�ҵ�
					continue;
				}
				//��3��
				x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
				if(MRR(RXaddr+TIOffice,3)==x)
				{//ʱ��=(��ǰʵʱ�ӵ���3��)
					i=GetDateCongealNo(x);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
					i=PnDateCongealNo(i,Pn);//�������ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
					if(i!=0xffffffff)
					{//����3�ն���
						if(Fn==246)
						{//�ն�������¼����(������ӳ��)
							OfficeAddr=GetClass2DataF246OfficeAddr(i,Pn);//ȡ��2����F246����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
							if(OfficeAddr==0)
							{//û�ҵ�
								continue;
							}
						}
						if(MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+ADDR_DATECONGEALFLAG,1)==0xff)
						{//�����־�ǲ�����,ֻ�е��Ĳ�������Ჹ,�����ǵ��������������ݱ��⽫0xffתΪ0xee
							if(Terminal_Class2DataLib[Fn].PnType!=pnType)
							{//���ǲ�������
								continue;
							}
						}
						if(DataBuffDate!=x)//��ǰUART���ݻ�����붳�����ݵ���������
						{
							DataBuffDate=x;//��ǰUART���ݻ�����붳�����ݵ���������
							MR(ADDR_AFN0D_UART_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//��ԭ�����ն�������
							x=YMD_Add1D(x);//�����ռ�1��,���ؼ�1���������
							LastSource_Full_CongealBuff(ADDR_AFN0D_UART_Buff_Start,x);//��2-3�ճ���ԭʼ�������붳�Ỻ��;���:LastYMD=�����ʱ��;����:0=û��������,1=��
						}
						Class2_Group(ADDR_AFN0D_UART_Buff_Start,Fn,Pn);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
						
						x=GetClass2DataLen(Fn,Pn,ADDR_AFN0D_UART_Buff_Start+OfficeAddr);//ȡ��2���ݳ���
						if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
						{
							if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
							{
								TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
							}
							MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
							goto MulTx;//����-���к���֡
						}
						MR(TXaddr,ADDR_AFN0D_UART_Buff_Start,3);//�ȶ�3�ֽڶ���ʱ��
						
						//����Fn�ڼ��г���Ϊ��ʡFLASH��MaxPn��Ϊ0,��������������ϳ�
						switch(Fn)
						{
						//2������.�ն���
							case 1://F1������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 2://F2�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 3://F3������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
							case 4://F4�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
								MR(TXaddr+3,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
								break;
							default:
								MR(TXaddr+3,ADDR_AFN0D_UART_Buff_Start+OfficeAddr,x);//�ٶ�x�ֽڶ�������
								break;
						}
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//�ǲ�������
							Data_0xFFto0xEE(TXaddr+3,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						}
						//��ǰȡ��2�����ݵķ�������г������������
						i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
						if(i&0x10000)
						{//������
							MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
						}
					#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
						{
							if(CheckUartData_0xEE(TXaddr+3,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
							{
								continue;
							}
						}
					#endif
						TXlen+=3+x;
						TXaddr+=3+x;
						break;
					}
					//û�ҵ�
					continue;
				}
			#endif//#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				
				//������
				DataBuffDate=MRR(RXaddr+TIOffice,3);
				i=DataBuffDate;
			#if UartDateTI==1//�ն�����ٶ�ʱ��0=���յ�24ʱ����,1=���յ�0ʱ����
				i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
			#endif
				i=GetDateCongealNo(i);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
				i=PnDateCongealNo(i,Pn);//�������ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
				if(i==0xffffffff)
				{//û�ҵ�
				#if (USER/100)==5//�Ϻ�
				NOF246:
					//��ʷ���ݶ�ʧӦ��ʽ������"��ȷ��"֡Ӧ��ͳһʹ����Ч����0xEEӦ��ʱ��ʹ�ñ������ʱ�꣬����������д����
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//���ǲ�������
						switch(Fn)
						{
							case 243://�ն����ź�ǿ��
								break;
							default:
								continue;
						}
					}
					MC(0xee,ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
					x=GetClass2DataLen(Fn,Pn,ADDR_DATABUFF);//ȡ��2���ݳ���
					if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
					{
						if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
						{
							TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
						}
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
						goto MulTx;//����-���к���֡
					}
					MWR(DataBuffDate,TXaddr,3);//3�ֽڶ���ʱ��
					MR(TXaddr+3,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
					//��ǰȡ��2�����ݵķ�������г������������
					i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
					if(i&0x10000)
					{//������
						MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
					}
					TXlen+=3+x;
					TXaddr+=3+x;
					break;
				#else//#if (USER/100)==5//�Ϻ�
					continue;
				#endif
				}
				if(Fn==246)
				{//�ն�������¼����(������ӳ��)
					OfficeAddr=GetClass2DataF246OfficeAddr(i,Pn);//ȡ��2����F246����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
					if(OfficeAddr==0)
					{//û�ҵ�
				#if (USER/100)==5//�Ϻ�
					//��ʷ���ݶ�ʧӦ��ʽ������"��ȷ��"֡Ӧ��ͳһʹ����Ч����0xEEӦ��ʱ��ʹ�ñ������ʱ�꣬����������д����
						goto NOF246;
				#else
						continue;
				#endif
					}
				}
				if(MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+ADDR_DATECONGEALFLAG,1)==0xff)
				{//�����־�ǲ�����,ֻ�е��Ĳ�������Ჹ,�����ǵ��������������ݱ��⽫0xffתΪ0xee
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//���ǲ�������
						continue;
					}
				}
			#if AFN0DF1MaxPn==0
				Class2_Group(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),Fn,Pn);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
			#endif
				x=GetClass2DataLen(Fn,Pn,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr);//ȡ��2���ݳ���
				if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
				{
					if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
					{
						TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
					goto MulTx;//����-���к���֡
				}
				MWR(DataBuffDate,TXaddr,3);//3�ֽڶ���ʱ��
		
			#if AFN0DF1MaxPn==0
				//����Fn�ڼ��г���Ϊ��ʡFLASH��MaxPn��Ϊ0,��������������ϳ�
				switch(Fn)
				{
				//2������.�ն���
					case 1://F1������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 2://F2�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 3://F3������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					case 4://F4�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						MR(TXaddr+3,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
						break;
				#if AFN0DF5MaxPn==0
					case 5://�����������й����������ܡ�����1��M��
						UART_AFN0DF5F6F7F8_Computer(TXaddr+3,DataBuffDate,161,Pn,5);//2���ն������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
						break;
					case 6://�����������޹����������ܡ�����1��M��
						UART_AFN0DF5F6F7F8_Computer(TXaddr+3,DataBuffDate,162,Pn,4);//2���ն������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
						break;
					case 7://�����㷴���й����������ܡ�����1��M��
						UART_AFN0DF5F6F7F8_Computer(TXaddr+3,DataBuffDate,163,Pn,5);//2���ն������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
						break;
					case 8://�����㷴���޹����������ܡ�����1��M��
						UART_AFN0DF5F6F7F8_Computer(TXaddr+3,DataBuffDate,164,Pn,4);//2���ն������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
						break;
				#endif
					default:
					#if (USER/100)==12//�Ĵ�
					if(Fn==245)
					{
							MR(TXaddr+3,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr,x);//�ٶ�x�ֽڶ�������
							p8=(u8*)(TXaddr+3);
							if((p8[4]!=0xEE)&&(p8[4]!=0xFF))
							{
								p8[4]&=0x1f;
								p8[10]&=0x1f;
							}
					}
					else
					{
						MR(TXaddr+3,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr,x);//�ٶ�x�ֽڶ�������
					}
					#else
						MR(TXaddr+3,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr,x);//�ٶ�x�ֽڶ�������
					#endif
						break;
				}
			#else
				MR(TXaddr+3,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+OfficeAddr,x);//�ٶ�x�ֽڶ�������
			#endif
				if(Terminal_Class2DataLib[Fn].PnType==pnType)
				{//�ǲ�������
					Data_0xFFto0xEE(TXaddr+3,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
				}
				//��ǰȡ��2�����ݵķ�������г������������
				i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
				if(i&0x10000)
				{//������
					MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
				}
			#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
				if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
				{
					if(CheckUartData_0xEE(TXaddr+3,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
					{
						continue;
					}
				}
			#endif
				TXlen+=3+x;
				TXaddr+=3+x;
				break;
			case 1://1=�����ն���
				DataBuffDate=MRR(RXaddr+TIOffice,3);
			#if (USER/100)==11	   //���� �����ն���ʱ����ǰһ��
				port=MRR(ADDR_AFN04F10+((Pn-1)*LEN_AFN04F10_Pn)+4,1);
				port&=0x1f;//�˿ں�
				if(port==31)
				{//ͨ�Ŷ˿�31 �ز�����
					i=GetRMDateCongealNo(DataBuffDate);//�õ������ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
				}
				else
				{
					i=DataBuffDate;
					i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
					i=GetRMDateCongealNo(i);//�õ������ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
				}
			#else
				i=GetRMDateCongealNo(DataBuffDate);//�õ������ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
			#endif
				i=PnRMDateCongealNo(i,Pn);//�����㳭���ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
				if(i==0xffffffff)
				{//û�ҵ�
				#if (USER/100)==5//�Ϻ�
					//��ʷ���ݶ�ʧӦ��ʽ������"��ȷ��"֡Ӧ��ͳһʹ����Ч����0xEEӦ��ʱ��ʹ�ñ������ʱ�꣬����������д����
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//���ǲ�������
						continue;
					}
					MC(0xee,ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
					x=GetClass2DataLen(Fn,Pn,ADDR_DATABUFF);//ȡ��2���ݳ���
					if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
					{
						if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
						{
							TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
						}
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
						goto MulTx;//����-���к���֡
					}
					MWR(DataBuffDate,TXaddr,3);//3�ֽڶ���ʱ��
					MR(TXaddr+3,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
					//��ǰȡ��2�����ݵķ�������г������������
					if(Terminal_Class2DataLib[Fn].PnType==pnType)
					{//�ǲ�������
						Data_0xFFto0xEE(TXaddr+3,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
					}
					i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
					if(i&0x10000)
					{//������
						MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
					}
					TXlen+=3+x;
					TXaddr+=3+x;
					break;
				#else//#if (USER/100)==5//�Ϻ�
					continue;
				#endif
				}
			/*
				if(MRR(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+ADDR_RMDATECONGEALFLAG,1)==0xff)
				{//�����־�ǲ�����,ֻ�е��Ĳ�������Ჹ,�����ǵ��������������ݱ��⽫0xffתΪ0xee
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//���ǲ�������
						continue;
					}
				}
			*/
			#if AFN0DF1MaxPn==0
				switch(Fn)
				{
				//2������.�����ն���
					case 9://F9������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 10://F10�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 11://F11������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					case 12://F12�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						Class2_Group(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),Fn,Pn);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
						//�����ն��ᰴ�˿ںŷֿ�ʱ�ֶ��ᣬ��û����ʱ�������Ϊ0xee
						x=MRR(ADDR_DATABUFF,1);
						if(x==0xee)
						{//�޶���
							continue;
						}
				}
			#else
				//�����ն��ᰴ�˿ںŷֿ�ʱ�ֶ��ᣬ��û����ʱ�������Ϊ0xee
				x=MRR(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+OfficeAddr,1);
				if(x==0xee)
				{//�޶���
					continue;
				}
			#endif
				x=GetClass2DataLen(Fn,Pn,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+OfficeAddr);//ȡ��2���ݳ���
				if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
				{
					if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
					{
						TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
					goto MulTx;//����-���к���֡
				}
			#if (USER/100)==11 //����
				port=MRR(ADDR_AFN04F10+((Pn-1)*LEN_AFN04F10_Pn)+4,1);
				port&=0x1f;//�˿ں�
				if(port==31)
				{//ͨ�Ŷ˿�31 �ز�����
					MR(TXaddr,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3);//�ȶ�3�ֽڶ���ʱ��
				}
				else
				{
					MWR(DataBuffDate,TXaddr,3);//3�ֽڶ���ʱ��
				}
			#else
				MR(TXaddr,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3);//�ȶ�3�ֽڶ���ʱ��
			#endif
			#if AFN0DF1MaxPn==0
				switch(Fn)
				{
				//2������.�����ն���
					case 9://F9������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 10://F10�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 11://F11������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					case 12://F12�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						MR(TXaddr+3,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
						break;
					default:
						MR(TXaddr+3,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+OfficeAddr,x);//�ٶ�x�ֽڶ�������
						break;
				}
			#else
				MR(TXaddr+3,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+OfficeAddr,x);//�ٶ�x�ֽڶ�������
			#endif
				//��ǰȡ��2�����ݵķ�������г������������
				if(Terminal_Class2DataLib[Fn].PnType==pnType)
				{//�ǲ�������
					Data_0xFFto0xEE(TXaddr+3,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
				}
				i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
				if(i&0x10000)
				{//������
					MWR(i&0xff,TXaddr+3+((i>>8)&0xff),1);
				}
			#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
				if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
				{
					if(CheckUartData_0xEE(TXaddr+3,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
					{
						continue;
					}
				}
			#endif
				TXlen+=3+x;
				TXaddr+=3+x;
				break;
			case 2://2=�¶���
			#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				//��1��
				x=MRR(ADDR_TYMDHMS+4,2);
				x=YM_Sub1M(x);//���¼�1��,���ؼ�1�������
				if(MRR(RXaddr+TIOffice,2)==x)
				{//ʱ��=(��ǰʵʱ�ӵ���1��)
					if(MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+ADDR_MONTHCONGEALFLAG,1)==0xff)
					{//�����־�ǲ�����,ֻ�е��Ĳ�������Ჹ,�����ǵ��������������ݱ��⽫0xffתΪ0xee
						if(Terminal_Class2DataLib[Fn].PnType!=pnType)
						{//���ǲ�������
							continue;
						}
					}
					i=GetMonthCongealNo(x);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
					i=PnMonthCongealNo(i,Pn);//�������¶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
					if(i!=0xffffffff)
					{//����1�¶���
						if(DataBuffDate!=x)//��ǰUART���ݻ�����붳�����ݵ���������
						{
							DataBuffDate=x;//��ǰUART���ݻ�����붳�����ݵ���������
							MR(ADDR_AFN0D_UART_Buff_Start,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),LEN_MONTHCONGEAL);//��ԭ���¶�������
							x=MRR(ADDR_TYMDHMS+3,3);//��Ч����ʱ��
							Source_Full_CongealBuff(ADDR_AFN0D_UART_Buff_Start,2,0,x,0);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
						}
						Class2_Group(ADDR_AFN0D_UART_Buff_Start,Fn,Pn);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
						
						x=GetClass2DataLen(Fn,Pn,ADDR_AFN0D_UART_Buff_Start+OfficeAddr);//ȡ��2���ݳ���
						if((TXlen+4+14+8+2+(2+x))>LEN_UARTnTx)
						{
							if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
							{
								TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
							}
							MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
							goto MulTx;//����-���к���֡
						}
						MR(TXaddr,ADDR_AFN0D_UART_Buff_Start,2);//�ȶ�2�ֽڶ���ʱ��
						switch(Fn)
						{
						//2������.�¶���
							case 17://F17������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 18://F18�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
							case 19://F19������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
							case 20://F20�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
								MR(TXaddr+2,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
								break;
							default:
								MR(TXaddr+2,ADDR_AFN0D_UART_Buff_Start+OfficeAddr,x);//�ٶ�x�ֽڶ�������
								break;
						}
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//�ǲ�������
							Data_0xFFto0xEE(TXaddr+2,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						}
						//��ǰȡ��2�����ݵķ�������г������������
						i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
						if(i&0x10000)
						{//������
							MWR(i&0xff,TXaddr+2+((i>>8)&0xff),1);
						}
					#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
						{
							if(CheckUartData_0xEE(TXaddr+2,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
							{
								continue;
							}
						}
					#endif
						TXlen+=2+x;
						TXaddr+=2+x;
						break;
					}
					//û�ҵ�
					continue;
				}
			#endif//#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
				//������
				DataBuffDate=MRR(RXaddr+TIOffice,2);
				i=DataBuffDate;
			#if UartMonthTI==1//�¶�����ٶ�ʱ��0=����δ��24ʱ����,1=���³�1�յ�0ʱ����
				i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
			#endif
				i=GetMonthCongealNo(i);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
				i=PnMonthCongealNo(i,Pn);//�������¶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
				if(i==0xffffffff)
				{//û�ҵ�
				#if (USER/100)==5//�Ϻ�
					//��ʷ���ݶ�ʧӦ��ʽ������"��ȷ��"֡Ӧ��ͳһʹ����Ч����0xEEӦ��ʱ��ʹ�ñ������ʱ�꣬����������д����
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//���ǲ�������
						continue;
					}
					MC(0xee,ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
					x=GetClass2DataLen(Fn,Pn,ADDR_DATABUFF);//ȡ��2���ݳ���
					if((TXlen+4+14+8+2+(2+x))>LEN_UARTnTx)
					{
						if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
						{
							TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
						}
						MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
						goto MulTx;//����-���к���֡
					}
					MWR(DataBuffDate,TXaddr,2);//2�ֽڶ���ʱ��
					MR(TXaddr+2,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
					//��ǰȡ��2�����ݵķ�������г������������
					i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
					if(i&0x10000)
					{//������
						MWR(i&0xff,TXaddr+2+((i>>8)&0xff),1);
					}
					TXlen+=2+x;
					TXaddr+=2+x;
					break;
				#else//#if (USER/100)==5//�Ϻ�
					continue;
				#endif
				}
				if(MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+ADDR_MONTHCONGEALFLAG,1)==0xff)
				{//�����־�ǲ�����,ֻ�е��Ĳ�������Ჹ,�����ǵ��������������ݱ��⽫0xffתΪ0xee
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//���ǲ�������
						continue;
					}
				}
		#if AFN0DF1MaxPn==0
				Class2_Group(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),Fn,Pn);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
		#endif
				x=GetClass2DataLen(Fn,Pn,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+OfficeAddr);//ȡ��2���ݳ���
				if((TXlen+4+14+8+2+(2+x))>LEN_UARTnTx)
				{
					if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
					{
						TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
					goto MulTx;//����-���к���֡
				}
				MWR(DataBuffDate,TXaddr,2);//2�ֽڶ���ʱ��
		#if AFN0DF1MaxPn==0
				switch(Fn)
				{
				//2������.�¶���
					case 17://F17������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 18://F18�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
					case 19://F19������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					case 20://F20�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						MR(TXaddr+2,ADDR_DATABUFF,x);//�ٶ�x�ֽڶ�������
						break;
			#if AFN0DF21MaxPn==0
					case 21://�����������й����������ܡ�����1��M��
						UART_AFN0DF21F22F23F24_Computer(TXaddr+2,DataBuffDate,177,Pn,5);//2���¶������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
						break;
					case 22://�����������޹����������ܡ�����1��M��
						UART_AFN0DF21F22F23F24_Computer(TXaddr+2,DataBuffDate,178,Pn,4);//2���¶������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
						break;
					case 23://�����㷴���й����������ܡ�����1��M��
						UART_AFN0DF21F22F23F24_Computer(TXaddr+2,DataBuffDate,179,Pn,5);//2���¶������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
						break;
					case 24://�����㷴���޹����������ܡ�����1��M��
						UART_AFN0DF21F22F23F24_Computer(TXaddr+2,DataBuffDate,180,Pn,4);//2���¶������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
						break;
			#endif
					default:
						MR(TXaddr+2,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+OfficeAddr,x);//�ٶ�x�ֽڶ�������
						break;
				}
		#else
				MR(TXaddr+2,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+OfficeAddr,x);//�ٶ�x�ֽڶ�������
		#endif
				if(Terminal_Class2DataLib[Fn].PnType==pnType)
				{//�ǲ�������
					Data_0xFFto0xEE(TXaddr+2,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
				}
				//��ǰȡ��2�����ݵķ�������г������������
				i=MRR(ADDR_Terminal_ClassDataLen,3);//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���
				if(i&0x10000)
				{//������
					MWR(i&0xff,TXaddr+2+((i>>8)&0xff),1);
				}
			#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
				if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
				{
					if(CheckUartData_0xEE(TXaddr+2,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
					{
						continue;
					}
				}
			#endif
				TXlen+=2+x;
				TXaddr+=2+x;
				break;
			case 3://3=����(��ʼʱ��ts����ʱ������ 5byte,���ݶ����ܶ�m 1byte,���ݵ���n 1byte)
				//���ݶ����ܶ�m
				p8d=(u8 *)(RXaddr+TIOffice);
			#if MaxACSampleCurveCongeal==0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
				//ACSamplePn=0;//��Ч���ɵ�ѹ���߲�����ż��;����0=��Ч,1=��Ч
				if(p8d[5]==127)
				{//Ĭ�Ͻ��ɵ�ѹ����1��,����15��
					p8d[5]=1;//15 �֡�30 �֡�45 �֡�0 ��
				}
			#else
				ACSamplePn=ACSampleVoltageCurvePn(Fn,Pn);//��Ч���ɵ�ѹ���߲�����ż��;����0=��Ч,1=��Ч
				if(p8d[5]==127)
				{//Ĭ�Ͻ��ɵ�ѹ����1��,����15��
					if(ACSamplePn)//��Ч���ɵ�ѹ���߲������0=��Ч,1=��Ч
					{
						p8d[5]=255;//1 �֡�2 �֡�3 �֡�......0 ��
					}
					else
					{
						p8d[5]=1;//15 �֡�30 �֡�45 �֡�0 ��
					}
				}
			#endif
				switch(p8d[5])
				{
					case 1://15 �֡�30 �֡�45 �֡�0 ��
						m=15;
						i=p8d[0];
						if(i==0x00)
						{
							break;
						}
						if(i<=0x15)
						{
							p8d[0]=0x15;//�ֻ���
							break;
						}
						if(i<=0x30)
						{
							p8d[0]=0x30;//�ֻ���
							break;
						}
						if(i<=0x45)
						{
							p8d[0]=0x45;//�ֻ���
							break;
						}
						p8d[0]=0x0;//�ֻ���
						YMDHM=MRR(RXaddr+TIOffice,5);
						YMDHM=YMDHM_AddM(YMDHM,60);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
						MWR(YMDHM,RXaddr+TIOffice,5);
						break;
					case 2://30 �֡�0 �֣�
						m=30;
						i=p8d[0];
						if(i==0x00)
						{
							break;
						}
						if(i<=0x30)
						{
							p8d[0]=0x30;//�ֻ���
							break;
						}
						p8d[0]=0x0;//�ֻ���
						YMDHM=MRR(RXaddr+TIOffice,5);
						YMDHM=YMDHM_AddM(YMDHM,60);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
						MWR(YMDHM,RXaddr+TIOffice,5);
						break;
					case 3://0 ��
						m=60;
						if(p8d[0]==0x00)
						{
							break;
						}
						p8d[0]=0x0;//�ֻ���
						YMDHM=MRR(RXaddr+TIOffice,5);
						YMDHM=YMDHM_AddM(YMDHM,60);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
						MWR(YMDHM,RXaddr+TIOffice,5);
						break;
			#if (USER/100)==5//�Ϻ�
					case 254://5 �֡�10 �֡�15 ��......��0 ��
						switch(Fn)
						{
							case 235://�ն�AB�ߵ�ѹ���ߣ���չ��
							case 236://�ն�BC�ߵ�ѹ���ߣ���չ��
							case 237://�ն�AC�ߵ�ѹ���ߣ���չ��
								m=5;
								break;
							default:
								goto err;
						}
						break;
			#else
					case 254://5 �֡�10 �֡�15 ��......��0 ��
						m=5;
						i=p8d[0];
						if(i==0x00)
						{
							break;
						}
						if(i<=0x05)
						{
							p8d[0]=0x05;//�ֻ���
							break;
						}
						if(i<=0x10)
						{
							p8d[0]=0x10;//�ֻ���
							break;
						}
						if(i<=0x15)
						{
							p8d[0]=0x15;//�ֻ���
							break;
						}
						if(i<=0x20)
						{
							p8d[0]=0x20;//�ֻ���
							break;
						}
						if(i<=0x25)
						{
							p8d[0]=0x25;//�ֻ���
							break;
						}
						if(i<=0x30)
						{
							p8d[0]=0x30;//�ֻ���
							break;
						}
						if(i<=0x35)
						{
							p8d[0]=0x35;//�ֻ���
							break;
						}
						if(i<=0x40)
						{
							p8d[0]=0x40;//�ֻ���
							break;
						}
						if(i<=0x45)
						{
							p8d[0]=0x45;//�ֻ���
							break;
						}
						if(i<=0x50)
						{
							p8d[0]=0x50;//�ֻ���
							break;
						}
						if(i<=0x55)
						{
							p8d[0]=0x55;//�ֻ���
							break;
						}
						p8d[0]=0x0;//�ֻ���
						YMDHM=MRR(RXaddr+TIOffice,5);
						YMDHM=YMDHM_AddM(YMDHM,60);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
						MWR(YMDHM,RXaddr+TIOffice,5);
						break;
					case 255://1 �֡�2 �֡�3 �֡�......0 ��
						m=1;
						break;
			#endif
					default:
						goto err;
				}
				if(m<CurveCongealTime)
				{//Ҫ���ܶ�<��С���߶�����ʱ��
				#if (USER/100)==5//�Ϻ�
					switch(Fn)
					{
						case 235://�ն�AB�ߵ�ѹ���ߣ���չ��
						case 236://�ն�BC�ߵ�ѹ���ߣ���չ��
						case 237://�ն�AC�ߵ�ѹ���ߣ���չ��
							break;
						default:
							goto err;
					}
				#else
					#if MaxACSampleCurveCongeal!=0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
					if(ACSamplePn==0)//��Ч���ɵ�ѹ���߲������0=��Ч,1=��Ч
					{
						goto err;
					}
					#else
					goto err;
					#endif
				#endif
				}
				//Fn���ݳ���
				FnLen=GetClass2DataLen(Fn,Pn,0);//ȡ��2���ݳ���
				//��ʼʱ�䵽��ǰʱ��������
		#if((USER/100)!=10)//�Ǻӱ��û�
				MC(0,ADDR_DATABUFF,12);
				MR(ADDR_DATABUFF+1,RXaddr+TIOffice,5);
				MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,5);
				i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(i>>31)
				{//��
					i=0;
				}
				else
				{
					i/=60;//�뻯Ϊ��
					i/=m;
					i++;
				}
				if(p8d[6]>i)
				{
					p8d[6]=i;
				}
		#endif
				//���ߵ����ʱ��֡
				PointNum=0;//������������뻺�����ĵ���
				while((TXlen+4+14+8+2+(7+(FnLen*PointNum)))<=LEN_UARTnTx)
				{
					PointNum++;
				}
				if(PointNum!=0)
				{
					PointNum--;
				}
				if(PointNum>p8d[6])
				{
					PointNum=p8d[6];
				}
				if(PointNum==0)
				{
					if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
					{
						TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
					goto MulTx;//����-���к���֡
				}
/*				
				if((TXlen+4+14+8+2+(7+(FnLen*p8d[6])))>LEN_UARTnTx)
				{
					if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
					{
						TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
					}
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
					goto MulTx;//����-���к���֡
				}
*/
				MR(TXaddr,RXaddr+TIOffice,7);//7�ֽڶ���ʱ��
				MWR(PointNum,TXaddr+6,1);//��ǰ֡����
				TXlen+=7;
				TXaddr+=7;
				//��������
				YMDHM=MRR(RXaddr+TIOffice,5);
				//��֡ʱ��͵���
				if(PointNum<p8d[6])
				{//������֡
					NextYMDHM=YMDHM_AddM(YMDHM,m*PointNum);//��֡�Ŀ�ʼʱ��
					MWR(NextYMDHM,RXaddr+TIOffice,5);
					p8d[6]-=PointNum;//��֡�����߻��е���
					MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
				}
				else
				{//û��֡,�ָ�����ʱ��,�ܶ�,����
					if(ALLPn!=0)//ȫ����Ч�����־;0=��,1=��
					{
						MR(RXaddr,(u32)&UARTCtrl->TI,7);//�Ĵ�ȫ��������ʱ���ߵ�ʱ��(������ʱ��),�ܶ�,����
					}
				}
				z=0;//û�ҵ��κ����ݱ�־
				for(y=0;y<PointNum;y++)//�������
				{
				#if (USER/100)==5//�Ϻ�
					z=1;//���ҵ����ݱ�־
					switch(Fn)
					{
						case 235://p0 �ն�AB�ߵ�ѹ���ߣ���չ��
						case 236://p0 �ն�BC�ߵ�ѹ���ߣ���չ��
						case 237://p0 �ն�AC�ߵ�ѹ���ߣ���չ��
							i=GetCurveCongealNo_Ext(YMDHM);//�õ���չ���߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
							if(i==0xffffffff)
							{//û�ҵ���ͬʱ��Ķ����
								MC(0xee,TXaddr,FnLen);
							}
							else
							{
								MR(TXaddr,ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*i)+OfficeAddr,FnLen);//�ٶ�x�ֽڶ�������
								Data_0xFFto0xEE(TXaddr,FnLen);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
							}
							TXlen+=FnLen;
							TXaddr+=FnLen;
							YMDHM=YMDHM_AddM(YMDHM,m);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
							continue;
						default:
							i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
							i=PnCurveCongealNo(i,Pn);//�����㶳�����Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
							break;
					}
				#else//#if (USER/100)==5//�Ϻ�
					#if MaxACSampleCurveCongeal==0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
					i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
					i=PnCurveCongealNo(i,Pn);//�����㶳�����Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
					#else
					if(ACSamplePn==0)//��Ч���ɵ�ѹ���߲������0=��Ч,1=��Ч
					{
						i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
						i=PnCurveCongealNo(i,Pn);//�����㶳�����Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
					}
					else
					{
						i=GetACSampleCurveCongealAddr(YMDHM,Fn);//�õ����ɲ��������߶���洢��ַ,����=0��ʾû�ҵ�
						if(i==0)
						{
							i=~i;
						}
					}
					#endif
				#endif
					if(i==0xffffffff)
					{//û�ҵ���ͬʱ��Ķ����
				NextCurve:
						MC(0xee,TXaddr,FnLen);
						TXlen+=FnLen;
						TXaddr+=FnLen;
						YMDHM=YMDHM_AddM(YMDHM,m);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
						continue;
					}
					
					if(Terminal_Class2DataLib[Fn].PnType!=pnType)
					{//���ǲ�������
						x=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR_CURVECONGEALFLAG,1);
						if(x==0xff)
						{//�����־�ǲ�����,ֻ�е��Ĳ�������Ჹ,�����ǵ���������������
							goto NextCurve;
						}
					}
					else
					{//�ǲ�������
					#if MaxACSampleCurveCongeal==0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
						MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
						p16=(u16 *)(ADDR_PnVSSetList);
						for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
						{
							if(Pn==p16[MapPn])
							{
								break;
							}
						}
						if(MapPn>=MaxKeyUser)
						{//����Ĳ������б���û�ҵ���ͬ�Ĳ������
							goto NextCurve;
						}
						MapPn+=1;
						if(Fn!=219)//��F219 �����������������
						{
							OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
							if(OfficeAddr==0)
							{//û�ҵ�
								goto NextCurve;
							}
						}
					#else
						if(ACSamplePn==0)//��Ч���ɵ�ѹ���߲������0=��Ч,1=��Ч
						{
							MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
							p16=(u16 *)(ADDR_PnVSSetList);
							for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
							{
								if(Pn==p16[MapPn])
								{
									break;
								}
							}
							if(MapPn>=MaxKeyUser)
							{//����Ĳ������б���û�ҵ���ͬ�Ĳ������
								goto NextCurve;
							}
							MapPn+=1;
							if(Fn!=219)//��F219 �����������������
							{
								OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
								if(OfficeAddr==0)
								{//û�ҵ�
									goto NextCurve;
								}
							}
						}
					#endif
					}
					
				#if Uart3761Check0xEE==0//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
					z=1;//���ҵ����ݱ�־
				#endif	

					if(Fn!=219)//��F219 �����������������
					{
					#if MaxACSampleCurveCongeal==0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
						MR(TXaddr,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,FnLen);//�ٶ�x�ֽڶ�������
					#else
						if(ACSamplePn==0)//��Ч���ɵ�ѹ���߲������0=��Ч,1=��Ч
						{
							MR(TXaddr,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,FnLen);//�ٶ�x�ֽڶ�������
						}
						else
						{
							MR(TXaddr,i,FnLen);//�ٶ�x�ֽڶ�������
						}
					#endif
						if(Terminal_Class2DataLib[Fn].PnType==pnType)
						{//�ǲ�������
							Data_0xFFto0xEE(TXaddr,FnLen);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
							x=Check_AFN04F11_Pn(Pn);//����ն��������ò���ָ���Ĳ������;����0��,1��
							if(x==0)
							{//���������,����Ӧ����ʾֵ����õ�
								switch(Fn)
								{
									case 97://�����й��ܵ���������
										UART_AFN0DF97F98F99F100_Computer(TXaddr,YMDHM,m,101,Pn,i);//2���������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵ����Fn
										break;
									case 98://�����޹��ܵ���������
										UART_AFN0DF97F98F99F100_Computer(TXaddr,YMDHM,m,102,Pn,i);//2���������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵ����Fn
										break;
									case 99://�����й��ܵ���������
										UART_AFN0DF97F98F99F100_Computer(TXaddr,YMDHM,m,103,Pn,i);//2���������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵ����Fn
										break;
									case 100://�����޹��ܵ���������
										UART_AFN0DF97F98F99F100_Computer(TXaddr,YMDHM,m,104,Pn,i);//2���������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵ����Fn
										break;
								}
							}
						}
					#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
						if(Terminal_Class2DataLib[Fn].PnType==pnType)//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
						{
							if(CheckUartData_0xEE(TXaddr,FnLen)==0)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
							{
								z=1;//���ҵ����ݱ�־
							}
						}
						else
						{
							z=1;//���ҵ����ݱ�־
						}
					#endif
					}
					else
					{//F219 �����������������
						//�й����ʼ���¼A.9 kW 3
						OfficeAddr=GetClass2DataOfficeAddr(81,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr,3);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
						//�޹����ʼ���¼A.9 kvar 3
						OfficeAddr=GetClass2DataOfficeAddr(85,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+3,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+3,3);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
					#if MaxACSampleCurveCongeal==0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
						//A ���ѹ����¼A.7 V 2
						OfficeAddr=GetClass2DataOfficeAddr(89,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+6,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+6,2);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
						//B ���ѹ����¼A.7 V 2
						OfficeAddr=GetClass2DataOfficeAddr(90,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+8,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+8,2);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
						//C ���ѹ����¼A.7 V 2
						OfficeAddr=GetClass2DataOfficeAddr(91,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+10,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+10,2);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
					#else
						x=ACSampleVoltageCurvePn(89,Pn);//��Ч���ɵ�ѹ���߲�����ż��;����0=��Ч,1=��Ч
						if(x==0)
						{//���ǽ��ɵ�
							//A ���ѹ����¼A.7 V 2
							OfficeAddr=GetClass2DataOfficeAddr(89,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
							MR(TXaddr+6,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//�ٶ�x�ֽڶ�������
							x=Data_0xFFto0xEE(TXaddr+6,2);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
							if(x==0)
							{
								z=1;//���ҵ����ݱ�־
							}
							//B ���ѹ����¼A.7 V 2
							OfficeAddr=GetClass2DataOfficeAddr(90,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
							MR(TXaddr+8,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//�ٶ�x�ֽڶ�������
							x=Data_0xFFto0xEE(TXaddr+8,2);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
							if(x==0)
							{
								z=1;//���ҵ����ݱ�־
							}
							//C ���ѹ����¼A.7 V 2
							OfficeAddr=GetClass2DataOfficeAddr(91,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
							MR(TXaddr+10,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,2);//�ٶ�x�ֽڶ�������
							x=Data_0xFFto0xEE(TXaddr+10,2);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
							if(x==0)
							{
								z=1;//���ҵ����ݱ�־
							}
						}
						else
						{//���ɵ�ѹ���ߵ�
							//A ���ѹ����¼A.7 V 2
							//B ���ѹ����¼A.7 V 2
							//C ���ѹ����¼A.7 V 2
							x=GetACSampleCurveCongealAddr(YMDHM,89);//�õ����ɲ��������߶���洢��ַ,����=0��ʾû�ҵ�
							if(x==0)
							{
								MC(0xee,TXaddr+6,6);
							}
							else
							{
								MR(TXaddr+6,x,6);//�ٶ�x�ֽڶ�������
								z=1;//���ҵ����ݱ�־
							}
						}
					#endif//#if MaxACSampleCurveCongeal==0//��󽻲�1���Ӷ������ߵ���(����Ϊ0��ʾ����)
						//A ���������¼A.25 A 3
						OfficeAddr=GetClass2DataOfficeAddr(92,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+12,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+12,3);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
						//B ���������¼A.25 A 3
						OfficeAddr=GetClass2DataOfficeAddr(93,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+15,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+15,3);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
						//C ���������¼A.25 A 3
						OfficeAddr=GetClass2DataOfficeAddr(94,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+18,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,3);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+18,3);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
						//�����й��ܵ�����ʾֵ����¼A.14 kWh 4
						OfficeAddr=GetClass2DataOfficeAddr(101,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+21,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,4);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+21,4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
						//һ�����޹��ܵ�����ʾֵ����¼A.14 kvar 4
						OfficeAddr=GetClass2DataOfficeAddr(145,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+25,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,4);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+25,4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
						//�������޹��ܵ�����ʾֵ����¼A.14 kvar 4
						OfficeAddr=GetClass2DataOfficeAddr(146,MapPn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
						MR(TXaddr+29,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+OfficeAddr,4);//�ٶ�x�ֽڶ�������
						x=Data_0xFFto0xEE(TXaddr+29,4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
						if(x==0)
						{
							z=1;//���ҵ����ݱ�־
						}
					}
					TXlen+=FnLen;
					TXaddr+=FnLen;
					YMDHM=YMDHM_AddM(YMDHM,m);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
				}
				if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
				{//��֡
					if(z==0x0)
					{//û�ҵ��κ�����
						TXlen-=7+(FnLen*PointNum);
						TXaddr-=7+(FnLen*PointNum);
						continue;
					}
				}
				break;
		}
		FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
		f=1;//������Ч���������־;0=��,1=��
/*
		switch(Fn)
		{//�����ݺ���������ݵ�Fn

		}
*/
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
	if(UARTCtrl->LockFlags&0x30)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B5-B7����
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
#if (USER/100)==7//�㽭
	if(UARTCtrl->LockFlags&0x30)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B5-B7����
	{//������ʱ����
		if(PORTn==GPRSPORT)
		{
			p16=(u16 *)((u32)&Terminal_Ram->UART_S_Timer);
			p16[0]=10;
//			p16[0]=(UARTCtrl->TxFrameCount)*5;
//			p16[0]=20;
		}
	}
#endif
	return;
}

#if (USER/100)==8//�û���ʶ��ͨ�ð�
void Uart_AFN0DF321(u32 PORTn)//����ʱ��������ͳ������
{
	u32 i;
	u32 x;
	u32 TXlen;
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8;
	u32 Pn;
	u32 RXDIaddr;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0d;//AFN
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
	RXDIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	CopyDI(PORTn,RXDIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�;�����ݱ�ʶΪȫ����Ч��������Ŀ��DI��2�ֽ�Pn������Ϊ��Hex��1��ʼ
	i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
	Pn=i&0xffff;
	if(Pn!=0)
	{
		goto err;
	}
	x=MRR(RXDIaddr+4,3);
	x=Check_YMD(x);//�����պϷ����,����0�Ϸ�,1�Ƿ�
	if(x==1)
	{
		goto err;
	}
	x=MRR(RXDIaddr+4,3);
	p8=(u8 *)ADDR_DATABUFF;
	x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
	p8[0]=x;
	if(x!=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[0]-1),3))
	{
		goto err;
	}
	x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[0]-1)+3,4);
	if(x==0xffffffff)
	{
		x=0xeeeeeeee;
	}
	TXlen=12+7;
	//����
	p8txbuff[1]=TXlen;
	p8txbuff[2]=(TXlen>>8);
	p8txbuff[21]=x;
	p8txbuff[22]=(x>>8);
	p8txbuff[23]=(x>>16);
	p8txbuff[24]=(x>>24);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}
void Uart_AFN0DF322(u32 PORTn)//��������ʱ��������ͳ����ϸ	   ����̨�����Ҫ�ٶ�ȫ����Ч����������
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 RXDIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
	u32 LEN_UARTnTx;
	u32 SourDI;//���ηֽ�ǰ��DI
	u32 TIlen;//ʱ�곤��
	u32 TIOffice;//ʱ��ƫ��
	u32 ALLPn;//ȫ����Ч�����־��0=��,1=��
	u32 DataBuffDate;//��ǰUART���ݻ�����붳�����ݵ���������
	u32 TimeCom;
	u8 * p8;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	DataBuffDate=0;//��ǰUART���ݻ�����붳�����ݵ���������
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0d;//AFN(Ϊ��ʱ�ϱ�֡��д��
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
	TIlen=0;//ʱ�곤��
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
			if(ALLPn==0)//ȫ����Ч�����־��0=��,1=��
			{
				//����ʱ�곤��,ÿFn��ÿPnһ��ʱ��
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//����Pn��ϸ���
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
				if(i==0)
				{
					i=1;//p0��
				}
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����Fn��ϸ���
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i*=x;//�ܸ���
			}
			else
			{//�����ݱ�ʶΪȫ����Ч������
				//����ʱ�곤��,ÿFnһ��ʱ��
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����Fn��ϸ���
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
			}
			if(TIlen==0)//ʱ�곤��
			{//ǰ�滹û��һ����ʶ��ȷ��,δ֪����ʶʱ�곤��
				goto err;
			}
			i*=TIlen;//ʱ�곤��
			RXaddr+=i;//���յ�ַ+��ʱ�곤��
			if(DIlen>=(i+4))
			{
				DIlen-=i;
			}
			else
			{
				goto err;
			}
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
		if(ALLPn!=0)//ȫ����Ч�����־;0=��,1=��
		{
			if(Pn>MaxRS485AddCarrierPn)
			{//ȫ��Pn��
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
			{
				continue;
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
			if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
			{
				continue;
			}
		}
		TIlen=3;//�ն���ʱ�곤��
		if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
		{//�����ݱ�ʶΪȫ����Ч������
			if(Pn==0)
			{//p0��Fn���
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
			}
			else
			{//��p0��Fn���
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//����ͬ����Pn��ϱ�Pnǰ��Pn����
				x=(Pn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϸ���
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i*=x;
				y=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				x&=y;
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i+=x;
			}
		}
		else
		{
			i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//������λΪ1�ĸ���(0-32)
		}
		TIOffice=i*TIlen;//ʱ��ƫ��
		if(DIlen<(TIOffice+TIlen+4))
		{
			DIlen=0;
			continue;
		}
		DataBuffDate=MRR(RXaddr+TIOffice,3);
		x=Check_YMD(DataBuffDate);//�����պϷ����,����0�Ϸ�,1�Ƿ�
		if(x==1)
		{
			goto err;
		}
	#if UartDateTI==1//�ն�����ٶ�ʱ��0=���յ�24ʱ����,1=���յ�0ʱ����
		i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
	#endif
		x=3;//ȡ��2���ݳ���
		if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
		{
			if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
			{
				TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
			}
			MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
			goto MulTx;//����-���к���֡
		}
		MWR(DataBuffDate,TXaddr,3);//3�ֽڶ���ʱ��
		p8=(u8 *)ADDR_TYMDHMS;
		TimeCom=DataComp(TXaddr,(u32)ADDR_TYMDHMS+3,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		if((TimeCom==0)||(TimeCom==1))
		{
			goto err;
		}
		p8=(u8 *)TXaddr;//3�ֽڶ���ʱ��
		TimeCom=DataComp(TXaddr,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[0]-1),3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		if(TimeCom==0)
		{
			MR(TXaddr+3,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[0]-1)+3+4+(Pn-1)*3,3);//�ٶ�x�ֽڶ�������
		}
		Data_0xFFto0xEE(TXaddr+3,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
	#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
			if(CheckUartData_0xEE(TXaddr+3,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
			{
				continue;
			}
	#endif
		TXlen+=3+x;
		TXaddr+=3+x;
		FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
		f=1;//������Ч���������־;0=��,1=��
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
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
	return;
MulTx://����-���к���֡
	if(TXlen<=4)
	{
			goto err;
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
#endif

#if (USER/100)==6//�û���ʶ������
void Uart_AFN0DF40x(u32 PORTn)//AFN0DF400��F401��F402
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 RXDIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
	u32 LEN_UARTnTx;
	u32 SourDI;//���ηֽ�ǰ��DI
	u32 TIlen;//ʱ�곤��
	u32 TIOffice;//ʱ��ƫ��
	u32 ALLPn;//ȫ����Ч�����־��0=��,1=��
	u32 DataBuffDate;//��ǰUART���ݻ�����붳�����ݵ���������
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 ADDR_Data;
	u32 YMD;
	
	DataBuffDate=0;//��ǰUART���ݻ�����붳�����ݵ���������
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	p8txbuff[12]=0x0d;//AFN(Ϊ��ʱ�ϱ�֡��д��
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
	TIlen=0;//ʱ�곤��
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
			if(ALLPn==0)//ȫ����Ч�����־��0=��,1=��
			{
				//����ʱ�곤��,ÿFn��ÿPnһ��ʱ��
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//����Pn��ϸ���
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
				if(i==0)
				{
					i=1;//p0��
				}
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����Fn��ϸ���
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i*=x;//�ܸ���
			}
			else
			{//�����ݱ�ʶΪȫ����Ч������
				//����ʱ�곤��,ÿFnһ��ʱ��
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����Fn��ϸ���
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
			}
			if(TIlen==0)//ʱ�곤��
			{//ǰ�滹û��һ����ʶ��ȷ��,δ֪����ʶʱ�곤��
				goto err;
			}
			i*=TIlen;//ʱ�곤��
			RXaddr+=i;//���յ�ַ+��ʱ�곤��
			if(DIlen>=(i+4))
			{
				DIlen-=i;
			}
			else
			{
				goto err;
			}
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
		if(ALLPn!=0)//ȫ����Ч�����־;0=��,1=��
		{
			if(Pn>MaxRS485AddCarrierPn)
			{//ȫ��Pn��
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
			{
				continue;
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
			if(PnSet(Pn)==0)//��Ч������ż��;����0=��Ч,1=��Ч
			{
				continue;
			}
		}
		TIlen=3;//�ն���ʱ�곤��
		if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
		{//�����ݱ�ʶΪȫ����Ч������
			if(Pn==0)
			{//p0��Fn���
				i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
			}
			else
			{//��p0��Fn���
				i=MRR(((u32)(&UARTCtrl->SourDI)),1);//����ͬ����Pn��ϱ�Pnǰ��Pn����
				x=(Pn-1)%8;
				x=0xff<<x;
				x=~x;
				i&=x;
				i=BitNum(i);//������λΪ1�ĸ���(0-32)
				x=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϸ���
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i*=x;
				y=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
				x=(Fn-1)%8;
				x=0xff<<x;
				x=~x;
				x&=y;
				x=BitNum(x);//������λΪ1�ĸ���(0-32)
				i+=x;
			}
		}
		else
		{
			i=MRR(((u32)(&UARTCtrl->SourDI))+2,1);//����ͬ����Fn��ϱ�Fnǰ��Fn����
			x=(Fn-1)%8;
			x=0xff<<x;
			x=~x;
			i&=x;
			i=BitNum(i);//������λΪ1�ĸ���(0-32)
		}
		TIOffice=i*TIlen;//ʱ��ƫ��
		if(DIlen<(TIOffice+TIlen+4))
		{
			DIlen=0;
			continue;
		}
		DataBuffDate=MRR(RXaddr+TIOffice,3);
		x=Check_YMD(DataBuffDate);//�����պϷ����,����0�Ϸ�,1�Ƿ�
		if(x==1)
		{
			goto err;
		}
		switch(Fn)
		{
			case 400://F400 ������̶�ʱ��㵥���ѹ
				x=8;//ȡ��2���ݳ���
				break;
			case 401://F401 ������̶�ʱ��������ѹ������
				x=72;//ȡ��2���ݳ���
				break;
			case 402://F402 ������̶�ʱ���A�ࡢ�������
				x=24;//ȡ��2���ݳ���
				break;
			default:
				return;
		}
		if((TXlen+4+14+8+2+(3+x))>LEN_UARTnTx)
		{
			if(ALLPn==0)//ȫ����Ч�����־;0=��,1=��
			{
				TXlen+=4;//����ȫ��������ʱ4�ֽ���ϱ�ʶ;ȫ��������ʱ��������Զ�����
			}
			MWR(SourDI,(u32)(&UARTCtrl->DestDI),4);//�ָ����ηֽ�ǰ��DI
			goto MulTx;//����-���к���֡
		}
		MWR(DataBuffDate,TXaddr,3);//3�ֽڶ���ʱ��
		i=DataComp(TXaddr,(u32)ADDR_TYMDHMS+3,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		if(i==1)
		{
			goto err;
		}
		YMD=MRR((u32)ADDR_TYMDHMS+3,3);
		if(DataBuffDate==YMD)
		{
			ADDR_Data=ADDR_AFN0DF400_Source;
		}
		else
		{
			ADDR_Data=ADDR_AFN0DF40x_Start;
			for(i=0;i<MaxDate_AFN0DF40x;i++)
			{
				YMD=MRR(ADDR_Data,3);
				if(YMD==DataBuffDate)
				{
					break;
				}
				ADDR_Data+=LEN_AFN0DF40x;
			}
			if(i>=3)
			{
				goto err;
			}
			ADDR_Data+=3;
		}
		switch(Fn)
		{
			case 400://F400 ������̶�ʱ��㵥���ѹ
				MR(TXaddr+3,ADDR_Data+((Pn-1)*x),x);//�ٶ�x�ֽڶ�������
				break;
			case 401://F401 ������̶�ʱ��������ѹ������
				MR(TXaddr+3,ADDR_Data+(8*AFN0DF400MaxPn)+((Pn-1)*x),x);//�ٶ�x�ֽڶ�������
				break;
			case 402://F402 ������̶�ʱ���A�ࡢ�������
				MR(TXaddr+3,ADDR_Data+(80*AFN0DF400MaxPn)+((Pn-1)*x),x);//�ٶ�x�ֽڶ�������
				break;
			default:
				goto err;
		}
		Data_0xFFto0xEE(TXaddr+3,x);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
//	#if Uart3761Check0xEE==1//0xEE���ݼ��:0=�����;1=���,��ȫ0xEE���ط���
//			if(CheckUartData_0xEE(TXaddr+3,x)==1)//�����������UART�����Ƿ�ȫ0xee;���:��Byte>5��Byte��Ϊ5�ֽڵĳ���ʱ�곤��;����0=����ȫ0xEE,1=��ȫ0xEE
//			{
//				continue;
//			}
//	#endif
		TXlen+=3+x;
		TXaddr+=3+x;
		FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
		f=1;//������Ч���������־;0=��,1=��
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
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
	return;
MulTx://����-���к���֡
	if(TXlen<=4)
	{
			goto err;
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
#endif
