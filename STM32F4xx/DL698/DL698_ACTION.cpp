
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_TESAM.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"

#include "../DL698/DL698_ACTION_EVENT.h"
#include "../DL698/DL698_ACTION_4000.h"
#include "../DL698/DL698_ACTION_5000.h"
#include "../DL698/DL698_ACTION_6000.h"
#include "../DL698/DL698_ACTION_8000.h"
#include "../DL698/DL698_ACTION_F000.h"
#include "../DL698/DL698_FILE.h"
#include "../DL698/DL698_GET_SET_ACTION_2000.h"
#include "../Device/IC_BUZZ.h"
#include "../DL698/DL698_FactoryTest.h"


//ACTION-Request��=CHOICE
//{
//��������                	     [1] ActionRequest��
//�������ɸ����󷽷�����                  	     [2] ActionRequestList��
//�������ɸ����󷽷����ȡ���ɸ�������������   [3] ActionThenGetRequestNormalList
//}
//ACTION-Response��=CHOICE
//{
//��������Ӧ                	 [1] ActionResponseNormal��
//�������ɸ����󷽷�����Ӧ                	 [2] ActionResponseNormalList��
//�������ɸ����󷽷����ȡ���ɸ����Ե���Ӧ	 [3] ActionThenGetResponseNormalList
//}

void ACTION_Request(u32 PORTn)
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
	u32 OAD;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//�ͻ���������Ϣ

	BUZZ_Generate(2);//��������,��ڷ�����
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

	LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	p8tx[0]=135;//������Ӧ        	[135]	ACTION-Response
	switch(p8rx[LEN_SA+8+1])
	{
		case 1://��������                	     [1] ActionRequest
//ActionRequest��=SEQUENCE
//{
//  �������-���ȼ�      PIID��
//  һ�����󷽷�������   OMD��
//  ��������             Data
//}
//ActionResponseNormal��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  һ�����󷽷�������    OMD��
//  ����ִ�н��          DAR��
//  ������������          Data  OPTIONAL
//}
			p8tx[1]=1;//��������Ӧ                	 [1] ActionResponseNormal��
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			p8rx+=LEN_SA+8+3;
			p8tx[LEN_TxAPDU+0]=p8rx[0];//OMD
			p8tx[LEN_TxAPDU+1]=p8rx[1];//OMD
			p8tx[LEN_TxAPDU+2]=p8rx[2];//OMD
			p8tx[LEN_TxAPDU+3]=p8rx[3];//OMD
			p8tx[LEN_TxAPDU+4]=0;//DAR
			p8tx[LEN_TxAPDU+5]=0;//Data OPTIONAL=0 ��ʾû������
			LEN_TxAPDU+=5;//6;
			LENmax_TxSPACE-=5;//6;
			LEN_OAD=ACTION(PORTn,p8rx,LEN_RxAPDU,p8tx+LEN_TxAPDU,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
			{
				if(UARTCtrl->DAR==254)//����Ӧ�޷���֡(��չ��   254
				{
					LEN_TxAPDU=0;
					break;
				}
				p8tx[LEN_TxAPDU-1]=UARTCtrl->DAR;
			}
			if((LEN_OAD&0xffff)==0)
			{//û���ݷ���
				//p8tx[LEN_TxAPDU-1]=1;//Data OPTIONAL=1 ��ʾ������
				LEN_TxAPDU+=1;
			}
			else
			{
				LEN_TxAPDU+=LEN_OAD&0xffff;
			}
			break;
		case 2://�������ɸ����󷽷�����                  	     [2] ActionRequestList
//ActionRequestList��=SEQUENCE
//{
//  �������-���ȼ�  PIID��
//  ���ɸ���������   SEQUENCE OF
//  {
//һ�����󷽷�������  OMD��
//    ��������            Data
//  }
//}
//ActionResponseNormalList��=SEQUENCE
//{
//  �������-���ȼ�-ACD     PIID-ACD��
//  ���ɸ����󷽷��������  SEQUENCE OF
//  {
//һ�����󷽷�������   OMD��
//     ����ִ�н��         DAR��
//     ������������         Data  OPTIONAL
//}
//}
			p8tx[1]=2;//�������ɸ����󷽷�����Ӧ                	 [2] ActionResponseNormalList��
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			p8tx[3]=0;//(SEQUENCE OF)
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=4;
			
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			LEN_RxAPDU-=1;
			p8rx+=LEN_SA+8+4;
			while(x--)
			{
				if(LENmax_TxSPACE<6)
				{
					break;//���ͻ��岻��
				}
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OMD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OMD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OMD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OMD
				p8tx[LEN_TxAPDU+4]=0;//DAR
				p8tx[LEN_TxAPDU+5]=0;//Data OPTIONAL=0 ��ʾû������
				LEN_TxAPDU+=5;//6;
				LENmax_TxSPACE-=5;//6;
				LEN_OAD=ACTION(PORTn,p8rx,LEN_RxAPDU,p8tx+LEN_TxAPDU,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
				if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
				{
					p8tx[LEN_TxAPDU-1]=UARTCtrl->DAR;
				}
				if((LEN_OAD>>16)==0)
				{
					i=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					LEN_OAD&=0xffff;
					LEN_OAD|=i<<16;
				}
				if((LEN_OAD&0xffff)==0)
				{//û�����ݷ���
					//p8tx[LEN_TxAPDU-1]=1;//Data OPTIONAL=1 ��ʾ������
					LEN_TxAPDU+=1;
				}
				else
				{
					LEN_TxAPDU+=LEN_OAD&0xffff;
				}
				p8tx[3]++;//+1(SEQUENCE OF)
				LEN_RxAPDU-=4+(LEN_OAD>>16);
				p8rx+=4+(LEN_OAD>>16);
			}
			break;
		case 3://�������ɸ����󷽷����ȡ���ɸ�������������   [3] ActionThenGetRequestNormalList
//ActionThenGetRequestNormalList��=SEQUENCE
//{
//  �������-���ȼ�  PIID��
//  ���ɸ��������󷽷����ȡ��������  SEQUENCE OF
//  {
//һ�����õĶ��󷽷�������   OMD��
//��������                   Data��
//һ����ȡ�Ķ�������������   OAD��
//��ȡ��ʱ                   unsigned
//  }
//}
//ActionThenGetResponseNormalList��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  �������ɸ����󷽷����ȡ���ԵĽ��  SEQUENCE OF
//  {
//һ�����õĶ��󷽷�������   OMD��
//     ����ִ�н��               DAR��
//     ������������               Data OPTIONAL��
//     һ����ȡ�Ķ�������������   OAD��
//     ��ȡ��Ӧ����               Get-Result
//}
//}
			p8tx[1]=3;//���ú��ȡ���ɸ�������������	[3] SetThenGetRequestNormalList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			p8tx[3]=0;//(SEQUENCE OF)
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=4;
			
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			LEN_RxAPDU-=1;
			p8rx+=LEN_SA+8+4;
			while(x--)
			{
				//����1��OAD
				if(LENmax_TxSPACE<6)
				{
					break;//���ͻ��岻��
				}
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OMD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OMD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OMD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OMD
				p8tx[LEN_TxAPDU+4]=0;//DAR
				p8tx[LEN_TxAPDU+5]=0;//Data OPTIONAL=0 ��ʾû������
				LEN_TxAPDU+=5;//6;
				LENmax_TxSPACE-=5;//6;
				LEN_OAD=ACTION(PORTn,p8rx,LEN_RxAPDU,p8tx+LEN_TxAPDU,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
				if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
				{
					p8tx[LEN_TxAPDU-1]=UARTCtrl->DAR;
				}
				if((LEN_OAD>>16)==0)
				{
					i=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					LEN_OAD&=0xffff;
					LEN_OAD|=i<<16;
				}
				if((LEN_OAD&0xffff)==0)
				{//û�����ݷ���
					//p8tx[LEN_TxAPDU-1]=1;//Data OPTIONAL=1 ��ʾ������
					LEN_TxAPDU+=1;
				}
				else
				{
					LEN_TxAPDU+=LEN_OAD&0xffff;
				}
				LEN_RxAPDU-=4+(LEN_OAD>>16);
				p8rx+=4+(LEN_OAD>>16);
				//��ȡ
				if(LENmax_TxSPACE<6)
				{
					break;//���ͻ��岻��
				}
				OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
				//��ʱʱ��̶�1�ֽ���ֵ
				LEN_TxAPDU+=4;
				LENmax_TxSPACE-=4;
				LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+1,LENmax_TxSPACE-1);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
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
					p8tx[LEN_TxAPDU]=1;//Get-Result��=CHOICE,[1]   DATA
					LEN_TxAPDU+=LEN_OAD+1;
					LENmax_TxSPACE-=LEN_OAD+1;
				}
				p8tx[3]++;//+1(SEQUENCE OF)
				p8rx+=4+1;//��ʱʱ��̶�1�ֽ���ֵ
				LEN_RxAPDU-=4+1;
			}
			break;
		default:
			LEN_TxAPDU=0;
			break;
	}
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
}

u32 ACTION(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
//OMD��=SEQUENCE
//{
//�����ʶ  OI��
//  ������ʶ  unsigned��1��255����
//  ����ģʽ  unsigned��0��
//}
//������ʶ���������󷽷���š�
//����ģʽ����ֵĬ��Ϊ0��
	//u32 i;
	u32 OI;
	u32 Len;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;
	Len=0;
	OI=(p8rx[0]<<8)+p8rx[1];
	
	if((OI&0xf000)==0x3000)
	{
		Len=ACTION_EVENT(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	if(((OI>>8)==0x23)||((OI>>8)==0x24))
	{//�������룬�ܼ���
		Len=ACTION_23xx_24xx(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//�����ܼ�����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	switch(OI)
	{
	#if (USER/100)==17//����ϵ
		case 0x4048://�ϱ�ʵʱ����
			Len=ACTION_4048(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����һ�����󷽷�;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
	#endif
		case 0x4000://�㲥Уʱ
			Len=ACTION_4000(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����һ�����󷽷�;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x4006://ʱ��Դ
			//����127�����ã���
			//����128�����ã���
			return 0;
		case 0x4200://·����Ϣ��Ԫ
			Len=ACTION_4200(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x4300://�����豸
			Len=ACTION_4300(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x5000://��������������Ա�
		case 0x5001:
		case 0x5002:
		case 0x5003:
		case 0x5004:
		case 0x5005:
		case 0x5006:
		case 0x5007:
		case 0x5008:
		case 0x5009:
		case 0x500A:
		case 0x500B:
		case 0x5011:
			Len=ACTION_5000(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x6000://�ɼ��������ñ�
			Len=ACTION_6000(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			Terminal_Ram->SET6000=0x55;//�ɼ��������ñ������ñ�־:0=û����,0x55=����
			return Len;
		case 0x6002://�ѱ�
			Len=ACTION_6002(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x6012://�������ñ�
			Len=ACTION_6012(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			Terminal_Ram->SETTask=0xff;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
			MR(ADDR_6012_2_SDRAM,ADDR_6012_2,LENmax_6012_2);//�������ñ�FLASHtoSDRAM
			return Len;
		case 0x6014://��ͨ�ɼ�����
			Len=ACTION_6014(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			Terminal_Ram->SETTask=0xff;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
			MR(ADDR_6014_SDRAM,ADDR_6014,LENmax_6014);//��ͨ�ɼ�����FLASHtoSDRAM
			return Len;
		case 0x6016://�¼��ɼ�����
			Len=ACTION_6016(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			Terminal_Ram->SETTask=0xff;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
			MR(ADDR_6016_SDRAM,ADDR_6016,LENmax_6016);//�¼��ɼ�����FLASHtoSDRAM
			return Len;
		case 0x6018://͸������
			Len=ACTION_6018(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			Terminal_Ram->SETTask=0xff;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
			return Len;
		case 0x601C://�ϱ�����
			Len=ACTION_601C(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			MR(ADDR_601C_SDRAM,ADDR_601C,LENmax_601C);//�ϱ�����FLASHtoSDRAM
			return Len;
		case 0x601E://�ɼ������
			Len=ACTION_601E(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		
		case 0x8000://ң��
			Len=ACTION_8000(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x8001://����
			Len=ACTION_8001(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x8002://�߷Ѹ澯
			Len=ACTION_8002(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x8103://ʱ�ι���
			Len=ACTION_8103(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x8104://���ݿ�
			Len=ACTION_8104(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x8105://Ӫҵ��ͣ��
			Len=ACTION_8105(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x8106://��ǰ�����¸���
			Len=ACTION_8106(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x8107://�����
			Len=ACTION_8107(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0x8108://�µ��
			Len=ACTION_8108(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		
		case 0xF001://�ļ��ֿ鴫�����
			Len=ACTION_F001(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF002://FTP�������
			Len=ACTION_F002(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF100://ESAM
			Len=ACTION_F100(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF101://��ȫģʽ����
			Len=ACTION_F101(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		
		case 0xF200://RS232
			Len=ACTION_F200(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF201://RS485
			Len=ACTION_F201(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF202://����
			Len=ACTION_F202(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF205://�̵������
			Len=ACTION_F205(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF207://�๦�ܶ���
			Len=ACTION_F207(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF209://�ز�/΢�������߽ӿ�
			Len=ACTION_F209(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		case 0xF20B://����
			Len=ACTION_F20B(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		
		
		case 0xFFFF://�����Բ�
			Len=ACTION_FFFF(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}


































