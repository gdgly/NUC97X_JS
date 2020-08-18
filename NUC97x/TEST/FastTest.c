
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/TEST/FastTest.h"
#include "../../NUC97x/TEST/Test_ALIGN.h"

#include "../../NUC97x/Driver/NUC97x_SPIFLASH.h"
#include "../../STM32F4xx/Device/NAND.h"
#include "../../STM32F4xx/Device/IC_TESAM.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_CRC.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_RCC.h"
#include "../../STM32F4xx/Display/Display.h"
#include "../../STM32F4xx/Device/MEMRW.h"






void FastTest(void)//外围器件快速测试
{
	u32 i;
	u64 d64;
	
//DATA ALIGN
	i=Test_ALIGN();//数据对齐测试;返回:0=正确,1=错误
	if(i)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
		DisplayStringAutoUp(0,9,(u8*)"\x0""DATA ALIGN  ERROR!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	}
//CPU CLK
	i=GetClockMHz(SYS_SYSTEM);//NUC97x得到时钟频率MHz
	if(i!=300)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
		DisplayStringAutoUp(0,9,(u8*)"\x0""300MHz  ERROR!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol

	}
//SPI FLASH
	i=ID_SPIFLASH();//读SPI FLASH器件ID
	if(i!=0xEF15)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
		DisplayStringAutoUp(0,9,(u8*)"\x0""W25Q32JV  ERROR!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	}
//NAND FLASH
	d64=ID_NAND();//读器件ID
#ifdef  NAND_1G
	if(d64!=0xEFF1809500)
#endif
#ifdef  NAND_2G	
	if(d64!=0xEFDA909504)
#endif
	{
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
		DisplayStringAutoUp(0,9,(u8*)"\x0""W29N02GV  ERROR1");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	}
//RTC
	if(Comm_Ram->ErrorIC&(1<<4))//硬件错误标志(置位):B4=RTC
	{
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
		DisplayStringAutoUp(0,9,(u8*)"\x0""RX8025T  ERROR!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	}
//
	
}


u32 TESAM_CMD_TxRx_FastTest(u32 ClaInsP1P2,u32 Lc,u32 ADDR_DATA)//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
{
//ClaInsP1P2是APDU命令4字节首标分别为Cla(指令类别),Ins(指令类别中的指令代码),P1(参数1),P2(参数2)
//Lc(P3)是APDU命令的数据字段中(发送)呈现的字节数
	u32 i;
	u32 x;
	u8 * p8;
  
  while(UART7Ctrl->Task<100)
  {
    TESAM_RXTX();//TESAM接收发送
  }
  p8=(u8 *)(ADDR_UART7Tx);
	p8[0]=0x55;
	p8[1]=ClaInsP1P2>>24;
	p8[2]=ClaInsP1P2>>16;
	p8[3]=ClaInsP1P2>>8;
	p8[4]=ClaInsP1P2>>0;
	p8[5]=Lc>>8;
	p8[6]=Lc;
	MR(ADDR_UART7Tx+7,ADDR_DATA,Lc);
	//LRC1的计算方法：对CLA INS P1 P2 Len1 Len2 DATA数据，每个字节的异或值，再取反
	x=p8[1];
	for(i=0;i<(5+Lc);i++)
	{
		x^=p8[2+i];
	}
	x=(~x);
	p8[7+Lc]=x;
	
	UART7Ctrl->Task=10;//等待发送后接收
  while(UART7Ctrl->Task<100)
  {
    TESAM_RXTX();//TESAM接收发送
  }
	
	p8=(u8 *)(ADDR_UART7Rx);
	i=(p8[1]<<8)+p8[2];
	if((UART7Ctrl->Task!=100)||(i!=0x9000))
	{
		return 1;
	}
	return 0;
}

void FastTest_AfterStartTimer(void)//外围器件快速测试在启动1ms定时中断后
{
	u32 i;
	u32 MAC;
	u64 d64;
	u8* p8;
	
	i=i;
//以太网MAC地址计算
//0xDE684896 C7641C33
//0xDE684896 C7516F21
//0xDE6874a3 47826733
	
	d64=Unique_ID_SPIFLASH();//读SPI FLASH器件Unique ID
	MWR(d64,ADDR_ETHMAC,8);
	MAC=RAM_CRC32(ADDR_ETHMAC,8);//RAM中数据CRC32计算
	p8=(u8*)(ADDR_ETHMAC);
	//MAC0
	p8[0]=0xac;
	p8[1]=0xe0;
	p8[2]=MAC>>24;
	p8[3]=MAC>>16;
	p8[4]=MAC>>8;
	p8[5]=MAC;
#if MainProtocol==698
	MW(ADDR_ETHMAC,ADDR_4510_5+2,6);//以太网通信模块1;5 MAC地址
#endif
	//MAC1
	p8=(u8*)(ADDR_ETHMAC+6);
	p8[0]=0xac;
	p8[1]=0xe2;
	p8[2]=MAC>>24;
	p8[3]=MAC>>16;
	p8[4]=MAC>>8;
	p8[5]=MAC;

//TESAM
//---TESAM LOOP TEST----
/*
	while(1)
	{
		UART7Ctrl->Task=0;//从0开始,因上电即打开了电源,电源上升速度慢
		while(TESAM_CMD_TxRx_FastTest(0x803600FF,0,0))
		{
			UART7Ctrl->Task=0;//从0开始,因上电即打开了电源,电源上升速度慢
			Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
			DisplayStringAutoUp(0,9,(u8*)"\x0""TESAM  ERROR!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
		}
	}
*/
//---TESAM LOOP TEST END----
	
#if IC_TESAM==1//终端用ESAM,0==没,1==有
	i=2;//如果错误再测1次
	while(i)
	{
		i--;
		UART7Ctrl->Task=0;//从0开始,因上电即打开了电源,电源上升速度慢
	#if MainProtocol!=698
		if(TESAM_CMD_TxRx_FastTest(0x800E0002,0,0))//获取芯片序列号8字节
	#else
		if(TESAM_CMD_TxRx_FastTest(0x803600FF,0,0))//获取终端安全芯片信息
	#endif
		{
			UART7Ctrl->Task=0;//从0开始,因上电即打开了电源,电源上升速度慢
			if(i==0)
			{
				Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
				DisplayStringAutoUp(0,9,(u8*)"\x0""TESAM  ERROR!");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
			}
		}
		else
		{
			break;
		}
	}
#endif

}







