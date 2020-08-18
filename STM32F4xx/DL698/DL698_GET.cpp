

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_TESAM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_DATALIB.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET_iMeter.h"
#include "../DL698/DL698_GET_4100.h"
#include "../DL698/DL698_GET_6000.h"
#include "../DL698/DL698_GET_F000.h"
#include "../DL698/DL698_GET_EVENT.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../Calculate/Calculate.h"

#include "../DL698/CALC_SwitchIn.h"
#include "../DL698/DL698_FILE.h"
#include "../DL698/DL698_GET_SET_ACTION_2000.h"
#include "../DL698/DL698_CONNECT.h"
#include "../DL698/DL698_FactoryTest.h"

#include "../DL698/DL698_JSON.h"

//GET-Request∷=CHOICE
//{
//读取一个对象属性请求             [1] GetRequestNormal，
//读取若干个对象属性请求           [2] GetRequestNormalList，
//读取一个记录型对象属性请求       [3] GetRequestRecord，
//读取若干个记录型对象属性请求	  [4] GetRequestRecordList，
//读取分帧响应的下一个数据块请求	  [5] GetRequestNext
//}
//GET-Response∷=CHOICE
//{
//读取一个对象属性的响应         	[1]  GetResponseNormal，
//读取若干个对象属性的响应       	[2]  GetResponseNormalList，
//读取一个记录型对象属性的响应   	[3]  GetResponseRecord，
//读取若干个记录型对象属性的响应 	[4]  GetResponseRecordList，
//分帧响应一个数据块                	[5]  GetResponseNext
//}

void GET_Request(u32 PORTn)
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
	u8* pDATA;
	u32 OAD;
	ms_Type *ms;
	u16 *p16;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息

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

	if(p8rx[LEN_SA+8+1]!=5)
	{//不是读取分帧响应的下一个数据块请求	  [5] GetRequestNext
		UARTCtrl->GET_Reques_CHOICE=p8rx[LEN_SA+8+1];//原得到请求选择
		if(UARTCtrl->pTxData)//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
		}
	}
	UARTCtrl->PIID=p8rx[LEN_SA+8+2];//PIID-ACD
	i=p8rx[LEN_SA+8+1];
	switch(i)
	{
		case 1://读取一个对象属性请求             [1] GetRequestNormal，
//GetRequestNormal∷=SEQUENCE
//{
//  服务序号-优先级     PIID，
//  一个对象属性描述符  OAD
//}
//GetResponseNormal∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  一个对象属性及其结果  A-ResultNormal
//}
//A-ResultNormal∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  及其结果        Get-Result
//}
//Get-Result∷=CHOICE
//{
//  错误信息  [0]   DAR，
//  数据      [1]   Data
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=133;//读取响应         	[133]	GET-Response
			p8tx[1]=1;//[1] GetRequestNormal
			p8tx[2]=UARTCtrl->PIID;//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
			
			p8rx+=LEN_SA+8+3;
			//时间标签
			if(p8rx[4]==1)
			{//有时间标签
				i=Uart_TimeTag(p8rx+4+1);//时间标签有效性判断;入口:p8rx指向时间标签;返回:0=有效,1=无效
				if(i)
				{//无效
					p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
					p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
					p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
					p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
					p8tx[LEN_TxAPDU+4]=0;//Get-Result∷=CHOICE,错误信息  [0]   DAR
					p8tx[LEN_TxAPDU+5]=32;//时间标签无效(32)
					LEN_TxAPDU+=6;
					break;
				}
			}
			
			UARTCtrl->NEXT=0;//DL698分帧标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧
			UARTCtrl->NEXTBlock=1;//DL698分帧传输过程的帧序号，取值范围0~4095
			UARTCtrl->NEXTOADNum=0;//DL698分帧还需传输的OAD个数
			UARTCtrl->pRxBuff=(u32)p8rx;
			UARTCtrl->NEXTOADSubNum=0;//DL698分帧传输OAD内子计数
			
			OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
			p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
			p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
			p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
			p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
			LEN_TxAPDU+=4;
			LENmax_TxSPACE-=4;
			LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+1,LENmax_TxSPACE-1);
			if(UARTCtrl->DAR!=0)//数据访问结果
			{
				p8tx[LEN_TxAPDU+0]=0;//Get-Result∷=CHOICE,错误信息  [0]   DAR
				p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
				LEN_TxAPDU+=2;
			}
			else
			{
				p8tx[LEN_TxAPDU]=1;//Get-Result∷=CHOICE, [1]   DATA
				LEN_TxAPDU+=LEN_OAD+1;
			}
			break;
		case 2://读取若干个对象属性请求           [2] GetRequestNormalList，
//GetRequestNormalList∷=SEQUENCE
//{
//  服务序号-优先级       PIID，
//  若干个对象属性描述符  SEQUENCE OF OAD
//}
//GetResponseNormalList∷=SEQUENCE
//{
//  服务序号-优先级-ACD     PIID-ACD，
//  若干个对象属性及其结果  SEQUENCE OF A-ResultNormal
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=133;//读取响应         	[133]	GET-Response
			p8tx[1]=2;//读取若干个对象属性响应   	[2] 
			p8tx[2]=UARTCtrl->PIID;//PIID-ACD
			p8tx[3]=0;//SEQUENCE OF
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=LEN_TxAPDU;
		
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			p8rx+=LEN_SA+8+4;
			while(x--)
			{
				if(LENmax_TxSPACE<6)
				{
					GET_Request_Frame(PORTn);//分帧标志
					break;//发送缓冲不够
				}
				UARTCtrl->NEXT=0;//DL698分帧标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧
				UARTCtrl->NEXTBlock=1;//DL698分帧传输过程的帧序号，取值范围0~4095
				UARTCtrl->NEXTOADNum=x;//DL698分帧还需传输的OAD个数
				UARTCtrl->pRxBuff=(u32)p8rx;
				UARTCtrl->NEXTOADSubNum=0;//DL698分帧传输OAD内子计数
				
				OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
				LENmax_TxSPACE-=4;
				LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+4+1,LENmax_TxSPACE-1);
				if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
				{
					break;
				}
				LEN_TxAPDU+=4;
				if(UARTCtrl->DAR!=0)//数据访问结果
				{
					p8tx[LEN_TxAPDU+0]=0;//Get-Result∷=CHOICE,错误信息  [0]   DAR
					p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
					LEN_TxAPDU+=2;
					LENmax_TxSPACE-=2;
				}
				else
				{
					p8tx[LEN_TxAPDU]=1;//Get-Result∷=CHOICE, [1]   DATA
					LEN_TxAPDU+=LEN_OAD+1;
					LENmax_TxSPACE-=LEN_OAD+1;
				}
				p8rx+=4;
				p8tx[3]++;//+1(SEQUENCE OF)
			}
			break;
		case 3://读取一个记录型对象属性请求       [3] GetRequestRecord，
//GetRequestRecord∷=SEQUENCE
//{
//  服务序号-优先级         PIID，
//  读取一个记录型对象属性  GetRecord
//}
//GetRecord∷=SEQUENCE
//{
//  对象属性描述符     OAD，
//  记录选择描述符     RSD，
//  记录列选择描述符   RCSD
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8rx+=LEN_SA+8+3;
			LEN_OAD=GET_RecordOAD(PORTn,p8rx);//读一个记录型对象属性;返回:b31-b16接收ROAD及数据字节数,b15-b0发送ROAD及数据字节数,数据访问结果UARTCtrl->DAR
			p8rx+=LEN_OAD>>16;
			LEN_TxAPDU=LEN_OAD&0xffff;
			if(UARTCtrl->DAR!=0)//数据访问结果
			{
				p8tx[LEN_TxAPDU+0]=0;//Get-Result∷=CHOICE,错误信息  [0]   DAR
				p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
				LEN_TxAPDU+=2;
				LENmax_TxSPACE-=2;
			}
			break;
		case 4://读取若干个记录型对象属性请求	  [4] GetRequestRecordList，
//GetRequestRecordList∷=SEQUENCE
//{
//  服务序号-优先级           PIID，
//  读取若干个记录型对象属性  SEQUENCE OF GetRecord
//}
//GetResponseRecordList∷=SEQUENCE
//{
//  服务序号-优先级-ACD           PIID-ACD，
//  若干个记录型对象属性及其结果  SEQUENCE OF A-ResultRecord
//}
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			p8rx+=LEN_SA+8+4;
			ms=Get_ms();
			LEN_TxAPDU=0;
			pDATA=0;
			for(i=0;i<x;i++)
			{
				LEN_OAD=GET_RecordOAD(PORTn,p8rx);//读一个记录型对象属性;返回:b31-b16接收ROAD及数据字节数,b15-b0发送ROAD及数据字节数,数据访问结果UARTCtrl->DAR
				p8rx+=LEN_OAD>>16;
				TI=LEN_OAD&0xffff;
//				if(UARTCtrl->DAR!=0)//数据访问结果
//				{
//					p8tx[LEN_TxAPDU+0]=0;//Get-Result∷=CHOICE,错误信息  [0]   DAR
//					p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
//					LEN_TxAPDU+=2;
//					LENmax_TxSPACE-=2;
//				}
				if(pDATA==0)
				{
					pDATA=(u8*)ms->ms_malloc(TI);
				}
				else
				{
					pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU+TI);
				}
				if(UARTCtrl->pTxData)
				{
					ms->ms_free(UARTCtrl->pTxData);
					ms->ms_free(pDATA);
					return;
				}
				if(pDATA==0)
				{
					return;
				}
				if(TI>=3)
				{
					TI-=3;
					MR((u32)pDATA+LEN_TxAPDU,(u32)p8tx+3,TI);//去开始3byte
					LEN_TxAPDU+=TI;//总发送数据长度
				}
			}
		//客户机接收缓冲长度
			ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
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
			
			if((LEN_TxAPDU+4)>LENmax_TxSPACE)
			{
				return;
			}
			p8tx[0]=0x85;
			p8tx[1]=4;
			p8tx[2]=UARTCtrl->PIID;
			p8tx[3]=x;//SEQUENCE OF
			MR((u32)p8tx+4,(u32)pDATA,LEN_TxAPDU);
			ms->ms_free(pDATA);
			LEN_TxAPDU+=4;
			
//	UARTCtrl->pTxData=0;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
//	UARTCtrl->LEN_TxData=0;//需发送数据总长度(包括每段前的2字节段长度)
//	UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
			break;
		case 5://读取分帧响应的下一个数据块请求	  [5] GetRequestNext
//GetRequestNext∷=SEQUENCE
//{
//  服务序号-优先级                PIID，
//  正确接收的最近一次数据块序号   long-unsigned
//}
//GetResponseNext∷=SEQUENCE
//{
//  服务序号-优先级-ACD  	PIID-ACD，
//  末帧标志           	BOOLEAN，
//分帧序号           	long-unsigned，
//分帧响应             	CHOICE
//{
//错误信息    	    	[0]   DAR，
//  对象属性         	[1]   SEQUENCE OF A-ResultNormal，
//  记录型对象属性    	[2]   SEQUENCE OF A-ResultRecord
//}
//}
			p16=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
			p16[0]=TimerS_UARTRAMFREE;
		//68 15 00 43 05 01 00 00 00 00 00 10 04 5D 05 05 0F 00 02 00 81 1A 16
			p8rx+=LEN_SA+8+3;
			i=(p8rx[0]<<8)+p8rx[1];//正确接收的最近一次数据块序号
			x=UARTCtrl->NEXTBlock;
			UARTCtrl->NEXTBlock=i+1;
			if(i!=x)
			{//序号错
				UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
				p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
				p8tx[0]=133;//读取响应         	[133]	GET-Response
				p8tx[1]=5;//[[5] GetRequestNext
				p8tx[2]=UARTCtrl->PIID;//PIID-ACD
				p8tx[3]=1;//未帧标志
				p8tx[4]=UARTCtrl->NEXTBlock>>8;//正确接收的最近一次数据块序号
				p8tx[5]=UARTCtrl->NEXTBlock;
				p8tx[6]=0;//分帧响应             	CHOICE
				p8tx[7]=255;//错误类型:255
				LEN_TxAPDU=8;
				break;
			}
			//copy rx+Rxoffset到rx
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			x=p8rx[Rxoffset+1]+(p8rx[Rxoffset+2]<<8);//原接收帧总长度
			x+=3;//整半字
			x>>=1;
			x<<=1;
			i=Get_LEN_UARTnRx(PORTn);
			if((Rxoffset+x)>i)
			{
				x=i-Rxoffset;
			}
			x/=2;
			p16=(u16*)p8rx;
			for(i=0;i<x;i++)
			{
				p16[i]=p16[(Rxoffset/2)+i];
			}
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			switch(UARTCtrl->GET_Reques_CHOICE)//原得到请求选择
			{
				case 1://读取一个对象属性请求
					p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
					p8tx[0]=133;//读取响应         	[133]	GET-Response
					p8tx[1]=5;//[1] GetRequestNormal,[5]  GetResponseNext
					p8tx[2]=UARTCtrl->PIID;//PIID-ACD
					LEN_TxAPDU=3;
					LENmax_TxSPACE-=LEN_TxAPDU;
					p8rx=(u8*)UARTCtrl->pRxBuff;
					OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
					p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
					p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
					p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
					p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
					LEN_TxAPDU+=4;
					LENmax_TxSPACE-=4;
					LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+1,LENmax_TxSPACE-1);
					if(UARTCtrl->DAR!=0)//数据访问结果
					{
						p8tx[LEN_TxAPDU+0]=0;//Get-Result∷=CHOICE,错误信息  [0]   DAR
						p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
						LEN_TxAPDU+=2;
					}
					else
					{
						p8tx[LEN_TxAPDU]=1;//Get-Result
						LEN_TxAPDU+=LEN_OAD+1;
					}
					break;
				case 2://读取若干个对象属性请求
					p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
					p8tx[0]=133;//读取响应         	[133]	GET-Response
					p8tx[1]=5;//读取若干个对象属性响应   	[2] ,[5]  GetResponseNext
					p8tx[2]=UARTCtrl->PIID;//PIID-ACD
					p8tx[3]=0;//SEQUENCE OF
					LEN_TxAPDU=4;
					LENmax_TxSPACE-=LEN_TxAPDU;
				
					x=UARTCtrl->NEXTOADNum;//x=p8rx[LEN_SA+8+3];//SEQUENCE OF
					p8rx=(u8*)UARTCtrl->pRxBuff;//p8rx+=LEN_SA+8+4;
					while(x--)
					{
						if(LENmax_TxSPACE<6)
						{
							GET_Request_Frame(PORTn);//分帧标志
							break;//发送缓冲不够
						}
						//UARTCtrl->NEXT=0;//DL698分帧标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧
						//UARTCtrl->NEXTBlock=1;//DL698分帧传输过程的帧序号，取值范围0~4095
						UARTCtrl->NEXTOADNum=x;//DL698分帧还需传输的OAD个数
						UARTCtrl->pRxBuff=(u32)p8rx;
						UARTCtrl->NEXTOADSubNum=0;//DL698分帧传输OAD内子计数
						
						OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
						p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
						p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
						p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
						p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
						LEN_TxAPDU+=4;
						LENmax_TxSPACE-=4;
						LEN_OAD=GET_OAD_DATA(PORTn,OAD,p8tx+LEN_TxAPDU+1,LENmax_TxSPACE-1);
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
							p8tx[LEN_TxAPDU]=1;//数据
							LEN_TxAPDU+=LEN_OAD+1;
							LENmax_TxSPACE-=LEN_OAD+1;
						}
						p8rx+=4;
						p8tx[3]++;//+1(SEQUENCE OF)
					}
					break;
				case 3://读取一个记录型对象属性请求
					LEN_TxAPDU=GET_RecordOAD_Next(PORTn);//读一个记录型对象属性分帧响应的下一帧;返回：LEN_TxAPDU
					if(LEN_TxAPDU==0)
					{
						UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
						p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
						p8tx[0]=133;//读取响应         	[133]	GET-Response
						p8tx[1]=5;//[[5] GetRequestNext
						p8tx[2]=UARTCtrl->PIID;//PIID-ACD
						p8tx[3]=1;//未帧标志
						p8tx[4]=UARTCtrl->NEXTBlock>>8;//正确接收的最近一次数据块序号
						p8tx[5]=UARTCtrl->NEXTBlock;
						p8tx[6]=0;//分帧响应             	CHOICE
						p8tx[7]=255;//错误类型:255
						LEN_TxAPDU=8;
					}
					break;
				case 4://读取若干个记录型对象属性请求
					LEN_TxAPDU=GET_RecordOAD_Next(PORTn);//读一个记录型对象属性分帧响应的下一帧;返回：LEN_TxAPDU
					if(LEN_TxAPDU==0)
					{//下一个记录
						UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
						p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
						p8tx[0]=133;//读取响应         	[133]	GET-Response
						p8tx[1]=5;//[[5] GetRequestNext
						p8tx[2]=UARTCtrl->PIID;//PIID-ACD
						p8tx[3]=1;//未帧标志
						p8tx[4]=UARTCtrl->NEXTBlock>>8;//正确接收的最近一次数据块序号
						p8tx[5]=UARTCtrl->NEXTBlock;
						p8tx[6]=0;//分帧响应             	CHOICE
						p8tx[7]=255;//错误类型:255
						LEN_TxAPDU=8;
					}
					break;
				default:
					return;
			}
			break;
		default:
			return;
	}
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
}


void GET_Request_Frame(u32 PORTn)//分帧标志
{
	UARTCtrl_TypeDef *UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	{
		UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	}
	else
	{
		UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	}
}

u32 GET_OAD_Element(u32 PORTn,u8* ADDR_OAD,u32 Element,u32 LENmax_OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象元素;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
{
	u32 Len;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8=Get_Element(ADDR_OAD,Element,0,LENmax_OAD);//计算元素地址,使用ADDR_128KDATABUFF
	if(p8==0)
	{
		UARTCtrl->DAR=6;//对象不存在
		return 0;
	}
	Len=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
	if(Len>LENmax_TxSPACE)
	{
		GET_Request_Frame(PORTn);//分帧标志
		return 0;
	}
	if(Len==0)
	{
		if(LENmax_TxSPACE<1)
		{
			GET_Request_Frame(PORTn);//分帧标志
			return 0;
		}
		p8tx[0]=0;//NULL
		return 1;
	}
	MR((u32)p8tx,(u32)p8,Len);
	return Len;
}



u32 GET_OAD_DATA(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
//  一个对象属性描述符   OAD
//OAD∷=SEQUENCE
//{
//  对象标识          OI，
//  属性标识及其特征  bit-string（SIZE（8）），
//  属性内元素索引    unsigned（1…255）
//}
//对象属性标识及其特征——用bit0…bit7表示八位位组的最低位到最高位，其中：
//1)	bit0…bit4编码表示对象属性编号，取值0…31，其中0表示整个对象属性，即对象的所有属性；
//2)	bit5…bit7编码表示属性特征，属性特征是对象同一个属性在不同快照环境下取值模式，取值0…7，特征含义在具体类属性中描述。
//属性内元素索引——00H表示整个属性全部内容。如果属性是一个结构或数组，01H指向对象属性的第一个元素；如果属性是一个记录型的存储区，非零值n表示最近第n次的记录。

//A-ResultNormal∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  及其结果        Get-Result
//}
//Get-Result∷=CHOICE
//{
//  错误信息  [0]   DAR，
//  数据      [1]   Data
//}
	u32 Len;
	DL698_SETLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功

	if((OAD&0xff00)==0x0100)
	{//读逻辑名
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
	}
	if((OAD>>24)==0x44)
	{//应用连接IO列表
		Len=GET_OAD_ConnectApp(PORTn,OAD,p8tx,LENmax_TxSPACE);//读应用连接可访问对象列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
		return Len;
	}
	if(((OAD>>16)&0xff00)<0x2100)
	{//iMeter
		Len=GET_iMeter_OAD(PORTn,OAD,p8tx,LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
		return Len;
	}
	
#if (USER/100)==17//???
	if((OAD&0xffffff00)==0x40480200)
	{
		pLib=Get_DL698_SET_pLib(OAD&0xffffff00);//??DATA_pLib??;??:OAD;??:Lib??(=0??????OI)
		if(pLib==0)
		{
			UARTCtrl->DAR=6;//?????           (6)
			return 0;
		}
		MR((u32)p8tx,pLib->Addr,8);
		return 8;
		//MR((u32)p8tx,ADDR_4048_127,8);
		//return 8;
	}
	if((OAD&0xffffff00)==0x45000C00)
	{
		Len=Get_Request_4500(PORTn,OAD,p8tx,LENmax_TxSPACE);
		return Len;
	}
#endif
#if (USER/100)==15//安徽专变
	if((OAD&0xffffff00)==0x31060B00)
	{//安徽专变终端停/上电事件属性11配置参数2
 			pLib=Get_DL698_SET_pLib(OAD&0xffffff00);//得到DATA_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
			if(pLib==0)
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
				return 0;
			}
			MR((u32)p8tx,pLib->Addr,4);
			return 4;
	}
#endif
	if(((OAD>>16)&0xf000)==0x3000)
	{//事件
		Len=GET_EVENT_OAD(PORTn,OAD,p8tx,LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
		return Len;
	}
	if((OAD>>16)==0xF001)
	{//文件传输
		Len=GET_Request_F001(PORTn,OAD,p8tx,LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
		return Len;
	}
	if((OAD>>16)==0xF100)
	{//ESAM
		Len=GET_ESAM_OAD(PORTn,OAD,p8tx,LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
		return Len;
	}
	if(((OAD>>16)&0xff00)==0xF200)
	{//输入输出设备类
		Len=GET_Request_F200(pLib,PORTn,OAD,p8tx,LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
		if(Len)
		{
			return Len;
		}
		UARTCtrl->DAR=0;//数据访问结果;成功
	}
	
#if (USER/100)==9//河南
	if(OAD==0xff100200)//电表接线错误信息表
	{
		Len=GET_Request_FF10(pLib,PORTn,OAD,p8tx,LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
		if(Len)
		{
			return Len;
		}
		UARTCtrl->DAR=6;//对象不存在（6）
		return 0;
	}
#endif
#if MeterSpec==20//自动适应电能表规格
	switch(OAD)
	{
		case 0x41040200://额定电压
		case 0x41050200://额定电流/基本电流
		case 0x41060200://最大电流
		case 0x41090200://电能表有功常数
		case 0x410A0200://电能表无功常数
			 Len=GET_Request_4100(PORTn,OAD,p8tx,LENmax_TxSPACE);
				return Len;
	}
#endif
	if(((OAD>>24)==0x23)||((OAD>>24)==0x24))
	{//脉冲输入，总加组
		Len=GET_23xx_24xx(PORTn,OAD,p8tx,LENmax_TxSPACE);//读总加组和脉冲计量;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
		return Len;
	}
	pLib=Get_DL698_DATA_pLib(OAD&0xffffff00);//得到DATA_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
	if(pLib==0)
	{
		pLib=Get_DL698_SET_pLib(OAD&0xffffff00);//得到SET_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
		if(pLib==0)
		{
			Len=GET_OAD_FactoryTest(PORTn,OAD,p8tx,LENmax_TxSPACE);//读工厂测试结果;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
			if(Len)
			{
				return Len;
			}
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		}
	}
	if(OAD&0xff00)
	{//单个属性
		if((OAD&0xff)==0)
		{//全部元素
			switch(OAD)
			{
				case 0x60000200://采集档案配置表
				case 0x60120200://任务配置表
				case 0x60140200://普通采集方案
				case 0x60160200://事件采集方案
				case 0x60180200://透明方案
				case 0x601A0200://透明方案结果
				case 0x601C0200://上报方案
				case 0x601E0200://采集规则库
				case 0x60320200://采集状态集
				case 0x60340200://采集任务监控集
					Len=GET_Request_6000(pLib,PORTn,OAD,p8tx,LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
					return Len;
				case 0x60020200://所有搜表结果
				case 0x60020300://当前元素个数.所有搜表结果
				case 0x60020400://最大元素个数,所有搜表结果
				case 0x60020500://跨台区搜表结果
				case 0x60020600://所有搜表结果记录数
				case 0x60020700://跨台区搜表结果记录数
					Len=GET_Request_6002(PORTn,OAD,p8tx,LENmax_TxSPACE);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
					return Len;
				
				case 0x60000300://当前元素个数.采集档案配置表
				case 0x60140300://当前元素个数.普通采集方案集
				case 0x60160300://当前元素个数.事件采集方案集
				case 0x60180300://当前元素个数.透明方案集
				case 0x601A0300://当前元素个数.透明方案结果
				case 0x601C0300://当前元素个数.上报方案集
				case 0x601E0300://当前元素个数.采集规则库
				case 0x60320300://当前元素个数.采集状态集
				case 0x60340300://当前元素个数.采集任务监控集
					if(LENmax_TxSPACE<3)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					Len=GET_NUMper(pLib);//计算当前元素个数
					p8tx[0]=DataType_long_unsigned;
					p8tx[1]=Len>>8;
					p8tx[2]=Len;
					return 3;
				case 0x60000400://最大元素个数.采集档案配置表
				case 0x60140400://最大元素个数.普通采集方案集
				case 0x60160400://最大元素个数.事件采集方案集
				case 0x60180400://最大元素个数.透明方案集
				case 0x601A0400://最大元素个数.透明方案结果
				case 0x601C0400://最大元素个数.上报方案集
				case 0x601E0400://最大元素个数.采集规则库
				case 0x60320400://最大元素个数.采集状态集
				case 0x60340400://最大元素个数.采集任务监控集
					if(LENmax_TxSPACE<3)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					Len=pLib->LENmax/pLib->LENper;
					p8tx[0]=DataType_long_unsigned;
					p8tx[1]=Len>>8;
					p8tx[2]=Len;;
					return 3;
				case 0xF2000300://设备对象数-RS232
				case 0xF2010300://设备对象数-RS485
				case 0xF2020300://设备对象数-红外
				case 0xF2030300://设备对象数-开关量
				case 0xF2040300://设备对象数-直流模拟量
				case 0xF2050300://设备对象数-继电器输出
				case 0xF2060300://设备对象数-告警输出
				case 0xF2070300://设备对象数-多功能端子
				case 0xF2090300://设备对象数-载波/微功率无线接口
				case 0xF20A0300://设备对象数-脉冲输入设备
				case 0xF20B0300://蓝牙
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					p8tx[0]=DataType_unsigned;
					p8tx[1]=pLib->LENmax;//设备对象数
					return 2;

				case 0x40000200://日期时间
					Len=8;
					break;
				default:
					Len=Get_DL698DataLen((u8*)pLib->Addr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
					if(pLib->pDataType)
					{
						if((Len&0xff)==0)
						{
							UARTCtrl->DAR=7;//类型不匹配           （7）
							return 0;
						}
					}
					Len>>=8;
					break;
			}
			if(Len>pLib->LENmax)
			{
				if(pLib->LENmax!=0)
				{//数据是存储在FLASH
					UARTCtrl->DAR=9;//数据块不可用         （9）
					return 0;
				}
			}
			if(Len>LENmax_TxSPACE)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			switch(OAD>>16)
			{
				case 0x4000://日期时间
					if((OAD&0xff00)==0x200)
					{//属性=2
						p8tx[0]=DataType_date_time_s;
						MR((u32)p8tx+1,pLib->Addr,7);
						Len=8;
						break;
					}
				default:
					if((pLib->rw&0x1)||(Comm_Ram->Factory==0x55))
					{
						MR((u32)p8tx,pLib->Addr,Len);
					}
					else
					{//不允许读
						UARTCtrl->DAR=3;//数据访问结果;拒绝读写             （3）
						return 0;
					}
					break;
			}
		}
		else
		{//单个元素
			Len=GET_OAD_Element(PORTn,(u8*)pLib->Addr,OAD&0xff,pLib->LENmax,p8tx,LENmax_TxSPACE);//读一个对象元素;;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
		}
		
//正确读后处理
		switch(OAD)
		{
			case 0xF2030200://读取开关量输入开关量单元
				CLR_SwitchIn_CD();//清开关量输入变位CD 
				break;
		}
		return Len;
	}
	else
	{//0表示整个对象属性，即对象的所有属性
		UARTCtrl->DAR=3;//数据访问结果;拒绝读写             （3）
		return 0;
	}
}




























