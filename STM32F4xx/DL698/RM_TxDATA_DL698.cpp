
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_TASK.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"
#if ((USER/100)==9)//河南
//明文读取
u32 RM_SECURITY(u32 OI)//抄表安全模式字,入口:OI,出口:(同安全请求)b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
{
	return 0;
}
#else
//明文+数据验证码读取
u32 RM_SECURITY(u32 OI)//抄表安全模式字,入口:OI,出口:(同安全请求)b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
{
	switch(OI&0xf000)
	{
		case 0x0000://当前电能
			return 0;//明文
		case 0x1000://最大需量
			return 0x81;//明文+数据验证码
		case 0x2000:
			if(OI==0x202c)
			{//当前钱包文件
				return 0;//明文
			}
			else
			{//变量
				return 0x81;//明文+数据验证码
			}
		case 0x3000://事件
			return 0x81;//明文+数据验证码
		case 0x4000:
			switch(OI)
			{
				case 0x4000://日期时间
				case 0x4001://通信地址
				case 0x4002://表号
				case 0x4111://备案号
					return 0;//明文
				default:
					return 0x81;//明文+数据验证码
			}
		case 0x5000://冻结
			return 0x81;//明文+数据验证码(电科院台体测试MAC验证时读取的数据项)
		case 0x6000://采集监控
			return 0;//明文
		case 0x7000://集合
			return 0;//明文
		case 0x8000://控制
			return 0;//明文
		case 0xf000://分帧传输,ESAM,安全模式参数,输入输出接口设备，自定义
			return 0;//明文
		default:
			return 0;//明文
	}
}
#endif


u32 RM_TxDATA_DL698(u32 PORTn,u8 *p8CSD,u8 *p8tx)//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
{
	u32 i;
	u32 x;
	u32 Byte;
	u32 YYMD;
	UARTCtrl_TypeDef * UARTCtrl;
#if (USER/100)==15//安徽
	u32 NUM_RCSD;
#endif	
//	u8* p8;
	u32 y;
	//u32 offset;
	u64 d64a;
	u64 d64b;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//起始字符（68H）  1Byte
	p8tx[0]=0x68;
//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
//控制域C  1Byte
	p8tx[3]=0x43;
//地址域A
	p8tx[4]=5;
	p8tx[5]=UARTCtrl->RMAddr[2]>>8;
	p8tx[6]=UARTCtrl->RMAddr[2];
	p8tx[7]=UARTCtrl->RMAddr[1]>>8;
	p8tx[8]=UARTCtrl->RMAddr[1];
	p8tx[9]=UARTCtrl->RMAddr[0]>>8;
	p8tx[10]=UARTCtrl->RMAddr[0];
//客户机地址CA  1Byte
	p8tx[11]=0;
//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
//空出3字节，当安全模式读取时用
	p8tx+=3;
//APDU
	i=UARTCtrl->TaskID;

	switch((i>>8)&0xff)//任务号
	{
	#if(USER/100)==17//江苏系
		default:
			//if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
			
			if(((UARTCtrl->TaskID>>20)&0xf)==5)
			{
				p8tx[1]=0x15;
#if 0
				x=Terminal_Router->NoteNum1;//已知总从节点数
				d64a=MRR((u32)p8tx+2,6);
				for(y=0;y<x;y++)
				{
					d64b=MRR((u32)&NoteList1->List1[y].Addr,6);
					if(d64a==d64b)
					{
						d64b=MRR((u32)&NoteList1->List1[y].CollectorAddr,6);
//						d64b=R_Inverse(d64b,6);
//						MWR(d64b,(u32)&UARTCtrl->RMAddr,6);
//						p8tx[2]=UARTCtrl->RMAddr[2]>>8;
//						p8tx[3]=UARTCtrl->RMAddr[2];
//						p8tx[4]=UARTCtrl->RMAddr[1]>>8;
//						p8tx[5]=UARTCtrl->RMAddr[1];
//						p8tx[6]=UARTCtrl->RMAddr[0]>>8;
//						p8tx[7]=UARTCtrl->RMAddr[0];
						MWR(d64b,(u32)p8tx+2,6);
						break;//原已存在
					}
				}
				if(y==x)
				{
					myprintf("[%s:%d]:没有这个从节点信息\n",(u32)&__func__,(u32)__LINE__,0);
					return 0;
				}
//				p8=(u8*)ADDR_Meter_Collector;
//				offset=0;
//				for(x=0;x<NUM_RMmax;x++)
//				{
//					for(y=0;y<6;y++)
//					{
//						if((p8[y+offset]!=p8tx[2+y])&&p8[y+offset]!=0)
//						{
//							break;
//						}
//					}
//					if(y!=6)//不相同并且不是0
//					{
//						offset+=12;
//					}
//					else
//					{
//						MR((u32)p8tx+2,(u32)p8+offset+6,6);
//						MR((u32)&UARTCtrl->RMAddr,(u32)p8+offset+6,6);
//						Data_Inverse((u32)&UARTCtrl->RMAddr,6);
//						break;
//					}
//					
//				}
				myprintf("[%s:%d]:系统当前正在抄读采集器地址\n",(u32)&__func__,(u32)__LINE__,0);
				for(i=6;i>0;i--)
				{
					myprintf("%02x",NoteList1->List1[y].CollectorAddr[i-1],0,0);
				}
				myprintf("\n",0,0,0);
#else
				
				x=Terminal_Router->NoteNum;//已知总从节点数
				//myprintf("[%s:%d]:Terminal_Router->NoteNum=%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
				d64a=MRR((u32)p8tx+2,6);
				for(y=0;y<x;y++)
				{
					d64b=MRR((u32)&NoteList->List[y].Addr,6);
					if(d64a==d64b)
					{
						d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
						MWR(d64b,(u32)p8tx+2,6);
						break;//原已存在
					}
				}
				if(y==x)
				{
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:没有这个从节点信息\n",(u32)&__func__,(u32)__LINE__,0);
					#endif
					return 0;
				}
				if(d64b==0)//采集器地址都是0的时候,跳过这只,寻找下一只有效采集器地址
				{
					if((y+1)>=NUM_RMmax)
					{
						i=0;
					}else
					{
						i=y+1;
					}
					for(;i<NUM_RMmax;i++)
					{
						d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
						if(d64b!=0)
						{
							MWR(d64b,(u32)p8tx+2,6);
							break;
						}
					}
					if(i>=NUM_RMmax)
					{
						for(i=0;i<y;i++)
						{
							d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
							if(d64b!=0)
							{
								MWR(d64b,(u32)p8tx+2,6);
								break;
							}
						}
						if(i>=y)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:没有一只电表下面有有效采集器地址\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							return 0;
						}
					}
				}
				#ifdef DEBUG_JSON
				myprintf("[%s:%d]:系统当前正在抄读采集器地址\n",(u32)&__func__,(u32)__LINE__,0);
				for(i=6;i>0;i--)
				{
					myprintf("%02x",NoteList->List[y].CollectorAddr[i-1],0,0);
				}
				myprintf("\n",0,0,0);
				#endif
#endif
				switch(UARTCtrl->res23)
				{
					case 0://开始点抄
						p8tx[14]=7;//操作一个对象方法
						p8tx[15]=1;//
						//UARTCtrl->PIID++;
						//p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						p8tx[16]=0;//PIID写死
						Byte=17;
						
						p8tx[17]=0x60;
						p8tx[18]=0x43;
						p8tx[19]=0x7f;
						p8tx[20]=0;
						p8tx[21]=0x11;
						p8tx[22]=1;
						Byte+=6;
						break;
					case 1://上报若干个记录型对象属性，有后续帧
					case 2://上报若干个记录型对象属性，无后续帧
					case 10:
					case 11:
						//08 02 00				// ReportResponseRecordList
						//01					// 1个OAD
						//60 12 03 00			// 对应上报的OAD
						p8tx[0]=0x03;
						p8tx[14]=8;//上报若干个记录型对象属性响应帧
						p8tx[15]=2;//
						//p8tx[16]=UARTCtrl->PIID;
						p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						Byte=17;
						
						p8tx[17]=1;
						p8tx[18]=0x60;
						p8tx[19]=0x12;
						p8tx[20]=0x03;
						p8tx[21]=0;
						Byte+=5;					
						break;
					case 3://ReportNotificationSimplifyRecord上报方式，有后续帧
					case 8:
						//08 06 		// ReportResponseSimplifyRecord
						//02 
						p8tx[0]=0x03;
						p8tx[14]=8;//上报若干个记录型对象属性响应帧
						p8tx[15]=6;//
						//p8tx[16]=UARTCtrl->PIID;
						p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						Byte=17;
						break;
					case 4:///ReportNotificationSimplifyRecord上报方式，无后续帧
					case 9:
						p8tx[14]=7;//操作一个对象方法
						p8tx[15]=1;//
						//UARTCtrl->PIID++;
						p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						//p8tx[16]=UARTCtrl->PIID;
						Byte=17;
						
						p8tx[17]=0x60;
						p8tx[18]=0x43;
						p8tx[19]=0x7f;
						p8tx[20]=0;
						p8tx[21]=0x11;
						p8tx[22]=0;
						Byte+=6;
						break;
					case 5://采集器无数据响应
						break;
					default:
						break;
				}
				
				
				p8tx[Byte]=0;
				Byte+=1;
			//安全模式读取
				p8tx-=3;//空出的3字节
				x=Byte-14;//APDU长度
				if((p8tx[4]&0x30)!=0)//逻辑地址不为0，表示操作对应逻辑设备，采集器或者STA
				{//明文读取
					for(i=0;i<x;i++)
					{
						p8tx[14+i]=p8tx[17+i];
					}
				}
				else
				{//明文+数据验证码读取
				#if RM_SECURITY_MeterNo==0//0=否,1=是;DL698安全模式抄表时先抄读电能表表号(电能表表号用于计校验码)
					UARTCtrl->RM_Rx_date_40020200[0]=2;//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
				#else
					if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
						UARTCtrl->RM_Rx_date_40020200[0]=2;
				#endif
					if(UARTCtrl->RM_Rx_date_40020200[0]!=2)//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
					{//先用明文读电能表表号
						UARTCtrl->RM_Rx_date_40020200[0]=1;//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
						p8tx[14]=5;//GET-Request
						p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
						p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
						Byte=17;
						//OAD
						p8CSD[1]=0x00;
						p8CSD[2+0]=(RM_SECURITY_MeterNo_OAD>>24)&0xff;//电能表表号 或 电能表ESAM序列号
						p8CSD[2+1]=(RM_SECURITY_MeterNo_OAD>>16)&0xff;
						p8CSD[2+2]=(RM_SECURITY_MeterNo_OAD>>8)&0xff;
						p8CSD[2+3]=RM_SECURITY_MeterNo_OAD&0xff;
						for(i=0;i<4;i++)
						{
							p8tx[Byte+i]=p8CSD[2+i];
						}
						Byte+=4;
					//时间标签
						p8tx[Byte]=0;
						Byte+=1;
					//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
					//结束字符（16H）
						Byte+=3;
						Byte-=2;
						p8tx[1]=Byte;
						p8tx[2]=Byte>>8;
						Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
						return Byte+2;
					}
					p8tx[14]=0x10;
					p8tx[15]=0;//明文应用数据单元 [0] 密文应用数据单元 [1]
					p8tx[16]=x;//octet-string长度
					Byte+=3;
					p8tx[Byte]=1;//随机数 [1]  RN
					p8tx[Byte+1]=16;//长度
					Byte+=2;
					i=TESAM_CMD_TxRx(0x80040010,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
					if(i)
					{
						UART7Ctrl->Task=0;//关电源重启
					}
					MR((u32)p8tx+Byte,ADDR_UART7Rx+5,16);
					MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//抄表安全模式时发送给电表的16字节随机数
					Byte+=16;
				}
			//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
			//结束字符（16H）
				Byte+=3;
				Byte-=2;
				p8tx[1]=Byte;
				p8tx[2]=Byte>>8;
				Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误		
				return Byte+2;
				
			}

			break;
	#else
			default:
				break;
	#endif
		case RMTASKnoPOWER://停上电事件固定任务号
			p8tx[14]=5;//GET-Request
		#if (USER/100)==15//安徽
			p8tx[15]=3;
		#else
			p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
		#endif
			UARTCtrl->PIID++;
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
		#if (USER/100)==15//安徽
			p8tx[Byte]=9;//RSD， 选择方法9:Selector9为指定选取上第n次记录
			p8tx[Byte+1]=1;//上第1次记录  unsigned
			NUM_RCSD=UARTCtrl->RMCSD[6];
			p8tx[Byte+2]=NUM_RCSD;
			for(i=0;i<NUM_RCSD;i++)
			{
				p8tx[Byte+3+i*5]=0;
				p8tx[Byte+4+i*5]=p8CSD[7+i*4];
				p8tx[Byte+5+i*5]=p8CSD[8+i*4];
				p8tx[Byte+6+i*5]=p8CSD[9+i*4];
				p8tx[Byte+7+i*5]=p8CSD[10+i*4];
			}
			//p8tx[Byte+2]=0;//RCSD
			//Byte+=3;
			Byte+=3+NUM_RCSD*5;
		#endif
		//时间标签
			p8tx[Byte]=0;
			Byte+=1;
		//安全模式读取
			p8tx-=3;//空出的3字节
			x=Byte-14;//APDU长度
			i=(p8CSD[2]<<8)|p8CSD[3];
			i=RM_SECURITY(i);//抄表安全模式字,入口:OI,出口:(同安全请求)b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
			if(i==0)
			{//明文读取
				for(i=0;i<x;i++)
				{
					p8tx[14+i]=p8tx[17+i];
				}
			}
			else
			{//明文+数据验证码读取
			#if RM_SECURITY_MeterNo==0//0=否,1=是;DL698安全模式抄表时先抄读电能表表号(电能表表号用于计校验码)
				UARTCtrl->RM_Rx_date_40020200[0]=2;//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
			#else
					if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
						UARTCtrl->RM_Rx_date_40020200[0]=2;
			#endif
				if(UARTCtrl->RM_Rx_date_40020200[0]!=2)//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
				{//先用明文读电能表表号
					UARTCtrl->RM_Rx_date_40020200[0]=1;//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
					p8tx[14]=5;//GET-Request
					p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
					p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
					Byte=17;
					//OAD
					p8CSD[1]=0x00;
					p8CSD[2+0]=(RM_SECURITY_MeterNo_OAD>>24)&0xff;//电能表表号 或 电能表ESAM序列号
					p8CSD[2+1]=(RM_SECURITY_MeterNo_OAD>>16)&0xff;
					p8CSD[2+2]=(RM_SECURITY_MeterNo_OAD>>8)&0xff;
					p8CSD[2+3]=RM_SECURITY_MeterNo_OAD&0xff;
					for(i=0;i<4;i++)
					{
						p8tx[Byte+i]=p8CSD[2+i];
					}
					Byte+=4;
				//时间标签
					p8tx[Byte]=0;
					Byte+=1;
				//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
				//结束字符（16H）
					Byte+=3;
					Byte-=2;
					p8tx[1]=Byte;
					p8tx[2]=Byte>>8;
					Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
					return Byte+2;
				}
				p8tx[14]=0x10;
				p8tx[15]=0;//明文应用数据单元 [0] 密文应用数据单元 [1]
				p8tx[16]=x;//octet-string长度
				Byte+=3;
				p8tx[Byte]=1;//随机数 [1]  RN
				p8tx[Byte+1]=16;//长度
				Byte+=2;
				i=TESAM_CMD_TxRx(0x80040010,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
				if(i)
				{
					UART7Ctrl->Task=0;//关电源重启
				}
				MR((u32)p8tx+Byte,ADDR_UART7Rx+5,16);
				MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//抄表安全模式时发送给电表的16字节随机数
				Byte+=16;
			}
		//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
		//结束字符（16H）
			Byte+=3;
			Byte-=2;
			p8tx[1]=Byte;
			p8tx[2]=Byte>>8;
			Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
			return Byte+2;
			
//			//05 03 0E 31 0F 02 00 09 01 03 00 20 22 02 00 00 20 1E 02 00 00 20 20 02 00 00
//			p8tx[14]=5;//GET-Request
//			p8tx[15]=3;//读取一个记录型对象属性请求       [3] GetRequestRecord
//			UARTCtrl->PIID++;
//			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
//			Byte=17;
//			//OAD
//			for(i=0;i<4;i++)
//			{
//				p8tx[Byte+i]=p8CSD[2+i];
//			}
//			Byte+=4;
//			//选择方法
//			p8tx[Byte]=9;//RSD， 选择方法9:Selector9为指定选取上第n次记录
//			p8tx[Byte+1]=UARTCtrl->RM_DI_N;//上第n次记录  unsigned
//			Byte+=2;
//			//RCSD，SEQUENCE OF个数
//			p8tx[Byte]=0;//当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”
//			Byte++;
//		//时间标签
//			p8tx[Byte]=0;
//		//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
//		//结束字符（16H）
//			Byte+=4;
//			
//			Byte-=2;
//			p8tx[1]=Byte;
//			p8tx[2]=Byte>>8;
//			Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
//			return Byte+2;


//		default:
//			break;
	}

	switch((i>>24)&0xf)//方案类型
	{
		case 1://普通采集方案
		#if (USER/100)==17//江苏系
			if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
			//if(((UARTCtrl->TaskID>>20)&0xf)==5)
				p8CSD[1]=1;
		#endif
			p8tx[14]=5;//GET-Request
			if(p8CSD[1]==0)
			{//OAD
				p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
				UARTCtrl->PIID++;
				p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
				Byte=17;
				for(i=0;i<4;i++)
				{
					p8tx[Byte+i]=p8CSD[2+i];
				}
				Byte+=4;
			}
			else
			{//ROAD
/*
				//先判单相表曲线
				i=UARTCtrl->RMCSD[2+0]<<24;
				i|=UARTCtrl->RMCSD[2+1]<<16;
				i|=UARTCtrl->RMCSD[2+2]<<8;
				i|=UARTCtrl->RMCSD[2+3];
				if(i==0x50020200)
				{//分钟冻结
					i=UARTCtrl->TaskID;
					i=(i>>8)&0xff;//任务号
					i=UARTCtrl->MeterCount[i];//取档案号
					p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
					p8=Get_Element(p8,2,8,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{
						i=p8[1];//取接线方式
						if(i==1)//未知（0），单相（1），三相三线（2），三相四线（3）
						{//单相表曲线,抄实时数据
							p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
							UARTCtrl->PIID++;
							p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
							Byte=17;
							i=UARTCtrl->RM_DI;
							p8tx[Byte+0]=i>>24;
							p8tx[Byte+1]=i>>16;
							p8tx[Byte+2]=i>>8;
							p8tx[Byte+2]=i>>0;
							Byte+=4;
							break;
						}
					}
				}
*/
				p8tx[15]=3;//读取一个记录型对象属性请求       [3] GetRequestRecord
				UARTCtrl->PIID++;
				p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
				Byte=17;
				//OAD
				for(i=0;i<4;i++)
				{
					p8tx[Byte+i]=p8CSD[2+i];
				}
				Byte+=4;
				switch((UARTCtrl->TaskID>>20)&0xf)//采集类型
				{
					default:
					case 0://0=采集当前数据
		//				p8tx[Byte]=0;//RSD， 选择方法0:不选择       [0]  NULL
		//				Byte+=1;
		//				break;
					case 1://1=采集上第N次
						p8tx[Byte]=9;//RSD， 选择方法9:Selector9为指定选取上第n次记录
						p8tx[Byte+1]=UARTCtrl->RM_DI_N;//上第n次记录  unsigned
						Byte+=2;
						break;
					case 2://2=按冻结时标采集
						p8tx[Byte]=1;//RSD， 选择方法1
						p8tx[Byte+1]=0x20;//OAD，数据冻结时间
						p8tx[Byte+2]=0x21;
						p8tx[Byte+3]=0x02;
						p8tx[Byte+4]=0x00;
						p8tx[Byte+5]=DataType_date_time_s;//date_time_s
						Byte+=6;
						for(i=0;i<4;i++)//时间:年年月日
						{
							p8tx[Byte+i]=Comm_Ram->DL698YMDHMS[i];
						}
						p8tx[Byte+4]=0x00;//时间:时
						p8tx[Byte+5]=0x00;//时间:分
						p8tx[Byte+6]=0x00;//时间:秒
						i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//采集数据的OI
						switch(i)
						{
							case 0x5004://日冻结
								break;
							case 0x5006://月冻结
								p8tx[Byte+3]=0x01;//固定为1日
								break;
						}
						Byte+=7;
						break;
					case 3://3=按时标间隔采集
						//采曲线数据
						p8tx[Byte]=1;//RSD， 选择方法1
						p8tx[Byte+1]=0x20;//OAD，数据冻结时间
						p8tx[Byte+2]=0x21;
						p8tx[Byte+3]=0x02;
						p8tx[Byte+4]=0x00;
						p8tx[Byte+5]=DataType_date_time_s;//date_time_s
						Byte+=6;
						DateSaveTI(PORTn,p8tx+Byte);//普通采集方案的采集存储时标;返回:pOUT=date_time_s
						Byte+=7;
						break;
					case 4://4=RetryMetering 补抄(类同,2=按冻结时标采集)
						x=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//采集数据的OI
						switch(x)
						{
							case 0x5002://分钟冻结(补抄曲线数据)
								p8tx[Byte]=1;//RSD， 选择方法1
								p8tx[Byte+1]=0x20;//OAD，数据冻结时间
								p8tx[Byte+2]=0x21;
								p8tx[Byte+3]=0x02;
								p8tx[Byte+4]=0x00;
								p8tx[Byte+5]=DataType_date_time_s;//date_time_s
								Byte+=6;
								DateSaveTI(PORTn,p8tx+Byte);//普通采集方案的采集存储时标;返回:pOUT=date_time_s
								Byte+=7;
								break;
							case 0x5004://补抄日冻结
								p8tx[Byte]=1;//RSD， 选择方法1
								p8tx[Byte+1]=0x20;//OAD，数据冻结时间
								p8tx[Byte+2]=0x21;
								p8tx[Byte+3]=0x02;
								p8tx[Byte+4]=0x00;
								p8tx[Byte+5]=DataType_date_time_s;//date_time_s
								Byte+=6;
								YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
								//i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
								#if (USER/100)==17
								myprintf("[%s:%d]:补抄\n",(u32)&__func__,(u32)__LINE__,0);
								if(UARTCtrl->RM_DI_TI_Count==0)
								{
									UARTCtrl->RM_DI_TI_Count=1;
								}
								i=(UARTCtrl->RM_DI_TI_Count-1);
								#else
								i=UARTCtrl->RM_DI_TI_Count;
								#endif
								while(i--)
								{
									YYMD=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
								}
								p8tx[Byte+0]=YYMD>>24;
								p8tx[Byte+1]=YYMD>>16;
								p8tx[Byte+2]=YYMD>>8;
								p8tx[Byte+3]=YYMD>>0;
								p8tx[Byte+4]=0x00;//时间:时
								p8tx[Byte+5]=0x00;//时间:分
								p8tx[Byte+6]=0x00;//时间:秒
								Byte+=7;
								break;
							case 0x5006://月冻结
							default:
								return 0;
						}
						break;
				}
//			#if (USER/100)==17//江苏系
//				x=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//采集数据的OI
//				switch(x)
//				{
//					case 0x5005://抄表日冻结
//						p8tx[15]=2;//读取一个对象列表属性请求             [2] GetRequestNormalList
//						Byte-=7;
//						x=p8CSD[6];//RCSD，SEQUENCE OF个数
//						p8tx[Byte]=x;
//						Byte++;
//						//Byte-=3;
//						for(i=0;i<x;i++)
//						{
//							p8tx[Byte]=p8CSD[7+(i*4)+0];//OAD
//							p8tx[Byte+1]=p8CSD[7+(i*4)+1];//OAD
//							p8tx[Byte+2]=p8CSD[7+(i*4)+2];//OAD
//							p8tx[Byte+3]=p8CSD[7+(i*4)+3];//OAD
//							Byte+=4;
//						}
//						break;
//					default:
//						if((((UARTCtrl->TaskID>>20)&0xf)!=5)||(Terminal_Router->MinuteCollect!=1))
//						{
//							x=p8CSD[6];//RCSD，SEQUENCE OF个数
//							p8tx[Byte]=x;
//							Byte++;
//							for(i=0;i<x;i++)
//							{
//								p8tx[Byte+0]=0;//类型=OAD
//								p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
//								p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
//								p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
//								p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
//								Byte+=5;
//							}	
//						}
//						break;
//						
//				}
//			#else
				x=p8CSD[6];//RCSD，SEQUENCE OF个数
				p8tx[Byte]=x;
				Byte++;
				for(i=0;i<x;i++)
				{
					p8tx[Byte+0]=0;//类型=OAD
					p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
					p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
					p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
					p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
					Byte+=5;
				}
//			#endif
//				x=p8CSD[6];//RCSD，SEQUENCE OF个数
//				p8tx[Byte]=x;
//				Byte++;
//				for(i=0;i<x;i++)
//				{
//					p8tx[Byte+0]=0;//类型=OAD
//					p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
//					p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
//					p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
//					p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
//					Byte+=5;
//				}
			}
			break;
		case 2://事件采集方案
			//05 03 0E 31 0F 02 00 09 01 03 00 20 22 02 00 00 20 1E 02 00 00 20 20 02 00 00
			p8tx[14]=5;//GET-Request
			p8tx[15]=3;//读取一个记录型对象属性请求       [3] GetRequestRecord
			UARTCtrl->PIID++;
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
			//选择方法
			p8tx[Byte]=9;//RSD， 选择方法9:Selector9为指定选取上第n次记录
			p8tx[Byte+1]=1;//上第n次记录  unsigned
			Byte+=2;
			//RCSD，SEQUENCE OF个数
//			p8CSD[5]=0;//RCSD，SEQUENCE OF个数//!!!测试台模拟表不支持SEQUENCE OF个数=0抄表
//			p8tx[Byte]=0;
//			Byte++;
			x=p8CSD[6];//RCSD，SEQUENCE OF个数
			p8tx[Byte]=x;
			Byte++;
			for(i=0;i<x;i++)
			{
				p8tx[Byte+0]=0;//类型=OAD
				p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
				p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
				p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
				p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
				Byte+=5;
			}
			break;
		case 3://透明方案
			
			return 0;
		default:
			return 0;
	}
	
//时间标签
	p8tx[Byte]=0;
	Byte+=1;
//安全模式读取
	p8tx-=3;//空出的3字节
	x=Byte-14;//APDU长度
	i=(p8CSD[2]<<8)|p8CSD[3];
	i=RM_SECURITY(i);//抄表安全模式字,入口:OI,出口:(同安全请求)b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
	if(i==0)
	{//明文读取
		for(i=0;i<x;i++)
		{
			p8tx[14+i]=p8tx[17+i];
		}
	}
	else
	{//明文+数据验证码读取
	#if RM_SECURITY_MeterNo==0//0=否,1=是;DL698安全模式抄表时先抄读电能表表号(电能表表号用于计校验码)
		UARTCtrl->RM_Rx_date_40020200[0]=2;//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
	#else
		if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
			UARTCtrl->RM_Rx_date_40020200[0]=2;
	#endif
		if(UARTCtrl->RM_Rx_date_40020200[0]!=2)//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
		{//先用明文读电能表表号
			UARTCtrl->RM_Rx_date_40020200[0]=1;//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
			p8tx[14]=5;//GET-Request
			p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			p8CSD[1]=0x00;
			p8CSD[2+0]=(RM_SECURITY_MeterNo_OAD>>24)&0xff;//电能表表号 或 电能表ESAM序列号
			p8CSD[2+1]=(RM_SECURITY_MeterNo_OAD>>16)&0xff;
			p8CSD[2+2]=(RM_SECURITY_MeterNo_OAD>>8)&0xff;
			p8CSD[2+3]=RM_SECURITY_MeterNo_OAD&0xff;
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
		//时间标签
			p8tx[Byte]=0;
			Byte+=1;
		//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
		//结束字符（16H）
			Byte+=3;
			Byte-=2;
			p8tx[1]=Byte;
			p8tx[2]=Byte>>8;
			Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
			return Byte+2;
		}
		p8tx[14]=0x10;
		p8tx[15]=0;//明文应用数据单元 [0] 密文应用数据单元 [1]
		p8tx[16]=x;//octet-string长度
		Byte+=3;
		p8tx[Byte]=1;//随机数 [1]  RN
		p8tx[Byte+1]=16;//长度
		Byte+=2;
		i=TESAM_CMD_TxRx(0x80040010,0,0);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
		if(i)
		{
			UART7Ctrl->Task=0;//关电源重启
		}
		MR((u32)p8tx+Byte,ADDR_UART7Rx+5,16);
		MR((u32)&UARTCtrl->Rand,ADDR_UART7Rx+5,16);//抄表安全模式时发送给电表的16字节随机数
		Byte+=16;
	}
//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
//结束字符（16H）
	Byte+=3;
	Byte-=2;
	p8tx[1]=Byte;
	p8tx[2]=Byte>>8;
	Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	return Byte+2;
}

/*
u32 RM_TxDATA_DL698(u32 PORTn,u8 *p8CSD,u8 *p8tx)//抄表发送帧;入口:p8CSD=需发送的CSD指针,p8tx=发送缓冲指针;返回:组帧后总长度字节数
{
	u32 i;
	u32 x;
	u32 Byte;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//起始字符（68H）  1Byte
	p8tx[0]=0x68;
//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
//控制域C  1Byte
	p8tx[3]=0x43;
//地址域A
	p8tx[4]=5;
	p8tx[5]=UARTCtrl->RMAddr[2]>>8;
	p8tx[6]=UARTCtrl->RMAddr[2];
	p8tx[7]=UARTCtrl->RMAddr[1]>>8;
	p8tx[8]=UARTCtrl->RMAddr[1];
	p8tx[9]=UARTCtrl->RMAddr[0]>>8;
	p8tx[10]=UARTCtrl->RMAddr[0];
//客户机地址CA  1Byte
	p8tx[11]=0;
//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
//APDU
	i=UARTCtrl->TaskID;
	switch((i>>8)&0xff)//任务号
	{
		case RMTASKnoPOWER://停上电事件固定任务号
			p8tx[14]=5;//GET-Request
			p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
			UARTCtrl->PIID++;
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
		//时间标签
			p8tx[Byte]=0;
		//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
		//结束字符（16H）
			Byte+=4;
			
			Byte-=2;
			p8tx[1]=Byte;
			p8tx[2]=Byte>>8;
			Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
			return Byte+2;
			
//			//05 03 0E 31 0F 02 00 09 01 03 00 20 22 02 00 00 20 1E 02 00 00 20 20 02 00 00
//			p8tx[14]=5;//GET-Request
//			p8tx[15]=3;//读取一个记录型对象属性请求       [3] GetRequestRecord
//			UARTCtrl->PIID++;
//			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
//			Byte=17;
//			//OAD
//			for(i=0;i<4;i++)
//			{
//				p8tx[Byte+i]=p8CSD[2+i];
//			}
//			Byte+=4;
//			//选择方法
//			p8tx[Byte]=9;//RSD， 选择方法9:Selector9为指定选取上第n次记录
//			p8tx[Byte+1]=UARTCtrl->RM_DI_N;//上第n次记录  unsigned
//			Byte+=2;
//			//RCSD，SEQUENCE OF个数
//			p8tx[Byte]=0;//当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”
//			Byte++;
//		//时间标签
//			p8tx[Byte]=0;
//		//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
//		//结束字符（16H）
//			Byte+=4;
//			
//			Byte-=2;
//			p8tx[1]=Byte;
//			p8tx[2]=Byte>>8;
//			Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
//			return Byte+2;
		default:
			break;
	}
	switch((i>>24)&0xf)//方案类型
	{
		case 1://普通采集方案
			p8tx[14]=5;//GET-Request
			if(p8CSD[1]==0)
			{//OAD
				p8tx[15]=1;//读取一个对象属性请求             [1] GetRequestNormal
				UARTCtrl->PIID++;
				p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
				Byte=17;
				for(i=0;i<4;i++)
				{
					p8tx[Byte+i]=p8CSD[2+i];
				}
				Byte+=4;
			}
			else
			{//ROAD
				p8tx[15]=3;//读取一个记录型对象属性请求       [3] GetRequestRecord
				UARTCtrl->PIID++;
				p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
				Byte=17;
				//OAD
				for(i=0;i<4;i++)
				{
					p8tx[Byte+i]=p8CSD[2+i];
				}
				Byte+=4;
				switch((UARTCtrl->TaskID>>20)&0xf)//采集类型
				{
					default:
					case 0://0=采集当前数据
		//				p8tx[Byte]=0;//RSD， 选择方法0:不选择       [0]  NULL
		//				Byte+=1;
		//				break;
					case 1://1=采集上第N次
						p8tx[Byte]=9;//RSD， 选择方法9:Selector9为指定选取上第n次记录
						p8tx[Byte+1]=UARTCtrl->RM_DI_N;//上第n次记录  unsigned
						Byte+=2;
						break;
					case 2://2=按冻结时标采集
						p8tx[Byte]=1;//RSD， 选择方法1
						p8tx[Byte+1]=0x20;//OAD，数据冻结时间
						p8tx[Byte+2]=0x21;
						p8tx[Byte+3]=0x02;
						p8tx[Byte+4]=0x00;
						p8tx[Byte+5]=DataType_date_time_s;//date_time_s
						Byte+=6;
						for(i=0;i<4;i++)//时间:年年月日
						{
							p8tx[Byte+i]=Comm_Ram->DL698YMDHMS[i];
						}
						p8tx[Byte+4]=0x00;//时间:时
						p8tx[Byte+5]=0x00;//时间:分
						p8tx[Byte+6]=0x00;//时间:秒
						i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//采集数据的OI
						switch(i)
						{
							case 0x5004://日冻结
								break;
							case 0x5006://月冻结
								p8tx[Byte+3]=0x01;//固定为1日
								break;
						}
						Byte+=7;
						break;
					case 3://3=按时标间隔采集
						//采曲线数据
						p8tx[Byte]=1;//RSD， 选择方法1
						p8tx[Byte+1]=0x20;//OAD，数据冻结时间
						p8tx[Byte+2]=0x21;
						p8tx[Byte+3]=0x02;
						p8tx[Byte+4]=0x00;
						p8tx[Byte+5]=DataType_date_time_s;//date_time_s
						Byte+=6;
						DateSaveTI(PORTn,p8tx+Byte);//普通采集方案的采集存储时标;返回:pOUT=date_time_s
						Byte+=7;
						break;
					case 4://4=RetryMetering 补抄
						
						break;
				}
				x=p8CSD[6];//RCSD，SEQUENCE OF个数
				p8tx[Byte]=x;
				Byte++;
				for(i=0;i<x;i++)
				{
					p8tx[Byte+0]=0;//类型=OAD
					p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
					p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
					p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
					p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
					Byte+=5;
				}
			}
			break;
		case 2://事件采集方案
			//05 03 0E 31 0F 02 00 09 01 03 00 20 22 02 00 00 20 1E 02 00 00 20 20 02 00 00
			p8tx[14]=5;//GET-Request
			p8tx[15]=3;//读取一个记录型对象属性请求       [3] GetRequestRecord
			UARTCtrl->PIID++;
			p8tx[16]=UARTCtrl->PIID&0x3f;//PIID
			Byte=17;
			//OAD
			for(i=0;i<4;i++)
			{
				p8tx[Byte+i]=p8CSD[2+i];
			}
			Byte+=4;
			//选择方法
			p8tx[Byte]=9;//RSD， 选择方法9:Selector9为指定选取上第n次记录
			p8tx[Byte+1]=1;//上第n次记录  unsigned
			Byte+=2;
			//RCSD，SEQUENCE OF个数
//			p8CSD[5]=0;//RCSD，SEQUENCE OF个数//!!!测试台模拟表不支持SEQUENCE OF个数=0抄表
//			p8tx[Byte]=0;
//			Byte++;
			x=p8CSD[6];//RCSD，SEQUENCE OF个数
			p8tx[Byte]=x;
			Byte++;
			for(i=0;i<x;i++)
			{
				p8tx[Byte+0]=0;//类型=OAD
				p8tx[Byte+1]=p8CSD[7+(i*4)+0];//OAD
				p8tx[Byte+2]=p8CSD[7+(i*4)+1];//OAD
				p8tx[Byte+3]=p8CSD[7+(i*4)+2];//OAD
				p8tx[Byte+4]=p8CSD[7+(i*4)+3];//OAD
				Byte+=5;
			}
			break;
		case 3://透明方案
			
			return 0;
		default:
			return 0;
	}
	
//时间标签
	p8tx[Byte]=0;
//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
//结束字符（16H）
	Byte+=4;
	
	Byte-=2;
	p8tx[1]=Byte;
	p8tx[2]=Byte>>8;
	Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	return Byte+2;
}
*/



//698广播校时
//68 1A 00 43 C0 AA 00 5E F7 07 01 00 40 00 7F 00 1C 07 E1 06 05 0A 18 0C 00 EC 67 16
//698广播单地址校时
//地址为 01 00 10 56 93 58
//68 1F 00 43 05 58 93 56 10 00 01 00 17 B6 07 01 00 40 00 7F 00 1C 07 E1 06 05 0A 33 1A 00 30 8C 16

u32 BroadcastTime_TxDATA_DL698(u8 *p8tx)//DL698广播校时发送帧;返回:组帧后总长度字节数
{
	u32 Byte;
	
//起始字符（68H）  1Byte
	p8tx[0]=0x68;
//长度域L  2Byte(除起始字符和结束字符之外的帧字节数)
//控制域C  1Byte
	p8tx[3]=0x43;
//地址域A
	p8tx[4]=0xC0;
	p8tx[5]=0xAA;
//客户机地址CA  1Byte
	p8tx[6]=0;
//帧头校验HCS  2Byte(帧头校验HCS为2字节，是对帧头部分除起始字符和HCS本身之外的所有字节的校验)
//APDU
	p8tx[9]=0x07;
	p8tx[10]=1;
	p8tx[11]=0;//PIID
	p8tx[12]=0x40;//OAD
	p8tx[13]=0x00;
	p8tx[14]=0x7F;
	p8tx[15]=0;
	p8tx[16]=DataType_date_time_s;
	MR((u32)p8tx+17,ADDR_DL698YMDHMS,7);
	Byte=24;
	
//时间标签
	p8tx[Byte]=0;
//帧校验FCS  2Byte(是对整帧除起始字符、结束字符和FCS本身之外的所有字节的校验)
//结束字符（16H）
	Byte+=4;
	
	Byte-=2;
	p8tx[1]=Byte;
	p8tx[2]=Byte>>8;
	Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	return Byte+2;
}








