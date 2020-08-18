
//ERC�¼�
#include "Project.h"
#include "Terminal_AFN0E_Event.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "../Terminal/DataConversion_SH698.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_Uart_AFN04_Paramter.h"
#include "../terminal/Terminal_USER5xx.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"



extern const u32 DL645_2007_RMLib[];

void Init_Class3Data_UpPower(void)//��3����(�¼����㣩��ʼ��(�ϵ�)
{
	u32 i;
	u8 * p8;

#if ERC22_TNum!=0
	u32 Pn;
	u16 * p16;
//ERC22�Խ���¼�
	//���ܼӵ���,ԭ��鳬�ޱ�־����
	MC(0,ADDR_ERC22_TotalEnergy,(ADDR_ERC22_SOUR)-(ADDR_ERC22_TotalEnergy));
	//��ʼ�����䶨ʱ
	p16=(u16 *)(ADDR_ERC22_Timer);
	for(Pn=0;Pn<ERC22_TNum;Pn++)
	{
    i=MRR(ADDR_AFN04F15+(Pn*9)+3,1);//ȡ�����ĵ�������ʱ�����估�Աȷ�����־
		i&=3;
		switch(i)
		{
			case 0:
				i=60*60;
				break;
			case 1:
				i=30*60;
				break;
			default:
				i=15*60;
				break;
		}
		p16[Pn]=i;
	}
#endif
//ERC9������·�쳣�¼�
	MC(0xee,ADDR_ERC9_SOUR,LEN_ERC9_SOUR_Pn*ERC9MaxPn);
	p8=(u8 *)(ADDR_ERC9_SOUR);
	for(i=0;i<ERC9MaxPn;i++)
	{
		p8[0]=0;
		p8+=LEN_ERC9_SOUR_Pn;
	}
//ERC10��ѹ��·�쳣�¼�
	MC(0xee,ADDR_ERC10_SOUR,LEN_ERC10_SOUR_Pn*ERC10MaxPn);
	p8=(u8 *)(ADDR_ERC10_SOUR);
	for(i=0;i<ERC10MaxPn;i++)
	{
		p8[0]=0;
		p8[1]=0;
		p8+=LEN_ERC10_SOUR_Pn;
	}
//ERC29���ܱ����,�����¼�
	//�嶨ʱ��ʼʱ����ֵ,�ָ�������־����
	p8=(u8 *)(ADDR_ERC29_SOUR);
	for(i=0;i<ERC29MaxPn;i++)
	{
		p8[0]=0xee;
		p8[19]=0xee;
		p8+=LEN_ERC29_SOUR;
	}
//ERC30���ܱ�ͣ���¼�
	//�嶨ʱ��ʼʱ����ֵ,�ָ�������־����
	p8=(u8 *)(ADDR_ERC30_SOUR);
	for(i=0;i<ERC30MaxPn;i++)
	{
		p8[0]=0xee;
		p8+=LEN_ERC30_SOUR;
	}
//ERC37ERC38���ܱ���Ƕ˸��¼���¼ԭʼ����
	MC(0xee,ADDR_ERC37_38_SOUR,LEN_ERC37_38_SOUR*ERC37MaxPn);
//ERC40�ų��쳣�¼���¼
	//�巢���ܴ���Ϊ��Ч
	p8=(u8 *)(ADDR_ERC40_SOUR+1);
	for(i=0;i<ERC40MaxPn;i++)
	{
		p8[2]=0xee;
		p8[3]=0xee;
		p8[4]=0xee;
		p8+=LEN_ERC40_SOUR;
	}
#if (USER/100)==5//�û���ʶ:�Ϻ�
	//����ܱ��ɿ�������Ϊ0xee
	MC(0xee,ADDR_ERC68_SOUR,3*MaxRS485AddCarrierPn);
	//��ʾ��ʧѹȫʧѹ����(�ϵ�ʱ��Ϊ0xee)
	MC(0xee,ADDR_AFN0CF26_Ext,LEN_AFN0CF26_Ext*MaxRS485AddCarrierPn);
	//ERC41ֱ�Ӷ����ܱ��ʱ�¼���¼
	MC(0xee,ADDR_ERC41_SOUR,LEN_ERC41_SOUR*MaxRS485AddCarrierPn);
#endif
}
void Init_Class3Data_Command(void)//��3����(�¼�)��ʼ��(����,ȫ�����)
{
	//�¼��������¼���־״̬
	MC(0x0,ADDR_AFN0CF7_FM,8);
  MC(0x0,ADDR_AFN0CF8,8);
	//��Ҫ�¼��ȴ����ʱ�־
	MC(0x0,ADDR_ADCFlags,3);
	//�¼���¼
	MC(0x0,ADDR_AFN0EF1,LEN_TERMINAL_EVENT*256);
	MC(0x0,ADDR_AFN0EF2,LEN_TERMINAL_EVENT*256);
	MC(0x0,ADDR_AFN0EF3,MaxERC*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)));
	//�����־
	MC(0x0,ADDR_ERC21_Flag,1);//ERC21 485��������¼���¼��־
	MC(0x0,ADDR_RM_CountFlags+32,MaxRS485AddCarrierPn/4);//ÿ����2λԭ���ܱ����־(0=�ɹ�,1=ʧ��->�ɹ�,2=�ɹ�->ʧ��,3=ʧ��)
	//���ܱ��¼�����
	MC(0x0,ADDR_MeterEventCount_FM,8);
	//���ܱ������ϱ�״̬��
	MC(0x0,ADDR_AutoReportWord,LEN_AutoReportWord*MaxRS485AddCarrierPn);
	//���ܱ���¼�ԭ��������
	MC(0xff,ADDR_MeterEventCount,LEN_MeterEventCount*MaxRS485AddCarrierPn);

	Init_Class3Data_UpPower();//��3����(�¼�����)��ʼ��(�ϵ�)
}

void Init_Class3Data(u32 Pn)//��3����(�¼�)��ʼ��(ָ���Ĳ���������,Pn=1~n)
{
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	Pn--;
	//���ܱ������ϱ�״̬��
	MC(0,ADDR_AutoReportWord+(LEN_AutoReportWord*Pn),LEN_AutoReportWord);
	//���ܱ���¼�ԭ��������
	MC(0xff,ADDR_MeterEventCount+(LEN_MeterEventCount*Pn),LEN_MeterEventCount);
}

u32 ERC_Event_Set(u32 EventNo)//����¼���¼����,����0=����¼,1=��¼
{
	u64 d64;
	u64 d64b;
	
	EventNo--;
	if(EventNo>63)
	{//ERC�Ƿ�
		return 0;
	}
	d64=1;
	d64<<=EventNo;
	d64b=MRR(ADDR_AFN04F9,8);
#if (USER/100)==2//�û���ʶ:ɽ����;ERC14�ն�ͣ/�ϵ��¼��ն�ͣ��ʱӦ��������վ�ϱ�ͣ���¼�(�����ն˵�"�ն�ͣ/�ϵ��¼�"�Ƿ�����Ϊ��Ҫ�¼�)
	d64b|=1<<(14-1);
#endif
	if((d64&d64b)==0x0)
	{//�����¼
		return 0;
	}
	return 1;
}

u32 ERC_Event_Save_Re(u32 BuffAddr)//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
{
	u32 i;
	u32 x;
	u64 d64;
	u64 d64b;
	u8 * p8s;

	p8s=(u8 *)(BuffAddr);
	i=p8s[0];
	i-=1;
	if(i>63)
	{//ERC�Ƿ�
	#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
		if(i==(68-1))
		{
			d64=0;
			goto save;//ERC68���¼Ϊ��Ҫ�¼�
		}
	#endif
		return 0;
	}
	d64=1;
	d64<<=i;
	d64b=MRR(ADDR_AFN04F9,8);
#if (USER/100)==2//�û���ʶ:ɽ����;ERC14�ն�ͣ/�ϵ��¼��ն�ͣ��ʱӦ��������վ�ϱ�ͣ���¼�(�����ն˵�"�ն�ͣ/�ϵ��¼�"�Ƿ�����Ϊ��Ҫ�¼�)
	d64b|=1<<(14-1);
#endif
	if((d64&d64b)==0x0)
	{//�����¼
		return 0;
	}
	d64b=MRR(ADDR_AFN04F9+8,8);
#if (USER/100)==2//�û���ʶ:ɽ����;ERC14�ն�ͣ/�ϵ��¼��ն�ͣ��ʱӦ��������վ�ϱ�ͣ���¼�(�����ն˵�"�ն�ͣ/�ϵ��¼�"�Ƿ�����Ϊ��Ҫ�¼�)
	d64b|=1<<(14-1);
#endif
	if((d64&d64b)!=0x0)
	{//��Ҫ�¼�
	#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
	save://ERC68���¼Ϊ��Ҫ�¼�
	#endif
		i=MRR(ADDR_AFN0CF7_FM,1);//��Ҫ�¼�����
		if(p8s[0]!=14)
		{//����ͣ�ϵ��¼�
			MW(BuffAddr,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),p8s[1]+2);
		}
		else
		{//��ͣ�ϵ��¼�,����1���ֽ��ϵ��¼��־
		#if (USER/100)==7//�û���ʶ:�㽭ϵ
			MW(BuffAddr,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),p8s[1]+2+2);
		#else
			MW(BuffAddr,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),p8s[1]+2+1);
		#endif
		}
		//��Ҫ�¼�����+1
		i+=1;
    MWR(i,ADDR_AFN0CF7_FM,1);
		//��Ҫ�¼��ȴ����ʱ�־ADC=1
		MWR(0x010101,ADDR_ADCFlags,3);
		
		if(MRR(ADDR_AFN05F29,1)==0x55)
		{//�����ն������ϱ�
			i=MRR(ADDR_AFN0CF7_FM+2,1);//GPRS��ǰ��û�ϴ���ȷ����Ҫ�¼�����+1
			if(i!=0xff)
			{
				i+=1;
				MWR(i,ADDR_AFN0CF7_FM+2,1);
			}
			i=MRR(ADDR_AFN0CF7_FM+4,1);//��̫����ǰ��û�ϴ���ȷ����Ҫ�¼�����+1
			if(i!=0xff)
			{
				i+=1;
				MWR(i,ADDR_AFN0CF7_FM+4,1);
			}
		#if RS232AutoRePort==1//RS232�������ϱ�0=��ֹ,1=����
			i=MRR(ADDR_AFN0CF7_FM+6,1);//RS232��ǰ��û�ϴ���ȷ����Ҫ�¼�����+1
			if(i!=0xff)
			{
				i+=1;
				MWR(i,ADDR_AFN0CF7_FM+6,1);
			}
		#endif
		}
		else
		{//��ֹ�ն������ϱ�
			MC(0,ADDR_AFN0CF7_FM+2,6);//�嵱ǰ��û�ϴ���ȷ����Ҫ�¼�����,��ǰ��?����Ҫ�¼������ϴ���ȷ��
		}
	}
	else
	{//һ���¼�
		i=MRR(ADDR_AFN0CF7_FM+1,1);//һ���¼�����
		if(p8s[0]!=14)
		{//����ͣ�ϵ��¼�
			MW(BuffAddr,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),p8s[1]+2);
		}
		else
		{//��ͣ�ϵ��¼�,����1���ֽ��ϵ��¼��־
			MW(BuffAddr,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),p8s[1]+2+1);
		}
		i+=1;//һ���¼�����+1
		MWR(i,ADDR_AFN0CF7_FM+1,1);
	}
	//AFN0CF8�¼���־
	d64b=MRR(ADDR_AFN0CF8,8);
	d64b|=d64;
	MWR(d64b,ADDR_AFN0CF8,8);
	
	d64=0;
		//��Ҫ�¼���־
	for(i=0;i<256;i++)
	{
		x=MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),1);//ERC��
		x-=1;
		if(x<=63)
		{
			d64b=1;
			d64b<<=x;
			d64|=d64b;
		}
	}
		//һ���¼���־
	for(i=0;i<256;i++)
	{
		x=MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),1);//ERC��
		x-=1;
		if(x<=63)
		{
			d64b=1;
			d64b<<=x;
			d64|=d64b;
		}
	}
	d64b=MRR(ADDR_AFN0CF8,8);
	d64b&=d64;
	MWR(d64b,ADDR_AFN0CF8,8);
	
	PerERC_Event_Save(BuffAddr);//����ERC�¼���¼
	return 1;
}


#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����

u32 Get_ADDR_UARTnRMEvent(u32 PORTn)//�õ�UART�������ܱ��¼�ջ�ṹ��ַ
{
	switch(PORTn)
	{
		case RS485_1PORT:
			return ADDR_UARTRMEvent_4851;
		case RS485_2PORT:
			return ADDR_UARTRMEvent_4852;
		case RS485_3PORT:
			return ADDR_UARTRMEvent_4853;
		case RS485_4PORT://(�ز�)
			return ADDR_UARTRMEvent_4854;
	}
	return 0;
}

u32 Check_Meter_Event_Save(void)//�����ܱ��¼������ռ�;����0=�洢�M(û�ϱ�),1-255�����ɴ洢������
{
	u32 x;
	u32 y;
	
	x=MRR(ADDR_MeterEventCount_FM+2,4);
	y=x;
	x=(x&0xff)+((x>>8)&0xff);//GPRS��ǰ��û�ϴ���ȷ�ϵ��ܱ��¼�����+��ǰ��?�����ܱ��¼������ϴ���ȷ��
	y>>=16;
	y=(y&0xff)+((y>>8)&0xff);//��̫����ǰ��û�ϴ���ȷ�ϵ��ܱ��¼�����+��ǰ��?�����ܱ��¼������ϴ���ȷ��
	if(x>y)
	{
		x=y;
	}
	if(x>=255)
	{
		return 0;
	}
	x=255-x;
	return x;
}

void Meter_Event_WriteBuff(u32 PORTn,u32 Pn,u32 Addr_FnDataBuff,u8 *p8RMLib)//��ȡ�ĵ��ܱ��¼�д�뻺��ADDR_DATABUFF
{
	u32 i;
	u32 x;
	u32 y;
	
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

	switch(PORTn)
	{
		case ACSAMPLEPORT:
			i=1;
			break;
		case RS485_1PORT://RS485-1
			i=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			i=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			i=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			i=31;
			break;
		default:
			i=0;
			break;
	}
	MWR(i,ADDR_DATABUFF,1);//1byte �˿ں�
	MR(ADDR_DATABUFF+1,((u32)p8rxbuff)+1,6);//6BYTE ͨ�ŵ�ַ
	if(((UARTCtrl->RMCount&1)==0)&&(UARTCtrl->RMDI!=0xffffffff))
	{//���¼�������
		MWR(0,ADDR_DATABUFF+7,1);//������������=0
	}
	else
	{//�¼�
		MWR(7,ADDR_DATABUFF+7,1);//������������
		//ȡ��������DI	
		i=(u32)p8RMLib;
		i+=10+(5*(UARTCtrl->RMCount-1));
		MR(ADDR_DATABUFF+10,i,4);
		//ȡ��ǰ����ֵ
		i=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*(UARTCtrl->RMCount/2)),3);
		i=hex_bcd(i);
		MWR(i,ADDR_DATABUFF+14,3);
		//�������ݱ�ʶ����3�ֽڵ�
		i=MRR(((u32)p8rxbuff)+512+2,4);//ȡ�Ĵ�����ݱ�ʶ
		switch(i)
		{
			case 0x03120101://B70��������(�����й�)
			case 0x03120201://B70��������(�����й�)
			case 0x03120301://B70��������(1�����޹�)
			case 0x03120401://B70��������(2�����޹�)
			case 0x03120501://B70��������(3�����޹�)
			case 0x03120601://B70��������(4�����޹�)
				MWR(22,ADDR_DATABUFF+7,1);//������������
				i>>=8;
				i&=0xff;
				i--;
				x=i;
				for(i=0;i<6;i++)
				{
					y=MRR(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*((UARTCtrl->RMCount/2)-x))+(4*i),3);
					y=hex_bcd(y);
					MWR(y,ADDR_DATABUFF+14+(i*3),3);
				}
				break;
			case 0x03060001://B68������Դʧ��
				MWR(10,ADDR_DATABUFF+7,1);//������������
				MR(ADDR_DATABUFF+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI),3);
				break;
			case 0x03050001://B74ȫʧѹ
				MWR(10,ADDR_DATABUFF+7,1);//������������
				MR(ADDR_DATABUFF+14+3,ADDR_MeterEventCount+(Pn*LEN_MeterEventCount)+(4*NUM_MeterEventDI)+3,3);
				break;
		}
	}
	//
	i=p8FnData[0]+(p8FnData[1]<<8);
	MWR(i,ADDR_DATABUFF+8,2);//2byte����
	MR(ADDR_DATABUFF+32,((u32)p8rxbuff)+512,i);//����
}

void Meter_Event_Save(u32 Addr_Buff)//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
{	
	u32 i;
	
	i=MRR(ADDR_MeterEventCount_FM,1);//��Ҫ�¼�����
	MW(Addr_Buff,ADDR_MeterEvent+(LEN_MeterEvent*i),LEN_MeterEvent);
	//�¼�����+1
	i+=1;
  MWR(i,ADDR_MeterEventCount_FM,1);
	
	i=MRR(ADDR_MeterEventCount_FM+2,1);//GPRS��ǰ��û�ϴ���ȷ����Ҫ�¼�����+1
	if(i!=0xff)
	{
		i+=1;
		MWR(i,ADDR_MeterEventCount_FM+2,1);
	}
	i=MRR(ADDR_MeterEventCount_FM+4,1);//��̫����ǰ��û�ϴ���ȷ����Ҫ�¼�����+1
	if(i!=0xff)
	{
		i+=1;
		MWR(i,ADDR_MeterEventCount_FM+4,1);
	}
#if RS232AutoRePort==1//RS232�������ϱ�0=��ֹ,1=����
	i=MRR(ADDR_MeterEventCount_FM+6,1);//RS232��ǰ��û�ϴ���ȷ����Ҫ�¼�����+1
	if(i!=0xff)
	{
		i+=1;
		MWR(i,ADDR_MeterEventCount_FM+6,1);
	}
#endif
}

void WriteAutoReportWord(u32 PORTn,u32 rxbuff)//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
{
#if ((USER/100)==5)//�Ϻ��û�
#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
	u32 i;
	u32 y;
	u32 z;
	u32 f;
	u32 MeterNo;
	u8 *p8s;
	u8 *p8d;
	u64 CommAddr;
	
	f=0;//û��Ӧ�¼���־
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1;
			break;
		case RS485_1PORT://RS485-1
			PORTn=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			PORTn=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			PORTn=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			PORTn=31;
			break;
		default:
			return;
	}
	CommAddr=MRR(rxbuff+1,6);
	MeterNo=Check_AFN04F10_Addr(PORTn,CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
	if(MeterNo==0)
	{
		return;//û�ҵ�
	}
	MeterNo--;
	//��ǰ�Զ��ϱ�״̬��תΪ��λ��ͬƫ�Ƶ����������洢
	MR(ADDR_DATABUFF,ADDR_AutoReportWord+(MeterNo*LEN_AutoReportWord),96);
	for(i=0;i<96;i++)
	{
		if((i!=3)&&(i!=8))
		{//bit3ʱ�ӵ�ص�ѹ�͡�bit8ͣ�糭����Ƿѹ����
			MC(0,ADDR_DATABUFF+i,1);
		}
	}
	p8s=(u8*)(rxbuff);
	p8d=(u8*)(ADDR_DATABUFF);
	if(p8s[9]<=(4+12+2))
	{//���ȴ���ϸ�ʽ(ֻ��10�ֽ�״̬��)
		return;
	}
	y=p8s[14];
	if(y&0x20)
	{//bit5
		y=1;//û����������Ϊ0xff
		f=1;//û��Ӧ�¼���־
		p8d[5]=y;
	}
	if(y&0x80)
	{//bit7
		y=1;//û����������Ϊ0xff
		f=1;//û��Ӧ�¼���־
		p8d[7]=y;
	}
	//���ݻ�
	z=0;
	for(i=0;i<96;i++)
	{
		z|=p8d[i];
	}
	p8d[i]=z;
	//12byte �����ϱ��¼���λ��־�Ĵ�,���ڷ���λ����
	MR(ADDR_DATABUFF+96+1,rxbuff+14,12);
	//д
	MW(ADDR_DATABUFF,ADDR_AutoReportWord+(MeterNo*LEN_AutoReportWord),96+1+12);
	//û��Ӧ�¼�д�����ϱ�״̬����Ϊ�¼�
	if(f!=0)
	{
		MWR(PORTn,ADDR_DATABUFF,1);//1byte �˿ں�
		MR(ADDR_DATABUFF+1,rxbuff+1,6);//6BYTE ͨ�ŵ�ַ
		MWR(0,ADDR_DATABUFF+7,1);//������������=0
		//
		i=p8s[9];
		MWR(i,ADDR_DATABUFF+8,2);//2byte����
		MR(ADDR_DATABUFF+32,rxbuff+10,i);//����
		Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
	}
#endif
#endif

#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 f;
	u32 MeterNo;
	u8 *p8s;
	u8 *p8d;
	u64 CommAddr;
	
	f=0;//û��Ӧ�¼���־
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1;
			break;
		case RS485_1PORT://RS485-1
			PORTn=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			PORTn=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			PORTn=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			PORTn=31;
			break;
		default:
			return;
	}
	CommAddr=MRR(rxbuff+1,6);
	MeterNo=Check_AFN04F10_Addr(PORTn,CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
	if(MeterNo==0)
	{
		return;//û�ҵ�
	}
	MeterNo--;
	//��ǰ�Զ��ϱ�״̬��תΪ��λ��ͬƫ�Ƶ����������洢
	MC(0,ADDR_DATABUFF,96);
	z=0;//������λ����
	p8s=(u8*)(rxbuff);
	p8d=(u8*)(ADDR_DATABUFF);
	if(p8s[9]<=(4+12+2))
	{//���ȴ���ϸ�ʽ(ֻ��10�ֽ�״̬��)
		return;
	}
	for(i=0;i<12;i++)
	{
		for(x=0;x<8;x++)
		{
			y=p8s[14+i];
			y&=(1<<x);
			if(y!=0)
			{//����������
				y=p8s[14+12+1+z];
				if(y==0xff)
				{
					y=10;//û����������Ϊ0xff
				}
				if((((i*8)+x)>=1)&&(((i*8)+x)<=9))
				{//B1-B9û��Ӧ�¼�
					y=1;//û����������Ϊ0xff
					f=1;//û��Ӧ�¼���־
				}
				p8d[(i*8)+x]=y;
				z++;//������λ����
			}
		}
	}
	//���ݻ�
	z=0;
	for(i=0;i<96;i++)
	{
		z|=p8d[i];
	}
	p8d[i]=z;
	//12byte �����ϱ��¼���λ��־�Ĵ�,���ڷ���λ����
	MR(ADDR_DATABUFF+96+1,rxbuff+14,12);
	//д
	MW(ADDR_DATABUFF,ADDR_AutoReportWord+(MeterNo*LEN_AutoReportWord),96+1+12);
	//û��Ӧ�¼�д�����ϱ�״̬����Ϊ�¼�
	if(f!=0)
	{
		MWR(PORTn,ADDR_DATABUFF,1);//1byte �˿ں�
		MR(ADDR_DATABUFF+1,rxbuff+1,6);//6BYTE ͨ�ŵ�ַ
		MWR(0,ADDR_DATABUFF+7,1);//������������=0
		//
		i=p8s[9];
		MWR(i,ADDR_DATABUFF+8,2);//2byte����
		MR(ADDR_DATABUFF+32,rxbuff+10,i);//����
		Meter_Event_Save(ADDR_DATABUFF);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
	}
#endif
}
u32 Check_AutoReportWord(u32 PORTn)//�����ܱ������ϱ�״̬��;����:0=��,!=0 B31-B16Ϊ�������(1��ʼ),B15-B0ΪҪ��ȡ�������ϱ�״̬��λ��(0-95)
{
#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
	return 0;
#else
	u32 x;
	u8 *p8s;
	u8 *p8d;
	u32 Meter;
	u32 port;
	
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			port=1;
			break;
		case RS485_1PORT://RS485-1
			port=2;//RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			port=3;//RS485PORT=2;
			break;
		case RS485_3PORT://RS485-3
			port=4;//RS485PORT=3;
			break;
		case RS485_4PORT://RS485-4
			port=31;
			break;
		default:
			port=0;
			break;
	}
//ADDR_AFN04F10�����ڴ�
	#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
		#error (LEN_AFN04F10_Pn*AFN04F10MaxPn)>(128*1024)
	#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	p8s=(u8*)(ADDR_128KDATABUFF);
	for(Meter=0;Meter<AFN04F10MaxPn;Meter++)
	{
		x=p8s[2]+(p8s[3]<<8);
		if(x!=0)
		{//��������Ч
			x=p8s[4];
			x&=0x1f;
			if(x==port)
			{//�˿ں���ͬ
				//p8d=(u8*)(ADDR_AutoReportWord+(Meter*LEN_AutoReportWord));
			#if LEN_AutoReportWord>LEN_DATABUFF
				#error LEN_AutoReportWord>LEN_DATABUFF
			#endif
				MR(ADDR_DATABUFF,ADDR_AutoReportWord+(Meter*LEN_AutoReportWord),LEN_AutoReportWord);
				p8d=(u8*)(ADDR_DATABUFF);
				if(p8d[96]!=0)
				{//���ݻ��ֵ!=0
					for(x=0;x<96;x++)
					{
						if(p8d[x]!=0)
						{
							return (((Meter+1)<<16)+x);
						}
					}
					//ȫ0,�����ݻ�ֵ�ֽ�
					//MC(0,((u32)p8d)+96,1);
					MC(0,ADDR_AutoReportWord+(Meter*LEN_AutoReportWord)+96,1);
				}
			}
		}
		p8s+=LEN_AFN04F10_Pn;
	}
	return 0;
#endif
}

u32 RMEvene_YesNo(u32 PORTn,u32 Pn,u8 * p8RMLib,u32 RMCount,u32 Protocol)//������ּ��¼�������Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)
{
#if ((USER/100)==5)//�Ϻ�
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(Protocol==1)
	{
		return 0;
	}
	if(p8RMLib[0]!=3)
	{
		return 0;
	}
	if(Pn==0)
	{
		return 0;
	}
	Pn--;
	if((RMCount%3)==0)
	{//������
	#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
MeterEvent://���������ϱ�״̬�֣����͸�λ�����ϱ�״̬��
	#endif
		//ȡDI	
		x=(u32)p8RMLib;
		x+=10+(5*RMCount);
		x=MRR(x,4);
		if(Protocol==3)
		{
			x=Get_Di_From_OAD(x);
		}
		if(x==0)
		{//����
			return 0;
		}
		x>>=8;//ֻ�Ƚϸ�3�ֽ�,ʹ�������Ͷ��¼�������ͬ
	#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
//		//2014��12��26�չ���Ҫ��
//		//���ݷּ�����������ݲ�����С���Ĭ�ϣ������Ϊ1�������Ϊ2
//		y=Check_AFN04F10_Pn(Pn+1);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
//		i=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
//		switch(i)
//		{
//			case 0x51://�����ѹ�û�
//			case 0x71://����ֲ�ʽ�����û�
//				i=1;
//				break;
//			case 0x52://�����ѹ�û�
//			case 0x72://����ֲ�ʽ�����û�
//			case 0x02://ר���û�
//			case 0x62://������俼�˼�����
//				i=2;
//				break;
//			default://�����������5С���1����
//				i=1;
//				break;
//		}

//���ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����20150122.doc
//				��2 ���ݴ�С��Ĭ�������¼��ּ���
//			F105����Ԥ�����������ʼ�����豣��Ĭ�����ã�
//�ն�����	   ר���ն�	                         ������
//����˿�	   485�˿�	               485�˿�	                          �ز��˿�
//�û�����		�ܱ�	         �ܱ�	        ����	                    ����
//���ܱ�����	   �����	        �����  	�����  �����             �����  �����
//��С���  						  		 								0-2
//			�����ִ�С��		 0-2		 5-2	 ����				6-2		����
//								 6-2		 7-2						5-2
//																		7-2
//F106����ʶ	1��		 	 	 1��		 2��	  3��				4��		 5��
//ע��F106����������д���09�����ܵ��ܱ�֧�ֵĲ��֣��ն�����ݵ��ܱ�ظ�����񶨼������֡�

		//ȡ�˿ں�
		y=Check_AFN04F10_Pn(Pn+1);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		i=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+4,1);
		i&=0x1f;
		if((i==2) || (i==3))
		{//RS485-I�ڡ�RS485-II��
			//ȡ��С���
			i=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+26,1);
			switch(i)
			{
				case 0x02://ר���û�
				case 0x62://������俼�˼�����
					i=1;//ȡ�ּ����
					break;
				case 0x52://�����ѹ�û�
				case 0x72://����ֲ�ʽ�����û�
					i=2;//ȡ�ּ����
					break;
				default://����
					i=3;//ȡ�ּ����
					break;
			}
		}
		else
		{
			if(i==31)
			{//�ز���
				//ȡ��С���
				i=MRR(ADDR_AFN04F10+((y-1)*LEN_AFN04F10_Pn)+26,1);
				switch(i)
				{
					case 0x02://ר���û�
					case 0x62://������俼�˼�����
					case 0x52://�����ѹ�û�
					case 0x72://����ֲ�ʽ�����û�
						i=4;//ȡ�ּ����
						break;
					default://����
						i=5;//ȡ�ּ����
						break;
				}
			}
			else
			{
				return 0;
			}
		}
	#else//#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�������
		//p8=(u8*)(ADDR_AFN04F105+Pn);
		//i=p8[0];//ȡ�ּ����
		i=MRR(ADDR_AFN04F105+Pn,1);//ȡ�ּ����
	#endif
		if((i==0)||(i>MaxMeterEventClass))//�����ܱ��¼��ּ����
		{
			return 0;
		}
		i--;
	#if (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
		#error (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
	#endif
		//p8=(u8*)(ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)));
		MR(ADDR_DATABUFF,ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)),(5*NUM_AFN04F106_PerC));
		p8=(u8*)(ADDR_DATABUFF);
		for(i=0;i<NUM_AFN04F106_PerC;i++)
		{
			y=p8[4];
			if(y!=0)
			{//���ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
				if(y==UARTCtrl->RMEventTaskC)
				{//�ּ�ͬ
					y=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);
					y>>=8;//ֻ�Ƚϸ�3�ֽ�,ʹ�������Ͷ��¼�������ͬ
					if(((x&0xff)==0xff)||((y&0xff)==0xff))
					{//������
						if((x>>8)==(y>>8))
						{//���ݱ�ʶͬ
							return 1;
						}
					}
					else
					{
						if(y==x)
						{//���ݱ�ʶͬ
							return 1;
						}
					}
				}
			}
			p8+=5;
		}
	}
	else
	{//��1-10���¼�
	#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
		if(RMCount==((60)*3+1))
		{//��λ�����ϱ�״̬��
			//���������ϱ�״̬�֣����͸�λ�����ϱ�״̬��
			RMCount=((60)*3);
			goto MeterEvent;
		}
	#endif
		//ȡDI
		x=(u32)p8RMLib;
		x+=10+(5*RMCount);
		x=MRR(x,4);
		if(Protocol==3)
		{
			x=Get_Di_From_OAD(x);
		}
		if(x==0)
		{//����
			return 0;
		}
		p8=(u8*)(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount));
		i=RMCount;
		i/=3;
		i*=4;
		p8+=i;
		i=MRR(((u32)p8)+3,1);
		if((i!=0)&&(i!=0xff))
		//if((p8[3]!=0)&&(p8[3]!=0xff))
		{//��������hex!=0ͬʱ!=0xff
			return 1;
		}
	}
#else//#if ((USER/100)==5)//�Ϻ�
//�����Ϻ�
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(Protocol==1)
	{
		return 0;
	}
	if(p8RMLib[0]!=3)
	{
		return 0;
	}
	if(Pn==0)
	{
		return 0;
	}
	Pn--;
	if((RMCount&1)==0)
	{//������
		//ȡDI	
		x=(u32)p8RMLib;
		x+=10+(5*RMCount);
		x=MRR(x,4);
		if(x==0)
		{//����
			return 0;
		}
		if(x==0xffffffff)
		{//DI=0XFFFFFFFF�������¼���������ڴ˶�ʱ����
			x=Search_AFN04F106_NoEvene_DI(PORTn,Pn);//������ּ��¼�����AFN04F106�����з��¼��೭��;����0=û�ҵ�,1=�ҵ�
			return x;
		}
		//
		UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
		UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
		UARTCtrl->RMDI=0xffffffff;//��Ч

		//ȡ�ּ����
//		p8=(u8*)(ADDR_AFN04F105+Pn);
//		i=p8[0];
		i=MRR(ADDR_AFN04F105+Pn,1);
		if((i==0)||(i>MaxMeterEventClass))//�����ܱ��¼��ּ����
		{
			return 0;
		}
		i--;
	#if (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
		#error (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
	#endif
		//p8=(u8*)(ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)));
		MR(ADDR_DATABUFF,ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)),(5*NUM_AFN04F106_PerC));
		p8=(u8*)(ADDR_DATABUFF);
		for(i=0;i<NUM_AFN04F106_PerC;i++)
		{
			y=p8[4];
			if(y!=0)
			{//���ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
				if(y==UARTCtrl->RMEventTaskC)
				{//�ּ�ͬ
					y=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);
					if(y==x)
					{//���ݱ�ʶͬ
						return 1;
					}
				}
			}
			p8+=5;
		}
	}
	else
	{//��1-10���¼�
		p8=(u8*)(ADDR_MeterEventCount+(Pn*LEN_MeterEventCount));
		i=RMCount;
		i/=2;
		i*=4;
		p8+=i;
		//if((p8[3]!=0)&&(p8[3]!=0xff))
		i=MRR(((u32)p8)+3,1);
		if((i!=0)&&(i!=0xff))
		{//��������hex!=0ͬʱ!=0xff
			Search_AFN04F106_Evene_DI(PORTn,Pn,RMCount);//������ּ��¼��ĵ�ǰ�¼�����DI����AFN04F106�����зǷ����������¼��೭��DI,����:������UARTCtrl->RMDI=DI,û����UARTCtrl->RMDI=0xffffffff
			return 1;
		}
	}
#endif
	return 0;
}

void Search_AFN04F106_Evene_DI(u32 PORTn,u32 Pn,u32 RMCount)//������ּ��¼��ĵ�ǰ�¼�����DI����AFN04F106�����зǷ����������¼��೭��DI,����:������UARTCtrl->RMDI=DI,û����UARTCtrl->RMDI=0xffffffff
{
	u32 i;
	u32 x;
	u32 DI;
	u32 NumDI;
	u32 AND;
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->RMDI=0xffffffff;//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ

	//ȡDI
	x=DL645_2007_RMLib[3];
	x+=10+(5*RMCount);
	DI=MRR(x,4);
	if(DI==0)
	{//����
		return;
	}
	if((DI&0xff00)==0xff00)
	{
		AND=0xffff0000;//������
	}
	else
	{
		AND=0xffffff00;//������
	}
	DI&=AND;
	NumDI=MRR(x-5,4);
	
	//p8=(u8*)(ADDR_AFN04F105+Pn);
	//i=p8[0];//ȡ�ּ����
	i=MRR(ADDR_AFN04F105+Pn,1);//ȡ�ּ����
	if((i==0)||(i>MaxMeterEventClass))//�����ܱ��¼��ּ����
	{
		return;
	}
	i--;
	//p8=(u8*)(ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)));
	#if (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
		#error (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
	#endif
	MR(ADDR_DATABUFF,ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)),(5*NUM_AFN04F106_PerC));
	p8=(u8*)(ADDR_DATABUFF);
	i=UARTCtrl->SearchDICount;//�����¼�����AFN04F106������������
	if(i==0)
	{//��������=0,ͬʱͳ���ܸ���
		UARTCtrl->TotalDICount=0;//�����¼�����AFN04F106��������������DI����(��SearchDICount=0ʱ����)
		for(;i<NUM_AFN04F106_PerC;i++)
		{
			x=p8[4];
			if(x!=0)
			{//���ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
				if(x==UARTCtrl->RMEventTaskC)
				{//�ּ�ͬ
					x=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//ȡDI
					if(x!=NumDI)
					{//���Ƕ�����DI
						x&=AND;
						if(DI==x)
						{//��ͬ
							UARTCtrl->TotalDICount++;//�����¼�����AFN04F106��������������DI����(��SearchDICount=0ʱ����)
							if(UARTCtrl->RMDI==0xffffffff)
							{
								UARTCtrl->SearchDICount=i;//�����¼�����AFN04F106������������
								x=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//ȡDI
								UARTCtrl->RMDI=x;//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
							}
						}
					}
				}
			}
			p8+=5;
		}
		if(UARTCtrl->RMDI==0xffffffff)
		{
			UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
			UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
		}
		return;
	}
	p8+=(i*5);
	for(;i<NUM_AFN04F106_PerC;i++)
	{
		x=p8[4];
		if(x!=0)
		{//���ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
			if(x==UARTCtrl->RMEventTaskC)
			{//�ּ�ͬ
				x=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//ȡDI
				if(x!=NumDI)
				{//���Ƕ�����DI
					x&=AND;
					if(DI==x)
					{//��ͬ
						UARTCtrl->SearchDICount=i;//�����¼�����AFN04F106������������
						x=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//ȡDI
						UARTCtrl->RMDI=x;//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
						return;
					}
				}
			}
		}
		p8+=5;
	}
	UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
	UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
}
//�¼��������ݱ�ʶ����:
//1.����DI�ĸ�ʽΪxxxxxx00,���¼���¼DI�ĸ�ʽΪxxxxxx01,�¼��޷��������,��FF
//2.����DI�ĸ�ʽΪxxxxxx01,���¼���¼DI�ĸ�ʽΪxxxxFF01,,�¼��з��������,��FF
u32 Search_AFN04F106_NoEvene_DI(u32 PORTn,u32 Pn)//������ּ��¼�����AFN04F106�����з��¼��೭��;����0=û�ҵ�,1=�ҵ�
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 DI;
	u8 *p8;
	u8 * p8RMLib;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->RMDI=0xffffffff;//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
	//p8=(u8*)(ADDR_AFN04F105+Pn);
	//i=p8[0];//ȡ�ּ����
	i=MRR(ADDR_AFN04F105+Pn,1);//ȡ�ּ����
	if((i==0)||(i>MaxMeterEventClass))//�����ܱ��¼��ּ����
	{
		return 0;
	}
	i--;
	//p8=(u8*)(ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)));
	#if (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
		#error (5*NUM_AFN04F106_PerC)>LEN_DATABUFF
	#endif
	MR(ADDR_DATABUFF,ADDR_AFN04F106+(i*(5*NUM_AFN04F106_PerC)),(5*NUM_AFN04F106_PerC));
	p8=(u8*)(ADDR_DATABUFF);
	i=UARTCtrl->SearchDICount;//�����¼�����AFN04F106������������
	if(i==0)
	{//��������=0,ͬʱͳ���ܸ���
		UARTCtrl->TotalDICount=0;//�����¼�����AFN04F106��������������DI����(��SearchDICount=0ʱ����)
		for(;i<NUM_AFN04F106_PerC;i++)
		{
			x=p8[4];
			if(x!=0)
			{//���ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
				if(x==UARTCtrl->RMEventTaskC)
				{//�ּ�ͬ
					DI=p8[2]+(p8[3]<<8);//ȡDI��2�ֽ�
					p8RMLib=(u8*)DL645_2007_RMLib[3];
					z=p8RMLib[1];//ͨ�ű�ʶ����
					p8RMLib+=10;
					for(y=0;y<z;y++)
					{
						x=p8RMLib[2]+(p8RMLib[3]<<8);//ȡ����DI��2�ֽ�
						if(DI==x)
						{//��ͬ
							break;
						}
						p8RMLib+=5;
					}
					if(y>=z)
					{//����û��ͬ,�����¼��೭��
						UARTCtrl->TotalDICount++;//�����¼�����AFN04F106��������������DI����(��SearchDICount=0ʱ����)
						if(UARTCtrl->RMDI==0xffffffff)
						{
							UARTCtrl->SearchDICount=i;//�����¼�����AFN04F106������������
							DI=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//ȡDI
							UARTCtrl->RMDI=DI;//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
						}
					}
				}
			}
			p8+=5;
		}
		if(UARTCtrl->RMDI==0xffffffff)
		{
			UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
			UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
			return 0;
		}
		return 1;
	}
	p8+=(i*5);
	for(;i<NUM_AFN04F106_PerC;i++)
	{
		x=p8[4];
		if(x!=0)
		{//���ܱ����ݵȼ�BIN 1 (0��ʾɾ��)
			if(x==UARTCtrl->RMEventTaskC)
			{//�ּ�ͬ
				DI=p8[2]+(p8[3]<<8);//ȡDI��2�ֽ�
				p8RMLib=(u8*)DL645_2007_RMLib[3];
				z=p8RMLib[1];//ͨ�ű�ʶ����
				p8RMLib+=10;
				for(y=0;y<z;y++)
				{
					x=p8RMLib[2]+(p8RMLib[3]<<8);//ȡ����DI��2�ֽ�
					if(DI==x)
					{//��ͬ
						break;
					}
					p8RMLib+=5;
				}
				if(y>=z)
				{//����û��ͬ,�����¼��೭��
					UARTCtrl->SearchDICount=i;//�����¼�����AFN04F106������������
					DI=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);//ȡDI
					UARTCtrl->RMDI=DI;//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
					return 1;//Ҫ��
				}
			}
		}
		p8+=5;
	}
	UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
	UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
	return 0;//ûҪ��
}

#endif


#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
u32 ERC_Event_Save(u32 BuffAddr)//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
{
	u32 i;
	//��û��¼�ϵ��¼����ȼ��ϵ��¼�����ԭ�����¼�
	if(MRR(ADDR_ERC14SOUR,1)==0)//ERC14�¼���¼��־(�ϵ�)
	{
		MWR(0x14,ADDR_ERC14SOUR,1);//ERC14�¼���¼��־
		if(MRR(ADDR_UpFlags,4)!=0x12345678)//4BYTE ������־:0x12345678Ϊ���Ź���λ,����Ϊ���ŵ�Ӳ����λ
		{
			ERC14_Event_End();//�ն�ͣ/�ϵ��¼�(�ϵ�),��ר���¼����ݻ���
		}
	}
	//��¼�¼�
	i=ERC_Event_Save_Re(BuffAddr);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	return i;
}
#endif
#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
u32 ERC_Event_Save(u32 BuffAddr)//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
{
	u32 i;
	u32 x;

	//��û��¼�ϵ��¼����ȼ��ϵ��¼�����ԭ�����¼�
	if(Terminal_Ram->PowerNote==0)//�ϵ��¼���¼:0=û��¼,!=0�Ѽ�¼;
	{
	//�Ȳ���Ҫ�¼�ͣ���¼���¼
		for(i=0;i<256;i++)
		{
			if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),1)==14)
			{//ERC=14,��ͣ/�ϵ��¼�
				if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+8,6)==0xeeeeeeeeeeee)
				{//�ϵ��¼��־=0xee
					MC(0,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+8+5,1);//�ϵ��¼��־=0
					x=MRR(ADDR_AFN0CF7_FM,1);
					if(i==x)
					{
						x++;
						MWR(x,ADDR_AFN0CF7_FM,1);
					}
				}
			}
		}
	//�ٲ�һ���¼�ͣ���¼���¼
		for(i=0;i<256;i++)
		{
			if(MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),1)==14)
			{//ERC=14,��ͣ/�ϵ��¼�
				if(MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i)+8,6)==0xeeeeeeeeeeee)
				{//�ϵ��¼��־=0xee
					MC(0,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i)+8+5,1);//�ϵ��¼��־=0
					x=MRR(ADDR_AFN0CF7_FM+1,1);
					if(i==x)
					{
						x++;
						MWR(x,ADDR_AFN0CF7_FM+1,1);
					}
				}
			}
		}
	}
	//��¼�¼�
	i=ERC_Event_Save_Re(BuffAddr);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	return i;
}
#endif



extern  u8 ADDR_AFN09F1[];//�ն˰汾��Ϣ
#if ERC1==1
void ERC1_Event(u32 f)//���ݳ�ʼ���Ͱ汾�����¼;f=1���ݳ�ʼ��,f=2�汾���,f=3���ݳ�ʼ��ͬʱ�汾���
{
	u8 * p8;

	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=1;//ERC
	p8[1]=14;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=f;
	MR(ADDR_DATABUFF+8,ADDR_AFN0CF1+12,4);
	if((f&0x2)==0x2)
	{//�汾���
		MR(ADDR_DATABUFF+12,((u32)ADDR_AFN09F1)+12,4);
	}
	else
	{//û�汾���
		MR(ADDR_DATABUFF+12,ADDR_AFN0CF1+12,4);
	}
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC2==1
void ERC2_Event(u32 f)//������ʧ��¼;f=1�ն˲�����ʧ,f=2�����������ʧ,f=3�ն˲�����ʧͬʱ�����������ʧ
{
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=2;//ERC
	p8[1]=6;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=f;
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC3==1
void ERC3_Event(u32 rxbuff)//���������¼,��ڽ��ջ�����ַ
{
	u32 i;
	u32 x;
	u8 * p8;
	u8 * p8rxbuff;
	
	p8rxbuff=(u8 *)(rxbuff);
	i=0;
	while(i!=p8rxbuff[0])
	{
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=3;//ERC
		p8[1]=6;//����Le(��ֵ,��С10)
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
		p8[7]=p8rxbuff[11]>>1;//����վ��ַ
		x=0;
		while((8+(4*x))<=LEN_TERMINAL_EVENT)
		{
			if(i==p8rxbuff[0])
			{
				break;
			}
			MR(ADDR_DATABUFF+8+(4*x),rxbuff+14+(4*i),4);
			i++;
			x++;
			p8[1]+=4;
		}
		ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	}
}
#endif

#if ERC4==1
void ERC4_Event(u32 sc,u32 ss)//״̬����λ��¼;���sc=״̬��λ,ss=��λ��״̬
{
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=4;//ERC
	p8[1]=7;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=sc;
	p8[8]=ss;
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC5==1
void ERC5_Event(u32 n,u32 kw1,u32 kw2)//ң����բ��¼;���n=��բ�ִ�(0-7),kw1=��բʱ����(�ܼӹ���),kw2=��բ��2���ӵĹ���(�ܼӹ���)
{
	u64 d64;
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=5;//ERC
	p8[1]=10;//����Le
	//��բʱ��Ϊ��ǰ��¼ʱ���2��
	d64=MRR(ADDR_TYMDHMS+1,5);//��ʱ������
	d64=YMDHM_SubM(d64,2);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
	MWR(d64,ADDR_DATABUFF+2,5);
	p8[7]=1<<n;
	p8[8]=kw1;
	p8[9]=kw1>>8;
	p8[10]=kw2;
	p8[11]=kw2>>8;
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC6==1
void ERC6_Event(u32 Pn,u32 Bin,u32 Type,u32 kW1,u32 kW2,u32 kW3)//������բ��¼;���Pn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type�������,kw1��բǰ����,kw2��բ��2���ӹ���,kw3��բʱ���ʶ�ֵ
{
	u32 i;
	u32 x;
	u64 d64;
	u8 * p8;
	
	if(Bin==0x0)
	{
		return;
	}
	x=0x80;
	for(i=0;i<8;i++)
	{
		if((x&Bin)!=0x0)
		{
			break;
		}
		x>>=1;
	}
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=6;//ERC
	p8[1]=14;//����Le
	//��բʱ��Ϊ��ǰ��¼ʱ���2��
	d64=MRR(ADDR_TYMDHMS+1,5);//��ʱ������
	d64=YMDHM_SubM(d64,2);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
	MWR(d64,ADDR_DATABUFF+2,5);
	p8[7]=Pn;
	p8[8]=x;
	p8[9]=Type;
	p8[10]=kW1;
	p8[11]=kW1>>8;
	p8[12]=kW2;
	p8[13]=kW2>>8;
	p8[14]=kW3;
	p8[15]=kW3>>8;
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC7==1
void ERC7_Event(u32 Pn,u32 Bin,u32 Type,u32 kWh1,u32 kWh2)//�����բ��¼;���Pn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type������,kwh1��բʱ������,kwh2��բʱ��������ֵ
{
	u32 i;
	u32 x;
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=7;//ERC
	p8[1]=16;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;

	x=0;
	for(i=0;i<MaxControlRound;i++)
	{
		x<<=1;
		x|=1;
	}
	
	p8[8]=Bin&x;
	p8[9]=Type;
	MWR(kWh1,ADDR_DATABUFF+10,4);
	MWR(kWh2,ADDR_DATABUFF+14,4);
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC8==1
void ERC8_Event(u32 Pn,u32 FnDataBuff)//���ܱ�������,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
{
	u32 i;
	u32 f;
	u8 * p8;
	u8 * p8FnData;
	
	if(Pn>=ERC8MaxPn)
	{
		return;
	}
	p8=(u8 *)(ADDR_ERC8_SOUR+(ERC8_SOUR_Pn_LEN*Pn));
	p8FnData=(u8 *)(FnDataBuff);
	f=0;
	//���ܱ����ʱ�α仯
	if(p8[0]==p8FnData[0])
	{
		if(DataComp(((u32)p8)+1,((u32)p8FnData)+1,4)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		{
			f|=1;
		}
	}
	if(p8[5]==p8FnData[5])
	{
		if(DataComp(((u32)p8)+6,((u32)p8FnData)+6,4)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		{
			f|=1;
		}
	}
	//���ܱ����ո���
	for(i=0;i<3;i++)
	{
		if(p8[10+(2*i)]!=0xee)
		{
			if(p8FnData[10+(2*i)]!=0xee)
			{
				if(DataComp(((u32)p8)+10+(2*i),((u32)p8FnData)+10+(2*i),2)!=0x0)
				{
					f|=4;
				}
			}
		}
	}
	//���ܱ����峣������
	for(i=0;i<2;i++)
	{
		if(p8[10+6+(3*i)]!=0xee)
		{
			if(p8FnData[10+6+(3*i)]!=0xee)
			{
				if(DataComp(((u32)p8)+10+6+(3*i),((u32)p8FnData)+10+6+(3*i),3)!=0x0)
				{
					f|=8;
				}
			}
		}
	}
	//���ܱ�Ļ��������ʸ���
	for(i=0;i<2;i++)
	{
		if(p8[10+6+6+(3*i)]!=0xee)
		{
			if(p8FnData[10+6+6+(3*i)]!=0xee)
			{
				if(DataComp(((u32)p8)+10+6+6+(3*i),((u32)p8FnData)+10+6+6+(3*i),3)!=0x0)
				{
					f|=0x10;
				}
			}
		}
	}
	//���ܱ���ʱ�����
	if(p8[10+6+6+6]!=0xee)
	{
		if(p8FnData[10+6+6+6]!=0xee)
		{
			if(DataComp(((u32)p8)+10+6+6+6,((u32)p8FnData)+10+6+6+6,6)!=0x0)
			{
				f|=0x2;
			}
		}
	}
	//���ܱ������������
	if(p8[10+6+6+6+6]!=0xee)
	{
		if(p8FnData[10+6+6+6+6]!=0xee)
		{
			//if(DataComp(((u32)p8)+10+6+6+6+6,((u32)p8FnData)+10+6+6+6+6,6)!=0x0)
			if(DataComp(((u32)p8)+10+6+6+6+6,((u32)p8FnData)+10+6+6+6+6,3)!=0x0)
			{
				f|=0x20;
			}
		}
	}
	MW((u32)p8FnData,(u32)p8,ERC8_SOUR_Pn_LEN);
	if(f!=0x0)
	{
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=8;//ERC
		p8[1]=8;//����Le
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
		Pn++;
		p8[7]=Pn;
		p8[8]=Pn>>8;
		p8[9]=f;
		ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	}
}
#endif

#if ERC9==1
void ERC9_Event_Write(u32 Pn,u32 s,u32 f)//�����쳣д,���:Pn�������(��1��ʼ),s=1��ʼ=0�ָ�,f=�쳣��־
{
	u32 i;
	u8 * p8;

	if(ERC_Event_Set(9)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8=(u8 *)(ADDR_ERC9_SOUR+((Pn-1)*LEN_ERC9_SOUR_Pn));
		p8[0]=0;
		return;
	}
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=9;//ERC
	p8[1]=28;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(s<<7);
	p8[9]=f;
	i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	MR(ADDR_DATABUFF+10,i+37,15);
	i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	MR(ADDR_DATABUFF+25,i+6,5);
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}

void ERC9_3_Event(u32 Pn)//������·�쳣.����,���:Pn�������(��1��ʼ)
{
	u32 i;
	u32 is;//�����
	u32 Line;//���߷�ʽ
	u32 ia;
	u32 ib;
	u32 ic;
	u32 f;
	u8 * p8s;
	u8 * p8d;
/*
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
//��ѹ��·�쳣��������·�쳣�������쳣����ѹ/������ƽ��Խ�ޡ���ѹԽ�ޡ�����Խ�޵�6���¼����ն˲�Ҫ���ʹ��DL/T645-2007�Ĳ���������
	i=Check_ACSamplePn(Pn);//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
	if(i==0)
	{
		return;
	}
#endif
*/
	i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		p8d=(u8 *)(i);
		if(Pn<=ERC9MaxPn)
		{//Pn<=�������������(�������)
			Pn--;
			p8d+=43;
			is=MRR(ADDR_AFN04F25+(11*Pn)+6,1);//�������0.0A
			is<<=8;//000.000A
			is=bcd_hex(is);
			is*=5;
			is/=100;//5%IB
			is=hex_bcd(is);
			Line=MRR(ADDR_AFN04F25+(11*Pn)+15,1);//�����߷�ʽ
			Line&=3;
			ia=MRR((u32)p8d,3);//ȡA����
			ia&=0x7fffff;
			ib=MRR(((u32)p8d)+3,3);//ȡB����
			ib&=0x7fffff;
			ic=MRR(((u32)p8d)+6,3);//ȡC����
			ic&=0x7fffff;
	
			p8s=(u8 *)(ADDR_ERC9_SOUR+(Pn*LEN_ERC9_SOUR_Pn));
			f=0xc0;//����ָ���־=0
			//A�ָ�
			if(p8d[0]!=0xee)
			{
				if(ia>=is)
				{//IA>=is
					if((p8d[2]&0x80)==0x0)
					{
						if((p8s[0]&0x3)==0x3)
						{
							f|=1;
							p8s[0]&=0xfc;
						}
					}
				}
			}
			//B�ָ�
			if(Line!=0x1)
			{//��������
				if(p8d[3]!=0xee)
				{
					if(ib>=is)
					{//IB>=is
						if((p8d[5]&0x80)==0x0)
						{
							if((p8s[0]&0xc)==0xc)
							{
								f|=2;
								p8s[0]&=0xf3;
							}
						}
					}
				}
			}
			//C�ָ�
			if(p8d[6]!=0xee)
			{
				if(ic>=is)
				{//IC>=is
					if((p8d[8]&0x80)==0x0)
					{
						if((p8s[0]&0x30)==0x30)
						{
							f|=4;
							p8s[0]&=0xcf;
						}
					}
				}
			}
			if((f&0x07)!=0x0)
			{//�лָ�
				ERC9_Event_Write(Pn+1,0,f);//�����쳣д,���:Pn�������(��1��ʼ),s=1��ʼ=0�ָ�,f=�쳣��־
			}
			f=0xc0;//������ʼ��־=0
			//A����
			if(p8d[0]!=0xee)
			{
				if(ia>=is)
				{//IA>=is
					if((p8d[2]&0x80)!=0x0)
					{
						if((p8s[0]&0x3)==0x0)
						{
							f|=1;
							p8s[0]|=0x3;
						}
					}
				}
			}
			//B����
			if(Line!=0x1)
			{//��������
				if(p8d[3]!=0xee)
				{
					if(ib>=is)
					{//IB>=is
						if((p8d[5]&0x80)!=0x0)
						{
							if((p8s[0]&0xc)==0x0)
							{
								f|=2;
								p8s[0]|=0xc;
							}
						}
					}
				}
			}
			//C����
			if(p8d[6]!=0xee)
			{
				if(ic>=is)
				{//IC>=is
					if((p8d[8]&0x80)!=0x0)
					{
						if((p8s[0]&0x30)==0x0)
						{
							f|=4;
							p8s[0]|=0x30;
						}
					}
				}
			}
			if((f&0x07)!=0x0)
			{//�в���
				ERC9_Event_Write(Pn+1,1,f);//�����쳣д,���:Pn�������(��1��ʼ),s=1��ʼ=0�ָ�,f=�쳣��־
			}
			
		}
	}
}
#endif

#if ERC10==1
void ERC10_Event_Write(u32 Pn,u32 s,u32 f)//��ѹ�쳣д,���:Pn�������(��1��ʼ),s=1��ʼ=0�ָ�,f=�쳣��־
{
	u32 i;
	u8 * p8;
	if(ERC_Event_Set(10)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8=(u8 *)(ADDR_ERC10_SOUR+(LEN_ERC10_SOUR_Pn*(Pn-1)));
		p8[0]=0;
		return;
	}
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=10;//ERC
	p8[1]=28;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(s<<7);
	p8[9]=f;
	i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	MR(ADDR_DATABUFF+10,i+37,15);
	i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	MR(ADDR_DATABUFF+25,i+6,5);
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}

#if (((USER/100)==2)||((USER/100)==5))//�û���ʶ:ɽ��,�Ϻ�
void ERC10_Event(void)//��ѹ��·�쳣(����ʧѹ)
{
}
#else
void ERC10_Event(void)//��ѹ��·�쳣(����ʧѹ)
{
	u32 i;
	u32 up;//��������
	u32 ul;//ʧѹ����
	u32 is;//�����
	u32 Line;//���߷�ʽ
	u32 ua;
	u32 ub;
	u32 uc;
	u32 ia;
	u32 ib;
	u32 ic;
	u32 fui;//��ѹ����������Ч��־
	u32 f1;//�����־
	u32 f2;//ʧѹ��־	

	u8 * p8s;
	u8 * p8d;
	u16 * p16;

	u32 Pn;
	Pn=Terminal_Ram->I2SUBTask4_15;//ERC10_Event();//��ѹ��·�쳣(���ࡢʧѹ)
	if(Pn>AFN04F25MaxPn)
	{
		Pn=1;
	}
/*
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
//��ѹ��·�쳣��������·�쳣�������쳣����ѹ/������ƽ��Խ�ޡ���ѹԽ�ޡ�����Խ�޵�6���¼����ն˲�Ҫ���ʹ��DL/T645-2007�Ĳ���������
	i=Check_ACSamplePn(Pn);//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
	if(i==0)
	{
		return;
	}
#endif
*/
	i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		if(Pn<=AFN04F25MaxPn)
		{//Pn<=�������������(�������)
			if(Pn<=AFN04F26MaxPn)
			{//Pn<=��������ֵ����(�������)
				Pn--;
				up=MRR(ADDR_AFN04F26+(57*Pn)+4,2);//����������
				ul=MRR(ADDR_AFN04F25+(11*Pn)+4,2);//�����ѹ000.0V����ʧѹ����
				ul=bcd_hex(ul);
				ul*=60;//60%UN
				ul/=100;
				ul=hex_bcd(ul);
				
				is=MRR(ADDR_AFN04F25+(11*Pn)+6,1);//�������0.0A
				is<<=8;//000.000A
				is=bcd_hex(is);
				is*=5;
				is/=100;//5%IB
				is=hex_bcd(is);
				Line=MRR(ADDR_AFN04F25+(11*Pn)+15,1);//�����߷�ʽ
				Line&=3;
				ua=MRR(i+37,2);//ȡA��ѹ
				ub=MRR(i+39,2);//ȡB��ѹ
				uc=MRR(i+41,2);//ȡC��ѹ
				ia=MRR(i+43,3);//ȡA����
				ia&=0x7fffff;
				ib=MRR(i+46,3);//ȡB����
				ib&=0x7fffff;
				ic=MRR(i+49,3);//ȡC����
				ic&=0x7fffff;
				p8s=(u8 *)(i);
				p8s+=37;
				fui=0;//��ѹ����������Ч��־
				f1=0;//�����־
				//A��
				if(p8s[0]!=0xee)
				{
					if(p8s[6]!=0xee)
					{
						fui|=1;//��ѹ����������Ч��־
						if(ua<up)
						{
							if(ia<is)
							{
								f1|=1;//�����־
							}
						}
					}
				}
				//B��
				if(Line!=0x1)
				{//��������
					if(p8s[2]!=0xee)
					{
						if(p8s[9]!=0xee)
						{
							fui|=2;//��ѹ����������Ч��־
							if(ub<up)
							{
								if(ib<is)
								{
									f1|=2;//�����־
								}
							}
						}
					}
				}
				//C��
				if(p8s[4]!=0xee)
				{
					if(p8s[12]!=0xee)
					{
						fui|=4;//��ѹ����������Ч��־
						if(uc<up)
						{
							if(ic<is)
							{
								f1|=4;//�����־
							}
						}
					}
				}
				f2=0;//ʧѹ��־	
				//A��
				if(p8s[0]!=0xee)
				{
					if(p8s[6]!=0xee)
					{
						fui|=1;//��ѹ����������Ч��־
						if(ua<ul)
						{
							if(ia>=is)
							{
								f2|=1;//ʧѹ��־	
							}
						}
					}
				}
				//B��
				if(Line!=0x1)
				{//��������
					if(p8s[2]!=0xee)
					{
						if(p8s[9]!=0xee)
						{
							fui|=2;//��ѹ����������Ч��־
							if(ub<ul)
							{
								if(ib>=is)
								{
									f2|=2;//ʧѹ��־
								}
							}
						}
					}
				}
				//C��
				if(p8s[4]!=0xee)
				{
					if(p8s[12]!=0xee)
					{
						fui|=4;//��ѹ����������Ч��־
						if(uc<ul)
						{
							if(ic>=is)
							{
								f2|=4;//ʧѹ��־
							}
						}
					}
				}

				p8d=(u8 *)(ADDR_ERC10_SOUR+(LEN_ERC10_SOUR_Pn*Pn));
				p16=(u16 *)(ADDR_ERC10_Timer);//ERC10����ʧѹ�¼���ʱ�붨ʱ��
				i=f1|(f2<<4);
				if(i==p8d[1])
				{
					if(p16[Pn]==0x0)
					{//��ʱ�붨ʱ=0
						//����ָ�
						i=~f1;
						i&=0x7;
						i&=fui;
						if((i&0x1)!=0x0)
						{
							if((p8d[0]&0x3)!=0x1)
							{
								i&=0xfe;
							}
						}
						if((i&0x2)!=0x0)
						{
							if((p8d[0]&0x0c)!=0x4)
							{
								i&=0xfd;
							}
						}
						if((i&0x4)!=0x0)
						{
							if((p8d[0]&0x30)!=0x10)
							{
								i&=0xfb;
							}
						}
						if(i!=0x0)
						{
							i|=1<<6;
							ERC10_Event_Write(Pn+1,0,i);//��ѹ�쳣д,���:Pn�������(��1��ʼ),s=1��ʼ=0�ָ�,f=�쳣��־
						}
						//ʧѹ�ָ�
						i=~f2;
						i&=0x7;
						i&=fui;
						if((i&0x1)!=0x0)
						{
							if((p8d[0]&0x3)!=0x2)
							{
								i&=0xfe;
							}
						}
						if((i&0x2)!=0x0)
						{
							if((p8d[0]&0x0c)!=0x8)
							{
								i&=0xfd;
							}
						}
						if((i&0x4)!=0x0)
						{
							if((p8d[0]&0x30)!=0x20)
							{
								i&=0xfb;
							}
						}
						if(i!=0x0)
						{
							i|=2<<6;
							ERC10_Event_Write(Pn+1,0,i);//��ѹ�쳣д,���:Pn�������(��1��ʼ),s=1��ʼ=0�ָ�,f=�쳣��־
						}
						//�������
						i=f1;
						if((i&0x1)!=0x0)
						{
							if((p8d[0]&0x3)==0x1)
							{
								i&=0xfe;
							}
						}
						if((i&0x2)!=0x0)
						{
							if((p8d[0]&0xc)==0x4)
							{
								i&=0xfd;
							}
						}
						if((i&0x4)!=0x0)
						{
							if((p8d[0]&0x30)==0x10)
							{
								i&=0xfb;
							}
						}
						if(i!=0x0)
						{
							i|=1<<6;
							ERC10_Event_Write(Pn+1,1,i);//��ѹ�쳣д,���:Pn�������(��1��ʼ),s=1��ʼ=0�ָ�,f=�쳣��־
						}
						//ʧѹ����
						i=f2;
						if((i&0x1)!=0x0)
						{
							if((p8d[0]&0x3)==0x2)
							{
								i&=0xfe;
							}
						}
						if((i&0x2)!=0x0)
						{
							if((p8d[0]&0xc)==0x8)
							{
								i&=0xfd;
							}
						}
						if((i&0x4)!=0x0)
						{
							if((p8d[0]&0x30)==0x20)
							{
								i&=0xfb;
							}
						}
						if(i!=0x0)
						{
							i|=2<<6;
							ERC10_Event_Write(Pn+1,1,i);//��ѹ�쳣д,���:Pn�������(��1��ʼ),s=1��ʼ=0�ָ�,f=�쳣��־
						}
						//�浱ǰ��־
						i=0;
						if((f1&1)!=0x0)
						{
							i|=1;
						}
						else
						{
							if((f2&1)!=0x0)
							{
								i|=2;
							}
						}
						if((f1&2)!=0x0)
						{
							i|=4;
						}
						else
						{
							if((f2&2)!=0x0)
							{
								i|=8;
							}
						}
						if((f1&4)!=0x0)
						{
							i|=0x10;
						}
						else
						{
							if((f2&4)!=0x0)
							{
								i|=0x20;
							}
						}
						p8d[0]=i;
					}
				}
				else
				{
					p8d[1]=i;
					i=MRR(ADDR_AFN04F26+(57*Pn)+56,1);//����ʧѹʱ����ֵ BIN min 1 
					i*=60;
					p16[Pn]=i;//�ȶ���ʱֵS
				}
				Pn++;
			}
		}
	}
	Pn++;
	Terminal_Ram->I2SUBTask4_15=Pn;
}
#endif
#endif

#if ERC11==1
void ERC11_Event(u32 Pn,u32 f)//�����쳣,���:Pn�������(��1��ʼ),f=0�ָ�=1����
{
	u32 i;
	u32 x;
	u8 * p8;
/*
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
//��ѹ��·�쳣��������·�쳣�������쳣����ѹ/������ƽ��Խ�ޡ���ѹԽ�ޡ�����Խ�޵�6���¼����ն˲�Ҫ���ʹ��DL/T645-2007�Ĳ���������
	i=Check_ACSamplePn(Pn);//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
	if(i==0)
	{
		return;
	}
#endif
*/
	i=GetClass1DataAddr(49,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		Pn--;
		p8=(u8 *)(ADDR_ERC11_SOUR);
		if(ERC_Event_Set(11)==0)//����¼���¼����,����0=����¼,1=��¼
		{
			i=1<<Pn%8;
			x=p8[Pn/8];
			x&=~i;
			p8[Pn/8]=x;
			return;
		}
		i=p8[Pn/8];
		i>>=Pn%8;
		i&=1;
		if((i^f)!=0x0)
		{//�仯
			i=1<<Pn%8;
			x=p8[Pn/8];
			x&=~i;
			i=f;
			i<<=Pn%8;
			x|=i;
			p8[Pn/8]=x;
			Pn++;
			p8=(u8 *)(ADDR_DATABUFF);
			p8[0]=11;//ERC
			p8[1]=24;//����Le
			MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
			p8[7]=Pn;
			p8[8]=(Pn>>8)|(f<<7);
			i=GetClass1DataAddr(49,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
			MR(ADDR_DATABUFF+9,i,12);
			i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
			MR(ADDR_DATABUFF+21,i+6,5);
			ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
		}
	}
}
#endif

#if ERC12==1
void ERC12_Event(u32 Pn)//���ܱ�ʱ�䳬��,���:Pn�������(��1��ʼ)
{
	u32 i;
	u32 x;
	u32 f;
	u8 * p8;
#if (USER/100)==5//�Ϻ�
	u32 HM;
#endif
	
	
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
#if (USER/100)==5//�Ϻ�
//�˶�Ƶ�ʣ�0-ÿ�պ˶�һ�Σ�1-ÿ�ܺ˶�һ�Σ�2-ÿ�º˶�һ�Σ�3-ÿ�ֳ���˶�һ�Ρ���Ĭ��3��
//�˶Կ�ʼ���ڣ�ÿ�պ˶�һ��Ϊ0��ÿ�ܺ˶�Ϊ1-7��ÿ�º˶�Ϊ1-31����Ĭ��0���˶�Ƶ��Ϊ3ʱ���жϴ˲�����
//�˶Կ�ʼСʱ��0-23����ʾ0�㵽23�㡣��Ĭ��4���˶�Ƶ��Ϊ3ʱ���жϴ˲�����
//�˶Կ�ʼ���ӣ�0-59����ʾ0�ֵ�59�֡���Ĭ��5���˶�Ƶ��Ϊ3ʱ���жϴ˲�����
	i=MRR(ADDR_AFN04F241,4);//�˶�Ƶ��
	x=i&0xff;
	HM=i>>24;//��ʼ��
	HM=hex_bcd(HM);
	f=(i>>16)&0xff;
	f=hex_bcd(f);
	f<<=8;
	HM|=f;
	switch(x)
	{
		case 0://ÿ�պ˶�һ��
			x=MRR(ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			f=MRR(ADDR_TYMDHMS+3,3);
			if(x==f)
			{
				return;//������У
			}
			f=MRR(ADDR_TYMDHMS+1,2);
			if(f<HM)
			{
				return;//û����ʼ��
			}
			MW(ADDR_TYMDHMS+3,ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			break;
		case 1://ÿ�ܺ˶�һ��
			x=(i>>8)&0xff;
			f=Comm_Ram->TYMDHMS[6];//��ǰ��0-6
			if(f==0)
			{
				f=7;
			}
			if(x!=f)
			{//�ܲ�ͬ
				return;
			}
			x=MRR(ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			f=MRR(ADDR_TYMDHMS+3,3);
			if(x==f)
			{
				return;//������У
			}
			f=MRR(ADDR_TYMDHMS+1,2);
			if(f<HM)
			{
				return;//û����ʼʱ��
			}
			MW(ADDR_TYMDHMS+3,ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			break;
		case 2://ÿ�º˶�һ��
			x=(i>>8)&0xff;
			f=Comm_Ram->TYMDHMS[3];//��ǰ��
			f=bcd_hex(f);
			if(x!=f)
			{//�ղ�ͬ
				return;
			}
			x=MRR(ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			f=MRR(ADDR_TYMDHMS+3,3);
			if(x==f)
			{
				return;//������У
			}
			f=MRR(ADDR_TYMDHMS+1,2);
			if(f<HM)
			{
				return;//û����ʼʱ��
			}
			MW(ADDR_TYMDHMS+3,ADDR_ERC12_SOUR_Ext+(LEN_ERC12_SOUR_Ext*(Pn-1)),3);
			break;
	}
#endif
	i=GetClass1DataAddr(162,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		MR(ADDR_DATABUFF,i+6,6);
		MR(ADDR_DATABUFF+6,i,6);
		p8=(u8 *)(ADDR_DATABUFF);
		if(p8[0]==0xee)
		{//û����
			return;
		}
		i=MRR(ADDR_DATABUFF+1,2);//ȡ���ܱ�ʱ��ʱ��
		if(i==0x2359)
		{//���ܱ�ʱ��ʱ��=23ʱ59��,���Ƚ�,��ʱ�����������Ϊ���ζ�ȡ
			return;
		}
		p8[4]&=0x1f;//ȥ�������ֽ��ϵ�����
		p8[10]&=0x1f;//ȥ�������ֽ��ϵ�����
		i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
		if(i==0)
		{//���ܱ�ʱ�ӺϷ�
			i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
			if((i>>31)!=0x0)
			{//��
				i=~i;
				i++;
			}
		#if (USER/100)==5//�Ϻ�
			x=MRR(ADDR_AFN04F241+4,1);//�������������	BIN	1
			x*=60;
		#else
			x=MRR(ADDR_AFN04F59+3,2);//���ܱ�Уʱ��ֵ	BIN	s	2
		#endif
			f=0;
			if(x!=0)
			{//�¼��жϷ�ֵΪ0ʱ,�����ж�
				if(i>=x)
				{
					f=1;
				}
			}
		}
		else
		{//���ܱ�ʱ�ӷǷ�
			f=1;
		}
		Pn--;
		p8=(u8 *)(ADDR_ERC12_SOUR);
		if(ERC_Event_Set(12)==0)//����¼���¼����,����0=����¼,1=��¼
		{
			i=1<<Pn%8;
			x=p8[Pn/8];
			x&=~i;
			p8[Pn/8]=x;
			return;
		}
		i=p8[Pn/8];
		i>>=Pn%8;
		i&=1;
		if((i^f)!=0x0)
		{//�仯
			i=1<<Pn%8;
			x=p8[Pn/8];
			x&=~i;
			i=f;
			i<<=Pn%8;
			x|=i;
			p8[Pn/8]=x;
			Pn++;
			p8=(u8 *)(ADDR_DATABUFF);
			p8[0]=12;//ERC
			p8[1]=7;//����Le
			MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
			p8[7]=Pn;
			p8[8]=(Pn>>8)|(f<<7);
			ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
		#if ((USER/100)==5)//�Ϻ��û�
			//�Ϻ���չERC61�����ܱ�ʱ��ƫ����
			p8=(u8 *)(ADDR_DATABUFF);
			p8[0]=61;//ERC
			p8[1]=14;//����Le
			i=GetClass1DataAddr(162,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
			MR(ADDR_DATABUFF+2,i,12);
			p8[14]=Pn;
			p8[15]=(Pn>>8)|(f<<7);
			ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
		#endif
		}
	}
}
#endif

#if ERC13==1
void ERC13_Event(u32 Pn,u32 FnDataBuff)//��������Ϣ,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
{
	u32 i;
	u32 f;
	u8 * p8;
	u8 * p8FnData;
	
	if(Pn>=ERC13MaxPn)
	{
		return;
	}
	if(ERC_Event_Set(13)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		MC(0xee,ADDR_ERC13_SOUR+(ERC13_SOUR_Pn_LEN*Pn),ERC13_SOUR_Pn_LEN);
		return;
	}
	
	p8=(u8 *)(ADDR_ERC13_SOUR+(ERC13_SOUR_Pn_LEN*Pn));
	p8FnData=(u8 *)(FnDataBuff);
	f=0;
#if (USER/100)!=5//�����Ϻ�
	//���ܱ��̴������������������������仯
	if(p8[0]!=0xee)
	{
		if(p8FnData[0]!=0xee)
		{
			if(DataComp(((u32)p8),((u32)p8FnData),3)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			{
				f|=1;
			}
		}
	}
	if(p8[3]!=0xee)
	{
		if(p8FnData[3]!=0xee)
		{
			if(DataComp(((u32)p8)+3,((u32)p8FnData)+3,3)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			{
				f|=1;
			}
		}
	}
	//���ܱ��������仯
	if(p8[6]==p8FnData[6])
	{
		if(DataComp(((u32)p8)+7,((u32)p8FnData)+7,3)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		{
			f|=2;
		}
	}
	//���ܱ�ʧѹ�����仯
	if(p8[10]!=0xee)
	{
		if(p8FnData[10]!=0xee)
		{
			if(DataComp(((u32)p8)+10,((u32)p8FnData)+10,3)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			{
				f|=4;
			}
		}
	}
	//���ܱ�ͣ������仯
	if(p8[13]!=0xee)
	{
		if(p8FnData[13]!=0xee)
		{
			if(DataComp(((u32)p8)+13,((u32)p8FnData)+13,3)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			{
				f|=8;
			}
		}
	}
#endif
	//���ܱ���Ƿѹ
	i=p8FnData[16];
	if(i!=0xee)
	{
		if(i==0x1)
		{//�ϵ�ʱ�����Ƿѹ��������ϱ�һ��
			if(i!=p8[16])
			{
				f|=0x10;
			}
		}
		else
		{
			if(p8[16]!=0xee)
			{
				if(i!=p8[16])
				{
					f|=0x10;
				}
			}
		}
	}
	MW((u32)p8FnData,(u32)p8,ERC13_SOUR_Pn_LEN);
	if(f!=0x0)
	{
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=13;//ERC
		p8[1]=8;//����Le
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
		Pn++;
		p8[7]=Pn;
		if((f&0x10)!=0x0)
		{//���Ƿѹ�в����ָ�,����ֻ�в���
			if(i==0x0)
			{//���ܱ���Ƿѹ�ָ�
				p8[8]=Pn>>8;
				p8[9]=0x10;
				ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				f&=0x0f;
				if(f==0x0)
				{
					return;
				}
			}
		}
		p8[8]=(Pn>>8)|0x80;
		p8[9]=f;
		ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	}
}
#endif

#if ERC14==1
#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
#if (USER/100)!=2//�û���ʶ:����ɽ����
void ERC14_Event_Start(void)//�ն�ͣ/�ϵ��¼�(ͣ��)
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=14;//ERC
	p8[1]=10;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//��ʱ������
	MC(0xee,ADDR_DATABUFF+7,6);//��ʱ������,�ϵ��¼��־=0xee
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
void ERC14_Event_End(void)//�ն�ͣ/�ϵ��¼�(�ϵ�),��ר���¼����ݻ���
{
	u32 i;
	u32 x;
	u8 * p8;
	
//�Ȳ���Ҫ�¼�ͣ���¼���¼
	for(i=0;i<256;i++)
	{
		if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),1)==14)
		{//ERC=14,��ͣ/�ϵ��¼�
			if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+7,6)==0xeeeeeeeeeeee)
			{//�ϵ��¼��־=0xee
				MC(0,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+7+5,1);//�ϵ��¼��־=0
				x=MRR(ADDR_AFN0CF7_FM,1);
				if(i==x)
				{
					x++;
					MWR(x,ADDR_AFN0CF7_FM,1);
				}
				MR(ADDR_ERC14SOUR,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),7);
				MW(ADDR_TYMDHMS+1,ADDR_ERC14SOUR+7,5);
				ERC_Event_Save_Re(ADDR_ERC14SOUR);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				return;
			}
		}
	}
	
//�ٲ�һ���¼�ͣ���¼���¼
	for(i=0;i<256;i++)
	{
		if(MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),1)==14)
		{//ERC=14,��ͣ/�ϵ��¼�
			if(MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i)+7,6)==0xeeeeeeeeeeee)
			{//�ϵ��¼��־=0xee
				MC(0,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i)+7+5,1);//�ϵ��¼��־=0
				x=MRR(ADDR_AFN0CF7_FM+1,1);
				if(i==x)
				{
					x++;
					MWR(x,ADDR_AFN0CF7_FM+1,1);
				}
				MR(ADDR_ERC14SOUR,ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*i),7);
				MW(ADDR_TYMDHMS+1,ADDR_ERC14SOUR+7,5);
				ERC_Event_Save_Re(ADDR_ERC14SOUR);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				return;
			}
		}
	}
//ԭûͣ���¼���¼,��ͣ���¼
	p8=(u8 *)(ADDR_ERC14SOUR);
	p8[0]=14;//ERC
	p8[1]=10;//����Le
	//ͣ��ʱ:��ʱ������
	MC(0xee,ADDR_ERC14SOUR+7,5);//�ϵ�ʱ�䣺��ʱ������
	MC(0,ADDR_ERC14SOUR+12,1);//�ϵ��¼��־=0,ͣ��ʱ��Ϊ0xee,�ϵ��¼ʱ��Ϊ0,���ڼ�¼ָ���1ʶ��
	ERC_Event_Save_Re(ADDR_ERC14SOUR);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
//ԭûͣ���¼���¼,�ټ�¼�ϵ��¼�
	//ͣ��ʱ:��ʱ������
	MR(ADDR_ERC14SOUR+7,ADDR_TYMDHMS+1,5);//��ʱ������
	ERC_Event_Save_Re(ADDR_ERC14SOUR);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#else//#if (USER/100)!=2//�û���ʶ:����ɽ����
//ɽ����
void ERC14_Event_Start(void)//�ն�ͣ/�ϵ��¼�(ͣ��),��ר���¼����ݻ���
{
	u8 * p8;
	p8=(u8 *)(ADDR_ERC14SOUR);
	p8[0]=14;//ERC
	p8[1]=10;//����Le
	MR(ADDR_ERC14SOUR+2,ADDR_TFM_YMDHMS+1,5);//ͣ��ʱ:��ʱ������
	MC(0xee,ADDR_ERC14SOUR+2+10,1);//1BYTE ͣ��ʱ��Ϊ0xee,�ϵ��¼ʱ��Ϊ0,���ڼ�¼ָ���1ʶ��
	ERC_Event_Save_Re(ADDR_ERC14SOUR);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
void ERC14_Event_End(void)//�ն�ͣ/�ϵ��¼�(�ϵ�),��ר���¼����ݻ���
{
	u32 i;
	u32 x;
	u8 * p8;
	p8=(u8 *)(ADDR_ERC14SOUR);
	p8[0]=14;//ERC
	p8[1]=10;//����Le
	//ͣ��ʱ:��ʱ������
	MR(ADDR_ERC14SOUR+7,ADDR_TYMDHMS+1,5);//��ʱ������
	MC(0,ADDR_ERC14SOUR+2+10,1);//1BYTE ͣ��ʱ��Ϊ0xee,�ϵ��¼ʱ��Ϊ0,���ڼ�¼ָ���1ʶ��
//�Ȳ���Ҫ�¼�ͣ���¼���¼
	for(i=0;i<256;i++)
	{
		if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i),1)==14)
		{//ERC=14,��ͣ/�ϵ��¼�
			if(MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+7+5,1)==0xee)
			{//�ϵ��¼��־=0xee
				MC(0,ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*i)+7+5,1);//1BYTE ͣ��ʱ��Ϊ0xee,�ϵ��¼ʱ��Ϊ0,���ڼ�¼ָ���1ʶ��
				x=MRR(ADDR_AFN0CF7_FM,1);
				if(i==x)
				{
					x++;
					MWR(x,ADDR_AFN0CF7_FM,1);
				}
			}
		}
	}
	ERC_Event_Save_Re(ADDR_ERC14SOUR);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif//#if (USER/100)!=2//�û���ʶ:����ɽ����
#endif//#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ

#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
void ERC14_Event_Start(u32 f)//�ն�ͣ/�ϵ��¼�(ͣ��);���:�ϵ��¼ʱ�¼�����ָ�����1��־,0=����1,0xee=���1
{
	u8 * p8;
#if (USER/100)==11//����
	u64 YMDHMS;
#endif

	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=14;//ERC
	p8[1]=11;//����Le
	p8[2]=1;//D0=����,D1=��Ч
#if (USER/100)==11//����
	//���ֲ���ʱͣ����ϵ�ʱ�����5���ܹ�̨��
	YMDHMS=MRR(ADDR_POWER_YMDHMS,6);
	YMDHMS=YMDHMS_AddS(YMDHMS,5);//������ʱ�����S��,���ؼ�S����������ʱ����
	YMDHMS>>=8;
	MWR(YMDHMS,ADDR_DATABUFF+3,5);
#else
	MR(ADDR_DATABUFF+3,ADDR_POWER_YMDHMS+1,5);//ERC14�¼��ж�����ǰ��ʱ��
#endif
	MC(0xee,ADDR_DATABUFF+8,5);//��ʱ������
	p8[13]=f;//�ϵ��¼��־
	ERC_Event_Save_Re(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
void ERC14_Event_End(u32 f)//�ն�ͣ/�ϵ��¼�(�ϵ�);���:���Ա�־
{
	u8 * p8;
#if (USER/100)==11//����
	u64 YMDHMS;
#endif

	p8=(u8 *)(ADDR_ERC14SOUR+2);
	p8[0]=14;//ERC
	p8[1]=11;//����Le
    p8[2]=f;//����
#if (USER/100)==7//�㽭������һ���ֽ������ж��Ƿ��ϱ�
    p8[2]&=0x03;//����   
#endif
 #if (USER/100)==11//����
	//���ֲ���ʱͣ����ϵ�ʱ�����5���ܹ�̨��
	YMDHMS=MRR(ADDR_ERC14SOUR+16,6);
	YMDHMS=YMDHMS_AddS(YMDHMS,5);//������ʱ�����S��,���ؼ�S����������ʱ����
	YMDHMS>>=8;
	MWR(YMDHMS,((u32)p8)+3,5);
	YMDHMS=MRR(ADDR_ERC14SOUR+22,6);
	YMDHMS=YMDHMS_AddS(YMDHMS,5);//������ʱ�����S��,���ؼ�S����������ʱ����
	YMDHMS>>=8;
	MWR(YMDHMS,((u32)p8)+8,5);
 #else
	MR(((u32)p8)+3,ADDR_ERC14SOUR+17,5);//ͣ��ʱ:��ʱ������
	MR(((u32)p8)+8,ADDR_ERC14SOUR+23,5);//�ϵ�ʱ�䣺��ʱ������
 #endif
	p8[13]=0;//�ϵ��¼��־=0,ͣ��ʱ��Ϊ0xee,�ϵ��¼ʱ��Ϊ0,���ڼ�¼ָ���1ʶ��
#if (USER/100)==7//�㽭������һ���ֽ������ж��Ƿ��ϱ�
    p8[14]=(f&0x80);
#endif
	ERC_Event_Save(ADDR_ERC14SOUR+2);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif//#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
#endif//#if ERC14==1






#if ERC15==1//ERC15г��Խ�޸澯
//�Ѱ�����ͳ����
#endif

#if ERC16==1//ֱ��ģ����Խ��
void ERC16_Event(u32 Pn,u32 ss,u32 f,u32 dc)//ֱ��ģ����Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=Խ�ޱ�־,dc=��ǰֱ��ģ����
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=16;//ERC
	p8[1]=9;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//��ʱ������
	p8[7]=Pn|(ss<<7);
	p8[8]=f;
	p8[9]=dc;
	p8[10]=dc>>8;
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC17==1
	#if (USER/100)==5//�Ϻ�
void ERC17_Event(u32 Pn,u32 ss,u32 f,u32 unu,u32 uni)//��ѹ/������ƽ���Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,unu=ѹ��ƽ���,uni=������ƽ���
{
}
	#else
void ERC17_Event(u32 Pn,u32 ss,u32 f,u32 unu,u32 uni)//��ѹ/������ƽ���Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,unu=ѹ��ƽ���,uni=������ƽ���
{
	u32 i;
	u8 * p8;
/*
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
//��ѹ��·�쳣��������·�쳣�������쳣����ѹ/������ƽ��Խ�ޡ���ѹԽ�ޡ�����Խ�޵�6���¼����ն˲�Ҫ���ʹ��DL/T645-2007�Ĳ���������
	i=Check_ACSamplePn(Pn);//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
	if(i==0)
	{
		return;
	}
#endif
*/
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=17;//ERC
	p8[1]=27;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(ss<<7);
	p8[9]=f;
	p8[10]=unu;
	p8[11]=unu>>8;
	p8[12]=uni;
	p8[13]=uni>>8;
	i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		MR(ADDR_DATABUFF+14,i+37,15);
		ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	}
}
	#endif
#endif

#if ERC18==1

#endif

#if ERC19==1
void ERC19_Event(u32 Pn,u32 RxAddr,u32 kWh1,u32 kWh2)//����������ü�¼
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=19;//ERC
	p8[1]=31;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	MR(ADDR_DATABUFF+8,RxAddr,17);//�������
	MWR(kWh1,ADDR_DATABUFF+25,4);//���ι���ǰʣ����������ѣ�
	MWR(kWh2,ADDR_DATABUFF+29,4);//���ι����ʣ����������ѣ�
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC20==1
void ERC20_Event(u64 PWL,u64 PWH,u32 MSA)//��Ϣ��֤�����¼,���:PWH_PWL��Ϣ��֤��,MSA=����վ��ַ
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=20;//ERC
	p8[1]=22;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	MWR(PWL,ADDR_DATABUFF+7,8);//д�Ĵ������ݵ�������,Byte<=8
	MWR(PWH,ADDR_DATABUFF+7+8,8);//д�Ĵ������ݵ�������,Byte<=8
	p8[23]=MSA;
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif


#if ERC21==1
void ERC21_Event(void)//ERC21 485��������¼�
{
	u32 i;
	u32 x;
	u32 f;
	u32 MeterNo;
	u32 Addr;
	u8 * p8;

	f=1;//����
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		x=MRR(Addr,2);//����������� BIN 2 
		if(x!=0)
		{//���������Ч
			x=MRR(Addr+2,1);//ͨ�����ʼ��˿ں� BIN 1 
			x&=0x1f;
			if((x>=2)&&(x<=4))
			{//��RS485-1-2-3
				MeterNo=i;
				p8=(u8 *)(ADDR_RM_CountFlags+32+(MeterNo/4));
				MeterNo%=4;
				MeterNo<<=1;
				if(((p8[0]>>MeterNo)&3)<=1)
				{//0=�ɹ�,1=ʧ��->�ɹ�,2=�ɹ�->ʧ��,3=ʧ��
					f=0;
				}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	if(f==1)
	{//ȫ��ʧ��,��¼485�������
		i=MRR(ADDR_ERC21_Flag,1);//ERC21 485��������¼���¼��־
		if(i==0)
		{//û��¼
			p8=(u8 *)(ADDR_DATABUFF);
			p8[0]=21;//ERC
			p8[1]=6;//����Le
			MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
			p8[7]=4;//4=485�������
			i=ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
			MWR(i,ADDR_ERC21_Flag,1);//ERC21 485��������¼���¼��־
		}
	}
}
#endif

#if ERC22==1
void ERC22_Event(void)//�й��ܵ������Խ�޼�����¼���¼
{
#if MaxDiffGroup!=0
	u32 i;
	u32 Pn;//����
	u32 x;//�Աȵ��ܼ����
	u32 y;//�����ܼ����
	u32 absolute;//����ֵ
	u32 relative;//���ֵ
	u32 f;
	u32 Len;//�¼����ݳ���
	u64 a64;
	u64 b64;
	
	u8 * p8;
	u8 * p8s;
	u16 * p16;
	
	Pn=Terminal_Ram->ERC22Task;
	if(Pn>=MaxDiffGroup)
	{
		Pn=0;
		Terminal_Ram->ERC22Task=0;
	}
	p16=(u16 *)(ADDR_ERC22_Timer);//ERC22�Խ���¼��붨ʱ��
	if(p16[Pn]==0x0)
	{//���䶨ʱ��
		i=MRR(ADDR_AFN04F15+(Pn*9)+3,1);//ȡ�����ĵ�������ʱ�����估�Աȷ�����־
		i&=3;
		switch(i)
		{
			case 0:
				i=60*60;
				break;
			case 1:
				i=30*60;
				break;
			default:
				i=15*60;
				break;
		}
		p16[Pn]=i;//�����䶨ʱS
		
		x=MRR(ADDR_AFN04F15+(Pn*9)+1,1);//�Աȵ��ܼ����
		if((x!=0x0)&&(x<=MaxTotalAddGroup))
		{
			x--;
			y=MRR(ADDR_AFN04F15+(Pn*9)+2,1);//�����ܼ����
			if((y!=0x0)&&(y<=MaxTotalAddGroup))
			{
				y--;
				//ȡ�Ա��ܼӵ���
				i=MRR(ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn)+(x*4),4);//�����������ݵ��Ĵ���
				a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				//ȡ�����ܼӵ���
				i=MRR(ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn)+(y*4),4);//�����������ݵ��Ĵ���
				b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				//����ƫ��ֵ
				a64-=b64;
				if((a64>>63)!=0x0)
				{//��
					a64=(~a64)+1;
				}
				absolute=kWh_Data03(a64);
				//���ƫ��ֵ
				relative=(a64*100)/b64;
				//�Ƚ�
				f=0;
				i=MRR(ADDR_AFN04F15+(Pn*9)+3,1);//ȡ�����ĵ�������ʱ�����估�Աȷ�����־
				i>>=7;
				if(i==0x0)
				{//��ԶԱ�
					i=MRR(ADDR_AFN04F15+(Pn*9)+4,1);//ȡ�Խ�����ƫ��ֵ
					if(i==0)
					{//�ȶ�ֵ=0���Ƚ�
						p8=(u8 *)(ADDR_ERC22_SOUR);//ԭ��鳬�ޱ�־,0=û��,1=����
						p8[Pn]=0;
					}
					else
					{
						if(relative>=i)
						{
							f=1;
						}
					}
				}
				else
				{//���ԶԱ�
					i=MRR(ADDR_AFN04F15+(Pn*9)+5,4);//ȡ�Խ�޾���ƫ��ֵ
					i&=0xefffffff;//ȥ����,��������ֵ
					if(i==0)
					{//�ȶ�ֵ=0���Ƚ�
						p8=(u8 *)(ADDR_ERC22_SOUR);//ԭ��鳬�ޱ�־,0=û��,1=����
						p8[Pn]=0;
					}
					else
					{
						b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
						if(a64>=b64)
						{
							f=1;
						}
					}
				}
				p8=(u8 *)(ADDR_ERC22_SOUR);//ԭ��鳬�ޱ�־,0=û��,1=����
				if((f^p8[Pn])!=0x0)
				{//�仯
					p8[Pn]=f;
					p8=(u8 *)(ADDR_DATABUFF);
					p8[0]=22;//ERC
					MR(ADDR_DATABUFF+2,ADDR_TFM_YMDHMS+1,5);//��ʱ������
					p8[7]=(Pn+1)|(f<<7);//D7����/ֹ��־	D6=0	D5��D0������������	BIN	1
					MR(ADDR_DATABUFF+8,ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn)+(x*4),4);//Խ��ʱ�Ա��ܼ����й��ܵ�����	����¼A.3	4
					MR(ADDR_DATABUFF+12,ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn)+(y*4),4);//Խ��ʱ�����ܼ����й��ܵ�����	����¼A.3	4
					p8[16]=relative;//Խ��ʱ�Խ�����ƫ��ֵ	BIN	1
					MWR(absolute,ADDR_DATABUFF+17,4);//Խ��ʱ�Խ�޾���ƫ��ֵ	����¼A.3	4
					p8s=(u8 *)(ADDR_AFN04F14+((2+64)*x));//�Ա��ܼ�������
					f=MRR(((u32)(p8s))+1,1);//�Աȵ��ܼ������������n��1��n��64��	BIN	1
					p8[21]=f;
					p8s+=2;
					Len=22;//�ܳ�����22�ֽ�
					for(i=0;i<f;i++)
					{//�Ա��ܼ�����������й��ܵ���ʾֵ
						if((Len+5)<=LEN_TERMINAL_EVENT)//�ն��¼���¼�̶�����
						{
							x=GetClass1DataAddr(129,(MRR(((u32)(p8s))+i,1)&0x3f)+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
							if(x!=0x0)
							{
								MR(ADDR_DATABUFF+Len,x+6,5);
								Len+=5;
							}
						}
					}
					if(Len<LEN_TERMINAL_EVENT)
					{
						p8s=(u8 *)(ADDR_AFN04F14+((2+64)*y));//�����ܼ�������
						f=MRR(((u32)(p8s))+1,1);//�Աȵ��ܼ������������n��1��n��64��	BIN	1
						p8[Len]=f;
						Len++;
						p8s+=2;
						for(i=0;i<f;i++)
						{//�Ա��ܼ�����������й��ܵ���ʾֵ
							if((Len+5)<=LEN_TERMINAL_EVENT)//�ն��¼���¼�̶�����
							{
								x=GetClass1DataAddr(129,(MRR(((u32)(p8s))+i,1)&0x3f)+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
								if(x!=0x0)
								{
									MR(ADDR_DATABUFF+Len,x+6,5);
									Len+=5;
								}
							}
						}
					}
					p8[1]=Len-2;//����Le
					ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				}
			}
		}
		//���ܼӵ���
		MC(0,ADDR_ERC22_TotalEnergy+(4*MaxTotalAddGroup*Pn),4*MaxTotalAddGroup);
	}
	
	if(ERC_Event_Set(22)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8=(u8 *)(ADDR_ERC22_SOUR);//ԭ��鳬�ޱ�־,0=û��,1=����
		p8[Pn]=0;
	}
	Pn++;
	Terminal_Ram->ERC22Task=Pn;
#endif
}
#endif

#if ERC23==1
void ERC23_Event(u32 Pn,u32 Bin,u32 Type,u32 kWh1,u32 kWh2)//��ظ澯��¼;���Pn�ܼ����,Bin��բ�ִ�,Type������,kwh1�澯ʱ������,kwh2�澯ʱ��������ֵ
{
	u32 i;
	u32 x;
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=23;//ERC
	p8[1]=16;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	x=0;
	for(i=0;i<MaxControlRound;i++)
	{
		x<<=1;
		x|=1;
	}
	p8[8]=Bin&x;
	p8[9]=Type;
	MWR(kWh1,ADDR_DATABUFF+10,4);
	MWR(kWh2,ADDR_DATABUFF+14,4);
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC24==1
void ERC24_Event(u32 Pn,u32 ss,u32 f)//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
{
	u32 i;
	u8 * p8;
/*
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
//��ѹ��·�쳣��������·�쳣�������쳣����ѹ/������ƽ��Խ�ޡ���ѹԽ�ޡ�����Խ�޵�6���¼����ն˲�Ҫ���ʹ��DL/T645-2007�Ĳ���������
	i=Check_ACSamplePn(Pn);//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
	if(i==0)
	{
		return;
	}
#endif
*/
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=24;//ERC
	p8[1]=14;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(ss<<7);
	p8[9]=f;
	i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		MR(ADDR_DATABUFF+10,i+37,6);
		ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	}
}
#endif

#if ERC25==1
void ERC25_Event(u32 Pn,u32 ss,u32 f)//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
{
	u32 i;
	u8 * p8;
/*
#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
//��ѹ��·�쳣��������·�쳣�������쳣����ѹ/������ƽ��Խ�ޡ���ѹԽ�ޡ�����Խ�޵�6���¼����ն˲�Ҫ���ʹ��DL/T645-2007�Ĳ���������
	i=Check_ACSamplePn(Pn);//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
	if(i==0)
	{
		return;
	}
#endif
*/
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=25;//ERC
	p8[1]=17;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(ss<<7);
	p8[9]=f;
	i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		MR(ADDR_DATABUFF+10,i+43,9);
		ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	}
}
#endif

#if ERC26==1
void ERC26_Event(u32 Pn,u32 ss,u32 f,u32 kva1,u32 kva2)//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=26;//ERC
	p8[1]=14;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(ss<<7);
	p8[9]=f;
	MWR(kva1,ADDR_DATABUFF+10,3);
	MWR(kva2,ADDR_DATABUFF+13,3);
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC27==1
void ERC27_Event(u32 Pn,u32 FnDataBuff)//���ܱ�ʾ���½���¼(����1���ݴ���ǰ����¼�),���Pn=�������(1��ʼ),Fn�������ݻ����ַ
{
	u32 i;
	u8 * p8;
	
	i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
	if(i!=0x0)
	{
		p8=(u8 *)(i);
		if(p8[6]!=0xee)
		{
			p8=(u8 *)(FnDataBuff);
			if(p8[7]!=0xee)
			{
				i=DataComp(i+6,FnDataBuff+7,5);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
	#if (USER/100)==2//�û���ʶ:ɽ�������¼��ָ�
				if(i==0)
				{//����0=���
					return;
				}
				if(i==0x1)
				{//1=Data1>Data2
					i=1;//����
				}
				else
				{
					i=0;//�ָ�
				}
				p8=(u8*)(ADDR_ERC27_SOUR+((Pn-1)*LEN_ERC27_SOUR));
				if(i==p8[0])
				{//û�仯
					return;
				}
				p8[0]=i;
				p8=(u8 *)(ADDR_DATABUFF+2048);//����1���ݴ���ǰ����¼�,������DATABUFF����,�򽻲�����Ҳ����DATABUFF����
				p8[0]=27;//ERC
				p8[1]=17;//����Le
				MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
				p8[7]=Pn;
				p8[8]=(Pn>>8)|(i<<7);
				i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
				MR(ADDR_DATABUFF+2048+9,i+6,5);
				MR(ADDR_DATABUFF+2048+14,FnDataBuff+7,5);
				ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	#else
				if(i==0x1)
				{//1=Data1>Data2
					p8=(u8 *)(ADDR_DATABUFF+2048);//����1���ݴ���ǰ����¼�,������DATABUFF����,�򽻲�����Ҳ����DATABUFF����
					p8[0]=27;//ERC
					p8[1]=17;//����Le
					MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
					p8[7]=Pn;
					p8[8]=(Pn>>8)|(0x80);
					i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
					MR(ADDR_DATABUFF+2048+9,i+6,5);
					MR(ADDR_DATABUFF+2048+14,FnDataBuff+7,5);
					ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				}
	#endif
			}
		}
	}
}
#endif



void ClrERC29SOUR(u32 Pn)//���г���ʧ����ERC29���ܱ����,�����¼�ԭʼ����
{
	if(Pn<=ERC29MaxPn)
	{
		Pn--;
		MC(0xee,ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*Pn),1);//�ж�ʱֻ��1�ֽ�
	}
}

#if ERC29==1
//�ù��ʼ���ʵ�ʷ�����������
void ERC29_Event(u32 Pn,u32 FnDataBuff)//���ܱ����,����(����1���ݴ���ǰ����¼�),���Pn=�������(1��ʼ),Fn�������ݻ����ַ
{
	#if (USER/100)!=2//�û���ʶ:����ɽ����,ɽ������ר�õİ����ж��¼�
	u32 i;
	u32 x;
	u32 y;
	u32 t;
	u32 Errf;
	u32 f;
	double f64;
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	
	if((Pn<=ERC29MaxPn)&&(Pn!=0))
	{
		p8s=(u8 *)(ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)));
		i=ERC_Event_Set(29);//����¼���¼����,����0=����¼,1=��¼
		if(i==0)//����¼���¼����,����0=����¼,1=��¼
		{
			p8s[5]=0;
		}
		x=ERC_Event_Set(28);//����¼���¼����,����0=����¼,1=��¼
		if(x==0)//����¼���¼����,����0=����¼,1=��¼
		{
			p8s[6]=0;
		}
		i|=x;
		if(i==0)
		{
			p8s[0]=0xee;
			return;
		}
		i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
		if(i==0)
		{
			return;
		}
		p8d=(u8 *)(i);
		if(p8d[6]!=0xee)
		{//�п�ʼ����ֵ
			if(p8s[0]!=0xee)
			{//�п�ʼʱ����ֵ
				p8=(u8*)(FnDataBuff);
				if(p8[6]!=0xee)
				{//�е�ǰ����ֵ
					MR(ADDR_DATABUFF,ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)),1);//�ϴγ���ʱ����
					MR(ADDR_DATABUFF+1,i,5);//�ϴγ���ʱ���ʱ������
					MW(ADDR_TYMDHMS,ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)),5);//�����ϴγ���ʱ�����ʱ����
					MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);//��ǰʱ��
					t=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
					if((t>>31)==0)
					{//��ֵΪ��
						i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
						if(i==0)
						{
							return;
						}
						i=MRR(i+5,3);//ȡ���й�����xx.xxxx KW
						if(i!=0xeeeeee)
						{//���й�������Ч
							if(i&0x800000)
							{//����Ϊ��,����
								i=0;
							}
							i=bcd_hex(i);
							f64=i;
							f64*=t;
							f64/=3600;//kws��Ϊkwh,4λС��
							t=(u32)f64;//t=Ӧ�ߵ���kwh,4λС��
							if(t<=100)
							{//Ӧ�ߵ���kwh<=0.0100ʱ���Ƚ�
								return;
							}
							x=MRR(((u32)p8d)+5+1,4);//��ԭʾֵxxxx.xxxx
							y=MRR(FnDataBuff+6+1,4);//����ǰʾֵxxxx.xxxx
							x=bcd_hex(x);
							y=bcd_hex(y);
							if(y>=x)
							{//��ǰʾֵ>=ԭʾֵ
								i=y-x;//i=ʾֵ����ĵ���
								y=i*10;
								y/=t;//ʾֵ����ĵ���/ʵ��Ӧ�ߵ���=x.x
								y=hex_bcd(y);
								
								f=0;//���߱�־
								Errf=0;//�����־
								x=MRR(ADDR_AFN04F59+1,1);//ȡ���ܱ������ֵ	���ݸ�ʽ22		l
								if(x==0)
								{//��ֵ=0
									x=~x;
								}
								if(y>x)
								{//(ʾֵ����ĵ���/ʵ�ʷ�������)>���ܱ������ֵ,����Ϊ���ܱ����
									f=1;//���߱�־
									p8s[5]&=0x0f;//��ָ���ʱ����
									p8s[6]|=0xf0;//����ָ�����(���ϻָ�)
									if(p8s[6]&0x01)
									{//ԭ�����Ѳ���,�����
										Errf=1;//�����־
									}
								}
								else
								{
									x=MRR(ADDR_AFN04F59,1);//ȡ���ܱ�����ֵ	���ݸ�ʽ22		l
									if(x==0)
									{//��ֵ=0
										x=~x;
									}
									if(y>x)
									{//(ʾֵ����ĵ���/ʵ�ʷ�������)>���ܱ�����ֵ,����Ϊ���ܱ���
										Errf=1;//�����־
										p8s[6]&=0x0f;//��ָ���ʱ����
										p8s[5]|=0xf0;//���߻ָ�����(���ϻָ�)
									}
//									else
//									{//������
//										if(t>=i)
//										{
//											i=t-i;
//											y=i*10;
//											y/=t;//ʾֵ����ĵ���/ʵ��Ӧ�ߵ���=x.x
//											y=hex_bcd(y);
//											if(y>x)
//											{//(ʾֵ����ĵ���/ʵ�ʷ�������)>���ܱ�����ֵ,����Ϊ���ܱ���
//												Errf=1;//�����־
//												p8s[6]&=0x0f;//��ָ���ʱ����
//												p8s[5]|=0xf0;//���߻ָ�����(���ϻָ�)
//											}
//										}
//									}
								}
							//��¼
								//����ָ���¼
								if(Errf==0)
								{
									if((p8s[6]&0xf0)<0x30)
									{//�ָ���ʱ����(376.1�����������ָ��¼�����ʱ2�������ں����)
										//2016.7�ָĳ���ʱ3�������ں����
										p8s[6]+=0x10;
									}
									else
									{
										x=p8s[6]&0x1;
										if(x!=0)
										{
											if((x^Errf)!=0x0)
											{//�仯
												p8s[6]&=0xfe;
												p8s[6]|=Errf;
												p8=(u8 *)(ADDR_DATABUFF+2048);
												p8[0]=28;//ERC
												p8[1]=18;//����Le
												MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
												p8[7]=Pn;
												p8[8]=(Pn>>8)|(Errf<<7);
												MR(ADDR_DATABUFF+2048+9,((u32)p8d)+5+1,5);//�����ʱ��Ӧ�����й�����ʾֵ
												MR(ADDR_DATABUFF+2048+14,FnDataBuff+6+1,5);//�����ʱ�����й�����ʾֵ
												MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59,1);//ȡ���ܱ�����ֵ	���ݸ�ʽ22		l
												ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
											}
										}
									}
								}
								//���߻ָ���¼
								if(f==0)
								{
									if((p8s[5]&0xf0)<0x20)
									{//�ָ���ʱ����(376.1�����������ָ��¼�����ʱ2�������ں����)
										p8s[5]+=0x10;
									}	
									else
									{
										x=p8s[5]&0x1;
										if(x!=0)
										{
											if((x^f)!=0x0)
											{//�仯
												p8s[5]&=0xfe;
												p8s[5]|=f;
												p8=(u8 *)(ADDR_DATABUFF+2048);
												p8[0]=29;//ERC
												p8[1]=18;//����Le
												MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
												p8[7]=Pn;
												p8[8]=(Pn>>8)|(f<<7);
												MR(ADDR_DATABUFF+2048+9,((u32)p8d)+5+1,5);//���߷���ǰ�����й�����ʾֵ
												MR(ADDR_DATABUFF+2048+14,FnDataBuff+6+1,5);//���߷����������й�����ʾֵ
												MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59+1,1);//ȡ���ܱ������ֵ	���ݸ�ʽ22		l
												ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
											}
										}
									}
								}
								//���������¼
								x=p8s[6]&0x1;
								if(x==0)
								{
									if((x^Errf)!=0x0)
									{//�仯
										p8s[6]&=0xfe;
										p8s[6]|=Errf;
										p8=(u8 *)(ADDR_DATABUFF+2048);
										p8[0]=28;//ERC
										p8[1]=18;//����Le
										MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
										p8[7]=Pn;
										p8[8]=(Pn>>8)|(Errf<<7);
										MR(ADDR_DATABUFF+2048+9,((u32)p8d)+5+1,5);//�����ʱ��Ӧ�����й�����ʾֵ
										MR(ADDR_DATABUFF+2048+14,FnDataBuff+6+1,5);//�����ʱ�����й�����ʾֵ
										MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59,1);//ȡ���ܱ�����ֵ	���ݸ�ʽ22		l
										ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
									}
								}
								//���߲�����¼
								x=p8s[5]&0x1;
								if(x==0)
								{
									if((x^f)!=0x0)
									{//�仯
										p8s[5]&=0xfe;
										p8s[5]|=f;
										p8=(u8 *)(ADDR_DATABUFF+2048);
										p8[0]=29;//ERC
										p8[1]=18;//����Le
										MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
										p8[7]=Pn;
										p8[8]=(Pn>>8)|(f<<7);
										MR(ADDR_DATABUFF+2048+9,((u32)p8d)+5+1,5);//���߷���ǰ�����й�����ʾֵ
										MR(ADDR_DATABUFF+2048+14,FnDataBuff+6+1,5);//���߷����������й�����ʾֵ
										MR(ADDR_DATABUFF+2048+19,ADDR_AFN04F59+1,1);//ȡ���ܱ������ֵ	���ݸ�ʽ22		l
										ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
									}
								}
								
							}
						}
					}
				}
			}
		}
		MW(ADDR_TYMDHMS,ADDR_ERC29_SOUR+(LEN_ERC29_SOUR*(Pn-1)),5);//�����ϴγ���ʱ�����ʱ����
	}
	#endif
}
#endif

#if ERC30==1
void ERC30_Event(u32 Pn)//���ܱ�ͣ���¼�,���:Pn=�������(1��ʼ)
{
	u32 i;
//	u32 x;
//	u32 Ib;
	u32 f;
	u32 t;//ͣ�߶�ʱ������
	u8 * p8;
	u8 * p8s;
	u8 * p8d;
	u16 * p16;
	double f64;
	
	p16=(u16 *)(ADDR_ERC30_Timer);
	if((Pn<=ERC30MaxPn)&&(Pn!=0))
	{
		p8s=(u8 *)(ADDR_ERC30_SOUR+(LEN_ERC30_SOUR*(Pn-1)));
		if(ERC_Event_Set(30)==0)//����¼���¼����,����0=����¼,1=��¼
		{
			p8s[0]=0xee;
			p8s[5]=0;
			return;
		}
		t=MRR(ADDR_AFN04F59+2,1);//���ܱ�ͣ����ֵ	BIN	15min	l
		if(t==0x0)
		{//ͣ����ֵ=0
			p8s[0]=0xee;
			p8s[5]=0;
			return;
		}
		t*=(15*60);
		if(t>0xffff)
		{
			t=0xffff;
		}
		i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
		i+=6;
		p8d=(u8 *)(i);
		if(p8s[0]==0xee)
		{//�޿�ʼֵ
			MR((u32)p8s,(u32)p8d,5);
			p16[Pn-1]=t;
		}
		else
		{//�п�ʼֵ
			if(p8d[0]!=0xee)
			{//��ǰ��ֵ
				f=DataComp((u32)p8s,(u32)p8d,5);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				if(f!=0x0)
				{//��ͣ��
					f=0;//�ָ�
					MR((u32)p8s,(u32)p8d,5);
					p16[Pn-1]=t;//���¿�ʼ��ʱ
				}
				else
				{
					//���ʼ��㶨ʱ��Ӧ�ߵ���>=0.04
					i=GetClass1DataAddr(25,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
					if(i==0)
					{
						return;
					}
					i=MRR(i+5,3);//ȡ���й�����xx.xxxx KW
					if(i!=0xeeeeee)
					{//���й�������Ч
						if(i&0x800000)
						{//����Ϊ��,����
							i=0;
						}
						i=bcd_hex(i);
						f64=i;
						f64*=t;
						f64/=3600;//kws��Ϊkwh,4λС��
						i=(u32)f64;//t=Ӧ�ߵ���kwh,4λС��
						if(i<400)
						{//Ӧ�ߵ���kwh<0.0400ʱ���Ƚ�
							p16[Pn-1]=t;//���¿�ʼ��ʱ
							return;
						}
					}
/*
					Ib=MRR(ADDR_AFN04F25+(11*Pn)+6,1);//ȡ�����(BCD x.x)
					if(Ib==0)
					{
						Ib=~Ib;
					}
					x=GetClass1DataAddr(25,Pn+1);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�Terminal_ClassDataLen_ADDR
					if(x!=0)
					{
						x+=5+24+14+1;
						for(i=0;i<3;i++)
						{
							t=MRR(x+(3*i),2);//xxx.xxx
							if(t!=0xeeee)
							{
								t&=0x7fff;//ȥ��������λ
								if(t>=Ib)
								{
									f=1;//����
									break;
								}
							}
						}
					}
*/
					if(p16[Pn-1]==0x0)
					{//ͣ����ֵ��ʱ��
						f=1;//����
						p16[Pn-1]=t;//���¿�ʼ��ʱ
					}
					else
					{
						return;
					}
				}
				t=MRR(ADDR_AFN04F59+2,1);//���ܱ�ͣ����ֵ	BIN	15min	
				if((p8s[5]^f)!=0x0)
				{//ͣ�߱仯
					p8s[5]=f;
					p8=(u8 *)(ADDR_DATABUFF);
					p8[0]=30;//ERC
					p8[1]=13;//����Le
					MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
					p8[7]=Pn;
					p8[8]=(Pn>>8)|(f<<7);
					MR(ADDR_DATABUFF+9,(u32)p8s,5);
					p8[14]=t;
					ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				}
			}
		}
	}
}
#endif


#if ERC31==1
void ERC31_Event(u32 MeterNo,u32 Pn,u32 f)//�ն�485����ʧ���¼�,��ڲ������,f=1����=0�ָ�
{
	u8 * p8;
	
	if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	MeterNo--;
	p8=(u8 *)(ADDR_RM_CountFlags+32+(MeterNo/4));
	MeterNo%=4;
	MeterNo<<=1;
	if(f==1)
	{//f=1����
		if(((p8[0]>>MeterNo)&0x3)!=2)
		{
			return;
		}
#if (USER/100)==11//����ϵ
#else
		p8=(u8 *)(ADDR_AFN0CF129+((6+(5*(1+TMaxTariff)))*(Pn-1)));
		if(p8[6]==0xee)
		{//�����һ�γ���ɹ������й��ܵ���ʾֵ	
			return;
		}
#endif
	}
	else
	{//�ָ�
		if(((p8[0]>>MeterNo)&0x3)!=1)
		{
			return;
		}
	}

//ERC31����ʧ���¼�
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=31;//ERC
	p8[1]=21;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8)|(f<<7);
	Pn--;
	MR(ADDR_DATABUFF+9,ADDR_AFN0CF129+((6+(5*(1+TMaxTariff)))*Pn),5);//���һ�γ���ɹ�ʱ�䣺��ʱ������
	MR(ADDR_DATABUFF+14,ADDR_AFN0CF129+((6+(5*(1+TMaxTariff)))*Pn)+6,5);//���һ�γ���ɹ������й��ܵ���ʾֵ	
	MR(ADDR_DATABUFF+19,ADDR_AFN0CF130+((6+(4*(1+TMaxTariff)))*Pn)+6,4);//���һ�γ���ɹ������޹��ܵ���ʾֵ
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC32==1
void ERC32_Event(u32 n1,u32 n2)//ͨ�������������¼�,n1=�����ѷ�����ͨ������,n2=��ͨ����������
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=32;//ERC
	p8[1]=13;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	MWR(n1,ADDR_DATABUFF+7,4);//�ִ浽�Ƕ����ֽ�,������ݵ�ַ,�����ֽ���
	MWR(n2,ADDR_DATABUFF+11,4);//�ִ浽�Ƕ����ֽ�,������ݵ�ַ,�����ֽ���
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC33==1
void ERC33_Event(u32 Pn,u32 FnDataBuff)//���ܱ�����״̬�ֱ�λ�¼�(����1���ݴ���ǰд�¼�),���Pn=�������(1��ʼ),Fn�������ݻ����ַ
{
	u8 * p8;
	
 #if (USER/100)==0//���Ժ�ͼ��
	if(Check_ACSamplePn(Pn)==1)//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
	{
		return;//���ɲ����㲻��¼
	}
 #endif
	p8=(u8 *)(ADDR_DATABUFF+2048);//����1���ݴ���ǰд���¼�,������DATABUFF����,�򽻲�����Ҳ����DATABUFF����
	p8[0]=33;//ERC
	p8[1]=35;//����Le
	MR(ADDR_DATABUFF+2048+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8);
	MR(ADDR_DATABUFF+2048+9,FnDataBuff+6,28);
	ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

#if ERC34==1

#endif

#if ERC35==1
void ERC35_Event(u32 Port,u32 n,u32 DataAddr)//����δ֪����¼�;���Port=�˿ں�,n=δ֪�����,DataAddr=δ֪���ͨ�ŵ�ַ��ÿ��8�ֽ����ݴ�ŵĵ�ַ
{
	u32 i;
	u8 * p8;

	while(n!=0x0)
	{
		i=((LEN_TERMINAL_EVENT)-9)/8; //64 ���ն��¼���¼�̶����ȼ����¼�ܴ��δ֪�����
		if(n<i)
		{
			i=n;
		}
		p8=(u8 *)(ADDR_DATABUFF);
		p8[0]=35;//ERC
		p8[1]=7+(i*8);//����Le
		MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
		p8[7]=Port;//�˿ں�
		p8[8]=i;//�����
		MR(ADDR_DATABUFF+9,DataAddr,i*8);
		ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
		n-=i;
		DataAddr+=(i*8);
	}
}
#endif

#if ERC36==1
void ERC36_Event(u32 CD,u32 ST)//���������·���ؽ���״̬����λ��¼;��ڣ�CD=��λ,ST=״̬
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=36;//ERC
	p8[1]=7;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=CD;//״̬��λ��־
	p8[8]=ST;//��λ��״̬
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼	
}
#endif

#if ERC37==1
void ERC37_Event(u32 Pn,u32 ADDR_DATA)//���ܱ�����¼���¼;���:Pn�������(��1��ʼ)
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=37;//ERC
	p8[1]=70;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8);
	MR(ADDR_DATABUFF+9,ADDR_DATA,63);
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼	
}
#endif

#if ERC38==1
void ERC38_Event(u32 Pn,u32 ADDR_DATA)//���ܱ���ť���¼���¼;���:Pn�������(��1��ʼ)
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=38;//ERC
	p8[1]=70;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=(Pn>>8);
	MR(ADDR_DATABUFF+9,ADDR_DATA,63);
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼	
}
#endif

#if ERC39==1
/*
void ERC39_Event(u32 Fn,u32 Pn,u32 f)//����ʧ���¼���¼;���:��ǰ������FnPn,f=0����ɹ�,f=1����ʧ��
{
	u32 i;
	u8 * p8;
	
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=39;//ERC
	p8[1]=21;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=Pn>>8;
	p8[8]|=(f<<7);//�����ָ���־
	MC(0xee,ADDR_DATABUFF+9,14);
	switch(Fn)
	{
		case 129://F129 ��ǰ�����й�����ʾֵ
		//���һ�γ���ɹ�ʱ�䣺��ʱ������	����¼A.15	5
		//���һ�γ���ɹ������й��ܵ���ʾֵ	����¼A.14	5
			i=GetClass1DataAddr(129,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				MR(ADDR_DATABUFF+9,i,5);
				MR(ADDR_DATABUFF+14,i+6,5);
			}
		//���һ�γ���ɹ������޹��ܵ���ʾֵ	����¼A.11	4
			i=GetClass1DataAddr(130,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				MR(ADDR_DATABUFF+19,i+6,4);
			}
			break;
		//case 162://F162 ���ܱ�����ʱ��
		case 0xe3://��չ3 ���г����ն��������й�����ʾֵ���ܡ�����1��M��
		//���һ�γ���ɹ�ʱ�䣺��ʱ������	����¼A.15	5
		//���һ�γ���ɹ������й��ܵ���ʾֵ	����¼A.14	5
		//���һ�γ���ɹ������޹��ܵ���ʾֵ	����¼A.11	4
			return;
		default:
			return;
	}
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
*/
void ERC39_Event(u32 Pn)//����ʧ���¼���¼
{
	u32 i;
	u32 x;
	u32 y;
	u32 YMD;
	u8 *p8;
	u32 OfficeAddr;
	
	i=MRR(ADDR_TCongealCount+14,2);
	if(i<4)
	{
		return;//�ն���������4��
	}
	p8=(u8*)(ADDR_ERC39_SOUR);//1byte ERC39����ʧ���¼���¼��־:0=��(�ϵ���ո���),1=ʧ�ܼ�¼,2=�ɹ���¼
	if(p8[Pn-1]==1)
	{
		return;
	}
	if(ERC_Event_Set(39)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8[Pn-1]=0;
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(161,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	//�����3���Ƿ��г���
	YMD=MRR(ADDR_TYMDHMS+3,3);
	YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
	for(i=0;i<3;i++)
	{
		YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
		x=GetDateCongealNo(YMD);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
		if(x!=0xffffffff)
		{//�ҵ�
			x=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,1);
			if((x!=0xff)&&(x!=0xee))
			{
				return;//����3�����г���
			}
		}
	}
	//������ǳɹ�����
	for(i=0;i<MaxDateCongeal;i++)
	{
		YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
		x=GetDateCongealNo(YMD);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
		if(x!=0xffffffff)
		{//�ҵ�
			y=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,1);
			if((y!=0xff)&&(y!=0xee))
			{
				break;
			}
		}
	}
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=39;//ERC
	p8[1]=21;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	p8[7]=Pn;
	p8[8]=Pn>>8;
	p8[8]|=(1<<7);//�����ָ���־
	MC(0xee,ADDR_DATABUFF+9,14);
	if(i<MaxDateCongeal)
	{
		MR(ADDR_DATABUFF+9,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr,5);//���һ�γ���ɹ�ʱ�䣺��ʱ������	����¼A.15	5
		Data_0xFFto0xEE(ADDR_DATABUFF+9,5);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
		MR(ADDR_DATABUFF+14,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,5);//���һ�γ���ɹ������й��ܵ���ʾֵ	����¼A.14	5
		Data_0xFFto0xEE(ADDR_DATABUFF+14,5);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
		OfficeAddr=GetClass2DataOfficeAddr(162,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
		MR(ADDR_DATABUFF+19,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,4);//���һ�γ���ɹ������޹��ܵ���ʾֵ	����¼A.11	4
		Data_0xFFto0xEE(ADDR_DATABUFF+19,4);//�Ƚ�����:ȫ0xFF���滻Ϊȫ0xEE;����0=����ȫ0xFF,1=��ȫ0xFF
	}
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	p8=(u8*)(ADDR_ERC39_SOUR);//1byte ERC39����ʧ���¼���¼��־:0=��(�ϵ���ո���),1=ʧ�ܼ�¼,2=�ɹ���¼
	p8[Pn-1]=1;
}

void ERC39_Event_Cancel(u32 Pn)//����ʧ���¼��ָ�
{
	u32 i;
	u32 x;
	u32 y;
	u32 YMD;
	u8 *p8;
	u32 OfficeAddr;

	p8=(u8*)(ADDR_ERC39_SOUR);//1byte ERC39����ʧ���¼���¼��־:0=��(�ϵ���ո���),1=ʧ�ܼ�¼,2=�ɹ���¼
	if(p8[Pn-1]!=1)
	{
		return;
	}
	if(ERC_Event_Set(39)==0)//����¼���¼����,����0=����¼,1=��¼
	{
		p8[Pn-1]=0;
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(161,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
	//�����3���Ƿ��г���
	YMD=MRR(ADDR_TYMDHMS+3,3);
	YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
	for(i=0;i<3;i++)
	{
		YMD=YMD_Sub1D(YMD);//�����ռ�1��,���ؼ�1���������
		x=GetDateCongealNo(YMD);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
		if(x!=0xffffffff)
		{//�ҵ�
			y=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,1);
			if((y!=0xff)&&(y!=0xee))
			{
				//����3�����г���
				p8[Pn-1]=2;//2=�ɹ�
				p8=(u8 *)(ADDR_DATABUFF);
				p8[0]=39;//ERC
				p8[1]=21;//����Le
				MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
				p8[7]=Pn;
				p8[8]=Pn>>8;
				MR(ADDR_DATABUFF+9,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr,5);//���һ�γ���ɹ�ʱ�䣺��ʱ������	����¼A.15	5
				MR(ADDR_DATABUFF+14,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*x)+OfficeAddr+6,5);//���һ�γ���ɹ������й��ܵ���ʾֵ	����¼A.14	5
				MC(0xee,ADDR_DATABUFF+19,4);//���һ�γ���ɹ������޹��ܵ���ʾֵ	����¼A.11	4
				ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
				return;
			}
		}
	}
}
#endif

#if ERC40==1
void ERC40_Meter_Event(u32 FnDataAddr,u32 Pn)//ERC40ֱ�Ӷ����ܱ�ų��쳣�¼���¼;���:Fn�����׵�ַ,�������(1��ʼ)
{
	u8 *p8;
	if(Pn<=ERC40MaxPn)
	{
		Pn--;
		p8=(u8 *)(ADDR_DATABUFF+2048);
		p8[0]=40;//ERC
		p8[1]=15;//����Le
		MR(ADDR_DATABUFF+2048+2,FnDataAddr+15,5);//��ʱ������
		Pn++;
		p8[7]=Pn;
		p8[8]=(Pn>>8)|(1<<7);
		p8[9]=2;//�豸���ͣ�2=���ܱ�
		MR(ADDR_DATABUFF+2048+10,FnDataAddr+6,6);//�豸��ַ
		p8[16]=2;//�ų��쳣����:2=ǿ�ų�
		ERC_Event_Save(ADDR_DATABUFF+2048);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
	}
}
#endif

#if ERC41==1
void ERC41_Event(u32 ADDR_BUFF)//��ʱ�¼���¼;���:ADDR_BUFF�ĵ�2�ֽڿ�ʼ�ֱ�Ϊ2BYTE�������,6BYTE��ʱǰʱ��,6BYTE��ʱ��ʱ��
{
	u8 * p8;
	p8=(u8 *)(ADDR_BUFF);
	p8[0]=41;//ERC
	p8[1]=14;//����Le
	ERC_Event_Save(ADDR_BUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#if (USER/100)==5//�Ϻ�
void ERC41_Meter_Event(u32 FnDataAddr,u32 Pn)//ERC41ֱ�Ӷ����ܱ��ʱ�¼���¼;���:Fn�����׵�ַ,�������(1��ʼ)
{
	u32 x;
	u32 y;

	if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	//��ÿ��15:00�ĳ������ڽ�����ȶ�Уʱ�������ɼ�¼
	x=MRR(ADDR_TYMDHMS,3);//��ǰʱ����
	if(x<0x150000)
	{
		return;
	}
	x=MRR(ADDR_TYMDHMS+3,2);//��ǰ����
	y=MRR(ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*(Pn-1)),2);//ԭ�ȶ�����
	if(x==y)
	{
		return;
	}
	MWR(x,ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*(Pn-1)),2);
	
	y=MRR(FnDataAddr+6,3);//��ǰ���ܱ�ʱ���޸Ĵ���
	if(y!=0xeeeeee)
	{
		x=MRR(ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*(Pn-1))+2,3);//ԭ���ܱ�ʱ���޸Ĵ���
		MWR(y,ADDR_ERC41_SOUR+(LEN_ERC41_SOUR*(Pn-1))+2,3);//����ԭ���ܱ�ʱ���޸Ĵ���
		if(x!=0xeeeeee)
		{
			if(y!=0)
			{//��ǰ���ܱ�ʱ���޸Ĵ���!=0
				if(x!=y)
				{//�����仯
					MWR(Pn,ADDR_DATABUFF+2,2);
					MR(ADDR_DATABUFF+4,FnDataAddr+6+3,12);
					ERC41_Event(ADDR_DATABUFF);//��ʱ�¼���¼;���:ADDR_BUFF�ĵ�2�ֽڿ�ʼ�ֱ�Ϊ2BYTE�������,6BYTE��ʱǰʱ��,6BYTE��ʱ��ʱ��
				}
			}
		}
	}
}
#else//#if (USER/100)==5//�Ϻ�
void ERC41_Meter_Event(u32 FnDataAddr,u32 Pn)//ERC41ֱ�Ӷ����ܱ��ʱ�¼���¼;���:Fn�����׵�ַ,�������(1��ʼ)
{
	u32 i;
	u32 x;
	u32 y;

	i=GetClass1DataAddr(166,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(i!=0)
	{
		x=MRR(i+5,3);//ԭ���ܱ�ʱ���޸Ĵ���
		if(x!=0xeeeeee)
		{
			y=MRR(FnDataAddr+6,3);//��ǰ���ܱ�ʱ���޸Ĵ���
			if(y!=0xeeeeee)
			{
				if(y!=0)
				{//��ǰ���ܱ�ʱ���޸Ĵ���!=0
					if(x!=y)
					{//�����仯
						MWR(Pn,ADDR_DATABUFF+2,2);
						MR(ADDR_DATABUFF+4,FnDataAddr+6+3,12);
						ERC41_Event(ADDR_DATABUFF);//��ʱ�¼���¼;���:ADDR_BUFF�ĵ�2�ֽڿ�ʼ�ֱ�Ϊ2BYTE�������,6BYTE��ʱǰʱ��,6BYTE��ʱ��ʱ��
					}
				}
			}
		}
	}
}
#endif
#endif


#if ERC45==1
void ERC45_Event(void)//�ն˵��ʧЧ�¼�
{
	u8 * p8;
	p8=(u8 *)(ADDR_DATABUFF);
	p8[0]=45;//ERC
	p8[1]=5;//����Le
	MR(ADDR_DATABUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	ERC_Event_Save(ADDR_DATABUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif

void PerERC_Event_Save(u32 BuffAddr)//����ERC�¼���¼
{
	u32 i;
	u32 x;
	u8 * p8s;
	
	p8s=(u8 *)(BuffAddr);
	i=p8s[0];
	i-=1;
	if(i>(MaxERC-1))
	{
		return;
	}
	MR(ADDR_DATABUFF_ERAM,ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT))),2);
	x=MRR(ADDR_DATABUFF_ERAM,2);
	if(x<MaxClassEventNum)
	{
		MR(ADDR_DATABUFF_ERAM+2,ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)))+2,MaxClassEventNum*LEN_TERMINAL_EVENT);
	}
	else
	{
		MR(ADDR_DATABUFF_ERAM+2,ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)))+2+LEN_TERMINAL_EVENT,(MaxClassEventNum-1)*LEN_TERMINAL_EVENT);
	}
	x++;
	if(x>MaxClassEventNum)
	{
		x=MaxClassEventNum;
	}
	MWR(x,ADDR_DATABUFF_ERAM,2);
	x--;
	MR(ADDR_DATABUFF_ERAM+2+(LEN_TERMINAL_EVENT*x),BuffAddr,LEN_TERMINAL_EVENT);
	MW(ADDR_DATABUFF_ERAM,ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT))),2+(MaxClassEventNum*LEN_TERMINAL_EVENT));
}

#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
void ERC51_Event(u32 Pn,u32 ADDR_BUFF)//Уʱ��¼�¼�;���:Pn�������(��0��ʼ)
{
	u8 * p8;
	p8=(u8 *)(ADDR_BUFF);
	p8[0]=51;//ERC
	p8[1]=12;//����Le
	MR(ADDR_BUFF+2,ADDR_TYMDHMS+1,5);//��ʱ������
	MR(ADDR_AFN04F299_Ext+28+1+4*25+4+1,ADDR_TYMDHMS,6);
	p8[7]=Pn;
	p8[8]=(Pn>>8);
	ERC_Event_Save(ADDR_BUFF);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
}
#endif


