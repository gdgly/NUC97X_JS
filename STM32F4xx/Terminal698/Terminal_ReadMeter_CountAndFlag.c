
//�ն˳�������ͳɹ�ʧ�ܱ�־
#include "Project.h"
#include "Terminal_ReadMeter_CountAndFlag.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0E_Event.h"
#include "../Calculate/Calculate.h"




void RM_CountAndFlags(u32 PORTn,u32 MeterNo,u32 f)//���ܱ���ɹ�ʧ�ܼ����ͱ�־,���UART�����ַ,���ܱ����,��־f=0�ɹ�,f=1ʧ��
{
	u32 i;
	u8 * p8;
	u16 * p16;
	
	//ÿ����2λԭ���ܱ����־(0=�ɹ�,1=ʧ��->�ɹ�,2=�ɹ�->ʧ��,3=ʧ��)
	if((MeterNo!=0x0)&&(MeterNo<=MaxRS485AddCarrierPn))
	{
		MeterNo--;
		p8=(u8 *)(ADDR_RM_CountFlags+32+(MeterNo/4));
		MeterNo%=4;
		MeterNo<<=1;
		i=3;
		i<<=MeterNo;
		
		if(ERC_Event_Set(31)==0)//����¼���¼����,����0=����¼,1=��¼
		{
			if(f==0x0)
			{//�ɹ�
					p8[0]&=~i;
					i=1;
					i<<=MeterNo;
					p8[0]|=i;
			}
			else
			{//ʧ��
					p8[0]&=~i;
					i=2;
					i<<=MeterNo;
					p8[0]|=i;
			}
		}
		else
		{
			if(f==0x0)
			{//�ɹ�
				if(((p8[0]>>MeterNo)&3)<=1)
				{//ԭҲ�ɹ�
					p8[0]&=~i;
				}
				else
				{//ԭʧ��,ʧ��->�ɹ�
					p8[0]&=~i;
					i=1;
					i<<=MeterNo;
					p8[0]|=i;
				}
			}
			else
			{//ʧ��
				if(((p8[0]>>MeterNo)&3)>=2)
				{//ԭҲʧ��
					p8[0]|=i;
				}
				else
				{//ԭ�ɹ�,�ɹ�->ʧ��
					p8[0]&=~i;
					i=2;
					i<<=MeterNo;
					p8[0]|=i;
				}
			}
		}	
			
		//�ɹ�ʧ�ܼ���
		p16=(u16 *)(ADDR_RM_CountFlags);
		switch(PORTn)
		{
			case RS485_1PORT:
				i=0;
				break;
			case RS485_2PORT:
				i=2;
				break;
			case RS485_3PORT:
				i=4;
				break;
			case RS485_4PORT:
				i=6;
				break;
			default:
				return;
		}
		if(f==0x0)
		{
			p16[i]++;//�ɹ�����
		}
		else
		{
			p16[i+1]++;//ʧ�ܼ���
		}
	}
}


