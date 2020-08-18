
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../STM32F4xx/Test/LOG.h"
#include "../../STM32F4xx/OS/OS.h"
#include "../../STM32F4xx/MS/MS.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_WDG.h"



const u8 TEST_LOG_CHAR[]=
{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


void Test_LOG(void)//测试LOG日志
{
	u32 i;
	u32 x;
	u32 LEN_File;
	u16* p16;
	u8* p8;
	os_Type *os;
	ms_Type *ms;
	int yaffs;
	struct yaffs_stat stat;
	u32 Count;
	u32 Count2;
	u32 Count3;
	
	DisplayClr();//清屏
	DisplayStringAutoUp(0,9,(u8*)"\x0""LOG TEST");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	os=Get_os();
	ms=Get_ms();
	
	os->yaffs_unlinkdir("user/TEST/");
	Count=0;
	Count2=0;
	Count3=0;
	while(1)
	{
		WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
		#ifdef PIN_ESC
		if(Pin_Read(PIN_ESC))//读引脚,入口引脚使用名;返回0或1
		{
			os->yaffs_unlinkdir("user/TEST/");
			return;
		}
		#endif
		LOG(698,0,RS232PORT,(u8*)TEST_LOG_CHAR,16,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		Count+=45;
		LOG(13762,0,4,(u8*)TEST_LOG_CHAR,16,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		Count2+=40;
		LOG(13763,0,GPRSPORT,(u8*)TEST_LOG_CHAR,16,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
		Count3+=40;
		//读比较DL698.log
		yaffs=os->yaffs_open("user/TEST/DL698.log",O_RDWR,0);
		if(yaffs<0)
		{
			DisplayStringAutoUp(0,9,(u8*)"\x0""yaffs_open_read");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_fstat(yaffs, &stat);
		if(stat.st_size!=Count)
		{
			os->yaffs_close(yaffs);
			DisplayStringAutoUp(0,9,(u8*)"\x0""stat.st_size!=Count");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		p8=ms->ms_malloc(Count);
		if(p8==0)
		{
			os->yaffs_unlinkdir("user/TEST/");
			return;
		}
		ms->ms_free(p8);//释放我的内存堆分配
		LEN_File=os->yaffs_READ(yaffs,(void *)p8,Count);
		if(LEN_File!=Count)
		{
			os->yaffs_close(yaffs);
			DisplayStringAutoUp(0,9,(u8*)"\x0""LEN_File!=Count");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_close(yaffs);
		for(i=0;i<Count;)
		{
			for(x=0;x<16;x++)
			{
				if(p8[i+27+x]!=TEST_LOG_CHAR[x])
				{
					break;
				}
			}
			if(x<16)
			{
				DisplayStringAutoUp(0,9,(u8*)"\x0""File Data");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				continue;
			}
			i+=45;
		}
//读比较GDW1376-2.log
		yaffs=os->yaffs_open("user/TEST/GDW1376-2.log",O_RDWR,0);
		if(yaffs<0)
		{
			DisplayStringAutoUp(0,9,(u8*)"\x0""yaffs_open_read");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_fstat(yaffs, &stat);
		if(stat.st_size!=Count2)
		{
			os->yaffs_close(yaffs);
			DisplayStringAutoUp(0,9,(u8*)"\x0""stat.st_size!=Count");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		p8=ms->ms_malloc(Count);
		if(p8==0)
		{
			os->yaffs_unlinkdir("user/TEST/");
			return;
		}
		ms->ms_free(p8);//释放我的内存堆分配
		LEN_File=os->yaffs_READ(yaffs,(void *)p8,Count2);
		if(LEN_File!=Count2)
		{
			os->yaffs_close(yaffs);
			DisplayStringAutoUp(0,9,(u8*)"\x0""LEN_File!=Count");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_close(yaffs);
		for(i=0;i<Count2;)
		{
			for(x=0;x<16;x++)
			{
				if(p8[i+22+x]!=TEST_LOG_CHAR[x])
				{
					break;
				}
			}
			if(x<16)
			{
				DisplayStringAutoUp(0,9,(u8*)"\x0""File Data");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				continue;
			}
			i+=40;
		}
//读比较GDW1376-3.log
		yaffs=os->yaffs_open("user/TEST/GDW1376-3.log",O_RDWR,0);
		if(yaffs<0)
		{
			DisplayStringAutoUp(0,9,(u8*)"\x0""yaffs_open_read");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_fstat(yaffs, &stat);
		if(stat.st_size!=Count3)
		{
			os->yaffs_close(yaffs);
			DisplayStringAutoUp(0,9,(u8*)"\x0""stat.st_size!=Count");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		p8=ms->ms_malloc(Count3);
		if(p8==0)
		{
			os->yaffs_unlinkdir("user/TEST/");
			return;
		}
		ms->ms_free(p8);//释放我的内存堆分配
		LEN_File=os->yaffs_READ(yaffs,(void *)p8,Count3);
		if(LEN_File!=Count3)
		{
			os->yaffs_close(yaffs);
			DisplayStringAutoUp(0,9,(u8*)"\x0""LEN_File!=Count");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			continue;
		}
		os->yaffs_close(yaffs);
		for(i=0;i<Count3;)
		{
			for(x=0;x<16;x++)
			{
				if(p8[i+22+x]!=TEST_LOG_CHAR[x])
				{
					break;
				}
			}
			if(x<16)
			{
				DisplayStringAutoUp(0,9,(u8*)"\x0""File Data");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				continue;
			}
			i+=40;
		}
		
//OK
		p16=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row)*84)+4);
		for(i=0;i<8;i++)
		{
			p16[i]=0;
		}
		i=hex_bcd(Count);
		DisplayData_OffZero(i,8,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
		LCDDataOut();//点阵数据输出到LCD屏
	}
}



















