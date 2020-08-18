

#ifndef CONGEAL_H
#define CONGEAL_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 Get_CongealFileData_RCSD(u8 *pRCSD,u8 *pRecord,u8* pTime,u32 NUM_DATA,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL);//冻结表的列选择;入口:pRCSD指向行选择的RCSD(无数据类型),pRecord指向数据块,pTime指向冻结时间(无数据类型),NUM_DATA冻结数据列表数,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
u32 Get_CongealFileData(u32 FileID,u32 FileCount,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax);//得到冻结文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF

void CONGEAL_Record(void);//冻结
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
