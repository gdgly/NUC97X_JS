#ifndef CALC_TAG_H
#define CALC_TAG_H
#ifdef __cplusplus
 extern "C" {
#endif


void Init_Data230x_D(void);//��ʼ���ܼ���������
void Init_Data230x_M(void);//��ʼ���ܼ���������
void Init_Data230x(void);//��ʼ���ܼ�������
u32 Get_CT_PT(u16 * pAddr);//��ȡTSA��Ӧ��CT,PTֵ��CT�Ÿ�2�ֽڣ�PT�ŵ�2�ֽڣ�ֵ�Ѿ��ߵ�λ�������ˣ�����ֱ��ʹ�á�
u32 Is_TSA_Same(u8 *pAddr1,u8 *pAddr2);//�жϵ�ַ�Ƿ���ͬ,���� 0=��ͬ 1=����ͬ
u32 Is_FnDataBuff_Have_TAGData(u32 oad,u8* pbuf);//���� 0=������oad�����ݣ�!=0��oad��Ӧ�����ݵ�ַ
void Do_Update_ADDR_230x_Sour(u16 * pAddr,u32 oad_data_addr,u32 data_dest,u32 TAG_NO,u32 add_mise,u32 type,u32 group_type,u32 pn_type);//pAddr1��TSA��data_sour��01 05 06 XXXX 06 XXXX,data_dest 0X14 XXXXXXXX
void Update_ADDR_230x_Sour(u16 * pAddr,u32 oad_data_addr,u32 TAG_NO,u32 add_mise,u32 type,u32 group_type,u32 pn_type);//�����ݸ��µ�ADDR_230x_Sour��֮ǰû�о��¼�
void Update_ADDR_230x_x(u32 addr_data,u32 LENper,u64 value,u32 TAG_NO,u32 add_mise,u32 fee);//�����ܼӵ���
void Update_ADDR_230x_11(u64 value,u32 TAG_NO,u32 fee);//�����ܼ�ʣ��������ѣ�
void Update_Group_Total_Data_6000(u8 *FnDataBuff);//�����ܼ��������,���ñ���6000���
void Update_Group_Total_Data_240x(void);//�����ܼ��������,���ñ���������

void Calc_TAG_Power(void);//����ȫ���ܼ���Ĺ���
void Calc_PerTAG_Power(u32 TAG_NO,u32 Power_type);//���㵥һ�ܼӵĹ��ʣ�TAG_NO=�ܼ����� 1--8��Power_type=�������ͣ�0=�й����� 1=�޹�����
u64 Get_6000_Power(u32 pn,u32 Power_type);//��ȡ������Ĺ���
u64 Get_TSA_BY_PN_noInverse(u32 Pn);//����������ţ��õ�TSA���������������͡�������˳��
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif

