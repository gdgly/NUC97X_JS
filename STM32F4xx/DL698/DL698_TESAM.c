

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_TESAM.h"
#include "../DL698/DL698_GET.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/IC_TESAM.h"
#include "../Device/MEMRW.h"



u32 GET_ESAM_OAD(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 offset;
	u32 Len;
	UARTCtrl_TypeDef *UARTCtrl;
	u8 *p8esamrx;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	p8esamrx=(u8 *)(ADDR_UART7Rx);
	switch((OAD>>8)&0xff)
	{
		default:
			UARTCtrl->DAR=6;//对象不存在
			return 0;
		case 1://逻辑名	（static）
			if(LENmax_TxSPACE<4)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			p8tx[0]=DataType_octet_string;//long-unsigned
			p8tx[1]=2;
			p8tx[2]=OAD>>24;
			p8tx[3]=OAD>>16;
			return 4;
		case 2://ESAM序列号	（static）
//0 ESAM 序列号(8B )
//8 ESAM 版本号（ 4B ）
//12 对称 密钥版本（16 B）
//28 证书 版本 （主站证书版本号 1B+终端证书版本号 1B）
//30 会话 时效门限（ 4B）
//34 会话 时效剩余间（ 4B）
//当前计数器
//38 ASCTR：单地址应用协商计数器（ 4B）
//42 ARCTR：主动上报计数器 （4B）
//46 AGSEQ ：应用广播通信序列号 （4B）
//50 终端证书序列号(16B )
//66 主站证书序列号（ 16 B）
			Len=8;
			offset=0;
			break;
		case 3://ESAM版本号	（static）
			Len=4;
			offset=8;
			break;
		case 4://对称密钥版本	（static）
			Len=16;
			offset=12;
			break;
		case 5://会话时效门限	（static）
			Len=4;
			offset=30;
			break;
		case 6://会话时效剩余时间	（dyn.）
			Len=4;
			offset=34;
			break;
		case 7://当前计数器	（static）
			offset=38;
			if(LENmax_TxSPACE<17)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			i=TESAM_CMD_TxRx(0x803600FF,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{
				UART7Ctrl->Task=0;//关电源重启
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;
			}
			p8tx[0]=DataType_structure;
			p8tx[1]=3;
			p8tx[2]=DataType_double_long_unsigned;
			MR((u32)p8tx+3,ADDR_UART7Rx+5+offset,4);
			p8tx[7]=DataType_double_long_unsigned;
			MR((u32)p8tx+8,ADDR_UART7Rx+5+offset+4,4);
			p8tx[12]=DataType_double_long_unsigned;
			MR((u32)p8tx+13,ADDR_UART7Rx+5+offset+8,4);
			return 17;
		case 8://证书版本	（static）
			Len=2;
			offset=28;
			break;
		case 9://终端证书序列号	（static）
			Len=16;
			offset=50;
			break;
		case 11://主站证书序列号	（static）
			Len=16;
			offset=66;
			break;
		case 10://终端证书	（static）
			i=TESAM_CMD_TxRx(0x8036000B,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{
				UART7Ctrl->Task=0;//关电源重启
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;
			}
			Len=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
			if(LENmax_TxSPACE<(Len+4))
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			p8tx[0]=DataType_octet_string;
			p8tx[1]=0x82;
			p8tx[2]=Len>>8;
			p8tx[3]=Len;
			MR((u32)p8tx+4,ADDR_UART7Rx+5,Len);
			return Len+4;
		case 12:// 主站证书	（static）
			i=TESAM_CMD_TxRx(0x8036000C,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{
				UART7Ctrl->Task=0;//关电源重启
				UARTCtrl->DAR=22;//ESAM验证失败        （22）
				return 0;
			}
			Len=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
			if(LENmax_TxSPACE<(Len+4))
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			p8tx[0]=DataType_octet_string;
			p8tx[1]=0x82;
			p8tx[2]=Len>>8;
			p8tx[3]=Len;
			MR((u32)p8tx+4,ADDR_UART7Rx+5,Len);
			return Len+4;
		case 13://ESAM安全存储对象列表	（static）
//需要存储到ESAM中的对象,ESAM安全存储对象列表∷=array OAD
			
			UARTCtrl->DAR=6;//对象不存在
			return 0;
	}
	if(LENmax_TxSPACE<(Len+2))
	{
		GET_Request_Frame(PORTn);//分帧标志
		return 0;
	}
	i=TESAM_CMD_TxRx(0x803600FF,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
	if(i)
	{
		UART7Ctrl->Task=0;//关电源重启
		UARTCtrl->DAR=22;//ESAM验证失败        （22）
		return 0;
	}
	if((((OAD>>8)&0xff)==5)||(((OAD>>8)&0xff)==6))
	{
		p8tx[0]=DataType_double_long_unsigned;
		MR((u32)p8tx+1,ADDR_UART7Rx+5+offset,Len);
		return Len+1;
	}
	else	
	{
		p8tx[0]=DataType_octet_string;
		p8tx[1]=Len;
		MR((u32)p8tx+2,ADDR_UART7Rx+5+offset,Len);
		return Len+2;
	}
//	p8tx[0]=DataType_octet_string;
//	p8tx[1]=Len;
//	MR((u32)p8tx+2,ADDR_UART7Rx+5+offset,Len);
//	return Len+2;
}





















