

//�ն�3������
#ifndef AFN0E_H
#define AFN0E_H




#define LEN_TERMINAL_EVENT     128//�ն��¼���¼�̶�����
#define ADDR_AFN0EF1     ADDR_AFN0E_Start//��Ҫ�¼���¼
//ÿ�¼��̶�ռ��LEN_TERMINAL_EVENT�ֽ�
//��¼��ʽ:
//�¼�����ERC	BIN	1
//�¼���¼����Le	BIN	1
//�¼���¼����	
#define ADDR_AFN0EF2     ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*256)//һ���¼���¼
//ÿ�¼��̶�ռ��LEN_TERMINAL_EVENT�ֽ�
//��¼��ʽ:
//�¼�����ERC	BIN	1
//�¼���¼����Le	BIN	1
//�¼���¼����

#define ADDR_AFN0EF3     (ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*256))
//ERC1
//2BYTE �����¼�����
//15*LEN_TERMINAL_EVENT�ֽ� 15���¼���¼
//...
//ERC68 15��
//2BYTE �����¼�����
//15*LEN_TERMINAL_EVENT�ֽ� 15���¼���¼
	

#define ADDR_AutoReportWord     (ADDR_AFN0EF3+(MaxERC*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT))))//���ܱ������ϱ�״̬��
//AFN04F10�������1
//96byte �����ϱ��¼���������hex(ƫ�ƶ�Ӧ״̬�ֵ�λ��,��ʼ��ֵ0)
//1BYTE ��96BYTE�����ݻ�(��ʼ��ֵ0)
//12byte �����ϱ��¼���λ��־�Ĵ�,���ڷ���λ����(��ʼ��ֵ0)
//...
//AFN04F10�������n
#define LEN_AutoReportWord     (96+1+12)
#define ADDR_MeterEventCount     ADDR_AutoReportWord+(LEN_AutoReportWord*MaxRS485AddCarrierPn)//�Ĵ���ܱ���¼�ԭ��������
//������1
//3byte ��1�����ܱ��¼����ݱ�ʶԭ��������BCD(��ʼ��ֵ0xff)
//1byte ��������hex(��ʼ��ֵ0xff)
//...
//3byte ��n�����ܱ��¼����ݱ�ʶԭ��������BCD(��ʼ��ֵ0xff)
//1byte ��������hex(��ʼ��ֵ0xff)

//3byte 0x03060000//39 B68������Դʧ��������3byte���ۼ�ʱ��
//3byte 0x03050000//45 B74ȫʧѹ�������3byte���ۼ�ʱ��

//...
//������n
#define NUM_MeterEventDI     (60+6)//�����ı�ʶ����
#define LEN_MeterEventCount     ((4*NUM_MeterEventDI)+6)//ÿ�����㳤��

#define ADDR_MeterEvent          ((ADDR_MeterEventCount+(LEN_MeterEventCount*MaxRS485AddCarrierPn)+0xfff)&0xfffff000)//��ȡ���Ǳ��¼�
//���ܱ��¼�1
//1byte �˿ں�(1,2,3,4��31)
//6byte ���ܱ�ͨ�ŵ�ַ(BCD)

//1byte ����(ֵ4+3��4+6��4+18,����ֵ��Ч;ר���ڷ��������洢)//�Ϻ�:����(ֵ4+3��4+6��4+18��4+3+4+3����ֵ��Ч,�������������ۼ�ʱ��,4+3+4+3�е��������ۼ�ʱ�����ݱ�ʶ������)
//2byte �ܳ���(1-10���¼����ݴ洢)

//22byte ���ݱ�ʶ������(FLASH�ռ�̶�����22byte;ר���ڷ��������洢)//�Ϻ�:�������������ۼ�ʱ��
//256byte �����ݱ�ʶ����,���ݳ������ݱ�ʶ������,...(256��1030)//�Ϻ�:�������ݱ�ʶ����,���ݳ��ȵ�Ԫ
//...
//���ܱ��¼�256
#define LEN_MeterEvent     (10+22+256)//(10+22+1030)//���ܴ���LEN_UARTnTx���������ϱ��᲻���ϱ���ѭ��

#define ADDR_ACSampleCurve     ((ADDR_MeterEvent+(LEN_MeterEvent*256)+0xfff)&0xfffff000)//�ڲ�����1���ӵ�ѹ����,������ʱ��������ʱ�ֶ�00��1��1��0ʱ0�ּ���洢λ��(��ʼ��ʱ�Զ�ѡ������������Ϊ0xffֵ)
//5byte ������ʱ��
//2byte A���ѹ����
//2byte B���ѹ����
//2byte C���ѹ����
#define LEN_ACSampleCurve     11//ÿ���ֽ���


#define ADDR_AFN0E_End     (ADDR_ACSampleCurve+(LEN_ACSampleCurve*MaxACSampleCurveCongeal))

#if (ADDR_AFN0E_End-ADDR_AFN0E_Start)>LEN_Terminal_EFLASH_AFN0E
	#error (ADDR_AFN0E_End-ADDR_AFN0E_Start)>LEN_Terminal_EFLASH_AFN0E
#endif

#endif
