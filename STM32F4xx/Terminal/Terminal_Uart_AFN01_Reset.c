
//UART 控制命令
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"






void Uart_AFN01(u32 PORTn)//复位
{
	u32 i;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u32 * p32;
  UARTCtrl_TypeDef * UARTCtrl;
	
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8rxbuff=(u8 *)(Get_ADDR_UARTnRx(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	
	p8txbuff[6]=(p8txbuff[6]&0xf0);//响应功能码 0
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<(12+16))
	{
		goto err;
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
//PW认证
	i=Uart_376_PW(PORTn);//消息认证码字段PW;入口PW首址,出口0有效,1无效
	if(i==4)
	{
		return;
	}
	if(i!=0x0)
	{
err:
		//全部否认
		Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		return;
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
//数据(启动站功能码10（请求1级数据）用于应用层请求确认（CON=1）的链路传输)
	DIaddr=((u32)p8rxbuff)+14;
//	RXaddr=ADDR_UART_BUFF+4+UART_FIFO_LEN+UART_CTRL_LEN+14+4;
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	if(DIlen<4)
	{
		goto err;
	}
	else
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		Fn=(i>>16);
		Pn=i&0xffff;
		if(Fn==0x0)
		{
			goto err;
		}
		if(Fn>5)
		{
			goto err;
		}
		if(Pn!=0x0)
		{
			goto err;
		}
		p32=(u32 *)(ADDR_DATABUFF);
		p32[0]=(u32)UARTCtrl;
		p32[1]=Fn;
		MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
		//全部确认
		Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		
		BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
		Terminal_Ram->ResetCommand_S_Timer=3;//延时5S
	}
}

