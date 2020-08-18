

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_F000.h"
#include "../DL698/RM_TASK.h"
#include "../Device/IC_TESAM.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../../STM32F4xx/MS/MS.h"




//TESAM����
u32 ACTION_F100(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 LEN_TypeByte1;
	u32 LEN_Data1;
	u32 LEN_TypeByte2;
	u32 LEN_Data2;
	u32 DI;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
//	LenTx=0;
	switch(i)
	{
		case 1://1	��λ��������
//������=integer��0��	ͨ�÷�����instance-specific��
			UART7Ctrl->Task=0;
			LenRx=2;
			return (LenRx<<16)+0;
		case 2://2	ִ�У�������
//������=Data	ͨ�÷�����instance-specific��
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 3://3	ESAM������������
//������=SID
//Ӧ���=octet-string	��ESAM���ݵĲ��������ܱ�͸����ESAM��
			
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 4://4	���ݸ��£�������
//������=structure
//{
//��������   octet-string��
//������֤�� SID_MAC
//}	���ݡ�����MAC�ȷ���ESAM��֤����֤�ɹ��������õ��նˡ����ܱ��С�
//�������ݸ�ʽ���壺
//4�ֽ�OAD+ 1�ֽ�����LEN + ���ݣ���esam�ļ��ṹ����
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 5://5	Э��ʧЧ��������
//������=NULL
//Ӧ���=��ǰ����ʱ��   date_time_s	
			if(LENmax_TxSPACE<7)
			{
				UARTCtrl->DAR=8;//Խ��(8)
				return 0;
			}
			MR((u32)p8tx,ADDR_DL698YMDHMS,7);
			LenRx=1;
			LenTx=7;
			UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
			return (LenRx<<16)+LenTx;
		case 6://6	Ǯ��������������
//������=structure
//{
//  ��������   integer��
//������   double-long-unsigned��
//�������   double-long-unsigned��
//����       octet-string��
//������֤�� SID_MAC��
//���       octet-string
//}	�������ͣ�0������ 1����ֵ 2-�˷�
//����ʱ������Ҫ��֤�ͻ���ţ�ֱ�ӽ��ͻ����д��ESAM�Ķ�Ӧ�ļ��У��ٽ��г�ֵ������
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 7://7	��Կ���£�������
//������=structure
//F1 00 07 00 02 02
//{
//��Կ����   octet-string��
//09 82 07 20 ...
//������֤�� SID_MAC
//5E 81 2E 00 00 0B 07 2D 00 00 00 00 00 00 00 00 39 04 95 65 D6 06 00 AB CD 16
//}	
		//����TESAM:��ȫ��ʶ+��������AttachData+����Endata1+MAC
			//������Կ����Endata1����
			LEN_TypeByte1=Get_DL698ByteTypeLenByte(p8rx+7);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			LEN_Data1=Get_DL698ByteTypeLenValue(p8rx+7);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			//��ȫ��ʶ
			DI=MRR((u32)p8rx+7+LEN_TypeByte1+LEN_Data1+1,4);
			DI=R_Inverse(DI,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
			if(DI!=0x812E0000)
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			//ȡ��������
			LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			MR(ADDR_UART7Tx+7,(u32)p8rx+7+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
			//ȡ��Կ����Endata1
			MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+7+LEN_TypeByte1,LEN_Data1);
			//ȡMAC
			MR(ADDR_UART7Tx+7+LEN_Data2-2+LEN_Data1,(u32)p8rx+7+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2+1,4);
			i=LEN_Data2-2+LEN_Data1+4;
			i=TESAM_CMD_TxRx(0x812E0000,i,ADDR_UART7Tx+7);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			LenRx=3+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2+5;
			return (LenRx<<16)+0;
		case 8://8	֤����£�������
//������=structure
//F1 00 08 00 02 02
//{
//֤������  octet-string��
//09 82 05 F0 ...
//��ȫ��ʶ  SID
//5D 81 30 02 03 02 05 F0
//}	
		//����TESAM:��ȫ��ʶ+��������AttachData+����Endata1
			//������Կ����Endata1����
			LEN_TypeByte1=Get_DL698ByteTypeLenByte(p8rx+7);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			LEN_Data1=Get_DL698ByteTypeLenValue(p8rx+7);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			//��ȫ��ʶ
			DI=MRR((u32)p8rx+7+LEN_TypeByte1+LEN_Data1+1,4);
			DI=R_Inverse(DI,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
			if(DI!=0x81300203)
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			//ȡ��������
			LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			MR(ADDR_UART7Tx+7,(u32)p8rx+7+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
			//ȡ��Կ����Endata1
			MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+7+LEN_TypeByte1,LEN_Data1);
			i=LEN_Data2-2+LEN_Data1;
			i=TESAM_CMD_TxRx(0x81300203,i,ADDR_UART7Tx+7);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			LenRx=3+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2;
			return (LenRx<<16)+0;
		case 9://9	����Э��ʱЧ��������
//������=structure 
//{
//��������  octet-string��
//��ȫ��ʶ  SID
//} 
		//����TESAM:��ȫ��ʶ+��������AttachData+����Endata1
			//������Կ����Endata1����
			LEN_TypeByte1=Get_DL698ByteTypeLenByte(p8rx+7);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			LEN_Data1=Get_DL698ByteTypeLenValue(p8rx+7);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			//��ȫ��ʶ
			DI=MRR((u32)p8rx+7+LEN_TypeByte1+LEN_Data1+1,4);
			DI=R_Inverse(DI,4);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
			if(DI!=0x81340105)
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			//ȡ��������
			LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+7+LEN_TypeByte1+LEN_Data1+5);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
			if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			MR(ADDR_UART7Tx+7,(u32)p8rx+7+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
			//ȡ��Կ����Endata1
			MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+7+LEN_TypeByte1,LEN_Data1);
			i=LEN_Data2-2+LEN_Data1;
			i=TESAM_CMD_TxRx(0x81340105,i,ADDR_UART7Tx+7);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{
				UARTCtrl->DAR=22;//ESAM��֤ʧ��        ��22��
				return 0;//����
			}
			LenRx=3+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2;
			return (LenRx<<16)+0;
		case 10://10	Ǯ����ʼ����������
//������=structure
//{
//  Ԥ�ý��   double-long-unsigned��
//������֤�� SID_MAC
//}	
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		default:
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
	}
}


//��ȫģʽ����
u32 ACTION_F101(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 1://����1����λ��������������=integer��0����λʱ���������3��
			MC(0,ADDR_F101_3,2);
			LenRx=2;
			return (LenRx<<16)+LenTx;
		case 127://����127��������ʽ��ȫģʽ�����������ʶ��Ȩ�ޣ������ʶ��=OI��Ȩ�ޡ�=long-unsigned
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[3]<<8)+p8rx[4];
			MR(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_F101_3)
			{
				Array=0;
				MC(0,ADDR_F101_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_F101_3;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�,�滻
				MR((u32)p8,(u32)p8rx,LENper_F101_3);
				MW(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			}
			else
			{//����
				if(Array<NUMmax_F101_3)
				{
					MR((u32)p8,(u32)p8rx,LENper_F101_3);
					Array++;
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array;
					MW(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
				}
				else
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
				}
			}
			return (LenRx<<16)+LenTx;
		case 128://����128��ɾ����ʽ��ȫģʽ�����������ʶ�������ʶ��=OI
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			//����ͬ��OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_F101_3)
			{
				Array=0;
				MC(0,ADDR_F101_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_F101_3;
			}
			if((i<Array)&&(Array!=0))
			{//�ҵ�,ɾ��
				MR((u32)p8,(u32)p8+LENper_F101_3,LENmax_F101_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			}
			else
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
			}
			return (LenRx<<16)+LenTx;
		case 129://����129������������ʽ��ȫģʽ������array ��ȫģʽ������
//��ȫģʽ������=structure
//{
//�����ʶ    OI��
//��ȫģʽ    long-unsigned
//}
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx+=i;
			Method=p8rx[1];//���Ӹ���
			p8rx+=2;
			MR(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			while(Method--)
			{
				OI=(p8rx[3]<<8)+p8rx[4];
				//����ͬ��OI
				p8=(u8*)ADDR_128KDATABUFF;
				Array=p8[1];
				if(Array>NUMmax_F101_3)
				{
					Array=0;
				}
				p8+=2;
				for(i=0;i<Array;i++)
				{
					if(OI==((p8[3]<<8)+p8[4]))
					{
						break;
					}
					p8+=LENper_F101_3;
				}
				if((i<Array)&&(Array!=0))
				{//�ҵ�,�滻
					MR((u32)p8,(u32)p8rx,LENper_F101_3);
				}
				else
				{//����
					if(Array<NUMmax_F101_3)
					{
						MR((u32)p8,(u32)p8rx,LENper_F101_3);
						Array++;
						p8=(u8*)ADDR_128KDATABUFF;
						p8[0]=DataType_array;
						p8[1]=Array;
					}
					else
					{
						UARTCtrl->DAR=8;//Խ��                 ��8��
					}
				}
				p8rx+=LENper_F101_3;
			}
			MW(ADDR_128KDATABUFF,ADDR_F101_3,LENmax_F101_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}
//RS232
u32 ACTION_F200(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	LenTx=0;
	switch(i)
	{
		case 127://����127�����ö˿ڣ��˿ںţ��˿ڲ������˿ڹ��ܣ��˿ںš�=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx>LENper_F200_2)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return (LenRx<<16)+LenTx;
			}
				//�˿ڡ�=structure
				//{
				//�˿ں�     OAD
				//�˿ڲ���   COMDCB��
				//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
				//}
//			OAD=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];
//			if(OAD==0xF2000201)
//			{
			if((p8rx[0]!=DataType_structure)&&(p8rx[0]!=DataType_array))
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			i=p8rx[9];
			if(i>2)
			{//У��λ:��У�飨0������У�飨1����żУ�飨2��
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			
			MW((u32)p8rx,ADDR_F200_2,LENper_F200_2);
			return (LenRx<<16)+LenTx;
//			}
//			UARTCtrl->DAR=6;//���󲻴���           ��6��
//			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}
//RS485
u32 ACTION_F201(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127�����ö˿ڣ��˿ںţ��˿ڲ������˿ڹ��ܣ��˿ںš�=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx>LENper_F201_2)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
				//�˿ڡ�=structure
				//{
				//�˿ں�     OAD
				//�˿ڲ���   COMDCB��
				//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
				//}
//			OAD=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];
//			if(OAD==0xF2010201)
			if(p8rx[6]==1)
			{//RS485-1
				MW((u32)p8rx,ADDR_F201_2,LENper_F201_2);
				return (LenRx<<16)+LenTx;
			}
//			if(OAD==0xF2010202)
			if(p8rx[6]==2)
			{//RS485-2
				MW((u32)p8rx,ADDR_F201_2+LENper_F201_2,LENper_F201_2);
				return (LenRx<<16)+LenTx;
			}
//			if(OAD==0xF2010203)
			if(p8rx[6]==3)
			{//RS485-3
				MW((u32)p8rx,ADDR_F201_2+(2*LENper_F201_2),LENper_F201_2);
				return (LenRx<<16)+LenTx;
			}
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}
//����
u32 ACTION_F202(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127�����ö˿ڣ��˿ںţ��˿ڲ������˿ڹ��ܣ��˿ںš�=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx>LENper_F202_2)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return (LenRx<<16)+LenTx;
			}
				//�˿ڡ�=structure
				//{
				//�˿ں�     OAD
				//�˿ڲ���   COMDCB��
				//�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��}
				//}
//			OAD=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];
//			if(OAD==0xF2020201)
//			{
				MW((u32)p8rx,ADDR_F202_2,LENper_F202_2);
				return (LenRx<<16)+LenTx;
//			}
//			UARTCtrl->DAR=6;//���󲻴���           ��6��
//			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}
//�̵������
u32 ACTION_F205(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127���޸Ŀ������ԣ��̵����ţ��������ԣ��̵����š�=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx>LENper_F205_127)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return (LenRx<<16)+LenTx;
			}
			Method=p8rx[6];//�̵�����
//			OAD=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];
			if((Method==0)||(Method>NUMmax_F205_127))
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
				return (LenRx<<16)+LenTx;
			}
			Method--;
			MW((u32)p8rx,ADDR_F205_127+(Method*LENper_F205_127),LENper_F205_127);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//�๦�ܶ���
u32 ACTION_F207(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127���޸Ĺ���ģʽ��·�ţ�����ģʽ��·�š�=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx>LENper_F207_127)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return (LenRx<<16)+LenTx;
			}
			MW((u32)p8rx,ADDR_F207_127,LENper_F207_127);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//�ز�/΢�������߽ӿ�
#define NUMmax_ReTx     3//�������ط�����
u32 ACTION_F209(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	UARTCtrl_TypeDef *UARTCtrlProxy;
	u32 NUM_ReTx;
	u32 offset;
	u8 *p8;
	u16 *p16;
	#ifndef LINUX
	u32 *p32;	
	#else
	ms_Type* ms;//�õ�ms���ַ
	#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	LenTx=0;
	switch(i)
	{
		case 127://����127��͸��ת����������
//������=structure
//{
//  ͨ�ŵ�ַ                     TSA��
//  ���յȵ����ĳ�ʱʱ�䣨�룩   long-unsigned��
//  ͸��ת������                 octet-string
//}
//���ؽ����=octet-string
			p8rx+=4;
			LEN_RxAPDU-=4;
//02 03 55 07 05 00 00 00 00 00 13 12 00 14 09 19 68 17 00 43 05 13 00 00 00 00 00 10 54 74 05 01 00 ...
			p16=(u16*)(ADDR_DATABUFF);
			p16[0]=0;
			p16[1]=0;
			p16[2]=0;
			MR(ADDR_DATABUFF,(u32)p8rx+5,6);
			UARTCtrl->RMAddr[0]=p16[0];
			UARTCtrl->RMAddr[1]=p16[1];
			UARTCtrl->RMAddr[2]=p16[2];

			LenRx=p8rx[3];
			LenRx+=4;
			offset=LenRx;//��ʱʱ���ַ
			LenRx+=3;
			i=p8rx[LenRx+1];
			if((i&0x80)==0)
			{
				i+=2;
				LenRx+=i;
			}
			else
			{
				i&=0x7f;
				if(i==1)
				{
					i=p8rx[LenRx+2];
					i+=3;
					LenRx+=i;
				}
				else
				{
					i=(p8rx[LenRx+2]<<8)|p8rx[LenRx+3];
					i+=4;
					LenRx+=i;
				}
			}
			
			i=AddrToMeterSetNo((u16*)(ADDR_DATABUFF));//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=RMNmax��ʾ��Ч
			if(i>=NUM_RMmax)
			{//û�ҵ��������
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
			if(p8[0]!=DataType_structure)
			{//������Ч
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			p8=Get_Element(p8+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8==0)
			{//û�ҵ��˿ں�
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			i=(p8[1]<<24)+(p8[2]<<16)+(p8[3]<<8)+p8[4];//�˿�OAD
			i=OADtoPORTn(i);//OADֵתΪͨ�ſں�
			if(i!=RS485_4PORT)
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(i);
			if(UARTCtrlProxy->Function==0)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			UARTCtrlProxy->ProxyPORTn=PORTn;//�������Ķ˿ں�
			//���յȵ����ĳ�ʱʱ�䣨�룩
			p16=(u16*)Get_ADDR_UARTnTimer_S(PORTn);//�õ�UART�붨ʱ����ַ
			i=(p8rx[offset+1]<<8)|p8rx[offset+2];
			p16[0]=i;
			offset+=3;
			i=p8rx[offset+1];
			if((i&0x80)==0)
			{
				offset+=2;
			}
			else
			{
				i&=0x7f;
				if(i==1)
				{
					i=p8rx[offset+2];
					offset+=3;
				}
				else
				{
					i=(p8rx[offset+2]<<8)|p8rx[offset+3];
					offset+=4;
				}
			}
			//if(i>LEN_UART2FnDataBuff)
			if(i>LEN_UART2Tx)
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			p8=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
			MR((u32)p8,(u32)p8rx+offset,i);
			UARTCtrlProxy->ProxyTxRxByte=i;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
			UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
			NUM_ReTx=0;//�����ط�����
			while(1)
			{
			#ifndef LINUX
				p32=(u32*)ADDR_MS_ENTRY;
				(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
			#else
				ms=(ms_Type *)Get_ms();
				ms->Main_Thread();
			#endif
				if(UARTCtrlProxy->ProxyTask==0)//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
				{
					if(UARTCtrlProxy->ProxyTxRxByte==0)
					{//�����ݽ���
						NUM_ReTx++;//�����ط�����
						if(NUM_ReTx<NUMmax_ReTx)
						{
							UARTCtrlProxy->ProxyTxRxByte=i;//���������ֽ���,���ͺͽ��պ������ڷ���ڵ�FnDataBuff
							UARTCtrlProxy->ProxyTask=1;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
							continue;
						}
					}
					break;
				}
			}
			//����������
			i=UARTCtrlProxy->ProxyTxRxByte;
			if((i+4)>LENmax_TxSPACE)
			{
				p8tx[LenTx]=0;
				LenTx+=1;
				return (LenRx<<16)+LenTx;
			}
			p8tx[LenTx]=DataType_octet_string;
			LenTx+=1;
			if(i<=127)
			{
				p8tx[LenTx]=i;
				LenTx+=1;
			}
			else
			{
				p8tx[LenTx+0]=0x82;
				p8tx[LenTx+1]=i>>8;
				p8tx[LenTx+2]=i;
				LenTx+=3;
			}
			MR((u32)p8tx+LenTx,(u32)p8,i);
			LenTx+=i;
			return (LenRx<<16)+LenTx;
			
		case 128://����128�����ö˿ڲ������˿ںţ�ͨ�Ų������˿ںš�=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx>LENper_F209_128)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return (LenRx<<16)+LenTx;
			}
			MW((u32)p8rx,ADDR_F209_128,LENper_F209_128);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}


//����
u32 ACTION_F20B(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
//	u32 OAD;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127�����ö˿ڲ������˿ںţ�ͨ�Ų������˿ںš�=OAD
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			if(LenRx>LENper_F20B_127)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return (LenRx<<16)+LenTx;
			}
			MW((u32)p8rx,ADDR_F20B_127,LENper_F20B_127);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}




