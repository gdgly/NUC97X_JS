
//终端本机信息显示
#include "../Hcsg/ProjectCSG.h"

#include "../Display/DisplayLoop_Info.h"
#include "../Display/Display.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"





u32 DisplayLoop_Info(u32 Row)//终端信息显示,返回下行
{
/*
	u32 i;
	u32 x;
	u32 data;
	u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
	
	switch(TerminalDisplayLoop->Count)
	{
		case 0://主站IP、端口号、心跳周期
			DisplayMenuString(Row,0,2,(u8 *)
																			"主站通信参数:\n\r"
																			"主IP=\n\r"
																			"主端口号=\n\r"
																			"备IP=\n\r"
																			"备端口号=\n\r"
																			"心跳周期 =\n\r"
																			"当前链接 =\n\r"
																			);//显示菜单字符串
			//主IP
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*5));
			for(i=0;i<4;i++)
			{
				data=MRR(ADDR_AFN04F3+i,1);
				data=hex_bcd(data);
				p16=DisplayData_OffZero(data,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				p16[0]='.'+0x2000;
				p16++;
			}
			p16--;
			p16[0]=0;
			//主端口号
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
			data=MRR(ADDR_AFN04F3+4,2);
			data=hex_bcd(data);
			DisplayData_OffZero(data,5,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			//备IP
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*5));
			for(i=0;i<4;i++)
			{
				data=MRR(ADDR_AFN04F3+6+i,1);
				data=hex_bcd(data);
				p16=DisplayData_OffZero(data,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
				p16[0]='.'+0x2000;
				p16++;
			}
			p16--;
			p16[0]=0;
			//备端口号
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
			data=MRR(ADDR_AFN04F3+10,2);
			data=hex_bcd(data);
			DisplayData_OffZero(data,5,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			//心跳周期
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			data=MRR(ADDR_AFN04F1+5,1);
			data=hex_bcd(data);
			DisplayData_OffZero(data,3,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			//当前链接
			Row++;
			i=0;
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
			if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			{
				if(UARTCtrl->LinkCount<=2)
				{//主ip
					i=1;
				}
				else
				{//备用ip
					i=2;
				}
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			if(UARTCtrl->LinkTask==101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
			{
				if(UARTCtrl->LinkCount<2)
				{//主ip
					i=1;
				}
				else
				{//备用ip
					i=2;
				}
			}
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			switch(i)
			{
				case 0:
					DispalyStringContinue(p16,(u8 *)"无");//显示字符串继续
					break;
				case 1:
					DispalyStringContinue(p16,(u8 *)"主IP");//显示字符串继续
					break;
				case 2:
					DispalyStringContinue(p16,(u8 *)"备IP");//显示字符串继续
					break;
			}
			break;
		case 1://APN、虚拟专网用户名、密码
			DisplayMenuString(Row,0,2,(u8 *)
																			"无线通信参数:\n\r"
																			"APN=\n\r"
																			" \n\r"
																			"用户名=\n\r"
																			" \n\r"
																			"密码=\n\r"
																			);//显示菜单字符串
			//APN
			Row++;
			MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
			MC(0,ADDR_DATABUFF+16,1);
			DisplayString(Row,4,0,(u8 *)ADDR_DATABUFF);//显示字符串
			//虚拟专网用户名
			Row+=2;
			MR(ADDR_DATABUFF,ADDR_AFN04F16,32);
			MC(0,ADDR_DATABUFF+32,1);
			DisplayString(Row,7,0,(u8 *)ADDR_DATABUFF);//显示字符串
			//密码
			Row+=2;
			MR(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
			MC(0,ADDR_DATABUFF+32,1);
			DisplayString(Row,6,0,(u8 *)ADDR_DATABUFF);//显示字符串
			break;
		case 2://主站手机号码、短信中心号码
			DisplayMenuString(Row,0,2,(u8 *)
																			"主站通信号码:\n\r"
																			"电话\n\r"
																			"短信\n\r"
																			"本机通信号码:\n\r"
																			"电话\n\r"
																			"IMSI\n\r"
																			);//显示菜单字符串
			//主站手机号码
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*4));
			for(i=0;i<8;i++)
			{
				data=MRR(ADDR_AFN04F4+i,1);
				x=data>>4;
				if(x==0xf)
				{
					break;
				}
				if(x==0xa)
				{
					p16[0]=','+0x2000;
				}
				else
				{
					if(x==0xb)
					{
						p16[0]='#'+0x2000;
					}
					else
					{
						p16[0]=x+0x30+0x2000;
					}
				}
				p16++;
				x=data&0xf;
				if(x==0xf)
				{
					break;
				}
				if(x==0xa)
				{
					p16[0]=','+0x2000;
				}
				else
				{
					if(x==0xb)
					{
						p16[0]='#'+0x2000;
					}
					else
					{
						p16[0]=x+0x30+0x2000;
					}
				}
				p16++;
			}
			//短信中心号码
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*4));
			for(i=0;i<8;i++)
			{
				data=MRR(ADDR_AFN04F4+8+i,1);
				x=data>>4;
				if(x==0xf)
				{
					break;
				}
				if(x==0xa)
				{
					p16[0]=','+0x2000;
				}
				else
				{
					if(x==0xb)
					{
						p16[0]='#'+0x2000;
					}
					else
					{
						p16[0]=x+0x30+0x2000;
					}
				}
				p16++;
				x=data&0xf;
				if(x==0xf)
				{
					break;
				}
				if(x==0xa)
				{
					p16[0]=','+0x2000;
				}
				else
				{
					if(x==0xb)
					{
						p16[0]='#'+0x2000;
					}
					else
					{
						p16[0]=x+0x30+0x2000;
					}
				}
				p16++;
			}
			//本机号码
			Row+=2;
			DispalyStringOnlyOneRow(Row,4,(u8*)ADDR_MobilePhoneNumber);//显示字符串仅在1行内
			//本机IMSI
			Row++;
			DisplayString(Row,5,0,(u8 *)ADDR_MobilePhoneNumber+20);//显示字符串
			break;
		case 3://行政区码、逻辑地址、厂商代码、设备编号、软硬件版本号
			DisplayMenuString(Row,0,2,(u8 *)
																			"终端信息:\n\r"
																			"行政区码 = \n\r"
																			"逻辑地址 = \n\r"
																			"厂商代码 = \n\r"
																			"设备编号 = \n\r"
																			"软件版本 = \n\r"
																			"硬件版本 = \n\r"
																			);//显示菜单字符串
			//行政区码
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			data=MRR(ADDR_AREACODE,2);
			DisplayData(data,4,0,p16);//数据显示,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			//逻辑地址
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
#if ((USER/100)==11)//吉林集中器
			if(Comm_Ram->Factory==0x55)
			{//工厂状态
				data=MRR(ADDR_TERMINAL,2);
				data=hex_bcd(data);
				DisplayData_OffZero(data,5,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			}
			else
			{
				data=MRR(ADDR_04000401,6);
				DisplayData_OffZero(data,8,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
			}
#else
			data=MRR(ADDR_TERMINAL,2);
			data=hex_bcd(data);
			DisplayData_OffZero(data,5,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
#endif
			//厂商代码
			Row++;
			MR(ADDR_DATABUFF,ADDR_AFN0CF1,4);
			MC(0,ADDR_DATABUFF+4,1);
			DisplayString(Row,11,0,(u8 *)ADDR_DATABUFF);//显示字符串
			//设备编号
			Row++;
			MR(ADDR_DATABUFF,ADDR_AFN0CF1+4,8);
			MC(0,ADDR_DATABUFF+8,1);
			DisplayString(Row,11,0,(u8 *)ADDR_DATABUFF);//显示字符串
			//软件版本
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
		#if (USER/100)==5//上海
			p16[0]='Z'+0x2000;
			p16[1]='Z'+0x2000;
			p16[2]=((SVER>>4)&0xf)+0x2030;
			p16[3]=((SVER>>0)&0xf)+0x2030;
		#else
			p16[0]='V'+0x2000;
			p16[1]=((SVER>>8)&0xf)+0x30+0x2000;
			p16[2]='.'+0x2000;
			p16[3]=((SVER>>4)&0xf)+0x30+0x2000;
			//p16[4]=((SVER>>0)&0xf)+0x30+0x2000;
		#endif
			//硬件版本
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			p16[0]='V'+0x2000;
			p16[1]=((HVER>>8)&0xf)+0x30+0x2000;
			p16[2]='.'+0x2000;
			p16[3]=((HVER>>4)&0xf)+0x30+0x2000;
			//p16[4]=((HVER>>0)&0xf)+0x30+0x2000;
			break;
//		case 4://
//			break;
	}
*/
	return Row;
}










