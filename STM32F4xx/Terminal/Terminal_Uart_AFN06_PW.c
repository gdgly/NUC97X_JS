
//身份认证及密钥协商
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"





u32 Uart_376_PW(u32 PORTn)//消息认证码字段PW;出口0有效,1无效,4已返错误帧
{
	u32 i;
	u32 mac;
	u32 DataLen;
	u32 p1;
	u32 p2;
	u64 PWL;
	u64 PWH;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	
	if(UART7Ctrl->Lock==0x55)//0x55=通信功能正在被调用禁止其他程序再次调用
	{
		return 1;
	}
	if(Comm_Ram->POWER<3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
  {
		return 1;
	}
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	i=MRR(ADDR_AFN04F5,1);//终端上行通信消息认证参数
	switch(i)//消息认证方案号
	{
		default://表示不认证
			return 0;
		case 255://255表示专用硬件认证方案
//MAC运算过程的明文数据包括:AFN,SEQ,数据单元标识,数据单元,及附加域中的时间标签(注：376测试软件时间标签不参与计算)
			//计算PW地址取MAC寄存
			DataLen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if((p8rxbuff[13]&0x80)==0x0)
			{//无Tp
				i=((u32)p8rxbuff)+6+DataLen-16;
			}
			else
			{//有Tp
				i=((u32)p8rxbuff)+6+DataLen-(16+6);
			}
			mac=MRR(i,4);//读存贮器数据到寄存器,Byte<=8
			PWL=MRR(i,8);//读存贮器数据到寄存器,Byte<=8
			PWH=MRR(i+8,8);//读存贮器数据到寄存器,Byte<=8

//			//若有Tp,将6字节Tp移到PW区以连接源数据便于MAC计算时发送
//			if((p8rxbuff[13]&0x80)!=0x0)
//			{//有Tp
//				MEMRead(i,i+16,6);
//			}

			//参于MAC计算的明文数据长度
			if((p8rxbuff[13]&0x80)==0x0)
			{//无Tp
				DataLen-=6+16;//从AFN开始,但16字节PW不参加
			}
			else
			{//有Tp
				DataLen-=6+16+6;//从AFN开始,但16字节PW和时间标签不参加(注：376测试软件时间标签不参与计算)
			}
			//P1和P2的所指示的密钥KID
			if((p8rxbuff[11]&0x1)==0x0)
			{//单地址
				switch(p8rxbuff[12])
				{
					case 0x01:
						p2=0x20;
						break;
					case 0x04:
						p2=0x21;
						break;
					case 0x05:
						p2=0x22;
						break;
					case 0x10:
						p2=0x23;
						break;
				#if (USER/100)==0//电科院测试
					case 0x0F:
						p2=0x24;
						break;
				#endif
					default:
						return 0;
				}
				if(MRR(ADDR_TerminalRealTimer+2,2)==0)//密钥失效定时24小时
				{//回否认帧F4,签名校验错误
					p8txbuff[12]=0;//AFN=0
					p8txbuff[14]=0;
					p8txbuff[15]=0;
					p8txbuff[16]=8;//F4
					p8txbuff[17]=0;
					p8txbuff[18]=1;//错误类型:1=签名校验错,2=密文校验错,3=对称MAC验证失败
					MC(0xff,((u32)p8txbuff)+19,16);
					p8txbuff[1]=12+17;//数据长度
					p8txbuff[2]=0;
					p8txbuff[6]=0x80;//控制码DIR=1上行,PRM=0表示此帧报文来自从动站,要求访问位ACD暂0,功能码=0
					i=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息(Tp)
					Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位			
					return 4;
				}
				i=TESAM_CMD_TxRx(0x80FA0300+p2,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto MACErr;
				}
				i=TESAM_CMD_TxRx(0x80FA0200,DataLen,((u32)p8rxbuff)+12);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto MACErr;
				}
				Data_Inverse(ADDR_UART7Rx+5,4);//数据倒序(高低字节调换),入口Le>=2
				i=MRR(ADDR_UART7Rx+5,4);//读存贮器数据到寄存器,Byte<=8
				if(i==mac)
				{
					return 0;
				}
			}
			else
			{//组地址
				//8022+KID1+KID2+LC+(00+TotalTN+130202224622+Data)+ 20130202224622+ 05
				i=p8rxbuff[9]+(p8rxbuff[10]<<8);//组地址
				if(i==0xffff)
				{//广播地址
					switch(p8rxbuff[12])
					{
						case 0x01:
							p1=0x10;//KID1
							p2=0x1a;//KID2
							break;
						case 0x04:
							p1=0x11;//KID1
							p2=0x1b;//KID2
							break;
						case 0x05:
							p1=0x12;//KID1
							p2=0x1c;//KID2
							break;
						case 0x10:
							p1=0x13;//KID1
							p2=0x1d;//KID2
							break;
					#if (USER/100)==0//电科院测试
						case 0x0F:
							p1=0x15;//KID1
							p2=0x1F;//KID2
							break;
					#endif
						default:
							return 0;
					}
					if(MRR(ADDR_TerminalRealTimer+2,2)==0)//密钥失效定时24小时
					{//回否认帧F4,签名校验错误
						p8txbuff[12]=0;//AFN=0
						p8txbuff[14]=0;
						p8txbuff[15]=0;
						p8txbuff[16]=8;//F4
						p8txbuff[17]=0;
						p8txbuff[18]=1;//错误类型:1=签名校验错,2=密文校验错,3=对称MAC验证失败
						MC(0xff,((u32)p8txbuff)+19,16);
						p8txbuff[1]=12+17;//数据长度
						p8txbuff[2]=0;
						p8txbuff[6]=0x80;//控制码DIR=1上行,PRM=0表示此帧报文来自从动站,要求访问位ACD暂0,功能码=0
						i=p8rxbuff[1]+(p8rxbuff[2]<<8);
						Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息(Tp)
						Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位			
						return 4;
					}
				}
				else
				{//组地址
					switch(p8rxbuff[12])
					{
						case 0x01:
							p1=0x0c;//KID1
							p2=0x16;//KID2
							break;
						case 0x04:
							p1=0x0d;//KID1
							p2=0x17;//KID2
							break;
						case 0x05:
							p1=0x0e;//KID1
							p2=0x18;//KID2
							break;
						case 0x10:
							p1=0x0f;//KID1
							p2=0x19;//KID2
							break;
					#if (USER/100)==0//电科院测试
						case 0x0F:
							p1=0x14;//KID1
							p2=0x1E;//KID2
							break;
					#endif
						default:
							return 0;
					}
					if(MRR(ADDR_TerminalRealTimer+2,2)==0)//密钥失效定时24小时
					{//回否认帧F4,签名校验错误
						p8txbuff[12]=0;//AFN=0
						p8txbuff[14]=0;
						p8txbuff[15]=0;
						p8txbuff[16]=8;//F4
						p8txbuff[17]=0;
						p8txbuff[18]=1;//错误类型:1=签名校验错,2=密文校验错,3=对称MAC验证失败
						MC(0xff,((u32)p8txbuff)+19,16);
						p8txbuff[1]=12+17;//数据长度
						p8txbuff[2]=0;
						p8txbuff[6]=0x80;//控制码DIR=1上行,PRM=0表示此帧报文来自从动站,要求访问位ACD暂0,功能码=0
						i=p8rxbuff[1]+(p8rxbuff[2]<<8);
						Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息(Tp)
						Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位			
						return 4;
					}
				}
				//MAC倒序
				DataLen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				if((p8rxbuff[13]&0x80)==0x0)
				{//无Tp
					i=((u32)p8rxbuff)+6+DataLen-16;
				}
				else
				{//有Tp
					i=((u32)p8rxbuff)+6+DataLen-(16+6);
				}
				Data_Inverse(i,4);//数据倒序(高低字节调换),入口Le>=2
				//参于MAC计算的明文数据长度
				if((p8rxbuff[13]&0x80)==0x0)
				{//无Tp
					DataLen-=6+16;//从AFN开始,但16字节PW不参加
				}
				else
				{//有Tp
					DataLen-=6+16+6;//从AFN开始,但16字节PW和时间标签不参加(注：376测试软件时间标签不参与计算)
				}
				
				p8rxbuff[3]=0;//广播数据类型；固定为0：明文+MAC
				p8rxbuff[4]=p8rxbuff[10];//组地址高字节
				p8rxbuff[5]=p8rxbuff[9];//组地址低字节
				p8rxbuff[6]=0x13;//主站时间；6字节；默认130202224622
				p8rxbuff[7]=0x02;
				p8rxbuff[8]=0x02;
				p8rxbuff[9]=0x22;
				p8rxbuff[10]=0x46;
				p8rxbuff[11]=0x22;
				//12 1 应用层功能码AFN
				//13 1 帧序列域SEQ
				//14 4 数据单元标识
				//18 n 数据
				//	 4 MAC
				p8rxbuff[12+DataLen+4+0]=0x20;
				p8rxbuff[12+DataLen+4+1]=0x13;
				p8rxbuff[12+DataLen+4+2]=0x02;
				p8rxbuff[12+DataLen+4+3]=0x02;
				p8rxbuff[12+DataLen+4+4]=0x22;
				p8rxbuff[12+DataLen+4+5]=0x46;
				p8rxbuff[12+DataLen+4+6]=0x22;
				p8rxbuff[12+DataLen+4+7]=0x05;
				
				i=TESAM_CMD_TxRx(0x80220000+(p1<<8)+p2,9+DataLen+4+8,((u32)p8rxbuff)+3);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto MACErr;
				}
				return 0;
			}
		MACErr://错误
			//ERC20事件记录
			i=p8rxbuff[11]>>1;//启动站地址
			ERC20_Event(PWL,PWH,i);//消息认证错误记录,入口:PWH_PWL消息认证码,MSA=启动站地址
//			//取随机数
//			TESAM_CMD_TxRx(0x801A0800,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
//			MR(ADDR_TESAM_User+8,ADDR_UART7Rx+5,4);
			//返回帧
			p8txbuff[12]=0;//AFN=0
			p8txbuff[14]=0;
			p8txbuff[15]=0;
			p8txbuff[16]=8;//F4
			p8txbuff[17]=0;
			p8txbuff[18]=3;//错误类型:1=签名校验错,2=密文校验错,3=对称MAC验证失败
				
			MC(0xff,((u32)p8txbuff)+19,8);//数据体低8字节的当前终端随机数为8字节0xFF
			MR(((u32)p8txbuff)+19+8,ADDR_TESAM_User,8);//高8字节为芯片序列号
			Data_Inverse(((u32)p8txbuff)+19,8);//数据倒序(高低字节调换),入口Le>=2
			Data_Inverse(((u32)p8txbuff)+19+8,8);//数据倒序(高低字节调换),入口Le>=2
			p8txbuff[1]=12+17;//数据长度
			p8txbuff[2]=0;
			p8txbuff[6]=0x80;//控制码DIR=1上行,PRM=0表示此帧报文来自从动站,要求访问位ACD暂0,功能码=0
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+i-6);//附加信息(Tp)
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return 4;
	}
}

/*
帧格式
0 1 0x68
1 2 L(用户域长度,接收时去了D1D0表示的协议号即左移2位）
3 2 L(用户域长度）
5 1 0x68
6 1 控制域C
7 2 行政区划码A1
9 2 终端地址A2(终端地址A2选址范围为1～65535。A2=0000H为无效地址，A2=FFFFH且A3的D0位为"1"时表示系统广播地址)
11 1 主站地址和组地址标志A3(A3的D0位为终端组地址标志，D0=0表示终端地址A2为单地址；D0=1表示终端地址A2为组地址；A3的D1～D7组成0～127个主站地址MSA)
12 1 应用层功能码AFN
13 1 帧序列域SEQ
14 4 数据单元标识
18 n 数据单元
...
PW 16
Tp 6
CS 1
0x16 1
*/


void Uart_AFN06(u32 PORTn)//身份认证及密钥协商
{
	u32 i;
	u32 x;
	u32 DIlen;
	u32 DIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 Fn;
	u32 p1;
//	u32 ErrType;//错误类型:1=签名校验错,2=密文校验错,3=对称MAC验证失败
  
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 *p8esamrxbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(UART7Ctrl->Lock==0x55)//0x55=通信功能正在被调用禁止其他程序再次调用
	{
		return;
	}
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8esamrxbuff=(u8 *)(ADDR_UART7Rx);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<(12+16))
	{
err:
		//全部否认
		Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息(Tp)
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		return;
	}
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+16+6))
		{
			goto err;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}

	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识和参数数据长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+16+6);
	}
	else
	{
		DIlen-=(8+16);
	}
	if(DIlen<4)
	{
		goto err;
	}
	DIaddr=((u32)p8rxbuff)+14;
	TXaddr=((u32)p8txbuff)+14+4;
	TXlen=0;

	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	if(DIlen<4)
	{
		goto err;
	}
	else
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		Fn=(i>>16);
		switch(Fn)
		{
			case 14://F14	对称密钥更新
			case 15://F15	终端证书更新(请求)
			case 16://F16	CA证书更新
			//case 17://F17	内部认证
			//case 18://F18	外部认证
			//case 19://F19	状态切换
			case 20://F20	置离线计数器
			case 21://F21	转加密授权
				if(MRR(ADDR_TerminalRealTimer+2,2)==0)//密钥失效定时24小时
				{//回否认帧F4,签名校验错误
				err_F4:
					p8txbuff[12]=0;//AFN=0
					p8txbuff[14]=0;
					p8txbuff[15]=0;
					p8txbuff[16]=8;//F4
					p8txbuff[17]=0;
					p8txbuff[18]=1;//错误类型:1=签名校验错,2=密文校验错,3=对称MAC验证失败
					MC(0xff,((u32)p8txbuff)+19,16);
					p8txbuff[1]=12+17;//数据长度
					p8txbuff[2]=0;
					p8txbuff[6]=0x80;//控制码DIR=1上行,PRM=0表示此帧报文来自从动站,要求访问位ACD暂0,功能码=0
					DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息(Tp)
					Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
					return;
				}
				break;
		}
		switch(Fn)
		{
			case 11://F11	获取终端信息
				//获取芯片状态信息1字节
				i=TESAM_CMD_TxRx(0x800E0005,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					UART7Ctrl->Task=0;//关电源后重启
					goto err;
				}
				p1=MRR(ADDR_UART7Rx+5,1);
				//获取芯片序列号8字节
				i=TESAM_CMD_TxRx(0x800E0002,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(ADDR_TESAM_User,ADDR_UART7Rx+5,8);
				MR(TXaddr,ADDR_UART7Rx+5,8);
				Data_Inverse(TXaddr,8);//数据倒序(高低字节调换),入口Le>=2
				//获取证书序列号：16字节
				i=TESAM_CMD_TxRx(0x80320002+(p1<<8),0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(TXaddr+8,ADDR_UART7Rx+5,16);
				Data_Inverse(TXaddr+8,16);//数据倒序(高低字节调换),入口Le>=2
				//获取离线计数器4字节
				i=TESAM_CMD_TxRx(0x800E0003,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(TXaddr+8+16,ADDR_UART7Rx+5,4);
				Data_Inverse(TXaddr+8+16,4);//数据倒序(高低字节调换),入口Le>=2
				MWR(p1,TXaddr+8+16+4,1);//芯片状态信息1字节
				//获取密钥版本：8字节
				TESAM_CMD_TxRx(0x800E0006,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(TXaddr+8+16+4+1,ADDR_UART7Rx+5,8);
				Data_Inverse(TXaddr+8+16+4+1,8);//数据倒序(高低字节调换),入口Le>=2
				TXlen=8+16+4+1+8;
			ACKDATA:
				TXlen+=12;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ+4字节标识=12字节
				p8txbuff[1]=TXlen;
				p8txbuff[2]=TXlen>>8;
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息(Tp)
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				return;
			case 12://F12	会话初始化/恢复
				if(p8rxbuff[18+3]==1)
				{//会话恢复
					if(MRR(ADDR_TerminalRealTimer+2,2)==0)//密钥失效定时24小时
					{//回否认帧F4,签名校验错误
						goto err_F4;
					}
				}
				i=MRR(((u32)p8rxbuff)+18,2);//报文1长度
				if((18+2+i+16+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				//报文1长度=Ln+86
				if(i<86)
				{
					goto err;
				}
				p1=i-86;//主站证书长度
				Data_Inverse(((u32)p8rxbuff)+18+4,p1);//主站证书
				Data_Inverse(((u32)p8rxbuff)+18+4+p1,16);//EKS1
				Data_Inverse(((u32)p8rxbuff)+18+4+p1+16,4);//MAC1
				Data_Inverse(((u32)p8rxbuff)+18+4+p1+16+4,64);//签名数据S1
				MR(((u32)p8rxbuff)+11,((u32)p8rxbuff)+18+2,i);
				//报文1+YYYYMMDDhhmmss
				p8rxbuff[11+i]=0x20;
				MR(((u32)p8rxbuff)+11+i+1,ADDR_TYMDHMS,6);
				Data_Inverse(((u32)p8rxbuff)+11+i+1,6);//数据倒序(高低字节调换),入口Le>=2
				i=TESAM_CMD_TxRx(0x84100000,i+7,((u32)p8rxbuff)+11);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				i=(p8esamrxbuff[3]<<8)+p8esamrxbuff[4];//返回数据长度
				if((18+2+i+8+2)>Get_LEN_UARTnTx(PORTn))
				{
					goto err;
				}
				if(p8esamrxbuff[6]==0)//会话ID:0=初始化,1=恢复
				{//会话初始化
					//Version(1字节)+Session ID(1字节，00)+CRL序列号(16字节)+主站证书C1序列号(16字节)+终端证书C2(Var)+ Eks2(R2) (16字节)+数字签名S2(64字节)
					if(i<144)
					{
						goto err;
					}
					p1=i-114;//返回报文2长度=Ln+114
					MWR(i,((u32)p8txbuff)+18,2);
					MR(((u32)p8txbuff)+18+2,((u32)p8esamrxbuff)+5,i);
					//1字节版本号
					//1字节会话ID
					Data_Inverse(((u32)p8txbuff)+18+4,16);//CRL序列号
					Data_Inverse(((u32)p8txbuff)+18+4+16,16);//主站序列号
					Data_Inverse(((u32)p8txbuff)+18+4+16+16,p1);//终端证书
					Data_Inverse(((u32)p8txbuff)+18+4+16+16+p1,16);//EKS2
					Data_Inverse(((u32)p8txbuff)+18+4+16+16+p1+16,64);//签名数据S2
					TXlen=2+i;
				}
				else
				{//会话恢复
					//Version(1字节)+Session ID(1字节)+ Eks2(R2) (16字节)+R3(16字节)+MAC2(4字节)
					MWR(38,((u32)p8txbuff)+18,2);
					MR(((u32)p8txbuff)+18+2,((u32)p8esamrxbuff)+5,i);
					//1字节版本号
					//1字节会话ID
					Data_Inverse(((u32)p8txbuff)+18+4,16);//Eks2(R2)
					Data_Inverse(((u32)p8txbuff)+18+4+16,16);//R3
					Data_Inverse(((u32)p8txbuff)+18+4+16+16,4);//MAC2
					TXlen=2+38;
					MWR(24*60,ADDR_TerminalRealTimer+2,2);//密钥失效定时24小时
				}
				goto ACKDATA;//上传报文2
			case 13://F13	会话协商
				i=MRR(((u32)p8rxbuff)+18,2);//报文3长度
				if((18+2+i+16+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				//报文3长度=Ln+181
				if(i<181)
				{
					goto err;
				}
				p1=i-181;//主站证书验证码长度
				Data_Inverse(((u32)p8rxbuff)+18+2,113);//数据倒序(高低字节调换),入口Le>=2
				Data_Inverse(((u32)p8rxbuff)+18+2+113,p1);//数据倒序(高低字节调换),入口Le>=2
				Data_Inverse(((u32)p8rxbuff)+18+2+113+p1,4);//数据倒序(高低字节调换),入口Le>=2
				Data_Inverse(((u32)p8rxbuff)+18+2+113+p1+4,64);//数据倒序(高低字节调换),入口Le>=2
				i=TESAM_CMD_TxRx(0x84120000,i,((u32)p8rxbuff)+18+2);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				i=(p8esamrxbuff[3]<<8)+p8esamrxbuff[4];//返回数据长度
				if(i!=(16+4))
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18,((u32)p8esamrxbuff)+5,i);
				Data_Inverse(((u32)p8txbuff)+18,16);//数据倒序(高低字节调换),入口Le>=2
				Data_Inverse(((u32)p8txbuff)+18+16,4);//数据倒序(高低字节调换),入口Le>=2
				TXlen=16+4;
				MWR(24*60,ADDR_TerminalRealTimer+2,2);//密钥失效定时24小时
				goto ACKDATA;
			case 14://F14	对称密钥更新
				x=MRR(((u32)p8rxbuff)+18+1,1);//更新密钥的总条数
				i=32*x;//每条密钥32字节
				if((18+2+i+16+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				while(x!=0)
				{
					Data_Inverse(((u32)p8rxbuff)+18+2+(32*(x-1)),32);//数据倒序(高低字节调换),入口Le>=2
					x--;
				}
				i=TESAM_CMD_TxRx(0x84240100,i+1,((u32)p8rxbuff)+18+1);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				goto ACKOK;
			case 15://F15	终端证书更新(请求)
				//获取芯片状态信息1字节
				i=TESAM_CMD_TxRx(0x800E0005,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				p1=MRR(ADDR_UART7Rx+5,1);
				//获取终端证书序列号16字节
				i=TESAM_CMD_TxRx(0x80320002+(p1<<8),0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18,((u32)p8esamrxbuff)+5,16);
				Data_Inverse(((u32)p8txbuff)+18,16);//数据倒序(高低字节调换),入口Le>=2
				//终端生产非对称密钥对，返回公钥长度为64字节
				i=TESAM_CMD_TxRx(0x802C0001,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18+16,((u32)p8esamrxbuff)+5,64);
				Data_Inverse(((u32)p8txbuff)+18+16,64);//数据倒序(高低字节调换),入口Le>=2
				TXlen=16+64;
				goto ACKDATA;
			case 16://F16	CA证书更新
				//证书更新报文包含了证书更新命令中的P1、P2、长度和证书密文数据
				i=MRR(((u32)p8rxbuff)+18,2);//密文长度
				if((18+2+i+16+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				Data_Inverse(((u32)p8rxbuff)+18+2,i);//数据倒序(高低字节调换),入口Le>=2
				p1=p8rxbuff[18+2];//p1
				p1<<=8;
				p1+=p8rxbuff[18+2+1];//p2
				i=p8rxbuff[18+4];//Lc
				i<<=8;
				i+=p8rxbuff[18+4+1];//Lc低字节
				i=TESAM_CMD_TxRx(0x84300000+p1,i,((u32)p8rxbuff)+18+2+4);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				goto ACKOK;
			case 17://F17	内部认证(该命令仅用于本地通信口,远程通信口应不支持该命令)
				//加密主站下的随机数R4
				Data_Inverse(((u32)p8rxbuff)+18,16);//数据倒序(高低字节调换),入口Le>=2
				i=TESAM_CMD_TxRx(0x84260000,16,((u32)p8rxbuff)+18);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18,((u32)p8esamrxbuff)+5,16);
				Data_Inverse(((u32)p8txbuff)+18,16);//数据倒序(高低字节调换),入口Le>=2
				//取16字节随机数R5
				i=TESAM_CMD_TxRx(0x801A1000,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18+16,((u32)p8esamrxbuff)+5,16);
				Data_Inverse(((u32)p8txbuff)+18+16,16);//数据倒序(高低字节调换),入口Le>=2
				TXlen=16+16;
				goto ACKDATA;
			case 18://F18	外部认证(该命令仅用于本地通信口,远程通信口应不支持该命令)
				//解密R5
				Data_Inverse(((u32)p8rxbuff)+18,16);//数据倒序(高低字节调换),入口Le>=2
				i=TESAM_CMD_TxRx(0x84280000,16,((u32)p8rxbuff)+18);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				//取16字节随机数R6,用于状态切换
				i=TESAM_CMD_TxRx(0x801A1000,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18,((u32)p8esamrxbuff)+5,16);
				Data_Inverse(((u32)p8txbuff)+18,16);//数据倒序(高低字节调换),入口Le>=2
				TXlen=16;
				goto ACKDATA;
			case 19://F19	状态切换
				Data_Inverse(((u32)p8rxbuff)+18+1,16);//数据倒序(高低字节调换),入口Le>=2
				Data_Inverse(((u32)p8rxbuff)+18+1+16,4);//数据倒序(高低字节调换),入口Le>=2
				p1=p8rxbuff[18];//P1：主站下发切换状态00从测试证书切换到正式证书,01从正式证书切换到测试证书
				i=TESAM_CMD_TxRx(0x842A0000+(p1<<8),20,((u32)p8rxbuff)+18+1);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				goto ACKOK;
			case 20://F20	置离线计数器
				Data_Inverse(((u32)p8rxbuff)+18,20);//数据倒序(高低字节调换),入口Le>=2
				i=TESAM_CMD_TxRx(0x84200000,20,((u32)p8rxbuff)+18);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				goto ACKOK;
			case 21://F21	转加密授权
				Data_Inverse(((u32)p8rxbuff)+18,32);//数据倒序(高低字节调换),入口Le>=2
				i=TESAM_CMD_TxRx(0x80160000,32,((u32)p8rxbuff)+18);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					goto err;
				}
				//清原任务
				for(i=0;i<MaxAFN10F12TaskType;i++)
				{
					MC(0,ADDR_AFN10F12+(i*LEN_AFN10F12PerTask),2+1+2+LEN_AFN10F12TaskData+2);//清任务
				}
				//清任务完成标志
				MC(0,ADDR_AFN10F13,MaxRS485AddCarrierPn*MaxAFN10F12TaskType);
				//清各口任务计数,免执行本条后立即下发批量任务来不及清计数值的理论上时间竟争冲突
			//RS485-1
				if(Get_LEN_UARTnCtrl(RS485_1PORT)!=0)//得到UART控制数据长度
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
					UARTCtrl->BatchMeterCount=0;//转发批量任务电表计数
				}
			//RS485-2
				if(Get_LEN_UARTnCtrl(RS485_2PORT)!=0)//得到UART控制数据长度
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
					UARTCtrl->BatchMeterCount=0;//转发批量任务电表计数
				}
			//RS485-3
				if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)//得到UART控制数据长度
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
					UARTCtrl->BatchMeterCount=0;//转发批量任务电表计数
				}
			//RS485-4(载波)
				if(Get_LEN_UARTnCtrl(RS485_4PORT)!=0)//得到UART控制数据长度
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					UARTCtrl->BatchMeterCount=0;//转发批量任务电表计数
				}
				goto ACKOK;
			default:
				goto err;
		
		}
	}
/*
err_F4://硬件安全认证错误
	p8txbuff[12]=0;//AFN=0
	p8txbuff[14]=0;//Pn=0
	p8txbuff[15]=0;
	p8txbuff[16]=8;//F4
	p8txbuff[17]=0;
	p8txbuff[18]=ErrType;//错误类型:1=签名校验错,2=密文校验错,3=对称MAC验证失败
	MEMClr(0xff,((u32)p8txbuff)+19,16);
	p8txbuff[1]=12+17;//数据长度
	p8txbuff[2]=0;
	p8txbuff[6]=0x80;//控制码DIR=1上行,PRM=0表示此帧报文来自从动站,要求访问位ACD暂0,功能码=0
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息(Tp)
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
	return;
*/
ACKOK:
	//全部确认
	Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息(Tp)
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}















