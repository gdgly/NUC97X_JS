

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Device/IC_TESAM.h"
#include "../STM32F4xx/STM32F4xx_RCC.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"

#include "../Display/Display.h"
#include "../Device/MEMRW.h"





u32 TEST_TESAM_CMD_TxRx(u32 ClaInsP1P2,u32 Lc,u32 ADDR_DATA)//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
{
//ClaInsP1P2是APDU命令4字节首标分别为Cla(指令类别),Ins(指令类别中的指令代码),P1(参数1),P2(参数2)
//Lc(P3)是APDU命令的数据字段中(发送)呈现的字节数
	u32 i;
	u32 x;
	u8 * p8;
  
	UART7Ctrl->Lock=0;
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



u32 Test_ESAM(u32 ADDR_BASE)
{
	u32 i;
	u32 BR;
	u64 d64;
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	__enable_irq();//允许中断
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	
//TESAM
	DisplayStringAutoUp(0,9,(u8*)"\x0""TESAM");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	while(UART7Ctrl->Task<100)
	{
		TESAM_RXTX();//TESAM接收发送(标准SPI1口)
	}
	d64=0;
	BR=4;
	for(i=0;i<5;i++)
	{
		SPI1->CR1&=~(0x7<<3);
		SPI1->CR1|=(BR<<3);
		BR--;
	#if MainProtocol!=698
		TEST_TESAM_CMD_TxRx(0x800E0002,0,0);//获取芯片序列号8字节
	#else
		TEST_TESAM_CMD_TxRx(0x803600FF,0,0);//获取芯片序列号8字节
	#endif
		while(UART7Ctrl->Task<100)
		{
			TESAM_RXTX();//TESAM接收发送(标准SPI1口)
		}
		if(UART7Ctrl->Task==100)
		{
			d64=MRR(ADDR_UART7Rx+5,8);//取序列号
		}
		else
		{
			BR+=2;
			if(BR>7)
			{
				BR=7;
			}
			break;
		}
		if(BR==0)
		{
			break;
		}
	}
	SPI1->CR1&=~(0x7<<3);
	SPI1->CR1|=(3<<3);
	BR=RCC_GetPCLK2Freq()/(2<<BR);
	if(BR>2000000)
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	else
	{
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	//存储参数
	MWR(BR,ADDR_DATABUFF,4);
	MWR(d64,ADDR_DATABUFF+4,8);
	
//MESAM
	
	
	//存储参数
	MWR(0xffffffff,ADDR_DATABUFF+12,4);
	MWR(d64,ADDR_DATABUFF+16,8);
	//写
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_ESAM_Characteristics),24);
	
	return TestResult;
}








