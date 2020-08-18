
//���˵�
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../STM32F4xx/Menu/Menu.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/Display/Draw.h"
#include "../../STM32F4xx/OS/OS.h"
#include "../../STM32F4xx/MS/MS.h"
#include "../../STM32F4xx/Calculate/ascii.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_ADC.h"

#include "../../NUC97x/TEST/TEST.h"
//#include "../Test/Test_RAM.h"		
#include "../../NUC97x/TEST/Test_FLASH_Block_Err.h"
#include "../../NUC97x/TEST/Test_FLASH_Life.h"



void _0(void)
{
}
const u8 pE2_Main3[]=//�������˵�
{
	"�������˵�\n\r"
	"1.�������ܲ���\n\r"
	"2.FLASH�������\n\r"
	"3.���������͵�ز���\n\r"
	"4.���������͵�ز���\n\r"
	"5.ɾ�����Բ˵�\n\r"
	
};





void Test_Main(void)//���˵�
{
	int yaffs;
	struct yaffs_stat stat;
	char pFileName[256];
	char* p8;
	u32 LEN_File;
	u32 *p32s;
	os_Type *os;	
	ms_Type *ms;

	if(Comm_Ram->Factory!=0x55)//0x55=����״̬
	{
	DelTestMenu://�ָ������˵�
		os=Get_os();
		p8=pFileName;
		p8=StrCopy(p8,"user/Program Files/");
		p8=StrCopy(p8,FileFactory_Menu);
		p8[0]=0;
		yaffs=os->yaffs_open(pFileName,O_RDWR, 0);
		if(yaffs>=0)
		{
			os->yaffs_fstat(yaffs, &stat);
			LEN_File=stat.st_size;
			ms=Get_ms();
			p32s=ms->ms_malloc((size_t)LEN_File);//��̬�����ҵ��ڴ��
			os->yaffs_READ(yaffs,(void *)p32s,LEN_File);
			os->yaffs_close(yaffs);
			if(LEN_File)
			{
				p8=pFileName;
				p8=StrCopy(p8,"user/Update/");
				p8=StrCopy(p8,FileFactory_Menu);
				p8[0]=0;
				yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
				if(yaffs>=0)
				{
					os->yaffs_WRITE(yaffs,p32s,LEN_File);
					os->yaffs_close(yaffs);
					//�ٴ򿪶�һ��
					yaffs=os->yaffs_open(pFileName,O_RDWR,0);
					os->yaffs_close(yaffs);
					FileDownload->New=0;//�����ļ������־:0=��,1=û
				}
			}
			ms->ms_free(p32s);//�ͷ��ҵ��ڴ�ѷ���
		}
		MenuDel();//�����ϼ��˵�
		return;
	}
	DOTMIXPMODE->Mix=2;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
	Draw_Clr();
	
	STM32F4xx_Temperature();//STM32F4xx�����ı����¶�
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main3);//��ʾ�˵�
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			
//---TEST---	
//Test_YAFFS_LOOP();//ѭ������YAFFS�ļ���д�ɿ���
//Test_FlushCache();//Test_FlushCache

//---TEST END---
		
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://���ܲ���
					#ifndef LINUX
						Test_ReStartOS();//�������ܲ�������OSǰ
					#else
						Test();//�������ܲ���
					#endif
						break;
					case 2://FLASH�������
						Test_FLASH_Block_Err();
						break;
					case 3://����FLASH��������
						MWR_RTC(0,(u32*)&RTC97x->BKP3R);//RTC->BKP3R=0;
						Test_FLASH_Life();
						break;
					case 4://����FLASH��������
						Test_FLASH_Life();
						break;
					case 5://ɾ�����Բ˵�
						goto DelTestMenu;		
					default:
						break;
				}        
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}









