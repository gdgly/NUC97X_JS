
//Calculate
#ifndef Calculate_H
#define Calculate_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 hex_bcd(u32 hex);//HEX数转为8位压缩BCD数
u64 hex_bcd16(u64 hex16);//HEX数转为16位压缩BCD数
u32 bcd_hex(u32 bcd);//8位压缩BCD数转为HEX
u64 bcd16_hex(u64 bcd);//16位压缩BCD数转为HEX
u32 bcd_add(u32 bcd1,u32 bcd2);//无符号bcd数加
void Data_Inverse(u32 DataAddr,u32 Le);//数据倒序(高低字节调换),入口Le>=2
u32 DataComp(u32 DataAddr1,u32 DataAddr2,u32 Byte);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
u32 WinDataComp(u32 DataAddr1,u32 DataAddr2,u32 Byte,u32 TByte);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
u32 BitLoca(u32 bin);//计算第1个置位为1的位置(0-32)
u32 BitNum(u32 bin);//计算置位为1的个数(0-32)

u32 YM_D(u32 YM);//取指定年月的天数
u32 YMDCalculateW(u32 RTCAddr);//指定地址缓冲中的年月日周时分秒计算周
u32 Check_BCD(u32 BCD);//BCD合法检查,返回0合法,1非法
u32 Check_HMS(u32 HMS);//时分秒合法检查,返回0合法,1非法
u32 Check_YMD(u32 YMD);//年月日合法检查,返回0合法,1非法
u32 Check_YMDHMS(u64 YMDHMS);//年月日时分秒合法检查,返回0合法,1非法
u32 YMDHMS_Sec(u32 RTCAddr);//时钟对00年01月01日00时00分00秒的秒计数
u32 TimeDifference(u32 RTC1Addr,u32 RTC2Addr);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
u32 YMDHMS_Sub(u64 YMDHMS1,u64 YMDHMS2);//时钟差值计算,入口时钟1(秒分时日月年);R时钟2(秒分时日月年);返回HEX补码时钟快或慢的差值(秒),时钟1时钟2非法=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负

u32 YMD_Add1D(u32 YMD);//年月日加1日,返回加1后的年月日
u32 YMD_Sub1D(u32 YMD);//年月日减1日,返回减1后的年月日
u32 YM_Add1M(u32 YM);//年月加1月,返回加1后的年月
u32 YM_Sub1M(u32 YM);//年月减1月,返回减1后的年月
u64 YMDHM_AddM(u64 YMDHM,u32 m);//年月日时分加m分,返回加m分后的年月日时分
u64 YMDHM_SubM(u64 YMDHM,u32 m);//年月日时分减m分,返回减m分后的年月日时分
u32 YMDH_AddH(u32 YMDH,u32 h);//年月日时加h时,返回加h时后的年月日时
u32 YMDH_SubH(u32 YMDH,u32 h);//年月日时减h时,返回减h时后的年月日时
u64 YMDHMS_AddS(u64 YMDHMS,u32 S);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
u64 YMDHMS_SubS(u64 YMDHMS,u32 S);//年月日时分秒减S秒,返回减S秒后的年月日时分秒

u32 CopyString(u8 *ps,u8 *pd);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
u8* SearchString(u8* pData,u8* pStd,u32 LEN_pData,u32 LEN_Std);//数据搜索:入口:pStd为要从pData中的数据串,LEN_pData=pData总字节数,LEN_Std=要找的数据串字节数(0值表示要找的数据串为以0结束的标准字符串),返回:0=没找到,!=0为从pData找到相同串后的第1个字节地址
u32 ASCII_HEX(u32 ASCIIAddr);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
u32 ASCIIdp_HEX(u8* pData);//有小数点10进制的ASCII码转为HEX,例"1234.56"->0x1e240,入口:ASCII码开始地址,非0-9和'.'ASCII码为结束
u32 ASCII_BCD(u32 ASCIIAddr,u32 Byte);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
u32 BCD_ASCII(u32 BCD,u32 Addr);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
u32 HEX_ASCII(u32 HEX,u32 Addr);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
u32 IP_ASCII_HEX(u32 Addr);//ASCII转为HEX的IP;入口可为指向非ASCII数字0-9的地址;返回0表示非IP地址
void IP4_HEX_ASCII(u8 *hex,u8 *ascii);

u64 YMDHM_Int15(u64 YMDHM);//年月日时分化为整15分,如00时18分化为00时15分


//hex rtc
u32 YYM_D_hex(u32 YYM);//取指定年月的天数
u32 Check_HMS_hex(u32 HMS);//时分秒合法检查,返回0合法,1非法
u32 Check_YYMD_hex(u32 YYMD);//年年月日合法检查,返回0合法,1非法
u32 Check_YYMDHMS_hex(u64 YYMDHMS);//年年月日时分秒合法检查,返回0合法,1非法
u32 YYMDHMS_Sec_hex(u8* pRTC);//时钟对00年01月01日00时00分00秒的秒计数;年在低字节秒在高字节
u32 YYMDHMS_Sub_hex(u8* pRTC1,u8* pRTC2);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
u64 YYMDHMS_Sub_hex64(u8* pRTC1,u8* pRTC2);//hex64位时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
u32 YYMD_Add1D_hex(u32 YYMD);//年年月日加1日,返回加1后的年年月日
u32 YYMD_Sub1D_hex(u32 YYMD);//年年月日减1日,返回减1后的年年月日
u32 YYM_AddM_hex(u32 YYM,u32 m);//年年月加m月,返回加m月后的年年月
u32 YYM_SubM_hex(u32 YYM,u32 m);//年年月减m月,返回减m月后的年年月
u64 YYMDHM_AddM_hex(u64 YYMDHM,u32 m);//年年月日时分加m分,返回加m分后的年年月日时分
u64 YYMDHM_SubM_hex(u64 YYMDHM,u32 m);//年年月日时分减m分,返回减m分后的年年月日时分
u32 YYMDH_AddH_hex(u32 YYMDH,u32 h);//年年月日时加h时,返回加h时后的年年月日时
u32 YYMDH_SubH(u32 YYMDH,u32 h);//年年月日时减h时,返回减h时后的年年月日时
u64 YYMDHMS_AddS_hex(u64 YYMDHMS,u32 s);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
u64 YYMDHMS_SubS_hex(u64 YYMDHMS,u32 s);//年年月日时分秒减S秒,返回减S秒后的年年月日时分秒

u32 Check_pYYMDHMS_hex(u8* pYYMDHMS);//年年月日时分秒合法检查,返回0合法,1非法
void pYYMDHMS_AddS_hex(u8* pYYMDHMS,u32 s);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
void pYYMDHMS_SubS_hex(u8* pYYMDHMS,u32 s);//年年月日时分秒减S秒,返回减S秒后的年年月日时分秒
void pYYM_AddM_hex(u8* pYYM,u32 m);//年年月加m月,返回加m月后的年年月
void pYYM_SubM_hex(u8* pYYM,u32 m);//年年月减m月,返回减m月后的年年月
void bcdYMDHMS_hexYYMDHMS(u8* pBCD,u8* pHEX);//BCD的年月日时分秒转为HEX的年年月日时分秒
void HexToASCII(u8* in, u8* out, u16 wInLen);
u32 IsAllAByte(const u8* p, u8 b, u16 len);


#ifdef __cplusplus
 }
#endif 
#endif




