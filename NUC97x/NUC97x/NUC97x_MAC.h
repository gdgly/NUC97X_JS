

#ifndef NUC97x_MAC_H
#define NUC97x_MAC_H
#ifdef __cplusplus
 extern "C" {
#endif

void UnInit_NUC97x_MAC0(void);
void Init_NUC97x_MAC0(void);
void UnInit_NUC97x_MAC1(void);
void Init_NUC97x_MAC1(void);
void UnInit_NUC97x_MAC(void);
void Init_NUC97x_MAC(void);
	 
void ETH0_MAC_Config(void);//MAC0����,��LINK������,��ȡPYH״̬����Ӧֵ����
void ETH1_MAC_Config(void);//MAC1����,��LINK������,��ȡPYH״̬����Ӧֵ���� 
void ETH_MAC_Config(void);//MAC����,��LINK������,��ȡPYH״̬����Ӧֵ���� 

	 
void ETH_DMA_Config(u32 DMATDLA,u32 DMARDLA);//DMA����;���:DMATDLA=�����������б��ַ,DMARDLA=�����������б��ַ
void ETH_DMATxDescList(u32 DMATDLA,u32 ADDR_TxBuff1,u32 ADDR_TxBuff2,u32 Byte,u32 FS,u32 LS);//ETH��������������������
	//���:DMATDLA=�����������б��ַ,TxBuff1=���ͻ�������ַ,TxBuff2=����������ַ,Byte=�����ֽ�,FS=1(�׶�),LS=1(ĩ��)
void ETH_DMARxDescList(u32 DMARDLA,u32 ADDR_RxBuff1,u32 ADDR_RxBuff2,u32 LEN_Buff,u32 FS,u32 LS);//ETH��������������������
	//���:DMARDLA=�����������б��ַ,RxBuff1=���ջ�������ַ,RxBuff2=����������ַ,LEN_Buff=���ջ���������,FS=1(�׶�),LS=1(ĩ��)


#ifdef __cplusplus
 }
#endif
#endif




