
//工厂测试(扩展)
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"






void Uart_AFN255(u32 PORTn)//工厂测试(扩展)
{
	u32 i;
	u32 DIlen;
	u32 Fn;
//	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 *p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<12)
	{
err:
		//全部否认
		Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		return;
ok:
		//全部确认
		Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
		return;
	}
//Tp时间标签有效判定

//PW认证

	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识和参数数据长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
	if(DIlen<4)
	{
		goto err;
	}
//数据(启动站功能码10（请求1级数据）用于应用层请求确认（CON=1）的链路传输)
	DIaddr=((u32)p8rxbuff)+14;
	RXaddr=((u32)p8rxbuff)+14+4;
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
		}
		else
		{
			Fn=(i>>16);
			if((Fn!=32)&&(Fn!=37))
			{//不是读
				if(Comm_Ram->Factory!=0x55)//0x55=工厂状态
				{
					goto err;
				}
			}
			//Pn=i&0xffff;
			switch(Fn)//Fn
			{
				case 14://自测模式1(硬件测试)
					if(FileDownload->File&(1<<4))//文件标志(置位表示OK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=Font_ttf,B7=Module,...B31=
					{
						p32=(u32*)ADDR_Menu_ENTRY;
						if(p32[128]==8)
						{//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Test,5=Font,6=PACK,7=Router,8=TEST
							//全部确认
							Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
							DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
							Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
							BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
							while(1)
							{
								p32=(u32*)ADDR_MS_ENTRY;
								(*(void(*)())p32[115])();//Main_Thread; 115://主线程
								if(UARTCtrl->Task==0)
								{
									break;
								}
							}
							p32=(u32*)ADDR_Menu_ENTRY;
							(*(void(*)())p32[115])();//115://电气性能测试
							goto ok;
						}
					}
					goto err;
				case 15://自测模式2(FLASH读写测试)

					goto err;
				case 16://自测模式3(FLASH寿命测试启动)

					goto err;
				case 17://自测模式3(FLASH寿命测试继续)

					goto err;
				case 32://读自检信息
					MC(0,((u32)p8txbuff)+18,128);
					//4字节方案号
					MWR(Project,((u32)p8txbuff)+18,4);
					//4字节软件模块错误标志
					MR(((u32)p8txbuff)+18+4,(u32)&FileDownload->File,4);
//					//4字节IC等硬件错误标志
//					 //B8=MESAM 电能表用ESAM芯片(在硬件测试1 时测)
//					 //B9=TESAM 终端用ESAM芯片(在硬件测试1 时测)
//					i=MRR(ADDR_MeterError_IEEPROM_Start+64+28,4);
//					i&=0x300;
//					Comm_Ram->ErrorIC|=i;
//					MR(((u32)p8txbuff)+18+8,(u32)&Comm_Ram->ErrorIC,4);
//					//4字节IS61LV25616数据总线错误标志
//					MR(((u32)p8txbuff)+18+12,ADDR_MeterError_IEEPROM_Start+64+32,4);
//					//4字节IS61LV25616地址总线错误标志
//					MR(((u32)p8txbuff)+18+16,ADDR_MeterError_IEEPROM_Start+64+32+4,4);
//					//4字节SST39VF6401B数据总线错误标志
//					MR(((u32)p8txbuff)+18+20,ADDR_MeterError_IEEPROM_Start+64+32+8,4);
//					//4字节SST39VF6401B地址总线错误标志
//					MR(((u32)p8txbuff)+18+24,ADDR_MeterError_IEEPROM_Start+64+32+12,4);
//					//4字节MX29GLxxx数据总线错误标志
//					MR(((u32)p8txbuff)+18+28,ADDR_MeterError_IEEPROM_Start+64+32+16,4);
//					//4字节MX29GLxxx地址总线错误标志
//					MR(((u32)p8txbuff)+18+32,ADDR_MeterError_IEEPROM_Start+64+32+20,4);
//					//4字节输入输出自测标志:(置位表示错误,注:与IRAM中标志置位表示OK相反)
//					i=MRR(ADDR_MeterError_IEEPROM_Start+64+32+24,4);
//					i=~i;
//					MWR(i,((u32)p8txbuff)+18+36,4);
//					//4字节自测标志:(置位表示错误)
//					MR(((u32)p8txbuff)+18+40,ADDR_MeterError_IEEPROM_Start+64+32+28,4);
//				

					//测试总结论
					i=MRR(ADDR_Characteristics_BASE1+OFFSET_TestResult_Characteristics,1);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
					if(i)
					{
						MC(0xff,((u32)p8txbuff)+18+40,4);
					}
					
					i=4+128;
					i+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
					p8txbuff[1]=i;
					p8txbuff[2]=i>>8;
					Uart_376_NoAUX(PORTn);//没附加信息
					Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
					break;
				case 37://所有版本信息
					MC(0,((u32)p8txbuff)+18,256);
					//4*8字节I0版本
					i=MRR(ADDR_MS_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18,i,32);
					//4*8字节I1版本
					i=MRR(ADDR_OS_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18+32,i,32);
					//4*8字节I2版本
					i=MRR(ADDR_Meter_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18+64,i,32);
					//4*8字节E1版本
					i=MRR(ADDR_Terminal_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18+96,i,32);
					//4*8字节E2版本
					i=MRR(ADDR_Menu_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18+128,i,32);
					
					//
					i=4+256;
					i+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
					p8txbuff[1]=i;
					p8txbuff[2]=i>>8;
					Uart_376_NoAUX(PORTn);//没附加信息
					Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
					break;
				case 101://设置2字节行政地址、2字节终端地址、8字节设备编号,6字节MAC地址
					MW(RXaddr,ADDR_AREACODE,18);
					MW(RXaddr+4,ADDR_AFN0CF1+4,8);//终端版本信息中的设备编号
					MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//本终端实地址填入发送区
					goto ok;
				case 102://设置工厂模式定时时间(工厂模式有效分定时器hex)
		#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
					MW(RXaddr,ADDR_MeterFMTimer+14,2);
		#else
					MW(RXaddr,ADDR_TerminalFMTimer+14,2);
		#endif
					goto ok;
				default:
					goto err;
			}
		}
	}
}




