#include "../Hcsg/ProjectCSG.h"
#include "../Hcsg/IRAMCSG.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
//#include "../I2_terminal/Terminal_SwitchIn.h"
//#include "../I2_terminal/Terminal_Contrl_Out.h"
#include "../Display/Display.h"
#include "../Calculate/Calculate.h"




u32 DisplayLoop_TopICO(u32 Row)//��ͼ���ʱ����ʾ
{
	u32 i,j,k,x,y,m,n;
	u8 *p8;
  u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
#if (Project/100)==3 ||(Project/100)==9    //��������
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
#endif

	
	//�����źź�����
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(0*2));
	p16[0]=0;
	p16[1]=0;
	p16[2]=0;
	p16[3]=0;
	p16[4]=0;
	i=Terminal_Ram->Signal;
	if(i!=0)
	{
		p16[0]=2;//'����'
		switch(i)
		{
			case 1:
				p16[1]=3;//'�ź�ǿ��1'
				break;
			case 2:
				p16[1]=4;//'�ź�ǿ��2'
				break;
			case 3:
				p16[1]=4;//'�ź�ǿ��2'
				p16[2]=5;//'�ź�ǿ��3'
				break;
			case 4:
				p16[1]=4;//'�ź�ǿ��2'
				p16[2]=6;//'�ź�ǿ��4'
				break;
		}
	}
	if((UART1Ctrl->LinkTask==101)||(UART1Ctrl->LinkTask==102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
	{
		if(Terminal_Ram->MODULSEL==1)//����ģ��ѡ��0=,1=��ΪCDMA MC323,2=GPRS��׼3763,3=ģ���û����
		{
			p16[3]=9;//'C'
			p16[4]=10;
		}
		else
		{
//			if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
//			{
//				p16[3]=7;//'G'
//				p16[4]=8;
//			}
			if(Comm_Ram->RemoteWirelessModuleType == (1 << 0))//2G GSM/GPRSģʽ
			{
				p16[3]=65;//�п�2G
				p16[4]=66;
			}
			else if(Comm_Ram->RemoteWirelessModuleType == (1 << 1))//3G WCDMAģʽ
			{
				p16[3]=67;//�п�3G
				p16[4]=68;
			}
			else if(Comm_Ram->RemoteWirelessModuleType == (1 << 5))//4G LTEģʽ
			{
				p16[3]=69;//�п�4G
				p16[4]=70;
			}
			else
			{
				p16[3]=9;//'C'
				p16[4]=10;
			}
		}
	}
	
//��̫������
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if((UARTCtrl->LinkTask==101)||(UARTCtrl->LinkTask==102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
	{
		p16[3]=11;//11 'L'
		p16[4]=12;
	}



//�쳣�澯ָʾ����ʾ�ն˻���������쳣��������ն˷����쳣ʱ���ñ�־����1Hz��Ƶ����˸��ʾ
	p8=(u8 *)(ADDR_MYMDHMS);
	y=TerminalGetLastAlarmDI();
	k=(y>>24)&0x000000FF;               //E2
	j=y&0x00FFFF00;                     //0000
	m=(y>>4)&0x0000000F;
	n=y&0x0000000F;
	if((k==0xE2)&&(j==0)&&(m<=4))
	{		
		if((p8[0]%2)==1)
		{	
			p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(5*2));
			p16[0]=17;// '�澯'
			p16[1]=18;
		}
		if((p8[0]%2)==0)
		{
			p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(5*2));
			p16[0]=0x2030 + m;
			if(n==0x0A)
			{
				p16[1]=0x2000 + 'A';
			}
			else if(n==0x0B)
			{
				p16[1]=0x2000 + 'B';
			}
			else if(n==0x0C)
			{
				p16[1]=0x2000 + 'C';
			}
			else if(n==0x0D)
			{
				p16[1]=0x2000 + 'D';
			}
			else if(n==0x0E)
			{
				p16[1]=0x2000 + 'E';
			}
			else if(n==0x0F)
			{
				p16[1]=0x2000 + 'F';
			}
			else
			{
				p16[1]=0x2030 + n;
			}	
		}
	}


//���Ի��ѯ��ʾ�Ĳ������	
//Ĭ����ʾ�������0
#if (Project/100)==3 || (Project/100)==9      //��������
	p16=(u16 *)(ADDR_STRINGBUFF+(0*84)+4+(8*2));
	i = hex_bcd(Pn_Lcd_Data->u16Pn);
	if(Pn_Lcd_Data->u16Pn==0xFFFF)              //�����㲻����ʱ���������Ϊ0xFFFF
	{
		p16[0]=0x2030; 
		p16[1]=0x2030;
		p16[2]=0x2030; 
		p16[3]=0x2030;
	}
	else
	{
		p16[0]=0x2000 + (((i >> 12) & 0x000F) + 0x30);
		p16[1]=0x2000 + (((i >> 8) & 0x000F) + 0x30);
		p16[2]=0x2000 + (((i >> 4) & 0x000F) + 0x30);
		p16[3]=0x2000 + ((i & 0x000F) + 0x30);
	}
#endif	


//�ɳ����
	x=Comm_Ram->UBACKUP;
	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		x=bcd_hex(x);
		if(x>=20)
		{
			x-=20;//�����������ڳ��-0.30V
		}
		else
		{
			x=0;
		}
		x=hex_bcd(x);
	}
	i=38;//38'���0��ͼ'
	if(x>=RechargeableBatteryGrid1)
	{
	  i=40;//40'���1��ͼ'
	}
	if(x>=RechargeableBatteryGrid2)
	{
	  i=42;//42'���2��ͼ'
	}
	if(x>=RechargeableBatteryGrid3)
	{
	  i=44;//44'���3��ͼ'
	}
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(12*2));
	p16[0]=i;
	p16[1]=i+1;

	
//��ǰʱ��
	p8=(u8 *)(ADDR_RTCBUFF);
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(15*2));
	p16[0]=((p8[2]>>4)&0xf)+0x30+0x2000;
	p16[1]=((p8[2]>>0)&0xf)+0x30+0x2000;
	p16[2]=':'+0x2000;
	p16[3]=((p8[1]>>4)&0xf)+0x30+0x2000;
	p16[4]=((p8[1]>>0)&0xf)+0x30+0x2000;
//	p16[5]=':'+0x2000;
//	p16[6]=((p8[0]>>4)&0xf)+0x30+0x2000;
//	p16[7]=((p8[0]>>0)&0xf)+0x30+0x2000;
//
	Row++;
	return Row;
}





u32 DisplayLoop_BottomICO(u32 Row)//��ͼ����ʾ
{
  u16 *p16;
	#if (Project/100)==3 || (Project/100)==9
		u64 Databuff = 0;
		QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485I = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_I;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485II = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_II;
		QCSG_RDMETER_CTRL_S* pstRDMeter_Ctrl_RS485III = (QCSG_RDMETER_CTRL_S*)QCSG_RDMETER_CTRL_RS485_III;
	#endif
	
	#if (Project/100)==3 || (Project/100)==9
	if((pstRDMeter_Ctrl_RS485I->u8Rdm_Sem != 0)||(pstRDMeter_Ctrl_RS485II->u8Rdm_Sem != 0)||(pstRDMeter_Ctrl_RS485III->u8Rdm_Sem != 0))
	{
		DisplayString(Row,0,0,(u8 *)"�ն����ڳ���..");//��ʾ�ַ���
	}
	else
	{
		DisplayString(Row,0,0,(u8 *)"              ");//��ʾ�ַ���
	}
	#endif	

	
	#if (Project/100)==2
		MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
		TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
		if((MeterFileInfo->TotalMeters > 0)&&((pTSysCtrl->Flag & TERMINAL_READ_METER_ON_FLAG) != 0))
		{
			DisplayString(Row,0,0,(u8 *)"�ն����ڳ���..");//��ʾ�ַ���
		}
		else
		{
			DisplayString(Row,0,0,(u8 *)"              ");//��ʾ�ַ���
		}
	#endif	




//���ü�
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(12*2));
#if KEY_SET==1//���ð���0=û,1=��
	#if ((Project/100)<5) || ((Project/100)==9)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
	if(MRR(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
	#else
	if(MRR(ADDR_TerminalFMTimer+12,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
	#endif
#endif
//����ģʽ(���������ü�)
	if(Comm_Ram->Factory==0x55)//����״̬
	{
		p16[0]=25;// '����ģʽ'
		p16[1]=26;
	}
//ʱ�ӵ��Ƿѹ
	if(Comm_Ram->URTC<0x200)
	{//С��2.00V
		if(Comm_Ram->RTCBuff[0]&1)
		{
	  		p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(14*2));
			p16[0]=21;//'���Ƿѹ'
			p16[1]=22;
		}
	}


//ʧѹ��־
#if (Project/100)==3 || (Project/100)==9
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(16*2));          //ʧѹ��־
	Pn_Lcd_Data->u16Res1 |= LCD_RT_BOTTOM_ICO;									
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Terminal040005FF[6],6);	
	if(Databuff == 0xFFFFFFFFFFFF )
	{
		p16[0]=71;//��ʧѹ
		p16[1]=72;
	}
	else
	{
		Databuff &= 0x100010001;
		if(Databuff == 0x000000001)
		{//A��ʧѹ
			p16[0]=77;
			p16[1]=78;
		}
		if(Databuff == 0x000010000)
		{//B��ʧѹ
			p16[0]=75;
			p16[1]=76;
		}
		if(Databuff == 0x100000000)
		{//C��ʧѹ
			p16[0]=73;
			p16[1]=74;
		}
		if(Databuff == 0x000010001)
		{//AB��ʧѹ
			p16[0]=83;
			p16[1]=84;
		}
		if(Databuff == 0x100010000)
		{//BC��ʧѹ
			p16[0]=79;
			p16[1]=80;
		}
		if(Databuff == 0x100000001)
		{//AC��ʧѹ
			p16[0]=81;
			p16[1]=82;
		}
		if(Databuff == 0x00)
		{//��ʧѹ
			p16[0]=71;
			p16[1]=72;
		}
		if(Databuff == 0x100010001)
		{//ȫʧѹ
			p16[0]=85;
			p16[1]=86;
		}
	}
	
	
	//ʧ����־
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(18*2));          //ʧ����־
	Pn_Lcd_Data->u16Res1 |= LCD_RT_BOTTOM_ICO;									
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Terminal040005FF[6],6);
	if(Databuff == 0xFFFFFFFFFFFF )
	{
		p16[0]=87;//��ʧ��
		p16[1]=88;
	}
	else
	{	
		Databuff &= 0x800080008;
		if(Databuff == 0x000000008)
		{//A��ʧ��
			p16[0]=93;
			p16[1]=94;
		}
		if(Databuff == 0x000080000)
		{//B��ʧ��
			p16[0]=89;
			p16[1]=90;
		}
		if(Databuff == 0x800000000)
		{//C��ʧ��
			p16[0]=91;
			p16[1]=92;
		}
		if(Databuff == 0x000080008)
		{//AB��ʧ��
			p16[0]=99;
			p16[1]=100;
		}
		if(Databuff == 0x800080000)
		{//BC��ʧ��
			p16[0]=95;
			p16[1]=96;
		}
		if(Databuff == 0x800000008)
		{//AC��ʧ��
			p16[0]=97;
			p16[1]=98;
		}
		if(Databuff == 0x00)
		{//��ʧ��
			p16[0]=87;
			p16[1]=88;
		}
		if(Databuff == 0x800080008)
		{//ȫʧ��
			p16[0]=101;
			p16[1]=102;
		}
	}
#endif	
	Row++;
	return Row;
}



