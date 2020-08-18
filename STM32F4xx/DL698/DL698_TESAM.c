

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_TESAM.h"
#include "../DL698/DL698_GET.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/IC_TESAM.h"
#include "../Device/MEMRW.h"



u32 GET_ESAM_OAD(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 offset;
	u32 Len;
	UARTCtrl_TypeDef *UARTCtrl;
	u8 *p8esamrx;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	p8esamrx=(u8 *)(ADDR_UART7Rx);
	switch((OAD>>8)&0xff)
	{
		default:
			UARTCtrl->DAR=6;//���󲻴���
			return 0;
		case 1://�߼���	��static��
			if(LENmax_TxSPACE<4)
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			p8tx[0]=DataType_octet_string;//long-unsigned
			p8tx[1]=2;
			p8tx[2]=OAD>>24;
			p8tx[3]=OAD>>16;
			return 4;
		case 2://ESAM���к�	��static��
//0 ESAM ���к�(8B )
//8 ESAM �汾�ţ� 4B ��
//12 �Գ� ��Կ�汾��16 B��
//28 ֤�� �汾 ����վ֤��汾�� 1B+�ն�֤��汾�� 1B��
//30 �Ự ʱЧ���ޣ� 4B��
//34 �Ự ʱЧʣ��䣨 4B��
//��ǰ������
//38 ASCTR������ַӦ��Э�̼������� 4B��
//42 ARCTR�������ϱ������� ��4B��
//46 AGSEQ ��Ӧ�ù㲥ͨ�����к� ��4B��
//50 �ն�֤�����к�(16B )
//66 ��վ֤�����кţ� 16 B��
			Len=8;
			offset=0;
			break;
		case 3://ESAM�汾��	��static��
			Len=4;
			offset=8;
			break;
		case 4://�Գ���Կ�汾	��static��
			Len=16;
			offset=12;
			break;
		case 5://�ỰʱЧ����	��static��
			Len=4;
			offset=30;
			break;
		case 6://�ỰʱЧʣ��ʱ��	��dyn.��
			Len=4;
			offset=34;
			break;
		case 7://��ǰ������	��static��
			offset=38;
			if(LENmax_TxSPACE<17)
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			i=TESAM_CMD_TxRx(0x803600FF,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{
				UART7Ctrl->Task=0;//�ص�Դ����
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;
			}
			p8tx[0]=DataType_structure;
			p8tx[1]=3;
			p8tx[2]=DataType_double_long_unsigned;
			MR((u32)p8tx+3,ADDR_UART7Rx+5+offset,4);
			p8tx[7]=DataType_double_long_unsigned;
			MR((u32)p8tx+8,ADDR_UART7Rx+5+offset+4,4);
			p8tx[12]=DataType_double_long_unsigned;
			MR((u32)p8tx+13,ADDR_UART7Rx+5+offset+8,4);
			return 17;
		case 8://֤��汾	��static��
			Len=2;
			offset=28;
			break;
		case 9://�ն�֤�����к�	��static��
			Len=16;
			offset=50;
			break;
		case 11://��վ֤�����к�	��static��
			Len=16;
			offset=66;
			break;
		case 10://�ն�֤��	��static��
			i=TESAM_CMD_TxRx(0x8036000B,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{
				UART7Ctrl->Task=0;//�ص�Դ����
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;
			}
			Len=(p8esamrx[3]<<8)+p8esamrx[4];//�������ݳ���
			if(LENmax_TxSPACE<(Len+4))
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			p8tx[0]=DataType_octet_string;
			p8tx[1]=0x82;
			p8tx[2]=Len>>8;
			p8tx[3]=Len;
			MR((u32)p8tx+4,ADDR_UART7Rx+5,Len);
			return Len+4;
		case 12:// ��վ֤��	��static��
			i=TESAM_CMD_TxRx(0x8036000C,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{
				UART7Ctrl->Task=0;//�ص�Դ����
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;
			}
			Len=(p8esamrx[3]<<8)+p8esamrx[4];//�������ݳ���
			if(LENmax_TxSPACE<(Len+4))
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			p8tx[0]=DataType_octet_string;
			p8tx[1]=0x82;
			p8tx[2]=Len>>8;
			p8tx[3]=Len;
			MR((u32)p8tx+4,ADDR_UART7Rx+5,Len);
			return Len+4;
		case 13://ESAM��ȫ�洢�����б�	��static��
//��Ҫ�洢��ESAM�еĶ���,ESAM��ȫ�洢�����б��=array OAD
			
			UARTCtrl->DAR=6;//���󲻴���
			return 0;
	}
	if(LENmax_TxSPACE<(Len+2))
	{
		GET_Request_Frame(PORTn);//��֡��־
		return 0;
	}
	i=TESAM_CMD_TxRx(0x803600FF,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
	if(i)
	{
		UART7Ctrl->Task=0;//�ص�Դ����
		UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
		return 0;
	}
	if((((OAD>>8)&0xff)==5)||(((OAD>>8)&0xff)==6))
	{
		p8tx[0]=DataType_double_long_unsigned;
		MR((u32)p8tx+1,ADDR_UART7Rx+5+offset,Len);
		return Len+1;
	}
	else	
	{
		p8tx[0]=DataType_octet_string;
		p8tx[1]=Len;
		MR((u32)p8tx+2,ADDR_UART7Rx+5+offset,Len);
		return Len+2;
	}
//	p8tx[0]=DataType_octet_string;
//	p8tx[1]=Len;
//	MR((u32)p8tx+2,ADDR_UART7Rx+5+offset,Len);
//	return Len+2;
}





















