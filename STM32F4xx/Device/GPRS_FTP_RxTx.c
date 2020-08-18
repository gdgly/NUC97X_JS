
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#endif
#include "GPRS_FTP_RxTx.h"
#include "../Terminal/ATString.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/ascii.h"
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../OS/OS.h"
#include "../MS/MS.h"
#include "../OS/My_Heap.h"


u32 GPRS_FTP_Tx(void)//GPRS_FTP_Tx����;���ط����ֽ���
{
	u32 i;
	u32 x;
	u32 y;
	u32 TxByte;
	u16 *p16;
	u8 *p8tx;
	u32 FTPFileCount;//FTP�ļ����ظ�������
#if MainProtocol==698
	u8* p8;
#endif
	
	p8tx=(u8*)(ADDR_ATTx);
	TxByte=0;
	FTPFileCount=MRR(ADDR_FTPFileFlags+4,1);
	switch(UART1Ctrl->LinkTask2)//ͨ��2��������(FTP����)
	{
		case 0://��������ȹر��ļ�����
			TxByte=CopyString((u8 *)"AT$MYFTPCLOSE\r",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			break;
		case 1://����FTP�ļ�����
			TxByte=CopyString((u8 *)"AT$MYFTPOPEN=0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
		#if MainProtocol==376
			//Ŀ��IP ��ַ,Ŀ�Ķ˿ں�
			MR(ADDR_DATABUFF,ADDR_AFN0FF1+11,LEN_AFN0FF1-11);
			i=WinDataComp((u32)(u8 *)"//",ADDR_DATABUFF,2,64);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
			if(i==0)
			{//��IP
				UART1Ctrl->FTPErr=1;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				return 0;
			}
			i+=2;
			y=WinDataComp((u32)(u8 *)",",i,1,128);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
			if(y==0)
			{//��IP��Ŀ¼
				UART1Ctrl->FTPErr=1;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				return 0;
			}
			x=WinDataComp((u32)(u8 *)"/",i,1,128);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
			if(x>y)
			{//��Ŀ¼
				x=y;
			}
			MC(0,x,1);//�ַ�'/'��Ϊ��������0
			p8tx[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)i,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			//�˺�
			i=WinDataComp((u32)(u8 *)",",x,1,128);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
			if(i==0)
			{//��Ŀ¼
				i=x;
			}
			i+=1;
			x=WinDataComp((u32)(u8 *)"/",i,1,128);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
			if(x==0)
			{//���˺�
				UART1Ctrl->FTPErr=1;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				return 0;
			}
			MC(0,x,1);//�ַ�'/'��Ϊ��������0
			p8tx[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)i,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			//����
			p8tx[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)(x+1),p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='1';//0��ʾ����ģʽ��1��ʾ����ģʽ��Ĭ�ϱ���ģʽ��
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='3';//5-180s��FTP����������ݿ��г�ʱ���ã���λ�룻Ĭ��Ϊ30��
			TxByte++;
			p8tx[TxByte]='0';//
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='0';//0��ʾ������ģʽ (Ĭ��)��1��ʾ�ı�ģʽ
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
		#else
			//MainProtocol=698
//����4����������Ϣ����=structure
//{
//IP��ַ  octet-string��
//�˿�    long-unsigned��
//�û���  visible-string��
//����    visible-string
//}
			//Ŀ��IP ��ַ,Ŀ�Ķ˿ں�
			MR(ADDR_DATABUFF,ADDR_F002_4,LENmax_F002_4);
			p8=Get_Element((u8*)ADDR_DATABUFF,1,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8==0)
			{//��IP
				UART1Ctrl->FTPErr=1;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				return 0;
			}
			if(p8[1]!=4)
			{
				UART1Ctrl->FTPErr=1;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			IP4_HEX_ASCII(p8+2,(u8 *)ADDR_DATABUFF+1024);
			TxByte+=CopyString((u8 *)ADDR_DATABUFF+1024,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8=Get_Element((u8*)ADDR_DATABUFF,2,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8)
			{//�ж˿�����
				if(p8[0]==DataType_long_unsigned)
				{
					i=(p8[1]<<8)|p8[2];
					if(i)
					{//�˿ں�!=0
						p8tx[TxByte]=':';
						TxByte+=1;
						TxByte+=HEX_ASCII(i,(u32)p8tx+TxByte);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					}
				}
			}
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			//�˺�
			p8=Get_Element((u8*)ADDR_DATABUFF,3,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8==0)
			{//���˺�
				UART1Ctrl->FTPErr=1;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			i=p8[1];
			MR((u32)p8tx+TxByte,(u32)p8+2,i);
			TxByte+=i;
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			//����
			p8=Get_Element((u8*)ADDR_DATABUFF,4,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8==0)
			{//�޿���
				UART1Ctrl->FTPErr=1;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			i=p8[1];
			MR((u32)p8tx+TxByte,(u32)p8+2,i);
			TxByte+=i;
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='1';//0��ʾ����ģʽ��1��ʾ����ģʽ��Ĭ�ϱ���ģʽ��
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='3';//5-180s��FTP����������ݿ��г�ʱ���ã���λ�룻Ĭ��Ϊ30��
			TxByte++;
			p8tx[TxByte]='0';//
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='0';//0��ʾ������ģʽ (Ĭ��)��1��ʾ�ı�ģʽ
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
		#endif
			break;
		case 2://��ȡFTP�ļ���С
			TxByte=CopyString((u8 *)"AT$MYFTPSIZE=",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='/';
			TxByte++;
		#if MainProtocol==376
			MC(0,ADDR_DATABUFF,512);
			MR(ADDR_DATABUFF,ADDR_AFN0FF1+11,LEN_AFN0FF1-11);
			x=WinDataComp((u32)(u8 *)"/",ADDR_DATABUFF+12,1,128);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
			if(x!=0)
			{//��Ŀ¼
				x+=1;
				y=WinDataComp((u32)(u8 *)",",x,1,128);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
				if(y!=0)
				{//��Ŀ¼
					MC(0,y,1);
					p8tx[TxByte]='/';
					TxByte++;
					TxByte+=CopyString((u8 *)x,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				}
			}
			
		#else
			//MainProtocol==698
			p8=Get_Element((u8*)ADDR_F002_7,1,1,LENmax_F002_7);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8)
			{//�ҵ��ļ�·��������
				i=p8[1];
				if(i)
				{
					p8+=2;
					if(p8[0]=='/')
					{//���ļ�·��
						//�����·�����ַ������������ļ�����
						while(i)
						{
							i--;
							if(p8[i]=='/')
							{
								break;
							}
						}
						MR((u32)p8tx+TxByte,(u32)p8,i);
						TxByte+=i;
					}
				}
			}
		#endif
			TxByte+=CopyString((u8 *)FileUserDIR,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			p8tx[TxByte]='/';
			TxByte++;
			switch(FTPFileCount)//FTP�ļ����ظ�������
			{
				case 0:
					//TxByte+=CopyString((u8 *)FileFTP_MS,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 1:
					//TxByte+=CopyString((u8 *)FileFTP_OS,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 2:
					//TxByte+=CopyString((u8 *)FileFTP_Meter,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 3:
					//TxByte+=CopyString((u8 *)FileFTP_Terminal,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 4:
				#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=
					//TxByte+=CopyString((u8 *)FileFTP_Menu,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				#else
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
					return 0;
				#endif
				case 5:
				#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=
					//TxByte+=CopyString((u8 *)FileFTP_Font,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				#else
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
					return 0;
				#endif
				case 6:
					//TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 7:
			#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=			
					switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=
					{
						case 1:
							TxByte+=CopyString((u8 *)FileFTP_PLC_50,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 2:
							TxByte+=CopyString((u8 *)FileFTP_PLC_TC,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 3:
							TxByte+=CopyString((u8 *)FileFTP_PLC_ES,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 4:
							TxByte+=CopyString((u8 *)FileFTP_PLC_CX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 5:
							TxByte+=CopyString((u8 *)FileFTP_PLC_01,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 6:
							TxByte+=CopyString((u8 *)FileFTP_PLC_LH,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 7:
							TxByte+=CopyString((u8 *)FileFTP_PLC_NX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 8:
							TxByte+=CopyString((u8 *)FileFTP_PLC_QL,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 101:
							TxByte+=CopyString((u8 *)FileFTP_PLC_MX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 102:
							TxByte+=CopyString((u8 *)FileFTP_PLC_FC,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 103:
							TxByte+=CopyString((u8 *)FileFTP_PLC_NW,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 104:
							TxByte+=CopyString((u8 *)FileFTP_PLC_SR,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
							
						default:
							MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
							MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
							UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
							UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
							return 0;
					}
					break;
			#endif
			#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=			
					TxByte+=CopyString((u8 *)FileFTP_MODULE,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
			#endif
			#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=
					if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
					{
						switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=
						{
							case 1:
								TxByte+=CopyString((u8 *)FileFTP_PLC_50,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 2:
								TxByte+=CopyString((u8 *)FileFTP_PLC_TC,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 3:
								TxByte+=CopyString((u8 *)FileFTP_PLC_ES,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 4:
								TxByte+=CopyString((u8 *)FileFTP_PLC_CX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 5:
								TxByte+=CopyString((u8 *)FileFTP_PLC_01,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 6:
								TxByte+=CopyString((u8 *)FileFTP_PLC_LH,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 7:
								TxByte+=CopyString((u8 *)FileFTP_PLC_NX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 8:
								TxByte+=CopyString((u8 *)FileFTP_PLC_QL,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 101:
								TxByte+=CopyString((u8 *)FileFTP_PLC_MX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 102:
								TxByte+=CopyString((u8 *)FileFTP_PLC_FC,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 103:
								TxByte+=CopyString((u8 *)FileFTP_PLC_NW,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 104:
								TxByte+=CopyString((u8 *)FileFTP_PLC_SR,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
								
							default:
								MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
								MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
								UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
								UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
								return 0;
						}
						break;
					}
					else
					{
						TxByte+=CopyString((u8 *)FileFTP_MODULE,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						break;
					}
			#endif
			#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
					return 0;
			#endif
				default:
					UART1Ctrl->LinkTask2=6;//ͨ��2��������(FTP����)//6=�ر�FTP����
					return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
		case 3://�õ��ļ�����(���
			TxByte=CopyString((u8 *)"AT$MYFTPGET=",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='/';
			TxByte++;
		#if MainProtocol==376
			MC(0,ADDR_DATABUFF,512);
			MR(ADDR_DATABUFF,ADDR_AFN0FF1+11,LEN_AFN0FF1-11);
			x=WinDataComp((u32)(u8 *)"/",ADDR_DATABUFF+12,1,128);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
			if(x!=0)
			{//��Ŀ¼
				x+=1;
				y=WinDataComp((u32)(u8 *)",",x,1,128);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
				if(y!=0)
				{//��Ŀ¼
					MC(0,y,1);
					p8tx[TxByte]='/';
					TxByte++;
					TxByte+=CopyString((u8 *)x,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				}
			}
		#else
			//MainProtocol==698
			p8=Get_Element((u8*)ADDR_F002_7,1,1,LENmax_F002_7);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8)
			{//�ҵ��ļ�·��������
				i=p8[1];
				if(i)
				{
					p8+=2;
					if(p8[0]=='/')
					{//���ļ�·��
						//�����·�����ַ������������ļ�����
						while(i)
						{
							i--;
							if(p8[i]=='/')
							{
								break;
							}
						}
						MR((u32)p8tx+TxByte,(u32)p8,i);
						TxByte+=i;
					}
				}
			}
		#endif
			TxByte+=CopyString((u8 *)FileUserDIR,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			p8tx[TxByte]='/';
			TxByte++;
			switch(FTPFileCount)//FTP�ļ����ظ�������
			{
				case 0:
					//TxByte+=CopyString((u8 *)FileFTP_MS,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 1:
					//TxByte+=CopyString((u8 *)FileFTP_OS,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 2:
					//TxByte+=CopyString((u8 *)FileFTP_Meter,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 3:
					//TxByte+=CopyString((u8 *)FileFTP_Terminal,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 4:
				#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=
					//TxByte+=CopyString((u8 *)FileFTP_Menu,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				#else
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
					return 0;
				#endif
				case 5:
				#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=
					//TxByte+=CopyString((u8 *)FileFTP_Font,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				#else
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
					return 0;
				#endif
				case 6:
					//TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 7:
			#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=			
					switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=
					{
						case 1:
							TxByte+=CopyString((u8 *)FileFTP_PLC_50,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 2:
							TxByte+=CopyString((u8 *)FileFTP_PLC_TC,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 3:
							TxByte+=CopyString((u8 *)FileFTP_PLC_ES,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 4:
							TxByte+=CopyString((u8 *)FileFTP_PLC_CX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 5:
							TxByte+=CopyString((u8 *)FileFTP_PLC_01,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 6:
							TxByte+=CopyString((u8 *)FileFTP_PLC_LH,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 7:
							TxByte+=CopyString((u8 *)FileFTP_PLC_NX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 8:
							TxByte+=CopyString((u8 *)FileFTP_PLC_QL,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 101:
							TxByte+=CopyString((u8 *)FileFTP_PLC_MX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 102:
							TxByte+=CopyString((u8 *)FileFTP_PLC_FC,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 103:
							TxByte+=CopyString((u8 *)FileFTP_PLC_NW,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 104:
							TxByte+=CopyString((u8 *)FileFTP_PLC_SR,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						default:
							MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
							MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
							UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
							UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
							return 0;
					}
					break;
			#endif
			#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=			
					TxByte+=CopyString((u8 *)FileFTP_MODULE,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
			#endif
			#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=
					if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
					{
						switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=
						{
							case 1:
								TxByte+=CopyString((u8 *)FileFTP_PLC_50,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 2:
								TxByte+=CopyString((u8 *)FileFTP_PLC_TC,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 3:
								TxByte+=CopyString((u8 *)FileFTP_PLC_ES,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 4:
								TxByte+=CopyString((u8 *)FileFTP_PLC_CX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 5:
								TxByte+=CopyString((u8 *)FileFTP_PLC_01,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 6:
								TxByte+=CopyString((u8 *)FileFTP_PLC_LH,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 7:
								TxByte+=CopyString((u8 *)FileFTP_PLC_NX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 8:
								TxByte+=CopyString((u8 *)FileFTP_PLC_QL,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 101:
								TxByte+=CopyString((u8 *)FileFTP_PLC_MX,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 102:
								TxByte+=CopyString((u8 *)FileFTP_PLC_FC,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 103:
								TxByte+=CopyString((u8 *)FileFTP_PLC_NW,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							case 104:
								TxByte+=CopyString((u8 *)FileFTP_PLC_SR,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
								break;
							default:
								MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
								MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
								UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
								UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
								return 0;
						}
						break;
					}
					else
					{
						TxByte+=CopyString((u8 *)FileFTP_MODULE,p8tx+TxByte);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						break;
					}
			#endif
			#if ((Project/100)<2)||((Project/100)>4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��,700=
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
					return 0;
			#endif
				default:
					UART1Ctrl->FTPErr=2;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
					return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			//�ļ����ݵ�ƫ����
			p8tx[TxByte]=',';
			TxByte++;
			i=MRR((u32)&UART1Ctrl->FTPByte,4);
			i=hex_bcd(i);
			TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
			//��ȡ�ļ����ݵĳ���
			p8tx[TxByte]=',';
			TxByte++;
			x=MRR((u32)&UART1Ctrl->FTPFileLen,4);//FTP��ǰ�ļ��ܳ���
			y=MRR((u32)&UART1Ctrl->FTPByte,4);//FTP��ǰ�ļ�����������ֽ���
			if(x<=y)
			{
				UART1Ctrl->LinkTask2=2;//ͨ��2��������(FTP����)//2=��ȡFTP�ļ���С
				return 0;
			}
			x-=y;
			if(x>LEN_DATABUFF)
			{
				x=LEN_DATABUFF;
			}
			x=hex_bcd(x);
			TxByte+=BCD_ASCII(x,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
			p8tx[TxByte]='\r';
			TxByte++;
			break;
		case 4://�õ��ļ�����
			p16=(u16*)(ADDR_UART1IntATRx);
			if(p16[0]==p16[1])
			{
				Terminal_Ram->AT_MS_Timer=40000/10;//AT���յ�һ���ֽڳ�ʱ��ʱֵ(��)
			}
			UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
			return 0;
		case 5://һ���ļ�������ɣ��ȴ��������
//		  p32=(u32 *)(ADDR_DATABUFF);
//		  MR(ADDR_DATABUFF,ADDR_Download_ENTRY+(4*64),8);
//			if(p32[0]==0)
//			{//�ļ��Ѹ���
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)//2=��ȡFTP�ļ���С
//			}
			break;
		case 6://�ر�FTP����
			TxByte=CopyString((u8 *)"AT$MYFTPCLOSE\r",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			break;
		default:
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
			break;
	}
	return TxByte;
}


void GPRS_FTP_Rx(void)//GPRS_FTP_Rx����
{
	u32 i;
	u32 x;
	u32 y;
	u32 RxByte;
	u32 Addr;
	u8 *p8s;
	u8 *p8d;
	u16 *p16fifo;
	u32 *p32;
	u32 p1;
	u32 FTPFileCount;//FTP�ļ����ظ�������
	os_Type *os;	
	ms_Type* ms;
#ifdef IC_STM32F4xx
	FILE *file;
#endif
#ifdef IC_NUC9xx
	int yaffs;
	char *pFileName;
#endif
	
	p16fifo=(u16*)(ADDR_UART1IntATRx);
	FTPFileCount=MRR(ADDR_FTPFileFlags+4,1);
	switch(UART1Ctrl->LinkTask2)//ͨ��2��������(FTP����)
	{
		case 0://��������ȹر��ļ�����
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			else
			{
				UART1Ctrl->LinkSubTask++;//����AT���ͼ���
				if(UART1Ctrl->LinkSubTask<=2)//����AT���ͼ���
				{
					return;
				}
			}
	#if MainProtocol==376
			i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
	#else	
			i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
			if(i)
			{
				i=MRR(i+1,2);;//��������(��)  long-unsigned
				i=(i>>8)+((i&0xff)<<8);
				i/=60;
			}
			else
			{
				i=0;
			}
	#endif
			if(i<=1)
			{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
				Terminal_Ram->GPRSWaitTx_S_Timer=15;//�ȴ�����FTP�ļ������붨ʱ��
			}
			else
			{
				Terminal_Ram->GPRSWaitTx_S_Timer=30;//�ȴ�����FTP�ļ������붨ʱ��
			}
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->LinkTask2++;//ͨ��2��������(FTP����)
			break;
		case 1://����FTP�ļ�����
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//��������(��)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->LinkTask2++;//ͨ��2��������(FTP����)
				return;
			}
			if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//�ȴ�����FTP�ļ������붨ʱ��
			{
		#if MainProtocol==376
				i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
		#else	
				i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
				if(i)
				{
					i=MRR(i+1,2);;//��������(��)  long-unsigned
					i=(i>>8)+((i&0xff)<<8);
					i/=60;
				}
				else
				{
					i=0;
				}
		#endif
				if(i<=1)
				{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
					Terminal_Ram->GPRSWaitTx_S_Timer=15;//�ȴ�����FTP�ļ������붨ʱ��
				}
				else
				{
					Terminal_Ram->GPRSWaitTx_S_Timer=30;//�ȴ�����FTP�ļ������붨ʱ��
				}
				UART1Ctrl->LinkSubTask++;//����AT���ͼ���
				if(UART1Ctrl->LinkSubTask>2)//����AT���ͼ���
				{
					UART1Ctrl->FTPErr=1;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				}
			}
			break;
		case 2://��ȡFTP�ļ���С
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//��������(��)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"$MYFTPSIZE: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				MC(0,ADDR_DATABUFF,64);
				i=GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				if(i!=0)
				{
					i=ASCII_HEX(ADDR_DATABUFF);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
					if((i>LENmax_Download)||(i==0))
					{//�ļ����ȴ���
						MWR(5,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
						MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
						UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
						UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
					}
					else
					{//�ļ�����OK
						MWR(1,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
						MWR(i,(u32)&UART1Ctrl->FTPFileLen,4);//FTP��ǰ�ļ��ܳ���
						MWR(0,(u32)&UART1Ctrl->FTPByte,4);//FTP��ǰ�ļ�����������ֽ���
						UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
						UART1Ctrl->LinkTask2++;//ͨ��2��������(FTP����)
					}
					return;
				}
			}
			UART1Ctrl->LinkSubTask++;//����AT���ͼ���
			if(UART1Ctrl->LinkSubTask>2)//����AT���ͼ���
			{
				MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
				MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
				//NextATCount();//��1��AT
				UART1Ctrl->SubTask=0;//������
				UART1Ctrl->ATCount++;
			}
			break;
		case 3://�õ��ļ�����(���
			Addr=ATRxSearch((u32)((u8*)"CONNECT\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//��������(��)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				if(p16fifo[1]>=(LEN_UART1IntATRx-10))
				{
					p16fifo[1]-=(LEN_UART1IntATRx-10);
				}
				UART1Ctrl->ATTask=1;//AT����:1=����;�����м�ձ������Ȳ������
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->LinkTask2++;//ͨ��2��������(FTP����)
				return;
			}
			UART1Ctrl->LinkSubTask++;//����AT���ͼ���
			if(UART1Ctrl->LinkSubTask>2)//����AT���ͼ���
			{
				MWR(5,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
				MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
			}
			break;
		case 4://�õ��ļ�����
			p1=p16fifo[1];//ԭp1
			//��ȡ�ֽڳ���
			RxByte=MRR((u32)&UART1Ctrl->FTPFileLen,4);//FTP��ǰ�ļ��ܳ���
			y=MRR((u32)&UART1Ctrl->FTPByte,4);//FTP��ǰ�ļ�����������ֽ���
			if(RxByte<=y)
			{
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
				return;
			}
			RxByte-=y;
			if(RxByte>LEN_DATABUFF)
			{
				RxByte=LEN_DATABUFF;
			}
			i=GetATRxByte();//�жϽ���,���㵱ǰp0��Ӧp1����δ�����ֽ���
			if(i<(RxByte+6))
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					p16fifo[1]=p1;//ԭp1
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//��������(��)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
				}
				else
				{
					UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
					UART1Ctrl->LinkTask2=3;//�õ��ļ�����(���
					//NextATCount();//��1��AT
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->SubTask=0;//������
					UART1Ctrl->ATCount++;
				}
				return;
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			//�����������Է�ADDR_DATABUFF
			x=p16fifo[1];
			while(x>=(LEN_UART1IntATRx-10))
			{
				x-=(LEN_UART1IntATRx-10);
			}
			p8s=(u8*)(ADDR_UART1IntATRx+10);
			p8d=(u8*)(ADDR_DATABUFF);
			for(i=0;i<RxByte;i++)
			{
				if(x==p16fifo[0])
				{
					break;
				}
				p8d[i]=p8s[x];
				x++;
				if(x>=(LEN_UART1IntATRx-10))
				{
					x=0;
				}
			}
			p16fifo[1]=x;//�޸��Ѷ�ָ��
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				//д
				if((y+RxByte)>LENmax_Download)//FTP��ǰ�ļ�����������ֽ���
				{
					goto FileErr;
				}
				else
				{
					MW(ADDR_DATABUFF,ADDR_Download_ENTRY+y,RxByte);
					y+=RxByte;
					MWR(y,(u32)&UART1Ctrl->FTPByte,4);

					x=MRR((u32)&UART1Ctrl->FTPFileLen,4);//FTP��ǰ�ļ��ܳ���
					if(y>=x)
					{//��ǰ�Ѷ�ȡ����>=FTP��ǰ�ļ��ܳ���,�������
						if(y!=x)
						{
							goto FileErr;
						}
						os=Get_os();
						ms=Get_ms();
					#ifdef IC_STM32F4xx
						i=os->ffree("N0:");
						if(i<LENmax_Download)
						{//�ռ䲻��
							//ɾ��TEST�ļ���
							os->fdelete("N0:/TEST/",NULL);//ɾ���ļ�
							i=os->ffree("N0:");
							if(i<LENmax_Download)
							{//�ռ仹����
								//ɾ��Program Files�ļ���
								os->fdelete("N0:/Program Files/",NULL);//ɾ���ļ�
							}
						}
					#endif
					#ifdef IC_NUC9xx
						i=os->yaffs_freespace("user/Update");
						if(i<LENmax_Download)
						{//�ռ䲻��
							//ɾ��TEST�ļ���
								os->yaffs_unlinkdir("user/TEST/");
								i=os->yaffs_freespace("user/Update");
								if(i<LENmax_Download)
								{//�ռ仹����
									//ɾ��Program Files�ļ���
									os->yaffs_unlinkdir("user/Program Files/");
								}
						}
					#endif
						p8s=ms->ms_malloc((size_t)LENmax_Download);//��̬�����ҵ��ڴ��
						p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//��̬�����ҵ��ڴ��
						if((p8s==0)||(p8d==0))
						{
							ms->Init_my_heap();
							p8s=ms->ms_malloc((size_t)LENmax_Download);//��̬�����ҵ��ڴ��
							p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//��̬�����ҵ��ڴ��
						}
						i=MRR(ADDR_Download_ENTRY,1);
						if(i==0)
						{//��ѹ��
							MR((u32)p8s,ADDR_Download_ENTRY,LENmax_Download);
							p32=(u32*)p8s;
							//x=p32[111]-p32[110];//ԭʼ�ļ�����
							if(x>LENmax_Download)
							{
							FileErr:
								ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
								ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
								MWR(5,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
								MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
								UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
								UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
								return;
							}
					#ifdef IC_STM32F4xx
						#if MainProtocol==376
							switch(p32[128])//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
							{
								case 0:
									file=os->fopen("N0:/Download/MS.bin","w");
									break;
								case 1:
									file=os->fopen("N0:/Download/OS.bin","w");
									break;
								case 2:
									file=os->fopen("N0:/Download/Meter.bin","w");
									break;
								case 3:
									file=os->fopen("N0:/Download/Terminal.bin","w");
									break;
								case 4:
									file=os->fopen("N0:/Download/Menu.bin","w");
									break;
								case 5:
									file=os->fopen("N0:/Download/Font.bin","w");
									break;
								case 7:
									file=os->fopen("N0:/Download/Module.bin","w");
									break;
								default:
									goto FileErr;
							}
						#else
							switch(p32[128])//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
							{
								case 0:
									file=os->fopen("N0:/Download/MS698.bin","w");
									break;
								case 1:
									file=os->fopen("N0:/Download/OS698.bin","w");
									break;
								case 2:
									file=os->fopen("N0:/Download/Meter698.bin","w");
									break;
								case 3:
									file=os->fopen("N0:/Download/Terminal698.bin","w");
									break;
								case 4:
									file=os->fopen("N0:/Download/Menu698.bin","w");
									break;
								case 5:
									file=os->fopen("N0:/Download/Font.bin","w");
									break;
								case 7:
									file=os->fopen("N0:/Download/Module.bin","w");
									break;
								default:
									goto FileErr;
							}
						#endif
					#endif
						}
						else
						{//Lzmaѹ��
							MR((u32)p8s,ADDR_Download_ENTRY,LENmax_Download);
							i=p8s[5]+(p8s[6]<<8)+(p8s[7]<<16)+(p8s[8]<<24);
							//x=ԭʼ�ļ�����
							if((i>(2*LENmax_Download))||(x>LENmax_Download))
							{
								goto FileErr;
							}
							y=os->RAM_LzmaDecode((u32)p8s,x,(u32)p8d,i);//RAM->RAM��Lzma��ѹ���㷨;����:��ѹ����ĳ���
							if(y!=i)
							{
								goto FileErr;
							}
							p32=(u32*)p8d;
					#ifdef IC_STM32F4xx
						#if MainProtocol==376
							switch(p32[128])//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
							{
								case 0:
									file=os->fopen("N0:/Download/MS.zip","w");
									break;
								case 1:
									file=os->fopen("N0:/Download/OS.zip","w");
									break;
								case 2:
									file=os->fopen("N0:/Download/Meter.zip","w");
									break;
								case 3:
									file=os->fopen("N0:/Download/Terminal.zip","w");
									break;
								case 4:
									file=os->fopen("N0:/Download/Menu.zip","w");
									break;
								case 5:
									file=os->fopen("N0:/Download/Font.zip","w");
									break;
								case 6:
									file=os->fopen("N0:/Download/PACK.zip","w");
									break;
								case 7:
									file=os->fopen("N0:/Download/Module.zip","w");
									break;
								default:
									goto FileErr;
							}
						#else
							switch(p32[128])//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
							{
								case 0:
									file=os->fopen("N0:/Download/MS698.zip","w");
									break;
								case 1:
									file=os->fopen("N0:/Download/OS698.zip","w");
									break;
								case 2:
									file=os->fopen("N0:/Download/Meter698.zip","w");
									break;
								case 3:
									file=os->fopen("N0:/Download/Terminal698.zip","w");
									break;
								case 4:
									file=os->fopen("N0:/Download/Menu698.zip","w");
									break;
								case 5:
									file=os->fopen("N0:/Download/Font.zip","w");
									break;
								case 6:
									file=os->fopen("N0:/Download/PACK698.zip","w");
									break;
								case 7:
									file=os->fopen("N0:/Download/Module.zip","w");
									break;
								default:
									goto FileErr;
							}
						#endif
						}
						if(file==NULL)
						{
							goto FileErr;
						}
						os->fwrite(p8s,1,x,file);
						os->fclose(file);
				#endif
				#ifdef IC_NUC9xx
						pFileName=(char*)ADDR_DATABUFF;
						pFileName=StrCopy(pFileName,"user/Update/");
						switch(p32[128])//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
						{
							case 0:
								pFileName=StrCopy(pFileName,FileFactory_MS);
								break;
							case 1:
								pFileName=StrCopy(pFileName,FileFactory_OS);
								break;
							case 2:
								pFileName=StrCopy(pFileName,FileFactory_Meter);
								break;
							case 3:
								pFileName=StrCopy(pFileName,FileFactory_Terminal);
								break;
							case 4:
								pFileName=StrCopy(pFileName,FileFactory_Menu);
								break;
							case 5:
								pFileName=StrCopy(pFileName,FileFactory_Font);
								break;
							case 6:
								pFileName=StrCopy(pFileName,FileFactory_PACK);
								break;
							case 7:
								pFileName=StrCopy(pFileName,FileFactory_Modele);
								break;
							default:
								goto FileErr;
						}
					}
					pFileName[0]=0;
					yaffs=os->yaffs_open((char*)ADDR_DATABUFF,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
					if(yaffs<0)
					{
						goto FileErr;
					}
					os->yaffs_WRITE(yaffs,p8s,x);
					//os->yaffs_fsync(yaffs);
					os->yaffs_close(yaffs);
				#endif
						ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
						ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
						FileDownload->New=0;//�����ļ������־:0=��,1=û
						//OK
						if(FTPFileCount<7)
						{//PACK
							MC(2,ADDR_FTPFileFlags+5,7);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
							MWR(7,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
							UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
							UART1Ctrl->LinkTask2++;//ͨ��2��������(FTP����)//һ���ļ�������ɣ��ȴ��������
						}
						else
						{
							MWR(2,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
							MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
							UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
							UART1Ctrl->LinkTask2++;//ͨ��2��������(FTP����)//һ���ļ�������ɣ��ȴ��������
						}
						return;
					}
				}
				UART1Ctrl->LinkTask2=3;//�õ��ļ�����(���
				//NextATCount();//��1��AT
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
				UART1Ctrl->ATCount++;
				return;
			}
			if(Terminal_Ram->AT_MS_Timer!=0)
			{//����û���
				p16fifo[1]=p1;//ԭp1
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//��������(��)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
				if(i<=1)
				{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
					if(Terminal_Ram->AT_MS_Timer>(15000/10))
					{
						Terminal_Ram->AT_MS_Timer=(15000/10);
					}
				}
				return;
			}
			UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
			//NextATCount();//��1��AT
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->SubTask=0;//������
			UART1Ctrl->ATCount++;
			break;
		//case 5://һ���ļ�������ɣ��ȴ��������
		//	break;
		case 6://�ر�FTP����
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//��������(��)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//�����ļ�����+1
			break;
		default:
			if(Terminal_Ram->AT_MS_Timer!=0)
			{//����û���
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//�������� BIN min  1�ֽ�
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//��������(��)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
				if(i<=1)
				{//��������=1��(60��),�ȴ���ʱ��15��,����3��Ϊ45��,����3��ʱ�������������ʱ�����ӻ���LinkSubTask=0������ѭ���޷��˳�
					if(Terminal_Ram->AT_MS_Timer>(15000/10))
					{
						Terminal_Ram->AT_MS_Timer=(15000/10);
					}
				}
				return;
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
			break;
	}
}


