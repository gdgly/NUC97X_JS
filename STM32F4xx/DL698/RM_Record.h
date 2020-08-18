

#ifndef RM_Record_H
#define RM_Record_H
#ifdef __cplusplus
 extern "C" {
#endif



u32 Get_FileID(u32 PORTn);//UART各任务的UART各任务的FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0保留
u32 IDtoFileID(u32 id);//入口:任务号id,返回:文件存储FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类;无效id返回=0
u32 IDtoTaskID(u32 id);//入口:任务号id;出口：TaskID
//u32 RCSDtoNextProjectID(u8* pRCSD,u32 ProjectID);//记录列选择RCSD相对应的下个方案号,入口:ProjectID=开始搜索的方案号;返回:方案号,>=NUMmax_6014表示无效方案号
//u32 ProjectIDtoTaskID(u32 ProjectID);//普通采集方案号转为任务号;返回:采集任务的任务号,>=RMTASKmax表示无效任务号
u32 RCSDtoID(u8* pRCSD);//记录列选择RCSD相对应的任务号,返回:数据在采集任务的任务号,>=RMTASKmax表示无效任务号
u32 RCSDtoFileID(u8* pRCSD);//记录列选择RCSD相对应的FileID,入口:pRCSD指向类型字节;返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO=0xff;RCSD中只有通信地址等非抄表类数据时返回0xffffffff
u32 Get_RecordFileName(u32 FileID,u32 FileCount,u8* pTime);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0保留,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
u32 Get_RecordFileName_JS(u32 FileID,u32 FileCount,u8* pTime);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0保留,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
 	 
u32 Get_RecordFileData_RCSD(u8 *pRCSD,u8 *pAddr,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
u32 Get_LastRecordFileData(u32 ID,u16* pAddr);//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF的数据长度，0=没找到
u32 Get_LastRecordFileDataHasID(u32 ID,u16* pAddr);//
	 
u32 AutoDeleteFile(u32 filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
u32 AutoDelete_ErrID_RecordFile(void);//自动删除任务已不存在的记录文件;返回:0=没删除,1=有删除
	 
void RM_RecordFile(u32 PORTn,u8 *pbuf);//抄表数据处理(存贮,事件)
void RM_RecordFile_FailAddr(u32 PORTn);//抄表失败地址记录
	 
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
