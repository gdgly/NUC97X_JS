

#ifndef ADDR_AFN0C_HourCongeal_H
#define ADDR_AFN0C_HourCongeal_H





//��1����
#define ADDR_HOURCONGEAL_START     (ADDR_AFN0C_HourCongeal_Start)//Сʱ���Ὺʼ��ַ
//1BYTEСʱ����������ʱ�궳��ʱ��ʱ
//1BYTE Сʱ�����ܶ�,ֵ=0ʱ��ʾ��Ч����
#define ADDR_AFN0CF81     ADDR_HOURCONGEAL_START+2//Сʱ�����ܼ��й�����
//Сʱ�����ܼ��й�����1	����¼A.2	kW	2
//����	����	����	����
//Сʱ�����ܼ��й�����i	����¼A.2	kW	2
#define ADDR_AFN0CF82     ADDR_AFN0CF81+(2*MaxHourCongeal*MaxTotalAddGroup)//Сʱ�����ܼ��޹�����
//Сʱ�����ܼ��޹�����1	����¼A.2	kvar	2
//����	����	����	����
//Сʱ�����ܼ��޹�����i	����¼A.2	kvar	2
#define ADDR_AFN0CF83     ADDR_AFN0CF82+(2*MaxHourCongeal*MaxTotalAddGroup)//Сʱ�����ܼ��й��ܵ�����
//Сʱ�����ܼ��й��ܵ�����1	����¼A.3	kWh	4
//����	����	����	����
//Сʱ�����ܼ��й��ܵ�����i	����¼A.3	kWh	4
#define ADDR_AFN0CF84     ADDR_AFN0CF83+(4*MaxHourCongeal*MaxTotalAddGroup)//Сʱ�����ܼ��޹��ܵ�����
//Сʱ�����ܼ��޹��ܵ�����1	����¼A.3	kvarh	4
//����	����	����	����
//Сʱ�����ܼ��޹��ܵ�����i	����¼A.3	kvarh	4
#define ADDR_AFN0CF89     ADDR_AFN0CF84+(4*MaxHourCongeal*MaxTotalAddGroup)//Сʱ�����й�����
//Сʱ�����й�����1	����¼A.9	kW	3
//����	����	����	����
//Сʱ�����й�����i	����¼A.9	kW	3
#define ADDR_AFN0CF90     ADDR_AFN0CF89+(3*MaxHourCongeal*AFN0CF89MaxPn)//Сʱ����A���й�����
//Сʱ�����й�����1	����¼A.9	kW	3
//����	����	����	����
//Сʱ�����й�����i	����¼A.9	kW	3
#define ADDR_AFN0CF91     ADDR_AFN0CF90+(3*MaxHourCongeal*AFN0CF90MaxPn)//Сʱ����B���й�����
//Сʱ�����й�����1	����¼A.9	kW	3
//����	����	����	����
//Сʱ�����й�����i	����¼A.9	kW	3
#define ADDR_AFN0CF92     ADDR_AFN0CF91+(3*MaxHourCongeal*AFN0CF91MaxPn)//Сʱ����C���й�����
//Сʱ�����й�����1	����¼A.9	kW	3
//����	����	����	����
//Сʱ�����й�����i	����¼A.9	kW	3
#define ADDR_AFN0CF93     ADDR_AFN0CF92+(3*MaxHourCongeal*AFN0CF92MaxPn)//Сʱ�����޹�����
//Сʱ�����޹�����1	����¼A.9	kvar	3
//����	����	����	����
//Сʱ�����޹�����i	����¼A.9	kvar	3
#define ADDR_AFN0CF94     ADDR_AFN0CF93+(3*MaxHourCongeal*AFN0CF93MaxPn)//Сʱ����A�޹�����
//Сʱ�����޹�����1	����¼A.9	kvar	3
//����	����	����	����
//Сʱ�����޹�����i	����¼A.9	kvar	3
#define ADDR_AFN0CF95     ADDR_AFN0CF94+(3*MaxHourCongeal*AFN0CF94MaxPn)//Сʱ����B�޹�����
//Сʱ�����޹�����1	����¼A.9	kvar	3
//����	����	����	����
//Сʱ�����޹�����i	����¼A.9	kvar	3
#define ADDR_AFN0CF96     ADDR_AFN0CF95+(3*MaxHourCongeal*AFN0CF95MaxPn)//Сʱ����C�޹�����
//Сʱ�����޹�����1	����¼A.9	kvar	3
//����	����	����	����
//Сʱ�����޹�����i	����¼A.9	kvar	3
#define ADDR_AFN0CF97     ADDR_AFN0CF96+(3*MaxHourCongeal*AFN0CF96MaxPn)//Сʱ����A���ѹ
//Сʱ����A���ѹ1	����¼A.7	V	2
//����	����	����	����
//Сʱ����A���ѹi	����¼A.7	V	2
#define ADDR_AFN0CF98     ADDR_AFN0CF97+(2*MaxHourCongeal*AFN0CF97MaxPn)//Сʱ����B���ѹ
//Сʱ����B���ѹ1	����¼A.7	V	2
//����	����	����	����
//Сʱ����B���ѹi	����¼A.7	V	2
#define ADDR_AFN0CF99     ADDR_AFN0CF98+(2*MaxHourCongeal*AFN0CF98MaxPn)//Сʱ����C���ѹ
//Сʱ����C���ѹ1	����¼A.7	V	2
//����	����	����	����
//Сʱ����C���ѹi	����¼A.7	V	2
#define ADDR_AFN0CF100     ADDR_AFN0CF99+(2*MaxHourCongeal*AFN0CF99MaxPn)//Сʱ����A�����
//Сʱ����A�����1	����¼A.25	A	3
//����	����	����	����
//Сʱ����A�����i	����¼A.25	A	3
#define ADDR_AFN0CF101     ADDR_AFN0CF100+(3*MaxHourCongeal*AFN0CF100MaxPn)//Сʱ����B�����
//Сʱ����B�����1	����¼A.25	A	3
//����	����	����	����
//Сʱ����B�����i	����¼A.25	A	3
#define ADDR_AFN0CF102     ADDR_AFN0CF101+(3*MaxHourCongeal*AFN0CF101MaxPn)//Сʱ����C�����
//Сʱ����C�����1	����¼A.25	A	3
//����	����	����	����
//Сʱ����C�����i	����¼A.25	A	3
#define ADDR_AFN0CF103     ADDR_AFN0CF102+(3*MaxHourCongeal*AFN0CF102MaxPn)//Сʱ�����������
//Сʱ����N�����1	����¼A.25	A	3
//����	����	����	����
//Сʱ����N�����i	����¼A.25	A	3
#define ADDR_AFN0CF105     ADDR_AFN0CF103+(3*MaxHourCongeal*AFN0CF103MaxPn)//Сʱ���������й��ܵ�����
//Сʱ���������й��ܵ�����1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ�����i	����¼A.13	kWh	4
#define ADDR_AFN0CF106     ADDR_AFN0CF105+(4*MaxHourCongeal*AFN0CF105MaxPn)//Сʱ���������޹��ܵ�����
//Сʱ���������޹��ܵ�����1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������޹��ܵ�����i	����¼A.13	kWh	4
#define ADDR_AFN0CF107     ADDR_AFN0CF106+(4*MaxHourCongeal*AFN0CF106MaxPn)//Сʱ���ᷴ���й��ܵ�����
//Сʱ���ᷴ���й��ܵ�����1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���ᷴ���й��ܵ�����i	����¼A.13	kWh	4
#define ADDR_AFN0CF108     ADDR_AFN0CF107+(4*MaxHourCongeal*AFN0CF107MaxPn)//Сʱ���ᷴ���޹��ܵ�����
//Сʱ���ᷴ���޹��ܵ�����1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���ᷴ���޹��ܵ�����i	����¼A.13	kWh	4
#define ADDR_AFN0CF109     ADDR_AFN0CF108+(4*MaxHourCongeal*AFN0CF108MaxPn)//Сʱ���������й��ܵ���ʾֵ
//Сʱ���������й��ܵ���ʾֵ1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ���ʾֵi	����¼A.13	kWh	4
#define ADDR_AFN0CF110     ADDR_AFN0CF109+(4*MaxHourCongeal*AFN0CF109MaxPn)//Сʱ���������޹��ܵ���ʾֵ
//Сʱ���������й��ܵ���ʾֵ1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ���ʾֵi	����¼A.13	kWh	4
#define ADDR_AFN0CF111     ADDR_AFN0CF110+(4*MaxHourCongeal*AFN0CF110MaxPn)//Сʱ���ᷴ���й��ܵ���ʾֵ
//Сʱ���������й��ܵ���ʾֵ1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ���ʾֵi	����¼A.13	kWh	4
#define ADDR_AFN0CF112     ADDR_AFN0CF111+(4*MaxHourCongeal*AFN0CF111MaxPn)//Сʱ���ᷴ���޹��ܵ���ʾֵ
//Сʱ���������й��ܵ���ʾֵ1	����¼A.13	kWh	4
//����	����	����	����
//Сʱ���������й��ܵ���ʾֵi	����¼A.13	kWh	4
#define ADDR_AFN0CF113     ADDR_AFN0CF112+(4*MaxHourCongeal*AFN0CF112MaxPn)//Сʱ�����ܹ�������
//Сʱ�����ܹ�������1	����¼A.5	��	2
//����	����	����	����
//Сʱ�����ܹ�������i	����¼A.5	��	2
#define ADDR_AFN0CF114     ADDR_AFN0CF113+(2*MaxHourCongeal*AFN0CF113MaxPn)//Сʱ����A��������
//Сʱ����A��������1	����¼A.5	��	2
//����	����	����	����
//Сʱ����A��������i	����¼A.5	��	2
#define ADDR_AFN0CF115     ADDR_AFN0CF114+(2*MaxHourCongeal*AFN0CF114MaxPn)//Сʱ����B��������
//Сʱ����B��������1	����¼A.5	��	2
//����	����	����	����
//Сʱ����B��������i	����¼A.5	��	2
#define ADDR_AFN0CF116     ADDR_AFN0CF115+(2*MaxHourCongeal*AFN0CF115MaxPn)//Сʱ����C��������
//Сʱ����C��������1	����¼A.5	��	2
//����	����	����	����
//Сʱ����C��������i	����¼A.5	��	2
#define ADDR_AFN0CF121     ADDR_AFN0CF116+(2*MaxHourCongeal*AFN0CF116MaxPn)//Сʱ����ֱ��ģ����
//Сʱ����ֱ��ģ����1	����¼A.2	2
//����		
//Сʱ����ֱ��ģ����i	����¼A.2	2
#define ADDR_HOURCONGEAL_END     ADDR_AFN0CF121+(2*MaxHourCongeal*AFN0CF121MaxPn)//Сʱ���Ỻ�������ַ


#define ADDR_AFN0C_HourCongeal_End     (ADDR_HOURCONGEAL_END)//��1���ݽ�����ַ





#endif


