

#ifndef ATNo_H
#define ATNo_H


//����AT�����
#define AT_AT     0//�����ʼ��
	//ģ����Ϣ
#define AT_ATE0     (AT_AT+1)//�ػ����Ҵ��ڼ��1
#define AT_MYTYPE    (AT_ATE0+1)//��ѯԶ��ͨ��ģ������2
#define AT_MYGMR     (AT_MYTYPE+1)//��ȡģ������汾
#define AT_CPIN     (AT_MYGMR+1)//�����
#define AT_MYNETURC     (AT_CPIN+1)//�������ϱ�
#define AT_MYCCID    (AT_MYNETURC+1)//��ȡSIM�����к�ICCID
#define AT_CNUM     (AT_MYCCID+1)//ȡ��������
#define AT_CIMI     (AT_CNUM+1)//������ƶ�̨��ʶ����IMSI
#define AT_CSPN     (AT_CIMI+1)//��ȡSIM��������Ӫ������

	//��������
#define AT_MYNETCON_APN     (AT_CSPN+1)//����APN 10
#define AT_MYNETCON_USERPWD     (AT_MYNETCON_APN+1)//����USERPWD:�û���������
#define AT_CSQ_NOLINK     (AT_MYNETCON_USERPWD+1)//���ź�û����ǰ
#define AT_CREG     (AT_CSQ_NOLINK+1)//��ȡ��ǰ����ע��״̬
#define AT_SYSINFO	(AT_CREG+1)//��ѯע���������ͣ�3G/4G
#define AT_MYNETACT     (AT_SYSINFO+1)//������������
#define AT_MYIPFILTER     (AT_MYNETACT+1)//IP���ʿ�������
#define AT_CMFG     (AT_MYIPFILTER+1)//���ö���Ϣģʽ
#define AT_ZCVF     (AT_CMFG+1)//�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
#define AT_CTA     (AT_ZCVF+1)//���ߵȴ�ʱ��(��CDMAʱ��Ҫ)

	//������ѭ����ʼ
#define AT_MYNETSRV     (AT_CTA+1)//���÷������ 20
#define AT_MYNETOPEN     (AT_MYNETSRV+1)//��������
	//��������ѭ����ʼ
#define AT_SMS_NOLINK     (AT_MYNETOPEN+1)//���ŷ���û����ǰ
#define AT_CSQ_NOLINK2     (AT_SMS_NOLINK+1)//���ź�û����2
#define AT_LOOP_NOLINK     (AT_CSQ_NOLINK2+1)//ATѭ��,û����

	//���Ӻ�ѭ����ʼ
#define AT_CSQ_LINK     (AT_LOOP_NOLINK+1)//���ź����Ӻ�
#define AT_SYSINFO_LINK     (AT_CSQ_LINK+1)//��ѯע���������ͣ�3G/4G
#define AT_MYNETOPEN_LINK     (AT_SYSINFO_LINK+1)//���Ӽ��
	//�����շ�
#define AT_MYNETREAD     (AT_MYNETOPEN_LINK+1)//��ȡ����
#define AT_MYNETWRITE     (AT_MYNETREAD+1)//��������
#define AT_MYFTP     (AT_MYNETWRITE+1)//FTP�ļ����� 30
#define AT_SMS_LINK     (AT_MYFTP+1)//���ŷ���
#define AT_LOOP     (AT_SMS_LINK+1)//ATѭ��,���߿���


	//ͣ�ػ�
#define AT_HOLD     (AT_LOOP+1)//HOLD��ͣ 33
#define AT_MYNETCLOSE     (AT_HOLD+1)//�ر����� 34
#define AT_MYPOWEROFF     (AT_MYNETCLOSE+1)//�ػ� 35
#define AT_MYRESET		  (AT_MYPOWEROFF+1)//err20190410 ����ģ��CFUN     36
#define AT_Max     (AT_MYRESET+1) //37


#endif
