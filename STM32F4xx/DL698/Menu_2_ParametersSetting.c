
//�˵�2 �ն˲�������
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/RTC.h"
#include "../DL698/DL698_DataDef.h"

extern u64 Get_TSA_BY_PN(u32 Pn);//����������ţ��õ�TSA���������������͡�
extern const u8 UnitList[][6];//ͨ�õ�λ�б�
extern const u8 ProtocolList[][13];

#if ((Project/100)==2)//������

const u8 pMenu2_2[]=//����������������鿴
{
	"����������鿴\n\r"
	"  1.ͨ��ͨ������\n\r"
	"  2.����������\n\r"
	"  3.������ʱ������\n\r"
	"  4.������������\n\r"
	"  5.���������\n\r"
};
#endif
#if ((Project/100)==3)//ר��

const u8 pMenu2_2[]=//����������������鿴
{
	"����������鿴\n\r"
	"  1.ͨ��ͨ������\n\r"
	"  2.����������\n\r"
	"  3.�ն�ʱ������\n\r"
	"  4.������������\n\r"
	"  5.�ն˱��\n\r"
};
#endif

const u8 pMenu2_2_1[]=
{
	"  1.���߹�������\n\r"
	"  2.��̫������\n\r"
};
const u8 pMenu2_2_2[]=
{
	"���ܱ��������\n\r"
	"�������\n\r"
	"������\n\r"
	"ͨ�Ŷ˿�\n\r"
	"ͨ�Ź�Լ\n\r"
	"��ַ\n\r"
	"������\n\r"
	"���߷�ʽ\n\r"
	"�ɼ���\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_3[]=
{
	"ʱ������\n\r"
	"����:\n\r"
	"ʱ��:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_5[]=
{
	"ͨ�ŵ�ַ����\n\r"
	"��ַ\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_4[]=
{
	"�˵�����\n\r"
	"����:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_1_1[]=
{
	"���߹�������\n\r"
	"  1.ͨ��ģʽ����\n\r"
	"  2.�����˿�����\n\r"
	"  3.��վͨ�Ų�������\n\r"
	"  4.APN����\n\r"
	"  5.�û�������\n\r"
	"  6.��������\n\r"
};

const u8 pMenu2_2_1_1_1[]=
{
	"ͨ��ģʽ����\n\r"
	"����ģʽ:\n\r"
	"���߷�ʽ:\n\r"
	"���ӷ�ʽ:\n\r"
	"Ӧ�÷�ʽ:\n\r"
	"��ʱʱ��:\n\r"
	"�ط�����:\n\r"
	"��������:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_1_1_2[]=
{
	"�����˿�����\n\r"
	"�����˿�:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_1_1_3[]=
{
	"��վIP���˿�����\n\r"
	"��IP:\n\r"
	"���˿�:\n\r"
	"��IP:\n\r"
	"���˿�:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_1_2[]=
{
	"��̫������\n\r"
	"  1.ͨ������\n\r"
	"  2.�����˿�����\n\r"
	"  3.��վͨ�Ų�������\n\r"
	"  4.�ն�IP����\n\r"
	"  5.PPPoE�û�������\n\r"
	"  6.PPPoE��������\n\r"
	"  7.MAC��ַ����\n\r"
};

const u8 pMenu2_2_1_2_1[]=
{
	"ͨ������\n\r"
	"����ģʽ:\n\r"
	"���ӷ�ʽ:\n\r"
	"Ӧ�÷�ʽ:\n\r"
	"��ʱʱ��:\n\r"
	"�ط�����:\n\r"
	"��������:\n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_1_2_4[]=
{
	"�ն�IP����\n\r"
	"����:\n\r"
	"IP  :\n\r"
	"����:\n\r"
	"����:\n\r"
	"      ȷ������\n\r"
};


const u8 pMenu2_2_1_2_6[]=
{
	"MAC��ַ����\n\r"
	"  1.ʮ����\n\r"
	"  2.ʮ������\n\r"
};

const u8 pMenu2_2_1_2_6_1[]=
{
	"MAC��ַ����\n\r"
	"MAC:\n\r"
	" \n\r"
	"      ȷ������\n\r"
};

const u8 pMenu2_2_1_2_6_2[]=
{
	"MAC��ַ����\n\r"
	"MAC\n\r"
	"      ȷ������\n\r"
};

const u8 IpTypeList[][7]=//IP�����б�
{
	"DHCP ",//0
	"��̬ ",//1
	"PPPoE",//2
};




const u8 OnlineModeList[][9]=//����ģʽ�б�
{
	"��������", //0
	"��������", //1
};

const u8 WorkModeList[][7]=//����ģʽ�б�
{
	"���  ",   //0
	"�ͻ���", //1
	"������", //2
};

const u8 ConnectList[][4]=//���ӷ�ʽ�б�
{
	"TCP",   //0
	"UDP",   //1
};

const u8 ApplicationList[][7]=//���ӷ�ʽ�б�
{
	"����  ",   //0
	"������",   //1
};

const u8 CommunicationSpeedList[][7]=//ͨ�������б�
{
	"300   ",      //0
	"600   ",     //1
	"1200  ",     //2
	"2400  ",     //3
	"4800  ",     //4
	"7200  ",     //5
	"9600  ",     //6
	"19200 ",     //7
	"38400 ",     //8
	"57600 ",     //9
	"115200",    //10
	"����Ӧ",    //255
};


const u8 PortList[][13]=//ͨ��Э���б�
{
	"δ֪        ",//0 
	"����        ",
	"485-1       ",
	"485-2       ",
	"�ز�        ",
};

const u8 ConnectType[][9]=//���߷�ʽ
{
	"δ֪    ",
	"����    ",
	"��������",
	"��������"
};

const u8 default_6000[]={0x02,0x04,
						0x12,0x00,0x00,
						0x02,0x0a,
							0x55,0x07,0x05,0x00,0x00,0x00,0x00,0x00,0x00,
							0x16,0xff, //����������Ӧ
							0x16,0x00,// ��Լ δ֪
							0x51,0xe2,0x01,0x02,0x01, //�˿� δ֪
							0x09,0x02,0x00,0x00,
							0x11,0x00,//���ʸ��� 0
							0x11,0x00,
							0x16,0x01,
							0x12,0x08,0x98,
							0x12,0x00,0x0f,
						0x02,0x04,
							0x55,0x07,0x05,0x00,0x00,0x00,0x00,0x00,0x00,
							0x09,0x02,0x00,0x00,
							0x12,0x00,0x01,
							0x12,0x00,0x01,
						0x01,0x00
	                    };


//���·ǹ̶����ȵĳ�Ա���� ������ DataType_bit_string  DataType_octet_string  DataType_visible_string,�����ǲ�������256�ġ�
//�ɹ�����1 ʧ�ܷ���0 
//������� u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data, ͬ Get_ElementADDR(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data);
//u32 New_data_addr,u32 New_data_len ��Ա�����ݵĵ�ַ�ͳ���
u32 Update_ElementDATA_unfixed(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data,u32 New_data_addr,u32 New_data_len)
{
	//�������ȴ�ŵ�ADDR_128KDATABUFF+32K�ĵط�
	u32 old_addr;
	u32 perupdate_datalen;//����ǰ,�ĳ�Ա�ĳ��ȣ���������������һ�ֽڵ�
	u32 perupdate_sumlen;//����ǰ�����ṹ�ĳ��ȣ������жϸ��º���ܳ��Ƿ�ᳬ��LENmax_Data
	u32 len;
//	u8 *p;
	u32 DataType;
	old_addr=Get_ElementADDR(ADDR_Data,Element,SubElement,LENmax_Data);//����Ԫ�ص�ַ(ʵ�ʵ�ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
	if(old_addr==0)//Ҫ���µĳ�Ա��ַ������
	{
		return 1;
	}
	DataType=MRR(old_addr,1);
	perupdate_datalen=Get_DL698DataLen_S((u8 *)old_addr,LENmax_Data);
	perupdate_sumlen=Get_DL698DataLen_S((u8 *)ADDR_Data,LENmax_Data);
	if(New_data_len>(perupdate_datalen-1))//���º�ĳ��������ӵ�
	{
		if((New_data_len-(perupdate_datalen-1))>(LENmax_Data-perupdate_sumlen))//ʣ���ֽڲ�����
		{
			return 1;
		}
	}
//	p=(u8 *)ADDR_128KDATABUFF+32*1024;
	MR(ADDR_128KDATABUFF+32*1024,ADDR_Data,old_addr-ADDR_Data+1);//�ȴ�����ǰ������ + ����������������һλ
	len=old_addr-ADDR_Data+1;
	if(DataType==DataType_TSA)//TSA�ر� 55 07 05 00 00 00 00 00 00 
	{
		MWR(New_data_len+1,ADDR_128KDATABUFF+32*1024+len,1);
		len+=1;
		MWR(New_data_len-1,ADDR_128KDATABUFF+32*1024+len,1);
		len+=1;
	}
	else if(DataType==DataType_array){//����һ���Ա����Ա������ARRAY �� STRUCTUCE
		;
	}
	else
	{
		MWR(New_data_len,ADDR_128KDATABUFF+32*1024+len,1);
		len+=1;
	}
	MR(ADDR_128KDATABUFF+32*1024+len,New_data_addr,New_data_len);//�� ���µ���Щ���� 
	len+=New_data_len;
	MR(ADDR_128KDATABUFF+32*1024+len,ADDR_Data+(old_addr-ADDR_Data)+perupdate_datalen,perupdate_sumlen-(old_addr-ADDR_Data)-perupdate_datalen);//���ϸ��µ�󲻱��Ա������
	len+=(perupdate_sumlen-(old_addr-ADDR_Data)-perupdate_datalen);
	MW(ADDR_128KDATABUFF+32*1024,ADDR_Data,len);
	return 0;
}
//���¹̶����ȵĳ�Ա���� 
//������� u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data, ͬ Get_ElementADDR(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data);
//u64 New_data,u32 New_data_len
//�ɹ�����1 ʧ�ܷ���0 
u32 Update_ElementDATA_fixed(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data,u32 New_data_addr,u32 New_data_byte)
{
	u32 ret;
	ret=Get_ElementADDR(ADDR_Data,Element,SubElement,LENmax_Data);//����Ԫ�ص�ַ(ʵ�ʵ�ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
	if(ret==0)//Ҫ���µĳ�Ա��ַ������
	{
		return 1;
	}
	MW(New_data_addr,ret+1,New_data_byte);
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_1_1
// Descriptions:        ͨ��ģʽ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_1(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_1);//��ʾ�˵�
			//����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
			p8=Get_Element((u8*)ADDR_4500_2,1,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+1,10,0,(u8 *)WorkModeList[p8[1]&0x03]);
			}
		  //���߷�ʽ:�������ߣ�0�����������1��
			p8=Get_Element((u8*)ADDR_4500_2,2,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+2,10,0,(u8 *)OnlineModeList[p8[1]&0x03]);
			}
		  //���ӷ�ʽ:TCP��0����UDP��1��
			p8=Get_Element((u8*)ADDR_4500_2,3,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+3,10,0,(u8 *)ConnectList[p8[1]&0x03]);
			}
		  //����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
			p8=Get_Element((u8*)ADDR_4500_2,4,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+4,10,0,(u8 *)ApplicationList[p8[1]&0x03]);
			}
		  //��ʱʱ�估�ط�������bit7~bit2����ʱʱ�䣨�룩bit1~bit0���ط�����
			p8=Get_Element((u8*)ADDR_4500_2,11,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				//��ʱʱ�䣨�룩
				i=(p8[1]>>2);
				p=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
				bcd_p8(p,i,3);
				DisplayString(Ln+5,13,0,(u8 *)UnitList[9]);//0 s
				//�ط�����
				i=(p8[1]&0x03);
				p=(u8*)ADDR_STRINGBUFF+(Ln+6)*84+4+10*2;
				bcd_p8(p,i,1);
				DisplayString(Ln+6,11,0,(u8 *)UnitList[10]);//1 ��
			}
			//��������(��)  long-unsigned
			p8=Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
        		i=p8[1];
				i<<=8;
				i|=p8[2];
				p=(u8*)ADDR_STRINGBUFF+(Ln+7)*84+4+10*2;
				bcd_p8(p,i,5);
			  DisplayString(Ln+7,15,0,(u8 *)UnitList[9]);//0 s				
			}
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
					if(Stack->MenuStack[MenuCount->Count].EditRow!=8)
					{
						DisplayString(Ln+8,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1://����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
							MenuCount->v1=10;
							MenuCount->v2=15;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2://���߷�ʽ:�������ߣ�0�����������1��
							MenuCount->v1=10;
							MenuCount->v2=17;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3://���ӷ�ʽ:TCP��0����UDP��1��
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4://����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
							MenuCount->v1=10;
							MenuCount->v2=15;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5://��ʱʱ��
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6://�ط�����
							MenuCount->v1=10;
							MenuCount->v2=10;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7://��������(��)  long-unsigned
							MenuCount->v1=10;
							MenuCount->v2=14;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
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
							case 1://����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x46://��
										i=1;
										break;
									case 0xF8://��
										i=2;
										break;
									case 0x7A://��
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)WorkModeList[i]);
								break;
							case 2://���߷�ʽ:�������ߣ�0�����������1��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x8E://��
										i=1;
										break;
									case 0x8C://��
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[i]);//1 �������� 2 �������� 3 ʱ������
								break;
							case 3://���ӷ�ʽ:TCP��0����UDP��1��
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectList[i]);
								break;
							case 4://����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x30://��
										i=1;
										break;
									case 0x82://��
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ApplicationList[i]);
								break;
							case 5://��ʱʱ��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x<63)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 6://�ط�����
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x<3)
								{
									p8_add1(p8,1);
								}
								else
								{
									bcd_p8(p8,0,1);
								}
								break;
							case 7://��������(��)  long-unsigned
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
							case 1://����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x46://��
										i=2;
										break;
									case 0xF8://��
										i=0;
										break;
									case 0x7A://��
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)WorkModeList[i]);
								break;
							case 2://���߷�ʽ:�������ߣ�0�����������1��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x8E://��
										i=1;
										break;
									case 0x8C://��
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)OnlineModeList[i]);//1 �������� 2 �������� 3 ʱ������
								break;
							case 3://���ӷ�ʽ:TCP��0����UDP��1��
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectList[i]);
								break;
							case 4://����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x30://��
										i=1;
										break;
									case 0x82://��
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ApplicationList[i]);
								break;
							case 5://��ʱʱ��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,63,3);
								}
								break;
							case 6://�ط�����
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x>0)
								{
									p8_sub1(p8,1);
								}
								else
								{
									bcd_p8(p8,3,1);
								}
								break;
							case 7://��������(��)  long-unsigned
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 5://��ʱʱ��
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 6://�ط�����
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 7://��������(��)  long-unsigned
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 8://ȷ������
								DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
								//�������õ�����
								//1.����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
								x=Get_ElementADDR(ADDR_4500_2,1,0,LENmax_4500_2);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
									switch(p8[0])
									{
										case 0x46://��
											i=0;
											break;
										case 0xF8://��
											i=0x01;
											break;
										case 0x7A://��
											i=0x02;
											break;
										default:
											break;
									}
									MWR(i,x+1,1);
								}
								
								//2.���߷�ʽ:�������ߣ�0�����������1��
								x=Get_ElementADDR(ADDR_4500_2,2,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+10*2;
									switch(p8[0])
									{
										case 0x8E://��
											i=0;
											break;
										case 0x8C://��
											i=0x01;
											break;
										default:
											break;
									}
									MWR(i,x+1,1);
								}
								//3.���ӷ�ʽ:TCP��0����UDP��1��
								x=Get_ElementADDR(ADDR_4500_2,3,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+10*2;
									switch(p8[0])
									{
										case 0x54://TCP
											i=0;
											break;
										case 0x55://UDP
											i=0x01;
											break;
									}
									MWR(i,x+1,1);
								}
								//4.����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
								x=Get_ElementADDR(ADDR_4500_2,4,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)								
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
									switch(p8[0])
									{
										case 0x30://��
											i=0;
											break;
										case 0x82://��
											i=0x01;
											break;
									}
									MWR(i,x+1,1);									
								}
								//5.��ʱʱ�估�ط�������bit7~bit2����ʱʱ�䣨�룩bit1~bit0���ط�����
								x=Get_ElementADDR(ADDR_4500_2,11,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)
								{
									//��ʱʱ�䣨�룩
									p8=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
									i=p8_bcd(p8,3);
									i<<=2;
									//�ط�����
									p8=(u8*)ADDR_STRINGBUFF+(Ln+6)*84+4+10*2;
									k=p8_bcd(p8,1);
									i|=k;
									MWR(i,x+1,1);									
								}
								//7.��������(��)  long-unsigned
								x=Get_ElementADDR(ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+7)*84+4+10*2;
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);									
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
							case 5://��ʱʱ��
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>63)
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 6://�ط�����
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x>3)
								{
									bcd_p8(p8,0,1);
								}
								break;
							case 7://7.��������(��)
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
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_1_2
// Descriptions:        �����˿�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;	
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_2);//��ʾ�˵�
			p8=Get_Element((u8*)ADDR_4500_2,5,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{
				if(p8[1])
				{//���������Ϊ0����ʾ��1�������˿�
					i=p8[3];
					i<<=8;
					i|=p8[4];
					p=(u8*)ADDR_STRINGBUFF+(Ln+1)*84+4+9*2;
					bcd_p8(p,i,5);					
				}
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
						DisplayString(Ln+2,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{					
						case 1://�����˿�
							MenuCount->v1=9;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2://ȷ������
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
							case 1://�����˿�  long-unsigned
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
							case 1://�����˿�  long-unsigned
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,9,0,"OK");
								//�������õ�����
							  //�����˿�
								x=Get_ElementADDR(ADDR_4500_2,5,1,LENmax_4500_2);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=((i>>8)&0xff);
									MWR(k,x+1,2);
								}
								else//ԭ���ǿյ� 
								{
									MR(ADDR_DATABUFF,ADDR_4500_2,LENmax_4500_2);
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=((i>>8)&0xff);
									k<<=8;
									k|=0x12;//��Ա������0X12
									k<<=8;
									k|=1;//ARRAY�ĸ�����1
									Update_ElementDATA_unfixed(ADDR_DATABUFF,5,0,LENmax_4500_2,(u32)&k,4);
									MW(ADDR_DATABUFF,ADDR_4500_2,LENmax_4500_2);
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
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
// Function name:       Menu2_2_1_1_3
// Descriptions:        ��վͨ�Ų�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_3(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_3);//��ʾ�˵�
			//IP
			p8=Get_Element((u8*)ADDR_4500_3,1,1,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+(Ln+1)*84+4+5*2;
				for(i=0;i<=3;i++)
				{
					bcd_p8(p+i*8,p8[i+2],3);
				}
				for(i=0;i<=2;i++)
				{
					p[3*2+i*8]='.';
					p[3*2+i*8+1]=0x20;
				}
			}
			//�˿�
			p8=Get_Element((u8*)ADDR_4500_3,1,2,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{//�˿�
				p=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			//��IP
			p8=Get_Element((u8*)ADDR_4500_3,2,1,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+5*2;
				for(i=0;i<=3;i++)
				{
					bcd_p8(p+i*8,p8[i+2],3);
				}
				for(i=0;i<=2;i++)
				{
					p[3*2+i*8]='.';
					p[3*2+i*8+1]=0x20;
				}
			}
			//���˿�
			p8=Get_Element((u8*)ADDR_4500_3,2,2,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{//�˿�
				p=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			MenuCount->v1=5;
			MenuCount->v2=21;
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
						DisplayString(Ln+5,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{	
						case 1://IP
							MenuCount->v1=5;
							MenuCount->v2=21;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							break;
						case 2://�˿�
							MenuCount->v1=7;
							MenuCount->v2=11;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3://IP
							MenuCount->v1=5;
							MenuCount->v2=21;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							break;
						case 4://�˿�
							MenuCount->v1=7;
							MenuCount->v2=11;
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
							case 1://IP ���Ҽ�����
							case 3:
								break;	
							case 2://�˿�
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://�˿�
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP ���������
							case 3:
								break;	
							case 2://�˿�
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://�˿�
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2://�˿�
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+5,9,0,"OK");
								//�������õ�����
								x=Get_ElementADDR(ADDR_4500_3,1,1,LENmax_4500_3);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
									MW(ADDR_DATABUFF,x+2,4);//дIP
								}	
								x=Get_ElementADDR(ADDR_4500_3,1,2,LENmax_4500_3);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);//д�˿�
								}	

								x=Get_ElementADDR(ADDR_4500_3,2,1,LENmax_4500_3);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
									MW(ADDR_DATABUFF,x+2,4);//дIP
								}	
								x=Get_ElementADDR(ADDR_4500_3,2,2,LENmax_4500_3);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);//д�˿�
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
// Function name:       Menu2_2_1_1_4
// Descriptions:        APN����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_4(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
//	u32 k;
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
			p8=" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayClr();//��ȫ��
			DisplayString(0,6,0,"APN����");
			DisplayString(MinLn,MinCol,0,p8);
			p8=Get_Element((u8*)ADDR_4500_2,6,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			MC(0,ADDR_DATABUFF,LENmax_4500_2);
			if(p8)
			{
				MenuCount->v3=p8[1];//APN����
				p8+=2;
				if(MenuCount->v3>16)
				{//�ݹ̶�APN����Ϊ16�ֽ�
					MenuCount->v3=16;
					
				}
				MC(0,ADDR_DATABUFF+MenuCount->v3,1);
				MR(ADDR_DATABUFF,(u32)p8,MenuCount->v3);
			}
			p=(u8 *)ADDR_DATABUFF;
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
					MC(0,ADDR_DATABUFF,LENmax_4500_2);
					p8=(u8 *)ADDR_DATABUFF;
					p=(u8 *)(ADDR_STRINGBUFF+(EditLn)*84+4+(EditCol)*2);
					for(i=0;i<=15;i++)
					{
						if(p[i*2]!=0x20 && p[i*2]!=0x00)
						{
							p8[i]=p[i*2];
						}
						else
						{
							p8[i]=0;
							break;
						}
					}
					Update_ElementDATA_unfixed(ADDR_4500_2,6,0,LENmax_4500_2,ADDR_DATABUFF,i);
					//���óɹ���ʾ�ϣ�
					DisplayClrRow(1,9);
					DisplayString(1,0,0,p8);
					DispalyAlign(1,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					DisplayString(4,7,0,"����OK");
					DisplayString(5,4,0,"��������˳�");
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_1_5
// Descriptions:        �û������á���������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1_5(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
//	u32 x;
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
		case 0://����0����ʼ��
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayClr();//��ȫ��
			DisplayString(MinLn,MinCol,0,p8);
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow)
			{
				case 5:
					DisplayString(0,5,0,"�û�������");
					p=Get_Element((u8*)ADDR_4500_2,7,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					MC(0,ADDR_DATABUFF,LENmax_4500_2);
					if(p)
					{
						i=p[1];//�û�������
						p+=2;
						if(i>32)
						{//�ݹ̶��û�������32�ֽ�
							i=32;
						}
						MC(0,ADDR_DATABUFF+i,1);
						MR(ADDR_DATABUFF,(u32)p,i);						
					}
					p8=(u8 *)ADDR_DATABUFF;
					DisplayString(MenuCount->v1,EditCol,0,p8);
				break;
				case 6:
					DisplayString(0,6,0,"��������");
					p=Get_Element((u8*)ADDR_4500_2,8,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					MC(0,ADDR_DATABUFF,LENmax_4500_2);
					if(p)
					{
						i=p[1];//�û�������
						p+=2;
						if(i>32)
						{//�ݹ̶��û�������32�ֽ�
							i=32;
						}
						MC(0,ADDR_DATABUFF+i,1);
						MR(ADDR_DATABUFF,(u32)p,i);						
					}
					p8=(u8 *)ADDR_DATABUFF;
					DisplayString(MenuCount->v1,EditCol,0,p8);
					break;
			}			
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10Ms��ʱ��
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
					MC(0,ADDR_DATABUFF,LENmax_4500_2);
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
					switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
					{
						case 5:
							Update_ElementDATA_unfixed(ADDR_4500_2,7,0,LENmax_4500_2,ADDR_DATABUFF,i);
							break;
						case 6:
							Update_ElementDATA_unfixed(ADDR_4500_2,8,0,LENmax_4500_2,ADDR_DATABUFF,i);
							break;
					}
					//���óɹ���ʾOK
					DisplayClrRow(1,9);
					DisplayString(1,2,0,p8);
					DispalyAlign(1,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					DispalyAlign(2,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					DisplayString(4,7,0,"����OK");
					DisplayString(5,4,0,"��������˳�");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
					p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������� 0=��1=��
					p8d[0]=1;//�˵���ʾ���������� 0=��1=��
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
				p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������� 0=��1=��
				p8d[0]=1;//�˵���ʾ���������� 0=��1=��
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_2_1
// Descriptions:        ͨ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_1(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_1);//��ʾ�˵�
			//����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
			p8=Get_Element((u8*)ADDR_4510_2,1,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+1,10,0,(u8 *)WorkModeList[p8[1]&0x03]);
			}
		  //���ӷ�ʽ:TCP��0����UDP��1��
			p8=Get_Element((u8*)ADDR_4510_2,2,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+2,10,0,(u8 *)ConnectList[p8[1]&0x03]);
			}
		  //����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
			p8=Get_Element((u8*)ADDR_4510_2,3,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				DisplayString(Ln+3,10,0,(u8 *)ApplicationList[p8[1]&0x03]);
			}
		  //��ʱʱ�估�ط�������bit7~bit2����ʱʱ�䣨�룩bit1~bit0���ط�����
			p8=Get_Element((u8*)ADDR_4510_2,7,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				//��ʱʱ�䣨�룩
				i=(p8[1]>>2);
				p=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
				bcd_p8(p,i,3);
				DisplayString(Ln+4,13,0,(u8 *)UnitList[9]);//0 s
				//�ط�����
				i=(p8[1]&0x03);
				p=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
				bcd_p8(p,i,1);
				DisplayString(Ln+5,11,0,(u8 *)UnitList[10]);//1 ��
			}
			//��������(��)  long-unsigned
			p8=Get_Element((u8*)ADDR_4510_2,8,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
        		i=p8[1];
				i<<=8;
				i|=p8[2];
				p=(u8*)ADDR_STRINGBUFF+(Ln+6)*84+4+10*2;
				bcd_p8(p,i,5);
			  DisplayString(Ln+6,15,0,(u8 *)UnitList[9]);//0 s				
			}
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
					if(Stack->MenuStack[MenuCount->Count].EditRow!=7)
					{
						DisplayString(Ln+7,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1://����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
							MenuCount->v1=10;
							MenuCount->v2=15;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2://���ӷ�ʽ:TCP��0����UDP��1��
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3://����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
							MenuCount->v1=10;
							MenuCount->v2=15;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4://��ʱʱ��
							MenuCount->v1=10;
							MenuCount->v2=12;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5://�ط�����
							MenuCount->v1=10;
							MenuCount->v2=10;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6://��������(��)  long-unsigned
							MenuCount->v1=10;
							MenuCount->v2=14;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
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
							case 1://����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x46://��
										i=1;
										break;
									case 0xF8://��
										i=2;
										break;
									case 0x7A://��
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)WorkModeList[i]);
								break;
							case 2://���ӷ�ʽ:TCP��0����UDP��1��
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectList[i]);
								break;
							case 3://����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x30://��
										i=1;
										break;
									case 0x82://��
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ApplicationList[i]);
								break;
							case 4://��ʱʱ��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x<63)
								{
									p8_add1(p8,3);
								}
								else
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 5://�ط�����
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x<3)
								{
									p8_add1(p8,1);
								}
								else
								{
									bcd_p8(p8,0,1);
								}
								break;
							case 6://��������(��)  long-unsigned
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
							case 1://����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x46://��
										i=2;
										break;
									case 0xF8://��
										i=0;
										break;
									case 0x7A://��
										i=1;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)WorkModeList[i]);
								break;
							case 2://���ӷ�ʽ:TCP��0����UDP��1��
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ConnectList[i]);
								break;
							case 3://����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								switch(p8[0])
								{
									case 0x30://��
										i=1;
										break;
									case 0x82://��
										i=0;
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,10,0,(u8 *)ApplicationList[i]);
								break;
							case 4://��ʱʱ��
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>0)
								{
									p8_sub1(p8,3);
								}
								else
								{
									bcd_p8(p8,63,3);
								}
								break;
							case 5://�ط�����
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x>0)
								{
									p8_sub1(p8,1);
								}
								else
								{
									bcd_p8(p8,3,1);
								}
								break;
							case 6://��������(��)  long-unsigned
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
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 4://��ʱʱ��
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 5://�ط�����
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 6://��������(��)  long-unsigned
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 7://ȷ������
								DisplayClrRow_4(Ln+Stack->MenuStack[MenuCount->Count].EditRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,9,0,"OK");
								//�������õ�����
								//1.����ģʽ:���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��
								x=Get_ElementADDR(ADDR_4510_2,1,0,LENmax_4510_2);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+10*2);
									switch(p8[0])
									{
										case 0x46://��
											i=0;
											break;
										case 0xF8://��
											i=0x01;
											break;
										case 0x7A://��
											i=0x02;
											break;
										default:
											break;
									}
									MWR(i,x+1,1);
									
								}
								//2.���ӷ�ʽ:TCP��0����UDP��1��
								x=Get_ElementADDR(ADDR_4510_2,2,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+10*2;
									switch(p8[0])
									{
										case 0x54://TCP
											i=0;
											break;
										case 0x55://UDP
											i=0x01;
											break;
									}
									MWR(i,x+1,1);
								}
								//3.����Ӧ�÷�ʽ:����ģʽ��0����������ģʽ��1��
								x=Get_ElementADDR(ADDR_4510_2,3,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)								
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+10*2;
									switch(p8[0])
									{
										case 0x30://��
											i=0;
											break;
										case 0x82://��
											i=0x01;
											break;
									}
									MWR(i,x+1,1);
								}
								//4.��ʱʱ�估�ط�������bit7~bit2����ʱʱ�䣨�룩bit1~bit0���ط�����
								x=Get_ElementADDR(ADDR_4510_2,7,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)
								{
									//��ʱʱ�䣨�룩
									p8=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+10*2;
									i=p8_bcd(p8,3);
									i<<=2;
									//�ط�����
									p8=(u8*)ADDR_STRINGBUFF+(Ln+5)*84+4+10*2;
									k=p8_bcd(p8,1);
									i|=k;
									MWR(i,x+1,1);
								}
								//6.��������(��)  long-unsigned
								x=Get_ElementADDR(ADDR_4510_2,8,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
								if(x)
								{
									p8=(u8*)ADDR_STRINGBUFF+(Ln+6)*84+4+10*2;
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);
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
							case 4://��ʱʱ��
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,3);
								if(x>63)
								{
									bcd_p8(p8,0,3);
								}
								break;
							case 5://�ط�����
								p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
								x=p8_bcd(p8,1);
								if(x>3)
								{
									bcd_p8(p8,0,1);
								}
								break;
							case 6://6.��������(��)
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
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_2_2
// Descriptions:        �����˿�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;	
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_2);//��ʾ�˵�
			p8=Get_Element((u8*)ADDR_4510_2,4,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{
				if(p8[1])
				{//���������Ϊ0����ʾ��1�������˿�
					i=p8[3];
					i<<=8;
					i|=p8[4];
					p=(u8*)ADDR_STRINGBUFF+(Ln+1)*84+4+9*2;
					bcd_p8(p,i,5);					
				}
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
						DisplayString(Ln+2,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{					
						case 1://�����˿�
							MenuCount->v1=9;
							MenuCount->v2=13;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 2://ȷ������
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
							case 1://�����˿�  long-unsigned
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
							case 1://�����˿�  long-unsigned
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 2:
								DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+2,9,0,"OK");
								//�������õ�����
							  //�����˿�
								x=Get_ElementADDR(ADDR_4510_2,4,1,LENmax_4510_2);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=((i>>8)&0xff);
									MWR(k,x+1,2);
								}	
								else//ԭ���ǿյ� 
								{
									MR(ADDR_DATABUFF,ADDR_4510_2,LENmax_4510_2);
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+9*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=((i>>8)&0xff);
									k<<=8;
									k|=0x12;//��Ա������0x12
									k<<=8;
									k|=1;//ARRAY�ĸ�����1
									Update_ElementDATA_unfixed(ADDR_DATABUFF,5,0,LENmax_4500_2,(u32)&k,4);
									MW(ADDR_DATABUFF,ADDR_4510_2,LENmax_4510_2);
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
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
// Function name:       Menu2_2_1_2_3
// Descriptions:        ��վͨ�Ų�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_3(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 Ln;
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1_3);//��ʾ�˵�
			//IP
			p8=Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+(Ln+1)*84+4+5*2;
				for(i=0;i<=3;i++)
				{
					bcd_p8(p+i*8,p8[i+2],3);
				}
				for(i=0;i<=2;i++)
				{
					p[3*2+i*8]='.';
					p[3*2+i*8+1]=0x20;
				}
			}
			//�˿�
			p8=Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{//�˿�
				p=(u8*)ADDR_STRINGBUFF+(Ln+2)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			//��IP
			p8=Get_Element((u8*)ADDR_4510_3,2,1,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{//IP
				p=(u8*)ADDR_STRINGBUFF+(Ln+3)*84+4+5*2;
				for(i=0;i<=3;i++)
				{
					bcd_p8(p+i*8,p8[i+2],3);
				}
				for(i=0;i<=2;i++)
				{
					p[3*2+i*8]='.';
					p[3*2+i*8+1]=0x20;
				}
			}
			//���˿�
			p8=Get_Element((u8*)ADDR_4510_3,2,2,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(p8)
			{//�˿�
				p=(u8*)ADDR_STRINGBUFF+(Ln+4)*84+4+7*2;
				i=p8[1];
				i<<=8;
				i|=p8[2];
				bcd_p8(p,i,5);
			}
			MenuCount->v1=5;
			MenuCount->v2=21;
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
						DisplayString(Ln+5,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{	
						case 1://IP
							MenuCount->v1=5;
							MenuCount->v2=21;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							break;
						case 2://�˿�
							MenuCount->v1=7;
							MenuCount->v2=11;
							ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3://IP
							MenuCount->v1=5;
							MenuCount->v2=21;
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
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
							break;
						case 4://�˿�
							MenuCount->v1=7;
							MenuCount->v2=11;
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
							case 1://IP ���Ҽ�����
							case 3:
								break;	
							case 2://�˿�
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://�˿�
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP ���������
							case 3:
								break;	
							case 2://�˿�
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
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
							case 4://�˿�
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
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
							case 1://IP
							case 3:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								break;
							case 2://�˿�
							case 4:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+5,9,0,"OK");
								//�������õ�����
								x=Get_ElementADDR(ADDR_4510_3,1,1,LENmax_4510_3);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
									MW(ADDR_DATABUFF,x+2,4);//дIP
								}	
								x=Get_ElementADDR(ADDR_4510_3,1,2,LENmax_4510_3);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+7*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);//д�˿�
								}	

								x=Get_ElementADDR(ADDR_4510_3,2,1,LENmax_4510_3);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
									for(i=0;i<=3;i++)
									{
										p8[i]=p8_bcd(p+8*i,3);
									}
									MW(ADDR_DATABUFF,x+2,4);//дIP
								}	
								x=Get_ElementADDR(ADDR_4510_3,2,2,LENmax_4510_3);//����Ԫ�ص�ַ(ʵ���߼���ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(x)
								{
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+7*2);
									i=p8_bcd(p8,5);
									k=(i&0xff);
									k<<=8;
									k|=(i>>8);
									MWR(k,x+1,2);//д�˿�
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
				else//if(Stack->MenuStack[MenuCount->Count].EditCol==0)
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+7*2);
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
// Function name:       Menu2_2_1_2_4
// Descriptions:        �ն�IP����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_4(void)
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

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_4);//��ʾ�˵�
			//��ʾ�ڴ�������
			//1.��ȡ����
			p8=Get_Element((u8*)ADDR_4510_4,1,0,LENmax_4510_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
		  if(p8)
			{
				i=p8[1];//IP����
				DisplayString(Ln+1,5,0,(u8 *)IpTypeList[(i&0x03)]);
			}	
			if(i==1)
			{//1=��̬
				MR(ADDR_DATABUFF,ADDR_4510_4+6,4);//���ն�IP
				MR(ADDR_DATABUFF+4,ADDR_4510_4+12,4);//����������
				MR(ADDR_DATABUFF+4+4,ADDR_4510_4+18,4);//������
			}
			else
			{//0=DHCP,2=PPPoE
				MC(0,ADDR_DATABUFF,12);//���ն�IP���������롢����
			}
			p8=(u8 *)ADDR_DATABUFF;
			//2.�ն�IP
			p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//3.��������
			p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[4+i],3);
			}
			for(i=0;i<=2;i++)
			{
				p[3*2+i*8]='.';
				p[3*2+i*8+1]=0x20;
			}
			//4.����
			p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
			for(i=0;i<=3;i++)
			{
				bcd_p8(p+i*8,p8[8+i],3);
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
						DisplayString(Ln+5,6,0,"ȷ������");
					}
					switch(Stack->MenuStack[MenuCount->Count].EditRow)
					{
						case 1:
							MenuCount->v1=5;
							MenuCount->v2=9;
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
									case 0x44://DHCP
										i=1;
										MR(ADDR_DATABUFF,ADDR_4510_4+6,4);//���ն�IP
										MR(ADDR_DATABUFF+4,ADDR_4510_4+12,4);//����������
										MR(ADDR_DATABUFF+4+4,ADDR_4510_4+18,4);//������
										break;
									case 0x06://��̬
										i=2;
										MC(0,ADDR_DATABUFF,12);//���ն�IP���������롢����
										break;
									case 0x50://PPPoE
										i=0;
										MC(0,ADDR_DATABUFF,12);//���ն�IP���������롢����
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)IpTypeList[i]);
								//�ն�IP
								p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[i],3);
								}
								for(i=0;i<=2;i++)
								{
									p[3*2+i*8]='.';
									p[3*2+i*8+1]=0x20;
								}
								//��������
								p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[4+i],3);
								}
								for(i=0;i<=2;i++)
								{
									p[3*2+i*8]='.';
									p[3*2+i*8+1]=0x20;
								}
								//����
								p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[8+i],3);
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
									case 0x44://DHCP
										i=2;
										MC(0,ADDR_DATABUFF,12);//���ն�IP���������롢����
										break;
									case 0x06://��̬
										i=0;
										MC(0,ADDR_DATABUFF,12);//���ն�IP���������롢����
										break;
									case 0x50://PPPoE
										i=1;
										MR(ADDR_DATABUFF,ADDR_4510_4+6,4);//���ն�IP
										MR(ADDR_DATABUFF+4,ADDR_4510_4+12,4);//����������
										MR(ADDR_DATABUFF+4+4,ADDR_4510_4+18,4);//������
										break;
								}
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,5,0,(u8 *)IpTypeList[i]);
								//�ն�IP
								p=(u8*)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[i],3);
								}
								for(i=0;i<=2;i++)
								{
									p[3*2+i*8]='.';
									p[3*2+i*8+1]=0x20;
								}
								//��������
								p=(u8*)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[4+i],3);
								}
								for(i=0;i<=2;i++)
								{
									p[3*2+i*8]='.';
									p[3*2+i*8+1]=0x20;
								}
								//����
								p=(u8*)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									bcd_p8(p+i*8,p8[8+i],3);
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
//								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(MenuCount->v1)*2);
//								//�ж��ڴ������д� ���ʼ��1=��̬
//								if(p8[0]==0x24)
//								{//����IP����
//									DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v1,0,(u8 *)IpTypeList[1]);
//								}
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x06)
								{//�̶�IP�������� �Զ�����
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x06)
								{//�̶�IP�������� �Զ�����
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p8[0]==0x06)
								{//�̶�IP�������� �Զ�����
									Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								}
								break;
							case 5:
								DisplayClrRow_4(Ln+5);//����Ļ��ʾ��ָ����(4�������ֲ���)
								DisplayString(Ln+5,9,0,"OK");
								//�������õ�����
							  //1.IP����
								p8=(u8 *)ADDR_DATABUFF;
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								if(p[0]==0x44)
								{//DHCP
									p8[0]=0x00;
								}
								if(p[0]==0x06)
								{//��̬
									p8[0]=0x01;
								}
								if(p[0]==0x50)
								{//PPPoE
									p8[0]=0x02;
								}
								MW(ADDR_DATABUFF,ADDR_4510_4+3,1);//дIP����
								//2.IP
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[1+i]=p8_bcd(p+8*i,3);
								}
								//3.��������
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+3)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[5+i]=p8_bcd(p+8*i,3);
								}
								//4.����
								p=(u8 *)(ADDR_STRINGBUFF+(Ln+4)*84+4+5*2);
								for(i=0;i<=3;i++)
								{
									p8[9+i]=p8_bcd(p+8*i,3);
								}
								if(p8[0]==1)
								{//��̬
									MW(ADDR_DATABUFF+1,ADDR_4510_4+6,4);//д�ն�IP
									MW(ADDR_DATABUFF+1+4,ADDR_4510_4+12,4);//д��������
									MW(ADDR_DATABUFF+1+4+4,ADDR_4510_4+18,4);//д����									
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
// Function name:       Menu2_2_1_2_5
// Descriptions:         PPPoE�û�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_5(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	//u32 x;
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
		case 0://����0����ʼ��
			Stack->MenuStack[MenuCount->Count].Task=2;
			p8=" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`!@#$%^&*()_+-={}[]|:;'\",.?/\\<>";
			DisplayClr();//��ȫ��
			DisplayString(MinLn,MinCol,0,p8);
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow+Stack->MenuStack[MenuCount->Count-1].MoveRow)
			{
				case 5:
					DisplayString(0,3,0,"PPPoE�û�������");
					p=Get_Element((u8*)ADDR_4510_4,5,0,LENmax_4510_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					MC(0,ADDR_DATABUFF,LENmax_4510_4);
					if(p)
					{
						i=p[1];//�û�������
						p+=2;
						if(i>32)
						{//�ݹ̶��û�������32�ֽ�
							i=32;
						}
						MC(0,ADDR_DATABUFF+32,1);
						MR(ADDR_DATABUFF,(u32)p,i);						
					}
					p8=(u8 *)ADDR_DATABUFF;
					DisplayString(MenuCount->v1,EditCol,0,p8);
				break;
				case 6:
					DisplayString(0,4,0,"PPPoE��������");
					p=Get_Element((u8*)ADDR_4510_4,6,0,LENmax_4510_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					MC(0,ADDR_DATABUFF,LENmax_4510_4);
					if(p)
					{
						i=p[1];//�û�������
						p+=2;
						if(i>32)
						{//�ݹ̶��û�������32�ֽ�
							i=32;
						}
						MC(0,ADDR_DATABUFF+32,1);
						MR(ADDR_DATABUFF,(u32)p,i);						
					}
					p8=(u8 *)ADDR_DATABUFF;
					DisplayString(MenuCount->v1,EditCol,0,p8);
					break;
			}			
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10Ms��ʱ��
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
					MC(0,ADDR_DATABUFF,LENmax_4510_4);
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
						case 5:
							Update_ElementDATA_unfixed(ADDR_4510_4,5,0,LENmax_4510_4,ADDR_DATABUFF,i);
							break;
						case 6:
							Update_ElementDATA_unfixed(ADDR_4510_4,6,0,LENmax_4510_4,ADDR_DATABUFF,i);
							break;
					}
					//���óɹ���ʾOK
					DisplayClrRow(1,9);
					DisplayString(1,2,0,p8);
					DispalyAlign(1,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					DispalyAlign(2,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					DisplayString(4,7,0,"����OK");
					DisplayString(5,4,0,"��������˳�");
					Stack->MenuStack[MenuCount->Count].Task=3;
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
					p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������� 0=��1=��
					p8d[0]=1;//�˵���ʾ���������� 0=��1=��
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
				p8d=(u8 *)ADDR_MenuICO;//�˵���ʾ���������� 0=��1=��
				p8d[0]=1;//�˵���ʾ���������� 0=��1=��
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

//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_1_2_6_1
// Descriptions:        MAC��ַ����(ʮ����)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu2_2_1_2_6_1(void)
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

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_6_1);//��ʾ�˵�
			//��ʾ�ڴ�������
			//MAC			
			p8=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+5*2);
			MR(ADDR_DATABUFF,ADDR_4510_5+2,6);//��MAC
			Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
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
						DisplayString(Ln+3,6,0,"ȷ������");
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
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
								DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
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
								MR(ADDR_DATABUFF,ADDR_4510_5+2,6);//��MAC
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
									Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
									MW(ADDR_DATABUFF,ADDR_4510_5+2,6);//дMAC
									DisplayString(Row,9,0,"OK");
								}
								else
								{
									if(IC_24AAxxx)
									{//IC_24AAxxxΪ0��ʾ�����ã�1��ʾ����ΨһMAC���ʲ�������
										DisplayString(Row,2,0,"MACΨһ��������");
									}
									else
									{//(p8[0]&0x01)==1 �鲥��ַ
										DisplayString(Row,1,0,"���ֽ�Ϊ����������");
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
// Function name:       Menu2_2_1_2_6_2
// Descriptions:        MAC��ַ���ã�ʮ�����ƣ�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void Menu2_2_1_2_6_2(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 Row=1;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif
	Row+=Ln;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_6_2);//��ʾ�˵�
			//��ʾ�ڴ�������
			//MAC			
			p8=(u8 *)ADDR_DATABUFF;
			p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+3*2);
			MR(ADDR_DATABUFF,ADDR_4510_5+2,6);//��MAC
			Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
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
						DisplayString(Ln+2,6,0,"ȷ������");
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
								MR(ADDR_DATABUFF,ADDR_4510_5+2,6);//��MAC
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
									Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
									MW(ADDR_DATABUFF,ADDR_4510_5+2,6);//дMAC
									DisplayString(Row,9,0,"OK");
								}
								else
								{
									if(IC_24AAxxx)
									{//IC_24AAxxxΪ0��ʾ�����ã�1��ʾ����ΨһMAC���ʲ�������
										DisplayString(Row,6,0,"MAC Ψһ");
									}
									else
									{//(p8[0]&0x01)==1 �鲥��ַ
										DisplayString(Row,4,0,"���ֽ�Ϊ����");
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
// Function name:       Menu2_2_1_2_6
// Descriptions:         MAC��ַ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2_6(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2_6);//��ʾ�˵�
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
						MenuCreate((u32)Menu2_2_1_2_6_1);//�����¼��˵�   MAC��ַ����(ʮ����)         
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_2_6_2);//�����¼��˵�   MAC��ַ����(ʮ������)
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
// Function name:       Menu2_2_1_1
// Descriptions:        ���߹�������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_1(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_1);//��ʾ�˵�
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
						MenuCreate((u32)Menu2_2_1_1_1);//ͨ��ģʽ����           
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_1_2);//�����˿�����           
						break;
					case 3:
						MenuCreate((u32)Menu2_2_1_1_3);//��վͨ�Ų�������           
						break;
					case 4:
						MenuCreate((u32)Menu2_2_1_1_4);//APN����           
						break;
					case 5:
					case 6:
						MenuCount->v1=1;
						MenuCreate((u32)Menu2_2_1_1_5);//5��6�û������� ����������
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
// Function name:       Menu2_2_6
// Descriptions:        ��̫������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1_2(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_1_2);//��ʾ�˵�
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
						MenuCreate((u32)Menu2_2_1_2_1);//1.ͨ������           
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_2_2);//2.�����˿�����           
						break;
					case 3:
						MenuCreate((u32)Menu2_2_1_2_3);//3.��վͨ�Ų�������           
						break;
					case 4:
						MenuCreate((u32)Menu2_2_1_2_4);//4.�ն�IP����           
						break;
					case 5:
					case 6:
						MenuCount->v1=1;
						MenuCreate((u32)Menu2_2_1_2_5);//5��6 PPPoE�û�������������           
						break;
					case 7:
						MenuCreate((u32)Menu2_2_1_2_6);//7. MAC��ַ����           
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
// Function name:       Menu2_2_1
// Descriptions:        ͨ��ͨ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_1(void)//�˵�4_3
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																		"ͨ��ͨ������\n\r"
																		"  1.���߹�������\n\r"
																		"  2.��̫������\n\r"
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
						MenuCreate((u32)Menu2_2_1_1);
						break;
					case 2:
						MenuCreate((u32)Menu2_2_1_2);
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
// Function name:       Menu2_2_2
// Descriptions:        ���ܱ��������  ���ع���: ������=����Ӧ �˿�=δ֪ ��Լ=δ֪ ʱ����ɾ���ò�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_2(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i=0;
	u32 j=0;
	u32 k;
	u64 x;
	u64 y;
	u32 port_oad;
	u32 delete_pn=0;
#if (LENper_6000+1024)>LEN_DATABUFF
 	#error
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_2);//��ʾ�˵�
			//��ʾ�ڴ�������
			MenuCount->v1=8;
			MenuCount->v2=11;
			if(MenuCount->v3>=NUMmax_6000)
			{//MenuCount->v3��¼������ŵ�ȫ�ֱ���
				MenuCount->v3=NUMmax_6000-1;
			}
			if(MenuCount->v3<1)
			{//MenuCount->v3��¼������ŵ�ȫ�ֱ���
				MenuCount->v3=1;
			}
			p=(u8*)(ADDR_STRINGBUFF+1*84+4+(MenuCount->v1)*2);
			bcd_p8(p,MenuCount->v3,4);
			//�ȰѲ�������ADDR_DATABUFF+1024��������Ч��������Ϣ������default_6000���Ĭ�ϲ�����
			//Ȼ���޸ĵ����ݶ��ȸ��µ�ADDR_DATABUFF+1024������ϵĲ������������ˣ���һ��д��ADDR_6000
			//��������Ŀ���ǣ������������١���ֹԭ�������ݱ��޸��ˡ�
			//�ȶ�������ADDR_DATABUFF+1024��Ȼ���ٸ��ݽ����������ADDR_DATABUFF+1024���ݣ��ٱ��棬�Ͳ����޸Ľ����޷��޸ĵ�����
			//�������
			p8=(u8*)ADDR_DATABUFF+1024;
			MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
			if(p8[0]==0)//������������ڸ�������ţ���Ӧ�����¼ӵģ�����Ĭ������
			{
				MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
			}
			else//���ڣ�����ѡ�����ź����ò��������������Ƿ�һ��
			{
				p8=Get_Element((u8*)(ADDR_DATABUFF+1024),1,0,LENper_6000);
				k=p8[2]+p8[1]*0x100;
				if(k!=MenuCount->v3)
				{
					MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
				}
			}
			//ͨ������
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,2,LENper_6000);
			p=(u8*)(ADDR_STRINGBUFF+2*84+4+8*2);
			if(p8[1]>10)
				DisplayString(2,8,0,(u8 *)CommunicationSpeedList[11]);//��ʾͨ������
			else
				DisplayString(2,8,0,(u8 *)CommunicationSpeedList[p8[1]]);//��ʾͨ������

			//�˿ںš�ͨ��Э������
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,4,LENper_6000);
			port_oad=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|(p8[4]);
			if(port_oad==0xf2090201)
				DisplayString(3,8,0,(u8 *)PortList[4]);//��ʾ�˿�--�ز�
			else if(port_oad==0xf2080201)
				DisplayString(3,8,0,(u8 *)PortList[1]);//��ʾ�˿�--����
			else if(port_oad==0xf2010201)
				DisplayString(3,8,0,(u8 *)PortList[2]);//��ʾ�˿�--4851
			else if(port_oad==0xf2010202)
				DisplayString(3,8,0,(u8 *)PortList[3]);//��ʾ�˿�--4582
			else
				DisplayString(3,8,0,(u8 *)PortList[0]);//��ʾ�˿�--δ֪
			//ͨ��Э������	
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,3,LENper_6000);
			if(p8[1]>4)
			{
				DisplayString(4,8,0,(u8 *)ProtocolList[0]);//ͨ��Э���б�
			}
			else
			{
				DisplayString(4,8,0,(u8 *)ProtocolList[p8[1]]);//ͨ��Э���б�
			}
		
			//ͨ�ŵ�ַ
			p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+4*2);
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,1,LENper_6000);
			MR(ADDR_DATABUFF,(u32)p8+3,(p8[1]-1));
			Data_Inverse(ADDR_DATABUFF,(p8[1]-1));//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			if((p8[1]-1)>6)
				y=MRR(ADDR_DATABUFF,6);
			else
				y=MRR(ADDR_DATABUFF,(p8[1]-1));
			DisplayData(y,12,0,p16);
			
			//������
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,6,LENper_6000);
			p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+6*2);
			DisplayData(p8[1],2,0,p16);
			
			//���߷�ʽ
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),2,8,LENper_6000);
			if(p8[1]>4)
			{
				DisplayString(7,8,0,(u8 *)ConnectType[0]);//ͨ��Э���б�
			}
			else
			{
				DisplayString(7,8,0,(u8 *)ConnectType[p8[1]]);//ͨ��Э���б�
			}


			//�ɼ�����ַ
			p16=(u16 *)(ADDR_STRINGBUFF+8*84+4+6*2);
			p8=Get_Element((u8*)(ADDR_DATABUFF+1024),3,1,LENper_6000);
			MR(ADDR_DATABUFF,(u32)p8+3,(p8[1]-1));
			Data_Inverse(ADDR_DATABUFF,(p8[1]-1));//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			if((p8[1]-1)>6)
				y=MRR(ADDR_DATABUFF,6);
			else
				y=MRR(ADDR_DATABUFF,(p8[1]-1));
			DisplayData(y,12,0,p16);

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
							DisplayString(9,7,0,"ȷ������");
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
							DisplayString(0,3,0,"���ܱ��������");
#else
							DisplayString(0,5,0,"���ܱ�����");
#endif

						}
					}
					
					if((Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)!=10)
					{
						DisplayClrRow_4(10-Stack->MenuStack[MenuCount->Count].MoveRow);//����Ļ��ʾ��ָ����(4�������ֲ���)
						DisplayString(10-Stack->MenuStack[MenuCount->Count].MoveRow,7,0,"ȷ������");
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
							MenuCount->v2=14;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 3:
							MenuCount->v1=8;
							MenuCount->v2=12;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 4:
							MenuCount->v1=8;
							MenuCount->v2=15;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 5:
							MenuCount->v1=4;
							MenuCount->v2=15;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 6:
							MenuCount->v1=6;
							MenuCount->v2=8;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 7:
							MenuCount->v1=8;
							MenuCount->v2=15;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 8:
							MenuCount->v1=6;
							MenuCount->v2=17;
							ArrowLR(Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
							NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
							break;
						case 9:
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
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x<(NUMmax_6000-2))
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
								x=p8_bcd(p8,2);
								if(p8[0]==0xa6&&p8[1]==0x63)
								{//����Ӧ
									i=0;
								}
								else
								{
									switch(x)
									{
										case 30:
											i=1;
											break;
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
											i=8;
											break;
										case 38:
											i=9;
											break;
										case 57:
											i=10;
											break;
										case 11:
											i=11;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//δ֪
									i=1;
								}
								else if(p8[0]==0x5c&&p8[1]==0x50)
								{//����
									i=2;
								}
								else if(p8[0]==0x7a&&p8[1]==0x61)
								{//�ز�
									i=0;
								}
								else
								{
									switch(x)
									{
									case 48:
										x=p8_bcd(p8+8,1);
										if(x==1)
											i=3;
										if(x==2)
											i=4;
										break;
									default:
										break;
								}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)PortList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//δ֪
									i=1;
								}
								else
								{
									switch(x)
									{
									case 64:
										x=p8_bcd(p8+8,2);
										if(x==19)
											i=2;
										if(x==20)
											i=3;
										break;
									case 69:
										i=4;
										break;
									case 18:
										i=0;
										break;
									default:
										break;
									}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
								p8_add1(p8,12);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+6*2);
								p8_add1(p8,2);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0xca&&p8[1]==0x5c)//δ֪
								{
									i=1;
								}
								else if(p8[0]==0x50&&p8[1]==0x4a)//����
								{
									i=2;
								}
								else if(p8[8]==0xf4&&p8[9]==0x58)//��������
								{
									i=3;
								}
								else//��������
								{
									i=0;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ConnectType[i]);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+6*2);
								p8_add1(p8,12);
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
								p8=(u8 *)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>1)
								{
									p8_sub1(p8,4);
								}
								else
								{
									bcd_p8(p8,(NUMmax_6000-1),4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 2:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xa6&&p8[1]==0x63)
								{//����Ӧ
									i=10;
								}
								else
								{
									switch(x)
									{
										case 30:
											i=11;
											break;
										case 60:
											i=0;
											break;
										case 12:
											i=1;
											break;
										case 24:
											i=2;
											break;
										case 48:
											i=3;
											break;
										case 72:
											i=4;
											break;
										case 96:
											i=5;
											break;
										case 19:
											i=6;
											break;
										case 38:
											i=7;
											break;
										case 57:
											i=8;
											break;
										case 11:
											i=9;
											break;
										default:
											break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)CommunicationSpeedList[i]);
								break;
							case 3:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//δ֪
									i=4;
								}
								else if(p8[0]==0x5c&&p8[1]==0x50)
								{//����
									i=0;
								}
								else if(p8[0]==0x7a&&p8[1]==0x61)
								{//�ز�
									i=3;
								}
								else
								{
									switch(x)
									{
									case 48:
										x=p8_bcd(p8+8,1);
										if(x==1)
											i=1;
										if(x==2)
											i=2;
										break;
									default:
										break;
									}
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)PortList[i]);
								break;
							case 4:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								x=p8_bcd(p8,2);
								if(p8[0]==0xca&&p8[1]==0x5c)
								{//δ֪
									i=4;
								}
								else
								{
									switch(x)
									{
									case 64:
										x=p8_bcd(p8+8,2);
										if(x==19)
											i=0;
										if(x==20)
											i=1;
										break;
									case 69:
										i=2;
										break;
									case 18:
										i=3;
										break;
									default:
										break;
									}
									
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ProtocolList[i]);
								break;
							case 5:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
								p8_sub1(p8,12);
								break;
							case 6:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+6*2);
								p8_sub1(p8,2);
								break;
							case 7:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
								if(p8[0]==0xca&&p8[1]==0x5c)//δ֪
								{
									i=3;
								}
								else if(p8[0]==0x50&&p8[1]==0x4a)//����
								{
									i=0;
								}
								else if(p8[8]==0xf4&&p8[9]==0x58)//��������
								{
									i=1;
								}
								else//��������
								{
									i=2;
								}
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,8,0,(u8 *)ConnectType[i]);
								break;
							case 8:
								p8=(u8 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+6*2);
								p8_sub1(p8,12);
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
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 5:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 6:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
								break;
							case 8:
								Stack->MenuStack[MenuCount->Count].EditCol=MenuCount->v1;
								DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,18,0,"��");
								break;
							case 9:
								DisplayClrRow_4(9);//����Ļ��ʾ��ָ����(4�������ֲ���)
								p=(u8*)(ADDR_STRINGBUFF+(1-Stack->MenuStack[MenuCount->Count].MoveRow)*84+4+8*2);
								p8=(u8*)(ADDR_STRINGBUFF+1*84+4+8*2);
								x=p8_bcd(p8,4);
								if(x>NUMmax_6000)
								{//�����㳬�����ֵ �򲻿�����
									DisplayString(Stack->MenuStack[MenuCount->Count].EditRow,7,0,"��Ч������");
								}
								else
								{
									DisplayClrRow_4(9);//����Ļ��ʾ��ָ����(4�������ֲ���)
									DisplayString(9,9,0,"OK");

									p8=(u8*)ADDR_DATABUFF+1024;
									MR(ADDR_DATABUFF+1024,ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
									if(p8[0]==0)//������������ڸ�������ţ���Ӧ�����¼ӵģ�����Ĭ������
									{
										MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
									}
									else//���ڣ�����ѡ�����ź����ò��������������Ƿ�һ��
									{
										p8=Get_Element((u8*)(ADDR_DATABUFF+1024),1,0,LENper_6000);
										k=p8[2]+p8[1]*0x100;
										if(k!=MenuCount->v3)
										{
											MR(ADDR_DATABUFF+1024,(u32)default_6000,sizeof(default_6000));
										}
									}
									
									MWR(MenuCount->v3,ADDR_DATABUFF,2);
									Data_Inverse(ADDR_DATABUFF,2);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),1,0,LENper_6000,ADDR_DATABUFF,2);
									
									p8=(u8 *)(ADDR_STRINGBUFF+2*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xa6&&p8[1]==0x63)
									{//����Ӧ
										i=255;
										delete_pn+=1;
									}
									else
									{
										switch(x)
										{
											case 30:
												i=0;
												break;
											case 60:
												i=1;
												break;
											case 12:
												i=2;
												break;
											case 24:
												i=3;
												break;
											case 48:
												i=4;
												break;
											case 72:
												i=5;
												break;
											case 96:
												i=6;
												break;
											case 19:
												i=7;
												break;
											case 38:
												i=8;
												break;
											case 57:
												i=9;
												break;
											case 11:
												i=10;
												break;
											default:
												break;
										}
									}
									MWR(i,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,2,LENper_6000,ADDR_DATABUFF,1);
									
									p8=(u8 *)(ADDR_STRINGBUFF+3*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xca&&p8[1]==0x5c)
									{//δ֪
										i=0;
										delete_pn+=1;
									}
									else if(p8[0]==0x5c&&p8[1]==0x50)
									{//����
										i=0xf2080201;
									}
									else if(p8[0]==0x7a&&p8[1]==0x61)
									{//�ز�
										i=0xf2090201;
									}
									else
									{
										switch(x)
										{
										case 48:
											x=p8_bcd(p8+8,1);
											if(x==1)
												i=0xf2010201;
											if(x==2)
												i=0xf2010202;
											break;
										default:
											break;
										}
									}
								
									MWR(i,ADDR_DATABUFF,4);
									Data_Inverse(ADDR_DATABUFF,4);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,4,LENper_6000,ADDR_DATABUFF,4);
									
									p8=(u8 *)(ADDR_STRINGBUFF+4*84+4+8*2);
									x=p8_bcd(p8,2);
									if(p8[0]==0xca&&p8[1]==0x5c)
									{//δ֪
										i=0;
										delete_pn+=1;
									}
									else
									{
										switch(x)
										{
										case 64:
											x=p8_bcd(p8+8,2);
											if(x==19)
												i=1;
											if(x==20)
												i=2;
											break;
										case 69:
											i=3;
											break;
										case 18:
											i=4;
											break;
										default:
											break;
										}
										
									}
									MWR(i,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,3,LENper_6000,ADDR_DATABUFF,1);

									p8=(u8 *)(ADDR_STRINGBUFF+6*84+4+6*2);
									x=p8_bcd(p8,2);
									MWR(x,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,6,LENper_6000,ADDR_DATABUFF,1);
									
									//�������õ�����
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8 *)(ADDR_STRINGBUFF+5*84+4+4*2);
									p8[0]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;
									p8[1]=(p[2*2]-0x30)*16+(p[3*2]-0x30)*1;
									p8[2]=(p[4*2]-0x30)*16+(p[5*2]-0x30)*1;
									p8[3]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;
									p8[4]=(p[8*2]-0x30)*16+(p[9*2]-0x30)*1;
									p8[5]=(p[10*2]-0x30)*16+(p[11*2]-0x30)*1;
									Update_ElementDATA_unfixed((ADDR_DATABUFF+1024),2,1,LENper_6000,ADDR_DATABUFF,6);

									i=0;
									p8=(u8 *)(ADDR_STRINGBUFF+7*84+4+8*2);
									if(p8[0]==0xca&&p8[1]==0x5c)//δ֪
									{
										i=0;
									}
									else if(p8[0]==0x50&&p8[1]==0x4a)//����
									{
										i=1;
									}
									else if(p8[8]==0xf4&&p8[9]==0x58)//��������
									{
										i=2;
									}
									else if(p8[8]==0xb6&&p8[9]==0x5a)//��������
									{
										i=3;
									}
									MWR(i,ADDR_DATABUFF,1);
									Update_ElementDATA_fixed((ADDR_DATABUFF+1024),2,8,LENper_6000,ADDR_DATABUFF,1);
																		

									//�������õ�����
									p8=(u8 *)ADDR_DATABUFF;
									p=(u8 *)(ADDR_STRINGBUFF+8*84+4+6*2);
									p8[0]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;
									p8[1]=(p[2*2]-0x30)*16+(p[3*2]-0x30)*1;
									p8[2]=(p[4*2]-0x30)*16+(p[5*2]-0x30)*1;
									p8[3]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;
									p8[4]=(p[8*2]-0x30)*16+(p[9*2]-0x30)*1;
									p8[5]=(p[10*2]-0x30)*16+(p[11*2]-0x30)*1;
									Update_ElementDATA_unfixed((ADDR_DATABUFF+1024),3,1,LENper_6000,ADDR_DATABUFF,6);
									
									if(delete_pn==3)//���ع�������
									{
										MC(0,(ADDR_DATABUFF+1024),LENper_6000);
										DisplayString(9,6,0,"ɾ���ɹ�");
									}
									MW((u32)(ADDR_DATABUFF+1024),ADDR_6000+LENper_6000*MenuCount->v3,LENper_6000);
									Terminal_Ram->SET6000=0x55;//�ɼ��������ñ������ñ�־:0=û����,0x55=����
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
								if(x>NUMmax_6000 || x<1)
								{
									bcd_p8(p8,1,4);
								}
								MenuCount->v3=p8_bcd(p8,4);
								Stack->MenuStack[MenuCount->Count].Task=0;
								break;
							case 5:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+5*84+4+(MenuCount->v1)*2);
								//ͨ�ŵ�ַBCDռ6���ֽڣ��ж��ڴ���������д����ʼ��
								for(i=0;i<=11;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=12)
								{
									bcd_p8(p8,1,12);
								}
								break;
							case 6:
								p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
								p16[0]=0x2020;
								p16[1]=0x2020;
								p16[2]=32;//'���Ҽ�ͷ'
								p16[3]=33;
								p8=(u8 *)(ADDR_STRINGBUFF+6*84+4+(MenuCount->v1)*2);
								//ͨ�ŵ�ַBCDռ6���ֽڣ��ж��ڴ���������д����ʼ��
								for(i=0;i<=1;i++)
								{
									if(p8[i*2]>0x39 || p8[i*2]<0x30)
									{
										break;
									}
								}
								if(i!=2)
								{
									bcd_p8(p8,1,2);
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
	/*
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
			DisplayString(4-Stack->MenuStack[MenuCount->Count].MoveRow,10,0,"	   ");			
			break;
	}*/
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu2_2_2
// Descriptions:        ������ʱ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_3(void)
{
	u8 *p;
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u32 Ln;
	u32 x;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_3);//��ʾ�˵�
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=6;
			MenuCount->v2=13;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//����
			MR(ADDR_DATABUFF,ADDR_DL698YMDHMS,2);
			Data_Inverse(ADDR_DATABUFF,2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			x=MRR(ADDR_DATABUFF,2);//��
			x=hex_bcd(x);//HEX��תΪ8λѹ��BCD��
			x-=0x2000;
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
			p[0*2]=((x>>4)&0xf)+0x30;
			p[1*2]=(x&0xf)+0x30;
			p[2*2]='-';
			x=MRR(ADDR_DL698YMDHMS+2,1);//��
			x=hex_bcd(x);//HEX��תΪ8λѹ��BCD��
			p[3*2]=(x>>4)+0x30;
			p[4*2]=(x&0xf)+0x30;
			p[5*2]='-';
			x=MRR(ADDR_DL698YMDHMS+3,1);//��
			x=hex_bcd(x);//HEX��תΪ8λѹ��BCD��
			p[6*2]=(x>>4)+0x30;
			p[7*2]=(x&0xf)+0x30;
			for(i=0;i<=7;i++)
			{
				p[i*2+1]=0x20;
			}
			//ʱ��
			p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
			x=MRR(ADDR_DL698YMDHMS+4,1);//ʱ
			x=hex_bcd(x);//HEX��תΪ8λѹ��BCD��
			p[0*2]=(x>>4)+0x30;
			p[1*2]=(x&0xf)+0x30;
			p[2*2]=':';
			x=MRR(ADDR_DL698YMDHMS+5,1);//��
			x=hex_bcd(x);//HEX��תΪ8λѹ��BCD��
			p[3*2]=(x>>4)+0x30;
			p[4*2]=(x&0xf)+0x30;
			p[5*2]=':';
			x=MRR(ADDR_DL698YMDHMS+6,1);//��
			x=hex_bcd(x);//HEX��תΪ8λѹ��BCD��
			p[6*2]=(x>>4)+0x30;
			p[7*2]=(x&0xf)+0x30;
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
						DisplayString(Ln+3,6,0,"ȷ������");
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
							DisplayString(Ln+Stack->MenuStack[MenuCount->Count].EditRow,16,0,"����");
						}
						else//if(Stack->MenuStack[MenuCount->Count].EditRow==3)
						{
							//�������õ�����
							DisplayClrRow_4(Ln+3);//����Ļ��ʾ��ָ����(4�������ֲ���)
							DisplayString(Ln+3,9,0,"OK");
							p8=(u8 *)(ADDR_RTCBUFF);
							p=(u8 *)(ADDR_STRINGBUFF+(Ln+2)*84+4+6*2);
							p8[0]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//��
							p8[1]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//��
							p8[2]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//ʱ
							p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+6*2);
							p8[4]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;//��
							p8[5]=(p[3*2]-0x30)*16+(p[4*2]-0x30)*1;//��
							p8[6]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;//��
//							WriteLPCRTC(ADDR_RTCBUFF);//д�ڲ�RTC
//							SyncLPCRTC();//ͬ���ڲ�RTC(���ڲ�д�ⲿ)
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
// Function name:       Menu2_2_5
// Descriptions:        ��������ַ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_5(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u32 i;
	u32 j;
	u32 k;
	u64 x;
	u32 Ln;
	UARTCtrl_TypeDef * UARTCtrl;
	
#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_5);//��ʾ�˵�
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
			MenuCount->v1=4;
			MenuCount->v2=15;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,MenuCount->v2,MenuCount->v1);
			//��ʾ�ڴ�������
			//ͨ�ŵ�ַ
				p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+(MenuCount->v1)*2);
				p8=(u8 *)ADDR_DATABUFF;
				MR(ADDR_DATABUFF,ADDR_4001+1,LENmax_4001-1);
				if(p8[0]>6)
				{
					p8[0]=6;
				}
				Data_Inverse(ADDR_DATABUFF+1,p8[0]);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				x=MRR(ADDR_DATABUFF+1,p8[0]);
				DisplayData(x,12,0,p16);
			break;
		case 1://����1:ѡ��
			if(((USER/100) != 9)||(Comm_Ram->Factory==0x55)){//�����ֳ������ܱ༭�ն˵�ַ
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
								MenuCount->v1=4;
								MenuCount->v2=15;
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
									p8_add1(p8,12);
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
									p8_sub1(p8,12);
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
									p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+4*2);
									//ͨ�ŵ�ַBCDռ6���ֽڣ��ж��ڴ���������д����ʼ��
									for(i=0;i<=11;i++)
									{
										if(p8[i*2]>0x39 || p8[i*2]<0x30 || p8[i*2+1]!=0x20)
										{
											break;
										}
									}
									if(i!=12)
									{
										bcd_p8(p8,1,12);
									}
									break;
								case 2:
									DisplayClrRow_4(Ln+2);//����Ļ��ʾ��ָ����(4�������ֲ���)
									DisplayString(Ln+2,9,0,"OK");
									//�������õ�����
									p8=(u8 *)ADDR_DATABUFF;
									MR(ADDR_DATABUFF+1024,ADDR_4001+1,LENmax_4001-1);//��ͨ�ŵ�ַ
									p=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+4*2);
									p8[0]=(p[0*2]-0x30)*16+(p[1*2]-0x30)*1;
									p8[1]=(p[2*2]-0x30)*16+(p[3*2]-0x30)*1;
									p8[2]=(p[4*2]-0x30)*16+(p[5*2]-0x30)*1;
									p8[3]=(p[6*2]-0x30)*16+(p[7*2]-0x30)*1;
									p8[4]=(p[8*2]-0x30)*16+(p[9*2]-0x30)*1;
									p8[5]=(p[10*2]-0x30)*16+(p[11*2]-0x30)*1;
									i=DataComp(ADDR_DATABUFF,ADDR_DATABUFF+1024+1,6);
									if((i!=0)||(p8[1024]!=6))//�������µĵ�ַ
									{
										MWR(6,ADDR_4001+1,1);//дͨ�ŵ�ַ
										MW(ADDR_DATABUFF,ADDR_4001+2,6);//дͨ�ŵ�ַ
										UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
										UARTCtrl->ReNewLink=0x55;//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
										UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
										UARTCtrl->ReNewLink=0x55;//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
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
									p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
									p16[0]=0x2020;
									p16[1]=0x2020;
									p16[2]=32;//'���Ҽ�ͷ'
									p16[3]=33;
									p8=(u8 *)(ADDR_STRINGBUFF+((Ln+1))*84+4+(MenuCount->v1)*2);
									//ͨ�ŵ�ַBCDռ6���ֽڣ��ж��ڴ���������д����ʼ��
									for(i=0;i<=11;i++)
									{
										if(p8[i*2]>0x39 || p8[i*2]<0x30)
										{
											break;
										}
									}
									if(i!=12)
									{
										bcd_p8(p8,1,12);
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
// Function name:       Menu2_2_4
// Descriptions:        ������������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2_4(void)
{
	u32 x;
	u32 i;
	u32 j;
	u32 k;
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u32 Ln;

#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu2_2_4);//��ʾ�˵�
			/*��ʾ�ڴ�������*/
			x=MRR(ADDR_MenuKey,3);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
			DisplayData(x,6,0,p16);
			MenuCount->v1=5;
			MenuCount->v2=10;
			ArrowLR(Ln+Stack->MenuStack[MenuCount->Count].EditRow,18);//��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
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
							MenuCount->v1=5;
							MenuCount->v2=10;
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
					}
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{//�Ҽ�
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8_add1(p8,6);
								break;
						}
					}
					if((Comm_Ram->ButtonVal&0x04)!=0)
					{//���
						switch(Stack->MenuStack[MenuCount->Count].EditRow)
						{
							case 1:
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+5*2);
								p8_sub1(p8,6);
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
							case 2:
								DisplayString(Ln+2,6,0,"   OK   ");
								/*�������õ�����*/
								p32=(u32 *)ADDR_DATABUFF;
								p8=(u8 *)(ADDR_STRINGBUFF+(Ln+1)*84+4+5*2);
								p32[0]=(p8[0]-0x30)*16*16*16*16*16+(p8[2]-0x30)*16*16*16*16+(p8[4]-0x30)*16*16*16+(p8[6]-0x30)*16*16+(p8[8]-0x30)*16+(p8[10]-0x30)*1;
								MW(ADDR_DATABUFF,ADDR_MenuKey,3);//д�˵�����
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
// Function name:       Menu2_2
// Descriptions:        ����������������鿴
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu2_2(void)//����������������鿴
{
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
						MenuCreate((u32)Menu2_2_1);//ͨ��ͨ������    
						break;
					case 2:
						MenuCreate((u32)Menu2_2_2);//����������
						break;
					case 3:
						MenuCreate((u32)Menu2_2_3);//������ʱ������
						break;
					case 4:
						MenuCreate((u32)Menu2_2_4);//������������
						break;
					case 5:
						MenuCreate((u32)Menu2_2_5);//��������ַ����
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
