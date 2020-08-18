
#ifndef IRAM_Meter_H
#define IRAM_Meter_H

typedef __packed struct
{
#define ADDR_I1Task      ADDR_Meter_Ram_Start
  u8 I1Task;//I1��ʱ����
  u8 I1SubTask1;//I1�ӷ�ʱ����1,���ڼ�������:��ѹ,����,����,...��
  u8 res1;
	u8 I1MonthTask;//������(��)��������
#define ADDR_SSP1     (ADDR_I1Task+4)//SSP1,����IDT90E36��д
  u8 EMUParaCount;//����оƬ������ʼ������
  u8 EMUInit;//����оƬ��ʼ����־:0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=������ʼ��,5=���
  u8 SSP1ErrorCount;//SSP1ͨ�Ŵ������
  u8 SSP1Sched;//SSP1�������:0=����0,1=����1
  u8 SSP1Task0;//SSP1����0:0=��,0x1=�ȴ���,0x2=����ʼ,0x3=���Ƚ�,0x4=�����,0x5=������;0x81=�ȴ�д,0x82=д��ʼ,0x83=д�Ƚ�,0x84=д���,0x85=д����
  u8 SSP1Task1;//SSP1����1:0=��,0x1=�ȴ���,0x2=����ʼ,0x3=���Ƚ�,0x4=�����,0x5=������;0x81=�ȴ�д,0x82=д��ʼ,0x83=д�Ƚ�,0x84=д���,0x85=д����
	u8 SSP1Task1Count;//SSP1����1�����������������
	u8 TxCount;//���ͼ���
	u16 SPI2TxData;//�ڽ����ж���Ҫ�������͵�����
	u16 res2;
#define ADDR_SSP1Data     ADDR_SSP1+12
  u16 SSP1Addr0;//SSP1��д���ݵ�ַ0
  u16 SSP1Addr1;//SSP1��д���ݵ�ַ1	
  u32 SSP1Data0;//SSP1��д����0
  u32 SSP1Data1;//SSP1��д����1
  u32 SSP1Data2;//SSP1��д����2
#define ADDR_Measure     ADDR_SSP1Data+16//����
	u8 PW;//���߷�ʽ;0=��������,1=��������
	u8 NOPLoad;//λΪ1��ʾ����;b2=�й�C,b1=�й�B,b0=�й�A;(��ַ�̶�ΪADDR_Measure+1,ͨ�ſ���Ҫ��)
	u8 NOQLoad;//λΪ1��ʾ����;b2=�޹�C,b1=�޹�B,b0=�޹�A
	u8 NOSLoad;//λΪ1��ʾ����;b2=����C,b1=����B,b0=����A
	u8 TQ;//�ܵ�ǰ���� 1-4�ֱ��ʾ1-4����,0=�����ȷ��
	u8 AQ;//A�൱ǰ���� 1-4�ֱ��ʾ1-4����,0=�����ȷ��
	u8 BQ;//B�൱ǰ���� 1-4�ֱ��ʾ1-4����,0=�����ȷ��
	u8 CQ;//C�൱ǰ���� 1-4�ֱ��ʾ1-4����,0=�����ȷ��
	u8 DQ;//B0-B3�޹�����������ԭ��ǰ����1-4�ֱ��ʾ1-4����,0=�����ȷ��(��TQ����),B7=�й���������ԭ����0��1��
	u8 UARTFlags;//b7=���ü�,b6=��ȫ��֤,b5=����,b4=�ز�ģ��STA��ԭ״̬�Ĵ�,b3=�ز�ͨ��,b2=RS485-1ͨ��,b1=RS485-2ͨ��,b0=����ͨ��
#define ADDR_METER07_CTRL_SRAM     (ADDR_Measure+10)
	u8 Tariff;//1BYTE ���ܱ�ǰִ�з���(HEX ����1,2,3,4 �ֱ��Ӧ���,��,ƽ,�ȵȷ���)
	u8 Ctrl1;//1BYTE ����ʱ�����л�״̬0=1��,1=2��(ͬ��FLASH��ֵ)
	u8 Ctrl2;//1BYTE ������ʱ�α��л�״̬0=1��,1=2��(ͬ��FLASH��ֵ)
	u8 Ctrl3;//1BYTE �����ݶ��л�״̬0=1��,1=2��(ͬ��FLASH��ֵ)
	u8 Ctrl4;//1BYTE ���׷��ʵ���л�0=1��,1=2��(ͬ��FLASH��ֵ)
	u8 Ctrl5;//1BYTE ��ǰ���е�n���ݵ��(HEX)
	u8 Ctrl6;//1BYTE 1=�̵������Կ�����
	u8 Ctrl7;//1BYTE �̵���ԭ����բ״̬,0=��բ,1=��բ
	u8 Ctrl8;//1BYTE �̵�����������������
	u8 Ctrl9;//1BYTE �������������ֹ,0=����,����ֵ��ֹ
#define ADDR_Meter_MS_Timer     ADDR_METER07_CTRL_SRAM+10//�Ǳ�10MS��ʱ��
	u16 DisplayScanTimer;//1 ��ʾɨ�� ÿ10MS��1��ʱ��
	u16 PTrip_MS_Timer;//2 ���ɿ�����բ����10MS��ʱ��
	u16 NTrip_MS_Timer;//3 ���ɿ��غ�բ����10MS��ʱ��
	u16 MT_MS_Timer;//4 ���ö����������10MS��ʱ
	u16 LVTest_MS_Timer;//5 ȫʧѹ����90E36����10MS��ʱ��
	u16 Read90E36Timer;//6 ѭ����90E36��������ÿ10MS��1��ʱ��
#define Meter_MS_Timer_Num     6//�Ǳ�10MS��ʱ����
  
#define ADDR_Meter_S_Timer     (ADDR_Meter_MS_Timer+(2*Meter_MS_Timer_Num))//S����ʱ��
	u16 S_PR;//1 60���Ƶ������
	u16 DemandCYC_S_Timer;//2 ���������붨ʱ��
	u16 SlipCYC_S_Timer;//3 ���������붨ʱ��
	u16 TariffCYC_S_Timer;//4 �������ڷ����л��붨ʱ��
	u16 PowerCYC_S_Timer;//5 ���������й�����ı��붨ʱ��
	u16 MeanPower_S_Timer;//6 һ�����й���ƽ�������붨ʱ��
	u16 ALP_S_Timer;//7 A�����붨ʱ��
	u16 BLP_S_Timer;//8 B�����붨ʱ��
	u16 CLP_S_Timer;//9 C�����붨ʱ��
	u16 ALLLV_S_Timer;//10 ȫʧѹ�͵����붨ʱ��
	u16 ALV_S_Timer;//11 Aʧѹ�붨ʱ��
	u16 BLV_S_Timer;//12 Bʧѹ�붨ʱ��
	u16 CLV_S_Timer;//13 Cʧѹ�붨ʱ��
	u16 ALC_S_Timer;//14 Aʧ���붨ʱ��
	u16 BLC_S_Timer;//15 Bʧ���붨ʱ��
	u16 CLC_S_Timer;//16 Cʧ���붨ʱ��
	u16 VIP_S_Timer;//17 ��ѹ�������붨ʱ��
	u16 IIP_S_Timer;//18 �����������붨ʱ��
	u16 ALO_S_Timer;//19 A�����붨ʱ��
	u16 BLO_S_Timer;//20 B�����붨ʱ��
	u16 CLO_S_Timer;//21 C�����붨ʱ��
	u16 UNI_S_Timer;//22 ������ƽ���붨ʱ��
	u16 UNN_S_Timer;//23 �������ز�ƽ���붨ʱ��
	u16 AVO_S_Timer;//24 A��ѹ�붨ʱ��
	u16 BVO_S_Timer;//25 B��ѹ�붨ʱ��
	u16 CVO_S_Timer;//26 C��ѹ�붨ʱ��
	u16 TFO_S_Timer;//27 �ܹ������������붨ʱ��
	u16 PDO_S_Timer;//28 ���й����������붨ʱ��
	u16 NDO_S_Timer;//29 ���й����������붨ʱ��
	u16 ReTrip_S_Timer;//30 ���ɿ��ز����붨ʱ��
	u16 ESAMPAY_S_Timer;//31 ESAMǮ�������붨ʱ��
	u16 ExitTrip_S_Timer;//32 �ⲿ���ɿ���Ԥ��բ����ʱ���붨ʱ��
	u16 TripAttr_S_Timer;//33 ��բǰ�澯��ʱ�붨ʱ��
  u16 DisplayLoopTimer;//34 ѭ���붨ʱ��
  u16 BackLightTimer;//35 �������ʾ�붨ʱ��
	u16 UNU_S_Timer;//36 ��ѹ��ƽ���붨ʱ��
	
#define Meter_S_Timer_Num     36//�����ʱ����
#define ADDR_Meter_SADD_Timer     ADDR_Meter_S_Timer+(2*Meter_S_Timer_Num)//S��Ӷ�ʱ��
	u16 AH_S_Timer;//��ʱ��������Ӽ���
	u16 GP_SAdd_Timer;//���жϳ�������Ӽ���
	
#define Meter_SADD_Timer_Num     2//��Ӷ�ʱ����
	
	
#define ADDR_Meter_T_Flags     ADDR_Meter_SADD_Timer+(2*Meter_SADD_Timer_Num)
	u8 TFlags;//���ʱ����±�־
			//B0=����±�־,����RTCʵʱ���ʿ���(ֵ0��ʾ����,ʹ�ú���1)
			//B1=�ָ��±�־,����FM�ֶ�ʱ��(ֵ1��ʾ����,ʹ�ú���0)(��ǰ���ô˱�־��ʱ)
			//B2=LCD��������(�����ȡ��)
			//...
			//B7=
	u8 res3;
	u16 res4;
#define ADDR_EventFlags     ADDR_Meter_T_Flags+4//�¼���־
	u8 LPFlags;//b0-b2�ֱ�ΪABC����˲ʱ,b3=,b4-b6�ֱ�ΪABC����ȷ��,b7=
	u8 LPFlagsOld;//
	u8 LVFlags;//b0-b2�ֱ�ΪABCʧѹ˲ʱ,b3=,b4-b6�ֱ�ΪABCʧѹȷ��,b7=
	u8 LVFlagsOld;//
	u8 LIFlags;//b0-b2�ֱ�ΪABCʧ��˲ʱ,b3=,b4-b6�ֱ�ΪABCʧ��ȷ��,b7=
	u8 LIFlagsOld;//
	u8 AllLVFlags;//b0=ȫʧѹ˲ʱ,b1=����˲ʱ;b3=ȫʧѹȷ��,b4=����ȷ��
	u8 AllLVFlagsOld;//
	u8 IPFlags;//b7=��ѹ������,b6=����������,b5=��ѹ������ȷ��,b4=����������ȷ��,b3=��ѹ������˲ʱ,b2=����������˲ʱ,b1=, b0=
	u8 IPFlagsOld;//
	u8 LOFlags;//b0-b2�ֱ�ΪABC����˲ʱ,b3=,b4-b6�ֱ�ΪABC����ȷ��,b7=
	u8 LOFlagsOld;//
	u8 UNFlags;//b0=������ƽ��˲ʱb1=�������ز�ƽ��˲ʱb2=��ѹ��ƽ��˲ʱ;b4=������ƽ��ȷ��b5=�������ز�ƽ��ȷ��b6=��ѹ��ƽ��ȷ��
	u8 UNFlagsOld;//
	u8 VOFlags;//b0-b2�ֱ�ΪABC��ѹ˲ʱ,b3=,b4-b6�ֱ�ΪABC��ѹȷ��,b7=
	u8 VOFlagsOld;//
	u8 FOFlags;///b0=�ܹ�����������˲ʱ, b4=�ܹ�����������ȷ��
	u8 FOFlagsOld;//
	u8 DOFlags;//b0=���й���������˲ʱ,b1=���й���������˲ʱ, b4=�й���������ȷ��,B5=���й���������ȷ��
	u8 DOFlagsOld;//
	u8 DIRFlags;//b0-b2�ֱ�ΪABC��������˲ʱ,b3=,b4-b6�ֱ�ΪABC��������ȷ��,b7=
	u8 DIRFlagsOld;//
#define ADDR_SEGLCDCtrl     ADDR_EventFlags+22//ѭ�Կ���(�ֶ���)
	u8 LcdLoop;// 0 ѭ�Լ���
	u8 LcdRound;// 1 ѭ���ֻؼ���
	u8 LcdNext;// 2 B0=NEXTI����,B1=NEXTD����,B2=ԭ����,B3-B7����
	u8 reserve1;// 3 
	u8 LcdErr1;// 4 ERR1���������쳣ѭ�Լ���
	u8 LcdErr2;// 5 ERR2�¼����쳣ѭ�Լ���
	u8 LcdErr3;//	6 ERR3 IC���쳣ѭ�Լ���
	u8 ErrFlags;// 7 �����ϱ�־;B7=,B6=ʱ��,B5=�洢��,B4=�ڲ�����,B3=ʱ�ӵ��,B2=�ڿ���ʼ��,B1=ESAM,B0=���ƻ�·,
	u8 reserve2;// 8 
	u8 CardErr1;// 9 IC���쳣��־;B7=δ��Ǧ���,B6=�޸���Կ����,B5=�ֳ��������ÿ��Ա����Ѿ�ʧЧ,B4=���糬�ڻ�,B3=��ֵ��������,B2=�ͻ���Ų�ƥ��,B1=ESAM��֤ʧ��,B0=��֤����
	u8 CardErr2;// 10 IC���쳣��־;B7=���ļ���ʽ���Ϸ�,B6=��Ƶ�ѹ����,B5=���𻵻������Ϳ�,B4=���δ����,B3=����ѿ���,B2=�޸���Կ������Ϊ0,B1=�޸ı�ſ���,B0=��ǰ�ο�
	u8 CardErr3;// 11 IC���쳣��־;B7=����������IC���쳣��B6-B3=����,B2=��������,B1=�Ѿ����������¿�����,B0=�����ʹ�
	u8 ButtonVal;// 12 ����״̬(ʹ�ú���0);
	u8 ButtonKeep;// 13 ���±���3��״̬(ʹ�ú���0)
	u8 LcdCard;// 14 B7-B0='��''����''��''�ɹ�''ʧ��'�빺��''͸֧''��բ' 
	u8 LcdCardFlash;// 15 ����B7-B0='��''����''��''�ɹ�''ʧ��'�빺��''͸֧''��բ' 
#define ADDR_04000501     ADDR_SEGLCDCtrl+16//�������״̬��,Ҫ���ֶ���
	u16 RunFlag1;//2BYTE ״̬��1	B7=����,B6=����,B5=�޹�����,B4=�й�����,B3=������,B2=ʱ�ӵ��,B1=�������㷽ʽ,B0=����
			// B15-B8=����
	u16 RunFlag2;//2BYTE ״̬��2 B7=����,B6=C�޹�����,B5=B�޹�����,B4=A�޹�����,B3=����,B2=C�й�����,B1=B�й�����,B0=A�й�����
			// B15-B8=����
	u16 RunFlag3;//2BYTE ״̬��3 B7=Ԥ��բ����,B6=�̵�������,B5=��ǰ����ʱ��,B4=�̵���״̬,B3=�������,B2B1=���緽ʽ,B0=��ǰ����ʱ����
			// B15-B12=����,B11=��ǰ������,B10=��ǰ���з��ʵ����,B9B8=���ܱ�����
	u16 RunFlag4;//2BYTE ״̬��4(A�����״̬) B7=����,B6=��������,B5=����,B4=����,B3=ʧ��,B2=��ѹ,B1=Ƿѹ,B0=ʧѹ
			//(A�����״̬)B15-B9=����,B8=����
	u16 RunFlag5;//2BYTE ״̬��5(B�����״̬) B7=����,B6=��������,B5=����,B4=����,B3=ʧ��,B2=��ѹ,B1=Ƿѹ,B0=ʧѹ
			//(B�����״̬)B15-B9=����,B8=����
	u16 RunFlag6;//2BYTE ״̬��6(C�����״̬) B7=����,B6=��������,B5=����,B4=����,B3=ʧ��,B2=��ѹ,B1=Ƿѹ,B0=ʧѹ
			//(C�����״̬)B15-B9=����,B8=����
	u16 RunFlag7;//2BYTE ״̬��7(�������״̬) B7=�ܹ�������������,B6=��������,B5=����,B4=����,B3=ʧ��,B2=��ѹ,B1=Ƿѹ,B0=ʧѹ
			//(�������״̬)B15-B9=����,B8=�������ز�ƽ��
	
	#if MeterMaxDemandCalculate==0//����������㣺0=���巨,1=����ƽ����
	//���巨�����������
#define ADDR_PARKW_IMP     (ADDR_04000501+14)//һ�����й���ƽ������������
	u16 MeanImp;
	u8 DemandCYC;//��ǰ��������������
	u8 SlipCYC;//��ǰ�����û���ʱ��
#define ADDR_MAXKW_IMP     (ADDR_PARKW_IMP+4)//�й���������
	u16 ActiveImpShift[60];//120BYTE
	u16 ActiveImp;//2BYTE 0.01CF����
#define ADDR_MAXKVAR_IMP     (ADDR_MAXKW_IMP+120+2)//�޹���������
	u16 ReactiveImpShift[60];//120BYTE
	u16 ReactiveImp;//2BYTE 0.01CF����
#define ADDR_FactoryCal     ADDR_MAXKVAR_IMP+122
	#else
	//����ƽ���������������
#define ADDR_PARKW_IMP     (ADDR_04000501+14)//һ�����й���ƽ������������
	u8 DemandCYC;//��ǰ��������������
	u8 SlipCYC;//��ǰ�����û���ʱ��
	u8 kWParCount;//hexƽ����������
	u8 res5;
	u32 kWParAdd;//4byte hex�й�����ƽ����
#define ADDR_MAXKW_PAR     (ADDR_PARKW_IMP+8)//�й�����ƽ��
	u8 ActiveParShift[3*60];//180BYTE hex
	u32 ActiveParAdd;//4byte hexƽ����
	u16 ActiveParCount;//2BYTE hexƽ����������
#define ADDR_MAXKVAR_PAR     (ADDR_MAXKW_PAR+180+6)//�޹�����ƽ��
	u8 ReactiveParShift[3*60];//180BYTE hex
	u16 ReactiveParCount;//2BYTE hexƽ����������
	u32 ReactiveParAdd;//4byte hexƽ����
#define ADDR_FactoryCal     ADDR_MAXKVAR_PAR+180+6
	#endif
	u32 CS;//У������У���


}Meter_Ram_TypeDef;
#define Meter_Ram     ((Meter_Ram_TypeDef *)(ADDR_Meter_Ram_Start))

#define ADDR_07LIB_REG     (((ADDR_FactoryCal+4)+3)&0xfffffffc)//�Ǳ�07��������ݼĴ�(�ֶ���)
//4BYTE 0 ͨ�ű�ʶ
//1BYTE 1.0 ��ʾ����չ��ʶ
//1BYTE 1.1 B7-B4����,B3=Һ���鿴��־,B2=1��ʾ����ˢ�±�־,B1=����˵���־,B0=1��ʾDI���±�־
//1BYTE 1.2 B7-B1����,B0=1�ޱ�ʶ����(��û���õĶ�����)
//1BYTE 1.3
//4BYTE 2 �ڲ����ݱ��
//4BYTE 3 ͨ�ű�ʶ��ָ��(�ṹ�����)
//4BYTE 4 ���ݿ�ʼ��ַ
//4BYTE 5 ����ָ���ָ��(�ṹ�����)
//4BYTE 6 LCD��ָ��(�ṹ�����)
//1BYTE 7.0 ���º�
//1BYTE 7.1 ���ʺ�
//1BYTE 7.2 ������ʾС��λ��
//1BYTE 7.3 ������ʾС��λ��
#define ADDR_LCDBUFF1     (ADDR_07LIB_REG+32)//��ʽLCD��ʾ������1(�ֶ���)
//8BYTE 0 �ַ�����N0-N7(С�˶���)
//1BYTE 8 B7-B1=����С����P7-P1,B0=��
//2BYTE 9 �ַ�����n0-n1(С�˶���)
//1BYTE 11 B7-B0='��ǰ''��''��''���''��''��''��''��'
//1BYTE 12 B7-B0='��''��''��''��''��''��''��''��'
//1BYTE 13 B7-B0='��''ƽ''��''��''��''��''��''COS'
//1BYTE 14 B7-B0='��''����''ʣ��'��''��''��''��''��'
//1BYTE 15 B7-B0='��''ʧ''ѹ''��''����''ʱ''��''��'
//1BYTE 16 B7-B0='��1''��2''��''Ԫ''��''V''V''��'
//1BYTE 17 B7-B0='��''��''var''��''�ڻ�''ʱ��''�ֵ�''������'

//1BYTE 18 ����B7-B0=��7-��0
//1BYTE 19 ����B7-B1=С����P7-P1,B0=����
//1BYTE 20 ����B7-B2=����,B1-B0=����1-����0
//1BYTE 21 ����B7-B0='��ǰ''��''��''���''��''��''��''��'
//1BYTE 22 ����B-B0='��''��''��''��''��''��''��''��'
//1BYTE 23 ����B7-B0='��''ƽ''��''��''��''��''��''COS'
//1BYTE 24 ����B7-B0='��''����''ʣ��'��''��''��''��''��'
//1BYTE 25 ����B7-B0='��''ʧ''ѹ''��''����''ʱ''��''��'
//1BYTE 26 ����B7-B0='��1''��2''��''Ԫ''��''V''V''��'
//1BYTE 27 ����B7-B0='��''��''var''��''�ڻ�''ʱ��''�ֵ�''������'
//4BYTE ����
#define ADDR_LCDBUFF2     (ADDR_LCDBUFF1+32)//��ʽLCD��ʾ������2(�ֶ���)
//8BYTE 0 �ַ�����N0-N7(С�˶���)
//1BYTE 8 B7-B1=����С����P7-P1,B0=��
//2BYTE 9 �ַ�����n0-n1(С�˶���)
//1BYTE 11 B7-B0='���''���''���''���''��1''��2''��3''��4'
//1BYTE 12 B7-B0='���''���''��ƽ''���''Ua''Ub''Uc''������'
//1BYTE 13 B7-B0='-''Ia''-''Ib''-''Ic''��1''��2'
//1BYTE 14 B7-B0='���X1''���X2''����''�ź�1''�ź�2''�ź�3''�ź�4''�ز�'
//1BYTE 15 B7-B0='����''��Ͳ''ͨ��1''ͨ��2''����''����''��''��'
//1BYTE 16 B7-B0='��''����''��''�ɹ�''ʧ��'�빺��''͸֧''��բ'
//1BYTE 17 ����

//1BYTE 18 ����B7-B0=��7-��0
//1BYTE 19 ����B7-B1=С����P7-P1,B0=����
//1BYTE 20 ����B7-B2=����,B1-B0=����1-����0
//1BYTE 21 ����B7-B0='��''��''��''��''��1''��2''��3''��4'
//1BYTE 22 ����B7-B0='���''���''��ƽ''���''Ua''Ub''Uc''������'
//1BYTE 23 ����B7-B0='-''Ia''-''Ib''-''Ic''��1''��2'
//1BYTE 24 ����B7-B0='���X1''���X2''����''�ź�1''�ź�2''�ź�3''�ź�4''�ز�'
//1BYTE 25 ����B7-B0='����''��Ͳ''ͨ��1''ͨ��2''����''����''��''��'
//1BYTE 26 ����B7-B0='��''����''��''�ɹ�''ʧ��'�빺��''͸֧''��բ'
//1BYTE 27 ����
//1BYTE 28 ����
//3BYTE ����

#define ADDR_MT     (ADDR_LCDBUFF2+32)
//1BYTE ���ö���;0=ʱ��������,1=��������,2=ʱ��Ͷ��
#define ADDR_04001301     ADDR_MT+1
//1BYTE ����ͨ�����߼��ź�ǿ��ָʾ
#define ADDR_FREQUENCY     ADDR_04001301+1//����Ƶ��
//2BYTE_BCD 2λС��
#define ADDR_PARPT     ADDR_FREQUENCY+2//һ�����й���ƽ������
//3BYTE_BCD 4λС��
#define ADDR_PMAX     ADDR_PARPT+3//��ǰ�й�����
//3BYTE_BCD 4λС��
#define ADDR_QMAX     ADDR_PMAX+3//��ǰ�޹�����
//3BYTE_BCD 4λС��
#define ADDR_SMAX     ADDR_QMAX+3//��ǰ��������
//3BYTE_BCD 4λС��
#define ADDR_IBPH     ADDR_SMAX+3//������ƽ����
//3BYTE_BCD 2λС��
#define ADDR_UNPH     ADDR_IBPH+3//��ѹ��ƽ����
//3BYTE_BCD 2λС��
#define ADDR_0280000B     ADDR_UNPH+3//��ǰ���ݵ��
//4BYTE BCD 4λС��(Ԫ/kWh)
#define ADDR_UA     ADDR_0280000B+4//A��ѹ
//4BYTE_BCD 5λС��
#define ADDR_UB     ADDR_UA+4//B��ѹ
//4BYTE_BCD 5λС��
#define ADDR_UC     ADDR_UB+4//C��ѹ
//4BYTE_BCD 5λС��
#define ADDR_IA     ADDR_UC+4//A����
//4BYTE_SBCD 5λС��
#define ADDR_IB     ADDR_IA+4//B����
//4BYTE_SBCD 5λС��
#define ADDR_IC     ADDR_IB+4//C����
//4BYTE_SBCD 5λС��
#define ADDR_IN     ADDR_IC+4//���ߵ���
//4BYTE_SBCD 5λС��
#define ADDR_PT     ADDR_IN+4//���й�����
//4BYTE_SBCD 6λС��
#define ADDR_PA     ADDR_PT+4//A�й�����
//4BYTE_SBCD 6λС��
#define ADDR_PB     ADDR_PA+4//B�й�����
//4BYTE_SBCD 6λС��
#define ADDR_PC     ADDR_PB+4//C�й�����
//4BYTE_SBCD 6λС��
#define ADDR_QT     ADDR_PC+4//���޹�����
//4BYTE_SBCD 6λС��
#define ADDR_QA     ADDR_QT+4//A�޹�����
//4BYTE_SBCD 6λС��
#define ADDR_QB     ADDR_QA+4//B�޹�����
//4BYTE_SBCD 6λС��
#define ADDR_QC     ADDR_QB+4//C�޹�����
//4BYTE_SBCD 6λС��
#define ADDR_ST     ADDR_QC+4//�����ڹ���
//4BYTE_SBCD 6λС��
#define ADDR_SA     ADDR_ST+4//A���ڹ���
//4BYTE_SBCD 6λС��
#define ADDR_SB     ADDR_SA+4//B���ڹ���
//4BYTE_SBCD 6λС��
#define ADDR_SC     ADDR_SB+4//C���ڹ���
//4BYTE_SBCD 6λС��
#define ADDR_COST     ADDR_SC+4//�ܹ�������
//2BYTE_SBCD 3λС��
#define ADDR_COSA     ADDR_COST+2//A��������
//2BYTE_SBCD 3λС��
#define ADDR_COSB     ADDR_COSA+2//B��������
//2BYTE_SBCD 3λС��
#define ADDR_COSC     ADDR_COSB+2//C��������
//2BYTE_SBCD 3λС��
#define ADDR_ANGLE_UIA     ADDR_COSC+2//A��ѹ�����н�
//2BYTE_BCD 1λС��
#define ADDR_ANGLE_UIB     ADDR_ANGLE_UIA+2//B��ѹ�����н�
//2BYTE_BCD 1λС��
#define ADDR_ANGLE_UIC     ADDR_ANGLE_UIB+2//C��ѹ�����н�
//2BYTE_BCD 1λС��
#define ADDR_ANGLE_UA     ADDR_ANGLE_UIC+2//UA�Ƕ�(����0)
//2BYTE_BCD 1λС��
#define ADDR_ANGLE_UB     ADDR_ANGLE_UA+2//UB�Ƕ�(��UA)
//2BYTE_BCD 1λС��
#define ADDR_ANGLE_UC     ADDR_ANGLE_UB+2//UC�Ƕ�(��UA)
//2BYTE_BCD 1λС��
#define ADDR_ANGLE_IA     ADDR_ANGLE_UC+2//IA�Ƕ�(��UA)
//2BYTE_BCD 1λС��
#define ADDR_ANGLE_IB     ADDR_ANGLE_IA+2//IB�Ƕ�(��UA)
//2BYTE_BCD 1λС��
#define ADDR_ANGLE_IC     ADDR_ANGLE_IB+2//IC�Ƕ�(��UA)
//2BYTE_BCD 1λС��

#define ADDR_UAA     ADDR_ANGLE_IC+2//A��ѹ�ܻ���
//2BYTE 00.00%
#define ADDR_UBA     ADDR_UAA+2//B��ѹ�ܻ���
//2BYTE 00.00%
#define ADDR_UCA     ADDR_UBA+2//C��ѹ�ܻ���
//2BYTE 00.00%
#define ADDR_IAA     ADDR_UCA+2//A�����ܻ���
//2BYTE 00.00%
#define ADDR_IBA     ADDR_IAA+2//B�����ܻ���
//2BYTE 00.00%
#define ADDR_ICA     ADDR_IBA+2//C�����ܻ���
//2BYTE 00.00%
#define ADDR_FFT_UA     ADDR_ICA+2//UAг������
//2BYTE 0��
//2BYTE 1��
//...
//2BYTE 21��
#define ADDR_FFT_UB     ADDR_FFT_UA+(2*22)//UBг������
//2BYTE 0��
//2BYTE 1��
//...
//2BYTE 21��
#define ADDR_FFT_UC     ADDR_FFT_UB+(2*22)//UCг������
//2BYTE 0��
//2BYTE 1��
//...
//2BYTE 21��
#define ADDR_FFT_IA     ADDR_FFT_UC+(2*22)//IAг������
//2BYTE 0��
//2BYTE 1��
//...
//2BYTE 21��
#define ADDR_FFT_IB     ADDR_FFT_IA+(2*22)//IBг������
//2BYTE 0��
//2BYTE 1��
//...
//2BYTE 21��
#define ADDR_FFT_IC     ADDR_FFT_IB+(2*22)//ICг������
//2BYTE 0��
//2BYTE 1��
//...
//2BYTE 21��
#define ADDR_DFT_SCALE     ADDR_FFT_IC+(2*22)//DFT�ź�����GAIN
//2BYTE 
#define ADDR_UA_FUND     ADDR_DFT_SCALE+2//A��ѹ����
//3BYTE_BCD 3λС��
#define ADDR_UB_FUND     ADDR_UA_FUND+3//B��ѹ����
//3BYTE_BCD 3λС��
#define ADDR_UC_FUND     ADDR_UB_FUND+3//C��ѹ����
//3BYTE_BCD 3λС��
#define ADDR_IA_FUND     ADDR_UC_FUND+3//A��������
//3BYTE_BCD 3λС��(�޷���)
#define ADDR_IB_FUND     ADDR_IA_FUND+3//B��������
//3BYTE_BCD 3λС��(�޷���)
#define ADDR_IC_FUND     ADDR_IB_FUND+3//C��������
//3BYTE_BCD 3λС��(�޷���)

#define ADDR_02800020     ADDR_IC_FUND+3//��ǰ���
//4BYTE_BCD XXXX.XXXXԪ/kWh
#define ADDR_02800021     ADDR_02800020+4//��ǰ���ʵ��
//4BYTE_BCD XXXX.XXXXԪ/kWh
#define ADDR_02800022     ADDR_02800021+4//�����֤ʱЧʣ��ʱ��
//1BYTE_HEX���Ƶ
//2BYTE_BCD ��
#define ADDR_02800023     ADDR_02800022+3//������֤ʱЧʣ��ʱ��
//1BYTE_HEX���Ƶ
//1BYTE_BCD ��
#define ADDR_04001502     ADDR_02800023+2//�忨״̬��(��ȡ���ϵ����0)
//2BYTE   
#define ADDR_04040300     ADDR_04001502+2//Һ���鿴
//5BYTE NNNNNNNN,NN(FFFFFFFF,FFʱ,��ʾȫ����ʾ)
#define ADDR_LUAB     ADDR_04040300+5//�ߵ�ѹUAB
//2BYTE_BCD 1λС��
#define ADDR_LUBC     ADDR_LUAB+2//�ߵ�ѹUBC
//2BYTE_BCD 1λС��
#define ADDR_LUCA     ADDR_LUBC+2//�ߵ�ѹUCA
//2BYTE_BCD 1λС��


#define ADDR_Meter_Ram_End     (ADDR_LUCA+2)


#endif


