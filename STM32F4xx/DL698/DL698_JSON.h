#ifndef DL698_JSON_H
#define DL698_JSON_H

 #ifdef __cplusplus
 extern "C" {
#endif
/*
#define JSONFILEPATH 			"/usr/app/" 				//JSON文件夹名字存放路径
#define JSONFILENAMEHEAD_FRE    "frz_data_"					//JSON冻结文件名字头
#define JSONFILENAMEHEAD_REAL   "realtime_data_"			//JSON分钟文件名字头
#define JSONTEMPFILENAEM        "filetemp"

#define JSONFREPATH			"/usr/app/frz_data/"			//冻结压缩包存放的位置
//#define JSONREALPATH		"/usr/app/real_data/" 			//实时压缩包存放的位置
#define JSONREALPATH		"/tmp/real_data/" 				//实时压缩包存放的位置

#define JSONFREPATH_RAM			"/tmp/frz_data/"			//JSON冻结文件名字头
#define JSONREALPATH_RAM		"/tmp/real_data/"			//JSON实时文件存放的位置


#define COMPRESSENDSTRING_GZ  	"tar.gz"
#define COMPRESSENDSTRING_BZ  	"tar.bz2"

#define DEBUG_JSON
//#define UNFORMAT_JSON
#define DEBUG_FIRST_SPACENUM 	4
#define DEBUG_SECOND_SPACENUM 	8
#define DEBUG_THIRD_SPACENUM 	12

#define STR_ADDR 		"addr"
#define STR_ITEMLIST 	"itemList"
#define STR_TYPE 		"type"
#define STR_ITEMID 		"itemId"
#define STR_OT 			"ot"
#define STR_DT 			"dt"
#define STR_INTY 		"intv"
#define STR_NUM 		"num"
#define STR_DL 			"dl"
#define STR_MV			"mv"
#define STR_MDT			"mdt"

#define STR_COLOUMNHEAD	"coloumnhead"

#define REPORT_FILEJSON_DATASIZE 1024
#define ONEDATASIZE 720

#define RAMTOFLASH_COPYMAXSIZE (8*1024*1024)  //当RAM的数据量大于这个值的时候就会把数据文件移动到flash
#define RAMTOFLASH_COPYONESIZE (1024*1024)		//每一次RAM从Flash拷贝的数量
*/
void SetReportInfo(void);
u8 GetReportInfo(void);
void ClrReportInfo(void);

void myprintf(char * pStr, u32 param1, u32 param2, u32 param3);

void Terminal_PackJSON(void);//终端打包JSON文件任务
void TaskReport_JsonFile(u32 PORTn);
u32 Report_JsonFile(u16 *pTime,u32 PORTn);

u8 *GetFileJsonName_Compress(u8 *ptime);//得到上报文件压缩包的名字(带绝对路径)
u8 *GetRealTime_FileJson(u8 *ptime);
u8 *GetRealTime_FileJson_Pack(u8 *ptime);//得到上报文件压缩包的名字(带绝对路径)
u8 *GetFrzFileJsonName_Compress(void);

u32 Find_TodayFrzFile(void);
void Mkdir_JsonFile(const char *path);

void Delet_JsonFile(u8 *fname);//删除昨天的数据
void Delet_All_FromFlash(u32 FileID);//删除Flash里面的数据
u32  Copy_RamToFlash(s32 fd);
u32 GetOneMetAddr_TEST(u16 *MetCount);
u32 CalCulate_Space(const char *path);
void Delet_RAM_FrzFile(void);
u32 GetOneMetAddr(u16 *MetCount);
void Crl_AllJsonFileAndTar(void);

/*
void text_flash(void);
void read_flash(u32 FileID,u16 *pAddr);
VOID test_saveMIN(void);
void tar_text(void);
void tar_thread(void);
void TEST_PACK(void);
void log_Flash(void);
VOID test_saveDAY(void);
VOID test_saveMIN15(void);
*/
//extern u8 IsCreateFrzJson;//今天是不是已经生成了日冻结文件
//extern u8 Lastrealtime[7];//打包前的时间需要保存



//==========================text==========================
/*
#define TEST_METNUM 400
#define TEST_TIME 1440
*/
#ifdef __cplusplus
 }
#endif
#endif

