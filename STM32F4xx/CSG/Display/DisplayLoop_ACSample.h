

#ifndef DisplayLoop_ACSample_H
#define DisplayLoop_ACSample_H


void GetACSample(u32 DI);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
u32 DisplayLoop_ACSample(u32 Row);//内部交采数据显示,返回下行

#endif
