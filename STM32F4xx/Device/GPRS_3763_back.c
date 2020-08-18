
//����GPRSģ�� 1376.3��׼
#if MainProtocol==376
#include "../Head/Project.h"
#include "../Terminal/Terminal_Uart_3761.h"
#include "../terminal/Terminal_USER11xx.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_DataDef.h"
#endif

#include "../Device/GPRS_3763.h"
#include "../Device/ATNo.h"
#include "../Terminal/ATString.h"
#include "../Device/GPRS_FTP_RxTx.h"
#include "../Device/GPRS_SMS_RxTx.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../STM32F4xx/STM32F4xx_ADC.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../TEST/LOG.h"



#define NUM_ReATTx     1//AT�ط�����(ʵ�ʷ��ʹ���=�ط�����+1)


//SocketID 0-5ʹ��Լ��
//0 ������
//1 FTP����
//2 FTP����
//3-5 δ��


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

//������ƶ�̨��ʶ����IMSI
ADDR_ATStr_CIMI
	dcb "AT+CIMI\r",0
ADDR_ERRStr_CIMI
	dcb "������ƶ���ʶʧ��",0

/*
//��ȡSIM��������Ӫ������
ADDR_ATStr_CSPN
	dcb "AT+CSPN?\r",0
ADDR_ERRStr_CSPN
	dcb "������Ӫ������ʧ��",0
*/

	//��������
//���ź�û����ǰ
ADDR_ATStr_CSQ_NOLINK
	dcb "AT+CSQ\r",0
ADDR_ERRStr_CSQ_NOLINK
	dcb "Զ�����߶��ź�ʧ��",0

//��ȡ��ǰ����ע��״̬
ADDR_ATStr_CREG
#if (USER/100)==13//������
	dcb "AT+CGREG?\r",0
#else
	dcb "AT+CREG?\r",0
#endif
ADDR_ERRStr_CREG
	dcb "��ȡ����ע��״̬ʧ��",0

//��ȡSIM��������Ӫ������
#if (USER/100)!=0//���ǵ��Ժ����(1376.3��׼�޴�����)
ADDR_ATStr_COPS2
	dcb "AT+COPS=0,2\r",0
ADDR_ERRStr_COPS2
	dcb "AT+COPS=2ʧ��",0
ADDR_ATStr_COPS
	dcb "AT+COPS?\r",0
ADDR_ERRStr_COPS
	dcb "������Ӫ������ʧ��",0
#endif

//ȡ��������
ADDR_ATStr_CNUM
	dcb "AT+CNUM\r",0
ADDR_ERRStr_CNUM
	dcb "����������ʧ��",0
	
//��ѯע���������ͣ�3G/4G
ADDR_ATStr_SYSINFO
	dcb "AT$MYSYSINFO\r",0
ADDR_ERRStr_SYSINFO
	dcb "��ѯ�Ƿ�ע����4G����",0


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
	dcb "Զ�����߿�������ʧ��",0
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
	
	ALIGN 4	
};





__asm void GPRS_3763_AT_Command_Lib(void)//AT˳��ִ�п�
{
//�����ʼ��
	dcd AT_AT
	dcd ADDR_ATStr_AT//AT�����ַ�����ַ
	dcd ADDR_ERRStr_AT//������ʾ�ַ�����ַ
	dcd 4000//AT����ص�1���ַ���ʱ(ms)

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

//������ƶ�̨��ʶ����IMSI
	dcd AT_CIMI
	dcd ADDR_ATStr_CIMI//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CIMI//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//��ȡSIM��������Ӫ������
/*
	dcd AT_CSPN
	dcd ADDR_ATStr_CSPN//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CSPN//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
*/

//���ź�û����ǰ
	dcd AT_CSQ_NOLINK
	dcd ADDR_ATStr_CSQ_NOLINK//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CSQ_NOLINK//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//��ȡ��ǰ����ע��״̬
	dcd AT_CREG
	dcd ADDR_ATStr_CREG//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CREG//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
	
#if (USER/100)!=0//���ǵ��Ժ����(1376.3��׼�޴�����)
	dcd AT_COPS2
	dcd ADDR_ATStr_COPS2//AT�����ַ�����ַ
	dcd ADDR_ERRStr_COPS2//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

	dcd AT_COPS
	dcd ADDR_ATStr_COPS//AT�����ַ�����ַ
	dcd ADDR_ERRStr_COPS//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
#endif

//ȡ��������
	dcd AT_CNUM
	dcd ADDR_ATStr_CNUM//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CNUM//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
	
//��ѯע���������ͣ�2G/3G/4G
	dcd AT_SYSINFO
	dcd ADDR_ATStr_SYSINFO//AT�����ַ�����ַ
	dcd ADDR_ERRStr_SYSINFO//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//����APN(�Ͻ���AT_SYSINFO���ֿ�)
	dcd AT_MYNETCON_APN
	dcd ADDR_ATStr_MYNETCON_APN//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETCON_APN//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//����USERPWD:�û���������
	dcd AT_MYNETCON_USERPWD
	dcd ADDR_ATStr_MYNETCON_USERPWD//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETCON_USERPWD//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

//������������
	dcd AT_MYNETACT
	dcd ADDR_ATStr_MYNETACT//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETACT//������ʾ�ַ�����ַ
	dcd 90000//AT����ص�1���ַ���ʱ(ms)

//IP���ʿ�������
	dcd AT_MYIPFILTER
	dcd ADDR_ATStr_MYIPFILTER//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYIPFILTER//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)

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
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
//��ѯע���������ͣ�2G/3G/4G
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
	dcd 40000//AT����ص�1���ַ���ʱ(ms)
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

};



void GPRS_3763_NextATCount(void)//��1��AT
{
	UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
	UART1Ctrl->SubTask=0;//������
	UART1Ctrl->ATCount++;
	//�¸�AT���ʱ����
	switch(UART1Ctrl->ATCount)//�¸�AT����
	{
		case AT_CSQ_NOLINK://���ź�û����ǰ
			Terminal_Ram->GPRSWaitTx_S_Timer=60;//GPRS�ȴ��ź�ǿ�Ȳ�ѯ(����ǰ)��ʱ
			break;
		case AT_CREG://��ȡ��ǰ����ע��״̬
			Terminal_Ram->GPRSWaitTx_S_Timer=60;//�ȴ�ע�ᶨʱ
			break;
		case AT_SYSINFO_LINK:
		case AT_SYSINFO://��ѯע���������ͣ�2G/3G/4G
			Terminal_Ram->GPRSWaitTx_S_Timer=10;//�ȴ�������ȷע���������ͣ�2G/3G/4G
			break;
		case AT_MYNETACT://������������
			Terminal_Ram->GPRSWaitTx_S_Timer=60;//�ȴ������������Ӷ�ʱ
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


void GPRS_3763_AT_Tx(void)//AT�����
{
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 AT_No;
	u32 TxByte;
	u8 *p8;
	u16 *p16;
	u32 *p32;
	u8 *p8rx;
	u8 *p8tx;
	
	u32 FTPFileCount;//FTP�ļ����ظ�������
	
	n=n;
	p8tx=(u8*)(ADDR_ATTx);
	TxByte=0;
	AT_No=UART1Ctrl->ATCount;//AT����
	if(AT_No>AT_Max)
	{
		AT_No=0;
		UART1Ctrl->ATCount=0;//AT����
	}
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	switch(p32[4*AT_No])//AT����
	{
		case AT_AT://�����ʼ��
			TxByte=CopyString((u8 *)"AT\r",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			switch(UART1Ctrl->SubTask)//������
			{
				case 0://����57600
					UART1Ctrl->BpsCtrl=0x3+(9<<5);//����57600
					break;
				case 1://����115200
					UART1Ctrl->BpsCtrl=0x3+(10<<5);//����115200
					break;
				case 2://����38400
					UART1Ctrl->BpsCtrl=0x3+(8<<5);
					break;
				case 3://����19200
					UART1Ctrl->BpsCtrl=0x3+(7<<5);
					break;
				case 4://����9600
					UART1Ctrl->BpsCtrl=0x3+(6<<5);
					break;
				case 5://����4800
					UART1Ctrl->BpsCtrl=0x3+(5<<5);
					break;
				case 6://����2400
					UART1Ctrl->BpsCtrl=0x3+(4<<5);
					break;
				case 7://����1200
					UART1Ctrl->BpsCtrl=0x3+(3<<5);
					break;
					
//				case 10://��1��AT
//					GPRS_3763_NextATCount();//��������,57.6k��ͨ��
//					return;
				default:
					TxByte=CopyString((u8 *)"AT+IPR=57600\r",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					break;
			}
			//����UART1�ڷ���
			Init_UARTn(GPRSPORT);//��ʼ��UART��,���ͨ�ſ�����������
			UART1Ctrl->TxByte=TxByte;//�����ֽ���(���ֶ���)
			UART1Ctrl->TxCount=0;
		#if LOG_1376_3==1//0=û,1=��
			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
			Start_UARTn_Tx(GPRSPORT);//����UART�ڷ���
			UART1Ctrl->ATTask=2;//AT����:0=����,1=����,2=���ڷ���,3=����
			return;
		case AT_CNUM://ȡ��������
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			break;		
		case AT_MYNETCON_APN://����APN
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
	#if MainProtocol==376
			MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
			MC(0,ADDR_DATABUFF+16,1);
	#else
			//DL698
			i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
			if(i==0)
			{//����ͨ����=0
				p8=Get_Element((u8 *)ADDR_4500_2,6,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8)
				{//�ҵ�
					i=p8[1];//�ֽ���
					p8+=2;
					p8[i]=0;
					MR(ADDR_DATABUFF,(u32)p8,i+1);
				}
				else
				{//û�ҵ�
					CopyString((u8 *)"cmnet\x0",(u8 *)ADDR_DATABUFF);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				}
			}
			else
			{//�б���ͨ����
				MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
				p8=(u8*)ADDR_128KDATABUFF;
				x=p8[1];//ȡ����ͨ����
				i=Comm_Ram->CSPN;//0=��,1=�й��ƶ�ͨ��-----china mobile  CMCC,2=�й���ͨͨѶ-----china unicom  CUCC,3=�й�����  ------CHINA TELECOM  CTCC		
				switch(i)
				{
					default:
					case 1://1=�й��ƶ�ͨ��-----china mobile  CMCC
						n=0;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
						break;
					case 2://2=�й���ͨͨѶ-----china unicom  CUCC
						n=2;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
						break;
					case 3://3=�й�����  ------CHINA TELECOM  CTCC	
						n=1;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
						break;
				}
				p8+=2;
				for(i=0;i<x;i++)
				{
					if(p8[3]==n)//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
					{
						y=p8[5];//2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 �� 
						y+=2;
						if(y==Comm_Ram->RemoteWirelessModuleSYSINFO)//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
						{
							break;
						}
					}
					p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				}
				if(i<x)
				{//�ҵ�
					p8+=6;
					i=p8[1];//�ֽ���
					p8+=2;
					p8[i]=0;
					MR(ADDR_DATABUFF,(u32)p8,i+1);
				}
				else
				{
					CopyString((u8 *)"cmnet\x0",(u8 *)ADDR_DATABUFF);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				}
			}
	#endif
			TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
		case AT_MYNETCON_USERPWD://����USERPWD:�û���������
			//AT$MYNETCON=0,"USERPWD","card,card"
		#if (USER/100)==14//����
			#if MainProtocol==376
				MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
				p8=(u8*)(ADDR_DATABUFF);
				if(((p8[0]!='f')&&(p8[0]!='F'))
				|| ((p8[1]!='j')&&(p8[1]!='J'))
				|| ((p8[2]!='e')&&(p8[2]!='E'))
				|| ((p8[3]!='p')&&(p8[3]!='P'))
				|| (p8[4]!='.')
				|| ((p8[5]!='f')&&(p8[4]!='F'))
				|| ((p8[6]!='j')&&(p8[6]!='J')))
				{//���ض�APN���û��������ȡAFN04F16����ֵ
				}
				else
				{
					TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					MR(ADDR_DATABUFF,ADDR_AFN0CF1+4,12);//�豸���
					p8[12]=0;//�ַ�����β
					//�û���
					TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					TxByte+=CopyString((u8 *)"@fjep.vpdn.fj",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					p8tx[TxByte++]=',';
					//����
					for(i=0;i<12;i++)
					{//���򿽱�
						p8tx[TxByte+i]=p8[11-i];
					}
					TxByte+=12;
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]='\r';
					TxByte++;
					break;
				}
			#endif
			#if MainProtocol==698
				#warning "����������"
			#endif
		#endif
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
	#if MainProtocol==376
			if(MRR(ADDR_AFN04F16,1)!=0)
			{
				MR(ADDR_DATABUFF,ADDR_AFN04F16,32);
				MC(0,ADDR_DATABUFF+32,1);
				TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				p8tx[TxByte]=',';
				TxByte++;
				MR(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
				TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			else
			{//�û����������Ϊ��,�ֶ�������,��','�ź���','��
				if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
				{
					p8tx[TxByte]=',';
					TxByte++;
				}
			}
	#else
			//DL698
			i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
			if(i==0)
			{//����ͨ����=0
				p8=Get_Element((u8 *)ADDR_4500_2,7,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8)
				{//�ҵ�
					i=p8[1];//�ֽ���
					p8+=2;
					p8[i]=0;
					TxByte+=CopyString(p8,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					p8tx[TxByte]=',';
					TxByte++;
					p8=Get_Element((u8 *)ADDR_4500_2,8,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//�ҵ�
						i=p8[1];//�ֽ���
						p8+=2;
						p8[i]=0;
						TxByte+=CopyString(p8,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					}
				}
				else
				{//û�ҵ�;�û����������Ϊ��,�ֶ�������,��','�ź���','��
					if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
					{
						p8tx[TxByte]=',';
						TxByte++;
					}
				}
			}
			else
			{//�б���ͨ����
				MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
				p8=(u8*)ADDR_128KDATABUFF;
				x=p8[1];//ȡ����ͨ����
				i=Comm_Ram->CSPN;//0=��,1=�й��ƶ�ͨ��-----china mobile  CMCC,2=�й���ͨͨѶ-----china unicom  CUCC,3=�й�����  ------CHINA TELECOM  CTCC		
				switch(i)
				{
					default:
					case 1://1=�й��ƶ�ͨ��-----china mobile  CMCC
						n=0;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
						break;
					case 2://2=�й���ͨͨѶ-----china unicom  CUCC
						n=2;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
						break;
					case 3://3=�й�����  ------CHINA TELECOM  CTCC	
						n=1;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
						break;
				}
				p8+=2;
				for(i=0;i<x;i++)
				{
					if(p8[3]==n)//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
					{
						y=p8[5];//2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 �� 
						y+=2;
						if(y==Comm_Ram->RemoteWirelessModuleSYSINFO)//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
						{
							break;
						}
					}
					p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				}
				if(i<x)
				{//�ҵ�
					p8+=6;
					p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					//�û���
					if(p8[0]==DataType_visible_string)
					{
						i=p8[1];//�ֽ���
						p8+=2;
						MR(ADDR_DATABUFF,(u32)p8,i);
						MC(0,ADDR_DATABUFF+i,1);
						TxByte+=CopyString((u8*)ADDR_DATABUFF,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
						p8+=i;
						//����
						p8tx[TxByte]=',';
						TxByte++;
						if(p8[0]==DataType_visible_string)
						{
							i=p8[1];//�ֽ���
							p8+=2;
							MR(ADDR_DATABUFF,(u32)p8,i);
							MC(0,ADDR_DATABUFF+i,1);
							TxByte+=CopyString((u8*)ADDR_DATABUFF,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
						}
					}
					else
					{//û�ҵ�;�û����������Ϊ��,�ֶ�������,��','�ź���','��
						if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
						{
							p8tx[TxByte]=',';
							TxByte++;
						}
					}
				}
				else
				{//û�ҵ�;�û����������Ϊ��,�ֶ�������,��','�ź���','��
					if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
					{
						p8tx[TxByte]=',';
						TxByte++;
					}
				}
			}
	#endif
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
		case AT_MYIPFILTER://IP���ʿ�������
			if(UART1Ctrl->SubTask>=5)
			{
				GPRS_3763_NextATCount();//��1��AT
				return;
			}
	#if MainProtocol==376
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			x=MRR(ADDR_IPFILTER+(4*UART1Ctrl->SubTask),4);
			if(x==0)
			{
				UART1Ctrl->SubTask++;//���μ���
				return;
			}
			p8tx[TxByte]='"';
			TxByte++;
			for(i=0;i<4;i++)
			{
				y=hex_bcd(x&0xff);//HEX��תΪ8λѹ��BCD��
				x>>=8;
				TxByte+=BCD_ASCII(y,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
				p8tx[TxByte]='.';
				TxByte++;
			}
			TxByte--;//IP��4���ֽڵ�'.'ɾȥ
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)"255.255.255.255",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
	#else
			GPRS_3763_NextATCount();//��1��AT
			return;
	#endif
			
	#if (USER/100)==0//�û���ʶ,���Ժ���Բ�������AT
		case AT_CMFG://���ö���Ϣģʽ
		case AT_ZCVF://�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
		case AT_CTA://���ߵȴ�ʱ��(��CDMAʱ��Ҫ);�����ݴ���ʱ����10���CDMAͨ��ģ�������ͷſ��нӿ���Դ
			GPRS_3763_NextATCount();//��1��AT
			return;
	#else
		#if (USER/100)==14//����ϵ
		case AT_ZCVF://�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
//		case AT_CTA://���ߵȴ�ʱ��(��CDMAʱ��Ҫ);�����ݴ���ʱ����10���CDMAͨ��ģ�������ͷſ��нӿ���Դ
			GPRS_3763_NextATCount();//��1��AT
			return;
		#else
		case AT_ZCVF://�򿪶��Ź���(������CDMA(MC8332)ʱ��Ҫ)
		case AT_CTA://���ߵȴ�ʱ��(��CDMAʱ��Ҫ);�����ݴ���ʱ����10���CDMAͨ��ģ�������ͷſ��нӿ���Դ
			GPRS_3763_NextATCount();//��1��AT
			return;
		#endif//(USER/100)==14//����ϵ
	#endif
	
		case AT_MYNETSRV://���÷������
			switch(UART1Ctrl->SubTask)//������
			{
				case 0://ȷ������ģʽ
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F8,1);//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����;
			#else
				//DL698
				//ͨ�����á�=structure
				//����ģʽ  enum{���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��}��
				//���߷�ʽ      enum{�������ߣ�0�����������1��}��
				//���ӷ�ʽ      enum{TCP��0����UDP��1��}��
				//����Ӧ�÷�ʽ  enum{����ģʽ��0����������ģʽ��1��}��
					MR(ADDR_128KDATABUFF,ADDR_4500_2+2,6);
					p8=(u8*)ADDR_128KDATABUFF;
					i=p8[1]<<4;//B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ
					i|=p8[5]<<7;//B7=0TCP=1UDP
			#endif
					UART1Ctrl->TCPUDP=i;
					i&=0x30;
					if(UART1Ctrl->TCPUDP&0x80)
					{//UDP
						if(i==0x20)
						{//=2������ģʽ
							UART1Ctrl->SubTask=4;
						}
						else
						{//=0���ģʽ=1�ͻ���ģʽ
							UART1Ctrl->SubTask=3;
						}
					}
					else
					{//TCP
						if(i==0x20)
						{//=2������ģʽ
							UART1Ctrl->SubTask=2;
						}
						else
						{//=0���ģʽ=1�ͻ���ģʽ
							UART1Ctrl->SubTask=1;
						}
					}
					return;
				//case 1://TCP�ͻ���ģʽ
				default://TCP�ͻ���ģʽ
					//AT$MYNETSRV=0,0,0,0,"172.22.33.2:5000" 
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				TCPClient:
					p8tx[TxByte]='"';
					TxByte+=1;
					UART1Ctrl->LinkCount++;
			#if MainProtocol==376
					if(UART1Ctrl->LinkCount>=4)
					{
						UART1Ctrl->LinkCount=0;
					}
					if(UART1Ctrl->LinkCount<=2)
					{//��ip
						MR(ADDR_DATABUFF,ADDR_AFN04F3,6);
					}
					else
					{//����ip
						MR(ADDR_DATABUFF,ADDR_AFN04F3+6,6);
					}
			#else
					//DL698
					i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
					if(i==0)
					{//����ͨ����=0
						if(UART1Ctrl->LinkCount>=4)
						{
							UART1Ctrl->LinkCount=0;
						}
						i=MRR(ADDR_4500_3+1,1);
						if((UART1Ctrl->LinkCount<=2)||(i<2))
						{//��ip
							p8=Get_Element((u8*)ADDR_4500_3,1,1,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
							if(p8)
							{
								MR(ADDR_DATABUFF,(u32)p8+2,4);
								p8=Get_Element((u8*)ADDR_4500_3,1,2,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
								if(p8)
								{
									i=p8[1]<<8;
									i|=p8[2];
									p8=(u8*)(ADDR_DATABUFF);
									p8[4]=i;
									p8[5]=i>>8;
								}
							}
						}
						else
						{//����ip
							p8=Get_Element((u8*)ADDR_4500_3,2,1,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
							if(p8)
							{
								MR(ADDR_DATABUFF,(u32)p8+2,4);
								p8=Get_Element((u8*)ADDR_4500_3,2,2,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
								if(p8)
								{
									i=p8[1]<<8;
									i|=p8[2];
									p8=(u8*)(ADDR_DATABUFF);
									p8[4]=i;
									p8[5]=i>>8;
								}
							}
						}
					}
					else
					{//�б���ͨ����
						MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
						p8=(u8*)ADDR_128KDATABUFF;
						x=p8[1];//ȡ����ͨ����
						i=Comm_Ram->CSPN;//0=��,1=�й��ƶ�ͨ��-----china mobile  CMCC,2=�й���ͨͨѶ-----china unicom  CUCC,3=�й�����  ------CHINA TELECOM  CTCC		
						switch(i)
						{
							default:
							case 1://1=�й��ƶ�ͨ��-----china mobile  CMCC
								n=0;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
								break;
							case 2://2=�й���ͨͨѶ-----china unicom  CUCC
								n=2;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
								break;
							case 3://3=�й�����  ------CHINA TELECOM  CTCC	
								n=1;//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
								break;
						}
						p8+=2;
						for(i=0;i<x;i++)
						{
							if(p8[3]==n)//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
							{
								y=p8[5];//2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 �� 
								y+=2;
								if(y==Comm_Ram->RemoteWirelessModuleSYSINFO)//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
								{
									break;
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
						MC(0,ADDR_DATABUFF,6);
						if(i<x)
						{//�ҵ�
							if(UART1Ctrl->LinkCount>=4)
							{
								UART1Ctrl->LinkCount=0;
							}
							p8=Get_Element(p8,8,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
							if(p8)
							{
								i=p8[1];//��վͨ�Ų��� array��
								if((UART1Ctrl->LinkCount<=2)||(i<2))
								{//��ip
									p8+=2;
									MR(ADDR_DATABUFF,(u32)p8+4,4);
									p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
									if(p8)
									{
										i=p8[1]<<8;
										i|=p8[2];
										p8=(u8*)(ADDR_DATABUFF);
										p8[4]=i;
										p8[5]=i>>8;
									}
								}
								else
								{//����ip
									p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
									if(p8)
									{
										p8+=2;
										MR(ADDR_DATABUFF,(u32)p8+4,4);
										p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
										if(p8)
										{
											i=p8[1]<<8;
											i|=p8[2];
											p8=(u8*)(ADDR_DATABUFF);
											p8[4]=i;
											p8[5]=i>>8;
										}
									}
								}
							}
						}
					}
			#endif
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<4;i++)
					{
						y=hex_bcd(p8[i]);//HEX��תΪ8λѹ��BCD��
						TxByte+=BCD_ASCII(y,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
						p8tx[TxByte]='.';
						TxByte++;
					}
					TxByte--;//IP��4���ֽڵ�'.'ɾȥ
					//Ŀ�Ķ˿ں�
					p8tx[TxByte]=':';
					TxByte+=1;
					y=hex_bcd(p8[4]+(p8[5]<<8));//HEX��תΪ8λѹ��BCD��
					TxByte+=BCD_ASCII(y,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]='\r';
					TxByte++;
					break;
				case 2://TCP������ģʽ
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,1,0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					p8tx[TxByte]='"';
					TxByte++;
					TxByte+=CopyString((u8 *)"127.0.0.1",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					p8tx[TxByte]=':';
					TxByte+=1;
					//������ģʽ,�˿ں����趨
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F7+20,1);//�ն������˿� BIN 2
					i+=(20+1);
					i+=(1+MRR(ADDR_AFN04F7+i,1));
					i=MRR(ADDR_AFN04F7+i,2);
					i=hex_bcd(i);//HEX��תΪ8λѹ��BCD��
			#else
//#define ADDR_4500_2     ADDR_4003+LENmax_4003//����ͨ��ģ��1;2ͨ������
//ͨ�����á�=structure
//{
//����ģʽ  enum{���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��}��
//���߷�ʽ      enum{�������ߣ�0�����������1��}��
//���ӷ�ʽ      enum{TCP��0����UDP��1��}��
//����Ӧ�÷�ʽ  enum{����ģʽ��0����������ģʽ��1��}��
//�����˿��б�  array long-unsigned��
//APN            visible-string��
//�û���         visible-string��
//����           visible-string��
//�����������ַ octet-string��
//����˿�            long-unsigned��
//��ʱʱ�估�ط�����  bit-string(SIZE(8))
//��
//bit7~bit2����ʱʱ�䣨�룩��
//bit1~bit0���ط�����
//����
//��������(��)  long-unsigned
//}
					i=MRR(ADDR_4500_2+12,2);
					i=(i>>8)+((i&0xff)<<8);
					i=hex_bcd(i);//HEX��תΪ8λѹ��BCD��
			#endif
					TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]='\r';
					TxByte++;
					break;
				case 3://UDP�ͻ���ģʽ
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,2,0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					goto TCPClient;
				case 4://UDP������ģʽ
					//�޴�ģʽ
					WarningOccur(GPRSPORT,(u8 *)"Զ����������ģʽ��");//�澯����,��ڸ澯����͸澯�ַ���
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,0,0,0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					goto TCPClient;
			}
			break;
		case AT_MYNETOPEN://��������
			if(UART1Ctrl->LinkCtrl==0)//���ӿ���0=������,1=����
			{
				GPRS_3763_NextATCount();//��1��AT
			}
			else
			{
		#if MainProtocol==376
				i=MRR(ADDR_AFN04F8,1);
				i&=0x3;
				if(i==2)
				{//��������ģʽ
					if(UART1Ctrl->ReLinkNum==0)//��������ģʽ�ز�����,�յ�������������ʱ�ó�ֵ
					{//��������ģʽ�ز�����=0;
						i=MRR(ADDR_AFN05F38,1);
						if(i!=0x0)
						{
							MC(0,ADDR_AFN05F38,1);//������ͨ���Զ����������������0x55(0x5a)
						}
						GPRS_3763_NextATCount();//��1��AT
						break;
					}
					UART1Ctrl->ReLinkNum--;//��������ģʽ�ز�����,�յ�������������ʱ�ó�ֵ
				}
				else
				{//��������ģʽ,ʱ������ģʽ
					if(Terminal_Ram->GPRSReConnection_S_Timer!=0x0)//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
					{
						WarningOccur(GPRSPORT,(u8 *)"Զ�������ز������ʱ");//�澯����,��ڸ澯����͸澯�ַ���
						GPRS_3763_NextATCount();//��1��AT
						break;
					}
					//ÿ�ε�¼ʧ�ܺ󣬾�����������0.5-1.5���������ʱ����������ƣ������µ�¼
				#if (USER/100)==13//������ר��
					Terminal_Ram->GPRSReConnection_S_Timer=0;//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
				#else
					i=MRR(ADDR_AFN04F8+1,2);//�������ߡ�ʱ������ģʽ�ز���� BIN �� 2
					Terminal_Ram->GPRSReConnection_S_Timer=i;//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
				#endif
				}
		#else
				//698.45
				
				
		#endif
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				Terminal_Ram->GPRSWaitConnection_S_Timer=90;//20 GPRS�������ӷ���ȴ��붨ʱ��
				UART1Ctrl->NETOPENCount++;//���������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
			}
			break;

		case AT_LOOP_NOLINK://ATѭ��,û����
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->SubTask=0;//������
		#if (USER/100)==13//������ר��
			if(UART1Ctrl->NETOPENCount>=5)//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
		#else
			if(UART1Ctrl->NETOPENCount>=3)//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
		#endif
			{
				UART1Ctrl->NETOPENCount=0;//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
				UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
			}
			else
			{
				if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
				{
					UART1Ctrl->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];
					x=0;//Ҫ���·������ӱ�־
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F8,1);
					i&=0x3;
			#else
					i=0;
					
			#endif
					if(i==2)
					{//��������ģʽ
						if(UART1Ctrl->ReLinkNum!=0)//��������ģʽ�ز�����,�յ�������������ʱ�ó�ֵ
						{//��������ģʽ�ز�����=0;
							x=1;//Ҫ���·������ӱ�־
						}
					}
					else
					{//��������ģʽ,ʱ������ģʽ
						if(Terminal_Ram->GPRSReConnection_S_Timer==0x0)//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
						{
							x=1;//Ҫ���·������ӱ�־
						}
					}
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->SubTask=0;//������
					if(x==0)//Ҫ���·������ӱ�־
					{
						UART1Ctrl->ATCount=AT_SMS_NOLINK;//���ŷ���û����ǰ
					}
					else
					{
						UART1Ctrl->ATCount=AT_MYNETSRV;//���÷������
					}
				}
			}
			break;
		
		
		case AT_CSQ_LINK://���ź����Ӻ�(ÿ��1��)
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
			{
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			else
			{
				GPRS_3763_NextATCount();//��1��AT
				return;
			}
			break;
		case AT_SYSINFO:
		case AT_SYSINFO_LINK://
	#if MainProtocol==376
		#if (USER/100)!=0//���ǵ��Ժ�ͼ�
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->TYMDHMS[0])//
			{
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//
			}
			else
			{
				GPRS_3763_NextATCount();//??1??AT
				return;
			}
			break;
		#else
			GPRS_3763_NextATCount();//??1??AT
			return;
		#endif
	#endif
	#if MainProtocol==698
		#if (USER/100)!=0//���ǵ��Ժ�ͼ�
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])
			{
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);
			}
			else
			{
				GPRS_3763_NextATCount();//??1??AT
				return;
			}
			break;
		#else
			GPRS_3763_NextATCount();//??1??AT
			break;
		#endif
	#endif
		case AT_MYNETOPEN_LINK://���Ӽ��(ÿ��1��)
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
			{
				UART1Ctrl->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			}
			else
			{
				GPRS_3763_NextATCount();//��1��AT
				return;
			}
			break;
		case AT_MYNETREAD://��ȡ����
			p16=(u16*)(ADDR_UART1IntRx);
			if(p16[0]!=p16[1])
			{
				UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			#if MainProtocol==698
				Uart_698_RxTx(GPRSPORT);//UART_3761���շ���
			#else
				Uart_3761_RxTx(GPRSPORT);//UART_3761���շ���
			#endif
				return;
			}
			else
			{
				TxByte=CopyString((u8 *)"AT$MYNETREAD=0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				//i=LEN_UART1IntATRx-1024;
				i=1460;
				i=hex_bcd(i);//HEX��תΪ8λѹ��BCD��
				TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
				p8tx[TxByte]='\r';
				TxByte++;
			}
			break;
		case AT_MYNETWRITE://��������
			switch(UART1Ctrl->ATWriteTask)//AT���ݷ�������:0=������,1=������
			{
				case 0://��ATͷ
					TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					i=Terminal_Ram->CopyTxByte;//���跢���ֽ���
					if(i==0)
					{//���跢���ֽ�������
						if(UART1Ctrl->Task==6)//6=��֡�������ڷ���
						{
						#if MainProtocol==376
							UART1Ctrl->Task=4;//4=��֡�������������
						#else
							UART1Ctrl->Task=0;
						#endif
						}
						else
						{
							UART1Ctrl->Task=0;//0=����
						}
						UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
						GPRS_3763_NextATCount();//��1��AT
						return;
					}
					if(i>1460)
					{
						i=1460;
					}
					i=hex_bcd(i);
					TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8tx[TxByte]='\r';
					TxByte++;
					break;
				case 1://������
					i=Terminal_Ram->ReqTxByte;
					x=Terminal_Ram->CopyTxByte;
					y=Terminal_Ram->CopyTxCount;
					if((i>LEN_UART1Tx)||(i>x)||(i==0))
					{//�ɷ��ͳ��ȴ���,������֡
						UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
						UART1Ctrl->Task=7;//0=����
						GPRS_3763_NextATCount();//��1��AT
						return;
					}
					Terminal_Ram->CopyTxByte=x-i;//���跢���ֽ�
					Terminal_Ram->CopyTxCount=y+i;//�ѷ��ͼ���
					
					UART1Ctrl->TxByte=i;
			    UART1Ctrl->TxCount=LEN_ATTx+y;
				#if LOG_1376_3==1//0=û,1=��
					LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx+LEN_ATTx+y,i,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				#endif
				#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
					LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx+LEN_ATTx+y,i,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				#endif
					
					Start_UARTn_Tx(GPRSPORT);//����UART�ڷ���
					
					x=MRR((u32)&UART1Ctrl->RxTxByte,4);
					x+=i;
					MWR(x,(u32)&UART1Ctrl->RxTxByte,4);//ͨ������
					UART1Ctrl->ATTask=2;//AT����:0=����,1=����,2=���ڷ���,3=����
					return;
				default:
					UART1Ctrl->ATWriteTask=0;//AT���ݷ�������:0=������,1=������
					break;
			}
			break;
		case AT_MYFTP://FTP�ļ�����
			if(UART1Ctrl->LinkTask!=101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{
				GPRS_3763_NextATCount();//��1��AT
				return;
			}
		//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
		//�������
			FTPFileCount=MRR(ADDR_FTPFileFlags+4,1);//�����ļ�����;0-x,=MaxFileNum��FTP,=(MaxFileNum+1)����֡,=(MaxFileNum+2)���������ʾ,=(MaxFileNum+3)����������ʾ
			if(FTPFileCount>(MaxFileNum+1))//��������ļ�����
			{
				UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
				UART1Ctrl->FTPLinkCount=0;//FTP�������Ӽ���
				UART1Ctrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
				GPRS_3763_NextATCount();//��1��AT
				return;
			}
			Comm_Ram->DisplayAllErrorTimer=10;//FTP�ļ�������ʾ�붨ʱ��
			x=UART1Ctrl->FTPErr;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
			if((x!=0)||(FTPFileCount==(MaxFileNum+1)))
			{
				if(x==1)
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
				
				p8rx=(u8 *)ADDR_DATABUFF;
				p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MR(ADDR_DATABUFF,ADDR_AFN0FF1_Rx,256);//�ָ�FTP����֡��ǰ256�ֽ�
				MR((u32)p8tx,ADDR_AFN0FF1_Tx,18);//�ָ�FTP����֡��ǰ18�ֽ�
				MWR(y,((u32)p8tx)+18,4);//���ر�ʶ4�ֽ�
				i=4+4;//���س���
				i+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
				p8tx[1]=i;
				p8tx[2]=i>>8;
				i=p8rx[1]+(p8rx[2]<<8);
//				Uart_376_AUX(GPRSPORT,((u32)p8rx)+6+i-6);//������Ϣ(EC,TP),�ݵȴ���֡���ݷ���
//				Uart_376_CS((u32)p8tx);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				
				GPRS_3763_NextATCount();//��1��AT
				UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����

			#if (USER/100)==5//�Ϻ�
				i=MRR(ADDR_AFN0FF97,1);//1byte ������־:0=����F97����,1=��F97�����
				if(i!=0)
				{//��F97�����,�޷���֡
					UART1Ctrl->Task=0;//0=����
				}
				//�������ն�����汾��	ASCII	10
				MC(0,ADDR_DATABUFF,10);
				p8rx=(u8*)(ADDR_DATABUFF);
				p8rx[0]='Z';
				p8rx[1]='Z';
				p8rx[2]=((SVER>>4)&0xf)+0x30;
				p8rx[3]=((SVER>>0)&0xf)+0x30;
				//����ʱ�䣺��ʱ������	����¼A.15	5
				MR(ADDR_DATABUFF+10,ADDR_TYMDHMS+1,5);
				MW(ADDR_DATABUFF,ADDR_AFN0FF98+10,15);
			#endif
				return;
			}
	#if Extern_GPRS_FTP_RxTx==0//�ⲿ����:0=I2,1=E1,2=E2,3=;W5200 GPRS�ļ�����
			TxByte=GPRS_FTP_Tx();//GPRS_FTP_Tx����
	#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
    		p32=(u32 *)(ADDR_E1_ENTRY);
    		TxByte=(*(u32(*)())p32[106])();//GPRS_FTP_Tx����
			}
	#endif
			break;
			
		case AT_SMS_NOLINK://���ŷ���û����ǰ
		case AT_SMS_LINK://���ŷ���
		#if LEN_UART11Ctrl==0
			GPRS_3763_NextATCount();//��1��AT
		#else
			TxByte=GPRS_SMS_Tx();//GPRS_SMS_Tx����;���ط����ֽ���
		#endif
			break;
			
		case AT_LOOP://ATѭ��,���߿���
			if(UART1Ctrl->LinkCtrl==0)//���ӿ���0=������,1=����
			{
				if(UART1Ctrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					UART1Ctrl->LinkTask=102;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					UART1Ctrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
					Terminal_Ram->GPRSHeartBeat_S_Timer=0;//2 �����붨ʱ��
				}
				if(UART1Ctrl->LinkTask==103)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
					return;
				}
			}
			UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
			UART1Ctrl->SubTask=0;//������
			UART1Ctrl->ATCount=AT_CSQ_LINK;
			break;
			
		default:
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			break;
	}
	if(TxByte!=0)
	{
		//����UART1�ڷ���
		UART1Ctrl->BpsCtrl=0x3+(9<<5);//����57.6K 
		Init_UARTn(GPRSPORT);//��ʼ��UART��,���ͨ�ſ�����������
		UART1Ctrl->TxByte=TxByte;//�����ֽ���(���ֶ���)
    UART1Ctrl->TxCount=0;
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
	#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
		switch(p32[4*AT_No])//AT����
		{
			case AT_CSQ_LINK://���ź����Ӻ�(ÿ��1��)
			case AT_MYNETOPEN_LINK://���Ӽ��(ÿ��1��)
			case AT_MYNETREAD://������
				
				break;
			default:
				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				break;
		}
	#endif

		Start_UARTn_Tx(GPRSPORT);//����UART�ڷ���
		UART1Ctrl->ATTask=2;//AT����:0=����,1=����,2=���ڷ���,3=����
	}
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



void GPRS_3763_AT_Rx(void)//AT�������
{
	u32 i;
	u32 x;
	u32 y;
	u32 AT_No;
	u32 RxByte;
	u32 Addr;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;
	u16 *p16fifo;
	u32 *p32;
	u32 p0;
	u32 p1;
#if LOG_1376_3||DEBUG_GPRStoRS232
	u8* p8LOG;
#endif
	
	DMA_UARTn_RX_Pointer(GPRSPORT);//����DMA_UARTn����ָ��ֵ(p0)
	
	AT_No=UART1Ctrl->ATCount;//AT����
	if(AT_No>AT_Max)
	{
		AT_No=0;
		UART1Ctrl->ATCount=0;//AT����
	}
	p16fifo=(u16*)(ADDR_UART1IntATRx);
	if(p16fifo[0]!=p16fifo[2])
	{//���ֽڽ���
		p16fifo[2]=p16fifo[0];
		Terminal_Ram->AT_MS_Timer=1000/10;//AT���յ�һ���ֽں�ʱ��ʱֵ
	}
//	if(Terminal_Ram->AT_MS_Timer!=0)
//	{//����û���
//		return;
//	}
//	UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	switch(p32[4*AT_No])//AT����
	{
		case AT_AT://�����ʼ��
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=2;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{//�ط���AT
				UART1Ctrl->LinkSubTask++;//����AT���ͼ���
				if(UART1Ctrl->LinkSubTask>=3)//����AT���ͼ���
				{//����MC8332ģ�鵱�Զ�����ʱͷ2�ε�AT����Ӧ(���ڼ��),��3�β���Ӧ
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					switch(UART1Ctrl->SubTask)//������
					{
						case 0://����57600
						case 1://����115200
						case 2://����38400
						case 3://����19200
						case 4://����9600
						case 5://����4800
						case 6://����2400
						//case 7://����1200
							UART1Ctrl->SubTask++;//������
							return;
						//case 10://��1��AT
						default:
						#if (USER/100)==0//�û���ʶ,���Ժ���Բ�֧��AT+IPR
							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
							GPRS_3763_NextATCount();//��1��AT
						#else
							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
							UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
						#endif
							return;
					}
				}
			}
			else
			{//���յ�OK
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				if(UART1Ctrl->SubTask<10)//������
				{
					UART1Ctrl->SubTask+=10;//������
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				}
				else
				{
					GPRS_3763_NextATCount();//��1��AT
				}
			}
			return;
		case AT_MYTYPE://��ѯԶ��ͨ��ģ������
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Comm_Ram->RemoteWirelessModuleType=1;//GPRSģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
			Addr=ATRxSearch((u32)((u8*)"$MYTYPE:"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				x=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(x!=0)
				{
					p8s=(u8*)x;
					i = ASCII_BCD(x,2);
//					i=((p8s[0]-0x30)<<4)+(p8s[1]-0x30);
					Comm_Ram->RemoteWirelessModuleType=i;//GPRSģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
				}
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_3763_NextATCount();//��1��AT
				break;
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			if(UART1Ctrl->LinkSubTask<=2)//����AT���ͼ���
			{
				UART1Ctrl->LinkSubTask++;//����AT���ͼ���
				break;
			}
			GPRS_3763_NextATCount();//��1��AT
			break;
			
		case AT_MYGMR://��ȡģ������汾
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
			#if MainProtocol==376
				//���̴��ţ�ASCII��4�ֽ�
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_AFN09F9,4);
				//ģ���ͺţ�ASCII��8�ֽ�
				MC(0,ADDR_DATABUFF,8);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_AFN09F9+4,8);
				//����汾�ţ�ASCII��4�ֽ�
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8,4);
				//����������ڣ������꣬3�ֽ�
				MC(0,ADDR_DATABUFF,6);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				i=ASCII_BCD(ADDR_DATABUFF,6);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
				MWR(i,ADDR_AFN09F9+4+8+4,3);
				Data_Inverse(ADDR_AFN09F9+4+8+4,3);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//Ӳ���汾�ţ�ASCII��4�ֽ�
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8+4+3,4);
				//Ӳ���������ڣ������꣬3�ֽ�
				MC(0,ADDR_DATABUFF,6);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				i=ASCII_BCD(ADDR_DATABUFF,6);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
				MWR(i,ADDR_AFN09F9+4+8+4+3+4,3);
				Data_Inverse(ADDR_AFN09F9+4+8+4+3+4,3);//���ݵ���(�ߵ��ֽڵ���),���Le>=2		
				
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				GPRS_3763_NextATCount();//��1��AT
			#endif
			#if MainProtocol==698
//�汾��Ϣ��=structure
//{
//2 ���̴���       visible-string(SIZE (4))��
//8 ����汾��     visible-string(SIZE (4))��
//14 ����汾����   visible-string(SIZE (6))��
//22 Ӳ���汾��     visible-string(SIZE (4))��
//28 Ӳ���汾����   visible-string(SIZE (6))��
//36 ������չ��Ϣ   visible-string(SIZE (8))
//}
				//���̴��ţ�ASCII��4�ֽ�
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_4500_5+4,4);
				//ģ���ͺţ�ASCII��8�ֽ�
				MC(0,ADDR_DATABUFF,8);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_4500_5+38,8);
				//����汾�ţ�ASCII��4�ֽ�
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_4500_5+10,4);
				//����������ڣ������꣬3�ֽ�
				MC(0,ADDR_DATABUFF,6);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_4500_5+16,6);
				//Ӳ���汾�ţ�ASCII��4�ֽ�
				MC(0,ADDR_DATABUFF,4);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_4500_5+24,4);
				//Ӳ���������ڣ������꣬3�ֽ�
				MC(0,ADDR_DATABUFF,6);
				GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				MW(ADDR_DATABUFF,ADDR_4500_5+30,6);
				
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				GPRS_3763_NextATCount();//��1��AT
			#endif
				break;
			}
			goto ReTxAT;//�ط���AT
		case AT_CPIN://�����
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=3;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"READY"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_3763_NextATCount();//��1��AT
				break;
			}
			goto ReTxAT;//�ط���AT
		case AT_MYCCID://��ȡSIM�����к�ICCID
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"$MYCCID"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				LineATIntRx(Addr+3,ADDR_DATABUFF,20);//ȡAT�жϽ����������Է��ڻ�����
		#if MainProtocol==698
				p8s=(u8*)ADDR_4500_7;//����ͨ��ģ��1;7 SIM����ICCID
				//visible-string(SIZE(20))
				p8s[0]=DataType_visible_string;
				p8s[1]=20;
				MW(ADDR_DATABUFF,(u32)p8s+2,20);
		#endif
		#if MainProtocol==376
			#if (USER/100)!=12//���Ĵ�
				//�Ĵ�Ҫ��AFN09F9��ICCID����дSIM���ֻ����롣
				MW(ADDR_DATABUFF,ADDR_AFN09F9+4+8+4+3+4+3,20);
			#endif
			#if (USER/100)==12//�Ĵ�
				//�Ĵ���չAFN0CF2040��дICCID
//				MW(ADDR_DATABUFF,ADDR_AFN0CF16+8,20);
				MW(ADDR_DATABUFF,ADDR_AFN0CF2040+7,20);
			#endif
			#if (USER/100)==13//������
				WarningOccur(GPRSPORT,(u8 *)"��ȡSIM��ICCID");//�澯����,��ڸ澯����͸澯�ַ���
			#endif
		#endif
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_3763_NextATCount();//��1��AT
				break;
			}
			goto ReTxAT;//�ط���AT
		case AT_CNUM://ȡ��������
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			p8d=(u8*)(ADDR_MobilePhoneNumber);
			p8d[0]=0;
			Addr=ATRxSearch((u32)((u8*)"+CNUM:"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					LineATIntRx(Addr,ADDR_DATABUFF,20);//ȡAT�жϽ����������Է��ڻ�����
					p8s=(u8*)(ADDR_DATABUFF);
					if(p8s[0]=='"')
					{
						p8s++;
						for(i=0;i<19;i++)
						{
							x=p8s[i];
							if(x=='"')
							{
								break;
							}
							p8d[i]=x;
						}
						p8d[i]=0;
					}
				}
			#if MainProtocol==698
				p8s=(u8*)(ADDR_MobilePhoneNumber);
				p8d=(u8*)ADDR_4500_10;
				//visible-string(SIZE(16))
				p8d[0]=DataType_visible_string;
				for(i=0;i<16;i++)
				{
					if(p8s[i]==0)
					{
						break;
					}
					p8d[2+i]=p8s[i];
				}
				p8d[1]=i;
			#endif
		#if MainProtocol==376
			#if (USER/100)==12//�Ĵ�
				//�Ĵ�Ҫ��AFN09F9��ICCID����дSIM���ֻ����롣
				for(i=0;i<20;i++)
				{
					if(p8d[i]==0)
					{
						break;
					}
				}
				MC(0x30,ADDR_AFN09F9+4+8+4+3+4+3,(20-i));
				MW(ADDR_MobilePhoneNumber,ADDR_AFN09F9+4+8+4+3+4+3+(20-i),i);//дSIM���ֻ�����
				if(i>8)
				{//�Ĵ���չ6���ֽ�BCD��绰����
					y=ASCII_BCD(ADDR_MobilePhoneNumber+i-8,8);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
					MWR(y,ADDR_AFN0CF2040+1,4);//дSIM���ֻ�����
					y=ASCII_BCD(ADDR_MobilePhoneNumber,(i-8));//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
					MWR(y,ADDR_AFN0CF2040+1+4,2);//дSIM���ֻ�����
					Data_Inverse(ADDR_AFN0CF2040+1,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				}
				else
				{
					y=ASCII_BCD(ADDR_MobilePhoneNumber,i);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
					MWR(y,ADDR_AFN0CF2040+1,4);//дSIM���ֻ�����
					MWR(0,ADDR_AFN0CF2040+1+4,2);//дSIM���ֻ�����
					Data_Inverse(ADDR_AFN0CF2040+1,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				}		
			#endif
		#endif
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//����AT���ͼ���
				{
					UART1Ctrl->LinkSubTask++;//����AT���ͼ���
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
					break;
				}
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			GPRS_3763_NextATCount();//��1��AT
			break;
		case AT_CIMI://������ƶ�̨��ʶ����IMSI
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			if(Addr!=0)
			{
				p1=p16fifo[1];
				p8s=(u8*)(ADDR_UART1IntATRx+10);
				while((p8s[p1]==0x0d)||(p8s[p1]==0x0a))
				{
					p1++;
					if(p1>=(LEN_UART1IntATRx-10))
					{
						p1-=(LEN_UART1IntATRx-10);
					}
				}
				Addr=ADDR_UART1IntATRx+10+p1;
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
			#if MainProtocol==698
				p8s=(u8*)(ADDR_DATABUFF);
				p8d=(u8*)(ADDR_4500_8);//����ͨ��ģ��1;8 IMSI
				//visible-string(SIZE(20))
				p8d[0]=DataType_visible_string;
				p8d[1]=19;
				p8d+=2;
				for(i=0;i<19;i++)
				{
					if((p8s[i]==0x0d)||(p8s[i]==0x0a))
					{
						break;
					}
					p8d[i]=p8s[i];
				}
			#endif
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//����AT���ͼ���
				{
					UART1Ctrl->LinkSubTask++;//����AT���ͼ���
					break;
				}
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			GPRS_3763_NextATCount();//��1��AT
			break;
/*
		case AT_CSPN://��ȡSIM��������Ӫ������
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=4;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"+CSPN: \""));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
//�й��ƶ�ͨ��-----china mobile  CMCC
//�й���ͨͨѶ-----china unicom  CUCC
//�й�����  ------CHINA TELECOM  CTCC	
				x=GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				p8s=(u8*)(ADDR_DATABUFF);
				for(i=0;i<x;i++)
				{
					if(p8s[i]=='"')
					{
						p8s[i]=0;
						break;
					}
				}
				i=(p8s[0]<<24)|(p8s[1]<<16)|(p8s[2]<<8)|(p8s[3]<<0);
				switch(i)
				{
					case ('C'<<24)|('M'<<16)|('C'<<8)|('C'<<0):
						Comm_Ram->CSPN=1;//1=�й��ƶ�ͨ��-----china mobile  CMCC
						break;
					case ('C'<<24)|('U'<<16)|('C'<<8)|('C'<<0):
						Comm_Ram->CSPN=2;//2=�й���ͨͨѶ-----china unicom  CUCC
						break;
					case ('C'<<24)|('T'<<16)|('C'<<8)|('C'<<0):
						Comm_Ram->CSPN=3;//3=�й�����  ------CHINA TELECOM  CTCC
						break;
					default:
						Comm_Ram->CSPN=255;//255=δ֪
						break;
				}
				WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			}
			
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//����AT���ͼ���
				{
					UART1Ctrl->LinkSubTask++;//����AT���ͼ���
					break;
				}
			}
			GPRS_3763_NextATCount();//��1��AT
			break;
*/
#if (USER/100)!=0//���ǵ��Ժ����(1376.3��׼�޴�����)
		case AT_COPS://��ȡSIM��������Ӫ������
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=4;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"+COPS:"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)"\""));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
//�й��ƶ�ͨ��-----china mobile  CMCC   46000 46002 46004 46007 46008
//�й���ͨͨѶ-----china unicom  CUCC   46001 46006 46009 46010
//�й�����  ------CHINA TELECOM  CTCC	 46003 46005 46011 46012
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					x=GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
					i=ASCII_HEX(ADDR_DATABUFF);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
					switch(i)
					{
						case 46000:
						case 46002:
						case 46004:
						case 46007:
						case 46008:
							Comm_Ram->CSPN=1;//1=�й��ƶ�ͨ��-----china mobile  CMCC
							WarningOccur(GPRSPORT,(u8*)"CMCC");//�澯����,��ڸ澯����͸澯�ַ���
							break;
						case 46001:
						case 46006:
						case 46009:
						case 46010:
							Comm_Ram->CSPN=2;//2=�й���ͨͨѶ-----china unicom  CUCC
							WarningOccur(GPRSPORT,(u8*)"CUCC");//�澯����,��ڸ澯����͸澯�ַ���
							break;
						case 46003:
						case 46005:
						case 46011:
						case 46012:
							Comm_Ram->CSPN=3;//3=�й�����  ------CHINA TELECOM  CTCC
							WarningOccur(GPRSPORT,(u8*)"CTCC");//�澯����,��ڸ澯����͸澯�ַ���
							break;
						default:
#if MainProtocol==376
              Comm_Ram->CSPN=1;//1=�й��ƶ�ͨ��-----china mobile  CMCC
							WarningOccur(GPRSPORT,(u8*)"δ֪��Ӫ��");//�澯����,��ڸ澯����͸澯�ַ���
#endif
#if MainProtocol==698
              Comm_Ram->CSPN=0;
#endif
							break;
					}
				}
#if MainProtocol==698
        i=Comm_Ram->CSPN;
        if(i==0)
        {
          p8d=(u8*)ADDR_DATABUFF;
          p8s=(u8*)(ADDR_4500_8);//����ͨ��ģ��1;8 IMSI
          MR(ADDR_DATABUFF,(u32)p8s+2,5);
          p8d[5]=0;
          i=ASCII_HEX(ADDR_DATABUFF);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
          switch(i)
          {
            case 46000:
            case 46002:
            case 46004:
            case 46007:
            case 46008:
              Comm_Ram->CSPN=1;//1=�й��ƶ�ͨ��-----china mobile  CMCC
              WarningOccur(GPRSPORT,(u8*)"CMCC");//�澯����,��ڸ澯����͸澯�ַ���
              break;
            case 46001:
            case 46006:
            case 46009:
            case 46010:
              Comm_Ram->CSPN=2;//2=�й���ͨͨѶ-----china unicom  CUCC
              WarningOccur(GPRSPORT,(u8*)"CUCC");//�澯����,��ڸ澯����͸澯�ַ���
              break;
            case 46003:
            case 46005:
            case 46011:
            case 46012:
              Comm_Ram->CSPN=3;//3=�й�����  ------CHINA TELECOM  CTCC
              WarningOccur(GPRSPORT,(u8*)"CTCC");//�澯����,��ڸ澯����͸澯�ַ���
              break;
            default:
              Comm_Ram->CSPN=0;
              break;
          }
          i=Comm_Ram->CSPN;
          if(i==0)
          {
            p8s=(u8*)ADDR_4500_7;//����ͨ��ģ��1;7 SIM����ICCID
            MR(ADDR_DATABUFF,(u32)p8s+2,6);
            p8d[6]=0;
            i=ASCII_HEX(ADDR_DATABUFF);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
            //�Ĵ�Ҫ��376�ն˵�ICCID����дSIM���ֻ����룬698�ն�δ֪
            switch(i)
            {
              case 898600:
              case 898602:
              case 898604:
              case 898607:
              case 898608:
                Comm_Ram->CSPN=1;//1=�й��ƶ�ͨ��-----china mobile  CMCC
                WarningOccur(GPRSPORT,(u8*)"CMCC");//�澯����,��ڸ澯����͸澯�ַ���
                break;
              case 898601:
              case 898606:
              case 898609:
              case 898610:
                Comm_Ram->CSPN=2;//2=�й���ͨͨѶ-----china unicom  CUCC
                WarningOccur(GPRSPORT,(u8*)"CUCC");//�澯����,��ڸ澯����͸澯�ַ���
                break;
              case 898603:
              case 898605:
              case 898611:
              case 898612:
                Comm_Ram->CSPN=3;//3=�й�����  ------CHINA TELECOM  CTCC
                WarningOccur(GPRSPORT,(u8*)"CTCC");//�澯����,��ڸ澯����͸澯�ַ���
                break;
              default:
                Comm_Ram->CSPN=1;//1=�й��ƶ�ͨ��-----china mobile  CMCC
                WarningOccur(GPRSPORT,(u8*)"δ֪��Ӫ��");//�澯����,��ڸ澯����͸澯�ַ���
                break;
            }
          }
        }
#endif
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//����AT���ͼ���
				{
					UART1Ctrl->LinkSubTask++;//����AT���ͼ���
					break;
				}
			}
			GPRS_3763_NextATCount();//��1��AT
			break;
#endif	
			//���¼�����������ʧ��ʱ����ģ��,��ʹ���ն���
		case AT_MYNETCON_APN://����APN
		case AT_MYNETCON_USERPWD://����USERPWD:�û���������
		case AT_MYNETSRV://���÷������
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			if(Addr!=0)
			{
			#if (USER/100)==13//������
				if(p32[4*AT_No]==AT_MYNETCON_APN)
				{
					WarningOccur(GPRSPORT,(u8 *)"����APN");//�澯����,��ڸ澯����͸澯�ַ���
				}
				if(p32[4*AT_No]==AT_MYNETCON_USERPWD)
				{
					WarningOccur(GPRSPORT,(u8 *)"�����û�������");//�澯����,��ڸ澯����͸澯�ַ���
				}
			#endif
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//�޸��Ѷ�ָ��
			}
			else
			{
				if(UART1Ctrl->LinkSubTask<=2)//����AT���ͼ���
				{
					UART1Ctrl->LinkSubTask++;//����AT���ͼ���
					break;
				}
			}
			GPRS_3763_NextATCount();//��1��AT
			break;
			
		case AT_CSQ_NOLINK://���ź�û����ǰ
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=5;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"+CSQ: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
				LineATIntRx(Addr,ADDR_DATABUFF,2);//ȡAT�жϽ����������Է��ڻ�����
				GPRS_Signal(ADDR_DATABUFF);//GPRS�ź�ǿ��0-4
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				if((Terminal_Ram->SignaldBm>=5)&&(Terminal_Ram->SignaldBm!=0x99))
				{//ģ��⵽�ź�,��û�ź���ȴ�120��
					GPRS_3763_NextATCount();//��1��AT
				}
				else
				{
					if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//GPRS�ȴ��ź�ǿ�Ȳ�ѯ(����ǰ)��ʱ
					{
						WarningOccur(GPRSPORT,(u8 *)"Զ���������ź�");//�澯����,��ڸ澯����͸澯�ַ���
						UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
					}
				}
			#if (USER/100)==13//������
				WarningOccur(GPRSPORT,(u8 *)"Զ�����߶��ź�");//�澯����,��ڸ澯����͸澯�ַ���
			#endif
				return;
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			goto ReTxAT;//�ط���AT
		case AT_CREG://��ȡ��ǰ����ע��״̬
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
		#if (USER/100)==13//������
			Addr=ATRxSearch((u32)((u8*)"+CGREG:"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
		#else
			Addr=ATRxSearch((u32)((u8*)"+CREG:"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
		#endif
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					i=MRR(Addr,1);
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
					switch(i)
					{
						//case '0'://û��ע�ᣬME���ڲ�û������ѰҪע����µ���Ӫ��
						//case '4'://δ֪ԭ��
						default:
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//�ȴ�ע�ᶨʱ
							{
								WarningOccur(GPRSPORT,(u8 *)"Զ������ע��ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
								UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
							}
							return;
						case '1'://ע���˱�������
							GPRS_3763_NextATCount();//��1��AT
							break;
						case '2'://û��ע�ᣬ��MS������ѰҪע����µ���Ӫ��
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//�ȴ�ע�ᶨʱ
							{
								WarningOccur(GPRSPORT,(u8 *)"Զ������ע����Ѱ��ʱ");//�澯����,��ڸ澯����͸澯�ַ���
								UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
							}
							return;
						case '3'://ע�ᱻ�ܾ�
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//�ȴ�ע�ᶨʱ
							{
								WarningOccur(GPRSPORT,(u8 *)"Զ������ע�ᱻ�ܾ�");//�澯����,��ڸ澯����͸澯�ַ���
								UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
							}
							return;
						case '5'://ע������������
							WarningOccur(GPRSPORT,(u8 *)"Զ������ע����������");//�澯����,��ڸ澯����͸澯�ַ���
							GPRS_3763_NextATCount();//��1��AT
							break;
					}
					break;
				}
			}
			goto ReTxAT;//�ط���AT
		case AT_SYSINFO_LINK:
		case AT_SYSINFO://��ѯע���������ͣ�2G/3G/4G
//����
//AT$MYSYSINFO
//����ֵ
//<CR><LF>$ MYSYSINFOsysmode>��<MNC>
//<CR><LF>OK<CR><LF>
//ע��
//��a��<sysmode>:��ǰע���������ʽ��
//   ��1��0��No service
//   ��2��2��2G������GPRS,CDMA��
//   ��3��3��3G������WCDMA,TD��S CDMA,CDMA EVD0��
//   ��4��4��4G������FDD��LTE, FDD��LTE��
//��b��<MNC>:������Ӫ�̴��룺
//   ��1��00��ע��ʧ��
//   ��2��01��China Mobile���й��ƶ���
//   ��3��02��China Unicom���й���ͨ��
//   ��4��03��China Telecom���й����ţ�
//ʾ����
//AT$MYSYSINFO
//$MYSYSINFO��4��01  //��ʾ��ǰע�����й��ƶ���4G������
//OK
		
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Comm_Ram->RemoteWirelessModuleType=1;//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
			Addr=ATRxSearch((u32)((u8*)"$MYSYSINFO: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				p8d = (u8 *)Addr;
				i=p8d[0]-0x30;
				switch(i)
				{
					//case 0://�޷���
					default:
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						break;
					case 0://�޷���
						if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
						{
							GPRS_3763_NextATCount();//��1��AT							
						}
						else
						{//�ȴ�������ȷע���������ͣ�2G/3G/4G
							UART1Ctrl->LinkSubTask=0;//����AT���ͼ���											
							goto ReTxAT;
						}
						break;
					case 2://GSM/GPRSģʽ 2G
						//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						break;
					case 3://WCDMAģʽ 3G
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						break;
					case 4://17://LTEģʽ 4G
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						//Comm_Ram->RemoteWirelessModuleType=(1<<5);//b5=1
						break;
				}
				i=p8d[3]-0x30;
				switch(i)
				{
					case 0:
							if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
							{						
								GPRS_3763_NextATCount();//��1��AT
							}
							else
							{//�ȴ�������ȷע���������ͣ�2G/3G/4G
								UART1Ctrl->LinkSubTask=0;//����AT���ͼ���											
								goto ReTxAT;
							}
							break;
					case 1: //1=�й��ƶ�ͨ��china mobile
					case 2: //2=�й���ͨͨѶchina unicom
					case 3: //3=�й�����CHINA TELECOM
					default:
							Comm_Ram->CSPN=i;
							break;
				}
				
#if (USER/100)==15//
		   	i=p8d[0]-0x30;
				x=Comm_Ram->RemoteWirelessModuleSYSINFO;//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
				Comm_Ram->RemoteWirelessModuleSYSINFO=i;//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
				if(x!=i)
				{//ע������仯(2G,3G,4G)
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->SubTask=0;//������
					UART1Ctrl->ATCount=AT_MYNETCON_APN;
					return;
				}
				i=p8d[3]-0x30;
        x=Comm_Ram->CSPN;
        Comm_Ram->CSPN=i;
        if(x!=i)
				{//ע������仯(1=�й��ƶ�ͨ��china mobile,2=�й���ͨͨѶchina unicom,3=�й�����CHINA TELECOM)
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					UART1Ctrl->SubTask=0;//������
					UART1Ctrl->ATCount=AT_MYNETCON_APN;
					return;
				}
#endif
#if (USER/100)==9//���ֺ����г��ĵ�����������ֻ֧��4G���磬���û��ע�ᵽ4G�������ڴ˵ȴ���GPRSWaitTx_S_Timer����
        x=UART1Ctrl->LinkTask;
        if(x<100)
        {//ֻ��δ��¼��վʱ���ж��Ƿ�ע�ᵽ4G����¼����֮���ж�
          x=Comm_Ram->CSPN;
          if(x==3)//�����г��ĵ������������п���ֻ֧��4G����
          {
            i=Comm_Ram->RemoteWirelessModuleSYSINFO;
            switch(i)
            {
              case 4:
                break;
              default:
                if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
                {
                  break;						
                }
                else
                {//�ȴ�������ȷע���������ͣ�2G/3G/4G
                  UART1Ctrl->LinkSubTask=0;//����AT���ͼ���											
                  goto ReTxAT;
                }
            }
          }
        }
#endif

      }
			GPRS_3763_NextATCount();//��1��AT
			break;
		case AT_MYNETACT://������������
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=6;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
			#if (USER/100)==13//������
				WarningOccur(GPRSPORT,(u8 *)"������������");//�澯����,��ڸ澯����͸澯�ַ���
			#endif
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//�޸��Ѷ�ָ��
				GPRS_3763_NextATCount();//��1��AT
			}
			else
			{
				if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//�ȴ������������Ӷ�ʱ
				{
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
					//UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
					GPRS_3763_NextATCount();//��1��AT;���ػ����ܴ������ն����޸�
				}
				return;
			}
			break;
		case AT_MYIPFILTER://IP���ʿ�������
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
			#if (USER/100)==13//������
				WarningOccur(GPRSPORT,(u8 *)"IP���ʿ�������");//�澯����,��ڸ澯����͸澯�ַ���
			#endif
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//�޸��Ѷ�ָ��
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask++;//������
				break;
			}
			goto ReTxAT;//�ط���AT
		case AT_MYNETOPEN://��������
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=7;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				WarningCancel(GPRSPORT);//�澯ȡ��,��ڸ澯����
				UART1Ctrl->LinkTask=100;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
				UART1Ctrl->ATCount=AT_CSQ_LINK;//���ź����Ӻ�
		#if MainProtocol==376
				i=MRR(ADDR_AFN04F1+5,1);
				if(i==0x0)
				{
					i=1;//��С1��
				}
				i*=60;
				i+=60;//��60����ܵ���վ������ʱ���
		#else
				//DL698.45
				i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(i)
				{
					i=MRR(i+1,2);;//��������(��)  long-unsigned
					i=(i>>8)+((i&0xff)<<8);
					if(i==0)
					{
						i=60;//��С1��
					}
				}
				else
				{
					i=60;//��С1��
				}
		#endif
				if((UART1Ctrl->TCPUDP&0x30)==0x20)//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
				{//�Ƿ�����ģʽ
					if((UART1Ctrl->TCPUDP&0x80)!=0)
					{//��UDP
						i=6000;//UDP��ʼ�ȴ���վ���붨ʱ60��
					}
					Terminal_Ram->GPRSHeartBeat_S_Timer=i;//GPRS�����붨ʱ��
				}
			#if (USER/100)==13//������
				WarningOccur(GPRSPORT,(u8 *)"Զ�����߿�������");//�澯����,��ڸ澯����͸澯�ַ���
			#endif
			}
			else
			{
				WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
				GPRS_3763_NextATCount();//��1��AT
			}
			break;
			
		case AT_CSQ_NOLINK2://���ź�û����2	
		case AT_CSQ_LINK://���ź����Ӻ�
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			if(p32[4*AT_No]!=AT_CSQ_LINK)//AT����
			{
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			}
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			if(p32[4*AT_No]!=AT_CSQ_LINK)//AT����
			{
				LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			}
		#endif
			
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"+CSQ: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				LineATIntRx(Addr,ADDR_DATABUFF,2);//ȡAT�жϽ����������Է��ڻ�����
				GPRS_Signal(ADDR_DATABUFF);//GPRS�ź�ǿ��0-4
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_3763_NextATCount();//��1��AT
				break;
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			goto ReTxAT;//�ط���AT
		case AT_MYNETOPEN_LINK://��ѯ��������
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			//LOG_ATRx();//GPRSģ��AT��������LOG;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			//LOGRS232_ATRx();//GPRSģ��AT��������LOG;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: 0"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
		#if MainProtocol==376
				MC(0,ADDR_AFN0CF4+1,16+6);
				//LocalIP     
				p16fifo[1]+=1;   //ģ�鷵��֡�в���,�д�"�źͲ���"�ŵ�,���ݴ���		
				Addr+=1;
				x=ATRxSearch((u32)((u8*)"\""));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if((x-1)==Addr)
				{
					Addr = x;
				}
				x=ATRxSearch((u32)((u8*)"\""));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ				
				y=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if((x<y)||(x==0))
				{
					x = y-1;
				}
				if(x>Addr)
				{
					x-=Addr;
					x-=1;
					if(x<16)
					{
						MR(ADDR_AFN0CF4+1,Addr,x);
						//��ʾ����IP
						CopyString((u8 *)"��IP ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						MR(ADDR_DATABUFF+5,Addr,x);
						MC(0,ADDR_DATABUFF+5+x,1);
						WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
						//local_port
						Addr+=x;
						Addr+=2;
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						x=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
						if(x>Addr)
						{
							x-=Addr;
							x-=1;
							if(x<6)
							{
								MR(ADDR_AFN0CF4+1+16,Addr,x);
							}
						}
						
						CopyString((u8 *)"0.0.0.0",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						i=DataComp(ADDR_AFN0CF4+1,ADDR_DATABUFF,7);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
						if(i==0)
						{//�Ͽ�����
							Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							if(Addr!=0)
							{
								p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
							}
							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
							UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
							return;
						}
					}
				}
		#else
				//DL698
				MC(0,ADDR_MobilePhoneNumber+40,16+6);
				//LocalIP			
				p16fifo[1]+=1;   //ģ�鷵��֡�в���,�д�"�źͲ���"�ŵ�,���ݴ���		
				Addr+=1;
				x=ATRxSearch((u32)((u8*)"\""));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if((x-1)==Addr)
				{
					Addr = x;
				}
				x=ATRxSearch((u32)((u8*)"\""));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ				
				y=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if((x<y)||(x==0))
				{
					x = y-1;
				}
				if(x>Addr)
				{
					x-=Addr;
					x-=1;
					if(x<16)
					{
						MR(ADDR_MobilePhoneNumber+40,Addr,x);
						//��ʾ����IP
						CopyString((u8 *)"��IP ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						MR(ADDR_DATABUFF+5,Addr,x);
						MC(0,ADDR_DATABUFF+5+x,1);
						WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
						//local_port
						Addr+=x;
						Addr+=2;
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						x=ATRxSearch((u32)((u8*)","));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
						if(x>Addr)
						{
							x-=Addr;
							x-=1;
							if(x<6)
							{
								MR(ADDR_MobilePhoneNumber+40+16,Addr,x);
							}
						}
						
						CopyString((u8 *)"0.0.0.0",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						i=DataComp(ADDR_MobilePhoneNumber+40,ADDR_DATABUFF,7);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
						if(i==0)
						{//�Ͽ�����
							Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							if(Addr!=0)
							{
								p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
							}
							WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
							UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
							return;
						}
					}
				}
		#endif
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				}
				GPRS_3763_NextATCount();//��1��AT
				break;
			}
			goto ReTxAT;//�ط���AT
		case AT_MYNETREAD://��ȡ����
			p1=p16fifo[1];//ԭp1
			Addr=ATRxSearch((u32)((u8*)"$MYNETREAD: 0,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				MC(0,ADDR_DATABUFF,64);
				i=GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
				if(i!=0)
				{
					RxByte=ASCII_HEX(ADDR_DATABUFF);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
					i=GetATRxByte();//�жϽ���,���㵱ǰp0��Ӧp1����δ�����ֽ���
					if(i<(RxByte+4))//���ֽ���Ϊ0ʱԭRxByte+6Ϊ����0x0d0x0a��OK0x0d0x0a,�ָ�Ϊȥ������OK0x0d0x0a,Ϊ���߼����RxByte+4
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//����û���
							p16fifo[1]=p1;//ԭp1
						}
						else
						{
							Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
							if(Addr!=0)
							{
								p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
							}
							UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
							GPRS_3763_NextATCount();//��1��AT

						}
						return;
					}
					UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
					if(RxByte)
					{
						i=MRR((u32)&UART1Ctrl->RxTxByte,4);
						i+=RxByte;
						MWR(i,(u32)&UART1Ctrl->RxTxByte,4);//ͨ������
					}
					
					x=p16fifo[1];
					while(x>=(LEN_UART1IntATRx-10))
					{
						x-=(LEN_UART1IntATRx-10);
					}
					
				#if LOG_1376_3==1//0=û,1=��
					if(RxByte!=0)
					{
						LOG(13763,0,GPRSPORT,(u8 *)ADDR_UART1IntATRx+10,x,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
					}
				#endif
				#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
					if(RxByte!=0)
					{
						LOGRS232(13763,0,GPRSPORT,(u8 *)ADDR_UART1IntATRx+10,x,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
					}
				#endif
					
					p8s=(u8*)(ADDR_UART1IntATRx+10);
					p16=(u16*)(ADDR_UART1IntRx);
					y=p16[0];
					if(y>=(LEN_UART1IntRx-10))
					{
						y=0;
					}
					p8d=(u8*)(ADDR_UART1IntRx+10);
				#if LOG_1376_3||DEBUG_GPRStoRS232
					p8LOG=(u8*)ADDR_DATABUFF;
				#endif
					for(i=0;i<RxByte;i++)
					{
						p8d[y]=p8s[x];
				#if (LOG_1376_3||DEBUG_GPRStoRS232)
						if(RxByte<=LEN_DATABUFF)
						{
							p8LOG[0]=p8s[x];
							p8LOG++;
						}
				#endif
						x++;
						if(x>=(LEN_UART1IntATRx-10))
						{
							x=0;
						}
						y++;
						if(y>=(LEN_UART1IntRx-10))
						{
							y=0;
						}
					}
					p16fifo[1]=x;//�޸��Ѷ�ָ��
					p16[0]=y;//����ָ��
				#if LOG_1376_3==1//0=û,1=��
					if(RxByte!=0)
					{
						//hex����
						LOG_LINK(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
						p16fifo[2]=x;//�޸��Ѷ�ָ��
						//acii
						LOG_ATRx(1);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
					}
				#endif
				#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
					if(RxByte!=0)
					{
						//hex����
						LOGRS232_LINK(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,RxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
						p16fifo[2]=x;//�޸��Ѷ�ָ��
						//acii
						LOGRS232_ATRx(1);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
					}
				#endif
					
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
			#if MainProtocol==698	
				Uart_698_RxTx(GPRSPORT);//UART_3761���շ���
			#else
				Uart_3761_RxTx(GPRSPORT);//UART_3761���շ���
			#endif
					if(RxByte==0)
					{
						GPRS_3763_NextATCount();//��1��AT
					}
					return;
				}
			}
			if(Terminal_Ram->AT_MS_Timer!=0)
			{//����û���
				p16fifo[1]=p1;//ԭp1
				return;
			}
			
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
//			if((UART1Ctrl->TCPUDP&0x30)!=0x20)//ԭ����ģʽ(ͬF8������)B7=0TCP=1UDP,B6=,B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ,B3-B0=����
//			{//���Ƿ�����ģʽ
//				goto ReTxAT;//�ط���AT
//			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			GPRS_3763_NextATCount();//��1��AT
			break;
		case AT_MYNETWRITE://��������
			switch(UART1Ctrl->ATWriteTask)//AT���ݷ�������:0=������,1=������
			{
				case 0://��ATͷ
					Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr==0)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//����û���
							return;
						}
					}
					UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
					y=p16fifo[1];//y=ԭp1
					
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						MC(0,ADDR_DATABUFF,64);
								
						p0=p16fifo[0];//p0
						p1=p16fifo[1];//p1
						p8s=(u8*)(ADDR_UART1IntATRx+10);
						p8d=(u8*)(ADDR_DATABUFF);
						if(p0>=p1)
						{
							RxByte=p0-p1;
						}
						else
						{
							RxByte=(p0+(LEN_UART1IntATRx-10))-p1;
						}
						//ȡ�ַ���
						i=0;
						while(RxByte!=0)
						{
							p8d[i]=p8s[p1];
							i++;
							if(i>=8)
							{//�ַ���̫����
								i=0;
							}
							p1++;
							if(p1>=(LEN_UART1IntATRx-10))
							{
								p1=0;
							}
							RxByte--;
						}
						p16fifo[1]=p1;
						
						//i=GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
						if(i==0)
						{//����0x0d0x0a���ַ�������
							if(Terminal_Ram->AT_MS_Timer!=0)
							{//����û���
								UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
								p16fifo[1]=y;//y=ԭp1
								return;
							}
						}
						else
						{
							i=ASCII_HEX(ADDR_DATABUFF);//10���Ƶ�ASCII��תΪHEX,��"1234"->0x04d2,���ASCII�뿪ʼ��ַ��0-9ASCII��Ϊ����
							if(i!=0x0)
							{
								x=Terminal_Ram->CopyTxByte;//���跢���ֽ���
								if(x>1460)
								{
									x=1460;
								}
								if((i>=x)||(Terminal_Ram->AT_MS_Timer==0))//i>=���跢���ֽ�������ճ�ʱ
								{
						#if LOG_1376_3==1//0=û,1=��
							p16fifo[1]=y;//y=ԭp1
							LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
							p16fifo[1]=p1;
						#endif
						#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
							p16fifo[1]=y;//y=ԭp1
							LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
							p16fifo[1]=p1;
						#endif
									Terminal_Ram->ReqTxByte=i;//�������ֽ���
									UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
									UART1Ctrl->ATWriteTask=1;//AT���ݷ�������:0=������,1=������
									Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//1��,GPRS�������ݵȴ���ʱ�붨ʱ��
									return;
								}
								else
								{
									UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
									p16fifo[1]=y;//y=ԭp1
									return;
								}
							}
						}
					}
					if(Terminal_Ram->GPRSWaitTx_S_Timer!=0)//2��,GPRS�������ݵȴ���ʱ�붨ʱ��
					{
						return;
					}
					else
					{
						if(UART1Ctrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
						{
							WarningOccur(GPRSPORT,(u8 *)"Զ�����ߵȴ����ͳ�ʱ");//�澯����,��ڸ澯����͸澯�ַ���
						}
						else
						{
							WarningOccur(GPRSPORT,(u8 *)"Զ�����ߵ�¼��վʧ��");//�澯����,��ڸ澯����͸澯�ַ���
						}
						UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
					}
					break;
				case 1://������
					Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr==0)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//����û���
							return;
						}
					}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(1);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(1);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
					UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
					Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr!=0)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
					UART1Ctrl->ATWriteTask=0;//AT���ݷ�������:0=������,1=������
					if(Terminal_Ram->CopyTxByte!=0x0)//���跢���ֽ���
					{
						UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
						UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
					}
					else
					{
						if(UART1Ctrl->Task==6)
						{//��֡�������ڷ���
						#if MainProtocol==376
							UART1Ctrl->Task=4;//4=��֡�������������
						#else
							UART1Ctrl->Task=0;//0=����
						#endif
						}
						else
						{
							UART1Ctrl->Task=0;//0=����
						}
						GPRS_3763_NextATCount();//��1��AT
					}
					break;
			}
			break;
		case AT_MYFTP://FTP�ļ�����
	#if Extern_GPRS_FTP_RxTx==0//�ⲿ����:0=I2,1=E1,2=E2,3=;W5200 GPRS�ļ�����
			GPRS_FTP_Rx();//GPRS_FTP_Rx����
	#else
			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
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
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			break;
		case AT_MYPOWEROFF://�ػ�
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				UART1Ctrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				Comm_Ram->UART1Timer=15000/10;//�ػ���ʱ15s
#if (USER/100)==13//������
				WarningOccur(GPRSPORT,(u8 *)"ģ�����ڹػ�");//�澯����,��ڸ澯����͸澯�ַ���
#endif
			}
			else
			{
				UART1Ctrl->LinkSubTask++;//����AT���ͼ���
				if(UART1Ctrl->LinkSubTask<=2)//����AT���ͼ���
				{
					UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
				}
				else
				{
					UART1Ctrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					Comm_Ram->UART1Timer=15000/10;//�ػ���ʱ15s
				}
			}
			break;
		default:
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
		#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
			LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		#endif
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				GPRS_3763_NextATCount();//��1��AT
			}
			else
			{
			ReTxAT://�ط���AT
				UART1Ctrl->LinkSubTask++;//����AT���ͼ���
				if(UART1Ctrl->LinkSubTask>=(NUM_ReATTx+1))//����AT���ͼ���
				{
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
				}
				return;
			}
			break;
	}
}


void GPRS_3763_AT_RxTx(void)//GPRS_3763ģ��AT���շ���
{
	u32 *p32;
	switch(UART1Ctrl->ATTask)//AT����:0=����,1=����,2=���ڷ���,3=����
	{
		case 1://����
			GPRS_3763_AT_Tx();//AT�����
			break;
		case 2://���ڷ���
			if(Wait_UARTn_Tx(GPRSPORT)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				p32=(u32*)(&GPRS_3763_AT_Command_Lib);
				switch(p32[4*UART1Ctrl->ATCount])//AT����
				{
					case AT_SMS_NOLINK://���ŷ���û����ǰ
					case AT_SMS_LINK://���ŷ�������
						if(UART1Ctrl->SubTask==1)//������
						{//��SMS������
							Terminal_Ram->AT_MS_Timer=30000/10;//GPRS AT������10MS��ʱ��
						}
						else
						{
							Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//GPRS AT������10MS��ʱ��
						}
						break;
					default:
						Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//GPRS AT������10MS��ʱ��
						break;
				}
				UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
				break;
			}
			break;
		case 3://����
			GPRS_3763_AT_Rx();//AT�������
			break;
		default:
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			break;
	}
}


void UnInit_GPRS_3763(void)//GPRSģ������Ϊ��������
{
#ifdef IC_STM32F4xx
	u32 i;
	i=
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11);//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Pin_Configure(PIN_ON_GPRS,i);//on/off
	Pin_Configure(PIN_RST_GPRS,i);//reset
	i=
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11);//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Pin_Configure(PIN_USART6_TX,i);//TXD
	Pin_Configure(PIN_USART6_RX,i);//RXD
	//RI
#endif
#ifdef IC_NUC9xx
	u32 i;
	i=
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11);//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Pin_Configure(PIN_ON_GPRS,i);//on/off
	Pin_Configure(PIN_RST_GPRS,i);//reset
	i=
					0|//B3-B0:���ù���ѡ��AF(0-15)
		 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
		 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
		 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
		 (2<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
		(0<<11);//B11  :��Ϊ���ʱ��0=���0,1=���1	
	Pin_Configure(PIN_UART_TXD6,i);//TXD
	Pin_Configure(PIN_UART_RXD6,i);//RXD
	//RI
#endif
}

void GPRS_3763_RxTx(void)//Զ������GPRSģ����շ���
{
	u32 i;
	u32 x;
#if MainProtocol==698
	u32 LEN_UARTnTx;
	u32 LEN_ADDR;
	u32 LEN_TAPDU;
	u32 LEN_TxAPDU;
	u32 LEN_Tx;
	u8* p8tx;
#endif
	
/*
//�Բ�����
	if(Comm_Ram->SelfTestCommand==0x55555555)
	{
		if(UART1Ctrl->LinkCount>2)
		{//��ip
			UART1Ctrl->LinkCount=0;//��̫������IP
			if(UART1Ctrl->LinkTask>=8)
			{//8=��ʼ��AT����
				UART1Ctrl->LinkTask=1;//���ܵ�Դ
			}
		}
	}
*/
//��ʼ��
	if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		if(UART1Ctrl->LinkTask>2)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
		{//����Դ��
			if(Pin_Read(PIN_ENR5V0)==0)//������,�������ʹ����;����0��1
			{//�ж��й��˵�Դ
				//UART1Ctrl->PORT=0;//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				UART1Ctrl->LinkTask=1;//1=�ص�Դ
				return;
			}
		}
	}
	switch(UART1Ctrl->LinkTask)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	{
		case 0://0=�ػ���ʱ
			Terminal_Ram->Signal=0;//GPRS�ź�ǿ��0-4
			Terminal_Ram->SignaldBm=0x99;//�ź�ǿ��0-31��99
			if(Comm_Ram->UART1Timer==0x0)
			{
				UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 1://1=���ܵ�Դ
			UnInit_GPRS_3763();//GPRSģ������Ϊ��������
		
//			i=MRR(ADDR_TERMINAL,1);//�ն˵�ַ�ĵ�1�ֽ���Ϊ�����
//			i&=0x0f;
//			i*=100;//0-30��������ʱ
//			if((Comm_Ram->Factory==0x55)||(Comm_Ram->POWER<3))
//			{//����ģʽ���ع���ʱ��¼���������ʱ���㹤���������Ժ�������¼
//				i=0;
//			}
			i=0;
		
			x=Comm_Ram->MYMDHMS[1]+(Comm_Ram->MYMDHMS[2]<<8);//ʱ��
			if(x<0x0030)
			{//��С��0ʱ30��,�ص�Դ
				OffR5V0();//��R5V0��Դ
				//�ر��ܵ�Դ��ʱ20��+(0-30��������ʱ)
				i+=20000/10;
			}
			else
			{//����0ʱ30��,���ص�Դ
			#if ((USER/100)==7)//�û���ʶ:�㽭�������ӳ������ݰ�  				
			  if(Pin_Read(PIN_PWR_DOWN)==0)//������,�������ʹ����;����0��1	
				{
					OffR5V0();//��R5V0��Դ
					i=10000/10;//10s
        }	
			#endif	
			}
			Comm_Ram->UART1Timer=i;
			UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			return;//����UART1Timerֱ�ӷ���
		case 2://���ܵ�Դ
			if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
			{
				//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				if(UART1Ctrl->PORT==0)
				{
					UART1Ctrl->PORT=1;
				}
				else
				{
					return;//��ع���ʱ����1�κ�ֹͣ
				}
			}
			GPRS_Voltage();//GPRS��Դ��ѹADC����
			if(Comm_Ram->UGPRS<0x0030)//BCD 2λС��
			{//<0.30V
				
//				i=MRR(ADDR_TERMINAL,1);//�ն˵�ַ�ĵ�1�ֽ���Ϊ�����
//				i&=0x0f;
//				i*=100;
				i=0;
				
				i+=10000/10;
				if(Comm_Ram->UART1Timer<=i)
				{//��С����ʱ10��+(0-30��������ʱ)
					Comm_Ram->UART1Timer=0x0;
				}
			}
			if(Comm_Ram->UART1Timer==0x0)
			{
				OnR5V0();//��R5V0��Դ
				Comm_Ram->UART1Timer=1000/10;//���ܵ�Դ��ʱ1��
				UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 3://��ģ��ON/OFF���=0
			if(Comm_Ram->UART1Timer==0x0)
			{
  			if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
  			{
					Comm_Ram->UART1Timer=1000/10;//��ʱ
					while(1)
					{
						GPRS_Voltage();//GPRS��Դ��ѹADC����
						x=Comm_Ram->UGPRS;
						if((x<0x0350)||(x>0x0450))//BCD 2λС��
						{//3.80V>��ѹ>4.20V(�͵��Ժʱ������,��3.50V��4.50V)
							if(Comm_Ram->UART1Timer==0)
							{
								Comm_Ram->ErrorIC|=1<<30;//B30=GPRSģ���Դ
								Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				MC(0,ADDR_UART1IntATRx,LEN_UART1IntATRx);
				UART1Ctrl->Task=0;//0=���ڽ���
				UART1Ctrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
				UART1Ctrl->RxByte=0;//�����ֽ���=0
				UART1Ctrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
				UART1Ctrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
				Terminal_Ram->GPRSHeartBeat_S_Timer=0;//���������붨ʱ��
				UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
				UART1Ctrl->ATCount=0;//AT����
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
				UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
				UART1Ctrl->NETOPENCount=0;//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
				UART1Ctrl->FTPErr=0;//FTP�����־:0=��,1=FTP�������޷���¼,2=�Ҳ���ָ���ļ�Ŀ¼
			#if MainProtocol==698
				UART1Ctrl->ReNewLink=0;//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
			#endif
				Terminal_Ram->GPRS_ONOFF_MS_Timer=1010/10;//15 GPRS ONOFF�ſ���,=1�����,0=����
				Pin_Configure(PIN_ON_GPRS,//ON���=0
								0|//B3-B0:���ù���ѡ��AF(0-15)
					 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1
				Pin_Configure(PIN_RST_GPRS,//RESET����,����
								0|//B3-B0:���ù���ѡ��AF(0-15)
					 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				Comm_Ram->UART1Timer=2000/10;//2S��ʱ�ȴ��ж������
				UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 4://PIN_ON_RST���1S��10%��ʱ
			if(Comm_Ram->UART1Timer==0x0)
			{
				Pin_OutH(PIN_ON_GPRS);//ON���=1
				Comm_Ram->UART1Timer=3000/10;//��ʱ��λ
				UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 5://��λ
			if(Comm_Ram->UART1Timer==0x0)
			{
				Pin_Configure(PIN_RST_GPRS,//RESET
								0|//B3-B0:���ù���ѡ��AF(0-15)
					 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				
				Comm_Ram->UART1Timer=300/10;//��λ��ʱ0.3��(>=200ms)
				UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 6://��λ���
			if(Comm_Ram->UART1Timer==0x0)
			{
				Pin_Configure(PIN_RST_GPRS,//RESET����,����
								0|//B3-B0:���ù���ѡ��AF(0-15)
					 (0<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
					 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
					 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
					 (1<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
					(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
				Comm_Ram->UART1Timer=10000/10;//��AT����ǰ��ʱ;GPRS�ϵ��10������ɳ�ʼ�����ܽ���AT����
				UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 7://��ʼ��UART1
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=1;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			if(Comm_Ram->UART1Timer==0x0)
			{
				UART1Ctrl->BpsCtrl=0x3+(9<<5);//��ʼ����57.6
				Init_UARTn(GPRSPORT);//��ʼ��UART��,���ͨ�ſ�����������
				UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 8://��ʼ��AT����
			break;
		case 100://����ok
		case 101://��¼ok
		case 102://���˳���¼
			break;
		case 103://�˳���¼
		case 104://�ػ�
			if(UART1Ctrl->ATTask==0)//AT����:0=����,1=����,2=���ڷ���,3=����
			{
				UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
				UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
				UART1Ctrl->LinkTask=5;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			break;
		default:
			UART1Ctrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			return;
	}

	
	if(UART1Ctrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}	
//AT���շ���
	if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
	{
		GPRS_3763_AT_RxTx();//GPRS_3763ģ��AT���շ���
		return;
	}
	
//376.1���շ���
	switch(UART1Ctrl->Task)
	{
		case 0://����
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			GPRS_3763_AT_RxTx();//GPRS_3763ģ��AT���շ���
			break;
		case 2://�ȴ���֡���ݷ���
		#if MainProtocol==376
		case 5://�ȴ���֡���ݷ���
			#if (USER/100)==11//����ϵ
				USER11xx_TxAddr(GPRSPORT);//����ϵ����֡�ն˵�ַ2�ֽڳ��ȱ�Ϊ4�ֽ�,�����¼���֡У���,�跢�͵�֡�ֽ���+2
			#endif
			Terminal_Ram->CopyTxByte=UART1Ctrl->TxByte;//�����ֽ���
			UART1Ctrl->TxCount=0;//�ѷ��ͼ���
			Terminal_Ram->CopyTxCount=0;//�ѷ��ͼ���
			Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//2��,GPRS�������ݵȴ���ʱ�붨ʱ��
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			UART1Ctrl->ATCount=AT_MYNETWRITE;//��������
			UART1Ctrl->ATWriteTask=0;//AT���ݷ�������:0=������,1=������
			UART1Ctrl->Task++;
		#endif
		#if MainProtocol==698
			LEN_UARTnTx=Get_LEN_UARTnTx(GPRSPORT);
			p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
			LEN_ADDR=p8tx[4];//ȡ��ַ�ֽ���
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[1]+(p8tx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
			LEN_TAPDU-=3+LEN_ADDR+2+2;
			if(LEN_TAPDU<=LENmax_TxAPDU)//��·���֡���ͣ�ÿ֡���APDU�ֽ���
			{
			#if LOG_698==1//0=û,1=��
				LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
				LOGRS232(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
				Terminal_Ram->CopyTxByte=UART1Ctrl->TxByte;//�����ֽ���
				UART1Ctrl->TxCount=0;//�ѷ��ͼ���
				Terminal_Ram->CopyTxCount=0;//�ѷ��ͼ���
				Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//2��,GPRS�������ݵȴ���ʱ�붨ʱ��
				UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
				UART1Ctrl->ATCount=AT_MYNETWRITE;//��������
				UART1Ctrl->ATWriteTask=0;//AT���ݷ�������:0=������,1=������
				UART1Ctrl->Task++;
			}
			else
			{
				p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MW((u32)p8tx,(u32)p8tx+LEN_UARTnTx,UART1Ctrl->TxByte);
				UART1Ctrl->TxAPDUSEQ=0;//��·���֡��ż���0-4095
				UART1Ctrl->TxAPDUByte=0;//��·���֡�ѷ���APDU�ֽ���
				UART1Ctrl->Task=5;//5=�ȴ���·���֡���ݷ���
			}
		#endif
			break;
	#if MainProtocol==698
		case 5://�ȴ���֡���ݷ���
			LEN_UARTnTx=Get_LEN_UARTnTx(GPRSPORT);
			p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
			LEN_ADDR=p8tx[4];//ȡ��ַ�ֽ���
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[LEN_UARTnTx+1]+(p8tx[LEN_UARTnTx+2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
			LEN_TAPDU-=3+LEN_ADDR+2+2;//������L�Ϳ�����C=3byte,֡ͷУ��=2byte,֡У��=2byte
			MR((u32)p8tx,(u32)p8tx+LEN_UARTnTx,4+LEN_ADDR+2);
			p8tx[3]|=(1<<5);//��֡��־λ
			LEN_Tx=4+LEN_ADDR+2;
			i=UART1Ctrl->TxAPDUByte;
			if(i>=LEN_TAPDU)
			{
				UART1Ctrl->RxByte=0;//rx����=0
				UART1Ctrl->Task=0;//0=����
				return;
			}
			LEN_TxAPDU=LEN_TAPDU-i;
		//��֡��ʽ����
		//bit0~bit11����ʾ��֡������̵�֡��ţ�ȡֵ��Χ0~4095��ѭ��ʹ�ã�
		//bit12~bit13��������
		//bit15=0��bit14=0����ʾ��֡����������ʼ֡��
		//bit15=1��bit14=0����ʾ��֡����ȷ��֡��ȷ��֡������APDUƬ���򣩣�
		//bit15=0��bit14=1����ʾ��֡�������֡��
		//bit15=1��bit14=1����ʾ��֡�����м�֡��
			x=UART1Ctrl->TxAPDUSEQ;//��·���֡��ż���0-4095
			UART1Ctrl->TxAPDUSEQ++;//��·���֡��ż���0-4095
			x&=0x7ff;
			if(UART1Ctrl->TxAPDUByte==0)
			{//��1֡
				LEN_TxAPDU=LENmax_TxAPDU;
			}
			else
			{
				if(LEN_TxAPDU>LENmax_TxAPDU)//��·���֡���ͣ�ÿ֡���APDU�ֽ���
				{//�м�֡
					x|=0xC000;
					LEN_TxAPDU=LENmax_TxAPDU;
				}
				else
				{//���֡
					x|=0x4000;
				}
			}
			p8tx[LEN_Tx]=x;
			p8tx[LEN_Tx+1]=x>>8;
			LEN_Tx+=2;
			MR((u32)p8tx+LEN_Tx,(u32)p8tx+LEN_UARTnTx+4+LEN_ADDR+2+UART1Ctrl->TxAPDUByte,LEN_TxAPDU);
			UART1Ctrl->TxAPDUByte+=LEN_TxAPDU;
			LEN_Tx+=LEN_TxAPDU;
			LEN_Tx+=2+1;
			UART1Ctrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
			Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
			#if LOG_68==1//0=û,1=��
			LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
			LOGRS232(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UART1Ctrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			
			Terminal_Ram->CopyTxByte=UART1Ctrl->TxByte;//�����ֽ���
			UART1Ctrl->TxCount=0;//�ѷ��ͼ���
			Terminal_Ram->CopyTxCount=0;//�ѷ��ͼ���
			Terminal_Ram->GPRSWaitTx_S_Timer=1*60;//2��,GPRS�������ݵȴ���ʱ�붨ʱ��
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			UART1Ctrl->ATCount=AT_MYNETWRITE;//��������
			UART1Ctrl->ATWriteTask=0;//AT���ݷ�������:0=������,1=������
			UART1Ctrl->Task++;
			break;
	#endif
			
		case 3://��֡�������ڷ���
		case 6://��֡�������ڷ���
			if(UART1Ctrl->ATTask==0)//AT����:0=����,1=����,2=���ڷ���,3=����
			{//����ԭ���ͻ������RAM����ȵ�
				WarningOccur(GPRSPORT,(u8 *)"GPRS������BUG");//�澯����,��ڸ澯����͸澯�ַ���
				UART1Ctrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				//�ز������ʱ����Ϊ0�Ա���������
				Terminal_Ram->GPRSReConnection_S_Timer=0;//7 GPRS�������ߡ�ʱ������ģʽ�ز�����붨ʱ��
			}
			break;
		case 4://��֡�������������
			#if MainProtocol==698
				Uart_698_RxTx(GPRSPORT);//UART_3761���շ���
			#else
				Uart_3761_RxTx(GPRSPORT);//UART_3761���շ���
			#endif
			break;
		default:
			#if MainProtocol==698
				Uart_698_RxTx(GPRSPORT);//UART_3761���շ���
			#else
				Uart_3761_RxTx(GPRSPORT);//UART_3761���շ���
			#endif
			break;
	}

}












