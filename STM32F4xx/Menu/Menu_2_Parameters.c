
//�˵�2 ������ֵ
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Terminal/Terminal_AFN0C_RealTimeData_Fn.h"



extern const u8 UnitList[][6];//ͨ�õ�λ�б�
extern const u8 SList[][2];//���������б�
extern const u8 CommunicationSpeedList[][6];//ͨ�������б�

const u8 QUnitList[][4]=//������λ�б�
{
	"kWh",
	"MWh",
};

#if ((Project/100)==3 && (USER/100)==10)//�ӱ�ר��
const u8 ProtocolList[][13]=//ͨ��Э���б�
{
	"����       ",//0 �ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
	"DLT645-1997",//1 DL/T 645-1997
	"��������   ",//2 ��������װ��ͨ��Э��	
	"��ʤ��Լ   ",//7 ��ʤ��Լ
	"DLT645-2007",//30 DL/T 645-2007
	"��ѹ�ز�   ",//31 "���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
	"����       ",//����
};
#elif ((USER/100)==5)//�Ϻ�
const u8 ProtocolList[][13]=//ͨ��Э���б�
{
	"����       ",//0 �ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
	"DLT645-1997",//1 DL/T 645-1997
	"��������   ",//2 ��������װ��ͨ��Э��	
	"698.45��Լ ",//698��Լ
	"�Ϻ���Լ   ",//21 ��ʤ��Լ
	"DLT645-2007",//30 DL/T 645-2007
	"��ѹ�ز�   ",//31 "���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
	"����       ",//����
};
#elif ((Project/100)==3 && (USER/100)==12)//�Ĵ�ר��
const u8 ProtocolList[][13]=//ͨ��Э���б�
{
	"����       ",//0 �ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
	"DLT645-1997",//1 DL/T 645-1997
	"��������   ",//2 ��������װ��ͨ��Э��	
	"�����Լ   ",//4 �����Լ
	"DLT645-2007",//30 DL/T 645-2007
	"��ѹ�ز�   ",//31 "���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
	"����       ",//����
};
#else
const u8 ProtocolList[][13]=//ͨ��Э���б�
{
	"����       ",//0 �ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
	"DLT645-1997",//1 DL/T 645-1997
	"��������   ",//2 ��������װ��ͨ��Э��	
	"DLT645-2007",//30 DL/T 645-2007
	"��ѹ�ز�   ",//31 "���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
	"����       ",//����
};
#endif

const u8 S0List[][5]=//���¸����б�
{
	"�ϸ�",
	"�¸�",
};

const u8 pMenu3_2[]=
{
	"������ֵ\n\r"
	"  1.ʱ�οز���\n\r"
	"  2.���ݿز���\n\r"
	"  3.Ӫҵ��ͣ�ز���\n\r"
	"  4.�����¸��ز���\n\r"
	"  5.�µ�ز���\n\r"
	"  6.kv ki kp����\n\r"
	"  7.���ܱ����\n\r"
	"  8.���ò���\n\r"
#if (USER/100)==13//������
	"  9.�ն˹�����ά��\n\r"
#endif
};

const u8 pMenu4_2[]=
{
	"������ֵ\n\r"
	"  1.kv ki����\n\r"
	"  2.���ܱ����\n\r"
};

const u8 pMenu_2_1[]=
{
	"ʱ�οز���\n\r"
	"  1.����1\n\r"
	"  2.����2\n\r"
	"  3.����3\n\r"
};

const u8 pMenu_2_1_1[]=
{
	"ʱ�ο�?����\n\r"
	"�ܼ���\n\r"
	"����ϵ��:\n\r"
	"Ͷ���ִ�:\n\r"
	"����ʱ�μ���ֵ(kW):\n\r"
};

const u8 pMenu_2_2[]=
{
	"���ݿز���\n\r"
	"�ܼ���\n\r"
};

const u8 WorkersDayOffList[][20]=//���ݿز����б�
{
	"���ʶ�ֵ  :",
	"��ʼʱ��  :",
	"����ʱ��  :",
	"ÿ���޵���:",
	"Ͷ���ִ�  :",
};

const u8 pMenu_2_3[]=
{
	"�����¸��ز���\n\r"
	"�ܼ���\n\r"
};

const u8 PowerLowerList[][20]=//�����¸����б�
{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
	"����ϵ��:",
	"��ǰ����:",
	"����ʱ��:",
	"Ͷ���ִ�:",
	"��1�澯 :",
	"��2�澯 :",
#else
	"���ʶ�ֵ:",
	"����ϵ��:",
	"��ǰ����:",
	"����ʱ��:",
	"Ͷ���ִ�:",
	"��1�澯 :",
	"��2�澯 :",
#endif
};

#if((USER/100)!=5)//���Ϻ��û�
const u8 pMenu_2_4[]=
{
	"Ӫҵ��ͣ�ز���\n\r"
	"�ܼ���\n\r"
};

const u8 BusinessQuitList[][20]=//Ӫҵ��ͣ�ز����б�
{
	"���ʶ�ֵ:",
	"��ʼʱ��:",
	"����ʱ��:",
	"Ͷ���ִ�:",
};

const u8 pMenu_2_5[]=
{
	"�µ�ز���\n\r"
	"�ܼ���\n\r"
};

const u8 MonthEQList[][20]=//�µ�ز����б�
{
	"��ض�ֵ:",
	"����ϵ��:",
	"��ֵϵ��:",
	"���õ���:",
	"Ͷ���ִ�:",
};
#endif

const u8 pMenu_2_6[]=
{
	"Kv Ki Kp����\n\r"
	"  1.Kp����\n\r"
	"  2.Kv Ki����\n\r"
};

const u8 pMenu_2_6_1[]=
{
	"Kp����\n\r"
	"����\n\r"
};

const u8 pMenu_2_6_2[]=
{
	"Kv Ki����\n\r"
	"������\n\r"
};

const u8 KvKiList[][20]=//KvKi�б�
{
	"Kv:",
	"Ki:",
};

const u8 pMenu_2_7[]=
{
	"���ܱ����\n\r"
	"������\n\r"
};

#if(((Project/100)==3) && ((USER/100)==5))//�Ϻ�ר��
const u8 MeterList[][20]=//���ܱ�����б�
{
	"������:",
	"ͨ  ��:",
	"Э  ��:",
	"���ַ:",
	"�����:",
	"С���:",
};
#else
const u8 MeterList[][20]=//���ܱ�����б�
{
	"�ֱ��:",
	"ͨ��  :",
	"Э��  :",
	"���ַ:",
};
#endif

const u8 MeterParameterList[][20]=//̨���������б�
{
	"������",
	"ͨ������",
	"ͨ�Ŷ˿�",
	"ͨ�Ź�Լ",
	"ͨ�ŵ�ַ",
	"�ɼ���",
	"�����",
	"С���",
};

const u8 pMenu2_2_3[]=
{
	"�������в���\n\r"
	"  1.����ʱ������\n\r"
	"  2.������\n\r"
};

const u8 MeterReadingDateList[][20]=//�������б�
{
	"������",
	"1 2 3 4 5 6 7 8 9",
	"10 11 12 13 14 15",
	"16 17 18 19 20 21",
	"22 23 24 25 26 27",
	"28 29 30 31",
};

#if ((Project/100)==3 || (Project/100)==4)//ר���նˡ������ն�
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_7
// Descriptions:        ���ܱ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_7(void)//�˵�2_7
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 Row;
	u32 i;
	u32 j=0;
	u64 x;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"���ܱ����\n\r"
																			"��  ��:\n\r"
																			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+2*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<=5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)MeterList[i]);
		}
		//�������
		p16=(u16*)ADDR_DATABUFF;
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v2-1)+2,2);
		if(p16[0])
		{//������Ų�Ϊ0ʱ����ʾ��Ӧ����Ϣ
			Row=3;
			p=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
			bcd_p8(p,p16[0],4);
			//ͨ�����ʼ��˿ںš�ͨ��Э������
			Row++;
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v2-1)+4,2);
			p=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
			bcd_p8(p,p8[0] & 0x1F,2);
			x=hex_bcd(p8[0] & 0x1F);
			switch(x)
			{
				case 1:
					DisplayString(Row,10,0,":����");			
					break;
				case 2:
					DisplayString(Row,10,0,":485I");			
					break;
				case 3:
					DisplayString(Row,10,0,":485II");			
					break;
				case 31:
					DisplayString(Row,10,0,":�ز�");			
					break;
				default:
					break;
			}
			Row++;
			switch(p8[1])
			{
				case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
					j++;
				case 30://DL/T 645-2007
					j++;
				case 21://�Ϻ���Լ
					j++;
				case 3://698.45
					j++;
				case 2://��������װ��ͨ��Э��
					j++;
				case 1://DL/T 645-1997
					j++;
				case 0://�ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
					break;
				default:
					j=6;
					break;
			}
			DisplayString(Row,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
			//ͨ�ŵ�ַ
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+8*2);
			x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v2-1)+6,6);
			DisplayData(x,12,0,p16);
			//����ż�С���
			Row++;
			x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v2-1)+26,1);
			p=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
			bcd_p8(p,x>>4,2);
			Row++;
			p=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
			bcd_p8(p,x&0x0f,2);
		}
	}
	else
	{
		DisplayString(3,4,0,"��Ч�������");	
	}
}
#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_7
// Descriptions:        ���ܱ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_7(void)//�˵�2_7
{
	u8 *p8;
	u16 *p16;
	u16 *p;
	u32 i;
	u32 j=0;
	u64 x;
	u32 Ln;
#if (USER/100)==13//������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_7);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+5);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<=3;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)MeterList[i]);
		}
		p=(u16*)ADDR_DATABUFF;
		for(i=1;i<=MaxRS485AddCarrierPn;i++)
		{
			MR(ADDR_DATABUFF,ADDR_AFN04F10+27*(i-1)+2,2);
			if(p[0]==MenuCount->v2)
			{//�ҵ��ò�����
				break;
			}
		}
		if(i!=MaxRS485AddCarrierPn+1)
		{//������Ŵ���
			//�ֱ��
			p8=(u8 *)ADDR_DATABUFF;						                              
			MC(0,ADDR_DATABUFF+12,1);
			MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(i-1),12);
			DisplayString(Ln+2,8,0,p8);//��ʾ�ַ���
			//ͨ�����ʼ��˿ںš�ͨ��Э������
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F10+27*(i-1)+4,2);
			x=hex_bcd(p8[0] & 0x1F);
			switch(x)
			{
				case 1:
					DisplayString(Ln+3,10,0,":����");			
					break;
				case 2:
					DisplayString(Ln+3,10,0,":485I");			
					break;
				case 3:
					DisplayString(Ln+3,10,0,":485II");			
					break;
				case 31:
					DisplayString(Ln+3,10,0,":�ز�");			
					break;
				default:
					break;
			}
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+8*2);
			DisplayData_OffZero(x,2,0,p16);
#if ((Project/100)==3 && (USER/100)==10)//�ӱ�ר��
			switch(p8[1])
			{
				case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
					j++;
				case 30://DL/T 645-2007
					j++;
				case 7://��ʤ��Լ
					j++;
				case 2://��������װ��ͨ��Э��
					j++;
				case 1://DL/T 645-1997
					j++;
				case 0://�ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
					break;
				default:
					j=6;
					break;
			}
#elif ((Project/100)==3 && (USER/100)==12)//�Ĵ�ר��
			switch(p8[1])
			{
				case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
					j++;
				case 30://DL/T 645-2007
					j++;
				case 4://�����Լ
					j++;
				case 2://��������װ��ͨ��Э��
					j++;
				case 1://DL/T 645-1997
					j++;
				case 0://�ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
					break;
				default:
					j=6;
					break;
			}
#else
			switch(p8[1])
			{
				case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
					j++;
				case 30://DL/T 645-2007
					j++;
				case 2://��������װ��ͨ��Э��
					j++;
				case 1://DL/T 645-1997
					j++;
				case 0://�ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
					break;
				default:
					j=5;
					break;
			}
#endif
			DisplayString(Ln+4,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
			//ͨ�ŵ�ַ
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+8*2);
			x=MRR(ADDR_AFN04F10+27*(i-1)+6,6);
			DisplayData(x,12,0,p16);
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_6_2
// Descriptions:        Kv Ki����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_6_2(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_6_2);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+3);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<=1;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)KvKiList[i]);
		}
		//��ѹ����������Kv
		x=MRR(ADDR_AFN04F25+11*(MenuCount->v2-1),2);			
		y=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+4*2);
		DisplayData_OffZero(y,5,0,p16);
		//��������������Ki
		x=MRR(ADDR_AFN04F25+11*(MenuCount->v2-1)+2,2);			
		y=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+4*2);
		DisplayData_OffZero(y,5,0,p16);
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}
#endif

#if ((Project/100)==3)//ר���ն�
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_6_1
// Descriptions:        Kp����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_6_1(void)
{
#if MaxImpInPort
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_6_1);//��ʾ�˵�
			if(MenuCount->v2>MaxImpInPort)
			{//MenuCount->v2��¼��������˿ں�
				MenuCount->v2=MaxImpInPort;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(1,9,1,MaxImpInPort);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+2);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxImpInPort && MenuCount->v2>=1)
	{
		DisplayString(Ln+2,0,0,"Kp:");
		//�����Kp
		x=MRR(ADDR_AFN04F11+5*(MenuCount->v2-1)+3,2);			
		y=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+4*2);
		DisplayData_OffZero(y,5,0,p16);
	}
	else
	{
		DisplayString(Ln+2,5,0,"��Ч�����");	
	}
#endif
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_6
// Descriptions:        Kv Ki Kp����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_6(void)//�˵�2_6
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_6);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_2_6_1);
						break;
					case 2:
						MenuCreate((u32)Menu_2_6_2);
						break;
					default:
						break;
				}                
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}

#if((USER/100)!=5)//���Ϻ��û�
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_5
// Descriptions:        �µ�ز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_5(void)//�˵�2_5
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 z;
#if (USER/100)==13//������
	u32 Row=3;
#else
	u32 Row=2;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_5);//��ʾ�˵�
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Row-2+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Row-2+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Row,Row+4);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Row+i,0,0,(u8 *)MonthEQList[i]);
		}
		//�µ�ض�ֵ
		y=MRR(ADDR_AFN04F46+5*(MenuCount->v2-1),4);				
		if(y==0xEEEEEEEE)
		{//�޴�������-------
			DisplayString(Row,9,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+Row*84+10*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)QUnitList[(y>>30)&0x01]);//0 kWh 1 MWh
		}
		Row++;
		//��������ϵ��
		y=MRR(ADDR_AFN04F46+5*(MenuCount->v2-1)+4,1);				
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
		p16=DisplayData_OffZero(y,2,0,p16);
		DisplayString(Row,13,0,(u8 *)UnitList[6]);//6 %
		Row++;
		//�µ����ض�ֵ����ϵ��
		y=MRR(ADDR_AFN04F20,1);			                  
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(14*2));
		DisplayString(Row,10,0,(u8 *)S0List[y>>7]);//�ϸ� �¸�
		y&=0x7f;
		p16=DisplayData_OffZero(y,2,0,p16);
		for(i=15;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(Row,i,0,(u8 *)UnitList[6]);//6 %
		Row++;
		//�����ܼ��й��ܵ�����
		y=MRR(GetClass1DataAddr(21,MenuCount->v2)+1,4);	       
//		y=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*(MenuCount->v2-1))+1,4);
		if(y==0xEEEEEEEE)
		{//�޴�������-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+Row*84+10*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(Row,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+Row*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Row,i,0,(u8 *)QUnitList[(y>>30)&0x01]);//0 kWh 1 MWh
		}
		Row++;
		//�µ�Ͷ���ִ�
		x=MRR(ADDR_AFN04F48+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//�ܿ�
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
	}
	else
	{
		DisplayString(Row,4,0,"��Ч�ܼ����");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_4
// Descriptions:        Ӫҵ��ͣ�ز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_4(void)//�˵�2_4
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 Ln;
#if (USER/100)==13//������
	Ln=1;
#else
	Ln=0;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_4);//��ʾ�˵�
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+5);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=3;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)BusinessQuitList[i]);
		}
		//Ӫҵ��ͣ�ع��ʶ�ֵ
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1)+6,2);			
		if(y==0xEEEE)
		{//�޴�������-------
			DisplayString(Ln+2,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+((Ln+2)*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(Ln+2,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Ln+2,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//��ʼʱ�� ������
		p16=(u16 *)(ADDR_STRINGBUFF+4+((Ln+3)*84)+(10*2));
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1)+1,2);			
		DisplayData1(y,4,2,'-',p16);
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1),1);			
		DisplayString(Ln+3,15,0,"-");
		DisplayData(y,2,0,p16+6);
		//����ʱ�� ������
		p16=(u16 *)(ADDR_STRINGBUFF+4+((Ln+4)*84)+(10*2));
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1)+4,2);
		DisplayData1(y,4,2,'-',p16);
		y=MRR(ADDR_AFN04F44+8*(MenuCount->v2-1)+3,1);
		DisplayString(4,15,0,"-");
		DisplayData(y,2,0,p16+6);
		//����Ͷ���ִ�
		x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+((Ln+5)*84)+(10*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//�ܿ�
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�ܼ����");	
	}
}
#endif

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3
// Descriptions:        �����¸��ز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_3(void)//�˵�2_3
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_3);//��ʾ�˵�
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+2*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)PowerLowerList[i]);
		}
		//�¸�ϵ��
		y=MRR(ADDR_AFN0CF6+3+8*(MenuCount->v2-1)+2,1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(14*2));
		DisplayString(3,10,0,(u8 *)S0List[y>>7]);//�ϸ� �¸�
		y&=0x7f;
		p16=DisplayData_OffZero(y,2,0,p16);
		for(i=15;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(3,i,0,(u8 *)UnitList[6]);//6 %
		//��ǰ����
		y=MRR(ADDR_AFN0CF17+2*(MenuCount->v2-1),2);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(4*84)+(10*2));
		p16 = PowerP16(y,p16);
		DisplayString(4,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+4*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(4,i,0,(u8 *)UnitList[0]);//0 kW
		//�¸��ؿ���ʱ��
		y=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+4,1);			
		z=hex_bcd(y/2);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(5*84)+(10*2));
		p16=DisplayData_OffZero(z,3,0,p16);
		if(y%2)
		{
			p16[0]=0x2000+'.';
			p16[1]=0x2000+'5';
			for(i=11;i<20;i++)
			{
				if(p16+2 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
				{
					break;
				}
			}
		}
		else
		{
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
				{
					break;
				}
			}
		}
		DisplayString(5,i,0,(u8 *)UnitList[7]);//7 h
		//����Ͷ���ִ�
		x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(6*84)+(10*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//�ܿ�
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
		//��1�澯ʱ��
		x=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+4,1);
		y=hex_bcd(x);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(7*84)+(10*2));
		p16=DisplayData_OffZero(y,3,0,p16);
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(7,i,0,(u8 *)UnitList[8]);//8 min
		//��2�澯ʱ��
		x=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+5,1);
		y=hex_bcd(x);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(8*84)+(10*2));
		p16=DisplayData_OffZero(y,3,0,p16);
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(8,i,0,(u8 *)UnitList[8]);//8 min
	}
	else
	{
		DisplayString(3,4,0,"��Ч�ܼ����");	
	}
}
#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3
// Descriptions:        �����¸��ز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_3(void)//�˵�2_3
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_3);//��ʾ�˵�
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PowerLowerList[i]);
		}
		//��ǰ���ض�ֵ
		y=MRR(ADDR_AFN0CF6+3+8*(MenuCount->v2-1),2);			
		if(y==0xEEEE)
		{//�޴�������-------
			DisplayString(2,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(10*2));
			p16 = PowerP16(y,p16);
			DisplayString(2,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+2*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(2,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//�¸�ϵ��
		y=MRR(ADDR_AFN0CF6+3+8*(MenuCount->v2-1)+2,1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(14*2));
		DisplayString(3,10,0,(u8 *)S0List[y>>7]);//�ϸ� �¸�
		y&=0x7f;
		p16=DisplayData_OffZero(y,2,0,p16);
		for(i=15;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(3,i,0,(u8 *)UnitList[6]);//6 %
		//��ǰ����
		y=MRR(ADDR_AFN0CF17+2*(MenuCount->v2-1),2);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(4*84)+(10*2));
		p16 = PowerP16(y,p16);
		DisplayString(4,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+4*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(4,i,0,(u8 *)UnitList[0]);//0 kW
		//�¸��ؿ���ʱ��
		y=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+4,1);			
		z=hex_bcd(y/2);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(5*84)+(10*2));
		p16=DisplayData_OffZero(z,3,0,p16);
		if(y%2)
		{
			p16[0]=0x2000+'.';
			p16[1]=0x2000+'5';
			for(i=11;i<20;i++)
			{
				if(p16+2 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
				{
					break;
				}
			}
		}
		else
		{
			for(i=11;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
				{
					break;
				}
			}
		}
		DisplayString(5,i,0,(u8 *)UnitList[7]);//7 h
		//����Ͷ���ִ�
		x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(6*84)+(10*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//�ܿ�
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
		//��1�澯ʱ��
		x=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+4,1);
		y=hex_bcd(x);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(7*84)+(10*2));
		p16=DisplayData_OffZero(y,3,0,p16);
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(7,i,0,(u8 *)UnitList[8]);//8 min
		//��2�澯ʱ��
		x=MRR(ADDR_AFN05F12+9*(MenuCount->v2-1)+5,1);
		y=hex_bcd(x);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(8*84)+(10*2));
		p16=DisplayData_OffZero(y,3,0,p16);
		for(i=11;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(8,i,0,(u8 *)UnitList[8]);//8 min
	}
	else
	{
		DisplayString(2,4,0,"��Ч�ܼ����");	
	}
}
#endif						  

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_2
// Descriptions:        ���ݿز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_2(void)//�˵�2_2
{
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 i;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_2);//��ʾ�˵�
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+6);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)WorkersDayOffList[i]);
		}
		//���ݿض�ֵ
		y=MRR(ADDR_AFN04F42+6*(MenuCount->v2-1),2);			
		if(y==0xEEEE)
		{//�޴�������-------
			DisplayString(Ln+2,12,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+(12*2));
			p16 = PowerP16(y,p16);
			DisplayString(Ln+2,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
			for(i=13;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(Ln+2,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//�޵���ʼʱ��
		y=MRR(ADDR_AFN04F42+6*(MenuCount->v2-1)+2,2);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+(12*2));
		DisplayData1(y,4,2,':',p16);
		//�޵�����ʱ��
		x=MRR(ADDR_AFN04F42+6*(MenuCount->v2-1)+4,1);			
		y=hex_bcd(x/2);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+(12*2));
		p16=DisplayData_OffZero(y,2,0,p16);
		if(x%2)
		{
			p16[0]=0x2000+'.';
			p16[1]=0x2000+'5';
			for(i=13;i<20;i++)
			{
				if(p16+2 == (u16 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+i*2))
				{
					break;
				}
			}
		}
		else
		{
			for(i=13;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+i*2))
				{
					break;
				}
			}
		}
		DisplayString(Ln+4,i,0,(u8 *)UnitList[7]);//7 h
		//ÿ���޵���
		x=MRR(ADDR_AFN04F42+6*(MenuCount->v2-1)+5,1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+(12*2));
		for(i=1;i<=7;i++)
		{
			if((x>>i)&0x01)
			{
				p16[i-1]=i+0x2030;
			}
			else
			{
				p16[i-1]=0x2000+'-';
			}
		}
		//����Ͷ���ִ�
		x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+(12*2));
		for(i=0;i<=7;i++)
		{
			if(((x>>i)&0x01)==1)
			{//�ܿ�
				p16[i]=i+0x2031;
			}
			else
			{
				p16[i]=0x2000+'-';
			}
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�ܼ����");	
	}
}

	#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_1
// Descriptions:        ʱ�οز�������1 2 3
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1_1(void)
{
	u8 *a;
	u8 *b;
	u8 *c;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 Ln;
	u32 m=0;
	u32 n=0;
	u32 N;//������0 1 2
	u32 x;
	u32 y;
	u32 l;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
	u8 * p8d;
#endif

//������
	N = (Stack->MenuStack[MenuCount->Count-1].EditRow)-1;
//����ʱ�� Ŀǰ��������Ϊһ�������Ŀ���ʱ��
	p8=(u8 *)ADDR_DATABUFF;
	a=(u8 *)(ADDR_DATABUFF+200);//ʱ����ʼλ��
	b=(u8 *)(ADDR_DATABUFF+250);//ʱ�ν���λ��
	c=(u8 *)(ADDR_DATABUFF+300);//ʱ�ζ�Ӧ�Ŀ��Ʒ�ʽ������1������2��������
	MR(ADDR_DATABUFF,ADDR_AFN04F18,12);
	for(i=0;i<12;i++)
	{
		p8[12+4*i]=p8[i]&0x03;
		p8[13+4*i]=(p8[i]>>2)&0x03;
		p8[14+4*i]=(p8[i]>>4)&0x03;
		p8[15+4*i]=(p8[i]>>6)&0x03;
	}
	for(i=0;i<48;)
	{
		if(m>=8)
		{
			break;
		}
		else
		{
			switch(p8[12+i])
			{
				case 0:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=0)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=0;
					i=j;
					break;
				case 1:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=1)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=1;
					i=j;
					break;
				case 2:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=2)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=2;
					i=j;
					break;
				case 3:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=3)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=3;
					i=j;
					break;
			}
		}
	}
	MenuCount->v3=m;//һ���Ϊ���ٸ�ʱ��
//MenuCount->v2��¼�ܼ���� �����޸�ֵ
	if(MenuCount->v2>MaxTotalAddGroup)
	{
		MenuCount->v2=MaxTotalAddGroup;
	}
	if(MenuCount->v2<1)
	{
		MenuCount->v2=1;
	}
//�����1����ʾ�˶��ٸ����ö�ֵ��ʱ��
	p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
	if(p16[0]==0x40B8 && p16[1]==0x40B9)
	{//���½��С�
		x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
		if((x>>N)&0x01)
		{
			y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//����Nʱ�κ�
			for(j=0;j<4;j++)
			{
				if((y>>j)&0x01)
				{
					n++;
				}
			}
			MenuCount->v1=n;//��1����ʾ�˶��ٸ����ö�ֵ��ʱ��
		}
	}
	n=0;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu_SH(0,Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1_1);//��ʾ�˵�
			p16=(u16 *)(ADDR_STRINGBUFF+0*84+4+10*2);
			p16[0]=N+0x2031;
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			//ʱ�οض�ֵ����ϵ��
			x=MRR(ADDR_AFN04F19,1);			                    
			p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(13*2));
			DisplayString(2,9,0,(u8 *)S0List[x>>7]);//�ϸ� �¸�
			x&=0x7f;
			p16=DisplayData_OffZero(x,2,0,p16);
			for(i=14;i<17;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+2*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(2,i,0,(u8 *)UnitList[6]);//6 %
			//����Ͷ���ִ�
			x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			  
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(9*2));
			for(i=0;i<=7;i++)
			{
				if(((x>>i)&0x01)==1)
				{//�ܿ�
					p16[i]=i+0x2031;
				}
				else
				{
					p16[i]=0x2000+'-';
				}
			}
			
			if(MenuCount->v3<=4)
			{
				for(j=0;j<MenuCount->v3;j++)
				{
					Ln=5+j;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
					if(c[j])
					{//����1,����2,����
						p16[11]=34;// '��'
					}
					x=hex_bcd(a[j]>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if((a[j]&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
					x=hex_bcd((b[j]+1)>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if(((b[j]+1)&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
				}

				x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
				switch((x>>N)&0x01)
				{
					case 0:
						for(j=0;j<MenuCount->v3;j++)
						{
							Ln=5+j;
							DisplayString(Ln,13,0,"��");	
						}
						break;
					case 1:
						y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//����Nʱ�κ�
						for(j=0;j<MenuCount->v3;j++)
						{
							switch((y>>j)&0x01)
							{
								case 0:
									Ln=5+j;
									DisplayString(Ln,13,0,"��");	
									break;
								case 1:
									n++;
									Ln=5+j;
									p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
									l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(n-1),2);//����Nʱ��n���ʶ�ֵ
									if(l==0xEEEE)
									{//�޴�������-------
										DisplayString(Ln,13,0,"-------");	
									}
									else
									{
										p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
										PowerP16(l,p16);
										DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
									}
									break;
								}
						}
						break;
				}
			}
			else
			{
				DisplayString(9,18,0,"��");
				for(j=0;j<4;j++)
				{
					Ln=5+j;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
					if(c[j])
					{//����1,����2,����
						p16[11]=34;// '��'
					}
					x=hex_bcd(a[j]>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if((a[j]&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
					x=hex_bcd((b[j]+1)>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if(((b[j]+1)&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
				}

				x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
				switch((x>>N)&0x01)
				{
					case 0:
						for(j=0;j<4;j++)
						{
							Ln=5+j;
							DisplayString(Ln,13,0,"��");	
						}
						break;
					case 1:
						y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//����Nʱ�κ�
						for(j=0;j<4;j++)
						{
							switch((y>>j)&0x01)
							{
								case 0:
									Ln=5+j;
									DisplayString(Ln,13,0,"��");	
									break;
								case 1:
									n++;
									Ln=5+j;
									p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
									l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(n-1),2);//����Nʱ��n���ʶ�ֵ
									if(l==0xEEEE)
									{//�޴�������-------
										DisplayString(Ln,13,0,"-------");	
									}
									else
									{
										p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
										PowerP16(l,p16);
										DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
									}
									break;
							}
						}
						break;
				}
			}
			break;
		case 1://����1:ѡ��
			x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
				if(p16[0]==0x40B8 && p16[1]==0x40B9)
				{//���½��С�
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//�¼�
						DisplayClrRow(2,9);
						DisplayString(0,18,0,"��");
						for(j=4;j<MenuCount->v3;j++)
						{
							Ln=j-2;
							p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
							if(c[j])
							{//����1,����2,����
								p16[11]=34;// '��'
							}
							x=hex_bcd(a[j]>>1);
							DisplayData(x,2,0,p16);
							p16[2]=0x2000+':';
							if((a[j]&0x01)==0)
							{
								p16[3]=0x2030;
								p16[4]=0x2030;
							}
							else
							{
								p16[3]=0x2033;
								p16[4]=0x2030;
							}
							p16[5]=0x2000+'-';
							p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
							x=hex_bcd((b[j]+1)>>1);
							DisplayData(x,2,0,p16);
							p16[2]=0x2000+':';
							if(((b[j]+1)&0x01)==0)
							{
								p16[3]=0x2030;
								p16[4]=0x2030;
							}
							else
							{
								p16[3]=0x2033;
								p16[4]=0x2030;
							}
						}

						x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
						switch((x>>N)&0x01)
						{
							case 0:
								for(j=4;j<MenuCount->v3;j++)
								{
									Ln=j-2;
									DisplayString(Ln,13,0,"��");	
								}
								break;
							case 1:
								y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//����Nʱ�κ�
								for(j=4;j<MenuCount->v3;j++)
								{
									switch((y>>j)&0x01)
									{
										case 0:
											Ln=j-2;
											DisplayString(Ln,13,0,"��");	
											break;
										case 1:
											n++;
											Ln=j-2;
											p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
											l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(MenuCount->v1+n-1),2);//����Nʱ��MenuCount->v1+n���ʶ�ֵ
											if(l==0xEEEE)
											{//�޴�������-------
												DisplayString(Ln,13,0,"-------");	
											}
											else
											{
												p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
												PowerP16(l,p16);
												DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
											}
											break;
									}
								}
								break;
						}
					}
				}
				p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
				if(p16[0]==0x40B6 && p16[1]==0x40B7)
				{//���Ͻ��С�
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//��
						Stack->MenuStack[MenuCount->Count].Task=0;
					}
				}
				Comm_Ram->ButtonVal&=~0x03;//��ʹ�ú���0
			}
			NdigitSelect_SH(1,1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
				p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
				p8d[0]=1;//�˵���ʾ���������ã�0=��1=��
#endif
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
	#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1_1
// Descriptions:        ʱ�οز�������1 2 3
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1_1(void)
{
	u8 *a;
	u8 *b;
	u8 *c;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 Ln;
	u32 m=0;
	u32 n=0;
	u32 N;//������0 1 2
	u32 x;
	u32 y;
	u32 l;

//������
	N = (Stack->MenuStack[MenuCount->Count-1].EditRow)-1;
//����ʱ�� Ŀǰ��������Ϊһ�������Ŀ���ʱ��
	p8=(u8 *)ADDR_DATABUFF;
	a=(u8 *)(ADDR_DATABUFF+200);//ʱ����ʼλ��
	b=(u8 *)(ADDR_DATABUFF+250);//ʱ�ν���λ��
	c=(u8 *)(ADDR_DATABUFF+300);//ʱ�ζ�Ӧ�Ŀ��Ʒ�ʽ������1������2��������
	MR(ADDR_DATABUFF,ADDR_AFN04F18,12);
	for(i=0;i<12;i++)
	{
		p8[12+4*i]=p8[i]&0x03;
		p8[13+4*i]=(p8[i]>>2)&0x03;
		p8[14+4*i]=(p8[i]>>4)&0x03;
		p8[15+4*i]=(p8[i]>>6)&0x03;
	}
	for(i=0;i<48;)
	{
		if(m>=8)
		{
			break;
		}
		else
		{
			switch(p8[12+i])
			{
				case 0:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=0)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=0;
					i=j;
					break;
				case 1:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=1)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=1;
					i=j;
					break;
				case 2:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=2)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=2;
					i=j;
					break;
				case 3:
					m++;
					for(j=i+1;j<48;j++)
					{
						if(p8[12+j]!=3)
						{
							break;
						}
					}
					a[m-1]=i;
					b[m-1]=j-1;
					c[m-1]=3;
					i=j;
					break;
			}
		}
	}
	MenuCount->v3=m;//һ���Ϊ���ٸ�ʱ��
//MenuCount->v2��¼�ܼ���� �����޸�ֵ
	if(MenuCount->v2>MaxTotalAddGroup)
	{
		MenuCount->v2=MaxTotalAddGroup;
	}
	if(MenuCount->v2<1)
	{
		MenuCount->v2=1;
	}
//�����1����ʾ�˶��ٸ����ö�ֵ��ʱ��
	p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
	if(p16[0]==0x40B8 && p16[1]==0x40B9)
	{//���½��С�
		x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
		if((x>>N)&0x01)
		{
			y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//����Nʱ�κ�
			for(j=0;j<4;j++)
			{
				if((y>>j)&0x01)
				{
					n++;
				}
			}
			MenuCount->v1=n;//��1����ʾ�˶��ٸ����ö�ֵ��ʱ��
		}
	}
	n=0;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1_1);//��ʾ�˵�
			p16=(u16 *)(ADDR_STRINGBUFF+0*84+4+10*2);
			p16[0]=N+0x2031;
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			//ʱ�οض�ֵ����ϵ��
			x=MRR(ADDR_AFN04F19,1);			                    
			p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(13*2));
			DisplayString(2,9,0,(u8 *)S0List[x>>7]);//�ϸ� �¸�
			x&=0x7f;
			p16=DisplayData_OffZero(x,2,0,p16);
			for(i=14;i<17;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+2*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(2,i,0,(u8 *)UnitList[6]);//6 %
			//����Ͷ���ִ�
			x=MRR(ADDR_AFN04F45+1*(MenuCount->v2-1),1);			  
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(9*2));
			for(i=0;i<=7;i++)
			{
				if(((x>>i)&0x01)==1)
				{//�ܿ�
					p16[i]=i+0x2031;
				}
				else
				{
					p16[i]=0x2000+'-';
				}
			}
			
			if(MenuCount->v3<=4)
			{
				for(j=0;j<MenuCount->v3;j++)
				{
					Ln=5+j;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
					if(c[j])
					{//����1,����2,����
						p16[11]=34;// '��'
					}
					x=hex_bcd(a[j]>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if((a[j]&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
					x=hex_bcd((b[j]+1)>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if(((b[j]+1)&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
				}

				x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
				switch((x>>N)&0x01)
				{
					case 0:
						for(j=0;j<MenuCount->v3;j++)
						{
							Ln=5+j;
							DisplayString(Ln,13,0,"��");	
						}
						break;
					case 1:
						y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//����Nʱ�κ�
						for(j=0;j<MenuCount->v3;j++)
						{
							switch((y>>j)&0x01)
							{
								case 0:
									Ln=5+j;
									DisplayString(Ln,13,0,"��");	
									break;
								case 1:
									n++;
									Ln=5+j;
									p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
									l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(n-1),2);//����Nʱ��n���ʶ�ֵ
									if(l==0xEEEE)
									{//�޴�������-------
										DisplayString(Ln,13,0,"-------");	
									}
									else
									{
										p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
										PowerP16(l,p16);
										DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
									}
									break;
								}
						}
						break;
				}
			}
			else
			{
				DownArrow();
				for(j=0;j<4;j++)
				{
					Ln=5+j;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
					if(c[j])
					{//����1,����2,����
						p16[11]=34;// '��'
					}
					x=hex_bcd(a[j]>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if((a[j]&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
					x=hex_bcd((b[j]+1)>>1);
					DisplayData(x,2,0,p16);
					p16[2]=0x2000+':';
					if(((b[j]+1)&0x01)==0)
					{
						p16[3]=0x2030;
						p16[4]=0x2030;
					}
					else
					{
						p16[3]=0x2033;
						p16[4]=0x2030;
					}
				}

				x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
				switch((x>>N)&0x01)
				{
					case 0:
						for(j=0;j<4;j++)
						{
							Ln=5+j;
							DisplayString(Ln,13,0,"��");	
						}
						break;
					case 1:
						y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//����Nʱ�κ�
						for(j=0;j<4;j++)
						{
							switch((y>>j)&0x01)
							{
								case 0:
									Ln=5+j;
									DisplayString(Ln,13,0,"��");	
									break;
								case 1:
									n++;
									Ln=5+j;
									p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
									l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(n-1),2);//����Nʱ��n���ʶ�ֵ
									if(l==0xEEEE)
									{//�޴�������-------
										DisplayString(Ln,13,0,"-------");	
									}
									else
									{
										p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
										PowerP16(l,p16);
										DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
									}
									break;
							}
						}
						break;
				}
			}
			break;
		case 1://����1:ѡ��
			x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
				if(p16[0]==0x40B8 && p16[1]==0x40B9)
				{//���½��С�
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//�¼�
						DisplayClrRow(2,9);
						UpArrow();
						for(j=4;j<MenuCount->v3;j++)
						{
							Ln=j-2;
							p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
							if(c[j])
							{//����1,����2,����
								p16[11]=34;// '��'
							}
							x=hex_bcd(a[j]>>1);
							DisplayData(x,2,0,p16);
							p16[2]=0x2000+':';
							if((a[j]&0x01)==0)
							{
								p16[3]=0x2030;
								p16[4]=0x2030;
							}
							else
							{
								p16[3]=0x2033;
								p16[4]=0x2030;
							}
							p16[5]=0x2000+'-';
							p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(6*2));
							x=hex_bcd((b[j]+1)>>1);
							DisplayData(x,2,0,p16);
							p16[2]=0x2000+':';
							if(((b[j]+1)&0x01)==0)
							{
								p16[3]=0x2030;
								p16[4]=0x2030;
							}
							else
							{
								p16[3]=0x2033;
								p16[4]=0x2030;
							}
						}

						x=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
						switch((x>>N)&0x01)
						{
							case 0:
								for(j=4;j<MenuCount->v3;j++)
								{
									Ln=j-2;
									DisplayString(Ln,13,0,"��");	
								}
								break;
							case 1:
								y=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N),1);//����Nʱ�κ�
								for(j=4;j<MenuCount->v3;j++)
								{
									switch((y>>j)&0x01)
									{
										case 0:
											Ln=j-2;
											DisplayString(Ln,13,0,"��");	
											break;
										case 1:
											n++;
											Ln=j-2;
											p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(0*2));
											l=MRR(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1))+1+17*(N)+1+2*(MenuCount->v1+n-1),2);//����Nʱ��MenuCount->v1+n���ʶ�ֵ
											if(l==0xEEEE)
											{//�޴�������-------
												DisplayString(Ln,13,0,"-------");	
											}
											else
											{
												p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
												PowerP16(l,p16);
												DisplayString(Ln,12,0,(u8 *)SList[(l>>12)&0x01]);//+ -
											}
											break;
									}
								}
								break;
						}
					}
				}
				p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
				if(p16[0]==0x40B6 && p16[1]==0x40B7)
				{//���Ͻ��С�
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//��
						Stack->MenuStack[MenuCount->Count].Task=0;
					}
				}
				Comm_Ram->ButtonVal&=~0x03;//��ʹ�ú���0
			}
			NdigitSelect(1,9,1,MaxTotalAddGroup);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
	#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1
// Descriptions:        ʱ�οز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1(void)
{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
	u8 * p8;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow>=1 && Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow<=3)
				{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
					p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
					p8[0]=0;//�˵���ʾ���������ã�0=��1=��
#endif
					MenuCreate((u32)Menu_2_1_1);
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
#endif

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2
// Descriptions:        ������ֵ�˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2(void)//�˵�2
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"������ֵ\n\r"
																			"  1.ʱ�οز���\n\r"
																			"  2.���ݿز���\n\r"
																			"  3.�¸��ز���\n\r"
																			"  4.Kv Ki Kp\n\r"
																			"  5.���ܱ����\n\r"
																			"  6.���ò���\n\r"
																			);//��ʾ�˵�
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_2_1);
						break;
					case 2:
						MenuCreate((u32)Menu_2_2);
						break;
					case 3:
						MenuCreate((u32)Menu_2_3);
						break;
					case 4:
						MenuCreate((u32)Menu_2_6);
						break;
					case 5:
						MenuCreate((u32)Menu_2_7);
						break;
					case 6:
						//���±�̼���������ȷ���룬���ɽ������ò˵�
						//����ģʽ�£���ʱ�ɽ������ò˵�
						if(Comm_Ram->Factory==0x55)
						{//����״̬
							MenuCreate((u32)Menu_8);//�����¼��˵�
						}
						else
						{
//								if(MRR(ADDR_MeterFMTimer,2))
//								{//���±�̼�
								PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//�����¼��˵�
//								}
						}
						break;
					default:
						break;
				}                
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2
// Descriptions:        ������ֵ�˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2(void)//�˵�2
{
	#if ((Project/100)==3)//ר���ն�
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu3_2);//��ʾ�˵�
				Arrowhead();
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
				break;
			case 1://����1:ѡ��
				MenuSelect();//�˵�ѡ��
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_2_1);
							break;
						case 2:
							MenuCreate((u32)Menu_2_2);
							break;
						case 3:
							MenuCreate((u32)Menu_2_4);
							break;
						case 4:
							MenuCreate((u32)Menu_2_3);
							break;
						case 5:
							MenuCreate((u32)Menu_2_5);
							break;
						case 6:
							MenuCreate((u32)Menu_2_6);
							break;
						case 7:
							MenuCreate((u32)Menu_2_7);
							break;
						case 8:
							//���±�̼���������ȷ���룬���ɽ������ò˵�
							//����ģʽ�£���ʱ�ɽ������ò˵�
							if(Comm_Ram->Factory==0x55)
							{//����״̬
								MenuCreate((u32)Menu_8);//�����¼��˵�
							}
							else
							{
//								if(MRR(ADDR_MeterFMTimer,2))
//								{//���±�̼�
									PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//�����¼��˵�
//								}
							}
							break;
						#if (USER/100)==13//������
						case 9:
							//���±�̼���������ȷ���룬���ɽ������ò˵�
							//����ģʽ�£���ʱ�ɽ������ò˵�
							if(Comm_Ram->Factory==0x55)
							{//����״̬
								MenuCreate((u32)Menu_9);//�����¼��˵�
							}
							else
							{
//								if(MEMReadReg(ADDR_MeterFMTimer,2))
//								{//���±�̼�
									PassWordCreateEdit();
//								}
//								else
//								{
//									MenuCreate((u32)Menu_0);//�����¼��˵�
//								}
							}
							break;
						#endif
						default:
							break;
					}                
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
					MenuDel();//�����ϼ��˵�
				}
				Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
				break;
			default:
				break;
		}
	#endif
	#if ((Project/100)==4)//�����ն�
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu4_2);//��ʾ�˵�
				Arrowhead();
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
				break;
			case 1://����1:ѡ��
				MenuSelect();//�˵�ѡ��
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_2_6_2);
							break;
						case 2:
							MenuCreate((u32)Menu_2_7);
							break;
						default:
							break;
					}                
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
					MenuDel();//�����ϼ��˵�
				}
				Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
				break;
			default:
				break;
		}
	#endif
}
#endif
