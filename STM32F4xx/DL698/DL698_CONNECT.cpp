
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_CONNECT.h"
#include "../Device/IC_TESAM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/MS.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"



//������Ӧ�ò�Э�����ݵ�Ԫ��Server-APDU��
//Server-APDU��=SEQUENCE
//{
//  Ӧ�ò����  CHOICE
//  {    
//    ����Ӧ��������Ӧ	[130]	CONNECT-Response��
//�Ͽ�Ӧ��������Ӧ 	[131]	RELEASE-Response��
//�Ͽ�Ӧ������֪ͨ  	[132]	RELEASE-Notification��
//    ��ȡ��Ӧ         	[133]	GET-Response��
//������Ӧ       	[134]	SET-Response��
//    ������Ӧ        	[135]	ACTION-Response��
//    �ϱ�֪ͨ        	[136]	REPORT-Notification��
//������Ӧ          	[137]	PROXY-Response
//}��
//  �����ϱ���Ϣ�� 	FollowReport  OPTIONAL��
//ʱ���ǩ		TimeTag	     OPTIONAL
//}


__align(4) const u8 FactoryVersion[]=//���������̰汾��Ϣ
{
	FactoryCode,//���̴���         visible-string(SIZE (4))��
	((SVER>>12)&0xf)+0x30,((SVER>>8)&0xf)+0x30,((SVER>>4)&0xf)+0x30,((SVER>>0)&0xf)+0x30,//����汾��      visible-string(SIZE (4))��
	((SVDATE>>20)&0xf)+0x30,((SVDATE>>16)&0xf)+0x30,((SVDATE>>12)&0xf)+0x30,((SVDATE>>8)&0xf)+0x30,((SVDATE>>4)&0xf)+0x30,((SVDATE>>0)&0xf)+0x30,//����汾����   visible-string(SIZE (6))��
	((HVER>>12)&0xf)+0x30,((HVER>>8)&0xf)+0x30,((HVER>>4)&0xf)+0x30,((HVER>>0)&0xf)+0x30,//Ӳ���汾��      visible-string(SIZE (4))��
	((HVDATE>>20)&0xf)+0x30,((HVDATE>>16)&0xf)+0x30,((HVDATE>>12)&0xf)+0x30,((HVDATE>>8)&0xf)+0x30,((HVDATE>>4)&0xf)+0x30,((HVDATE>>0)&0xf)+0x30,//Ӳ���汾����   visible-string(SIZE (6))��
	FactoryInfoExt,//������չ��Ϣ   visible-string(SIZE (8))
};


__align(4) const u8 ProtocolConformance[]=//�̶���Э��һ���Կ�
{
	(Protocol0<<7)+(Protocol1<<6)+(Protocol2<<5)+(Protocol3<<4)+(Protocol4<<3)+(Protocol5<<2)+(Protocol6<<1)+(Protocol7<<0),
	(Protocol8<<7)+(Protocol9<<6)+(Protocol10<<5)+(Protocol11<<4)+(Protocol12<<3)+(Protocol13<<2)+(Protocol14<<1)+(Protocol15<<0),
	(Protocol16<<7)+(Protocol17<<6)+(Protocol18<<5)+(Protocol19<<4)+(Protocol20<<3)+(Protocol21<<2)+(Protocol22<<1)+(Protocol23<<0),
	(Protocol24<<7)+(Protocol25<<6)+(Protocol26<<5)+(Protocol27<<4)+(Protocol28<<3)+(Protocol29<<2)+(Protocol30<<1)+(Protocol31<<0),
	(Protocol32<<7)+(Protocol33<<6)+(Protocol34<<5)+(Protocol35<<4)+(Protocol36<<3)+(Protocol37<<2)+(Protocol38<<1)+(Protocol39<<0),
	0,
	0,
	0,
};

__align(4) const u8 FunctionConformance[]=//�̶��Ĺ���һ���Կ�
{
	(Function0<<7)+(Function1<<6)+(Function2<<5)+(Function3<<4)+(Function4<<3)+(Function5<<2)+(Function6<<1)+(Function7<<0),
	(Function8<<7)+(Function9<<6)+(Function10<<5)+(Function11<<4)+(Function12<<3)+(Function13<<2)+(Function14<<1)+(Function15<<0),
	(Function16<<7)+(Function17<<6)+(Function18<<5)+(Function19<<4)+(Function20<<3)+(Function21<<2)+(Function22<<1)+(Function23<<0),
	(Function24<<7)+(Function25<<6)+(Function26<<5)+(Function27<<4)+(Function28<<3)+(Function29<<2)+(Function30<<1)+(Function31<<0),
	(Function32<<7)+(Function33<<6)+(Function34<<5)+(Function35<<4)+(Function36<<3)+(Function37<<2)+(Function38<<1)+(Function39<<0),
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};


void CONNECT_Request(u32 PORTn)//����
{
	u32 i;
	u32 LEN_SA;//��������ַSA�ĳ���
	u32 TI;//ʱ���ǩ:0=��,1=��
	u32 LEN_RxAPDU;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//��󻹿ɷ��͵��ֽ���
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	ms_Type *ms;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//�ͻ���������Ϣ
	ServerConnectInfo_TypeDef *ServerConnectInfo;//�����������Ϣ
	u32 Lc;
	u8 *p8esamrx;
	
	p8esamrx=(u8 *)(ADDR_UART7Rx);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
//68 17 00 43  05  01 00 00 00 00 00  10  26 F6 
//05   Ӧ�ò����  CHOICE
//01   GET-Request��=CHOICE
//00   PIID
//40 01 02 00    OAD
//00   TI
//ED 03 16
//ʱ���ǩ
	i=p8rx[LEN_RxAPDU-2];
	if(i)
	{//��ʱ���ǩ
		//TimeTag��=SEQUENCE
		//{
		//  ����ʱ�� 		   	date_time_s��
		//  ��������ʱʱ�� 	TI
		//}
		TI=1;//ʱ���ǩ:0=��,1=��
		LENmax_TxSPACE-=8+5;//ʱ���ǩ:1=��
	}
	else
	{
		TI=0;//ʱ���ǩ:0=��,1=��
		LEN_RxAPDU-=1;
		LENmax_TxSPACE-=1;//ʱ���ǩ:0=��
	}
//
	LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	
	if(UARTCtrl->pTxData)//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
	{
		ms=Get_ms();
		ms->ms_free(UARTCtrl->pTxData);
		UARTCtrl->pTxData=0;
	}
	UARTCtrl->PIID=p8rx[LEN_SA+8+1];//PIID
//Connect-Request��=SEQUENCE
//{
//0  �������-���ȼ�           PIID��
//1  ������Ӧ�ò�Э��汾��    long-unsigned��
//3 	������Э��һ���Կ�        ProtocolConformance��
//11  �����Ĺ���һ���Կ�        FunctionConformance��
//27  �ͻ�������֡���ߴ�      long-unsigned��
//29  �ͻ�������֡���ߴ�      long-unsigned��
//31  �ͻ�������֡��󴰿ڳߴ�  unsigned��
//32  �ͻ������ɴ���APDU�ߴ� long-unsigned��
//34  ������Ӧ�����ӳ�ʱʱ��    double-long-unsigned��
//38  ��֤�������              ConnectMechanismInfo
	//ConnectMechanismInfo��=CHOICE
	//{
	//  ��������    [0] NullSecurity��
	//  һ������    [1] PasswordSecurity��
	//  �ԳƼ���    [2] SymmetrySecurity��
	//  ����ǩ��    [3] SignatureSecurity
	//}
//}
	MR((u32)&UARTCtrl->ConnectInfo,(u32)p8rx+LEN_SA+8+2,sizeof(ClientConnectInfo_TypeDef));
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
//CONNECT-Response��=SEQUENCE
//{
//  �������-���ȼ�-ACD       PIID-ACD��
//  ���������̰汾��Ϣ        FactoryVersion��
	//FactoryVersion��=SEQUENCE
	//{
	//���̴���         visible-string(SIZE (4))��
	//����汾��      visible-string(SIZE (4))��
	//����汾����   visible-string(SIZE (6))��
	//Ӳ���汾��      visible-string(SIZE (4))��
	//Ӳ���汾����   visible-string(SIZE (6))��
	//������չ��Ϣ   visible-string(SIZE (8))
	//}
//  �̶���Ӧ�ò�Э��汾��    long-unsigned��
//  �̶���Э��һ���Կ�        ProtocolConformance��
//  �̶��Ĺ���һ���Կ�        FunctionConformance��
//  ����������֡���ߴ�      long-unsigned��
//  ����������֡���ߴ�      long-unsigned��
//  ����������֡��󴰿ڳߴ�  unsigned��
//  ���������ɴ���APDU�ߴ� long-unsigned��
//  �̶���Ӧ�����ӳ�ʱʱ��    double-long-unsigned��
//  ������Ӧ����              ConnectResponseInfo
//}
	p8tx[0]=130;//����Ӧ��������Ӧ	[130]	CONNECT-Response��
	p8tx[1]=p8rx[LEN_SA+8+1];//PIID-ACD
	//���������̰汾��Ϣ
	MR((u32)p8tx+2,(u32)FactoryVersion,32);
	//�̶���Ӧ�ò�Э��汾��
	ServerConnectInfo=(ServerConnectInfo_TypeDef *)(p8tx+2);
	ServerConnectInfo->APPVER[0]=ClientConnectInfo->APPVER[0];
	ServerConnectInfo->APPVER[1]=ClientConnectInfo->APPVER[1];
	//�̶���Э��һ���Կ�
	for(i=0;i<8;i++)
	{
		ServerConnectInfo->Protocol[i]=ProtocolConformance[i]&ClientConnectInfo->Protocol[i];
	}
	//�̶��Ĺ���һ���Կ�
	for(i=0;i<16;i++)
	{
		ServerConnectInfo->Function[i]=FunctionConformance[i]&ClientConnectInfo->Function[i];
	}
	//����������֡���ߴ�
	i=Get_LEN_UARTnTx(PORTn);
	i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	ServerConnectInfo->Len_Tx[0]=i;
	ServerConnectInfo->Len_Tx[1]=i>>8;
	//����������֡���ߴ�
	i=Get_LEN_UARTnRx(PORTn);
	i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	ServerConnectInfo->Len_Rx[0]=i;
	ServerConnectInfo->Len_Rx[1]=i>>8;
	//����������֡��󴰿ڳߴ�
	i=Get_LEN_UARTnIntRx(PORTn);//�õ�UART�жϽ��ջ��峤��
	i-=10;
	i/=ServerConnectInfo->Len_Rx[0]|(ServerConnectInfo->Len_Rx[1]<<8);
	ServerConnectInfo->NUM_RxFrame=i;
	//���������ɴ���APDU�ߴ�
	ServerConnectInfo->Len_APDU[0]=ServerConnectInfo->Len_Rx[0];
	ServerConnectInfo->Len_APDU[1]=ServerConnectInfo->Len_Rx[1];
	//�̶���Ӧ�����ӳ�ʱʱ��
	ServerConnectInfo->ConnectOVERTime[0]=ClientConnectInfo->ConnectOVERTime[0];
	ServerConnectInfo->ConnectOVERTime[1]=ClientConnectInfo->ConnectOVERTime[1];
	ServerConnectInfo->ConnectOVERTime[2]=ClientConnectInfo->ConnectOVERTime[2];
	ServerConnectInfo->ConnectOVERTime[3]=ClientConnectInfo->ConnectOVERTime[3];
	//������Ӧ����
//ConnectResponseInfo��=SEQUENCE
//{
//  ��֤���      ConnectResult��
//  ��֤������Ϣ  SecurityData  OPTIONAL
//}
//ConnectResult��=ENUMERATED
//{
//  ������Ӧ������     ��0����
//�������             ��1����
//�Գƽ��ܴ���         ��2����
//�ǶԳƽ��ܴ���       ��3����
//ǩ������             ��4����
//Э��汾��ƥ��       ��5����
//��������             ��255��
//}

	ServerConnectInfo->ConnectResult=0;//��֤���:������Ӧ������
	p8tx[2+sizeof(ServerConnectInfo_TypeDef)]=0;//��֤������Ϣ  OPTIONAL=0 ��ʾû��
	LEN_TxAPDU=2+sizeof(ServerConnectInfo_TypeDef)+1;
	LENmax_TxSPACE-=2+sizeof(ServerConnectInfo_TypeDef)+1;
	
  i=p8rx[LEN_SA+8+1+38];
	switch(i)
	{
		default:
		case 0://��������    [0] NullSecurity
			UARTCtrl->CONNECT=100;
			UARTCtrl->OAD_44000400[0]=DataType_unsigned;//��ǰ���ӵĿͻ�����ַ
			UARTCtrl->OAD_44000400[1]=p8rx[LEN_SA+5];
			break;
		case 1://һ������    [1] PasswordSecurity
//PasswordSecurity��=visible-string
			UARTCtrl->CONNECT=100;//���ӱ�־
			UARTCtrl->OAD_44000400[0]=DataType_unsigned;//��ǰ���ӵĿͻ�����ַ
			UARTCtrl->OAD_44000400[1]=p8rx[LEN_SA+5];
			
			MR(ADDR_DATABUFF,ADDR_4401+1,LENmax_4401-1);
			i=Compare_octet_string(p8rx+LEN_SA+8+1+39,(u8 *)ADDR_DATABUFF);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i)
			{
				UARTCtrl->CONNECT=0;
				UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
				ServerConnectInfo->ConnectResult=1;//��֤���:�������
				break;
			}
			break;
		case 2://�ԳƼ���    [2] SymmetrySecurity
//SymmetrySecurity��=SEQUENCE
//{
//  ����1        octet-string��
//  �ͻ���ǩ��1  octet-string
//}
//��������
//68 5E 00 43 05 11 11 11 11 11 11 10 54 12 
//02 ����Ӧ����������	 [2]		CONNECT-Request
//0D 00 16 FF FF FF FF C0 00 00 00 FF FE C4 00 00 00 00 00 00 00 00 00 00 00 00 00 02 00 04 00 01 07 D0 00 00 1C 20 
//02 ������֤�Ļ�����Ϣ 2=�ԳƼ���
//20 02 88 B7 02 ED E9 F3 F4 42 B8 38 F5 54 8B 14 90 9F BE 92 F4 72 B1 B0 09 4B 0A 0E B9 D7 B6 A9 35 ����1
//04 33 16 83 7B �ͻ���ǩ��1
//00 
//59 6B 16
//��������
//68 90 00 C3 05 11 11 11 11 11 11 10 40 C0 82 0D 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 00 00 FF FF FF FF C0 00 00 00 FF FE C4 00 00 00 00 00 00 00 00 00 00 00 00 00 02 00 02 00 01 07 D0 00 00 1C 20 00 01 30 7D 13 AB 42 72 E7 76 63 8D 79 1A 59 7B 1E 5D BE CB 93 8F 6D 9A 2B 0C 90 04 FE E7 CA 3F C8 73 0D 3E DC 43 78 3B E5 CF F8 DC E6 CA EB DB A0 F2 97 04 2F 71 8A 84 00 00 CC CC 16

		case 3://����ǩ��    [3] SignatureSecurity
//SignatureSecurity��=SEQUENCE
//{
//  ����2        octet-string��
//  �ͻ���ǩ��2  octet-string
//}
			UARTCtrl->CONNECT=100;//���ӱ�־
			UARTCtrl->OAD_44000400[0]=DataType_unsigned;//��ǰ���ӵĿͻ�����ַ
			UARTCtrl->OAD_44000400[1]=p8rx[LEN_SA+5];
		
			p8tx[2+sizeof(ServerConnectInfo_TypeDef)]=1;//��֤������Ϣ  OPTIONAL=0 ��ʾû��
			p8rx+=LEN_SA+8+1+39;
			i=p8rx[0];//  ����2        octet-string
			if(i&0x80)
			{
				if((i&0x7f)==1)
				{
					Lc=p8rx[1];
					if(Lc>(LEN_UART7Tx-7))
					{
						UARTCtrl->CONNECT=0;
						UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
						ServerConnectInfo->ConnectResult=255;//��֤���:��������
						break;
					}
					MR(ADDR_UART7Tx+7,(u32)p8rx+2,Lc);
					p8rx+=2+Lc;
				}
				else
				{
					Lc=(p8rx[1]<<8)+p8rx[2];
					if(Lc>(LEN_UART7Tx-7))
					{
						UARTCtrl->CONNECT=0;
						UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
						ServerConnectInfo->ConnectResult=255;//��֤���:��������
						break;
					}
					MR(ADDR_UART7Tx+7,(u32)p8rx+3,Lc);
					p8rx+=3+Lc;
				}
			}
			else
			{
				Lc=i;
				MR(ADDR_UART7Tx+7,(u32)p8rx+1,Lc);
				p8rx+=1+Lc;
			}
			i=p8rx[0];//  �ͻ���ǩ��2  octet-string
			if(i&0x80)
			{
				if((i&0x7f)==1)
				{
					i=p8rx[1];
					if((Lc+i)>(LEN_UART7Tx-7))
					{
						UARTCtrl->CONNECT=0;
						UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
						ServerConnectInfo->ConnectResult=255;//��֤���:��������
						break;
					}
					MR(ADDR_UART7Tx+7+Lc,(u32)p8rx+2,i);
					Lc+=i;
				}
				else
				{
					i=(p8rx[1]<<8)+p8rx[2];
					if((Lc+i)>(LEN_UART7Tx-7))
					{
						UARTCtrl->CONNECT=0;
						UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
						ServerConnectInfo->ConnectResult=255;//��֤���:��������
						break;
					}
					MR(ADDR_UART7Tx+7+Lc,(u32)p8rx+3,i);
					Lc+=i;
				}
			}
			else
			{
				MR(ADDR_UART7Tx+7+Lc,(u32)p8rx+1,i);
				Lc+=i;
			}
			i=TESAM_CMD_TxRx(0x80020000,Lc,ADDR_UART7Tx+7);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{
				UARTCtrl->CONNECT=0;
				UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
				ServerConnectInfo->ConnectResult=4;//��֤���:ǩ������4��
				break;
			}
//SecurityData��=SEQUENCE
//{
//  �����������    RN��
//  ������ǩ����Ϣ  octet-string
//}
			i=(p8esamrx[3]<<8)+p8esamrx[4];//�������ݳ���
			if(i<48)
			{
				UARTCtrl->CONNECT=0;
				UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
				ServerConnectInfo->ConnectResult=4;//��֤���:ǩ������4��
				break;
			}
			//�����������    RN
			p8tx[LEN_TxAPDU]=48;
			MR((u32)p8tx+LEN_TxAPDU+1,(u32)p8esamrx+5,48);
			LEN_TxAPDU+=1+48;
			i-=48;
			if(i>127)
			{
				UARTCtrl->CONNECT=0;
				UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
				ServerConnectInfo->ConnectResult=4;//��֤���:ǩ������4��
				break;
			}
			//������ǩ����Ϣ  octet-string
			p8tx[LEN_TxAPDU]=i;
			MR((u32)p8tx+LEN_TxAPDU+1,(u32)p8esamrx+5+48,i);
			LEN_TxAPDU+=1+i;
			break;
	}
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
}


void RELEASE_Request(u32 PORTn)//�Ͽ�
{
//	u32 i;
	u32 LEN_SA;//��������ַSA�ĳ���
	u32 TI;//ʱ���ǩ:0=��,1=��
	u32 LEN_RxAPDU;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//��󻹿ɷ��͵��ֽ���
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	ms_Type *ms;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//�ͻ���������Ϣ

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
//68 17 00 43  05  01 00 00 00 00 00  10  26 F6 
//05   Ӧ�ò����  CHOICE
//01   GET-Request��=CHOICE
//00   PIID
//40 01 02 00    OAD
//00   TI
//ED 03 16
//
	LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	
	if(UARTCtrl->pTxData)//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
	{
		ms=Get_ms();
		ms->ms_free(UARTCtrl->pTxData);
		UARTCtrl->pTxData=0;
	}
	UARTCtrl->PIID=p8rx[LEN_SA+8+1];//PIID
//RELEASE-Response��=SEQUENCE
//{
//  �������-���ȼ�-ACD  PIID-ACD��
//  ���  ENUMERATED
//{
//�ɹ�  ��0��
//}
//}
	UARTCtrl->CONNECT=0;//���ӱ�־
	UARTCtrl->OAD_44000400[0]=0;//��ǰ���ӵĿͻ�����ַ
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
	ClientConnectInfo->Len_Rx[0]=0;//�ͻ������ջ��峤��ΪĬ��ֵ
	ClientConnectInfo->Len_Rx[1]=0;
	
	p8tx[0]=131;//�Ͽ�Ӧ��������Ӧ 	[131]	RELEASE-Response��
	p8tx[1]=p8rx[LEN_SA+8+1];//PIID-ACD
	p8tx[2]=0;//�ɹ�  ��0��
	LEN_TxAPDU=3;
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
}

void RELEASE_Notification(u32 PORTn)//�Ͽ�֪ͨ
{

//�Ͽ�Ӧ������֪ͨ  	[132]	RELEASE-Notification��
	
	
}




















