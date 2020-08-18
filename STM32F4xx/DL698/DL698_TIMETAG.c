
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_TIMETAG.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"




u32 DL698_TIMETAG(u8* p8rx)//时间标签;入口:接收帧首址;返回:0=有效时间内,1=无效时间
{
	u32 i;
	u32 LEN_SA;
	u32 TI;
	u32 Len;
	
	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	switch(p8rx[3]&0x7)//应用层功能码
	{
		//0	保留	
		case 1://	链路管理	链路连接管理（登录，心跳，退出登录）
//LINK-APDU∷=CHOICE
//{
//  预连接请求	     [1]		LINK-Request，
//预连接响应       [129]	LINK-Response
//}
			return 0;//返回:0=有效时间内,1=无效时间
		//2	保留	
		case 3://	用户数据	应用连接管理及数据交换服务
//客户机应用层协议数据单元（Client-APDU）
//Client-APDU∷=SEQUENCE
//{
//  应用层服务  CHOICE
//  {
//    建立应用连接请求	 [2]		CONNECT-Request，
//    断开应用连接请求	 [3]		RELEASE-Request，
//    读取请求			 [5]		GET-Request，
//    设置请求 			 [6]		SET-Request，
//    操作请求 			 [7]		ACTION-Request，
//    上报应答 			 [8]		REPORT-Response，
//    代理请求   		 [9]		PROXY-Request
//}，
//时间标签    TimeTag  OPTIONAL
//}
			i=p8rx[LEN_SA+8];
			switch(i)
			{
				case 2://建立应用连接请求	 [2]		CONNECT-Request，
					return 0;//返回:0=有效时间内,1=无效时间
				case 3://断开应用连接请求	 [3]		RELEASE-Request，
					return 0;//返回:0=有效时间内,1=无效时间
				case 5://读取请求			 [5]		GET-Request，
					return 0;//返回:0=有效时间内,1=无效时间
				case 6://设置请求 			 [6]		SET-Request，
					return 0;//返回:0=有效时间内,1=无效时间
				case 7://操作请求 			 [7]		ACTION-Request，
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
							p8rx+=LEN_SA+8+3+4;
							Len=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
							p8rx+=Len;
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
							break;
					}
					break;
				case 8://上报应答 			 [8]		REPORT-Response，
					return 0;//返回:0=有效时间内,1=无效时间
				case 9://代理请求   		 [9]		PROXY-Request
					return 0;//返回:0=有效时间内,1=无效时间
				case 16://安全请求   		[16]		SECURITY-Request
					return 0;//返回:0=有效时间内,1=无效时间
				default:
					return 0;//返回:0=有效时间内,1=无效时间
			}
			break;
		//4…7	保留	
	}
	if(p8rx[0]==0)
	{//无时间标签
		return 0;//返回:0=有效时间内,1=无效时间
	}
	p8rx++;
	i=Check_pYYMDHMS_hex(p8rx);//年年月日时分秒合法检查,返回0合法,1非法
	if(i)
	{
		return 1;//返回:0=有效,1=无效
	}
	TI=(p8rx[8]<<8)+p8rx[9];//取频率值
	if(TI==0)
	{
		return 0;//返回:0=有效,1=无效
	}
	switch(p8rx[7])
	{
		case 0://秒      （0），
			break;
		case 1://分      （1），
			TI*=60;
			break;
		case 2://时      （2），
			TI*=60*60;
			break;
		case 3://日      （3），
		case 4://月      （4），
		case 5://年      （5）
		default:
			TI*=60*60*24;
			break;
	}
	i=YYMDHMS_Sub_hex((u8*)&Comm_Ram->DL698YMDHMS,p8rx);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
	if(i>>31)
	{
		i=~i;
		i++;
	}
	if(i>TI)
	{
		return 1;//返回:0=有效,1=无效
	}
	return 0;//返回:0=有效,1=无效
}

