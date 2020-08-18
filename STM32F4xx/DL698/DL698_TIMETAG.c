
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_TIMETAG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"




u32 DL698_TIMETAG(u8* p8rx)//ʱ���ǩ;���:����֡��ַ;����:0=��Чʱ����,1=��Чʱ��
{
	u32 i;
	u32 LEN_SA;
	u32 TI;
	u32 Len;
	
	LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	switch(p8rx[3]&0x7)//Ӧ�ò㹦����
	{
		//0	����	
		case 1://	��·����	��·���ӹ�����¼���������˳���¼��
//LINK-APDU��=CHOICE
//{
//  Ԥ��������	     [1]		LINK-Request��
//Ԥ������Ӧ       [129]	LINK-Response
//}
			return 0;//����:0=��Чʱ����,1=��Чʱ��
		//2	����	
		case 3://	�û�����	Ӧ�����ӹ������ݽ�������
//�ͻ���Ӧ�ò�Э�����ݵ�Ԫ��Client-APDU��
//Client-APDU��=SEQUENCE
//{
//  Ӧ�ò����  CHOICE
//  {
//    ����Ӧ����������	 [2]		CONNECT-Request��
//    �Ͽ�Ӧ����������	 [3]		RELEASE-Request��
//    ��ȡ����			 [5]		GET-Request��
//    �������� 			 [6]		SET-Request��
//    �������� 			 [7]		ACTION-Request��
//    �ϱ�Ӧ�� 			 [8]		REPORT-Response��
//    ��������   		 [9]		PROXY-Request
//}��
//ʱ���ǩ    TimeTag  OPTIONAL
//}
			i=p8rx[LEN_SA+8];
			switch(i)
			{
				case 2://����Ӧ����������	 [2]		CONNECT-Request��
					return 0;//����:0=��Чʱ����,1=��Чʱ��
				case 3://�Ͽ�Ӧ����������	 [3]		RELEASE-Request��
					return 0;//����:0=��Чʱ����,1=��Чʱ��
				case 5://��ȡ����			 [5]		GET-Request��
					return 0;//����:0=��Чʱ����,1=��Чʱ��
				case 6://�������� 			 [6]		SET-Request��
					return 0;//����:0=��Чʱ����,1=��Чʱ��
				case 7://�������� 			 [7]		ACTION-Request��
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
							p8rx+=LEN_SA+8+3+4;
							Len=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							p8rx+=Len;
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
							break;
					}
					break;
				case 8://�ϱ�Ӧ�� 			 [8]		REPORT-Response��
					return 0;//����:0=��Чʱ����,1=��Чʱ��
				case 9://��������   		 [9]		PROXY-Request
					return 0;//����:0=��Чʱ����,1=��Чʱ��
				case 16://��ȫ����   		[16]		SECURITY-Request
					return 0;//����:0=��Чʱ����,1=��Чʱ��
				default:
					return 0;//����:0=��Чʱ����,1=��Чʱ��
			}
			break;
		//4��7	����	
	}
	if(p8rx[0]==0)
	{//��ʱ���ǩ
		return 0;//����:0=��Чʱ����,1=��Чʱ��
	}
	p8rx++;
	i=Check_pYYMDHMS_hex(p8rx);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
	if(i)
	{
		return 1;//����:0=��Ч,1=��Ч
	}
	TI=(p8rx[8]<<8)+p8rx[9];//ȡƵ��ֵ
	if(TI==0)
	{
		return 0;//����:0=��Ч,1=��Ч
	}
	switch(p8rx[7])
	{
		case 0://��      ��0����
			break;
		case 1://��      ��1����
			TI*=60;
			break;
		case 2://ʱ      ��2����
			TI*=60*60;
			break;
		case 3://��      ��3����
		case 4://��      ��4����
		case 5://��      ��5��
		default:
			TI*=60*60*24;
			break;
	}
	i=YYMDHMS_Sub_hex((u8*)&Comm_Ram->DL698YMDHMS,p8rx);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
	if(i>>31)
	{
		i=~i;
		i++;
	}
	if(i>TI)
	{
		return 1;//����:0=��Ч,1=��Ч
	}
	return 0;//����:0=��Ч,1=��Ч
}

