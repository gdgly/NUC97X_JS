

#include "Project.h"
#include "Terminal_ReadMeter_HX.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../terminal/airthmetic.h"


//void Uart_HX_RxTx(u32 PORTn);
void Read_HX_6E_TxData(u32 BuffAddr,u32 DI,u64 CommAddr);
u32 HX_Rx(u32 PORTn);
u32 HX_6E_RxData(u32 PORTn);

#if ((Project/100)==3)&&((USER/100)==12)//四川专变
//void Uart_HX_RxTx(u32 PORTn)//UART_DL645接收发送;0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
//{
//	u32 i;
//    UARTCtrl_TypeDef * UARTCtrl;
//	u16 * p16timer;
//	
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
//	switch(UARTCtrl->Task)//0=空闲,1=完成1帧发收,2=接收超时(或错误),3=启动发送,4=正在发送,5=正在接收
//	{
//		default:
//			UARTCtrl->Task=0;
//			break;
//		case 0://0=空闲
//			break;
//		case 1://1=完成1帧发收
//			break;
//		case 2://2=接收超时(或错误)
//			break;
//		case 3://3=启动发送
//			if(p16timer[0]!=0)
//			{
//				break;
//			}
//			Init_UARTn(PORTn);//初始化UART口,入口通信控制字已填入
//			Start_UARTn_Tx(PORTn);//启动UART口发送
//			UARTCtrl->Task=4;
//#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
//			if(PORTn==RS485_1PORT)
//			{
//				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
//			}
//			if(PORTn==RS485_2PORT)
//			{
//				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485抄表发送LED指示MS定时器,每10MS减1定时器
//			}
//#endif
//			break;
//		case 4://4=正在发送
//			if(Wait_UARTn_Tx(PORTn)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
//			{
//				p16timer[0]=UARTCtrl->TxToRxTimerOver;//发送后接收超时定时值ms/10(半字对齐)
//				UARTCtrl->RxByte=0;//RX计数(已接收字节数)=0
//				UARTCtrl->Task=5;
//			}
//			break;
//		case 5://5=正在接收
//			i=HX_Rx(PORTn);//DL645帧接收;返回0没完成,1完成,2超时
//			switch(i)
//			{
//				case 0://0没完成
//					return;
//				case 1://1完成
//					UARTCtrl->Task=1;
//					break;
//				default://2超时
//					UARTCtrl->Task=2;
//					return;
//			}
//			p16timer[0]=50/10;//下帧启动发送延时50MS后	  (定时器的单位是10ms)
//#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
//			if(PORTn==RS485_1PORT)
//			{
//				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
//			}
//			if(PORTn==RS485_2PORT)
//			{
//				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
//			}
//#endif
//			break;
//	}
//}

u32 HX_Rx(u32 PORTn)//HX帧接收;返回0没完成,1完成,2超时
					//经过这里的转换，接收寄存器的数据已经发生了变化
{
	u32 i;
	u32 p0;
	u32 p1;
	u32 RxByte;
	u16 j;
	uint8_t crc16_resh = 0;
  uint8_t crc16_resl = 0;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);  //得到UART中断接收缓冲长度	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);		   //得到UART接收缓冲长度
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);		   //得到UART接收缓冲地址
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+6);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	
	p0=p16fifo[0];//中断接收指针	  2字节
	p1=p16fifo[1];//当前已读指针	  2字节
	if((p0>=(LEN_UARTnIntRx-6))||(p1>=(LEN_UARTnIntRx-6)))
	{
		p16fifo[0]=0;//中断接收指针
		p16fifo[1]=0;//当前已读指针
		p16fifo[2]=0;//规约检查指针
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//有数据接收重置超时定时
//		p16timer[0]=1000/10;//字节间超时定时	
		p16timer[0]=500/10;//字节间超时定时
	}
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-6))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;

		if(RxByte==1)
		{
			if(i!=2)
			{	
				RxByte=0;	
				continue;
			}
		}
		else
		{
			if(RxByte==2) 	
			{
				if(i!=0x45)
				{	
					RxByte=0;   	
					continue; 	
				}
			}
			else
			{
				if(RxByte>=16)	//就算不扩展，最基本的长度
				{
					if(i == 3)
					{
						i = CheckHXRxData(p8rxbuff,0,RxByte);
						if(i>=1000)	
						{	
							RxByte=0; 
							continue;	
						}
						else
						{
							j = Crc_HX(p8rxbuff, i-3); //计算红相表的校验值
							crc16_resl = (uint8_t)(j&0XFF);
					    j  >>=8;
					    crc16_resh  = (uint8_t)(j&0XFF);
							if ((p8rxbuff[i-3]==crc16_resh)&&(p8rxbuff[i-2]== crc16_resl))
					    {//和校验正确
								p16fifo[1]=p1;//当前已读指针
								p16fifo[2]=p1;//检查指针==读指针,免检省时
								UARTCtrl->RxByte=i;
								return 1;
							}
							else
							{	
								RxByte=0;	
								continue;   
							}  
						}
					}
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//超时
		return 2;//2=接收超时
	}
	return 0;
}

u32 HX_6E_RxData(u32 PORTn)//HX帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
{
	u32 i,j;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 datalength;
	UARTCtrl_TypeDef * UARTCtrl;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);					    
	datalength = UARTCtrl->TxCount;
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	j=0;

	//通信地址比较
	for(i=0;i<4;i++)
	{
		if(p8txbuff[2+i+j] == 0x10)
		{
			j++;
			if(p8rxbuff[6+i]!=(p8txbuff[2+i+j]-0x40))	
			{
				break; //如果不相等，直接退出	
			}
		}
		else 
		{
			if(p8rxbuff[6+i]!=p8txbuff[2+i+j])
			{
				break;
			}
		}
	}
	if(i!=4)
	{
	  return 1;//返回0正确,1地址错,2控制码错,3数据标识错
	}
	CheckHXRxData(p8txbuff,0,(u32)datalength);
	if(p8txbuff[12] == 0x4c)	   //登入判断
	{
	//	if([12] != 0x06) return 3;  
	}
	else 	   //非登入判断
	{
		for(i=0;i<3;i++)
		{
			if(p8rxbuff[12+i]!=p8txbuff[12+i])	
			{
				break;
			}
		}
		if(i!=3)
		{
			return 3;	//返回0正确,1地址错,2控制码错,3数据标识错
		}	   
	}
	return 0;//返回0正确,1地址错,2控制码错,3数据标识错
}

#endif			

