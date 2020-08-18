
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_F000.h"
#include "../Calculate/Calculate.h"
#include "../DL698/Info_3762.h"

u32 Get_Request_4500(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u8 *p8s;
	u32 Len;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	if(LENmax_TxSPACE<2)
	{
		GET_Request_Frame(PORTn);//��֡��־
		return 0;
	}
	
	p8s=(u8*)ADDR_45000C00;
	p8tx[0]=DataType_visible_string;
	p8tx[1]=0x1A;
	
	//type=
	p8tx[2] = 0x74;
	p8tx[3] = 0x79;
	p8tx[4] = 0x70;
	p8tx[5] = 0x65;
	p8tx[6] = 0x3d;
	
	//ģ������ 3fд��
	p8tx[7] = 0x33;
	p8tx[8] = 0x66;
	
	p8tx[9] = 0x3b;
	
	//id=
	p8tx[10] = 0x69;
	p8tx[11] = 0x64;
	p8tx[12] = 0x3d;
	
	MR((u32)p8tx+13,(u32)p8s+2,15);
	Len=0x1C;
	return Len;
}

u32 GET_Request_F200(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Len;
	u8* p8s;
	u8* p8s1;
	u8* p8s2;
	u8* p8s3;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 Array;
	u32 NUMmax;
	u32 n;
	u32 m,M;
	u32 j,k,L;
//	u32 data;

	Array=Array;
	NUMmax=NUMmax;
	n=n;
	m=m;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	if(LENmax_TxSPACE<2)
	{
		GET_Request_Frame(PORTn);//��֡��־
		return 0;
	}
	p8tx[0]=DataType_array;
	p8tx[1]=0;
	Len=2;
	switch(OAD>>16)//OI
	{
		case 0xF200://�豸����-RS232
			switch((OAD>>8)&0xff)//����
			{
				case 2:
					//����2���豸�����б�ֻ������=array �˿�
					//�˿ڡ�=structure
					//{
					//  �˿������� visible-string��
					//�˿ڲ���   COMDCB��
					//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
					//}
					if(LENmax_TxSPACE<19)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8tx[1]=1;//������
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=3;
					p8tx[Len+2]=DataType_visible_string;
					p8tx[Len+3]=5;
					p8tx[Len+4]='R';
					p8tx[Len+5]='S';
					p8tx[Len+6]='2';
					p8tx[Len+7]='3';
					p8tx[Len+8]='2';
					p8tx[Len+9]=DataType_COMDCB;
					Len+=10;
					MR((u32)p8tx+Len,ADDR_F200_2+8,5);
					Len+=5;
					p8tx[Len+0]=DataType_enum;
					Len+=1;
					MR((u32)p8tx+Len,ADDR_F200_2+14,1);
					Len+=1;
					return Len;
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
			//return 0;
		case 0xF201://�豸����-RS485
			switch((OAD>>8)&0xff)//����
			{
				case 2:
					//����2���豸�����б�ֻ������=array �˿�
					//�˿ڡ�=structure
					//{
					//  �˿������� visible-string��
					//�˿ڲ���   COMDCB��
					//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
					//}
					if(LENmax_TxSPACE<40)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8tx[1]=2;//������
					//485-1
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=3;
					p8tx[Len+2]=DataType_visible_string;
					p8tx[Len+3]=7;
					p8tx[Len+4]='R';
					p8tx[Len+5]='S';
					p8tx[Len+6]='4';
					p8tx[Len+7]='8';
					p8tx[Len+8]='5';
					p8tx[Len+9]='-';
					p8tx[Len+10]='1';
					p8tx[Len+11]=DataType_COMDCB;
					Len+=12;
					MR((u32)p8tx+Len,ADDR_F201_2+8,5);
					Len+=5;
					p8tx[Len+0]=DataType_enum;
					Len+=1;
					MR((u32)p8tx+Len,ADDR_F201_2+14,1);
					Len+=1;
					//485-2
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=3;
					p8tx[Len+2]=DataType_visible_string;
					p8tx[Len+3]=7;
					p8tx[Len+4]='R';
					p8tx[Len+5]='S';
					p8tx[Len+6]='4';
					p8tx[Len+7]='8';
					p8tx[Len+8]='5';
					p8tx[Len+9]='-';
					p8tx[Len+10]='2';
					p8tx[Len+11]=DataType_COMDCB;
					Len+=12;
					MR((u32)p8tx+Len,ADDR_F201_2+LENper_F201_2+8,5);
					Len+=5;
					p8tx[Len+0]=DataType_enum;
					Len+=1;
					MR((u32)p8tx+Len,ADDR_F201_2+LENper_F201_2+14,1);
					Len+=1;
					return Len;
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
			//return 0;
		case 0xF202://�豸����-����
			switch((OAD>>8)&0xff)//����
			{
				case 2:
					//����2���豸�����б�ֻ������=array �˿�
					//�˿ڡ�=structure
					//{
					//  �˿������� visible-string��
					//�˿ڲ���   COMDCB��
					//}
					if(LENmax_TxSPACE<17)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8tx[1]=1;//������
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=3;
					p8tx[Len+2]=DataType_visible_string;
					p8tx[Len+3]=5;
					p8tx[Len+4]='I';
					p8tx[Len+5]='R';
					p8tx[Len+6]=' ';
					p8tx[Len+7]=' ';
					p8tx[Len+8]=' ';
					p8tx[Len+9]=DataType_COMDCB;
					Len+=10;
					MR((u32)p8tx+Len,ADDR_F202_2+8,5);
					Len+=5;
					return Len;
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
			//return 0;
		case 0xF203://����������
			switch((OAD>>8)&0xff)//����
			{
				case 0x42://״̬����λ������״̬
					i=OAD&0xff;
					if((i>NUMmax_SWITCHIN)||(i==0))
					{
						return 0;
					}
					if(LENmax_TxSPACE<6)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					i--;
					i*=6;
					i+=ADDR_F203_2+2;
					MR((u32)p8tx,i,6);
					return 6;
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
			//reutrn 0;
		case 0xF204://ֱ��ģ����
			return 0;
		case 0xF205://�̵������
			//����2���豸�����б�ֻ������=array �̵�����Ԫ(�ڶ�ͨ��ʱ��д)
			//�̵�����Ԫ��=structure
			//{
			//  ������    visible-string��
			//��ǰ״̬  enum{δ�����0���������1��}��
			//��������  enum{����ʽ��0��������ʽ��1��}��
			//����״̬  enum{���루0����δ���루1) }
			//}
			p8d=(u8*)ADDR_F205_2;
			p8d[0]=DataType_array;
			p8d[1]=2;
			p8d+=2;
		//1
			p8d[0]=DataType_structure;
			p8d[1]=4;
			p8d[2]=DataType_visible_string;
			p8d[3]=0;
			p8d[4]=DataType_enum;//��ǰ״̬  enum{δ�����0���������1��}
			p8s=(u8*)ADDR_Terminal_Remote_State;//����״̬,0=�ϵ�򲻿���,1=Ͷ��,2=...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
			if(p8s[0]<102)
			{
				p8d[5]=0;//{δ�����0���������1��}
			}
			else
			{
				p8d[5]=1;//{δ�����0���������1��}
			}
			p8d[6]=DataType_enum;//��������  enum{����ʽ��0��������ʽ��1��}
			p8d[7]=0;
			p8d[8]=DataType_enum;//����״̬  enum{���루0����δ���루1) }
			p8s=(u8*)ADDR_ContrlUARTData;//����ģ�������������
			if(p8s[2]&1)
			{
				p8d[9]=1;
			}
			else
			{
				p8d[9]=0;
			}
			p8d+=10;
		//2
			p8d[0]=DataType_structure;
			p8d[1]=4;
			p8d[2]=DataType_visible_string;
			p8d[3]=0;
			p8d[4]=DataType_enum;//��ǰ״̬  enum{δ�����0���������1��}
			p8s=(u8*)ADDR_Terminal_Remote_State+Terminal_Ctrl_Pn_Len;//����״̬,0=�ϵ�򲻿���,1=Ͷ��,2=...,100=����,101=��բ׼��,102=��բ,103=��բ2���Ӻ�
			if(p8s[0]<102)
			{
				p8d[5]=0;//{δ�����0���������1��}
			}
			else
			{
				p8d[5]=1;//{δ�����0���������1��}
			}
			p8d[6]=DataType_enum;//��������  enum{����ʽ��0��������ʽ��1��}
			p8d[7]=0;
			p8d[8]=DataType_enum;//����״̬  enum{���루0����δ���루1) }
			p8s=(u8*)ADDR_ContrlUARTData;//����ģ�������������
			if(p8s[2]&4)
			{
				p8d[9]=1;
			}
			else
			{
				p8d[9]=0;
			}
			return 0;
		case 0xF206://�澯���
			//����2���豸�����б�ֻ������=array �澯���(�ڶ�ͨ��ʱ��д)
			//�澯�����=enum
			//{
			//δ�����0���������1��
			//}
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			p8d=(u8*)ADDR_F206_2;
			p8d[0]=DataType_array;
			p8d[1]=0;
	#endif
	#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			p8d=(u8*)ADDR_F206_2;
			p8d[0]=DataType_array;
			p8d[1]=1;
			p8d+=2;
			p8d[0]=DataType_enum;
			p8s=(u8*)ADDR_ContrlUARTData;//����ģ�������������
//#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//����ģ�������������
//1BYTE	Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
//1BYTE Ҫ���������ģ��Ŀ���1:b0=�澯�̵�������,b1=����ָʾ��,b2-b6,b7=�ִμ̵���������ʽ(0=����1=�ڲ��Զ�����ʱ��ƽ)
			if(p8s[1]&1)//b0=�澯�̵�������
			{
				p8d[1]=1;
			}
			else
			{
				p8d[1]=0;
			}
	#endif
	#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
			{
				p8d=(u8*)ADDR_F206_2;
				p8d[0]=DataType_array;
				p8d[1]=0;
			}
			else
			{
				p8d=(u8*)ADDR_F206_2;
				p8d[0]=DataType_array;
				p8d[1]=1;
				p8d+=2;
				p8d[0]=DataType_enum;
				p8s=(u8*)ADDR_ContrlUARTData;//����ģ�������������
//#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//����ģ�������������
//1BYTE	Ҫ���������ģ��Ŀ���0:b0=�ִ�1�̵�������,b1=�ִ�1ָʾ�Ƶ���,b2=�ִ�1ָʾ�Ƶ��̵�,b3=�ִ�2�̵�������,b4=�ִ�2ָʾ�Ƶ���,b5=�ִ�2ָʾ�Ƶ��̵�,b6=����ָʾ�Ƶ���,b7=���ָʾ�Ƶ���
//1BYTE Ҫ���������ģ��Ŀ���1:b0=�澯�̵�������,b1=����ָʾ��,b2-b6,b7=�ִμ̵���������ʽ(0=����1=�ڲ��Զ�����ʱ��ƽ)
			if(p8s[1]&1)//b0=�澯�̵�������
				{
					p8d[1]=1;
				}
				else
				{
					p8d[1]=0;
				}
			}
	#endif
			return 0;
			
		case 0xF207://�๦�ܶ���
			return 0;
		
		case 0xF209://�ز�/΢�������߽ӿ�
			switch((OAD>>8)&0xff)//����
			{
			#if (((Project/100)==2)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
				case 2:
					//����2���豸�����б�ֻ������=array ����ͨ��ģ�鵥Ԫ
					//����ͨ��ģ�鵥Ԫ��=structure
					//{
					//�˿�������   visible-string��
					//ͨ�Ų���     COMDCB��
					//�汾��Ϣ     VersionInfo
					//}
					//VersionInfo��=structure
					//{
					//  ���̴���    visible-string(SIZE(2))��
					//  оƬ����    visible-string(SIZE(2))��
					//  �汾����     Date��
					//  ����汾     long-unsigned
					//}
					Len=Get_DL698DataLen_S((u8*)ADDR_F209_2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);
					if(LENmax_TxSPACE<Len)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					// �˿�������
					p8s=UART_3762_AFNxFn(0x03,12);
					M=p8s[2];
					MR((u32)p8tx,ADDR_F209_2,Len);
		
					//�˿�������	visible-string
					p8tx[4] = DataType_visible_string;
					p8tx[5] = 129; 
					p8tx[6] = 105+2*M; 
					//type=
					p8tx[7] = 0x74;
					p8tx[8] = 0x79;
					p8tx[9] = 0x70;
					p8tx[10] = 0x65;
					p8tx[11] = 0x3d;
					
					//ģ������ PLCд��
					p8tx[12] = 0x50;
					p8tx[13] = 0x4c;
					p8tx[14] = 0x43;

					// ;
					p8tx[15] = 0x3b;

					//mfrs=
					p8tx[16] = 0x6d;
					p8tx[17] = 0x66;
					p8tx[18] = 0x72;
					p8tx[19] = 0x73;
					p8tx[20] = 0x3d;

					// ���̴���
					i=p8s[0]+(p8s[1]<<8);
					p8tx[21]=i>>8;
					p8tx[22]=i;

					//;
					p8tx[23] = 0x3b; 
					
					//idformat=
					p8tx[24] = 0x69; 
					p8tx[25] = 0x64; 
					p8tx[26] = 0x66; 
					p8tx[27] = 0x6f; 
					p8tx[28] = 0x72; 
					p8tx[29] = 0x6d; 
					p8tx[30] = 0x61; 
					p8tx[31] = 0x74; 
					p8tx[32] = 0x3d; 

					// bcd bin ascll
					p8tx[33] = 0x32;

					// ��
					p8tx[34] = 0x3b;

					// id =
					p8tx[35] = 0x69;
					p8tx[36] = 0x64;
					p8tx[37] = 0x3d;
					MR((u32)(p8tx + 38 + 49),(ADDR_AFN10F112 + 9),24);
					Data_Inverse((u32)p8tx+87,24);
					HexToASCII(&p8tx[87],&p8tx[38],24);

					// ��
					p8tx[62 + 24] = 0x3b;	

					// mmfrs=
					p8tx[63+ 24] = 0x6d;
					p8tx[64+ 24] = 0x6d;
					p8tx[65+ 24] = 0x66;
					p8tx[66+ 24] = 0x72;
					p8tx[67+ 24] = 0x73;
					p8tx[68+ 24] = 0x3d;

					//  ģ�鳧�̴���
					i=p8s[0]+(p8s[1]<<8);
					p8tx[69+ 24]=i>>8;
					p8tx[70+ 24]=i;

					// ��
					p8tx[71+ 24] = 0x3b;

					// midformat=
					p8tx[72+ 24] = 0x6d;
					p8tx[73+ 24] = 0x69;
					p8tx[74+ 24] = 0x64;
					p8tx[75+ 24] = 0x66;
					p8tx[76+ 24] = 0x6f;
					p8tx[77+ 24] = 0x72;
					p8tx[78+ 24] = 0x6d;
					p8tx[79+ 24] = 0x61;
					p8tx[80+ 24] = 0x74;
					p8tx[81+ 24] = 0x3d;

					// 02
					p8tx[82+ 24] = 0x31;

					// ;
					p8tx[83+ 24] = 0x3b;
					
					//mid =
					p8tx[84+ 24] = 0x6d;
					p8tx[85+ 24] = 0x69;
					p8tx[86+ 24] = 0x64;
					p8tx[87+ 24] = 0x3d;

					// ģ��ID��Ϣ	
					Data_Inverse((u32)p8s+4,M);	
					HexToASCII(&p8s[4],&p8tx[112],M);
//					p8s2 = (u8*)ADDR_F209_2;
					MR((u32)(p8tx+6+128),(ADDR_F209_2 + 6),Len-6);

					// оƬ���̴���
					p8tx[21] = p8tx[6+128+14];
					p8tx[22] = p8tx[6+128+15];
					Len+=128;
					return Len;
				case 5:
					//����5���ӽڵ�����б�ֻ������=array �ӽڵ㵥Ԫ
					//�ӽڵ㵥Ԫ	����2��=structure
					//{
					//  �ӽڵ����      long-unsigned,
					//  �ӽڵ�ͨ�ŵ�ַ  octet-string,
					//  �ӽڵ�������    visible-string
					//}
					//ÿ�ڵ��ֽ���=2+3+8+2=15
					p8tx[0]=DataType_array;
					Len=2;
					Array=0;//������������
#if(USER/100)==17//����ϵ
					p8s1=(u8*)ADDR_AFN10F7;
					NUMmax = p8s1[0];
					NUMmax |=p8s1[1]<<8;

#else
					NUMmax=Terminal_Router->NoteNum;//��ǰ·��������֪�ܴӽڵ���(���ֶ���)
#endif	
					n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
					while(1)
					{
						if(n>=NUMmax)
						{
							break;
						}
						if((Array>=127)||(LENmax_TxSPACE<171))
						{//���ͻ��岻��,��֡
							if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
							{
								UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
							}
							else
							{
								UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
							}
							UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
							p8tx[1]=Array;//������������
							return Len;
						}
					//F210�ӽڵ㵥Ԫ	����2��=structure
						p8tx[Len+0]=DataType_structure;
						p8tx[Len+1]=3;
					//{
					//  �ӽڵ����      long-unsigned,
						p8tx[Len+2]=DataType_long_unsigned;
						p8tx[Len+3]=(n+1)>>8;
						p8tx[Len+4]=n+1;
					//  �ӽڵ�ͨ�ŵ�ַ  octet-string,
						p8tx[Len+5]=DataType_octet_string;
						p8tx[Len+6]=6;
						MR((u32)p8tx+Len+7,ADDR_AFN10F7+2+(n*61),6);
						p8s1=(u8*)ADDR_AFN10F7;
						for(j=0;j<NUM_RMmax;j++)
						{
							if(Compare_DL698DataString(p8s1 + 2 + 61 * j ,p8tx+Len+7, 6) == 0)
							{
								break;
							}
						}
						p8s2= (u8*)ADDR_AFN10F104;
						for(L=0;L<NUM_RMmax;L++)
						{
							if(Compare_DL698DataString(p8s2 + 2 + 15 * L ,p8tx+Len+7, 6) == 0)
							{
								break;
							}
						}
						p8s3=(u8*)ADDR_AFN10F112;
						for(k=0;k<NUM_RMmax;k++)
						{
							if(Compare_DL698DataString(p8s3 + 2 + 33 * k ,p8tx+Len+7, 6) == 0)
							{
								break;
							}
						}

						
						Data_Inverse((u32)p8tx+Len+7,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2

						M = p8s1[(61*j) + 9 +2];
	
					//  �ӽڵ�������    visible-string
						p8tx[Len+13]=DataType_visible_string;
#if 0					
						if(n==0)
						{
							p8tx[Len+14]=108;
						}
						else
						{
							p8tx[Len+14]=129;
							p8tx[Len+15]=128;
						}
#else						
						p8tx[Len+14]=129;
						p8tx[Len+15]=103+2*M;
#endif

						//128 �ֽ�ģ��ID��Ϣ
						//type=
						p8tx[Len+16] = 0x74;
						p8tx[Len+17] = 0x79;
						p8tx[Len+18] = 0x70;
						p8tx[Len+19] = 0x65;
						p8tx[Len+20] = 0x3d;

#if 0
						//ģ������ PLCд��
						p8tx[Len+21] = 0x50;
						p8tx[Len+22] = 0x4c;
						p8tx[Len+23] = 0x43;
#else
						//HexToASCII(p8s3 + (33 * k) + 2 + 6,&p8tx[Len + 21],1);
						p8tx[Len+21]=(p8s3[(33 * k) + 2 + 6] + 0x30); 
#endif

						// ;
						p8tx[Len+22] = 0x3b;

						//mfrs=
						p8tx[Len+23] = 0x6d;
						p8tx[Len+24] = 0x66;
						p8tx[Len+25] = 0x72;
						p8tx[Len+26] = 0x73;
						p8tx[Len+27] = 0x3d;

						// оƬ���̴���
#if 0
						if(n==0)
						{
							MR((u32)(p8tx+Len+30),(ADDR_F209_2 + 20),2);
						}
						else
						{
							MR((u32)(p8tx + Len + 30),(ADDR_AFN10F112 + 2 + 7 + 10 +(33 * n)) ,2);
						}
#else

						//�ڵ������� BIN 2
						//�ڵ�1��ַ BIN 6
						//�ڵ�1����汾 BIN 2
						//�ڵ�1����汾���� BIN 3
						//�ڵ�1ģ�鳧�̴��� BIN 2
						//�ڵ�1оƬ���� BIN 2
						//...
						//�ڵ�n��ַ BIN 6
						//�ڵ�n����汾 BIN 2
						//�ڵ�n����汾���� BIN 3
						//�ڵ�nģ�鳧�̴��� BIN 2
						//�ڵ�nоƬ���� BIN 2

						p8tx[Len+29]=p8s2[13+ 2 + 15*L];
						p8tx[Len+28]=p8s2[14+ 2 + 15*L];
//						Data_Inverse((u32)p8tx+Len+29,2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2 								
#endif
						//;
						p8tx[Len+30] = 0x3b; 
						
						//idformat=
						p8tx[Len+31] = 0x69; 
						p8tx[Len+32] = 0x64; 
						p8tx[Len+33] = 0x66; 
						p8tx[Len+34] = 0x6f; 
						p8tx[Len+35] = 0x72; 
						p8tx[Len+36] = 0x6d; 
						p8tx[Len+37] = 0x61; 
						p8tx[Len+38] = 0x74; 
						p8tx[Len+39] = 0x3d; 

						// bcd bin ascll
						p8tx[Len+40] = 0x32;

						// ��
						p8tx[Len+41] = 0x3b;

						// id =
						p8tx[Len+42] = 0x69;
						p8tx[Len+43] = 0x64;
						p8tx[Len+44] = 0x3d;

						//p8s2 = (u8*)ADDR_AFN10F112;
						MR((u32)(p8tx + Len + 45 + 49),(ADDR_AFN10F112 + 2 + 7 + (33 * k)) ,24);
						Data_Inverse((u32)p8tx+Len+94,24);
						HexToASCII(&p8tx[Len + 94],&p8tx[Len+45],24);

						// ��
						p8tx[Len + 93] = 0x3b;	

						// mmfrs=
						p8tx[Len + 94] = 0x6d;
						p8tx[Len + 95] = 0x6d;
						p8tx[Len + 96] = 0x66;
						p8tx[Len + 97] = 0x72;
						p8tx[Len + 98] = 0x73;
						p8tx[Len + 99] = 0x3d;

						//	ģ�鳧�̴���
#if	0
						if(n==0)
						{
							p8s=UART_3762_AFNxFn(0x03,12);
							i=p8s[0]+(p8s[1]<<8);
							p8tx[Len + 104]=i>>8;
							p8tx[Len + 105]=i;

						}
						else
						{
							p8s1=(u8*)ADDR_AFN10F7;
							i=p8s1[2+7+61*m]+(p8s1[2+7+1+61*m]<<8);
							p8tx[Len + 104]=i>>8;
							p8tx[Len + 105]=i;
						
}
#else
						p8tx[Len + 101]=p8s2[11 + 2 + 15*L];;
						p8tx[Len + 100]=p8s2[12 + 2 + 15*L];;
#endif

						// ��
						p8tx[Len + 102] = 0x3b;

						// midformat=
						p8tx[Len + 103] = 0x6d;
						p8tx[Len + 104] = 0x69;
						p8tx[Len + 105] = 0x64;
						p8tx[Len + 106] = 0x66;
						p8tx[Len + 107] = 0x6f;
						p8tx[Len + 108] = 0x72;
						p8tx[Len + 109] = 0x6d;
						p8tx[Len + 110] = 0x61;
						p8tx[Len + 111] = 0x74;
						p8tx[Len + 112] = 0x3d;

						// 02
						p8tx[Len + 113] = 0x31;

						// ;
						p8tx[Len + 114] = 0x3b;
						
						//mid =
						p8tx[Len + 115] = 0x6d;
						p8tx[Len + 116] = 0x69;
						p8tx[Len + 117] = 0x64;
						p8tx[Len + 118] = 0x3d;

						// ģ��ID��Ϣ		
#if 0
						if(n==0)
						{
							HexToASCII(&p8s[4],&p8tx[Len + 124],11);
						}
						else
						{
							HexToASCII(&p8s1[2+11+n*61],&p8tx[Len + 124],11);
						}
#else
						Data_Inverse((u32)p8s1+2+11+j*61,M);
						HexToASCII(&p8s1[2+11+j*61],&p8tx[Len + 119],M);
						Data_Inverse((u32)p8s1+2+11+j*61,M);	// �� ram ֱ�Ӳ�������Ҫת����������ı�ԭ������
#endif						
					//  �ӽڵ���Ӧʱ��
//						p8tx[Len+16+p8tx[Len+15]]=DataType_long_unsigned;
//						p8tx[Len+17+p8tx[Len+15]]=0;
//						p8tx[Len+18+p8tx[Len+15]]=3;
						
					//�ӽڵ����һ��ͨ�ųɹ�ʱ�� 6032
//						p8tx[Len+19+p8tx[Len+15]]=DataType_date_time_s;
//						// ע��7 �ֽ�
//						p8s = (u8*)ADDR_6032;
//						for(i=0;i<NUM_RMmax;i++)
//						{
//							if(Compare_DL698DataString(&p8tx[Len+7],(u8*)(ADDR_6032 +  5 + (64 * i)), 6) == 0)
//							{
//								break;
//							}
//						}
//						MR((u32)(p8tx + Len + 20 + p8tx[Len+15]),(u32)(ADDR_6032 + 20 + (64 * i)) ,7);
						
					//}
#if 0
						if(n==0)
						{
							Len+=(16+108);
						}
						else
						{
							Len+=(16+128);
						}		
#else
						Len+=(16+125);
#endif						
						LENmax_TxSPACE-=(16+125);
						Array++;//������������
						n++;
					}
					if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					{
						UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
					p8tx[1]=Array;//������������
					return Len;
				case 6:
					//����6���ӽڵ�����б�������ڣ���= TI
					//�������ڣ�ָ�ӽڵ�����б�����������Ϣ��������Ϣ�ȸ�������
					if(LENmax_TxSPACE<4)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					//TI��=SEQUENCE
					//{
					//   ��λ  ENUMERATED
					//   {
					//       ��      ��0����
					//       ��      ��1����
					//       ʱ      ��2����
					//       ��      ��3����
					//       ��      ��4����
					//       ��      ��5��
					//    }��
					//    ���ֵ  long-unsigned
					//}
					if(LENmax_TxSPACE<4)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					MR((u32)p8tx,ADDR_F209_6,4);
					Len=4;
					return Len;
				case 7:
					//����7�������ز���Ϣ��ֻ����::=array�����ز���Ԫ
				#if (((USER/100)==9)||((USER/100)==17))//���Ϻͽ���
					i=0;
					n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
					p8s=(u8*)ADDR_DATABUFF;
					//�ڵ���ʼ��ţ�ÿ�β�ѯ��������1��ʼ��ѯ
					while(i<n)
					{//���Ǵ�0��ʼ
						m=n-i;
						if(m>15)
						{
							m=15;
						}
						UART_3762_AFN03F101(PORTn,i+1,m,(u8*)ADDR_DATABUFF);//(����)��ѯ�ڵ�̨����Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���ʼ�����֡��0��ʼ,Num=�ڵ�����(���֧��15��),p8Buff=���ݷ��û���
						NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
						if(n>=NUMmax)
						{
							break;
						}
						m=p8s[4];//����Ӧ��Ľڵ�����
						if(m==0)
						{
							break;
						}
						i+=m;
					}
					p8tx[0]=DataType_array;
					Len=2;
					Array=0;//������������
					while(1)
					{
						UART_3762_AFN10F21(PORTn,n+1,32,(u8*)ADDR_DATABUFF);//��ѯ����������Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
						p8s=(u8*)ADDR_DATABUFF;
					//���У�
						//�ڵ�������	BIN	2
						//�ڵ���ʼ���	BIN	2
						//����Ӧ��Ľڵ�����n	BIN	1
						//�ڵ�1��ַ	BIN	6
						//�ڵ�1����ڵ���Ϣ	BIN	5
						//����	����	����
						//�ڵ�n��ַ	BIN	6
						//�ڵ�n����ڵ���Ϣ	BIN	5
						NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
						m=p8s[4];//����Ӧ��Ľڵ�����
						if(m==0)
						{
							break;
						}
						p8s+=5;
						while(m)
						{
							if(n>=NUMmax)
							{
								break;
							}
							if((Array>=127)||(LENmax_TxSPACE<20))//ÿ�ڵ��ֽ���=2+8+2+3+3+2=20
							{//���ͻ��岻��,��֡
								if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								{
									UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
								p8tx[1]=Array;//������������
								//·�ɻָ�
								Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
								Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
								return Len;
							}
						//F211�����ز���Ԫ  ����2::=structure
							p8tx[Len+0]=DataType_structure;
							p8tx[Len+1]=5;
						//{
						//�ڵ��ַ   octet-string(SIZE(6)),
							p8tx[Len+2]=DataType_octet_string;
							p8tx[Len+3]=6;
							MR((u32)p8tx+Len+4,(u32)p8s,6);
							Data_Inverse((u32)p8tx+Len+4,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
						//�ڵ����� enum{���ڵ�(0),�ӽڵ�(1)},
							p8tx[Len+10]=DataType_enum;
							if(n==0)
							{
								p8tx[Len+11]=0;
							}
							else
							{
								p8tx[Len+11]=1;
							}
						//�ڵ��ʶ long-unsingned,
							p8tx[Len+12]=DataType_long_unsigned;
							p8tx[Len+13]=p8s[7];
							p8tx[Len+14]=p8s[6];
						//����ڵ��ʶ long-unsigned,
							p8tx[Len+15]=DataType_long_unsigned;
							p8tx[Len+16]=p8s[9];
							p8tx[Len+17]=p8s[8];
						//�ڵ���Ϣunsigned
							p8tx[Len+18]=DataType_unsigned;
							p8tx[Len+19]=p8s[10];
						//}
							p8s+=11;
							Len+=20;
							LENmax_TxSPACE-=20;
							Array++;//������������
							n++;
							m--;
						}
						if(m)
						{//��
							break;
						}
					}
					if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					{
						UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
					p8tx[1]=Array;//����������
					//·�ɻָ�
					Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
					Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				#else
					if(UART4Ctrl->Lock_InfoTask==0x55)//��·������������Ϣ������:0x55=����
					{
						UARTCtrl->Lock_InfoTask=0x55;//��·������������Ϣ������:0x55=����
						return 1;//��ʾ�����ݼ���
					}
					else
					{
						UARTCtrl->Lock_InfoTask=0;//��·������������Ϣ������:0x55=����
					}
					i=0;
					n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
					p8tx[0]=DataType_array;
					Len=2;
					Array=0;//������������
					while(1)
					{
						p8s=(u8*)ADDR_AFN10F21;//��·�ɶ�ȡ������������Ϣ
						//2byte �ڵ�������
						//6byte �ڵ�1��ַ
						//5byte �ڵ�1����������Ϣ
						//...
						//6byte �ڵ�n��ַ
						//5byte �ڵ�n����������Ϣ
						NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
						m=NUMmax-n;//����Ӧ��Ľڵ�����
						if(m==0)
						{
							break;
						}
						p8s+=2+(11*n);
						while(m)
						{
							if(n>=NUMmax)
							{
								break;
							}
							if((Array>=127)||(LENmax_TxSPACE<20))//ÿ�ڵ��ֽ���=2+8+2+3+3+2=20
							{//���ͻ��岻��,��֡
								if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								{
									UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
								p8tx[1]=Array;//������������
								return Len;
							}
						//F211�����ز���Ԫ  ����2::=structure
							p8tx[Len+0]=DataType_structure;
							p8tx[Len+1]=5;
						//{
						//�ڵ��ַ   octet-string(SIZE(6)),
							p8tx[Len+2]=DataType_octet_string;
							p8tx[Len+3]=6;
							MR((u32)p8tx+Len+4,(u32)p8s,6);
							Data_Inverse((u32)p8tx+Len+4,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
						//�ڵ����� enum{���ڵ�(0),�ӽڵ�(1)},
							p8tx[Len+10]=DataType_enum;
							if(n==0)
							{
								p8tx[Len+11]=0;
							}
							else
							{
								p8tx[Len+11]=1;
							}
						//�ڵ��ʶ long-unsingned,
							p8tx[Len+12]=DataType_long_unsigned;
							p8tx[Len+13]=p8s[7];
							p8tx[Len+14]=p8s[6];
						//����ڵ��ʶ long-unsigned,
							p8tx[Len+15]=DataType_long_unsigned;
							p8tx[Len+16]=p8s[9];
							p8tx[Len+17]=p8s[8];
						//�ڵ���Ϣunsigned
							p8tx[Len+18]=DataType_unsigned;
							p8tx[Len+19]=p8s[10];
						//}
							p8s+=11;
							Len+=20;
							LENmax_TxSPACE-=20;
							Array++;//������������
							n++;
							m--;
						}
						break;
					}
					if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					{
						UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
					p8tx[1]=Array;//����������
				#endif
					return Len;
				case 8:
					//���� 8��������Ϣ��ֻ������ =structure
				//#if (USER/100)==9//����
				#if (((USER/100)==9)||((USER/100)==17))//���Ϻͽ���
					p8s=UART_3762_AFNxFn(0x10,111);//��ѯ��������Ϣ,����ز�Ƶ����ŵ�(�Զ���ͣ�ͻָ�·��);���:p8Buff=���ݷ��û���
				//376.2���б������ݵ�Ԫ��ʽ
					//������ڵ�������n BIN 1
					//���ڵ������ʶ��  BIN 3
					//���ڵ����ڵ��ַ  BIN 6
					//�ھӽڵ�1�����ʶ��  BIN 3
					//����
					//�ھӽڵ�n�����ʶ��  BIN 3
				#else
					if(UART4Ctrl->Lock_InfoTask==0x55)//��·������������Ϣ������:0x55=����
					{
						UARTCtrl->Lock_InfoTask=0x55;//��·������������Ϣ������:0x55=����
						return 1;//��ʾ�����ݼ���
					}
					else
					{
						UARTCtrl->Lock_InfoTask=0;//��·������������Ϣ������:0x55=����
					}
					p8s=(u8*)ADDR_AFN10F111;
				#endif
					n=p8s[0];
					n*=3;
					n+=18;
					if(LENmax_TxSPACE<n)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
				//���� 8��������Ϣ��ֻ������ =structure
				//{
					p8tx[0]=DataType_structure;
					p8tx[1]=3;
				//���ڵ������ʶ�� octet -string(SIZE(3)) ��
					p8tx[2]=DataType_octet_string;
					p8tx[3]=3;
					p8tx[4]=p8s[3];
					p8tx[5]=p8s[2];
					p8tx[6]=p8s[1];
				//���ڵ�����ַ octet -string(SIZE(6)) ��
					p8tx[7]=DataType_octet_string;
					p8tx[8]=6;
					MR((u32)p8tx+9,(u32)p8s+4,6);
					Data_Inverse((u32)p8tx+9,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//�ھӽڵ������ʶ�� array octet -string(SIZE(3))
					p8tx[15]=DataType_array;
					n=p8s[0];
					if(n<=127)
					{
						p8tx[16]=n;
						Len=17;
					}
					else
					{
						p8tx[16]=0x81;
						p8tx[17]=n;
						Len=18;
					}
					p8s+=10;
					for(i=0;i<n;i++)
					{
						p8tx[Len+0]=DataType_octet_string;
						p8tx[Len+1]=0x03;
						p8tx[Len+2]=p8s[2];
						p8tx[Len+3]=p8s[1];
						p8tx[Len+4]=p8s[0];
						Len+=5;
						p8s+=5;
					}
				//}
					return Len;
				case 9:
					//���� 9������ز�Ƶ����ţ��� =unsigned
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8s=UART_3762_AFNxFn(0x03,16);//��ѯ��������Ϣ,����ز�Ƶ����ŵ�(�Զ���ͣ�ͻָ�·��);���:p8Buff=���ݷ��û���
					//376.2���б������ݵ�Ԫ��ʽ
					//����ز�Ƶ��  BIN 1
					//����ز�Ƶ�Σ�0��1.953��11.96MHz��1�� 2.441��5.615 MHz��2��255��ʾ����
					p8tx[0]=DataType_unsigned;
					p8tx[1]=p8s[0];
					Len=2;
					return Len;

				case 10:
					n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
					p8s=(u8*)ADDR_DATABUFF;
					//�ڵ���ʼ��ţ�ÿ�β�ѯ��������1��ʼ��ѯ
					p8tx[0]=DataType_array;
					Len=2;
					Array=0;//������������
					while(1)
					{
						UART_3762_AFN10F31(PORTn,n+1,15,(u8*)ADDR_DATABUFF);//��ѯ��λ��Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���Ŵ�1��ʼ,����1Ϊ���ڵ�,����Ϊ�ӽڵ�,Num=�ڵ�����,p8Buff=���ݷ��û���
						p8s=(u8*)ADDR_DATABUFF;
						NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
						m=p8s[4];//����Ӧ��Ľڵ�����
						if(m==0)
						{
							break;
						}
						p8s+=5;
						while(m)
						{
							if(n>=NUMmax)
							{
								break;
							}
							if((Array>=127)||(LENmax_TxSPACE<13))//ÿ�ڵ��ֽ���
							{//���ͻ��岻��,��֡
								if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								{
									UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
								p8tx[1]=Array;//������������
								//·�ɻָ�
								Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
								Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
								return Len;
							}
						//�ڵ���λ��Ϣ��Ԫ  ����2::=structure
							p8tx[Len+0]=DataType_structure;
							p8tx[Len+1]=3;
						//{

							//�ڵ����
							p8tx[Len+2]=DataType_long_unsigned;
							p8tx[Len+3]=(n + 1)>>8;
							p8tx[Len+4]=(n + 1);

							//�ڵ��ַ   octet-string(SIZE(6)),
							p8tx[Len+5]=DataType_octet_string;
							p8tx[Len+6]=6;
							MR((u32)p8tx+Len+7,(u32)p8s,6);
							Data_Inverse((u32)p8tx+Len+7,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2

							//�ڵ���λ��Ϣ},
							p8tx[Len+13]=DataType_long_unsigned;
							p8tx[Len+14]=p8s[7];
							p8tx[Len+15]=p8s[6];
						//}
							p8s+=8;
							Len+=16;
							LENmax_TxSPACE-=16;
							Array++;//������������
							n++;
							m--;
						}
						if(m)
						{//��
							break;
						}
					}
					if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					{
						UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
					p8tx[1]=Array;//����������
					//·�ɻָ�
					Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
					Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
					return Len;
					
				case 11:
					//̨��ʶ���־ enum{���ڵ�(0),�ӽڵ�(1)},
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8s=UART_3762_AFNxFn(0x10,4);//��ѯ̨��ʶ��״̬��ȡ,����Э���ȡ��ֻ֧��AFN05_F6
					p8tx[0]=DataType_enum;
					if(p8s[7] & 0x08)
						p8tx[1]=1;
					else
						p8tx[1]=0;
					Len=2;
					return Len;

				case 12:
					//����  12���ڵ�汾��Ϣ��ֻ������=array  �ڵ�汾��Ϣ��Ԫ
#if 0
					�ڵ�汾��Ϣ��Ԫ��=structure
					{
						�ڵ��ַ  octet-string(SIZE(6))��
						�ڵ�����  enum{���ڵ�(0)���ӽڵ�(1)}��
						����汾��  long-unsigned��
						����汾����  Date��
						ģ�鳧�̴���  visible-string(SIZE(2))��
						оƬ����  visible-string(SIZE(2))
					}				
#endif
					n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
					if(LENmax_TxSPACE<29)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					// ���ڵ�汾��Ϣ
					p8tx[0]=DataType_array;
					p8s1=(u8*)ADDR_DATABUFF;
					MR((u32)p8s1,ADDR_F209_2,Get_DL698DataLen_S((u8*)ADDR_F209_2,0));
					
					Len=2;
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=6;	
					
					//�ڵ��ַ	 octet-string(SIZE(6)),
					p8tx[Len+2]=DataType_octet_string;
					p8tx[Len+3]=6;
					MR((u32)p8tx+Len+4,ADDR_4001+2,6);
					Data_Inverse((u32)p8tx+Len+4,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2

					//�ڵ����� enum{���ڵ�(0),�ӽڵ�(1),
					p8tx[Len+10]=DataType_enum;
					p8tx[Len+11]=0;

					//����汾��  ,
					p8tx[Len+12]=DataType_long_unsigned;
					p8tx[Len+13]=p8s1[29];
					p8tx[Len+14]=p8s1[30];

					// ����汾����  Date
					p8tx[Len+15]=DataType_date;
					p8tx[Len+16]=p8s1[23];
					p8tx[Len+17]=p8s1[24];
					p8tx[Len+18]=p8s1[25];
					p8tx[Len+19]=p8s1[26];
					p8tx[Len+20]=0;
					
					//  ���̴���    visible-string(SIZE(2))��
					p8tx[Len+21]=DataType_visible_string;
					p8tx[Len+22]=2;
					p8tx[Len+23]=p8s1[16];
					p8tx[Len+24]=p8s1[17];

					//  оƬ����    visible-string(SIZE(2))��
					p8tx[Len+25]=DataType_visible_string;
					p8tx[Len+26]=2;
					p8tx[Len+27]=p8s1[20];
					p8tx[Len+28]=p8s1[21];

					Len+=29;
					LENmax_TxSPACE-=29;

					// �ӽڵ���Ϣ
					p8s=(u8*)ADDR_AFN10F104;
					n=p8s[0];
					n|=p8s[1]<<8;
					p8s+=2;
					Array=1;//������������
					while(1)
					{
						if(n==0)
						{
							break;
						}
						while(n)
						{
							if(n >= NUMmax)
							{
								break;
							}
							if((Array>=127)||(LENmax_TxSPACE<29))//ÿ�ڵ��ֽ���
							{//���ͻ��岻��,��֡
								if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								{
									UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								UARTCtrl->NEXTOADSubNum=m;//DL698��֡����OAD���Ӽ���
								p8tx[1]=Array;//������������
								return Len;
							}
							p8tx[Len+0]=DataType_structure;
							p8tx[Len+1]=6;	
							
							//�ڵ��ַ	 octet-string(SIZE(6)),
							p8tx[Len+2]=DataType_octet_string;
							p8tx[Len+3]=6;
							MR((u32)p8tx+Len+4,(u32)p8s,6);
							Data_Inverse((u32)p8tx+Len+4,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2

							//�ڵ����� enum{���ڵ�(0),�ӽڵ�(1),
							p8tx[Len+10]=DataType_enum;
							p8tx[Len+11]=1;

							//����汾��  ,
							p8tx[Len+12]=DataType_long_unsigned;
							i=p8s[6]+(p8s[7]<<8);
							i=bcd_hex(i);
							p8tx[Len+13]=i>>8;;
							p8tx[Len+14]=i;

							// ����汾����  Date
							p8tx[Len+15]=DataType_date;
							i=p8s[10];	//��
							i=bcd_hex(i);
							i+=2000;
							p8tx[Len+16]=i>>8;
							p8tx[Len+17]=i;
							i=p8s[9];	//��
							i=bcd_hex(i);
							p8tx[Len+18]=i;
							i=p8s[8];	//��
							i=bcd_hex(i);
							p8tx[Len+19]=i;
							p8tx[Len+20]=0;	//��

							//  ���̴���    visible-string(SIZE(2))��
							p8tx[Len+21]=DataType_visible_string;
							p8tx[Len+22]=2;
							i=p8s[11]+(p8s[12]<<8);
							p8tx[Len+23]=i>>8;
							p8tx[Len+24]=i;
							
							//  оƬ����    visible-string(SIZE(2))��
							p8tx[Len+25]=DataType_visible_string;
							p8tx[Len+26]=2;
							i=p8s[13]+(p8s[14]<<8);
							p8tx[Len+27]=i>>8;
							p8tx[Len+28]=i;

							p8s+=15;
							Len+=29;
							LENmax_TxSPACE-=29;
							Array++;//������������
							m++;
							n--;
						}
						if(n)
						{//��
							break;
						}
					}
					if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					{
						UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
					}
					UARTCtrl->NEXTOADSubNum=m;//DL698��֡����OAD���Ӽ���
					p8tx[1]=Array;//����������
					return Len;

				case 20:
					//���� 20�������ģ���� =unsigned
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8s=UART_3762_AFNxFn(0x10,9);//��ѯ��������Ϣ,����ز�Ƶ����ŵ�(�Զ���ͣ�ͻָ�·��);���:p8Buff=���ݷ��û���
					p8tx[0]=DataType_long_unsigned;
					p8tx[1]=p8s[1];
					p8tx[2]=p8s[0];
					Len=3;
					return Len;
					
				case 22:
					//�ܾ��ڵ��ϱ�ʹ�ܱ�־
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8tx[0]=DataType_enum;
					i=MRR(ADDR_F209_22+1,1);
					p8tx[1]=(i&0x01);
					Len=2;
					return Len;
			#endif
				default:
					UARTCtrl->DAR=6;//���󲻴���
					return 0;
			}
			//return 0;
			
		default:
			UARTCtrl->DAR=6;//���󲻴���
			return 0;
	}
}


#if (USER/100)==9//����
#if (((Project/100)==2)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//����
//����2�����ߴ�����Ϣ���= array�����ߴ�����Ϣ
//�����ߴ�����Ϣ��=structure
//{
//̨���ڵ����� unsigned��
//�ڵ��ַ octet-string��
//�ڵ���Ϣ bit-string(SIZE(8))��
//�ڵ�̨����ַ octet-string
//}
//�ڵ���Ϣ��b0Ϊ������b1-2Ϊ̨��״̬��b3-5Ϊ����b6Ϊ��·�쳣��b7Ϊ����
//a)	������0��ʾ���Ǽ������·��ı�����1��ʾ�Ǽ������·��ı�����
//b)	̨��״̬��0��ʾ�ڵ�̨����ַ��ȷ����1��ʾ�ڵ�̨����ַ�ͱ���������ͬ�����ڱ�̨����2��ʾ�ڵ�̨����ַ�ͱ���������ַ����ͬ��
//c)	���򣺽ڵ����ڵ�Դ�����0��ʾ��ȷ������1���α�ʾ��1��2��3�ࣨ001����A�ࣻ010����B�ࣻ100����C�ࣻ111�������ࣩ��
//d)	��·�쳣���ӽڵ���·�쳣��ǣ��á�1��ʱ����ʾ�ӽڵ���·���쳣�������ܱ�Ϊ���๩��ʱ����·�쳣��ʾ����߷��ӣ������ܱ�Ϊ���๩��ʱ����·�쳣��ʾ������
//e)	�ڵ�̨����ַ���ڵ��ַ������̨����ַ��������̨����ַ������ȷ��ȫ��F��
u32 GET_Request_FF10(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 Len;
	u8* p8s;
//	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 Array;
	u32 NUMmax;
	u32 n;
	u32 m;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	if(LENmax_TxSPACE<2)
	{
		GET_Request_Frame(PORTn);//��֡��־
		return 0;
	}
	
	i=0;
	n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
	p8s=(u8*)ADDR_DATABUFF;
	//�ڵ���ʼ��ţ����б�������֡��0��ʼ
	while(i<n)
	{//���Ǵ�0��ʼ
		m=n-i;
		if(m>15)
		{
			m=15;
		}
		UART_3762_AFN03F101(PORTn,i,m,(u8*)ADDR_DATABUFF);//(����)��ѯ�ڵ�̨����Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���ʼ�����֡��0��ʼ,Num=�ڵ�����(���֧��15��),p8Buff=���ݷ��û���
		NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
		if(n>=NUMmax)
		{
			break;
		}
		m=p8s[2];//����Ӧ��Ľڵ�����
		if(m==0)
		{
			break;
		}
		i+=m;
	}
	
	p8tx[0]=DataType_array;
	p8tx[1]=0;
	Len=2;
	Array=0;//������������
	while(1)
	{
		UART_3762_AFN03F101(PORTn,n,15,(u8*)ADDR_DATABUFF);//(����)��ѯ�ڵ�̨����Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���ʼ�����֡��0��ʼ,Num=�ڵ�����(���֧��15��),p8Buff=���ݷ��û���
		p8s=(u8*)ADDR_DATABUFF;
//���У�
	//̨���ڵ�������	BIN	2
	//�����ϱ��ڵ�����n(n<=15)	BIN	1
		
	//�ڵ�1��ַ	BCD	6
	// b7   	 b6	    b5-b3	  b2-b1	     b0	     BS	1
	//����	��·�쳣	  ����	 ̨��״̬	  ����		
	//�ڵ�1̨����ַ	BCD	6
	//��	��	��
	//a)	������0��ʾ���Ǽ������·��ı�����1��ʾ�Ǽ������·��ı�����
	//b)	̨��״̬��0��ʾ�ڵ�̨����ַ��ȷ����1��ʾ�ڵ�̨����ַ�ͱ���������ͬ�����ڱ�̨����2��ʾ�ڵ�̨����ַ�ͱ���������ַ����ͬ��
	//c)	���򣺽ڵ����ڵ�Դ�����0��ʾ��ȷ������1���α�ʾ��1��2��3�ࣨ001����A�ࣻ010����B�ࣻ100�ñ�C�ࣻ111�������ࣩ��
	//d)	��·�쳣���ӽڵ���·�쳣��ǣ��á�1��ʱ����ʾ�ӽڵ���·���쳣�������ܱ�Ϊ���๩��ʱ����·�쳣��ʾ����߷��ӣ������ܱ�Ϊ���๩��ʱ����·�쳣��ʾ������
	//e)	�ڵ�̨����ַ���ڵ��ַ������̨����ַ��������̨����ַ������ȷ��ȫ��F��
		NUMmax=p8s[0]|(p8s[1]<<8);//�ڵ�������
		m=p8s[2];//����Ӧ��Ľڵ�����
		if(m==0)
		{
			break;
		}
		p8s+=3;
		while(m)
		{
			if(n>=NUMmax)
			{
				break;
			}
			if((Array>=127)||(LENmax_TxSPACE<24))//ÿ�ڵ��ֽ���=
			{//���ͻ��岻��,��֡
				if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
				{
					UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
				}
				else
				{
					UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
				}
				UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
				p8tx[1]=Array;//������������
				//·�ɻָ�
				Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
				Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				return Len;
			}
			
		//�����ߴ�����Ϣ��=structure
			p8tx[Len+0]=DataType_structure;
			p8tx[Len+1]=4;
		//{
		//̨���ڵ����� unsigned��
			p8tx[Len+2]=DataType_long_unsigned;
			p8tx[Len+3]=NUMmax>>8;
			p8tx[Len+4]=NUMmax;
		//�ڵ��ַ octet-string��
			Data_Inverse((u32)p8s,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			p8tx[Len+5]=DataType_octet_string;
			p8tx[Len+6]=6;
			MR((u32)p8tx+Len+7,(u32)p8s,6);
		//�ڵ���Ϣ bit-string(SIZE(8))��
	// b7   	 b6	    b5-b3	  b2-b1	     b0	     BS	1
	//����	��·�쳣	  ����	 ̨��״̬	  ����		
			x=p8s[6];
			i=Bit_Inverse(x,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
//			i&=0x83;
//			i|=(x&0x06)<<4;
//			i|=(x&0x38)>>1;
			p8tx[Len+13]=DataType_bit_string;
			p8tx[Len+14]=8;
			p8tx[Len+15]=i;
		//�ڵ�̨����ַ octet-string
			Data_Inverse((u32)p8s+7,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			p8tx[Len+16]=DataType_octet_string;
			p8tx[Len+17]=6;
			MR((u32)p8tx+Len+18,(u32)p8s+7,6);
		//}
			p8s+=13;
			Len+=24;
			LENmax_TxSPACE-=24;
			Array++;//������������
			n++;
			m--;
		}
		if(m)
		{//��
			break;
		}
	}
	if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	{
		UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	}
	UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
	p8tx[1]=Array;//����������
	//·�ɻָ�
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
	Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
	return Len;
	
}
#endif
#endif










