
//LCD
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Display/LCDModule.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Display/Display.h"
#include "../Display/DisplayFlash.h"
#include "../Display/LCDdot.h"
#include "../Device/MEMRW.h"
#include "../Test/Test_RAM.h"
#include "../emWin/emWinApp.h"





void Init_ST75161(void);
void Init_LCDModule(void)//点阵LCD显示外部总线控制EMC初始化
{
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
	Pin_Configure(PIN_LCD_RST,//引脚配置
									  0|//B3-B0:复用功能选择AF(0-15)
							 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
							 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
							 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
							 (0<<9)|//B10-B9:上拉/下拉:00：无上拉或下拉,01：上拉,10：下拉,11：保留
							(0<<11));//B11  :当为输出时：0=输出0,1=输出1	
	
	//DisplayClr();//清屏
#endif
	Comm_Ram->ErrorIC|=RAM_Test(ADDR_Terminal_IRAM_Start,ADDR_TerminalDisplay_IRAM_End-ADDR_Terminal_IRAM_Start);//RAM测试;返回:0=正确,1=错误
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
	SoftDelayUS(10*1000);//延时
  Pin_OutH(PIN_LCD_RST);//复位解除
	SoftDelayUS(100*1000);//实测复位后至少延时50MS,现100MS
#endif
}

void Startdisplay(void)//起动显示
{
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
	u16 *p16;
	u32 i;
	u32 *p32;
/*
	p32=(u32*)(ADDR_AUTO_ERROR_RESTART);//自动检测到如SDRAM等错误看门狗复位标志
	if(p32[0]==0x12345678)
	{
		Pin_SCU(PIN_LCD_RST,0+(0<<3)+(1<<4)+(0<<5)+(1<<6)+(1<<17)+(1<<18));//解除复位LCD
		Comm_Ram->DisplayAllErrorTimer=0;//10;//上电全显或系统错误显示秒定时器
		return;
	}
*/
	
  DisplayString(0,2,0,(u8 *)"用电信息采集系统");//显示字符串
  StrBuff->Row[0].Font=1;
	
#if (Project/100)==0//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"智能表");//显示字符串
#endif
#if (Project/100)==1//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"无线智能表");//显示字符串
#endif
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"Ⅰ型集中器");//显示字符串
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"Ⅲ型专变终端");//显示字符串
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"三合一终端");//显示字符串
#endif
#if (Project/100)==5//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"Ⅱ型集中器");//显示字符串
#endif
#if (Project/100)==6//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"通信模块");//显示字符串
#endif
	DispalyAlign(2,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
#if MainProtocol==698
	DisplayString(3,0,0,(u8 *)"DL/T698.45");//显示字符串
	DispalyAlign(3,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
#else
	DisplayString(3,0,0,(u8 *)"Q/GDW137x-2013");//显示字符串
	DispalyAlign(3,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
#endif
//版本号
	p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4);
#if MainProtocol==376
	#if (USER/100)==5//上海
		p16[0]='Z'+0x2000;
		p16[1]='Z'+0x2000;
		p32=(u32*)(ADDR_Terminal_ENTRY);
		p32=(u32*)p32[114];
		i=p32[4];
		if(i==0xffffffff)
		{
			i=0;
		}
		p16[2]=((i>>4)&0xf)+0x2030;
		p16[3]=((i>>0)&0xf)+0x2030;
	#else
		#if (USER/100)==12//四川
			i=i;
			p32=p32;
			p16[0]='1'+0x2000;
			p16[1]='6'+0x2000;
			p16[2]='C'+0x2000;
			p16[3]='4'+0x2000;
		#else
			p16[0]='V'+0x2000;
			p32=(u32*)(ADDR_Terminal_ENTRY);
			p32=(u32*)p32[114];
			i=p32[4];
			if(i==0xffffffff)
			{
				i=0;
			}
			p16[1]=(i>>8)+0x2030;
			p16[2]='.'+0x2000;
			p16[3]=((i>>4)&0xf)+0x2030;
			//p16[4]=((i>>0)&0xf)+0x2030;
		#endif
	#endif
#endif
#if MainProtocol==698	
			p16[0]='V'+0x2000;
			p32=(u32*)(ADDR_Terminal_ENTRY);
			p32=(u32*)p32[114];
			i=p32[4];
			if(i==0xffffffff)
			{
				i=0;
			}
			p16[1]=(i>>8)+0x2030;
			p16[2]='.'+0x2000;
			p16[3]=((i>>4)&0xf)+0x2030;
			//p16[4]=((i>>0)&0xf)+0x2030;
#endif
#if MainProtocol==CSG
			p16[0]='V'+0x2000;
			p32=(u32*)(ADDR_Terminal_ENTRY);
			p32=(u32*)p32[114];
			i=p32[4];
			if(i==0xffffffff)
			{
				i=0;
			}
			p16[1]=(i>>8)+0x2030;
			p16[2]='.'+0x2000;
			p16[3]=((i>>4)&0xf)+0x2030;
			//p16[4]=((i>>0)&0xf)+0x2030;
#endif

	DispalyAlign(4,0,2);//行显示对齐(1左对齐,2居中,3右对齐)

#if USER==100//用户标识
  DisplayString(5,0,0,(u8 *)"标准版");//显示字符串
#endif
#if USER==200//用户标识
  DisplayString(5,0,0,(u8 *)"山东版");//显示字符串
#endif
#if USER==300//用户标识
  DisplayString(5,0,0,(u8 *)"福建版");//显示字符串
#endif
#if USER==400//用户标识
  DisplayString(5,0,0,(u8 *)"陕西版");//显示字符串
#endif
#if USER==500//用户标识
  DisplayString(5,0,0,(u8 *)"上海版");//显示字符串
#endif
#if USER==600//用户标识
  DisplayString(5,0,0,(u8 *)"湖南版");//显示字符串
#endif
#if USER==601//用户标识
  DisplayString(5,0,0,(u8 *)"湖南湘潭版");//显示字符串
#endif

#if USER==800//用户标识
  DisplayString(5,0,0,(u8 *)"通用版");//显示字符串
#endif
#if USER==900//用户标识
  DisplayString(5,0,0,(u8 *)"河南版");//显示字符串
#endif
#if USER==1000//用户标识
  DisplayString(5,0,0,(u8 *)"河北版");//显示字符串
#endif
#if USER==1100//用户标识
  DisplayString(5,0,0,(u8 *)"吉林版");//显示字符串
#endif
#if USER==1200//用户标识
  DisplayString(5,0,0,(u8 *)"四川版");//显示字符串
#endif

#if USER==1400//用户标识
  DisplayString(5,0,0,(u8 *)"福建版");//显示字符串
#endif
#if (USER/100)==15
  DisplayString(5,0,0,(u8 *)"安徽版");//显示字符串
#endif
#if (USER/100)==16
  DisplayString(5,0,0,(u8 *)"内蒙版");//显示字符串
#endif

#if (USER/100)==33
  DisplayString(5,0,0,(u8 *)"云南版");//显示字符串
#endif

	DispalyAlign(5,0,2);//行显示对齐(1左对齐,2居中,3右对齐)	
	
	
#if (USER/100)!=0//用户标识
  DisplayString(7,1,0,(u8 *)"     正泰仪表");//显示字符串
#endif
  StrBuff->Row[7].Font=1;//2;
	
	LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
#endif
}


#if HVER>=0x410		// 硬件版本号:4.10;LCD总线分开
const u16 PIN_LCD_LABLE[]=
{
	PIN_LCD_CS,
	PIN_LCD_WR,
	PIN_LCD_RD,
	PIN_LCD_A0,
	PIN_LCD_D0,
	PIN_LCD_D1,
	PIN_LCD_D2,
	PIN_LCD_D3,
	PIN_LCD_D4,
	PIN_LCD_D5,
	PIN_LCD_D6,
	PIN_LCD_D7,
};
void Init_PIN_LCD(void)//LCD总线引脚初始化
{
	u32 i;
	for(i=0;i<(sizeof(PIN_LCD_LABLE)/2);i++)
	{
		Pin_Configure(PIN_LCD_LABLE[i],//引脚配置
						0|//B3-B0:复用功能选择AF(0-15)
			 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
			 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
			 (2<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
			 (1<<9)|//B10-B9:上拉/下拉:00=无上拉或下拉,01=上拉,10=下拉,11=保留
			(1<<11));//B11  :当为输出时：0=输出0,1=输出1
	}
}
void LCD_WC(u32 Data)//写LCD命令
{
  GPIO_TypeDef *GPIOx;
	
	GPIOx=(GPIO_TypeDef *)(GPIOA_BASE+((PIN_LCD_D0>>8)*0x400));
	//clr:data,A0,WR,CS
	GPIOx->BSRR=((0xFF<<4)+(1<<(PIN_LCD_A0&0xff))+(1<<(PIN_LCD_WR&0xff))+(1<<(PIN_LCD_CS&0xff)))<<16;
	//set data
	GPIOx->BSRR=Data<<4;
	Data=2;//延时
	while(Data--);
	//set WR
	GPIOx->BSRR=(1<<(PIN_LCD_WR&0xff));
	GPIOx->BSRR=(1<<(PIN_LCD_CS&0xff));
}
void LCD_WD(u32 Data)//写LCD数据
{
  GPIO_TypeDef *GPIOx;
	
	GPIOx=(GPIO_TypeDef *)(GPIOA_BASE+((PIN_LCD_D0>>8)*0x400));
	//clr:data,WR,CS
	GPIOx->BSRR=((0xFF<<4)+(1<<(PIN_LCD_WR&0xff))+(1<<(PIN_LCD_CS&0xff)))<<16;
	//set data,A0
	GPIOx->BSRR=(Data<<4)+(1<<(PIN_LCD_A0&0xff));
	Data=2;//延时
	while(Data--);
	//set WR
	GPIOx->BSRR=1<<(PIN_LCD_WR&0xff);
	GPIOx->BSRR=(1<<(PIN_LCD_CS&0xff));
}
u32 LCD_RC(void)//读LCD命令
{
	u32 i;
	u32 Data;
  GPIO_TypeDef *GPIOx;
	
	//D0-D7配置为输入
	for(i=4;i<(sizeof(PIN_LCD_LABLE)/2);i++)
	{
		Pin_Configure(PIN_LCD_LABLE[i],//引脚配置
						0|//B3-B0:复用功能选择AF(0-15)
			 (0<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
			 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
			 (0<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
			 (0<<9)|//B10-B9:上拉/下拉:00=无上拉或下拉,01=上拉,10=下拉,11=保留
			(0<<11));//B11  :当为输出时：0=输出0,1=输出1
	}
	GPIOx=(GPIO_TypeDef *)(GPIOA_BASE+((PIN_LCD_D0>>8)*0x400));
	//clr A0,CS
	GPIOx->BSRR=((1<<(PIN_LCD_A0&0xff))+(1<<(PIN_LCD_CS&0xff)))<<16;
	//clr RD
	GPIOx->BSRR=(1<<(PIN_LCD_RD&0xff))<<16;
	Data=2;//延时
	while(Data--);
	Data=GPIOx->IDR;
	//set RD
	GPIOx->BSRR=(1<<(PIN_LCD_RD&0xff))+(1<<(PIN_LCD_CS&0xff));
	Data>>=4;
	Data&=0xff;
	//D0-D7配置为输出
	for(i=4;i<(sizeof(PIN_LCD_LABLE)/2);i++)
	{
		Pin_Configure(PIN_LCD_LABLE[i],//引脚配置
						0|//B3-B0:复用功能选择AF(0-15)
			 (1<<4)|//B5-B4:模式:00：输入（复位状态）,01：通用输出模式,10：复用功能模式,11：模拟模式
			 (0<<6)|//B6   :输出类型:0：输出推挽（复位状态）,1：输出开漏
			 (3<<7)|//B8-B7:输出速度:00：2 MHz（低速）,01：25 MHz（中速）,10：50 MHz（快速）,11：30 pF 时为 100 MHz（高速）（15 pF 时为 80 MHz 输出（最大速度））
			 (1<<9)|//B10-B9:上拉/下拉:00=无上拉或下拉,01=上拉,10=下拉,11=保留
			(1<<11));//B11  :当为输出时：0=输出0,1=输出1
	}
	return Data;
}
#endif


#if IC_UC1698==1//0=没,1=有(点阵显示)
#if HVER<0x410//硬件版本号:4.10
//FMC Configuration
//---NOR/PSRAM control-----------
//Memory type    |LCD Interface
//Bank           |Bank 1 NOR/PSRAM 3
//Write operation|Enabled
//Write FIFO     |Enabled
//Extended mode  |Disabled
//---NOR/PSRAM timing-----------
//Address setup time in HCLK clock cycles  |8
//Data setup time in HCLK clock cycles     |8
//Bus turn around time in HCLK clock cycles|0
void LCDDataOut(void)//点阵数据输出到LCD屏
{
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
  u32 i;
  u32 x;
  u32 y;
  u32 z;
  u8 * p8c;
  u8 * p8d;
//	u8 * p8e;

  p8c=(u8 *)0x68000000;
//控制初始化
	//power control
  //26:111010## Set LCD Bias Ratio(Default:11b=12)
  p8c[0]=0xE8+1;//Bias Ratio:1/10 bias
  //6: 001010## Set Power Control(Default:10b)
  p8c[0]=0x28+2;//3;//power control set as internal power
  //5: 001001## Set Temp Compensation(Default:0)
  p8c[0]=0x24+2;//0=-0.00%/℃,1=-0.05%/℃,2=-0.15%/℃,3=-0.25%/℃
  //10:10000001 ######## Set Vbias Potentiometer(double-byte command)(Default:40H)
  p8c[0]=0x81;//electronic potentionmeter
  p8c[0]=0xb8;//0xBF;//PM value:xx  (对比度调节)

  //display control
  //15:1010010# Set All-Pixel-ON(Default:0)
  p8c[0]=0xa4+0;//all pixel off
  //16:1010011# Set Inverse Display(Default:0)
  p8c[0]=0xa6+0;//inverse display off

  //lcd control
  //18:11000### Set LCD Mapping Control(Default:0)
	p8c[0]=0xc0+4;//19:partial display and MX disable,MY enable
//	p8c[0]=0xc0+0;//19:partial display and MX disable,MY enable
  //14:101000## Set Line Rate(Default:10b)
  p8c[0]=0xa0+2;//line rate 0=8.5kips 1=10.4kips 2=12.6kips 3=15.2klps

  //20:1101000# Set Color Patten(Default:0BGR)
  p8c[0]=0xd0+1;//rgb-rgb
  //21:110101## Set Color Mode(Default:10b)
  p8c[0]=0xd4+1;//4k color mode
  //11:1000010# Set Partial Display Control(Default:0)
  p8c[0]=0x84+0;//partial display control disable

  //n-line inversion
  //19:11001000 ---##### Set N-Line Inversion(Default:1DH)
  p8c[0]=0xc8;//
  p8c[0]=0;//
  //com scan fuction
  //22:11011### Set COM Scan Function(Default:0)
  p8c[0]=0xd8+2;//enable FRC,PWM,LRM sequence
  //window
  //30:11110100 -####### Set Window Program Starting Column Address(Default:0)
  p8c[0]=0xf4;//wpc0:column start from 130
  p8c[0]=0x25;//
  //32:11110110 -####### Set Window Program Ending Column Address(Default:127)
  p8c[0]=0xf6;//wpc1:column End 272
  p8c[0]=0x5a;//
  //31:11110101 ######## Set Window Program Starting Row Address(Default:0)
  p8c[0]=0xf5;//wpp0:row start from 0
  p8c[0]=0;//
  //33:11110111 ######## Set Window Program Ending Row Address(Default:159)
  p8c[0]=0xf7;//wpp0 row end 160
  p8c[0]=0x9f;//
  //34:1111100# Window Program Mode(Default:0:Inside)
  p8c[0]=0xf8;//UC1698Write1ByteCommand(0xf8);//inside mode
  //12:10001### Set RAM Address Control(Default:001b)
//  UC1698Write1ByteCommand(0x88+1);//RAM control,先列+1,列回0后行+1
  //17:10101### Set Display Enable(Default:110b)
  p8c[0]=0xa8+5;//display on,select on/off mode.Green Enhance mode disable
	
  //scroll line
  //8: 0100#### Set Scroll Line LSB(Default:0)
  p8c[0]=0x40+0;//low bit of scroll line
  //   0101#### Set Scroll Line MSB(Default:0)
  p8c[0]=0x50+0;//high bit of scroll line

  //13:10010000 ######## Set Fixed Lines(Default:0)
  p8c[0]=0x90;//FLT,FLB set
  p8c[0]=0x0;//
	
  //partial display
  //11:1000010# Set Partial Display Control(Default:0)
  p8c[0]=0x84+0;//set partial display control:off
  //27:11110001 -####### Set COM End(Default:159)
  p8c[0]=0xf1;//com end 159
  p8c[0]=159;//
  //28:11110010 -####### Set Partial Display Start(Default:0)
  p8c[0]=0xf2;//display start 0
  p8c[0]=0;//
  //29:11110011 -####### Set Partial Display End(Default:159)
  p8c[0]=0xf3;//display end 159
  p8c[0]=159;//
  
//LCD死机检测
	for(i=0;i<1000;i++)
	{
		x=p8c[0];
		if((x==0xBA)||(x==0xBE))
		{
			break;
		}
		SoftDelayUS(1);
	}
	if(i>=1000)
	{//错误
		Init_LCDModule();//点阵LCD显示外部总线控制EMC初始化
		return;
	}
	
	
  //1: ######## Write Data Byte
  //2: ######## Read Data Byte
  //3: Get Status&PM
  //4: 0000#### Set Column Address LSB(Default:0)
  //   00010### Set Column Address MSB(Default:0)
  //9: 0110#### Set Row Address LSB(Default:0)
  //   0111#### Set Row Address MSB(Default:0)
  //23:11100010 System Reset
  //24:11100011 NOP
  //35:10111000 ---##### Set MTP Opration Control(Default:10H)
  //36:10111001 -####### ------## Set MTP Write Mask(Default:0)
  //37:11110100 ######## Set Vmtp1 Potentiomter(Default:N/A)
  //38:11110101 ######## Set Vmtp2 Potentiomter(Default:N/A)
  //39:11110110 ######## Set MTP Write Timer(Default:N/A)
  //40:11110111 ######## Set MTP Read Timer(Default:N/A)

	
//	if((Comm_Ram->SelfTestFlags&0x3)==0)
//	{
//  DisplayFlash();//显示动画
  LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
//	}


//加横线
	if(Comm_Ram->DisplayAllErrorTimer==0)
	{
#if MainProtocol==CSG
			//上横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//下横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
#else
	#if (USER/100)==5//上海
		p8d=(u8*)(ADDR_MenuICO);//1byte 菜单中显示顶行和底行图标 0=否,1=是
		if((DOTMIXPMODE->Task==4)||(p8d[0]!=0))//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
		{
	#else
		if(DOTMIXPMODE->Task==4)//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
		{
	#endif
			//上横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//下横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
		}
#endif
	}
//点阵缓冲A和B合成输出
  p8c[0]=0x88+1;//RAM control,先列+1,列回0后行+1
	
//	switch(DOTMIXPMODE->Mix)//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
//	{
//		case 0://A

//正显
			p8d=(u8 *)ADDR_DOTDATABUFF;
  		for(y=0;y<160;y++)
  		{
    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
    		p8c[0]=0x00+0x5;//Set Column Address LSB
    		p8c[0]=0x10+0x2;//Set Column Address MSB
    		for(x=0;x<20;x++)
    		{
      		z=p8d[0];
      		p8d++;
      		i=0;
      		if(z&0x01)
      		{
        		i=0x80;
      		}
      		if(z&0x02)
      		{
        		i|=0x8;
      		}
      		p8c[4]=i;
      		i=0;
      		if(z&0x04)
      		{
      	  	i=0x80;
      		}
      		if(z&0x08)
      		{
       	 		i|=0x8;
      		}
      		p8c[4]=i;
      		i=0;
      		if(z&0x10)
      		{
        		i=0x80;
      		}
      		if(z&0x20)
      		{
      	  	i|=0x8;
    	  	}
      		p8c[4]=i;
      		i=0;
      		if(z&0x40)
      		{
      	  	i=0x80;
      		}
      		if(z&0x80)
      		{
       	 		i|=0x8;
      		}
    	  	p8c[4]=i;
    		}
    		p8c[4]=0;
	 		}

/*
//倒显
			p8d=(u8 *)ADDR_DOTDATABUFF;
  		for(y=0;y<160;y++)
  		{
    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
    		p8c[0]=0x00+0x5;//Set Column Address LSB
    		p8c[0]=0x10+0x2;//Set Column Address MSB
    		for(x=0;x<20;x++)
    		{
      		z=p8d[19-x];
      		i=0;
      		if(z&0x80)
      		{
        		i=0x80;
      		}
      		if(z&0x40)
      		{
        		i|=0x8;
      		}
      		p8c[4]=i;
      		i=0;
      		if(z&0x20)
      		{
      	  	i=0x80;
      		}
      		if(z&0x10)
      		{
       	 		i|=0x8;
      		}
      		p8c[4]=i;
      		i=0;
      		if(z&0x08)
      		{
        		i=0x80;
      		}
      		if(z&0x04)
      		{
      	  	i|=0x8;
    	  	}
      		p8c[4]=i;
      		i=0;
      		if(z&0x02)
      		{
      	  	i=0x80;
      		}
      		if(z&0x01)
      		{
       	 		i|=0x8;
      		}
    	  	p8c[4]=i;
    		}
    		p8c[4]=0;
				p8d+=20;
	 		}
			//18:11000### Set LCD Mapping Control(Default:0)
			p8c[0]=0xc0+2;//19:partial display and MX disable,MY enable
*/
		



//			break;
//		case 1://B
//			p8d=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//      		p8d++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 2://A|B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z|=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 3://A^B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z^=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 4://A&B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z&=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//	}
#endif
}
#else//#if HVER<0x410//硬件版本号:4.10
//HVER>=0x410//硬件版本号:4.10
#include "../STM32F4xx/STM32F4xx_UART.h"
void LCDDataOut(void)//点阵数据输出到LCD屏
{
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
  u32 i;
  u32 x;
  u32 y;
  u32 z;
  u8 * p8d;

//---测试显示---
//  UARTCtrl_TypeDef* UARTCtrl;
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//	p8d=(u8*)ADDR_DATABUFF;
//	i=UARTCtrl->TaskID;
//	if(i)
//	{
//		MC(0,ADDR_DATABUFF,8);
//		p8d[0]=UARTCtrl->ReRMCount;
//		i>>=8;
//		i&=0xff;
//		i=UARTCtrl->NUMrmFAIL[i];
//		p8d[1]=i;
//		DisplayRam(8,ADDR_DATABUFF);//显示中RAM数据(调试用)
//		
//		i=(u32)&UARTCtrl->NUMrmFAIL[24];
//		if(i)
//		{
//			i=i;
//		}
//	}
	
//  if(MenuCount->Count==0)//菜单堆栈计数
//  {//菜单堆栈计数=0
//		i=MRR(ADDR_UA+2,2);
//		x=(i>>8)|((i&0xff)<<8);
//		MWR(x,ADDR_DATABUFF,5);
//		DisplayRam(5,ADDR_DATABUFF);//显示中RAM数据(调试用)
//		i=MRR(ADDR_UB+2,2);
//		x=(i>>8)|((i&0xff)<<8);
//		MWR(x,ADDR_DATABUFF,5);
//		DisplayRam(6,ADDR_DATABUFF);//显示中RAM数据(调试用)
//		i=MRR(ADDR_UC+2,2);
//		x=(i>>8)|((i&0xff)<<8);
//		MWR(x,ADDR_DATABUFF,5);
//		DisplayRam(7,ADDR_DATABUFF);//显示中RAM数据(调试用)
//		DisplayRam(8,ADDR_PowerEvent+15);//显示中RAM数据(调试用)
//	}

//DisplayRam(8,ADDR_6000_SDRAM+(2*LENper_6000));//显示中RAM数据(调试用)


//---测试显示end---
	
	
	Init_PIN_LCD();//LCD总线引脚初始化
//控制初始化
	//power control
  //26:111010## Set LCD Bias Ratio(Default:11b=12)
  LCD_WC(0xE8+1);//p8c[0]=0xE8+1;//Bias Ratio:1/10 bias
  //6: 001010## Set Power Control(Default:10b)
  LCD_WC(0x28+2);//p8c[0]=0x28+2;//3;//power control set as internal power
  //5: 001001## Set Temp Compensation(Default:0)
  LCD_WC(0x24+2);//p8c[0]=0x24+2;//0=-0.00%/℃,1=-0.05%/℃,2=-0.15%/℃,3=-0.25%/℃
  //10:10000001 ######## Set Vbias Potentiometer(double-byte command)(Default:40H)
	LCD_WC(0x81);// p8c[0]=0x81;//electronic potentionmeter
  LCD_WC(0xb8);//p8c[0]=0xb8;//0xBF;//PM value:xx  (对比度调节)

  //display control
  //15:1010010# Set All-Pixel-ON(Default:0)
  LCD_WC(0xa4+0);//p8c[0]=0xa4+0;//all pixel off
  //16:1010011# Set Inverse Display(Default:0)
  LCD_WC(0xa6+0);//p8c[0]=0xa6+0;//inverse display off

  //lcd control
  //18:11000### Set LCD Mapping Control(Default:0)
  LCD_WC(0xc0+4);//p8c[0]=0xc0+4;//19:partial display and MX disable,MY enable
  //14:101000## Set Line Rate(Default:10b)
  LCD_WC(0xa0+2);//p8c[0]=0xa0+2;//line rate 0=8.5kips 1=10.4kips 2=12.6kips 3=15.2klps

  //20:1101000# Set Color Patten(Default:0BGR)
  LCD_WC(0xd0+1);//p8c[0]=0xd0+1;//rgb-rgb
  //21:110101## Set Color Mode(Default:10b)
  LCD_WC(0xd4+1);//p8c[0]=0xd4+1;//4k color mode
  //11:1000010# Set Partial Display Control(Default:0)
  LCD_WC(0x84+0);//p8c[0]=0x84+0;//partial display control disable

  //n-line inversion
  //19:11001000 ---##### Set N-Line Inversion(Default:1DH)
  LCD_WC(0xc8);//p8c[0]=0xc8;//
  LCD_WC(0);//p8c[0]=0;//
  //com scan fuction
  //22:11011### Set COM Scan Function(Default:0)
  LCD_WC(0xd8+2);//p8c[0]=0xd8+2;//enable FRC,PWM,LRM sequence
  //window
  //30:11110100 -####### Set Window Program Starting Column Address(Default:0)
  LCD_WC(0xf4);//p8c[0]=0xf4;//wpc0:column start from 130
  LCD_WC(0x25);//p8c[0]=0x25;//
  //32:11110110 -####### Set Window Program Ending Column Address(Default:127)
	LCD_WC(0xf6);// p8c[0]=0xf6;//wpc1:column End 272
  LCD_WC(0x5a);//p8c[0]=0x5a;//
  //31:11110101 ######## Set Window Program Starting Row Address(Default:0)
  LCD_WC(0xf5);//p8c[0]=0xf5;//wpp0:row start from 0
  LCD_WC(0);//p8c[0]=0;//
  //33:11110111 ######## Set Window Program Ending Row Address(Default:159)
  LCD_WC(0xf7);//p8c[0]=0xf7;//wpp0 row end 160
  LCD_WC(0x9f);//p8c[0]=0x9f;//
  //34:1111100# Window Program Mode(Default:0:Inside)
  LCD_WC(0xf8);//p8c[0]=0xf8;//UC1698Write1ByteCommand(0xf8);//inside mode
  //12:10001### Set RAM Address Control(Default:001b)
//  UC1698Write1ByteCommand(0x88+1);//RAM control,先列+1,列回0后行+1
  //17:10101### Set Display Enable(Default:110b)
  LCD_WC(0xa8+5);//p8c[0]=0xa8+5;//display on,select on/off mode.Green Enhance mode disable
  //scroll line
  //8: 0100#### Set Scroll Line LSB(Default:0)
  LCD_WC(0x40+0);//p8c[0]=0x40+0;//low bit of scroll line
  //   0101#### Set Scroll Line MSB(Default:0)
  LCD_WC(0x50+0);//p8c[0]=0x50+0;//high bit of scroll line
  //13:10010000 ######## Set Fixed Lines(Default:0)
  LCD_WC(0x90);//p8c[0]=0x90;//FLT,FLB set
  LCD_WC(0);//p8c[0]=0x0;//
  //partial display
  //11:1000010# Set Partial Display Control(Default:0)
  LCD_WC(0x84+0);//p8c[0]=0x84+0;//set partial display control:off
  //27:11110001 -####### Set COM End(Default:159)
  LCD_WC(0xf1);//p8c[0]=0xf1;//com end 159
  LCD_WC(159);//p8c[0]=159;//
  //28:11110010 -####### Set Partial Display Start(Default:0)
  LCD_WC(0xf2);//p8c[0]=0xf2;//display start 0
  LCD_WC(0);//p8c[0]=0;//
  //29:11110011 -####### Set Partial Display End(Default:159)
  LCD_WC(0xf3);//p8c[0]=0xf3;//display end 159
  LCD_WC(159);//p8c[0]=159;//
  
//LCD死机检测
	for(i=0;i<1000;i++)
	{
		x=LCD_RC();
		if((x==0xBA)||(x==0xBE))
		{
			break;
		}
		SoftDelayUS(1);
	}
	if(i>=1000)
	{//错误
		Init_LCDModule();//点阵LCD显示外部总线控制EMC初始化
		return;
	}
  //1: ######## Write Data Byte
  //2: ######## Read Data Byte
  //3: Get Status&PM
  //4: 0000#### Set Column Address LSB(Default:0)
  //   00010### Set Column Address MSB(Default:0)
  //9: 0110#### Set Row Address LSB(Default:0)
  //   0111#### Set Row Address MSB(Default:0)
  //23:11100010 System Reset
  //24:11100011 NOP
  //35:10111000 ---##### Set MTP Opration Control(Default:10H)
  //36:10111001 -####### ------## Set MTP Write Mask(Default:0)
  //37:11110100 ######## Set Vmtp1 Potentiomter(Default:N/A)
  //38:11110101 ######## Set Vmtp2 Potentiomter(Default:N/A)
  //39:11110110 ######## Set MTP Write Timer(Default:N/A)
  //40:11110111 ######## Set MTP Read Timer(Default:N/A)




//	if((Comm_Ram->SelfTestFlags&0x3)==0)
//	{
//  DisplayFlash();//显示动画
  LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
//	}


//加横线
	if(Comm_Ram->DisplayAllErrorTimer==0)
	{
#if MainProtocol==CSG
			//上横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//下横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
#else
	#if (USER/100)==5//上海
		p8d=(u8*)(ADDR_MenuICO);//1byte 菜单中显示顶行和底行图标 0=否,1=是
		if((DOTMIXPMODE->Task==4)||(p8d[0]!=0))//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
		{
	#else
		if(DOTMIXPMODE->Task==4)//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
		{
	#endif
			//上横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//下横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
		}
#endif
	}
//点阵缓冲A和B合成输出
  LCD_WC(0x88+1);//p8c[0]=0x88+1;//RAM control,先列+1,列回0后行+1
//	switch(DOTMIXPMODE->Mix)//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
//	{
//		case 0://A
			p8d=(u8 *)ADDR_DOTDATABUFF;
  		for(y=0;y<160;y++)
  		{
    		LCD_WC(0x60+(y&0xf));//p8c[0]=0x60+(y&0xf);//Set Row Address LSB
    		LCD_WC(0x70+((y>>4)&0xf));//p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
    		LCD_WC(0x00+0x5);//p8c[0]=0x00+0x5;//Set Column Address LSB
    		LCD_WC(0x10+0x2);//p8c[0]=0x10+0x2;//Set Column Address MSB
    		for(x=0;x<20;x++)
    		{
      		z=p8d[0];
      		p8d++;
      		i=0;
      		if(z&0x01)
      		{
        		i=0x80;
      		}
      		if(z&0x02)
      		{
        		i|=0x8;
      		}
      		LCD_WD(i);//p8c[4]=i;
      		i=0;
      		if(z&0x04)
      		{
      	  	i=0x80;
      		}
      		if(z&0x08)
      		{
       	 		i|=0x8;
      		}
      		LCD_WD(i);//p8c[4]=i;
      		i=0;
      		if(z&0x10)
      		{
        		i=0x80;
      		}
      		if(z&0x20)
      		{
      	  	i|=0x8;
    	  	}
      		LCD_WD(i);//p8c[4]=i;
      		i=0;
      		if(z&0x40)
      		{
      	  	i=0x80;
      		}
      		if(z&0x80)
      		{
       	 		i|=0x8;
      		}
    	  	LCD_WD(i);//p8c[4]=i;
    		}
    		LCD_WD(0);//p8c[4]=0;
	 		}

//			break;
//		case 1://B
//			p8d=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//      		p8d++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 2://A|B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z|=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 3://A^B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z^=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 4://A&B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z&=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//	}
#endif
}
#endif//#if HVER<0x410//硬件版本号:4.10
#endif //#if IC_UC1698==1//0=没,1=有(点阵显示)




#if IC_ST75161==1//0=没,1=有(点阵显示)
#if HVER<0x410//硬件版本号:4.10
void LCDDataOut(void)//点阵数据输出到LCD屏
{
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
  u32 i;
  u32 x;
  u32 y;
  u32 z;
	u32 b;
	u32 c;
  u8 * p8c;
  u8 * p8d;

//控制初始化
  p8c=(u8 *)0x68000000;
	p8c[0]=0x31;//Extension Command 2 
	p8c[0]=0xD7;//Disable Auto Read 
	p8c[4]=0x9F; 
	p8c[0]=0xE0;//Enable OTP read 
	p8c[4]=0x00;//Disable Auto Read 
  SoftDelayUS(40);//软件us延时
	p8c[0]=0xE3;//OTP Up-Load 
	SoftDelayUS(40);//软件us延时
	p8c[0]=0xE1;//OTP Control Out

	p8c[0]=0x30;//Extension Command 1
	p8c[0]=0x94;//Sleep out 
	
//	if((Comm_Ram->SelfTestFlags&0x3)==0)
//	{
//  DisplayFlash();//显示动画
#if Font_ttf==0
  LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
#else
	if(Comm_Ram->Init_emWin<2)//上电后emWin已初始化标志:=0没初始化,=1初始化失败,>=2初始化成功
	{
		LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
	}
	else
	{
		TTF_LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
	}
#endif
//	}

//加横线
	if(Comm_Ram->DisplayAllErrorTimer==0)
	{
#if MainProtocol==CSG
			//上横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//下横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
#else
	#if (USER/100)==5//上海
		p8d=(u8*)(ADDR_MenuICO);//1byte 菜单中显示顶行和底行图标 0=否,1=是
		if((DOTMIXPMODE->Task==4)||(p8d[0]!=0))//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
		{
	#else
		if(DOTMIXPMODE->Task==4)//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
		{
	#endif
			//上横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//下横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
		}
#endif
	}

//控制初始化
	p8c[0]=0x20;//Power Control
	p8c[4]=0x0B;//VB, VR,VF All ON
	p8c[0]=0x81;//set Vop
	p8c[4]=0x3F;//Vop[5:0]  15.5V    1/12  0x30
	p8c[4]=0x04;//Vop[8:6]
	
	p8c[0]=0x31;//Extension Command 2
	p8c[0]=0x20;//Set Gray Scale Level 
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x17;//
	p8c[4]=0x17;//
	p8c[4]=0x17;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x1D;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x1D;//
	p8c[4]=0x1D;//
	p8c[4]=0x1D;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[0]=0x32;//Analog Circuit Set
	p8c[4]=0x00;
	p8c[4]=0x01;//Booster Efficiency =6KHz 
	p8c[4]=0x02;//12 bias
	p8c[0]=0x51;//Booster Level Set
	p8c[4]=0xFa;//Booster Level x8
	
	p8c[0]=0x30;//Extension Command 1
	p8c[0]=0xF0;//Display Mode
	p8c[4]=0x10;//Mono Mode
	p8c[0]=0x0c;//LSB on top
	p8c[0]=0xCA;//Displsy Control
	p8c[4]=0x00;//CL Dividing Ratio:Not Divide 
	p8c[4]=0x9F;//Duty Set:160 Duty
	p8c[4]=0x00;//Nline=
	p8c[0]=0xBC;//Data Scan Direction 
	p8c[4]=0x00;//MV=0,MX=0
	p8c[0]=0xA6;//Normal Display
	
	p8c[0]=0x31;//Extension Command 2
	p8c[0]=0x40;//Internal Power Supply
	p8c[0]=0xF0;// Frame Rate 
	p8c[4]=0x06;// FA=46Hz
	p8c[4]=0x0C;// FB=55Hz		0B
	p8c[4]=0x0C;// FC=55Hz	  0B
	p8c[4]=0x0C;// FD=55Hz	  0B
	p8c[0]=0xF2;// Temperature Range
	p8c[4]=0x05;// TA=-35
	p8c[4]=0x23;// TB=-5
	p8c[4]=0x40;// TC=24
	p8c[0]=0xF4;// Temperature Gradient Compensation
	p8c[4]=0xAF;// 0xFF
	p8c[4]=0x07;// 0x0F
	p8c[4]=0x00;// 
	p8c[4]=0x00;// 
	p8c[4]=0x00;// 
	p8c[4]=0x00;//00 	
	p8c[4]=0xAF;//56~64;64~72
	p8c[4]=0xFF;//

//LCD死机检测
	for(i=0;i<1000;i++)
	{
		p8c[0]=0x25;//NOP operation
		x=p8c[0];
		if((x==0xC2)||(x==0xCA))
		{
			break;
		}
		SoftDelayUS(1);
	}
	if(i>=1000)
	{//错误
		Init_LCDModule();//点阵LCD显示外部总线控制EMC初始化
		return;
	}

//window
	p8c[0]=0x30;//Extension Command 1 
	p8c[0]=0x15;//Column Address Setting 
	p8c[4]=0x00;//Starting column address
	p8c[4]=0x9F;//Ending column address

	p8c[0]=0x75;//Row Address Setting 
	p8c[4]=0x00;//Starting page address 
	p8c[4]=0x13;//Ending page address
	p8c[0]=0x5c;//Write data

	for(y=0;y<20;y++)
	{
		p8d=(u8 *)ADDR_DOTDATABUFF+(y*160);
		for(x=0;x<20;x++)
		{
			c=0x01;
			for(z=0;z<8;z++)
			{
				i=0;
				for(b=0;b<8;b++)
				{
					if(p8d[b*20]&c)
					{
						i|=(1<<b);
					}
				}
				p8c[4]=i;
				c<<=1;
			}
			p8d++;
		}
	}
	p8c[0]=0xAF;//Display ON
#endif
}
#else//#if HVER<0x410//硬件版本号:4.10
//HVER>=0x410//硬件版本号:4.10
void LCDDataOut(void)//点阵数据输出到LCD屏
{
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
  u32 i;
  u32 x;
  u32 y;
  u32 z;
	u32 b;
	u32 c;
  u8 * p8d;

	
//---测试显示---
	
#if TEST_DISPVDD5V0==1//0无效,1=运行;显示VDD5V0电压
	#include "../STM32F4xx/STM32F4xx_ADC.h"
	VDD5V0_Voltage();//VDD5V0电压ADC测量
	DisplayRam(8,ADDR_VDD5V0);//显示中RAM数据(调试用)
#endif
	
//---测试显示end---
	
	
	
	
	Init_PIN_LCD();//LCD总线引脚初始化
//控制初始化
	LCD_WC(0x31);//p8c[0]=0x31;//Extension Command 2 
	LCD_WC(0xd7);//p8c[0]=0xD7;//Disable Auto Read 
	LCD_WD(0x9f);//p8c[4]=0x9F; 
	LCD_WC(0xe0);//p8c[0]=0xE0;//Enable OTP read 
	LCD_WD(0x00);//p8c[4]=0x00;//Disable Auto Read 
  SoftDelayUS(40);//软件us延时
	LCD_WC(0xe3);//p8c[0]=0xE3;//OTP Up-Load 
	SoftDelayUS(40);//软件us延时
	LCD_WC(0xe1);//p8c[0]=0xE1;//OTP Control Out

	LCD_WC(0x30);//p8c[0]=0x30;//Extension Command 1
	LCD_WC(0x94);//p8c[0]=0x94;//Sleep out 
	
//	if((Comm_Ram->SelfTestFlags&0x3)==0)
//	{
//  DisplayFlash();//显示动画
#if Font_ttf==0
  LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
#else
	if(Comm_Ram->Init_emWin<2)//上电后emWin已初始化标志:=0没初始化,=1初始化失败,>=2初始化成功
	{
		LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
	}
	else
	{
		TTF_LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
	}
#endif
//	}

//加横线
	if(Comm_Ram->DisplayAllErrorTimer==0)
	{
#if MainProtocol==CSG
			//上横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//下横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
#else
	#if (USER/100)==5//上海
		p8d=(u8*)(ADDR_MenuICO);//1byte 菜单中显示顶行和底行图标 0=否,1=是
		if((DOTMIXPMODE->Task==4)||(p8d[0]!=0))//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
		{
	#else
		if(DOTMIXPMODE->Task==4)//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
		{
	#endif
			//上横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//下横线
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
		}
#endif
	}

//控制初始化
	LCD_WC(0x20);//p8c[0]=0x20;//Power Control
	LCD_WD(0x0B);//p8c[4]=0x0B;//VB, VR,VF All ON
	LCD_WC(0x81);//p8c[0]=0x81;//set Vop
	LCD_WD(0x3F);//p8c[4]=0x3F;//Vop[5:0]  15.5V    1/12  0x30
	LCD_WD(0x04);//p8c[4]=0x04;//Vop[8:6]
	
	LCD_WC(0x31);//p8c[0]=0x31;//Extension Command 2
	LCD_WC(0x20);//p8c[0]=0x20;//Set Gray Scale Level 
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WD(0x17);//p8c[4]=0x17;//
	LCD_WD(0x17);//p8c[4]=0x17;//
	LCD_WD(0x17);//p8c[4]=0x17;//
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WD(0x1D);//p8c[4]=0x1D;//
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WD(0x1D);//p8c[4]=0x1D;//
	LCD_WD(0x1D);//p8c[4]=0x1D;//
	LCD_WD(0x1D);//p8c[4]=0x1D;//
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WD(0x00);//p8c[4]=0x00;//
	LCD_WC(0x32);//p8c[0]=0x32;//Analog Circuit Set
	LCD_WD(0x00);//p8c[4]=0x00;
	LCD_WD(0x01);//p8c[4]=0x01;//Booster Efficiency =6KHz 
	LCD_WD(0x02);//p8c[4]=0x02;//12 bias
	LCD_WC(0x51);//p8c[0]=0x51;//Booster Level Set
	LCD_WD(0xFA);//p8c[4]=0xFa;//Booster Level x8
	
	LCD_WC(0x30);//p8c[0]=0x30;//Extension Command 1
	LCD_WC(0xF0);//p8c[0]=0xF0;//Display Mode
	LCD_WD(0x10);//p8c[4]=0x10;//Mono Mode
	LCD_WC(0x0C);//p8c[0]=0x0c;//LSB on top
	LCD_WC(0xCA);//p8c[0]=0xCA;//Displsy Control
	LCD_WD(0x00);//p8c[4]=0x00;//CL Dividing Ratio:Not Divide 
	LCD_WD(0x9F);//p8c[4]=0x9F;//Duty Set:160 Duty
	LCD_WD(0x00);//p8c[4]=0x00;//Nline=
	LCD_WC(0xBC);//p8c[0]=0xBC;//Data Scan Direction 
	LCD_WD(0x00);//p8c[4]=0x00;//MV=0,MX=0
	LCD_WC(0xA6);//p8c[0]=0xA6;//Normal Display
	
	LCD_WC(0x31);//p8c[0]=0x31;//Extension Command 2
	LCD_WC(0x40);//p8c[0]=0x40;//Internal Power Supply
	LCD_WC(0xF0);//p8c[0]=0xF0;// Frame Rate 
	LCD_WD(0x06);//p8c[4]=0x06;// FA=46Hz
	LCD_WD(0x0C);//p8c[4]=0x0C;// FB=55Hz		0B
	LCD_WD(0x0C);//p8c[4]=0x0C;// FC=55Hz	  0B
	LCD_WD(0x0C);//p8c[4]=0x0C;// FD=55Hz	  0B
	LCD_WC(0xF2);//p8c[0]=0xF2;// Temperature Range
	LCD_WD(0x05);//p8c[4]=0x05;// TA=-35
	LCD_WD(0x23);//p8c[4]=0x23;// TB=-5
	LCD_WD(0x40);//p8c[4]=0x40;// TC=24
	LCD_WC(0xF4);//p8c[0]=0xF4;// Temperature Gradient Compensation
	LCD_WD(0xAF);//p8c[4]=0xAF;// 0xFF
	LCD_WD(0x07);//p8c[4]=0x07;// 0x0F
	LCD_WD(0x00);//p8c[4]=0x00;// 
	LCD_WD(0x00);//p8c[4]=0x00;// 
	LCD_WD(0x00);//p8c[4]=0x00;// 
	LCD_WD(0x00);//p8c[4]=0x00;//00 	
	LCD_WD(0xAF);//p8c[4]=0xAF;//56~64;64~72
	LCD_WD(0xFF);//p8c[4]=0xFF;//

//LCD死机检测
	for(i=0;i<1000;i++)
	{
		LCD_WC(0x25);//p8c[0]=0x25;//NOP operation
		x=LCD_RC();
		if((x==0xC2)||(x==0xCA))
		{
			break;
		}
		SoftDelayUS(1);
	}
	if(i>=1000)
	{//错误
		Init_LCDModule();//点阵LCD显示外部总线控制EMC初始化
		return;
	}

//window
	LCD_WC(0x30);//p8c[0]=0x30;//Extension Command 1 
	LCD_WC(0x15);//p8c[0]=0x15;//Column Address Setting 
	LCD_WD(0x00);//p8c[4]=0x00;//Starting column address
	LCD_WD(0x9F);//p8c[4]=0x9F;//Ending column address

	LCD_WC(0x75);//p8c[0]=0x75;//Row Address Setting 
	LCD_WD(0x00);//p8c[4]=0x00;//Starting page address 
	LCD_WD(0x13);//p8c[4]=0x13;//Ending page address
	LCD_WC(0x5C);//p8c[0]=0x5c;//Write data

	for(y=0;y<20;y++)
	{
		p8d=(u8 *)ADDR_DOTDATABUFF+(y*160);
		for(x=0;x<20;x++)
		{
			c=0x01;
			for(z=0;z<8;z++)
			{
				i=0;
				for(b=0;b<8;b++)
				{
					if(p8d[b*20]&c)
					{
						i|=(1<<b);
					}
				}
				LCD_WD(i);//p8c[4]=i;
				c<<=1;
			}
			p8d++;
		}
	}
	LCD_WC(0xAF);//p8c[0]=0xAF;//Display ON
#endif
}
#endif//#if HVER<0x410//硬件版本号:4.10
#endif//#if IC_ST75161==1//0=没,1=有(点阵显示)


/*
void GUI_LCDDataOut(void)//点阵数据输出到LCD屏
{
  u32 i;
  u32 x;
  u32 y;
  u32 z;
	u32 b;
	u32 c;
  u8 * p8c;
  u8 * p8d;
	
//控制初始化
  p8c=(u8 *)0x68000000;
	p8c[0]=0x31;//Extension Command 2 
	p8c[0]=0xD7;//Disable Auto Read 
	p8c[4]=0x9F; 
	p8c[0]=0xE0;//Enable OTP read 
	p8c[4]=0x00;//Disable Auto Read 
	//SoftDelayUS(40);//软件us延时
	i = 40 * (168000000/4000000);
  while(i--);
	
	p8c[0]=0xE3;//OTP Up-Load 
	//SoftDelayUS(40);//软件us延时
	i = 40 * (168000000/4000000);
  while(i--);
	
	p8c[0]=0xE1;//OTP Control Out

	p8c[0]=0x30;//Extension Command 1
	p8c[0]=0x94;//Sleep out 
	
//控制初始化
	p8c[0]=0x20;//Power Control
	p8c[4]=0x0B;//VB, VR,VF All ON
	p8c[0]=0x81;//set Vop
	p8c[4]=0x3F;//Vop[5:0]  15.5V    1/12  0x30
	p8c[4]=0x04;//Vop[8:6]
	
	p8c[0]=0x31;//Extension Command 2
	p8c[0]=0x20;//Set Gray Scale Level 
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x17;//
	p8c[4]=0x17;//
	p8c[4]=0x17;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x1D;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x1D;//
	p8c[4]=0x1D;//
	p8c[4]=0x1D;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[0]=0x32;//Analog Circuit Set
	p8c[4]=0x00;
	p8c[4]=0x01;//Booster Efficiency =6KHz 
	p8c[4]=0x02;//12 bias
	p8c[0]=0x51;//Booster Level Set
	p8c[4]=0xFa;//Booster Level x8
	
	p8c[0]=0x30;//Extension Command 1
	p8c[0]=0xF0;//Display Mode
	p8c[4]=0x10;//Mono Mode
	p8c[0]=0x0c;//LSB on top
	p8c[0]=0xCA;//Displsy Control
	p8c[4]=0x00;//CL Dividing Ratio:Not Divide 
	p8c[4]=0x9F;//Duty Set:160 Duty
	p8c[4]=0x00;//Nline=
	p8c[0]=0xBC;//Data Scan Direction 
	p8c[4]=0x00;//MV=0,MX=0
	p8c[0]=0xA6;//Normal Display
	
	p8c[0]=0x31;//Extension Command 2
	p8c[0]=0x40;//Internal Power Supply
	p8c[0]=0xF0;// Frame Rate 
	p8c[4]=0x06;// FA=46Hz
	p8c[4]=0x0C;// FB=55Hz		0B
	p8c[4]=0x0C;// FC=55Hz	  0B
	p8c[4]=0x0C;// FD=55Hz	  0B
	p8c[0]=0xF2;// Temperature Range
	p8c[4]=0x05;// TA=-35
	p8c[4]=0x23;// TB=-5
	p8c[4]=0x40;// TC=24
	p8c[0]=0xF4;// Temperature Gradient Compensation
	p8c[4]=0xFF;// 
	p8c[4]=0x0F;//0x08
	p8c[4]=0x00;// 
	p8c[4]=0x00;// 
	p8c[4]=0x00;// 
	p8c[4]=0x00;//00 	
	p8c[4]=0xAF;//56~64;64~72
	p8c[4]=0xFF;//

//window
	p8c[0]=0x30;//Extension Command 1 
	p8c[0]=0x15;//Column Address Setting 
	p8c[4]=0x00;//Starting column address
	p8c[4]=0x9F;//Ending column address

	p8c[0]=0x75;//Row Address Setting 
	p8c[4]=0x00;//Starting page address 
	p8c[4]=0x13;//Ending page address
	p8c[0]=0x5c;//Write data

	for(y=0;y<20;y++)
	{
		p8d=(u8*)ADDR_DOTDATABUFF;//_aPlain_0;
		p8d+=(y*160);
		for(x=0;x<20;x++)
		{
			c=0x01;
			for(z=0;z<8;z++)
			{
				i=0;
				for(b=0;b<8;b++)
				{
					if(p8d[b*20]&c)
					{
						i|=(1<<b);
					}
				}
				p8c[4]=i;
				c<<=1;
			}
			p8d++;
		}
	}
	p8c[0]=0xAF;//Display ON
}
*/





