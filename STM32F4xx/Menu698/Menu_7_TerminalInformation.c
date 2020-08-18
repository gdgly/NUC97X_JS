
//�˵�7 �ն���Ϣ
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"


extern const u8 UnitList[][6];//ͨ�õ�λ�б�
//extern const u8 OnlineModeList[][11];//����ģʽ�б�
//extern const u8 ConnectWayList[][6];//���ӷ�ʽ�б�

const u8 pMenu_7[]=
{
	"�ն���Ϣ\n\r"
	"  1.����汾\n\r"
	"  2.�ն˵�ַ\n\r"
};

const u8 pMenu_7_1[]=
{
	"�ն˰汾\n\r"
	"����汾:\n\r"
	"����:\n\r"
	"Ӳ���汾:\n\r"
	"����:\n\r"
};

const u8 pMenu_7_2[]=
{
	"ͨ�ŵ�ַ\n\r"
	"��ַ\n\r"
};
#if ((Project/100)==3)||((Project/100)==4)//ר���ն�

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2
// Descriptions:        �ն˵�ַ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_7_2(void)
{
	u16 *p16;
	u64 x=0;
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_2);//��ʾ�˵�
	//1.��ַ
	p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+4*2);
	MR(ADDR_DATABUFF,ADDR_4001+2,6);
	Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	x=MRR(ADDR_DATABUFF,6);
	DisplayData(x,12,0,p16);

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1
// Descriptions:        �汾��Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_7_1(void)
{
	u16 *p16;
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1);//��ʾ�˵�
	//1.���̴���
	//1.����汾
	p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+9*2);
	p16[0]=0x2000+'V';
	p16[1]=(SVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((SVER>>4)&0xf)+0x2030;
	p16[4]=((SVER>>0)&0xf)+0x2030;

	//2.����汾����
	p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2);
	p16[0]=2+0x2030;
	p16[1]=0+0x2030;
	p16[2]=((SVDATE>>20)&0xf)+0x2030;
	p16[3]=((SVDATE>>16)&0xf)+0x2030;
	p16[4]='-'+0x2000;
	p16[5]=((SVDATE>>12)&0xf)+0x2030;
	p16[6]=((SVDATE>>8)&0xf)+0x2030;
	p16[7]='-'+0x2000;
	p16[8]=((SVDATE>>4)&0xf)+0x2030;
	p16[9]=((SVDATE>>0)&0xf)+0x2030;
	//3.Ӳ���汾
	p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+9*2);
	p16[0]=0x2000+'V';
	p16[1]=(HVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((HVER>>4)&0xf)+0x2030;
	p16[4]=((HVER>>0)&0xf)+0x2030;
	//4.Ӳ���汾����
	p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+5*2);
	p16[0]=2+0x2030;
	p16[1]=0x2030;
	p16[2]=((HVDATE>>20)&0xf)+0x2030;
	p16[3]=((HVDATE>>16)&0xf)+0x2030;
	p16[4]='-'+0x2000;
	p16[5]=((HVDATE>>12)&0xf)+0x2030;
	p16[6]=((HVDATE>>8)&0xf)+0x2030;
	p16[7]='-'+0x2000;
	p16[8]=((HVDATE>>4)&0xf)+0x2030;
	p16[9]=((HVDATE>>0)&0xf)+0x2030;	
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7
// Descriptions:        �ն���Ϣ�˵�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_7(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_7_1);//�����¼��˵�
						break;
					case 2:
						MenuCreate((u32)Menu_7_2);//�����¼��˵�
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
