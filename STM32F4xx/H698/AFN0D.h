

//�ն�2������
#ifndef AFN0D_H
#define AFN0D_H


//��2�����ն���
#define ADDR_DATECONGEAL_START     ADDR_AFN0D_Start//�ն��Ὺʼ��ַ
//3BYTE ����ʱ��(��ǰʱ����һ�յ�������)
#define ADDR_DATECONGEALFLAG     3//�ն����־
//1byte �����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
#define  ADDR_AFN0DF1     ADDR_DATECONGEALFLAG+1//������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//�ն���
#define  ADDR_AFN0DF2      ADDR_AFN0DF1+((6+(17*(1+TMaxTariff)))*AFN0DF1MaxPn)//�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//�ն���
#define  ADDR_AFN0DF3      ADDR_AFN0DF2+((6+(17*(1+TMaxTariff)))*AFN0DF2MaxPn)//������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//�ն���
#define  ADDR_AFN0DF4      ADDR_AFN0DF3+((6+(14*(1+TMaxTariff)))*AFN0DF3MaxPn)//�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//�ն���
#define  ADDR_AFN0DF5      ADDR_AFN0DF4+((6+(14*(1+TMaxTariff)))*AFN0DF4MaxPn)//�����������й����������ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF6      ADDR_AFN0DF5+((1+(4*(1+TMaxTariff)))*AFN0DF5MaxPn)//�����������޹����������ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF7      ADDR_AFN0DF6+((1+(4*(1+TMaxTariff)))*AFN0DF6MaxPn)//�����㷴���й����������ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF8      ADDR_AFN0DF7+((1+(4*(1+TMaxTariff)))*AFN0DF7MaxPn)//�����㷴���޹����������ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF25      ADDR_AFN0DF8+((1+(4*(1+TMaxTariff)))*AFN0DF8MaxPn)//���������ܼ���������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
//�ն���
#define  ADDR_AFN0DF26      ADDR_AFN0DF25+(32*AFN0DF25MaxPn)//���������ܼ������������������ʱ��
//�ն���
#define  ADDR_AFN0DF27      ADDR_AFN0DF26+(24*AFN0DF26MaxPn)//�������յ�ѹͳ������
//�ն���
	#if (USER/100)==11//����
#define  ADDR_AFN0DF28      ADDR_AFN0DF27+((66+27)*AFN0DF27MaxPn_ReadMeter)//�������ղ�ƽ���Խ���ۼ�ʱ��
	#else
#define  ADDR_AFN0DF28      ADDR_AFN0DF27+((66+27)*AFN0DF27MaxPn)//�������ղ�ƽ���Խ���ۼ�ʱ��
	#endif
//�ն���
#define  ADDR_AFN0DF29      ADDR_AFN0DF28+(14*AFN0DF28MaxPn)//�������յ���Խ��ͳ��
//�ն���
#define  ADDR_AFN0DF30      ADDR_AFN0DF29+(38*AFN0DF29MaxPn)//�����������ڹ���Խ���ۼ�ʱ��
//�ն���
#define  ADDR_AFN0DF31      ADDR_AFN0DF30+(4*AFN0DF30MaxPn)//�������ո�������ͳ��
//�ն���
#define  ADDR_AFN0DF32      ADDR_AFN0DF31+(10*AFN0DF31MaxPn)//�������յ��ܱ��������
//�ն���
#define  ADDR_AFN0DF41      ADDR_AFN0DF32+(61*AFN0DF32MaxPn)//�����������Ͷ���ۼ�ʱ��ʹ���
//�ն���
#define  ADDR_AFN0DF42      ADDR_AFN0DF41+(72*AFN0DF41MaxPn)//�������ա��µ������ۼƲ������޹�������
//�ն���
#define  ADDR_AFN0DF43      ADDR_AFN0DF42+(4*AFN0DF42MaxPn)//�������չ������������ۼ�ʱ��
//�ն���
#define  ADDR_AFN0DF45      ADDR_AFN0DF43+(6*AFN0DF43MaxPn)//������ͭ�������й�����ʾֵ
//�ն���
#define  ADDR_AFN0DF49      ADDR_AFN0DF45+(10*AFN0DF45MaxPn)//�ն��չ���ʱ�䡢�ո�λ�ۼƴ���
//�ն���
#define  ADDR_AFN0DF50      ADDR_AFN0DF49+4//�ն��տ���ͳ������
//�ն���
#define  ADDR_AFN0DF53      ADDR_AFN0DF50+4//�ն�����վ��ͨ������
//�ն���
#define  ADDR_AFN0DF57      ADDR_AFN0DF53+4//�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//�ն���
#define  ADDR_AFN0DF58      ADDR_AFN0DF57+(12*AFN0DF57MaxPn)//�ܼ������ۼ��й����������ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF59      ADDR_AFN0DF58+((1+(4*(1+TMaxTariff)))*AFN0DF58MaxPn)//�ܼ������ۼ��޹����������ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF113      ADDR_AFN0DF59+((1+(4*(1+TMaxTariff)))*AFN0DF59MaxPn)//������A��2��19��г�����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF114      ADDR_AFN0DF113+(114*AFN0DF113MaxPn)//������B��2��19��г�����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF115      ADDR_AFN0DF114+(114*AFN0DF114MaxPn)//������C��2��19��г�����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF116      ADDR_AFN0DF115+(114*AFN0DF115MaxPn)//������A��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF117      ADDR_AFN0DF116+(114*AFN0DF116MaxPn)//������B��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF118      ADDR_AFN0DF117+(114*AFN0DF117MaxPn)//������C��2��19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF121      ADDR_AFN0DF118+(114*AFN0DF118MaxPn)//������A��г��Խ����ͳ������
//�ն���
#define  ADDR_AFN0DF122      ADDR_AFN0DF121+(77*AFN0DF121MaxPn)//������B��г��Խ����ͳ������
//�ն���
#define  ADDR_AFN0DF123      ADDR_AFN0DF122+(77*AFN0DF122MaxPn)//������C��г��Խ����ͳ������
//�ն���
#define  ADDR_AFN0DF124      ADDR_AFN0DF123+(77*AFN0DF123MaxPn)//������A���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
//�ն���
#define  ADDR_AFN0DF125      ADDR_AFN0DF124+(81*AFN0DF124MaxPn)//������B���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
//�ն���
#define  ADDR_AFN0DF126      ADDR_AFN0DF125+(81*AFN0DF125MaxPn)//������C���ն����ѹг����ѹ�����ʡ�г������95%����ֵ
//�ն���
#define  ADDR_AFN0DF129      ADDR_AFN0DF126+(81*AFN0DF123MaxPn)//������ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//�ն���
#define  ADDR_AFN0DF153      ADDR_AFN0DF129+(14*AFN0DF129MaxPn)//��������������й�����ʾֵ
//�ն���
#define  ADDR_AFN0DF154      ADDR_AFN0DF153+(20*AFN0DF153MaxPn)//��������������޹�����ʾֵ
//�ն���
#define  ADDR_AFN0DF155      ADDR_AFN0DF154+(17*AFN0DF154MaxPn)//��������෴���й�����ʾֵ
//�ն���
#define  ADDR_AFN0DF156      ADDR_AFN0DF155+(20*AFN0DF155MaxPn)//��������෴���޹�����ʾֵ
//�ն���
#define  ADDR_AFN0DF161      ADDR_AFN0DF156+(17*AFN0DF156MaxPn)//�����������й�����ʾֵ���ܡ�����1��M��
//�ն˳���ʱ��	����¼A.15	��ʱ������	5
//������M	BIN	��	1
//�����й��ܵ���ʾֵ	����¼A.14	kWh	5
//����1�����й�����ʾֵ	����¼A.14	kWh	5
//����	����	����	����
//����M�����й�����ʾֵ	����¼A.14	kWh	5
#define  ADDR_AFN0DF162      ADDR_AFN0DF161+((6+(5*(1+TMaxTariff)))*AFN0DF161MaxPn)//�����������޹�������޹�1������ʾֵ���ܡ�����1��M��
//�ն˳���ʱ��	����¼A.15	��ʱ������	5
//������M	BIN	��	1
//�����޹�������޹�1���ܵ���ʾֵ	����¼A.11	kvarh	4
//����1�����޹�������޹�1������ʾֵ	����¼A.11	kvarh	4
//����	����	����	����
//����M�����޹�������޹�1������ʾֵ	����¼A.11	kvarh	4
#define  ADDR_AFN0DF163      ADDR_AFN0DF162+((6+(4*(1+TMaxTariff)))*AFN0DF162MaxPn)//�����㷴���й�����ʾֵ���ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF164      ADDR_AFN0DF163+((6+(5*(1+TMaxTariff)))*AFN0DF163MaxPn)//�����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF165      ADDR_AFN0DF164+((6+(4*(1+TMaxTariff)))*AFN0DF164MaxPn)//������1�����޹�����ʾֵ���ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF166      ADDR_AFN0DF165+((6+(4*(1+TMaxTariff)))*AFN0DF165MaxPn)//������2�����޹�����ʾֵ���ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF167      ADDR_AFN0DF166+((6+(4*(1+TMaxTariff)))*AFN0DF166MaxPn)//������3�����޹�����ʾֵ���ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF168      ADDR_AFN0DF167+((6+(4*(1+TMaxTariff)))*AFN0DF167MaxPn)//������4�����޹�����ʾֵ���ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF185      ADDR_AFN0DF168+((6+(4*(1+TMaxTariff)))*AFN0DF168MaxPn)//�����������й��������������ʱ�䣨�ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF186      ADDR_AFN0DF185+((6+(7*(1+TMaxTariff)))*AFN0DF185MaxPn)//�����������޹��������������ʱ�䣨�ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF187      ADDR_AFN0DF186+((6+(7*(1+TMaxTariff)))*AFN0DF186MaxPn)//�����㷴���й��������������ʱ�䣨�ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF188      ADDR_AFN0DF187+((6+(7*(1+TMaxTariff)))*AFN0DF187MaxPn)//�����㷴���޹��������������ʱ�䣨�ܡ�����1��M��
//�ն���
#define  ADDR_AFN0DF209      ADDR_AFN0DF188+((6+(7*(1+TMaxTariff)))*AFN0DF188MaxPn)//��������ܱ�Զ�̿���ͨ�ϵ�״̬����¼
//�ն���
#define  ADDR_AFN0DF210      ADDR_AFN0DF209+((5+13)*AFN0DF209MaxPn)//��������ܱ����õ���Ϣ
//�ն���
#define  ADDR_AFN0DF211      ADDR_AFN0DF210+(41*AFN0DF210MaxPn)//�����������ѹ��������ƽ���95%����ֵ
//�ն���
#define  ADDR_AFN0DF246      ADDR_AFN0DF211+(4*AFN0DF211MaxPn)//�ն�������¼����(������)
//�����ܴ���	BCD	3
//��1��ͣ�ϵ��¼�ͣ��ʱ��	����¼A.15	5
//��1��ͣ�ϵ��¼��ϵ�ʱ��	����¼A.15	5
//����	����	����
//��10��ͣ�ϵ��¼�ͣ��ʱ��	����¼A.15	5
//��10��ͣ�ϵ��¼��ϵ�ʱ��	����¼A.15	5
#define  ADDR_AFN0DF246Pn     ADDR_AFN0DF246+(103*F246MaxPn)//�ն�������¼����ӳ���Pn��
//2*F246MaxPn�ֽ�
#define  ADDR_DATECONGEAL_BASEEND     (ADDR_AFN0DF246Pn+(2*F246MaxPn))


	#if (USER/100)==5//�Ϻ�
#define  ADDR_AFN0DF241      ADDR_DATECONGEAL_BASEEND//�ն����¼��վ����(p0)
//4BYTE �յ�¼����n
#define  ADDR_AFN0DF243      ADDR_AFN0DF241+4//�ն����ź�ǿ��(p0)
//1BYTE 
#define LEN_DATECONGEAL     (ADDR_AFN0DF243+1)//ÿ���ն��������ܳ���
	#else
#define LEN_DATECONGEAL     (ADDR_DATECONGEAL_BASEEND)//ÿ���ն��������ܳ���
	#endif
#define ADDR_DATECONGEAL_END     (ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*MaxDateCongeal))




//��2���ݳ����ն���
#define ADDR_RMDATECONGEAL_START     ADDR_DATECONGEAL_END//�����ն��Ὺʼ��ַ
//3BYTE ����ʱ��(������)
#define ADDR_RMDATECONGEALFLAG     3//�����ն����־
//1byte �����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
#define  ADDR_AFN0DF9     ADDR_RMDATECONGEALFLAG+1//������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//�����ն���
#define  ADDR_AFN0DF10      ADDR_AFN0DF9+((6+(17*(1+TMaxTariff)))* AFN0DF9MaxPn)//�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//�����ն���
#define  ADDR_AFN0DF11      ADDR_AFN0DF10+((6+(17*(1+TMaxTariff)))*AFN0DF10MaxPn)//������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//�����ն���
#define  ADDR_AFN0DF12      ADDR_AFN0DF11+((6+(17*(1+TMaxTariff)))*AFN0DF11MaxPn)//�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//�����ն���
#define  ADDR_AFN0DF169      ADDR_AFN0DF12+((6+(17*(1+TMaxTariff)))*AFN0DF12MaxPn)//�����������й�����ʾֵ���ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF170      ADDR_AFN0DF169+((6+(5*(1+TMaxTariff)))*AFN0DF169MaxPn)//�����������޹�������޹�1������ʾֵ���ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF171      ADDR_AFN0DF170+((6+(4*(1+TMaxTariff)))*AFN0DF170MaxPn)//�����㷴���й�����ʾֵ���ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF172      ADDR_AFN0DF171+((6+(5*(1+TMaxTariff)))*AFN0DF171MaxPn)//�����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF173      ADDR_AFN0DF172+((6+(4*(1+TMaxTariff)))*AFN0DF172MaxPn)//������1�����޹�����ʾֵ���ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF174      ADDR_AFN0DF173+((6+(4*(1+TMaxTariff)))*AFN0DF173MaxPn)//������2�����޹�����ʾֵ���ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF175      ADDR_AFN0DF174+((6+(4*(1+TMaxTariff)))*AFN0DF174MaxPn)//������3�����޹�����ʾֵ���ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF176      ADDR_AFN0DF175+((6+(4*(1+TMaxTariff)))*AFN0DF175MaxPn)//������4�����޹�����ʾֵ���ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF189      ADDR_AFN0DF176+((6+(4*(1+TMaxTariff)))*AFN0DF176MaxPn)//�����������й��������������ʱ�䣨�ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF190      ADDR_AFN0DF189+((6+(7*(1+TMaxTariff)))*AFN0DF189MaxPn)//�����������޹��������������ʱ�䣨�ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF191      ADDR_AFN0DF190+((6+(7*(1+TMaxTariff)))*AFN0DF190MaxPn)//�����㷴���й��������������ʱ�䣨�ܡ�����1��M��
//�����ն���
#define  ADDR_AFN0DF192      ADDR_AFN0DF191+((6+(7*(1+TMaxTariff)))*AFN0DF191MaxPn)//�����㷴���޹��������������ʱ�䣨�ܡ�����1��M��
//�����ն���

#define LEN_RMDATECONGEAL     ( ADDR_AFN0DF192+((6+(7*(1+TMaxTariff)))*AFN0DF192MaxPn))//(( ADDR_AFN0DF192+((6+(7*(1+TMaxTariff)))*MAX_POINT))-(RMDATECONGEAL_START))//ÿ�γ����ն��������ܳ���
#define ADDR_RMDATECONGEAL_END     ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*MaxRMDateCongeal)


//��2�����¶���
#define ADDR_MONTHCONGEAL_START     ADDR_RMDATECONGEAL_END//�¶��Ὺʼ��ַ
//2byte ����ʱ��(��ʱ�����µ���1��)
#define ADDR_MONTHCONGEALFLAG     2//�¶����־
//1byte �����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
#define  ADDR_AFN0DF17     ADDR_MONTHCONGEALFLAG+1//������������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//�¶���
#define  ADDR_AFN0DF18      ADDR_AFN0DF17+((6+(17*(1+TMaxTariff)))*AFN0DF17MaxPn)//�����㷴����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//�¶���
#define  ADDR_AFN0DF19      ADDR_AFN0DF18+((6+(17*(1+TMaxTariff)))*AFN0DF18MaxPn)//������������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//�¶���
#define  ADDR_AFN0DF20      ADDR_AFN0DF19+((6+(14*(1+TMaxTariff)))*AFN0DF19MaxPn)//�����㷴����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//�¶���
#define  ADDR_AFN0DF21      ADDR_AFN0DF20+((6+(14*(1+TMaxTariff)))*AFN0DF20MaxPn)//�����������й����������ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF22      ADDR_AFN0DF21+((1+(4*(1+TMaxTariff)))*AFN0DF21MaxPn)//�����������޹����������ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF23      ADDR_AFN0DF22+((1+(4*(1+TMaxTariff)))*AFN0DF22MaxPn)//�����㷴���й����������ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF24      ADDR_AFN0DF23+((1+(4*(1+TMaxTariff)))*AFN0DF23MaxPn)//�����㷴���޹����������ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF33      ADDR_AFN0DF24+((1+(4*(1+TMaxTariff)))*AFN0DF24MaxPn)//���������ܼ���������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
//�¶���
#define  ADDR_AFN0DF34      ADDR_AFN0DF33+(32*AFN0DF33MaxPn)//���������ܼ������й��������������ʱ��
//�¶���
#define  ADDR_AFN0DF35      ADDR_AFN0DF34+(24*AFN0DF34MaxPn)//�������µ�ѹͳ������
//�¶���
	#if (USER/100)==11//����
#define  ADDR_AFN0DF36      ADDR_AFN0DF35+((66+27)*AFN0DF35MaxPn_ReadMeter)//�������ղ�ƽ���Խ���ۼ�ʱ��
	#else
#define  ADDR_AFN0DF36      ADDR_AFN0DF35+((66+27)*AFN0DF35MaxPn)//�������²�ƽ���Խ���ۼ�ʱ��
	#endif
//�¶���
#define  ADDR_AFN0DF37      ADDR_AFN0DF36+(16*AFN0DF36MaxPn)//�������µ���Խ��ͳ��
//�¶���
#define  ADDR_AFN0DF38      ADDR_AFN0DF37+(38*AFN0DF37MaxPn)//�����������ڹ���Խ���ۼ�ʱ��
//�¶���
#define  ADDR_AFN0DF39      ADDR_AFN0DF38+(4*AFN0DF38MaxPn)//�������¸�������ͳ��
//�¶���
#define  ADDR_AFN0DF44      ADDR_AFN0DF39+(12*AFN0DF39MaxPn)//�������¹������������ۼ�ʱ��
//�¶���
#define  ADDR_AFN0DF46      ADDR_AFN0DF44+(6*AFN0DF44MaxPn)//�������¶���ͭ�������й�����ʾֵ
//�¶���
#define  ADDR_AFN0DF51      ADDR_AFN0DF46+(10*AFN0DF46MaxPn)//�ն��¹���ʱ�䡢�¸�λ�ۼƴ���
//�¶���
#define  ADDR_AFN0DF52      ADDR_AFN0DF51+4//�ն��¿���ͳ������
//�¶���
#define  ADDR_AFN0DF54      ADDR_AFN0DF52+4//�ն�����վ��ͨ������
//�¶���
#define  ADDR_AFN0DF60      ADDR_AFN0DF54+4//�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//�¶���
#define  ADDR_AFN0DF61      ADDR_AFN0DF60+(12*AFN0DF60MaxPn)//�ܼ������ۼ��й����������ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF62      ADDR_AFN0DF61+((1+(4*(1+TMaxTariff)))*AFN0DF61MaxPn)//�ܼ������ۼ��޹����������ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF65      ADDR_AFN0DF62+((1+(4*(1+TMaxTariff)))*AFN0DF62MaxPn)//�ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
//�¶���
#define  ADDR_AFN0DF66      ADDR_AFN0DF65+(6*AFN0DF65MaxPn)//�ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
//�¶���
#define  ADDR_AFN0DF130      ADDR_AFN0DF66+(6*AFN0DF66MaxPn)//ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//�¶���
#define  ADDR_AFN0DF157      ADDR_AFN0DF130+(14*AFN0DF130MaxPn)//�������¶�����������й�����ʾֵ
//�¶���
#define  ADDR_AFN0DF158      ADDR_AFN0DF157+(20*AFN0DF157MaxPn)//�������¶�����������޹�����ʾֵ
//�¶���
#define  ADDR_AFN0DF159      ADDR_AFN0DF158+(17*AFN0DF158MaxPn)//�������¶�����෴���й�����ʾֵ
//�¶���
#define  ADDR_AFN0DF160      ADDR_AFN0DF159+(20*AFN0DF159MaxPn)//�������¶�����෴���޹�����ʾֵ
//�¶���
#define  ADDR_AFN0DF177      ADDR_AFN0DF160+(17*AFN0DF160MaxPn)//�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF178      ADDR_AFN0DF177+((6+(5*(1+TMaxTariff)))*AFN0DF177MaxPn)//�����������޹�������޹�1������ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF179      ADDR_AFN0DF178+((6+(4*(1+TMaxTariff)))*AFN0DF178MaxPn)//�����㷴���й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF180      ADDR_AFN0DF179+((6+(5*(1+TMaxTariff)))*AFN0DF179MaxPn)//�����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF181      ADDR_AFN0DF180+((6+(4*(1+TMaxTariff)))*AFN0DF180MaxPn)//������1�����޹�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF182      ADDR_AFN0DF181+((6+(4*(1+TMaxTariff)))*AFN0DF181MaxPn)//������2�����޹�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF183      ADDR_AFN0DF182+((6+(4*(1+TMaxTariff)))*AFN0DF182MaxPn)//������3�����޹�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF184      ADDR_AFN0DF183+((6+(4*(1+TMaxTariff)))*AFN0DF183MaxPn)//������4�����޹�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF193      ADDR_AFN0DF184+((6+(4*(1+TMaxTariff)))*AFN0DF184MaxPn)//�����������й��������������ʱ�䣨�ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF194      ADDR_AFN0DF193+((6+(7*(1+TMaxTariff)))*AFN0DF193MaxPn)//�����������޹��������������ʱ�䣨�ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF195      ADDR_AFN0DF194+((6+(7*(1+TMaxTariff)))*AFN0DF194MaxPn)//�����㷴���й��������������ʱ�䣨�ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF196      ADDR_AFN0DF195+((6+(7*(1+TMaxTariff)))*AFN0DF195MaxPn)//�����㷴���޹��������������ʱ�䣨�ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF201      ADDR_AFN0DF196+((6+(7*(1+TMaxTariff)))*AFN0DF196MaxPn)//�������1ʱ�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF202      ADDR_AFN0DF201+((6+(5*(1+TMaxTariff)))*AFN0DF201MaxPn)//�������2ʱ�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF203      ADDR_AFN0DF202+((6+(5*(1+TMaxTariff)))*AFN0DF202MaxPn)//�������3ʱ�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF204      ADDR_AFN0DF203+((6+(5*(1+TMaxTariff)))*AFN0DF203MaxPn)//�������4ʱ�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF205      ADDR_AFN0DF204+((6+(5*(1+TMaxTariff)))*AFN0DF204MaxPn)//�������5ʱ�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF206      ADDR_AFN0DF205+((6+(5*(1+TMaxTariff)))*AFN0DF205MaxPn)//�������6ʱ�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF207      ADDR_AFN0DF206+((6+(5*(1+TMaxTariff)))*AFN0DF206MaxPn)//�������7ʱ�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF208      ADDR_AFN0DF207+((6+(5*(1+TMaxTariff)))*AFN0DF207MaxPn)//�������8ʱ�����������й�����ʾֵ���ܡ�����1��M��
//�¶���
#define  ADDR_AFN0DF212      ADDR_AFN0DF208+((6+(5*(1+TMaxTariff)))*AFN0DF208MaxPn)//�����������ѹ��������ƽ���95%����ֵ
//�¶���
#define  ADDR_AFN0DF213      ADDR_AFN0DF212+(4*AFN0DF212MaxPn)//��������ܱ��ز���������ʱ��
//�¶���
#define  ADDR_AFN0DF214      ADDR_AFN0DF213+((5+18)*AFN0DF213MaxPn)//��������ܱ�����޸Ĵ�����ʱ��
//�¶���
#define  ADDR_AFN0DF215      ADDR_AFN0DF214+((5+24)*AFN0DF214MaxPn)//��������ܱ����õ���Ϣ
//�¶���
#define  ADDR_AFN0DF216      ADDR_AFN0DF215+((5+36)*AFN0DF215MaxPn)//��������ܱ������Ϣ
//�ն˳���ʱ��	����¼A.15	��ʱ������	5
//������M	����¼A.8	BIN	2
//�ѽ��й��ܵ���	����¼A.14	kWh	5
//�ѽ����1�����й��ܵ���	����¼A.14	kWh	5
//����	����¼A.14	kWh	5
//�ѽ����M�����й��ܵ���	����¼A.14	kWh	5
//δ���й��ܵ���	����¼A.14	kWh	5
//δ�����1�����й��ܵ���	����¼A.14	kWh	5
//����	����¼A.14	kWh	5
//δ�����M�����й��ܵ���	����¼A.14	kWh	5
//���ϵ���	����¼A.14	kWh	5

#define  ADDR_MONTHCONGEAL_BASEEND     (ADDR_AFN0DF216+((6+(10*(1+TMaxTariff))+5)*AFN0DF216MaxPn))


	#if (USER/100)==5//�Ϻ�
#define  ADDR_AFN0DF242      ADDR_MONTHCONGEAL_BASEEND//�¶����¼��վ����(p0)
//4byte �µ�¼����n

#define LEN_MONTHCONGEAL     (ADDR_AFN0DF242+4)//ÿ���¶��������ܳ���
	#else
#define LEN_MONTHCONGEAL     (ADDR_MONTHCONGEAL_BASEEND)//ÿ���¶��������ܳ���
	#endif
#define ADDR_MONTHCONGEAL_END     ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*MaxMonthCongeal)






//��2��������
#define ADDR_CURVECONGEAL_START     ADDR_MONTHCONGEAL_END
//5BYTE����ʱ��(��ʱ������)
#define ADDR_CURVECONGEALFLAG     5//���߶����־
//1byte �����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
#define  ADDR_AFN0DF73     ADDR_CURVECONGEALFLAG+1//�ܼ����й���������
//
#define  ADDR_AFN0DF74      ADDR_AFN0DF73+(2*AFN0DF73MaxPn)//�ܼ����޹���������
//
#define  ADDR_AFN0DF75      ADDR_AFN0DF74+(2*AFN0DF74MaxPn)//�ܼ����й�����������
//
#define  ADDR_AFN0DF76      ADDR_AFN0DF75+(4*AFN0DF75MaxPn)//�ܼ����޹�����������
//
#define  ADDR_AFN0DF81      ADDR_AFN0DF76+(4*AFN0DF76MaxPn)//�������й���������
//
#define  ADDR_AFN0DF82      ADDR_AFN0DF81+(3*AFN0DF81MaxPn)//������A���й���������
//
#define  ADDR_AFN0DF83      ADDR_AFN0DF82+(3*AFN0DF82MaxPn)//������B���й���������
//
#define  ADDR_AFN0DF84      ADDR_AFN0DF83+(3*AFN0DF83MaxPn)//������C���й���������
//
#define  ADDR_AFN0DF85      ADDR_AFN0DF84+(3*AFN0DF84MaxPn)//�������޹���������
//
#define  ADDR_AFN0DF86      ADDR_AFN0DF85+(3*AFN0DF85MaxPn)//������A�޹���������
//
#define  ADDR_AFN0DF87      ADDR_AFN0DF86+(3*AFN0DF86MaxPn)//������B�޹���������
//
#define  ADDR_AFN0DF88      ADDR_AFN0DF87+(3*AFN0DF87MaxPn)//������C�޹���������
//
#define  ADDR_AFN0DF89      ADDR_AFN0DF88+(3*AFN0DF88MaxPn)//������A���ѹ����
//
#define  ADDR_AFN0DF90      ADDR_AFN0DF89+(2*AFN0DF89MaxPn)//������B���ѹ����
//
#define  ADDR_AFN0DF91      ADDR_AFN0DF90+(2*AFN0DF90MaxPn)//������C���ѹ����
//
#define  ADDR_AFN0DF92      ADDR_AFN0DF91+(2*AFN0DF91MaxPn)//������A���������
//
#define  ADDR_AFN0DF93      ADDR_AFN0DF92+(3*AFN0DF92MaxPn)//������B���������
//
#define  ADDR_AFN0DF94      ADDR_AFN0DF93+(3*AFN0DF93MaxPn)//������C���������
//
#define  ADDR_AFN0DF95      ADDR_AFN0DF94+(3*AFN0DF94MaxPn)//�����������������
//
#define  ADDR_AFN0DF97      ADDR_AFN0DF95+(3*AFN0DF95MaxPn)//�����������й��ܵ���������
//
#define  ADDR_AFN0DF98      ADDR_AFN0DF97+(4*AFN0DF97MaxPn)//�����������޹��ܵ���������
//
#define  ADDR_AFN0DF99      ADDR_AFN0DF98+(4*AFN0DF98MaxPn)//�����㷴���й��ܵ���������
//
#define  ADDR_AFN0DF100      ADDR_AFN0DF99+(4*AFN0DF99MaxPn)//�����㷴���޹��ܵ���������
//
#define  ADDR_AFN0DF101      ADDR_AFN0DF100+(4*AFN0DF100MaxPn)//�����������й��ܵ���ʾֵ
//
#define  ADDR_AFN0DF102      ADDR_AFN0DF101+(4*AFN0DF101MaxPn)//�����������޹��ܵ���ʾֵ
//
#define  ADDR_AFN0DF103      ADDR_AFN0DF102+(4*AFN0DF102MaxPn)//�����㷴���й��ܵ���ʾֵ
//
#define  ADDR_AFN0DF104      ADDR_AFN0DF103+(4*AFN0DF103MaxPn)//�����㷴���޹��ܵ���ʾֵ
//
#define  ADDR_AFN0DF105      ADDR_AFN0DF104+(4*AFN0DF104MaxPn)//�������ܹ�������
//
#define  ADDR_AFN0DF106      ADDR_AFN0DF105+(2*AFN0DF105MaxPn)//������A�๦������
//
#define  ADDR_AFN0DF107      ADDR_AFN0DF106+(2*AFN0DF106MaxPn)//������B�๦������
//
#define  ADDR_AFN0DF108      ADDR_AFN0DF107+(2*AFN0DF107MaxPn)//������C�๦������
//
#define  ADDR_AFN0DF109      ADDR_AFN0DF108+(2*AFN0DF108MaxPn)//�������ѹ��λ������
//
#define  ADDR_AFN0DF110      ADDR_AFN0DF109+(6*AFN0DF109MaxPn)//�����������λ������
//
#define  ADDR_AFN0DF138      ADDR_AFN0DF110+(6*AFN0DF110MaxPn)//ֱ��ģ������������
//
#define  ADDR_AFN0DF145      ADDR_AFN0DF138+(2*AFN0DF138MaxPn)//������1�����޹��ܵ���ʾֵ����
//
#define  ADDR_AFN0DF146      ADDR_AFN0DF145+(4*AFN0DF145MaxPn)//������4�����޹��ܵ���ʾֵ����
//
#define  ADDR_AFN0DF147      ADDR_AFN0DF146+(4*AFN0DF146MaxPn)//������2�����޹��ܵ���ʾֵ����
//
#define  ADDR_AFN0DF148      ADDR_AFN0DF147+(4*AFN0DF147MaxPn)//������3�����޹��ܵ���ʾֵ����
//
//#define  ADDR_AFN0DF217      ADDR_AFN0DF148+(4*AFN0DF148MaxPn)//̨�����г����ز����ڵ����������
//
//#define  ADDR_AFN0DF218      ADDR_AFN0DF217+1//̨�����г����ز����ڵ�ɫ��������
//
//F219 �����������������
#define  ADDR_KeyUserPnListCongeal     (ADDR_AFN0DF148+(4*AFN0DF148MaxPn))//����չ:���г����ص㻧���߶���Pn�б�
//�ص㻧���߶���������1-MaxKeyUser��Ӧ��ʵ�ʲ������,0��0xffΪ��Ч

	#if (ADDR_KeyUserPnListCongeal+(2*MaxKeyUser))<1024
#define LEN_CURVECONGEAL     1024//��С1K(��ΪFLASH����)
	#else
#define LEN_CURVECONGEAL     (ADDR_KeyUserPnListCongeal+(2*MaxKeyUser))
	#endif

#define ADDR_CURVECONGEAL_BASEEND     ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*MaxCurveCongeal)//ͨ�����߽���


#if (USER/100)==5//�Ϻ�
#define ADDR_AFN0CF242_Next     ADDR_CURVECONGEAL_BASEEND//���յ�¼��վ��Ϣ
//6BYTE ��¼ʱ��1 ����¼A.1(���ֵ0xff)
//...
//6BYTE ��¼ʱ��n ����¼A.1(���ֵ0xff)
#define AFN0CF242MaxCount     256//���յ�¼��վ��Ϣ����¼����
#define ADDR_AFN0CF243     ADDR_AFN0CF242_Next+(6*AFN0CF242MaxCount)//�����ź�ǿ��
//00:00�ź�ǿ��	BIN	1(���ֵ0xff)
//01:00�ź�ǿ��	BIN	1(���ֵ0xff)
//����	BIN	1
//22:00�ź�ǿ��	BIN	1(���ֵ0xff)
//23:00�ź�ǿ��	BIN	1(���ֵ0xff)
#define ADDR_CURVECONGEAL_ExtStart     ADDR_AFN0CF243+24//��չ���߿�ʼ
//5BYTE����ʱ��(��ʱ������)
#define ADDR_CURVECONGEALFLAG_Ext     5//���߶����־
//1byte �����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
#define  ADDR_AFN0DF235     ADDR_CURVECONGEALFLAG_Ext+1
//2byte �ն�AB�ߵ�ѹ����
#define  ADDR_AFN0DF236     ADDR_AFN0DF235+2
//2byte �ն�BC�ߵ�ѹ����
#define  ADDR_AFN0DF237     ADDR_AFN0DF236+2
//2byte �ն�CA�ߵ�ѹ����

#define LEN_CURVECONGEAL_Ext     (ADDR_AFN0DF237+2)
#define ADDR_CURVECONGEAL_ExtEND     ADDR_CURVECONGEAL_ExtStart+(LEN_CURVECONGEAL_Ext*MaxCurveCongeal_Ext)//��չ���߽���
#else//#if (USER/100)==5//�Ϻ�
#define ADDR_CURVECONGEAL_ExtEND     ADDR_CURVECONGEAL_BASEEND//��չ���߽���
#endif


#define ADDR_CURVECONGEAL_END     ADDR_CURVECONGEAL_ExtEND//���߽���







#define ADDR_AFN0D_End     (ADDR_CURVECONGEAL_END)//��2���ݽ�����ַ

#if (ADDR_AFN0D_End-ADDR_AFN0D_Start)>LEN_Terminal_EFLASH_AFN0D
	#error (ADDR_AFN0D_End-ADDR_AFN0D_Start)>LEN_Terminal_EFLASH_AFN0D
#endif

#endif


