
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_USER14xx.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"




#if (USER/100)==14

u32 ACTION_Add_USER14xx(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 LenRx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(p8rx[4+0]==DataType_array)
	{//���һ��
		Array=p8rx[4+1];
		if(Array&0x80)
		{
			Array&=0x7f;
			if(LEN_RxAPDU<=(4+2+Array))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2+Array;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2+Array;
			i=Array;
			Array=0;
			while(i--)
			{
				Array<<=8;
				Array|=p8rx[0];
				p8rx+=1;
			}
		}
		else
		{
			if(LEN_RxAPDU<=(4+2))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2;
		}
	}
	else
	{//��Ӹ���һ��
		if(LEN_RxAPDU<=(4+2))
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		Array=1;
		LenRx=0;
		p8rx+=4;
		LEN_RxAPDU-=4;
	}
	while(Array--)
	{
		i=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		if(LEN_RxAPDU<i)
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		LenRx+=i;
		if(i>pLib->LENper)
		{
			UARTCtrl->DAR=8;//Խ��                 ��8��
			return (LenRx<<16)+0;
		}
		
		if(p8rx[2]==DataType_unsigned)
		{
			n=p8rx[3];//ȡ�������
		}
		else
		{
			n=(p8rx[3]<<8)+p8rx[4];//ȡ�������
		}
		if(n>=(pLib->LENmax/pLib->LENper))
		{
			UARTCtrl->DAR=8;//Խ�磨8��
			return (LenRx<<16)+0;
		}
		MW((u32)p8rx,pLib->Addr+(n*pLib->LENper),i);
		p8rx+=i;
	}
	return (LenRx<<16)+0;
}

u32 ACTION_Del_USER14xx(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 LenRx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(p8rx[4+0]==DataType_array)
	{//ɾ��һ��
		Array=p8rx[4+1];
		if(Array&0x80)
		{
			Array&=0x7f;
			if(LEN_RxAPDU<=(4+2+Array))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2+Array;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2+Array;
			i=Array;
			Array=0;
			while(i--)
			{
				Array<<=8;
				Array|=p8rx[0];
				p8rx+=1;
			}
		}
		else
		{
			if(LEN_RxAPDU<=(4+2))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2;
		}
	}
	else
	{//ɾ��һ��
		if(LEN_RxAPDU<=(4+2))
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		Array=1;
		LenRx=0;
		p8rx+=4;
		LEN_RxAPDU-=4;
	}
	while(Array--)
	{
		if(LEN_RxAPDU<2)
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		if(p8rx[0]==DataType_unsigned)
		{
			n=p8rx[1];//ȡ�������
			p8rx+=2;
			LenRx+=2;
		}
		else
		{
			n=(p8rx[1]<<8)+p8rx[2];//ȡ�������
			p8rx+=3;
			LenRx+=3;
		}
		if(n<(pLib->LENmax/pLib->LENper))
		{
			MC(0,pLib->Addr+(n*pLib->LENper),1);
		}
	}
	return (LenRx<<16)+0;
}





extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6E11[];
//ȫ��͸�����񷽰���
u32 ACTION_6E110200(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{

	u32 i;
	u32 x;
	u32 Method;
	u32 Array;
	u32 LenRx;
	u32 LenTx;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
//����2�����ñ���=arrayȫ��͸�����񷽰���Ԫ(��Ӧ���񷽰������б����Ӧ�������²�����ʼ������ȣ�û�б���Ĳ�Ӧ����ִ������)
	pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_6E11;
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://Add��array ͸�����񷽰���Ԫ��;��ӻ����һ��͸�����񷽰���Ԫ�����������б仯��Ӧ��������ݼ�¼�����¿�ʼ�������
			i=ACTION_Add_USER14xx(pLib,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 128://Delete��array ͸�����񷽰��ţ� �����񷽰���ɾ��һ��͸�����񷽰���Ԫ�������������ü������ݼ�¼
			i=ACTION_Del_USER14xx(pLib,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 129://Clear (����) ������=NULL
			//���͸�����񷽰����ñ�
			MC(0,ADDR_6E11,LENmax_6E11);
			//��վ�����������
			MC(0,ADDR_6E13,LENmax_6E13);
			//��ռ������ݼ�¼
			
			//
			LenRx=1;
			p8rx+=4+1;
			LEN_RxAPDU-=4+1;
			return (LenRx<<16)+LenTx;
		case 130://����130��Update��array ������
//������=structure 
//{
//͸�����񷽰���  long-unsigned��
//ִ�����ȼ�      unsigned
//}
//���µĲ��Ը�������״̬��0ɾ���÷������������������ݣ�254Ϊ��ͣ����255Ϊ�����������񷽰����ȼ����䣬ִ�������ǰ���������¼�����¿�ʼִ�У��򴥷�һ�ζ���
			
			
			return i;
		default:
			UARTCtrl->DAR=6;//���󲻴��� 6
			return 0;
	}
}

extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6E13[];
//ȫ��͸����������
u32 ACTION_6E13(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{

	u32 i;
	u32 x;
	u32 Method;
	u32 Array;
	u32 LenRx;
	u32 LenTx;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
//����2�����ñ���=arrayȫ��͸�����񷽰���Ԫ(��Ӧ���񷽰������б����Ӧ�������²�����ʼ������ȣ�û�б���Ĳ�Ӧ����ִ������)
	pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_6E13;
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://Add��array ȫ��͸����������Ԫ��
			i=ACTION_Add_USER14xx(pLib,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 128://Delete��array ������
//������=structure
//{
//  ͸�����񷽰���long-unsigned��
//  ͨ�ŵ�ַ����   MS
//}
//��͸�����񷽰��ź͵�ַɾ��һ��͸����������
			
			return i;
		case 129://Delete��array͸�����񷽰��ţ�
//��͸�����񷽰���ɾ��һ��͸����������
			return (LenRx<<16)+LenTx;
		case 130://Clear(����) ������=NULL ���͸����������
			
			
			return i;
		default:
			UARTCtrl->DAR=6;//���󲻴��� 6
			return 0;
	}
}







#endif//#if (USER/100)==14






















