

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_RECORD.h"
#include "../DL698/RM_TASK.h"

#include "../DL698/DL698_TESAM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET_iMeter.h"
#include "../DL698/DL698_GET_6000.h"
#include "../DL698/DL698_GET_F000.h"
#include "../DL698/DL698_GET_EVENT.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../DL698/CONGEAL_Record.h"
#include "../DL698/EVENT_Record.h"

#include "../Calculate/Calculate.h"
#include "../Terminal698/Terminal_Init.h"
#include "../DL698/DL698_REPORT.h"


#include "../DL698/DL698_JSON.h"




u32 GET_RecordOAD(u32 PORTn,u8 *p8rx)//��һ����¼�Ͷ�������;����:b31-b16����ROAD����,b15-b0����ROAD�������ֽ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 n;
	u32 LEN_RSD;
	u32 LEN_MS;
	u32 LEN_RxOAD;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8SOUR;
	u8* pDATA;
	u8 *p8tx;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;
	u32 M_Offset;
	u32 LEN_TxData;
	u32 LEN_ReadData;
	ms_Type* ms;
	u16 *p16;
	u16* p16s;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//�ͻ���������Ϣ
	u32 OAD;
	DL698_SETLIB_TypeDef *pLib;
	u32 NUMmax;
	u32 MeterCount;
	u32 NUMmax_LIST;
	u64 d64;
	u32 FileID;
	u32 NUM_TIDOT;
	
#if(USER/100)==17//����ϵ
	u32 filename;
	msFILE_TypeDef *file;
	u8 *p8d;
	RecordFileHead_TypeDef* RecordFileHead;
#endif
	
	p16=(u16*)(ADDR_1kDATABUFF);
	p16[0]=0;//��0��RCSDΪȫѡʱ�������¼�ʱ��OAD�б���
	
	ms=Get_ms();
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;
	p16=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//�붨ʱ��
	p16[0]=TimerS_UARTRAMFREE;
	OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
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
	LEN_RxOAD=0;
	LEN_TxAPDU=0;
	UARTCtrl->LEN_TxData=0;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
	UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
	switch(OAD)//��������������     OAD
	{
//---60xx���� START----
		case 0x60000200://�ɼ��������ñ�
		case 0x60120200://�������ñ�
		case 0x60140200://��ͨ�ɼ�������
		case 0x60160200://�¼��ɼ�������
		case 0x60180200://͸��������
		case 0x601E0200://�ɼ������
			FLASHtoSDRAM();//����FLASHtoSDRAM
			switch(p8rx[4])//��¼ѡ��������RSD
			{
				case 0:
		//��ѡ��       [0]  NULL
					LEN_RxOAD+=4+1;
				
					pLib=Get_DL698_SET_pLib(OAD);//�õ�SET_pLibָ��;���:OI=�����ʶ,Attribute=���Ժ�;����:Libָ��(=0��ʾ����Ӧ��OI)
					if(pLib==0)
					{
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						break;
					}
					NUMmax=pLib->LENmax/pLib->LENper;
					LENmax_TxSPACE=ms->ms_malloc_sizemax();
					pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
					if(pDATA==0)
					{
							UARTCtrl->DAR=255;
							break;
					}
					LEN_TxAPDU=0;
					switch(OAD)//��������������     OAD
					{
						case 0x60000200://�ɼ��������ñ�
							p8SOUR=(u8*)ADDR_6000_SDRAM;
							OAD=0x60010200;//��׼��Ԫ
							break;
						case 0x60120200://�������ñ�
							p8SOUR=(u8*)ADDR_6012_2_SDRAM;
							OAD=0x60130200;//��׼��Ԫ
							break;
						case 0x60140200://��ͨ�ɼ�������
							p8SOUR=(u8*)ADDR_6014_SDRAM;
							OAD=0x60150200;//��׼��Ԫ
							break;
						case 0x60160200://�¼��ɼ�������
							p8SOUR=(u8*)ADDR_6016_SDRAM;
							OAD=0x60170200;//��׼��Ԫ
							break;
						case 0x60180200://͸��������
							p8SOUR=(u8*)pLib->Addr;
							OAD=0x60190200;//��׼��Ԫ
							break;
						case 0x601E0200://�ɼ������
							p8SOUR=(u8*)pLib->Addr;
							OAD=0x601F0200;//��׼��Ԫ
							break;
					}
					//�޸���ѡ��
					MR(ADDR_DATABUFF,(u32)p8rx,LEN_RxOAD);
					p8rx=(u8*)ADDR_DATABUFF;
					p8rx[LEN_RxOAD+0]=1;
					p8rx[LEN_RxOAD+1]=0;
					p8rx[LEN_RxOAD+2]=OAD>>24;
					p8rx[LEN_RxOAD+3]=(OAD>>16)&0xff;
					p8rx[LEN_RxOAD+4]=(OAD>>8)&0xff;
					p8rx[LEN_RxOAD+5]=(OAD>>0)&0xff;
					for(n=0;n<NUMmax;n++)
					{
						if(LENmax_TxSPACE<2)
						{
							break;
						}
						x=Get_DL698DataLen_S((u8*)p8SOUR,pLib->LENper);//�õ�DL698�������ͳ���(����:�����ֽں�����);
						if(x>2)
						{
							x+=2;//���ݿ鳤��(��������2�ֽ�)
							if(x>LENmax_TxSPACE)
							{
								UARTCtrl->DAR=255;//�������
								break;
							}
							pDATA[LEN_TxAPDU+0]=x;
							pDATA[LEN_TxAPDU+1]=x>>8;
							MR((u32)pDATA+LEN_TxAPDU+2,(u32)p8SOUR,x-2);
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
						}
						p8SOUR+=pLib->LENper;
					}
					pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
					UARTCtrl->pTxData=pDATA;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
					UARTCtrl->LEN_TxData=LEN_TxAPDU;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
					UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
					break;
				case 1:
		//Selector1��=SEQUENCE
		//{
		//  ��������������  OAD��
		//  ��ֵ            Data
		//}
//05 03 08 
//60 00 02 00 ��������������     OAD
				
//01 60 01 02 01 12 00 01 ��¼ѡ��������RSD
//01 00 60 01 02 00 //��¼��ѡ��������RCSD
//00 ��ʱ���ǩ
					LEN_RxOAD+=4;
					pLib=Get_DL698_SET_pLib(OAD);//�õ�SET_pLibָ��;���:OI=�����ʶ,Attribute=���Ժ�;����:Libָ��(=0��ʾ����Ӧ��OI)
					if(pLib==0)
					{
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						break;
					}
					switch(p8rx[8])//������ѡ���Ԫ�غ�
					{
						case 1://ѡ���һ��Ԫ��(������ŵ�)
							switch(p8rx[9])
							{
								case DataType_unsigned:
									n=p8rx[10];
									LEN_RSD=7;
									break;
								case DataType_long_unsigned:
								default:
									n=(p8rx[10]<<8)+p8rx[11];
									LEN_RSD=8;
									break;
							}
							LEN_RxOAD+=LEN_RSD;
							if(n>=(pLib->LENmax/pLib->LENper))
							{
								UARTCtrl->DAR=255;
								break;
							}
							n*=pLib->LENper;
							n+=pLib->Addr;
//							LEN_TxAPDU=Get_DL698DataLen((u8*)n,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							LEN_TxAPDU=Get_DL698DataLen((u8*)n,(Attribute_TypeDef *)0,0,pLib->LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							if(pLib->pDataType)
							{
								if((LEN_TxAPDU&0xff)==0)
								{
									UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
									break;
								}
							}
							LEN_TxAPDU>>=8;
							LEN_TxAPDU+=2;//���ݿ鳤��(��������2�ֽ�)
							pDATA=(u8*)ms->ms_malloc(LEN_TxAPDU);
							if(pDATA==0)
							{
								UARTCtrl->DAR=255;//����                 ��255��
								break;
							}
							pDATA[0]=LEN_TxAPDU;
							pDATA[1]=LEN_TxAPDU>>8;
							MR((u32)pDATA+2,n,LEN_TxAPDU-2);
							pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
							UARTCtrl->pTxData=pDATA;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
							UARTCtrl->LEN_TxData=LEN_TxAPDU;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
							UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
							break;
						default:
							UARTCtrl->DAR=255;//����                 ��255��
							break;
					}
					break;
				case 2:
		//Selector2��=SEQUENCE
		//{
		//  ��������������  OAD��
		//  ��ʼֵ          Data��
		//  ����ֵ          Data��
		//  ���ݼ��        Data
		//}
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 3:
		//Selector3��=SEQUENCE OF Selector2
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 4:
		//Selector4��=SEQUENCE
		//{
		//  �ɼ�����ʱ��  date_time_s��
		//  ���ܱ���    MS
		//}
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 5:
		//Selector5��=SEQUENCE
		//{
		//  �ɼ��洢ʱ��  date_time_s��
		//  ���ܱ���    MS
		//}
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 6:
		//Selector6��=SEQUENCE
		//{
		//  �ɼ�����ʱ����ʼֵ  date_time_s��
		//  �ɼ�����ʱ�����ֵ  date_time_s��
		//  ʱ����            TI��
		//  ���ܱ���          MS
		//}
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 7:
		//Selector7��=SEQUENCE
		//{
		//  �ɼ��洢ʱ����ʼֵ  date_time_s��
		//  �ɼ��洢ʱ�����ֵ  date_time_s��
		//  ʱ����            TI��
		//  ���ܱ���          MS
		//}
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 8:
		//Selector8��=SEQUENCE
		//{
		//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
		//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
		//  ʱ����            TI��
		//  ���ܱ���          MS
		//}
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 9:
		//Selector9��=SEQUENCE
		//{
		//  �ϵ�n�μ�¼  unsigned
		//}
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 10:
		//Selector10��=SEQUENCE
		//{
		//��n����¼  unsigned��
		//���ܱ���  MS
		//}
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				default:
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
			}
			break;
			
//---60���� END----
			
//---6034�ɼ������� START----
		case 0x60340200://�ɼ�����������
			switch(p8rx[4])//��¼ѡ��������RSD
			{
				default:
					UARTCtrl->DAR=6;//���󲻴���           ��6��
					break;
				case 1:
		//Selector1��=SEQUENCE
		//{
		//  ��������������  OAD��
		//  ��ֵ            Data
		//}
//05 03 08 
//60 00 02 00 ��������������     OAD
//01 60 01 02 01 12 00 01 ��¼ѡ��������RSD
//01 00 60 01 02 00 //��¼��ѡ��������RCSD
//00 ��ʱ���ǩ

//05 03 0e 
//60 34 02 00 ��������������     OAD
//01 60 35 02 01 11 01 ��¼ѡ��������RSD,�õ�һ��Ԫ������ID unsigned���м�¼��ѡ��
//00 ��¼��ѡ��������RCSD
//00 ��ʱ���ǩ
					LEN_RxOAD+=4;
					//LEN_RSD=8;
					//LEN_RxOAD+=LEN_RSD;
					
					switch(p8rx[8])//������ѡ���Ԫ�غ�
					{
						case 1://����ID unsigned���м�¼��ѡ��
							p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
//68 42 00 C3 05 01 00 00 00 00 00 10 75 85 85 03 0C 
//60 34 02 00 01 
//00 60 35 02 00 01 01 02 08 11 01 16 02 1C 07 E0 0B 0B 00 02 00 1C 07 E0 0B 0B 00 02 02 12 00 02 12 00 02 12 00 02 12 00 02 00 00 3D 50 16 
							p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
							p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
							p8tx[1]=3;//[3] GeResponseRecord
							p8tx[2]=UARTCtrl->PIID;//PIID-ACD
							p8tx[3]=p8rx[0];//OAD
							p8tx[4]=p8rx[1];
							p8tx[5]=p8rx[2];
							p8tx[6]=p8rx[3];
							p8tx[7]=1;//������
							p8tx[8]=0;//OAD����
							p8tx[9]=0x60;
							p8tx[10]=0x35;
							p8tx[11]=0x02;
							p8tx[12]=0x00;
							p8tx[13]=1;//��¼����
							p8tx[14]=1;//M��
							
							n=p8rx[10];//����ID
							if(n>=RMTASKmax)
							{
								UARTCtrl->DAR=255;
								break;
							}
							RM_Monitor(n,0,0);//�ɼ�������;���:ID=�����,NUM_Tx=��������,NUM_Rx=��������
							n=ADDR_6034+(n*LENper_6034);
							LEN_TxAPDU=Get_DL698DataLen((u8*)n,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							LEN_TxAPDU>>=8;
							if((15+LEN_TxAPDU)>LENmax_TxSPACE)
							{
								UARTCtrl->DAR=255;
								break;
							}
							MR((u32)p8tx+15,n,LEN_TxAPDU);
							LEN_TxAPDU+=15;
							return LEN_TxAPDU;
						default:
							UARTCtrl->DAR=255;
							break;
					}
					break;
				}
			break;
//---6034�ɼ������� END----
				
		case 0x60020200://�����ѱ���
			UARTCtrl->DAR=255;//�������
			return 0;
		case 0x60020500://��̨���ѱ���
			UARTCtrl->DAR=255;//�������
			return 0;
			
		case 0x601A0200://͸�����������
		case 0x601C0200://�ϱ�������
			UARTCtrl->DAR=255;//�������
			return 0;
		
//---60120300�������ñ� START----
		case 0x60120300://�������ñ�����3�����¼
			//����RSD����
			LEN_RSD=DataLen_RSD_NoMS(p8rx+4,0,0);//������MS���Ȳ���
			LEN_RSD>>=8;
			if(LEN_RSD)
			{
				LEN_RSD--;//����������
			}
			//����MS����
			if((p8rx[4]==0)||(p8rx[4]==9))//0=��ѡ����MS,9=�ϵ�n�μ�¼��MS
			{
				LEN_MS=0;
			}
			else
			{
				LEN_MS=DataLen_MS(p8rx+4+LEN_RSD,0,0);
				LEN_MS>>=8;
				if(LEN_MS)
				{
					LEN_MS--;//����������
				}
			}
			LEN_RxOAD+=4;
			LEN_RxOAD+=LEN_RSD;
			LEN_RxOAD+=LEN_MS;
			LENmax_TxSPACE=ms->ms_malloc_sizemax();
			pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
			if(pDATA==0)
			{
				UARTCtrl->DAR=255;
				break;
			}
			if(p8rx[4]==0)
			{//��ѡ����MS
				NUMmax_LIST=0;
				p16=(u16*)(ADDR_AddrList+2);
				p8SOUR=(u8*)ADDR_6000_SDRAM;
				for(i=0;i<NUMmax_6000;i++)
				{
					if(p8SOUR[0]==DataType_structure)
					{
						p16s=(u16*)(p8SOUR+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
						p16[0]=p16s[0];
						p16[1]=p16s[1];
						p16[2]=p16s[2];
						p16+=3;
						NUMmax_LIST+=1;
					}
					p8SOUR+=LENper_6000;
				}
				p16=(u16*)(ADDR_AddrList);
				p16[0]=NUMmax_LIST;
			}
			else
			{
				if(p8rx[4]==9)//9=�ϵ�n�μ�¼��MS
				{
					p16=(u16*)(ADDR_AddrList);
					p16[0]=1;//��MS,Ϊ���³���for()�ܽ��������ֵ=1
				}
				else
				{
					MStoAddrList(p8rx+4+LEN_RSD);//���ܱ��ϵ���ַ�б�;���:pMS(����������);����:��ַ�б���ADDR_AddrList
				}
			}
			p16=(u16*)(ADDR_AddrList);
			NUMmax_LIST=p16[0];
			LEN_TxAPDU=0;
			//ע:RCSDǰû�������ͣ�RCSDtoFileID()��RCSDtoID()������������
			FileID=RCSDtoFileID(p8rx+LEN_RxOAD-1);//��¼��ѡ��RCSD���Ӧ��FileID,���:pRCSDָ�������ֽ�;����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO=0xff
			if(FileID==0)
			{//0��ʾû�ҵ���ЧFileID
				pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
				UARTCtrl->pTxData=pDATA;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
				UARTCtrl->LEN_TxData=LEN_TxAPDU;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
				UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
				break;
			}
			for(MeterCount=0;MeterCount<NUMmax_LIST;MeterCount++)
			{
				if(LENmax_TxSPACE>2)
				{
					switch(p8rx[4])
					{
						case 0:
				//��ѡ��       [0]  NULL
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							if(x)
							{
								x+=2;//���ݿ鳤��(��������2�ֽ�)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;//�������
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							break;
						case 1:
				//Selector1��=SEQUENCE
				//{
				//  ��������������  OAD��
				//  ��ֵ            Data
				//}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							if(x)
							{
								x+=2;//���ݿ鳤��(��������2�ֽ�)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							break;
						case 2:
				//Selector2��=SEQUENCE
				//{
				//  ��������������  OAD��
				//  ��ʼֵ          Data��
				//  ����ֵ          Data��
				//  ���ݼ��        Data
				//}
							n=(p8rx[5]<<24)|(p8rx[6]<<16)|(p8rx[7]<<8)|p8rx[8];
							switch(n)
							{
								case 0x20210200://���ݶ���ʱ��
									if((p8rx[4+5]==DataType_date_time_s)&&(p8rx[4+5+8]==DataType_date_time_s))
									{
										if((p8rx[4+5+8+8]==DataType_TI)||(p8rx[4+5+8+8]==0))
										{
											x=Compare_DL698DataString(p8rx+4+5+1,p8rx+4+5+8+1,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
											NUM_TIDOT=0;//�����ж�ʱ���TI��������ݵ���
											while((x!=1)&&(NUM_TIDOT<NUMmax_TIDOT))
											{
												NUM_TIDOT++;
												if(LENmax_TxSPACE<2)
												{
													break;
												}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
												x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
												if(x)
												{
													x+=2;//���ݿ鳤��(��������2�ֽ�)
													if(x>LENmax_TxSPACE)
													{
														UARTCtrl->DAR=255;//�������
														break;
													}
													pDATA[LEN_TxAPDU+0]=x;
													pDATA[LEN_TxAPDU+1]=x>>8;
													LEN_TxAPDU+=x;
													LENmax_TxSPACE-=x;
												}
												//���ݼ��
												if(p8rx[4+5+8+8]==0)
												{//TI=��
													break;
												}
												else
												{
													x=pTItoSECOND(p8rx+4+5+8+8+1);//TIתΪ��ֵ,���:pTI����������,����:hex��
													if(x==0)
													{//�޼��
														break;
													}
												}
												pYYMDHMS_AddTI_hex(p8rx+4+5+1,p8rx+4+5+8+8+1);//��������ʱ�����ʱ����TI,���:pYYMDHMS,pTI����������,���ؼӼ��TI�����������ʱ����
												//pYYMDHMS_AddS_hex(p8rx+4+5+1,x);//��������ʱ�����S��,���ؼ�S������������ʱ����
												x=Compare_DL698DataString(p8rx+4+5+1,p8rx+4+5+8+1,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
											}
										}
									}
									break;
								default:
									break;
							}
							break;
						case 3:
				//Selector3��=SEQUENCE OF Selector2
							n=(p8rx[6]<<24)|(p8rx[7]<<16)|(p8rx[8]<<8)|p8rx[9];
							switch(n)
							{
								case 0x20210200://���ݶ���ʱ��
									NUMmax=p8rx[5];
									if(NUMmax>=2)
									{
										n=Compare_DL698DataString(p8rx+4+2,p8rx+4+6+8+8+4,4+8+8+4);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
										if(n==0)
										{
											if(NUMmax)
											{
												NUMmax--;//2��ʱ����ͬ
											}
										}
									}
									for(n=0;n<NUMmax;n++)
									{
										if((p8rx[4+6]==DataType_date_time_s)&&(p8rx[4+6+8]==DataType_date_time_s))
										{
											if((p8rx[4+6+8+8]==DataType_TI)||(p8rx[4+6+8+8]==0))
											{
												x=Compare_DL698DataString(p8rx+4+6+1,p8rx+4+6+8+1,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
												NUM_TIDOT=0;//�����ж�ʱ���TI��������ݵ���
												while((x!=1)&&(NUM_TIDOT<NUMmax_TIDOT))
												{
													NUM_TIDOT++;
													if(LENmax_TxSPACE<2)
													{
														break;
													}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
													x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
													if(x)
													{
														x+=2;//���ݿ鳤��(��������2�ֽ�)
														if(x>LENmax_TxSPACE)
														{
															UARTCtrl->DAR=255;//�������
															break;
														}
														pDATA[LEN_TxAPDU+0]=x;
														pDATA[LEN_TxAPDU+1]=x>>8;
														LEN_TxAPDU+=x;
														LENmax_TxSPACE-=x;
													}
													//���ݼ��
													if(p8rx[4+6+8+8]==0)
													{//�޼��
														break;
													}
													else
													{
														x=pTItoSECOND(p8rx+4+6+8+8+1);//TIתΪ��ֵ,���:pTI����������;����:hex��
														if(x==0)
														{//�޼��
															break;
														}
													}
													pYYMDHMS_AddTI_hex(p8rx+4+6+1,p8rx+4+6+8+8+1);//��������ʱ�����ʱ����TI,���:pYYMDHMS,pTI����������,���ؼӼ��TI�����������ʱ����
													//pYYMDHMS_AddS_hex(p8rx+4+6+1,x);//��������ʱ�����S��,���ؼ�S������������ʱ����
													x=Compare_DL698DataString(p8rx+4+6+1,p8rx+4+6+8+1,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
												}
											}
										}
										if(n)
										{
											MW((u32)p8rx,(u32)p8rx+(4+6+8+8+4)-6,6);
										}
										p8rx+=4+8+8+4;
										if(LEN_RSD>=(4+8+8+4))
										{
											LEN_RSD-=(4+8+8+4);
										}
									}
									p8rx-=NUMmax*(4+8+8+4);
									LEN_RSD+=NUMmax*(4+8+8+4);
									break;
								default:
									break;
							}
							break;
						case 4:
				//Selector4��=SEQUENCE
				//{
				//  �ɼ�����ʱ��  date_time_s��
				//  ���ܱ���    MS
				//}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							if(x)
							{
								x+=2;//���ݿ鳤��(��������2�ֽ�)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;//�������
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							break;
						case 5:
				//Selector5��=SEQUENCE
				//{
				//  �ɼ��洢ʱ��  date_time_s��
				//  ���ܱ���    MS
				//}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							if(x)
							{
								x+=2;//���ݿ鳤��(��������2�ֽ�)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;//�������
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							break;
						case 6:
				//Selector6��=SEQUENCE
				//{
				//  �ɼ�����ʱ����ʼֵ  date_time_s��
				//  �ɼ�����ʱ�����ֵ  date_time_s��
				//  ʱ����            TI��
				//  ���ܱ���          MS
				//}
						case 7:
				//Selector7��=SEQUENCE
				//{
				//  �ɼ��洢ʱ����ʼֵ  date_time_s��
				//  �ɼ��洢ʱ�����ֵ  date_time_s��
				//  ʱ����            TI��
				//  ���ܱ���          MS
				//}
						case 8:
				//Selector8��=SEQUENCE
				//{
				//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
				//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
				//  ʱ����            TI��
				//  ���ܱ���          MS
				//}
//05 03 0D 
//60 12 03 00 
//07  Selector7
//07 E0 0B 0B 00 00 00  �ɼ��洢ʱ����ʼֵ  date_time_s
//07 E0 0B 0B 17 3B 3B  �ɼ��洢ʱ�����ֵ  date_time_s
//00 FF FF  ʱ����  TI
//01 04 00 20 2A 02 00 00 00 10 02 00 00 20 00 02 00 00 20 01 02 00 
//00 
							d64=MRR((u32)p8rx+4+1,7);
							x=Compare_DL698DataString(p8rx+4+1,p8rx+4+1+7,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							NUM_TIDOT=0;//�����ж�ʱ���TI��������ݵ���
							while((x!=1)&&(NUM_TIDOT<NUMmax_TIDOT))
							{
								NUM_TIDOT++;
								if(LENmax_TxSPACE<2)
								{
									break;
								}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
								x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
								if(x)
								{
									x+=2;//���ݿ鳤��(��������2�ֽ�)
									if(x>LENmax_TxSPACE)
									{
										UARTCtrl->DAR=255;//�������
										break;
									}
									pDATA[LEN_TxAPDU+0]=x;
									pDATA[LEN_TxAPDU+1]=x>>8;
									LEN_TxAPDU+=x;
									LENmax_TxSPACE-=x;
								}
//								if(p8rx[4]==8)
//								{//�ɼ��ɹ�ʱ��
//									break;
//								}
								x=pTItoSECOND(p8rx+4+1+7+7);//TIתΪ��ֵ,���:pTI����������;����:hex��
								if(x==0)
								{//�޼��
									break;
								}
								pYYMDHMS_AddTI_hex(p8rx+4+1,p8rx+4+1+7+7);//��������ʱ�����ʱ����TI,���:pYYMDHMS,pTI����������,���ؼӼ��TI�����������ʱ����
								//pYYMDHMS_AddS_hex(p8rx+4+1,x);//��������ʱ�����S��,���ؼ�S������������ʱ����
								x=Compare_DL698DataString(p8rx+4+1,p8rx+4+1+7,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							}
							MWR(d64,(u32)p8rx+4+1,7);
							break;
						case 9:
				//Selector9��=SEQUENCE
				//{
				//  �ϵ�n�μ�¼  unsigned
				//}
							UARTCtrl->DAR=255;//�������
							break;
						case 10:
				//Selector10��=SEQUENCE
				//{
				//��n����¼  unsigned��
				//���ܱ���  MS
				//}
							NUMmax=p8rx[4+1];//��n����¼
							i=0;//��ʼ�ļ�������
							while(1)
							{
								if((NUMmax==0)||(i>=127))
								{
									break;
								}
//u32 Get_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							#if(USER/100)==17//����ϵ
								
								
								if(FileID==MeterAutoReportFileID_3011)
								{
										Comm_Ram->msFILESearchCount=0;
										filename=Get_RecordFileName(FileID,0xffffffff,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
										if(filename!=0xffffffff)
										{//�ҵ�
											if(filename<FILENAMEmax_FLASH)
											{
												file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
											}
											else
											{
												file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
											}
											i=file->LEN_DATA;
											if(i>(NUM_RMmax*sizeof(RecordFileHead_TypeDef)))
											{
												i-=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
											}
											//ms=Get_ms();
											//p8d=(u8*)ms->ms_realloc(p8d,i);
											//p8d=(u8*)ms->ms_malloc(i);//��̬�����ҵ��ڴ��
											p8d=(u8*)ADDR_DATABUFF;
											if(p8d)
											{
												//ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
												ms->msfile_read(filename,NUM_RMmax*sizeof(RecordFileHead_TypeDef),p8d,i);
												n=file->NUM_DATA;
												if((n*sizeof(RecordFileHead_TypeDef))>LEN_128KDATABUFF)
												{
													n=0;
												}
												ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,n*sizeof(RecordFileHead_TypeDef));
												RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
												for(i=0;i<n;i++)
												{
													x=Get_RecordFileData_RCSD(p8rx+4+LEN_RSD+LEN_MS,(u8*)&RecordFileHead->Addr0,p8d,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2,0);//��¼�����ѡ��;���:pRCSDָ����ѡ���RCSD(����������),pTSA,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
													if(x)
													{
														//f=1;//���ҵ����ݱ�־
														NUMmax--;//��n��-1
														x+=2;//���ݿ鳤��(��������2�ֽ�)
														if(x>LENmax_TxSPACE)
														{
															UARTCtrl->DAR=255;//�������
															break;
														}
														pDATA[LEN_TxAPDU+0]=x;
														pDATA[LEN_TxAPDU+1]=x>>8;
														LEN_TxAPDU+=x;
														LENmax_TxSPACE-=x;
													}
													p8d+=RecordFileHead->LEN_DATA;
													RecordFileHead++;
												}
												MeterCount=NUMmax_LIST+1;
												break;
//												
												}
											}
										
								}
								else
								x=GET_RecordFileData(FileID,i,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							#else	
								x=GET_RecordFileData(FileID,i,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
							#endif	
								if(x)
								{
									//f=1;//���ҵ����ݱ�־
									NUMmax--;//��n��-1
									x+=2;//���ݿ鳤��(��������2�ֽ�)
									if(x>LENmax_TxSPACE)
									{
										UARTCtrl->DAR=255;//�������
										break;
									}
									pDATA[LEN_TxAPDU+0]=x;
									pDATA[LEN_TxAPDU+1]=x>>8;
									LEN_TxAPDU+=x;
									LENmax_TxSPACE-=x;
								}
								i++;//�ļ�������
							}
							break;
						default:
							UARTCtrl->DAR=255;//�������
							break;
					}
				}
			}
			pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
			UARTCtrl->pTxData=pDATA;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
			UARTCtrl->LEN_TxData=LEN_TxAPDU;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
			UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
			break;
//---60120300�������ñ� END----
			
//---50�ն˶��� START---
		case 0x50000200://˲ʱ����
		case 0x50010200://�붳��
		case 0x50020200://���Ӷ���
		case 0x50030200://Сʱ����
		case 0x50040200://�ն���
		case 0x50050200://�����ն���
		case 0x50060200://�¶���
		case 0x50070200://�궳��
		case 0x50080200://ʱ�����л�����
		case 0x50090200://��ʱ�α��л�����
		case 0x500A0200://���ʵ���л�����
		case 0x500B0200://�����л�����
		case 0x50110200://���ݽ��㶳��
//05 03 03 
//50 04 02 00 01 20 21 02 00 1c 07 E0 01 14 00 00 00 02 00 20 21 02 00 00 00 10 02 00 00
			//����RSD����
			LEN_RSD=DataLen_RSD_NoMS(p8rx+4,0,0);//������MS���Ȳ���
			LEN_RSD>>=8;
			if(LEN_RSD)
			{
				LEN_RSD--;//����������
			}
			LEN_RxOAD+=4;
			LEN_RxOAD+=LEN_RSD;
			LENmax_TxSPACE=ms->ms_malloc_sizemax();
			if(LENmax_TxSPACE<2)
			{
					UARTCtrl->DAR=255;
					break;
			}
			pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
			if(pDATA==0)
			{
					UARTCtrl->DAR=255;
					break;
			}
			LEN_TxAPDU=0;
			switch(p8rx[4])
			{
				case 0:
		//��ѡ��       [0]  NULL
					UARTCtrl->DAR=255;//�������
					break;
				case 1:
		//Selector1��=SEQUENCE
		//{
		//  ��������������  OAD��
		//  ��ֵ            Data
		//}
					x=Get_CongealFileData(OAD>>16,0xffffffff,p8rx+4,p8rx+4+LEN_RSD,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ������ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
					if(x)
					{
						x+=2;//���ݿ鳤��(��������2�ֽ�)
						if(x>LENmax_TxSPACE)
						{
							UARTCtrl->DAR=255;
							break;
						}
						pDATA[LEN_TxAPDU+0]=x;
						pDATA[LEN_TxAPDU+1]=x>>8;
						LEN_TxAPDU+=x;
						LENmax_TxSPACE-=x;
					}
					break;
				case 2:
		//Selector2��=SEQUENCE
		//{
		//  ��������������  OAD��
		//  ��ʼֵ          Data��
		//  ����ֵ          Data��
		//  ���ݼ��        Data
		//}
					n=(p8rx[5]<<24)|(p8rx[6]<<16)|(p8rx[7]<<8)|p8rx[8];
					switch(n)
					{
						case 0x20210200://���ݶ���ʱ��
							if((p8rx[4+5]==DataType_date_time_s)&&(p8rx[4+5+8]==DataType_date_time_s))
							{
								if((p8rx[4+5+8+8]==DataType_TI)||(p8rx[4+5+8+8]==0))
								{
									x=Compare_DL698DataString(p8rx+4+5+1,p8rx+4+5+8+1,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
									while(x!=1)
									{
										if(LENmax_TxSPACE<2)
										{
											break;
										}
										x=Get_CongealFileData(OAD>>16,0xffffffff,p8rx+4,p8rx+4+LEN_RSD,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ������ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
										if(x)
										{
											x+=2;//���ݿ鳤��(��������2�ֽ�)
											if(x>LENmax_TxSPACE)
											{
												UARTCtrl->DAR=255;//�������
												break;
											}
											pDATA[LEN_TxAPDU+0]=x;
											pDATA[LEN_TxAPDU+1]=x>>8;
											LEN_TxAPDU+=x;
											LENmax_TxSPACE-=x;
										}
										//���ݼ��
										if(p8rx[4+5+8+8]==0)
										{
											break;;//x=15*60;//15��
										}
										else
										{
											x=pTItoSECOND(p8rx+4+5+8+8+1);//TIתΪ��ֵ,���:pTI����������;����:hex��
											if(x==0)
											{
												break;//x=24*60*60;
											}
										}
										pYYMDHMS_AddTI_hex(p8rx+4+5+1,p8rx+4+5+8+8+1);//��������ʱ�����ʱ����TI,���:pYYMDHMS,pTI����������,���ؼӼ��TI�����������ʱ����
										//pYYMDHMS_AddS_hex(p8rx+4+5+1,x);//��������ʱ�����S��,���ؼ�S������������ʱ����
										x=Compare_DL698DataString(p8rx+4+5+1,p8rx+4+5+8+1,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
									}
								}
							}
							break;
						default:
							break;
					}
					break;
				case 3:
		//Selector3��=SEQUENCE OF Selector2
					n=(p8rx[6]<<24)|(p8rx[7]<<16)|(p8rx[8]<<8)|p8rx[9];
					switch(n)
					{
						case 0x20210200://���ݶ���ʱ��
							NUMmax=p8rx[5];
							if(NUMmax>=2)
							{
								n=Compare_DL698DataString(p8rx+4+2,p8rx+4+6+8+8+4,4+8+8+4);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
								if(n==0)
								{
									if(NUMmax)
									{
										NUMmax--;//2��ʱ����ͬ
									}
								}
							}
							for(n=0;n<NUMmax;n++)
							{
								if((p8rx[4+6]==DataType_date_time_s)&&(p8rx[4+6+8]==DataType_date_time_s))
								{
									if((p8rx[4+6+8+8]==DataType_TI)||(p8rx[4+6+8+8]==0))
									{
										x=Compare_DL698DataString(p8rx+4+6+1,p8rx+4+6+8+1,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
										while(x!=1)
										{
											if(LENmax_TxSPACE<2)
											{
												break;
											}
											x=Get_CongealFileData(OAD>>16,0xffffffff,p8rx+4,p8rx+4+LEN_RSD,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ������ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
											if(x)
											{
												x+=2;//���ݿ鳤��(��������2�ֽ�)
												if(x>LENmax_TxSPACE)
												{
													UARTCtrl->DAR=255;//�������
													break;
												}
												pDATA[LEN_TxAPDU+0]=x;
												pDATA[LEN_TxAPDU+1]=x>>8;
												LEN_TxAPDU+=x;
												LENmax_TxSPACE-=x;
											}
											//���ݼ��
											if(p8rx[4+6+8+8]==0)
											{
												break;//x=15*60;//15��
											}
											else
											{
												x=pTItoSECOND(p8rx+4+6+8+8+1);//TIתΪ��ֵ,���:pTI����������;����:hex��
												if(x==0)
												{
													break;//x=24*60*60;
												}
											}
											pYYMDHMS_AddTI_hex(p8rx+4+6+1,p8rx+4+6+8+8+1);//��������ʱ�����ʱ����TI,���:pYYMDHMS,pTI����������,���ؼӼ��TI�����������ʱ����
											//pYYMDHMS_AddS_hex(p8rx+4+6+1,x);//��������ʱ�����S��,���ؼ�S������������ʱ����
											x=Compare_DL698DataString(p8rx+4+6+1,p8rx+4+6+8+1,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
										}
									}
								}
								if(n)
								{
									MW((u32)p8rx,(u32)p8rx+(4+6+8+8+4)-6,6);
								}
								p8rx+=4+8+8+4;
								if(LEN_RSD>=(4+8+8+4))
								{
									LEN_RSD-=(4+8+8+4);
								}
							}
							p8rx-=NUMmax*(4+8+8+4);
							LEN_RSD+=NUMmax*(4+8+8+4);
							break;
						default:
							break;
					}
					break;
				case 4:
		//Selector4��=SEQUENCE
		//{
		//  �ɼ�����ʱ��  date_time_s��
		//  ���ܱ���    MS
		//}
				case 5:
		//Selector5��=SEQUENCE
		//{
		//  �ɼ��洢ʱ��  date_time_s��
		//  ���ܱ���    MS
		//}
				case 6:
		//Selector6��=SEQUENCE
		//{
		//  �ɼ�����ʱ����ʼֵ  date_time_s��
		//  �ɼ�����ʱ�����ֵ  date_time_s��
		//  ʱ����            TI��
		//  ���ܱ���          MS
		//}
				case 7:
		//Selector7��=SEQUENCE
		//{
		//  �ɼ��洢ʱ����ʼֵ  date_time_s��
		//  �ɼ��洢ʱ�����ֵ  date_time_s��
		//  ʱ����            TI��
		//  ���ܱ���          MS
		//}
				case 8:
		//Selector8��=SEQUENCE
		//{
		//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
		//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
		//  ʱ����            TI��
		//  ���ܱ���          MS
		//}
					break;
				case 9:
		//Selector9��=SEQUENCE
		//{
		//  �ϵ�n�μ�¼  unsigned
		//}
					NUMmax=p8rx[4+1];//�ϵ�n��
					if(NUMmax)
					{
						NUMmax--;
					}
					if(p8rx[4+2]==0)
					{//����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ����
						//������ѡ��OAD
						if(LEN_RxOAD<1024)
						{//��֤���峤��
							MR(ADDR_DATABUFF,(u32)p8rx,LEN_RxOAD);
							p8rx=(u8*)ADDR_DATABUFF;
							n=MRR(ADDR_5004+1,1);//�ն�������������Ա�
							p8rx[LEN_RxOAD]=n;//����
							for(x=0;x<n;x++)
							{
								p8rx[LEN_RxOAD+1+(5*x)]=0;//OAD
								MR((u32)p8rx+LEN_RxOAD+1+(5*x)+1,ADDR_5004+2+(x*13)+6,4);
							}
						}
					}
					x=Get_CongealFileData(OAD>>16,NUMmax,p8rx+4,p8rx+4+LEN_RSD,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//�õ������ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
					if(x)
					{
						x+=2;//���ݿ鳤��(��������2�ֽ�)
						if(x>LENmax_TxSPACE)
						{
							UARTCtrl->DAR=255;
							break;
						}
						pDATA[LEN_TxAPDU+0]=x;
						pDATA[LEN_TxAPDU+1]=x>>8;
						LEN_TxAPDU+=x;
						LENmax_TxSPACE-=x;
					}
			#if (USER/100)==0//���Ժ����
					else
					{
						//01 00 90 01 09 00
						i=p8rx[4+LEN_RSD]<<8;
						i|=p8rx[4+LEN_RSD+1];
						if(i==0x0100)
						{
							i=p8rx[4+LEN_RSD+2]<<24;
							i|=p8rx[4+LEN_RSD+3]<<16;
							i|=p8rx[4+LEN_RSD+4]<<8;
							i|=p8rx[4+LEN_RSD+5];
							if(i==0x90010900)
							{
								x=1+2;//���ݿ鳤��(��������2�ֽ�)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								pDATA[LEN_TxAPDU+2]=0;//���ݿ�
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
						}
					}
			#endif
					break;
				case 10:
		//Selector10��=SEQUENCE
		//{
		//��n����¼  unsigned��
		//���ܱ���  MS
		//}
				default:
					break;
			}
			pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
			UARTCtrl->pTxData=pDATA;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
			UARTCtrl->LEN_TxData=LEN_TxAPDU;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
			UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
			break;
//---50�ն˶��� END---
			
		default:
			switch(OAD>>28)
			{
//---�¼���¼ START---
				case 3://�¼���¼
					switch(p8rx[4])//��¼ѡ��������RSD
					{
						case 0:
				//��ѡ��       [0]  NULL
							
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						case 1:
				//Selector1��=SEQUENCE
				//{
				//  ��������������  OAD��
				//  ��ֵ            Data
				//}
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						case 2:
				//Selector2��=SEQUENCE
				//{
				//  ��������������  OAD��
				//  ��ʼֵ          Data��
				//  ����ֵ          Data��
				//  ���ݼ��        Data
				//}
//05 03 0d 
//30 0f 02 00 
//02
//20 1e 02 00 
//1c 07 e1 05 1f 0d 2f 1c 
//1c 07 e1 05 1f 0d 30 3b 
//54 00 00 00 
//00
//00
//���Ժ��ѹ�������ʱ�õ�����������һ����ͬ��ʵ��û��
//����
//85 03 0D 
//30 0F 02 00 
//04 
//00 20 22 02 00 
//00 20 1E 02 00 
//00 20 20 02 00 
//00 20 24 02 00 
//01 00 
//00 00 
							LEN_RSD=25;
							LEN_RxOAD+=4+LEN_RSD;
							OAD&=0xffffff00;
							OAD|=1;//�ϵ�n��
							GET_EventRecord(PORTn,OAD,(u8*)ADDR_128KDATABUFF,LEN_128KDATABUFF,1);//�õ��¼���¼;���:ALL=0ֻ�¼���OAD�б�,ALL=1��OAD�б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
							if(UARTCtrl->DAR)
							{
								//���¼������Ժ����ʱ�践��������
								UARTCtrl->DAR=0;
								p8tx=(u8*)ADDR_1kDATABUFF;
								p8tx[0]=4;
								MWR(0x0002222000,ADDR_1kDATABUFF+1,5);
								MWR(0x00021e2000,ADDR_1kDATABUFF+1+5,5);
								MWR(0x0002202000,ADDR_1kDATABUFF+1+5+5,5);
								MWR(0x0002242000,ADDR_1kDATABUFF+1+5+5+5,5);
								pDATA=(u8*)ms->ms_malloc(1);//��ʹָ�벻Ϊ�գ���������
								if(pDATA==0)
								{
									UARTCtrl->DAR=255;
									break;
								}
								UARTCtrl->pTxData=pDATA;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
								UARTCtrl->LEN_TxData=0;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
								UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
								break;
							}
							LENmax_TxSPACE=ms->ms_malloc_sizemax();
							if(LENmax_TxSPACE<2)
							{
								UARTCtrl->DAR=255;
								break;
							}
							pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
							if(pDATA==0)
							{
								UARTCtrl->DAR=255;
								break;
							}
							LEN_TxAPDU=0;
							x=GET_EventRecord_RCSD(OAD,p8rx+4+LEN_RSD,(u8*)ADDR_128KDATABUFF,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2,ALLDATANULLSEL);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
							if(x)
							{
								x+=2;//���ݿ鳤��(��������2�ֽ�)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
							UARTCtrl->pTxData=pDATA;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
							UARTCtrl->LEN_TxData=LEN_TxAPDU;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
							UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
							break;
						case 3:
				//Selector3��=SEQUENCE OF Selector2
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						case 4:
				//Selector4��=SEQUENCE
				//{
				//  �ɼ�����ʱ��  date_time_s��
				//  ���ܱ���    MS
				//}
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						case 5:
				//Selector5��=SEQUENCE
				//{
				//  �ɼ��洢ʱ��  date_time_s��
				//  ���ܱ���    MS
				//}
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						case 6:
				//Selector6��=SEQUENCE
				//{
				//  �ɼ�����ʱ����ʼֵ  date_time_s��
				//  �ɼ�����ʱ�����ֵ  date_time_s��
				//  ʱ����            TI��
				//  ���ܱ���          MS
				//}
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						case 7:
				//Selector7��=SEQUENCE
				//{
				//  �ɼ��洢ʱ����ʼֵ  date_time_s��
				//  �ɼ��洢ʱ�����ֵ  date_time_s��
				//  ʱ����            TI��
				//  ���ܱ���          MS
				//}
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						case 8:
				//Selector8��=SEQUENCE
				//{
				//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
				//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
				//  ʱ����            TI��
				//  ���ܱ���          MS
				//}
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						case 9:
				//Selector9��=SEQUENCE
				//{
				//  �ϵ�n�μ�¼  unsigned
				//}
							LEN_RSD=2;
							LEN_RxOAD+=4+LEN_RSD;
							n=p8rx[5];//�ϵ�n��
							OAD&=0xffffff00;
							OAD|=n;
							GET_EventRecord(PORTn,OAD,(u8*)ADDR_128KDATABUFF,LEN_128KDATABUFF,1);//�õ��¼���¼;���:ALL=0ֻ�¼���OAD�б�,ALL=1��OAD�б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
							if(UARTCtrl->DAR)
							{
								break;
							}
							LENmax_TxSPACE=ms->ms_malloc_sizemax();
							if(LENmax_TxSPACE<2)
							{
								UARTCtrl->DAR=255;
								break;
							}
							pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
							if(pDATA==0)
							{
								UARTCtrl->DAR=255;
								break;
							}
							LEN_TxAPDU=0;
							x=GET_EventRecord_RCSD(OAD,p8rx+4+LEN_RSD,(u8*)ADDR_128KDATABUFF,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2,ALLDATANULLSEL);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
							if(x)
							{
								x+=2;//���ݿ鳤��(��������2�ֽ�)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
							UARTCtrl->pTxData=pDATA;//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
							UARTCtrl->LEN_TxData=LEN_TxAPDU;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
							UARTCtrl->LEN_ReadData=0;//�Ѷ�ȡ�ķ��������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
							break;
						case 10:
				//Selector10��=SEQUENCE
				//{
				//��n����¼  unsigned��
				//���ܱ���  MS
				//}
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
						default:
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							break;
					}
					break;
//---�¼���¼ END---
					
//---δ֪  START---
				default:
//RET_ErrDAR:
					if(UARTCtrl->pTxData)//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
					{
						ms->ms_free(UARTCtrl->pTxData);
						UARTCtrl->pTxData=0;
					}
					//����RSD����
					LEN_RSD=DataLen_RSD_NoMS(p8rx+4,0,0);//������MS���Ȳ���
					LEN_RSD>>=8;
					if(LEN_RSD)
					{
						LEN_RSD--;//����������
					}
					LEN_RxOAD=4+LEN_RSD;
					LEN_TxAPDU=0;
					//break;

					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
					p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
					p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
					p8tx[1]=3;//[3] GeResponseRecord
					p8tx[2]=UARTCtrl->PIID;//PIID-ACD
					p8tx[3]=p8rx[0];//OAD
					p8tx[4]=p8rx[1];
					p8tx[5]=p8rx[2];
					p8tx[6]=p8rx[3];
					LEN_TxAPDU=7;
					LENmax_TxSPACE-=LEN_TxAPDU;
					x=LEN_RxOAD;
					n=p8rx[LEN_RxOAD];//SEQUENCE OF CSD����
					if(n==0)
					{
						//����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ������
						p8rx=(u8*)ADDR_1kDATABUFF;
						n=p8rx[0];
						p8rx-=x;
					}
					LEN_RxOAD++;
					while(n--)
					{
						if(p8rx[LEN_RxOAD]==0)
						{
							LEN_RxOAD+=5;
						}
						else
						{
							i=p8rx[LEN_RxOAD+5];
							i*=4;
							i+=6;
							LEN_RxOAD+=i;
						}
					}
					n=LEN_RxOAD-x;//CSD����
					LEN_RxOAD<<=16;
					if((n+2)>LENmax_TxSPACE)
					{
						UARTCtrl->DAR=8;//Խ�磨8��
						return LEN_RxOAD;
					}
					MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+x,n);//copy CSD
					LEN_TxAPDU+=n;
					LENmax_TxSPACE-=n;
					p8tx[LEN_TxAPDU]=0;//0=����
					p8tx[LEN_TxAPDU+1]=4;//����δ����           ��4��
					LEN_TxAPDU+=2;
					return LEN_RxOAD|LEN_TxAPDU;
//---δ֪  END---
			}
			break;
	}
	
//GetResponseRecord��=SEQUENCE
//{
//  �������-���ȼ�-ACD         PIID-ACD��
//  һ����¼�Ͷ������Լ�����  A-ResultRecord
//}
//A-ResultRecord��=CHOICE
//{
//  ������Ϣ       [0] DAR��
//  ��¼����       [1] A-RecordData
//}
//A-RecordData��=SEQUENCE
//{
//  ��¼�Ͷ�������������    	OAD��
//  һ�м�¼N������������ 	RCSD��
//  M����¼                	SEQUENCE OF A-RecordRow
//}
//A-RecordRow��=SEQUENCE
//{
//  ��1������   Data��
//  ��2������   Data��
//  ��
//  ��N������   Data
//}
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
	p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
	p8tx[1]=3;//[3] GeResponseRecord
	p8tx[2]=UARTCtrl->PIID;//PIID-ACD
	p8tx[3]=p8rx[0];//OAD
	p8tx[4]=p8rx[1];
	p8tx[5]=p8rx[2];
	p8tx[6]=p8rx[3];
	LEN_TxAPDU=7;
	LENmax_TxSPACE-=LEN_TxAPDU;
	
	x=LEN_RxOAD;
	n=p8rx[LEN_RxOAD];//SEQUENCE OF CSD����
	if(n==0)
	{
		//����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ������
		p8rx=(u8*)ADDR_1kDATABUFF;
		n=p8rx[0];
		p8rx-=x;
	}
	LEN_RxOAD++;
	while(n--)
	{
		if(p8rx[LEN_RxOAD]==0)
		{
			LEN_RxOAD+=5;
		}
		else
		{
			i=p8rx[LEN_RxOAD+5];
			i*=4;
			i+=6;
			LEN_RxOAD+=i;
		}
	}
	n=LEN_RxOAD-x;//CSD����
	LEN_RxOAD<<=16;
	if((n+2)>LENmax_TxSPACE)
	{
		if(UARTCtrl->pTxData)//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
		{
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
		}
		UARTCtrl->DAR=8;//Խ�磨8��
		return LEN_RxOAD;
	}
	MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+x,n);//copy CSD
	LEN_TxAPDU+=n;
	LENmax_TxSPACE-=n;
	if(UARTCtrl->pTxData==0)//�跢�����ݻ���ָ��(��̬����),�ֶ���֯,ÿ��ǰ��2�ֽڶγ���
	{
		//UARTCtrl->DAR=6;//���󲻴���           ��6��
		//return LEN_RxOAD|LEN_TxAPDU;
		UARTCtrl->LEN_TxData=0;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
	}
	if(UARTCtrl->DAR)
	{//����
		ms->ms_free(UARTCtrl->pTxData);
		UARTCtrl->pTxData=0;
		//return LEN_RxOAD|LEN_TxAPDU;
		UARTCtrl->DAR=0;
		UARTCtrl->LEN_TxData=0;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
	}
	
	p8tx[LEN_TxAPDU]=1;//01 ���� ��¼����
	M_Offset=LEN_TxAPDU+1;//M����¼ƫ��
	LEN_TxAPDU+=2;
	LENmax_TxSPACE-=2;
	LEN_TxData=UARTCtrl->LEN_TxData;//�跢�������ܳ���(����ÿ��ǰ��2�ֽڶγ���)
	LEN_ReadData=UARTCtrl->LEN_ReadData;
	pDATA=UARTCtrl->pTxData;
	n=0;
	while(1)
	{
		if(LEN_ReadData>=LEN_TxData)
		{
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
			break;
		}
		if(n>=127)
		{
			break;
		}
		x=pDATA[LEN_ReadData]+(pDATA[LEN_ReadData+1]<<8);
		if(x<2)
		{
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
			break;
		}
		x-=2;
		if(x>LENmax_TxSPACE)
		{
			break;
		}
		MR((u32)p8tx+LEN_TxAPDU,(u32)pDATA+LEN_ReadData+2,x);
		LEN_TxAPDU+=x;
		LENmax_TxSPACE-=x;
		LEN_ReadData+=x+2;
		n++;//M����¼
	}
	UARTCtrl->LEN_ReadData=LEN_ReadData;
	p8tx[M_Offset]=n;//M����¼
	if(UARTCtrl->pTxData==0)
	{//����֡
		UARTCtrl->NEXT=0;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	}
	else
	{//����֡
		UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
		UARTCtrl->NEXTBlock=1;
	}
	return LEN_RxOAD|LEN_TxAPDU;
}


u32 GET_RecordOAD_Next(u32 PORTn)//��һ����¼�Ͷ������Է�֡��Ӧ����һ֡;���أ�LEN_TxAPDU
{
	u32 i;
	u32 x;
	u32 n;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8;
	u8* p8rx;
	u8 *p8tx;
//	u16* p16;
	u32 LEN_SA;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;
	u32 M_Offset;
	u32 LEN_TxData;
	u32 LEN_ReadData;
	ms_Type *ms;
	ClientConnectInfo_TypeDef* ClientConnectInfo;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->pTxData==0)
	{
		return 0;
	}
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
	//��֡TX:68 D4 07 C3 05 01 00 00 00 00 00 10 A9 57 85 05 0D  00 00 01  02 01  60 12 03 00  03 00 60 42 02 00 00 20 2A 02 00 01 50 02 02 00 03 20 21 02 00 00 10 02 00 00 20 02 00 01 18 1C 07 E0 0B 0B 00 00 00 55 07 05 00 00 00 00 00 01 01 03 1C 07 E0 0B 0B 00 00 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 1C 07 E0 0B 0B 00 0F 00 55 07 05 00 00 00 00 00 01 01 03 1C 07 E0 0B 0B 00 0F 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 1C 07 E0 0B 0B 00 1E 00 55 07 05 00 00 00 00 00 01 01 03 1C 07 E0 0B 0B 00 1E 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 1C 07 E0 0B 0B 00 2D 00 55 07 05 00 00 00 00 00 01 01 03 1C 07 E0 0B 0B 00 2D 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 ...
	//copy Tx��Rx
	LEN_SA=p8tx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	x=5+LEN_SA+3+8+4;
	n=p8tx[x];//SEQUENCE OF CSD����
	x++;
	while(n--)
	{
		if(p8tx[x]==0)
		{//OAD
			x+=5;
		}
		else
		{//ROAD
			i=p8tx[x+5];
			i*=4;
			i+=6;
			x+=i;
		}
	}
	i=Get_LEN_UARTnRx(PORTn);
	if(x>i)
	{
		UARTCtrl->pTxData=0;
		return 0;
	}
	MR((u32)p8rx,(u32)p8tx,x);
	//copy OAD��CSD
	LEN_TxAPDU=x-(5+LEN_SA+3+8);
	MR((u32)p8tx+TxAPDUOffset+3,(u32)p8rx+5+LEN_SA+3+8,LEN_TxAPDU);
	
	p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
	p8tx[0]=133;//��ȡ��Ӧ         	[133]	GET-Response
	p8tx[1]=5;//[3] GeResponseRecord,,[5]  GetResponseNext
	p8tx[2]=UARTCtrl->PIID;//PIID-ACD
	//p8tx[3]=p8rx[0];//OAD
	//p8tx[4]=p8rx[1];
	//p8tx[5]=p8rx[2];
	//p8tx[6]=p8rx[3];
	LEN_TxAPDU+=3;
	LENmax_TxSPACE-=LEN_TxAPDU;
	
	p8tx[LEN_TxAPDU]=1;//01 ���� ��¼����
	M_Offset=LEN_TxAPDU+1;//M����¼ƫ��
	LEN_TxAPDU+=2;
	LENmax_TxSPACE-=2;
	LEN_TxData=UARTCtrl->LEN_TxData;
	LEN_ReadData=UARTCtrl->LEN_ReadData;
	p8=UARTCtrl->pTxData;
	n=0;
	while(1)
	{
		if(LEN_ReadData>=LEN_TxData)
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
			break;
		}
		if(n>=127)
		{
			break;
		}
		x=p8[LEN_ReadData]+(p8[LEN_ReadData+1]<<8);
		if(x<2)
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
			break;
		}
		x-=2;
		if(x>LENmax_TxSPACE)
		{
			break;
		}
		MR((u32)p8tx+LEN_TxAPDU,(u32)p8+LEN_ReadData+2,x);
		LEN_TxAPDU+=x;
		LENmax_TxSPACE-=x;
		LEN_ReadData+=x+2;
		n++;//M����¼
	}
	UARTCtrl->LEN_ReadData=LEN_ReadData;
	p8tx[M_Offset]=n;//M����¼
	if(UARTCtrl->pTxData==0)
	{//����֡
		UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	}
	else
	{//����֡
		UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	}
//	//copy rx��rx+Rxoffset
//	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
//	i=Get_LEN_UARTnRx(PORTn);
//	x=p8rx[1]+(p8rx[2]<<8);
//	x+=3;//������
//	x>>=1;
//	x<<=1;
//	if((x+Rxoffset)>i)
//	{
//		x=i-Rxoffset;
//	}
//	x/=2;
//	p16=(u16*)p8rx;
//	while(x)
//	{
//		x--;
//		p16[(Rxoffset/2)+x]=p16[x];
//	}
	return LEN_TxAPDU;
}























