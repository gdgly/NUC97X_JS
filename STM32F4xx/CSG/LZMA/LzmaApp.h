



#ifndef LzmaApp_H
#define LzmaApp_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 RAM_LzmaEncode(u32 ADDR_Sour,u32 LEN_Sour,u32 ADDR_Dest,u32 DictSize);//RAM->RAM的Lzma压缩算法:入口：DictSize=字典长度;返回:压缩后的长度(包括开始的13字节为lzma格式头,0=ERR)
u32 RAM_LzmaDecode(u32 ADDR_Sour,u32 LEN_Sour,u32 ADDR_Dest,u32 MaxLEN_Dest);//RAM->RAM的Lzma解压缩算法;入口:LEN_Sour包括开始的13字节为lzma格式头;返回:解压缩后的长度(0=ERR)

	 
#ifdef __cplusplus
 }
#endif 
#endif

