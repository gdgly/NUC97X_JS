
//��2���ݶ��Ỻ��
#ifndef AFN0D_Congeal_Buff_H
#define AFN0D_Congeal_Buff_H



//��2ԭʼ����
#define ADDR_AFN0DF75_SOUR     ADDR_AFNOD_SOUR_Start//���߶����ܼ��й��ܵ���ԭʼ
//�ܼ��й��ܵ���ԭʼ,��ʼ��ֵ0xee,���ն�������AFN0CF19ʱͬʱ��0
#define ADDR_AFN0DF76_SOUR     ADDR_AFN0DF75_SOUR+(4*AFN0DF75MaxPn)//���߶����ܼ��޹��ܵ���ԭʼ
//�ܼ��޹��ܵ���ԭʼ,��ʼ��ֵ0xee,���ն�������AFN0CF20ʱͬʱ��0
#define ADDR_AFN0DF97_SOUR     ADDR_AFN0DF76_SOUR+(4*AFN0DF76MaxPn)//���߶��������й��ܵ�����ԭʼ
//�����й��ܵ���ԭʼ,��ʼ��ֵ0xee
#define ADDR_AFN0DF98_SOUR     ADDR_AFN0DF97_SOUR+(5*AFN0DF97MaxPn)//���߶��������޹��ܵ�����ԭʼ
//�����޹��ܵ���ԭʼ,��ʼ��ֵ0xee
#define ADDR_AFN0DF99_SOUR     ADDR_AFN0DF98_SOUR+(4*AFN0DF98MaxPn)//���߶��ᷴ���й��ܵ���ԭʼ
//�����й��ܵ���ԭʼ,��ʼ��ֵ0xee
#define ADDR_AFN0DF100_SOUR     ADDR_AFN0DF99_SOUR+(5*AFN0DF99MaxPn)//���߶��ᷴ���޹��ܵ���ԭʼ
//�����޹��ܵ���ԭʼ,��ʼ��ֵ0xee
#define ADDR_AFNOD_SOUR_End     (ADDR_AFN0DF100_SOUR+(4*AFN0DF100MaxPn))//






#define ADDR_AFN0D_Congeal_Buff_Start     ADDR_AFNOD_SOUR_End//��2���ݶ��Ỻ��
//ѡ��2�������������
#if ((LEN_DATECONGEAL>=LEN_RMDATECONGEAL)&&(LEN_DATECONGEAL>=LEN_MONTHCONGEAL)&&(LEN_DATECONGEAL>=LEN_CURVECONGEAL))
#define ADDR_AFN0D_Congeal_Buff_End     (ADDR_AFN0D_Congeal_Buff_Start+LEN_DATECONGEAL)
#else
 #if ((LEN_RMDATECONGEAL>=LEN_DATECONGEAL)&&(LEN_RMDATECONGEAL>=LEN_MONTHCONGEAL)&&(LEN_RMDATECONGEAL>=LEN_CURVECONGEAL))
#define ADDR_AFN0D_Congeal_Buff_End     (ADDR_AFN0D_Congeal_Buff_Start+LEN_RMDATECONGEAL)
 #else
  #if ((LEN_MONTHCONGEAL>=LEN_DATECONGEAL)&&(LEN_MONTHCONGEAL>=LEN_RMDATECONGEAL)&&(LEN_MONTHCONGEAL>=LEN_CURVECONGEAL))
#define ADDR_AFN0D_Congeal_Buff_End     (ADDR_AFN0D_Congeal_Buff_Start+LEN_MONTHCONGEAL)
  #else
#define ADDR_AFN0D_Congeal_Buff_End     (ADDR_AFN0D_Congeal_Buff_Start+LEN_CURVECONGEAL)
  #endif
 #endif
#endif

#define LEN_AFN0D_Congeal_Buff     ((ADDR_AFN0D_Congeal_Buff_End)-(ADDR_AFN0D_Congeal_Buff_Start))

#endif

















