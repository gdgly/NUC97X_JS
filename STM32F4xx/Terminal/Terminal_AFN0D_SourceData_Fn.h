

#ifndef Terminal_AFN0D_SourceData_Fn_H
#define Terminal_AFN0D_SourceData_Fn_H



typedef __packed struct
{
	vu8 Fn;//��2����Fn
  vu32 SourAddr;//ԭʼ���ݲ�����1��ַ,��Ϊ0ʱ��ʾ��Ч
	vu16 Len;//ԭʼ����ÿPn����
}Terminal_Class2SourceDataLib_TypeDef;


u32 GetClass2SourceDataAddr(u32 Fn,u32 Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
void SaveClass2SourceData(u32 Fn,u32 Pn,u32 ADDR_Buff);//������2�ز�����ԭʼ����
void SaveAddDateSourceData(u32 Fn,u32 Pn,u32 ADDR_Buff);//������2���г�������2-3�յ���ʾֵԭʼ����;���:Fn=0xee��2��,=0xef��3��
void SaveAddDataToMonth(u32 Fn,u32 Pn,u32 FnDataAddr,u32 OfficeAddr);//���������³�1������תΪ�¶���


#endif


