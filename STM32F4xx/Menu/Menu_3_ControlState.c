
//�˵�3 ����״̬
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"


#if ((Project/100)==3)//ר���ն�

const u8 ControlStateList[][5]=//���ƽ��Ͷ���б�
{
	"���",
	"Ͷ��",
};

const u8 pMenu_3[]=
{
	"����״̬\n\r"
	"�ܼ���\n\r"
};

const u8 NControlList[][18]=//�޿���Ͷ���б�
{
	"   ��ǰ�޿���Ͷ��",
};

#if(USER/100==5)//�Ϻ��û�
const u8 ControlList[][20]=//�����б�
{
	"ʱ�ο�    :",
	"���ݿ�    :",
	"Ӫҵ��ͣ��:",
	"�����¸���:",
	"�µ��    :",
	"�����:    ����:",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3
// Descriptions:        ����״̬
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3(void)//�˵�3
{
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3);
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
	DisplayClrRow(3,9);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		x=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+2,2);//������״̬�����״̬
		y=MRR(ADDR_AFN0CF5,1);//������״̬
		if(x==0&&y==0)
		{
			DisplayString(3,0,0,(u8 *)NControlList[0]);
		}
		else
		{
			y=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+2,1);//������״̬
			for(i=0;i<4;i++)
			{
				DisplayString(3+i,0,0,(u8 *)ControlList[i]);
				DisplayString(3+i,14,0,(u8 *)ControlStateList[(y>>i)&0x01]);//D0ʱ�ο�D1���ݿ�D2Ӫҵ��ͣ��D3��ǰ�����¸��� ��� Ͷ��
			}
			y=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+3,1);//�����״̬
			for(i=0;i<2;i++)
			{
				DisplayString(7+i,0,0,(u8 *)ControlList[i+4]);
			}
			DisplayString(7,14,0,(u8 *)ControlStateList[(y>>0)&0x01]);//D0�µ��D1����� ��� Ͷ��
			DisplayString(8,7,0,(u8 *)ControlStateList[(y>>1)&0x01]);//D0�µ��D1����� ��� Ͷ��
			y=MRR(ADDR_AFN0CF5,1);//������״̬
			DisplayString(8,16,0,(u8 *)ControlStateList[y&0x01]);//D0���� ��� Ͷ��
		}
	}
	else
	{
		DisplayString(3,4,0,"��Ч�ܼ����");
	}
}

#else
const u8 ControlList[][14]=//�����б�
{
	"ʱ�ο�    :",
	"���ݿ�    :",
	"Ӫҵ��ͣ��:",
	"�����¸���:",
	"�µ��    :",
	"�����    :",
	"����      :",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3
// Descriptions:        ����״̬
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3(void)//�˵�3
{
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>MaxTotalAddGroup)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=MaxTotalAddGroup;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3);
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
	DisplayClrRow(2,8);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxTotalAddGroup && MenuCount->v2>=1)
	{
		x=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+2,2);//������״̬�����״̬
		y=MRR(ADDR_AFN0CF5,1);//������״̬
		if(x==0&&y==0)
		{
			DisplayString(2,0,0,(u8 *)NControlList[0]);
		}
		else
		{
			y=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+2,1);//������״̬
			for(i=0;i<4;i++)
			{
				DisplayString(2+i,0,0,(u8 *)ControlList[i]);
				DisplayString(2+i,14,0,(u8 *)ControlStateList[(y>>i)&0x01]);//D0ʱ�ο�D1���ݿ�D2Ӫҵ��ͣ��D3��ǰ�����¸��� ��� Ͷ��
			}
			y=MRR(ADDR_AFN0CF5+2+6*(MenuCount->v2-1)+3,1);//�����״̬
			for(i=0;i<2;i++)
			{
				DisplayString(6+i,0,0,(u8 *)ControlList[i+4]);
				DisplayString(6+i,14,0,(u8 *)ControlStateList[(y>>i)&0x01]);//D0�µ��D1����� ��� Ͷ��
			}
			y=MRR(ADDR_AFN0CF5,1);//������״̬
			DisplayString(8,0,0,(u8 *)ControlList[6]);
			DisplayString(8,16,0,(u8 *)ControlStateList[y&0x01]);//D0���� ��� Ͷ��
		}
	}
	else
	{
		DisplayString(2,4,0,"��Ч�ܼ����");
	}
}
#endif

#endif
