
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"


#if (USER/100)==14//����

//���ܱ�ʧѹ�¼�
#define Init_Enable_Event_3000     0//�¼���Ч��ʶbool
#define Init_Report_Event_3000     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�Ƿѹ�¼�
#define Init_Enable_Event_3001     0//�¼���Ч��ʶbool
#define Init_Report_Event_3001     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��ѹ�¼�
#define Init_Enable_Event_3002     0//�¼���Ч��ʶbool
#define Init_Report_Event_3002     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����¼�
#define Init_Enable_Event_3003     0//�¼���Ч��ʶbool
#define Init_Report_Event_3003     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ʧ���¼�
#define Init_Enable_Event_3004     0//�¼���Ч��ʶbool
#define Init_Report_Event_3004     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����¼�
#define Init_Enable_Event_3005     0//�¼���Ч��ʶbool
#define Init_Report_Event_3005     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����¼�
#define Init_Enable_Event_3006     0//�¼���Ч��ʶbool
#define Init_Report_Event_3006     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��������¼�
#define Init_Enable_Event_3007     0//�¼���Ч��ʶbool
#define Init_Report_Event_3007     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����¼�
#define Init_Enable_Event_3008     0//�¼���Ч��ʶbool
#define Init_Report_Event_3008     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ������й����������¼�
#define Init_Enable_Event_3009     0//�¼���Ч��ʶbool
#define Init_Report_Event_3009     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ����й����������¼�
#define Init_Enable_Event_300A     0//�¼���Ч��ʶbool
#define Init_Report_Event_300A     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��޹����������¼�
#define Init_Enable_Event_300B     0//�¼���Ч��ʶbool
#define Init_Report_Event_300B     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��������������¼�
#define Init_Enable_Event_300C     0//�¼���Ч��ʶbool
#define Init_Report_Event_300C     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ȫʧѹ�¼�
#define Init_Enable_Event_300D     0//�¼���Ч��ʶbool
#define Init_Report_Event_300D     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����Դ�����¼�
#define Init_Enable_Event_300E     0//�¼���Ч��ʶbool
#define Init_Report_Event_300E     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��ѹ�������¼�
#define Init_Enable_Event_300F     1//�¼���Ч��ʶbool
#define Init_Report_Event_300F     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����������¼�
#define Init_Enable_Event_3010     0//�¼���Ч��ʶbool
#define Init_Report_Event_3010     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����¼�
#define Init_Enable_Event_3011     0//�¼���Ч��ʶbool
#define Init_Report_Event_3011     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ����¼�
#define Init_Enable_Event_3012     0//�¼���Ч��ʶbool
#define Init_Report_Event_3012     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ������¼�
#define Init_Enable_Event_3013     0//�¼���Ч��ʶbool
#define Init_Report_Event_3013     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ����������¼�
#define Init_Enable_Event_3014     0//�¼���Ч��ʶbool
#define Init_Report_Event_3014     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��¼������¼�
#define Init_Enable_Event_3015     0//�¼���Ч��ʶbool
#define Init_Report_Event_3015     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�Уʱ�¼�
#define Init_Enable_Event_3016     0//�¼���Ч��ʶbool
#define Init_Report_Event_3016     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ʱ�α����¼�
#define Init_Enable_Event_3017     0//�¼���Ч��ʶbool
#define Init_Report_Event_3017     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ʱ�������¼�
#define Init_Enable_Event_3018     0//�¼���Ч��ʶbool
#define Init_Report_Event_3018     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ������ձ���¼�
#define Init_Enable_Event_3019     0//�¼���Ч��ʶbool
#define Init_Report_Event_3019     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����ձ���¼�
#define Init_Enable_Event_301A     0//�¼���Ч��ʶbool
#define Init_Report_Event_301A     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ����¼�
#define Init_Enable_Event_301B     0//�¼���Ч��ʶbool
#define Init_Report_Event_301B     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ���ť���¼�
#define Init_Enable_Event_301C     0//�¼���Ч��ʶbool
#define Init_Report_Event_301C     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��ѹ��ƽ���¼�
#define Init_Enable_Event_301D     0//�¼���Ч��ʶbool
#define Init_Report_Event_301D     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ������ƽ���¼�
#define Init_Enable_Event_301E     0//�¼���Ч��ʶbool
#define Init_Report_Event_301E     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ���բ�¼�
#define Init_Enable_Event_301F     0//�¼���Ч��ʶbool
#define Init_Report_Event_301F     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��բ�¼�
#define Init_Enable_Event_3020     0//�¼���Ч��ʶbool
#define Init_Report_Event_3020     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ڼ��ձ���¼�
#define Init_Enable_Event_3021     0//�¼���Ч��ʶbool
#define Init_Report_Event_3021     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��й���Ϸ�ʽ����¼�
#define Init_Enable_Event_3022     0//�¼���Ч��ʶbool
#define Init_Report_Event_3022     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��޹���Ϸ�ʽ����¼�
#define Init_Enable_Event_3023     0//�¼���Ч��ʶbool
#define Init_Report_Event_3023     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ���ʲ��������¼�
#define Init_Enable_Event_3024     0//�¼���Ч��ʶbool
#define Init_Report_Event_3024     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ���ݱ����¼�
#define Init_Enable_Event_3025     0//�¼���Ч��ʶbool
#define Init_Report_Event_3025     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ���Կ�����¼�
#define Init_Enable_Event_3026     0//�¼���Ч��ʶbool
#define Init_Report_Event_3026     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��쳣�忨�¼�
#define Init_Enable_Event_3027     0//�¼���Ч��ʶbool
#define Init_Report_Event_3027     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ����¼
#define Init_Enable_Event_3028     0//�¼���Ч��ʶbool
#define Init_Report_Event_3028     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��˷Ѽ�¼
#define Init_Enable_Event_3029     0//�¼���Ч��ʶbool
#define Init_Report_Event_3029     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�㶨�ų������¼�
#define Init_Enable_Event_302A     0//�¼���Ч��ʶbool
#define Init_Report_Event_302A     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��ɿ��������¼�
#define Init_Enable_Event_302B     0//�¼���Ч��ʶbool
#define Init_Report_Event_302B     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ��Դ�쳣�¼�
#define Init_Enable_Event_302C     0//�¼���Ч��ʶbool
#define Init_Report_Event_302C     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�������ز�ƽ���¼�	
#define Init_Enable_Event_302D     0//�¼���Ч��ʶbool
#define Init_Report_Event_302D     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ʱ�ӹ����¼�
#define Init_Enable_Event_302E     0//�¼���Ч��ʶbool
#define Init_Report_Event_302E     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ����оƬ�����¼�
#define Init_Enable_Event_302F     0//�¼���Ч��ʶbool
#define Init_Report_Event_302F     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//ͨ��ģ�����¼�
#define Init_Enable_Event_3030     0//�¼���Ч��ʶbool
#define Init_Report_Event_3030     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն˳�ʼ���¼�
#define Init_Enable_Event_3100     0//�¼���Ч��ʶbool
#define Init_Report_Event_3100     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն˰汾����¼�
#define Init_Enable_Event_3101     0//�¼���Ч��ʶbool
#define Init_Report_Event_3101     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն�״̬����λ�¼�
#define Init_Enable_Event_3104     0//�¼���Ч��ʶbool
#define Init_Report_Event_3104     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ʱ�ӳ����¼�
#define Init_Enable_Event_3105     0//�¼���Ч��ʶbool
#define Init_Report_Event_3105     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն�ͣ/�ϵ��¼�
#define Init_Enable_Event_3106     0//�¼���Ч��ʶbool
#define Init_Report_Event_3106     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն�ֱ��ģ����Խ�����¼�
#define Init_Enable_Event_3107     0//�¼���Ч��ʶbool
#define Init_Report_Event_3107     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն�ֱ��ģ����Խ�����¼�
#define Init_Enable_Event_3108     0//�¼���Ч��ʶbool
#define Init_Report_Event_3108     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն���Ϣ��֤�����¼�
#define Init_Enable_Event_3109     0//�¼���Ч��ʶbool
#define Init_Report_Event_3109     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�豸���ϼ�¼
#define Init_Enable_Event_310A     0//�¼���Ч��ʶbool
#define Init_Report_Event_310A     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ʾ���½��¼�
#define Init_Enable_Event_310B     0//�¼���Ч��ʶbool
#define Init_Report_Event_310B     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�����������¼�
#define Init_Enable_Event_310C     0//�¼���Ч��ʶbool
#define Init_Report_Event_310C     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����¼�
#define Init_Enable_Event_310D     0//�¼���Ч��ʶbool
#define Init_Report_Event_310D     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�ͣ���¼�
#define Init_Enable_Event_310E     0//�¼���Ч��ʶbool
#define Init_Report_Event_310E     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն˳���ʧ���¼�
#define Init_Enable_Event_310F     0//�¼���Ч��ʶbool
#define Init_Report_Event_310F     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//��ͨ�����������¼�
#define Init_Enable_Event_3110     0//�¼���Ч��ʶbool
#define Init_Report_Event_3110     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//����δ֪���ܱ��¼�
#define Init_Enable_Event_3111     0//�¼���Ч��ʶbool
#define Init_Report_Event_3111     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//��̨�����ܱ��¼�
#define Init_Enable_Event_3112     0//�¼���Ч��ʶbool
#define Init_Report_Event_3112     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն˶�ʱ�¼�
#define Init_Enable_Event_3114     0//�¼���Ч��ʶbool
#define Init_Report_Event_3114     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//ң����բ��¼
#define Init_Enable_Event_3115     0//�¼���Ч��ʶbool
#define Init_Report_Event_3115     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�й��ܵ������Խ���¼���¼
#define Init_Enable_Event_3116     0//�¼���Ч��ʶbool
#define Init_Report_Event_3116     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�����·����״̬��λ�¼���¼
#define Init_Enable_Event_3117     0//�¼���Ч��ʶbool
#define Init_Report_Event_3117     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն˱�̼�¼
#define Init_Enable_Event_3118     0//�¼���Ч��ʶbool
#define Init_Report_Event_3118     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն˵�����·�쳣�¼�
#define Init_Enable_Event_3119     0//�¼���Ч��ʶbool
#define Init_Report_Event_3119     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ�����״̬�л��¼�
#define Init_Enable_Event_311A     0//�¼���Ч��ʶbool
#define Init_Report_Event_311A     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�ն˶Ե��Уʱ��¼
#define Init_Enable_Event_311B     0//�¼���Ч��ʶbool
#define Init_Report_Event_311B     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//���ܱ����ݱ����ؼ�¼
#define Init_Enable_Event_311C     0//�¼���Ч��ʶbool
#define Init_Report_Event_311C     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//������բ��¼
#define Init_Enable_Event_3200     0//�¼���Ч��ʶbool
#define Init_Report_Event_3200     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//�����բ��¼
#define Init_Enable_Event_3201     0//�¼���Ч��ʶbool
#define Init_Report_Event_3201     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//����������ü�¼
#define Init_Enable_Event_3202     0//�¼���Ч��ʶbool
#define Init_Report_Event_3202     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��
//��ظ澯�¼���¼
#define Init_Enable_Event_3203     0//�¼���Ч��ʶbool
#define Init_Report_Event_3203     0//���ϱ���0��,�¼������ϱ���1�����¼��ָ��ϱ���2�����¼������ָ����ϱ���3��



//---�¼���---
const u8 InitData_3000[]=//���ܱ�ʧѹ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3000,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3000,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200�����¼�����ʱ�̵�ѹ
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200�����¼�����ʱ�̵���	
};
const u8 InitData_3000_5[]=//���ܱ�ʧѹ�¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	4,
//{
//��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
//	0x06,0xB4,//78%�αȵ�ѹ
	((MeterUBhex*78/100)>>8)&0xff,((MeterUBhex*78+50)/100)&0xff,//78%�αȵ�ѹ
//  ��ѹ�ָ�����  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
//	0x07,0x4E,//85%�αȵ�ѹ
	((MeterUBhex*85/100)>>8)&0xff,((MeterUBhex*85+50)/100)&0xff,//85%�αȵ�ѹ
//  ������������  double-long����λ��A�����㣺-4����
	DataType_double_long,
//	0,0,0,75,//0.5%�������������
	((MeterIBhex*50/100)>>24)&0xff,((MeterIBhex*50/100)>>16)&0xff,((MeterIBhex*50/100)>>8)&0xff,((MeterIBhex*50+50)/100)&0xff,//0.5%�������������
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3001[]=//���ܱ�Ƿѹ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3001,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3001,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200�����¼�����ʱ�̵�ѹ
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200�����¼�����ʱ�̵���	
};
const u8 InitData_3001_5[]=//���ܱ�Ƿѹ�¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
//	0x06,0xB4,//78%�αȵ�ѹ
	((MeterUBhex*78/100)>>8)&0xff,((MeterUBhex*78+50)/100)&0xff,//78%�αȵ�ѹ
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3002[]=//���ܱ��ѹ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3002,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3002,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200�����¼�����ʱ�̵�ѹ
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200�����¼�����ʱ�̵���	
};
const u8 InitData_3002_5[]=//���ܱ��ѹ�¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
//	0x0A,0x50,//120%�αȵ�ѹ
	((MeterUBhex*120/100)>>8)&0xff,((MeterUBhex*120+50)/100)&0xff,//120%�αȵ�ѹ
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3003[]=//���ܱ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3003,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3003,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200�����¼�����ʱ�̵�ѹ
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200�����¼�����ʱ�̵���	
};
const u8 InitData_3003_5[]=//���ܱ�����¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	3,
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
//	0x05,0x28,//60%�αȵ�ѹ
	((MeterUBhex*60/100)>>8)&0xff,((MeterUBhex*60+50)/100)&0xff,//60%�αȵ�ѹ
//  ������������  double-long����λ��A�����㣺-4����
	DataType_double_long,
//	0,0,0,75,//0.5%�������������
	((MeterIBhex*50/100)>>24)&0xff,((MeterIBhex*50/100)>>16)&0xff,((MeterIBhex*50/100)>>8)&0xff,((MeterIBhex*50+50)/100)&0xff,//0.5%�������������
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3004[]=//���ܱ�ʧ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3004,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3004,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200�����¼�����ʱ�̵�ѹ
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200�����¼�����ʱ�̵���	
};
const u8 InitData_3004_5[]=//���ܱ�ʧ���¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	4,
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
//	0x06,0x04,//70%�αȵ�ѹ
	((MeterUBhex*70/100)>>8)&0xff,((MeterUBhex*70+50)/100)&0xff,//70%�αȵ�ѹ
//  ������������  double-long����λ��A�����㣺-4����
	DataType_double_long,
//0,0,0,75,//0.5%�������������	
	((MeterIBhex*50/100)>>24)&0xff,((MeterIBhex*50/100)>>16)&0xff,((MeterIBhex*50/100)>>8)&0xff,((MeterIBhex*50+50)/100)&0xff,//0.5%�������������
//  �����ָ�����  double-long����λ��A�����㣺-4����
	DataType_double_long,
//	0,0,0x02,0xEE,//5%�������������
	((MeterIBhex*500/100)>>24)&0xff,((MeterIBhex*500/100)>>16)&0xff,((MeterIBhex*500/100)>>8)&0xff,((MeterIBhex*500+50)/100)&0xff,//0.5%�������������
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3005[]=//���ܱ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3005,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3005,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x00,0x22,0x00,//20002200�����¼�����ʱ�̵�ѹ
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200�����¼�����ʱ�̵���	
};
const u8 InitData_3005_5[]=//���ܱ�����¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	2,
//{
//  ������������  double-long����λ��A�����㣺-4����
	DataType_double_long,
//	0,1,0x19,0x40,//1.2Imax
	((MeterIMaxhex*120)>>24)&0xff,((MeterIMaxhex*120)>>16)&0xff,((MeterIMaxhex*120)>>8)&0xff,(MeterIMaxhex*120)&0xff,//1.2Imax

//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3006[]=//���ܱ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3006,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3006,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3006_5[]=//���ܱ�����¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	3,
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
//	0x05,0x28,//60%�αȵ�ѹ
	((MeterUBhex*60/100)>>8)&0xff,((MeterUBhex*60+50)/100)&0xff,//60%�αȵ�ѹ
//  ������������  double-long����λ��A�����㣺-4����
	DataType_double_long,
//	0,0,0,75,//0.5%�������������	
	((MeterIBhex*50/100)>>24)&0xff,((MeterIBhex*50/100)>>16)&0xff,((MeterIBhex*50/100)>>8)&0xff,((MeterIBhex*50+50)/100)&0xff,//0.5%�������������
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3007[]=//���ܱ��ʷ����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3007,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3007,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���	
};
const u8 InitData_3007_5[]=//���ܱ��ʷ����¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	2,
//{
//  �й����ʴ�������  double-long����λ��W�����㣺-1����
	DataType_double_long,
//	0,0,0,17,//0.5%�����������
	((MeterIBhex*MeterUBhex/20000)>>24)&0xff,((MeterIBhex*MeterUBhex/20000)>>16)&0xff,((MeterIBhex*MeterUBhex/20000)>>8)&0xff,((MeterIBhex*MeterUBhex+10000)/20000)&0xff,//0.5%�����������

//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3008[]=//���ܱ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3008,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3008,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3008_5[]=//���ܱ�����¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	2,
//{
//  �й����ʴ�������  double-long����λ��W�����㣺-1����
	DataType_double_long,
//	0,0,0x3D,0xE0,//1.2Imax��100%�αȵ�ѹ�µĵ����й�����
	((MeterIMaxhex*MeterUBhex*12/1000)>>24)&0xff,((MeterIMaxhex*MeterUBhex*12/1000)>>16)&0xff,((MeterIMaxhex*MeterUBhex*12/1000)>>8)&0xff,((MeterIMaxhex*MeterUBhex*12+500)/1000)&0xff,//1.2Imax��100%�αȵ�ѹ�µĵ����й�����

//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_3009[]=//���ܱ������й����������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3009,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3009,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3009_6[]=//���ܱ������й����������¼�-5
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��������  double-long-unsigned����λ��kW�����㣺-4����
	DataType_double_long_unsigned,
//	0,0,0xB9,0xA0,//1.2Imax��100%�αȵ�ѹ�µĺ����й�����
	((MeterIMaxhex*MeterUBhex*36/1000)>>24)&0xff,((MeterIMaxhex*MeterUBhex*36/1000)>>16)&0xff,((MeterIMaxhex*MeterUBhex*36/1000)>>8)&0xff,((MeterIMaxhex*MeterUBhex*36+500)/1000)&0xff,//1.2Imax��100%�αȵ�ѹ�µĺ����й�����

//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_300A[]=//���ܱ����й����������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_300A,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_300A,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_300A_6[]=//���ܱ����й����������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��������  double-long-unsigned����λ��kW�����㣺-4����
	DataType_double_long_unsigned,
//	0,0,0xB9,0xA0,//1.2Imax��100%�αȵ�ѹ�µĺ����й�����
	((MeterIMaxhex*MeterUBhex*36/1000)>>24)&0xff,((MeterIMaxhex*MeterUBhex*36/1000)>>16)&0xff,((MeterIMaxhex*MeterUBhex*36/1000)>>8)&0xff,((MeterIMaxhex*MeterUBhex*36+500)/1000)&0xff,//1.2Imax��100%�αȵ�ѹ�µĺ����й�����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_300B[]=//���ܱ��޹����������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_300B,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_300B,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_300B_5[]=//���ܱ��޹����������¼�-5
{
//����5�����ò�������=structure
	DataType_structure,
	2,
//{
//  �������� double-long-unsigned����λ��kvar�����㣺-4����
	DataType_double_long_unsigned,
	0,0,0,0,//1.2Imax��100%�αȵ�ѹ�µĺ����޹�����
//  �ж���ʱʱ�� unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_300C[]=//���ܱ��������������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_300C,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_300C,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_300C_6[]=//���ܱ��������������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ���޷�ֵ  long����λ��%�����㣺-1����
	DataType_long,
	1,0x2C,//0.3
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_300D[]=//���ܱ�ȫʧѹ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_300D,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_300D,
//�����������Ա��=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x20,0x01,0x22,0x00,//20012200�����¼�����ǰ����
};
const u8 InitData_300D_6[]=//���ܱ�ȫʧѹ�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_300E[]=//���ܱ�����Դ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_300E,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_300E,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_300E_6[]=//���ܱ�����Դ�����¼�
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_300F[]=//���ܱ��ѹ�������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_300F,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_300F,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���	
};
const u8 InitData_300F_6[]=//���ܱ��ѹ�������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3010[]=//���ܱ�����������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3010,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3010,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3010_6[]=//���ܱ�����������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3011[]=//���ܱ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3011,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3011,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3011_6[]=//���ܱ�����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3012[]=//���ܱ����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3012,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3012,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3012_6[]=//���ܱ����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3013[]=//���ܱ������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3013,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3013,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x20,0x22,0x01,//00202201�����¼�����ǰ�����й��ܵ���	
};
const u8 InitData_3013_6[]=//���ܱ������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3014[]=//���ܱ����������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3014,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3014,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3014_6[]=//���ܱ����������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3015[]=//���ܱ��¼������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3015,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3015,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3015_6[]=//���ܱ��¼������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3016[]=//���ܱ�Уʱ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3016,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3016,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x40,0x00,0x22,0x00,//40002200�����¼�����ǰʱ������
	DataType_OAD,
	0x40,0x00,0x82,0x00,//40008200�����¼�������ʱ������	
};
const u8 InitData_3016_6[]=//���ܱ�Уʱ�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3017[]=//���ܱ�ʱ�α����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3017,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3017,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3017_6[]=//���ܱ�ʱ�α����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3018[]=//���ܱ�ʱ�������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3018,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3018,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3018_6[]=//���ܱ�ʱ�������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3019[]=//���ܱ������ձ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3019,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3019,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3019_6[]=//���ܱ������ձ���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_301A[]=//���ܱ�����ձ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_301A,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_301A,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_301A_6[]=//���ܱ�����ձ���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_301B[]=//���ܱ����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_301B,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_301B,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���
};
const u8 InitData_301B_6[]=//���ܱ����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_301C[]=//���ܱ���ť���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_301C,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_301C,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���	
};
const u8 InitData_301C_6[]=//���ܱ���ť���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_301D[]=//���ܱ��ѹ��ƽ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_301D,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_301D,
//�����������Ա��=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x20,0x26,0x62,0x00,//20266200�����¼������ڼ��ѹ��ƽ����
};
const u8 InitData_301D_6[]=//���ܱ��ѹ��ƽ���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��ֵ  long����λ��%�����㣺-2����
	DataType_long,
	0x0B,0xB8,//30%
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_301E[]=//���ܱ������ƽ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_301E,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_301E,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_301E_6[]=//���ܱ������ƽ���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��ֵ  long����λ��%�����㣺-2����
	DataType_long,
	0x0B,0xB8,//30%
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_301F[]=//���ܱ���բ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_301F,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_301F,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_301F_6[]=//���ܱ���բ�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3020[]=//���ܱ��բ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3020,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3020,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3020_6[]=//���ܱ��բ�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3021[]=//���ܱ�ڼ��ձ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3021,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3021,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3021_6[]=//���ܱ�ڼ��ձ���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3022[]=//���ܱ��й���Ϸ�ʽ����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3022,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3022,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3022_6[]=//���ܱ��й���Ϸ�ʽ����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3023[]=//���ܱ��޹���Ϸ�ʽ����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3023,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3023,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3023_6[]=//���ܱ��޹���Ϸ�ʽ����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3024[]=//���ܱ���ʲ��������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3024,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3024,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3024_6[]=//���ܱ���ʲ��������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3025[]=//���ܱ���ݱ����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3025,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3025,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3025_6[]=//���ܱ���ݱ����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3026[]=//���ܱ���Կ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3026,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3026,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3026_6[]=//���ܱ���Կ�����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3027[]=//���ܱ��쳣�忨�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3027,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3027,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3027_6[]=//���ܱ��쳣�忨�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3028[]=//���ܱ����¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3028,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3028,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3028_6[]=//���ܱ����¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3029[]=//���ܱ��˷Ѽ�¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3029,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3029,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3029_6[]=//���ܱ��˷Ѽ�¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_302A[]=//���ܱ�㶨�ų������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_302A,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_302A,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���	
};
const u8 InitData_302A_6[]=//���ܱ�㶨�ų������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_302B[]=//���ܱ��ɿ��������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_302B,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_302B,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���	
};
const u8 InitData_302B_6[]=//���ܱ��ɿ��������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_302C[]=//���ܱ��Դ�쳣�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_302C,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_302C,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���	
};
const u8 InitData_302C_6[]=//���ܱ��Դ�쳣�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_302D[]=//���ܱ�������ز�ƽ���¼�	
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_302D,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_302D,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_302D_6[]=//���ܱ�������ز�ƽ���¼�	-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��ֵ  long����λ��%�����㣺-2��
	DataType_long,
	0x23,0x28,//90%
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	60,
//}
};

const u8 InitData_302E[]=//���ܱ�ʱ�ӹ����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_302E,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_302E,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_302E_6[]=//���ܱ�ʱ�ӹ����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_302F[]=//���ܱ����оƬ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_302F,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_302F,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_302F_6[]=//���ܱ����оƬ�����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3030[]=//ͨ��ģ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3030,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3030,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3030_6[]=//ͨ��ģ�����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3100[]=//�ն˳�ʼ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3100,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3100,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3100_6[]=//�ն˳�ʼ���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3101[]=//�ն˰汾����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3101,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3101,
//�����������Ա��=array OAD
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	DataType_array,
	2,
	DataType_OAD,
	0x43,0x00,0x22,0x02,//43002202�����¼�����ǰ����汾��
	DataType_OAD,
	0x43,0x00,0x42,0x02,//43004202�����¼�����������汾��
#endif
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	DataType_array,
	2,
	DataType_OAD,
	0x43,0x03,0x22,0x02,//43032202�����¼�����ǰ����汾��
	DataType_OAD,
	0x43,0x03,0x42,0x02,//43034202�����¼�����������汾��
#endif
#if ((Project/100)!=2)&&((Project/100)!=3)
	DataType_array,
	0,
#endif
};
const u8 InitData_3101_6[]=//�ն˰汾����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3104[]=//�ն�״̬����λ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3104,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3104,
//�����������Ա��=array OAD
	DataType_array,
	5,
//201E4200�����¼�����ʱ��
	DataType_OAD,
	0x20,0x1e,0x42,0x00,
//F2034201������1·״̬����λ������״̬����������
	DataType_OAD,
	0xF2,0x03,0x42,0x01,
//F2034202������2·״̬����λ������״̬
	DataType_OAD,
	0xF2,0x03,0x42,0x02,
//F2034203������3·״̬����λ������״̬
	DataType_OAD,
	0xF2,0x03,0x42,0x03,
//F2034204������4·״̬����λ������״̬
	DataType_OAD,
	0xF2,0x03,0x42,0x04,
};
const u8 InitData_3104_6[]=//�ն�״̬����λ�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3105[]=//���ܱ�ʱ�ӳ����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3105,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3105,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3105_6[]=//���ܱ�ʱ�ӳ����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//�쳣�б���ֵ  long-unsigned����λ���룩��
	DataType_long_unsigned,
	300>>8,300&0xff,//300��
//�����ɼ������ unsigned
	DataType_unsigned,
	28,
//}
};

const u8 InitData_3106[]=//�ն�ͣ/�ϵ��¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3106,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3106,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3106_6[]=//�ն�ͣ/�ϵ��¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//ͣ�����ݲɼ����ò���  structure
	DataType_structure,
	4,
//{
//�ɼ���־  bit-string(SIZE(8))��
	DataType_bit_string,
	2,
	0xC0,
//ͣ���¼�����ʱ������Сʱ�� unsigned ��
	DataType_unsigned,
	1,
//ͣ���¼�����ʱ����ֵ�����ӣ� unsigned��
	DataType_unsigned,
	5,
//��Ҫ��ȡͣ���¼����ܱ�       array TSA
	DataType_array,
	0,
//}��
//ͣ���¼������ֵ����  structure
	DataType_structure,
	6,
//{
//ͣ��ʱ����С��Ч��������ӣ� long-unsigned��
	DataType_long_unsigned,
	1>>8,1&0xff,
//ͣ��ʱ�������Ч��������ӣ� long-unsigned��
	DataType_long_unsigned,
	4320>>8,4320&0xff,//4320
//ͣ���¼���ֹʱ��ƫ����ֵ�����ӣ� long-unsigned��
	DataType_long_unsigned,
	5>>8,5&0xff,
//ͣ���¼�ʱ������ƫ����ֵ�����ӣ� long-unsigned��
	DataType_long_unsigned,
	1>>8,1&0xff,
#if MeterUBhex==2200//�αȵ�ѹxxx.x(V)
//ͣ�緢����ѹ��ֵ  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	1320>>8,1320&0xff,//132.0
//ͣ��ָ���ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
	DataType_long_unsigned,
	1760>>8,1760&0xff,//176.0
#endif
#if MeterUBhex==1000//�αȵ�ѹxxx.x(V)
//ͣ�緢����ѹ��ֵ  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	600>>8,600&0xff,//60.0
//ͣ��ָ���ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
	DataType_long_unsigned,
	800>>8,800&0xff,//80.0
#endif
#if MeterUBhex==577//�αȵ�ѹxxx.x(V)
//ͣ�緢����ѹ��ֵ  long-unsigned����λ��V�����㣺-1����
	DataType_long_unsigned,
	346>>8,346&0xff,//34.6
//ͣ��ָ���ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
	DataType_long_unsigned,
	462>>8,462&0xff,//46.2
#endif
//}
//}
//�ɼ���־��
//bit0���á�1����Ч���á�0����Ч��
//bit1���á�1�����ѡ������㣬�á�0��ֻ�ɼ����õĲ����㡣
};


const u8 InitData_3107[]=//�ն�ֱ��ģ����Խ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3107,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3107,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3107_6[]=//�ն�ֱ��ģ����Խ�����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  ֱ��ģ��������  double-long
	DataType_double_long,
	0,0,0,0,
//}
};

const u8 InitData_3108[]=//�ն�ֱ��ģ����Խ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3108,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3108,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3108_6[]=//�ն�ֱ��ģ����Խ�����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  ֱ��ģ��������  double-long
	DataType_double_long,
	0,0,0,0,
//}
};

const u8 InitData_3109[]=//�ն���Ϣ��֤�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3109,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3109,
//�����������Ա��=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x44,0x01,0x22,0x01,//44012201 �¼�����ǰ��ȫ��֤����
};
const u8 InitData_3109_6[]=//�ն���Ϣ��֤�����¼�
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_310A[]=//�豸���ϼ�¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_310A,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_310A,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_310A_6[]=//�豸���ϼ�¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_310B[]=//���ܱ�ʾ���½��¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_310B,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_310B,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���
};
const u8 InitData_310B_6[]=//���ܱ�ʾ���½��¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �����ɼ������ unsigned
	DataType_unsigned,
	0,
//}
};

const u8 InitData_310C[]=//�����������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_310C,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_310C,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���
};
const u8 InitData_310C_6[]=//�����������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��ֵ  double-long-unsigned����λ��%���޻��㣩,
	DataType_double_long_unsigned,
	(400>>24)&0xff,(400>>16)&0xff,(400>>8)&0xff,400&0xff,//4.0
//  �����ɼ������ unsigned
	DataType_unsigned,
	0,
};

const u8 InitData_310D[]=//���ܱ�����¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_310D,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_310D,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
	DataType_OAD,
	0x00,0x10,0x82,0x01,//00108201�����¼������������й��ܵ���
};
const u8 InitData_310D_6[]=//���ܱ�����¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��ֵ  double-long-unsigned����λ��%���޻��㣩,
	DataType_double_long_unsigned,
	(600>>24)&0xff,(600>>16)&0xff,(600>>8)&0xff,600&0xff,//6.0
//  �����ɼ������ unsigned
	DataType_unsigned,
	0,
};

const u8 InitData_310E[]=//���ܱ�ͣ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_310E,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_310E,
//�����������Ա��=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201�����¼�����ǰ�����й��ܵ���
};
const u8 InitData_310E_6[]=//���ܱ�ͣ���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  ��ֵ           TI,
	DataType_TI,
	1,15>>8,15&0xff,
//  �����ɼ������ unsigned
	DataType_unsigned,
	0,
//}
};

const u8 InitData_310F[]=//�ն˳���ʧ���¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_310F,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_310F,
//�����������Ա��=array OAD
	DataType_array,
	3,
	DataType_OAD,
	0x60,0x41,0x22,0x00,//60412200���¼�����ǰ���һ�γ���ɹ�ʱ��
	DataType_OAD,
	0x00,0x10,0x22,0x01,//00102201���¼�����ǰ���һ�γ���ɹ������й��ܵ���ʾֵ
	DataType_OAD,
	0x00,0x30,0x22,0x01,//00302201���¼�����ǰ���һ�γ���ɹ������޹��ܵ���ʾֵ
};
const u8 InitData_310F_6[]=//�ն˳���ʧ���¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	2,
//{
//  �����ִ�      unsigned,
	DataType_unsigned,
	3,
//    �����ɼ������ unsigned
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3110[]=//��ͨ�����������¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3110,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3110,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x22,0x00,0x42,0x02,//22004202-�¼����������ѷ���ͨ������
	DataType_OAD,
	0x31,0x10,0x06,0x01,//31100601-��ͨ����������
};
const u8 InitData_3110_6[]=//��ͨ�����������¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  ͨ����������  double-long-unsigned����λ��byte��
	DataType_double_long_unsigned,
	0,0,0,0,
//}
};

const u8 InitData_3111[]=//����δ֪���ܱ��¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3111,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3111,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3111_6[]=//����δ֪���ܱ��¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3112[]=//��̨�����ܱ��¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3112,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3112,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3112_6[]=//��̨�����ܱ��¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3114[]=//�ն˶�ʱ�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3114,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3114,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x40,0x00,0x22,0x00,//40002200���¼�����ǰ��ʱʱ��
	DataType_OAD,
	0x40,0x00,0x82,0x00,//40008200���¼��������ʱʱ��
};
const u8 InitData_3114_6[]=//�ն˶�ʱ�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3115[]=//ң����բ��¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3115,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3115,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3115_6[]=//ң����բ��¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3116[]=//�й��ܵ������Խ���¼���¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3116,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3116,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3116_6[]=//�й��ܵ������Խ���¼���¼-6
{
//����6�����ò�������=array �й��ܵ������������
	DataType_array,
	0,
////�й��ܵ�����������á�=structure
//	DataType_structure,
////{
////  �й��ܵ����������� unsigned��
//	DataType_unsigned,
////  �Աȵ��ܼ���           OI��
//	DataType_OI,
////  ���յ��ܼ���           OI��
//	DataType_OI,
////  �����ĵ�������ʱ�����估�Աȷ�����־ bit-string��SIZE��8������
//	DataType_bit_string,
////  �Խ�����ƫ��ֵ integer����λ��%�����㣺0����
//	DataType_integer,
////  �Խ�޾���ƫ��ֵ long64����λ��kWh�����㣺-4��
//	DataType_long64,
////}
//�����ĵ�������ʱ�����估�Աȷ�����־��
//bit0��bit1�����ʾ��������ʱ���ȣ�ȡֵ��Χ0��2���α�ʾ60���ӵ�����30���ӵ�����15���ӵ���������ֵ��Ч��
//bit7��ʾ�Աȷ�����־���á�0������ԶԱȣ���ʽ����ʽ��1�����á�1�������ԶԱȣ���ʽ����ʽ��2����
//bit2��bit6���á�
//	 ��  .....	(1)
//	  .............	(2)
//ʽ�У�
//    Q�����Աȵ��ܼ����ܵ�������
//    q�������յ��ܼ����ܵ�������
};

const u8 InitData_3117[]=//�����·����״̬��λ�¼���¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3117,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3117,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3117_6[]=//�����·����״̬��λ�¼���¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3118[]=//�ն˱�̼�¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3118,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3118,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3118_6[]=//�ն˱�̼�¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3119[]=//�ն˵�����·�쳣�¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3119,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3119,
//�����������Ա��=array OAD
	DataType_array,
	2,
	DataType_OAD,
	0x20,0x03,0x42,0x00,//20034200�����¼�����ʱ��ѹ�������
	DataType_OAD,
	0x00,0x10,0x42,0x01,//00104201�����¼�����ʱ�����й��ܵ���
};
const u8 InitData_3119_6[]=//�ն˵�����·�쳣�¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_311A[]=//���ܱ�����״̬�л��¼�
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_311A,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_311A,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_311A_6[]=//���ܱ�����״̬�л��¼�-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//   �ж���ʱʱ��  long-unsigned����λ��s�����㣺0��
	DataType_long_unsigned,
	0,0,
//}
};

const u8 InitData_311B[]=//�ն˶Ե��Уʱ��¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_311B,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_311B,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_311B_6[]=//�ն˶Ե��Уʱ��¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_311C[]=//���ܱ����ݱ����ؼ�¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_311C,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_311C,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_311C_6[]=//���ܱ����ݱ����ؼ�¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	1,
//{
//  �����ɼ��������    unsigned
	DataType_unsigned,
	0,
//}
};

const u8 InitData_3200[]=//������բ��¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3200,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3200,
//�����������Ա��=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x23,0x01,0x23,0x00,//23012300������բ����ǰ�ܼ��й����� �ܼ���1
};
const u8 InitData_3200_6[]=//������բ��¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3201[]=//�����բ��¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3201,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3201,
//�����������Ա��=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x23,0x01,0x49,0x00,//23014900������բ����ʱ�����й��ܼӵ�����	�ܼ���1
};
const u8 InitData_3201_6[]=//�����բ��¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3202[]=//����������ü�¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3202,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3202,
//�����������Ա��=array OAD
	DataType_array,
	1,
	DataType_OAD,
	0x81,0x0c,0x22,0x01,//810C2201�¼�����ǰ��������õ�Ԫ
};
const u8 InitData_3202_6[]=//����������ü�¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};

const u8 InitData_3203[]=//��ظ澯�¼���¼
{
//��Ч��ʶ��=bool
	DataType_bool,
	Init_Enable_Event_3203,
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
	DataType_enum,
	Init_Report_Event_3203,
//�����������Ա��=array OAD
	DataType_array,
	0,
};
const u8 InitData_3203_6[]=//��ظ澯�¼���¼-6
{
//����6�����ò�������=structure
	DataType_structure,
	0,
//{
//}
};




#endif




