

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_REPORT.h"
#include "../DL698/TASK.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/RM_Record.h"
#include "../DL698/EVENT_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"


void RM_Monitor(u32 ID,u32 NUM_Tx,u32 NUM_Rx);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数,NUM_Rx=接收条数

//REPORT-Notification∷=CHOICE
//{
//上报若干个对象属性         [1] ReportNotificationList，
//上报若干个记录型对象属性   [2] ReportNotificationRecordList
//}


void REPORT_PORTn(msFILE_TypeDef *file)//通道不需上报时置已上报数据个数=当前数据个数；使用ADDR_DATABUFF
{
/*
	u32 i;
	u32 n;
	u32 OI;
	u32 NUM_DATA;
	u8* p8;
	
	NUM_DATA=file->NUM_DATA;
	MR(ADDR_DATABUFF,ADDR_4300_10,LENmax_4300_10);//上报通道
	p8=(u8*)ADDR_DATABUFF;
	n=p8[1];//数组数
	p8+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8[1]<<8)|p8[2];
		if(OI==0x4500)//GPRS
		{
			break;
		}
		p8+=5;
	}
	if(i>=n)
	{//GPRS非上报通道
		for(i=0;i<NUMmax_601C;i++)
		{
			file->NUM_GPRSreport[i]=NUM_DATA;
		}
	}
	p8=(u8*)ADDR_DATABUFF;
	p8+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8[1]<<8)|p8[2];
		if(OI==0x4510)//ENET
		{
			break;
		}
		p8+=5;
	}
	if(i>=n)
	{//ENET非上报通道
		for(i=0;i<NUMmax_601C;i++)
		{
			file->NUM_ENETreport[i]=NUM_DATA;
		}
	}
*/
}
	
void REPORT_complete(u32 PORTn)//主动上报完成后处理(定时任务)
{
	u32 i;
	u32 x;
	u32 n;
	u32 id;
	u32 filename;
	msFILE_TypeDef *file;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	x=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
	UARTCtrl->REPORT_NUM_File=0;//1帧主动上报数据来自不同文件的个数
	
	if(UARTCtrl->HostACK==5)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
	{
		id=UARTCtrl->TaskID;
		id&=0xff;//上报方案号
		if(id<NUMmax_601C)
		{
			if((x!=0)&&(x<=REPORT_NUMFILEmax))
			{
				Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
				for(i=0;i<x;i++)
				{
					filename=UARTCtrl->REPORT_filename[i];//当前正在上报数据的文件名,0xffff表示无效文件名
					if(filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
					{
						if(filename<recordFILENAMEmax_FLASH)
						{
							file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
						}
						else
						{
							file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
						}
						n=UARTCtrl->REPORT_NUM_Data[i];
					#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
						if(PORTn==GPRSPORT)
						{
							file->NUM_GPRSreport[id]+=n;//当前正在上报数据的文件中包含有电能表的个数或事件个数
						}
						else
						{
							file->NUM_ENETreport[id]+=n;//当前正在上报数据的文件中包含有电能表的个数或事件个数
						}
					#else
						file->NUM_GPRSorENETreport[id]+=n;//当前正在上报数据的文件中包含有电能表的个数或事件个数
					#endif
					}
				}
			}
		}
		UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
	}
}

void REPORT_END(u32 PORTn)//主动上报任务结束时间(定时任务)
{
	u32 i;
	u32 id;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	id=(i>>8)&0xff;//任务号
	if(id>=NUMmax_6012_2)
	{
		return;
	}
	UARTCtrl->TaskTime[id].Run=2;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
	for(i=0;i<7;i++)
	{
		UARTCtrl->TaskTime[id].EndTime[i]=Comm_Ram->DL698YMDHMS[i];//任务执行结束时间YYMDHMS
	}
}

//#define ADDR_601C     ADDR_6018+LENmax_6018///上报方案集(初始化值0x00)
//属性2（上报方案 report plan）∷=structure//删除时structure的数据单元清为0(NULL)
//{
//方案编号          unsigned，
//上报通道          array OAD，
//上报响应超时时间  TI，
//最大上报次数      unsigned，
//上报内容          strcuture
//{
//  类型  unsigned，
//  数据  Data
//}
//}
//上报内容：
//上报类型	  上报内容	表示
// 0	      OAD	对象属性数据
// 1	      RecordData	上报记录型对象属性
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//	记录型对象属性描述符  RCSD，
//	行选择                RSD
//}
//#define LENper_601C     512
#include "../DL698/DL698_JSON.h"
void REPORT_Notification(u32 PORTn)//上报通知(定时任务)
{
	u32 i;
	u32 x;
	u32 n;
	u32 TaskID;
	u32 id;
	u32 reportid;
	u32 fileid;
	u32 OAD;
	u32 LEN_Tx;
	//u32 TxByte;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//最大还可发送的字节数
	u8* p8;
	u32 NUM_RCSD;
	u32 LEN_RCSD;
	u8* pRCSD;
	u8* pRSD;
	u32 fRSD;//行选择有效标志
	u32 FILECOUNT;
	u32 FILE_OFFSET;
	u32 fDATA;//有数据标志
	u32 NUM_READFILE;//读文件总次数
	u16* p16time;
	u8* p8tx;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//抄表记录文件头文件
	u32 ProjectType;//方案类型
	
	
	if((PORTn!=GPRSPORT)&&(PORTn!=ENETPORT))
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Task!=0)
	{
		return;
	}
	if(UARTCtrl->LinkTask!=101)
	{//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
		return;
	}
	//myprintf("111111111111111111111111111\n", 0, 0, 0);

//---用RS232口测试上报---
	
//	if(PORTn!=RS232PORT)
//	{
//		return;
//	}
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//	if(UARTCtrl->Task!=0)
//	{
//		return;
//	}
	
//---用RS232口测试上报 END---
	

	//myprintf("UARTCtrl->HostACK=%d\n", UARTCtrl->HostACK, 0, 0);
	if((UARTCtrl->HostACK)&&((UARTCtrl->HostACK!=5)&&(UARTCtrl->HostACK!=8)))//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
	{//有其他主动上报等待确认返回
		return;
	}
	i=Comm_Ram->DL698YMDHMS[6];
	if(UARTCtrl->REPORT_RTCS==i)//空闲时进入上报的RTC秒
	{
		//myprintf("空闲时进入上报的RTC秒\n", 0, 0, 0);
		return;
	}
	if(i&1)
	{
		return;//当前时钟奇数秒,奇数报事件,偶数报定时任务
	}
	UARTCtrl->REPORT_RTCS=i;//空闲时进入上报的RTC秒
	i=MRR(ADDR_4300_8+1,1);//允许主动上报
	if(i==0)
	{//不允许主动上报
		UARTCtrl->REPORT_NUMmax=0;//若主站没应答最大还需重发上报次数,每上报1次减1
		//不需主动上报的文件数据上报计数处理
		/*
		Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化(1=管理数据保存,2=不需主动上报处理)
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
		{
			if(filename==recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			}
			if(file->BLOCK_ENTRY)
			{
				x=file->NUM_DATA;
				for(i=0;i<RMTASKmax;i++)
				{
					file->NUM_REPORT[i]=x;
				}
			}
			file++;
		}
		*/
		return;
	}
	switch(PORTn)
	{
		case GPRSPORT:
			p16time=(u16*)&Terminal_Ram->GPRS_WaitHostACK_S_Timer;//29 GPRS主动上报等待主站确认秒定时器
			break;
		case ENETPORT:
			p16time=(u16*)&Terminal_Ram->ENET_WaitHostACK_S_Timer;//30 ENET主动上报等待主站确认秒定时器
			break;
		default:
		case RS232PORT:
			p16time=(u16*)&Terminal_Ram->RS232_WaitHostACK_S_Timer;//31 RS232主动上报等待主站确认秒定时器
			break;
	}
	if(p16time[0]!=0)
	{//上报响应超时时间没到
		return;
	}
	i=UARTCtrl->REPORT_NUMmax;//若主站没应答最大还需重发上报次数,每上报1次减1
	if(i)
	{
		i--;
		UARTCtrl->REPORT_NUMmax=i;//若主站没应答最大还需重发上报次数,每上报1次减1
		p16time[0]=UARTCtrl->REPORT_OVER_SECOND;//上报响应超时时间寄存
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
		i=Get_LEN_UARTnTx(PORTn);
		p8+=i;
		LEN_Tx=p8[1]|(p8[2]<<8);
		LEN_Tx+=2;
		if(LEN_Tx<=i)
		{
			MR((u32)p8tx,(u32)p8,LEN_Tx);
			UARTCtrl->TxByte=LEN_Tx;
			UARTCtrl->Task=2;//2=等待单帧数据发送
		}
		return;
	}
	REPORT_complete(PORTn);//当前主动上报完成后处理(定时任务)
	
#if(Project/100==2)&&(USER/100==17)//江苏集中器
	if(Terminal_Router->MinuteCollect!=0)
	{
		i=Report_JsonFile(p16time,PORTn);
		if(i)
		{
			return;
		}
	}
#endif
//心跳优先
	if(PORTn==GPRSPORT)
	{
		if(Terminal_Ram->GPRSHeartBeat_S_Timer==0)//GPRS心跳秒定时器
		{
			#if(Project/100==2)&&(USER/100==17)//江苏集中器
			if(UARTCtrl->HostACK==8)//江苏
			{
				UARTCtrl->HostACK=3;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
			}
			#endif
			return;
		}
	}
	if(PORTn==ENETPORT)
	{
		if(Terminal_Ram->ENETHeartBeat_S_Timer==0)//以太网心跳秒定时器
		{
			#if(Project/100==2)&&(USER/100==17)//江苏集中器
			if(UARTCtrl->HostACK==8)//江苏
			{
				UARTCtrl->HostACK=3;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
			}
			#endif
			return;
		}
	}
	
	

	TASK(PORTn,0);//任务;入口:RM=0上报任务,RM=1采集任务
	TaskID=UARTCtrl->TaskID;
	id=(TaskID>>8)&0xff;//任务号
	#if (USER/100)==15//安徽
	if(id==16)
	{
		n=2;//任务ID=2
		RM_Monitor(2,0,0);
		p8=(u8*)ADDR_6034+(n*LENper_6034);
		p8=Get_Element(p8,2,0,0);
		if(p8)
		{
			if(p8[0]!=0x16||p8[1]!=0x02)
			{
				return;
			}
		}
		else
			return;
	}
	#endif
	if(TaskID)
	{//有上报任务
//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//b31-b28优先级:1=首要,2=必要,3=需要,4=可能
//b27-b24方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//b23-b20普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集,4=补抄
//b19-b16普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标,7=相对上月月末23点59分
		if(id>=NUMmax_6012_2)
		{
			return;
		}
		if(UARTCtrl->TaskTime[id].Run==2)//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
		{
			return;
		}
		reportid=TaskID&0xff;//上报方案号
		if(reportid>=NUMmax_601C)
		{//方案编号超
			REPORT_END(PORTn);//主动上报任务结束时间
			return;
		}
		p8=(u8*)ADDR_601C_SDRAM+((TaskID&0xff)*LENper_601C);
		if(p8[0]!=DataType_structure)
		{//删除
			REPORT_END(PORTn);//主动上报任务结束时间
			return;
		}
		//上报方案中的上报通道
		x=PORTntoOAD(PORTn);//通信口号转为OAD值
		p8+=4;
		n=p8[1];//数组数
		p8+=2;
		x>>=16;
		for(i=0;i<n;i++)
		{
			OAD=(p8[1]<<8)|p8[2];
			if(OAD==x)
			{
				break;
			}
			p8+=5;
		}
		if(i>=n)
		{//非上报端口
/*
			if(reportid!=0)//上报方案号
			{
				Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
				for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
				{
					if(filename==recordFILENAMEmax_FLASH)
					{
						file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
					}
					if(file->BLOCK_ENTRY)
					{
						x=file->NUM_DATA;
						if(PORTn==GPRSPORT)
						{
							file->NUM_GPRSreport[reportid]=x;
						}
						else
						{
							file->NUM_ENETreport[reportid]=x;
						}
					}
					file++;
				}
			}
*/
			REPORT_END(PORTn);//主动上报任务结束时间
			return;//非上报通道
		}
		//
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
		//上报内容
		p8=(u8*)ADDR_601C_SDRAM+((TaskID&0xff)*LENper_601C);
		p8=Get_Element(p8,5,0,0);//计算元素地址,使用ADDR_128KDATABUFF
		if(p8==0)
		{
			REPORT_END(PORTn);//主动上报任务结束时间
			return;
		}
		if(p8[3]==0)
		{//0=OAD	对象属性数据
			REPORT_END(PORTn);//主动上报任务结束时间(定时任务)
			p8+=4;
			if(p8[0]==DataType_array)
			{
				n=p8[1];
				p8+=2;
			}
			else
			{
				n=1;
			}
		//APDU：88 01 01 01 40 00 02 00 01 1C 07 E0 0B 0E 00 02 00 00 00 
			p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=0x88;//上报通知 REPORT-Notification
			p8tx[1]=0x01;//上报若干个对象属性         [1] ReportNotificationList
									//上报若干个记录型对象属性   [2] ReportNotificationRecordList
			i=UARTCtrl->REPORT_PIID;
			i++;
			i&=0x3f;
			UARTCtrl->REPORT_PIID=i;
			p8tx[2]=i;//PIID
			p8tx[3]=0;//SEQUENCE OF OAD，个数=?
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=LEN_TxAPDU+2;//保留2byte(1byte时间标签,1byte上报信息FollowReport OPTIONAL）
			
			while(n--)
			{
				if(LENmax_TxSPACE<6)
				{
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8+1,4);
				LENmax_TxSPACE-=4;
				UARTCtrl->NEXT=0;//DL698分帧标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧
				i=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
				p8+=5;
				i=GET_OAD_DATA(PORTn,i,p8tx+LEN_TxAPDU+4+1,LENmax_TxSPACE-1);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
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
					p8tx[LEN_TxAPDU]=1;//Get-Result∷=CHOICE,  [1]   DATA
					LEN_TxAPDU+=i+1;
					LENmax_TxSPACE-=i+1;
				}
				p8tx[3]++;//SEQUENCE OF OAD，个数=?
			}
			p8tx[LEN_TxAPDU+0]=0;//时间标签
			p8tx[LEN_TxAPDU+1]=0;//上报信息FollowReport OPTIONAL
			LEN_TxAPDU+=2;
			
		}
		else
		{//1=RecordData	上报记录型对象属性
			//先计算上报的数据在抄表方案后,然后由方案号确定任务号用以文件搜索.
			p8+=6;
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//	记录型对象属性描述符  RCSD，
//	行选择                RSD
//}
//02 03
//51 60 12 03 00 主对象属性描述符      OAD
//60 01 RCSD∷=SEQUENCE OF CSD
//01 50 04 02 00 02 00 10 02 00 00 20 02 00 
//5a 00 行选择                RSD
			//主对象属性描述符 OAD
			OAD=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
			switch(OAD)
			{
				case 0x60120300:
					ms=(ms_Type *)Get_ms();
					pRCSD=p8+5;//记录型对象属性描述符  RCSD
					i=Get_DL698DataLen_S(pRCSD,0);//得到DL698数据类型长度(包括:类型字节和数据)
				
					pRSD=pRCSD+i;//行选择 RSD
					fileid=RCSDtoFileID(pRCSD);//记录列选择RCSD相对应的FileID,返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO字段=0xff;RCSD中只有通信地址等非抄表类数据时返回0xffffffff
					if((fileid==0)||(fileid==0xffffffff))
					{//0表示没找到,无效FileID
						UARTCtrl->TaskTime[id].Run=2;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
						return;
					}
					ProjectType=fileid>>28;//方案类型
					if(ProjectType==1)
					{//普通采集方案
						fileid>>=20;
					}
					else
					{
						fileid>>=12;
						fileid&=0xff;
					}
					//找文件中数据上报
//TxAPDU：
//88 上报通知
//02 上报若干个记录型对象属性
//02 PIID
//01 SEQUENCE OF A-ResultNormal
//60 12 03 00  OAD
//01 RCSD，SEQUENCE OF个数=1
//01 50 04 02 00 02 00 10 02 00 00 20 02 00 //RCSD
//01 记录数据
//02 M条记录，M=2
//01 02 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 
//01 02 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 
//00 00 
					p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
					p8tx[0]=0x88;//上报通知 REPORT-Notification
					p8tx[1]=0x02;//上报若干个对象属性[1] ReportNotificationList,上报若干个记录型对象属性   [2] ReportNotificationRecordList
					i=UARTCtrl->REPORT_PIID;
					i++;
					i&=0x3f;
					UARTCtrl->REPORT_PIID=i;
					p8tx[2]=i;//PIID
					//SEQUENCE OF A-ResultNormal
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
					p8tx[3]=1;//SEQUENCE OF A-ResultNormal
					p8tx[4]=0x60;
					p8tx[5]=0x12;
					p8tx[6]=0x03;
					p8tx[7]=0x00;
					//RCSD总长度
					NUM_RCSD=pRCSD[1];//RCSD，SEQUENCE OF个数
					LEN_RCSD=0;
					for(n=0;n<NUM_RCSD;n++)
					{
						x=pRCSD[2+LEN_RCSD];
						if(x==0)
						{//OAD
							LEN_RCSD+=5;
						}
						else
						{//ROAD
							x=pRCSD[2+LEN_RCSD+5];
							x*=4;
							x+=6;
							LEN_RCSD+=x;
						}
					}
					p8tx[8]=NUM_RCSD;
					MR((u32)p8tx+9,(u32)pRCSD+2,LEN_RCSD);
					p8tx[9+LEN_RCSD]=1;//记录数据
					p8tx[10+LEN_RCSD]=0;//M条记录
					
					LEN_TxAPDU=11+LEN_RCSD;
					LENmax_TxSPACE-=LEN_TxAPDU+2;//保留2byte(1byte时间标签,1byte上报信息FollowReport OPTIONAL）
					
					fDATA=0;//有数据标志;0=没,1=有,2=表示发送缓冲不够
					NUM_READFILE=0;//读文件总次数
					UARTCtrl->REPORT_NUM_File=0;//1帧主动上报数据来自不同文件的个数
					FILECOUNT=96;//先上报文件计数大的数据
					
					while(1)
					{
						if(UARTCtrl->REPORT_NUM_File>=REPORT_NUMFILEmax)//上报数据的不同文件最大数
						{
							break;
						}
						if(fDATA==2)//有数据标志;0=没,1=有,2=表示发送缓冲不够
						{
							break;
						}
						file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
						for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
						{
							if(fDATA==2)//有数据标志;0=没,1=有,2=表示发送缓冲不够
							{
								break;
							}
							if(filename==recordFILENAMEmax_FLASH)
							{
								file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
							}
							if(file->BLOCK_ENTRY)
							{
								if(file->FILECOUNT==FILECOUNT)
								{//计数号相同
									i=file->FILEID;
									if((i&0xff)==0)
									{//文件类型=0
										if(ProjectType==1)//方案类型
										{//普通采集方案
											i>>=20;
										#if (USER/100)==0//是电科院测试
										//---电科院测试软件，上报曲线数据为上上个任务时间数据---
											if(((i>>4)&0xf)==3)
											{//采集类型=3,曲线数据
												p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
												p8+=4+1;//执行频率   TI
												n=pTItoSECOND(p8);//TI转为秒值,入口:pTI无数据类型,出口:hex秒
												MR(ADDR_DATABUFF,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
												pYYMDHMS_SubS_hex((u8*)ADDR_DATABUFF,n);//年年月日时分秒减S秒,返回减S秒后的年年月日时分秒
												n=Compare_DL698DataString(file->save_date_time_s,(u8*)ADDR_DATABUFF,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
												if(n!=2)
												{//没到上报时间,暂不报
													i=~fileid;//使i!=fileid
												}
											}
										//---电科院测试软件，上报曲线数据... RND---
										#endif
										}
										else
										{
											i>>=12;
											i&=0xff;
										}
										if(i==fileid)
										{//采集相同
										#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
											if(PORTn==GPRSPORT)
											{
												n=file->NUM_GPRSreport[reportid];
											}
											else
											{
												n=file->NUM_ENETreport[reportid];
											}
										#else
											n=file->NUM_GPRSorENETreport[reportid];
										#endif
											if(file->NUM_DATA>n)
											{//有数据需上报
												//读记录头
												ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,NUM_RMmax*sizeof(RecordFileHead_TypeDef));
												RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
												FILE_OFFSET=0;//计算数据地址
												for(i=0;i<n;i++)
												{
													FILE_OFFSET+=RecordFileHead->LEN_DATA;
													RecordFileHead++;
												}
												i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
												if(i>=REPORT_NUMFILEmax)//上报数据的不同文件最大数
												{
													break;
												}
												UARTCtrl->REPORT_filename[i]=filename;//1帧主动上报数据不同文件名列表
												UARTCtrl->REPORT_NUM_Data[i]=0;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
												i++;
												UARTCtrl->REPORT_NUM_File=i;//1帧主动上报数据来自不同文件的个数
												while(1)
												{
													i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
													if(i)
													{
														i--;
													}
													n=UARTCtrl->REPORT_NUM_Data[i];
												#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
													if(PORTn==GPRSPORT)
													{
														n+=file->NUM_GPRSreport[reportid];
													}
													else
													{
														n+=file->NUM_ENETreport[reportid];
													}
												#else
													n+=file->NUM_GPRSorENETreport[reportid];
												#endif
													if(file->NUM_DATA<=n)
													{//没数据需上报
														break;
													}
													i=RecordFileHead->LEN_DATA;
													NUM_READFILE++;//读文件总次数
													if(NUM_READFILE>127)//读文件总次数
													{//读文件总次数，限制每次运行时间
														break;
													}
													if(p8tx[10+LEN_RCSD]>=127)//M条记录
													{
														fDATA=2;//有数据标志;0=没,1=有,2=表示发送缓冲不够
														break;
													}
													//取记录数据
													ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+FILE_OFFSET,(u8*)ADDR_128KDATABUFF+(64*1024),i);
													fRSD=0;//行选择有效标志
													switch(pRSD[1])//行选择
													{
														case 0:
												//不选择       [0]  NULL
															fRSD=1;//行选择有效标志
															break;
														case 1:
												//Selector1∷=SEQUENCE
												//{
												//  对象属性描述符  OAD，
												//  数值            Data
												//}
															fRSD=0;//行选择有效标志
															break;
														case 2:
												//Selector2∷=SEQUENCE
												//{
												//  对象属性描述符  OAD，
												//  起始值          Data，
												//  结束值          Data，
												//  数据间隔        Data
												//}
															fRSD=0;//行选择有效标志
															break;
														case 3:
												//Selector3∷=SEQUENCE OF Selector2
															fRSD=0;//行选择有效标志
															break;
														case 4:
												//Selector4∷=SEQUENCE
												//{
												//  采集启动时间  date_time_s，
												//  电能表集合    MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//数据的电能表地址
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+7);//比较TSA是否符合MS集合,入口:pADDR_6000已在RAM,pTSA,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
															break;
														case 5:
												//Selector5∷=SEQUENCE
												//{
												//  采集存储时间  date_time_s，
												//  电能表集合    MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//数据的电能表地址
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+7);//比较TSA是否符合MS集合,入口:pADDR_6000已在RAM,pTSA,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
															break;
														case 6:
												//Selector6∷=SEQUENCE
												//{
												//  采集启动时间起始值  date_time_s，
												//  采集启动时间结束值  date_time_s，
												//  时间间隔            TI，
												//  电能表集合          MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//数据的电能表地址
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+17);//比较TSA是否符合MS集合,入口:pADDR_6000已在RAM,pTSA,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
															break;
														case 7:
												//Selector7∷=SEQUENCE
												//{
												//  采集存储时间起始值  date_time_s，
												//  采集存储时间结束值  date_time_s，
												//  时间间隔            TI，
												//  电能表集合          MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//数据的电能表地址
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+17);//比较TSA是否符合MS集合,入口:pADDR_6000已在RAM,pTSA,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
															break;
														case 8:
												//Selector8∷=SEQUENCE
												//{
												//  采集成功时间起始值  date_time_s，
												//  采集成功时间结束值  date_time_s，
												//  时间间隔            TI，
												//  电能表集合          MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//数据的电能表地址
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+17);//比较TSA是否符合MS集合,入口:pADDR_6000已在RAM,pTSA,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
															break;
														case 9:
												//Selector9∷=SEQUENCE
												//{
												//  上第n次记录  unsigned
												//}
															if(file->FILECOUNT<=1)
															{//上报最近1次
																fRSD=1;//行选择有效标志
															}
															else
															{
																fRSD=0;//行选择有效标志
															}
															break;
														case 10:
												//Selector10∷=SEQUENCE
												//{
												//上n条记录  unsigned，
												//电能表集合  MS
												//}
															if(file->FILECOUNT<=1)
															{//上报最近1次
																p8=(u8*)ADDR_DATABUFF;
																p8[0]=DataType_TSA;
																p8[1]=7;
																p8[2]=5;
																MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//数据的电能表地址
																fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+1);//比较TSA是否符合MS集合,入口:pADDR_6000已在RAM,pTSA,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
															}
															else
															{
																fRSD=0;//行选择有效标志
															}
															break;
														default:
															fRSD=0;//行选择有效标志
															break;
													}
													if(fRSD)//行选择有效标志
													{//行选择后有数据需上报
														p8=(u8*)(ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)));//数据的电能表地址
														i=Get_RecordFileData_RCSD(pRCSD+1,p8,(u8*)ADDR_128KDATABUFF+(64*1024),(u8*)ADDR_DATABUFF,LEN_DATABUFF,0);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
														if(i)
														{
															if(i<=LENmax_TxSPACE)
															{
																MR((u32)p8tx+LEN_TxAPDU,ADDR_DATABUFF,i);
																LEN_TxAPDU+=i;
																LENmax_TxSPACE-=i;
																p8tx[10+LEN_RCSD]++;//M条记录
																fDATA=1;//有数据标志;0=没,1=有,2=表示发送缓冲不够
															}
															else
															{
																if(fDATA)//有数据标志;0=没,1=有,2=表示发送缓冲不够
																{
																	fDATA=2;//有数据标志;0=没,1=有,2=表示发送缓冲不够
																	break;
																}
																//若单个数据缓冲不够则放弃本数据上报
																
															}
														}
													}
													i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
													if(i)
													{
														i--;
													}
													UARTCtrl->REPORT_NUM_Data[i]++;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
													FILE_OFFSET+=RecordFileHead->LEN_DATA;
													RecordFileHead++;
												}
											}
										}
									}
								}
							}
							file++;
						}
						if(FILECOUNT==0)
						{
							break;
						}
						FILECOUNT--;
					}
					if(fDATA==0)//有数据标志
					{//没上报数据
						if(UARTCtrl->REPORT_NUM_File==0)//1帧主动上报数据来自不同文件的个数
						{
							REPORT_END(PORTn);//主动上报任务结束时间
						}
						else
						{
							//文件中已处理过的数据个数
							UARTCtrl->HostACK=5;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
							REPORT_complete(PORTn);//当前主动上报完成后处理(定时任务)
						}
						return;
					}				
					p8tx[LEN_TxAPDU+0]=0;//时间标签
					p8tx[LEN_TxAPDU+1]=0;//上报信息FollowReport OPTIONAL
					LEN_TxAPDU+=2;
					break;
				default:
					UARTCtrl->TaskTime[id].Run=2;//任务执行状态:0=未执行,1=执行中,2=已执行,3=执行中暂停,4=执行未完成
					return;
			}
		}
		
		i=MRR(ADDR_F101_2+1,1);//读取安全模式参数
		if(i==0)
		{//0—不启用安全模式参数、默认安全模式参数
			p8tx-=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=0x68;
			//2byte长度域L
			p8tx[3]=0x83;//控制域C
			p8tx[4]=5;//TSA
			MR((u32)p8tx+5,ADDR_4001+2,6);
			Data_Inverse((u32)p8tx+5,6);//数据倒序(高低字节调换),入口Le>=2
			p8tx[11]=0;//客户机地址CA
			//2byte帧头校验HCS
			MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
			//2byte校验
			LEN_Tx=14+LEN_TxAPDU+2;
			p8tx[LEN_Tx]=0x16;
			LEN_Tx++;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
		}
		else
		{//1—启用安全模式参数、默认安全模式参数
			p8tx-=TxAPDUOffset;//TxAPDU存放开始地址
			p8tx[0]=0x68;
			//2byte长度域L
			p8tx[3]=0x83;//控制域C
			p8tx[4]=5;//TSA
			MR((u32)p8tx+5,ADDR_4001+2,6);
			Data_Inverse((u32)p8tx+5,6);//数据倒序(高低字节调换),入口Le>=2
			p8tx[11]=0;//客户机地址CA
			//2byte帧头校验HCS
			p8tx[14]=0x10;//0x10,0x00,0x13,//安全传输
			p8tx[15]=0x00;//安全类别
			//明文长度
			if(LEN_TxAPDU<127)
			{
				p8tx[16]=LEN_TxAPDU;
				LEN_Tx=17;
			}
			else
			{
				p8tx[16]=0x82;
				p8tx[17]=LEN_TxAPDU>>8;
				p8tx[18]=LEN_TxAPDU;
				LEN_Tx=19;
			}
			i=TESAM_CMD_TxRx(0x80140103,LEN_TxAPDU,(u32)p8tx+TxAPDUOffset);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{//ESAM验证失败
				UARTCtrl->REPORT_NUM_File=0;//1帧主动上报数据来自不同文件的个数
				return;
			}
			MR((u32)p8tx+LEN_Tx,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
			LEN_Tx+=LEN_TxAPDU;
			//02 0C 82 07 EF 22 99 AD 50 09 00 00 00 0F 04 FB 43 41 D0
			p8tx[LEN_Tx]=2;//随机数+数据MAC  [2]  RN_MAC
			p8tx[LEN_Tx+1]=12;
			MR((u32)p8tx+LEN_Tx+2,ADDR_UART7Rx+5,12);
			p8tx[LEN_Tx+2+12]=4;
			MR((u32)p8tx+LEN_Tx+15,ADDR_UART7Rx+5+12,4);
			LEN_Tx+=19;
			LEN_Tx+=3;//2byte校验,1byte 0x16
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
		}
		
		Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
		UARTCtrl->Task=2;//2=等待单帧数据发送
		//发送帧缓存在DataBuff+LEN_UARTnTx,用以重发
		i=Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
		i+=Get_LEN_UARTnTx(PORTn);
		MW((u32)p8tx,i,LEN_Tx+2);
		//上报响应超时时间  TI
		p16time[0]=WaitHostACKMinTimer;//默认最小值
		UARTCtrl->REPORT_OVER_SECOND=WaitHostACKMinTimer;//上报响应超时时间寄存
		UARTCtrl->REPORT_NUMmax=0;//若主站没应答最大还需重发上报次数,每上报1次减1
		UARTCtrl->HostACK=5;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
		p8=(u8*)ADDR_601C_SDRAM+((TaskID&0xff)*LENper_601C);
		p8=Get_Element(p8,3,0,0);//计算元素地址,使用ADDR_128KDATABUFF
		if(p8)
		{
			i=MRR((u32)p8+1,3);
			i=TItoSECOND(i);//TI转为秒值,出口:hex秒
			if(i>WaitHostACKMaxTimer)
			{
				i=WaitHostACKMaxTimer;
			}
			if(i<WaitHostACKMinTimer)
			{
				i=WaitHostACKMinTimer;
			}
			p16time[0]=i;
			UARTCtrl->REPORT_OVER_SECOND=i;//上报响应超时时间寄存
		//最大上报次数
			i=p8[5];
			if(i>=1)
			{
				i--;
				UARTCtrl->REPORT_NUMmax=i;//若主站没应答最大还需重发上报次数,每上报1次减1
				if(i==0)
				{
					p16time[0]=0;
				}
			}
		}

	}
}




void EVENT_REPORT_complete(u32 PORTn,u32 ACK)//主动上报完成后处理(事件),入口:ACK=0没确认,1=确认
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 n;
	u32 filename;
	msFILE_TypeDef *file;
	UARTCtrl_TypeDef *UARTCtrl;
	EventFileHead_TypeDef* EventFileHead;
	ms_Type *ms;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	x=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
	UARTCtrl->REPORT_NUM_File=0;//1帧主动上报数据来自不同文件的个数
	
	if(UARTCtrl->HostACK==4)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
	{
		ms=(ms_Type *)Get_ms();
		if((x!=0)&&(x<=REPORT_NUMFILEmax))
		{
			Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
			for(i=0;i<x;i++)
			{
				filename=UARTCtrl->REPORT_filename[i];//当前正在上报数据的文件名,0xffff表示无效文件名
				if(filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
				{
					if(filename<recordFILENAMEmax_FLASH)
					{
						file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
					}
					else
					{
						file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
					}
					n=UARTCtrl->REPORT_NUM_Data[i];
					if(n)
					{
					#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
						if(PORTn==GPRSPORT)
						{
							z=file->NUM_GPRSreport[0];//已上报数
							file->NUM_GPRSreport[0]=z+n;//当前正在上报数据的文件中包含有电能表的个数或事件个数
						}
						else
						{
							z=file->NUM_ENETreport[0];//已上报数
							file->NUM_ENETreport[0]=z+n;//当前正在上报数据的文件中包含有电能表的个数或事件个数
						}
					#else
							z=file->NUM_GPRSorENETreport[0];//已上报数
							file->NUM_GPRSorENETreport[0]=z+n;//当前正在上报数据的文件中包含有电能表的个数或事件个数
					#endif
						if((file->FILEID&0xff)!=2)
						{//非事件记录文件2,是0=全事件采集记录或5=DL645电能表主动上报
							continue;
						}
						//事件已上报标志
						y=file->NUM_DATA;
						ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,y*sizeof(EventFileHead_TypeDef));
						EventFileHead=(EventFileHead_TypeDef*)(ADDR_128KDATABUFF+(z*sizeof(EventFileHead_TypeDef)));
						while(n--)
						{
							if(z<y)
							{
								if(EventFileHead->REPORT&1)//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
								{
									if(PORTn==GPRSPORT)
									{
										EventFileHead->REPORT|=2;//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
									}
									else
									{
										EventFileHead->REPORT|=4;//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
									}
									if(ACK)
									{
										EventFileHead->ACK=ACK;//上报确认:0=没确认,1=确认
									}
								}
								EventFileHead++;
								z++;
							}
						}
						ms->msfile_write(filename,0,(u8*)ADDR_128KDATABUFF,y*sizeof(EventFileHead_TypeDef));
					}
				}
			}
		}
		UARTCtrl->HostACK=0;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
	}
}


void EVENT_REPORT_Notification(u32 PORTn)//上报通知(事件)
{
	u32 i;
	u32 x;
	u32 n;
	u32 fileid;
	u32 OI;
	u32 LEN_Tx;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//最大还可发送的字节数
	u8* p8;
//	u32 FILECOUNT;
	u32 FILE_OFFSET;
	u32 fDATA;//有数据标志
	u32 NUM_READFILE;//读文件总次数
	u16* p16time;
	u8* p8tx;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	EventFileHead_TypeDef* EventFileHead;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 NUM_RCSD;
	u32 LEN_RCSD;
	u8* pRCSD;
	u32 NUMmax_DATA;
	#if(USER/100)==15//安徽
	u8* p8s;
	u8* p8SOUR;
	u16* p16s;
	u8* p8Data;
	u32 j;
	u32 len;
	u32 Len;
	u8* p8t;
	DL698_SETLIB_TypeDef *pLib;
	#endif

#define TEST_REPORT_PORTn_RS232     0//0=否,1=是;测试用上报口为RS232
#if TEST_REPORT_PORTn_RS232==0
	if((PORTn!=GPRSPORT)&&(PORTn!=ENETPORT))
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Task!=0)
	{
		return;
	}
	if(UARTCtrl->LinkTask!=101)
	{//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
		return;
	}
#else
//---用RS232口测试上报---
	if(PORTn!=RS232PORT)
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Task!=0)
	{
		return;
	}
//---用RS232口测试上报 END---
#endif
	
	if((UARTCtrl->HostACK)&&(UARTCtrl->HostACK!=4))//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件
	{//有其他主动上报等待确认返回
		return;
	}
	i=Comm_Ram->DL698YMDHMS[6];
	if(UARTCtrl->REPORT_RTCS==i)//空闲时进入上报的RTC秒
	{
		return;
	}
	if((i&1)==0)
	{
		return;//当前时钟奇数秒,奇数报事件,偶数报定时任务
	}
	UARTCtrl->REPORT_RTCS=i;//空闲时进入上报的RTC秒
	i=MRR(ADDR_4300_8+1,1);//允许主动上报
	if(i==0)
	{//不允许主动上报
		UARTCtrl->REPORT_NUMmax=0;//若主站没应答最大还需重发上报次数,每上报1次减1
		//不需主动上报的文件数据上报计数处理
		/*
		Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化(1=管理数据保存,2=不需主动上报处理)
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
		{
			if(filename==recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			}
			if(file->BLOCK_ENTRY)
			{
				
			}
			file++;
		}
		*/
		return;
	}
	switch(PORTn)
	{
		case GPRSPORT:
			p16time=(u16*)&Terminal_Ram->GPRS_WaitHostACK_S_Timer;//29 GPRS主动上报等待主站确认秒定时器
			break;
		case ENETPORT:
			p16time=(u16*)&Terminal_Ram->ENET_WaitHostACK_S_Timer;//30 ENET主动上报等待主站确认秒定时器
			break;
		default:
		case RS232PORT:
			p16time=(u16*)&Terminal_Ram->RS232_WaitHostACK_S_Timer;//31 RS232主动上报等待主站确认秒定时器
			break;
	}
	if(p16time[0]!=0)
	{//上报响应超时时间没到
		return;
	}
	i=UARTCtrl->REPORT_NUMmax;//若主站没应答最大还需重发上报次数,每上报1次减1
	if(i)
	{
		i--;
		UARTCtrl->REPORT_NUMmax=i;//若主站没应答最大还需重发上报次数,每上报1次减1
		p16time[0]=UARTCtrl->REPORT_OVER_SECOND;//上报响应超时时间寄存
		//发送帧缓存在DataBuff+LEN_UARTnTx,用以重发
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
		i=Get_LEN_UARTnTx(PORTn);
		p8+=i;
		LEN_Tx=p8[1]|(p8[2]<<8);
		LEN_Tx+=2;
		if(LEN_Tx<=i)
		{
			MR((u32)p8tx,(u32)p8,LEN_Tx);
			UARTCtrl->TxByte=LEN_Tx;
			UARTCtrl->Task=2;//2=等待单帧数据发送
		}
		return;
	}
	ms=(ms_Type *)Get_ms();
	EVENT_REPORT_complete(PORTn,0);//主动上报完成后处理(事件),入口:ACK=0没确认,1=确认
	
//心跳优先
	if(PORTn==GPRSPORT)
	{
		if(Terminal_Ram->GPRSHeartBeat_S_Timer==0)//GPRS心跳秒定时器
		{
			return;
		}
	}
	if(PORTn==ENETPORT)
	{
		if(Terminal_Ram->ENETHeartBeat_S_Timer==0)//以太网心跳秒定时器
		{
			return;
		}
	}
//
	fDATA=0;//有数据标志;0=没,1=有,2=表示发送缓冲不够
	NUM_READFILE=0;//读文件总次数
	UARTCtrl->REPORT_NUM_File=0;//1帧主动上报数据来自不同文件的个数
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(UARTCtrl->REPORT_NUM_File>=REPORT_NUMFILEmax)//上报数据的不同文件最大数
		{
			break;
		}
		if(fDATA==2)//有数据标志;0=没,1=有,2=表示发送缓冲不够
		{
			break;
		}
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			fileid=file->FILEID;
			if((fileid&0xff)==2)
			{//文件分类=2，事件记录
				x=file->NUM_DATA;
			#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
				if(PORTn==GPRSPORT)
				{
					n=file->NUM_GPRSreport[0];
				}
				else
				{
					n=file->NUM_ENETreport[0];
				}
			#else
				n=file->NUM_GPRSorENETreport[0];
			#endif
				if(x>n)
				{//有事件需上报
					i=x*sizeof(EventFileHead_TypeDef);
					if(i<=LEN_128KDATABUFF)
					{
						if(NUM_READFILE>127)//读文件总次数
						{//读文件总次数，限制每次搜索时间
							break;
						}
						ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,i);
						NUM_READFILE++;//读文件总次数
						EventFileHead=(EventFileHead_TypeDef*)(ADDR_128KDATABUFF);
						FILE_OFFSET=0;//计算数据地址
						for(i=0;i<n;i++)
						{
							FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
							EventFileHead++;
						}
						i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
						if(i>=REPORT_NUMFILEmax)//上报数据的不同文件最大数
						{
							break;
						}
						UARTCtrl->REPORT_filename[i]=filename;//1帧主动上报数据不同文件名列表
						UARTCtrl->REPORT_NUM_Data[i]=0;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
						i++;
						UARTCtrl->REPORT_NUM_File=i;//1帧主动上报数据来自不同文件的个数
						while(1)
						{
							i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
							if(i)
							{
								i--;
							}
							n=UARTCtrl->REPORT_NUM_Data[i];
						#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
							if(PORTn==GPRSPORT)
							{
								n+=file->NUM_GPRSreport[0];
							}
							else
							{
								n+=file->NUM_ENETreport[0];
							}
						#else
							n+=file->NUM_GPRSorENETreport[0];
						#endif
							if(file->NUM_DATA<=n)
							{//没数据需上报
								break;
							}
							if(EventFileHead->REPORT&1)//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
							{//需上报
								if(fDATA==0)//有数据标志;0=没,1=有,2=表示发送缓冲不够
								{
									p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
//APDU:88 02 01 
//01  SEQUENCE OF OAD，个数=?
//31 04 02 00  OAD
//06 OAD个数
//00 20 22 02 00 00 20 1e 02 00 00 20 20 02 00 00 20 24 02 00 00 33 00 02 00 00 f2 03 42 01  OAD列表
//01 Get-Result∷=CHOICE, [1]   DATA
//01 记录数m						
//06 00 00 00 31 1c 07 e1 03 10 0c 18 00 1c 07 e1 03 10 0c 18 00 00 01 02 02 02 51 45 00 00 00 16 00 02 02 51 45 10 00 00 16 00 02 02 11 00 11 00 00 00
									p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
									p8tx[0]=0x88;//上报通知 REPORT-Notification
									p8tx[1]=0x02;//上报若干个对象属性         [1] ReportNotificationList
															//上报若干个记录型对象属性   [2] ReportNotificationRecordList
									i=UARTCtrl->REPORT_PIID;
									i++;
									i&=0x3f;
									UARTCtrl->REPORT_PIID=i;
									p8tx[2]=i;//PIID
									p8tx[3]=0;//SEQUENCE OF OAD，个数=?
									LEN_TxAPDU=4;
									LENmax_TxSPACE-=LEN_TxAPDU+2;//保留2byte(1byte时间标签,1byte上报信息FollowReport OPTIONAL）
								}
								//事件OAD
								if(LENmax_TxSPACE<5)
								{
									if(fDATA)//有数据标志;0=没,1=有,2=表示发送缓冲不够
									{
										fDATA=2;//有数据标志;0=没,1=有,2=表示发送缓冲不够
										break;
									}
									//单个数据大于缓冲长度丢弃本数据上报
									i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
									if(i)
									{
										i--;
									}
									UARTCtrl->REPORT_NUM_Data[i]++;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
									FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
									EventFileHead++;
									continue;
								}
								OI=file->FILEID;
								NUMmax_DATA=DEPTH_EventFile(OI>>16);//事件文件存储深度;返回:存储深度
								OI>>=8;
								p8tx[LEN_TxAPDU+0]=OI>>16;
								p8tx[LEN_TxAPDU+1]=OI>>8;
								p8tx[LEN_TxAPDU+2]=OI;
								p8tx[LEN_TxAPDU+3]=0x00;
								p8tx[LEN_TxAPDU+4]=0;//OAD个数=?
								LEN_Tx=5;
								LENmax_TxSPACE-=5;
								//OAD列表
								i=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
								ms->msfile_read(filename,((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+FILE_OFFSET,(u8*)ADDR_128KDATABUFF+((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef)),i);
								i=EventFileHead->LEN_event[0]+(EventFileHead->LEN_event[1]<<8);
								p8=(u8*)ADDR_128KDATABUFF+((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+i;
								n=p8[1];
								p8tx[LEN_TxAPDU+4]=n;//OAD个数=?
								n*=5;
								if(LENmax_TxSPACE<(n+2))
								{
									if(fDATA)//有数据标志;0=没,1=有,2=表示发送缓冲不够
									{
										fDATA=2;//有数据标志;0=没,1=有,2=表示发送缓冲不够
										break;
									}
									//单个数据大于缓冲长度丢弃本数据上报
									i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
									if(i)
									{
										i--;
									}
									UARTCtrl->REPORT_NUM_Data[i]++;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
									FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
									EventFileHead++;
									continue;
								}
								MR((u32)p8tx+LEN_TxAPDU+LEN_Tx,(u32)p8+2,n);
								p8tx[LEN_TxAPDU+LEN_Tx+n]=1;//Get-Result∷=CHOICE, [1]   DATA
								p8tx[LEN_TxAPDU+LEN_Tx+n+1]=1;//记录数m
								LEN_Tx+=n+2;
								LENmax_TxSPACE-=n+2;
								
								i=EventFileHead->LEN_event[0]+(EventFileHead->LEN_event[1]<<8);
								i-=2;
								if(LENmax_TxSPACE<i)
								{
									if(fDATA)//有数据标志;0=没,1=有,2=表示发送缓冲不够
									{
										fDATA=2;//有数据标志;0=没,1=有,2=表示发送缓冲不够
										break;
									}
									//数据错误放弃本数据,单个数据大于缓冲长度丢弃本数据上报
									i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
									if(i)
									{
										i--;
									}
									UARTCtrl->REPORT_NUM_Data[i]++;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
									FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
									EventFileHead++;
									continue;
								}
								MR((u32)p8tx+LEN_TxAPDU+LEN_Tx,ADDR_128KDATABUFF+((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+2,i);
								LEN_Tx+=i;
								LENmax_TxSPACE-=i;
								p8tx[3]++;//SEQUENCE OF OAD，个数=?
								LEN_TxAPDU+=LEN_Tx;
								#if(USER/100)==15//安徽
								if(OI==0x00310602)
								{
									p8t=Get_Element(p8tx+LEN_TxAPDU-i,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
									if(p8t==0)
										return;
								}
								if(OI==0x00310602&&p8t[1]==0)//停电事件上报数据处理
								{
									LEN_Tx=0;
									Len=0;
									for(i=0;i<NUMmax_6012_2;i++)
									{
										p8s=(u8*)ADDR_6012_2_SDRAM+(i*LENper_6012_2);//任务配置表
										if(p8s[0]==DataType_structure)
										{
											if(p8s[9]==1)
											{//普通采集方案
												pLib=Get_DL698_SET_pLib(0x31060B00);//得到DATA_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
												if(pLib==0)
													break;
												MR(ADDR_DATABUFF,pLib->Addr,4);
												p8t=(u8*)ADDR_DATABUFF;
												if(p8s[11]==p8t[3])
												{//方案编号
													p8SOUR=(u8*)ADDR_6000_SDRAM;
													for(i=0;i<NUMmax_6000;i++)
													{
														if(p8SOUR[0]==DataType_structure)
														{
															p16s=(u16*)(p8SOUR+10);//取采集档案配置序号的通信地址
															len=Get_LastRecordFileDataHasID(54,p16s);
															if(len!=0)//在数据记录里面有该TSA的数据
															{
																LEN_RCSD=0;
																if(Len==0)
																{
																	p8tx[LEN_TxAPDU+0]=0x60;
																	p8tx[LEN_TxAPDU+1]=0x12;
																	p8tx[LEN_TxAPDU+2]=0x03;
																	p8tx[LEN_TxAPDU+3]=0x00;
																	
																	p8Data=(u8*)ADDR_128KDATABUFF;
																	NUM_RCSD = p8Data[22];
																	p8tx[LEN_TxAPDU+4]=NUM_RCSD+1;
																	j=0;
																	for(n=0;n<NUM_RCSD;n++)
																	{
																		x=5;
																		MR((u32)p8tx+LEN_TxAPDU+5+LEN_RCSD,(u32)p8Data+j+24,x);
																		LEN_RCSD+=x;
																		j+=x+1;
																	}
																	p8tx[LEN_TxAPDU+5+LEN_RCSD]=0;//目标服务器地址OAD
																	p8tx[LEN_TxAPDU+6+LEN_RCSD]=0x20;
																	p8tx[LEN_TxAPDU+7+LEN_RCSD]=0x2A;
																	p8tx[LEN_TxAPDU+8+LEN_RCSD]=0x02;
																	p8tx[LEN_TxAPDU+9+LEN_RCSD]=0x00;
																	LEN_Tx+=10+LEN_RCSD;
																	LEN_TxAPDU+=10+LEN_RCSD;
																	LENmax_TxSPACE-=10+LEN_RCSD;
																	p8tx[LEN_TxAPDU]=1;
																	p8tx[LEN_TxAPDU+1]=0;
																}
																len-=23+j;
																p8tx[LEN_TxAPDU+1]++;									
																MR((u32)p8tx+LEN_TxAPDU+2+Len,(u32)p8Data+23+j,len);														
																p8tx[LEN_TxAPDU+2+len+Len]=0x55;//目标服务器地址数据
																p8tx[LEN_TxAPDU+3+len+Len]=0x07;
																p8tx[LEN_TxAPDU+4+len+Len]=0x05;
																MR((u32)p8tx+LEN_TxAPDU+5+len+Len,(u32)p16s,6);
																Len+=9+len;
															}
														}
														p8SOUR+=LENper_6000;
													}
												}
											}
										}
									}
									if(Len>0)
									{
										p8tx[3]++;
										LEN_Tx+=Len+2;
										LENmax_TxSPACE-=Len+2;
										LEN_TxAPDU+=Len+2;
									}
								}
								#endif
								fDATA=1;//有数据标志;0=没,1=有,2=表示发送缓冲不够
							}	
							i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
							if(i)
							{
								i--;
							}
							UARTCtrl->REPORT_NUM_Data[i]++;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
							FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
							EventFileHead++;
							if(fDATA)
							{
								break;//文件有事件即上报,不同数据记录帧格式不同!
							}
						}
					}
				}
			}
	//电能表全事件采集上报
			if((fileid>>28)==2)
			{//事件采集方案
				x=file->NUM_DATA;
			#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
				if(PORTn==GPRSPORT)
				{
					n=file->NUM_GPRSreport[0];
				}
				else
				{
					n=file->NUM_ENETreport[0];
				}
			#else
				n=file->NUM_GPRSorENETreport[0];
			#endif
				if(x>n)
				{//有事件需上报
					i=x*sizeof(RecordFileHead_TypeDef);
					if(i<=LEN_128KDATABUFF)
					{
						if(NUM_READFILE>127)//读文件总次数
						{//读文件总次数，限制每次运行时间
							break;
						}
						NUM_READFILE++;//读文件总次数
						i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
						if(i>=REPORT_NUMFILEmax)//上报数据的不同文件最大数
						{
							break;
						}
						UARTCtrl->REPORT_filename[i]=filename;//1帧主动上报数据不同文件名列表
						UARTCtrl->REPORT_NUM_Data[i]=0;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
						i++;
						UARTCtrl->REPORT_NUM_File=i;//1帧主动上报数据来自不同文件的个数

						//读记录头
						ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,x*sizeof(RecordFileHead_TypeDef));
						RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
						FILE_OFFSET=0;//计算数据地址
						for(i=0;i<n;i++)
						{
							FILE_OFFSET+=RecordFileHead->LEN_DATA;
							RecordFileHead++;
						}
						//取记录数据
						i=RecordFileHead->LEN_DATA;
						ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+FILE_OFFSET,(u8*)ADDR_128KDATABUFF+(64*1024),i);
	//TxAPDU：
	//88 上报通知
	//02 上报若干个记录型对象属性
	//02 PIID
	//01 SEQUENCE OF A-ResultNormal
	//60 12 03 00  OAD
	//01 RCSD，SEQUENCE OF个数=1
	//01 50 04 02 00 02 00 10 02 00 00 20 02 00 //RCSD
	//01 记录数据
	//02 M条记录，M=2
	//01 02 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 
	//01 02 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 
	//00 00 
						p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
						p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
						p8tx[0]=0x88;//上报通知 REPORT-Notification
						p8tx[1]=0x02;//上报若干个对象属性[1] ReportNotificationList,上报若干个记录型对象属性   [2] ReportNotificationRecordList
						i=UARTCtrl->REPORT_PIID;
						i++;
						i&=0x3f;
						UARTCtrl->REPORT_PIID=i;
						p8tx[2]=i;//PIID
						p8tx[3]=1;//SEQUENCE OF A-ResultNormal
						p8tx[4]=0x60;
						p8tx[5]=0x12;
						p8tx[6]=0x03;
						p8tx[7]=0x00;
						
						LEN_RCSD=0;//无类型的RCSD长度
						p8tx[9+LEN_RCSD]=0;//OAD
						p8tx[10+LEN_RCSD]=0x60;//0x60400200://采集启动时标
						p8tx[11+LEN_RCSD]=0x40;
						p8tx[12+LEN_RCSD]=0x02;
						p8tx[13+LEN_RCSD]=0x00;
						LEN_RCSD+=5;
						p8tx[9+LEN_RCSD]=0;//OAD
						p8tx[10+LEN_RCSD]=0x60;//0x60410200://采集成功时标
						p8tx[11+LEN_RCSD]=0x41;
						p8tx[12+LEN_RCSD]=0x02;
						p8tx[13+LEN_RCSD]=0x00;
						LEN_RCSD+=5;
						p8tx[9+LEN_RCSD]=0;//OAD
						p8tx[10+LEN_RCSD]=0x60;//0x60420200://采集存储时标
						p8tx[11+LEN_RCSD]=0x42;
						p8tx[12+LEN_RCSD]=0x02;
						p8tx[13+LEN_RCSD]=0x00;
						LEN_RCSD+=5;
						p8tx[9+LEN_RCSD]=0;//OAD
						p8tx[10+LEN_RCSD]=0x20;//0x202A0200://目标服务器地址
						p8tx[11+LEN_RCSD]=0x2A;
						p8tx[12+LEN_RCSD]=0x02;
						p8tx[13+LEN_RCSD]=0x00;
						LEN_RCSD+=5;

						pRCSD=(u8*)ADDR_128KDATABUFF+(64*1024)+21;
						NUM_RCSD=pRCSD[1];//RCSD，SEQUENCE OF个数
						p8tx[8]=NUM_RCSD+(LEN_RCSD/5);//+目标服务器地址等
						//文件中有类型RCSD转为无类型的RCSD
						i=2;//文件中有类型的RCSD长度
						for(n=0;n<NUM_RCSD;n++)
						{
							if(pRCSD[i+1]==0)
							{//OAD
								x=5;
								MR((u32)p8tx+9+LEN_RCSD,(u32)pRCSD+i+1,x);
								LEN_RCSD+=x;
								i+=x+1;
							}
							else
							{//ROAD
								x=pRCSD[i+6];
								x*=4;
								x+=6;
								MR((u32)p8tx+9+LEN_RCSD,(u32)pRCSD+i+1,x);
								LEN_RCSD+=x;
								i+=x+1;
							}
						}
						p8tx[9+LEN_RCSD]=1;//记录数据
						p8tx[10+LEN_RCSD]=0;//M条记录
						
						LEN_TxAPDU=11+LEN_RCSD;
						LENmax_TxSPACE-=LEN_TxAPDU+2;//保留2byte(1byte时间标签,1byte上报信息FollowReport OPTIONAL）	
					#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
						if(PORTn==GPRSPORT)
						{
							n=file->NUM_GPRSreport[0];
						}
						else
						{
							n=file->NUM_ENETreport[0];
						}
					#else
						n=file->NUM_GPRSorENETreport[0];
					#endif
						p8=(u8*)(ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)));//数据的电能表地址
						i=Get_RecordFileData_RCSD(p8tx+8,p8,(u8*)ADDR_128KDATABUFF+(64*1024),(u8*)ADDR_DATABUFF,LEN_DATABUFF,0);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
						if(i)
						{
							if(i<=LENmax_TxSPACE)
							{
								MR((u32)p8tx+LEN_TxAPDU,ADDR_DATABUFF,i);
								LEN_TxAPDU+=i;
								LENmax_TxSPACE-=i;
								p8tx[10+LEN_RCSD]++;//M条记录
								fDATA=1;//有数据标志;0=没,1=有,2=表示发送缓冲不够
							}
						}
						i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
						if(i)
						{
							i--;
						}
						UARTCtrl->REPORT_NUM_Data[i]++;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
						if(fDATA)
						{
							break;//文件有事件即上报,不同数据记录帧格式不同!
						}

					}
				}
			}
	//DL645电能表主动上报
			if((fileid&0xff)==5)
			{//文件分类=5
				x=file->NUM_DATA;
			#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
				if(PORTn==GPRSPORT)
				{
					n=file->NUM_GPRSreport[0];
				}
				else
				{
					n=file->NUM_ENETreport[0];
				}
			#else
				n=file->NUM_GPRSorENETreport[0];
			#endif
				if(x>n)
				{//有事件需上报
					i=x*sizeof(RecordFileHead_TypeDef);
					if(i<=LEN_128KDATABUFF)
					{
						if(NUM_READFILE>127)//读文件总次数
						{//读文件总次数，限制每次运行时间
							break;
						}
						NUM_READFILE++;//读文件总次数
						i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
						if(i>=REPORT_NUMFILEmax)//上报数据的不同文件最大数
						{
							break;
						}
						UARTCtrl->REPORT_filename[i]=filename;//1帧主动上报数据不同文件名列表
						UARTCtrl->REPORT_NUM_Data[i]=0;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
						i++;
						UARTCtrl->REPORT_NUM_File=i;//1帧主动上报数据来自不同文件的个数

						//读记录头
						ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,x*sizeof(RecordFileHead_TypeDef));
						RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
						FILE_OFFSET=0;//计算数据地址
						for(i=0;i<n;i++)
						{
							FILE_OFFSET+=RecordFileHead->LEN_DATA;
							RecordFileHead++;
						}
						//取记录数据
						i=RecordFileHead->LEN_DATA;
						ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+FILE_OFFSET,(u8*)ADDR_128KDATABUFF+(64*1024),i);
	//ReportNotificationTransData∷=SEQUENCE 
	//{ 
	//	服务序号-优先级-ACD PIID-ACD， 
	//	数据来源端口号 OAD， 
	//	透明数据 SEQUENCE OF octet-string 
	//}
	//TxAPDU：
	//88 上报通知
	//03 上报透明数据 [3] ReportNotificationTransData
	//02 服务序号-优先级-ACD PIID-ACD
	//F2 01 02 01  数据来源端口号 OAD
	//01 透明数据 SEQUENCE OF octet-string 
						p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
						p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
						p8tx[0]=0x88;//上报通知 REPORT-Notification
						p8tx[1]=0x03;//上报透明数据 [3] ReportNotificationTransData
						i=UARTCtrl->REPORT_PIID;
						i++;
						i&=0x3f;
						UARTCtrl->REPORT_PIID=i;
						p8tx[2]=i;//PIID
						LEN_TxAPDU=3;
						LENmax_TxSPACE-=LEN_TxAPDU+2;//保留2byte(1byte时间标签,1byte上报信息FollowReport OPTIONAL）	
						
						i=RecordFileHead->LEN_DATA;
						if(i<=LENmax_TxSPACE)
						{
							MR((u32)p8tx+LEN_TxAPDU,ADDR_128KDATABUFF+(64*1024),i);
							LEN_TxAPDU+=i;
							LENmax_TxSPACE-=i;
							fDATA=1;//有数据标志;0=没,1=有,2=表示发送缓冲不够
						}

						i=UARTCtrl->REPORT_NUM_File;//1帧主动上报数据来自不同文件的个数
						if(i)
						{
							i--;
						}
						UARTCtrl->REPORT_NUM_Data[i]++;//1帧主动上报数据不同文件名中已处理数据个数列表,包括没任何列选中的个数
						if(fDATA)
						{
							break;//文件有事件即上报,不同数据记录帧格式不同!
						}
					}
				}
			}
			//
			
		}
		file++;
	}
	if(fDATA==0)//有数据标志
	{//没上报数据
		if(UARTCtrl->REPORT_NUM_File)//1帧主动上报数据来自不同文件的个数
		{
			//文件中已处理过的数据个数
			UARTCtrl->HostACK=4;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
			EVENT_REPORT_complete(PORTn,0);//主动上报完成后处理(事件),入口:ACK=0没确认,1=确认
		}
		return;
	}		
	p8tx[LEN_TxAPDU+0]=0;//时间标签
	p8tx[LEN_TxAPDU+1]=0;//上报信息FollowReport OPTIONAL
	LEN_TxAPDU+=2;
			
	i=MRR(ADDR_F101_2+1,1);//读取安全模式参数
	if(i==0)
	{//0—不启用安全模式参数、默认安全模式参数
		p8tx-=TxAPDUOffset;//TxAPDU存放开始地址
		p8tx[0]=0x68;
		//2byte长度域L
		p8tx[3]=0x83;//控制域C
		p8tx[4]=5;//TSA
		MR((u32)p8tx+5,ADDR_4001+2,6);
		Data_Inverse((u32)p8tx+5,6);//数据倒序(高低字节调换),入口Le>=2
		p8tx[11]=0;//客户机地址CA
		//2byte帧头校验HCS
		MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
		//2byte校验
		LEN_Tx=14+LEN_TxAPDU+2;
		p8tx[LEN_Tx]=0x16;
		LEN_Tx++;
		UARTCtrl->TxByte=LEN_Tx;
		LEN_Tx-=2;
		p8tx[1]=LEN_Tx;
		p8tx[2]=LEN_Tx>>8;
	}
	else
	{//1—启用安全模式参数、默认安全模式参数
		p8tx-=TxAPDUOffset;//TxAPDU存放开始地址
		p8tx[0]=0x68;
		//2byte长度域L
		p8tx[3]=0x83;//控制域C
		p8tx[4]=5;//TSA
		MR((u32)p8tx+5,ADDR_4001+2,6);
		Data_Inverse((u32)p8tx+5,6);//数据倒序(高低字节调换),入口Le>=2
		p8tx[11]=0;//客户机地址CA
		//2byte帧头校验HCS
		p8tx[14]=0x10;//0x10,0x00,0x13,//安全传输
		p8tx[15]=0x00;//安全类别
		//明文长度
		if(LEN_TxAPDU<127)
		{
			p8tx[16]=LEN_TxAPDU;
			LEN_Tx=17;
		}
		else
		{
			p8tx[16]=0x82;
			p8tx[17]=LEN_TxAPDU>>8;
			p8tx[18]=LEN_TxAPDU;
			LEN_Tx=19;
		}
		i=TESAM_CMD_TxRx(0x80140103,LEN_TxAPDU,(u32)p8tx+TxAPDUOffset);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
		if(i)
		{//ESAM验证失败
			UARTCtrl->REPORT_NUM_File=0;//1帧主动上报数据来自不同文件的个数
			return;
		}
		MR((u32)p8tx+LEN_Tx,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
		LEN_Tx+=LEN_TxAPDU;
		//02 0C 82 07 EF 22 99 AD 50 09 00 00 00 0F 04 FB 43 41 D0
		p8tx[LEN_Tx]=2;//随机数+数据MAC  [2]  RN_MAC
		p8tx[LEN_Tx+1]=12;
		MR((u32)p8tx+LEN_Tx+2,ADDR_UART7Rx+5,12);
		p8tx[LEN_Tx+2+12]=4;
		MR((u32)p8tx+LEN_Tx+15,ADDR_UART7Rx+5+12,4);
		LEN_Tx+=19;
		LEN_Tx+=3;//2byte校验,1byte 0x16
		UARTCtrl->TxByte=LEN_Tx;
		LEN_Tx-=2;
		p8tx[1]=LEN_Tx;
		p8tx[2]=LEN_Tx>>8;
	}
	
	Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	UARTCtrl->Task=2;//2=等待单帧数据发送
	//发送帧缓存在DataBuff+LEN_UARTnTx,用以重发
	i=Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
	i+=Get_LEN_UARTnTx(PORTn);
	MW((u32)p8tx,i,LEN_Tx+2);
	//上报响应超时时间  TI
	p16time[0]=WaitHostACKMinTimer;//默认最小值
	UARTCtrl->REPORT_OVER_SECOND=WaitHostACKMinTimer;//上报响应超时时间寄存
	UARTCtrl->REPORT_NUMmax=0;//若主站没应答最大还需重发上报次数,每上报1次减1
	UARTCtrl->HostACK=4;//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
	if(PORTn==GPRSPORT)
	{//公网通信模块1
		p8=Get_Element((u8 *)ADDR_4500_2,11,0,LENmax_4500_2);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
		
	}
	else
	{//以太网通信模块1,其他口
		p8=Get_Element((u8 *)ADDR_4510_2,7,0,LENmax_4510_2);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
	}
	if(p8)
	{
		x=p8[1];//bit7~bit2：超时时间（秒），bit1~bit0：重发次数
		i=x>>2;
		if(i>WaitHostACKMaxTimer)
		{
			i=WaitHostACKMaxTimer;
		}
		if(i<WaitHostACKMinTimer)
		{
			i=WaitHostACKMinTimer;
		}
		p16time[0]=i;
		UARTCtrl->REPORT_OVER_SECOND=i;//上报响应超时时间寄存
		//最大上报次数
		i=x&0x3;
		if(i>=1)
		{
			i--;
			UARTCtrl->REPORT_NUMmax=i;//若主站没应答最大还需重发上报次数,每上报1次减1
			if(i==0)
			{
				p16time[0]=0;
			}
		}
	}
	
//u32 Event_3106(u8* pOccurSour,u32 Occur);//终端停/上电事件记录;入口:f=属性标志,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
//Event_3106(0,1);//终端停/上电事件记录
	
	
}


//REPORT-Response∷=CHOICE
//{
//上报若干个对象属性的响应      	 [1] ReportResponseList，
//上报若干个记录型对象属性的响应	 [2] ReportResponseRecordList
//}
void REPORT_Response(u32 PORTn)//上报应答
{
	u32 LEN_SA;
	u8* p8rx;
	UARTCtrl_TypeDef *UARTCtrl;
	u16 *p16time;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	switch(p8rx[LEN_SA+8+1])
	{
		case 1://上报若干个对象属性的响应      	 [1] ReportResponseList

			break;
		case 2://上报若干个记录型对象属性的响应	 [2] ReportResponseRecordList
			
			break;
	}
	switch(PORTn)
	{
		case GPRSPORT:
			p16time=(u16*)&Terminal_Ram->GPRS_WaitHostACK_S_Timer;//29 GPRS主动上报等待主站确认秒定时器
			break;
		case ENETPORT:
			p16time=(u16*)&Terminal_Ram->ENET_WaitHostACK_S_Timer;//30 ENET主动上报等待主站确认秒定时器
			break;
		default:
		case RS232PORT:
			p16time=(u16*)&Terminal_Ram->RS232_WaitHostACK_S_Timer;//31 RS232主动上报等待主站确认秒定时器
			break;
	}
	if(UARTCtrl->HostACK==4)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
	{
		p16time[0]=0;
		UARTCtrl->REPORT_NUMmax=0;//若主站没应答最大还需重发上报次数,每上报1次减1
		UARTCtrl->REPORT_RTCS=0xff;//空闲时进入上报的RTC秒
		EVENT_REPORT_complete(PORTn,1);//主动上报完成后处理(事件),入口:ACK=0没确认,1=确认
	}
	if(UARTCtrl->HostACK==5)//主动上报需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务,6=电能表事件,7=终端请求主站时钟
	{
		p16time[0]=0;
		UARTCtrl->REPORT_NUMmax=0;//若主站没应答最大还需重发上报次数,每上报1次减1
		UARTCtrl->REPORT_RTCS=0xff;//空闲时进入上报的RTC秒
		REPORT_complete(PORTn);//主动上报完成后处理(定时任务)
	}

}








