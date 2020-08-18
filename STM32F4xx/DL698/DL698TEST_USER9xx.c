

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698TEST.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/RM_Record.h"
#include "../MS/MS.h"
#include "../Terminal698/Terminal_Init.h"




#if RUN_DL698TEST_USER==1//0��Ч,1=����;�û������
#if (USER/100)==9//���ϰ�

u32 CALC_FLASH_NUM_File(void)//ͳ��FLASH���ļ���
{
	u32 i;
	u32 filename;
	msFILE_TypeDef *file;
	
	i=0;
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			i++;
		}
		file++;
	}
	return i;
}

u32 CALC_SDRAM_NUM_File(void)//ͳ��SDRAM���ļ���
{
	u32 i;
	u32 filename;
	msFILE_TypeDef *file;
	
	i=0;
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=recordFILENAMEmax_FLASH;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			i++;
		}
		file++;
	}
	return i;
}

u32 CALC_NUM_File(void)//ͳ�����ļ���
{
	u32 i;
	u32 filename;
	msFILE_TypeDef *file;
	
	i=0;
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			i++;
		}
		file++;
	}
	return i;
}


const u8 RM_RecordFile_CODE[]=
{
//UART�������ݻ��嶨��
//#define LEN_RM_DI_CSD     512
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
	1,0,0,0,0,0,
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
	0,0,
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
	0x07,0xE0,4,18,0,0,0,
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
	0x07,0xE0,4,18,0,0,0,
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
	0x07,0xE0,4,18,0,0,0,
//29  �ɼ���CSD  array CSD(����������)
	DataType_array,//array
	1,//��
	DataType_CSD,
	1,//ROAD
	0x50,0x04,0x02,0x00,//�ն���
	2,//������������������  SEQUENCE OF����=2
	0x00,0x10,0x02,0x00,//�����й��ܼ�����
	0x00,0x20,0x02,0x00,//�����й��ܼ�����
	
//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������)
	0x1,0x5,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,
	0x1,0x5,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,
};

u32 DL698TEST_USER9xx_Init_1(void)//���ϰ����_�����¼�ļ�
{
	u32 i;
	u8* p8rx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 PORTn;
	ms_Type *ms;
	u32 NUM_File;
	
	ms=Get_ms();
	ms->msfile_format();
	PORTn=RS485_1PORT;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	MR((u32)p8rx,(u32)RM_RecordFile_CODE,sizeof(RM_RecordFile_CODE));
	i=sizeof(RM_RecordFile_CODE);
	i-=8;
	MWR(i,(u32)p8rx+6,2);//����
	
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
	
//ʵʱ�ɼ�����
	//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//b27-b24��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//b23-b20��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�
//b19-b16��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��,7=���������ĩ23��59��
	UARTCtrl->TaskID=0x01011515;
	MWR(0x00000001,(u32)p8rx,6);//���ܱ��ַ
	Data_Inverse((u32)p8rx,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	MWR(0x20180310010203,(u32)p8rx+22,7);//�ɼ��洢ʱ��
	Data_Inverse((u32)p8rx+22,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	RM_RecordFile(PORTn,p8rx);//�����¼
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
	//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	UARTCtrl->TaskID=0x01011515;
	MWR(0x00000002,(u32)p8rx,6);//���ܱ��ַ
	Data_Inverse((u32)p8rx,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	MWR(0x20180310010000,(u32)p8rx+22,7);//�ɼ��洢ʱ��
	Data_Inverse((u32)p8rx+22,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	RM_RecordFile(PORTn,p8rx);//�����¼
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
//ͳ�����ļ���=2

//�ղɼ�����
	//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//b27-b24��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//b23-b20��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�
//b19-b16��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��,7=���������ĩ23��59��
	UARTCtrl->TaskID=0x01231616;
	MWR(0x00000001,(u32)p8rx,6);//���ܱ��ַ
	Data_Inverse((u32)p8rx,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//�ɼ��洢ʱ��
	Data_Inverse((u32)p8rx+22,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	RM_RecordFile(PORTn,p8rx);//�����¼
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
	//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	UARTCtrl->TaskID=0x01231616;
	MWR(0x00000002,(u32)p8rx,6);//���ܱ��ַ
	Data_Inverse((u32)p8rx,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//�ɼ��洢ʱ��
	Data_Inverse((u32)p8rx+22,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	RM_RecordFile(PORTn,p8rx);//�����¼
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
//ͳ�����ļ���=3

//�²ɼ�����
	//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//b27-b24��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//b23-b20��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�
//b19-b16��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��,7=���������ĩ23��59��,7=���������ĩ23��59��
	UARTCtrl->TaskID=0x01271717;
	MWR(0x00000001,(u32)p8rx,6);//���ܱ��ַ
	Data_Inverse((u32)p8rx,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	MWR(0x20180310010000,(u32)p8rx+22,7);//�ɼ��洢ʱ��
	Data_Inverse((u32)p8rx+22,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	RM_RecordFile(PORTn,p8rx);//�����¼
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
//ͳ�����ļ���=4
	
//15�������߲ɼ�����
	//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	UARTCtrl->TaskID=0x01361818;
	MWR(0x00000001,(u32)p8rx,6);//���ܱ��ַ
	Data_Inverse((u32)p8rx,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//�ɼ��洢ʱ��
	Data_Inverse((u32)p8rx+22,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	RM_RecordFile(PORTn,p8rx);//�����¼
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
	UARTCtrl->TaskID=0x01361818;
	MWR(0x00000002,(u32)p8rx,6);//���ܱ��ַ
	Data_Inverse((u32)p8rx,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//�ɼ��洢ʱ��
	Data_Inverse((u32)p8rx+22,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	RM_RecordFile(PORTn,p8rx);//�����¼
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
//ͳ�����ļ���=5
	
//�¼��ɼ�����	
	//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//b27-b24��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//b23-b20��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�
//b19-b16��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��,7=���������ĩ23��59��
	UARTCtrl->TaskID=0x02000101;
	MWR(0x00000001,(u32)p8rx,6);//���ܱ��ַ
	Data_Inverse((u32)p8rx,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	MWR(0x20180310020000,(u32)p8rx+22,7);//�ɼ��洢ʱ��
	Data_Inverse((u32)p8rx+22,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
	RM_RecordFile(PORTn,p8rx);//�����¼
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
//ͳ�����ļ���=6
	
	return NUM_File;
}


const u8 InitData_6000_2_TEST_1[]=//�ɼ���������
{
//����2��Acquisition document definition����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	4,
//{
//�������  long-unsigned��
	DataType_long_unsigned,
	0,1,
//������Ϣ  Basic_object��
	
//��չ��Ϣ  Extended_object��
	
//������Ϣ  Annex_object
//}

//Basic_object��=structure
	DataType_structure,
	10,
//{
//ͨ�ŵ�ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x01,
//������    enum
//{
//300bps��0����  600bps��1����    1200bps��2����
//2400bps��3���� 4800bps��4����   7200bps��5����
//9600bps��6���� 19200bps��7����  38400bps��8����
//57600bps��9����115200bps��10��������Ӧ��255��
//}��
	DataType_enum,
	255,
//��Լ����  enum
//{
//  δ֪ (0)��
//DL/T645-1997��1����
//DL/T645-2007��2����
//DL/T698.45��3����
//CJ/T 188-2004��4��
//}��
	DataType_enum,
	2,
//�˿�      OAD��
	DataType_OAD,
	0xF2,0x08,0x02,0x01, //��������ͨ�ſں�(����)
//ͨ������  octet-string��
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//���ʸ���  unsigned��
	DataType_unsigned,
	4,
//�û�����  unsigned��
	DataType_unsigned,
	21,//C��
//���߷�ʽ  enum
//{
//   δ֪��0����
//   ���ࣨ1����
//   �������ߣ�2����
//   �������ߣ�3��
//}
	DataType_enum,
	3,
//���ѹ    long-unsigned(����-1����λV),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//�����    long-unsigned(����-1����λA)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object��=structure
	DataType_structure,
	4,
//{
//�ɼ�����ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//�ʲ���      octet-string��
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned��
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object��=array structure
	DataType_array,
	0,
//{
//������������  OAD��
//����ֵ        Data
//}
};
const u8 InitData_6000_2_TEST_2[]=//�ɼ���������
{
//����2��Acquisition document definition����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	4,
//{
//�������  long-unsigned��
	DataType_long_unsigned,
	0,1,
//������Ϣ  Basic_object��
	
//��չ��Ϣ  Extended_object��
	
//������Ϣ  Annex_object
//}

//Basic_object��=structure
	DataType_structure,
	10,
//{
//ͨ�ŵ�ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x02,
//������    enum
//{
//300bps��0����  600bps��1����    1200bps��2����
//2400bps��3���� 4800bps��4����   7200bps��5����
//9600bps��6���� 19200bps��7����  38400bps��8����
//57600bps��9����115200bps��10��������Ӧ��255��
//}��
	DataType_enum,
	255,
//��Լ����  enum
//{
//  δ֪ (0)��
//DL/T645-1997��1����
//DL/T645-2007��2����
//DL/T698.45��3����
//CJ/T 188-2004��4��
//}��
	DataType_enum,
	2,
//�˿�      OAD��
	DataType_OAD,
	0xF2,0x08,0x02,0x01, //��������ͨ�ſں�(����)
//ͨ������  octet-string��
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//���ʸ���  unsigned��
	DataType_unsigned,
	4,
//�û�����  unsigned��
	DataType_unsigned,
	51,//F��
//���߷�ʽ  enum
//{
//   δ֪��0����
//   ���ࣨ1����
//   �������ߣ�2����
//   �������ߣ�3��
//}
	DataType_enum,
	3,
//���ѹ    long-unsigned(����-1����λV),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//�����    long-unsigned(����-1����λA)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object��=structure
	DataType_structure,
	4,
//{
//�ɼ�����ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//�ʲ���      octet-string��
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned��
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object��=array structure
	DataType_array,
	0,
//{
//������������  OAD��
//����ֵ        Data
//}
};

void DL698TEST_USER9xx_Init_2(void)//���ϰ����_���ò�ͬ�û�����
{
//RSDѡ��10 MS=ȫ����(���ϰ���Բ�ͬ�û���������ж���֡)
//68 34 00 43 05 01 00 00 00 00 00 10 1f ea 05 03 04 60 12 03 00 0a 01 01 05 00 40 01 02 00 00 60 40 02 00 00 60 41 02 00 00 60 42 02 00 00 00 10 02 00 00 52 d3 16

	ms_Type *ms;
	
	ms=Get_ms();
	ms->msfile_format();
	
	//��ʼ������
	DL698_INIT(3);//��ʼ��,���Command:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��

	//�嵵��
	MC(0,ADDR_6000,LENper_6000*NUMmax_6000);
	//��ʼ������
	MW((u32)InitData_6000_2_TEST_1,ADDR_6000+(1*LENper_6000),LENper_6000);
	MW((u32)InitData_6000_2_TEST_2,ADDR_6000+(2*LENper_6000),LENper_6000);
	
	FLASHtoSDRAM();//����FLASHtoSDRAM
}



void DL698TEST_Init_USER(void)//�û�����Գ�̨��
{
//	DL698TEST_USER9xx_Init_1();//���ϰ����_�����¼�ļ�
	DL698TEST_USER9xx_Init_2();//���ϰ����_��ͬ�û���������ж�
	
}

void DL698TEST_USER(void)//�û������
{
	u32 FLASH_NUM_File;
	u32 SDRAM_NUM_File;
	u32 NUM_File;
	
	FLASH_NUM_File=CALC_FLASH_NUM_File();//ͳ��FLASH���ļ���
	SDRAM_NUM_File=CALC_SDRAM_NUM_File();//ͳ��SDRAM���ļ���
	NUM_File=CALC_NUM_File();//ͳ�����ļ���
	if(NUM_File!=(FLASH_NUM_File+SDRAM_NUM_File))
	{
		NUM_File=NUM_File;
	}
	NUM_File=recordFILENAMEmax_RAM;
	if(NUM_File)
	{
		NUM_File=NUM_File;
	}
	
}

#endif
#endif













