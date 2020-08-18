
//�˵�3 ����״̬
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_DataDef.h"



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
#if ((Project/100)==3)||((Project/100)==4)//ר���ն�

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
	u32 y;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=NUMmax_TAG;
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
	DisplayClrRow(2,8);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=NUMmax_TAG && MenuCount->v2>=1)
	{
		y=0;
		i=MRR(ADDR_Terminal_kWCtrl4_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<0);
		
		i=MRR(ADDR_Terminal_kWCtrl3_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<1);

		i=MRR(ADDR_Terminal_kWCtrl2_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<2);

		i=MRR(ADDR_Terminal_kWCtrl1_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<3);
		
		i=MRR(ADDR_Terminal_kWhCtrl1_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<4);
		
		i=MRR(ADDR_Terminal_kWhCtrl2_State+(MenuCount->v2-1)*Terminal_Ctrl_Pn_Len,1);
		if(i!=0)
			y|=(1<<5);
	
		if(y==0)//���أ���ض�û��
		{
			DisplayString(2,0,0,(u8 *)NControlList[0]);
		}
		else
		{
			for(i=0;i<6;i++)
			{
				DisplayString(2+i,0,0,(u8 *)ControlList[i]);
				DisplayString(2+i,14,0,(u8 *)ControlStateList[(y>>i)&0x01]);//D0ʱ�ο�D1���ݿ�D2Ӫҵ��ͣ��D3��ǰ�����¸��� ��� Ͷ��
			}
			
			y=MRR(ADDR_8001_2+2,1);//������״̬
			DisplayString(8,0,0,(u8 *)ControlList[6]);
			DisplayString(8,14,0,(u8 *)ControlStateList[y&0x01]);//D0���� ��� Ͷ��
		}
	}
	else
	{
		DisplayString(2,4,0,"��Ч�ܼ����");
	}
}
#endif
