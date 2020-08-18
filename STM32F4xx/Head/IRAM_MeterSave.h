
#ifndef IRAM_MeterSave_H
#define IRAM_MeterSave_H


//�������൱ǰ����--------------------------------------------------------------------------
#define ADDR_MeterEnergy_IRAM_Start     ADDR_Meter_SaveRam_Start//�Ǳ������������IRAM��ʼ��ַ
#define ADDR_MFM_YMDHMS     ADDR_MeterEnergy_IRAM_Start//����ǰʱ��
//��,��,ʱ,��,��,��
#define ADDR_BATTERY_TIMER     ADDR_MFM_YMDHMS+6//��ع�����ʱ�ۼ�(��)
//4BYTE BCD��
#define ADDR_MeterFMTimer0     ADDR_BATTERY_TIMER+4//�ֶ�ʱ��
//6BYTE �ϴμ�������ʱ������
//2BYTE ��Ӽ��������
#define ADDR_MeterFMTimer     ADDR_MeterFMTimer0+8//�ּ���ʱ��(�����ϲ��ֿ�)
//2BYTE  ���ü����·ֶ�ʱ��hex
//2BYTE  ���ܱ�2����������ֶ�ʱ��hex
//2BYTE  ���ܱ�4����������ֶ�ʱ��hex
//2BYTE  
//2BYTE  
//2BYTE  
//2BYTE  
//2BYTE  ����ģʽ��Ч�ֶ�ʱ��hex
#define MeterFMTimer_Num     8//�ֶ�ʱ����
#define ADDR_AH     ADDR_MeterFMTimer+(2*MeterFMTimer_Num)//��ʱ
//4BYTE HEX A��000000.00��ʱ
//4BYTE HEX B��000000.00��ʱ
//4BYTE HEX C��000000.00��ʱ
//2BYTE HEX A���������,1��ʱ=1000*3600������,0.01��ʱ=36000������
//2BYTE HEX B���������,1��ʱ=1000*3600������,0.01��ʱ=36000������
//2BYTE HEX C���������,1��ʱ=1000*3600������,0.01��ʱ=36000������
#define ADDR_00000000     ADDR_AH+18//��ǰ����й�����
//5*(1+M)BYTE BCD NNNNNN.NNNN ��ǰ����й���,����1-M����
#define ADDR_00010000     ADDR_00000000+(5*(1+MMaxTariff))//�����й�����
//5*(1+M)BYTE BCD NNNNNN.NNNN��,����1-M����
//1*(1+M)BYTE HEX ��,����1-M��Ƶ
//4BYTE CRC32
#define ADDR_00020000     ADDR_00010000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//�����й�����
//5*(1+M)BYTE BCD NNNNNN.NNNN��,����1-M����
//1*(1+M)BYTE HEX ��,����1-M��Ƶ
//4BYTE CRC32
#define ADDR_00030000     ADDR_00020000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//����޹�1����
//5*(1+M)BYTE BCD NNNNNN.NNNN ��ǰ����޹���,����1-M����
#define ADDR_00040000     ADDR_00030000+(5*(1+MMaxTariff))//����޹�2����
//5*(1+M)BYTE BCD NNNNNN.NNNN ��ǰ����޹���,����1-M��Ƶ
#define ADDR_00050000     ADDR_00040000+(5*(1+MMaxTariff))//��1�����޹�����
//5*(1+M)BYTE BCD NNNNNN.NNNN ��,����1-M����
//1*(1+M)BYTE HEX ��,����1-M��Ƶ
//4BYTE CRC32
#define ADDR_00060000     ADDR_00050000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//��2�����޹�����
//5*(1+M)BYTE BCD NNNNNN.NNNN��,����1-M����
//1*(1+M)BYTE HEX ��,����1-M��Ƶ
//4BYTE CRC32
#define ADDR_00070000     ADDR_00060000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//��3�����޹�����
//5*(1+M)BYTE BCD NNNNNN.NNNN��,����1-M����
//1*(1+M)BYTE HEX ��,����1-M��Ƶ
//4BYTE CRC32
#define ADDR_00080000     ADDR_00070000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//��4�����޹�����
//5*(1+M)BYTE BCD NNNNNN.NNNN��,����1-M����
//1*(1+M)BYTE HEX ��,����1-M��Ƶ
//4BYTE CRC32
#define ADDR_00090000     ADDR_00080000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//�������ڵ���
//5*(1+M)BYTE BCD NNNNNN.NNNN��,����1-M����
//1*(1+M)BYTE HEX ��,����1-M��Ƶ
//4BYTE CRC32
#define ADDR_000A0000     ADDR_00090000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//�������ڵ���
//5*(1+M)BYTE BCD NNNNNN.NNNN��,����1-M����
//1*(1+M)BYTE HEX ��,����1-M��Ƶ
//4BYTE CRC32
#define ADDR_000C0000     ADDR_000A0000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//��ǰ�¶�����й����ۼ��õ���
//4BYTE BCD NNNNNN.NN
#define ADDR_000D0000     ADDR_000C0000+4//��ǰ�������������й����ۼ��õ���
//4BYTE BCD YMDH��ǰ����ʱ��������ʱ
//4BYTE BCD NNNNNN.NN��ǰ�������������й����ۼ��õ���

#define ADDR_00800000     ADDR_000D0000+8//�������ܵ���
//5BYTE BCD  NNNNNN.NNNN �����ܵ���
//5BYTE BCD  NNNNNN.NNNN ���й������ܵ���
//5BYTE BCD  NNNNNN.NNNN ���й������ܵ���
//5BYTE BCD  NNNNNN.NNNN ���й�г���ܵ���
//5BYTE BCD  NNNNNN.NNNN ���й�г���ܵ���
//5BYTE BCD  NNNNNN.NNNN ͭ���й������ܵ���
//5BYTE BCD  NNNNNN.NNNN �����й������ܵ���

//1BYTE HEX �������ܷ�Ƶ
//1BYTE HEX ���й��������ܷ�Ƶ
//1BYTE HEX ���й��������ܷ�Ƶ
//1BYTE HEX ���й�г�����ܷ�Ƶ
//1BYTE HEX ���й�г�����ܷ�Ƶ
//1BYTE HEX ͭ���й����ܷ�Ƶ
//1BYTE HEX �����й����ܷ�Ƶ
//4BYTE CRC32
#define ADDR_00150000     ADDR_00800000+35+7+4//A����
//5BYTE BCD  NNNNNN.NNNN �������й�����
//5BYTE BCD  NNNNNN.NNNN �෴���й�����
//5BYTE BCD  NNNNNN.NNNN ������޹� 1 ����
//5BYTE BCD  NNNNNN.NNNN ������޹� 2 ����
//5BYTE BCD  NNNNNN.NNNN ���һ�����޹�����
//5BYTE BCD  NNNNNN.NNNN ��ڶ������޹�����
//5BYTE BCD  NNNNNN.NNNN ����������޹�����
//5BYTE BCD  NNNNNN.NNNN ����������޹�����
//5BYTE BCD  NNNNNN.NNNN ���������ڵ���
//5BYTE BCD  NNNNNN.NNNN �෴�����ڵ���
//5BYTE BCD  NNNNNN.NNNN ���������
//5BYTE BCD  NNNNNN.NNNN �������й���������
//5BYTE BCD  NNNNNN.NNNN �෴���й���������
//5BYTE BCD  NNNNNN.NNNN �������й�г������
//5BYTE BCD  NNNNNN.NNNN �෴���й�г������
//5BYTE BCD  NNNNNN.NNNN ��ͭ���й����ܲ�����
//5BYTE BCD  NNNNNN.NNNN �������й����ܲ�����

//1BYTE HEX �������й����ܷ�Ƶ
//1BYTE HEX �෴���й����ܷ�Ƶ
//1BYTE HEX ������޹� 1 ���ܷ�Ƶ
//1BYTE HEX ������޹� 2 ���ܷ�Ƶ
//1BYTE HEX ���һ�����޹����ܷ�Ƶ
//1BYTE HEX ��ڶ������޹����ܷ�Ƶ
//1BYTE HEX ����������޹����ܷ�Ƶ
//1BYTE HEX ����������޹����ܷ�Ƶ
//1BYTE HEX ���������ڵ��ܷ�Ƶ
//1BYTE HEX �෴�����ڵ��ܷ�Ƶ
//1BYTE HEX ��������ܷ�Ƶ
//1BYTE HEX �������й��������ܷ�Ƶ
//1BYTE HEX �෴���й��������ܷ�Ƶ
//1BYTE HEX �������й�г�����ܷ�Ƶ
//1BYTE HEX �෴���й�г�����ܷ�Ƶ
//1BYTE HEX ��ͭ���й����ܲ�������Ƶ
//1BYTE HEX �������й����ܲ�������Ƶ
//4BYTE CRC32
#define ADDR_00290000     ADDR_00150000+85+17+4//B����
//5BYTE BCD  NNNNNN.NNNN �������й�����
//5BYTE BCD  NNNNNN.NNNN �෴���й�����
//5BYTE BCD  NNNNNN.NNNN ������޹� 1 ����
//5BYTE BCD  NNNNNN.NNNN ������޹� 2 ����
//5BYTE BCD  NNNNNN.NNNN ���һ�����޹�����
//5BYTE BCD  NNNNNN.NNNN ��ڶ������޹�����
//5BYTE BCD  NNNNNN.NNNN ����������޹�����
//5BYTE BCD  NNNNNN.NNNN ����������޹�����
//5BYTE BCD  NNNNNN.NNNN ���������ڵ���
//5BYTE BCD  NNNNNN.NNNN �෴�����ڵ���
//5BYTE BCD  NNNNNN.NNNN ���������
//5BYTE BCD  NNNNNN.NNNN �������й���������
//5BYTE BCD  NNNNNN.NNNN �෴���й���������
//5BYTE BCD  NNNNNN.NNNN �������й�г������
//5BYTE BCD  NNNNNN.NNNN �෴���й�г������
//5BYTE BCD  NNNNNN.NNNN ��ͭ���й����ܲ�����
//5BYTE BCD  NNNNNN.NNNN �������й����ܲ�����

//1BYTE HEX �������й����ܷ�Ƶ
//1BYTE HEX �෴���й����ܷ�Ƶ
//1BYTE HEX ������޹� 1 ���ܷ�Ƶ
//1BYTE HEX ������޹� 2 ���ܷ�Ƶ
//1BYTE HEX ���һ�����޹����ܷ�Ƶ
//1BYTE HEX ��ڶ������޹����ܷ�Ƶ
//1BYTE HEX ����������޹����ܷ�Ƶ
//1BYTE HEX ����������޹����ܷ�Ƶ
//1BYTE HEX ���������ڵ��ܷ�Ƶ
//1BYTE HEX �෴�����ڵ��ܷ�Ƶ
//1BYTE HEX ��������ܷ�Ƶ
//1BYTE HEX �������й��������ܷ�Ƶ
//1BYTE HEX �෴���й��������ܷ�Ƶ
//1BYTE HEX �������й�г�����ܷ�Ƶ
//1BYTE HEX �෴���й�г�����ܷ�Ƶ
//1BYTE HEX ��ͭ���й����ܲ�������Ƶ
//1BYTE HEX �������й����ܲ�������Ƶ
//4BYTE CRC32
#define ADDR_003D0000     ADDR_00290000+85+17+4//C����
//5BYTE BCD  NNNNNN.NNNN �������й�����
//5BYTE BCD  NNNNNN.NNNN �෴���й�����
//5BYTE BCD  NNNNNN.NNNN ������޹� 1 ����
//5BYTE BCD  NNNNNN.NNNN ������޹� 2 ����
//5BYTE BCD  NNNNNN.NNNN ���һ�����޹�����
//5BYTE BCD  NNNNNN.NNNN ��ڶ������޹�����
//5BYTE BCD  NNNNNN.NNNN ����������޹�����
//5BYTE BCD  NNNNNN.NNNN ����������޹�����
//5BYTE BCD  NNNNNN.NNNN ���������ڵ���
//5BYTE BCD  NNNNNN.NNNN �෴�����ڵ���
//5BYTE BCD  NNNNNN.NNNN ���������
//5BYTE BCD  NNNNNN.NNNN �������й���������
//5BYTE BCD  NNNNNN.NNNN �෴���й���������
//5BYTE BCD  NNNNNN.NNNN �������й�г������
//5BYTE BCD  NNNNNN.NNNN �෴���й�г������
//5BYTE BCD  NNNNNN.NNNN ��ͭ���й����ܲ�����
//5BYTE BCD  NNNNNN.NNNN �������й����ܲ�����

//1BYTE HEX �������й����ܷ�Ƶ
//1BYTE HEX �෴���й����ܷ�Ƶ
//1BYTE HEX ������޹� 1 ���ܷ�Ƶ
//1BYTE HEX ������޹� 2 ���ܷ�Ƶ
//1BYTE HEX ���һ�����޹����ܷ�Ƶ
//1BYTE HEX ��ڶ������޹����ܷ�Ƶ
//1BYTE HEX ����������޹����ܷ�Ƶ
//1BYTE HEX ����������޹����ܷ�Ƶ
//1BYTE HEX ���������ڵ��ܷ�Ƶ
//1BYTE HEX �෴�����ڵ��ܷ�Ƶ
//1BYTE HEX ��������ܷ�Ƶ
//1BYTE HEX �������й��������ܷ�Ƶ
//1BYTE HEX �෴���й��������ܷ�Ƶ
//1BYTE HEX �������й�г�����ܷ�Ƶ
//1BYTE HEX �෴���й�г�����ܷ�Ƶ
//1BYTE HEX ��ͭ���й����ܲ�������Ƶ
//1BYTE HEX �������й����ܲ�������Ƶ
//4BYTE CRC32
#define ADDR_00900100     ADDR_003D0000+85+17+4
//4BYTE ʣ�����NNNNNN.NN kWh(BCD,ʵʱ��
//4BYTE ͸֧����NNNNNN.NN kWh(BCD,ʵʱ��
//4BYTE δ�ۼ�����NNNNNN.NN kWh(HEX)
//4BYTE ԭ���ڿۼ���������й��ܵ���ֵ(HEX)
//4BYTE (��û)ESAM�ۼ���Ӧʣ�����NNNNNN.NN kWh(HEX)
//4BYTE CRC32
#define ADDR_00900200     ADDR_00900100+16+4
//4BYTE ʣ����NNNNNN.NNԪ(BCD,ʵʱ��
//4BYTE ͸֧���NNNNNN.NNԪ(BCD,ʵʱ��
//4BYTE δ�ۼ����NNNNNN.NNԪ(HEX)
//4BYTE δ�ۼ����С��λ0.00NNNNԪ(HEX)
//4BYTE �����������ۼ��õ���(HEX)
//4*(1+MMaxTariff)BYTE ԭ���ڼ������,����1-����M����ʾֵ(HEX)
//4BYTE �Ͻ�����0�����������ۼ��õ�����������ʱ(BCD)
//4BYTE (��û)ESAM�ۼ���Ӧʣ����NNNNNN.NNԪ(HEX)
//4BYTE CRC32
#define ADDR_ESAM_NOTE     ADDR_00900200+20+(4*(1+MMaxTariff))+4+4//ESAM�м�¼��ʣ��������
//4BYTE ESAM�м�¼��ʣ����(�����)HHHHHH.HH(HEX)
#define ADDR_000B0000     ADDR_ESAM_NOTE+4//������������й����ۼ��õ���
//5BYTE BCD �ϴ��Ѳ��ڼ��������й��ܵ���
//5BYTE BCD ��ǰ������������й����ۼ��õ���

#define ADDR_MeterCurveTime     ADDR_000B0000+10//�Ǳ�07���ߴ�����¼,�����0ʱ��Ϊֵ0
//5BYTE ��ʱ������,���߼�¼��ʼʱ��
//5BYTE ��ʱ������,��������߼�¼ʱ��(�ȽϿ�ʼʱ�䰴�ֻ���ɴ洢ƫ�Ƶ���)
//4BYTE ��
#define ADDR_CONGEAL_COUNT     ADDR_MeterCurveTime+14//�������
//1BYTE ��ʱ����(��60��)����HEX
//5BYTE ����
//1BYTE �ն���(��62��)����HEX
//5BYTE �ϴ��ն���ķ�ʱ������
//1BYTE ���㶳��(��254��)����HEX
//5BYTE �ϴ����㶳��ķ�ʱ������

#define ADDR_02800024     ADDR_CONGEAL_COUNT+18//�����ܵ�ѹ�ϸ�ʱ��
//3BYTE_BCD ��
#define ADDR_CLRYMD     ADDR_02800024+3//�ϴ�ÿ����0ʱ��������
//3BYTE_BCD ������
#define ADDR_BroadcasTimeFlag     ADDR_CLRYMD+3//���ܱ��չ㲥Уʱ��־
//1BYTE �㲥Уʱ��־,0=û�㲥Уʱ��1=�ѹ㲥Уʱ
#define ADDR_MESAMMACErrorCount     ADDR_BroadcasTimeFlag+1//MESAM��MAC������У��������,��ÿ�յ������0
//1BYTE_HEX MACУ������Ľ��ܴ������,ÿ���յ�������֡MACУ��ʧ�ܡ�����У��ʧ�����ۼƴﵽ200�Σ���Ҫ����ܱ�Զ�����Z������Զ�̿��ơ����㹦�ܹ�����ÿ�յ���㣬�������״̬���ۼƴ���
#define ADDR_ClrAutoReport_S_Time     ADDR_MESAMMACErrorCount+1//��0�����ϱ�״̬���붨ʱ��(ͣ��ʱ�䲻��)
//2byte �붨ʱ��




#define ADDR_Meter_SaveRam_CRC     ADDR_ClrAutoReport_S_Time+2
//4byte 0x12345678
//4byte CRC
#define ADDR_Meter_SaveRam_End     (ADDR_Meter_SaveRam_CRC+4)
#define ADDR_MeterEnergy_IRAM_End     ADDR_Meter_SaveRam_End//�Ǳ������������IRAM������ַ
//�������൱ǰ���ݽ���--------------------------------------------------------------------------



#endif
