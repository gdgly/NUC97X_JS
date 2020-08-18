
//�˵�1 ʵʱ����
#include "Project698.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "../Display/Draw.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"

void Menu_1_5_1(void);

const u8 QUnitList[][4]=//������λ�б�
{
	"kWh",
	"MWh",
};
extern const u8 UnitList[][6];//ͨ�õ�λ�б�
extern const u8 QCUnitList[][7];//����(��)��λ�б�
extern const u8 PurchaseWayList[][5];//���緽ʽ�б�
extern const u8 SList[][2];//���������б�

const u8 SwitchStateList[][3]=//����״̬�б�
{
	"��",
	"��",
};

const u8 ShiftList[][7]=//��λ��Ϣ�б�
{
	"����λ",
	"��λ",
};

const u8 SwitchNatureList[][5]=//���������б�
{
	"����",
	"����",
};

const u8 YCPList[][6]=//���߱����б�
{
	"10:1",//10�������1kW
	"10:2",//10�������2kW
	"10:3",
	"10:4",
	"10:5",
	"10:6",
	"10:7",
	"10:8",
	"10:9",
	"10:10",
	"10:11",
	"10:12",
	"10:13",
	"10:14",
	"10:15",
	"10:16",
};
const u8 pMenu_1[]=//ר��
{
	"ʵʱ����\n\r"
	"  1.��ǰ����\n\r"
	"  2.��ǰ����\n\r"
//	"  3.��������\n\r"
	"  3.����״̬\n\r"
//	"  5.���ؼ�¼\n\r"
//	"  6.��ؼ�¼\n\r"
//	"  7.ң�ؼ�¼\n\r"
//	"  8.ʧ���¼\n\r"
//	"  9.����������Ϣ\n\r"
};

const u8 pMenu4_1[]=//����
{
	"ʵʱ����\n\r"
	"  1.��ǰ����\n\r"
	"  2.��������\n\r"
	"  3.����״̬\n\r"
	"  4.�¼���¼\n\r"
};

const u8 pMenu_1_1[]=
{
	"��ǰ����\n\r"
	"  1.�����幦��\n\r"
	"  2.���ܼ��鹦��\n\r"
};

const u8 pMenu_1_1_1[]=
{
	"��ǰ����\n\r"
	"����\n\r"
};

const u8 PulsePowerList[][8]=//���幦���б�
{
	"��ַ: ",
	"�й���:",
	"",
	"�޹���:",
	"",
};

const u8 NPulseList[][20]=
{
	" �������δ����",
};

const u8 pMenu_1_1_2[]=
{
	"��ǰ����\n\r"
	"�ܼ���\n\r"
};

const u8 AddPowerList[][20]=//�ܼ��鹦���б�
{
	"�й�:",
	"",
	"�޹�:",
	"",
};

const u8 pMenu_1_2[]=
{
	"��ǰ����\n\r"
	"  1.�����й�����\n\r"
	"  2.�����޹�����\n\r"
	"  3.�����й�����\n\r"
	"  4.�����޹�����\n\r"
};


const u8 EQList[][20]=//�����б�
{
	"��:",
	"��:",
	"��:",
	"ƽ:",
	"��:",
};

const u8 pMenu_1_2_1[]=
{
	"�����й�����\n\r"
	"�ܼ���\n\r"
};

const u8 pMenu_1_2_2[]=
{
	"�����޹�����\n\r"
	"�ܼ���\n\r"
};


const u8 pMenu_1_2_3[]=
{
	"�����й�����\n\r"
	"�ܼ���\n\r"
};

const u8 pMenu_1_2_4[]=
{
	"�����޹�����\n\r"
	"�ܼ���\n\r"
};

const u8 pMenu_1_3[]=
{
	"��������\n\r"
	"  1.�й���������\n\r"
	"  2.�޹���������\n\r"
};

const u8 pMenu_1_3_1[]=
{
	"�й���������\n\r"
	"������\n\r"
};

const u8 pMenu_1_3_2[]=
{
	"�޹���������\n\r"
	"������\n\r"
};

const u8 pMenu_1_4[]=
{
	"����״̬\n\r"
	"  1.����״̬\n\r"
	"  2.��λ��Ϣ\n\r"
	"  3.��������\n\r"
};

const u8 pMenu_1_4_1[]=
{
	" \n\r"
	"����״̬:\n\r"
};

const u8 pMenu_1_4_2[]=
{
	" \n\r"
	"��λ��Ϣ:\n\r"
};

const u8 pMenu_1_4_3[]=
{
	" \n\r"
	"��������:\n\r"
};

const u8 PowerControlList[][20]=//��������б�
{
	"ʱ�ο�",
	"���ݿ�",
	"Ӫҵ��ͣ��",
	"�����¸���",
};

const u8 EControlList[][20]=//�������б�
{
	"�µ��",
	"�����",
};


#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
const u8 ERC5List[][20]=//ң�ؼ�¼�б�
{
	"��բʱ��:",//0
	"",			//1
	"��բ�ִ�:",//2
	"��ʱ����:",//3
	"������:",//4
};
const u8 ERC6List[][20]=//���ؼ�¼�б�
{
	"��բʱ��:",//0
	"",			//1
	"�ܼ����:",//2
	"��բ�ִ�:",//3
	"�������:",//4
	"��ǰ����:",//5
	"������:",//6
	"��ʱ��ֵ:",//7
};
const u8 ERC7List[][20]=//��ؼ�¼�б�
{
	"��բʱ��:",//0
	"",			//1
	"�ܼ����:",//2
	"��բ�ִ�:",//3
	"������:",//4
	"��ʱ����:",//5
	"��ʱ��ֵ:",//6
};
const u8 ERC14List[][20]=//ʧ���¼�б�
{
	"ʧ��ʱ��:",//0
	"",			//1
	"�ָ�ʱ��:",//2
};
	#else
const u8 EventList[][20]=
{
	"ERC:",
	"��Ҫ�¼�:",
	"һ���¼�:",
};

const u8 LogoutList[][21]=//�¼���¼�б�
{
	"���ݳ�ʼ���Ͱ汾���", //ERC1
	"������ʧ",             //ERC2
	"�������",             //ERC3
	"״̬����λ",           //ERC4
	"ң����բ",             //ERC5
	"������բ",             //ERC6
	"�����բ",             //ERC7
	"���ܱ�������",       //ERC8
	"������·�쳣",         //ERC9
	"��ѹ��·�쳣",         //ERC10
	"�����쳣",             //ERC11
	"���ܱ�ʱ�䳬��",       //ERC12
	"��������Ϣ",         //ERC13
	"�ն�ͣ/�ϵ�",          //ERC14
	"г��Խ�޸澯",         //ERC15
	"ֱ��ģ����Խ��",       //ERC16
	"��ѹ������ƽ���Խ��", //ERC17
	"������Ͷ������",       //ERC18
	"�����������",         //ERC19
	"��Ϣ��֤����",         //ERC20
	"�ն˹���",             //ERC21
	"�й��ܵ������Խ��", //ERC22
	"��ظ澯",             //ERC23
	"��ѹԽ��",             //ERC24
	"����Խ��",             //ERC25
	"���ڹ���Խ��",         //ERC26
	"���ܱ�ʾ���½�",       //ERC27
	"����������",           //ERC28
	"���ܱ����",           //ERC29
	"���ܱ�ͣ��",           //ERC30
	"�ն�485����ʧ��",      //ERC31
	"ͨ������������",       //ERC32
	"���ܱ�����״̬�ֱ�λ", //ERC33
	"CT�쳣",               //ERC34
	"����δ֪���",         //ERC35
	"���ƻ�·״̬����λ",   //ERC36
	"���ܱ�����¼�",     //ERC37
	"���ܱ���ť���¼�",   //ERC38
	"����ʧ���¼�",         //ERC39
//	"�ӽڵ������ϱ��¼�",   //ERC40
	"�ų��쳣�¼�",         //ERC40
	"��ʱ�¼�",             //ERC41
};
const u8 ERC1[]=
{
	"���ݳ�ʼ���Ͱ汾���\n\r"
	"  ERC1��� ? ��\n\r"
	"���ʱ��:\n\r"
	"\n\r"
	"�¼���־:\n\r"
	"\n\r"
	"���ǰ����汾:\n\r"
	"���������汾:\n\r"
};

const u8 ERC2[]=
{
	"������ʧ\n\r"
	"  ERC2��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"�¼���־:\n\r"
};

const u8 ERC3[]=
{
	"�������\n\r"
	"  ERC3��� ? ��\n\r"
	"���ʱ��:\n\r"
	"\n\r"
	"����վ��ַ:\n\r"
	"���ݱ�ʶ:\n\r"
};

const u8 ERC4[]=
{
	"״̬����λ\n\r"
	"  ERC4��� ? ��\n\r"
	"��λʱ��:\n\r"
	"\n\r"
	"״̬��λ:\n\r"
	"���״̬:\n\r"
};

const u8 ERC5[]=
{
	"ң����բ\n\r"
	"  ERC5��� ? ��\n\r"
	"��բʱ��:\n\r"
	"\n\r"
	"��բ�ִ�:\n\r"
	"��ʱ����:\n\r"
	"������:\n\r"
};

const u8 ERC6[]=
{
	"������բ\n\r"
	"  ERC6��� ? ��\n\r"
	"��բʱ��:\n\r"
	"\n\r"
	"�ܼ����:\n\r"
	"��բ�ִ�:\n\r"
	"�������:\n\r"
	"��ǰ����:\n\r"
	"������:\n\r"
	"��ʱ��ֵ:\n\r"
};

const u8 ERC7[]=
{
	"�����բ\n\r"
	"  ERC7��� ? ��\n\r"
	"��բʱ��:\n\r"
	"\n\r"
	"�ܼ����:\n\r"
	"��բ�ִ�:\n\r"
	"������:\n\r"
	"��ʱ����:\n\r"
	"��ʱ��ֵ:\n\r"
};

const u8 ERC8[]=
{
	"���ܱ�������\n\r"
	"  ERC8��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"�������:\n\r"
	"�����־: ����ʱ�Σ�\n\r"
	"�䡢���ʱ�䣿�䡢��\n\r"
	"���գ��䡢���峣����\n\r"
	"�䡢���������ʣ��䡢\n\r"
	"���ܱ��������������\n\r"
};

const u8 IUnusualList[][21]=//������·�쳣�б�
{
	"��·",
	"��·",
	"����",
};

const u8 ERC9[]=
{
	"������·�쳣\n\r"
	"  ERC9��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"�쳣��־: ---\n\r"
	"Ua,Ia:\n\r"
	"Ub,Ib:\n\r"
	"Uc,Ic:\n\r"
	"��   :\n\r"//����ʱ��������й��ܵ���ʾֵ
};

const u8 ERC10[]=
{
	"��ѹ��·�쳣\n\r"
	" ERC10��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"�쳣��־: ---\n\r"
	"Ua,Ia:\n\r"
	"Ub,Ib:\n\r"
	"Uc,Ic:\n\r"
	"��   :\n\r"//����ʱ��������й��ܵ���ʾֵ
};

const u8 ERC11[]=
{
	"�����쳣\n\r"
	" ERC11��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"��U,Ia:\n\r"
	"��U,Ib:\n\r"
	"��U,Ic:\n\r"
	"��:\n\r"//����ʱ��������й��ܵ���ʾֵ
};

const u8 ERC12[]=
{
	"���ܱ�ʱ�䳬��\n\r"
	" ERC12��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
};

const u8 ERC13[]=
{
	"��������Ϣ\n\r"
	" ERC13��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"�쳣��־: ��̴�����\n\r"
	"������������������\n\r"
	"������������䡢ʧѹ\n\r"
	"�������䡢ͣ�������\n\r"
	"�䡢��أ�Ƿѹ\n\r"
};

const u8 ERC14[]=
{
	"ʧ���¼\n\r"
	" ERC14��� ? ��\n\r"
	"ʧ��ʱ��:\n\r"
	"\n\r"
	"�ָ�ʱ��:\n\r"
	"\n\r"
};

const u8 ERC15[]=
{
	"г��Խ�޸澯\n\r"
	" ERC15��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"�쳣��־:     ---\n\r"
};

const u8 ERC16[]=
{
	"ֱ��ģ����Խ��\n\r"
	" ERC16��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"Խ�ޱ�־:\n\r"
	"\n\r"
	"Խ��ʱֱ��ģ��������\n\r"
};

const u8 ERC17_1[]=
{
	"��ѹ/����ƽ��Խ��\n\r"
	" ERC17��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"�쳣��־:\n\r"
	"\n\r"
	"\n\r"
	"����ʱ��ѹ��ƽ���:\n\r"
	"\n\r"
};

const u8 ERC17_2[]=
{
	"��ѹ/����ƽ��Խ��\n\r"
	" ERC17��� ? ��\n\r"
	"����ʱ������ƽ���:\n\r"
	"\n\r"
	"Ua,Ia:\n\r"
	"Ub,Ib:\n\r"
	"Uc,Ic:\n\r"
};

const u8 ERC18[]=
{
	"������Ͷ������\n\r"
	" ERC18��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"�쳣��־:\n\r"
};

const u8 ERC19_1[]=
{
	"�����������\n\r"
	" ERC19��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"�ܼ����:\n\r"
	"���絥��:\n\r"
	"׷��ˢ��:\n\r"
	"������ֵ:\n\r"
	"\n\r"
};

const u8 ERC19_2[]=
{
	"�����������\n\r"
	" ERC19��� ? ��\n\r"
	"��������:\n\r"
	"\n\r"
	"��բ����:\n\r"
	"\n\r"
	"����ǰʣ�����(��):\n\r"
	"\n\r"
	"�����ʣ�����(��):\n\r"
};

const u8 ERC20[]=
{
	"��Ϣ��֤����\n\r"
	" ERC20��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"PW:\n\r"
	"\n\r"
	"MSA:\n\r"
};

const u8 TerminalBugList[][21]=//�ն˹����б�
{
	"�ն������ڴ�",//����1
	"ʱ��",        //����2
	"����ͨ��",    //����3
	"485����",     //����4
	"��ʾ��",      //����5
	"�ز�ͨ���쳣",//����6
};

const u8 ERC21[]=
{
	"�ն˹���\n\r"
	" ERC21��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"����:\n\r"
};

const u8 ERC22[]=
{
	"�й��ܵ������Խ��\n\r"
	" ERC22��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"Խ��ʱ�ܼ��й�������\n\r"
	"    �Ա�:\n\r"
	"    ����:\n\r"
	"���ƫ��:\n\r"
	"����ƫ��:\n\r"
};

const u8 ERC23[]=
{
	"��ظ澯\n\r"
	" ERC23��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"�ܼ����:\n\r"
	"Ͷ���ִ�:\n\r"
	"������:\n\r"
	"�澯����:\n\r"
	"�澯��ֵ:\n\r"
};

const u8 ERC24[]=
{
	"��ѹԽ��\n\r"
	" ERC24��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"Խ�ޱ�־: ---\n\r"
	"\n\r"
	"����ʱUa:\n\r"
	"����ʱUb:\n\r"
	"����ʱUc:\n\r"
};

const u8 ERC25[]=
{
	"����Խ��\n\r"
	" ERC25��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"Խ�ޱ�־: ---\n\r"
	"\n\r"
	"����ʱIa:\n\r"
	"����ʱIb:\n\r"
	"����ʱIc:\n\r"
};

const u8 ERC26[]=
{
	"���ڹ���Խ��\n\r"
	" ERC26��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"Խ�ޱ�־:\n\r"
	"����ʱ����:\n\r"
	"����ʱ��ֵ:\n\r"
};

const u8 ERC27[]=
{
	"���ܱ�ʾ���½�\n\r"
	" ERC27��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"��ǰ���й��ܵ���ʾֵ\n\r"
	"\n\r"
	"�������й��ܵ���ʾֵ\n\r"
};

const u8 ERC28[]=
{
	"����������\n\r"
	" ERC28��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"����ǰ���й���ʾֵ\n\r"
	"\n\r"
	"��������й���ʾֵ\n\r"
	"\n\r"
	"������ֵ:\n\r"
};

const u8 ERC29[]=
{
	"���ܱ����\n\r"
	" ERC29��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"��ǰ���й��ܵ���ʾֵ\n\r"
	"\n\r"
	"�ɺ����й��ܵ���ʾֵ\n\r"
	"\n\r"
	"������ֵ:\n\r"
};

const u8 ERC30[]=
{
	"���ܱ�ͣ��\n\r"
	" ERC30��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"ͣʱ���й��ܵ���ʾֵ\n\r"
	"\n\r"
	"ͣ����ֵ:\n\r"
};

const u8 ERC31[]=
{
	"�ն�485����ʧ��\n\r"
	" ERC31��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"���һ�γ���ɹ�ʱ��\n\r"
	"\n\r"
	"�����ܵ���ʾֵ:\n\r"
	"  �й�:\n\r"
	"  �޹�:\n\r"
};

const u8 ERC32[]=
{
	"ͨ������������\n\r"
	" ERC32��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ͨ������:\n\r"
	"\n\r"
	"����������:\n\r"
	"\n\r"
};

const u8 ERC33[]=
{
	"���ܱ�����״̬�ֱ�λ\n\r"
	" ERC33��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"�������:\n\r"
};

const u8 ERC34List[][21]=//ERC33ͨ�������������б�
{
	"һ�β��·",
	"���β��·", 
	"���β��·",
};

const u8 ERC34[]=
{
	"CT�쳣\n\r"
	" ERC34��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"�쳣��־:\n\r"
};

const u8 ERC35[]=
{
	"����δ֪���\n\r"
	" ERC35��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"ͨ�Ŷ˿�:\n\r"
	"���ֿ���:\n\r"
};

const u8 ERC36[]=
{
	"���ƻ�·״̬����λ\n\r"
	" ERC36��� ? ��\n\r"
	"��λʱ��:\n\r"
	"\n\r"
	"״̬��λ:\n\r"
	"���״̬:\n\r"
};

const u8 ERC37[]=
{
	"���ܱ�����¼�\n\r"
	" ERC37��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"�������:\n\r"
	"�����ܴ���:\n\r"
};

const u8 ERC38[]=
{
	"���ܱ���ť���¼�\n\r"
	" ERC38��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"�������:\n\r"
	"�����ܴ���:\n\r"
};

const u8 ERC39[]=
{
	"����ʧ���¼�\n\r"
	" ERC39��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"���һ�γ���ɹ�ʱ��\n\r"
	"\n\r"
	"�����ܵ���ʾֵ:\n\r"
	"  �й�:\n\r"
	"  �޹�:\n\r"
};

//const u8 ERC40[]=
//{
//	"�ӽڵ������ϱ��¼�\n\r"
//	" ERC40��� ? ��\n\r"
//	"����ʱ��:\n\r"
//	"\n\r"
//	"�¼��ӽڵ��ַ:\n\r"
//	"\n\r"
//	"�ӽڵ�����:\n\r"
//};

//const u8 NodeTypeList[][15]=//�ӽڵ������б�
//{
//	"�ն���Ӵ�����",
//	"�ɼ���",
//	"���ܱ�",
//};

const u8 ERC40[]=
{
	"�ų��쳣�¼�\n\r"
	" ERC40��� ? ��\n\r"
	"����ʱ��:\n\r"
	"\n\r"
	"��ֹ��Pn: ������?\n\r"
	"�豸����:\n\r"
	"\n\r"
	"�豸��ַ:\n\r"
	"\n\r"
	"�쳣����:\n\r"
};

const u8 DeviceTypeList[][15]=//�豸�����б�
{
	"�ն���Ӵ�����",//00H
	"�ɼ���",        //01H
	"���ܱ�",        //02H
	"����",          //03H-FFH
};

const u8 AbnormalTypeList[][9]=//�ų��쳣�����б�
{
	"��Ƶ�ų�",//0
	"��Ƶ�ų�",//1
	"ǿ�ų�",  //2
};


const u8 ERC41[]=
{
	"��ʱ�¼�\n\r"
	" ERC41��� ? ��\n\r"
	"�������:\n\r"
	"��ǰʱ��:\n\r"
	"\n\r"
	"�Ժ�ʱ��:\n\r"
	"\n\r"
};
	#endif
#if ((Project/100)==3)||((Project/100)==4)//ר���ն�

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_4
// Descriptions:        �����޹�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_2_4(void)//�˵�1_2_6
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
 	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_4);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			y=MRR(ADDR_230x_2+LENper_230x_2*(MenuCount->v2-1),2);
			if(((y&0xff)==DataType_array)&&((y>>8)>0))
				MenuCount->v3=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUMmax_TAG);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	
	DisplayClrRow(Ln+2,Ln+6);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//���������޹����� �ܼ��ƽ��
		for(i=0;i<=4;i++)
		{
			y=MRR(ADDR_230x_10+LENper_230x_10*(MenuCount->v2-1)+2+i*9,1);
			if(y!=DataType_long64)
			{//�޴�������---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				y=MRR(ADDR_230x_10+LENper_230x_10*(MenuCount->v2-1)+2+i*9+1,8);
				y=R_Inverse(y,8);
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+3*2);
				disp_long64_kw(y,0,1,p16,3);
				//DisplayData_OffZero(y,8,4,p16);
			}
			//DisplayString(Ln+2+i,14,0,(u8 *)UnitList[3]);//3 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�ܼ���");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_3
// Descriptions:        �����й�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_2_3(void)//�˵�1_2_5
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_3);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			y=MRR(ADDR_230x_2+LENper_230x_2*(MenuCount->v2-1),2);
			if(((y&0xff)==DataType_array)&&((y>>8)>0))
				MenuCount->v3=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUMmax_TAG);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}

	DisplayClrRow(Ln+2,Ln+6);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//���������й����� �ܼ��ƽ��
		for(i=0;i<=4;i++)
		{
			y=MRR(ADDR_230x_9+LENper_230x_9*(MenuCount->v2-1)+2+i*9,1);
			if(y!=DataType_long64)
			{//�޴�������---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				y=MRR(ADDR_230x_9+LENper_230x_9*(MenuCount->v2-1)+2+i*9+1,8);
				y=R_Inverse(y,8);
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+3*2);
				disp_long64_kw(y,0,1,p16,2);
				//DisplayData_OffZero(y,8,4,p16);
			}
			//DisplayString(Ln+2+i,14,0,(u8 *)UnitList[2]);//2 kWh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�ܼ���");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_2
// Descriptions:        �����޹�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_2_2(void)//�˵�1_2_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_2);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			y=MRR(ADDR_230x_2+LENper_230x_2*(MenuCount->v2-1),2);
			if(((y&0xff)==DataType_array)&&((y>>8)>0))
				MenuCount->v3=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUMmax_TAG);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	
	DisplayClrRow(Ln+2,Ln+6);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		//���������޹����� �ܼ��ƽ��
		for(i=0;i<=4;i++)
		{
			y=MRR(ADDR_230x_8+LENper_230x_8*(MenuCount->v2-1)+2+i*9,1);
			if(y!=DataType_long64)
			{//�޴�������---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				y=MRR(ADDR_230x_8+LENper_230x_8*(MenuCount->v2-1)+2+i*9+1,8);
				y=R_Inverse(y,8);
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+3*2);
				disp_long64_kw(y,0,1,p16,3);
				//DisplayData_OffZero(y,8,4,p16);
			}
			//DisplayString(Ln+2+i,14,0,(u8 *)UnitList[3]);//3 kvarh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�ܼ���");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2_1
// Descriptions:        �����й�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_2_1(void)//�˵�1_2_1
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u64 y;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2_1);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			y=MRR(ADDR_230x_2+LENper_230x_2*(MenuCount->v2-1),2);
			if(((y&0xff)==DataType_array)&&((y>>8)>0))
				MenuCount->v3=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,11,8);
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,NUMmax_TAG);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	
	DisplayClrRow(Ln+2,Ln+6);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<=4;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)EQList[i]);
		}
		//���������й����� �ܼ��ƽ��
		for(i=0;i<=4;i++)
		{
			y=MRR(ADDR_230x_7+LENper_230x_7*(MenuCount->v2-1)+2+i*9,1);
			if(y!=DataType_long64)
			{//�޴�������---------
				DisplayString(Ln+2+i,4,0,"---------");	
			}
			else
			{
				y=MRR(ADDR_230x_7+LENper_230x_7*(MenuCount->v2-1)+2+i*9+1,8);
				y=R_Inverse(y,8);
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2+i)*84+3*2);
				disp_long64_kw(y,0,1,p16,2);
				//DisplayData_OffZero(y,8,4,p16);
			}
			//splayString(Ln+2+i,14,0,(u8 *)UnitList[2]);//2 kWh
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�ܼ���");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_2
// Descriptions:        ��ǰ����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_2(void)//�˵�1_2
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_2);//��ʾ�˵�
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_2_1);
						break;
					case 2:
						MenuCreate((u32)Menu_1_2_2);
						break;
					case 3:
						MenuCreate((u32)Menu_1_2_3);
						break;
					case 4:
						MenuCreate((u32)Menu_1_2_4);
						break;
				}        
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_2
// Descriptions:        ��ǰ���ܼ��鹦��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_2(void)//�˵�1_1_2
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;
	u64 a64;
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_1_2);//��ʾ�˵�
			if(MenuCount->v2>NUMmax_TAG)
			{//MenuCount->v2��¼�ܼ����
				MenuCount->v2=NUMmax_TAG;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			MR(ADDR_DATABUFF,ADDR_230x_2,LENmax_230x_2);
			y=MRR(ADDR_230x_2+LENper_230x_2*(MenuCount->v2-1),2);
			if(((y&0xff)==DataType_array)&&((y>>8)>0))
				MenuCount->v3=0;
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			p8[0*2]=MenuCount->v2+0x30;
			p8[0*2+1]=0x20;
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,9,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(1,9,1,NUMmax_TAG);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+3);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<=3;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)AddPowerList[i]);
		}
		//�й�
		a64=MRR(ADDR_230x_3+(MenuCount->v2-1)*LENper_230x_3,1);
		if(a64!=DataType_long64)
		{//�޴�������-------
			DisplayString(Ln+3,6,0,"-------");	
		}
		else
		{
			a64=MRR(ADDR_230x_3+(MenuCount->v2-1)*LENper_230x_3+1,8);
			a64=R_Inverse(a64,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+(1*2));
			disp_long64_kw(a64,0,1,p16,0);
		}
		//�޹�
		a64=MRR(ADDR_230x_4+(MenuCount->v2-1)*LENper_230x_4,1);	       
		if(a64!=DataType_long64)
		{//�޴�������-------
			DisplayString(Ln+5,6,0,"-------");	
		}
		else
		{
			a64=MRR(ADDR_230x_4+(MenuCount->v2-1)*LENper_230x_4+1,8);
			a64=R_Inverse(a64,8);
			p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+5)*84+(1*2));
			disp_long64_kw(a64,0,1,p16,1);
		}
	}
	else
	{
		DisplayString(Ln+2,4,0,"��Ч�ܼ����");	
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1_1
// Descriptions:        ��ǰ�й�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1_1(void)//�˵�1_1_1
{
#if NUMmax_IMPIN
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 y;
	u32 Ln;
	u64 a64;
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_1_1);
			if(MenuCount->v2>NUMmax_IMPIN)
			{//MenuCount->v2��¼��������˿ں�
				MenuCount->v2=NUMmax_IMPIN;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			MenuCount->v3=0xffff;
			for(i=0;i<NUMmax_IMPIN;i++)
			{
				p8=Get_Element((u8 *)(ADDR_240x_4+i*LENper_240x_4),1,1,LENmax_240x_4);
				if(p8!=0)
				{
					y=MRR((u32)(p8+1),4);
					y=R_Inverse(y,4);
					if((y&0xff)==MenuCount->v2)
					{
						MenuCount->v3=i;
						break;
					}
				}
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+9*2);
			bcd_p8(p8,MenuCount->v2,2);
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln+Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,10,9);
			break;
		case 1://����1:ѡ��
			NdigitSelect(2,9,1,NUMmax_IMPIN);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(Ln+2,Ln+4);//����Ļ��ʾ��ָ����
	if(MenuCount->v3!=0xffff)
	{
		for(i=0;i<5;i++)
		{
			DisplayString(Ln+2+i,0,0,(u8 *)PulsePowerList[i]);
		}
		//ͨѶ��ַ
		p8=(u8 *)ADDR_DATABUFF;
		MC(0,ADDR_DATABUFF,LENper_240x_2);
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+2)*84+5*2);
		MR(ADDR_DATABUFF,ADDR_240x_2+MenuCount->v3*LENper_240x_2+2,6);
		Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
		a64=MRR(ADDR_DATABUFF,6);
		DisplayData(a64,12,0,p16);

		p8=Get_Element((u8 *)(ADDR_240x_4+MenuCount->v3*LENper_240x_4),1,2,LENmax_240x_4);
		if(p8!=0)
		{
			if((p8[1]==0)||(p8[1]==2))
			{
				//��ǰ�й����� ��
				y=MRR(ADDR_240x_5+MenuCount->v3*LENper_240x_5,1);
				if(y!=DataType_double_long)
				{
					DisplayString(Ln+4,8,0,"-------");	
				}
				else
				{
					y=MRR(ADDR_240x_5+MenuCount->v3*LENper_240x_5+1,4);
					y=R_Inverse(y,4);
					a64=y;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+1*2);
					disp_long64_kw(a64,0,1,p16,0);
				}
				DisplayString(Ln+6,8,0,"-------");
			}
			else
			{
				//��ǰ�޹����� ��
				DisplayString(Ln+4,8,0,"-------");	
				y=MRR(ADDR_240x_5+MenuCount->v3*LENper_240x_5,1);
				if(y!=DataType_double_long)
				{
					DisplayString(Ln+6,8,0,"-------");	
				}
				else
				{
					y=MRR(ADDR_240x_5+MenuCount->v3*LENper_240x_5+1,4);
					y=R_Inverse(y,4);
					a64=y;
					p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+1*2);
					disp_long64_kw(a64,0,1,p16,0);
				}
			}
		}
		
	}
	else
	{
		DisplayString(Ln+2,5,0,"��Ч�����");	
	}
#endif
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_1
// Descriptions:        ��ǰ���ʲ˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_1(void)//
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_1);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_1_1);
						break;
					case 2:
						MenuCreate((u32)Menu_1_1_2);
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_3_1
// Descriptions:        �������߲˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
/*
void Menu_1_3_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 ADDR;
	u32 i;
	u32 j;
	u32 l;
	u32 m;
	u32 k;
	u32 min;
	u32 hour;
	u32 max;
	u32 Dot;//�������߶���ĵ���
	u32 xCP;//x�������
	u32 yCP;//y�������
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			DisplayClr();//��ȫ��
			Draw_Clr();
			Stack->MenuStack[MenuCount->Count].Task=1;
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
			{
				case 1:
					DisplayMenuString(0,0,2,(u8 *)pMenu_1_3_1);//��ʾ�˵��ַ���
					ADDR=ADDR_AFN0DF81;
					break;
				case 2:
					DisplayMenuString(0,0,2,(u8 *)pMenu_1_3_2);//��ʾ�˵��ַ���
					ADDR=ADDR_AFN0DF85;
					break;
			}
			DispalyAlign(0,0,2);//����ʾ����(1�����,2����,3�Ҷ���) 
			if(MenuCount->v2>MaxRS485AddCarrierPn)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=MaxRS485AddCarrierPn;
			}
			if(MenuCount->v2<1)
			{//MenuCount->v2��¼�������ȫ�ֱ���
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+8*2);
			bcd_p8(p8,MenuCount->v2,4);
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinColN(Stack->MenuStack[MenuCount->Count].EditRow,11,8);  
			
			Dot = (24*60/CurveCongealTime);
			if(Dot)
			{
				Draw_Clr();
				Draw_Line(16,96,144,96);//����
				Draw_Line(144,96,140,92);
				Draw_Line(144,96,140,100);
				Draw_Line(40,32,40,160);//����
				Draw_Line(40,32,36,36);
				Draw_Line(40,32,44,36);
				DisplayString(6,4,0,"0");
				xCP=1;
				DisplayString(5,17,0,"1:1");

				p16=(u16 *)ADDR_DATABUFF;
				for(j=0;j<Dot;j++)
				{
					p16[j]=0xEEEE;
				}
				//2��������0ʱ��������ֵ��Ϊ0;����2�������Ƿ��У��ڼ���ֵ�ڲ飬��������־Ϊ��ʱ����ȫ���ڲ顣
				m=MRR(ADDR_TCongealCount+7,2);//���߼���
				if(m&0x8000)
				{
					l=MaxCurveCongeal;
				}
				else
				{
					l=(m&0x7fff);
				}
				if(m)
				{
					for(i=0;i<l;i++)
					{
						if(MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+2,3) == MRR(ADDR_TYMDHMS+3,3))
						{//����ʱ���뵱ǰ������ͬ
							min = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i),1);//ʱ�� ��
							hour = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+1,1);//ʱ�� ʱ
							if(((min>>4)*10+(min&0xf))%CurveCongealTime == 0)
							{
								j = ((hour>>4)*10+(hour&0xf))*(60/CurveCongealTime) + ((min>>4)*10+(min&0xf))/CurveCongealTime;
								y = MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i)+ADDR+3*(MenuCount->v2-1),3);
								for(k=0;k<5;k++)
								{
									if(((y>>(k*4))&0x0f) > 9)
									{
										break;
									}
								}
								if(k==5)
								{
									p16[j] = ((y>>20)&0x7)*100+((y>>16)&0xf)*10+((y>>12)&0xf)*1+((y>>8)&0x8000);//���λΪ����λ
								}
							}
						}
					}
					max=0;
					for(i=0;i<Dot;i++)
					{
						if((p16[i]!=0xEEEE) && ((p16[i]&0x7fff)>max))
						{
							max=(p16[i]&0x7fff);
						}
					}
	
					yCP = (max/60)+1;
					DisplayString(2,0,0,(u8 *)YCPList[max/60]);//y�����
					for(i=0;i<Dot;i++)
					{
						if(p16[i]>>15)
						{//����
							if(p16[i]!=0xEEEE)
							{
								p16[i]=96+(p16[i]&0x7fff)/yCP;
							}
						}
						else
						{
							p16[i]=96-(p16[i]&0x7fff)/yCP;
						}
					}
					if(Dot==1)
					{
						if(p16[0]!=0xEEEE)
						{
							Draw_Line(40+0*xCP,p16[0],40+0*xCP,p16[0]);
						}
					}
					else
					{
						for(i=0;i<Dot-1;i++)
						{
							if((p16[i]!=0xEEEE) && (p16[i+1]!=0xEEEE))
							{
								Draw_Line(40+i*xCP,p16[i],40+(i+1)*xCP,p16[i+1]);
							}
							else
							{
								if((p16[i]!=0xEEEE) && (p16[i+1]==0xEEEE))
								{
									Draw_Line(40+i*xCP,p16[i],40+i*xCP,p16[i]);
								}
								if((p16[i]==0xEEEE) && (p16[i+1]!=0xEEEE))
								{
									Draw_Line(40+(i+1)*xCP,p16[i+1],40+(i+1)*xCP,p16[i+1]);
								}
							}
						}
					}
				}
			}
			break;
		case 1://����1:ѡ��
			NdigitSelect(4,8,1,MaxRS485AddCarrierPn);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			break;
		default:
			break;
	}
}
*/
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_3
// Descriptions:        �������߲˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_3(void)
{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	u8 * p8;
	p8=(u8 *)ADDR_MenuICO;//�˵���ʾ���������ã�0=��1=��
#endif
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_3);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
					case 2:
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
						p8[0]=0;//�˵���ʾ���������ã�0=��1=��
#endif
						//MenuCreate((u32)Menu_1_3_1);
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4_3
// Descriptions:        ��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_4_3(void)
{
#if (NUMmax_SWITCHIN)
	u16 *p16;
	u32 i;
	u32 j=0;
	u32 x;
	u32 y;
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	u32 Row=3;
#else
	u32 Row=2;
#endif

	for(i=0;i<NUMmax_SWITCHIN;i++)
	{
		j|=(1<<(7-i));
	}

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_4_3);//��ʾ�˵�
	x=Get_ElementADDR(ADDR_F203_4,1,0,LENmax_F203_4);
	if(x!=0)
	{
		x=MRR(x+2,1);
	}

	if((x&j)==0)
	{//״̬����û����
		DisplayString(Row,2,0,"��״̬������");
	}
	else
	{
		y=Get_ElementADDR(ADDR_F203_4,2,0,LENmax_F203_4);
		if(y!=0)
		{
			y=MRR(y+2,1);
		}
		//ң��  
		for(i=0;i<NUMmax_SWITCHIN;i++)
		{
			if((x>>(7-i))&0x01)
			{
				DisplayString(Row,2,0,"ң�� :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)SwitchNatureList[((y>>(7-i)))&0x01]);
				Row++;
			}
		}                            	 
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
#endif
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4_2
// Descriptions:        ��λ��Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_4_2(void)
{
#if (NUMmax_SWITCHIN)
	u16 *p16;
	u32 i;
	u32 j=0;
	u32 x;
	u32 y;
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	u32 Row=3;
#else
	u32 Row=2;
#endif

	for(i=0;i<NUMmax_SWITCHIN;i++)
	{
		j|=(1<<(7-i));
	}

	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_4_2);//��ʾ�˵�
	x=Get_ElementADDR(ADDR_F203_4,1,0,LENmax_F203_4);
	if(x!=0)
	{
		x=MRR(x+2,1);
	}
	if((x&j)==0)
	{//״̬����û����
		DisplayString(Row,2,0,"��״̬������");
	}
	else
	{
		//ң��  
		for(i=0;i<NUMmax_SWITCHIN;i++)
		{
			y=Get_ElementADDR(ADDR_F203_2,i+1,2,LENmax_F203_2);
			if(y!=0)
			{
				y=MRR(y+1,1);
				if(y>1)
					y=0;
			}
			if((x>>(7-i))&0x01)
			{
				DisplayString(Row,2,0,"ң�� :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)ShiftList[y]);
				Row++;
			}
		}                            	 
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
#endif
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4_1
// Descriptions:        ����״̬
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_4_1(void)
{
#if (NUMmax_SWITCHIN)
	u16 *p16;
	u32 i;
	u32 j=0;
	u32 x;
	u32 y;
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	u32 Row=3;
#else
	u32 Row=2;
#endif

	for(i=0;i<NUMmax_SWITCHIN;i++)
	{
		j|=(1<<(7-i));
	}
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_4_1);//��ʾ�˵�
	x=Get_ElementADDR(ADDR_F203_4,1,0,LENmax_F203_4);
	if(x!=0)
	{
		x=MRR(x+2,1);
	}
	if((x&j)==0)
	{//״̬����û����
		DisplayString(Row,2,0,"��״̬������");
	}
	else
	{
		//ң��  
		for(i=0;i<NUMmax_SWITCHIN;i++)
		{
			y=Get_ElementADDR(ADDR_F203_2,i+1,1,LENmax_F203_2);
			if(y!=0)
			{
				y=MRR(y+1,1);
				if(y>1)
					y=0;
			}
			if((x>>(7-i))&0x01)
			{
				DisplayString(Row,2,0,"ң�� :");
				p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+6*2);
				p16[0]=0x2030+i+1;
				DisplayString(Row,9,0,(u8 *)SwitchStateList[y]);
				Row++;
			}
		}
	}
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
#endif
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_4
// Descriptions:        ����״̬�˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_4(void)
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1_4);//��ʾ�˵�
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_4_1);
						break;
					case 2:
						MenuCreate((u32)Menu_1_4_2);
						break;
					case 3:
						MenuCreate((u32)Menu_1_4_3);
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}

#if ((Project/100)==3) && ((USER/100)==5)//�Ϻ�ר���ն�
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_5_1_1
// Descriptions:        �¼���¼����(��2��)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_5_1_1(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 x;
	u32 ADDR;
	u64 z;
	u64 y;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>((MenuCount->v3)>>8))
			{
				MenuCount->v2=1;
			}
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
			{
				case 2:
					x = Event(1,(MenuCount->v3)&0xff,MenuCount->v2);//m=1��Ҫ�¼���m=0һ���¼����¼���¼����ERC�����n�� 
					ADDR = ADDR_AFN0EF1;
					break;
				case 3:
					x = Event(0,(MenuCount->v3)&0xff,MenuCount->v2);//m=1��Ҫ�¼���m=0һ���¼����¼���¼����ERC�����n��
					ADDR = ADDR_AFN0EF2;
					break;
			}
			if(x)
			{
				switch((MenuCount->v3)&0xff)
				{
					case 17:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC17_2);//��ʾ�˵�
						UpArrow();
						//����ʱ������ƽ���
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+12,2);
						if(y==0xEEEE)
						{//�޴�������-----
							DisplayString(3,6,0,"-----");	
						}
						else
						{
							DisplayString(3,6,0,(u8 *)SList[y>>15]);//+ -
							y&=0x7fff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+3*84+7*2);
							DisplayData_OffZero(y,4,1,p16);
						}
						DisplayString(3,14,0,(u8 *)UnitList[6]);//6 %
						//����ʱ��ѹ����ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14+2*i,2);   
							if(y==0xEEEE)
							{//�޴�������-----
								DisplayString(4+i,6,0,"-----");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+6*2);
								p16=DisplayData_OffZero(y,4,1,p16);
								p16[0]=0x2000+',';
							}
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+20+3*i,3);   
							if(y==0xEEEEEE)
							{//�޴�������------
								DisplayString(4+i,12,0,"------");	
							}
							else
							{
								DisplayString(4+i,12,0,(u8 *)SList[y>>23]);//+ -
								y&=0x7fffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(4+i)*84+13*2);
								DisplayData_OffZero(y,6,3,p16);
							}
						}
						break;
					case 19:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC19_2);//��ʾ�˵�
						UpArrow();
						//�������ޡ���բ���ޡ�ʣ�������
						for(i=0;i<=3;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+17+4*i,4);
							if(y==0xEEEEEEEE)
							{//�޴�������-------
								DisplayString(3+2*i,3,0,"-------");	
							}
							else
							{
								z=y&0xfffffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(3+2*i)*84+3*2);
								p16=DisplayData_OffZero(z,7,0,p16);
								DisplayString(3+2*i,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
								for(j=4;j<20;j++)
								{
									if(p16 == (u16 *)(ADDR_STRINGBUFF+(3+2*i)*84+4+j*2))
									{
										break;
									}
								}
								DisplayString(3+2*i,j+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/�� 1 MWh/Ԫ
							}
							DispalyAlign(3+2*i,17,3);//����ʾ����(1�����,2����,3�Ҷ���)
						}
						break;
				}
				//����� ? ��
				p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
				bcd_p8(p8,MenuCount->v2,3);
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
				p16[0]=0x2020;
				p16[1]=0x2020;
				p16[2]=32;//'���Ҽ�ͷ'
				p16[3]=33;
			}
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,12,10);
			break;
		case 1://����1:ѡ��
			NdigitSelect(3,10,1,(MenuCount->v3)>>8);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuCount->v2 = ((MenuCount->v3)&0xff);
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			switch((MenuCount->v3)&0xff)
			{
				case 17:
				case 19:
					if(Stack->MenuStack[MenuCount->Count].EditCol==0)
					{
						if(Stack->MenuStack[MenuCount->Count].EditCol==0)
						{
							if((Comm_Ram->ButtonVal&0x02)!=0)
							{//�ϼ�
								CreateScreen((u32)Menu_1_5_1);
							}
							Comm_Ram->ButtonVal&=~0x02;//��ʹ�ú���0
						}
					}
					break;
			}
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_5_1
// Descriptions:        �¼���¼����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_5_1(void)
{
	u8 *p8;
	u8 *p;
	u16 *p16;
	u16 *pa16;
	u32 i;
	u32 j;
	u32 k;
	u32 x;
	u32 ADDR;
	u64 z;
	u64 y;
	u32 Ln;
	u32 Col;
	u32 Fn;
	u32 pn;
	u32 m1;
	u32 m2;
	u32 n;
	u32 m;

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			if(MenuCount->v2>((MenuCount->v3)>>8))
			{
				MenuCount->v2=1;
			}
			switch(Stack->MenuStack[MenuCount->Count-1].EditRow)
			{
				case 2:
					x = Event(1,(MenuCount->v3)&0xff,MenuCount->v2);//m=1��Ҫ�¼���m=0һ���¼����¼���¼����ERC�����n�� 
					ADDR = ADDR_AFN0EF1;
					break;
				case 3:
					x = Event(0,(MenuCount->v3)&0xff,MenuCount->v2);//m=1��Ҫ�¼���m=0һ���¼����¼���¼����ERC�����n��
					ADDR = ADDR_AFN0EF2;
					break;
			}
			if(x)
			{
				switch((MenuCount->v3)&0xff)
				{
					case 1:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC1);//��ʾ�˵�
						//�������ʱ��

						//�¼���־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						if(y&0x1)
						{
							DisplayString(4,10,0,"��ʼ��");
						}
						else
						{
							DisplayString(4,10,0,"δ��ʼ��");
						}
						if(y&0x2)
						{
							DisplayString(5,10,0,"�汾���");
						}
						else
						{
							DisplayString(5,10,0,"�汾δ���");
						}
						//���ǰ��������汾��
						p8=(u8 *)(ADDR_DATABUFF);
						MR(ADDR_DATABUFF,ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,8);
						p=(u8 *)(ADDR_STRINGBUFF+6*84+4+16*2);
						for(i=0;i<=3;i++)
						{
							p[i*2]=p8[i];
							p[i*2+1]=0x20;
						}
						p=(u8 *)(ADDR_STRINGBUFF+7*84+4+16*2);
						for(i=0;i<=3;i++)
						{
							p[i*2]=p8[4+i];
							p[i*2+1]=0x20;
						}
						break;
					case 2:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC2);//��ʾ�˵�
						//����ʱ��

						//�¼���־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						if(y&0x1)
						{
							DisplayString(5,4,0,"�ն˲�����ʧ");
						}
						else
						{
							DisplayString(5,4,0,"�ն˲���δ��ʧ");
						}
						if(y&0x2)
						{
							DisplayString(6,4,0,"�����������ʧ");
						}
						else
						{
							DisplayString(6,4,0,"���������δ��ʧ");
						}
						break;
					case 3:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC3);//��ʾ�˵�
						//�������ʱ��

						//����վ��ַ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y=hex_bcd(y);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+12*2);
						DisplayData_OffZero(y,3,0,p16);
						//����������ݵ�Ԫ��ʶ
						k=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+1,1);//����
						p8=(u8 *)(ADDR_DATABUFF);
						k=((k-6)/4);
						Ln=5;
						Col=9;
						for(j=0;j<k;j++)
						{
							MR(ADDR_DATABUFF,ADDR+(LEN_TERMINAL_EVENT*(x-1))+8+4*j,4);
							for(i=0;i<8;i++)
							{
								//Fnpn
								Fn=DT1DT2_Fn(p8[2],p8[3],i);
								if(Fn)
								{
									if(p8[0]==0 && p8[1]==0)
									{
										pn=0;
										n=1;
										for(m1=0;m1<4;m1++)
										{
											n*=10;
											if((Fn/n)==0)
											{
												break;
											}
										}
										n=1;
										for(m2=0;m2<4;m2++)
										{
											n*=10;
											if((pn/n)==0)
											{
												break;
											}
										}
										if(Col>=20)
										{
											Ln++;
											Col=1;
										}
										else
										{
											if((19-Col)<(m1+m2+2))
											{
												Ln++;
												Col=1;
											}
											else
											{
												Col++;
											}
										}
										p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+(Col-1)*2);
										p16[0]=0x2000+'F';
										p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+Col*2);
										pa16=DisplayData_OffZero(hex_bcd(Fn&0xff),3,0,p16);
										pa16[0]=0x2000+'p';
										pa16=DisplayData_OffZero(hex_bcd(pn),3,0,pa16+1);
										Col+=(pa16-p16+1);
									}
									else
									{
										for(m=0;m<8;m++)
										{
											pn=DA1DA2_pn(p8[0],p8[1],m);
											if(pn)
											{
												n=1;
												for(m1=0;m1<4;m1++)
												{
													n*=10;
													if((Fn/n)==0)
													{
														break;
													}
												}
												n=1;
												for(m2=0;m2<4;m2++)
												{
													n*=10;
													if((pn/n)==0)
													{
														break;
													}
												}
												if(Col>=20)
												{
													Ln++;
													Col=1;
												}
												else
												{
													if((19-Col)<(m1+m2+2))
													{
														Ln++;
														Col=1;
													}
													else
													{
														Col++;
													}
												}
												p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+(Col-1)*2);
												p16[0]=0x2000+'F';
												p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+Col*2);
												pa16=DisplayData_OffZero(hex_bcd(Fn&0xff),3,0,p16);
												pa16[0]=0x2000+'p';
												pa16=DisplayData_OffZero(hex_bcd(pn),3,0,pa16+1);
												Col+=(pa16-p16+1);
											}
										}
									}
								}
							}
							if(Ln==9 && Col>16)
							{
								DisplayString(9,17,0,"...");
								break;
							}
						}
						break;
					case 4:
					case 36://ERC4��ERC36��¼���������ݸ�ʽһ��
						if(((MenuCount->v3)&0xff)==4)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC4);//��ʾ�˵�
						}
						if(((MenuCount->v3)&0xff)==36)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC36);//��ʾ�˵�
						}
						//��λʱ��

						//״̬��λ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						for(i=0;i<=7;i++)
						{
							if((y>>i)&0x01)
							{//������λ
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//��λ��״̬
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=7;i++)
						{
							if(((y>>i)&0x01)==1)
							{
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						break;
					case 5:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC5);//��ʾ�˵�
						//��բʱ��

						//��բ�ִ�
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						for(i=0;i<=7;i++)
						{
							if((y>>i)&0x01)
							{//�ܿ�
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//��բʱ����
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,2);
						if(y==0xEEEE)
						{//�޴�������-------
							DisplayString(5,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(5*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(5,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+5*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(5,i,0,(u8 *)UnitList[0]);//0 kW
						}
						//��բ��2���ӵĹ���
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,2);
						if(y==0xEEEE)
						{//�޴�������-------
							DisplayString(6,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(6*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(6,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+6*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(6,i,0,(u8 *)UnitList[0]);//0 kW
						}
						break;
					case 6:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC6);//��ʾ�˵�
						//��բʱ��

						//�ܼ����
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//��բ�ִ�
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						for(i=0;i<=7;i++)
						{
							if((y>>i)&0x01)
							{//�ܿ�
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//�������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						for(i=0;i<4;i++)
						{
							if((y>>i)&0x01)
							{
								DisplayString(6,10,0,(u8 *)PowerControlList[i]);
							}
						}
						//��բǰ����
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,2);
						if(y==0xEEEE)
						{//�޴�������-------
							DisplayString(7,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(7*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(7,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(7,i,0,(u8 *)UnitList[0]);//0 kW
						}
						//��բ��2���ӵĹ���
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+12,2);
						if(y==0xEEEE)
						{//�޴�������-------
							DisplayString(8,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(8*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(8,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(8,i,0,(u8 *)UnitList[0]);//0 kW
						}
						//��բʱ���ʶ�ֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,2);
						if(y==0xEEEE)
						{//�޴�������-------
							DisplayString(9,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+(9*84)+(10*2));
							p16 = PowerP16(y,p16);
							DisplayString(9,9,0,(u8 *)SList[(y>>12)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+9*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(9,i,0,(u8 *)UnitList[0]);//0 kW
						}
						break;
					case 7:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC7);//��ʾ�˵�
						//��բʱ��

						//�ܼ����
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//��բ�ִ�
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						for(i=0;i<=7;i++)
						{
							if((y>>i)&0x01)
							{//�ܿ�
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						for(i=0;i<2;i++)
						{
							if((y>>i)&0x01)
							{
								DisplayString(6,10,0,(u8 *)EControlList[i]);
							}
						}
						//��բʱ����
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,4);
						if(y==0xEEEEEEEE)
						{//�޴�������-------
							DisplayString(7,10,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+10*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(7,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
								{
									break;
								}
							}
						}
						//��բʱ��ֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,4);
						if(y==0xEEEEEEEE)
						{//�޴�������-------
							DisplayString(8,10,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+10*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(8,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
								{
									break;
								}
							}
						}
						break;
					case 8:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC8);//��ʾ�˵�
						//����ʱ��

						//�������
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�����־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if((y>>0)&0x01)
						{//���ܱ����ʱ��
							DisplayString(5,18,0,"��");
						}
						else
						{
							DisplayString(5,18,0,"δ");
						}
						if((y>>1)&0x01)
						{//���ܱ���ʱ��
							DisplayString(6,12,0,"��");
						}
						else
						{
							DisplayString(6,12,0,"δ");
						}
						if((y>>2)&0x01)
						{//���ܱ�����
							DisplayString(7,4,0,"��");
						}
						else
						{
							DisplayString(7,4,0,"δ");
						}
						if((y>>3)&0x01)
						{//���ܱ����峣��
							DisplayString(7,18,0,"��");
						}
						else
						{
							DisplayString(7,18,0,"δ");
						}
						if((y>>4)&0x01)
						{//���ܱ���������
							DisplayString(8,14,0,"��");
						}
						else
						{
							DisplayString(8,14,0,"δ");
						}
						if((y>>5)&0x01)
						{//���ܱ������������
							DisplayString(9,14,0,"��");
						}
						else
						{
							DisplayString(9,14,0,"δ");
						}
						break;
					case 9:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC9);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�쳣��־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if((y>>i)&0x01)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						DisplayString(5,15,0,(u8 *)IUnusualList[((y>>6)&0x3)-1]);
						//����ʱ��ѹ����ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10+2*i,2);   
							if(y==0xEEEE)
							{//�޴�������-----
								DisplayString(6+i,6,0,"-----");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+6*2);
								p16=DisplayData_OffZero(y,4,1,p16);
								p16[0]=0x2000+',';
							}
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+16+3*i,3);   
							if(y==0xEEEEEE)
							{//�޴�������------
								DisplayString(6+i,12,0,"------");	
							}
							else
							{
								DisplayString(6+i,12,0,(u8 *)SList[y>>23]);//+ -
								y&=0x7fffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+13*2);
								DisplayData_OffZero(y,6,3,p16);
							}
						}
						//����ʱ�����й��ܵ���ʾֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+25,5);
						if(y==0xEEEEEEEEEE)
						{//�޴�������-------
							DisplayString(9,6,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+6*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						break;
					case 10:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC10);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�쳣��־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if((y>>i)&0x01)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						if(((y>>6)&0x3) == 1)
						{
							DisplayString(5,15,0,"����");
						}
						if(((y>>6)&0x3) == 2)
						{
							DisplayString(5,15,0,"ʧѹ");
						}
						//����ʱ��ѹ����ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10+2*i,2);   
							if(y==0xEEEE)
							{//�޴�������-----
								DisplayString(6+i,6,0,"-----");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+6*2);
								p16=DisplayData_OffZero(y,4,1,p16);
								p16[0]=0x2000+',';
							}
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+16+3*i,3);   
							if(y==0xEEEEEE)
							{//�޴�������------
								DisplayString(6+i,12,0,"------");	
							}
							else
							{
								DisplayString(6+i,12,0,(u8 *)SList[y>>23]);//+ -
								y&=0x7fffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+13*2);
								DisplayData_OffZero(y,6,3,p16);
							}
						}
						//����ʱ�����й��ܵ���ʾֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+25,5);
						if(y==0xEEEEEEEEEE)
						{//�޴�������-------
							DisplayString(9,6,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+6*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						break;
					case 11:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC11);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�Ƕ�
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9+2*i,2);
							if(y==0xEEEE)
							{
								DisplayString(5+i,7,0,"-----,");
							}	       
							else
							{
								DisplayString(5+i,7,0,(u8 *)SList[y>>15]);//+ -
								y&=0x7fff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(5+i)*84+8*2);
								p16=DisplayData_OffZero(y,4,1,p16);
								p16[0]=0x2000+',';
							}
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+15+2*i,2);	       
							if(y==0xEEEE)
							{
								DisplayString(5+i,14,0,"-----");
							}	       
							else
							{
								DisplayString(5+i,14,0,(u8 *)SList[y>>15]);//+ -
								y&=0x7fff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(5+i)*84+15*2);
								DisplayData_OffZero(y,4,1,p16);
							}
						}
						//����ʱ�����й��ܵ���ʾֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+21,5);
						if(y==0xEEEEEEEEEE)
						{//�޴�������-------
							DisplayString(8,4,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+4*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						break;
					case 12:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC12);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						break;
					case 13:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC13);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�쳣��־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if((y>>0)&0x01)
						{//��̴�������������������
							DisplayString(6,16,0,"��");
						}
						else
						{
							DisplayString(6,16,0,"δ");
						}
						if((y>>1)&0x01)
						{//�������
							DisplayString(7,10,0,"��");
						}
						else
						{
							DisplayString(7,10,0,"δ");
						}
						if((y>>2)&0x01)
						{//ʧѹ����
							DisplayString(8,4,0,"��");
						}
						else
						{
							DisplayString(8,4,0,"δ");
						}
						if((y>>3)&0x01)
						{//ͣ�����
							DisplayString(8,18,0,"��");
						}
						else
						{
							DisplayString(8,18,0,"δ");
						}
						if((y>>4)&0x01)
						{//���Ƿѹ
							DisplayString(9,8,0,"��");
						}
						else
						{
							DisplayString(9,8,0,"δ");
						}
						break;
					case 14:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC14);//��ʾ�˵�
//#define NewERC14	0//ERC14ͣ���¼���¼����0=ԭ��׼��1=����Ӫ����2014.1.15֪ͨ
						//ʧ��ʱ��
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+2+NewERC14,5);
						p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+10*2);
						for(i=0;i<2;i++)
						{
							DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+':';
						//�ָ�ʱ��
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7+NewERC14,5);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<2;i++)
						{
							DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+':';
						break;
					case 15://���䡢�������Ĺ���
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC15);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�쳣��־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+14*2);
						for(i=0;i<3;i++)
						{
							if((y>>i)&0x01)
							{
								p16[i]=i+0x2041;
							}
						}
						if(y>>7)
						{//����
							DisplayString(5,10,0,"����");
						}
						else
						{//��ѹ
							DisplayString(5,10,0,"��ѹ");
						}
						break;
					case 16://���䡢�������Ĺ���
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC16);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>7)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x3f);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//Խ�ޱ�־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						if(y&0x1)
						{
							DisplayString(5,10,0,"Խ����");
						}
						else
						{
							DisplayString(5,10,0,"δԽ����");
						}
						if(y&0x2)
						{
							DisplayString(6,10,0,"Խ����");
						}
						else
						{
							DisplayString(6,10,0,"δԽ����");
						}
						//Խ��ʱֱ��ģ��������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,2);
						if(y==0xEEEE)
						{//�޴�������-------
							DisplayString(8,10,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+(11*2));
							PowerP16(y,p16);
							DisplayString(8,10,0,(u8 *)SList[(y>>12)&0x01]);//+ -
						}
						break;
					case 17:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC17_1);//��ʾ�˵�
						DownArrow();
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�쳣��־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if(y&0x1)
						{
							DisplayString(6,2,0,"��ѹ��ƽ���Խ��");
						}
						else
						{
							DisplayString(6,2,0,"��ѹ��ƽ���δԽ��");
						}
						if((y>>1)&0x1)
						{
							DisplayString(7,2,0,"������ƽ���Խ��");
						}
						else
						{
							DisplayString(7,2,0,"������ƽ���δԽ��");
						}
						//����ʱ��ѹ��ƽ���
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,2);
						if(y==0xEEEE)
						{//�޴�������-----
							DisplayString(9,8,0,"-----");	
						}
						else
						{
							DisplayString(9,7,0,(u8 *)SList[y>>15]);//+ -
							y&=0x7fff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+8*2);
							DisplayData_OffZero(y,4,1,p16);
						}
						DisplayString(9,15,0,(u8 *)UnitList[6]);//6 %
						break;
					case 18://���޴˹���
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC18);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�쳣��־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if(y&0x01)
						{
							DisplayString(5,10,0,"��ѹ");
						}
						else
						{
							DisplayString(5,10,0,"�޹�ѹ");
						}
						if(y&0x01)
						{
							DisplayString(6,10,0,"װ�ù���");
						}
						else
						{
							DisplayString(6,10,0,"װ���޹���");
						}
						if(y&0x01)
						{
							DisplayString(7,6,0,"ִ�л�·����");
						}
						else
						{
							DisplayString(7,6,0,"ִ�л�·�޹���");
						}
						break;
					case 19:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC19_1);//��ʾ�˵�
						DownArrow();
						//����ʱ��

						//�ܼ����
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//���絥��
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,4);
						y=hex_bcd16(y);
						p16=(u16 *)(ADDR_STRINGBUFF+4+5*84+10*2);
						DisplayData(y,10,0,p16);
						//׷��/ˢ�±�־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+12,1);
						switch(y)
						{
							case 0x55:
								i=0;
								break;
							case 0xAA:
								i=1;
								break;
							default:
								i=2;
								break;
						}
						DisplayString(6,10,0,(u8 *)PurchaseWayList[i]);//0׷�� 1ˢ�� 2��Ч
						//��������ֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+13,4);
						if(y==0xEEEEEEEE)
						{//�޴�������-------
							DisplayString(8,3,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+3*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(8,2,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(j=4;j<20;j++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+j*2))
								{
									break;
								}
							}
							DisplayString(8,j+1,0,(u8 *)QCUnitList[(y>>30)&0x01]);//0 kWh/�� 1 MWh/Ԫ
						}
						DispalyAlign(8,17,3);//����ʾ����(1�����,2����,3�Ҷ���)
						break;
					case 20:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC20);//��ʾ�˵�
						//����ʱ��

						//��Ϣ��֤��PW
						p8=(u8 *)(ADDR_DATABUFF);
						MR(ADDR_DATABUFF,ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,16);
						for(j=0;j<=1;j++)
						{
							p=(u8 *)(ADDR_STRINGBUFF+(4+j)*84+4+4*2);
							for(i=0;i<8;i++)
							{
								if((p8[i+8*j]>>4)<=9)
								{
									p[(2*i)*2]=(p8[i+8*j]>>4)+0x30;
								}
								else
								{
									p[(2*i)*2]=(p8[i+8*j]>>4)+0x37;
								}
								p[(2*i)*2+1]=0x20;
								if((p8[i+8*j]&0xf)<=9)
								{
									p[(2*i+1)*2]=(p8[i+8*j]&0xf)+0x30;
								}
								else
								{
									p[(2*i+1)*2]=(p8[i+8*j]&0xf)+0x37;
								}
								p[(2*i+1)*2+1]=0x20;
							}
						}
						//����վ��ַMSA
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+23,1);
						y=hex_bcd(y);
						p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+4*2);
						DisplayData_OffZero(y,3,0,p16);
						break;
					case 21:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC21);//��ʾ�˵�
						//����ʱ��

						//�ն˹���
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						if(y<=6 && y>0)
						{
							DisplayString(4,6,0,(u8 *)TerminalBugList[y-1]);
						}
						break;
					case 22:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC22);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־������������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>7)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7f);
						DisplayData_OffZero(y,2,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//Խ��ʱ�Ա��ܼ����й��ܵ�������Խ��ʱ�����ܼ����й��ܵ�����
						for(i=0;i<2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8+4*i,4);
							if(y==0xEEEEEEEE)
							{//�޴�������-------
								DisplayString(6+i,10,0,"-------");	
							}
							else
							{
								DisplayString(6+i,10,0,(u8 *)SList[(y>>28)&0x01]);//+ -
								z=y&0xfffffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+i)*84+11*2);
								p16=DisplayData_OffZero(z,7,0,p16);
								if((y>>30)&0x01)
								{
									p16[0]=0x2000+'M';
								}
								else
								{
									p16[0]=0x2000+'k';
								}
							}
						}
						//���ƫ��ֵ
						p16=(u16 *)(ADDR_STRINGBUFF+8*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+16,1);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						p16[0]=0x2000+'%';
						//����ƫ��ֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+17,4);
						if(y==0xEEEEEEEE)
						{//�޴�������-------
							DisplayString(9,10,0,"-------");	
						}
						else
						{
							DisplayString(9,10,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+11*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							if((y>>30)&0x01)
							{
								p16[0]=0x2000+'M';
							}
							else
							{
								p16[0]=0x2000+'k';
							}
						}
						break;
					case 23:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC23);//��ʾ�˵�
						//����ʱ��

						//�ܼ����
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=hex_bcd(y);
						DisplayData_OffZero(y,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//Ͷ���ִ�
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=7;i++)
						{
							if(((y>>i)&0x01)==1)
							{//�ܿ�
								p16[i]=i+0x2031;
							}
							else
							{
								p16[i]=0x2000+'-';
							}
						}
						//������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						for(i=0;i<2;i++)
						{
							if((y>>i)&0x01)
							{
								DisplayString(6,10,0,(u8 *)EControlList[i]);
							}
						}
						//�澯ʱ������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,4);
						if(y==0xEEEEEEEE)
						{//�޴�������-------
							DisplayString(7,6,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+10*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(7,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+7*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(7,i,0,(u8 *)QUnitList[(y>>30)&0x01]);//0 kWh 1 MWh
						}
						//�澯ʱ��ض�ֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,4);
						if(y==0xEEEEEEEE)
						{//�޴�������-------
							DisplayString(8,6,0,"-------");	
						}
						else
						{
							z=y&0xfffffff;
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+10*2);
							p16=DisplayData_OffZero(z,7,0,p16);
							DisplayString(8,9,0,(u8 *)SList[(y>>28)&0x01]);//+ -
							for(i=11;i<20;i++)
							{
								if(p16 == (u16 *)(ADDR_STRINGBUFF+8*84+4+i*2))
								{
									break;
								}
							}
							DisplayString(8,i,0,(u8 *)QUnitList[(y>>30)&0x01]);//0 kWh 1 MWh
						}
						break;
					case 24:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC24);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//Խ�ޱ�־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if(((y>>i)&0x01)==1)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						if(((y>>6)&0x3) == 1)
						{
							DisplayString(6,10,0,"Խ������");
						}
						if(((y>>6)&0x3) == 2)
						{
							DisplayString(6,10,0,"Խ������");
						}
						//����ʱ�ĵ�ѹABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10+2*i,2);  
							if(y==0xEEEE)
							{//�޴�������-----
								DisplayString(7+i,10,0,"----");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(7+i)*84+10*2);
								DisplayData_OffZero(y,4,1,p16);
							}
							DisplayString(7+i,16,0,(u8 *)UnitList[4]);//4 V
						}
						break;
					case 25:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC25);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//Խ�ޱ�־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if(((y>>i)&0x01)==1)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						if(((y>>6)&0x3) == 1)
						{
							DisplayString(6,10,0,"Խ������");
						}
						if(((y>>6)&0x3) == 2)
						{
							DisplayString(6,10,0,"Խ����");
						}
						//����ʱ�ĵ���ABC
						for(i=0;i<=2;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10+3*i,3);   
							if(y==0xEEEEEE)
							{//�޴�������------
								DisplayString(7+i,10,0,"------");	
							}
							else
							{
								DisplayString(7+i,10,0,(u8 *)SList[y>>23]);//+ -
								y&=0x7fffff;
								p16=(u16 *)(ADDR_STRINGBUFF+4+(7+i)*84+11*2);
								DisplayData_OffZero(y,6,3,p16);
							}
							DisplayString(7+i,18,0,(u8 *)UnitList[5]);//4 A
						}
						break;
					case 26:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC26);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//Խ�ޱ�־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if(((y>>6)&0x3) == 1)
						{
							DisplayString(5,10,0,"Խ������");
						}
						if(((y>>6)&0x3) == 2)
						{
							DisplayString(5,10,0,"Խ����");
						}
						//����ʱ���ڹ���
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,3);
						if(y==0xEEEEEE)
						{//�޴�������-------
							DisplayString(6,12,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+12*2);
							DisplayData_OffZero(y,6,4,p16);
						}
						//����ʱ���ڹ�����ֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+13,3);
						if(y==0xEEEEEE)
						{//�޴�������-------
							DisplayString(7,12,0,"-------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+12*2);
							DisplayData_OffZero(y,6,4,p16);
						}

						break;
					case 27:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC27);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�½�ǰ�����й��ܵ���ʾֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,5);
						if(y==0xEEEEEEEEEE)
						{//�޴�������-------
							DisplayString(6,4,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+4*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						//�½��������й��ܵ���ʾֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,5);
						if(y==0xEEEEEEEEEE)
						{//�޴�������-------
							DisplayString(8,4,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+4*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						break;
					case 28://�ݲ�֧�� ��ERC29��¼���������ݸ�ʽһ��
					case 29:
						if(((MenuCount->v3)&0xff)==28)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC28);//��ʾ�˵�
						}
						if(((MenuCount->v3)&0xff)==29)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC29);//��ʾ�˵�
						}
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//����ǰ���������й��ܵ���ʾֵ
						for(i=0;i<=1;i++)
						{
							y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9+5*i,5);
							if(y==0xEEEEEEEEEE)
							{//�޴�������-------
								DisplayString(6+2*i,4,0,"-----------");	
							}
							else
							{
								p16=(u16 *)(ADDR_STRINGBUFF+4+(6+2*i)*84+4*2);			  
								DisplayData_OffZero(y,10,4,p16);
							}
						}
						//������ֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+19,1);
						p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+10*2);			  
						DisplayData_OffZero(y,2,1,p16);
						break;
					case 30:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC30);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//���ܱ�ͣ��ʱ�����й��ܵ���ʾֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,5);
						if(y==0xEEEEEEEEEE)
						{//�޴�������-------
							DisplayString(6,8,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+6*84+9*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						//���ܱ�ͣ����ֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,1);
						if(y==0xEE)
						{//�޴�������-------
							DisplayString(7,10,0,"---");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+10*2);
							DisplayData_OffZero(y,3,0,p16);
						}
						break;
					case 31:
					case 39://ERC31��ERC39��¼���������ݸ�ʽһ��
						if(((MenuCount->v3)&0xff)==31)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC31);//��ʾ�˵�
						}
						if(((MenuCount->v3)&0xff)==39)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC39);//��ʾ�˵�
						}
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//���һ�γ���ɹ�ʱ��
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,5);
						p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+2*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						for(i=0;i<2;i++)
						{
							DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+9+3*i);
						}
						p16[11]=0x2000+':';
						//���һ�γ���ɹ������й��ܵ���ʾֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+14,5);
						if(y==0xEEEEEEEEEE)
						{//�޴�������-------
							DisplayString(8,7,0,"-----------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+8*84+7*2);			  
							DisplayData_OffZero(y,10,4,p16);
						}
						//���һ�γ���ɹ������޹��ܵ���ʾֵ
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+19,4);
						if(y==0xEEEEEEEE)
						{//�޴�������-------
							DisplayString(9,7,0,"---------");	
						}
						else
						{
							p16=(u16 *)(ADDR_STRINGBUFF+4+9*84+7*2);			  
							DisplayData_OffZero(y,8,2,p16);
						}
						break;
					case 32:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC32);//��ʾ�˵�
						//����ʱ��

						//�����ѷ�����ͨ������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,4);
						p16=(u16 *)(ADDR_STRINGBUFF+4+5*84+4*2);
						y=hex_bcd16(y);			  
						DisplayData_OffZero(y,10,0,p16);
						//��ͨ����������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+11,4);
						p16=(u16 *)(ADDR_STRINGBUFF+4+7*84+4*2);			  
						y=hex_bcd16(y);			  
						DisplayData_OffZero(y,10,0,p16);
						break;
					case 33:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC33);//��ʾ�˵�
						//����ʱ��

						//�������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=hex_bcd(y);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						break;
					case 34://���޴˹���
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC34);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�쳣��־
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<=2;i++)
						{
							if(((y>>i)&0x01)==1)
							{//ABC
								p16[i]=i+0x2041;
							}
						}
						if(((y>>6)&0x3)>0)
						{
							DisplayString(6,10,0,(u8 *)ERC34List[((y>>6)&0x3)-1]);
						}
						break;
					case 35://���޴˹���
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC35);//��ʾ�˵�
						//����ʱ��

						//�ն�ͨ�Ŷ˿ں�D0~D5
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,1);
						y&=0x3F;
						y=hex_bcd(y);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						DisplayData_OffZero(y,2,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//���ֿ���
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+8,1);
						y=hex_bcd(y);
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						DisplayData_OffZero(y,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						break;
					case 37://���ܱ�����¼���¼
					case 38://���ܱ���ť���¼���¼//ERC37��ERC38��¼���������ݸ�ʽһ��
						if(((MenuCount->v3)&0xff)==37)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC37);//��ʾ�˵�
						}
						if(((MenuCount->v3)&0xff)==38)
						{
							DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC38);//��ʾ�˵�
						}
						//����ʱ��

						//�������
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						y=hex_bcd(y);
						p16=DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�����ܴ���
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+11*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,3);
						p16=DisplayData_OffZero(y,6,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						break;
//					case 40://�ӽڵ������ϱ��¼���¼
//						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC40);//��ʾ�˵�
//						//����ʱ��
//
//						//�¼��ӽڵ��ַ
//						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+4*2);
//						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,6);
//						p16=DisplayData_OffZero(y,12,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
//						//�ӽڵ�����
//						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+13,1);
//						if(y<3)
//						{//03H-FFH����
//							DisplayString(7,4,0,(u8 *)NodeTypeList[y]);
//						}
//						break;
					case 40://�ų��쳣�¼���¼
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC40);//��ʾ�˵�
						//����ʱ��

						//��/ֹ��־���������
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+7,2);
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+16*2);
						if(y>>15)
						{
							DisplayString(4,10,0,"����");
						}
						else
						{
							DisplayString(4,10,0,"�ָ�");
						}
						y=hex_bcd(y&0x7fff);
						DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�豸����
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+9,1);
						if(y>3)
						{//03H-FFH����
							y=3;
						}
						DisplayString(6,4,0,(u8 *)DeviceTypeList[y]);
						//�豸��ַ
						p16=(u16 *)(ADDR_STRINGBUFF+8*84+4+4*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,6);
						p16=DisplayData_OffZero(y,12,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//�ų��쳣����
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+16,1);
						if(y<3)
						{
							DisplayString(9,10,0,(u8 *)AbnormalTypeList[y]);
						}
						break;
					case 41:
						DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)ERC41);//��ʾ�˵�
						//����ʱ��
						
						//�������
						p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+10*2);
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+2,2);
						y=hex_bcd(y&0xfff);
						p16=DisplayData_OffZero(y,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
						//��ʱǰʱ��
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+4,6);
						y&=0xff1fffffffff;
						p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(5-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						p16=(u16 *)(ADDR_STRINGBUFF+4*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(2-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+':';
						p16[5]=0x2000+':';
						//��ʱ��ʱ��
						y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+10,6);
						y&=0xff1fffffffff;
						p16=(u16 *)(ADDR_STRINGBUFF+5*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(5-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+'-';
						p16[5]=0x2000+'-';
						p16=(u16 *)(ADDR_STRINGBUFF+6*84+4+10*2);
						for(i=0;i<3;i++)
						{
							DisplayData((y>>(8*(2-i)))&0xff,2,0,p16+3*i);
						}
						p16[2]=0x2000+':';
						p16[5]=0x2000+':';
						break;
				}
				//�¼�����ʱ��
				if((((MenuCount->v3)&0xff)!=41) && (((MenuCount->v3)&0xff)!=14))
				{//ERC41��ʱ�¼���¼�޷���ʱ�䡢ERC14�ն�ͣ�ϵ��¼���¼���ϱ�׼��һ��������ʱ��д���������
					y=MRR(ADDR+(LEN_TERMINAL_EVENT*(x-1))+2,5);
					p16=(u16 *)(ADDR_STRINGBUFF+2*84+4+10*2);
					for(i=0;i<3;i++)
					{
						DisplayData((y>>(8*(4-i)))&0xff,2,0,p16+3*i);
					}
					p16[2]=0x2000+'-';
					p16[5]=0x2000+'-';
					p16=(u16 *)(ADDR_STRINGBUFF+3*84+4+10*2);
					for(i=0;i<2;i++)
					{
						DisplayData((y>>(8*(1-i)))&0xff,2,0,p16+3*i);
					}
					p16[2]=0x2000+':';
				}
				//����� ? ��
				p8=(u8*)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+10*2);
				bcd_p8(p8,MenuCount->v2,3);
				p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+16*2);
				p16[0]=0x2020;
				p16[1]=0x2020;
				p16[2]=32;//'���Ҽ�ͷ'
				p16[3]=33;
			}
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,12,10);
			break;
		case 1://����1:ѡ��
			NdigitSelect(3,10,1,(MenuCount->v3)>>8);
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuCount->v2 = ((MenuCount->v3)&0xff);
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x20;//��ʹ�ú���0
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//�¼�
					switch((MenuCount->v3)&0xff)
					{
						case 17:
						case 19:
							CreateScreen((u32)Menu_1_5_1_1);
							break;
					}
				}
				Comm_Ram->ButtonVal&=~0x01;//��ʹ�ú���0
			}
			break;
		default:
			break;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1_5
// Descriptions:        �¼���¼����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1_5(void)
{
	u8 *p8;
	u16 *p16;
	u32 i;
	u32 j;
	u32 m=0;
	u32 n=0;
	u32 y;
	u32 EventNumMax=41;//�¼���¼������ֵ

	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayClr();
			DisplayString(1,0,0,(u8 *)EventList[0]);
			Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
			if(MenuCount->v2>EventNumMax)
			{//MenuCount->v2��¼�¼����
				MenuCount->v2=EventNumMax;
			}
			if(MenuCount->v2<1)
			{
				MenuCount->v2=1;
			}
			p8=(u8*)(ADDR_STRINGBUFF+1*84+4+9*2);
			bcd_p8(p8,MenuCount->v2,2);
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+16*2);
			p16[0]=0x2020;
			p16[1]=0x2020;
			p16[2]=32;//'���Ҽ�ͷ'
			p16[3]=33;
			NegativeLnMaxColMinColN(1,10,9);
			break;
		case 1://����1:ѡ��
			if(Stack->MenuStack[MenuCount->Count].EditRow==1)
			{
				NdigitSelect(2,9,1,EventNumMax);
			}
			if(Stack->MenuStack[MenuCount->Count].EditCol==0)
			{
				if((Comm_Ram->ButtonVal&0x01)!=0)
				{//���Ƽ�����
					if(Stack->MenuStack[MenuCount->Count].EditRow<3)
					{
						Stack->MenuStack[MenuCount->Count].EditRow++;
					}
				}
				if((Comm_Ram->ButtonVal&0x02)!=0)
				{//���Ƽ�����
					if(Stack->MenuStack[MenuCount->Count].EditRow>1)
					{
						Stack->MenuStack[MenuCount->Count].EditRow--;
					}
				}
				if(Stack->MenuStack[MenuCount->Count].EditRow==1)
				{
					p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+16*2);
					p16[0]=0x2020;
					p16[1]=0x2020;
					p16[2]=32;//'���Ҽ�ͷ'
					p16[3]=33;
					NegativeLnMaxColMinColN(Stack->MenuStack[MenuCount->Count].EditRow,10,9);
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x23;//��ʹ�ú���0
			break;
		default:
			break;
	}
	DisplayClrRow(0,0);
	DisplayClrRow(2,3);
	if(MenuCount->v2<=EventNumMax && MenuCount->v2>=1)
	{
		for(i=0;i<=1;i++)
		{
			DisplayString(2+i,0,0,(u8 *)EventList[i+1]);
		}
		DisplayString(0,0,0,(u8 *)LogoutList[MenuCount->v2-1]);
		//��Ҫ�¼���¼
		for(i=1;i<=256;i++)
		{
			y=MRR(ADDR_AFN0EF1+(LEN_TERMINAL_EVENT*(i-1)),1);//i:1~256����i����Ҫ�¼���¼ERC����
			if(y==MenuCount->v2)
			{
				m++;//��¼��Ҫ�¼�ERCy�ĸ���
			}
		}
		p16=(u16*)(ADDR_STRINGBUFF+2*84+4+9*2);
		m=hex_bcd(m);
		p16=DisplayData_OffZero(m,3,0,p16);
		//һ���¼���¼
		for(i=1;i<=256;i++)
		{
			y=MRR(ADDR_AFN0EF2+(LEN_TERMINAL_EVENT*(i-1)),1);//i:1~256����i��һ���¼���¼ERC����
			if(y==MenuCount->v2)
			{
				n++;//��¼һ���¼�ERCy�ĸ���
			}
		}
		p16=(u16*)(ADDR_STRINGBUFF+3*84+4+9*2);
		n=hex_bcd(n);
		p16=DisplayData_OffZero(n,3,0,p16);
		if(Stack->MenuStack[MenuCount->Count].EditRow!=1)
		{
			p16=(u16 *)(ADDR_STRINGBUFF+1*84+4+18*2);
			p16[0]=0;
			p16[1]=0;
			for(j=0;j<10;j++)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+(j*84)+4);
				for(i=0;i<20;i++)
				{
					p16[i]&=0x7fff;
				}
			}
			p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4);
			for(i=0;i<20;i++)
			{
				p16[i]|=0x8000;
			}														
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow)
				{
					case 2:
						m=bcd_hex(m);
						if(m)
						{
							MenuCount->v3=(m<<8)+MenuCount->v2;
							MenuCreate((u32)Menu_1_5_1);//�����¼��˵�
						}
						break;
					case 3:
						n=bcd_hex(n);
						if(n)
						{
							MenuCount->v3=(n<<8)+MenuCount->v2;
							MenuCreate((u32)Menu_1_5_1);//�����¼��˵�
						}
						break;
				}
			}
			Comm_Ram->ButtonVal&=~0x10;//��ʹ�ú���0
		}
	}
	else
	{
		DisplayString(0,0,0,"��ЧERC��");
	}
	DispalyAlign(0,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
}
#endif


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_1
// Descriptions:        ʵʱ���ݲ˵�����
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_1(void)//�˵�1
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{
		case 0://����0:��ʼ��
			Stack->MenuStack[MenuCount->Count].Task=1;
			DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_1);//��ʾ�˵�
			Arrowhead();
			MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
			break;
		case 1://����1:ѡ��
			MenuSelect();//�˵�ѡ��
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
				switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
				{
					case 1:
						MenuCreate((u32)Menu_1_1);//�����¼��˵�
						break;
					case 2:
						MenuCreate((u32)Menu_1_2);//�����¼��˵�
						break;
					case 3:
					//	MenuCreate((u32)Menu_1_3);//�����¼��˵�
					//	break;
					//case 4:
						MenuCreate((u32)Menu_1_4);//�����¼��˵�
						break;
					case 5:
						//MenuCreate((u32)Menu_1_5);
						break;
					default:
						break;
				}
			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x33;//��ʹ�ú���0
			break;
		default:
			break;
	}
}


#endif

