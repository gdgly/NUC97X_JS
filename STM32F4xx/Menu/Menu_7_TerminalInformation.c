
//�˵�7 �ն���Ϣ
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"


#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
extern const u8 ProtocolList[][13];//ͨ��Э���б�
extern const u8 CommunicationSpeedList[][6];//ͨ�������б�
//�ն���Ϣ
//��1�������1��Ϣ��ͨ����Э�顢���ַ��������
//��2�������2��Ϣ��ͨ����Э�顢���ַ��������
//��3������ͨ����Ϣ��GPRS/CDMA��:�ն˵�ַ����������-�ն˵�ַ����IP���˿ںš�APN���ź�ǿ��ֵ��
//��4��������汾�š�Ӳ���汾��
//��5������ͨ����Ϣ����̫�������ն˵�ַ����������-�ն˵�ַ��������IP�����ء��˿ںš���������
void Menu_7(void)
{
	u32 i;
	u32 j=0;
	u64 x;
	u32 Row;
	u8 *p8;
	u8 *p8s;
	u16 *p16a;
	u16 *p16;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"���1��Ϣ\n\r"
																	"������:\n\r"
																	"ͨ  ��:\n\r"
																	"Э  ��:\n\r"
																	"���ַ:\n\r"
																	);//��ʾ�˵�

	p16a=(u16*)ADDR_DATABUFF;
	for(i=1;i<=MaxRS485AddCarrierPn;i++)
	{
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+2,2);
		if(p16a[0]==1)
		{//�ҵ�������1
			break;
		}
	}
	if(i!=MaxRS485AddCarrierPn+1)
	{//������Ŵ���
		Row=2;
		//ͨ������
		p8=(u8*)ADDR_DATABUFF;
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+4,2);
		DisplayString(Row,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//��ʾͨ������
		//�˿ں�
		Row++;
		p8s=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
		bcd_p8(p8s,p8[0] & 0x1F,2);
		//ͨ��Э������
		Row++;
		switch(p8[1])
		{
			case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
				j++;
			case 30://DL/T 645-2007
				j++;
			case 21://�Ϻ���Լ
				j++;
			case 3://698.45
				j++;
			case 2://��������װ��ͨ��Э��
				j++;
			case 1://DL/T 645-1997
				j++;
			case 0://�ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
				break;
			default:
				j=6;
				break;
		}
		DisplayString(Row,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
		//ͨ�ŵ�ַ
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+8*2);
		x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);
		DisplayData(x,12,0,p16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7_1);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7_4);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}
void Menu_7_1(void)
{
	u32 i;
	u32 j=0;
	u64 x;
	u32 Row;
	u8 *p8;
	u8 *p8s;
	u16 *p16a;
	u16 *p16;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"���2��Ϣ\n\r"
																	"������:\n\r"
																	"ͨ  ��:\n\r"
																	"Э  ��:\n\r"
																	"���ַ:\n\r"
																	);//��ʾ�˵�

	p16a=(u16*)ADDR_DATABUFF;
	for(i=1;i<=MaxRS485AddCarrierPn;i++)
	{
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+2,2);
		if(p16a[0]==2)
		{//�ҵ�������2
			break;
		}
	}
	if(i!=MaxRS485AddCarrierPn+1)
	{//������Ŵ���
		Row=2;
		//ͨ������
		p8=(u8*)ADDR_DATABUFF;
		MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+4,2);
		DisplayString(Row,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//��ʾͨ������
		//�˿ں�
		Row++;
		p8s=(u8*)(ADDR_STRINGBUFF+Row*84+4+8*2);
		bcd_p8(p8s,p8[0] & 0x1F,2);
		//ͨ��Э������
		Row++;
		switch(p8[1])
		{
			case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
				j++;
			case 30://DL/T 645-2007
				j++;
			case 21://�Ϻ���Լ
				j++;
			case 3://698.45
				j++;
			case 2://��������װ��ͨ��Э��
				j++;
			case 1://DL/T 645-1997
				j++;
			case 0://�ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���
				break;
			default:
				j=5;
				break;
		}
		DisplayString(Row,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
		//ͨ�ŵ�ַ
		Row++;
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+8*2);
		x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(i-1)+6,6);
		DisplayData(x,12,0,p16);
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7_2);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}
void Menu_7_2(void)
{
	u32 x;
	u32 y;
	u32 Row;
	u32 i;
	u8 *p;
	u16 *p16;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"��ͨ��GPRS/CDMA\n\r"
																	"�ն˵�ַ:****-\n\r"
																	"IP:\n\r"
																	"�˿�:\n\r"
																	"APN:\n\r"
																	"�ź�ǿ��:\n\r"
																	);//��ʾ�˵�

	Row=2;
	//1.����������
	x=MRR(ADDR_AREACODE,2);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	DisplayData(x,4,0,p16);
	//2.�ն˵�ַ
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+14*2);
	x=MRR(ADDR_TERMINAL,2);
	y=hex_bcd(x);
	DisplayData(y,5,0,p16);
	//3.��վ��IP
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*5));
	for(i=0;i<4;i++)
	{
		x=MRR(ADDR_AFN04F3+i,1);
		x=hex_bcd(x);
		p16=DisplayData_OffZero(x,3,0,p16);
		p16[0]='.'+0x2000;
		p16++;
	}
	p16--;
	p16[0]=0;
	//4.��վ�˿ں�
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+5*2);
	p16=(u16*)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN04F3+4,2);
	bcd_p8(p,p16[0],5);
	//5.APN
	Row++;
	p=(u8 *)ADDR_DATABUFF;
	MC(0,ADDR_DATABUFF+16,1);
	MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
	DisplayString(Row,4,0,p);//��ʾ�ַ���
	//6.�ź�ǿ��
	Row++;
/*
	�ź�ǿ�ȣ��Ϻ���չ��
	�ź�ǿ��	�ź�Ʒ��
	   0        <=-113dBm
	   1        -111dBm
	   2        -109dBm
	  ...        ...
	   31        >=-51dBm
	   99       δ֪���޷���ȡ
*/
	x=Terminal_Ram->SignaldBm;//�ź�ǿ��0-0x31��0x99
	x=bcd_hex(x);
	if(x<=31)
	{
		if(x==0)
		{
			DisplayString(Row,9,0,"<=");//��ʾ�ַ���
		}
		if(x==31)
		{
			DisplayString(Row,9,0,">=");//��ʾ�ַ���
		}
		x*=2;
		x=113-x;
		x=hex_bcd(x);
		p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+11*2);
		p16[0]='-'+0x2000;
		DisplayData_OffZero(x,3,0,p16+1);
		DisplayString(Row,15,0,"dBm");//��ʾ�ַ���
	}
	else
	{
		DisplayString(Row,10,0,"----");//��ʾ�ַ���
	}

	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7_3);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7_1);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}
void Menu_7_3(void)
{
	u32 Row;
	u64 x;
	u8 *p8;
	u16 *p16;

	//��ȫ��
	DisplayClr();//��ȫ��
	Row=1;
	DisplayMenuString_SH(Row,0,0,(u8 *)
																	"����汾:\n\r"
																	"�������:\n\r"
																	"Ӳ���汾:\n\r"
																	"Ӳ������:\n\r"
																	"ICCID��:\n\r"
																	" \n\r"
//																	"SIM���ֻ���:\n\r"
//																	" \n\r"
																	);//��ʾ�˵��ַ���

	//1.����汾
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
	p16[0]='Z'+0x2000;
	p16[1]='Z'+0x2000;
	p16[2]=((SVER>>4)&0xf)+0x30+0x2000;
	p16[3]=((SVER>>0)&0xf)+0x30+0x2000;
	//�����������
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
	p16[0]=((SVDATE>>20)&0xf)+0x30+0x2000;
	p16[1]=((SVDATE>>16)&0xf)+0x30+0x2000;
	p16[2]='-'+0x2000;
	p16[3]=((SVDATE>>12)&0xf)+0x30+0x2000;
	p16[4]=((SVDATE>>8)&0xf)+0x30+0x2000;
	p16[5]='-'+0x2000;
	p16[6]=((SVDATE>>4)&0xf)+0x30+0x2000;
	p16[7]=((SVDATE>>0)&0xf)+0x30+0x2000;
	//2.Ӳ���汾
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
	p16[0]='V'+0x2000;
	p16[1]=((HVER>>8)&0xf)+0x30+0x2000;
	p16[2]='.'+0x2000;
	p16[3]=((HVER>>4)&0xf)+0x30+0x2000;
	p16[4]=((HVER>>0)&0xf)+0x30+0x2000;
	//Ӳ����������
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
	p16[0]=((HVDATE>>20)&0xf)+0x30+0x2000;
	p16[1]=((HVDATE>>16)&0xf)+0x30+0x2000;
	p16[2]='-'+0x2000;
	p16[3]=((HVDATE>>12)&0xf)+0x30+0x2000;
	p16[4]=((HVDATE>>8)&0xf)+0x30+0x2000;
	p16[5]='-'+0x2000;
	p16[6]=((HVDATE>>4)&0xf)+0x30+0x2000;
	p16[7]=((HVDATE>>0)&0xf)+0x30+0x2000;
	//ICCID��
	Row+=2;
	p8=(u8 *)ADDR_DATABUFF;
	MC(0,ADDR_DATABUFF+20,1);
	MR(ADDR_DATABUFF,ADDR_AFN09F9+4+8+4+3+4+3,20);
	DisplayString(Row,0,0,p8);
	//SIM���ֻ���
/*
	���ò���F243�նˣ�SIM�����ֻ����룺
	�ֻ�����	BCD	8
	--����16λǰ�油0������SIM���ֻ�����13800138000 ���ü�Ϊ "0000013800138000"
	ֻ��Ϊ��¼SIM���ֻ�������ն˶�Ӧ��ϵ
*/
	Row++;
	DisplayString(Row,0,0,"SIM���ֻ���:");
	Row++;
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+4*2);
	x=MRR(ADDR_AFN04F243,8);
	DisplayData_OffZero(x,16,0,p16);

	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7_4);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7_2);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}
void Menu_7_4(void)
{
	u32 x;
	u32 y;
	u32 i;
	u32 Row;
	u8 *p;
	u8 *p8;
	u16 *p16;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																	"��ͨ����̫��\n\r"
																	"�ն˵�ַ:****-\n\r"
																	"����IP:\n\r"
																	"\n\r"
																	"����:\n\r"
																	"����:\n\r"
																	"�˿�:\n\r"
																	);//��ʾ�˵�

	Row=2;
	//1.����������
	x=MRR(ADDR_AREACODE,2);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	DisplayData(x,4,0,p16);
	//2.�ն˵�ַ
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+14*2);
	x=MRR(ADDR_TERMINAL,2);
	y=hex_bcd(x);
	DisplayData(y,5,0,p16);

	//�ն�IP���������롢����
	p8=(u8 *)ADDR_DATABUFF;
	if(MRR(ADDR_AFN04F7+64,1)==0)
	{//0 �̶�
		MR(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//���ն�IP���������롢����
	}
	else
	{//1 �Զ�
		MR(ADDR_DATABUFF+1,ADDR_AFN0CF4+29,12);//���ն�IP���������롢����
	}
	//�ն�IP
	Row++;
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+5*2);
	for(i=0;i<=3;i++)
	{
		bcd_p8(p+i*8,p8[1+i],3);
	}
	for(i=0;i<=2;i++)
	{
		p[3*2+i*8]='.';
		p[3*2+i*8+1]=0x20;
	}
	//��������
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+5*2);
	for(i=0;i<=3;i++)
	{
		bcd_p8(p+i*8,p8[5+i],3);
	}
	for(i=0;i<=2;i++)
	{
		p[3*2+i*8]='.';
		p[3*2+i*8+1]=0x20;
	}
	//����
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+5*2);
	for(i=0;i<=3;i++)
	{
		bcd_p8(p+i*8,p8[9+i],3);
	}
	for(i=0;i<=2;i++)
	{
		p[3*2+i*8]='.';
		p[3*2+i*8+1]=0x20;
	}
	//�ն������˿�
	Row++;
	MR(ADDR_DATABUFF,ADDR_AFN04F7+20,1);//�ⲿFLASH��
	p8=(u8 *)ADDR_DATABUFF;
	i=20+1+p8[0];
	MR(ADDR_DATABUFF,ADDR_AFN04F7+i,1);//�ⲿFLASH��
	i+=1+p8[0];
	x=MRR(ADDR_AFN04F7+i,2);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
	y=hex_bcd(x);
	DisplayData(y,5,0,p16);

	if((Comm_Ram->ButtonVal&0x08)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7);//����
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		CreateScreen((u32)Menu_7_3);//����
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)//λ=1��ʾ����;B0=����,B1=����,B2=����,B3=����,B4=ȷ��,B5=�˳�
	{//����
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}
#else

extern const u8 UnitList[][6];//ͨ�õ�λ�б�
extern const u8 OnlineModeList[][11];//����ģʽ�б�
extern const u8 ConnectWayList[][6];//���ӷ�ʽ�б�

const u8 pMenu_7[]=
{
	"�ն���Ϣ\n\r"
	"  1.������Ϣ\n\r"
	"  2.������Ϣ\n\r"
	"  3.ר����Ϣ\n\r"
};

const u8 pMenu_7_1[]=
{
	"������Ϣ\n\r"
	"��������:\n\r"
	"�ն˵�ַ:\n\r"
	"���̴���:\n\r"
	"�豸���:\n\r"
	"����汾:\n\r"
	"Ӳ���汾:\n\r"
	"��ǰ����:\n\r"
	"��ǰʱ��:\n\r"
};

const u8 pMenu_7_2[]=
{
	"������Ϣ\n\r"
	"��IP:\n\r"
	"�˿�:\n\r"
	"��IP:\n\r"
	"�˿�:\n\r"
	"APN :\n\r"
};

const u8 pMenu_7_3[]=
{
	"ר����Ϣ\n\r"
	"����ģʽ:\n\r"
	"���ӷ�ʽ:\n\r"
	"�ز����:\n\r" 
	"�ز�����:\n\r"
	"����ʱ��:\n\r" 
	"��������:\n\r"
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_3
// Descriptions:        �ն���Ϣר����Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_7_3(void)
{
	u16 *p16;
	u32 i;
	u32 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_3);//��ʾ�˵�
	//����ģʽ
	x=MRR(ADDR_AFN04F8,1);
	DisplayString(1,10,0,(u8 *)OnlineModeList[x&0x03]);
	DisplayString(2,10,0,(u8 *)ConnectWayList[(x&0x80)>>7]);
	//�ز����
	x=MRR(ADDR_AFN04F8+1,2);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+10*2);
	p16=DisplayData_OffZero(x,5,0,p16);
	for(i=11;i<17;i++)
	{
		if(p16 == (u16 *)(ADDR_STRINGBUFF+3*84+4+i*2))
		{
			break;
		}
	}
	DisplayString(3,i,0,(u8 *)UnitList[9]);//9 s
	//�ز�����
	x=MRR(ADDR_AFN04F8+3,1);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
	p16=DisplayData_OffZero(x,3,0,p16);
	for(i=11;i<15;i++)
	{
		if(p16 == (u16 *)(ADDR_STRINGBUFF+4*84+4+i*2))
		{
			break;
		}
	}
	DisplayString(4,i,0,(u8 *)UnitList[10]);//10 ��
	//�Զ�����ʱ��
	x=MRR(ADDR_AFN04F8+4,1);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
	p16=DisplayData_OffZero(x,3,0,p16);
	for(i=11;i<15;i++)
	{
		if(p16 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
		{
			break;
		}
	}
	DisplayString(5,i,0,(u8 *)UnitList[8]);//8 min
	//��������
	x=MRR(ADDR_AFN04F1+5,1);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+10*2);
	p16=DisplayData_OffZero(x,3,0,p16);
	for(i=11;i<15;i++)
	{
		if(p16 == (u16 *)(ADDR_STRINGBUFF+6*84+4+i*2))
		{
			break;
		}
	}
	DisplayString(6,i,0,(u8 *)UnitList[8]);//8 min
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_2
// Descriptions:        �ն���Ϣ������Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_7_2(void)
{
	u8 *p;
	u16 *p16;
	u32 i;
	u32 x;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_2);//��ʾ�˵�
	//1.��վIP
	p16=(u16 *)(ADDR_STRINGBUFF+(84*1)+4+(2*5));
	for(i=0;i<4;i++)
	{
		x=MRR(ADDR_AFN04F3+i,1);
		x=hex_bcd(x);
		p16=DisplayData_OffZero(x,3,0,p16);
		p16[0]='.'+0x2000;
		p16++;
	}
	p16--;
	p16[0]=0;
	//3.����IP
	p16=(u16 *)(ADDR_STRINGBUFF+(84*3)+4+(2*5));
	for(i=0;i<4;i++)
	{
		x=MRR(ADDR_AFN04F3+6+i,1);
		x=hex_bcd(x);
		p16=DisplayData_OffZero(x,3,0,p16);
		p16[0]='.'+0x2000;
		p16++;
	}
	p16--;
	p16[0]=0;
	//2.��վ�˿ں�
	x=MRR(ADDR_AFN04F3+4,2);
	x=hex_bcd(x);
	p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+5*2);
	DisplayData_OffZero(x,5,0,p16);
	//4.���ö˿ں�
	x=MRR(ADDR_AFN04F3+10,2);
	x=hex_bcd(x);
	p16=(u16*)(ADDR_STRINGBUFF+4*84+4+5*2);
	DisplayData_OffZero(x,5,0,p16);
	//5.APN
	p=(u8 *)ADDR_DATABUFF;
	MC(0,ADDR_DATABUFF+16,1);
	MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
	DisplayString(5,5,0,p);//��ʾ�ַ���
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7_1
// Descriptions:        �ն���Ϣ������Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_7_1(void)
{
	u8 *p,*p8;
	u16 *p16;
	u32 x;
	u32 y;
	u32 i;
	u32 Row=1;

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7_1);//��ʾ�˵�
	//1.����������
	x=MRR(ADDR_AREACODE,2);
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	DisplayData(x,4,0,p16);
	Row++;
	//2.�ն˵�ַ
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	x=MRR(ADDR_TERMINAL,2);
	p16=DisplayData_OffZero(x,4,0,p16);
	p16[0]=0x2000+'H';
	p16[1]=0x2000+',';
	p16=p16+2;
	y=hex_bcd(x);
	p16=DisplayData_OffZero(y,5,0,p16);
	Row++;
	//3.���̴��š�4.�豸��š�5.����汾
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p8=(u8 *)ADDR_DATABUFF;
	MR(ADDR_DATABUFF,ADDR_AFN0CF1,4+8+4);
	for(i=0;i<=3;i++)
	{
		p[i*2]=p8[i];
		p[i*2+1]=0x20;
	}
	Row++;
	p=(u8*)(ADDR_STRINGBUFF+Row*84+4+9*2);
	for(i=0;i<=7;i++)
	{
		p[i*2]=p8[4+i];
		p[i*2+1]=0x20;
	}
	Row++;
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	for(i=0;i<=3;i++)
	{
		p[i*2]=p8[4+8+i];
		p[i*2+1]=0x20;
	}
	Row++;
	//6.Ӳ���汾
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
	p16[0]=0x2000+'V';
	p16[1]=(HVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((HVER>>4)&0xf)+0x2030;
	Row++;
	//7.��ǰ����
	p8=(u8 *)ADDR_TYMDHMS;
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
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
	Row++;
	//8.��ǰʱ��
	p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
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
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7
// Descriptions:        �ն���Ϣ�˵�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_7(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_7);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_7_1);//�����¼��˵�
						break;
					case 2:
						MenuCreate((u32)Menu_7_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu_7_3);//�����¼��˵�
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
