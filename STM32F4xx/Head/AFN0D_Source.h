
//���г�����2���������¶���ԭʼ����(��ʼֵΪ����ֵ0xFF)
//(2��������1-24��������������й�����ʾֵԭʼ������RAM,���ڴ�)

#ifndef AFN0D_Source_H
#define AFN0D_Source_H

//������1
#define	ADDR_AFN0D_Source_Date_Start     ADDR_AFN0D_Source_Start//(4K����)��ʼ��ַ
	//��2�����ն���ԭʼ����(����)
#define  ADDR_AFN0D_DateSource_Date     ADDR_AFN0D_Source_Date_Start//3byte �����й�4�������޹�,�ն˳���ʱ��������,�½���ADDR_AFN0DF161_Source����Ѱַ
#define  ADDR_AFN0DF161_Source      ADDR_AFN0D_DateSource_Date+3//�����������й�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF162_Source      ADDR_AFN0DF161_Source+(6+(5*(1+TMaxTariff)))//�����������޹�������޹�1������ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF163_Source      ADDR_AFN0DF162_Source+(6+(4*(1+TMaxTariff)))//�����㷴���й�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF164_Source      ADDR_AFN0DF163_Source+(6+(5*(1+TMaxTariff)))//�����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF165_Source      ADDR_AFN0DF164_Source+(6+(4*(1+TMaxTariff)))//������1�����޹�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF166_Source      ADDR_AFN0DF165_Source+(6+(4*(1+TMaxTariff)))//������2�����޹�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF167_Source      ADDR_AFN0DF166_Source+(6+(4*(1+TMaxTariff)))//������3�����޹�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF168_Source      ADDR_AFN0DF167_Source+(6+(4*(1+TMaxTariff)))//������4�����޹�����ʾֵ���ܡ�����1��M��

#define  ADDR_AFN0DF185_Source      ADDR_AFN0DF168_Source+(6+(4*(1+TMaxTariff)))//�����������й��������������ʱ�䣨�ܡ�����1��M��
#define  ADDR_AFN0DF186_Source      ADDR_AFN0DF185_Source+(6+(7*(1+TMaxTariff)))//�����������޹��������������ʱ�䣨�ܡ�����1��M��
#define  ADDR_AFN0DF187_Source      ADDR_AFN0DF186_Source+(6+(7*(1+TMaxTariff)))//�����㷴���й��������������ʱ�䣨�ܡ�����1��M��
#define  ADDR_AFN0DF188_Source      ADDR_AFN0DF187_Source+(6+(7*(1+TMaxTariff)))//�����㷴���޹��������������ʱ�䣨�ܡ�����1��M��
#define  ADDR_AFN0DF209_Source      ADDR_AFN0DF188_Source+(6+(7*(1+TMaxTariff)))//��������ܱ�Զ�̿���ͨ�ϵ�״̬����¼
#if(USER/100)==12//�Ĵ�
#define  ADDR_AFN0DF210_Source      ADDR_AFN0DF209_Source+(5+13)//�ն�����ܱ����õ���Ϣ
#define ADDR_AFN0D_Source_Date_End     (ADDR_AFN0DF210_Source+(5+12))
#endif

#ifndef ADDR_AFN0D_Source_Date_End
#define ADDR_AFN0D_Source_Date_End     (ADDR_AFN0DF209_Source+(5+13))
#endif

#define LEN_AFN0D_Source_Date     (ADDR_AFN0D_Source_Date_End-ADDR_AFN0D_Source_Date_Start)

	//��2�����¶���ԭʼ����
#define ADDR_AFN0D_Source_Month_Start     ADDR_AFN0D_Source_Date_End//��ʼ��ַ
#define  ADDR_AFN0D_MonthSource_Date     ADDR_AFN0D_Source_Month_Start//2byte �����й�4�������޹�,�ն˳���ʱ������,�½���ADDR_AFN0DF177_Source����Ѱַ
#define  ADDR_AFN0DF177_Source      ADDR_AFN0D_MonthSource_Date+2//�����������й�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF178_Source      ADDR_AFN0DF177_Source+(6+(5*(1+TMaxTariff)))//�����������޹�������޹�1������ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF179_Source      ADDR_AFN0DF178_Source+(6+(4*(1+TMaxTariff)))//�����㷴���й�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF180_Source      ADDR_AFN0DF179_Source+(6+(5*(1+TMaxTariff)))//�����㷴���޹�������޹�1������ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF181_Source      ADDR_AFN0DF180_Source+(6+(4*(1+TMaxTariff)))//������1�����޹�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF182_Source      ADDR_AFN0DF181_Source+(6+(4*(1+TMaxTariff)))//������2�����޹�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF183_Source      ADDR_AFN0DF182_Source+(6+(4*(1+TMaxTariff)))//������3�����޹�����ʾֵ���ܡ�����1��M��
#define  ADDR_AFN0DF184_Source      ADDR_AFN0DF183_Source+(6+(4*(1+TMaxTariff)))//������4�����޹�����ʾֵ���ܡ�����1��M��

#define  ADDR_AFN0DF193_Source      ADDR_AFN0DF184_Source+(6+(4*(1+TMaxTariff)))//�����������й��������������ʱ�䣨�ܡ�����1��M��
#define  ADDR_AFN0DF194_Source      ADDR_AFN0DF193_Source+(6+(7*(1+TMaxTariff)))//�����������޹��������������ʱ�䣨�ܡ�����1��M��
#define  ADDR_AFN0DF195_Source      ADDR_AFN0DF194_Source+(6+(7*(1+TMaxTariff)))//�����㷴���й��������������ʱ�䣨�ܡ�����1��M��
#define  ADDR_AFN0DF196_Source      ADDR_AFN0DF195_Source+(6+(7*(1+TMaxTariff)))//�����㷴���޹��������������ʱ�䣨�ܡ�����1��M��
#define  ADDR_AFN0DF213_Source      ADDR_AFN0DF196_Source+(6+(7*(1+TMaxTariff)))//��������ܱ��ز���������ʱ��
#define  ADDR_AFN0DF214_Source      ADDR_AFN0DF213_Source+(5+18)//��������ܱ�����޸Ĵ�����ʱ��
#define  ADDR_AFN0DF215_Source      ADDR_AFN0DF214_Source+(5+24)//��������ܱ����õ���Ϣ
#define  ADDR_AFN0DF216_Source      ADDR_AFN0DF215_Source+(5+36)//��������ܱ������Ϣ

#define ADDR_AFN0D_Source_Month_End     (ADDR_AFN0DF216_Source+(6+(10*(1+TMaxTariff))))
#define LEN_AFN0D_Source_Month     (ADDR_AFN0D_Source_Month_End-ADDR_AFN0D_Source_Month_Start)


#if ((ADDR_AFN0D_Source_Month_End-ADDR_AFN0D_Source_Start)>=1024)
#define LEN_AFN0D_Source     (ADDR_AFN0D_Source_Month_End-ADDR_AFN0D_Source_Start)
#else
#define LEN_AFN0D_Source     1024
#endif

//������2
	//��2�����ն���ԭʼ����
	//��2�����¶���ԭʼ����
//...
//������Max
	//��2�����ն���ԭʼ����
	//��2�����¶���ԭʼ����


#define  ADDR_LastDate_Source_Start     ((ADDR_AFN0D_Source_Date_Start+(LEN_AFN0D_Source*MaxRS485AddCarrierPn)+0xfff)&0xfffff000)//(4K����)���������1-3���ն���ԭʼ���ݵ�ַ
//���������2���ն���ԭʼ����
//������1
#define  Office_LastDate_Date     0
//3byte ���ܱ���ʱ��������(�������ո��º��0ʱ0�ֶ���)
#define  Office_AFN0DF161_LastDate_Source     Office_LastDate_Date+3
//6+(5*(1+TMaxTariff))byte F161 ����ʱ��+������+�����й�����ʾֵ���ܡ�����1��M��
#define  Office_AFN0DF162_LastDate_Source     Office_AFN0DF161_LastDate_Source+(6+(5*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F162 ����ʱ��+������+�����޹�������޹�1������ʾֵ���ܡ�����1��M��
#define  Office_AFN0DF163_LastDate_Source     Office_AFN0DF162_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(5*(1+TMaxTariff))byte F163 ����ʱ��+������+�����й�����ʾֵ���ܡ�����1��M��
#define  Office_AFN0DF164_LastDate_Source     Office_AFN0DF163_LastDate_Source+(6+(5*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F164 ����ʱ��+������+�����޹�������޹�2������ʾֵ���ܡ�����1��M��
#define  Office_AFN0DF165_LastDate_Source     Office_AFN0DF164_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F165 ����ʱ��+������+1�����޹�����ʾֵ���ܡ�����1��M��
#define  Office_AFN0DF166_LastDate_Source     Office_AFN0DF165_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F166 ����ʱ��+������+2�����޹�����ʾֵ���ܡ�����1��M��
#define  Office_AFN0DF167_LastDate_Source     Office_AFN0DF166_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F167 ����ʱ��+������+3�����޹�����ʾֵ���ܡ�����1��M��
#define  Office_AFN0DF168_LastDate_Source     Office_AFN0DF167_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(4*(1+TMaxTariff))byte F168 ����ʱ��+������+4�����޹�����ʾֵ���ܡ�����1��M��

#if (((USER/100)==5)&&(Project==300))//�Ϻ�ר��
#define  Office_AFN0DF185_LastDate_Source     Office_AFN0DF168_LastDate_Source+(6+(4*(1+TMaxTariff)))
//6+(7*(1+TMaxTariff))byte F168 ����ʱ��+������+�����й��������������ʱ�䣨�ܡ�����1��M��
#define  Office_AFN0DF187_LastDate_Source     Office_AFN0DF185_LastDate_Source+(6+(7*(1+TMaxTariff)))
//6+(7*(1+TMaxTariff))byte F168 ����ʱ��+������+�����й��������������ʱ�䣨�ܡ�����1��M��
#define  LEN_LastDate_Source_PerPn     (Office_AFN0DF187_LastDate_Source+(6+(7*(1+TMaxTariff))))
#endif

#if(USER/100)==12//�Ĵ�
#define  Office_AFN0DF210_LastDate_Source      Office_AFN0DF168_LastDate_Source+(6+(4*(1+TMaxTariff)))//�ն�����ܱ����õ���Ϣ
//5+12 ����ʱ��+�������+ʣ����+�ۼƹ�����
#define LEN_LastDate_Source_PerPn     (Office_AFN0DF210_LastDate_Source+17)
#endif

#ifndef LEN_LastDate_Source_PerPn
#define  LEN_LastDate_Source_PerPn     (Office_AFN0DF168_LastDate_Source+(6+(4*(1+TMaxTariff))))
#endif

//������2
//...
//������Max
//...
//���������3���ն���ԭʼ����
//...
#define  ADDR_LastDate_Source_End     (ADDR_LastDate_Source_Start+(2*LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn))




#define ADDR_AFN0D_Source_End     ADDR_LastDate_Source_End
#if ((ADDR_AFN0D_Source_End-ADDR_AFN0D_Source_Date_Start)>LEN_Terminal_EFLASH_AFN0D_Source)
	#error "((ADDR_AFN0D_Source_End-ADDR_AFN0D_Source_Date_Start)>LEN_Terminal_EFLASH_AFN0D_Source)"
#endif


#endif

















