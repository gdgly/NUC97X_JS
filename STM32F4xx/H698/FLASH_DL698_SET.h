

#ifndef FLASH_class8_H
#define FLASH_class8_H
#ifdef __cplusplus
 extern "C" {
#endif

	 

//---ͨ����---
#define ADDR_4001     ADDR_DL698_SET_START//ͨ�ŵ�ַ
//����2��=octet-string
#define LENmax_4001     18
#define ADDR_4002     ADDR_4001+LENmax_4001//���
//����2��=octet-string
#define LENmax_4002     18
#define ADDR_4003     ADDR_4002+LENmax_4002//�ͻ����
//����2��=octet-string
#define LENmax_4003     18
#define ADDR_4001_Factory     ADDR_4003+LENmax_4003//ͨ�ŵ�ַ,���,�ͻ���ŵĳ�������
//LENmax_4001+LENmax_4002+LENmax_4003
#define LENmax_4001_Factory     (LENmax_4001+LENmax_4002+LENmax_4003)
	 
#define ADDR_4005     ADDR_4001_Factory+LENmax_4001_Factory//���ַ
//����2��=array octet-string
#define LENmax_4005     (2+(16*18))
#define  ADDR_4300_7     ADDR_4005+LENmax_4005//��������ϱ�
//����7��=bool
#define LENmax_4300_7     2
#define  ADDR_4300_8     ADDR_4300_7+LENmax_4300_7//���������ϱ�
//����8��=bool
#define LENmax_4300_8     2
#define  ADDR_4300_9     ADDR_4300_8+LENmax_4300_8//��������վͨ��
//����9��=bool
#define LENmax_4300_9     2
#define  ADDR_4300_10     ADDR_4300_9+LENmax_4300_9//�ϱ�ͨ��
//����10��=array OAD
#define LENmax_4300_10     32
#define ADDR_4401     ADDR_4300_10+LENmax_4300_10//Ӧ��������֤����
//����2��ֻд����=visible-string
#define LENmax_4401     128
#define ADDR_4500_2     ADDR_4401+LENmax_4401//����ͨ��ģ��1;2ͨ������
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
//��ʱʱ�估�ط�����  unsigned
//��
//bit7~bit2����ʱʱ�䣨�룩��
//bit1~bit0���ط�����
//����
//��������(��)  long-unsigned
//}
#define LENmax_4500_2     128
#define ADDR_4500_3     ADDR_4500_2+LENmax_4500_2//����ͨ��ģ��1;3��վͨ�Ų�����
//��վͨ�Ų������=array ��վͨ�Ų���
//��վͨ�Ų�������=structure
//{
//IP��ַ   octet-string��
//�˿�     long-unsigned��
//}
#define LENmax_4500_3     64
#define ADDR_4500_4     ADDR_4500_3+LENmax_4500_3//����ͨ��ģ��1;4����ͨ�Ų���
//����ͨ�Ų�������=structure
//{
//�������ĺ���     visible-string(SIZE(16))��
//��վ����         array visible-string(SIZE(16))
//����֪ͨĿ�ĺ��� array visible-string(SIZE(16))
//}
#define LENmax_4500_4     64
#define ADDR_4500_11     ADDR_4500_4+LENmax_4500_4//����ͨ��ģ��1;11 ����IP
//octet-string
#define LENmax_4500_11     22
#define ADDR_4510_2     ADDR_4500_11+LENmax_4500_11//��̫��ͨ��ģ��1;2ͨ������
//ͨ�����á�=structure
//{
//����ģʽ  enum{���ģʽ��0�����ͻ���ģʽ��1����������ģʽ��2��}��
//���ӷ�ʽ  enum{TCP��0����UDP��1��}��
//����Ӧ�÷�ʽenum{����ģʽ��0����������ģʽ��1��}
//�����˿��б�array long-unsigned��
//�����������ַ  octet-string��
//����˿�  long-unsigned��
//��ʱʱ�估�ط�����  unsigned
//��
//bit7~bit2����ʱʱ�䣨�룩��
//bit1~bit0���ط�����
//����
//��������(��)  long-unsigned
//}
#define LENmax_4510_2     128
#define ADDR_4510_3     ADDR_4510_2+LENmax_4510_2//��̫��ͨ��ģ��1;3��վͨ�Ų�����
//��վͨ�Ų������=array ��վͨ�Ų���
//��վͨ�Ų�������=structure
//{
//IP��ַ  octet-string��
//�˿�     long-unsigned
//}
#define LENmax_4510_3     64
#define ADDR_4510_4     ADDR_4510_3+LENmax_4510_3//��̫��ͨ��ģ��1;4��������
//�������á�=structure
//{
//IP���÷�ʽenum{DHCP��0������̬��1����PPPoE��2��}��
//IP��ַ       octet-string��
//��������     octet-string��
//���ص�ַ     octet-string��
//PPPoE�û���  octet-string��
//PPPoE����    octet-string
//}
#define LENmax_4510_4     128
#define ADDR_4510_5     ADDR_4510_4+LENmax_4510_4// ��̫��ͨ��ģ��1;5 MAC��ַ
//MAC��ַ��=octet-string 
#define LENmax_4510_5     16
	
#define ADDR_F101_2     ADDR_4510_5+LENmax_4510_5//��ȫģʽ����(��ʼ��ֵ0x00)
//����2����ȫģʽѡ�񣩡�=enum
//{
//�����ð�ȫģʽ������0����
//���ð�ȫģʽ����  ��1��
//}
//��ȫģʽѡ�����壺0�������ð�ȫģʽ������Ĭ�ϰ�ȫģʽ������1�����ð�ȫģʽ������Ĭ�ϰ�ȫģʽ������
#define LENmax_F101_2     2
#define ADDR_F101_3     ADDR_F101_2+LENmax_F101_2//��ʽ��ȫģʽ����(��ʼ��ֵ0x00)
//����3����ʽ��ȫģʽ��������=array ��ȫģʽ����
//��ȫģʽ������=structure
//{
//  �����ʶ   OI��
//  ��ȫģʽ   long-unsigned
//}
//����3Ϊ��ʽ��ȫģʽ����������ֵ�����������ȫ�Բ�������3�У���Ĭ�ϰ�ȫģʽ���������ͬһ����ȫ���ڰ�ȫģʽ������Ĭ�ϰ�ȫģʽ���о���˵��������ʽ��ȫģʽ����ִ�С���ȫģʽ������Ĭ�ϰ�ȫ�������嶨�����¼F����ȫģʽ�������¼F�б�F.1��
#define LENper_F101_3     8//ʵ����
#define NUMmax_F101_3     128
#define LENmax_F101_3     (4+(LENper_F101_3*NUMmax_F101_3))
#if LENmax_F101_3>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_F200_2     ADDR_F101_3+LENmax_F101_3//RS232
//�˿ڡ�=structure
//{
//�˿ں�     OAD
//�˿ڲ���   COMDCB��
//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
//}
#define LENper_F200_2     15
#define LENmax_F200_2     LENper_F200_2
#define ADDR_F201_2     ADDR_F200_2+LENmax_F200_2//RS485
//�˿ڡ�=structure
//{
//�˿ں�     OAD
//�˿ڲ���   COMDCB��
//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
//}
#define LENper_F201_2     15
#define NUMmax_F201_2     3
#define LENmax_F201_2     (LENper_F201_2*NUMmax_F201_2)
#define ADDR_F202_2     ADDR_F201_2+LENmax_F201_2//����
//�˿ڡ�=structure
//{
//�˿ں�     OAD
//�˿ڲ���   COMDCB��
//}
#define LENper_F202_2     15
#define LENmax_F202_2     LENper_F202_2
#define ADDR_F002_4     ADDR_F202_2+LENmax_F202_2//FTP�ļ�
//����4����������Ϣ����=structure
//{
//IP��ַ  octet-string��
//�˿�    long-unsigned��
//�û���  visible-string��
//����    visible-string
//}
#define LENmax_F002_4     64
#define ADDR_F002_7     ADDR_F002_4+LENmax_F002_4
//����7���ӷ��������أ��ļ���Ϣ��Э�����ͣ�
//������=structure
//{
//	�ļ���Ϣ structure
//	Э������ enum
//	{
//	telnet+zmodemЭ�� ��0����
//	ftpЭ�� ��1����
//	sftpЭ�� ��2����
//	http Э�� ��3����
//	httpsЭ�� ��4��
//	}
//}
#define LENmax_F002_7     128
#define ADDR_F002_8      ADDR_F002_7+LENmax_F002_7
//����8���ϴ������������ļ���Ϣ��Э�����ͣ�
//������ =structure
//{
//�ļ� ��Ϣ structure
//Э������  enum
//}
//�Կͻ���ģʽ��������ָ��Զ�̷������ϴ��ļ�����ͨ����������������ִ�������Դ�ļ��������򷵻ش���Ŀ���ļ��������򴴽���
#define LENmax_F002_8     120//128

//4BYTE ������Ч��־�̶�Ϊ0x12345678
//4BYTE CRC32
#define ADDR_DL698_COMSET_END     (ADDR_F002_8+LENmax_F002_8+8)//ADDR_F002_8+LENmax_F002_8
//---ͨ�������---


//���²����ڳ�ʼ��ʱ����ȫ����0
#define ADDR_MenuKey     ADDR_DL698_COMSET_END//����˵�����
//6byte  ��ʼֵΪ0




//---�¼�����---
#define NUMmax_EventOAD     16//����¼�����OAD������ø���
#define LENmax_EventOAD     82//2+(5*NUMmax_EventOAD)
#if (LENmax_EventOAD!=(2+(5*NUMmax_EventOAD)))
	#error
#endif
#define ADDR_3000     ADDR_MenuKey+6//���ܱ�ʧѹ�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3000_5     ADDR_3000+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//��ѹ��������  long-unsigned����λ��V�����㣺-1����
//  ��ѹ�ָ�����  long-unsigned����λ��V�����㣺-1����
//  ������������  double-long����λ��A�����㣺-4����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3000_5     15
#define ADDR_3001     ADDR_3000_5+LENmax_3000_5//���ܱ�Ƿѹ�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3001_5     ADDR_3001+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3001_5     7
#define ADDR_3002     ADDR_3001_5+LENmax_3001_5//���ܱ��ѹ�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3002_5     ADDR_3002+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3002_5     7
#define ADDR_3003     ADDR_3002_5+LENmax_3002_5//���ܱ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3003_5     ADDR_3003+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
//  ������������  double-long����λ��A�����㣺-4����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3003_5     12
#define ADDR_3004     ADDR_3003_5+LENmax_3003_5//���ܱ�ʧ���¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3004_5     ADDR_3004+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
//  ������������  double-long����λ��A�����㣺-4����
//  �����ָ�����  double-long����λ��A�����㣺-4����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3004_5     17
#define ADDR_3005     ADDR_3004_5+LENmax_3004_5//���ܱ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3005_5     ADDR_3005+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  ������������  double-long����λ��A�����㣺-4����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3005_5     9
#define ADDR_3006     ADDR_3005_5+LENmax_3005_5//���ܱ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3006_5     ADDR_3006+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
//  ������������  double-long����λ��A�����㣺-4����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3006_5     12
#define ADDR_3007     ADDR_3006_5+LENmax_3006_5//���ܱ��������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3007_5     ADDR_3007+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  �й����ʴ�������  double-long����λ��W�����㣺-1����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3007_5     9
#define ADDR_3008     ADDR_3007_5+LENmax_3007_5//���ܱ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3008_5     ADDR_3008+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  �й����ʴ�������  double-long����λ��W�����㣺-1����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3008_5     9
#define ADDR_3009     ADDR_3008_5+LENmax_3008_5//���ܱ������й����������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3009_6     ADDR_3009+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ��������  double-long-unsigned����λ��kW�����㣺-4����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_3009_6     9
#define ADDR_300A     ADDR_3009_6+LENmax_3009_6//���ܱ����й����������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_300A_6     ADDR_300A+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ��������  double-long-unsigned����λ��kW�����㣺-4����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_300A_6     9
#define ADDR_300B     ADDR_300A_6+LENmax_300A_6//���ܱ��޹����������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_300B_5     ADDR_300B+4+LENmax_EventOAD
//����5�����ò�������=structure
//{
//  �������� double-long-unsigned����λ��kvar�����㣺-4����
//  �ж���ʱʱ�� unsigned����λ��s�����㣺0��
//}
#define LENmax_300B_5     9
#define ADDR_300C     ADDR_300B_5+LENmax_300B_5//���ܱ��������������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_300C_6     ADDR_300C+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ���޷�ֵ  long����λ��%�����㣺-1����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_300C_6     7
#define ADDR_300D     ADDR_300C_6+LENmax_300C_6//���ܱ�ȫʧѹ�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_300D_6     ADDR_300D+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_300D_6     2
#define ADDR_300E     ADDR_300D_6+LENmax_300D_6//���ܱ�����Դ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_300E_6     ADDR_300E+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_300E_6     4
#define ADDR_300F     ADDR_300E_6+LENmax_300E_6//���ܱ��ѹ�������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_300F_6     ADDR_300F+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_300F_6     4
#define ADDR_3010     ADDR_300F_6+LENmax_300F_6//���ܱ�����������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3010_6     ADDR_3010+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_3010_6     4
#define ADDR_3011     ADDR_3010_6+LENmax_3010_6//���ܱ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3011_6     ADDR_3011+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_3011_6     4
#define ADDR_3012     ADDR_3011_6+LENmax_3011_6//���ܱ����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3012_6     ADDR_3012+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3012_6     2
#define ADDR_3013     ADDR_3012_6+LENmax_3012_6//���ܱ������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3013_6     ADDR_3013+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3013_6     2
#define ADDR_3014     ADDR_3013_6+LENmax_3013_6//���ܱ����������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3014_6     ADDR_3014+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3014_6     2
#define ADDR_3015     ADDR_3014_6+LENmax_3014_6//���ܱ��¼������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3015_6     ADDR_3015+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3015_6     2
#define ADDR_3016     ADDR_3015_6+LENmax_3015_6//���ܱ�Уʱ�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3016_6     ADDR_3016+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3016_6     2
#define ADDR_3017     ADDR_3016_6+LENmax_3016_6//���ܱ�ʱ�α����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3017_6     ADDR_3017+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3017_6     2
#define ADDR_3018     ADDR_3017_6+LENmax_3017_6//���ܱ�ʱ�������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3018_6     ADDR_3018+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3018_6     2
#define ADDR_3019     ADDR_3018_6+LENmax_3018_6//���ܱ������ձ���¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3019_6     ADDR_3019+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3019_6     2
#define ADDR_301A     ADDR_3019_6+LENmax_3019_6//���ܱ�����ձ���¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_301A_6     ADDR_301A+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_301A_6     2
#define ADDR_301B     ADDR_301A_6+LENmax_301A_6//���ܱ����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_301B_6     ADDR_301B+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_301B_6     2
#define ADDR_301C     ADDR_301B_6+LENmax_301B_6//���ܱ���ť���¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_301C_6     ADDR_301C+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_301C_6     2
#define ADDR_301D     ADDR_301C_6+LENmax_301C_6//���ܱ��ѹ��ƽ���¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_301D_6     ADDR_301D+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ��ֵ  long����λ��%�����㣺-2����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_301D_6     7
#define ADDR_301E     ADDR_301D_6+LENmax_301D_6//���ܱ������ƽ���¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_301E_6     ADDR_301E+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ��ֵ  long����λ��%�����㣺-2����
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_301E_6     7
#define ADDR_301F     ADDR_301E_6+LENmax_301E_6//���ܱ���բ�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_301F_6     ADDR_301F+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_301F_6     2
#define ADDR_3020     ADDR_301F_6+LENmax_301F_6//���ܱ��բ�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3020_6     ADDR_3020+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3020_6     2
#define ADDR_3021     ADDR_3020_6+LENmax_3020_6//���ܱ�ڼ��ձ���¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3021_6     ADDR_3021+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3021_6     2
#define ADDR_3022     ADDR_3021_6+LENmax_3021_6//���ܱ��й���Ϸ�ʽ����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3022_6     ADDR_3022+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3022_6     2
#define ADDR_3023     ADDR_3022_6+LENmax_3022_6//���ܱ��޹���Ϸ�ʽ����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3023_6     ADDR_3023+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
//�¼�����Դ��=enum
//{
//�޹���Ϸ�ʽ1�����֣�0����
//�޹���Ϸ�ʽ2�����֣�1��
//}
#define LENmax_3023_6     2
#define ADDR_3024     ADDR_3023_6+LENmax_3023_6//���ܱ���ʲ��������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3024_6     ADDR_3024+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3024_6     2
#define ADDR_3025     ADDR_3024_6+LENmax_3024_6//���ܱ���ݱ����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3025_6     ADDR_3025+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3025_6     2
#define ADDR_3026     ADDR_3025_6+LENmax_3025_6//���ܱ���Կ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3026_6     ADDR_3026+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3026_6     2
#define ADDR_3027     ADDR_3026_6+LENmax_3026_6//���ܱ��쳣�忨�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3027_6     ADDR_3027+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3027_6     2
#define ADDR_3028     ADDR_3027_6+LENmax_3027_6//���ܱ����¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3028_6     ADDR_3028+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3028_6     2
#define ADDR_3029     ADDR_3028_6+LENmax_3028_6//���ܱ��˷Ѽ�¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3029_6     ADDR_3029+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3029_6     2
#define ADDR_302A     ADDR_3029_6+LENmax_3029_6//���ܱ�㶨�ų������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_302A_6     ADDR_302A+4+LENmax_EventOAD//���޶�����ɾ������6
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_302A_6     4
#define ADDR_302B     ADDR_302A_6+LENmax_302A_6//���ܱ��ɿ��������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_302B_6     ADDR_302B+4+LENmax_EventOAD//���޶�����ɾ������6
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_302B_6     4
#define ADDR_302C     ADDR_302B_6+LENmax_302B_6//���ܱ��Դ�쳣�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_302C_6     ADDR_302C+4+LENmax_EventOAD//���޶�����ɾ������6
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_302C_6     4
#define ADDR_302D     ADDR_302C_6+LENmax_302C_6//���ܱ�������ز�ƽ���¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_302D_6     ADDR_302D+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ��ֵ  long����λ��%�����㣺-2��
//  �ж���ʱʱ��  unsigned����λ��s�����㣺0��
//}
#define LENmax_302D_6     7
#define ADDR_302E     ADDR_302D_6+LENmax_302D_6//���ܱ�ʱ�ӹ����¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_302E_6     ADDR_302E+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_302E_6     4
#define ADDR_302F     ADDR_302E_6+LENmax_302E_6//���ܱ����оƬ�����¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_302F_6     ADDR_302F+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
#define LENmax_302F_6     4
#define ADDR_3030     ADDR_302F_6+LENmax_302F_6//ͨ��ģ�����¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3030_6     ADDR_3030+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �ж���ʱ  unsigned����λ��s�����㣺0��
//}
//�¼�����Դ��=OAD
//�¼�����ԴΪͨ��ģ��OAD��
#define LENmax_3030_6     4


#define ADDR_3100     ADDR_3030_6+LENmax_3030_6//�ն˳�ʼ���¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3100_6     ADDR_3100+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3100_6     2
#define ADDR_3101     ADDR_3100_6+LENmax_3100_6//�ն˰汾����¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3101_6     ADDR_3101+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3101_6     2
#define ADDR_3104     ADDR_3101_6+LENmax_3101_6//�ն�״̬����λ�¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3104_6     ADDR_3104+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3104_6     2
#define ADDR_3105     ADDR_3104_6+LENmax_3104_6//���ܱ�ʱ�ӳ����¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3105_6     ADDR_3105+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//�쳣�б���ֵ  long-unsigned����λ���룩��
//�����ɼ������ unsigned
//}
//�¼�����Դ��=TSA
//�ɼ������������Ҫ�������OAD�Ĳɼ�����
#define LENmax_3105_6     7
#define ADDR_3106     ADDR_3105_6+LENmax_3105_6//�ն�ͣ/�ϵ��¼�	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3106_6     ADDR_3106+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//ͣ�����ݲɼ����ò���  structure
//{
//�ɼ���־  bit-string(SIZE(8))��
//ͣ���¼�����ʱ������Сʱ�� unsigned ��
//ͣ���¼�����ʱ����ֵ�����ӣ� unsigned��
//��Ҫ��ȡͣ���¼����ܱ�       array TSA
//}��
//ͣ���¼������ֵ����  structure
//{
//ͣ��ʱ����С��Ч��������ӣ� long-unsigned��
//ͣ��ʱ�������Ч��������ӣ� long-unsigned��
//ͣ���¼���ֹʱ��ƫ����ֵ�����ӣ� long-unsigned��
//ͣ���¼�ʱ������ƫ����ֵ�����ӣ� long-unsigned��
//ͣ�緢����ѹ��ֵ  long-unsigned����λ��V�����㣺-1����
//ͣ��ָ���ѹ��ֵ  long-unsigned����λ��V�����㣺-1��
//}
//}
//�ɼ���־��
//bit0���á�1����Ч���á�0����Ч��
//bit1���á�1�����ѡ������㣬�á�0��ֻ�ɼ����õĲ����㡣
#define LENmax_3106_6     84
#define ADDR_3107     ADDR_3106_6+LENmax_3106_6//�ն�ֱ��ģ����Խ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3107_6     ADDR_3107+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ֱ��ģ��������  double-long
//}
//�¼�����Դ��=OAD��ֱ��ģ�����ţ�
#define LENmax_3107_6     7
#define ADDR_3108     ADDR_3107_6+LENmax_3107_6//�ն�ֱ��ģ����Խ�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3108_6     ADDR_3108+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ֱ��ģ��������  double-long
//}
//�¼�����Դ��=OAD��ֱ��ģ�����ţ�
#define LENmax_3108_6     7
#define ADDR_3109     ADDR_3108_6+LENmax_3108_6//�ն���Ϣ��֤�����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3109_6     ADDR_3109+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3109_6     2
#define ADDR_310A     ADDR_3109_6+LENmax_3109_6//�豸���ϼ�¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_310A_6     ADDR_310A+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
//�¼�����Դ��=enum
//{
//  �ն������ڴ���ϣ�0����
//  ʱ�ӹ���        ��1����
//  ����ͨ�Ź���    ��2����
//  485�������    ��3����
//  ��ʾ�����      ��4����
//  �ز�ͨ���쳣    ��5��
//}
#define LENmax_310A_6     2
#define ADDR_310B     ADDR_310A_6+LENmax_310A_6//���ܱ�ʾ���½��¼�;ADDR_310B-ADDR_310E_6���ֿܷ����,�����¼�ʱ1�ζ���
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_310B_6     ADDR_310B+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �����ɼ������ unsigned
//}
//�¼�����Դ��=TSA
//�ɼ������������Ҫ�������OAD�Ĳɼ�����
#define LENmax_310B_6     4
#define ADDR_310C     ADDR_310B_6+LENmax_310B_6//�����������¼�;ADDR_310B-ADDR_310E_6���ֿܷ����,�����¼�ʱ1�ζ���
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_310C_6     ADDR_310C+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ��ֵ  double-long-unsigned����λ��%���޻��㣩,
//  �����ɼ������ unsigned
//}
//�¼�����Դ��=TSA
//�ɼ������������Ҫ�������OAD�Ĳɼ�����
#define LENmax_310C_6     9
#define ADDR_310D     ADDR_310C_6+LENmax_310C_6//���ܱ�����¼�;ADDR_310B-ADDR_310E_6���ֿܷ����,�����¼�ʱ1�ζ���
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_310D_6     ADDR_310D+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ��ֵ  double-long-unsigned����λ��%���޻��㣩,
//  �����ɼ������ unsigned
//}
//�¼�����Դ��=TSA
//�ɼ������������Ҫ�������OAD�Ĳɼ�����
#define LENmax_310D_6     9
#define ADDR_310E     ADDR_310D_6+LENmax_310D_6//���ܱ�ͣ���¼�;ADDR_310B-ADDR_310E_6���ֿܷ����,�����¼�ʱ1�ζ���
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_310E_6     ADDR_310E+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ��ֵ           TI,
//  �����ɼ������ unsigned
//}
//�¼�����Դ��=TSA
//�ɼ������������Ҫ�������OAD�Ĳɼ�����
#define LENmax_310E_6     8
#define ADDR_310F     ADDR_310E_6+LENmax_310E_6//�ն˳���ʧ���¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_310F_6     ADDR_310F+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �����ִ�      unsigned,
//  �����ɼ������ unsigned
//}
//�¼�����Դ��=TSA
#define LENmax_310F_6     6
#define ADDR_3110     ADDR_310F_6+LENmax_310F_6//��ͨ�����������¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3110_6     ADDR_3110+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  ͨ����������  double-long-unsigned����λ��byte��
//}
#define LENmax_3110_6     7
#define ADDR_3111     ADDR_3110_6+LENmax_3110_6//����δ֪���ܱ��¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3111_6     ADDR_3111+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3111_6     2
#define ADDR_3112     ADDR_3111_6+LENmax_3111_6//��̨�����ܱ��¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3112_6     ADDR_3112+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3112_6     2
#define ADDR_3114     ADDR_3112_6+LENmax_3112_6//�ն˶�ʱ�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3114_6     ADDR_3114+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3114_6     2
#define ADDR_3115     ADDR_3114_6+LENmax_3114_6//ң����բ��¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3115_6     ADDR_3115+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
//�¼�����Դ��=unsigned���̵�����ţ�
#define LENmax_3115_6     2
#define ADDR_3116     ADDR_3115_6+LENmax_3115_6//�й��ܵ������Խ���¼���¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3116_6     ADDR_3116+4+LENmax_EventOAD
//����6�����ò�������=array �й��ܵ������������
//�й��ܵ�����������á�=structure
//{
//  �й��ܵ����������� unsigned��
//  �Աȵ��ܼ���           OI��
//  ���յ��ܼ���           OI��
//  �����ĵ�������ʱ�����估�Աȷ�����־ bit-string��SIZE��8������
//  �Խ�����ƫ��ֵ integer����λ��%�����㣺0����
//  �Խ�޾���ƫ��ֵ long64����λ��kWh�����㣺-4��
//}
//�����ĵ�������ʱ�����估�Աȷ�����־��
//bit0��bit1�����ʾ��������ʱ���ȣ�ȡֵ��Χ0��2���α�ʾ60���ӵ�����30���ӵ�����15���ӵ���������ֵ��Ч��
//bit7��ʾ�Աȷ�����־���á�0������ԶԱȣ���ʽ����ʽ��1�����á�1�������ԶԱȣ���ʽ����ʽ��2����
//bit2��bit6���á�
//	 ��  .....	(1)
//	  .............	(2)
//ʽ�У�
//    Q�����Աȵ��ܼ����ܵ�������
//    q�������յ��ܼ����ܵ�������
#define LENmax_3116_6     194
#define ADDR_3117     ADDR_3116_6+LENmax_3116_6//�����·����״̬��λ�¼���¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3117_6     ADDR_3117+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3117_6     2
#define ADDR_3118     ADDR_3117_6+LENmax_3117_6//�ն˱�̼�¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3118_6     ADDR_3118+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3118_6     2
#define ADDR_3119     ADDR_3118_6+LENmax_3118_6//�ն˵�����·�쳣�¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3119_6     ADDR_3119+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
//�¼�����Դ��=enum{��·(0)����·(1)}
#define LENmax_3119_6     2
#define ADDR_311A     ADDR_3119_6+LENmax_3119_6//���ܱ�����״̬�л��¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_311A_6     ADDR_311A+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//   �ж���ʱʱ��  long-unsigned����λ��s�����㣺0��
//}
//�¼�����Դ��=NULL
//���¼�ֻ��¼�������״̬��Ǩ��
#define LENmax_311A_6     5
#define ADDR_311B     ADDR_311A_6+LENmax_311A_6//�ն˶Ե��Уʱ��¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_311B_6     ADDR_311B+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_311B_6     5
#define ADDR_311C     ADDR_311B_6+LENmax_311B_6//���ܱ����ݱ����ؼ�¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_311C_6     ADDR_311C+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �����ɼ��������    unsigned
//}
//�¼�����Դ��=TSA
#define LENmax_311C_6     4

#define ADDR_311F     ADDR_311C_6+LENmax_311C_6//�ܾ��ӽڵ�������¼ 
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_311F_6     ADDR_311F+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//  �����ɼ��������    unsigned
//}
//�¼�����Դ��=TSA
#define LENmax_311F_6     3

#define ADDR_3200     ADDR_311F_6+LENmax_311F_6//������բ��¼
//#define ADDR_3200     ADDR_311C_6+LENmax_311C_6//������բ��¼

//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3200_6     ADDR_3200+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3200_6     2
#define ADDR_3201     ADDR_3200_6+LENmax_3200_6//�����բ��¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3201_6     ADDR_3201+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3201_6     2
#define ADDR_3202     ADDR_3201_6+LENmax_3201_6//����������ü�¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3202_6     ADDR_3202+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
//�¼�����Դ��=OI
#define LENmax_3202_6     2
#define ADDR_3203     ADDR_3202_6+LENmax_3202_6//��ظ澯�¼���¼
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3203_6     ADDR_3203+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3203_6     2

//---�¼���������---

	 
//---ͨ����---
#define ADDR_4000_3     ADDR_3203_6+LENmax_3203_6//Уʱģʽ
//����3��Уʱģʽ����=enum
//{
//��վ��ʱ��0����
//�ն˾�ȷУʱ��1����
//����/GPS��2����
//������255��
//}
#define LENmax_4000_3     2
#define ADDR_4000_4     ADDR_4000_3+LENmax_4000_3//��׼Уʱ����
//����4����׼Уʱ��������=structure
//{
//�������ʱ���ܸ���   unsigned��
//���ֵ�޳�����       unsigned��
//��Сֵ�޳�����       unsigned��
//ͨѶ��ʱ��ֵ         unsigned����λ���룩�� 
//������Ч����         unsigned
//}
#define LENmax_4000_4     12
#define ADDR_4004     ADDR_4000_4+LENmax_4000_4//�豸����λ��
//����2��=structure
//{
	//����  structure
	//{
		//��λ   enum{E��0����W��1��}��
		//��     unsigned��
		//��     unsigned��
		//��     unsigned
	//}��
	//γ��  structure
	//{
		//��λ   enum{S��0����N��1��}��
		//��     unsigned��
		//��     unsigned��
		//��     unsigned
	//}��
	//�߶ȣ�cm��  double-long-unsigned
//}
#define LENmax_4004     30
#define ADDR_4006     ADDR_4004+LENmax_4004//ʱ��Դ
//����2��ֻ������=structure
//{
//	ʱ��Դ  enum
//	{
//		�ڲ���0����ʱ��оƬ��1����������ʱ�ӣ�2��������ʱ�ӣ�3��������ʱ�ӣ�4��
//	}��
//	״̬  enum
//	{
//		���ã�0���������ã�1��
//  }
//}
#define LENmax_4006     16
#define ADDR_4007     ADDR_4006+LENmax_4006//LCD����
//����2::=structure
//{
//  �ϵ�ȫ��ʱ��  unsigned��
//  �������ʱ��  long-unsigned(����ʱ�������ʱ��)��
//  ��ʾ�鿴�������ʱ��  long-unsigned��
//  �е簴����Ļפ�����ʱ��  long-unsigned��
//  �޵簴����Ļפ�����ʱ��  long-unsigned��
//  ��ʾ����С��λ��  unsigned��
//  ��ʾ���ʣ����������С��λ��  unsigned
//}
//����ʱ��ĵ�λ��Ϊ���롣
#define LENmax_4007     20


//---���ܱ���---
#define ADDR_iMeterControlData     ADDR_4007+LENmax_4007
//1byte
//1BYTE ����ʱ�����л�״̬0=1��,1=2��
//1BYTE ������ʱ�α��л�̬0=1��,1=2��
//1BYTE �����ݶ��л�״̬0=1��,1=2��
//1BYTE ���׷��ʵ���л�0=1��,1=2��
//1byte 
//7byte YYMDHMS hex����ʱ�����л�ʱ��
//1byte 
//7byte YYMDHMS hex������ʱ�α��л�ʱ��
//1byte 
//7byte YYMDHMS hex�����ݶ��л�ʱ��
//1byte 
//7byte YYMDHMS hex���׷��ʵ���л�ʱ��
//1byte 
#define ADDR_4008     ADDR_iMeterControlData+38//������ʱ�����л�ʱ��,�����ϲ��ֿ�1��д��
//����2��=date_time_s
//��=FFH
#define LENmax_4008     8
#define ADDR_4009     ADDR_4008+LENmax_4008//��������ʱ���л�ʱ��
//����2��=date_time_s
//��=FFH
#define LENmax_4009     8
#define ADDR_400A     ADDR_4009+LENmax_4009//�����׷�ʱ�����л�ʱ��
//����2��=date_time_s
//��=FFH
#define LENmax_400A     8
#define ADDR_400B     ADDR_400A+LENmax_400A//�����׽��ݵ���л�ʱ��
//����2��=date_time_s
//��=FFH
#define LENmax_400B     8
#define ADDR_400C     ADDR_400B+LENmax_400B//ʱ��ʱ����
//����2��=structure
//{
//��ʱ����(p��14)                 unsigned��
//��ʱ�α�����q��8��              unsigned��
//��ʱ����(ÿ���л���)��m��14��   unsigned��
//��������k��63��                 unsigned��
//������������n��254��            unsigned
//}
#define LENmax_400C     12
#define ADDR_400D     ADDR_400C+LENmax_400C//������
//����2��=unsigned���޵�λ���޻���
#define LENmax_400D     2
#define ADDR_400E     ADDR_400D+LENmax_400D//г����������
//����2��=unsigned���޵�λ���޻���
#define LENmax_400E     2
#define ADDR_400F     ADDR_400E+LENmax_400E//��Կ������
//����2��=unsigned���޵�λ���޻���
#define LENmax_400F     2
#define ADDR_4010     ADDR_400F+LENmax_400F//����Ԫ����
//����2��=unsigned���޵�λ���޻���
#define LENmax_4010     2
#define ADDR_4011     ADDR_4010+LENmax_4010//�������ձ�
//����2��=array ��������
//�������ա�=structure
//{
//  ����        date��
//  ��ʱ�α��  unsigned
//}
//���ܱ�ʵ��ʹ�ö��ٸ���������ȡ���ڹ�����������������ܱ������ٿɱ���20���������ա�
#define LENper_4011     8
#define NUMmax_4011     30
#define LENmax_4011     (2+(LENper_4011*NUMmax_4011))
#define ADDR_4012     ADDR_4011+LENmax_4011//������������
//����2��=bit-string(SIZE(8))
#define LENmax_4012     3
#define ADDR_4013     ADDR_4012+LENmax_4012//��������õ���ʱ�α��
//����2��=unsigned���޵�λ���޻���
#define LENmax_4013     2
#define ADDR_4014     ADDR_4013+LENmax_4013//��ǰ��ʱ����
//����2��=array
//ʱ����=structure
//{
//��          unsigned,
//��          unsigned,
//  ��ʱ�α��  unsigned
//}
#define LENper_4014     8
#define NUMmax_4014     14
#define LENmax_4014     (2+(LENper_4014*NUMmax_4014))
#define ADDR_4015     ADDR_4014+LENmax_4014//������ʱ����
//����2��=array
//ʱ����=structure
//{
//��          unsigned,
//��          unsigned,
//  ��ʱ�α��  unsigned
//}
#define LENper_4015     8
#define NUMmax_4015     14
#define LENmax_4015     (2+(LENper_4015*NUMmax_4015))
#define ADDR_4016     ADDR_4015+LENmax_4015//��ǰ����ʱ�α�
//����2��=array ��ʱ�α�
//��ʱ�α��=array ʱ��
//ʱ�Ρ�=structure
//{
//  ʱ     unsigned,
//  ��     unsigned,
//  ���ʺ� unsigned
//}
//���ʺţ���ʱ�β��õķ��ʺ�
#define LENper_4016     (2+(8*14))
#define NUMmax_4016     8
#define LENmax_4016     (2+(LENper_4016*NUMmax_4016))
#define ADDR_4017     ADDR_4016+LENmax_4016//��������ʱ�α�
//����2��=array ��ʱ�α�
//��ʱ�α��=array ʱ��
//ʱ�Ρ�=structure
//{
//  ʱ     unsigned,
//  ��     unsigned,
//  ���ʺ� unsigned
//}
//���ʺţ���ʱ�β��õķ��ʺ�
#define LENper_4017     (2+(8*14))
#define NUMmax_4017     8
#define LENmax_4017     (2+(LENper_4017*NUMmax_4017))
#define ADDR_4018     ADDR_4017+LENmax_4017//��ǰ�׷��ʵ��
//����2��=array ���ʵ��
//���ʵ�ۡ�=double-long-unsigned
//��λ��Ԫ/kWh�����㣺-4�����ܱ����֧��32�����ʵ�ۣ���ǰ�׷��ʵ��Э��ֻ����
#define LENper_4018     5
#define NUMmax_4018     8
#define LENmax_4018     (2+(LENper_4018*NUMmax_4018))
#define ADDR_4019     ADDR_4018+LENmax_4018//�����׷��ʵ��
//����2��=array ���ʵ��
//���ʵ�ۡ�=double-long-unsigned
//��λ��Ԫ/kWh�����㣺-4�����ܱ����֧��32�����ʵ�ۣ���ǰ�׷��ʵ��Э��ֻ����
#define LENper_4019     5
#define NUMmax_4019     8
#define LENmax_4019     (2+(LENper_4019*NUMmax_4019))
#define ADDR_401A     ADDR_4019+LENmax_4019//��ǰ�׽��ݵ��
//���ݲ�����=structure
//{
//  ����ֵ����    array ����ֵ��
//  ���ݵ������  array ���ݵ�ۣ�
//  ���ݽ���������  array ���ݽ�����
//}
//����ֵ��=double-long-unsigned
//���ݵ�ۡ�=double-long-unsigned
//���ݽ����ա�=structure
//{
//��  unsigned��
//  ��  unsigned��
//  ʱ  unsigned
//}
//����ֵ����λ��kWh�����㣺-2
//���ݵ�ۣ���λ��Ԫ/kWh�����㣺-4
//���ݽ��������¡��ա�ʱ����Чʱ�򰴽�����ִ�н��ݶ��ᣬ�����1���ݽ������н��¡�����Ч���Դ�ÿ�¸���ʱִ�н��ݶ��ᣬ��ȫ����Чʱ��ִ�н��ݶ��ᡣ
//��ǰ�׽��ݲ���Э��ֻ����
#define LENmax_401A     512
#define ADDR_401B     ADDR_401A+LENmax_401A//�����׽��ݵ��
//ͬ401A,�����׽��ݲ����ɶ���д
#define LENmax_401B     512
#define ADDR_401C     ADDR_401B+LENmax_401B//�������������
//����2��=double-long-unsigned���޵�λ������:-2
#define LENmax_401C     8
#define ADDR_401D     ADDR_401C+LENmax_401C//��ѹ���������
//����2��=double-long-unsigned���޵�λ������:-2
#define LENmax_401D     8
#define ADDR_401E     ADDR_401D+LENmax_401D//���������ֵ
//����2����������=structure
//{
//���������ֵ1    double-long-unsigned��
//���������ֵ2    double-long-unsigned��
//}
//��λ��Ԫ�����㣺-2
#define LENmax_401E     12
#define ADDR_401F     ADDR_401E+LENmax_401E//���������ֵ
//����2����������=structure
//{
//͸֧�����ֵ     double-long-unsigned��
//�ڻ������ֵ     double-long-unsigned��
//��բ��������ֵ double-long-unsigned��
//}
//��λ��Ԫ�����㣺-2
#define LENmax_401F     18
#define ADDR_4020     ADDR_401F+LENmax_401F//����������ֵ
//����2��=structure
//{
//����������ֵ1    double-long-unsigned��
//����������ֵ2    double-long-unsigned��
//}
//��λ��kWh�����㣺-2
#define LENmax_4020     12
#define ADDR_4021     ADDR_4020+LENmax_4020//����������ֵ
//����2��=structure
//{
//�ڻ�������ֵ     double-long-unsigned��
//͸֧������ֵ     double-long-unsigned��
//��բ���������ֵ double-long-unsigned��
//}
//��λ��kWh�����㣺-2
#define LENmax_4021     18
#define ADDR_4023     ADDR_4021+LENmax_4021//��֤��Чʱ��
//����2��=long-unsigned ����λ�����ӣ��޻���
#define LENmax_4023     3
#define ADDR_4024     ADDR_4023+LENmax_4023//�޳�
//����2��=enum {�޳�Ͷ�루1�����޳������2��}
#define LENmax_4024     2
#define ADDR_4030     ADDR_4024+LENmax_4024//��ѹ�ϸ��ʲ���
//����2��=structure
//{
//	��ѹ��������  long-unsigned����λ��V�����㣺-1����
//  ��ѹ��������  long-unsigned����λ��V�����㣺-1����
//	��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1����
//  ��ѹ�ϸ�����  long-unsigned����λ��V�����㣺-1��
//}
#define LENmax_4030     14
#define ADDR_4100     ADDR_4030+LENmax_4030//�����������
//����2��=unsigned����λ�����ӣ����㣺0
#define LENmax_4100     2
#define ADDR_4101     ADDR_4100+LENmax_4100//����ʱ��
//����2��=unsigned����λ�����ӣ����㣺0
#define LENmax_4101     2
#define ADDR_4102     ADDR_4101+LENmax_4101//У��������
//����2��=unsigned����λ�����룬���㣺0
#define LENmax_4102     2
#define ADDR_4103     ADDR_4102+LENmax_4102//�ʲ��������
//����2��=visible-string(SIZE(32))
#define LENmax_4103     34

#define ADDR_410C     ADDR_4103+LENmax_4103//ABC����絼ϵ��
//����2��=structure
//{
//A ��絼  long����λ���ޣ����㣺-3
//B ��絼  long����λ���ޣ����㣺-3
//C ��絼  long����λ���ޣ����㣺-3
// }
#define LENmax_410C     12
#define ADDR_410D     ADDR_410C+LENmax_410C//ABC����翹ϵ��
//����2��=structure
//{
//A ��翹  long����λ���ޣ����㣺-3
//B ��翹  long����λ���ޣ����㣺-3
//C ��翹  long����λ���ޣ����㣺-3
// }
#define LENmax_410D     12
#define ADDR_410E     ADDR_410D+LENmax_410D//ABC�������ϵ��
//����2��=structure
//{
//A �����  long����λ���ޣ����㣺-3
//B �����  long����λ���ޣ����㣺-3
//C �����  long����λ���ޣ����㣺-3
// }
#define LENmax_410E     12
#define ADDR_410F     ADDR_410E+LENmax_410E//ABC�������ϵ��
//����2��=structure
//{
//A �����  long����λ���ޣ����㣺-3
//B �����  long����λ���ޣ����㣺-3
//C �����  long����λ���ޣ����㣺-3
// }
#define LENmax_410F     12
#define ADDR_4112     ADDR_410F+LENmax_410F//�й���Ϸ�ʽ������
//����2��=bit-string(SIZE(8))������¼G
#define LENmax_4112     3
#define ADDR_4113     ADDR_4112+LENmax_4112//�޹���Ϸ�ʽ1������
//����2��=bit-string(SIZE(8))������¼G
#define LENmax_4113     3
#define ADDR_4114     ADDR_4113+LENmax_4113//�޹���Ϸ�ʽ2������
//����2��=bit-string(SIZE(8))������¼G
#define LENmax_4114     3
#define ADDR_4115     ADDR_4114+LENmax_4114//IC��
//����2��=structure
//{
//IC������  enum
//{
//����Ԥ�ƿ���0�����û���������1����
//�û����翨��2�����û�������3��
//}��
//}
#define LENmax_4115     4
#define ADDR_4116     ADDR_4115+LENmax_4115//������
//����2�����ò�������=array ����������
//���������ڡ�=structure
//{
//��  unsigned,
//ʱ  unsigned
//}
#define LENper_4116     6
#define NUMmax_4116     31
#define LENmax_4116     (2+(LENper_4116*NUMmax_4116))
#define ADDR_4117     ADDR_4116+LENmax_4116//�ڼ�������������
//����2�����ò�������= TI
#define LENmax_4117     5




//---�ն���---

//4200	11	·�ɱ�
//4201	8	·����Ϣ��Ԫ

#define ADDR_4202     ADDR_4117+LENmax_4117//����ͨ�Ų���
//����2��=structure
//{
//������־            bool��
//����ͨ�Ŷ˿ں�      OAD��
//�ܵȴ���ʱ��10ms��  long-unsigned��
//�ֽڳ�ʱ��10ms��    long-unsigned��
//�ط�����            unsigned��
//Ѳ�����ڣ�min��     unsigned��
//�����������˿���    unsigned��
//�����������ն˵�ַ  array TSA
//}
#define LENmax_4202     128
#define ADDR_4204_2     ADDR_4202+LENmax_4202//�ն˹㲥Уʱ-2
//����2���ն˹㲥Уʱ��������=structure
//{
//     �ն˹㲥Уʱ����ʱ��  time��
//     �Ƿ�����              bool
//}
#define LENmax_4204_2     16
#define ADDR_4204_3     ADDR_4204_2+LENmax_4204_2//�ն˹㲥Уʱ-3
//����3���ն˵���ַ�㲥Уʱ��������=structure
//{
//     ʱ�������ֵ          unsigned����λ���룩��
//     �ն˹㲥Уʱ����ʱ��  time��
//     �Ƿ�����              bool
//}
//���ʱ��������ն��Զ������ã�ǰ���ǲɼ������������вɼ����ʱ�ӡ�ִ�е���ַ�㲥Уʱ�����ɶ�Ӧ���ն˶Ե��Уʱ�¼�
#define LENmax_4204_3     16



#define ADDR_5000     ADDR_4204_3+LENmax_4204_3//˲ʱ��������������Ա�
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
#define LENmax_50xx     (2+(NUMmax_50xx*13))
#define ADDR_5001     ADDR_5000+LENmax_50xx//�붳������������Ա�
//ͬ��
#define ADDR_5002     ADDR_5001+LENmax_50xx//���Ӷ�������������Ա�
//ͬ��
#define ADDR_5003     ADDR_5002+LENmax_50xx//Сʱ��������������Ա�
//ͬ��
#define ADDR_5004     ADDR_5003+LENmax_50xx//�ն�������������Ա�
//ͬ��
#define ADDR_5005     ADDR_5004+LENmax_50xx//�����ն�������������Ա�
//ͬ��
#define ADDR_5006     ADDR_5005+LENmax_50xx//�¶�������������Ա�
//ͬ��
#define ADDR_5007     ADDR_5006+LENmax_50xx//�궳������������Ա�
//ͬ��
#define ADDR_5008     ADDR_5007+LENmax_50xx//ʱ�����л���������������Ա�
//ͬ��
#define ADDR_5009     ADDR_5008+LENmax_50xx//��ʱ�α��л���������������Ա�
//ͬ��
#define ADDR_500A     ADDR_5009+LENmax_50xx//���ʵ���л���������������Ա�
//ͬ��
#define ADDR_500B     ADDR_500A+LENmax_50xx//�����л���������������Ա�
//ͬ��
#define ADDR_5011     ADDR_500B+LENmax_50xx//���ݽ��㶳������������Ա�
//ͬ��





#define ADDR_6000     ADDR_5011+LENmax_50xx//�ɼ��������ñ�(��ʼ��ֵ0x00)
//����2�����ñ�ֻ������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//	�������  long-unsigned,
//	������Ϣ  Basic_object��
//	��չ��Ϣ  Extended_object��
//	������Ϣ  Annex_object
//}

//Basic_object��=structure
//{
//	ͨ�ŵ�ַ  TSA��
//	������    enum
//	{
//		300bps��0����  600bps��1����    1200bps��2����
//		2400bps��3���� 4800bps��4����   7200bps��5����
//		9600bps��6���� 19200bps��7����  38400bps��8����
//		57600bps��9����115200bps��10��������Ӧ��255��
//	}��
//	��Լ����  enum
//	{
//  	δ֪ (0)��
//		DL/T645-1997��1����
//		DL/T645-2007��2����
//		DL/T698.45��3����
//		CJ/T 188-2004��4��
//	}��
//	�˿�      OAD��
//	ͨ������  octet-string��
//	���ʸ���  unsigned��
//	�û�����  unsigned��
//	���߷�ʽ  enum
//	{
//   δ֪��0����
//   ���ࣨ1����
//   �������ߣ�2����
//   �������ߣ�3��
//	}
//���ѹ    long-unsigned(����-1����λV),
//�����    long-unsigned(����-1����λA)
//}

//Extended_object��=structure
//{
//	�ɼ�����ַ  TSA��
//	�ʲ���      octet-string��
//	PT          long-unsigned��
//	CT          long-unsigned
//}
//Annex_object��=array structure
//{
//	������������  OAD��
//	����ֵ        Data
//}
#define LENper_6000     128
#define NUMmax_6000     NUM_RMmax
#define LENmax_6000     (LENper_6000*NUMmax_6000)

#define ADDR_6002_8     ADDR_6000+LENmax_6000//�ѱ����
//����8��=structure
//{
//	�Ƿ�����ÿ�������ѱ�   bool��
//	�Զ����²ɼ�����       bool��
//	�Ƿ�����ѱ�����¼�   bool��
//	����ѱ���ѡ��       enum
//	{
//		�����             ��0����
//		ÿ�������ѱ�ǰ��� ��1����
//		ÿ���ѱ�ǰ���     ��2��
//	}
//}
#define LENmax_6002_8     10
#define ADDR_6002_9     ADDR_6002_8+LENmax_6002_8//ÿ�������ѱ��������
//����9��ÿ�������ѱ�������ã���=array ��ʱ�ѱ����
//��ʱ�ѱ������=structure
//{
//   ��ʼʱ��         time��
//   �ѱ�ʱ����min��  long-unsigned
//}
#define LENmax_6002_9     (2+(9*12))
#define ADDR_6002_127     ADDR_6002_9+LENmax_6002_9//ʵʱ�����ѱ�(��ʼ��ֵ0xFF)
//�ѱ�ʱ����=long-unsigned����λ�����ӣ���ʾ�ѱ����ʱ�䣬0��ʾ����ʱ��ֱ���ѱ������
#define LENmax_6002_127     3
#define ADDR_6012_2     ADDR_6002_127+LENmax_6002_127//�������ñ�(��ʼ��ֵ0x00)
//����2���������õ�Ԫ����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//	����ID    unsigned��
//	ִ��Ƶ��   TI��
//	��������   enum
//	{
//		��ͨ�ɼ����� ��1����   �¼��ɼ����� ��2����
//		͸������     ��3����   �ϱ�����     ��4����
//		�ű�����     ��5��
//	}��
//	�������    unsigned��
//	��ʼʱ��    date_time_s��
//	����ʱ��    date_time_s��
//	��ʱ        TI��
//	ִ�����ȼ�  unsigned{��Ҫ��1������Ҫ��2������Ҫ��3�������ܣ�4��}��
//	״̬        enum{������1����ͣ�ã�2��}��
//	����ʼǰ�ű�id   long-unsigned��
//	������ɺ�ű�id   long-unsigned��
//	��������ʱ��       structure��
//}
//����������Ϊ�ű�ʱ���������Ϊ�ű�id
//��������ʱ�Ρ�=structrue
//{
//	����  enum
//	{
//		ǰ�պ�    ��0����
//		ǰ�����    ��1����
//		ǰ�պ��    ��2����
//		ǰ����    ��3��
//	}��
//	ʱ�α�  array ʱ��
//}
//ʱ�Ρ�=structure
//{
//	��ʼСʱ  unsigned��
//	��ʼ����  unsigned��
//	����Сʱ  unsigned��
//	��������  unsigned
//}
#define LENper_6012_2     128
#define LENmax_6012_2     (LENper_6012_2*NUMmax_6012_2)
#define ADDR_6014     ADDR_6012_2+LENmax_6012_2//��ͨ�ɼ�����
//����2����ͨ�ɼ���������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//	�������  unsigned��
//	�洢���  long-unsigned��
//	�ɼ���ʽ  structure
//	{
//  	�ɼ�����  unsigned��
//  	�ɼ�����  Data
//	}��
//	��¼��ѡ��  array CSD��
//	���ܱ���  MS��
//	�洢ʱ��ѡ�� enum
//	{
//		δ����              ��0����
//		����ʼʱ��        ��1����
//		��Ե���0��0��    ��2����
//		�������23��59��  ��3����
//		�������0��0��    ��4����
//		��Ե���1��0��0�֣�5����
//		���ݶ���ʱ��        ��6��
//    ���������ĩ23��59�� (7)
//	}
//}
//�ɼ���ʽ���ݸ�ʽ���£�
//�ɼ�����	 �ɼ�����	 ��ʾ
//0	       NULL	     �ɼ���ǰ����
//1	       unsigned	 �ɼ��ϵ�N��
//2				 NULL	     ������ʱ��ɼ�
//3	       TI	       ��ʱ�����ɼ�
//4        RetryMetering ����

//RetryMetering::=structure
//{
//	����ʱ���� TI
//	��������(��N��) long-unsigned
//}
#define LENper_6014     512
#define LENmax_6014     (LENper_6014*NUMmax_6014)
#define ADDR_6016     ADDR_6014+LENmax_6014//�¼��ɼ�������(��ʼ��ֵ0x00)
//����2���¼��ɼ�����Event acq plan����=structure
//{
//�������        unsigned��
//�ɼ���ʽ        structure
//{
//  �ɼ�����      unsigned��
//  �ɼ�����      Data
//}
//���ܱ���      MS��
//�ϱ���ʶ        bool��True�������ϱ���False�����ϱ�����
//�洢���        long-unsigned
//}
//�ɼ���ʽ���ݸ�ʽ���£�
//�ɼ�����	 �ɼ�����	��ʾ
//0	array ROAD	���ڲɼ��¼�����
//1	NULL	����֪ͨ�ɼ������¼�����
//2	array ROAD	����֪ͨ�ɼ�ָ���¼�����
//3	NULL ����֪ͨ�洢���ɵ��¼�����
#define LENper_6016     1024
#define LENmax_6016     (LENper_6016*NUMmax_6016)
#define ADDR_6018     ADDR_6016+LENmax_6016//͸��������(��ʼ��ֵ0x00)
//����2��͸����������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//	�������     unsigned��
//	�������ݼ�   array �������ݣ�
//	�������ݡ�=structure
//	{
//		ͨ�ŵ�ַ        	TSA��
//		��ʼǰ�ű�id   	long-unsigned��
//		��ɺ�ű�id   	long-unsigned��
//		�������Ʊ�־  	structure��
//		{
//			�ϱ�͸������������ȴ���������  bool��
//			�ȴ��������ĳ�ʱʱ�䣨�룩      long-unsigned��
//			����ȶԱ�ʶ    enum{���ȶԣ�0�����ȣ�1�����ȶ��ϱ���2��}��
//			����ȶԲ���    structure
//			{
//				�����ֽ�      unsigned��
//				��ȡ��ʼ      long-unsigned��
//				��ȡ����      long-unsigned
//			}
//		}
//		�������ļ� 	 	array ��������
//		�������ġ�=structure
//		{
//			�������  unsigned��
//			��������  octet-string
//		}
//	}
//	�洢���     long-unsigned
//}
#define LENper_6018     8192
#define LENmax_6018     (LENper_6018*NUMmax_6018)
#define ADDR_601C     ADDR_6018+LENmax_6018//�ϱ�������(��ʼ��ֵ0x00)
//����2���ϱ����� report plan����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//�������          unsigned��
//�ϱ�ͨ��          array OAD��
//�ϱ���Ӧ��ʱʱ��  TI��
//����ϱ�����      unsigned��
//�ϱ�����          strcuture
//{
//  ����  unsigned��
//  ����  Data
//}
//}
//�ϱ����ݣ�
//�ϱ�����	  �ϱ�����	��ʾ
// 0	      OAD	������������
// 1	      RecordData	�ϱ���¼�Ͷ�������
//RecordData��=structure
//{
//  ����������������      OAD��
//	��¼�Ͷ�������������  RCSD��
//	��ѡ��                RSD
//}
#define LENper_601C     512
#define LENmax_601C     (LENper_601C*NUMmax_601C)
#define ADDR_601E     ADDR_601C+LENmax_601C//�ɼ������(��ʼ��ֵ0x00)
//����2��=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//	������ѡ��������  CSD��
//	��������  structrue
//	{
//		AcqCmd_2007  structure
//		{
//			����DI  array octet-string(SIZE(4))��
//			����DI  array octet-string(SIZE(4))
//		}��
//		AcqCmd_1997  structure
//		{
//			����DI  array octet-string(SIZE(2))��
//			����DI  array octet-string(SIZE(2))
//		}��
//		AcqCmd_Trans  structure
//		{
//			Frame  octet-string
//		}
//	}
//}
#define LENper_601E     256//160
#define NUMmax_601E     512
#define LENmax_601E     (LENper_601E*NUMmax_601E)


#define ADDR_8000_2     ADDR_601E+LENmax_601E//ң��-����2�����ò���)
//����2�����ò���)��=structure//;//ɾ��ʱstructure���͵�Ԫ=0
//{
//  �̵�����բ��������ֵ    double-long-unsigned����λ��A������-4����
//  ���������ޱ�����ʱʱ��  long-unsigned����λ�����ӣ�����0��
//}
#define LENmax_8000_2     10
#define ADDR_8000_127     ADDR_8000_2+LENmax_8000_2//ң��-����127�������澯��������
//����127�������澯��������������=NULL
//����128�����������������������=NULL
//����=0x55,���=0x0
#define LENmax_8000_127     1
#define ADDR_8000_129     ADDR_8000_127+LENmax_8000_127//ң��-����129����բ��������
//����129����բ��������
//������=array structure;//ɾ��ʱ������=0
//{
//�̵���      OAD����բʱ��ӦOADΪ0x00000000
//�澯��ʱ    unsigned����λ�����ӣ����㣺0����
//�޵�ʱ��    long-unsigned����λ�����ӣ����㣺0��ֵΪ0��ʾ�����޵磩��
//�Զ���բ    bool��True���Զ���բ��False�����Զ���բ��
//}
#define LENper_8000_129     14
#define LENmax_8000_129     (2+(LENper_8000_129*NUMmax_CR))
#define ADDR_8000_130     ADDR_8000_129+LENmax_8000_129//ң��-����130����բ��������
//����130����բ��������
//������=array structure;//ɾ��ʱ������=0
//{
//�̵���    OAD����բ��ӦOADΪ0x00000000
//����      enum{��բ����0����ֱ�Ӻ�բ��1��}
//����      visible-string
//}
#define LENmax_8000_130     (2+(32*NUMmax_CR))

#define ADDR_8001_127     ADDR_8000_130+LENmax_8000_130//����127��Ͷ�뱣��
//����127��Ͷ�뱣�磨������������=NULL
//����Ͷ�뱣��״̬������ֹһ��ִ����բ�ļ̵���������ƣ��һָ�����բ�ļ̵���������ơ�
//����128��������磨������������=NULL
//���ڽ������״̬��
//Ͷ��=0x55,���=0x0
#define LENmax_8001_127     1
#define ADDR_8001_3     ADDR_8001_127+LENmax_8001_127//����-��������վ�����ͨ��ʱ�������ӣ�
//����3��=long-unsigned
//��������վ�����ͨ��ʱ�������ӣ���0��ʾ���Զ����硣
#define LENmax_8001_3     3
#define ADDR_8001_4     ADDR_8001_3+LENmax_8001_3//����-�ϵ��Զ�����ʱ�������ӣ�
//����4��=long-unsigned
//�ϵ��Զ�����ʱ�������ӣ���0��ʾ�ϵ粻�Զ����硣
#define LENmax_8001_4     3
#define ADDR_8001_5     ADDR_8001_4+LENmax_8001_4//����-�Զ�����ʱ��
//����5��=array �Զ�����ʱ��;//ɾ��ʱ������=0
//�Զ�����ʱ�Ρ�=structure
//{
//��ʼʱ�䣨ʱ��  unsigned��
//����ʱ�䣨ʱ��  unsigned
//}
//ʱ����������Ϊǰ�պ󿪡�
#define LENmax_8001_5     (8*24)
#define ADDR_8002_127     ADDR_8001_5+LENmax_8001_5//����127���߷Ѹ澯Ͷ��
//����127���߷Ѹ澯Ͷ�루������
//������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//  �澯ʱ�Σ�octet-string(SIZE(3))��
//  �澯��Ϣ��visible-string(SIZE(200))
//}
//����128��ȡ���߷Ѹ澯��������������=NULL
#define LENmax_8002_127     256


#define ADDR_8100_2     ADDR_8002_127+LENmax_8002_127//�ն˱�����ֵ
//�ն˱�����ֵ��=long64����λ��W�����㣺-1��
#define LENmax_8100_2     9
#define ADDR_8101_2     ADDR_8100_2+LENmax_8100_2//�ն˹���ʱ��
//����2�����ò�������=array unsigned;//ɾ��ʱ������=0
//D7	D6	D5	D4	D3	D2	D1	D0		
//1:30-2:00	1:00-1:30	0:30-1:00	0:00-0:30	��l�ֽ�
//3:30-4:00	3:00-3:30	2:30-3:00	2:00-2:30	��2�ֽ�
//...	
//23:30-24:00	23:00-23:30	22:30-23:00	22:00-22:30	��12�ֽ�
#define LENmax_8101_2     (2+(2*12))
#define ADDR_8102_2     ADDR_8101_2+LENmax_8101_2//���ظ澯ʱ��
//����2�����ò�������=array unsigned
//�澯ʱ�䰴˳���ʾ1-n�ִεĹ��ظ澯ʱ�䣨��λ�����ӣ�
#define LENmax_8102_2     18
#define ADDR_8103_2     ADDR_8102_2+LENmax_8102_2//ʱ�ι���-���Ʒ�����(��ʼ��ֵ0x00)
//����2�����Ʒ���������=array ʱ�ι������õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure//
//{
//�ܼ������  OI��
//������ʶ    bit-string(SIZE(8))��
//��һ�׶�ֵ  PowerCtrlParam��
//�ڶ��׶�ֵ  PowerCtrlParam��
//�����׶�ֵ  PowerCtrlParam��
//ʱ�ι��ض�ֵ����ϵ��  integer����λ��%��
//}
//PowerCtrlParam��=structure
//{
//	ʱ�κ�          bit-string(SIZE(8))��
//	ʱ��1���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��2���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��3���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��4���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��5���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��6���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��7���ض�ֵ  long64����λ��W�����㣺-1����
//	ʱ��8���ض�ֵ  long64����λ��W�����㣺-1��
//}
#define LENper_8103_2     241//ʵ����
#define LENmax_8103_2     (2+( LENper_8103_2*NUMmax_TAG))
#if LENmax_8103_2>(LEN_128KDATABUFF/2)
	#error
#endif
#define ADDR_8103_127     ADDR_8103_2+LENmax_8103_2//ʱ�ι���-�����л�(��ʼ��ֵ0x00)
//array //ɾ��ʱ������=0
//ʱ�ι��ط����л���=structure
//�ܼ�������=OI
//���Ʒ�����=structure
//{
//ʱ�ι���Ͷ���ʶ    bit-string(SIZE(8))��
//ʱ�ι��ض�ֵ������  unsigned
//}
//ʱ�ι���Ͷ���ʶ��D0��D7��˳���λ��ʾ��1����8ʱ�Σ��á�1������Ч���á�0������Ч��
//ʱ�ι��ض�ֵ�����ţ���ֵ��Χ��0��2���α�ʾ��1����3�׷���������ֵ��Ч��
#define LENper_8103_127     12//ʵ����
#define LENmax_8103_127     (2+(LENper_8103_127*NUMmax_TAG))
#define ADDR_8104_2     ADDR_8103_127+LENmax_8103_127//���ݿ����õ�Ԫ
//����2�����Ʒ���������=array ���ݿ����õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure
//{
//�ܼ������    OI��
//���ݿض�ֵ    long64����λ��W�����㣺-1����
//�޵���ʼʱ��  date_time_s����=FFFFH����=FFH����=FFH����
//�޵�����ʱ��  long-unsigned����λ�����ӣ���
//ÿ���޵���    bit-string(SIZE(8)) 
//}
//ÿ���޵��գ�D1��D7��ʾ����һ�������գ�D0=0��
#define LENper_8104_2     28//ʵ����
#define LENmax_8104_2     (2+(LENper_8104_2*NUMmax_TAG))
#if LENmax_8104_2>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_8105_2     ADDR_8104_2+LENmax_8104_2//Ӫҵ��ͣ�����õ�Ԫ(��ʼ��ֵ0x00)
//����2�����Ʒ���������=array Ӫҵ��ͣ�����õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure
//{
//�ܼ������      OI��
//��ͣ��ʼʱ��    date_time_s��ʱ=FFH����=FFH����
//��ͣ����ʱ��    date_time_s��ʱ=FFH����=FFH����
//��ͣ�ع��ʶ�ֵ  long64����λ��W�����㣺-1��
//}
#define LENper_8105_2     30//ʵ����
#define LENmax_8105_2     (2+(LENper_8105_2*NUMmax_TAG))
#if LENmax_8105_2>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_8106_127     ADDR_8105_2+LENmax_8105_2//��ǰ�����¸���-����127 Ͷ�루�ܼ�����󣬿��Ʒ�����(��ʼ��ֵ0x00)
//array;//ɾ��ʱ������=0
//structure
//�ܼ�������=OI
//���Ʒ�����=structure
//{
//��ǰ�����¸��ض�ֵ����ʱ��    unsigned����λ�����ӣ���
//��ǰ�����¸��ض�ֵ����ϵ��    integer����λ��%����
//�غ��ܼ��й����ʶ�����ʱʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��صĿ���ʱ��      unsigned����λ��0.5Сʱ����
//��ǰ�����¸��ص�1�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�2�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�3�ָ澯ʱ��  unsigned����λ�����ӣ���
//��ǰ�����¸��ص�4�ָ澯ʱ��  unsigned����λ�����ӣ�
//}
#define LENper_8106_127     23//ʵ����
#define LENmax_8106_127     (2+(LENper_8106_127*NUMmax_TAG))
#if LENmax_8106_127>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_8107_2     ADDR_8106_127+LENmax_8106_127//��������õ�Ԫ
//����2�����Ʒ���������=array ��������õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure
//{
//	�ܼ������      OI��
//���絥��        double-long-unsigned��
//׷��/ˢ�±�ʶ   enum{׷�ӣ�0����ˢ�£�1��}��
//��������        enum{������0������ѣ�1��}��
//���������ѣ�ֵ  long64����λ��kWh/Ԫ�� ���㣺-4����
//	��������ֵ      long64����λ��kWh/Ԫ�����㣺-4����
//	��բ����ֵ      long64����λ��kWh/Ԫ�����㣺-4��
//	�����ģʽ      enum{����ģʽ��0����Զ��ģʽ��1��}
//}
#define LENper_8107_2     43//ʵ����
#define LENmax_8107_2     (2+(LENper_8107_2*NUMmax_TAG))
#if LENmax_8107_2>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_8108_2     ADDR_8107_2+LENmax_8107_2//�µ�����õ�Ԫ
//����2�����Ʒ���������=array �µ�����õ�Ԫ;//ɾ��ʱ������=0
//����2��=structure
//{
//	�ܼ������      OI��
//	�µ����ض�ֵ    long64����λ��kWh�����㣺-4����
//	��������ֵϵ��  unsigned����λ��%����
//	�µ����ض�ֵ����ϵ��  integer����λ��%��
//}
#define LENper_8108_2     18//ʵ����
#define LENmax_8108_2     (2+(LENper_8108_2*NUMmax_TAG))
#if LENmax_8108_2>(LEN_128KDATABUFF/2)
#error
#endif

#define ADDR_8103_3     ADDR_8108_2+LENmax_8108_2//ʱ�ι���-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬;//ɾ��ʱ������=0
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
#define LENper_8103_3     7//ʵ����
#define LENmax_8103_3     (2+(LENper_8103_3*NUMmax_TAG))
#define ADDR_8104_3     ADDR_8103_3+LENmax_8103_3//���ݿ�-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬;//ɾ��ʱ������=0
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
#define LENper_8104_3     7//ʵ����
#define LENmax_8104_3     (2+(LENper_8104_3*NUMmax_TAG))
#define ADDR_8105_3     ADDR_8104_3+LENmax_8104_3//Ӫҵ��ͣ��-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬;//ɾ��ʱ������=0
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
#define LENper_8105_3     7//ʵ����
#define LENmax_8105_3     (2+(LENper_8105_3*NUMmax_TAG))
#define ADDR_8106_3     ADDR_8105_3+LENmax_8105_3//��ǰ�����¸���-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬;//ɾ��ʱ������=0
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
#define LENper_8106_3     7//ʵ����
#define LENmax_8106_3     (2+(LENper_8106_3*NUMmax_TAG))
#define ADDR_8107_3     ADDR_8106_3+LENmax_8106_3//�����-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬;//ɾ��ʱ������=0
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
#define LENper_8107_3     7//ʵ����
#define LENmax_8107_3     (2+(LENper_8107_3*NUMmax_TAG))
#define ADDR_8108_3     ADDR_8107_3+LENmax_8107_3//�µ��-�ܼ������Ͷ��״̬(��ʼ��ֵ0x00)
//����Ͷ��״̬��=array һ���ܼ������Ͷ��״̬;//ɾ��ʱ������=0
//һ���ܼ������Ͷ��״̬��=structure
//{
//  �ܼ������  OI��
//Ͷ��״̬  enum{δͶ�루0����Ͷ�루1��}
//}
#define LENper_8108_3     7//ʵ����
#define LENmax_8108_3     (2+(LENper_8108_3*NUMmax_TAG))


#define ADDR_F203_4     ADDR_8108_3+LENmax_8108_3
//����4��=structure
//{
//�����������־bit-string(SIZE��8��)
//��
//bit0��bit7��˳���λ��ʾ��1��8·״̬�����룬�á�1�������룬�á�0����δ���롣
//����
//���������Ա�־bit-string(SIZE��8��)
//��
//  bit0��bit7��˳���λ��ʾ��1��8·״̬�����룬�á�1���������㡣�á�0�������մ��㡣
//��
//}
#define LENmax_F203_4     8
#define ADDR_F204_4     ADDR_F203_4+LENmax_F203_4
//����4��=array ֱ��ģ��������
//ֱ��ģ�������á�=structure
//{
//  ������ʼֵ  double-long��
//  ���̽���ֵ  double-long��
//  ���㼰��λ  Scaler_Unit
//}
#define NUMmax_F204_4     NUMmax_DC
#define LENmax_F204_4     (2+(15*NUMmax_F204_4))
#define ADDR_F205_127     ADDR_F204_4+LENmax_F204_4
//����127���޸Ŀ������ԣ��̵����ţ��������ԣ�
//��=structure
//{
//�̵����š�=OAD
//�������ԡ�=enum{����ʽ��0��������ʽ��1��}��
//}
#define LENper_F205_127     9
#define NUMmax_F205_127     NUMmax_CR
#define LENmax_F205_127     (LENper_F205_127*NUMmax_F205_127)
#define ADDR_F206_4     ADDR_F205_127+LENmax_F205_127//�澯���
//����4��=array ����澯ʱ��
//����澯ʱ�Ρ�=structure
//{
//  ��ʼʱ��  Time��
//  ����ʱ��  Time
//}
#define NUMmax_F206_4     12
#define LENmax_F206_4     (2+(10*NUMmax_F206_4))
#define ADDR_F207_127     ADDR_F206_4+LENmax_F206_4
//����127���޸Ĺ���ģʽ��·�ţ�����ģʽ��
//��=structure
//{
//·�š�=OAD
//���ӹ��ܡ�=enum
//{
//     �����������0����
//     ��������  ��1����
//     ʱ��Ͷ��  ��2��
//}
//}
#define LENper_F207_127     9
#define NUMmax_F207_127     1
#define LENmax_F207_127     (LENper_F207_127*NUMmax_F207_127)
#define ADDR_F209_6     ADDR_F207_127+LENmax_F207_127//�ز�/΢�������߽ӿ�
//����6���ӽڵ�����б�������ڣ���= TI
//�������ڣ�ָ�ӽڵ�����б�����������Ϣ��������Ϣ�ȸ�������
#define LENmax_F209_6     4
#define ADDR_F209_9     ADDR_F209_6+LENmax_F209_6//�ز�/΢�������߽ӿ�
//���� 9������ز�Ƶ����ţ��� =unsigned
#define LENmax_F209_9     2

#define ADDR_F209_11 			ADDR_F209_9+LENmax_F209_9
//���� 9��̨��ʶ����ͣ��ǣ��� =unsigned
#define LENmax_F209_11     2

#define ADDR_F209_22 			ADDR_F209_11+LENmax_F209_11
//���� 9���ܾ��ڵ��ϱ�ʹ�ܱ�־���� =enum
//enum{��ֹ��0����ʹ�ܣ�1��}
#define LENmax_F209_22     2


#define ADDR_F209_128     ADDR_F209_22+LENmax_F209_22//�ز�/΢�������߽ӿ�
//����128�����ö˿ڲ������˿ںţ�ͨ�Ų�����
//��=structure
//{
//�˿ںš�=OAD
//�˿ڲ���   COMDCB��
//}
#define LENper_F209_128     13
#define NUMmax_F209_128     1
#define LENmax_F209_128     (LENper_F209_128*NUMmax_F209_128)
#define ADDR_F20B_127     ADDR_F209_128+LENmax_F209_128//����ģ��
//����127�����ö˿ڣ��˿ںţ�ͨ�Ų�����
//��=structure
//{
//�˿ںš�=OAD
//�˿ڲ���   COMDCB��
//}
#define LENper_F20B_127     13
#define NUMmax_F20B_127     1
#define LENmax_F20B_127     (LENper_F20B_127*NUMmax_F20B_127)




#define ADDR_F001_4     ADDR_F20B_127+LENmax_F20B_127//�ļ������״̬��(���ڹ���ģʽʱ����0)
//����4�������״̬�֣�ֻ������=bit-string
//��bitλ��ʶÿ�����ݿ�Ĵ���״̬��bit0�����һ�����ݿ飬�������ơ�bitN=0����ʾδ���䣬bitN=1����ʾ����ɹ���N��0~�ܴ������-1����

#define LENmax_F001_4     256
#define ADDR_F001_7     ADDR_F001_4+LENmax_F001_4//�ļ��ֿ鴫�����(���ڹ���ģʽʱ����0)
//����7���������䣨������
//������=structure
//{
//	�ļ���Ϣ��=structure
//	{
//		Դ�ļ�    visible-string��
//		Ŀ���ļ�  visible-string��//Ŀ���ļ����ļ�·�������ƣ��ļ�·�������Ƹ�ʽ����[/·��/]�ļ����� ������豸��֧���ļ�����������Ϊ�մ���
//		�ļ���С  double-long-unsigned��
//		�ļ�����  bit-string(SIZE(3))��
							//bit0������1���ɶ���0�����ɶ���
							//bit1��д��1����д��0������д��
							//bit2��ִ�У�1����ִ�У�0������ִ�У�
//		�ļ��汾  visible-string
//		�ļ����  enum
//		{
//			�ն��ļ�          ��0����
//			����ͨ��ģ���ļ�  ��1����
//			Զ��ͨ��ģ���ļ�  ��2����
//			�ɼ����ļ�        ��3����
//			�ӽڵ�ͨ��ģ���ļ���4����
//			�����ļ�          (255)
//		}
//	}
//	������С  long-unsigned��
//	У���=structure
//	{
//		У������  enum
//		{
//    	CRCУ�飨Ĭ�ϣ�	��0����
//			md5У��			��1����
//			SHA1У��			��2����
//			����				��255��
//		}��
//  	У��ֵ  octet-string
//	}
//}
//�ļ���Ϣ���ݽṹ����173������2��
//У�����ݽṹ����174������4��
#define LENmax_F001_7     128
#define ADDR_F001_END     ADDR_F001_7+LENmax_F001_7


//�ܼ���
#define ADDR_230x_2     ADDR_F001_END
//�ܼ����ñ��=array �ܼ������õ�Ԫ//���ʱ������=0
//�ܼ����õ�Ԫ��=structure
//{
//�����ܼӵķ�·ͨ�ŵ�ַ  TSA��
//�ܼӱ�־	            enum{����0��������1��}��
//�������־	            enum{�ӣ�0��������1��}
//}
#define LENper_230x_2     (2+(15*8))
#define LENmax_230x_2     (LENper_230x_2*NUMmax_TAG)

#define ADDR_230x_13     ADDR_230x_2+LENmax_230x_2
//�ܼ��黬��ʱ������	unsigned����λ���֣�
#define LENper_230x_13     2
#define LENmax_230x_13     (LENper_230x_13*NUMmax_TAG)

#define ADDR_230x_14     ADDR_230x_13+LENmax_230x_13
//�ܼ��鹦���ִ�����
//�����ִα�ʶλ��=bit-string��SIZE(8)��
//�����ִα�ʶλ��bit0��bit7��˳���λ��ʾ��1����8�ִο��ص��ܿ����ã��á�1����ʾ���ִο����ܿأ��á�0����ʾ���ܿ�
#define LENper_230x_14     3
#define LENmax_230x_14     (LENper_230x_14*NUMmax_TAG)

#define ADDR_230x_15     ADDR_230x_14+LENmax_230x_14
//�ܼ������ִ�����
//����ִα�ʶλ��=bit-string��SIZE(8)��
//����ִα�ʶλ��bit0��bit7��˳���λ��ʾ��1����8�ִο��ص��ܿ����ã��á�1����ʾ���ִο����ܿأ��á�0����ʾ���ܿ�
#define LENper_230x_15     3
#define LENmax_230x_15     (LENper_230x_15*NUMmax_TAG)


//�������
#define ADDR_240x_2     ADDR_230x_15+LENmax_230x_15
//ͨ�ŵ�ַ��=octet-string
#define LENper_240x_2     18
#define LENmax_240x_2     (LENper_240x_2*NUMmax_IMPIN)
#define ADDR_240x_3     ADDR_240x_2+LENmax_240x_2
//���������ʡ�=structure
//{
//PT  long-unsigned��
//CT  long-unsigned
//}
#define LENper_240x_3     8
#define LENmax_240x_3     (LENper_240x_3*NUMmax_IMPIN)
#define ADDR_240x_4     ADDR_240x_3+LENmax_240x_3
//�������á�=array ���嵥Ԫ//ɾ��ʱ������=0
//���嵥Ԫ��=structure
//{
//��������˿ں�	OAD��
//��������		enum
//{
//	 �����й���0����
//	 �����޹���1����
//	 �����й���2����
//	 �����޹���3��
//}��
//���峣��k     long-unsigned
//}
#define LENper_240x_4     (2+12)
#define LENmax_240x_4     (LENper_240x_4*NUMmax_IMPIN)


//������չ
#define ADDR_4520_2     ADDR_240x_4+LENmax_240x_4//����Զ��ͨ�Ŷ����㱸��ͨ��
//����2��=array ����ͨ�� //������=0��ʾ��Ч
//����ͨ����=structure 
//{ 
//	��Ӫ�� enum
//	{ 
//		CMCC���ƶ��� = 0�� 
//		CTCC�����ţ� = 1�� 
//		CUCC����ͨ�� = 2�� 
//		δ֪ = 255 
//	}�� 
//	�������� enum 
//	 2G = 0��3G = 1�� 4G = 2�� δ֪ = 255 �� 
//	APN visible-string�� 
//	�û��� visible-string�� 
//	���� visible-string�� 
//	�����������ַ octet-string�� 
//	����˿� long-unsigned�� 
//	��վͨ�Ų��� array structure 
//	{ 
//		IP��ַ octet-string�� 
//		�˿� long-unsigned 
//	} 
//} 
#define LENmax_4520_2     1024
#define ADDR_3411     ADDR_4520_2+LENmax_4520_2//�����ߴ����¼�
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD
#define ADDR_3411_6     ADDR_3411+4+LENmax_EventOAD
//����6�����ò�������=structure
//{
//}
#define LENmax_3411_6     2

#define ADDR_8002_3     ADDR_3411_6+LENmax_3411_6//�߷Ѹ澯����
//����3���߷Ѹ澯��������=structure 
//{
//�澯ʱ�Σ�octet-string(SIZE(3))�� 
//�澯��Ϣ��visible-string(SIZE(1...200)) 
//} 
#define LENmax_8002_3     216


//---��������---
#if (USER/100)==14
#define ADDR_6E00     ADDR_8002_3+LENmax_8002_3//(����)�ⲿ����ָ���ǳ����ɺ��ն˱���������ͨ���˿ڽ���������
//����2���ⲿ�豸���ݲɼ�ģʽ����= enum
//{
//����Э�飨0����
//����������1��
//}
#define LENmax_6E00     2
#define ADDR_6E01     ADDR_6E00+LENmax_6E00//(����)����Ӧ��֧��ԭ�ն˵Ĳɼ�ģʽ��ԭ�ն�Ϊ376��Ӧ��֧�ֲ�����ģʽ��698.45Э��ģʽ��698.45���ն�Ӧ��֧��ԭ698.45Э�鷽ʽ�ɼ���
//����2�����ɺ��ն˱������ݲɼ�ģʽ����= enum
//{
//376ģʽ      ��0����
//698.45ģʽ   ��1��
//}
//֧������ģʽ�Ŀɽ����л���ֻ֧��һ�ֵĸò���ӦΪֻ��
#define ADDR_6E02     ADDR_6E01+2//(����)����оƬ����,ֻ��
//����2������оƬ����,ֻ������= enum
//{
//2013��  ��0����
//698.45�棨1��
//}
#define LENmax_6E01     2
#define ADDR_6E11     ADDR_6E02+LENmax_6E01//(����)ȫ��͸�����񷽰���
//����2�����ñ���=arrayȫ��͸�����񷽰���Ԫ(��Ӧ���񷽰������б����Ӧ�������²�����ʼ������ȣ�û�б���Ĳ�Ӧ����ִ������)
//6E12 ����2���������õ�Ԫ����=structure
//{ 
//	͸�����񷽰���  long-unsigned��
//	�˿�            OAD��
//	�������        structure
//	{
//    �������   unsigned
//		���Ӳ���   data
//	}��
//	ִ�����ȼ�      unsigned��
//	ִ�ж�ռ��      bool
//	{
//		����ռ��0������ռ��1��
//	}��
//	��ʼʱ��             date_time_s��
//	����ʱ��             date_time_s��
//	��������             TI��
//	ʧ����������         TI��
//	ÿ�ִ�ʧ�����Դ���   unsigned��
//	���ʧ�������ִ�     unsigned��
//	Э�������ʽ       enum
//	{
//	�����˵��
//	}��
//	��֡��������         bool
//	{��������0����������1��}��
//	ͨ�Ŷ�������          unsigned��
//	���ݴ洢����          long-unsigned��
//	�ϱ��������          unsigned��
//	���ݼӹ���ʽ         structure
//	{
//		�ӹ���ʽ           unsigned
//		���ݼӹ����Ӳ���   data
//	}��
//	����ʼǰ�ű�id    long-unsigned��
//	������ɺ�ű�id    long-unsigned
//}
//���ݼӹ���ʽ��ʽ��
//�ӹ���ʽ	�ӹ�����ʽ	��ʾ
//0	NULL	���ӹ�
//������ʽ����������չ
//����3������ͳ�Ʊ�ֻ������=array ����ͳ�Ƶ�Ԫ
//����ͳ�Ƶ�Ԫ����=structure
//{
//	͸�����񷽰���       long-unsigned��
//	�˿�                 OAD��
//	ִ�����ȼ�           unsigned��
//	ִ�ж�ռ��           bool��
//	�����þ������������� long-unsigned��
//	��С�����������     long-unsigned��
//	�������������     long-unsigned
//}
//����127��Add��array ͸�����񷽰���Ԫ����ӻ����һ��͸�����񷽰���Ԫ�����������б仯��Ӧ��������ݼ�¼�����¿�ʼ������ȡ�
//����128��Delete��array ͸�����񷽰��ţ������񷽰���ɾ��һ��͸�����񷽰���Ԫ�������������ü������ݼ�¼��
//����129��Clear (����) ������=NULL ���͸�����񷽰����ñ������������ü������ݼ�¼��
//����130��Update��array ������
//������=structure 
//{
//͸�����񷽰���  long-unsigned��
//ִ�����ȼ�      unsigned
//}
//���µĲ��Ը�������״̬��0ɾ���÷������������������ݣ�254Ϊ��ͣ����255Ϊ�����������񷽰����ȼ����䣬ִ�������ǰ���������¼�����¿�ʼִ�У��򴥷�һ�ζ��ᡣ
#define LENper_6E11     256
#define LENmax_6E11     (LENper_6E11*RMTASKmax)
#define ADDR_6E13     ADDR_6E11+LENmax_6E11//(����)ȫ��͸����������
//����2��=array ȫ��͸����������Ԫ
//6E14 ����2��͸����������Ԫ����=structure
//{
//	͸�����񷽰���    long-unsigned��
//	�����������      long-unsigned��
//	ͨ�ŵ�ַ����      MS��
//	�˿�ͨ�ſ��ƿ�    COMDCB��
//	�����ĸ�ʽ��    enum
//	{ 
//	�������¼
//	}��
//	����������      octet-string��
//	��Ӧ���ĸ�ʽ��    enum
//	{ 
//	�������¼
//	}��
//	��Ӧ����Ԥ�������     long-unsigned��
//	�ɹ���Ӧ���������Ч���� unsigned��
//	���ر�����֤��ʽ  structure
//	{
//		��֤��ʽ        unsigned��
//	��֤����        data��
//	�������񷽰���  long-unsigned��
//	}
//}
//���ر�����֤��ʽ��
//��֤����ʽ	��֤����	��ʾ
//0	NULL	����֤
//1	���ıȶԲ���	���ıȶԲ�һ����֤
//2	���ıȶԲ���	���ıȶԱȶ�һ����֤
//���ıȶԲ�����=structure
//{
//    �����ֽ�      octet-string��
//    ��ȡ��ʼ      long-unsigned��
//    ��ȡ����      long-unsigned
//}
//��ȡ��ʼ�ֽ��������ֽں�ʼ�㣬��СֵΪ1

//����127��Add��array ȫ��͸����������Ԫ��
//��Ӹ���һ����һ��͸����������, �����Ĳ�������ͨ�ŵ�ַ����MS�⣩�б����Ӧɾ��ԭ��ִ�м�¼����ִ�У�û�б���Ĳ�Ӧ�ظ�ִ�У������ӱ�Ƶ��±������Ӧ�Զ�����ִ�С�
//����128��Delete��array ������
//������=structure
//{
//  ͸�����񷽰���long-unsigned��
//  ͨ�ŵ�ַ����   MS
//}
//��͸�����񷽰��ź͵�ַɾ��һ��͸����������
//����129��Delete��array͸�����񷽰��ţ�
//��͸�����񷽰���ɾ��һ��͸����������
//����130��Clear(����)
//������=NULL
//���͸���������񼯡�
#define LENper_6E13     256
#define LENmax_6E13     (LENper_6E13*NUMmax_6014)
#define ADDR_6E17     ADDR_6E13+LENmax_6E13//(����)�ϱ�������
//����2��=array �ϱ�������Ԫ
//6E18 ����2���ϱ����� report plan����=structure
//{
//�ϱ��������      unsigned��
//�ϱ�ͨ��          array OAD��
//�ϱ����ȼ�        unsigned��
//�ϱ���Ӧ��ʱʱ��  TI��
//����ϱ�����      unsigned��
//�����֤��ʶ      enum
//{
//ֱ���ϱ���0������֤ͨ���ϱ���1������֤��ͨ���ϱ���2��
//}��
//�ϱ�ʱ��       unsigned��   
//{
//N�������ݺϲ��ϱ���N����
//�ȴ���վ�������255��
//}��
//�ϱ�����          strcuture
//{
//  ����  unsigned��
//  ����  Data
//}
//}
//�ϱ����ݣ�
//�ϱ�����	�ϱ�����	��ʾ
//0	OAD	������������
//1	RecordData	�ϱ���¼�Ͷ�������
//RecordData��=structure
//{
//  ����������������      OAD��
//��¼�Ͷ�������������  RCSD��
//��ѡ��                RSD
//}
//����127��Add��array �ϱ�������Ԫ��
//��ӻ����һ���ϱ�������Ԫ��
//����128��Delete��array �ϱ�������ţ�
//ɾ��һ���ϱ�������Ԫ��
//����129��Clear(����)
//������=NULL
//����ϱ���������
#define LENper_6E17     512
#define LENmax_6E17     (LENper_6E17*NUMmax_601C)
#define ADDR_6E30     ADDR_6E17+LENmax_6E17//(����)��վ��͸�������ȼ�
//����2����վ��͸�������ȼ�����=array��͸�������ȼ�
//��͸�������ȼ���=structure
//{ 
//�˿�            OAD�� 
//��͸���ȼ�      unsigned��
//}
#define LENmax_6E30     64
#define ADDR_6E33     ADDR_6E30+LENmax_6E30//(����)�����ϱ��¼��ϱ�����
//����2���¼��ϱ����� report plan����=structure
//{
//�ϱ�ͨ��          array OAD��
//�ϱ����ȼ�        unsigned��
//�ϱ���Ӧ��ʱʱ��  TI��
//����ϱ�����      unsigned
//}
#define LENmax_6E33     32

#define ADDR_DL698_SET_END     ADDR_6E33+LENmax_6E33
#endif//#if (USER/100)==14
//---�������� END---

//---��������---
#if (USER/100)==15//����
	#define ADDR_3106_11     ADDR_8002_3+LENmax_8002_3
	#define LENmax_3106_11		4
	#define ADDR_DL698_SET_END     ADDR_3106_11+LENmax_3106_11
#endif
//---�������� END---

//------------
	#define ADDR_4048_127		 ADDR_8002_3+LENmax_8002_3
	#define LENmax_4048_127		8
	#define ADDR_DL698_SET_END     ADDR_4048_127+LENmax_4048_127
//END---

#ifndef ADDR_DL698_SET_END
#define ADDR_DL698_SET_END     ADDR_8002_3+LENmax_8002_3
#endif


#if ((ADDR_DL698_SET_END)-(ADDR_DL698_SET_START))>LEN_FLASH_DL698_SET
	#error LEN_FLASH_DL698_SET
#endif




#ifdef __cplusplus
 }
#endif
#endif










