
//���ݸ�ʽת��
#include "Project.h"
#include "../Terminal/DataConversion_SH698.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../OS/OS.h"
#include <stdarg.h>
#include <string.h>


#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
__asm void ADDR_RM_30000700_1001FF01(void)//ʧѹ
{
	dcdu 0x30000700//��ʾ����֧�ֵ��¼���¼��OAD
	dcb 53//��Ҫ��ȡ��OAD����
	dcdu 0x201e0200//����ʱ��
	dcb 1//��4λ��ʾ���¼���¼��ĵڼ�������4λ��ʾ�������� PROTO_DT_ARRAY PROTO_DT_STRUCTURE�ĵڼ�����Ա
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0//��λֻ���� PROTO_DT_ARRAY PROTO_DT_STRUCTURE��������������ʱ��Ч����ʾ��Ա����������
	dcdu 0x00102201//����ʱ�̵�������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00202201//����ʱ�̵ķ�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00302201//����ʱ�̵�����޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00402201//����ʱ�̵�����޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x00112201//����ʱ�̵�A��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00212201//����ʱ�̵�A�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00312201//����ʱ�̵�A������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00412201//����ʱ�̵�A������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x20002200//����ʱ�̵�A���ѹ
	dcb 0x11	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_LONG_UN	//��Ա����������0x12 unsigned_long
	dcdu 0x20012200//����ʱ�̵�A�����
	dcb 0x11	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x05 double-long
	dcdu 0x20042200//����ʱ�̵�A���й�����
	dcb 0x11	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x05 double-long
	dcdu 0x20052200//����ʱ�̵�A���޹�����
	dcb 0x11	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x12
	dcdu 0x200a2200//����ʱ�̵�A�������
	dcb 0x11	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb 0x10	//��Ա����������0x0f long
	dcdu 0x00122201//����ʱ�̵�B��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00222201//����ʱ�̵�B�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00322201//����ʱ�̵�B������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00422201//����ʱ�̵�B������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x20002200//����ʱ�̵�A���ѹ
	dcb 0x21	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_LONG_UN	//��Ա����������0x12 unsigned_long
	dcdu 0x20012200//����ʱ�̵�A�����
	dcb 0x21	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x05 double-long
	dcdu 0x20042200//����ʱ�̵�A���й�����
	dcb 0x21	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x05 double-long
	dcdu 0x20052200//����ʱ�̵�A���޹�����
	dcb 0x21	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x12
	dcdu 0x200a2200//����ʱ�̵�A�������
	dcb 0x21	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb 0x10	//��Ա����������0x0f long
	dcdu 0x00132201//����ʱ�̵�C��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00232201//����ʱ�̵�C�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00332201//����ʱ�̵�C������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00432201//����ʱ�̵�C������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x20002200//����ʱ�̵�A���ѹ
	dcb 0x31	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_LONG_UN	//��Ա����������0x12 unsigned_long
	dcdu 0x20012200//����ʱ�̵�A�����
	dcb 0x31	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x05 double-long
	dcdu 0x20042200//����ʱ�̵�A���й�����
	dcb 0x31	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x05 double-long
	dcdu 0x20052200//����ʱ�̵�A���޹�����
	dcb 0x31	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG	//��Ա����������0x12

	dcdu 0x200a2200//����ʱ�̵�A�������
	dcb 0x31	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb 0x10	//��Ա����������0x0f long


	dcdu 0x20296200//����ʱ�̵�A�������
	dcb 0x11	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG_UN	//��Ա����������0x0f long

	dcdu 0x20296200//����ʱ�̵�A�������
	dcb 0x21	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG_UN	//��Ա����������0x0f long

	dcdu 0x20296200//����ʱ�̵�A�������
	dcb 0x31	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG_UN	//��Ա����������0x0f long

	dcdu 0x20296200//����ʱ�̵�A�������
	dcb 0x41	//��¼���һ����ARRAY�ĵ�1����Ա
	dcb PROTO_DT_ARRAY	//����������ARRAY=1
	dcb PROTO_DT_DOUBLE_LONG_UN	//��Ա����������0x0f long

	
	dcdu 0x20200200//����ʱ��
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0

	dcdu 0x00108201//����ʱ�̵�������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00208201//����ʱ�̵ķ�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00308201//����ʱ�̵�����޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00408201//����ʱ�̵�����޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00118201//����ʱ�̵�A��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00218201//����ʱ�̵�A�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00318201//����ʱ�̵�A������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00418201//����ʱ�̵�A������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00128201//����ʱ�̵�B��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00228201//����ʱ�̵�B�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00328201//����ʱ�̵�B������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00428201//����ʱ�̵�B������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00138201//����ʱ�̵�C��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00238201//����ʱ�̵�C�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00338201//����ʱ�̵�C������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00438201//����ʱ�̵�C������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0;

	ALIGN
}



__asm void ADDR_RM_300F0200_1400FF01(void)//��ѹ������
{
	dcdu 0x300F0200//��ʾ����֧�ֵ��¼���¼��OAD
	dcb 34//��Ҫ��ȡ��OAD����
	dcdu 0x201e0200//����ʱ��
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0
	dcdu 0x00102201//����ʱ�̵�������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00202201//����ʱ�̵ķ�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00302201//����ʱ�̵�����޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00402201//����ʱ�̵�����޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x00112201//����ʱ�̵�A��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00212201//����ʱ�̵�A�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00312201//����ʱ�̵�A������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00412201//����ʱ�̵�A������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x00122201//����ʱ�̵�B��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00222201//����ʱ�̵�B�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00322201//����ʱ�̵�B������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00422201//����ʱ�̵�B������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x00132201//����ʱ�̵�C��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00232201//����ʱ�̵�C�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00332201//����ʱ�̵�C������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00432201//����ʱ�̵�C������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x20200200//����ʱ��
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0

	dcdu 0x00108201//����ʱ�̵�������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00208201//����ʱ�̵ķ�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00308201//����ʱ�̵�����޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00408201//����ʱ�̵�����޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00118201//����ʱ�̵�A��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00218201//����ʱ�̵�A�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00318201//����ʱ�̵�A������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00418201//����ʱ�̵�A������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00128201//����ʱ�̵�B��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00228201//����ʱ�̵�B�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00328201//����ʱ�̵�B������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00428201//����ʱ�̵�B������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00138201//����ʱ�̵�C��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00238201//����ʱ�̵�C�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00338201//����ʱ�̵�C������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00438201//����ʱ�̵�C������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	
	ALIGN
}


__asm void ADDR_RM_30110200_03110001(void)//�����¼
{
	dcdu 0x30110200//��ʾ����֧�ֵ��¼���¼��OAD
	dcb 2//��Ҫ��ȡ��OAD����
	dcdu 0x201e0200//����ʱ��
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0
	dcdu 0x20200200//����ʱ��
	dcb 1
	dcb PROTO_DT_DATE_TIME_S
	dcb 0
	
	ALIGN
}



__asm void ADDR_RM_30160200_03300401(void)//Уʱ��¼
{
	dcdu 0x30160200//��ʾ����֧�ֵ��¼���¼��OAD
	dcb 3//��Ҫ��ȡ��OAD����
	dcdu 0x20220200//�����ߴ���---û����ӦOAD�����¼���¼��Ŵ��棬����4�ֽ�
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DOUBLE_LONG_UN//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0
	dcdu 0x40002200//Уʱǰʱ��
	dcb 1
	dcb PROTO_DT_DATE_TIME_S
	dcb 0
	dcdu 0x40008200//Уʱ��ʱ��
	dcb 1
	dcb PROTO_DT_DATE_TIME_S
	dcb 0
	
	ALIGN
}

__asm void ADDR_RM_301B0200_03300D01(void)//����Ǽ�¼
{
	dcdu 0x301b0200//��ʾ����֧�ֵ��¼���¼��OAD
	dcb 14//��Ҫ��ȡ��OAD����
	dcdu 0x201e0200//����ʱ��
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0
	dcdu 0x20200200//����ʱ��
	dcb 1
	dcb PROTO_DT_DATE_TIME_S
	dcb 0
	dcdu 0x00102201//����ǰ��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00202201//����ǰ�ķ�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00502201//����ǰ�ĵ�һ�����޹���
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00602201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00702201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00802201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00108201
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00208201
	dcb 2	
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00508201
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00608201
	dcb 2	
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00708201
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00808201
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	ALIGN
}
__asm void ADDR_RM_301C0200_03300E01(void)//����ť�Ǽ�¼
{
	dcdu 0x301c0200//��ʾ����֧�ֵ��¼���¼��OAD
	dcb 10//��Ҫ��ȡ��OAD����
	dcdu 0x201e0200//����ʱ��
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcdu 0x20200200//����ʱ��
	dcb 1
	dcb PROTO_DT_DATE_TIME_S
	dcb 0
	dcdu 0x00102201//����ǰ��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00202201//����ǰ�ķ�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00502201//����ǰ�ĵ�һ�����޹���
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00602201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00702201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00802201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00108201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00208201
	dcb 1	
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00508201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00608201
	dcb 1	
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00708201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00808201
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	ALIGN
}
__asm void ADDR_RM_301D0200_1600FF01(void)//��ѹ��ƽ��
{
	dcdu 0x301D0200//��ʾ����֧�ֵ��¼���¼��OAD
	dcb 35//��Ҫ��ȡ��OAD����
	dcdu 0x201e0200//����ʱ��
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0
	
	dcdu 0x00102201//����ʱ�̵�������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00202201//����ʱ�̵ķ�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00302201//����ʱ�̵�����޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00402201//����ʱ�̵�����޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x00112201//����ʱ�̵�A��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00212201//����ʱ�̵�A�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00312201//����ʱ�̵�A������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00412201//����ʱ�̵�A������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x00122201//����ʱ�̵�B��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00222201//����ʱ�̵�B�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00322201//����ʱ�̵�B������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00422201//����ʱ�̵�B������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x00132201//����ʱ�̵�C��������
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00232201//����ʱ�̵�C�����
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00332201//����ʱ�̵�C������޹�1
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00432201//����ʱ�̵�C������޹�2
	dcb 1
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	dcdu 0x20286200//���ƽ����
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_LONG_UN//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0
	
	dcdu 0x20200200//����ʱ��
	dcb 1//���¼���¼��ĵڼ�����
	dcb PROTO_DT_DATE_TIME_S//�����ݵ����ͣ����������ж��Ƿ���ȷ��
	dcb 0

	dcdu 0x00108201//����ʱ�̵�������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00208201//����ʱ�̵ķ�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00308201//����ʱ�̵�����޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00408201//����ʱ�̵�����޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00118201//����ʱ�̵�A��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00218201//����ʱ�̵�A�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00318201//����ʱ�̵�A������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00418201//����ʱ�̵�A������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00128201//����ʱ�̵�B��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00228201//����ʱ�̵�B�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00328201//����ʱ�̵�B������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00428201//����ʱ�̵�B������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	
	dcdu 0x00138201//����ʱ�̵�C��������
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00238201//����ʱ�̵�C�����
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00338201//����ʱ�̵�C������޹�1
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0
	dcdu 0x00438201//����ʱ�̵�C������޹�2
	dcb 2
	dcb PROTO_DT_DOUBLE_LONG_UN
	dcb 0

	
	ALIGN
}

const u32 DL698_to_DL376_Lib[]=
{
	(u32)ADDR_RM_30000700_1001FF01,//ʧѹ
	(u32)ADDR_RM_300F0200_1400FF01,//��ѹ������
	(u32)ADDR_RM_30110200_03110001,//�����¼
	(u32)ADDR_RM_30160200_03300401,//Уʱ��¼
	(u32)ADDR_RM_301B0200_03300D01,//����Ǽ�¼
	(u32)ADDR_RM_301C0200_03300E01,//����ť�Ǽ�¼
	(u32)ADDR_RM_301D0200_1600FF01,//��ѹ��ƽ��
};

const u16 fcstab[256] = {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


//����FCSУ���
//p8rxbuff = ��Ҫ�����ԭʼ����ָ��
//Byte = ��Ҫ��������ݳ���
u32 Get_16bitfcs_code(const u8* p8rxbuff,u32 Byte)
{
	u32 fcs = 0xffff;
	u32 i = 0;
	for(i=0;i<Byte;i++)
	{
		fcs = ((fcs)>>8)^fcstab[((fcs)^(p8rxbuff[i]))&0xff];
	}
	return fcs;
}



//s_data ԭʼ����ָ�룬������0x1c��ͷ��
//����ת��������ݸ�ʽ::
//s_data��ʽHEX:0x1c YY YY MM DD hh mm ss
//d_data��ʽBCD:ss mm hh 00 DD MM YY
void  DateTimeStoDCB(u8* s_data,u8* d_data)//��698��date_time_s��ʽ��ʱ��ת��376.1�ĸ�ʽ
{
	if(s_data[0]!= 0x1c)
		return ;
	d_data[0]=hex_bcd(s_data[7])&0xff;
	d_data[1]=hex_bcd(s_data[6])&0xff;
	d_data[2]=hex_bcd(s_data[5])&0xff;
	d_data[3]=0;
	d_data[4]=hex_bcd(s_data[4])&0xff;
	d_data[5]=hex_bcd(s_data[3])&0xff;
	d_data[6]=hex_bcd(s_data[2]+(s_data[1]<<8)-2000)&0xff;
}
u32 Get_LastTimeDi_From_OAD(u32 OAD,u32 type)//ͨ��OAD��ö�Ӧ���ۼ�ʱ������ݱ�ʶ��type=1��ʶA�� =2��ʶB��  =3��ʾC��
{
	switch(OAD)
		{
		case 0x30000a00:
			return 0x10010002+0x10000*(type-1);//ʧѹ
		case 0x30010a00:
			return 0x11010002+0x10000*(type-1);//Ƿѹ
		case 0x30020a00:
			return 0x12010002+0x10000*(type-1);//��ѹ
		case 0x30040a00:
			return 0x18010002+0x10000*(type-1);//ʧ��
		case 0x30050a00:
			return 0x19010002+0x10000*(type-1);//����
		case 0x30030a00:
			return 0x13010002+0x10000*(type-1);//����
		default:
			return 0;
		}
}
u32 Get_EventNumDi_From_OAD(u32 OAD)//ͨ��OAD��ö�Ӧ���¼����������ݱ�ʶ��type=1��ʶA�� =2��ʶB��  =3��ʾC��
{
	switch(OAD)
		{
		case 0x30000700:
			return 0x10010001;//ʧѹ
		case 0x30000800:
			return 0x10020001;//ʧѹ
		case 0x30000900:
			return 0x10030001;//ʧѹ
			
		case 0x30010700:
			return 0x11010001;//Ƿѹ
		case 0x30010800:
			return 0x11020001;//Ƿѹ
		case 0x30010900:
			return 0x11030001;//Ƿѹ
			
		case 0x30020700:
			return 0x12010001;//��ѹ
		case 0x30020800:
			return 0x12020001;//��ѹ
		case 0x30020900:
			return 0x12030001;//��ѹ
			
		case 0x30040700:
			return 0x18010001;//ʧ��
		case 0x30040800:
			return 0x18020001;//ʧ��
		case 0x30040900:
			return 0x18030001;//ʧ��
			
		case 0x30050700:
			return 0x19010001;//����
		case 0x30050800:
			return 0x19020001;//����
		case 0x30050900:
			return 0x19030001;//����
			
		case 0x30030700:
			return 0x13010001;//����
		case 0x30030800:
			return 0x13020001;//����
		case 0x30030900:
			return 0x13030001;//����


		case 0x301B0200:
			return 0x03300D00;//B10�����

		case 0x301C0200:
			return 0x03300E00;//B11����ť��


		case 0x300F0200:
			return 0x14000001;//B64 ��ѹ������

		case 0x301D0200:
			return 0x16000001;//B66 ��ѹ��ƽ��

		case 0x30110200:
			return 0x03110000;//B69 ����

		case 0x300D0200:
			return 0x03050000;//B74 ȫʧѹ

		case 0x30160200:
			return 0x03300400;//B84 Уʱ


		default:
			return 0;
		}
}

u32 Get_Di_From_OAD(u32 OAD)//ͨ��OAD�õ�645�����ݱ�ʶ��ʧ�ܷ���0
{
	switch(OAD)
	{
	case 0x301B0400:
		return 0x03300D00;  //  B10����Ǵ���
	case 0x301B0200:
		return 0x03300D01;//B10�����
	case 0x301C0400:
		return 0x03300E00;// 3 B11����ť�Ǵ���
	case 0x301C0200:
		return 0x03300E01;//B11����ť��
	case 0x30000a00:
		return 0x10010001;//8 B16 A�� B�� C��ʧѹ����
	case 0x30000700:
		return 0x1001FF01;//B16 A��ʧѹ
	case 0x30000800:
		return 0x1002FF01;//B16 B��ʧѹ
	case 0x30000900:
		return 0x1003FF01;//B16 B��ʧѹ
	case 0x30010a00:
		return 0x11010001;//9 B17 A�� B�� C��Ƿѹ����
	case 0x30010700:
		return 0x1101FF01;//B17 A��Ƿѹ
	case 0x30010800:
		return 0x1102FF01;//B17 A��Ƿѹ
	case 0x30010900:
		return 0x1103FF01;//B17 A��Ƿѹ
	case 0x30020a00:
		return 0x12010001;//10 B18 A�� B�� C���ѹ����
	case 0x30020700:
		return 0x1201FF01;//B18 A���ѹ
	case 0x30020800:
		return 0x1202FF01;//B18 B���ѹ
	case 0x30020900:
		return 0x1203FF01;//B18 C���ѹ
	case 0x30040a00:
		return 0x18010001;//11 B19 A�� B�� C��ʧ������
	case 0x30040700:
		return 0x1801FF01;//B19 A��ʧ��
	case 0x30040800:
		return 0x1802FF01;//B19 A��ʧ��
	case 0x30040900:
		return 0x1803FF01;//B19 A��ʧ��
	case 0x30050a00:
		return 0x19010001;//12 B20 A�� B�� C���������
	case 0x30050700:
		return 0x1901FF01;//B20 A�����
	case 0x30050800:
		return 0x1902FF01;//B20 A�����
	case 0x30050900:
		return 0x1903FF01;//B20 A�����
	case 0x30030a00:
		return 0x13010001;//15 B23 A�� B�� C��������
	case 0x30030700:
		return 0x1301FF01;//B23 A�����
	case 0x30030800:
		return 0x1302FF01;//B23 A�����
	case 0x30030900:
		return 0x1303FF01;//B23 A�����
	case 0x300F0400:
		return 0x14000001;//35 B64 ��ѹ���������
	case 0x300F0200:
		return 0x1400FF01;//B64 ��ѹ������
	case 0x301D0400:
		return 0x16000001;//37 B66 ��ѹ��ƽ�����
	case 0x301D0200:
		return 0x1600FF01;//B66 ��ѹ��ƽ��
	case 0x30110400:
		return 0x03110000;//40 B69 �������
	case 0x30110200:
		return 0x03110001;//B69 ����
	case 0x300D0400:
		return 0x03050000;//45 B74 ȫʧѹ����
	case 0x300D0200:
		return 0x03050001;//B74 ȫʧѹ
	case 0x30160400:
		return 0x03300400;//50 B84 Уʱ����
	case 0x30160200:
		return 0x03300401;//B84 Уʱ
	case 0x20140200://���״̬�� ʱ�ӵ��Ƿѹ   ͣ�糭����Ƿѹ
		return 0x040005ff;
	default:
		return 0;
	}
}


u32 Get_698DateTime_st(u8*p8txbuff,u32  type)//��376��ʱ���ʽת��698�ĸ�ʽ
{
	u32 x;

	p8txbuff[0]=0x1c;
	if((type&0x08)!=0)//�ն��ᣬʱ���ǵ�ǰʱ���һ��
	{
		x=MRR(ADDR_TYMDHMS+3,3);//��ǰ������
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		p8txbuff[1]=(bcd_hex(x>>16)+2000)>>8;
		p8txbuff[2]=(bcd_hex(x>>16)+2000)&0xff;
		p8txbuff[3]=bcd_hex((x>>8)&0xff);
		p8txbuff[4]=bcd_hex(x&0xff);
	}
	else//�¶��ᣬʱ���ǵ�ǰʱ���һ��
	{
		x=MRR(ADDR_TYMDHMS+4,2);//��ǰ����
		x=YM_Sub1M(x);//���¼�1��,���ؼ�1�������
		p8txbuff[1]=(bcd_hex(x>>8)+2000)>>8;
		p8txbuff[2]=(bcd_hex(x>>8)+2000)&0xff;
		p8txbuff[3]=bcd_hex(x&0xff);
		p8txbuff[4]=1;
	}
	
	p8txbuff[5]=0x00;
	p8txbuff[6]=0x00;
	p8txbuff[7]=0x00;
	return 8;
}

u32 Change_698DateTime_to_376(u32 add_sour,u32 add_dest)//��698��ʱ�����ת��376��ʽ,�ɹ����س��ȣ�ʧ�ܷ���0
{
	u32 index=0;
	u8* p_sour=(u8*)add_sour;
	u8* p_dest=(u8*)add_dest;
	if(p_sour[0]!=0x1c)
	{
		return 0;
	}
	p_dest[index++]=hex_bcd(p_sour[7]);//ss
	p_dest[index++]=hex_bcd(p_sour[6]);//mm
	p_dest[index++]=hex_bcd(p_sour[5]);//hh
	p_dest[index++]=hex_bcd(p_sour[4]);//DD
	p_dest[index++]=hex_bcd(p_sour[3]);//MM
	if((p_sour[2]+p_sour[1]<<8)==0)
		p_dest[index++]=0;
	else
		p_dest[index++]=hex_bcd((p_sour[2]+(p_sour[1]<<8))-2000);//YY
	return index;
}
u32 Change_698DoubleLongUnsigned_to_376(u32 p_sour,u32 p_dest)//��698��ʱ�����ת��376��ʽ,�ɹ����س��ȣ�ʧ�ܷ���0
{
	u32 value=0;
	u8* pp_dest=(u8*)p_dest;
	if(MRR(p_sour,1)!=PROTO_DT_DOUBLE_LONG_UN)
	{
		return 0;
	}
	value=MRR(p_sour+1,4);//double-long-unsigned
	value=Change_Data(value,4);
	value=hex_bcd(value);

	pp_dest[0]=value&0xff;
	pp_dest[1]=(value>>8)&0xff;
	pp_dest[2]=(value>>16)&0xff;
	pp_dest[3]=(value>>24)&0xff;
	//MWR(value,p_dest,4);

	return 4;
}
u32 Change_698LongUnsigned_to_376(u32 p_sour,u32 p_dest)//��698��ʱ�����ת��376��ʽ,�ɹ����س��ȣ�ʧ�ܷ���0
{
	u32 value=0;
	if(MRR(p_sour,1)!=PROTO_DT_LONG_UN)
	{
		return 0;
	}
	value=MRR(p_sour+1,2);//double-long-unsigned
	value=Change_Data(value,2);
	value=hex_bcd(value);
	MWR(value,p_dest,2);

	return 2;
}
u32 Get_0xee_By_Type(u32 type,u32 p_dest)//���������ͣ���д��Ч����
{
	u32 len;
	u32 index;
	u8* pp_dest=(u8*)p_dest;
	switch(type)
	{
		case PROTO_DT_DOUBLE_LONG:
		case PROTO_DT_DOUBLE_LONG_UN:
			len=4;
			break;
		case PROTO_DT_LONG:
		case PROTO_DT_LONG_UN:
			len=2;
			break;
		case PROTO_DT_DATE_TIME_S:
			len=6;
			break;
		default:
			return 0;
	}
	for(index=0;index<len;index++)
	{
		pp_dest[index]=0xee;
	}
	return len;
}
u32 Get_Len_Of_Structure(u32 member_num, u8* apdu)//ȡ�ṹ����APDU�����ݳ���,ʧ�ܷ���0���ɹ����س���
{
	u32 i;
	u32 field_len, data_len; 
	u32 len=0;
	u32 ret;

	for(i=0;i<member_num;i++) 
	{
		switch(apdu[len])
		{
		case PROTO_DT_NULL:				//NULL
			len += 1;
			break;
		case PROTO_DT_BOOL:				//BOOL
		case PROTO_DT_INTEGER:			//integer
		case PROTO_DT_UNSIGNED:			//unsigned
		case PROTO_DT_ENUM:				//enum
			len += 2;
			break;
		case PROTO_DT_LONG:			//long
		case PROTO_DT_LONG_UN:		//long unsigned
		case PROTO_DT_OI:			//OI
		case PROTO_DT_SCALER_UNIT:	//Scaler_Unit
 			len += 3;
			break;		
		case PROTO_DT_TIME:
		case PROTO_DT_TI:
			len += 4;
			break;
		case PROTO_DT_DOUBLE_LONG:
		case PROTO_DT_DOUBLE_LONG_UN:
		case PROTO_DT_LONG64:			//long64
		case PROTO_DT_LONG64_UN:		//long64-unsigned
		case PROTO_DT_FLOAT32:			//float32
		case PROTO_DT_OAD:				//OAD
		case PROTO_DT_OMD:				//OMD
			len += 5;
			break;
		case PROTO_DT_DATE:
			len += 6;
			break;
		case PROTO_DT_DATE_TIME_S:
			len += 8;
			break;
		case PROTO_DT_FLOAT64:			//float64
			len += 9;
			break;
		case PROTO_DT_DATE_TIME:
			len += 11;
			break;
		case PROTO_DT_BIT_STRING:		//λ��
			if(apdu[1+len]&0x80) 
			{
				field_len = apdu[1+len] & 0x7f;
				if(field_len==1)// ����λ��1 
				{
					data_len = apdu[2+len];
					len+=2;
				}
				else if(field_len==2)// ����λ��2 
				{
					data_len=MRR(((u32)apdu)+2+len,2);
					data_len=Change_Data(data_len,2);
					len += 3;
				}
				else //����λ����2ֱ�Ӵ���̫����
				{
					return 0;
				}
			}
			else 
			{
				data_len = apdu[1+len];
				len++;
			}
			len++;		//����id�ĳ���
			len+= data_len/8+((data_len%8)?1:0);	//�������ݵĳ���
			break;
		case PROTO_DT_OCTET_STRING:		//�ֽڴ�
		case PROTO_DT_VISIBLE_STRING:
		case PROTO_DT_UTF8_STING:
		case PROTO_DT_TSA:
		case PROTO_DT_MAC:
		case PROTO_DT_RN:
			if(apdu[1+len]&0x80) 
			{
				field_len=apdu[1+len]&0x7f;
				if(field_len==1) 
				{
					data_len = apdu[2+len];
					len+=2;
				}
				else if(field_len==2) 
				{
					data_len=MRR(((u32)apdu)+2+len,2);
					data_len=Change_Data(data_len,2);
					len+=3;
				}
				else 
				{
					return 0;
				}
			}
			else 
			{
				data_len=apdu[1+len];
				len++;
			}
			len++;		//����id�ĳ���
			len+=data_len;		//�������ݵĳ���
			break;
		case PROTO_DT_ROAD:
			data_len=5+apdu[5+len]*4;
			len+=data_len+1;
			break;
		case PROTO_DT_ARRAY:			//array
		case PROTO_DT_STRUCTURE:		//structure
			ret=Get_Len_Of_Structure(apdu[1+len], apdu+2+len);
			if(ret==0)
			{
				return 0;
			}
			len+=ret+2;
			break;
		case PROTO_DT_SID:
			len++;		//ID�ĳ���
			len+=5+apdu[len+4];
			break;
		case PROTO_DT_SID_MAC:
			len++;		//ID�ĳ���
			len+=5+apdu[len+4];		//SID�ĳ���
			len+=1+apdu[len];			//MAC�ĳ���
			break;
		case PROTO_DT_COMDCB:
			len+=6;
			break;
		default:
			return 0;
		}
	}

	return len;
}

u32 Get_OAD_Data_Position(u8*p8txbuff,u32 oad,u32 no)//������֡���ѯָ��OAD�����ݣ��������ݵ���ʼλ�ã������������͡�ʧ�ܷ���0
{
	u32 oad_num;
	u32 i;
	u32 index;
	u32 frame_oad;
	oad_num=p8txbuff[0];
	if(oad_num==0)
		return 0;
	index=1;
	for(i=0;i<oad_num;i++)
	{
		index++;//ȥ�� OAD  ��  ROAD ѡ����λ
		frame_oad=MRR((u32)p8txbuff+index,4);
		frame_oad=Change_Data(frame_oad,4);
		if((frame_oad&0xffff0000)==(oad&0xffff0000))//�ҵ�ָ����OAD������OAD����2λ�п��ܲ���ȷ�������ֻ�ж�OI ��
		{
			no--;//no ��ʾҪ�ҵڼ���
			if(no==0)
			{
				break;
			}
			else
			{
				index+=4;
			}
		}
		else
		{
			index+=4;
		}
	}
	if(i>=oad_num)//��������ҪOAD������
	{
		return 0;
	}
	if(p8txbuff[1+oad_num*5]!=1)//��¼��������
	{
		return 0;
	}
	if(p8txbuff[1+oad_num*5+1]!=1)//��������1
	{
		return 0;
	}
	index=1+oad_num*5+2;//��λ�����ݿ�ʼ�ĵط�

	oad_num=i;//���ݲ�����Ҫ�����Ĳ���
	for(i=0;i<oad_num;i++)
	{
		frame_oad=Get_Len_Of_Structure(1,p8txbuff+index);
		if(frame_oad==0)
		{
			return 0;
		}
		else
		{
			index+=frame_oad;
		}
	}
	return index;
	
}
u32 Change_Form_698_to_376(u32 Oad,u32 ADDR_Dest,u32 ADDR_Sour)//��698���¼���¼ת��645-2007�ĸ�ʽ������Ǽ�¼,�ɹ������ֽ�����ʧ�ܷ���0
{
	u32 index=0;
	u32 pos=0;
	u32 i=0;
	u8* p_lib;

	u32 oad11;//Ҫ���OAD
	u32 no;//OAD��˳����ࡢ�ṹ�ĵڼ�����Ա
	u32 type;//��������
	u32 array_type;//�ࡢ�ṹ�������͵ĳ�Ա����
	if(((Oad&0x0000ff00)==0x0700)||((Oad&0x0000ff00)==0x0800)||((Oad&0x0000ff00)==0x0900))
		Oad=0x30000700;
	i=sizeof(DL698_to_DL376_Lib)/4;
	for(index=0;index<i;index++)//����Ӧ��ת����Լ��
	{
		p_lib=(u8 *)DL698_to_DL376_Lib[index];
		pos=MRR((u32)p_lib,4);
		if(pos==Oad)
			break;
	}
	if(index>=i)
		return 0;
	pos=0;
	for(i=0;i<p_lib[4];i++)
	{
		oad11=MRR(((u32)p_lib)+5+i*7,4);//��¼Ҫ���OAD
		no=p_lib[5+i*7+4];//����λ��ʾ�ڼ���������λ��ʾ�ڼ�����Ա
		type=p_lib[5+i*7+5];//��������
		array_type=p_lib[5+i*7+6];//�ࡢ�ṹ�������͵ĳ�Ա����
		//MWR(oad11,ADDR_Dest+pos,4);
		//Data_Inverse(ADDR_Dest+pos,4);
		//pos+=4;//�����ã����Ƿ����˿��������oad
		index=Get_OAD_Data_Position((u8*)ADDR_Sour,oad11,no&0xf);
		if(index==0)//û�ҵ�����0xee
		{
			if(type==PROTO_DT_ARRAY)
				index=Get_0xee_By_Type(array_type,ADDR_Dest+pos);
			else
				index=Get_0xee_By_Type(type,ADDR_Dest+pos);
			if(index==0)
				return 0;
			else
				pos+=index;
			continue;
		}
		switch(type)
		{
			case PROTO_DT_ARRAY:
				index+=2;//ȥ�� 01 xx ��2λ

				switch(array_type)
				{
					case PROTO_DT_LONG:
					case PROTO_DT_LONG_UN:
						index+=3*((no>>4)-1);
						index=Change_698LongUnsigned_to_376(ADDR_Sour+index,ADDR_Dest+pos);
						if(index==0)
							return 0;
						else
							pos+=index;
						break;
					case PROTO_DT_DOUBLE_LONG:
					case PROTO_DT_DOUBLE_LONG_UN:
						index+=5*((no>>4)-1);
						index=Change_698DoubleLongUnsigned_to_376(ADDR_Sour+index,ADDR_Dest+pos);
						if(index==0)
							return 0;
						else
							pos+=index;
						break;
					default:
						return 0;
				}
				break;
			case PROTO_DT_DOUBLE_LONG:
			case PROTO_DT_DOUBLE_LONG_UN:
				index=Change_698DoubleLongUnsigned_to_376(ADDR_Sour+index,ADDR_Dest+pos);
				if(index==0)
					return 0;
				else
					pos+=index;
				break;
			case PROTO_DT_LONG_UN:
			case PROTO_DT_LONG:
				index=Change_698LongUnsigned_to_376(ADDR_Sour+index,ADDR_Dest+pos);
				if(index==0)
					return 0;
				else
					pos+=index;
				break;
			case PROTO_DT_DATE_TIME_S:
				index=Change_698DateTime_to_376(ADDR_Sour+index,ADDR_Dest+pos);
				if(index==0)
					return 0;
				else
					pos+=index;
				break;
			default:
				return 0;
		}
	}
	 return pos;
}

u32 Change_Data(u32 data,u32 type)//�ߵ�����data �ĸߵ�λ��type���ֽ���,type������С�ڵ���4�����ڵ���1��
{
	u8 v1=data&0xff;
	u8 v2=(data>>8)&0xff;
	u8 v3=(data>>16)&0xff;
	u8 v4=(data>>24)&0xff;
	if(type==1)
		return v1;
	else if(type==2)
		return (v1<<8)+v2;
	else if(type==3)
		return (v1<<16)+(v2<<8)+v3;
	else
		return (v1<<24)+(v2<<16)+(v3<<8)+v4;
}
//Log_Printf_Level����������Ա���ͨ���������޷�ʹ��
//������һ���Ӿͳ��������ǲ�֧�� �β��� ...
//�⺯������ʹ��
void Log_Printf_Level(u32 log_level,const char* fmt,...)//����sprintf�ķ�ʽ����ӡ��־���ļ�log_level������־�ȼ�������ϵͳĬ����־���𽫲���ӡ
{
	char *p_buf=(char *)ADDR_DATABUFF;
	u8 *p_date=(u8 *)ADDR_TYMDHMS;
	u32 len;
	va_list args;

	if(log_level>CURRENT_LOG_LEVEL)//��־���𲻹�������ӡ
		return ;	
	
	sprintf(p_buf,"[%02d-%02d %02d:%02d:%02d] ",p_date[4],p_date[3],p_date[2],p_date[1],p_date[0]);
	
	va_start(args,fmt);
	len=vsprintf(p_buf+16,(char *)fmt,args);
	va_end(args);

	if(len==0)
		return;
	
	Write_Log_To_File(ADDR_DATABUFF,strlen(p_buf));
}

void Log_Dumphex_Level(u32 log_level,u8 *title,u32 addr_log,u32 log_len)//��ӡһ�����ݵ���־�ļ�
{
	char *p_buf=(char *)ADDR_DATABUFF;
	u8 *p_date=(u8 *)ADDR_TYMDHMS;
	u8 *p_data=(u8 *)addr_log;
	u32 index;
	u32 pos=0;
	if(log_level>CURRENT_LOG_LEVEL)//��־���𲻹�������ӡ
		return ;
	MC(0,ADDR_DATABUFF,LEN_DATABUFF);
	if(title!=NULL)
		sprintf(p_buf,"\n[%02d-%02d %02d:%02d:%02d]  %s\n",p_date[4],p_date[3],p_date[2],p_date[1],p_date[0],title);
	pos=strlen(p_buf);
	for(index=0;index<log_len;index++)
	{
		sprintf(p_buf+pos,"%02x ",p_data[index]);
		pos=strlen(p_buf);
	}
	sprintf(p_buf+pos,"\n");	
	pos+=1;
	
	Write_Log_To_File(ADDR_DATABUFF,pos);
}

//��־�ļ���С������LOG_MAX_LEN��Ŀǰ��1M
u32 Write_Log_To_File(u32 addr_data,u32 data_len)//����־�ļ�д��־������д����־�ĳ��ȣ�0��ʾʧ�ܡ�
{
	FILE* p_file=NULL;
	os_Type *os=NULL;
	u32 ret=0;
	u8 *p_data=(u8 *)addr_data;
	
	os=Get_os();
	if(os==NULL)//���ϵͳʧ��
		return 0;

	p_file=os->fopen("N0:log.txt","a");
	if(p_file==NULL)//���ļ�ʧ��
		return 0;
	ret=os->ftell(p_file);//��õ�ǰ��־�ļ��ĳ���
	if((ret+data_len)>LOG_MAX_LEN)//���β�����Ҫ�����ˣ�
		os->fseek(p_file,0,SEEK_SET);//��ͷ��ʼд
	ret=os->fwrite(p_data,1,data_len,p_file);
	os->fclose(p_file);
	return ret;
}

u32 Add_Log_Date_Head(u32 Addr)//���ʱ���������ʱ����ĳ���
{
	u8 *p_date=(u8 *)ADDR_TYMDHMS;
	char *p_buf=(char *)Addr;
	sprintf(p_buf,"[%02d-%02d %02d:%02d:%02d] ",p_date[4],p_date[3],p_date[2],p_date[1],p_date[0]);
	return 17;
}
#endif



