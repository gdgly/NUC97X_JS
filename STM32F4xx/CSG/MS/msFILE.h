

#ifndef msFILE_H
#define msFILE_H
#ifdef __cplusplus
 extern "C" {
#endif
	 

#define FILENAMEmax_FLASH     ((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(msFILE_TypeDef))//存储在FLASH中的文件的最大文件数
#define FILENAMEmax_RAM     (LEN_RAMmsFILEmanage/sizeof(msFILE_TypeDef))//存储在RAM中的文件的最大文件数
#define recordFILENAMEmax_FLASH     (FILENAMEmax_FLASH-0)//限定最大FLASH抄表记录用文件名
#define recordFILENAMEmax_RAM     (FILENAMEmax_RAM-0)//限定最大RAM抄表记录用文件名
//存储在FLASH中的文件名:0到(FILENAMEmax_FLASH-1)
//存储在RAM中的文件名:FILENAMEmax_FLASH到(FILENAMEmax_FLASH+FILENAMEmax_RAM-1)

void msfile_init(void);//msfile初始化
void msfile_format(void);//msfile格式化
u32 msfile_free(void);//msfile返回nand FLASH文件自由空间,要查RAM文件自由空间等同u32 ms_malloc_sizemax(void);//最大可分配的尺寸(字节数)
u32 msfile_write(u32 filename,u32 offset,u8 *buf,u32 byte);//msfile写;返回:写入字节数
u32 msfile_read(u32 filename,u32 offset,u8 *buf,u32 byte);//msfile读;返回:读出字节数
u32 msfile_Delete(u32 filename);//msfile删除;返回:0=失败,1=成功
u32 msfile_len(u32 filename);//msfile得到文件总长度
u32 msfile_set_lenmin(u32 filename,u32 lenmin);//msfile设置预留最小文件长度仅nand flash(原文件有效数据总长度及内容不变只预占用最小存储空间);返回:0=失败,1=成功
u32 msfile_check(u32 filename);//msfile文件检查;返回:0=空,1=有效文件
u32 msfile_rename(u32 filename,u32 newfilename);//文件名重命名;程序自动过程:先删除newfilename,然后重命名;返回:0=错误,1=成功

#if MainProtocol==698
void msfile_lenclear(u32 filename);//清0文件相关的次数,0-(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)表示文件在RAM
u32 msfile_getfilename(u32 FileID);//搜索符合任务FileID的文件名:返回:0-(FILENAMEmax_FLASH+FILENAMEmax_RAM-1),0xffffffff表示没找到
#endif


#ifdef __cplusplus
}
#endif
#endif

