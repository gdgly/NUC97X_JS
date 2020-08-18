
#ifndef Project_H
#define Project_H


#define Project     300//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//#define IC_STM32F4xx
#define IC_NUC9xx

#define DEBUG     0//0=����,1=����
#define Enable_sysprintf     0//NUC97x OS��UART0ϵͳ�������; 0=��ֹ,1=����
#define USER     1200//500//1400//1200//�û���ʶ
//0xx=ä�����Ժ�ͼ��
	//0=ä�����Ժ�ͼ��
//1xx=��׼ϵ
	//100=��׼��
//2xx=ɽ��ϵ
	//200=ɽ����
//3xx=����ϵ(����������)
	//300=������
//4xx=����ϵ
	//400=������
//5xx=�Ϻ�ϵ
	//500=�Ϻ���
//6xx=����ϵ
	//600=���ϰ�
	//601=������̶��
//7xx=�㽭ϵ
	//700=�㽭��
//8xx=ͨ���г�ϵ
	//800=ͨ���г���
//9xx=����ϵ
	//900=���ϰ�
//10xx=�ӱ�ϵ
	//1000=�ӱ���
//11xx=����ϵ
	//1100=���ְ�
//12xx=�Ĵ�ϵ
	//1200=�Ĵ���
//13xx=

//14xx=�¸���ϵ
	//1400=������


#define FactoryDI     0//�������̱�ʶ
//0=��̩
//1=

#include "type.h"
#ifdef IC_STM32F4xx
	#include "stm32f4xx.h"
#endif
#ifdef IC_NUC9xx
	#include "../../NUC97x/Driver/Include/nuc970.h"
	#include "../../NUC97x/Driver/Include/sys.h"
#endif






#if ((Project/100)==0)//���ܱ�
#include "Project0xx.h"
#include "Pin_Project0xx.h"
#endif

#if ((Project/100)==1)//�����
	#if (USER/100)==0//ä�����Ժ�ͼ��
		#include "Project1xx_USER0xx.h"
	#endif
	#if (USER/100)==1//��׼ϵ
		#include "Project1xx_USER0xx.h"
	#endif
#include "Pin_Project1xx.h"
#endif

#if ((Project/100)==2)//������
	#if (USER/100)==0//ä�����Ժ�ͼ��
		#include "Project2xx_USER0xx.h"
	#endif
	#if (USER/100)==1//��׼ϵ
		#include "Project2xx_USER0xx.h"
	#endif
	#if (USER/100)==2//ɽ��ϵ
		#include "Project2xx_USER2xx.h"
	#endif
	#if (USER/100)==3//����ϵ
		#include "Project2xx_USER3xx.h"
	#endif
	#if (USER/100)==4//����ϵ
		#include "Project2xx_USER4xx.h"
	#endif
	#if (USER/100)==5//�Ϻ�ϵ
		#include "Project2xx_USER5xx.h"
	#endif
	#if (USER/100)==6//����ϵ
		#include "Project2xx_USER6xx.h"
	#endif
	#if (USER/100)==7//�㽭ϵ
		#include "Project2xx_USER7xx.h"
	#endif
	#if (USER/100)==8//ͨ���г�ϵ
		#include "Project2xx_USER8xx.h"
	#endif
	#if (USER/100)==9//����ϵ
		#include "Project2xx_USER9xx.h"
	#endif
	
	#if (USER/100)==11//����ϵ
		#include "Project2xx_USER11xx.h"
	#endif
	#if (USER/100)==12//�Ĵ�ϵ
		#include "Project2xx_USER12xx.h"
	#endif
	
	#if (USER/100)==14//����ϵ
		#include "Project2xx_USER14xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "Pin_Project2xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_2xx.h"
	#endif
#endif

#if ((Project/100)==3)//ר���ն�
	#if (USER/100)==0//ä�����Ժ�ͼ��
		#include "Project3xx_USER0xx.h"
	#endif
	#if (USER/100)==1//��׼ϵ
		#include "Project3xx_USER0xx.h"
	#endif
	#if (USER/100)==2//ɽ��ϵ
		#include "Project3xx_USER2xx.h"
	#endif
	#if (USER/100)==3//����ϵ
		#include "Project3xx_USER3xx.h"
	#endif
	#if (USER/100)==4//����ϵ
		#include "Project3xx_USER4xx.h"
	#endif
	#if (USER/100)==5//�Ϻ�ϵ
		#include "Project3xx_USER5xx.h"
	#endif
	#if (USER/100)==6//����ϵ
		#include "Project3xx_USER6xx.h"
	#endif
	
	#if (USER/100)==8//ͨ���г�ϵ
		#include "Project3xx_USER8xx.h"
	#endif
	#if (USER/100)==10//�ӱ�ϵ
		#include "Project3xx_USER10xx.h"
	#endif
	
	#if (USER/100)==12//�Ĵ�ϵ
		#include "Project3xx_USER12xx.h"
	#endif
	
	#ifdef IC_STM32F4xx
		#include "Pin_Project3xx.h"
	#endif
	#ifdef IC_NUC9xx
		#include "../H698/PinNUC97x_Project698_3xx.h"
	#endif
#endif

#if ((Project/100)==4)//�����ն�
	#if (USER/100)==0//ä�����Ժ�ͼ��
		#include "Project4xx_USER0xx.h"
	#endif
	#if (USER/100)==1//��׼ϵ
		#include "Project4xx_USER0xx.h"
	#endif
#include "Pin_Project4xx.h"
#endif

#if ((Project/100)==5)//����������
#include "Project5xx.h"
#include "Pin_Project5xx.h"
#endif

#if ((Project/100)==6)//ͨ��ģ��
#include "Project6xx.h"
#include "Pin_Project6xx.h"
#endif

#include "AFN04MaxPn.h"
#include "AFN05MaxPn.h"
#include "AFN0CMaxPn.h"
#include "AFN0DMaxPn.h"
#include "AFN0EMaxPn.h"

#include "IRAM.h"
#include "IEEPROM.h"
#include "EFLASH.h"
#include "ERAM.h"


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
#define RechargeableBatteryStartMin     0x480//�ɳ�������������С��ѹ4.80V
#define RechargeableBatteryStopMin     0x460//�ɳ���ֹͣ������С��ѹ4.60V
	#ifdef IC_STM32F4xx
#define SuperCapBatteryStopMin       0x400//�������ݵ��ֹͣ������С��ѹ4.00V(ʵ��Լ3.50Vʱ����Ӳ����λ)
	#endif
	#ifdef IC_NUC9xx
#define SuperCapBatteryStopMin       0x300//�������ݵ��ֹͣ������С��ѹ4.00V(ʵ��Լ3.50Vʱ����Ӳ����λ)
	#endif
//���4.8V�ɳ���:
//����Լ5.5V-5.6V
//����1����������Լ20%
//5.0VʱԼ����20%����
//4.8Vʱ������5%����
//4.4V�������й������
//4.0V���¿ɻָ�Լ70%-80%����
//4.8V-5.0Vʱ���������С




#endif







