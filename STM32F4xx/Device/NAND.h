

#ifndef NAND_H
#define NAND_H



void Init_NAND(void);//复位nand
u64 ID_NAND(void);//读器件ID(1Gb*8=0x01F1801Dxx,1Gb*16=0x01C1805Dxx)
u32 Wait_NAND(u32 usMax);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
void Enable_Internal_ECC(void);//允许nand内部ECC
void Disable_Internal_ECC(void);//取消nand内部ECC


u32 Read_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//NAND_FLASH 没替换读
void BlockErase_NAND(u32 ADDR_Dest,u32 Busy_Wait);//NAND_FLASH 擦除当前1个块;没块替换;入口:Busy_Wait=1NAND忙时等待Busy_Wait=0不等待
u32 BLOCK_Write_NAND(u32 ADDR_Dest);//块写ADDR_128KWRITE_BUFF中数据到FLASH块,同时读出比较;返回;0=正确,1=错误;没块替换


u32 RePlace_BLOCK_ADDR(u32 ADDR);//取替换块地址
u32 RePlace_Read_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//NAND_FLASH 有替换读;返回:0=没错误,1=1位错误,2=无法校正的2位以上错误
void RePlace_Write_NAND(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//NAND_FLASH写;若错误则块替换
void RePlace_Clr_NAND(u32 Data,u32 ADDR_Dest,u32 Byte);//NAND_FLASH清除;若错误则块替换
void RePlace_BlockErase_NAND(u32 ADDR_Dest,u32 Byte);//NAND_FLASH 块擦除;有块替换


u32 Check_4kNandBuff(void);//检查4K(BKPSRAM) NAND FLASH的写缓冲,若无效则清0;返回:0=错误,1=正确
void NR_4kBuff(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//读NAND_FLASH的写入缓冲
void NB_4kBuff(u32 ADDR_Dest,u32 Byte);//擦除NAND_FLASH时同时更新写入缓冲
void NW_4kBuff(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//写入Nand FLASH的数据先到4kBuff
void NC_4kBuff(u32 Data,u32 ADDR_Dest,u32 Byte);//清除写入Nand FLASH的数据先到4kBuff
void NAND4kBuff_Write_NAND(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH

u32 Check_128kNandBuff(void);//检查128K(BKPSDRAM) NAND FLASH的写缓冲,若无效则清0;返回:0=错误,1=正确
void NR_128kBuff(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//读NAND_FLASH的写入缓冲
void NB_128kBuff(u32 ADDR_Dest,u32 Byte);//擦除NAND_FLASH时同时更新写入缓冲
void NW_128kBuff(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//写入Nand FLASH的数据先到128kBuff
void NC_128kBuff(u32 Data,u32 ADDR_Dest,u32 Byte);//清除写入Nand FLASH的数据先到128kBuff
void NAND128kBuff_Write_NAND(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH

#endif




