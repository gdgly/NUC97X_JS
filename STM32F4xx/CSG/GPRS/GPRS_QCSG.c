
//����GPRSģ�� ����Զ��ͨ�ű�׼
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../GPRS/GPRS_QCSG.h"
#include "../GPRS/ATNo.h"
#include "../GPRS/ATString.h"
#include "../GPRS/GPRS_FTP_RxTx.h"
#include "../GPRS/GPRS_SMS_RxTx.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../TEST/LOG.h"

#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"

#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "Terminal_Task.h"

#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"


// SocketID 0-5ʹ��Լ��
// 0 ������
// 1 FTP����
// 2 FTP����
// 3-5 δ��

const SelfAdaptionCommunicationParaStruct SelfAdaptionParaArry[3][2] = 
{
//�ƶ�
	{

		{
			{0x21, 0x1C, 116,162, 20, 100, 0, 0},		// MasterAddr	
			0x02,										// MasterAddrType

			{											// APN
				'Y', 'N', 'D', 'W', 'F', 'K', '.', 'Y',
			 	'N', 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}
		},

		{
			{0x78, 0x17, 198,27, 111, 10, 0, 0},		// MasterAddr	
			//{0xD8, 0x07, 178,151, 247, 183, 0, 0},		// MasterAddr
			0x02,										// MasterAddrType

			{											// APN
				'C', 'M', 'I', 'O', 'T', 'Y', 'N', 'D',
			 	'W', 'F', 'K', '.', 'Y', 'N', 0, 0
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}

		}
		

	},
//��ͨ
	{
		{
			{0x21, 0x1C, 46,1, 100, 172, 0, 0},			// MasterAddr	
			0x02,										// MasterAddrType

			{											// APN
				'Y', 'N', 'D', 'W', '.', 'F', 'K', '.', 
			 	'Y', 'N', 'A', 'P', 'N', 0, 0, 0, 
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}
		},

		{
			{0x79, 0x17, 198,27, 111, 10, 0, 0},		// MasterAddr	
			0x02,										// MasterAddrType

			{											// APN
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}
		}
	},
//����

	{
		{
			{0x21, 0x1C, 71,1, 100, 172, 0, 0},			// MasterAddr	
			0x02,										// MasterAddrType
		
			{											// APN
				'Y', 'E', 'P', 'G', 'F', 'K', '.', 'V',
				'P', 'D', 'N', '.', 'Y', 'N', 0, 0
			},
			{											// APNUserName
				'q', 'j', 'd', 'w', '@', 'y', 'n', 'd',
				'w', 's', 'k', '.', 'v', 'p', 'd', 'n',
				'.', 'y', 'n', 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'g', 'd', 'j', '1', '7', '2', 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 
				0, 0, 0, 0, 0, 0, 0, 0
			}
		},
		{
			{0x7A, 0x17, 198,27, 111, 10, 0, 0},		// MasterAddr	
			0x02,										// MasterAddrType

			{											// APN
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNUserName
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0
			},
			{											// APNPassWord
				'C', 'M', 'N', 'E', 'T', 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0,
			 	0, 0, 0, 0, 0, 0, 0, 0, 
			 	0, 0, 0, 0, 0, 0, 0, 0
			}
		}
	}
};


__asm void GPRS_3763_AT_Command_Str(void)
{
//�����ʼ��
ADDR_ATStr_AT
	dcb "AT\r",0
ADDR_ERRStr_AT
	dcb "ATָ������Ӧ",0
	
	//ģ����Ϣ
//�ػ����Ҵ��ڼ��
ADDR_ATStr_ATE0
	dcb "ATE0\r",0
ADDR_ERRStr_ATE0
	dcb "û��⵽Զ������ģ��",0

//��ѯԶ��ͨ��ģ������
ADDR_ATStr_MYTYPE
	dcb "AT$MYTYPE?\r",0
ADDR_ERRStr_MYTYPE
	dcb "��ѯԶ����������ʧ��",0

//��ȡģ������汾
ADDR_ATStr_MYGMR
	dcb "AT$MYGMR\r",0
ADDR_ERRStr_MYGMR
	dcb "��Զ�����߰汾ʧ��",0

//�����
ADDR_ATStr_CPIN
	dcb "AT+CPIN?\r",0
ADDR_ERRStr_CPIN
	dcb "Զ��������SIM/UIM��",0

//�������ϱ�
ADDR_ATStr_MYNETURC
	dcb "AT$MYNETURC=0\r",0
ADDR_ERRStr_MYNETURC
	dcb "�������ϱ�ʧ��",0

//��ȡSIM�����к�ICCID
ADDR_ATStr_MYCCID
	dcb "AT$MYCCID\r",0
ADDR_ERRStr_MYCCID
	dcb "����CCIDʧ��",0

//ȡ��������
ADDR_ATStr_CNUM
	dcb "AT+CNUM\r",0
ADDR_ERRStr_CNUM
	dcb "����������ʧ��",0
	
//������ƶ�̨��ʶ����IMSI
ADDR_ATStr_CIMI
	dcb "AT+CIMI\r",0
ADDR_ERRStr_CIMI
	dcb "������ƶ���ʶʧ��",0

//��ȡSIM��������Ӫ������
ADDR_ATStr_CSPN
	dcb "AT+CSPN?\r",0
ADDR_ERRStr_CSPN
	dcb "������Ӫ������ʧ��",0

	//��������
//����APN
ADDR_ATStr_MYNETCON_APN
	dcb "AT$MYNETCON=0,",'"',"APN",'"',',','"',0
ADDR_ERRStr_MYNETCON_APN
	dcb "����APNʧ��",0

//����USERPWD:�û���������
ADDR_ATStr_MYNETCON_USERPWD
	dcb "AT$MYNETCON=0,",'"',"USERPWD",'"',',','"',0
ADDR_ERRStr_MYNETCON_USERPWD
	dcb "����USERPWDʧ��",0

//���ź�û����ǰ
ADDR_ATStr_CSQ_NOLINK
	dcb "AT+CSQ\r",0
ADDR_ERRStr_CSQ_NOLINK
	dcb "Զ�����߶��ź�ʧ��",0

//��ȡ��ǰ����ע��״̬
ADDR_ATStr_CREG
	dcb "AT+CREG?\r",0
ADDR_ERRStr_CREG
	dcb "��ȡ����ע��״̬ʧ��",0

//��ѯע���������ͣ�3G/4G
ADDR_ATStr_SYSINFO
	dcb "AT$MYSYSINFO\r",0
ADDR_ERRStr_SYSINFO
	dcb "��ѯ�Ƿ�ע����4G����",0

//������������
ADDR_ATStr_MYNETACT
	dcb "AT$MYNETACT=0,1\r",0
ADDR_ERRStr_MYNETACT
	dcb "������������ʧ��",0

//IP���ʿ�������
ADDR_ATStr_MYIPFILTER
	dcb "AT$MYIPFILTER=0,1,",0
ADDR_ERRStr_MYIPFILTER
	dcb "IP��������ʧ��",0
	
//���ö���Ϣģʽ(0=PDU,1=TEXT)
ADDR_ATStr_CMFG
 #if SM_CMGF==0//����ģʽ:0=PDUģʽ,1=�ı�ģʽ
	dcb "AT+CMGF=0\r",0
 #else
	dcb "AT+CMGF=1\r",0
 #endif
ADDR_ERRStr_CMFG
	dcb "���ö���Ϣģʽʧ��",0

//�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
ADDR_ATStr_ZCVF
	dcb "AT+ZCVF=2\r",0
ADDR_ERRStr_ZCVF
	dcb "�򿪶��Ź���ʧ��",0

//���ߵȴ�ʱ��(��CDMAʱ��Ҫ);�����ݴ���ʱ����10���CDMAͨ��ģ�������ͷſ��нӿ���Դ
ADDR_ATStr_CTA
	dcb "AT+CTA=10\r",0
ADDR_ERRStr_CTA
	dcb "�������ߵȴ�ʱ��ʧ��",0



	//û����ѭ����ʼ
//���÷������
ADDR_ATStr_MYNETSRV
	dcb 0
ADDR_ERRStr_MYNETSRV
	dcb "���÷������ʧ��",0
//��������
ADDR_ATStr_MYNETOPEN
	dcb "AT$MYNETOPEN=0\r",0
ADDR_ERRStr_MYNETOPEN
	//dcb "Զ�����߿�������ʧ��",0
	dcb 0
//���ŷ���û����ǰ
//���ź�û����2
//ATѭ��,û����

	//���Ӽ��
ADDR_ATStr_MYNETOPEN_LINK
	dcb "AT$MYNETOPEN?\r",0
ADDR_ERRStr_MYNETOPEN_LINK
	dcb "Զ���������类�Ͽ�",0
	//�����շ�
//��ȡ����
ADDR_ATStr_MYNETREAD
	dcb 0
ADDR_ERRStr_MYNETREAD
	dcb "Զ�����߶�����ʧ��",0
//��������
ADDR_ATStr_MYNETWRITE
	dcb "AT$MYNETWRITE=0,",0
ADDR_ERRStr_MYNETWRITE
	dcb "Զ�����߷�������ʧ��",0
//���ŷ���
	

//HOLD��ͣ
ADDR_ATStr_HOLD
	dcb 0
//�ر�����
ADDR_ATStr_MYNETCLOSE
	dcb "AT$MYNETCLOSE=0\r",0
ADDR_ERRStr_MYNETCLOSE
	dcb "Զ�����߹ر�����ʧ��",0
//�ػ�
ADDR_ATStr_MYPOWEROFF
	dcb "AT$MYPOWEROFF\r",0
ADDR_ERRStr_MYPOWEROFF
	dcb "Զ������ģ��ػ�ʧ��",0
	
//���� err20190410 ����δ��⵽SIM��ʱ���в��ϵ�GPRSģ������
ADDR_ATStr_MYRESET
	 dcb "AT+CFUN=1,1\r",0
ADDR_ERRStr_MYRESET
	 dcb "Զ������ģ������ʧ��",0

 
	ALIGN 4	
};





__asm void GPRS_QCSG_AT_Command_Lib(void)//AT˳��ִ�п�
{
//�����ʼ��
	dcd AT_AT
	dcd ADDR_ATStr_AT//AT�����ַ�����ַ
	dcd ADDR_ERRStr_AT//������ʾ�ַ�����ַ
	dcd 500//AT����ص�1���ַ���ʱ(ms)

//�ػ���
	dcd AT_ATE0
	dcd ADDR_ATStr_ATE0//AT�����ַ�����ַ
	dcd ADDR_ERRStr_ATE0//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//��ѯԶ��ͨ��ģ������
	dcd AT_MYTYPE
	dcd ADDR_ATStr_MYTYPE//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYTYPE//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//��ȡģ������汾
	dcd AT_MYGMR
	dcd ADDR_ATStr_MYGMR//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYGMR//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//�����
	dcd AT_CPIN
	dcd ADDR_ATStr_CPIN//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CPIN//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//�������ϱ�
	dcd AT_MYNETURC
	dcd ADDR_ATStr_MYNETURC//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETURC//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//��ȡSIM�����к�ICCID
	dcd AT_MYCCID
	dcd ADDR_ATStr_MYCCID//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYCCID//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//ȡ��������
	dcd AT_CNUM
	dcd ADDR_ATStr_CNUM//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CNUM//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//������ƶ�̨��ʶ����IMSI
	dcd AT_CIMI
	dcd ADDR_ATStr_CIMI//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CIMI//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)

//��ȡSIM��������Ӫ������
	dcd AT_CSPN
	dcd ADDR_ATStr_CSPN//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CSPN//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)


//����APN
	dcd AT_MYNETCON_APN
	dcd ADDR_ATStr_MYNETCON_APN//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETCON_APN//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//����USERPWD:�û���������
	dcd AT_MYNETCON_USERPWD
	dcd ADDR_ATStr_MYNETCON_USERPWD//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETCON_USERPWD//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//���ź�û����ǰ
	dcd AT_CSQ_NOLINK
	dcd ADDR_ATStr_CSQ_NOLINK//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CSQ_NOLINK//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)

//��ȡ��ǰ����ע��״̬
	dcd AT_CREG
	dcd ADDR_ATStr_CREG//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CREG//������ʾ�ַ�����ַ
	dcd 3000//AT����ص�1���ַ���ʱ(ms)
	
//��ѯע���������ͣ�3G/4G
	dcd AT_SYSINFO
	dcd ADDR_ATStr_SYSINFO//AT�����ַ�����ַ
	dcd ADDR_ERRStr_SYSINFO//������ʾ�ַ�����ַ
	dcd 3000//AT����ص�1���ַ���ʱ(ms)

//������������
	dcd AT_MYNETACT
	dcd ADDR_ATStr_MYNETACT//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETACT//������ʾ�ַ�����ַ
	dcd 90000//AT����ص�1���ַ���ʱ(ms)

//IP���ʿ�������
	dcd AT_MYIPFILTER
	dcd ADDR_ATStr_MYIPFILTER//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYIPFILTER//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)

//���ö���Ϣģʽ
	dcd AT_CMFG
	dcd ADDR_ATStr_CMFG//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CMFG//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
	dcd AT_ZCVF
	dcd ADDR_ATStr_ZCVF//AT�����ַ�����ַ
	dcd ADDR_ERRStr_ZCVF//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//���ߵȴ�ʱ��(��CDMAʱ��Ҫ);�����ݴ���ʱ����10���CDMAͨ��ģ�������ͷſ��нӿ���Դ
	dcd AT_CTA
	dcd ADDR_ATStr_CTA//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CTA//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)


	//û����ѭ����ʼ
//���÷������
	dcd AT_MYNETSRV
	dcd ADDR_ATStr_MYNETSRV//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETSRV//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//��������
	dcd AT_MYNETOPEN
	dcd ADDR_ATStr_MYNETOPEN//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETOPEN//������ʾ�ַ�����ַ
	dcd 90000//AT����ص�1���ַ���ʱ(ms)
//���ŷ���û����ǰ
	dcd AT_SMS_NOLINK
	dcd 0//AT�����ַ�����ַ
	dcd 0//������ʾ�ַ�����ַ
	dcd 6000//AT����ص�1���ַ���ʱ(ms)
//���ź�û����2
	dcd AT_CSQ_NOLINK2
	dcd ADDR_ATStr_CSQ_NOLINK//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CSQ_NOLINK//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//ATѭ��,û����
	dcd AT_LOOP_NOLINK
	dcd 0//AT�����ַ�����ַ
	dcd 0//������ʾ�ַ�����ַ
	dcd 0//AT����ص�1���ַ���ʱ(ms)


//���ź����Ӻ�
	dcd AT_CSQ_LINK
	dcd ADDR_ATStr_CSQ_NOLINK//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CSQ_NOLINK//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)
//��ѯע���������ͣ�3G/4G
	dcd AT_SYSINFO_LINK
	dcd ADDR_ATStr_SYSINFO//AT�����ַ�����ַ
	dcd ADDR_ERRStr_SYSINFO//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//��ѯ��������
	dcd AT_MYNETOPEN_LINK
	dcd ADDR_ATStr_MYNETOPEN_LINK//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETOPEN_LINK//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//��ȡ����
	dcd AT_MYNETREAD
	dcd ADDR_ATStr_MYNETREAD//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETREAD//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)
//��������
	dcd AT_MYNETWRITE
	dcd ADDR_ATStr_MYNETWRITE//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETWRITE//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)
//FTP�ļ�����
	dcd AT_MYFTP
	dcd 0//AT�����ַ�����ַ
	dcd 0//������ʾ�ַ�����ַ
	dcd 35000//40000//AT����ص�1���ַ���ʱ(ms)
//���ŷ���
	dcd AT_SMS_LINK
	dcd 0//AT�����ַ�����ַ
	dcd 0//������ʾ�ַ�����ַ
	dcd 6000//AT����ص�1���ַ���ʱ(ms)


//ATѭ��,���߿���
	dcd AT_LOOP
	dcd 0//AT�����ַ�����ַ
	dcd 0//������ʾ�ַ�����ַ
	dcd 0//AT����ص�1���ַ���ʱ(ms)




//HOLD��ͣ
	dcd AT_HOLD
	dcd ADDR_ATStr_HOLD//AT�����ַ�����ַ
	dcd ADDR_ERRStr_ATE0//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//�ر�����
	dcd AT_MYNETCLOSE
	dcd ADDR_ATStr_MYNETCLOSE//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETCLOSE//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//�ػ�
	dcd AT_MYPOWEROFF
	dcd ADDR_ATStr_MYPOWEROFF//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYPOWEROFF//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//���� err20190410 GPRSģ������
	dcd AT_MYRESET
	dcd ADDR_ATStr_MYRESET//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYRESET//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

};

#define GPRS_MAX_WRITE_DATA_LEN 		(1460)

void GPRS_QCSG_NextATCount(void)//��1��AT
{
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;
	
	pUartCtrl->LinkSubTask = 0;		//����AT���ͼ���
	pUartCtrl->SubTask = 0;			//������
	pUartCtrl->ATCount++;
	//�¸�AT���ʱ����
	switch(pUartCtrl->ATCount)//�¸�AT����
	{
		case AT_CSQ_NOLINK://���ź�û����ǰ
			pTerminalRam->GPRSWaitTx_S_Timer = 60;//GPRS�ȴ��ź�ǿ�Ȳ�ѯ(����ǰ)��ʱ
			break;
		case AT_CREG://��ȡ��ǰ����ע��״̬
			pTerminalRam->GPRSWaitTx_S_Timer = 60;//�ȴ�ע�ᶨʱ
			break;
		case AT_MYNETACT://������������
			pTerminalRam->GPRSWaitTx_S_Timer = 30;//�ȴ������������Ӷ�ʱ
			break;
		
	}
}

void LineATIntRx(u32 ADDR_ATIntRx,u32 ADDR_BUFF,u32 Byte)//ȡAT�жϽ����������Է��ڻ�����
{
	u32 i;
	u8 *p8s;
	u8 *p8d;
	
	while(ADDR_ATIntRx>=(ADDR_UART1IntATRx+LEN_UART1IntATRx))
	{
		ADDR_ATIntRx-=(LEN_UART1IntATRx-10);
	}
	p8s=(u8*)(ADDR_ATIntRx);
	p8d=(u8*)(ADDR_BUFF);
	for(i=0;i<Byte;i++)
	{
		p8d[0]=p8s[0];
		p8s++;
		p8d++;
		if(((u32)p8s)>=(ADDR_UART1IntATRx+LEN_UART1IntATRx))
		{
			p8s=(u8*)(ADDR_UART1IntATRx+10);
		}
	}
}

const u8 Err_Comm_Addr[COMM_AADR_SIZE] ={0,0,0,0,0,0};//��Ч��ַ 


void GPRS_QCSG_AT_Tx(void)//AT�����
{
	u32 i, x, y, AT_No, TxByte, FTPFileCount;	//FTP�ļ����ظ�������
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u8 *p8rxbuff;
	u8 *p8txbuff;
	CommunicationParaStruct CommPara;
	//SelfAdaptionParaStruct SelfAdaption_Temp;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;

	#if ((Project/100 == 3) && (USER == 3400))//err20190528 ���ϸ�������Ӧ����
		E000011x_CommunicationParaStruct E0000111_CommPara;
		E000011x_CommunicationParaStruct E0000112_CommPara;
		E000011x_CommunicationParaStruct E0000113_CommPara;
		MemSet((u8*)&E0000111_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		MemSet((u8*)&E0000112_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		MemSet((u8*)&E0000113_CommPara, 0, sizeof(E000011x_CommunicationParaStruct));
		MR((u32)&E0000111_CommPara, ADDR_CommunicationPara_E0000111, sizeof(E000011x_CommunicationParaStruct));
		MR((u32)&E0000112_CommPara, ADDR_CommunicationPara_E0000112, sizeof(E000011x_CommunicationParaStruct));
		MR((u32)&E0000113_CommPara, ADDR_CommunicationPara_E0000113, sizeof(E000011x_CommunicationParaStruct));
		
	#endif	

	

	if(pTerminalRam->AT_MS_Timer != 0) return;
	p8txbuff = (u8*)(ADDR_ATTx);
	TxByte = 0;
	AT_No = pUartCtrl->ATCount;//AT����
	if(AT_No > AT_Max)
	{
		AT_No = 0;
		pUartCtrl->ATCount = 0;//AT����
	}
	p32=(u32*)(&GPRS_QCSG_AT_Command_Lib);
	switch(p32[4*AT_No])//AT����
	{
		case AT_AT://�����ʼ��
			TxByte = CopyString((u8 *)"AT\r\n",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			#if LOG_1376_3==1//0=û,1=��
				LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			switch(pUartCtrl->SubTask)	//�����񣬱�ʾʹ���ĸ������ʽ��з��ͳ�ʼ ATָ��
			{
			#if (TEST_ON == 0)
				case 0://����115200
					pUartCtrl->BpsCtrl=0x3+(10<<5);
					break;
				case 1://����57600
					pUartCtrl->BpsCtrl=0x3+(9<<5);
					break;
				case 2://����38400
					pUartCtrl->BpsCtrl=0x3+(8<<5);
					break;
				case 3://����19200
					pUartCtrl->BpsCtrl=0x3+(7<<5);
					break;
				case 4://����9600
					pUartCtrl->BpsCtrl=0x3+(6<<5);
					break;
			#endif

			#if (TEST_ON == 1)
				case 0://����115200
					pUartCtrl->BpsCtrl=0x3+(10<<5);
					break;			
				case 1://����38400
					pUartCtrl->BpsCtrl=0x3+(8<<5);
					break;				
				case 2://����9600
					pUartCtrl->BpsCtrl=0x3+(6<<5);
					break;
			#endif
				default:
//				#if  (TEST_ON == 0)
//					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
//					pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
//					return;
//				#else
					TxByte = CopyString((u8 *)"AT+IPR=57600\r\n",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					break;				
				//#endif
			}
			Init_UARTn(GPRSPORT);			// ��ʼ��UART��,���ͨ�ſ�����������
			for(i = 0; i < 80000; i++);
			pUartCtrl->TxByte = TxByte;		// �����ֽ���(���ֶ���)
			pUartCtrl->TxCount = 0;
			#if LOG_1376_3==1//0=û,1=��
				LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			Start_UARTn_Tx(GPRSPORT);	// ����UART�ڷ���
			pUartCtrl->ATTask = 2;		// AT����:0=����,1=����,2=���ڷ���,3=����
			return;
		case AT_CNUM://ȡ��������
			if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
			{//GSM����֧��GPRS
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			else
			{//����CDMA(MC8332)
				TxByte = CopyString((u8 *)"AT+CNUM?\r",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			break;	
		case AT_CSPN: //����ָ�֧��AT_CSPN
			GPRS_QCSG_NextATCount();
			return;
		case AT_MYNETCON_APN://����APN
			if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
			{//GSM����֧��GPRS
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				
				//MR((u32)&SelfAdaption_Temp, (u32)ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));
				MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));

				//err20190528 ���ϸ�������Ӧ
				if(pCommRam->CSPN == 1)//�ƶ�
				{
					if(pTerminalRam->LinkCount_SelfAdaption == 0)//0 1 2
					{
						if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 1;
							pTerminalRam->LinkCount_SelfAdaption = 1;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 1)//0 1 2
					{
						if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 2;
							pTerminalRam->LinkCount_SelfAdaption = 2;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 2)//0 1 2
					{
						if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 3;
							pTerminalRam->LinkCount_SelfAdaption = 3;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 3)
					{
						if(MemCompare(E0000111_CommPara.MasterAddr,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//ȷ�ϵ�ַ��Ч
						{
							MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN1, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 4)
					{
						if(MemCompare(E0000111_CommPara.MasterBackAddr1,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//ȷ�ϵ�ַ��Ч
						{
							MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN2, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					
				}
				else if(pCommRam->CSPN == 2)//��ͨ
				{
					if(pTerminalRam->LinkCount_SelfAdaption == 0)//0 1 2
					{
						if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 1;
							pTerminalRam->LinkCount_SelfAdaption = 1;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 1)//0 1 2
					{
						if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 2;
							pTerminalRam->LinkCount_SelfAdaption = 2;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 2)//0 1 2
					{
						if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 3;
							pTerminalRam->LinkCount_SelfAdaption = 3;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 3)
					{
						if(MemCompare(E0000112_CommPara.MasterAddr,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//ȷ�ϵ�ַ��Ч
						{
							MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN1, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 4)
					{
						if(MemCompare(E0000112_CommPara.MasterBackAddr1,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//ȷ�ϵ�ַ��Ч
						{
							MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN2, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					
				}
				else if(pCommRam->CSPN == 3)//����
				{
					if(pTerminalRam->LinkCount_SelfAdaption == 0)//0 1 2
					{
						if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 1;
							pTerminalRam->LinkCount_SelfAdaption = 1;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 1)//0 1 2
					{
						if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 2;
							pTerminalRam->LinkCount_SelfAdaption = 2;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 2)//0 1 2
					{
						if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
						{
							MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
						}
						else
						{		
							pUartCtrl->LinkCount = 3;
							pTerminalRam->LinkCount_SelfAdaption = 3;
						}
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 3)
					{
						if(MemCompare(E0000113_CommPara.MasterAddr,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//ȷ�ϵ�ַ��Ч
						{
							MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN1, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					if(pTerminalRam->LinkCount_SelfAdaption == 4)
					{
						if(MemCompare(E0000113_CommPara.MasterBackAddr1,Err_Comm_Addr,COMM_AADR_SIZE) != 0)//ȷ�ϵ�ַ��Ч
						{
							MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN2, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
						}
						else pTerminalRam->LinkCount_SelfAdaption ++;
					}
					
				}
				else 
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
				}

				if(pTerminalRam->LinkCount_SelfAdaption >= 5)//����ͨѶ�������Ѿ��Թ�
				{
					pTerminalRam->LinkCount_SelfAdaption = 0;
					pUartCtrl->LinkCount = 0;
					pUartCtrl->LinkCtrl = 0;
//						WarningOccur(GPRSPORT,(u8 *)"Զ��������վ����ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
					pUartCtrl->State = GPRS_STATE_LINK_FAIL;
					//err20190313 �޸�GPRS���̣�����IP��������ʧ�ܲ�ֱ������ģ�飬��������ͨ�Ų������ٳ��ԡ�
					pTerminalRam->GPRS_Para_Reset_Count++;
					if(pTerminalRam->GPRS_Para_Reset_Count>=3)//��ѭ�����Գ���3����Ӳ������ģ��
					{
						pTerminalRam->GPRS_Para_Reset_Count = 0;
						pTerminalRam->GPRS_Para_Reset_Flag = 0;
						pUartCtrl->ATCount = AT_MYPOWEROFF;		
					}
					else 
					{
						pTerminalRam->GPRS_Para_Reset_Flag = 1;
						pUartCtrl->ATCount = AT_MYNETACT;//����APN����
					}
					//pUartCtrl->ATCount = AT_MYPOWEROFF;		// �ػ�
					return;
				}

				
//				if(SelfAdaption_Temp.SelfAdaptionFlag == 0x01)//
//				{
//					if(pCommRam->CSPN == 1)
//						MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[0][pTerminalRam->LinkCount_SelfAdaption].APN, APN_NAME_MAX_SIZE);
//					else if(pCommRam->CSPN == 2)
//						MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[1][pTerminalRam->LinkCount_SelfAdaption].APN, APN_NAME_MAX_SIZE);
//					else if(pCommRam->CSPN == 3)
//						MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[2][pTerminalRam->LinkCount_SelfAdaption].APN, APN_NAME_MAX_SIZE);
//					else 
//					{
//						MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
//					}
//				}
//				else 
//				{
//					MR(ADDR_DATABUFF, (u32)CommPara.APN, APN_NAME_MAX_SIZE);
//				}

				if(pTerminalRam->LinkCount_SelfAdaption >=2)
					MC(0, ADDR_DATABUFF + APN_USER_NAME_MAX_SIZE, 1);
				else 
					MC(0, ADDR_DATABUFF + APN_NAME_MAX_SIZE, 1);
				TxByte += CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				p8txbuff[TxByte++]='"';
				p8txbuff[TxByte++]='\r';
			}
			else
			{//����CDMA(MC8332)
				//AUTH����Ȩ���ͣ�0��NONE��1��PAP��2��CHAP��Ĭ��ֵ1
				//AT$MYNETCON=0,"AUTH",1
				TxByte=CopyString((u8 *)"AT$MYNETCON=0,\"AUTH\",1",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				p8txbuff[TxByte++]='\r';
			}
			break;		
		case AT_MYNETCON_USERPWD://����USERPWD:�û���������
			//AT$MYNETCON=0,"USERPWD","card,card"
			TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);			//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			
			//MR((u32)&SelfAdaption_Temp, (u32)ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));
			MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));


			//err20190528 ���ϸ�������Ӧ
			if(pCommRam->CSPN == 1)//�ƶ�
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{
					MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN1UserName, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����

				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN2UserName, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				
			}
			else if(pCommRam->CSPN == 2)//��ͨ
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{
					MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN1UserName, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN2UserName, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				
			}
			else if(pCommRam->CSPN == 3)//����
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{	
					MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN1UserName, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN2UserName, APN_USER_NAME_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				
			}
			else 
			{
				MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
			}



//			if(SelfAdaption_Temp.SelfAdaptionFlag == 0x01)//
//			{
//				if(pCommRam->CSPN == 1)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[0][pTerminalRam->LinkCount_SelfAdaption].APNUserName, APN_USER_NAME_MAX_SIZE);
//				else if(pCommRam->CSPN == 2)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[1][pTerminalRam->LinkCount_SelfAdaption].APNUserName, APN_USER_NAME_MAX_SIZE);
//				else if(pCommRam->CSPN == 3)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[2][pTerminalRam->LinkCount_SelfAdaption].APNUserName, APN_USER_NAME_MAX_SIZE);
//				else 
//				{
//					MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
//				}
//			}
//			else 
//			{
//				MR(ADDR_DATABUFF, (u32)CommPara.APNUserName, APN_USER_NAME_MAX_SIZE);
//			}
			
			MC(0, ADDR_DATABUFF + APN_USER_NAME_MAX_SIZE, 1);
			TxByte += CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);	//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8txbuff[TxByte++]=',';


			//err20190528 ���ϸ�������Ӧ
			if(pCommRam->CSPN == 1)//�ƶ�
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{		
					MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN1PassWord, APN_USER_PWD_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000111_CommPara.APN2PassWord, APN_USER_PWD_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				
			}
			else if(pCommRam->CSPN == 2)//��ͨ
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{		
					MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN1PassWord, APN_USER_PWD_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{
					MR(ADDR_DATABUFF, (u32)E0000112_CommPara.APN2PassWord, APN_USER_PWD_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				
			}
			else if(pCommRam->CSPN == 3)//����
			{
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);					
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 3)
				{					
					MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN1PassWord, APN_USER_PWD_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
				if(pTerminalRam->LinkCount_SelfAdaption == 4)
				{					
					MR(ADDR_DATABUFF, (u32)E0000113_CommPara.APN2PassWord, APN_USER_PWD_MAX_SIZE);//�˴�32�ֽڲ�ȷ���Ƿ����
				}
			}
			else 
			{
				MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
			}

			
//			if(SelfAdaption_Temp.SelfAdaptionFlag == 0x01)//
//			{
//				if(pCommRam->CSPN == 1)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[0][pTerminalRam->LinkCount_SelfAdaption].APNPassWord, APN_USER_PWD_MAX_SIZE);
//				else if(pCommRam->CSPN == 2)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[1][pTerminalRam->LinkCount_SelfAdaption].APNPassWord, APN_USER_PWD_MAX_SIZE);
//				else if(pCommRam->CSPN == 3)
//					MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[2][pTerminalRam->LinkCount_SelfAdaption].APNPassWord, APN_USER_PWD_MAX_SIZE);
//				else 
//				{
//					MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
//				}
//			}
//			else 
//			{
//				MR(ADDR_DATABUFF, (u32)CommPara.APNPassWord, APN_USER_PWD_MAX_SIZE);
//			}
			
			MC(0, ADDR_DATABUFF + APN_USER_PWD_MAX_SIZE, 1);
		
			TxByte += CopyString((u8 *)ADDR_DATABUFF,p8txbuff+TxByte);	//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8txbuff[TxByte++]='"';
			p8txbuff[TxByte++]='\r';
			break;

		case AT_MYNETACT:
			if(((pTerminalRam->LinkCount_SelfAdaption >= 3) && (pTerminalRam->SelfAdaptionChangeFlag == 1))|| (pTerminalRam->GPRS_Para_Reset_Flag == 1) )//�����л���ʶ
			//if(pTerminalRam->GPRS_Para_Reset_Flag == 1) //�����л���ʶ
			{//�л�APN���û������������ȥ����
				TxByte=CopyString((u8 *)"AT$MYNETACT=0,0\r",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			else 
			{
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			break;
		case AT_MYIPFILTER://IP���ʿ�������
			if(pUartCtrl->SubTask >= 5)
			{
				GPRS_QCSG_NextATCount();//��1��AT
				return;
			}		
			//x = MRR(ADDR_IPFILTER+(4*UART1Ctrl->SubTask),4);
			x = 0;
			if(x == 0)
			{
				pUartCtrl->SubTask++;//���μ���
				return;
			}
			TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8txbuff[TxByte]='"';
			TxByte++;
			for(i=0;i<4;i++)
			{
				y=hex_bcd(x&0xff);//HEX��תΪ8λѹ��BCD��
				x>>=8;
				TxByte+=BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
				p8txbuff[TxByte]='.';
				TxByte++;
			}
			TxByte--;//IP��4���ֽڵ�'.'ɾȥ
			p8txbuff[TxByte++] = '"';
			p8txbuff[TxByte++] = ',';
			p8txbuff[TxByte++] = '"';
			TxByte += CopyString((u8 *)"255.255.255.255",p8txbuff+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8txbuff[TxByte++] = '"';
			break;
	#if (USER/100)==0//�û���ʶ,���Ժ���Բ�������AT
		case AT_CMFG://���ö���Ϣģʽ
		case AT_ZCVF://�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
		case AT_CTA://���ߵȴ�ʱ��(��CDMAʱ��Ҫ);�����ݴ���ʱ����10���CDMAͨ��ģ�������ͷſ��нӿ���Դ
			GPRS_QCSG_NextATCount();//��1��AT
			return;
	#else
		#if (USER/100)==14//����ϵ
			case AT_ZCVF://�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
//			case AT_CTA://���ߵȴ�ʱ��(��CDMAʱ��Ҫ);�����ݴ���ʱ����10���CDMAͨ��ģ�������ͷſ��нӿ���Դ
				GPRS_QCSG_NextATCount();//��1��AT
				return;
		#else
			case AT_ZCVF://�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
			case AT_CTA://���ߵȴ�ʱ��(��CDMAʱ��Ҫ);�����ݴ���ʱ����10���CDMAͨ��ģ�������ͷſ��нӿ���Դ
				if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
				{//GSM����֧��GPRS
					GPRS_QCSG_NextATCount();//��1��AT
					return;
				}
				else
				{//����CDMA(MC8332)
					TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				}
				break;
		#endif//(USER/100)==14//����ϵ
	#endif
		case AT_MYNETSRV://���÷������
			pUartCtrl->State = GPRS_STATE_CONFIG_LINK_PARA;
			MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			switch(pUartCtrl->SubTask)//������
			{
				case 0://ȷ������ģʽ
					if(CommPara.IpLinkType == 0xFF) CommPara.IpLinkType = 0;
					i = CommPara.IpLinkType << 7;
					//ԭ����ģʽ(ͬF8������)B7 = 0:TCP,= 1:UDP; B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����;
					if(CommPara.CommunicationMode == 0xFF) CommPara.CommunicationMode = 0;
					i += CommPara.CommunicationMode << 4;
					pUartCtrl->TCPUDP = i;		
					i &= 0x30;
					if((pUartCtrl->TCPUDP & 0x80) != 0)
					{// UDP
						if(i == 0x20)
						{// =2������ģʽ
							pUartCtrl->SubTask = 4;
						}
						else
						{//=0���ģʽ=1�ͻ���ģʽ
							pUartCtrl->SubTask = 3;
						}
					}
					else
					{// TCP
						if(i==0x20)
						{// =2������ģʽ
							pUartCtrl->SubTask = 2;
						}
						else
						{// =0���ģʽ=1�ͻ���ģʽ
							pUartCtrl->SubTask =1 ;
						}
					}
					return;
				case 1://TCP�ͻ���ģʽ
				//default://TCP�ͻ���ģʽ
					//AT$MYNETSRV=0,0,0,0,"172.22.33.2:5000" 
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				TCPClient:
					p8txbuff[TxByte++] = '"';
		
					//err20190528 ���ϸ�������Ӧ
					if(pTerminalRam->LinkCount_SelfAdaption == 0)//0 1 2
					{
						MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
					}
					else if(pTerminalRam->LinkCount_SelfAdaption == 1)//0 1 2
					{
						MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr1, 6);
					}
					else if(pTerminalRam->LinkCount_SelfAdaption == 2)//0 1 2
					{
						MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr2, 6);
					}
					else if(pTerminalRam->LinkCount_SelfAdaption == 3)
					{
						if(pCommRam->CSPN == 1)//�ƶ�
							MR(ADDR_DATABUFF, (u32)E0000111_CommPara.MasterAddr, COMM_AADR_SIZE);
						else if(pCommRam->CSPN == 2)//��ͨ
							MR(ADDR_DATABUFF, (u32)E0000112_CommPara.MasterAddr, COMM_AADR_SIZE);
						else if(pCommRam->CSPN == 3)//����
							MR(ADDR_DATABUFF, (u32)E0000113_CommPara.MasterAddr, COMM_AADR_SIZE);
						else 
							MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
					}
					else if(pTerminalRam->LinkCount_SelfAdaption == 4)
					{
						if(pCommRam->CSPN == 1)//�ƶ�
							MR(ADDR_DATABUFF, (u32)E0000111_CommPara.MasterBackAddr1, COMM_AADR_SIZE);
						else if(pCommRam->CSPN == 2)//��ͨ
							MR(ADDR_DATABUFF, (u32)E0000112_CommPara.MasterBackAddr1, COMM_AADR_SIZE);
						else if(pCommRam->CSPN == 3)//����
							MR(ADDR_DATABUFF, (u32)E0000113_CommPara.MasterBackAddr1, COMM_AADR_SIZE);
						else 
							MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
					}
					/*
					else if(pTerminalRam->LinkCount_SelfAdaption <= 2)
					{
						if(pUartCtrl->LinkCount == 0)
						{//��ip
							if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
							{
								MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
							}
							else
							{		
								pUartCtrl->LinkCount = 1;
								pTerminalRam->LinkCount_SelfAdaption = 1;
							}
						}
						if(pUartCtrl->LinkCount == 1)
						{//����ip1
							if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
							{
								MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr1, 6);
							}
							else
							{
								pUartCtrl->LinkCount = 2;
								pTerminalRam->LinkCount_SelfAdaption = 2;
							}
						}
						if(pUartCtrl->LinkCount == 2)
						{//����iP2
							if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
							{
								MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr2, 6);
							}
							else
							{
								pUartCtrl->LinkCount = MASTER_ADDR_NUM;
								pTerminalRam->LinkCount_SelfAdaption = 3;
							}
						}
						if(pUartCtrl->LinkCount >= MASTER_ADDR_NUM)
						{
							pTerminalRam->LinkCount_SelfAdaption = 0;
							pUartCtrl->LinkCount = 0;
							pUartCtrl->LinkCtrl = 0;
	//						WarningOccur(GPRSPORT,(u8 *)"Զ��������վ����ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
							pUartCtrl->State = GPRS_STATE_LINK_FAIL;
							//err20190313 �޸�GPRS���̣�����IP��������ʧ�ܲ�ֱ������ģ�飬��������ͨ�Ų������ٳ��ԡ�
							pTerminalRam->GPRS_Para_Reset_Count++;
							if(pTerminalRam->GPRS_Para_Reset_Count>=3)//��ѭ�����Գ���3����Ӳ������ģ��
							{
								pTerminalRam->GPRS_Para_Reset_Count = 0;
								pTerminalRam->GPRS_Para_Reset_Flag = 0;
								pUartCtrl->ATCount = AT_MYPOWEROFF;		
							}
							else 
							{
								pTerminalRam->GPRS_Para_Reset_Flag = 1;
								pUartCtrl->ATCount = AT_MYNETACT;//����APN����
							}
							//pUartCtrl->ATCount = AT_MYPOWEROFF;		// �ػ�
							return;
						}
					}*/


					
					//MR((u32)&SelfAdaption_Temp, (u32)ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));

//					if(SelfAdaption_Temp.SelfAdaptionFlag == 0x01)//
//					{
//						if(pTerminalRam->LinkCount_SelfAdaption < 2)
//						{
//					
//							if(pCommRam->CSPN == 1)
//								MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[0][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//							else if(pCommRam->CSPN == 2)
//								MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[1][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//							else if(pCommRam->CSPN == 3)
//								MR(ADDR_DATABUFF, (u32)SelfAdaptionParaArry[2][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//							else 
//							{
//								MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
//							}

//							//����FLASH�е�ͨ�Ų���������Һ���鿴��ǰͨ�Ų���
//							if((pCommRam->CSPN <= 3) && (pCommRam->CSPN >= 1))
//							{
//								MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
//								MR((u32)CommPara.APN , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].APN, APN_NAME_MAX_SIZE);
//								MR((u32)CommPara.APNUserName , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].APNUserName, APN_USER_NAME_MAX_SIZE);
//								MR((u32)CommPara.APNPassWord , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].APNPassWord, APN_USER_PWD_MAX_SIZE);
//								MR((u32)CommPara.MasterAddr , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//								MR((u32)CommPara.MasterBackAddr1 , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
//								MR((u32)CommPara.MasterBackAddr2 , (u32)SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddr, 6);
// //								CommPara.MasterAddrType = SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType;
// //								CommPara.MasterBackAddr1Type = SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType;
// //								CommPara.MasterBackAddr2Type = SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType; //�˴�Һ����ʾ�Բ���
//								MR((u32)&CommPara.MasterAddrType , (u32)&SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType, 1);
//								MR((u32)&CommPara.MasterBackAddr1Type , (u32)&SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType, 1);
//								MR((u32)&CommPara.MasterBackAddr2Type , (u32)&SelfAdaptionParaArry[pCommRam->CSPN-1][pTerminalRam->LinkCount_SelfAdaption].MasterAddrType, 1);
//								MW((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
//							}				
//						}
//						else 
//						{
//							pTerminalRam->LinkCount_SelfAdaption = 0;
//							pTerminalRam->SelfAdaptionChangeFlag = 0;
//							pUartCtrl->LinkCount = 0;
//							pUartCtrl->LinkCtrl = 0;
//	//						WarningOccur(GPRSPORT,(u8 *)"Զ��������վ����ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
//							pUartCtrl->State = GPRS_STATE_LINK_FAIL;
//							pUartCtrl->ATCount = AT_MYPOWEROFF;		// �ػ�
//							return;
//						}
//					}
//					else
//					{
//					
//						if(pUartCtrl->LinkCount == 0)
//						{//��ip
//							if(CommPara.MasterAddrType == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
//							{
//								MR(ADDR_DATABUFF, (u32)CommPara.MasterAddr, 6);
//							}
//							else
//							{
//								pUartCtrl->LinkCount = 1;
//							}
//						}
//						if(pUartCtrl->LinkCount == 1)
//						{//����ip1
//							if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
//							{
//								MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr1, 6);
//							}
//							else
//							{
//								pUartCtrl->LinkCount = 2;
//							}
//						}
//						if(pUartCtrl->LinkCount == 2)
//						{//����iP2
//							if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_GPRS) //ͨ��ͨ�����ͣ�02:GPRS/CDMA
//							{
//								MR(ADDR_DATABUFF, (u32)CommPara.MasterBackAddr2, 6);
//							}
//							else
//							{
//								pUartCtrl->LinkCount = MASTER_ADDR_NUM;
//							}
//						}
//						if(pUartCtrl->LinkCount >= MASTER_ADDR_NUM)
//						{
//							pUartCtrl->LinkCount = 0;
//							pUartCtrl->LinkCtrl = 0;
//	//						WarningOccur(GPRSPORT,(u8 *)"Զ��������վ����ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
//							pUartCtrl->State = GPRS_STATE_LINK_FAIL;
//							//err20190313 �޸�GPRS���̣�����IP��������ʧ�ܲ�ֱ������ģ�飬��������ͨ�Ų������ٳ��ԡ�
//							pTerminalRam->GPRS_Para_Reset_Count++;
//							if(pTerminalRam->GPRS_Para_Reset_Count>=3)//��ѭ�����Գ���3����Ӳ������ģ��
//							{
//								pTerminalRam->GPRS_Para_Reset_Count = 0;
//								pTerminalRam->GPRS_Para_Reset_Flag = 0;
//								pUartCtrl->ATCount = AT_MYPOWEROFF;		
//							}
//							else 
//							{
//								pTerminalRam->GPRS_Para_Reset_Flag = 1;
//								pUartCtrl->ATCount = AT_MYNETACT;//����APN����
//							}
//							//pUartCtrl->ATCount = AT_MYPOWEROFF;		// �ػ�
//							return;
//						}
//					}
					
					p8 = (u8*)(ADDR_DATABUFF);
					for(i = 0; i < 4; i++)
					{
						y = hex_bcd(p8[5-i]);//HEX��תΪ8λѹ��BCD��
						TxByte += BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
						p8txbuff[TxByte++] = '.';
					}
					TxByte--;//IP��4���ֽڵ�'.'ɾȥ
					//Ŀ�Ķ˿ں�
					p8txbuff[TxByte++] = ':';
					y = hex_bcd(p8[0] + (p8[1]<<8));//HEX��תΪ8λѹ��BCD��
					TxByte += BCD_ASCII(y,((u32)p8txbuff)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8txbuff[TxByte++] = '"';
					p8txbuff[TxByte++] = '\r';
					break;
				case 2://TCP������ģʽ
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,1,0,",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					p8txbuff[TxByte++] = '"';
					TxByte += CopyString((u8 *)"127.0.0.1",p8txbuff+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					p8txbuff[TxByte++] = ':';
					//������ģʽ,�˿ں����趨
					i = CommPara.TcpPortNumber;
					i = hex_bcd(i);//HEX��תΪ8λѹ��BCD��		
					TxByte += BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8txbuff[TxByte++] = '"';
					break;
				case 3://UDP�ͻ���ģʽ
					TxByte = CopyString((u8 *)"AT$MYNETSRV=0,0,2,0,",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					goto TCPClient;
				case 4://UDP������ģʽ
					//�޴�ģʽ
					WarningOccur(GPRSPORT,(u8 *)"Զ����������ģʽ��");//�澯����,��ڸ澯����͸澯�ַ���
					TxByte = CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					goto TCPClient;
			}
			break;	
		case AT_MYNETOPEN://��������		
			if(pUartCtrl->LinkCtrl == 0)//���ӿ���0=������,1=����
			{
				GPRS_QCSG_NextATCount();//��1��AT
			}
			else
			{
				pUartCtrl->State = GPRS_STATE_LINKING;
				if(pTerminalRam->GPRSReConnection_S_Timer != 0x0)		//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
				{
					WarningOccur(GPRSPORT,(u8 *)"Զ�������ز������ʱ");//�澯����,��ڸ澯����͸澯�ַ���
					GPRS_QCSG_NextATCount();
				}
				else
				{
					TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					//pTerminalRam->GPRSWaitConnection_S_Timer = 90;//20 GPRS�������ӷ���ȴ��붨ʱ��
					pTerminalRam->GPRSReConnection_S_Timer = 30;
					pUartCtrl->NETOPENCount++;//���������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
				}	
			}
			break;
		case AT_LOOP_NOLINK://ATѭ��,û����
			pUartCtrl->LinkSubTask = 0;	// ����AT���ͼ���
			pUartCtrl->SubTask = 0;		// ������
			MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			if(pUartCtrl->NETOPENCount >= CommPara.RedialCnt)//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
			{
				pUartCtrl->NETOPENCount = 0;			// ��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
				//pTerminalRam->LinkCount_SelfAdaption ++ ;	//�л�����������
				if(pTerminalRam->LinkCount_SelfAdaption <= 2)
				{
					pUartCtrl->LinkCount++; 
					pTerminalRam->LinkCount_SelfAdaption++;
					if(pTerminalRam->LinkCount_SelfAdaption == 3)//�л�������Ӧ���� ��������APN
					{
						pTerminalRam->SelfAdaptionChangeFlag = 1;
						pUartCtrl->ATCount = AT_MYNETACT;//����APN����
					}
				}
				else if(pTerminalRam->LinkCount_SelfAdaption > 2)
				{
					pTerminalRam->LinkCount_SelfAdaption++;
					if(pTerminalRam->LinkCount_SelfAdaption >= 5)//����ͨѶ�������Ѿ��Թ�
					{
						pTerminalRam->LinkCount_SelfAdaption = 0;
						pUartCtrl->LinkCount = 0;
						pUartCtrl->LinkCtrl = 0;
	//						WarningOccur(GPRSPORT,(u8 *)"Զ��������վ����ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
						pUartCtrl->State = GPRS_STATE_LINK_FAIL;
						//err20190313 �޸�GPRS���̣�����IP��������ʧ�ܲ�ֱ������ģ�飬��������ͨ�Ų������ٳ��ԡ�
						pTerminalRam->GPRS_Para_Reset_Count++;
						if(pTerminalRam->GPRS_Para_Reset_Count>=3)//��ѭ�����Գ���3����Ӳ������ģ��
						{
							pTerminalRam->GPRS_Para_Reset_Count = 0;
							pTerminalRam->GPRS_Para_Reset_Flag = 0;
							pUartCtrl->ATCount = AT_MYPOWEROFF; 	
						}
						else 
						{
							pTerminalRam->GPRS_Para_Reset_Flag = 1;
							pUartCtrl->ATCount = AT_MYNETACT;//����APN����
						}
					}
					else 
					{
						pTerminalRam->SelfAdaptionChangeFlag = 1;
						pUartCtrl->ATCount = AT_MYNETACT;//����APN����
					}
				}
				
				
			}
			else
			{
				if(pUartCtrl->LINKCHECK_RTCS != pCommRam->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
				{
					pUartCtrl->LINKCHECK_RTCS = pCommRam->RTCBuff[0];
					if(pTerminalRam->GPRSReConnection_S_Timer == 0x0)//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
					{
						pUartCtrl->ATCount = AT_MYNETSRV;//���÷������
					}
					else
					{
						pUartCtrl->ATCount = AT_SMS_NOLINK;//���ŷ���û����ǰ
					}
				}
			}
			break;
		case AT_CSQ_LINK://���ź����Ӻ�(ÿ��1��)
			if(pUartCtrl->LINKCHECK_RTCS != pCommRam->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
			{
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			else
			{
				GPRS_QCSG_NextATCount();//��1��AT
				return;
			}
			break;
		case AT_SYSINFO:
		case AT_SYSINFO_LINK://
			//#if (USER/100)==14//����ϵ
				if(pUartCtrl->LINKCHECK_RTCS != pCommRam->RTCBuff[0])//
				{
					TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//
				}
				else
				{
					GPRS_QCSG_NextATCount();
					return;
				}
				break;
//			#else
//				GPRS_QCSG_NextATCount();
//				return;
			//#endif
		case AT_MYNETOPEN_LINK://���Ӽ��(ÿ��1��)
			if(pUartCtrl->LINKCHECK_RTCS != pCommRam->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
			{
				pUartCtrl->LINKCHECK_RTCS = pCommRam->RTCBuff[0];
				TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			else
			{
				GPRS_QCSG_NextATCount();//��1��AT
				return;
			}
			break;
		case AT_MYNETREAD://��ȡ����
			p16 = (u16*)(ADDR_UART1IntRx);
			if(p16[0] != p16[1])
			{
				pUartCtrl->ATTask = 0;//AT����:0=����,1=����,2=���ڷ���,3=����
				return;
			}
			else
			{
				TxByte=CopyString((u8 *)"AT$MYNETREAD=0,",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				i = GPRS_MAX_WRITE_DATA_LEN;
				i = hex_bcd(i);//HEX��תΪ8λѹ��BCD��
				TxByte += BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
				p8txbuff[TxByte++] = '\r';
			}
			break;
		case AT_MYNETWRITE://��������
			switch(pUartCtrl->ATWriteTask)					// AT���ݷ�������:0=������,1=������
			{
				case 0://��ATͷ
					if(pTerminalRam->AT_MS_Timer != 0) return;
					i = pTerminalRam->CopyTxByte;			// ���跢���ֽ���
					if(pUartCtrl->Task != UART_TASK_TXING)
					{
						pUartCtrl->ATTask = 0;				// AT����:0=����,1=����,2=���ڷ���,3=����
						pUartCtrl->TxByte = 0;
						GPRS_QCSG_NextATCount();
						return;
					}
					if((i == 0) || (i > GPRS_MAX_WRITE_DATA_LEN))
					{
						pUartCtrl->Task = UART_TASK_RX;
						pUartCtrl->ATTask = 0;				// AT����:0=����,1=����,2=���ڷ���,3=����
						pUartCtrl->TxByte = 0;
						pTerminalRam->CopyTxByte = 0;
						WarningOccur(GPRSPORT,(u8 *)"Զ�����߷��ͳ��ȴ���");//�澯����,��ڸ澯����͸澯�ַ���
						GPRS_QCSG_NextATCount();
						return;
					}
					TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);
					i = hex_bcd(i);
					TxByte += BCD_ASCII(i,((u32)p8txbuff)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8txbuff[TxByte++] = '\r';
					break;
				case 1://������
					i = pTerminalRam->ReqTxByte;
					x = pTerminalRam->CopyTxByte;
					y = pTerminalRam->CopyTxCount;
					if((i > GPRS_MAX_WRITE_DATA_LEN) || (i == 0) || (x == 0))
					{//�ɷ��ͳ��ȴ���,������֡
						pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
						pUartCtrl->Task = UART_TASK_TX_DONE;		// 0=����
						GPRS_QCSG_NextATCount();					// ��1��AT
						return;
					}
					pTerminalRam->CopyTxByte = x - i;				// ���跢���ֽ�
					pTerminalRam->CopyTxCount = y + i;				// �ѷ��ͼ���				
					pUartCtrl->TxByte = i;
				    pUartCtrl->TxCount = LEN_ATTx + y;
					#if LOG_1376_3==1//0=û,1=��
						LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx+LEN_ATTx+y,i,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
					#endif
					Start_UARTn_Tx(GPRSPORT);//����UART�ڷ���
					
					x = MRR((u32)&pUartCtrl->RxTxByte,4);
					x += i;
					MWR(x, (u32)&pUartCtrl->RxTxByte, 4);//ͨ������
					pUartCtrl->ATTask = 2;//AT����:0=����,1=����,2=���ڷ���,3=����
					return;
				default:
					pUartCtrl->ATWriteTask = 0;//AT���ݷ�������:0=������,1=������
					break;
			}
			break;
		case AT_MYFTP://FTP�ļ�����
			if(pUartCtrl->LinkTask != 101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{
				GPRS_QCSG_NextATCount();//��1��AT
				return;
			}
			// FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
			// �������
			FTPFileCount = MRR(ADDR_FTPFileFlags + 4, 1);//�����ļ�����;0-x,=MaxFileNum��FTP,=(MaxFileNum+1)����֡,=(MaxFileNum+2)���������ʾ,=(MaxFileNum+3)����������ʾ
			if(FTPFileCount > (MaxFileNum + 1))		// ��������ļ�����
			{
				pUartCtrl->LinkTask2 = 0;			// ͨ��2��������(FTP����)
				pUartCtrl->FTPLinkCount = 0;		// FTP�������Ӽ���
				pUartCtrl->FTPErr = 0;				// FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				GPRS_QCSG_NextATCount();			// ��1��AT
				return;
			}
			pCommRam->DisplayAllErrorTimer = 10;//FTP�ļ�������ʾ�붨ʱ��
			x = pUartCtrl->FTPErr;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
			if((x != 0) || (FTPFileCount == (MaxFileNum + 1)))
			{
				if(x == 1)
				{//�������޷���¼
					y=0xFFFFFFFE;//FTP �������޷���¼����д0xFFFFFFFEH
				}
				else
				{
					if(x==2)
					{//���������Ҳ���ָ���ļ�
						y=0xFFFFFFFD;//FTP ���������Ҳ���ָ���ļ�����д0xFFFFFFFDH
					}
					else
					{
						for(i=0;i<MaxFileNum;i++)
						{
							x=MRR(ADDR_FTPFileFlags+5+i,1);
							if(x==5)//5=�ļ�����
							{
								break;
							}
						}
						if(i<MaxFileNum)
						{//���ļ�����
							y=0xFFFFFFFF;//��鷢���ļ�������д0xFFFFFFFFH
						}
						else
						{
							y=0;
						}
					}
				}
				MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//FTP�ļ��������ѷ���֡��־
				
				p8rxbuff=(u8 *)ADDR_DATABUFF;
				p8txbuff=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MR(ADDR_DATABUFF,ADDR_AFN0FF1_Rx,256);//�ָ�FTP����֡��ǰ256�ֽ�
				MR((u32)p8txbuff,ADDR_AFN0FF1_Tx,18);//�ָ�FTP����֡��ǰ18�ֽ�
				MWR(y,((u32)p8txbuff)+18,4);//���ر�ʶ4�ֽ�
				i=4+4;// ���س���
				i+=8;// 1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
				p8txbuff[1]=i;
				p8txbuff[2]=i>>8;
				i=p8rxbuff[1]+(p8rxbuff[2]<<8);
				//Uart_376_AUX(GPRSPORT,((u32)p8rxbuff)+6+i-6);//������Ϣ(EC,TP),�ݵȴ���֡���ݷ���
				//Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				
				GPRS_QCSG_NextATCount();//��1��AT
				pUartCtrl->ATTask = 0;//AT����:0=����,1=����,2=���ڷ���,3=����
				return;
			}
			//#if Extern_GPRS_FTP_RxTx==0//�ⲿ����:0=I2,1=E1,2=E2,3=;W5200 GPRS�ļ�����
			//		TxByte=GPRS_FTP_Tx();//GPRS_FTP_Tx����
			//#else
			//		if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			//		{
		    //		//p32=(u32 *)(ADDR_E1_ENTRY);
		    //		TxByte=(*(u32(*)())p32[106])();//GPRS_FTP_Tx����
			//		}
			//#endif
			break;		
		case AT_SMS_NOLINK://���ŷ���û����ǰ
		case AT_SMS_LINK://���ŷ���
			#if LEN_UART11Ctrl == 0
				GPRS_QCSG_NextATCount();//��1��AT
			#else
				TxByte = GPRS_SMS_Tx();//GPRS_SMS_Tx����;���ط����ֽ���
			#endif
			break;	
		case AT_LOOP://ATѭ��,���߿���
			if(pUartCtrl->LinkCtrl == 0)//���ӿ���0=������,1=����
			{
				if(pUartCtrl->LinkTask == 101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					pUartCtrl->LinkTask = 102;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					pUartCtrl->HeartberatCount = 0;//������������,�յ�ȷ��֡��0
					pTerminalRam->GPRSHeartBeat_S_Timer = 0;
				}
				if(pUartCtrl->LinkTask == 103)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
					return;
				}
			}
			pUartCtrl->LinkSubTask = 0;		
			pUartCtrl->SubTask = 0;			
			pUartCtrl->ATCount = AT_CSQ_LINK;
			break;	
		case AT_MYRESET://err20190410 GPRSģ������ָ�� ����SIM��δ����ʱ����GPRS��Դ
			TxByte=CopyString((u8 *)"AT+CFUN=1,1\r",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			break;
		default:
			TxByte = CopyString((u8 *)p32[(4*AT_No)+1],p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			break;
	}
	if(TxByte != 0)
	{
		#if (TEST_ON == 1)
		switch(pUartCtrl->BpsCtrlBack)
		{
			
				case 0://����115200
					pUartCtrl->BpsCtrl=0x3+(10<<5);
					break;			
				case 1://����38400
					pUartCtrl->BpsCtrl=0x3+(8<<5);
					break;				
				case 2://����9600
					pUartCtrl->BpsCtrl=0x3+(6<<5);
					break;
			
		}
		#else
			pUartCtrl->BpsCtrl = 0x3+(9<<5);//����57.6K 
		#endif
		Init_UARTn(GPRSPORT);//��ʼ��UART��,���ͨ�ſ�����������
		pUartCtrl->TxByte = TxByte;//�����ֽ���(���ֶ���)
		pUartCtrl->TxCount = 0;
		#if LOG_1376_3==1//0=û,1=��
			switch(p32[4*AT_No])//AT����
			{
				case AT_CSQ_LINK://���ź����Ӻ�(ÿ��1��)
				case AT_MYNETOPEN_LINK://���Ӽ��(ÿ��1��)
				case AT_MYNETREAD://������			
					break;
				default:
					LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
					break;
			}
		#endif
		Start_UARTn_Tx(GPRSPORT);//����UART�ڷ���
		pUartCtrl->ATTask = 2;//AT����:0=����,1=����,2=���ڷ���,3=����
	}
}


void GPRS_QCSG_AT_Rx(void)//AT�������
{
	u32 i, x, y, AT_No, RxByte, Addr, p0, p1;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;
	u16 *p16fifo;
	u32 *p32;
	CommunicationParaStruct CommPara;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;
	u32 Temp_Code;
	DMA_UARTn_RX_Pointer(GPRSPORT);//����DMA_UARTn����ָ��ֵ(p0)
	AT_No = pUartCtrl->ATCount;//AT����
	if(AT_No > AT_Max)
	{
		AT_No = 0;
		pUartCtrl->ATCount = 0;//AT����
	}
	p16fifo = (u16*)(ADDR_UART1IntATRx);
	if(p16fifo[0] != p16fifo[2])
	{//���ֽڽ���
		p16fifo[2] = p16fifo[0];
		pTerminalRam->AT_MS_Timer = 50/10;		//AT���յ�һ���ֽں�ʱ��ʱֵ
	}

	if(pTerminalRam->AT_MS_Timer != 0) return;
	p32 = (u32*)(&GPRS_QCSG_AT_Command_Lib);
	switch(p32[4*AT_No])//AT����
	{
		case AT_AT:// �����ͼ�Ҫ���ϵ���ʼ������ATָ���Ӧ�𿪻��ɹ������������������Σ����30�Σ�ÿ��һ��
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���

			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// ��һ��ATָ��ͼ��ʱ�䣬 1S
		  	pUartCtrl->ATTask = 0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr == 0)
			{//�ط���AT
				pUartCtrl->LinkSubTask++;				// ����AT���ͼ���
				if(pUartCtrl->LinkSubTask >= 30)		// ����AT���ͼ���,30�� //err20180626 ̨�����ʱ30�θ�Ϊ5��
				{										// ����MC8332ģ�鵱�Զ�����ʱͷ2�ε�AT����Ӧ(���ڼ��),��3�β���Ӧ
					pUartCtrl->LinkSubTask=0;			// ����AT���ͼ���
					switch(pUartCtrl->SubTask)			// ������
					{
					#if (TEST_ON == 0)
						case 0://����57600
						case 1://����115200
						case 2://����38400
						case 3://����19200
						case 4://����9600
					#endif	
					#if (TEST_ON == 1)
						case 0://����57600						
						case 1://����38400						
						case 2://����9600
					#endif	
							pUartCtrl->SubTask++;//������
							return;
						default:
							#if (USER/100)==0//�û���ʶ,���Ժ���Բ�֧��AT+IPR
								WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
								GPRS_QCSG_NextATCount();//��1��AT
							#else
								WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
								pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
							#endif
							return;
					}
				}
				pUartCtrl->State = GPRS_STATE_MATCH_BAUD_RATE;
			}
			else
			{//���յ� OK
				p16fifo[1] = Addr - (ADDR_UART1IntATRx + 10);//�޸��Ѷ�ָ��
				
			#if (TEST_ON == 0)
				if(pUartCtrl->SubTask < 10)//������
				{
					pUartCtrl->SubTask += 10;//������
					pUartCtrl->LinkSubTask = 0;//����AT���ͼ���
				}
				else
				{
					GPRS_QCSG_NextATCount();//��1��AT
				}
			#else 
				pUartCtrl ->BpsCtrlBack = pUartCtrl->SubTask;
				GPRS_QCSG_NextATCount();//��1��AT
			#endif
			}
			return;
		case AT_MYTYPE://��ѯԶ��ͨ��ģ������
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return;

			#if LOG_1376_3==1	
				LOG_ATRx(0);			//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			pCommRam->RemoteWirelessModuleType = 1;		// GPRSģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
			Addr = ATRxSearch((u32)((u8*)"$MYTYPE:"));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				x = ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(x != 0)
				{
					p8s = (u8*)x;
					i = ASCII_BCD(x,2);
					pCommRam->RemoteWirelessModuleType = i;//GPRSģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
				}
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_QCSG_NextATCount();
				break;
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				GPRS_QCSG_NextATCount();
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2) pUartCtrl->LinkSubTask++;	//����AT���ͼ���
			}
			break;	
		case AT_MYGMR://��ȡģ������汾
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK"));		// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				GPRS_QCSG_NextATCount();
				break;
			}
			goto ReTxAT;//�ط���AT
		case AT_CPIN://����飬�����洢
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pUartCtrl->State = GPRS_STATE_RECONGNIZE_SIM_CARD;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"READY"));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_QCSG_NextATCount();
				break;
			}
			pUartCtrl->LinkSubTask++;//����AT���ͼ���
			if(pUartCtrl->LinkSubTask > 2)//����AT���ͼ���
			{
				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
				pUartCtrl->ATCount = AT_MYRESET;//�ػ�
			}
			return;
			//goto ReTxAT;//�ط���AT
		case AT_MYCCID://��ȡSIM�����к�ICCID
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pUartCtrl->State = GPRS_STATE_RECONGNIZE_SIM_CARD;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"$MYCCID"));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				LineATIntRx(Addr+3,ADDR_DATABUFF,20);//ȡAT�жϽ����������Է��ڻ�����
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_QCSG_NextATCount();//��1��AT
				break;
			}
			pUartCtrl->State = GPRS_STATE_RECONGNIZE_SIM_CARD;
			goto ReTxAT;//�ط���AT
		case AT_CNUM://ȡ��������
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pUartCtrl->State = GPRS_STATE_RECONGNIZE_SIM_CARD;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			p8d = (u8*)(ADDR_MobilePhoneNumber);
			p8d[0] = 0;
			Addr = ATRxSearch((u32)((u8*)"+CNUM:"));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
				{//GSM����֧��GPRS
					Addr = ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr != 0)
					{
						LineATIntRx(Addr,ADDR_DATABUFF,20);//ȡAT�жϽ����������Է��ڻ�����
						p8s=(u8*)(ADDR_DATABUFF);
						if(p8s[0] == '"')
						{
							p8s++;
							for(i = 0; i < 19; i++)
							{
								x = p8s[i];
								if(x == '"') break;
								p8d[i] = x;
							}
							p8d[i] = 0;
						}
					}
				}
				else
				{//����CDMA(MC8332)
					LineATIntRx(Addr,ADDR_DATABUFF,20);//ȡAT�жϽ����������Է��ڻ�����
					p8s = (u8*)(ADDR_DATABUFF);
					for(i = 0; i < 20; i++)
					{
						x = p8s[i];
						if((x < 0x30) || (x > 0x39)) break;
						p8d[i] = x;
					}
					p8d[i] = 0;
				}
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2)//����AT���ͼ���
				{
					pUartCtrl->LinkSubTask++;//����AT���ͼ���
					Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr != 0)
					{
						p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
					break;
				}
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			GPRS_QCSG_NextATCount();//��1��AT
			break;
		case AT_CIMI://������ƶ�̨��ʶ����IMSI
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pUartCtrl->State = GPRS_STATE_REG_NET;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			if(Addr != 0)
			{
				if((pCommRam->RemoteWirelessModuleType & 0x3F) != 0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
				{//GSM����֧��GPRS
					p1 = p16fifo[1];
					p8s = (u8*)(ADDR_UART1IntATRx+10);
					while((p8s[p1] == 0x0d)||(p8s[p1] == 0x0a))
					{
						p1++;
						if(p1 >= (LEN_UART1IntATRx-10))
						{
							p1 -= (LEN_UART1IntATRx-10);
						}
					}
					Addr = ADDR_UART1IntATRx+10+p1;
				}
				else
				{//����CDMA(MC8332)
					Addr=ATRxSearch((u32)((u8*)"+CIMI: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr==0)
					{
						pUartCtrl->LinkSubTask++;//����AT���ͼ���
						if(pUartCtrl->LinkSubTask>2)//����AT���ͼ���
						{
							GPRS_QCSG_NextATCount();//��1��AT
						}
						Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
						if(Addr!=0)
						{
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						}
						break;
					}
				}
				LineATIntRx(Addr,ADDR_DATABUFF,19);//ȡAT�жϽ����������Է��ڻ�����
				p8s=(u8*)(ADDR_DATABUFF);
				p8d=(u8*)(ADDR_MobilePhoneNumber+20);
				for(i=0;i<19;i++)
				{
					if((p8s[i]==0x0d)||(p8s[i]==0x0a))
					{
						break;
					}
					p8d[i]=p8s[i];
				}
				p8d[i]=0;
				
				Temp_Code = 0;
				Temp_Code = (p8d[0]-'0')*10000 + 
							(p8d[1]-'0')*1000 +
							(p8d[2]-'0')*100 +
							(p8d[3]-'0')*10 +
							(p8d[4]-'0')*1;
				switch(Temp_Code)
				{
					case 46000:
					case 46002:
					case 46004:
					case 46007:
					case 46008:
						pCommRam->CSPN=1;	// 1=�й��ƶ�ͨ��-----china mobile  CMCC
						break;
					case 46001:
					case 46006:
					case 46009:
					case 46010:
						pCommRam->CSPN=2;	// 2=�й���ͨͨѶ-----china unicom  CUCC
						break;
					case 46003:
					case 46005:
					case 46011:
					case 46012:
						pCommRam->CSPN=3;	// 3=�й�����  ------CHINA TELECOM  CTCC
						break;
					default:
						pCommRam->CSPN=0;//��
						break;
				}
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2)//����AT���ͼ���
				{
					pUartCtrl->LinkSubTask++;//����AT���ͼ���
					break;
				}
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			GPRS_QCSG_NextATCount();//��1��AT
			break;
		case AT_CSPN://��ȡSIM��������Ӫ������
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pUartCtrl->State = GPRS_STATE_REG_NET;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;				// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"+CSPN: \""));// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				//�й��ƶ�ͨ��-----china mobile  CMCC
				//�й���ͨͨѶ-----china unicom  CUCC
				//�й�����  ------CHINA TELECOM  CTCC	
				x = GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				p8s = (u8*)(ADDR_DATABUFF);
				for(i = 0; i < x; i++)
				{
					if(p8s[i] == '"')
					{
						p8s[i] = 0;
						break;
					}
				}
				i = (p8s[0]<<24)|(p8s[1]<<16)|(p8s[2]<<8)|(p8s[3]<<0);
				switch(i)
				{
					case ('C'<<24)|('M'<<16)|('C'<<8)|('C'<<0):
						pCommRam->CSPN=1;	// 1=�й��ƶ�ͨ��-----china mobile  CMCC
						break;
					case ('C'<<24)|('U'<<16)|('C'<<8)|('C'<<0):
						pCommRam->CSPN=2;	// 2=�й���ͨͨѶ-----china unicom  CUCC
						break;
					case ('C'<<24)|('T'<<16)|('C'<<8)|('C'<<0):
						pCommRam->CSPN=3;	// 3=�й�����  ------CHINA TELECOM  CTCC
						break;
					default:
						pCommRam->CSPN=0;//��
						break;
				}
				WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			}	
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2)//����AT���ͼ���
				{
					pUartCtrl->LinkSubTask++;//����AT���ͼ���
					break;
				}
			}
			GPRS_QCSG_NextATCount();//��1��AT
			break;			
		case AT_CSQ_NOLINK://���ź�û����ǰ
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 100;				// ��һ��ATָ��ͼ��ʱ�䣬 1S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"+CSQ: "));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);	//�޸��Ѷ�ָ��
				pUartCtrl->LinkSubTask = 0;					//����AT���ͼ���
				pUartCtrl->SubTask = 0;						//������
				LineATIntRx(Addr,ADDR_DATABUFF,2);			//ȡAT�жϽ����������Է��ڻ�����
				GPRS_Signal(ADDR_DATABUFF);					//GPRS�ź�ǿ��0-4
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				if((pTerminalRam->SignaldBm >= 5) && (pTerminalRam->SignaldBm != 0x99))
				{//ģ��⵽�ź�,��û�ź���ȴ�120��
					GPRS_QCSG_NextATCount();
				}
				else
				{
					if(pTerminalRam->GPRSWaitTx_S_Timer == 0)//GPRS�ȴ��ź�ǿ�Ȳ�ѯ(����ǰ)��ʱ
					{
						WarningOccur(GPRSPORT,(u8 *)"Զ���������ź�");//�澯����,��ڸ澯����͸澯�ַ���
						pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
					}
				}
				return;
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			goto ReTxAT;//�ط���AT
		//���¼�����������ʧ��ʱ����ģ��,��ʹ���ն���
		case AT_MYNETCON_APN://����APN
		case AT_MYNETCON_USERPWD://����USERPWD:�û���������
		case AT_MYNETSRV://���÷������
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			//pUartCtrl->State = GPRS_STATE_REG_NET;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 80;				// ��һ��ATָ��ͼ��ʱ�䣬 0.8S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));	// AT���������������
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			else
			{
				if(pUartCtrl->LinkSubTask <= 2)//����AT���ͼ���
				{
					pUartCtrl->LinkSubTask++;//����AT���ͼ���
					break;
				}
			}
			GPRS_QCSG_NextATCount();
			break;	
		case AT_CREG://��ȡ��ǰ����ע��״̬
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pUartCtrl->State = GPRS_STATE_REG_NET;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 100;				// ��һ��ATָ��ͼ��ʱ�䣬 1S
			pUartCtrl->ATTask = 0;							// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"+CREG:"));		// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr = ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					i = MRR(Addr,1);
					Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr != 0)
					{
						p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
					switch(i)
					{
						//case '0'://û��ע�ᣬME���ڲ�û������ѰҪע����µ���Ӫ��
						//case '4'://δ֪ԭ��
						default:
							if(pTerminalRam->GPRSWaitTx_S_Timer == 0)//�ȴ�ע�ᶨʱ
							{
								WarningOccur(GPRSPORT,(u8 *)"Զ������ע��ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
								pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
							}
							return;
						case '1'://ע���˱�������
							GPRS_QCSG_NextATCount();//��1��AT
							break;
						case '2'://û��ע�ᣬ��MS������ѰҪע����µ���Ӫ��
							if(pTerminalRam->GPRSWaitTx_S_Timer == 0)//�ȴ�ע�ᶨʱ
							{
								WarningOccur(GPRSPORT,(u8 *)"Զ������ע����Ѱ��ʱ");//�澯����,��ڸ澯����͸澯�ַ���
								pUartCtrl->ATCount=AT_MYPOWEROFF;//�ػ�
							}
							return;
						case '3'://ע�ᱻ�ܾ�
							WarningOccur(GPRSPORT,(u8 *)"Զ������ע�ᱻ�ܾ�");//�澯����,��ڸ澯����͸澯�ַ���
							pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
							return;
						case '5'://ע������������
							WarningOccur(GPRSPORT,(u8 *)"Զ������ע����������");//�澯����,��ڸ澯����͸澯�ַ���
							GPRS_QCSG_NextATCount();//��1��AT
							break;
					}
					break;
				}
			}
			goto ReTxAT;//�ط���AT
		case AT_MYNETACT://������������
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pUartCtrl->State = GPRS_STATE_CONFIG_LINK_PARA;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 100;			// ��һ��ATָ��ͼ��ʱ�䣬 1S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK"));		// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			else
			{
				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
			}

			if((pTerminalRam->SelfAdaptionChangeFlag == 1) || (pTerminalRam->GPRS_Para_Reset_Flag == 1))
			{
				pUartCtrl->ATCount = AT_MYNETCON_APN;//����APN����
				pTerminalRam->SelfAdaptionChangeFlag = 0;
				pTerminalRam->GPRS_Para_Reset_Flag = 0;
			}
			else 
				GPRS_QCSG_NextATCount();
			break;
		case AT_MYIPFILTER://IP���ʿ�������
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;					// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK"));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				pUartCtrl->LinkSubTask = 0;//����AT���ͼ���
				pUartCtrl->SubTask++;//������
				break;
			}
			goto ReTxAT;
		case AT_MYNETOPEN://��������
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pUartCtrl->State = GPRS_STATE_LINKING;
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;					// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK"));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr - (ADDR_UART1IntATRx+10);	// �޸��Ѷ�ָ��
				WarningCancel(GPRSPORT);					// �澯ȡ��,��ڸ澯����
				//pUartCtrl->LinkTask = 100;
				pUartCtrl->LinkSubTask = 0;					// ����AT���ͼ���
				pUartCtrl->SubTask = 0;						// ������
				pUartCtrl->ATCount = AT_CSQ_LINK;			// ���ź����Ӻ�
				pUartCtrl->LinkCount = 0;					// ���ӳɹ������Ӵ������� 
				//pTerminalRam->LinkCount_SelfAdaption = 0;		// ���ӳɹ�������Ӧ���Ӵ�������
				//pTerminalRam->SelfAdaptionChangeFlag = 0;		// ���ӳɹ� ����Ӧ�����л���ʶ����
				MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
				MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
				i = CommPara.HeartBeatTimeGap;
				if(i == 0x0) i = 1;		//��С1��
				i *= 60;
				i += 60;					//��60����ܵ���վ������ʱ���
				pTerminalRam->GPRSHeartBeat_S_Timer = 0;
				if((pUartCtrl->TCPUDP & 0x30) == 0x20)//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
				{//�Ƿ�����ģʽ
					if((pUartCtrl->TCPUDP & 0x80) != 0)
					{//��UDP
						i = 6000;//UDP��ʼ�ȴ���վ���붨ʱ60��
					}
					pTerminalRam->GPRSHeartBeat_S_Timer = i;//GPRS�����붨ʱ��
				}			
				pUartCtrl->State = GPRS_STATE_LINK_SUCCESS;
			
			}
			else
			{
				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
				GPRS_QCSG_NextATCount();
			}
			break;		
		case AT_CSQ_NOLINK2://���ź�û����2	
		case AT_CSQ_LINK://���ź����Ӻ�
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			
			#if LOG_1376_3==1//0=û,1=��
				if(p32[4*AT_No] != AT_CSQ_LINK)//AT����
				{
					LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
				}
			#endif
			pTerminalRam->AT_MS_Timer = 80;				// ��һ��ATָ��ͼ��ʱ�䣬 0.8S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"+CSQ: "));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				LineATIntRx(Addr, ADDR_DATABUFF, 2);//ȡAT�жϽ����������Է��ڻ�����
				GPRS_Signal(ADDR_DATABUFF);//GPRS�ź�ǿ��0-4
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_QCSG_NextATCount();//��1��AT
				break;
			}
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			goto ReTxAT;//�ط���AT
		case AT_SYSINFO_LINK:
		case AT_SYSINFO://��ѯע���������ͣ�3G/4G
//			#if (USER/100)==14//����ϵ
				Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
				
				#if LOG_1376_3==1//0=û,1=��
					LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
				#endif
				pTerminalRam->AT_MS_Timer = 50;				// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
				pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
				pCommRam->RemoteWirelessModuleType = 1;		// Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
				Addr = ATRxSearch((u32)((u8*)"$MYSYSINFO: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					p8d = (u8 *)Addr;
					i = p8d[0]-0x30;
					switch(i)
					{		
						case 2://GSM/GPRSģʽ 2G
							pCommRam->RemoteWirelessModuleType = (1 << 0); 	//b0=1
							break;
						case 3://WCDMAģʽ 3G
							pCommRam->RemoteWirelessModuleType = (1 << 1); 	//b1=1
							break;
						case 4://17://LTEģʽ 4G
							pCommRam->RemoteWirelessModuleType = (1 << 5); 	//b5=1
							break;
						case 0://�޷���
						default:
							break;
					}
				}
//			#endif
			GPRS_QCSG_NextATCount();//��1��AT
			break;
		case AT_MYNETOPEN_LINK://��ѯ��������
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 80;				// ��һ��ATָ��ͼ��ʱ�䣬 0.8S
			pUartCtrl->ATTask = 0;						// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"$MYNETOPEN: 0"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				if(pUartCtrl->LinkTask < 101)
				{				
					pUartCtrl->LinkTask = 100;
				}
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);	// �޸��Ѷ�ָ��
				MC(0,ADDR_MobilePhoneNumber+40,16+6);
				//LocalIP
				p16fifo[1] += 2;
				Addr += 2;
				x = ATRxSearch((u32)((u8*)"\""));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(x == 0) 
				{
					x = ATRxSearch((u32)((u8*)","));
					p16fifo[1] -= 1;
					Addr -= 1;
				}
				if(x > Addr)
				{
					x -= Addr;
					x -= 1;
					if(x < 16)
					{
						MR(ADDR_MobilePhoneNumber+40, Addr, x);
						//��ʾ����IP
						CopyString((u8 *)"��IP ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						MR(ADDR_DATABUFF+5, Addr, x);
						MC(0, ADDR_DATABUFF+5+x, 1);
						WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
						//local_port
						Addr += x;
						Addr += 2;
						p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						x = ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
						if(x > Addr)
						{
							x -= Addr;
							x -= 1;
							if(x < 6) MR(ADDR_MobilePhoneNumber+40+16, Addr, x);
						}	
						CopyString((u8 *)"0.0.0.0",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						i = DataComp(ADDR_MobilePhoneNumber+40, ADDR_DATABUFF, 7);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
						if(i == 0)
						{//�Ͽ�����
							Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							if(Addr != 0)
							{
								p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
							}
							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
							pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
							return;
						}
					}
				}
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_QCSG_NextATCount();//��1��AT
				break;
			}
			goto ReTxAT;
		case AT_MYNETREAD:		// ��ȡ����
			Addr = ATRxSearch((u32)((u8*)"$MYNETREAD: 0,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			pTerminalRam->AT_MS_Timer = 50;			// ��һ��ATָ��ͼ��ʱ�䣬 0.7S
			pUartCtrl->ATTask = 0;					// AT����:0=����,1=����,2=���ڷ���,3=����
			p1 = p16fifo[1];						// ԭp1
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				MC(0, ADDR_DATABUFF, 64);
				i = GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				if(i != 0)
				{
					RxByte = ASCII_HEX(ADDR_DATABUFF);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
					i = GetATRxByte();//�жϽ���,���㵱ǰp0��Ӧp1����δ�����ֽ���
					if(i < (RxByte + 4))//���ֽ���Ϊ0ʱԭRxByte+6Ϊ����0x0d0x0a��OK0x0d0x0a,�ָ�Ϊȥ������OK0x0d0x0a,Ϊ���߼����RxByte+4
					{
						Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
						if(Addr != 0)
						{
							p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						}
						GPRS_QCSG_NextATCount();
						return;
					}
					i = MRR((u32)&pUartCtrl->RxTxByte, 4);
					i += RxByte;
					MWR(i, (u32)&pUartCtrl->RxTxByte, 4);//ͨ������			
					x = p16fifo[1];
					while(x >= (LEN_UART1IntATRx-10))
					{
						x -= (LEN_UART1IntATRx-10);
					}
					#if LOG_1376_3==1//0=û,1=��
						if(RxByte!=0)
						{
							y=p16fifo[0];//����p0
							p16fifo[0]=x;//p0��p1ֵ,�Ƚ�ASCII��
							p16fifo[1]=p1;
							LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
							p16fifo[0]=y;//�ָ�p0
							p16fifo[1]=x;//�ָ�p1
						}
					#endif
					p8s = (u8*)(ADDR_UART1IntATRx+10);
					p16 = (u16*)(ADDR_UART1IntRx);
					y = p16[0];
					if(y >= (LEN_UART1IntRx-10)) y = 0;

					p8d = (u8*)(ADDR_UART1IntRx+10);	
					for(i = 0; i < RxByte; i++)
					{
						p8d[y]=p8s[x];
						x++;
						if(x >= (LEN_UART1IntATRx-10)) x = 0;
						y++;
						if(y >= (LEN_UART1IntRx-10)) y = 0;
					}
					p16fifo[1] = x;			//�޸��Ѷ�ָ��
					p16[0] = y;				//����ָ��
					#if LOG_1376_3==1//0=û,1=��
						if(RxByte != 0)
						{
							LOG_LINK(13763, 0, GPRSPORT, (u8 *)ADDR_UART1IntRx+10, RxByte, 0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
							p16fifo[2] = x;//�޸��Ѷ�ָ��
							LOG_ATRx(1);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
						}
					#endif
					Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr != 0)
					{
						p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}

					if(RxByte == 0)
					{	
						pUartCtrl->Round++;
						if(pUartCtrl->Round > 60)
						{
							pUartCtrl->Round = 0;
							GPRS_QCSG_NextATCount();
						}
					}
					else
					{
						UartReveiveFrame(GPRSPORT);
						p16[0] = 0;
						p16[1] = 0;
						p16[2] = 0;
						p16[3] = 0;
						p16[4] = 0;
					}
					return;
				}
			}
			else
			{
				Addr = ATRxSearch((u32)((u8*)"ERROR: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10)-3;//�޸��Ѷ�ָ��,3�ֽڴ�����룬err:913
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
					pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
					return;
				}
			}
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			GPRS_QCSG_NextATCount();
			break;
		case AT_MYNETWRITE://��������
			if(pUartCtrl->ATWriteTask == 0)//AT���ݷ�������:0=������,1=������,2=�ȴ����������ʱ
			{
				Addr = ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
				
				#if LOG_1376_3==1//0=û,1=��
					LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
				#endif	
				y = p16fifo[1];//y=ԭp1
				Addr = ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					pUartCtrl->ATTask = 1;						// AT����:0=����,1=����,2=���ڷ���,3=����
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);	// �޸��Ѷ�ָ��
					MC(0,ADDR_DATABUFF,64);
							
					p0 = p16fifo[0];//p0
					p1 = p16fifo[1];//p1
					p8s = (u8*)(ADDR_UART1IntATRx+10);
					p8d = (u8*)(ADDR_DATABUFF);
					if(p0 >= p1)
					{
						RxByte = p0 - p1;
					}
					else
					{
						RxByte = (p0+(LEN_UART1IntATRx-10)) - p1;
					}
					
					i = 0;
					while(RxByte != 0)	//ȡ�����ַ���
					{
						p8d[i] = p8s[p1];
						i++;
						if(i >= 8) i = 0;	//�ַ���̫����
						p1++;
						if(p1 >= (LEN_UART1IntATRx-10)) p1 = 0;
						RxByte--;
					}
					p16fifo[1] = p1;					
					i = ASCII_HEX(ADDR_DATABUFF);				//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
					if(i >= pTerminalRam->CopyTxByte)			// ���ο��Է����ֽ���
					{
						pTerminalRam->ReqTxByte = i;			// �������ֽ���
						pUartCtrl->LinkSubTask = 0;				// ����AT���ͼ���
						pUartCtrl->ATWriteTask = 1;				// AT���ݷ�������:0=������,1=������
						pTerminalRam->GPRSWaitTx_S_Timer = 60;	// 1��,GPRS�������ݵȴ���ʱ�붨ʱ��
					}
					else
					{
						WarningOccur(GPRSPORT,(u8 *)"Զ�����߷��ͻ��岻��");//�澯����,��ڸ澯����͸澯�ַ���
						pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
					}
				}
				else
				{// �������ݵ�һ������ʧ�ܣ��� 1S ������
					pTerminalRam->AT_MS_Timer = 100;
					pUartCtrl->ATTask = 1;
				}
				if(pTerminalRam->GPRSWaitTx_S_Timer != 0) return;		// 2��,GPRS�������ݵȴ���ʱ�붨ʱ��
				if(pUartCtrl->LinkTask == 101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					WarningOccur(GPRSPORT,(u8 *)"Զ�����ߵȴ����ͳ�ʱ");//�澯����,��ڸ澯����͸澯�ַ���
				}
				else
				{
					WarningOccur(GPRSPORT,(u8 *)"Զ�����ߵ�¼��վʧ��");//�澯����,��ڸ澯����͸澯�ַ���
				}
				pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
			}
			else if(pUartCtrl->ATWriteTask == 1)
			{
				Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
				
				#if LOG_1376_3==1//0=û,1=��
					LOG_ATRx(1);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
				#endif
				Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr != 0)
				{
					p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				pUartCtrl->ATWriteTask = 2;
				pTerminalRam->AT_MS_Timer = 50 + ((pTerminalRam->ReqTxByte / 10) << 1);	// �ȴ�������ʱ��0.5 + �����ֽ�����Ӧ��ʱ��
			}
			else
			{
				if(pTerminalRam->AT_MS_Timer != 0) return;
				pUartCtrl->Task = UART_TASK_TX_DONE;
				pUartCtrl->ATTask = 1;			// AT����:0=����,1=����,2=���ڷ���,3=����
				pUartCtrl->LinkSubTask = 0;		// ����AT���ͼ���
				pUartCtrl->ATWriteTask = 0;		// AT���ݷ�������:0=������,1=������,2=�ȴ����������ʱ
				GPRS_QCSG_NextATCount();		// �ȴ����ݷ����Ѿ���ʱ����һ�� AT ָ�������ٴ���ʱ
				pTerminalRam->CopyTxByte = 0;
			}
			break;
		case AT_MYFTP://FTP�ļ�����
			#if Extern_GPRS_FTP_RxTx==0//�ⲿ����:0=I2,1=E1,2=E2,3=;W5200 GPRS�ļ�����
				GPRS_FTP_Rx();//GPRS_FTP_Rx����
			#else
				if((pCommRam->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
				{
	    			p32=(u32 *)(ADDR_E1_ENTRY);
	    			(*(void(*)())p32[107])();//GPRS_FTP_Rx����
				}
			#endif	
			break;
		case AT_SMS_NOLINK://���ŷ���û����ǰ
		case AT_SMS_LINK://���ŷ���
			#if LEN_UART11Ctrl!=0
				GPRS_SMS_Rx();//GPRS_SMS_Rx����
			#endif
			break;	
		case AT_HOLD://HOLD��ͣ
			pUartCtrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			break;
		case AT_MYPOWEROFF://�ػ�
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				pUartCtrl->LinkTask = 0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				pCommRam->UART1Timer = 15000/10;//�ػ���ʱ15s
			}
			else
			{
				pUartCtrl->LinkSubTask++;			//����AT���ͼ���
				if(pUartCtrl->LinkSubTask <= 2)		//����AT���ͼ���
				{
					pUartCtrl->ATTask = 1;//AT����:0=����,1=����,2=���ڷ���,3=����
					pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
				}
				else
				{
					//����Ҫ��û���յ�ȷ��֡������ON/OFF����2s������ʱ15s�ȴ��ػ�
					Pin_Configure(PIN_ON_GPRS,//���
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
					pTerminalRam->GPRS_ONOFF_MS_Timer = 201;
					pUartCtrl->LinkTask = 0;
					pCommRam->UART1Timer = 15000/10;	//�ػ���ʱ15s
					MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
					pTerminalRam->GPRSReConnection_S_Timer = bcd_hex(CommPara.RedialTimeGap);
					pTerminalRam->GPRSReConnection_S_Timer *= 60;
				}
			}

			///////////////////////////ybwhz����///////////////////////////////
		if(YBWHZ_TEST)
		{
			u8 *p8;
			u8 count = 0;
			MC(0, ADDR_YBWHZ_DATABUFF, 64);	
			p8=(u8*)(ADDR_YBWHZ_DATABUFF);
			p8[count++] = 'G';
			p8[count++] = 'P';
			p8[count++] = 'R';
			p8[count++] = 'S';
			p8[count++] = ' ';
			p8[count++] = 'O';
			p8[count++] = 'F';
			p8[count++] = 'F';
			
			p8[count++] = '#';

			QCSG_LogToRS232Port((u32)1,ADDR_YBWHZ_DATABUFF,count);
		}
		////////////////////////////ybwhz����/////////////////////////////
			
			break;

		case AT_MYRESET://err20190410 GPRSģ������ָ��
			Addr = ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				pTerminalRam->Signal = 0;			//GPRS�ź�ǿ��0-4
				pTerminalRam->SignaldBm = 0x99;		//�ź�ǿ��0-31��99	
				pUartCtrl->LinkCount = 0;
				pUartCtrl->LinkTask = 2;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				pCommRam->UART1Timer = 15000/10;//�ػ���ʱ15s
			}
			else
			{
				pUartCtrl->LinkSubTask++;			//����AT���ͼ���
				if(pUartCtrl->LinkSubTask > 2)		//����AT���ͼ���
				{
					//pUartCtrl->ATTask = 1;//AT����:0=����,1=����,2=���ڷ���,3=����
					pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
				}
			}
			//TxByte=CopyString((u8 *)"AT$CFUN=1,1\r",p8txbuff);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			break;
		default:
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if((Addr == 0) && (pTerminalRam->AT_MS_Timer != 0)) return; //����û���
			
			#if LOG_1376_3==1//0=û,1=��
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			pTerminalRam->AT_MS_Timer = 50;			// ��һ��ATָ��ͼ��ʱ�䣬 0.5S
			pUartCtrl->ATTask = 0;					// AT����:0=����,1=����,2=���ڷ���,3=����
			Addr = ATRxSearch((u32)((u8*)"OK"));	// AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr != 0)
			{
				p16fifo[1] = Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				GPRS_QCSG_NextATCount();//��1��AT
			}
			else
			{
			ReTxAT://�ط���AT
				pUartCtrl->LinkSubTask++;//����AT���ͼ���
				if(pUartCtrl->LinkSubTask > 2)//����AT���ͼ���
				{
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
					pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
				}
				return;
			}
			break;
	}
}


void GPRS_QCSG_AT_RxTx(void)//GPRS_QCSGģ��AT���շ���
{
	u32 *p32;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;
	switch(pUartCtrl->ATTask)//AT����:0=����,1=����,2=���ڷ���,3=����
	{
		case 1://����
			GPRS_QCSG_AT_Tx();//AT�����
			break;
		case 2://���ڷ���
			if(Wait_UARTn_Tx(GPRSPORT)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				p32=(u32*)(&GPRS_QCSG_AT_Command_Lib);
				switch(p32[4*pUartCtrl->ATCount])//AT����
				{
					case AT_SMS_NOLINK://���ŷ���û����ǰ
					case AT_SMS_LINK://���ŷ�������
						if(pUartCtrl->SubTask == 1)//������
						{//��SMS������
							pTerminalRam->AT_MS_Timer = 30000/10;//GPRS AT������10MS��ʱ��
						}
						else
						{
							pTerminalRam->AT_MS_Timer = p32[(4*pUartCtrl->ATCount)+3]/10;//GPRS AT������10MS��ʱ��
						}
						break;
					default:
						pTerminalRam->AT_MS_Timer = p32[(4*pUartCtrl->ATCount)+3]/10;//GPRS AT������10MS��ʱ��
						break;
				}
				pUartCtrl->ATTask = 3;//AT����:0=����,1=����,2=���ڷ���,3=����
				break;
			}
			break;
		case 3://����
			GPRS_QCSG_AT_Rx();//AT�������
			break;
		default:
			pUartCtrl->ATTask = 0;//AT����:0=����,1=����,2=���ڷ���,3=����
			break;
	}
}


void UnInit_GPRS_QCSG(void)//GPRSģ������Ϊ��������
{
	Pin_Configure(PIN_GPRS_PCTRL,//���
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Pin_Configure(PIN_RST_GPRS,//���
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
	Pin_Configure(PIN_ON_GPRS,//���
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		 (1<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
}

void GPRS_QCSG_RxTx(u32 PORTn)//Զ������GPRSģ����շ���
{
	u32 i;
	CommunicationParaStruct* CommPara;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	UARTCtrl_TypeDef* pUartCtrl = UART1Ctrl;
	UartTaskCtrlStruct* UartTask = (UartTaskCtrlStruct*)ADDR_GPRS_UART_Task_Ctrl;

#if ((Project/100)==2)||((Project/100)==3 || ((Project/100)==9))	
	i =  Pin_Read(PIN_GPRS_ID4);
#endif	

	i |= Pin_Read(PIN_GPRS_ID3) << 1;
	i |= Pin_Read(PIN_GPRS_ID2) << 2;
	i |= Pin_Read(PIN_GPRS_ID1) << 3;
	
#if ((Project/100)==2)||((Project/100)==3)	|| ((Project/100)==9)
	i |= Pin_Read(PIN_GPRS_ID0) << 4;
#endif

#if ((Project/100)==2)||((Project/100)==3)	|| ((Project/100)==9)
	if(i == 0x1F)
#endif

#if ((Project/100)==5)	
	if(i == 0x0E)
#endif
	{
		pTerminalRam->MODULSEL = i;
		pTerminalRam->Signal = 0;
		pUartCtrl->LinkTask = 0;
		WarningOccur(GPRSPORT,(u8 *)"Զ��ģ��û����");
		pUartCtrl->State = GPRS_STATE_UNPLUGGED;
		return;
	}
	if(pTerminalRam->MODULSEL != i)
	{
		WarningCancel(GPRSPORT);
		pTerminalRam->MODULSEL = i;
		pUartCtrl->LinkTask = 0;
	}

	if(pCommRam->POWER < 3)					// ��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		if(pUartCtrl->LinkTask > 2)			// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
		{//����Դ��
			if(Pin_Read(PIN_ENR5V0) == 0)		// ������,�������ʹ����;����0��1
			{//�ж��й��˵�Դ
				pUartCtrl->PORT = 0;			// �����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				pUartCtrl->LinkTask = 1;		// 1=�ص�Դ
				return;
			}
		}
	}
	switch(pUartCtrl->LinkTask)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		case 0:	// 0=�ػ���ʱ
			MR((u32)CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			if(pTerminalRam->GPRSReConnection_S_Timer > ((bcd_hex(CommPara->RedialTimeGap))*60))//err20190104 RAM���� ��ֹRAM�������������ʱ�����
				pTerminalRam->GPRSReConnection_S_Timer = (bcd_hex(CommPara->RedialTimeGap)) * 60;
			if(pTerminalRam->GPRSReConnection_S_Timer == 0)
			{
				
				pTerminalRam->Signal = 0;			//GPRS�ź�ǿ��0-4
				pTerminalRam->SignaldBm = 0x99;		//�ź�ǿ��0-31��99	
				if(pCommRam->UART1Timer == 0x0) 
				{
					pUartCtrl->LinkTask++;
				}
				pUartCtrl->LinkCount = 0;
				pTerminalRam->LinkCount_SelfAdaption = 0;//�ػ� ����Ӧ���Ӵ�������
				pTerminalRam->SelfAdaptionChangeFlag = 0;//�ػ� ����Ӧ�л���ʶ����
			}
			pUartCtrl->State = GPRS_STATE_POWER_OFF;
			return;
		case 1:	// 1=���ܵ�Դ
			OffR5V0();				// ��R5V0��Դ		
			if((pUartCtrl->LinkCtrl != 0) && (pUartCtrl->LinkCount < MASTER_ADDR_NUM))
			{
				UnInit_GPRS_QCSG();	
				pUartCtrl->BpsCtrl = 0x3+(10<<5);	//��ʼ����115200
				Init_UARTn(GPRSPORT);				//��ʼ��UART��,���ͨ�ſ�����������
				pCommRam->UART1Timer = 10000/10;
				pUartCtrl->LinkTask++;
			}	
			pUartCtrl->State = GPRS_STATE_POWER_OFF;
			return;
		case 2://���ܵ�Դ
			if(pCommRam->POWER < 3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
			{	
				if(pUartCtrl->PORT == 0)
				{//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
					pUartCtrl->PORT = 1; 
				}
				else
				{
					return;//��ع���ʱ����1�κ�ֹͣ
				}
			}
			//GPRS_Voltage();//GPRS��Դ��ѹADC����
			//if(pCommRam->UGPRS < 0x0030)//BCD 2λС��
			//{//<0.30V
			//	if(pCommRam->UART1Timer <= 10000) pCommRam->UART1Timer = 0x0;	//��С����ʱ10�� + (20��Ĺص�Դ��ʱ)
			//}
			if((pCommRam->UART1Timer == 0x0) && 
			   (pUartCtrl->LinkCtrl != 0) && 
			   (pUartCtrl->LinkCount < MASTER_ADDR_NUM))
			{										// �ػ������� 10 S����������ʱ�����������������ӣ�������ѭ���ڴ�			
				Pin_OutH(PIN_RST_GPRS);
				OnR5V0();							// ��R5V0��Դ
				Pin_OutH(PIN_GPRS_PCTRL);
				pCommRam->UART1Timer = 201/10;		// ���ܵ�Դ��ʱ0.2��
				pUartCtrl->LinkTask++;	
				pUartCtrl->State = GPRS_STATE_POWER_ON;
			}
			return;
		case 3:// ��ģ��ON/OFF���=0
			if(pCommRam->UART1Timer != 0x0) return;
			if(pCommRam->POWER == 3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
  			{
				GPRS_Voltage();//GPRS��Դ��ѹADC����
				//if((pCommRam->UGPRS < 0x0350) || (pCommRam->UGPRS > 0x0450))	// BCD 2λС��
				//{																// 3.80V>��ѹ>4.20V(�͵��Ժʱ������,��3.50V��4.50V)
				//	pCommRam->ErrorIC |= 1 << 30;								// B30=GPRSģ���Դ
				//	pCommRam->DisplayAllErrorTimer = 10;						// �ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
				//}
			}
			MC(0,ADDR_UART1IntATRx,LEN_UART1IntATRx);
			pUartCtrl->Task=0;//0=���ڽ���
			pUartCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			pUartCtrl->RxByte=0;//�����ֽ���=0
			pUartCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
			pUartCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0		
			pUartCtrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			pUartCtrl->ATCount=0;//AT����
			pUartCtrl->LinkSubTask=0;//����AT���ͼ���
			pUartCtrl->SubTask=0;//������
			pUartCtrl->LinkTask2=0;//ͨ��2��������(FTP����)
			pUartCtrl->LinkCount = 0;
			pTerminalRam->LinkCount_SelfAdaption = 0;//����Ӧ���Ӵ��� ����
			pTerminalRam->SelfAdaptionChangeFlag = 0;//����Ӧ�л���ʶ ����
			pUartCtrl->NETOPENCount=0;//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
			pUartCtrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
			pTerminalRam->GPRSHeartBeat_S_Timer=0;//���������붨ʱ��
			Pin_Configure(PIN_ON_GPRS,//ON���=0
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			pTerminalRam->GPRS_ONOFF_MS_Timer = 102;
			Pin_Configure(PIN_RST_GPRS,//RESET����,����
							0|//B3-B0:���ù���ѡ��AF(0-15)
				 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
				 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
				 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
				 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
				 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
			pCommRam->UART1Timer = 2000/10;	// 2S��ʱ�ȴ��ж������
			pUartCtrl->LinkTask++;			// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			return;//����UART1Timerֱ�ӷ���
		case 4://PIN_ON_RST���1S��10%��ʱ
			if(pCommRam->UART1Timer == 0x0)
			{
				Pin_OutH(PIN_ON_GPRS);//ON���=1
				pCommRam->UART1Timer = 10000/10;//��ʱ��λ
				pUartCtrl->LinkTask = 7;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;
		case 5://��λ
			if(pCommRam->UART1Timer == 0x0)
			{
				Pin_Configure(PIN_RST_GPRS,//RESET
								0|//B3-B0:���ù���ѡ��AF(0-15)
					 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				
				pCommRam->UART1Timer = 300/10;//��λ��ʱ0.3��(>=200ms)
				pUartCtrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;
		case 6://��λ���
			if(pCommRam->UART1Timer == 0x0)
			{
				Pin_Configure(PIN_RST_GPRS,//RESET����,����
									0|//B3-B0:���ù���ѡ��AF(0-15)
						 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
						 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
						 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
						 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
						 (0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				pCommRam->UART1Timer = 10000/10;//��AT����ǰ��ʱ;GPRS�ϵ��10������ɳ�ʼ�����ܽ���AT����
				pUartCtrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 7://��ʼ��UART1
			if(pCommRam->UART1Timer == 0x0)
			{
				pUartCtrl->BpsCtrl = 0x3+(10<<5);	//��ʼ����115200
				Init_UARTn(GPRSPORT);				//��ʼ��UART��,���ͨ�ſ�����������
				pUartCtrl->LinkTask++;				//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				pUartCtrl->State = GPRS_STATE_MATCH_BAUD_RATE;
			}
			return;//����UART1Timerֱ�ӷ���
		case 8://��ʼ��AT����
			break;
		case 100://����ok
			pUartCtrl->State = GPRS_STATE_LINK_SUCCESS;
			break;
		case 101://��¼ok
			pUartCtrl->State = GPRS_STATE_LOG_SUCCESS;	

			break;
		case 102://���˳���¼
			break;
		case 103://�˳���¼
		case 104://�ػ�
			//if(pUartCtrl->ATTask == 0)//AT����:0=����,1=����,2=���ڷ���,3=����
			//{
			//	pUartCtrl->ATCount = AT_MYPOWEROFF;//�ػ�
			//	pUartCtrl->ATTask = 1;//AT����:0=����,1=����,2=���ڷ���,3=����
			//}
			pUartCtrl->LinkTask = 1;
			break;
		default:
			pUartCtrl->LinkTask = 0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			return;
	}
	
	if(pUartCtrl->ATTask != 0)//AT����:0=����,1=����,2=���ڷ���,3=����
	{
		GPRS_QCSG_AT_RxTx();
	}
	
	switch(pUartCtrl->Task)
	{
		case UART_TASK_RX:
			if((pUartCtrl->ATTask == 0) || (pUartCtrl->ATTask > 3)) pUartCtrl->ATTask = 1;	// AT����:0=����,1=����,2=���ڷ���,3=����
			break;
		case UART_TASK_RX_DONE:
			UartTask->Flag |= UART_FLAG_RX_DONE;
			UartTask->FrameLen = pUartCtrl->RxByte;
			UartTask->FrameType = pUartCtrl->FrameType;
			UartTask->FrameBuf = (u8*)Get_ADDR_UARTnRx(PORTn);
			pUartCtrl->Task = UART_TASK_RX;
			pUartCtrl->ATTask = 1;	// AT����:0=����,1=����,2=���ڷ���,3=����
		
			#if LOG_1376_1==1		// 0=û,1=��
			LOG(13761, 0, GPRSPORT, UartTask->FrameBuf, UartTask->FrameLen, 0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			break;
		case UART_TASK_TX:					
			if(pUartCtrl->ATTask == 0)						// AT����:0=����,1=����,2=���ڷ���,3=����
			{
				#if LOG_1376_1==1		// 0=û,1=��
				LOG(13761, 1, GPRSPORT, (u8 *)Get_ADDR_UARTnTx(GPRSPORT), pTerminalRam->CopyTxByte, 0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				#endif
				//pTerminalRam->CopyTxByte = pUartCtrl->TxByte;	// �����ֽ����ڴ����������������룬pUartCtrl->TxByte�ֶλᱻ����������£�����ʹ��
				pUartCtrl->TxCount = 0;							// �ѷ��ͼ���
				pTerminalRam->CopyTxCount = 0;					// �ѷ��ͼ���
				pTerminalRam->GPRSWaitTx_S_Timer = 2*60;		// 2��,GPRS�������ݵȴ���ʱ�붨ʱ��
				pUartCtrl->ATCount = AT_MYNETWRITE;				// ��������
				pUartCtrl->ATWriteTask = 0;						// AT���ݷ�������:0=������,1=������
				pUartCtrl->Task = UART_TASK_TXING;
				pTerminalRam->AT_MS_Timer = 0;
				pUartCtrl->ATTask = 1;
			}
			break;
		case UART_TASK_TXING:		//�������ڷ���
			if(pUartCtrl->ATTask == 0)//AT����:0=����,1=����,2=���ڷ���,3=����
			{//����ԭ���ͻ������RAM����ȵ�
				WarningOccur(GPRSPORT,(u8 *)"GPRS������BUG");//�澯����,��ڸ澯����͸澯�ַ���
				pUartCtrl->LinkTask = 0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				//�ز������ʱ����Ϊ0�Ա���������
				pTerminalRam->GPRSReConnection_S_Timer = 0;//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
			}
			break;
		case UART_TASK_TX_DONE:
			pUartCtrl->Task = UART_TASK_RX;
			pUartCtrl->ATTask = 1;							// AT����:0=����,1=����,2=���ڷ���,3=����
			pUartCtrl->TxCount = 0;							// �ѷ��ͼ���
			pTerminalRam->CopyTxByte = 0;					// �����ֽ���
			pTerminalRam->CopyTxCount = 0;					// �ѷ��ͼ���
			pTerminalRam->GPRSWaitTx_S_Timer = 0;			// 2��,GPRS�������ݵȴ���ʱ�붨ʱ��
			CommPara = (CommunicationParaStruct*)ADDR_DATABUFF;
			MR((u32)CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			pTerminalRam->GPRSHeartBeat_S_Timer = bcd_hex(CommPara->HeartBeatTimeGap);
			pTerminalRam->GPRSHeartBeat_S_Timer *= 60;
			if(pTerminalRam->GPRSHeartBeat_S_Timer == 0) pTerminalRam->GPRSHeartBeat_S_Timer = 15*60;
			UartTask->Flag |= UART_FLAG_TX_DONE;
			break;
		default:
			pUartCtrl->Task = UART_TASK_RX;
			pUartCtrl->ATTask = 1;
			break;
	}

}



void GPRSTest(void)
{

}


/*
void GPRS_URC_Rx(void)//�����ϱ�����
{
	u8 *p8;
	u32 *p32;
	u32 Addr;
	u32 ClrByte;
	
	//���ݵ��������ϱ� $MYURCREAD
	Addr=ATRxSearch((u32)((u8*)"$MYURCREAD"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
	if(Addr!=0)
	{
		ClrByte=10;//����0�ֽ���
	ClrURC:
		Addr-=(ADDR_UART1IntATRx+6);
		p8=(u8*)(ADDR_UART1IntATRx+6);
		while(ClrByte!=0)
		{
			if(Addr==0)
			{
				Addr=(LEN_UART1IntATRx-6);
			}
			Addr--;
			p8[Addr]=0;
			ClrByte--;
		}
		p32=(u32*)(&GPRS_3763_AT_Command_Lib);
		Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//AT���յ�һ���ֽڳ�ʱ��ʱֵ(��)
		return;
	}
	//��·�Ͽ������ϱ�
	Addr=ATRxSearch((u32)((u8*)"$MYURCCLOSE: 0"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
	if(Addr!=0)
	{
		WarningOccur(GPRSPORT,(u8 *)"GPRS��·����վ�Ͽ�");//�澯����,��ڸ澯����͸澯�ַ���
		UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
		UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
		ClrByte=10;//����0�ֽ���
		goto ClrURC;
	}
	//��������״̬�����ϱ�
	Addr=ATRxSearch((u32)((u8*)"$MYURCACT: 0,0"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
	if(Addr!=0)
	{
		WarningOccur(GPRSPORT,(u8 *)"GPRS���������Ͽ�");//�澯����,��ڸ澯����͸澯�ַ���
		UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
		UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
		ClrByte=10;//����0�ֽ���
		goto ClrURC;
	}
	//�ͻ������������ϱ�
	
	
	//FTP���ӶϿ������ϱ�
}
*/





