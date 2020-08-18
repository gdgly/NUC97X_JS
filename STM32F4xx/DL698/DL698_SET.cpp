

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SET.h"
#include "../DL698/DL698_SET_6000.h"
#include "../DL698/DL698_SET_EVENT.h"
#include "../DL698/DL698_GET.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../Device/RTC.h"
#include "../MS/MS.h"
#include "../DL698/TASK.h"
#include "../DL698/EVENT_Record.h"
#include "../DL698/DL698_GET_SET_ACTION_2000.h"
#include "../Device/IC_BUZZ.h"

#include "../DL698/DL698_JSON.h"
#include "../DL698/Info_3762.h"


//设置请求的数据类型（SET-Request）定义
//SET-Request∷=CHOICE
//{
//设置一个对象属性请求        	[1] SetRequestNormal，
//设置若干个对象属性请求     	[2] SetRequestNormalList，
//设置后读取若干个对象属性请求	[3] SetThenGetRequestNormalList
//}
//SET-Response∷=CHOICE
//{
//设置一个对象属性的确认信息响应   	[1] SetResponseNormal，
//设置若干个对象属性的确认信息响应  	[2] SetResponseNormalList，
//设置的确认信息以及读取的响应 	     [3] SetThenGetResponseNormalList
//}

void SET_Request(u32 PORTn)
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
	u8* p8;
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
//68 1D 00 43  05  01 00 00 00 00 00  10  AE E1 
//06   应用层服务  CHOICE
//01   SET-Request∷=CHOICE
//00   PIID
//40 01 01 00    OAD
//09   DataType
//04 12 34 56 78 
//00   TI
//CB AE 16 

	LEN_RxAPDU-=1;
	LENmax_TxSPACE-=1;//时间标签:0=无
	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	LEN_RxAPDU-=(LEN_SA+7+3+2);

	p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
	p8tx[0]=134;//设置响应       	[134]	SET-Response
	
	switch(p8rx[LEN_SA+8+1])
	{
		case 1://设置一个对象属性请求        	[1] SetRequestNormal
//设置一个对象属性请求的数据类型定义
//SetRequestNormal∷=SEQUENCE
//{
//  服务序号-优先级      PIID，
//  一个对象属性描述符   OAD，
//  数据                 Data
//}
//SetResponseNormal∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  一个对象属性描述符    OAD，
//  设置执行结果          DAR
//}
			p8tx[1]=1;//设置一个对象属性的确认信息响应   	[1] SetResponseNormal
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			LEN_TxAPDU=3;
			LENmax_TxSPACE-=LEN_TxAPDU;
		
			p8rx+=LEN_SA+8+3;
			//时间标签
			LEN_OAD=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			if(p8rx[4+LEN_OAD]==1)
			{//有时间标签
				i=Uart_TimeTag(p8rx+4+LEN_OAD+1);//时间标签有效性判断;入口:p8rx指向时间标签;返回:0=有效,1=无效
				if(i)
				{//1=不相同p1>p2
					p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
					p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
					p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
					p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
					p8tx[LEN_TxAPDU+4]=32;//时间标签无效(32)
					LEN_TxAPDU+=5;
					break;
				}
			}
			LEN_OAD=SET_OAD(PORTn,p8rx,LEN_RxAPDU);//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
			p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
			p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
			p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
			p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
			p8=(u8*)ADDR_1kDATABUFF;//设置的OAD数组
			p8[0]=DataType_array;
			p8[1]=1;
			p8[2]=DataType_OAD;
			p8[3]=p8rx[0];
			p8[4]=p8rx[1];
			p8[5]=p8rx[2];
			p8[6]=p8rx[3];
			Event_31180200(p8);//终端编程事件记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF

			p8tx[LEN_TxAPDU+4]=UARTCtrl->DAR;//数据访问结果
			LEN_TxAPDU+=5;
			break;
		case 2://设置若干个对象属性请求     	[2] SetRequestNormalList
//设置若干个对象属性请求的数据类型定义
//SetRequestNormalList∷=SEQUENCE
//{
//  服务序号-优先级  PIID，
//  若干个对象属性   SEQUENCE OF
//  {
//    一个对象属性描述符  OAD，
//    数据                Data
//  }
//}
//SetResponseNormalList∷=SEQUENCE
//{
//  服务序号-优先级-ACD     PIID-ACD，
//  若干个对象属性设置结果  SEQUENCE OF
//  {
//一个对象属性描述符   OAD，
//     设置执行结果         DAR
//}
//}
			p8tx[1]=2;//设置若干个对象属性的确认信息响应  	[2] SetResponseNormalList，
			p8tx[2]=p8rx[LEN_SA+8+2];//PIID-ACD
			p8tx[3]=0;//(SEQUENCE OF)
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=4;
			
			x=p8rx[LEN_SA+8+3];//SEQUENCE OF
			LEN_RxAPDU-=1;
			p8rx+=LEN_SA+8+4;
			
			//时间标签
			LEN_OAD=0;
			i=x;
			while(i--)
			{
				LEN_OAD+=4;
				LEN_OAD+=Get_DL698DataLen_S(p8rx+LEN_OAD,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			}
			if(p8rx[LEN_OAD]==1)
			{//有时间标签
				i=Uart_TimeTag(p8rx+LEN_OAD+1);//时间标签有效性判断;入口:p8rx指向时间标签;返回:0=有效,1=无效
				if(i)
				{//无效
					LEN_OAD=0;
					while(x--)
					{
						p8tx[LEN_TxAPDU+0]=p8rx[0+LEN_OAD];//OAD
						p8tx[LEN_TxAPDU+1]=p8rx[1+LEN_OAD];//OAD
						p8tx[LEN_TxAPDU+2]=p8rx[2+LEN_OAD];//OAD
						p8tx[LEN_TxAPDU+3]=p8rx[3+LEN_OAD];//OAD
						p8tx[LEN_TxAPDU+4]=32;//时间标签无效(32)
						LEN_TxAPDU+=5;
						LEN_OAD+=4;
						LEN_OAD+=Get_DL698DataLen_S(p8rx+LEN_OAD,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					}
					break;
				}
			}
			
			p8=(u8*)ADDR_1kDATABUFF;//设置的OAD数组(事件用)
			p8[0]=DataType_array;
			p8[1]=0;
			while(x--)
			{
				if(LENmax_TxSPACE<5)
				{
					break;//发送缓冲不够
				}
				LEN_OAD=SET_OAD(PORTn,p8rx,LEN_RxAPDU);//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
				
				p8tx[LEN_TxAPDU+4]=UARTCtrl->DAR;//数据访问结果
				LEN_TxAPDU+=5;
				LENmax_TxSPACE-=5;
				if(LEN_OAD==0)
				{
					LEN_OAD=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
				}
				if(UARTCtrl->DAR==0)//数据访问结果
				{
					i=p8[1];//设置的OAD数组(事件用)
					i*=5;
					i+=2;
					p8[i+0]=DataType_OAD;
					p8[i+1]=p8rx[0];//OAD
					p8[i+2]=p8rx[1];//OAD
					p8[i+3]=p8rx[2];//OAD
					p8[i+4]=p8rx[3];//OAD
					p8[1]++;//数组数+=1
				}
				
				p8tx[3]++;//+1(SEQUENCE OF)
				if(LEN_RxAPDU<(4+LEN_OAD))
				{
					p8tx[LEN_TxAPDU-1]=255;//数据访问结果:其它
					break;//错误
				}
				LEN_RxAPDU-=4+LEN_OAD;
				p8rx+=4+LEN_OAD;
			}
			Event_31180200(p8);//终端编程事件记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			break;
		case 3://设置后读取若干个对象属性请求	[3] SetThenGetRequestNormalList
//设置后读取若干个对象属性请求的数据类型定义
//SetThenGetRequestNormalList∷=SEQUENCE
//{
//  服务序号-优先级  PIID，
//  若干个设置后读取对象属性  SEQUENCE OF
//{
//一个设置的对象属性   OAD，
//数据                 Data，
	//一个读取的对象属性   OAD，
	//延时读取时间         unsigned //延时读取时间——单位：秒，0表示采用服务器默认的延时时间。
//  }
//}
//SetThenGetResponseNormalList∷=SEQUENCE
//{
//  服务序号-优先级-ACD   PIID-ACD，
//  若干个对象属性设置后读取结果  SEQUENCE OF
//  {
//一个设置的对象属性描述符   OAD，
//     设置执行结果               DAR，
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
			
			p8=(u8*)ADDR_1kDATABUFF;//设置的OAD数组(事件用)
			p8[0]=DataType_array;
			p8[1]=0;
			while(x--)
			{
				//设置1个OAD
				if(LENmax_TxSPACE<5)
				{
					break;//发送缓冲不够
				}
				LEN_OAD=SET_OAD(PORTn,p8rx,LEN_RxAPDU);//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
				
				p8tx[LEN_TxAPDU+4]=UARTCtrl->DAR;//数据访问结果
				LEN_TxAPDU+=5;
				LENmax_TxSPACE-=5;
				
				if(LEN_OAD==0)
				{
					LEN_OAD=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
				}
				if(UARTCtrl->DAR==0)//数据访问结果
				{
					i=p8[1];//设置的OAD数组(事件用)
					i*=5;
					i+=2;
					p8[i+0]=DataType_OAD;
					p8[i+1]=p8rx[0];//OAD
					p8[i+2]=p8rx[1];//OAD
					p8[i+3]=p8rx[2];//OAD
					p8[i+4]=p8rx[3];//OAD
					p8[1]++;//数组数+=1
				}				
				
				if(LEN_RxAPDU<(4+LEN_OAD))
				{
					p8tx[LEN_TxAPDU-1]=255;//数据访问结果:其它
					break;//错误
				}
				LEN_RxAPDU-=4+LEN_OAD;
				p8rx+=4+LEN_OAD;
				//读1个OAD
				if(LENmax_TxSPACE<6)
				{
					break;//发送缓冲不够
				}
				OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
				p8tx[LEN_TxAPDU+0]=p8rx[0];//OAD
				p8tx[LEN_TxAPDU+1]=p8rx[1];//OAD
				p8tx[LEN_TxAPDU+2]=p8rx[2];//OAD
				p8tx[LEN_TxAPDU+3]=p8rx[3];//OAD
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
					p8tx[LEN_TxAPDU]=1;
					LEN_TxAPDU+=LEN_OAD+1;
					LENmax_TxSPACE-=LEN_OAD+1;
				}
				p8rx+=4+1;//1byte 设置后读取延时
				LEN_RxAPDU-=4+1;
				p8tx[3]++;//+1(SEQUENCE OF)
			}
			Event_31180200(p8);//终端编程事件记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			break;
		default:
			LEN_TxAPDU=0;
			break;
	}
	TI=0;
	Uart_698_TxResponse(PORTn,TI,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
}

u32 SET_OAD_Element(u32 PORTn,u8 *p8rx,u8* ADDR_OAD,u32 Element,u32 LENmax_OAD)//设置一个对象元素;返回:本元素的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Len;//新元素长度
	u32 LenT;//原数据总长度
	u32 Len0;//原元素前长度
	u32 Len1;//原元素长度
	u32 Len2;//原元素后长度
	u8* p8;
	u8 *p8s;
	u8 *p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//新元素长度
	Len=Get_DL698DataLen_S(p8rx,LENmax_OAD);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
	if(ADDR_OAD==0)
	{
		UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
		return Len;
	}
	if(LENmax_OAD>LEN_128KDATABUFF)
	{
		UARTCtrl->DAR=255;//数据访问结果
		return Len;
	}
	MR(ADDR_128KDATABUFF,(u32)ADDR_OAD,LENmax_OAD);
	//原数据总长度
	LenT=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF,LENmax_OAD);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
	//原元素地址
	p8=Get_Element((u8*)ADDR_128KDATABUFF,Element,0,0);//计算元素地址,使用ADDR_128KDATABUFF
	if(p8==0)
	{
		UARTCtrl->DAR=255;//数据访问结果
		return Len;
	}
	//原元素长度
	Len1=Get_DL698DataLen_S(p8,0);
	//原元素前长度
	Len0=(u32)p8-(ADDR_128KDATABUFF);
	//原元素后长度
	Len2=LenT-(Len0+Len1);
	if((LenT+Len0+Len+Len2)>LEN_128KDATABUFF)
	{
		UARTCtrl->DAR=255;//数据访问结果
		return Len;
	}
	//组织新数据结构
	p8s=(u8*)ADDR_128KDATABUFF;
	p8d=(u8*)ADDR_128KDATABUFF+LenT;
	//原元素前数据
	for(i=0;i<Len0;i++)
	{
		p8d[0]=p8s[i];
		p8d++;
	}
	//新元素数据
	for(i=0;i<Len;i++)
	{
		p8d[0]=p8rx[i];
		p8d++;
	}
	//原元素后数据
	p8s=(u8*)ADDR_128KDATABUFF+Len0+Len1;
	for(i=0;i<Len2;i++)
	{
		p8d[0]=p8s[i];
		p8d++;
	}
	i=Len0+Len+Len2;
	if(i>LENmax_OAD)
	{
		UARTCtrl->DAR=255;//数据访问结果
		return Len;
	}
	MW(ADDR_128KDATABUFF+LenT,(u32)ADDR_OAD,i);
	UARTCtrl->DAR=0;//数据访问结果;成功
	return Len;
}

u32 SET_OAD(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU)//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
{
//  一个对象属性描述符   OAD
//  数据                 Data
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
	u32 i;
	u32 n;
	u32 OAD;
	u32 Len;
	DL698_SETLIB_TypeDef *pLib;
	u8 *p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];

	if(((OAD>>8)&0xff)==1)
	{//逻辑名只读
		UARTCtrl->DAR=3;//数据访问结果;拒绝读写（3）
		return 4;
	}
	if(((OAD>>24)==0x23)||((OAD>>24)==0x24))
	{//脉冲输入，总加组
		Len=SET_23xx_24xx(PORTn,p8rx,LEN_RxAPDU);//设置总加组和脉冲计量;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
		return Len;
	}
	switch(OAD>>16)
	{
		case 0x6012://任务配置表
		case 0x6014://普通采集方案集
		case 0x6016://事件采集方案集
		case 0x6018://透明方案集
		case 0x601c://上报方案集
		case 0x601e://采集规则库
			Terminal_Ram->SETTask=0xff;//任务配置表采集方案有设置标志:0=没设置,0xff=设置
			Len=SET_OAD_6000(PORTn,p8rx,LEN_RxAPDU);//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
			switch(OAD>>16)
			{
				case 0x6012://任务配置表
					MR(ADDR_6012_2_SDRAM,ADDR_6012_2,LENmax_6012_2);//任务配置表FLASHtoSDRAM
					break;
				case 0x6014://普通采集方案集
					MR(ADDR_6014_SDRAM,ADDR_6014,LENmax_6014);//普通采集方案FLASHtoSDRAM
					break;
				case 0x6016://事件采集方案集
					MR(ADDR_6016_SDRAM,ADDR_6016,LENmax_6016);//事件采集方案FLASHtoSDRAM
					break;
				case 0x601c://上报方案集
					MR(ADDR_601C_SDRAM,ADDR_601C,LENmax_601C);//上报方案FLASHtoSDRAM
					break;
			}
			return Len;
	}
#if (USER/100)==15//安徽
	if((OAD&0xffffff00)==0x31060B00)
	{
		Len= Get_DL698DataLen_S(p8rx+4,0);
		pLib=Get_DL698_SET_pLib(OAD);//得到SET_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
		if(pLib==0)
		{
			UARTCtrl->DAR=6;////数据访问结果;对象不存在           （6）
			return 0;
		}
		MW((u32)p8rx+4,pLib->Addr,Len);
		UARTCtrl->DAR=0;//数据访问结果;成功
		return Len;
	}
#endif
	if((OAD&0xf0000000)==0x30000000)
	{//设置事件类
		Len=SET_OAD_EVENT(PORTn,p8rx,LEN_RxAPDU);//设置一个对象属性;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
		return Len;
	}
	p8rx+=4;
	switch(OAD)
	{
		case 0x40000200://日期时间
			if(LEN_RxAPDU<(4+8))
			{
				UARTCtrl->DAR=7;//数据访问结果;7=类型不匹配
				return 0;
			}
			//date_time_s∷=SEQUENCE
			//{
			//  year          long-unsigned，
			//  month         unsigned，
			//  day           unsigned，
			//  hour          unsigned，
			//  minute        unsigned，
			//  second        unsigned
			//}
			if(p8rx[0]!=DataType_date_time_s)
			{
				UARTCtrl->DAR=255;//数据访问结果
				return 0;
			}
			i=Check_pYYMDHMS_hex(p8rx+1);//年年月日时分秒合法检查,返回0合法,1非法
			if(i)
			{
				UARTCtrl->DAR=255;//数据访问结果
				return 0;
			}
			p8rx+=1;
			MR(ADDR_128KDATABUFF+(64*1024),ADDR_DL698YMDHMS,7);//对时事件发生前时间
			p8d=(u8*)ADDR_RTCBUFF;
			i=p8rx[6];
			i=hex_bcd(i);
			p8d[0]=i;
			i=p8rx[5];
			i=hex_bcd(i);
			p8d[1]=i;
			i=p8rx[4];
			i=hex_bcd(i);
			p8d[2]=i;
			i=p8rx[3];
			i=hex_bcd(i);
			p8d[4]=i;
			i=p8rx[2];
			i=hex_bcd(i);
			p8d[5]=i;
			i=(p8rx[0]<<8)+p8rx[1];
			i=hex_bcd(i);
			p8d[6]=i;
			WriteRTC(ADDR_RTCBUFF);//写实时钟
			MR(ADDR_DL698YMDHMS,(u32)p8rx,7);//对时事件结束后时间
			#if (USER/100)==17
			MR((u32)JSON->Lastrealtime,ADDR_DL698YMDHMS,7);//对时事件结束后时间
			MR((u32)JSON->reportfiletime,ADDR_DL698YMDHMS,7);//对时事件结束后时间
			#endif
			Event_OAD(0x31140200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			UARTCtrl->DAR=0;//数据访问结果;成功
			//Terminal_Ram->SETtime=0xff;//时钟设置标志:0=没设置,0xff=设置,用于电科院载波口互换性测试
			return 8;
			
//设置值检查
		case 0x40010200://通信地址
			if(p8rx[1]==0)
			{//长度=0
				Len= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				UARTCtrl->DAR=8;//数据访问结果
				return Len;
			}
			i=Uart_TSA_Comp(p8rx+1,(u8 *)ADDR_4001+1);//通信地址比较,pFLASH数据读到ADDR_128K_DDATABUFF中比较,返回:0=相同,!=0不相同
			if(i)
			{//地址不同
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
				UARTCtrl->ReNewLink=0x55;//0x55=通信参数变化当前帧发送完成后重新连接
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
				UARTCtrl->ReNewLink=0x55;//0x55=通信参数变化当前帧发送完成后重新连接
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
			}
			break;
		case 0x40000300://校时模式
			if((p8rx[1]>=3)&&(p8rx[1]!=255))
			{
				Len= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				UARTCtrl->DAR=8;//数据访问结果
				return Len;
			}
			break;
		case 0x40040200://设备地理位置
			if(p8rx[5]>1)
			{
				Len= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				UARTCtrl->DAR=8;//数据访问结果
				return Len;
			}
			break;
		case 0x42020200://级连参数
			i=(p8rx[5]<<24)|(p8rx[6]<<16)|(p8rx[7]<<8)|(p8rx[8]<<0);
			if((i!=0xf2010201)&&(i!=0xf2010202))
			{//不是rs485口
				Len= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				UARTCtrl->DAR=6;//数据访问结果
				return Len;
			}
			break;
		case 0x42040200://广播校时参数-2
			i=(p8rx[3]<<16)|(p8rx[4]<<8)|(p8rx[5]<<0);
			i=Check_HMS_hex(i);//时分秒合法检查,返回0合法,1非法
			if(i)
			{
				Len= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				UARTCtrl->DAR=8;//数据访问结果
				return Len;
			}
			break;
		case 0x42040300://广播校时参数-3
			i=(p8rx[5]<<16)|(p8rx[6]<<8)|(p8rx[7]<<0);
			i=Check_HMS_hex(i);//时分秒合法检查,返回0合法,1非法
			if(i)
			{
				Len= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				UARTCtrl->DAR=8;//数据访问结果
				return Len;
			}
			break;
		case 0x45000200://无线公网通信配置
			if(p8rx[3]>2)
			{
				Len= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				UARTCtrl->DAR=8;//数据访问结果
				return Len;
			}
			break;
		case 0x80010500://自动保电时段
			n=p8rx[1];//数组数
			for(i=0;i<n;i++)
			{
				if((p8rx[2+(6*i)+3]>24)||(p8rx[2+(6*i)+5]>24))
				{
					Len= Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
					UARTCtrl->DAR=8;//数据访问结果
					return Len;
				}
			}
			break;
	#if ((Project/100)==2)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
		case 0xF2090900://宽带载波频段序号
			i=MRR(ADDR_F209_9+1,1);
			if(i!=p8rx[1])
			{//宽带载波频段序号变化
				MW((u32)p8rx+1,ADDR_F209_9+1, 1);
				UART_3762_AFN5F16(RS485_4PORT,&p8rx[1]);
				Terminal_Router->RouterInitTask=0;//路由器抄读任务0-99公共初始化,100-199分厂家初始化,>=200路由抄表
			}
			UARTCtrl->DAR=0;//数据访问结果;成功
			return 1;
		case 0xF2090B00://网络拓扑识别开关
#if 0		
// 台区识别 成功后 会自动关闭
			i=MRR(ADDR_F209_11+1,1);
			if(i!=p8rx[1])
			{
				MW((u32)p8rx+1,ADDR_F209_11+1, 1);
				UART_3762_AFN5F6(RS485_4PORT,(u32)p8rx[1]);
				//路由恢复
				Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
				Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
			}
#else
			UART_3762_AFN5F6(RS485_4PORT,(u32)p8rx[1]);
			//路由恢复
			Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
			Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
#endif
			UARTCtrl->DAR=0;//数据访问结果;成功
			return 1;
		case 0xF2091600://拒绝节点上报使能标志
			ms_Type* ms;
			ms=(ms_Type *)Get_ms();
			i=MRR(ADDR_F209_22+1,1);
			ms->link_printf("1-%x\n",i,0,0);
			if(i!=p8rx[1])
			{
				MW((u32)p8rx+1,ADDR_F209_22+1, 1);
				i=MRR(ADDR_F209_22+1,1);
				ms->link_printf("2-%x\n",i,0,0);
				UART_3762_AFN5F200(RS485_4PORT,&p8rx[1]);
				//UART_3762_AFN5F200(RS485_4PORT,&p8rx[1]);
				Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
				Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				ms->link_printf("3-%x\n",i,0,0);
			}
			
			UARTCtrl->DAR=0;//数据访问结果;成功
			return 1;
	#endif
		default:
			break;
	}
	pLib=Get_DL698_SET_pLib(OAD);//得到SET_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//数据访问结果;对象不存在           （6）
		return 0;
	}
	if((OAD>>8)&0xff)
	{//单个属性
		if(pLib->Addr==0)
		{
			UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
			return 0;
		}
		if(Comm_Ram->Factory!=0x55)
		{//不是工厂模式
			if((pLib->rw&0x2)==0)
			{//不允许写
				UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
				return 0;
			}
		}
		if((OAD&0xff)==0)
		{//全部元素
			Len=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
			if(pLib->pDataType)
			{
				if((Len&0xff)==0)
				{
					UARTCtrl->DAR=7;//类型不匹配           （7）
					return 0;
				}
			}
			Len>>=8;
			if(LEN_RxAPDU<(4+Len))
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(Len>pLib->LENmax)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return Len;
			}
			MW((u32)p8rx,pLib->Addr,Len);
			UARTCtrl->DAR=0;//数据访问结果;成功
			return Len;
		}
		else
		{//单个元素
			Len=SET_OAD_Element(PORTn,p8rx,(u8*)pLib->Addr,OAD&0xff,pLib->LENmax);//设置一个对象元素;返回:本元素的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
			return Len;
		}
	}
	else
	{//0表示整个对象属性，即对象的所有属性
		UARTCtrl->DAR=3;//数据访问结果;拒绝读写(3）
		return 0;
	}
}




























