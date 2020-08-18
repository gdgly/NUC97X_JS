//���˵�
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "Menu.h"
#include "Menu1.h"
#include "Menu2.h"
#include "Menu3.h"
#include "Menu4.h"
#include "Menu5.h"
#include "Menu6.h"
#include "Menu7.h"
#include "Menu8.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "string.h"
#include "../Display/Draw.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"

void _0(void)
{
}


#if ((Project/100)==3) || ((Project/100)==9)//�����ն�
const u8 pMenu_Main[]=//�����ն����˵�
{
	"���˵�\n\r"
	"ʵʱ����\n\r"
	"���ܼ���\n\r"
	"�����������\n\r"
	"ͣ��ͳ��\n\r"
	"�¼���¼\n\r"
	"�쳣�澯\n\r"
	"�����������ѯ\n\r"
	"�ն˹�����ά��\n\r"
};
#endif

#if ((Project/100)==2)//������
const u8 JZQpMenu_Main[]=//���������˵�
{
	"\n\r"
	"       ���˵�\n\r"	
	"���ݲ�ѯ\n\r"
	"�����������ѯ\n\r"
	"�ն˹�����ά��\n\r"
};
#endif
//-------------------------------------------------------------------------------------------------
// Function name:       PassWord
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
//-------------------------------------------------------------------------------------------------
void PassWord(void)
{
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u32 LnTitle=3;
	u32 ColTitle=6;
	u32 Ln=5;
	u32 Col=6;
	u32 Length=6;
	u32 i;
	u32 x;
	u32 k;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			MenuCount->v1=0;
			DisplayString(LnTitle,ColTitle,0,"��������");
			DisplayString(Ln,Col,0,"0*****>");
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			NegativeLnCol(Stack->MenuStack[MenuCount->Count].EditRow,Stack->MenuStack[MenuCount->Count].EditCol);
			MenuCount->v1=0;
			break;
		case 1://����1:��������
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//���Ƽ�����
				p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					i=Stack->MenuStack[MenuCount->Count].EditCol-Col;
					i=(Length-i-1)*4;
					MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
					p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					Stack->MenuStack[MenuCount->Count].EditCol++;
				}
				else	//err20181130 ����Һ�� ������βѭ������
				{
					Stack->MenuStack[MenuCount->Count].EditCol = 6;//�ص���һ������
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				MemCopy(&x,p16,2);
				x&=0x00ff;
				if((x<0x30 && x!=0x2a) || (x>0x39 && x!=0x3e))//ֻ�����֡�>��*��ѡ�У���������ȫ������
				{
					if(Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
					{
						Stack->MenuStack[MenuCount->Count].EditCol++;
					}
				}	
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
					p16[0]=0x2030;
				}
			}
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//���Ƽ�����
				p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					
						i=Stack->MenuStack[MenuCount->Count].EditCol-6;
						i=(Length-i-1)*4;
						MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
						p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
				{
					Stack->MenuStack[MenuCount->Count].EditCol--;
				}
				else //err20181130 ����Һ�� ������βѭ������
				{
					Stack->MenuStack[MenuCount->Count].EditCol = 12;//ѡ�����һ���ַ�
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				MemCopy(&x,p16,2);
				x&=0x00ff;
				if((x<0x30 && x!=0x2a) || (x>0x39 && x!=0x3e))//ֻ�����֡�>��*��ѡ�У���������ȫ������
				{
					if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
					{
						Stack->MenuStack[MenuCount->Count].EditCol--;
					}
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>=Col && Stack->MenuStack[MenuCount->Count].EditCol<(Col+Length))
				{
					p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
					p16[0]=0x2030;
				}
			}
			NegativeLnCol(Stack->MenuStack[MenuCount->Count].EditRow,Stack->MenuStack[MenuCount->Count].EditCol);
			UpDownSelect();
			if(((Comm_Ram->ButtonVal&0x10)!=0)&&(Stack->MenuStack[MenuCount->Count].EditCol==Col+Length))
			{//ȷ��
				Stack->MenuStack[MenuCount->Count].Task=2;
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
			break;
		case 2://����2���ж������Ƿ���ȷ����ȷ����ת���˵�7��8��������ʾ�Ի���			
			p32=(u32 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_GUIPASSWORD,3);
			p32[0]&=0x00FFFFFF;
			k=hex_bcd(p32[0]);
			k&=0x00FFFFFF;
			MenuCount->v1&=0xFFFFFF;
			if((k==MenuCount->v1)||(PWD==MenuCount->v1))	//�����Ż�������һ���������룬����������������õ����벻ƥ��ʱ�����볬������PWDҲ�ɽ��� ybzyl_20170901		
			{//�Ƚ��������������ȷ�������Ƿ����
				#if ((Project/100)==3) || ((Project/100)==9)//����
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==7)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_7;
					}
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;
					}		
					else
					{
						MenuDel();//�����ϼ��˵�	    
					}
				#endif
				#if ((Project/100)==2)//������
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==3)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_7;
					}
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==4)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;
					}		
					else
					{
						MenuDel();//�����ϼ��˵�	    
					}
				#endif
				Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
				Stack->MenuStack[MenuCount->Count].MoveRow=0;//�˵���������(����ʱ��Ϊ0,����ʱ����)
				Stack->MenuStack[MenuCount->Count].EditRow=1;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
				Stack->MenuStack[MenuCount->Count].EditCol=0;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
			}
			else
			{
				DisplayClr();
				DisplayString(4,6,0,"�������");
				DisplayString(5,3,0,"�밴���ؼ�����");
				Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
					MenuDel();//�����ϼ��˵�	    
				}
				Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			}
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       PassWordCreateEdit
// Descriptions:        ��������༭��
// input parameters:    ��һ��������ַ
// output parameters:   ��ʼ���¼��˵��������������
// Returned value:      ��
// Author:              ����÷
//-------------------------------------------------------------------------------------------------
void PassWordCreateEdit(void)//��������༭��
{
	MenuCount->Count++;
	Stack->MenuStack[MenuCount->Count].PC=(u32)PassWord;//�˵��������е�ַ(�ر�ע��:��ֵ�������Ķ�)
	Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
	Stack->MenuStack[MenuCount->Count].MoveRow=0;//�˵���������(����ʱ��Ϊ0,����ʱ����)
	Stack->MenuStack[MenuCount->Count].EditRow=5;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
	Stack->MenuStack[MenuCount->Count].EditCol=6;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_Main
// Descriptions:        ���˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_Main(void)//���˵�
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			#if ((Project/100)==3) || ((Project/100)==9)//�����ն�
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_Main);//��ʾ�˵�
			#endif
			#if ((Project/100)==2)//������
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)JZQpMenu_Main);//��ʾ�˵�
//			Stack->MenuStack[MenuCount->Count].EditRow=2;
			#endif
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
						#if ((Project/100)==3) || ((Project/100)==9)//�����ն�&���
							MenuCreate((u32)Menu_1);//�����¼��˵�  1.ʵʱ����
						#endif
						break;
					case 2:
						#if ((Project/100)==3) || ((Project/100)==9)//�����ն�
							MenuCreate((u32)Menu_2);//�����¼��˵�  2.���ܼ���							
						#endif
						#if ((Project/100)==2)//������
							MenuCreate((u32)Menu_1);//���ݲ�ѯ
						#endif
						break;
					case 3:							
						#if ((Project/100)==3) || ((Project/100)==9)//�����ն�
							MenuCreate((u32)Menu_3);//�����¼��˵�  3.�����������							
						#endif
						#if ((Project/100)==2)//������
							PassWordCreateEdit();//��������༭��
						#endif
						break;
					case 4:
						#if ((Project/100)==3) || ((Project/100)==9)//�����ն�
							MenuCreate((u32)Menu_4);//�����¼��˵�  4.ͣ��ͳ��						
						#endif
						#if ((Project/100)==2)//������
							PassWordCreateEdit();//��������༭��
						#endif
						break;
					case 5:
						MenuCreate((u32)Menu_5);//�����¼��˵�  5.�¼���¼
						break;
					case 6:
						MenuCreate((u32)Menu_6);//�����¼��˵�  6.�쳣�澯
						break;
					case 7:						
						PassWordCreateEdit();//��������༭��
						break;
					case 8:
						PassWordCreateEdit();//��������༭��
						break;
					default:
						break;
				}        
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
			break;
		default:
			break;
	}
}