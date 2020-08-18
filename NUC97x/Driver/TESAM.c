
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/Device/IC_TESAM.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SPI.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../../STM32F4xx/Device/MEMRW.h"


#if IC_TESAM==0//终端用ESAM,0==没,1==有
void TESAM_RXTX(void)//TESAM接收发送
{
}
u32 TESAM_CMD_TxRx(u32 ClaInsP1P2,u32 Lc,u32 ADDR_DATA)//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
{
	return 0;
}
#else//#if IC_TESAM==0//终端用ESAM,0==没,1==有

void TESAM_RXTX(void)//TESAM接收发送(标准SPI0口)
{
  u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	u16 * p16timer;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	u32 RxByte;
	
	p16timer=(u16 *)(ADDR_UART7Timer);
	switch(UART7Ctrl->Task)
	{
		case 0://上电或复位(关电源)
			//SPI引脚为输入下拉
			UnInit_SPI1();//缷载SPI1初始化(TESAM),SPI引为输入下拉
			//关电源
			Pin_Configure(PIN_ENTESAM,//引脚配置
									0|//B3-B0:复用功能选择AF(0-15)
						 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
						 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
						(1<<11));//B11  :当为输出时：0=输出0,1=输出1
			p16timer[0]=50/10;//延时50ms
			while(p16timer[0]);
			UART7Ctrl->Task++;
			//break;
		case 1://开电源
			//ESAM芯片上电复位期间，SSN引脚保持高电平， MISO引脚保持输入状态，MOSI引脚输出低电平
			Pin_Configure(PIN_ENTESAM,//引脚配置
									0|//B3-B0:复用功能选择AF(0-15)
						 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
						 (1<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
						 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
						(0<<11));//B11  :当为输出时：0=输出0,1=输出1
			Pin_Configure(PIN_SPI0_SS1,//输入上拉
									0|//B3-B0:复用功能选择AF(0-15)
						 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
						 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
						 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
						 (1<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
						(0<<11));//B11  :当为输出时：0=输出0,1=输出1
			p16timer[0]=50/10;//延时50ms
			while(p16timer[0]);
			UART7Ctrl->Task++;
			//break;
		case 2://开电源延时
			UART7Ctrl->Task=100;
			break;
			
    case 10://启动发送(先发7字节 0x55 CLA INS P1 P2 Len1 Len2)
			Init_SPI1();//SPI1初始化(TESAM)
			SoftDelayUS(10);//在SSN置高后，T-ESAM将进入低功耗状态，高电平保持时间至少10us以上
			//CS=0
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=2;
			SoftDelayUS(50);//接口设备将SSN再次置低后，需等待50us以上时间才能开始发送数据
			//先发7字节(先发送 MSB)
			p32TxData=(u32*)(REG_SPI0_TX0);
			p32ctrl=(u32*)(REG_SPI0_CNTRL);
		  p8=(u8 *)(ADDR_UART7Tx);
			for(i=0;i<7;i++)
			{
				p32TxData[0]=p8[i];
				//p32ctrl[0]|=1;
				p32ctrl[0]=0x80000000|(8<<3)|(1<<1)|1;
				while(p32ctrl[0]&1);
			#if MainProtocol==698
				//SoftDelayUS(1);//发送数据字节之间延时需大于15us
			#else
				//SoftDelayUS(20);//发送数据字节之间延时需大于15us
			#endif
			}
			x=(p8[5]<<8)+p8[6];//Lc
			//再发后续数据和LRC1
			x+=1;
			SoftDelayUS(100);//接口设备发送完数据Len2后，需要加100μs时间间隔，再发送DATA
		  p8=(u8 *)(ADDR_UART7Tx+7);
			for(i=0;i<x;i++)
			{
				p32TxData[0]=p8[i];
				//p32ctrl[0]|=1;
				p32ctrl[0]=0x80000000|(8<<3)|(1<<1)|1;
				while(p32ctrl[0]&1);
			#if MainProtocol==698
				//SoftDelayUS(1);//发送数据字节之间延时需大于15us
			#else
				//SoftDelayUS(20);//发送数据字节之间延时需大于15us
			#endif
			}
			//CS=1
			SoftDelayUS(5);
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=0;
			UART7Ctrl->Task++;
			//break;
		case 11://启动接收
			//Init_SPI1();//SPI1初始化(TESAM)
			SoftDelayUS(10);//在SSN置高后，T-ESAM将进入低功耗状态，高电平保持时间至少10us以上
			//CS=0
			p32ctrl=(u32*)(REG_SPI0_SSR);
			p32ctrl[0]=2;
			SoftDelayUS(50);//接口设备将SSN再次置低后，需等待50us以上时间才能开始发送数据
			MC(0,ADDR_UART7Rx,LEN_UART7Rx);//清接收缓冲
			p16timer[0]=3000/10;//等待第1个0x55数据返回接收定时
			UART7Ctrl->Task++;
			//break;
		case 12://正在接收
			RxByte=0;
			p32TxData=(u32*)(REG_SPI0_TX0);
			p32TxData[0]=0;
			p32RxData=(u32*)(REG_SPI0_RX0);
			p32ctrl=(u32*)(REG_SPI0_CNTRL);
			p8=(u8 *)(ADDR_UART7Rx);
			while(1)
			{
				if(RxByte==0)
				{
					if(p16timer[0]==0)
					{
						//CS=1
						p32ctrl=(u32*)(REG_SPI0_SSR);
						p32ctrl[0]=0;
						UART7Ctrl->Task=101;//101=接收错误
						return;
					}
				}
				//p32ctrl[0]|=1;
				p32ctrl[0]=0x80000000|(8<<3)|(1<<1)|1;
				while(p32ctrl[0]&1);
				if(RxByte>=LEN_UART7Rx)
				{
					UART7Ctrl->RxByte=RxByte;//接收字节数
					SoftDelayUS(5);//每次 SPI 通讯结束后，需等待一段时间才可以将 SSN 拉高，该等待时间 Tcsd>3us 
					//CS=1
					p32ctrl=(u32*)(REG_SPI0_SSR);
					p32ctrl[0]=0;
					UART7Ctrl->Task++;//接收结束
					return;
				}
				i=p32RxData[0];
				p8[RxByte]=i;
				if(RxByte==0)
				{
					if((i&0xff)==0x55)
					{
						RxByte=1;//接收字节数
					#if MainProtocol==698
						//SoftDelayUS(1);//发送数据字节之间延时需大于15us
					#else
						//SoftDelayUS(20);//发送数据字节之间延时需大于15us
					#endif
					}
					else
					{
						SoftDelayUS(20);//查询0x55,该间隔时Tq>15us
					}
				}
				else
				{
					RxByte++;
					if(RxByte>=6)
					{
						i=(p8[3]<<8)+p8[4];
						i+=6;
						if(RxByte>=i)
						{
							UART7Ctrl->RxByte=RxByte;//接收字节数
							SoftDelayUS(5);//每次 SPI 通讯结束后，需等待一段时间才可以将 SSN 拉高，该等待时间 Tcsd>3us 
							//CS=1
							p32ctrl=(u32*)(REG_SPI0_SSR);
							p32ctrl[0]=0;
							UART7Ctrl->Task++;//接收结束
							return;
						}
					}
				#if MainProtocol==698
					//SoftDelayUS(1);//发送数据字节之间延时需大于15us
				#else
					//SoftDelayUS(20);//发送数据字节之间延时需大于15us
				#endif
				}
			}
    case 13://接收结束
			//LRC2的计算方法：对SW1 SW2 Len1 Len2 DATA数据，每个字节的异或值，再取反。
			p8=(u8 *)(ADDR_UART7Rx+1);
			x=(p8[2]<<8)+p8[3];
			x+=3;
			y=p8[0];
			for(i=0;i<x;i++)
			{
				y^=p8[1+i];
			}
			y=(~y);
			y&=0xff;
			if(y==p8[1+x])
			{
				UART7Ctrl->Task=100;//100=接收完成
			}
			else
			{
				UART7Ctrl->Task=101;//101=接收错误
			}
      break;
     
		case 100://接收完成
      break;
		case 101://接收错误
			break;
		default:
      UART7Ctrl->Task=0;//上电或复位(关电源)
			break;
	}
}

u32 TESAM_CMD_TxRx(u32 ClaInsP1P2,u32 Lc,u32 ADDR_DATA)//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
{
//ClaInsP1P2是APDU命令4字节首标分别为Cla(指令类别),Ins(指令类别中的指令代码),P1(参数1),P2(参数2)
//Lc(P3)是APDU命令的数据字段中(发送)呈现的字节数
	u32 i;
	u32 x;
	u8 * p8;
//  u32 *p32;
  
	if(UART7Ctrl->Lock==0x55)
	{
    if(UART7Ctrl->Task==100)
    {
      UART7Ctrl->Task=101;
    }
		return 1;//0x55=通信功能正在被调用禁止其他程序再次调用
	}
	if(Lc>(LEN_UART7Tx-8))
	{//发送缓冲长度不够
		UART7Ctrl->Task=101;
		return 1;
	}  
  UART7Ctrl->Lock=0x55;
  while(UART7Ctrl->Task<100)
  {
    TESAM_RXTX();//TESAM接收发送
//    p32=(u32 *)ADDR_MS_ENTRY;
//    (*(void(*)())p32[115])();//115 主线程
  }
  p8=(u8 *)(ADDR_UART7Tx);
	p8[0]=0x55;
	p8[1]=ClaInsP1P2>>24;
	p8[2]=ClaInsP1P2>>16;
	p8[3]=ClaInsP1P2>>8;
	p8[4]=ClaInsP1P2>>0;
	p8[5]=Lc>>8;
	p8[6]=Lc;
	if(ADDR_DATA!=(ADDR_UART7Tx+7))
	{
		MR(ADDR_UART7Tx+7,ADDR_DATA,Lc);
	}
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
//    p32=(u32 *)ADDR_MS_ENTRY;
//    (*(void(*)())p32[115])();//115 主线程
  }
  UART7Ctrl->Lock=0;
	
	p8=(u8 *)(ADDR_UART7Rx);
	i=(p8[1]<<8)+p8[2];
	if((UART7Ctrl->Task!=100)||(i!=0x9000))
	{
		return 1;
	}
	return 0;
}

#endif//#if IC_TESAM==0//终端用ESAM,0==没,1==有



