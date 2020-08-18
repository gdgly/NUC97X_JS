
#ifndef NUC97x_DMA_H
#define NUC97x_DMA_H

void MR_DMA(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//使用DMA Channel 0存储器读(限仅使用于RAM)
void MW_DMA(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//使用DMA Channel 0存储器写(限仅使用于RAM)



#endif
