
#ifndef Project_H
#define Project_H


#define Project     200//����0=���ܱ�,100=�����,200=I�ͼ�����,300=ר���ն�,400=����һ�ն�,500=���ͼ�����,600=ͨ��ģ��,700=
//#define IC_STM32F4xx
#define IC_NUC9xx
#define LINUX


#define DEBUG     1//0=����,1=����
#define Enable_sysprintf     0//NUC97x OS��UART0ϵͳ�������; 0=��ֹ,1=����

#define USER     1700//901//1501//1501//1401//1600//1501//901//�û���ʶ
//0xx=ä�����Ժ�ͼ��
	//0=ä�����Ժ�ͼ��
//1xx=��׼ϵ
	//100=��׼��
//2xx=ɽ��ϵ
	//200=ɽ����
//3xx=
	//300=
//4xx=����ϵ
	//401=������
//5xx=�Ϻ�ϵ
	//501=�Ϻ���
//6xx=����ϵ
	//600=
	//601=���ϰ�
//7xx=�㽭ϵ
	//701=�㽭��
//8xx=ͨ���г�ϵ
	//801=ͨ���г���
//9xx=����ϵ
	//900=ʡ���Ժ̨����԰�(���ն��¼����ò���ͬ�������Ժ���԰�)
	//901=���ϰ�
//10xx=�ӱ�ϵ
	//1001=�ӱ���
//11xx=����ϵ
	//1101=���ְ�
//12xx=�Ĵ�ϵ
	//1200=ʡ���Ժ̨����԰�
	//1201=�Ĵ���
//13xx=

//14xx=����ϵ
	//1401=������
//15xx=����ϵ
	//1500=ʡ���Ժ̨����԰�
	//1501=���հ�
//16xx=����ϵ
	//1600=ʡ���Ժ̨����԰�
	//1601=���ɰ�
//17xx=����ϵ
	//1700=ʡ���Ժ̨����԰�
	//1701=���հ�


	
#define FactoryDI     0//�������̱�ʶ
//0=��̩
//1=

#ifdef IC_STM32F4xx
	#include "type.h"
	#include "stm32f4xx.h"
#endif
#ifdef IC_NUC9xx
 #ifndef LINUX
	#include "type.h"
	#include "../../NUC97x/Driver/Include/nuc970.h"
	#include "../../NUC97x/Driver/Include/sys.h"
 #else
		#define __disable_irq()    off_irq()
		#define __enable_irq()     destore_irq(0)

		#ifdef LINUX_DRIVE
		#define ADDR_ERAM_linux    0x02000000
		#else
		#define ADDR_ERAM_linux    0xB9000000
		#endif

	#ifdef LINUX_CROSS_GCC
		#define    __packed  //__attribute__ ((packed))
		#ifdef LINUX_DRIVE

		#else
			#include </mnt/hgfs/eclipse-698/include/drive/nuc970.h>
			#include </mnt/hgfs/eclipse-698/include/drive/sys.h>
		#endif

		#include </mnt/hgfs/eclipse-698/include/public/type.h>
	#else
		#include "type.h"
		#include "../../NUC97x/Driver/Include/nuc970.h"
		#include "../../NUC97x/Driver/Include/sys.h"
	#endif

 #endif
#endif


#if ((Project/100)==0)//���ܱ�
#include "Project698_0xx.h"
#include "Pin_Project698_0xx.h"
#endif

#if ((Project/100)==1)//�����
	#if (USER/100)==0//ä�����Ժ�ͼ��
		#include "Project698_1xx_USER0xx.h"
	#endif
	#if (USER/100)==1//��׼ϵ
		#include "Project698_1xx_USER0xx.h"
	#endif
#include "Pin_Project698_1xx.h"
#endif

//---������---
#if ((Project/100)==2)//������
	#if (USER/100)==0//ä�����Ժ�ͼ��
		#include "Project698_2xx_USER0xx.h"
	#endif
	#if (USER/100)==1//��׼ϵ
		#include "Project698_2xx_USER0xx.h"
	#endif
	#if (USER/100)==2//ɽ��ϵ
		#include "Project698_2xx_USER2xx.h"
	#endif
	#if (USER/100)==3//����ϵ
		#include "Project698_2xx_USER3xx.h"
	#endif
	#if (USER/100)==4//����ϵ
		#include "Project698_2xx_USER4xx.h"
	#endif
	#if (USER/100)==5//�Ϻ�ϵ
		#include "Project698_2xx_USER5xx.h"
	#endif
	#if (USER/100)==6//����ϵ
		#include "Project698_2xx_USER6xx.h"
	#endif
	#if (USER/100)==7//�㽭ϵ
		#include "Project698_2xx_USER7xx.h"
	#endif
	#if (USER/100)==8//ͨ���г�ϵ
		#include "Project698_2xx_USER8xx.h"
	#endif
	#if (USER/100)==9//����ϵ
		#include "Project698_2xx_USER9xx.h"
	#endif
	
	#if (USER/100)==11//����ϵ
		#include "Project698_2xx_USER11xx.h"
	#endif
	
	#if (USER/100)==14//����ϵ
		#include "Project698_2xx_USER14xx.h"
	#endif
	
	#if (USER/100)==17//����ϵ
		#include "Project698_2xx_USER17xx.h"
	#endif
	
	
	#ifdef IC_STM32F4xx
		#include "Pin_Project698_2xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "PinNUC97x_Project698_2xx.h"
	#endif

#endif
//---������ END---

//---ר���ն�---
#if ((Project/100)==3)//ר���ն�
	#if (USER/100)==0//ä�����Ժ�ͼ��
		#include "Project698_3xx_USER0xx.h"
	#endif
	#if (USER/100)==1//��׼ϵ
		#include "Project698_3xx_USER0xx.h"
	#endif
	#if (USER/100)==2//ɽ��ϵ
		#include "Project698_3xx_USER2xx.h"
	#endif
	#if (USER/100)==3//����ϵ
		#include "Project698_3xx_USER3xx.h"
	#endif
	#if (USER/100)==4//����ϵ
		#include "Project698_3xx_USER4xx.h"
	#endif
	#if (USER/100)==5//�Ϻ�ϵ
		#include "Project698_3xx_USER5xx.h"
	#endif
	#if (USER/100)==6//����ϵ
		#include "Project698_3xx_USER6xx.h"
	#endif
	
	#if (USER/100)==8//ͨ���г�ϵ
		#include "Project698_3xx_USER8xx.h"
	#endif
	#if (USER/100)==10//�ӱ�ϵ
		#include "Project698_3xx_USER10xx.h"
	#endif
	
	#if (USER/100)==12//�Ĵ�ϵ
		#include "Project698_3xx_USER12xx.h"
	#endif
	#if (USER/100)==15//����ϵ
		#include "Project698_3xx_USER15xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "Pin_Project698_3xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "PinNUC97x_Project698_3xx.h"
	#endif
#endif
//---ר���ն� END---

//---����һ�ն�---
#if ((Project/100)==4)//����һ�ն�

	#if (USER/100)==16//����ϵ
		#include "Project698_4xx_USER16xx.h"
	#endif

	#ifdef IC_STM32F4xx
		#include "Pin_Project698_4xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "PinNUC97x_Project698_4xx.h"
	#endif
#endif
//---����һ�ն� END---

//---����������---
#if ((Project/100)==5)//����������
	#if (USER/100)==0//ä�����Ժ�ͼ��
		#include "Project698_5xx_USER0xx.h"
	#endif

	#ifdef IC_STM32F4xx
		#include "Pin_Project698_5xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "PinNUC97x_Project698_5xx.h"
	#endif

#endif
//---���������� END---

#if ((Project/100)==6)//ͨ��ģ��
#include "Project6xx.h"
#include "Pin_Project6xx.h"
#endif



#include "../H698/IRAM698.h"
#include "../H698/IEEPROM698.h"
#include "../H698/EFLASH698.h"
#include "../H698/ERAM698.h"









//Pn����
#define p0Type     0//�ն���Ϣ��
#define pnType     1//�������
#define GnType     2//�ܼ����
#define CnType     3//�������������
#define DnType     4//ֱ��ģ������
#define RnType     5//�����ִκ�
#define TnType     6//�����
#define EnType     7//�¼�ERC��
#define MDCnType   8//���ܱ����ݷּ����
#define MDEnType   9//���ܱ����ݵȼ���



//���Ź���ʱֵ
#define WDTTimerOutVal     20000//���Ź���ʱֵ�����TimerOutMS=(0xfff*256)/32=32768ms
//�ɳ���
#define RechargeableBatteryGrid3     0x520//�ɳ���3��ʱ��ѹ
#define RechargeableBatteryGrid2     0x500//�ɳ���2��ʱ��ѹ
#define RechargeableBatteryGrid1     0x480//�ɳ���1��ʱ��ѹ
#define RechargeableBatteryStartMin  0x480//�ɳ�������������С��ѹ4.80V
#define RechargeableBatteryStopMin   0x440//�ɳ���ֹͣ������С��ѹ4.40V
	#ifdef IC_STM32F4xx
#define SuperCapBatteryStopMin       0x400//�������ݵ��ֹͣ������С��ѹ4.00V(ʵ��Լ3.50Vʱ����Ӳ����λ)
	#endif
	#ifdef IC_NUC9xx
#define SuperCapBatteryStopMin       0x300//�������ݵ��ֹͣ������С��ѹ3.00V(ʵ��2.70Vʱ��ѹ��·�������4.8V��2.2VʱCPU���ܹ���)
	#endif
	
//LOG
#define LOG_698          0//0=û,1=��
#define LOG_1376_1       0//0=û,1=��
#define LOG_1376_2       0//0=û,1=��
#define LOG_1376_3       0//1//0=û,1=��
#define LOG_645_1997     0//0=û,1=��
#define LOG_645_2007     0//0=û,1=��
//���Դ�ӡ��RS232
#define DEBUG_RS485_1toRS232  0//0=��,1=��;����:RS485_1���շ����ݴ�ӡ��RS232��
#define DEBUG_RS485_2toRS232  0//0=��,1=��;����:RS485_2���շ����ݴ�ӡ��RS232��
#define DEBUG_RS485_4toRS232  0//0=��,1=��;����:RS485_4(�ز�)��1376.2�շ����ݴ�ӡ��RS232��
#define DEBUG_GPRStoRS232     0//0=��,1=��;����:GPRS��1376.3�շ����ݴ�ӡ��RS232��
#define DEBUG_698toRS232      0//0=��,1=��;����:698.45�շ����ݴ�ӡ��RS232��
#define DEBUG_PORTtoRS232     (DEBUG_RS485_1toRS232|DEBUG_RS485_2toRS232|DEBUG_RS485_4toRS232|DEBUG_GPRStoRS232|DEBUG_698toRS232)
//��������
#define RUN_DL698TEST_1   	  0//0��Ч,1=����;ȫ��������Ϊ�ڲ����ɲ������ݴ洢������
#define RUN_DL698TEST_2   	  0//0��Ч,1=����;����601E��Լ��
#define RUN_DL698TEST_USER    0//0��Ч,1=����;�û������
#define TEST_DISPVDD5V0       0//0��Ч,1=����;��ʾVDD5V0��ѹ


#endif







