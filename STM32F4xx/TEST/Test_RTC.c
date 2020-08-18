

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_TIM.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../Device/RTC.h"
#include "../Calculate/Calculate.h"
#include "../Device/IO_I2C.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"


void DS3231_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231写
void DS3231_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231读
void RX8025T_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//RX8025T写
void RX8025T_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//RX8025T读
void Test_WriteRTC(u32 ADDR_BUFF)//写外部硬实时钟
{
#if IC_DS3231==1
  DS3231_Write(ADDR_BUFF,0,7);//DS3231写
#endif
#if IC_RX8025T==1//0=没,1=有
	u8 * p8;
	p8=(u8 *)(ADDR_BUFF);
	p8[3]=1<<p8[3];
	RX8025T_Write(ADDR_BUFF,0,7);//RX8025T写
#endif
}
void Test_ReadRTC(u32 ADDR_BUFF)//读外部硬实时钟到指定缓冲地址(约668us,cclk=96MHz)
{
#if IC_DS3231==1
	 u8 *p8s;
	DS3231_Read(ADDR_BUFF,0,7);//DS3231读
	p8s=(u8 *)(ADDR_BUFF);
	p8s[3]&=0x7;
	if(p8s[3]==7)
	{
		p8s[3]=0;
	}
	p8s[5]&=0x1f;
#endif
#if IC_RX8025T==1//0=没,1=有
	u32 i;
	u8 *p8s;
	RX8025T_Read(ADDR_BUFF,0,7);//RX8025T读
	p8s=(u8 *)(ADDR_BUFF);
	i=p8s[3];
	i=BitLoca(i);//计算第1个置位为1的位置(0-32)
	if(i)
	{
		i--;
	}
	p8s[3]=i;
#endif
}

void Test_DS3231_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte,u32 ms)//DS3231读
{
	u32 ReNum;
	u32 i;
	u8 * p8;
	p8=(u8 *)(ADDR_RAM);
	for(ReNum=0;ReNum<2;ReNum++)
	{
		//InitI2c2();//I2C2初始化
		if(IOI2C_Start(PIN_SCL2,PIN_SDA2)==0x0)//IO仿I2C启动,返回0=正确,1=错误
		{
			if(IOI2C_Write(PIN_SCL2,PIN_SDA2,0xD0)==0x0)//IO仿I2C字节写,返回0=正确,1=错误
			{
				if(IOI2C_Write(PIN_SCL2,PIN_SDA2,ADDR_Drive)==0x0)//IO仿I2C字节写,返回0=正确,1=错误
				{
					if(IOI2C_ReStart(PIN_SCL2,PIN_SDA2)==0x0)//IO仿I2C重启动,返回0=正确,1=错误
					{
						if(IOI2C_Write(PIN_SCL2,PIN_SDA2,0xD1)==0x0)//IO仿I2C字节写,返回0=正确,1=错误
						{
							SoftDelayMS(ms);//软件ms延时
							for(i=0;i<(Byte-1);i++)
							{
								p8[i]=IOI2C_ReadAck(PIN_SCL2,PIN_SDA2);//IO仿I2C字节读ACK
							}
							p8[i]=IOI2C_ReadNoAck(PIN_SCL2,PIN_SDA2);//IO仿I2C字节读NOACK
              Comm_Ram->ErrorIC&=~(1<<4);//硬件错误标志(置位):B4=RTC
							IOI2C_Stop(PIN_SCL2,PIN_SDA2);//IO仿I2C停止
							return;
						}
					}
				}
			}
		}
		IOI2C_Stop(PIN_SCL2,PIN_SDA2);//IO仿I2C停止
    Comm_Ram->ErrorIC|=(1<<4);//硬件错误标志(置位):B4=RTC
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
	}
}
void Test_RX8025T_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte,u32 ms)//RX8025T读
{
	u32 ReNum;
	u32 i;
	u8 * p8;
	p8=(u8 *)(ADDR_RAM);
	for(ReNum=0;ReNum<2;ReNum++)
	{
		//InitI2c2();//I2C2初始化
		if(IOI2C_Start(PIN_SCL2,PIN_SDA2)==0x0)//IO仿I2C启动,返回0=正确,1=错误
		{
			if(IOI2C_Write(PIN_SCL2,PIN_SDA2,0x64)==0x0)//IO仿I2C字节写,返回0=正确,1=错误
			{
				if(IOI2C_Write(PIN_SCL2,PIN_SDA2,ADDR_Drive)==0x0)//IO仿I2C字节写,返回0=正确,1=错误
				{
					if(IOI2C_ReStart(PIN_SCL2,PIN_SDA2)==0x0)//IO仿I2C重启动,返回0=正确,1=错误
					{
						if(IOI2C_Write(PIN_SCL2,PIN_SDA2,0x65)==0x0)//IO仿I2C字节写,返回0=正确,1=错误
						{
							SoftDelayMS(ms);//软件ms延时
							for(i=0;i<(Byte-1);i++)
							{
								p8[i]=IOI2C_ReadAck(PIN_SCL2,PIN_SDA2);//IO仿I2C字节读ACK
							}
							p8[i]=IOI2C_ReadNoAck(PIN_SCL2,PIN_SDA2);//IO仿I2C字节读NOACK
              Comm_Ram->ErrorIC&=~(1<<4);//硬件错误标志(置位):B4=RTC
							IOI2C_Stop(PIN_SCL2,PIN_SDA2);//IO仿I2C停止
							return;
						}
					}
				}
			}
		}
		IOI2C_Stop(PIN_SCL2,PIN_SDA2);//IO仿I2C停止
    Comm_Ram->ErrorIC|=(1<<4);//硬件错误标志(置位):B4=RTC
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
	}
}





u32 Test_RTC(u32 ADDR_BASE)
{
	u32 i;
	u32 ReNum;
	u8 *p8;
	u32 RWErr;//读写:0=正确,1=错误
	u32 UpErr;//进位更新:0=正确,1=错误
	u32 tRST;//I2C总线复位ms
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	__enable_irq();//允许中断
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
#if IC_DS3231==1//0=没,1=有
	DisplayStringAutoUp(0,9,(u8*)"\x0""DS3231");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
#if IC_RX8025T==1//0=没,1=有
	DisplayStringAutoUp(0,9,(u8*)"\x0""RX8025T");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
//写读比较
	ReNum=2;
	while(ReNum--)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[0]=0x59;
		p8[1]=0x59;
		p8[2]=0x23;
		p8[3]=6;
		p8[4]=0x31;
		p8[5]=0x12;
		p8[6]=0x99;
		Test_WriteRTC(ADDR_DATABUFF);//写外部硬实时钟
		Test_ReadRTC(ADDR_DATABUFF+7);//读外部硬实时钟到指定缓冲地址
		for(i=0;i<7;i++)
		{
			if(p8[i]!=p8[7+i])
			{
				if(i==3)
				{//周
					if(p8[7+i]==6)
					{
						continue;
					}
				}
				RWErr=1;//读写:0=正确,1=错误
				TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
				break;
			}
		}
		if(i>=7)
		{
			RWErr=0;//读写:0=正确,1=错误
			break;
		}
	}
//进位更新
	Comm_Ram->GPMSTimer=1500/10;//18 通用(兼虚拟UART11) 每10MS减1定时器
	UpErr=1;//进位更新:0=正确,1=错误
	while(Comm_Ram->GPMSTimer)
	{
		Test_ReadRTC(ADDR_DATABUFF+1);//读外部硬实时钟到指定缓冲地址
		p8=(u8*)ADDR_DATABUFF+1;
		if(p8[6]==0)
		{
			if(p8[5]==1)
			{
				if(p8[4]==1)
				{
					if(p8[3]==0)
					{
						if(p8[2]==0)
						{
							if(p8[1]==0)
							{
								if((p8[0]==0)||(p8[0]==1))
								{
									UpErr=0;//更新:0=正确,1=错误
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	if(UpErr)//进位更新:0=正确,1=错误
	{
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
	}
//I2C总线复位ms
	tRST=950;//I2C总线复位ms
	p8=(u8*)ADDR_DATABUFF+8;
	MC(0xff,ADDR_DATABUFF+8,7);
#if IC_DS3231==1//0=没,1=有
	Test_DS3231_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231读
#endif
#if IC_RX8025T==1//0=没,1=有
	Test_RX8025T_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231读
#endif
	i=1;//错误
	if(((p8[6]<<16)+(p8[5]<<8)+p8[4])==0x000101)
	{
		i=0;//正确
	}
	if(i==0)
	{//950ms时还正确
		for(i=0;i<6;i++)
		{
			tRST+=10;//I2C总线复位ms
			MC(0xff,ADDR_DATABUFF+8,7);
		#if IC_DS3231==1//0=没,1=有
			Test_DS3231_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231读
		#endif
		#if IC_RX8025T==1//0=没,1=有
			Test_RX8025T_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231读
		#endif
			if(((p8[6]<<16)+(p8[5]<<8)+p8[4])!=0x000101)
			{
				break;
			}
		}
	}
	else
	{//950ms时错误
		for(i=0;i<6;i++)
		{
			tRST-=10;//I2C总线复位ms
			MC(0xff,ADDR_DATABUFF+8,7);
		#if IC_DS3231==1//0=没,1=有
			Test_DS3231_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231读
		#endif
		#if IC_RX8025T==1//0=没,1=有
			Test_RX8025T_Read(ADDR_DATABUFF+8,0,7,tRST);//DS3231读
		#endif
			if(((p8[6]<<16)+(p8[5]<<8)+p8[4])==0x000101)
			{
				break;
			}
		}
	}
	if((tRST<900)||(tRST>1000))
	{
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
	}
	
//存储参数
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=RWErr;
	p8[1]=UpErr;
	p8[2]=tRST;
	p8[3]=tRST>>8;
	p8[4]=0;//1Hz输出(0=OK,1=ERROR)
	p8[5]=0;//32kHz输出(0=OK,1=ERROR)
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_RTC_Characteristics),6);
	
	if(TestResult==0)//总测试结果0=合格,1=不合格,0xff=无结论
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	}
	
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=0x00;
	p8[1]=0x00;
	p8[2]=0x12;
	p8[3]=2;
	p8[4]=0x01;
	p8[5]=0x10;
	p8[6]=0x19;
	Test_WriteRTC(ADDR_DATABUFF);//写外部硬实时钟
	return TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
}







