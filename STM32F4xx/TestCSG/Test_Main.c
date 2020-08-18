
//���˵�
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Menu/Menu.h"
#include "../Display/Display.h"
#include "../Display/Draw.h"

#include "../TEST/Test.h"
#include "../TEST/Test_FLASH_Block_Err.h"
#include "../TEST/Test_FLASH_Life.h"
#include "../OS/OS.h"
#include "../MS/MS.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"


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



#include "../Test/Test_RAM.h"		


void Test_Main(void)//���˵�
{
	fsStatus fs;
	fsFileInfo info;
	FILE *file;
	u32 LEN_File;
	u32 *p32s;
	os_Type *os;	
	ms_Type *ms;
	
	
	if(Comm_Ram->Factory!=0x55)//0x55=����״̬
	{
	DelTestMenu://�ָ������˵�
		os=Get_os();
		info.fileID = 0; 
		os->fchdir("N0:/Program Files");
		fs=os->ffind("Menu.*",&info);
		if(fs!=fsOK)
		{
			info.fileID = 0; 
			fs=os->ffind("Menu698.*",&info);
		}
		if(fs==fsOK)
		{
			file=os->fopen((char *)&info.name,"r");
			if(file!=NULL)
			{
				ms=Get_ms();
				p32s=ms->ms_malloc((size_t)LENmax_Menu);//��̬�����ҵ��ڴ��
				LEN_File=os->fread((void *)p32s,1,LENmax_Menu,file);
				os->fclose(file);
				if(LEN_File)
				{
					os->fchdir("N0:/Download");
					file=os->fopen((char *)&info.name,"w");
					if(file!=NULL)
					{
						os->fwrite((void *)p32s,1,LEN_File,file);
						os->fclose(file);
						FileDownload->New=0;//�����ļ������־:0=��,1=û
					}
				}
				ms->ms_free(p32s);//�ͷ��ҵ��ڴ�ѷ���
			}
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
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://���ܲ���
						Test_ReStartOS();//�������ܲ�������OSǰ
						break;
					case 2://FLASH�������
						Test_FLASH_Block_Err();
						break;
					case 3://����FLASH��������
						Test_FLASH_Life();
						break;
					case 4://����FLASH��������
						RTC->BKP3R=0;
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









