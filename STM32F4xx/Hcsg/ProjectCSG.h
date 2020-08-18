
#ifndef Project_H
#define Project_H


#define Project    300//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
#define IC_STM32F4xx
//#define IC_NUC9xx

//����
#define DEBUG     1// 0=����,1=����
#define Enable_sysprintf     0//NUC97x OS��UART0ϵͳ�������; 0=��ֹ,1=����
#define USER     	3400//3000//�û���ʶ
// 0xx=ä�����Ժ�ͼ��
	//0=ä�����Ժ�ͼ��
// 1xx=��׼ϵ
	//100=��׼��
// 2xx=ɽ��ϵ
	//200=ɽ����
// 3xx=����ϵ
	//300=������
// 4xx=����ϵ
	//400=������
// 5xx=�Ϻ�ϵ
	//500=�Ϻ���
// 6xx=����ϵ
	//600=���ϰ�
	//601=������̶��
// 7xx=�㽭ϵ
	//700=�㽭��
// 8xx=ͨ���г�ϵ
	//800=ͨ���г���
// 9xx=����ϵ
	//900=���ϰ�
// 10xx=�ӱ�ϵ
	//1000=�ӱ���
// 11xx=����ϵ
	//1100=���ְ�
// 12xx=�Ĵ�ϵ
	//1200=�Ĵ���
// 13xx=

// 14xx=�¸���ϵ
	//1400=������

//30xx=������׼��
//31xx = �㶫ϵ
	//3100 = �㶫��
//32xx = ����ϵ
	//3200 = ������
//33xx = ����ϵ
	//3300 = ���ݰ�
//34xx = ����ϵ
	//3400 = ���ϰ�
//35xx = ����ϵ
	//3500 = ���ϰ�

	


#define FactoryDI     0		//�������̱�ʶ  0 = ��̩ 1 = ?

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
	#define __packed   
	#include "type.h"
	extern u32 ADDR_ERAM_LINUX;
//	#include "../../NUC97x/Driver/Include/nuc970.h"
//	#include "../../NUC97x/Driver/Include/sys.h"
	#endif
#endif
#ifdef IC_IMX6xx
	#define __packed   
	#include "type.h"
	extern u32 ADDR_ERAM_LINUX;
#endif


#if ((Project/100)==2)	//������

	#if (USER/100)==0	//ä�����Ժ�ͼ��
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==1	//��׼ϵ
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#if (USER/100)==30	//������׼
		#include "../Hcsg/ProjectCSG_2xx_USER30xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "../Hcsg/Pin_ProjectCSG_2xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_2xx.h"
	#endif
	
#endif

#if ((Project/100)==3)	//ר���ն�

	#if (USER/100)==0	//ä�����Ժ�ͼ��
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	#if (USER/100)==1	//��׼ϵ
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	#if (USER/100)==30	//������׼
		#include "../Hcsg/ProjectCSG_3xx_USER30xx.h"
	#endif
	
	#if (USER/100)==34	//����ϵ
		#include "../Hcsg/ProjectCSG_3xx_USER34xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "../Hcsg/Pin_ProjectCSG_3xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_3xx.h"
	#endif
	
#endif

#if ((Project/100)==5)	//II�ͼ�����

	#if (USER/100)==0	//ä�����Ժ�ͼ��
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#if (USER/100)==1	//��׼ϵ
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#if (USER/100)==30	//������׼
		#include "../Hcsg/ProjectCSG_5xx_USER30xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "../Hcsg/Pin_ProjectCSG_5xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_5xx.h"
	#endif
	
#endif


#include "../Hcsg/IRAMCSG.h"
#include "../Hcsg/IEEPROMCSG.h"
#include "../Hcsg/EFLASHCSG.h"
#include "../Hcsg/ERAMCSG.h"



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
#define WDTTimerOutVal     				20000		//���Ź���ʱֵ�����TimerOutMS=(0xfff*256)/32=32768ms
//�ɳ���
#define RechargeableBatteryGrid3    	0x520		//�ɳ���3��ʱ��ѹ
#define RechargeableBatteryGrid2     	0x500		//�ɳ���2��ʱ��ѹ
#define RechargeableBatteryGrid1     	0x480		//�ɳ���1��ʱ��ѹ
#define RechargeableBatteryStartMin     0x480		//�ɳ�������������С��ѹ4.80V
#define RechargeableBatteryStopMin     	0x440		//�ɳ���ֹͣ������С��ѹ4.40V
	#ifdef IC_STM32F4xx
#define SuperCapBatteryStopMin       0x400//�������ݵ��ֹͣ������С��ѹ4.00V(ʵ��Լ3.50Vʱ����Ӳ����λ)
	#endif
	#ifdef IC_NUC9xx
#define SuperCapBatteryStopMin       0x300//�������ݵ��ֹͣ������С��ѹ4.00V(ʵ��Լ3.50Vʱ����Ӳ����λ)
	#endif

//LOG
#define LOG_698          0//1//0=û,1=��
#define LOG_1376_1       0//0=û,1=��
#define LOG_1376_2       0//1//0=û,1=��
#define LOG_1376_3       0//0=û,1=��
#define LOG_645_1997     0//0=û,1=��
#define LOG_645_2007     0//0=û,1=��
//���Դ�ӡ��RS232
#define DEBUG_RS485_1toRS232  0//0=��,1=��;����:RS485_1���շ����ݴ�ӡ��RS232��
#define DEBUG_RS485_2toRS232  0//0=��,1=��;����:RS485_2���շ����ݴ�ӡ��RS232��
#define DEBUG_RS485_4toRS232  0//0=��,1=��;����:RS485_4(�ز�)��1376.2�շ����ݴ�ӡ��RS232��
#define DEBUG_GPRStoRS232     0//0=��,1=��;����:GPRS��1376.3�շ����ݴ�ӡ��RS232��
#define DEBUG_698toRS232      0//0=��,1=��;����:698.45�շ����ݴ�ӡ��RS232��
#define DEBUG_PORTtoRS232     (DEBUG_RS485_1toRS232|DEBUG_RS485_2toRS232|DEBUG_RS485_4toRS232|DEBUG_GPRStoRS232|DEBUG_698toRS232)


#endif







