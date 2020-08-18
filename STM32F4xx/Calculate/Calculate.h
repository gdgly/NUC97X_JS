
//Calculate
#ifndef Calculate_H
#define Calculate_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 hex_bcd(u32 hex);//HEX��תΪ8λѹ��BCD��
u64 hex_bcd16(u64 hex16);//HEX��תΪ16λѹ��BCD��
u32 bcd_hex(u32 bcd);//8λѹ��BCD��תΪHEX
u64 bcd16_hex(u64 bcd);//16λѹ��BCD��תΪHEX
u32 bcd_add(u32 bcd1,u32 bcd2);//�޷���bcd����
void Data_Inverse(u32 DataAddr,u32 Le);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
u32 DataComp(u32 DataAddr1,u32 DataAddr2,u32 Byte);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
u32 WinDataComp(u32 DataAddr1,u32 DataAddr2,u32 Byte,u32 TByte);//�������ݱȽ�,������DataAddr1��ͬ���ݵ�DataAddr2��ʼ��ַ,��=0�ں���TByte�ֽ�������ͬ
u32 BitLoca(u32 bin);//�����1����λΪ1��λ��(0-32)
u32 BitNum(u32 bin);//������λΪ1�ĸ���(0-32)

u32 YM_D(u32 YM);//ȡָ�����µ�����
u32 YMDCalculateW(u32 RTCAddr);//ָ����ַ�����е���������ʱ���������
u32 Check_BCD(u32 BCD);//BCD�Ϸ����,����0�Ϸ�,1�Ƿ�
u32 Check_HMS(u32 HMS);//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
u32 Check_YMD(u32 YMD);//�����պϷ����,����0�Ϸ�,1�Ƿ�
u32 Check_YMDHMS(u64 YMDHMS);//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
u32 YMDHMS_Sec(u32 RTCAddr);//ʱ�Ӷ�00��01��01��00ʱ00��00��������
u32 TimeDifference(u32 RTC1Addr,u32 RTC2Addr);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
u32 YMDHMS_Sub(u64 YMDHMS1,u64 YMDHMS2);//ʱ�Ӳ�ֵ����,���ʱ��1(���ʱ������);Rʱ��2(���ʱ������);����HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��

u32 YMD_Add1D(u32 YMD);//�����ռ�1��,���ؼ�1���������
u32 YMD_Sub1D(u32 YMD);//�����ռ�1��,���ؼ�1���������
u32 YM_Add1M(u32 YM);//���¼�1��,���ؼ�1�������
u32 YM_Sub1M(u32 YM);//���¼�1��,���ؼ�1�������
u64 YMDHM_AddM(u64 YMDHM,u32 m);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
u64 YMDHM_SubM(u64 YMDHM,u32 m);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
u32 YMDH_AddH(u32 YMDH,u32 h);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
u32 YMDH_SubH(u32 YMDH,u32 h);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
u64 YMDHMS_AddS(u64 YMDHMS,u32 S);//������ʱ�����S��,���ؼ�S����������ʱ����
u64 YMDHMS_SubS(u64 YMDHMS,u32 S);//������ʱ�����S��,���ؼ�S����������ʱ����

u32 CopyString(u8 *ps,u8 *pd);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
u8* SearchString(u8* pData,u8* pStd,u32 LEN_pData,u32 LEN_Std);//��������:���:pStdΪҪ��pData�е����ݴ�,LEN_pData=pData���ֽ���,LEN_Std=Ҫ�ҵ����ݴ��ֽ���(0ֵ��ʾҪ�ҵ����ݴ�Ϊ��0�����ı�׼�ַ���),����:0=û�ҵ�,!=0Ϊ��pData�ҵ���ͬ����ĵ�1���ֽڵ�ַ
u32 ASCII_HEX(u32 ASCIIAddr);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
u32 ASCIIdp_HEX(u8* pData);//��С����10���Ƶ�ASCII��תΪHEX,��"1234.56"->0x1e240,���:ASCII�뿪ʼ��ַ,��0-9��'.'ASCII��Ϊ����
u32 ASCII_BCD(u32 ASCIIAddr,u32 Byte);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
u32 BCD_ASCII(u32 BCD,u32 Addr);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
u32 HEX_ASCII(u32 HEX,u32 Addr);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
u32 IP_ASCII_HEX(u32 Addr);//ASCIIתΪHEX��IP;��ڿ�Ϊָ���ASCII����0-9�ĵ�ַ;����0��ʾ��IP��ַ
void IP4_HEX_ASCII(u8 *hex,u8 *ascii);

u64 YMDHM_Int15(u64 YMDHM);//������ʱ�ֻ�Ϊ��15��,��00ʱ18�ֻ�Ϊ00ʱ15��


//hex rtc
u32 YYM_D_hex(u32 YYM);//ȡָ�����µ�����
u32 Check_HMS_hex(u32 HMS);//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
u32 Check_YYMD_hex(u32 YYMD);//�������պϷ����,����0�Ϸ�,1�Ƿ�
u32 Check_YYMDHMS_hex(u64 YYMDHMS);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
u32 YYMDHMS_Sec_hex(u8* pRTC);//ʱ�Ӷ�00��01��01��00ʱ00��00��������;���ڵ��ֽ����ڸ��ֽ�
u32 YYMDHMS_Sub_hex(u8* pRTC1,u8* pRTC2);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
u64 YYMDHMS_Sub_hex64(u8* pRTC1,u8* pRTC2);//hex64λʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
u32 YYMD_Add1D_hex(u32 YYMD);//�������ռ�1��,���ؼ�1�����������
u32 YYMD_Sub1D_hex(u32 YYMD);//�������ռ�1��,���ؼ�1�����������
u32 YYM_AddM_hex(u32 YYM,u32 m);//�����¼�m��,���ؼ�m�º��������
u32 YYM_SubM_hex(u32 YYM,u32 m);//�����¼�m��,���ؼ�m�º��������
u64 YYMDHM_AddM_hex(u64 YYMDHM,u32 m);//��������ʱ�ּ�m��,���ؼ�m�ֺ����������ʱ��
u64 YYMDHM_SubM_hex(u64 YYMDHM,u32 m);//��������ʱ�ּ�m��,���ؼ�m�ֺ����������ʱ��
u32 YYMDH_AddH_hex(u32 YYMDH,u32 h);//��������ʱ��hʱ,���ؼ�hʱ�����������ʱ
u32 YYMDH_SubH(u32 YYMDH,u32 h);//��������ʱ��hʱ,���ؼ�hʱ�����������ʱ
u64 YYMDHMS_AddS_hex(u64 YYMDHMS,u32 s);//��������ʱ�����S��,���ؼ�S������������ʱ����
u64 YYMDHMS_SubS_hex(u64 YYMDHMS,u32 s);//��������ʱ�����S��,���ؼ�S������������ʱ����

u32 Check_pYYMDHMS_hex(u8* pYYMDHMS);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
void pYYMDHMS_AddS_hex(u8* pYYMDHMS,u32 s);//��������ʱ�����S��,���ؼ�S������������ʱ����
void pYYMDHMS_SubS_hex(u8* pYYMDHMS,u32 s);//��������ʱ�����S��,���ؼ�S������������ʱ����
void pYYM_AddM_hex(u8* pYYM,u32 m);//�����¼�m��,���ؼ�m�º��������
void pYYM_SubM_hex(u8* pYYM,u32 m);//�����¼�m��,���ؼ�m�º��������
void bcdYMDHMS_hexYYMDHMS(u8* pBCD,u8* pHEX);//BCD��������ʱ����תΪHEX����������ʱ����
void HexToASCII(u8* in, u8* out, u16 wInLen);
u32 IsAllAByte(const u8* p, u8 b, u16 len);


#ifdef __cplusplus
 }
#endif 
#endif




