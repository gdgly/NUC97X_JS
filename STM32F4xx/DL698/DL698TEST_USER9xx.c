

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698TEST.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/RM_Record.h"
#include "../MS/MS.h"
#include "../Terminal698/Terminal_Init.h"




#if RUN_DL698TEST_USER==1//0无效,1=运行;用户版测试
#if (USER/100)==9//河南版

u32 CALC_FLASH_NUM_File(void)//统计FLASH总文件数
{
	u32 i;
	u32 filename;
	msFILE_TypeDef *file;
	
	i=0;
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			i++;
		}
		file++;
	}
	return i;
}

u32 CALC_SDRAM_NUM_File(void)//统计SDRAM总文件数
{
	u32 i;
	u32 filename;
	msFILE_TypeDef *file;
	
	i=0;
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=recordFILENAMEmax_FLASH;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			i++;
		}
		file++;
	}
	return i;
}

u32 CALC_NUM_File(void)//统计总文件数
{
	u32 i;
	u32 filename;
	msFILE_TypeDef *file;
	
	i=0;
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			i++;
		}
		file++;
	}
	return i;
}


const u8 RM_RecordFile_CODE[]=
{
//UART抄表数据缓冲定义
//#define LEN_RM_DI_CSD     512
//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
	1,0,0,0,0,0,
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
	0,0,
//8 7byte 采集启动时标  date_time_s(无数据类型)
	0x07,0xE0,4,18,0,0,0,
//15 7byte 采集成功时标  date_time_s(无数据类型)
	0x07,0xE0,4,18,0,0,0,
//22 7byte 采集存储时标  date_time_s(无数据类型)
	0x07,0xE0,4,18,0,0,0,
//29  采集的CSD  array CSD(有数据类型)
	DataType_array,//array
	1,//数
	DataType_CSD,
	1,//ROAD
	0x50,0x04,0x02,0x00,//日冻结
	2,//关联对象属性描述符  SEQUENCE OF个数=2
	0x00,0x10,0x02,0x00,//正向有功总及费率
	0x00,0x20,0x02,0x00,//反向有功总及费率
	
//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型)
	0x1,0x5,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,
	0x1,0x5,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,
};

u32 DL698TEST_USER9xx_Init_1(void)//河南版测试_抄表记录文件
{
	u32 i;
	u8* p8rx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 PORTn;
	ms_Type *ms;
	u32 NUM_File;
	
	ms=Get_ms();
	ms->msfile_format();
	PORTn=RS485_1PORT;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	MR((u32)p8rx,(u32)RM_RecordFile_CODE,sizeof(RM_RecordFile_CODE));
	i=sizeof(RM_RecordFile_CODE);
	i-=8;
	MWR(i,(u32)p8rx+6,2);//长度
	
	NUM_File=CALC_NUM_File();//统计总文件数
	
//实时采集任务
	//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//b27-b24方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//b23-b20普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集
//b19-b16普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标,7=相对上月月末23点59分
	UARTCtrl->TaskID=0x01011515;
	MWR(0x00000001,(u32)p8rx,6);//电能表地址
	Data_Inverse((u32)p8rx,6);//数据倒序(高低字节调换),入口Le>=2
	MWR(0x20180310010203,(u32)p8rx+22,7);//采集存储时标
	Data_Inverse((u32)p8rx+22,7);//数据倒序(高低字节调换),入口Le>=2
	RM_RecordFile(PORTn,p8rx);//抄表记录
	NUM_File=CALC_NUM_File();//统计总文件数
	//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	UARTCtrl->TaskID=0x01011515;
	MWR(0x00000002,(u32)p8rx,6);//电能表地址
	Data_Inverse((u32)p8rx,6);//数据倒序(高低字节调换),入口Le>=2
	MWR(0x20180310010000,(u32)p8rx+22,7);//采集存储时标
	Data_Inverse((u32)p8rx+22,7);//数据倒序(高低字节调换),入口Le>=2
	RM_RecordFile(PORTn,p8rx);//抄表记录
	NUM_File=CALC_NUM_File();//统计总文件数
//统计总文件数=2

//日采集任务
	//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//b27-b24方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//b23-b20普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集
//b19-b16普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标,7=相对上月月末23点59分
	UARTCtrl->TaskID=0x01231616;
	MWR(0x00000001,(u32)p8rx,6);//电能表地址
	Data_Inverse((u32)p8rx,6);//数据倒序(高低字节调换),入口Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//采集存储时标
	Data_Inverse((u32)p8rx+22,7);//数据倒序(高低字节调换),入口Le>=2
	RM_RecordFile(PORTn,p8rx);//抄表记录
	NUM_File=CALC_NUM_File();//统计总文件数
	//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	UARTCtrl->TaskID=0x01231616;
	MWR(0x00000002,(u32)p8rx,6);//电能表地址
	Data_Inverse((u32)p8rx,6);//数据倒序(高低字节调换),入口Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//采集存储时标
	Data_Inverse((u32)p8rx+22,7);//数据倒序(高低字节调换),入口Le>=2
	RM_RecordFile(PORTn,p8rx);//抄表记录
	NUM_File=CALC_NUM_File();//统计总文件数
//统计总文件数=3

//月采集任务
	//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//b27-b24方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//b23-b20普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集
//b19-b16普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标,7=相对上月月末23点59分,7=相对上月月末23点59分
	UARTCtrl->TaskID=0x01271717;
	MWR(0x00000001,(u32)p8rx,6);//电能表地址
	Data_Inverse((u32)p8rx,6);//数据倒序(高低字节调换),入口Le>=2
	MWR(0x20180310010000,(u32)p8rx+22,7);//采集存储时标
	Data_Inverse((u32)p8rx+22,7);//数据倒序(高低字节调换),入口Le>=2
	RM_RecordFile(PORTn,p8rx);//抄表记录
	NUM_File=CALC_NUM_File();//统计总文件数
//统计总文件数=4
	
//15分钟曲线采集任务
	//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	UARTCtrl->TaskID=0x01361818;
	MWR(0x00000001,(u32)p8rx,6);//电能表地址
	Data_Inverse((u32)p8rx,6);//数据倒序(高低字节调换),入口Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//采集存储时标
	Data_Inverse((u32)p8rx+22,7);//数据倒序(高低字节调换),入口Le>=2
	RM_RecordFile(PORTn,p8rx);//抄表记录
	NUM_File=CALC_NUM_File();//统计总文件数
	UARTCtrl->TaskID=0x01361818;
	MWR(0x00000002,(u32)p8rx,6);//电能表地址
	Data_Inverse((u32)p8rx,6);//数据倒序(高低字节调换),入口Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//采集存储时标
	Data_Inverse((u32)p8rx+22,7);//数据倒序(高低字节调换),入口Le>=2
	RM_RecordFile(PORTn,p8rx);//抄表记录
	NUM_File=CALC_NUM_File();//统计总文件数
//统计总文件数=5
	
//事件采集任务	
	//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//b27-b24方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//b23-b20普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集
//b19-b16普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标,7=相对上月月末23点59分
	UARTCtrl->TaskID=0x02000101;
	MWR(0x00000001,(u32)p8rx,6);//电能表地址
	Data_Inverse((u32)p8rx,6);//数据倒序(高低字节调换),入口Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//采集存储时标
	Data_Inverse((u32)p8rx+22,7);//数据倒序(高低字节调换),入口Le>=2
	RM_RecordFile(PORTn,p8rx);//抄表记录
	NUM_File=CALC_NUM_File();//统计总文件数
//统计总文件数=6
	
	return NUM_File;
}


const u8 InitData_6000_2_TEST_1[]=//采集档案配置
{
//属性2（Acquisition document definition）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	4,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
	0,1,
//基本信息  Basic_object，
	
//扩展信息  Extended_object，
	
//附属信息  Annex_object
//}

//Basic_object∷=structure
	DataType_structure,
	10,
//{
//通信地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x01,
//波特率    enum
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
	DataType_enum,
	255,
//规约类型  enum
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//}，
	DataType_enum,
	2,
//端口      OAD，
	DataType_OAD,
	0xF2,0x08,0x02,0x01, //交流采样通信口号(虚拟)
//通信密码  octet-string，
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//费率个数  unsigned，
	DataType_unsigned,
	4,
//用户类型  unsigned，
	DataType_unsigned,
	21,//C类
//接线方式  enum
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}
	DataType_enum,
	3,
//额定电压    long-unsigned(换算-1，单位V),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//额定电流    long-unsigned(换算-1，单位A)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object∷=structure
	DataType_structure,
	4,
//{
//采集器地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//资产号      octet-string，
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned，
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object∷=array structure
	DataType_array,
	0,
//{
//对象属性描述  OAD，
//属性值        Data
//}
};
const u8 InitData_6000_2_TEST_2[]=//采集档案配置
{
//属性2（Acquisition document definition）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	4,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
	0,1,
//基本信息  Basic_object，
	
//扩展信息  Extended_object，
	
//附属信息  Annex_object
//}

//Basic_object∷=structure
	DataType_structure,
	10,
//{
//通信地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x02,
//波特率    enum
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
	DataType_enum,
	255,
//规约类型  enum
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//}，
	DataType_enum,
	2,
//端口      OAD，
	DataType_OAD,
	0xF2,0x08,0x02,0x01, //交流采样通信口号(虚拟)
//通信密码  octet-string，
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//费率个数  unsigned，
	DataType_unsigned,
	4,
//用户类型  unsigned，
	DataType_unsigned,
	51,//F类
//接线方式  enum
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}
	DataType_enum,
	3,
//额定电压    long-unsigned(换算-1，单位V),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//额定电流    long-unsigned(换算-1，单位A)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object∷=structure
	DataType_structure,
	4,
//{
//采集器地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//资产号      octet-string，
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned，
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object∷=array structure
	DataType_array,
	0,
//{
//对象属性描述  OAD，
//属性值        Data
//}
};

void DL698TEST_USER9xx_Init_2(void)//河南版测试_设置不同用户档案
{
//RSD选择10 MS=全部表(河南版测试不同用户类的数据招读组帧)
//68 34 00 43 05 01 00 00 00 00 00 10 1f ea 05 03 04 60 12 03 00 0a 01 01 05 00 40 01 02 00 00 60 40 02 00 00 60 41 02 00 00 60 42 02 00 00 00 10 02 00 00 52 d3 16

	ms_Type *ms;
	
	ms=Get_ms();
	ms->msfile_format();
	
	//初始化参数
	DL698_INIT(3);//初始化,入口Command:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化

	//清档案
	MC(0,ADDR_6000,LENper_6000*NUMmax_6000);
	//初始化档案
	MW((u32)InitData_6000_2_TEST_1,ADDR_6000+(1*LENper_6000),LENper_6000);
	MW((u32)InitData_6000_2_TEST_2,ADDR_6000+(2*LENper_6000),LENper_6000);
	
	FLASHtoSDRAM();//参数FLASHtoSDRAM
}



void DL698TEST_Init_USER(void)//用户版测试初台化
{
//	DL698TEST_USER9xx_Init_1();//河南版测试_抄表记录文件
	DL698TEST_USER9xx_Init_2();//河南版测试_不同用户类的数据招读
	
}

void DL698TEST_USER(void)//用户版测试
{
	u32 FLASH_NUM_File;
	u32 SDRAM_NUM_File;
	u32 NUM_File;
	
	FLASH_NUM_File=CALC_FLASH_NUM_File();//统计FLASH总文件数
	SDRAM_NUM_File=CALC_SDRAM_NUM_File();//统计SDRAM总文件数
	NUM_File=CALC_NUM_File();//统计总文件数
	if(NUM_File!=(FLASH_NUM_File+SDRAM_NUM_File))
	{
		NUM_File=NUM_File;
	}
	NUM_File=recordFILENAMEmax_RAM;
	if(NUM_File)
	{
		NUM_File=NUM_File;
	}
	
}

#endif
#endif













