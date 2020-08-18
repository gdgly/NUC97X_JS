


#ifndef Terminal_Event_H
#define Terminal_Event_H



#if ((Project/100)==1)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
#include "../Head/ERC_Project1_USER0xx.h"
#endif

#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#if(USER/100)==4//����
#include "../Head/ERC_Project2_USER4xx.h"
	#else
		#if(USER/100)==5//�Ϻ�
#include "../Head/ERC_Project2_USER5xx.h"
		#else
			#if(USER/100)==6//����
#include "../Head/ERC_Project2_USER6xx.h"
			#else
				#if(USER/100)==7//�㽭
#include "../Head/ERC_Project2_USER7xx.h"
				#else
					#if(USER/100)==8//ͨ���г�
#include "../Head/ERC_Project2_USER8xx.h"
					#else
						#if(USER/100)==9//����
#include "../Head/ERC_Project2_USER9xx.h"
						#else
							#if(USER/100)==11//����
#include "../Head/ERC_Project2_USER11xx.h"
							#else
								#if(USER/100)==12//�Ĵ�
#include "../Head/ERC_Project2_USER12xx.h"
								#else
									#if(USER/100)==3//����
#include "../Head/ERC_Project2_USER3xx.h"
									#else
#include "../Head/ERC_Project2_USER0xx.h"
									#endif
								#endif
							#endif
						#endif
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	#if(USER/100)==5//�Ϻ�
#include "../Head/ERC_Project3_USER5xx.h"
	#else
		#if(USER/100)==10//�ӱ�
#include "../Head/ERC_Project3_USER10xx.h"
		#else
			#if(USER/100)==12//�Ĵ�
#include "../Head/ERC_Project3_USER12xx.h"
			#else
				#if(USER/100)==15//����
#include "../Head/ERC_Project3_USER15xx.h"
				#else
					#if(USER/100)==8//ͨ���г�
#include "../Head/ERC_Project3_USER8xx.h"
					#else
#include "../Head/ERC_Project3_USER0xx.h"
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#if ((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
#include "../Head/ERC_Project4_USER0xx.h"
#endif

#if ((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

#endif

#if ((Project/100)==6)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

#endif










void Init_Class3Data_UpPower(void);//��3����(�¼����㣩��ʼ��(�ϵ�)
void Init_Class3Data_Command(void);//��3����(�¼�)��ʼ��(����,ȫ�����)
void Init_Class3Data(u32 Pn);//��3����(�¼�)��ʼ��(ָ���Ĳ���������,Pn=1~n)

u32 ERC_Event_Set(u32 EventNo);//����¼���¼����,����0=����¼,1=��¼
u32 ERC_Event_Save(u32 BuffAddr);//��3����(�¼�)����,�Զ�ʶ����Ҫ��һ��;����0û��¼,1��¼
u32 Get_ADDR_UARTnRMEvent(u32 PORTn);//�õ�UART�������ܱ��¼�ջ�ṹ��ַ
u32 Check_Meter_Event_Save(void);//�����ܱ��¼������ռ�;����0=�洢�M(û�ϱ�),1-255�����ɴ洢������
void Meter_Event_WriteBuff(u32 PORTn,u32 Pn,u32 Addr_FnDataBuff,u8 *p8RMLib);//��ȡ�ĵ��ܱ��¼�д�뻺��ADDR_DATABUFF
void Meter_Event_Save(u32 Addr_Buff);//ֱ�Ӵ�����ȡ�ĵ��ܱ��¼�
void WriteAutoReportWord(u32 PORTn,u32 rxbuff);//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
u32 Check_AutoReportWord(u32 PORTn);//�����ܱ������ϱ�״̬��;����:0=��,!=0 B31-B16Ϊ�������(1��ʼ),B15-B0ΪҪ��ȡ�������ϱ�״̬��λ��(0-95)
u32 RMEvene_YesNo(u32 PORTn,u32 Pn,u8 * p8RMLib,u32 RMCount,u32 Protocol);//������ּ��¼�������Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)
void Search_AFN04F106_Evene_DI(u32 PORTn,u32 Pn,u32 RMCount);//������ּ��¼��ĵ�ǰ�¼�����DI����AFN04F106�����зǷ����������¼��೭��DI,����:������UARTCtrl->RMDI=DI,û����UARTCtrl->RMDI=0xffffffff
u32 Search_AFN04F106_NoEvene_DI(u32 PORTn,u32 Pn);//������ּ��¼�����AFN04F106�����з��¼��೭��;����0=û�ҵ�,1=�ҵ�


void ERC1_Event(u32 f);//���ݳ�ʼ���Ͱ汾�����¼;f=1���ݳ�ʼ��,f=2�汾���,f=3���ݳ�ʼ��ͬʱ�汾���
void ERC2_Event(u32 f);//������ʧ��¼;f=1�ն˲�����ʧ,f=2�����������ʧ,f=3�ն˲�����ʧͬʱ�����������ʧ
void ERC3_Event(u32 rxbuff);//���������¼,��ڽ��ջ�����ַ
void ERC4_Event(u32 sc,u32 ss);//״̬����λ��¼;���sc=״̬��λ,ss=��λ��״̬
void ERC5_Event(u32 n,u32 kw1,u32 kw2);//ң����բ��¼;���n=��բ�ִ�(0-7),kw1=��բʱ����(�ܼӹ���),kw2=��բ��2���ӵĹ���(�ܼӹ���)
void ERC6_Event(u32 Pn,u32 Bin,u32 Type,u32 kW1,u32 kW2,u32 kW3);//������բ��¼;���Pn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type�������,kw1��բǰ����,kw2��բ��2���ӹ���,kw3��բʱ���ʶ�ֵ
void ERC7_Event(u32 Pn,u32 Bin,u32 Type,u32 kWh1,u32 kWh2);//�����բ��¼;���Pn�ܼ����(��1��ʼ),Bin��բ�ִ�,Type������,kwh1��բʱ������,kwh2��բʱ��������ֵ
void ERC8_Event(u32 Pn,u32 FnDataBuff);//���ܱ�������,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
void ERC9_3_Event(u32 Pn);//������·�쳣.����,���:Pn�������(��1��ʼ)
//void ERC10_Event(u32 Pn);//��ѹ��·�쳣(���ࡢʧѹ),���:Pn�������(��1��ʼ)
void ERC10_Event(void);//��ѹ��·�쳣(���ࡢʧѹ)
void ERC11_Event(u32 Pn,u32 f);//�����쳣,���:Pn�������(��1��ʼ),f=0�ָ�=1����
void ERC12_Event(u32 Pn);//���ܱ�ʱ�䳬��,���:Pn�������(��1��ʼ)
void ERC13_Event(u32 Pn,u32 FnDataBuff);//��������Ϣ,���Pn=�������(0��ʼ),Fn�������ݻ����ַ
	#if NewERC14==0//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
void ERC14_Event_Start(void);//�ն�ͣ/�ϵ��¼�(ͣ��)
void ERC14_Event_End(void);//�ն�ͣ/�ϵ��¼�(�ϵ�),��ר���¼����ݻ���
	#else
void ERC14_Event_Start(u32 f);//�ն�ͣ/�ϵ��¼�(ͣ��);���:�ϵ��¼ʱ�¼�����ָ�����1��־,0=����1,0xee=���1
void ERC14_Event_End(u32 f);//�ն�ͣ/�ϵ��¼�(�ϵ�),��ר���¼����ݻ���
	#endif
//ERC15г��Խ�޸澯(�Ѱ�����ͳ����)
void ERC16_Event(u32 Pn,u32 ss,u32 f,u32 dc);//ֱ��ģ����Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=Խ�ޱ�־,dc=��ǰֱ��ģ����
void ERC17_Event(u32 Pn,u32 ss,u32 f,u32 unu,u32 uni);//��ѹ/������ƽ���Խ��,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,unu=ѹ��ƽ���,uni=������ƽ���
//ERC18������Ͷ��������¼
void ERC19_Event(u32 Pn,u32 RxAddr,u32 kWh1,u32 kWh2);//����������ü�¼
void ERC20_Event(u64 PWL,u64 PWH,u32 MSA);//��Ϣ��֤�����¼,���:PWH_PWL��Ϣ��֤��,MSA=����վ��ַ
void ERC21_Event(void);//ERC21�ն˹��ϼ�¼
void ERC22_Event(void);//�й��ܵ������Խ�޼�����¼���¼
void ERC23_Event(u32 Pn,u32 Bin,u32 Type,u32 kWh1,u32 kWh2);//��ظ澯��¼;���Pn�ܼ����,Bin��բ�ִ�,Type������,kwh1�澯ʱ������,kwh2�澯ʱ��������ֵ
void ERC24_Event(u32 Pn,u32 ss,u32 f);//��ѹԽ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
void ERC25_Event(u32 Pn,u32 ss,u32 f);//����Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־
void ERC26_Event(u32 Pn,u32 ss,u32 f,u32 kva1,u32 kva2);//���ڹ���Խ�޼�¼,���:Pn�������(��1��ʼ),ss=0�ָ�=1����,f=�쳣��־,kva1=����ʱ�����ڹ���,kva2=����ʱ�����ڹ�����ֵ
void ERC27_Event(u32 Pn,u32 FnDataBuff);//���ܱ�ʾ���½���¼(����1���ݴ���ǰ����¼�),���Pn=�������(1��ʼ),Fn�������ݻ����ַ
//ERC28�����������¼
void ClrERC29SOUR(u32 Pn);//���г���ʧ����ERC29���ܱ����,�����¼�ԭʼ����
void ERC29_Event(u32 Pn,u32 FnDataBuff);//���ܱ����,����(����1���ݴ���ǰ����¼�),���Pn=�������(1��ʼ),Fn�������ݻ����ַ
void ERC30_Event(u32 Pn);//���ܱ�ͣ���¼�,���:Pn=�������(1��ʼ)
void ERC31_Event(u32 MeterNo,u32 Pn,u32 f);//�ն�485����ʧ���¼�,��ڲ������,f=1����=0�ָ�
void ERC32_Event(u32 n1,u32 n2);//ͨ�������������¼�,n1=�����ѷ�����ͨ������,n2=��ͨ����������
void ERC33_Event(u32 Pn,u32 FnDataBuff);//���ܱ�����״̬�ֱ�λ�¼�(����1���ݴ���ǰд�¼�),���Pn=�������(1��ʼ),Fn�������ݻ����ַ
//ERC34 CT�쳣�¼�
void ERC35_Event(u32 Port,u32 n,u32 DataAddr);//����δ֪����¼�;���Port=�˿ں�,n=δ֪�����,DataAddr=δ֪���ͨ�ŵ�ַ��ÿ��8�ֽ����ݴ�ŵĵ�ַ
void ERC36_Event(u32 CD,u32 ST);//���������·���ؽ���״̬����λ��¼;��ڣ�CD=��λ,ST=״̬
void ERC37_Event(u32 Pn,u32 ADDR_DATA);//���ܱ�����¼���¼;���:Pn�������(��1��ʼ)
void ERC38_Event(u32 Pn,u32 ADDR_DATA);//���ܱ���ť���¼���¼;���:Pn�������(��1��ʼ)
//void ERC39_Event(u32 Fn,u32 Pn,u32 f);//����ʧ���¼���¼;���:��ǰ������FnPn,f=0����ɹ�,f=1����ʧ��
void ERC39_Event(u32 Pn);//����ʧ���¼���¼
void ERC39_Event_Cancel(u32 Pn);//����ʧ���¼��ָ�
void ERC40_Meter_Event(u32 FnDataAddr,u32 Pn);//ERC40ֱ�Ӷ����ܱ�ų��쳣�¼���¼;���:Fn�����׵�ַ,�������(1��ʼ)
void ERC41_Event(u32 ADDR_BUFF);//��ʱ�¼���¼;���:ADDR_BUFF�ĵ�2�ֽڿ�ʼ�ֱ�Ϊ2BYTE�������,6BYTE��ʱǰʱ��,6BYTE��ʱ��ʱ��
void ERC41_Meter_Event(u32 FnDataAddr,u32 Pn);//ERC41ֱ�Ӷ����ܱ��ʱ�¼���¼;���:Fn�����׵�ַ,�������(1��ʼ)


void ERC45_Event(void);//�ն˵��ʧЧ�¼�
void PerERC_Event_Save(u32 BuffAddr);//����ERC�¼���¼
#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
void ERC51_Event(u32 Pn,u32 ADDR_BUFF);//Уʱ��¼�¼�;���:Pn�������(��0��ʼ)
#endif

#endif








