

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SET.h"
#include "../DL698/DL698_SET_6000.h"
#include "../DL698/DL698_SET_EVENT.h"
#include "../DL698/DL698_GET.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../Device/RTC.h"
#include "../MS/MS.h"
#include "../DL698/TASK.h"
#include "../DL698/EVENT_Record.h"
#include "../DL698/DL698_GET_SET_ACTION_2000.h"
#include "../Device/IC_BUZZ.h"

#include "../DL698/DL698_JSON.h"
#include "../DL698/Info_3762.h"


//����������������ͣ�SET-Request������
//SET-Request��=CHOICE
//{
//����һ��������������        	[1] SetRequestNormal��
//�������ɸ�������������     	[2] SetRequestNormalList��
//���ú��ȡ���ɸ�������������	[3] SetThenGetRequestNormalList
//}
//SET-Response��=CHOICE
//{
//����һ���������Ե�ȷ����Ϣ��Ӧ   	[1] SetResponseNormal��
//�������ɸ��������Ե�ȷ����Ϣ��Ӧ  	[2] SetResponseNormalList��
//���õ�ȷ����Ϣ�Լ���ȡ����Ӧ 	     [3] SetThenGetResponseNormalList
//}

void SET_Request(u32 PORTn)
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
	u8* p8;
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
//68 1D 00 43  05  01 00 00 00 00 00  10  AE E1 
//06   Ӧ�ò����  CHOICE
//01   SET-Request��=CHOICE
//00   PIID
//40 01 01 00    OAD
//09   DataType
//04 12 34 56 78 
//00   TI
//CB AE 16 

	LEN_RxAPDU-=1;
	LENmax_TxSPACE-=1;//ʱ���ǩ:0=��
	LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	p8tx[0]=134;//������Ӧ       	[134]	SET-Response
	
	switch(p8rx[LEN_SA+8+1])
	{
		case 1://����һ��������������        	[1] SetRequestNormal
//����һ����������������������Ͷ���
//SetRequestNormal��=SEQUENCE
//{
//  �������-���ȼ�      PIID��
//  һ����������������   OAD��
//  ����                 Data
//}
//SetResponseNormal��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  һ����������������    OAD��
//  ����ִ�н��          DAR
//}
			p8tx[1]=1;//����һ���������Ե�ȷ����Ϣ��Ӧ   	[1] SetResponseNormal
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
		
			p8rx+=LEN_SA+8+3;
			//ʱ���ǩ
			LEN_OAD=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			if(p8rx[4+LEN_OAD]==1)
			{//��ʱ���ǩ
				i=Uart_TimeTag(p8rx+4+LEN_OAD+1);//ʱ���ǩ��Ч���ж�;���:p8rxָ��ʱ���ǩ;����:0=��Ч,1=��Ч
				if(i)
				{//1=����ͬp1>p2
					p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
					p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
					p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
					p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
					p8tx[LEN_TxAPDU+4]=32;//ʱ���ǩ��Ч(32)
					LEN_TxAPDU+=5;
					break;
				}
			}
			LEN_OAD=SET_OAD(PORTn,p8rx,LEN_RxAPDU);//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
			p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
			p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
			p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
			p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
			p8=(u8*)ADDR_1kDATABUFF;//���õ�OAD����
			p8[0]=DataType_array;
			p8[1]=1;
			p8[2]=DataType_OAD;
			p8[3]=p8rx[0];
			p8[4]=p8rx[1];
			p8[5]=p8rx[2];
			p8[6]=p8rx[3];
			Event_31180200(p8);//�ն˱���¼���¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF

			p8tx[LEN_TxAPDU+4]=UARTCtrl->DAR;//���ݷ��ʽ��
			LEN_TxAPDU+=5;
			break;
		case 2://�������ɸ�������������     	[2] SetRequestNormalList
//�������ɸ���������������������Ͷ���
//SetRequestNormalList��=SEQUENCE
//{
//  �������-���ȼ�  PIID��
//  ���ɸ���������   SEQUENCE OF
//  {
//    һ����������������  OAD��
//    ����                Data
//  }
//}
//SetResponseNormalList��=SEQUENCE
//{
//  �������-���ȼ�-ACD     PIID-ACD��
//  ���ɸ������������ý��  SEQUENCE OF
//  {
//һ����������������   OAD��
//     ����ִ�н��         DAR
//}
//}
			p8tx[1]=2;//�������ɸ��������Ե�ȷ����Ϣ��Ӧ  	[2] SetResponseNormalList��
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			p8tx[3]=0;//(SEQUENCE OF)
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=4;
			
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			LEN_RxAPDU-=1;
			p8rx+=LEN_SA+8+4;
			
			//ʱ���ǩ
			LEN_OAD=0;
			i=x;
			while(i--)
			{
				LEN_OAD+=4;
				LEN_OAD+=Get_DL698DataLen_S(p8rx+LEN_OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			}
			if(p8rx[LEN_OAD]==1)
			{//��ʱ���ǩ
				i=Uart_TimeTag(p8rx+LEN_OAD+1);//ʱ���ǩ��Ч���ж�;���:p8rxָ��ʱ���ǩ;����:0=��Ч,1=��Ч
				if(i)
				{//��Ч
					LEN_OAD=0;
					while(x--)
					{
						p8tx[LEN_TxAPDU+0]=p8rx[0+LEN_OAD];//OAD
						p8tx[LEN_TxAPDU+1]=p8rx[1+LEN_OAD];//OAD
						p8tx[LEN_TxAPDU+2]=p8rx[2+LEN_OAD];//OAD
						p8tx[LEN_TxAPDU+3]=p8rx[3+LEN_OAD];//OAD
						p8tx[LEN_TxAPDU+4]=32;//ʱ���ǩ��Ч(32)
						LEN_TxAPDU+=5;
						LEN_OAD+=4;
						LEN_OAD+=Get_DL698DataLen_S(p8rx+LEN_OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					}
					break;
				}
			}
			
			p8=(u8*)ADDR_1kDATABUFF;//���õ�OAD����(�¼���)
			p8[0]=DataType_array;
			p8[1]=0;
			while(x--)
			{
				if(LENmax_TxSPACE<5)
				{
					break;//���ͻ��岻��
				}
				LEN_OAD=SET_OAD(PORTn,p8rx,LEN_RxAPDU);//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
				
				p8tx[LEN_TxAPDU+4]=UARTCtrl->DAR;//���ݷ��ʽ��
				LEN_TxAPDU+=5;
				LENmax_TxSPACE-=5;
				if(LEN_OAD==0)
				{
					LEN_OAD=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				}
				if(UARTCtrl->DAR==0)//���ݷ��ʽ��
				{
					i=p8[1];//���õ�OAD����(�¼���)
					i*=5;
					i+=2;
					p8[i+0]=DataType_OAD;
					p8[i+1]=p8rx[0];//OAD
					p8[i+2]=p8rx[1];//OAD
					p8[i+3]=p8rx[2];//OAD
					p8[i+4]=p8rx[3];//OAD
					p8[1]++;//������+=1
				}
				
				p8tx[3]++;//+1(SEQUENCE OF)
				if(LEN_RxAPDU<(4+LEN_OAD))
				{
					p8tx[LEN_TxAPDU-1]=255;//���ݷ��ʽ��:����
					break;//����
				}
				LEN_RxAPDU-=4+LEN_OAD;
				p8rx+=4+LEN_OAD;
			}
			Event_31180200(p8);//�ն˱���¼���¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			break;
		case 3://���ú��ȡ���ɸ�������������	[3] SetThenGetRequestNormalList
//���ú��ȡ���ɸ���������������������Ͷ���
//SetThenGetRequestNormalList��=SEQUENCE
//{
//  �������-���ȼ�  PIID��
//  ���ɸ����ú��ȡ��������  SEQUENCE OF
//{
//һ�����õĶ�������   OAD��
//����                 Data��
	//һ����ȡ�Ķ�������   OAD��
	//��ʱ��ȡʱ��         unsigned //��ʱ��ȡʱ�䡪����λ���룬0��ʾ���÷�����Ĭ�ϵ���ʱʱ�䡣
//  }
//}
//SetThenGetResponseNormalList��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  ���ɸ������������ú��ȡ���  SEQUENCE OF
//  {
//һ�����õĶ�������������   OAD��
//     ����ִ�н��               DAR��
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
			
			p8=(u8*)ADDR_1kDATABUFF;//���õ�OAD����(�¼���)
			p8[0]=DataType_array;
			p8[1]=0;
			while(x--)
			{
				//����1��OAD
				if(LENmax_TxSPACE<5)
				{
					break;//���ͻ��岻��
				}
				LEN_OAD=SET_OAD(PORTn,p8rx,LEN_RxAPDU);//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
				
				p8tx[LEN_TxAPDU+4]=UARTCtrl->DAR;//���ݷ��ʽ��
				LEN_TxAPDU+=5;
				LENmax_TxSPACE-=5;
				
				if(LEN_OAD==0)
				{
					LEN_OAD=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				}
				if(UARTCtrl->DAR==0)//���ݷ��ʽ��
				{
					i=p8[1];//���õ�OAD����(�¼���)
					i*=5;
					i+=2;
					p8[i+0]=DataType_OAD;
					p8[i+1]=p8rx[0];//OAD
					p8[i+2]=p8rx[1];//OAD
					p8[i+3]=p8rx[2];//OAD
					p8[i+4]=p8rx[3];//OAD
					p8[1]++;//������+=1
				}				
				
				if(LEN_RxAPDU<(4+LEN_OAD))
				{
					p8tx[LEN_TxAPDU-1]=255;//���ݷ��ʽ��:����
					break;//����
				}
				LEN_RxAPDU-=4+LEN_OAD;
				p8rx+=4+LEN_OAD;
				//��1��OAD
				if(LENmax_TxSPACE<6)
				{
					break;//���ͻ��岻��
				}
				OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
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
					p8tx[LEN_TxAPDU]=1;
					LEN_TxAPDU+=LEN_OAD+1;
					LENmax_TxSPACE-=LEN_OAD+1;
				}
				p8rx+=4+1;//1byte ���ú��ȡ��ʱ
				LEN_RxAPDU-=4+1;
				p8tx[3]++;//+1(SEQUENCE OF)
			}
			Event_31180200(p8);//�ն˱���¼���¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			break;
		default:
			LEN_TxAPDU=0;
			break;
	}
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
}

u32 SET_OAD_Element(u32 PORTn,u8 *p8rx,u8* ADDR_OAD,u32 Element,u32 LENmax_OAD)//����һ������Ԫ��;����:��Ԫ�ص����ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Len;//��Ԫ�س���
	u32 LenT;//ԭ�����ܳ���
	u32 Len0;//ԭԪ��ǰ����
	u32 Len1;//ԭԪ�س���
	u32 Len2;//ԭԪ�غ󳤶�
	u8* p8;
	u8 *p8s;
	u8 *p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//��Ԫ�س���
	Len=Get_DL698DataLen_S(p8rx,LENmax_OAD);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
	if(ADDR_OAD==0)
	{
		UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
		return Len;
	}
	if(LENmax_OAD>LEN_128KDATABUFF)
	{
		UARTCtrl->DAR=255;//���ݷ��ʽ��
		return Len;
	}
	MR(ADDR_128KDATABUFF,(u32)ADDR_OAD,LENmax_OAD);
	//ԭ�����ܳ���
	LenT=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF,LENmax_OAD);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
	//ԭԪ�ص�ַ
	p8=Get_Element((u8*)ADDR_128KDATABUFF,Element,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
	if(p8==0)
	{
		UARTCtrl->DAR=255;//���ݷ��ʽ��
		return Len;
	}
	//ԭԪ�س���
	Len1=Get_DL698DataLen_S(p8,0);
	//ԭԪ��ǰ����
	Len0=(u32)p8-(ADDR_128KDATABUFF);
	//ԭԪ�غ󳤶�
	Len2=LenT-(Len0+Len1);
	if((LenT+Len0+Len+Len2)>LEN_128KDATABUFF)
	{
		UARTCtrl->DAR=255;//���ݷ��ʽ��
		return Len;
	}
	//��֯�����ݽṹ
	p8s=(u8*)ADDR_128KDATABUFF;
	p8d=(u8*)ADDR_128KDATABUFF+LenT;
	//ԭԪ��ǰ����
	for(i=0;i<Len0;i++)
	{
		p8d[0]=p8s[i];
		p8d++;
	}
	//��Ԫ������
	for(i=0;i<Len;i++)
	{
		p8d[0]=p8rx[i];
		p8d++;
	}
	//ԭԪ�غ�����
	p8s=(u8*)ADDR_128KDATABUFF+Len0+Len1;
	for(i=0;i<Len2;i++)
	{
		p8d[0]=p8s[i];
		p8d++;
	}
	i=Len0+Len+Len2;
	if(i>LENmax_OAD)
	{
		UARTCtrl->DAR=255;//���ݷ��ʽ��
		return Len;
	}
	MW(ADDR_128KDATABUFF+LenT,(u32)ADDR_OAD,i);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	return Len;
}

u32 SET_OAD(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU)//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
{
//  һ����������������   OAD
//  ����                 Data
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
	u32 i;
	u32 n;
	u32 OAD;
	u32 Len;
	DL698_SETLIB_TypeDef *pLib;
	u8 *p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];

	if(((OAD>>8)&0xff)==1)
	{//�߼���ֻ��
		UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д��3��
		return 4;
	}
	if(((OAD>>24)==0x23)||((OAD>>24)==0x24))
	{//�������룬�ܼ���
		Len=SET_23xx_24xx(PORTn,p8rx,LEN_RxAPDU);//�����ܼ�����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	switch(OAD>>16)
	{
		case 0x6012://�������ñ�
		case 0x6014://��ͨ�ɼ�������
		case 0x6016://�¼��ɼ�������
		case 0x6018://͸��������
		case 0x601c://�ϱ�������
		case 0x601e://�ɼ������
			Terminal_Ram->SETTask=0xff;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
			Len=SET_OAD_6000(PORTn,p8rx,LEN_RxAPDU);//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
			switch(OAD>>16)
			{
				case 0x6012://�������ñ�
					MR(ADDR_6012_2_SDRAM,ADDR_6012_2,LENmax_6012_2);//�������ñ�FLASHtoSDRAM
					break;
				case 0x6014://��ͨ�ɼ�������
					MR(ADDR_6014_SDRAM,ADDR_6014,LENmax_6014);//��ͨ�ɼ�����FLASHtoSDRAM
					break;
				case 0x6016://�¼��ɼ�������
					MR(ADDR_6016_SDRAM,ADDR_6016,LENmax_6016);//�¼��ɼ�����FLASHtoSDRAM
					break;
				case 0x601c://�ϱ�������
					MR(ADDR_601C_SDRAM,ADDR_601C,LENmax_601C);//�ϱ�����FLASHtoSDRAM
					break;
			}
			return Len;
	}
#if (USER/100)==15//����
	if((OAD&0xffffff00)==0x31060B00)
	{
		Len= Get_DL698DataLen_S(p8rx+4,0);
		pLib=Get_DL698_SET_pLib(OAD);//�õ�SET_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
		if(pLib==0)
		{
			UARTCtrl->DAR=6;////���ݷ��ʽ��;���󲻴���           ��6��
			return 0;
		}
		MW((u32)p8rx+4,pLib->Addr,Len);
		UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
		return Len;
	}
#endif
	if((OAD&0xf0000000)==0x30000000)
	{//�����¼���
		Len=SET_OAD_EVENT(PORTn,p8rx,LEN_RxAPDU);//����һ����������;����:����������ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
		return Len;
	}
	p8rx+=4;
	switch(OAD)
	{
		case 0x40000200://����ʱ��
			if(LEN_RxAPDU<(4+8))
			{
				UARTCtrl->DAR=7;//���ݷ��ʽ��;7=���Ͳ�ƥ��
				return 0;
			}
			//date_time_s��=SEQUENCE
			//{
			//  year          long-unsigned��
			//  month         unsigned��
			//  day           unsigned��
			//  hour          unsigned��
			//  minute        unsigned��
			//  second        unsigned
			//}
			if(p8rx[0]!=DataType_date_time_s)
			{
				UARTCtrl->DAR=255;//���ݷ��ʽ��
				return 0;
			}
			i=Check_pYYMDHMS_hex(p8rx+1);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
			if(i)
			{
				UARTCtrl->DAR=255;//���ݷ��ʽ��
				return 0;
			}
			p8rx+=1;
			MR(ADDR_128KDATABUFF+(64*1024),ADDR_DL698YMDHMS,7);//��ʱ�¼�����ǰʱ��
			p8d=(u8*)ADDR_RTCBUFF;
			i=p8rx[6];
			i=hex_bcd(i);
			p8d[0]=i;
			i=p8rx[5];
			i=hex_bcd(i);
			p8d[1]=i;
			i=p8rx[4];
			i=hex_bcd(i);
			p8d[2]=i;
			i=p8rx[3];
			i=hex_bcd(i);
			p8d[4]=i;
			i=p8rx[2];
			i=hex_bcd(i);
			p8d[5]=i;
			i=(p8rx[0]<<8)+p8rx[1];
			i=hex_bcd(i);
			p8d[6]=i;
			WriteRTC(ADDR_RTCBUFF);//дʵʱ��
			MR(ADDR_DL698YMDHMS,(u32)p8rx,7);//��ʱ�¼�������ʱ��
			#if (USER/100)==17
			MR((u32)JSON->Lastrealtime,ADDR_DL698YMDHMS,7);//��ʱ�¼�������ʱ��
			MR((u32)JSON->reportfiletime,ADDR_DL698YMDHMS,7);//��ʱ�¼�������ʱ��
			#endif
			Event_OAD(0x31140200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
			//Terminal_Ram->SETtime=0xff;//ʱ�����ñ�־:0=û����,0xff=����,���ڵ��Ժ�ز��ڻ����Բ���
			return 8;
			
//����ֵ���
		case 0x40010200://ͨ�ŵ�ַ
			if(p8rx[1]==0)
			{//����=0
				Len= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				UARTCtrl->DAR=8;//���ݷ��ʽ��
				return Len;
			}
			i=Uart_TSA_Comp(p8rx+1,(u8 *)ADDR_4001+1);//ͨ�ŵ�ַ�Ƚ�,pFLASH���ݶ���ADDR_128K_DDATABUFF�бȽ�,����:0=��ͬ,!=0����ͬ
			if(i)
			{//��ַ��ͬ
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
				UARTCtrl->ReNewLink=0x55;//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
				UARTCtrl->ReNewLink=0x55;//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
			}
			break;
		case 0x40000300://Уʱģʽ
			if((p8rx[1]>=3)&&(p8rx[1]!=255))
			{
				Len= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				UARTCtrl->DAR=8;//���ݷ��ʽ��
				return Len;
			}
			break;
		case 0x40040200://�豸����λ��
			if(p8rx[5]>1)
			{
				Len= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				UARTCtrl->DAR=8;//���ݷ��ʽ��
				return Len;
			}
			break;
		case 0x42020200://��������
			i=(p8rx[5]<<24)|(p8rx[6]<<16)|(p8rx[7]<<8)|(p8rx[8]<<0);
			if((i!=0xf2010201)&&(i!=0xf2010202))
			{//����rs485��
				Len= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				UARTCtrl->DAR=6;//���ݷ��ʽ��
				return Len;
			}
			break;
		case 0x42040200://�㲥Уʱ����-2
			i=(p8rx[3]<<16)|(p8rx[4]<<8)|(p8rx[5]<<0);
			i=Check_HMS_hex(i);//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
			if(i)
			{
				Len= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				UARTCtrl->DAR=8;//���ݷ��ʽ��
				return Len;
			}
			break;
		case 0x42040300://�㲥Уʱ����-3
			i=(p8rx[5]<<16)|(p8rx[6]<<8)|(p8rx[7]<<0);
			i=Check_HMS_hex(i);//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
			if(i)
			{
				Len= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				UARTCtrl->DAR=8;//���ݷ��ʽ��
				return Len;
			}
			break;
		case 0x45000200://���߹���ͨ������
			if(p8rx[3]>2)
			{
				Len= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				UARTCtrl->DAR=8;//���ݷ��ʽ��
				return Len;
			}
			break;
		case 0x80010500://�Զ�����ʱ��
			n=p8rx[1];//������
			for(i=0;i<n;i++)
			{
				if((p8rx[2+(6*i)+3]>24)||(p8rx[2+(6*i)+5]>24))
				{
					Len= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					UARTCtrl->DAR=8;//���ݷ��ʽ��
					return Len;
				}
			}
			break;
	#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		case 0xF2090900://����ز�Ƶ�����
			i=MRR(ADDR_F209_9+1,1);
			if(i!=p8rx[1])
			{//����ز�Ƶ����ű仯
				MW((u32)p8rx+1,ADDR_F209_9+1, 1);
				UART_3762_AFN5F16(RS485_4PORT,&p8rx[1]);
				Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
			return 1;
		case 0xF2090B00://��������ʶ�𿪹�
#if 0		
// ̨��ʶ�� �ɹ��� ���Զ��ر�
			i=MRR(ADDR_F209_11+1,1);
			if(i!=p8rx[1])
			{
				MW((u32)p8rx+1,ADDR_F209_11+1, 1);
				UART_3762_AFN5F6(RS485_4PORT,(u32)p8rx[1]);
				//·�ɻָ�
				Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
				Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
			}
#else
			UART_3762_AFN5F6(RS485_4PORT,(u32)p8rx[1]);
			//·�ɻָ�
			Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
			Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
#endif
			UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
			return 1;
		case 0xF2091600://�ܾ��ڵ��ϱ�ʹ�ܱ�־
			ms_Type* ms;
			ms=(ms_Type *)Get_ms();
			i=MRR(ADDR_F209_22+1,1);
			ms->link_printf("1-%x\n",i,0,0);
			if(i!=p8rx[1])
			{
				MW((u32)p8rx+1,ADDR_F209_22+1, 1);
				i=MRR(ADDR_F209_22+1,1);
				ms->link_printf("2-%x\n",i,0,0);
				UART_3762_AFN5F200(RS485_4PORT,&p8rx[1]);
				//UART_3762_AFN5F200(RS485_4PORT,&p8rx[1]);
				Terminal_Router->StopFlags&=~(1<<3);//b3=UART��·����Ϣ
				Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				ms->link_printf("3-%x\n",i,0,0);
			}
			
			UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
			return 1;
	#endif
		default:
			break;
	}
	pLib=Get_DL698_SET_pLib(OAD);//�õ�SET_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//���ݷ��ʽ��;���󲻴���           ��6��
		return 0;
	}
	if((OAD>>8)&0xff)
	{//��������
		if(pLib->Addr==0)
		{
			UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
			return 0;
		}
		if(Comm_Ram->Factory!=0x55)
		{//���ǹ���ģʽ
			if((pLib->rw&0x2)==0)
			{//������д
				UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
				return 0;
			}
		}
		if((OAD&0xff)==0)
		{//ȫ��Ԫ��
			Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			if(pLib->pDataType)
			{
				if((Len&0xff)==0)
				{
					UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
					return 0;
				}
			}
			Len>>=8;
			if(LEN_RxAPDU<(4+Len))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(Len>pLib->LENmax)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return Len;
			}
			MW((u32)p8rx,pLib->Addr,Len);
			UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
			return Len;
		}
		else
		{//����Ԫ��
			Len=SET_OAD_Element(PORTn,p8rx,(u8*)pLib->Addr,OAD&0xff,pLib->LENmax);//����һ������Ԫ��;����:��Ԫ�ص����ݳ���(������4�ֽ�OAD),���ݷ��ʽ��UARTCtrl->DAR
			return Len;
		}
	}
	else
	{//0��ʾ�����������ԣ����������������
		UARTCtrl->DAR=3;//���ݷ��ʽ��;�ܾ���д(3��
		return 0;
	}
}




























