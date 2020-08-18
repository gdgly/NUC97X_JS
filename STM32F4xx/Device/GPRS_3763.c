
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
#include "../MS/MS.h"
#if (USER/100)==14
#include "../DL698/DL698_FJ_TASKM_USER14xx.h"
#endif

#define DEBUG_GPRS    0////0=����,1=����

#define F_DUPFD		0	/* Duplicate file descriptor.  */
#define F_GETFD		1	/* Get file descriptor flags.  */
#define F_SETFD		2	/* Set file descriptor flags.  */
#define F_GETFL		3	/* Get file status flags.  */
#define F_SETFL		4	/* Set file status flags.  */

#define EINTR       4     /* Interrupted system call*/
#define EAGAIN      11    /* Try again*/
#define EWOULDBLOCK EAGAIN/* Operation would block */
#define	ENOTSOCK    88    /* Socketoperation on non-socket */
#define	EADDRINUSE  98    /* Address already in use */
#define ECONNRESET  104    /*Connection reset by peer */
#define EISCONN     106   /*Transport endpoint is already connected*/
#define ETIMEDOUT   110   /*Connection timed out*/            // ���ӳ�ʱ
#define EALREADY    114   /*Operation already in progress*/   // �������ڽ�����
#define	EINPROGRESS	115	  /* Operation now in progress */

#define SOL_SOCKET	1
#define SO_REUSEADDR	2
#define SO_BINDTODEVICE	25

#define NUM_ReATTx     1//AT�ط�����(ʵ�ʷ��ʹ���=�ط�����+1)

//SocketID 0-5ʹ��Լ��
//0 ������ģʽ�£��ͻ������ܵ�����socket,
//1 ������ģʽ�£��ͻ������ܵ�����socket
//2 �����ͻ�������
//3 ������
//4 FTP����
//5 FTP����
//3-5 δ��
//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket
#define TCP_0_OPEN 0x80  //TCP clinet  socket_open
#define TCP_0 0x40       //TCP clinet link
#define TCP_1_LISTEN_OPEN 0x20  //TCP server  socket_open
#define TCP_1 0x10       //TCP server link
#define TCP_SELECT_SERVER 0x08  //SELECT TCP_clinet or TCP_server
#define FORCE_TCP_1 0x04 //FORCE_TCP_server
#define TCP_1_SOCKET_MASK 0x3 //TCP server  socket mask

#define TCP_0_SOCKET 0x3 //TCP clinet  socket
#define TCP_1_LISTEN 2   //TCP server  listen
#if (PPP_ENABLE==1)
void PPP_RxTx(void);
void Myprint_ATRx(void);
void log_out(u32  ADDR,u32 Byte, char x);
u32 Search_String(u32 ADDR_String,u32 ADDR_T,u32 Len);
u32 Check_pppd_exist(void);//���ڷ���1�������ڷ���0
#endif

#define GPRSNETTOTALCOUNT 3
#define GPRSNETMAINIPCOUNT 1
#define GPRSNETOPENCOUNT 3
#define GPRSAUTOTODISSCON_S_Timer 120//23 GPRS��������ģʽ������ͨ���Զ������붨ʱ��,������ͨѶ�ر�socket��ʱ
#define GPRSWait_SRVST_Timer 240    //�ȴ�ģ���ϱ�^SRVST: 2
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

//�����ϱ�
#if (USER/100)==17//����ϵ ���������ϱ�
ADDR_ATStr_MYNETURC
	dcb "AT$MYNETURC=1\r",0
#else //�������ϱ�
ADDR_ATStr_MYNETURC
	dcb "AT$MYNETURC=0\r",0
#endif
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
#if PPP_ENABLE==1
ADDR_ATStr_MYNETCON_APN
	dcb "AT$MYNETCON=1,",'"',"APN",'"',',','"',0
ADDR_ERRStr_MYNETCON_APN
	dcb "����APNʧ��",0

//����USERPWD:�û���������
ADDR_ATStr_MYNETCON_USERPWD
	dcb "AT$MYNETCON=1,",'"',"USERPWD",'"',',','"',0
ADDR_ERRStr_MYNETCON_USERPWD
	dcb "����USERPWDʧ��",0
#else
ADDR_ATStr_MYNETCON_APN
	dcb "AT$MYNETCON=0,",'"',"APN",'"',',','"',0
ADDR_ERRStr_MYNETCON_APN
	dcb "����APNʧ��",0

//����USERPWD:�û���������
ADDR_ATStr_MYNETCON_USERPWD
	dcb "AT$MYNETCON=0,",'"',"USERPWD",'"',',','"',0
ADDR_ERRStr_MYNETCON_USERPWD
	dcb "����USERPWDʧ��",0
#endif


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
	dcb "AT$MYNETOPEN=3\r",0
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
	dcb "AT$MYNETWRITE=3,",0
ADDR_ERRStr_MYNETWRITE
	dcb "Զ�����߷�������ʧ��",0
//���ŷ���
	

//HOLD��ͣ
ADDR_ATStr_HOLD
	dcb 0
//�ر�����
ADDR_ATStr_MYNETCLOSE
	dcb "AT$MYNETCLOSE=3\r",0
ADDR_ERRStr_MYNETCLOSE
	dcb "Զ�����߹ر�����ʧ��",0
//�ػ�
ADDR_ATStr_MYPOWEROFF
	dcb "AT$MYPOWEROFF\r",0
ADDR_ERRStr_MYPOWEROFF
	dcb "Զ������ģ��ػ�ʧ��",0
	
//�ر���������
ADDR_ATStr_MYNETACTOFF
	dcb "AT$MYNETACT=0,0\r",0
ADDR_ERRStr_MYNETACTOFF
	dcb "�ر���������ʧ��",0
	
ADDR_ATStr_CGDCONT
	dcb "AT+CGDCONT=1,",'"',"IP",'"',',','"',0
ADDR_ERRStr_CGDCONT
	dcb "����isp��������ʧ��",0
	
ADDR_ATStr_ATDT
	dcb "ATDT*",0
ADDR_ERRStr_ATDT
	dcb "���ý������ʧ��",0
	
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
	dcd 2000//AT����ص�1���ַ���ʱ(ms)

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
	#if PPP_ENABLE==1
	dcd 4000//AT����ص�1���ַ���ʱ(ms)
	#else
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
	#endif

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
	#if PPP_ENABLE==1
	dcd 4000//AT����ص�1���ַ���ʱ(ms)
	#else
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
	#endif
//ATѭ��,û����
	dcd AT_LOOP_NOLINK
	dcd 0//AT�����ַ�����ַ
	dcd 0//������ʾ�ַ�����ַ
	dcd 0//AT����ص�1���ַ���ʱ(ms)


//���ź����Ӻ�
	dcd AT_CSQ_LINK
	dcd ADDR_ATStr_CSQ_NOLINK//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CSQ_NOLINK//������ʾ�ַ�����ַ
	#if PPP_ENABLE==1
	dcd 4000//AT����ص�1���ַ���ʱ(ms)
	#else
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
	#endif
//��ѯע���������ͣ�2G/3G/4G
	dcd AT_SYSINFO_LINK
	dcd ADDR_ATStr_SYSINFO//AT�����ַ�����ַ
	dcd ADDR_ERRStr_SYSINFO//������ʾ�ַ�����ַ
	#if PPP_ENABLE==1
	dcd 4000//AT����ص�1���ַ���ʱ(ms)
	#else
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
	#endif
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
	dcd 6000//AT����ص�1���ַ���ʱ(ms)
//�ػ�
	dcd AT_MYPOWEROFF
	dcd ADDR_ATStr_MYPOWEROFF//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYPOWEROFF//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
	
	//�ر���������
	dcd AT_MYNETACTOFF
	dcd ADDR_ATStr_MYNETACTOFF//AT�����ַ�����ַ
	dcd ADDR_ERRStr_MYNETACTOFF//������ʾ�ַ�����ַ
	dcd 1000//AT����ص�1���ַ���ʱ(ms)
  //���ܿͻ�������
	dcd AT_ACCEPT
	dcd 0//AT�����ַ�����ַ
	dcd 0//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)
	dcd AT_CGDCONT
	dcd ADDR_ATStr_CGDCONT//AT�����ַ�����ַ
	dcd ADDR_ERRStr_CGDCONT//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)
	dcd AT_ATDT
	dcd ADDR_ATStr_ATDT//AT�����ַ�����ַ
	dcd ADDR_ERRStr_ATDT//������ʾ�ַ�����ַ
	dcd 2000//AT����ص�1���ַ���ʱ(ms)
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
#if (USER/100)==17
//�������Byte�ֽڵĻ��棬������ǰ�ֽ�
//��ַ�������ֽ���
void LineClearCache(u32 ADDR,u32 Byte,u32 Direction)//ȡAT�жϽ����������Է��ڻ�����
{
	u8 *p8;
	p8=(u8 *)ADDR;
	while(Byte--)
	{
		p8[0]=' ';
		if(Direction)//����ɾ��
		{
			if(((u32)p8)>=(ADDR_UART1IntATRx+LEN_UART1IntATRx))
			{
				p8=(u8*)(ADDR_UART1IntATRx+10);
			}
			else
			{
				p8++;
			}
		}
		else//����ɾ��
		{
			if(((u32)p8)<=(ADDR_UART1IntATRx+10))
			{
				p8=(u8*)(ADDR_UART1IntATRx+LEN_UART1IntATRx);
			}
			else
			{
				p8--;
			}
		}
	}
	
	
}
u32 GPRS_URC_Rx(void)//�����ϱ�����
{
  u32 i;
	u32 x;
//	u32 y;
	u32 AT_No;
//	u32 RxByte;
	u32 Addr;
//	u8 *p8s;
//	u8 *p8d;
//	u16 *p16;
	u16 *p16fifo;
	u32 *p32;
	u32 rcode;
#if PPP_ENABLE==1
	ms_Type * ms;
	ms=Get_ms();
#endif
	rcode=1;
	AT_No=UART1Ctrl->ATCount;//AT����
	p16fifo=(u16*)(ADDR_UART1IntATRx);
#if PPP_ENABLE==1
#if((DEBUG==1)&&(DEBUG_GPRS==1))
    Addr=ATRxSearch((u32)((u8*)"\r\n"));
		if(Addr)
		{
			Myprint_ATRx();
		}
#endif
#endif
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	switch(p32[4*AT_No])//AT����
	{
		case AT_MYPOWEROFF://�ػ����
		rcode=0;//�ڴ˲��費���κμ��
		break;
		case AT_AT://�رջ���
		  switch(UART1Ctrl->SubTask)
			{
			  case 10://Ϊ0ʱ�ȴ�ģ���ϱ�
					MC(0,ADDR_DATABUFF,20);
				  CopyString((u8 *)"�ȴ��ϱ�SRVST:   ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					x=Terminal_Ram->GPRSWaitTx_S_Timer;
//					i+=(i/10)*6;//һ�ֽ�DECתBCD
				  i=x/100;
					BCD_ASCII(i,ADDR_DATABUFF+14);
				  x%=100;
				  i=x/10;
				  BCD_ASCII(i,ADDR_DATABUFF+15);
				  i=x%10;
				  BCD_ASCII(i,ADDR_DATABUFF+16);
					WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				  Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(!Addr)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//����û���
							return rcode;
						}
					}
					if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
					{
//						MR(ADDR_DATABUFF,ADDR_4500_5+38,8);//�ж��Ƿ��ǹ�תһ��ģ�飬��������������������JS18GZYT
//						i=Compare_DL698DataString((u8*)ADDR_DATABUFF,(u8*)"JS18GZYT",8);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//						if(!i)
//						{//==0,
//							UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
//							UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
//							rcode=0;
//						}
//						else
						{
							UART1Ctrl->SubTask=11;
						}
						break;
					}
					
					Addr=ATRxSearch((u32)((u8*)"^SRVST: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr)
					{
					#if LOG_1376_3==1//0=û,1=��
			       LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		      #endif
					#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
							LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
					#endif
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						i = ASCII_BCD(Addr,1);
						if((i==2)&&(MRR(Addr+1,1)=='\r'))
						{//�Ѿ�ע�ᵽ����
							UART1Ctrl->SubTask=11;
						}
						LineClearCache(Addr,9,0);//��͵�ַ��9���ֽڣ���ֹ�ظ�ʶ��
					}
//ʵ���õ�������̫С��ע�͵�
//					Addr=ATRxSearch((u32)((u8*)"+CGREG: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//					if(Addr)
//					{
//					  #if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
//							LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
//					  #endif
//						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
//						i = ASCII_BCD(Addr,1);
//						if(i==2)//2	δע�ᣬ�� MT ������ѰҪע����µ���Ӫ�� 
//						{
//							Terminal_Ram->GPRSWaitTx_S_Timer=90;//�ȴ�ģ���ϱ�^SRVST: 2
//						}
//					}
					Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr)
					{
						p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
				  break;
				case 11://Ϊ1ʱ�ȴ��Ѿ�����������ȥ����ATEO
				  UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
					UART1Ctrl->SubTask=12;
				  break;
				case 12://�Ѿ�ȥ������ATEO��ȥ�������
				default:
					rcode=0;
				  break;
			}
			break;
			default:
			Addr=ATRxSearch((u32)((u8*)"^"));//��ѯ�Ƿ����ԡ�^��Ϊ��ʼ���ϱ�
			if(Addr)
			{
			  x=p16fifo[1];
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)"\r\n"));
				if(!Addr)
				{//û���ϱ���
				  p16fifo[1]=x;
					if(Terminal_Ram->AT_MS_Timer==0)
					{//���ճ�ʱ���ط�����AT����
						Addr=ATRxSearch((u32)((u8*)"^"));//��ѯ�Ƿ����ԡ�^��Ϊ��ʼ���ϱ�
						LineClearCache(Addr,2,0);//��͵�ַ��9���ֽڣ���ֹ�ظ�ʶ��
						UART1Ctrl->Task=0;
					}
				  break;
				}
				#if LOG_1376_3==1//0=û,1=��
			  LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		    #endif
				#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
					LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
				#endif
				Addr=ATRxSearch((u32)((u8*)"SRVST: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr)
				{
					i = ASCII_BCD(Addr,1);
					if((i!=2)||((i==2)&&(MRR(Addr+1,1)!=0x0D)))
					{//û��ע�ᵽ������Ҫ�ȴ�ע��
					  UART1Ctrl->LinkTask=8;
						Terminal_Ram->GPRSWaitTx_S_Timer=GPRSWait_SRVST_Timer;
						MC(0,ADDR_UART1IntATRx,LEN_UART1IntATRx);
						UART1Ctrl->Task=0;//0=���ڽ���
						UART1Ctrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
						UART1Ctrl->RxByte=0;//�����ֽ���=0
						UART1Ctrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
						UART1Ctrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
						Terminal_Ram->GPRSHeartBeat_S_Timer=0;//���������붨ʱ��
						UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
						UART1Ctrl->ATCount=AT_AT;//AT����
						UART1Ctrl->SubTask=10;//������
						UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
						UART1Ctrl->LinkTask2=0;//ͨ��2��������(FTP����)
						UART1Ctrl->NETOPENCount=0;//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
						UART1Ctrl->res3=0;//45200200���Ѿ���ͼ���ò����ĸ���
				    UART1Ctrl->res4=0;//�ͻ��˺ͷ�����ѡ�����·
				    UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//Զ�˿ͻ���Ϊ����ʱΪbit1-bit0Ϊ0x3,ʵ��socketֻ��Ϊ0����1
						Terminal_Ram->CopyTxByte=0;
						LineClearCache(Addr,9,0);//��͵�ַ��9���ֽڣ�����Ѿ��ϱ���SRVST: ��ֹ�ظ�ʶ��
#if PPP_ENABLE==1
						if(UART1Ctrl->TCPSocket>2)
						{
							ms->link_close(UART1Ctrl->TCPSocket);
							UART1Ctrl->TCPSocket=0;
						}
						if(Svr_Skt->ppp0_clint>2)
						{
							ms->link_close(Svr_Skt->ppp0_clint);
							Svr_Skt->ppp0_clint=0;
						}
						Svr_Skt->ppp0_clint=0;
						//������ģ�飬����ֻ�ٴν�������socket
						if(Svr_Skt->ppp0_listen>2)
						{
							ms->link_close(Svr_Skt->ppp0_listen);
							Svr_Skt->ppp0_listen=0;
						}
#endif

						break;
					}
				}
				p16fifo[1]=x;
			}

			Addr=ATRxSearch((u32)((u8*)"+"));//��ѯ�Ƿ����ԡ�+��Ϊ��ʼ���ϱ�
			if(Addr)
			{
			  x=p16fifo[1];
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)"\r\n"));
				if(!Addr)
				{//û���ϱ���
				  p16fifo[1]=x;
					if(Terminal_Ram->AT_MS_Timer==0)
					{//���ճ�ʱ���ط�����AT����
						Addr=ATRxSearch((u32)((u8*)"+"));//��ѯ�Ƿ����ԡ�^��Ϊ��ʼ���ϱ�
						LineClearCache(Addr,2,0);//��͵�ַ��9���ֽڣ���ֹ�ظ�ʶ��
					}
				  break;
				}
//û������ȥ��
//				Addr=ATRxSearch((u32)((u8*)"CGREG: "));
//				if(Addr)
//				{
//					#if LOG_1376_3==1//0=û,1=��
//			    LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
//		      #endif
//				  #if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
//							LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
//					#endif
//					i = ASCII_BCD(Addr,1);
//					if((i!=1)&&(i!=5))//1	ע���˱�������,5	ע������������ 
//					{//û��ע�ᵽ������Ҫ�ȴ�ע��
//						UART1Ctrl->ATCount=AT_ATE0;
//						UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
//						UART1Ctrl->SubTask=0;
//						break;
//					}
//				}
				p16fifo[1]=x;
			}

//#warning "test_end"
//      switch(p32[4*AT_No])
//			{
//			 case AT_ACCEPT://��дOK;
//			   p16fifo[0]=6;
//			   p16fifo[1]=0;
//				 p8s=(u8*)(ADDR_UART1IntATRx+10);
//				 x=CopyString((u8 *)"OK\r\n",p8s);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
//			   break;
//			 case AT_MYNETOPEN_LINK://��дOK;
//				 i=UART1Ctrl->res4;
//			 if(!(i&TCP_0_OPEN))
//			 {
//				 if(((i&TCP_1_SOCKET_MASK)==TCP_1_SOCKET_MASK))
//				 {//���socketΪ�գ�
//					 p16fifo[0]=42;
//					 p16fifo[1]=0;
//					 p8s=(u8*)(ADDR_UART1IntATRx+10);
//					 x=CopyString((u8 *)"$MYURCCLIENT: 0,\"100.192.1.52\",58575\r\n",p8s);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
//				 }
//			 }
//				 
//			   break;
//			 default:
//			   break;
//			}
//#warning "test_end"		
			Addr=ATRxSearch((u32)((u8*)"$"));//��ѯ�Ƿ����ԡ�$��Ϊ��ʼ���ϱ�
			if(Addr)
			{
			  x=p16fifo[1];
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				Addr=ATRxSearch((u32)((u8*)"\r\n"));
				if(!Addr)
				{//û���ϱ���
				  p16fifo[1]=x;
					if(Terminal_Ram->AT_MS_Timer==0)
					{//���ճ�ʱ���ط�����AT����
						Addr=ATRxSearch((u32)((u8*)"$"));//��ѯ�Ƿ����ԡ�^��Ϊ��ʼ���ϱ�
						LineClearCache(Addr,2,0);//��͵�ַ��9���ֽڣ���ֹ�ظ�ʶ��
					}
				  break;
				}
				Addr=ATRxSearch((u32)((u8*)"MYURCCLIENT: "));//$MYURCCLIENT: 1,��172.16.23.100��,31256
				if(Addr)
				{
					#if LOG_1376_3==1//0=û,1=��
			     LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		      #endif
				  #if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
						LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
					#endif
					i = ASCII_BCD(Addr,1);//TCP server socket
					if(i<3)
					{//�ϱ����Ƿ���Ҫ���socket
						
					  UART1Ctrl->res4&=~TCP_1_SOCKET_MASK;
					  UART1Ctrl->res4|=i;
						UART1Ctrl->ATTask=1;
						UART1Ctrl->ATCount=AT_ACCEPT;
						UART1Ctrl->LinkSubTask=0;
						UART1Ctrl->SubTask=0;
						rcode=1;//������ص���1�򷵻�
					}
					else
					{//���򲻽��ܣ��Թ�
					  rcode=0;
					}
					Addr=ATRxSearch((u32)((u8*)"\r\n"));
					if(Addr)
					{
					  p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
					break;
				}
				Addr=ATRxSearch((u32)((u8*)"MYURCCLOSE: "));//$MYURCCLIENT: 1,��172.16.23.100��,31256
				if(Addr)
				{
					i = ASCII_BCD(Addr,1);//�ϱ��ĹرյĶ˿�
					switch(i)
					{
						case TCP_0_SOCKET://TCP clinet  socket
							UART1Ctrl->res4&=~TCP_0_OPEN;//TCP clinet  socket_closeͨ������
				      UART1Ctrl->res4&=~TCP_0;//TCP clinet linkͨ������
							break;
						case TCP_1_LISTEN://TCP server  listen
							UART1Ctrl->res4|=TCP_1_LISTEN_OPEN;
							break;
						case 0://TCP server  socket
						case 1://TCP server  socket
							UART1Ctrl->res4&=~TCP_1;//TCPserverͨ���Ͽ�
						  UART1Ctrl->res4|=TCP_1_SOCKET_MASK;
							break;
						default :
							break;
					}
					Addr=ATRxSearch((u32)((u8*)"\r\n"));
					if(Addr)
					{
					  p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
					break;
				}

//				Addr=ATRxSearch((u32)((u8*)"MYURCACT: "));//$MYURCCLIENT: 1,��172.16.23.100��,31256
//				if(Addr)
//				{
//					Addr=ATRxSearch((u32)((u8*)"\r\n"));
//				}
//				Addr=ATRxSearch((u32)((u8*)"MYURCSRVPORT: "));//$MYURCCLIENT: 1,��172.16.23.100��,31256
//				if(Addr)
//				{
//					Addr=ATRxSearch((u32)((u8*)"\r\n"));
//				}
				
				p16fifo[1]=x;
			}
			rcode=0;
			break;
	}
	return rcode;
}
void DISPLAY_WarningOccur(u32 AT_No)
{
	u32 i;
	switch(AT_No)
	{
		case AT_AT:
			WarningOccur(GPRSPORT,(u8 *)"AT������ͬ��");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_MYTYPE:
			WarningOccur(GPRSPORT,(u8 *)"��ѯԶ��ͨ��ģ������");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_MYGMR:
			WarningOccur(GPRSPORT,(u8 *)"��ȡģ������汾");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_CPIN:
			WarningOccur(GPRSPORT,(u8 *)"�����");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_MYNETURC:
			WarningOccur(GPRSPORT,(u8 *)"���������ϱ�");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_MYCCID:
			WarningOccur(GPRSPORT,(u8 *)"��ȡSIM�����к�ICCID");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_CIMI:
			WarningOccur(GPRSPORT,(u8 *)"������ƶ�̨��ʶ����IMSI");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_MYNETCON_APN:
		case AT_MYNETCON_USERPWD:
			MC(0,ADDR_DATABUFF,20);
		  if(AT_No==AT_MYNETCON_APN)
			{
				CopyString((u8 *)"����    ��Ӫ�� GAPN",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			}
			else
			{
				CopyString((u8 *)"����    ��Ӫ�� G����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			}
		  i=Comm_Ram->CSPN;
		  switch(i)
			{
				case 1: //1=�й��ƶ�ͨ��china mobile
					CopyString((u8 *)"�ƶ�",(u8*)(ADDR_DATABUFF+4));//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 2: //2=�й���ͨͨѶchina unicom
					CopyString((u8 *)"��ͨ",(u8*)(ADDR_DATABUFF+4));//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 3: //3=�й�����CHINA TELECOM
					CopyString((u8 *)"����",(u8*)(ADDR_DATABUFF+4));//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;		
				case 54://?754: ע���Ͻ��յ���1.8GHz����ҵ������
					CopyString((u8 *)"ר��",(u8*)(ADDR_DATABUFF+4));//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
				default:
					break;
			}
		  i=Comm_Ram->RemoteWirelessModuleSYSINFO; //Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
      BCD_ASCII(i,ADDR_DATABUFF+14);
			WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_MYNETACT:
			WarningOccur(GPRSPORT,(u8 *)"������������");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_MYNETOPEN:
			WarningOccur(GPRSPORT,(u8 *)"����Զ�̷���");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_MYPOWEROFF:
			WarningOccur(GPRSPORT,(u8 *)"�ر�Զ��ģ�飬����");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_CGDCONT:
			WarningOccur(GPRSPORT,(u8 *)"����ISP��������");//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case AT_ATDT:
			WarningOccur(GPRSPORT,(u8 *)"����ATDT����");//�澯����,��ڸ澯����͸澯�ַ���
			break;
	}
}
#endif //#if (USER/100)==17



void GPRS_3763_AT_Tx(void)//AT�����
{
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 AT_No;
	u32 TxByte;
	u8 *p8;
#if (PPP_ENABLE==1)//ʹ��PPP���Ž�������
#else
	u16 *p16;
#endif
	u32 *p32;
	u8 *p8rx;
	u8 *p8tx;
	
	u32 FTPFileCount;//FTP�ļ����ظ�������
#ifdef LINUX
	ms_Type * ms;
	ms=Get_ms();
	ms->linux_uart_receive(GPRSPORT);
#endif
	n=n;
	p8tx=(u8*)(ADDR_ATTx);
	TxByte=0;
	AT_No=UART1Ctrl->ATCount;//AT����
	if(AT_No>AT_Max)
	{
		AT_No=0;
		UART1Ctrl->ATCount=0;//AT����
	}
	#if (USER/100)==17
	DMA_UARTn_RX_Pointer(GPRSPORT);//����DMA_UARTn����ָ��ֵ(p0)
	i=GPRS_URC_Rx();//�����ϱ����գ�Ĭ�Ϸ���1
	if(i)//������ص���1�򷵻�
	{
		return;
	}
	#endif
	p32=(u32*)(&GPRS_3763_AT_Command_Lib);
	
#if (USER/100)==17
  DISPLAY_WarningOccur(p32[4*AT_No]);
#endif
	switch(p32[4*AT_No])//AT����
	{
		case AT_AT://�����ʼ��
			TxByte=CopyString((u8 *)"AT\r",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			switch(UART1Ctrl->SubTask)//������
			{
				case 0://����57600
					UART1Ctrl->BpsCtrl=0x3+(9<<5);
					break;
				case 1://����115200
					UART1Ctrl->BpsCtrl=0x3+(10<<5);
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
//#ifndef LINUX
		Init_UARTn(GPRSPORT);//��ʼ��UART��,���ͨ�ſ�����������
//#else
//		ms->linux_Init_UARTn(GPRSPORT);
//#endif
#if PPP_ENABLE==1
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			  log_out((u32)p8tx,TxByte, 'c');
#endif
#endif
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
#if (USER/100)==17
          case 54://54����ר��
						 n=i;
					break;
#endif
				}
				p8+=2;
				y=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
				for(i=0;i<y;i++)//�Թ��Ѿ����͵�APN����
				{
				  p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				}
				for(;i<x;i++)//��ʱi�Ѿ�=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
				{
					if(p8[3]==n)//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
					{
		#if COMPARE_2G_3G_4G==1 //����APNʱ�ȶ�2G��3G��4G
						y=p8[5];//2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 �� 
						y+=2;
						if(y==Comm_Ram->RemoteWirelessModuleSYSINFO)//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
		#else
						//����APNʱ���ȶ�2G��3G��4G
		#endif
						{
							break;
						}
					}
					#if (USER/100)==17
					if((p8[3]==0x95)&&(n==54))
					{//��������̨�ն���Э��ȷ��ǰ�͹ҵ��ֳ��ˣ�ר�����ͻ���0x95
						break;
					}
					#endif
					p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				}
				UART1Ctrl->res3=i;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
				
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
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
					return;
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
			#if (USER/100)==14
			p8=Get_Element((u8 *)ADDR_4500_2,6,0,LENmax_4500_2);//��ȡAPN
			if((p8)&&(p8[1]==7)&&((p8[0]=='f')||(p8[0]=='F'))
			&&((p8[1]=='j')||(p8[1]=='J'))
			&&((p8[3]=='p')||(p8[3]=='P'))
			&&(p8[4]=='.')
			&&((p8[5]=='f')||(p8[4]=='F'))
			&&((p8[6]=='j')||(p8[6]=='J')))
			{//�豸������֤ʱ���͵�APN�û���Ӧ�Զ������Ӧ�ĺ�׺������ʶ��ͨ��ģ��ΪGPRSʱΪA@fjep.fj����ʶ��ͨ��ģ��ΪCDMAʱΪA@fjep.vpdn.fj��
				p8=Get_Element((u8 *)ADDR_4500_2,7,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8)
				{//�ҵ��û���
					i=p8[1];//�ֽ���
					p8+=2;
					p8[i]=0;
					TxByte+=CopyString(p8,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
//   ��2��01��China Mobile���й��ƶ���
//   ��3��02��China Unicom���й���ͨ��
//   ��4��03��China Telecom���й����ţ�
					if((Comm_Ram->CSPN==1)||(Comm_Ram->CSPN==2))
					{
						TxByte+=CopyString((u8 *)"@fjep.fj",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					}
					else if(Comm_Ram->CSPN==3)
					{
						TxByte+=CopyString((u8 *)"@fjep.vpdn.fj",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					}
					else
					{
						TxByte+=CopyString((u8 *)"@fjep.vpdn.fj",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					}
					p8tx[TxByte]=',';
					TxByte++;
					p8=Get_Element((u8 *)ADDR_4500_2,8,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//�ҵ�����
						i=p8[1];//�ֽ���
						p8+=2;
						p8[i]=0;
						TxByte+=CopyString(p8,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					}
				}
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
			}
			#endif
			
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
				p8+=2;
				y=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������,������APNʱ�Ѿ�ȷ��
				for(i=0;i<y;i++)//�Թ��Ѿ����͵��û�������
				{
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
	    switch(UART1Ctrl->SubTask)//������
			{
				default:
			  case 0://ȷ������ģʽ
				  MR(ADDR_128KDATABUFF,ADDR_4500_2+2,6);
					p8=(u8*)ADDR_128KDATABUFF;
					i=p8[1]<<4;//B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ
					i|=p8[5]<<7;//B7=0TCP=1UDP
					UART1Ctrl->TCPUDP=i;
					i&=0x30;
					if((i==0x20)||(i==0x00))
					{//=2������ģʽ=0���ģʽ
					  UART1Ctrl->SubTask=1;
						return;
					}
					else
					{
						GPRS_3763_NextATCount();//��1��AT
						return;
					}
				case 1://	2��ɾ��ȫ��IP��֤ͨ����192.168.0.23��,��255.255.255.255��
					TxByte=CopyString((u8 *)"AT$MYIPFILTER=0,2,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
          p8tx[TxByte++]='"';
				  TxByte+=CopyString((u8 *)"0.0.0.0",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				  p8tx[TxByte++]='"';
				  p8tx[TxByte++]=',';
				  p8tx[TxByte++]='"';
				  TxByte+=CopyString((u8 *)"0.0.0.0",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
				  p8tx[TxByte++]='"';
				  p8tx[TxByte++]='\r';
					break;
				case 2://����IP����.����������IP��
				case 3://����IP����.�����ñ���IP
					TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					if(UART1Ctrl->SubTask==2)
					{
						x=1;
						p8tx[14]='0';
					}
					else
					{
						x=2;
						p8tx[14]='1';
					}
					i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
					if(i==0)
					{
					  i=MRR(ADDR_4500_3+1,1);//��վIP��ַ����
						if(x<=i)
						{
						  p8=Get_Element((u8*)ADDR_4500_3,x,1,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
							MR(ADDR_DATABUFF,(u32)p8+2,4);
						}
						else
						{
						  GPRS_3763_NextATCount();//��1��AT
							return;
						}
					}
					else
					{
					  MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
						p8=(u8*)ADDR_128KDATABUFF;
						p8+=2;
						y=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������,������APNʱ�Ѿ�ȷ��
						for(i=0;i<y;i++)//�Թ��Ѿ����͵��û�������
						{
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
						p8=Get_Element(p8,8,0,LENmax_4520_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
						//��ʱp8ָ��4520���ض�ͨѶ��������վIP�����б�
						i=p8[1];
						if(x<=i)
						{
						  p8=Get_Element(p8,x,1,LENmax_4520_2);//�ҵ�IP��ַ
						  MR(ADDR_DATABUFF,(u32)p8+2,4);
						}
						else
						{
						  GPRS_3763_NextATCount();//��1��AT
							return;
						}
					}
					//AT$MYIPFILTER=0,1,��192.168.0.23��,��255.255.255.255��	// ���ָ����IP��֤ͨ��
					
					//dcb "AT$MYIPFILTER=0,1,",0
					p8tx[TxByte]='"';
					TxByte++;
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<4;i++)
					{
						y=hex_bcd(p8[i]);//HEX��תΪ8λѹ��BCD��
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
					TxByte+=CopyString((u8 *)"0.0.0.0",p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					p8tx[TxByte]='"';
					TxByte++;
					p8tx[TxByte]='\r';
					TxByte++;
				break;
			}
			
			break;
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
				case 1://TCP�ͻ���ģʽ
				default://TCP�ͻ���ģʽ
					//AT$MYNETSRV=0,0,0,0,"172.22.33.2:5000" 
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,3,0,0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
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
						x=p8[1];
						p8+=2;
						y=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������,������APNʱ�Ѿ�ȷ��
						for(i=0;i<y;i++)//�Թ��Ѿ����͵��û�������
						{
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
						//��ʱp8ָ��4520���ض�ͨѶ��������վIP�����б�
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
					TxByte=CopyString((u8 *)"AT$MYNETSRV=0,2,1,0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					p8tx[14]=0x30+TCP_1_LISTEN;//�޸�socketֵ
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
					i=MRR(ADDR_4500_2+13,2);
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
				i=UART1Ctrl->TCPUDP;
				i&=0x30;
				switch(UART1Ctrl->SubTask)
				{
				  default:
				  case 0://�ͻ���open socket
						if((i==0x10)||(i==0x00))
						{//=1�ͻ���ģʽ=0���ģʽ
						  if(!(UART1Ctrl->res4&TCP_0_OPEN))
							{//��û��open socket
							  TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
								break;
							}
						}
						//������ģʽ�����Ѿ���socket��
						UART1Ctrl->SubTask++;
					  return;
					case 1://�����open socket
						if((i==0x20)||(i==0x00))
						{//=2������ģʽ
						  if(!(UART1Ctrl->res4&TCP_1_LISTEN_OPEN))
							{//��û��open socket
							  TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
						    p8tx[13]=0x30+TCP_1_LISTEN;//ʹ��TCP_1_LISTEN��soket����
								break;
							}
						}
						//����ģʽ�����Ѿ���socket��
						UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				    UART1Ctrl->SubTask=0;//������
				    UART1Ctrl->ATCount=AT_CSQ_LINK;//���ź����Ӻ�
						return;
				}
		#endif
				Terminal_Ram->GPRSWaitConnection_S_Timer=120;//20 GPRS�������ӷ���ȴ��붨ʱ��
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
				i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
				if(UART1Ctrl->res3<i)//res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
				{//��û�г��������е�APN������
					UART1Ctrl->res3++;
					UART1Ctrl->ATCount=AT_MYNETCON_APN;
					UART1Ctrl->LinkCount=0;
					return;
				}
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
#if (PPP_ENABLE==1)//ʹ��PPP���Ž������� 
				UART1Ctrl->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];
#endif
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
#if (PPP_ENABLE==1)//ʹ��PPP���Ž�������  
		if(0==(Comm_Ram->RTCBuff[0])%5)
		{
				if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])
				{
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("recheck----AT_CSQ_LINK\n",0,0,0);
#endif
			    UART1Ctrl->ATCount=AT_CSQ_LINK;
				}
		}
		  return;
#else
			if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
			{
				UART1Ctrl->LINKCHECK_RTCS=Comm_Ram->RTCBuff[0];


				if((UART1Ctrl->LINKCHECK_RTCS%5)<3)
				{
					GPRS_3763_NextATCount();//��1��AT
					return;
				}
				TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
  #if (USER/100)==17
				UART1Ctrl->SubTask=0;//�����г�������ģ������������ֲ�ͬ��ʽ��״̬���ñ�־���ڴ���״̬1��״̬2
  #endif

			}
			else
			{
				GPRS_3763_NextATCount();//��1��AT
				return;
			}
			break;
#endif
		case AT_MYNETREAD://��ȡ����
#if (PPP_ENABLE==1)//ʹ��PPP���Ž�������
     GPRS_3763_NextATCount();//��1��AT
		 return;
#else
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
			  i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket
				if(0x00==(UART1Ctrl->TCPUDP&0x30))
				{//��ǰ���ģʽ
					if(!(i&FORCE_TCP_1))//��ǰsocketδ�������������socket
					{
						i^=TCP_SELECT_SERVER;
					}
					UART1Ctrl->res4=i;
				}
//				  if((!(i&TCP_SELECT_SERVER))&&(i&TCP_0)&&(i&TCP_0_OPEN))
          if((!(i&TCP_SELECT_SERVER))&&(i&TCP_0_OPEN))
					{//�ͻ���ģʽ��ȡsocket0
					  TxByte=CopyString((u8 *)"AT$MYNETREAD=3,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					}
					else if ((i&TCP_SELECT_SERVER)&&(i&TCP_1))
					{//TCP server ͨ������
           //Զ��TCP�ͻ��˽�������
						TxByte=CopyString((u8 *)"AT$MYNETREAD=0,",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
						p8tx[13]=0x30+(i&TCP_1_SOCKET_MASK);//�޸�socketֵ
					}
					else
					{
					  if(UART1Ctrl->LINKCHECK_RTCS!=Comm_Ram->RTCBuff[0])//���߼��,ÿ����1��(GPRSģ�飺���źźʹ򿪲�ѯ)
						{
							Uart_698_RxTx(GPRSPORT);//UART_3761���շ���
							if(UART1Ctrl->HostACK>=4)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
							{//�����������ϱ��ȴ�ȷ�Ϸ���
								if(!((i&TCP_0_OPEN)||(i&TCP_0)))
								{//socket�����ڻ���ͨ�����Ӳ�����
									UART1Ctrl->NETOPENCount=0;
									UART1Ctrl->ATCount=AT_MYNETSRV;
									return;
								}
							}
						}
					  GPRS_3763_NextATCount();//��1��AT
					  return;
					}
				  //i=LEN_UART1IntATRx-1024;
					i=1460;
					i=hex_bcd(i);//HEX��תΪ8λѹ��BCD��
					TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
					p8tx[TxByte]='\r';
					TxByte++;
			}
			break;
#endif
		case AT_MYNETWRITE://��������
//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket
//#define TCP_0_OPEN 0x80  //TCP clinet  socket_open
//#define TCP_0 0x40       //TCP clinet link
//#define TCP_1_LISTEN_OPEN 0x20  //TCP server  socket_open
//#define TCP_1 0x10       //TCP server link
//#define TCP_SELECT_SERVER 0x08  //SELECT TCP_clinet and TCP_server
//#define FORCE_TCP_1 0x04 //FORCE_TCP_server
//#define TCP_1_SOCKET_MASK 0x3 //TCP server  socket

//#define TCP_1_LISTEN 4   //TCP server  listen
#if (PPP_ENABLE==1)//ʹ��PPP���Ž�������
     GPRS_3763_NextATCount();//��1��AT
		 return;
#else
			switch(UART1Ctrl->ATWriteTask)//AT���ݷ�������:0=������,1=������
			{
				case 0://��ATͷ
			    i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket
					TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					if ((i&TCP_SELECT_SERVER)&&(i&TCP_1))
					{//TCP server ͨ������//Զ��TCP�ͻ��˽�������
						p8tx[14]=0x30+(i&TCP_1_SOCKET_MASK);//�޸�socketֵ
					}
					else //if(!(i&TCP_SELECT_SERVER))
					{//��ѯ�ͻ���ģʽsocket0
#if (USER/100)==17
						if(Terminal_Ram->CopyTxByte!=0)
						{//��ǰ��Ҫ��������
							if(!((i&TCP_0_OPEN)||(i&TCP_0)))
							{//socket�����ڻ���ͨ�����Ӳ�����
								UART1Ctrl->NETOPENCount=0;
							  UART1Ctrl->ATCount=AT_MYNETSRV;
								return;
							}
						}
#endif
					}
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
#endif
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
		case AT_MYNETCLOSE://�ر�socket����
		  if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
			{
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
				UART1Ctrl->ATCount=AT_CSQ_LINK;
				return;
			}
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
		  break;
		case AT_MYNETACTOFF://�ر���������
		  if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
			{
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
				UART1Ctrl->ATCount=AT_MYNETACT;
				return;
			}
		  TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
		  break;
		case AT_ACCEPT://��������������ʱ������ģ���ϱ�����վIP
		  if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
			{
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
				UART1Ctrl->ATCount=AT_CSQ_LINK;
				return;
			}
		  TxByte=CopyString((u8 *)"AT$MYNETACCEPT=0,0,0\r",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[15]=0x30+(UART1Ctrl->res4&TCP_1_SOCKET_MASK);
		  break;
		case AT_CGDCONT://��������������ʱ������ģ���ϱ�����վIP
		  if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
			{
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask=0;//������
				UART1Ctrl->ATCount=AT_CSQ_LINK;
				return;
			}//AT+CGDCONT=1,"IP"," 
		  TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			
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
#if (USER/100)==17
          case 54://54: ����ר��
						 n=i;
					break;
#endif
				}
				p8+=2;
				y=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
				for(i=0;i<y;i++)//�Թ��Ѿ����͵�APN����
				{
				  p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				}
				for(;i<x;i++)//��ʱi�Ѿ�=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
				{
					if(p8[3]==n)//CMCC���ƶ��� = 0��CTCC�����ţ� = 1��CUCC����ͨ�� = 2
					{
						#if COMPARE_2G_3G_4G==1 //����APNʱ�ȶ�2G��3G��4G
						y=p8[5];//2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 �� 
						y+=2;
						if(y==Comm_Ram->RemoteWirelessModuleSYSINFO)//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
						#else
										//����APNʱ���ȶ�2G��3G��4G
						#endif
						{
							break;
						}
					}
					p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				}
				UART1Ctrl->res3=i;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
				
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
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
					return;
				}
			}
			TxByte+=CopyString((u8 *)ADDR_DATABUFF,p8tx+TxByte);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;	
			#if LOG_1376_3==1//0=û,1=��
			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
		  break;
		case AT_ATDT://���ò��ŷ�ʽ�����в���
		    UART1Ctrl->ATTask=1;
		    UART1Ctrl->LinkTask=89;
				UART1Ctrl->ATCount=AT_CSQ_LINK;//���ź�ǿ��
				return;
//			TxByte=CopyString((u8 *)"ATD*99***1#\r",p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
//			#if LOG_1376_3==1//0=û,1=��
//			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
//			#endif
//			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
//				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
//			#endif
//		  break;
		default:
			TxByte=CopyString((u8 *)p32[(4*AT_No)+1],p8tx);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
//			#if LOG_1376_3==1//0=û,1=��
//			LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
//			#endif
//			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
//				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
//			#endif
			break;
	}
	if(TxByte!=0)
	{
		//����UART1�ڷ���
		UART1Ctrl->BpsCtrl=0x3+(9<<5);//����57.6K 
//#ifndef LINUX
//		Init_UARTn(GPRSPORT);//��ʼ��UART��,���ͨ�ſ�����������
//#else
//		ms->linux_Init_UARTn(GPRSPORT);
//#endif
		UART1Ctrl->TxByte=TxByte;//�����ֽ���(���ֶ���)
    UART1Ctrl->TxCount=0;
	#if LOG_1376_3==1//0=û,1=��
		switch(p32[4*AT_No])//AT����
		{
			case AT_CSQ_LINK://���ź����Ӻ�(ÿ��1��)
			case AT_MYNETOPEN_LINK://���Ӽ��(ÿ��1��)
			case AT_MYNETREAD://������
			  LOG(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
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
				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				break;
			default:
				LOGRS232(13763,1,GPRSPORT,(u8 *)ADDR_ATTx,TxByte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
				break;
		}
	#endif
#if PPP_ENABLE==1
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			  log_out((u32)p8tx,TxByte, 'c');
#endif
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
	
#ifdef LINUX
	ms_Type * ms;
	ms=Get_ms();
	ms->linux_uart_receive(GPRSPORT);
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

	#if (USER/100)==17
	i=GPRS_URC_Rx();//�����ϱ����գ�Ĭ�Ϸ���1
	if(i)//������ص���1�򷵻�
	{
		return;
	}
	#endif
	
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
			  if(UART1Ctrl->SubTask<10)//����ͬ��������
			  {
#if (USER/100)==17
					UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
					Terminal_Ram->GPRSWaitTx_S_Timer=GPRSWait_SRVST_Timer;//�ȴ�ģ���ϱ�^SRVST: 2
					UART1Ctrl->SubTask=10;
#else
          UART1Ctrl->SubTask+=10;//������
					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
#endif
			  }
				else if(UART1Ctrl->SubTask>10)//������
				{//�Ѿ��·�AT+IPR�������Ѿ�
				  if(UART1Ctrl->BpsCtrl!=0x3+(9<<5))
				  {//ֻ�в����ʲ�Ϊ57600ʱ����ȥ��ʼ������
				    UART1Ctrl->BpsCtrl=0x3+(9<<5);
          //#ifndef LINUX 
            Init_UARTn(GPRSPORT);//��ʼ��UART��,���ͨ�ſ�����������
          //#else
          //    ms->linux_Init_UARTn(GPRSPORT);
          //#endif
          }
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
//				MW(ADDR_DATABUFF,ADDR_4500_5+30,6);
//				if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
//				{//==0,
//          i=Compare_DL698DataString((u8*)ADDR_4500_5+38,(u8*)"JS18GZYT",8);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//					if(!i)
//					{//�ȴ�SVRST����ʱ�Ѿ�Ϊ0
//						UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
//					  UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
//						return;
//					}
//				}
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
			if(!Addr)
			{
			  Addr=ATRxSearch((u32)((u8*)"$myccid"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			}
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
#if (USER/100)==17
    if(UART1Ctrl->LinkSubTask>=(NUM_ReATTx))//����AT���ͼ���
		{
		  GPRS_3763_NextATCount();//��1��AT
			return;
		}
#endif
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
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
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
			  if(p32[4*AT_No]==AT_MYNETSRV)
				{
					i=UART1Ctrl->TCPUDP;
					i&=0x30;
					if(UART1Ctrl->TCPUDP&0x80)
					{//UDP
						if(i==0x00)
						{//=2������ģʽ=0���ģʽ=1�ͻ���ģʽ
							UART1Ctrl->SubTask=4;
						}
					}
					else
					{//TCP
						if(i==0x00)
						{//=2������ģʽ=0���ģʽ=1�ͻ���ģʽ
							if(!(UART1Ctrl->res4&TCP_1_LISTEN_OPEN))
							{//�����˿ڻ�û�д�
							   if(UART1Ctrl->SubTask!=2)
								{
									UART1Ctrl->SubTask=2;
									UART1Ctrl->LinkSubTask=0;
									p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
									break;
								}
							} 
						}
					}
				}
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
			#if (PPP_ENABLE==1)//ʹ��PPP���Ž�������
			if(p32[4*AT_No]==AT_MYNETCON_USERPWD)
			{
       UART1Ctrl->ATCount=AT_CGDCONT;//����isp��������
		   break;
			}
			else
      #endif
			{
			  GPRS_3763_NextATCount();//��1��AT
			}
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
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
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
//				i=0;//��������õ�
//				for(x=0;x<3;x++)
//				{
//				  if(p8d[x]==',')
//					{
//					  break;
//					}
//					i<<=4;
//					i|=p8d[x]-0x30;
//				}
				switch(i)
				{
					//case 0://�޷���
					default:
						Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
						break;
					case 0://�޷���
						Addr=ATRxSearch((u32)((u8*)"OK\r\n"));
						if(Addr)
						{
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						}
						if(Terminal_Ram->GPRSWaitTx_S_Timer==0)
						{
		#if (USER/100)==17
							UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
							UART1Ctrl->ATCount=AT_ATE0;
							UART1Ctrl->SubTask=0;
							Terminal_Ram->GPRSWaitTx_S_Timer=GPRSWait_SRVST_Timer;
							UART1Ctrl->res3=0;//45200200���Ѿ���ͼ���ò����ĸ���
							UART1Ctrl->res4=0;//�ͻ��˺ͷ�����ѡ�����·
							UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//Զ�˿ͻ���Ϊ����ʱΪbit1-bit0Ϊ0x3,ʵ��socketֻ��Ϊ0����1
	            Comm_Ram->RemoteWirelessModuleSYSINFO=i; 
							#else
							GPRS_3763_NextATCount();//��1��AT					
		#endif
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
				i=0;
				for(x=2;x<5;x++)
				{
				  if(p8d[x]==0x0D)
					{
					  break;
					}
					i<<=4;
					i|=p8d[x]-0x30;
				}
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
#if (USER/100)==17
					case 0x754://?754: ע���Ͻ��յ���1.8GHz����ҵ������
					case 0x854://?854: ע���Ͻ��յ��� 1.8GHz I��II��ҵ�񣨳����أ�����
					case 0x954://?954: ע���Ͻ��յ��� 1.8GHz III��IV��ҵ������
						 Comm_Ram->CSPN=54;
					break;
#endif //(USER/100)==17
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
    Addr=ATRxSearch((u32)((u8*)"OK\r\n"));
		if(Addr)
		{
			p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
		}
			GPRS_3763_NextATCount();//��1��AT
			break;
		case AT_MYNETACT://������������
		#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
			DOTMIXPMODE->Ln9Code=6;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
		#endif
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
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
				Addr=ATRxSearch((u32)((u8*)"ERROR: 902"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr)
				{//�Ѿ�����PDP
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//�޸��Ѷ�ָ��
					UART1Ctrl->ATCount=AT_MYNETACTOFF;
					break;
				}
				Addr=ATRxSearch((u32)((u8*)"ERROR: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
				if(Addr)
				{//
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
					i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
			    if(UART1Ctrl->res3<i)//res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
			    {//��û�г��������е�APN������
					  UART1Ctrl->res3++;
						UART1Ctrl->ATCount=AT_MYNETCON_APN;
						return;
					}
					UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�,���յ�ͨѶ������ͬһ����Ӫ�������в�ͬ��APN��USERPASS��û�취����ȥ
					return;
				}
				if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//�ȴ������������Ӷ�ʱ
				{
				  i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
			    if(UART1Ctrl->res3<i)//res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
			    {//��û�г��������е�APN������
					  UART1Ctrl->res3++;
						UART1Ctrl->ATCount=AT_MYNETCON_APN;
						return;
					}
					WarningOccur(GPRSPORT,(u8 *)p32[(4*AT_No)+2]);//�澯����,��ڸ澯����͸澯�ַ���
#if (USER/100)==17
          UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�,���յ�ͨѶ������ͬһ����Ӫ�������в�ͬ��APN��USERPASS��û�취����ȥ
#else //(USER/100)==17
					//UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
					GPRS_3763_NextATCount();//��1��AT;���ػ����ܴ������ն����޸�
#endif
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
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
			#if (USER/100)==13//������
				WarningOccur(GPRSPORT,(u8 *)"IP���ʿ�������");//�澯����,��ڸ澯����͸澯�ַ���
			#endif
#if MainProtocol==376
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+6);//�޸��Ѷ�ָ��
#elseif MainProtocol==698
        p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
#endif
				UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
				UART1Ctrl->SubTask++;//������
				if(UART1Ctrl->SubTask>3)//sub0ȷ�Ϲ���ģʽ��sub1ɾ�����з���ǽ��sub2��������IP��sub3���ñ���IP
				{
					GPRS_3763_NextATCount();//��1��AT
				}
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
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				
				i=UART1Ctrl->TCPUDP;
				i&=0x30;
				switch(UART1Ctrl->SubTask)
				{
				  default:
				  case 0://�ͻ���open socket
						//�ͻ���ģʽ
						UART1Ctrl->res4|=TCP_0_OPEN;  //TCP clinet  socket_open
						UART1Ctrl->res4&=~TCP_SELECT_SERVER;
						UART1Ctrl->NETOPENCount=0;
#if (USER/100)==17
			      Terminal_Ram->GPRSAutoDiscon_S_Timer=GPRSAUTOTODISSCON_S_Timer;//����������ͨ�������ر�socket��ʱ//23 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
#endif
						if(i==0x00)
						{//���ģʽ
							if(UART1Ctrl->HostACK!=0)
							{

								UART1Ctrl->ATCount=AT_MYNETWRITE;
								return;
							}
						  UART1Ctrl->SubTask=1;
							return;
						}
						break;
					case 1://�����open socket
						UART1Ctrl->res4|=TCP_1_LISTEN_OPEN;  //TCP server  socket_open
						UART1Ctrl->res4|=TCP_SELECT_SERVER;
					  break;
				}
				if(UART1Ctrl->HostACK!=0)
				{
				  UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
					UART1Ctrl->ATCount=AT_MYNETWRITE;
					return;
				}
				WarningCancel(GPRSPORT);//�澯ȡ��,��ڸ澯����
				if((i==0x10)||(i==0x00))
				{//�ͻ����ͻ��ģʽ�·���½֡
					if(UART1Ctrl->LinkTask<100)
					{
						UART1Ctrl->LinkTask=100;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					}
				}
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
//			if(p32[4*AT_No]!=AT_CSQ_LINK)//AT����
			{
				LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			}
		#endif
		#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
//			if(p32[4*AT_No]!=AT_CSQ_LINK)//AT����
			{
				LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			}
		#endif
			
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
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
#if (USER/100)==17
			switch(UART1Ctrl->SubTask)
			{
				case 0:
				{
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
				LOG_ATRx(0);//GPRSģ��AT��������LOG;ʹ��ADDR_DATABUFF
			#endif
			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
				LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;ʹ��ADDR_DATABUFF
			#endif
					UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
//					i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket		
//					if(i&TCP_0_OPEN)
//					{
//						Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: 3"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//						if(Addr!=0)
//						{
//							Addr++;
//						}
//					}
//					else if(i&TCP_1_LISTEN_OPEN)
//					{
//					  MC(0,ADDR_DATABUFF,64);
//						p8s=(u8*)ADDR_DATABUFF;
//						CopyString((u8 *)"$MYNETOPEN: 0",p8s);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
//						p8s[12]=0x30+TCP_1_LISTEN;//�޸�socketֵ
//						Addr=ATRxSearch(ADDR_DATABUFF);//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//					}
//					else if(i^TCP_1_SOCKET_MASK)
//					{
//					  MC(0,ADDR_DATABUFF,64);
//						p8s=(u8*)ADDR_DATABUFF;
//						CopyString((u8 *)"$MYNETOPEN: 0",p8s);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
//						p8s[12]=0x30+(i&TCP_1_SOCKET_MASK);//(i&TCP_1_SOCKET_MASK);//�޸�socketֵ
//						Addr=ATRxSearch(ADDR_DATABUFF);//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//					}
//					else
//					{
//						Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//						if(Addr!=0)
//						{
//							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
//						}
//						GPRS_3763_NextATCount();//��1��AT
//						break;
//					}
					Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr!=0)
					{
						Addr+=2;
					}
          p1=p16fifo[1];
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
						
//						//���ڿͻ������������ģʽ�µĴ���
////						$MYNETOPEN: 2,"100.32.73.171",5100,"100.32.73.84","0.0.0.0","0.0.0.0",1,"0:0:0:0",0\r
////            $MYNETOPEN: 0,"100.32.73.171",5100,"100.32.73.84","0.0.0.0","0.0.0.0",0,"127.0.0.1",5100\r
//						p16fifo[1]=lastp16fifo;
//						
////						p8s=(u8*)(ADDR_UART1IntATRx+10+p16fifo[1]);���Դ���
////						i=CopyString((u8 *)"$MYNETOPEN: 0,\r",p8s);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
//						
//						UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket		
//						UART1Ctrl->res4&=~TCP_1;//TCPserverͨ������
//						Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//						Addr2=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//						while(Addr<Addr2)
//						{
//							if(!Addr)
//							{//Addr==0
//								break;
//							}
//							p8s = (u8 *)Addr;
//							i=p8s[0]-0x30;
//							switch(i)
//							{
//								case 3://�ͻ���socket
//									break;
//								case 0://������socket
//								case 1:
//									UART1Ctrl->res4&=~TCP_1_SOCKET_MASK;//����socket
//					        UART1Ctrl->res4|=i;
//								  UART1Ctrl->res4|=TCP_1;//TCPserverͨ������
//									break;
//								case 2://����socket
//									break;
//								default:
//									break;
//							}
//				      Addr=ATRxSearch((u32)((u8*)"\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
//							Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: "));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//						}
						
						
				#endif
						Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
						if(Addr!=0)
						{
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						}
	#if (USER/100)==17
						p8s=(u8 *)ADDR_4500_5+4;
						if((p8s[0]==0x5A)&&(p8s[1]==0x58)&&(p8s[2]==0x47)&&(p8s[3]==0x44))//'ZXGD',���˸ߴ��ģ����ظ�������
						{
							Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//GPRS AT������10MS��ʱ��
							UART1Ctrl->SubTask=1;
							UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
							return;
						}								
						//	i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket		
					if(UART1Ctrl->res4&TCP_0_OPEN)
					{
						if(!UART1Ctrl->HostACK)
						{//�ϱ���ʱ��ʱ��Ϊ0
							if(!Terminal_Ram->GPRSAutoDiscon_S_Timer)//����������ͨ�������ر�socket��ʱ//23 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
							{//������ͨ�Ŷ�ʱ������
								UART1Ctrl->ATCount=AT_MYNETCLOSE;
								return;
							}
						}
					}
						          
	#endif
						GPRS_3763_NextATCount();//��1��AT
						break;
					}
				   goto ReTxAT;//�ط���AT
					 
#if (USER/100)==17
				}
				case 1:
					Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr==0)
					{
						if(Terminal_Ram->AT_MS_Timer!=0)
						{//����û���
							return;
						}
					}
					Addr=ATRxSearch((u32)((u8*)"$MYNETOPEN: 0"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr!=0)
					{
						Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
						if(Addr!=0)
						{
							p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
						}
						UART1Ctrl->SubTask=0;
						UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
						GPRS_3763_NextATCount();//��1��AT
						break;
					}
					goto ReTxAT;//�ط���AT
				default:
				  UART1Ctrl->SubTask=0;
					UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
				  goto ReTxAT;//�ط���AT
			}
			break;
#endif
		case AT_MYNETREAD://��ȡ����
			p1=p16fifo[1];//ԭp1
			if(!(UART1Ctrl->res4&TCP_SELECT_SERVER))
			{
			  Addr=ATRxSearch((u32)((u8*)"$MYNETREAD: 3,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			}
			else
			{
			  MC(0,ADDR_DATABUFF,64);
				p8s=(u8*)ADDR_DATABUFF;
			  CopyString((u8 *)"$MYNETREAD: 0,",p8s);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
        p8s[12]=0x30+(i&TCP_1_SOCKET_MASK);//�޸�socketֵ
				Addr=ATRxSearch(ADDR_DATABUFF);//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			}
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
					  if((UART1Ctrl->Task!=2)||(UART1Ctrl->Task!=4))
						{
						  UART1Ctrl->res4&=~FORCE_TCP_1;//������ǰsocket
						}
						GPRS_3763_NextATCount();//��1��AT
					}
					else
					{
					  UART1Ctrl->res4|=FORCE_TCP_1;//������ǰsocket
					}
					return;
				}
			}
			else
			{
				Addr=ATRxSearch((u32)((u8*)"ERROR\r"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
        if(Addr)
				{//���ش���
					if(!(UART1Ctrl->res4&TCP_SELECT_SERVER))
					{//����������ͨ��
						UART1Ctrl->res4&=~TCP_0;//TCPclentͨ������
						UART1Ctrl->res4&=~TCP_0_OPEN;//TCPclentͨ������
					}
					else
					{
						UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket		
						UART1Ctrl->res4&=~TCP_1;//TCPserverͨ������
					}
					UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			    GPRS_3763_NextATCount();//��1��AT
					break;
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
					i=UART1Ctrl->res4;
				  if(!(i&TCP_SELECT_SERVER))
					{
					  Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 3,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
          }
					else
					{
						MC(0,ADDR_DATABUFF,64);
						p8s=(u8*)ADDR_DATABUFF;
						CopyString((u8 *)"$MYNETWRITE: 0,",p8s);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
						p8s[13]=0x30+(i&TCP_1_SOCKET_MASK);//�޸�socketֵ
						Addr=ATRxSearch(ADDR_DATABUFF);//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					}
//					Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
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
						
//						i=GetATRxString(ADDR_DATABUFF);//ȡAT���ص�ǰp1ָ�뿪ʼ��0x0d0x0a�������ַ���,����ַ�����ŵ�ַ;����:����0x0d0x0a���ַ�������,ͬʱ�޸�p1ָ�����ַ�����ʼ
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
#if (USER/100)==17
          p8s=Get_Element((u8*)ADDR_4500_5,1,0,LENmax_4500_5);
					if((p8s[2]==0x48)&&(p8s[3]==0x57))//'HW'
					{
					  p8s=Get_Element((u8*)ADDR_4500_5,4,0,LENmax_4500_5);
						Terminal_Ram->AT_MS_Timer=p32[(4*UART1Ctrl->ATCount)+3]/10;//GPRS AT������10MS��ʱ��
						UART1Ctrl->ATTask=3;//AT����:0=����,1=����,2=���ڷ���,3=����
						UART1Ctrl->ATWriteTask=2;//AT���ݷ�������:0=������,1=������,2���ȴ��ϱ����ڽ����г�����ģ�黪Ϊ���������⴦��
						return;
					}
#endif
          
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
//						if(UART1Ctrl->HostACK>0)
//						{
//						  UART1Ctrl->res4|=FORCE_TCP_1;//������ǰsocket
//						}
//						else
//						{
//						  UART1Ctrl->res4&=~FORCE_TCP_1;//������ǰsocket
//						}
						GPRS_3763_NextATCount();//��1��AT
					}
					break;
#if (USER/100)==17
				case 2://������
				  Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
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
					Addr=ATRxSearch((u32)((u8*)"$MYNETWRITE: 0,"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
					if(Addr!=0)
					{
					  p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
					}
					UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
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
#endif
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
		case AT_MYNETCLOSE://�ر�socket����
		  Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"$MYNETCLOSE: 3"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		  #endif
			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
					LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			if(Addr!=0)
			{
			  UART1Ctrl->res4&=~TCP_0_OPEN;//TCP clinet  socket_closeͨ������
				UART1Ctrl->res4&=~TCP_0;//TCP clinet linkͨ������
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				UART1Ctrl->LinkSubTask=2;
			}
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			}
			else
			{
			  UART1Ctrl->LinkSubTask++;
			}
		  break;
		case AT_MYNETACTOFF://�ر���������
		  Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		 #endif
			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
					LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			if(Addr)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				UART1Ctrl->ATCount=AT_MYNETACT;
			}
			else
			{
			  UART1Ctrl->LinkSubTask++;
			}
		  break;
		case AT_ACCEPT://��������������ʱ������ģ���ϱ�����վIP
		  Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		 #endif
			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
					LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			if(Addr)
			{
			  UART1Ctrl->res4|=TCP_1;//TCPserverͨ������
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
				UART1Ctrl->ATCount=AT_MYNETREAD;
			}
			else
			{
			  UART1Ctrl->LinkSubTask++;
			}
		  break;
		case AT_CGDCONT://����isp��������
		  Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//����û���
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
			#if LOG_1376_3==1//0=û,1=��
			LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
		  #endif
			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
					LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
			#endif
			if(Addr)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
			  GPRS_3763_NextATCount();//��1��AT
				break;
			}
			goto ReTxAT;//�ط���AT
		case AT_ATDT://����isp�������
		   UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
			 return;
//		  Addr=ATRxSearch((u32)((u8*)"CONNECT\r\n"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//			if(Addr==0)
//			{
//				if(Terminal_Ram->AT_MS_Timer!=0)
//				{//����û���
//					return;
//				}
//			}
//			UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
//			Addr=ATRxSearch((u32)((u8*)"CONNECT"));//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
//			#if LOG_1376_3==1//0=û,1=��
//			  LOG_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
//		  #endif
//			#if DEBUG_GPRStoRS232==1//0=��,1=��;����:GPRS���շ����ݴ�ӡ��RS232��
//					LOGRS232_ATRx(0);//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
//			#endif
//			if(Addr)
//			{
//				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//�޸��Ѷ�ָ��
//			  UART1Ctrl->ATCount=AT_CSQ_LINK;
//				break;
//			}
//			if(UART1Ctrl->LinkSubTask>=2)//����AT���ͼ���
//			{
//				i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
//				if(UART1Ctrl->res3<i)//res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
//				{//��û�г��������е�APN������
//					UART1Ctrl->LinkSubTask=0;//����AT���ͼ���
//				  UART1Ctrl->SubTask=0;//������
//					UART1Ctrl->res3++;
//					UART1Ctrl->ATCount=AT_CGDCONT;
//					return;
//				}
//				return;
//			}
//			goto ReTxAT;//�ط���AT
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
#if PPP_ENABLE==1
			UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
#else
      UART1Ctrl->ATTask=0;//AT����:0=����,1=����,2=���ڷ���,3=����
#endif
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
#if (PPP_ENABLE==1)
	u16* p16;
	u32 p0;
	int fd;
	int err;
	s32 socket;
	UARTCtrl_TypeDef * UARTCtrl;
	ms_Type * ms;
	u16 * p16timer;
	u8 *p8s;
	ms=Get_ms();
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
			#if ((USER/100)==17)//�û���ʶ:���ռ�����
				OffR5V0();//��R5V0��Դ
				//�ر��ܵ�Դ��ʱ20��+(0-30��������ʱ)
				i+=10000/10;
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
				UART1Ctrl->res3=0;//45200200���Ѿ���ͼ���ò����ĸ���
				UART1Ctrl->res4=0;//�ͻ��˺ͷ�����ѡ�����·
				UART1Ctrl->res4|=TCP_1_SOCKET_MASK;//Զ�˿ͻ���Ϊ����ʱΪbit1-bit0Ϊ0x3,ʵ��socketֻ��Ϊ0����1
				Terminal_Ram->CopyTxByte=0;
#if PPP_ENABLE==1
				if(UART1Ctrl->TCPSocket>2)
				{
					ms->link_close(UART1Ctrl->TCPSocket);
					UART1Ctrl->TCPSocket=0;
				}
				if(Svr_Skt->ppp0_clint>2)
				{
					ms->link_close(Svr_Skt->ppp0_clint);
					Svr_Skt->ppp0_clint=0;
				}
				Svr_Skt->ppp0_clint=0;
				//������ģ�飬����ֻ�ٴν�������socket
				if(Svr_Skt->ppp0_listen>2)
				{
					ms->link_close(Svr_Skt->ppp0_listen);
					Svr_Skt->ppp0_listen=0;
				}
#endif
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
//#ifndef LINUX
		Init_UARTn(GPRSPORT);//��ʼ��UART��,���ͨ�ſ�����������
//#else
//    ms=Get_ms();
//		ms->linux_Init_UARTn(GPRSPORT);
//#endif
				UART1Ctrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;//����UART1Timerֱ�ӷ���
		case 8://��ʼ��AT����
#if PPP_ENABLE==1//PPP���ſ�ʼǰ��Ҫ��ȡSYSINFO��ȷ��ͨ�Ų���
      GPRS_3763_AT_RxTx();//GPRS_3763ģ��AT���շ���
			return;
		case 88://kill pppd�����˳�͸��ģʽ
		case 89://��鲦�Ŵ����豸
		case 90://ִ�в��ų���,ͨ��SYSINFO��ȡ�����������ͣ��·��û������� 
		case 91://��ѯ���Ž��,���ʧ�ܻ��߳�ʱ��ת��task=88
		case 92://ȷ������������ģʽ
		case 93://�ͻ���ģʽ�µ�socket����
		case 94://��������������socket
		case 95://���У���task=100
		  GPRS_3763_AT_RxTx();//GPRS_3763ģ��AT���շ���//���ڼ��ģ���ϱ�����Ϣ���ź�ǿ�ȵ�
		  PPP_RxTx();//socket���ӽ�����
		return;
#else
     break;
#endif
		case 100://����ok
		case 101://��¼ok
		case 102://���˳���¼
			break;
		case 103://�˳���¼
		case 104://�ػ�
#if PPP_ENABLE==1
#else
			if(UART1Ctrl->ATTask==0)//AT����:0=����,1=����,2=���ڷ���,3=����
#endif
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
#if PPP_ENABLE==1
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("\n",0,0,0);
			ms->link_printf("start main loop\n",0,0,0);
#endif
//���pppd���Ž����Ƿ���Ȼ����
  {
    i=Check_pppd_exist();//���ڷ���1�������ڷ���0
		if(!i)
		{
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_system((char*)"/bin/cat /tmp/pppdpid");
			ms->link_printf("pppd pid not exist,restart\n",0,0,0);
#endif
		 UART1Ctrl->LinkTask=89;
//		  UART1Ctrl->LinkTask=8;
//			UARTCtrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
//			UARTCtrl->ATCount=AT_MYPOWEROFF;//�ػ�
		  return;
		}
		CopyString((u8 *)"��IP",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		MR(ADDR_DATABUFF+4,ADDR_PPP0_IP4ADDR_ASCII,19);
		WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ�
	}
	if(UART1Ctrl->res4&TCP_1_LISTEN_OPEN)
	{//TCP�����˿ڽ���
		socket=ms->link_accept(Svr_Skt->ppp0_listen,NULL,NULL);
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("\n",0,0,0);
		ms->link_printf("ms->link_accept=%d\n",socket,0,0);
#endif
		if(socket>0)
		{
			if(Svr_Skt->ppp0_clint>2)
			{
				ms->link_close(Svr_Skt->ppp0_clint);
			}
			if((fd=ms->link_fcntl(socket,F_GETFL,0))>=0)//��ȡ�ļ�������������
			{
				if((fd=ms->link_fcntl(socket, F_SETFL, fd | O_NONBLOCK))>=0)	//���ļ�����������������Ϊ������
				{
					UARTCtrl->res4|=TCP_1;
			    Svr_Skt->ppp0_clint=socket;
				}
				else
				{
				  if(socket>2)
				  {
					  ms->link_close(socket);
					}
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("------------------------------------------------------\n",0,0,0);
		ms->link_printf("ms->link_accept_close=%d\n",socket,0,0);
#endif
				}
			}
			else
			{
				ms->link_close(socket);
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("------------------------------------------------------\n",0,0,0);
		ms->link_printf("ms->link_accept_close=%d\n",socket,0,0);
#endif
			}
		}
		else
		{
			i=ms->link_errno();
	#if ((DEBUG==1)&&(DEBUG_GPRS==1))
				ms->link_printf("ms->link_accept_error=%d\n",i,0,0);
	#endif
			if(i!=EWOULDBLOCK)
			{//�����쳣
				if(Svr_Skt->ppp0_listen>2)
				{
					ms->link_close(Svr_Skt->ppp0_listen);
					Svr_Skt->ppp0_listen=0;
				}
				
				UART1Ctrl->res4&=~TCP_1_LISTEN_OPEN;//�������socket�����ı�־
				UARTCtrl->LinkTask=94;
				return;
			}
		}
	}
	else
	{
	 if((0x00==(UART1Ctrl->TCPUDP&0x30))||(0x20==(UART1Ctrl->TCPUDP&0x30)))//B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ
	 {
		 if(Svr_Skt->ppp0_listen>2)
		{
			ms->link_close(Svr_Skt->ppp0_listen);
			Svr_Skt->ppp0_listen=0;
		}
		UARTCtrl->LinkTask=94;
		return;
	 }
	}
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(GPRSPORT));
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("UARTCtrl->Task=%d\n",UARTCtrl->Task,0,0);
#endif
	switch(UARTCtrl->Task)
	{
		case 0://����
		  GPRS_3763_AT_RxTx();//���ڼ��ģ���ϱ�����Ϣ���ź�ǿ�ȵ�
		  i=UART1Ctrl->res4;//bit7==1socket_open,==0socket_close,bit6=1 TCP clinet ͨ��������,bit5,==1socket_open,==0socket_closebit4=1TCP server ͨ������,bit3,==0�ͻ���ͨѶ==1�����ͨѶbit2ǿ�Ʒ����ͨѶbit1-0 tcp�����socket
			if(0x00==(UART1Ctrl->TCPUDP&0x30))//B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ
			{//��ǰ���ģʽ
				if((i&TCP_0_OPEN)&&(!(i&TCP_1)))
				{//�ͻ���ģʽ�����ϣ�����ģʽ��������
				  i&=~TCP_SELECT_SERVER;
				}
				else if((!(i&TCP_0_OPEN))&&(i&TCP_1))
				{//�ͻ���ģʽ�����ӣ�����ģʽ�пͻ�������
				  i|=TCP_SELECT_SERVER;
				}
				else
				{
				  if(!(i&FORCE_TCP_1))//��ǰsocketδ�������������Ҫ����socket//#define FORCE_TCP_1 0x04 
				  {
					  i^=TCP_SELECT_SERVER;//#define TCP_SELECT_SERVER 0x08  
				  }
				}
				UART1Ctrl->res4=i;
			}
			else if(0x10==(UART1Ctrl->TCPUDP&0x30))
			{//�ͻ���ģʽ
			  UART1Ctrl->res4&=~TCP_SELECT_SERVER;
			}
			else if(0x20==(UART1Ctrl->TCPUDP&0x30))
			{//������ģʽ
			  UART1Ctrl->res4|=TCP_SELECT_SERVER;
			}
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("UART1Ctrl->res4=%x\n",UART1Ctrl->res4,0,0);
#endif
			p16=(u16*)Get_ADDR_UARTn(GPRSPORT);//�õ�UART�����׵�ַ
			LEN_UARTnTx=Get_LEN_UARTnIntRx(GPRSPORT);//�õ�UART�жϽ��ջ��峤��
			LEN_UARTnTx-=10;
			p0=p16[0];
			if(p0>=LEN_UARTnTx)
			{
				p0=0;
			}
			x=LEN_UARTnTx-p0;//�ɽ��ճ���
			fd=0;
			if((UARTCtrl->TCPUDP&0x80)==0x0)
			{//TCP
				i=UART1Ctrl->res4;
				if((!(i&TCP_SELECT_SERVER))&&(i&TCP_0_OPEN))
				{//�ͻ���ģʽ
					fd=ms->link_recv(UARTCtrl->TCPSocket,(void*)((u32)p16+10+p0),x,MSG_DONTWAIT);

#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("current mod=clinet mode\n",0,0,0);
			ms->link_printf("tcplink_recv_socket=%d\n",UARTCtrl->TCPSocket,0,0);
			ms->link_printf("link_recv_return=%d\n",fd,0,0);
			ms->link_printf("recv_buffer_addr=%x\n",(u32)p16,0,0);
			err=ms->link_errno();
			ms->link_printf("link_recv_error=%d\n",err,0,0);
			if(fd>0)
			{
			  ms->link_printf("link_recv_data------------------------\n",0,0,0);
			  log_out((u32)p16+10+p0,fd, 'x');
			}
#endif
					if(fd==0)
					{
					 	UARTCtrl->res4&=~TCP_0_OPEN;
						return;
					}
					else if(fd<0)
					{//������ģʽ�µĽ���
						err=ms->link_errno();
						if(err==ECONNRESET)
						{
						  UARTCtrl->res4&=~TCP_0_OPEN;
							return;
						}
						else if((err!=EINTR)&&(err!=EAGAIN))
						{
							UARTCtrl->res4&=~TCP_0_OPEN;
//							UARTCtrl->LinkTask=93;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
						  return;
						}
#if (USER/100)==17
			      if(Terminal_Ram->GPRSAutoDiscon_S_Timer==0)//����������ͨ�������ر�socket��ʱ//23 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
						{//������ʱ�䵽
						  if(UARTCtrl->TCPSocket>2)
							{
							  ms->link_close(UARTCtrl->TCPSocket);
							  UARTCtrl->TCPSocket=0;
							}
						  UARTCtrl->res4&=~TCP_0_OPEN;
							return;
						}
#endif
						fd=0;
					}
					else if(fd>0)
					{
#if (USER/100)==17
			      Terminal_Ram->GPRSAutoDiscon_S_Timer=GPRSAUTOTODISSCON_S_Timer;//����������ͨ�������ر�socket��ʱ//23 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
#endif
					}
				}
				else if ((i&TCP_SELECT_SERVER)&&(i&TCP_1))
				{//TCP server ͨ������
					fd=ms->link_recv(Svr_Skt->ppp0_clint,(void*)((u32)p16+10+p0),x,MSG_DONTWAIT);
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("current mod=server mode\n",0,0,0);
			ms->link_printf("tcplink_recv_socket=%d\n",Svr_Skt->ppp0_clint,0,0);
			ms->link_printf("link_recv_return=%d\n",fd,0,0);
			err=ms->link_errno();
			ms->link_printf("link_recv_error=%d\n",err,0,0);
			if(fd>0)
			{
			  ms->link_printf("link_recv_data------------------------\n",0,0,0);
			  log_out((u32)p16+10+p0,fd, 'x');
			}
#endif
					if(fd>0)
					{//���������վ(�ͻ���)���������ݣ���ǿ�ƺ���վ(�ͻ���)ͨѶ
						UART1Ctrl->res4|=FORCE_TCP_1;
					}
					else if(fd==0)
					{
					  ms->link_close(Svr_Skt->ppp0_clint);
						Svr_Skt->ppp0_clint=0;
					 	UARTCtrl->res4&=~TCP_1;
						return;
					}
					else if(fd<0)
					{//������ģʽ�µĽ���
					  UART1Ctrl->res4&=~FORCE_TCP_1;//�������
						err=ms->link_errno();
						if((err!=EINTR)&&(err!=EAGAIN))
						{
							ms->link_close(Svr_Skt->ppp0_clint);
							Svr_Skt->ppp0_clint=0;
							UARTCtrl->res4&=~TCP_1;
						  return;
						}
						fd=0;
					}
				}
			}
			else
			{//UDP
				fd=ms->link_recv(UARTCtrl->UDPSocket,(void *)((u32)p16+10+p0),x,MSG_DONTWAIT);
			}

			if(fd<=LEN_UARTnTx)
			{
				p0+=fd;
				if(p0>=LEN_UARTnTx)
				{
					p0=0;
				}
				p16[0]=p0;
			}
			//���߿���
			if(UARTCtrl->LinkCtrl==0)//���ӿ���0=������,1=����
			{
				if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					UARTCtrl->LinkTask=102;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 ��̫�������붨ʱ��
					return;
				}
			}
			break;
		case 2://�ȴ���֡���ݷ���
		#if MainProtocol==376
		case 5://�ȴ���֡���ݷ���
			#if (USER/100)==11//����ϵ
				USER11xx_TxAddr(GPRSPORT);//����ϵ����֡�ն˵�ַ2�ֽڳ��ȱ�Ϊ4�ֽ�,�����¼���֡У���,�跢�͵�֡�ֽ���+2
			#endif
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
			UARTCtrl->Task++;
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
				LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
				LOGRS232(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
				UARTCtrl->TxCount=0;
				p16timer[0]=30000/10;//30��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
				UARTCtrl->Task++;
				Terminal_Ram->CopyTxByte=UARTCtrl->TxByte;//�����ֽ���
				Terminal_Ram->CopyTxCount=0;//�ѷ��ͼ���
			}
			else
			{
				p8tx=(u8 *)Get_ADDR_UARTnTx(GPRSPORT);
				MW((u32)p8tx,(u32)p8tx+LEN_UARTnTx,UARTCtrl->TxByte);
				UARTCtrl->TxAPDUSEQ=0;//��·���֡��ż���0-4095
				UARTCtrl->TxAPDUByte=0;//��·���֡�ѷ���APDU�ֽ���
				UARTCtrl->Task=5;//5=�ȴ���·���֡���ݷ���
			}
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("------------------------------\n",0,0,0);
			ms->link_printf("UARTCtrl->Task=%d\n",UARTCtrl->Task,0,0);
			ms->link_printf("UARTCtrl->TxByte=%d\n",UARTCtrl->TxByte,0,0);
#endif
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
			i=UARTCtrl->TxAPDUByte;
			if(i>=LEN_TAPDU)
			{
				UARTCtrl->RxByte=0;//rx����=0
				UARTCtrl->Task=0;//0=����
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
			x=UARTCtrl->TxAPDUSEQ;//��·���֡��ż���0-4095
			UARTCtrl->TxAPDUSEQ++;//��·���֡��ż���0-4095
			x&=0x7ff;
			if(UARTCtrl->TxAPDUByte==0)
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
			MR((u32)p8tx+LEN_Tx,(u32)p8tx+LEN_UARTnTx+4+LEN_ADDR+2+UARTCtrl->TxAPDUByte,LEN_TxAPDU);
			UARTCtrl->TxAPDUByte+=LEN_TxAPDU;
			LEN_Tx+=LEN_TxAPDU;
			LEN_Tx+=2+1;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
			Uart_698_HCSFCS((u8*)p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		 #if LOG_68==1//0=û,1=��
			LOG(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		 #endif
		 #if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
			LOGRS232(698,1,GPRSPORT,(u8 *)Get_ADDR_UARTnTx(GPRSPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		 #endif
			Terminal_Ram->CopyTxByte=UARTCtrl->TxByte;//�����ֽ���
			Terminal_Ram->CopyTxCount=0;//�ѷ��ͼ���
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
			UARTCtrl->Task++;
			break;
	#endif
		case 3://���͵�֡�������ڷ���
		case 6://��֡�������ڷ���
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
      ms->link_printf("------------------------------\n",0,0,0);
			ms->link_printf("Terminal_Ram->CopyTxByte=%d\n",Terminal_Ram->CopyTxByte,0,0);
#endif
			if(Terminal_Ram->CopyTxByte)
			{
				if(UARTCtrl->HostACK)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
				{//����������ϱ�֡
					UART1Ctrl->res4&=~TCP_SELECT_SERVER;
#if (USER/100)==17
			    Terminal_Ram->GPRSAutoDiscon_S_Timer=GPRSAUTOTODISSCON_S_Timer;//����������ͨ�������ر�socket��ʱ//23 GPRS��������ģʽ������ͨ���Զ������붨ʱ��
#endif
				}
				if((UARTCtrl->TCPUDP&0x80)==0x0)
				{//TCP
					x=2048;
					if(x>Terminal_Ram->CopyTxByte)
					{
						x=Terminal_Ram->CopyTxByte;
					}
					p8s=(u8*)Get_ADDR_UARTnTx(GPRSPORT);
					p8s+=Terminal_Ram->CopyTxCount;
					i=UART1Ctrl->res4;
					fd=0;
					if ((i&TCP_SELECT_SERVER)&&(i&TCP_1))
					{//TCP server ͨ������
					  fd=ms->link_send(Svr_Skt->ppp0_clint, (void *)p8s,x, MSG_DONTWAIT);
						
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
					ms->link_printf("tcplink_send_socket=%d\n",Svr_Skt->ppp0_clint,0,0);
					ms->link_printf("link_send_return=%d\n",fd,0,0);
					if(fd<0)
					{
					  err=ms->link_errno();
						ms->link_printf("link_send_error=%d\n",err,0,0);
						if(err==ENOTSOCK)
						{

						}
					}
					if(fd>0)
					{
						log_out((u32)p8s,fd, 'x');
					}
#endif
					  if(fd==0)
						{
						  UART1Ctrl->res4&=~FORCE_TCP_1;//�������
						  ms->link_close(Svr_Skt->ppp0_clint);
							UARTCtrl->res4&=~TCP_1;
							Svr_Skt->ppp0_clint=0;
							return;
						}
					}
					else if((!(i&TCP_SELECT_SERVER))&&(i&TCP_0_OPEN))
					{//�ͻ���ģʽ
					  fd=ms->link_send(UARTCtrl->TCPSocket, (void *)p8s,x, MSG_DONTWAIT);
						if(fd==0)
						{//���ӱ��Ͽ�
						  UARTCtrl->LinkTask=93;
					 	  UARTCtrl->res4&=~TCP_0_OPEN;
							return;
						}
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
					ms->link_printf("tcplink_send_socket=%d\n",UARTCtrl->TCPSocket,0,0);
					ms->link_printf("link_send_return=%d\n",fd,0,0);
					if(fd<0)
					{
					  err=ms->link_errno();
						ms->link_printf("link_send_error=%d\n",err,0,0);
						if(err==ENOTSOCK)
						{
							
						}
					}
					if(fd>0)
					{
					  ms->link_printf("link_send_data------------------------\n",0,0,0);
						log_out((u32)p8s,fd, 'x');
					}
#endif
					}
					else
					{
#if ((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("-------------reconnect socket-------------\n",0,0,0);
#endif
					  UARTCtrl->LinkTask=93;
						return;
					}

          if(fd>0)
					{
						Terminal_Ram->CopyTxByte-=fd;
						Terminal_Ram->CopyTxCount+=fd;
						Terminal_Ram->ENETLED_MS_Timer=50/10;//14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
					}
					else if(fd<0)
					{
					  err=ms->link_errno();
						if((err==EINTR)||(err==EAGAIN))
						{
							fd=0;
						}
						else
						{
						  UARTCtrl->LinkTask=93;
					 	  UARTCtrl->res4&=~TCP_0_OPEN;
							return;
						}
						
					}
				}
				else
				{//UDP
//��ʱ�������������
//					x=1440;
//					if(x>UARTCtrl->TxByte)
//					{
//						x=UARTCtrl->TxByte;
//					}
//					p8s=(u8*)Get_ADDR_UARTnTx(GPRSPORT);
//					p8s+=UARTCtrl->TxCount;
//					UARTCtrl->TxByte-=x;
//					UARTCtrl->TxCount+=x;
//				#if MainProtocol==376
//					MR((u32)&addr.sin_addr.s_addr,ADDR_AFN04F3,4);
//					i=MRR(ADDR_AFN04F3+4,2);
//					i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
//					addr.sin_port = i;//�˿ں�:����	
//				#endif
//				#if MainProtocol==698
//					i=(u32)Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
//					if(i)
//					{
//						MR((u32)&addr.sin_addr.s_addr,i+2,4);
//					}
//					else
//					{
//						MWR(192+(168<<8)+(1<<16)+(3<<24),(u32)&addr.sin_addr.s_addr,4);
//					}
//					i=(u32)Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
//					if(i)
//					{
//						i=MRR(i+1,2);
//						addr.sin_port = i;//�˿ں�:����	
//					}
//					else
//					{
//						i=1024;
//						i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
//						addr.sin_port = i;//�˿ں�:����	
//					}
//				#endif
//				#if MainProtocol==CSG
//				/*
//					MR((u32)&addr->addr[0],ADDR_AFN04F3,4);
//					MR((u32)&addr->port,ADDR_AFN04F3+4,2);
//				*/
//				#endif
//					ms->link_sendto(UARTCtrl->UDPSocket, (void *)p8s,x, MSG_DONTWAIT,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
//					Terminal_Ram->ENETLED_MS_Timer=50/10;//14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
				}
			}
			else
			{
				if(UART1Ctrl->HostACK==8)
				 {
					 UART1Ctrl->HostACK=0;
				 }
				 if(UARTCtrl->Task==3)
				{
					UARTCtrl->Task=7;//7=�ȴ���������תΪ����
				}
				else
				{
				#if MainProtocol==376
					UARTCtrl->Task=4;//4=��֡�������������
				#else
					UARTCtrl->Task=7;//7=�ȴ���������תΪ����
				#endif
				}
			}
			break;
	}
	if((UARTCtrl->Task==2)||(UARTCtrl->Task==3)||(UARTCtrl->Task==6))
	{
	  return;
	}
  Uart_698_RxTx(GPRSPORT);//UART_698���շ���
#else
//AT���շ���
	if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
	{
		#if (USER/100)==14
		if(UART1Ctrl->ATTask==1)
		{
			if(UART1Ctrl->ATCount>AT_AT)
			{
				FJ_TASK(GPRSPORT);
				if(UART1Ctrl->TaskID!=0)
				{
					return;
				}
			}
		}
	  #endif
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

#endif
	return;
}

#if PPP_ENABLE==1
void log_out(u32  ADDR,u32 Byte, char x)//
{
	u8 *p8str;
#if DEBUG==1//0=����,1=����
	u32 i;
	ms_Type * ms;
	ms=Get_ms();
	i=0;
	p8str=(u8*)ADDR;
	while(i<Byte)
	{
	  switch(x)
		{
		case 'd':
		ms->link_printf("%4d",p8str[0],0,0);
		break;
		case 'c':
		ms->link_printf("%c",p8str[0],0,0);
		break;
		case 'x':
		ms->link_printf("%4x",p8str[0],0,0);
		break;
		}
		i++;
		p8str++;
	}
	ms->link_printf("\n",0,0,0);
#else
	if(Byte>2000)
	{
		Byte=2000;
	}
  p8str=(u8*)Get_ADDR_UARTnTx(RS232PORT);
	MR((u32)p8str,ADDR,Byte);
	UART0Ctrl->TxByte=Byte;
	Init_UARTn(RS232PORT);//��ʼ��UART��,���ͨ�ſ�����������
	Start_UARTn_Tx(RS232PORT);//����UART�ڷ���
	UART0Ctrl->Task=3;//3=��֡�������ڷ���
	while(UART0Ctrl->Task!=0)//0=���ڽ���,1=�������,2=�ȴ���֡���ݷ���,3=��֡�������ڷ���
	{
		switch(UART0Ctrl->Task)
		{
			case 1://1=�������
				UART0Ctrl->Task=0;//0=���ڽ���
				break;
			case 2://2=�ȴ���֡���ݷ���
				UART0Ctrl->Task=3;//���ڷ��ͱ�־
				Start_UARTn_Tx(RS232PORT);//����UART�ڷ���
				break;
			case 3://3=��֡�������ڷ���
				if(Wait_UARTn_Tx(RS232PORT)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
				{			
					UART0Ctrl->Task=0;//0=���ڽ���
					UART0Ctrl->RxByte=0;//rx����=0
				}
				break;
		}
	}
	#endif
	return;
}
void Myprint_ATRx(void)
{
	u32 Byte;
	u32 p0;
	u32 p1;
	u8 *p8d;
	u16 *p16;
	ms_Type * ms;
	ms=Get_ms();
	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		return;
	}
	if(p1>=(LEN_UART1IntATRx-10))
	{
		return;
	}
	if(p0>=p1)
	{
		Byte=p0-p1;
	}
	else
	{
		Byte=(p0+(LEN_UART1IntATRx-10))-p1;
	}
	if(Byte<2)
	{
	  return;
	}
	p8d=(u8*)ADDR_UART1IntATRx+10+p1;
	while(Byte--)
	{
		ms->link_printf("%c",p8d[0],0,0);
		p8d++;
		if(((u32)p8d)>=ADDR_UART1IntATRx+LEN_UART1IntATRx)
		{
			p8d=(u8*)ADDR_UART1IntATRx+10;
		}
	}
	return;
}
void tcp_log_start(u32 mode ,void *p)
{
	u8 *str;
	int fd;
	ms_Type * ms;
	ms=Get_ms();
	str=(u8 *)malloc(200);
	fd=ms->link_open("/var/run/tcp_log",O_RDWR|O_CREAT|O_APPEND);
	switch(mode)
	{
		case 1://����
		CopyString((u8 *)"����SOCKET",str);
		ms->link_write(fd,str,sizeof("����SOCKET"));
		ms->link_write(fd,p,sizeof(s32));
		break;
		case 2://��
		CopyString((u8 *)"��SOCKET",str);
		ms->link_write(fd,str,sizeof("��SOCKET"));
		ms->link_write(fd,p,sizeof(struct sockaddr));
		break;
		case 3://����
		CopyString((u8 *)"����SOCKET",str);
		ms->link_write(fd,str,sizeof("����SOCKET"));
		ms->link_write(fd,p,sizeof(struct sockaddr));
		break;
		case 4://����
		CopyString((u8 *)"����SOCKET",str);
		ms->link_write(fd,str,sizeof("����SOCKET"));
		ms->link_write(fd,p,sizeof(struct sockaddr));
		break;
		case 5://��
		break;
		case 6://д
		break;
	}
	ms->link_close(fd);
	free(str);
}
void tcp_log_end(u32 mode ,u32 ack)
{
	u8 *str;
	int fd;
	ms_Type * ms;
	ms=Get_ms();
	str=(u8 *)malloc(200);
	fd=ms->link_open("/var/run/tcp_log",O_RDWR|O_CREAT|O_APPEND);
	switch(mode)
	{
		case 1://����
		CopyString((u8 *)"����SOCKET",str);
		ms->link_write(fd,str,sizeof("����SOCKET"));
		ms->link_write(fd,&ack,sizeof(s32));
		break;
		case 2://��
		CopyString((u8 *)"��SOCKET",str);
		ms->link_write(fd,str,sizeof("��SOCKET"));
		ms->link_write(fd,&ack,sizeof(s32));
		break;
		case 3://����
		CopyString((u8 *)"����SOCKET",str);
		ms->link_write(fd,str,sizeof("����SOCKET"));
		ms->link_write(fd,&ack,sizeof(s32));
		break;
		case 4://����
		CopyString((u8 *)"����SOCKET",str);
		ms->link_write(fd,str,sizeof("����SOCKET"));
		ms->link_write(fd,&ack,sizeof(s32));
		break;
		case 5://��
		break;
		case 6://д
		break;
	}
	ms->link_close(fd);
	free(str);
}
u32 Search_String(u32 ADDR_String,u32 ADDR_T,u32 Len)//AT�������������;���:ADDR_StringΪҪ�ҵ���0�������ַ��������ݴ���ַ,����:0=û�ҵ�,!=0Ϊ�ҵ���ͬ����ĵ�1���ֽڵ�ַ
{
	u32 i;
	u32 offset;
	u32 x;
	u32 Byte;
	u8 *p8s;
	u8 *p8d;
	
  offset=0;//�Ѿ�ƫ�Ƶĸ���
	p8s=(u8*)(ADDR_T);
	p8d=(u8*)(ADDR_String);
	Byte=0;//�ַ����Ƚ��ֽ���
	while(p8d[Byte]!=0)
	{
		Byte++;
	}
	while(offset<Len)
	{
		if((Len-offset)<Byte)
		{
			return 0;
		}
		x=offset;
		for(i=0;i<Byte;i++)
		{
			if(p8s[x]!=p8d[i])
			{
				break;
			}
			x++;
		}
		if(i==Byte)
		{
			return (((u32)p8s)+x);
		}
		offset++;
	}
	return 0;
}


u32 Check_pppd_exist(void)//���ڷ���1�������ڷ���0
{
  int fd;
//	int err;
	u32 x;
	u32 i;
	u8 * p8s;
  ms_Type * ms;
	
	ms=Get_ms();
	if((fd=ms->link_open("/tmp/pppdpid",O_RDWR))<0)
	{//���ļ�
	  ms->link_system((char*)"cat /dev/null > /tmp/pppdpid");
		ms->link_system((char*)"/bin/ps -w | /bin/grep \"pppd-config$\" > /tmp/pppdpid");
		fd=ms->link_open("/tmp/pppdpid",O_RDWR);
	}
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("function=Check_pppd_exist\n",0,0,0);
//			ms->link_system((char*)"/bin/ps -w");
#endif
	if(fd>=0)
	{//
		p8s=(u8*)ADDR_DATABUFF;
		x=CopyString((u8 *)"/proc/",p8s);
		p8s+=x;
		ms->link_lseek(fd,0,SEEK_SET);//��λ����ʼ
		if((ms->link_read(fd,p8s,5))!=5)//��ȡpppd-pid
		{
		  ms->link_close(fd);
		  return 0;
		}
		ms->link_close(fd);
		for(i=0;i<5;i++)
		{
			if(p8s[i]!=' ')
			{
				MR((u32)p8s,(u32)p8s+i,5-i);
				break;
			}
		}
		p8s+=5-i;
		x=CopyString((u8 *)"/status",p8s);
		p8s[x]=0;
		if((fd=ms->link_open((char*)ADDR_DATABUFF,O_RDWR))<0)
		{//·����������
			return 0;
		}
		ms->link_lseek(fd,0,SEEK_SET);//��λ����ʼ
		if((ms->link_read(fd,(u8*)ADDR_DATABUFF,13))!=13)//��ȡpppd-pid
		{
			ms->link_close(fd);
			UART1Ctrl->LinkTask=88;
			return 0;
		}
		ms->link_close(fd);
		i=Search_String((u32)(u8*)"pppd",ADDR_DATABUFF,13);
		if(!i)
		{//û�ҵ�
			return 0;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
u32 KILL_pppd(void)
{
  int fd;
	u32 x;
  ms_Type * ms;
	
	ms=Get_ms();
	if((fd=ms->link_open("/tmp/pppdpid",O_RDWR))<0)
	{//���ļ�
	  ms->link_system((char*)"cat /dev/null > /tmp/pppdpid");
		ms->link_system((char*)"/bin/ps -w | /bin/grep \"pppd-config$\" > /tmp/pppdpid");
		fd=ms->link_open("/tmp/pppdpid",O_RDWR);
	}
	if(fd>=0)
	{//
	  x=CopyString((u8 *)"/bin/kill ",(u8*)ADDR_DATABUFF);
		MC(0,ADDR_DATABUFF+x,7);
		ms->link_lseek(fd,0,SEEK_SET);//��λ����ʼ
		if((ms->link_read(fd,(u8*)ADDR_DATABUFF+x,5))!=5)//��ȡpppd-pid
		{
		  ms->link_close(fd);
			ms->link_system((char*)"chat -s -v -f /etc/ppp/peers/chat-disconnect");
		  return 0;
		}
		ms->link_close(fd);
		ms->link_system((char*)ADDR_DATABUFF);
	}
	return 1;
}
void PPP_RxTx(void)
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8* p8tx;
	ms_Type * ms;
//	ms_Type * ms2;
	struct sockaddr_in addr;
	int fd;
	int err;
	u32 TxByte;
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(GPRSPORT));
	
	ms=Get_ms();
	switch(UARTCtrl->LinkTask)
	{
	  case 88://�˳�͸��ģʽ/kill $(cat /var/tmp/ppp0.pid)
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
		ms->link_printf("happen err\n",0,0,0);
#endif
      i=Check_pppd_exist();//���ڷ���1�������ڷ���0
			if(i)
			{
			  KILL_pppd();
			}
			i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
			UARTCtrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
			if(UARTCtrl->res3<i)//res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������
			{//��û�г��������е�APN������
				UARTCtrl->res3++;
				UARTCtrl->ATCount=AT_MYNETCON_APN;
				UARTCtrl->LinkCount=0;
				UARTCtrl->LinkTask=8;
				return;
			}
			else
			{
			  UARTCtrl->LinkTask=104;
				return;
			}
		case 89://�򿪴���
		  WarningOccur(GPRSPORT,(u8 *)"��鲦�Ŵ���");//�澯����,��ڸ澯����͸澯�ַ���
		#if((DEBUG==1)&&(DEBUG_GPRS==1))
		  ms->link_system((char*)"/bin/ls -l /dev/ttyUSB*");
		#endif
		  i=Check_pppd_exist();//���ڷ���1�������ڷ���0
			if(i)
			{
			  KILL_pppd();
			}
			p8=(u8*)ADDR_DATABUFF;
			fd=ms->link_open("/etc/ppp/peers/pppd-config",O_RDWR);
			if(fd>=0)
			{
			  ms->link_lseek(fd,0,SEEK_SET);
			  ms->link_read(fd,p8,20);
				ms->link_close(fd);
				if(!Compare_DL698DataString((u8*)ADDR_4500_5+4,(u8*)"HW",2))//link_write
				{
				  x=Search_String((u32)(u8*)"ttyUSB0",(u32)p8,20);
					if(!x)
					{//û�ҵ�
					  ms->link_system((char*)"/bin/sed -i '1,1c/dev/ttyUSB0 9600' /etc/ppp/peers/pppd-config");
					}
					fd=ms->link_open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
					ms->link_system((char*)"/bin/rm -r /var/lock/LCK..ttyUSB0");
				}
				else if(!Compare_DL698DataString((u8*)ADDR_4500_5+4,(u8*)"GSCN",4))
				{
				  x=Search_String((u32)(u8*)"ttyUSB2",(u32)p8,20);
					if(!x)
					{//û�ҵ�
					  ms->link_system((char*)"/bin/sed -i '1,1c/dev/ttyUSB2 9600' /etc/ppp/peers/pppd-config");
					}
					fd=ms->link_open("/dev/ttyUSB2",O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
					ms->link_system((char*)"/bin/rm -r /var/lock/LCK..ttyUSB2");
				}
				else if(!Compare_DL698DataString((u8*)ADDR_4500_5+4,(u8*)"ZXGD",4))
				{
				  x=Search_String((u32)(u8*)"ttyUSB2 115200",(u32)p8,20);
					if(!x)
					{//û�ҵ�
					  ms->link_system((char*)"/bin/sed -i '1,1c/dev/ttyUSB2 115200' /etc/ppp/peers/pppd-config");
					}
					fd=ms->link_open("/dev/ttyUSB2",O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
					ms->link_system((char*)"/bin/rm -r /var/lock/LCK..ttyUSB2");
				}
				else
				{
				  fd=ms->link_open("/dev/ttyUSB2",O_RDWR|O_NOCTTY|O_NONBLOCK|O_NDELAY);
					ms->link_system((char*)"/bin/rm -r /var/lock/LCK..ttyUSB*");
				}
			}
			if(fd<0)
			{//���ڴ�ʧ��
			  UARTCtrl->LinkTask=8;
			  UART1Ctrl->ATTask=1;//AT����:0=����,1=����,2=���ڷ���,3=����
				UART1Ctrl->ATCount=AT_MYPOWEROFF;//�ػ�
			}
			else
			{
			  ms->link_close(fd);
			  UARTCtrl->LinkTask++;
				UART1Ctrl->LinkCount=0;
			}
			return;
		case 90://ִ�в��ų���,ͨ��SYSINFO��ȡ�����������ͣ��·��û������� 
//		pppd  %s 115200 mru 1280 nodetach debug dump %s defaultroute usepeerdns novj  user \"%s\" password\"%s\"  novjccomp noipdefaultipcp-accept-local ipcp-accept-remote connect-delay 5000 ipcp-max-failure 60ipcp-max-configure 60 -am
//    pppd user "card" password "card" file /etc/ppp/peers/chint-config   
//    TxByte=CopyString((u8 *)"/usr/sbin/pppd   file /etc/ppp/peers/pppd-config&",p8tx);
		 WarningOccur(GPRSPORT,(u8 *)"ִ�в��ų���");//�澯����,��ڸ澯����͸澯�ַ���
		 ms->link_system((char*)"/bin/mkdir /var/lock");
		 ms->link_system((char*)"/bin/rm -r /var/run/ppp*");
		#if((DEBUG==1)&&(DEBUG_GPRS==1))
		  ms->link_system((char*)"/bin/ls -l /var/lock");
			ms->link_system((char*)"/bin/ls -l /var/run");
		#endif
		  p8tx=(u8*)ADDR_DATABUFF;
			UARTCtrl->LinkTask++;
		  TxByte=CopyString((u8 *)"/usr/sbin/pppd  user \"",p8tx);
		  i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
			if(i==0)
			{//����ͨ����=0
				p8=Get_Element((u8 *)ADDR_4500_2,7,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8)
				{//�ҵ��û���
					i=p8[1];//�ֽ���
					p8+=2;
					MR((u32)p8tx+TxByte,(u32)p8,i);
					TxByte+=i;
				}
				TxByte+=CopyString((u8 *)"\" password \"",p8tx+TxByte);
				p8=Get_Element((u8 *)ADDR_4500_2,8,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8)
				{//�ҵ�����
					i=p8[1];//�ֽ���
					p8+=2;
					MR((u32)p8tx+TxByte,(u32)p8,i);
					TxByte+=i;
				}
			}
			else
			{//�б���ͨ����
				MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
				p8=(u8*)ADDR_128KDATABUFF;
				x=p8[1];//ȡ����ͨ����
				p8+=2;
				y=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������,������APNʱ�Ѿ�ȷ��
				for(i=0;i<y;i++)//�Թ��Ѿ����͵��û�������
				{
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
						MR((u32)p8tx+TxByte,(u32)p8,i);
						TxByte+=i;
						p8+=i;
					}
					//����
					TxByte+=CopyString((u8 *)"\" password \"",p8tx+TxByte);
					if(p8[0]==DataType_visible_string)
					{
						i=p8[1];//�ֽ���
						p8+=2;
						MR((u32)p8tx+TxByte,(u32)p8,i);
						TxByte+=i;
					}
				}
			}
			p8tx[TxByte++]='\"';
			TxByte+=CopyString((u8 *)"  file /etc/ppp/peers/pppd-config&",p8tx+TxByte);
			p8tx[TxByte]=0;
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			log_out(ADDR_DATABUFF,TxByte, 'c');//
			ms->link_printf("\n",0,0,0);
#endif
			ms->link_system((char*)ADDR_DATABUFF);
			UART1Ctrl->LinkCount++;
			Terminal_Ram->GPRSWaitConnection_S_Timer=40;//25 GPRS�������ӷ���ȴ��붨ʱ��
			p16timer[0]=4000/10;
			return;
		case 91://��ѯ���Ž��,���ʧ�ܻ��߳�ʱ
		  if(p16timer[0]!=0)
			{
				return;
			}
			if(Terminal_Ram->GPRSWaitConnection_S_Timer==0)
			{
			  UARTCtrl->LinkTask=88;
				break;
			}
			p16timer[0]=1000/10;
			ms->link_system((char*)"cat /dev/null > /tmp/pppdpid");
			ms->link_system((char*)"/bin/ps -w | /bin/grep \"pppd-config$\" > /tmp/pppdpid");
			fd=ms->link_open("/tmp/pppdpid",O_RDWR);
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("fd=%d\n",fd,0,0);
			ms->link_system((char*)"/bin/cat /tmp/pppdpid");
#endif
      if(fd<0)
			{//
			  UARTCtrl->LinkTask=88;
				break;
			}
			ms->link_lseek(fd,0,SEEK_SET);//��λ����ʼ
			err=ms->link_read(fd,(u8*)ADDR_DATABUFF,5);//��ȡpppd-pid
			ms->link_close(fd);
			if(err<=0)
			{//�����ݣ������Ѿ���ֹ
				if(UART1Ctrl->LinkCount>=2)
				{
					UARTCtrl->LinkTask=88;
					UART1Ctrl->LinkCount=0;
				}
				else
				{
					UARTCtrl->LinkTask=90;
				}
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			ms->link_printf("could not find pppd pid \n",0,0,0);
				ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
#endif
			  break;
			}
//			ms->link_system((char*)"/bin/rm -r /var/run/pppdstat");
//		 ms->link_system("/bin/grep -o -E \"IPLOCAL=[0-9].+;\" /var/run/pppd2.tdb > /var/run/ppp0.ip");
		  WarningOccur(GPRSPORT,(u8 *)"�ȴ��������");//�澯����,��ڸ澯����͸澯�ַ���
		#if((DEBUG==1)&&(DEBUG_GPRS==1))
				ms->link_system((char*)"/sbin/ifconfig");
		#endif
        ms->link_system((char*)"cat /dev/null > /tmp/ppp0.ip");
				ms->link_system((char*)"/sbin/ifconfig ppp0|/bin/grep -o -E \"inet addr:[0-9].+\" > /tmp/ppp0.ip");
				fd=ms->link_open("/tmp/ppp0.ip",O_RDWR);
				if(fd<0)
				{
					break;
				}
				ms->link_lseek(fd,0,SEEK_SET);//��λ����ʼ
				i=ms->link_read(fd,(u8*)ADDR_DATABUFF,30);//inet addr:255.255.255.255;
				ms->link_close(fd);
				if(i==0)
				{//��û���ҵ�IP��ַ
				  break;
				}
				//ms->link_system((char*)"/bin/rm  /tmp/ppp0.ip");
				i=Search_String((u32)(u8*)"inet addr:",ADDR_DATABUFF,30);
				if(!i)
				{//û���ҵ�IP��ַ
					break;
				}
				x=Search_String((u32)(u8*)" ",i,17);
				if(!x)
				{//�ҵ��ո�
					break;
				}
				x--;
				x-=i;
				//����IP��ַ��RAM����
				CopyString((u8 *)"\" ",(u8*)ADDR_PPP0_IP4ADDR_ASCII);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				MR(ADDR_PPP0_IP4ADDR_ASCII+1,i,x);//
				CopyString((u8 *)"\"",(u8*)ADDR_PPP0_IP4ADDR_ASCII+x+1);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				MC(0,ADDR_PPP0_IP4ADDR_ASCII+x+2,1);
				
				CopyString((u8 *)"��IP",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				MR(ADDR_DATABUFF+4,ADDR_PPP0_IP4ADDR_ASCII,19);
				WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ�
				UARTCtrl->LinkTask++;
			return;
		case 92:// ��ȡppp0��IP��ַ����ȷ������ģʽ
		  //WarningOccur(GPRSPORT,(u8 *)"��ȡ�ն˹���ģʽ");//�澯����,��ڸ澯����͸澯�ַ���
			MR(ADDR_128KDATABUFF,ADDR_4500_2+2,6);
			p8=(u8*)ADDR_128KDATABUFF;
			i=p8[1]<<4;//B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ
			i|=p8[5]<<7;//B7=0TCP=1UDP
			UARTCtrl->TCPUDP=i;
#if((DEBUG==1)&&(DEBUG_GPRS==1))
			log_out(ADDR_PPP0_IP4ADDR_ASCII,20, 'c');//
			ms->link_printf("\n",0,0,0);
//			ms->link_system("/sbin/ifconfig eth0 down");
	  	ms->link_system("/bin/ps -w");
		  ms->link_system("/sbin/ifconfig");
	  	ms->link_system("/bin/ip route show");
#endif
			UARTCtrl->LinkTask++;
		  UARTCtrl->LinkCount=0;
			Svr_Skt->ppp0_subtask=0;
			return;
		case 93://�ͻ���ģʽ�µ�socket����
		  //WarningOccur(GPRSPORT,(u8 *)"�ͻ���ģʽ������վ");//�澯����,��ڸ澯����͸澯�ַ���
#if((DEBUG==1)&&(DEBUG_GPRS==1))
		switch(Svr_Skt->ppp0_subtask)
		{
			case 3:
				if(p16timer[0]==0)
				{
					ms->link_printf("\n",0,0,0);
					ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
					ms->link_printf("Svr_Skt->ppp0_subtask=%d\n",Svr_Skt->ppp0_subtask,0,0);
				}
				break;
			default:
				ms->link_printf("\n",0,0,0);
        ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
			  ms->link_printf("Svr_Skt->ppp0_subtask=%d\n",Svr_Skt->ppp0_subtask,0,0);
				break;
		}

#endif
		  i=UARTCtrl->TCPUDP&0x30;
      if((i==0x00)||(i==0x10))////B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ
			{//���ģʽ�»��߿ͻ���ģʽ
				if(UARTCtrl->TCPUDP&0x80)////B7=0TCP=1UDP
				{//UDP
				  
				}
				else
				{//TCP
			//		ms2->link_printf("\n",0,0,0);
					switch(Svr_Skt->ppp0_subtask)
					{
					  case 0://�ȼ��ԭ����socket�Ƿ���ڣ����������close
						{
							if(UARTCtrl->res4&TCP_0_OPEN)
							{//�ͻ���socket�Ѿ�����
							  UARTCtrl->LinkTask++;
								break;
							}
							if(UARTCtrl->TCPSocket>2)
							{
								ms->link_close(UARTCtrl->TCPSocket);
							}
							UARTCtrl->TCPSocket=0;
							Svr_Skt->ppp0_subtask++;
						}
						break;
						case 1://����socket
						{
							UARTCtrl->TCPSocket=ms->link_socket(AF_INET,SOCK_STREAM,0);//TCP=SOCK_STREAM ,UDP=SOCK_DGRAM
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprslink_socket=%d\n",UARTCtrl->TCPSocket,0,0);
				#endif
							if(UARTCtrl->TCPSocket<0)
							{
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							 i=ms->link_errno();
							 ms->link_printf("gprslink_socketerror=%d\n",i,0,0);
				#endif	
								UARTCtrl->LinkTask=88;
								return;
							}
							if(UARTCtrl->TCPSocket<3)
							{
								return;
							}
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("to set socket ms->link_fcntl\n",0,0,0);
			 ms->link_printf("gprslink_socket=%d\n",UARTCtrl->TCPSocket,0,0);   
			 ms->link_printf("F_GETFL=%d\n",F_GETFL,0,0);
#endif
				fd=ms->link_fcntl(UARTCtrl->TCPSocket,F_GETFL,0);//��ȡ�ļ�������������
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("ms->link_fcntl=%d\n",fd,0,0);
#endif
							fd=ms->link_fcntl(UARTCtrl->TCPSocket, F_SETFL, fd | O_NONBLOCK);	//���ļ�����������������Ϊ������
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("ms->link_fcntl=%d\n",fd,0,0);
#endif
							if ((err=ms->link_setsockopt(UARTCtrl->TCPSocket, SOL_SOCKET, SO_BINDTODEVICE, (char *)"ppp0", 5))<0)
							{
#if((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("ms->link_setsockopt=%d\n",err,0,0);
#endif
									ms->link_close(UARTCtrl->TCPSocket);
									UARTCtrl->TCPSocket=0;
									UARTCtrl->LinkTask=88;
								return;
							}
							Svr_Skt->ppp0_subtask++;
						}
						break;
						case 2://bind��ַ
						{
							addr.sin_family = AF_INET;// ʹ�� IPv4 Э��
							
							UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
							x=UARTCtrl->PORT;//��ǰppp���ŵ�PORT
							UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
							i=UARTCtrl->PORT;
							if(i<SocketPortnMin)
							{
								i=SocketPortnMin;
							}
							i++;
							if(i==x)
							{
								i++;
							}
							x=MRR(ADDR_4500_2+13,2);//��ȡ�����˿�
							x=R_Inverse(x,2);
							if(i==x)//����������˿��ظ����������1
							{
								i++;
							}
							if((i<SocketPortnMin)||(i>SocketPortnMax))
							{
								i=SocketPortnMin;
							}
							UARTCtrl->PORT=i;
							i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							addr.sin_port = i;//�˿ں�:����
							i=IP_ASCII_HEX(ADDR_PPP0_IP4ADDR_ASCII);//ASCIIתΪHEX��IP;��ڿ�Ϊָ���ASCII����0-9�ĵ�ַ;����0��ʾ��IP��ַ
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("addr.sin_addr.s_addr =%x\n",i,0,0);
				#endif
							addr.sin_addr.s_addr = i;//IP��ַ��ע:�ֽ�˳�� "192.168.0.1" = 0x0100A8C0
							fd=ms->link_bind(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprs_link_bind_return =%d\n",fd,0,0);
							ms->link_printf("addr.sin_addr.s_addr =%x\n",addr.sin_addr.s_addr,0,0);
							ms->link_printf("addr.sin_port =%x\n",addr.sin_port,0,0);
				#endif
							if(fd)
							{//���óɹ�ʱ����0��ʧ��ʱ����-1
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							 err=ms->link_errno();
							 ms->link_printf("gprslink_binderror=%d\n",err,0,0);
				#endif
				        ms->link_close(UARTCtrl->TCPSocket);
								UARTCtrl->TCPSocket=0;
								UARTCtrl->LinkTask=88;
								return;
							}
							Svr_Skt->ppp0_subtask++;
							UARTCtrl->NETOPENCount=0;
							p16timer[0]=0;
							Terminal_Ram->GPRSWaitConnection_S_Timer=20;//connect��ʱʱ��
						}
						break;
						case 3://conectԶ�̷�����
						{
						  if(p16timer[0]!=0)
							{
								return;
							}
							//��ȡ��վIP��������
							i=MRR(ADDR_4520_2+1,1);//ȡ����ͨ����
							if(i==0)
							{//����ͨ����=0
								if(UART1Ctrl->LinkCount>=GPRSNETTOTALCOUNT)
								{
									UART1Ctrl->LinkCount=0;
								}
								i=MRR(ADDR_4500_3+1,1);
								if((UART1Ctrl->LinkCount<=GPRSNETMAINIPCOUNT)||(i<2))
								{//��ip
									p8=Get_Element((u8*)ADDR_4500_3,1,1,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
									if(p8)
									{
										MR((u32)&addr.sin_addr.s_addr,(u32)p8+2,4);
										p8=Get_Element((u8*)ADDR_4500_3,1,2,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
										if(p8)
										{
											i=MRR((u32)p8+1,2);
											addr.sin_port = i;//�˿ں�:����
										}
									}
								}
								else
								{//����ip
									p8=Get_Element((u8*)ADDR_4500_3,2,1,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
									if(p8)
									{
										MR((u32)&addr.sin_addr.s_addr,(u32)p8+2,4);
										p8=Get_Element((u8*)ADDR_4500_3,2,2,LENmax_4500_3);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
										if(p8)
										{
											i=MRR((u32)p8+1,2);
											addr.sin_port = i;//�˿ں�:����
										}
									}
								}
							}
							else
							{//�б���ͨ����
								MR(ADDR_128KDATABUFF,ADDR_4520_2,LENmax_4520_2);
								p8=(u8*)ADDR_128KDATABUFF;
								x=p8[1];
								p8+=2;
								y=UART1Ctrl->res3;//����Զ��ģ������ʹ��45200200����ʱ���Ѿ��Թ��Ĳ�������,������APNʱ�Ѿ�ȷ��
								for(i=0;i<y;i++)//�Թ��Ѿ����͵��û�������
								{
									p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
								}
								//��ʱp8ָ��4520���ض�ͨѶ��������վIP�����б�
								if(i<x)
								{//�ҵ�
									if(UART1Ctrl->LinkCount>=GPRSNETTOTALCOUNT)
									{
										UART1Ctrl->LinkCount=0;
									}
									p8=Get_Element(p8,8,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
									if(p8)
									{
										i=p8[1];//��վͨ�Ų��� array��
										if((UART1Ctrl->LinkCount<=GPRSNETMAINIPCOUNT)||(i<2))
										{//��ip
											p8+=2;
											MR((u32)&addr.sin_addr.s_addr,(u32)p8+4,4);
											p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
											if(p8)
											{
												i=MRR((u32)p8+1,2);
												addr.sin_port = i;//�˿ں�:����
											}
										}
										else
										{//����ip
											p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
											if(p8)
											{
												MR((u32)&addr.sin_addr.s_addr,(u32)p8+4,4);
												p8=Get_Element(p8,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
												if(p8)
												{
													i=MRR((u32)p8+1,2);
													addr.sin_port = i;//�˿ں�:����
												}
											}
										}
									}
								}
							}
							
							addr.sin_family = AF_INET;// ʹ�� IPv4 Э��
							fd=ms->link_connect(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprs_link_connect_sockaddr =%x\n",addr.sin_addr.s_addr,0,0);
							ms->link_printf("gprs_link_connect_port =%x\n",addr.sin_port,0,0);
				#endif
							if(fd)
							{
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							 err=ms->link_errno();
							 ms->link_printf("gprslink_connecterror=%d\n",err,0,0);
				#endif
								err=ms->link_errno();
								if(err==EINPROGRESS)
								{//����������
								  p16timer[0]=1000/10;//�´μ�ⶨʱ
									MC(0,ADDR_DATABUFF+14,2);
									CopyString((u8 *)"�� ��������վ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									BCD_ASCII(UART1Ctrl->LinkCount+1,ADDR_DATABUFF+2);
			            WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
									return;
								}
								else if(err==EISCONN)
								{//�Ѿ�������
									UARTCtrl->res4|=TCP_0_OPEN;
								  Svr_Skt->ppp0_subtask=0;
							    UARTCtrl->LinkTask++;
								  break;
								}
								else if(err==EALREADY)
								{//
								  p16timer[0]=1000/10;//�´μ�ⶨʱ
									MC(0,ADDR_DATABUFF+14,2);
									CopyString((u8 *)"�� ��������վ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									BCD_ASCII(UART1Ctrl->LinkCount+1,ADDR_DATABUFF+2);
			            WarningOccur(GPRSPORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
									return;
								}
								UART1Ctrl->LinkCount++;
								UARTCtrl->NETOPENCount++;//���������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
				#if((DEBUG==1)||(DEBUG_GPRS==1))
							 ms->link_printf("gprslink_connect_UART1Ctrl->LinkCount=%d\n",UART1Ctrl->LinkCount,0,0);
							 ms->link_printf("gprslink_connect_UART1Ctrl->NETOPENCount=%d\n",UART1Ctrl->NETOPENCount,0,0);
				#endif
								WarningOccur(GPRSPORT,(u8 *)"������վʧ��");//�澯����,��ڸ澯����͸澯�ַ���
								if(UART1Ctrl->NETOPENCount>=GPRSNETOPENCOUNT)//��������������������(��ǰ�����ﵽx��ʱ��ģ���Դ)
								{
									UARTCtrl->LinkCount=0;
								  ms->link_close(UARTCtrl->TCPSocket);
									UARTCtrl->TCPSocket=0;
                  UARTCtrl->LinkTask=88;
								}
								return;
							}
							UARTCtrl->res4|=TCP_0_OPEN;
							Svr_Skt->ppp0_subtask=0;
							UARTCtrl->LinkTask++;
							UARTCtrl->LinkCount=0;
						}
						break;
						default:
						{
							if(UARTCtrl->TCPSocket>2)
							{
								ms->link_close(UARTCtrl->TCPSocket);
							}
							Svr_Skt->ppp0_subtask=0;
						}
						break;
					}
				}
			}
			return;
		case 94://���������������һ���߳������ͻ�������
		  //WarningOccur(GPRSPORT,(u8 *)"������ģʽ���������˿�");//�澯����,��ڸ澯����͸澯�ַ���
      i=UARTCtrl->TCPUDP&0x30;
#if((DEBUG==1)&&(DEBUG_GPRS==1))
      switch(Svr_Skt->ppp0_subtask)
			{
			  case 1:
					if(p16timer[0]==0)
					{
						ms->link_printf("\n",0,0,0);
						ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
						ms->link_printf("Svr_Skt->ppp0_subtask=%d\n",Svr_Skt->ppp0_subtask,0,0);
					}
				break;
				default:
				  ms->link_printf("\n",0,0,0);
          ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
			    ms->link_printf("Svr_Skt->ppp0_subtask=%d\n",Svr_Skt->ppp0_subtask,0,0);
				break;
			}
#endif
      if((i==0x00)||(i==0x20))////B5-B4=0���ģʽ=1�ͻ���ģʽ=2������ģʽ
			{//���ģʽ�»��߷�����ģʽ
			  if(UARTCtrl->TCPUDP&0x80)////B7=0TCP=1UDP
				{//UDP
				  
				}
				else
				{//TCP
				  switch(Svr_Skt->ppp0_subtask)
					{
					case 0://�ȼ��ԭ����socket�Ƿ���ڣ����������close
					{
					  if(UARTCtrl->res4&TCP_1_LISTEN_OPEN)
						{//����socket�Ѿ�����
							UARTCtrl->LinkTask++;
							break;
						}
						
						if(Svr_Skt->ppp0_clint>2)
						{
							ms->link_close(Svr_Skt->ppp0_clint);
						}
						Svr_Skt->ppp0_clint=0;
						//������ģ�飬����ֻ�ٴν�������socket
						if(Svr_Skt->ppp0_listen>2)
						{
							ms->link_close(Svr_Skt->ppp0_listen);
							p16timer[0]=120000/10*(UARTCtrl->LinkCount+1);//�´μ�ⶨʱ
						}
						Svr_Skt->ppp0_listen=0;
						Svr_Skt->ppp0_subtask++;
					}
					break;
					case 1://����socket
					{
						WarningOccur(GPRSPORT,(u8 *)"�ȴ����������˿�");//�澯����,��ڸ澯����͸澯�ַ���
					  if(p16timer[0]!=0)
						{
							return;
						}
						Svr_Skt->ppp0_listen=ms->link_socket(AF_INET,SOCK_STREAM,0);//TCP=SOCK_STREAM ,UDP=SOCK_DGRAM
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprs_svr_link_ppp0_listen=%d\n",Svr_Skt->ppp0_listen,0,0);
				#endif
						if(Svr_Skt->ppp0_listen<0)
						{
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							 err=ms->link_errno();
							 ms->link_printf("gprs_svr_link_socketerror=%d\n",err,0,0);
				#endif	
							UARTCtrl->LinkTask=88;
							return;
						}
						if(Svr_Skt->ppp0_listen<3)
						{
							return;
						}
						err=1;
						if ((err=ms->link_setsockopt(Svr_Skt->ppp0_listen, SOL_SOCKET, SO_REUSEADDR, &err, sizeof(err)))<0)
						{
#if((DEBUG==1)&&(DEBUG_GPRS==1))
		ms->link_printf("ms->link_setsockopt=%d\n",err,0,0);
#endif
								ms->link_close(Svr_Skt->ppp0_listen);
								Svr_Skt->ppp0_listen=0;
								UARTCtrl->LinkTask=88;
							return;
						}
						fd=ms->link_fcntl(Svr_Skt->ppp0_listen,F_GETFL,0);//��ȡ�ļ�������������
						ms->link_fcntl(Svr_Skt->ppp0_listen, F_SETFL, fd | O_NONBLOCK);	//���ļ�����������������Ϊ������
						Svr_Skt->ppp0_subtask++;
					}
					break;
					case 2://��socket
					{
						addr.sin_family = AF_INET;// ʹ�� IPv4 Э��
						i=MRR(ADDR_4500_2+13,2);//��ȡ�����˿�
						addr.sin_port = i;//�˿ں�:����
	//					i=IP_ASCII_HEX(ADDR_PPP0_IP4ADDR_ASCII);//ASCIIתΪHEX��IP;��ڿ�Ϊָ���ASCII����0-9�ĵ�ַ;����0��ʾ��IP��ַ
				    i=0;
						addr.sin_addr.s_addr = i;//IP��ַ��ע:�ֽ�˳�� "192.168.0.1" = 0x0100A8C0
						#if((DEBUG==1)&&(DEBUG_GPRS==1))
						ms->link_printf("gprs_svrlisten_socket_s_addr =%x\n",addr.sin_addr.s_addr,0,0);
						ms->link_printf("gprs_svrlisten_socket_s_listenport =%x\n",addr.sin_port,0,0);
				#endif
						fd=ms->link_bind(Svr_Skt->ppp0_listen,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
				#if((DEBUG==1)&&(DEBUG_GPRS==1))
							ms->link_printf("gprs_svr_link_bind_return =%d\n",fd,0,0);
				#endif
						if(fd)
						{//���óɹ�ʱ����0��ʧ��ʱ����-1
							err=ms->link_errno();
				#if((DEBUG==1)||(DEBUG_GPRS==1))
							 ms->link_printf("gprs_svr_link_binderror=%d\n",err,0,0);
				#endif
				      if((err==EADDRINUSE)&&(UARTCtrl->LinkCount<2)) /* Address already in use */
							{
							  Svr_Skt->ppp0_subtask=0;//�ڵ���0ʱ��ر�����socket������ʱ
								UARTCtrl->LinkCount++;
							}
							else
							{
								if(Svr_Skt->ppp0_listen>2)
								{
									ms->link_close(Svr_Skt->ppp0_listen);
								}
								Svr_Skt->ppp0_listen=0;
								UARTCtrl->LinkTask=88;
							}
							return;
						}
						Svr_Skt->ppp0_subtask++;
					}
					break;
					case 3://����sokcet
					{
						fd=ms->link_listen(Svr_Skt->ppp0_listen,5);
						if(fd)
						{//���óɹ�ʱ����0��ʧ��ʱ����-1
				#if((DEBUG==1)||(DEBUG_GPRS==1))
							 err=ms->link_errno();
							 ms->link_printf("gprs_svr_link_binderror=%d\n",err,0,0);
				#endif
				      if(Svr_Skt->ppp0_listen>2)
							{
								ms->link_close(Svr_Skt->ppp0_listen);
								Svr_Skt->ppp0_listen=0;
							}
							UARTCtrl->LinkTask=88;
							return;
						}
						UARTCtrl->res4|=TCP_1_LISTEN_OPEN;
						Svr_Skt->ppp0_subtask=0;
						UARTCtrl->LinkTask++;
					}
					break;
					default:
					if(Svr_Skt->ppp0_listen>2)
					{
						ms->link_close(Svr_Skt->ppp0_listen);
						Svr_Skt->ppp0_listen=0;
					}
					Svr_Skt->ppp0_subtask=0;
					break;
					}
				}
			}
			return;
		case 95:
#if((DEBUG==1)&&(DEBUG_GPRS==1))
       ms->link_printf("UARTCtrl->LinkTask=%d\n",UARTCtrl->LinkTask,0,0);
#endif
    // p16=(u16 *)(ADDR_HeartberatTimer);//������������붨ʱ��
	if((UART1Ctrl->Task)&&(Terminal_Ram->CopyTxByte))
	{//��������Ҫ����ʱ
		UARTCtrl->LinkTask=101;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	}
	else
	{
	  UARTCtrl->LinkTask=100;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
	}
		break;
		}
		return;
}
#endif

















