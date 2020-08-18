/*************************************************
��Ȩ:��̩�Ǳ������ն˲�
�ļ�:Menu_1_RealTimeData.c
����:zyl
�汾:1.00
����:20170717
����:�������ز˵�1ʵʱ���ݴ����ļ�
*************************************************/


//�˵�1 ʵʱ����
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu1.h"
#include "Menu3.h"
#include "Menu6.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../QCSG_Head/QCSG_Hal.h"





const u8 pMenu_1[]=
{
	"\n\r"
	"������ţ�\n\r"
	"        ȷ��\n\r"
};


#if ((Project/100)==2)
const u8 JZQpMenu_1[]=
{
	"\n\r"
	"ʵʱ������ʾ\n\r"
	"��ʷ������ʾ\n\r"
	"����ͳ������\n\r"
	"�쳣�澯\n\r"
};
#endif



const u8 pMenu_1_1_5[][20]=//�����б�
{
	"�й���         kWh",
	"�й���         kWh",
	"�й���         kWh",
	"�й�ƽ         kWh",
	"�й���         kWh",
};


const u8 pMenu_1_2_1[]=
{
	"\n\r"
	"ѡ������:\n\r"
	"        ȷ��\n\r"
};


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1
// Descriptions:        ����������ʵʱ����ѡ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1(void)//����������ʵʱ����ѡ�������
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j = 0,Ln;	
	u32 FlashLenAddr = 0, FlashLen = 0;	
	MeasurePointStruct  MeasurePoint;	
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;		//�˵��ӵ�0�п�ʼ
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1);//��ʾ�˵�
			
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
									MC(0,(u32)Pn_Lcd_Data,sizeof(QCSG_LCD_RAMDATA_S));
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;									
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}								
								MenuCreate((u32)Menu_1_1_1);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_1
// Descriptions:        �����㵱ǰ��ѹ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_1(void)//�����㵱ǰ��ѹ����
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	

	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"\n\r"
																	"      ������   \n\r"
																	"��ѹ(V):       \n\r"	
																	"  Ua/Uab =     \n\r"
																	"  Ub/Uac =     \n\r"
																	"  Uc/Ubc =     \n\r"
																	);//��ʾ�˵�
  if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,6);//����Ļ��ʾ��ָ����
		DisplayString(Ln+4,4,0,"�����㲻����");
//		if((Comm_Ram->ButtonVal&0x10)!=0)
//		{//ȷ�ϼ������·���
//			MenuDel();//�����ϼ��˵�
//		}
//		Comm_Ram->ButtonVal&=~0x10;//��ʹ�ú���0
	}
	else
	{
		if(Pn_Lcd_Data->u16Pn<0x0A)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
		}
		else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
		}
		else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
		}
		else if(Pn_Lcd_Data->u16Pn>=0x3E8)
		{
			p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
			bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
		}
		//A���ѹ
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0201FF00[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,11,0,"---.-");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+11*2);
			DisplayData(Databuff,4,1,q16);
		}
		//B���ѹ
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0201FF00[2],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"---.-");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
			DisplayData(Databuff,4,1,q16);
		}
		//C���ѹ
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0201FF00[4],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+6,11,0,"---.-");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
			DisplayData(Databuff,4,1,q16);
		}
		
		if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
		{//����
			CreateScreen((u32)Menu_1_1_2);//����
		}
		if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
		{//����
			CreateScreen((u32)Menu_1_1_6);//����
		}
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_2
// Descriptions:        �����㵱ǰ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_2(void)
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"\n\r"
																	"      ������    \n\r"
																	"����(A):        \n\r"
																	"  Ia =          \n\r"
																	"  Ib =          \n\r"
																	"  Ic =          \n\r"
																	);//��ʾ�˵�	
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	//A�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0202FF00[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"---.---");
	}
	else
	{
		if((Databuff&0x800000) != 0)
		{
			Databuff &= 0x7FFFFF;
			DisplayString(Ln+4,7,0,"-");
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);
			DisplayData(Databuff,5,3,q16);
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
			DisplayData(Databuff,6,3,q16);
		}
	}
	//B�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0202FF00[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,7,0,"---.---");
	}
	else
	{
		if((Databuff&0x800000) != 0)
		{
			Databuff &= 0x7FFFFF;
			DisplayString(Ln+5,7,0,"-");
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+8*2);
			DisplayData(Databuff,5,3,q16);
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);
			DisplayData(Databuff,6,3,q16);
		}
		
//		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);
//		DisplayData(Databuff,6,3,q16);
	}
	//C�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0202FF00[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,7,0,"---.---");
	}
	else
	{
		if((Databuff&0x800000) != 0)
		{
			Databuff &= 0x7FFFFF;
			DisplayString(Ln+6,7,0,"-");
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+8*2);
			DisplayData(Databuff,5,3,q16);
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
			DisplayData(Databuff,6,3,q16);
		}
//		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
//		DisplayData(Databuff,6,3,q16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_3);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_3
// Descriptions:        �����㵱ǰ�й����ʡ������й���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_3(void)//�����㵱ǰ�й����ʡ������й���������
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"      ������    \n\r"
																	"���й�����(kW):\n\r"
																	"  P�� =        \n\r"
																	"�����й�����(kW):\n\r"
																	"  Pa =         \n\r"
																	"  Pb =         \n\r"
																	"  Pc =         \n\r"
																	);//��ʾ�˵�
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
	//���й�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0203FF00[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,8,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+8*2);
		DisplayData(Databuff,6,4,q16);
	}
	//A���й�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0203FF00[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	//B���й�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0203FF00[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	//C���й�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0203FF00[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_4);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_2);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_4
// Descriptions:        �������ز����㵱ǰ�޹����ʡ������޹���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_4(void)//�������ز����㵱ǰ�޹����ʡ������޹���������
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"      ������    \n\r"
																	"���޹�����(kvar):\n\r"
																	"  Q�� =         \n\r"
																	"�����޹�����(kvar):\n\r"
																	"  Qa =          \n\r"
																	"  Qb =          \n\r"
																	"  Qc =          \n\r"
																	);//��ʾ�˵�	
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
	//���޹�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0204FF00[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,8,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+8*2);
		DisplayData(Databuff,6,4,q16);
	}
	//A���޹�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0204FF00[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	//B���޹�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0204FF00[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	//C���޹�����
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0204FF00[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,7,0,"--.----");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+7*2);
		DisplayData(Databuff,6,4,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_5);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_3);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}
#endif


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_4
// Descriptions:        ���������������㵱ǰ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_4(void)//���������������㵱ǰ��������
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"      ������    \n\r"
																	"�ܹ�������:     \n\r"
																	"  COS���� =      \n\r"
																	"���๦������:\n\r"
																	"  COS��a =       \n\r"
																	"  COS��b =       \n\r"
																	"  COS��c =       \n\r"
																	);//��ʾ�˵�
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
	
	//�ܹ�������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+3,12,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+12*2);
		DisplayData(Databuff,4,3,q16);
	}
	//A�๦������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[2],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,11,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,4,3,q16);
	}
	//B�๦������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[4],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,11,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,4,3,q16);
	}
	//C�๦������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[6],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,11,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,4,3,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_5);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_3);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	
#endif


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_5
// Descriptions:        �������ز����㵱ǰ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_5(void)//�������ز����㵱ǰ�������
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"      ������    \n\r"
																	"��ѹ������λ��(��):\n\r"
																	"  Uab/Ua =      \n\r"
																	"  Ub =          \n\r"
																	"  Uc =          \n\r"
																	"  Ia =          \n\r"
																	"  Ib =          \n\r"
																	"  Ic =          \n\r"
																	);//��ʾ�˵�
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
	
	//Uab/Ua
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+3,11,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+11*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Ub
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[2],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+4,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Uc
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[4],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Ia
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[6],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Ib
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[8],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	//Ic
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0207FF00[10],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+8,8,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+8*2);
		DisplayData(Databuff,4,1,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_6);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_4);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	
#endif


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_5
// Descriptions:        ���������������������й��ܵ���ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_5(void)//���������������������й��ܵ���ʾֵ
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"\n\r"
																			"  �����й�����ʾֵ  "
																			"    ������          "																			
																			);//��ʾ�˵�
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	
	DisplayClrRow(Ln+3,Ln+7);//����Ļ��ʾ��ָ����		
	for(i=0;i<=4;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_1_1_5[i]);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+3,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[4],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+4,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[8],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+5,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�й�ƽ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[12],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+6,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0001FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_6);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_4);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	
#endif


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_6
// Descriptions:        �������ز����㵱ǰ����������Ƶ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_6(void)//�������ز����㵱ǰ����������Ƶ������
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"\n\r"
																	"      ������    \n\r"
																	"��������:    \n\r"
																	"  PF =        \n\r"
																	"Ƶ��(HZ):    \n\r"
																	"  f =         \n\r"
																	);//��ʾ�˵�
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	
	//�ܹ�������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0206FF00[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+4,7,0,"-.---");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,4,3,q16);
	}
	//Ƶ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data02800002[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,6,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,4,2,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_5);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
		Pn_Lcd_Data->u16Pn = 0;
		Pn_Lcd_Data->u16Res1 = 0;
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	
#endif


#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_6
// Descriptions:        ���������������㷴���й�����ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_6(void)//���������������㷴���й�����ʾֵ
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"\n\r"
																			"  �����й�����ʾֵ  "
																			"    ������          "																			
																			);//��ʾ�˵�
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	
	DisplayClrRow(Ln+3,Ln+7);//����Ļ��ʾ��ָ����		
	for(i=0;i<=4;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_1_1_5[i]);
	}
	
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+3,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[4],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+4,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[8],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+5,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�й�ƽ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[12],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+6,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0002FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_1_1_5);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	
#endif


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2
// Descriptions:        ��ʷ������ʾ��ѡ������㣩
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_2(void)
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j = 0,Ln=0;	   //�˵��ӵ�0�п�ʼ
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1);//��ʾ�˵�
			
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
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;									
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}								
								MenuCreate((u32)Menu_1_2_1);
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
// Function name:       Menu_1_2_1
// Descriptions:        ��ʷ������ʾ(ѡ������)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_2_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;//�˵��ӵ�0�п�ʼ
	
	
	MenuCount->v1=0;
	MenuCount->v2=0;
	if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,6);//����Ļ��ʾ��ָ����
		DisplayString(Ln+4,4,0,"�����㲻����");
	}
	else
	{
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				DisplayClr();//��ȫ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_1);//��ʾ�˵�
				
				//����
				p8=(u8 *)ADDR_DATABUFF;
				MC(0,ADDR_DATABUFF,3);
				p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
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
				
				MenuCount->v1=9;
				MenuCount->v2=16;
				NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
				break;
			case 2:
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//���� 
					Stack->MenuStack[MenuCount->Count].Task = 0;
					Stack->MenuStack[MenuCount->Count].MoveRow = 0;
					Stack->MenuStack[MenuCount->Count].EditRow = 1;
					Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
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
								MenuCount->v2=16;
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
									if(p8[7*2]>=0x30 && p8[7*2]<0x39)
									{
										p8[7*2]++;
									}
									else if(p8[7*2]==0x39)
									{
										if(p8[6*2]>=0x30 && p8[6*2]<0x39)
										{
											p8[7*2]=0x30;
											p8[6*2]++;
										}
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
										if((p8[6*2]-0x30)*10+(p8[6*2]-0x30)*1>30)
										{//30��
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
									}
									if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
									{
										if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>29)
										{//29��
											p8[7*2]=0x31;
											p8[6*2]=0x30;
										}
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
									if(p8[6*2]>0x30 && p8[6*2]<=0x39)
									{
										if(p8[7*2]>0x30 && p8[7*2]<=0x39)
										{
											p8[7*2]--;
										}
										else if(p8[7*2]==0x30)
										{
											p8[7*2]=0x39;
											p8[6*2]--;
										}
									}
									else if(p8[6*2]==0x30)
									{
										if(p8[7*2]>0x31 && p8[7*2]<=0x39)
										{
											p8[7*2]--;
										}
										else if(p8[7*2]==0x31)
										{
											if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==1) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==3) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==5) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==7) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==8) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==10) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==12))
											{
												p8[7*2]=0x31;
												p8[6*2]=0x33;
											}
											if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==4) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==6) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==9) || ((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==11))
											{
												p8[7*2]=0x30;
												p8[6*2]=0x33;
											}
											if(((p8[3*2]-0x30)*10+(p8[4*2]-0x30)*1==2))
											{
												p8[7*2]=0x39;
												p8[6*2]=0x32;
											}
										}
									}
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
									break;
								case 2:
									//�������õ�����
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//��
									p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//��
									p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//��				
									
									RealTime.u8Day=p8[0];
									RealTime.u8Month=p8[1];
									RealTime.u8Year=p8[2];
									RealTime.u8Second=0;
									RealTime.u8Minute=0;
									RealTime.u8Hour=0;
									Menu_1_2_1_1(&RealTime);
									Stack->MenuStack[MenuCount->Count].Task = 2;
									Stack->MenuStack[MenuCount->Count].EditRow = 1;
									Stack->MenuStack[MenuCount->Count].MoveRow = 0;
									Stack->MenuStack[MenuCount->Count].EditCol = 0;
									MenuCount->v1=9;
									MenuCount->v2=16;
									NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							if(Stack->MenuStack[MenuCount->Count].EditRow<2)
							{
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
							}
							switch(Stack->MenuStack[MenuCount->Count].EditRow)
							{
								case 1:
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
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
								default:
									break;
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
				Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
				break;
			default:
				break;
		}
		if((Comm_Ram->ButtonVal&0x20)!=0)
		{//����
			MenuDel();//�����ϼ��˵�
		}
		Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0	  
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_1_1
// Descriptions:        ��ʷ�������й�����ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_2_1_1(RealTimeStruct *Time)
{
	u16 *p16,*q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayClr();//��ȫ��
	DisplayString(2,6,0,"������");
	DisplayString(3,3,0,"�����й��ܵ���");
	DisplayString(4,1,0,"�й���         kWh");
	DisplayString(5,3,0,"�����й��ܵ���");
	DisplayString(6,1,0,"�й���         kWh");
	
	if(Pn_Lcd_Data->u16Pn<0x0A)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		DisplayData_OffZero(Pn_Lcd_Data->u16Pn,4,0,p16);		
	}
	else if((Pn_Lcd_Data->u16Pn>=0xA)&&(Pn_Lcd_Data->u16Pn<0x64))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,2);
	}
	else if((Pn_Lcd_Data->u16Pn>=0x64)&&(Pn_Lcd_Data->u16Pn<0x3E8))
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,3);
	}
	else if(Pn_Lcd_Data->u16Pn>=0x3E8)
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+12*2);
		bcd_p8((u8*)p16,Pn_Lcd_Data->u16Pn,4);
	}
	
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(Time, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050601FF, &DataTime, PnData, &DataLen);
		//�����й���
		MR((u32)&Databuff,(u32)&PnData[1],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+4,7,0,"------.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
			DisplayData(Databuff,8,2,q16);
		}
		
		//�����й���
		NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050602FF, &DataTime, PnData, &DataLen);		
		MR((u32)&Databuff,(u32)&PnData[1],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+6,7,0,"------.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+7*2);
			DisplayData(Databuff,8,2,q16);
		}
	//}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_3
// Descriptions:        ����ͳ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_3(void)
{
	u8 AmrInfo[8];
	u16 *p;
	u32 x = 0;
	
	DisplayClr();//��ȫ��
	Get_MeterAMRSuccessInfo(AmrInfo);
	DisplayString(1,4,0,"����ͳ������");
	
	DisplayString(2,0,0,"�������й��ɹ�:");
	p=(u16*)(ADDR_STRINGBUFF+2*84+4+15*2);
	
	x = AmrInfo[0];
	x += AmrInfo[1] << 8;
	bcd_p8((u8*)p,x,4);
	
	DisplayString(3,0,0,"�������й�ʧ��:");
	p=(u16*)(ADDR_STRINGBUFF+3*84+4+15*2);
	x = AmrInfo[2];
	x += AmrInfo[3] << 8;
	bcd_p8((u8*)p,x,4);
	
	DisplayString(4,0,0,"�������й��ɹ�:");
	p=(u16*)(ADDR_STRINGBUFF+4*84+4+15*2);
	x = AmrInfo[4];
	x += AmrInfo[5] << 8;
	bcd_p8((u8*)p,x,4);
	
	DisplayString(5,0,0,"�������й�ʧ��:");
	p=(u16*)(ADDR_STRINGBUFF+5*84+4+15*2);
	x = AmrInfo[6];
	x += AmrInfo[7] << 8;
	bcd_p8((u8*)p,x,4);

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0	
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4
// Descriptions:        �쳣�澯
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_4(void)
{
	Menu_6();
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}


#if ((Project/100)==3) || ((Project/100)==9)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1
// Descriptions:        ��������ʵʱ����ѡ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1(void)//��������ʵʱ����ѡ�������
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j = 0,Ln;	
	u32 FlashLenAddr = 0, FlashLen = 0;
	MeasurePointStruct  MeasurePoint;	
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;		//�˵��ӵ�0�п�ʼ

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1);//��ʾ�˵�
			
		  //Ĭ����ʾ�������Ϊ0
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			bcd_p8(p,0,4);		
			MenuCount->v1=9;
			MenuCount->v2=12;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
									MC(0,(u32)Pn_Lcd_Data,sizeof(QCSG_LCD_RAMDATA_S));
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;									
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}								
								MenuCreate((u32)Menu_1_1_1);
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
#endif



#if ((Project/100)==2)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1
// Descriptions:        �������������ݲ�ѯ���˵�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1(void)//�������������ݲ�ѯ���˵�
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)JZQpMenu_1);//��ʾ�˵�
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
						MenuCreate((u32)Menu_1_1);//ʵʱ������ʾ
						break;
					case 2:
						MenuCreate((u32)Menu_1_2);//��ʷ������ʾ							
						break;
					case 3:							
						MenuCreate((u32)Menu_1_3);//����ͳ������							
						break;
					case 4:
						MenuCreate((u32)Menu_1_4);//�쳣�澯
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
