
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../Device/MEMRW.h"
#include "../DL698/CALC_TAG.h"
#include "../Calculate/Calculate.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_REPORT.h"


//extern u32 RCSDtoID(u8* pRCSD);//��¼��ѡ��RCSD���Ӧ�������,����:�����ڲɼ�����������,>=RMTASKmax��ʾ��Ч�����


#define INVALID_DATA 0xffffffffffffffff


const u8 Init_long64[]=
{
	DataType_long64,
	0,0,0,0,0,0,0,0,
};
const u8 Init_EnergyData_long64[]=
{
	DataType_array,
	TMaxTariff+1,
	DataType_long64,
	0,0,0,0,0,0,0,0,
	DataType_long64,
	0,0,0,0,0,0,0,0,
	DataType_long64,
	0,0,0,0,0,0,0,0,
	DataType_long64,
	0,0,0,0,0,0,0,0,
	DataType_long64,
	0,0,0,0,0,0,0,0,
#if TMaxTariff>4
	#error
#endif
};


void Init_Data230x_D(void)//��ʼ���ܼ���������
{
	u32 n;
	u8* p8;
	
//�ܼ����й�����	
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_7+(n*LENper_230x_7);
		MW((u32)Init_EnergyData_long64,(u32)p8,sizeof(Init_EnergyData_long64));
	}
//�ܼ����޹�����
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_8+(n*LENper_230x_8);
		MW((u32)Init_EnergyData_long64,(u32)p8,sizeof(Init_EnergyData_long64));
	}
}

void Init_Data230x_M(void)//��ʼ���ܼ���������
{
	u32 n;
	u8* p8;
	
//�ܼ����й�����
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_9+(n*LENper_230x_9);
		MW((u32)Init_EnergyData_long64,(u32)p8,sizeof(Init_EnergyData_long64));
	}
//�ܼ����޹�����
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_10+(n*LENper_230x_10);
		MW((u32)Init_EnergyData_long64,(u32)p8,sizeof(Init_EnergyData_long64));
	}
}







void Init_Data230x(void)//��ʼ���ܼ�������
{
	u32 n;
	u8* p8;
	
//�ܼ��й�����
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_3+(n*LENper_230x_3);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//�ܼ��޹�����
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_4+(n*LENper_230x_4);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//�ܼӻ���ʱ����ƽ���й�����
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_5+(n*LENper_230x_5);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//�ܼӻ���ʱ����ƽ���޹�����
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_6+(n*LENper_230x_6);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
	
//�ܼӵ���������ԭʼ����ʾֵ
	MC(0,ADDR_230x_Sour,LENmax_230x_Sour);
//�ܼ���������
	Init_Data230x_D();//��ʼ���ܼ���������
//�ܼ���������
	Init_Data230x_M();//��ʼ���ܼ���������
	
//�ܼ�ʣ��������ѣ�
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_11+(n*LENper_230x_11);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_12+(n*LENper_230x_12);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//�ܼ��黬��ʱ������
//�ܼ��鹦���ִ�����
//�ܼ������ִ�����
	
//�ܼ����������״̬
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_16+(n*LENper_230x_16);
//�ܼ����������״̬��=structure
		p8[0]=DataType_structure;
		p8[1]=6;
//{
//ʱ�οض�ֵ������	   unsigned��
		p8[2]=DataType_unsigned;
		p8[3]=0;
//����ʱ����Ч��־λ  bit-string(SIZE(8))��
		p8[4]=DataType_bit_string;
		p8[5]=8;
		p8[6]=0;
//����״̬            PCState��
		p8[7]=DataType_bit_string;
		p8[8]=8;
		p8[9]=0;
//���״̬            ECState��
		p8[10]=DataType_bit_string;
		p8[11]=8;
		p8[12]=0;
//�����ִ�״̬        TrunState��
		p8[13]=DataType_bit_string;
		p8[14]=8;
		p8[15]=0;
//����ִ�״̬        TrunState
		p8[16]=DataType_bit_string;
		p8[17]=8;
		p8[18]=0;
//}
	}
//�ܼ����������״̬
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_17+(n*LENper_230x_17);
//��ǰ����״̬��=structure
		p8[0]=DataType_structure;
		p8[1]=7;
//{
//	��ǰ���ض�ֵ             long64����λ��W ���㣺-1����   
		p8[2]=DataType_long64;
		p8[3]=0;
		p8[4]=0;
		p8[5]=0;
		p8[6]=0;
		p8[7]=0;
		p8[8]=0;
		p8[9]=0;
		p8[10]=0;
//	��ǰ�����¸��ظ���ϵ��   integer����λ��%��
		p8[11]=DataType_integer;
		p8[12]=0;
//	������բ���״̬         OutputState��
		p8[13]=DataType_bit_string;
		p8[14]=8;
		p8[15]=0;
//	�µ����բ���״̬       OutputState��
		p8[16]=DataType_bit_string;
		p8[17]=8;
		p8[18]=0;
//	�������բ���״̬       OutputState��
		p8[19]=DataType_bit_string;
		p8[20]=8;
		p8[21]=0;
//	����Խ�޸澯״̬         PCAlarmState��
		p8[22]=DataType_bit_string;
		p8[23]=8;
		p8[24]=0;
//	���Խ�޸澯״̬         ECAlarmState
		p8[25]=DataType_bit_string;
		p8[26]=8;
		p8[27]=0;
//}
	}
}





//pAddr��TSA������ADDR_6000����ܼ������������0
//����Ǵ��ڵģ�����CT,PT��0�ģ�Ĭ�ϳ�1
u32 Get_CT_PT(u16 * pAddr)//��ȡTSA��Ӧ��CT,PTֵ��CT�Ÿ�2�ֽڣ�PT�ŵ�2�ֽڣ�ֵ�Ѿ��ߵ�λ�������ˣ�����ֱ��ʹ�á�
{
	u32 i;
	u32 j;
	u32 CTPT=0;
	u8 *p8;
	
	i=AddrToMeterSetNo(pAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
	if(i<NUM_RMmax)//�����õ�������
	{
		p8=Get_Element((u8 *)(ADDR_6000_SDRAM+LENper_6000*i), 2,4,LENper_6000);
		if(p8==0)
		{
			return CTPT;
		}
		j=MRR((u32)(p8+1),4);
		j=R_Inverse(j,4);
		if((j&0xffffff00)==0xf20a0200)//��6000��˿�����壬Ҳ������6000����
		{
			i=NUM_RMmax;
		}
	}

	if(i<NUM_RMmax)//�����õ�������
	{
		j=ADDR_6000_SDRAM+LENper_6000*i;
		j=Get_ElementADDR(ADDR_6000_SDRAM+LENper_6000*i,3,3,LENper_6000);//CT = 0X12 XX XX 
		if(j!=0)
		{
			j=MRR(j+1,2);
			j=R_Inverse(j,2);
		}
		if(j==0)//����������ó�0������û�ҵ���������Ĭ�ϳ�1
		{
			j=1;
		}
		CTPT|=(j<<16);
		j=Get_ElementADDR(ADDR_6000_SDRAM+LENper_6000*i,3,4,LENper_6000);//PT = 0X12 XX XX 
		if(j!=0)
		{
			j=MRR(j+1,2);
			j=R_Inverse(j,2);
		}
		if(j==0)//����������ó�0������û�ҵ���������Ĭ�ϳ�1
		{
			j=1;
		}
		CTPT|=j;
		return CTPT;
	}
	//����˵��TSA������ADDR_6000���棬����������
#if NUMmax_IMPIN!=0
	#if LENmax_240x_2>LEN_DATABUFF
		#error LENmax_240x_2 Over
	#endif		
	MR(ADDR_DATABUFF,ADDR_240x_2,LENmax_240x_2);
	for(i=0;i<NUMmax_IMPIN;i++)
	{
		if(Is_TSA_Same((u8*)pAddr,(u8 *)(ADDR_DATABUFF+i*LENper_240x_2+2))==0)
		{
			j=Get_ElementADDR(ADDR_240x_3+i*LENper_240x_3,2,0,LENper_240x_3);
			if(j!=0)
			{
				j=MRR(j+1,2);
				j=R_Inverse(j,2);
			}
			if(j==0)//����������ó�0������û�ҵ���������Ĭ�ϳ�1
			{
				j=1;
			}
			CTPT|=(j<<16);
			j=Get_ElementADDR(ADDR_240x_3+i*LENper_240x_3,1,0,LENper_240x_3);
			if(j!=0)
			{
				j=MRR(j+1,2);
				j=R_Inverse(j,2);
			}
			if(j==0)//����������ó�0������û�ҵ���������Ĭ�ϳ�1
			{
				j=1;
			}
			CTPT|=j;
			return CTPT;	
		}
	}
	return CTPT;
#else
	return 0;
#endif
}

u32 Is_TSA_Same(u8 *pAddr1,u8 *pAddr2)//�жϵ�ַ�Ƿ���ͬ,���� 0=��ͬ 1=����ͬ
{
	u32 index;
	
	if((pAddr1==0)||(pAddr2==0))//�βδ���
	{
		return 1;
	}
	for(index=0;index<6;index++)
	{
		if(pAddr1[index]!=pAddr2[index])
		{
			return 1;
		}
	}
	return 0;
}
//UART�������ݻ��嶨��
//#define LEN_RM_DI_CSD     512
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)

//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)

//pbuf�Ǵ�Get_ADDR_UARTnFnDataBuff(PORTn)�õ���
//������pbuf���ҳ���ǰ����oad��Ӧ���ݵĵ�ַ��oadֻ���ǵ�ǰ���ݣ���ǰ�����й�����ʾֵ������,���� ��ַ������������
u32 Is_FnDataBuff_Have_TAGData(u32 oad,u8* FnDataBuff)//�ж�FnDataBuff�Ƿ�����ܼ�������ݣ����� 0=������oad�����ݣ�!=0��oad��Ӧ�����ݵ�ַ
{
	u32 pos=0;//��¼FnDataBuff�����ݿ�ʼ�ĵ�ַ
	u32 index=0;
	u32 num=0xffff;//�β�����Ҫ��ѯoad�ڼ�¼���е���ţ�=0xffff ��ʾ��¼��û�и�oad������
	u32 sum=0;//��¼oad��������road�ͼ������oad�ĸ���
	u32 i;
	if(FnDataBuff==0)//�βδ���
	{
		return 0;
	}

	pos=31;//��λ��  �ɼ���CSD  array CSD(����������)��ARRAY N CSD-1 CSD-2 CSD-N
	for(index=0;index<FnDataBuff[30];index++)
	{
		pos+=1;
		if(FnDataBuff[pos]==1)//road
		{
			pos+=1;
			pos+=4;
			i=FnDataBuff[pos];
			pos+=i*4;
		}
		if(FnDataBuff[pos]==0)//oad
		{
			pos+=1;
			i=MRR((u32)FnDataBuff+pos,4);
			i=R_Inverse(i,4);
			pos+=4;
			if(i==oad)//�ҵ��βε�oad�ˣ���¼�¸�oad�ڼ�¼�����ţ�����Ŵ�0��ʼ
			{
				num=sum;
			}
			sum+=1;
		}
	}
	i=(u32)FnDataBuff+pos;//��������ã������塣
	if(num==0xffff)//��֡���ݼ�¼��û��oad��ֱ�ӷ���0
	{
		return 0;
	}
	while(num)//�����ݿ�ʼ������ǰ������ݣ���ָ���β�oad��������ʼ��ַ�ˡ�
	{
		i=Get_DL698DataLen_S(FnDataBuff+pos,0);
		pos+=i;
		num-=1;
	}
	return (u32)FnDataBuff+pos;
}

//data_sour����������:
//�й�
//��������=double-long-unsigned����λ��kWh�����㣺-2  06 NN NN NN NN 
//�߾��ȵ�������=long64-unsigned����λ��kWh�����㣺-4  0X15 NN NN NN NN NN NN NN NN 

//�޹�
//��������=double-long����λ��kvarh�����㣺-2
//�߾��ȵ�������=long64����λ��kvarh�����㣺-4

//data_sour��ֵ���� CT PT֮�����data_dest��
//u32 TAG_NO���ܼ�����ţ���0��ʼ��u32 add_mise���������־0=�� 1=��
//u32 type 0=���� 1=���� 2=���� 3=����
//u32 group_type=0;//�ܼӱ�־ 0=���� 1=����
//ԭʼֵ�� 0.1W �ľ������洢,���������� long64-unsigned  0x15 
//u32 pn_type ���ò��������� 0=6000  1=����

void Do_Update_ADDR_230x_Sour(u16 * pAddr,u32 oad_data_addr,u32 data_dest,u32 TAG_NO,u32 add_mise,u32 type,u32 group_type,u32 pn_type)//pAddr1��TSA��data_sour��01 05 06 XXXX 06 XXXX,data_dest 0X14 XXXXXXXX��
{
	u32 CTPT;
	u32 index;
	u64 value_sour;//���data_sour��ֵ�����³���������
	u64 value_dest;//���֮ǰdata_dest���ֵ����֮ǰ��ŵ�����
	u64 value_add=0;//����
	u8 *psour=(u8 *)oad_data_addr;
	u8 *pdest=(u8 *)data_dest;
	u32 data_type=0xff;//0=double-long-unsigned  1=long64-unsigned
	
	CTPT=Get_CT_PT(pAddr);
	if(CTPT==0)
	{
		CTPT=0x00010001;
	}
	if((psour[0]!=DataType_array)||(psour[1]==0))
	{
		return;
	}
	if(psour[2]==DataType_double_long_unsigned)
	{
		data_type=0;
	}
	else if(psour[2]==DataType_long64_unsigned)
	{
		data_type=1;
	}
	else if(psour[2]==DataType_double_long)
	{
		data_type=2;
	}
	else if(psour[2]==DataType_long64)
	{
		data_type=3;
	}

	if(data_type==0xff)
		return;
	for(index=0;index<psour[1];index++)
	{
		if((data_type==0)||(data_type==2))//DataType_double_long_unsigned
		{
			value_sour=MRR(oad_data_addr+2+index*5+1,4);
			value_sour=R_Inverse(value_sour,4);
			if(pn_type==0)
				value_sour*=100;// KW -2 ת�� 0.1W
			value_sour*=(CTPT&0xffff);
			value_sour*=((CTPT>>16)&0xffff);
		}
		else//DataType_long64_unsigned
		{
			value_sour=MRR(oad_data_addr+2+index*9+1,8);
			value_sour=R_Inverse(value_sour,8);
			value_sour*=(CTPT&0xffff);
			value_sour*=((CTPT>>16)&0xffff);
		}
		if(pdest[index*9]==DataType_long64_unsigned)//˵��֮ǰdata_dest���Ѿ����������
		{
			value_dest=MRR(data_dest+index*9+1,8);
			value_dest=R_Inverse(value_dest,8);
			if(value_dest>value_sour)//�µ�ֵ���ϵ�ֵС, ��ͨ��������ڣ������ǵ���Ѿ��ǵ�����ˣ��ֻص�0
			{
				if(value_dest>9000000000)
				{//��ǰֵ��0
					value_add=value_sour;
					value_add+=10000000000;
					value_add-=value_dest;
				}
				else//���󣬲�Ӧ��
				{
					value_add=0;
				}
			}
			else//�µ�ֵ���ϵ�ֵ��
			{
				value_add=value_sour-value_dest;
			}
			//value_add=1;//debug�ã�ÿ��������1
			if(value_add!=0)//����������0
			{
				if((group_type==0)&&(type==0))//�ܼӱ�־ 0=���� 1=����
				{
					Update_ADDR_230x_x(ADDR_230x_7,LENper_230x_7,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_x(ADDR_230x_9,LENper_230x_9,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_11(value_add,TAG_NO,index);
				}
				else if((group_type==0)&&(type==1))//�ܼӱ�־ 0=���� 1=����
				{
					Update_ADDR_230x_x(ADDR_230x_8,LENper_230x_8,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_x(ADDR_230x_10,LENper_230x_10,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_11(value_add,TAG_NO,index);
				}
				else if((group_type==1)&&(type==2))//�ܼӱ�־ 0=���� 1=����
				{
					Update_ADDR_230x_x(ADDR_230x_7,LENper_230x_7,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_x(ADDR_230x_9,LENper_230x_9,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_11(value_add,TAG_NO,index);
				}
				else if((group_type==1)&&(type==3))//�ܼӱ�־ 0=���� 1=����
				{
					Update_ADDR_230x_x(ADDR_230x_8,LENper_230x_8,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_x(ADDR_230x_10,LENper_230x_10,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_11(value_add,TAG_NO,index);
				}
			}
		}
		else//��һ�ν������������������׶�
		{
			pdest[index*9]=DataType_long64_unsigned;
		}
		//���³���ֵ
		MWR(value_sour,data_dest+index*9+1,8);
		Data_Inverse(data_dest+index*9+1,8);
	}
}
//u32 TAG_NO���ܼ�����ţ���0��ʼ��u32 add_mise���������־0=�� 1=��
//u32 type 0=���� 1=���� 2=���� 3=����
//u32 group_type �ܼӱ�־ 0=���� 1=����
//u32 pn_type ���ò��������� 0=6000  1=����
void Update_ADDR_230x_Sour(u16 * pAddr,u32 oad_data_addr,u32 TAG_NO,u32 add_mise,u32 type,u32 group_type,u32 pn_type)//�����ݸ��µ�ADDR_230x_Sour��֮ǰû�и�TSA�ľ��¼�
{
	u8 *p_data;
	u32 index;
	p_data=(u8 *)(ADDR_230x_Sour+LENper_230x_Sour*TAG_NO);
	if(p_data[0]>NUMmax_230x)
	{
		p_data[0]=NUMmax_230x;
	}
	for(index=0;index<p_data[0];index++)
	{
		if(Is_TSA_Same((u8 *)pAddr,(u8 *)(ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+index*LENsig_230x_sour))==0)
		{
			Do_Update_ADDR_230x_Sour(pAddr,oad_data_addr,ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+index*LENsig_230x_sour+6+type*9*(TMaxTariff+1),TAG_NO,add_mise,type,group_type,pn_type);
			break;
		}
	}
	if(index>=p_data[0])//�����˵��֮ǰû�����Ҫ�¼ӣ��ӵ�p_data[0]λ�ã����ж��Ƿ���������
	{
		if(p_data[0]<(NUMmax_230x-1))
		{
			MW((u32)pAddr,ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+p_data[0]*LENsig_230x_sour,6);
			MC(0,ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+p_data[0]*LENsig_230x_sour+6,LENsig_230x_sour-6);//��Ϊ���¼ӣ�����ԭ�������ݡ�
			Do_Update_ADDR_230x_Sour(pAddr,oad_data_addr,ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+p_data[0]*LENsig_230x_sour+6+type*9*(TMaxTariff+1),TAG_NO,add_mise,type,group_type,pn_type);
			p_data[0]+=1;//������1
		}
	}
}
//u32 TAG_NO���ܼ�����ţ���0��ʼ��u32 add_mise���������־0=�� 1=��;u32 fee ������ ��0��ʼ,0=�ܣ�1=��...
//value�ľ�����0.1W,  0.1var
//u32 addr_data �������ݵĵ�ַ��u32 LENper
void Update_ADDR_230x_x(u32 addr_data,u32 LENper,u64 value,u32 TAG_NO,u32 add_mise,u32 fee)//�����ܼӵ���
{
	u8 *p_data;
	u32 index;
	u64 value_sour;
	p_data=(u8 *)(addr_data+LENper*TAG_NO);
	if((p_data[0]==0)||(p_data[1]==0))
	{
		MC(0,addr_data+LENper*TAG_NO,LENper);
		p_data[0]=DataType_array;
	}
	p_data[2+fee*9]=DataType_long64;
	value_sour=MRR(addr_data+LENper*TAG_NO+2+fee*9+1,8);
	value_sour=R_Inverse(value_sour,8);
	if(value_sour>>63)//ԭ���Ǹ�����
	{
		value_sour=(~value_sour)+1;//ת������
		if(add_mise==0)//��
		{
			if(value_sour>value)//ԭֵ�����������Ǹ���
			{
				value_sour-=value;
				value_sour=(~value_sour)+1;
			}
			else//ԭֵ��������������ֱ��
			{
				value_sour=value-value_sour;
			}
		}
		else//��ֵ��ȥһ��ֵ���ڸ�ֵ
		{
			value_sour+=value;
			value_sour=(~value_sour)+1;
		}
	}
	else//ԭ��������
	{
		if(add_mise==0)//��
		{
			value_sour+=value;
		}
		else//��
		{
			if(value_sour>=value)
			{
				value_sour-=value;
			}
			else
			{
				value_sour=value-value_sour;
				value_sour=(~value_sour)+1;
			}
		}
	}

	MWR(value_sour,addr_data+LENper*TAG_NO+2+fee*9+1,8);
	Data_Inverse(addr_data+LENper*TAG_NO+2+fee*9+1,8);
	value_sour=0;
	for(index=0;index<(TMaxTariff+1);index++)
	{
		if(p_data[2+index*9]==DataType_long64)
			value_sour+=1;
	}
	p_data[1]=value_sour;
	return;
}

void Update_ADDR_230x_11(u64 value,u32 TAG_NO,u32 fee)//�ܼ�ʣ��������ѣ�
{
	u8 *p_data;
	u64 value_sour;
	u32 i;//0=������,1=�����
	u32 j;

	i=MRR(ADDR_Terminal_kWhCtrl2_State+TAG_NO*Terminal_Ctrl_Pn_Len,1);
	if(i==0)//���ܼ���Ĺ����û��Ͷ�룬�������ˡ�
	{
		return;
	}
	i=MRR(ADDR_Terminal_kWhCtrl2_State+TAG_NO*Terminal_Ctrl_Pn_Len+7,1);
	if(i==1)//��ʣ���ѣ���Ҫ�ҳ���ǰʱ���Ƿ������Ͷ�Ӧ�ķ���
	{
		if(fee==0)//�п����ǵ�ѣ�Ҫ������ʱ�ΰѵ����Ǯ�����Բ����ܡ�
	  {
			return;
		}	
		i=MRR(ADDR_METER07_CTRL_SRAM,1);//1BYTE ���ܱ�ǰִ�з���(HEX ����1,2,3,4 �ֱ��Ӧ���,��,ƽ,�ȵȷ���)
		j=Get_ElementADDR(ADDR_4018,i,0,LENmax_4018);//��λ���ʵ��
		if(j!=0)
		{
			j=MRR(j+1,4);//��λ��Ԫ/kWh�����㣺-4,double-long-unsigned
			j=R_Inverse(j,4);
			value*=j;//�����������Է��ʾ͵��ڵ��
		}
		//��������ڣ����൱�ڷ�����1
	}
	else//��ʣ�ڵ���
	{
		if(fee!=0)//�ͼ����ܣ�̨������ֻ����
		{
			return;
		}
	}
	p_data=(u8 *)(ADDR_230x_11+LENper_230x_11*TAG_NO);
	if(p_data[0]==0)
	{
		MC(0,ADDR_230x_11+LENper_230x_11*TAG_NO,LENper_230x_11);
		p_data[0]=DataType_long64;
	}
	value_sour=MRR(ADDR_230x_11+LENper_230x_11*TAG_NO+1,8);
	value_sour=R_Inverse(value_sour,8);
	if(value_sour>>63)//ԭ���Ǹ�����
	{
		value_sour=(~value_sour)+1;//ת������
		value_sour+=value;
		value_sour=(~value_sour)+1;//ת�ɸ���
	}
	else//ԭ��������
	{
		if(value_sour>=value)
		{
			value_sour-=value;
		}
		else
		{
			value_sour=value-value_sour;
			value_sour=(~value_sour)+1;//ת�ɸ���
		}
	}
	MWR(value_sour,ADDR_230x_11+LENper_230x_11*TAG_NO+1,8);
	Data_Inverse(ADDR_230x_11+LENper_230x_11*TAG_NO+1,8);
}

void Update_Group_Total_Data_6000(u8 *FnDataBuff)//�����ܼ��������,���ñ���6000���
{
	u32 TAG_NO;//�ܼ�����ţ���0��ʼ
	u32 index;
	u8 *p_data;
	u8 *p8;//ָ��Get_Element��ִ�н����=0��ʾû�ҵ�
	u32 oad_data_addr=0;//oad��Ӧ������pbuf��ĵ�ַ��=0��ʾû�ҵ�
	u32 add_mise=0;//�������־ 0=�� 1=��
	u32 group_type=0;//�ܼӱ�־ 0=���� 1=����
	ms_Type *ms;
	
	ms=Get_ms();
	p_data=ms->ms_malloc((size_t)LENmax_230x_2);//976�ֽ�
	if((u32)p_data==0)//��̬����ʧ��
	{
		return;
	}
 	MR((u32)p_data,ADDR_230x_2,LENmax_230x_2);
	
	for(TAG_NO=0;TAG_NO<NUMmax_TAG;TAG_NO++)
	{
		if(p_data[LENper_230x_2*TAG_NO]==DataType_array)//���ܼ��������˲���
		{
			for(index=0;index<p_data[LENper_230x_2*TAG_NO+1];index++)//���ܼ������ò�������TSA
			{
				p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,1,LENper_230x_2);
				if(p8!=0)
				{
					if(Is_TSA_Same((u8 *)(p8+3),(u8 *)FnDataBuff)==0)//���ܼ����������˸�TSA
					{
						p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,2,LENper_230x_2);//��ȡ�ܼӱ�־
						if(p8!=0)
						{
							group_type=p8[1];//enum{����0��������1��}��
						}
						p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,3,LENper_230x_2);//��ȡ�������־
						if(p8!=0)
						{
							add_mise=p8[1];//enum{�ӣ�0��������1��}
						}	
						oad_data_addr=Is_FnDataBuff_Have_TAGData(0x00100200,FnDataBuff);//�ж�FnDataBuff�Ƿ�����ܼ�������ݣ����� 0=������oad�����ݣ�!=0��oad��Ӧ�����ݵ�ַ
						if(oad_data_addr!=0)
							Update_ADDR_230x_Sour((u16 *)FnDataBuff,oad_data_addr,TAG_NO,add_mise,0,group_type,0);
						oad_data_addr=Is_FnDataBuff_Have_TAGData(0x00200200,FnDataBuff);//�ж�FnDataBuff�Ƿ�����ܼ�������ݣ����� 0=������oad�����ݣ�!=0��oad��Ӧ�����ݵ�ַ
						if(oad_data_addr!=0)
							Update_ADDR_230x_Sour((u16 *)FnDataBuff,oad_data_addr,TAG_NO,add_mise,2,group_type,0);
						oad_data_addr=Is_FnDataBuff_Have_TAGData(0x00300200,FnDataBuff);//�ж�FnDataBuff�Ƿ�����ܼ�������ݣ����� 0=������oad�����ݣ�!=0��oad��Ӧ�����ݵ�ַ
						if(oad_data_addr!=0)
							Update_ADDR_230x_Sour((u16 *)FnDataBuff,oad_data_addr,TAG_NO,add_mise,1,group_type,0);
						oad_data_addr=Is_FnDataBuff_Have_TAGData(0x00400200,FnDataBuff);//�ж�FnDataBuff�Ƿ�����ܼ�������ݣ����� 0=������oad�����ݣ�!=0��oad��Ӧ�����ݵ�ַ
						if(oad_data_addr!=0)
							Update_ADDR_230x_Sour((u16 *)FnDataBuff,oad_data_addr,TAG_NO,add_mise,3,group_type,0);
					}
				}
			}
		}
	}
	ms->ms_free(p_data);
}
void Update_Group_Total_Data_240x(void)//�����ܼ��������,���ñ���������
{
	u32 TAG_NO;//�ܼ�����ţ���0��ʼ
	u32 index;
	u8 *p_data;
	u8 *p_240x;
	u8 *p8;//ָ��Get_Element��ִ�н����=0��ʾû�ҵ�
	u32 add_mise=0;//�������־ 0=�� 1=��
	u32 group_type=0;//�ܼӱ�־ 0=���� 1=����
	u32 imp_type=0;//��������  �����й���0���������޹���1�� �����й���2���������޹���3��
	u32 k;
	
	ms_Type *ms;
	ms=Get_ms();
	p_data=ms->ms_malloc((size_t)LENmax_230x_2);//976�ֽ�
	if((u32)p_data==0)//��̬����ʧ��
	{
		return;
	}
	MR((u32)p_data,ADDR_230x_2,LENmax_230x_2);
	
	p_240x=ms->ms_malloc((size_t)((ADDR_DL698_SET_END)-(ADDR_240x_2)));//976�ֽ�
	if((u32)p_240x==0)//��̬����ʧ��
	{
		ms->ms_free(p_data);
		return;
	}
	MR((u32)p_240x,ADDR_240x_2,((ADDR_DL698_SET_END)-(ADDR_240x_2)));

	
	for(TAG_NO=0;TAG_NO<NUMmax_TAG;TAG_NO++)
	{
		if(p_data[LENper_230x_2*TAG_NO]==DataType_array)//���ܼ��������˲���
		{
			for(index=0;index<p_data[LENper_230x_2*TAG_NO+1];index++)
			{
				p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,1,LENper_230x_2);//�ҳ��ܼ������TSA
				if(p8!=0)
				{
				#if NUMmax_IMPIN!=0
					for(k=0;k<NUMmax_IMPIN;k++)
					{
						if(Is_TSA_Same((u8 *)(p8+3),(u8 *)(p_240x+k*LENper_240x_2+2))==0)//�ܼ������TSA������ĵ�ַһ��
						{
							p8=Get_Element(p_240x+LENmax_240x_2+LENmax_240x_3+k*LENper_240x_4,1,2,LENper_240x_4);//��ȡ�ܼӱ�־
							if(p8!=0)
							{
								imp_type=p8[1];//��������  �����й���0���������޹���1�� �����й���2���������޹���3��
							}
							p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,2,LENper_230x_2);//��ȡ�ܼӱ�־
							if(p8!=0)
							{
								group_type=p8[1];//enum{����0��������1��}��
							}
							p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,3,LENper_230x_2);//��ȡ�������־
							if(p8!=0)
							{
								add_mise=p8[1];//enum{�ӣ�0��������1��}
							}
							Update_ADDR_230x_Sour((u16 *)(p_240x+k*LENper_240x_2+2),ADDR_impEnergy+k*LENper_impEnergy,TAG_NO,add_mise,imp_type,group_type,1);
						}
					}
				#else
					add_mise=add_mise;
					group_type=group_type;
					imp_type=imp_type;
					k=k;
				#endif
				}
			}
		}
	}
	ms->ms_free(p_data);
	ms->ms_free(p_240x);
	Calc_TAG_Power();
}

void Calc_TAG_Power(void)//����ȫ���ܼ���Ĺ���
{
	u32 index;
	for(index=0;index<NUMmax_TAG;index++)
	{
		Calc_PerTAG_Power(index+1,0);
		Calc_PerTAG_Power(index+1,1);
	}
}
void Calc_PerTAG_Power(u32 TAG_NO,u32 Power_type)//���㵥һ�ܼӵĹ��ʣ�TAG_NO=�ܼ����� 1--8��Power_type=�������ͣ�0=�й����� 1=�޹�����
{
	u8 *p_data;
	u8 *p8;
	u32 i;
	u32 j;
	u32 index;
	u64 a64=0;
	u64 b64;
	u32 CTPT;
	ms_Type *ms;
	ms=Get_ms();
	p_data=ms->ms_malloc((size_t)((LENmax_230x_2)+(LENmax_240x_2)+(LENmax_240x_4)));//976�ֽ�
	if((u32)p_data==0)//��̬����ʧ��
	{
		return ;
	}
 	MR((u32)p_data,ADDR_230x_2,LENmax_230x_2);
	MR((u32)p_data+LENmax_230x_2,ADDR_240x_2,LENmax_240x_2);
	MR((u32)p_data+LENmax_230x_2+LENmax_240x_2,ADDR_240x_4,LENmax_240x_4);
	i=MRR((u32)p_data+(TAG_NO-1)*LENper_230x_2,2);
	if(((i&0xff)!=DataType_array)||((i>>8)==0))//��ȡ���ܼ��鲻������Ч����
	{
		goto err;
	}
	for(index=0;index<p_data[(TAG_NO-1)*LENper_230x_2+1];index++)
	{
		p8=Get_Element(p_data+(TAG_NO-1)*LENper_230x_2,index+1,1,LENper_230x_2);//ָ��TSA
		if(p8!=0)
		{
			CTPT=Get_CT_PT((u16 *)(p8+3));
			if(CTPT==0)
			{
				CTPT=0x00010001;
			}
			i=AddrToMeterSetNo((u16 *)(p8+3));
			if(i<NUM_RMmax)//�����õ�������
			{
				p8=Get_Element((u8 *)(ADDR_6000_SDRAM+LENper_6000*i), 2,4,LENper_6000);
				if(p8==0)
				{
					goto err;
				}
				j=MRR((u32)(p8+1),4);
				j=R_Inverse(j,4);
				if((j&0xffffff00)==0xf20a0200)//��6000��˿�����壬Ҳ������6000����
				{
					i=NUM_RMmax;
				}
			}
			p8=Get_Element(p_data+(TAG_NO-1)*LENper_230x_2,index+1,1,LENper_230x_2);//ָ��TSA
			if(i<NUM_RMmax)//�����õ�������
			{
				b64=Get_6000_Power(i,Power_type);
				if(b64==INVALID_DATA)//û�й�������
				{
					goto err;
				}
				b64*=(CTPT&0xffff);
				b64*=((CTPT>>16)&0xffff);
				a64+=b64;
			}
			else//ֻ����������������ˡ�
			{
			#if NUMmax_IMPIN!=0
				for(j=0;j<NUMmax_IMPIN;j++)
				{
					if(Is_TSA_Same((u8 *)(p8+3),(u8 *)((u32)p_data+LENmax_230x_2+j*LENper_240x_2+2))==0)
					{
						break;
					}
				}
				if(j>=NUMmax_IMPIN)//����6000�Ҳ������������㣬���ܼ���������ԣ�ֱ�ӷ��ش���
				{
					goto err;
				}
				p8=Get_Element((u8 *)((u32)p_data+LENmax_230x_2+LENmax_240x_2+j*LENper_240x_4),1,2,LENper_240x_4);
				if(p8==0)
				{
					goto err;
				}
				i=p8[1];//��������		enum
				if((Power_type==0)&&((i==1)||(i==3)))//�������Բ�ƥ���
				{
					b64=0;
				}
				else if((Power_type==1)&&((i==0)||(i==2)))//�������Բ�ƥ���
				{
					b64=0;
				}
				else
				{
					i=MRR(ADDR_240x_5+j*LENper_240x_5+1,4);
					i=R_Inverse(i,4);
					b64=i;
					b64*=(CTPT&0xffff);
					b64*=((CTPT>>16)&0xffff);
				}
				a64+=b64;
			#else
				goto err;
			#endif
			}
		}
	}
	if(Power_type==0)
	{
		MWR(DataType_long64,ADDR_230x_3+(TAG_NO-1)*LENper_230x_3,1);
		MWR(a64,ADDR_230x_3+(TAG_NO-1)*LENper_230x_3+1,8);
		Data_Inverse(ADDR_230x_3+(TAG_NO-1)*LENper_230x_3+1,8);
	}
	else
	{
		MWR(DataType_long64,ADDR_230x_4+(TAG_NO-1)*LENper_230x_4,1);
		MWR(a64,ADDR_230x_4+(TAG_NO-1)*LENper_230x_4+1,8);
		Data_Inverse(ADDR_230x_4+(TAG_NO-1)*LENper_230x_4+1,8);
	}
err:
	ms->ms_free(p_data);
}

u64 Get_6000_Power(u32 pn,u32 Power_type)//��ȡ������Ĺ��� Power_type=�������ͣ�0=�й����� 1=�޹�����
{
	u64 res=INVALID_DATA;
	u8 pRCSD[]={DataType_RCSD,0x01,0x00,0x00,0x00,0x00,0x00};
	u32 task_id;//����oad�������
	u32 data_addr;//���һ����¼oad���ݵĴ�ŵ�ַ
	u32 data_len;
	u64 tsa;
	u8 *p_data;
	u8 *pp;
	u32 oad;
	
	if(Power_type==0)
		oad=0x20040200;
	else
		oad=0x20050200;
	tsa=Get_TSA_BY_PN_noInverse(pn);
	if(tsa==INVALID_DATA)//��ȡ�������ַʧ��
	{
		return INVALID_DATA;
	}
	//��һ�� ����oad ����ȫ���ɵ����ݡ�
	pRCSD[3]=(oad>>24)&0xff;
	pRCSD[4]=(oad>>16)&0xff;
	pRCSD[5]=(oad>>8)&0xff;
	task_id=RCSDtoID(pRCSD);//��¼��ѡ��RCSD���Ӧ�������,����:�����ڲɼ�����������,>=RMTASKmax��ʾ��Ч�����
	if(task_id>=RMTASKmax)//Ҫ��ѯ��OAD�����κ�һ���������棬�϶���û�����ݵġ�
	{
		return INVALID_DATA;
	}
	#if (USER/100)==15//����
		data_addr=Get_LastRecordFileDataHasID(task_id,(u16* )&tsa);//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF�����ݳ��ȣ�0=û�ҵ�
	#else
		data_addr=Get_LastRecordFileData(task_id,(u16* )&tsa);//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF�����ݳ��ȣ�0=û�ҵ�
	#endif
	if(data_addr==0)//�����ݼ�¼����û�и�TSA������
	{
		return INVALID_DATA;
	}
	data_len=Get_RecordFileData_RCSD(pRCSD+1,(u8 * )&tsa,(u8 * )ADDR_128KDATABUFF,(u8 *)ADDR_DATABUFF,1024,0);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
	if(data_len==0)
	{
		return INVALID_DATA;
	}
	//���⣬���ݶ��Ѿ�����ADDR_DATABUFF
	//
	p_data=(u8 *)ADDR_DATABUFF;
	if(p_data[0]==DataType_array)//��Ϊǰ����ѡʱ��ѡȫ���ģ�������Ӧ��������ġ�
	{
		pp=Get_Element(p_data,1,0,LEN_DATABUFF);
		if(pp!=0)
		{
			switch(pp[0])
			{
				case DataType_long_unsigned:
				case DataType_long:
					res=(pp[1]<<8)+pp[2];
					break;
				case DataType_double_long:
				case DataType_double_long_unsigned:
					res=(pp[1]<<24)+(pp[2]<<16)+(pp[3]<<8)+pp[4];
//					res/=1000;
					break;
				default:
					break;
			}
		}
		else
		{
			res=INVALID_DATA;
		}
	}
	else
	{
		res=INVALID_DATA;
	}
	return res;
}

u64 Get_TSA_BY_PN_noInverse(u32 Pn)//����������ţ��õ�TSA���������������͡�������˳��
{
	u32 Addr;
	u64 Meter;
	u8 *p8;
	u32 Len;
	
	Addr=ADDR_6000+LENper_6000*Pn;
	p8=Get_Element((u8 *)Addr,2,1,LENper_6000);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
	if(p8==0)
	{
		return INVALID_DATA;
	}
	Len=p8[1];
	if(Len==0)
	{
		return 0;
	}
	MR(ADDR_DATABUFF,(u32)p8+3,Len);
	if(Len>8)
		Meter=MRR(ADDR_DATABUFF,8);
	else
		Meter=MRR(ADDR_DATABUFF,Len);
	return Meter;
}

