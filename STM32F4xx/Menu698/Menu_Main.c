
//���˵�
#include "Project698.h"
#include "Menu.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "string.h"
#include "../Display/Draw.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/MS.h"
const u8 SList[][2]=//���������б�
{
	" ",//+����ʾ
	"-",
};
const u8 UnitList[][6]=//ͨ�õ�λ�б�
{
	"kW",    //0
	"kvar",  //1
	"kWh",   //2
	"kvarh", //3
	"V",     //4
	"A",     //5
	"%",     //6
	"h",     //7
	"min",   //8
	"s",     //9
	"��",    //10
};
const u8 ProtocolList[][13]=//ͨ��Э���б�
{
	"δ֪    ",//0 
	"645-1997",//1 DL/T 645-1997
	"645-2007",//2 DL/T 645-2007	
	"698.45  ",//3 DL/T698.45
	"188-2004",//4 CJ/T 188-2004
	"ABB     ",//5 ABB
	"IEC1107 ",//6 ������
};

void _0(void)
{
}

#if (USER/100)==17 //����
const u8 pE2_Main2[]=//���������˵�
{
	"\n\r"
	"   ������������ʾ\n\r"
	"   ����������鿴\n\r"
	"   �ն˹�����ά��\n\r"
};

#else
const u8 pE2_Main2[]=//���������˵�
{
	"���˵�\n\r"
	"   ������������ʾ\n\r"
	"   ����������鿴\n\r"
	"   �ն˹�����ά��\n\r"
};
#endif
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
	"  8.�ն˹�����ά��\n\r"
};


//-------------------------------------------------------------------------------------------------
// Function name:       PassWord
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
//-------------------------------------------------------------------------------------------------
void Menu2_2_1(void);
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
	#if(USER/100)==17
	u32 Flag;
	Flag=0;
	#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(LnTitle,ColTitle,0,(u8*)"��������");
			DisplayString(Ln,Col,0,(u8*)"0*****>");
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
			#if(USER/100)==17 //����ʡ
			i=Stack->MenuStack[MenuCount->Count].EditRow;
			if(i>0)
			{
				i--;
				Stack->MenuStack[MenuCount->Count].EditRow=i;
				Flag=1;
			}
			#endif
			UpDownSelect();
			#if(USER/100)==17 //����ʡ
			if(Flag!=0)
			{
				i=Stack->MenuStack[MenuCount->Count].EditRow;
				i++;
				Stack->MenuStack[MenuCount->Count].EditRow=i;
			}
			#endif
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
			if(((p32[0]&0xffffff)==(MenuCount->v1&0xffffff))||((PWD_1==(MenuCount->v1&0xffffff))))
			{//�Ƚ��������������ȷ�������Ƿ����
				#if ((Project/100)==2)//������
//					#if (USER/100)==0//���Ժ����
//					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==1)
//					{
//						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2_1;
//					}
//					#else
					if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==2)
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
					}
//					#endif
					else if(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==3)
					{
						#if (USER/100)==17 //����
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3_JS;//�����¼��˵� 3.�ն˹�����ά��
						#else
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
						#endif
					}
					else
					{
						MenuDel();//�����ϼ��˵�	    
					}
				#endif
				#if ((Project/100)==3)//ר��
					i=Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow;
					i=MenuCount->Count;
					if((Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)&&(MenuCount->Count==3))
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
					}
					else if((Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)&&(MenuCount->Count==2))
					{
						Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
					}
					else
					{
						MenuDel();//�����ϼ��˵�	    
					}
				#endif
				#if ((Project/100)==4)//����һ
					if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
					{
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
					}
					else
					{
						i=Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow;
						i=MenuCount->Count;
						if((Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)&&(MenuCount->Count==3))
						{
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_2;
						}
						else if((Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow==8)&&(MenuCount->Count==2))
						{
							Stack->MenuStack[MenuCount->Count].PC=(u32)Menu2_3;
						}
						else
						{
							MenuDel();//�����ϼ��˵�	    
						}
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
				DisplayString(4,6,0,(u8*)"�������");
				DisplayString(5,3,0,(u8*)"�밴���ؼ�����");
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
// Function name:       E2_Main
// Descriptions:        ���˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_Main(void)//���˵�
{
	DOTMIXPMODE->Mix=2;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
	Draw_Clr();
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
							#if (USER/100)==17 //����
							MenuCreate((u32)Menu_4_JS);//�����¼��˵�  1.���ܱ�������ʾ
							#else
							MenuCreate((u32)Menu_4);//�����¼��˵�  1.���ܱ�������ʾ
							#endif
							break;
						case 2:
//						#if (USER/100)==0//���Ժ����
//							MenuCreate((u32)Menu2_2);//�����¼��˵� 2.����������鿴
//							break;
//						#endif
						case 3:
							//�ǹ���ģʽ������ȷ���룬���ɽ���2��3�Ӳ˵�
							//����ģʽ�£���ʱ�ɽ���2��3�Ӳ˵�
							if(Comm_Ram->Factory==0x55)
							{//����״̬
								if(Stack->MenuStack[MenuCount->Count].EditRow==2)
								{
									MenuCreate((u32)Menu2_2);//�����¼��˵� 2.����������鿴
								}
								if(Stack->MenuStack[MenuCount->Count].EditRow==3)
								{
									#if (USER/100)==17 //����
									MenuCreate((u32)Menu2_3_JS);//�����¼��˵� 3.�ն˹�����ά��
									#else
									MenuCreate((u32)Menu2_3);//�����¼��˵� 3.�ն˹�����ά��
									#endif
								
								}
							}
							else
							{
								PassWordCreateEdit();//��������༭��
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
									MenuCreate((u32)Menu2_3);//�����¼��˵�
								}
								else
								{
									PassWordCreateEdit();
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
	#if ((Project/100)==4)//����һ�ն�
		if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
		{
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
								MenuCreate((u32)Menu_4);//�����¼��˵�  1.���ܱ�������ʾ
								break;
							case 2:
							case 3:
								//�ǹ���ģʽ������ȷ���룬���ɽ���2��3�Ӳ˵�
								//����ģʽ�£���ʱ�ɽ���2��3�Ӳ˵�
								if(Comm_Ram->Factory==0x55)
								{//����״̬
									if(Stack->MenuStack[MenuCount->Count].EditRow==2)
									{
										MenuCreate((u32)Menu2_2);//�����¼��˵� 2.����������鿴
									}
									if(Stack->MenuStack[MenuCount->Count].EditRow==3)
									{
										MenuCreate((u32)Menu2_3);//�����¼��˵� 3.�ն˹�����ά��
									}
								}
								else
								{
									PassWordCreateEdit();//��������༭��
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
		}
		else
		{
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
									MenuCreate((u32)Menu2_3);//�����¼��˵�
								}
								else
								{
									PassWordCreateEdit();
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
void MEMWrite(u32 ADDR_RAM,u32 ADDR_FLASH,u32 Byte);//�洢��д
void MEMClr(u8 Data,u32 ADDR_FLASH,u32 Byte);//�洢�����
u64 MEMReadReg(u32 ADDR_FLASH,u8 Byte);//�洢�������Ĵ���(<=8�ֽ�)
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
	if(MEMReadReg(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
#else
	if(MEMReadReg(ADDR_TerminalFMTimer+12,2))
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






