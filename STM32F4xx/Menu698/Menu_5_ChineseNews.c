//�˵�5 ������Ϣ
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"



const u8 pMenu_5[]=
{
	"������Ϣ\n\r"
	"  1.��ͨ������Ϣ\n\r"
	"  2.��Ҫ������Ϣ\n\r"
};

const u8 pMenu_5_1[]=
{
	"��ͨ������Ϣ\n\r"
	"��Ϣ���:\n\r"
};

const u8 pMenu_5_2[]=
{
	"��Ҫ������Ϣ\n\r"
	"��Ϣ���:\n\r"
};

#if ((Project/100)==3)||((Project/100)==4)//ר���ն�

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_1
// Descriptions:        ��ͨ������Ϣ�˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_1(void)//�˵�5_1
{
	return;
	/*
	u8 *p8;
	u16 *p16;
	u32 m;
	u32 y;
	u32 x;
	u32 Ln;
	u32 i;
	u32 j;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_1);//��ʾ�˵�
			if(MenuCount->v2>16)
			{//MenuCount->v2��¼��Ϣ��ţ�1~16��Ӧ0~15��
				MenuCount->v2=16;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v1=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			bcd_p8(p8,MenuCount->v2,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,10,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(2,9,1,16);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	for(j=0;j<2;j++)
	{
		DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
		if(MenuCount->v2<=16 && MenuCount->v2>=1)
		{
			m=MEMReadReg(ADDR_AFN05F32+202*(MenuCount->v2-1),1);
			if(m>>4==0 && (m&0xf)==MenuCount->v2-1)//modified by sym 20120504
			{//�ж��Ƿ�Ϊ��ͨ��Ϣ0����Ϣ����Ƿ�ΪMenuCount->v2-1
				y=MEMReadReg(ADDR_AFN05F32+202*(MenuCount->v2-1)+1,1);//����ͨ��Ϣ����
				p8=(u8*)ADDR_DATABUFF;
				MEMClr(0,ADDR_DATABUFF,202);
				MEMRead(ADDR_DATABUFF,ADDR_AFN05F32+202*(MenuCount->v2-1)+2,y);
				x=DisplayString1(2,0,0,p8+MenuCount->v1);
				Ln=x&0xff;
				i=x>>16;
				if(Ln==9 && p8[i]!=0)
				{
					DownArrow();
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//��
						MenuCount->v1=i;
						UpArrow();
					}
					Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
				}
				p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
				if(p16[0]==0x40B6 && p16[1]==0x40B7)
				{//���Ͻ��С�
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//��
						p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
						p16[0]=0x0;
						p16[1]=0x0;
						MenuCount->v1=0;
						DownArrow();
					}
					Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
				}
			}
		}
		else
		{
			DisplayString(2,5,0,"��Ч��Ϣ��");
		}
	}
	*/
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_2
// Descriptions:        ��Ҫ������Ϣ�˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_2(void)//�˵�5_2
{
	return;
	/*
	u8 *p8;
	u16 *p16;
	u32 m;
	u32 y;
	u32 x;
	u32 Ln;
	u32 i;
	u32 j;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_2);//��ʾ�˵�
			if(MenuCount->v2>16)
			{//MenuCount->v2��¼��Ϣ��ţ�1~16��Ӧ0~15��
				MenuCount->v2=16;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v1=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			bcd_p8(p8,MenuCount->v2,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,10,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(2,9,1,16);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	for(j=0;j<2;j++)
	{
		DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
		if(MenuCount->v2<=16 && MenuCount->v2>=1)
		{
			m=MEMReadReg(ADDR_AFN05F32+202*16+202*(MenuCount->v2-1),1);
			if(m>>4==1 && (m&0xf)==MenuCount->v2-1)//modified by sym 20120504
			{//�ж��Ƿ�Ϊ��Ҫ��Ϣ1����Ϣ����Ƿ�ΪMenuCount->v2-1
				y=MEMReadReg(ADDR_AFN05F32+202*16+202*(MenuCount->v2-1)+1,1);//����Ҫ��Ϣ����
				p8=(u8*)ADDR_DATABUFF;
				MEMClr(0,ADDR_DATABUFF,202);
				MEMRead(ADDR_DATABUFF,ADDR_AFN05F32+202*16+202*(MenuCount->v2-1)+2,y);
				x=DisplayString1(2,0,0,p8+MenuCount->v1);
				Ln=x&0xff;
				i=x>>16;
				if(Ln==9 && p8[i]!=0)
				{
					DownArrow();
					if((Comm_Ram->ButtonVal&0x01)!=0)
					{//��
						MenuCount->v1=i;
						UpArrow();
					}
					Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
				}
				p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
				if(p16[0]==0x40B6 && p16[1]==0x40B7)
				{//���Ͻ��С�
					if((Comm_Ram->ButtonVal&0x02)!=0)
					{//��
						p16=(u16 *)(ADDR_STRINGBUFF+4+0*84+18*2);
						p16[0]=0x0;
						p16[1]=0x0;
						MenuCount->v1=0;
						DownArrow();
					}
					Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
				}
			}
		}
		else
		{
			DisplayString(2,5,0,"��Ч��Ϣ��");
		}
	}
	*/
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5
// Descriptions:        ������Ϣ�˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5(void)//�˵�5
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{	  //ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCount->v1=0;
						//MenuCreate((u32)Menu_5_1);//�����¼��˵�
						break;
					case 2:
						MenuCount->v1=0;
						//MenuCreate((u32)Menu_5_2);//�����¼��˵�
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
