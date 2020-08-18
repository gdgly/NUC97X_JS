

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CALC_RESET_FLOW_TIME.h"
#include "../DL698/EVENT_Record.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"



void FLOW_Count(u32 PORTn)//ͨ����������
{
	u32 i;
	u8 *p8;
	u32 Byte;
	UARTCtrl_TypeDef * UARTCtrl;
	
//#define ADDR_2200     ADDR_TerminalRealTimer+2+(TREALMTIMER_NUM*2)//ͨ������
//��ֵ��=structure
//{
//����ͨ������  double-long-unsigned��
//����ͨ������  double-long-unsigned
//}
//��λ��byte�����㣺0
//#define LENmax_2200     12
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Byte=UARTCtrl->RxTxByte;
	p8=(u8*)ADDR_2200;
	if((Byte)||(p8[0]!=DataType_structure))
	{
		UARTCtrl->RxTxByte=0;
		p8[0]=DataType_structure;
		p8[1]=2;
//����ͨ������
		p8[2]=DataType_double_long_unsigned;
		i=(p8[3]<<24)+(p8[4]<<16)+(p8[5]<<8)+p8[6];
		i+=Byte;
		p8[3]=i>>24;
		p8[4]=i>>16;
		p8[5]=i>>8;
		p8[6]=i;
//����ͨ������
		p8[7]=DataType_double_long_unsigned;
		i=(p8[8]<<24)+(p8[9]<<16)+(p8[10]<<8)+p8[11];
		i+=Byte;
		p8[8]=i>>24;
		p8[9]=i>>16;
		p8[10]=i>>8;
		p8[11]=i;
		//�����¼�
		Byte=MRR(ADDR_3110_6+3,4);//ͨ����������  double-long-unsigned����λ��byte��
		if(Byte!=0)
		{
			Byte=R_Inverse(Byte,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���)
			if(i>=Byte)
			{//����
				p8=(u8*)ADDR_2200_EventRecordFags;//ͨ�����������¼��Ѽ�¼��־,�¸���ʱ��0
				if(p8[0]==0)
				{//����û��¼
					p8[0]=1;
					Event_OAD(0x31100200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
		}
	}
}

void POWER_Time(void)//����ʱ��
{
	u32 i;
	u8 *p8;
	
//#define ADDR_2203     ADDR_2200+LENmax_2200//����ʱ��
//��ֵ��=structure
//{
//�չ����ۼ�ʱ��  double-long-unsigned��
//�¹����ۼ�ʱ��  double-long-unsigned
//}
//��λ�����ӣ����㣺0
//1byte �ϴμ��㹩��ʱ���ʵʱ�ӷ�
//#define LENmax_2203     13
	p8=(u8*)ADDR_2203;
	i=Comm_Ram->DL698YMDHMS[5];//��(�ȸ��ֽں���ֽ�),��,��,ʱ,��,��,�ٷ���,��
	if(p8[12]==i)
	{
		return;
	}
	p8[12]=i;
	if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		return;
	}
	p8[0]=DataType_structure;
	p8[1]=2;
//�չ����ۼ�ʱ��  double-long-unsigned��
	p8[2]=DataType_double_long_unsigned;
	i=(p8[3]<<24)+(p8[4]<<16)+(p8[5]<<8)+p8[6];
	i++;
	p8[3]=i>>24;
	p8[4]=i>>16;
	p8[5]=i>>8;
	p8[6]=i;
//�¹����ۼ�ʱ��  double-long-unsigned
	p8[7]=DataType_double_long_unsigned;
	i=(p8[8]<<24)+(p8[9]<<16)+(p8[10]<<8)+p8[11];
	i++;
	p8[8]=i>>24;
	p8[9]=i>>16;
	p8[10]=i>>8;
	p8[11]=i;
}

void RESET_Count(void)//�ն˸�λ����
{
	u32 i;
	u8 *p8;
	
//#define ADDR_2204     ADDR_2203+LENmax_2203//��λ�ۼƴ���
//��ֵ��=structure
//{
//�ո�λ�ۼƴ���  long-unsigned��
//�¸�λ�ۼƴ���  long-unsigned
//}
//#define LENmax_2204     8
	p8=(u8*)ADDR_2204;
	p8[0]=DataType_structure;
	p8[1]=2;
//�ո�λ�ۼƴ���
	p8[2]=DataType_long_unsigned;
	i=(p8[3]<<8)+p8[4];
	i++;
	p8[3]=i>>8;
	p8[4]=i;
//�¸�λ�ۼƴ���
	p8[5]=DataType_long_unsigned;
	i=(p8[6]<<8)+p8[7];
	i++;
	p8[6]=i>>8;
	p8[7]=i;
}

void CLR_RESET_FLOW_TIME(void)//��0�ն˸�λ����_ͨ������_����ʱ��
{
//ͨ����������
	MC(0,ADDR_2200+3,4);
	MC(0,ADDR_2200+8,4);
//ͨ�����������¼��Ѽ�¼��־
	MC(0,ADDR_2200_EventRecordFags,1);//ͨ�����������¼��Ѽ�¼��־,�¸���ʱ��0
//����ʱ��
	MC(0,ADDR_2203+3,4);
	MC(0,ADDR_2203+8,4);
//��λ�ۼƴ���
	MC(0,ADDR_2204+3,2);
	MC(0,ADDR_2204+6,2);
	
}
















