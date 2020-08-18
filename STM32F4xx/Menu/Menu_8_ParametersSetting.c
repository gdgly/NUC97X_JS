
//�˵�8 �ն˲�������
#include "Project.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"
#include "../terminal/Terminal_AFN0C_RealTimeData_Init.h"
#include "../terminal/Terminal_AFN0D_CongealData_Init.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"


//extern u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
//extern void PnVSAFN04F10SetNoList(void);//�������Pn��ӦAFN04F10��������б�
//extern void AFN04F150_Computer(void);//��������Ч��־����

extern const u8 UnitList[][6];//ͨ�õ�λ�б�
extern const u8 ProtocolList[][13];//ͨ��Э���б�

const u8 OnlineModeList[][11]=//����ģʽ�б�
{
	"���ݴ���",//�洢����Ϣ�д�
	"��������",
	"��������",
	"ʱ������",
};

const u8 ConnectWayList[][6]=//���ӷ�ʽ�б�
{
	"TCP",
	"UDP",
};

const u8 CommunicationSpeedList[][6]=//ͨ�������б�
{
	"Ĭ�� ",
	"600  ",
	"1200 ",
	"2400 ",
	"4800 ",
	"7200 ",
	"9600 ",
	"19200",
};

const u8 IpTypeList[][7]=//IP�����б�
{
	"�̶�",
	"�Զ�",
	"����",
};

const u8 pMenu_8[]=//ר�䡢���� ������
{
	"�ն˲�������\n\r"
	"  1.���ں�ʱ������\n\r"
	"  2.�ն˵�ַ����\n\r"
	"  3.ͨ�Ų�������\n\r"
	"  4.IP���˿ں�����\n\r"
	"  5.APN����\n\r"
	"  6.ר���û�������\n\r"
	"  7.ר����������\n\r"
	"  8.���ܱ�����\n\r"
	"  9.�ն���������\n\r"
};

const u8 pMenu1_8[]=//ר�䡢���� ������
{
	"�ն˲�������\n\r"
	"  1.���ں�ʱ������\n\r"
	"  2.�ն˵�ַ����\n\r"
	"  3.ͨ�Ų�������\n\r"
	"  4.IP���˿ں�����\n\r"
	"  5.APN����\n\r"
	"  6.ר���û�������\n\r"
	"  7.ר����������\n\r"
	"  8.���ܱ�����\n\r"
};

const u8 pMenu_8_2[]=
{
	"�ն˵�ַ����\n\r"
	"  1.������������\n\r"
	"  2.�߼���ַ����\n\r"
	"  3.�ն�IP����\n\r"
	"  4.MAC��ַ����\n\r"
	"  5.�豸�������\n\r"
	"  6.�ն������˿�\n\r"
};

const u8 pMenu_8_2_1[]=
{
	"������������\n\r"
	"��������:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_2_2[]=
{
	"�߼���ַ����\n\r"
	"  1.ʮ����\n\r"
	"  2.ʮ������\n\r"
};

const u8 pMenu_8_2_2_1[]=
{
	"�߼���ַ����\n\r"
	"�߼���ַ:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_2_3[]=
{
	"�ն�IP����\n\r"
	"����:\n\r"
	"IP  :\n\r"
	"����:\n\r"
	"����:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_2_4[]=
{
	"MAC��ַ����\n\r"
	"  1.ʮ����\n\r"
	"  2.ʮ������\n\r"
};

const u8 pMenu_8_2_4_1[]=
{
	"MAC��ַ����\n\r"
	"MAC:\n\r"
	" \n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_2_4_2[]=
{
	"MAC��ַ����\n\r"
	"MAC\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_2_5[]=
{
	"�豸�������\n\r"
	"�豸���\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_2_6[]=
{
	"�ն������˿�����\n\r"
	"�����˿�:\n\r"
	"      ȷ������\n\r"
};

#if ((Project/100)==2)//������
const u8 pMenu2_2[]=//����������������鿴
{
	"����������鿴\n\r"
	"  1.ͨ��ͨ������\n\r"
	"  2.���ܱ��������\n\r"
	"  3.������ʱ������\n\r"
	"  4.������������\n\r"
	"  5.���������\n\r"
};

const u8 pMenu_8_1[]=
{
	"������ʱ������\n\r"
	"����:\n\r"
	"ʱ��:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_3[]=
{
	"ͨ��ģʽ����\n\r"
	"����ģʽ:\n\r"
	"���ӷ�ʽ:\n\r"
	"�ز����:\n\r"
	"�ز�����:\n\r"
	"����ʱ��:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_4[]=
{
	"��վIP���˿�����\n\r"
	"��IP:\n\r"
	"�˿�:\n\r"
	"��IP:\n\r"
	"�˿�:\n\r"
	"����:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_8[]=
{
	"���ܱ��������\n\r"
	"�������\n\r"
	"������\n\r"
	"ͨ������\n\r"
	"ͨ�Ŷ˿�\n\r"
	"ͨ�Ź�Լ\n\r"
	"ͨ�ŵ�ַ\n\r"
	"�ɼ���\n\r"
	"�����\n\r"
	"С���\n\r"
	"      ȷ������\n\r"
};
#else
const u8 pMenu_8_1[]=
{
	"���ں�ʱ������\n\r"
	"����:\n\r"
	"ʱ��:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_3[]=
{
	"ͨ�Ų�������\n\r"
	"����ģʽ:\n\r"
	"���ӷ�ʽ:\n\r"
	"�ز����:\n\r"
	"�ز�����:\n\r"
	"����ʱ��:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_4[]=
{
	"IP���˿ں�����\n\r"
	"��IP:\n\r"
	"�˿�:\n\r"
	"��IP:\n\r"
	"�˿�:\n\r"
	"����:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu_8_8[]=
{
	"���ܱ�����\n\r"
	"�������\n\r"
	"������\n\r"
	"ͨ������\n\r"
	"ͨ�Ŷ˿�\n\r"
	"ͨ�Ź�Լ\n\r"
	"ͨ�ŵ�ַ\n\r"
	"�ɼ���\n\r"
	"�����\n\r"
	"С���\n\r"
	"      ȷ������\n\r"
};
#endif

const u8 pMenu_8_9[]=
{
	"�ն���������\n\r"
	"�˿ں�  :\n\r"
	"������  :\n\r"
	"��������:\n\r"
	"�����:\n\r"
	"      ȷ������\n\r"
};

const u8 PulseAttributeList[][20]=//���������б�
{
	"�����й�",
	"�����޹�",
	"�����й�",
	"�����޹�",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_1
// Descriptions:        ���ں�ʱ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_1(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/ )//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_1);//��ʾ�˵�
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=6;
			MenuCount->v2=13;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//����
			p8=(u8 *)ADDR_TYMDHMS;
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
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=3)
					{
						DisplayString(Ln+3,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=6;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=6;
							MenuCount->v2=13;
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
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
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
								if((p8[6*2]-0x30)*10+(p8[7*2]-0x30)*1>59)
								{
									p8[7*2]=0x30;
									p8[6*2]=0x30;
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
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
								if(p8[7*2]>0x30 && p8[7*2]<=0x39)
								{
									p8[7*2]--;
								}
								else if(p8[7*2]==0x30)
								{
									if(p8[6*2]>0x30 && p8[6*2]<=0x39)
									{
										p8[7*2]=0x39;
										p8[6*2]--;
									}
									else if(p8[6*2]==0x30)
									{
										p8[7*2]=0x39;
										p8[6*2]=0x35;
									}
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						if(Stack->MenuStack[MenuCount->Count].EditRow<3)
						{
							Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
							DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
						}
						else//if(Stack->MenuStack[MenuCount->Count].EditRow==3)
						{
							//�������õ�����
							DisplayClrRow_4(Ln+3);//����Ļ��ʾ��ָ����(4�������ֲ���)
							DisplayString(Ln+3,9,0,(u8*)"OK");
							p8=(u8 *)(ADDR_RTCBUFF);
							p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
							p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//��
							p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//��
							p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//ʱ
							p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
							p8[4]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//��
							p8[5]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//��
							p8[6]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//��
//				            WriteLPCRTC(ADDR_RTCBUFF);//д�ڲ�RTC
//				            SyncLPCRTC();//ͬ���ڲ�RTC(���ڲ�д�ⲿ)
							WriteRTC(ADDR_RTCBUFF);//дʵʱ��
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
// Function name:       Menu_8_2_1
// Descriptions:        ������������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_1(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_1);//��ʾ�˵�
			//��ʾ�ڴ�������
			//����������
			x=MRR(ADDR_AREACODE,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			DisplayData(x,4,0,p16);

			ArrowLR(Ln+1,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=9;
			MenuCount->v2=12;
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
						DisplayString(Ln+2,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=12;
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
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_add1(p8,4);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								p8_sub1(p8,4);
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//����������BCDռ2���ֽ�(0~65535) �ж��ڴ���������д����ʼ��
								for(i=0;i<=3;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=4)
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								//�������õ�����
								MR(ADDR_DATABUFF,ADDR_AREACODE,2);//������������
								p16=(u16 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p16[0]=(p[0]-0x30)*16*16*16+(p[2]-0x30)*16*16+(p[4]-0x30)*16+(p[6]-0x30)*1;
								MW(ADDR_DATABUFF,ADDR_AREACODE,2);//д����������
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
// Function name:       Menu_8_2_2_1
// Descriptions:        �߼���ַ���ã�ʮ���ƣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_2_1(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 y;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2_1);//��ʾ�˵�
			//��ʾ�ڴ�������
			//�߼���ַ
			x=MRR(ADDR_TERMINAL,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			y=hex_bcd(x);
			if(y==0)
			{
				DisplayString(Ln+1,9,0,(u8*)"��Ч");
			}
			else
			{
				DisplayData(y,5,0,p16);
			}

			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=9;
			MenuCount->v2=13;
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
						DisplayString(Ln+2,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=13;
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
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,1,5);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>1)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//�ն˵�ַBINռ2���ֽڣ�1~65535��0��ʾ��Ч��ַ �ж�Ϊ��Ч�����ʼ��
								for(i=0;i<=4;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=5)
								{
									bcd_p8(p8,1,4);
								}
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								//�������õ�����
								p16=(u16 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AREACODE+2,2);//���߼���ַ
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p16[0]=p8_bcd(p,5);
								MW(ADDR_DATABUFF,ADDR_AREACODE+2,2);//д�߼���ַ
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>65535 || x<1)
								{
									bcd_p8(p8,1,5);
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
// Function name:       Menu_8_2_2_2
// Descriptions:        �߼���ַ���ã�ʮ�����ƣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_2_2(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2_1);//��ʾ�˵�
			//��ʾ�ڴ�������
			//�߼���ַ
			x=MRR(ADDR_TERMINAL,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
			if(x==0)
			{
				DisplayString(Ln+1,9,0,(u8*)"��Ч");
			}
			else
			{
				p16 = DisplayData(x,4,0,p16);
				p16[0] = 0x2000+'H';
			}

			MenuCount->v1=9;
			MenuCount->v2=12;
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
						DisplayString(Ln+2,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=12;
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
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								//�������õ�����
								p8=(u8 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AREACODE+2,2);//���߼���ַ
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								for(i=0;i<=1;i++)
								{
									if((p[(0+2*i)*2]>=0x30) && (p[(0+2*i)*2]<=0x39))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[1-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[1-i]=(p[(0+2*i)*2]-0x30)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
									if((p[(0+2*i)*2]>=0x41) && (p[(0+2*i)*2]<=0x46))
									{
										if((p[(1+2*i)*2]>=0x30) && (p[(1+2*i)*2]<=0x39))
										{
											p8[1-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x30)*1;
										}
										if((p[(1+2*i)*2]>=0x41) && (p[(1+2*i)*2]<=0x46))
										{
											p8[1-i]=(p[(0+2*i)*2]-0x37)*16+(p[(1+2*i)*2]-0x37)*1;
										}
									}
								}
								MW(ADDR_DATABUFF,ADDR_AREACODE+2,2);//д�߼���ַ
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x13;//��ʹ�ú���0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x03;//��ʹ�ú���0
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								for(i=0;i<4;i++)
								{
									p16[i]=0x0;
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
// Function name:       Menu_8_2_2
// Descriptions:        �߼���ַ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
#if ((USER/100)==11)//���ּ�����
void Menu_8_2_2(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u64 x;
	u64 y;
	u32 Ln;

	if(Comm_Ram->Factory==0x55)
	{//����״̬
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2);//��ʾ�˵�
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
							MenuCreate((u32)Menu_8_2_2_1);//�����¼��˵�            
							break;
						case 2:
							MenuCreate((u32)Menu_8_2_2_2);//�����¼��˵�
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
	{//�ǹ���״̬
		Ln=0;//�˵��ӵ�0�п�ʼ
//		Ln=1;//�˵��ӵ�1�п�ʼ		   //20160624
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2_1);//��ʾ�˵�
				//��ʾ�ڴ�������
				//����ͨ�ŵ�ַ
				x=MRR(ADDR_04000401,6);
				p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
				DisplayData(x,8,0,p16);
	
				ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
				MenuCount->v1=9;
				MenuCount->v2=16;
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
							DisplayString(Ln+2,6,0,(u8*)"ȷ������");
						}
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								MenuCount->v1=9;
								MenuCount->v2=16;
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
							default:
								break;
						}
						if((Comm_Ram->ButtonVal&0x08)!=0)
						{//�Ҽ�
							switch(Stack->MenuStack[MenuCount->Count].EditRow)
							{
								case 1:
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
									x=p8_bcd(p8,5);
									if(x<99999999)
									{
										p8_add1(p8,8);
									}
									else
									{
										bcd_p8(p8,1,8);
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
									x=p8_bcd(p8,8);
									if(x>1)
									{
										p8_sub1(p8,8);
									}
									else
									{
										bcd_p8(p8,99999999,8);
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
									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18,0,(u8*)"��");
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
									//�ն˵�ַBCD�� 4�ֽ� �ж�Ϊ��Ч�����ʼ��
									for(i=0;i<=7;i++)
									{
										if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
										{
											break;
										}
									}
									if(i!=8)
									{
										bcd_p8(p8,1,8);
									}
									break;
								case 2:
									DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
									DisplayString(Ln+2,9,0,(u8*)"OK");
									//�������õ�����
									p8=(u8 *)ADDR_DATABUFF;
									MR(ADDR_DATABUFF,ADDR_04000401,6);
									x=MRR(ADDR_04000401,6);//������ͨ�ŵ�ַ
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									y=p8_bcd(p,8);
									y=hex_bcd16(y);
									x=(x&0xffff00000000)+y;
									MWR(x,ADDR_04000401,6);
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
									p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+18*2);
									p16[0]=32;//'���Ҽ�ͷ'
									p16[1]=33;
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
									x=p8_bcd(p8,8);
									if(x>99999999 || x<1)
									{
										bcd_p8(p8,1,8);
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
}
#else
void Menu_8_2_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_2);//��ʾ�˵�
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
						MenuCreate((u32)Menu_8_2_2_1);//�����¼��˵�            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2_2_2);//�����¼��˵�
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
// Function name:       Menu_8_2_3
// Descriptions:        �ն�IP����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_3(void)
{
	u8 *p;
	u8 *p8;
	u8 *p8d;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Row=1;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_3);//��ʾ�˵�
			//��ʾ�ڴ�������
			//��ȡ����
			p8=(u8 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F7+64,1);
			if(p8[0]==0 || p8[0]==1)
			{
				DisplayString(Row,5,0,(u8 *)IpTypeList[p8[0]]);
			}
			else
			{
				DisplayString(Row,5,0,(u8 *)IpTypeList[2]);
			}
			Row++;
			
			if(MRR(ADDR_AFN04F7+64,1)==0)
			{//0 �̶�
				MR(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//���ն�IP���������롢����
			}
			else
			{//1 �Զ�
				MR(ADDR_DATABUFF+1,ADDR_AFN0CF4+29,12);//���ն�IP���������롢����
			}
			//�ն�IP
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
			Row++;
			//��������
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
			Row++;
			//����
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

			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=5;
			MenuCount->v2=8;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=5)
					{
						DisplayString(Ln+5,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=8;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
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
								p8d=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8=(u8 *)ADDR_DATABUFF;
								switch(p8d[0])
								{
									case 0x8E://�̶�
										i=1;
										MR(ADDR_DATABUFF+1,ADDR_AFN0CF4+29,12);//���ն�IP���������롢����
										break;
									case 0xA6://�Զ�
										i=0;
										MR(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//���ն�IP���������롢����
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)IpTypeList[i]);
								//�ն�IP
								Row=Ln+2;
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
								Row++;
								//��������
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
								Row++;
								//����
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
								p8d=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8=(u8 *)ADDR_DATABUFF;
								switch(p8d[0])
								{
									case 0x8E://�̶�
										i=1;
										MR(ADDR_DATABUFF+1,ADDR_AFN0CF4+29,12);//���ն�IP���������롢����
										break;
									case 0xA6://�Զ�
										i=0;
										MR(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//���ն�IP���������롢����
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)IpTypeList[i]);
								//�ն�IP
								Row=Ln+2;
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
								Row++;
								//��������
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
								Row++;
								//����
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//IP��ȡ����BINռ1���ֽڣ�0�̶�1�Զ������ж��ڴ������д� ���ʼ��
								if(p8[0]==0x24)
								{//����IP����
									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v1,0,(u8 *)IpTypeList[0]);
								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x8E)
								{//�̶�IP�������� �Զ�����
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x8E)
								{//�̶�IP�������� �Զ�����
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x8E)
								{//�̶�IP�������� �Զ�����
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+5,9,0,(u8*)"OK");
								Row=1;
								Row+=Ln;
								//�������õ�����
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								if(p[0]==0x8E)
								{//�̶�
									p8[0]=0x00;
								}
								if(p[0]==0xA6)
								{//�Զ�
									p8[0]=0x01;
								}
								MW(ADDR_DATABUFF,ADDR_AFN04F7+64,1);//дIP����
								
								Row++;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[1+i]=p8_bcd(p+8*i,3);
								}
								Row++;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[5+i]=p8_bcd(p+8*i,3);
								}
								Row++;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[9+i]=p8_bcd(p+8*i,3);
								}
								if(p8[0]==0)
								{//�̶�
									MW(ADDR_DATABUFF+1,ADDR_AFN04F7,12);//д�ն�IP���������롢����
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
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
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
// Function name:       Menu_8_2_4_1
// Descriptions:        MAC��ַ����(ʮ����)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_4_1(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Row=1;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_4_1);//��ʾ�˵�
			//��ʾ�ڴ�������
			//MAC			
			p8=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
			MR(ADDR_DATABUFF,ADDR_TERMINAL+10,6);//��MAC
			for(i=0;i<3;i++)
			{
				p16 = DisplayData(hex_bcd(p8[i]),3,0,p16);
				p16[0] = 0x2000+'-';
				p16++;
			}
			p16--;
			p16[0]=0x0;
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
			for(i=0;i<3;i++)
			{
				p16 = DisplayData(hex_bcd(p8[3+i]),3,0,p16);
				p16[0] = 0x2000+'-';
				p16++;
			}
			p16--;
			p16[0]=0x0;			
			MenuCount->v1=5;
			MenuCount->v2=15;
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
						DisplayClrRow_4(Ln+3);//����Ļ��ʾ��ָ����(4�������ֲ���)
						DisplayString(Ln+3,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=15;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=15;
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//�ж��ڴ������д� ���ʼ��
								for(i=0;i<3;i++)
								{
									for(k=4*i;k<=4*i+2;k++)
									{
										if(p8[k*2]<0x30 || p8[k*2]>0x39 ||p8[k*2+1]!=0x20)
										{
											break;
										}
									}
									if(k!=4*i+3)
									{
										break;
									}
								}
								if(i!=3)
								{
									for(i=0;i<3;i++)
									{
										for(k=4*i;k<=4*i+2;k++)
										{
											p8[k*2]=0x30;
										}
									}
								}
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//�ж��ڴ������д� ���ʼ��
								for(i=0;i<3;i++)
								{
									for(k=4*i;k<=4*i+2;k++)
									{
										if(p8[k*2]<0x30 || p8[k*2]>0x39 ||p8[k*2+1]!=0x20)
										{
											break;
										}
									}
									if(k!=4*i+3)
									{
										break;
									}
								}
								if(i!=3)
								{
									for(i=0;i<3;i++)
									{
										for(k=4*i;k<=4*i+2;k++)
										{
											p8[k*2]=0x30;
										}
									}
								}
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 3:
								//�������õ�����
								Row=1;
								Row+=Ln;
								MR(ADDR_DATABUFF,ADDR_AREACODE+12,6);//��MAC
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<3;i++)
								{
									p8[i]=p8_bcd(p+8*i,3);
								}
								Row++;
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
								for(i=0;i<3;i++)
								{
									p8[3+i]=p8_bcd(p+8*i,3);
								}
								Row++;
								DisplayClrRow_4(Row);//����Ļ��ʾ��ָ����(4�������ֲ���)
								if(IC_24AAxxx==0 && (p8[0]&0x01)==0)
								{
									MW(ADDR_DATABUFF,ADDR_AREACODE+12,6);//дMAC
									DisplayString(Row,9,0,(u8*)"OK");
								}
								else
								{
									if(IC_24AAxxx)
									{//IC_24AAxxxΪ0��ʾ�����ã�1��ʾ����ΨһMAC���ʲ�������
										DisplayString(Row,2,0,(u8*)"MACΨһ��������");
									}
									else
									{//(p8[0]&0x01)==1 �鲥��ַ
										DisplayString(Row,1,0,(u8*)"���ֽ�Ϊ����������");
									}
								}
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x13;//��ʹ�ú���0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x03;//��ʹ�ú���0
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
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								for(i=0;i<8;i++)
								{
									p8[11*2+i]=0x0;
								}
								if((p8[2*2]-0x30)&0x01)
								{//0������ַ 1�鲥��ַ MAC��ַΪ������ַ ���ֽ�ĩλ���ǵ���/�鲥��ַ
									p8[2*2]=p8[2*2]-1;
								}
								break;
							case 2:
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								for(i=0;i<8;i++)
								{
									p8[11*2+i]=0x0;
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
// Function name:       Menu_8_2_4_2
// Descriptions:        MAC��ַ���ã�ʮ�����ƣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_4_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 Row=1;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_4_2);//��ʾ�˵�
			//��ʾ�ڴ�������
			//MAC			
			p8=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+3*2);
			MR(ADDR_DATABUFF,ADDR_TERMINAL+10,6);//��MAC
			for(i=0;i<6;i++)
			{
				p16 = DisplayData(p8[i],2,0,p16);
				p16[0] = 0x2000+'-';
				p16++;
			}
			p16--;
			p16[0]=0x0;
			MenuCount->v1=3;
			MenuCount->v2=19;
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
						DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
						DisplayString(Ln+2,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=3;
							MenuCount->v2=19;
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//�ж��ڴ������д� ���ʼ��
								for(i=0;i<=5;i++)
								{
									for(k=3*i;k<=3*i+1;k++)
									{
										if(p8[k*2]<0x30 || (p8[k*2]>0x39 && p8[k*2]<0x41) || p8[k*2]>0x46 ||p8[k*2+1]!=0x20)
										{
											break;
										}
									}
									if(k!=3*i+2)
									{
										break;
									}
								}
								if(i!=6)
								{
									for(i=0;i<=5;i++)
									{
										for(k=3*i;k<=3*i+1;k++)
										{
											p8[k*2]=0x30;
										}
									}
								}
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//�������õ�����
								Row=1;
								Row+=Ln;
								MR(ADDR_DATABUFF,ADDR_AREACODE+12,6);//��MAC
								p=(u8 *)(ADDR_STRINGBUFF+Row*84+4+3*2);
								p8=(u8 *)ADDR_DATABUFF;
								for(i=0;i<=5;i++)
								{
									if((p[(0+3*i)*2]>=0x30) && (p[(0+3*i)*2]<=0x39))
									{
										if((p[(1+3*i)*2]>=0x30) && (p[(1+3*i)*2]<=0x39))
										{
											p8[i]=(p[(0+3*i)*2]-0x30)*16+(p[(1+3*i)*2]-0x30)*1;
										}
										if((p[(1+3*i)*2]>=0x41) && (p[(1+3*i)*2]<=0x46))
										{
											p8[i]=(p[(0+3*i)*2]-0x30)*16+(p[(1+3*i)*2]-0x37)*1;
										}
									}
									if((p[(0+3*i)*2]>=0x41) && (p[(0+3*i)*2]<=0x46))
									{
										if((p[(1+3*i)*2]>=0x30) && (p[(1+3*i)*2]<=0x39))
										{
											p8[i]=(p[(0+3*i)*2]-0x37)*16+(p[(1+3*i)*2]-0x30)*1;
										}
										if((p[(1+3*i)*2]>=0x41) && (p[(1+3*i)*2]<=0x46))
										{
											p8[i]=(p[(0+3*i)*2]-0x37)*16+(p[(1+3*i)*2]-0x37)*1;
										}
									}
								}
								Row++;
								DisplayClrRow_4(Row);//����Ļ��ʾ��ָ����(4�������ֲ���)
								if(IC_24AAxxx==0 && (p8[0]&0x01)==0)
								{
									MW(ADDR_DATABUFF,ADDR_AREACODE+12,6);//дMAC
									DisplayString(Row,9,0,(u8*)"OK");
								}
								else
								{
									if(IC_24AAxxx)
									{//IC_24AAxxxΪ0��ʾ�����ã�1��ʾ����ΨһMAC���ʲ�������
										DisplayString(Row,6,0,(u8*)"MAC Ψһ");
									}
									else
									{//(p8[0]&0x01)==1 �鲥��ַ
										DisplayString(Row,4,0,(u8*)"���ֽ�Ϊ����");
									}
								}
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x13;//��ʹ�ú���0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x03;//��ʹ�ú���0
						break;
					}
				}
				else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
				{
					SelectH(MenuCount->v2,MenuCount->v1);
					UpDownSelectH();
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						p8=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+3*2);
						if(p8[1*2]>=0x30 && p8[1*2]<=0x39)
						{
							if((p8[1*2]-0x30)&0x01)
							{//0������ַ 1�鲥��ַ MAC��ַΪ������ַ ���ֽ�ĩλ��ʾ����/�鲥��ַ
								p8[1*2]=p8[1*2]-1;
							}
						}
						else if(p8[1*2]>=0x41 && p8[1*2]<=0x46)
						{
							if((p8[1*2]-0x37)&0x01)
							{//0������ַ 1�鲥��ַ MAC��ַΪ������ַ ���ֽ�ĩλ��ʾ����/�鲥��ַ
								p8[1*2]=p8[1*2]-1;
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
// Function name:       Menu_8_2_4
// Descriptions:        MAC��ַ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_4(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_4);//��ʾ�˵�
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
						MenuCreate((u32)Menu_8_2_4_1);//�����¼��˵�            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2_4_2);//�����¼��˵�
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
// Function name:       Menu_8_2_5
// Descriptions:        �豸�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_5(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_5);//��ʾ�˵�
			//��ʾ�ڴ�������
			//�豸���
			p8=(u8 *)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN0CF1+4,8);
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+8*2);
			for(i=0;i<=7;i++)
			{
				p[i*2]=p8[i];
				p[i*2+1]=0x20;
			}
			MenuCount->v1=8;
			MenuCount->v2=15;
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
						DisplayString(Ln+2,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=15;
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//�ж��ڴ������д� ���ʼ��
								for(i=0;i<8;i++)
								{
									if(p8[i*2]<0x30 || (p8[i*2]>0x39 && p8[i*2]<0x41) || p8[i*2]>0x46 ||p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=8)
								{
									for(i=0;i<8;i++)
									{
										p8[i*2]=0x30;
										p8[i*2+1]=0x20;
									}
								}
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2:
								//�������õ�����
								MR(ADDR_DATABUFF,ADDR_AFN0CF1+4,8);//���豸���
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+8*2);
								p8=(u8 *)ADDR_DATABUFF;
								for(i=0;i<8;i++)
								{
									p8[i]=p[i*2];
								}
								MW(ADDR_DATABUFF,ADDR_AFN0CF1+4,8);//д�豸���
								MW(ADDR_DATABUFF,ADDR_TERMINAL+2,8);//д�豸���
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								break;
							default:
								break;
						}
						Comm_Ram->ButtonVal&=~0x13;//��ʹ�ú���0
					}
					else
					{
						Comm_Ram->ButtonVal&=~0x03;//��ʹ�ú���0
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								for(i=0;i<4;i++)
								{
									p16[i]=0x0;
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
// Function name:       Menu_8_2_6
// Descriptions:        �ն������˿�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2_6(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 y;
	u32 Row=1;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2_6);//��ʾ�˵�
			//��ʾ�ڴ�������
			//�ն������˿�
			MR(ADDR_DATABUFF,ADDR_AFN04F7+20,1);//�ⲿFLASH��
			p8=(u8 *)ADDR_DATABUFF;
			i=20+1+p8[0];
			MR(ADDR_DATABUFF,ADDR_AFN04F7+i,1);//�ⲿFLASH��
			i+=1+p8[0];
			x=MRR(ADDR_AFN04F7+i,2);
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+9*2);
			y=hex_bcd(x);
			DisplayData(y,5,0,p16);

			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=9;
			MenuCount->v2=13;
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
						DisplayString(Ln+2,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=9;
							MenuCount->v2=13;
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
						default:
							break;
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								for(i=0;i<=4;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=5)
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,9,0,(u8*)"OK");
								//�������õ�����
								MR(ADDR_DATABUFF,ADDR_AFN04F7+20,1);//�ⲿFLASH��
								p8=(u8 *)ADDR_DATABUFF;
								i=20+1+p8[0];
								MR(ADDR_DATABUFF,ADDR_AFN04F7+i,1);//�ⲿFLASH��
								i+=1+p8[0];
								MR(ADDR_DATABUFF,ADDR_AFN04F7+i,2);//�ⲿFLASH��
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p16=(u16 *)ADDR_DATABUFF;
								p16[0]=p8_bcd(p,5);
								MW(ADDR_DATABUFF,ADDR_AFN04F7+i,2);//д�ն������˿�
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
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
// Function name:       Menu_8_2
// Descriptions:        �ն˵�ַ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu_8_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_2);//��ʾ�˵�
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
						MenuCreate((u32)Menu_8_2_1);//�����¼��˵�            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu_8_2_3);//�����¼��˵�
						break;
					case 4:
						MenuCreate((u32)Menu_8_2_4);//�����¼��˵�
						break;
					case 5:
						MenuCreate((u32)Menu_8_2_5);//�����¼��˵�
						break;
					case 6:
						MenuCreate((u32)Menu_8_2_6);//�����¼��˵�
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
// Function name:       Menu_8_3
// Descriptions:        ͨ�Ų�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_3(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_3);//��ʾ�˵�
			DisplayString(Ln+3,15,0,(u8 *)UnitList[9]);//9 s
			DisplayString(Ln+4,13,0,(u8 *)UnitList[10]);//10 ��
			DisplayString(Ln+5,13,0,(u8 *)UnitList[8]);//8 min
			//����ģʽ���ز�������ز��������Զ�����ʱ��
			p=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F8,5);
			DisplayString(Ln+1,10,0,(u8 *)OnlineModeList[p[0]&0x03]);
			DisplayString(Ln+2,10,0,(u8 *)ConnectWayList[(p[0]&0x80)>>7]);
			p8=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
			bcd_p8(p8,p[3],3);
			p8=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
			bcd_p8(p8,p[4],3);
			//���ز����
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F8+1,2);
			p8=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+10*2;
			bcd_p8(p8,p16[0],5);
			
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=10;
			MenuCount->v2=17;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=6)
					{
						DisplayString(Ln+6,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=10;
							MenuCount->v2=17;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=10;
							MenuCount->v2=14;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x8E://��
										i=2;
										break;
									case 0x8C://��
										i=3;
										break;
									case 0xD4://ʱ
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[i]);//1 �������� 2 �������� 3 ʱ������
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x54://TCP
										i=1;
										break;
									case 0x55://UDP
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectWayList[i]);//0 TCP,1 UDP
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x<255)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x<255)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x8E://��
										i=3;
										break;
									case 0x8C://��
										i=1;
										break;
									case 0xD4://ʱ
										i=2;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[i]);//1 �������� 2 �������� 3 ʱ������
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x55://UDP
										i=0;
										break;
									case 0x54://TCP
										i=1;
										break;
								}
								DisplayString((Ln+Stack->MenuStack[MenuCount->Count].EditRow),10,0,(u8 *)ConnectWayList[i]);//0 TCP,1 UDP
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,255,3);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,255,3);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//���߷�ʽ1~3 �ж��ڴ���������д����ʼ��
								if(p8[0]==0x24)
								{
									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[1]);//1 ��������
								}
								break;
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 5:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 6:
								DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,(u8*)"OK");
								//�������õ�����
								p=(u8*)ADDR_DATABUFF;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
								MR(ADDR_DATABUFF,ADDR_AFN04F8,5);//������ģʽ���ز�������ز��������Զ�����ʱ��
								switch(p8[0])
								{
									case 0x8E://��
										p[0]&=0xFD;
										p[0]|=0x01;
										break;
									case 0x8C://��
										p[0]&=0xFE;
										p[0]|=0x02;
										break;
									case 0xD4://ʱ
										p[0]|=0x03;
										break;
									default:
										break;
								}

								p8=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+10*2;
								switch(p8[0])
								{
									case 0x54://TCP
										p[0]&=0x7F;
										break;
									case 0x55://UDP
										p[0]|=0x80;
										break;
								}
								p8=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+10*2;
								x=p8_bcd(p8,5);
								p[1]=x&0xff;
								p[2]=x>>8;
								p8=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
								p[3]=p8_bcd(p8,3);
								p8=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
								p[4]=p8_bcd(p8,3);
								MW(ADDR_DATABUFF,ADDR_AFN04F8,5);//д����ģʽ���ز�������ز��������Զ�����ʱ��
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
							case 3:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 5:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
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
// Function name:       Menu_8_4
// Descriptions:        IP���˿ں�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_4(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) /*|| ((USER/100)==11)*/)//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_4);//��ʾ�˵�
			DisplayString(Ln+5,8,0,(u8 *)UnitList[8]);//8 min
			//��ʾ�ڴ�������
			//1.��վIP
			p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
			p8=(u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F3,4);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//3.����IP
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
			MR(ADDR_DATABUFF+4,ADDR_AFN04F3+6,4);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[4+i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//5.��������
			p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
			MR(ADDR_DATABUFF+8,ADDR_AFN04F1+5,1);
			bcd_p8(p,p8[8],3);
			//2.��վ�˿ں�
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F3+4,2);
			bcd_p8(p,p16[0],5);
			//4.���ö˿ں�
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
			MR(ADDR_DATABUFF+2,ADDR_AFN04F3+10,2);
			bcd_p8(p,p16[1],5);
			
			MenuCount->v1=5;
			MenuCount->v2=19;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=6)
					{
						DisplayString(Ln+6,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=5;
							MenuCount->v2=9;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=5;
							MenuCount->v2=19;
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=5;
							MenuCount->v2=9;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=5;
							MenuCount->v2=7;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
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
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 3:
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x<255)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
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
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
								break;
							case 3:
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,255,3);
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
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 5:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 6:
								DisplayClrRow_4(Ln+6);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+6,9,0,(u8*)"OK");
								//�������õ�����
								p8=(u8 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F3,12);//����վIP����վ�˿ںš�����IP�����ö˿ں�
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[i]=p8_bcd(p+8*i,3);
								}
								p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p,5);
								p8[4]=x&0xff;
								p8[5]=x>>8;
								p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[6+i]=p8_bcd(p+8*i,3);
								}
								p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								x=p8_bcd(p,5);
								p8[10]=x&0xff;
								p8[11]=x>>8;
								MW(ADDR_DATABUFF,ADDR_AFN04F3,12);//д��վIP����վ�˿ںš�����IP�����ö˿ں�
								
								p=(u8*)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
								p8[12]=p8_bcd(p,3);
								MW(ADDR_DATABUFF+12,ADDR_AFN04F1+5,1);//д5.��������
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
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 3:
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
								}
								x=p8_bcd(p8+4*2,3);
								if(x>255)
								{
									bcd_p8(p8+4*2,0,3);
								}
								x=p8_bcd(p8+8*2,3);
								if(x>255)
								{
									bcd_p8(p8+8*2,0,3);
								}
								x=p8_bcd(p8+12*2,3);
								if(x>255)
								{
									bcd_p8(p8+12*2,0,3);
								}
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
								}
								break;
							case 5:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+5)*84+4+5*2);
								x=p8_bcd(p8,3);
								if(x>255)
								{
									bcd_p8(p8,0,3);
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
// Function name:       Menu_8_5
// Descriptions:        APN����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
//-------------------------------------------------------------------------------------------------
void Menu_8_5(void)//
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 MinLn=4;//�������С�к�
	u32 MaxLn=8;//���������к�
	u32 LastLnMaxCol=14;//����̵����һ������к�
	u32 MinCol=0;//�������С�к�
	u32 MaxCol=19;//���������к�
	u32 EditLn=2;
	u32 EditCol=2;
	u32 EditLength=16;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	u8 * p8d;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=(u8*)" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayString(0,6,0,(u8*)"APN����");
			DisplayString(MinLn,MinCol,0,p8);
			
			p=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF+16,1);
			MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
			DisplayString(EditLn,EditCol,0,p);
			
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
			break;
		case 1://����1������ѡ��
			KeyboardSelect(MinLn,MaxLn,LastLnMaxCol,MinCol,MaxCol,EditLn,EditCol);//����ѡ��
			break;
		case 2://����2���༭��ѡ��
			if((Comm_Ram->ButtonVal&0x01)!=0)
			{//�¼�
				Stack->MenuStack[MenuCount->Count].EditRow = 4;
				Stack->MenuStack[MenuCount->Count].EditCol = 0;
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				*p16|=0x8000;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			else
			{
				if((Comm_Ram->ButtonVal&0x08)!=0)
				{//�Ҽ�
					Stack->MenuStack[MenuCount->Count].MoveRow ++;
					if(Stack->MenuStack[MenuCount->Count].MoveRow == EditLength)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = 0;
					}
				}
				if((Comm_Ram->ButtonVal&0x04)!=0)
				{//���
					if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
					{
						Stack->MenuStack[MenuCount->Count].MoveRow --;
					}
					else
					{
						Stack->MenuStack[MenuCount->Count].MoveRow = EditLength-1;
					}
				}
				NegativeLnCol(EditLn,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					//��������
					p8=(u8 *)ADDR_DATABUFF;
					MC(0,ADDR_DATABUFF+16,1);
					p=(u8 *)(ADDR_STRINGBUFF+(EditLn)*84+4+(EditCol)*2);
					for(i=0;i<=15;i++)
					{
						if(p[i*2]!=0x20 && p[i*2]!=0x00)
						{
							p8[i]=p[i*2];
						}
						else
						{
							for(j=i;j<=15;j++)
							{
								p8[j]=0x00;
							}
							break;
						}
					}
					MW(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
					//���óɹ���ʾ�ϣ�
					DisplayClrRow(1,9);
					DisplayString(1,0,0,p8);
					DispalyAlign(1,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					DisplayString(4,7,0,(u8*)"����OK");
					DisplayString(5,4,0,(u8*)"��������˳�");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
					p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
					p8d[0]=1;//�˵���ʾ���������ã�0=��1=��
#endif
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//�����ϼ��˵�
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//��ʹ�ú���0
			break;
		case 3://���óɹ���������˳�
			Stack->MenuStack[MenuCount->Count].MoveRow =0;
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
				p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
				p8d[0]=1;//�˵���ʾ���������ã�0=��1=��
#endif
				Stack->MenuStack[MenuCount->Count].MoveRow =0;
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
			break;
		default:
			break;
	}
}

#if (((Project/100)==3) || ((Project/100)==4) || ((Project/100)==2))//ר���նˡ������նˡ�������
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_7
// Descriptions:        ר���û�������������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_7(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 MinLn=4;//�������С�к�
	u32 MaxLn=8;//���������к�
	u32 LastLnMaxCol=14;//����̵����һ������к�
	u32 MinCol=0;//�������С�к�
	u32 MaxCol=19;//���������к�
	u32 EditCol=0;
	u32 EditLength=20;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	u8 * p8d;
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=(u8*)" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayString(MinLn,MinCol,0,p8);
			p=(u8 *)ADDR_DATABUFF;
			MC(0,ADDR_DATABUFF+32,1);
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow)
			{
				case 6:
					DisplayString(0,3,0,(u8*)"ר���û�������");
					MR(ADDR_DATABUFF,ADDR_AFN04F16,32);
					break;
				case 7:
					DisplayString(0,4,0,(u8*)"ר����������");
					MR(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
					break;
			}
			DisplayString(MenuCount->v1,EditCol,0,p);
			
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			NegativeLnCol(MenuCount->v1,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
			break;
		case 1://����1������ѡ��
			KeyboardSelect(MinLn,MaxLn,LastLnMaxCol,MinCol,MaxCol,MenuCount->v1,EditCol);//����ѡ��
			break;
		case 2://����2���༭��ѡ��
			if((Comm_Ram->ButtonVal&0x01)!=0)
			{//�¼�
				Stack->MenuStack[MenuCount->Count].EditRow = 4;
				Stack->MenuStack[MenuCount->Count].EditCol = 0;
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
				*p16|=0x8000;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			else
			{
				if((Comm_Ram->ButtonVal&0x08)!=0)
				{//�Ҽ�
					switch(MenuCount->v1)
					{
						case 1:
							Stack->MenuStack[MenuCount->Count].MoveRow ++;
							if(Stack->MenuStack[MenuCount->Count].MoveRow == EditLength)
							{
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								(MenuCount->v1)++;
							}
							break;
						case 2:
							Stack->MenuStack[MenuCount->Count].MoveRow ++;
							if(Stack->MenuStack[MenuCount->Count].MoveRow == 12)
							{
								Stack->MenuStack[MenuCount->Count].MoveRow = 0;
								(MenuCount->v1)--;
							}
							break;
					}
				}
				if((Comm_Ram->ButtonVal&0x04)!=0)
				{//���
					switch(MenuCount->v1)
					{
						case 1:
							if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
							{
								Stack->MenuStack[MenuCount->Count].MoveRow --;
							}
							else
							{
								Stack->MenuStack[MenuCount->Count].MoveRow = 11;
								(MenuCount->v1)++;
							}
							break;
						case 2:
							if(Stack->MenuStack[MenuCount->Count].MoveRow > 0)
							{
								Stack->MenuStack[MenuCount->Count].MoveRow --;
							}
							else
							{
								Stack->MenuStack[MenuCount->Count].MoveRow = EditLength-1;
								(MenuCount->v1)--;
							}
							break;
					}
				}
				NegativeLnCol(MenuCount->v1,Stack->MenuStack[MenuCount->Count].MoveRow+EditCol);
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					//��������
					p8=(u8 *)ADDR_DATABUFF;
					MC(0,ADDR_DATABUFF+32,1);
					p=(u8 *)(ADDR_STRINGBUFF+1*84+4+(EditCol)*2);
					for(i=0;i<=19;i++)
					{
						if(p[i*2]==0x20 || p[i*2]==0x00)
						{
							p[i*2]=0x00;
						}
						p8[i]=p[i*2];
					}
					p=(u8 *)(ADDR_STRINGBUFF+2*84+4+0*2);
					for(i=0;i<=11;i++)
					{
						if(p[i*2]==0x20 || p[i*2]==0x00)
						{
							p[i*2]=0x00;
						}
						p8[20+i]=p[i*2];
					}
					for(i=0;i<=31;i++)
					{
						if(p8[i]==0x00)
						{
							break;
						}
					}
					if(i!=32)
					{
						for(j=i;j<=31;j++)
						{
							p8[j]=0x00;
						}
					}
					switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
					{
						case 6:
							MW(ADDR_DATABUFF,ADDR_AFN04F16,32);
							break;
						case 7:
							MW(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
							break;
					}
					//���óɹ���ʾ�ϣ�
					DisplayClrRow(1,9);
					DisplayString(1,2,0,p8);
					DispalyAlign(1,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					DispalyAlign(2,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					DisplayString(4,7,0,(u8*)"����OK");
					DisplayString(5,4,0,(u8*)"��������˳�");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
					p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
					p8d[0]=1;//�˵���ʾ���������ã�0=��1=��
#endif
					Stack->MenuStack[MenuCount->Count].MoveRow =0;
					MenuDel();//�����ϼ��˵�
				}
			}
			Comm_Ram->ButtonVal&=~0x3D;//��ʹ�ú���0
			break;
		case 3://���óɹ���������˳�
			if((Comm_Ram->ButtonVal&0x3F)!=0)
			{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
				p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
				p8d[0]=1;//�˵���ʾ���������ã�0=��1=��
#endif
				Stack->MenuStack[MenuCount->Count].MoveRow =0;
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
#endif

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8
// Descriptions:        ���ܱ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_8(void)
{
	u8 *p8;
	u8 *p;
	u8 *p8d;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
	u64 y;
	#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	u8 * p8s;
	#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu_SH(0,Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8);//��ʾ�˵�
			//��ʾ�ڴ�������
			MenuCount->v1=8;
			MenuCount->v2=11;
			if(MenuCount->v3>MaxRS485AddCarrierPn)
			{//MenuCount->v3��¼������ŵ�ȫ�ֱ���
				MenuCount->v3=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v3<1)
			{//MenuCount->v3��¼������ŵ�ȫ�ֱ���
				MenuCount->v3=1;
			}
			p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+(MenuCount->v1)*2);
			bcd_p8(p,MenuCount->v3,4);
			//�������
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
			if(p16[0])
			{//������Ų�Ϊ0ʱ����ʾ��Ӧ����Ϣ
				p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
				bcd_p8(p,p16[0],4);
				//ͨ�����ʼ��˿ںš�ͨ��Э������
				p8=(u8*)ADDR_DATABUFF;
				p=(u8*)(ADDR_STRINGBUFF+3*84+4+8*2);
				MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,2);
				DisplayString(3,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//��ʾͨ������
				p=(u8*)(ADDR_STRINGBUFF+4*84+4+8*2);
				bcd_p8(p,p8[0] & 0x1F,2);
				switch(p8[1])
				{
					case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
						j++;
					case 30://DL/T 645-2007
						j++;
				#if (((Project/100)==3&&(USER/100)==5))//�Ϻ�ר��
					case 21://�Ϻ���Լ
						j++;
				#endif
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
				DisplayString(5,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
				//ͨ�ŵ�ַ
				p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+6,6);
				DisplayData(x,12,0,p16);
				//�����ɼ���ͨ�ŵ�ַ
				p16=(u16 *)(ADDR_STRINGBUFF+7*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+20,6);
				DisplayData(x,12,0,p16);
				//����ż�С���
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
				p=(u8*)(ADDR_STRINGBUFF+8*84+4+8*2);
				bcd_p8(p,x>>4,2);
				p=(u8*)(ADDR_STRINGBUFF+9*84+4+8*2);
				bcd_p8(p,x&0x0f,2);
			}
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].Task==2)
					{
						Stack->MenuStack[MenuCount->Count].Task=1;
						if(Stack->MenuStack[MenuCount->Count].MoveRow!=0)
						{
							MR(ADDR_STRINGBUFF+0*84+0*2,ADDR_STRINGBUFF+1*84+0*2,84*9);
							for(i=0;i<=8;i++)
							{
								StrBuff->Row[i].Row --;
							}
							DisplayClrRow_4(9);//����Ļ��ʾ��ָ����(4�������ֲ���)
							DisplayString(9,6,0,(u8*)"ȷ������");
						}
						else
						{
							for(i=9;i>0;i--)
							{
								MR(ADDR_STRINGBUFF+i*84+0*2,ADDR_STRINGBUFF+(i-1)*84+0*2,84);
								StrBuff->Row[i].Row ++;
							}
							DisplayClrRow(0,0);
#if ((Project/100)==2)//������
							DisplayString(0,3,0,(u8*)"���ܱ��������");
#else
							DisplayString(0,5,0,(u8*)"���ܱ�����");
#endif

						}
					}
					
					if((Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)!=10)
					{
						DisplayClrRow_4(10-Stack->MenuStack[MenuCount->Count].MoveRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
						DisplayString(10-Stack->MenuStack[MenuCount->Count].MoveRow,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=11;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=8;
							MenuCount->v2=11;
							p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p16=(u16*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
								bcd_p8(p,p16[0],4);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=8;
							MenuCount->v2=12;
							p=(u8*)(ADDR_STRINGBUFF+(3-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p8=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
								DisplayString(3-Stack->MenuStack[MenuCount->Count].MoveRow,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//��ʾͨ������
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+4*84+4+8*2);
							if(p[0]==0x0)
							{
								p8=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
								bcd_p8(p,p8[0] & 0x1F,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p=(u8*)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p16=(u16 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
								if(p16[0]==0)
								{//ԭʼ�������Ϊ0ʱĬ��DL/T 645-2007
									j=4;
								}
								else
								{
									p8=(u8*)ADDR_DATABUFF;
									MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+5,1);
									switch(p8[0])
									{
										case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
											j++;
										case 30://DL/T 645-2007
											j++;
									#if (((Project/100)==3&&(USER/100)==5))//�Ϻ�ר��
										case 21://�Ϻ���Լ
											j++;
									#endif	
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
								}
								DisplayString(5-Stack->MenuStack[MenuCount->Count].MoveRow,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p16=(u16 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p16[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+6,6);
								DisplayData(x,12,0,p16);
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p16=(u16 *)(ADDR_STRINGBUFF+(7-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p16[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+20,6);
								DisplayData(x,12,0,p16);
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+(8-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
								bcd_p8(p,x>>4,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 9:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+(9-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
								bcd_p8(p,x&0x0f,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 10:
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0x62&&p8[1]==0x55)
								{//Ĭ
									i=1;
								}
								else
								{
									switch(x)
									{
										case 60:
											i=2;
											break;
										case 12:
											i=3;
											break;
										case 24:
											i=4;
											break;
										case 48:
											i=5;
											break;
										case 72:
											i=6;
											break;
										case 96:
											i=7;
											break;
										case 19:
											i=0;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<31)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0x5C && p8[1]==0x47)
								{//����
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=6;
											}
											else
											{
												if(p8[0]==0xA0 && p8[1]==0x4A)
												{//��
													i=0;
												}
												else
												{
													if(p8[0]==0x54 && p8[1]==0x59)
													{//�Ϻ���Լ
														i=5;
													}
													else//698.45
														i=4;
												}
											}
										}
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<15)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 9:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<15)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
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
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>0)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0x62&&p8[1]==0x55)
								{//Ĭ
									i=7;
								}
								else
								{
									switch(x)
									{
										case 19:
											i=6;
											break;
										case 96:
											i=5;
											break;
										case 72:
											i=4;
											break;
										case 48:
											i=3;
											break;
										case 24:
											i=2;
											break;
										case 12:
											i=1;
											break;
										case 60:
											i=0;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>1)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,31,2);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//��
									i=5;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=4;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else
											{
												if(p8[0]==0x5C && p8[1]==0x47)
												{//����
													i=6;
												}
												else
												{
													if(p8[0]==0x54 && p8[1]==0x59)//�Ϻ���Լ
														i=3;
													else//698.45
														i=2;
												}
											}
										}
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,15,2);
								}
								break;
							case 9:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,15,2);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 3:
								//ͨ�����ʣ�D7~D5��(1~7)��ͨ�Ŷ˿ںţ�D4~D0��(1~31)BINռ1���ֽ�  �����ж��ڴ������Ƿ��д�
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								//ͨ�����ʣ�D7~D5��(1~7)��ͨ�Ŷ˿ںţ�D4~D0��(1~31)BINռ1���ֽ�  �����ж��ڴ������Ƿ��д�
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0x86 && p8[1]==0x47)
								{//����
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[0]);//0 ����
								}
								break;
							case 6:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//ͨ�ŵ�ַBCDռ6���ֽ� �ж��ڴ���������д����ʼ��
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 7:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//�����ɼ���ͨ�ŵ�ַBCDռ6���ֽ� �ж��ڴ���������д����ʼ��
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 8:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 9:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 10:
								p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								p8=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								if(p8_bcd(p8,4)>MaxRS485AddCarrierPn)
								{//�����㳬�����ֵ �򲻿�����
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"��Ч������");
								}
								else
								{
									if(MeterPnCompare(p8_bcd(p,4),p8_bcd(p8,4)) || PulsePnCompare(MaxImpInPort+1,p8_bcd(p8,4)))
									{//�������ظ� ��������
										DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"�������ظ�");
									}
									else
									{//�����㲻�ظ� ������
//Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
//Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
//u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr)//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
//PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
//AFN04F150_Computer();//��������Ч��־����
//Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�

//��ͨ�Ŷ˿�Ϊ31ʱҪ���е���ͬ����ͨ�ŵ�ַ��ͬ������
										p=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										x=p8_bcd(p,2);//�¶˿ں�
										p8=(u8 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										y=p8_bcd(p8,12);//��ͨ�ŵ�ַ
										y=hex_bcd16(y);
										y=Check_AFN04F10_Addr(x,y);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
										p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										i=p8_bcd(p,4);//�²������
										
									#if(USER/100)==13//������
										//���ɲ����㲻�ܱ�ɾ��
										p8=(u8 *)ADDR_DATABUFF;
										MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
										if((p8[2]+(p8[3]<<8))==1)
										{//ԭ�������Ϊ1
											if((p8[4]&0x1f)==1)
											{//ԭ�˿ں�Ϊ1
												if(p8[5]==2)
												{//ԭͨ��Э������Ϊ����
													p=(u8 *)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
													if((i!=1) || (x!=1) || (p[0]!=0x5C || p[1]!=0x50))
													{//���õĲ������!=1�����õĶ˿ں�!=1�����õ�ͨ��Э�����Ͳ��ǽ���
														DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,4,0,(u8*)"���ɲ���ɾ��");
														break;
													}
												}
											}
										}
									#endif
										if(i==0)
										{//�������Ϊ0 ������
											k=1;//kΪ1������
										}
										else
										{//������Ų�Ϊ0
											if((y!=0) && (y!=MenuCount->v3))
											{//��ͬͨ�ŵ�ַ��������ź͵�ǰ���õ�������Ų�ͬ����������
												k=0;//kΪ0��������
												DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"���ַ�ظ�");
											}
											else
											{
												k=1;//kΪ1������
											}
										}
										if(k==1)
										{//
											if(x!=31)
											{
												p8=(u8 *)ADDR_DATABUFF;
												MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��,700=			
												if((p8[0]&0x1F)==31)
												{//ԭ�˿�31���е���ͬ��
													Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
													Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
												}
#endif
											}
											else
											{
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��,700=			
												Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
												Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
#endif
											}
											DisplayClrRow_4(Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
											DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,9,0,(u8*)"OK");
											//�������õ�����
											p8=(u8 *)ADDR_DATABUFF;
											MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
											p8d=(u8 *)ADDR_DATABUFF+100;
											MR(ADDR_DATABUFF+100,ADDR_DATABUFF,LEN_AFN04F10_Pn);
											p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,4);
											p8[0]=x&0xff;
											p8[1]=x>>8;	
											p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,4);
											p8[2]=x&0xff;
											p8[3]=x>>8;	
											p=(u8*)(ADDR_STRINGBUFF+(3-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,2);
											if(p[0]==0x62 && p[1]==0x55)
											{//Ĭ�� D7~D5:000
												p8[4]&=0x1F;
											}
											else
											{
												switch(x)
												{
													case 60://600 D7~D5:001
														p8[4]&=0x3F;
														p8[4]|=0x20;
														break;
													case 12://1200 D7~D5:010
														p8[4]&=0x5F;
														p8[4]|=0x40;
														break;
													case 24://2400 D7~D5:011
														p8[4]&=0x7F;
														p8[4]|=0x60;
														break;
													case 48://4800 D7~D5:100
														p8[4]&=0x9F;
														p8[4]|=0x80;
														break;
													case 72://7200 D7~D5:101
														p8[4]&=0xBF;
														p8[4]|=0xA0;
														break;
													case 96://9600 D7~D5:110
														p8[4]&=0xDF;
														p8[4]|=0xC0;
														break;
													case 19://19200 D7~D5:111
														p8[4]&=0xfF;
														p8[4]|=0xE0;
														break;
													default:
														break;
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,2);
											p8[4]=(p8[4]&0xE0)+x;
											p=(u8 *)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											if(p[0]==0xA0 && p[1]==0x4A)
											{//��ѹ�ز� 31
												p8[5]=31;
											}
											else
											{
												if(p[9*2]==0x30)
												{//DL/T645-2007 30
													p8[5]=30;
												}
												else
												{
													if(p[0]==0x5C && p[1]==0x50)
													{//�������� 2
														p8[5]=2;
													}
													else
													{
														if(p[9*2]==0x39)
														{//DL/T645-1997 1
															p8[5]=1;
														}
														else
														{
															if(p[0]==0x5C && p[1]==0x47)//���� 0
															{
																p8[5]=0;
															}
															else
															{
																if(p8[0]==0x54 && p8[1]==0x59)//�Ϻ���Լ
																	p8[5]=21;
																else//698.45
																	p8[5]=3;
															}
														}
													}
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											for(i=0;i<=5;i++)
											{
												if(p[(11-(2*i+1))*2]>=0x30 && p[(11-(2*i+1))*2]<=0x39)
												{
													if(p[(11-2*i)*2]>=0x30 && p[(11-2*i)*2]<=0x39)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x30)*1;
													}
													if(p[(11-2*i)*2]>=0x41 && p[(11-2*i)*2]<=0x46)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x37)*1;
													}
												}
												if(p[(11-(2*i+1))*2]>=0x41 && p[(11-(2*i+1))*2]<=0x46)
												{
													if(p[(11-2*i)*2]>=0x30 && p[(11-2*i)*2]<=0x39)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x37)*16+(p[(11-2*i)*2]-0x30)*1;
													}
													if(p[(11-2*i)*2]>=0x41 && p[(11-2*i)*2]<=0x46)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x37)*16+(p[(11-2*i)*2]-0x37)*1;
													}
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(7-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											for(i=0;i<=5;i++)
											{
												p8[20+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x30)*1;
											}
											p=(u8 *)(ADDR_STRINGBUFF+(8-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											p8[26]=((p8_bcd(p,2))<<4)+(p8_bcd(p+84,2));
			//����ΪAFN04F10д��������ԭ���ݱȽ�
											for(i=0;i<=(LEN_AFN04F10_Pn-1);i++)
											{
												if(p8[i]!=p8d[i])
												{
													break;
												}
											}
											if(i!=LEN_AFN04F10_Pn)
											{//д��������ԭ���ݲ���ͬ
												Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
											}
			//����ΪAFN04F10д��������ԭ���ݱȽ�
											if(p8[18]==0)
											{//����Ϊ0
												p8[18]=4;//Ĭ��д�����4
											}
											MW(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
	
											PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
											AFN04F150_Computer();//��������Ч��־����
											Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
		
	//�˵����ж�AFN04F10���õĵط�������������ű仯�������Ƿ����Ϲ��ܡ�
	//Init_Class1Data(pnType,Pn);//��1���ݳ�ʼ��(ָ��Pn����)
	//Init_Class2Data(Pn);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
	//pnType�궨��Ϊ���������ͣ���ֱ���ô�����;PnΪ��1��ʼ�Ĳ������
	
											if((p8[2]+(p8[3]<<8))!=(p8d[2]+(p8d[3]<<8)))
											{//������Ų�ͬ
												if((p8d[2]+(p8d[3]<<8))!=0)
												{
													Init_Class1Data(pnType,p8d[2]+(p8d[3]<<8));//��1���ݳ�ʼ��(ָ��Pn����)
													Init_Class2Data(p8d[2]+(p8d[3]<<8));//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
												}
												if((p8[2]+(p8[3]<<8))!=0)
												{//�²������
													Init_Class1Data(pnType,p8[2]+(p8[3]<<8));//��1���ݳ�ʼ��(ָ��Pn����)
													Init_Class2Data(p8[2]+(p8[3]<<8));//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
												}
											}
											else
											{
												if((p8d[2]+(p8d[3]<<8))!=0)
												{//ԭ�������
													for(i=6;i<=12;i++)
													{
														if(p8[i]!=p8d[i])
														{
															break;
														}
													}
													if(i!=13)
													{//ͨ�ŵ�ַ�仯
														Init_Class1Data(pnType,p8d[2]+(p8d[3]<<8));//��1���ݳ�ʼ��(ָ��Pn����)
														Init_Class2Data(p8d[2]+(p8d[3]<<8));//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
													}
												}
											}
										}
									}
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
					Select_SH(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
					UpDownSelect_SH(Stack->MenuStack[MenuCount->Count].EditRow);
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn || x<1)
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn)
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 3:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>31 || x<1)
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 5:
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 7:
								break;
							case 8:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>15 || x<1)
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 9:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>15 || x<1)
								{
									bcd_p8(p8,0,2);
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
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
				p8s=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
				p8s[0]=1;//�˵���ʾ���������ã�0=��1=��
#endif
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	//ͨ�Ŷ˿ں�
	p8=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
	x=p8_bcd(p8,2);
	switch(x)
	{
		case 1:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)":���� ");			
			break;
		case 2:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)":485I ");			
			break;
		case 3:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)":485II");			
			break;
		case 31:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)":�ز� ");			
			break;
		default:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,(u8*)"      ");			
			break;
	}
}
	#else
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_8
// Descriptions:        ���ܱ�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_8(void)
{
	u8 *p8;
	u8 *p;
	u8 *p8d;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_8);//��ʾ�˵�
			//��ʾ�ڴ�������
			MenuCount->v1=8;
			MenuCount->v2=11;
			if(MenuCount->v3>MaxRS485AddCarrierPn)
			{//MenuCount->v3��¼������ŵ�ȫ�ֱ���
				MenuCount->v3=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v3<1)
			{//MenuCount->v3��¼������ŵ�ȫ�ֱ���
				MenuCount->v3=1;
			}
			p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+(MenuCount->v1)*2);
			bcd_p8(p,MenuCount->v3,4);
			//�������
			p16=(u16*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
			if(p16[0])
			{//������Ų�Ϊ0ʱ����ʾ��Ӧ����Ϣ
				p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
				bcd_p8(p,p16[0],4);
				//ͨ�����ʼ��˿ںš�ͨ��Э������
				p8=(u8*)ADDR_DATABUFF;
				p=(u8*)(ADDR_STRINGBUFF+3*84+4+8*2);
				MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,2);
				DisplayString(3,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//��ʾͨ������
				p=(u8*)(ADDR_STRINGBUFF+4*84+4+8*2);
				bcd_p8(p,p8[0] & 0x1F,2);
#if ((Project/100)==3 && (USER/100)==10)//�ӱ�ר��
				switch(p8[1])
				{
					case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
						j++;
					case 30://DL/T 645-2007
						j++;
					case 7://��ʤ��Լ
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
#elif ((USER/100)==5)//�Ϻ��û�
				switch(p8[1])
				{
					case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
						j++;
					case 30://DL/T 645-2007
						j++;
					case 21://�Ϻ���Լ
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
#elif ((Project/100)==3 && (USER/100)==12)//�Ĵ�ר��
				switch(p8[1])
				{
					case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
						j++;
					case 30://DL/T 645-2007
						j++;
					case 4://�����Լ
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
#else
				switch(p8[1])
				{
					case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
						j++;
					case 30://DL/T 645-2007
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
#endif
				DisplayString(5,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
				//ͨ�ŵ�ַ
				p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+6,6);
				DisplayData(x,12,0,p16);
				//�����ɼ���ͨ�ŵ�ַ
				p16=(u16 *)(ADDR_STRINGBUFF+7*84+4+8*2);
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+20,6);
				DisplayData(x,12,0,p16);
				//����ż�С���
				x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
				p=(u8*)(ADDR_STRINGBUFF+8*84+4+8*2);
				bcd_p8(p,x>>4,2);
				p=(u8*)(ADDR_STRINGBUFF+9*84+4+8*2);
				bcd_p8(p,x&0x0f,2);
			}
			ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].Task==2)
					{
						Stack->MenuStack[MenuCount->Count].Task=1;
						if(Stack->MenuStack[MenuCount->Count].MoveRow!=0)
						{
							MR(ADDR_STRINGBUFF+0*84+0*2,ADDR_STRINGBUFF+1*84+0*2,84*9);
							for(i=0;i<=8;i++)
							{
								StrBuff->Row[i].Row --;
							}
							DisplayClrRow_4(9);//����Ļ��ʾ��ָ����(4�������ֲ���)
							DisplayString(9,6,0,(u8*)"ȷ������");
						}
						else
						{
							for(i=9;i>0;i--)
							{
								MR(ADDR_STRINGBUFF+i*84+0*2,ADDR_STRINGBUFF+(i-1)*84+0*2,84);
								StrBuff->Row[i].Row ++;
							}
							DisplayClrRow(0,0);
#if ((Project/100)==2)//������
							DisplayString(0,3,0,(u8*)"���ܱ��������");
#else
							DisplayString(0,5,0,(u8*)"���ܱ�����");
#endif

						}
					}
					
					if((Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)!=10)
					{
						DisplayClrRow_4(10-Stack->MenuStack[MenuCount->Count].MoveRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
						DisplayString(10-Stack->MenuStack[MenuCount->Count].MoveRow,6,0,(u8*)"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCount->v1=8;
							MenuCount->v2=11;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=8;
							MenuCount->v2=11;
							p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p16=(u16*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
								bcd_p8(p,p16[0],4);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=8;
							MenuCount->v2=12;
							p=(u8*)(ADDR_STRINGBUFF+(3-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p8=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
								DisplayString(3-Stack->MenuStack[MenuCount->Count].MoveRow,8,0,(u8 *)CommunicationSpeedList[p8[0]>>5]);//��ʾͨ������
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+4*84+4+8*2);
							if(p[0]==0x0)
							{
								p8=(u8*)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
								bcd_p8(p,p8[0] & 0x1F,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p=(u8*)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								p16=(u16 *)ADDR_DATABUFF;
								MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+2,2);
								if(p16[0]==0)
								{//ԭʼ�������Ϊ0ʱĬ��DL/T 645-2007
#if ((Project/100)==3 && (USER/100)==10)//�ӱ�ר��
									j=4;
#elif ((USER/100)==5)//�Ϻ�
									j=4;
#elif ((Project/100)==3 && (USER/100)==12)//�Ĵ�ר��
									j=4;
#else
									j=3;
#endif
								}
								else
								{
									p8=(u8*)ADDR_DATABUFF;
									MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+5,1);
#if ((Project/100)==3 && (USER/100)==10)//�ӱ�ר��
									switch(p8[0])
									{
										case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
											j++;
										case 30://DL/T 645-2007
											j++;
										case 7://��ʤ��Լ
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
#elif ((USER/100)==5)//�Ϻ��û�
									switch(p8[0])
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
#elif ((Project/100)==3 && (USER/100)==12)//�Ĵ�ר��
									switch(p8[0])
									{
										case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
											j++;
										case 30://DL/T 645-2007
											j++;
										case 4://�����Լ
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
#else
									switch(p8[0])
									{
										case 31://"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
											j++;
										case 30://DL/T 645-2007
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
#endif
								}
								DisplayString(5-Stack->MenuStack[MenuCount->Count].MoveRow,8,0,(u8 *)ProtocolList[j]);//ͨ��Э���б�
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p16=(u16 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p16[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+6,6);
								DisplayData(x,12,0,p16);
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=8;
							MenuCount->v2=19;
							p16=(u16 *)(ADDR_STRINGBUFF+(7-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p16[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+20,6);
								DisplayData(x,12,0,p16);
							}
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
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+(8-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
								bcd_p8(p,x>>4,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 9:
							MenuCount->v1=8;
							MenuCount->v2=9;
							p=(u8*)(ADDR_STRINGBUFF+(9-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
							if(p[0]==0x0)
							{
								x=MRR(ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+26,1);
								bcd_p8(p,x&0x0f,2);
							}
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 10:
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<MaxRS485AddCarrierPn)
								{
									p8_add1(p8,4);
								}
								else
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0x62&&p8[1]==0x55)
								{//Ĭ
									i=1;
								}
								else
								{
									switch(x)
									{
										case 60:
											i=2;
											break;
										case 12:
											i=3;
											break;
										case 24:
											i=4;
											break;
										case 48:
											i=5;
											break;
										case 72:
											i=6;
											break;
										case 96:
											i=7;
											break;
										case 19:
											i=0;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<31)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
#if ((Project/100)==3 && (USER/100)==10)//�ӱ�ר��
								if(p8[0]==0x5C && p8[1]==0x47)
								{//����
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=5;
											}
											else
											{
												if(p8[0]==0xA0 && p8[1]==0x4A)
												{//��
													i=0;
												}
												else
												{//��ʤ��Լ
													i=4;
												}
											}
										}
									}
								}
#elif ((USER/100)==5)//�Ϻ��û�
								if(p8[0]==0x5C && p8[1]==0x47)
								{//����
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=5;
											}
											else
											{
												if(p8[0]==0xA0 && p8[1]==0x4A)
												{//��
													i=0;
												}
												else
												{//�Ϻ���Լ
													i=4;
												}
											}
										}
									}
								}
#elif ((Project/100)==3 && (USER/100)==12)//�Ĵ�ר��
								if(p8[0]==0x5C && p8[1]==0x47)
								{//����
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=5;
											}
											else
											{
												if(p8[0]==0xA0 && p8[1]==0x4A)
												{//��
													i=0;
												}
												else
												{//�����Լ
													i=4;
												}
											}
										}
									}
								}
#else
								if(p8[0]==0x5C && p8[1]==0x47)
								{//����
									i=1;
								}
								else
								{
									if(p8[9*2]==0x39)
									{//DL/T645-1997
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=3;
										}
										else
										{
											if(p8[9*2]==0x30)
											{//DL/T645-2007
												i=4;
											}
											else//if(p8[0]==0xA0 && p8[1]==0x4A)//��
											{
												i=0;
											}
										}
									}
								}
#endif
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_add1(p8,12);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<15)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 9:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x<15)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
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
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>0)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,MaxRS485AddCarrierPn,4);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0x62&&p8[1]==0x55)
								{//Ĭ
									i=7;
								}
								else
								{
									switch(x)
									{
										case 19:
											i=6;
											break;
										case 96:
											i=5;
											break;
										case 72:
											i=4;
											break;
										case 48:
											i=3;
											break;
										case 24:
											i=2;
											break;
										case 12:
											i=1;
											break;
										case 60:
											i=0;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>1)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,31,2);
								}
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
#if ((Project/100)==3 && (USER/100)==10)//�ӱ�ר��
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//��
									i=4;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=3;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else
											{
												if(p8[0]==0x5C && p8[1]==0x47)
												{//����
													i=4;
												}
												else
												{//��ʤ��Լ
													i=2;
												}
											}
										}
									}
								}
#elif ((USER/100)==5)//�Ϻ��û�
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//��
									i=4;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=3;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else
											{
												if(p8[0]==0x5C && p8[1]==0x47)
												{//����
													i=4;
												}
												else
												{//�Ϻ���Լ
													i=2;
												}
											}
										}
									}
								}
#elif ((Project/100)==3 && (USER/100)==12)//�Ĵ�ר��
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//��
									i=4;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=3;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else
											{
												if(p8[0]==0x5C && p8[1]==0x47)
												{//����
													i=4;
												}
												else
												{//�����Լ
													i=2;
												}
											}
										}
									}
								}
#else
								if(p8[0]==0xA0 && p8[1]==0x4A)
								{//��
									i=3;
								}
								else
								{
									if(p8[9*2]==0x30)
									{//DL/T645-2007
										i=2;
									}
									else
									{
										if(p8[0]==0x5C && p8[1]==0x50)
										{//��
											i=1;
										}
										else
										{
											if(p8[9*2]==0x39)
											{//DL/T645-1997
												i=0;
											}
											else//if(p8[0]==0x5C && p8[1]==0x47)//����
											{
												i=4;
											}
										}
									}
								}
#endif
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								p8_sub1(p8,12);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,15,2);
								}
								break;
							case 9:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,15,2);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 3:
								//ͨ�����ʣ�D7~D5��(1~7)��ͨ�Ŷ˿ںţ�D4~D0��(1~31)BINռ1���ֽ�  �����ж��ڴ������Ƿ��д�
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								//ͨ�����ʣ�D7~D5��(1~7)��ͨ�Ŷ˿ںţ�D4~D0��(1~31)BINռ1���ֽ�  �����ж��ڴ������Ƿ��д�
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0x86 && p8[1]==0x47)
								{//����
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[0]);//0 ����
								}
								break;
							case 6:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//ͨ�ŵ�ַBCDռ6���ֽ� �ж��ڴ���������д����ʼ��
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 7:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
								//�����ɼ���ͨ�ŵ�ַBCDռ6���ֽ� �ж��ڴ���������д����ʼ��
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 8:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 9:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,(u8*)"����");
								break;
							case 10:
								p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								p8=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								if(p8_bcd(p8,4)>MaxRS485AddCarrierPn)
								{//�����㳬�����ֵ �򲻿�����
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"��Ч������");
								}
								else
								{
//u32 Check_AFN04F10_Pn(u32 Pn)//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
//									if(MeterPnCompare(p8_bcd(p,4),p8_bcd(p8,4)) || PulsePnCompare(MaxImpInPort+1,p8_bcd(p8,4)))
									if(((Check_AFN04F10_Pn(p8_bcd(p8,4))!=0)&& (Check_AFN04F10_Pn(p8_bcd(p8,4))!=MenuCount->v3)) || PulsePnCompare(MaxImpInPort+1,p8_bcd(p8,4)))
									{//�������ظ� ��������
										DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"�������ظ�");
									}
									else
									{//�����㲻�ظ� ������
//Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
//Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
//u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr)//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
//PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
//AFN04F150_Computer();//��������Ч��־����
//Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�

//��ͨ�Ŷ˿�Ϊ31ʱҪ���е���ͬ����ͨ�ŵ�ַ��ͬ������
										p=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										x=p8_bcd(p,2);//�¶˿ں�
										p8=(u8 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										y=p8_bcd(p8,12);//��ͨ�ŵ�ַ
										y=hex_bcd16(y);
										y=Check_AFN04F10_Addr(x,y);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
										p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
										i=p8_bcd(p,4);//�²������
										
										if(i==0)
										{//�������Ϊ0 ������
											k=1;//kΪ1������
										}
										else
										{//������Ų�Ϊ0
											if((y!=0) && (y!=MenuCount->v3))
											{//��ͬͨ�ŵ�ַ��������ź͵�ǰ���õ�������Ų�ͬ����������
												k=0;//kΪ0��������
												DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8*)"���ַ�ظ�");
											}
											else
											{
												k=1;//kΪ1������
											}
										}
										if(k==1)
										{//
											if(x!=31)
											{
												p8=(u8 *)ADDR_DATABUFF;
												MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1)+4,1);
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��,700=			
												if((p8[0]&0x1F)==31)
												{//ԭ�˿�31���е���ͬ��
													Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
													Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
												}
#endif
											}
											else
											{
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��,700=			
												Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
												Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
#endif
											}
											DisplayClrRow_4(Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
											DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
											//�������õ�����
											p8=(u8 *)ADDR_DATABUFF;
											MR(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
											p8d=(u8 *)ADDR_DATABUFF+100;
											MR(ADDR_DATABUFF+100,ADDR_DATABUFF,LEN_AFN04F10_Pn);
											p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,4);
											p8[0]=x&0xff;
											p8[1]=x>>8;	
											p=(u8*)(ADDR_STRINGBUFF+(2-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,4);
											p8[2]=x&0xff;
											p8[3]=x>>8;	
											p=(u8*)(ADDR_STRINGBUFF+(3-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,2);
											if(p[0]==0x62 && p[1]==0x55)
											{//Ĭ�� D7~D5:000
												p8[4]&=0x1F;
											}
											else
											{
												switch(x)
												{
													case 60://600 D7~D5:001
														p8[4]&=0x3F;
														p8[4]|=0x20;
														break;
													case 12://1200 D7~D5:010
														p8[4]&=0x5F;
														p8[4]|=0x40;
														break;
													case 24://2400 D7~D5:011
														p8[4]&=0x7F;
														p8[4]|=0x60;
														break;
													case 48://4800 D7~D5:100
														p8[4]&=0x9F;
														p8[4]|=0x80;
														break;
													case 72://7200 D7~D5:101
														p8[4]&=0xBF;
														p8[4]|=0xA0;
														break;
													case 96://9600 D7~D5:110
														p8[4]&=0xDF;
														p8[4]|=0xC0;
														break;
													case 19://19200 D7~D5:111
														p8[4]&=0xfF;
														p8[4]|=0xE0;
														break;
													default:
														break;
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											x=p8_bcd(p,2);
											p8[4]=(p8[4]&0xE0)+x;
											p=(u8 *)(ADDR_STRINGBUFF+(5-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											if(p[0]==0xA0 && p[1]==0x4A)
											{//��ѹ�ز� 31
												p8[5]=31;
											}
											else
											{
												if(p[9*2]==0x30)
												{//DL/T645-2007 30
													p8[5]=30;
												}
												else
												{
													if(p[0]==0x5C && p[1]==0x50)
													{//�������� 2
														p8[5]=2;
													}
													else
													{
														if(p[9*2]==0x39)
														{//DL/T645-1997 1
															p8[5]=1;
														}
														else
														{
#if ((Project/100)==3 && (USER/100)==10)//�ӱ�ר��
															if(p[0]==0x5C && p[1]==0x47)//���� 0
															{
																p8[5]=0;
															}
															else
															{//��ʤ��Լ
																p8[5]=7;
															}
#elif ((USER/100)==5)//�Ϻ��û�
															if(p[0]==0x5C && p[1]==0x47)//���� 0
															{
																p8[5]=0;
															}
															else
															{//�Ϻ���Լ
																p8[5]=21;
															}
#elif ((Project/100)==3 && (USER/100)==12)//�Ĵ�ר��
															if(p[0]==0x5C && p[1]==0x47)//���� 0
															{
																p8[5]=0;
															}
															else
															{//�����Լ
																p8[5]=4;
															}
#else
//if(p[0]==0x5C && p[1]==0x47)//���� 0
															p8[5]=0;
#endif
														}
													}
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(6-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											for(i=0;i<=5;i++)
											{
												if(p[(11-(2*i+1))*2]>=0x30 && p[(11-(2*i+1))*2]<=0x39)
												{
													if(p[(11-2*i)*2]>=0x30 && p[(11-2*i)*2]<=0x39)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x30)*1;
													}
													if(p[(11-2*i)*2]>=0x41 && p[(11-2*i)*2]<=0x46)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x37)*1;
													}
												}
												if(p[(11-(2*i+1))*2]>=0x41 && p[(11-(2*i+1))*2]<=0x46)
												{
													if(p[(11-2*i)*2]>=0x30 && p[(11-2*i)*2]<=0x39)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x37)*16+(p[(11-2*i)*2]-0x30)*1;
													}
													if(p[(11-2*i)*2]>=0x41 && p[(11-2*i)*2]<=0x46)
													{
														p8[6+i]=(p[(11-(2*i+1))*2]-0x37)*16+(p[(11-2*i)*2]-0x37)*1;
													}
												}
											}
											p=(u8 *)(ADDR_STRINGBUFF+(7-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											for(i=0;i<=5;i++)
											{
												p8[20+i]=(p[(11-(2*i+1))*2]-0x30)*16+(p[(11-2*i)*2]-0x30)*1;
											}
											p=(u8 *)(ADDR_STRINGBUFF+(8-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
											p8[26]=((p8_bcd(p,2))<<4)+(p8_bcd(p+84,2));
			//����ΪAFN04F10д��������ԭ���ݱȽ�
											for(i=0;i<=(LEN_AFN04F10_Pn-1);i++)
											{
												if(p8[i]!=p8d[i])
												{
													break;
												}
											}
											if(i!=LEN_AFN04F10_Pn)
											{//д��������ԭ���ݲ���ͬ
												Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
											}
			//����ΪAFN04F10д��������ԭ���ݱȽ�
											if(p8[18]==0)
											{//����Ϊ0
												p8[18]=4;//Ĭ��д�����4
											}
											MW(ADDR_DATABUFF,ADDR_AFN04F10+LEN_AFN04F10_Pn*(MenuCount->v3-1),LEN_AFN04F10_Pn);
	
											PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
											AFN04F150_Computer();//��������Ч��־����
											Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
	
	//�˵����ж�AFN04F10���õĵط�������������ű仯�������Ƿ����Ϲ��ܡ�
	//Init_Class1Data(pnType,Pn);//��1���ݳ�ʼ��(ָ��Pn����)
	//Init_Class2Data(Pn);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
	//pnType�궨��Ϊ���������ͣ���ֱ���ô�����;PnΪ��1��ʼ�Ĳ������
	
											if((p8[2]+(p8[3]<<8))!=(p8d[2]+(p8d[3]<<8)))
											{//������Ų�ͬ
												if((p8d[2]+(p8d[3]<<8))!=0)
												{
													Init_Class1Data(pnType,p8d[2]+(p8d[3]<<8));//��1���ݳ�ʼ��(ָ��Pn����)
													Init_Class2Data(p8d[2]+(p8d[3]<<8));//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
												}
												if((p8[2]+(p8[3]<<8))!=0)
												{//�²������
													Init_Class1Data(pnType,p8[2]+(p8[3]<<8));//��1���ݳ�ʼ��(ָ��Pn����)
													Init_Class2Data(p8[2]+(p8[3]<<8));//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
												}
											}
											else
											{
												if((p8d[2]+(p8d[3]<<8))!=0)
												{//ԭ�������
													for(i=6;i<=12;i++)
													{
														if(p8[i]!=p8d[i])
														{
															break;
														}
													}
													if(i!=13)
													{//ͨ�ŵ�ַ�仯
														Init_Class1Data(pnType,p8d[2]+(p8d[3]<<8));//��1���ݳ�ʼ��(ָ��Pn����)
														Init_Class2Data(p8d[2]+(p8d[3]<<8));//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
													}
												}
											}
										}
									}
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn || x<1)
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>MaxRS485AddCarrierPn)
								{
									bcd_p8(p8,0,4);
								}
								break;
							case 3:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>31 || x<1)
								{
									bcd_p8(p8,1,2);
								}
								break;
							case 5:
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,0,12);
								}
								break;
							case 7:
								break;
							case 8:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>15 || x<1)
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 9:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(x>15 || x<1)
								{
									bcd_p8(p8,0,2);
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
	//ͨ�Ŷ˿ں�
	p8=(u8 *)(ADDR_STRINGBUFF+(4-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
	x=p8_bcd(p8,2);
	switch(x)
	{
		case 1:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,":���� ");			
			break;
		case 2:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,":485I ");			
			break;
		case 3:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,":485II");			
			break;
		case 31:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,":�ز� ");			
			break;
		default:
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,"      ");			
			break;
	}
}
	#endif
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8_9
// Descriptions:        �ն���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_8_9(void)
{
#if MaxImpInPort
	u8 *p8;
	u8 *p;
	u8 *p8d;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8_9);//��ʾ�˵�
			MenuCount->v1=10;
			MenuCount->v2=11;
			if(MenuCount->v3>MaxImpInPort)
			{//MenuCount->v3��¼��������˿ں�
				MenuCount->v3=MaxImpInPort;
			}
			if(MenuCount->v3<1)
			{
				MenuCount->v3=1;
			}		
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
			bcd_p8(p8,MenuCount->v3,2);
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//��ʾ�ڴ�������
			//����˿������������
			x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+1,1);
			if(x)
			{//�����㲻Ϊ0ʱ��ʾ��Ӧ�Ĳ�����š��������ԡ������
				p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
				bcd_p8(p,x,2);
				//��������
				x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+2,1);
				DisplayString(Ln+3,10,0,(u8 *)PulseAttributeList[x&0x3]);//���������б�
				//�����
				x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+3,2);			
				p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
				bcd_p8(p,x,5);
			}
			break;
		case 1://����1:ѡ��
			for(j=0;j<2;j++)
			{
				if(Stack->MenuStack[MenuCount->Count].EditCol==0)
				{
					BoxSelect();
					if(Stack->MenuStack[MenuCount->Count].EditRow!=5)
					{
						DisplayClrRow_4(Ln+5);//����Ļ��ʾ��ָ����(4�������ֲ���)
						DisplayString(Ln+5,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=10;
							MenuCount->v2=11;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=10;
							MenuCount->v2=11;
							x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+1,1);
							p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
							if(p[0]==0x0)
							{
								bcd_p8(p,x,2);
							}
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=10;
							MenuCount->v2=17;
							x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+2,1);
							p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
							if(p[0]==0x0)
							{
								DisplayString(Ln+3,10,0,(u8 *)PulseAttributeList[x&0x3]);//���������б�
							}
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=10;
							MenuCount->v2=14;
							x=MRR(ADDR_AFN04F11+5*(MenuCount->v3-1)+3,2);			
							p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
							if(p[0]==0x0)
							{
								bcd_p8(p,x,5);
							}
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x<MaxImpInPort)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,1,2);
								}
								MenuCount->v3=p8_bcd(p8,2);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x<MaxRS485AddCarrierPn && x<64)
								{
									p8_add1(p8,2);
								}
								else
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
								switch(p8[0*2])
								{
									case 0x80://����
										switch(p8[4*2])
										{
											case 0xAE://�й�
												i=1;
												break;
											case 0x1E://�޹�
												i=2;
												break;
										}
										break;
									case 0xE6://����
										switch(p8[4*2])
										{
											case 0xAE://�й�
												i=3;
												break;
											case 0x1E://�޹�
												i=0;
												break;
										}
										break;
								}
								DisplayString(Ln+3,10,0,(u8 *)PulseAttributeList[i]);//���������б�
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x<65535)
								{
									p8_add1(p8,5);
								}
								else
								{
									bcd_p8(p8,0,5);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x>1)
								{
									p8_sub1(p8,2);
								}
								else
								{
									bcd_p8(p8,MaxImpInPort,2);
								}
								MenuCount->v3=p8_bcd(p8,2);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x>0)
								{
									p8_sub1(p8,2);
								}
								else
								{
									if(MaxRS485AddCarrierPn<64)
									{
										bcd_p8(p8,MaxRS485AddCarrierPn,2);
									}
									else
									{
										bcd_p8(p8,64,2);
									}
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
								switch(p8[0*2])
								{
									case 0x80://����
										switch(p8[4*2])
										{
											case 0xAE://�й�
												i=3;
												break;
											case 0x1E://�޹�
												i=0;
												break;
										}
										break;
									case 0xE6://����
										switch(p8[4*2])
										{
											case 0xAE://�й�
												i=1;
												break;
											case 0x1E://�޹�
												i=2;
												break;
										}
										break;
								}
								DisplayString(Ln+3,10,0,(u8 *)PulseAttributeList[i]);//���������б�
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>0)
								{
									p8_sub1(p8,5);
								}
								else
								{
									bcd_p8(p8,65535,5);
								}
								break;
							default:
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x10)!=0)
					{//ȷ��
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 2:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 5:
								p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
								p8=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
								if(p8_bcd(p8,2)>MaxRS485AddCarrierPn)
								{//�����㳬�����ֵ �򲻿�����
									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,"��Ч������");
								}
								else
								{
									if(MeterPnCompare(MaxRS485AddCarrierPn+1,p8_bcd(p8,2)))
									{//���������ظ� ��������
										DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,"�������ظ�");
									}
									else
									{//�������㲻�ظ�
										if(PulsePnCompare(p8_bcd(p,2),p8_bcd(p8,2)))
										{//�����������ظ� �ж���������
											p8=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
											switch(p8[0*2])
											{
												case 0x80://����
													switch(p8[4*2])
													{
														case 0xAE://�й�
															i=0;
															break;
														case 0x1E://�޹�
															i=1;
															break;
													}
													break;
												case 0xE6://����
													switch(p8[4*2])
													{
														case 0xAE://�й�
															i=2;
															break;
														case 0x1E://�޹�
															i=3;
															break;
													}
													break;
											}
											p8=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
											for(k=0;k<MaxImpInPort;k++)
											{
												if(k!=MenuCount->v3-1)
												{
													if(p8_bcd(p8,2) == MRR(ADDR_AFN04F11+5*k+1,1) && i == MRR(ADDR_AFN04F11+5*(k)+2,1))
													{
														break;
													}
												}
											}
											if(k == MaxImpInPort)
											{//�������Բ��ظ� ������
												DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
												DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
												//�������õ�����
												p8=(u8 *)ADDR_DATABUFF;
												MR(ADDR_DATABUFF,ADDR_AFN04F11+5*(MenuCount->v3-1),5);
												p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
												p8[0]=p8_bcd(p,1);
												p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
												p8[1]=p8_bcd(p,2);
												p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
												switch(p[0*2])
												{
													case 0x80://����
														switch(p[4*2])
														{
															case 0xAE://�й�
																i=0;
																break;
															case 0x1E://�޹�
																i=1;
																break;
														}
														break;
													case 0xE6://����
														switch(p[4*2])
														{
															case 0xAE://�й�
																i=2;
																break;
															case 0x1E://�޹�
																i=3;
																break;
														}
														break;
												}
												p8[2]=(p8[2]&0xFC)+i;
												p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
												x=p8_bcd(p,5);
												p8[3]=(x&0xff);
												p8[4]=(x>>8);
												MW(ADDR_DATABUFF,ADDR_AFN04F11+5*(MenuCount->v3-1),5);
				//����ΪAFN04F11д��������ԭ���ݱȽ�
												p8d=(u8 *)(ADDR_AFN04F11+5*(MenuCount->v3-1));
												for(i=0;i<5;i++)
												{
													if(p8[i]!=p8d[i])
													{
														break;
													}
												}
												if(i!=5)
												{//д��������ԭ���ݲ���ͬ
													Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
												}
				//����ΪAFN04F11д��������ԭ���ݱȽ�
											}
											else
											{
												DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
												DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,6,0,"�����ظ�");
											}
										}
										else
										{
											DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
											DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
											//�������õ�����
											p8=(u8 *)ADDR_DATABUFF;
											MR(ADDR_DATABUFF,ADDR_AFN04F11+5*(MenuCount->v3-1),5);
											p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
											p8[0]=p8_bcd(p,1);
											p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+10*2);
											p8[1]=p8_bcd(p,2);
											p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+10*2);
											switch(p[0*2])
											{
												case 0x80://����
													switch(p[4*2])
													{
														case 0xAE://�й�
															i=0;
															break;
														case 0x1E://�޹�
															i=1;
															break;
													}
													break;
												case 0xE6://����
													switch(p[4*2])
													{
														case 0xAE://�й�
															i=2;
															break;
														case 0x1E://�޹�
															i=3;
															break;
													}
													break;
											}
											p8[2]=(p8[2]&0xFC)+i;
											p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+10*2);
											x=p8_bcd(p,5);
											p8[3]=(x&0xff);
											p8[4]=(x>>8);
											MW(ADDR_DATABUFF,ADDR_AFN04F11+5*(MenuCount->v3-1),5);
			//����ΪAFN04F11д��������ԭ���ݱȽ�
											p8d=(u8 *)(ADDR_AFN04F11+5*(MenuCount->v3-1));
											for(i=0;i<5;i++)
											{
												if(p8[i]!=p8d[i])
												{
													break;
												}
											}
											if(i!=5)
											{//д��������ԭ���ݲ���ͬ
												Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
											}
			//����ΪAFN04F11д��������ԭ���ݱȽ�
										}
									}
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
						{
							case 1:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x>MaxImpInPort || x<1)
								{
									bcd_p8(p8,1,2);
								}
								MenuCount->v3=p8_bcd(p8,2);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,2);
								if(x>MaxRS485AddCarrierPn)
								{
									bcd_p8(p8,0,2);
								}
								break;
							case 4:
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,5);
								if(x>65535)
								{
									bcd_p8(p8,0,5);
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
#endif
}

#if ((USER/100)==5)//�Ϻ��û�
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_SH_8_10
// Descriptions:        SIM���ֻ���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_SH_8_10(void)
{
	u8 *p8;
	u8 *p8d;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u64 x;
	u32 Ln;
#if ((Project/100)==3)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"SIM���ֻ�������\n\r"
																			" \n\r"
																			"      ȷ������\n\r"
																			);//��ʾ�˵�
			/*��ʾ�ڴ�������*/
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+2*2);
			x=MRR(ADDR_AFN04F243,8);
			DisplayData(x,16,0,p16);

			MenuCount->v1=2;
			MenuCount->v2=17;
			ArrowLR(Ln+1,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
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
							MenuCount->v1=2;
							MenuCount->v2=17;
							ArrowLR(Ln+1,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2:
							MenuCount->v1=0;
							for(k=0;k<10;k++)
							{
								if(k!=(Ln+2))
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8_add1(p8,16);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
								p8_sub1(p8,16);
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
								DisplayString(Ln+1,18,0,"��");
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,9,0,"OK");
								//�������õ�����
								MR(ADDR_DATABUFF,ADDR_AFN04F243,8);//��SIM���ֻ�����
								p8d=(u8 *)ADDR_DATABUFF;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+2*2);
								for(i=0;i<8;i++)
								{
									p8d[7-i]=((p8[4*i]-0x30)*16)+(p8[2+4*i]-0x30);
								}
								MW(ADDR_DATABUFF,ADDR_AFN04F243,8);//дSIM���ֻ�����
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
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+18*2);
								p16[0]=32;//'���Ҽ�ͷ'
								p16[1]=33;
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
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8
// Descriptions:        �ն˲������ò˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
#if ((Project/100)==3 || (Project/100)==4)//ר���նˡ������ն�
	#if ((Project/100)==3 && ((USER/100)==5 || (USER/100)==12) || (USER/100==13))//�Ϻ�ר�䡢�Ĵ�ר�䡢������
void Menu_8(void)//�˵�8
{
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
	u8 * p8;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																			"�ն˲�������\n\r"
																			"  1.SIM���ֻ�������\n\r"
																			"  2.�ն˵�ַ����\n\r"
																			"  3.ͨ�Ų�������\n\r"
																			"  4.IP���˿ں�����\n\r"
																			"  5.APN����\n\r"
																			"  6.ר���û�������\n\r"
																			"  7.ר����������\n\r"
																			"  8.���ܱ�����\n\r"
																			"  9.�ն���������\n\r"
																			"  10.���ں�ʱ������\n\r"
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
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
						MenuCreate((u32)Menu_SH_8_10);//�����¼��˵� SIM���ֻ���������
#endif
						break;
					case 2:
						MenuCreate((u32)Menu_8_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu_8_3);//�����¼��˵�
						break;
					case 4:
						MenuCreate((u32)Menu_8_4);//�����¼��˵�
						break;
					case 5:
						DisplayClr();
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
						p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
						p8[0]=0;//�˵���ʾ���������ã�0=��1=��
#endif
						MenuCreate((u32)Menu_8_5);//�����¼��˵�
						break;
					case 6:
					case 7:
						DisplayClr();
						MenuCount->v1=1;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
						p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
						p8[0]=0;//�˵���ʾ���������ã�0=��1=��
#endif
						MenuCreate((u32)Menu_8_7);//�����¼��˵�
						break;
					case 8:
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13))//�Ϻ�ר�䡢������
						p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
						p8[0]=0;//�˵���ʾ���������ã�0=��1=��
#endif
						MenuCreate((u32)Menu_8_8);//�����¼��˵�
						break;
					case 9:
						MenuCreate((u32)Menu_8_9);//�����¼��˵�
						break;
					case 10:
						MenuCreate((u32)Menu_8_1);//�����¼��˵� ʱ�����������            
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
	#else
void Menu_8(void)//�˵�8
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
		#if (MaxImpInPort)//������
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_8);//��ʾ�˵�
		#else
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu1_8);//��ʾ�˵�
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
						MenuCreate((u32)Menu_8_1);//�����¼��˵�            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2);//�����¼��˵�
						break;
					case 3:
						MenuCreate((u32)Menu_8_3);//�����¼��˵�
						break;
					case 4:
						MenuCreate((u32)Menu_8_4);//�����¼��˵�
						break;
					case 5:
						DisplayClr();
						MenuCreate((u32)Menu_8_5);//�����¼��˵�
						break;
					case 6:
					case 7:
						DisplayClr();
						MenuCount->v1=1;
						MenuCreate((u32)Menu_8_7);//�����¼��˵�
						break;
					case 8:
						MenuCreate((u32)Menu_8_8);//�����¼��˵�
						break;
					case 9:
					#if (MaxImpInPort)//������
						MenuCreate((u32)Menu_8_9);//�����¼��˵�
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
	#endif
#endif
#if ((Project/100)==2)//������
//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_3
// Descriptions:        �ŵ���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_3(void)//�ŵ���������
{
	DisplayClr();
	DisplayString(4,1,0,"��ǰ����Ӧ��������");
	DisplayString(5,4,0,"�����������");
	Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
	if((Comm_Ram->ButtonVal&0x3F)!=0)
	{
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_1
// Descriptions:        ͨ����ϸ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1(void)//ͨ����ϸ����
{
#if ((USER/100)==11)//����
	u8 * p8;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"ͨ����ϸ����\n\r"
																		"  1.��վIP���˿�����\n\r"
																		"  2.APN����\n\r"
																		"  3.�ն�IP����\n\r"
																		"  4.�ն������˿�����\n\r"
																		"  5.MAC��ַ����\n\r"
																	#if ((USER/100)==5)//�Ϻ��û�
																		"  6.SIM���ֻ�������\n\r"
																	#endif
																	#if ((USER/100)==11)//����
																		"  6.ר���û�������\n\r"
																		"  7.ר����������\n\r"
																	#endif
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
						MenuCreate((u32)Menu_8_4);//��վIP���˿�����           
						break;
					case 2:
						DisplayClr();
					#if ((USER/100)==11)//����
						p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
						p8[0]=0;//�˵���ʾ���������ã�0=��1=��
					#endif
						MenuCreate((u32)Menu_8_5);//APN����           
						break;
					case 3:
						MenuCreate((u32)Menu_8_2_3);//�ն�IP����           
						break;
					case 4:
						MenuCreate((u32)Menu_8_2_6);//�ն������˿�����           
						break;
					case 5:
						MenuCreate((u32)Menu_8_2_4);//MAC��ַ����           
						break;
				#if ((USER/100)==5)//�Ϻ��û�
					case 6:
						MenuCreate((u32)Menu_SH_8_10);//SIM���ֻ�������
						break;
				#endif
				#if ((USER/100)==11)//����
					case 6:
					case 7:
						DisplayClr();
						MenuCount->v1=1;
						p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
						p8[0]=0;//�˵���ʾ���������ã�0=��1=��
						MenuCreate((u32)Menu_8_7);//�����¼��˵�
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1
// Descriptions:        ͨ��ͨ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1(void)//ͨ��ͨ�����ã��ŵ��������á�ͨ��ģʽ���á�ͨ����ϸ���ã�
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"ͨ��ͨ������\n\r"
																		"  1.ͨ����ϸ����\n\r"
																		"  2.ͨ��ģʽ����\n\r"
																		"  3.�ŵ���������\n\r"
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
						MenuCreate((u32)Menu2_2_1_1);//ͨ����ϸ����           
						break;
					case 2:
						MenuCreate((u32)Menu_8_3);//ͨ��ģʽ����           
						break;
					case 3:
						MenuCreate((u32)Menu2_2_1_3);//�ŵ���������           
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
// Function name:       Menu2_5
// Descriptions:        �������������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_5(void)//�������������
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"�������������\n\r"
																		"  1.���������������\n\r"
																		"  2.��������ַ����\n\r"
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
						MenuCreate((u32)Menu_8_2_1);//��������������            
						break;
					case 2:
						MenuCreate((u32)Menu_8_2_2);//�ն˵�ַ����            
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
// Function name:       Menu2_2
// Descriptions:        ����������������鿴
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2(void)//����������������鿴
{
#if ((USER/100)==11)//����
u8 * p8;
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2);//��ʾ�˵�
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
						MenuCreate((u32)Menu2_2_1);//ͨ��ͨ�����ã��ŵ��������á�ͨ��ģʽ���á�ͨ����ϸ���ã�       
						break;
					case 2:
					#if ((USER/100)==11)//����
						p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
						p8[0]=0;//�˵���ʾ���������ã�0=��1=��
					#endif
						MenuCreate((u32)Menu_8_8);//���ܱ��������
						break;
					case 3:
						MenuCreate((u32)Menu_8_1);//���ں�ʱ������
						break;
					case 4:
						MenuCreate((u32)Menu_9_5);//������������
						break;
					case 5:
						MenuCreate((u32)Menu2_5);//�������������
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
/*
//20120718��������������Ա��ʱ��Ҫ���ԭ�����㡢���ú�������1�����ݡ����µ������ȡ�
//��ʱ�˵������ò�����ʱ��û�г�ʼ����Щ���ݡ�
	x=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5)+1,2);//ԭ������ź���������
	MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5),5);
	y=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5)+1,2);//���ú������ź���������
	if(x!=y)
	{//������Ż��������Ա仯
		x&=0xff;
		y&=0xff;
		Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
		Init_Class1Data_PnMap(pnType,x);//��1���ݳ�ʼ��(ָ��Pn����),Pnӳ����Pn
		Init_Class1Data_PnMap(pnType,y);//��1���ݳ�ʼ��(ָ��Pn����),Pnӳ����Pn
		//x=MapPn(x);//������ӳ��Pnֵ
		//y=MapPn(y);//������ӳ��Pnֵ
		Init_EnergyInc_Souser_Pn(x);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
		Init_EnergyInc_Souser_Pn(y);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
		Init_Energy_Pn(x);//���������µ�������Ϊ0(ָ����Pn����)
		Init_Energy_Pn(y);//���������µ�������Ϊ0(ָ����Pn����)
	}
//Init_Class1Data(pnType,Pn);//��1���ݳ�ʼ��(ָ��Pn����)
//Init_Class2Data(Pn);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
//pnType�궨��Ϊ���������ͣ���ֱ���ô�����;PnΪ��1��ʼ�Ĳ������

Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr)//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
AFN04F150_Computer();//��������Ч��־����
Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
*/
