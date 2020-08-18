
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_CONNECT.h"
#include "../Device/IC_TESAM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/MS.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"



//服务器应用层协议数据单元（Server-APDU）
//Server-APDU∷=SEQUENCE
//{
//  应用层服务  CHOICE
//  {    
//    建立应用连接响应	[130]	CONNECT-Response，
//断开应用连接响应 	[131]	RELEASE-Response，
//断开应用连接通知  	[132]	RELEASE-Notification，
//    读取响应         	[133]	GET-Response，
//设置响应       	[134]	SET-Response，
//    操作响应        	[135]	ACTION-Response，
//    上报通知        	[136]	REPORT-Notification，
//代理响应          	[137]	PROXY-Response
//}，
//  跟随上报信息域 	FollowReport  OPTIONAL，
//时间标签		TimeTag	     OPTIONAL
//}


__align(4) const u8 FactoryVersion[]=//服务器厂商版本信息
{
	FactoryCode,//厂商代码         visible-string(SIZE (4))，
	((SVER>>12)&0xf)+0x30,((SVER>>8)&0xf)+0x30,((SVER>>4)&0xf)+0x30,((SVER>>0)&0xf)+0x30,//软件版本号      visible-string(SIZE (4))，
	((SVDATE>>20)&0xf)+0x30,((SVDATE>>16)&0xf)+0x30,((SVDATE>>12)&0xf)+0x30,((SVDATE>>8)&0xf)+0x30,((SVDATE>>4)&0xf)+0x30,((SVDATE>>0)&0xf)+0x30,//软件版本日期   visible-string(SIZE (6))，
	((HVER>>12)&0xf)+0x30,((HVER>>8)&0xf)+0x30,((HVER>>4)&0xf)+0x30,((HVER>>0)&0xf)+0x30,//硬件版本号      visible-string(SIZE (4))，
	((HVDATE>>20)&0xf)+0x30,((HVDATE>>16)&0xf)+0x30,((HVDATE>>12)&0xf)+0x30,((HVDATE>>8)&0xf)+0x30,((HVDATE>>4)&0xf)+0x30,((HVDATE>>0)&0xf)+0x30,//硬件版本日期   visible-string(SIZE (6))，
	FactoryInfoExt,//厂家扩展信息   visible-string(SIZE (8))
};


__align(4) const u8 ProtocolConformance[]=//商定的协议一致性块
{
	(Protocol0<<7)+(Protocol1<<6)+(Protocol2<<5)+(Protocol3<<4)+(Protocol4<<3)+(Protocol5<<2)+(Protocol6<<1)+(Protocol7<<0),
	(Protocol8<<7)+(Protocol9<<6)+(Protocol10<<5)+(Protocol11<<4)+(Protocol12<<3)+(Protocol13<<2)+(Protocol14<<1)+(Protocol15<<0),
	(Protocol16<<7)+(Protocol17<<6)+(Protocol18<<5)+(Protocol19<<4)+(Protocol20<<3)+(Protocol21<<2)+(Protocol22<<1)+(Protocol23<<0),
	(Protocol24<<7)+(Protocol25<<6)+(Protocol26<<5)+(Protocol27<<4)+(Protocol28<<3)+(Protocol29<<2)+(Protocol30<<1)+(Protocol31<<0),
	(Protocol32<<7)+(Protocol33<<6)+(Protocol34<<5)+(Protocol35<<4)+(Protocol36<<3)+(Protocol37<<2)+(Protocol38<<1)+(Protocol39<<0),
	0,
	0,
	0,
};

__align(4) const u8 FunctionConformance[]=//商定的功能一致性块
{
	(Function0<<7)+(Function1<<6)+(Function2<<5)+(Function3<<4)+(Function4<<3)+(Function5<<2)+(Function6<<1)+(Function7<<0),
	(Function8<<7)+(Function9<<6)+(Function10<<5)+(Function11<<4)+(Function12<<3)+(Function13<<2)+(Function14<<1)+(Function15<<0),
	(Function16<<7)+(Function17<<6)+(Function18<<5)+(Function19<<4)+(Function20<<3)+(Function21<<2)+(Function22<<1)+(Function23<<0),
	(Function24<<7)+(Function25<<6)+(Function26<<5)+(Function27<<4)+(Function28<<3)+(Function29<<2)+(Function30<<1)+(Function31<<0),
	(Function32<<7)+(Function33<<6)+(Function34<<5)+(Function35<<4)+(Function36<<3)+(Function37<<2)+(Function38<<1)+(Function39<<0),
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};


void CONNECT_Request(u32 PORTn)//连接
{
	u32 i;
	u32 LEN_SA;//服务器地址SA的长度
	u32 TI;//时间标签:0=无,1=有
	u32 LEN_RxAPDU;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//最大还可发送的字节数
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	ms_Type *ms;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息
	ServerConnectInfo_TypeDef *ServerConnectInfo;//服务机连接信息
	u32 Lc;
	u8 *p8esamrx;
	
	p8esamrx=(u8 *)(ADDR_UART7Rx);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//除起始字符和结束字符之外的帧字节数
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
//68 17 00 43  05  01 00 00 00 00 00  10  26 F6 
//05   应用层服务  CHOICE
//01   GET-Request∷=CHOICE
//00   PIID
//40 01 02 00    OAD
//00   TI
//ED 03 16
//时间标签
	i=p8rx[LEN_RxAPDU-2];
	if(i)
	{//有时间标签
		//TimeTag∷=SEQUENCE
		//{
		//  发送时标 		   	date_time_s，
		//  允许传输延时时间 	TI
		//}
		TI=1;//时间标签:0=无,1=有
		LENmax_TxSPACE-=8+5;//时间标签:1=有
	}
	else
	{
		TI=0;//时间标签:0=无,1=有
		LEN_RxAPDU-=1;
		LENmax_TxSPACE-=1;//时间标签:0=无
	}
//
	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
	
	if(UARTCtrl->pTxData)//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
	{
		ms=Get_ms();
		ms->ms_free(UARTCtrl->pTxData);
		UARTCtrl->pTxData=0;
	}
	UARTCtrl->PIID=p8rx[LEN_SA+8+1];//PIID
//Connect-Request∷=SEQUENCE
//{
//0  服务序号-优先级           PIID，
//1  期望的应用层协议版本号    long-unsigned，
//3 	期望的协议一致性块        ProtocolConformance，
//11  期望的功能一致性块        FunctionConformance，
//27  客户机发送帧最大尺寸      long-unsigned，
//29  客户机接收帧最大尺寸      long-unsigned，
//31  客户机接收帧最大窗口尺寸  unsigned，
//32  客户机最大可处理APDU尺寸 long-unsigned，
//34  期望的应用连接超时时间    double-long-unsigned，
//38  认证请求对象              ConnectMechanismInfo
	//ConnectMechanismInfo∷=CHOICE
	//{
	//  公共连接    [0] NullSecurity，
	//  一般密码    [1] PasswordSecurity，
	//  对称加密    [2] SymmetrySecurity，
	//  数字签名    [3] SignatureSecurity
	//}
//}
	MR((u32)&UARTCtrl->ConnectInfo,(u32)p8rx+LEN_SA+8+2,sizeof(ClientConnectInfo_TypeDef));
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
//CONNECT-Response∷=SEQUENCE
//{
//  服务序号-优先级-ACD       PIID-ACD，
//  服务器厂商版本信息        FactoryVersion，
	//FactoryVersion∷=SEQUENCE
	//{
	//厂商代码         visible-string(SIZE (4))，
	//软件版本号      visible-string(SIZE (4))，
	//软件版本日期   visible-string(SIZE (6))，
	//硬件版本号      visible-string(SIZE (4))，
	//硬件版本日期   visible-string(SIZE (6))，
	//厂家扩展信息   visible-string(SIZE (8))
	//}
//  商定的应用层协议版本号    long-unsigned，
//  商定的协议一致性块        ProtocolConformance，
//  商定的功能一致性块        FunctionConformance，
//  服务器发送帧最大尺寸      long-unsigned，
//  服务器接收帧最大尺寸      long-unsigned，
//  服务器接收帧最大窗口尺寸  unsigned，
//  服务器最大可处理APDU尺寸 long-unsigned，
//  商定的应用连接超时时间    double-long-unsigned，
//  连接响应对象              ConnectResponseInfo
//}
	p8tx[0]=130;//建立应用连接响应	[130]	CONNECT-Response，
	p8tx[1]=p8rx[LEN_SA+8+1];//PIID-ACD
	//服务器厂商版本信息
	MR((u32)p8tx+2,(u32)FactoryVersion,32);
	//商定的应用层协议版本号
	ServerConnectInfo=(ServerConnectInfo_TypeDef *)(p8tx+2);
	ServerConnectInfo->APPVER[0]=ClientConnectInfo->APPVER[0];
	ServerConnectInfo->APPVER[1]=ClientConnectInfo->APPVER[1];
	//商定的协议一致性块
	for(i=0;i<8;i++)
	{
		ServerConnectInfo->Protocol[i]=ProtocolConformance[i]&ClientConnectInfo->Protocol[i];
	}
	//商定的功能一致性块
	for(i=0;i<16;i++)
	{
		ServerConnectInfo->Function[i]=FunctionConformance[i]&ClientConnectInfo->Function[i];
	}
	//服务器发送帧最大尺寸
	i=Get_LEN_UARTnTx(PORTn);
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	ServerConnectInfo->Len_Tx[0]=i;
	ServerConnectInfo->Len_Tx[1]=i>>8;
	//服务器接收帧最大尺寸
	i=Get_LEN_UARTnRx(PORTn);
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	ServerConnectInfo->Len_Rx[0]=i;
	ServerConnectInfo->Len_Rx[1]=i>>8;
	//服务器接收帧最大窗口尺寸
	i=Get_LEN_UARTnIntRx(PORTn);//得到UART中断接收缓冲长度
	i-=10;
	i/=ServerConnectInfo->Len_Rx[0]|(ServerConnectInfo->Len_Rx[1]<<8);
	ServerConnectInfo->NUM_RxFrame=i;
	//服务器最大可处理APDU尺寸
	ServerConnectInfo->Len_APDU[0]=ServerConnectInfo->Len_Rx[0];
	ServerConnectInfo->Len_APDU[1]=ServerConnectInfo->Len_Rx[1];
	//商定的应用连接超时时间
	ServerConnectInfo->ConnectOVERTime[0]=ClientConnectInfo->ConnectOVERTime[0];
	ServerConnectInfo->ConnectOVERTime[1]=ClientConnectInfo->ConnectOVERTime[1];
	ServerConnectInfo->ConnectOVERTime[2]=ClientConnectInfo->ConnectOVERTime[2];
	ServerConnectInfo->ConnectOVERTime[3]=ClientConnectInfo->ConnectOVERTime[3];
	//连接响应对象
//ConnectResponseInfo∷=SEQUENCE
//{
//  认证结果      ConnectResult，
//  认证附加信息  SecurityData  OPTIONAL
//}
//ConnectResult∷=ENUMERATED
//{
//  允许建立应用连接     （0），
//密码错误             （1），
//对称解密错误         （2），
//非对称解密错误       （3），
//签名错误             （4），
//协议版本不匹配       （5），
//其他错误             （255）
//}

	ServerConnectInfo->ConnectResult=0;//认证结果:允许建立应用连接
	p8tx[2+sizeof(ServerConnectInfo_TypeDef)]=0;//认证附加信息  OPTIONAL=0 表示没有
	LEN_TxAPDU=2+sizeof(ServerConnectInfo_TypeDef)+1;
	LENmax_TxSPACE-=2+sizeof(ServerConnectInfo_TypeDef)+1;
	
  i=p8rx[LEN_SA+8+1+38];
	switch(i)
	{
		default:
		case 0://公共连接    [0] NullSecurity
			UARTCtrl->CONNECT=100;
			UARTCtrl->OAD_44000400[0]=DataType_unsigned;//当前连接的客户机地址
			UARTCtrl->OAD_44000400[1]=p8rx[LEN_SA+5];
			break;
		case 1://一般密码    [1] PasswordSecurity
//PasswordSecurity∷=visible-string
			UARTCtrl->CONNECT=100;//连接标志
			UARTCtrl->OAD_44000400[0]=DataType_unsigned;//当前连接的客户机地址
			UARTCtrl->OAD_44000400[1]=p8rx[LEN_SA+5];
			
			MR(ADDR_DATABUFF,ADDR_4401+1,LENmax_4401-1);
			i=Compare_octet_string(p8rx+LEN_SA+8+1+39,(u8 *)ADDR_DATABUFF);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i)
			{
				UARTCtrl->CONNECT=0;
				UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
				ServerConnectInfo->ConnectResult=1;//认证结果:密码错误
				break;
			}
			break;
		case 2://对称加密    [2] SymmetrySecurity
//SymmetrySecurity∷=SEQUENCE
//{
//  密文1        octet-string，
//  客户机签名1  octet-string
//}
//接收数据
//68 5E 00 43 05 11 11 11 11 11 11 10 54 12 
//02 建立应用连接请求	 [2]		CONNECT-Request
//0D 00 16 FF FF FF FF C0 00 00 00 FF FE C4 00 00 00 00 00 00 00 00 00 00 00 00 00 02 00 04 00 01 07 D0 00 00 1C 20 
//02 请求认证的机制信息 2=对称加密
//20 02 88 B7 02 ED E9 F3 F4 42 B8 38 F5 54 8B 14 90 9F BE 92 F4 72 B1 B0 09 4B 0A 0E B9 D7 B6 A9 35 密文1
//04 33 16 83 7B 客户机签名1
//00 
//59 6B 16
//发送数据
//68 90 00 C3 05 11 11 11 11 11 11 10 40 C0 82 0D 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 00 00 FF FF FF FF C0 00 00 00 FF FE C4 00 00 00 00 00 00 00 00 00 00 00 00 00 02 00 02 00 01 07 D0 00 00 1C 20 00 01 30 7D 13 AB 42 72 E7 76 63 8D 79 1A 59 7B 1E 5D BE CB 93 8F 6D 9A 2B 0C 90 04 FE E7 CA 3F C8 73 0D 3E DC 43 78 3B E5 CF F8 DC E6 CA EB DB A0 F2 97 04 2F 71 8A 84 00 00 CC CC 16

		case 3://数字签名    [3] SignatureSecurity
//SignatureSecurity∷=SEQUENCE
//{
//  密文2        octet-string，
//  客户机签名2  octet-string
//}
			UARTCtrl->CONNECT=100;//连接标志
			UARTCtrl->OAD_44000400[0]=DataType_unsigned;//当前连接的客户机地址
			UARTCtrl->OAD_44000400[1]=p8rx[LEN_SA+5];
		
			p8tx[2+sizeof(ServerConnectInfo_TypeDef)]=1;//认证附加信息  OPTIONAL=0 表示没有
			p8rx+=LEN_SA+8+1+39;
			i=p8rx[0];//  密文2        octet-string
			if(i&0x80)
			{
				if((i&0x7f)==1)
				{
					Lc=p8rx[1];
					if(Lc>(LEN_UART7Tx-7))
					{
						UARTCtrl->CONNECT=0;
						UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
						ServerConnectInfo->ConnectResult=255;//认证结果:其他错误
						break;
					}
					MR(ADDR_UART7Tx+7,(u32)p8rx+2,Lc);
					p8rx+=2+Lc;
				}
				else
				{
					Lc=(p8rx[1]<<8)+p8rx[2];
					if(Lc>(LEN_UART7Tx-7))
					{
						UARTCtrl->CONNECT=0;
						UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
						ServerConnectInfo->ConnectResult=255;//认证结果:其他错误
						break;
					}
					MR(ADDR_UART7Tx+7,(u32)p8rx+3,Lc);
					p8rx+=3+Lc;
				}
			}
			else
			{
				Lc=i;
				MR(ADDR_UART7Tx+7,(u32)p8rx+1,Lc);
				p8rx+=1+Lc;
			}
			i=p8rx[0];//  客户机签名2  octet-string
			if(i&0x80)
			{
				if((i&0x7f)==1)
				{
					i=p8rx[1];
					if((Lc+i)>(LEN_UART7Tx-7))
					{
						UARTCtrl->CONNECT=0;
						UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
						ServerConnectInfo->ConnectResult=255;//认证结果:其他错误
						break;
					}
					MR(ADDR_UART7Tx+7+Lc,(u32)p8rx+2,i);
					Lc+=i;
				}
				else
				{
					i=(p8rx[1]<<8)+p8rx[2];
					if((Lc+i)>(LEN_UART7Tx-7))
					{
						UARTCtrl->CONNECT=0;
						UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
						ServerConnectInfo->ConnectResult=255;//认证结果:其他错误
						break;
					}
					MR(ADDR_UART7Tx+7+Lc,(u32)p8rx+3,i);
					Lc+=i;
				}
			}
			else
			{
				MR(ADDR_UART7Tx+7+Lc,(u32)p8rx+1,i);
				Lc+=i;
			}
			i=TESAM_CMD_TxRx(0x80020000,Lc,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{
				UARTCtrl->CONNECT=0;
				UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
				ServerConnectInfo->ConnectResult=4;//认证结果:签名错误（4）
				break;
			}
//SecurityData∷=SEQUENCE
//{
//  服务器随机数    RN，
//  服务器签名信息  octet-string
//}
			i=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
			if(i<48)
			{
				UARTCtrl->CONNECT=0;
				UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
				ServerConnectInfo->ConnectResult=4;//认证结果:签名错误（4）
				break;
			}
			//服务器随机数    RN
			p8tx[LEN_TxAPDU]=48;
			MR((u32)p8tx+LEN_TxAPDU+1,(u32)p8esamrx+5,48);
			LEN_TxAPDU+=1+48;
			i-=48;
			if(i>127)
			{
				UARTCtrl->CONNECT=0;
				UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
				ServerConnectInfo->ConnectResult=4;//认证结果:签名错误（4）
				break;
			}
			//服务器签名信息  octet-string
			p8tx[LEN_TxAPDU]=i;
			MR((u32)p8tx+LEN_TxAPDU+1,(u32)p8esamrx+5+48,i);
			LEN_TxAPDU+=1+i;
			break;
	}
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
}


void RELEASE_Request(u32 PORTn)//断开
{
//	u32 i;
	u32 LEN_SA;//服务器地址SA的长度
	u32 TI;//时间标签:0=无,1=有
	u32 LEN_RxAPDU;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//最大还可发送的字节数
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	ms_Type *ms;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//除起始字符和结束字符之外的帧字节数
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
//68 17 00 43  05  01 00 00 00 00 00  10  26 F6 
//05   应用层服务  CHOICE
//01   GET-Request∷=CHOICE
//00   PIID
//40 01 02 00    OAD
//00   TI
//ED 03 16
//
	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
	
	if(UARTCtrl->pTxData)//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
	{
		ms=Get_ms();
		ms->ms_free(UARTCtrl->pTxData);
		UARTCtrl->pTxData=0;
	}
	UARTCtrl->PIID=p8rx[LEN_SA+8+1];//PIID
//RELEASE-Response∷=SEQUENCE
//{
//  服务序号-优先级-ACD  PIID-ACD，
//  结果  ENUMERATED
//{
//成功  （0）
//}
//}
	UARTCtrl->CONNECT=0;//连接标志
	UARTCtrl->OAD_44000400[0]=0;//当前连接的客户机地址
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	ClientConnectInfo->Len_Rx[0]=0;//客户机接收缓冲长度为默认值
	ClientConnectInfo->Len_Rx[1]=0;
	
	p8tx[0]=131;//断开应用连接响应 	[131]	RELEASE-Response，
	p8tx[1]=p8rx[LEN_SA+8+1];//PIID-ACD
	p8tx[2]=0;//成功  （0）
	LEN_TxAPDU=3;
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
}

void RELEASE_Notification(u32 PORTn)//断开通知
{

//断开应用连接通知  	[132]	RELEASE-Notification，
	
	
}




















