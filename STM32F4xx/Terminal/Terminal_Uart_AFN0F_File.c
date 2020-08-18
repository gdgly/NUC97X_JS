
//�ļ�����
#include "Project.h"
#include "Terminal_Uart_AFN0F_File.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../Device/IC_BUZZ.h"
#include "../OS/OS.h"
#include "../MS/MS.h"
#include "../OS/My_Heap.h"



void WriteFileSegmentFlag(u32 seg)//д�յ��ļ��α�־,���:��ǰ����
{
	u32 i;
	u32 Addr;
	
	if(seg>=1024)
	{
		return;
	}
	Addr=seg/8;
	Addr+=(ADDR_AFN0CF14+4);
	seg%=8;
	seg=1<<seg;
	seg=(~seg);
	i=MRR(Addr,1);
	i&=seg;
	MWR(i,Addr,1);
}

u32 CheckFileSegmentFlag(u32 seg)//����յ��ļ��α�־,���:�ܶ���;����:0û�յ��κζΣ�1���յ���2ȫ���յ�
{
	u32 i;
	u32 x;
	u32 f;
	u8 *p8;
	
	if(seg==0)
	{
		return 2;
	}
	if(seg>=1024)
	{
		return 0;
	}
	f=0;//b0=1�ж�û�յ�,b1=1�ж��յ�
	MR(ADDR_DATABUFF,ADDR_AFN0CF14+4,128);
	p8=(u8*)ADDR_DATABUFF;
	x=0;
	while(1)
	{
		for(i=0;i<8;i++)
		{
			if((p8[x]&(1<<i))!=0)
			{
				f|=1;//�ж�û�յ�
			}
			else
			{
				f|=2;//�ж��յ�
			}
			seg--;
			if(seg==0)
			{
				switch(f)
				{
					case 1:
						return 0;
					case 2:
						return 2;
					case 3:
						return 1;
				}
			}
		}
		x++;
	}
}




void Uart_AFN0F(u32 PORTn)//�ļ�����
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 DIlen;
	u32 Fn;
	u32 DIaddr;
	
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u32 * p32;
	u8 *p8s;
	u8 *p8d;
	FILE *file;
	os_Type *os;
	ms_Type* ms;
#if (USER/100)==5//�Ϻ�
	u8 * p8;
#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)(Get_ADDR_UARTnRx(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
#if (USER/100)==5//�Ϻ�
	if(DIlen<(12+0))
	{//F98��FTP�����ļ�����������PW
		goto ERR;
	}
#else
	if(DIlen<(12+16))
	{
		goto ERR;
	}
#endif

//PW��֤
	if(p8rxbuff[16]!=2)
	{//����F2�Զ����ļ������F98�Ϻ�FTP�����ļ�������
		i=Uart_376_PW(PORTn);//��Ϣ��֤���ֶ�PW;���PW��ַ,����0��Ч,1��Ч,4�ѷ�����֡
		if(i==4)
		{
			return;
		}
		if(i!=0x0)
		{
			goto ERR;
		}
	}
/*
//������������ÿ���
	if(PORTn==IRPORT)
	{
    if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_MeterFMTimer,2)==0))
		{
			goto ERR;
		}
	}
*/
//�û������ݱ�ʶ�Ͳ������ݳ���
	if(p8rxbuff[16]!=2)
	{//����F2�Զ����ļ������F98�Ϻ�FTP�����ļ�������
		if((p8rxbuff[13]&0x80)!=0x0)
		{//��Tp
			DIlen-=(8+16+6);
		}
		else
		{
			DIlen-=(8+16);
		}
	}
	else
	{//��F2�Զ����ļ������F98�Ϻ�FTP�����ļ�������
		if((p8rxbuff[13]&0x80)!=0x0)
		{//��Tp
			DIlen-=(8+6);
		}
		else
		{
			DIlen-=(8);
		}
	}

//����(����վ������10������1�����ݣ�����Ӧ�ò�����ȷ�ϣ�CON=1������·����)
	DIaddr=((u32)p8rxbuff)+14;
	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	if(DIlen<4)
	{
	ERR:
		//ȫ������
		Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		Uart_376_NoAUX(PORTn);//û������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		return;
	}
	else
	{
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		Fn=(i>>16);
		switch(Fn)
		{
			case 1://���䷽ʽ1
				if(DIlen<(4+11))
				{
					goto ERR;
				}
				if(p8rxbuff[18+0]==0)
				{//�ļ���ʶ=0,�����װ�ļ�,�ָ�������ǰ״̬��
					UARTCtrl->LinkTask2=0;//ͨ��2��������(FTP����)
					UARTCtrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
					MC(0,ADDR_AFN0CF14,4);//�ļ���װδ�յ����ݶ�:���
					MC(0xff,ADDR_AFN0CF14+4,128);//�ļ���װδ�յ����ݶ�:���ڸ����ݶ�δ�յ���־
					MC(0xff,ADDR_FTPFileFlags,5+16);//��FTP�ļ����ر�־
					MC(0,ADDR_AFN0FF1,LEN_AFN0FF1);
					MC(0,((u32)p8rxbuff)+18+5,4);
				F1RET:
					MR(((u32)p8txbuff)+18,((u32)p8rxbuff)+18+5,4);//���ص�ǰ�α�ʶ4�ֽ�
					i=4+4;//���س���
					i+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
					p8txbuff[1]=i;
					p8txbuff[2]=i>>8;
					DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
					Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				}
				else
				{
					switch(p8rxbuff[18+2])//�ļ�ָ��
					{
						case 0://���ķ�ʽ��װ
						/*
							if(UARTCtrl->AddrType>=2)//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
							{//��֡�����ַ��㲥��ַ
								i=MRR(((u32)p8rxbuff)+18+3,2);//�ܶ���
								i=CheckFileSegmentFlag(i);//����յ��ļ��α�־,���:�ܶ���;����:0û�յ��κζΣ�1���յ���2ȫ���յ�
								if(i==0)
								{
									return;//û����
								}
							}
						*/
						case 2://�������ַ����
							x=MRR(((u32)p8rxbuff)+18+3,2);//�ܶ���
							i=MRR(((u32)p8rxbuff)+18+5,4);//��i��
							if(i==0)
							{
								UARTCtrl->LinkTask2=0;//ͨ��2��������(FTP����)
								UARTCtrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
								MC(0,ADDR_AFN0CF14,4);//�ļ���װδ�յ����ݶ�:���
								MC(0xff,ADDR_AFN0CF14+4,128);//�ļ���װδ�յ����ݶ�:���ڸ����ݶ�δ�յ���־
								MC(0xff,ADDR_FTPFileFlags,5+16);//��FTP�ļ����ر�־
								MC(0,ADDR_AFN0FF1,LEN_AFN0FF1);
							}
							if((x>1024)||(x==0))
							{//�ܶ�����
								goto F1ERR;
							}
							if(i>=x)
							{//��i��>=�ܶ�����
						F1ERR:
								if(UARTCtrl->AddrType>1)//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
								{//��֡�����ַ��㲥��ַ
									return;
								}
								MC(0xff,((u32)p8rxbuff)+18+5,4);//���ص�ǰ�α�ʶ4�ֽ�Ϊ�ļ�����
								goto F1RET;
							}
							y=MRR(((u32)p8rxbuff)+18+9,2);//��i�����ݳ���Lf
							if((x*y)>LENmax_Download)
							{//����512K��
								goto F1ERR;
							}
							z=Get_LEN_UARTnRx(PORTn);//�õ�UART���ջ��峤��
							if((18+11+y+2)>z)
							{//��i�����ݳ���Lf>���ջ��峤�ȴ�
								goto F1ERR;
							}
						#if (USER/100)!=7//�����㽭�û�
							Data_Inverse(((u32)p8rxbuff)+18+11,y);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
						#endif
							z=MRR(ADDR_AFN0FF1+9,2);//ȡ����ĵ�i�����ݳ���Lf
							if((i+1)==x)
							{//������
								//�������ļ�����
								MWR((i*z)+y,ADDR_LEN_AFN0FF1,4);//376.1 F1�����ļ����ܳ��ȼ����Ĵ�
							}
							else
							{
								if(z!=0)
								{//���е�i�����ݳ���Lf
									if(z!=y)
									{
										goto F1ERR;
									}
								}
								z=y;//��i �����ݳ���Lf
								MWR(y,ADDR_AFN0FF1+9,2);//����ĵ�i�����ݳ���Lf
							}
							MW(((u32)p8rxbuff)+18+11,ADDR_Download_ENTRY+(i*z),y);
							WriteFileSegmentFlag(i);//д�յ��ļ��α�־,���:��ǰ����
							MW(((u32)p8rxbuff)+18,ADDR_AFN0FF1,1);//д�ļ���ʶ(��ʾ�õ�)
							i=CheckFileSegmentFlag(x);//����յ��ļ��α�־,���:�ܶ���;����:0û�յ��κζΣ�1���յ���2ȫ���յ�
							if(i==2)
							{//2ȫ���յ�
								MC(0,ADDR_AFN0FF1,LEN_AFN0FF1);//���ļ���ʶ(��ʾ�õ�)
								//
								os=Get_os();
								ms=Get_ms();
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
									x=p32[111]-p32[110];//ԭʼ�ļ�����
									if(x>LENmax_Download)
									{
										ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
										ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
										goto F1ERR;
									}
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
											ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
											ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
											goto F1ERR;
									}
								}
								else
								{//Lzmaѹ��
									MR((u32)p8s,ADDR_Download_ENTRY,LENmax_Download);
									i=p8s[5]+(p8s[6]<<8)+(p8s[7]<<16)+(p8s[8]<<24);
									x=MRR(ADDR_LEN_AFN0FF1,4);//ԭʼ�ļ�����
									if((i>(2*LENmax_Download))||(x>LENmax_Download))
									{
										ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
										ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
										goto F1ERR;
									}
									y=os->RAM_LzmaDecode((u32)p8s,x,(u32)p8d,i);//RAM->RAM��Lzma��ѹ���㷨;����:��ѹ����ĳ���
									if(y!=i)
									{
										ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
										ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
										goto F1ERR;
									}
									p32=(u32*)p8d;
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
											ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
											ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
											goto F1ERR;
									}
								}
								if(file==NULL)
								{
									ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
									ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
									goto F1ERR;
								}
								os->fwrite(p8s,1,x,file);
								os->fclose(file);
								ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
								ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
								FileDownload->New=0;//�����ļ������־:0=��,1=û
								FileDownload->PORTn=PORTn;//������صĶ˿ں�
							}
							if(UARTCtrl->AddrType>=2)//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
							{//��֡�����ַ��㲥��ַ
								return;
							}
							goto F1RET;
							
						case 1://FTP ��ʽ��װ
							//��������
							UARTCtrl->LinkTask2=0;//ͨ��2��������(FTP����)
							UARTCtrl->FTPLinkCount=0;//FTP�������Ӽ���
							UARTCtrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
							//FTP�ļ����ر�־
							MC(0,ADDR_FTPFileFlags+4,1+16);
							//�ļ���ʶ,ָ�IP��ַ��
							MC(0,ADDR_DATABUFF,LEN_AFN0FF1);
							i=DIlen-4;
							if(i>LEN_AFN0FF1)
							{
								i=LEN_AFN0FF1;
							}
							MR(ADDR_DATABUFF,((u32)p8rxbuff)+18,i);
							MW(ADDR_DATABUFF,ADDR_AFN0FF1,LEN_AFN0FF1);
							//�ݲ�����
							//FTP����֡��ǰ256�ֽڱ���
							MW((u32)p8rxbuff,ADDR_AFN0FF1_Rx,256);
							//FTP����֡��ǰ18�ֽڱ���
							MW((u32)p8txbuff,ADDR_AFN0FF1_Tx,18);
						#if (USER/100)==5//�Ϻ�
							//��F97FTP�����־
							MC(0,ADDR_AFN0FF97,1);
						#endif
							return;
						default:
							goto ERR;
					}
				}
				return;
		#if (USER/100)==5//�Ϻ�
			case 97://FTP�����ļ�����
				if(DIlen<(4+126))
				{
					goto ERR;
				}
				//��Ϣ����
				p8rxbuff[17]=1;//1byte ������־:0=����F97����,1=��F97�����
				MW(((u32)p8rxbuff)+17,ADDR_AFN0FF97,127);
				//��������
				UARTCtrl->LinkTask2=0;//ͨ��2��������(FTP����)
				UARTCtrl->FTPLinkCount=0;//FTP�������Ӽ���
				UARTCtrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				//FTP�ļ����ر�־
				MC(0,ADDR_FTPFileFlags+4,1+16);
				//ת��IP·����ΪF1�ļ���ʶ,ָ�IP��ַ��
				MC(0,ADDR_DATABUFF,LEN_AFN0FF1);
				p8=(u8*)(ADDR_DATABUFF);
				//�ļ���ʶ	BIN	1
				//�ļ�����	BIN	1
				p8[1]=1;//�ļ�ָ��	BIN	1
				//�ܶ���n	BIN	2
				//��i�α�ʶ��ƫ�ƣ�i=0��n-1��	BIN	4
				//��i�����ݳ���Lf	BIN	2
				//�ļ�����	BIN	Lf(ftp://�ļ�·��,�˺�/����ķ�ʽ��д)
					//ip
				i=11;
				i+=CopyString((u8 *)"ftp://",p8+i);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				for(x=0;x<4;x++)
				{
					y=hex_bcd(p8rxbuff[18+x]);//HEX��תΪ8λѹ��BCD��
					i+=BCD_ASCII(y,((u32)p8)+i);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8[i]='.';
					i++;
				}
					//�˿ں�
				i--;
				p8[i]=':';
				i++;
				y=hex_bcd(p8rxbuff[18+6]+(p8rxbuff[18+7]<<8));//HEX��תΪ8λѹ��BCD��
				i+=BCD_ASCII(y,((u32)p8)+i);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					//�ļ�·��
				p8[i]='/';
				i++;
				MC(0,ADDR_DATABUFF+256,21);
				MR(ADDR_DATABUFF+256,((u32)p8rxbuff)+18+36,20);
				i+=CopyString((u8 *)(ADDR_DATABUFF+256),p8+i);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					//�˺�
				p8[i]=',';
				i++;
				MC(0,ADDR_DATABUFF+256,11);
				MR(ADDR_DATABUFF+256,((u32)p8rxbuff)+18+16,10);
				i+=CopyString((u8 *)(ADDR_DATABUFF+256),p8+i);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					//����
				p8[i]='/';
				i++;
				MC(0,ADDR_DATABUFF+256,11);
				MR(ADDR_DATABUFF+256,((u32)p8rxbuff)+18+26,10);
				i+=CopyString((u8 *)(ADDR_DATABUFF+256),p8+i);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				MW(ADDR_DATABUFF,ADDR_AFN0FF1,LEN_AFN0FF1);
				
				//��F98 FTP�����ļ�����������
					//����ǰ�ն�����汾��	ASCII	10
				MC(0,ADDR_DATABUFF,20);
				p8[0]='Z';
				p8[1]='Z';
				p8[2]=((SVER>>4)&0xf)+0x30;
				p8[3]=((SVER>>0)&0xf)+0x30;
					//�������ն�����汾��	ASCII	10
					//����ʱ�䣺��ʱ������	����¼A.15	5
				MC(0xee,ADDR_DATABUFF+20,5);
				MW(ADDR_DATABUFF,ADDR_AFN0FF98,25);
				//ȫ��ȷ��
				Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				BUZZ_Generate(2);//��������,��ڷ�����
				return;
			case 98://FTP�����ļ�������
				MR(((u32)p8txbuff)+18,ADDR_AFN0FF98,25);//���ص�ǰ�α�ʶ4�ֽ�
				i=4+25;//���س���
				i+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
				p8txbuff[1]=i;
				p8txbuff[2]=i>>8;
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				return;
		#endif
/*
			case 2://���䷽ʽ2
				//DATABUFF��ʼ32�ֽ�ʹ��
				//1BYTE �ŵ�0=RS232,1=GPRS��RJ45
				//1BYTE ����
				//1BYTE �ļ��������	BIN	1
				//1BYTE �ļ����(0-255)	BIN	1
				//16BYTE �ļ�����	ASCII	16
				//2BYTE �ļ��ܶ���n(n>=1)	BIN	2
				//2BYTE �ļ����ݵ�ǰ�����ţ�i=1~n��	BIN	2
				//2BYTE �ļ�ÿ���ֽڳ���(L)	BIN	2
				//4BYTE ���������ֽ���
				//2BYTE ����
				if(DIlen<(4+24))
				{
					goto ERR;
				}
				p8=(u8 *)(ADDR_DATABUFF);
				MEMRead(ADDR_DATABUFF,ADDR_FileDown+(p8rxbuff[19]*32),32);
				if(p8rxbuff[19]<8)//�ļ����
				{//�ļ����=0-7
					if(p8rxbuff[18]<8)
					{//�ļ��������=0-7
						if(p8rxbuff[18]==0)
						{//�ļ��������=0������������
							MEMClr(0,ADDR_DATABUFF,32);
							if(PORTn!=RS232PORT)
							{//UART1
								p8[0]=1;
							}
							MEMRead(ADDR_DATABUFF+2,((u32)p8rxbuff)+18,20);
							MEMWrite(ADDR_DATABUFF,ADDR_FileDown+(p8rxbuff[19]*32),32);
RET_OK:
							Terminal_Ram->RequestFileTimer=0;//�����ļ�����MS��ʱ��
							//ȫ��ȷ��
							Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
							Uart_376_NoAUX(PORTn);//û������Ϣ
							Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
							return; 
						}
						if(p8rxbuff[18]==1)
						{//�ļ��������=1��ͣ
							MEMClr(1,ADDR_FileDown+2+(p8rxbuff[19]*32),1);
							goto RET_OK;
						}
			 			if(p8rxbuff[18]==2)
						{//�ļ��������=2����
 							MEMClr(2,ADDR_FileDown+2+(p8rxbuff[19]*32),1);
							goto RET_OK;
						}
						if(p8rxbuff[18]==3)
						{//�ļ��������=3ȡ��
 							MEMClr(3,ADDR_FileDown+2+(p8rxbuff[19]*32),1);
							goto RET_OK;
						}
						if(p8rxbuff[18]==4)
						{//�ļ��������=4�������
							goto RET_OK;
 						}
						if(p8rxbuff[18]==5)
						{//�ļ��������=5��������Ϣ
							MEMRead(((u32)p8txbuff)+18,ADDR_FileDown+2+(p8rxbuff[19]*32),24);
							p8txbuff[19]=p8rxbuff[19];//�ļ����
							p8txbuff[1]=8+4+24+(4*6);
							p8txbuff[3]=8+4+24+(4*6);
							p8txbuff[2]=0;
							p8txbuff[4]=0;
							p8txbuff+=18;
							p8txbuff[22]=4*6;//�ļ�ÿ���ֽڳ���(L)=4*�汾����
							p8txbuff[23]=0;
							MEMClr(' ',((u32)p8txbuff)+24,4*10);
							//Ӳ���汾��
							p8txbuff[24+0]=(HV>>8)+0x30;
							p8txbuff[24+1]='.';
							p8txbuff[24+2]=((HV>>4)&0xf)+0x30;
							p8txbuff[24+3]=(HV&0xf)+0x30;
							//���ģ��I0�汾��
							p32=(u32 *)(ADDR_DATABUFF);
							MEMRead(ADDR_DATABUFF,ADDR_I0_ENTRY+(4*68),4);
							MEMRead(ADDR_DATABUFF,p32[0]+16,4);
							i=p32[0];
							p8txbuff[28+0]=(i>>8)+0x30;
							p8txbuff[28+1]='.';
							p8txbuff[28+2]=((i>>4)&0xf)+0x30;
							p8txbuff[28+3]=(i&0xf)+0x30;
							//���ģ��I1�汾��
							MEMRead(ADDR_DATABUFF,ADDR_I1_ENTRY+(4*68),4);
							if((p32[0]>ADDR_I1_ENTRY)&&(p32[0]<(ADDR_I1_ENTRY+(256*1024))))
							{
								MEMRead(ADDR_DATABUFF,p32[0]+16,4);
								i=p32[0];
								p8txbuff[32+0]=(i>>8)+0x30;
								p8txbuff[32+1]='.';
								p8txbuff[32+2]=((i>>4)&0xf)+0x30;
								p8txbuff[32+3]=(i&0xf)+0x30;
							}
							//���ģ��I2�汾��
							MEMRead(ADDR_DATABUFF,ADDR_I2_ENTRY+(4*68),4);
							if((p32[0]>ADDR_I2_ENTRY)&&(p32[0]<(ADDR_I2_ENTRY+(256*1024))))
							{
								MEMRead(ADDR_DATABUFF,p32[0]+16,4);
								i=p32[0];
								p8txbuff[36+0]=(i>>8)+0x30;
								p8txbuff[36+1]='.';
								p8txbuff[36+2]=((i>>4)&0xf)+0x30;
								p8txbuff[36+3]=(i&0xf)+0x30;
							}
							//���ģ��E1�汾��
							MEMRead(ADDR_DATABUFF,ADDR_E1_ENTRY+(4*68),4);
							if((p32[0]>ADDR_E1_ENTRY)&&(p32[0]<(ADDR_E1_ENTRY+(256*1024))))
							{
								MEMRead(ADDR_DATABUFF,p32[0]+16,4);
								i=p32[0];
								p8txbuff[40+0]=(i>>8)+0x30;
								p8txbuff[40+1]='.';
								p8txbuff[40+2]=((i>>4)&0xf)+0x30;
								p8txbuff[40+3]=(i&0xf)+0x30;
							}
							//���ģ��E2�汾��
							MEMRead(ADDR_DATABUFF,ADDR_E2_ENTRY+(4*68),4);
							if((p32[0]>ADDR_E2_ENTRY)&&(p32[0]<(ADDR_E2_ENTRY+(256*1024))))
							{
								MEMRead(ADDR_DATABUFF,p32[0]+16,4);
								i=p32[0];
								p8txbuff[44+0]=(i>>8)+0x30;
								p8txbuff[44+1]='.';
								p8txbuff[44+2]=((i>>4)&0xf)+0x30;
								p8txbuff[44+3]=(i&0xf)+0x30;
							}
							//���ģ��E4�汾��
							
							
							p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
							Uart_376_NoAUX(PORTn);//û������Ϣ
							Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
							return; 
						}
						if(p8rxbuff[18]==6)
						{//�ļ��������=6��վ���ն��������ݵ���Ӧ���ն�����վ��������
							if((p8rxbuff[36]+(p8rxbuff[37]<<8))<(p8rxbuff[38]+(p8rxbuff[39]<<8)))
							{//�ܶ���<��ǰ������
								goto NOACK;
							}
							if((p8rxbuff[36]+(p8rxbuff[37]<<8))==0x0)
							{//�ܶ���=0
								goto NOACK;
							}
							if((p8rxbuff[38]+(p8rxbuff[39]<<8))==0x0)
							{//��ǰ������=0
								goto NOACK;
							}
							if((p8rxbuff[40]+(p8rxbuff[41]<<8))==0x0)
							{//��ǰ�γ���==0
								goto NOACK;
							}
							if((p8rxbuff[38]+(p8rxbuff[39]<<8))!=(p8[22]+(p8[23]<<8)+1))
							{//��ǰ������!=����κ�
								if(PORTn==ENETPORT)
								{
									Terminal_Ram->RequestFileTimer=5000/10;//��5����ٷ�����֡,�⴫����ʱ���������ͬ֡�������ظ�ѭ��
								}
								else
								{
									if(PORTn==GPRSPORT)
									{
										Terminal_Ram->RequestFileTimer=10000/10;//��10����ٷ�����֡,�⴫����ʱ���������ͬ֡�������ظ�ѭ��
									}
									else
									{
										if(PORTn==SMSPORT)
										{
											Terminal_Ram->RequestFileTimer=60000/10;//��60����ٷ�����֡,�⴫����ʱ���������ͬ֡�������ظ�ѭ��
										}
										else
										{//����ͨ��5S
											Terminal_Ram->RequestFileTimer=5000/10;//��5����ٷ�����֡,�⴫����ʱ���������ͬ֡�������ظ�ѭ��
										}
									}
								}
								return;
							}
							p8[22]=p8rxbuff[38];
							p8[23]=p8rxbuff[39];
							x=p8[26]+(p8[27]<<8)+(p8[28]<<16)+(p8[29]<<24);//���������ֽ���
							i=p8rxbuff[40]+(p8rxbuff[41]<<8);//��ǰ���ֽ���
							if((x+i)>(256*1024))
							{//(���������ֽ���+��ǰ���ֽ���)>((256*1024)-4)
								goto NOACK;
							}
							if(DIlen!=4+24+i)
							{//���ݵ�Ԫ��ʶ�����ݵĳ���!=4+24+L
								goto NOACK;
							}
							MEMWrite(((u32)p8rxbuff)+42,ADDR_Download_ENTRY+x,i);//д��ǰ������
							//�ܶ���,��ǰ������,ÿ���ֽڳ���,���������ֽ���
							x+=i;
							p8[26]=x;
							p8[27]=x>>8;
							p8[28]=x>>16;
							p8[29]=x>>24;
							MEMWrite(ADDR_DATABUFF,ADDR_FileDown+(p8rxbuff[19]*32),32);
							if((p8[20]+(p8[21]<<8))==(p8[22]+(p8[23]<<8)))
							{//��ǰ����=�ܶ���,���
								MEMClr(4,ADDR_FileDown+2+(p8rxbuff[19]*32),1);//4=������ɱ�־
								//Terminal_Ram->RequestFileTimer=2000/10;//�����ļ�������ʱ2��ȴ�д�����������
								//return;
							}
NOACK:
							//���ش���,ʹ�����ܷ�����֡,�ӿ촫���ٶ�
							Terminal_Ram->RequestFileTimer=0;//�����ļ�����MS��ʱ��
							
						}
						if(p8rxbuff[18]==7)
						{//�ļ��������=7������վIP
							MEMWrite(((u32)p8rxbuff)+20,ADDR_AFN04F3,12);//��վIP��ַ�Ͷ˿�
							WWDT_RESET();//����ʹ�ܿ��Ź���λ
							while(1)
							{
							}
						}
						
					}
				}
				break;
*/
			default:
				goto ERR;
		}
	}
}

/*
const u8 Request_File_Example[]=//�����ļ�֡
{
	0x68,36,0,36,0,0x68,
	0xCA,//������;D7(DIR���䷽��λ),D6(PRM������־λ),D5(֡����λFCB/Ҫ�����λACD),D4(֡������ЧλFCV/��),D3-D0(������)
	0,0,0,0,//�����ն˵�ַ��������
	0,//����ַ
	0x0F,//AFN=0X0F
	0x60,//֡������SEQ;D7(TpVʱ���ǩ��Чλ),R6(FIR��֡),D5(FINĩ֡),D4(CONȷ�ϱ�־λ),D3-D0(PSEQ/RSEQ)
	0,0,2,0,//���ݵ�Ԫ��ʶP0-F2
	6,//�ļ��������
	0,//�ļ����
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//�ļ�����	ASCII	16
	0,0,//�ļ��ܶ���n(n>=1)
	0,0,//�ļ����ݵ�ǰ�����ţ�i=1~n)
	0,0,//�ļ�ÿ���ֽڳ���(L)
};
void Request_File_Tx(u32 PORTn)//���������ļ�����֡
{
	u32 i;
	u8 * p8txbuff;
	u8 * p8;
  UARTCtrl_TypeDef * UARTCtrl;

	p8=(u8 *)(ADDR_DATABUFF);
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	MEMRead((u32)p8txbuff,(u32)Request_File_Example,42);
  //ȡ���ն˵�ַ
	MEMRead(((u32)p8txbuff)+7,ADDR_AREACODE,4);
	//ȡ���,����,�ܶ���,��ǰ����,ÿ���ֽڳ���
	MEMRead(((u32)p8txbuff)+19,ADDR_DATABUFF+3,23);
	//��ǰ����+1
	i=p8[22]+(p8[23]<<8);
	i++;
	p8txbuff[38]=i;
	p8txbuff[39]=i>>8;
	p8txbuff[13]|=(UARTCtrl->TxSEQ&0xf);//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ
	UARTCtrl->TxSEQ+=1;//����֡���PSEQ+1
	
	Uart_376_NoAUX(PORTn);//û������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}
*/

void Request_File(void)//�����ļ�����
{
/*
	u32 PORTn;
	u32 i;
	u32 FileParaAddr;
	u8 * p8;
	u16 * p16timer;
	u32 *p32;
  UARTCtrl_TypeDef * UARTCtrl;


  p32=(u32 *)(ADDR_DATABUFF);
  MEMRead(ADDR_DATABUFF,ADDR_Download_ENTRY+(4*64),8);//�洢����
	if((p32[1]>p32[0])&&((p32[1]-p32[0])<=0x40000))
	{
		MEMRead(ADDR_DATABUFF,ADDR_Download_ENTRY+(p32[1]-p32[0]-4),4);//�洢����
		if(p32[0]==0xA55A5AA5)
		{
			return;//ԭ����û����
		}
	}

	FileParaAddr=ADDR_FileDown;
	p8=(u8 *)(ADDR_DATABUFF);
	for(i=0;i<8;i++)
	{
		MEMRead(ADDR_DATABUFF,FileParaAddr,32);
		if((p8[2]==0x0)||(p8[2]==0x2))
		{//���������
			if(p8[0]==0x0)
			{//RS232
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
				if(UARTCtrl->Task==0x0)
				{//0=����
					p16timer=(u16 *)(Get_ADDR_UARTnTimer(RS232PORT));
					if(p16timer[0]==0x0)
					{//��ʱ��ʱ=0
						if(Terminal_Ram->RequestFileTimer==0x0)//�����ļ�����MS��ʱ��
						{
							Terminal_Ram->RequestFileTimer=1000/10;//��û�ش�1000MS���ط�
							Request_File_Tx(RS232PORT);//���������ļ�����֡
						}
					}
				}
			}
			else
			{//���߻���̫��
				PORTn=ENETPORT;
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
				if(UARTCtrl->LinkTask!=101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					PORTn=GPRSPORT;
					UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
					if(UARTCtrl->LinkTask!=101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					{
						return;//û��¼
					}
				}
				if(PORTn==GPRSPORT)
				{
					if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
					{
						return;
					}
				}
				//��¼
					if(UARTCtrl->Task==0x0)
					{//0=����
						p16timer=(u16 *)(Get_ADDR_UARTnTimer(PORTn));
						if(p16timer[0]==0x0)
						{//��ʱ��ʱ=0
							if(Terminal_Ram->RequestFileTimer==0x0)//�����ļ�����MS��ʱ��
							{
								if(PORTn==ENETPORT)//ģ������
								{//��̫��
									Terminal_Ram->RequestFileTimer=5000/10;//��û�ش�5S���ط�
								}
								else
								{//����ģ��
									Terminal_Ram->RequestFileTimer=5000/10;//��û�ش�10S���ط�
								}
								Request_File_Tx(PORTn);//���������ļ�����֡
							}
						}
					}
				
			}
		}
		FileParaAddr+=32;
	}
*/
}
















