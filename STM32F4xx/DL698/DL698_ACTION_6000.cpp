
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_6000.h"
#include "../DL698/SearchMeter_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../DL698/TASK.h"
#include "../Terminal698/Terminal_Init.h"


u32 ACTION_Add(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 LenRx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(p8rx[4+0]==DataType_array)
	{//���һ��
		Array=p8rx[4+1];
		if(Array&0x80)
		{
			Array&=0x7f;
			if(LEN_RxAPDU<=(4+2+Array))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2+Array;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2+Array;
			i=Array;
			Array=0;
			while(i--)
			{
				Array<<=8;
				Array|=p8rx[0];
				p8rx+=1;
			}
		}
		else
		{
			if(LEN_RxAPDU<=(4+2))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2;
		}
	}
	else
	{//��Ӹ���һ��
		if(LEN_RxAPDU<=(4+2))
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		Array=1;
		LenRx=0;
		p8rx+=4;
		LEN_RxAPDU-=4;
	}
	while(Array--)
	{
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2[];//�¼��ɼ�����
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_601C[];
		if((pLib==DL698_SETLIB_6016_2)||(pLib==DL698_SETLIB_601C))
		{
			//�ϱ�����6016,601C��data�������ͣ����Ƚ���������
			i=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		}
		else
		{
			i=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			if((i&0xff)==0)
			{
				UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
				return 0;
			}
			i>>=8;
		}
		if(LEN_RxAPDU<i)
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		LenRx+=i;
		if(i>pLib->LENper)
		{
			UARTCtrl->DAR=8;//Խ��                 ��8��
			return (LenRx<<16)+0;
		}
		if(p8rx[2]==DataType_unsigned)
		{
			n=p8rx[3];//ȡ�������
		}
		else
		{
			n=(p8rx[3]<<8)+p8rx[4];//ȡ�������
		}
		if(n>=(pLib->LENmax/pLib->LENper))
		{
			UARTCtrl->DAR=8;//Խ�磨8��
			return (LenRx<<16)+0;
		}
		MW((u32)p8rx,pLib->Addr+(n*pLib->LENper),i);
		p8rx+=i;
	}
	return (LenRx<<16)+0;
}


u32 ACTION_Add_6000(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 LenRx;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(p8rx[4+0]==DataType_array)
	{//���һ��
		Array=p8rx[4+1];
		if(Array&0x80)
		{
			Array&=0x7f;
			if(LEN_RxAPDU<=(4+2+Array))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2+Array;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2+Array;
			i=Array;
			Array=0;
			while(i--)
			{
				Array<<=8;
				Array|=p8rx[0];
				p8rx+=1;
			}
		}
		else
		{
			if(LEN_RxAPDU<=(4+2))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2;
		}
	}
	else
	{//��Ӹ���һ��
		if(LEN_RxAPDU<=(4+2))
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		Array=1;
		LenRx=0;
		p8rx+=4;
		LEN_RxAPDU-=4;
	}
	while(Array--)
	{
		//��鲨����
		p8=Get_Element(p8rx+5,2,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		if(p8)
		{
			if((p8[1]>10)&&(p8[1]!=255))
			{
				UARTCtrl->DAR=255;
				return 0;
			}
		}

		//����Լ����
	#if (USER%100)==0//̨�����
		p8=Get_Element(p8rx+5,3,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		if(p8)
		{
			if(p8[1]>4)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
		}
	#endif
		//�����߷�ʽ
		p8=Get_Element(p8rx+5,8,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		if(p8)
		{
			if(p8[1]>3)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
		}
		//��鵥Ԫ���ݳ���
		i=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
		if((i&0xff)==0)
		{
			UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
			return 0;
		}
		i>>=8;
		if(LEN_RxAPDU<i)
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		LenRx+=i;
		if(i>pLib->LENper)
		{
			UARTCtrl->DAR=8;//Խ��                 ��8��
			return (LenRx<<16)+0;
		}
		//����������
		if(p8rx[2]==DataType_unsigned)
		{
			n=p8rx[3];//ȡ�������
		}
		else
		{
			n=(p8rx[3]<<8)+p8rx[4];//ȡ�������
		}
		if(n>=(pLib->LENmax/pLib->LENper))
		{
			UARTCtrl->DAR=8;//Խ��                 ��8��
			return (LenRx<<16)+0;
		}
		MW((u32)p8rx,pLib->Addr+(n*pLib->LENper),i);//nand
		//MW((u32)p8rx,ADDR_6000_SDRAM+(n*pLib->LENper),i);//sdram
		p8rx+=i;
	}
	return (LenRx<<16)+0;
}

u32 ADDR_6000_Check(void)//ADDR_6000�������ܱ��ַ�ظ����;����:0=û�ظ�,1=���ظ�,���Զ�ɾ���ظ����
{
	u32 i;
	u32 n;
	u32 m;
	u32 f;
	u8* p8s;
	u8* p8d;
	u8* buff;
	ms_Type *ms;
	
	f=0;
	ms=Get_ms();
	p8s=(u8*)ms->ms_malloc(LENmax_6000);//��̬�����ҵ��ڴ��
	ms->ms_free(p8s);//�ͷ��ҵ��ڴ�ѷ���
	if(p8s)
	{
		buff=p8s;
		MR((u32)p8s,ADDR_6000,LENmax_6000);
		for(n=0;n<NUM_RMmax;n++)
		{
			if(p8s[0]==DataType_structure)
			{
				p8d=buff;
				for(m=0;m<NUM_RMmax;m++)
				{
					if(p8d[0]==DataType_structure)
					{
						for(i=0;i<6;i++)
						{
							if(p8s[10+i]!=p8d[10+i])
							{
								break;
							}
						}
						if(i>=6)
						{//�ظ�
							if(n!=m)
							{//����ͬһ���
								p8d[0]=0;//sdram
								MC(0,ADDR_6000+(m*LENper_6000),1);//nand
								f=1;
							}
						}
					}
					p8d+=LENper_6000;
				}
			}
			p8s+=LENper_6000;
		}
	}
	return f;
}



u32 ACTION_Del(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 LenRx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(p8rx[4+0]==DataType_array)
	{//ɾ��һ��
		Array=p8rx[4+1];
		if(Array&0x80)
		{
			Array&=0x7f;
			if(LEN_RxAPDU<=(4+2+Array))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2+Array;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2+Array;
			i=Array;
			Array=0;
			while(i--)
			{
				Array<<=8;
				Array|=p8rx[0];
				p8rx+=1;
			}
		}
		else
		{
			if(LEN_RxAPDU<=(4+2))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=2;
			p8rx+=4+2;
			LEN_RxAPDU-=4+2;
		}
	}
	else
	{//ɾ��һ��
		if(LEN_RxAPDU<=(4+2))
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		Array=1;
		LenRx=0;
		p8rx+=4;
		LEN_RxAPDU-=4;
	}
	while(Array--)
	{
		if(LEN_RxAPDU<2)
		{
			UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
			return 0;
		}
		if(p8rx[0]==DataType_unsigned)
		{
			n=p8rx[1];//ȡ�������
			p8rx+=2;
			LenRx+=2;
		}
		else
		{
			n=(p8rx[1]<<8)+p8rx[2];//ȡ�������
			p8rx+=3;
			LenRx+=3;
		}
		if(n<(pLib->LENmax/pLib->LENper))
		{
			MC(0,pLib->Addr+(n*pLib->LENper),1);
		}
	}
	return (LenRx<<16)+0;
}

//�ɼ��������ñ�
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6000_2_0[];
u32 ACTION_6000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 Method;
	u32 Array;
	u32 LenRx;
	u32 LenTx;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_6000_2_0;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127��Add���ɼ��������õ�Ԫ�����һ���ɼ��������õ�Ԫ�����õ�Ԫ����Ϣ�л�����Ϣ�����������ġ�
			if(p8rx[4]!=DataType_structure)
			{
				LenRx= Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				UARTCtrl->DAR=255;//���ݷ��ʽ��
				return (LenRx<<16)+LenTx;
			}
			i=ACTION_Add_6000((DL698_SETLIB_TypeDef *)DL698_SETLIB_6000_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
		#if (USER%100)!=0
			x=ADDR_6000_Check();//ADDR_6000�������ܱ��ַ�ظ����;����:0=û�ظ�,1=���ظ�,���Զ�ɾ���ظ����
			if(x)
			{
				UARTCtrl->DAR=255;//���ݷ��ʽ��
			}
		#endif
			return i;
		case 128://����128��AddBatch��array �ɼ��������õ�Ԫ��������Ӳɼ��������õ�Ԫ��
			i=ACTION_Add_6000((DL698_SETLIB_TypeDef *)DL698_SETLIB_6000_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
		#if (USER%100)!=0
			x=ADDR_6000_Check();//ADDR_6000�������ܱ��ַ�ظ����;����:0=û�ظ�,1=���ظ�,���Զ�ɾ���ظ����
			if(x)
			{
				UARTCtrl->DAR=255;//���ݷ��ʽ��
			}
		#endif
			return i;
		case 129://����129��Update��������ţ�������Ϣ���������õ�Ԫ�Ļ�����Ϣ����
			Method=(p8rx[4+3]<<8)+p8rx[4+4];//ȡ�������
			if(Method>=NUM_RMmax)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+LenTx;
			}
			if(LEN_RxAPDU<(4+5))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=5;
			p8rx+=4+5;
			LEN_RxAPDU-=4+5;
			LenRx=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			if((LenRx&0xff)==0)
			{
				UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
				return 0;
			}
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			//ԭ����
			i=Get_DL698DataLen((u8*)(ADDR_6000+(Method*LENper_6000)+5),(Attribute_TypeDef *)pLib->pDataType,2,LENper_6000);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			i>>=8;
			if((i>LENper_6000)||(i<2))
			{
				i=2;
			}
			//�������ݳ���1
			Array=Get_DL698DataLen((u8*)(ADDR_6000+(Method*LENper_6000)+5+i),(Attribute_TypeDef *)pLib->pDataType,11,LENper_6000);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			Array>>=8;
			if((Array>LENper_6000)||(Array<2))
			{
				Array=2;
			}
			//�������ݳ���2
			x=Get_DL698DataLen((u8*)(ADDR_6000+(Method*LENper_6000)+5+i+Array),(Attribute_TypeDef *)pLib->pDataType,16,LENper_6000);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			x>>=8;
			if((x>LENper_6000)||(x<2))
			{
				x=2;
			}
			x+=Array;
			if((LenRx+x)>LENper_6000)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+LenTx;
			}
			//������
			MR(ADDR_128KDATABUFF,(u32)p8rx,LenRx);
			//ԭ��������
			MR(ADDR_128KDATABUFF+LenRx,ADDR_6000+(Method*LENper_6000)+5+i,x);
			//д
			MW(ADDR_128KDATABUFF,ADDR_6000+(Method*LENper_6000)+5,LenRx+x);
			return (LenRx<<16)+LenTx;
		case 130://����130��Update��������ţ���չ��Ϣ��������Ϣ���������õ�Ԫ����չ��Ϣ�Լ�������Ϣ������ΪNULL��ʾ�����¡�
			Method=(p8rx[4+3]<<8)+p8rx[4+4];//ȡ�������
			if(Method>=NUM_RMmax)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
				return (LenRx<<16)+LenTx;
			}
			if(LEN_RxAPDU<(4+5))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=5;
			p8rx+=4+5;
			LEN_RxAPDU-=4+5;
	//��չ��Ϣ
			if(p8rx[0]==0)
			{//����ΪNULL��ʾ������
				if(LEN_RxAPDU<1)
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
				p8rx+=1;
				LenRx=1;
				LEN_RxAPDU-=1;
			}
			else
			{
				LenRx=Get_DL698DataLen(p8rx,0,11,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				if((LenRx&0xff)==0)
				{
					UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
					return 0;
				}
				LenRx>>=8;
				if(LEN_RxAPDU<LenRx)
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
				//ԭ����ƫ��
				Array=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+5,0,2,LENper_6000);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				Array>>=8;
				if((Array>LENper_6000)||(Array<2))
				{
					Array=2;
				}
				Array+=5;
				//ԭ����
				i=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+Array,0,11,LENper_6000);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				i>>=8;
				if((i>LENper_6000)||(i<2))
				{
					i=2;
				}
				//������ݳ���
				x=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+Array+i,0,16,LENper_6000);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				if((x>LENper_6000)||(x<2))
				{
					x=2;
				}
				if((Array+LenRx+x)>LENper_6000)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+LenTx;
				}
				//������
				MR(ADDR_128KDATABUFF,(u32)p8rx,LenRx);
				//ԭ��������
				MR(ADDR_128KDATABUFF+LenRx,ADDR_6000+(Method*LENper_6000)+Array+i,x);
				//д
				MW(ADDR_128KDATABUFF,ADDR_6000+(Method*LENper_6000)+Array,LenRx+x);
				p8rx+=LenRx;
				LEN_RxAPDU-=LenRx;
			}
		//������Ϣ
			if(p8rx[0]==0)
			{//����ΪNULL��ʾ������
				if(LEN_RxAPDU<1)
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
				p8rx+=1;
				LenRx=1;
				LEN_RxAPDU-=1;
			}
			else
			{
				LenRx=Get_DL698DataLen(p8rx,0,16,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				if((LenRx&0xff)==0)
				{
					UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
					return 0;
				}
				LenRx>>=8;
				if(LEN_RxAPDU<LenRx)
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
				//ԭ����ƫ��
				Array=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+5,0,2,LENper_6000);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				Array>>=8;
				if((Array>LENper_6000)||(Array<2))
				{
					Array=2;
				}
				Array+=5;
				i=Get_DL698DataLen((u8*)ADDR_6000+(Method*LENper_6000)+Array,0,11,LENper_6000);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				i>>=8;
				if((i>LENper_6000)||(i<2))
				{
					i=2;
				}
				Array+=i;
				if((Array+LenRx)>LENper_6000)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+LenTx;
				}
				//д
				MW((u32)p8rx,ADDR_6000+(Method*LENper_6000)+Array,LenRx);
				p8rx+=LenRx;
				LEN_RxAPDU-=LenRx;
			}
			return (LenRx<<16)+LenTx;
		case 131://����131��Delete��������ţ�ɾ�����õ�Ԫ��ͨ���������ɾ��
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6000_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 132://����132��Delete��������Ϣ��ɾ�����õ�Ԫ��ͨ��������Ϣ����ɾ��
			p8rx+=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<(4+LenRx))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			x=0;//�ܼ���
			while(1)
			{
				if(x>=NUM_RMmax)
				{
					break;
				}
				MR(ADDR_128KDATABUFF,ADDR_6000+(x*LENper_6000),LEN_128KDATABUFF);
				p8=(u8*)ADDR_128KDATABUFF+5;
				for(i=0;i<(LEN_128KDATABUFF/LENper_6000);i++)
				{
					if(x>=NUM_RMmax)
					{
						break;
					}
				//�Ƚ�
					for(Array=0;Array<LenRx;Array++)
					{
						if(p8rx[Array]!=p8[Array])
						{
							break;
						}
					}
					if(Array<LenRx)
					{
						p8+=LENper_6000;
						x++;
						continue;
					}
				//������
					MC(0,ADDR_6000+(x*LENper_6000),1);//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
					p8+=LENper_6000;
					x++;
				}
			}
			return (LenRx<<16)+LenTx;
		case 133://����133��Delete��ͨ�ŵ�ַ�� �˿ںţ�ɾ�����õ�Ԫ��ͨ��ͨ�ŵ�ַ���˿�ɾ��
			//structure
			//{
			//	TSA
			//	OAD
			//}
			p8rx+=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LenRx>>=8;
			if(LEN_RxAPDU<(4+LenRx))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			x=0;//�ܼ���
			while(1)
			{
				if(x>=NUM_RMmax)
				{
					break;
				}
				MR(ADDR_128KDATABUFF,ADDR_6000+(x*LENper_6000),LEN_128KDATABUFF);
				p8=(u8*)ADDR_128KDATABUFF;
				for(i=0;i<(LEN_128KDATABUFF/LENper_6000);i++)
				{
					if(x>=NUM_RMmax)
					{
						break;
					}
				//�Ƚ�TSA
					Method=2;//��������ƫ��
					Array=7;//FLASH����ƫ��
					if(p8rx[Method]==DataType_NULL)
					{
						Method++;//��������ƫ��
						Array+=p8[Array]+1;//FLASH����ƫ��
						Array+=4;
					}
					else
					{
						if(p8rx[Method]==DataType_TSA)
						{
							Method++;//��������ƫ��
							Array++;//FLASH����ƫ��
							LenTx=p8rx[Method]+1;//�Ƚ��ֽ���
							while(LenTx)
							{
								if(p8rx[Method]!=p8[Array])
								{
									break;
								}
								LenTx--;
								Method++;//��������ƫ��
								Array++;//FLASH����ƫ��
							}
							if(LenTx)
							{//����ͬ
								p8+=LENper_6000;
								x++;
								continue;
							}
							Array+=4;//FLASH����ƫ��
						}
						else
						{
							UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
							return 0;
						}
					}
				//�Ƚ�OAD
					if(p8rx[Method]!=DataType_NULL)
					{
						if(p8rx[Method]!=DataType_OAD)
						{
							UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
							return 0;
						}
						LenTx=4;
						while(LenTx)
						{
							LenTx--;
							Method++;//��������ƫ��
							Array++;//FLASH����ƫ��
							if(p8rx[Method]!=p8[Array])
							{//����ͬ
								break;
							}
						}
						if(LenTx)
						{//����ͬ
							p8+=LENper_6000;
							x++;
							continue;
						}
					}
					//������
					MC(0,ADDR_6000+(x*LENper_6000),1);//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
					p8+=LENper_6000;
					x++;
				}
			}
			LenTx=0;
			return (LenRx<<16)+LenTx;
		case 134://����134��Clear()��ղɼ��������ñ�
			if(p8rx[4]!=0)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			MC(0,ADDR_6000,LENper_6000*NUM_RMmax);
			LenRx=1;
			p8rx+=4+1;
			LEN_RxAPDU-=4+1;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//�ѱ�
u32 ACTION_6002(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	LenTx=0;
	LenRx=0;
	p8rx+=4;
	switch(i)
	{
		case 127://����127��ʵʱ�����ѱ��ѱ�ʱ����
//			MW((u32)p8rx,ADDR_6002_127,3);
//			LenTx=MRR(ADDR_6002_8+9,1);
//			if(LenTx==2)
//			{//ÿ���ѱ�ǰ���
//				DEL_SearchMeter_Record(3);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
//				DEL_SearchMeter_Record(4);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
//			}
			LenRx=(p8rx[1]<<8)|p8rx[2];
			if(LenRx==0)
			{//�Զ��ѱ����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ����
				Terminal_Ram->AutoSearch_M_Timer=65535;//2 �Զ��ѱ�����ʱ��ֶ�ʱ��
			}
			else
			{
				Terminal_Ram->AutoSearch_M_Timer=LenRx;//2 �Զ��ѱ�����ʱ��ֶ�ʱ��
			}
			
		#if EnRS485AutoSearchMeter==1//0=��,1=��;����RS485���Զ��ѱ�
			MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*0),7);//RS485-1��ʼ�ѱ��ַ
			MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*1),7);//RS485-2��ʼ�ѱ��ַ
			
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			UARTCtrl->AutoSearchTime=LenRx;//�Զ��ѱ����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ����
			UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+3;//������3=ʵʱ�����ѱ�
			UARTCtrl->AutoSearchTask=1;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
		
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
			UARTCtrl->AutoSearchTime=LenRx;//�Զ��ѱ����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ����
			UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+3;//������3=ʵʱ�����ѱ�
			UARTCtrl->AutoSearchTask=1;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
		#endif
			
		#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->AutoSearchTime=LenRx;//�Զ��ѱ����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ����
			UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+3;//������3=ʵʱ�����ѱ�
			UARTCtrl->AutoSearchTask=1;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
		#endif
			MR(ADDR_DATABUFF,ADDR_6002_8,LENmax_6002_8);//�ѱ����
			p8=(u8*)ADDR_DATABUFF;
			if(p8[9]==2)
			{//ÿ���ѱ�ǰ���
				DEL_SearchMeter_Record(3);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
				DEL_SearchMeter_Record(4);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
			}
		
			LenTx=0;
			LenRx=3;
			return (LenRx<<16)+LenTx;
		case 128://����128������ѱ���()
			DEL_SearchMeter_Record(3);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
			return (LenRx<<16)+LenTx;
		case 129://����129����տ�̨���ѱ���()
			DEL_SearchMeter_Record(4);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}



//�������ñ�
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6012_2_0[];
u32 ACTION_6012(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 filename;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 3://��ռ�¼��
			//ɾ����¼�ļ�
			ms=Get_ms();
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
			{
				if(filename==recordFILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
				}
				if(file->BLOCK_ENTRY)
				{
					//�ļ�����:0=��ͨ�ɼ��������ݼ�¼
					if((file->FILEID&0xff)==0)
					{
						//ɾ��
						i=filename;
						if(i>=recordFILENAMEmax_FLASH)
						{
							i-=recordFILENAMEmax_FLASH;
							i+=FILENAMEmax_FLASH;
						}
						ms->msfile_Delete(i);
					}
				}
				file++;
			}
			LenRx=1;
			return (LenRx<<16)+LenTx;
		case 127://����127��Add��array �������õ�Ԫ����ӻ����һ���������õ�Ԫ��
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_6012_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 128://����128��Delete��array ����ID��ɾ��һ�����õ�Ԫ��
		#if (USER/100)==0
		//һ���Բ������bug
			if(p8rx[4+3]==1)
			{
				p8rx[4+3]=10;
			}
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6012_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		#else
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6012_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		#endif
		case 129://����129��Clear()����������ñ�
			MC(0,ADDR_6012_2,LENper_6012_2*NUMmax_6012_2);
//		#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//			//���Ժ�������������Ҫ���������������ִ��
//			FLASHtoSDRAM();//����FLASHtoSDRAM
//			TASK(RS485_4PORT,1);//����;���:RM=0�ϱ�����,RM=1�ɼ�����
//		#endif
			LenRx=1;
			return (LenRx<<16)+LenTx;
		case 130://����130��Update(����ID��״̬)��������״̬��
			//::==DataType_structure
			//{
				//unsigned
				//emun
			//}
		#if (USER/100)==0
		//һ���Բ������bug
			Method=p8rx[4+3];//ȡ����ID
			if(Method==10)
			{
				MWR(10,ADDR_6012_2+(1*LENper_6012_2)+3,1);//�����1��Ϊ10
				MWR(2,ADDR_6012_2+(1*LENper_6012_2)+35,1);//״̬1��Ϊ2
			}
			else
			{
				if(Method<NUMmax_6012_2)
				{
					i=p8rx[4+5];
					MWR(i,ADDR_6012_2+(Method*LENper_6012_2)+35,1);
				}
			}
		#else
			Method=p8rx[4+3];//ȡ����ID
			if(Method<NUMmax_6012_2)
			{
				i=p8rx[4+5];
				MWR(i,ADDR_6012_2+(Method*LENper_6012_2)+35,1);
			}
		#endif
			LenRx=4;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//��ͨ�ɼ�������
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6014_2_0[];
u32 ACTION_6014(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 Method;
	u32 array;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8;
	u32 Len0;
	u32 Len1;
	u32 Len2;
	
	x=x;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127��Add��array ��ͨ�ɼ���������ӻ����һ����ͨ�ɼ�������
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_6014_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 128://����128��Delete��array ������ţ�ɾ��һ����ͨ�ɼ�������
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6014_2_0,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 129://����129��Clear()�����ͨ�ɼ���������
			MC(0,ADDR_6014,LENper_6014*NUMmax_6014);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		case 130://����130��Set_CSD��������ţ�array CSD�����÷����ļ�¼��ѡ��
//structure
//{
//unsigned �������
//array CSD
//}
			p8rx+=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			if((i&0xff)==0)
			{
				UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
				return 0;
			}
			i>>=8;
			if(LEN_RxAPDU<(i+4))
			{
				UARTCtrl->DAR=9;//���ݿ鲻���ã�9��
				return 0;
			}
			LenRx=i;
			array=1;
			while(array--)
			{
				Method=p8rx[3];//ȡ�������
				if(Method>=NUMmax_6014)
				{
					UARTCtrl->DAR=8;//Խ�磨8��
					break;
				}
				else
				{
				#if LENper_6014>LEN_DATABUFF
					#error
				#endif
					MR(ADDR_DATABUFF,ADDR_6014+(Method*LENper_6014),LENper_6014);
					p8=Get_Element((u8 *)ADDR_DATABUFF,4,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
					if(p8==0)
					{
						UARTCtrl->DAR=255;
						break;
					}
					//ԭCSDǰ���ݳ���
					Len0=(u32)p8-ADDR_DATABUFF;
					//ԭCSD���ݳ���
					Len1=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					//ԭ�������ݳ���
					Len2=Get_DL698DataLen_S(p8+Len1,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					Len2+=2;
					//��CSD���ݳ���
					i=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
					if((Len0+i+Len2)<=LENper_6014)
					{
						MW((u32)p8rx+4,ADDR_6014+(Method*LENper_6014)+Len0,i);
						//д��ԭ��������
						MW(ADDR_DATABUFF+Len0+Len1,ADDR_6014+(Method*LENper_6014)+Len0+i,Len2);
					}
					else
					{
						UARTCtrl->DAR=8;//Խ�磨8��
						break;
					}
				}
				i=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				p8rx+=i;
			}
			return (LenRx<<16)+LenTx;
	#if (USER/100)==9//������չ
		case 200:
//����200��Set_MS��������
//������=structure
//{
//  �������      unsigned��
//	���ܱ���    MS
//}
//���÷����ĵ��ܱ��ϡ�
			p8rx+=4;
			Method=p8rx[3];//ȡ������
			if(Method>=NUMmax_6014)
			{
				UARTCtrl->DAR=8;//Խ�磨8��
				return 0;
			}
		#if LENper_6014>LEN_DATABUFF
			#error
		#endif
			MR(ADDR_DATABUFF,ADDR_6014+(Method*LENper_6014),LENper_6014);
			//������ܱ���MS��Ĵ洢ʱ��ѡ�� enum
			p8=Get_Element((u8 *)ADDR_DATABUFF,6,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8==0)
			{
				UARTCtrl->DAR=9;//���ݿ鲻���� ��9��
				return 0;
			}
			x=p8[0]|(p8[1]<<8);//����ԭ����
			//������ܱ���MS��ʼ��ַ
			p8=Get_Element((u8 *)ADDR_DATABUFF,5,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8==0)
			{
				UARTCtrl->DAR=9;//���ݿ鲻���ã�9��
				return 0;
			}
			LenRx=Get_DL698DataLen_S(p8rx+4,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			if(((u32)p8+LenRx+2)>(ADDR_DATABUFF+LEN_DATABUFF))
			{
				UARTCtrl->DAR=9;//���ݿ鲻���ã�9��
				return 0;
			}
			MW((u32)p8rx+4,(u32)p8,LenRx);
			p8+=LenRx;
			p8[0]=x;//�ָ�ԭ����
			p8[1]=x>>8;
			p8+=2;
			if((u32)p8>=ADDR_DATABUFF)
			{
				i=(u32)p8-ADDR_DATABUFF;
				if(i>LENper_6014)
				{
					UARTCtrl->DAR=9;//���ݿ鲻���ã�9��
					return 0;
				}
				MW(ADDR_DATABUFF,ADDR_6014+(Method*LENper_6014),i);
			}
			LenRx+=4;
			return (LenRx<<16)+LenTx;
	#endif
		default:
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
	}
}

//�¼��ɼ�������
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6016_2[];
u32 ACTION_6016(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127��Add��array �¼��ɼ���������ӻ����һ���¼��ɼ�������
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_6016_2,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 128://����128��Delete��array ������ţ�ɾ��һ���¼��ɼ�������
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6016_2,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 129://����129��Clear()����¼��ɼ���������
			MC(0,ADDR_6016,LENper_6016*NUMmax_6016);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		case 130://����130��UpdateReportFlag(������ţ��ϱ���ʶ)�ϱ���ʶ��=bool
//structure
//{
//unsigned �������
//bool
//}
			if(LEN_RxAPDU<(4+6))
			{
				UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
				return 0;
			}
			LenRx=6;
			Method=p8rx[4+3];//ȡ�������
			if(Method>=NUMmax_6016)
			{
				UARTCtrl->DAR=8;//Խ��                 ��8��
			}
			else
			{
				pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_6016_2;
				i=Get_DL698DataLen((u8*)ADDR_6016+(Method*LENper_6016),(Attribute_TypeDef *)pLib->pDataType,0,LENper_6016);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				i>>=8;
				if(i>5)
				{
					i-=5;
					MW((u32)p8rx+4+4,ADDR_6016+(Method*LENper_6016)+i,2);
				}
			}
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//͸��������
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_6018[];
u32 ACTION_6018(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127��Add��͸����������Ӹ���һ��͸�����������һ�鷽�����ݡ�
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_6018,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 128://����128��AddMeterFrame��������ţ�ͨ�ŵ�ַ���������Ʊ�־���������ļ������һ�鱨�ġ�
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return LenTx+(LenRx<<16);
		case 129://����129��Delete��������ţ�array ͨ�ŵ�ַ��ɾ��һ��������һ�鷽�����ݡ�
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return (LenRx<<16)+LenTx;
		case 130://����130��Delete��array ������ţ�ɾ��һ��͸��������
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_6018,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 131://����131��Clear()���͸����������
			MC(0,ADDR_6018,LENper_6018*NUMmax_6018);
			//ͬʱ��ս����
			//MC(0,ADDR_601A,LENper_601A*NUMmax_601A);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//�ϱ�������
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_601C[];
u32 ACTION_601C(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127��Add��array �ϱ���������ӻ����һ���ϱ�������
			i=ACTION_Add((DL698_SETLIB_TypeDef *)DL698_SETLIB_601C,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//��������һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 128://����128��Delete��array ������ţ�ɾ��һ���ϱ�������
			i=ACTION_Del((DL698_SETLIB_TypeDef *)DL698_SETLIB_601C,PORTn,p8rx,LEN_RxAPDU,p8tx,LENmax_TxSPACE);//����ɾ��һ����һ����������;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
			return i;
		case 129://����129��Clear()����ϱ���������
			MC(0,ADDR_601C,LENper_601C*NUMmax_601C);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}

//�ɼ������
extern const DL698_SETLIB_TypeDef  DL698_SETLIB_601E[];
u32 Search_LIB601E_OAD(u32 OAD)//�Ӳɼ������������OAD���������(��0��ʼ),����ֵ>=NUMmax_601E��ʾû�ҵ�
{
	u32 i;
	u32 x;
	u32 n;
	u8 *p8;
	
	n=0;
	while(1)
	{
		if(n>=NUMmax_601E)
		{
			return n;
		}
		MR(ADDR_128KDATABUFF,ADDR_601E+(n*LENper_601E),LEN_128KDATABUFF);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<(LEN_128KDATABUFF/LENper_601E);i++)
		{
			if(n>=NUMmax_601E)
			{
				return n;
			}
			if(p8[0]==DataType_structure)
			{//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
				x=(p8[3]<<24)+(p8[4]<<16)+(p8[5]<<8)+p8[6];//ȡCSD
				if(x==OAD)
				{
					return n;
				}
			}
			n++;
			p8+=LENper_601E;
		}
	}
}
u32 Search_LIB601E_SPACE(void)//�Ӳɼ��������������1�������õ����(��0��ʼ),����ֵ>=NUMmax_601E��ʾû�ҵ�
{
	u32 i;
	u32 n;
	u8 *p8;
	
	n=0;
	while(1)
	{
		if(n>=NUMmax_601E)
		{
			return n;
		}
		MR(ADDR_128KDATABUFF,ADDR_601E+(n*LENper_601E),LEN_128KDATABUFF);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<(LEN_128KDATABUFF/LENper_601E);i++)
		{
			if(n>=NUMmax_601E)
			{
				return n;
			}
			if(p8[0]==0)
			{//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
				return n;
			}
			n++;
			p8+=LENper_601E;
		}
	}
}
u32 ACTION_601E(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Array;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	DL698_SETLIB_TypeDef *pLib;
	
	pLib=(DL698_SETLIB_TypeDef *)DL698_SETLIB_601E;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Method=p8rx[2];
	LenTx=0;
	switch(Method)
	{
		case 127://����127��Add��array �ɼ�������ӻ����һ��ɼ�����
			if(p8rx[4+0]==DataType_array)
			{//���һ��
				Array=p8rx[4+1];
				if(Array&0x80)
				{
					Array&=0x7f;
					if(LEN_RxAPDU<=(4+2+Array))
					{
						UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
						return 0;
					}
					LenRx=2+Array;
					p8rx+=4+2;
					LEN_RxAPDU-=4+2+Array;
					i=Array;
					Array=0;
					while(i--)
					{
						Array<<=8;
						Array|=p8rx[0];
						p8rx+=1;
					}
				}
				else
				{
					if(LEN_RxAPDU<=(4+2))
					{
						UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
						return 0;
					}
					LenRx=2;
					p8rx+=4+2;
					LEN_RxAPDU-=4+2;
				}
			}
			else
			{//��Ӹ���һ��
				if(LEN_RxAPDU<=(4+2))
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
				Array=1;
				LenRx=0;
				p8rx+=4;
				LEN_RxAPDU-=4;
			}
			while(Array--)
			{
				i=Get_DL698DataLen(p8rx,(Attribute_TypeDef *)pLib->pDataType,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				if((i&0xff)==0)
				{
					UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
					return 0;
				}
				i>>=8;
				if(LEN_RxAPDU<i)
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
				LenRx+=i;
				if(i>pLib->LENper)
				{
					UARTCtrl->DAR=8;//Խ��                 ��8��
					return (LenRx<<16)+0;
				}
				n=(p8rx[3]<<24)+(p8rx[4]<<16)+(p8rx[5]<<8)+p8rx[6];//ȡOAD
				n=Search_LIB601E_OAD(n);//�Ӳɼ������������OAD���������(��0��ʼ),����ֵ>=NUMmax_601E��ʾû�ҵ�
				if(n>=NUMmax_601E)
				{
					n=Search_LIB601E_SPACE();//�Ӳɼ��������������1�������õ����(��0��ʼ),����ֵ>=NUMmax_601E��ʾû�ҵ�
					if(n>=NUMmax_601E)
					{
						UARTCtrl->DAR=8;//Խ��                 ��8��
						return (LenRx<<16)+0;
					}
				}
				MW((u32)p8rx,pLib->Addr+(n*pLib->LENper),i);
			}
			return (LenRx<<16)+0;
		case 128://����128��Delete(array CSD)ɾ��һ��ɼ�����
			if(p8rx[4+0]==DataType_array)
			{//ɾ��һ��
				Array=p8rx[4+1];
				if(Array&0x80)
				{
					Array&=0x7f;
					if(LEN_RxAPDU<=(4+2+Array))
					{
						UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
						return 0;
					}
					LenRx=2+Array;
					p8rx+=4+2;
					LEN_RxAPDU-=4+2+Array;
					i=Array;
					Array=0;
					while(i--)
					{
						Array<<=8;
						Array|=p8rx[0];
						p8rx+=1;
					}
				}
				else
				{
					if(LEN_RxAPDU<=(4+2))
					{
						UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
						return 0;
					}
					LenRx=2;
					p8rx+=4+2;
					LEN_RxAPDU-=4+2;
				}
			}
			else
			{//ɾ��һ��
				if(LEN_RxAPDU<=(4+2))
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
				Array=1;
				LenRx=0;
				p8rx+=4;
				LEN_RxAPDU-=4;
			}
			while(Array--)
			{
				i=Get_DL698DataLen(p8rx,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				if((i&0xff)==0)
				{
					UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
					return 0;
				}
				i>>=8;
				if(LEN_RxAPDU<i)
				{
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
				n=(p8rx[2]<<24)+(p8rx[3]<<16)+(p8rx[4]<<8)+p8rx[5];//ȡOAD
				n=Search_LIB601E_OAD(n);//�Ӳɼ������������OAD���������(��0��ʼ),����ֵ>=NUMmax_601E��ʾû�ҵ�
				if(n<NUMmax_601E)
				{
					MC(0,pLib->Addr+(n*pLib->LENper),1);
				}
				LenRx+=i;
				p8rx+=i;
			}
			return (LenRx<<16)+0;
		case 129://����129��Clear()��ղɼ�����⡣
			MC(0,ADDR_601E,LENper_601E*NUMmax_601E);
			LenRx=1;
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
	}
}




























