
//数据转发
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../Device/IC_BUZZ.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"




void Forwarding(u32 PORTn);//数据转发处理过程
void Uart_AFN10(u32 PORTn)//数据转发
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Fn;
	u32 DIaddr;
	u32 DIlen;
	u32 TXaddr;
	u32 TXlen;
	u32 LEN_UARTnTx;
	u64 d64a;
	u64 d64b;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	u32 *p32;

	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
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
		if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
		{
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
			if(i!=0x0)
			{
				return;//舍弃该报文
			}
		}
	}
//PW认证
	i=Uart_376_PW(PORTn);//消息认证码字段PW;出口0有效,1无效,4已返错误帧
	if(i==4)
	{
		return;
	}
	if(i!=0x0)
	{
err:
		//全部否认
		p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//响应功能码 9无数据
		Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		return;
	}

//若红外口需设置开关
//#if (USER/100)==5//上海
//	if(PORTn==IRPORT)
//	{
//		if(Comm_Ram->SelfTestCommand!=0x55555555)//自测试命令0x55555555测试1(硬件),0xAAAAAAAA测试2(FLASH),0x5555AAAA测试3(FLASH寿命测试)
//		{
//			goto err;
//		}
//	}
//#endif

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
	if(DIlen<=4)
	{
		goto err;
	}
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	i=DItoFnPn(PORTn);//数据标识分解为FnPn
	Fn=(i>>16);
	switch(Fn)
	{
		case 1://F1透明转发
			if(DIlen<(4+6))
			{//数据单元最小6+1字节
				goto err;
			}
			i=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//转发字节数L
			if(i==0x0)
			{//转发字节数L=0错
				goto err;
			}
//	if(p8rxbuff[18]==2)
//	{
//		p8rxbuff[18]=31;//仅模拟台体走载波口ESAM内部测试用
//	}
			switch(p8rxbuff[18])
			{
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
				case 1://端口号=1,内部交流采样接口
#endif
				case 2://端口号=2,RS485-1
				case 3://端口号=3,RS485-2
#if (((Project/100)==2)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
				case 4://端口号=4,RS485-3
	#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
				case 31://端口号=31,载波
	#endif			
#endif
					if(p8rxbuff[18]!=31)
					{//RS485口
						p16timer[0]=5000/10;//等待数据转发口空闲定时5S
					}
					else
					{//载波口
						p16timer[0]=120000/10;//等待数据转发口空闲定时120S
					}
					UARTCtrl->Task=8;//8=等待通过其它口数据转发
					break;
				default:
					goto err;
			}
			break;
		case 9://F9转发主站直接对电表的抄读数据命令
		case 10://F10转发主站直接对电表的批量抄读数据命令
			switch(p8rxbuff[18])
			{
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
				case 1://端口号=1,内部交流采样接口
#endif
				case 2://端口号=2,RS485-1
				case 3://端口号=3,RS485-2
#if ((Project/100)==5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
				case 4://端口号=4,RS485-3
#endif
#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块
				case 31://端口号=31,载波
#endif			
					if(p8rxbuff[18]!=31)
					{//RS485口
						p16timer[0]=5000/10;//等待数据转发口空闲定时5S
					}
					else
					{//载波口
						p16timer[0]=120000/10;//等待数据转发口空闲定时120S
					}
//----F9转为F10相同命令格式-----
					if(Fn==9)
					{
						i=p8rxbuff[19];//转发中继级数n
						if(i==0xff)
						{
							i=0;
						}
						i*=6;//中继级数和地址字节长度
						x=MRR(((u32)p8rxbuff)+20+i+6+1,4);//转发直接抄读的数据标识
						p8rxbuff[20+i+6+1]=1;//标识个数=1
						MWR(x,((u32)p8rxbuff)+20+i+6+2,4);//转发直接抄读的数据标识
					}
//------------------------------			
					UARTCtrl->Task=8;//8=等待通过其它口数据转发
					break;
				default:
					goto err;
			}
			break;

		case 12://F12对批量电表下发任务命令
			z=p8rxbuff[18+2];//任务类型
			if(z>=MaxAFN10F12TaskType)//最大转发F12任务类型数
			{//任务类型>=MaxAFN10F12TaskType
				goto err;
			}
			y=MRR(ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData,2);//原已配置电表数量n
			i=(p8rxbuff[18+3]+(p8rxbuff[18+4]<<8));//本次任务数据长度Ln
			x=p8rxbuff[18+5+i]+(p8rxbuff[18+5+i+1]<<8);//本次配置电表数量n
			TXlen=5+i+2+(x*40);//配置区总长度
			if(DIlen!=(TXlen+4))
			{
				goto err;
			}
			DIaddr=(u32)p8rxbuff+18+5+i+2;//第1个配置地址
			//删除本次配置内的相同表号
			for(i=0;i<x;i++)
			{
				d64a=MRR(DIaddr+(i*40),8);
				for(Fn=0;Fn<x;Fn++)
				{
					d64b=MRR(DIaddr+(Fn*40),8);
					if(d64a==d64b)
					{//相同地址
						if(i!=Fn)
						{//不是自己
							MR(DIaddr+(Fn*40),DIaddr+((Fn+1)*40),(x-Fn)*40);
							x--;//本次配置电表数量-1
							Fn--;
						}
					}
				}
				p32=(u32 *)(ADDR_MS_ENTRY);
				(*(void(*)())p32[115])();//调用I0_Main_Return;63 开环主任务
			}
			//删除与原配置相同表号
			for(i=0;i<y;i++)
			{
				d64a=MRR(ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData+2+(i*40),8);
				for(Fn=0;Fn<x;Fn++)
				{
					d64b=MRR(DIaddr+(Fn*40),8);
					if(d64a==d64b)
					{//相同地址
						MR(DIaddr+(Fn*40),DIaddr+((Fn+1)*40),(x-Fn)*40);
						x--;//本次配置电表数量-1
						Fn--;
					}
				}
				p32=(u32 *)(ADDR_MS_ENTRY);
				(*(void(*)())p32[115])();//调用I0_Main_Return;63 开环主任务
			}
			if((5+LEN_AFN10F12TaskData+2+((y+x)*40))>LEN_AFN10F12PerTask)
			{//存储空间不够
				goto err;
			}
			//写任务格式	BIN	2
			//写任务类型	BIN	1
			//写任务数据长度Ln	BIN	2
			//写任务数据	BIN	LEN_AFN10F12TaskData
			MW(((u32)p8rxbuff)+18,ADDR_AFN10F12+(z*LEN_AFN10F12PerTask),5+LEN_AFN10F12TaskData);
			//写本次配置的电表数据
			MW(DIaddr,ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData+2+(y*40),x*40);
			//写己配置总电表数
			MWR(y+x,ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData,2);
			
			//全部确认
			BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
			Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			break;
		case 13://F13查询对批量电表下发任务的执行状态
			z=p8rxbuff[18];//查询的任务类型
			if(z>=MaxAFN10F12TaskType)//最大转发F12任务类型数
			{
				goto err;
			}
			y=MRR(ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData,2);//配置电表数量n
			TXaddr=(u32)p8txbuff+18;
			TXlen=4;
			MWR(y,TXaddr+1,2);//批量执行对电表的任务中电表总数量
			TXaddr+=5;
			TXlen+=5;
			Fn=0;//批量执行对电表的任务中已成功的电表数量n
			for(;UARTCtrl->MulFrameGPCR<y;UARTCtrl->MulFrameGPCR++)//多帧时通用计数寄存器
			{
				if(MRR(ADDR_AFN10F13+(z*MaxRS485AddCarrierPn)+UARTCtrl->MulFrameGPCR,1)==1)
				{//0=没完成,1=完成,2=错误
					if((TXlen+14+8+2+(8))>LEN_UARTnTx)
					{
						goto MulTx;//发送-还有后续帧
					}
					MR(TXaddr,ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData+2+(UARTCtrl->MulFrameGPCR*40),8);//表号
					TXaddr+=8;
					TXlen+=8;
					Fn++;//批量执行对电表的任务中已成功的电表数量n
				}
			}
			MWR(Fn,(u32)p8txbuff+18+3,2);//已成功的电表数量n
			
			TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
			p8txbuff[1]=TXlen;
			p8txbuff[2]=TXlen>>8;
			if(UARTCtrl->TxFrameCount!=0)//寄存多帧数据时已发送帧计数
			{
				p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
				p8txbuff[13]|=0x20;//返回帧暂为多帧结束帧TpV,FIR,FIN,CON;PSEQ|RSEQ
			}
			UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		MulTx://发送-还有后续帧
			MWR(Fn,(u32)p8txbuff+18+3,2);//已成功的电表数量n
			TXlen+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
			p8txbuff[1]=TXlen;
			p8txbuff[2]=TXlen>>8;
			p8txbuff[13]&=0x9f;//返回帧为多帧中间帧TpV,FIR,FIN,CON;PSEQ|RSEQ
			if(UARTCtrl->TxFrameCount==0)//寄存多帧数据时已发送帧计数
			{
				p8txbuff[13]|=0x40;//返回帧为多帧起始帧TpV,FIR,FIN,CON;PSEQ|RSEQ
			}
			UARTCtrl->TxFrameCount++;//寄存多帧数据时已发送帧计数
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息,暂等待单帧数据发送
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			//下响应帧序号RSEQ+1
			i=p8rxbuff[13];
			x=i;
			i&=0xf0;
			x++;
			x&=0x0f;
			i|=x;
			p8rxbuff[13]=i;
			UARTCtrl->Task=5;//5=等待多帧数据发送
			return;
		default:
			goto err;
	}
}
