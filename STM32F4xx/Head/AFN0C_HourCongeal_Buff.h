
//Сʱ�������ݻ���
#ifndef AFN0C_HourCongeal_Buff_H
#define AFN0C_HourCongeal_Buff_H





//Сʱ����������ʱ��Td_h	��ͼ		2
#define ADDR_AFN0CF81_BUFF     ADDR_HOURCONGEAL_BUFF_START+2//Сʱ�����ܼ��й�����
//Сʱ�����ܼ��й�����1	����¼A.2	kW	2
//����	����	����	����
//Сʱ�����ܼ��й�����i	����¼A.2	kW	2
#define ADDR_AFN0CF82_BUFF     ADDR_AFN0CF81_BUFF+(2*MaxHourCongeal*AFN0CF81MaxPn)//Сʱ�����ܼ��޹�����
//Сʱ�����ܼ��޹�����1	����¼A.2	kvar	2
//����	����	����	����
//Сʱ�����ܼ��޹�����i	����¼A.2	kvar	2
#define ADDR_AFN0CF83_BUFF     ADDR_AFN0CF82_BUFF+(2*MaxHourCongeal*AFN0CF82MaxPn)//Сʱ�����ܼ��й��ܵ�����
//Сʱ�����ܼ��й��ܵ�����1	����¼A.3	kWh	4
//����	����	����	����
//Сʱ�����ܼ��й��ܵ�����i	����¼A.3	kWh	4
#define ADDR_AFN0CF84_BUFF     ADDR_AFN0CF83_BUFF+(4*MaxHourCongeal*AFN0CF83MaxPn)//Сʱ�����ܼ��޹��ܵ�����
//Сʱ�����ܼ��޹��ܵ�����1	����¼A.3	kvarh	4
//����	����	����	����
//Сʱ�����ܼ��޹��ܵ�����i	����¼A.3	kvarh	4
#define ADDR_AFN0CF89_BUFF     ADDR_AFN0CF84_BUFF+(4*MaxHourCongeal*AFN0CF84MaxPn)//Сʱ�����й�����
//Сʱ�����й�����1	����¼A.9	kW	3
//����	����	����	����
//Сʱ�����й�����i	����¼A.9	kW	3
#define ADDR_AFN0CF90_BUFF     ADDR_AFN0CF89_BUFF+(3*MaxHourCongeal*AFN0CF89MaxPn)//Сʱ����A���й�����
//Сʱ�����й�����1	����¼A.9	kW	3
//����	����	����	����
//Сʱ�����й�����i	����¼A.9	kW	3
#define ADDR_AFN0CF91_BUFF     ADDR_AFN0CF90_BUFF+(3*MaxHourCongeal*AFN0CF90MaxPn)//Сʱ����B���й�����
//Сʱ�����й�����1	����¼A.9	kW	3
//����	����	����	����
//Сʱ�����й�����i	����¼A.9	kW	3
#define ADDR_AFN0CF92_BUFF     ADDR_AFN0CF91_BUFF+(3*MaxHourCongeal*AFN0CF91MaxPn)//Сʱ����C���й�����
//Сʱ�����й�����1	����¼A.9	kW	3
//����	����	����	����
//Сʱ�����й�����i	����¼A.9	kW	3
#define ADDR_AFN0CF93_BUFF     ADDR_AFN0CF92_BUFF+(3*MaxHourCongeal*AFN0CF92MaxPn)//Сʱ�����޹�����
//Сʱ�����޹�����1	����¼A.9	kvar	3
//����	����	����	����
//Сʱ�����޹�����i	����¼A.9	kvar	3
#define ADDR_AFN0CF94_BUFF     ADDR_AFN0CF93_BUFF+(3*MaxHourCongeal*AFN0CF93MaxPn)//Сʱ����A�޹�����
//Сʱ�����޹�����1	����¼A.9	kvar	3
//����	����	����	����
//Сʱ�����޹�����i	����¼A.9	kvar	3
#define ADDR_AFN0CF95_BUFF     ADDR_AFN0CF94_BUFF+(3*MaxHourCongeal*AFN0CF94MaxPn)//Сʱ����B�޹�����
//Сʱ�����޹�����1	����¼A.9	kvar	3
//����	����	����	����
//Сʱ�����޹�����i	����¼A.9	kvar	3
#define ADDR_AFN0CF96_BUFF     ADDR_AFN0CF95_BUFF+(3*MaxHourCongeal*AFN0CF95MaxPn)//Сʱ����C�޹�����
//Сʱ�����޹�����1	����¼A.9	kvar	3
//����	����	����	����
//Сʱ�����޹�����i	����¼A.9	kvar	3
#define ADDR_AFN0CF97_BUFF     ADDR_AFN0CF96_BUFF+(3*MaxHourCongeal*AFN0CF96MaxPn)//Сʱ����A���ѹ
//Сʱ����A���ѹ1	����¼A.7	V	2
//����	����	����	����
//Сʱ����A���ѹi	����¼A.7	V	2
#define ADDR_AFN0CF98_BUFF     ADDR_AFN0CF97_BUFF+(2*MaxHourCongeal*AFN0CF97MaxPn)//Сʱ����B���ѹ
//Сʱ����B���ѹ1	����¼A.7	V	2
//����	����	����	����
//Сʱ����B���ѹi	����¼A.7	V	2
#define ADDR_AFN0CF99_BUFF     ADDR_AFN0CF98_BUFF+(2*MaxHourCongeal*AFN0CF98MaxPn)//Сʱ����C���ѹ
//Сʱ����C���ѹ1	����¼A.7	V	2
//����	����	����	����
//Сʱ����C���ѹi	����¼A.7	V	2
#define ADDR_AFN0CF100_BUFF     ADDR_AFN0CF99_BUFF+(2*MaxHourCongeal*AFN0CF99MaxPn)//Сʱ����A�����
//Сʱ����A�����1	����¼A.25	A	3
//����	����	����	����
//Сʱ����A�����i	����¼A.25	A	3
#define ADDR_AFN0CF101_BUFF     ADDR_AFN0CF100_BUFF+(3*MaxHourCongeal*AFN0CF100MaxPn)//Сʱ����B�����
//Сʱ����B�����1	����¼A.25	A	3
//����	����	����	����
//Сʱ����B�����i	����¼A.25	A	3
#define ADDR_AFN0CF102_BUFF     ADDR_AFN0CF101_BUFF+(3*MaxHourCongeal*AFN0CF101MaxPn)//Сʱ����C�����
//Сʱ����C�����1	����¼A.25	A	3
//����	����	����	����
//Сʱ����C�����i	����¼A.25	A	3
#define ADDR_AFN0CF103_BUFF     ADDR_AFN0CF102_BUFF+(3*MaxHourCongeal*AFN0CF102MaxPn)//Сʱ�����������
//Сʱ����N�����1	����¼A.25	A	3
//����	����	����	����
//Сʱ����N�����i	����¼A.25	A	3
#define ADDR_AFN0CF105_BUFF     ADDR_AFN0CF103_BUFF+(3*MaxHourCongeal*AFN0CF103MaxPn)//Сʱ���������й��ܵ�����
//Сʱ���������й��ܵ�����1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ�����i	����¼A.13	kWh	4
#define ADDR_AFN0CF106_BUFF     ADDR_AFN0CF105_BUFF+(4*MaxHourCongeal*AFN0CF105MaxPn)//Сʱ���������޹��ܵ�����
//Сʱ���������޹��ܵ�����1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������޹��ܵ�����i	����¼A.13	kWh	4
#define ADDR_AFN0CF107_BUFF     ADDR_AFN0CF106_BUFF+(4*MaxHourCongeal*AFN0CF106MaxPn)//Сʱ���ᷴ���й��ܵ�����
//Сʱ���ᷴ���й��ܵ�����1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���ᷴ���й��ܵ�����i	����¼A.13	kWh	4
#define ADDR_AFN0CF108_BUFF     ADDR_AFN0CF107_BUFF+(4*MaxHourCongeal*AFN0CF107MaxPn)//Сʱ���ᷴ���޹��ܵ�����
//Сʱ���ᷴ���޹��ܵ�����1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���ᷴ���޹��ܵ�����i	����¼A.13	kWh	4
#define ADDR_AFN0CF109_BUFF     ADDR_AFN0CF108_BUFF+(4*MaxHourCongeal*AFN0CF108MaxPn)//Сʱ���������й��ܵ���ʾֵ
//Сʱ���������й��ܵ���ʾֵ1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ���ʾֵi	����¼A.13	kWh	4
#define ADDR_AFN0CF110_BUFF     ADDR_AFN0CF109_BUFF+(4*MaxHourCongeal*AFN0CF109MaxPn)//Сʱ���������޹��ܵ���ʾֵ
//Сʱ���������й��ܵ���ʾֵ1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ���ʾֵi	����¼A.13	kWh	4
#define ADDR_AFN0CF111_BUFF     ADDR_AFN0CF110_BUFF+(4*MaxHourCongeal*AFN0CF110MaxPn)//Сʱ���ᷴ���й��ܵ���ʾֵ
//Сʱ���������й��ܵ���ʾֵ1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ���ʾֵi	����¼A.13	kWh	4
#define ADDR_AFN0CF112_BUFF     ADDR_AFN0CF111_BUFF+(4*MaxHourCongeal*AFN0CF111MaxPn)//Сʱ���ᷴ���޹��ܵ���ʾֵ
//Сʱ���������й��ܵ���ʾֵ1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ���ʾֵi	����¼A.13	kWh	4
#define ADDR_AFN0CF113_BUFF     ADDR_AFN0CF112_BUFF+(4*MaxHourCongeal*AFN0CF112MaxPn)//Сʱ�����ܹ�������
//Сʱ�����ܹ�������1	����¼A.5	��	2
//����	����	����	����
//Сʱ�����ܹ�������i	����¼A.5	��	2
#define ADDR_AFN0CF114_BUFF     ADDR_AFN0CF113_BUFF+(2*MaxHourCongeal*AFN0CF113MaxPn)//Сʱ����A��������
//Сʱ����A��������1	����¼A.5	��	2
//����	����	����	����
//Сʱ����A��������i	����¼A.5	��	2
#define ADDR_AFN0CF115_BUFF     ADDR_AFN0CF114_BUFF+(2*MaxHourCongeal*AFN0CF114MaxPn)//Сʱ����B��������
//Сʱ����B��������1	����¼A.5	��	2
//����	����	����	����
//Сʱ����B��������i	����¼A.5	��	2
#define ADDR_AFN0CF116_BUFF     ADDR_AFN0CF115_BUFF+(2*MaxHourCongeal*AFN0CF115MaxPn)//Сʱ����C��������
//Сʱ����C��������1	����¼A.5	��	2
//����	����	����	����
//Сʱ����C��������i	����¼A.5	��	2
#define ADDR_AFN0CF121_BUFF     ADDR_AFN0CF116_BUFF+(2*MaxHourCongeal*AFN0CF116MaxPn)//Сʱ����ֱ��ģ����
//Сʱ����ֱ��ģ����1	����¼A.2	2
//����		
//Сʱ����ֱ��ģ����i	����¼A.2	2
#define ADDR_HOURCONGEAL_BUFF_END     (ADDR_AFN0CF121_BUFF+(2*MaxHourCongeal*AFN0CF121MaxPn))//Сʱ���Ỻ�������ַ


#define LEN_HOURCONGEAL_BUFF     ((ADDR_HOURCONGEAL_BUFF_END)-(ADDR_HOURCONGEAL_BUFF_START))







#endif

