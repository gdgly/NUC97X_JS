
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_TESAM.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"

#include "../DL698/DL698_ACTION_EVENT.h"
#include "../DL698/DL698_ACTION_4000.h"
#include "../DL698/DL698_ACTION_5000.h"
#include "../DL698/DL698_ACTION_6000.h"
#include "../DL698/DL698_ACTION_8000.h"
#include "../DL698/DL698_ACTION_F000.h"
#include "../DL698/DL698_FILE.h"
#include "../DL698/DL698_GET_SET_ACTION_2000.h"
#include "../Device/IC_BUZZ.h"
#include "../DL698/DL698_FactoryTest.h"


//ACTION-Request∷=CHOICE
//{
//方法请求                	     [1] ActionRequest，
//操作若干个对象方法请求                  	     [2] ActionRequestList，
//操作若干个对象方法后读取若干个对象属性请求   [3] ActionThenGetRequestNormalList
//}
//ACTION-Response∷=CHOICE
//{
//方法的响应                	 [1] ActionResponseNormal，
//操作若干个对象方法的响应                	 [2] ActionResponseNormalList，
//操作若干个对象方法后读取若干个属性的响应	 [3] ActionThenGetResponseNormalList
//}

void ACTION_Request(u32 PORTn)
{
	u32 i;
	u32 x;
	u32 LEN_SA;//服务器地址SA的长度
	u32 TI;//时间标签:0=无,1=有
	u32 LEN_RxAPDU;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//最大还可发送的字节数
	u32 LEN_OAD;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	u32 OAD;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息

	BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
#else
	//客户机接收缓冲长度
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//不为0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LEN_RxAPDU=p8rx[1]+(p8rx[2]<<8);//除起始字符和结束字符之外的帧字节数

	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
	p8tx[0]=135;//操作响应        	[135]	ACTION-Response
	switch(p8rx[LEN_SA+8+1])
	{
		case 1://方法请求                	     [1] ActionRequest
//ActionRequest∷=SEQUENCE
//{
//  服务序号-优先级      PIID，
//  一个对象方法描述符   OMD，
//  方法参数             Data
//}
//ActionResponseNormal∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  一个对象方法描述符    OMD，
//  操作执行结果          DAR，
//  操作返回数据          Data  OPTIONAL
//}
			p8tx[1]=1;//方法的响应                	 [1] ActionResponseNormal，
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			p8rx+=LEN_SA+8+3;
			p8tx[LEN_TxAPDU+0]=p8rx[0];//OMD
			p8tx[LEN_TxAPDU+1]=p8rx[1];//OMD
			p8tx[LEN_TxAPDU+2]=p8rx[2];//OMD
			p8tx[LEN_TxAPDU+3]=p8rx[3];//OMD
			p8tx[LEN_TxAPDU+4]=0;//DAR
			p8tx[LEN_TxAPDU+5]=0;//Data OPTIONAL=0 表示没有数据
			LEN_TxAPDU+=5;//6;
			LENmax_TxSPACE-=5;//6;
			LEN_OAD=ACTION(PORTn,p8rx,LEN_RxAPDU,p8tx+LEN_TxAPDU,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			if(UARTCtrl->DAR!=0)//数据访问结果
			{
				if(UARTCtrl->DAR==254)//不响应无返回帧(扩展）   254
				{
					LEN_TxAPDU=0;
					break;
				}
				p8tx[LEN_TxAPDU-1]=UARTCtrl->DAR;
			}
			if((LEN_OAD&0xffff)==0)
			{//没数据返回
				//p8tx[LEN_TxAPDU-1]=1;//Data OPTIONAL=1 表示有数据
				LEN_TxAPDU+=1;
			}
			else
			{
				LEN_TxAPDU+=LEN_OAD&0xffff;
			}
			break;
		case 2://操作若干个对象方法请求                  	     [2] ActionRequestList
//ActionRequestList∷=SEQUENCE
//{
//  服务序号-优先级  PIID，
//  若干个对象属性   SEQUENCE OF
//  {
//一个对象方法描述符  OMD，
//    方法参数            Data
//  }
//}
//ActionResponseNormalList∷=SEQUENCE
//{
//  服务序号-优先级-ACD     PIID-ACD，
//  若干个对象方法操作结果  SEQUENCE OF
//  {
//一个对象方法描述符   OMD，
//     操作执行结果         DAR，
//     操作返回数据         Data  OPTIONAL
//}
//}
			p8tx[1]=2;//操作若干个对象方法的响应                	 [2] ActionResponseNormalList，
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			p8tx[3]=0;//(SEQUENCE OF)
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=4;
			
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			LEN_RxAPDU-=1;
			p8rx+=LEN_SA+8+4;
			while(x--)
			{
				if(LENmax_TxSPACE<6)
				{
					break;//发送缓冲不够
				}
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OMD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OMD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OMD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OMD
				p8tx[LEN_TxAPDU+4]=0;//DAR
				p8tx[LEN_TxAPDU+5]=0;//Data OPTIONAL=0 表示没有数据
				LEN_TxAPDU+=5;//6;
				LENmax_TxSPACE-=5;//6;
				LEN_OAD=ACTION(PORTn,p8rx,LEN_RxAPDU,p8tx+LEN_TxAPDU,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
				if(UARTCtrl->DAR!=0)//数据访问结果
				{
					p8tx[LEN_TxAPDU-1]=UARTCtrl->DAR;
				}
				if((LEN_OAD>>16)==0)
				{
					i=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
					LEN_OAD&=0xffff;
					LEN_OAD|=i<<16;
				}
				if((LEN_OAD&0xffff)==0)
				{//没有数据返回
					//p8tx[LEN_TxAPDU-1]=1;//Data OPTIONAL=1 表示有数据
					LEN_TxAPDU+=1;
				}
				else
				{
					LEN_TxAPDU+=LEN_OAD&0xffff;
				}
				p8tx[3]++;//+1(SEQUENCE OF)
				LEN_RxAPDU-=4+(LEN_OAD>>16);
				p8rx+=4+(LEN_OAD>>16);
			}
			break;
		case 3://操作若干个对象方法后读取若干个对象属性请求   [3] ActionThenGetRequestNormalList
//ActionThenGetRequestNormalList∷=SEQUENCE
//{
//  服务序号-优先级  PIID，
//  若干个操作对象方法后读取对象属性  SEQUENCE OF
//  {
//一个设置的对象方法描述符   OMD，
//方法参数                   Data，
//一个读取的对象属性描述符   OAD，
//读取延时                   unsigned
//  }
//}
//ActionThenGetResponseNormalList∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  操作若干个对象方法后读取属性的结果  SEQUENCE OF
//  {
//一个设置的对象方法描述符   OMD，
//     操作执行结果               DAR，
//     操作返回数据               Data OPTIONAL，
//     一个读取的对象属性描述符   OAD，
//     读取响应数据               Get-Result
//}
//}
			p8tx[1]=3;//设置后读取若干个对象属性请求	[3] SetThenGetRequestNormalList
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			p8tx[3]=0;//(SEQUENCE OF)
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=4;
			
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			LEN_RxAPDU-=1;
			p8rx+=LEN_SA+8+4;
			while(x--)
			{
				//设置1个OAD
				if(LENmax_TxSPACE<6)
				{
					break;//发送缓冲不够
				}
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OMD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OMD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OMD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OMD
				p8tx[LEN_TxAPDU+4]=0;//DAR
				p8tx[LEN_TxAPDU+5]=0;//Data OPTIONAL=0 表示没有数据
				LEN_TxAPDU+=5;//6;
				LENmax_TxSPACE-=5;//6;
				LEN_OAD=ACTION(PORTn,p8rx,LEN_RxAPDU,p8tx+LEN_TxAPDU,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
				if(UARTCtrl->DAR!=0)//数据访问结果
				{
					p8tx[LEN_TxAPDU-1]=UARTCtrl->DAR;
				}
				if((LEN_OAD>>16)==0)
				{
					i=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
					LEN_OAD&=0xffff;
					LEN_OAD|=i<<16;
				}
				if((LEN_OAD&0xffff)==0)
				{//没有数据返回
					//p8tx[LEN_TxAPDU-1]=1;//Data OPTIONAL=1 表示有数据
					LEN_TxAPDU+=1;
				}
				else
				{
					LEN_TxAPDU+=LEN_OAD&0xffff;
				}
				LEN_RxAPDU-=4+(LEN_OAD>>16);
				p8rx+=4+(LEN_OAD>>16);
				//读取
				if(LENmax_TxSPACE<6)
				{
					break;//发送缓冲不够
				}
				OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
				//延时时间固定1字节秒值
				LEN_TxAPDU+=4;
				LENmax_TxSPACE-=4;
				LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+1,LENmax_TxSPACE-1);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
				if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
				{
					break;
				}
				if(UARTCtrl->DAR!=0)//数据访问结果
				{
					p8tx[LEN_TxAPDU+0]=0;//Get-Result∷=CHOICE,错误信息  [0]   DAR
					p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
					LEN_TxAPDU+=2;
					LENmax_TxSPACE-=2;
				}
				else
				{
					p8tx[LEN_TxAPDU]=1;//Get-Result∷=CHOICE,[1]   DATA
					LEN_TxAPDU+=LEN_OAD+1;
					LENmax_TxSPACE-=LEN_OAD+1;
				}
				p8tx[3]++;//+1(SEQUENCE OF)
				p8rx+=4+1;//延时时间固定1字节秒值
				LEN_RxAPDU-=4+1;
			}
			break;
		default:
			LEN_TxAPDU=0;
			break;
	}
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
}

u32 ACTION(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
//OMD∷=SEQUENCE
//{
//对象标识  OI，
//  方法标识  unsigned（1…255），
//  操作模式  unsigned（0）
//}
//方法标识——即对象方法编号。
//操作模式——值默认为0。
	//u32 i;
	u32 OI;
	u32 Len;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;
	Len=0;
	OI=(p8rx[0]<<8)+p8rx[1];
	
	if((OI&0xf000)==0x3000)
	{
		Len=ACTION_EVENT(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
		return Len;
	}
	if(((OI>>8)==0x23)||((OI>>8)==0x24))
	{//脉冲输入，总加组
		Len=ACTION_23xx_24xx(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法总加组和脉冲计量;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
		return Len;
	}
	switch(OI)
	{
	#if (USER/100)==17//江苏系
		case 0x4048://上报实时数据
			Len=ACTION_4048(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//操作一个对象方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
	#endif
		case 0x4000://广播校时
			Len=ACTION_4000(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//操作一个对象方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x4006://时钟源
			//方法127：启用（）
			//方法128：禁用（）
			return 0;
		case 0x4200://路由信息单元
			Len=ACTION_4200(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x4300://电气设备
			Len=ACTION_4300(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x5000://冻结关联对象属性表
		case 0x5001:
		case 0x5002:
		case 0x5003:
		case 0x5004:
		case 0x5005:
		case 0x5006:
		case 0x5007:
		case 0x5008:
		case 0x5009:
		case 0x500A:
		case 0x500B:
		case 0x5011:
			Len=ACTION_5000(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x6000://采集档案配置表
			Len=ACTION_6000(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			Terminal_Ram->SET6000=0x55;//采集档案配置表有设置标志:0=没设置,0x55=设置
			return Len;
		case 0x6002://搜表
			Len=ACTION_6002(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x6012://任务配置表
			Len=ACTION_6012(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			Terminal_Ram->SETTask=0xff;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
			MR(ADDR_6012_2_SDRAM,ADDR_6012_2,LENmax_6012_2);//任务配置表FLASHtoSDRAM
			return Len;
		case 0x6014://普通采集方案
			Len=ACTION_6014(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			Terminal_Ram->SETTask=0xff;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
			MR(ADDR_6014_SDRAM,ADDR_6014,LENmax_6014);//普通采集方案FLASHtoSDRAM
			return Len;
		case 0x6016://事件采集方案
			Len=ACTION_6016(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			Terminal_Ram->SETTask=0xff;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
			MR(ADDR_6016_SDRAM,ADDR_6016,LENmax_6016);//事件采集方案FLASHtoSDRAM
			return Len;
		case 0x6018://透明方案
			Len=ACTION_6018(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			Terminal_Ram->SETTask=0xff;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
			return Len;
		case 0x601C://上报方案
			Len=ACTION_601C(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			MR(ADDR_601C_SDRAM,ADDR_601C,LENmax_601C);//上报方案FLASHtoSDRAM
			return Len;
		case 0x601E://采集规则库
			Len=ACTION_601E(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		
		case 0x8000://遥控
			Len=ACTION_8000(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x8001://保电
			Len=ACTION_8001(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x8002://催费告警
			Len=ACTION_8002(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x8103://时段功控
			Len=ACTION_8103(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x8104://厂休控
			Len=ACTION_8104(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x8105://营业报停控
			Len=ACTION_8105(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x8106://当前功率下浮控
			Len=ACTION_8106(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x8107://购电控
			Len=ACTION_8107(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0x8108://月电控
			Len=ACTION_8108(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		
		case 0xF001://文件分块传输管理
			Len=ACTION_F001(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF002://FTP传输管理
			Len=ACTION_F002(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF100://ESAM
			Len=ACTION_F100(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF101://安全模式参数
			Len=ACTION_F101(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		
		case 0xF200://RS232
			Len=ACTION_F200(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF201://RS485
			Len=ACTION_F201(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF202://红外
			Len=ACTION_F202(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF205://继电器输出
			Len=ACTION_F205(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF207://多功能端子
			Len=ACTION_F207(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF209://载波/微功率无线接口
			Len=ACTION_F209(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		case 0xF20B://蓝牙
			Len=ACTION_F20B(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		
		
		case 0xFFFF://工厂自测
			Len=ACTION_FFFF(PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
			return Len;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}


































