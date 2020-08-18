

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_SET_ACTION_2000.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/Contrl_Manage.h"
#include "../DL698/CALC_TAG.h"



const s8 ADDR_230x_18[]=//�ܼ��黻�㼰��λ
{
//���㼰��λ��=structure
 DataType_structure,10,
//{
// ����3��λ�����=��λ��W�����㣺-1
 DataType_Scaler_Unit,-1,Unit_W,
// ����4��λ�����=��λ��var�����㣺-1
 DataType_Scaler_Unit,-1,31, 
// ����5��λ�����=��λ��W�����㣺-1
 DataType_Scaler_Unit,-1,27,
// ����6��λ�����=��λ��var�����㣺-1
 DataType_Scaler_Unit,-1,31, 
// ����7��λ�����=��λ��kWh�����㣺-4
 DataType_Scaler_Unit,-4,33, 
// ����8��λ�����=��λ��kvarh�����㣺-4
 DataType_Scaler_Unit,-4,35, 
// ����9��λ�����=��λ��kWh�����㣺-4
 DataType_Scaler_Unit,-4,33, 
// ����10��λ�����=��λ��kvarh�����㣺-4
 DataType_Scaler_Unit,-4,35, 
// ����11��λ�����=��λ��kWh/Ԫ�����㣺-4
 DataType_Scaler_Unit,-4,54, 
// ����12��λ�����=��λ��W�����㣺-1 
 DataType_Scaler_Unit,-1,27,
//}
};
const s8 ADDR_240x_19[]=//����������㼰��λ
{
//���㼰��λ��=structure
 DataType_structure,14,
//{
//����5��λ�����=��λ��W�����㣺-1
 DataType_Scaler_Unit,-1,Unit_W,
//����6��λ�����=��λ��var�����㣺-1
 DataType_Scaler_Unit,-1,Unit_var,
//����7��λ�����=��λ��kWh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//����8��λ�����=��λ��kWh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//����9��λ�����=��λ��kWh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//����10��λ�����=��λ��kWh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//����11��λ�����=��λ��kvarh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//����12��λ�����=��λ��kvarh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//����13��λ�����=��λ��kvarh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//����14��λ�����=��λ��kvarh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//����15��λ�����=��λ��kWh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//����16��λ�����=��λ��kWh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//����17��λ�����=��λ��kvarh�����㣺-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//����18��λ�����=��λ��kvarh�����㣺-4 
 DataType_Scaler_Unit,-4,Unit_kvarh,
//}
};

extern const u8 Init_EnergyData_double_long_unsigned[];//��ʼ������double_long_unsigned����
extern const u8 Init_double_long[];//��ʼ������double_long
u32 GET_23xx_24xx(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//���ܼ�����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Len;
	u32 LENmax;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	switch(OAD>>24)
	{
		case 0x23://�ܼ���
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_TAG))
			{
				UARTCtrl->DAR=6;//���󲻴���
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 1://1���߼���	��static��
					if(LENmax_TxSPACE<4)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8tx[0]=DataType_octet_string;//long-unsigned
					p8tx[1]=2;
					p8tx[2]=OAD>>24;
					p8tx[3]=OAD>>16;
					return 4;
				case 2://2���ܼ����ñ�	��static��
					p8=(u8*)ADDR_230x_2+(n*LENper_230x_2);
					LENmax=LENper_230x_2;
					break;
				case 3://3���ܼ��й�����	��dyn.��
					p8=(u8*)ADDR_230x_3+(n*LENper_230x_3);
					LENmax=LENper_230x_3;
					break;
				case 4://4���ܼ��޹�����	��dyn.��
					p8=(u8*)ADDR_230x_4+(n*LENper_230x_4);
					LENmax=LENper_230x_4;
					break;
				case 5://5���ܼӻ���ʱ����ƽ���й�����	��dyn.��
					p8=(u8*)ADDR_230x_5+(n*LENper_230x_5);
					LENmax=LENper_230x_5;
					break;
				case 6://6���ܼӻ���ʱ����ƽ���޹�����	��dyn.��
					p8=(u8*)ADDR_230x_6+(n*LENper_230x_6);
					LENmax=LENper_230x_6;
					break;
				case 7://7���ܼ����й�����	��dyn.��
					p8=(u8*)ADDR_230x_7+(n*LENper_230x_7);
					LENmax=LENper_230x_7;
					break;
				case 8://8���ܼ����޹�����	��dyn.��
					p8=(u8*)ADDR_230x_8+(n*LENper_230x_8);
					LENmax=LENper_230x_8;
					break;
				case 9://9���ܼ����й�����	��dyn.��
					p8=(u8*)ADDR_230x_9+(n*LENper_230x_9);
					LENmax=LENper_230x_9;
					break;
				case 10://10���ܼ����޹�����	��dyn.��
					p8=(u8*)ADDR_230x_10+(n*LENper_230x_10);
					LENmax=LENper_230x_10;
					break;
				case 11://11���ܼ�ʣ��������ѣ�	��dyn.��
					p8=(u8*)ADDR_230x_11+(n*LENper_230x_11);
					LENmax=LENper_230x_11;
					break;
				case 12://12����ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ��dyn.��
					p8=(u8*)ADDR_230x_12+(n*LENper_230x_12);
					LENmax=LENper_230x_12;
					break;
				case 13://13���ܼ��黬��ʱ������	��static��
					p8=(u8*)ADDR_230x_13+(n*LENper_230x_13);
					LENmax=LENper_230x_13;
					break;
				case 14://14���ܼ��鹦���ִ�����	��static��
					p8=(u8*)ADDR_230x_14+(n*LENper_230x_14);
					LENmax=LENper_230x_14;
					break;
				case 15://15���ܼ������ִ�����	��static��
					p8=(u8*)ADDR_230x_15+(n*LENper_230x_15);
					LENmax=LENper_230x_15;
					break;
				case 16://16���ܼ����������״̬	��dyn.��
				#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					Contrl_SetState(n);//�ܼ����������״̬;���:�ܼ����0-7
				#endif
				#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					Contrl_SetState(n);//�ܼ����������״̬;���:�ܼ����0-7
				#endif
					p8=(u8*)ADDR_230x_16+(n*LENper_230x_16);
					LENmax=LENper_230x_16;
					break;
				case 17://17���ܼ��鵱ǰ����״̬	��dyn.��
					p8=(u8*)ADDR_230x_17+(n*LENper_230x_17);
					LENmax=LENper_230x_17;
					break;
				case 18://18. ���㼰��λ	��static��
					p8=(u8*)ADDR_230x_18;
					LENmax=sizeof(ADDR_230x_18);
					break;
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
			i=OAD&0xff;
			if(i==0)
			{//ȫ��Ԫ��
				Len=Get_DL698DataLen_S(p8,LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				if(LENmax_TxSPACE<Len)
				{
					GET_Request_Frame(PORTn);//��֡��־
					return 0;
				}
				MR((u32)p8tx,(u32)p8,Len);
			}
			else
			{//ָ��Ԫ��
				p8=Get_Element(p8,i,0,LENmax);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
				if(p8==0)
				{
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
				}
				Len=Get_DL698DataLen_S(p8,LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				if(LENmax_TxSPACE<Len)
				{
					GET_Request_Frame(PORTn);//��֡��־
					return 0;
				}
				MR((u32)p8tx,(u32)p8,Len);
			}
			return Len;
			
		case 0x24://�������
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_IMPIN))
			{
				UARTCtrl->DAR=6;//���󲻴���
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 1://1���߼���	��static��
					if(LENmax_TxSPACE<4)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8tx[0]=DataType_octet_string;//long-unsigned
					p8tx[1]=2;
					p8tx[2]=OAD>>24;
					p8tx[3]=OAD>>16;
					return 4;
				case 2://2��ͨ�ŵ�ַ	��static��
					p8=(u8*)ADDR_240x_2+(n*LENper_240x_2);
					LENmax=LENmax_240x_2;
					break;
				case 3://3������������	��static��
					p8=(u8*)ADDR_240x_3+(n*LENper_240x_3);
					LENmax=LENper_240x_3;
					break;
				case 4://4����������	��static��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4);
					LENmax=LENper_240x_4;
					break;
				case 5://5���й�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if((i==0)||(i==2))
					{
						p8=(u8*)ADDR_240x_5+(n*LENper_240x_5);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_double_long;
						LENmax=0;
					}
					break;
				case 6://6���޹�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if((i==1)||(i==3))
					{
						p8=(u8*)ADDR_240x_5+(n*LENper_240x_5);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_double_long;
						LENmax=0;
					}
					break;
				case 7://7�����������й�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==0)
					{
						p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 8://8�����������й�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==0)
					{
						p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 9://9�����շ����й�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==2)
					{
						p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 10://10�����·����й�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==2)
					{
						p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 11://11�����������޹�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==1)
					{
						p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 12://12�����������޹�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==1)
					{
						p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 13://13�����շ����޹�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==3)
					{
						p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 14://14�����·����޹�����	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==3)
					{
						p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 15://15�������й�����ʾֵ	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==0)
					{
						p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 16://16�������޹�����ʾֵ	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==1)
					{
						p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 17://17�������й�����ʾֵ	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==2)
					{
						p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 18://18�������޹�����ʾֵ	��dyn.��
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//�����й���0),�����޹���1���� �����й���2���������޹���3��
					if(i==3)
					{
						p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 19://19�����㼰��λ	��static��
					p8=(u8*)ADDR_240x_19;
					LENmax=sizeof(ADDR_240x_19);
					break;
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
			i=OAD&0xff;
			if(i==0)
			{//ȫ��Ԫ��
				Len=Get_DL698DataLen_S(p8,LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				if(LENmax_TxSPACE<Len)
				{
					GET_Request_Frame(PORTn);//��֡��־
					return 0;
				}
				MR((u32)p8tx,(u32)p8,Len);
			}
			else
			{//ָ��Ԫ��
				p8=Get_Element(p8,i,0,LENmax);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
				if(p8==0)
				{
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
				}
				Len=Get_DL698DataLen_S(p8,LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				if(LENmax_TxSPACE<Len)
				{
					GET_Request_Frame(PORTn);//��֡��־
					return 0;
				}
				MR((u32)p8tx,(u32)p8,Len);
			}
			return Len;
		default:
			UARTCtrl->DAR=6;//���󲻴���
			return 0;
	}
}

u32 SET_23xx_24xx(u32 PORTn,u8* p8rx,u32 LEN_RxAPDU)//�����ܼ�����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Len;
	u32 OAD;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	OAD=p8rx[0]<<24;
	OAD|=p8rx[1]<<16;
	OAD|=p8rx[2]<<8;
	OAD|=p8rx[3];
	p8rx+=4;
	Len=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
	switch(OAD>>24)
	{
		case 0x23://�ܼ���
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_TAG))
			{
				UARTCtrl->DAR=6;//���󲻴���
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 2://2���ܼ����ñ�	��static��
					if(Len>LENper_230x_2)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_230x_2+(n*LENper_230x_2);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 13://13���ܼ��黬��ʱ������	��static��
					if(Len>LENper_230x_13)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_230x_13+(n*LENper_230x_13);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 14://14���ܼ��鹦���ִ�����	��static��
					if(Len>LENper_230x_14)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_230x_14+(n*LENper_230x_14);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 15://15���ܼ������ִ�����	��static��
					if(Len>LENper_230x_15)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_230x_15+(n*LENper_230x_15);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
		case 0x24://�������
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_IMPIN))
			{
				UARTCtrl->DAR=6;//���󲻴���
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 2://2��ͨ�ŵ�ַ	��static��
					if(Len>LENper_240x_2)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_240x_2+(n*LENper_240x_2);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 3://3������������	��static��
					if(Len>LENper_240x_3)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_240x_3+(n*LENper_240x_3);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 4://4����������	��static��
					if(Len>LENper_240x_4)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
		default:
			UARTCtrl->DAR=6;//���󲻴���
			return Len;
	}
}

u32 ACTION_23xx_24xx(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//�����ܼ�����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 x;
	u32 LenRx;
	u32 LenTx;
	u32 OAD;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	OAD=p8rx[0]<<24;
	OAD|=p8rx[1]<<16;
	OAD|=p8rx[2]<<8;
	OAD|=p8rx[3];
	p8rx+=4;
	switch(OAD>>24)
	{
		case 0x23://�ܼ���
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_TAG))
			{
				UARTCtrl->DAR=6;//���󲻴���
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 1://1	����ܼ����ñ�(����) ������=NULL
					Init_Data230x();//��ʼ���ܼ�������
					MWR(DataType_array,ADDR_230x_2+(n*LENper_230x_2),2);//���ʱ������=0
					LenRx=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 2://2	ִ�У������� ������=Data
					LenRx=2;
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 3://3	���һ���ܼ����õ�Ԫ��������������=�ܼ����õ�Ԫ
			#if LENper_230x_2>(LEN_DATABUFF/2)
				#error
			#endif
//�ܼ����ñ��=array �ܼ������õ�Ԫ//���ʱ������=0
//�ܼ����õ�Ԫ��=structure
//{
//�����ܼӵķ�·ͨ�ŵ�ַ  TSA��
//�ܼӱ�־	            enum{����0��������1��}��
//�������־	            enum{�ӣ�0��������1��}
//}
					LenRx=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					i=p8rx[3]<<8;
					i|=p8rx[4];
					if(i!=0x0705)
					{
						UARTCtrl->DAR=255;
						LenTx=0;
						return (LenRx<<16)+LenTx;
					}
					MR(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					p8=(u8*)ADDR_DATABUFF;
					p8[0]=DataType_array;
					n=p8[1];//������
					p8+=2;
					for(i=0;i<n;i++)
					{
						LenTx=Compare_DL698DataString(p8rx+5,p8+5,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(LenTx==0)
						{//��ͬ
							break;
						}
						p8+=15;
					}
					MW((u32)p8rx,(u32)p8,15);
					p8=(u8*)ADDR_DATABUFF;
					if(i>=n)
					{//����
						n++;
						p8[1]=n;
					}
					LenTx=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					if(LenTx>LENper_230x_2)
					{
						UARTCtrl->DAR=255;
						LenTx=0;
						return (LenRx<<16)+LenTx;
					}
					n=(OAD>>16)&0xff;
					n--;
					MW(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 4://4	��������ܼ����õ�Ԫ��������������=array�ܼ����õ�Ԫ
					LenRx=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					x=p8rx[1];//������Ӹ���
					p8rx+=2;
					MR(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					while(x--)
					{
						i=p8rx[3]<<8;
						i|=p8rx[4];
						if(i!=0x0705)
						{
							UARTCtrl->DAR=255;
							LenTx=0;
							return (LenRx<<16)+LenTx;
						}
						
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_array;
						n=p8[1];//������
						p8+=2;
						for(i=0;i<n;i++)
						{
							LenTx=Compare_DL698DataString(p8rx+5,p8+5,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(LenTx==0)
							{//��ͬ
								break;
							}
							p8+=15;
						}
						MW((u32)p8rx,(u32)p8,15);
						p8=(u8*)ADDR_DATABUFF;
						if(i>=n)
						{//����
							n++;
							p8[1]=n;
						}
						p8=(u8*)ADDR_DATABUFF;
						LenTx=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
						if(LenTx>LENper_230x_2)
						{
							UARTCtrl->DAR=255;
							LenTx=0;
							return (LenRx<<16)+LenTx;
						}
						p8rx+=15;
					}
					n=(OAD>>16)&0xff;
					n--;
					MW(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 5://5	ɾ��һ���ܼ����õ�Ԫ��������������=�����ܼӵķ�·ͨ�ŵ�ַTSA
					LenRx=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					MR(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					p8=(u8*)ADDR_DATABUFF;
					n=p8[1];//������
					p8+=2;
					for(i=0;i<n;i++)
					{
						LenTx=Compare_DL698DataString(p8rx+3,p8+5,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(LenTx==0)
						{//�ҵ�
							MR((u32)p8,(u32)p8+15,(n-(i+1))*15);
							p8=(u8*)ADDR_DATABUFF;
							n--;
							p8[1]=n;
							n=(OAD>>16)&0xff;
							n--;
							MW(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
							LenTx=0;
							return (LenRx<<16)+LenTx;
						}
						p8+=15;
					}
					//û�ҵ�
					UARTCtrl->DAR=6;//���󲻴���
					LenTx=0;
					return (LenRx<<16)+LenTx;
				default:
					UARTCtrl->DAR=255;
					LenRx=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					LenTx=0;
					return (LenRx<<16)+LenTx;
			}
		case 0x24://�������
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_IMPIN))
			{
				UARTCtrl->DAR=6;//���󲻴���
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 1://1	��λ��������������=bit-string
				case 2://2	ִ�У�������������=Data
					LenRx=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 3://3	����������뵥Ԫ�����嵥Ԫ��
				#if LENper_240x_4>(LEN_DATABUFF/2)
					#error
				#endif
//���嵥Ԫ��=structure
//{
//��������˿ں�	OAD��//0xF20A0201
//��������		enum
//{
//   �����й���0����
//	 �����޹���1����
//	 �����й���2����
//	 �����޹���3��
//}��
//���峣��k     long-unsigned
//}
					LenRx=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					p8=(u8*)ADDR_DATABUFF;
					p8[0]=DataType_array;
					p8[1]=1;//������
					MR((u32)p8+2,(u32)p8rx,12);
					MW(ADDR_DATABUFF,ADDR_240x_4+(n*LENper_240x_4),LENper_240x_4);
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 4://4	ɾ���������뵥Ԫ����������˿ںţ�
					LenRx=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					MWR(DataType_array,ADDR_240x_4+(n*LENper_240x_4),2);
					LenTx=0;
					return (LenRx<<16)+LenTx;
				default:
					LenRx=0;
					LenTx=0;
					return (LenRx<<16)+LenTx;
			}
		default:
			UARTCtrl->DAR=6;//���󲻴���
			LenRx=0;
			LenTx=0;
			return (LenRx<<16)+LenTx;
	}
}















