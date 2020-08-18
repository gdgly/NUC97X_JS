
//�ն˿���������
#include "Project.h"
#include "Terminal_SwitchIn.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"




#if (((Project/100)!=2)&&((Project/100)!=3)&&((Project/100)!=4)&&((Project/100)!=5))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
void Init_SwitchIn(void)//��ʼ���ն˿���������
{
}
void Terminal_SwitchIn(void)//�ն˿���������
{
}
#else
const u16 PIN_SwitchIn_List[]=//�ն˿��������������б�
{
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	PIN_SWIN1,
	PIN_SWIN2,
	PIN_SWIN3,
	PIN_SWIN4,
	#endif
	#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	PIN_SWIN1,
	PIN_SWIN2,
	#endif	
	#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	PIN_SWIN1,
	PIN_SWIN2,
	PIN_SWIN3,
	PIN_SWIN4,
	#endif
//	PIN_P208_SIN5,
//	PIN_P208_SIN6,
//	PIN_P208_DOOR,//�Žӵ�ͳһ������D7λ
};

u32 Get_SwitchIn_Route(void)//�õ�����������·��
{
	u32 i;
	i=sizeof(PIN_SwitchIn_List)/2;
	if(i>MaxSwitchInPort)//��󿪹�������˿���(��������˿���+�Žӵ�)
	{
		i=MaxSwitchInPort;//��󿪹�������˿���(��������˿���+�Žӵ�)
	}
	return i;
}

void Init_SwitchIn(void)//��ʼ���ն˿���������
{
	u32 i;
	u32 x;
	u32 y;
	u32 MaxPort;

	MaxPort=Get_SwitchIn_Route();//�õ�����������·��
//������
	x=0;
	for(i=0;i<MaxPort;i++)
	{
		x|=(Pin_Read(PIN_SwitchIn_List[i])<<i);//������,�������ʹ����;����0��1
	}
	x|=(Pin_Read(PIN_DOOR_IN)<<7);//�Žӵ�ͳһ������D7λ
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}
	y|=0x80;//�Žӵ�ͳһ��D7λ

	x=~x;//��"O"��"��"״̬����"l"��"��"״̬
	x&=y;

//����ͷֺ�״̬
	for(i=0;i<8;i++)
	{
		if((MRR(ADDR_AFN04F12,1)&(1<<i))==0x0)
		{//δ����=��"O"��"��"״̬
			x&=~(1<<i);
		}
		else
		{//����
			if((MRR(ADDR_AFN04F12+1,1)&(1<<i))==0x0)
			{//��"0"�����մ���
				y=x;
				x&=~(1<<i);
				y=~y;
				y&=(1<<i);
				x|=y;
			}
		}
	}
	MWR(x,ADDR_AFN0CF9,2);//д״̬
}

void Terminal_SwitchIn(void)//�ն˿���������
{
	u32 i;
	u32 x;
	u32 y;
	u32 f;
	u32 MaxPort;
	u8 * p8;
	
	MaxPort=Get_SwitchIn_Route();//�õ�����������·��
//������
	x=0;
	for(i=0;i<MaxPort;i++)
	{
		x|=(Pin_Read(PIN_SwitchIn_List[i])<<i);//������,�������ʹ����;����0��1
	}
	x|=(Pin_Read(PIN_DOOR_IN)<<7);//�Žӵ�ͳһ������D7λ
	
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}
	y|=0x80;//�Žӵ�ͳһ��D7λ
	
	x=~x;//��"O"��"��"״̬����"l"��"��"״̬
	x&=y;
//�����˲�
	f=0;//�˲�OK��־
	p8=(u8*)ADDR_SwitchIn;//����������
	for(i=0;i<8;i++)
	{
		if((x&(1<<i))!=p8[2*i])
		{
			p8[2*i]=x&(1<<i);//����������ԭ״̬
			p8[(2*i)+1]=0;//�����������˲�����
		}
		else
		{
			if(p8[(2*i)+1]<3)//Լ1��
			{
				p8[(2*i)+1]++;//�����������˲�����
			}
			else
			{
				f|=(1<<i);//�˲�OK��־
			}
		}
	}
//����ͷֺ�״̬
	for(i=0;i<8;i++)
	{
		if((MRR(ADDR_AFN04F12,1)&(1<<i))==0x0)
		{//δ����=��"O"��"��"״̬
			x&=~(1<<i);
		}
		else
		{//����
			if((MRR(ADDR_AFN04F12+1,1)&(1<<i))==0x0)
			{//��"0"�����մ���
				y=x;
				x&=~(1<<i);
				y=~y;
				y&=(1<<i);
				x|=y;
			}
		}
	}
//������Чλ
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}
	y|=0x80;//�Žӵ�ͳһ��D7λ
	x&=y;
	f&=y;
//��λ��־
	MR(ADDR_DATABUFF,ADDR_AFN0CF9,2);//��ԭ״̬
	p8=(u8 *)(ADDR_DATABUFF);
	y=x^p8[0];//��λ��־
	y&=f;
	p8[1]|=y;//��λ��־
	p8[0]&=(~f);//ȡû�ȶ�λ��ԭλ
	x&=f;//ȡ��ǰ�ȶ���λ
	p8[0]|=x;//��ǰ״̬
	MW(ADDR_DATABUFF,ADDR_AFN0CF9,2);//д״̬
	if(y!=0x0)
	{//�б�λ
		ERC4_Event(y,x);//״̬����λ��¼;���sc=״̬��λ,ss=��λ��״̬
	}
}
#endif

//#define AFN0CF9_ADDR     TERMINAL_CLASS1DATA_FM_START_ADDR//�ն�״̬������λ��־(��2�ֽڱ�λ��־��UART_READDATA�����ö���־,���ն�״̬��־�����ӳ�������0)
////�ֽ�1��D0��D7��λ��Ӧl��8·״̬����״̬ST����"O"��"��"״̬����"l"��"��"״̬��
////�ֽ�2��D0��D7��λ��Ӧ1��8·״̬���ı�λCD����"O"����ǰ��ң�Ŵ��ͺ���״̬�仯����"1"����ǰ��ң�Ŵ��ͺ�������һ��״̬�仯
//F12���ն�״̬���������
//	��31���ն�״̬������������ݵ�Ԫ��ʽ
//��������	���ݸ�ʽ	�ֽ���
//״̬�������־λ����Ӧ1��8·״̬����	BS8	1
//״̬�����Ա�־λ����Ӧ1��8·״̬����	BS8	1
//
//--״̬�������־λ��D0��D7��˳���λ��ʾ��1��8·״̬�����룬��"1"�����룬��"0"��δ���롣
//--״̬�����Ա�־λ��D0��D7��˳���λ��ʾ��1��8·״̬�����룬��"1"�������㡣��"0"�����մ���


