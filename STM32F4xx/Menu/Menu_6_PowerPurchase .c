
//�˵�6 ������Ϣ
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu6.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"




#if ((Project/100)==3)//ר���ն�

extern const u8 SList[][2];//���������б�

const u8 QCUnitList[][7]=//����(��)��λ�б�
{
	"kWh/��",
	"MWh/Ԫ",
};

const u8 PurchaseWayList[][5]=//���緽ʽ�б�
{
	"׷��",
	"ˢ��",
	"��Ч",
};

const u8 pMenu_6[]=
{
	"������Ϣ\n\r"
	"���ƶ���\n\r"
};

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
const u8 PPList[][20]=//����ز����б�
{
	"���絥��:",//0
	"���緽ʽ:",//1
	"��ǰ����:",//2
	" ",
	"�������:",//4
	" ",	
	"��������:",//6
	" ",
	"��բ����:",//8
	" ",
	"ʣ�����:",//10	
	" ",
};

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
	u32 j;
	u32 y;
	u32 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//��ʾ�˵�
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
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)PPList[i+6]);
		}
		//1.��������2.��բ����
		for(i=0;i<2;i++)
		{
			y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1)+9+4*i,4);
			if(y==0xEEEEEEEE)
			{//�޴�������-------
				DisplayString(4+2*i,3,0,"-------");	
			}
			else
			{
				z=y&0xfffffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+2*i)*84+3*2);
				p16=DisplayData_OffZero(z,7,0,p16);
				DisplayString(4+2*i,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
				for(j=4;j<20;j++)
				{
					if(p16 == (u16 *)(ADDR_STRINGBUFF+(4+2*i)*84+4+j*2))
					{
						break;
					}
				}
				DisplayString(4+2*i,j+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/�� 1 MWh/Ԫ
			}
			DispalyAlign(4+2*i,17,3);//����ʾ����(1�����,2����,3�Ҷ���)
		}
		//3.ʣ��������ѣ�
		y=MRR(ADDR_AFN0CF23+4*(MenuCount->v2-1),4);
		if(y==0xEEEEEEEE)
		{//�޴�������-------
			DisplayString(8,3,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+3*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(8,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=4;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(8,i+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/�� 1 MWh/Ԫ
		}
		DispalyAlign(8,17,3);//����ʾ����(1�����,2����,3�Ҷ���)
		UpArrow();
	}
	else
	{
		DisplayString(3,4,0,"��Ч���ƶ���");	
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
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)PPList[i]);
		}
		//1.���絥��
		y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1),4);
		z=hex_bcd16(y);
		p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+10*2);
		DisplayData(z,10,0,p16);
		//2.���緽ʽ
		y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1)+4,1);
		switch(y)
		{
			case 0x55:
				i=0;
				break;
			case 0xAA:
				i=1;
				break;
			default:
				i=2;
				break;
		}
		DisplayString(4,10,0,(u8 *)PurchaseWayList[i]);//0׷�� 1ˢ�� 2��Ч
		//3.��ǰ����
		y=MRR(ADDR_AFN04F47_Ext+8*(MenuCount->v2-1),4);
		if(y==0xEEEEEEEE)
		{//�޴�������-------
			DisplayString(6,3,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+3*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(6,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=4;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+6*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(6,i+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/�� 1 MWh/Ԫ
		}
		DispalyAlign(6,17,3);//����ʾ����(1�����,2����,3�Ҷ���)
		//4.�������
		y=MRR(ADDR_AFN04F47_Ext+8*(MenuCount->v2-1)+4,4);
		if(y==0xEEEEEEEE)
		{//�޴�������-------
			DisplayString(8,3,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+3*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(8,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=4;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(8,i+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/�� 1 MWh/Ԫ
		}
		DispalyAlign(8,17,3);//����ʾ����(1�����,2����,3�Ҷ���)

		DownArrow();
	}
	else
	{
		DisplayString(3,4,0,"��Ч���ƶ���");	
	}
}

#else

const u8 PPList[][20]=//����ز����б�
{
	"���絥��:",//0
	"���緽ʽ:",//1
	"��������:",//2	
	" ",	
	"��������:",//4
	" ",
	"��բ����:",//6
	" ",
	"ʣ�������:",//8	
};

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
	u32 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//��ʾ�˵�
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
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		DisplayString(2,0,0,(u8 *)PPList[8]);
		//ʣ��������ѣ�
		y=MRR(ADDR_AFN0CF23+4*(MenuCount->v2-1),4);
		if(y==0xEEEEEEEE)
		{//�޴�������-------
			DisplayString(3,3,0,"-------");	
		}
		else
		{
			z=y&0xfffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+3*2);
			p16=DisplayData_OffZero(z,7,0,p16);
			DisplayString(3,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
			for(i=4;i<20;i++)
			{
				if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
				{
					break;
				}
			}
			DisplayString(3,i+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/�� 1 MWh/Ԫ
		}
		DispalyAlign(3,17,3);//����ʾ����(1�����,2����,3�Ҷ���)
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
	u32 j;
	u32 y;
	u64 z;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_6);//��ʾ�˵�
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
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		for(i=0;i<=7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PPList[i]);
		}
		//1.���絥��
		y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1),4);
		z=hex_bcd16(y);
		p16=(u16 *)(ADDR_STRINGBUFF+4+2*84+10*2);
		DisplayData(z,10,0,p16);
		//2.���緽ʽ
		y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1)+4,1);
		switch(y)
		{
			case 0x55:
				i=0;
				break;
			case 0xAA:
				i=1;
				break;
			default:
				i=2;
				break;
		}
		DisplayString(3,10,0,(u8 *)PurchaseWayList[i]);//0׷�� 1ˢ�� 2��Ч
		//3.���������ѣ�4.��������5.��բ����
		for(i=0;i<=2;i++)
		{
			y=MRR(ADDR_AFN04F47+17*(MenuCount->v2-1)+5+4*i,4);
			if(y==0xEEEEEEEE)
			{//�޴�������-------
				DisplayString(5+2*i,3,0,"-------");	
			}
			else
			{
				z=y&0xfffffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(5+2*i)*84+3*2);
				p16=DisplayData_OffZero(z,7,0,p16);
				DisplayString(5+2*i,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
				for(j=4;j<20;j++)
				{
					if(p16 == (u16 *)(ADDR_STRINGBUFF+(5+2*i)*84+4+j*2))
					{
						break;
					}
				}
				DisplayString(5+2*i,j+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/�� 1 MWh/Ԫ
			}
			DispalyAlign(5+2*i,17,3);//����ʾ����(1�����,2����,3�Ҷ���)
		}
		DownArrow();
	}
	else
	{
		DisplayString(2,4,0,"��Ч���ƶ���");	
	}
}
#endif

#endif
