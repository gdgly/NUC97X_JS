
//�˵�9 ϵͳ����
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_UART.h"



const u8 pMenu_9[]=
{
	"�ն˹�����ά��\n\r"
	"  1.�����ն�F1\n\r"
	"  2.��������ʼ��F2\n\r"
	"  3.�ָ���������F3\n\r"
	"  4.��������ʼ��F4\n\r"
	"  5.�������\n\r"
	"  6.485�ڹ�������\n\r"
};

const u8 pMenu_9_6[]=
{
	"485�ڹ�������\n\r"
	"485�� ·\n\r"
	"����:\n\r"
	"      ȷ������\n\r"
};

const u8 Function_485List[][20]=//485�ڹ����б�
{
	"�ն�ά����",
	"�ն˳����",
};

const u8 pMenu_9_5[]=
{
	"�˵�����\n\r"
	"����:\n\r"
	"      ȷ������\n\r"
};

#if ((Project/100)==2)//������
const u8 pMenu2_3[]=
{
	"�ն˹�����ά��\n\r"
	"  1.�����ն�F1\n\r"
	"  2.��������ʼ��F2\n\r"
	"  3.�ָ���������F3\n\r"
	"  4.��������ʼ��F4\n\r"
	"  5.�������汾\n\r"
	"  6.�ز��������\n\r"
	"  7.�ֳ�����\n\r"
	"  8.�豸�������\n\r"
#if ((USER/100)==5)//�Ϻ�������
	"  9.�ֶ�����\n\r"
#endif
#if ((USER/100)==8)//ͨ�ü�����
	"  9.����ͨ�ŵ�ַ\n\r"
#endif
};

#if ((USER/100)==2)//ɽ����
const u8 Function_232List[][20]=//232�ڹ����б�
{
	"�ն˼���",
	"�ն�ά����",
};
const u8 pMenu2_3_7_2[]=
{
	"232�ڹ�������\n\r"
	"���ܣ�\n\r"
	"      ȷ������\n\r"
};
const u8 pMenu2_3_7[]=
{
	"�ֳ�����\n\r"
	"  1.485�ڹ�������\n\r"
	"  2.232�ڹ�������\n\r"
};
#else
	#if (Project/100)==2//������
const u8 pMenu2_3_7[]=
{
	"�ֳ�����\n\r"
	"  1.485�ڹ�������\n\r"
	"  2.�Զ��ѱ�\n\r"
};
const u8 pMenu2_3_7_2[]=
{
	"�Զ��ѱ�\n\r"
	"  1.�ѱ�ر�\n\r"
	"  2.���ò�����F10\n\r"
	"  3.���ò�����F10\n\r"
};
	#else
const u8 pMenu2_3_7[]=
{
	"�ֳ�����\n\r"
	"  1.485�ڹ�������\n\r"
};
	#endif
#endif

const u8 pMenu2_3_5[]=
{
	"�������汾\n\r"
	"����汾:\n\r"
	"Ӳ���汾:\n\r"
};

const u8 pMenu2_3_6_4[]=
{
	"����������\n\r"
	"������:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_3_6_5[]=
{
	"����ʱ������\n\r"
	"ʱ����:\n\r"
};

const u8 pMenu2_3_6[]=
{
	"�ز��������\n\r"
	"  1.��ͣ����F49\n\r"
	"  2.�ָ�����F50\n\r"
	"  3.���³���F51\n\r"
	"  4.����������\n\r"
	"  5.����ʱ������\n\r"
};
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_1
// Descriptions:        �����ն�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_9_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"�ն�����");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					p8=(u8 *)ADDR_DATABUFF;
					for(i=0;i<=3;i++)
					{
						p8[i]=0x55;
					}
					p8[4]=1;
					MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_2
// Descriptions:        ��������ʼ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_9_2(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,5,0,"���ݳ�ʼ��");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					//��������ʼ��
					p8=(u8 *)ADDR_DATABUFF;
					for(i=0;i<=3;i++)
					{
						p8[i]=0x55;
					}
					p8[4]=2;
					MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					DisplayString(4,4,0,"���ݳ�ʼ����");
					DisplayString(5,6,0,"��ȴ�...");
				}
			}
			if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			{
				DisplayString(4,4,0,"���ݳ�ʼ��OK");
				DisplayString(5,4,0,"�����ؼ��˳�");
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_3
// Descriptions:        �ָ���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_9_3(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"�ָ���������");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					//�ָ���������
					p8=(u8 *)ADDR_DATABUFF;
					for(i=0;i<=3;i++)
					{
						p8[i]=0x55;
					}
					p8[4]=3;
					MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					DisplayString(4,3,0,"�ָ�����������");
					DisplayString(5,6,0,"��ȴ�...");
				}
			}
			if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			{
				DisplayString(4,3,0,"�ָ���������OK");
				DisplayString(5,4,0,"�����ؼ��˳�");
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_4
// Descriptions:        ��������ʼ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_9_4(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,5,0,"������ʼ��");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					//��������ʼ��
					p8=(u8 *)ADDR_DATABUFF;
					for(i=0;i<=3;i++)
					{
						p8[i]=0x55;
					}
					p8[4]=4;
					MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					DisplayString(4,4,0,"������ʼ����");
					DisplayString(5,6,0,"��ȴ�...");
				}
			}
			if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			{
				DisplayString(4,4,0,"������ʼ��OK");
				DisplayString(5,4,0,"�����ؼ��˳�");
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_5
// Descriptions:        ����Ӳ˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_9_5(void)
{
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u8 *p8;
	u16 *p16;
	u32 *p32;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_9_5);//��ʾ�˵�
			/*��ʾ�ڴ�������*/
			x=MRR(ADDR_MenuKey,3);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
			DisplayData(x,6,0,p16);
			MenuCount->v1=5;
			MenuCount->v2=10;
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(Ln+2,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=10;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=(Ln+Stack->MenuStack[MenuCount->Count].EditRow))
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8_add1(p8,6);
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//���
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8_sub1(p8,6);
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 2:
								DisplayString(Ln+2,6,0,"   OK   ");
								/*�������õ�����*/
								MR(ADDR_DATABUFF,ADDR_MenuKey,3);//���˵�����
								p32=(u32 *)ADDR_DATABUFF;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								p32[0]=(p8[0]-0x30)*16*16*16*16*16+(p8[2]-0x30)*16*16*16*16+(p8[4]-0x30)*16*16*16+(p8[6]-0x30)*16*16+(p8[8]-0x30)*16+(p8[10]-0x30)*1;
								MW(ADDR_DATABUFF,ADDR_MenuKey,3);//д�˵�����
								break;
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
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
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_6_1
// Descriptions:        ����״̬�½�ֹ485�ڹ���������ʾ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_9_6_1(void)
{
	DisplayClr();
	DisplayString(4,1,0,"����״̬�½�ֹ����");
	DisplayString(5,3,0,"�밴���ؼ��˳�");
	Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//���ؼ�
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9_6
// Descriptions:        485�ڹ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_9_6(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Num485;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

//ר�䡢������Ϊ2·485�� ���䡢�ɼ���Ϊ3·485
#if ((Project/100)==3 || (Project/100)==2)//ר���նˡ�������
	Num485=2;
#endif
#if ((Project/100)==4)//�����ն�
	Num485=3;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_9_6);//��ʾ�˵�
			if(MenuCount->v3>Num485)
			{//MenuCount->v3��¼��n·485
				MenuCount->v3=Num485;
			}
			if(MenuCount->v3<1)
			{
				MenuCount->v3=1;
			}		
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
			bcd_p8(p8,MenuCount->v3,1);
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=5;
			MenuCount->v2=5;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//��ʾ�ڴ�������
			//��n·485���ܣ��ն˳���or���ܱ�ڣ�//0 �ն�ά���� ��0 �ն˳����
			x=MRR(ADDR_UART_PORT_SET+(4*MenuCount->v3),1);
			if(x!=0)
			{
				x=1;
			}
			DisplayString(Ln+2,5,0,(u8 *)Function_485List[x]);//0 �ն�ά���� 1 �ն˳����
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=3)
					{
						DisplayString(Ln+3,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=5;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=14;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=(Ln+Stack->MenuStack[MenuCount->Count].EditRow))
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,1);
								if(x<Num485)
								{
									p8_add1(p8,1);
								}
								else
								{
									bcd_p8(p8,1,1);
								}
								MenuCount->v3=p8_bcd(p8,1);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								switch(p8[0])
								{
									case 0xE8://�ն˳����
										i=0;
										break;
									case 0xBA://�ն�ά����
										i=1;
										break;
									default:
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)Function_485List[i]);//0 �ն�ά���� 1 �ն˳����
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,1);
								if(x>1)
								{
									p8_sub1(p8,1);
								}
								else
								{
									bcd_p8(p8,Num485,1);
								}
								MenuCount->v3=p8_bcd(p8,1);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								switch(p8[0])
								{
									case 0xE8://�ն˳����
										i=0;
										break;
									case 0xBA://�ն�ά����
										i=1;
										break;
									default:
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)Function_485List[i]);//0 �ն�ά���� 1 �ն˳����
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 3:
								DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
//#if ((Project/100)==3 && (USER/100)==5)//ר���ն����û���ʶ:�Ϻ���
//								//RS485-1����������ģʽ��ֻ���г���
//								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"RS485-1ֻ����");
//#else
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
//#endif
								//�������õ�����
								p=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_UART_PORT_SET+(4*MenuCount->v3),1);//��485�ڹ���
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
								switch(p8[0])
								{
									case 0xE8://�ն˳����
										p[0]=1;
										break;
									case 0xBA://�ն�ά����
										p[0]=0;
										break;
								}
//#if ((Project/100)==3 && (USER/100)==5)//ר���ն����û���ʶ:�Ϻ���
//								//RS485-1����������ģʽ��ֻ���г���
//								if(MenuCount->v3==2)
//								{//RS485-2
//									if(p[0]==0)//�ն�ά����
//									{
//										MWR(180,ADDR_TerminalFMTimer+42,2);
//									}
//									MW(ADDR_DATABUFF,ADDR_UART_PORT_SET+(4*MenuCount->v3),1);//д485�ڹ���
//								}
//#else
//								MW(ADDR_DATABUFF,ADDR_UART_PORT_SET+(4*MenuCount->v3),1);//д485�ڹ���
//#endif
#if ((USER/100)==5)//�û���ʶ:�Ϻ���
								if(MenuCount->v3==2)
								{//RS485-2
									if(p[0]==0)//�ն�ά����
									{
										MWR(180,ADDR_TerminalFMTimer+42,2);
									}
								}
#endif
								MW(ADDR_DATABUFF,ADDR_UART_PORT_SET+(4*MenuCount->v3),1);//д485�ڹ���
								break;
							default:
								break;
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,1);
								if(x>Num485 || x<1)
								{
									bcd_p8(p8,1,1);
								}
								MenuCount->v3=p8_bcd(p8,1);
								Stack->MenuStack[MenuCount->Count].Task=0;								
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
		default:
			break;
	}
}
#if ((Project/100)==3 || (Project/100)==4)//ר���նˡ������ն�
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_9
// Descriptions:        ϵͳ����˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_9(void)//�˵�9
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
//#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
//			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
//																			"�ն˹�����ά��\n\r"
//																			"  1.�����ն�F1\n\r"
//																			"  2.��������ʼ��F2\n\r"
//																			"  3.�ָ���������F3\n\r"
//																			"  4.��������ʼ��F4\n\r"
//																			"  5.�������\n\r"
//																			);//��ʾ�˵�
//#else
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_9);//��ʾ�˵�
//#endif
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_9_1);//�����¼��˵�
						break;
					case 2:
						MenuCreate((u32)Menu_9_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu_9_3);//�����¼��˵�
						break;
					case 4:
						MenuCreate((u32)Menu_9_4);//�����¼��˵�
						break;
					case 5:
						MenuCreate((u32)Menu_9_5);//�����¼��˵�
						break;
					case 6:
//#if (!((Project/100)==3 && (USER/100)==5))//���Ϻ�ר��
						if(Comm_Ram->Factory!=0x55)
						{//�ǹ���״̬
							MenuCreate((u32)Menu_9_6);//�����¼��˵� 485�ڹ�������
						}
						else//����״̬
						{
							MenuCreate((u32)Menu_9_6_1);//�����¼��˵� ����״̬�½�ֹ485�ڹ�������
						}
//#endif
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

#if ((Project/100)==2)//������
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_5
// Descriptions:        �������汾
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_5(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i;
	u32 Row=1;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_5);//��ʾ�˵�
	//1.����汾
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF1+12,4);
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	for(i=0;i<=3;i++)
	{
		p[i*2]=p8[i];
		p[i*2+1]=0x20;
	}
	Row++;
	//2.Ӳ���汾
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p16[0]=0x2000+'V';
	p16[1]=(HVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((HVER>>4)&0xf)+0x2030;
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6_5
// Descriptions:        ����ʱ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_5(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 y;
	u32 z;
	u32 Row=1;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_5);//��ʾ�˵�
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=7;
			MenuCount->v2=7;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//�˿�31������ʱ����
			x=MRR(ADDR_AFN04F33+((14+(4*24))*30)+13,1);
			if(x>7)
			{
				x=7;
			}
			MenuCount->v3=x;//��¼����ʱ����
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+7*2);
			DisplayData(x,1,0,p16);
			Row++;
			if(x!=0)
			{
				for(i=0;i<x;i++)
				{
					y=MRR(ADDR_AFN04F33+((14+(4*24))*30)+14+4*i,2);//��i+1ʱ�ο�ʼʱ��
					p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+0*2);
					p16[0]=i+1+0x2030;
					p16[1]='.'+0x2000;
					if(y==0xEEEE)
					{//�޴�������-----
						DisplayString(Row,2,0,"-----");	
					}
					else
					{
						DisplayData1(y,4,2,':',p16+2);
					}
					p16[7]='-'+0x2000;	
					z=MRR(ADDR_AFN04F33+((14+(4*24))*30)+14+4*i+2,2);//��i+1ʱ�ν���ʱ��
					if(z==0xEEEE)
					{//�޴�������-----
						DisplayString(Row,8,0,"-----");	
					}
					else
					{
						DisplayData1(z,4,2,':',p16+8);
					}
					Row++;
				}
			}
			for(i=0;i<=MenuCount->v3+2;i++)
			{
				StrBuff->Row[i].Row=i;
			}
			DisplayString(Row,6,0,"ȷ������");
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				for(i=0;i<=MenuCount->v3+2;i++)
				{
					StrBuff->Row[i].Row=i;
				}
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=(MenuCount->v3+2))
					{
						DisplayString(MenuCount->v3+2,6,0,"ȷ������");
					}
					if(Stack->MenuStack[MenuCount->Count].EditRow==1)
					{
						MenuCount->v1=7;
						MenuCount->v2=7;
						ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
						NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					}
					else
					{
						for(k=0;k<10;k++)
						{
							if(k!=Stack->MenuStack[MenuCount->Count].EditRow)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
								if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
								{
									for(i=2;i<=3;i++)
									{
										p16[i]=(p16[i]&0x8000)+0x2020;
									}
								}
							}
						}
						if(Stack->MenuStack[MenuCount->Count].EditRow==MenuCount->v3+2)
						{
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
						}
						else
						{
							for(i=0;i<MenuCount->v3;i++)
							{
								MenuCount->v1=2;
								MenuCount->v2=12;
								NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							}
						}
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
							p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
							x=p8_bcd(p8,1);
							if(x<7)
							{
								p8_add1(p8,1);
							}
							else
							{
								bcd_p8(p8,0,1);
							}
							x=p8_bcd(p8,1);
							if(x>MenuCount->v3)
							{
								DisplayClrRow(MenuCount->v3+2,MenuCount->v3+2);//����Ļ��ʾ��ָ����
								y=x-MenuCount->v3;
								for(i=0;i<y;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(MenuCount->v3+2+i)*84+4+0*2);
									p16[0]=MenuCount->v3+1+i+0x2030;
									p16[1]='.'+0x2000;
									p16[2]=0x2030;
									p16[3]=0x2030;
									p16[4]=':'+0x2000;
									p16[5]=0x2030;
									p16[6]=0x2030;
									p16[7]='-'+0x2000;
									p16[8]=0x2030;
									p16[9]=0x2030;
									p16[10]=':'+0x2000;
									p16[11]=0x2030;
									p16[12]=0x2030;
								}
								DisplayString(x+2,6,0,"ȷ������");
								MenuCount->v3=x;
							}
							if(x<MenuCount->v3)
							{
								DisplayClrRow(x+2,MenuCount->v3+2);//����Ļ��ʾ��ָ����
								DisplayString(x+2,6,0,"ȷ������");
								MenuCount->v3=x;
							}
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//���
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
							p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
							x=p8_bcd(p8,1);
							if(x>0)
							{
								p8_sub1(p8,1);
							}
							else
							{
								bcd_p8(p8,7,1);
							}
							x=p8_bcd(p8,1);
							if(x>MenuCount->v3)
							{
								DisplayClrRow(MenuCount->v3+2,MenuCount->v3+2);//����Ļ��ʾ��ָ����
								y=x-MenuCount->v3;
								for(i=0;i<y;i++)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+(MenuCount->v3+2+i)*84+4+0*2);
									p16[0]=MenuCount->v3+1+i+0x2030;
									p16[1]='.'+0x2000;
									p16[2]=0x2030;
									p16[3]=0x2030;
									p16[4]=':'+0x2000;
									p16[5]=0x2030;
									p16[6]=0x2030;
									p16[7]='-'+0x2000;
									p16[8]=0x2030;
									p16[9]=0x2030;
									p16[10]=':'+0x2000;
									p16[11]=0x2030;
									p16[12]=0x2030;
								}
								DisplayString(x+2,6,0,"ȷ������");
								MenuCount->v3=x;
							}
							if(x<MenuCount->v3)
							{
								DisplayClrRow(x+2,MenuCount->v3+2);//����Ļ��ʾ��ָ����
								DisplayString(x+2,6,0,"ȷ������");
								MenuCount->v3=x;
							}
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						if(Stack->MenuStack[MenuCount->Count].EditRow<(MenuCount->v3+2) && Stack->MenuStack[MenuCount->Count].EditRow>1)
						{
							Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
							DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
						}
						else
						{
							//�������õ�����
							DisplayClrRow_4(MenuCount->v3+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
							DisplayString(MenuCount->v3+2,9,0,"OK");
							p8=(u8 *)ADDR_DATABUFF;
							MR(ADDR_DATABUFF,ADDR_AFN04F33+((14+(4*24))*30)+13,1+4*24);
							p=(u8 *)(ADDR_STRINGBUFF+1*84+4+7*2);
							p8[0]=p[0]-0x30;
							for(i=0;i<MenuCount->v3;i++)
							{
								p=(u8 *)(ADDR_STRINGBUFF+(i+2)*84+4+2*2);
								p8[1+4*i]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//��ʼʱ�䣺��
								p8[2+4*i]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//��ʼʱ�䣺ʱ
								p8[3+4*i]=(p[9*2]-0x30)*16+(p[10*2]-0x30)*1;//����ʱ�䣺��
								p8[4+4*i]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//����ʱ�䣺ʱ
							}
							MW(ADDR_DATABUFF,ADDR_AFN04F33+((14+(4*24))*30)+13,1+4*24);
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
						if(Stack->MenuStack[MenuCount->Count].EditRow<(MenuCount->v3+2))
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=0x2020;
							p16[3]=0x2020;
							if(Stack->MenuStack[MenuCount->Count].EditRow==1)
							{
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+7*2);
								if(p8[0]-0x30>7)
								{
									p8[0]=0x37;
								}
							}
							else
							{
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+2*2);
								for(i=0;i<2;i++)
								{
									if(((p8[(0+i*6)*2]-0x30)*10+(p8[(1+i*6)*2]-0x30)*1>23) || (p8[(0+i*6)*2]=='-') || (p8[(1+i*6)*2]=='-'))
									{
										p8[(0+i*6)*2]=0x30;
										p8[(1+i*6)*2]=0x30;
									}
									if(((p8[(3+i*6)*2]-0x30)*10+(p8[(4+i*6)*2]-0x30)*1>59) || (p8[(3+i*6)*2]=='-') || (p8[(4+i*6)*2]=='-'))
									{
										p8[(3+i*6)*2]=0x30;
										p8[(4+i*6)*2]=0x30;
									}
								}
							}
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
		default:
			break;
	}	  
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6_4
// Descriptions:        ����ʱ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_4(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Row=1;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_4);//��ʾ�˵�
			//��ʾ�ڴ�������
			//�˿�31����ʱ����
			x=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);
			x=hex_bcd(x);
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
			DisplayData(x,2,0,p16);

			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=9;
			MenuCount->v2=10;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(2,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=10;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=Stack->MenuStack[MenuCount->Count].EditRow)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_add1(p8,2);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_sub1(p8,2);
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//(1~60) �ж��ڴ���������д����ʼ��
								for(i=0;i<=1;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=2)
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 2:
								DisplayClrRow_4(2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(2,9,0,"OK");
								//�������õ�����
								MR(ADDR_DATABUFF,ADDR_AFN04F33+((14+(4*24))*30)+9,1);//���˿�31����ʱ����
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+1*84+4+9*2);
								p8[0]=p8_bcd(p,2);
								MW(ADDR_DATABUFF,ADDR_AFN04F33+((14+(4*24))*30)+9,1);//д�˿�31����ʱ����
								break;
							default:
								break;
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,2);
								if(x>60 || x<1)
								{
									bcd_p8(p8,1,2);
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
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6_3
// Descriptions:        ���³���F51
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_3(void)
{
	u8 *p8;
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"�ز����³���");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					p8=(u8 *)ADDR_DATABUFF;
					p8[0]=0x55;
					MW(ADDR_DATABUFF,ADDR_AFN05F51+30,1);
					DisplayString(4,3,0,"�ز������³���");
					DisplayString(5,4,0,"�����ؼ��˳�");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6_2
// Descriptions:        �ָ�����F50
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_2(void)
{
	u8 *p8;
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"�ز��ָ�����");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					p8=(u8 *)ADDR_DATABUFF;
					p8[0]=0;
					MW(ADDR_DATABUFF,ADDR_AFN05F50+30,1);
					DisplayClr();
					DisplayString(4,3,0,"�ز��ѻָ�����");
					DisplayString(5,4,0,"�����ؼ��˳�");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6_1
// Descriptions:        ��ͣ����F49
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_6_1(void)
{
	u8 *p8;
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"�ز���ͣ����");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					p8=(u8 *)ADDR_DATABUFF;
					p8[0]=0x55;
					MW(ADDR_DATABUFF,ADDR_AFN05F49+30,1);
					DisplayClr();
					DisplayString(4,3,0,"�ز�����ͣ����");
					DisplayString(5,4,0,"�����ؼ��˳�");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_6
// Descriptions:        �ز��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_6(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu2_3_6_1);//�����¼��˵�
						break;
					case 2:
						MenuCreate((u32)Menu2_3_6_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu2_3_6_3);//�����¼��˵�
						break;
					case 4:
						MenuCreate((u32)Menu2_3_6_4);//�����¼��˵�
						break;
					case 5:
						MenuCreate((u32)Menu2_3_6_5);//�����¼��˵�
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
#if ((USER/100)==2)//ɽ����
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2
// Descriptions:        232�ڹ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_7_2);//��ʾ�˵�
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=5;
			MenuCount->v2=14;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//��ʾ�ڴ�������
			//232����//0=�ն˼���,!=0�ն�ά����
			x=MRR(ADDR_UART_PORT_SET,1);
			if(x!=0)
			{
				x=1;
			}
			DisplayString(1,5,0,(u8 *)Function_232List[x]);//0=�ն˼���,!=0�ն�ά����
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=2)
					{
						DisplayString(2,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=14;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=Stack->MenuStack[MenuCount->Count].EditRow)
								{
									p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+16*2);
									if((p16[2]&0x7fff)==32 && (p16[3]&0x7fff)==33)
									{
										for(i=2;i<=3;i++)
										{
											p16[i]=(p16[i]&0x8000)+0x2020;
										}
									}
								}
							}
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								if(p8[0]==0xBA)
								{//�ն�ά����
									i=0;//�ն˼���
								}
								else
								{//�ն˼���
									i=1;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)Function_232List[i]);//0=�ն˼��ӿ�,!=0�ն�ά����
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								if(p8[0]==0xBA)
								{//�ն�ά����
									i=0;//�ն˼���
								}
								else
								{//�ն˼���
									i=1;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)Function_232List[i]);//0=�ն˼��ӿ�,!=0�ն�ά����
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 2:
								DisplayClrRow_4(Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
								//�������õ�����
								p=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_UART_PORT_SET,1);//��232�ڹ���
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+9*2);
								if(p8[0]==0xBA)
								{//�ն�ά����
									p[0]=1;
								}
								else
								{//�ն˼���
									p[0]=0;
									MWR(180,ADDR_TerminalFMTimer+38,2);
								}
								MW(ADDR_DATABUFF,ADDR_UART_PORT_SET,1);//д232�ڹ���
								break;
							default:
								break;
						}
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
		default:
			break;
	}
}
#else
	#if (Project/100)==2//������
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2_3
// Descriptions:        �ѱ����ò�����F10
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2_3(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"���ò�����F10");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					MWR(0x55,ADDR_AFN05F149,1);
					MWR(2,ADDR_AFN05F149+1,1);
					MC(0,ADDR_AFN0CF13,2);//�������ĵ����Ϣ
					MWR(0x55555555,ADDR_AFN05F149+2,4);
					DisplayClr();
					DisplayString(4,5,0,"�ѱ��ѿ���");
					DisplayString(5,4,0,"�����ؼ��˳�");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2_2
// Descriptions:        �ѱ����ò�����F10
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2_2(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"���ò�����F10");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					MWR(0x55,ADDR_AFN05F149,1);
					MWR(1,ADDR_AFN05F149+1,1);
					MC(0,ADDR_AFN0CF13,2);//�������ĵ����Ϣ
					MWR(0x55555555,ADDR_AFN05F149+2,4);
					DisplayClr();
					DisplayString(4,5,0,"�ѱ��ѿ���");
					DisplayString(5,4,0,"�����ؼ��˳�");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2_1
// Descriptions:        �ѱ�ر�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2_1(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"�ѱ�ر�");
			DisplayString(5,6,0,"NO / YES");
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			p16[0]|=0x8000;
			p16[1]|=0x8000;
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			break;
		case 1://����1:ѡ��
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+6*2);
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//��
				if(p16[5]>>15)
				{
					p16[5]&=0x7fff;
					p16[6]&=0x7fff;
					p16[7]&=0x7fff;
					p16[0]|=0x8000;
					p16[1]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//��
				if(p16[0]>>15)
				{
					p16[0]&=0x7fff;
					p16[1]&=0x7fff;
					p16[5]|=0x8000;
					p16[6]|=0x8000;
					p16[7]|=0x8000;
				}
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				if(p16[0]>>15)
				{//NO
					MenuDel();//�����ϼ��˵�
				}
				if(p16[5]>>15)
				{//YES
					MWR(0,ADDR_AFN05F149,6);
					DisplayClr();
					DisplayString(4,5,0,"�ѱ��ѹر�");
					DisplayString(5,4,0,"�����ؼ��˳�");
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3C;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_2
// Descriptions:        �Զ��ѱ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_7_2);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu2_3_7_2_1);//�����¼��˵�
						break;
					case 2:
						MenuCreate((u32)Menu2_3_7_2_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu2_3_7_2_3);//�����¼��˵�
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
#endif
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7
// Descriptions:        �ֳ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_7);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						if(Comm_Ram->Factory!=0x55)
						{//�ǹ���״̬
							MenuCreate((u32)Menu_9_6);//�����¼��˵� 485�ڹ�������
						}
						else//����״̬
						{
							MenuCreate((u32)Menu_9_6_1);//�����¼��˵� ����״̬�½�ֹ485�ڹ�������
						}
						break;
					case 2:
#if ((USER/100)==2)//ɽ����
							MenuCreate((u32)Menu2_3_7_2);//�����¼��˵� 232�ڹ�������
#else
	#if (Project/100)==2//������
							MenuCreate((u32)Menu2_3_7_2);//�����¼��˵� 232�ڹ�������
	#endif
#endif
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

#if ((USER/100)==5)//�Ϻ�������
u32 Check_AFN04F10_MeterNo(u32 MeterNo)//����ն˵��ܱ�/��������װ�����ò���ָ����ͨ�ŵ�ַ��6λ;����ֵΪ��ͬ��6λͨ�ŵ�ַ�Ĳ���������
{
	u32 i;
	u32 Meter;
	u32 Num;
	u8 *p8;

	Num=0;
	p8=(u8*)(ADDR_AFN04F10+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[4]+(p8[5]<<8)+(p8[6]<<16);
		if(i==MeterNo)
		{
			if((p8[0]+(p8[1]<<8))!=0)
			{//������Ų�Ϊ0
				Num++;
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return Num;
}
u32 Check_AFN04F10_MeterNo1(u32 Pn,u32 MeterNo)//�Ӳ�����Pn�ſ�ʼ����ն˵��ܱ�/��������װ�����ò���ָ����ͨ�ŵ�ַ��6λ;����ֵ!=0Ϊ��ͬ��6λͨ�ŵ�ַ�Ĳ������,0=û��
{
	u32 i;
	u32 Meter;
	u8 *p8;

	p8=(u8*)(ADDR_AFN04F10+2+LEN_AFN04F10_Pn*(Pn-1));
	for(Meter=Pn;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[4]+(p8[5]<<8)+(p8[6]<<16);
		if(i==MeterNo)
		{
			if((p8[0]+(p8[1]<<8))!=0)
			{//������Ų�Ϊ0
				return (p8[0]+(p8[1]<<8));
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------
// Function name:       Menu2_SH_9_9_2
// Descriptions:        �Ϻ��������˵�9_9_2 �ֶ�����2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_SH_9_9_2(void)
{
/*
	�������߱��ֶ������ܣ���ͨ����Ż������Ž����ֶ�����
	ͨ�����������ź���λ��ѯ���г������������ж�Ӧ�Ĳ�������Ϣ��
	�ƶ�����ѡ������������ֶ�����
	ͨ���������������Ž����ֶ�����
	������ֳ���ʽ�����������ض�Ӧ�����㵱ǰ�����й��ܵ���ʾֵ��һ������F129����
*/
	u32 i;
	u32 j;
	u64 x;
	u64 y;
	u64 MeterNo;
//	u32 cs;
	u8 *p8;
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
																			"�ֶ�����\n\r"
																			"��ַ��6λ\n\r"
																			"   ��ѯ��������Ϣ\n\r"
																			);//��ʾ�˵�
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+9*2);
			DisplayData(0x000001,6,0,p16);
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
						DisplayString(2,3,0,"��ѯ��������Ϣ");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=14;
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_add1(p8,6);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_sub1(p8,6);
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
							Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
						}
						if(Stack->MenuStack[MenuCount->Count].EditRow==2)
						{
							Stack->MenuStack[MenuCount->Count].Task=2;
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								for(i=0;i<6;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=6)
								{
									bcd_p8(p8,1,6);
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
			p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+9*2);
			MeterNo=0;
			y=1;
			for(i=0;i<6;i++)
			{
				MeterNo+=(p8[(6-1-i)*2]-0x30)*y;
				y*=16;
			}

			MenuCount->v3=Check_AFN04F10_MeterNo(MeterNo);//����ն˵��ܱ�/��������װ�����ò���ָ����ͨ�ŵ�ַ��6λ;����ֵΪ��ͬ��6λͨ�ŵ�ַ�Ĳ���������
			DisplayClr();//��ȫ��
			if(MenuCount->v3!=0)
			{
				Stack->MenuStack[MenuCount->Count].Task=3;
				Stack->MenuStack[MenuCount->Count].MoveRow=0;//�˵���������(����ʱ��Ϊ0,����ʱ����)
				Stack->MenuStack[MenuCount->Count].EditRow=1;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
				Stack->MenuStack[MenuCount->Count].EditCol=0;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"�ֶ�����\n\r"
																				"      ��    ��\n\r"
																				"      ȷ�Ϸ���\n\r"
																				);//��ʾ�˵��ַ���
				p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+8*2);
				DisplayData(0x0001,4,0,p16);
				//�������
				DisplayString(3,0,0,(u8 *)"�������");
				x=Check_AFN04F10_MeterNo1(1,MeterNo);//�Ӳ�����Pn�ſ�ʼ����ն˵��ܱ�/��������װ�����ò���ָ����ͨ�ŵ�ַ��6λ;����ֵ!=0Ϊ��ͬ��6λͨ�ŵ�ַ�Ĳ������,0=û��
				p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+8*2);
				bcd_p8(p8,x,4);
				//ͨ�ŵ�ַ
				DisplayString(4,0,0,(u8 *)"ͨ�ŵ�ַ");
				p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(x-1)+6,6);
				DisplayData(x,12,0,p16);

				MenuCount->v1=8;
				MenuCount->v2=11;
				NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			}
			else
			{
				DisplayMenuString(4,0,2,(u8 *)
												"      ���޴˱�\n\r"
												"   �밴������˳�\n\r"
												);//��ʾ�˵��ַ���
			}
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{//������������ϼ��˵�
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
			break;
		case 3:
			p8=(u8 *)(ADDR_STRINGBUFF+4*84+4+14*2);
			MeterNo=0;
			y=1;
			for(i=0;i<6;i++)
			{
				MeterNo+=(p8[(6-1-i)*2]-0x30)*y;
				y*=16;
			}

			for(j=0;j<2;j++)
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
						MenuCount->v2=11;
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
							x=p8_bcd(p8,4);
							if(x<MenuCount->v3)
							{
								p8_add1(p8,4);
							}
							else
							{
								bcd_p8(p8,1,4);
							}
							break;
						default:
							break;
					}
					//�������
					p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
					x=p8_bcd(p8,4);
					p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+8*2);
					if(x!=1)
					{
						y=p8_bcd(p8,4);
						y+=1;
					}
					else
					{
						y=1;
					}
					x=Check_AFN04F10_MeterNo1(y,MeterNo);//�Ӳ�����Pn�ſ�ʼ����ն˵��ܱ�/��������װ�����ò���ָ����ͨ�ŵ�ַ��6λ;����ֵ!=0Ϊ��ͬ��6λͨ�ŵ�ַ�Ĳ������,0=û��
					bcd_p8(p8,x,4);
					//ͨ�ŵ�ַ
					p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+8*2);
					x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(x-1)+6,6);
					DisplayData(x,12,0,p16);
				}
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					//�ֶ����� AFN0CF129����ʾֵ
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
						if(UARTCtrl->Lock==0x55)
						{
							return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						}
						if(UARTCtrl->Task==0)
						{
/*
�ն˽���AFN0CF129��֡��ʽ
68 
32 00 //L
32 00 //L
68 
7B //C
34 12 01 00 //�ն˵�ַ������������ 
02 
0C //AFN
61 //SEQ
01 01 01 10 //���ݵ�Ԫ��ʶ
44 //1�ֽ� У���CS
16 
*/
							p8=(u8 *)ADDR_DATABUFF;
							p8[0]=0x68;
							p8[1]=0x32;//L
							p8[2]=0x00;
							p8[3]=0x32;//L
							p8[4]=0x00;
							p8[5]=0x68;
							p8[6]=0x7B;//C
							MR(((u32)ADDR_DATABUFF)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
							p8[11]=0x02;
							p8[12]=0x0C;//AFN									 
							p8[13]=0x61;//SEQ
							//���ݱ�ʶ
							p8[14]=0;
							p8[15]=0;									 
							i=p8_bcd((u8 *)ADDR_STRINGBUFF+3*84+4+8*2,4);
							MenuCount->v3=i;//�������
							if(i!=0x0)
							{
								i-=1;
								p8[14]|=(1<<(i%8));
								p8[15]=((i/8)+1);
							}
							p8[16]=0x01;
							p8[17]=0x10;
//							//У���CS����
//							cs=0;
//							for(i=0;i<12;i++)
//							{
//								cs+=p8[6+i];
//							}
//							cs&=0xff;
//							p8[18]=cs;
//							p8[19]=0x16;

							p8[1]=0x0C;//ȥ��־λD1D0,ͬʱ�������ֻ��0-16383
//							MW(ADDR_DATABUFF,(u32)p8rxbuff,20);									 
							MW(ADDR_DATABUFF,(u32)p8rxbuff,18);									 

							UARTCtrl->Task=1;
							Stack->MenuStack[MenuCount->Count].Task=4;
						}
					}
					Comm_Ram->ButtonVal&=~0x1B;//��ʹ�ú���0
				}
				else
				{
					Comm_Ram->ButtonVal&=~0x0F;//��ʹ�ú���0
					break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		case 4:
//�ӷ��ͻ�������ȡ����
			if(UARTCtrl->Task==0)
			{
				DisplayClr();//��ȫ��
				DisplayMenuString(2,0,2,(u8 *)
												"�����й�����(kWh):\n\r"
												"  �� = ----\n\r"
												"  �� = ----\n\r"
												"  ƽ = ----\n\r"
												"  �� = ----\n\r"
												"  �� = ----\n\r"
												);//��ʾ�˵��ַ���
/*
68 
B6 00 
B6 00 
68 
A8 
01 22 01 00 
02 
0C 
66 
01 01 01 10 //14���ݱ�ʶ
48 03 10 01 14 //18����ʱ��
04 //������
00 00 00 00 00 //24��
00 00 00 00 00 //29����1 
00 00 00 00 00 //34����2 
00 00 00 00 00 //39����3 
00 00 00 00 00 //44����4 
29 2C //EC
1C 
16
*/
				//�����й�����ʾֵ
				//�Ƚ�AFN��SEQ�����ݱ�ʶ
				for(i=0;i<6;i++)
				{
					if(p8rxbuff[12+i]!=p8txbuff[12+i])
					{
						break;
					}
				}
				if(i==6)
				{//AFN��SEQ�����ݱ�ʶ���պͷ��͵�һ��
					p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v3-1)*LEN_AFN04F10_Pn)+5);
					if((p8[0]==21) && (p8[13]!=4))
					{//�Ϻ���Լ������2
			/*
				����Ϻ��������������з�����Ϊ2ʱ������������1��Ӧ�Ϻ������ƽ������������2��Ӧ�Ϻ�����ʹȣ�
				�����������з�����Ϊ4ʱ������������1��4�ֱ��Ӧ��Ч���ݡ��Ϻ������ƽ����Ч���ݡ��Ϻ�����ʹȡ�
				�˷���˳���һ��Ͷ������ݾ���Ҫ��
			*/			
						//��
						y=MRR((u32)p8txbuff+24,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
						//��

						//ƽ
						y=MRR((u32)p8txbuff+24+5,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3+2)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
						//��

						//��
						y=MRR((u32)p8txbuff+24+10,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3+4)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
					}
					else
					{
						x=p8[13];
						if(x>4)
						{//��������4
							x=4;
						}
						for(j=0;j<(x+1);j++)
						{
							y=MRR((u32)p8txbuff+24+5*j,5);	                   
							if(y!=0xEEEEEEEEEE)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(3+j)*84+6*2);
								DisplayData_OffZero(y,10,4,p16);
							}
						}
					}
				}
			}
			else
			{
				DisplayClr();//��ȫ��
				DisplayMenuString(4,0,2,(u8 *)
												"      ���ڳ���\n\r"
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
//------------------------------------------------------------------------------------------------
// Function name:       Menu2_SH_9_9_1
// Descriptions:        �Ϻ��������˵�9_9_1 �ֶ�����1
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_SH_9_9_1(void)
{
/*
	�������߱��ֶ������ܣ���ͨ����Ż������Ž����ֶ�����
	ͨ�����������ź���λ��ѯ���г������������ж�Ӧ�Ĳ�������Ϣ��
	�ƶ�����ѡ������������ֶ�����
	ͨ���������������Ž����ֶ�����
	������ֳ���ʽ�����������ض�Ӧ�����㵱ǰ�����й��ܵ���ʾֵ��һ������F129����
*/
	u32 i;
	u32 j;
	u32 x;
	u64 y;
//	u32 cs;
	u8 *p8;
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
																			"�ֶ�����\n\r"
																			"�������\n\r"
																			"      ȷ�Ϸ���\n\r"
																			);//��ʾ�˵�
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+8*2);
			DisplayData(0x0001,4,0,p16);
			MenuCount->v1=8;
			MenuCount->v2=11;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							MenuCount->v2=11;
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
								y=p8_bcd(p8,4);
								if(y<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,1,4);
								}
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
								y=p8_bcd(p8,4);
								if(y>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						//�ֶ����� AFN0CF129����ʾֵ
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
							if(UARTCtrl->Lock==0x55)
							{
								return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
							}
							if(UARTCtrl->Task==0)
							{
/*
�ն˽���AFN0CF129��֡��ʽ
68 
32 00 //L
32 00 //L
68 
7B //C
34 12 01 00 //�ն˵�ַ������������ 
02 
0C //AFN
61 //SEQ
01 01 01 10 //���ݵ�Ԫ��ʶ
44 //1�ֽ� У���CS
16 
*/
								p8=(u8 *)ADDR_DATABUFF;
								p8[0]=0x68;
								p8[1]=0x32;//L
								p8[2]=0x00;
								p8[3]=0x32;//L
								p8[4]=0x00;
								p8[5]=0x68;
								p8[6]=0x7B;//C
								MR(((u32)ADDR_DATABUFF)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
								p8[11]=0x02;
								p8[12]=0x0C;//AFN									 
								p8[13]=0x61;//SEQ
								//���ݱ�ʶ
								p8[14]=0;
								p8[15]=0;									 
								i=p8_bcd((u8 *)ADDR_STRINGBUFF+1*84+4+8*2,4);
								MenuCount->v3=i;//�������
								if(i!=0x0)
								{
									i-=1;
									p8[14]|=(1<<(i%8));
									p8[15]=((i/8)+1);
								}
								p8[16]=0x01;
								p8[17]=0x10;
//								//У���CS����
//								cs=0;
//								for(i=0;i<12;i++)
//								{
//									cs+=p8[6+i];
//								}
//								cs&=0xff;
//								p8[18]=cs;
//								p8[19]=0x16;

								p8[1]=0x0C;//ȥ��־λD1D0,ͬʱ�������ֻ��0-16383
//								MW(ADDR_DATABUFF,(u32)p8rxbuff,20);									 
								MW(ADDR_DATABUFF,(u32)p8rxbuff,18);									 

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
								for(i=0;i<4;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=4)
								{
									bcd_p8(p8,1,4);
								}
								y=p8_bcd(p8,4);
								if(y>MaxRS485AddCarrierPn || y<1)
								{
									bcd_p8(p8,1,4);
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
				DisplayMenuString(2,0,2,(u8 *)
												"�����й�����(kWh):\n\r"
												"  �� = ----\n\r"
												"  �� = ----\n\r"
												"  ƽ = ----\n\r"
												"  �� = ----\n\r"
												"  �� = ----\n\r"
												);//��ʾ�˵��ַ���
/*
68 
B6 00 
B6 00 
68 
A8 
01 22 01 00 
02 
0C 
66 
01 01 01 10 //14���ݱ�ʶ
48 03 10 01 14 //18����ʱ��
04 //������
00 00 00 00 00 //24��
00 00 00 00 00 //29����1 
00 00 00 00 00 //34����2 
00 00 00 00 00 //39����3 
00 00 00 00 00 //44����4 
29 2C //EC
1C 
16
*/
				//�����й�����ʾֵ
				//�Ƚ�AFN��SEQ�����ݱ�ʶ
				for(i=0;i<6;i++)
				{
					if(p8rxbuff[12+i]!=p8txbuff[12+i])
					{
						break;
					}
				}
				if(i==6)
				{//AFN��SEQ�����ݱ�ʶ���պͷ��͵�һ��
					p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v3-1)*LEN_AFN04F10_Pn)+5);
					if((p8[0]==21) && (p8[13]!=4))
					{//�Ϻ���Լ������2
			/*
				����Ϻ��������������з�����Ϊ2ʱ������������1��Ӧ�Ϻ������ƽ������������2��Ӧ�Ϻ�����ʹȣ�
				�����������з�����Ϊ4ʱ������������1��4�ֱ��Ӧ��Ч���ݡ��Ϻ������ƽ����Ч���ݡ��Ϻ�����ʹȡ�
				�˷���˳���һ��Ͷ������ݾ���Ҫ��
			*/			
						//��
						y=MRR((u32)p8txbuff+24,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
						//��

						//ƽ
						y=MRR((u32)p8txbuff+24+5,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3+2)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
						//��

						//��
						y=MRR((u32)p8txbuff+24+10,5);	                   
						if(y!=0xEEEEEEEEEE)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(3+4)*84+6*2);
							DisplayData_OffZero(y,10,4,p16);
						}
					}
					else
					{
						x=p8[13];
						if(x>4)
						{//��������4
							x=4;
						}
						for(j=0;j<(x+1);j++)
						{
							y=MRR((u32)p8txbuff+24+5*j,5);	                   
							if(y!=0xEEEEEEEEEE)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(3+j)*84+6*2);
								DisplayData_OffZero(y,10,4,p16);
							}
						}
					}
				}
			}
			else
			{
				DisplayClr();//��ȫ��
				DisplayMenuString(4,0,2,(u8 *)
												"      ���ڳ���\n\r"
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
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_SH_9_9
// Descriptions:        �Ϻ��������˵�9_9 �ֶ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_SH_9_9(void)//
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�ֶ�����\n\r"
																		"  1.�����㷽ʽ\n\r"
																		"  2.���ַ��ʽ\n\r"
																		);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu2_SH_9_9_1);
						break;
					case 2:
						MenuCreate((u32)Menu2_SH_9_9_2);
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
#if ((USER/100)==8)//ͨ�ü�����
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_9_9
// Descriptions:        ��������Ҫ����ʾ����ͨ�ŵ�ַ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_9_9(void)
{
	u16 *p16;
	u32 Row=1;
	u64 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																"����ͨ�ŵ�ַ\n\r"
																);//��ʾ�˵�
	//����ͨ�ŵ�ַ
	x=MRR(ADDR_04000401,6);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+0*2);
	DisplayData(x,12,0,p16);
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}
#endif
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3
// Descriptions:        �������ն˹�����ά��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3(void)//
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_9_1);//�����¼��˵�
						break;
					case 2:
						MenuCreate((u32)Menu_9_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu_9_3);//�����¼��˵�
						break;
					case 4:
						MenuCreate((u32)Menu_9_4);//�����¼��˵�
						break;
					case 5:
						MenuCreate((u32)Menu2_3_5);//�����¼��˵�
						break;
					case 6:
						MenuCreate((u32)Menu2_3_6);//�����¼��˵�
						break;
					case 7:
						MenuCreate((u32)Menu2_3_7);//�ֳ�����
						break;
					case 8:
						MenuCreate((u32)Menu_8_2_5);//�豸�������
						break;
#if ((USER/100)==5)//�Ϻ�������
					case 9://�ֶ�����
						MenuCreate((u32)Menu2_SH_9_9);//�����¼��˵�	�Ϻ������� �ֶ�����
						break;
#endif
#if ((USER/100)==8)//ͨ�ü�����
					case 9://��ʾ����ͨ�ŵ�ַ
						MenuCreate((u32)Menu2_9_9);//�����¼��˵�	��������Ҫ����ʾ����ͨ�ŵ�ַ
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
}
#endif

