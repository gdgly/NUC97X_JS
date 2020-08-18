
//集中抄抄表2类数据日月冻结原始数据(初始值为擦除值0xFF)

#ifndef AFN56_Source_H
#define AFN56_Source_H

#define	ADDR_AFN56_Source_Date_Start     ADDR_AFN56_Source_Start
#define  LEN_AFN56_Source_Date	(3*1024*1024)//3M

//事件存储区
#define ADDR_AFN56_Event_Date_Start			ADDR_AFN56_Source_Date_Start
#define LEN_AFN56_Event_Date	(512*1024)
#define PER_EVENTSAVE_SPACE 256
#define MAX_EVENTSAVE_NUM 1024
//4K索引 = 1024*4   (每4字节代表一个事件,D24-D31:0x55  需上报标志    D0-D15  事件序号0-1023    D16-D23:0或0xFF可写标志
//每次事件写入时,将下一事件可写标志清零
//256K = 256 * 1024
#define ADDR_AFN56_Event_Date_End				(ADDR_AFN56_Event_Date_Start+LEN_AFN56_Event_Date)

//索引区
#define ADDR_AFN56_Index_Start     ((ADDR_AFN56_Event_Date_End+0x1ffff)&0xfffe0000)  //起始地址128K对齐
#define  LEN_AFN56_Index	((2*1024+512)*1024)//3M

//具体任务索引表
#define  LEN_AFN56_DataSource	(90*1024*1024)//90M
//每片数据区应为256+32=288，  现场实际运行时，在一帧数据刚好在两个128k存储区时，发现有尾部部分数据没有写入的情况，特修改为256
#define  LEN_AFN56_Data_Per		288//每片数据区:32byte Info + 256byte Data  共计90*1024*1024 / 288片区域
//#define  LEN_AFN56_Data_Per		256//每片数据区:32byte Info + 224byte Data  共计90*1024*1024 / 256片区域
#if LEN_AFN56_Data_Per>=288
  #define  LEN_AFN56_Data_Length		288//每片数据区:32byte Info + 224byte Data  共计90*1024*1024 / 256片区域
#else
  #define  LEN_AFN56_Data_Length		LEN_AFN56_Data_Per//每片数据区:32byte Info + 224byte Data  共计90*1024*1024 / 256片区域
#endif
//#define MAX_AFN56DATA_SUN		327680// 90*1024*1024 / 288 = 327680 = 320K byte
#define MAX_AFN56DATA_SUN		(LEN_AFN56_DataSource / LEN_AFN56_Data_Per) // 90*1024*1024 / 288 = 327680 = 320K byte
typedef __packed struct
{
	u16 TaskID;//任务号，唯一标识1-65535
	u32 Index;//分配空间起始序号 4byte  1-327680
	u8  Depth;//存储深度				 1byte
	u8  Res[8];//备用
	u8  Check;//校验						 1byte
}FuJSubTaskHead_TypeDef;//福建具体任务数据信息结构
#define LEN_SubTaskHead_Per 16//每个具体任务信息结构长度为16byte

#define  ADDR_AFN56_DataTable_Start		((ADDR_AFN56_Index_Start+LEN_SubTaskHead_Per*65536+0x1ffff)&0xfffe0000)//最大具体任务数65536 任务号1-65536    //起始地址128k对齐
//#define  LEN_AFN56_DataTable	40960//327680 / 8   ;1bit = 288byte  + 2byte check
#define  LEN_AFN56_DataTable	(MAX_AFN56DATA_SUN / 8)//40960//327680 / 8   ;1bit = 288byte  + 2byte check
#define  LEN_AFN56_Table	(LEN_AFN56_DataTable+2)//(327680 / 8 )+2  ;1bit = 288byte  + 2byte check
#if (LEN_AFN56_Table > (128*1024))
 #error "GetIdleSpace(u32 TaskID,u32 Size)--MR(ADDR_128KDATABUFF,ADDR_AFN56_DataTable_Start,LEN_AFN56_Table)"
#endif
#define  ADDR_AFN56_DataTable_End		((ADDR_AFN56_DataTable_Start+LEN_AFN56_Table+0x1ffff)&0xfffe0000) //结束地址128K对齐

#if (ADDR_AFN56_DataTable_End-ADDR_AFN56_Index_Start)>LEN_AFN56_Index
	#error "(ADDR_AFN56_DataTable_End-ADDR_AFN56_Index_Start)>LEN_AFN56_Index"
#endif

#define  ADDR_AFN56_Index_End				((ADDR_AFN56_Index_Start+LEN_AFN56_Index+0x1ffff)&0xfffe0000) //结束地址128K对齐

//数据区
#define  ADDR_AFN56_DataSource_Start		((ADDR_AFN56_Index_End+0x1ffff)&0xfffe0000) //开始地址128K对齐
//#define  LEN_AFN56_DataSource	(90*1024*1024)//90M

typedef __packed struct
{
	u16 SubTaskID;//任务号，唯一标识1-65535
	u8  STime[6];//相对时标
	u8  RTime[6];//绝对时标
	u8  MeterAddr[6];//通信地址
	u8  Len;//数据长度
	u32 Round;//轮次
	u8  ReportFlag;//数据上报状态
	u8  ReadTimes;//抄读次数
	u8  res1;//RetryTimes;//重试次数--第N论重试
	u8  ReTimes;//当前周期抄读总次数
	u8	Res[2];//备用
	u8  Check;//校验						 1byte
}FuJSubTaskDataHead_TypeDef;//福建具体任务数据信息结构

#define LEN_FuJSubTaskDataHead	32



#define  ADDR_AFN56_DataSource_End			((ADDR_AFN56_DataSource_Start+LEN_AFN56_DataSource+0x1ffff)&0xfffe0000) //结束地址128K对齐


#define ADDR_AFN56_Source_End     ADDR_AFN56_DataSource_End
#if ((ADDR_AFN56_Source_End-ADDR_AFN56_Source_Start)>LEN_Terminal_EFLASH_AFN56)
	#error "((ADDR_AFN56_Source_End-ADDR_AFN56_Source_Start)>LEN_Terminal_EFLASH_AFN56)"
#endif



#endif
