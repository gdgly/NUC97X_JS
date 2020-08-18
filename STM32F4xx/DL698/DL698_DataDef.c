

#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698TEST.h"


//�õ�date_time_s��7�ֽ�
void Get_date_time_s(u8* p8d)//�õ�date_time_s��7�ֽ�
{
	u8*p8s;
	p8s=(u8*)ADDR_DL698YMDHMS;
	p8d[0]=p8s[0];
	p8d[1]=p8s[1];
	p8d[2]=p8s[2];
	p8d[3]=p8s[3];
	p8d[4]=p8s[4];
	p8d[5]=p8s[5];
	p8d[6]=p8s[6];
}

//���ݵ���
u64 R_Inverse(u64 Data,u32 Byte)//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
{
	u64 NewData;
	if(Byte<2)
	{
		return Data;
	}
	NewData=0;
	while(Byte--)
	{
		NewData<<=8;
		NewData|=Data&0xff;
		Data>>=8;
	}
	return NewData;
}
//λ����
u32 Bit_Inverse(u32 Data,u32 bit)//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
{
	u32 i;
	u32 NewData;
	if(bit<2)
	{
		return Data;
	}
	NewData=0;
	bit--;
	for(i=0;i<=bit;i++)
	{
		if(Data&1)
		{
			NewData|=1<<(bit-i);
		}
		Data>>=1;
	}
	return NewData;
}

//�õ����ȵ�Ԫ��ֵ
u32 Get_DL698ByteTypeLenByte(u8 *p8)//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
{
	u32 i;
	i=p8[0];
	if(i&0x80)
	{
		i&=0x7f;
		i++;
		return i;
	}
	else
	{
		return 1;
	}
}
u32 Get_DL698ByteTypeLenValue(u8 *p8)//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
{
	u32 i;
	u32 Len;
	i=p8[0];
	if(i&0x80)
	{
		p8+=1;
		i&=0x7f;
		Len=0;
		while(i--)
		{
			Len<<=8;
			Len|=p8[0];
			p8+=1;
		}
		return Len;
	}
	else
	{
		return i;
	}
}
u32 Get_DL698BinTypeLenByte(u8 *p8)//�õ�DL698λ����������ʾ���ȵ�Ԫ���ֽ���
{
	u32 i;
	i=p8[0];
	if(i&0x80)
	{
		i&=0x7f;
		i+=7;
		i/=8;
		i++;
		return i;
	}
	else
	{
		return 1;
	}
}
u32 Get_DL698BinTypeLenValue(u8 *p8)//�õ�DL698λ���ͳ��ȳ��ȵ�Ԫ��ֵתΪ�ֽ���
{
	u32 i;
	u32 Len;
	
	i=p8[0];
	if(i&0x80)
	{
		p8+=1;
		i&=0x7f;
		Len=0;
		while(i--)
		{
			Len<<=8;
			Len|=p8[0];
			p8+=1;
		}
		Len+=7;
		Len/=8;
		return Len;
	}
	else
	{
		i+=7;
		i/=8;
		return i;
	}
}

u64 Get_TSA_Value(u8 *p8)//�õ�TSA�Ĳ�����6�ֽ�ͨ�ŵ�ַ,����˳�򲻱�,���:p8=TSA���ͺ���ֽڣ�����Чֵ��=0
{
	u32 i;
	u64 d64;
	u64 d64d;
	
	d64=0;
	if(p8[0])
	{
		i=p8[1];
		i++;
		if(i>6)
		{
			i=6;
		}
		d64=MRR((u32)p8+2,6);
		d64d=0;
		while(i--)
		{
			d64d<<=8;
			d64d|=0xff;
		}
		d64&=d64d;
	}
	return d64;
}

//TIתΪ��ֵ
u32 TItoSECOND(u32 TI)//TIתΪ��ֵ,���:��λ�ڵ��ֽ�,����:hex��
{
	u32 i;
	i=TI>>16;
	i|=TI&0xff00;
	switch(TI&0xff)
	{
		case 0://  ��      ��0����
			break;
		case 1://��      ��1����
			i*=60;
			break;
		case 2://ʱ      ��2����
			i*=60*60;
			break;
		case 3://��      ��3����
			i*=24*60*60;
			break;
		case 4://��      ��4����
			i*=31*24*60*60;
			break;
		case 5://��      ��5��
			i*=365*24*60*60;
			break;
		default:
			i*=365*24*60*60;
			break;
	}
	return i;
}

u32 pTItoSECOND(u8 *pTI)//TIתΪ��ֵ,���:pTI����������;����:hex��
{
	u32 i;
	i=(pTI[1]<<8)+pTI[2];//ȡƵ��ֵ
	switch(pTI[0])
	{
		case 0://  ��      ��0����
			break;
		case 1://��      ��1����
			i*=60;
			break;
		case 2://ʱ      ��2����
			i*=60*60;
			break;
		case 3://��      ��3����
			i*=24*60*60;
			break;
		case 4://��      ��4����
			i*=31*24*60*60;
			break;
		case 5://��      ��5��
			i*=365*24*60*60;
			break;
		default:
			i*=365*24*60*60;
			break;
	}
	return i;
}

void pYYMDHMS_AddTI_hex(u8* pYYMDHMS,u8* pTI)//��������ʱ�����ʱ����TI,���:pYYMDHMS,pTI����������,���ؼӼ��TI�����������ʱ����
{
	u32 i;
	i=(pTI[1]<<8)+pTI[2];//ȡƵ��ֵ
	switch(pTI[0])
	{
		case 0://  ��      ��0����
			break;
		case 1://��      ��1����
			i*=60;
			break;
		case 2://ʱ      ��2����
			i*=60*60;
			break;
		case 3://��      ��3����
			i*=24*60*60;
			break;
		case 4://��      ��4����
			pYYM_AddM_hex(pYYMDHMS,i);//�����¼�m��,���ؼ�m�º��������
			return;
		case 5://��      ��5��
			i*=365*24*60*60;
			break;
		default:
			i*=365*24*60*60;
			break;
	}
	pYYMDHMS_AddS_hex(pYYMDHMS,i);//��������ʱ�����S��,���ؼ�S������������ʱ����
}


//********���ȼ���***********
u32 DataLen_NULL(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	0//	��
{
	return (1<<8)+TypeCount;
}
u32 DataLen_array(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	1//	SEQUENCE OF Data����6.3.3.1���������Ԫ���ڶ������Ի򷽷��������ж���
{
	u32 Array;
	u32 Len;
	u32 Len_Element;
	
	Array=p8[0];
	if(Array&0x80)
	{
		p8+=1;
		Len_Element=Array&0x7f;
		Len=Len_Element+2;
		Array=0;
		while(Len_Element--)
		{
			Array<<=8;
			Array|=p8[0];
			p8+=1;
		}
	}
	else
	{
		p8+=1;
		Len=2;
	}
	if(Array==0)
	{//������������������͸���,��������=0ʱ����Ԫ�ظ�����
		if(pDataType)
		{
			TypeCount+=pDataType->TypeList[TypeCount];
			TypeCount+=1;//������������������͸���1byte
		}
	}
	else
	{
		TypeCount++;//�������������µ����͸����ֽ�
	}
	Len_Element=TypeCount;
	while(Array--)
	{
		Len_Element=Get_DL698DataLen(p8,pDataType,TypeCount,0);//�õ�DL698�������ͳ���
		if(pDataType)
		{
			if((Len_Element&0xff)==0)
			{
				return 0;//����
			}
		}
		Len+=Len_Element>>8;
		p8+=Len_Element>>8;
		if(Len>LENmax_APDU)
		{
			return 0;//APDU��󳤶�����
		}
	}
	TypeCount=Len_Element&0xff;
	return (Len<<8)+TypeCount;
}
u32 DataLen_structure(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	2//	SEQUENCE OF Data����6.3.3.1�����ṹ��Ԫ���ڶ������Ի򷽷��������ж���	
{
	u32 Structure;
	u32 Len;
	u32 Len_Element;
	
	Structure=p8[0];
	if(Structure&0x80)
	{
		p8+=1;
		Len_Element=Structure&0x7f;
		Len=Len_Element+2;
		Structure=0;
		while(Len_Element--)
		{
			Structure<<=8;
			Structure|=p8[0];
			p8+=1;
		}
	}
	else
	{
		p8+=1;
		Len=2;
	}
	while(Structure--)
	{
		Len_Element=Get_DL698DataLen(p8,pDataType,TypeCount,0);//�õ�DL698�������ͳ���
		TypeCount=Len_Element&0xff;
		if(pDataType)
		{
			if(TypeCount==0)
			{
				return 0;//����
			}
		}
		Len_Element>>=8;
		p8+=Len_Element;
		Len+=Len_Element;
		if(Len>LENmax_APDU)
		{
			return 0;//APDU��󳤶�����
		}
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_bool(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	3//	����ֵ��BOOLEAN��	1��0
{
	return (2<<8)+TypeCount;
}
u32 DataLen_bit_string(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	4//	����λ����BIT STRING��	
{
	u32 i;
	u32 Len;
	u32 Len_Element;
	
	i=p8[0];
	if(i&0x80)
	{
		p8+=1;
		Len_Element=i&0x7f;
		Len=Len_Element+2;
		i=0;
		while(Len_Element--)
		{
			i<<=8;
			i|=p8[0];
			p8+=1;
		}
	}
	else
	{
		p8+=1;
		Len=2;
	}
	i+=7;
	i/=8;
	Len+=i;
	if(Len>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_double_long(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	5//	32����λ������Integer32��	-231��231-1
{
	return ((1+4)<<8)+TypeCount;
}
u32 DataLen_double_long_unsigned(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	6//	32����λ��������double-long-unsigned��	0��232-1
{
	return ((1+4)<<8)+TypeCount;
}
//����	7-8		
u32 DataLen_octet_string(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	9//	8����λλ�飨�ֽڣ�����OCTET STRING��	
{
	u32 i;
	u32 Len;
	u32 Len_Element;
	
	i=p8[0];
	if(i&0x80)
	{
		p8+=1;
		Len_Element=i&0x7f;
		Len=Len_Element+2;
		i=0;
		while(Len_Element--)
		{
			i<<=8;
			i|=p8[0];
			p8+=1;
		}
	}
	else
	{
		p8+=1;
		Len=2;
	}
	Len+=i;
	if(Len>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_visible_string(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	10//	ASCII�ַ�����VisibleString��	
{
	u32 i;
	u32 Len;
	u32 Len_Element;
	
	i=p8[0];
	if(i&0x80)
	{
		p8+=1;
		Len_Element=i&0x7f;
		Len=Len_Element+2;
		i=0;
		while(Len_Element--)
		{
			i<<=8;
			i|=p8[0];
			p8+=1;
		}
	}
	else
	{
		p8+=1;
		Len=2;
	}
	Len+=i;
	if(Len>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (Len<<8)+TypeCount;
}
//����	11		
u32 DataLen_UTF8_string(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	12//	UTF-8������ַ���	
{
	u32 i;
	u32 Len;
	u32 Len_Element;
	
	i=p8[0];
	if(i&0x80)
	{
		p8+=1;
		Len_Element=i&0x7f;
		Len=Len_Element+2;
		i=0;
		while(Len_Element--)
		{
			i<<=8;
			i|=p8[0];
			p8+=1;
		}
	}
	else
	{
		p8+=1;
		Len=2;
	}
	Len+=i;
	if(Len>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (Len<<8)+TypeCount;
}
//����	13-14		
u32 DataLen_integer(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	15//	8����λ������integer��	-128��127
{
	return ((1+1)<<8)+TypeCount;
}
u32 DataLen_long(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	16//	16����λ������long��	-32768��32767
{
	return ((1+2)<<8)+TypeCount;
}
u32 DataLen_unsigned(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	17//	8����λ��������Unsigned8��	0��255
{
	return ((1+1)<<8)+TypeCount;
}
u32 DataLen_long_unsigned(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	18//	16����λ��������Unsigned16��	0��65535
{
	return ((1+2)<<8)+TypeCount;
}
//����	19		
u32 DataLen_long64(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	20//	64����λ������Integer64��	-263��263-1
{
	return ((1+8)<<8)+TypeCount;
}
u32 DataLen_long64_unsigned(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	21//	64����λ��������Unsigned64��	0��264-1
{
	return ((1+8)<<8)+TypeCount;
}
u32 DataLen_enum(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	22//	ö�ٵ�Ԫ���ڶ������Ի򷽷��������ж���	0��255
{
	return ((1+1)<<8)+TypeCount;
}
u32 DataLen_float32(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	23//	octet-string��SIZE��4����
{
	return ((1+4)<<8)+TypeCount;
}
u32 DataLen_float64(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	24//	octet-string��SIZE��8����
{
	return ((1+8)<<8)+TypeCount;
}
u32 DataLen_date_time(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	25//	octet-string��SIZE��10����
{
	return ((1+10)<<8)+TypeCount;
}
u32 DataLen_date(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	26//	octet-string��SIZE��5����
{
	return ((1+5)<<8)+TypeCount;
}
u32 DataLen_time(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	27//	octet-string��SIZE��3����
{
	return ((1+3)<<8)+TypeCount;
}
u32 DataLen_date_time_s(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	28//octet-string��SIZE��7����
{
	return ((1+7)<<8)+TypeCount;
}

//����	29-79	
u32 DataLen_OI(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	80//	��6.3.3.30��
{
	return ((1+2)<<8)+TypeCount;
}
u32 DataLen_OAD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	81//	��6.3.3.11��
{
	return ((1+4)<<8)+TypeCount;
}
u32 DataLen_ROAD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	82//	��6.3.3.12��
{
	u32 i;
	i=p8[4];
	i*=4;
	i+=6;
	return (i<<8)+TypeCount;
}
u32 DataLen_OMD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	83//	��6.3.3.18��
{
	return ((1+4)<<8)+TypeCount;
}
u32 DataLen_TI(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	84//	��6.3.3.28��
{
	return ((1+3)<<8)+TypeCount;
}
u32 DataLen_TSA(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	85//	��6.3.3.19��
{
	u32 i;
	i=p8[0];
	i+=2;
	return (i<<8)+TypeCount;
}
u32 DataLen_MAC(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	86//	��6.3.3.21��
{
	u32 i;
	u32 Len;
	u32 Len_Element;
	
	i=p8[0];
	if(i&0x80)
	{
		p8+=1;
		Len_Element=i&0x7f;
		Len=Len_Element+2;
		i=0;
		while(Len_Element--)
		{
			i<<=8;
			i|=p8[0];
			p8+=1;
		}
	}
	else
	{
		p8+=1;
		Len=2;
	}
	Len+=i;
	if(Len>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_RN(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	87//	��6.3.3.24��
{
	u32 i;
	u32 Len;
	u32 Len_Element;
	
	i=p8[0];
	if(i&0x80)
	{
		p8+=1;
		Len_Element=i&0x7f;
		Len=Len_Element+2;
		i=0;
		while(Len_Element--)
		{
			i<<=8;
			i|=p8[0];
			p8+=1;
		}
	}
	else
	{
		p8+=1;
		Len=2;
	}
	Len+=i;
	if(Len>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_Region(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	88//	��6.3.3.29��
{
	return (0<<8)+TypeCount;
}
u32 DataLen_Scaler_Unit(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	89//	��6.3.3.20��
{
	return ((1+2)<<8)+TypeCount;
}
u32 DataLen_RSD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	90//	��6.3.3.13��
{
	u32 i;
	u32 Len;
	switch(p8[0])
	{
		case 0:
//��ѡ��       [0]  NULL
			Len=2;
			break;
		case 1:
//Selector1��=SEQUENCE
//{
//  ��������������  OAD��
//  ��ֵ            Data
//}
			p8+=5;
			Len=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			Len+=1+5;
			break;
		case 2:
//Selector2��=SEQUENCE
//{
//  ��������������  OAD��
//  ��ʼֵ          Data��
//  ����ֵ          Data��
//  ���ݼ��        Data
//}
			Len=5;
			Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			Len+=1;
			break;
		case 3:
//Selector3��=SEQUENCE OF Selector2
			i=p8[1];//SEQUENCE OF
			Len=2;
			while(i--)
			{
				Len+=4;
				Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			}
			Len+=1;
			break;
		case 4:
//Selector4��=SEQUENCE
//{
//  �ɼ�����ʱ��  date_time_s��
//  ���ܱ���    MS
//}
			Len=DataLen_MS(p8+1+7,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MSû��������
			}
			Len+=2+7;
			break;
		case 5:
//Selector5��=SEQUENCE
//{
//  �ɼ��洢ʱ��  date_time_s��
//  ���ܱ���    MS
//}
			Len=DataLen_MS(p8+1+7,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MSû��������
			}
			Len+=2+7;
			break;
		case 6:
//Selector6��=SEQUENCE
//{
//  �ɼ�����ʱ����ʼֵ  date_time_s��
//  �ɼ�����ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			Len=DataLen_MS(p8+1+17,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MSû��������
			}
			Len+=2+17;
			break;
		case 7:
//Selector7��=SEQUENCE
//{
//  �ɼ��洢ʱ����ʼֵ  date_time_s��
//  �ɼ��洢ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			Len=DataLen_MS(p8+1+17,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MSû��������
			}
			Len+=2+17;
			break;
		case 8:
//Selector8��=SEQUENCE
//{
//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			Len=DataLen_MS(p8+1+17,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MSû��������
			}
			Len+=2+17;
			break;
		case 9:
//Selector9��=SEQUENCE
//{
//  �ϵ�n�μ�¼  unsigned
//}
			Len=2+1;
			break;
		case 10:
//Selector10��=SEQUENCE
//{
//��n����¼  unsigned��
//���ܱ���  MS
//}
			Len=DataLen_MS(p8+1+1,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MSû��������
			}
			Len+=2+1;
			break;
		default:
			Len=2;
			break;
	}
	if(Len>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_RSD_NoMS(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//������MS���Ȳ���
{
	u32 i;
	u32 Len;
	switch(p8[0])
	{
		case 0:
//��ѡ��       [0]  NULL
			Len=2;
			break;
		case 1:
//Selector1��=SEQUENCE
//{
//  ��������������  OAD��
//  ��ֵ            Data
//}
			p8+=5;
			Len=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			Len+=1+5;
			break;
		case 2:
//Selector2��=SEQUENCE
//{
//  ��������������  OAD��
//  ��ʼֵ          Data��
//  ����ֵ          Data��
//  ���ݼ��        Data
//}
			Len=5;
			Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			Len+=1;
			break;
		case 3:
//Selector3��=SEQUENCE OF Selector2
			i=p8[1];//SEQUENCE OF
			Len=2;
			while(i--)
			{
				Len+=4;
				Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				Len+=Get_DL698DataLen_S(p8+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			}
			Len+=1;
			break;
		case 4:
//Selector4��=SEQUENCE
//{
//  �ɼ�����ʱ��  date_time_s��
//  ���ܱ���    MS
//}
			Len=2+7;
			break;
		case 5:
//Selector5��=SEQUENCE
//{
//  �ɼ��洢ʱ��  date_time_s��
//  ���ܱ���    MS
//}
			Len=2+7;
			break;
		case 6:
//Selector6��=SEQUENCE
//{
//  �ɼ�����ʱ����ʼֵ  date_time_s��
//  �ɼ�����ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			Len=2+17;
			break;
		case 7:
//Selector7��=SEQUENCE
//{
//  �ɼ��洢ʱ����ʼֵ  date_time_s��
//  �ɼ��洢ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			Len=2+17;
			break;
		case 8:
//Selector8��=SEQUENCE
//{
//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			Len=2+17;
			break;
		case 9:
//Selector9��=SEQUENCE
//{
//  �ϵ�n�μ�¼  unsigned
//}
			Len=2+1;
			break;
		case 10:
//Selector10��=SEQUENCE
//{
//��n����¼  unsigned��
//���ܱ���  MS
//}
			Len=2+1;
			break;
		default:
			Len=2;
			break;
	}
	if(Len>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (Len<<8)+TypeCount;
}

u32 DataLen_CSD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	91//	��6.3.3.15��
{
//CSD��=CHOICE
//{
//��������������       [0] OAD��
//��¼�Ͷ������������� [1] ROAD
//}
	u32 i;
	if(p8[0]==0)
	{//OAD
		return ((2+4)<<8)+TypeCount;
	}
	else
	{//ROAD
		i=p8[5];
		i*=4;
		i+=7;
		return (i<<8)+TypeCount;
	}
}
u32 DataLen_MS(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	92//	��6.3.3.16��
{
//MS��=CHOICE
//{
//  �޵��ܱ�		[0]���൱����Ч����
//  ȫ���û���ַ  	[1]��ȫ���ɲɼ��ĵ��ܱ�
//  һ���û�����  	[2] 	SEQUENCE OF unsigned��ָ���������û����͵���Щ���ܱ�
//  һ���û���ַ  	[3] 	SEQUENCE OF TSA��ָ�������ɵ��ܱ�ͨ�ŵ�ַ����Щ���ܱ�
//  һ���������  	[4] 	SEQUENCE OF long-unsigned��ָ�������ɵ��ܱ�������ŵ���Щ���ܱ�
//һ���û��������� [5] SEQUENCE OF Region����������ʽ�����û����ͷ�Χ
//һ���û���ַ���� [6] SEQUENCE OF Region����������ʽ�������ܱ�ͨ�ŵ�ַ��Χ
//һ������������� [7] SEQUENCE OF Region,��������ʽ�������ܱ���������ŷ�Χ
//}
//Region��=SEQUENCE
//{
//   ��λ  ENUMERATED
//   {
//       ǰ�պ�    ��0����
//       ǰ�����    ��1����
//       ǰ�պ��    ��2����
//       ǰ����    ��3��
//    }��
//��ʼֵ  Data��
//����ֵ  Data
//}
//Region�����������ݵ����䷶Χ�������������֣�����ʼֵ������ֵ����[��ʼֵ������ֵ��������ʼֵ������ֵ]��[��ʼֵ������ֵ]
	u32 i;
	u32 n;
	u32 Len;
	switch(p8[0])
	{
		case 0:
		case 1:
//68 37 00 43 05 01 00 00 00 00 00 10 AC 14 07 01 3D 60 14 7F 00 01 01 02 06 11 01 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 10 02 00 5C 01 16 04 00 4D 75 16 
			i=1+1;
			break;
		case 2://SEQUENCE OF ǰ���������͵�unsigned
//68 39 00 43 05 01 00 00 00 00 00 10 71 5D 07 01 3E 60 14 7F 00 01 01 02 06 11 02 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 20 02 00 5C 02 01 00 16 03 00 E6 53 16 
			i=p8[1];
			i+=3;
			break;
		case 3://SEQUENCE OF ǰ���������͵�TSA
//68 50 00 43 05 01 00 00 00 00 00 10 C5 9B 07 01 3F 60 14 7F 00 01 01 02 06 11 03 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 30 02 00 5C 03 03 07 05 00 00 00 00 00 01 07 05 00 00 00 00 00 02 07 05 00 00 00 00 00 03 16 06 00 4B E0 16 
			i=3;
			n=p8[1];
			p8+=2;
			while(n--)
			{
				Len=p8[0];
				Len++;
				i+=Len;
				p8+=Len;
			}
			break;
		case 4://SEQUENCE OF ǰ���������͵�long-unsigned
//68 3E 00 43 05 01 00 00 00 00 00 10 97 FD 07 01 00 60 14 7F 00 01 01 02 06 11 04 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 40 02 00 5C 04 03 00 01 00 02 00 03 16 06 00 DF BE 16 
			i=p8[1];
			i*=2;
			i+=3;
			break;
		case 5://SEQUENCE OF Region,���������͵�unsigned
//68 3D 00 43 05 01 00 00 00 00 00 10 24 03 07 01 01 60 14 7F 00 01 01 02 06 11 05 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 50 02 00 
//5C 05 01 02 11 00 11 01 16 02 00 62 53 16 
			i=p8[1];
			i*=5;
			i+=3;
			break;
		case 6://SEQUENCE OF Region,���������͵�TSA
//68 4B 00 43 05 01 00 00 00 00 00 10 99 A8 07 01 02 60 14 7F 00 01 01 02 06 11 06 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 60 02 00 
//5C 06 01 02 55 07 05 00 00 00 00 00 01 55 07 05 00 00 00 00 00 02 16 06 00 86 21 16 
			i=3;
			n=p8[1];
			p8+=2;
			while(n--)
			{
				i+=2;
				p8+=2;
				Len=p8[0];
				Len+=1;
				i+=Len;
				p8+=Len;
				Len=p8[1];
				Len+=2;
				i+=Len;
				p8+=Len;
			}
			break;
		case 7://SEQUENCE OF Region,���������͵�long-unsigned
//68 46 00 43 05 01 00 00 00 00 00 10 F7 1F 07 01 03 60 14 7F 00 01 01 02 06 11 07 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 70 02 00 
//5C 07 02 00 12 00 01 12 00 02 02 12 00 02 12 00 03 16 04 00 9D 35 16 
			i=p8[1];
			i*=7;
			i+=3;
			break;
		default:
			i=0;
			break;
	}
	return (i<<8)+TypeCount;
}




u32 DataLen_SID(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	93//	��6.3.3.22��
{
	u32 i;
	p8+=4;
	i=p8[0];
	if(i&0x80)
	{
		i=(p8[1]<<8)|p8[2];
		i+=3;
	}
	else
	{
		i+=1;
	}
	i+=4;
	if(i>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (i<<8)+TypeCount;
}
u32 DataLen_SID_MAC(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	94//	��6.3.3.23��
{
	u32 i;
	p8+=4;
	i=p8[0];
	if(i&0x80)
	{
		i=(p8[1]<<8)|p8[2];
		i+=3;
	}
	else
	{
		i+=1;
	}
	i+=4+5;//4byte MACǰ��1�ֽڳ���
	if(i>LENmax_APDU)
	{
		return 0;//APDU��󳤶�����
	}
	return (i<<8)+TypeCount;
}
u32 DataLen_COMDCB(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	95//	��6.3.3.34��
{
	return ((1+5)<<8)+TypeCount;
}
u32 DataLen_RCSD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	96
{
//RCSD��=SEQUENCE OF CSD
//CSD��=CHOICE
//{
//��������������       [0] OAD��
//��¼�Ͷ������������� [1] ROAD
//}
	u32 i;
	u32 n;
	u32 Len;
	
	n=p8[0];
	p8++;
	Len=2;
	while(n--)
	{
		if(p8[0]==0)
		{//OAD
			Len+=5;
			p8+=5;
		}
		else
		{//ROAD
			i=p8[5];
			i*=4;
			i+=6;
			Len+=i;
			p8+=i;
		}
	}
	return (Len<<8)+TypeCount;
}

//����	97-255	




typedef struct
{
	u32 (*DataLen_NULL)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	0//	��
	u32 (*DataLen_array)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	1//	SEQUENCE OF Data����6.3.3.1���������Ԫ���ڶ������Ի򷽷��������ж���
	u32 (*DataLen_structure)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	2//	SEQUENCE OF Data����6.3.3.1�����ṹ��Ԫ���ڶ������Ի򷽷��������ж���	
	u32 (*DataLen_bool)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	3//	����ֵ��BOOLEAN��	1��0
	u32 (*DataLen_bit_string)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	4//	����λ����BIT STRING��	
	u32 (*DataLen_double_long)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	5//	32����λ������Integer32��	-231��231-1
	u32 (*DataLen_double_long_unsigned)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	6//	32����λ��������double-long-unsigned��	0��232-1
	u32 (*DataLen_NULL7)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//7����
	u32 (*DataLen_NULL8)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//8����
	u32 (*DataLen_octet_string)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	9//	8����λλ�飨�ֽڣ�����OCTET STRING��	
	u32 (*DataLen_visible_string)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	10//	ASCII�ַ�����VisibleString��	
	u32 (*DataLen_NULL11)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//11����
	u32 (*DataLen_UTF8_string)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	12//	UTF-8������ַ���	
	u32 (*DataLen_NULL13)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//13����
	u32 (*DataLen_NULL14)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//14����
	u32 (*DataLen_integer)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	15//	8����λ������integer��	-128��127
	u32 (*DataLen_long)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	16//	16����λ������long��	-32768��32767
	u32 (*DataLen_unsigned)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	17//	8����λ��������Unsigned8��	0��255
	u32 (*DataLen_long_unsigned)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	18//	16����λ��������Unsigned16��	0��65535
	u32 (*DataLen_NULL19)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//19����
	u32 (*DataLen_long64)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	20//	64����λ������Integer64��	-263��263-1
	u32 (*DataLen_long64_unsigned)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	21//	64����λ��������Unsigned64��	0��264-1
	u32 (*DataLen_enum)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	22//	ö�ٵ�Ԫ���ڶ������Ի򷽷��������ж���	0��255
	u32 (*DataLen_float32)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	23//	octet-string��SIZE��4����
	u32 (*DataLen_float64)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	24//	octet-string��SIZE��8����
	u32 (*DataLen_date_time)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	25//	octet-string��SIZE��10����
	u32 (*DataLen_date)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	26//	octet-string��SIZE��5����
	u32 (*DataLen_time)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	27//	octet-string��SIZE��3����
	u32 (*DataLen_date_time_s)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	28//octet-string��SIZE��7����
	

//����	29-79	
	u32 (*DataLen_OI)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	80
	u32 (*DataLen_OAD)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	81
	u32 (*DataLen_ROAD)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	82
	u32 (*DataLen_OMD)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	83
	u32 (*DataLen_TI)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	84
	u32 (*DataLen_TSA)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	85
	u32 (*DataLen_MAC)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	86
	u32 (*DataLen_RN)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	87
	u32 (*DataLen_Region)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	88
	u32 (*DataLen_Scaler_Unit)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	89
	u32 (*DataLen_RSD)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	90
	u32 (*DataLen_CSD)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	91
	u32 (*DataLen_MS)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	92
	u32 (*DataLen_SID)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	93
	u32 (*DataLen_SID_MAC)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	94
	u32 (*DataLen_COMDCB)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	95
	u32 (*DataLen_RCSD)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	96
	//����	96-255	
	
}DataLen_TypeDef;
__align(4) const DataLen_TypeDef  DataLen_List[]=
{
	DataLen_NULL,//	0//	��
	DataLen_array,//	1//	SEQUENCE OF Data����6.3.3.1���������Ԫ���ڶ������Ի򷽷��������ж���
	DataLen_structure,//	2//	SEQUENCE OF Data����6.3.3.1�����ṹ��Ԫ���ڶ������Ի򷽷��������ж���	
	DataLen_bool,//	3//	����ֵ��BOOLEAN��	1��0
	DataLen_bit_string,//	4//	����λ����BIT STRING��	
	DataLen_double_long,//	5//	32����λ������Integer32��	-231��231-1
	DataLen_double_long_unsigned,//	6//	32����λ��������double-long-unsigned��	0��232-1
	DataLen_NULL,//7����
	DataLen_NULL,//8����
	DataLen_octet_string,//	9//	8����λλ�飨�ֽڣ�����OCTET STRING��	
	DataLen_visible_string,//	10//	ASCII�ַ�����VisibleString��	
	DataLen_NULL,//11����
	DataLen_UTF8_string,//	12//	UTF-8������ַ���	
	DataLen_NULL,//13����
	DataLen_NULL,//14����
	DataLen_integer,//	15//	8����λ������integer��	-128��127
	DataLen_long,//	16//	16����λ������long��	-32768��32767
	DataLen_unsigned,//	17//	8����λ��������Unsigned8��	0��255
	DataLen_long_unsigned,//	18//	16����λ��������Unsigned16��	0��65535
	DataLen_NULL,//19����
	DataLen_long64,//	20//	64����λ������Integer64��	-263��263-1
	DataLen_long64_unsigned,//	21//	64����λ��������Unsigned64��	0��264-1
	DataLen_enum,//	22//	ö�ٵ�Ԫ���ڶ������Ի򷽷��������ж���	0��255
	DataLen_float32,//	23//	octet-string��SIZE��4����
	DataLen_float64,//	24//	octet-string��SIZE��8����
	DataLen_date_time,//	25//	octet-string��SIZE��10����
	DataLen_date,//	26//	octet-string��SIZE��5����
	DataLen_time,//	27//	octet-string��SIZE��3����
	DataLen_date_time_s,//	28//octet-string��SIZE��7����
	
	//����	29-79	
	DataLen_OI,//	80
	DataLen_OAD,//	81
	DataLen_ROAD,//	82
	DataLen_OMD,//	83
	DataLen_TI,//	84
	DataLen_TSA,//	85
	DataLen_MAC,//	86
	DataLen_RN,//	87
	DataLen_Region,//	88
	DataLen_Scaler_Unit,//	89
	DataLen_RSD,//	90
	DataLen_CSD,//	91
	DataLen_MS,//	92
	DataLen_SID,//	93
	DataLen_SID_MAC,//	94
	DataLen_COMDCB,//	95
	DataLen_RCSD,// 96
	//����	97-255	
};

u32 Get_DL698DataLen(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount,u32 LENmax)//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
{
	u32 DataType;
	u32 *p32;
	u32 Len;
	
	if(((u32)p8>=0x30000000)&&((u32)p8<0xb0000000))
	{//NAND FLASH
		if((LENmax)&&(LENmax<=LEN_128KDATABUFF))
		{
			MR(ADDR_128KDATABUFF,(u32)p8,LENmax);
			p8=(u8*)ADDR_128KDATABUFF;
			DataType=p8[0];
		}
		else
		{
			return 0;//����
		}
	}
	else
	{
		DataType=p8[0];
	}
	
	if(pDataType)
	{//���Զ���ָ����Ч
		if(TypeCount>=pDataType->TypeNum)
		{
			return 0;//����
		}
		if(DataType!=pDataType->TypeList[TypeCount])
		{
			if(pDataType->TypeList[TypeCount]==DataType_Data)
			{//����Data
				Len=Get_DL698DataLen(p8,0,0,0);
				Len>>=8;
				Len<<=8;
				Len|=TypeCount+1;
				return Len;
			}
			else
			{//����ֵ�̶�
				if((TypeCount==0)&&(DataType==DataType_NULL))
				{//�����1������Ϊ0��ʾ������
					
				}
				else
				{
					return 0;//����
				}
			}
		}
	}
	if(DataType>=80)
	{
		DataType-=(80-29);//����	29-79	
	}
	if(DataType>=sizeof(DataLen_List)/4)
	{
		return 0;//����
	}
	p32=(u32*)DataLen_List;
	return (*(u32(*)())p32[DataType])((u32)((u32)p8+1),(Attribute_TypeDef *)pDataType,(u32)TypeCount+1);
}

u32 Get_DL698DataLen_S(u8 *p8,u32 LENmax)//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
{
	u32 len;
	len=Get_DL698DataLen(p8,0,0,LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
	len>>=8;
	return len;
}

u32 Deal_DataNull_JS(u8 pDataType,u8* p8)
{
	u64 data;
	data=0;
	switch(pDataType)
	{
		case DataType_double_long://Integer32
			data=MRR((u32)p8,4);
			data=R_Inverse(data,4);
			if(data==0x7fffffff)
				return 1;
			break;
		case DataType_double_long_unsigned://double-long-unsigned
			data=MRR((u32)p8,4);
			data=R_Inverse(data,4);
			if(data==0xffffffff)
				return 1;
			break;
		case DataType_integer://integer
			data=MRR((u32)p8,1);
			data=R_Inverse(data,1);
			if(data==0x7f)
				return 1;
			break;
		case DataType_long://long
			data=MRR((u32)p8,2);
			data=R_Inverse(data,2);
			if(data==0x7fff)
				return 1;
			break;
		case DataType_unsigned://Unsigned8
			data=MRR((u32)p8,1);
			data=R_Inverse(data,1);
			if(data==0xff)
				return 1;
			break;
		case DataType_long_unsigned://Unsigned16
			data=MRR((u32)p8,2);
			data=R_Inverse(data,2);
			if(data==0xffff)
				return 1;
			break;
		default:
			break;;
	}
	return 0;
}
//����Ԫ�ص�ַ
//���:pData=Ҫ��������ݿ�ʼ��ַ
//LENmax_Data=Ҫ��������ݴ����ܵ���󳤶����ڵ�������nand flashʱ�ȶ���sdram�������ֽ���(Ϊ���ټ���,��������������ram�н���),��󳤶�����128k;�����ݱ���ram��ֵ�޹�;��������nand flash��LENmax_Data=0ʱ���Զ���ȡ128k����Ӱ�������ٶ�
//Element��Ҫ��������ݵ�ַ����Ԫ�ؼ���,ÿ1����������Ϊ1��Ԫ��,�ṹ�����鲻�����ж��ٸ�Ԫ�ؼ�Ϊ1��Ԫ��,Ԫ�ش�0��ʼ���,Ҫ�����Ԫ�ش�1��ʼ���0��Ԫ��Ϊ��ʼ��ַ�������
//SubElement������Ԫ��Ϊ�ṹ������,Ҫ������Ԫ�ؽṹ�������ڵ�Ԫ�ص�ַ��ָ����Ԫ�ؼ���,��Ԫ��Ҳ��0��ʼ���,Ҫ�������Ԫ�ش�1��ʼ���0����Ԫ��Ϊ��Ԫ�ص�ַ�������,����Ԫ��ʱ��ֵ����=0
//ע��:����Ԫ��Ϊ����ʱ,��Ԫ�ؼ�����ڼ����������,��Ԫ�ؼ���=(�ڼ���*������Ԫ�ظ���)+��ǰ��Ԫ�ؼ���;
//����:��ַָ��Ԫ�ص������ֽ�,0=����
u8 *Get_Element(u8 *pData,u32 Element,u32 SubElement,u32 LENmax_Data)//����Ԫ�ص�ַ(�����ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
{
	u32 i;
	u32 Len;
	u32 ElementCount;

	if(Element==0)
	{
		return pData;
	}
	if(((u32)pData>=0x30000000)&&((u32)pData<0xb0000000))
	{//NAND FLASH
		if((LENmax_Data>LEN_128KDATABUFF)||(LENmax_Data==0))
		{
			//LENmax_Data=LEN_128KDATABUFF;
//�¼�����ʹ��Լ��
//ADDR_DATABUFF�¼��༭���
//ADDR_128KDATABUFF  32KBYTE ������DL698_DataDef
//ADDR_128KDATABUFF+32K 32KBYTE �¼�����,�����������Ա��
//ADDR_128KDATABUFF+64K 32KBYTE �����¼�����ʱ����
//ADDR_128KDATABUFF+96K 32KBYTE �¼��и������ݵ���ӦOAD˳���array OAD
			LENmax_Data=32*1024;
		}
		MR(ADDR_128KDATABUFF,(u32)pData,LENmax_Data);
		pData=(u8*)ADDR_128KDATABUFF;
	}
	if(LENmax_Data==0)
	{
#ifdef IC_STM32F4xx
//0x08000000   2MB           IFLASH
		if(((u32)pData>=0x08000000)&&((u32)pData<0x08200000))
		{
			LENmax_Data=0x08200000-(u32)pData;
		}
//0x10000000   64KB          CCM DATA RAM
		if(((u32)pData>=0x10000000)&&((u32)pData<0x10010000))
		{
			LENmax_Data=0x10010000-(u32)pData;
		}
//0x20000000   STM32F429=112+16+64=192K    ISRAM
	//0x20000000-0x2001BFFF=112K
	//0x2001C000-0x2001FFFF=16K
	//0x20020000-0x2002FFFF=64K
	//0x40002850 80Byte RTC-BKPSRAM
	//0x40024000 4K(BKPSRAM)
//0x20000000   STM32F469=160+32+128=320K    ISRAM
	//0x20000000-0x20027FFF=160K
	//0x20028000-0x2002FFFF=32K
	//0x20030000-0x2004FFFF=128K
	//0x40024000 4K(BKPSRAM)
		if(((u32)pData>=0x20000000)&&((u32)pData<0x20050000))
		{
			LENmax_Data=0x20050000-(u32)pData;
		}
//0x30000000   128-1024MB    NAND FLASH
//0xA0000000                 QUADSPI
//0xC0000000   8-128MB       SDRAM
	#if IC_MT48LC4M16||IC_MT48LC8M8//8M
		if(((u32)pData>=0xC0000000)&&((u32)pData<0xC0800000))
		{
			//LENmax_Data=0xC0800000-(u32)pData;
			LENmax_Data=LEN_128KDATABUFF;
		}
	#endif
	#if IC_MT48LC8M16||IC_MT48LC16M8//16M
		if(((u32)pData>=0xC0000000)&&((u32)pData<0xC1000000))
		{
			//LENmax_Data=0xC1000000-(u32)pData;
			LENmax_Data=LEN_128KDATABUFF;
		}
	#endif
	#if IC_MT48LC16M16||IC_MT48LC32M8//32M
		if(((u32)pData>=0xC0000000)&&((u32)pData<0xC2000000))
		{
			//LENmax_Data=0xC2000000-(u32)pData;
			LENmax_Data=LEN_128KDATABUFF;
		}
	#endif
	#if IC_MT48LC32M16||IC_MT48LC64M8//64M
		if(((u32)pData>=0xC0000000)&&((u32)pData<0xC4000000))
		{
			//LENmax_Data=0xC4000000-(u32)pData;
			LENmax_Data=LEN_128KDATABUFF;
		}
	#endif
#endif	
#ifdef IC_NUC9xx
		LENmax_Data=LEN_128KDATABUFF;
#endif
	}
	Len=0;
	i=pData[0];
	if((i==DataType_array)||(i==DataType_structure))
	{
		ElementCount=Get_DL698ByteTypeLenValue(pData+1);//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
		if(Element>ElementCount)
		{
			return 0;
		}
		i=pData[1];
		if(i&0x80)
		{
			i&=0x7f;
			i+=2;
			pData+=i;
			Len=i;
		}
		else
		{
			pData+=2;
			Len=2;
		}
		
	}
//	else
//	{
//		return 0;
//	}
	ElementCount=1;
	while(1)
	{
		if(ElementCount==Element)
		{//�ҵ���Ԫ��
			if(SubElement==0)
			{
				return pData;
			}
			i=pData[0];
			if((i==DataType_array)||(i==DataType_structure))
			{
				i=pData[1];
				if(i==0)
				{//�����ṹ��Ԫ�ظ���Ϊ0
					return 0;
				}
				if(i&0x80)
				{
					if((pData[2]|pData[3])==0)
					{//�����ṹ��Ԫ�ظ���Ϊ0
						return 0;
					}
					i&=0x7f;
					i+=2;
					pData+=i;
					Len+=i;
				}
				else
				{
					pData+=2;
					Len+=2;
				}
			}
//			else
//			{
//				return 0;
//			}
			ElementCount=1;
			while(1)
			{
				if(ElementCount==SubElement)
				{//�ҵ���Ԫ��
					return pData;
				}
				i=Get_DL698DataLen_S(pData,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				if(i==0)
				{
					return 0;
				}
				pData+=i;
				Len+=i;
				if(Len>=LENmax_Data)
				{
					return 0;
				}
				ElementCount++;
			}
		}
		i=Get_DL698DataLen_S(pData,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		if(i==0)
		{
			return 0;
		}
		pData+=i;
		Len+=i;
		if(Len>=LENmax_Data)
		{
			return 0;
		}
		ElementCount++;
	}
}

u32 Get_ElementADDR(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data)//����Ԫ�ص�ַ(ʵ�ʵ�ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
{
	u8* p8;
	p8=Get_Element((u8*)ADDR_Data,Element,SubElement,LENmax_Data);//����Ԫ�ص�ַ(�����ַ),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
	if(p8)
	{
		if((ADDR_Data>=0x30000000)&&(ADDR_Data<0xb0000000))
		{//NAND FLASH
			p8-=ADDR_128KDATABUFF;
			ADDR_Data+=(u32)p8;
			return ADDR_Data;
		}
	#ifdef IC_STM32F4xx
		if((ADDR_Data>=0x08000000)&&(ADDR_Data<0x08200000))
		{//IFLASH
			p8-=ADDR_128KDATABUFF;
			ADDR_Data+=(u32)p8;
			return ADDR_Data;
		}
	#endif
	#ifdef IC_NUC9xx
		
	#endif
		return (u32)p8;
	}
	return 0;
}


u32 Compare_TSA_MS(u8 *pADDR_6000,u8 *pTSA,u8 *pMS)//�Ƚ�TSA�Ƿ����MS����,���:pADDR_6000����RAM,pTSA,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
{
	u32 i;
	u32 x;
	u32 n;
	u8 *p8;
	
	i=pMS[0];
	n=pMS[1];
	pMS+=2;
	switch(i)
	{
		case 0://  �޵��ܱ�		[0]��
			return 0;
		case 1://  ȫ���û���ַ  	[1]��
			return 1;
		case 2://  һ���û�����  	[2] 	SEQUENCE OF ǰ���������͵�unsigned��
			for(i=0;i<NUMmax_6000;i++)
			{
				if(pADDR_6000[0]==DataType_structure)
				{//������Ч
					p8=Get_Element(pADDR_6000,2,7,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
					if(p8)
					{
						for(x=0;x<n;x++)
						{
							if(pMS[x]==p8[1])
							{//�û�������ͬ
								//i=Compare_octet_string(pTSA,pADDR_6000+8);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
								i=Compare_TSA_string(pTSA,pADDR_6000+8);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
								if(i==0)
								{
									return 1;
								}
							}
						}
					}
				}
				pADDR_6000+=LENper_6000;
			}
			return 0;
		case 3://  һ���û���ַ  	[3] 	SEQUENCE OF ǰ���������͵�TSA��
			for(i=0;i<n;i++)
			{
				//x=Compare_octet_string(pTSA,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				x=Compare_TSA_string(pTSA,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				if(x==0)
				{
					return 1;
				}
				x=pMS[0];
				x++;
				pMS+=x;
			}
			return 0;
		case 4://  һ���������  	[4] 	SEQUENCE OF ǰ���������͵�long-unsigned��
			for(i=0;i<n;i++)
			{
				x=(pMS[0]<<8)+pMS[1];
				if(x<NUMmax_6000)
				{
					p8=pADDR_6000+(x*LENper_6000)+8;
					//x=Compare_octet_string(pTSA,p8);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					x=Compare_TSA_string(pTSA,p8);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(x==0)
					{
						return 1;
					}
				}
				pMS+=2;
			}
			return 0;
		case 5://һ���û��������� [5] SEQUENCE OF Region�����������͵�unsigned
//68 3D 00 43 05 01 00 00 00 00 00 10 24 03 07 01 01 60 14 7F 00 01 01 02 06 11 05 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 50 02 00 
//5C 05 01 02 11 00 11 01 16 02 00 62 53 16 
			for(i=0;i<NUMmax_6000;i++)
			{
				if(pADDR_6000[0]==DataType_structure)
				{//������Ч
					p8=pADDR_6000+8;
					//x=Compare_octet_string(pTSA,p8);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					x=Compare_TSA_string(pTSA,p8);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(x==0)
					{
						p8=Get_Element(pADDR_6000,2,7,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
						if(p8)
						{
							for(x=0;x<n;x++)
							{
								switch(pMS[0])
								{
									case 0://ǰ�պ�    ��0��
										if((p8[1]>=pMS[2])&&(p8[1]<pMS[4]))
										{
											return 1;
										}
										break;
									case 1://ǰ�����    ��1��
										if((p8[1]>pMS[2])&&(p8[1]<=pMS[4]))
										{
											return 1;
										}
										break;
									case 2://ǰ�պ��    ��2��
										if((p8[1]>=pMS[2])&&(p8[1]<=pMS[4]))
										{
											return 1;
										}
										break;
									case 3://ǰ����    ��3��
										if((p8[1]>pMS[2])&&(p8[1]<pMS[4]))
										{
											return 1;
										}
										break;
								}
								pMS+=5;
							}
						}
						return 0;
					}
				}
				pADDR_6000+=LENper_6000;
			}
			return 0;
		case 6://һ���û���ַ���� [6] SEQUENCE OF Region�����������͵�TSA
//68 4B 00 43 05 01 00 00 00 00 00 10 99 A8 07 01 02 60 14 7F 00 01 01 02 06 11 06 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 60 02 00 
//5C 06 01 02 55 07 05 00 00 00 00 00 01 55 07 05 00 00 00 00 00 02 16 06 00 86 21 16 
			for(i=0;i<n;i++)
			{
				x=pMS[0];
				pMS+=2;
				switch(x)
				{
					case 0://ǰ�պ�    ��0��
						//x=Compare_octet_string(pTSA,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						x=Compare_TSA_string(pTSA,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(x<2)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pTSA,pMS+x);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(pTSA,pMS+x);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x==2)
							{
								return 1;
							}
						}
						break;
					case 1://ǰ�����    ��1��
						//x=Compare_octet_string(pTSA,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						x=Compare_TSA_string(pTSA,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(x==1)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pTSA,pMS+x);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(pTSA,pMS+x);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x!=1)
							{
								return 1;
							}
						}
						break;
					case 2://ǰ�պ��    ��2��
						//x=Compare_octet_string(pTSA,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						x=Compare_TSA_string(pTSA,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(x<2)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pTSA,pMS+x);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(pTSA,pMS+x);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x!=1)
							{
								return 1;
							}
						}
						break;
					case 3://ǰ����    ��3��
						//x=Compare_octet_string(pTSA,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						x=Compare_TSA_string(pTSA,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(x==1)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pTSA,pMS+x);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(pTSA,pMS+x);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x==2)
							{
								return 1;
							}
						}
						break;
				}
				x=pMS[0];
				x++;
				x+=1;
				pMS+=x;
				x=pMS[0];
				x++;
				pMS+=x;
			}
			return 0;
		case 7://һ������������� [7] SEQUENCE OF Region ���������͵�long-unsigned
//68 46 00 43 05 01 00 00 00 00 00 10 F7 1F 07 01 03 60 14 7F 00 01 01 02 06 11 07 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 70 02 00 
//5C 07 02 00 12 00 01 12 00 02 02 12 00 02 12 00 03 16 04 00 9D 35 16 
			for(x=0;x<NUMmax_6000;x++)
			{
				if(pADDR_6000[0]==DataType_structure)
				{//������Ч
					//i=Compare_octet_string(pTSA,pADDR_6000+8);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					i=Compare_TSA_string(pTSA,pADDR_6000+8);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i==0)
					{
						break;
					}
				}
				pADDR_6000+=LENper_6000;
			}
			if(x>=NUMmax_6000)
			{
				return 0;
			}
			//x=�������
			for(i=0;i<n;i++)
			{
				switch(pMS[0])
				{
					case 0://ǰ�պ�    ��0��
						if((x>=((pMS[2]<<8)+pMS[3]))&&(x<((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 1://ǰ�����    ��1��
						if((x>((pMS[2]<<8)+pMS[3]))&&(x<=((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 2://ǰ�պ��    ��2��
						if((x>=((pMS[2]<<8)+pMS[3]))&&(x<=((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 3://ǰ����    ��3��
						if((x>((pMS[2]<<8)+pMS[3]))&&(x<((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
				}
				pMS+=7;
			}
			return 0;
	}
	return 0;
}

u32 Compare_SETNO_MS(u8 *pADDR_6000,u32 SETNO,u8 *pMS)//�Ƚ���������Ƿ����MS����,���:pADDR_6000����RAM,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
{
	u32 i;
	u32 x;
	u32 n;
	u8 *p8;
	
	
 #if RUN_DL698TEST_1==1//0��Ч,1=����;ȫ��������Ϊ�ڲ����ɲ������ݴ洢������
	if(SETNO<=10)
	{
		return 1;
	}
 #endif
	
	pADDR_6000+=SETNO*LENper_6000;
	if(pADDR_6000[0]!=DataType_structure)
	{
		return 0;
	}
	i=pMS[0];
	n=pMS[1];
	pMS+=2;
	switch(i)
	{
		case 0://  �޵��ܱ�		[0]��
			return 0;
		case 1://  ȫ���û���ַ  	[1]��
			return 1;
		case 2://  һ���û�����  	[2] 	SEQUENCE OF unsigned��
			p8=Get_Element(pADDR_6000,2,7,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8)
			{
				for(x=0;x<n;x++)
				{
					if(pMS[x]==p8[1])
					{//�û�������ͬ
						return 1;
					}
				}
			}
			return 0;
		case 3://  һ���û���ַ  	[3] 	SEQUENCE OF TSA��
			for(i=0;i<n;i++)
			{
				//x=Compare_octet_string(pADDR_6000+8,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				x=Compare_TSA_string(pADDR_6000+8,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				if(x==0)
				{
					return 1;
				}
				x=pMS[0];
				x++;
				pMS+=x;
			}
			return 0;
		case 4://  һ���������  	[4] 	SEQUENCE OF long-unsigned��
			for(i=0;i<n;i++)
			{
				x=(pMS[0]<<8)+pMS[1];
				if(x==SETNO)
				{
					return 1;
				}
				pMS+=2;
			}
			return 0;
		case 5://һ���û��������� [5] SEQUENCE OF Region��
//68 3D 00 43 05 01 00 00 00 00 00 10 24 03 07 01 01 60 14 7F 00 01 01 02 06 11 05 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 50 02 00 
//5C 05 01 02 11 00 11 01 16 02 00 62 53 16 
			p8=Get_Element(pADDR_6000,2,7,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8)
			{
				for(x=0;x<n;x++)
				{
					switch(pMS[0])
					{
						case 0://ǰ�պ�    ��0��
							if((p8[1]>=pMS[2])&&(p8[1]<pMS[4]))
							{
								return 1;
							}
							break;
						case 1://ǰ�����    ��1��
							if((p8[1]>pMS[2])&&(p8[1]<=pMS[4]))
							{
								return 1;
							}
							break;
						case 2://ǰ�պ��    ��2��
							if((p8[1]>=pMS[2])&&(p8[1]<=pMS[4]))
							{
								return 1;
							}
							break;
						case 3://ǰ����    ��3��
							if((p8[1]>pMS[2])&&(p8[1]<pMS[4]))
							{
								return 1;
							}
							break;
					}
					pMS+=5;
				}
			}
			return 0;
		case 6://һ���û���ַ���� [6] SEQUENCE OF Region��
//68 4B 00 43 05 01 00 00 00 00 00 10 99 A8 07 01 02 60 14 7F 00 01 01 02 06 11 06 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 60 02 00 
//5C 06 01 02 55 07 05 00 00 00 00 00 01 55 07 05 00 00 00 00 00 02 16 06 00 86 21 16 
			for(i=0;i<n;i++)
			{
				x=pMS[0];
				pMS+=2;
				switch(x)
				{
					case 0://ǰ�պ�    ��0��
						//x=Compare_octet_string(pADDR_6000+8,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						x=Compare_TSA_string(pADDR_6000+8,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(x<2)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pADDR_6000+8,pMS+x);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(pADDR_6000+8,pMS+x);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x==2)
							{
								return 1;
							}
						}
						break;
					case 1://ǰ�����    ��1��
						//x=Compare_octet_string(pADDR_6000+8,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						x=Compare_TSA_string(pADDR_6000+8,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(x==1)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pADDR_6000+8,pMS+x);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(pADDR_6000+8,pMS+x);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x!=1)
							{
								return 1;
							}
						}
						break;
					case 2://ǰ�պ��    ��2��
						//x=Compare_octet_string(pADDR_6000+8,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						x=Compare_TSA_string(pADDR_6000+8,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(x<2)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pADDR_6000+8,pMS+x);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(pADDR_6000+8,pMS+x);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x!=1)
							{
								return 1;
							}
						}
						break;
					case 3://ǰ����    ��3��
						//x=Compare_octet_string(pADDR_6000+8,pMS);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						x=Compare_TSA_string(pADDR_6000+8,pMS);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
						if(x==1)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pADDR_6000+8,pMS+x);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(pADDR_6000+8,pMS+x);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x==2)
							{
								return 1;
							}
						}
						break;
				}
				x=pMS[0];
				x++;
				x+=1;
				pMS+=x;
				x=pMS[0];
				x++;
				pMS+=x;
			}
			return 0;
		case 7://һ������������� [7] SEQUENCE OF Region
//68 46 00 43 05 01 00 00 00 00 00 10 F7 1F 07 01 03 60 14 7F 00 01 01 02 06 11 07 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 70 02 00 
//5C 07 02 00 12 00 01 12 00 02 02 12 00 02 12 00 03 16 04 00 9D 35 16 
			for(i=0;i<n;i++)
			{
				switch(pMS[0])
				{
					case 0://ǰ�պ�    ��0��
						if((SETNO>=((pMS[2]<<8)+pMS[3]))&&(SETNO<((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 1://ǰ�����    ��1��
						if((SETNO>((pMS[2]<<8)+pMS[3]))&&(SETNO<=((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 2://ǰ�պ��    ��2��
						if((SETNO>=((pMS[2]<<8)+pMS[3]))&&(SETNO<=((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 3://ǰ����    ��3��
						if((SETNO>((pMS[2]<<8)+pMS[3]))&&(SETNO<((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
				}
				pMS+=7;
			}
			return 0;
	}
	return 0;
}

void MStoAddrList(u8* pMS)//���ܱ��ϵ���ַ�б�;���:pMS(����������);����:��ַ�б���ADDR_AddrList
{
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 NUM;
	u8 *pADDR_6000;
	u16 *p16s;
	u16* p16d;
	u8 *p8;
	u64 d64;
	u64 d64a;
	u64 d64b;
	
	NUM=0;//�б���
	p16d=(u16*)(ADDR_AddrList+2);
	pADDR_6000=(u8*)ADDR_6000_SDRAM;
	i=pMS[0];
	n=pMS[1];
	pMS+=2;
	switch(i)
	{
		case 0://  �޵��ܱ�		[0]��
			break;
		case 1://  ȫ���û���ַ  	[1]
			for(i=0;i<NUMmax_6000;i++)
			{
				if(pADDR_6000[0]==DataType_structure)
				{
					p16s=(u16*)(pADDR_6000+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
					p16d[0]=p16s[0];
					p16d[1]=p16s[1];
					p16d[2]=p16s[2];
					p16d+=3;
					NUM+=1;
				}
				pADDR_6000+=LENper_6000;
			}
			break;
		case 2://  һ���û�����  	[2] 	SEQUENCE OF unsigned��
			for(x=0;x<n;x++)
			{
				pADDR_6000=(u8*)ADDR_6000_SDRAM;
				for(i=0;i<NUMmax_6000;i++)
				{
					if(pADDR_6000[0]==DataType_structure)
					{
						p8=pADDR_6000+5;
						p8=Get_Element(p8,7,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
						if(p8)
						{
							if(pMS[0]==p8[1])
							{//�û�������ͬ
								if(NUM>=NUMmax_6000)
								{
									break;
								}
								p16s=(u16*)(pADDR_6000+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
								p16d[0]=p16s[0];
								p16d[1]=p16s[1];
								p16d[2]=p16s[2];
								p16d+=3;
								NUM+=1;
							}
						}
					}
					pADDR_6000+=LENper_6000;
				}
				pMS++;
			}
			break;
		case 3://  һ���û���ַ  	[3] 	SEQUENCE OF TSA��
			//03 01 07 05 00 00 00 00 00 01
			for(i=0;i<n;i++)
			{
				p8=(u8*)(pMS+2);
				p16d[0]=p8[0]|(p8[1]<<8);
				p16d[1]=p8[2]|(p8[3]<<8);
				p16d[2]=p8[4]|(p8[5]<<8);
				p16d+=3;
				NUM+=1;
				
				x=pMS[0];
				x++;
				pMS+=x;
			}
			break;
		case 4://  һ���������  	[4] 	SEQUENCE OF long-unsigned��
			for(i=0;i<n;i++)
			{
				x=(pMS[0]<<8)+pMS[1];
				if(x<NUMmax_6000)
				{
					p8=(u8*)pADDR_6000+(x*LENper_6000);
					if(p8[0]==DataType_structure)
					{
						p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
						p16d[0]=p16s[0];
						p16d[1]=p16s[1];
						p16d[2]=p16s[2];
						p16d+=3;
						NUM+=1;
					}
				}
				pMS+=2;
			}
			break;
		case 5://һ���û��������� [5] SEQUENCE OF Region��
			for(x=0;x<n;x++)
			{
				switch(pMS[0])
				{
					case 0://ǰ�պ�    ��0��
						pADDR_6000=(u8*)ADDR_6000_SDRAM;
						for(i=0;i<NUMmax_6000;i++)
						{
							if(pADDR_6000[0]==DataType_structure)
							{
								p8=pADDR_6000+5;
								p8=Get_Element(p8,7,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(p8)
								{
									if((p8[1]>=pMS[2])&&(p8[1]<pMS[4]))
									{//�û�������ͬ
										if(NUM>=NUMmax_6000)
										{
											break;
										}
										p16s=(u16*)(pADDR_6000+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
										p16d[0]=p16s[0];
										p16d[1]=p16s[1];
										p16d[2]=p16s[2];
										p16d+=3;
										NUM+=1;
									}
								}
							}
							pADDR_6000+=LENper_6000;
						}
						break;
					case 1://ǰ�����    ��1��
						pADDR_6000=(u8*)ADDR_6000_SDRAM;
						for(i=0;i<NUMmax_6000;i++)
						{
							if(pADDR_6000[0]==DataType_structure)
							{
								p8=pADDR_6000+5;
								p8=Get_Element(p8,7,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(p8)
								{
									if((p8[1]>pMS[2])&&(p8[1]<=pMS[4]))
									{//�û�������ͬ
										if(NUM>=NUMmax_6000)
										{
											break;
										}
										p16s=(u16*)(pADDR_6000+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
										p16d[0]=p16s[0];
										p16d[1]=p16s[1];
										p16d[2]=p16s[2];
										p16d+=3;
										NUM+=1;
									}
								}
							}
							pADDR_6000+=LENper_6000;
						}
						break;
					case 2://ǰ�պ��    ��2��
						pADDR_6000=(u8*)ADDR_6000_SDRAM;
						for(i=0;i<NUMmax_6000;i++)
						{
							if(pADDR_6000[0]==DataType_structure)
							{
								p8=pADDR_6000+5;
								p8=Get_Element(p8,7,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(p8)
								{
									if((p8[1]>=pMS[2])&&(p8[1]<=pMS[4]))
									{//�û�������ͬ
										if(NUM>=NUMmax_6000)
										{
											break;
										}
										p16s=(u16*)(pADDR_6000+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
										p16d[0]=p16s[0];
										p16d[1]=p16s[1];
										p16d[2]=p16s[2];
										p16d+=3;
										NUM+=1;
									}
								}
							}
							pADDR_6000+=LENper_6000;
						}
						break;
					case 3://ǰ����    ��3��
						pADDR_6000=(u8*)ADDR_6000_SDRAM;
						for(i=0;i<NUMmax_6000;i++)
						{
							if(pADDR_6000[0]==DataType_structure)
							{
								p8=pADDR_6000+5;
								p8=Get_Element(p8,7,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(p8)
								{
									if((p8[1]>pMS[2])&&(p8[1]<pMS[4]))
									{//�û�������ͬ
										if(NUM>=NUMmax_6000)
										{
											break;
										}
										p16s=(u16*)(pADDR_6000+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
										p16d[0]=p16s[0];
										p16d[1]=p16s[1];
										p16d[2]=p16s[2];
										p16d+=3;
										NUM+=1;
									}
								}
							}
							pADDR_6000+=LENper_6000;
						}
						break;
				}
				pMS+=5;
			}
			break;
		case 6://һ���û���ַ���� [6] SEQUENCE OF Region��
			for(i=0;i<n;i++)
			{
				x=pMS[0];
				pMS++;
				d64a=MRR((u32)pMS+3,pMS[2]+1);
				pMS+=pMS[1]+2;
				d64b=MRR((u32)pMS+3,pMS[2]+1);
				pMS+=pMS[1]+2;
				d64a=R_Inverse(d64a,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
				d64b=R_Inverse(d64b,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
				d64a=bcd16_hex(d64a);
				d64b=bcd16_hex(d64b);
				switch(x)
				{
					case 0://ǰ�պ�    ��0��
						while(d64a<d64b)
						{
							if(NUM>=NUMmax_6000)
							{
								break;
							}
							d64=hex_bcd16(d64a);
							d64=R_Inverse(d64,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							p16d[0]=d64;
							d64>>=16;
							p16d[1]=d64;
							d64>>=16;
							p16d[2]=d64;
							p16d+=3;
							NUM+=1;
							d64a+=1;
						}
						break;
					case 1://ǰ�����    ��1��
						d64a+=1;
						while(d64a<=d64b)
						{
							if(NUM>=NUMmax_6000)
							{
								break;
							}
							d64=hex_bcd16(d64a);
							d64=R_Inverse(d64,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							p16d[0]=d64;
							d64>>=16;
							p16d[1]=d64;
							d64>>=16;
							p16d[2]=d64;
							p16d+=3;
							NUM+=1;
							d64a+=1;
						}
						break;
					case 2://ǰ�պ��    ��2��
						while(d64a<=d64b)
						{
							if(NUM>=NUMmax_6000)
							{
								break;
							}
							d64=hex_bcd16(d64a);
							d64=R_Inverse(d64,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							p16d[0]=d64;
							d64>>=16;
							p16d[1]=d64;
							d64>>=16;
							p16d[2]=d64;
							p16d+=3;
							NUM+=1;
							d64a+=1;
						}
						break;
					case 3://ǰ����    ��3��
						d64a+=1;
						while(d64a<d64b)
						{
							if(NUM>=NUMmax_6000)
							{
								break;
							}
							d64=hex_bcd16(d64a);
							d64=R_Inverse(d64,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							p16d[0]=d64;
							d64>>=16;
							p16d[1]=d64;
							d64>>=16;
							p16d[2]=d64;
							p16d+=3;
							NUM+=1;
							d64a+=1;
						}
						break;
				}
			}
			break;
		case 7://һ������������� [7] SEQUENCE OF Region
			for(x=0;x<n;x++)
			{
				switch(pMS[0])
				{
					case 0://ǰ�պ�    ��0��
						i=(pMS[2]<<8)+pMS[3];
						y=(pMS[5]<<8)+pMS[6];
						while(i<y)
						{
							if(i<NUMmax_6000)
							{
								if(NUM>=NUMmax_6000)
								{
									break;
								}
								p8=(u8*)pADDR_6000+(i*LENper_6000);
								if(p8[0]==DataType_structure)
								{
									p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
									p16d[0]=p16s[0];
									p16d[1]=p16s[1];
									p16d[2]=p16s[2];
									p16d+=3;
									NUM+=1;
								}
							}
							i++;
						}
						break;
					case 1://ǰ�����    ��1��
						i=(pMS[2]<<8)+pMS[3];
						i++;
						y=(pMS[5]<<8)+pMS[6];
						while(i<=y)
						{
							if(i<NUMmax_6000)
							{
								if(NUM>=NUMmax_6000)
								{
									break;
								}
								p8=(u8*)pADDR_6000+(i*LENper_6000);
								if(p8[0]==DataType_structure)
								{
									p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
									p16d[0]=p16s[0];
									p16d[1]=p16s[1];
									p16d[2]=p16s[2];
									p16d+=3;
									NUM+=1;
								}
							}
							i++;
						}
						break;
					case 2://ǰ�պ��    ��2��
						i=(pMS[2]<<8)+pMS[3];
						y=(pMS[5]<<8)+pMS[6];
						while(i<=y)
						{
							if(i<NUMmax_6000)
							{
								if(NUM>=NUMmax_6000)
								{
									break;
								}
								p8=(u8*)pADDR_6000+(i*LENper_6000);
								if(p8[0]==DataType_structure)
								{
									p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
									p16d[0]=p16s[0];
									p16d[1]=p16s[1];
									p16d[2]=p16s[2];
									p16d+=3;
									NUM+=1;
								}
							}
							i++;
						}
						break;
					case 3://ǰ����    ��3��
						i=(pMS[2]<<8)+pMS[3];
						i++;
						y=(pMS[5]<<8)+pMS[6];
						while(i<y)
						{
							if(i<NUMmax_6000)
							{
								if(NUM>=NUMmax_6000)
								{
									break;
								}
								p8=(u8*)pADDR_6000+(i*LENper_6000);
								if(p8[0]==DataType_structure)
								{
									p16s=(u16*)(p8+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
									p16d[0]=p16s[0];
									p16d[1]=p16s[1];
									p16d[2]=p16s[2];
									p16d+=3;
									NUM+=1;
								}
							}
							i++;
						}
						break;
				}
				pMS+=7;
			}
			break;
	}
	p16d=(u16*)(ADDR_AddrList);
	p16d[0]=NUM;
}

u32 PORTntoOAD(u32 PORTn)//ͨ�ſں�תΪOADֵ
{
//#define OAD_RS232     0xF2000201
//#define OAD_GPRS     	0x45000000
//#define OAD_RS4851     0xF2010201
//#define OAD_RS4852     0xF2010202
//#define OAD_RS4854     0xF2090201//�ز�/΢�������߽ӿ�
//#define OAD_RS4853     0xF2010203//
//#define OAD_ENET     	0x45100000
//#define OAD_ACSAMPLE     0xF2080201//���ɽӿ�
	switch(PORTn)
	{
//#define RS232PORT     0//RS232ͨ�ſں�
		case 0:
			return 0xF2000201;
//#define GPRSPORT     1//GPRSͨ�ſں�
		case 1:
			return 0x45000201;
//#define RS485_1PORT     2//RS485_1ͨ�ſں�
		case 2:
			return 0xF2010201;
//#define RS485_2PORT     3//RS485-2ͨ�ſں�
		case 3:
			return 0xF2010202;
//#define RS485_4PORT     4//RS485_4ͨ�ſں�(�������ز�,ר�����ģ��)
		case 4:
			return 0xF2090201;
//#define RS485_3PORT     5//RS485_3ͨ�ſں�(WI-FI)
		case 5:
			return 0xF2010203;
//#define IRPORT     6//����ͨ�ſں�
//#define TESAMPORT     7//�ն�ESAMͨ�ſں�
//#define MESAMPORT     8//���ESAMͨ�ſں�
//#define CARDPORT     9//���CPUCARDͨ�ſں�
//#define ENETPORT     10//��̫��ͨ�ſں�
		case 10:
			return 0x45100201;
//#define SMSPORT     11//����ͨ�ſں�
//#define ACSAMPLEPORT     12//��������ͨ�ſں�(����)
		case 12:
			return 0xF2080201;
//#define EFTPCMDPORT     13//UART13(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
//#define EFTPDATAPORT     14//UART14(��̫��FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
//#define GFTPCMDPORT     15//UART15(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
//#define GFTPDATAPORT     16//UART16(����FTP����,ʹ��ADDR_DATABUFF��ADDR_4KWRITE_BUFF������8K����)
		default:
			return 0;
	}
}

u32 OADtoPORTn(u32 OAD)//OADֵתΪͨ�ſں�
{
	switch(OAD)
	{
		case 0xF2010201://RS485_1
			return 2;
		case 0xF2010202://RS485_2
			return 3;
		case 0xF2010203://RS485_3
			return 5;
		default:
			OAD>>=16;
			switch(OAD)
			{
				//case 0xF2000201://RS232
				case 0xF200:
					return 0;
				//case 0x45000201://GPRS
				case 0x4500:
					return 1;
				//case 0xF2090201://RS485_4ͨ�ſں�(�������ز�,ר�����ģ��)
				case 0xF209:
					return 4;
				//case 0x45100201://ENET
				case 0x4510:
					return 10;
				//case 0xF2080201://��������ͨ�ſں�(����)
				case 0xF208:
					return 12;
				default:
					return 0;
			}
	}
}


u32 Compare_DL698DataString(u8 *p1,u8 *p2,u32 Byte)//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
{
	u32 i;
	u32 x;
	u32 y;
	
	for(i=0;i<Byte;i++)
	{
		x=p1[i];
		y=p2[i];
		if(x!=y)
		{
			if(x>y)
			{
				return 1;//p1>p2
			}
			return 2;//p1<p2
		}
	}
	return 0;
}

//u32 Compare_octet_string(u8 *p1,u8 *p2)//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//{
//	u32 i;
//	u32 x;
//	u32 y;
//	
//	x=p1[0];
//	y=p2[0];
//	if(x>=y)
//	{//������p1>=p2
//		if(x==y)
//		{//������p1=p2
//			while(x)
//			{
//				if(p1[x]!=p2[x])
//				{
//					if(p1[x]>p2[x])
//					{
//						return 1;//p1>p2
//					}
//					return 2;//p1<p2
//				}
//				x--;
//			}
//			return 0;
//		}
//		else
//		{//������p1>p2
//			x-=y;
//			for(i=0;i<x;i++)
//			{
//				if(p1[y+i])
//				{
//					return 1;//p1>p2
//				}
//			}
//			while(y)
//			{
//				if(p1[y]!=p2[y])
//				{
//					if(p1[y]>p2[y])
//					{
//						return 1;//p1>p2
//					}
//					return 2;//p1<p2
//				}
//				y--;
//			}
//			return 0;
//		}
//	}
//	else
//	{//������p1<p2
//		y-=x;
//		for(i=0;i<y;i++)
//		{
//			if(p2[x+i])
//			{
//				return 2;//p1<p2
//			}
//		}
//		while(x)
//		{
//			if(p1[x]!=p2[x])
//			{
//				if(p1[x]>p2[x])
//				{
//					return 1;//p1>p2
//				}
//				return 2;//p1<p2
//			}
//			x--;
//		}
//		return 0;
//	}
//}

u32 Compare_octet_string(u8 *p1,u8 *p2)//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
{
	u32 i;
	u32 x;
	u32 y;
	
	x=p1[0];
	y=p2[0];
	if(x>=y)
	{//������p1>=p2
		if(x==y)
		{//������p1=p2
			for(i=1;i<=x;i++)
			{
				if(p1[i]!=p2[i])
				{
					if(p1[i]>p2[i])
					{
						return 1;//p1>p2
					}
					return 2;//p1<p2
				}
			}
			return 0;
		}
		else
		{//������p1>p2
			x-=y;
			for(i=1;i<=x;i++)
			{
				if(p1[i])
				{
					return 1;//p1>p2
				}
			}
			for(i=1;i<=y;i++)
			{
				if(p1[x+i]!=p2[i])
				{
					if(p1[x+i]>p2[i])
					{
						return 1;//p1>p2
					}
					return 2;//p1<p2
				}
				
			}
			return 0;
		}
	}
	else
	{//������p1<p2
		y-=x;
		for(i=1;i<=y;i++)
		{
			if(p2[i])
			{
				return 2;//p1<p2
			}
		}
		for(i=1;i<=x;i++)
		{
			if(p1[i]!=p2[y+i])
			{
				if(p1[i]>p2[y+i])
				{
					return 1;//p1>p2
				}
				return 2;//p1<p2
			}
		}
		return 0;
	}
}

u32 Compare_TSA_string(u8 *p1,u8 *p2)//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
{
	u32 i;
	u32 x;
	u32 y;
	
	x=p1[0];
	y=p2[0];
	if((x==0)||(y==0))
	{
		return 0;
	}
	x--;
	y--;
	p1++;
	p2++;
	if(x>=y)
	{//������p1>=p2
		if(x==y)
		{//������p1=p2
			for(i=1;i<=x;i++)
			{
				if(p1[i]!=p2[i])
				{
					if(p1[i]>p2[i])
					{
						return 1;//p1>p2
					}
					return 2;//p1<p2
				}
			}
			return 0;
		}
		else
		{//������p1>p2
			x-=y;
			for(i=1;i<=x;i++)
			{
				if(p1[i])
				{
					return 1;//p1>p2
				}
			}
			for(i=1;i<=y;i++)
			{
				if(p1[x+i]!=p2[i])
				{
					if(p1[x+i]>p2[i])
					{
						return 1;//p1>p2
					}
					return 2;//p1<p2
				}
				
			}
			return 0;
		}
	}
	else
	{//������p1<p2
		y-=x;
		for(i=1;i<=y;i++)
		{
			if(p2[i])
			{
				return 2;//p1<p2
			}
		}
		for(i=1;i<=x;i++)
		{
			if(p1[i]!=p2[y+i])
			{
				if(p1[i]>p2[y+i])
				{
					return 1;//p1>p2
				}
				return 2;//p1<p2
			}
		}
		return 0;
	}
}

u16 *MeterSetAddrNo(u16* pCollectorAddr)
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	u32 x;
	u64 d64a;
	u64 d64b;
	u16 *pAddr;
	u16 *pMeterNo;
	u32 j;
	
	pMeterNo=(u16*)ADDR_DATABUFF;
	p8=(u8*)pCollectorAddr;
	Addr0=p8[0]|(p8[1]<<8);
	Addr1=p8[2]|(p8[3]<<8);
	Addr2=p8[4]|(p8[5]<<8);

	x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���

	d64a=MRR((u32)p8,6);
	for(i=0;i<x;i++)
	{
		d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
		d64b=R_Inverse(d64b,6);
		if(d64a==d64b)
		{
			d64b=MRR((u32)&NoteList->List[i].Addr,6);
			d64b=R_Inverse(d64b,6);
			MWR(d64b,(u32)p8,6);
			Addr0=p8[0]|(p8[1]<<8);
			Addr1=p8[2]|(p8[3]<<8);
			Addr2=p8[4]|(p8[5]<<8);
			break;//ԭ�Ѵ���
		}
		p8=(u8*)ADDR_6000_SDRAM;
		for(i=0;i<NUMmax_6000;i++)
		{
			if(p8[0]==DataType_structure)
			{
				pAddr=(u16*)(p8+10);
				if(pAddr[0]==Addr0)
				{
					if(pAddr[1]==Addr1)
					{
						if(pAddr[2]==Addr2)
						{
							pMeterNo[j]=i;
							j++;
						}
					}
				}
			}
			p8+=LENper_6000;
		}
	}
	
	return pMeterNo;
}

u32 AddrToMeterSetNoJS(u16* pAddr)
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	u32 x;
	u64 d64a;
	u64 d64b;
	
	p8=(u8*)pAddr;
	Addr0=p8[0]|(p8[1]<<8);
	Addr1=p8[2]|(p8[3]<<8);
	Addr2=p8[4]|(p8[5]<<8);
//	p8=(u8*)ADDR_Meter_Collector;
//	for(i=0;i<NUM_RMmax;i++)
//	{
//		Data_Inverse((u32)p8+6,6);
//		pAddr=(u16*)(p8+6);
//		if(pAddr[0]==Addr0)
//		{
//			if(pAddr[1]==Addr1)
//			{
//				if(pAddr[2]==Addr2)
//				{
//					Data_Inverse((u32)p8,6);
//					Addr0=p8[0]|(p8[1]<<8);
//					Addr1=p8[2]|(p8[3]<<8);
//					Addr2=p8[4]|(p8[5]<<8);
//					Data_Inverse((u32)p8,6);
//					break;
//				}
//			}
//		}
//		Data_Inverse((u32)p8+6,6);
//		p8+=12;
//	}
#if(USER/100)==17//����ϵ
	#if 0
	x=Terminal_Router->NoteNum1;//��֪�ܴӽڵ���

	d64a=MRR((u32)p8,6);
	for(i=0;i<x;i++)
	{
		d64b=MRR((u32)&NoteList1->List1[i].CollectorAddr,6);
		d64b=R_Inverse(d64b,6);
		if(d64a==d64b)
		{
			d64b=MRR((u32)&NoteList1->List1[i].Addr,6);
			d64b=R_Inverse(d64b,6);
			MWR(d64b,(u32)p8,6);
			Addr0=p8[0]|(p8[1]<<8);
			Addr1=p8[2]|(p8[3]<<8);
			Addr2=p8[4]|(p8[5]<<8);
			break;//ԭ�Ѵ���
		}
	}
	#else
	x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���

	d64a=MRR((u32)p8,6);
	for(i=0;i<x;i++)
	{
		d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
		d64b=R_Inverse(d64b,6);
		if(d64a==d64b)
		{
			d64b=MRR((u32)&NoteList->List[i].Addr,6);
			d64b=R_Inverse(d64b,6);
			MWR(d64b,(u32)p8,6);
			Addr0=p8[0]|(p8[1]<<8);
			Addr1=p8[2]|(p8[3]<<8);
			Addr2=p8[4]|(p8[5]<<8);
			break;//ԭ�Ѵ���
		}
	}
	#endif
#endif
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			pAddr=(u16*)(p8+10);
			if(pAddr[0]==Addr0)
			{
				if(pAddr[1]==Addr1)
				{
					if(pAddr[2]==Addr2)
					{
						return i;
					}
				}
			}
		}
		p8+=LENper_6000;
	}
	return 0xffff;
}
u32 AddrToMeterSetNo(u16* pAddr)//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	
	p8=(u8*)pAddr;
	Addr0=p8[0]|(p8[1]<<8);
	Addr1=p8[2]|(p8[3]<<8);
	Addr2=p8[4]|(p8[5]<<8);
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			pAddr=(u16*)(p8+10);
			if(pAddr[0]==Addr0)
			{
				if(pAddr[1]==Addr1)
				{
					if(pAddr[2]==Addr2)
					{
						return i;
					}
				}
			}
		}
		p8+=LENper_6000;
	}
	return 0xffff;
}

u32 AddrTowToMeterSetNo(u16* pAddr)//����ͨ�ŵ�ַ�ɼ�����ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	u8* p8Data;
	
	p8=(u8*)pAddr;
	Addr0=p8[0]|(p8[1]<<8);
	Addr1=p8[2]|(p8[3]<<8);
	Addr2=p8[4]|(p8[5]<<8);
	p8=(u8*)ADDR_6000_SDRAM;
	for(i=0;i<NUMmax_6000;i++)
	{
		if(p8[0]==DataType_structure)
		{
			pAddr=(u16*)(p8+10);
			if(pAddr[0]==Addr0)
			{
				if(pAddr[1]==Addr1)
				{
					if(pAddr[2]==Addr2)
					{
						return i;
					}
				}
			}
			p8Data=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ
			if(p8Data[0]==DataType_structure)
			{
				p8Data+=5;
				if((p8Data[0]+(p8Data[1]<<8))==Addr0)
				{
					if((p8Data[2]+(p8Data[3]<<8))==Addr1)
					{
						if((p8Data[4]+(p8Data[5]<<8))==Addr2)
						{
							return i;
						}
					}
				}
			}
		}
		p8+=LENper_6000;
	}
	return 0xffff;
}

#endif













