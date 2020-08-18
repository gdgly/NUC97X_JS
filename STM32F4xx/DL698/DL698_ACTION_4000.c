
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_4000.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../MS/MS.h"
#include "../Terminal/Terminal_Init.h"
#include "../DL698/EVENT_Record.h"
#include "../Calculate/Calculate.h"
#include "../Device/RTC.h"



#include "../Terminal/Terminal_ReadMeter_ACSample.h"
#include "../Terminal/Terminal_ReadMeter_DL645_2007_FnData.h"
#include "../Terminal/Terminal_ReadMeter_SaveFnData.h"


void TEST_JCXL(void)
{
	u8 *p8;
	u32 *p32;
	u32 i;
	UARTCtrl_TypeDef *UARTCtrl1;
	UARTCtrl1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
	i=UARTCtrl1->Task;
//	if(MRR((Get_ADDR_UARTnRx(ACSAMPLEPORT)+1+7),1)==0x19)
	{//TEST
		p8=(u8 *)Get_ADDR_UARTnRx(ACSAMPLEPORT);
		p8[0]=0x68;
		MR(Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,ADDR_4001+2,6);//通信地址
		Data_Inverse((u32)(p8+1),6);//数据倒序(高低字节调换),入口Le>=2
		p8[7]=0x68;
		p8[8]=0x19;
		p8[9]=0x08;
		p8[10]=0x35;
		p8[11]=0x33;
		p8[12]=0x33;
		p8[13]=0x33;
		p8[14]=0x45;
		p8[15]=0x33;
		p8[16]=0x33;
		p8[17]=0x33;
		p8[18]=0x00;
		p8[19]=0x16;
		UARTCtrl1->RxByte=20;
		for(i=0;i<(UARTCtrl1->RxByte-2);i++)
		{
			p8[18]+=p8[i];
		}
	}
//	else
//	{
//		MC(0xaa,Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,6);//通信地址用0xaa
//	}
//		MC(0xaa,Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,6);//通信地址用0xaa
	i=UARTCtrl1->Task;//原任务寄存
	UARTCtrl1->Task=1;//1=接收完成
	if(FileDownload->File&0x04)//文件标志(置位表示OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
	{
		p32=(u32 *)(ADDR_Meter_ENTRY);
		(*(void(*)())p32[130])((u32)ACSAMPLEPORT);//调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送
	}
	UARTCtrl1->Task=i;//原任务恢复
	i=UARTCtrl1->TxByte;//发送字节数	
}


#if (USER/100)==17//江苏系
u32 ACTION_4048(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//????????;??:b31-b16????????(???4??OMD),b15-b0?????????????(???4??OMD),??????UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	p8d=p8d;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	i=p8rx[2];
	switch(i)
	{
		case 127:
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//??DL698??????(??:???????);??:LENmax??NAND??????ADDR_128KDATABUFF?,??:b31-b8??,b7-b0????
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//??????         (9)
				return 0;
			}
			if(i>LENmax_4048_127)
			{
				UARTCtrl->DAR=8;//??                 (8)
				return (LenRx<<16)+0;
			}
			//??????????
			MW((u32)p8rx,ADDR_4048_127,i);
			
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=4;//?????(4)
			return 0;
	}
}
#endif

//广播校时
u32 ACTION_4000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//操作一个对象方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	p8d=p8d;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	i=p8rx[2];
	switch(i)
	{
		case 127://方法127：广播校时
			p8rx+=4;
			LenRx=8;
			p8rx+=1;
			i=Check_pYYMDHMS_hex(p8rx);//年年月日时分秒合法检查,返回0合法,1非法
			if(i)
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
#if (USER/100)!=0//不是电科院测试
			//不是电科院测试需比较时钟差>5分，和日广播校时标志
			i=YYMDHMS_Sub_hex(p8rx,(u8*)ADDR_DL698YMDHMS);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
			if(i>>31)
			{
				i=~i;
				i++;
			}
			if(i>(5*60))//时钟差>5分
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			i=MRR(ADDR_BroadcasTimeFlag,1);//智能表日广播校时标志
			if(i)
			{//0=没广播校时，1=已广播校时
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			MWR(1,ADDR_BroadcasTimeFlag,1);//智能表日广播校时标志
#endif
			
//			//RS485-1
//			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
//			//其他任务
//			i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
//			i|=UARTCtrl->BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
//			i|=UARTCtrl->AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
//			i|=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
//			if(i==0)
//			{//无其他任务
//				UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+2;
//				UARTCtrl->BroadcastTimeTask=1;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
//			}
//			//RS485-2
//			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
//			//其他任务
//			i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
//			i|=UARTCtrl->BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
//			i|=UARTCtrl->AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
//			i|=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
//			if(i==0)
//			{//无其他任务
//				UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+2;
//				UARTCtrl->BroadcastTimeTask=1;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
//			}
//		#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//			//载波口广播校时
//			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//			//其他任务
//			i=UARTCtrl->ProxyTask;//代理任务:0=空,1=有代理,2...=正在代理过程,完成结束后回0
//			i|=UARTCtrl->BroadcastTimeTask;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
//			i|=UARTCtrl->AutoSearchTask;//自动搜表任务:0=空,1=等待,2...过程
//			i|=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
//			if(i==0)
//			{//无其他任务
//				UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+2;
//				UARTCtrl->BroadcastTimeTask=1;//广播校时任务:0=空,1=校时-2等待开始,2-127校时-2过程;0x81=校时-3等待开始,0x82-0xFF校时-3过程
//			}
//		#endif

			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
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
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=4;//对象未定义（4）
			return 0;
	}
}


//路由信息单元
u32 ACTION_4200(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//操作一个对象方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenRx=0;
	LenTx=0;
	Method=p8rx[2];
	p8rx+=4;
	switch(Method)
	{
//方法127：Add（路由信息单元）添加一个路由信息单元。
//方法128：AddBatch（array路由信息单元）批量添加路由信息单元。
//方法129：Update（TSA，路由信息单元）更新路由表的路由信息单元对象。
//方法134：Clear()//清空路由表
		case 127://方法127：Add（路由信息单元）添加一个路由信息单元。
			
		case 128://方法128：AddBatch（array路由信息单元）批量添加路由信息单元。
			
		case 129://方法129：Update（TSA，路由信息单元）更新路由表的路由信息单元对象。
			
		case 134://方法134：Clear()//清空路由表
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=4;//对象未定义           （4）
			return 0;
	}
}

//电气设备
#include "../DL698/CALC_RESET_FLOW_TIME.h"
u32 ACTION_4300(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//操作一个对象方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
  u8 * p8;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	//u32 i;
	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenRx=1;
	Method=p8rx[2];
	p8rx+=4;
	switch(Method)
	{
//1．复位	必选
//2．执行	可选
//3. 数据初始化	必选
//4. 恢复出厂参数	必选
//5. 事件初始化	必选
//6. 需量初始化	可选
		case 1://1．复位	必选
			Comm_Ram->RESET=0x55;//0x55=重启动(立即使用看门狗复位)
			Comm_Ram->ACTION_RESET_PORTn=PORTn;//操作复位的通信端口号
		#ifdef IC_NUC9xx
			//延时等待最后字节发送
			Comm_Ram->WaitUartCompleteMSTimer=5000/10;//等待UART帧返回定时器 每10ms减1定时器
		#endif
			return (LenRx<<16)+0;
		case 2:///2．执行	可选
			return (LenRx<<16)+0;
		case 3://3. 数据初始化	必选
      p8=(u8 *)Get_ADDR_UARTnRx(PORTn);
      Method=(p8[4]>>4)&0x3;
      if(Method==1)
      {//逻辑地址=1代表交采
        Method=MRR(ADDR_F101_2+1,1);//安全模式参数
        if(Method)
        {//启用安全模式SECURITY
          if((UARTCtrl->SECURITY_Request&0xC0)!=0xC0)//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID 
          {
            UARTCtrl->DAR=20;
            return (LenRx<<16)+0;
          }
        }
        //清SaveRam
        MC(0,ADDR_Meter_SaveRam_Start,LEN_Meter_SaveRam);
        //清EFLASH
        MC(0,ADDR_MeterEnergy_EFLASH_Start,(ADDR_MeterClr_End)-(ADDR_MeterEnergy_EFLASH_Start));
        MC(0,ADDR_Meter_EFLASH_Ext_Start,(ADDR_Meter_EFLASH_Ext_End)-(ADDR_Meter_EFLASH_Ext_Start));//仪表参数扩展
        return (LenRx<<16)+0;
      }
			Init_ALLDATA();//终端全部数据初始化(清0)
			Event_OAD(0x31000200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
#if (USER/100)==15//安微专变
//---重启动----
			Comm_Ram->RESET=0x55;//0x55=重启动(立即使用看门狗复位)
			Comm_Ram->ACTION_RESET_PORTn=PORTn;//操作复位的通信端口号
		#ifdef IC_NUC9xx
			//延时等待最后字节发送
			Comm_Ram->USBMSTimer=5000/10;//26 USB 每10MS减1定时器
		#endif
//---重启动 END----
#else
//---不重启动---
			//因电科院测试软件测流量超限功能bug,故不重启复位,但增加记录复位次数
			RESET_Count();//终端复位次数
//---不重启动 END---
#endif
			return (LenRx<<16)+0;
		case 4://4. 恢复出厂参数	必选
//参数∷=array OAD 将设备的配置恢复到出厂设置。
//参数定义的OAD列表，保持现状，不在恢复之列。
			#if (USER/100)==0//电科院				
				DL698_SETINIT(3,p8rx);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
			#else
				if(Comm_Ram->Factory==0x55)
				{//工厂模式
					DL698_SETINIT(3,p8rx);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
				}
				else
				{
					DL698_SETINIT(4,p8rx);//设置参数初始化:入口Command:3=参数初始化(即恢复至出厂配置),4/5=除与系统主站通信外的参数初始化,,6=仅部份参数初始化
				}
				Init_ALLDATA();//终端全部数据初始化(清0)
			#endif
			Event_OAD(0x31000200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			
			Comm_Ram->RESET=0x55;//0x55=重启动(立即使用看门狗复位)
			Comm_Ram->ACTION_RESET_PORTn=PORTn;//操作复位的通信端口号
		#ifdef IC_NUC9xx
			//延时等待最后字节发送
			Comm_Ram->WaitUartCompleteMSTimer=5000/10;//等待UART帧返回定时器 每10ms减1定时器
		#endif
			LenRx=p8rx[1];
			LenRx*=5;
			LenRx+=2;
			return (LenRx<<16)+0;
		case 5://5. 事件初始化	必选
			ms=Get_ms();
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
			{
				if(file->BLOCK_ENTRY)
				{
					if((file->FILEID&0xff)==2)
					{//文件分类=2,事件记录
						ms->msfile_Delete(filename);
					}
				}
				file++;
			}
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			for(filename=0;filename<recordFILENAMEmax_RAM;filename++)
			{
				if(file->BLOCK_ENTRY)
				{
					if((file->FILEID&0xff)==2)
					{//文件分类=2,事件记录
						ms->msfile_Delete(filename+FILENAMEmax_FLASH);
					}
				}
				file++;
			}
			//清事件当前值记录表
			MC(0,ADDR_EventCurrentvalue_3003,LENper_EventCurrentvalue*NUM_EventCurrentvalue);
			
			Event_OAD(0x31000200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			return (LenRx<<16)+0;
		case 6://6. 需量初始化	可选
//			//当前需量对象清零
//			UARTCtrl1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
//			p8rxbuff=(u8 *)Get_ADDR_UARTnRx(ACSAMPLEPORT);
//			p8txbuff=(u8 *)Get_ADDR_UARTnTx(ACSAMPLEPORT);
//			Task=UARTCtrl1->Task;//原任务寄存
//			UARTCtrl1->Task=1;//1=接收完成
//			UARTCtrl1->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
//			p8rxbuff[0]=0xFE;
//			p8rxbuff[1]=0xFE;
//			p8rxbuff[2]=0xFE;
//			p8rxbuff[3]=0xFE;
//			p8rxbuff+=4;
//			p8rxbuff[0]=0x68;
//			MC(0xaa,((u32)p8rxbuff)+1,6);
//			p8rxbuff[7]=0x68;
//			p8rxbuff[8]=0x19;//控制码
//			p8rxbuff[9]=8;//数据长度
//			p8rxbuff[10]=0x35;
////			p8rxbuff[11]=0x44;
////			p8rxbuff[12]=0x44;
////			p8rxbuff[13]=0x44;
//			p8rxbuff[11]=0x33;
//			p8rxbuff[12]=0x33;
//			p8rxbuff[13]=0x33;
//			p8rxbuff[14]=0x45;
//			p8rxbuff[15]=0x33;
//			p8rxbuff[16]=0x33;
//			p8rxbuff[17]=0x33;
//			p8rxbuff[18]=0x99;
//			p8rxbuff[19]=0x16;
//			UARTCtrl1->RxByte=24;
//			p32=(u32 *)(ADDR_Meter_ENTRY);
//			//MR((u32)p8rxbuff,(u32)p8txbuff,UARTCtrl->TxByte);
//			(*(void(*)())p32[130])((u32)ACSAMPLEPORT); //调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送
//			//MR((u32)p8rxbuff,(u32)p8txbuff,UARTCtrl->TxByte);
//			UARTCtrl1->Task=Task;//原任务恢复

			p8=(u8 *)Get_ADDR_UARTnRx(PORTn);
      Method=(p8[4]>>4)&0x3;
      if(Method==1)
      {//逻辑地址=1代表交采
        Method=MRR(ADDR_F101_2+1,1);//安全模式参数
        if(Method)
        {//启用安全模式SECURITY
          if((UARTCtrl->SECURITY_Request&0xC0)!=0xC0)//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID 
          {
            UARTCtrl->DAR=20;
            return (LenRx<<16)+0;
          }
        }
				TEST_JCXL();//清645交采需量
				return (LenRx<<16)+0;
			}
//			
//			
//			if(Comm_Ram->Factory==0x55)
//			{//是工厂模式
//				//保护工厂模式定时
//				i=MRR(ADDR_MeterFMTimer+14,2);//工厂模式定时
//				//清SaveRam
//				MC(0,ADDR_Meter_SaveRam_Start,LEN_Meter_SaveRam*2);
//				
//				//恢复工厂模式定时
//				MWR(i,ADDR_MeterFMTimer+14,2);//工厂模式定时
//				//清EFLASH
//				//MC(0,ADDR_MeterEnergy_EFLASH_Start,(ADDR_MeterClr_End)-(ADDR_MeterEnergy_EFLASH_Start));
//				//MC(0,ADDR_Meter_EFLASH_Ext_Start,(ADDR_Meter_EFLASH_Ext_End)-(ADDR_Meter_EFLASH_Ext_Start));//仪表参数扩展
//				MC(0,ADDR_MeterDemand_EFLASH_Start,(ADDR_MeterDemand_EFLASH_End)-(ADDR_MeterDemand_EFLASH_Start));
//				return (LenRx<<16)+0;
//			}
//			
			
			
			//			//当前需量对象清零
				
		
			Event_OAD(0x31000200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			return (LenRx<<16)+0;
		default:
			UARTCtrl->DAR=4;//对象未定义           （4）
			return (LenRx<<16)+0;
	}
}




























