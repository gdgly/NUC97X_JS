

//数据格式转换
#ifndef DataConversion_H
#define DataConversion_H


u64 Data02_kW(u32 Data02);//数据格式02转为单位统一为kw的hex补码,有3位小数
u32 kW_Data02(u64 d64);//单位统一为kw有3位小数的hex补码转为数据格式02
u64 Data03_kWh(u32 Data03);//数据格式03转为单位统一为kwh的hex补码
u64 kWh_Data03(u64 d64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节

u32 Data_0xFFto0xEE(u32 DataAddr,u32 Byte);//比较数据:全0xFF则替换为全0xEE;返回0=不是全0xFF,1=是全0xFF





#endif



