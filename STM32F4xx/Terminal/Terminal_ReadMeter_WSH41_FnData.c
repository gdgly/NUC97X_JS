
//�ն˳�����ʤ��Լ4.1�汾������������Fn���ݻ���
#include "Project.h"
#include "Terminal_ReadMeter_WSH41_FnData.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_ReadMeter_CountAndFlag.h"



#if (USER/100)==10//�ӱ�
extern const u32 WSH41_RMLib[];
void Terminal_ReadMeter_WSH41_TxData(u32 PORTn,u8 *p8RMLib)//�ն���ʤ��Լ4.1�汾������
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	//ȡͨ�ŵ�ַ
	MR(((u32)p8txbuff),ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
	
	i=p8txbuff[1]&0xf;
	i<<=8;
	i+=p8txbuff[0];
	//���ַת��
	p8txbuff[0]=bcd_hex(i);//8λѹ��BCD��תΪHEX
	//�����־��55H
	p8txbuff[1]=0x55;
	
	//ȡDI
	i=(u32)p8RMLib;
	i+=10+(3*UARTCtrl->RMCount);
	MR(((u32)p8txbuff)+2,i,2);
	
	//ȡ���ݳ���
	i+=2;
	MR(((u32)p8txbuff)+4,i,1);
	
	//CS����
	p8txbuff[5]=0;
	for(i=0;i<5;i++)
	{
	p8txbuff[5]+=p8txbuff[i];
	}
	//������
	p8txbuff[6]=0x0D;
	UARTCtrl->TxByte=7;//TX����(�跢���ֽ���)
}

void ReadMeter_WSH41_FnData(u32 PORTn,u8 *p8RMLib)//��ʤ��Լ4.1�汾���������������Fn���ݻ���
{
	ReadMeter_WSH41_MulFnData(PORTn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//��ʤ��Լ4.1�汾���������������Fn���ݻ���
}

void ReadMeter_WSH41_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib)//��ʤ��Լ4.1�汾���������������Fn���ݻ���
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u64 d64a;
	u64 d64b;
	
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8FnData;

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=((u8 *)Get_ADDR_UARTnRx(PORTn))+4;
	p8FnData=(u8 *)Addr_FnDataBuff;
		
	i=p8RMLib[0];
	switch(i)
	{
//1 F25 ��ǰ���༰����/�޹����ʡ����������������ѹ��������������������ڹ���
		case 25:
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			y=MRR(ADDR_AFN04F25+((Pn-1)*11)+10,1);//ȡ��Դ���߷�ʽ BS8  1
			y&=0x3;//D0��D1�����ʾ�������Դ���߷�ʽ����ֵ1��3���α�ʾ�������ߡ��������ߡ������
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���й�����		
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ3�ֽڣ�0.1W����1376.1��Լ3�ֽ�4λС����kW��
					MWR(y,((u32)p8FnData)+6,3);
					break;
				case 1://A�й�����
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ3�ֽڣ�0.1W����1376.1��Լ3�ֽ�4λС����kW��
					MWR(y,((u32)p8FnData)+6+3,3);
					break;
				case 2://B�й�����
					if(y==1)
					{//��������
						break;
					}
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ3�ֽڣ�0.1W����1376.1��Լ3�ֽ�4λС����kW��
					MWR(y,((u32)p8FnData)+6+6,3);
					break;
				case 3://C�й�����
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ3�ֽڣ�0.1W����1376.1��Լ3�ֽ�4λС����kW��
					MWR(y,((u32)p8FnData)+6+9,3);
					break;
				case 4://���޹�����
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ3�ֽڣ�0.1var����1376.1��Լ3�ֽ�4λС����kvar��
					MWR(y,((u32)p8FnData)+6+12,3);
					break;
				case 5://A�޹�����
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ3�ֽڣ�0.1var����1376.1��Լ3�ֽ�4λС����kvar��
					MWR(y,((u32)p8FnData)+6+15,3);
					break;
				case 6://B�޹�����
					if(y==1)
					{//��������
						break;
					}
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ3�ֽڣ�0.1var����1376.1��Լ3�ֽ�4λС����kvar��
					MWR(y,((u32)p8FnData)+6+18,3);
					break;
				case 7://C�޹�����
					y=MRR((u32)p8rxbuff+5,3);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ3�ֽڣ�0.1var����1376.1��Լ3�ֽ�4λС����kvar��
					MWR(y,((u32)p8FnData)+6+21,3);
					break;
				case 8://�ܹ�������
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ2�ֽڣ�0.001����1376.1��Լ2�ֽ�1λС����%��
					MWR(y,((u32)p8FnData)+6+24,2);
					break;
				case 9://A��������
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ2�ֽڣ�0.001����1376.1��Լ2�ֽ�1λС����%��
					MWR(y,((u32)p8FnData)+6+26,2);
					break;
				case 10://B��������
					if(y==1)
					{//��������
						break;
					}
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ʤ��Լ2�ֽڣ�0.001����1376.1��Լ2�ֽ�1λС����%��
					MWR(y,((u32)p8FnData)+6+28,2);
					break;
				case 11://C��������
					d64a=MRR((u32)p8rxbuff+5,2);
					d64a=hex_bcd16(d64a);//HEX��תΪ16λѹ��BCD��
					//��ʤ��Լ2�ֽڣ�0.001����1376.1��Լ2�ֽ�1λС����%��
					MWR(d64a,((u32)p8FnData)+6+30,2);
					//��,A,B,C���ڹ���(���й�����/������������)
					for(i=0;i<4;i++)
					{
						x=MRR(((u32)p8FnData)+6+(3*i),3);//ȡ�й�����
						if(x!=0xeeeeee)
						{
							x&=0x7fffff;//ȥ����λ
							x=bcd_hex(x);
							d64a=x;
							d64a*=1000;//����������3λС��
							x=MRR(((u32)p8FnData)+6+24+(2*i),2);//ȡ��������
							if(x!=0xeeee)
							{
								x&=0x7fff;//ȥ����λ
								x=bcd_hex(x);
								d64a/=x;
								x=d64a;
								x=hex_bcd(x);
								MWR(x,((u32)p8FnData)+6+50+(3*i),3);
							}
						}
					}
					break;
				case 12://A���ѹ
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ѹ ��ʤ��Լ2�ֽڣ�0.01V����1376.1��Լ2�ֽ�1λС����V��
					y>>=4;
					MWR(y,((u32)p8FnData)+6+32,2);
					break;
				case 13://B���ѹ
					if(y==1)
					{//��������
						break;
					}
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ѹ ��ʤ��Լ2�ֽڣ�0.01V����1376.1��Լ2�ֽ�1λС����V��
					y>>=4;
					MWR(y,((u32)p8FnData)+6+34,2);
					break;
				case 14://C���ѹ
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//��ѹ ��ʤ��Լ2�ֽڣ�0.01V����1376.1��Լ2�ֽ�1λС����V��
					y>>=4;
					MWR(y,((u32)p8FnData)+6+36,2);
					break;
				case 15://A�����
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//���� ��ʤ��Լ2�ֽڣ�0.01A����1376.1��Լ3�ֽ�3λС����A��
					y<<=4;
					MWR(y,((u32)p8FnData)+6+38,3);
					break;
				case 16://B�����
					if(y==1)
					{//��������
						break;
					}
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//���� ��ʤ��Լ2�ֽڣ�0.01A����1376.1��Լ3�ֽ�3λС����A��
					y<<=4;
					MWR(y,((u32)p8FnData)+6+41,3);
					break;
				case 17://C�����
					y=MRR((u32)p8rxbuff+5,2);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					//���� ��ʤ��Լ2�ֽڣ�0.01A����1376.1��Լ3�ֽ�3λС����A��
					y<<=4;
					MWR(y,((u32)p8FnData)+6+44,3);
					break;
				//�������

				case 18://�й�����0-3λ���δ����ܡ�A��B��C��0Ϊ��1Ϊ��
					x=MRR((u32)p8rxbuff+5,1);
//					if((x&0x1)!=0x0)
//					{//��
//						if(p8FnData[6+2]!=0xee)
//						{
//							p8FnData[6+2]|=0x80;
//						}
//					}
//					if((x&0x2)!=0x0)
//					{//A
//						if(p8FnData[6+5]!=0xee)
//						{
//							p8FnData[6+5]|=0x80;
//						}
//					}
//					if((x&0x4)!=0x0)
//					{//B
//						if(p8FnData[6+8]!=0xee)
//						{
//							p8FnData[6+8]|=0x80;
//						}
//					}
//					if((x&0x8)!=0x0)
//					{//C
//						if(p8FnData[6+11]!=0xee)
//						{
//							p8FnData[6+11]|=0x80;
//						}
//					}
					for(i=0;i<4;i++)
					{
						if(p8FnData[6+2+i*3]!=0xee)
						{
							if(x&0x1)
							{
								p8FnData[6+2+i*3]|=0x80;
							}
						}
						x>>=1;
					}
					break;
				case 19://�޹�����0-3λ���δ����ܡ�A��B��C��0Ϊ��1Ϊ��
					x=MRR((u32)p8rxbuff+5,1);
					for(i=0;i<4;i++)
					{
						if(p8FnData[6+14+i*3]!=0xee)
						{
							if(x&0x1)
							{
								p8FnData[6+14+i*3]|=0x80;
							}
						}
						x>>=1;
					}
					break;
			}
			break;
//2 F26 A��B��C�������ͳ�����ݼ����һ�ζ����¼
		case 26:
			break;
//3 F27 ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
		case 27:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://���ܱ�����ʱ��YYMMDDHhmm
					// ��ʤ��Լ5�ֽڣ����ν���Ϊ�꣨����λ������ʱ�֣���1376.1��Լ6�ֽڣ����ʱ�����꣩
					p8=(u8*)ADDR_DATABUFF;
					y=MRR((u32)p8rxbuff+5,1);
					p8[6]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+1,1);
					p8[5]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+2,1);
					p8[4]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					p8[3]=0;
					y=MRR((u32)p8rxbuff+5+3,1);
					p8[2]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+4,1);
					p8[1]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					p8[0]=0;
					YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
					MW((u32)p8,((u32)p8FnData)+6,3);
					MW(((u32)p8)+4,((u32)p8FnData)+6+3,3);
					//����
					i=p8[3];
					if(i==0x0)
					{
						i=7;
					}
					i<<=5;
					p8FnData[6+3+1]|=i;
					break;
				case 1://��ع���ʱ��
					y=MRR((u32)p8rxbuff+5,4);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					// ��ʤ��Լ4�ֽڣ��֣���1376.1��Լ4�ֽڣ��֣�
					MWR(y,((u32)p8FnData)+6+6,4);
					break;
				case 2://����ܴ���
					y=MRR((u32)p8rxbuff+5+12,1);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					// ��ʤ��Լ1�ֽڣ�1376.1��Լ3�ֽ�
 					MWR(y,((u32)p8FnData)+6+10,3);
					//��������������
					y=MRR((u32)p8rxbuff+5,1);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					// ��ʤ��Լ1�ֽڣ�1376.1��Լ3�ֽ�
 					MWR(y,((u32)p8FnData)+6+28,3);					
					break;
				case 3://���һ�α�̷���ʱ��
					p8FnData[6+13]=0;
					// ��ʤ��Լ4�ֽڣ�����ʱ�֣���1376.1��Լ6�ֽڣ����ʱ�����꣩
					y=MRR((u32)p8rxbuff+5+3,1);
					p8FnData[6+14]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+2,1);
					p8FnData[6+15]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+1,1);
					p8FnData[6+16]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5,1);
					p8FnData[6+17]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					p8FnData[6+18]=0;
					break;
					//��������ܴ���
					//���һ�ε�����㷢��ʱ��
				case 4://���һ������������㷢��ʱ��
					p8FnData[6+31]=0;
					// ��ʤ��Լ4�ֽڣ�����ʱ�֣���1376.1��Լ6�ֽڣ����ʱ�����꣩
					y=MRR((u32)p8rxbuff+5+3,1);
					p8FnData[6+32]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+2,1);
					p8FnData[6+33]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+1,1);
					p8FnData[6+34]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5,1);
					p8FnData[6+35]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					p8FnData[6+36]=0;
					break;
					//�¼������ܴ���
					//���һ���¼����㷢��ʱ��
					//Уʱ�ܴ���
					//���һ��Уʱ����ʱ��
			}
			break;
//4 F28 �������״̬�ּ����λ��־
		case 28:
			break;
//5 F29 ��ǰͭ�������й��ܵ���ʾֵ
//6 F30 ��һ������ͭ�������й��ܵ���ʾֵ
		case 29:
		case 30:
			break;
//7 F31 ��ǰA��B��C������/�����й�����ʾֵ������޹�1/2����ʾֵ
//8 F32 ��һ������A��B��C������/�����й�����ʾֵ������޹�1/2����ʾֵ
		case 31:
		case 32:
			break;

//9 F49 ��ǰ��ѹ��������λ��
//10 F57 ��ǰA��B��C�����ѹ������2��N��г����Чֵ
//11 F58 ��ǰA��B��C�����ѹ������2��N��г��������
//12 F65 ��ǰ������Ͷ��״̬
//13 F66 ��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//14 F67 ���ա����µ������ۼƲ������޹�������
//15 F73 ֱ��ģ������ǰ����


//16 F129 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
		case 129:
			UARTCtrl->RMError&=(~0x02);//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
//18 F131 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
		case 131:
			d64a=MRR((u32)p8rxbuff+5,4);
			d64a=hex_bcd16(d64a);//HEX��תΪ16λѹ��BCD��
			//��ʤ��Լ4�ֽڣ�Wh����1376.1��Լ5�ֽ�4λС����kWh��
			d64a<<=4;
			MWR(d64a,((u32)p8FnData)+6+1+5*UARTCtrl->RMCount,5);
			p8FnData[6]=4;   //���������̶�ֵ
			break;
//24 F137 ���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
//26 F139 ���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
		case 139:
		case 137:
			d64a=MRR((u32)p8rxbuff+5,3);
			d64a=hex_bcd16(d64a);//HEX��תΪ16λѹ��BCD��
			//��ʤ��Լ3�ֽڣ�Wh����1376.1��Լ5�ֽ�4λС����kWh��
			d64a<<=4;
			MWR(d64a,((u32)p8FnData)+6+1+5*UARTCtrl->RMCount,5);
			p8FnData[6]=4;   //���������̶�ֵ
			break;
//40 F153 ��һʱ�����������й�����ʾֵ���ܡ�����1��M��
//41 F154 �ڶ�ʱ�����������й�����ʾֵ���ܡ�����1��M��
//42 F155 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//43 F156 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//44 F157 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//45 F158 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//46 F159 ����ʱ�����������й�����ʾֵ���ܡ�����1��M��
//47 F160 �ڰ�ʱ�����������й�����ʾֵ���ܡ�����1��M��
		case 153:
		case 154:
		case 155:
		case 156:
		case 157:
		case 158:
		case 159:
		case 160:
			break;
//17 F130 ��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��
//19 F132 ��ǰ�����޹�������޹�2������ʾֵ���ܡ�����1��M��
//25 F138 ���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��
//27 F140 ���£���һ�����գ������޹�������޹�2������ʾֵ���ܡ�����1��M��
		case 130:
		case 132:
		case 138:
		case 140:
			break;
//20 F133 ��ǰһ�����޹�����ʾֵ���ܡ�����1��M��
//21 F134 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
//22 F135 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
//23 F136 ��ǰ�������޹�����ʾֵ���ܡ�����1��M��
		case 133:
		case 134:
		case 135:
		case 136:
			d64a=MRR((u32)p8rxbuff+5,4);
			d64a=hex_bcd16(d64a);//HEX��תΪ16λѹ��BCD��
			//��ʤ��Լ4�ֽڣ�varh����1376.1��Լ4�ֽ�2λС����kvarh��
			d64a>>=4;
			MWR(d64a,((u32)p8FnData)+6+1+4*UARTCtrl->RMCount,4);
			p8FnData[6]=4;   //���������̶�ֵ4
			break;
//28 F141 ���£���һ�����գ�һ�����޹�����ʾֵ���ܡ�����1��M��
//29 F142 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//30 F143 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
//31 F144 ���£���һ�����գ��������޹�����ʾֵ���ܡ�����1��M��
		case 141:
		case 142:
		case 143:
		case 144:
			y=MRR((u32)p8rxbuff+5,3);
			y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			//��ʤ��Լ3�ֽڣ�varh����1376.1��Լ4�ֽ�2λС����kvarh��
			y>>=4;
			MWR(y,((u32)p8FnData)+6+1+4*UARTCtrl->RMCount,4);
			p8FnData[6]=4;   //���������̶�ֵ4
			break;
//32 F145 ���������й��������������ʱ�䣨�ܡ�����1��M��
//34 F147 ���·����й��������������ʱ�䣨�ܡ�����1��M��
//36 F149 ���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
//38 F151 ���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
		case 145:
		case 147:
		case 149:
		case 151:
			if((UARTCtrl->RMCount%2)==0)
			{			
				// ��ʤ��Լ2�ֽڣ�0.1W����1376.1��Լ3�ֽ�4λС����KW��
				y=MRR((u32)p8rxbuff+5,2);
				y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
				MWR(y,((u32)p8FnData)+6+1+7*(UARTCtrl->RMCount/2),3);
			}
			else
			{
				// ��ʤ��Լ4�ֽڣ�����ʱ�֣���1376.1��Լ4�ֽڣ���ʱ���£�
				y=MRR((u32)p8rxbuff+5+3,1);
				p8FnData[6+1+3+7*(UARTCtrl->RMCount/2)]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
				y=MRR((u32)p8rxbuff+5+2,1);
				p8FnData[6+1+4+7*(UARTCtrl->RMCount/2)]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
				y=MRR((u32)p8rxbuff+5+1,1);
				p8FnData[6+1+5+7*(UARTCtrl->RMCount/2)]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
				y=MRR((u32)p8rxbuff+5,1);
				p8FnData[6+1+6+7*(UARTCtrl->RMCount/2)]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			}
			p8FnData[6]=4;   //���������̶�ֵ
			break;
//33 F146 ���������޹��������������ʱ�䣨�ܡ�����1��M��
//35 F148 ���·����޹��������������ʱ�䣨�ܡ�����1��M��
//37 F150 ���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
//39 F152 ���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
		case 146:
		case 148:
		case 150:
		case 152:
			break;
//48 F161 ���ܱ�Զ�̿���ͨ�ϵ�״̬����¼
		case 161:
			break;
//49 F162 ���ܱ�����ʱ��
		case 162:
			// ��ʤ��Լ5�ֽڣ����ν���Ϊ�꣨����λ������ʱ�֣���1376.1��Լ6�ֽڣ����ʱ�����꣩
			p8=(u8*)ADDR_DATABUFF;
			y=MRR((u32)p8rxbuff+5,1);
			p8[6]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			y=MRR((u32)p8rxbuff+5+1,1);
			p8[5]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			y=MRR((u32)p8rxbuff+5+2,1);
			p8[4]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			p8[3]=0;
			y=MRR((u32)p8rxbuff+5+3,1);
			p8[2]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			y=MRR((u32)p8rxbuff+5+4,1);
			p8[1]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			p8[0]=0;
			YMDCalculateW(ADDR_DATABUFF);//ָ����ַ�����е���������ʱ���������
			MW((u32)p8,((u32)p8FnData)+7,3);
			MW(((u32)p8)+4,((u32)p8FnData)+7+3,3);
			//����
			i=p8[3];
			if(i==0x0)
			{
				i=7;
			}
			i<<=5;
			p8FnData[7+3+1]|=i;
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
//49 F165 ���ܱ��ز���������ʱ��
		case 165:
			switch(UARTCtrl->RMCount)//�����������
			{
				case 0://����ܴ���
					y=MRR((u32)p8rxbuff+5+12,1);
					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					// ��ʤ��Լ1�ֽڣ�1376.1��Լ3�ֽ�
 					MWR(y,((u32)p8FnData)+6,3);
					break;
				case 1://���һ�α�̷���ʱ��
					p8FnData[6+3]=0;
					// ��ʤ��Լ4�ֽڣ�����ʱ�֣���1376.1��Լ6�ֽڣ����ʱ�����꣩
					y=MRR((u32)p8rxbuff+5+3,1);
					p8FnData[6+4]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+2,1);
					p8FnData[6+5]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5+1,1);
					p8FnData[6+6]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					y=MRR((u32)p8rxbuff+5,1);
					p8FnData[6+7]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
					p8FnData[6+8]=0;
					break;
			}
			break;
			
//50 F166 ���ܱ�����޸Ĵ�����ʱ��
		case 166:
//51 F167 ���ܱ����õ���Ϣ
		case 167:
//52 F168 ���ܱ������Ϣ
		case 168:
			break;
//		
//#if ERC8==1		
//		case 0xE1:
//0xE1 ��չ1 ERC8���ܱ��������¼�������
//			switch(UARTCtrl->RMCount)//�����������
//			{
//				case 0://���һ�α��ʱ��(��Fn=27����ʱ)
//					p8FnData[6]=0;
//					// ��ʤ��Լ4�ֽڣ�����ʱ�֣���1376.1��Լ6�ֽڣ����ʱ�����꣩
//					y=MRR((u32)p8rxbuff+5+3,1);
//					p8FnData[6+1]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
//					y=MRR((u32)p8rxbuff+5+2,1);
//					p8FnData[6+2]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
//					y=MRR((u32)p8rxbuff+5+1,1);
//					p8FnData[6+3]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
//					y=MRR((u32)p8rxbuff+5,1);
//					p8FnData[6+4]=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
//					p8FnData[6+5]=0;					
//					break;
//				case 1://��������������(��Fn=27����ʱ)
//					y=MRR((u32)p8rxbuff+5,1);
//					y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
//					// ��ʤ��Լ1�ֽڣ�1376.1��Լ3�ֽ�
// 					MWR(y,((u32)p8FnData)+6+6,3);									
//					Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
//					if(Pn==0x0)
//					{//����ŵĵ��ܱ�/��������װ��ɾ��
//						break;
//					}
//					ERC8_Event(Pn-1,(u32)p8FnData);//���ܱ�������,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
//					break;
//			}
//			break;
//#endif
#if ERC13==1
//0xE2 ��չ2 ERC13��������Ϣ�¼�������
		case 0xE2:
			//��̴���(��Fn=27����ʱ)����������������(��Fn=27����ʱ)
			y=MRR((u32)p8rxbuff+5+12,1);
			y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			// ��ʤ��Լ1�ֽڣ�1376.1��Լ3�ֽ�
			MWR(y,((u32)p8FnData)+6,3);
			//��������������
			y=MRR((u32)p8rxbuff+5,1);
			y=hex_bcd(y);//HEX��תΪ8λѹ��BCD��
			// ��ʤ��Լ1�ֽڣ�1376.1��Լ3�ֽ�
			MWR(y,((u32)p8FnData)+6+3,3);					
			break;

#endif
	}
}

#endif












