
//���˵�
#include "Project.h"
#include "Menu.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "string.h"
#include "../Display/Draw.h"
#include "../STM32F4xx/STM32F4xx_UART.h"


void _0(void)
{
}

#if ((Project/100)==2 && (USER/100)>1)//�ǹ�������������
//Comm_Ram->RouterType//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��,9=����
const u8 RouterTypeList[][10]=//�ز�·�������б�
{
	"����",//0
	"��̩",//1
	"����",//2
	"����",//3
	"����",//4
	"��˹��",//5
	"����΢",//6
	"Ѷ��",//7
	"Ǯ��",//8
	"����",//9
};
//Comm_Ram->RouterType//·��������:100=����,101=��ϣ,102=��Ѷ��,103=�з�,104=
const u8 RouterTypeWL_List[][10]=//��������·�������б�
{
	"����",//0
	"��ϣ",//1
	"��Ѷ��",//2
	"�з�",//3
};
#endif
#if ((Project/100)==4)//�����ն�
const u8 pE2_Main4[]=//�������˵�
{
	"���˵�\n\r"
	"  1.ʵʱ����\n\r"
	"  2.������ֵ\n\r"
	"  3.���ܱ�ʾ��\n\r"
	"  4.������Ϣ\n\r"
	"  5.�ն���Ϣ\n\r"
	"  6.�ն˲�������\n\r"
	"  7.�ն˹�����ά��\n\r"
};
#endif

#if ((Project/100)==3)//ר���ն�
const u8 pE2_Main3[]=//ר�����˵�
{
	"���˵�\n\r"
	"  1.ʵʱ����\n\r"
	"  2.������ֵ\n\r"
	"  3.����״̬\n\r"
	"  4.���ܱ�ʾ��\n\r"
	"  5.������Ϣ\n\r"
	"  6.������Ϣ\n\r"
	"  7.�ն���Ϣ\n\r"
//	"  8.�ն˲�������\n\r"
//	"  9.�ն˹�����ά��\n\r"
	"  8.�ն˹�����ά��\n\r"
};
#endif

#if ((Project/100)==2)//������
const u8 pE2_Main2[]=//���������˵�
{
	"���˵�\n\r"
	"  1.������������ʾ\n\r"
	"  2.����������鿴\n\r"
	"  3.�ն˹�����ά��\n\r"
//#if ((USER/100)==6)//����ϵ
//	"  4.�ز�����\n\r"
//#endif
#if ((USER/100)>1)//�ǹ�������
	"  4.����ģ�����\n\r"
	"  5.����ģ����Ϣ\n\r"
#endif
};
#endif

	#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
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

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
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
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
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
					i=Stack->MenuStack[MenuCount->Count].EditCol-7;
					i=(Length-i-1)*4;
					MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
					p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
				{
					Stack->MenuStack[MenuCount->Count].EditCol--;
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
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
			UpDownSelect_SH(Stack->MenuStack[MenuCount->Count].EditRow);
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
		case 2://����2���ж������Ƿ���ȷ����ȷ����ת���˵�8��������ʾ�Ի���
			p32=(u32 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_MenuKey,3);
			if((p32[0]&0xffffff)==(MenuCount->v1&0xffffff))
			{//�Ƚ��������������ȷ�������Ƿ����
//�˵�1�µĵ�8�Ӳ˵�Ϊ�ն˹�����ά��
//�˵�1�µĵ�2�Ӳ˵��µĵ�6�Ӳ˵�Ϊ�ն˲����������ѯ
				if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)
				{
					if((MenuCount->Count-1)==1)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_9;//�ն˹�����ά��
					}
				}
				else
				{
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==6)
					{
						if((MenuCount->Count-1)==2)
						{
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;//�ն˲����������ѯ
						}
					}
					else
					{
						MenuDel();//�����ϼ��˵�	    
					}
				} 
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
	#else
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

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
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
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
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
					i=Stack->MenuStack[MenuCount->Count].EditCol-7;
					i=(Length-i-1)*4;
					MenuCount->v1=((MenuCount->v1)&(~(0xf<<i)))+((p8[0]-0x30)<<i);
					p8[0]='*';
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol>Col)
				{
					Stack->MenuStack[MenuCount->Count].EditCol--;
				}
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				memcpy(&x,p16,2);
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
		case 2://����2���ж������Ƿ���ȷ����ȷ����ת���˵�8��������ʾ�Ի���
			p32=(u32 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_MenuKey,3);
			if((p32[0]&0xffffff)==(MenuCount->v1&0xffffff))
			{//�Ƚ��������������ȷ�������Ƿ����
				#if ((Project/100)==3)//ר���ն�
//�˵�1�µĵ�8�Ӳ˵�Ϊ�ն˹�����ά��
//�˵�1�µĵ�2�Ӳ˵��µĵ�8�Ӳ˵�Ϊ�ն˲����������ѯ
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)
					{
						switch(MenuCount->Count-1)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_9;//�ն˹�����ά��
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;//�ն˲����������ѯ
								break;
							default:
								MenuDel();//�����ϼ��˵�	    
								break;
						}
					}
					else
					{
						MenuDel();//�����ϼ��˵�	    
					}
				#endif
				#if ((Project/100)==4)//�����ն�
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==6)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_8;
					}
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==7)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu_9;
					}
					else
					{
						MenuDel();//�����ϼ��˵�	    
					}
				#endif
				#if ((Project/100)==2)//������
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==2)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
					}
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==3)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
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
	#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_0
// Descriptions:        ��ʾδ���±�̼�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

//void Menu_0(void)
//{
//	DisplayClr();
//	DisplayString(4,4,0,"��̼�δ����");
//	DisplayString(5,1,0,"�밴��̼������ؼ�");
//	Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
//	if((Comm_Ram->ButtonVal&0x20)!=0 || Comm_Ram->Factory==0x55 || MRR(ADDR_MeterFMTimer,2))
//	{//���ؼ�������ģʽ����̼�
//		MenuDel();//�����ϼ��˵�
//	}
//	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
//}

//	#if ((Project/100)==2 && (USER/100)==6)//����ϵ������
	#if ((Project/100)==2 && (USER/100)>1)//�ǹ�������������
//------------------------------------------------------------------------------------------------
// Function name:       Menu2_HN_4
// Descriptions:        ���ϼ������˵�4 �ز�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_HN_4(void)
{
	u64 i;
	u32 j;
//	u32 cs;
	u8 *p8;
	u8 *p8d;
	u16 *p16;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS232PORT);//�õ�UART��������ַ
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(RS232PORT);//�õ�UART���ջ����ַ
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(RS232PORT);//�õ�UART���ͻ����ַ
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"����ģ�����\n\r"
																			"Ŀ���ַ\n\r"
																			"      ȷ�Ϸ���\n\r"
																			);//��ʾ�˵�
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+8*2);
//			DisplayData(0x434030100004,12,0,p16);
			DisplayData(0x000000000001,12,0,p16);
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(2,6,0,"ȷ�Ϸ���");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=19;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//���
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						//�ز���������ת��F9 AFN0CF129����ʾֵ
						//���裺
						//1.�������Ϊ0ʱ����֡д�����ջ�����
						//2.�������1
						//3.�������Ϊ0ʱ���ӷ��ͻ�������ȡ����
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
							Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
						}
						if(Stack->MenuStack[MenuCount->Count].EditRow==2)
						{
							if(UARTCtrl->Task==0)
							{
/*
�ն˽���AFN10F9��֡��ʽ
68 
A6 00 //L
A6 00 //L
68 
7B //C
34 12 01 00 //�ն˵�ַ������������
02 //
10 //AFN
61 //SEQ
00 00 01 01 //���ݵ�Ԫ��ʶ
1F //�ն�ͨ�Ŷ˿ں�
00 //ת���м̼���
12 90 78 56 34 12 //6�ֽ� ת��Ŀ���ַ
01 //ת��ֱ�ӳ��������ݱ�ʶ���ͣ�0������DLT 645-1997,1������DL/T 645-2007
00 FF 01 00 //4�ֽ� ת��ֱ�ӳ��������ݱ�ʶ
90 78 56 34 12 90 78 56 34 12 90 78 56 34 12 90 //16�ֽ� PW
89 //1�ֽ� У���CS
16 
*/
								p8=(u8 *)ADDR_DATABUFF;
								p8[0]=0x68;
								p8[1]=0xA6;//L
								p8[2]=0x00;
								p8[3]=0xA6;//L
								p8[4]=0x00;
								p8[5]=0x68;
								p8[6]=0x7B;//C
								MR(((u32)ADDR_DATABUFF)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
								p8[11]=0x02;
								p8[12]=0x10;//AFN									 
								p8[13]=0x61;//SEQ									 
								p8[14]=0x00;//���ݵ�Ԫ��ʶ									 
								p8[15]=0x00;									 
								p8[16]=0x01;									 
								p8[17]=0x01;
								p8[18]=0x1F;//�ն�ͨ�Ŷ˿ں�
								p8[19]=0x00;//ת���м̼���
								//ת��Ŀ���ַ
								p8d=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
								for(i=0;i<6;i++)
								{
									p8[25-i]=(p8d[4*i]-0x30)*16+(p8d[4*i+2]-0x30);
								}

								p8[26]=0x01;//ת��ֱ�ӳ��������ݱ�ʶ���ͣ�0������DLT 645-1997,1������DL/T 645-2007									 
								p8[27]=0x00;//ת��ֱ�ӳ��������ݱ�ʶ
								p8[28]=0x00;									 
								p8[29]=0x01;
								p8[30]=0x00;
								for(i=0;i<16;i++)
								{
									p8[31+i]=0x00;//16�ֽ�PW
								}
//								//У���CS����
//								cs=0;
//								for(i=0;i<41;i++)
//								{
//									cs+=p8[6+i];
//								}
//								cs&=0xff;
//								p8[47]=cs;
//								p8[48]=0x16;

								p8[1]=0x29;//ȥ��־λD1D0,ͬʱ�������ֻ��0-16383
//								MW(ADDR_DATABUFF,(u32)p8rxbuff,49);									 
								MW(ADDR_DATABUFF,(u32)p8rxbuff,47);									 

								UARTCtrl->Task=1;
								Stack->MenuStack[MenuCount->Count].Task=2;
							}
						}
						Comm_Ram->ButtonVal&=~0x1F;//��ʹ�ú���0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//��ʹ�ú���0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							default:
								break;
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//��ʹ�ú���0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x0F;//��ʹ�ú���0
						break;
					}
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		case 2:
//�ӷ��ͻ�������ȡ����
			if(UARTCtrl->Task==0)
			{
				DisplayClr();//��ȫ��
				DisplayMenuString(4,0,2,(u8 *)
												"�����й�����(kWh):\n\r"
												"  �� =\n\r"
												);//��ʾ�˵��ַ���
/*
�ն˷���AFN10F9��֡��ʽ
68H
L
L
68H
C
A
AFN=10H
SEQ
���ݵ�Ԫ��ʶ
���ݵ�Ԫ
						ת��ͨ�Ŷ˿ں� 1BIN
						ת��Ŀ���ַ 6
						ת�������־ 1BIN
						ת��ֱ�ӳ��������������ֽ���k+4 1BIN
						ת��ֱ�ӳ��������ݱ�ʶ 4BIN
						ת��ֱ�ӳ������������� K
EC����������4.3.4.6.3��
Tp����������4.3.4.6.4��
CS
16H
*/
				p8=(u8 *)ADDR_DATABUFF;
				MR(ADDR_DATABUFF,(u32)p8txbuff,40);
				if(p8[25]==5)
				{//ת����������
					i=MRR((u32)p8txbuff+31,4);	                   
					p16=(u16 *)(ADDR_STRINGBUFF+4+5*84+6*2);
					DisplayData_OffZero(i,8,2,p16);
				}
				else
				{
					DisplayString(5,8,0,"------");
				}
			}
			else
			{
				DisplayClr();//��ȫ��
				DisplayMenuString(4,0,2,(u8 *)
												"     �ز�������\n\r"
												"   �����ĵȴ����\n\r"
												);//��ʾ�˵��ַ���
			}
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{//������������ϼ��˵�
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
			break;
		default:
			break;
	}
}

void Menu2_SX_5(void)//���������� �ز���Ϣ
{
	u32 Row;
	u32 i;
	u32 x;
	u8 *p8;
	u8 *p;
	u16 *p16;

	Row=1;
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"����ģ����Ϣ\n\r"
																	"ģ�鳧��:\n\r"
																	"���̴���:\n\r"
																	"�������:\n\r"
																	"����汾:\n\r"
																	);//��ʾ�˵��ַ���
	i=Comm_Ram->RouterType;
	if(i<101)
	{//�ز�ģ��
//Comm_Ram->RouterType//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��,9=����
		//�ز�����
		if(i>0 && i<=9)
		{
			DisplayString(Row,9,0,(u8 *)RouterTypeList[i]);
		}
		else
		{
			DisplayString(Row,9,0,(u8 *)"δ֪\n\r");
		}
	}
	else
	{//΢��������ģ��
//Comm_Ram->RouterType//·��������:100=����,101=��ϣ,102=��Ѷ��,103=�з�,104=
		//�������߳���
		i-=100;
		if(i>0 && i<=3)
		{
			DisplayString(Row,9,0,(u8 *)RouterTypeWL_List[i]);
		}
		else
		{
			DisplayString(Row,9,0,(u8 *)"δ֪\n\r");
		}
	}


	//���̴���
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p8=(u8 *)&Terminal_Router->RouterAddr+6;
	p[0]=p8[0];
	p[1]=0x20;
	p[2]=p8[1];
	p[3]=0x20;

	//�������
	Row++;
	p8=(u8 *)&Terminal_Router->RouterAddr+10;
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p[0*2]=(p8[2]>>4)+0x30;//��
	p[1*2]=(p8[2]&0xf)+0x30;
	p[2*2]='-';
	p[3*2]=(p8[1]>>4)+0x30;//��
	p[4*2]=(p8[1]&0xf)+0x30;
	p[5*2]='-';
	p[6*2]=(p8[0]>>4)+0x30;//��
	p[7*2]=(p8[0]&0xf)+0x30;
	for(i=0;i<=7;i++)
	{
		p[i*2+1]=0x20;
	}
	//����汾
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	x=MRR((u32)&Terminal_Router->RouterAddr+13,2);
	p16=DisplayData(x,4,0,p16);

	if((Comm_Ram->ButtonVal&0x20)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}
	#endif
//-------------------------------------------------------------------------------------------------
// Function name:       E2_Main
// Descriptions:        ���˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_Main(void)//���˵�
{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	u8 * p8;
	p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
	p8[0]=1;//�˵���ʾ���������ã�0=��1=��
#endif

#if (!((Project/100)==3 && (USER/100)==5))//���Ϻ�ר��
	u16 *p16;
	p16=(u16 *)(ADDR_STRINGBUFF+9*84+4+18*2);
#endif
	DOTMIXPMODE->Mix=2;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
	Draw_Clr();
	#if ((Project/100)==3)//ר���ն�
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main3);//��ʾ�˵�
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
							MenuCreate((u32)Menu_1);//�����¼��˵�
							break;
						case 2:
							MenuCreate((u32)Menu_2);//�����¼��˵�
							break;
						case 3:
							MenuCreate((u32)Menu_3);//�����¼��˵�
							break;
						case 4:
							MenuCreate((u32)Menu_4);//�����¼��˵�
							break;
						case 5:
							MenuCreate((u32)Menu_5);//�����¼��˵�
							break;
						case 6:
							MenuCreate((u32)Menu_6);
							break;
						case 7:
							MenuCreate((u32)Menu_7);//�����¼��˵�
							break;
						case 8:
							//���±�̼���������ȷ���룬���ɽ������ò˵�
							//����ģʽ�£���ʱ�ɽ������ò˵�
							if(Comm_Ram->Factory==0x55)
							{//����״̬
								MenuCreate((u32)Menu_9);//�����¼��˵�
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
	#endif
	#if ((Project/100)==4)//�����ն�
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main4);//��ʾ�˵�
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
							MenuCreate((u32)Menu_1);//�����¼��˵�
							break;
						case 2:
							MenuCreate((u32)Menu_2);//�����¼��˵�
							break;
						case 3:
							MenuCreate((u32)Menu_4);//�����¼��˵�
							break;
						case 4:
							MenuCreate((u32)Menu_5);//�����¼��˵�
							break;
						case 5:
							MenuCreate((u32)Menu_7);
							break;
						case 6:
						case 7:
							//���±�̼���������ȷ���룬���ɽ���8��9�Ӳ˵�
							//����ģʽ�£���ʱ�ɽ���8��9�Ӳ˵�
							if(Comm_Ram->Factory==0x55)
							{//����״̬
								if(Stack->MenuStack[MenuCount->Count].EditRow==6)
								{
									MenuCreate((u32)Menu_8);//�����¼��˵�
								}
								if(Stack->MenuStack[MenuCount->Count].EditRow==7)
								{
									MenuCreate((u32)Menu_9);//�����¼��˵�
								}
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
	#endif
	#if ((Project/100)==2)//������
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main2);//��ʾ�˵�
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
							MenuCreate((u32)Menu_4);//�����¼��˵� ���ܱ�ʾ��/������������������ʾ
							break;
						case 2:
						case 3:
							//���±�̼���������ȷ���룬���ɽ���3��4�Ӳ˵�
							//����ģʽ�£���ʱ�ɽ���3��4�Ӳ˵�
							if(Comm_Ram->Factory==0x55)
							{//����״̬
								if(Stack->MenuStack[MenuCount->Count].EditRow==2)
								{
									MenuCreate((u32)Menu2_2);//�����¼��˵� ����������������鿴
								}
								if(Stack->MenuStack[MenuCount->Count].EditRow==3)
								{
									MenuCreate((u32)Menu2_3);//�����¼��˵�	�������ն˹�����ά��
								}
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
//#if ((USER/100)==6)//����ϵ
#if ((USER/100)>1)//�ǹ�������
						case 4:
//							MenuCreate((u32)Menu2_HN_4);//�����¼��˵� ���ϼ������˵�4 �ز�����
							if(MenuCount->Count>=MaxMenuCount)
							{
								return;
							}
							MenuCount->Count++; 
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_HN_4;//�˵��������е�ַ(�ر�ע��:��ֵ�������Ķ�)	���ϼ������˵�4 �ز�����
							Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
							Stack->MenuStack[MenuCount->Count].MoveRow=0;//�˵���������(����ʱ��Ϊ0,����ʱ����)
							Stack->MenuStack[MenuCount->Count].EditRow=2;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
							Stack->MenuStack[MenuCount->Count].EditCol=0;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
							break;
						case 5://�ز���Ϣ ����Ҫ��
							MenuCreate((u32)Menu2_SX_5);//�����¼��˵�	���������� �ز���Ϣ
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
//	#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
//		if(MRR(ADDR_MeterFMTimer,2))
//		{
//			p16[0]=23;// '��̰���'
//			p16[1]=24;
//		}
//		else
//		{
//			p16[0]=0x2000;
//			p16[1]=0x2000;
//		}
//	#else
//		if(MRR(ADDR_TerminalFMTimer+12,2))
//		{
//			p16[0]=23;// '��̰���'
//			p16[1]=24;
//		}
//		else
//		{
//			p16[0]=0x2000;
//			p16[1]=0x2000;
//		}
//	#endif
#if (!((Project/100)==3 && (USER/100)==5))//���Ϻ�ר��
	//����ģʽ(���������ü�)
		if(Comm_Ram->Factory==0x55)//����״̬
		{
			p16[0]=25;// '����ģʽ'
			p16[1]=26;
		}
		if(Stack->MenuStack[MenuCount->Count].EditRow==9)
		{
			p16[0]|=0x8000;
			p16[1]|=0x8000;
		}
#endif
}


/*
void DisplayMenu(u32 MoveRow,u8 * pStr)//��ʾ�˵�
---------------void MenuCreate(u32 FunctionAddr)//�����¼��˵�------------
---------------void MenuDel(void)//�����ϼ��˵�---------------------------
void MenuSelect(void)//�˵�ѡ��

void DisplayClr(void)//��ȫ��
void DisplayClrRow(u32 StartRow,u32 EndRow)//����Ļ��ʾ��ָ����
u32 DisplayString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//��ʾ�ַ���
u32 DispalyStringOnlyOneRow(u32 Row,u32 Col,u8 * pStr)//��ʾ�ַ�������1����
u16 * DisplayData(u64 Num,u32 BitNum,u32 DpNum,u16 * p16)//������ʾ(������),���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF

void MR(u32 ADDR_RAM,u32 ADDR_FLASH,u32 Byte);//�洢����
void MW(u32 ADDR_RAM,u32 ADDR_FLASH,u32 Byte);//�洢��д
void MEMClr(u8 Data,u32 ADDR_FLASH,u32 Byte);//�洢�����
u64 MRR(u32 ADDR_FLASH,u8 Byte);//�洢�������Ĵ���(<=8�ֽ�)
void MEMWriteReg(u64 Data,u32 ADDR_FLASH,u8 Byte);//�Ĵ���д���洢��(<=8�ֽ�)


//��д���ݻ���(���������Ͷ�ջ����)
#define LEN_DATABUFF     4096
#define ADDR_DATABUFF     (ADDR_Terminal_Ram_Start+LEN_Terminal_Ram)
//�����ֶ����4096�ֽ�



Comm_Ram->ButtonVal//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�,B6=����,B7=
Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��

typedef __packed struct
{
  u8 Count;//�˵���ջ����
  u8 Res1;
  u8 Res2;
  u8 Res3;
  u32 v1;//�˵��ñ���1
  u32 v2;//�˵��ñ���2
  u32 v3;//�˵��ñ���3
}MenuCount_TypeDef;
#define MenuCount     ((MenuCount_TypeDef *)(ADDR_MenuStack))
#define MaxMenuCount     10//���˵�����
typedef __packed struct
{
  u32 PC;//�˵��������е�ַ(�ر�ע��:��ֵ�������Ķ�)
  u8 Task;//�˵��������(������˳�ʱ����Ϊ0)
  u8 MoveRow;//�˵���������(����ʱ��Ϊ0,����ʱ����)
  u8 EditRow;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
  u8 EditCol;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
  u8 TotalRow;//�˵�������
  u8 MenuRow;//�˵���ǰװ������
  u8 res2;
  u8 res3;
}MenuSave_TypeDef;
typedef __packed struct
{
  MenuSave_TypeDef     MenuStack[MaxMenuCount+1];
}MenuStack_TypeDef;
#define Stack     ((MenuStack_TypeDef *)(ADDR_MenuStack+16))



typedef __packed struct
{
  u8 DotLn;//�����м�����(0-255)
  u8 Row;//�ַ�����(0-9)(1���ַ��������Ͽɷֶ�����ʾ)
  u8 Font;//���������0-4
  u8 Align;//���뷽ʽ,0=�����,1=����,2=�Ҷ���
  u16 Code[40];//��ʾ����
               //��ʾ����ÿ���ֽڴ���8*16����,ÿ����ʾ����80�ֽ�40������
               //D15=1��ʾ����
              //B14=0B13=0���Խ�С�ֿ�ȡ����,����ſռ�0-0X2000=8192
              //B14=0B13=1,B12-B0=0-255��ASCII8*16��׼�ֿ�ȡ����,����ſռ�=256
              //B14=0B13=1,B12-B0=256-511��ASCII8*12��׼ȡ����(�ݲ���),����ſռ�=256
              //B14=0B13=1,B12-B0=512-767��ASCII8*8��׼ȡ����(�ݲ���),����ſռ�=256
              //B14=1B13-B0��ʾ��GB2312��׼�ֿ�(�޸ĺ�)ȡ����,����ſռ�0-0X4000
}StrLnBuff_TypeDef;
typedef __packed struct
{
  StrLnBuff_TypeDef Row[16];
}StrBuff_TypeDef;
#define StrBuff     ((StrBuff_TypeDef *)(ADDR_STRINGBUFF))


*/

/*
�˵������ü��ж������£�
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
	if(MRR(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
#else
	if(MRR(ADDR_TerminalFMTimer+12,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
#endif

  	if(Comm_Ram->Factory=0x55)//����״̬
	{
		p16[0]=25;// '����ģʽ'
		p16[1]=26;
	}
	//�ڹ���ģʽ�£����ü��������������Ч�ġ�

*/
/*
������ 13:57:04
  Terminal_Ram->AFN04F10UpDate=0;//0=AFN04F10�ն˵��ܱ�/��������װ�����ò������±�־,!=0û����
������ 14:33:48
  Terminal_Ram->AFN04F10F11F13UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
������ 14:34:30
  ԭTerminal_Ram->AFN04F10UpDate�����ˡ�
������ 10:47:13
  #define IC_MAC     0//0=û,1=��
������ 18:50:41
  IC_MAC�Ѹ�ΪIC_24AAxxx
*/
/*
Comm_Ram->RouterType//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=
(u32)&Terminal_Router->RouterAddr
typedef __packed struct
{
 u8 RouterAddr[6];//·�����ڵ��ַ(���Ƿ�����ǰ,ͨ�ſ��ַ��λ��)
 u8 RouterCODE[4];//���̴��� ASCII 2,оƬ���� ASCII 2
 u8 RouterVer[5];//·�����汾��,�ֱ�Ϊ:��,��,��,2BYTE�汾
*/






