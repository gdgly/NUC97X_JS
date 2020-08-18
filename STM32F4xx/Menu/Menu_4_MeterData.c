
//�˵�4 ���ܱ�ʾ��
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu4.h"
#include "../Device/MEMRW.h"
#include "../Terminal/Terminal_AFN0C_RealTimeData_Fn.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"



extern const u8 UnitList[][6];//ͨ�õ�λ�б�
extern const u8 SList[][2];//���������б�

#if ((Project/100)==2)//������
const u8 pMenu2_4[]=//������������������ʾ
{
	"������������ʾ\n\r"
	"  1.��ѹ����\n\r"
	"  2.�����\n\r"
	"  3.�������\n\r"
	"  4.�������\n\r"
	"  5.�������\n\r"
	"  6.��������\n\r"
	"  7.�������޹�����\n\r"
};

const u8 pMenu_4_7[]=//�������޹�����
{
	"�������޹�����\n\r"
	"  1.һ�����޹�����\n\r"
	"  2.�������޹�����\n\r"
	"  3.�������޹�����\n\r"
	"  4.�������޹�����\n\r"
};
#endif

const u8 pMenu_4[]=
{
	"���ܱ�ʾ��\n\r"
	"  1.��ѹ����\n\r"
	"  2.�����\n\r"
	"  3.�������\n\r"
	"  4.�������\n\r"
	"  5.�������\n\r"
	"  6.��������\n\r"
};

const u8 pMenu_4_1[]=
{
	"�����ѹ����\n\r"
	"������\n\r"
};

const u8 UIList[][20]=//�����ѹ�����б�
{
#if(((Project/100)==3 && (USER/100)!=5) || ((Project/100)==2))//�����������Ϻ�ר��
	"�ֱ��:",
	"���ַ:",
#endif
	"��ѹA :",
	"    B :",
	"    C :",
	"����A :",
	"    B :",
	"    C :",
};

const u8 pMenu_4_2[]=
{
	"�����\n\r"
	"������\n\r"
};

const u8 ActivePowerList[][20]=//����й������б�
{
#if(((Project/100)==3 && (USER/100)!=5) || ((Project/100)==2))//�����������Ϻ�ר��
	"�ֱ��:",
#endif
	"���ַ:",
	"�й���:",
	"     A:",
	"     B:",
	"     C:",
};

const u8 ReactivePowerList[][20]=//����޹������б�
{
#if(((Project/100)==3 && (USER/100)!=5) || ((Project/100)==2))//�����������Ϻ�ר��
	"�ֱ��:",
#endif
	"���ַ:",
	"�޹���:",
	"     A:",
	"     B:",
	"     C:",
};

const u8 ElectricEnergyList[][20]=//�������б�
{
#if ((USER/100)==5)//�Ϻ��û�
	#if ((Project/100)==3)//ר��
	"���ַ:",
	"  ��:",
	"  ��:",
	"  ƽ:",
	"  ��:",
	"  ��:",
	#elif ((Project/100)==2)//������
	"�ֱ��:",
	"���ַ:",
	"  ��:",
	"  ��:",
	"  ƽ:",
	"  ��:",
	"  ��:",
	#endif
#else
	"�ֱ��:",
	"���ַ:",
	"  ��:",
	"  ��:",
	"  ��:",
	"  ƽ:",
	"  ��:",
#endif
};

const u8 pMenu_4_5[]=
{
	"�������\n\r"
	"  1.���������й�\n\r"
	"  2.���������޹�\n\r"
	"  3.���·����й�\n\r"
	"  4.���·����޹�\n\r"
	"  5.���������й�\n\r"
	"  6.���������޹�\n\r"
	"  7.���·����й�\n\r"
	"  8.���·����޹�\n\r"
};

const u8 Demand1List[][20]=
{
#if ((USER/100)==5)//�Ϻ��û�
	#if ((Project/100)==3)//ר��
	"��:",
	" ",
	"��:",
	" ",
	"ƽ:",
	#elif ((Project/100)==2)//������
	"�ֱ��:",
	"���ַ:",
	"��:",
	" ",
	"��:",
	" ",
	"ƽ:",
	#endif
#else
	"�ֱ��:",
	"���ַ:",
	"��:",
	" ",
	"��:",
	" ",
	"��:",
#endif
};

const u8 Demand2List[][20]=
{
#if ((USER/100)==5)//�Ϻ��û�
	#if ((Project/100)==3)//ר��
	"��:",
	" ",
	"��:",
	#elif ((Project/100)==2)//������
	"�ֱ��:",
	"���ַ:",
	"��:",
	" ",
	"��:",
	#endif
#else
	"�ֱ��:",
	"���ַ:",
	"ƽ:",
	" ",
	"��:",
#endif
};

const u8 pMenu_4_5_1[]=
{
	"���������й�\n\r"
	"������\n\r"
};

const u8 pMenu_4_5_2[]=
{
	"���������޹�\n\r"
	"������\n\r"
};

const u8 pMenu_4_5_3[]=
{
	"���·����й�\n\r"
	"������\n\r"
};

const u8 pMenu_4_5_4[]=
{
	"���·����޹�\n\r"
	"������\n\r"
};

const u8 pMenu_4_5_5[]=
{
	"���������й�\n\r"
	"������\n\r"
};

const u8 pMenu_4_5_6[]=
{
	"���������޹�\n\r"
	"������\n\r"
};

const u8 pMenu_4_5_7[]=
{
	"���·����й�\n\r"
	"������\n\r"
};

const u8 pMenu_4_5_8[]=
{
	"���·����޹�\n\r"
	"������\n\r"
};

const u8 pMenu_4_6[]=
{
	"���������\n\r"
	"������\n\r"
};

const u8 PFList[][20]=//����������б�
{
#if(((Project/100)==3 && (USER/100)!=5) || ((Project/100)==2))//�����������Ϻ�ר��
	"�ֱ��:",
#endif
	"���ַ:",
	"������:",
	"     A:",
	"     B:",
	"     C:",
};

/*
u32 Check_AFN04F10_Pn(u32 Pn)//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
{
#if IC_SST39VF6401B==0//0=û,1=��
	u32 i;
	u32 Addr;
	u32 Meter;
	if(Pn==0)
	{
		return 0;
	}
	Addr=ADDR_AFN04F10+2;
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=MRR(Addr,2);
		if(i==Pn)
		{
			return Meter;
		}
		Addr+=LEN_AFN04F10_Pn;
	}
	return 0;
#else
	u32 i;
	u32 Meter;
	u8 *p8;
	if(Pn==0)
	{
		return 0;
	}
	p8=(u8*)(ADDR_AFN04F10+2);
	for(Meter=1;Meter<=AFN04F10MaxPn;Meter++)
	{
		i=p8[0]+(p8[1]<<8);
		if(i==Pn)
		{
			return Meter;
		}
		p8+=LEN_AFN04F10_Pn;
	}
	return 0;
#endif
}
*/

#if ((Project/100)==2)//2Ϊ������
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7_4
// Descriptions:        �������޹�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_7_4(void)//�˵�4_7_4
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�������޹�����\n\r"
																		"������\n\r"
			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//�������޹�����ʾֵ
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(136,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(2,4,0,"��Ч�������");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7_3
// Descriptions:        �������޹�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_7_3(void)//�˵�4_7_3
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�������޹�����\n\r"
																		"������\n\r"
			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//�������޹�����ʾֵ
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(135,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(2,4,0,"��Ч�������");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7_2
// Descriptions:        �������޹�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_7_2(void)//�˵�4_7_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�������޹�����\n\r"
																		"������\n\r"
			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//�������޹�����ʾֵ
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(134,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(2,4,0,"��Ч�������");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7_1
// Descriptions:        һ�����޹�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_7_1(void)//�˵�4_7_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"һ�����޹�����\n\r"
																		"������\n\r"
			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//һ�����޹�����ʾֵ
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(133,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(2,4,0,"��Ч�������");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_7
// Descriptions:        �������޹�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_7(void)//�˵�4_7
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_7);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_4_7_1);//һ�����޹�����
						break;
					case 2:
						MenuCreate((u32)Menu_4_7_2);//�������޹�����
						break;
					case 3:
						MenuCreate((u32)Menu_4_7_3);//�������޹�����
						break;
					case 4:
						MenuCreate((u32)Menu_4_7_4);//�������޹�����
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_6
// Descriptions:        ���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_6(void)//�˵�4_6
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_6);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)PFList[i]);
		}
#else
		for(i=0;i<=5;i++)
		{
			DisplayString(2+i,0,0,(u8 *)PFList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif

		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//�������� ��ABC
		for(i=0;i<=3;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+29+2*i,2);	       
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(4+i,8,0,"-----");	
			}
			else
			{
				DisplayString(4+i,7,0,(u8 *)SList[y>>15]);//+ -
				y&=0x7fff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+8*2);
				DisplayData_OffZero(y,4,1,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[6]);//6 %
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_8_1
// Descriptions:        ���·����޹��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_8_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_8);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
					CreateScreen((u32)Menu_4_5_8);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<3;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//���·����޹�������� ƽ��
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_8
// Descriptions:        ���·����޹��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5_8(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_8);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_5_8_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//���·����޹�������� �ܼ��
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//����ʱ�� ʱ:�� 
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(152,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_7_1
// Descriptions:        ���·����й��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_7_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_7);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
					CreateScreen((u32)Menu_4_5_7);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<3;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//���·����й�������� ƽ��
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_7
// Descriptions:        ���·����й��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5_7(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_7);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_5_7_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//���·����й�������� �ܼ��
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(151,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_6_1
// Descriptions:        ���������޹��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_6_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_6);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
					CreateScreen((u32)Menu_4_5_6);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<3;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//���������޹�������� ƽ��
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_6
// Descriptions:        ���������޹��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5_6(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_6);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_5_6_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//���������޹�������� �ܼ��
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(150,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_5_1
// Descriptions:        ���������й��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_5_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_5);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
					CreateScreen((u32)Menu_4_5_5);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand2List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//���������й�������� ƽ��
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_5
// Descriptions:        ���������й��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5_5(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_5);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{ 
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_5_5_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//���������й�������� �ܼ��
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(149,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_4_1
// Descriptions:        ���·����޹��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_4_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_4);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
				CreateScreen((u32)Menu_4_5_4);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand2List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//���·����޹�������� ƽ��
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_4
// Descriptions:        ���·����޹����������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5_4(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_4);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_5_4_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//���·����޹�������� �ܼ��
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(148,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_3_1
// Descriptions:        ���·����й��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_3_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_3);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{ 
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
					CreateScreen((u32)Menu_4_5_3);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand2List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//���·����й�������� ƽ��
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_3
// Descriptions:        ���·����й��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5_3(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_3);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_5_3_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//���·����й�������� �ܼ��
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(147,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_2_1
// Descriptions:        ���������޹��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_2_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_2);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
					CreateScreen((u32)Menu_4_5_2);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand2List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=1;i++)
		{
			//���������޹�������� ƽ��
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_2
// Descriptions:        ���������޹��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5_2(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_2);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_5_2_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		for(i=0;i<=2;i++)
		{
			//���������޹�������� �ܼ��
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[1]);//1 kvar
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(146,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuContent_4_5_1_1
// Descriptions:        ���������й��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_5_1_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_1);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
					CreateScreen((u32)Menu_4_5_1);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<3;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand2List[i]);
		}
#else
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+i,0,0,(u8 *)Demand2List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	#if ((Project/100)==2 && (USER/100)==5)//�Ϻ�������
		p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v2-1)*LEN_AFN04F10_Pn)+5);
		if((p8[0]==21) && (p8[13]!=4))
		{//�Ϻ���Լ������2
/*
	����Ϻ��������������з�����Ϊ2ʱ������������1��Ӧ�Ϻ������ƽ������������2��Ӧ�Ϻ�����ʹȣ�
	�����������з�����Ϊ4ʱ������������1��4�ֱ��Ӧ��Ч���ݡ��Ϻ������ƽ����Ч���ݡ��Ϻ�����ʹȡ�
	�˷���˳���һ��Ͷ������ݾ���Ҫ��
*/			
		//��
			DisplayString(Ln,5,0,"-------");	
			DisplayString(Ln,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			DisplayString(Ln+1,13,0,"-----");	
			//����ʱ�� ��-��
			DisplayString(Ln+1,7,0,"-----");	
		//��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+14,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+14,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+14,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		else
		{
			for(i=0;i<=1;i++)
			{
				//���������й�������� ƽ��
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+27+7*i,3);	                   
				if(y==0xEEEEEE)
				{//�޴�������-------
					DisplayString(Ln+2*i,5,0,"-------");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
					DisplayData_OffZero(y,6,4,p16);
				}
				DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
				//����ʱ�� ʱ:��
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+30+7*i,2);	                   
				if(y==0xEEEE)
				{//�޴�������-----
					DisplayString(Ln+1+2*i,13,0,"-----");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
					DisplayData1(y,4,2,':',p16);
				}
				//����ʱ�� ��-��
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+32+7*i,2);	                   
				if(y==0xEEEE)
				{//�޴�������-----
					DisplayString(Ln+1+2*i,7,0,"-----");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
					DisplayData1(y,4,2,'-',p16);
				}
			}
		}
	#else
		for(i=0;i<=1;i++)
		{
			//���������й�������� ƽ��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+27+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+30+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+32+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
	#endif
		UpArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5_1
// Descriptions:        ���������й��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5_1);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_5_1_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif
	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<=4;i++)
		{
			DisplayString(3+i,0,0,(u8 *)Demand1List[i]);
		}
#else
		for(i=0;i<=6;i++)
		{
			DisplayString(2+i,0,0,(u8 *)Demand1List[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	#if ((Project/100)==2 && (USER/100)==5)//�Ϻ�������
		p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v2-1)*LEN_AFN04F10_Pn)+5);
		if((p8[0]==21) && (p8[13]!=4))
		{//�Ϻ���Լ������2
/*
	����Ϻ��������������з�����Ϊ2ʱ������������1��Ӧ�Ϻ������ƽ������������2��Ӧ�Ϻ�����ʹȣ�
	�����������з�����Ϊ4ʱ������������1��4�ֱ��Ӧ��Ч���ݡ��Ϻ������ƽ����Ч���ݡ��Ϻ�����ʹȡ�
	�˷���˳���һ��Ͷ������ݾ���Ҫ��
*/			
		//��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		//��
			DisplayString(Ln+2,5,0,"-------");	
			DisplayString(Ln+2,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			DisplayString(Ln+3,13,0,"-----");	
			//����ʱ�� ��-��
			DisplayString(Ln+3,7,0,"-----");	
		//ƽ
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+7,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+4,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+4,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+7,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+4,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+4)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+7,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+4,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+4)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
		else
		{
			for(i=0;i<=2;i++)
			{
				//���������й�������� �ܼ��
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+7*i,3);	                   
				if(y==0xEEEEEE)
				{//�޴�������-------
					DisplayString(Ln+2*i,5,0,"-------");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
					DisplayData_OffZero(y,6,4,p16);
				}
				DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
				//����ʱ�� ʱ:��
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+7*i,2);	                   
				if(y==0xEEEE)
				{//�޴�������-----
					DisplayString(Ln+1+2*i,13,0,"-----");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
					DisplayData1(y,4,2,':',p16);
				}
				//����ʱ�� ��-��
				y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+7*i,2);	                   
				if(y==0xEEEE)
				{//�޴�������-----
					DisplayString(Ln+1+2*i,7,0,"-----");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
					DisplayData1(y,4,2,'-',p16);
				}
			}
		}
	#else
		for(i=0;i<=2;i++)
		{
			//���������й�������� �ܼ��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+7*i,3);	                   
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+7*i,2);	                   
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,7,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+7*2);
				DisplayData1(y,4,2,'-',p16);
			}
		}
	#endif
		DownArrow();
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_5
// Descriptions:        �������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_5(void)//�˵�4_5
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_5);//��ʾ�˵�
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_4_5_1);//�����¼��˵�
						break;
					case 2:
						MenuCreate((u32)Menu_4_5_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu_4_5_3);//�����¼��˵�
						break;
					case 4:
						MenuCreate((u32)Menu_4_5_4);//�����¼��˵�
						break;
					case 5:
						MenuCreate((u32)Menu_4_5_5);//�����¼��˵�
						break;
					case 6:
						MenuCreate((u32)Menu_4_5_6);//�����¼��˵�
						break;
					case 7:
						MenuCreate((u32)Menu_4_5_7);//�����¼��˵�
						break;
					case 8:
						MenuCreate((u32)Menu_4_5_8);//�����¼��˵�
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
// Function name:       Menu_4_4_2
// Descriptions:        ��������޹�����ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_4_2(void)//�˵�4_4_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�����޹�����ʾֵ\n\r"
																		"������\n\r"
			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
#else
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//�����޹�����ʾֵ
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(132,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_4_1
// Descriptions:        ������й�����ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_4_1(void)//�˵�4_4_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�����й�����ʾֵ\n\r"
																		"������\n\r"
			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
#else
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//�����й�����ʾֵ
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(131,MenuCount->v2)+6+5*i,5);	                   
			if(y==0xEEEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			DisplayString(4+i,17,0,(u8 *)UnitList[2]);//2 kWh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_4
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_4(void)//�˵�4_4
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�������\n\r"
																		"  1.�����й�����ʾֵ\n\r"
																		"  2.�����޹�����ʾֵ\n\r"
			);//��ʾ�˵�
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
						MenuCreate((u32)Menu_4_4_1);
						break;
					case 2:
						MenuCreate((u32)Menu_4_4_2);
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
// Function name:       Menu_4_3_2
// Descriptions:        ��������޹�����ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_3_2(void)//�˵�4_3_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�����޹�����ʾֵ\n\r"
																		"������\n\r"
			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
#else
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//�����޹�����ʾֵ
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(130,MenuCount->v2)+6+4*i,4);	                   
			if(y==0xEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"---------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,8,2,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[3]);//2 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_3_1
// Descriptions:        ��������й�����ʾֵ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_3_1(void)//�˵�4_3_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�����й�����ʾֵ\n\r"
																		"������\n\r"
			);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
#else
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//�����й�����ʾֵ
	#if ((Project/100)==2 && (USER/100)==5)//�Ϻ�������
		p8=(u8 *)(ADDR_AFN04F10+((MenuCount->v2-1)*LEN_AFN04F10_Pn)+5);
		if((p8[0]==21) && (p8[13]!=4))
		{//�Ϻ���Լ������2
/*
	����Ϻ��������������з�����Ϊ2ʱ������������1��Ӧ�Ϻ������ƽ������������2��Ӧ�Ϻ�����ʹȣ�
	�����������з�����Ϊ4ʱ������������1��4�ֱ��Ӧ��Ч���ݡ��Ϻ������ƽ����Ч���ݡ��Ϻ�����ʹȡ�
	�˷���˳���һ��Ͷ������ݾ���Ҫ��
*/			
			//��
			y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6,5);	                   
			if(y==0xEEEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			//��
			DisplayString(4+1,6,0,"-----------");	
			//ƽ
			y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+5,5);	                   
			if(y==0xEEEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+2,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+2)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			//��
			DisplayString(4+3,6,0,"-----------");	
			//��
			y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+10,5);	                   
			if(y==0xEEEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+4,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+4)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			for(i=0;i<5;i++)
			{
				DisplayString(4+i,17,0,(u8 *)UnitList[2]);//2 kWh
			}
		}
		else
		{
			for(i=0;i<5;i++)
			{
				y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+5*i,5);	                   
				if(y==0xEEEEEEEEEE)
				{//�޴�������-----------
					DisplayString(4+i,6,0,"-----------");	
				}
				else
				{
					p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
					DisplayData_OffZero(y,10,4,p16);
				}
				DisplayString(4+i,17,0,(u8 *)UnitList[2]);//2 kWh
			}
		}
	#else
		for(i=0;i<5;i++)
		{
			y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+5*i,5);	                   
			if(y==0xEEEEEEEEEE)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,10,4,p16);
			}
			DisplayString(4+i,17,0,(u8 *)UnitList[2]);//2 kWh
		}
	#endif
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_3
// Descriptions:        ����������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_3(void)//�˵�4_3
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�������\n\r"
																		"  1.�����й�����ʾֵ\n\r"
																		"  2.�����޹�����ʾֵ\n\r"
			);//��ʾ�˵�
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
						MenuCreate((u32)Menu_4_3_1);
						break;
					case 2:
						MenuCreate((u32)Menu_4_3_2);
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
// Function name:       MenuContent_4_2_1
// Descriptions:        ����޹�����(��1��)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuContent_4_2_1(void)//�˵�4_2_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_2);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���ϼ�����
					CreateScreen((u32)Menu_4_2);
				}
				Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,6);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,7);//����Ļ��ʾ��ָ����
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ReactivePowerList[i]);
		}
#else
		for(i=0;i<=5;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ReactivePowerList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
	
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//��ǰ�޹����� ��ABC
		for(i=0;i<=3;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+17+3*i,3);	       
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(4+i,8,0,"-------");	
			}
			else
			{
				DisplayString(4+i,7,0,(u8 *)SList[y>>23]);//+ -
				y&=0x7fffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+8*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(4+i,16,0,(u8 *)UnitList[1]);//1 kvar
		}
		UpArrow();
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_2
// Descriptions:        ����й�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_2(void)//�˵�4_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_2);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���¼�����
					CreateScreen((u32)MenuContent_4_2_1);
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif

	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<5;i++)
		{
			DisplayString(3+i,0,0,(u8 *)ActivePowerList[i]);
		}
#else
		for(i=0;i<=5;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ActivePowerList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(3*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(3,7,0,"------------");	
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//��ǰ�й����� ��ABC
		for(i=0;i<=3;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+5+3*i,3);	       
			if(y==0xEEEEEE)
			{//�޴�������-------
				DisplayString(4+i,8,0,"-------");	
			}
			else
			{
				DisplayString(4+i,7,0,(u8 *)SList[y>>23]);//+ -
				y&=0x7fffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+8*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(4+i,16,0,(u8 *)UnitList[0]);//0 kW
		}
		DownArrow();
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_1
// Descriptions:        ��ѹ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_1(void)//�˵�4_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
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
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4_1);//��ʾ�˵�
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif

	Ln+=2;
	if(MenuCount->v2<=MaxRS485AddCarrierPn && MenuCount->v2>=1)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		for(i=0;i<6;i++)
		{
			DisplayString(3+i,0,0,(u8 *)UIList[i]);
		}
#else
		for(i=0;i<=7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)UIList[i]);
		}
		//�ֱ��
		p8=(u8 *)ADDR_DATABUFF;						                              
		MC(0,ADDR_DATABUFF+12,1);
		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_AFN04F10_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);	   
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+(7*2));
			DisplayData(y,12,0,p16);
		}
		else
		{
			DisplayString(Ln,7,0,"------------");	
		}
		Ln++;
#endif
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//��ѹABC		
		for(i=0;i<=2;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+37+2*i,2);  
			if(y==0xEEEE)
			{//�޴�������-----
				DisplayString(Ln+i,8,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+i)*84+8*2);
				DisplayData_OffZero(y,4,1,p16);
			}
			DisplayString(Ln+i,14,0,(u8 *)UnitList[4]);//4 V
		}
		//����ABC
		Ln+=3;
		for(i=0;i<=2;i++)
		{
			y=MRR(GetClass1DataAddr(25,MenuCount->v2)+43+3*i,3);   
			if(y==0xEEEEEE)
			{//�޴�������------
				DisplayString(Ln+i,8,0,"------");	
			}
			else
			{
				DisplayString(Ln+i,7,0,(u8 *)SList[y>>23]);//+ -
				y&=0x7fffff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+i)*84+8*2);
				DisplayData_OffZero(y,6,3,p16);
			}
			DisplayString(Ln+i,14,0,(u8 *)UnitList[5]);//4 A
		}
	}
	else
	{
		DisplayString(Ln,4,0,"��Ч�������");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4
// Descriptions:        ���ܱ�ʾ��/������������������ʾ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4(void)//�˵�4
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
#if ((Project/100)!=2)//2Ϊ������
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4);//��ʾ�˵�
#else
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_4);//��ʾ�˵�
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
						MenuCreate((u32)Menu_4_1);
						break;
					case 2:
						MenuCreate((u32)Menu_4_2);
						break;
					case 3:
						MenuCreate((u32)Menu_4_3);
						break;
					case 4:
						MenuCreate((u32)Menu_4_4);
						break;
					case 5:
						MenuCreate((u32)Menu_4_5);
						break;
					case 6:
						MenuCreate((u32)Menu_4_6);
						break;
					case 7:
#if ((Project/100)==2)//2Ϊ������
						MenuCreate((u32)Menu_4_7);//�������޹�����
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

