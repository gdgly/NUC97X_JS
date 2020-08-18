

#ifndef DL698_DataDef_H
#define DL698_DataDef_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
//�������Ͷ���
#define DataType_NULL	0//	��	
#define DataType_array	1//	SEQUENCE OF Data����6.3.3.1���������Ԫ���ڶ������Ի򷽷��������ж���	
#define DataType_structure	2//	SEQUENCE OF Data����6.3.3.1�����ṹ��Ԫ���ڶ������Ի򷽷��������ж���	
#define DataType_bool	3//	����ֵ��BOOLEAN��	1��0
#define DataType_bit_string	4//	����λ����BIT STRING��	
#define DataType_double_long	5//	32����λ������Integer32��	-231��231-1
#define DataType_double_long_unsigned	6//	32����λ��������double-long-unsigned��	0��232-1
//����	7-8		
#define DataType_octet_string	9//	8����λλ�飨�ֽڣ�����OCTET STRING��	
#define DataType_visible_string	10//	ASCII�ַ�����VisibleString��	
//����	11		
#define DataType_UTF8_string	12//	UTF-8������ַ���	
//����	13-14		
#define DataType_integer	15//	8����λ������integer��	-128��127
#define DataType_long	16//	16����λ������long��	-32768��32767
#define DataType_unsigned	17//	8����λ��������Unsigned8��	0��255
#define DataType_long_unsigned	18//	16����λ��������Unsigned16��	0��65535
//����	19		
#define DataType_long64	20//	64����λ������Integer64��	-263��263-1
#define DataType_long64_unsigned	21//	64����λ��������Unsigned64��	0��264-1
#define DataType_enum	22//	ö�ٵ�Ԫ���ڶ������Ի򷽷��������ж���	0��255
#define DataType_float32	23//	octet-string��SIZE��4����
#define DataType_float64	24//	octet-string��SIZE��8����
#define DataType_date_time	25//	octet-string��SIZE��10����
#define DataType_date	26//	octet-string��SIZE��5����
#define DataType_time	27//	octet-string��SIZE��3����
#define DataType_date_time_s	28//octet-string��SIZE��7����

//����	29-79	
#define DataType_OI	80//
#define DataType_OAD	81//
#define DataType_ROAD	82//
#define DataType_OMD	83//
#define DataType_TI	84//
#define DataType_TSA	85//
#define DataType_MAC	86//
#define DataType_RN	87//
#define DataType_Region	88//
#define DataType_Scaler_Unit	89//
#define DataType_RSD	90//
#define DataType_CSD	91//
#define DataType_MS	92//
#define DataType_SID	93//
#define DataType_SID_MAC	94//
#define DataType_COMDCB	95//
#define DataType_RCSD	96//
//����	97-255	
	 
#define DataType_Data	255//�Զ���(�������ͷǹ̶���)


//��λ����
#define Unit_a  1//	a	ʱ��	��	
#define Unit_mo  2//	mo	ʱ��	��	
#define Unit_wk  3//	wk	ʱ��	��	7*24*60*60s
#define Unit_d  4//	d	ʱ��	��	24*60*60s
#define Unit_h  5//	h	ʱ��	Сʱ	60*60s
#define Unit_min  6//	min	ʱ��	��	60s
#define Unit_s  7//	s	ʱ�䣨t��	��	s
#define Unit_0  8//	��	���ࣩ��	��	rad*180/��
#define Unit_0c  9//	��	�¶ȣ�T��	���϶�	K-273.15
#define Unit_rmb  10//	����	�����أ�����		
#define Unit_m  11//	m	���ȣ�l��	��	m
#define Unit_m_s  12//	m/s	�ٶȣ�v��	��/��	m/s
#define Unit_m3  13//	m3	�����V��rV���Ǳ���������ֵ���ݻ���	������	m3
#define Unit_m3_  14//	m3	���������	������	m3
#define Unit_m3_h  15//	m3/h	����	������ÿСʱ	m3/(60*60s)
#define Unit_m3_h_  16//	m3/h	����������	������ÿСʱ	m3/(60*60s)
#define Unit_m3_d  17//	m3/d	����	������ÿ24Сʱ	m3/(24*60*60s)
#define Unit_m3_d_  18//	m3/d	����������	������ÿ24Сʱ	m3/(24*60*60s)
#define Unit_l  19//	l	�ݻ�	��	10-3m3
#define Unit_kg  20//	kg	������m��	ǧ��	kg
#define Unit_N  21//	N	����F��	ţ��	N
#define Unit_Nm  22//	Nm	����	ţ����	J=Nm=Ws
#define Unit_P  23//	P	ѹ����p��	��˹��	N/m2
#define Unit_bar  24//	bar	ѹ����p��	��	10-5N/m2
#define Unit_J  25//	J	����	����	J=Nm=Ws
#define Unit_J_h  26//	J/h	�ȹ�	��ÿСʱ	J/(60*60s)
#define Unit_W  27//	W	�й����ʣ�P��	��	W=J/s
#define Unit_kW  28//	kW	�й����ʣ�P��	ǧ��	kW=J/(s*1000)
#define Unit_VA  29//	VA	���ڹ��ʣ�S��	����	
#define Unit_kVA  30//	kVA	���ڹ��ʣ�S��	ǧ����	
#define Unit_var  31//	var	�޹����ʣ�Q��	��	
#define Unit_kvar  32//	kvar	�޹����ʣ�Q��	ǧ��	
#define Unit_kWh  33//	kWh	�й����� rw���й����ܱ���������ֵ	ǧ��-ʱ	kW*(60*60s)
#define Unit_kVAh 34//	kVAh	��������rS�����ڵ��ܱ���������ֵ	ǧ��-��-Сʱ	kVA*(60*60s)
#define Unit_kvarh  35//	kvarh	�޹�����rB���޹����ܱ���������	ǧ��-ʱ	kvar*(60*60s)
#define Unit_A  36//	A	������I��	����	A
#define Unit_C  37//	C	������Q��	����	C=As
#define Unit_V  38//	V	��ѹ��U��	����	V
#define Unit_V_m  39//	V/m	�糡ǿ�ȣ�E��	��ÿ��	V/m
#define Unit_F  40//	F	���ݣ�C��	����	C/V=As/V
#define Unit_R  41//	��	���裨R��	ŷķ	��=V/A
#define Unit_Rm  42//	��m2/m	����ϵ�����ѣ�		��m
#define Unit_Wb  43//	Wb	��ͨ��������	Τ��	Wb=Vs
#define Unit_T  44//	T	��ͨ�ܶȣ�T��	̩˹��	Wb/m2
#define Unit_A_m  45//	A/m	�ų�ǿ�ȣ�H��	����ÿ��	A/m
#define Unit_H  46//	H	��У�L��	����	H=Wb/A
#define Unit_Hz  47//	Hz	Ƶ��	����	1/s
#define Unit_1_Wh  48//	1/(Wh)	�й���������������		imp/kWh
#define Unit_1_varh  49//	1/(varh)	�޹���������������		
#define Unit_1_VAh  50//	1/(VAh)	������������������		
#define Unit__bfb  51//	%	�ٷֱ�	�ٷ�֮	
#define Unit_byte  52//	byte	�ֽ�	�ֽ�	
#define Unit_dBm  53//	dBm	�ֱ�����		
#define Unit_rmb_kWh  54//	Ԫ/kWh	���		
#define Unit_Ah  55//	Ah	��ʱ	��ʱ	
#define Unit_ms  56//	ms	����	����	
//57��253	����
//254	������λ
//255	�޵�λ��ȱ��λ������





typedef __packed struct
{	 
	u8 TypeNum;//�����б���
	u8 TypeList[256];//�����б�
}Attribute_TypeDef;//���Զ���

typedef __packed struct
{
#ifdef IC_STM32F4xx
	u16 OI;//OI
  u32 Addr;//ԭʼ���ݵ�ַ
	u8 LENper;//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	u8 array;//������
	u8 DataType;//��������
	s8 Scaler;//����
  u8 Unit;//��λ
	u8 IC;//�ӿ���
#endif
#ifdef IC_NUC9xx
	u32 OI;//u16 OI;//OI
  u32 Addr;//ԭʼ���ݵ�ַ
	u16 LENper;//u8 LENper;//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	u16 array;//u8 array;//������
	u8 DataType;//��������
	s8 Scaler;//����
  u8 Unit;//��λ
	u8 IC;//�ӿ���
#endif
}DL698_iMeterLIB_TypeDef;

typedef __packed struct
{
#ifdef IC_STM32F4xx
	u32 OAD;//�����ʶ
  u32 Addr;//���ݵ�ַ
	u16 LENper;//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	u32 LENmax;//FLASH�ռ���Ԥ������󳤶�
	u8 rw;//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	u32 *pDataType;//���������б�ָ��
	u32 *pInitData;//��ʼ��ԭʼ����ָ��
	u16 LEN_Sour;//��ʼ��ԭʼ���ݳ���
#endif
#ifdef IC_NUC9xx
	u32 OAD;//�����ʶ
  u32 Addr;//���ݵ�ַ
	u32 LENper;//u16 LENper;//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	u32 LENmax;//FLASH�ռ���Ԥ������󳤶�
	u32 rw;//u8 rw;//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	u32 *pDataType;//���������б�ָ��
	u32 *pInitData;//��ʼ��ԭʼ����ָ��
	u32 LEN_Sour;//u16 LEN_Sour;//��ʼ��ԭʼ���ݳ���
#endif
}DL698_SETLIB_TypeDef;

typedef __packed struct
{
#ifdef IC_STM32F4xx
	u16 OI;//�����ʶ
	u32 Addr;//���ò�����ַ
	u16 LENper;//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	u32 LENmax;//���ò���FLASH�ռ���Ԥ������󳤶�
	u8 rw;//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	u32 *pDataType;//���ò������������б�ָ��
	u32 *pInitData;//���ò�����ʼ��ԭʼ����ָ��
	u16 LEN_Sour;//���ò�����ʼ��ԭʼ���ݳ���
	
	u8 IC;//�ӿ���
  u32 Addr_EventOAD;//��Ч��ʶ,�ϱ���ʶ,��������OAD��ַ
	u32 *pInitEventOAD;//��Ч��ʶ,�ϱ���ʶ,��������OAD��ʼ��ԭʼ����ָ��
	u16 LEN_EventOAD;//��Ч��ʶ,�ϱ���ʶ,��������OAD��ʼ��ԭʼ���ݳ���
	
	u32 Addr_Currentvalue;//��ǰֵ��¼���ַ,0=��
#endif
#ifdef IC_NUC9xx
	u32 OI;//u16 OI;//�����ʶ
	u32 Addr;//���ò�����ַ
	u32 LENper;//u16 LENper;//�������ÿ���й̶����ʱΪÿ��Ĺ̶��������,����Ϊ0
	u32 LENmax;//���ò���FLASH�ռ���Ԥ������󳤶�
	u32 rw;//u8 rw;//��д:b0=��,b1=д,b2=ͨ�Ų���;0=�޶�д,1=ֻ��,2=ֻд,3=��д
	u32 *pDataType;//���ò������������б�ָ��
	u32 *pInitData;//���ò�����ʼ��ԭʼ����ָ��
	u32 LEN_Sour;//u16 LEN_Sour;//���ò�����ʼ��ԭʼ���ݳ���
	
	u32 IC;//u8 IC;//�ӿ���
  u32 Addr_EventOAD;//��Ч��ʶ,�ϱ���ʶ,��������OAD��ַ
	u32 *pInitEventOAD;//��Ч��ʶ,�ϱ���ʶ,��������OAD��ʼ��ԭʼ����ָ��
	u32 LEN_EventOAD;//u16 LEN_EventOAD;//��Ч��ʶ,�ϱ���ʶ,��������OAD��ʼ��ԭʼ���ݳ���
	
	u32 Addr_Currentvalue;//��ǰֵ��¼���ַ,0=��
#endif
}DL698_EVENTLIB_TypeDef;

typedef __packed struct
{
	u32 OAD;//�����ʶ
  u32 Addr;//���ݵ�ַ
}EVENT_TEMPDATALIB_TypeDef;//�����¼�����ʱ����ָ���

typedef __packed struct
{
	u32 *pLib;//���ַ
}DL698_LIB_List_TypeDef;




void Get_date_time_s(u8* p8d);//�õ�date_time_s��7�ֽ�
u64 R_Inverse(u64 Data,u32 Byte);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
u32 Bit_Inverse(u32 Data,u32 bit);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
u32 Get_DL698ByteTypeLenByte(u8 *p8);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
u32 Get_DL698ByteTypeLenValue(u8 *p8);//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
u32 Get_DL698BinTypeLenByte(u8 *p8);//�õ�DL698λ����������ʾ���ȵ�Ԫ���ֽ���
u32 Get_DL698BinTypeLenValue(u8 *p8);//�õ�DL698λ���ͳ��ȳ��ȵ�Ԫ��ֵתΪ�ֽ���
u64 Get_TSA_Value(u8 *p8);//�õ�TSA�Ĳ�����6�ֽ�ͨ�ŵ�ַ,����˳�򲻱�,���:p8=TSA���ͺ���ֽڣ�����Чֵ��=0
u32 TItoSECOND(u32 TI);//TIתΪ��ֵ,���:��λ�ڵ��ֽ�,����:hex��
u32 pTItoSECOND(u8 *pTI);//TIתΪ��ֵ,���:pTI����������,����:hex��
void pYYMDHMS_AddTI_hex(u8* pYYMDHMS,u8* pTI);//��������ʱ�����ʱ����TI,���:pYYMDHMS,pTI����������,���ؼӼ��TI�����������ʱ����

u32 DataLen_RSD_NoMS(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//������MS���Ȳ���
u32 DataLen_MS(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//
u32 DataLen_RCSD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//
u32 Get_DL698DataLen(u8* p8,Attribute_TypeDef *pAttribute,u32 TypeCount,u32 LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
u32 Get_DL698DataLen_S(u8* p8,u32 LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
u64 Get_DL698DataAddr(u8 *p8,Attribute_TypeDef *pAttribute,u32 TypeCount,u32 ElementCount,u32 Element,u32 LENmax);//�õ�DL698���ݵ�ַ,���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b63-b32 p8,b31-b8��Ԫ�ؼ���,b7-b0Ԫ�ؼ���
//����Ԫ�ص�ַ
//���:pData=Ҫ��������ݿ�ʼ��ַ
//LENmax_Data=Ҫ��������ݴ����ܵ���󳤶����ڵ�������nand flashʱ�ȶ���sdram�������ֽ���(Ϊ���ټ���,��������������ram�н���),��󳤶�����128k;�����ݱ���ram��ֵ�޹�;��������nand flash��LENmax_Data=0ʱ���Զ���ȡ128k����Ӱ�������ٶ�
//Element��Ҫ��������ݵ�ַ����Ԫ�ؼ���,ÿ1����������Ϊ1��Ԫ��,�ṹ�����鲻�����ж��ٸ�Ԫ�ؼ�Ϊ1��Ԫ��,Ԫ�ش�0��ʼ���,Ҫ�����Ԫ�ش�1��ʼ���0��Ԫ��Ϊ��ʼ��ַ�������
//SubElement������Ԫ��Ϊ�ṹ������,Ҫ������Ԫ�ؽṹ�������ڵ�Ԫ�ص�ַ��ָ����Ԫ�ؼ���,��Ԫ��Ҳ��0��ʼ���,Ҫ�������Ԫ�ش�1��ʼ���0����Ԫ��Ϊ��Ԫ�ص�ַ�������,����Ԫ��ʱ��ֵ����=0
//ע��:����Ԫ��Ϊ����ʱ,��Ԫ�ؼ�����ڼ����������,��Ԫ�ؼ���=(�ڼ���*������Ԫ�ظ���)+��ǰ��Ԫ�ؼ���;
//����:��ַָ��Ԫ�ص������ֽ�,0=����
u8 *Get_Element(u8 *pData,u32 Element,u32 SubElement,u32 LENmax_Data);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
u32 Get_ElementADDR(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data);//����Ԫ�ص�ַ(ʵ�ʵ�ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����

u32 Compare_TSA_MS(u8 *pADDR_6000,u8 *pTSA,u8 *pMS);//�Ƚ�TSA�Ƿ����MS����,���:pADDR_6000����RAM,pTSA,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
u32 Compare_SETNO_MS(u8 *pADDR_6000,u32 SETNO,u8 *pMS);//�Ƚ���������Ƿ����MS����,���:pADDR_6000����RAM,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
void MStoAddrList(u8*pMS);//���ܱ��ϵ���ַ�б�;���:pMS(����������);����:��ַ�б���ADDR_AddrList

u32 PORTntoOAD(u32 PORTn);//ͨ�ſں�תΪOADֵ
u32 OADtoPORTn(u32 OAD);//OADֵתΪͨ�ſں�
u32 Compare_DL698DataString(u8 *p1,u8 *p2,u32 Byte);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
u32 Compare_octet_string(u8 *p1,u8 *p2);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
u32 Compare_TSA_string(u8 *p1,u8 *p2);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
u32 OI_IC(u32 OI);//�õ��ӿ����;���:�����ʶOI
u32 AddrToMeterSetNo(u16* pAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
u32 AddrTowToMeterSetNo(u16* pAddr);//����ͨ�ŵ�ַ�ɼ�����ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
u32 AddrToMeterSetNoJS(u16* pAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��?
u16 *MeterSetAddrNo(u16* pCollectorAddr);

u32 Deal_DataNull_JS(u8 pDataType,u8* p8);



#ifdef __cplusplus
 }
#endif
#endif


