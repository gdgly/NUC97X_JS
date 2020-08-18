

//�ļ�����
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_FILE.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/ascii.h"
#include "../OS/OS.h"
#include "../MS/MS.h"
#include "../DL698/DL698_JSON.h"


//�ļ��ֿ鴫�����

u32 GET_Request_F001(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LEN_BLOCK;
	u32 LenRx;
	u8* p8s;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	switch((OAD>>8)&0xff)
	{
		case 4://�ļ������״̬��
			p8s=Get_Element((u8 *)ADDR_F001_7,2,0,LENmax_F001_7);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8s==0)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			LEN_BLOCK=(p8s[1]<<8)|p8s[2];//�����鳤��
			if(LEN_BLOCK==0)
			{
				UARTCtrl->DAR=255;//���ݷ��ʽ��;�ɹ�
				return 0;
			}
			p8s=Get_Element((u8 *)ADDR_F001_7+2,3,0,LENmax_F001_7);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8s)
			{
				i=(p8s[1]<<24)|(p8s[2]<<16)|(p8s[3]<<8)|p8s[4];//�ļ���С  double-long-unsigned
			}
			else
			{
				i=0;
			}
			if(i==0)
			{
				UARTCtrl->DAR=255;//���ݷ��ʽ��;�ɹ�
				return 0;
			}
			i+=LEN_BLOCK-1;
			i/=LEN_BLOCK;//������bit-string ����
			LenRx=i;
			LenRx+=7;
			LenRx/=8;//���ֽ���
			if(LenRx>LENmax_F001_4)
			{
				UARTCtrl->DAR=255;//���ݷ��ʽ��;�ɹ�
				return 0;
			}
			if(i<=127)
			{
				if(LENmax_TxSPACE<(LenRx+2))
				{
					UARTCtrl->DAR=255;//���ݷ��ʽ��;�ɹ�
					return 0;
				}
				p8tx[0]=DataType_bit_string;
				p8tx[1]=i;
				MR((u32)p8tx+2,ADDR_F001_4,LenRx);
				LenRx+=2;
			}
			else
			{
				if(LENmax_TxSPACE<(LenRx+4))
				{
					UARTCtrl->DAR=255;//���ݷ��ʽ��;�ɹ�
					return 0;
				}
				p8tx[0]=DataType_bit_string;
				p8tx[1]=0x82;
				p8tx[2]=i>>8;
				p8tx[3]=i;
				MR((u32)p8tx+4,ADDR_F001_4,LenRx);
				LenRx+=4;
			}
			return LenRx;
		default:
			UARTCtrl->DAR=255;//���ݷ��ʽ��;�ɹ�
			return 0;
	}
}

//���Ĵ���
u32 ACTION_F001(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 LenRx;
	u32 LenTx;
	u32 LEN_BLOCK;
	u32 NetFlag;
	u8* p8s;
	u8* p8s1;
#ifndef  LINUX
	u8* p8d;
#endif
	u32* p32;
	UARTCtrl_TypeDef *UARTCtrl;
#ifndef   LINUX
	os_Type *os;
#endif
	ms_Type* ms;
#ifdef IC_STM32F4xx
	FILE *file;
#endif
#ifdef IC_NUC9xx
#ifndef    LINUX
	int yaffs;
	char *pFileName;
#endif
#endif
#ifdef LINUX
	s32 fd;
#endif
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	switch(i)
	{
		case 1://1����λ
		case 2://2��ִ��
		case 3://3��ɾ��
		case 4://4��У��
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 5://5������
			p8s=Get_Element((u8 *)ADDR_F001_7+2,3,0,LENmax_F001_7);
			if(!p8s[0])
			{
				return 0; // �����ļ������ڣ�ֱ�ӷ���ȷ��֡
			}
			p8s = (u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_F001_7,LENmax_F001_7);
			if((p8rx[5] == 0xF2) && (p8rx[6] == 0x09) && (p8rx[7] == 0x02) && (p8rx[8] == 0x01))
			{
				if((p8rx[10] == 0xF2) && (p8rx[11] == 0x10) && (p8rx[12] == 0x02) && (p8rx[13] == 0x00))
				{
					p8s[34] = 4;	// ���������ͻ���STA
				}
				else
				{
					p8s[34] = 1;	// ���������ͻ���CCO
				}	
			}
			MW(ADDR_DATABUFF,ADDR_F001_7,LENmax_F001_7);
			Terminal_Router->UpDateTask = 16;	// ִ����������
			return 0;
		case 6://6. ����
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 7://����7���������䣨������,�ϴ�
			LenRx=0;
			LenTx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(i>LENmax_F001_7)
			{
				UARTCtrl->DAR=8;//Խ��(8)
				return 0;
			}
			LenRx+=i;
			MW((u32)p8rx,ADDR_F001_7,i);
			MC(0,ADDR_F001_4,LENmax_F001_4);//�崫���״̬��
			MC(0xff,ADDR_Download_ENTRY,512);//���ļ��ݴ���ļ�ͷ��Ϣ			
			return (LenRx<<16)+LenTx;
		case 8://����8��д�ļ���������,����
//������=structure
//{
//   �����  long-unsigned��
//   ������  octet-string
//}
//Ŀ���ļ��������򴴽��������㴫��״̬�֣�����Ŵ�0��ʼ��
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=(p8rx[3]<<8)|p8rx[4];//�����
			if(i>=(LENmax_F001_4*8))
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			p8rx+=5;
			LenRx=p8rx[1];//��֡�鳤��
			if((LenRx&0x80)==0)
			{
				p8rx+=2;
			}
			else
			{
				switch(LenRx)
				{
					case 0x81:
						LenRx=p8rx[2];//��֡�鳤��
						p8rx+=3;
						break;
					case 0x82:
						LenRx=(p8rx[2]<<8)|p8rx[3];//��֡�鳤��
						p8rx+=4;
						break;
					default:
						UARTCtrl->DAR=255;
						return 0;
				}
			}
			p8s=Get_Element((u8 *)ADDR_F001_7,2,0,LENmax_F001_7);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8s==0)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			LEN_BLOCK=(p8s[1]<<8)|p8s[2];//�����鳤��
			if(LenRx>LEN_BLOCK)
			{//�鳤��>�趨
				UARTCtrl->DAR=255;
				return 0;
			}
			if((LEN_BLOCK*i)>=LENmax_Download)
			{//>nand�ռ�
				UARTCtrl->DAR=255;
				return 0;
			}
			MW((u32)p8rx,ADDR_Download_ENTRY+(LEN_BLOCK*i),LEN_BLOCK);
			//�ļ������״̬��
	#if NUMmax_F001_4>LEN_DATABUFF
			#error
	#endif
			MR(ADDR_DATABUFF,ADDR_F001_4,LENmax_F001_4);
			p8s=(u8*)ADDR_DATABUFF;
			p8s+=i/8;
			i%=8;
			i=0x80>>i;
			p8s[0]|=i;
			MW(ADDR_DATABUFF,ADDR_F001_4,LENmax_F001_4);
			//�����ѽ��տ���
			p32=(u32*)(ADDR_DATABUFF);
			LenRx=0;
			for(i=0;i<(LENmax_F001_4/4);i++)
			{
				LenTx=p32[i];
				if(LenTx==0xffffffff)
				{
					LenRx+=32;//���տ�
				}
				else
				{
					LenTx=BitNum(LenTx);//������λΪ1�ĸ���(0-32)
					LenRx+=LenTx;
					break;
				}
			}
			LenRx*=LEN_BLOCK;//�ѽ����ļ�����
			p8s=Get_Element((u8 *)ADDR_F001_7+2,3,0,LENmax_F001_7);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8s)
			{
				x=(p8s[1]<<24)|(p8s[2]<<16)|(p8s[3]<<8)|p8s[4];//�ļ���С  double-long-unsigned
				p8s1=Get_Element((u8 *)ADDR_F001_7+2,6,0,LENmax_F001_7);
				if(p8s1)
				{
					// CCO ���� STA
					if((p8s1[1] == 1) ||(p8s1[1] == 4))
					{
						NetFlag = 1;
					}
					else if(p8s1[1] == 255)
					{
						NetFlag = 2;
					}
					else
					{
						NetFlag = 0;
					}
				}
				else
				{
						NetFlag = 0;
				}
#ifdef LINUX
				if((LenRx>=x)&&(LenRx>1024))
				{//ȫ������
					//os=Get_os();
					ms=Get_ms();

					fd=ms->link_open("/usr/app/pack.tar.gz",O_CREAT|O_RDWR);
					if(fd==-1)
					{
						Crl_AllJsonFileAndTar();//ɾ��JSON���е��ļ�����
						fd=ms->link_open("/usr/app/pack.tar.gz",O_CREAT|O_RDWR);
						if(fd==-1)
						{//Զ������ʧ��
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:Զ������ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							UARTCtrl->DAR=255;
							return 0;
						}
					}
					p8s=ms->ms_malloc((size_t)x);//��̬�����ҵ��ڴ��
					if(p8s==0)
					{
						ms->Init_my_heap();
						p8s=ms->ms_malloc((size_t)x);//��̬�����ҵ��ڴ��
						if(p8s==0)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:�ڴ�ѿռ�����ʧ��\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							ms->link_close(fd);//�ر��ļ����
							UARTCtrl->DAR=255;
							return 0;
						}
					}
					MR((u32)p8s,ADDR_Download_ENTRY,x);
					ms->link_lseek(fd,0,SEEK_SET);
					i=ms->link_write(fd,(u8 *)p8s,x);
					if(i==x)
					{//д���ļ��ɹ�
						FileDownload->New=0;//�����ļ������־:0=��,1=û
						FileDownload->PORTn=PORTn;//������صĶ˿ں�
					}
					ms->link_close(fd);//�ر��ļ����
					ms->ms_free(p8s);//�ͷŵ�����Ŀռ�
				}
#else
				if((LenRx>=x)&&(LenRx>1024))
				{//ȫ������
					os=Get_os();
					ms=Get_ms();
				#ifdef IC_STM32F4xx
					i=os->ffree("N0:");
					if(i<x)
					{//�ռ䲻��
						if(NetFlag > 1)
						{
							MC(0,ADDR_F001_7,LENmax_F001_7);
						}
						else
						{
							//ɾ��TEST�ļ���
							os->fdelete("N0:/TEST/",NULL);//ɾ���ļ�
							i=os->ffree("N0:");
							if(i<LenTx)
							{//�ռ仹����
								//ɾ��Program Files�ļ���
								os->fdelete("N0:/Program Files/",NULL);//ɾ���ļ�
							}
						}
					}
				#endif
				#ifdef IC_NUC9xx
					i=os->yaffs_freespace("user/Update");
					if(i<x)
					{//�ռ䲻��
						//ɾ��TEST�ļ���
						os->yaffs_unlinkdir("user/TEST/");
						i=os->yaffs_freespace("user/Update");
						if(i<LenTx)
						{//�ռ仹����
							//ɾ��Program Files�ļ���
							os->yaffs_unlinkdir("user/Program Files/");
						}
					}
				#endif
					p8s=ms->ms_malloc((size_t)x);//��̬�����ҵ��ڴ��
					p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//��̬�����ҵ��ڴ��
					if((p8s==0)||(p8d==0))
					{
						ms->Init_my_heap();
						p8s=ms->ms_malloc((size_t)x);//��̬�����ҵ��ڴ��
						p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//��̬�����ҵ��ڴ��
					}
					MR((u32)p8s,ADDR_Download_ENTRY,x);
					if(NetFlag > 0)
						i=x;
					else
						i=p8s[5]+(p8s[6]<<8)+(p8s[7]<<16)+(p8s[8]<<24);//ԭʼ�ļ�����
					if(i>(2*LENmax_Download))
					{
						ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
						ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
						UARTCtrl->DAR=255;
						return 0;
					}
					// HPLC CCO���� STA���� bin�ļ�����Ҫtar
					if(NetFlag > 0)
					{
					#ifdef IC_STM32F4xx
						file=os->fopen("N0:/Download/CCOorSTA.bin","w");
					#endif
					}
					else
					{
					LenRx=os->RAM_LzmaDecode((u32)p8s,x,(u32)p8d,i);//RAM->RAM��Lzma��ѹ���㷨;����:��ѹ����ĳ���
					if(LenRx!=i)
					{
						ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
						ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
						UARTCtrl->DAR=255;
						return 0;
					}
					p32=(u32*)p8d;
				#ifdef IC_STM32F4xx
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
							file=os->fopen("N0:/Download/Module698.zip","w");
							break;
						default:
							ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
							ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
							UARTCtrl->DAR=255;
							return 0;
						}
					}
					if(file==NULL)
					{
						ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
						ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
						UARTCtrl->DAR=255;
						return 0;
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
							ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
							ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
							UARTCtrl->DAR=255;
							return 0;
					}
					pFileName[0]=0;
					yaffs=os->yaffs_open((char*)ADDR_DATABUFF,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
					if(yaffs<0)
					{
						ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
						ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
						UARTCtrl->DAR=255;
						return 0;
					}
					os->yaffs_WRITE(yaffs,p8s,x);
					//os->yaffs_fsync(yaffs);
					os->yaffs_close(yaffs);
					//д���򿪶�һ��
//					yaffs=os->yaffs_open((char*)ADDR_DATABUFF,O_RDWR,0);
//					os->yaffs_close(yaffs);
					//��ʱ�ȴ�����ֽڷ���
					Comm_Ram->WaitUartCompleteMSTimer=5000/10;//�ȴ�UART֡���ض�ʱ�� ÿ10ms��1��ʱ��
				#endif
					ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
					ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
					if(NetFlag > 0)
					{
						if(NetFlag == 2)
						{
							Terminal_Router->UpDateTask = 0;
						}
						else
						{
							Terminal_Router->UpDateTask = 16;
						}
						FileDownload->New=1;//�����ļ������־:0=��,1=û
					}
					else
					{
						FileDownload->New=0;//�����ļ������־:0=��,1=û
					}
					FileDownload->PORTn=PORTn;//������صĶ˿ں�
				}

			}
#endif
		}
		return 0;
		case 9://����9�����ļ���������
/*
//������=structure
//{
//   �����  long-unsigned
//}
//Ӧ���=structure
//{
//   ������  octet-string
//}
//Դ�ļ��������򷵻ش���Ŀ���ļ��������򴴽��������㴫��״̬�֡�
			p8rx+=4;
			i=(p8rx[3]<<8)|p8rx[4];//�����
			if(i>=(LENmax_F001_4*8))
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			MR(ADDR_DATABUFF,ADDR_F001_4,LENmax_F001_4);
			p8s=(u8*)ADDR_DATABUFF;
			p8s+=i/8;
			x=i%8;
			x=0x80>>x;
			if((p8s[0]&x)==0)
			{//��û����
				UARTCtrl->DAR=255;
				return 0;
			}
			p8s=Get_Element((u8 *)ADDR_F001_7,2,0,LENmax_F001_7);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8s==0)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			LEN_BLOCK=(p8s[1]<<8)|p8s[2];//�����鳤��
			if(LEN_BLOCK==0)
			{
				LEN_BLOCK=1024;
			}
			p8s=Get_Element((u8 *)ADDR_F001_7+2,3,0,LENmax_F001_7);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			x=0;
			if(p8s)
			{
				x=(p8s[1]<<24)|(p8s[2]<<16)|(p8s[3]<<8)|p8s[4];//�ļ���С  double-long-unsigned
			}
			i*=LEN_BLOCK;//��ַƫ��
			if(i>=x)
			{//����
				UARTCtrl->DAR=255;
				return 0;
			}
			LenTx=x-i;
			if(LenTx>LEN_BLOCK)
			{
				LenTx=LEN_BLOCK;
			}
			if((LenTx+6)>LENmax_TxSPACE)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			p8tx[0]=DataType_structure;
			p8tx[1]=1;
			p8tx[2]=DataType_octet_string;
			p8tx[3]=0x82;
			p8tx[4]=LenTx>>8;
			p8tx[5]=LenTx;
			MR((u32)p8tx+6,ADDR_Download_ENTRY+i,LenTx);
			return LenTx+6;
*/
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 10://����10������ȶԣ�������
//������=structure
//{
//CPU���        unsigned��
//��Կ����        unsigned��
//������ʼ��ַ    double-long-unsigned��
//������ʼ��ַ    double-long-unsigned��
//���������ݳ���  long-unsigned
//}
//Ӧ���=structure
//{
//�ȶԿ�����  octet-string
//}

			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		default:
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
	}
}


//FTP�������
u32 ACTION_F002(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	switch(i)
	{
		case 1://1����λ
		case 2://2��ִ��
		case 3://3��ɾ��
		case 4://4��У��
		case 5://5������
		case 6://6. ����
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 7://����7���ӷ��������أ��ļ���Ϣ��Э�����ͣ�
			LenRx=0;
			LenTx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(i>LENmax_F002_7)
			{
				UARTCtrl->DAR=8;//Խ��(8)
				return 0;
			}
			LenRx+=i;
			MW((u32)p8rx,ADDR_F002_7,i);
			MC(0xff,ADDR_Download_ENTRY,512);//���ļ��ݴ���ļ�ͷ��Ϣ
			//FTP�ļ����ر�־
			MC(0,ADDR_FTPFileFlags+4,1+16);
			//��������
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
			UARTCtrl->LinkTask2=0;//ͨ��2��������(FTP����)
			UARTCtrl->FTPLinkCount=0;//FTP�������Ӽ���
			UARTCtrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			UARTCtrl->LinkTask2=0;//ͨ��2��������(FTP����)
			UARTCtrl->FTPLinkCount=0;//FTP�������Ӽ���
			UARTCtrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
			return (LenRx<<16)+LenTx;
		case 8://����8���ϴ������������ļ���Ϣ��Э�����ͣ�
			UARTCtrl->DAR=255;
			return 0;
		default:
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
	}
}




