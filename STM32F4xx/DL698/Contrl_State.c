
//�ܼ����������״̬
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"


#if ((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//��ǰ����״̬��=structure
//{
//	��ǰ���ض�ֵ             long64����λ��W ���㣺-1����     
//	��ǰ�����¸��ظ���ϵ��   integer����λ��%����
//	������բ���״̬         OutputState��
//	�µ����բ���״̬       OutputState��
//	�������բ���״̬       OutputState��
//	����Խ�޸澯״̬         PCAlarmState��
//	���Խ�޸澯״̬         ECAlarmState
//}
//OutputState��=bit-string(SIZE(8))
//bit0��bit7�ֱ��ʾ�ն�1��8�ִ���բ���״̬���á�1����������բ״̬���á�0����δ������բ״̬��
//PCAlarmState��=bit-string(SIZE(8))
//��˳���λ��ʾ���á�1��������ĳ�ֹ���Խ�޸澯״̬���á�0����δ������Ӧ״̬��
//bit0��ʱ�ο�
//bit1�����ݿ�
//bit2��Ӫҵ��ͣ��
//bit3����ǰ�����¸���
//bit4��bit7�����á�
//ECAlarmState��=bit-string(SIZE(8))
//�á�1��������ĳ�ֵ��Խ�޸澯״̬���á�0����δ������Ӧ״̬��
//bit0���µ��
//bit1�������
//bit2��bit7�����á�
void Contrl_State(void)//�ܼ��鵱ǰ����״̬
{
	u32 i;
	u32 Pn;
	u32 Priority;//���ȼ�
	u8* p8s;
	u8* p8d;
	u8* p8ctrl;
	u8* p8attr;
	
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		p8d=(u8*)ADDR_230x_17+(Pn*LENper_230x_17);
		p8d[0]=DataType_structure;
		p8d[1]=7;
		p8d+=2;
//��ǰ���ض�ֵlong64����λ��W ���㣺-1��
		p8d[0]=DataType_long64;
		p8d[1]=0;
		p8d[2]=0;
		p8d[3]=0;
		p8d[4]=0;
		p8d[5]=0;
		p8d[6]=0;
		p8d[7]=0;
		p8d[8]=0;
		p8d+=9;
//��ǰ�����¸��ظ���ϵ��integer����λ��%��
		p8d[0]=DataType_integer;
		p8d[1]=0;
		p8d+=2;
//������բ���״̬bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;
//�µ����բ���״̬bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;
//�������բ���״̬bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;
//����Խ�޸澯״̬bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;
//���Խ�޸澯״̬bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;

//�ն˿���-����;���Ƶ����ȼ��ɸߵ����ǵ�ǰ�����¸��ء�Ӫҵ��ͣ���ء����ݹ��ء�ʱ�ι���
		p8d=(u8*)ADDR_230x_17+(Pn*LENper_230x_17);
		Priority=0;//���ȼ�
	//bit3����ǰ�����¸���
		p8ctrl=(u8*)ADDR_8106_4;//��ǰ�����¸���-�ܼ���������״̬
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
		p8attr=(u8*)ADDR_8106_5;//��ǰ�����¸���-�ܼ���澯���״̬
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//δ�澯��0�����澯��1��
		p8s=(u8*)ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len);
		if(p8s[0])
		{
			Priority=1;//���ȼ�
			if(p8s[0]>=2)
			{
			//��ǰ���ض�ֵlong64����λ��W ���㣺-1��
				MR(ADDR_230x_17+(Pn*LENper_230x_17)+3,ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
			//��ǰ�����¸��ظ���ϵ��integer����λ��%��
				p8d[2+9+1]=p8s[3];
			}
			//������բ���״̬bit-string(SIZE(8))
			i=p8s[1];
			i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
			p8d[2+9+2+2]=i;
			p8ctrl[7]=i;//�ܼ���������״̬,bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
			//�µ����բ���״̬bit-string(SIZE(8))
			//�������բ���״̬bit-string(SIZE(8))
			//����Խ�޸澯״̬bit-string(SIZE(8))
			if(p8s[0]==100)
			{
				p8d[2+9+2+3+3+3+2]=1<<(7-3);
				p8attr[6]=1;//�ܼ���澯���״̬,δ�澯��0�����澯��1��
			}
			//���Խ�޸澯״̬bit-string(SIZE(8))
		}
	//bit2��Ӫҵ��ͣ��
		p8ctrl=(u8*)ADDR_8105_4;//Ӫҵ��ͣ��-�ܼ���������״̬
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
		p8attr=(u8*)ADDR_8105_5;//Ӫҵ��ͣ��-�ܼ���澯���״̬
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//δ�澯��0�����澯��1��
		if(Priority==0)//���ȼ�
		{
			p8s=(u8*)ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len);
			if(p8s[0]>=2)
			{
				Priority=1;//���ȼ�
			//��ǰ���ض�ֵlong64����λ��W ���㣺-1��
				MR(ADDR_230x_17+(Pn*LENper_230x_17)+3,ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
			//��ǰ�����¸��ظ���ϵ��integer����λ��%��
			//������բ���״̬bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
				p8d[2+9+2+2]=i;
				p8ctrl[7]=i;//�ܼ���������״̬,bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
			//�µ����բ���״̬bit-string(SIZE(8))
			//�������բ���״̬bit-string(SIZE(8))
			//����Խ�޸澯״̬bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+2]=1<<(7-2);
					p8attr[6]=1;//�ܼ���澯���״̬,δ�澯��0�����澯��1��
				}
			//���Խ�޸澯״̬bit-string(SIZE(8))
			}
		}
		
	//bit1�����ݿ�
		p8ctrl=(u8*)ADDR_8104_4;//���ݿ�-�ܼ���������״̬
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
		p8attr=(u8*)ADDR_8104_5;//���ݿ�-�ܼ���澯���״̬
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//δ�澯��0�����澯��1��
		if(Priority==0)//���ȼ�
		{
			p8s=(u8*)ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len);
			if(p8s[0]>=2)
			{
				Priority=1;//���ȼ�
			//��ǰ���ض�ֵlong64����λ��W ���㣺-1��
				MR(ADDR_230x_17+(Pn*LENper_230x_17)+3,ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
			//��ǰ�����¸��ظ���ϵ��integer����λ��%��
			//������բ���״̬bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
				p8d[2+9+2+2]=i;
				p8ctrl[7]=i;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
			//�µ����բ���״̬bit-string(SIZE(8))
			//�������բ���״̬bit-string(SIZE(8))
			//����Խ�޸澯״̬bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+2]=1<<(7-1);
					p8attr[6]=1;//δ�澯��0�����澯��1��
				}
			//���Խ�޸澯״̬bit-string(SIZE(8))
			}
		}
	//bit0:ʱ�ι���
		p8ctrl=(u8*)ADDR_8103_4;//ʱ�ι���-�ܼ���������״̬
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
		p8attr=(u8*)ADDR_8103_5;//ʱ�ι���-�ܼ���澯���״̬
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//δ�澯��0�����澯��1��
		if(Priority==0)//���ȼ�
		{
			p8s=(u8*)ADDR_Terminal_kWCtrl4_State+(Pn*Terminal_Ctrl_Pn_Len);
			if(p8s[0]>=2)
			{
				Priority=1;//���ȼ�
			//��ǰ���ض�ֵlong64����λ��W ���㣺-1��
				MR(ADDR_230x_17+(Pn*LENper_230x_17)+3,ADDR_Terminal_kWCtrl4_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
			//��ǰ�����¸��ظ���ϵ��integer����λ��%��
			//������բ���״̬bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
				p8d[2+9+2+2]=i;
				p8ctrl[7]=i;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
			//�µ����բ���״̬bit-string(SIZE(8))
			//�������բ���״̬bit-string(SIZE(8))
			//����Խ�޸澯״̬bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+2]=1<<(7-0);
					p8attr[6]=1;//δ�澯��0�����澯��1��
				}
			//���Խ�޸澯״̬bit-string(SIZE(8))
			}
		}
		
	//bit0���µ��
		p8ctrl=(u8*)ADDR_8108_4;//�µ��-�ܼ���������״̬
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
		p8attr=(u8*)ADDR_8108_5;//�µ��-�ܼ���澯���״̬
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//δ�澯��0�����澯��1��
		p8s=(u8*)ADDR_Terminal_kWhCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len);
		//��ǰ���ض�ֵlong64����λ��W ���㣺-1��
		//��ǰ�����¸��ظ���ϵ��integer����λ��%��
		//������բ���״̬bit-string(SIZE(8))
		//�µ����բ���״̬bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
				p8d[2+9+2+3+2]=i;
				p8ctrl[7]=i;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
		//�������բ���״̬bit-string(SIZE(8))
		//����Խ�޸澯״̬bit-string(SIZE(8))
		//���Խ�޸澯״̬bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+3+2]=1<<(7-0);
					p8attr[6]=1;//δ�澯��0�����澯��1��
				}
	//bit1�������
		p8ctrl=(u8*)ADDR_8107_4;//�����-�ܼ���������״̬
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
		p8attr=(u8*)ADDR_8107_5;//�����-�ܼ���澯���״̬
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//δ�澯��0�����澯��1��
		p8s=(u8*)ADDR_Terminal_kWhCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len);
		//��ǰ���ض�ֵlong64����λ��W ���㣺-1��
		//��ǰ�����¸��ظ���ϵ��integer����λ��%��
		//������բ���״̬bit-string(SIZE(8))
		//�µ����բ���״̬bit-string(SIZE(8))
		//�������բ���״̬bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
				p8d[2+9+2+3+3+2]=i;
				p8ctrl[7]=i;///bit0~bit7��Ӧ1~8�����ص����״̬��0��δ�����1�������
		//����Խ�޸澯״̬bit-string(SIZE(8))
		//���Խ�޸澯״̬bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+3+2]|=1<<(7-1);
					p8attr[6]=1;//δ�澯��0�����澯��1��
				}
	}
}

//�ܼ����������״̬��=structure
//{
//ʱ�οض�ֵ������	   unsigned��
//����ʱ����Ч��־λ  bit-string(SIZE(8))��
//����״̬            PCState��
//���״̬            ECState��
//�����ִ�״̬        TrunState��
//����ִ�״̬        TrunState
//}
//ʱ�οض�ֵ�����ţ���ʾ��Ͷ��Ĺ��ض�ֵ�����š�
//����ʱ����Ч��־λ��bit0��bit7��˳���λ��ʾ1��8ʱ�ο�Ͷ�����Чʱ�Σ��á�1������Ч���á�0������Ч
//PCState��=bit-string(SIZE(8))
//bit0��bit7��˳���λ��ʾ���á�1����Ͷ�룬�á�0���������
//bit0��ʱ�ο�
//bit1�����ݿ�
//bit2��Ӫҵ��ͣ��
//bit3����ǰ�����¸���
//bit4��bit7�����á�
//ECState��=bit-string(SIZE(8))
//bit0��bit7��˳���λ��ʾ���á�1����Ͷ�룬�á�0���������
//bit0���µ��
//bit1�������
//bit2��bit7������
//TrunState��=bit-string(SIZE(8))
//bit0��bit7��˳���λ��ʾ1��8�ִο��ص��ܿ�״̬���á�1�����ܿأ��á�0�������ܿء�
void Contrl_SetState(u32 Pn)//�ܼ����������״̬;���:�ܼ����0-7
{
	u32 i;
	u32 n;
	u32 OI;
	u8* p8s;
	u8* p8d;
	
	p8d=(u8*)ADDR_230x_16+(Pn*LENper_230x_16);
	p8d[0]=DataType_structure;
	p8d[1]=6;
	p8d+=2;
//ʱ�οض�ֵ������	   unsigned��
//����ʱ����Ч��־λ  bit-string(SIZE(8))��
	p8d[0]=DataType_unsigned;
	p8d[1]=0;
	p8d[2]=DataType_bit_string;
	p8d[3]=8;
	p8d[4]=0;
	MR(ADDR_DATABUFF,ADDR_8103_127,LENmax_8103_127);
	//����ӦOI�ķ����л�
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
	}
	if(i<n)
	{//�ҵ�
		p8d[1]=p8s[11];//ʱ�ι��ض�ֵ������
		p8d[4]=p8s[9];//����ʱ����Ч��־λ
	}
	p8d+=5;
//����״̬            PCState��
//PCState��=bit-string(SIZE(8))
//bit0��bit7��˳���λ��ʾ���á�1����Ͷ�룬�á�0���������
//bit0��ʱ�ο�
//bit1�����ݿ�
//bit2��Ӫҵ��ͣ��
//bit3����ǰ�����¸���
//bit4��bit7�����á�
	p8d[0]=DataType_bit_string;
	p8d[1]=8;
	p8d[2]=0;
	//ʱ�ο�
	MR(ADDR_DATABUFF,ADDR_8103_3,LENmax_8103_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
	}
	if(i<n)
	{//�ҵ�
		if(p8s[6]==1)
		{
			p8d[2]|=1<<7;
		}
	}
	//���ݿ�
	MR(ADDR_DATABUFF,ADDR_8104_3,LENmax_8104_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
	}
	if(i<n)
	{//�ҵ�
		if(p8s[6]==1)
		{
			p8d[2]|=1<<6;
		}
	}
	//Ӫҵ��ͣ��
	MR(ADDR_DATABUFF,ADDR_8105_3,LENmax_8105_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
	}
	if(i<n)
	{//�ҵ�
		if(p8s[6]==1)
		{
			p8d[2]|=1<<5;
		}
	}
	//��ǰ�����¸���
	MR(ADDR_DATABUFF,ADDR_8106_3,LENmax_8106_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
	}
	if(i<n)
	{//�ҵ�
		if(p8s[6]==1)
		{
			p8d[2]|=1<<4;
		}
	}
	p8d+=3;
//���״̬            ECState��
	//ECState��=bit-string(SIZE(8))
	//bit0��bit7��˳���λ��ʾ���á�1����Ͷ�룬�á�0���������
	//bit0���µ��
	//bit1�������
	//bit2��bit7������
	p8d[0]=DataType_bit_string;
	p8d[1]=8;
	p8d[2]=0;
	//�µ��
	MR(ADDR_DATABUFF,ADDR_8108_3,LENmax_8108_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
	}
	if(i<n)
	{//�ҵ�
		if(p8s[6]==1)
		{
			p8d[2]|=1<<7;
		}
	}
	//�����
	MR(ADDR_DATABUFF,ADDR_8107_3,LENmax_8107_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//�ܼ�������=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���
	}
	if(i<n)
	{//�ҵ�
		if(p8s[6]==1)
		{
			p8d[2]|=1<<6;
		}
	}
	p8d+=3;
//�����ִ�״̬        TrunState��
	//TrunState��=bit-string(SIZE(8))
	//bit0��bit7��˳���λ��ʾ1��8�ִο��ص��ܿ�״̬���á�1�����ܿأ��á�0�������ܿء�
	p8d[0]=DataType_bit_string;
	p8d[1]=8;
	i=MRR(ADDR_230x_14+(Pn*LENper_230x_14)+2,1);
	p8d[2]=i;
	p8d+=3;
//����ִ�״̬        TrunState
	p8d[0]=DataType_bit_string;
	p8d[1]=8;
	i=MRR(ADDR_230x_15+(Pn*LENper_230x_14)+2,1);
	p8d[2]=i;
	
}

#endif



















