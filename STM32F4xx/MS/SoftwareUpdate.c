
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../MS/SoftwareUpdate.h"
#include "../MS/SoftwareCheck.h"
#include "../Device/NAND.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_FLASH.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../OS/OS.h"
#include "../Calculate/ascii.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/Data_Save.h"


u32 SoftwareUpdateRW(char *FileName,u32 *p32s,u32 *p32d,u32 LEN_Program_Max,u32 Same,u32 SoftwareNo)//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
{
	u32 i;
	u32 LEN_SourFile;
	u32 LEN_File;
	FILE *file;
	u8 *p8;
	os_Type *os;
	fsStatus fs;

	
	WWDT_Enable_Feed(32000);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
	os=Get_os();
	LEN_File=0;
	file=os->fopen(FileName,"r");
	if(file!=NULL)
	{
		LEN_SourFile=os->fread((void *)p32s,1,LEN_Program_Max,file);
		os->fclose(file);
		if((p32s[0]&0xff)==0)
		{//��ѹ��
			MW((u32)p32s,(u32)p32d,LEN_SourFile);
			LEN_File=LEN_SourFile;
		}
		else
		{//Lzmaѹ��
			p8=(u8*)p32s;
			i=p8[5]+(p8[6]<<8)+(p8[7]<<16)+(p8[8]<<24);
			if(i<=LEN_Program_Max)
			{
				LEN_File=os->RAM_LzmaDecode((u32)p32s,LEN_SourFile,(u32)p32d,i);//RAM->RAM��Lzma��ѹ���㷨;����:��ѹ����ĳ���
				if(LEN_File!=i)
				{
					LEN_File=0;
				}
			}
		}
		if(LEN_File>(4*132))
		{
			i=SoftwareCheck(p32d);//������;���:�����ڵ�ַָ��,����:0=����,1=��ȷ
			if(i==0)//������;���:�����ڵ�ַָ��,����:0=����,1=��ȷ
			{
				LEN_File=0;
			}
			else
			{
				if(p32d[128]==SoftwareNo)//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
				{
					if(p32d[125]!=0)//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
					{
						Same&=p32d[125];
						if(Same!=p32d[125])
						{
							return 1;//��ûͬʱ�������,����1��ʾ�б��ļ�
						}
					}
					os->fchdir("N0:/Program Files");
					file=os->fopen(FileName,"w");
					if(file!=NULL)
					{
						os->fwrite((void *)p32s,1,LEN_SourFile,file);
						os->fclose(file);
					}
					
					if(SoftwareNo<=1)//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
					{//��MS,OS����
						//MW((u32)p32d,ADDR_Download_ENTRY,LEN_File);//�ڱ���������
						RePlace_Write_NAND((u32)p32d,ADDR_Download_ENTRY-ADDR_EFLASH_Start,LEN_File);//NAND_FLASHд;����������滻
						//MS,OS����������������,�ȱ�����ļ���־
						Same|=(1<<SoftwareNo);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
						RTC->BKP1R=Same;//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
						//��ɾ���ļ�
						os->fchdir("N0:/Download");
						os->fdelete(FileName,NULL);//ɾ���ļ�
						if(SoftwareNo==0)//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
						{//MS��BOOT����
							Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"MS                OK");
							i=p32d[124];//124//SVER;//����汾��
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=0;//����ѡ��
							p8[21]=0;//������
							p8+=13;
							p8[0]='V';
							p8[1]=(i>>8)+0x30;
							p8[2]='.';
							p8[3]=((i>>4)&0xf)+0x30;
							DisplayStringAutoUp(0,9,(u8*)ADDR_DATABUFF);//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
						}
						else
						{//OS
							// Disable SysTick timer                                               
							SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
							//USB-HS,ETHERNET ��λ
							RCC->AHB1RSTR=(1<<29)+(1<<25);
							//USB-FS ��λ
							RCC->AHB2RSTR=(1<<7);
							//�帴λ
							RCC->AHB1RSTR=0;
							RCC->AHB2RSTR=0;

							__disable_irq();//���ж�
							Write_STM32F4xx_FLASH((u32)p32d,p32d[110],LEN_File);//FLASHд;ʹ��ADDR_DATABUFF,ADDR_4KWRITE_BUFF,0x10089FE0-0x10089FFF=32Byte
							__enable_irq();//�����ж�
							//MC(0,ADDR_Download_ENTRY,4*132);//�屸����
							RePlace_Clr_NAND(0,ADDR_Download_ENTRY-ADDR_EFLASH_Start,4*32);//NAND_FLASH��0;����������滻
							Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"OS                OK");
							i=p32d[124];//124//SVER;//����汾��
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=0;//����ѡ��
							p8[21]=0;//������
							p8+=13;
							p8[0]='V';
							p8[1]=(i>>8)+0x30;
							p8[2]='.';
							p8[3]=((i>>4)&0xf)+0x30;
							p8[4]=(i&0xf)+0x30;
							DisplayStringAutoUp(0,9,(u8*)ADDR_DATABUFF);//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
							SoftDelayMS(1000);//���ms��ʱ
							
							(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
							ReSTART_OS();//����OS
						}
						return 2;//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
					}
					if(SoftwareNo!=7)//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
					{//�ڲ�FLASH
						__disable_irq();//���ж�
						Write_STM32F4xx_FLASH((u32)p32d,p32d[110],LEN_File);//FLASHд;ʹ��ADDR_DATABUFF,ADDR_4KWRITE_BUFF,0x10089FE0-0x10089FFF=32Byte
						__enable_irq();//�����ж�
					}
					else
					{//�ⲿFLASH��SDRAM
						MW((u32)p32d,p32d[110],LEN_File);
					}
					FileDownload->File|=(1<<SoftwareNo);//�ļ���־(��λ��ʾOK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=Font_ttf,B7=Module,...B31=
					Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
					switch(SoftwareNo)
					{
						case 2:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Meter             OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Meter Update      OK");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
							break;
						case 3:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Terminal          OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Terminal Update   OK");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
							break;
						case 4:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Menu              OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Menu Update       OK");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
							break;
						case 5:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Font              OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Font Update       OK");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
							break;
						case 7:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Module            OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Module Update     OK");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
							break;
						case 8:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Test              OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Test Update       OK");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
							break;
					}
					i=p32d[124];//124//SVER;//����汾��
					p8=(u8*)ADDR_DATABUFF;
					p8[0]=0;//����ѡ��
					p8[21]=0;//������
					p8+=13;
					p8[0]='V';
					p8[1]=(i>>8)+0x30;
					p8[2]='.';
					p8[3]=((i>>4)&0xf)+0x30;
					p8[4]=(i&0xf)+0x30;
					DisplayStringAutoUp(0,9,(u8*)ADDR_DATABUFF);//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				}
				else
				{
					LEN_File=0;
				}
			}
		}
	}
	fs=os->fchdir("N0:/Download");
	fs=fs;
	fs=os->fdelete(FileName,NULL);//ɾ���ļ�
	fs=fs;
	
	if(LEN_File==0)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		switch(SoftwareNo)
		{
			case 0:
				DisplayStringAutoUp(0,9,(u8*)"\x0""MS Update       FAIL");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				break;
			case 1:
				DisplayStringAutoUp(0,9,(u8*)"\x0""OS Update       FAIL");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				break;
			case 2:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Meter Update    FAIL");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				break;
			case 3:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Terminal Update FAIL");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				break;
			case 4:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Menu Update     FAIL");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				break;
			case 5:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Font Update     FAIL");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				break;
			case 7:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Module Update   FAIL");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				break;
			case 8:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Test Update     FAIL");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
				break;
		}
	}
	return LEN_File;
}

fsStatus FileFind (const char *FileFactory,const char *FileUser ,fsFileInfo *info)
{
	os_Type *os;
	
	os=Get_os();
	info->fileID = 0; 
	if(os->ffind(FileFactory,info)==fsOK)
	{
		return fsOK;
	}
	info->fileID = 0; 
	if(os->ffind(FileUser,info)==fsOK)
	{
		return fsOK;
	}
	return fsFileNotFound;
}




void SoftwareUpdate(void)//�������
{
	u32 i;
	u32 x;
	u32 y;
	u32 Same;//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
	u32 Reset;//���º�λ��־
	fsFileInfo info;
	FILE *file;
	u32 LEN_SourFile;
	u32 LEN_File;
	u8* p8;
	u32 *p32s;
	u32 *p32d;
	os_Type *os;
	UARTCtrl_TypeDef *UARTCtrl;
	
	os=Get_os();

	i=RTC->BKP0R&0xff;//FileDownload->USB0Task;//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
	//USB0����
	if(((i==2)||(i==3)))//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
	{
		return;//USB0�������ڲ���
	}
	//USB0�豸
	if((i==0)||(i>=6))//����:0=��,1=�ȴ�����,2=���������ʼ���ȴ�,3=��������,4=��������,5=��������,6=�豸����,7=�豸����
	{//6=�豸����
		return;
	}

	if(FileDownload->New==0)//�����ļ������־:0=��,1=û
	{
		i=FileDownload->PORTn;
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(i);
		if(UARTCtrl->Task!=0)
		{//���ǿ���
			return;
		}
		p32s=ms_malloc((size_t)1024*1024);//��̬�����ҵ��ڴ��
		p32d=ms_malloc((size_t)2048*1024);//��̬�����ҵ��ڴ��
		if((p32s==0)||(p32d==0))
		{
			Init_my_heap();//��ʼ�����ں�RTOS�޹ص��ҵ��ڴ��
			p32s=ms_malloc((size_t)1024*1024);//��̬�����ҵ��ڴ��
			p32d=ms_malloc((size_t)2048*1024);//��̬�����ҵ��ڴ��
		}
		
		os->fchdrive ("N0:");
		os->fchdir("N0:/Download");
		
//���д���ļ����Ƚ�ѹ�ļ�
		info.fileID=0;
		i=os->ffind(FileFactory_PACK,&info);
		if(i==fsOK)
		{
			file=os->fopen((char*)&info.name,"r");
			if(file!=NULL)
			{
				LEN_SourFile=os->fread((void *)p32s,1,1024*1024,file);
				LEN_File=0;
				os->fclose(file);
				if(p32s[0]&0xff)
				{//Lzmaѹ��
					p8=(u8*)p32s;
					i=p8[5]+(p8[6]<<8)+(p8[7]<<16)+(p8[8]<<24);
					if(i<=(2*1024*1024))
					{
						LEN_File=os->RAM_LzmaDecode((u32)p32s,LEN_SourFile,(u32)p32d,i);//RAM->RAM��Lzma��ѹ���㷨;����:��ѹ����ĳ���
						if(LEN_File!=i)
						{
							LEN_File=0;
						}
					}
				}
				if(LEN_File>(4*132))
				{
					for(i=0;i<32;i++)
					{
						y=p32d[2+(2*i)];
						x=p32d[3+(2*i)];
						if((x>=0x00200000)||(y>=0x00200000)||(x<y))
						{
							continue;
						}
						x-=y;
						if(x>(4*132))
						{
							y>>=1;
							y<<=1;
							y+=(u32)p32d;
					#if MainProtocol==376
							switch(i)
							{
								case 0:
									file=os->fopen("MS.bin","w");
									break;
								case 1:
									file=os->fopen("OS.bin","w");
									break;
								case 2:
									file=os->fopen("Meter.bin","w");
									break;
								case 3:
									file=os->fopen("Terminal.bin","w");
									break;
								case 4:
									file=os->fopen("Menu.bin","w");
									break;
								case 5:
									file=os->fopen("Font.bin","w");
									break;
								case 6:
									file=os->fopen("Test.bin","w");
									break;
								default:
									file=NULL;
									break;
							}
					#endif
					#if MainProtocol==698
							switch(i)
							{
								case 0:
									file=os->fopen("MS698.bin","w");
									break;
								case 1:
									file=os->fopen("OS698.bin","w");
									break;
								case 2:
									file=os->fopen("Meter698.bin","w");
									break;
								case 3:
									file=os->fopen("Terminal698.bin","w");
									break;
								case 4:
									file=os->fopen("Menu698.bin","w");
									break;
								case 5:
									file=os->fopen("Font.bin","w");
									break;
								case 6:
									file=os->fopen("Test698.bin","w");
									break;
								default:
									file=NULL;
									break;
							}
					#endif
					#if MainProtocol==CSG
							switch(i)
							{
								case 0:
									file=os->fopen("MSCSG.bin","w");
									break;
								case 1:
									file=os->fopen("OSCSG.bin","w");
									break;
								case 2:
									file=os->fopen("MeterCSG.bin","w");
									break;
								case 3:
									file=os->fopen("TerminalCSG.bin","w");
									break;
								case 4:
									file=os->fopen("MenuCSG.bin","w");
									break;
								case 5:
									file=os->fopen("Font.bin","w");
									break;
								case 6:
									file=os->fopen("TestCSG.bin","w");
									break;
								default:
									file=NULL;
									break;
							}
					#endif
							if(file!=NULL)
							{
								os->fwrite((void *)y,1,x,file);
								os->fclose(file);
							}
						}
					}
				}
				os->fdelete((char*)&info.name,NULL);//ɾ���ļ�
			}
		}
		
//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
		Same=RTC->BKP1R;//�Ѹ��µ���ͬʱ�����ļ�:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
		Reset=0;//���º�λ��־
//��1��ɨ��MS,OS,Meter,Terminal,Menu,Font
		//Font
		if(FileFind(FileFactory_Font,FileUser_Font,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Font,Same,5);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<5);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
			}
		}
		//Menu
		if(FileFind(FileFactory_Menu,FileUser_Menu,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Menu,Same,4);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<4);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				}
			}
		}
		//Terminal
		if(FileFind(FileFactory_Terminal,FileUser_Terminal,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Terminal,Same,3);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<3);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				#if MainProtocol==698
					(*(void(*)())p32d[117])();//�ն˰汾����¼�
				#endif
				}
			}
		}
		//Meter
		if(FileFind(FileFactory_Meter,FileUser_Meter,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Meter,Same,2);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<2);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				}
			}
		}
		//OS
		if(FileFind(FileFactory_OS,FileUser_OS,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_OS,Same,1);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				//OS����ʱ������OS�����᷵�ص��´���
				Same|=(1<<1);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				}
			}
		}
		//MS
		if(FileFind(FileFactory_MS,FileUser_MS,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_MS,Same,0);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{//MS��BOOT����
				Same|=(1<<0);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					SoftDelayMS(2000);//���ms��ʱ
					p32s=(u32 *)(0x08000000);
					(*(void(*)())p32s[110])((u32)p32d,p32d[111]-p32d[110]);
				}
			}
		}
//��2��ɨ��MS,OS,Meter,Terminal,Menu,Test,Font
		//Font
		if(FileFind(FileFactory_Font,FileUser_Font,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Font,Same,5);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<5);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
			}
		}
		//Menu
		if(FileFind(FileFactory_Menu,FileUser_Menu,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Menu,Same,4);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<4);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				}
			}
		}
		//Test
		if(FileFind(FileFactory_Test,FileFactory_Test,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Menu,Same,8);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<4);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				}
			}
		}
		//Terminal
		if(FileFind(FileFactory_Terminal,FileUser_Terminal,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Terminal,Same,3);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<3);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				#if MainProtocol==698
					(*(void(*)())p32d[117])();//�ն˰汾����¼�
				#endif
				}
			}
		}
		//Meter
		if(FileFind(FileFactory_Meter,FileUser_Meter,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Meter,Same,2);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				Same|=(1<<2);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				}
			}
		}
		//OS
		if(FileFind(FileFactory_OS,FileUser_OS,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_OS,Same,1);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{
				//OS����ʱ������OS�����᷵�ص��´���
				Same|=(1<<1);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					Reset|=p32d[129];//���º�λ��־
					(*(void(*)())p32d[113])();//���ģ����³�ʼ�����
				}
			}
		}
		//MS
		if(FileFind(FileFactory_MS,FileUser_MS,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_MS,Same,0);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			if(i!=0)
			{//MS��BOOT����
				Same|=(1<<0);//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//����ֵ=����
					SoftDelayMS(2000);//���ms��ʱ
					p32s=(u32 *)(0x08000000);
					(*(void(*)())p32s[110])((u32)p32d,p32d[111]-p32d[110]);
				}
			}
		}
		
	//PLC,������MS,OS���飬��MS,OS���»����ݴ浽��ͬ�ı���������(ADDR_Backup_ENTRY),ͬʱ���ϸ��¿������������ж���ģ������ݴ���
	#if (((Project/100)==2)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=,500=����������,600=,700=
		switch(Comm_Ram->RouterType)////·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��,100=����,101=��ϣ,102=��Ѷ��,103=�з�,104=ɣ��
		{
			case 1:
				i=FileFind(FileFactory_PLC_50,FileUser_PLC_50,&info);
				break;
			case 2:
				i=FileFind(FileFactory_PLC_TC,FileUser_PLC_TC,&info);
				break;
			case 3:
				i=FileFind(FileFactory_PLC_ES,FileUser_PLC_ES,&info);
				break;
			case 4:
				i=FileFind(FileFactory_PLC_CX,FileUser_PLC_CX,&info);
				break;
			case 5:
				i=FileFind(FileFactory_PLC_01,FileUser_PLC_01,&info);
				break;
			case 6:
				i=FileFind(FileFactory_PLC_LH,FileUser_PLC_LH,&info);
				break;
			case 7:
				i=FileFind(FileFactory_PLC_NX,FileUser_PLC_NX,&info);
				break;
			case 8:
				i=FileFind(FileFactory_PLC_QL,FileUser_PLC_QL,&info);
				break;
			case 101:
				i=FileFind(FileFactory_PLC_MX,FileUser_PLC_MX,&info);
				break;
			case 102:
				i=FileFind(FileFactory_PLC_FC,FileUser_PLC_FC,&info);
				break;
			case 103:
				i=FileFind(FileFactory_PLC_NW,FileUser_PLC_NW,&info);
				break;
			case 104:
				i=FileFind(FileFactory_PLC_SR,FileUser_PLC_SR,&info);
				break;
			default:
				i=fsFileNotFound;
				break;
		}
		if(i==fsOK)
		{
			SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Download,Same,7);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
		}
		else
		{
			i=FileFind(FileFactory_Modele,FileUser_MODULE,&info);
			if(i==fsOK)
			{
				SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Download,Same,7);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
			}
		}
	#endif
	#if (((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=,500=����������,600=,700=
		i=FileFind(FileFactory_Modele,FileUser_MODULE,&info);
		if(i==fsOK)
		{
			SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Download,Same,7);//������¶�д;����:0=û����,1=�ļ���ȷ����ͬʱ����,����ֵ=����
		}
	#endif
		RTC->BKP1R=0;//ͬʱ���±�־:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
		FileDownload->New=1;//�����ļ������־:0=��,1=û
		//���º�λ
		Reset>>=8;
		Reset&=0xff;
		if(Reset!=0)//���º�λ��־
		{
			SoftDelayMS(2000);//���ms��ʱ
			WWDT_RESET();//����ʹ�ܿ��Ź���λ
		}
		ms_free(p32s);//�ͷ��ҵ��ڴ�ѷ���
		ms_free(p32d);//�ͷ��ҵ��ڴ�ѷ���
		
	}
}

























