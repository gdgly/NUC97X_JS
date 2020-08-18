/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_DLT645_Table.h
��  ��:mxn
��  ��:1.00
��  ��:20170501
��  ��:�������ݱ�ʶ��645���ݱ�ʶ��Ӧ��
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_PUBLICDEFINE_H
#define __QCSG_PUBLICDEFINE_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_UpLinkProtocol.h"
/* Exported define -----------------------------------------------------------*/

//CL3200̨����Ժ̨꣬�����ʱ�򿪴˺�
#define FKGA43_CL3200_TEST

//CT���ο�·��·�澯ʹ�ܺ꣬����ʱ�򿪴˺꣬ʵ��ʹ��ʱΪ�����󱨣��ر�
//#define FKGA43_TEST_Alarm_CT
#define CT_OPEN_LIMIT_VALUE 0x010	//CT���ο�· ��ֵ0.10V	�Խӿ�·0.08V���� ̨�忪·0.08V
#define	CT_SHORT_LIMIT_VALUE 0x080	//CT���ζ�· ��ֵ0.80V �ԽӶ�·���2.0V ̨���·1.35V-1.75V 

/*
 * TODO:����Э����ض���
 */
//Э�����ݳ��ȶ���
#define MAX_QCSG_FRAME_LEN                  1024                    //Э���������ݳ���
#define MAX_DLT645_FRAME_LEN				220						//���645���ݰ�����
//Э��֡�ؼ�����
//#define QCSG_FRAME_HEAD                     0x68                    //Э��֡ͷ
//#define QCSG_FRAME_TAIL                     0x16                    //Э��֡β
//֡��ʶ
#define FIR_FIN_00                          0x00                    //��֡�м�֡
#define FIR_FIN_01                          0x20                    //��֡����֡
#define FIR_FIN_10                          0x40                    //��֡��ʼ֡
#define FIR_FIN_11                          0x60                    //��֡
//�ն����п����붨��
#define C_LINK_ACTREPORT                    0xC9
#define C_TASK_ACTREPORT                    0xCB
//���������ض���
#if ((Project/100)==2)||((Project/100)==9)			// I �ͼ�����
	#define MAX_PN_NUM                      1024	//�û�����
	#define MAX_PN_NUM2						8		//�ܱ���
#elif ((Project/100)==5)	// II �ͼ�����
	#define MAX_PN_NUM                      512	//err20180519 II�ͼ�����֧������������
	#define MAX_PN_NUM2						8	//û��
#else
	#define MAX_PN_NUM  					16	//���ر���
	#define MAX_PN_NUM2                     16	//���ر���
#endif

#define MAX_ALARM_645_DI_NUM2				(36)	//���澯+�¼�����645���ݱ�ʶ����


#define MAX_RELAYTASK_NUM                   256
#define MAX_COMMONTASK_NUM                  32
#define P(n)                                ((u16)(((n-1)/8+1) << 8)+(1 << ((n-1)%8)))
#define P0                                  0
#define PN_MAX                              P(MAX_PN_NUM)
//�������Լ����
#define METER_DLT645_97                     0
#define METER_DLT645_07                     1
#define METER_GD_97                         2
#define METER_GD_07                         3
#define METER_WASION                        4
#define METER_LANDISGYR_B                   5
#define METER_LANDISGYR_D                   6
#define METER_EDMI                          7
//Э������ֶ���
#define QCSG_NO_ERR                         0           //�޴���
#define QCSG_ERR_NO_RES                     1           //�м�����û�з���
#define QCSG_ERR_DATA_ERR                   2           //�������ݷǷ�
#define QCSG_ERR_PWD_ERR                    3           //����Ȩ�޲���
#define QCSG_ERR_NO_FN                      4           //�޴�������
#define QCSG_ERR_REQ_TIMEOUT                5           //����ʱ��ʧЧ
#define QCSG_ERR_DESTADDR_ERR               6           //Ŀ���ַ����
#define QCSG_ERR_CRC_ERR                    7           //crc����
/*
 * TODO:Э�������ض���
 */
//�������Ͷ���
#define TD_NOW_TYPE                         0x01        //ʵʱ��������
#define TD_C_TYPE                           0x02        //���߶���ʱ��
#define TD_D_TYPE                           0x04        //�ն���ʱ��
#define TD_M_TYPE                           0x08        //�¶���ʱ��
//��鷽������
#define NO_CHECK_METHOD                     0x00        //����Ҫ���
#define BCD_CHECK_METHOD                    0x01        //BCD��ʽ���
//��ȡ��������
#define COMMON_TAKE_METHOD                  0x00        //ͨ����ȡ�취
#define PRICE_TAKE_METHOD                   0x01        //�������ݵ���ȡ�취
#define THREE_PHASE_TAKE_METHOD1            0x02        //������ص���ȡ�취1(���ࣺ�ܡ�A��B��C������ A��
#define THREE_PHASE_TAKE_METHOD2            0x04        //������ص���ȡ�취2(���ࣺA  ��B��C������ A��
//����ת����������
#define COMMON_COV_METHOD                   (0x00)     	//����ֱ�ӿ���
#define PRICE_COV_METHOD                    (0x01)		//�������ӷ����ֶ�
#define HBYTE_ADD_YEAR_COV_METHOD           (0x02)		//���ֽڲ��䵱ǰ�꣬��Ҫ����64597�����������ʱ��
#define HLBIT_ADD_ZERO_COV_METHOD           (0x03)		//����λ������λ��0����Ҫ����64597Э��������ݸ�ʽת��
#define LBYTE_ADD_ZERO_COV_METHOD           (0x04)		//���ֽڲ���0������64597˲ʱ�޹����ʸ�ʽת��
#define HBYTE_ADD_FF_COV_METHOD				(0x05)		
//���ֽڲ� FF,�ֽ���Ϊ��������ʶ��Ӧ�ĳ��� - ��ǰ���ݳ���,�˷�����Ҫ���ڽ�07���ݱ�ʶ���Ȳ��뵽�������ݱ�ʶ����
#define TAKE_PRICE_DATA_COV_METHOD          (0x06)     	//�ӿ����ݣ���ȡ���еķ�������
#define LBIT_ADD_ZERO_COV_METHOD			(0x07)	   	//����λ��0����Ҫ����97Э�鵱ǰ��ѹֵ����ת��
#define HBYTE_ADD_ZERO_COV_METHOD           (0x08)		//���ֽڲ���0������64597����ܴ��������������ܴ�����ʽת��
#define TAKE_SIG_DATA_COV_METHOD          	(0x09)     	//�ӿ����ݣ���ȡ��������
//20180307����
#define LBYTE_ADD_PRICE_METHOD				(0x0A)		//���ֽ�ֱ�����ӷ������ֶΣ�����97����������鳭����
#define DEMAND_PRICE_COV_METHOD				(0x0B)		//�����������ӷ����ֶ�

//20190109����
#define LBYTE_ABANDON_HBYTE_ZERO			(0x0C)	//������͵�һ���ֽڣ�����ֽڲ�0��������ʣ����


//err20190531 EDMI��Լ����
#define EDMI_IEEE754_FLOAT_METHOD			(0x0D)	//EDMI IEEE754������ת���㷨32bit
#define EDMI_IEEE754_DOUBLE_METHOD			(0x0E)	//EDMI IEEE754������ת���㷨64bit

#define EDMI_Q_TYPE_TIME_METHOD				(0x0F)	//3�ֽ� Time HH/MM/SS
#define EDMI_R_TYPE_TIME_METHOD				(0x10)	//3�ֽ� Data DD/MM/HH
#define EDMI_T_TYPE_TIME_METHOD				(0x11)	//6�ֽ� Time/Data DD/MM/HH HH/MM/SS

#define PRICE_COV_EDMI_IEEE754_DOUBLE_METHOD (0x12)	//D�������ӷ����ֶ�
#define EDMI_REQ_BLOCK_METHOD				(0x13)	//�������������ʱ�����ݿ�
#define EDMI_REQ_SINGLE_METHOD				(0x14)	//�������������ʱ�䵥����
#define EDMI_VOLTAGE_BLOCK_METHOD			(0x15)	//��ѹ���ݿ�
#define EDMI_VOLTAGE_SINGLE_METHOD			(0x16)	//�����ѹ����
#define EDMI_CURRENT_BLOCK_METHOD			(0x17)	//�������ݿ�
#define EDMI_CURRENT_SINGLE_METHOD			(0x18)	//�����������
#define EDMI_POWER_BLOCK_METHOD				(0x19)	//�������ݿ�
#define EDMI_POWER_SINGLE_METHOD			(0x20)	//���๦������
#define EDMI_FACTOR_BLOCK_METHOD			(0x21)	//�����������ݿ�
#define EDMI_FACTOR_SINGLE_METHOD			(0x22)	//�ܹ�������
#define EDMI_ANGLE_BLOCK_METHOD				(0x23)	//��λ�����ݿ�
#define EDMI_ANGLE_SINGLE_METHOD			(0x24)	//��λ��
#define EDMI_TIME_METHOD					(0x25)	//ʱ��
#define EDMI_DATA_METHOD					(0x26)	//���������� ������FF	//err20190713-2 ����YYMMDDWW ����DDMMYY

#define NOPRICE_COV_EDMI_IEEE754_DOUBLE_METHOD	(0x27)	//5��D���ݲ����ӷ����ֶ� �������ߺͶ���
#define NOPRICE_COV_EDMI_REQ_BLOCK_METHOD		(0x28)	//5������������ݼ�����ʱ�����ݿ鲻���ӷ����ֶ�


#define FLOAT_TYPE 							(1)		//4�ֽڸ�����
#define DOUBLE_TYPE 						(2)		//8�ֽڸ�����
#define R_TYPE 								(3)		//3�ֽ� Data DD/MM/HH
#define Q_TYPE 								(4)		//3�ֽ� Time HH/MM/SS
#define T_TYPE 								(5)		//6�ֽ� Time/Data DD/MM/HH HH/MM/SS





/*
 * TODO:ͨ����ض���
 */
//����ͨ������
#define UP_CHNO_ETH                         1           //������̫��ͨ��
#define UP_CHNO_4G                          2           //Զ��ģ��ͨ��
#define UP_CHNO_IR                          3           //����ͨ��
#define UP_CHNO_PS2                         4           //PS2ͨ��
#define UP_CHNO_RS485                       5           //RS485ͨ��
//����ͨ������
#define RT_CHNO_JC                          1           //����
#define RT_CHNO_RS485_I                     2           //RS485I
#define RT_CHNO_RS485_II                    3           //RS485II
/*
 * TODO:����״̬����
 */
//��·ά�����趨��
#define LOGIN_STEP                          0x01        //���͵�¼
#define HEARTBEAT_STEP                      0x02        //��������
#define MAX_LINKERR_CNT                     0x02        //��·ά��֡ʧ�ܴ���
/*
 * TODO:��������
 */
//������
#define FUNC_LOCK                           0x55        //������ס״̬
#define FUNC_UNLOCK                         0           //��������״̬
#define TASK_JZQ_LOCK						0xAA		//��� ������������ס״̬
#define TASK_FK_LOCK						0xBB		//��� ����������ס״̬
#define TASK_PB_UNLOCK						0			//��� �������״̬

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	DI_E0000C00 = 0,		//���ر�� 0�Ŀ��ؽӵ���	��
	DI_E0000C50,			//0xE0000C50������״̬��
	DI_E0000C2F,			//�������0��״̬����
	DI_E0000C3F,			//��������ִ�
	DI_E0000C40,			//ң����բʱ��
}QCSG_DICHANGE_E;
//ʱ��ṹ��
typedef RealTimeStruct TIME_PARAM_S;
/*
typedef struct tagTimeParam
{
    u8 u8Second;
    u8 u8Minute;
    u8 u8Hour;
    u8 u8Day;
    u8 u8Month;
    u8 u8Year;
	u8 u8Week;
	u8 u8Res1;
}TIME_PARAM_S;
*/
//��������ʱ��ṹ��
typedef struct tagFrozenTimeParam
{
    u8 u8YearH;
    u8 u8YearL;
    u8 u8Month;
    u8 u8Day;
    u8 u8Hour;
    u8 u8Minute;	
	u8 u8Second;
	u8 u8Res1;
}FROZEN_TIME_PARAM_S;
//645���ݽṹ��
typedef struct
{
    u8 u8RevLen;
    u8 u8SendLen;
	u8 u8Res1;
	u8 u8Res2;
    u8 u8RevBuf[MAX_DLT645_FRAME_LEN];
    u8 u8SendBuf[MAX_DLT645_FRAME_LEN];
}DLT645_FRAME_S;
typedef struct
{
    u32 u32Fn;                                          //Fn
    u8  u8AFN;                                          //��Ӧ��AFN
    u8  u8FunEnable;                                    //�����Ƿ�֧��
    u16 u8Len;                                          //���ȵ���0xFFFF��ʾ�Ƕ���
    u16 u16PnMin;                                       //��ӦPn��Сֵ
    u16 u16PnMax;                                       //��ӦPn���ֵ
}QCSG_RESOLVE_S;
typedef struct
{
    u32 u32Ident;                  //���ݱ�ʶ
    u8  u8CheckMethod;             //�Ի�ȡ�����ݲ���ʲô����У��
    u8  u8SiDataLen;               //��Ϊ�����ݱ�ʶʱ����ʾÿ����Ԫ�����ݳ��ȣ���Ϊ�������ݱ�ʶʱ����645���ݱ�ʶ��Ӧ�����ݳ���
    u8  u8BlDataLen;               //��Ϊ�����ݱ�ʶʱ����ʾ���е�Ԫ�������ܳ��ȣ���Ϊ�������ݱ�ʶʱ������Ӧ�������ݱ�ʶ��ʾ�����ݳ���
    u8  u8TakeMethod;              //��ʽת������
}DLT645_RESOLVE_S;

typedef struct
{
    u32 u32Ident;                  //���ݱ�ʶ
    //u8  u8CheckMethod;             //�Ի�ȡ�����ݲ���ʲô����У��
    //u8 	DataType;					//EDMI���ݶ�Ӧ������
    //u8  u8SiDataLen;               //��Ϊ�����ݱ�ʶʱ����ʾÿ����Ԫ�����ݳ��ȣ���Ϊ�������ݱ�ʶʱ����645���ݱ�ʶ��Ӧ�����ݳ���
    //u8  u8BlDataLen;               //��Ϊ�����ݱ�ʶʱ����ʾ���е�Ԫ�������ܳ��ȣ���Ϊ�������ݱ�ʶʱ������Ӧ�������ݱ�ʶ��ʾ�����ݳ���
    //u8  u8TakeMethod;              //��ʽת������
}EDMI_RESOLVE_S;

typedef struct
{
    u32 u32Ident;                  //���ݱ�ʶ
    //u8  u8TakeMethod;              //��ʽת������
}EDMI_FROZEN_RESOLVE_S;


typedef struct
{
    u32 u32Ident;                                       //���ݱ�ʶ
    u8  u8SiDataLen;                                    //���ܱ����صĵ�����Ԫ���ݳ���
    u8  u8BlDataLen;                                    //���ܱ����ص��ܵĳ���,������ʻ򲻶���(0xFF)
}DLT645_DATALEN_S;
//����0x0C��0x0D���ݱ�ʶ��Ӧ��
typedef struct
{
    u32 u32Fn;                                          //����Э���Ӧ��Fn
    DLT645_RESOLVE_S stDLT64597Resolve[12];             //����Э��Fn��Ӧ��64597���ݱ���
    DLT645_RESOLVE_S stDLT64507Resolve[12];             //����Э��Fn��Ӧ��64507���ݱ���
    u16 u16Dlen;                                        //����Э��Fn��Ӧ�����ݳ���,������ʻ򲻶��������ǰ��յ�Ĭ�� 4 ���ʼ�������ݳ��ȣ���Ĭ�����ݳ���
    u8  u8FnType;                                       //Fn�����ͣ����ն��ᡢʵʱ���ݵ�
}QCSG_DLT645_CON_S;

//����0x0C��0x0D���ݱ�ʶ��Ӧ�� EDMI��
typedef struct
{
    u32 u32Fn;                                          //����Э���Ӧ��Fn
    //DLT645_RESOLVE_S stDLT64597Resolve[12];             //����Э��Fn��Ӧ��64597���ݱ���
    EDMI_RESOLVE_S stEDMIResolve[12];             //����Э��Fn��Ӧ��64507���ݱ���
    u16 u16Dlen;                                        //����Э��Fn��Ӧ�����ݳ���,������ʻ򲻶��������ǰ��յ�Ĭ�� 4 ���ʼ�������ݳ��ȣ���Ĭ�����ݳ���
    u16  u8FixMethod;                                       //��ʽת������//��Ϊu16����
}QCSG_EDMI_CON_S;


//�����������ݱ�ʶ��Ӧ�� EDMI��
typedef struct
{
    u32 u32Fn;                                          //����Э���Ӧ��Fn
    EDMI_FROZEN_RESOLVE_S stEDMIFROZENResolve[12];      //����Э��Fn��Ӧ��64507���ݱ���
    u32  u32FixMethod;                                  //��ʽת������
}QCSG_EDMI_FROZEN_CON_S;


//�����澯������645���ݱ�ʶ��Ӧ��
typedef struct
{
    u32 u32AlarmIdent;                                  //�澯���ݱ�ʶ
    u32 u32Dlt64597Ident[2];                            //��Ӧ��64597���ݱ�ʶ
    u32 u32Dlt64507Ident[6];                            //��Ӧ��64507���ݱ�ʶ
}QCSG_ALARM_CON_S;
//Э������ýṹ��
typedef struct
{
    //���յ�����
    u16 u16RxLField;                                    //���ճ�����
    u16 u16TxLField;                                    //���ͳ�����
	u8  u8TxCField;                                     //���Ϳ�����
	u8  u8RxCField;                                     //���տ�����
	u8  u8RxAddrField[7];                               //���յ�ַ��
    u8  u8TxAddrField[7];                               //���͵�ַ��	
    u8  u8RxAFNField;                                   //��������AFN
    u8  u8RxSEQField;                                   //���յ�SEQ��
    u8  u8TpVTransferDelayTime;                         //������ʱ
    u8  u8TxAFNField;                                   //��������AFN
    u8  u8TxSEQField;                                   //���յ�SEQ��
	u8	u8Res1;
	u8	u8Res2;
	u8	u8Res3;
    u16 u16TxLen;                                       //Э������������ݳ���
    u16 u16SendLen;                                     //�������ݳ���
    u8  u8TxBuf[MAX_QCSG_FRAME_LEN];                    //Э������������ݴ�Ŵ�
    u8  u8SendBuf[MAX_QCSG_FRAME_LEN+100];              //���ͻ�����
    //��������֯���͵�֡�Ļ�����
    u16 u16RsStartPt;                                   //��������ʼָ��
    u16 u16RsEndPt;                                     //�����Ľ���ָ��
    u16 u16OutPt;                                       //�����ָ��
	u16 u16Res4;
}QCSG_RESOLVE_INFO_S;
//ԭʼ����Э��֡�ṹ��
typedef struct
{
    u16 u16ReqFrameLen;                                 //��ȡ��֡����
    u16 u16ResFrameLen;                                 //��ȡ��֡����
    u8  u8ReqFrame[MAX_QCSG_FRAME_LEN];                 //��ȡ��Э��֡
    u8  u8ResFrame[MAX_QCSG_FRAME_LEN/4];               //��ȡ��Э��֡
}QCSG_REVFRAME_S;
//�����ϱ��ṹ��
typedef struct
{
    u8  u8CurrConnectState;                             //��ǰ����״̬
    u8  u8LinkProcessStep;                              //��·ά�������׶�
    u8  u8LinkErrCnt;                                   //��·ά������ʧ�ܴ���
    u8  u8ReportPseq;                                   //�����ϱ�֡���к�
    TIME_PARAM_S stLastHeartBeatTime;                   //��һ������ʱ��
    QCSG_RESOLVE_INFO_S stReport_Info;                  //�ϱ���Ϣ�ṹ��
}QCSG_ACTIVEREPORT_S;


//ʵʱ��������ṹ��
typedef struct
{
	TaskIDEnum SrcTask;
	u8 Lock;
	u8 FrameType;
	u8 DataType;
	u8 RelayType;
	u8 RelayDstPort;
	u8 RelayBaudRate;
	u8 RelayParityMode;
	u8 RelayDataBits;
	u8 RelayStopBits;
	u8 RelayTimeOut;
	u8 RelayFrameLen;
	u16 PnListNum;
	u16 PnListLen;
	QCSGFrameInfoStruct QCSGFrameInfo;
	u8 Data[SIZE_8K];
}QCSG_RTREADMETER_REQ_INFO_S; // RealTimeAMRReqStruct;
//ʵʱ�������ؽṹ��
typedef struct
{
	u16 DataLen;
	u16 u16PnFnListNum;
	u8 FrameType;
	u8 DataType;
	u8 Res1;
	u8 Res2;
	
	QCSGFrameInfoStruct QCSGFrameInfo;	
	u8 u8PnFnData[SIZE_1K];
}QCSG_RTREADMETER_RES_INFO_S;


//������Ϣ�ṹ��
typedef struct
{
	u32 u32BpsCtrl;										//�����㲨����
    u16 u16RDMeterIndex;                                //�������ܱ�ָ��
    u16 u16RDEventIndex;                                //�¼�����ָ��
    u16 u16RDIdentIndex;                                //�������ݱ�ʶָ��
    u16 u16PnNum;                                       //��������������
    u16 u16PnList[MAX_PN_NUM2];                          //�������б�
	u8	u8MeterId[6];									//���浱ǰ�����ִεı���
	u8	u8ProtocolType;									//���浱ǰ�����ִεĵ��ܱ�Э��
	u8	u8FailTimes;									//���浱ǰ�����������ʧ�ܴ��� ����52byte

	//err20190411 ����645����������ȥ�ظ�
	u32 ReadOK_DI_645_07_Num;							//�Ѿ���������645���ݱ�ʶ����
	u32 ReadOK_DI_645_07[MAX_ALARM_645_DI_NUM2];		//�Ѿ���������645���ݱ�ʶ 4*36 = 144byte
}QCSG_RDMETER_INFO_S;
//���������õ���������Ϣ����Ϣ�Ŀ��ƽṹ��

#if ((Project/100)==9)
	typedef struct 
	{
		u8 u8Lock;
		u8 u8Rdm_Sem;
		u8 u8Rd_Period;
		u8 u8PB_Task_Lock;//u8Res2;//err20181019 ��� �������븺�ش��볭�������� 
		u32 u32ReadPortParamTick;
		TIME_PARAM_S stSysTime_Back;

		u32 u32LockTimeOutTick;//err20190226 ������ʱ��ʱ��
	}QCSG_RDMETER_CTRL_S;
#else
	typedef struct 
	{
		u8 u8Lock;
		u8 u8Rdm_Sem;
		u8 u8Rd_Period;
		u8 u8PnNum_EDMI_IsLoad;//err20190611������õ�ǰ�˿��ѵ�½�ı��������
		//u8 u8Res2;
		u32 u32ReadPortParamTick;
		
		u32 u32LockTimeOutTick;//err20190226 ������ʱ��ʱ��
		TIME_PARAM_S stSysTime_Back;
		
		
	}QCSG_RDMETER_CTRL_S;
#endif 

//�м����񳭶���Ϣ
typedef struct
{
    u8 u8RDRelayTaskIndex;                              //�����м�����ָ��
    u8 u8RelayTaskNum;                                  //�м���������
    u8 u8NewRelayTaskNum;                               //�¶�ȡ���м���������
	u8 u8Res1;
    u8 u8RelayTaskList[MAX_RELAYTASK_NUM];              //�м������б�0,1,2,3...
    u8 u8NewRelayTaskList[MAX_RELAYTASK_NUM];           //�¶�ȡ���м������б�
    TIME_PARAM_S stLastTaskTime[MAX_RELAYTASK_NUM];     //��һ��ִ�й���ʱ��
}QCSG_RDRELAYTASK_INFO_S;
//�м������ϱ���Ϣ
typedef struct
{
    u8 u8ReportTaskIndex;                               //�м�����ָ��
    u8 u8RelayTaskNum;                                  //�м���������
	u8 u8Res1;
	u8 u8Res2;
    u8 u8RelayTaskList[MAX_RELAYTASK_NUM];              //�м������б�0,1,2,3...
    TIME_PARAM_S stLastTaskTime[MAX_RELAYTASK_NUM];     //��һ���ϱ�ʱ��
}QCSG_RELAYTASK_REPORT_INFO_S;
//��ͨ���񳭶���Ϣ
typedef struct
{
    u8  u8PnNum;                                        //��������������
    u8  u8RDMeterIndex;                                 //�������ܱ�ָ��
    u8  u8RDCommonTaskIdentNum;                         //ĳһ�������õľ������ݱ�ʶ����
    u8  u8RDCommonTaskIdentIndex;                       //�������������������ָ��
	u32 u8RDTaskIdentList[AMR_MAX_DATA_ID];				//���õľ��峭�������б�
	u32 u32BpsCtrl;
    u16 u16PnList[MAX_PN_NUM2];                          //�������б�,���ڸ��أ����֧��8�������㣬����Ϊ8
    u8  u8CommonTaskNum;                                //��ͨ��������
    u8  u8RDCommonTaskIndex;                            //����������ָ��
    u8  u8NewCommonTaskNum;                             //�¶�ȡ����ͨ��������
	u8	u8Res1;
	u8	u8MeterId[6];									//��ǰ���ı��ı���
	u8	u8ProtocolType;									//��ǰ���ı���Э��
	u8	u8MeterPort;									//��ǰ���ı��Ķ˿ں�
    u8  u8CommonTaskList[MAX_COMMONTASK_NUM];           //��ͨ�����б�0,1,2,3...
    u8  u8NewCommonTaskList[MAX_COMMONTASK_NUM];        //�¶�ȡ����ͨ�����б�
    TIME_PARAM_S stLastTaskTime[MAX_COMMONTASK_NUM];    //��һ��ִ�й���ʱ��
}QCSG_RDCOMMONTASK_INFO_S;
//��ͨ�����ϱ���Ϣ
typedef struct
{
    u8  u8PnNum;                                        //�����������õĲ���������
    u8  u8ReportTaskIndex;                              //�ϱ�������ָ��
	u8  u8CommonTaskNum;                                //��ͨ��������
	u8	u8Res1;
    u16 u16PnList[MAX_PN_NUM2];                          //�������б�,���ڸ��أ����֧��8�������㣬����Ϊ8
    u8  u8CommonTaskList[MAX_COMMONTASK_NUM];           //��ͨ�����б�0,1,2,3...
    TIME_PARAM_S stLastTaskTime[MAX_COMMONTASK_NUM];    //��¼��һ��ִ�е�ʱ��
}QCSG_COMMONTASK_REPORT_INFO_S;
//����������ʱ�洢�ṹ��
typedef struct
{
	u32 u32DataLen;
	u32 u32SaveFlag;
	u8 u8Data[4088];
}QCSG_FROZEN_RAM_DATA_S;
typedef struct
{
    u32 DataID;                                                                      
	u32 FlashAddr;  
	u16 DataLen;
	u8 ReadEn;
	u8 WriteEn;
}QCSG_DIDInfo;
//�澯��ض���
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data00010000[4];//��ǰ������
	u8 u8Data00010100[4];//��ǰ���� ����1
	u8 u8Data00010200[4];//��ǰ���� ����2
	u8 u8Data00010300[4];//��ǰ���� ����3
	u8 u8Data00010400[4];//��ǰ���� ����4
	u8 u8Data00020000[4];//��ǰ������
	u8 u8Data00020100[4];//��ǰ���� ����1
	u8 u8Data00020200[4];//��ǰ���� ����2
	u8 u8Data00020300[4];//��ǰ���� ����3
	u8 u8Data00020400[4];//��ǰ���� ����4
	u8 u8Data00030000[4];//��ǰ����޹�1��
	u8 u8Data00040000[4];//��ǰ����޹�2��
	u8 u8Data00050000[4];//һ�����޹���
	u8 u8Data00060000[4];//�������޹���
	u8 u8Data00070000[4];//�������޹���
	u8 u8Data00080000[4];//�������޹���
	u8 u8Data0201FF00[6];//��ѹ��
	u8 u8Data0202FF00[9];//������	
	u8 u8RdDoneFlag;
	u8 u8Data0203FF00[12];	//�й����ʿ�
	u8 u8Data0204FF00[12];	//�޹����ʿ�
	u8 u8Data0206FF00[8];//����������
	u8 u8Data0207FF00[6];//��ǿ�
	u8 u8Res[2];
	u8 u8Data01010000[8];//(��ǰ)���������������ʱ��
	u8 u8Data01020000[8];//(��ǰ)���������������ʱ��
	u8 u8Data01030000[8];//(��ǰ)����޹�1�����������ʱ��
	u8 u8Data01040000[8];//(��ǰ)����޹�2 �����������ʱ��
	//156byte
	//err20190509 ���Ƕ�ť�����Ӹ澯���¼������������
	//u8 u8Data03300D00[3];//�����Ǵ���
	//u8 u8Res2;
	//u8 u8Data03300E00[3];//��ť�ǿ�������
	//u8 u8Res3;
	
}QCSG_RAM_DATA_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_RAM_DATA_S stPn_Ram_Data[MAX_PN_NUM2];	//err20190509 156*16 = 2496 ->2624
}QCSG_PN_RAM_DATA_LIST_S;
typedef struct
{
	//��ģ���Ѿ��ϱ�������ֵ��־
	u8 u8GetSwitchValueFlag;
	//������ֵ
	u8 u8SwitchValue;	//��(1)	��(0)
	//�澯�������ָ���־
	u8 u8AlarmFlag;
	u8 u8Res1;
}QCSG_ALARM_E2000001_INFO_S;
typedef struct
{
	//������ֵ
	u8 u8Switch1Value;
	u8 u8Switch2Value;
	u8 u8Switch3Value;
	u8 u8Switch4Value;
	//�澯�������ָ���־
	u8 u8Alarm1Flag;
	u8 u8Alarm2Flag;
	u8 u8Alarm3Flag;
	u8 u8Alarm4Flag;
	//��ģ���Ѿ��ϱ�������ֵ��־
	u8 u8GetSwitchValueFlag;	
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
}QCSG_ALARM_E2000039_INFO_S;

//err20181107 ���Ӹ澯 ���ʳ���ֵ
typedef struct
{
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;	
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000028_INFO_S;

typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�������״̬��7
	u8 u8Data04000507[2];
	//��ѹ���������
	u8 u8Data14000001[3];
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;	
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000003_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000003_INFO_S stE2000003Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000003_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000004_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000004_INFO_S stE2000004Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000004_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000005_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000005_INFO_S stE2000005Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000005_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000006_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000006_INFO_S stE2000006Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000006_LIST_S;

typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000007_0C_INFO_S;



typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200000D_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200000D_INFO_S stE200000DData[MAX_PN_NUM2];
}QCSG_ALARM_E200000D_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200000E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200000E_INFO_S stE200000EData[MAX_PN_NUM2];
}QCSG_ALARM_E200000E_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200000F_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200000F_INFO_S stE200000FData[MAX_PN_NUM2];
}QCSG_ALARM_E200000F_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000010_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000010_INFO_S stE2000010Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000010_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000011_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000011_INFO_S stE2000011Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000011_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000012_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000012_INFO_S stE2000012Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000012_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000013_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000013_INFO_S stE2000013Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000013_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000014_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000014_INFO_S stE2000014Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000014_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000015_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000015_INFO_S stE2000015Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000015_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000016_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000016_INFO_S stE2000016Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000016_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000017_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000017_INFO_S stE2000017Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000017_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000018_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000018_INFO_S stE2000018Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000018_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001A_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001A_INFO_S stE200001AData[MAX_PN_NUM2];
}QCSG_ALARM_E200001A_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001B_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001B_INFO_S stE200001BData[MAX_PN_NUM2];
}QCSG_ALARM_E200001B_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001C_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001C_INFO_S stE200001CData[MAX_PN_NUM2];
}QCSG_ALARM_E200001C_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001D_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001D_INFO_S stE200001DData[MAX_PN_NUM2];
}QCSG_ALARM_E200001D_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001E_INFO_S stE200001EData[MAX_PN_NUM2];
}QCSG_ALARM_E200001E_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200001F_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200001F_INFO_S stE200001FData[MAX_PN_NUM2];
}QCSG_ALARM_E200001F_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000020_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000020_INFO_S stE2000020Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000020_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000021_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000021_INFO_S stE2000021Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000021_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000022_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000022_INFO_S stE2000022Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000022_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000023_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000023_INFO_S stE2000023Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000023_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000024_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000024_INFO_S stE2000024Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000024_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000025_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000025_INFO_S stE2000025Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000025_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000026_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000026_INFO_S stE2000026Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000026_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000027_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000027_INFO_S stE2000027Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000027_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E2000029_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000029_INFO_S stE2000029Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000029_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	//�¼��������ָ��׶�
	u8 u8OccurStep;
	u8 u8RestoreStep;
	u8 u8Res1;
	//�������ָ�����ʱ��
	u32 u32OccurTick;
	u32 u32RestoreTick;
}QCSG_ALARM_E200002A_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002A_INFO_S stE200002AData[MAX_PN_NUM2];
}QCSG_ALARM_E200002A_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	u8 u8Res1;
}QCSG_ALARM_E200002B_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002B_INFO_S stE200002BData[MAX_PN_NUM2];
}QCSG_ALARM_E200002B_LIST_S;
typedef struct
{
	u16 u16Pn;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	u8 u8Res1;
	//�״μ�¼��ʱ��
	TIME_PARAM_S stStartTime;
	u8 u8LastData00010000[4];	//��һ�ζ�ȡ�������й���	
	u8 u8LastData00020000[4];	//��һ�ζ�ȡ�ķ����й���
	u32 u32CalcEkwh;			//�ù��ʼ�����ĵ�������
	u32 u32ReadEkwh;			//�ö�ȡ����ǰֵ����һ��ֵ����ĵ�������
}QCSG_ALARM_E200002D_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002D_INFO_S stE200002DData[MAX_PN_NUM2];
}QCSG_ALARM_E200002D_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data03300200[3];
	u8 u8Res1;
}QCSG_ALARM_E2000031_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000031_INFO_S stE2000031Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000031_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	u8 u8Res1;
}QCSG_ALARM_E2000032_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000032_INFO_S stE2000032Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000032_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data03300000[3];
	u8 u8Res1;
}QCSG_ALARM_E2000035_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000035_INFO_S stE2000035Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000035_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data04010001[42];//��һ�׵�һ��ʱ�α�����
	u8 u8Data04020001[42];//err20181101 ���� �ڶ��׵�һ��ʱ�α�����
	u8 u8Res1;
	u8 u8Res2;
}QCSG_ALARM_E2000036_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000036_INFO_S stE2000036Data[MAX_PN_NUM2];//48byte * Num
}QCSG_ALARM_E2000036_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data04000409[3];
	u8 u8Data0400040A[3];
	u8 u8Res1;
	u8 u8Res2;
}QCSG_ALARM_E2000037_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000037_INFO_S stE2000037Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000037_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u16 u16Ct;
	u16 u16Pt;
}QCSG_ALARM_E2000038_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000038_INFO_S stE2000038Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000038_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data03300E00[3];
	u8 u8Res1;
	u8 u8Res2;

//err20180925 ��������
	u8 u8Data00010000[4];
	u8 u8Data00020000[4];
	u8 u8Data00050000[4];
	u8 u8Data00060000[4];
	u8 u8Data00070000[4];
	u8 u8Data00080000[4];
	
}QCSG_ALARM_E200004D_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200004D_INFO_S stE200004DData[MAX_PN_NUM2];
}QCSG_ALARM_E200004D_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data03300D00[3];
	u8 u8Res1;
	u8 u8Res2;

	//err20180925 ��������
	u8 u8Data00010000[4];
	u8 u8Data00020000[4];
	u8 u8Data00050000[4];
	u8 u8Data00060000[4];
	u8 u8Data00070000[4];
	u8 u8Data00080000[4];
	
}QCSG_ALARM_E200004E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200004E_INFO_S stE200004EData[MAX_PN_NUM2];
}QCSG_ALARM_E200004E_LIST_S;
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Data00010000[4];
	u8 u8Data00020000[4];
}QCSG_ALARM_E200002C_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002C_INFO_S stE200002CData[MAX_PN_NUM2];
}QCSG_ALARM_E200002C_LIST_S;
typedef struct
{
	u16 u16Pn;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	u8 u8Res1;
	u8 u8Data00010000[4];
	u8 u8Data00020000[4];
	u32 u32E_Quantity;
	u32 u32LastTick;
}QCSG_ALARM_E200002E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002E_INFO_S stE200002EData[MAX_PN_NUM2];
}QCSG_ALARM_E200002E_LIST_S;
typedef struct
{
	u16 u16Pn;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	u8 u8FailTimes;
}QCSG_ALARM_E200002F_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200002F_INFO_S stE200002FData[MAX_PN_NUM2];
}QCSG_ALARM_E200002F_LIST_S;
typedef struct
{
	u16 u16Pn;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	u8 u8Res1;
	u32 u32PnrefStartEnergy;
	u32 u32PndifStartEnergy;
	u32 u32PnrefEnergy;
	u32 u32PndifEnergy;
	u32 u32PndifData00010000;
	u8  u8Data00020000[4];//err20180925 �����ϴε������ݼ�¼
}QCSG_ALARM_E2000030_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E2000030_INFO_S stE2000030Data[MAX_PN_NUM2];
}QCSG_ALARM_E2000030_LIST_S;
typedef struct
{
	u16 u16Pn;
	//�¼�������ʶ
	u8 u8AlarmFlag;
	u8 u8Res1;
	u8 u8Data04000101[4];
	u8 u8Data04000102[3];
	u8 u8Res2;
	u32 u32E_Quantity;
	u32 u32LastTick;
}QCSG_ALARM_E200003E_INFO_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ALARM_E200003E_INFO_S stE200003EData[MAX_PN_NUM2];
}QCSG_ALARM_E200003E_LIST_S;
//�¼���¼��ض���
typedef struct
{
	u16 u16Pn;
	u8 u8EventFlag;
	u8 u8Res2;	
	TIME_PARAM_S stStartTime;
	TIME_PARAM_S stEndTime;
	u8 u8Start00010000[4];
	u8 u8End00010000[4];
	u8 u8Start00020000[4];
	u8 u8End00020000[4];
	u8 u8Start00030000[4];
	u8 u8End00030000[4];
	u8 u8Start00040000[4];
	u8 u8End00040000[4];
}QCSG_EVENT_ERD3_INFO_S;

//�¼���¼��ض���
typedef struct
{
	u16 u16Pn;
	u8 u8EventFlag;
	u8 u8Res2;	
	TIME_PARAM_S stStartTime;
	TIME_PARAM_S stEndTime;
	
	u8 u8Data03300400[3];
	u8 u8Res1;

}QCSG_EVENT_E2010010_INFO_S;

//�¼���¼��ض���
typedef struct
{
	u16 u16Pn;
	u8 u8EventFlag;
	u8 u8Res2;	
	TIME_PARAM_S stStartTime;
	TIME_PARAM_S stEndTime;
	
	u8 u8Data03300E00[3];
	u8 u8Res1;

}QCSG_EVENT_E2010011_INFO_S;

//�¼���¼��ض���
typedef struct
{
	u16 u16Pn;
	u8 u8EventFlag;
	u8 u8Res2;	
	TIME_PARAM_S stStartTime;
	TIME_PARAM_S stEndTime;
	
	u8 u8Data03300D00[3];
	u8 u8Res1;

}QCSG_EVENT_E2010012_INFO_S;

//ʧѹ��¼
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_ERD3_INFO_S stERD3Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010004_06_LIST_S;
//ʧ����¼
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_ERD3_INFO_S stERD3Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010007_09_LIST_S;
//�����¼
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_ERD3_INFO_S stERD3Data[MAX_PN_NUM2];
}QCSG_EVENT_E201000B_0D_LIST_S;

//Уʱ��¼

typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_E2010010_INFO_S stE2010010Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010010_LIST_S;


//����ť�п�����¼
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_E2010011_INFO_S stE2010011Data[MAX_PN_NUM2];//52*16 = 832 -> 56*16
}QCSG_EVENT_E2010011_LIST_S;

//���ǿ�����¼
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_E2010012_INFO_S stE2010012Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010012_LIST_S;

//�������俪�ռ�¼
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_EVENT_ERD3_INFO_S stERD3Data[MAX_PN_NUM2];
}QCSG_EVENT_E2010015_LIST_S;


//ͳ����ض���
#if (DLT500_REPORT == 0)
typedef struct
{
	u16 u16Pn;
	u8 u8MonitorTime[3];
	u8 u8PassRate[3];
	u8 u8Overrrun[3];
	u8 u8PassRateTime[3];					//����Ϊ���㷽�����ӣ��洢ʱ���ô˽ṹ��
	u8 u8UpLimitTime[3];
	u8 u8LowLimitTime[3];
	u8 u8MaxVoltage[2];
	u8 u8MinVoltage[2];	
	u8 u8MaxVoltageTime[4];
	u8 u8MinVoltageTime[4];
}QCSG_VOLTAGE_PASSRATE_S;
#else
typedef struct
{
	u16 u16Pn;
	u8 u8MonitorTime[3];					//��ѹ���ʱ��
	u8 u8PassRate[3];						//��ѹ�ϸ���
	u8 u8UpLimintRate[3];					//��ѹ��������
	u8 u8LowLimintRate[3];                  //��ѹ��������
	u8 u8PassRateTime[3];	                //��ѹ�ϸ����ۼ�ʱ��				
	u8 u8UpLimitTime[3];                    //��ѹ�������ۼ�ʱ��
	u8 u8LowLimitTime[3];                   //��ѹ�������ۼ�ʱ��
	u8 u8MaxVoltage[2];
	u8 u8MinVoltage[2];	
	u8 u8MaxVoltageTime[4];
	u8 u8MinVoltageTime[4];
}QCSG_VOLTAGE_PASSRATE_S;
#endif
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_VOLTAGE_PASSRATE_S stPassRate[MAX_PN_NUM2];
}QCSG_VOLTAGE_PASSRATE_LIST_S;
typedef struct
{
	u16 u16Pn;
	u8 u8Max_A_Current[3];
	u8 u8Min_A_Current[3];
	u8 u8Max_B_Current[3];
	u8 u8Min_B_Current[3];
	u8 u8Max_C_Current[3];
	u8 u8Min_C_Current[3];		
	u8 u8Max_A_Current_Time[4];
	u8 u8Min_A_Current_Time[4];
	u8 u8Max_B_Current_Time[4];
	u8 u8Min_B_Current_Time[4];
	u8 u8Max_C_Current_Time[4];
	u8 u8Min_C_Current_Time[4];
}QCSG_CURRENT_EXTREME_PASSRATE_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_CURRENT_EXTREME_PASSRATE_S stCurrentExtreme[MAX_PN_NUM2];
}QCSG_CURRENT_EXTREME_PASSRATE_LIST_S;
//��ֵͳ�ƽṹ��
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u8 u8Max_Value[4];
	u8 u8Min_Value[4];
	u8 u8Max_Value_Time[4];
	u8 u8Min_Value_Time[4];
}QCSG_VALUE_EXTREME_PASSRATE_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_VALUE_EXTREME_PASSRATE_S stCurrentExtreme[MAX_PN_NUM2];
}QCSG_VALUE_EXTREME_PASSRATE_LIST_S;
//������������ͳ�ƽṹ��
typedef struct
{
	u16 u16Pn;
	u8 u8MonitorTime[3];
	u8 u8Zone1Time[3];
	u8 u8Zone2Time[3];
	u8 u8Zone3Time[3];
	u8 u8Zone4Time[3];
	u8 u8Zone5Time[3];
	u8 u8Zone6Time[3];
	u8 u8Zone7Time[3];
	u8 u8Zone8Time[3];
	u8 u8Zone9Time[3];
	u8 u8Zone10Time[3];
}QCSG_POWER_FACTOR_ZONE_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_POWER_FACTOR_ZONE_S stPowerFactorZone[MAX_PN_NUM2];
}QCSG_POWER_FACTOR_ZONE_LIST_S;
//���ɿ������ݽṹ��
typedef struct
{
	u32 u32CtrlLedTick;
	//��ȡ���Ʋ�����ʱ��,����10msTick�����̼�¼����
	u32 u32ReadCtrlParamTick;
	//��ʱ�޵����ʱ�䣬����10msTick�����̼�¼����
	u32 u32TempRation_Time;	//�޵�ʱ��
	//ң�ؽ���ʱ��
	u32 u32YK_Timeout;		//�ڲ���0xE0000C40ң����բʱ��Ļ����ϼ���u32CurrTick
	//���������������10msTick�����̼�¼����
	u32 u32SlipTick;
	//�����ۼӼ�����������10msTick�����̼�¼����
	u32 u32SumPowerTick;
	//�����ɳ���ʱ�䣬����10msTick
	u32 u32PowerOccurTimeoutTick;
	//�ָ����ɳ���ʱ�䣬����10msTick
	u32 u32PowerRestoreTimeoutTick;
	//�澯���������澯�̵���������ʱ��������10msTick�����̼�¼����
	u32 u32AlarmTick;
	//�ִμ̵���������ʱ��������10msTick
	u32 u32CtrlOutTick;
	//�澯��ʱ��ʱ��������10msTick����ʱʱ�䵽��ֹͣ���棬�����̵��������̼�¼����
	u32 u32PowerAlarmTimeoutTick;
	//�澯��ʱ��ʱ��������10msTick����ʱʱ�䵽��ֹͣ���棬�����̵��������̼�¼����
	u32 u32EnergyAlarmTimeoutTick;
	//�澯��ʱ��ʱ��������10msTick����ʱʱ�䵽��ֹͣ���棬�����̵��������̼�¼����
	u32 u32YKAlarmTimeoutTick;		
	//��͸��ɣ�Э�����ñ���
	u32 u32LimitPower;
	//�����ɳ���ʱ�䣬Э�����ñ���
	u32 u32PowerOccurTimeout;
	//�����ɻָ�ʱ�䣬Э�����ñ���
	u32 u32PowerRestoreTimeout;
	//�澯��ʱ��Э�����ñ���
	u32 u32AlarmTimeout;//0xE0000C68����բ����澯��ʱ��ȱʡ 10 ����
	//�µ����޶Э�����ñ���
	u32 u32MaxEnergy_Month;		//E0000E00�µ������ƣ��µ����޶һ��ֵ��
	//�ִε�����Э�����ñ���
	u32 u32RoundEnergy_Month;		//E0000E01�µ������ƣ��ִε���(����)
	//����������,1minһ�ι��ʼ��㣬��15����15min�Ļ�����Ҵ�ֵ��Ϊ0�����̼�¼����
	u32 u32SlipCnt;
	//�Žڵ����Ա仯�����ʱ��ʱ����Ϊ�˷���̨��������̣�̨��������Ժ󣬹�30sec���жϸ澯����
	u32 u32Switch0Timeout;
	//����������ʱ��
	u32 u32BuzzerTimeout;
	u32 u32BuzzerTickBack;
	//ÿ���Ӽ���Ĺ����ۼ�ֵ�����̼�¼����
	s32	s32MinutePowerList[15];
	//����ʣ����̼�¼����
	s32 s32SlipPower;
	//ʱ�ο��ܼӹ���
	s32 s32SumPower;
	//�����ܼ���
	u16 u16PowerSumPnList[8];//0xE0000C69�����ۼӵĲ������1~8
	//����ܼ���
	u16 u16EnergySumPnList[8];
	//���ز���
	KwCtrlStruct stKwCtrl[4];
	//��ʼ�ϵ��ʶ�����ϵ�=0������ʱ�����=1�����̼�¼����
	u8 u8PowerUpFlag;	
	//�����ۼӱ�־��Э�����ñ���
	u8 u8PowerSumFlag;
	//����ۼӱ�־��Э�����ñ���
	u8 u8EnergySumFlag;
	//����״̬��Э�����ñ���
	u8 u8PowerHoldState;
	//������Ч��־��Э�����ñ���
	u8 u8PowerCtrlFlag;
	//�����Ч��־��Э�����ñ���
	u8 u8EnergyCtrlFlag;
	//��ʱ������Ч��־��Э�����ñ���
	u8 u8TempRationFlag;
	u8 u8TempRationFloatCoeff;	//��ʱ�޵�ָ��
	//��ʱ�޵翪ʼʱ��
	TIME_PARAM_S stTempRationStartTime;
	//��ʱ�޵��ϵ��־
	u8 u8TempRationPowerUpFlag;
	//���ʶ�ֵ����ϵ����Э�����ñ���
	u8 u8PowerFloatCoeff;
	//������ֵ����ϵ����Э�����ñ���
	u8 u8EnergyFloatCoeff;
	//���ݿ���Ч��־��Э�����ñ���
	u8 u8FactoryRestFlag;
	//���ʿ��ƻ���ʱ�䣬Э�����ñ���
	u8 u8SlipTime;
	//�����ִ���Ч��־��Э�����ñ���
	u8 u8PowerCtrl_RoundFlag;
	//�������ִ���Ч��־��Э�����ñ���
	u8 u8EnergyCtrl_RoundFlag;	//0xE0000C64�µ���������բ�ִ���Ч��־
	//ң���ִ���Ч��־��Э�����ñ���
	u8 u8YKCtrl_RoundFlag;		//��վҪ��Ҫ���ļ���
	//�澯��ʶ�����̼�¼����
	u8 u8PowerAlarmFlag;	//�й��ظ澯ʱ����1
	//�澯��ʶ�����̼�¼����
	u8 u8EnergyAlarmFlag;
	//�澯��ʶ�����̼�¼����
	u8 u8YKAlarmFlag;
	//���ؼ̵������״̬�����ݴ�״̬������բ,���̼�¼����
	u8 u8PowerCtrlSwitchState;
	//��ؼ̵������״̬�����ݴ�״̬������բ,���̼�¼����
	u8 u8EnergyCtrlSwitchState;
	//Զ��ң��״̬�����ݴ�״̬������բ�����̼�¼����
	u8 u8YKCtrlSwitchState;	//���ص�ʵ��״̬
	//Զ��ң�ؿ������״̬��Э�����ñ���
	u8 u8YKSwitchOutState;	//0xE0000C20~0xE0000C2E��·���ص�����բ״̬����OutputSwitchState���ϳɰ�λ��
	//LED��״̬�����̼�¼����
	u8 u8LedState;
	u8 u8LedStateBack;
	u8 u8Switch0Attr;	//����(0)����(1)
	u8 u8Switch1Attr;
	u8 u8Switch2Attr;
	u8 u8Switch3Attr;
	u8 u8Switch4Attr;	
	//ң��״̬��ң��ģ���ϱ���¼����
	u8 u8YXState;
	u8 u8YXLastState;
	//ң��״̬��ң�ع��̼�¼����
	u8 u8YKState;
	//ң��״̬���ݣ����ڽ��ʱ�ı���
	u8 u8YKStateBack;
	//���ؽ׶α��������ڹ����ɳ���ʱ�丨����־
	u8 u8PowerCtrlStep;
	//�̵��������ʱ�������������������
	u8 u8SwitchOutTick;
	//�������ڵ���־
	u8 u8SlipTimeoutFlag;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
}QCSG_CTRL_INFO_S;
//�������ṹ��
typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	u32 u32Energy00010000;	//���������ǵ��ܣ���ͬ
	u32 u32Energy00010100;
	u32 u32Energy00010200;
	u32 u32Energy00010300;
	u32 u32Energy00010400;
	u32 u32Energy00020000;
	u32 u32Energy00020100;
	u32 u32Energy00020200;
	u32 u32Energy00020300;
	u32 u32Energy00020400;
	u32 u32Energy00030000;
	u32 u32Energy00040000;
}QCSG_ENERGY_S;
//�µ���
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ENERGY_S stPn_Ram_Data[MAX_PN_NUM2];
}QCSG_MONTH_ENERGY_LIST_S;
//�յ���
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ENERGY_S stPn_Ram_Data[MAX_PN_NUM2];
}QCSG_DAY_ENERGY_LIST_S;
//���Ƶ�����Ҫ�������Ϣ
typedef struct 
{
	u32 u32OverLimitEnergy;	//��һ�β����澯�ĵ��ܣ���Ϊ��һ�ε���ֵ
	u32 u32RationEnergy;		//��ǰ���������ִε������٣������߲�ֵ,�ò�ֵ���ִε�������u32RoundEnergy_Month�Ƚ�
	//���ݵ�ϵͳʱ�䣬�µ����������ʹ��
	TIME_PARAM_S stSysCurrTime;
	//���ݵ���ʱ�޵����ʼʱ��
	TIME_PARAM_S stTempRationStartTime;
}QCSG_CTRL_POWERDOWN_SAVE_INFO_S;
typedef struct
{
	u32 u32CurrDayStatisticsIdent;		//����
	u32 u32TDStatisticsIdent;		//��
	u32 u32CurrMonthStatisticsIdent;	//����
	u32 u32TMStatisticsIdent;			//��
}QCSG_STATISTICSDATA_TABLE_S;

#define LCD_RT_AMR_REQ			(0x0001)

#define LCD_RT_AMR_REQ_TEST_START	(0x1000) //err20180730 ̨�� ΢��������ģ��ʵʱ�ٲ������ʱ�л������ʣ��·�һ��GUIʵʱ�ٲ�֡��Ӧ�����ʡ�
#define LCD_RT_AMR_REQ_TEST_END		(0x2000) //err20180730 ̨�� ΢��������ģ��ʵʱ�ٲ������ʱ�л������ʣ��·�һ��GUIʵʱ�ٲ�֡��Ӧ�����ʡ�

#define LCD_RT_AMR_READY		(0x0010)
#define LCD_RT_BOTTOM_ICO		(0x0080)

typedef struct
{
	u16 u16Pn;
	u16 u16Res1;
	//��Ҫ���������ݱ�ʶ
	u32 u32ReqNwIdentNum;
	u32 u32ReqNwIdentList[8];
	//�������
	u8 u8Data0202FF00[12];
	//�����ѹ
	u8 u8Data0201FF00[6];	
	//����Ƶ��
	u8 u8Data02800002[2];
	//�ܹ�������
	u8 u8Data0206FF00[8];
	//������λ��
	u8 u8Data0207FF00[12];
	//�й�����
	u8 u8Data0203FF00[12];	
	//�޹�����
	u8 u8Data0204FF00[12];	
	//��ǰ�����й�����ʾֵ
	u8 u8Data0001FF00[20];		
	//���������й�����ʾֵ
	u8 u8Data0002FF00[20];	
	//����޹�1
	u8 u8Data0003FF00[20];	
	//����޹�2
	u8 u8Data0004FF00[20];	
	//���������й�������
	u8 u8CurrDay0001FF00[20];		
	//���շ����й�������
	u8 u8CurrDay0002FF00[20];			
	//���������й�������
	u8 u8CurrMonth0001FF00[20];	
	//���·����й�������
	u8 u8CurrMonth0002FF00[20];		
	//���������޹��ܵ�����
	u8 u8CurrDay0003FF00[4];
	//���շ����޹��ܵ�����	
	u8 u8CurrDay0004FF00[4];
	//���������޹��ܵ�����	
	u8 u8CurrMonth0003FF00[4];	 
	//���·����޹��ܵ�����	
	u8 u8CurrMonth0004FF00[4];		
	
	//г�����ݣ�ʵʱ���ݣ�
	//A���ѹ��г��������
	u8 u8Data020A0100[2];  
	//B���ѹ��г��������	
	u8 u8Data020A0200[2];  
	//C���ѹ��г��������	
	u8 u8Data020A0300[2];  
	//A�������г��������	
	u8 u8Data020B0100[2]; 
	//B�������г��������	
	u8 u8Data020B0200[2];   
	//C�������г��������		
	u8 u8Data020B0300[2];    

#if (DLT500_REPORT == 1)
	//��ѹ��⣨ʵʱ���ݣ�
	//(����)�ܵ�ѹ�ϸ���ͳ������
	u8 u8DataE1000010[33];   
	//(����)A���ѹ�ϸ���ͳ������	
	u8 u8DataE1000011[33];  
	//(����)B���ѹ�ϸ���ͳ������	
	u8 u8DataE1000012[33];        
	//(����)C���ѹ�ϸ���ͳ������
	u8 u8DataE1000013[33];    
	//(����)�ܵ�ѹ�ϸ���ͳ������    
	u8 u8Data03100000[33];  
	//(����)A���ѹ�ϸ���ͳ������	
	u8 u8Data03100100[33];  
	//(����)B���ѹ�ϸ���ͳ������	
	u8 u8Data03100200[33];   
	//(����)C���ѹ�ϸ���ͳ������	
	u8 u8Data03100300[33];   
#else
	//��ѹ��⣨ʵʱ���ݣ�
	//(����)�ܵ�ѹ�ϸ���ͳ������
	u8 u8DataE1000010[27];   
	//(����)A���ѹ�ϸ���ͳ������	
	u8 u8DataE1000011[27];  
	//(����)B���ѹ�ϸ���ͳ������	
	u8 u8DataE1000012[27];        
	//(����)C���ѹ�ϸ���ͳ������
	u8 u8DataE1000013[27];    
	//(����)�ܵ�ѹ�ϸ���ͳ������    
	u8 u8Data03100000[27];  
	//(����)A���ѹ�ϸ���ͳ������	
	u8 u8Data03100100[27];  
	//(����)B���ѹ�ϸ���ͳ������	
	u8 u8Data03100200[27];   
	//(����)C���ѹ�ϸ���ͳ������	
	u8 u8Data03100300[27];        
#endif

	//��ѹ��ƽ���Խ��ͳ�����ݣ�ʵʱ���ݣ�
	//(����)��ֵ����ֵ����ʱ�䡢��ֵ�ۼ�ʱ��
	u8 u8DataE100001C[9];    
	//(����)��ֵ����ֵ����ʱ�䡢��ֵ�ۼ�ʱ��	
	u8 u8DataE100401C[9];  
	
	//������ƽ���Խ��ͳ�����ݣ�ʵʱ���ݣ�
	//(����)��ֵ����ֵ����ʱ�䡢��ֵ�ۼ�ʱ��
	u8 u8DataE100001B[9];       
	//(����)��ֵ����ֵ����ʱ�䡢��ֵ�ۼ�ʱ��			
	u8 u8DataE100401B[9];     
    
	//������������ͳ�����ݣ�ʵʱ���ݣ�	
	//(����)����1~����10���ۼ�ʱ��
	u8 u8DataE1000033[33];     
	//(����)����1~����10���ۼ�ʱ��		
	u8 u8DataE1004033[33];        
	//�������״̬��
	u8 u8Data040005FF[18];
	//�ն�״̬��
	u8 u8Terminal040005FF[18];
	u8 u8DataTerminalStateWord[2]; //�ն�״̬��
}QCSG_LCD_RAMDATA_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_LCD_RAMDATA_S stPnLcdRamData[MAX_PN_NUM2];
}QCSG_LCD_RAMDATA_LIST_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ENERGY_S stPn_Ram_Data[MAX_PN_NUM2];
}QCSG_LCD_DAY_ENERGY_LIST_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	QCSG_ENERGY_S stPn_Ram_Data[MAX_PN_NUM2];
}QCSG_LCD_MONTH_ENERGY_LIST_S;
typedef struct
{
	u8 u8PnNum;
	u8 u8Res1;
	u8 u8Res2;
	u8 u8Res3;
	u16 u16PnList[MAX_PN_NUM2];
}QCSG_FROZEN_PN_LIST_S;
typedef struct
{
	QCSG_FROZEN_PN_LIST_S st_TD_D_PnList;
	QCSG_FROZEN_PN_LIST_S st_TD_M_PnList;
}QCSG_RECORD_FROZEN_PN_LIST_S;


typedef struct {
	u32 EdmiCmd;
	//u8   EdmiMethod;
}stEdmi;


typedef struct {

  u32 NwCmd;
  stEdmi Edmi_info[8];	//8����MAX_EDMI_INFO_NUM����
  u32  u32FixMethod;                                       //��ʽת������
  
}stNwEdmi;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __QCSG_PUBLICDEFINE_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
