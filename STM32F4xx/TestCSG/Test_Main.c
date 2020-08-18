
//主菜单
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
const u8 pE2_Main3[]=//测试主菜单
{
	"测试主菜单\n\r"
	"1.电气性能测试\n\r"
	"2.FLASH坏块测试\n\r"
	"3.继续寿命和电池测试\n\r"
	"4.重启寿命和电池测试\n\r"
	"5.删除测试菜单\n\r"
	
};



#include "../Test/Test_RAM.h"		


void Test_Main(void)//主菜单
{
	fsStatus fs;
	fsFileInfo info;
	FILE *file;
	u32 LEN_File;
	u32 *p32s;
	os_Type *os;	
	ms_Type *ms;
	
	
	if(Comm_Ram->Factory!=0x55)//0x55=工厂状态
	{
	DelTestMenu://恢复正常菜单
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
				p32s=ms->ms_malloc((size_t)LENmax_Menu);//动态分配我的内存堆
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
						FileDownload->New=0;//有新文件载入标志:0=有,1=没
					}
				}
				ms->ms_free(p32s);//释放我的内存堆分配
			}
		}
		MenuDel();//返回上级菜单
		return;
	}
	DOTMIXPMODE->Mix=2;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
	Draw_Clr();
	
	STM32F4xx_Temperature();//STM32F4xx测量的表内温度
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{
		case 0://任务0:初始化
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pE2_Main3);//显示菜单
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//反显菜单行
			break;
		case 1://任务1:选择
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://性能测试
						Test_ReStartOS();//电气性能测试重启OS前
						break;
					case 2://FLASH坏块测试
						Test_FLASH_Block_Err();
						break;
					case 3://继续FLASH寿命测试
						Test_FLASH_Life();
						break;
					case 4://重启FLASH寿命测试
						RTC->BKP3R=0;
						Test_FLASH_Life();
						break;
					case 5://删除测试菜单
						goto DelTestMenu;		
					default:
						break;
				}        
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x33;//键使用后清0
			break;
		default:
			break;
	}
}









