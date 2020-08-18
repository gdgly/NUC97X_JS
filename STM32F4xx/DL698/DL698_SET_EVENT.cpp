

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SET_EVENT.h"
#include "../DL698/DL698_SET.h"
#include "../DL698/DL698_GET.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"






const u8 DL698_Attribute_EventOAD[]=
{
	3,//�����б���
//array
	DataType_array,
	1,
	DataType_OAD,
};


u32 SET_OAD_EVENT(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU)//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 OAD;
	u32 Len;
	DL698_EVENTLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
	p8rx+=4;
	
	Len=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
	Len>>=8;
	pLib=Get_DL698_EVENT_pLib(OAD);//�õ�EVENT_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
		return Len;
	}
	if((pLib->rw&2)==0)//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	{//����д
		if(Comm_Ram->Factory!=0x55)//����״̬
		{
			UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д             ��3��
			return Len;
		}
	}
	if(OAD&0xff00)
	{//��������
		if((OAD&0xff)==0)
		{//ȫ��Ԫ��
			switch(pLib->IC)
			{
				case 7://IC=7 �¼�����ӿ���
					switch((OAD>>8)&0xff)
					{
						case 1://1���߼���
						case 2://2���¼���¼��
						case 4://4����ǰ��¼��
						case 5://5������¼��
						case 7://7����ǰֵ��¼��
							UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
							return Len;
						case 9://9����Ч��ʶ
							if(p8rx[0]!=DataType_bool)
							{
								UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
								return Len;
							}
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD,2);
							UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
							return 2;
						case 8://8���ϱ���ʶ
							if((p8rx[0]!=DataType_enum)&&(p8rx[0]!=DataType_bool))
							{
								UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
								return Len;
							}
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD+2,2);
							UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
							return 2;
						case 3://3�������������Ա�
							Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)DL698_Attribute_EventOAD,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							if((Len&0xff)==0)
							{
								UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
								return 0;
							}
							Len>>=8;
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
								return Len;
							}
							if(Len>LENmax_EventOAD)
							{
								UARTCtrl->DAR=8;//Խ��                 ��8��
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD+4,Len);
							UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
							return Len;
						case 6://6�����ò���
							Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							if(pLib->pDataType)
							{
								if((Len&0xff)==0)
								{
									UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
									return 0;
								}
							}
							Len>>=8;
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
								return 0;
							}
							if(pLib->Addr==0)
							{
								UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
								return Len;
							}
		/*
							if(Comm_Ram->Factory!=0x55)
							{//���ǹ���ģʽ
								if((pLib->rw&0x2)==0)
								{//������д
									UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
									return Len;
								}
							}
		*/
							if(Len>pLib->LENmax)
							{
								UARTCtrl->DAR=8;//Խ��                 ��8��
								return Len;
							}
							MW((u32)p8rx,pLib->Addr,Len);
							UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
							return Len;
						default:
							UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
							return Len;
					}
					//return Len;
				case 24://IC=24 �����¼�����ӿ���
					switch((OAD>>8)&0xff)
					{
						case 1://1���߼���	��static��
						case 3://3����ǰ��¼��	��dyn.��
						case 4://4������¼��	��static��
						case 6://6���¼���¼��1	��dyn.��
						case 7://7���¼���¼��2	��dyn.��
						case 8://8���¼���¼��3	��dyn.��
						case 9://9���¼���¼��4	��dyn.��
						case 10://10����ǰֵ��¼��	��dyn.��
							UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
							return Len;
						case 12://12����Ч��ʶ	��static��
							if(p8rx[0]!=DataType_bool)
							{
								UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
								return Len;
							}
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD,2);
							UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
							return 2;
						case 11://11���ϱ���ʶ	��static��
							if(p8rx[0]!=DataType_enum)
							{
								UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
								return Len;
							}
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD+2,2);
							UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
							return 2;
						case 2://2�������������Ա�	��static��
							Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)DL698_Attribute_EventOAD,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							if((Len&0xff)==0)
							{
								UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
								return 0;
							}
							Len>>=8;
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
								return Len;
							}
							if(pLib->Addr_EventOAD==0)
							{
								UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
								return Len;
							}
							if(Len>LENmax_EventOAD)
							{
								UARTCtrl->DAR=8;//Խ��                 ��8��
								return Len;
							}
							MW((u32)p8rx,pLib->Addr_EventOAD+4,Len);
							UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
							return Len;
						case 5://5�����ò���	��static��
							Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							if(pLib->pDataType)
							{
								if((Len&0xff)==0)
								{
									UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
									return 0;
								}
							}
							Len>>=8;
							if(LEN_RxAPDU<(4+Len))
							{
								UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
								return 0;
							}
							if(pLib->Addr==0)
							{
								UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
								return Len;
							}
							if(Comm_Ram->Factory!=0x55)
							{//���ǹ���ģʽ
								if((pLib->rw&0x2)==0)
								{//������д
									UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
									return Len;
								}
							}
							if(Len>pLib->LENmax)
							{
								UARTCtrl->DAR=8;//Խ��                 ��8��
								return Len;
							}
							MW((u32)p8rx,pLib->Addr,Len);
							UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
							return Len;
						default:
							UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
							return Len;
					}
					//return Len;
				default:
					UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
					return Len;
			}
		}
		else
		{//����Ԫ��
			if(pLib->Addr==0)
			{
				UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
				return Len;
			}
			if(Comm_Ram->Factory!=0x55)
			{//���ǹ���ģʽ
				if((pLib->rw&0x2)==0)
				{//������д
					UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
					return Len;
				}
			}
			Len=SET_OAD_Element(PORTn,p8rx,(u8*)pLib->Addr,OAD&0xff,pLib->LENmax);//����һ������Ԫ��;����:��Ԫ�ص����ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		}
	}
	else
	{//0��ʾ�����������ԣ����������������
		UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
		return Len;
	}
//	return 0;
}






































