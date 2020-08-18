

#ifndef DMW_H
#define DMW_H

void DMW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//Ö±½Ó´æ´¢Æ÷Ð´(ÎÞ²Á³ý)
void DMC(u8 Data,u32 ADDR_Dest,u32 Byte);//Ö±½Ó´æ´¢Æ÷Çå³ý(ÎÞ²Á³ý)
void DMWR(u64 Data,u32 ADDR_Dest,u8 Byte);//¼Ä´æÆ÷Ö±½ÓÐ´µ½´æ´¢Æ÷(<=8×Ö½Ú)(ÎÞ²Á³ý)


#endif

