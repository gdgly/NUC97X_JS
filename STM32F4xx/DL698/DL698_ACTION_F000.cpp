

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_F000.h"
#include "../DL698/RM_TASK.h"
#include "../Device/IC_TESAM.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../../STM32F4xx/MS/MS.h"




//TESAM方法
u32 ACTION_F100(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 LEN_TypeByte1;
	u32 LEN_Data1;
	u32 LEN_TypeByte2;
	u32 LEN_Data2;
	u32 DI;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
//	LenTx=0;
	switch(i)
	{
		case 1://1	复位（参数）
//参数∷=integer（0）	通用方法，instance-specific。
			UART7Ctrl->Task=0;
			LenRx=2;
			return (LenRx<<16)+0;
		case 2://2	执行（参数）
//参数∷=Data	通用方法，instance-specific。
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 3://3	ESAM操作（参数）
//参数∷=SID
//应答∷=octet-string	对ESAM数据的操作，电能表透传给ESAM。
			
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 4://4	数据更新（参数）
//参数∷=structure
//{
//参数内容   octet-string，
//数据验证码 SID_MAC
//}	数据、数据MAC先发给ESAM验证，验证成功后，再设置到终端、电能表中。
//参数内容格式定义：
//4字节OAD+ 1字节内容LEN + 内容（见esam文件结构）。
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 5://5	协商失效（参数）
//参数∷=NULL
//应答∷=当前日期时间   date_time_s	
			if(LENmax_TxSPACE<7)
			{
				UARTCtrl->DAR=8;//越界(8)
				return 0;
			}
			MR((u32)p8tx,ADDR_DL698YMDHMS,7);
			LenRx=1;
			LenTx=7;
			UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
			return (LenRx<<16)+LenTx;
		case 6://6	钱包操作（参数）
//参数∷=structure
//{
//  操作类型   integer，
//购电金额   double-long-unsigned，
//购电次数   double-long-unsigned，
//户号       octet-string，
//数据验证码 SID_MAC，
//表号       octet-string
//}	操作类型：0—开户 1—充值 2-退费
//开户时，不需要验证客户编号，直接将客户编号写到ESAM的对应文件中；再进行充值操作。
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 7://7	密钥更新（参数）
//参数∷=structure
//F1 00 07 00 02 02
//{
//密钥密文   octet-string，
//09 82 07 20 ...
//数据验证码 SID_MAC
//5E 81 2E 00 00 0B 07 2D 00 00 00 00 00 00 00 00 39 04 95 65 D6 06 00 AB CD 16
//}	
		//发送TESAM:安全标识+附加数据AttachData+密文Endata1+MAC
			//计算密钥密文Endata1长度
			LEN_TypeByte1=Get_DL698ByteTypeLenByte(p8rx+7);//得到DL698字节类型用来表示长度单元的字节数
			LEN_Data1=Get_DL698ByteTypeLenValue(p8rx+7);//得到DL698字节类型用来表示长度单元的字节数
			//安全标识
			DI=MRR((u32)p8rx+7+LEN_TypeByte1+LEN_Data1+1,4);
			DI=R_Inverse(DI,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
			if(DI!=0x812E0000)
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			//取附加数据
			LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
			LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
			if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			MR(ADDR_UART7Tx+7,(u32)p8rx+7+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
			//取密钥密文Endata1
			MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+7+LEN_TypeByte1,LEN_Data1);
			//取MAC
			MR(ADDR_UART7Tx+7+LEN_Data2-2+LEN_Data1,(u32)p8rx+7+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2+1,4);
			i=LEN_Data2-2+LEN_Data1+4;
			i=TESAM_CMD_TxRx(0x812E0000,i,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			LenRx=3+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2+5;
			return (LenRx<<16)+0;
		case 8://8	证书更新（参数）
//参数∷=structure
//F1 00 08 00 02 02
//{
//证书内容  octet-string，
//09 82 05 F0 ...
//安全标识  SID
//5D 81 30 02 03 02 05 F0
//}	
		//发送TESAM:安全标识+附加数据AttachData+密文Endata1
			//计算密钥密文Endata1长度
			LEN_TypeByte1=Get_DL698ByteTypeLenByte(p8rx+7);//得到DL698字节类型用来表示长度单元的字节数
			LEN_Data1=Get_DL698ByteTypeLenValue(p8rx+7);//得到DL698字节类型用来表示长度单元的字节数
			//安全标识
			DI=MRR((u32)p8rx+7+LEN_TypeByte1+LEN_Data1+1,4);
			DI=R_Inverse(DI,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
			if(DI!=0x81300203)
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			//取附加数据
			LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
			LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
			if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			MR(ADDR_UART7Tx+7,(u32)p8rx+7+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
			//取密钥密文Endata1
			MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+7+LEN_TypeByte1,LEN_Data1);
			i=LEN_Data2-2+LEN_Data1;
			i=TESAM_CMD_TxRx(0x81300203,i,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			LenRx=3+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2;
			return (LenRx<<16)+0;
		case 9://9	设置协商时效（参数）
//参数∷=structure 
//{
//参数内容  octet-string，
//安全标识  SID
//} 
		//发送TESAM:安全标识+附加数据AttachData+密文Endata1
			//计算密钥密文Endata1长度
			LEN_TypeByte1=Get_DL698ByteTypeLenByte(p8rx+7);//得到DL698字节类型用来表示长度单元的字节数
			LEN_Data1=Get_DL698ByteTypeLenValue(p8rx+7);//得到DL698字节类型用来表示长度单元的字节数
			//安全标识
			DI=MRR((u32)p8rx+7+LEN_TypeByte1+LEN_Data1+1,4);
			DI=R_Inverse(DI,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
			if(DI!=0x81340105)
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			//取附加数据
			LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
			LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
			if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			MR(ADDR_UART7Tx+7,(u32)p8rx+7+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
			//取密钥密文Endata1
			MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+7+LEN_TypeByte1,LEN_Data1);
			i=LEN_Data2-2+LEN_Data1;
			i=TESAM_CMD_TxRx(0x81340105,i,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;//错误
			}
			LenRx=3+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2;
			return (LenRx<<16)+0;
		case 10://10	钱包初始化（参数）
//参数∷=structure
//{
//  预置金额   double-long-unsigned，
//数据验证码 SID_MAC
//}	
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		default:
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
	}
}


//安全模式参数
u32 ACTION_F101(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 1://方法1：复位（参数）参数∷=integer（0）复位时，清空属性3。
			MC(0,ADDR_F101_3,2);
			LenRx=2;
			return (LenRx<<16)+LenTx;
		case 127://方法127：增加显式安全模式参数（对象标识，权限）对象标识∷=OI，权限∷=long-unsigned
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[3]<<8)+p8rx[4];
			MR(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_F101_3)
			{
				Array=0;
				MC(0,ADDR_F101_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_F101_3;
			}
			if((i<Array)&&(Array!=0))
			{//找到,替换
				MR((u32)p8,(u32)p8rx,LENper_F101_3);
				MW(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			}
			else
			{//增加
				if(Array<NUMmax_F101_3)
				{
					MR((u32)p8,(u32)p8rx,LENper_F101_3);
					Array++;
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array;
					MW(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
				}
				else
				{
					UARTCtrl->DAR=8;//越界                 （8）
				}
			}
			return (LenRx<<16)+LenTx;
		case 128://方法128：删除显式安全模式参数（对象标识）对象标识∷=OI
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_F101_3)
			{
				Array=0;
				MC(0,ADDR_F101_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_F101_3;
			}
			if((i<Array)&&(Array!=0))
			{//找到,删除
				MR((u32)p8,(u32)p8+LENper_F101_3,LENmax_F101_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			}
			else
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
			}
			return (LenRx<<16)+LenTx;
		case 129://方法129：批量增加显式安全模式参数（array 安全模式参数）
//安全模式参数∷=structure
//{
//对象标识    OI，
//安全模式    long-unsigned
//}
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			Method=p8rx[1];//增加个数
			p8rx+=2;
			MR(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			while(Method--)
			{
				OI=(p8rx[3]<<8)+p8rx[4];
				//找相同的OI
				p8=(u8*)ADDR_128KDATABUFF;
				Array=p8[1];
				if(Array>NUMmax_F101_3)
				{
					Array=0;
				}
				p8+=2;
				for(i=0;i<Array;i++)
				{
					if(OI==((p8[3]<<8)+p8[4]))
					{
						break;
					}
					p8+=LENper_F101_3;
				}
				if((i<Array)&&(Array!=0))
				{//找到,替换
					MR((u32)p8,(u32)p8rx,LENper_F101_3);
				}
				else
				{//增加
					if(Array<NUMmax_F101_3)
					{
						MR((u32)p8,(u32)p8rx,LENper_F101_3);
						Array++;
						p8=(u8*)ADDR_128KDATABUFF;
						p8[0]=DataType_array;
						p8[1]=Array;
					}
					else
					{
						UARTCtrl->DAR=8;//越界                 （8）
					}
				}
				p8rx+=LENper_F101_3;
			}
			MW(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}
//RS232
u32 ACTION_F200(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	LenTx=0;
	switch(i)
	{
		case 127://方法127：配置端口（端口号，端口参数，端口功能）端口号∷=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx>LENper_F200_2)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return (LenRx<<16)+LenTx;
			}
				//端口∷=structure
				//{
				//端口号     OAD
				//端口参数   COMDCB，
				//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
				//}
//			OAD=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];
//			if(OAD==0xF2000201)
//			{
			if((p8rx[0]!=DataType_structure)&&(p8rx[0]!=DataType_array))
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			i=p8rx[9];
			if(i>2)
			{//校验位:无校验（0），奇校验（1），偶校验（2）
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			
			MW((u32)p8rx,ADDR_F200_2,LENper_F200_2);
			return (LenRx<<16)+LenTx;
//			}
//			UARTCtrl->DAR=6;//对象不存在           （6）
//			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}
//RS485
u32 ACTION_F201(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：配置端口（端口号，端口参数，端口功能）端口号∷=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx>LENper_F201_2)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
				//端口∷=structure
				//{
				//端口号     OAD
				//端口参数   COMDCB，
				//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
				//}
//			OAD=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];
//			if(OAD==0xF2010201)
			if(p8rx[6]==1)
			{//RS485-1
				MW((u32)p8rx,ADDR_F201_2,LENper_F201_2);
				return (LenRx<<16)+LenTx;
			}
//			if(OAD==0xF2010202)
			if(p8rx[6]==2)
			{//RS485-2
				MW((u32)p8rx,ADDR_F201_2+LENper_F201_2,LENper_F201_2);
				return (LenRx<<16)+LenTx;
			}
//			if(OAD==0xF2010203)
			if(p8rx[6]==3)
			{//RS485-3
				MW((u32)p8rx,ADDR_F201_2+(2*LENper_F201_2),LENper_F201_2);
				return (LenRx<<16)+LenTx;
			}
			UARTCtrl->DAR=6;//对象不存在           （6）
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}
//红外
u32 ACTION_F202(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：配置端口（端口号，端口参数，端口功能）端口号∷=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx>LENper_F202_2)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return (LenRx<<16)+LenTx;
			}
				//端口∷=structure
				//{
				//端口号     OAD
				//端口参数   COMDCB，
				//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
				//}
//			OAD=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];
//			if(OAD==0xF2020201)
//			{
				MW((u32)p8rx,ADDR_F202_2,LENper_F202_2);
				return (LenRx<<16)+LenTx;
//			}
//			UARTCtrl->DAR=6;//对象不存在           （6）
//			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}
//继电器输出
u32 ACTION_F205(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：修改开关属性（继电器号，开关属性）继电器号∷=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx>LENper_F205_127)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return (LenRx<<16)+LenTx;
			}
			Method=p8rx[6];//继电器号
//			OAD=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];
			if((Method==0)||(Method>NUMmax_F205_127))
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
				return (LenRx<<16)+LenTx;
			}
			Method--;
			MW((u32)p8rx,ADDR_F205_127+(Method*LENper_F205_127),LENper_F205_127);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//多功能端子
u32 ACTION_F207(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：修改工作模式（路号，工作模式）路号∷=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx>LENper_F207_127)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return (LenRx<<16)+LenTx;
			}
			MW((u32)p8rx,ADDR_F207_127,LENper_F207_127);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//载波/微功率无线接口
#define NUMmax_ReTx     3//最大代理重发次数
u32 ACTION_F209(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	UARTCtrl_TypeDef *UARTCtrlProxy;
	u32 NUM_ReTx;
	u32 offset;
	u8 *p8;
	u16 *p16;
	#ifndef LINUX
	u32 *p32;	
	#else
	ms_Type* ms;//得到ms表地址
	#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	LenTx=0;
	switch(i)
	{
		case 127://方法127：透明转发（参数）
//参数∷=structure
//{
//  通信地址                     TSA，
//  接收等到报文超时时间（秒）   long-unsigned，
//  透明转发命令                 octet-string
//}
//返回结果∷=octet-string
			p8rx+=4;
			LEN_RxAPDU-=4;
//02 03 55 07 05 00 00 00 00 00 13 12 00 14 09 19 68 17 00 43 05 13 00 00 00 00 00 10 54 74 05 01 00 ...
			p16=(u16*)(ADDR_DATABUFF);
			p16[0]=0;
			p16[1]=0;
			p16[2]=0;
			MR(ADDR_DATABUFF,(u32)p8rx+5,6);
			UARTCtrl->RMAddr[0]=p16[0];
			UARTCtrl->RMAddr[1]=p16[1];
			UARTCtrl->RMAddr[2]=p16[2];

			LenRx=p8rx[3];
			LenRx+=4;
			offset=LenRx;//超时时间地址
			LenRx+=3;
			i=p8rx[LenRx+1];
			if((i&0x80)==0)
			{
				i+=2;
				LenRx+=i;
			}
			else
			{
				i&=0x7f;
				if(i==1)
				{
					i=p8rx[LenRx+2];
					i+=3;
					LenRx+=i;
				}
				else
				{
					i=(p8rx[LenRx+2]<<8)|p8rx[LenRx+3];
					i+=4;
					LenRx+=i;
				}
			}
			
			i=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=RMNmax表示无效
			if(i>=NUM_RMmax)
			{//没找到配置序号
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
			if(p8[0]!=DataType_structure)
			{//配置无效
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			p8=Get_Element(p8+5,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8==0)
			{//没找到端口号
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			i=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//端口OAD
			i=OADtoPORTn(i);//OAD值转为通信口号
			if(i!=RS485_4PORT)
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(i);
			if(UARTCtrlProxy->Function==0)//口功能 0=终端维护口,!=0终端抄表口
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			UARTCtrlProxy->ProxyPORTn=PORTn;//发起代理的端口号
			//接收等到报文超时时间（秒）
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//得到UART秒定时器地址
			i=(p8rx[offset+1]<<8)|p8rx[offset+2];
			p16[0]=i;
			offset+=3;
			i=p8rx[offset+1];
			if((i&0x80)==0)
			{
				offset+=2;
			}
			else
			{
				i&=0x7f;
				if(i==1)
				{
					i=p8rx[offset+2];
					offset+=3;
				}
				else
				{
					i=(p8rx[offset+2]<<8)|p8rx[offset+3];
					offset+=4;
				}
			}
			//if(i>LEN_UART2FnDataBuff)
			if(i>LEN_UART2Tx)
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			p8=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
			MR((u32)p8,(u32)p8rx+offset,i);
			UARTCtrlProxy->ProxyTxRxByte=i;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
			UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
			NUM_ReTx=0;//代理重发次数
			while(1)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://主线程
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
				if(UARTCtrlProxy->ProxyTask==0)//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
				{
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//无数据接收
						NUM_ReTx++;//代理重发次数
						if(NUM_ReTx<NUMmax_ReTx)
						{
							UARTCtrlProxy->ProxyTxRxByte=i;//代理数据字节数,发送和接收后数据在发起口的FnDataBuff
							UARTCtrlProxy->ProxyTask=1;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
							continue;
						}
					}
					break;
				}
			}
			//代理返回数据
			i=UARTCtrlProxy->ProxyTxRxByte;
			if((i+4)>LENmax_TxSPACE)
			{
				p8tx[LenTx]=0;
				LenTx+=1;
				return (LenRx<<16)+LenTx;
			}
			p8tx[LenTx]=DataType_octet_string;
			LenTx+=1;
			if(i<=127)
			{
				p8tx[LenTx]=i;
				LenTx+=1;
			}
			else
			{
				p8tx[LenTx+0]=0x82;
				p8tx[LenTx+1]=i>>8;
				p8tx[LenTx+2]=i;
				LenTx+=3;
			}
			MR((u32)p8tx+LenTx,(u32)p8,i);
			LenTx+=i;
			return (LenRx<<16)+LenTx;
			
		case 128://方法128：配置端口参数（端口号，通信参数）端口号∷=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx>LENper_F209_128)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return (LenRx<<16)+LenTx;
			}
			MW((u32)p8rx,ADDR_F209_128,LENper_F209_128);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}


//蓝牙
u32 ACTION_F20B(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://方法127：配置端口参数（端口号，通信参数）端口号∷=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx>LENper_F20B_127)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return (LenRx<<16)+LenTx;
			}
			MW((u32)p8rx,ADDR_F20B_127,LENper_F20B_127);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}




