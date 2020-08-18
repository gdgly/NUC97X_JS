
//�˵�3 �ն˹�����ά��
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
//#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"

#include "../MS/msFILE.h"
#include "../MS/MS.h"


extern void Init_ALLDATA(void);//�ն�ȫ�����ݳ�ʼ��(��0)
const u8 pMenu2_3_Project2[]=
{
	"�ն˹�����ά��\n\r"
	"  1.�������汾\n\r"
	"  2.�ֳ�����\n\r"
	"  3.����������\n\r"
	"  4.���ݳ�ʼ��\n\r"
	"  5.������ʼ��\n\r"
	"  6.����������\n\r"
	"  7.�ز��������\n\r"
//	"  8.�ֶ�����\n\r"
};
const u8 pMenu2_3_Project3[]=
{
	"�ն˹�����ά��\n\r"
	"  1.�ֳ�����\n\r"// -----2
	"  2.�ն�����\n\r"// -----3
	"  3.���ݳ�ʼ��\n\r"// -----4
	"  4.������ʼ��\n\r"// -----5
	"  5.�ն�����\n\r"// -----6
};

const u8 pMenu2_3_1[]=
{
	"�汾��Ϣ\n\r"
	"����汾:\n\r"
	"����:\n\r"
	"Ӳ���汾:\n\r"
	"����:\n\r"
};
const u8 pMenu2_3_2[]=
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


const u8 pMenu2_3_6_Project2[]=
{
	"����������\n\r"
	"�ɼ���������:\n\r"
	"�������ø���:\n\r"
	"��ͨ�ɼ���������:\n\r"
	"�¼��ɼ���������:\n\r"
	"�ϱ���������:\n\r"
};

const u8 pMenu2_3_6_Project3[]=
{
	"�ն�����\n\r"
	"�ɼ���������:\n\r"
	"�������ø���:\n\r"
	"��ͨ�ɼ���������:\n\r"
	"�¼��ɼ���������:\n\r"
	"�ϱ���������:\n\r"
};


const u8 pMenu2_3_7[]=
{
	"�ز��������\n\r"
	"  1.��ͣ����\n\r"
	"  2.�ָ�����\n\r"
	"  3.���³���\n\r"
//	"  4.����������\n\r"
//	"  5.����ʱ������\n\r"
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_1
// Descriptions:        �������汾
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_1(void)
{
	u16 *p16;
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_1);//��ʾ�˵�
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
// Function name:       Menu2_3_2
// Descriptions:        �ֳ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_2(void)
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
#elif((Project/100)==2 && (USER/100)==17)//���ռ�����
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_2);//��ʾ�˵�
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_3
// Descriptions:        �����ն�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_3(void)
{
//	u8 *p8;
	u16 *p16;
//	u32 i;

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
					WWDT_RESET();
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
// Function name:       Menu2_3_4
// Descriptions:        ��������ʼ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_4(void)
{
//	u8 *p8;
	u16 *p16;
	u32 *p32;

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
					//p8=(u8 *)ADDR_DATABUFF;
					//for(i=0;i<=3;i++)
					//{
					//	p8[i]=0x55;
					//}
					//p8[4]=2;
					
					//MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					DisplayString(4,4,0,"���ݳ�ʼ����");
					DisplayString(5,6,0,"��ȴ�...");
					//Init_ALLDATA();
					p32=(u32*)ADDR_Terminal_ENTRY;
					(*(void(*)())p32[116])();//�ն�ȫ�����ݳ�ʼ��(��0)
					
					DisplayString(4,4,0,"���ݳ�ʼ��OK");
					DisplayString(5,4,0,"�����ؼ��˳�");
				}
			}
			//if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			//{
				//DisplayString(4,4,0,"���ݳ�ʼ��OK");
				//DisplayString(5,4,0,"�����ؼ��˳�");
			//}
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
// Function name:       Menu2_3_5
// Descriptions:        ������ʼ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_5(void)
{
//	u8 *p8;
	u16 *p16;
	u32 *p32;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,4,0,"������ʼ��");
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
					//p8=(u8 *)ADDR_DATABUFF;
					//for(i=0;i<=3;i++)
					//{
					//	p8[i]=0x55;
					//}
					//p8[4]=3;
					//MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
					
					
					DisplayString(4,3,0,"�ָ�����������");
					DisplayString(5,6,0,"��ȴ�...");
					//DL698_SETINIT(3,0);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
					p32=(u32*)ADDR_Terminal_ENTRY;
					(*(void(*)())p32[115])((u32)3,(u8*)0);
					
					DisplayString(4,3,0,"�ָ���������OK");
					DisplayString(5,4,0,"�����ؼ��˳�");
				}
			}
			//if(MRR(ADDR_AFN01Fx,5)==0 && p16[6]==0x2000+'.')
			//{
				//DisplayString(4,3,0,"�ָ���������OK");
				//DisplayString(5,4,0,"�����ؼ��˳�");
			//}
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
// Descriptions:        ����������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_6(void)
{
	u8 *p;
	u32 num;
	u32 index;
	u32 i;
	u32 Ln;
#if((Project/100)==2 && (USER/100)==17)//���ռ�����
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
#if ((Project/100)==2)//������
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_Project2);//��ʾ�˵�
#endif
#if ((Project/100)==3)//ר���ն�
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_Project3);//��ʾ�˵�
#endif
#if ((Project/100)==4)//3in1
	if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_Project2);//��ʾ�˵�
	}
	else
	{
		DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_6_Project3);//��ʾ�˵�
	}
#endif
	//�ɼ���������
	num=0;
	for(index=1;index<NUM_RMmax;index++){
		i=MRR(ADDR_6000_SDRAM+index*LENper_6000,2);
		if(i==0x0402)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//����
			break;
		}
	}
	p=(u8*)(ADDR_STRINGBUFF+(1+Ln)*84+4+13*2);
	bcd_p8(p,num,4);


	//�������ø���
	num=0;
	for(index=1;index<NUMmax_6012_2;index++){
		i=MRR(ADDR_6012_2_SDRAM+index*LENper_6012_2,2);
		if(i==0x0C02)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//����
			break;
		}
		
	}
	p=(u8*)(ADDR_STRINGBUFF+(2+Ln)*84+4+13*2);
	bcd_p8(p,num,3);
	
	//��ͨ�ɼ���������
	num=0;
	for(index=0;index<NUMmax_6014;index++){
		i=MRR(ADDR_6014_SDRAM+index*LENper_6014,2);
		if(i==0x0602)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//����
			break;
		}
	}
	p=(u8*)(ADDR_STRINGBUFF+(3+Ln)*84+4+17*2);
	bcd_p8(p,num,3);	

	//�¼��ɼ���������
	num=0;
	for(index=0;index<NUMmax_6016;index++){
		i=MRR(ADDR_6016_SDRAM+index*LENper_6016,2);
		if(i==0x0502)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//����
			break;
		}
	}
	p=(u8*)(ADDR_STRINGBUFF+(4+Ln)*84+4+17*2);
	bcd_p8(p,num,3);	

	//�ϱ���������			
	num=0;
	for(index=0;index<NUMmax_601C;index++){
		i=MRR(ADDR_601C_SDRAM+index*LENper_601C,2);
		if(i==0x0502)
			num++;
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//����
			break;
		}
		
	}
	p=(u8*)(ADDR_STRINGBUFF+(5+Ln)*84+4+13*2);
	bcd_p8(p,num,3);	

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_7_3
// Descriptions:        ���³���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_3(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"���³���");
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
				//if(p16[0]>>15)
				//{//NO
					MenuDel();//�����ϼ��˵�
				//}
				//if(p16[5]>>15)
				//{//YES
				//	WWDT_RESET();
				//}
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
// Descriptions:        �ָ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_2(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"�ָ�����");
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
				//if(p16[0]>>15)
				//{//NO
					MenuDel();//�����ϼ��˵�
				//}
				//if(p16[5]>>15)
				//{//YES
				//	WWDT_RESET();
				//}
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
// Function name:       Menu2_3_7_1
// Descriptions:        ��ͣ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_7_1(void)
{
	u16 *p16;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(4,6,0,"��ͣ����");
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
				//if(p16[0]>>15)
				//{//NO
					MenuDel();//�����ϼ��˵�
				//}
				//if(p16[5]>>15)
				//{//YES
				//	WWDT_RESET();
				//}
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
// Function name:       Menu2_3_7
// Descriptions:        �ز��������
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
					case 1://.��ͣ����
						MenuCreate((u32)Menu2_3_7_1);//�����¼��˵�
						break;
					case 2://.�ָ�����
						MenuCreate((u32)Menu2_3_7_2);//�����¼��˵�
						break;
					case 3://.���³���
						MenuCreate((u32)Menu2_3_7_3);//�����¼��˵�
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
		#if ((Project/100)==2)//������
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_Project2);//��ʾ�˵�
		#endif
		#if ((Project/100)==3)//ר��
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_Project3);//��ʾ�˵�
		#endif
		#if ((Project/100)==4)//����һ
			if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_Project2);//��ʾ�˵�
			}
			else
			{
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_Project3);//��ʾ�˵�
			}
		#endif
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				#if ((Project/100)==2)//������
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. �������汾
						MenuCreate((u32)Menu2_3_1);//�����¼��˵�
						break;
					case 2://2. �ֳ�����
						MenuCreate((u32)Menu2_3_2);//�����¼��˵�
						break;
					case 3://3. �����ն�
						MenuCreate((u32)Menu2_3_3);//�����¼��˵�
						break;
					case 4://4. ��������ʼ��
						MenuCreate((u32)Menu2_3_4);//�����¼��˵�
						break;
					case 5://5������ʼ��
						MenuCreate((u32)Menu2_3_5);//�����¼��˵�
						break;
					case 6://6����������
						MenuCreate((u32)Menu2_3_6);//�����¼��˵�
						break;
					case 7://7�ز��������
						MenuCreate((u32)Menu2_3_7);//�����¼��˵�
						break;
					default:
						break;
				}    
			#endif
			#if ((Project/100)==3)//ר��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://2. �ֳ�����
						MenuCreate((u32)Menu2_3_2);//�����¼��˵�
						break;
					case 2://3. �����ն�
						MenuCreate((u32)Menu2_3_3);//�����¼��˵�
						break;
					case 3://4. ��������ʼ��
						MenuCreate((u32)Menu2_3_4);//�����¼��˵�
						break;
					case 4://5������ʼ��
						MenuCreate((u32)Menu2_3_5);//�����¼��˵�
						break;
					case 5://6����������
						MenuCreate((u32)Menu2_3_6);//�����¼��˵�
						break;
					default:
						break;
				}    
			#endif
			#if ((Project/100)==4)
				if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
				{
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1://1. �������汾
							MenuCreate((u32)Menu2_3_1);//�����¼��˵�
							break;
						case 2://2. �ֳ�����
							MenuCreate((u32)Menu2_3_2);//�����¼��˵�
							break;
						case 3://3. �����ն�
							MenuCreate((u32)Menu2_3_3);//�����¼��˵�
							break;
						case 4://4. ��������ʼ��
							MenuCreate((u32)Menu2_3_4);//�����¼��˵�
							break;
						case 5://5������ʼ��
							MenuCreate((u32)Menu2_3_5);//�����¼��˵�
							break;
						case 6://6����������
							MenuCreate((u32)Menu2_3_6);//�����¼��˵�
							break;
						case 7://7�ز��������
							MenuCreate((u32)Menu2_3_7);//�����¼��˵�
							break;
						default:
							break;
					}    
				}
				else
				{
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1://2. �ֳ�����
							MenuCreate((u32)Menu2_3_2);//�����¼��˵�
							break;
						case 2://3. �����ն�
							MenuCreate((u32)Menu2_3_3);//�����¼��˵�
							break;
						case 3://4. ��������ʼ��
							MenuCreate((u32)Menu2_3_4);//�����¼��˵�
							break;
						case 4://5������ʼ��
							MenuCreate((u32)Menu2_3_5);//�����¼��˵�
							break;
						case 5://6����������
							MenuCreate((u32)Menu2_3_6);//�����¼��˵�
							break;
						default:
							break;
					}    
				}
			#endif
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
const u8 pMenu2_3_JS[]=//�������ն˹�����ά��
{
	"�ն˹�����ά��\n\r"
	"    1.�ն�����\n\r"
	"    2.�ն���Ϣ\n\r"
	"    3.�ն˹���\n\r"
};
const u8 pMenu2_3_1_JS[]=//����������
{
	"�ն�����\n\r"
	"     1.ң��״̬\n\r"
	"     2.�¼���Ϣ\n\r"
};
const u8 pMenu2_3_1_1_JS[]=//������ң��״̬
{
	"ң��״̬\n\r"
	"���:\n\r"
	"\n\r"
	"״̬:\n\r"
	"\n\r"
	"��λ:\n\r"
	"\n\r"
	"����:\n\r"
};
const u8 pMenu2_3_1_2_JS[]=//�������¼���Ϣ
{
	"�¼���Ϣ\n\r"
	"     1.�ն��¼�\n\r"
	"     2.����¼�\n\r"
};
const u8 pMenu2_3_1_2_1_JS[]=//�ն��¼���Ϣ
{
	"�¼���Ϣ\n\r"
	"����:\n\r"
	"\n\r"
	"����:\n\r"
	"\n\r"
	"����:\n\r"
};
const u8 pMenu2_3_1_2_2_JS[]=//����¼���Ϣ
{
	"����¼���Ϣ\n\r"
	"������:\n\r"
	"\n\r"
	"����:\n\r"
	"����:\n\r"
	"��ַ:\n\r"
	
};
const u8 pMenu2_3_2_JS[]=//�ն���Ϣ
{
	"����ͨѶģ����Ϣ\n\r"
	"HPLCģ��\n\r"
	"�汾\n\r"
	"���̴���:\n\r"
};

const u8 pMenu2_3_3_JS[]=//�ն˹���
{
	"�ն˹���\n\r"
	"  1.�ֳ�����\n\r"
	"  2.����������\n\r"
	"  3.���ݳ�ʼ��\n\r"
	"  4.������ʼ��\n\r"
	"  5.����������\n\r"
	"  6.�ز��������\n\r"
	"  7.USB����\n\r"
};
typedef struct
{
	u8 *pEventName;
	u32 EventOAD;
}Lcd_EventInfo_t;
const Lcd_EventInfo_t Lcd_EventInfo[]=
{
	{"�ն˶�ʱ    ",0x31140200},
	{"�ն˳�ʼ��  ",0x31000200},
	{"�汾���    ",0x31010200},
	{"״̬����λ  ",0x31040200},
	{"ͣ�ϵ�      ",0x31060200},
	{"��֤����    ",0x31090200},
	{"�豸����    ",0x310A0200},
	{"�ն˱��    ",0x31180200},
	{"������ݱ��",0x311C0200},
	{"���ʾ���½�",0x311B0200},
	{"����������  ",0x310C0200},
	{"���ܱ����  ",0x310D0200},
	{"���ܱ�ͣ��  ",0x310E0200},
	{"ʱ�䳬��    ",0x31050200},
	{"485����ʧ�� ",0x310F0200},
};

u32 DEPTH_EventFile(u32 OI)//�¼��ļ��洢���;���:OI;����:�洢���
{
	switch(OI)
	{
		case 0x3111://����δ֪���ܱ��¼�
		case 0x3112://��̨�����ܱ��¼�
#if ((2*NUM_RMmax)*LEN_EventFileHead)>LEN_128KDATABUFF
	#error
#endif
			return NUM_RMmax;//��󳭱���
		default:
			return NUMmax_Event;//����¼���¼��
	}
}
u32 GET_EventFileName(u32 OAD,u32 FileCount)//�����¼���¼�ļ���:���:OAD,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
{
	u32 FileID;
	msFILE_TypeDef *file;
	u32 SearchCount;
	
	if(OAD!=0xffffffff)
	{
		OAD&=0xffffff00;
		OAD|=2;//�ļ�����
	}
	SearchCount=Comm_Ram->msFILESearchCount;
	if(SearchCount<recordFILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(SearchCount*sizeof(msFILE_TypeDef)));
	}
	else
	{
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((SearchCount-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	for(;SearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);SearchCount++)
	{
		if(SearchCount==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			//�ļ�����
			if((FileID&0xff)!=2)
			{
				file++;
				continue;
			}
			//�ļ�ID
			if(OAD!=0xffffffff)
			{
				if(FileID!=OAD)
				{
					file++;
					continue;
				}
			}
			//�ļ�����
			if(FileCount!=0xffffffff)//�ļ�����
			{
				if(FileCount!=file->FILECOUNT)
				{
					file++;
					continue;
				}
			}
			Comm_Ram->msFILESearchCount=SearchCount+1;//�ļ���������
			if(SearchCount>=recordFILENAMEmax_FLASH)
			{
				SearchCount-=recordFILENAMEmax_FLASH;
				SearchCount+=FILENAMEmax_FLASH;
			}
			return SearchCount;
		}
		file++;
	}
	Comm_Ram->msFILESearchCount=SearchCount;//�ļ���������
	return 0xffffffff;
}
u32 Get_RecordFileName(u32 FileID,u32 FileCount,u8* pTime);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�

u32 Lcd_Get_EventData(u16 *pAddr,u32 OAD,u8 *pOut,u32 Type)//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���,LENMAX_NUM�������ֽڵ���󳤶�,returnȡ�����ݵĳ���
{
	//u8 *p8;
	u32 i;
	//u32 n;
	u32 NUM;
	u32 NUMmax_DATA;
	//u32 offset;
	ms_Type *ms;
	u32 filename;
	u32 result=0;
	u32 Len;
	msFILE_TypeDef *file;
	EventFileHead_TypeDef* EventFileHead;
	//RecordFileHead_TypeDef* RecordFileHead;//�����¼�ļ�ͷ�ļ�
	switch (Type)
	{
		case 0://�ն��¼�		
			NUMmax_DATA=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
			ms=Get_ms();
			Comm_Ram->msFILESearchCount=0;//�ļ���������
			filename=GET_EventFileName(OAD&0xffffff00,0);//�����¼���¼�ļ���:���:OI,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
			if(filename==0xffffffff)
			{//û�ҵ�
				return result;
			}
			if(filename<FILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			i=(OAD&0xff);
			NUM=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
			if(i>NUM)
			{//
				return result;
			}
			NUM=file->NUM_DATA;//�¼�����
			if(i>NUM)
			{//���ļ�����1�ļ���
				i-=NUM;
				Comm_Ram->msFILESearchCount=0;//�ļ���������
				filename=GET_EventFileName(OAD&0xffffff00,1);//�����¼���¼�ļ���:���:OI,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
				if(filename==0xffffffff)
				{//û�ҵ�
					return result;
				}
				if(filename<FILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
				}
				else
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
				}
				NUM=file->NUM_DATA;//�¼�����
				if(i>NUM)
				{//�ļ����ڼ���1�ļ���
					return result;
				}
			}
			NUM-=i;
			Len=(NUM+1)*sizeof(EventFileHead_TypeDef);
			if(Len>LEN_128KDATABUFF)
			{
				return result;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,Len);
			EventFileHead=(EventFileHead_TypeDef*)(ADDR_128KDATABUFF);
			i=0;
			for(Len=0;Len<NUM;Len++)
			{
				i+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
				EventFileHead++;
			}
			Len=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);			
			ms->msfile_read(filename,((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+i,pOut,Len);
			return Len;
			//break;
		case 1://����¼�
		/*
			if(pAddr==0)
			{
				return result;
			}
			for(NUM=0;NUM<127;NUM++)
			{
				ms=(ms_Type *)Get_ms();
				Comm_Ram->msFILESearchCount=0;
				filename=Get_RecordFileName(0xffffffff,NUM,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
				if(filename==0xffffffff)
				{
					continue;
				}
				if(filename<FILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
				}
				else
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
				}
				if(file->BLOCK_ENTRY==0)
				{
					return result;
				}
				
				n=file->NUM_DATA;
				if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
				{
					n=0;
				}
				ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
				offset=0;
				RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
				for(i=0;i<n;i++)
				{
					if(RecordFileHead->Addr0==pAddr[0])
					{
						if(RecordFileHead->Addr1==pAddr[1])
						{
							if(RecordFileHead->Addr2==pAddr[2])
							{
								break;
							}
						}
					}
					offset+=RecordFileHead->LEN_DATA;
					RecordFileHead++;
				}
				if(i>=n)
				{//û�ҵ�
					return 0;
				}
				i=RecordFileHead->LEN_DATA;
				if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
				{
					return 0;
				}
				ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);
				p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+21;
				
			}
			result=0;
			*/
			break;
		default:
		break;			
	}
	return result;
}
void DisPlayEventInfo(u32 S_Row,u32 E_Row,u32 Addr_Row)//(��һ��Ϊ0)��ʾ�¼��ķ���ʱ��ͽ���ʱ�� ���б�ı��ַu32 S_Row��ʼʱ��������,u32 E_Row����ʱ��������,u32 Addr_Row��ַ������
{
	u32 i;
	u64 Data;
	u8 *p8;
	u16 *p16;
	DisplayClrRow(8, 8);
	p8=(u8 *)ADDR_128KDATABUFF;
	//��ʼʱ��
	p8+=7;
	if(p8[0]==0x1c)
	{
		p8++;
	p16=(u16 *)(ADDR_STRINGBUFF+4+(S_Row)*84+6*2);
	//��
	i=(p8[0]<<8)|p8[1];
	i=hex_bcd(i);
	p16[0]=((i>>4)&0x0f)+0x30+0x2000;
	p16[1]=(i&0xf)+0x30+0x2000;
	p16[2]=0x2D+0x2000;//����-
	//��
	i=hex_bcd(p8[2]);
	p16[3]=(i>>4)+0x30+0x2000;
	p16[4]=(i&0xf)+0x30+0x2000;
	p16[5]=0x2D+0x2000;//����-
	//��
	i=hex_bcd(p8[3]);
	p16[6]=(i>>4)+0x30+0x2000;
	p16[7]=(i&0xf)+0x30+0x2000;
	p16[8]=0x2D+0x2000;//����-
	//ʱ
	i=hex_bcd(p8[4]);
	p16[9]=(i>>4)+0x30+0x2000;
	p16[10]=(i&0xf)+0x30+0x2000;
	p16[11]=0x2D+0x2000;//����-
	//��
	i=hex_bcd(p8[5]);
	p16[12]=(i>>4)+0x30+0x2000;
	p16[13]=(i&0xf)+0x30+0x2000;
	}else
	{
		p8++;
		DisplayString(S_Row,6,0,(u8 *)"--------------");
	}
	//����ʱ��
	p8+=7;
	if(p8[0]==0x1c)
	{
		p8++;
	p16=(u16 *)(ADDR_STRINGBUFF+4+(E_Row)*84+6*2);
	//��
	i=(p8[0]<<8)|p8[1];
	i=hex_bcd(i);
	p16[0]=((i>>4)&0x0f)+0x30+0x2000;
	p16[1]=(i&0xf)+0x30+0x2000;
	p16[2]=0x2D+0x2000;//����-
	//��
	i=hex_bcd(p8[2]);
	p16[3]=(i>>4)+0x30+0x2000;
	p16[4]=(i&0xf)+0x30+0x2000;
	p16[5]=0x2D+0x2000;//����-
	//��
	i=hex_bcd(p8[3]);
	p16[6]=(i>>4)+0x30+0x2000;
	p16[7]=(i&0xf)+0x30+0x2000;
	p16[8]=0x2D+0x2000;//����-
	//ʱ
	i=hex_bcd(p8[4]);
	p16[9]=(i>>4)+0x30+0x2000;
	p16[10]=(i&0xf)+0x30+0x2000;
	p16[11]=0x2D+0x2000;//����-
	//��
	i=hex_bcd(p8[5]);
	p16[12]=(i>>4)+0x30+0x2000;
	p16[13]=(i&0xf)+0x30+0x2000;
	}else
	{
		p8++;
		DisplayString(E_Row,6,0,(u8 *)"--------------");
	}
	//�豸��ַ
	if(p8[0]==0x55)
	{			
		p8+=3;
		DisplayString(Addr_Row,0,0,(u8 *)"��ַ:");
		Data_Inverse((u32)p8,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
		Data=MRR((u32)p8,6);
		p16=(u16 *)(ADDR_STRINGBUFF+Addr_Row*84+4+6*2);
		DisplayData(Data,12,0,p16);
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_3_1_1_JS
// Descriptions:		������ң��״̬
// input parameters:	��
// output parameters:	��
// Returned value:		��
//-------------------------------------------------------------------------------------------------
void Menu2_3_1_1_JS(void)
{
	u32 i;
	u32 x;
	u8 *p8;
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_1_1_JS);//��ʾ�˵�
			p8=(u8 *)ADDR_F203_2;
			//������״̬,��λ��Ϣ					
			if(p8[0]==1)//ȡ����Ч����
			{
				x=p8[1];//�õ�����������·��
				p8+=2;
				if(x==4)//������
				{
					//���ر��
					DisplayString(2, 6, 0, (u8 *)" 1");
					//����״̬
					p8+=3;
					if(p8[0]==1)
					{
						DisplayString(4, 6, 0, (u8 *)"��");
					}else
					{
						DisplayString(4, 6, 0, (u8 *)"��");
					}
					//���ر�λ
					p8+=2;
					if(p8[0]==1)
					{
						DisplayString(6, 6, 0, (u8 *)"��");
					}else
					{
						DisplayString(6, 6, 0, (u8 *)"��");
					}
					//���ر��
					DisplayString(2, 10, 0, (u8 *)" 2");
					//����״̬
					p8+=4;
					if(p8[0]==1)
					{
						DisplayString(4, 10, 0, (u8 *)"��");
					}else
					{
						DisplayString(4, 10, 0, (u8 *)"��");
					}
					//���ر�λ
					p8+=2;
					if(p8[0]==1)
					{
						DisplayString(6, 10, 0, (u8 *)"��");
					}else
					{
						DisplayString(6, 10, 0, (u8 *)"��");
					}
					//���ر��
					DisplayString(2, 14, 0, (u8 *)" 3");
					//����״̬
					p8+=4;
					if(p8[0]==1)
					{
						DisplayString(4, 14, 0, (u8 *)"��");
					}else
					{
						DisplayString(4, 14, 0, (u8 *)"��");
					}
					//���ر�λ
					p8+=2;
					if(p8[0]==1)
					{
						DisplayString(6, 14, 0, (u8 *)"��");
					}else
					{
						DisplayString(6, 14, 0, (u8 *)"��");
					}
					//���ر��
					DisplayString(2, 18, 0, (u8 *)" 4");
					//����״̬
					p8+=4;
					if(p8[0]==1)
					{
						DisplayString(4, 18, 0, (u8 *)"��");
					}else
					{
						DisplayString(4, 18, 0, (u8 *)"��");
					}
					//���ر�λ
					p8+=2;
					if(p8[0]==1)
					{
						DisplayString(6, 18, 0, (u8 *)"��");
					}else
					{
						DisplayString(6, 18, 0, (u8 *)"��");
					}
					//��������
					MR((u32)ADDR_DATABUFF,(u32)ADDR_F203_4,LENmax_F203_4);
					p8=(u8 *)ADDR_DATABUFF;
					if(p8[0]==0x02)
					{
						i=p8[7];
						//��һ·��������
						if(i&0x80)//1�ǳ���
						{
							DisplayString(8, 6, 0, (u8 *)"��");
						}else
						{
							DisplayString(8, 6, 0, (u8 *)"��");
						}
						i<<=1;
						//�ڶ�·��������
						if(i&0x80)//1�ǳ���
						{
							DisplayString(8, 10, 0, (u8 *)"��");
						}else
						{
							DisplayString(8, 10, 0, (u8 *)"��");
						}
						i<<=1;
						//����·��������
						if(i&0x80)//1�ǳ���
						{
							DisplayString(8, 14, 0, (u8 *)"��");
						}else
						{
							DisplayString(8, 14, 0, (u8 *)"��");
						}
						i<<=1;
						//����·��������
						if(i&0x80)//1�ǳ���
						{
							DisplayString(8, 18, 0, (u8 *)"��");
						}else
						{
							DisplayString(8, 18, 0, (u8 *)"��");
						}
					}else
					{
						DisplayString(8, 6, 0, (u8 *)"û�и���Ϣ");
					}
						
					
				}
			}else
			{
				DisplayClrRow(1, 8);
				DisplayString(6, 6, 0, (u8 *)"û����Ч����");
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
// Function name:		Menu2_3_1_1_JS
// Descriptions:		�ն��¼���Ϣ
// input parameters:	��
// output parameters:	��
// Returned value:		��
//-------------------------------------------------------------------------------------------------
void Menu2_3_1_2_JS(void)
{
	u32 i;	
	u32 x;
	u32 Ln;
	u32 S_Row=4;		//�¼��ķ���ʱ��������(�ӵ�0�п�ʼ��)
	u32 E_Row=6;		//�¼��Ľ���ʱ��������		(�ӵ�0�п�ʼ��)
	u32 Addr_Row=8;		//�¼��ĵ���ַ������(�ӵ�0�п�ʼ��)
	u32 MaxScrnNum;
	Ln=1;
	MaxScrnNum=(sizeof(Lcd_EventInfo)/sizeof(Lcd_EventInfo_t));//���·����ж��ٸ���ĻҪ��ʾ(��Ӧ����switch case����Ч����)
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			DisplayMenu(0,(u8 *)pMenu2_3_1_2_1_JS);//��ʾ�˵�
			for(x=0;x<MaxScrnNum;x++)
			{
				i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
				if(i>0)//�ҵ���Ч����
				{
					Stack->MenuStack[MenuCount->Count].MoveRow=x;//���浱ǰ��Ļ���±���(���ں��水��������)
					break;
				}
			}
			if(x>=MaxScrnNum)//˵���Ѿ��ҵ��б�����һ��,��û���ҵ�����
			{
				DisplayClrRow(2, 8);
				DisplayString(Ln+4,4,0,(u8 *)"û���¼���Ϣ");
			}else
			{
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayString(Ln+1, 6, 0, Lcd_EventInfo[x].pEventName);
				DisPlayEventInfo(S_Row,E_Row,Addr_Row);//��ʾ�¼��ķ���ʱ��ͽ���ʱ�� ���б�ı��ַ
			}
			break;
		case 1://����1:ѡ��
			//NextScreenSelect(MaxScrnNum);//������������ѡ��,MaxScrnNum����ж��ٸ���Ļ,Stack->MenuStack[MenuCount->Count].MoveRow��0��ʼ��
			if(Comm_Ram->ButtonVal&0x01)//����
			{
				Comm_Ram->ButtonVal&=~0x03;//�����λ
				x=Stack->MenuStack[MenuCount->Count].MoveRow;//���浱ǰ��Ļ���±���(���ں��水��������)
				if((x+1)<MaxScrnNum)//x�϶���MaxScrnNumС(MaxScrnNum�����Ļ��)
				{
					x++;
					for(;x<MaxScrnNum;x++)
					{
						i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
						if(i>0)//�ҵ���Ч����
						{
							Stack->MenuStack[MenuCount->Count].MoveRow=x;//���浱ǰ��Ļ���±���(���ں��水��������)
							break;
						}
					}
					if(x>=MaxScrnNum)//û����Ч����
					{
						MaxScrnNum=Stack->MenuStack[MenuCount->Count].MoveRow;//���浱ǰ��Ļ���±���(���ں��水��������)
						x=0;
						for(;x<MaxScrnNum;x++)
						{
							i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
							if(i>0)//�ҵ���Ч����
							{
								Stack->MenuStack[MenuCount->Count].MoveRow=x;//���浱ǰ��Ļ���±���(���ں��水��������)
								break;
							}
						}
						if(x>=MaxScrnNum)
						{
							return ;//û���µ����ݻ���û�б仯
						}
					}
				}else
				{
					MaxScrnNum=Stack->MenuStack[MenuCount->Count].MoveRow;//���浱ǰ��Ļ���±���(���ں��水��������)
					x=0;
					for(;x<MaxScrnNum;x++)
					{
						i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
						if(i>0)//�ҵ���Ч����
						{
							Stack->MenuStack[MenuCount->Count].MoveRow=x;//���浱ǰ��Ļ���±���(���ں��水��������)
							break;
						}
					}
					if(x>=MaxScrnNum)
					{
						return ;//û���µ����ݻ���û�б仯
					}
				}
				DisplayString(Ln+1, 6, 0, Lcd_EventInfo[x].pEventName);
				DisPlayEventInfo(S_Row,E_Row,Addr_Row);//��ʾ�¼��ķ���ʱ��ͽ���ʱ�� ���б�ı��ַ
			}			
			if(Comm_Ram->ButtonVal&0x02)//����
			{
				Comm_Ram->ButtonVal&=~0x03;//�����λ
				x=Stack->MenuStack[MenuCount->Count].MoveRow;//���浱ǰ��Ļ���±���(���ں��水��������)
				if(x!=0)//�����ǰ��ʾ�Ĳ��ǵ�һ���¼�
				{
					do
					{
						x--;//������ǰ�¼������±�
						i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
						if(i>0)//�ҵ���Ч����
						{
							Stack->MenuStack[MenuCount->Count].MoveRow=x;//���浱ǰ��Ļ���±���(���ں��水��������)
							break;
						}
					}while(x!=0);
					if((x==0)&&(i==0))//û������(�������±����ֿ�ʼ��ǰ��)
					{
						x=MaxScrnNum;//�����Ļ��
						x--;//�����Ļ�±���
						MaxScrnNum=Stack->MenuStack[MenuCount->Count].MoveRow;//���浱ǰ��Ļ���±���(���ں��水��������)
						while(x>MaxScrnNum)
						{
							i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���				
							if(i>0)//�ҵ���Ч����
							{
								Stack->MenuStack[MenuCount->Count].MoveRow=x;//���浱ǰ��Ļ���±���(���ں��水��������)
								break;
							}
							x--;//�����Ļ�±�
						}
						if((i==0)&&(x==MaxScrnNum))//û���ѵ���Ч����
						{
							return;//û���µ����ݻ���û�б仯
						}
					}
				}else//������Ļ�����ǵ�һ����Ļ
				{
					x=MaxScrnNum;//�����Ļ��
					x--;//�����Ļ�±���
					MaxScrnNum=Stack->MenuStack[MenuCount->Count].MoveRow;//���浱ǰ��Ļ���±���(���ں��水��������)
					while(x!=MaxScrnNum)
					{
						i=Lcd_Get_EventData(0,Lcd_EventInfo[x].EventOAD+1,(u8 *)ADDR_128KDATABUFF,0);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���				
						if(i>0)//�ҵ���Ч����
						{
							Stack->MenuStack[MenuCount->Count].MoveRow=x;//���浱ǰ��Ļ���±���(���ں��水��������)
							break;
						}
						x--;//�����Ļ�±�
					}
					if((i==0)&&(x==MaxScrnNum))//û���ѵ���Ч����
					{
						return;//û���µ����ݻ���û�б仯
					}
				}
				DisplayString(Ln+1, 6, 0, Lcd_EventInfo[x].pEventName);
				DisPlayEventInfo(S_Row,E_Row,Addr_Row);//��ʾ�¼��ķ���ʱ��ͽ���ʱ�� ���б�ı��ַ
			}
			break;
		default:
			break;
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0

}
//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_3_1_2_JS
// Descriptions:		����¼���Ϣ
// input parameters:	��
// output parameters:	��
// Returned value:		��
//-------------------------------------------------------------------------------------------------
/*
void Menu2_3_1_2_2_JS(void)
{
	u8 *p8;
	u16 *p16;
	u16 *pAddr;
	u32 i;
	u32 Ln;
	u64 Data;
	u32 MaxScrnNum=8;//���·����ж��ٸ���ĻҪ��ʾ(��Ӧ����switch case����Ч����)
	Ln=1;
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(0,(u8 *)pMenu2_3_1_2_2_JS);//��ʾ�˵�
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUM_RMmax-1;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);

			//ͨ�ŵ�ַ
			p8=(u8*)ADDR_DATABUFF+1024;
			MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v2,LENper_6000);
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,1,LENper_6000);
			if(p8)
			{
				MR(ADDR_DATABUFF,(u32)p8+3,6);
				p16=(u16 *)ADDR_DATABUFF;
				pAddr=(u16 *)ADDR_AddrList;
				for(i=0;i<3;i++)
				{
					pAddr[i]=p16[i];
				}
				Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				Data=MRR(ADDR_DATABUFF,6);
				p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+6*2);
				DisplayData(Data,12,0,p16);
			}else
			{
				DisplayString(Ln+5,6,0,(u8 *)"------------");
				pAddr=0;
			}
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUM_RMmax-1);
			NextScreenSelect(MaxScrnNum);//������������ѡ��,MaxScrnNum����ж��ٸ���Ļ,Stack->MenuStack[MenuCount->Count].MoveRow��0��ʼ��
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
	switch(Stack->MenuStack[MenuCount->Count].MoveRow)//�˵��������
	{
		case 0://������ݱ��
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"����:  ������ݱ��");
			i=Lcd_Get_EventData(pAddr,0x311C0200+1,(u8*)ADDR_128KDATABUFF,1);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
			break;
		case 1://���ܱ�ʾ���½�
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"����:  ���ʾ���½�");
			i=Lcd_Get_EventData(pAddr,0x310B0200+1,(u8*)ADDR_128KDATABUFF,1);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
			break;
		case 2://����������
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"����:  ����������");
			i=Lcd_Get_EventData(pAddr,0x310C0200+1,(u8*)ADDR_128KDATABUFF,1);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
			break;
		case 4://���ܱ����
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"����:  ���ܱ����");
			i=Lcd_Get_EventData(pAddr,0x310D0200+1,(u8*)ADDR_128KDATABUFF,1);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
			break;		
		case 5://���ܱ�ͣ��
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"����:  ���ܱ�ͣ��");
			i=Lcd_Get_EventData(pAddr,0x310E0200+1,(u8*)ADDR_128KDATABUFF,1);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
			break;
		case 6://ʱ�䳬��
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"����:  ʱ�䳬��");
			i=Lcd_Get_EventData(pAddr,0x31050200+1,(u8*)ADDR_128KDATABUFF,0);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
			break;
		case 7://485����ʧ��
			DisplayClrRow(Ln+2, Ln+2);
			DisplayString(Ln+2,0,0,(u8 *)"����:  485����ʧ��");
			//i=Lcd_Get_EventData(pAddr,0x310A0200+1,(u8*)ADDR_128KDATABUFF,1);//Get���һ���¼�������Ϣ pOut�����¼���������ʱ��,type 0�ն�1���
			i=1;
			break;
		default:
		break;
	}
	if(i==0)//ȡ����Ч����
	{
		p8=(u8 *)ADDR_128KDATABUFF;
		p8+=7;
		if(p8[0]==0x1c)
		{
			p8++;
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
		//��
		i=(p8[0]<<8)|p8[1];
		i=hex_bcd(i);
		p16[0]=((i>>4)&0x0f)+0x30+0x2000;
		p16[1]=(i&0xf)+0x30+0x2000;
		p16[2]=0x2D+0x2000;//����-
		//��
		i=hex_bcd(p8[2]);
		p16[3]=(i>>4)+0x30+0x2000;
		p16[4]=(i&0xf)+0x30+0x2000;
		p16[5]=0x2D+0x2000;//����-
		//��
		i=hex_bcd(p8[3]);
		p16[6]=(i>>4)+0x30+0x2000;
		p16[7]=(i&0xf)+0x30+0x2000;
		p16[8]=0x2D+0x2000;//����-
		//ʱ
		i=hex_bcd(p8[4]);
		p16[9]=(i>>4)+0x30+0x2000;
		p16[10]=(i&0xf)+0x30+0x2000;
		p16[11]=0x2D+0x2000;//����-
		//��
		i=hex_bcd(p8[5]);
		p16[12]=(i>>4)+0x30+0x2000;
		p16[13]=(i&0xf)+0x30+0x2000;
		}else
		{
			DisplayString(Ln+3,6,0,(u8 *)"--------------");
		}
		p8+=7;
		if(p8[0]==0x1c)
		{
			p8++;
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
		//��
		i=(p8[0]<<8)|p8[1];
		i=hex_bcd(i);
		p16[0]=((i>>4)&0x0f)+0x30+0x2000;
		p16[1]=(i&0xf)+0x30+0x2000;
		p16[2]=0x2D+0x2000;//����-
		//��
		i=hex_bcd(p8[2]);
		p16[3]=(i>>4)+0x30+0x2000;
		p16[4]=(i&0xf)+0x30+0x2000;
		p16[5]=0x2D+0x2000;//����-
		//��
		i=hex_bcd(p8[3]);
		p16[6]=(i>>4)+0x30+0x2000;
		p16[7]=(i&0xf)+0x30+0x2000;
		p16[8]=0x2D+0x2000;//����-
		//ʱ
		i=hex_bcd(p8[4]);
		p16[9]=(i>>4)+0x30+0x2000;
		p16[10]=(i&0xf)+0x30+0x2000;
		p16[11]=0x2D+0x2000;//����-
		//��
		i=hex_bcd(p8[5]);
		p16[12]=(i>>4)+0x30+0x2000;
		p16[13]=(i&0xf)+0x30+0x2000;
		}else
		{
			DisplayString(Ln+4,6,0,(u8 *)"--------------");
		}
	}else
	{
		DisplayString(Ln+3,6,0,(u8 *)"--------------");
		DisplayString(Ln+4,6,0,(u8 *)"--------------");
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:		Menu2_3_1_1_JS
// Descriptions:		�¼���Ϣ
// input parameters:	��
// output parameters:	��
// Returned value:		��
//-------------------------------------------------------------------------------------------------
void Menu2_3_1_2_JS(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_1_2_JS);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. ң��״̬
						MenuCount->v1=0;
						MenuCreate((u32)Menu2_3_1_2_1_JS);//�����¼��˵�
						break;
					case 2://2. �¼���Ϣ
						MenuCount->v2=1;
						MenuCreate((u32)Menu2_3_1_2_2_JS);//�����¼��˵�
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
*/
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_1_JS
// Descriptions:        ����������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_1_JS(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_1_JS);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. ң��״̬
						MenuCount->v1=0;//��֤����һ�ζ�Flash����һ��
						MenuCreate((u32)Menu2_3_1_1_JS);//�����¼��˵�
						break;
					case 2://2. �¼���Ϣ
						MenuCreate((u32)Menu2_3_1_2_JS);//�����¼��˵�
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
// Function name:       Menu2_3_2_JS
// Descriptions:        �ն���Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_2_JS(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 MaxScrnNum=2;
	NextScreenSelect(MaxScrnNum);//������������ѡ��,MaxScrnNum����ж��ٸ���Ļ
	switch (Stack->MenuStack[MenuCount->Count].MoveRow)
	{
		case 0:
			DisplayMenu(0,(u8 *)pMenu2_3_1);//��ʾ�˵�
			//1.���̴���
			//1.����汾
			p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+9*2);
			p16[0]=0x2000+'V';
			p16[1]=(SVER>>8)+0x2030;
			p16[2]='.'+0x2000;
			p16[3]=((SVER>>4)&0xf)+0x2030;
			p16[4]=((SVER>>0)&0xf)+0x2030;

			//2.����汾����
			p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+5*2);
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
			p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+9*2);
			p16[0]=0x2000+'V';
			p16[1]=(HVER>>8)+0x2030;
			p16[2]='.'+0x2000;
			p16[3]=((HVER>>4)&0xf)+0x2030;
			p16[4]=((HVER>>0)&0xf)+0x2030;
			//4.Ӳ���汾����
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+5*2);
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
			break;
		case 1:
			DisplayMenu(0,(u8 *)pMenu2_3_2_JS);//��ʾ�˵�
			//����汾+�汾����
			p8=(u8*)ADDR_F209_2;
			p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+4*2);
			i=p8[29];
			//i=hex_bcd(i);
			p16[0]=((i>>4)&0xf)+0x2000+0x30;
			p16[1]=(i&0xf)+0x2000+0x30;
			p16[2]='.'+0x2000;
			i=p8[30];
			//i=hex_bcd(i);
			p16[3]=((i>>4)&0xf)+0x2000+0x30;
			p16[4]=(i&0xf)+0x2000+0x30;
			p16[5]=' '+0x2000;
			//��
			i=(p8[23]<<8)|p8[24];
			i=hex_bcd(i);
			p16[6]=((i>>12)&0xf)+0x2000+0x30;
			p16[7]=((i>>8)&0xf)+0x2000+0x30;
			p16[8]=((i>>4)&0xf)+0x2000+0x30;
			p16[9]=(i&0xf)+0x2000+0x30;
			p16[10]='-'+0x2000;
			//��
			i=p8[25];
			i=hex_bcd(i);
			p16[11]=((i>>4)&0xf)+0x2000+0x30;
			p16[12]=(i&0xf)+0x2000+0x30;
			p16[13]='-'+0x2000;
			//��
			i=p8[26];
			i=hex_bcd(i);
			p16[14]=((i>>4)&0xf)+0x2000+0x30;
			p16[15]=(i&0xf)+0x2000+0x30;
			//���̴���
			p16=(u16 *)(ADDR_STRINGBUFF+4+4*84+10*2);
			i=p8[16];
			p16[0]=(i&0xff)+0x2000;
			i=p8[17];
			p16[1]=(i&0xff)+0x2000;
			break;
		default:
		break;
	}
	UpArrow();//���ϼ�ͷ
	DownArrow();//���¼�ͷ	
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_3_3_JS
// Descriptions:        �ն˹���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_3_JS(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_3_JS);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://2. �ֳ�����
						MenuCreate((u32)Menu2_3_2);//�����¼��˵�
						break;
					case 2://3. �����ն�
						MenuCreate((u32)Menu2_3_3);//�����¼��˵�
						break;
					case 3://4. ��������ʼ��
						MenuCreate((u32)Menu2_3_4);//�����¼��˵�
						break;
					case 4://5������ʼ��
						MenuCreate((u32)Menu2_3_5);//�����¼��˵�
						break;
					case 5://6����������
						MenuCreate((u32)Menu2_3_6);//�����¼��˵�
						break;
					case 6://7�ز��������
						MenuCreate((u32)Menu2_3_7);//�����¼��˵�
						break;
					case 7://USB����
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
// Function name:       Menu2_3_JS
// Descriptions:        �������ն˹�����ά��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_3_JS(void)//
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_3_JS);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. ����������
						MenuCreate((u32)Menu2_3_1_JS);//�����¼��˵�
						break;
					case 2://2. ��������Ϣ
						MenuCreate((u32)Menu2_3_2_JS);//�����¼��˵�
						break;
					case 3://3. ����������
						MenuCreate((u32)Menu2_3_3_JS);//�����¼��˵�
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

