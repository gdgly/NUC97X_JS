
//�ն˳���DL645_2007������������Fn���ݻ���
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "Terminal_ReadMeter_HX_6E_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_CountAndFlag.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"

#include "../terminal/Terminal_USER2xx.h"
#include "Terminal_ReadMeter_HX_6E_Lib.h"
#include "Terminal_ReadMeter_Event.h"
#include "../terminal/airthmetic.h"


#if ((Project/100)==3)&&((USER/100)==12)//�Ĵ�ר��
//������������ַ����Ϊǰ�油3��000����д��ַ��
//�����ַ����9��������ǰ�油12-sizeof(����)��0
extern const u32 HX_6E_RMLib[];


void Terminal_ReadMeter_HX_6E_TxData(u32 PORTn,u8 *p8RMLib)//��������������
{
	u32 i ,j;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[0]= 0x02;
	p8txbuff[0+1]= 0x45;
	//ȡͨ�ŵ�ַ
	if(PORTn==ACSAMPLEPORT)
	{//���ڲ����ɿ�
		MC(0xaa,((u32)p8txbuff)+1+1,6);
	}
	else
	{
		MR(((u32)p8txbuff)+1+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	}
	if(ChangeAddress(p8txbuff,2) == 0) 
	{
		for(i=0;i<10;i++)
		{
			p8txbuff[i] = 0; //�����ƻ�һ�·��͵�����ʹ�õ����ͨѶ��
		}
		return;//��12�����ֽ���ת����ת����ʵ��ͨѶ�еĵ�ַ
	}
// Э���е�Դ��ַ�������Զ����ַ0x00000001
	p8txbuff[6] =0;
	p8txbuff[7] =0;
	p8txbuff[8] =0;
	p8txbuff[9] =0x01;
	
	p8txbuff[10] = 0;
	p8txbuff[11] = UARTCtrl->TxSEQ;
	if(p8txbuff[11] == 0xff)
	{
	  UARTCtrl->TxSEQ = 0;	 //���к��Լ�����
	}
	else 
	{
		UARTCtrl->TxSEQ =  p8txbuff[11] +1;
	}
	//ȡDI	
	i=(u32)p8RMLib;
	i+=10+(5*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+12,i,4);
	if((p8txbuff[13] == 0)&&(p8txbuff[14] == 0)) 
	{//��������
		j =27	;
	}
	else 
	{//��ȡ����
		j = 15;
	}
	if(j == 27)
	{
		p8txbuff[13] = 0x45;
		p8txbuff[14] = 0x44;
		p8txbuff[15] = 0x4d;
		p8txbuff[16] = 0x49;
		p8txbuff[17] = 0x2c;
		p8txbuff[18] = 0x49;
		p8txbuff[19] = 0x4d;
		p8txbuff[20] = 0x44;
		p8txbuff[21] = 0x45;
		p8txbuff[22] = 0x49;
		p8txbuff[23] = 0x4d;
		p8txbuff[24] = 0x44;
		p8txbuff[25] = 0x45;
		p8txbuff[26] = 0;
	}
	i = (u32)Crc_HX(p8txbuff,j);
	p8txbuff[j] = (u8)(i>>8);
	p8txbuff[j+1] = (u8)(i&0x000000ff);

	i=(u32)CheckHXTxData(p8txbuff,0,(u8)(j+2));	 //���﷢�Ϳ⺯����������дС�ˣ���Ҫ���Ƿ��͵�����û��ô��
	if(i>250)
	{
		return;
	}
	p8txbuff[i] = 0x03;
	UARTCtrl->TxByte=i+1;//TX����(�跢���ֽ���)
}

void ReadMeter_HX_6E_FnData(u32 PORTn,u8 *p8RMLib)//HX���������������Fn���ݻ���
{
	ReadMeter_HX_6E_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//HX���������������Fn���ݻ���
}

void ReadMeter_HX_6E_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//HX���������������Fn���ݻ���
{
	u32 i;
	u32 x;
	u32 week;
	u64 d64a;
	u64 d64b;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;
	union change{u8 d[4]; float f;}changedata;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

	i=p8RMLib[0];
	switch(i)
	{
		case 129://A14
			if(UARTCtrl->RMCount != 0)
			{
				goto DATAPROCESS1;	//��ֵҲ����ͨ�� ���Ǹ���������һ������ֱ��return
			}
			else
			{//=0�ǵ�������
				break;
			}
		case 130://A11
		case 131://A14
		case 132://A11
		case 133://A11
		case 134://A11
		case 135://A11	
		case 136://A11	
			goto DATAPROCESS1;	//A11    //��ֵҲ����ͨ�� ���Ǹ���������һ������ֱ��return
		case 145://A23  A17  
		case 146://A23  A17
		case 147://A23  A17
		case 148://A23  A17	
			goto DATAPROCESS2;//A23  A17//A23 ��ֵҲ����ͨ�� ���Ǹ���������һ������ֱ��return

		case 25:
			goto DATAPROCESS3;	//A9 A5 A7 A25		 //С������ֵ��Э��������ܻ���ָ�ֵ ��ֵҲ����ͨ��
		case 27:
			goto DATAPROCESS4;		     
		case 49:
			goto DATAPROCESS5;	//A5		 //С������ֵ��Э��������ܻ���ָ�ֵ ��ֵҲ����ͨ��
		case 162:	
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0:
					p8FnData[7] = _8HexToBCD(p8rxbuff[17]);//��
					p8FnData[8] = _8HexToBCD(p8rxbuff[16]);//��
					p8FnData[9] = _8HexToBCD(p8rxbuff[15]);//ʱ
					//��ȷ�ĳ���ʱ��,������ʱ����(����)
					if(UARTCtrl->TransferDelay==0)
					{//��·�ϴ�����ʱʱ����
						MW(ADDR_AFN0CF2,((u32)p8FnData)+1,6);//���볭��ʱ��
					}
					else
					{
						d64a=MRR(ADDR_TYMDHMS,6);
						d64a=YMDHMS_SubS(d64a,UARTCtrl->TransferDelay);//������ʱ�����S��,���ؼ�S����������ʱ����
						//���ڼ���
						MWR(d64a,((u32)p8rxbuff)+15,3);
						MWR(d64a>>24,((u32)p8rxbuff)+15+4,3);
						YMDCalculateW(((u32)p8rxbuff)+15);//ָ����ַ�����е���������ʱ���������
						i=p8rxbuff[15+3];
						if(i==0x0)
						{
							i=7;//��������=7
						}
						i<<=5;
						d64b=i;
						d64b<<=32;
						d64a|=d64b;
						MWR(d64a,((u32)p8FnData)+1,6);//���볭��ʱ��
					}
					break;
				case 1: 
					p8FnData[10] = _8HexToBCD(p8rxbuff[15]);//��
					p8FnData[11] = _8HexToBCD(p8rxbuff[16]);//��
					p8FnData[12] = _8HexToBCD(p8rxbuff[17]);//��
					week=YMDCalculateWeek((uint32_t)p8rxbuff+15);//ָ����ַ�����е������ռ�����
					if(week==0)
					{
						week=7;
					}
					week<<=5;
					p8rxbuff[16]|=week;
					p8FnData[11] = p8rxbuff[16];//����-��
					break;
			}
			break;

	DATAPROCESS1:		  // ���ڴ������ݷ���ֵĲ��ã�����д�Ĺ����вŷ��ֲ�����
							  // �Ȳ���ͨ��֮�������޸�������������·�һ��
		x=4;
		if(x>TMaxTariff)
		{
		  x=TMaxTariff;
		}
		p8FnData[6]=x;
		p8FnData+=7;
		p8rxbuff+=15;
		changedata.d[0] = p8rxbuff[3];
		changedata.d[1] = p8rxbuff[2];
		changedata.d[2] = p8rxbuff[1];
		changedata.d[3] = p8rxbuff[0];
		if(changedata.f<0)
		{
			return; //���ָ����쳣����
		}
		if(i==129)
		{
			 x = UARTCtrl->RMCount-1;//��1���ǵ�������洢
		}
		else
		{
			 x = UARTCtrl->RMCount;
		}
		if((i==129)||(i==131))
		{//�й�����
			if(	ChangeFloatTo376BCD(changedata.f,p8rxbuff,1,1) == 1)
			{//A14�ĸ�ʽ
				p8FnData[0+5*x]=p8rxbuff[0];	  
				p8FnData[1+5*x]=p8rxbuff[1];
				p8FnData[2+5*x]=p8rxbuff[2];
				p8FnData[3+5*x]=p8rxbuff[3];
				p8FnData[4+5*x]=p8rxbuff[4];
			}
		}
		if((i==130)||((i>131)&&(i<=136)))
		{//�޹�����
			if(	ChangeFloatTo376BCD(changedata.f,p8rxbuff,2,0) == 1)
			{//A11�ĸ�ʽ
				p8FnData[0+4*x]=p8rxbuff[0];	  
				p8FnData[1+4*x]=p8rxbuff[1];
				p8FnData[2+4*x]=p8rxbuff[2];
				p8FnData[3+4*x]=p8rxbuff[3];
			}
		}
	  break;
		
	DATAPROCESS2:
		x=4;
		if(x>TMaxTariff)
		{
			x=TMaxTariff;
		} 
		p8FnData[6] = x;
		p8FnData+= 7;
		p8rxbuff+= 15;
		x = UARTCtrl->RMCount;
		if((x%2)==0)
		{//�������
			changedata.d[0] = p8rxbuff[3];
			changedata.d[1] = p8rxbuff[2];
			changedata.d[2] = p8rxbuff[1];
			changedata.d[3] = p8rxbuff[0];
			if(changedata.f<0)
			{
				return; //���ָ����쳣����
			}
			if(	ChangeFloatTo376BCD(changedata.f,p8rxbuff,3,1) == 1)
			{//A23�ĸ�ʽ
				p8FnData[0+(x/2)*7]=p8rxbuff[0];	  
				p8FnData[1+(x/2)*7]=p8rxbuff[1];
				p8FnData[2+(x/2)*7]=p8rxbuff[2];
			}
		}
		else
		{//�����������ʱ��
			p8FnData[3+(x/2)*7]=_8HexToBCD(p8rxbuff[4]); 	//�� 	A17
			p8FnData[4+(x/2)*7]=_8HexToBCD(p8rxbuff[3]);	//ʱ	A17
			p8FnData[5+(x/2)*7]=_8HexToBCD(p8rxbuff[0]);	//��	A17
			p8FnData[6+(x/2)*7]=_8HexToBCD(p8rxbuff[1]);	//��	A17
		}
		break;	
		
	DATAPROCESS3:
		p8FnData+= 6;
		p8rxbuff+= 15;
		x = UARTCtrl->RMCount;
		changedata.d[0] = p8rxbuff[3];
		changedata.d[1] = p8rxbuff[2];
		changedata.d[2] = p8rxbuff[1];
		changedata.d[3] = p8rxbuff[0];
		if(x<=7)
		{//��ǰ�ܡ�A��B��C��/�޹�����
			if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,4,1) == 1)
			{//A9�ĸ�ʽ
				p8FnData[0+x*3]=p8rxbuff[0];	  
				p8FnData[1+x*3]=p8rxbuff[1];
				p8FnData[2+x*3]=p8rxbuff[2];
			}
		}
		if(x == 8)
		{//��ǰ��������
			if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,5,2) == 1)
			{//A5�ĸ�ʽ
				p8FnData[24]=p8rxbuff[0];	  
				p8FnData[25]=p8rxbuff[1];
			}
		}
		if((x>=9)&&(x<=11))	//�յ�3�������ABC����������	  2*3���ֽ�
		{//ABC���ѹ
			if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,6,1) == 1)
			{//A7�ĸ�ʽ
				p8FnData[32+(x-9)*2]=p8rxbuff[0];	  
				p8FnData[33+(x-9)*2]=p8rxbuff[1];
			}
		}
		if((x>=12)&&(x<=14))
		{//ABC�����
			if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,7,2) == 1)
			{//A25�ĸ�ʽ
				p8FnData[38+(x-12)*3]=p8rxbuff[0];	  
				p8FnData[39+(x-12)*3]=p8rxbuff[1];
				p8FnData[40+(x-12)*3]=p8rxbuff[2];
			}
		} 
    break;	
		
	DATAPROCESS4:
		p8FnData+= 6;
		p8rxbuff+= 15;
		x = UARTCtrl->RMCount;
		switch(x)
		{
			case 0:	
				p8FnData[0] = _8HexToBCD(p8rxbuff[2]);//��
				p8FnData[1] = _8HexToBCD(p8rxbuff[1]);//��
				p8FnData[2] = _8HexToBCD(p8rxbuff[0]);//ʱ
				break;
			case 1: 
				week=YMDCalculateWeek((uint32_t)p8rxbuff);//ָ����ַ�����е������ռ�����
				if(week==0)
				{
					week=7;
				}
				week<<=5;
				p8rxbuff[1]|=week;
				p8FnData[3] = p8rxbuff[0];//��
				p8FnData[4] = p8rxbuff[1];//��
				p8FnData[5] = p8rxbuff[2];//��				
				break;
			case 2:
				if(_32HexToBCD(p8rxbuff,p8FnData,19) == 1)	   //����������MK6��ָ�MK3������һ��ָ�����û�����޷���֤
					break;
			case 3:	
				p8FnData[22] = _8HexToBCD(p8rxbuff[5]);
				p8FnData[23] = _8HexToBCD(p8rxbuff[4]);
				p8FnData[24] = _8HexToBCD(p8rxbuff[3]);
				p8FnData[25] = _8HexToBCD(p8rxbuff[0]);
				p8FnData[26] = _8HexToBCD(p8rxbuff[1]);
				p8FnData[27] = _8HexToBCD(p8rxbuff[2]);
				break;
			default:
				break;
		}
		break;
	DATAPROCESS5:
		p8FnData+= 1;
		p8rxbuff+= 15;
		x = UARTCtrl->RMCount;
		changedata.d[0] = p8rxbuff[3];
		changedata.d[1] = p8rxbuff[2];
		changedata.d[2] = p8rxbuff[1];
		changedata.d[3] = p8rxbuff[0];
		if(ChangeFloatTo376BCD(changedata.f,p8rxbuff,5,1) == 1)
		{
			p8FnData[0+x*2]=p8rxbuff[0];	  //A5�ĸ�ʽ
			p8FnData[1+x*2]=p8rxbuff[1];
		}
		break;
	}
}
#endif
