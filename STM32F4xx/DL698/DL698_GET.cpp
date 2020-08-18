

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_TESAM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_DATALIB.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET_iMeter.h"
#include "../DL698/DL698_GET_4100.h"
#include "../DL698/DL698_GET_6000.h"
#include "../DL698/DL698_GET_F000.h"
#include "../DL698/DL698_GET_EVENT.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../Calculate/Calculate.h"

#include "../DL698/CALC_SwitchIn.h"
#include "../DL698/DL698_FILE.h"
#include "../DL698/DL698_GET_SET_ACTION_2000.h"
#include "../DL698/DL698_CONNECT.h"
#include "../DL698/DL698_FactoryTest.h"

#include "../DL698/DL698_JSON.h"

//GET-Request��=CHOICE
//{
//��ȡһ��������������             [1] GetRequestNormal��
//��ȡ���ɸ�������������           [2] GetRequestNormalList��
//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord��
//��ȡ���ɸ���¼�Ͷ�����������	  [4] GetRequestRecordList��
//��ȡ��֡��Ӧ����һ�����ݿ�����	  [5] GetRequestNext
//}
//GET-Response��=CHOICE
//{
//��ȡһ���������Ե���Ӧ         	[1]  GetResponseNormal��
//��ȡ���ɸ��������Ե���Ӧ       	[2]  GetResponseNormalList��
//��ȡһ����¼�Ͷ������Ե���Ӧ   	[3]  GetResponseRecord��
//��ȡ���ɸ���¼�Ͷ������Ե���Ӧ 	[4]  GetResponseRecordList��
//��֡��Ӧһ�����ݿ�                	[5]  GetResponseNext
//}

void GET_Request(u32 PORTn)
{
	u32 i;
	u32 x;
	u32 LEN_SA;//��������ַSA�ĳ���
	u32 TI;//ʱ���ǩ:0=��,1=��
	u32 LEN_RxAPDU;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//��󻹿ɷ��͵��ֽ���
	u32 LEN_OAD;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	u8* pDATA;
	u32 OAD;
	ms_Type *ms;
	u16 *p16;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//�ͻ���������Ϣ

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
#if USE_ClientConnectInfo==0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��
	ClientConnectInfo=ClientConnectInfo;
#else
	//�ͻ������ջ��峤��
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//��Ϊ0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
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

	if(p8rx[LEN_SA+8+1]!=5)
	{//���Ƕ�ȡ��֡��Ӧ����һ�����ݿ�����	  [5] GetRequestNext
		UARTCtrl->GET_Reques_CHOICE=p8rx[LEN_SA+8+1];//ԭ�õ�����ѡ��
		if(UARTCtrl->pTxData)//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
		}
	}
	UARTCtrl->PIID=p8rx[LEN_SA+8+2];//PIID-ACD
	i=p8rx[LEN_SA+8+1];
	switch(i)
	{
		case 1://��ȡһ��������������             [1] GetRequestNormal��
//GetRequestNormal��=SEQUENCE
//{
//  �������-���ȼ�     PIID��
//  һ����������������  OAD
//}
//GetResponseNormal��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  һ���������Լ�����  A-ResultNormal
//}
//A-ResultNormal��=SEQUENCE
//{
//  ��������������  OAD��
//  ������        Get-Result
//}
//Get-Result��=CHOICE
//{
//  ������Ϣ  [0]   DAR��
//  ����      [1]   Data
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
			p8tx[1]=1;//[1] GetRequestNormal
			p8tx[2]=UARTCtrl->PIID;//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			p8rx+=LEN_SA+8+3;
			//ʱ���ǩ
			if(p8rx[4]==1)
			{//��ʱ���ǩ
				i=Uart_TimeTag(p8rx+4+1);//ʱ���ǩ��Ч���ж�;���:p8rxָ��ʱ���ǩ;����:0=��Ч,1=��Ч
				if(i)
				{//��Ч
					p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
					p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
					p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
					p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
					p8tx[LEN_TxAPDU+4]=0;//Get-Result��=CHOICE,������Ϣ  [0]   DAR
					p8tx[LEN_TxAPDU+5]=32;//ʱ���ǩ��Ч(32)
					LEN_TxAPDU+=6;
					break;
				}
			}
			
			UARTCtrl->NEXT=0;//DL698��֡��־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡
			UARTCtrl->NEXTBlock=1;//DL698��֡������̵�֡��ţ�ȡֵ��Χ0~4095
			UARTCtrl->NEXTOADNum=0;//DL698��֡���贫���OAD����
			UARTCtrl->pRxBuff=(u32)p8rx;
			UARTCtrl->NEXTOADSubNum=0;//DL698��֡����OAD���Ӽ���
			
			OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
			p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
			p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
			p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
			p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
			LEN_TxAPDU+=4;
			LENmax_TxSPACE-=4;
			LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+1,LENmax_TxSPACE-1);
			if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
			{
				p8tx[LEN_TxAPDU+0]=0;//Get-Result��=CHOICE,������Ϣ  [0]   DAR
				p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
				LEN_TxAPDU+=2;
			}
			else
			{
				p8tx[LEN_TxAPDU]=1;//Get-Result��=CHOICE, [1]   DATA
				LEN_TxAPDU+=LEN_OAD+1;
			}
			break;
		case 2://��ȡ���ɸ�������������           [2] GetRequestNormalList��
//GetRequestNormalList��=SEQUENCE
//{
//  �������-���ȼ�       PIID��
//  ���ɸ���������������  SEQUENCE OF OAD
//}
//GetResponseNormalList��=SEQUENCE
//{
//  �������-���ȼ�-ACD     PIID-ACD��
//  ���ɸ��������Լ�����  SEQUENCE OF A-ResultNormal
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
			p8tx[1]=2;//��ȡ���ɸ�����������Ӧ   	[2] 
			p8tx[2]=UARTCtrl->PIID;//PIID-ACD
			p8tx[3]=0;//SEQUENCE OF
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=LEN_TxAPDU;
		
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			p8rx+=LEN_SA+8+4;
			while(x--)
			{
				if(LENmax_TxSPACE<6)
				{
					GET_Request_Frame(PORTn);//��֡��־
					break;//���ͻ��岻��
				}
				UARTCtrl->NEXT=0;//DL698��֡��־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡
				UARTCtrl->NEXTBlock=1;//DL698��֡������̵�֡��ţ�ȡֵ��Χ0~4095
				UARTCtrl->NEXTOADNum=x;//DL698��֡���贫���OAD����
				UARTCtrl->pRxBuff=(u32)p8rx;
				UARTCtrl->NEXTOADSubNum=0;//DL698��֡����OAD���Ӽ���
				
				OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
				LENmax_TxSPACE-=4;
				LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+4+1,LENmax_TxSPACE-1);
				if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
				{
					break;
				}
				LEN_TxAPDU+=4;
				if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
				{
					p8tx[LEN_TxAPDU+0]=0;//Get-Result��=CHOICE,������Ϣ  [0]   DAR
					p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
					LEN_TxAPDU+=2;
					LENmax_TxSPACE-=2;
				}
				else
				{
					p8tx[LEN_TxAPDU]=1;//Get-Result��=CHOICE, [1]   DATA
					LEN_TxAPDU+=LEN_OAD+1;
					LENmax_TxSPACE-=LEN_OAD+1;
				}
				p8rx+=4;
				p8tx[3]++;//+1(SEQUENCE OF)
			}
			break;
		case 3://��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord��
//GetRequestRecord��=SEQUENCE
//{
//  �������-���ȼ�         PIID��
//  ��ȡһ����¼�Ͷ�������  GetRecord
//}
//GetRecord��=SEQUENCE
//{
//  ��������������     OAD��
//  ��¼ѡ��������     RSD��
//  ��¼��ѡ��������   RCSD
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8rx+=LEN_SA+8+3;
			LEN_OAD=GET_RecordOAD(PORTn,p8rx);//��һ����¼�Ͷ�������;����:b31-b16����ROAD�������ֽ���,b15-b0����ROAD�������ֽ���,���ݷ��ʽ��UARTCtrl->DAR
			p8rx+=LEN_OAD>>16;
			LEN_TxAPDU=LEN_OAD&0xffff;
			if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
			{
				p8tx[LEN_TxAPDU+0]=0;//Get-Result��=CHOICE,������Ϣ  [0]   DAR
				p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
				LEN_TxAPDU+=2;
				LENmax_TxSPACE-=2;
			}
			break;
		case 4://��ȡ���ɸ���¼�Ͷ�����������	  [4] GetRequestRecordList��
//GetRequestRecordList��=SEQUENCE
//{
//  �������-���ȼ�           PIID��
//  ��ȡ���ɸ���¼�Ͷ�������  SEQUENCE OF GetRecord
//}
//GetResponseRecordList��=SEQUENCE
//{
//  �������-���ȼ�-ACD           PIID-ACD��
//  ���ɸ���¼�Ͷ������Լ�����  SEQUENCE OF A-ResultRecord
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			p8rx+=LEN_SA+8+4;
			ms=Get_ms();
			LEN_TxAPDU=0;
			pDATA=0;
			for(i=0;i<x;i++)
			{
				LEN_OAD=GET_RecordOAD(PORTn,p8rx);//��һ����¼�Ͷ�������;����:b31-b16����ROAD�������ֽ���,b15-b0����ROAD�������ֽ���,���ݷ��ʽ��UARTCtrl->DAR
				p8rx+=LEN_OAD>>16;
				TI=LEN_OAD&0xffff;
//				if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
//				{
//					p8tx[LEN_TxAPDU+0]=0;//Get-Result��=CHOICE,������Ϣ  [0]   DAR
//					p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
//					LEN_TxAPDU+=2;
//					LENmax_TxSPACE-=2;
//				}
				if(pDATA==0)
				{
					pDATA=(u8*)ms->ms_malloc(TI);
				}
				else
				{
					pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU+TI);
				}
				if(UARTCtrl->pTxData)
				{
					ms->ms_free(UARTCtrl->pTxData);
					ms->ms_free(pDATA);
					return;
				}
				if(pDATA==0)
				{
					return;
				}
				if(TI>=3)
				{
					TI-=3;
					MR((u32)pDATA+LEN_TxAPDU,(u32)p8tx+3,TI);//ȥ��ʼ3byte
					LEN_TxAPDU+=TI;//�ܷ������ݳ���
				}
			}
		//�ͻ������ջ��峤��
			ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
			LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
			LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
#if USE_ClientConnectInfo==0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��
	ClientConnectInfo=ClientConnectInfo;
#else
	//�ͻ������ջ��峤��
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//��Ϊ0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
			
			if((LEN_TxAPDU+4)>LENmax_TxSPACE)
			{
				return;
			}
			p8tx[0]=0x85;
			p8tx[1]=4;
			p8tx[2]=UARTCtrl->PIID;
			p8tx[3]=x;//SEQUENCE OF
			MR((u32)p8tx+4,(u32)pDATA,LEN_TxAPDU);
			ms->ms_free(pDATA);
			LEN_TxAPDU+=4;
			
//	UARTCtrl->pTxData=0;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
//	UARTCtrl->LEN_TxData=0;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
//	UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
			break;
		case 5://��ȡ��֡��Ӧ����һ�����ݿ�����	  [5] GetRequestNext
//GetRequestNext��=SEQUENCE
//{
//  �������-���ȼ�                PIID��
//  ��ȷ���յ����һ�����ݿ����   long-unsigned
//}
//GetResponseNext��=SEQUENCE
//{
//  �������-���ȼ�-ACD  	PIID-ACD��
//  ĩ֡��־           	BOOLEAN��
//��֡���           	long-unsigned��
//��֡��Ӧ             	CHOICE
//{
//������Ϣ    	    	[0]   DAR��
//  ��������         	[1]   SEQUENCE OF A-ResultNormal��
//  ��¼�Ͷ�������    	[2]   SEQUENCE OF A-ResultRecord
//}
//}
			p16=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
			p16[0]=TimerS_UARTRAMFREE;
		//68 15 00 43 05 01 00 00 00 00 00 10 04 5D 05 05 0F 00 02 00 81 1A 16
			p8rx+=LEN_SA+8+3;
			i=(p8rx[0]<<8)+p8rx[1];//��ȷ���յ����һ�����ݿ����
			x=UARTCtrl->NEXTBlock;
			UARTCtrl->NEXTBlock=i+1;
			if(i!=x)
			{//��Ŵ�
				UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
				p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
				p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
				p8tx[1]=5;//[[5] GetRequestNext
				p8tx[2]=UARTCtrl->PIID;//PIID-ACD
				p8tx[3]=1;//δ֡��־
				p8tx[4]=UARTCtrl->NEXTBlock>>8;//��ȷ���յ����һ�����ݿ����
				p8tx[5]=UARTCtrl->NEXTBlock;
				p8tx[6]=0;//��֡��Ӧ             	CHOICE
				p8tx[7]=255;//��������:255
				LEN_TxAPDU=8;
				break;
			}
			//copy rx+Rxoffset��rx
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			x=p8rx[Rxoffset+1]+(p8rx[Rxoffset+2]<<8);//ԭ����֡�ܳ���
			x+=3;//������
			x>>=1;
			x<<=1;
			i=Get_LEN_UARTnRx(PORTn);
			if((Rxoffset+x)>i)
			{
				x=i-Rxoffset;
			}
			x/=2;
			p16=(u16*)p8rx;
			for(i=0;i<x;i++)
			{
				p16[i]=p16[(Rxoffset/2)+i];
			}
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			switch(UARTCtrl->GET_Reques_CHOICE)//ԭ�õ�����ѡ��
			{
				case 1://��ȡһ��������������
					p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
					p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
					p8tx[1]=5;//[1] GetRequestNormal,[5]  GetResponseNext
					p8tx[2]=UARTCtrl->PIID;//PIID-ACD
					LEN_TxAPDU=3;
					LENmax_TxSPACE-=LEN_TxAPDU;
					p8rx=(u8*)UARTCtrl->pRxBuff;
					OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
					p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
					p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
					p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
					p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
					LEN_TxAPDU+=4;
					LENmax_TxSPACE-=4;
					LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+1,LENmax_TxSPACE-1);
					if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
					{
						p8tx[LEN_TxAPDU+0]=0;//Get-Result��=CHOICE,������Ϣ  [0]   DAR
						p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
						LEN_TxAPDU+=2;
					}
					else
					{
						p8tx[LEN_TxAPDU]=1;//Get-Result
						LEN_TxAPDU+=LEN_OAD+1;
					}
					break;
				case 2://��ȡ���ɸ�������������
					p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
					p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
					p8tx[1]=5;//��ȡ���ɸ�����������Ӧ   	[2] ,[5]  GetResponseNext
					p8tx[2]=UARTCtrl->PIID;//PIID-ACD
					p8tx[3]=0;//SEQUENCE OF
					LEN_TxAPDU=4;
					LENmax_TxSPACE-=LEN_TxAPDU;
				
					x=UARTCtrl->NEXTOADNum;//x=p8rx[LEN_SA+8+3];//SEQUENCE OF
					p8rx=(u8*)UARTCtrl->pRxBuff;//p8rx+=LEN_SA+8+4;
					while(x--)
					{
						if(LENmax_TxSPACE<6)
						{
							GET_Request_Frame(PORTn);//��֡��־
							break;//���ͻ��岻��
						}
						//UARTCtrl->NEXT=0;//DL698��֡��־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡
						//UARTCtrl->NEXTBlock=1;//DL698��֡������̵�֡��ţ�ȡֵ��Χ0~4095
						UARTCtrl->NEXTOADNum=x;//DL698��֡���贫���OAD����
						UARTCtrl->pRxBuff=(u32)p8rx;
						UARTCtrl->NEXTOADSubNum=0;//DL698��֡����OAD���Ӽ���
						
						OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
						p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
						p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
						p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
						p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
						LEN_TxAPDU+=4;
						LENmax_TxSPACE-=4;
						LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+1,LENmax_TxSPACE-1);
						if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						{
							break;
						}
						if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
						{
							p8tx[LEN_TxAPDU+0]=0;//Get-Result��=CHOICE,������Ϣ  [0]   DAR
							p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
							LEN_TxAPDU+=2;
							LENmax_TxSPACE-=2;
						}
						else
						{
							p8tx[LEN_TxAPDU]=1;//����
							LEN_TxAPDU+=LEN_OAD+1;
							LENmax_TxSPACE-=LEN_OAD+1;
						}
						p8rx+=4;
						p8tx[3]++;//+1(SEQUENCE OF)
					}
					break;
				case 3://��ȡһ����¼�Ͷ�����������
					LEN_TxAPDU=GET_RecordOAD_Next(PORTn);//��һ����¼�Ͷ������Է�֡��Ӧ����һ֡;���أ�LEN_TxAPDU
					if(LEN_TxAPDU==0)
					{
						UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
						p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
						p8tx[1]=5;//[[5] GetRequestNext
						p8tx[2]=UARTCtrl->PIID;//PIID-ACD
						p8tx[3]=1;//δ֡��־
						p8tx[4]=UARTCtrl->NEXTBlock>>8;//��ȷ���յ����һ�����ݿ����
						p8tx[5]=UARTCtrl->NEXTBlock;
						p8tx[6]=0;//��֡��Ӧ             	CHOICE
						p8tx[7]=255;//��������:255
						LEN_TxAPDU=8;
					}
					break;
				case 4://��ȡ���ɸ���¼�Ͷ�����������
					LEN_TxAPDU=GET_RecordOAD_Next(PORTn);//��һ����¼�Ͷ������Է�֡��Ӧ����һ֡;���أ�LEN_TxAPDU
					if(LEN_TxAPDU==0)
					{//��һ����¼
						UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
						p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
						p8tx[1]=5;//[[5] GetRequestNext
						p8tx[2]=UARTCtrl->PIID;//PIID-ACD
						p8tx[3]=1;//δ֡��־
						p8tx[4]=UARTCtrl->NEXTBlock>>8;//��ȷ���յ����һ�����ݿ����
						p8tx[5]=UARTCtrl->NEXTBlock;
						p8tx[6]=0;//��֡��Ӧ             	CHOICE
						p8tx[7]=255;//��������:255
						LEN_TxAPDU=8;
					}
					break;
				default:
					return;
			}
			break;
		default:
			return;
	}
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
}


void GET_Request_Frame(u32 PORTn)//��֡��־
{
	UARTCtrl_TypeDef *UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	{
		UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	}
	else
	{
		UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	}
}

u32 GET_OAD_Element(u32 PORTn,u8* ADDR_OAD,u32 Element,u32 LENmax_OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ������Ԫ��;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 Len;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8=Get_Element(ADDR_OAD,Element,0,LENmax_OAD);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
	if(p8==0)
	{
		UARTCtrl->DAR=6;//���󲻴���
		return 0;
	}
	Len=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
	if(Len>LENmax_TxSPACE)
	{
		GET_Request_Frame(PORTn);//��֡��־
		return 0;
	}
	if(Len==0)
	{
		if(LENmax_TxSPACE<1)
		{
			GET_Request_Frame(PORTn);//��֡��־
			return 0;
		}
		p8tx[0]=0;//NULL
		return 1;
	}
	MR((u32)p8tx,(u32)p8,Len);
	return Len;
}



u32 GET_OAD_DATA(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
//  һ����������������   OAD
//OAD��=SEQUENCE
//{
//  �����ʶ          OI��
//  ���Ա�ʶ��������  bit-string��SIZE��8������
//  ������Ԫ������    unsigned��1��255��
//}
//�������Ա�ʶ��������������bit0��bit7��ʾ��λλ������λ�����λ�����У�
//1)	bit0��bit4�����ʾ�������Ա�ţ�ȡֵ0��31������0��ʾ�����������ԣ���������������ԣ�
//2)	bit5��bit7�����ʾ�������������������Ƕ���ͬһ�������ڲ�ͬ���ջ�����ȡֵģʽ��ȡֵ0��7�����������ھ�����������������
//������Ԫ����������00H��ʾ��������ȫ�����ݡ����������һ���ṹ�����飬01Hָ��������Եĵ�һ��Ԫ�أ����������һ����¼�͵Ĵ洢��������ֵn��ʾ�����n�εļ�¼��

//A-ResultNormal��=SEQUENCE
//{
//  ��������������  OAD��
//  ������        Get-Result
//}
//Get-Result��=CHOICE
//{
//  ������Ϣ  [0]   DAR��
//  ����      [1]   Data
//}
	u32 Len;
	DL698_SETLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�

	if((OAD&0xff00)==0x0100)
	{//���߼���
		if(LENmax_TxSPACE<4)
		{
			GET_Request_Frame(PORTn);//��֡��־
			return 0;
		}
		p8tx[0]=DataType_octet_string;//long-unsigned
		p8tx[1]=2;
		p8tx[2]=OAD>>24;
		p8tx[3]=OAD>>16;
		return 4;
	}
	if((OAD>>24)==0x44)
	{//Ӧ������IO�б�
		Len=GET_OAD_ConnectApp(PORTn,OAD,p8tx,LENmax_TxSPACE);//��Ӧ�����ӿɷ��ʶ����б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	if(((OAD>>16)&0xff00)<0x2100)
	{//iMeter
		Len=GET_iMeter_OAD(PORTn,OAD,p8tx,LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	
#if (USER/100)==17//???
	if((OAD&0xffffff00)==0x40480200)
	{
		pLib=Get_DL698_SET_pLib(OAD&0xffffff00);//??DATA_pLib??;??:OAD;??:Lib??(=0??????OI)
		if(pLib==0)
		{
			UARTCtrl->DAR=6;//?????           (6)
			return 0;
		}
		MR((u32)p8tx,pLib->Addr,8);
		return 8;
		//MR((u32)p8tx,ADDR_4048_127,8);
		//return 8;
	}
	if((OAD&0xffffff00)==0x45000C00)
	{
		Len=Get_Request_4500(PORTn,OAD,p8tx,LENmax_TxSPACE);
		return Len;
	}
#endif
#if (USER/100)==15//����ר��
	if((OAD&0xffffff00)==0x31060B00)
	{//����ר���ն�ͣ/�ϵ��¼�����11���ò���2
 			pLib=Get_DL698_SET_pLib(OAD&0xffffff00);//�õ�DATA_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
			if(pLib==0)
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
				return 0;
			}
			MR((u32)p8tx,pLib->Addr,4);
			return 4;
	}
#endif
	if(((OAD>>16)&0xf000)==0x3000)
	{//�¼�
		Len=GET_EVENT_OAD(PORTn,OAD,p8tx,LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	if((OAD>>16)==0xF001)
	{//�ļ�����
		Len=GET_Request_F001(PORTn,OAD,p8tx,LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	if((OAD>>16)==0xF100)
	{//ESAM
		Len=GET_ESAM_OAD(PORTn,OAD,p8tx,LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	if(((OAD>>16)&0xff00)==0xF200)
	{//��������豸��
		Len=GET_Request_F200(pLib,PORTn,OAD,p8tx,LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
		if(Len)
		{
			return Len;
		}
		UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	}
	
#if (USER/100)==9//����
	if(OAD==0xff100200)//�����ߴ�����Ϣ��
	{
		Len=GET_Request_FF10(pLib,PORTn,OAD,p8tx,LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
		if(Len)
		{
			return Len;
		}
		UARTCtrl->DAR=6;//���󲻴��ڣ�6��
		return 0;
	}
#endif
#if MeterSpec==20//�Զ���Ӧ���ܱ���
	switch(OAD)
	{
		case 0x41040200://���ѹ
		case 0x41050200://�����/��������
		case 0x41060200://������
		case 0x41090200://���ܱ��й�����
		case 0x410A0200://���ܱ��޹�����
			 Len=GET_Request_4100(PORTn,OAD,p8tx,LENmax_TxSPACE);
				return Len;
	}
#endif
	if(((OAD>>24)==0x23)||((OAD>>24)==0x24))
	{//�������룬�ܼ���
		Len=GET_23xx_24xx(PORTn,OAD,p8tx,LENmax_TxSPACE);//���ܼ�����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	pLib=Get_DL698_DATA_pLib(OAD&0xffffff00);//�õ�DATA_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
	if(pLib==0)
	{
		pLib=Get_DL698_SET_pLib(OAD&0xffffff00);//�õ�SET_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
		if(pLib==0)
		{
			Len=GET_OAD_FactoryTest(PORTn,OAD,p8tx,LENmax_TxSPACE);//���������Խ��;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
			if(Len)
			{
				return Len;
			}
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		}
	}
	if(OAD&0xff00)
	{//��������
		if((OAD&0xff)==0)
		{//ȫ��Ԫ��
			switch(OAD)
			{
				case 0x60000200://�ɼ��������ñ�
				case 0x60120200://�������ñ�
				case 0x60140200://��ͨ�ɼ�����
				case 0x60160200://�¼��ɼ�����
				case 0x60180200://͸������
				case 0x601A0200://͸���������
				case 0x601C0200://�ϱ�����
				case 0x601E0200://�ɼ������
				case 0x60320200://�ɼ�״̬��
				case 0x60340200://�ɼ������ؼ�
					Len=GET_Request_6000(pLib,PORTn,OAD,p8tx,LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
					return Len;
				case 0x60020200://�����ѱ���
				case 0x60020300://��ǰԪ�ظ���.�����ѱ���
				case 0x60020400://���Ԫ�ظ���,�����ѱ���
				case 0x60020500://��̨���ѱ���
				case 0x60020600://�����ѱ�����¼��
				case 0x60020700://��̨���ѱ�����¼��
					Len=GET_Request_6002(PORTn,OAD,p8tx,LENmax_TxSPACE);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
					return Len;
				
				case 0x60000300://��ǰԪ�ظ���.�ɼ��������ñ�
				case 0x60140300://��ǰԪ�ظ���.��ͨ�ɼ�������
				case 0x60160300://��ǰԪ�ظ���.�¼��ɼ�������
				case 0x60180300://��ǰԪ�ظ���.͸��������
				case 0x601A0300://��ǰԪ�ظ���.͸���������
				case 0x601C0300://��ǰԪ�ظ���.�ϱ�������
				case 0x601E0300://��ǰԪ�ظ���.�ɼ������
				case 0x60320300://��ǰԪ�ظ���.�ɼ�״̬��
				case 0x60340300://��ǰԪ�ظ���.�ɼ������ؼ�
					if(LENmax_TxSPACE<3)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					Len=GET_NUMper(pLib);//���㵱ǰԪ�ظ���
					p8tx[0]=DataType_long_unsigned;
					p8tx[1]=Len>>8;
					p8tx[2]=Len;
					return 3;
				case 0x60000400://���Ԫ�ظ���.�ɼ��������ñ�
				case 0x60140400://���Ԫ�ظ���.��ͨ�ɼ�������
				case 0x60160400://���Ԫ�ظ���.�¼��ɼ�������
				case 0x60180400://���Ԫ�ظ���.͸��������
				case 0x601A0400://���Ԫ�ظ���.͸���������
				case 0x601C0400://���Ԫ�ظ���.�ϱ�������
				case 0x601E0400://���Ԫ�ظ���.�ɼ������
				case 0x60320400://���Ԫ�ظ���.�ɼ�״̬��
				case 0x60340400://���Ԫ�ظ���.�ɼ������ؼ�
					if(LENmax_TxSPACE<3)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					Len=pLib->LENmax/pLib->LENper;
					p8tx[0]=DataType_long_unsigned;
					p8tx[1]=Len>>8;
					p8tx[2]=Len;;
					return 3;
				case 0xF2000300://�豸������-RS232
				case 0xF2010300://�豸������-RS485
				case 0xF2020300://�豸������-����
				case 0xF2030300://�豸������-������
				case 0xF2040300://�豸������-ֱ��ģ����
				case 0xF2050300://�豸������-�̵������
				case 0xF2060300://�豸������-�澯���
				case 0xF2070300://�豸������-�๦�ܶ���
				case 0xF2090300://�豸������-�ز�/΢�������߽ӿ�
				case 0xF20A0300://�豸������-���������豸
				case 0xF20B0300://����
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//��֡��־
						return 0;
					}
					p8tx[0]=DataType_unsigned;
					p8tx[1]=pLib->LENmax;//�豸������
					return 2;

				case 0x40000200://����ʱ��
					Len=8;
					break;
				default:
					Len=Get_DL698DataLen((u8*)pLib->Addr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
					if(pLib->pDataType)
					{
						if((Len&0xff)==0)
						{
							UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
							return 0;
						}
					}
					Len>>=8;
					break;
			}
			if(Len>pLib->LENmax)
			{
				if(pLib->LENmax!=0)
				{//�����Ǵ洢��FLASH
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
			}
			if(Len>LENmax_TxSPACE)
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			switch(OAD>>16)
			{
				case 0x4000://����ʱ��
					if((OAD&0xff00)==0x200)
					{//����=2
						p8tx[0]=DataType_date_time_s;
						MR((u32)p8tx+1,pLib->Addr,7);
						Len=8;
						break;
					}
				default:
					if((pLib->rw&0x1)||(Comm_Ram->Factory==0x55))
					{
						MR((u32)p8tx,pLib->Addr,Len);
					}
					else
					{//�������
						UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д             ��3��
						return 0;
					}
					break;
			}
		}
		else
		{//����Ԫ��
			Len=GET_OAD_Element(PORTn,(u8*)pLib->Addr,OAD&0xff,pLib->LENmax,p8tx,LENmax_TxSPACE);//��һ������Ԫ��;;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
		}
		
//��ȷ������
		switch(OAD)
		{
			case 0xF2030200://��ȡ���������뿪������Ԫ
				CLR_SwitchIn_CD();//�忪���������λCD 
				break;
		}
		return Len;
	}
	else
	{//0��ʾ�����������ԣ����������������
		UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д             ��3��
		return 0;
	}
}




























