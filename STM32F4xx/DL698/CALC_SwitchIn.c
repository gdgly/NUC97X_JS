

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CALC_SwitchIn.h"
#include "../DL698/EVENT_Record.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"




__align(4) const u16 PIN_SwitchIn_List[]=//�ն˿��������������б�
{
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,4=����һ,500=����������,600=ͨ��ģ��,700=
	PIN_SWIN1,
	PIN_SWIN2,
	PIN_SWIN3,
	PIN_SWIN4,
	//PIN_DOOR_IN,//�Žӵ�
	#endif
	
	#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,4=����һ,500=����������,600=ͨ��ģ��,700=
	PIN_SWIN1,
	PIN_SWIN2,
//	PIN_SWIN3,
//	PIN_SWIN4,
	#if (USER/100)==15//����
		#if NUMmax_SWITCHIN!=3
			#error
		#endif
	PIN_DOOR_IN,//�Žӵ�
	#endif
	#endif
	
	#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,4=����һ,500=����������,600=ͨ��ģ��,700=
	PIN_SWIN1,
	PIN_SWIN2,
	PIN_SWIN3,
	PIN_SWIN4,
	//PIN_DOOR_IN,//�Žӵ�
	#endif
};

u32 Get_SwitchIn_Route(void)//�õ�����������·��
{
	u32 i;
	
 #if (Project/100)!=4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,4=����һ,500=����������,600=ͨ��ģ��,700=
	i=sizeof(PIN_SwitchIn_List)/2;
	if(i>NUMmax_SWITCHIN)//��󿪹�������˿���(��������˿���+�Žӵ�)
	{
		i=NUMmax_SWITCHIN;//��󿪹�������˿���(��������˿���+�Žӵ�)
	}
	return i;
 #else
	#if NUMmax_SWITCHIN!=4
		#error
	#endif
	if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		i=2;
		return i;
	}
	else
	{
		i=4;
		return i;
	}
 #endif
}

void Init_SwitchIn(void)//��ʼ���ն˿���������
{
	u32 i;
	u32 x;
	u32 y;
	u32 set1;
	u32 set2;
	u32 MaxPort;
	u8 *p8;
	
	MaxPort=Get_SwitchIn_Route();//�õ�����������·��
//������
	x=0;
	for(i=0;i<MaxPort;i++)
	{
		x|=(Pin_Read(PIN_SwitchIn_List[i])<<i);//������,�������ʹ����;����0��1
	}
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}

	x=~x;//��"O"��"��"״̬����"l"��"��"״̬
	x&=y;

//����ͷֺ�״̬
	set1=MRR(ADDR_F203_4+4,4);
	set2=set1>>24;//���������Ա�־
	set1&=0xff;//�����������־
	for(i=0;i<MaxPort;i++)
	{
		if((set1&(0x80>>i))==0x0)
		{//δ����=��"O"��"��"״̬
			x&=~(1<<i);
		}
		else
		{//����
			if((set2&(0x80>>i))==0x0)
			{//��"0"�����մ���
				y=x;
				x&=~(1<<i);
				y=~y;
				y&=(1<<i);
				x|=y;
			}
		}
	}
	//��״̬
	p8=(u8*)ADDR_F203_2;
	p8[0]=DataType_array;
	p8[1]=MaxPort;
	p8+=2;
	for(i=0;i<MaxPort;i++)
	{
		p8[0]=DataType_structure;
		p8[1]=2;
		p8[2]=DataType_unsigned;
		p8[3]=x&0x1;
		p8[4]=DataType_unsigned;
		p8[5]=0;
		p8+=6;
		x>>=1;
	}
}

void CALC_SwitchIn(void)//����������
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 f;
	u32 set1;
	u32 set2;
	u32 MaxPort;
	u8* p8s;
	u8* p8d;
	
	MaxPort=Get_SwitchIn_Route();//�õ�����������·��
//������
	x=0;
	for(i=0;i<MaxPort;i++)
	{
		x|=(Pin_Read(PIN_SwitchIn_List[i])<<i);//������,�������ʹ����;����0��1
	}
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}
	x=~x;//��"O"��"��"״̬����"l"��"��"״̬
	x&=y;
//�����˲�
	f=0;//�˲�OK��־
	p8s=(u8*)ADDR_SwitchIn;//����������
	for(i=0;i<MaxPort;i++)
	{
		if((x&(1<<i))!=p8s[2*i])
		{
			p8s[2*i]=x&(1<<i);//����������ԭ״̬
			p8s[(2*i)+1]&=0xf0;//����������:b0-b3�˲�����,b4-b7������Ч���ȶ�����
		}
		else
		{
			if((p8s[(2*i)+1]&0x0f)<3)//Լ1��
			{
				p8s[(2*i)+1]++;//����������:b0-b3�˲�����,b4-b7������Ч���ȶ�����
			}
			else
			{
				f|=(1<<i);//�˲�OK��־
			}
		}
	}
	if(f!=y)
	{
		return;//û�ȶ�
	}
	
//����ͷֺ�״̬
	set1=MRR(ADDR_F203_4+4,4);
	set2=set1>>24;//���������Ա�־
	set1&=0xff;//�����������־
	for(i=0;i<MaxPort;i++)
	{
		if((set1&(0x80>>i))==0x0)
		{//δ����=��"O"��"��"״̬
			x&=~(1<<i);
			p8s[(2*i)+1]=0;//����������:b0-b3�˲�����,b4-b7������Ч���ȶ�����
		}
		else
		{//����
			if((set2&(0x80>>i))==0x0)
			{//��"0"�����մ���
				y=x;
				x&=~(1<<i);
				y=~y;
				y&=(1<<i);
				x|=y;
			}
		}
	}
	//��״̬
	p8d=(u8*)ADDR_F203_2;
	p8d[0]=DataType_array;
	p8d[1]=MaxPort;
	p8d+=2;
	f=0;
	for(i=0;i<MaxPort;i++)
	{
		p8d[0]=DataType_structure;
		p8d[2]=DataType_unsigned;
		p8d[4]=DataType_unsigned;
		p8d[1]=2;
		y=p8d[3];
		set2=(x>>i)&0x1;
		p8d[3]=set2;
		z=p8s[(2*i)+1];//����������:b0-b3�˲�����,b4-b7������Ч���ȶ�����
		if(z<0x20)
		{
			z+=0x10;
			p8s[(2*i)+1]=z;
		}
		z>>=4;
		if(z>=2)
		{//������Ч������2�βɼ����жϱ�λ
			if(y^set2)
			{//��λ
				p8d[5]=1;
				f|=1<<i;
			}
		}
		p8d+=6;
	}
	
	if(f!=0x0)
	{//�б�λ
		Event_OAD(0x31040200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	}
}

void CLR_SwitchIn_CD(void)//�忪���������λCD
{
	u32 i;
	u32 MaxPort;
	u8* p8;
	
	MaxPort=Get_SwitchIn_Route();//�õ�����������·��
	p8=(u8*)ADDR_F203_2+2;
	for(i=0;i<MaxPort;i++)
	{
		p8[5]=0;
		p8+=6;
	}
}


//#define ADDR_F203_4     ADDR_F101_3+LENmax_F101_3
//����4��=structure
//{
//�����������־bit-string(SIZE��8��)
//��
//bit0��bit7��˳���λ��ʾ��1��8·״̬�����룬�á�1�������룬�á�0����δ���롣
//����
//���������Ա�־bit-string(SIZE��8��)
//��
//  bit0��bit7��˳���λ��ʾ��1��8·״̬�����룬�á�1���������㡣�á�0�������մ��㡣
//��
//}

//#define ADDR_F203_2     ADDR_F100_13+LENmax_F100_13
//����2���豸�����б�ֻ������=array ��������Ԫ
//��������Ԫ��=structure
//{
	//״̬ST  unsigned��
	//��λCD  unsigned
//}
//״̬ST����0�����֡�״̬��1�����ϡ�״̬��
//��λCD����0����ǰ��ң�Ŵ��ͺ���״̬�仯��1����ǰ��ң�Ŵ��ͺ�������һ��״̬�仯��





















