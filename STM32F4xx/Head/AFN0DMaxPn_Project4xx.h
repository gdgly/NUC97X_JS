
//400=�����ն�
//2������(�ն���,�¶���,����)��Fn���Pn
#ifndef AFN0DMaxPn_Project4xx_H
#define AFN0DMaxPn_Project4xx_H



//2������.�ն���
#define AFN0DF1MaxPn     0//MaxRS485AddCarrierPn//������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#define AFN0DF2MaxPn     0//MaxRS485AddCarrierPn//�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#define AFN0DF3MaxPn     0//MaxRS485AddCarrierPn//������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
#define AFN0DF4MaxPn     0//MaxRS485AddCarrierPn//�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
	#if (RMM_DateCongeal_0xEE==0)
	//#if ((RMM_DateCongeal_0xEE==0)||(AFN0CF41F42F43F44==0))
	//RMM_DateCongeal_0xEE=0:2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	//AFN0CF41F42F43F44==0:���յ�����AFN0CF41F42F43F44����;0=ʾֵʵʱ����,1=��ǰʾֵ-����0�㶳��
#define AFN0DF5MaxPn     MaxRS485Pn//�����������й����������ܡ�����1��M��
#define AFN0DF6MaxPn     MaxRS485Pn//�����������޹����������ܡ�����1��M��
#define AFN0DF7MaxPn     MaxRS485Pn//�����㷴���й����������ܡ�����1��M��
#define AFN0DF8MaxPn     MaxRS485Pn//�����㷴���޹����������ܡ�����1��M��
	#else
		#if AFN0DF1MaxPn!=0
			#error AFN0DF1MaxPn!=0,AFN0DF5MaxPn==0
		#endif
#define AFN0DF5MaxPn     0//MaxRS485Pn//�����������й����������ܡ�����1��M��
#define AFN0DF6MaxPn     0//MaxRS485Pn//�����������޹����������ܡ�����1��M��
#define AFN0DF7MaxPn     0//MaxRS485Pn//�����㷴���й����������ܡ�����1��M��
#define AFN0DF8MaxPn     0//MaxRS485Pn//�����㷴���޹����������ܡ�����1��M��
	#endif
#define AFN0DF25MaxPn    MaxRS485Pn//���������ܼ���������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
#define AFN0DF26MaxPn    MaxRS485Pn//���������ܼ������������������ʱ��
#define AFN0DF27MaxPn    MaxRS485Pn//�������յ�ѹͳ������
#define AFN0DF28MaxPn    MaxRS485Pn//�������ղ�ƽ���Խ���ۼ�ʱ��
#define AFN0DF29MaxPn    MaxRS485Pn//�������յ���Խ��ͳ��
#define AFN0DF30MaxPn    MaxRS485Pn//�����������ڹ���Խ���ۼ�ʱ��
#define AFN0DF31MaxPn    MaxRS485Pn//�������ո�������ͳ��
#define AFN0DF32MaxPn     MaxRS485Pn//�������յ��ܱ��������
#define AFN0DF41MaxPn     MaxCPn//�����������Ͷ���ۼ�ʱ��ʹ���
#define AFN0DF42MaxPn     MaxCPn//�������ա��µ������ۼƲ������޹�������
#define AFN0DF43MaxPn     MaxRS485Pn//�������չ������������ۼ�ʱ��
#define AFN0DF45MaxPn     MaxRS485Pn//������ͭ�������й�����ʾֵ

#define AFN0DF57MaxPn     MaxTotalAddGroup//�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
#define AFN0DF58MaxPn     MaxTotalAddGroup//�ܼ������ۼ��й����������ܡ�����1��M��
#define AFN0DF59MaxPn     MaxTotalAddGroup//�ܼ������ۼ��޹����������ܡ�����1��M��
#define AFN0DF60MaxPn     MaxTotalAddGroup//�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
#define AFN0DF61MaxPn     MaxTotalAddGroup//�ܼ������ۼ��й����������ܡ�����1��M��
#define AFN0DF62MaxPn     MaxTotalAddGroup//�ܼ������ۼ��޹����������ܡ�����1��M��
#define AFN0DF65MaxPn     MaxTotalAddGroup//�ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
#define AFN0DF66MaxPn     MaxTotalAddGroup//�ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����

	#if MeterHarmonic==0//г������0=û,1=��
#define AFN0DF113MaxPn     0//MaxRS485Pn//������A��2��19��г�����������ֵ������ʱ��
#define AFN0DF114MaxPn     0//MaxRS485Pn//������B��2��19��г�����������ֵ������ʱ��
#define AFN0DF115MaxPn     0//MaxRS485Pn//������C��2��19��г�����������ֵ������ʱ��
#define AFN0DF116MaxPn     0//MaxRS485Pn//������A��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define AFN0DF117MaxPn     0//MaxRS485Pn//������B��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define AFN0DF118MaxPn     0//MaxRS485Pn//������C��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define AFN0DF121MaxPn     0//MaxRS485Pn//������A��г��Խ����ͳ������
#define AFN0DF122MaxPn     0//MaxRS485Pn//������B��г��Խ����ͳ������
#define AFN0DF123MaxPn     0//MaxRS485Pn//������C��г��Խ����ͳ������
#define AFN0DF124MaxPn     0//MaxRS485Pn//������A���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
#define AFN0DF125MaxPn     0//MaxRS485Pn//������B���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
#define AFN0DF126MaxPn     0//MaxRS485Pn//������C���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
	#else
#define AFN0DF113MaxPn     MaxRS485Pn//������A��2��19��г�����������ֵ������ʱ��
#define AFN0DF114MaxPn     MaxRS485Pn//������B��2��19��г�����������ֵ������ʱ��
#define AFN0DF115MaxPn     MaxRS485Pn//������C��2��19��г�����������ֵ������ʱ��
#define AFN0DF116MaxPn     MaxRS485Pn//������A��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define AFN0DF117MaxPn     MaxRS485Pn//������B��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define AFN0DF118MaxPn     MaxRS485Pn//������C��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define AFN0DF121MaxPn     MaxRS485Pn//������A��г��Խ����ͳ������
#define AFN0DF122MaxPn     MaxRS485Pn//������B��г��Խ����ͳ������
#define AFN0DF123MaxPn     MaxRS485Pn//������C��г��Խ����ͳ������
#define AFN0DF124MaxPn     MaxRS485Pn//������A���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
#define AFN0DF125MaxPn     MaxRS485Pn//������B���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
#define AFN0DF126MaxPn     MaxRS485Pn//������C���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
	#endif
#define AFN0DF129MaxPn     MaxDCPn//������ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
#define AFN0DF153MaxPn     MaxRS485Pn//��������������й�����ʾֵ
#define AFN0DF154MaxPn     MaxRS485Pn//��������������޹�����ʾֵ
#define AFN0DF155MaxPn     MaxRS485Pn//��������෴���й�����ʾֵ
#define AFN0DF156MaxPn     MaxRS485Pn//��������෴���޹�����ʾֵ
#define AFN0DF161MaxPn     MaxRS485AddCarrierPn//�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF162MaxPn     MaxRS485AddCarrierPn//�����������޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0DF163MaxPn     MaxRS485AddCarrierPn//�����㷴���й�����ʾֵ���ܡ�����1��M��
#define AFN0DF164MaxPn     MaxRS485AddCarrierPn//�����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0DF165MaxPn     MaxRS485AddCarrierPn//������1�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF166MaxPn     MaxRS485AddCarrierPn//������2�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF167MaxPn     MaxRS485AddCarrierPn//������3�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF168MaxPn     MaxRS485AddCarrierPn//������4�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF185MaxPn     MaxRS485AddCarrierPn//�����������й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF186MaxPn     MaxRS485Pn//�����������޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF187MaxPn     MaxRS485AddCarrierPn//�����㷴���й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF188MaxPn     MaxRS485Pn//�����㷴���޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF209MaxPn     MaxRS485AddCarrierPn//��������ܱ�Զ�̿���ͨ�ϵ�״̬����¼
#define AFN0DF210MaxPn     MaxRS485AddCarrierPn//��������ܱ����õ���Ϣ
#define AFN0DF211MaxPn     MaxRS485Pn//�����������ѹ��������ƽ���95%����ֵ

//2������.�����ն���
#define AFN0DF9MaxPn     0//MaxRS485AddCarrierPn//������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#define AFN0DF10MaxPn     0//MaxRS485AddCarrierPn//�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#define AFN0DF11MaxPn     0//MaxRS485AddCarrierPn//������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
#define AFN0DF12MaxPn     0//MaxRS485AddCarrierPn//�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
#define AFN0DF169MaxPn     MaxRS485AddCarrierPn//�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF170MaxPn     MaxRS485AddCarrierPn//�����������޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0DF171MaxPn     MaxRS485AddCarrierPn//�����㷴���й�����ʾֵ���ܡ�����1��M��
#define AFN0DF172MaxPn     MaxRS485AddCarrierPn//�����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0DF173MaxPn     MaxRS485AddCarrierPn//������1�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF174MaxPn     MaxRS485AddCarrierPn//������2�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF175MaxPn     MaxRS485AddCarrierPn//������3�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF176MaxPn     MaxRS485AddCarrierPn//������4�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF189MaxPn     MaxRS485AddCarrierPn//�����������й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF190MaxPn     MaxRS485AddCarrierPn//�����������޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF191MaxPn     MaxRS485AddCarrierPn//�����㷴���й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF192MaxPn     MaxRS485AddCarrierPn//�����㷴���޹��������������ʱ�䣨�ܡ�����1��M��

//2������.�¶���
#define AFN0DF17MaxPn     0//MaxRS485AddCarrierPn//������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#define AFN0DF18MaxPn     0//MaxRS485AddCarrierPn//�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#define AFN0DF19MaxPn     0//MaxRS485AddCarrierPn//������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
#define AFN0DF20MaxPn     0//MaxRS485AddCarrierPn//�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
	#if (RMM_DateCongeal_0xEE==0)
	//#if ((RMM_DateCongeal_0xEE==0)||(AFN0CF45F46F47F48==0))
	//RMM_DateCongeal_0xEE=0:2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	//AFN0CF45F46F47F48=0:���µ�����AFN0CF45F46F47F48����;0=ʾֵʵʱ����,1=��ǰʾֵ-����0�㶳��
#define AFN0DF21MaxPn     MaxRS485Pn//�����������й����������ܡ�����1��M��
#define AFN0DF22MaxPn     MaxRS485Pn//�����������޹����������ܡ�����1��M��
#define AFN0DF23MaxPn     MaxRS485Pn//�����㷴���й����������ܡ�����1��M��
#define AFN0DF24MaxPn     MaxRS485Pn//�����㷴���޹����������ܡ�����1��M��
	#else
		#if AFN0DF17MaxPn!=0
			#error AFN0DF17MaxPn!=0,AFN0DF21MaxPn==0
		#endif
#define AFN0DF21MaxPn     0//MaxRS485Pn//�����������й����������ܡ�����1��M��
#define AFN0DF22MaxPn     0//MaxRS485Pn//�����������޹����������ܡ�����1��M��
#define AFN0DF23MaxPn     0//MaxRS485Pn//�����㷴���й����������ܡ�����1��M��
#define AFN0DF24MaxPn     0//MaxRS485Pn//�����㷴���޹����������ܡ�����1��M��
	#endif
#define AFN0DF33MaxPn     MaxRS485Pn//���������ܼ���������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
#define AFN0DF34MaxPn     MaxRS485Pn//���������ܼ������й��������������ʱ��
#define AFN0DF35MaxPn     MaxRS485Pn//�������µ�ѹͳ������
#define AFN0DF36MaxPn     MaxRS485Pn//�������²�ƽ���Խ���ۼ�ʱ��
#define AFN0DF37MaxPn     MaxRS485Pn//�������µ���Խ��ͳ��
#define AFN0DF38MaxPn     MaxRS485Pn//�����������ڹ���Խ���ۼ�ʱ��
#define AFN0DF39MaxPn     MaxRS485Pn//�������¸�������ͳ��
#define AFN0DF44MaxPn     MaxRS485Pn//�������¹������������ۼ�ʱ��
#define AFN0DF46MaxPn     MaxRS485Pn//�������¶���ͭ�������й�����ʾֵ

#define AFN0DF130MaxPn     MaxDCPn//ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
#define AFN0DF157MaxPn     MaxRS485Pn//�������¶�����������й�����ʾֵ
#define AFN0DF158MaxPn     MaxRS485Pn//�������¶�����������޹�����ʾֵ
#define AFN0DF159MaxPn     MaxRS485Pn//�������¶�����෴���й�����ʾֵ
#define AFN0DF160MaxPn     MaxRS485Pn//�������¶�����෴���޹�����ʾֵ
#define AFN0DF177MaxPn     MaxRS485AddCarrierPn//�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF178MaxPn     MaxRS485AddCarrierPn//�����������޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0DF179MaxPn     MaxRS485AddCarrierPn//�����㷴���й�����ʾֵ���ܡ�����1��M��
#define AFN0DF180MaxPn     MaxRS485AddCarrierPn//�����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0DF181MaxPn     MaxRS485AddCarrierPn//������1�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF182MaxPn     MaxRS485AddCarrierPn//������2�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF183MaxPn     MaxRS485AddCarrierPn//������3�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF184MaxPn     MaxRS485AddCarrierPn//������4�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0DF193MaxPn     MaxRS485AddCarrierPn//�����������й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF194MaxPn     MaxRS485AddCarrierPn//�����������޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF195MaxPn     MaxRS485AddCarrierPn//�����㷴���й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF196MaxPn     MaxRS485AddCarrierPn//�����㷴���޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0DF201MaxPn     MaxRS485AddCarrierPn//�������1ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF202MaxPn     MaxRS485AddCarrierPn//�������2ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF203MaxPn     MaxRS485AddCarrierPn//�������3ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF204MaxPn     MaxRS485AddCarrierPn//�������4ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF205MaxPn     MaxRS485AddCarrierPn//�������5ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF206MaxPn     MaxRS485AddCarrierPn//�������6ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF207MaxPn     MaxRS485AddCarrierPn//�������7ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF208MaxPn     MaxRS485AddCarrierPn//�������8ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0DF212MaxPn     MaxRS485Pn//�����������ѹ��������ƽ���95%����ֵ
#define AFN0DF213MaxPn     MaxRS485AddCarrierPn//��������ܱ��ز���������ʱ��
#define AFN0DF214MaxPn     MaxRS485AddCarrierPn//��������ܱ�����޸Ĵ�����ʱ��
#define AFN0DF215MaxPn     MaxRS485AddCarrierPn//��������ܱ����õ���Ϣ
#define AFN0DF216MaxPn     MaxRS485AddCarrierPn//��������ܱ������Ϣ


//2������.����
#define AFN0DF73MaxPn     MaxTotalAddGroup//�ܼ����й���������
#define AFN0DF74MaxPn     MaxTotalAddGroup//�ܼ����޹���������
#define AFN0DF75MaxPn     MaxTotalAddGroup//�ܼ����й�����������
#define AFN0DF76MaxPn     MaxTotalAddGroup//�ܼ����޹�����������
#define AFN0DF81MaxPn     MaxKeyUser//�������й���������
#define AFN0DF82MaxPn     MaxKeyUser//������A���й���������
#define AFN0DF83MaxPn     MaxKeyUser//������B���й���������
#define AFN0DF84MaxPn     MaxKeyUser//������C���й���������
#define AFN0DF85MaxPn     MaxKeyUser//�������޹���������
#define AFN0DF86MaxPn     MaxKeyUser//������A�޹���������
#define AFN0DF87MaxPn     MaxKeyUser//������B�޹���������
#define AFN0DF88MaxPn     MaxKeyUser//������C�޹���������
#define AFN0DF89MaxPn     MaxKeyUser//������A���ѹ����
#define AFN0DF90MaxPn     MaxKeyUser//������B���ѹ����
#define AFN0DF91MaxPn     MaxKeyUser//������C���ѹ����
#define AFN0DF92MaxPn     MaxKeyUser//������A���������
#define AFN0DF93MaxPn     MaxKeyUser//������B���������
#define AFN0DF94MaxPn     MaxKeyUser//������C���������
#define AFN0DF95MaxPn     MaxKeyUser//�����������������
	#if MaxImpInPort!=0//�����������˿���(<=8)
#define AFN0DF97MaxPn     MaxRS485Pn//�����������й��ܵ�����
#define AFN0DF98MaxPn     MaxRS485Pn//�����������޹��ܵ�����
#define AFN0DF99MaxPn     MaxRS485Pn//�����㷴���й��ܵ�����
#define AFN0DF100MaxPn     MaxRS485Pn//�����㷴���޹��ܵ�����
	#else
#define AFN0DF97MaxPn     0//MaxRS485Pn//�����������й��ܵ�����
#define AFN0DF98MaxPn     0//MaxRS485Pn//�����������޹��ܵ�����
#define AFN0DF99MaxPn     0//MaxRS485Pn//�����㷴���й��ܵ�����
#define AFN0DF100MaxPn     0//MaxRS485Pn//�����㷴���޹��ܵ�����
	#endif
#define AFN0DF101MaxPn     MaxKeyUser//�����������й��ܵ���ʾֵ
#define AFN0DF102MaxPn     MaxKeyUser//�����������޹��ܵ���ʾֵ
#define AFN0DF103MaxPn     MaxKeyUser//�����㷴���й��ܵ���ʾֵ
#define AFN0DF104MaxPn     MaxKeyUser//�����㷴���޹��ܵ���ʾֵ
#define AFN0DF105MaxPn     MaxKeyUser//�������ܹ�������
#define AFN0DF106MaxPn     MaxKeyUser//������A�๦������
#define AFN0DF107MaxPn     MaxKeyUser//������B�๦������
#define AFN0DF108MaxPn     MaxKeyUser//������C�๦������
#define AFN0DF109MaxPn     MaxKeyUser//�������ѹ��λ������
#define AFN0DF110MaxPn     MaxKeyUser//�����������λ������
#define AFN0DF138MaxPn     MaxDCPn//ֱ��ģ������������
#define AFN0DF145MaxPn     MaxKeyUser//������1�����޹��ܵ���ʾֵ����
#define AFN0DF146MaxPn     MaxKeyUser//������2�����޹��ܵ���ʾֵ����
#define AFN0DF147MaxPn     MaxKeyUser//������3�����޹��ܵ���ʾֵ����
#define AFN0DF148MaxPn     MaxKeyUser//������4�����޹��ܵ���ʾֵ����
#define AFN0DF219MaxPn     MaxKeyUser//�����������������






#endif
