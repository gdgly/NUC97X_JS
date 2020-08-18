
//���г�����2���������¶���ԭʼ����(��ʼֵΪ����ֵ0xFF)

#ifndef AFN56_Source_H
#define AFN56_Source_H

#define	ADDR_AFN56_Source_Date_Start     ADDR_AFN56_Source_Start
#define  LEN_AFN56_Source_Date	(3*1024*1024)//3M

//�¼��洢��
#define ADDR_AFN56_Event_Date_Start			ADDR_AFN56_Source_Date_Start
#define LEN_AFN56_Event_Date	(512*1024)
#define PER_EVENTSAVE_SPACE 256
#define MAX_EVENTSAVE_NUM 1024
//4K���� = 1024*4   (ÿ4�ֽڴ���һ���¼�,D24-D31:0x55  ���ϱ���־    D0-D15  �¼����0-1023    D16-D23:0��0xFF��д��־
//ÿ���¼�д��ʱ,����һ�¼���д��־����
//256K = 256 * 1024
#define ADDR_AFN56_Event_Date_End				(ADDR_AFN56_Event_Date_Start+LEN_AFN56_Event_Date)

//������
#define ADDR_AFN56_Index_Start     ((ADDR_AFN56_Event_Date_End+0x1ffff)&0xfffe0000)  //��ʼ��ַ128K����
#define  LEN_AFN56_Index	((2*1024+512)*1024)//3M

//��������������
#define  LEN_AFN56_DataSource	(90*1024*1024)//90M
//ÿƬ������ӦΪ256+32=288��  �ֳ�ʵ������ʱ����һ֡���ݸպ�������128k�洢��ʱ��������β����������û��д�����������޸�Ϊ256
#define  LEN_AFN56_Data_Per		288//ÿƬ������:32byte Info + 256byte Data  ����90*1024*1024 / 288Ƭ����
//#define  LEN_AFN56_Data_Per		256//ÿƬ������:32byte Info + 224byte Data  ����90*1024*1024 / 256Ƭ����
#if LEN_AFN56_Data_Per>=288
  #define  LEN_AFN56_Data_Length		288//ÿƬ������:32byte Info + 224byte Data  ����90*1024*1024 / 256Ƭ����
#else
  #define  LEN_AFN56_Data_Length		LEN_AFN56_Data_Per//ÿƬ������:32byte Info + 224byte Data  ����90*1024*1024 / 256Ƭ����
#endif
//#define MAX_AFN56DATA_SUN		327680// 90*1024*1024 / 288 = 327680 = 320K byte
#define MAX_AFN56DATA_SUN		(LEN_AFN56_DataSource / LEN_AFN56_Data_Per) // 90*1024*1024 / 288 = 327680 = 320K byte
typedef __packed struct
{
	u16 TaskID;//����ţ�Ψһ��ʶ1-65535
	u32 Index;//����ռ���ʼ��� 4byte  1-327680
	u8  Depth;//�洢���				 1byte
	u8  Res[8];//����
	u8  Check;//У��						 1byte
}FuJSubTaskHead_TypeDef;//������������������Ϣ�ṹ
#define LEN_SubTaskHead_Per 16//ÿ������������Ϣ�ṹ����Ϊ16byte

#define  ADDR_AFN56_DataTable_Start		((ADDR_AFN56_Index_Start+LEN_SubTaskHead_Per*65536+0x1ffff)&0xfffe0000)//������������65536 �����1-65536    //��ʼ��ַ128k����
//#define  LEN_AFN56_DataTable	40960//327680 / 8   ;1bit = 288byte  + 2byte check
#define  LEN_AFN56_DataTable	(MAX_AFN56DATA_SUN / 8)//40960//327680 / 8   ;1bit = 288byte  + 2byte check
#define  LEN_AFN56_Table	(LEN_AFN56_DataTable+2)//(327680 / 8 )+2  ;1bit = 288byte  + 2byte check
#if (LEN_AFN56_Table > (128*1024))
 #error "GetIdleSpace(u32 TaskID,u32 Size)--MR(ADDR_128KDATABUFF,ADDR_AFN56_DataTable_Start,LEN_AFN56_Table)"
#endif
#define  ADDR_AFN56_DataTable_End		((ADDR_AFN56_DataTable_Start+LEN_AFN56_Table+0x1ffff)&0xfffe0000) //������ַ128K����

#if (ADDR_AFN56_DataTable_End-ADDR_AFN56_Index_Start)>LEN_AFN56_Index
	#error "(ADDR_AFN56_DataTable_End-ADDR_AFN56_Index_Start)>LEN_AFN56_Index"
#endif

#define  ADDR_AFN56_Index_End				((ADDR_AFN56_Index_Start+LEN_AFN56_Index+0x1ffff)&0xfffe0000) //������ַ128K����

//������
#define  ADDR_AFN56_DataSource_Start		((ADDR_AFN56_Index_End+0x1ffff)&0xfffe0000) //��ʼ��ַ128K����
//#define  LEN_AFN56_DataSource	(90*1024*1024)//90M

typedef __packed struct
{
	u16 SubTaskID;//����ţ�Ψһ��ʶ1-65535
	u8  STime[6];//���ʱ��
	u8  RTime[6];//����ʱ��
	u8  MeterAddr[6];//ͨ�ŵ�ַ
	u8  Len;//���ݳ���
	u32 Round;//�ִ�
	u8  ReportFlag;//�����ϱ�״̬
	u8  ReadTimes;//��������
	u8  res1;//RetryTimes;//���Դ���--��N������
	u8  ReTimes;//��ǰ���ڳ����ܴ���
	u8	Res[2];//����
	u8  Check;//У��						 1byte
}FuJSubTaskDataHead_TypeDef;//������������������Ϣ�ṹ

#define LEN_FuJSubTaskDataHead	32



#define  ADDR_AFN56_DataSource_End			((ADDR_AFN56_DataSource_Start+LEN_AFN56_DataSource+0x1ffff)&0xfffe0000) //������ַ128K����


#define ADDR_AFN56_Source_End     ADDR_AFN56_DataSource_End
#if ((ADDR_AFN56_Source_End-ADDR_AFN56_Source_Start)>LEN_Terminal_EFLASH_AFN56)
	#error "((ADDR_AFN56_Source_End-ADDR_AFN56_Source_Start)>LEN_Terminal_EFLASH_AFN56)"
#endif



#endif
