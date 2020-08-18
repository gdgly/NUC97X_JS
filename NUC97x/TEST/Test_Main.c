
//主菜单
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
const u8 pE2_Main3[]=//测试主菜单
{
	"测试主菜单\n\r"
	"1.电气性能测试\n\r"
	"2.FLASH坏块测试\n\r"
	"3.重启寿命和电池测试\n\r"
	"4.继续寿命和电池测试\n\r"
	"5.删除测试菜单\n\r"
	
};





void Test_Main(void)//主菜单
{
	int yaffs;
	struct yaffs_stat stat;
	char pFileName[256];
	char* p8;
	u32 LEN_File;
	u32 *p32s;
	os_Type *os;	
	ms_Type *ms;

	if(Comm_Ram->Factory!=0x55)//0x55=工厂状态
	{
	DelTestMenu://恢复正常菜单
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
			p32s=ms->ms_malloc((size_t)LEN_File);//动态分配我的内存堆
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
					//再打开读一次
					yaffs=os->yaffs_open(pFileName,O_RDWR,0);
					os->yaffs_close(yaffs);
					FileDownload->New=0;//有新文件载入标志:0=有,1=没
				}
			}
			ms->ms_free(p32s);//释放我的内存堆分配
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
			
//---TEST---	
//Test_YAFFS_LOOP();//循环测试YAFFS文件读写可靠性
//Test_FlushCache();//Test_FlushCache

//---TEST END---
		
			MenuSelect();//菜单选择
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1://性能测试
					#ifndef LINUX
						Test_ReStartOS();//电气性能测试重启OS前
					#else
						Test();//电气性能测试
					#endif
						break;
					case 2://FLASH坏块测试
						Test_FLASH_Block_Err();
						break;
					case 3://重启FLASH寿命测试
						MWR_RTC(0,(u32*)&RTC97x->BKP3R);//RTC->BKP3R=0;
						Test_FLASH_Life();
						break;
					case 4://继续FLASH寿命测试
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









