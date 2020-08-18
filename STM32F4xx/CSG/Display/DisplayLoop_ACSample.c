

#include "../Hcsg/ProjectCSG.h"

#include "../Display/DisplayLoop_ACSample.h"
#include "../Display/Display.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
//#include "../Terminal/Terminal_ReadMeter_ACSample.h"



void Display_I(u32 Row,u32 ADDR_DATA)//������ʾ
{
	u32 i;
	u16 *p16;
	i=MRR(ADDR_DATA,3);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*6));
	if(i&0x800000)
	{
		i&=0x7fffff;
		p16[0]='-'+0x2000;
	}
	p16++;
	DisplayData_OffZero(i,6,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
}
void Display_P(u32 Row,u32 ADDR_DATA)//������ʾ
{
	u32 i;
	u16 *p16;
	i=MRR(ADDR_DATA,3);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*6));
	if(i&0x800000)
	{
		i&=0x7fffff;
		p16[0]='-'+0x2000;
	}
	p16++;
	DisplayData_OffZero(i,6,4,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
}
void Display_C(u32 Row,u32 ADDR_DATA)//����������ʾ
{
	u32 i;
	u16 *p16;
	i=MRR(ADDR_DATA,2);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*10));
	if(i&0x8000)
	{
		i&=0x7fff;
		p16[0]='-'+0x2000;
	}
	p16++;
	DisplayData_OffZero(i,4,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
}
void Display_E(u32 Row,u32 ADDR_DATA)//������ʾ
{
	u32 i;
	u16 *p16;
	i=MRR(ADDR_DATA,4);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*8));
	DisplayData_OffZero(i,8,2,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
}
void Display_YMDHM(u32 Row,u32 ADDR_DATA)//�����������ʱ����ʾ
{
	u8 *p8;
	u16 *p16;
	p8=(u8*)(ADDR_DATA);
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*6));
	p16[0]=(p8[4]>>4)+0x30+0x2000;
	p16[1]=(p8[4]&0xf)+0x30+0x2000;
	p16[2]='.'+0x2000;
	p16[3]=(p8[3]>>4)+0x30+0x2000;
	p16[4]=(p8[3]&0xf)+0x30+0x2000;
	p16[5]='.'+0x2000;
	p16[6]=(p8[2]>>4)+0x30+0x2000;
	p16[7]=(p8[2]&0xf)+0x30+0x2000;
	p16[8]=' '+0x2000;
	p16[9]=(p8[1]>>4)+0x30+0x2000;
	p16[10]=(p8[1]&0xf)+0x30+0x2000;
	p16[11]=':'+0x2000;
	p16[12]=(p8[0]>>4)+0x30+0x2000;
	p16[13]=(p8[0]&0xf)+0x30+0x2000;
}




u32 DisplayLoop_ACSample(u32 Row)//�ڲ�����������ʾ,��������
{
	u32 i;
#if (DLT500_REPORT == 0)
	u32 x;
	u16 *p16;
#endif
	u32 ADDR_DATA;
	ADDR_DATA=Get_ADDR_UARTnTx(ACSAMPLEPORT)+14;
	
#if (DLT500_REPORT == 1)
	if(TerminalDisplayLoop->Count == 0)
	{//���ȡDLT500��������ֻ��ʾ��ѹ
			GetACSample(0x0201ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row+2,0,2,(u8 *)
						"��ѹ(V):\n\r"
						"  Ua/Uab =\n\r"
						"  Ub/Uac =\n\r"
						"  Uc/Ubc =\n\r"
						);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<3;i++)
			{
				DisplayData_OffZero(MRR(ADDR_DATA+(2*i),2),4,1,(u16 *)(ADDR_STRINGBUFF+(84*(Row+2+i))+4+(2*11)));//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			}
			Row+=3;
	}
#else
	switch(TerminalDisplayLoop->Count)
	{
		case 0://�����ѹ���������
			//��ѹ
			GetACSample(0x0201ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"��ѹ(V):\n\r"
																			"  Ua/Uab =\n\r"
																			"  Ub/Uac =\n\r"
																			"  Uc/Ubc =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<3;i++)
			{
				DisplayData_OffZero(MRR(ADDR_DATA+(2*i),2),4,1,(u16 *)(ADDR_STRINGBUFF+(84*(Row+i))+4+(2*11)));//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			}
			Row+=3;
			//����
			GetACSample(0x0202ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"����(A):\n\r"
																			"  Ia =\n\r"
																			"  Ib =\n\r"
																			"  Ic =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<3;i++)
			{
				Display_I(Row+i,ADDR_DATA+(3*i));//������ʾ
			}
			Row+=3;
			break;
		case 1://�й�����
			GetACSample(0x0203ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"�й�����(kW):\n\r"
																			"  P��=\n\r"
																			"  Pa =\n\r"
																			"  Pb =\n\r"
																			"  Pc =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<4;i++)
			{
				Display_P(Row+i,ADDR_DATA+(3*i));//������ʾ
			}
			Row+=4;
			break;
		case 2://�޹�����
			GetACSample(0x0204ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"�޹�����(kvar):\n\r"
																			"  Q��=\n\r"
																			"  Qa =\n\r"
																			"  Qb =\n\r"
																			"  Qc =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<4;i++)
			{
				Display_P(Row+i,ADDR_DATA+(3*i));//������ʾ
			}
			Row+=4;
			break;
		case 3://��������
			GetACSample(0x0206ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"��������:\n\r"
																			"  COS ��=\n\r"
																			"  COS a =\n\r"
																			"  COS b =\n\r"
																			"  COS c =\n\r"
																			);//��ʾ�˵��ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*(Row+1))+4+(2*5));
			p16[0]=237+0x2000;
			p16+=84/2;
			p16[0]=237+0x2000;
			p16+=84/2;
			p16[0]=237+0x2000;
			p16+=84/2;
			p16[0]=237+0x2000;
			Row++;
			for(i=0;i<4;i++)
			{
				Display_C(Row+i,ADDR_DATA+(2*i));//����������ʾ
			}
			Row+=4;
			break;
		case 4://�ܼ������������й�����
			GetACSample(0x0001ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"�����й�����(kWh):\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  ƽ =\n\r"
																			"  �� =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<5;i++)
			{
				Display_E(Row+i,ADDR_DATA+(4*i));//������ʾ
			}
			Row+=5;
			break;
		case 5://�ܼ������ʷ����й�����
			GetACSample(0x0002ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"�����й�����(kWh):\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  ƽ =\n\r"
																			"  �� =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<5;i++)
			{
				Display_E(Row+i,ADDR_DATA+(4*i));//������ʾ
			}
			Row+=5;
			break;
		case 6://�ܼ������ʽM���޹�1����
			GetACSample(0x0003ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"����޹�1����(kvarh)\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  ƽ =\n\r"
																			"  �� =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<5;i++)
			{
				x=MRR(ADDR_DATA+(4*i)+3,1);
				if(x&0x80)
				{//��
					x&=0x7f;
					MWR(x,ADDR_DATA+(4*i)+3,1);
					p16=(u16 *)(ADDR_STRINGBUFF+(84*(Row+i))+4+(2*8));
					p16[0]='-'+0x2000;
					p16++;
					x=MRR(ADDR_DATA+(4*i),4);
					DisplayData_OffZero(x,8,2,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				}
				else
				{
					Display_E(Row+i,ADDR_DATA+(4*i));//������ʾ
				}
			}
			Row+=5;
			break;
		case 7://�ܼ������ʽM���޹�2����
			GetACSample(0x0004ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"����޹�2����(kvarh)\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  ƽ =\n\r"
																			"  �� =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<5;i++)
			{
				x=MRR(ADDR_DATA+(4*i)+3,1);
				if(x&0x80)
				{//��
					x&=0x7f;
					MWR(x,ADDR_DATA+(4*i)+3,1);
					p16=(u16 *)(ADDR_STRINGBUFF+(84*(Row+i))+4+(2*8));
					p16[0]='-'+0x2000;
					p16++;
					x=MRR(ADDR_DATA+(4*i),4);
					DisplayData_OffZero(x,8,2,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				}
				else
				{
					Display_E(Row+i,ADDR_DATA+(4*i));//������ʾ
				}
			}
			Row+=5;
			break;
		case 8://�й��������
			GetACSample(0x0101ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"�����й��������(kW)\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  �� =\n\r"
																			"  ƽ =\n\r"
																			"  �� =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<5;i++)
			{
				Display_P(Row+i,ADDR_DATA+(8*i));//������ʾ
			}
			Row+=5;
			break;
		case 9://�й������������ʱ��
			GetACSample(0x0101ff00);//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
			DisplayMenuString(Row,0,2,(u8 *)
																			"�����й��������ʱ��\n\r"
																			" �� =\n\r"
																			" �� =\n\r"
																			" �� =\n\r"
																			" ƽ =\n\r"
																			" �� =\n\r"
																			);//��ʾ�˵��ַ���
			Row++;
			for(i=0;i<5;i++)
			{
				Display_YMDHM(Row+i,ADDR_DATA+3+(8*i));//�����������ʱ����ʾ
			}
			Row+=5;
			break;
	}
#endif
	return Row;
}




















