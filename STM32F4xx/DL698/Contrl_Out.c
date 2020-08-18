
//�ն˿���-���.�̵�����LED
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"





void BUZZCTRL(void)//��������������/��ֹ,=0��ֹ,!=0����
{
	u32 n;
	u32 hms;
	u32 hms1;
	u32 hms2;
	u32 e;
	u8 *p8;
	
//#define ADDR_F206_4     ADDR_F205_127+LENmax_F205_127//�澯���
////����4��=array �����澯ʱ��
////�����澯ʱ�Ρ�=structure
////{
////  ��ʼʱ��  Time��
////  ����ʱ��  Time
////}
//#define NUMmax_F206_4     12
//#define LENmax_F206_4     (2+(10*NUMmax_F206_4))
	hms=(Comm_Ram->DL698YMDHMS[4]<<16)|(Comm_Ram->DL698YMDHMS[5]<<8)|Comm_Ram->DL698YMDHMS[6];
	MR(ADDR_DATABUFF,ADDR_F206_4,LENmax_F206_4);
	p8=(u8*)ADDR_DATABUFF;
	n=p8[1];
	p8+=2;
	e=0;
	while(n--)
	{
		if(p8[4]==0xff)//FFH��ʾ��Ч
		{
			p8[4]=0;
		}
		if(p8[5]==0xff)//FFH��ʾ��Ч
		{
			p8[5]=0;
		}
		hms1=(p8[3]<<16)|(p8[4]<<8)|p8[5];
		if(p8[8]==0xff)//FFH��ʾ��Ч
		{
			p8[8]=0;
		}
		if(p8[9]==0xff)//FFH��ʾ��Ч
		{
			p8[9]=0;
		}
		hms2=(p8[7]<<16)|(p8[8]<<8)|p8[9];
		if((hms>=hms1)&&(hms<hms2))
		{
			e=1;
			break;
		}
		p8+=10;
	}
	if(e)
	{//����
		Comm_Ram->BUZZCTRL[8]=1;//����
	}
	else
	{//��ֹ
		Comm_Ram->BUZZCTRL[8]=0;//��ֹ
	}
}


#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
void Init_Contrl_Pin(void)//��ʼ���ն�������ƺ�LED����
{
	Pin_Configure(PIN_ATTR_LED,//�ϵ�ʱ���ָʾLED�ʳ�ʼ���0(��)
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
}
void Terminal_Contrl_Out(void)//�ն˿���-���.�̵�����LED
{
	#if ((Project/100)>=2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
	if(Comm_Ram->CheckLEDTimer)//24 �ϵ�ȫ��LED�����ʾ ÿ10MS��1��ʱ��
	{
		return;
	}
	BUZZCTRL();//��������������/��ֹ,=0��ֹ,!=0����
//����LED ָʾ
	//û����
	Pin_OutH(PIN_ATTR_LED);
	
	#endif
}
#endif
#if ((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
void Init_Contrl_Pin(void)//��ʼ���ն�������ƺ�LED����
{
	Pin_Configure(PIN_ATTR_LED,//�ϵ�ʱ���ָʾLED�ʳ�ʼ���0(��)
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
}

//u32 Terminal_Contrl_Round(void)//�ն˵�ǰ�趨�Ŀ����ִ�(���غϵ��);����B0-B7λ��־
//{
//	u32 i;
//	u32 x;
//	u8 *p8;
//	
//	//�����ִ��趨
//	MR(ADDR_DATABUFF,ADDR_230x_14,LENmax_230x_14);
//	//����ִ��趨
//	MR(ADDR_DATABUFF+LENmax_230x_14,ADDR_230x_15,LENmax_230x_15);
//	p8=(u8 *)(ADDR_DATABUFF);
//	x=0;
//	for(i=0;i<(2*NUMmax_TAG);i++)
//	{
//		x|=p8[2];
//		p8+=3;
//	}
//	return x;
//}

void Terminal_Contrl_Out(void)//�ն˿���-���.�̵�����LED
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u8 * p8;
//	u16 * p16;
	
	if(Comm_Ram->CheckLEDTimer)//24 �ϵ�ȫ��LED�����ʾ ÿ10MS��1��ʱ��
	{
		return;
	}
	
	BUZZCTRL();//��������������/��ֹ,=0��ֹ,!=0����
//�ն�����բ��բ��־
	x=0;
	p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
	if(p8[1]==0x0)
	{//������
		p8=(u8 *)(ADDR_Terminal_Remote_State);//�ն˿���-ң����բ
		for(i=0;i<NUMmax_CR;i++)
		{
			if(p8[0]>=102)
			{
				x|=1<<i;
			}
			p8+=Terminal_Ctrl_Pn_Len;
		}
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State);//�ն˿���-����.��ǰ�����¸���
		for(i=0;i<(6*NUMmax_TAG);i++)
		{
			x|=p8[1];
			p8+=Terminal_Ctrl_Pn_Len;
		}
	}
	p8=(u8 *)(ADDR_Terminal_Total_State);//�ն�����բ
	y=p8[0];//y=ԭ��բ��־
	p8[0]=x;//x=��ǰ��բ��־
	i=x^y;
	i&=y;//��ǰ��բ������־
	if(i!=0x0)
	{//���º�բ����
		Terminal_Ram->EnSwitchON_S_Timer=60;//��բ����֪ͨ��ʱ60��
		p8=(u8 *)(ADDR_Terminal_Total_State+1);//λ��־(�ִ�1-8)������բ֪ͨ(����բ״̬��ӦλתΪ��բʱ��λ,֪ͨ��ʱ������0)
		p8[0]|=i;
	}
	i=x^y;
	i&=x;//������բ��־
	if(i!=0x0)
	{//������բ
		Terminal_Ram->EnSwitchON_S_Timer=0;//��բ����֪ͨ��ʱ��
		Terminal_Ram->ReTrip_S_Timer=0;//8 �ն˲���բ�붨ʱ��
		Terminal_Ram->RoundTripFlags&=(~i);//�ն��ִ���բλ��־,B0-B7=0ûѭ��,=1��ѭ��
	}
//�ն�����բѭ��
	p8=(u8*)(ADDR_Terminal_Total_State);//�ն�����բ
	i=p8[0];
	x=i&0x1;
	if(i&2)
	{
		x|=0x8;
	}
	p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
	i=p8[1];
	if(i!=0)
	{
		i=2;
	}
	p8=(u8*)(ADDR_ContrlUARTData);//����ģ�������������
	p8[0]=x;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
	p8[1]=i;//Ҫ���������ģ��Ŀ���1:b0=�澯�̵�������,b1=����ָʾ��



	
//���բ������־
	if(Terminal_Ram->EnSwitchON_S_Timer==0x0)//�ն˺�բ����֪ͨ�붨ʱ��
	{
		p8=(u8 *)(ADDR_Terminal_Total_State+1);//λ��־(�ִ�1-8)������բ֪ͨ(����բ״̬��ӦλתΪ��բʱ��λ,֪ͨ��ʱ������0)
		p8[0]=0;
	}

//����,���LEDָʾ
	MR(ADDR_DATABUFF,ADDR_8103_3,(ADDR_8108_3+LENmax_8108_3)-(ADDR_8103_3));//Ͷ��(�ܼ����)
//����LEDָʾ
	//ʱ�ι���Ͷ��(�ܼ����)
	//���ݹ���Ͷ��(�ܼ����)
	//Ӫҵ��ͣ����Ͷ��(�ܼ����)
	//��ǰ�����¸���Ͷ��(�ܼ����)
	x=0;
	for(y=0;y<4;y++)
	{
		p8=(u8 *)(ADDR_DATABUFF+(y*LENmax_8103_3));
		n=p8[1];
		if(n>NUMmax_TAG)
		{
			n=NUMmax_TAG;
		}
		p8+=2;
		for(i=0;i<n;i++)
		{
			if(p8[6]!=0)
			{
				x=1;
			}
			p8+=7;
		}
	}
	if(x!=0x0)
	{//Ͷ��
		//���غ����--��ʾ�ն�ʱ�οء����ݿػ�ǰ�����¸�������һ�ֿ���Ͷ��
		p8=(u8*)(ADDR_ContrlUARTData);//����ģ�������������
		p8[0]|=0x40;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
	}
//���LEDָʾ
	//�����
	//�µ��
	x=0;
	for(y=0;y<2;y++)
	{
		p8=(u8 *)(ADDR_DATABUFF+((y+4)*LENmax_8103_3));
		n=p8[1];
		if(n>NUMmax_TAG)
		{
			n=NUMmax_TAG;
		}
		p8+=2;
		for(i=0;i<n;i++)
		{
			if(p8[6]!=0)
			{
				x=1;
			}
			p8+=7;
		}
	}
	if(x!=0x0)
	{//Ͷ��
		//��غ����--��ʾ�ն˹���ػ��µ��Ͷ��
		p8=(u8*)(ADDR_ContrlUARTData);//����ģ�������������
		p8[0]|=0x80;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
	}

//��բ�����LED ָʾ
	x=0;//�����ִ��趨,b0-b7
	MR(ADDR_DATABUFF,ADDR_230x_14,LENmax_230x_14);//�����ִ��趨(�ܼ����)
	p8=(u8 *)(ADDR_DATABUFF);
	x=0;
	for(i=0;i<NUMmax_TAG;i++)
	{
		x|=p8[2];
		p8+=3;
	}
	MR(ADDR_DATABUFF,ADDR_230x_15,LENmax_230x_15);//����ִ��趨(�ܼ����)
	p8=(u8 *)(ADDR_DATABUFF);
	y=0;//����ִ��趨,b0-b7
	for(i=0;i<NUMmax_TAG;i++)
	{
		y|=p8[2];
		p8+=3;
	}
	x|=y;//�����ִ��趨,b0-b7
	//λ����
	y=0;
	for(i=0;i<8;i++)
	{
		if(x&(1<<(7-i)))
		{
			y|=1<<i;
		}
	}
	x=y;
	
	p8=(u8 *)(ADDR_Terminal_Total_State);//�ն�����բ
	i=p8[0];
	p8=(u8*)(ADDR_ContrlUARTData);//����ģ�������������
	if((i&0x1)!=0x0)
	{//��
		//�����ִκ����--��ʾ�ն���Ӧ�ִδ�����բ״̬
		p8[0]|=2;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
	}
	else
	{//û��
		if((p8[2]&0x03)!=0x03)
		{
			//�����ִ��̵���--��ʾ�ն���Ӧ�ִε���բ��·�������߱���բ����
			p8[0]|=4;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
		}
		else
		{
			if(x&1)
			{//�ִ�1Ͷ��
			//�ƺ�һ����һ�뽻����˸��ʾ���ƻ�·���ؽ����쳣,�����ʾ���ִ�δͶ�����
				if((Comm_Ram->MYMDHMS[0]&1)==0)
				{
					p8[0]|=2;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
				}
				else
				{
					p8[0]|=4;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
				}
			}
		}
	}
	if((i&0x2)!=0x0)
	{//��
		//�����ִκ����--��ʾ�ն���Ӧ�ִδ�����բ״̬
		p8[0]|=0x10;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
	}
	else
	{//û��
		if((p8[2]&0x0c)!=0x0c)
		{
			//�����ִ��̵���--��ʾ�ն���Ӧ�ִε���բ��·�������߱���բ����
			p8[0]|=0x20;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
		}
		else
		{
			if(x&2)
			{//�ִ�2Ͷ��
			//�ƺ�һ����һ�뽻����˸��ʾ���ƻ�·���ؽ����쳣
				if((Comm_Ram->MYMDHMS[0]&1)==0)
				{
					p8[0]|=0x10;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
				}
				else
				{
					p8[0]|=0x20;//Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
				}
			}
		}
	}

//�ն˱���
	x=0;//������־;11=�߷Ѹ澯��,12=������բ��,13=���ظ澯��,14=��ظ澯��,15=ң����բ
	//�߷Ѹ澯
	p8=(u8 *)(ADDR_8002_2);//�߷Ѹ澯״̬��ֻ�� ��=enum{δ�澯(0)���澯(1)}
	p8[0]=DataType_enum;
	p8[1]=0;
	MR(ADDR_DATABUFF,ADDR_8002_127,LENmax_8002_127);
	p8=(u8 *)(ADDR_DATABUFF);
	if(p8[0]==DataType_structure)
	{
		//�߷Ѹ澯��������ֹ��־λ	BS24	3
		i=(p8[4]<<16)|(p8[5]<<8)|p8[6];
		y=Comm_Ram->DL698YMDHMS[4];//ʱ
		if(y>23)
		{
			y=23;
		}
		y=23-y;
		y=1<<y;
		i&=y;
		if(i!=0x0)
		{//�澯
			x=11;//11=�޷Ѹ澯��
			p8=(u8 *)(ADDR_8002_2);//�߷Ѹ澯״̬��ֻ�� ��=enum{δ�澯(0)���澯(1)}
			p8[1]=1;
		}
		else
		{
			BUZZ_Cancel(11);//11=�߷Ѹ澯��
		}
	}
	else
	{
		BUZZ_Cancel(11);//11=�߷Ѹ澯��
	}
	//������բ֪ͨ
	if(Terminal_Ram->EnSwitchON_S_Timer!=0x0)
	{
		x=12;//12=������բ��
	}
	else
	{
		BUZZ_Cancel(12);//12=������բ��
	}
	//���ظ澯
	p8=(u8 *)(ADDR_Terminal_kWCtrl1_State);//�ն˿���-����
	for(i=0;i<(4*NUMmax_TAG);i++)
	{
		if(p8[0]==100)
		{
			x=13;//13=���ظ澯��
		}
		p8+=Terminal_Ctrl_Pn_Len;
	}
	if(x!=13)
	{
		BUZZ_Cancel(13);//13=���ظ澯��
	}
	//��ظ澯
	p8=(u8 *)(ADDR_Terminal_kWhCtrl1_State);//�ն˿���-���.�µ��
	for(i=0;i<(2*NUMmax_TAG);i++)
	{
		if(p8[0]==100)
		{
			x=14;//14=��ظ澯��
		}
		p8+=Terminal_Ctrl_Pn_Len;
	}
	if(x!=14)
	{
		BUZZ_Cancel(14);//14=��ظ澯��
	}
	//ң����բ
	p8=(u8 *)(ADDR_Terminal_Remote_State);//�ն˿���-ң����բ
	for(i=0;i<NUMmax_CR;i++)
	{
		if(p8[0]==100)
		{
			x=15;//15=ң����բ�澯��
		}
		p8+=Terminal_Ctrl_Pn_Len;
	}
	if(x!=15)
	{
		BUZZ_Cancel(15);//15=����բ�澯��
	}
	//
	if(x!=0x0)
	{//�и澯
		if(x==11)
		{//�߷Ѹ澯
			BUZZ_Generate(x);//��������,��ڷ�����
		}
		else
		{
			if(Comm_Ram->BUZZCTRL[8]==1)//����
			{//�澯
				BUZZ_Generate(x);//��������,��ڷ�����
				p8=(u8*)(ADDR_ContrlUARTData);//����ģ�������������
				p8[1]|=0x1;//Ҫ���������ģ��Ŀ���1:b0=�澯�̵�������,b1=����ָʾ��
			}
		}
	}

//����LED ָʾ
	if(x==12)
	{
		x=0;//12=������բ��ָʾ����
	}
	if(x!=0x0)
	{//����
		p8=(u8 *)(ADDR_MYMDHMS);
		if(p8[0]&0x1)
		{
			Pin_OutL(PIN_ATTR_LED);
		}
		else
		{
			Pin_OutH(PIN_ATTR_LED);
		}
	}
	else
	{//û����
		Pin_OutH(PIN_ATTR_LED);
	}
}

#endif//#if ((Project/100)!=3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��



















