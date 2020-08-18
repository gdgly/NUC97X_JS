
//�˵�2 ������ֵ
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"

//#include "../Terminal/Terminal_AFN0C_RealTimeData_Fn.h"

extern const u8 UnitList[][6];//ͨ�õ�λ�б�
extern const u8 ProtocolList[][13];
extern const u8 SList[][2];//���������б�

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
	"  3.��ͣ�ز���\n\r"
	"  4.�¸��ز���\n\r"
	"  5.�µ�ز���\n\r"
	"  6.kv ki kp����\n\r"
	"  7.���ܱ����\n\r"
	"  8.���ò���\n\r"
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
	"��ֵ:",
	"��ʼʱ��:",
	" ",
	"����ʱ��:",
	"ÿ���޵���:",
	"Ͷ���ִ�:",
};

const u8 pMenu_2_3[]=
{
	"�����¸��ز���\n\r"
	"�ܼ���\n\r"
};

const u8 PowerLowerList[][20]=//�����¸����б�
{
	"��ǰ:",
	"����ϵ��:",
	"��ֵ:",
	"����ʱ��:",
	"Ͷ���ִ�:",
	"��1�澯 :",
	"��2�澯 :",
};

const u8 pMenu_2_4[]=
{
	"Ӫҵ��ͣ�ز���\n\r"
	"�ܼ���\n\r"
};

const u8 BusinessQuitList[][20]=//Ӫҵ��ͣ�ز����б�
{
	"��ֵ:",
	"��ʼʱ��:",
	" ",
	"����ʱ��:",
	" ",
	"Ͷ���ִ�:",
};

const u8 pMenu_2_5[]=
{
	"�µ�ز���\n\r"
	"�ܼ���\n\r"
};

const u8 MonthEQList[][20]=//�µ�ز����б�
{
	"��ֵ:",
	"����ϵ��:",
	"��ֵϵ��:",
	"�����õ���:",
	"",
	"Ͷ���ִ�:",
};

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
	"����\n\r"
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

const u8 MeterList[][20]=//���ܱ�����б�
{
	"�ֱ��:",
	"�˿�  :",
	"Э��  :",
	"���ַ:",
};

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
/*
const u8 pMenu_2_3[]=
{
	"�������в���\n\r"
	"  1.����ʱ������\n\r"
	"  2.������\n\r"
};
*/
const u8 MeterReadingDateList[][20]=//�������б�
{
	"������",
	"1 2 3 4 5 6 7 8 9",
	"10 11 12 13 14 15",
	"16 17 18 19 20 21",
	"22 23 24 25 26 27",
	"28 29 30 31",
};
#if ((Project/100)==3)||((Project/100)==4)//ר���ն�

//D7 �� D0 ��D6 �� D1 ��������λ��
u32 Bit_String_Change(u32 data)
{
	u32 i;
	u32 j=0;
	
	for(i=0;i<8;i++)
	{
		if(data&0x01)
		{
			j|=(1<<(7-i));
		}
		data=data>>1;
	}
	return j;
}
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
	u32 i;
	u32 j=0;
	u64 x;
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_7);//��ʾ�˵�
			if(MenuCount->v2>=NUMmax_6000)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUMmax_6000-1;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			MenuCount->v3=0;
			MR(ADDR_DATABUFF,ADDR_6000+LENper_6000*MenuCount->v2,LENper_6000);
			p8=(u8*)ADDR_DATABUFF;
			if(p8[0]==0)
			{
				MenuCount->v3=0xffff;
			}
			else
			{
				p8=Get_Element((u8*)ADDR_DATABUFF,1,0,LENper_6000);
				i=p8[2]+p8[1]*0x100;
				if(i!=MenuCount->v2)
				{
					MenuCount->v3=0xffff;
				}
			}
			
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUMmax_6000-1);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(2,5);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v2,LENper_6000);
		for(i=0;i<=3;i++)
		{
			DisplayString(2+i,0,0,(u8 *)MeterList[i]);
		}
		//p=(u16*)ADDR_DATABUFF;
		//�ֱ��
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MEMClr(0,ADDR_DATABUFF+12,1);
		//MEMRead(ADDR_DATABUFF,ADDR_AFN04F29+12*(i-1),12);
		//DisplayString(2,8,0,p8);//��ʾ�ַ���
		
		//ͨ�����ʼ��˿ںš�ͨ��Э������
		j=Get_ElementADDR(ADDR_DATABUFF+1024,2,4,LENper_6000);
		if(j!=0)
		{
			j=MRR(j+1,4);
			j=R_Inverse(j,4);
		}
		switch(j)
		{
			case 0xf2080201:
				DisplayString(3,8,0,"����");			
				break;
			case 0xf2010201:
				DisplayString(3,8,0,"485I");			
				break;
			case 0xf2010202:
				DisplayString(3,8,0,"485II");			
				break;
			case 0xf2090201:
				DisplayString(3,8,0,"�ز�");			
				break;
			default:
				DisplayString(3,8,0,"δ֪");	
				break;
		}
		//p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+8*2);
		//DisplayData_OffZero(x,2,0,p16);
		j=Get_ElementADDR(ADDR_DATABUFF+1024,2,3,LENper_6000);
		if(j!=0)
		{
			j=MRR(j+1,1);
			if(j>4)
				j=0;
		}
		DisplayString(4,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
		//ͨ�ŵ�ַ
		MC(0,ADDR_DATABUFF,12);
		x=0;
		j=Get_ElementADDR(ADDR_DATABUFF+1024,2,1,LENper_6000);
		if(j!=0)
		{
			MR(ADDR_DATABUFF,j+3,6);
			Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			x=MRR(ADDR_DATABUFF,6);
		}
		p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+7*2);
		DisplayData(x,12,0,p16);
	}
	else
	{
		DisplayString(2,4,0,"��Ч�������");	
	}
}

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

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_6_2);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_IMPIN)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUMmax_IMPIN;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			
			for(x=0;x<NUMmax_IMPIN;x++)
			{
				y=Get_ElementADDR(ADDR_240x_4+x*LENper_240x_4,1,1,LENper_240x_4);
				if(y!=0)
				{
					y=MRR(y+1,4);
					y=R_Inverse(y,4);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=x;
						break;
					}
				}
				MenuCount->v3=0xffff;
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
			NdigitSelect(4,8,1,NUMmax_IMPIN);
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
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<=1;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)KvKiList[i]);
		}
		//��ѹ����������Kv
		x=Get_ElementADDR(ADDR_240x_3+MenuCount->v3*LENper_240x_3,1,0,LENper_240x_3);
		if(x!=0)
		{
			x=MRR(x+1,2);
			x=R_Inverse(x,2);
		}		
		y=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+4*2);
		DisplayData_OffZero(y,5,0,p16);
		//��������������Ki
		x=Get_ElementADDR(ADDR_240x_3+MenuCount->v3*LENper_240x_3,2,0,LENper_240x_3);
		if(x!=0)
		{
			x=MRR(x+1,2);
			x=R_Inverse(x,2);
		}
		y=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+4*2);
		DisplayData_OffZero(y,5,0,p16);
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�����");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_6_1
// Descriptions:        Kp����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_6_1(void)
{
#if NUMmax_IMPIN
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_6_1);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_IMPIN)
			{//MenuCount->v2��¼��������˿ں�
				MenuCount->v2=NUMmax_IMPIN;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			MR(ADDR_DATABUFF,ADDR_240x_4,LENmax_240x_4);
			for(x=0;x<NUMmax_IMPIN;x++)
			{
				y=Get_ElementADDR(ADDR_240x_4+x*LENper_240x_4,1,1,LENper_240x_4);
				if(y!=0)
				{
					y=MRR(y+1,4);
					y=R_Inverse(y,4);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=x;
						break;
					}
				}
				MenuCount->v3=0xffff;
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
			NdigitSelect(1,9,1,NUMmax_IMPIN);
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
	if(MenuCount->v3!=0xffff)
	{
		DisplayString(Ln+2,0,0,"Kp:");
		//�����Kp
		x=Get_ElementADDR(ADDR_240x_4+MenuCount->v3*LENper_240x_4,1,3,LENper_240x_4);
		if(x!=0)
		{
			x=MRR(x+1,2);
			x=R_Inverse(x,2);
		}
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
	u32 Row=2;
	u64 a64;
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_5);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			MR(ADDR_DATABUFF,ADDR_8108_2,LENmax_8107_3);
			for(x=0;x<NUMmax_TAG;x++)
			{
				y=Get_ElementADDR(ADDR_DATABUFF,x+1,1,LENmax_8107_3);
				if(y!=0)
				{
					y=MRR(y+1,2);
					y=R_Inverse(y,2);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=x;
						break;
					}
				}
				MenuCount->v3=0xffff;
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
			NdigitSelect(1,9,1,NUMmax_TAG);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(2,6);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		MR(ADDR_DATABUFF,ADDR_8108_2,LENmax_8108_2);
		for(i=0;i<=5;i++)
		{
			DisplayString(2+i,0,0,(u8 *)MonthEQList[i]);
		}
		//�µ�ض�ֵ
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,2,LENmax_8108_2);
		if(y==0)
		{//�޴�������-------
			DisplayString(Row,9,0,"-------");	
		}
		else
		{
			a64=MRR(y+1,8);
			a64=R_Inverse(a64,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+Row*84+5*2);
			disp_long64_kw(a64,0,1,p16,2);
		}
		Row++;
		//��������ϵ��
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,3,LENmax_8108_2);
		if(y!=0)
		{
			y=MRR(y+1,1);
			y=hex_bcd(y);
		}
		
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
		p16=DisplayData_OffZero(y,2,0,p16);
		DisplayString(Row,13,0,(u8 *)UnitList[6]);//6 %
		Row++;
		//�µ����ض�ֵ����ϵ��
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,4,LENmax_8108_2);
		if(y!=0)
		{
			y=MRR(y+1,1);
		}		                  
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(14*2));
		DisplayString(Row,10,0,(u8 *)S0List[y>>7]);//�ϸ� �¸�
		if(y>>7)
			y=(~y)+1;
		y=hex_bcd(y&0xff);
		
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
		Row++;
		//�����ܼ��й��ܵ�����
		a64=MRR(ADDR_230x_9+(MenuCount->v2-1)*LENper_230x_9+3,8);
		a64=R_Inverse(a64,8);
		if(y==0xEEEEEEEE)
		{//�޴�������-------
			DisplayString(Row,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+Row*84+1*2);
			disp_long64_kw(a64,0,1,p16,2);
		}
		Row++;
		//�µ�Ͷ���ִ�
		x=MRR(ADDR_230x_15+(MenuCount->v2-1)*LENper_230x_15+2,1);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Row*84)+(10*2));
		x=Bit_String_Change(x);
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
// Descriptions:        �����¸��ز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_4(void)//�˵�2_3
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u64 a64;
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_3);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			MR(ADDR_DATABUFF,ADDR_8106_127,LENmax_8106_127);
			for(x=0;x<NUMmax_TAG;x++)
			{
				y=MRR(ADDR_DATABUFF+2+x*LENper_8106_127,2);
				if(y==0x0202)
				{
					y=Get_ElementADDR(ADDR_DATABUFF+2+x*LENper_8106_127,1,0,LENmax_8106_127);
					if(y!=0)
					{
						y=MRR(y+1,2);
						y=R_Inverse(y,2);
						if((y&0xff)==MenuCount->v2)
						{
							MenuCount->v3=x;
							break;
						}
					}
				}
				MenuCount->v3=0xffff;
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
			NdigitSelect(1,9,1,NUMmax_TAG);
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
	if(MenuCount->v3!=0xffff)
	{
		MR(ADDR_DATABUFF,ADDR_8106_127+2+MenuCount->v3*LENper_8106_127,LENper_8106_127);
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PowerLowerList[i]);
		}
		//��ǰ����
		a64=MRR(ADDR_230x_3+(MenuCount->v2-1)*LENper_230x_3+1,8);
		a64=R_Inverse(a64,8);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(5*2));
		disp_long64_kw(a64,0,1,p16,0);
		
		//�¸�ϵ��
		y=Get_ElementADDR(ADDR_DATABUFF,2,2,LENper_8106_127);
		if(y!=0)
			y=MRR(y+1,1);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(14*2));
		DisplayString(3,10,0,(u8 *)S0List[y>>7]);//�ϸ� �¸�
		if(y>>7)
			y=(~y)+1;
		y=hex_bcd(y&0xff);

		p16=DisplayData_OffZero(y,2,0,p16);
		for(i=15;i<20;i++)
		{
			if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
			{
				break;
			}
		}
		DisplayString(3,i,0,(u8 *)UnitList[6]);//6 %
		//��ǰ���ض�ֵ
		z=MRR(ADDR_Terminal_kWCtrl1_State+MenuCount->v3*Terminal_Ctrl_Pn_Len,1);
		if(z!=0)
		{
			a64=MRR(ADDR_Terminal_kWCtrl1_State+MenuCount->v3*Terminal_Ctrl_Pn_Len+8,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+(4*84)+(5*2));
			disp_long64_kw(a64,0,1,p16,0);
		}
		else//�����¸���δͶ��
		{
			DisplayString(4,10,0,"δͶ��");
		}

		//�¸��ؿ���ʱ��
		y=Get_ElementADDR(ADDR_DATABUFF,2,4,LENper_8106_127);
		if(y!=0)
			y=MRR(y+1,1);
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
		x=MRR(ADDR_230x_14+(MenuCount->v2-1)*LENper_230x_14+2,1);
		x=Bit_String_Change(x);
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
		x=Get_ElementADDR(ADDR_DATABUFF,2,5,LENper_8106_127);
		if(x!=0)
			x=MRR(x+1,1);
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
		x=Get_ElementADDR(ADDR_DATABUFF,2,6,LENper_8106_127);
		if(x!=0)
			x=MRR(x+1,1);
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
		DisplayString(2,2,0,"�޹����¸��ز���");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3
// Descriptions:        Ӫҵ��ͣ�ز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_3(void)//�˵�2_4
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 x;
	u32 y;
	u64 b64;
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_4);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			MR(ADDR_DATABUFF,ADDR_8105_2,LENmax_8105_2);
			for(x=0;x<NUMmax_TAG;x++)
			{
				y=Get_ElementADDR(ADDR_DATABUFF,x+1,1,LENmax_8105_2);
				if(y!=0)
				{
					y=MRR(y+1,2);
					y=R_Inverse(y,2);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=x;
						break;
					}
				}
				MenuCount->v3=0xffff;
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
			NdigitSelect(1,9,1,NUMmax_TAG);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(2,5);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		MR(ADDR_DATABUFF,ADDR_8105_2,LENmax_8105_2);
		for(i=0;i<=5;i++)
		{
			DisplayString(2+i,0,0,(u8 *)BusinessQuitList[i]);
		}
		//Ӫҵ��ͣ�ع��ʶ�ֵ
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,4,LENmax_8105_2);
		if(y==0)
		{//�޴�������-------
			DisplayString(2,10,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(5*2));
			b64=MRR(y+1,8);
			b64=R_Inverse(b64,8);
			disp_long64_kw(b64,0,1,p16,0);
		}
		//��ʼʱ�� ������
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,2,LENmax_8105_2);
		if(y!=0)
		{
			Disp_DataType_date_time_s(4,y);
		}
		//����ʱ�� ������
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,3,LENmax_8105_2);
		if(y!=0)
		{
			Disp_DataType_date_time_s(6,y);
		}
		//����Ͷ���ִ�
		x=MRR(ADDR_230x_14+(MenuCount->v2-1)*LENper_230x_14+2,1);
		x=Bit_String_Change(x);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(7*84)+(10*2));
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
		DisplayString(2,2,0,"��Ӫҵ��ͣ�ز���");	
	}
}

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
	u64 a64;
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_2);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			MR(ADDR_DATABUFF,ADDR_8104_2,LENmax_8104_2);
			for(x=0;x<NUMmax_TAG;x++)
			{
				y=Get_ElementADDR(ADDR_DATABUFF,x+1,1,LENmax_8104_2);
				if(y!=0)
				{
					y=MRR(y+1,2);
					y=R_Inverse(y,2);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=x;
						break;
					}
				}
				MenuCount->v3=0xffff;
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
			NdigitSelect(1,9,1,NUMmax_TAG);
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
	if(MenuCount->v3!=0xffff)
	{
		MR(ADDR_DATABUFF,ADDR_8104_2,LENmax_8104_2);
		for(i=0;i<=5;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)WorkersDayOffList[i]);
		}
		//���ݿض�ֵ
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,2,LENmax_8104_2);
		if(y==0)
		{//�޴�������-------
			DisplayString(Ln+2,12,0,"-------");	
		}
		else
		{
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+(5*2));
			a64=MRR(y+1,8);
			a64=R_Inverse(a64,8);
			disp_long64_kw(a64,0,1,p16,0);
			for(i=13;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+i*2))
				{
					break;
				}
			}
			//DisplayString(Ln+2,i,0,(u8 *)UnitList[0]);//0 kW
		}
		//�޵���ʼʱ�� 1C YY YY MM DD hh mm ss
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,3,LENmax_8104_2);
		if(y!=0)
		{
			Disp_DataType_date_time_s(4,y);
		}
		//�޵�����ʱ��
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,4,LENmax_8104_2);
		if(y!=0)
		{
			y=MRR(y+1,2);
			y=R_Inverse(y,2);
			y=hex_bcd(y);
		}
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+(9*2));
		p16=DisplayData_OffZero(y,5,0,p16);//65535
		DisplayString(Ln+5,15,0,"����");//7 h
		//ÿ���޵���
		y=Get_ElementADDR(ADDR_DATABUFF,MenuCount->v3+1,5,LENmax_8104_2);
		if(y!=0)
		{
			x=MRR(y+2,1);
			x=Bit_String_Change(x);
		}
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+(12*2));
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
		x=MRR(ADDR_230x_14+(MenuCount->v2-1)*LENper_230x_14+2,1);
		x=Bit_String_Change(x);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+(12*2));
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
		DisplayString(Ln+2,4,0,"�޳��ݿز���");	
	}
}


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
	u64 a64;
	u32 flag=0xffff;
//������
	N = (Stack->MenuStack[MenuCount->Count-1].EditRow)-1;
//����ʱ�� Ŀǰ��������Ϊһ�������Ŀ���ʱ��
	p8=(u8 *)ADDR_DATABUFF;
	a=(u8 *)(ADDR_DATABUFF+200);//ʱ����ʼλ��
	b=(u8 *)(ADDR_DATABUFF+250);//ʱ�ν���λ��
	c=(u8 *)(ADDR_DATABUFF+300);//ʱ�ζ�Ӧ�Ŀ��Ʒ�ʽ������1������2��������
	MR(ADDR_DATABUFF,ADDR_8101_2,LENmax_8101_2);
	if(p8[1]==12)//�����Ա����������12��������Լ ��A.10���ն˹���ʱ�����ݵ�Ԫ��ʽ
	{
		for(i=0;i<12;i++)
		{
			p8[LENmax_8101_2+4*i]=p8[3+i*2]&0x03;
			p8[LENmax_8101_2+4*i+1]=(p8[3+i*2]>>2)&0x03;
			p8[LENmax_8101_2+4*i+2]=(p8[3+i*2]>>4)&0x03;
			p8[LENmax_8101_2+4*i+3]=(p8[3+i*2]>>6)&0x03;
		}
		for(i=0;i<48;)
		{
			if(m>=8)
			{
				break;
			}
			else
			{
				switch(p8[LENmax_8101_2+i])
				{
					case 0:
						m++;
						for(j=i+1;j<48;j++)
						{
							if(p8[LENmax_8101_2+j]!=0)
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
							if(p8[LENmax_8101_2+j]!=1)
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
							if(p8[LENmax_8101_2+j]!=2)
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
							if(p8[LENmax_8101_2+j]!=3)
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
	}
	MenuCount->v3=m;//һ���Ϊ���ٸ�ʱ��
//MenuCount->v2��¼�ܼ���� �����޸�ֵ
	if(MenuCount->v2>NUMmax_TAG)
	{
		MenuCount->v2=NUMmax_TAG;
	}
	if(MenuCount->v2<1)
	{
		MenuCount->v2=1;
	}
#if LENmax_8103_2>(LEN_DATABUFF-400)
	#error
#endif
	MR(ADDR_DATABUFF+400,ADDR_8103_2,LENmax_8103_2);
	for(x=0;x<NUMmax_TAG;x++)
	{
		y=Get_ElementADDR(ADDR_DATABUFF+400,x+1,1,LENmax_8103_2);
		if(y!=0)
		{
			y=MRR(y+1,2);
			y=R_Inverse(y,2);
			if((y&0xff)==MenuCount->v2)
			{
				flag=x;
				break;
			}
		}
	}
//�����1����ʾ�˶��ٸ����ö�ֵ��ʱ��
	if(flag!=0xffff)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+18*2);
		if(p16[0]==0x40B8 && p16[1]==0x40B9)
		{//���½��С�
			x=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,2,0,LENper_8103_2);//������ʶ    bit-string(SIZE(8))��
			if(x!=0)
			{
				x=MRR(x+2,1);
				x=Bit_String_Change(x);
			}	
			if((x>>N)&0x01)
			{
				x=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,3+N,0,LENper_8103_2);//������ʶ	 bit-string(SIZE(8))��
				if(x!=0)
				{
					y=MRR(x+2,1);
					y=Bit_String_Change(y);
				}
				n=0;
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
			if(flag!=0xffff)
			{
				//ʱ�οض�ֵ����ϵ��
				x=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,6,0,LENper_8103_2);
				p16=(u16 *)(ADDR_STRINGBUFF+4+(2*84)+(13*2));
				x=MRR(x+1,1);
				DisplayString(2,9,0,(u8 *)S0List[x>>7]);//�ϸ� �¸�
				if(x>>7)
					x=(~x)+1;
				x=hex_bcd(x&0xff);
				
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
				x=MRR(ADDR_230x_14+(MenuCount->v2-1)*LENper_230x_14+2,1);
				x=Bit_String_Change(x);
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
					x=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,2,0,LENper_8103_2);
					if(x==0)
						return;
					x=MRR(x+2,1);
					x=Bit_String_Change(x);
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
							y=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,3+N,1,LENper_8103_2);
							if(y!=0)
								y=MRR(y+2,1);					
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
										l=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,3+N,1+n,LENper_8103_2);
										if(l==0)
										{//�޴�������-------
											DisplayString(Ln,13,0,"-------");	
										}
										else
										{
											p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
											a64=MRR(l+1,8);
											a64=R_Inverse(a64,8);
											disp_long64_kw(a64,0,1,p16,0);
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
					x=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,2,0,LENper_8103_2);
					if(x!=0)
						x=MRR(x+2,1);
					x=Bit_String_Change(x);
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
							y=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,3+N,1,LENper_8103_2);
							if(y!=0)
								y=MRR(y+2,1);
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
										l=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,3+N,1+n,LENper_8103_2);
										if(l==0)
										{//�޴�������-------
											DisplayString(Ln,13,0,"-------");	
										}
										else
										{
											p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
											a64=MRR(l+1,8);
											a64=R_Inverse(a64,8);
											disp_long64_kw(a64,0,1,p16,0);
										}
										break;
								}
							}
							break;
					}
				}
			}
			else
			{
				Ln=0;
				DisplayClrRow(Ln+2,Ln+6);//����Ļ��ʾ��ָ����
				DisplayString(Ln+2,4,0,"��ʱ�οز���");
			}
			break;
		case 1://����1:ѡ��
			//x=MEMReadReg(ADDR_AFN04F41+((1+(17*3))*(MenuCount->v2-1)),1);//������־
			if(flag!=0xffff)
			{
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
							x=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,2,0,LENper_8103_2);
							if(x!=0)
								x=MRR(x+2,1);
							x=Bit_String_Change(x);
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
									y=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,3+N,1,LENper_8103_2);
									if(y!=0)
										y=MRR(y+2,1);
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
												l=Get_ElementADDR(ADDR_DATABUFF+400+2+flag*LENper_8103_2,3+N,1+n+4,LENper_8103_2);
												if(l==0)
												{//�޴�������-------
													DisplayString(Ln,13,0,"-------");	
												}
												else
												{
													p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+(13*2));
													a64=MRR(l+1,8);
													a64=R_Inverse(a64,8);
													disp_long64_kw(a64,0,1,p16,0);
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
			}
			else
			{
				Ln=0;
				DisplayClrRow(Ln+2,Ln+6);//����Ļ��ʾ��ָ����
				DisplayString(Ln+2,4,0,"��ʱ�οز���");
			}
			NdigitSelect(1,9,1,NUMmax_TAG);
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1
// Descriptions:        ʱ�οز���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1(void)
{
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
						MenuCreate((u32)Menu_2_3);
						break;
					case 4:
						MenuCreate((u32)Menu_2_4);
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
							MenuCreate((u32)Menu2_2);//�����¼��˵�
						}
						else
						{
							PassWordCreateEdit();
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

#endif
