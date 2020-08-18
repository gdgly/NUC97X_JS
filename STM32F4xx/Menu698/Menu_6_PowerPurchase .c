
//�˵�6 ������Ϣ
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu6.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"





const u8 QCUnitList[][3]=//����(��)��λ�б�
{
	"kWh",
	"Ԫ ",
};

const u8 PurchaseWayList[][5]=//���緽ʽ�б�
{
	"׷��",
	"ˢ��",
	"��Ч",
};

const u8 PurchaseTypeList[][5]=//���������б�
{
	"����",
	"���",
	"��Ч",
};

const u8 pMenu_6[]=
{
	"������Ϣ\n\r"
	"���ƶ���\n\r"
};

const u8 PPList[][20]=//����ز����б�
{
	"����:",//0
	"���緽ʽ:",//1
	"��������:",//1
	"��������:",//2	
	" ",	
	"��������:",//4
	" ",
	"��բ����:",//6
	" ",
	"ʣ�������:",//8	
};
#if ((Project/100)==3)||((Project/100)==4)//ר���ն�

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_6_1
// Descriptions:        ������Ϣ����2����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_6_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u64 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			for(i=0;i<NUMmax_TAG;i++)
			{
				y=Get_ElementADDR(ADDR_8107_2,i+1,1,LENper_8107_2);//��ȡ �ܼ������      OI��
				if(y!=0)
				{
					y=MRR(y+1,2);
					y=R_Inverse(y,2);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=i;
						break;
					}
				}
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
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//�ϼ�
					CreateScreen((u32)Menu_6);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			break;
		default:
			break;
	}
	DisplayClrRow(2,3);//����Ļ��ʾ��ָ����
	p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	if(MenuCount->v3!=0xffff)
	{
		for(i=7;i<10;i++)
		{
			DisplayString(2+i-7,0,0,(u8 *)PPList[i]);
		}
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,4,LENper_8107_2);
		if(y!=0)
		{
			y=MRR(y+1,1);
			if(y>1)
				y=2;
		}
		else
		{
			y=2;
		}
		i=y;
		//��բ����
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,7,LENper_8107_2);
		if(y==0)
		{//�޴�������-------
			DisplayString(3,3,0,"-------"); 
		}
		else
		{
			z=MRR(y+1,8);
			z=R_Inverse(z,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+3*2);

			if(i==0)
				disp_long64_kw(z,1,4,p16,2);
			else
				disp_long64_kw(z,1,4,p16,4);
		}
		
		//ʣ��
		y=MRR(ADDR_230x_11+(MenuCount->v2-1)*LENper_230x_11,1);
		if(y!=DataType_long64)
		{//�޴�������-------
			DisplayString(5,3,0,"-------");	
		}
		else
		{
			z=MRR(ADDR_230x_11+(MenuCount->v2-1)*LENper_230x_11+1,8);
			z=R_Inverse(z,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+5*84+3*2);
			if(i==0)
				disp_long64_kw(z,0,1,p16,2);
			else
				disp_long64_kw(z,0,1,p16,4);
		}

		UpArrow();
	}
	else
	{
		DisplayString(2,4,0,"��Ч���ƶ���");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_6
// Descriptions:        ������Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_6(void)//�˵�6
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u64 z;
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			for(i=0;i<NUMmax_TAG;i++)
			{
				y=Get_ElementADDR(ADDR_8107_2,i+1,1,LENper_8107_2);//��ȡ �ܼ������      OI��
				if(y!=0)
				{
					y=MRR(y+1,2);
					y=R_Inverse(y,2);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=i;
						break;
					}
				}
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
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//�¼�
					CreateScreen((u32)Menu_6_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PPList[i]);
		}
		//1.���絥��
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,2,LENper_8107_2);
		if(y!=0)
		{
			y=MRR(y+1,4);
			y=R_Inverse(y,4);
		}
		z=hex_bcd16(y);
		p16=(u16 *)(ADDR_STRINGBUFF+4+2*84+5*2);
		DisplayData_OffZero(z,10,0,p16);
		//2.���緽ʽ
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,3,LENper_8107_2);
		if(y!=0)
		{
			y=MRR(y+1,1);
			if(y>1)
				y=2;
		}
		else
		{
			y=2;
		}

		DisplayString(3,10,0,(u8 *)PurchaseWayList[y]);//0׷�� 1ˢ�� 2��Ч
		//3.��������
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,4,LENper_8107_2);
		if(y!=0)
		{
			y=MRR(y+1,1);
			if(y>1)
				y=2;
		}
		else
		{
			y=2;
		}
		i=y;
		DisplayString(4,10,0,(u8 *)PurchaseTypeList[y]);//0���� 1��� 2��Ч
		
		//4.���������ѣ�4.��������5.��բ����
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,5,LENper_8107_2);
		if(y==0)
		{//�޴�������-------
			DisplayString(6,3,0,"-------");	
		}
		else
		{
			z=MRR(y+1,8);
			z=R_Inverse(z,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+3*2);

			if(i==0)
				disp_long64_kw(z,1,4,p16,2);
			else
				disp_long64_kw(z,1,4,p16,4);
		}
		//5.��������
		y=Get_ElementADDR(ADDR_8107_2,MenuCount->v3+1,6,LENper_8107_2);
		if(y==0)
		{//�޴�������-------
			DisplayString(8,3,0,"-------"); 
		}
		else
		{
			z=MRR(y+1,8);
			z=R_Inverse(z,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+3*2);

			if(i==0)
				disp_long64_kw(z,1,4,p16,2);
			else
				disp_long64_kw(z,1,4,p16,4);
		}

		DownArrow();
	}
	else
	{
		DisplayString(2,4,0,"��Ч���ƶ���");	
	}
}

#endif
