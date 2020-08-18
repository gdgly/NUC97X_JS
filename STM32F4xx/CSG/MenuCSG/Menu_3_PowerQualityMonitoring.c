/*************************************************
��Ȩ:��̩�Ǳ������ն˲�
�ļ�:Menu_3_PowerQualityMonitoring.c
����:zyl
�汾:1.00
����:20170717
����:�������ز˵�3����������⴦���ļ�
*************************************************/


//�˵�3 �����������
#include "../Hcsg/ProjectCSG.h"
#include "../Display/Display.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "Menu.h"
#include "Menu3.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"
#include "../QCSG_Head/QCSG_Hal.h"


const u8 pMenu_3[]=//�����ն˵����������
{
	"\n\r"
	"г������\n\r"
	"��ѹ�������\n\r"
	"��ѹ��ƽ���Խ������\n\r"
	"������ƽ���Խ������\n\r"
	"������������ͳ������\n\r"
};


//const u8 pMenu_3_1[]=//г������
//{
//	"\n\r"
//	"\n\r"
//	"A���ѹ����:\n\r"
//	"B���ѹ����:\n\r"
//	"C���ѹ����:\n\r"
//	"A���������:\n\r"
//	"B���������:\n\r"
//	"C���������:\n\r"
//};


const u8 pMenu_3_1[]=//г������
{
	"\n\r"
	"ʵʱ����\n\r"
	"��������\n\r"
	"��������\n\r"
	"��ʷ������\n\r"
	"��ʷ������\n\r"
};

const u8 pMenu_3_1_1[]=//г��ʵʱ����
{
	"\n\r"
	"��ѹг��������\n\r"
	"����г��������\n\r"
};

const u8 pMenu_3_1_4[]=//г������
{
	"\n\r"
	"A���ѹ�ܻ�������\n\r"
	"B���ѹ�ܻ�������\n\r"
	"C���ѹ�ܻ�������\n\r"
};

const u8 pMenu_3_1_4_1[]=
{
	"\n\r"
	"ѡ������:\n\r"
	"        ȷ��\n\r"
};


const u8 pMenu_3_1_4_1_2[]=
{
	"\n\r"
	"���ʱ��:    ��\n\r"
	"�ϸ���:       %\n\r"
	"��������:       %\n\r"
	"����ʱ��:    ��\n\r"
	"����ܻ���:     %\n\r"
	"����ʱ��:  -  -  -  \n\r"
	"����ܻ���:     %\n\r"
	"����ʱ��:  -  -  -  \n\r"
};


const u8 pMenu_3_1_4_1_3[]=
{
	"\n\r"
	"���ʱ��:      ��\n\r"
	"�ϸ���:       %\n\r"
	"��������:       %\n\r"
	"����ʱ��:      ��\n\r"
	"����ܻ���:     %\n\r"
	"����ʱ��:  -  -  -  \n\r"
	"����ܻ���:     %\n\r"
	"����ʱ��:  -  -  -  \n\r"
};

const u8 pMenu_3_2[]=//
{
	"\n\r"
	"��������\n\r"
	"��������\n\r"
};





#if (DLT500_REPORT == 1)
	const u8 pMenu_3_2_1[]=//��ѹ�ϸ���ͳ������
	{
		"\n\r"
		"A���ѹ�ϸ�������\n\r"
		"B���ѹ�ϸ�������\n\r"
		"C���ѹ�ϸ�������\n\r"
	};


	const u8 pMenu_3_2_1_1[]=//
	{
		"\n\r"
		"\n\r"
		"���ʱ��:      ��\n\r"
		"�ϸ���:       %\n\r"
		"��������:       %\n\r"
		"��������:       %\n\r"
		"�ϸ�ʱ��:      ��\n\r"
		"������ʱ��:      ��\n\r"
		"������ʱ��:      ��\n\r"
	};

#else
	const u8 pMenu_3_2_1[]=//��ѹ�ϸ���ͳ������
	{
		"\n\r"
		"�ܵ�ѹ�ϸ�������\n\r"
		"A���ѹ�ϸ�������\n\r"
		"B���ѹ�ϸ�������\n\r"
		"C���ѹ�ϸ�������\n\r"
	};



	const u8 pMenu_3_2_1_1[]=//
	{
		"\n\r"
		"\n\r"
		"���ʱ��:      ��\n\r"
		"�ϸ���:       %\n\r"
		"������:       %\n\r"
		"������ʱ��:      ��\n\r"
		"������ʱ��:      ��\n\r"
	};
#endif

	
const u8 pMenu_3_2_1_1_1[]=//
{
	"\n\r"
	"\n\r"
	"��ߵ�ѹ:     V\n\r"
	"��ߵ�ѹ����ʱ��:\n\r"
	"  ��  ��  ʱ  ��\n\r"
	"��͵�ѹ:     V\n\r"
	"��͵�ѹ����ʱ��:\n\r"
	"  ��  ��  ʱ  ��\n\r"
};


const u8 pMenu_3_3[]=//
{
	"\n\r"
	"��ʷ������\n\r"
	"��ʷ������\n\r"
};



const u8 pMenu_3_3_1[]=//��ѹ��ƽ���Խ��ͳ������1
{
	"\n\r"
	"\n\r"
	"���ֵ:     %\n\r"
	"��Сֵ:     %\n\r"
	"ƽ��ֵ:     %\n\r"
	"95%ֵ:     %\n\r"
};

const u8 pMenu_3_3_2[]=//��ѹ��ƽ���Խ��ͳ������2
{
	"\n\r"
	"\n\r"
	"��ֵ:     %\n\r"
	"��ֵ����ʱ��:\n\r"	
	"  ��  ��  ʱ  ��\n\r"
	"��ֵ�ۼ�ʱ��:\n\r"
	"����:        \n\r"
};


const u8 pMenu_3_5[]=//������������ͳ������
{
	"\n\r"
	"��������\n\r"
	"��������\n\r"
	"��ʷ������\n\r"
	"��ʷ������\n\r"
};



const u8 pMenu_3_5_1[]=//������������ͳ������1
{
	"\n\r"
	"�����������ʱ��:\n\r"
	"      ��\n\r"
	"����1ʱ��:      ��\n\r"
	"����2ʱ��:      ��\n\r"
	"����3ʱ��:      ��\n\r"
	"����4ʱ��:      ��\n\r"
};


const u8 pMenu_3_5_2[]=//������������ͳ������2
{
	"\n\r"
	"����5ʱ��:      ��\n\r"
	"����6ʱ��:      ��\n\r"
	"����7ʱ��:      ��\n\r"
	"����8ʱ��:      ��\n\r"
	"����9ʱ��:      ��\n\r"
	"����10ʱ��:      ��\n\r"
};



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1
// Descriptions:        г������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1(void)//г������
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_1_1);//ʵʱ����
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_2);//��������
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_3);//��������
						break;
					case 4:
						MenuCreate((u32)Menu_3_1_4);//��ʷ������
						break;
					case 5:
						MenuCreate((u32)Menu_3_1_5);//��ʷ������
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
// Function name:       Menu_3_1_1
// Descriptions:        г��ʵʱ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_1(void)//г��ʵʱ����
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_1);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_1_1_1);//��ѹг��������
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_1_2);//����г��������
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
// Function name:       Menu_3_1_1_1
// Descriptions:        ��ѹг��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_1_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"A���ѹ��г��������:\n\r"
																	"     %\n\r"	
																	"B���ѹ��г��������:\n\r"
																	"     %\n\r"
																	"C���ѹ��г��������:\n\r"
																	"     %\n\r"
																	);//��ʾ�˵�
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//A���ѹ��г��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020A0100[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	//B���ѹ��г��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020A0200[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+4,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	//C���ѹ��г��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020A0300[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_1_2
// Descriptions:        ����г��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_1_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"\n\r"
																	"A�������г��������:\n\r"
																	"     %\n\r"	
																	"B�������г��������:\n\r"
																	"     %\n\r"
																	"C�������г��������:\n\r"
																	"     %\n\r"
																	);//��ʾ�˵�
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//A�������г��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020A0200[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	//B�������г��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020B0200[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+4,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
	//C�������г��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data020B0300[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+6,0,0,"--.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+0*2);
		DisplayData(Databuff,4,2,q16);
	}
		
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_2
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_1_2_1);//A���ѹ�ܻ�������
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_2_2);//B���ѹ�ܻ�������
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_2_3);//C���ѹ�ܻ�������
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
// Function name:       Menu_3_1_2_1
// Descriptions:        �������ݣ�A���ѹ�ܻ������ݣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_2_1(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ	
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//			Ret=QCSG_DecOneDay(&RealTime);
//			if(Ret==TRUE)
//			{
		RealTimeToYYTime(&RealTime, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008025, &DataTime, PnData, &DataLen);
		//�ܻ�����ʱ��
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//�ܻ���ϸ���
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ��䳬������
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ�������ʱ��
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
		//}
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_2_2
// Descriptions:        B���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_2_2(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ	
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;		
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(&RealTime, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008026, &DataTime, PnData, &DataLen);
		//�ܻ�����ʱ��
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//�ܻ���ϸ���
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ��䳬������
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ�������ʱ��
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
		//}
	}

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_2_3
// Descriptions:        C���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_2_3(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ	
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(&RealTime, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008027, &DataTime, PnData, &DataLen);
		//�ܻ�����ʱ��
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//�ܻ���ϸ���
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ��䳬������
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ�������ʱ��
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//����ܻ���
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
			DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
		//}
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_3
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_3(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_1_3_1);//A���ѹ�ܻ�������
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_3_2);//B���ѹ�ܻ�������
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_3_3);//C���ѹ�ܻ�������
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
// Function name:       Menu_3_1_3_1
// Descriptions:        �������ݣ�A���ѹ�ܻ������ݣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_3_1(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//			Ret=QCSG_DecOneMonth(&RealTime);
//			if(Ret==TRUE)
//			{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C025, &DataTime, PnData, &DataLen);
			//�ܻ�����ʱ��
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+1,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//�ܻ���ϸ���
			MR((u32)&Databuff,(u32)&PnData[3],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+2,7,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
				DisplayData(Databuff,6,2,q16);
			}
			//�ܻ��䳬������
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[6],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				DisplayData(Databuff,6,2,q16);
			}
			//�ܻ�������ʱ��
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[9],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//����ܻ���
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[12],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+5,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}
			//����ܻ������ʱ��
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
			if(PnData[14]==0xFF)
			{
				DisplayString(Ln+6,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[14],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
			if(PnData[15]==0xFF)
			{
				DisplayString(Ln+6,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[15],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
			if(PnData[16]==0xFF)
			{
				DisplayString(Ln+6,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[16],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
			if(PnData[17]==0xFF)
			{
				DisplayString(Ln+6,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[17],2,0,q16);
			}
			//����ܻ���
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[18],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+7,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}	
			//����ܻ������ʱ��
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
			if(PnData[20]==0xFF)
			{
				DisplayString(Ln+8,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[18],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
			if(PnData[21]==0xFF)
			{
				DisplayString(Ln+8,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[19],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
			if(PnData[22]==0xFF)
			{
				DisplayString(Ln+8,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[20],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
			if(PnData[23]==0xFF)
			{
				DisplayString(Ln+8,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[21],2,0,q16);
			}
		//}
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_3_2
// Descriptions:        B���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_3_2(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//			Ret=QCSG_DecOneMonth(&RealTime);
//			if(Ret==TRUE)
//			{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C026, &DataTime, PnData, &DataLen);
			//�ܻ�����ʱ��
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+1,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//�ܻ���ϸ���
			MR((u32)&Databuff,(u32)&PnData[3],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+2,7,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
				DisplayData(Databuff,6,2,q16);
			}
			//�ܻ��䳬������
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[6],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				DisplayData(Databuff,6,2,q16);
			}
			//�ܻ�������ʱ��
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[9],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//����ܻ���
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[12],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+5,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}
			//����ܻ������ʱ��
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
			if(PnData[14]==0xFF)
			{
				DisplayString(Ln+6,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[14],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
			if(PnData[15]==0xFF)
			{
				DisplayString(Ln+6,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[15],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
			if(PnData[16]==0xFF)
			{
				DisplayString(Ln+6,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[16],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
			if(PnData[17]==0xFF)
			{
				DisplayString(Ln+6,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[17],2,0,q16);
			}
			//����ܻ���
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[18],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+7,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}	
			//����ܻ������ʱ��
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
			if(PnData[20]==0xFF)
			{
				DisplayString(Ln+8,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[18],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
			if(PnData[21]==0xFF)
			{
				DisplayString(Ln+8,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[19],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
			if(PnData[22]==0xFF)
			{
				DisplayString(Ln+8,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[20],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
			if(PnData[23]==0xFF)
			{
				DisplayString(Ln+8,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[21],2,0,q16);
			}	
		//}
	}	
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_3_3
// Descriptions:        C���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_3_3(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//			Ret=QCSG_DecOneMonth(&RealTime);
//			if(Ret==TRUE)
//			{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C027, &DataTime, PnData, &DataLen);
			//�ܻ�����ʱ��
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+1,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//�ܻ���ϸ���
			MR((u32)&Databuff,(u32)&PnData[3],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+2,7,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
				DisplayData(Databuff,6,2,q16);
			}
			//�ܻ��䳬������
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[6],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"----.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				DisplayData(Databuff,6,2,q16);
			}
			//�ܻ�������ʱ��
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[9],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				DisplayData(Databuff,6,0,q16);
			}
			//����ܻ���
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[12],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+5,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}
			//����ܻ������ʱ��
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
			if(PnData[14]==0xFF)
			{
				DisplayString(Ln+6,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[14],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
			if(PnData[15]==0xFF)
			{
				DisplayString(Ln+6,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[15],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
			if(PnData[16]==0xFF)
			{
				DisplayString(Ln+6,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[16],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
			if(PnData[17]==0xFF)
			{
				DisplayString(Ln+6,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[17],2,0,q16);
			}
			//����ܻ���
			Databuff = 0x00;
			MR((u32)&Databuff,(u32)&PnData[18],2);
			if(Databuff==0xFFFF)
			{
				DisplayString(Ln+7,11,0,"--.--");	
			}
			else
			{
				q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
				DisplayData(Databuff,4,2,q16);
			}	
			//����ܻ������ʱ��
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
			if(PnData[20]==0xFF)
			{
				DisplayString(Ln+8,18,0,"--");	
			}
			else
			{
						DisplayData(PnData[18],2,0,q16);										
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
			if(PnData[21]==0xFF)
			{
				DisplayString(Ln+8,15,0,"--");	
			}
			else
			{
				DisplayData(PnData[19],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
			if(PnData[22]==0xFF)
			{
				DisplayString(Ln+8,12,0,"--");	
			}
			else
			{
				DisplayData(PnData[20],2,0,q16);									
			}
			q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
			if(PnData[23]==0xFF)
			{
				DisplayString(Ln+8,9,0,"--");	
			}
			else
			{
				DisplayData(PnData[21],2,0,q16);
			}
		//}
	}

	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4
// Descriptions:        ��ʷ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_1_4_1);//A���ѹ�ܻ�������
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_4_2);//B���ѹ�ܻ�������
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_4_3);//C���ѹ�ܻ�������
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
// Function name:       Menu_3_1_4_1
// Descriptions:        ��ʷ��A���ѹ�ܻ������ݣ�ѡ�����ڣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//�˵��ӵ�0�п�ʼ
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//��ʾ�˵�
			
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
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
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
								Menu_3_1_4_1_2(&RealTime);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_1_2
// Descriptions:        ��ʷ��A���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_1_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
//Ret=QCSG_DecOneDay(&RealTime);
//if(Ret==TRUE)
//{
		RealTimeToYYTime(Time, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008025, &DataTime, PnData, &DataLen);
		//�ܻ�����ʱ��
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//�ܻ���ϸ���
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ��䳬������
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ�������ʱ��
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
	//}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_2
// Descriptions:        ��ʷ��B���ѹ�ܻ������ݣ�ѡ�����ڣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_2(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//�˵��ӵ�0�п�ʼ
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//��ʾ�˵�
			
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
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
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
								Menu_3_1_4_2_2(&RealTime);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_2_2
// Descriptions:        ��ʷ��B���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_2_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(Time, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008026, &DataTime, PnData, &DataLen);
		//�ܻ�����ʱ��
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//�ܻ���ϸ���
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ��䳬������
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ�������ʱ��
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//����ܻ���
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
	//}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_3
// Descriptions:        ��ʷ��C���ѹ�ܻ������ݣ�ѡ�����ڣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_3(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//�˵��ӵ�0�п�ʼ
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//��ʾ�˵�
			
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
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
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
								Menu_3_1_4_3_2(&RealTime);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_4_3_2
// Descriptions:        ��ʷ��C���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_4_3_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_2);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
//	Ret=QCSG_DecOneDay(&RealTime);
//	if(Ret==TRUE)
//	{
		RealTimeToYYTime(Time, &DataTime);
		NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008027, &DataTime, PnData, &DataLen);
		//�ܻ�����ʱ��
		MR((u32)&Databuff,(u32)&PnData[0],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+1,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//�ܻ���ϸ���
		MR((u32)&Databuff,(u32)&PnData[2],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+2,7,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ��䳬������
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[5],3);
		if(Databuff==0xFFFFFF)
		{
			DisplayString(Ln+3,9,0,"----.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
			DisplayData(Databuff,6,2,q16);
		}
		//�ܻ�������ʱ��
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[8],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+4,9,0,"----");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
			DisplayData(Databuff,4,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[10],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+5,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
		if(PnData[12]==0xFF)
		{
			DisplayString(Ln+6,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[12],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
		if(PnData[13]==0xFF)
		{
			DisplayString(Ln+6,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[13],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
		if(PnData[14]==0xFF)
		{
			DisplayString(Ln+6,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[14],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
		if(PnData[15]==0xFF)
		{
			DisplayString(Ln+6,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[15],2,0,q16);
		}
		//����ܻ���
		Databuff = 0x00;
		MR((u32)&Databuff,(u32)&PnData[16],2);
		if(Databuff==0xFFFF)
		{
			DisplayString(Ln+7,11,0,"--.--");	
		}
		else
		{
			q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
			DisplayData(Databuff,4,2,q16);
		}	
		//����ܻ������ʱ��
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
		if(PnData[18]==0xFF)
		{
			DisplayString(Ln+8,18,0,"--");	
		}
		else
		{
					DisplayData(PnData[18],2,0,q16);										
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
		if(PnData[19]==0xFF)
		{
			DisplayString(Ln+8,15,0,"--");	
		}
		else
		{
			DisplayData(PnData[19],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
		if(PnData[20]==0xFF)
		{
			DisplayString(Ln+8,12,0,"--");	
		}
		else
		{
			DisplayData(PnData[20],2,0,q16);									
		}
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
		if(PnData[21]==0xFF)
		{
			DisplayString(Ln+8,9,0,"--");	
		}
		else
		{
			DisplayData(PnData[21],2,0,q16);
		}
	//}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5
// Descriptions:        ��ʷ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_1_5_1);//A���ѹ�ܻ�������
						break;
					case 2:
						MenuCreate((u32)Menu_3_1_5_2);//B���ѹ�ܻ�������
						break;
					case 3:
						MenuCreate((u32)Menu_3_1_5_3);//C���ѹ�ܻ�������
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
// Function name:       Menu_3_1_5_1
// Descriptions:        ��ʷ��A���ѹ�ܻ������ݣ�ѡ�����ڣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//�˵��ӵ�0�п�ʼ
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//��ʾ�˵�
			
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
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
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
								Menu_3_1_5_1_2(&RealTime);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_1_2
// Descriptions:        ��ʷ��A���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_1_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
	RealTimeToYYTime(Time, &DataTime);
	NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C025, &DataTime, PnData, &DataLen);
	//�ܻ�����ʱ��
	MR((u32)&Databuff,(u32)&PnData[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//�ܻ���ϸ���
	MR((u32)&Databuff,(u32)&PnData[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,7,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//�ܻ��䳬������
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,9,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//�ܻ�������ʱ��
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//����ܻ���
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[12],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}
	//����ܻ������ʱ��
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
	if(PnData[14]==0xFF)
	{
		DisplayString(Ln+6,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[14],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
	if(PnData[15]==0xFF)
	{
		DisplayString(Ln+6,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[15],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
	if(PnData[16]==0xFF)
	{
		DisplayString(Ln+6,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[16],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
	if(PnData[17]==0xFF)
	{
		DisplayString(Ln+6,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[17],2,0,q16);
	}
	//����ܻ���
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[18],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}	
	//����ܻ������ʱ��
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
	if(PnData[20]==0xFF)
	{
		DisplayString(Ln+8,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[18],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
	if(PnData[21]==0xFF)
	{
		DisplayString(Ln+8,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[19],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
	if(PnData[22]==0xFF)
	{
		DisplayString(Ln+8,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[20],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
	if(PnData[23]==0xFF)
	{
		DisplayString(Ln+8,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[21],2,0,q16);
	}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_2
// Descriptions:        ��ʷ��B���ѹ�ܻ������ݣ�ѡ�����ڣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_2(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//�˵��ӵ�0�п�ʼ
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//��ʾ�˵�
			
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
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
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
								Menu_3_1_5_2_2(&RealTime);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_2_2
// Descriptions:        ��ʷ��B���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_2_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
	RealTimeToYYTime(Time, &DataTime);
	NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C026, &DataTime, PnData, &DataLen);
	//�ܻ�����ʱ��
	MR((u32)&Databuff,(u32)&PnData[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//�ܻ���ϸ���
	MR((u32)&Databuff,(u32)&PnData[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,7,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//�ܻ��䳬������
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,9,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//�ܻ�������ʱ��
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//����ܻ���
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[12],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}
	//����ܻ������ʱ��
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
	if(PnData[14]==0xFF)
	{
		DisplayString(Ln+6,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[14],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
	if(PnData[15]==0xFF)
	{
		DisplayString(Ln+6,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[15],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
	if(PnData[16]==0xFF)
	{
		DisplayString(Ln+6,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[16],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
	if(PnData[17]==0xFF)
	{
		DisplayString(Ln+6,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[17],2,0,q16);
	}
	//����ܻ���
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[18],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}	
	//����ܻ������ʱ��
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
	if(PnData[20]==0xFF)
	{
		DisplayString(Ln+8,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[18],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
	if(PnData[21]==0xFF)
	{
		DisplayString(Ln+8,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[19],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
	if(PnData[22]==0xFF)
	{
		DisplayString(Ln+8,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[20],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
	if(PnData[23]==0xFF)
	{
		DisplayString(Ln+8,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[21],2,0,q16);
	}	
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_3
// Descriptions:        ��ʷ��C���ѹ�ܻ������ݣ�ѡ�����ڣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_3(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 i,j,Ln;
	RealTimeStruct RealTime;	
	Ln=0;//�˵��ӵ�0�п�ʼ
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1);//��ʾ�˵�
			
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
			BoxSelect();
			Comm_Ram->ButtonVal &= ~0x0F;
			Stack->MenuStack[MenuCount->Count].Task = 2;
			switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
			{
				case 1:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 2:
					MenuCount->v1=7;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 3:
					MenuCount->v1=9;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 4:
					MenuCount->v1=9;
					MenuCount->v2=14;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 5:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 6:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 7:
					MenuCount->v1=11;
					MenuCount->v2=16;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				case 8:
					MenuCount->v1=9;
					MenuCount->v2=19;
					NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					break;
				default:
					break;
			}
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
								Menu_3_1_5_3_2(&RealTime);
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


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_1_5_3_2
// Descriptions:        ��ʷ��C���ѹ�ܻ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_1_5_3_2(RealTimeStruct *Time)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u32 DataLen = 0;
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
  DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_1_4_1_3);//��ʾ�˵�	
	Pn_Lcd_Data->u16Pn=0x0000;
	RealTimeToYYTime(Time, &DataTime);
	NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C027, &DataTime, PnData, &DataLen);
	//�ܻ�����ʱ��
	MR((u32)&Databuff,(u32)&PnData[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//�ܻ���ϸ���
	MR((u32)&Databuff,(u32)&PnData[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,7,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//�ܻ��䳬������
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,9,0,"----.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//�ܻ�������ʱ��
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"------");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//����ܻ���
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[12],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}
	//����ܻ������ʱ��
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+18*2);             //��
	if(PnData[14]==0xFF)
	{
		DisplayString(Ln+6,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[14],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+15*2);             //ʱ
	if(PnData[15]==0xFF)
	{
		DisplayString(Ln+6,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[15],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+12*2);             //��
	if(PnData[16]==0xFF)
	{
		DisplayString(Ln+6,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[16],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);	            //��
	if(PnData[17]==0xFF)
	{
		DisplayString(Ln+6,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[17],2,0,q16);
	}
	//����ܻ���
	Databuff = 0x00;
	MR((u32)&Databuff,(u32)&PnData[18],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+7,11,0,"--.--");	
	}
	else
	{
		q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+11*2);
		DisplayData(Databuff,4,2,q16);
	}	
	//����ܻ������ʱ��
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+18*2);             //��
	if(PnData[20]==0xFF)
	{
		DisplayString(Ln+8,18,0,"--");	
	}
	else
	{
				DisplayData(PnData[18],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+15*2);             //ʱ
	if(PnData[21]==0xFF)
	{
		DisplayString(Ln+8,15,0,"--");	
	}
	else
	{
		DisplayData(PnData[19],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+12*2);             //��
	if(PnData[22]==0xFF)
	{
		DisplayString(Ln+8,12,0,"--");	
	}
	else
	{
		DisplayData(PnData[20],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+9*2);	            //��
	if(PnData[23]==0xFF)
	{
		DisplayString(Ln+8,9,0,"--");	
	}
	else
	{
		DisplayData(PnData[21],2,0,q16);
	}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2
// Descriptions:        ��ѹ�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2(void)//��ѹ�������
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_2_1);//��������
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_2);//��������
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


#if (DLT500_REPORT == 1)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_2_1_1);//A���ѹ�ϸ���ͳ������
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_1_2);//B���ѹ�ϸ���ͳ������
						break;
					case 3:
						MenuCreate((u32)Menu_3_2_1_3);//C���ѹ�ϸ���ͳ������
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
// Function name:       Menu_3_2_1_1
// Descriptions:        A���ѹ�ϸ�������1�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"A���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ�ϸ��ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2_1
// Descriptions:        A���ѹ�ϸ�������2�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_1_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"A���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000011[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000011[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000011[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000011[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000011[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000011[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000011[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000011[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_3
// Descriptions:        B���ѹ�ϸ�������1�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"B���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ�ϸ��ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_3_1
// Descriptions:        B���ѹ�ϸ�������2�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_2_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"B���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000012[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000012[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000012[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000012[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000012[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000012[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000012[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000012[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_2);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_4
// Descriptions:        C���ѹ�ϸ�������1�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_3(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"C���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ�ϸ�ʱ�ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_3_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_3_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_4_1
// Descriptions:        C���ѹ�ϸ�������2�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_3_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"C���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000013[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000013[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000013[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000013[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000013[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000013[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000013[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000013[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_3);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_3);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_2_2_1);//A���ѹ�ϸ���ͳ������
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_2_2);//B���ѹ�ϸ���ͳ������
						break;
					case 3:
						MenuCreate((u32)Menu_3_2_2_3);//C���ѹ�ϸ���ͳ������
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
// Function name:       Menu_3_2_2_2
// Descriptions:        A���ѹ�ϸ�������1�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"A���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ�ϸ��ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2_1
// Descriptions:        A���ѹ�ϸ�������2�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_1_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"A���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100100[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100100[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100100[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100100[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100100[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100100[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100100[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100100[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_2
// Descriptions:        B���ѹ�ϸ�������1�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"B���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ�ϸ��ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_3_1
// Descriptions:        B���ѹ�ϸ�������2�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_2_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"B���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100200[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100200[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100200[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100200[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100200[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100200[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100200[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100200[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_2);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_4
// Descriptions:        C���ѹ�ϸ�������1�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_3(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"C���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ��������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,9,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ�ϸ��ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+8,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+8)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_3_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_3_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_4_1
// Descriptions:        C���ѹ�ϸ�������2�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_3_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"C���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100300[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100300[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100300[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100300[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[27],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100300[29],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100300[30],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100300[31],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100300[32],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_3);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_3);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_2_1_1);//�ܵ�ѹ�ϸ���ͳ������
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_1_2);//A���ѹ�ϸ���ͳ������
						break;
					case 3:
						MenuCreate((u32)Menu_3_2_1_3);//B���ѹ�ϸ���ͳ������
						break;
					case 4:
						MenuCreate((u32)Menu_3_2_1_4);//C���ѹ�ϸ���ͳ������
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
// Function name:       Menu_3_2_1_1
// Descriptions:        �ܵ�ѹ�ϸ�������1�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"�ܵ�ѹ�ϸ�������1");	
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_1_1
// Descriptions:        �ܵ�ѹ�ϸ�������2�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_1_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"�ܵ�ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000010[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000010[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000010[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000010[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000010[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000010[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000010[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000010[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000010[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2
// Descriptions:        A���ѹ�ϸ�������1�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"A���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2_1
// Descriptions:        A���ѹ�ϸ�������2�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_2_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"A���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000011[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000011[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000011[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000011[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000011[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000011[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000011[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000011[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000011[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_2);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_3
// Descriptions:        B���ѹ�ϸ�������1�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_3(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"B���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_3_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_3_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_3_1
// Descriptions:        B���ѹ�ϸ�������2�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_3_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"B���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000012[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000012[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000012[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000012[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000012[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000012[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000012[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000012[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000012[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_3);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_3);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_4
// Descriptions:        C���ѹ�ϸ�������1�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_4(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"C���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_4_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_4_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_4_1
// Descriptions:        C���ѹ�ϸ�������2�����գ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_1_4_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"C���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000013[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000013[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000013[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000013[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000013[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE1000013[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE1000013[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE1000013[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE1000013[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_4);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_1_4);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_2_2_1);//�ܵ�ѹ�ϸ���ͳ������
						break;
					case 2:
						MenuCreate((u32)Menu_3_2_2_2);//A���ѹ�ϸ���ͳ������
						break;
					case 3:
						MenuCreate((u32)Menu_3_2_2_3);//B���ѹ�ϸ���ͳ������
						break;
					case 4:
						MenuCreate((u32)Menu_3_2_2_4);//C���ѹ�ϸ���ͳ������
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
// Function name:       Menu_3_2_2_1
// Descriptions:        �ܵ�ѹ�ϸ�������1�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_1(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"�ܵ�ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0

}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_1_1�����£�
// Descriptions:        �ܵ�ѹ�ϸ�������2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_1_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"�ܵ�ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100000[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100000[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100000[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100000[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100000[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100000[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100000[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100000[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100000[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_2
// Descriptions:        A���ѹ�ϸ�������1�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_2(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"A���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_1_2_1
// Descriptions:        A���ѹ�ϸ�������2�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_2_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"A���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100100[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100100[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100100[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100100[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100100[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100100[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100100[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100100[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100100[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_2);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_3
// Descriptions:        B���ѹ�ϸ�������1�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_3(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"B���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_3_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_3_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_3_1
// Descriptions:        B���ѹ�ϸ�������2�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_3_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"B���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100200[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100200[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100200[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100200[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100200[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100200[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100200[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100200[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100200[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_3);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_3);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_4
// Descriptions:        C���ѹ�ϸ�������1�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_4(void)
{
	u16 *q16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"C���ѹ�ϸ�������1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ѹ���ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,9,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,6,0,q16);
	}
	//��ѹ�ϸ���
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,7,0,"----.--");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
		DisplayData(Databuff,6,2,q16);
	}
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}	
	//��ѹ������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,q16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_4_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_4_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_2_2_4_1
// Descriptions:        C���ѹ�ϸ�������2�����£�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_2_2_4_1(void)
{
	u16 *q16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayClr();//��ȫ��
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_2_1_1_1);//��ʾ�˵�	
	DisplayString(1,1,0,"C���ѹ�ϸ�������2");
	
	Pn_Lcd_Data->u16Pn = 0x0000;      //ֻ��ʾ���ɲ���������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ߵ�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[15],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��ߵ�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100300[17],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100300[18],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100300[19],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100300[20],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
	//��͵�ѹ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8Data03100300[21],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+5,9,0,"---.-");
	}
	else
	{
		q16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+9*2);
		DisplayData(Databuff,4,1,q16);
	}
	//��͵�ѹ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8Data03100300[23],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8Data03100300[24],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8Data03100300[25],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8Data03100300[26],1);
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+7,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,q16);										
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+7,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+7,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,q16);									
	}
	q16=(u16*)(ADDR_STRINGBUFF+(Ln+7)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+7,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,q16);
	}	
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_4);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_2_2_4);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}
#endif



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_3
// Descriptions:        ��ѹ��ƽ���Խ��ͳ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_3(void)//��ѹ��ƽ���Խ��ͳ������
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_3_1);//��ʷ������
						break;
					case 2:
						MenuCreate((u32)Menu_3_3_2);//��ʷ������
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
// Function name:       Menu_3_3_1
// Descriptions:        ��ѹ��ƽ���Խ��ͳ�����ݣ���ʷ�յ�ѹ��ƽ���1��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_3_1(void)
{
	u16 *p16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_1);//��ʾ�˵�
	DisplayString(Ln+1,0,0,"��ʷ�յ�ѹ��ƽ���1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008021, &DataTime, PnData, &DataLen);
				//���ֵ
				MR((u32)&Databuff,(u32)&PnData[0],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+2,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//��Сֵ
				MR((u32)&Databuff,(u32)&PnData[6],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+3,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//ƽ��ֵ
				MR((u32)&Databuff,(u32)&PnData[12],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+4,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//95%ֵ
				MR((u32)&Databuff,(u32)&PnData[14],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+5,6,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,4,2,p16);
				}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_3_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_3_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_3_1_1
// Descriptions:        ��ʷ�յ�ѹ��ƽ���2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_3_1_1(void)//��ʷ�յ�ѹ��ƽ���2
{
	u16 *p16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_2);//��ʾ�˵�
	DisplayString(Ln+1,0,0,"��ʷ�յ�ѹ��ƽ���2");
	
	
	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ֵ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100001C[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,5,0,"--.--");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
		DisplayData(Databuff,4,2,p16);
	}
	//��ֵ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE100001C[2],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE100001C[3],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE100001C[4],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE100001C[5],1);
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,p16);										
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,p16);
	}	
	//��ֵ�ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100001C[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,5,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_3_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_3_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_3_2
// Descriptions:        ��ѹ��ƽ���Խ��ͳ�����ݣ���ʷ�µ�ѹ��ƽ���1��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_3_2(void)
{
	u16 *p16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ	
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_1);//��ʾ�˵�
	DisplayString(Ln+1,0,0,"��ʷ�µ�ѹ��ƽ���1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C021, &DataTime, PnData, &DataLen);
				//���ֵ
				MR((u32)&Databuff,(u32)&PnData[0],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+2,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//��Сֵ
				MR((u32)&Databuff,(u32)&PnData[6],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+3,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//ƽ��ֵ
				MR((u32)&Databuff,(u32)&PnData[12],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+4,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//95%ֵ
				MR((u32)&Databuff,(u32)&PnData[14],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+5,6,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,4,2,p16);
				}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_3_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_3_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_3_2_1
// Descriptions:        ��ʷ�µ�ѹ��ƽ���2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_3_2_1(void)
{
	u16 *p16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_2);//��ʾ�˵�
	DisplayString(Ln+1,0,0,"��ʷ�µ�ѹ��ƽ���2");
	
	
	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ֵ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100401C[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,5,0,"--.--");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
		DisplayData(Databuff,4,2,p16);
	}
	//��ֵ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE100401C[2],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE100401C[3],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE100401C[4],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE100401C[5],1);
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,p16);										
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,p16);
	}	
	//��ֵ�ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100401C[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,5,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_3_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_3_2);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_4
// Descriptions:        ������ƽ���Խ��ͳ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_4(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_4_1);//��ʷ������
						break;
					case 2:
						MenuCreate((u32)Menu_3_4_2);//��ʷ������
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
// Function name:       Menu_3_4_1
// Descriptions:        ������ƽ���Խ��ͳ�����ݣ���ʷ�յ�����ƽ���1��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_4_1(void)
{
	u16 *p16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_1);//��ʾ�˵�
	DisplayString(Ln+1,0,0,"��ʷ�յ�����ƽ���1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008020, &DataTime, PnData, &DataLen);
				//���ֵ
				MR((u32)&Databuff,(u32)&PnData[0],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+2,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//��Сֵ
				MR((u32)&Databuff,(u32)&PnData[6],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+3,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//ƽ��ֵ
				MR((u32)&Databuff,(u32)&PnData[12],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+4,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//95%ֵ
				MR((u32)&Databuff,(u32)&PnData[14],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+5,6,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,4,2,p16);
				}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_4_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_4_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_4_1_1
// Descriptions:        ��ʷ�յ�����ƽ���2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_4_1_1(void)//��ʷ�յ�����ƽ���2
{
	u16 *p16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_2);//��ʾ�˵�
	DisplayString(Ln+1,0,0,"��ʷ�յ�����ƽ���2");
	
	
	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ֵ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100001B[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,5,0,"--.--");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
		DisplayData(Databuff,4,2,p16);
	}
	//��ֵ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE100001B[2],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE100001B[3],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE100001B[4],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE100001B[5],1);
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,p16);										
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,p16);
	}	
	//��ֵ�ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100001B[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,5,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_4_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_4_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_4_2
// Descriptions:        ������ƽ���Խ��ͳ�����ݣ���ʷ�µ�����ƽ���1��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_4_2(void)
{
	u16 *p16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_1);//��ʾ�˵�
	DisplayString(Ln+1,0,0,"��ʷ�µ�����ƽ���1");
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C020, &DataTime, PnData, &DataLen);
				//���ֵ
				MR((u32)&Databuff,(u32)&PnData[0],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+2,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//��Сֵ
				MR((u32)&Databuff,(u32)&PnData[6],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+3,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//ƽ��ֵ
				MR((u32)&Databuff,(u32)&PnData[12],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+4,7,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+7*2);
					DisplayData(Databuff,4,2,p16);
				}
				//95%ֵ
				MR((u32)&Databuff,(u32)&PnData[14],2);
				if(Databuff==0xFFFF)
				{
					DisplayString(Ln+5,6,0,"--.--");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+6*2);
					DisplayData(Databuff,4,2,p16);
				}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_4_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_4_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_4_2_1
// Descriptions:        ��ʷ�µ�����ƽ���2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_4_2_1(void)
{
	u16 *p16;
	u8 Databuff2[4];
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_3_2);//��ʾ�˵�
	DisplayString(Ln+1,0,0,"��ʷ�µ�����ƽ���2");
	
	
	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//��ֵ
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100401B[0],2);
	if(Databuff==0xFFFF)
	{
		DisplayString(Ln+2,5,0,"--.--");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
		DisplayData(Databuff,4,2,p16);
	}
	//��ֵ����ʱ��
	MR((u32)&Databuff2[0],(u32)&Pn_Lcd_Data->u8DataE100401B[2],1);
	MR((u32)&Databuff2[1],(u32)&Pn_Lcd_Data->u8DataE100401B[3],1);
	MR((u32)&Databuff2[2],(u32)&Pn_Lcd_Data->u8DataE100401B[4],1);
	MR((u32)&Databuff2[3],(u32)&Pn_Lcd_Data->u8DataE100401B[5],1);
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+12*2);             //��
	if(Databuff2[0]==0xFF)
	{
		DisplayString(Ln+4,12,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[0],2,0,p16);										
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+8*2);             //ʱ
	if(Databuff2[1]==0xFF)
	{
		DisplayString(Ln+4,8,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[1],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+4*2);             //��
	if(Databuff2[2]==0xFF)
	{
		DisplayString(Ln+4,4,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[2],2,0,p16);									
	}
	p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+0*2);	            //��
	if(Databuff2[3]==0xFF)
	{
		DisplayString(Ln+4,0,0,"--");	
	}
	else
	{
		DisplayData(Databuff2[3],2,0,p16);
	}	
	//��ֵ�ۼ�ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE100401B[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,5,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+5*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_4_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_4_2);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5
// Descriptions:        ������������ͳ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5(void)//������������ͳ������
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_5_1);//��������
						break;
					case 2:
						MenuCreate((u32)Menu_3_5_2);//��������
						break;
					case 3:
						MenuCreate((u32)Menu_3_5_3);//��ʷ������
						break;
					case 4:
						MenuCreate((u32)Menu_3_5_4);//��ʷ������
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
// Function name:       Menu_3_5_1
// Descriptions:        ���չ�����������ͳ������1
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5_1(void)//������������ͳ������
{
	u16 *p16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_1);//��ʾ�˵�

	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//�����������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,0,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+0*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����1ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����2ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����3ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����4ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_1_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_1_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_1_1
// Descriptions:        ���չ�����������ͳ������2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5_1_1(void)
{
	u16 *p16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_2);//��ʾ�˵�

	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//����5ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����6ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����7ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[21],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����8ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[24],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����9ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[27],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����10ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1000033[30],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,p16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_2
// Descriptions:        ���¹�����������ͳ������1
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5_2(void)//������������ͳ������
{
	u16 *p16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_1);//��ʾ�˵�

	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//�����������ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[0],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,0,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+0*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����1ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[3],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����2ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[6],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����3ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[9],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����4ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[12],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
		
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_2_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_2_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_2_1
// Descriptions:        ���¹�����������ͳ������2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5_2_1(void)
{
	u16 *p16;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_2);//��ʾ�˵�

	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
	Pn_Lcd_Data->u16Res1 |= LCD_RT_AMR_REQ;			
	//����5ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[15],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+1,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����6ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[18],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+2,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����7ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[21],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����8ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[24],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����9ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[27],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+5,10,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+5)*84+4+10*2);
		DisplayData(Databuff,6,0,p16);
	}
	//����10ʱ��
	MR((u32)&Databuff,(u32)&Pn_Lcd_Data->u8DataE1004033[30],3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,11,0,"------");
	}
	else
	{
		p16=(u16*)(ADDR_STRINGBUFF+(Ln+6)*84+4+11*2);
		DisplayData(Databuff,6,0,p16);
	}
	
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_2);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_3
// Descriptions:        ��ʷ�չ�����������ͳ������1
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5_3(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_1);//��ʾ�˵�

	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008043, &DataTime, PnData, &DataLen);
				//�����������ʱ��
				MR((u32)&Databuff,(u32)&PnData[0],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+2,0,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+0*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����1ʱ��
				MR((u32)&Databuff,(u32)&PnData[3],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+3,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����2ʱ��
				MR((u32)&Databuff,(u32)&PnData[6],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+4,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����3ʱ��
				MR((u32)&Databuff,(u32)&PnData[9],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+5,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����4ʱ��
				MR((u32)&Databuff,(u32)&PnData[12],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+6,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
		//}
		}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_3_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_3_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_3_1
// Descriptions:        ��ʷ�չ�����������ͳ������2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5_3_1(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_2);//��ʾ�˵�

	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008043, &DataTime, PnData, &DataLen);
				//����5ʱ��
				MR((u32)&Databuff,(u32)&PnData[15],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+1,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����6ʱ��
				MR((u32)&Databuff,(u32)&PnData[18],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+2,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����7ʱ��
				MR((u32)&Databuff,(u32)&PnData[21],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+3,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����8ʱ��
				MR((u32)&Databuff,(u32)&PnData[24],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+4,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����9ʱ��
				MR((u32)&Databuff,(u32)&PnData[27],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+5,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����10ʱ��
				MR((u32)&Databuff,(u32)&PnData[30],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+6,11,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+11*2);
					DisplayData(Databuff,6,0,q16);
				}
		//}
		}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_3);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_3);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_4
// Descriptions:        ��ʷ�¹�����������ͳ������1
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5_4(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_1);//��ʾ�˵�

	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C043, &DataTime, PnData, &DataLen);
				//�����������ʱ��
				MR((u32)&Databuff,(u32)&PnData[0],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+2,0,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+0*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����1ʱ��
				MR((u32)&Databuff,(u32)&PnData[3],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+3,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����2ʱ��
				MR((u32)&Databuff,(u32)&PnData[6],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+4,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����3ʱ��
				MR((u32)&Databuff,(u32)&PnData[9],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+5,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����4ʱ��
				MR((u32)&Databuff,(u32)&PnData[12],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+6,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
		//}
		}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_4_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_4_1);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3_5_4_1
// Descriptions:        ��ʷ�¹�����������ͳ������2
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3_5_4_1(void)
{
	u16 *q16;
	u32 Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
		
	DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3_5_2);//��ʾ�˵�

	Pn_Lcd_Data->u16Pn = 0x0000;//ֻ��ʾ�ն˱��������0������
		Ret=HAL_GetSystemTime(&RealTime);
		if(Ret==TRUE)
		{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
				RealTimeToYYTime(&RealTime, &DataTime);
				NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C043, &DataTime, PnData, &DataLen);
				//����5ʱ��
				MR((u32)&Databuff,(u32)&PnData[15],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+1,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����6ʱ��
				MR((u32)&Databuff,(u32)&PnData[18],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+2,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����7ʱ��
				MR((u32)&Databuff,(u32)&PnData[21],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+3,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����8ʱ��
				MR((u32)&Databuff,(u32)&PnData[24],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+4,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����9ʱ��
				MR((u32)&Databuff,(u32)&PnData[27],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+5,10,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+10*2);
					DisplayData(Databuff,6,0,q16);
				}
				//����10ʱ��
				MR((u32)&Databuff,(u32)&PnData[30],3);
				if(Databuff==0xFFFFFF)
				{
					DisplayString(Ln+6,11,0,"------");	
				}
				else
				{
					q16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+11*2);
					DisplayData(Databuff,6,0,q16);
				}
		//}
		}
				
	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_4);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_3_5_4);//����
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_3
// Descriptions:        �����������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_3(void)//�����������
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_3);//��ʾ�˵�
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
						MenuCreate((u32)Menu_3_1);//г������ 
						break;
					case 2:
						MenuCreate((u32)Menu_3_2);//��ѹ�������
						break;
					case 3:
						MenuCreate((u32)Menu_3_3);//��ѹ��ƽ���Խ��ͳ������
						break;
					case 4:
						MenuCreate((u32)Menu_3_4);//������ƽ���Խ��ͳ������
						break;
					case 5:
						MenuCreate((u32)Menu_3_5);//������������ͳ������
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
