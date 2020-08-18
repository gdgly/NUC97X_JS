/*************************************************
��Ȩ:��̩�Ǳ������ն˲�
�ļ�:Menu_2_PowerCalculation.c
����:zyl
�汾:1.00
����:20170717
����:�������ز˵�2���ܼ��������ļ�
*************************************************/


//�˵�2 ���ܼ���
#include "../Hcsg/ProjectCSG.h"
#include "../Display/Display.h"
#include "../Display/DisplayLoop_ICO.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "Menu.h"
#include "Menu2.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../QCSG_Head/QCSG_Hal.h"





const u8 EQList[][20]=//�����б�
{
	"�й���         kWh",
	"�й���         kWh",
	"�й���         kWh",
	"�й�ƽ         kWh",
	"�й���         kWh",
	"�޹���         kvarh",
};


const u8 pMenu_2[]=
{
	"\n\r"
	"��ǰ��������\n\r"
	"��ʷ������\n\r"
	"��ʷ������\n\r"
};

const u8 pMenu_2_1[]=
{
	"\n\r"
	"������ţ�\n\r"
	"        ȷ��\n\r"
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_1
// Descriptions:        ��ǰ�������ݣ�ѡ������㣩
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1(void)//��ǰ�������ݣ�ѡ������㣩
{
	u8 *p,*p8;
	u16 *p16;
	u64 x;
	u32 j,Ln=0;//�˵��ӵ�0�п�ʼ	
	MeasurePointStruct  MeasurePoint;
	u32 FlashLenAddr = 0, FlashLen = 0;		
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1);//��ʾ�˵�
			
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
								MenuCreate((u32)Menu_2_1_1);
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
// Function name:       Menu_2_1_1
// Descriptions:        ��ǰ�����й�����ʾֵ������޹�1�ܵ���ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1_1(void)//��ǰ�����й�����ʾֵ������޹�1����ʾֵ
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																					"\n\r"
																					"  ��ǰ�������ʾֵ  "
																					"    ������          "
																					);//��ʾ�˵�
	
	if(Pn_Lcd_Data->u16Pn == 0xFFFF)
	{
		DisplayClrRow(1,6);//����Ļ��ʾ��ָ����
		DisplayString(Ln+4,4,0,"�����㲻����");
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
		
		DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
		for(i=0;i<=5;i++)
		{
			DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
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
		//�޹���
		MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0003FF00[0],4);
		if(Databuff==0xFFFFFFFF)
		{
			DisplayString(Ln+8,6,0,"------.--");
		}
		else
		{
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
			DisplayData(Databuff,8,2,q16);
		}
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_6);//����
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
// Function name:       Menu_2_1_2
// Descriptions:        ��ǰ�����й�����ʾֵ������޹�2�ܵ���ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1_2(void)//��ǰ�����й�����ʾֵ������޹�2����ʾֵ
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
		
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																					"\n\r"
																					"  ��ǰ�������ʾֵ  "
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

	DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
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
	//�޹���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data0004FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_3);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_1);//����
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
// Function name:       Menu_2_1_3
// Descriptions:        ���������й�������������޹�1�ܵ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1_3(void)//���������й�������������޹�1������
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"\n\r"
																				"   �������������   "
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
		
	DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[0],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[4],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[8],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[12],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0001FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�޹���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0003FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_4);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_2);//����
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
// Function name:       Menu_2_1_4
// Descriptions:        ���շ����й�������������޹�2�ܵ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1_4(void)//���շ����й�������������޹�2�ܵ�����
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"\n\r"
																				"   ���շ��������   "
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

	DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[0],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[4],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[8],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[12],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0002FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�޹���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrDay0004FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
		
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_5);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_3);//����
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
// Function name:       Menu_2_1_5
// Descriptions:        ���������й�������������޹�1�ܵ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1_5(void)//���������й�������������޹�1�ܵ�����
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"\n\r"
																				"   �������������   "
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
		
	DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[0],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[4],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[8],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[12],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0001FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�޹���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0003FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_6);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_4);//����
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
// Function name:       Menu_2_1_6
// Descriptions:        ���·����й�������������޹�2�ܵ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_1_6(void)//���·����й�������������޹�2�ܵ�����
{
	u16 *p16,*q16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																				"\n\r"
																				"   ���·��������   "
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

	DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
	//�й���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[0],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[4],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[8],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[12],4);
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
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0002FF00[16],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+7,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}
	//�޹���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8CurrMonth0004FF00[0],4);
	if(Databuff==0xFFFFFFFF)
	{
		DisplayString(Ln+8,6,0,"------.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+6*2);
		DisplayData(Databuff,8,2,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_1_5);//����
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
// Function name:       Menu_2_2
// Descriptions:        ��ʷ�����ݣ�ѡ������㣩
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_2(void)//��ʷ�����ݣ�ѡ������㣩
{
	u8 *p,*p8;
	u16 *p16;
	u32 j,Ln;
	u64 x;
	MeasurePointStruct  MeasurePoint;
	u32 FlashLenAddr = 0, FlashLen = 0;		
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;//�˵��ӵ�0�п�ʼ


	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1);//��ʾ�˵�
			
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
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;
									
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}
								MenuCreate((u32)Menu_2_2_1);
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
// Function name:       Menu_2_2_1
// Descriptions:        �ն��������й�����ʾֵ������޹�1�ܵ���ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_2_1(void)//�ն��������й�����ʾֵ������޹�1�ܵ���ʾֵ
{
	u16 *p16,*q16;
	u32 i,Ret;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	" ��ʷ��������ʾ��"
																	"     ������   \n\r"
																	);//��ʾ�˵�

		
	if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,6);//����Ļ��ʾ��ָ����
		DisplayString(Ln+4,4,0,"�����㲻����");
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
		
		DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
		for(i=0;i<=5;i++)
		{
			DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
		}
		
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
			//Ret=QCSG_DecOneDay(&RealTime);
			//if(Ret==TRUE)
			//{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050601FF, &DataTime, PnData, &DataLen);
				//�й���
				MR((u32)&Databuff,(u32)&PnData[1],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+3,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//�й���
				MR((u32)&Databuff,(u32)&PnData[5],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+4,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//�й���
				MR((u32)&Databuff,(u32)&PnData[9],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+5,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//�й�ƽ
				MR((u32)&Databuff,(u32)&PnData[13],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+6,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//�й���
				MR((u32)&Databuff,(u32)&PnData[17],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+7,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050603FF, &DataTime, PnData, &DataLen);
				//�޹���
				MR((u32)&Databuff,(u32)&PnData[1],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+8,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+8)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}				
			//}
		}
	}		
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_2_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_2_2);//����
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
// Function name:       Menu_2_2_2
// Descriptions:        �ն��ᷴ���й�����ʾֵ������޹�2�ܵ���ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_2_2(void)//�ն��ᷴ���й�����ʾֵ������޹�2�ܵ���ʾֵ
{
	u16 *p16,*q16;
	u32 i,Ret;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	" ��ʷ�շ�����ʾ��"
																	"     ������    \n\r"
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
		
	DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
		
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
		//Ret=QCSG_DecOneDay(&RealTime);
		//if(Ret==TRUE)
		//{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050602FF, &DataTime, PnData, &DataLen);
			//�й���
			MR((u32)&Databuff,(u32)&PnData[1],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+3,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//�й���
			MR((u32)&Databuff,(u32)&PnData[5],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+4,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//�й���
			MR((u32)&Databuff,(u32)&PnData[9],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+5,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//�й�ƽ
			MR((u32)&Databuff,(u32)&PnData[13],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+6,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//�й���
			MR((u32)&Databuff,(u32)&PnData[17],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+7,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			NAND_ReadMeterAMRData(AMR_DailyFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x050604FF, &DataTime, PnData, &DataLen);
			//�޹���
			MR((u32)&Databuff,(u32)&PnData[1],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+8,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+8)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}				
		//}
	}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_2_1);//����
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
// Function name:       Menu_2_3
// Descriptions:        ��ʷ�����ݣ�ѡ������㣩
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_3(void)//��ʷ�����ݣ�ѡ������㣩
{
	u8 *p,*p8;
	u16 *p16;
	u32 j,Ln;
	u64 x;
	MeasurePointStruct  MeasurePoint;
	u32 FlashLenAddr = 0, FlashLen = 0;		
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	Ln=0;//�˵��ӵ�0�п�ʼ


	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2_1);//��ʾ�˵�
			
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
									Pn_Lcd_Data->u16Pn = x;
									Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;
									
								}
								else
								{
									Pn_Lcd_Data->u16Pn = 0xFFFF;
								}
								MenuCreate((u32)Menu_2_3_1);
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
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_2_3_1
// Descriptions:        �¶��������й�����ʾֵ������޹�1�ܵ���ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_3_1(void)//�¶��������й�����ʾֵ������޹�1�ܵ���ʾֵ
{
	u16 *p16,*q16;
	u32 i,Ret;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	" ��ʷ��������ʾ��"
																	"     ������   \n\r"
																	);//��ʾ�˵�
		
	if(Pn_Lcd_Data->u16Pn == 0xFFFF)
  {
		DisplayClrRow(1,6);//����Ļ��ʾ��ָ����
		DisplayString(Ln+4,4,0,"�����㲻����");
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
		
		DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
		for(i=0;i<=5;i++)
		{
			DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
		}
		
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
			//Ret=QCSG_DecOneMonth(&RealTime);
			//if(Ret==TRUE)
			//{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x0001FF01, &DataTime, PnData, &DataLen);
				//�й���
				MR((u32)&Databuff,(u32)&PnData[1],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+3,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//�й���
				MR((u32)&Databuff,(u32)&PnData[5],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+4,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//�й���
				MR((u32)&Databuff,(u32)&PnData[9],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+5,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//�й�ƽ
				MR((u32)&Databuff,(u32)&PnData[13],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+6,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				//�й���
				MR((u32)&Databuff,(u32)&PnData[17],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+7,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}
				NAND_ReadMeterAMRData(AMR_MonthFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x0003FF01, &DataTime, PnData, &DataLen);
				//�޹���
				MR((u32)&Databuff,(u32)&PnData[1],4);
				if(Databuff==0xFFFFFFFF)
				{
					DisplayString(Ln+8,6,0,"------.--");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+8)*84+6*2);
					DisplayData(Databuff,8,2,q16);
				}				
			//}
		}
	}		
		
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_3_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_3_2);//����
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
// Function name:       Menu_2_3_2
// Descriptions:        �¶��ᷴ���й�����ʾֵ������޹�2�ܵ���ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2_3_2(void)//�¶��ᷴ���й�����ʾֵ������޹�2�ܵ���ʾֵ
{
	u16 *p16,*q16;
	u32 i,Ret;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	" ��ʷ�·�����ʾ��"
																	"     ������    \n\r"
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
		
	DisplayClrRow(Ln+3,Ln+8);//����Ļ��ʾ��ָ����		
	for(i=0;i<=5;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)EQList[i]);
	}
		
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
		//Ret=QCSG_DecOneMonth(&RealTime);
		//if(Ret==TRUE)
		//{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x0002FF01, &DataTime, PnData, &DataLen);
			//�й���
			MR((u32)&Databuff,(u32)&PnData[1],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+3,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//�й���
			MR((u32)&Databuff,(u32)&PnData[5],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+4,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//�й���
			MR((u32)&Databuff,(u32)&PnData[9],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+5,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//�й�ƽ
			MR((u32)&Databuff,(u32)&PnData[13],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+6,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			//�й���
			MR((u32)&Databuff,(u32)&PnData[17],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+7,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}
			NAND_ReadMeterAMRData(AMR_MonthFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0x0004FF01, &DataTime, PnData, &DataLen);
			//�޹���
			MR((u32)&Databuff,(u32)&PnData[1],4);
			if(Databuff==0xFFFFFFFF)
			{
				DisplayString(Ln+8,6,0,"------.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+8)*84+6*2);
				DisplayData(Databuff,8,2,q16);
			}				
		//}
	}
			
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_3_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_2_3_1);//����
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
// Function name:       Menu_2
// Descriptions:        ���ܼ���
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_2(void)
{
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_2);//��ʾ�˵�
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
							MenuCreate((u32)Menu_2_1);
							break;
						case 2:
							MenuCreate((u32)Menu_2_2);
							break;
						case 3:
							MenuCreate((u32)Menu_2_3);
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
