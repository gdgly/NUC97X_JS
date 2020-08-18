/*************************************************
��Ȩ:��̩�Ǳ������ն˲�
�ļ�:Menu_8_TerminalManagement.c
����:zyl
�汾:1.00
����:20170717
����:�������ز˵�8�ն˹�����ά�������ļ�
*************************************************/

#include <string.h>

//�˵�8 �ն˹�����ά��
#include "../Hcsg/ProjectCSG.h"
#include "../Display/DisplayLoop.h"
#include "../Display/DisplayLoop_Warning.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../Display/Display.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"

#include "Menu.h"
#include "Menu8.h"
#include "Menu4.h"
#include "../Device/MEMRW.h"
#include "../Device/RTC.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../QCSG_Head/QCSG_Hal.h"




const u8 pMenu_8[]=
{
	"�ն˹�����ά��\n\r"
	"�����ն�\n\r"
	"Һ������\n\r"
	"״̬�鿴\n\r"
	"�ź�ǿ�Ⱥ͵�ص���\n\r"
	"����ʵʱ״̬\n\r"
	"�ն�ʱ������\n\r"
	"������������\n\r"
	"����ͨ������\n\r"
	"�����������\n\r"
	"�ն˱��\n\r"
	"�ն˰汾\n\r"
};

#if ((Project/100)==2)
const u8 JZQpMenu_8[]=
{
	"�ն˹�����ά��\n\r"
	"�����ն�\n\r"
	"Һ������\n\r"
	"�ź�ǿ�Ⱥ͵�ص���\n\r"
	"����ʵʱ״̬\n\r"
	"�ն�ʱ������\n\r"
	"������������\n\r"
	"����ͨ������\n\r"
	"�ն˱��\n\r"
	"�ն˰汾\n\r"
};
#endif

const u8 pMenu_8_1[]=
{
	"  \n\r"
	"�ն�Ӳ����λ\n\r"
	"��������λ\n\r"
	"�ָ���������\n\r"
};

const u8 pMenu_8_2[]=
{
	"\n\r"
	"��������\n\r"
	"�رձ���\n\r"
	"�Աȶ�����\n\r"
	"�ԱȶȽ���\n\r"
};


const u8 pMenu_8_3[]=
{
	"\n\r"
	"������״̬��Ϣ\n\r"
	"�ն�״̬��Ϣ\n\r"
};


const u8 pMenu_8_3_1[]=
{
	"\n\r"
	"������ţ�\n\r"
	"        ȷ��\n\r"
};

const u8 pMenu_8_3_2[]=
{
	"\n\r"
	"ң��״̬\n\r"
	"�Žڵ�״̬\n\r"
	"ͣ��ͳ��\n\r"
};


const u8 pMenu_8_4[]=
{
	"  �ź�ǿ�Ⱥ͵�ص���\n\r"
	
	"�ź�ǿ�ȣ�\n\r"
	"ͣ���ص�ѹ��\n\r"
	"ͣ���ص�����\n\r"
	"ʱ�ӵ�ص�����\n\r"
};


const u8 pMenu_8_6[]=
{
	"\n\r"
	"����:\n\r"
	"ʱ��:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_7[]=
{
	"\n\r"
	"��������:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_8[]=
{
	"\n\r"
	"485�˿�1\n\r"
	"485�˿�2\n\r"
	"485�˿�3\n\r"
};


const u8 pMenu_8_8_1[]=
{
	"\n\r"
	"\n\r"
	"����:\n\r"	
	"������:\n\r"
	"У�鷽ʽ:\n\r"
	"����λ:\n\r"
	"ֹͣλ:\n\r"
	"      ȷ������\n\r"
};


const u8 pMenu_8_10[]=
{
	"\n\r"
	"������������\n\r"
	"�߼���ַ����\n\r"
};

const u8 pMenu_8_10_1[]=
{
	"\n\r"
	"��������:\n\r"
	"      ȷ������\n\r"
};


const u8 pMenu_8_10_2[]=
{
	"\n\r"
	"�߼���ַ:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_11[]=
{
	"\n\r"
	"      �ն���Ϣ\n\r"
	"  ���̴��룺\n\r"
	"  �����汾��\n\r"
	"  �������ڣ�\n\r"
	"  Ӳ���汾��\n\r"
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_1
// Descriptions:        �����ն�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_1(void)//�����ն�
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_1);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://1. �ն�Ӳ����λ
						MenuCreate((u32)Menu_8_1_1);//�����¼��˵�
						break;
					case 2://2. ��������λ
						MenuCreate((u32)Menu_8_1_2);//�����¼��˵�
						break;
					case 3://3. �ָ���������
						MenuCreate((u32)Menu_8_1_3);//�����¼��˵�
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_1_1
// Descriptions:        �ն�Ӳ����λ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_1_1(void)
{
	u16 *p16;

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
				{//YES �ն�Ӳ����λ
					DataResetAction(0x02,GUI_PORT);
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
// Function name:       Menu_8_1_2
// Descriptions:        ��������λ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_1_2(void)
{
	u16 *p16;

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
				{//YES ��������ʼ��
					DataResetAction(0x01,GUI_PORT);				
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
// Function name:       Menu_8_1_3
// Descriptions:        �ָ���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_1_3(void)
{
	u16 *p16;

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
				{//YES �ָ���������
					DataResetAction(0x00,GUI_PORT);		
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
// Function name:       Menu_8_2
// Descriptions:        Һ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_2(void)
{	
	if((Comm_Ram->ButtonVal&0x2F)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2F;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3
// Descriptions:        ״̬�鿴
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_3(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_3);//��ʾ�˵�	
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_8_3_1);//������״̬��Ϣ
						break;
					case 2:
						MenuCreate((u32)Menu_8_3_2);//�ն�״̬��Ϣ
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_1
// Descriptions:        ������״̬��Ϣ��ѡ������㣩
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_3_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j = 0,Ln=0;		//�˵��ӵ�0�п�ʼ
	u32 FlashLenAddr = 0, FlashLen = 0;		
	MeasurePointStruct  MeasurePoint;	
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_3_1);//��ʾ�˵�
			
		  //Ĭ����ʾ�������Ϊ0
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			bcd_p8(p,0,4);		
			MenuCount->v1=9;
			MenuCount->v2=12;
			NegativeLnMaxColMinCol(Ln+1,MenuCount->v2,MenuCount->v1);
			if(Stack->MenuStack[MenuCount->Count].EditRow==2)
			{
				Stack->MenuStack[MenuCount->Count].EditRow=1;
				MenuCount->v1=9;
				MenuCount->v2=12;
				NegativeLnMaxColMinCol(Ln+1,MenuCount->v2,MenuCount->v1);
			}
			break;		
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
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
								//�������õ�����
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p,4);
								FlashLen = x;
								FlashLenAddr = ADDR_MP_START + (FlashLen * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct)));
								FlashLen = sizeof(MeasurePointStruct) ;
								MR((u32)&MeasurePoint,FlashLenAddr,FlashLen);
								if((MeasurePoint.State == 0x01) && (BCD_Check(MeasurePoint.Addr, METER_ADDR_SIZE) == NO_ERROR))
								{
									//MC(0,(u32)Pn_Lcd_Data,sizeof(QCSG_LCD_RAMDATA_S));
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;								
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}								
								MenuCreate((u32)Menu_8_3_1_1);
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x1F;//��ʹ�ú���0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x1F;//��ʹ�ú���0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					Select(MenuCount->v2,MenuCount->v1);
					UpDownSelect();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						if(Stack->MenuStack[MenuCount->Count].EditRow==1)
						{
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,4);
								if(x>MAX_PN_NUM)
								{
									bcd_p8(p8,9999,4);
								}
						}
						Stack->MenuStack[MenuCount->Count].EditCol=0;
						Comm_Ram->ButtonVal&=~0x1F;//��ʹ�ú���0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x1F;//��ʹ�ú���0
						break;
					}
				}
			}
			
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;
			break;
		default:
			break;
	}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_1_1
// Descriptions:        ������״̬��Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_3_1_1(void)//������״̬��Ϣ
{
	u16 *p16;
	u32 Ln = 0,Databuff;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	
	DisplayClr();//��ȫ��
	DisplayString(1,6,0,"������");
	DisplayString(2,0,0,"��ѹ����:�� �� ��");
	DisplayString(3,0,0,"��������:�� �� ��");
	DisplayString(4,0,0,"��ѹ������:��    ");
	DisplayString(5,0,0,"����������:��    ");
  if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,5);//����Ļ��ʾ��ָ����
		DisplayString(Ln+4,4,0,"�����㲻����");
	}
	else
	{
		if(Pn_Lcd_Data->u16Pn<0x0A)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
			DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
		}
		else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
		}
		else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
		}
		else if(Pn_Lcd_Data->u16Pn>=0x3E8)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
		}	

		//��ѹ����(A�����)
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[6],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(2,9,0,"��");
		}
		else
		{
			Databuff &= 0x8000;
			if(Databuff == 0x8000)
			{
				DisplayString(2,9,0,"��");
			}
			else
			{
				DisplayString(2,9,0,"��");
			}		
		}
		//B�����
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(2,12,0,"��");
		}
		else
		{
			Databuff &= 0x8000;
			if(Databuff == 0x8000)
			{
				DisplayString(2,12,0,"��");
			}
			else
			{
				DisplayString(2,12,0,"��");
			}		
		}
		//C�����
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(2,15,0,"��");
		}
		else
		{
			Databuff &= 0x8000;
			if(Databuff == 0x8000)
			{
				DisplayString(2,15,0,"��");
			}
			else
			{
				DisplayString(2,15,0,"��");
			}		
		}
		//ABC���������(A���������)
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[6],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(3,9,0,"��");
		}
		else
		{
			Databuff &= 0x4000;
			if(Databuff == 0x4000)
			{
				DisplayString(3,9,0,"��");
			}
			else
			{
				DisplayString(3,9,0,"��");
			}		
		}
		//B���������
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(3,12,0,"��");
		}
		else
		{
			Databuff &= 0x4000;
			if(Databuff == 0x4000)
			{
				DisplayString(3,12,0,"��");
			}
			else
			{
				DisplayString(3,12,0,"��");
			}		
		}
		//C���������
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(3,15,0,"��");
		}
		else
		{
			Databuff &= 0x4000;
			if(Databuff == 0x4000)
			{
				DisplayString(3,15,0,"��");
			}
			else
			{
				DisplayString(3,15,0,"��");
			}		
		}
		//��ѹ������
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[12],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(4,0,0,"��ѹ������:��        ");
		}
		else
		{
			Databuff &= 0x0100;
			if(Databuff == 0x0100)
			{
				DisplayString(4,0,0,"��ѹ������:��        ");
			}
			else
			{
				DisplayString(4,0,0,"��ѹ������:��        ");
			}		
		}
		//����������
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data040005FF[12],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(5,0,0,"����������:��        ");
		}
		else
		{
			Databuff &= 0x0200;
			if(Databuff == 0x0200)
			{
				DisplayString(5,0,0,"����������:��        ");
			}
			else
			{
				DisplayString(5,0,0,"����������:��        ");
			}		
		}
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_2
// Descriptions:        �ն�״̬��Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_3_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_3_2);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_8_3_2_1);//ң��״̬
						break;
					case 2:
						MenuCreate((u32)Menu_8_3_2_2);//�Žڵ�״̬					
						break;
					case 3:							
						MenuCreate((u32)Menu_8_3_2_3);//ͣ��ͳ��		
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_2_1
// Descriptions:        ң��״̬
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_3_2_1(void)
{
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//��ȫ��
	DisplayString(1,6,0,"ң��״̬");
	DisplayString(2,0,0,"4·ң��״̬:");
	
	//ң��״̬
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataTerminalStateWord[0],1);
	if(Databuff==0xFF)
	{
		DisplayString(Ln+3,0,0,"�� �� �� ��");
	}
	if((Databuff&0x08)==0x08)
	{
		DisplayString(Ln+3,0,0,"��");
	}
	if((Databuff&0x08)==0)
	{
		DisplayString(Ln+3,0,0,"��");
	}
	if((Databuff&0x10)==0x10)
	{
		DisplayString(Ln+3,3,0,"��");
	}
	if((Databuff&0x10)==0)
	{
		DisplayString(Ln+3,3,0,"��");
	}
	if((Databuff&0x20)==0x20)
	{
		DisplayString(Ln+3,6,0,"��");
	}
	if((Databuff&0x20)==0)
	{
		DisplayString(Ln+3,6,0,"��");
	}
	if((Databuff&0x40)==0x40)
	{
		DisplayString(Ln+3,9,0,"��");
	}
	if((Databuff&0x40)==0)
	{
		DisplayString(Ln+3,9,0,"��");
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_2_2
// Descriptions:        �Žڵ�״̬
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_3_2_2(void)
{
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//��ȫ��
	DisplayString(1,5,0,"�Žڵ�״̬");
	DisplayString(2,0,0,"�Žڵ�״̬:");
	
	//�Žڵ�״̬
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataTerminalStateWord[0],1);
	if(Databuff==0xFF)
	{
		DisplayString(Ln+2,11,0,"��");
	}
	if((Databuff&0x01)==1)
	{
		DisplayString(Ln+2,11,0,"��");
	}
	if((Databuff&0x01)==0)
	{
		DisplayString(Ln+2,11,0,"��");
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_3_2_3
// Descriptions:        ͣ��ͳ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_3_2_3(void)
{
	Menu_4();
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_4
// Descriptions:        �ź�ǿ�Ⱥ͵�ص���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_4(void)
{
	u8 *p8;
	u32 x,SignaldBm;
	
	DisplayClr();//��ȫ��
	DisplayString(1,1,0,"�ź�ǿ�Ⱥ͵�ص���");
	DisplayString(3,0,0,"�ź�ǿ��:            ");
	DisplayString(4,0,0,"���õ�ص���:        ");
	#if (Project/100)==3 || ((Project/100)==9)
		DisplayString(5,0,0,"ʱ�ӵ�ص���:        ");
	#endif
	
	SignaldBm = Get_GPRSSignaldBm();                //��ȡ�ź�ǿ��
	if(SignaldBm == 0x99)	
  {
		DisplayString(3,9,0,"0");
	}
	else
	{
		p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+9*2);
		DisplayData(SignaldBm,2,0,(u16*)p8);
	}
	
	
	//��ص���
	x=Comm_Ram->UBACKUP;                    
	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		x=bcd_hex(x);
		if(x>=20)
		{
			x-=20;//�����������ڳ��-0.30V
		}
		else
		{
			x=0;
		}
		x=hex_bcd(x);
	}
	if(x==0)
	{
		DisplayString(4,13,0,"0%");
	}
	if((x>0)&&(x<=RechargeableBatteryGrid1))
	{
		DisplayString(4,13,0,"25%");
	}
	if(x>RechargeableBatteryGrid1)
	{
	  DisplayString(4,13,0,"50%");
	}
	if(x>RechargeableBatteryGrid2)
	{
	  DisplayString(4,13,0,"75%");
	}
	if(x>RechargeableBatteryGrid3)
	{
	  DisplayString(4,13,0,"100%");
	}
		
	#if (Project/100)==3 || ((Project/100)==9)
		//ʱ�ӵ�ص���
		if(Comm_Ram->URTC<0x200)
		{//��ѹС��2.00V
			DisplayString(5,13,0,"0%");
		}
		if((Comm_Ram->URTC >= 0x200)&&(Comm_Ram->URTC < 0x240))
		{//��ѹ��2.00V~2.40V֮��
			DisplayString(5,13,0,"25%");
		}
		if((Comm_Ram->URTC >= 0x240)&&(Comm_Ram->URTC < 0x280))
		{//��ѹ��2.40V~2.80V֮��
			DisplayString(5,13,0,"50%");
		}
		if((Comm_Ram->URTC >= 0x280)&&(Comm_Ram->URTC < 0x320))
		{//��ѹ��2.80V~3.20V֮��
			DisplayString(5,13,0,"75%");
		}
		if((Comm_Ram->URTC >= 0x320)&&(Comm_Ram->URTC < 0x380))
		{//��ѹ��3.20V~3.80V֮��
			DisplayString(5,13,0,"100%");
		}
	#endif
		
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//���� 
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_5
// Descriptions:        ����ʵʱ״̬
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_5(void)
{
	u16 *p16;
	u32 State;	
	u8 IPAddr[16];
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	Terminal_Ram_TypeDef* pTerminal_Ram = Terminal_Ram;
	
	DisplayClr();//��ȫ��
	DisplayString(1,4,0,"����ʵʱ״̬");
	DisplayString(2,0,0,"��ǰ״̬:            ");
	DisplayString(4,0,0,"IP:                  ");//183.247.151.178
	DisplayString(5,0,0,"��Ӫ��:    ");
	DisplayString(6,0,0,"��������:      ");

		
	
	State=Get_GPRSState(GPRSPORT);
	
	if(State==GPRS_STATE_UNPLUGGED)             	//0
	{
		DisplayString(2,9,0,"���ڳ�ʼ��...");
	}
	else if(State==GPRS_STATE_POWER_OFF)             //1
	{
		DisplayString(2,9,0,"GPRS���ڹػ�...");
	}
	else if(State==GPRS_STATE_POWER_ON)              //2
	{
		DisplayString(2,9,0,"GPRS���ڿ���...");
	}
	else if(State==GPRS_STATE_MATCH_BAUD_RATE)       //3
	{
		DisplayString(2,9,0,"����ƥ�䲨����...");
	}
	else if(State==GPRS_STATE_RECONGNIZE_SIM_CARD)   //4
	{
		DisplayString(2,9,0,"����ʶ��SIM��...");
	}
	else if(State==GPRS_STATE_REG_NET)               //5
	{
		DisplayString(2,9,0,"������������...");
	}
	else if(State==GPRS_STATE_CONFIG_LINK_PARA)      //6
	{
		DisplayString(2,9,0,"�������ò���...");
	}
	else if(State==GPRS_STATE_LINKING)               //7
	{
		DisplayString(2,9,0,"��������...");
	}
	else if(State==GPRS_STATE_LINK_FAIL)             //8
	{
		DisplayString(2,9,0,"����ʧ��");
	}
	else if(State==GPRS_STATE_LINK_SUCCESS)          //9
	{
		Get_GPRSLocalIPAddr(IPAddr);
		DisplayString(2,9,0,"���ӳɹ�");
		p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+3*2);
		p16[0]=0x2000 + IPAddr[0];
		p16[1]=0x2000 + IPAddr[1];
		p16[2]=0x2000 + IPAddr[2];
		p16[3]=0x2000 + IPAddr[3];
		p16[4]=0x2000 + IPAddr[4];
		p16[5]=0x2000 + IPAddr[5];
		p16[6]=0x2000 + IPAddr[6];
		p16[7]=0x2000 + IPAddr[7];
		p16[8]=0x2000 + IPAddr[8];
		p16[9]=0x2000 + IPAddr[9];
		p16[10]=0x2000 + IPAddr[10];
		p16[11]=0x2000 + IPAddr[11];
		p16[12]=0x2000 + IPAddr[12];
		p16[13]=0x2000 + IPAddr[13];
		p16[14]=0x2000 + IPAddr[14];
		p16[15]=0x2000 + IPAddr[15];
	}
	else if(State==GPRS_STATE_LOG_SUCCESS)           //10
	{
		Get_GPRSLocalIPAddr(IPAddr);
		DisplayString(2,9,0,"��½��վ�ɹ�");
		p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+3*2);
		p16[0]=0x2000 + IPAddr[0];
		p16[1]=0x2000 + IPAddr[1];
		p16[2]=0x2000 + IPAddr[2];
		p16[3]=0x2000 + IPAddr[3];
		p16[4]=0x2000 + IPAddr[4];
		p16[5]=0x2000 + IPAddr[5];
		p16[6]=0x2000 + IPAddr[6];
		p16[7]=0x2000 + IPAddr[7];
		p16[8]=0x2000 + IPAddr[8];
		p16[9]=0x2000 + IPAddr[9];
		p16[10]=0x2000 + IPAddr[10];
		p16[11]=0x2000 + IPAddr[11];
		p16[12]=0x2000 + IPAddr[12];
		p16[13]=0x2000 + IPAddr[13];
		p16[14]=0x2000 + IPAddr[14];
		p16[15]=0x2000 + IPAddr[15];	
	}
	else
	{
		DisplayString(2,9,0,"            ");
		DisplayString(4,3,0,"                  ");	
		DisplayString(5,7,0,"    ");	
		DisplayString(6,9,0,"      ");		
	}	
	switch(pComm_Ram->CSPN)
	{
		case 0x01:
			DisplayString(5,7,0,"�ƶ�");
			break;
		case 0x02:
			DisplayString(5,7,0,"��ͨ");
			break;
		case 0x03:
			DisplayString(5,7,0,"����");
			break;
		default:
			DisplayString(5,7,0,"����");
		break;	
	}
	
	switch(pTerminal_Ram->LinkCount_SelfAdaption)
	{
		case 0x03:
			DisplayString(6,9,0,"��ͨ��");
			break;
		case 0x04:
			DisplayString(6,9,0,"������");
			break;
		case 0x00:
			DisplayString(6,9,0,"����IP");//�޸�Ϊ������1��ʼ����
			break;
		case 0x01:
			DisplayString(6,9,0,"����1");
			break;
		case 0x02:
			DisplayString(6,9,0,"����2");
			break;
		default:
			DisplayString(6,9,0,"δ֪");
		break;	
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//���� 
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_6
// Descriptions:        �ն�ʱ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_6(void)
{
#if ((Project/100) == 3)
	u8 u8PnNum = 0,ii = 0;
	u8 u8MeterId[6];
	u32 u32RevLen = 0;
	ERD09Struct stERD09Data;
	RealTimeYYStruct stYYTime;
	TIME_PARAM_S pstTime;			
	QCSG_PN_RAM_DATA_LIST_S* pstPn_Ram_Data = (QCSG_PN_RAM_DATA_LIST_S*)QCSG_PN_RAM_DATA_LIST;
	QCSG_RAM_DATA_S* pstRam_Data = NULL;
#endif
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,k,Ln;
	Ln=0;//�˵��ӵ�0�п�ʼ

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_6);//��ʾ�˵�
			
			//����
			p8=(u8 *)ADDR_MYMDHMS;
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
			p[0*2]=(p8[5]>>4)+0x30;//��
			p[1*2]=(p8[5]&0xf)+0x30;
			p[2*2]='-';
			p[3*2]=(p8[4]>>4)+0x30;//��
			p[4*2]=(p8[4]&0xf)+0x30;
			p[5*2]='-';
			p[6*2]=(p8[3]>>4)+0x30;//��
			p[7*2]=(p8[3]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
			//ʱ��
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
			p[0*2]=(p8[2]>>4)+0x30;//ʱ
			p[1*2]=(p8[2]&0xf)+0x30;
			p[2*2]=':';
			p[3*2]=(p8[1]>>4)+0x30;//��
			p[4*2]=(p8[1]&0xf)+0x30;
			p[5*2]=':';
			p[6*2]=(p8[0]>>4)+0x30;//��
			p[7*2]=(p8[0]&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
			
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ,ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=6;
			MenuCount->v2=13;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							MenuCount->v1=6;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ,ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ,ͬʱ��������е����Ҽ�ͷ
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
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 3:
								//�������õ�����
								p8=(u8 *)(ADDR_RTCBUFF);
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								p8[4]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//��
								p8[5]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//��
								p8[6]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//��
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
								p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//��
								p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//��
								p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//ʱ							
								WriteRTC(ADDR_RTCBUFF);//дʵʱ��
								
								DisplayClrRow_4(Ln+3);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+3,9,0,"OK");

								
								//err20181107 �����¼� ���� �ն�Уʱ��¼�¼� Һ���޸�ʱ��
								#if ((Project/100) == 3)
									

									HAL_GetSystemTime(&pstTime);
									
									if(QCSG_CheckEventMask(0xE2010010) == TRUE)
									{
										u8PnNum = pstPn_Ram_Data->u8PnNum;
										for(ii = 0;ii < u8PnNum;i++)
										{
											//Ѱ���ڴ��м�¼�Ĳ�����������Ϣ
											if(0 == pstPn_Ram_Data->stPn_Ram_Data[ii].u16Pn)
											{
												pstRam_Data = &pstPn_Ram_Data->stPn_Ram_Data[ii];
												break;
											}
										}	
										if(ii >= u8PnNum)
										{
											//Result = ERROR_VerifyFail;
											return;//break;
										}	
										MemCopy(&stERD09Data.Time,&pstTime,sizeof(TIME_PARAM_S));
										MemCopy(stERD09Data.C_00010000,pstRam_Data->u8Data00010000,4);
										MemCopy(stERD09Data.C_00020000,pstRam_Data->u8Data00020000,4);

										GetACSampleAddr(u8MeterId);	//ȡ���ɵ�ַ
										//��¼�¼�
										MemSet(&stYYTime,0,sizeof(stYYTime));
										//RD_Time_TO_YYTime(&pstTime,&stYYTime);//ת��ʱ���ʽ
										stYYTime.YearH = 0x20;
										stYYTime.YearL = pstTime.u8Year;
										stYYTime.Month = pstTime.u8Month;
										stYYTime.Day = pstTime.u8Day;
										stYYTime.Hour = pstTime.u8Hour;
										stYYTime.Minute = pstTime.u8Minute;
										
										u32RevLen = sizeof(stERD09Data);
										NAND_WriteEventRecord(u8MeterId,0xE2010010,&stYYTime,(u8*)&stERD09Data,&u32RevLen);
									}
								#endif
								
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
						if(Stack->MenuStack[MenuCount->Count].EditRow<3)
						{
							p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
							p16[0]=0x2020;
							p16[1]=0x2020;
							p16[2]=32;//'���Ҽ�ͷ'
							p16[3]=33;
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>12)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x31;
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>31)
									{//31��
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
								{
									if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>30)
									{//30��
										p8[7*2]=0x31;
										p8[6*2]=0x30;
									}
								}
								if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
								{
									if((((p8[0*2]-0x30)*10+(p8[1*2]-0x30))%4)==0x0)
									{//����2����29��
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29��
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									else
									{//ƽ��2��ֻ��28��
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>28)
										{//28��
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
								if((p8[0*2]-0x30)*10+(p8[1*2]-0x30)*1>23)
								{
									p8[0*2]=0x30;
									p8[1*2]=0x30;
								}
								if((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1>59)
								{
									p8[3*2]=0x30;
									p8[4*2]=0x30;
								}
								if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>59)
								{
									p8[7*2]=0x30;
									p8[6*2]=0x30;
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
// Function name:       Menu_8_7
// Descriptions:        ������������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_7(void)
{	
	u8 *p,*p8;
	u16 *p16;
	u32 *p32;
	u32 x,j,Ln=0;//�˵��ӵ�0�п�ʼ
	

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_7);//��ʾ�˵�
				
			//��ʾ�ڴ�������
			p8=(u8*)ADDR_DATABUFF;		
			MR(ADDR_DATABUFF,ADDR_GUIPASSWORD,3);			
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			p32=(u32*)p8;			
			bcd_p8(p,p32[0]&0x00FFFFFF,6);
					
			MenuCount->v1=9;
			MenuCount->v2=14;
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
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
					}				
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:	
								//�������õ�����								
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p,6);
								p8=(u8*)&x;
								MW((u32)p8,ADDR_GUIPASSWORD,3);							
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,6,0,"   OK   ");
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
// Function name:       Menu_8_8
// Descriptions:        ����ͨ�����ã�ѡ��˿ںţ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_8(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8);//��ʾ�˵�
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://485�˿�1
						MenuCreate((u32)Menu_8_8_1);//�����¼��˵�           
						break;
					case 2://485�˿�2
						MenuCreate((u32)Menu_8_8_2);//�����¼��˵�
						break;
					case 3://485�˿�3
						MenuCreate((u32)Menu_8_8_3);//�����¼��˵�
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



u32 ConvertRs485PortParaToBitValue(u32 Func, u32 BaudRate, u32 Parity, u32 DataBit, u32 StopBit)
{
	u32 BitValue = 0, BaudRateValue = 0, i = 0;

	BitValue |= (Func & 0x00000007);
	
	BaudRateValue = BaudRate;
	BaudRateValue *= 300;
	for(i = 0; i < MAX_BAUD_NUM; i++)
	{
		if(UartBaudRate[i] == BaudRateValue) break;
	}
	if(i >= MAX_BAUD_NUM) i = BAUD_RATE_1200_ID;

	BitValue |= (i & 0x0000000F)<< 3;

	BitValue |= (Parity & 0x00000003) << 7;

	BitValue |= (DataBit & 0x0000000F) << 9;
	
	BitValue |= (StopBit & 0x00000003) << 13;
	return BitValue;
}

void ConvertBitValueToRs485PortPara(u8* Func, u8* BaudRate, u8* Parity, u8* DataBit, u8* StopBit, u32 BitValue)
{
	u32 i = 0;

	i = BitValue & RS485_FUNCTION_BIT_MASK;
	Func[0] = (u8)i;

	i = BitValue & RS485_BAUD_RATE_BIT_MASK;
	i >>= 3;
	if(i >= MAX_BAUD_NUM) i = BAUD_RATE_1200_ID;

	BaudRate[0] = UartBaudRate[i]/300;

	i = BitValue & RS485_PARITY_BIT_MASK;
	i >>= 7;
	Parity[0] = (u8)i;

	i = BitValue & RS485_DATA_BIT_MASK;
	i >>= 9;
	DataBit[0] = (u8)i;

	i = BitValue & RS485_STOP_BIT_MASK;
	i >>= 13;
	StopBit[0] = (u8)i;
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8_1
// Descriptions:        485�˿�1
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_8_1(void)
{
	u8 *p,*p8;	
	u64 x;
	u32 j, FlashAddr,Ln = 0;   //�˵��ӵ�0�п�ʼ
	RS485PortParaStruct RS485PortPara;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8_1);//��ʾ�˵�
			DisplayString(Ln+1,5,0,"RS485�˿� 1");			
		
			FlashAddr = ADDR_RS485PortParaTbl;
			MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortParaStruct));	
			//��ʾ�ڴ��е�����
			//�˿ڹ���
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Function,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			DisplayData(p8[0],2,0,(u16*)p);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+2,5,0,"485������    ");			
					break;
				case 1:
					DisplayString(Ln+2,5,0,"485���߼���  ");			
					break;
				case 2:
					DisplayString(Ln+2,5,0,"485����      ");			
					break;
				case 3:
					DisplayString(Ln+2,5,0,"485�ϴ�ά����");		
					break;
				default:
					DisplayString(Ln+2,5,0,"485������    ");
					break;
			}
			//ͨ�Ų�����
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.BaudRate,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0x01:
					DisplayString(Ln+3,7,0,"300");				
					break;
				case 0x02:
					DisplayString(Ln+3,7,0,"600");				
					break;
				case 0x04:
					DisplayString(Ln+3,7,0,"1200");				
					break;
				case 0x08:
					DisplayString(Ln+3,7,0,"2400");				
					break;
				case 0x10:
					DisplayString(Ln+3,7,0,"4800");				
					break;
				case 0x20:
					DisplayString(Ln+3,7,0,"9600");
					break;
				case 0x40:
					DisplayString(Ln+3,7,0,"19200");
					break;
				case 0x80:
					DisplayString(Ln+3,7,0,"38400");
					break;
				case 0xC0:
					DisplayString(Ln+3,7,0,"57600");
					break;
				default:
					DisplayString(Ln+3,7,0,"1200");	
					break;	
			}							
			//У�鷽ʽ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Parity,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+4,9,0,"��У��");			
					break;
				case 1:
					DisplayString(Ln+4,9,0,"żУ��");		
					break;
				case 2:
					DisplayString(Ln+4,9,0,"��У��");			
					break;
				default:
					DisplayString(Ln+4,9,0,"żУ��");
					break;
			}				
			//����λ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.DataBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);	
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 5:
					DisplayString(Ln+5,7,0,"5 λ");			
					break;
				case 6:
					DisplayString(Ln+5,7,0,"6 λ");		
					break;
				case 7:
					DisplayString(Ln+5,7,0,"7 λ");			
					break;
				case 8:
					DisplayString(Ln+5,7,0,"8 λ");		
					break;
				default:
					DisplayString(Ln+5,7,0,"8 λ");
					break;
			}	
			//ֹͣλ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.StopBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+6,7,0," 1  λ");			
					break;
				case 1:
					DisplayString(Ln+6,7,0,"1.5 λ");		
					break;
				case 2:
					DisplayString(Ln+6,7,0," 2  λ");			
					break;
				default:
					DisplayString(Ln+6,7,0," 1  λ");
					break;
			}	
			pMenuCount->v3 = ConvertRs485PortParaToBitValue(RS485PortPara.Function, RS485PortPara.BaudRate, 
				               RS485PortPara.Parity, RS485PortPara.DataBit, RS485PortPara.StopBit);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)  //�༭��Ϊ0��ʱ��
				{
					BoxSelect();  //�༭��ѡ��				
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=17;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=7;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=7;
							MenuCount->v2=10;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=7;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// �༭�˿ڹ���
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485�ϴ�ά����");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485������    ");
								}
								break;
							case 3:		// �༭�˿ڲ�����
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								x++;
								if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// �༭�˿ڼ��鷽ʽ
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								x++;
								if(x > UART_PARITY_ODD) x = UART_PARITY_NONE;
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"��У��");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"żУ��");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"��У��");
								}
								break;
							case 5:		// �༭�˿�����λ
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								x++;
								if(x > UART_DATA_BIT_8) x = UART_DATA_BIT_5;
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 λ");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 λ");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 λ");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 λ");
								}
								break;
							case 6:		// �༭�˿�ֹͣλ
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								x++;
								if(x > UART_STOP_BIT_2) x = UART_STOP_BIT_1;
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  λ");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 λ");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  λ");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//���
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// �༭�˿ڹ���
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485�ϴ�ά����");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485������    ");
								}
								break;
							case 3:		// �༭�˿ڲ�����
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								if(x == 0)
								{
									x = BAUD_RATE_9600_ID;
								}
								else
								{
									x--;
									if(x == BAUD_RATE_7200_ID) x = BAUD_RATE_4800_ID;
								}							
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// �༭�˿ڼ��鷽ʽ
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								if(x == UART_PARITY_NONE)
								{
									x = UART_PARITY_ODD;
								}
								else
								{
									x--;
								}
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_NONE << 7);
									DisplayString(4,9,0,"��У��");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_EVEN << 7);
									DisplayString(4,9,0,"żУ��");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_ODD << 7);
									DisplayString(4,9,0,"��У��");
								}
								break;
							case 5:		// �༭�˿�����λ
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								if(x == UART_DATA_BIT_5) 
								{
									x = UART_DATA_BIT_8;
								}
								else
								{
									x--;
								}
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 λ");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 λ");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 λ");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 λ");
								}
								break;
							case 6:		// �༭�˿�ֹͣλ
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								if(x == UART_STOP_BIT_1)
								{
									x = UART_STOP_BIT_2;
								}
								else
								{
									x--;
								}
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  λ");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 λ");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  λ");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ�ϼ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:
								break;
							case 3:
								break;
							case 4:
								break;
							case 5:
								break;
							case 6:
								break;
							case 7:
								//�������õ�����					
								//�˿ڹ���
								FlashAddr = ADDR_RS485PortParaTbl;
								MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
								ConvertBitValueToRs485PortPara(&RS485PortPara.Function, &RS485PortPara.BaudRate, &RS485PortPara.Parity, 
									                           &RS485PortPara.DataBit, &RS485PortPara.StopBit, pMenuCount->v3);
								MW((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
										
								DisplayClrRow_4(Ln+7);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+7,9,0,"OK");																					
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8_2
// Descriptions:        485�˿�2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_8_2(void)
{
	u8 *p,*p8;
	u64 x;	
	u32 j, FlashAddr,Ln = 0;   //�˵��ӵ�0�п�ʼ
	RS485PortParaStruct RS485PortPara;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8_1);//��ʾ�˵�
			DisplayString(Ln+1,5,0,"RS485�˿� 2");			
		
			FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
			MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortParaStruct));	
			//��ʾ�ڴ��е�����
			//�˿ڹ���
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Function,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			DisplayData(p8[0],2,0,(u16*)p);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+2,5,0,"485������    ");			
					break;
				case 1:
					DisplayString(Ln+2,5,0,"485���߼���  ");			
					break;
				case 2:
					DisplayString(Ln+2,5,0,"485����      ");			
					break;
				case 3:
					DisplayString(Ln+2,5,0,"485�ϴ�ά����");		
					break;
				default:
					DisplayString(Ln+2,5,0,"485������    ");
					break;
			}
			//ͨ�Ų�����
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.BaudRate,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0x01:
					DisplayString(Ln+3,7,0,"300");				
					break;
				case 0x02:
					DisplayString(Ln+3,7,0,"600");				
					break;
				case 0x04:
					DisplayString(Ln+3,7,0,"1200");				
					break;
				case 0x08:
					DisplayString(Ln+3,7,0,"2400");				
					break;
				case 0x10:
					DisplayString(Ln+3,7,0,"4800");				
					break;
				case 0x20:
					DisplayString(Ln+3,7,0,"9600");
					break;
				case 0x40:
					DisplayString(Ln+3,7,0,"19200");
					break;
				case 0x80:
					DisplayString(Ln+3,7,0,"38400");
					break;
				case 0xC0:
					DisplayString(Ln+3,7,0,"57600");
					break;
				default:
					DisplayString(Ln+3,7,0,"1200");	
					break;	
			}							
			//У�鷽ʽ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Parity,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+4,9,0,"��У��");			
					break;
				case 1:
					DisplayString(Ln+4,9,0,"żУ��");		
					break;
				case 2:
					DisplayString(Ln+4,9,0,"��У��");			
					break;
				default:
					DisplayString(Ln+4,9,0,"żУ��");
					break;
			}				
			//����λ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.DataBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);	
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 5:
					DisplayString(Ln+5,7,0,"5 λ");			
					break;
				case 6:
					DisplayString(Ln+5,7,0,"6 λ");		
					break;
				case 7:
					DisplayString(Ln+5,7,0,"7 λ");			
					break;
				case 8:
					DisplayString(Ln+5,7,0,"8 λ");		
					break;
				default:
					DisplayString(Ln+5,7,0,"8 λ");
					break;
			}	
			//ֹͣλ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.StopBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+6,7,0," 1  λ");			
					break;
				case 1:
					DisplayString(Ln+6,7,0,"1.5 λ");		
					break;
				case 2:
					DisplayString(Ln+6,7,0," 2  λ");			
					break;
				default:
					DisplayString(Ln+6,7,0," 1  λ");
					break;
			}	
			pMenuCount->v3 = ConvertRs485PortParaToBitValue(RS485PortPara.Function, RS485PortPara.BaudRate, 
				               RS485PortPara.Parity, RS485PortPara.DataBit, RS485PortPara.StopBit);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)  //�༭��Ϊ0��ʱ��
				{
					BoxSelect();  //�༭��ѡ��				
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=17;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=7;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=7;
							MenuCount->v2=10;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=7;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// �༭�˿ڹ���
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485�ϴ�ά����");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485������    ");
								}
								break;
							case 3:		// �༭�˿ڲ�����
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								x++;
								if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// �༭�˿ڼ��鷽ʽ
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								x++;
								if(x > UART_PARITY_ODD) x = UART_PARITY_NONE;
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"��У��");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"żУ��");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"��У��");
								}
								break;
							case 5:		// �༭�˿�����λ
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								x++;
								if(x > UART_DATA_BIT_8) x = UART_DATA_BIT_5;
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 λ");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 λ");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 λ");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 λ");
								}
								break;
							case 6:		// �༭�˿�ֹͣλ
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								x++;
								if(x > UART_STOP_BIT_2) x = UART_STOP_BIT_1;
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  λ");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 λ");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  λ");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//���
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// �༭�˿ڹ���
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485�ϴ�ά����");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485������    ");
								}
								break;
							case 3:		// �༭�˿ڲ�����
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								if(x == 0)
								{
									x = BAUD_RATE_9600_ID;
								}
								else
								{
									x--;
									if(x == BAUD_RATE_7200_ID) x = BAUD_RATE_4800_ID;
								}							
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// �༭�˿�У�鷽ʽ
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								if(x == UART_PARITY_NONE)
								{
									x = UART_PARITY_ODD;
								}
								else
								{
									x--;
								}
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_NONE << 7);
									DisplayString(4,9,0,"��У��");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_EVEN << 7);
									DisplayString(4,9,0,"żУ��");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_ODD << 7);
									DisplayString(4,9,0,"��У��");
								}
								break;
							case 5:		// �༭�˿�����λ
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								if(x == UART_DATA_BIT_5) 
								{
									x = UART_DATA_BIT_8;
								}
								else
								{
									x--;
								}
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 λ");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 λ");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 λ");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 λ");
								}
								break;
							case 6:		// �༭�˿�ֹͣλ
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								if(x == UART_STOP_BIT_1)
								{
									x = UART_STOP_BIT_2;
								}
								else
								{
									x--;
								}
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  λ");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 λ");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  λ");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ�ϼ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:
								break;
							case 3:
								break;
							case 4:
								break;
							case 5:
								break;
							case 6:
								break;
							case 7:
								//�������õ�����					
								//�˿ڹ���
								FlashAddr = ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct);
								MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
								ConvertBitValueToRs485PortPara(&RS485PortPara.Function, &RS485PortPara.BaudRate, &RS485PortPara.Parity, 
									                           &RS485PortPara.DataBit, &RS485PortPara.StopBit, pMenuCount->v3);
								MW((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
										
								DisplayClrRow_4(Ln+7);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+7,9,0,"OK");																					
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8_3
// Descriptions:        485�˿�3
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_8_3(void)
{
	u8 *p,*p8;	
	u64 x;
	u32 j, FlashAddr,Ln = 0;   //�˵��ӵ�0�п�ʼ
	RS485PortParaStruct RS485PortPara;
	MenuCount_TypeDef* pMenuCount = MenuCount;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			MenuCount->v3=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8_1);//��ʾ�˵�
			DisplayString(Ln+1,5,0,"RS485�˿� 3");			
		
			FlashAddr = 2 * sizeof(RS485PortParaStruct);
			FlashAddr += ADDR_RS485PortParaTbl;
			MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortParaStruct));	
			//��ʾ�ڴ��е�����
			//�˿ڹ���
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Function,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			DisplayData(p8[0],2,0,(u16*)p);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+2,5,0,"485������    ");			
					break;
				case 1:
					DisplayString(Ln+2,5,0,"485���߼���  ");			
					break;
				case 2:
					DisplayString(Ln+2,5,0,"485����      ");			
					break;
				case 3:
					DisplayString(Ln+2,5,0,"485�ϴ�ά����");		
					break;
				default:
					DisplayString(Ln+2,5,0,"485������    ");
					break;
			}
			//ͨ�Ų�����
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.BaudRate,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0x01:
					DisplayString(Ln+3,7,0,"300");				
					break;
				case 0x02:
					DisplayString(Ln+3,7,0,"600");				
					break;
				case 0x04:
					DisplayString(Ln+3,7,0,"1200");				
					break;
				case 0x08:
					DisplayString(Ln+3,7,0,"2400");				
					break;
				case 0x10:
					DisplayString(Ln+3,7,0,"4800");				
					break;
				case 0x20:
					DisplayString(Ln+3,7,0,"9600");
					break;
				case 0x40:
					DisplayString(Ln+3,7,0,"19200");
					break;
				case 0x80:
					DisplayString(Ln+3,7,0,"38400");
					break;
				case 0xC0:
					DisplayString(Ln+3,7,0,"57600");
					break;
				default:
					DisplayString(Ln+3,7,0,"1200");	
					break;	
			}							
			//У�鷽ʽ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.Parity,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+4,9,0,"��У��");			
					break;
				case 1:
					DisplayString(Ln+4,9,0,"żУ��");		
					break;
				case 2:
					DisplayString(Ln+4,9,0,"��У��");			
					break;
				default:
					DisplayString(Ln+4,9,0,"żУ��");
					break;
			}				
			//����λ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.DataBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);	
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 5:
					DisplayString(Ln+5,7,0,"5 λ");			
					break;
				case 6:
					DisplayString(Ln+5,7,0,"6 λ");		
					break;
				case 7:
					DisplayString(Ln+5,7,0,"7 λ");			
					break;
				case 8:
					DisplayString(Ln+5,7,0,"8 λ");		
					break;
				default:
					DisplayString(Ln+5,7,0,"8 λ");
					break;
			}	
			//ֹͣλ
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,(u32)&RS485PortPara.StopBit,1);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
			bcd_p8(p,p8[0],2);
			switch(p8[0])
			{
				case 0:
					DisplayString(Ln+6,7,0," 1  λ");			
					break;
				case 1:
					DisplayString(Ln+6,7,0,"1.5 λ");		
					break;
				case 2:
					DisplayString(Ln+6,7,0," 2  λ");			
					break;
				default:
					DisplayString(Ln+6,7,0," 1  λ");
					break;
			}	
			pMenuCount->v3 = ConvertRs485PortParaToBitValue(RS485PortPara.Function, RS485PortPara.BaudRate, 
				               RS485PortPara.Parity, RS485PortPara.DataBit, RS485PortPara.StopBit);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)  //�༭��Ϊ0��ʱ��
				{
					BoxSelect();  //�༭��ѡ��				
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=17;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=7;
							MenuCount->v2=11;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=9;
							MenuCount->v2=14;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=7;
							MenuCount->v2=10;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=7;
							MenuCount->v2=12;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=0;
							MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);
							break;
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// �༭�˿ڹ���
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485�ϴ�ά����");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485������    ");
								}
								break;
							case 3:		// �༭�˿ڲ�����
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								x++;
								if(x > BAUD_RATE_9600_ID) x = BAUD_RATE_1200_ID;
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// �༭�˿ڼ��鷽ʽ
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								x++;
								if(x > UART_PARITY_ODD) x = UART_PARITY_NONE;
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"��У��");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"żУ��");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (x << 7);
									DisplayString(4,9,0,"��У��");
								}
								break;
							case 5:		// �༭�˿�����λ
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								x++;
								if(x > UART_DATA_BIT_8) x = UART_DATA_BIT_5;
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 λ");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 λ");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 λ");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 λ");
								}
								break;
							case 6:		// �༭�˿�ֹͣλ
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								x++;
								if(x > UART_STOP_BIT_2) x = UART_STOP_BIT_1;
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  λ");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 λ");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  λ");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//���
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:		// �༭�˿ڹ���
								x = pMenuCount->v3 & RS485_FUNCTION_BIT_MASK;
								if(x == UART_FUNC_AMR)
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_UPLINK;
									DisplayString(2,5,0,"485�ϴ�ά����");									
								}
								else
								{
									pMenuCount->v3 &= ~RS485_FUNCTION_BIT_MASK;
									pMenuCount->v3 |= UART_FUNC_AMR;
									DisplayString(2,5,0,"485������    ");
								}
								break;
							case 3:		// �༭�˿ڲ�����
								x = pMenuCount->v3 & RS485_BAUD_RATE_BIT_MASK;
								x >>= 3;
								if(x == 0)
								{
									x = BAUD_RATE_9600_ID;
								}
								else
								{
									x--;
									if(x == BAUD_RATE_7200_ID) x = BAUD_RATE_4800_ID;
								}							
								switch(x)
								{
									case BAUD_RATE_1200_ID:
										DisplayString(3,7,0,"1200 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_1200_ID << 3;
										break;
									case BAUD_RATE_2400_ID:
										DisplayString(3,7,0,"2400 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_2400_ID << 3;
										break;
									case BAUD_RATE_4800_ID:
										DisplayString(3,7,0,"4800 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_4800_ID << 3;
										break;
									case BAUD_RATE_9600_ID:
									default:
										DisplayString(3,7,0,"9600 ");
										pMenuCount->v3 &= ~RS485_BAUD_RATE_BIT_MASK;
										pMenuCount->v3 |= BAUD_RATE_9600_ID << 3;
										break;
								}
								break;
							case 4:		// �༭�˿ڼ��鷽ʽ
								x = pMenuCount->v3 & RS485_PARITY_BIT_MASK;
								x >>= 7;
								if(x == UART_PARITY_NONE)
								{
									x = UART_PARITY_ODD;
								}
								else
								{
									x--;
								}
								if(x == UART_PARITY_NONE)
								{
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_NONE << 7);
									DisplayString(4,9,0,"��У��");
																		
								}
								else if(x == UART_PARITY_EVEN)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_EVEN << 7);
									DisplayString(4,9,0,"żУ��");
								}
								else// if(x == UART_PARITY_ODD)
								{									
									pMenuCount->v3 &= ~RS485_PARITY_BIT_MASK;
									pMenuCount->v3 |= (UART_PARITY_ODD << 7);
									DisplayString(4,9,0,"��У��");
								}
								break;
							case 5:		// �༭�˿�����λ
								x = pMenuCount->v3 & RS485_DATA_BIT_MASK;
								x >>= 9;
								if(x == UART_DATA_BIT_5) 
								{
									x = UART_DATA_BIT_8;
								}
								else
								{
									x--;
								}
								if(x == UART_DATA_BIT_5)
								{
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_5 << 9);
									DisplayString(5,7,0,"5 λ");																	
								}
								else if(x == UART_DATA_BIT_6)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_6 << 9);
									DisplayString(5,7,0,"6 λ");
								}
								else if(x == UART_DATA_BIT_7)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_7 << 9);
									DisplayString(5,7,0,"7 λ");
								}
								else// if(x == UART_DATA_BIT_8)
								{									
									pMenuCount->v3 &= ~RS485_DATA_BIT_MASK;
									pMenuCount->v3 |= (UART_DATA_BIT_8 << 9);
									DisplayString(5,7,0,"8 λ");
								}
								break;
							case 6:		// �༭�˿�ֹͣλ
								x = pMenuCount->v3 & RS485_STOP_BIT_MASK;
								x >>= 13;
								if(x == UART_STOP_BIT_1)
								{
									x = UART_STOP_BIT_2;
								}
								else
								{
									x--;
								}
								if(x == UART_STOP_BIT_1)
								{
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1 << 13);
									DisplayString(6,7,0," 1  λ");																	
								}
								else if(x == UART_STOP_BIT_1_5)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_1_5 << 13);
									DisplayString(6,7,0,"1.5 λ");
								}
								else // if(x == UART_STOP_BIT_2)
								{									
									pMenuCount->v3 &= ~RS485_STOP_BIT_MASK;
									pMenuCount->v3 |= (UART_STOP_BIT_2 << 13);
									DisplayString(6,7,0," 2  λ");
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ�ϼ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 2:
								break;
							case 3:
								break;
							case 4:
								break;
							case 5:
								break;
							case 6:
								break;
							case 7:
								//�������õ�����					
								//�˿ڹ���
								FlashAddr = 2 * sizeof(RS485PortParaStruct);
								FlashAddr += ADDR_RS485PortParaTbl;
								MR((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
								ConvertBitValueToRs485PortPara(&RS485PortPara.Function, &RS485PortPara.BaudRate, &RS485PortPara.Parity, 
									                           &RS485PortPara.DataBit, &RS485PortPara.StopBit, pMenuCount->v3);
								MW((u32)&RS485PortPara,FlashAddr,sizeof(RS485PortPara));
										
								DisplayClrRow_4(Ln+7);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+7,9,0,"OK");																					
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_9
// Descriptions:        �����������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_9(void)
{
//	MenuSelect();//�˵�ѡ��
//	if((Comm_Ram->ButtonVal&0x10)!=0)
//	{//ȷ��          
//	}
	if((Comm_Ram->ButtonVal&0x2F)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2F;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_10
// Descriptions:        �ն˱��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_10(void)
{	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_10);//��ʾ�˵�
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://������������
						MenuCreate((u32)Menu_8_10_1);//�����¼��˵�           
						break;
					case 2://�ն˵�ַ����
						MenuCreate((u32)Menu_8_10_2);//�����¼��˵�
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_10_1
// Descriptions:        ������������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_10_1(void)
{	
	u8 *p8,*p;
	u16 *p16;
	u32 i,j,x;
	u32 *p32,*q32;
	u32 Ln=0;          //�˵��ӵ�0�п�ʼ	
	u32 m[6]={0};
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;


	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_10_1);//��ʾ�˵�
			
			//��ʾ�ڴ�������
			//����������			
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
			p32=(u32*)ADDR_DATABUFF;		
			MR(ADDR_DATABUFF,(u32)&CommPara.TerminalLocationCode,3);			
			q32=(u32*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);		
			DisplayData(p32[0]&0x00FFFFFF,6,0,(u16*)q32);
		
			MenuCount->v1=9;
			MenuCount->v2=14;
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
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:							
								//�������õ�����				
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								for(i=0;i<6;i++)
								{
									m[i]=(u32)CharToHex(p[2*i]);
								}
								m[0]=m[0]<<20;
								m[1]=m[1]<<16;
								m[2]=m[2]<<12;
								m[3]=m[3]<<8;
								m[4]=m[4]<<4;		
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								if(CommPara.TerminalLocationCode == (m[0]|m[1]|m[2]|m[3]|m[4]|m[5]))//������õ�����������ԭ����ͬ��������д���ڴ�
								{
									DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
									DisplayString(Ln+2,9,0,"OK");
								}
								else//������õ�����������ԭ����ͬ�����������ã����ն�ͬ����Ӳ����λ
								{
									CommPara.TerminalLocationCode = m[0]|m[1]|m[2]|m[3]|m[4]|m[5];
									MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								
									DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
									DisplayString(Ln+2,9,0,"OK");
									
									pTSysCtrl->TerminalLocation = CommPara.TerminalLocationCode;    //���ú����ն�ͬ������������
									#if(USER != 3400) //ybzyl20181031 �������ϰ漼��֧������������������������븺�ز�Ҫ����������������Ϊ�Ǵ���
										DataResetAction(0x02,GUI_PORT);//Ӳ����λ
									#endif
								}									
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF)||(x<1))
								{
									bcd_p8(p8,0,6);
								}
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
// Function name:       Menu_8_10_2
// Descriptions:        �ն˵�ַ���ã�ʮ�����ƣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_10_2(void)
{	
	u8 *p,*p8;
	u16 *p16;
	u32 *p32,*q32;
	u32 m[6]={0};
	u32 i,j,x,Ln=0;//�˵��ӵ�0�п�ʼ
	CommunicationParaStruct CommPara;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_10_2);//��ʾ�˵�
			
			//��ʾ�ڴ�������
			//�߼���ַ
			MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
			p32=(u32*)ADDR_DATABUFF;		
			MR(ADDR_DATABUFF,(u32)&CommPara.TerminalAddr,3);
			p32[0]=p32[0]&0x00FFFFFF;
			q32=(u32*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);					
			DisplayData(p32[0],6,0,(u16*)q32);
		
			MenuCount->v1=9;
			MenuCount->v2=14;
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
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//�������õ�����
								MR((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);						
								for(i=0;i<6;i++)
								{
									m[i]=(u32)CharToHex(p[2*i]);
								}
								m[0]=m[0]<<20;
								m[1]=m[1]<<16;
								m[2]=m[2]<<12;
								m[3]=m[3]<<8;
								m[4]=m[4]<<4;	
								if(CommPara.TerminalAddr == (m[0]|m[1]|m[2]|m[3]|m[4]|m[5]))//������õĵ�ַ��ԭ��ַ��ͬ��������д���ڴ�
								{
									DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
									DisplayString(Ln+2,9,0,"OK");
								}
								else //������õĵ�ַ��ԭ��ַ��ͬ�����������ã����ն�ͬ����ַ��Ӳ����λ
								{
									CommPara.TerminalAddr = m[0]|m[1]|m[2]|m[3]|m[4]|m[5];
									MW((u32)&CommPara,ADDR_CommunicationPara,sizeof(CommunicationParaStruct));								
									
									DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
									DisplayString(Ln+2,9,0,"OK");
									
									pTSysCtrl->TerminalAddr = CommPara.TerminalAddr;        //���ú����ն�ͬ���ն˵�ַ
									#if(USER != 3400) //ybzyl20181031 �������ϰ漼��֧������������������ն˵�ַ���ز�Ҫ������������Ϊ����������
										DataResetAction(0x02,GUI_PORT);//Ӳ����λ
									#endif
								}
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
					SelectH(MenuCount->v2,MenuCount->v1);
					UpDownSelectH();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);							
								x=p8_bcd(p8,6);
								if((x>0xFFFFFF)||(x<1))
								{
									bcd_p8(p8,0,6);
								}
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
// Function name:       Menu_8_11
// Descriptions:        �ն˰汾
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_11(void)
{
	u16 *p16;
	u32 Ln;
	Ln=0;//�˵��ӵ�0�п�ʼ
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_11);//��ʾ�˵�					
	//���̴���
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+11*2);
	p16[0]=0x2000+'C';
	p16[1]=0x2000+'H';
	p16[2]=0x2000+'N';
	p16[3]=0x2000+'T';	
	//�����汾
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+11*2);
	p16[0]=((DefaultVersionPara.SoftWareVersion>>8)&0xf)+0x2030;
	p16[1]='.'+0x2000;
	p16[2]=((DefaultVersionPara.SoftWareVersion>>4)&0xf)+0x2030;
	p16[3]=(DefaultVersionPara.SoftWareVersion&0xf)+0x2030;
	//��������
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+11*2);
	p16[0]=((DefaultVersionPara.Year>>4)&0xf)+0x2030;
	p16[1]=(DefaultVersionPara.Year&0xf)+0x2030;
	p16[2]=((DefaultVersionPara.Month>>4)&0xf)+0x2030;
	p16[3]=(DefaultVersionPara.Month&0xf)+0x2030;
	p16[4]=((DefaultVersionPara.Day>>4)&0xf)+0x2030;
	p16[5]=(DefaultVersionPara.Day&0xf)+0x2030;
	//Ӳ���汾
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
	p16[0]=((DefaultVersionPara.HardWareVersion>>8)&0xf)+0x2030;
	p16[1]='.'+0x2000;
	p16[2]=((DefaultVersionPara.HardWareVersion>>4)&0xf)+0x2030;
	p16[3]=(DefaultVersionPara.HardWareVersion&0xf)+0x2030;
	
	#if ((Project/100)==3) || ((Project/100)==9)
	//�����汾
	DisplayString(Ln+6,2,0,"�����汾:");
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
	p16[0]=0x2031;             //1
	p16[1]='.'+0x2000;         //.       
	p16[2]=0x2030;             //0
	p16[3]=0x2030;             //0
	#endif
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//���� 
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8
// Descriptions:        �ն˹�����ά��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8(void)//�ն˹�����ά��
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_8_1);        //1. �����ն�
						break;
					case 2:
						MenuCreate((u32)Menu_8_2);        //2. Һ������
						break;
					case 3:
						MenuCreate((u32)Menu_8_3);        //3. ״̬�鿴
						break;
					case 4:
						MenuCreate((u32)Menu_8_4);        //4. �ź�ǿ�Ⱥ͵�ص���
						break;
					case 5:
						MenuCreate((u32)Menu_8_5);        //5. ����ʵʱ״̬
						break;
					case 6:
						MenuCreate((u32)Menu_8_6);        //6.�ն�ʱ������
						break;
					case 7:
						MenuCreate((u32)Menu_8_7);        //7.������������
						break;
					case 8:
						MenuCreate((u32)Menu_8_8);        //8.����ͨ������
						break;
					case 9:
						MenuCreate((u32)Menu_8_9);        //9.�����������
						break;
					case 10:
						MenuCreate((u32)Menu_8_10);       //10.�ն˱��
						break;
					case 11:
						MenuCreate((u32)Menu_8_11);       //11.�ն˰汾
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
#endif


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8
// Descriptions:        �ն˹�����ά��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8(void)//�ն˹�����ά��
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)JZQpMenu_8);//��ʾ�˵�	
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_8_1);       //1. �����ն�
						break;
					case 2:
						MenuCreate((u32)Menu_8_2);       //2. Һ������
						break;
					case 3:
						MenuCreate((u32)Menu_8_4);       //3. �ź�ǿ�Ⱥ͵�ص���
						break;
					case 4:
						MenuCreate((u32)Menu_8_5);       //4. ����ʵʱ״̬
						break;
					case 5:
						MenuCreate((u32)Menu_8_6);       //5.�ն�ʱ������
						break;
					case 6:
						MenuCreate((u32)Menu_8_7);       //6.������������
						break;
					case 7:
						MenuCreate((u32)Menu_8_8);       //7.����ͨ������
						break;
					case 8:
						MenuCreate((u32)Menu_8_10);      //8.�ն˱��
						break;
					case 9:
						MenuCreate((u32)Menu_8_11);      //9.�ն˰汾
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
#endif