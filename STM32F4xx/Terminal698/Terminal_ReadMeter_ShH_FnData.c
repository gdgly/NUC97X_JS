
//�ն˳����Ϻ����Լ������������Fn���ݻ���
#include "Project.h"
#include "Terminal_ReadMeter_ShH_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




#if (USER/100)==5//�Ϻ�
extern const u32 ShH_RMLib[];
void Terminal_ReadMeter_ShH_TxData(u32 PORTn,u8 *p8RMLib)//�ն��Ϻ����Լ����������
{
/*
	�Ϻ���Լ֡��ʽ��
	ǰ���ַ�	FE  //0
	֡��ʼ��	68H //1+0
	��ַ��	    A0  //1+1
				A1  //1+2
				A2  //1+3
				A3  //1+4	   ���쳧���룬1�ֽ�ASCII�룬��P����ó���֡��Ϊ50H����վ�·����ն�Ϊ80H(BCD��)
				A4  //1+5
				A5  //1+6
	֡��ʼ��	68H //1+7
	������		C   //1+8
	�����򳤶�	L   //1+9	   ʮ������
	������		DATA//1+10	   ������+33H����
	����У����	CS	//1+10+L
	֡������	16H	//1+11+L
*/
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[0]=0xFE;
	p8txbuff[1+0]=0x68;
	//ȡͨ�ŵ�ַ
	MR(((u32)p8txbuff)+1+1,ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
//	p8txbuff[1+4]=bcd_hex(p8txbuff[1+4]);//8λѹ��BCD��תΪHEX
	//�Ϻ���Լ���ͨ�ŵ�ַA5=0xff����
	p8txbuff[1+6]=0xff;

	p8txbuff[1+7]=0x68;
	//������
	i=(u32)p8RMLib;
	i+=10+(2*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+1+8,i,1);
	p8txbuff[1+8]|=0x80;

	p8txbuff[1+9]=0;
	//CS����
	p8txbuff[1+10]=0;
	for(i=0;i<10;i++)
	{
		p8txbuff[1+10]+=p8txbuff[1+i];
	}
	p8txbuff[1+11]=0x16;
	UARTCtrl->TxByte=1+12;//TX����(�跢���ֽ���)
}

void ReadMeter_ShH_FnData(u32 PORTn,u8 *p8RMLib)//�Ϻ���Լ���������������Fn���ݻ���
{
	ReadMeter_ShH_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//�Ϻ���Լ���������������Fn���ݻ���
}

void ReadMeter_ShH_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//�Ϻ���Լ���������������Fn���ݻ���
{
/*
	����Ϻ��������������з�����Ϊ2ʱ������������1��Ӧ�Ϻ������ƽ������������2��Ӧ�Ϻ�����ʹȣ�
	�����������з�����Ϊ4ʱ������������1��4�ֱ��Ӧ��Ч���ݡ��Ϻ������ƽ����Ч���ݡ��Ϻ�����ʹȡ�
	�˷���˳���һ��Ͷ������ݾ���Ҫ��
*/			
	u32 i;
	u32 x;
	u32 YM;
	u32 YM1;
	u64 d64a;
	u64 d64b;
	
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8FnData=(u8 *)Addr_FnDataBuff;

		
	//i=MRR(ShH_RMLib[UARTCtrl->FnCount],1);
	i=p8RMLib[0];
	switch(i)
	{
//3 F27 ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
		case 27:
			p8FnData+=6;
			p8rxbuff+=10;
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�����ʱ��YYMMDDWWHhmmss
					d64a=MRR(((u32)p8rxbuff),6);
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData),6);
					break;
				//��ع���ʱ�� 4�ֽ�
				//����ܴ���   3�ֽ�
				//���һ�α�̷���ʱ�� 6�ֽ�
				//��������ܴ��� 3�ֽ�
				//���һ�����㷢��ʱ�� 6�ֽ�
				//���������ܴ���	3�ֽ�
//				case 1://���һ���������㷢��ʱ��
//					p8FnData+=3+3+4+3+6+3+6+3;
//					p8FnData[0]=0;
//					MW(((u32)p8rxbuff)+2+4,((u32)p8FnData)+1,4);
//					p8FnData[5]=0;
//					break;
//				case 2://���һ���¼����㷢��ʱ��
				case 1://���һ���¼����㷢��ʱ��
					p8FnData+=3+3+4+3+6+3+6+3+6+3;
					d64a=MRR(((u32)p8rxbuff),5);
					d64a<<=8;
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					p8=(u8 *)ADDR_DATABUFF;
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData),6);
					break;
//				case 3://���һ��Уʱ����ʱ��(��Уʱ��ʱ��)
				case 2://���һ��Уʱ����ʱ��(��Уʱ��ʱ��)
					p8FnData+=3+3+4+3+6+3+6+3+6+3+6+3;
					d64a=MRR(((u32)p8rxbuff),6);
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					p8=(u8 *)ADDR_DATABUFF;
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData),6);
					break;
			}
			break;
//16 F129 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
		case 129:
			UARTCtrl->RMError&=(~0x02);//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
			p8FnData+=6;
			p8rxbuff+=10;
AFN0DF161:
			p8=(u8 *)(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18);
			x=p8[0];//������
			if(x==4)
			{
				//������
				p8FnData[0]=x;
				//��
				p8FnData[1]=0;
				MW(((u32)p8rxbuff),(u32)p8FnData+2,4);
				//����2
				p8FnData[11]=0;
				MW(((u32)p8rxbuff)+4,(u32)p8FnData+12,4);
				//����4
				p8FnData[21]=0;
				MW(((u32)p8rxbuff)+8,(u32)p8FnData+22,4);
			}
			else
			{//����2
				//������
				p8FnData[0]=2;
				//��
				p8FnData[1]=0;
				MW(((u32)p8rxbuff),(u32)p8FnData+2,4);
				//����1
				p8FnData[6]=0;
				MW(((u32)p8rxbuff)+4,(u32)p8FnData+7,4);
				//����2
				p8FnData[11]=0;
				MW(((u32)p8rxbuff)+8,(u32)p8FnData+12,4);
			}
			break;
//24 F137 ���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
		case 137:
			p8FnData+=6;
			p8rxbuff+=10;
			p8=(u8 *)(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18);
			x=p8[0];//������
			//��1�������й�����
			YM=MRR(ADDR_TYMDHMS+4,2);//��ǰʱ������
			for(i=0;i<3;i++)
			{
				YM1=MRR(((u32)p8rxbuff)+3+i*17,2);//��n����ʱ������
				if(YM==YM1)
				{//��ǰʱ�����µ�����n����ʱ������
					break;
				}
			}
			if(x==4)
			{
				//������
				p8FnData[0]=x;
				if(i!=3)
				{
					//��
					p8FnData[1]=0;
					MW(((u32)p8rxbuff)+5+i*17,(u32)p8FnData+2,4);
					//����2
					p8FnData[11]=0;
					MW(((u32)p8rxbuff)+4+5+i*17,(u32)p8FnData+12,4);
					//����4
					p8FnData[21]=0;
					MW(((u32)p8rxbuff)+8+5+i*17,(u32)p8FnData+22,4);
				}
			}
			else
			{//����2
				//������
				p8FnData[0]=2;
				if(i!=3)
				{
					//��
					p8FnData[1]=0;
					MW(((u32)p8rxbuff)+5+i*17,(u32)p8FnData+2,4);
					//����1
					p8FnData[6]=0;
					MW(((u32)p8rxbuff)+4+5+i*17,(u32)p8FnData+7,4);
					//����2
					p8FnData[11]=0;
					MW(((u32)p8rxbuff)+8+5+i*17,(u32)p8FnData+12,4);
				}
			}
			//��2�������й�����
			YM=YM_Sub1M(YM);//���¼�1��,���ؼ�1�������
			for(i=0;i<3;i++)
			{
				YM1=MRR(((u32)p8rxbuff)+3+i*17,2);//��n����ʱ������
				if(YM==YM1)
				{//��ǰʱ�����¼�1�µ�����n����ʱ������
					break;
				}
			}
			if(x==4)
			{
				//������
				p8FnData[26+5]=x;
				if(i!=3)
				{
					//��
					p8FnData[27+5]=0;
					MW(((u32)p8rxbuff)+5+i*17,(u32)p8FnData+28+5,4);
					//����2
					p8FnData[37+5]=0;
					MW(((u32)p8rxbuff)+4+5+i*17,(u32)p8FnData+38+5,4);
					//����4
					p8FnData[47+5]=0;
					MW(((u32)p8rxbuff)+8+5+i*17,(u32)p8FnData+48+5,4);
				}
			}
			else
			{//����2
				//������
				p8FnData[26+5]=2;
				if(i!=3)
				{
					//��
					p8FnData[27+5]=0;
					MW(((u32)p8rxbuff)+5+i*17,(u32)p8FnData+28+5,4);
					//����1
					p8FnData[32+5]=0;
					MW(((u32)p8rxbuff)+4+5+i*17,(u32)p8FnData+33+5,4);
					//����2
					p8FnData[37+5]=0;
					MW(((u32)p8rxbuff)+8+5+i*17,(u32)p8FnData+38+5,4);
				}
			}
			break;
//32 F145 ���������й��������������ʱ�䣨�ܡ�����1��M��
//		case 145:
//			p8FnData+=6;
//			p8rxbuff+=10;
//			//������
//			p8FnData[0]=0;
//			//������
//			x=MRR(((u32)p8rxbuff),2);
//			x<<=4;
//			MWR(x,(u32)p8FnData+1,3);
//			//��ʱ��
//			MW(((u32)p8rxbuff)+2,(u32)p8FnData+4,4);
//			break;
//49 F162 ���ܱ�����ʱ��
		case 162:
			//���ڼ���
			d64a=MRR(((u32)p8rxbuff)+10,6);
			MWR(d64a,ADDR_DATABUFF,3);
			MWR(d64a>>24,ADDR_DATABUFF+4,3);
			p8=(u8 *)ADDR_DATABUFF;
			p8[3]=0;
			YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
			i=p8[3];
			if(i==0x0)
			{
				i=7;
			}
			i<<=5;
			d64b=i;
			d64b<<=32;
			d64a|=d64b;
			MWR(d64a,((u32)p8FnData)+1+6,6);

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
				MWR(d64a,((u32)p8rxbuff)+14,3);
				MWR(d64a>>24,((u32)p8rxbuff)+14+4,3);
				YMDCalculateW(((u32)p8rxbuff)+14);//ָ����ַ�����е���������ʱ���������
				i=p8rxbuff[14+3];
				if(i==0x0)
				{
					i=7;
				}
				i<<=5;
				d64b=i;
				d64b<<=32;
				d64a|=d64b;
				MWR(d64a,((u32)p8FnData)+1,6);//���볭��ʱ��
			}
			break;
//50 F166 ���ܱ�����޸Ĵ�����ʱ��
		case 166:
			p8FnData+=6;
			p8rxbuff+=10;
			switch(UARTCtrl->RMCount)//�����������
			{
				//Уʱ�ܴ��� 3�ֽ�
				case 0://���ܱ����һ��ʱ���޸�ǰ�ͺ�ʱ��
					//Уʱǰʱ��
					d64a=MRR(((u32)p8rxbuff)+6,6);
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData)+3,6);
					//Уʱ��ʱ��
					d64a=MRR(((u32)p8rxbuff),6);
					MWR(d64a,ADDR_DATABUFF,3);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					p8=(u8 *)ADDR_DATABUFF;
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData)+9,6);
					break;
				//���ܱ�ʱ�β����޸Ĵ���  3�ֽ�
				case 1://���һ�ε��ܱ�ʱ�β����޸�ʱ��
					d64a=MRR(((u32)p8rxbuff),5);
					p8=(u8 *)ADDR_DATABUFF;
					p8[0]=0;
					MWR(d64a,ADDR_DATABUFF+1,2);
					MWR(d64a>>24,ADDR_DATABUFF+4,3);
					p8[3]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					d64b=i;
					d64b<<=32;
					d64a|=d64b;
					MWR(d64a,((u32)p8FnData)+18,6);
					break;
			}
			break;
//��չ3 ���г����ն������ʾֵ���ܡ�����1��M��
		case 0xE3:
			x=MRR(ADDR_TYMDHMS+3,3);
			MWR(x,((u32)p8FnData)+6+Office_LastDate_Date,3);//����ʱ��
			p8FnData[0]=1;//�����������־
			MW(ADDR_TYMDHMS+1,((u32)p8FnData)+6+Office_AFN0DF161_LastDate_Source,5);//����ʱ��
			p8rxbuff+=10;
			p8FnData+=6+Office_AFN0DF161_LastDate_Source+5;
			goto AFN0DF161;
	}
}
#endif














