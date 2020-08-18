

//���ݸ�ʽת��
#ifndef DataConversion_SH698_H
#define DataConversion_SH698_H

#define	PROTO_DT_NULL				0	//NULL
#define	PROTO_DT_ARRAY				1	//array
#define	PROTO_DT_STRUCTURE			2	//structure
#define PROTO_DT_BOOL				3	//bool
#define PROTO_DT_BIT_STRING			4	//bit-string
#define PROTO_DT_DOUBLE_LONG		5	//double-long
#define PROTO_DT_DOUBLE_LONG_UN		6	//double-long-unsigned
#define PROTO_DT_OCTET_STRING		9	//octet-string
#define PROTO_DT_VISIBLE_STRING		10	//visible-string
#define PROTO_DT_UTF8_STING			12	//UTF8-string
#define PROTO_DT_INTEGER			15	//integer
#define PROTO_DT_LONG				16	//long
#define PROTO_DT_UNSIGNED			17	//unsigned
#define PROTO_DT_LONG_UN			18	//long-unsigned
#define PROTO_DT_LONG64				20	//long64
#define PROTO_DT_LONG64_UN			21	//long64-unsigned
#define PROTO_DT_ENUM				22	//enum
#define PROTO_DT_FLOAT32			23	//float32
#define PROTO_DT_FLOAT64			24	//float64
#define PROTO_DT_DATE_TIME			25	//date_time
#define PROTO_DT_DATE				26	//date
#define PROTO_DT_TIME				27	//time
#define PROTO_DT_DATE_TIME_S		28	//date_time_s
#define PROTO_DT_OI					80	//OI
#define PROTO_DT_OAD				81	//OAD
#define PROTO_DT_ROAD				82	//ROAD
#define PROTO_DT_OMD				83	//OMD
#define PROTO_DT_TI					84	//TI
#define PROTO_DT_TSA				85	//TSA
#define PROTO_DT_MAC				86	//MAC
#define PROTO_DT_RN					87	//RN
#define PROTO_DT_REGION				88	//Region
#define PROTO_DT_SCALER_UNIT		89	//Scaler_Unit
#define PROTO_DT_RSD				90	//RSD
#define PROTO_DT_CSD				91	//CSD
#define PROTO_DT_MS					92	//MS
#define PROTO_DT_SID				93	//SID
#define PROTO_DT_SID_MAC			94	//SID_MC
#define PROTO_DT_COMDCB				95	//COMDCB
#define	PROTO_DT_RCSD				96	//RCSD

#define LOG_LEVEL_ERR				1
#define LOG_LEVEL_INFO				2
#define LOG_LEVEL_DEBUG				3
#define CURRENT_LOG_LEVEL			3
#define LOG_MAX_LEN					1*1024*1024
//����FCSУ���
//p8rxbuff = ��Ҫ�����ԭʼ����ָ��
//Byte = ��Ҫ��������ݳ���
u32 Get_16bitfcs_code(const u8* p8rxbuff,u32 Byte);//����FCSУ���
//s_data ԭʼ����ָ�룬������0x1c��ͷ��
//����ת��������ݸ�ʽ::
//s_data��ʽHEX:0x1c YY YY MM DD hh mm ss
//d_data��ʽBCD:ss mm hh 00 DD MM YY
void  DateTimeStoDCB(u8* s_data,u8* d_data);//��698��date_time_s��ʽ��ʱ��ת��376.1�ĸ�ʽ
u32 Get_LastTimeDi_From_OAD(u32 OAD,u32 type);//ͨ��OAD��ö�Ӧ���ۼ�ʱ������ݱ�ʶ��type=1��ʶA�� =2��ʶB��  =3��ʾC��
u32 Get_EventNumDi_From_OAD(u32 OAD);//ͨ��OAD��ö�Ӧ���¼����������ݱ�ʶ��type=1��ʶA�� =2��ʶB��  =3��ʾC��
u32 Get_Di_From_OAD(u32 OAD);//ͨ��OAD�õ�645�����ݱ�ʾ
u32 Get_698DateTime_st(u8*p8txbuff,u32  type);

u32 Change_698DateTime_to_376(u32 add_sour,u32 add_dest);//��698��ʱ�����ת��376��ʽ,�ɹ����س��ȣ�ʧ�ܷ���0
u32 Change_698DoubleLongUnsigned_to_376(u32 p_sour,u32 p_dest);//��698��ʱ�����ת��376��ʽ,�ɹ����س��ȣ�ʧ�ܷ���0
u32 Change_698LongUnsigned_to_376(u32 p_sour,u32 p_dest);//��698��ʱ�����ת��376��ʽ,�ɹ����س��ȣ�ʧ�ܷ���0
u32 Get_0xee_By_Type(u32 type,u32 p_dest);//���������ͣ���д��Ч����
u32 Get_Len_Of_Structure(u32 member_num, u8* apdu);//ȡ�ṹ����APDU�����ݳ���,ʧ�ܷ���0���ɹ����س���
u32 Get_OAD_Data_Position(u8*p8txbuff,u32 oad,u32 no);//OAD��˳��ߵ��ģ����㴦��������֡���ѯָ��OAD�����ݣ��������ݵ���ʼλ�ã������������͡�ʧ�ܷ���0
u32 Change_Form_698_to_376(u32 Oad,u32 ADDR_Dest,u32 ADDR_Sour);//��698���¼���¼ת��645-2007�ĸ�ʽ������Ǽ�¼,�ɹ������ֽ�����ʧ�ܷ���0
u32 Change_Data(u32 data,u32 type);//�ߵ�����data �ĸߵ�λ��type���ֽ���,type������С�ڵ���4�����ڵ���1��
void Log_Printf_Level(u32 log_level,const char* fmt,...);
void Log_Dumphex_Level(u32 log_level,u8 *title,u32 addr_log,u32 log_len);
u32 Write_Log_To_File(u32 addr_data,u32 data_len);//����־�ļ�д��־������д����־�ĳ��ȣ�0��ʾʧ�ܡ�log_level������־�ȼ�������ϵͳĬ����־���𽫲���ӡ
u32 Add_Log_Date_Head(u32 Addr);//���ʱ���������ʱ����ĳ���

#endif



