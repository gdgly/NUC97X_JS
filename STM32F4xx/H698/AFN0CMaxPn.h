

//1������(ʵʱ)��Fn���Pn
#ifndef AFN0CMaxPn_H
#define AFN0CMaxPn_H




#define AFN0CF5MaxPn     MaxTotalAddGroup//�ն˿�������״̬
#define AFN0CF6MaxPn     MaxTotalAddGroup//�ն˵�ǰ����״̬

#define AFN0CF17MaxPn     MaxTotalAddGroup//��ǰ�ܼ��й�����;�ܼ����
#define AFN0CF18MaxPn     MaxTotalAddGroup//��ǰ�ܼ��޹�����;�ܼ����
#define AFN0CF19MaxPn     MaxTotalAddGroup//�����ܼ��й����������ܡ�����1��M��;�ܼ����
#define AFN0CF20MaxPn     MaxTotalAddGroup//�����ܼ��޹����������ܡ�����1��M��;�ܼ����
#define AFN0CF21MaxPn     MaxTotalAddGroup//�����ܼ��й����������ܡ�����1��M��;�ܼ����
#define AFN0CF22MaxPn     MaxTotalAddGroup//�����ܼ��޹����������ܡ�����1��M��;�ܼ����
#define AFN0CF23MaxPn     MaxTotalAddGroup//�ն˵�ǰʣ��������ѣ�;�ܼ����
#define AFN0CF24MaxPn     MaxTotalAddGroup//��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ;�ܼ����

#define AFN0CF25MaxPn     MaxRS485AddCarrierPn//��ǰ���༰����/�޹����ʡ����������������ѹ��������������������ڹ���(�������)
#define AFN0CF26MaxPn     MaxRS485AddCarrierPn//A��B��C�������ͳ�����ݼ����һ�ζ����¼
#define AFN0CF27MaxPn     MaxRS485AddCarrierPn//���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
#define AFN0CF28MaxPn     MaxRS485AddCarrierPn//�������״̬�ּ����λ��־
#define AFN0CF29MaxPn     MaxRS485AddCarrierPn//��ǰͭ�������й��ܵ���ʾֵ
#define AFN0CF30MaxPn     MaxRS485AddCarrierPn//��һ������ͭ�������й��ܵ���ʾֵ
#define AFN0CF31MaxPn     MaxRS485AddCarrierPn//��ǰA��B��C������/�����й�����ʾֵ������޹�1/2����ʾֵ
#define AFN0CF32MaxPn     MaxRS485AddCarrierPn//��һ������A��B��C������/�����й�����ʾֵ������޹�1/2����ʾֵ
#define AFN0CF33MaxPn     0//MaxRS485AddCarrierPn//��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#define AFN0CF34MaxPn     0//MaxRS485AddCarrierPn//��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
#define AFN0CF35MaxPn     0//MaxRS485AddCarrierPn//����������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
#define AFN0CF36MaxPn     0//MaxRS485AddCarrierPn//���·�����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
#define AFN0CF37MaxPn     0//MaxRS485AddCarrierPn//���£���һ�����գ�������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF38MaxPn     0//MaxRS485AddCarrierPn//���£���һ�����գ�������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF39MaxPn     0//MaxRS485AddCarrierPn//���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF40MaxPn     0//MaxRS485AddCarrierPn//���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��

#define AFN0CF41MaxPn     MaxRS485Pn//���������й�������(�ܡ�����l��M)(�������)
#define AFN0CF42MaxPn     MaxRS485Pn//���������޹�������(�ܡ�����1��M)(�������)
#define AFN0CF43MaxPn     MaxRS485Pn//���շ����й�������(�ܡ�����l��M)(�������)
#define AFN0CF44MaxPn     MaxRS485Pn//���շ����޹�������(�ܡ�����l��M)(�������)
#define AFN0CF45MaxPn     MaxRS485Pn//���������й�������(�ܡ�����l��M)(�������)
#define AFN0CF46MaxPn     MaxRS485Pn//���������޹�������(�ܡ�����1��M)(�������)
#define AFN0CF47MaxPn     MaxRS485Pn//���·����й�������(�ܡ�����1��M)(�������)
#define AFN0CF48MaxPn     MaxRS485Pn//���·����޹�������(�ܡ�����l��M)(�������)

#define AFN0CF49MaxPn     MaxRS485AddCarrierPn//��ǰ��ѹ��������λ��
	#if MeterHarmonic==0//г������0=û,1=��
#define AFN0CF57MaxPn     0//MaxRS485Pn//��ǰA��B��C�����ѹ������2��N��г����Чֵ
#define AFN0CF58MaxPn     0//MaxRS485Pn//��ǰA��B��C�����ѹ������2��N��г��������
	#else
#define AFN0CF57MaxPn     MaxRS485Pn//��ǰA��B��C�����ѹ������2��N��г����Чֵ
#define AFN0CF58MaxPn     MaxRS485Pn//��ǰA��B��C�����ѹ������2��N��г��������
	#endif
#define AFN0CF65MaxPn     MaxCPn//��ǰ������Ͷ��״̬
#define AFN0CF66MaxPn     MaxCPn//��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���(�������)
#define AFN0CF67MaxPn     MaxCPn//���ա����µ������ۼƲ������޹�������(�������)

#define AFN0CF73MaxPn     MaxDCPn//ֱ��ģ������ǰ����

#define AFN0CF81MaxPn     MaxTotalAddGroup//Сʱ�����ܼ��й�����
#define AFN0CF82MaxPn     MaxTotalAddGroup//Сʱ�����ܼ��޹�����
#define AFN0CF83MaxPn     MaxTotalAddGroup//Сʱ�����ܼ��й��ܵ�����
#define AFN0CF84MaxPn     MaxTotalAddGroup//Сʱ�����ܼ��޹��ܵ�����

#define AFN0CF89MaxPn     MaxRS485Pn//Сʱ�����й�����
#define AFN0CF90MaxPn     MaxRS485Pn//Сʱ����A���й�����
#define AFN0CF91MaxPn     MaxRS485Pn//Сʱ����B���й�����
#define AFN0CF92MaxPn     MaxRS485Pn//Сʱ����C���й�����
#define AFN0CF93MaxPn     MaxRS485Pn//Сʱ�����޹�����
#define AFN0CF94MaxPn     MaxRS485Pn//Сʱ����A�޹�����
#define AFN0CF95MaxPn     MaxRS485Pn//Сʱ����B�޹�����
#define AFN0CF96MaxPn     MaxRS485Pn//Сʱ����C�޹�����
#define AFN0CF97MaxPn     MaxRS485Pn//Сʱ����A���ѹ
#define AFN0CF98MaxPn     MaxRS485Pn//Сʱ����B���ѹ
#define AFN0CF99MaxPn     MaxRS485Pn//Сʱ����C���ѹ
#define AFN0CF100MaxPn     MaxRS485Pn//Сʱ����A�����
#define AFN0CF101MaxPn     MaxRS485Pn//Сʱ����B�����
#define AFN0CF102MaxPn     MaxRS485Pn//Сʱ����C�����
#define AFN0CF103MaxPn     MaxRS485Pn//Сʱ�����������
#define AFN0CF105MaxPn     MaxRS485Pn//Сʱ���������й��ܵ�����
#define AFN0CF106MaxPn     MaxRS485Pn//Сʱ���������޹��ܵ�����
#define AFN0CF107MaxPn     MaxRS485Pn//Сʱ���ᷴ���й��ܵ�����
#define AFN0CF108MaxPn     MaxRS485Pn//Сʱ���ᷴ���޹��ܵ�����
#define AFN0CF109MaxPn     MaxRS485Pn//Сʱ���������й��ܵ���ʾֵ
#define AFN0CF110MaxPn     MaxRS485Pn//Сʱ���������޹��ܵ���ʾֵ
#define AFN0CF111MaxPn     MaxRS485Pn//Сʱ���ᷴ���й��ܵ���ʾֵ
#define AFN0CF112MaxPn     MaxRS485Pn//Сʱ���ᷴ���޹��ܵ���ʾֵ
#define AFN0CF113MaxPn     MaxRS485Pn//Сʱ�����ܹ�������
#define AFN0CF114MaxPn     MaxRS485Pn//Сʱ����A��������
#define AFN0CF115MaxPn     MaxRS485Pn//Сʱ����B��������
#define AFN0CF116MaxPn     MaxRS485Pn//Сʱ����C��������
#define AFN0CF121MaxPn     MaxDCPn//Сʱ����ֱ��ģ����


#define AFN0CF129MaxPn     MaxRS485AddCarrierPn//��ǰ�����й�����ʾֵ���ܡ�����1��M��
#define AFN0CF130MaxPn     MaxRS485AddCarrierPn//��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0CF131MaxPn     MaxRS485AddCarrierPn//��ǰ�����й�����ʾֵ���ܡ�����1��M��
#define AFN0CF132MaxPn     MaxRS485AddCarrierPn//��ǰ�����޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0CF133MaxPn     MaxRS485AddCarrierPn//��ǰ1�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF134MaxPn     MaxRS485AddCarrierPn//��ǰ2�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF135MaxPn     MaxRS485AddCarrierPn//��ǰ3�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF136MaxPn     MaxRS485AddCarrierPn//��ǰ4�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF137MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF138MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0CF139MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF140MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ������޹�������޹�1������ʾֵ���ܡ�����1��M��
#define AFN0CF141MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ�1�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF142MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ�2�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF143MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ�3�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF144MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ�4�����޹�����ʾֵ���ܡ�����1��M��
#define AFN0CF145MaxPn     MaxRS485AddCarrierPn//���������й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF146MaxPn     MaxRS485AddCarrierPn//���������޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF147MaxPn     MaxRS485AddCarrierPn//���·����й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF148MaxPn     MaxRS485AddCarrierPn//���·����޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF149MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF150MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF151MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ������й��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF152MaxPn     MaxRS485AddCarrierPn//���£���һ�����գ������޹��������������ʱ�䣨�ܡ�����1��M��
#define AFN0CF153MaxPn     MaxRS485AddCarrierPn//��1ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF154MaxPn     MaxRS485AddCarrierPn//��2ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF155MaxPn     MaxRS485AddCarrierPn//��3ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF156MaxPn     MaxRS485AddCarrierPn//��4ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF157MaxPn     MaxRS485AddCarrierPn//��5ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF158MaxPn     MaxRS485AddCarrierPn//��6ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF159MaxPn     MaxRS485AddCarrierPn//��7ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF160MaxPn     MaxRS485AddCarrierPn//��8ʱ�����������й�����ʾֵ���ܡ�����1��M��
#define AFN0CF161MaxPn     MaxRS485AddCarrierPn//���ܱ�Զ�̿���ͨ�ϵ�״̬����¼
#define AFN0CF162MaxPn     MaxRS485AddCarrierPn//���ܱ�����ʱ��
#define AFN0CF165MaxPn     MaxRS485AddCarrierPn//���ܱ��ز���������ʱ��
#define AFN0CF166MaxPn     MaxRS485AddCarrierPn//���ܱ�����޸Ĵ�����ʱ��
#define AFN0CF167MaxPn     MaxRS485AddCarrierPn//���ܱ����õ���Ϣ
#define AFN0CF168MaxPn     MaxRS485AddCarrierPn//���ܱ������Ϣ
#if (Project/100)!=2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
#define AFN0CF169MaxPn     0//���г����м�·����Ϣ	�������
#else
#define AFN0CF169MaxPn     MaxRS485AddCarrierPn//���г����м�·����Ϣ	�������(��ʱ��·�����ж�ȡ)
#endif
#define AFN0CF170MaxPn     MaxRS485AddCarrierPn//���г���������Ϣ	�������

#define AFN0CF177MaxPn     MaxRS485AddCarrierPn//��ǰ����й�����ʾֵ���ܡ�����1~M���������
#define AFN0CF178MaxPn     MaxRS485AddCarrierPn//��1����������й�����ʾֵ���ܡ�����1~M���������
#define AFN0CF180MaxPn     MaxRS485AddCarrierPn//�ӽڵ�汾��Ϣ;�������


#endif


