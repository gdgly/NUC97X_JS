
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_8000.h"
#include "../DL698/DL698_ACTION_6000.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/Contrl_Manage.h"
#include "../DL698/EVENT_Record.h"




//ң��
u32 ACTION_8000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 f;
	u32 OAD;
	u8* p8;
	u8* p8d;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	
	LenRx=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
	if(p8rx[4+LenRx]==0)
	{//��ʱ���ǩ
		UARTCtrl->DAR=32;//ʱ���ǩ��Ч          (32)
		return (LenRx<<16)+LenTx;
	}
	i=Uart_TimeTag(p8rx+4+LenRx+1);//ʱ���ǩ��Ч���ж�;���:p8rxָ��ʱ���ǩ;����:0=��Ч,1=��Ч
	if(i)
	{
		UARTCtrl->DAR=32;//ʱ���ǩ��Ч          (32)
		return (LenRx<<16)+LenTx;
	}
	
	i=p8rx[2];
	switch(i)
	{
		case 127://����127�������澯��������������=NULL
			MC(0x55,ADDR_8000_127,1);
			return (LenRx<<16)+LenTx;
		case 128://����128�����������������������=NULL
			MC(0,ADDR_8000_127,1);
			return (LenRx<<16)+LenTx;
		case 129://����129����բ��������������=array structure
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(i>LENmax_8000_129)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+0;
			}
			//��ԭ��բ����
			MR(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			p8=p8rx;
			n=p8[1];//��ǰ����������
			if(n>NUMmax_CR)
			{
				n=NUMmax_CR;
			}
			p8+=2;
			while(n--)
			{
				OAD=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//�̵���OAD
				//�޵�ʱ��
				i=OAD&0xff;//�̵�����
				if((i!=0)&&(i<=NUMmax_CR))
				{
					i--;
					x=(p8[10]<<8)|p8[11];//�޵�ʱ��    long-unsigned����λ�����ӣ����㣺0��ֵΪ0��ʾ�����޵磩
					if(x)
					{//���޵�ʱ��
						MWR(x,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+22+(i*2),2);
					}
				}
				p8d=(u8*)ADDR_DATABUFF;
				x=p8d[1];//������
				if(x>NUMmax_CR)
				{
					x=NUMmax_CR;
					p8d[1]=x;//������
				}
				p8d+=2;
				for(i=0;i<x;i++)
				{
					f=(p8d[3]<<24)|(p8d[4]<<16)|(p8d[5]<<8)|p8d[6];//�̵���OAD
					if(f==OAD)
					{//ԭ�Ѿ���
						MW((u32)p8,(u32)p8d,LENper_8000_129);
						break;
					}
					p8d+=LENper_8000_129;
				}
				if(i>=x)
				{//û�ҵ�
					x++;//������+1
					if(x<=NUMmax_CR)
					{
						MW((u32)p8,(u32)p8d,LENper_8000_129);
						p8d=(u8*)ADDR_DATABUFF;
						p8d[0]=DataType_array;
						p8d[1]=x;//������
					}
				}
				i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				p8+=i;
			}
			MW(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);

			//�޸ĺ�բ����
			f=0;//��բ�����仯��־
			MR(ADDR_DATABUFF,ADDR_8000_130,LENmax_8000_130);
			n=p8rx[1];//������
			if(n>NUMmax_CR)
			{
				n=NUMmax_CR;
			}
			p8rx+=2;
			while(n--)
			{
				OAD=(p8rx[3]<<24)|(p8rx[4]<<16)|(p8rx[5]<<8)|p8rx[6];//�̵���OAD
				p8=(u8*)ADDR_DATABUFF;
				x=p8[1];//������
				if(x>NUMmax_CR)
				{
					x=NUMmax_CR;
					p8[1]=x;//������
				}
				p8+=2;
				for(y=0;y<x;y++)
				{
					i=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//�̵���OAD
					if(i==OAD)
					{
						//ɾ����բ����
						i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
						MR((u32)p8,((u32)p8)+i,LENmax_8000_130);
						x--;
						p8=(u8*)ADDR_DATABUFF;
						p8[1]=x;//������
						f=1;//�����仯��־
//						p8[3]=0;
//						p8[4]=0;
//						p8[5]=0;
//						p8[6]=0;
						break;
					}
					i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					p8+=i;
				}
				i=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				p8rx+=i;
			}
			if(f)
			{//�����仯��־
				MW(ADDR_DATABUFF,ADDR_8000_130,LENmax_8000_130);
			}

		#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			Terminal_Remote_Control();//�ն˿���-ң����բ
		#endif
		#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{
				Terminal_Remote_Control();//�ն˿���-ң����բ
			}
		#endif
			return (LenRx<<16)+LenTx;
		case 130://����130����բ��������������=array structure
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(i>LENmax_8000_130)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+LenTx;
			}
			//��ԭ��բ����
			MR(ADDR_DATABUFF,ADDR_8000_130,LENmax_8000_130);
			p8=p8rx;
			n=p8[1];//��ǰ����������
			p8+=2;
			while(n--)
			{
				OAD=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//�̵���OAD
				p8d=(u8*)ADDR_DATABUFF;
				x=p8d[1];//������
				p8d+=2;
				for(i=0;i<x;i++)
				{
					f=(p8d[3]<<24)|(p8d[4]<<16)|(p8d[5]<<8)|p8d[6];//�̵���OAD
					if(f==OAD)
					{
						
						break;
					}
					f=Get_DL698DataLen_S(p8d,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					p8d+=f;
				}
				if(i>=x)
				{//û�ҵ�
					x++;//������+1
					if(x<=NUMmax_CR)
					{
						i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
						MW((u32)p8,(u32)p8d,i);
						p8d=(u8*)ADDR_DATABUFF;
						p8d[0]=DataType_array;
						p8d[1]=x;//������
					}
				}
				i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				p8+=i;
			}
			MW(ADDR_DATABUFF,ADDR_8000_130,LENmax_8000_130);

			//�޸���բ����
			f=0;//��բ�����仯��־
			MR(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			n=p8rx[1];//������
			if(n>NUMmax_CR)
			{
				n=NUMmax_CR;
			}
			p8rx+=2;
			while(n--)
			{
				OAD=(p8rx[3]<<24)|(p8rx[4]<<16)|(p8rx[5]<<8)|p8rx[6];//�̵���OAD
				p8=(u8*)ADDR_DATABUFF;
				x=p8[1];//������
				if(x>NUMmax_CR)
				{
					x=NUMmax_CR;
					p8[1]=x;//������
				}
				p8+=2;
				for(y=0;y<x;y++)
				{
					i=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//�̵���OAD
					if(i==OAD)
					{
						//ɾ����բ����
						f=1;//�����仯��־
						MR((u32)p8,((u32)p8)+LENper_8000_129,LENmax_8000_129);
						x--;
						p8=(u8*)ADDR_DATABUFF;
						p8[1]=x;//������
						break;
					}
					p8+=LENper_8000_129;
				}
				i=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				p8rx+=i;
			}
			if(f)
			{//�����仯��־
				MW(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			}
		#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			Terminal_Remote_Control();//�ն˿���-ң����բ
		#endif
		#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{
				Terminal_Remote_Control();//�ն˿���-ң����բ
			}
		#endif
			return (LenRx<<16)+LenTx;
		case 131://����131��������ĺ�բ�������� ������=array structure
	#if (Project/100)<=2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(i>LENmax_8000_130)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+0;
			}
			MW((u32)p8rx,ADDR_8000_130,i);
			//�޸���բ����
			f=0;//��բ�����仯��־
			MR(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			n=p8rx[1];//������
			p8rx+=2;
			while(n--)
			{
				OAD=(p8rx[3]<<24)|(p8rx[4]<<16)|(p8rx[5]<<8)|p8rx[6];//�̵���OAD
				p8=(u8*)ADDR_DATABUFF;
				x=p8[1];//������
				p8+=2;
				while(x--)
				{
					i=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//�̵���OAD
					if(i==OAD)
					{
						f=1;//�����仯��־
						p8[3]=0;
						p8[4]=0;
						p8[5]=0;
						p8[6]=0;
						break;
					}
					p8+=LENper_8000_129;
				}
				i=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				p8rx+=i;
			}
			if(f)
			{//�����仯��־
				MW(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			}
	#else
			UARTCtrl->DAR=255;
	#endif
			return (LenRx<<16)+LenTx;
			
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//����
u32 ACTION_8001(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	
	LenRx=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
	if(p8rx[4+LenRx]==0)
	{//��ʱ���ǩ
		UARTCtrl->DAR=32;//ʱ���ǩ��Ч          (32)
		return (LenRx<<16)+LenTx;
	}
	i=Uart_TimeTag(p8rx+4+LenRx+1);//ʱ���ǩ��Ч���ж�;���:p8rxָ��ʱ���ǩ;����:0=��Ч,1=��Ч
	if(i)
	{
		UARTCtrl->DAR=32;//ʱ���ǩ��Ч          (32)
		return (LenRx<<16)+LenTx;
	}
	
	i=p8rx[2];
	switch(i)
	{
		case 127://����127��Ͷ�뱣�磨������������=NULL
			MC(0x55,ADDR_8001_127,1);
		#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			Terminal_Contrl_Protect();//�ն˿���-����
		#endif
		#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{
				Terminal_Contrl_Protect();//�ն˿���-����
			}
		#endif
			return (LenRx<<16)+LenTx;
		case 128://����128��������磨������������=NULL
			MC(0,ADDR_8001_127,1);
		#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			Terminal_Contrl_Protect();//�ն˿���-����
		#endif
		#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{
				Terminal_Contrl_Protect();//�ն˿���-����
			}
		#endif
			return (LenRx<<16)+LenTx;
		case 129://����129������Զ����磨������������=NULL
			MWR(DataType_long_unsigned,ADDR_8001_3,3);//����-��������վ�����ͨ��ʱ�������ӣ�
			MWR(DataType_long_unsigned,ADDR_8001_4,3);//����-�ϵ��Զ�����ʱ�������ӣ�
			MWR(DataType_array,ADDR_8001_5,2);//����-�Զ�����ʱ��
		#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			Terminal_Contrl_Protect();//�ն˿���-����
		#endif
		#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{
				Terminal_Contrl_Protect();//�ն˿���-����
			}
		#endif
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//�߷Ѹ澯
u32 ACTION_8002(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	i=p8rx[2];
	switch(i)
	{
		case 127://����127���߷Ѹ澯Ͷ�루������
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			if(i>LENmax_8002_127)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+0;
			}
			MW((u32)p8rx,ADDR_8002_127,i);
			Terminal_Contrl_Out();//�ն˿���-���.�̵�����LED
			return (LenRx<<16)+LenTx;
		case 128://����128��ȡ���߷Ѹ澯��������������=NULL
			MC(0,ADDR_8002_127,1);
			LenRx=1;
			Terminal_Contrl_Out();//�ն˿���-���.�̵�����LED
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//ʱ�ι���
u32 ACTION_8103(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://��ӿ��Ƶ�Ԫ
		case 5://���¿��Ƶ�Ԫ
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx!=LENper_8103_2)
			{
				UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
				return (LenRx<<16)+LenTx;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_2;
			}
			if(i<Array)
			{//�ҵ�
				MW((u32)p8rx,(u32)p8,LENper_8103_2);
				MW(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
			}
			else
			{//û�ҵ�
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8103_2*Array),LENper_8103_2);
					MW(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
				}
				else
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
				}
			}
			return (LenRx<<16)+LenTx;
		case 4://ɾ���ص�Ԫ
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_2;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8103_2,LENmax_8103_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
			}
			else
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
			}
	//����Ͷ��״̬
			MR(ADDR_128KDATABUFF,ADDR_8103_3,LENmax_8103_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_3;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8103_3,LENmax_8103_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8103_3,LENmax_8103_3);
			}
	//ʱ�ι��ط����л�
			MR(ADDR_128KDATABUFF,ADDR_8103_127,LENmax_8103_127);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_127,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_127;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8103_127,LENmax_8103_127);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8103_127,LENmax_8103_127);
			}
			return (LenRx<<16)+LenTx;
		case 6://����Ͷ��
		case 7://���ƽ��
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8103_3,LENmax_8103_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_3;
			}
			if(i>Array)
			{//û�ҵ�
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8103_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{δͶ�루0����Ͷ�루1��}
			}
			else
			{
				p8[6]=0;//{δͶ�루0����Ͷ�루1��}
			}
			MW(ADDR_128KDATABUFF,ADDR_8103_3,LENmax_8103_3);
			return (LenRx<<16)+LenTx;
		case 127://����127��ʱ�ι��ط����л����ܼ�����󣬿��Ʒ�����
//ʱ�ι��ط����л���=structure
//�ܼ�������=OI
//���Ʒ�����=structure
//{
//ʱ�ι���Ͷ���ʶ    bit-string(SIZE(8))��
//ʱ�ι��ض�ֵ������  unsigned
//}
//ʱ�ι���Ͷ���ʶ��D0��D7��˳���λ��ʾ��1����8ʱ�Σ��á�1������Ч���á�0������Ч��
//ʱ�ι��ض�ֵ�����ţ���ֵ��Χ��0��2���α�ʾ��1����3�׷���������ֵ��Ч��
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			if(i>LENper_8103_127)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
			MR(ADDR_128KDATABUFF,ADDR_8103_127,LENmax_8103_127);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_127,2);
			}
			p8+=2;
			for(Method=1;Method<=Array;Method++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_127;
			}
			if(Method>Array)
			{//û�ҵ�
				Array=Method;
				if(Method>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			Array--;
			MW((u32)p8rx,ADDR_128KDATABUFF+2+(Array*LENper_8103_127),LENper_8103_127);
			MW(ADDR_128KDATABUFF,ADDR_8103_127,LENmax_8103_127);
			return (LenRx<<16)+LenTx;

		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//���ݿ�
u32 ACTION_8104(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://��ӿ��Ƶ�Ԫ
		case 5://���¿��Ƶ�Ԫ
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8104_2;
			}
			if(i<Array)
			{//�ҵ�
				MW((u32)p8rx,(u32)p8,LENper_8104_2);
				MW(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
			}
			else
			{//û�ҵ�
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8104_2*Array),LENper_8104_2);
					MW(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
				}
				else
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
				}
			}
			return (LenRx<<16)+LenTx;
		case 4://ɾ���ص�Ԫ
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8104_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8104_2;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8104_2,LENmax_8104_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
			}
			else
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
			}
	//����Ͷ��״̬
			MR(ADDR_128KDATABUFF,ADDR_8104_3,LENmax_8104_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8104_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8104_3;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8104_3,LENmax_8104_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8104_3,LENmax_8104_3);
			}
			return (LenRx<<16)+LenTx;
		case 6://����Ͷ��
		case 7://���ƽ��
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8104_3,LENmax_8104_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8104_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8104_3;
			}
			if(i>Array)
			{//û�ҵ�
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8104_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{δͶ�루0����Ͷ�루1��}
			}
			else
			{
				p8[6]=0;//{δͶ�루0����Ͷ�루1��}
			}
			MW(ADDR_128KDATABUFF,ADDR_8104_3,LENmax_8104_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//Ӫҵ��ͣ��
u32 ACTION_8105(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://��ӿ��Ƶ�Ԫ
		case 5://���¿��Ƶ�Ԫ
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8105_2;
			}
			if(i<Array)
			{//�ҵ�
				MW((u32)p8rx,(u32)p8,LENper_8105_2);
				MW(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
			}
			else
			{//û�ҵ�
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8105_2*Array),LENper_8105_2);
					MW(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
				}
				else
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
				}
			}
			return (LenRx<<16)+LenTx;
		case 4://ɾ���ص�Ԫ
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			OI=(p8rx[1]<<8)+p8rx[2];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8105_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8105_2;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8105_2,LENmax_8105_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
			}
			else
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
			}
	//����Ͷ��״̬
			MR(ADDR_128KDATABUFF,ADDR_8105_3,LENmax_8105_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8105_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8105_3;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8105_3,LENmax_8105_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8105_3,LENmax_8105_3);
			}
			return (LenRx<<16)+LenTx;
		case 6://����Ͷ��
		case 7://���ƽ��
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8105_3,LENmax_8105_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8105_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8105_3;
			}
			if(i>Array)
			{//û�ҵ�
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8105_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{δͶ�루0����Ͷ�루1��}
			}
			else
			{
				p8[6]=0;//{δͶ�루0����Ͷ�루1��}
			}
			MW(ADDR_128KDATABUFF,ADDR_8105_3,LENmax_8105_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//��ǰ�����¸���
u32 ACTION_8106(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Pn;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	i=p8rx[2];
	switch(i)
	{
		case 7://7	���ƽ�����ܼ�������ܼ�������=OI
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			OI=(p8rx[1]<<8)+p8rx[2];//�ܼ������
			if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+0;
			}
	//����Ͷ��״̬
			MR(ADDR_128KDATABUFF,ADDR_8106_3,LENmax_8106_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8106_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8106_3;
			}
			if(i<Array)
			{//�ҵ�
				p8[6]=0;//δͶ�루0����Ͷ�루1��
				MW(ADDR_128KDATABUFF,ADDR_8106_3,LENmax_8106_3);
			}
			return (LenRx<<16)+LenTx;
			
		case 127://����127 Ͷ�루�ܼ�����󣬿��Ʒ�����
//structure
//�ܼ�������=OI
//���Ʒ�����=structure
//{
//��ǰ�����¸��ض�ֵ����ʱ��    unsigned����λ�����ӣ���
//��ǰ�����¸��ض�ֵ����ϵ��    integer����λ��%����
//�غ��ܼ��й����ʶ�����ʱʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��صĿ���ʱ��      unsigned����λ��0.5Сʱ����
//��ǰ�����¸��ص�1�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�2�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�3�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�4�ָ澯ʱ��  unsigned����λ�����ӣ�
//}
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx>LENper_8106_127)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];//�ܼ������
			if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+0;
			}
			Pn=OI&0xff;
			Pn--;
			i=p8rx[14];//��ǰ�����¸��صĿ���ʱ��0.5h
			i*=30;//��ǰ�����¸��صĿ���ʱ��0.5h
			MWR(i,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+6+(Pn*2),2);
			
			MR(ADDR_128KDATABUFF,ADDR_8106_127,LENmax_8106_127);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8106_127,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8106_127;
			}
			if(i>=Array)
			{//û�ҵ�
				if(i<NUMmax_TAG)
				{
					i++;
				}
				else
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
			}
			Array=i;
			if(Array==0)
			{
				Array=1;
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			Array--;
			MW((u32)p8rx,ADDR_128KDATABUFF+2+(Array*LENper_8106_127),LENper_8106_127);
			MW(ADDR_128KDATABUFF,ADDR_8106_127,LENmax_8106_127);
	//����Ͷ��״̬
			MR(ADDR_128KDATABUFF,ADDR_8106_3,LENmax_8106_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8106_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8106_3;
			}
			if(i<Array)
			{//�ҵ�
				p8[6]=1;//δͶ�루0����Ͷ�루1��
			}
			else
			{//û�ҵ�
				if(i<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=i+1;
					p8+=2;
					p8+=LENper_8106_3*i;
					p8[0]=DataType_structure;
					p8[1]=2;
					p8[2]=DataType_OI;
					p8[3]=OI>>8;
					p8[4]=OI;
					p8[5]=DataType_enum;
					p8[6]=1;
				}
				else
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
			}
			MW(ADDR_128KDATABUFF,ADDR_8106_3,LENmax_8106_3);
			return (LenRx<<16)+LenTx;

		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//�����
u32 ACTION_8107(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Pn;
	u32 Array;
	u8 *p8;
	u64 d64a;
	u64 d64b;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://��ӿ��Ƶ�Ԫ
		case 5://���¿��Ƶ�Ԫ
//��������õ�Ԫ
//����2��=structure
//{
//	�ܼ������      OI��
//���絥��        double-long-unsigned��
//׷��/ˢ�±�ʶ   enum{׷�ӣ�0����ˢ�£�1��}��
//��������        enum{������0������ѣ�1��}��
//���������ѣ�ֵ  long64����λ��kWh/Ԫ�� ���㣺-4����
//	��������ֵ      long64����λ��kWh/Ԫ�����㣺-4����
//	��բ����ֵ      long64����λ��kWh/Ԫ�����㣺-4��
//	�����ģʽ      enum{����ģʽ��0����Զ��ģʽ��1��}
//}
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻���ã�9��
				return 0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
			if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			Pn=OI&0xff;
			Pn--;
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8107_2;
			}
			if(i<Array)
			{//�ҵ�
			#if (USER/100)!=0//���ǵ��Ժ����
				//�ȽϹ��絥��
				i=MRR((u32)p8rx+6,4);
				Method=MRR((u32)p8+6,4);
				if(i==Method)
				{//���絥����ͬ
					UARTCtrl->DAR=255;
					return (LenRx<<16)+LenTx;
				}
			#endif
				MW((u32)p8rx,(u32)p8,LENper_8107_2);
				MW(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
			}
			else
			{//û�ҵ�
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8107_2*Array),LENper_8107_2);
					MW(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
				}
				else
				{
					UARTCtrl->DAR=8;//Խ�磨8��
					return (LenRx<<16)+LenTx;
				}
			}
			if(p8rx[11])
			{//ˢ�±�ʶenum{׷�ӣ�0����ˢ�£�1��}
				MW((u32)p8rx+15,ADDR_230x_11+(Pn*LENper_230x_11)+1,8);
			}
			else
			{//׷��
				d64a=MRR((u32)p8rx+15,8);
				d64b=MRR(ADDR_230x_11+(Pn*LENper_230x_11)+1,8);
				d64a=R_Inverse(d64a,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
				d64b=R_Inverse(d64b,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
				d64a+=d64b;
				d64a=R_Inverse(d64a,8);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
				MWR(d64a,ADDR_230x_11+(Pn*LENper_230x_11)+1,8);
			}
			//�����¼�
			p8=(u8*)ADDR_128KDATABUFF+(64*1024);//�����¼�����ʱ����
			p8[0]=DataType_OI;//����Դ
			p8[1]=OI>>8;
			p8[2]=OI;
			Event_OAD(0x32020200,p8,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			return (LenRx<<16)+LenTx;
		case 4://ɾ���ص�Ԫ
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8107_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8107_2;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8107_2,LENmax_8107_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
			}
			else
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
			}
	//����Ͷ��״̬
			MR(ADDR_128KDATABUFF,ADDR_8107_3,LENmax_8107_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8107_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8107_3;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8107_3,LENmax_8107_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8107_3,LENmax_8107_3);
			}
			return (LenRx<<16)+LenTx;
		case 6://����Ͷ��
		case 7://���ƽ��
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8107_3,LENmax_8107_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8107_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8107_3;
			}
			if(i>Array)
			{//û�ҵ�
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8107_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{δͶ�루0����Ͷ�루1��}
			}
			else
			{
				p8[6]=0;//{δͶ�루0����Ͷ�루1��}
			}
			MW(ADDR_128KDATABUFF,ADDR_8107_3,LENmax_8107_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//�µ��
u32 ACTION_8108(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://��ӿ��Ƶ�Ԫ
		case 5://���¿��Ƶ�Ԫ
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻���ã�9��
				return 0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8108_2;
			}
			if(i<Array)
			{//�ҵ�
				MW((u32)p8rx,(u32)p8,LENper_8108_2);
				MW(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
			}
			else
			{//û�ҵ�
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8108_2*Array),LENper_8108_2);
					MW(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
				}
				else
				{
					UARTCtrl->DAR=8;//Խ�磨8��
				}
			}
			return (LenRx<<16)+LenTx;
		case 4://ɾ���ص�Ԫ
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻���ã�9��
				return 0;
			}
			OI=(p8rx[1]<<8)+p8rx[2];
	//���Ʒ�����
			MR(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8108_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8108_2;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8108_2,LENmax_8108_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
			}
			else
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
			}
	//����Ͷ��״̬
			MR(ADDR_128KDATABUFF,ADDR_8108_3,LENmax_8108_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8108_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8108_3;
			}
			if(i<Array)
			{//�ҵ�
				MR((u32)p8,(u32)p8+LENper_8108_3,LENmax_8108_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8108_3,LENmax_8108_3);
			}
			return (LenRx<<16)+LenTx;
		case 6://����Ͷ��
		case 7://���ƽ��
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8108_3,LENmax_8108_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8108_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8108_3;
			}
			if(i>Array)
			{//û�ҵ�
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8108_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{δͶ�루0����Ͷ�루1��}
			}
			else
			{
				p8[6]=0;//{δͶ�루0����Ͷ�루1��}
			}
			MW(ADDR_128KDATABUFF,ADDR_8108_3,LENmax_8108_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}





