

#ifndef FLASH_DL698_DATA_H
#define FLASH_DL698_DATA_H
#ifdef __cplusplus
 extern "C" {
#endif


//����
	#if CompareMeterEventCount==1//ȫ�¼��ɼ������Ƿ�ȽϷ�������:0=���Ƚ�(ȫ����������д���ļ�),1=�Ƚ�(ֻ�д����仯���¼�д���ļ�)
#define ADDR_SourMeterEventCount     ADDR_FLASH_DL698_DATA_START//ȫ�¼��ɼ�������ܱ�ԭ�¼���������(��������˳������,��ʼ��ֵȫ��Ϊ0)
//��Ӧ�������ú�0
//6byte ���ܱ�ͨ�ŵ�ַ(��ͨ�ŵ�ַ�仯ʱ��0�б����)
//2byte �б����
//4byte �¼�OAD
//4byte ԭ�¼���������
//...
//��Ӧ�������ú�1
//6byte ���ܱ�ͨ�ŵ�ַ(��ͨ�ŵ�ַ�仯ʱ��0�б����)
//2byte �б����
//4byte �¼�OAD
//4byte ԭ�¼���������
//...

//...
//��Ӧ�������ú�n
//6byte ���ܱ�ͨ�ŵ�ַ(��ͨ�ŵ�ַ�仯ʱ��0�б����)
//2byte �б����
//4byte �¼�OAD
//4byte ԭ�¼���������
//...
#define LENper_SourMeterEventCount     512//���ٴ���8
#define ADDR_FLASH_DL698_DATA_END1     ADDR_SourMeterEventCount+(LENper_SourMeterEventCount*NUM_RMmax)
	#endif

	#ifndef ADDR_FLASH_DL698_DATA_END1
#define ADDR_FLASH_DL698_DATA_END1     ADDR_FLASH_DL698_DATA_START
	#endif


#if (USER/100)==15//����
#define ADDR_2131_All    		ADDR_FLASH_DL698_DATA_END1//�洢ÿ���յ�ABC���ѹ�ϸ��ʴ洢������,����һ����31�������
#define LENmax_2131_All     (48*NUM_RMmax)
#define ADDR_FLASH_DL698_DATA_END     ADDR_2131_All+(LENmax_2131_All*31*3)
#endif




#ifndef ADDR_FLASH_DL698_DATA_END
#define ADDR_FLASH_DL698_DATA_END     ADDR_FLASH_DL698_DATA_END1
#endif



#if ((ADDR_FLASH_DL698_DATA_END)-(ADDR_FLASH_DL698_DATA_START))>LEN_FLASH_DL698_DATA
	#error LEN_FLASH_DL698_DATA
#endif
	 
	 

#ifdef __cplusplus
}
#endif
#endif


