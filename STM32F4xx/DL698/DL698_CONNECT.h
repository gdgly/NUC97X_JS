

#ifndef Terminal698_CONNECT_H
#define Terminal698_CONNECT_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
void CONNECT_Request(u32 PORTn);
void RELEASE_Request(u32 PORTn);
void RELEASE_Notification(u32 PORTn);
u32 GET_OAD_ConnectApp(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE);//��Ӧ�����ӿɷ��ʶ����б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR


//Э��һ���Կ�
#define Protocol0     1//Ӧ������Э��Application Association			��0����
#define Protocol1     1//	�����������Get Normal						��1����
#define Protocol2     1//	�������������������Get With List			��2����
#define Protocol3     1//	�����¼�Ͷ�������Get Record				��3����
#define Protocol4     1//	���������������Get Proxy					��4����
#define Protocol5     1//	���������¼�Ͷ�������Get Proxy Record		��5����
#define Protocol6     1//	�����֡����֡Get Subsequent Frame			��6����
#define Protocol7     1//	���û�����������Set Normal					��7����
#define Protocol8     1//	�������û�����������Set With List			��8����
#define Protocol9     1//	���ú��ȡSet With Get						��9����
#define Protocol10     1//	�������ö�������Set Proxy					��10����
#define Protocol11     1//	�������ú��ȡ��������Set Proxy With Get		��11����
#define Protocol12     1//	ִ�ж��󷽷�Action Normal					��12����
#define Protocol13     1//	����ִ�ж��󷽷�Action With List				��13����
#define Protocol14     1//	ִ�з������ȡAction With List				��14����
#define Protocol15     1//	����ִ�ж��󷽷�Action Proxy				��15����
#define Protocol16     1//	����ִ�к��ȡAction Proxy With Get			��16����
#define Protocol17     1//	�¼������ϱ�Active Event Report				��17����
#define Protocol18     1//     �¼�β���ϱ�                              	��18)��
#define Protocol19     1//�¼��������λACD�ϱ�                    	��19)��
#define Protocol20     1//     ��֡���ݴ��� Slicing Service              	��20����
#define Protocol21     1//     Get-request��֡                           	��21����
#define Protocol22     1//Get-response��֡                          	��22����
#define Protocol23     1//     Set-request��֡                         		��23����
#define Protocol24     1//Set-response��֡                        		��24����
#define Protocol25     1//     Action-request��֡                      		��25����
#define Protocol26     1//Action-response��֡                     		��26����
#define Protocol27     1//Proxy-request ��֡                       		��27����
#define Protocol28     1//Proxy-response��֡                      		��28����
#define Protocol29     1//�¼��ϱ���֡                            		��29����
#define Protocol30     1//DL/T645-2007                            		��30����
#define Protocol31     1//��ȫ��ʽ����                            		��31����
#define Protocol32     0//��������IDΪ0�Ķ�ȡ����                     ��32����
#define Protocol33     0//��������IDΪ0�����÷���                     ��33��
#define Protocol34     0
#define Protocol35     0
#define Protocol36     0
#define Protocol37     0
#define Protocol38     0
#define Protocol39     0


//����һ���Կ�
#define Function0     1//	����������							��0����
#define Function1     1//	˫���й�����							��1����
#define Function2     1//	�޹����ܼ���							��2����
#define Function3     1//	���ڵ��ܼ���							��3����
#define Function4     1//	�й�����						    		��4����
#define Function5     1//	�޹�����						    		��5����
#define Function6     1//	��������						    		��6����
#define Function7     1//	������					     		��7����
#define Function8     1//	����									��8����
#define Function9     1//	���طѿ�								��9����
#define Function10     1//	Զ�̷ѿ�								��10����
#define Function11     1//	��������							��11����
#define Function12     1//	г������								��12����
#define Function13     1//	г������								��13����
#define Function14     1//	����ʧ���							��14����
#define Function15     1//	�๦�ܶ������						��15����
#define Function16     1//	�¼���¼						    		��16����
#define Function17     1//	�¼��ϱ�								��17����
#define Function18     1//	�¶Ȳ���								��18����
#define Function19     1//	״̬����⣨�磺�����/����ť�ǣ�		��19����
#define Function20     1//	��̫��ͨ��							��20����
#define Function21     1//	����ͨ��								��21����
#define Function22     0//	����ͨ��								��22����
#define Function23     1//	��ý��ɼ�							��23����
#define Function24     0//	����									��24����
#define Function25     1//	ֱ��ģ����							��25����
#define Function26     1//	�������12V���						��26����
#define Function27     1//	�ѱ�									��27����
#define Function28     0//	���ฺ��ƽ��							��28����
#define Function29     1//	����									��29����
#define Function30     0//	�ȶ�									��30��
#define Function31     0
#define Function32     0
#define Function33     0
#define Function34     0
#define Function35     0
#define Function36     0
#define Function37     0
#define Function38     0
#define Function39     0





#ifdef __cplusplus
 }
#endif
#endif

