

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_EVENT.h"
#include "../DL698/EVENT_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB_EVENT.h"





u32 ACTION_EVENT(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 x;
	u32 OAD;
	u32 LenRx;
	u32 Method;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_EVENTLIB_TypeDef *pLib;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OAD=(p8rx[0]<<8)+p8rx[1];
	Method=p8rx[2];
	p8rx+=4;
	
	LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
	LenRx>>=8;
	pLib=Get_DL698_EVENT_pLib(OAD<<16);//�õ�EVENT_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
	switch(Method)
	{
		case 1://����1����λ
			CLR_EventRecord(OAD);//����ӦOI���¼���¼
			return (LenRx<<16);
		case 3://3������һ�μ�¼
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return (LenRx<<16);
		case 4://4�����һ���¼�������������
			if(pLib==0)
			{
				UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
				return (LenRx<<16);
			}
			if(pLib->Addr_EventOAD)
			{
				OAD=(p8rx[1]<<24)+(p8rx[2]<<16)+(p8rx[3]<<8)+p8rx[4];
				MR(ADDR_128KDATABUFF,pLib->Addr_EventOAD+4,LENmax_EventOAD);
				p8=(u8*)ADDR_128KDATABUFF;
				n=p8[1];//������
				if(n>NUMmax_EventOAD)
				{
					n=0;
				}
				//��ԭ��ͬOAD
				p8+=3;
				for(i=0;i<n;i++)
				{
					x=(p8[0]<<24)+(p8[1]<<16)+(p8[2]<<8)+p8[3];
					if(x==OAD)
					{
						break;
					}
					p8+=5;
				}
				if(i>=n)
				{//û�ҵ�
					if(n>=NUMmax_EventOAD)
					{
						UARTCtrl->DAR=8;//Խ��                 ��8��
						return (LenRx<<16);
					}
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=n+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(5*n),5);
					n++;
					MW(ADDR_128KDATABUFF,pLib->Addr_EventOAD+4,2+(5*n));
				}
			}
			return (LenRx<<16);
		case 5://5��ɾ��һ���¼�������������
			if(pLib==0)
			{
				UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
				return (LenRx<<16);
			}
			if(pLib->Addr_EventOAD)
			{
				OAD=(p8rx[1]<<24)+(p8rx[2]<<16)+(p8rx[3]<<8)+p8rx[4];
				MR(ADDR_128KDATABUFF,pLib->Addr_EventOAD+4,LENmax_EventOAD);
				p8=(u8*)ADDR_128KDATABUFF;
				n=p8[1];//������
				if(n>NUMmax_EventOAD)
				{
					n=0;
				}
				//��ԭ��ͬOAD
				p8+=3;
				for(i=0;i<n;i++)
				{
					x=(p8[0]<<24)+(p8[1]<<16)+(p8[2]<<8)+p8[3];
					if(x==OAD)
					{
						break;
					}
					p8+=5;
				}
				if(i>=n)
				{//û�ҵ�
					return (LenRx<<16);
				}
				//�ҵ�
				MR(ADDR_128KDATABUFF+2+(5*i),ADDR_128KDATABUFF+2+(5*i)+5,LENmax_EventOAD);
				p8=(u8*)ADDR_128KDATABUFF;
				n--;
				p8[0]=DataType_array;
				p8[1]=n;
				MW(ADDR_128KDATABUFF,pLib->Addr_EventOAD+4,2+(5*n));
			}
			return (LenRx<<16);
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return (LenRx<<16);
	}
}






























