
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SET_6000.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_SETLIB.h"



u32 SET_OAD_6000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU)//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 OI;
	u32 Attribute;
	u32 Element;
	u32 Array;
	u32 Len;
	DL698_SETLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OI=(p8rx[0]<<8)+p8rx[1];
	Attribute=p8rx[2]&0x1f;
	Element=p8rx[3];
	p8rx+=4;
	
	Len=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
	Len>>=8;
	pLib=Get_DL698_SET_pLib((OI<<16)+(Attribute<<8));//�õ�EVENT_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
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
	if(Attribute)
	{//��������
		if(Element==0)
		{//ȫ��Ԫ��
			MC(0,pLib->Addr,pLib->LENmax);
			Array=p8rx[1];//������
			p8rx+=2;
			if(LEN_RxAPDU<(4+2))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LEN_RxAPDU-=(4+2);
			while(Array--)
			{
				Attribute=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				if(pLib->pDataType)
				{
					if((Attribute&0xff)==0)
					{
						UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
						return Len;
					}
				}
				Attribute>>=8;
				if(LEN_RxAPDU<Attribute)
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return Len;
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
				if(Attribute>pLib->LENmax)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return Len;
				}
				if(p8rx[2]==DataType_unsigned)
				{
					Element=p8rx[3];//ȡ�������
				}
				else
				{
					Element=(p8rx[3]<<8)+p8rx[4];//ȡ�������
				}
				if(Element>=(pLib->LENmax/pLib->LENper))
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return Len;
				}
				MW((u32)p8rx,pLib->Addr+(Element*pLib->LENper),Attribute);
				p8rx+=Attribute;
			}
			UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
			return Len;
		}
		else
		{//����Ԫ��
			UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
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




