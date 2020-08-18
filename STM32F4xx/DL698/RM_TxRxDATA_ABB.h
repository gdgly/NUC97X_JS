#ifndef RM_TxRxDATA_ABB_H
#define RM_TxRxDATA_ABB_H
#ifdef __cplusplus
extern "C" {
#endif

	
u32 ABB_CRC(u8* p8, u32 nByte); //ABB��CRC����
	
#define  ABB_SUB_OFFSET     LEN_UART2Rx+LEN_UART2Tx //ABB��֡��Ӧ��ʼ����ڽ��ջ���ĵ�ƫ��
#define  ABB_RM_LINK        0                       //����ABBԤ���ӵı�ʶ�����������Ǳߵĳ���Ķ�
#define  ABB_RM_CLASS11     ABB_RM_LINK+2           //��ʱû��
#define  ABB_KEY            ABB_RM_CLASS11+2        //�洢ABB��Զ��������Կ
#define  ABB_POWER_POINT    ABB_KEY+4               //����С��λ
#define  ABB_DEMAND_POINT   ABB_POWER_POINT+1       //����С��λ
#define  ABB_CLASS11_BLOCK1 ABB_DEMAND_POINT+1      //��11�����ݺ���
#define  ABB_CLASS11_BLOCK2 ABB_CLASS11_BLOCK1+1    //
#define  ABB_CLASS11_BLOCK3 ABB_CLASS11_BLOCK2+1    //
#define  ABB_CLASS11_BLOCK4 ABB_CLASS11_BLOCK3+1    //
#define  ABB_LEN_CLASS9     ABB_CLASS11_BLOCK4+1    //����9���ݳ���
#define  ABB_LEN_CLASS11    ABB_LEN_CLASS9+2        //����11���ݳ���

#define  ABB_LENMAX_PARA    32                      //ǰ���ABB�������̲����ͳ������ݵ���󳤶�
#define  ABB_LENMAX_RX_CLASS9    128                //����ABB����������9����󳤶�
#define  ABB_LENMAX_RX_CLASS11   1024               //�������ABB����������11����󳤶ȣ�
//1.ʵ���֡�ò���1024��2.Ԥ���Ľ��ջ���ĳ��Ȳ�ֹ1024
#define  ABB_LENMAX_POWER_CLASS11   0x016E
#if(ABB_LEN_CLASS11-ABB_RM_LINK > ABB_LENMAX_PARA)
#error "ABB�������̲������ȴ�����������ֵ"
#endif

#define  ABB_RM_CLASS9_OFFSET      ABB_RM_LINK+ABB_LENMAX_PARA                  //����9����
#define  ABB_RM_CLASS11_OFFSET     ABB_RM_CLASS9_OFFSET+ABB_LENMAX_RX_CLASS9      //����11����

#if(ABB_RM_CLASS11_OFFSET+ABB_LENMAX_RX_CLASS11-ABB_RM_LINK > LEN_UART2Rx)
#error "ABB�������ݳ��ȴ�����������ֵ"
#endif


#ifdef __cplusplus
}
#endif
#endif
