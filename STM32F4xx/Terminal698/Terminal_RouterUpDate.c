
#include "Project.h"
#include "Terminal_RouterUpDate.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"


#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=

void UpDateRouter(u32 PORTn)//路由软件更新
{
	if(Comm_Ram->RouterType!=5)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
	{
		UpDateRouter_Std(PORTn);//标准路由软件更新
	}
	else
	{
		UpDateRouter_01(PORTn);//瑞斯康路由软件更新
	}
}

void UpDateRouter_Std(u32 PORTn)//标准路由软件更新
{
/*
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 *p32;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p32=(u32*)(ADDR_E4_ENTRY);
	
	switch(Terminal_Router->UpDateTask)//软件更新任务
	{
		case 0://进入
			i=p32[68];//版本信息
			p32=(u32*)i;
			if(p32[5]!=Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
			{//路由器厂家错
				MC(0,ADDR_E4_ENTRY,4096);
				break;
			}
			if(UARTCtrl->Task<=2)
			{//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				Terminal_Router->UpDateCount=0;//软件更新帧计数
				Terminal_Router->UpDateReCount=0;//软件更新重试次数
				Terminal_Router->UpDateTask++;//软件更新任务
				WarningOccur(PORTn,(u8 *)"正在传输载波模块软件");//告警发生,入口告警代码和告警字符串
			}
			break;
		case 1://暂停路由器
			Terminal_Router->StopFlags=(1<<16);//暂停路由:b16=路由软件更新
			Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
			Terminal_Router->UpDateTask++;//软件更新任务
			break;
		case 2://发数据
			if(p32[64]!=ADDR_E4_ENTRY)
			{
				Terminal_Router->UpDateTask=0;//软件更新任务
				break;
			}
			Uart_3762_TxData(PORTn,0x15,1,0);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			z=Terminal_Router->MaxFileByte;//文件传输支持的最大单个数据包长度
			if(z>(256-13-11-2))
			{
				z=(256-13-11-2);
			}
			p8txbuff[13]=3;//文件标识	03H：本地通讯模块升级文件
			x=p32[63];
			y=p32[62];
			x-=y;
			y=(x+(z-1))/z;
			if((Terminal_Router->UpDateCount+1)<y)
			{
				p8txbuff[14]=0;//文件属性：起始帧、中间帧为00H；结束帧为01H
			}
			else
			{
				p8txbuff[14]=1;//文件属性：起始帧、中间帧为00H；结束帧为01H
			}
			p8txbuff[15]=0;//文件指令	00H：报文方式下装
			p8txbuff[16]=y;//总段数n	BIN	2
			p8txbuff[17]=y>>8;
			y=Terminal_Router->UpDateCount;//软件更新帧计数
			MWR(y,((u32)p8txbuff)+18,4);//第i段标识（i=0~n）	BIN	4
			//文件数据	BIN	Lf
			y*=z;//已发字节数
			if(x>=y)
			{
				i=x-y;
			}
			else
			{
				i=0;
			}
			if(i>z)
			{
				i=z;
			}
			if(i==0x0)
			{//发完
				UARTCtrl->Task=0;
				Terminal_Router->UpDateTask=8;//软件更新任务
				break;
			}
			MWR(i,((u32)p8txbuff)+22,2);//第i段数据长度Lf	BIN	2
			MR(((u32)p8txbuff)+24,p32[62]+y,i);
			//本帧总长度
			z=13+11+i+2;
			p8txbuff[1]=z;//总字节数
			p8txbuff[2]=z>>8;
			Uart_3762_CS((u32)p8txbuff);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
			UARTCtrl->TxByte=z;//发送字节数(半字对齐)
			UARTCtrl->Task=3;//等待发送
			Terminal_Router->UpDateTask++;//软件更新任务
			break;
		case 3://返回接收
			if(UARTCtrl->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				case 1://1=完成1帧发收
					if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1501)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
					{//返回收到当前段标识
						i=MRR(Get_ADDR_UARTnRx(PORTn)+13,4);
						if(i==Terminal_Router->UpDateCount)
						{//当检查发现文件错误时返回0xffff
							Terminal_Router->UpDateReCount=0;//软件更新重试次数
							Terminal_Router->UpDateCount++;//帧号+1
							Terminal_Router->UpDateTask--;//软件更新任务
							break;
						}
					}
				case 0://0=空闲
				case 2://2=接收超时(或错误)
					if(Terminal_Router->UpDateReCount<3)//软件更新重试次数
					{
						Terminal_Router->UpDateReCount++;//软件更新重试次数
						Terminal_Router->UpDateTask--;//软件更新任务
					}
					else
					{//失败
						WarningOccur(PORTn,(u8 *)"载波模块软件传输失败");//告警发生,入口告警代码和告警字符串
						Terminal_Ram->RouterCheck_S_Timer=5;//24 载波路由器死机秒定时器
						Terminal_Router->UpDateTask=9;//软件更新任务
					}
					break;
			}
			break;
		case 8://完成
			WarningOccur(PORTn,(u8 *)"载波模块正在更新软件");//告警发生,入口告警代码和告警字符串
			Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxUpDateTime;//升级操作最大等待时间秒
			Terminal_Router->UpDateTask++;//软件更新任务
			break;
		case 9://延时?S后复位
			if(Terminal_Ram->RouterCheck_S_Timer>60)//升级操作最大等待时间秒
			{
				Terminal_Ram->RouterCheck_S_Timer=60;//升级操作最大等待时间秒
			}
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 载波路由器死机秒定时器
			{
				MC(0,ADDR_E4_ENTRY,4096);
				Terminal_Router->UpDateTask=0;//软件更新任务
				Terminal_Router->RouterInitTask=0;//路由器初始化任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			break;
	}
*/
}


const u16 crc_ba[16] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
};

u32 UpDateRouter_01_CRC16(u32 Addr,u32 len)//瑞斯康路由软件更新CRC16
{
	u8 da;
	u16 crc;
	u8 *ptr;
	
	crc=0xffff;
	ptr=(u8*)Addr;
	while(len--!=0)
	{
		da = ((u8)(crc/256))/16;
		crc <<= 4;
		crc ^= crc_ba[da^(*ptr/16)];
		da = ((u8)(crc/256)/16);
		crc <<= 4;
		crc ^= crc_ba[da^(*ptr&0x0f)];
		ptr++;
	}
	return (crc ^ 0xffff);
}


void UpDateRouter_01(u32 PORTn)//瑞斯康路由软件更新
{
/*
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 *p32;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p32=(u32*)(ADDR_E4_ENTRY);
	
	switch(Terminal_Router->UpDateTask)//软件更新任务
	{
		case 0://进入
			i=p32[68];//版本信息
			p32=(u32*)i;
			if(p32[5]!=Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
			{//路由器厂家错
				MC(0,ADDR_E4_ENTRY,4096);
				break;
			}
			if(UARTCtrl->Task<=2)
			{//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
				Terminal_Router->UpDateCount=0;//软件更新帧计数
				Terminal_Router->UpDateReCount=0;//软件更新重试次数
				Terminal_Router->UpDateTask++;//软件更新任务
				WarningOccur(PORTn,(u8 *)"正在传输载波模块软件");//告警发生,入口告警代码和告警字符串
			}
			break;
		case 1://暂停路由器
			Terminal_Router->StopFlags=(1<<16);//暂停路由:b16=路由软件更新
			Terminal_Router->RouterCtrl=2;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
			Terminal_Router->UpDateTask++;//软件更新任务
			break;
		case 2://先利用文件标识为00H 的文件传输帧通知本地通信模块清除存储的已下载文件
			Uart_3762_TxData(PORTn,0x15,1,0);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			p8txbuff[13]=0;//文件标识	00H
			p8txbuff[14]=1;//文件属性：起始帧、中间帧为00H；结束帧为01H
			p8txbuff[15]=0;//文件指令	00H：报文方式下装
			p8txbuff[16]=1;//总段数n	BIN	2
			p8txbuff[17]=0;
			MWR(0,((u32)p8txbuff)+18,4);//第i段标识（i=0~n）	BIN	4
			MWR(130,((u32)p8txbuff)+22,2);//第i段数据长度Lf	BIN	2
			
			z=UpDateRouter_01_CRC16(((u32)p8txbuff)+24,128);//瑞斯康路由软件更新CRC16
			MWR(z,((u32)p8txbuff)+24+128,2);
			
			//本帧总长度
			z=13+11+130+2;
			p8txbuff[1]=z;//总字节数
			p8txbuff[2]=z>>8;
			Uart_3762_CS((u32)p8txbuff);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
			UARTCtrl->TxByte=z;//发送字节数(半字对齐)
			UARTCtrl->Task=3;//等待发送
			Terminal_Router->UpDateTask++;//软件更新任务
			break;
		case 3:
			if(UARTCtrl->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				case 1://1=完成1帧发收
					if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1501)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
					{//返回收到当前段标识
						i=MRR(Get_ADDR_UARTnRx(PORTn)+13,4);
						if(i!=0xffff)
						{//当检查发现文件错误时返回0xffff
							Terminal_Router->UpDateReCount=0;//软件更新重试次数
							Terminal_Router->UpDateTask++;//软件更新任务
							break;
						}
					}
				case 0://0=空闲
				case 2://2=接收超时(或错误)
					if(Terminal_Router->UpDateReCount<3)//软件更新重试次数
					{
						Terminal_Router->UpDateReCount++;//软件更新重试次数
						Terminal_Router->UpDateTask--;//软件更新任务
					}
					else
					{//失败
						WarningOccur(PORTn,(u8 *)"载波模块软件传输失败");//告警发生,入口告警代码和告警字符串
						Terminal_Ram->RouterCheck_S_Timer=5;//24 载波路由器死机秒定时器
						Terminal_Router->UpDateTask=9;//软件更新任务
					}
					break;
			}
			break;
		case 4://发数据
			if(p32[64]!=ADDR_E4_ENTRY)
			{
				Terminal_Router->UpDateTask=0;//软件更新任务
				break;
			}
			Uart_3762_TxData(PORTn,0x15,1,0);//376.2发送数据填入发送区,入口时若有信息域目的地址数据单元等则已填入,数据单元暂固定填入发送缓冲区+128地址
			z=128;//本地通信模块升级文件的每帧段数据长度Lf 规定为128+2字节
			p8txbuff[13]=3;//文件标识	03H：本地通讯模块升级文件
			x=p32[63];
			y=p32[62];
			x-=y;
			y=(x+(z-1))/z;
			if((Terminal_Router->UpDateCount+1)<y)
			{
				p8txbuff[14]=0;//文件属性：起始帧、中间帧为00H；结束帧为01H
			}
			else
			{
				p8txbuff[14]=1;//文件属性：起始帧、中间帧为00H；结束帧为01H
			}
			p8txbuff[15]=0;//文件指令	00H：报文方式下装
			p8txbuff[16]=y;//总段数n	BIN	2
			p8txbuff[17]=y>>8;
			y=Terminal_Router->UpDateCount;//软件更新帧计数
			MWR(y,((u32)p8txbuff)+18,4);//第i段标识（i=0~n）	BIN	4
			//文件数据	BIN	Lf
			y*=z;//已发字节数
			if(x>=y)
			{
				i=x-y;
			}
			else
			{
				i=0;
			}
			if(i>z)
			{
				i=z;
			}
			if(i==0x0)
			{//发完
				UARTCtrl->Task=0;
				Terminal_Router->UpDateTask=8;//软件更新任务
				break;
			}
			MWR(130,((u32)p8txbuff)+22,2);//第i段数据长度Lf	BIN	2
			MR(((u32)p8txbuff)+24,p32[62]+y,i);
			
			z=UpDateRouter_01_CRC16(((u32)p8txbuff)+24,128);//瑞斯康路由软件更新CRC16
			MWR(z,((u32)p8txbuff)+24+128,2);
			
			//本帧总长度
			z=13+11+130+2;
			p8txbuff[1]=z;//总字节数
			p8txbuff[2]=z>>8;
			Uart_3762_CS((u32)p8txbuff);//和校验计算并写入和比较,返回0=正确1=错误,入口接收或发送缓冲
			UARTCtrl->TxByte=z;//发送字节数(半字对齐)
			UARTCtrl->Task=3;//等待发送
			Terminal_Router->UpDateTask++;//软件更新任务
			break;
		case 5://返回接收
			if(UARTCtrl->Task>=3)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				break;
			}
			switch(UARTCtrl->Task)//UART_3762接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
			{
				case 1://1=完成1帧发收
					if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1501)//UART_3762接收数据得到AFN,Fn;B15-B8=AFN,B7-B0=Fn
					{//返回收到当前段标识
						i=MRR(Get_ADDR_UARTnRx(PORTn)+13,4);
						if(i==Terminal_Router->UpDateCount)
						{//当检查发现文件错误时返回0xffff
							Terminal_Router->UpDateReCount=0;//软件更新重试次数
							Terminal_Router->UpDateCount++;//帧号+1
							Terminal_Router->UpDateTask--;//软件更新任务
							break;
						}
					}
				case 0://0=空闲
				case 2://2=接收超时(或错误)
					if(Terminal_Router->UpDateReCount<3)//软件更新重试次数
					{
						Terminal_Router->UpDateReCount++;//软件更新重试次数
						Terminal_Router->UpDateTask--;//软件更新任务
					}
					else
					{//失败
						WarningOccur(PORTn,(u8 *)"载波模块软件传输失败");//告警发生,入口告警代码和告警字符串
						Terminal_Ram->RouterCheck_S_Timer=5;//24 载波路由器死机秒定时器
						Terminal_Router->UpDateTask=9;//软件更新任务
					}
					break;
			}
			break;
		case 8://完成
			WarningOccur(PORTn,(u8 *)"载波模块正在更新软件");//告警发生,入口告警代码和告警字符串
			Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxUpDateTime;//升级操作最大等待时间秒
			Terminal_Router->UpDateTask++;//软件更新任务
			break;
		case 9://延时?S后复位
			if(Terminal_Ram->RouterCheck_S_Timer>240)//升级操作最大等待时间秒
			{
				Terminal_Ram->RouterCheck_S_Timer=240;//升级操作最大等待时间秒
			}
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 载波路由器死机秒定时器
			{
				MC(0,ADDR_E4_ENTRY,4096);
				Terminal_Router->UpDateTask=0;//软件更新任务
				Terminal_Router->RouterInitTask=0;//路由器初始化任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			break;
	}
*/
}

#endif

//0x68
//总长度L
//总长度L
//控制码
//信息域R-1
//信息域R-2
//信息域R-3
//信息域R-4
//信息域R-5
//信息域R-6
//地址域A1,A2,A3
//应用功能码AFN
//数据单元标识DT1
//数据单元标识DT2
//数据单元
//CS
//0x16
