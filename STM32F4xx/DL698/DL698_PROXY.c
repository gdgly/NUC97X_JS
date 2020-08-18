
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_PROXY.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/TASK.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"

#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../Calculate/Calculate.h"
#include "../Device/IC_TESAM.h"


//PROXY-Request��=CHOICE
//{
//�����ȡ���ɸ������������ɸ�������������             [1] ProxyGetRequestList��
//�����ȡһ����������һ����¼�Ͷ�����������           [2] ProxyGetRequestRecord��
//�����������ɸ������������ɸ�������������             [3] ProxySetRequestList��
//�������ú��ȡ���ɸ������������ɸ�������������       [4] ProxySetThenGetRequestList��
//����������ɸ������������ɸ����󷽷�����             [5] ProxyActionRequestList��
//����������ȡ���ɸ������������ɸ����󷽷����������� [6] ProxyActionThenGetRequestList��
//����͸��ת����������                                 [7] ProxyTransCommandRequest
//}
//PROXY-Response��=CHOICE
//{
//�����ȡ���ɸ������������ɸ�����������Ӧ             [1] ProxyGetResponseList��
//�����ȡһ����������һ����¼�Ͷ���������Ӧ           [2] ProxyGetResponseRecord��
//�����������ɸ������������ɸ�����������Ӧ             [3] ProxySetResponseList��
//�������ú��ȡ���ɸ������������ɸ�����������Ӧ       [4] ProxySetThenGetResponseList��
//����������ɸ������������ɸ����󷽷���Ӧ             [5] ProxyActionResponseList��
//����������ȡ���ɸ������������ɸ����󷽷���������Ӧ [6] ProxyActionThenGetResponseList��
//����͸��ת��������Ӧ                                 [7] ProxyTransCommandResponse
//}


#define NUMmax_ReTx     2//�������ط�����

void PROXY_Request(u32 PORTn)
{
	u32 i;
	u32 x;
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
	
	u32 offset;
	u32 NUM_TSA;
	u32 NUM_OAD;
	u32 SETno;
	u32 ProxyPORTn;
	UARTCtrl_TypeDef * UARTCtrlProxy;
	u8* p8Data;
	u8* p8;
	u16 *p16;
	u32 NUM_ReTx;
	u8* p8OAD;
	u32 Err;
	u32 LEN_RSD;
	u32 LEN_RCSD;
	u32 CA;//�ͻ�����ַCA
#ifndef LINUX
	u32 *p32;
#endif

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
	p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���

	CA=p8rx[11];//�ͻ�����ַCA
	LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	if(UARTCtrl->pTxData)//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
	{
		ms=Get_ms();
		ms->ms_free(UARTCtrl->pTxData);
		UARTCtrl->pTxData=0;
	}
	UARTCtrl->PIID=p8rx[LEN_SA+8+2];//PIID
	switch(p8rx[LEN_SA+8+1])
	{
		case 1://�����ȡ���ɸ������������ɸ�������������             [1] ProxyGetRequestList��
//ProxyGetRequestList��=SEQUENCE
//{
//  �������-���ȼ�          PIID��
//  ������������ĳ�ʱʱ��   long-unsigned��
//  �������ɸ��������Ķ������Զ�ȡ  SEQUENCE OF
//  {
//    һ��Ŀ���������ַ         TSA��
//    ����һ���������ĳ�ʱʱ��   long-unsigned��
//    ���ɸ���������������       SEQUENCE OF OAD
//  }
//}
//68 45 00 43 05 01 00 00 00 00 00 10 44 E1 
//09 01 07 
//00 64 ������������ĳ�ʱʱ��   long-unsigned
//01 �������ɸ��������Ķ������Զ�ȡ  SEQUENCE OF
//07 05 22 22 22 22 22 22 һ��Ŀ���������ַ         TSA
//00 00 ����һ���������ĳ�ʱʱ��   long-unsigned
//09 ���ɸ���������������       SEQUENCE OF OAD
//00 10 02 00 OAD1
//00 20 02 00 OAD2
//00 00 02 00 OAD3
//00 30 02 01 OAD4
//00 40 02 01 OAD5
//00 50 02 00 OAD6
//00 60 02 00 OAD7
//00 70 02 00 OAD8
//00 80 02 00 OAD9
//00 92 C5 16 
//ProxyGetResponseList��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  �������ɸ��������Ķ�ȡ���  SEQUENCE OF
//  {
//		һ��Ŀ���������ַ  TSA��
//		���ɸ���������������������  SEQUENCE OF
//		{
//      ��������������  OAD��
//      �����ȡ���    Get-Result
//		}
//  }
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=137;//������Ӧ        	[137]	PROXY-Response
			p8tx[1]=1;//[1] ProxyGetResponseList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//������������ĳ�ʱʱ��
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//�õ�UART�붨ʱ����ַ
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//��ȡ����������
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// һ��Ŀ���������ַ         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA�ֽ���
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
				Err=1;//�ô���
				if(SETno<NUM_RMmax)
				{//�ҵ��������
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//������Ч
						p8=Get_Element(p8+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
						if(p8)
						{//�ҵ��˿ں�
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OADֵתΪͨ�ſں�
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if ((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//�������Ķ˿ں�
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
										if(p8)
										{//������
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://����Ӧ
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA�ֽ���
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//���ͻ��岻��
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//����һ���������ĳ�ʱʱ��
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//����1���������ĳ�ʱʱ����
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//��ǰ�跢��OAD��������ֵָ��
				NUM_OAD=p8rx[offset];
				offset+=1;
			//�鷢��֡
		#if ((USER%100)==0)||((USER/100)==9)//���Ժ,����
		//���Ķ�ȡ
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//��ʼ�ַ���68H��  1Byte
				p8Data[0]=0x68;
			//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
			//������C  1Byte
				p8Data[3]=0x43;
			//��ַ��A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//�ͻ�����ַCA  1Byte
				p8Data[11]=CA;
			//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
			//APDU
				p8Data[14]=5;//GET-Request
				p8Data[15]=2;//��ȡ���ɸ�������������           [2] GetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				i=4*NUM_OAD;
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//ʱ���ǩ
				p8Data[i]=0;
			//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
			//�����ַ���16H��
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		#else
		//����+������֤���ȡ
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//��ʼ�ַ���68H��  1Byte
				p8Data[0]=0x68;
			//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
			//������C  1Byte
				p8Data[3]=0x43;
			//��ַ��A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//�ͻ�����ַCA  1Byte
				p8Data[11]=CA;
			//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
				p8Data[14]=0x10;
				p8Data[15]=0;//����Ӧ�����ݵ�Ԫ [0] ����Ӧ�����ݵ�Ԫ [1]
				//APDU����;octet-string
			//APDU
				p8Data[14+3]=5;//GET-Request
				p8Data[15+3]=2;//��ȡ���ɸ�������������           [2] GetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16+3]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17+3]=NUM_OAD;
				i=4*NUM_OAD;
				MR((u32)p8Data+18+3,(u32)p8rx+offset,i);
				offset+=i;
				i+=18+3;
			//ʱ���ǩ
				p8Data[i]=0;
				i++;
				p8Data[16]=i-17;//APDU����;octet-string
			//�����
				p8Data[i]=1;//����� [1]  RN
				p8Data[i+1]=16;//����
				i+=2;
				//16byte�����
				x=TESAM_CMD_TxRx(0x80040010,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(x)
				{
					UART7Ctrl->Task=0;//�ص�Դ����
				}
				MR((u32)p8Data+i,ADDR_UART7Rx+5,16);
				MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//����ȫģʽʱ���͸�����16�ֽ������
				i+=16;
			//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
			//�����ַ���16H��
				i+=3;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		#endif

				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//���ɿ�
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
						if(UARTCtrlProxy->Task==2)//�ȴ���֡���ݷ���
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//������
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						NUM_ReTx=0;//�����ط�����
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
						while(1)
						{
					#ifndef LINUX
						p32=(u32*)ADDR_MS_ENTRY;		
						(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
					#else
						ms=(ms_Type *)Get_ms();
						ms->Main_Thread();
					#endif
							
							if(UARTCtrlProxy->ProxyTask==0)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//�����ݽ���
									if(NUM_ReTx<NUMmax_ReTx)//�����ط�����
									{
										NUM_ReTx++;//�����ط�����
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
										continue;
									}
								}
								break;
							}
						}
					}
				//����������
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//�����ݽ���
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(6*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD+(i*4),4);
							p8tx[LEN_TxAPDU+4]=0;
							p8tx[LEN_TxAPDU+5]=33;//����ʱ 
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
					}
					else
					{
						p8Data-=4;//ǰ��p8Data+=4;
					#if ((USER%100)==0)||((USER/100)==9)//���Ժ,����
						//���Ķ�ȡ
					#else
						//����+������֤���ȡ
						i=p8Data[1]+(p8Data[2]<<8);
						if(i>=(14+3))
						{
							i-=(14+3);
						}
						if((p8Data[16]&0x80)==0)
						{
							MR((u32)p8Data+14,(u32)p8Data+14+3,i);
						}
						else
						{
							if((p8Data[16]&0x7f)==1)
							{//1byte����
								MR((u32)p8Data+14,(u32)p8Data+14+4,i);
							}
							else
							{//2byte����
								MR((u32)p8Data+14,(u32)p8Data+14+5,i);
							}
						}
					#endif
						NUM_OAD=p8Data[17];//����OAD����
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
							if(p8Data[i+4]==0)
							{//0=���ش���
								if(LENmax_TxSPACE<6)
								{
									LEN_TxAPDU=0;//���ͻ��岻��
									return;
								}
								MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,6);
								i+=6;
								LEN_TxAPDU+=6;
								LENmax_TxSPACE-=6;
							}
							else
							{//1=����data
								x=Get_DL698DataLen_S(p8Data+i+5,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
								x+=5;
								if(LENmax_TxSPACE<x)
								{
									LEN_TxAPDU=0;//���ͻ��岻��
									return;
								}
								MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
								i+=x;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
						}
					}
				}
				else
				{//Err=1
					NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
					if(LENmax_TxSPACE<(1+(6*NUM_OAD)))
					{
						LEN_TxAPDU=0;//���ͻ��岻��
						break;
					}
					p8tx[LEN_TxAPDU]=NUM_OAD;
					LEN_TxAPDU+=1;
					LENmax_TxSPACE-=1;
					p8OAD++;
					for(i=0;i<NUM_OAD;i++)
					{
						MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD+(i*4),4);
						p8tx[LEN_TxAPDU+4]=0;
						p8tx[LEN_TxAPDU+5]=255;//��������
						LEN_TxAPDU+=6;
						LENmax_TxSPACE-=6;
					}
				}
				p8tx[3]++;//����������+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 2://�����ȡһ����������һ����¼�Ͷ�����������           [2] ProxyGetRequestRecord��
//ProxyGetRequestRecord��=SEQUENCE
//{
//  �������-���ȼ�     PIID��
//  ��������ĳ�ʱʱ��  long-unsigned��
//  Ŀ���������ַ      TSA��
//  ��������������      OAD��
//  ��¼ѡ��������      RSD��
//  ��¼��ѡ��������    RCSD
//}
//68 2E 00 43 05 04 00 00 00 00 00 10 71 7C 09 02 0B 
//00 1E  ��������ĳ�ʱʱ��  long-unsigned
//07 05 00 00 00 00 00 01  TSA
//50 04 02 00  OAD
//09 01  RSD
//02 00 00 10 02 00 00 00 10 02 00  RCSD
//00 31 07 16 
//ProxyGetResponseRecord��=SEQUENCE
//{
//  �������-���ȼ�-ACD          PIID-ACD��
//  Ŀ���������ַ               TSA��
//  һ����¼�Ͷ������Լ�����   A-ResultRecord
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=137;//������Ӧ        	[137]	PROXY-Response
			p8tx[1]=2;//[2] ProxyGetRequestRecord
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//������������ĳ�ʱʱ��
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//�õ�UART�붨ʱ����ַ
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			// һ��Ŀ���������ַ         TSA
			p16=(u16*)(ADDR_DATABUFF);
			p16[0]=0;
			p16[1]=0;
			p16[2]=0;
			i=p8rx[offset+1];//TSA�ֽ���
			i++;
			MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
			UARTCtrl->RMAddr[0]=p16[0];
			UARTCtrl->RMAddr[1]=p16[1];
			UARTCtrl->RMAddr[2]=p16[2];
			
			SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
			Err=1;//�ô���
			if(SETno<NUM_RMmax)
			{//�ҵ��������
				p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
				if(p8[0]==DataType_structure)
				{//������Ч
					p8=Get_Element(p8+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//�ҵ��˿ں�
						ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
						ProxyPORTn=OADtoPORTn(ProxyPORTn);//OADֵתΪͨ�ſں�
						if(ProxyPORTn==ACSAMPLEPORT)
						{
							UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
							Err=0;
						}
					#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
						if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
					#endif
					#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
						if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
					#endif
					#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
						if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
					#endif
					#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
						if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
					#endif
						{
							UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
							if(UARTCtrlProxy->Function)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
							{
								if(UARTCtrlProxy->AutoSearchTask==0)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
								{
									UARTCtrlProxy->ProxyPORTn=PORTn;//�������Ķ˿ں�
									p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
									p8=Get_Element(p8+5,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
									if(p8)
									{//������
										i=p8[1];
										switch(i)
										{
											case 0://300
											case 1://600
											case 2://1200
											case 3://2400
											case 4://4800
											case 5://7200
											case 6://9600
											case 7://19200
											case 8://38400
											case 9://57600
											case 10://115200
												UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
												break;
											case 255://����Ӧ
											default:
												UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
												break;
										}
										UARTCtrlProxy->ProxyRMprotocol=p8[3];//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
									}
									Err=0;
								}
							}
						}
					}
				}
			}
			i=p8rx[offset];//TSA�ֽ���
			i++;
			if(LENmax_TxSPACE<i)
			{
				LEN_TxAPDU=0;//���ͻ��岻��
				break;
			}
			MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
			offset+=i;
			LEN_TxAPDU+=i;
			LENmax_TxSPACE-=i;
			//����һ���������ĳ�ʱʱ��
			if(Err==0)
			{
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//�õ�UART�붨ʱ����ַ
				i=p16[0];
				UARTCtrlProxy->ProxyOverTime=i;//����1���������ĳ�ʱʱ����
			}
			//OAD
			p8OAD=p8rx+offset;//��ǰ�跢��OAD��������ֵָ��
		//�鷢��֡
		#if ((USER%100)==0)||((USER/100)==9)//���Ժ,����
		//���Ķ�ȡ
			p8Data[0]=0xfe;
			p8Data[1]=0xfe;
			p8Data[2]=0xfe;
			p8Data[3]=0xfe;
			p8Data+=4;
		//��ʼ�ַ���68H��  1Byte
			p8Data[0]=0x68;
		//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
		//������C  1Byte
			p8Data[3]=0x43;
		//��ַ��A
			p8Data[4]=5;
			p8Data[5]=UARTCtrl->RMAddr[2]>>8;
			p8Data[6]=UARTCtrl->RMAddr[2];
			p8Data[7]=UARTCtrl->RMAddr[1]>>8;
			p8Data[8]=UARTCtrl->RMAddr[1];
			p8Data[9]=UARTCtrl->RMAddr[0]>>8;
			p8Data[10]=UARTCtrl->RMAddr[0];
		//�ͻ�����ַCA  1Byte
			p8Data[11]=CA;
		//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
		//APDU
			p8Data[14]=5;//GET-Request
			p8Data[15]=3;//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord
			UARTCtrl->PIID++;
			p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
			i=DataLen_RSD_NoMS(p8rx+offset+4,0,0);//������MS���Ȳ���
			i>>=8;
			i--;//����������
			LEN_RSD=i;
			LEN_RCSD=p8rx[offset+4+i];//RCSD����
			LEN_RCSD*=5;
			LEN_RCSD+=1;
			i=4+LEN_RSD+LEN_RCSD;
			MR((u32)p8Data+17,(u32)p8rx+offset,i);
			offset+=i;
			i+=17;
		//ʱ���ǩ
			p8Data[i]=0;
		//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
		//�����ַ���16H��
			i+=4;
			i-=2;
			p8Data[1]=i;
			p8Data[2]=i>>8;
			Uart_698_HCSFCS(p8Data);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		#else
		//����+������֤���ȡ
			p8Data[0]=0xfe;
			p8Data[1]=0xfe;
			p8Data[2]=0xfe;
			p8Data[3]=0xfe;
			p8Data+=4;
		//��ʼ�ַ���68H��  1Byte
			p8Data[0]=0x68;
		//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
		//������C  1Byte
			p8Data[3]=0x43;
		//��ַ��A
			p8Data[4]=5;
			p8Data[5]=UARTCtrl->RMAddr[2]>>8;
			p8Data[6]=UARTCtrl->RMAddr[2];
			p8Data[7]=UARTCtrl->RMAddr[1]>>8;
			p8Data[8]=UARTCtrl->RMAddr[1];
			p8Data[9]=UARTCtrl->RMAddr[0]>>8;
			p8Data[10]=UARTCtrl->RMAddr[0];
		//�ͻ�����ַCA  1Byte
			p8Data[11]=CA;
		//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
			p8Data[14]=0x10;
			p8Data[15]=0;//����Ӧ�����ݵ�Ԫ [0] ����Ӧ�����ݵ�Ԫ [1]
			//APDU����;octet-string
		//APDU
			p8Data[14+3]=5;//GET-Request
			p8Data[15+3]=3;//��ȡһ����¼�Ͷ�����������       [3] GetRequestRecord
			UARTCtrl->PIID++;
			p8Data[16+3]=UARTCtrl->PIID&0x3f;//PIID
			i=DataLen_RSD_NoMS(p8rx+offset+4,0,0);//������MS���Ȳ���
			i>>=8;
			i--;//����������
			LEN_RSD=i;
			LEN_RCSD=p8rx[offset+4+i];//RCSD����
			LEN_RCSD*=5;
			LEN_RCSD+=1;
			i=4+LEN_RSD+LEN_RCSD;
			MR((u32)p8Data+17+3,(u32)p8rx+offset,i);
			offset+=i;
			i+=17+3;
		//ʱ���ǩ
			p8Data[i]=0;
			i++;
			p8Data[16]=i-17;//APDU����;octet-string
		//�����
			p8Data[i]=1;//����� [1]  RN
			p8Data[i+1]=16;//����
			i+=2;
			//16byte�����
			x=TESAM_CMD_TxRx(0x80040010,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(x)
			{
				UART7Ctrl->Task=0;//�ص�Դ����
			}
			MR((u32)p8Data+i,ADDR_UART7Rx+5,16);
			MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//����ȫģʽʱ���͸�����16�ֽ������
			i+=16;
		//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
		//�����ַ���16H��
			i+=3;
			i-=2;
			p8Data[1]=i;
			p8Data[2]=i>>8;
			Uart_698_HCSFCS(p8Data);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		#endif
		
			if(Err==0)
			{
				if(ProxyPORTn==ACSAMPLEPORT)
				{//���ɿ�
					x=Get_ADDR_UARTnRx(ProxyPORTn);
					MW((u32)p8Data,x,i+2);
					Uart_698_Data(ProxyPORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
					if(UARTCtrlProxy->Task==2)//�ȴ���֡���ݷ���
					{
						UARTCtrlProxy->Task=0;
						x=UARTCtrlProxy->TxByte;
						UARTCtrlProxy->TxByte=0;
						UARTCtrlProxy->ProxyTxRxByte=x;
						MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
					}
					else
					{
						UARTCtrlProxy->ProxyTxRxByte=0;
					}
				}
				else
				{//������
					UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
					UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
					NUM_ReTx=0;//�����ط�����
					p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
					while(1)
					{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
						
						if(UARTCtrlProxy->ProxyTask==0)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						{
							if(UARTCtrlProxy->ProxyTxRxByte==0)
							{//�����ݽ���
								if(NUM_ReTx<NUMmax_ReTx)//�����ط�����
								{
									NUM_ReTx++;//�����ط�����
									UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
									UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
									continue;
								}
							}
							break;
						}
					}
				}
			//����������
				if(UARTCtrlProxy->ProxyTxRxByte==0)
				{//�����ݽ���
					if(LENmax_TxSPACE<(4+LEN_RCSD+2))
					{
						LEN_TxAPDU=0;//���ͻ��岻��
						break;
					}
					MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);//OAD
					MR((u32)p8tx+LEN_TxAPDU+4,(u32)p8OAD+4+LEN_RSD,LEN_RCSD);//RCSD
					LEN_TxAPDU+=4+LEN_RCSD;
					p8tx[LEN_TxAPDU+0]=0;
					p8tx[LEN_TxAPDU+1]=33;//����ʱ 
					LEN_TxAPDU+=2;
					LENmax_TxSPACE-=4+LEN_RCSD+2;
				}
				else
				{
					p8Data-=4;//ǰ��p8Data+=4;
				#if ((USER%100)==0)||((USER/100)==9)//���Ժ,����
					//���Ķ�ȡ
				#else
					//����+������֤���ȡ
					i=p8Data[1]+(p8Data[2]<<8);
					if(i>=(14+3))
					{
						i-=(14+3);
					}
					if((p8Data[16]&0x80)==0)
					{
						MR((u32)p8Data+14,(u32)p8Data+14+3,i);
					}
					else
					{
						if((p8Data[16]&0x7f)==1)
						{//1byte����
							MR((u32)p8Data+14,(u32)p8Data+14+4,i);
						}
						else
						{//2byte����
							MR((u32)p8Data+14,(u32)p8Data+14+5,i);
						}
					}
				#endif
					i=17;
					NUM_OAD=p8Data[17+4];//RCSD����
					LEN_RCSD=5*NUM_OAD;
					LEN_RCSD+=1;
					if(p8Data[17+4+LEN_RCSD]==0)
					{//0=���ش���
						if(LENmax_TxSPACE<(4+LEN_RCSD+2))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							return;
						}
						MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+17,4+LEN_RCSD+2);
						LEN_TxAPDU+=4+LEN_RCSD+2;
						LENmax_TxSPACE-=4+LEN_RCSD+2;
					}
					else
					{//1=����data
						x=p8Data[17+4+LEN_RCSD+1];//M����¼
						LEN_RCSD+=4+2;
						while(x--)
						{
							for(i=0;i<NUM_OAD;i++)//RCSD����
							{
								offset=Get_DL698DataLen_S(p8Data+17+LEN_RCSD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
								LEN_RCSD+=offset;
							}
						}
						if(LENmax_TxSPACE<LEN_RCSD)
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							return;
						}
						MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+17,LEN_RCSD);
						LEN_TxAPDU+=LEN_RCSD;
						LENmax_TxSPACE-=LEN_RCSD;
					}
				}
			}
			else
			{//Err=1
					if(LENmax_TxSPACE<(4+LEN_RCSD+2))
					{
						LEN_TxAPDU=0;//���ͻ��岻��
						break;
					}
					MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);//OAD
					MR((u32)p8tx+LEN_TxAPDU+4,(u32)p8OAD+4+LEN_RSD,LEN_RCSD);//RCSD
					LEN_TxAPDU+=4+LEN_RCSD;
					p8tx[LEN_TxAPDU+0]=0;
					p8tx[LEN_TxAPDU+1]=255;//��������
					LEN_TxAPDU+=2;
					LENmax_TxSPACE-=4+LEN_RCSD+2;
			}
			break;
		case 3://�����������ɸ������������ɸ�������������             [3] ProxySetRequestList��
//ProxySetRequestList��=SEQUENCE
//{
//  �������-���ȼ�         PIID��
//  ������������ĳ�ʱʱ��  long-unsigned��
//  �������ɸ��������Ķ�����������  SEQUENCE OF
//  {
//    һ��Ŀ���������ַ        TSA��
//    ����һ���������ĳ�ʱʱ��  long-unsigned��
//    ���ɸ�����������������������  SEQUENCE OF
//    {
//      ��������������  OAD��
//      ��������        Data
//    }
//  }
//}		
//68 2D 00 43 05 04 00 00 00 00 00 10 C2 82 09 03 0A 
//00 1E  ������������ĳ�ʱʱ��  long-unsigned
//01   �������ɸ��������Ķ�����������  SEQUENCE OF
//07 05 00 00 00 00 00 01  һ��Ŀ���������ַ        TSA
//00 00  ����һ���������ĳ�ʱʱ��  long-unsigned
//01  ���ɸ�����������������������  SEQUENCE OF
//40 00 02 00 ��������������  OAD
//1C 07 E1 07 02 0D 22 02  ��������        Data
//00 E9 6B 16 
//ProxySetResponseList��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  �������ɸ��������Ķ�ȡ���  SEQUENCE OF
//  {
//һ��Ŀ���������ַ  TSA��
//���ɸ���������������������  SEQUENCE OF
//{
//      ��������������  OAD��
//      �������ý��    DAR
//}
//  }
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=137;//������Ӧ        	[137]	PROXY-Response
			p8tx[1]=3;// [3] ProxySetRequestList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//������������ĳ�ʱʱ��
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//�õ�UART�붨ʱ����ַ
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//����������
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// һ��Ŀ���������ַ         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA�ֽ���
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
				Err=1;//�ô���
				if(SETno<NUM_RMmax)
				{//�ҵ��������
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//������Ч
						p8=Get_Element(p8+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
						if(p8)
						{//�ҵ��˿ں�
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OADֵתΪͨ�ſں�
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//�������Ķ˿ں�
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
										if(p8)
										{//������
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://����Ӧ
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA�ֽ���
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//���ͻ��岻��
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//����һ���������ĳ�ʱʱ��
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//����1���������ĳ�ʱʱ����
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//��ǰ�跢��OAD��������ֵָ��
				NUM_OAD=p8rx[offset];
				offset+=1;
			//�鷢��֡
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//��ʼ�ַ���68H��  1Byte
				p8Data[0]=0x68;
			//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
			//������C  1Byte
				p8Data[3]=0x43;
			//��ַ��A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//�ͻ�����ַCA  1Byte
				p8Data[11]=CA;
			//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
			//APDU
				p8Data[14]=6;//�������� 			 [6]		SET-Request
				p8Data[15]=2;//�������ɸ�������������     	[2] SetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				
				i=0;
				for(x=0;x<NUM_OAD;x++)
				{
					LEN_RSD=Get_DL698DataLen_S(p8rx+offset+i+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					i+=4+LEN_RSD;
				}
				if(i>(LEN_UART2Tx-(4+18+4)))
				{
					LEN_TxAPDU=0;
					break;
				}
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//ʱ���ǩ
				p8Data[i]=0;
			//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
			//�����ַ���16H��
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
					
				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//���ɿ�
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
						if(UARTCtrlProxy->Task==2)//�ȴ���֡���ݷ���
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//������
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						NUM_ReTx=0;//�����ط�����
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
						while(1)
						{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
							if(UARTCtrlProxy->ProxyTask==0)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//�����ݽ���
									if(NUM_ReTx<NUMmax_ReTx)//�����ط�����
									{
										NUM_ReTx++;//�����ط�����
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
										continue;
									}
								}
								break;
							}
						}
					}
				//����������
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//�����ݽ���
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(5*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=33;//����ʱ 
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
						}
					}
					else
					{
						p8Data-=4;//ǰ��p8Data+=4;
						NUM_OAD=p8Data[17];//����OAD����
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
							if(LENmax_TxSPACE<5)
							{
								LEN_TxAPDU=0;//���ͻ��岻��
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,5);//4�ֽ�OAD+DAR
							i+=5;
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
						}
					}
				}
				else
				{//Err=1
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(5*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=255;//��������
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
						}
				}
				p8tx[3]++;//����������+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 4://�������ú��ȡ���ɸ������������ɸ�������������       [4] ProxySetThenGetRequestList��
//ProxySetThenGetRequestList��=SEQUENCE
//{
//  �������-���ȼ�          PIID��
//  ������������ĳ�ʱʱ��   long-unsigned��
//  �������ɸ��������Ķ����������ú��ȡ  SEQUENCE OF
//  {
//    һ��Ŀ���������ַ          TSA��
//    ����һ���������ĳ�ʱʱ��    long-unsigned��
//    ���ɸ��������Ե����ú��ȡ  SEQUENCE OF
//   {
//      ���õĶ�������������   OAD��
//      ����������ֵ           Data��
//      ��ȡ�Ķ�������������   OAD��
//      ������ʱ��ȡʱ��       unsigned
//    }
//  }
//}
//68 32 00 43 05 04 00 00 00 00 00 10 CB EF 09 04 0B 
//00 1E ������������ĳ�ʱʱ��   long-unsigned
//01  �������ɸ��������Ķ����������ú��ȡ  SEQUENCE OF
//07 05 00 00 00 00 00 01 һ��Ŀ���������ַ          TSA
//00 00  ����һ���������ĳ�ʱʱ��    long-unsigned
//01  ���ɸ��������Ե����ú��ȡ  SEQUENCE OF
//40 00 02 00  ���õĶ�������������   OAD
//1C 07 E1 07 02 0E 32 1A ����������ֵ           Data
//40 00 02 00  ��ȡ�Ķ�������������   OAD
//00  ������ʱ��ȡʱ��       unsigned
//00 98 CC 16 
//ProxySetThenGetResponseList��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  �������ɸ������������ú��ȡ���  SEQUENCE OF
//  {
//һ��Ŀ���������ַ  TSA��
//���ɸ������������ú��ȡ���  SEQUENCE OF
//{
//      ���õĶ�������������  OAD��
//      �������ý��          DAR��
//һ���������Լ�����  A-ResultNormal 
//}
//  }
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=137;//������Ӧ        	[137]	PROXY-Response
			p8tx[1]=4;// [4] ProxySetThenGetRequestList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//������������ĳ�ʱʱ��
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//�õ�UART�붨ʱ����ַ
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//����������
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// һ��Ŀ���������ַ         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA�ֽ���
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
				Err=1;//�ô���
				if(SETno<NUM_RMmax)
				{//�ҵ��������
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//������Ч
						p8=Get_Element(p8+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
						if(p8)
						{//�ҵ��˿ں�
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OADֵתΪͨ�ſں�
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//�������Ķ˿ں�
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
										if(p8)
										{//������
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://����Ӧ
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA�ֽ���
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//���ͻ��岻��
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//����һ���������ĳ�ʱʱ��
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//����1���������ĳ�ʱʱ����
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//��ǰ�跢��OAD��������ֵָ��
				NUM_OAD=p8rx[offset];
				offset+=1;
			//�鷢��֡
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//��ʼ�ַ���68H��  1Byte
				p8Data[0]=0x68;
			//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
			//������C  1Byte
				p8Data[3]=0x43;
			//��ַ��A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//�ͻ�����ַCA  1Byte
				p8Data[11]=CA;
			//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
			//APDU
				p8Data[14]=6;//�������� 			 [6]		SET-Request
				p8Data[15]=3;//[3] SetThenGetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				
				i=0;
				for(x=0;x<NUM_OAD;x++)
				{
// ���ɸ����ú��ȡ��������  SEQUENCE OF
// {
//		һ�����õĶ�������   OAD��
//		����                 Data��
//		һ����ȡ�Ķ�������   OAD��
//		��ʱ��ȡʱ��         unsigned
// }
					LEN_RSD=Get_DL698DataLen_S(p8rx+offset+i+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					i+=4+LEN_RSD+5;
				}
				if(i>(LEN_UART2Tx-(4+18+4)))
				{
					LEN_TxAPDU=0;
					break;
				}
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//ʱ���ǩ
				p8Data[i]=0;
			//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
			//�����ַ���16H��
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
					
				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//���ɿ�
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
						if(UARTCtrlProxy->Task==2)//�ȴ���֡���ݷ���
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//������
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						NUM_ReTx=0;//�����ط�����
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
						while(1)
						{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
							if(UARTCtrlProxy->ProxyTask==0)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//�����ݽ���
									if(NUM_ReTx<NUMmax_ReTx)//�����ط�����
									{
										NUM_ReTx++;//�����ط�����
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
										continue;
									}
								}
								break;
							}
						}
					}
				//����������
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//�����ݽ���
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(11*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=33;//����ʱ 
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=0;//0=����
							p8tx[LEN_TxAPDU+5]=33;//����ʱ 
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
					}
					else
					{//�����ݽ���
						p8Data-=4;//ǰ��p8Data+=4;
						NUM_OAD=p8Data[17];//����OAD����
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
//      ���õĶ�������������  OAD��
//      �������ý��          DAR��
							if(LENmax_TxSPACE<5)
							{
								LEN_TxAPDU=0;//���ͻ��岻��
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,5);//4�ֽ�OAD+DAR
							i+=5;
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
//һ���������Լ�����  A-ResultNormal 
							if(p8Data[i+4]==0)
							{//���ش���
								x=6;//4�ֽ�OAD+2�ֽ�DAR
							}
							else
							{//��������
								x=Get_DL698DataLen_S(p8Data+i+4+1,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
								x+=4+1;
							}
							if(LENmax_TxSPACE<x)
							{
								LEN_TxAPDU=0;//���ͻ��岻��
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
							i+=x;
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
						}
					}
				}
				else
				{//Err=1
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(11*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=255;//��������
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=0;//0=����
							p8tx[LEN_TxAPDU+5]=255;//��������
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
				}
				p8tx[3]++;//����������+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 5://����������ɸ������������ɸ����󷽷�����             [5] ProxyActionRequestList��
//ProxyActionRequestList��=SEQUENCE
//{
//  �������-���ȼ�         PIID��
//  ������������ĳ�ʱʱ��  long-unsigned��
//  �������ɸ��������Ķ��󷽷�����  SEQUENCE OF
//  {
//    һ��Ŀ���������ַ            TSA��
//    ����һ���������ĳ�ʱʱ��      long-unsigned��
//    ���ɸ����󷽷��������������  SEQUENCE OF
//    {
//      ���󷽷�������  OMD��
//      ���䷽������    Data
//    }
//  }
//}
//68 26 00 43 05 04 00 00 00 00 00 10 DB C0 09 05 0B 
//00 1E ������������ĳ�ʱʱ��  long-unsigned
//01 �������ɸ��������Ķ��󷽷�����  SEQUENCE OF
//07 05 00 00 00 00 00 01 һ��Ŀ���������ַ            TSA
//00 00 ����һ���������ĳ�ʱʱ��      long-unsigned
//01 ���ɸ����󷽷��������������  SEQUENCE OF
//F3 00 04 00 ���󷽷�������  OMD
//00 ���䷽������    Data
//00 6D FB 16 
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=137;//������Ӧ        	[137]	PROXY-Response
			p8tx[1]=5;// [5] ProxyActionRequestList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//������������ĳ�ʱʱ��
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//�õ�UART�붨ʱ����ַ
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//����������
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// һ��Ŀ���������ַ         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA�ֽ���
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
				Err=1;//�ô���
				if(SETno<NUM_RMmax)
				{//�ҵ��������
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//������Ч
						p8=Get_Element(p8+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
						if(p8)
						{//�ҵ��˿ں�
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OADֵתΪͨ�ſں�
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//�������Ķ˿ں�
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
										if(p8)
										{//������
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://����Ӧ
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA�ֽ���
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//���ͻ��岻��
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//����һ���������ĳ�ʱʱ��
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//����1���������ĳ�ʱʱ����
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//��ǰ�跢��OAD��������ֵָ��
				NUM_OAD=p8rx[offset];
				offset+=1;
			//�鷢��֡
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//��ʼ�ַ���68H��  1Byte
				p8Data[0]=0x68;
			//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
			//������C  1Byte
				p8Data[3]=0x43;
			//��ַ��A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//�ͻ�����ַCA  1Byte
				p8Data[11]=CA;
			//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
			//APDU
				p8Data[14]=7;//�������� 			 [7]		ACTION-Request
				p8Data[15]=2;//�������ɸ����󷽷�����                  	     [2] ActionRequestList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				
				i=0;
				for(x=0;x<NUM_OAD;x++)
				{
					LEN_RSD=Get_DL698DataLen_S(p8rx+offset+i+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					i+=4+LEN_RSD;
				}
				if(i>(LEN_UART2Tx-(4+18+4)))
				{
					LEN_TxAPDU=0;
					break;
				}
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//ʱ���ǩ
				p8Data[i]=0;
			//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
			//�����ַ���16H��
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
					
				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//���ɿ�
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
						if(UARTCtrlProxy->Task==2)//�ȴ���֡���ݷ���
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//������
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						NUM_ReTx=0;//�����ط�����
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
						while(1)
						{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
							if(UARTCtrlProxy->ProxyTask==0)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//�����ݽ���
									if(NUM_ReTx<NUMmax_ReTx)//�����ط�����
									{
										NUM_ReTx++;//�����ط�����
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
										continue;
									}
								}
								break;
							}
						}
					}
				//����������
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//�����ݽ���
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(6*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=33;//����ʱ 
							p8tx[LEN_TxAPDU+5]=0;//������������Data  OPTIONAL
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
					}
					else
					{
						p8Data-=4;//ǰ��p8Data+=4;
						NUM_OAD=p8Data[17];//����OAD����
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
							if(LENmax_TxSPACE<5)
							{
								LEN_TxAPDU=0;//���ͻ��岻��
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,5);//4�ֽ�OAD+DAR
							i+=5;
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
							//������������Data  OPTIONAL
//							if(p8Data[i]==0)
//							{
//								x=1;
//							}
//							else
//							{
//								x=Get_DL698DataLen_S(p8Data+i+1,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
//								x+=1;
//							}
							x=Get_DL698DataLen_S(p8Data+i,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							
							
							if(LENmax_TxSPACE<x)
							{
								LEN_TxAPDU=0;//���ͻ��岻��
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
							i+=x;
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
						}
					}
				}
				else
				{//Err=1
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(6*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=255;//��������
							p8tx[LEN_TxAPDU+5]=0;//������������Data  OPTIONAL
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
				}
				p8tx[3]++;//����������+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 6://����������ȡ���ɸ������������ɸ����󷽷����������� [6] ProxyActionThenGetRequestList��
//ProxyActionThenGetRequestList��=SEQUENCE
//{
//  �������-���ȼ�               PIID��
//  ������������ĳ�ʱʱ��        long-unsigned��
//  �������ɸ��������Ĳ������ȡ  SEQUENCE OF
//  {
//    һ��Ŀ���������ַ          TSA��
//    ����һ���������ĳ�ʱʱ��    long-unsigned��
//    ���ɸ����󷽷������ԵĲ������ȡ  SEQUENCE OF
//    {
//      �����Ķ��󷽷�������   OMD��
//      ���䷽������           Data��
//      ��ȡ�Ķ�������������   OAD��
//      ������ʱ��ȡʱ��       unsigned
//    }
//  }
//}
//68 2B 00 43 05 04 00 00 00 00 00 10 B5 77 09 06 0B 
//00 1E ������������ĳ�ʱʱ��        long-unsigned
//01 �������ɸ��������Ĳ������ȡ  SEQUENCE OF
//07 05 00 00 00 00 00 01 һ��Ŀ���������ַ          TSA
//00 00 ����һ���������ĳ�ʱʱ��    long-unsigned
//01 ���ɸ����󷽷������ԵĲ������ȡ  SEQUENCE OF
//F3 00 04 00 �����Ķ��󷽷�������   OMD
//00 ���䷽������           Data
//40 00 02 00 ��ȡ�Ķ�������������   OAD
//3C ������ʱ��ȡʱ��       unsigned
//00 F3 77 16 
//ActionThenGetResponseNormalList��=SEQUENCE
//{
//  �������-���ȼ�-ACD   PIID-ACD��
//  �������ɸ����󷽷����ȡ���ԵĽ��  SEQUENCE OF
//  {
//һ�����õĶ��󷽷�������   OMD��
//     ����ִ�н��               DAR��
//     ������������               Data OPTIONAL��
//һ���������Լ�����        A-ResultNormal 
//}
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=137;//������Ӧ        	[137]	PROXY-Response
			p8tx[1]=6;//[6] ProxyActionThenGetRequestList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//������������ĳ�ʱʱ��
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//�õ�UART�붨ʱ����ַ
			i=(p8rx[offset]<<8)|p8rx[offset+1];
			p16[0]=i;
			offset+=2;
			//����������
			p8tx[3]=0;
			NUM_TSA=p8rx[offset];
			offset++;
			LEN_TxAPDU++;
			LENmax_TxSPACE--;
			while(NUM_TSA--)
			{
				// һ��Ŀ���������ַ         TSA
				p16=(u16*)(ADDR_DATABUFF);
				p16[0]=0;
				p16[1]=0;
				p16[2]=0;
				i=p8rx[offset+1];//TSA�ֽ���
				i++;
				MR(ADDR_DATABUFF,(u32)p8rx+offset+2,i);
				UARTCtrl->RMAddr[0]=p16[0];
				UARTCtrl->RMAddr[1]=p16[1];
				UARTCtrl->RMAddr[2]=p16[2];
				
				SETno=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
				Err=1;//�ô���
				if(SETno<NUM_RMmax)
				{//�ҵ��������
					p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
					if(p8[0]==DataType_structure)
					{//������Ч
						p8=Get_Element(p8+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
						if(p8)
						{//�ҵ��˿ں�
							ProxyPORTn=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
							ProxyPORTn=OADtoPORTn(ProxyPORTn);//OADֵתΪͨ�ſں�
							if(ProxyPORTn==ACSAMPLEPORT)
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								Err=0;
							}
						#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
						#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT)||(ProxyPORTn==RS485_4PORT))
						#endif
						#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
						#endif
							{
								UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
								if(UARTCtrlProxy->Function)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
								{
									if(UARTCtrlProxy->AutoSearchTask==0)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
									{
										UARTCtrlProxy->ProxyPORTn=PORTn;//�������Ķ˿ں�
										p8=(u8*)ADDR_6000_SDRAM+(SETno*LENper_6000);
										p8=Get_Element(p8+5,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
										if(p8)
										{//������
											i=p8[1];
											switch(i)
											{
												case 0://300
												case 1://600
												case 2://1200
												case 3://2400
												case 4://4800
												case 5://7200
												case 6://9600
												case 7://19200
												case 8://38400
												case 9://57600
												case 10://115200
													UARTCtrlProxy->ProxyBpsCtrl=(0xb+(i<<5));
													break;
												case 255://����Ӧ
												default:
													UARTCtrlProxy->ProxyBpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
													break;
											}
											UARTCtrlProxy->ProxyRMprotocol=p8[3];//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
										}
										Err=0;
									}
								}
							}
						}
					}
				}
				i=p8rx[offset];//TSA�ֽ���
				i++;
				if(LENmax_TxSPACE<i)
				{
					LEN_TxAPDU=0;//���ͻ��岻��
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,i);
				offset+=i;
				LEN_TxAPDU+=i;
				LENmax_TxSPACE-=i;
				//����һ���������ĳ�ʱʱ��
				i=(p8rx[offset]<<8)|p8rx[offset+1];
				if(Err==0)
				{
					UARTCtrlProxy->ProxyOverTime=i;//����1���������ĳ�ʱʱ����
				}
				offset+=2;
				//SEQUENCE OF OAD
				p8OAD=p8rx+offset;//��ǰ�跢��OAD��������ֵָ��
				NUM_OAD=p8rx[offset];
				offset+=1;
			//�鷢��֡
				p8Data[0]=0xfe;
				p8Data[1]=0xfe;
				p8Data[2]=0xfe;
				p8Data[3]=0xfe;
				p8Data+=4;
			//��ʼ�ַ���68H��  1Byte
				p8Data[0]=0x68;
			//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
			//������C  1Byte
				p8Data[3]=0x43;
			//��ַ��A
				p8Data[4]=5;
				p8Data[5]=UARTCtrl->RMAddr[2]>>8;
				p8Data[6]=UARTCtrl->RMAddr[2];
				p8Data[7]=UARTCtrl->RMAddr[1]>>8;
				p8Data[8]=UARTCtrl->RMAddr[1];
				p8Data[9]=UARTCtrl->RMAddr[0]>>8;
				p8Data[10]=UARTCtrl->RMAddr[0];
			//�ͻ�����ַCA  1Byte
				p8Data[11]=CA;
			//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
			//APDU
				p8Data[14]=7;//�������� 			 [7]		ACTION-Request
				p8Data[15]=3;//[3] ActionThenGetRequestNormalList
				UARTCtrl->PIID++;
				p8Data[16]=UARTCtrl->PIID&0x3f;//PIID
				p8Data[17]=NUM_OAD;
				
				i=0;
				for(x=0;x<NUM_OAD;x++)
				{
//      �����Ķ��󷽷�������   OMD��
//      ���䷽������           Data��
//      ��ȡ�Ķ�������������   OAD��
//      ������ʱ��ȡʱ��       unsigned
					LEN_RSD=Get_DL698DataLen_S(p8rx+offset+i+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					i+=4+LEN_RSD+5;
				}
				if(i>(LEN_UART2Tx-(4+18+4)))
				{
					LEN_TxAPDU=0;
					break;
				}
				MR((u32)p8Data+18,(u32)p8rx+offset,i);
				offset+=i;
				i+=18;
			//ʱ���ǩ
				p8Data[i]=0;
			//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
			//�����ַ���16H��
				i+=4;
				i-=2;
				p8Data[1]=i;
				p8Data[2]=i>>8;
				Uart_698_HCSFCS(p8Data);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
					
				if(Err==0)
				{
					if(ProxyPORTn==ACSAMPLEPORT)
					{//���ɿ�
						x=Get_ADDR_UARTnRx(ProxyPORTn);
						MW((u32)p8Data,x,i+2);
						Uart_698_Data(ProxyPORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
						if(UARTCtrlProxy->Task==2)//�ȴ���֡���ݷ���
						{
							UARTCtrlProxy->Task=0;
							x=UARTCtrlProxy->TxByte;
							UARTCtrlProxy->TxByte=0;
							UARTCtrlProxy->ProxyTxRxByte=x;
							MR((u32)p8Data-4,Get_ADDR_UARTnTx(ProxyPORTn),x);
						}
						else
						{
							UARTCtrlProxy->ProxyTxRxByte=0;
						}
					}
					else
					{//������
						UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
						UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
						NUM_ReTx=0;//�����ط�����
						p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
						while(1)
						{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
							if(UARTCtrlProxy->ProxyTask==0)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
							{
								if(UARTCtrlProxy->ProxyTxRxByte==0)
								{//�����ݽ���
									if(NUM_ReTx<NUMmax_ReTx)//�����ط�����
									{
										NUM_ReTx++;//�����ط�����
										UARTCtrlProxy->ProxyTxRxByte=i+2+4;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
										UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
										continue;
									}
								}
								break;
							}
						}
					}
				//����������
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//�����ݽ���
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(12*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=33;//����ʱ 
							p8tx[LEN_TxAPDU+5]=0;//������������          Data  OPTIONAL
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=0;//0=����
							p8tx[LEN_TxAPDU+5]=33;//����ʱ 
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
					}
					else
					{
						p8Data-=4;//ǰ��p8Data+=4;
						NUM_OAD=p8Data[17];//����OAD����
						if(LENmax_TxSPACE<1)
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						i=18;
						while(NUM_OAD--)
						{
//  һ�����󷽷�������    OMD��
//  ����ִ�н��          DAR��
							if(LENmax_TxSPACE<5)
							{
								LEN_TxAPDU=0;//���ͻ��岻��
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,5);//4�ֽ�OAD+DAR
							i+=5;
							LEN_TxAPDU+=5;
							LENmax_TxSPACE-=5;
//  ������������          Data  OPTIONAL
//							if(p8Data[i]==0)
//							{
//								x=1;
//							}
//							else
//							{
//								x=Get_DL698DataLen_S(p8Data+i+1,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
//								x+=1;
//							}
							x=Get_DL698DataLen_S(p8Data+i,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							
							if(LENmax_TxSPACE<x)
							{
								LEN_TxAPDU=0;//���ͻ��岻��
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
							i+=x;
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
//һ���������Լ�����  A-ResultNormal 
							if(p8Data[i+4]==0)
							{//���ش���
								x=6;//4�ֽ�OAD+2�ֽ�DAR
							}
							else
							{//��������
								x=Get_DL698DataLen_S(p8Data+i+4+1,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
								x+=4+1;
							}
							if(LENmax_TxSPACE<x)
							{
								LEN_TxAPDU=0;//���ͻ��岻��
								return;
							}
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data+i,x);
							i+=x;
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
						}
					}
				}
				else
				{//Err=1
						NUM_OAD=p8OAD[0];//��ǰ�跢��OAD��������ֵָ��
						if(LENmax_TxSPACE<(1+(12*NUM_OAD)))
						{
							LEN_TxAPDU=0;//���ͻ��岻��
							break;
						}
						p8tx[LEN_TxAPDU]=NUM_OAD;
						LEN_TxAPDU+=1;
						LENmax_TxSPACE-=1;
						p8OAD++;
						for(i=0;i<NUM_OAD;i++)
						{
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=255;//��������
							p8tx[LEN_TxAPDU+5]=0;//������������          Data  OPTIONAL
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
							p8OAD+=4;
							p8OAD+=Get_DL698DataLen_S(p8OAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
							MR((u32)p8tx+LEN_TxAPDU,(u32)p8OAD,4);
							p8tx[LEN_TxAPDU+4]=0;//0=����
							p8tx[LEN_TxAPDU+5]=255;//��������
							LEN_TxAPDU+=6;
							LENmax_TxSPACE-=6;
						}
				}
				p8tx[3]++;//����������+1
				p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//������������ĳ�ʱʱ��
				if(p16[0]==0)
				{
					break;
				}
			}
			break;
		case 7://����͸��ת����������                                 [7] ProxyTransCommandRequest
//ProxyTransCommandRequest��=SEQUENCE
//{
//  �������-���ȼ�               PIID��
//  ����ת���˿�                  OAD��
//  �˿�ͨ�ſ��ƿ�                COMDCB��
//  ���յȴ����ĳ�ʱʱ�䣨�룩   long-unsigned��
//  ���յȴ��ֽڳ�ʱʱ�䣨���룩 long-unsigned��
//  ͸��ת������                 octet-string
//}
//68 3A 00 43 05 01 00 00 00 00 00 10 C2 A3 
//09 07 08 
//F2 01 02 01 ����ת���˿�                  OAD
//COMDCB��=SEQUENCE
//{
//	������    ENUMERATED
//	{
//		300bps��0����   600bps��1����     1200bps��2����
//		2400bps��3����  4800bps��4����    7200bps��5����
//		9600bps��6����  19200bps��7����   38400bps��8����
//		57600bps��9���� 115200bps��10���� ����Ӧ��255��
//	}��
//	У��λ  ENUMERATED {��У�飨0������У�飨1����żУ�飨2��}��
//	����λ  ENUMERATED {5��5����6��6����7��7����8��8��}��
//	ֹͣλ  ENUMERATED {1��1����2��2��}��
//	����    ENUMERATED {��(0)��Ӳ��(1)�����(2)}
//}
//03 COMDCB_������
//02 COMDCB_У��λ
//08 COMDCB_����λ
//01 COMDCB_ֹͣλ
//00 COMDCB_����
//00 0A ���յȴ����ĳ�ʱʱ�䣨�룩   long-unsigned
//00 0A ���յȴ��ֽڳ�ʱʱ�䣨���룩 long-unsigned
//19 68 17 00 43 05 22 22 22 22 22 22 10 C1 D6 05 01 07 00 10 02 00 00 04 01 16  ͸��ת������ octet-string
//00 C4 75 16 

//ProxyTransCommandResponse��=SEQUENCE
//{
//  �������-���ȼ�-ACD      PIID-ACD��  
//  ����ת���˿�             OAD��
//  ͸��ת������ؽ��     TransResult
//}
//TransResult��=CHOICE
//{
//  ������Ϣ      [0] DAR��
//  ��������      [1] octet-string
//}
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=137;//������Ӧ        	[137]	PROXY-Response
			p8tx[1]=7;//[7] ProxyTransCommandResponse
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			offset=LEN_SA+8+3;
			//����ת���˿�                  OAD��
			ProxyPORTn=(p8rx[offset]<<24)+(p8rx[offset+1]<<16)+(p8rx[offset+2]<<8)+p8rx[offset+3];//�˿�OAD
			MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+offset,4);
			offset+=4;
			LEN_TxAPDU+=4;
			LENmax_TxSPACE-=4;
			ProxyPORTn=OADtoPORTn(ProxyPORTn);//OADֵתΪͨ�ſں�
		#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if((ProxyPORTn!=ACSAMPLEPORT)&&(ProxyPORTn!=RS485_1PORT)&&(ProxyPORTn!=RS485_2PORT)&&(ProxyPORTn!=RS485_4PORT))
		#endif
		#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if((ProxyPORTn!=ACSAMPLEPORT)&&(ProxyPORTn!=RS485_1PORT)&&(ProxyPORTn!=RS485_2PORT))
		#endif
		#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if((ProxyPORTn!=ACSAMPLEPORT)&&(ProxyPORTn!=RS485_1PORT)&&(ProxyPORTn!=RS485_2PORT)&&(ProxyPORTn!=RS485_4PORT))
		#endif
		#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if((ProxyPORTn==RS485_1PORT)||(ProxyPORTn==RS485_2PORT))
		#endif
			{
				p8tx[LEN_TxAPDU]=0;//������Ϣ      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//����
				LEN_TxAPDU+=2;
				break;
			}
			UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ProxyPORTn);
			if(UARTCtrlProxy->Function==0)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
			{
				p8tx[LEN_TxAPDU]=0;//������Ϣ      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//����
				LEN_TxAPDU+=2;
				break;
			}
			if(UARTCtrlProxy->AutoSearchTask)//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
			{
				p8tx[LEN_TxAPDU]=0;//������Ϣ      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//����
				LEN_TxAPDU+=2;
				break;
			}
			UARTCtrlProxy->ProxyPORTn=PORTn;//�������Ķ˿ں�
			//D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			//������
			i=p8rx[offset];
			switch(i)
			{
				case 0://300
				case 1://600
				case 2://1200
				case 3://2400
				case 4://4800
				case 5://7200
				case 6://9600
				case 7://19200
				case 8://38400
				case 9://57600
				case 10://115200
					x=(0xb+(i<<5));
					break;
				case 255://����Ӧ
				default:
					x=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
					break;
			}
			x&=0xffe0;
			//D3λ��ʾ����У��λ0/1�ֱ��ʾ��/��
			//D2λ��ʾż����У��λ0/1�ֱ��ʾż����У��
			//	У��λ  ENUMERATED {��У�飨0������У�飨1����żУ�飨2��}��
			i=p8rx[offset+1];
			switch(i)
			{
				case 0://��У��
					break;
				case 1://��У��
					x|=0x0c;
					break;
				default://żУ��
					x|=0x08;
					break;
			}
			//D1-D0 ����λ��0��3�ֱ��ʾ5-8λ
			//	����λ  ENUMERATED {5��5����6��6����7��7����8��8��}��
			i=p8rx[offset+2];
			i-=5;
			i&=0x03;
			x|=i;
			//D4λ��ʾֹͣλλ��0/1�ֱ��ʾ1/2��ֹͣλ
			//	ֹͣλ  ENUMERATED {1��1����2��2��}��
			i=p8rx[offset+3];
			if(i==2)
			{
				x|=0x10;
			}
			//	����    ENUMERATED {��(0)��Ӳ��(1)�����(2)}
			//�����Ժһ���Բ���ʱ��Ҫ�б�
			if(p8rx[offset+4]>2)
			{
				p8tx[LEN_TxAPDU]=0;//������Ϣ      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//����
				LEN_TxAPDU+=2;
				break;
			}
			offset+=5;
			UARTCtrlProxy->ProxyBpsCtrl=x;
			UARTCtrlProxy->ProxyRMprotocol=0;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
			//���յȴ����ĳ�ʱʱ�䣨�룩   long-unsigned
			//���յȴ��ֽڳ�ʱʱ�䣨���룩 long-unsigned
			offset+=4;
			//͸��ת������ octet-string
			i=p8rx[offset];
			if((i&0x80)==0)
			{
				offset+=1;
			}
			else
			{
				i&=0x7f;
				if(i==1)
				{
					i=p8rx[offset+1];
					offset+=2;
				}
				else
				{
					i=(p8rx[offset+1]<<8)|p8rx[offset+2];
					offset+=3;
				}
			}
			//if(i>LEN_UART2FnDataBuff)
			if(i>LEN_UART2Tx)
			{
				p8tx[LEN_TxAPDU]=0;//������Ϣ      [0] DAR
				p8tx[LEN_TxAPDU+1]=255;//����
				LEN_TxAPDU+=2;
				break;
			}
			//Ŀ�ĵ�ַ
			UARTCtrl->RMAddr[0]=0;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
			UARTCtrl->RMAddr[1]=0;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
			UARTCtrl->RMAddr[2]=0;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
			//�鿪ʼ��0xfe����
			for(x=0;x<i;x++)
			{
				if(p8rx[offset+x]!=0xfe)
				{
					break;
				}
			}
			TI=FrameProtocol(p8rx+offset+x,i-x);//ͨ��Э���Զ�ʶ��:����:0=δ֪,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
			UARTCtrl->RMprotocol=TI;//ͨ��Э���Զ�ʶ��:����:0=δ֪,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
			switch(TI)
			{
				case 1://DL/T645-1997
				case 2://DL/T645-2007
					MR((u32)&UARTCtrl->RMAddr,(u32)p8rx+offset+x+1,6);
					Data_Inverse((u32)&UARTCtrl->RMAddr,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(ProxyPORTn==RS485_4PORT)
					{
						if((UARTCtrl->RMAddr[0]==0x9999)&&(UARTCtrl->RMAddr[1]==0x9999)&&(UARTCtrl->RMAddr[2]==0x9999))
						{//�㲥Уʱ
							if(p8rx[offset+x+8]==8)
							{//������=8
								//��������
								TI=Check_OtherTask(ProxyPORTn);//����Ƿ����������񣻷���:0=û,!=0��
								if(TI==0)
								{//����������
									MR((u32)p8Data,(u32)p8rx+offset+x,i-x);
									UARTCtrlProxy->BroadcastTimeProxyTask=1;//����͸��ת���㲥Уʱ����:0=��,1=�д���
									UARTCtrlProxy->TaskID=(RMTASKnoPOWER<<8)+2;
									UARTCtrlProxy->BroadcastTimeTask=1;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
									while(1)
									{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
										if(UARTCtrlProxy->BroadcastTimeProxyTask==0)//����͸��ת���㲥Уʱ����:0=��,1=�д���
										{
											break;
										}
									}
								}
								//����������
								i=0;
								if((i+5)>LENmax_TxSPACE)
								{
									LEN_TxAPDU=0;
									break;
								}
								p8tx[LEN_TxAPDU]=1;//��������      [1] octet-string
								p8tx[LEN_TxAPDU+1]=0;
								LEN_TxAPDU+=2;
								TI=0;
								Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
								return;
							}
						}
					}
				#endif
					break;
				case 3://DL/T698.45
					MR((u32)&UARTCtrl->RMAddr,(u32)p8rx+offset+x+5,6);
					Data_Inverse((u32)&UARTCtrl->RMAddr,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(ProxyPORTn==RS485_4PORT)
					{
						if((p8rx[offset+x+4]==0xC0)&&(p8rx[offset+x+5]==0xAA)&&(p8rx[offset+x+9]==7))
						{//�㲥Уʱ
							TI=p8rx[offset+x+12]<<24;
							TI|=p8rx[offset+x+13]<<16;
							TI|=p8rx[offset+x+14]<<8;
							TI|=p8rx[offset+x+15]<<0;
							if(TI==0x40007f00)
							{
								//��������
								TI=Check_OtherTask(ProxyPORTn);//����Ƿ����������񣻷���:0=û,!=0��
								if(TI==0)
								{//����������
									MR((u32)p8Data,(u32)p8rx+offset+x,i-x);
									UARTCtrlProxy->BroadcastTimeProxyTask=1;//����͸��ת���㲥Уʱ����:0=��,1=�д���
									UARTCtrlProxy->TaskID=(RMTASKnoPOWER<<8)+2;
									UARTCtrlProxy->BroadcastTimeTask=1;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
									while(1)
									{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
										if(UARTCtrlProxy->BroadcastTimeProxyTask==0)//����͸��ת���㲥Уʱ����:0=��,1=�д���
										{
											break;
										}
									}
								}
								//����������
								i=0;
								if((i+5)>LENmax_TxSPACE)
								{
									LEN_TxAPDU=0;
									break;
								}
								p8tx[LEN_TxAPDU]=1;//��������      [1] octet-string
								p8tx[LEN_TxAPDU+1]=0;
								LEN_TxAPDU+=2;
								TI=0;
								Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
								return;
							}
						}
					}
				#endif
					break;
				case 128://DL376.2
					UARTCtrl->RMAddr[0]=0xFFFF;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
					UARTCtrl->RMAddr[1]=0xFFFF;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
					UARTCtrl->RMAddr[2]=0xFFFF;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
					break;
				
				case 4://CJ/T 188-2004
				default:
					if(ProxyPORTn==RS485_4PORT)
					{
						//��Ŀ�ĵ�ַ�޷�ת
						p8tx[LEN_TxAPDU]=0;//������Ϣ      [0] DAR
						p8tx[LEN_TxAPDU+1]=255;//�������� 
						LEN_TxAPDU+=2;
						Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
						return;
					}
					UARTCtrl->RMAddr[0]=0xEEEE;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
					UARTCtrl->RMAddr[1]=0xEEEE;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
					UARTCtrl->RMAddr[2]=0xEEEE;//��ʾ��ַʱ��,�ز���ʱĿ�ĵ�ַ��
					break;
			}
			
			if(ProxyPORTn==ACSAMPLEPORT)
			{//���ɿ�
				if(UARTCtrl->RMprotocol==3)//ͨ��Э���Զ�ʶ��:����:0=δ֪,1=DL/T645-1997,2=DL/T645-2007,3=DL/T698.45,4=CJ/T 188-2004,128=DL376.2
				{
					TI=Get_ADDR_UARTnRx(ProxyPORTn);
					MW((u32)p8rx+offset+x,TI,i-x);
					Uart_698_Data(ProxyPORTn,0);//UART_698����;���:��ȫģʽSECURITY=0������=1����
					if(UARTCtrlProxy->Task==2)//�ȴ���֡���ݷ���
					{
						UARTCtrlProxy->Task=0;
						x=UARTCtrlProxy->TxByte;
						UARTCtrlProxy->TxByte=0;
						UARTCtrlProxy->ProxyTxRxByte=x;
						MR((u32)p8Data,Get_ADDR_UARTnTx(ProxyPORTn),x);
					}
					else
					{
						UARTCtrlProxy->ProxyTxRxByte=0;
					}
				}
				else
				{
					UARTCtrlProxy->ProxyTxRxByte=0;
				}
			}
			else
			{//������
				MR((u32)p8Data,(u32)p8rx+offset,i);
				UARTCtrlProxy->ProxyTxRxByte=i;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
				UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				
				NUM_ReTx=0;//�����ط�����
				while(1)
				{
	#ifndef LINUX
		p32=(u32*)ADDR_MS_ENTRY;		
		(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
	#else
		ms=(ms_Type *)Get_ms();
		ms->Main_Thread();
	#endif
					if(UARTCtrlProxy->ProxyTask==0)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
					{
						if(UARTCtrlProxy->ProxyTxRxByte==0)
						{//�����ݽ���
							if(NUM_ReTx<NUMmax_ReTx)
							{
								NUM_ReTx++;//�����ط�����
								UARTCtrlProxy->ProxyTxRxByte=i;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
								UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
								continue;
							}
						}
						break;
					}
				}
			}
			
			//����������
			i=UARTCtrlProxy->ProxyTxRxByte;
			if(i==0)
			{
				p8tx[LEN_TxAPDU]=0;//������Ϣ      [0] DAR
				p8tx[LEN_TxAPDU+1]=33;//����ʱ 
				LEN_TxAPDU+=2;
				break;
			}
			if((i+5)>LENmax_TxSPACE)
			{
				LEN_TxAPDU=0;
				break;
			}
			p8tx[LEN_TxAPDU]=1;//��������      [1] octet-string
			if(i<=127)
			{
				p8tx[LEN_TxAPDU+1]=i;
				LEN_TxAPDU+=2;
			}
			else
			{
				p8tx[LEN_TxAPDU+1]=0x82;
				p8tx[LEN_TxAPDU+2]=i>>8;
				p8tx[LEN_TxAPDU+3]=i;
				LEN_TxAPDU+=4;
			}
			MR((u32)p8tx+LEN_TxAPDU,(u32)p8Data,i);
			LEN_TxAPDU+=i;
			break;
		default:
			return;
	}
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
}































