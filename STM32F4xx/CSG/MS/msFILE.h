

#ifndef msFILE_H
#define msFILE_H
#ifdef __cplusplus
 extern "C" {
#endif
	 

#define FILENAMEmax_FLASH     ((LEN_msFILEmanage-LEN_msFILEunltFlags-12)/sizeof(msFILE_TypeDef))//�洢��FLASH�е��ļ�������ļ���
#define FILENAMEmax_RAM     (LEN_RAMmsFILEmanage/sizeof(msFILE_TypeDef))//�洢��RAM�е��ļ�������ļ���
#define recordFILENAMEmax_FLASH     (FILENAMEmax_FLASH-0)//�޶����FLASH�����¼���ļ���
#define recordFILENAMEmax_RAM     (FILENAMEmax_RAM-0)//�޶����RAM�����¼���ļ���
//�洢��FLASH�е��ļ���:0��(FILENAMEmax_FLASH-1)
//�洢��RAM�е��ļ���:FILENAMEmax_FLASH��(FILENAMEmax_FLASH+FILENAMEmax_RAM-1)

void msfile_init(void);//msfile��ʼ��
void msfile_format(void);//msfile��ʽ��
u32 msfile_free(void);//msfile����nand FLASH�ļ����ɿռ�,Ҫ��RAM�ļ����ɿռ��ͬu32 ms_malloc_sizemax(void);//���ɷ���ĳߴ�(�ֽ���)
u32 msfile_write(u32 filename,u32 offset,u8 *buf,u32 byte);//msfileд;����:д���ֽ���
u32 msfile_read(u32 filename,u32 offset,u8 *buf,u32 byte);//msfile��;����:�����ֽ���
u32 msfile_Delete(u32 filename);//msfileɾ��;����:0=ʧ��,1=�ɹ�
u32 msfile_len(u32 filename);//msfile�õ��ļ��ܳ���
u32 msfile_set_lenmin(u32 filename,u32 lenmin);//msfile����Ԥ����С�ļ����Ƚ�nand flash(ԭ�ļ���Ч�����ܳ��ȼ����ݲ���ֻԤռ����С�洢�ռ�);����:0=ʧ��,1=�ɹ�
u32 msfile_check(u32 filename);//msfile�ļ����;����:0=��,1=��Ч�ļ�
u32 msfile_rename(u32 filename,u32 newfilename);//�ļ���������;�����Զ�����:��ɾ��newfilename,Ȼ��������;����:0=����,1=�ɹ�

#if MainProtocol==698
void msfile_lenclear(u32 filename);//��0�ļ���صĴ���,0-(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)��ʾ�ļ���RAM
u32 msfile_getfilename(u32 FileID);//������������FileID���ļ���:����:0-(FILENAMEmax_FLASH+FILENAMEmax_RAM-1),0xffffffff��ʾû�ҵ�
#endif


#ifdef __cplusplus
}
#endif
#endif

