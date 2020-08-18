
//�˵�4 ���ܱ�ʾ��
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu4.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_Record.h"

#include "../MS/msFILE.h"
#include "../OS/OS.h"
#include "../MS/MS.h"

extern const u8 UnitList[][6];//ͨ�õ�λ�б�
extern const u8 SList[][2];//���������б�
extern u32 RCSDtoID(u8* pRCSD);//��¼��ѡ��RCSD���Ӧ�������,����:�ϱ������ڲɼ�����������,>=RMTASKmax��ʾ��Ч�����
extern u32 RCSDtoNextProjectID(u8* pRCSD,u32 ProjectID);//��¼��ѡ��RCSD���Ӧ���¸�������,���:ProjectID=��ʼ�����ķ�����;����:������,>=NUMmax_6014��ʾ��Ч������
extern u32 ProjectIDtoTaskID(u32 ProjectID);//��ͨ�ɼ�������תΪ�����;����:�ɼ�����������,>=RMTASKmax��ʾ��Ч�����

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
	"���ú�\n\r"
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
	"���ú�\n\r"
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
//	"  2.���������޹�\n\r"
	"  2.���·����й�\n\r"
//	"  4.���·����޹�\n\r"
//	"  5.���������й�\n\r"
//	"  6.���������޹�\n\r"
//	"  7.���·����й�\n\r"
//	"  8.���·����޹�\n\r"
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


u32 Check_6000_Pn(u32 Pn)//�ж��������Pn�Ƿ��д�����Ч�Ĳ��� ������=����0  ���ڷ��� 1
{
	u32 i;
	u32 Addr;
//	u32 Meter;
	u8 *p;
	if(Pn>=NUMmax_6000)
	{
		return 0;
	}
	Addr=ADDR_6000+(LENper_6000*Pn);
	MR(ADDR_DATABUFF,Addr,5);
	p=(u8 *)ADDR_DATABUFF;
	if(p[0]==0)// ����������NULL  02 04 12 
	{
		return 0;
	}
	Data_Inverse(ADDR_DATABUFF+3,2);
	i=MRR(ADDR_DATABUFF+3,2);
	if(i!=Pn)
	{
		return 0;
	}
	return 1;
}

u64 Get_TSA_BY_PN_noInverse_menu(u32 Pn)//����������ţ��õ�TSA���������������͡�������˳��
{

	u32 i;
	u32 Addr;
	u64 Meter;
	u8 *p8;
	
	i=Check_6000_Pn(Pn);
	if(i==0)
	{
		return INVALID_DATA;
	}
	Addr=ADDR_6000+LENper_6000*Pn;
	
	p8=Get_Element((u8 *)Addr,2,1,LENper_6000);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
	if(p8==0)
	{
		return INVALID_DATA;
	}
	if(p8[1]==0)
	{
		return INVALID_DATA;
	}
	MR(ADDR_DATABUFF,(u32)p8+3,(p8[1]-1));
	if((p8[1]-1)>8)
		Meter=MRR(ADDR_DATABUFF,8);
	else
		Meter=MRR(ADDR_DATABUFF,(p8[1]-1));
	return Meter;
}

u64 Get_TSA_BY_PN(u32 Pn)//����������ţ��õ�TSA���������������͡�
{

	u32 i;
	u32 Addr;
	u64 Meter;
	u8 *p8;
	i=Check_6000_Pn(Pn);
	if(i==0)
	{
		return INVALID_DATA;
	}
	Addr=ADDR_6000+LENper_6000*Pn;
	
	p8=Get_Element((u8 *)Addr,2,1,LENper_6000);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
	if(p8==0)
	{
		return INVALID_DATA;
	}
	MR(ADDR_DATABUFF,(u32)p8+3,(p8[1]-1));
	Data_Inverse(ADDR_DATABUFF,(p8[1]-1));
	if((p8[1]-1)>8)
		Meter=MRR(ADDR_DATABUFF,8);
	else
		Meter=MRR(ADDR_DATABUFF,(p8[1]-1));
	return Meter;
}
//numֻ�ڶ�ȡ����ʱ��Ч
//Ŀǰֻ֧�ֶ�ȡ��ǰ���ݣ�roadһֱ����0
u64 GetData(u32 road,u32 oad,u32 num,u32 pn)//��ȡ���ݣ�road=0��ʾ��ǰ����pn ���������,��������ǽṹ��num��ʾ�ڼ�����Ա
{
	u64 res=INVALID_DATA;
	u8 pRCSD[]={DataType_RCSD,0x01,0x00,0x00,0x00,0x00,0x00};
	u32 task_id;//����oad�������
	u32 data_addr;//���һ����¼oad���ݵĴ�ŵ�ַ
	u32 data_len;
	u64 tsa;
	u8 *p_data;
	u8 *pp;
	u32 index=0;
	u32 ProjectID;
	tsa=Get_TSA_BY_PN_noInverse_menu(pn);
	if(tsa==INVALID_DATA)//��ȡ�������ַʧ��
	{
		return INVALID_DATA;
	}
	//��һ�� ����oad ����ȫ���ɵ����ݡ�
	pRCSD[3]=(oad>>24)&0xff;
	pRCSD[4]=(oad>>16)&0xff;
	pRCSD[5]=(oad>>8)&0xff;

	if((Terminal_Ram->res13==(oad&0xffffff00))&&(Terminal_Ram->res14==num)&&(Terminal_Ram->res15==pn))
	{
		if(Terminal_Ram->rse16==INVALID_DATA)
			return INVALID_DATA;
		data_addr=Get_LastRecordFileData(Terminal_Ram->rse16,(u16* )&tsa);//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF�����ݳ��ȣ�0=û�ҵ�
		if(data_addr==0)
			return INVALID_DATA;
		data_len=Get_RecordFileData_RCSD(pRCSD+1,(u8 * )&tsa,(u8 * )ADDR_128KDATABUFF,(u8 *)ADDR_DATABUFF,1024,0);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
		if(data_len==0)
			return INVALID_DATA;
		goto read;
	}
	Terminal_Ram->res13=(oad&0xffffff00);
	Terminal_Ram->res14=num;
	Terminal_Ram->res15=pn;
	//�������ֺ�ʱ�����أ����Һ������������Ӧ����ʱ.
	//�����ס֮ǰ����Ĳ��������������ͬ����ֱ�ӴӼ�¼������task_id��ȥ�����ݣ�
	//�����ͬ���ʹ�ͷ��һ�顣
	for(index=0;index<NUMmax_6014;index++)
	{
		ProjectID=RCSDtoNextProjectID(pRCSD,index);//��¼��ѡ��RCSD���Ӧ���¸�������,���:ProjectID=��ʼ�����ķ�����;����:������,>=NUMmax_6014��ʾ��Ч������
		if(ProjectID>=NUMmax_6014)
		{
			continue;
		}
		index=ProjectID;
		task_id=ProjectIDtoTaskID(ProjectID);//��ͨ�ɼ�������תΪ�����;����:�ɼ�����������,>=RMTASKmax��ʾ��Ч�����
		if(task_id>=RMTASKmax)
		{
			continue;
		}
		data_addr=Get_LastRecordFileData(task_id,(u16* )&tsa);//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF�����ݳ��ȣ�0=û�ҵ�
		if(data_addr==0)//�����ݼ�¼����û�и�TSA������
		{
			continue;
		}
		data_len=Get_RecordFileData_RCSD(pRCSD+1,(u8 * )&tsa,(u8 * )ADDR_128KDATABUFF,(u8 *)ADDR_DATABUFF,1024,0);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
		if(data_len==0)
		{
			continue;
		}				
		break;
	}
	if(index>=NUMmax_6014)
	{
		Terminal_Ram->rse16=INVALID_DATA;
		return INVALID_DATA;
	}
	Terminal_Ram->rse16=task_id;
	//���⣬���ݶ��Ѿ�����ADDR_DATABUFF

	
	/*task_id=RCSDtoID(pRCSD);//��¼��ѡ��RCSD���Ӧ�������,����:�ϱ������ڲɼ�����������,>=RMTASKmax��ʾ��Ч�����
	if(task_id>=RMTASKmax)//Ҫ��ѯ��OAD�����κ�һ���������棬�϶���û�����ݵġ�
	{
		return INVALID_DATA;
	}
	data_addr=Get_LastRecordFileData(task_id,(u16* )&tsa);//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF�����ݳ��ȣ�0=û�ҵ�
	if(data_addr==0)//�����ݼ�¼����û�и�TSA������
	{
		return INVALID_DATA;
	}
	data_len=Get_RecordFileData_RCSD(pRCSD+1,(u8 * )&tsa,(u8 * )ADDR_128KDATABUFF,(u8 *)ADDR_DATABUFF,1024,0);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
	if(data_len==0)
	{
		return INVALID_DATA;
	}*/
	//���⣬���ݶ��Ѿ�����ADDR_DATABUFF
	//
read:	
	p_data=(u8 *)ADDR_DATABUFF;
	if(p_data[0]==DataType_array)//��Ϊǰ����ѡʱ��ѡȫ���ģ�������Ӧ��������ġ�
	{
		pp=Get_Element(p_data,oad&0xff,0,LEN_DATABUFF);
		if(pp!=0)
		{
			switch(pp[0])
			{
				case DataType_long_unsigned:
				case DataType_long:
					res=(pp[1]<<8)+pp[2];
					res=hex_bcd(res);
					break;
				case DataType_double_long:
				case DataType_double_long_unsigned:
					res=(pp[1]<<24)+(pp[2]<<16)+(pp[3]<<8)+pp[4];
					if(((oad>>16)==0x2004)||((oad>>16)==0x2005))//���ʵ�λ��һ������ʾ��KWH
					{
						res/=1000;
					}
					res=hex_bcd(res);
					break;
				case DataType_structure://��ֻ���� ����
					switch(num)
					{
						case 1://��������
							res=(pp[3]<<24)+(pp[4]<<16)+(pp[5]<<8)+pp[6];
							res=hex_bcd(res);
							break;
						case 3://����ʱ�� ��-��
							res=(hex_bcd(pp[10])<<8)+hex_bcd(pp[11]);
							break;
						case 2://����ʱ�� ʱ-��
							res=(hex_bcd(pp[12])<<8)+hex_bcd(pp[13]);
							break;
						default:
							res=INVALID_DATA;
					}
					break;
			}
		}
		else
		{
			res=INVALID_DATA;
		}
	}
	else
	{
		res=INVALID_DATA;
	}
	return res;
}

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
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUM_RMmax-1;
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//��ʾ�ַ���
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			y=GetData(0,0x00800200+1+i,0,MenuCount->v2);
			//y=MRR(GetClass1DataAddr(136,MenuCount->v2)+6+4*i,4);	                   
			if(y==INVALID_DATA)
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
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUM_RMmax-1;
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//��ʾ�ַ���
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			//y=MRR(GetClass1DataAddr(135,MenuCount->v2)+6+4*i,4);	   
			y=GetData(0,0x00700200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUM_RMmax-1;
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//��ʾ�ַ���
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	  
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
			//y=MRR(GetClass1DataAddr(134,MenuCount->v2)+6+4*i,4);
			y=GetData(0,0x00600200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			if(MenuCount->v2>=NUM_RMmax)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUM_RMmax-1;
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
	{
		for(i=0;i<7;i++)
		{
			DisplayString(2+i,0,0,(u8 *)ElectricEnergyList[i]);
		}
		//�ֱ��
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//��ʾ�ַ���
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(133,MenuCount->v2)+6+4*i,4);	 
			y=GetData(0,0x00500200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif

		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+29+2*i,2);	 
			y=GetData(0,0x200A0200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-----
				DisplayString(4+i,8,0,"-----");	
			}
			else
			{
				DisplayString(4+i,7,0,(u8 *)SList[y>>15]);//+ -
				y&=0x7fff;
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+8*2);
				DisplayData_OffZero(y,4,3,p16);
			}
			DisplayString(4+i,15,0,(u8 *)UnitList[7]);//�޷���
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�������");	
	}
}
/*

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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
			//y=MRR(GetClass1DataAddr(152,MenuCount->v2)+27+7*i,3);	   
			y=GetData(0X50060200,0x200A0200+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(152,MenuCount->v2)+30+7*i,2);	
			y=GetData(0X50060200,0x200A0200+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			//y=MRR(GetClass1DataAddr(152,MenuCount->v2)+32+7*i,2);	  
			y=GetData(0X50060200,0x200A0200+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
		//p8=(u8 *)ADDR_DATABUFF;						                              
		//MC(0,ADDR_DATABUFF+12,1);
		//MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
		//DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
*/
/*
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
*/
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+27+7*i,3);	
			y=GetData(0,0x10200200+1+3+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+30+7*i,2);
			y=GetData(0,0x10200200+1+3+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+32+7*i,2);	 
			y=GetData(0,0x10200200+1+3+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+6+7*i,3);
			y=GetData(0,0x10200200+1+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+9+7*i,2);	  
			y=GetData(0,0x10200200+1+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			//y=MRR(GetClass1DataAddr(147,MenuCount->v2)+11+7*i,2);	  
			y=GetData(0,0x10200200+1+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
/*
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	     
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
*/
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+18*2);
	p16[0]=0x0;
	p16[1]=0x0;
	DisplayClrRow(Ln+2,Ln+7);//����Ļ��ʾ��ָ����
	Ln+=2;
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	    
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
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+27+7*i,3);	   
			y=GetData(0,0x10100200+1+3+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-------
				DisplayString(Ln+2*i,5,0,"-------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2*i)*84+5*2);
				DisplayData_OffZero(y,6,4,p16);
			}
			DisplayString(Ln+2*i,13,0,(u8 *)UnitList[0]);//0 kW
			//����ʱ�� ʱ:�� ����ʱ�� ��-��
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+30+7*i,2);	
			y=GetData(0,0x10100200+1+3+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+32+7*i,2);	  
			y=GetData(0,0x10100200+1+3+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+6+7*i,3);
			y=GetData(0,0x10100200+1+i,1,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+9+7*i,2);	 
			y=GetData(0,0x10100200+1+i,2,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-----
				DisplayString(Ln+1+2*i,13,0,"-----");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+1+2*i)*84+13*2);
				DisplayData1(y,4,2,':',p16);
			}
			//����ʱ�� ��-��
			//y=MRR(GetClass1DataAddr(145,MenuCount->v2)+11+7*i,2);	 
			y=GetData(0,0x10100200+1+i,3,MenuCount->v2);
			if(y==INVALID_DATA)
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
					//case 2:
					//	MenuCreate((u32)Menu_4_5_2);//�����¼��˵�
					//	break;
					case 2:
						MenuCreate((u32)Menu_4_5_3);//�����¼��˵�
						break;
					//case 4:
					//	MenuCreate((u32)Menu_4_5_4);//�����¼��˵�
					//	break;
					//case 5:
					//	MenuCreate((u32)Menu_4_5_5);//�����¼��˵�
					//	break;
					//case 6:
					//	MenuCreate((u32)Menu_4_5_6);//�����¼��˵�
					//	break;
					//case 7:
					//	MenuCreate((u32)Menu_4_5_7);//�����¼��˵�
					//	break;
					//case 8:
					//	MenuCreate((u32)Menu_4_5_8);//�����¼��˵�
					//	break;
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	  	   
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
			//y=MRR(GetClass1DataAddr(132,MenuCount->v2)+6+4*i,4);	
			y=GetData(0,0x00400200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	  	   
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
			//y=MRR(GetClass1DataAddr(131,MenuCount->v2)+6+5*i,5);	  
			y=GetData(0,0x00200200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,10,2,p16);
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	 
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
			//y=MRR(GetClass1DataAddr(130,MenuCount->v2)+6+4*i,4);	
			y=GetData(0,0x00300200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(129,MenuCount->v2)+6+5*i,5);
			y=GetData(0,0x00100200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
			{//�޴�������-----------
				DisplayString(4+i,6,0,"-----------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
				DisplayData_OffZero(y,10,2,p16);
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
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

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	   
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+17+3*i,3);	
			y=GetData(0,0x20050200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			NdigitSelect(4,8,1,NUM_RMmax-1);
			break;
		default:
			break;
	}
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	DisplayClrRow(3,8);//����Ļ��ʾ��ָ����
#else
	DisplayClrRow(2,9);//����Ļ��ʾ��ָ����
#endif

	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(2,7,0,p8);//��ʾ�ַ���
#endif
		//���ַ
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	  
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+5+3*i,3);	
			y=GetData(0,0x20040200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUM_RMmax-1);
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
	if(MenuCount->v2<=NUM_RMmax && MenuCount->v2>=1)
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
//		p8=(u8 *)ADDR_DATABUFF;						                              
//		MC(0,ADDR_DATABUFF+12,1);
//		MR(ADDR_DATABUFF,ADDR_AFN04F29+12*(MenuCount->v2-1),12);
//		DisplayString(Ln,7,0,p8);//��ʾ�ַ���
		//���ַ
		Ln++;
		i=Check_6000_Pn(MenuCount->v2);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
		if(i!=0)
		{
			y=Get_TSA_BY_PN(MenuCount->v2);	 	   
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+37+2*i,2); 
			y=GetData(0,0x20000200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
			//y=MRR(GetClass1DataAddr(25,MenuCount->v2)+43+3*i,3);   
			y=GetData(0,0x20010200+1+i,0,MenuCount->v2);
			if(y==INVALID_DATA)
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
//	DisplayClr();//��ȫ��
//	DisplayLoop_Contrl(1);
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

