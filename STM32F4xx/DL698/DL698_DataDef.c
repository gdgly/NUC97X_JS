

#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698TEST.h"


//得到date_time_s共7字节
void Get_date_time_s(u8* p8d)//得到date_time_s共7字节
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

//数据倒序
u64 R_Inverse(u64 Data,u32 Byte)//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
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
//位倒序
u32 Bit_Inverse(u32 Data,u32 bit)//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
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

//得到长度单元的值
u32 Get_DL698ByteTypeLenByte(u8 *p8)//得到DL698字节类型用来表示长度单元的字节数
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
u32 Get_DL698ByteTypeLenValue(u8 *p8)//得到DL698字节类型长度单元的值
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
u32 Get_DL698BinTypeLenByte(u8 *p8)//得到DL698位类型用来表示长度单元的字节数
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
u32 Get_DL698BinTypeLenValue(u8 *p8)//得到DL698位类型长度长度单元的值转为字节数
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

u64 Get_TSA_Value(u8 *p8)//得到TSA的不大于6字节通信地址,数据顺序不变,入口:p8=TSA类型后的字节，若无效值则=0
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

//TI转为秒值
u32 TItoSECOND(u32 TI)//TI转为秒值,入口:单位在低字节,出口:hex秒
{
	u32 i;
	i=TI>>16;
	i|=TI&0xff00;
	switch(TI&0xff)
	{
		case 0://  秒      （0），
			break;
		case 1://分      （1），
			i*=60;
			break;
		case 2://时      （2），
			i*=60*60;
			break;
		case 3://日      （3），
			i*=24*60*60;
			break;
		case 4://月      （4），
			i*=31*24*60*60;
			break;
		case 5://年      （5）
			i*=365*24*60*60;
			break;
		default:
			i*=365*24*60*60;
			break;
	}
	return i;
}

u32 pTItoSECOND(u8 *pTI)//TI转为秒值,入口:pTI无数据类型;出口:hex秒
{
	u32 i;
	i=(pTI[1]<<8)+pTI[2];//取频率值
	switch(pTI[0])
	{
		case 0://  秒      （0），
			break;
		case 1://分      （1），
			i*=60;
			break;
		case 2://时      （2），
			i*=60*60;
			break;
		case 3://日      （3），
			i*=24*60*60;
			break;
		case 4://月      （4），
			i*=31*24*60*60;
			break;
		case 5://年      （5）
			i*=365*24*60*60;
			break;
		default:
			i*=365*24*60*60;
			break;
	}
	return i;
}

void pYYMDHMS_AddTI_hex(u8* pYYMDHMS,u8* pTI)//年年月日时分秒加时间间隔TI,入口:pYYMDHMS,pTI无数据类型,返回加间隔TI后的年年月日时分秒
{
	u32 i;
	i=(pTI[1]<<8)+pTI[2];//取频率值
	switch(pTI[0])
	{
		case 0://  秒      （0），
			break;
		case 1://分      （1），
			i*=60;
			break;
		case 2://时      （2），
			i*=60*60;
			break;
		case 3://日      （3），
			i*=24*60*60;
			break;
		case 4://月      （4），
			pYYM_AddM_hex(pYYMDHMS,i);//年年月加m月,返回加m月后的年年月
			return;
		case 5://年      （5）
			i*=365*24*60*60;
			break;
		default:
			i*=365*24*60*60;
			break;
	}
	pYYMDHMS_AddS_hex(pYYMDHMS,i);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
}


//********长度计算***********
u32 DataLen_NULL(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	0//	空
{
	return (1<<8)+TypeCount;
}
u32 DataLen_array(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	1//	SEQUENCE OF Data（见6.3.3.1　）数组的元素在对象属性或方法的描述中定义
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
	{//本数组包括的数据类型个数,用于数组=0时跳过元素个数用
		if(pDataType)
		{
			TypeCount+=pDataType->TypeList[TypeCount];
			TypeCount+=1;//本数组包括的数据类型个数1byte
		}
	}
	else
	{
		TypeCount++;//跳过数组类型下的类型个数字节
	}
	Len_Element=TypeCount;
	while(Array--)
	{
		Len_Element=Get_DL698DataLen(p8,pDataType,TypeCount,0);//得到DL698数据类型长度
		if(pDataType)
		{
			if((Len_Element&0xff)==0)
			{
				return 0;//错误
			}
		}
		Len+=Len_Element>>8;
		p8+=Len_Element>>8;
		if(Len>LENmax_APDU)
		{
			return 0;//APDU最大长度限制
		}
	}
	TypeCount=Len_Element&0xff;
	return (Len<<8)+TypeCount;
}
u32 DataLen_structure(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	2//	SEQUENCE OF Data（见6.3.3.1　）结构的元素在对象属性或方法的描述中定义	
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
		Len_Element=Get_DL698DataLen(p8,pDataType,TypeCount,0);//得到DL698数据类型长度
		TypeCount=Len_Element&0xff;
		if(pDataType)
		{
			if(TypeCount==0)
			{
				return 0;//错误
			}
		}
		Len_Element>>=8;
		p8+=Len_Element;
		Len+=Len_Element;
		if(Len>LENmax_APDU)
		{
			return 0;//APDU最大长度限制
		}
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_bool(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	3//	布尔值（BOOLEAN）	1或0
{
	return (2<<8)+TypeCount;
}
u32 DataLen_bit_string(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	4//	比特位串（BIT STRING）	
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
		return 0;//APDU最大长度限制
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_double_long(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	5//	32比特位整数（Integer32）	-231…231-1
{
	return ((1+4)<<8)+TypeCount;
}
u32 DataLen_double_long_unsigned(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	6//	32比特位正整数（double-long-unsigned）	0…232-1
{
	return ((1+4)<<8)+TypeCount;
}
//保留	7-8		
u32 DataLen_octet_string(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	9//	8比特位位组（字节）串（OCTET STRING）	
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
		return 0;//APDU最大长度限制
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_visible_string(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	10//	ASCII字符串（VisibleString）	
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
		return 0;//APDU最大长度限制
	}
	return (Len<<8)+TypeCount;
}
//保留	11		
u32 DataLen_UTF8_string(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	12//	UTF-8编码的字符串	
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
		return 0;//APDU最大长度限制
	}
	return (Len<<8)+TypeCount;
}
//保留	13-14		
u32 DataLen_integer(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	15//	8比特位整数（integer）	-128…127
{
	return ((1+1)<<8)+TypeCount;
}
u32 DataLen_long(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	16//	16比特位整数（long）	-32768…32767
{
	return ((1+2)<<8)+TypeCount;
}
u32 DataLen_unsigned(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	17//	8比特位正整数（Unsigned8）	0…255
{
	return ((1+1)<<8)+TypeCount;
}
u32 DataLen_long_unsigned(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	18//	16比特位正整数（Unsigned16）	0…65535
{
	return ((1+2)<<8)+TypeCount;
}
//保留	19		
u32 DataLen_long64(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	20//	64比特位整数（Integer64）	-263…263-1
{
	return ((1+8)<<8)+TypeCount;
}
u32 DataLen_long64_unsigned(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	21//	64比特位正整数（Unsigned64）	0…264-1
{
	return ((1+8)<<8)+TypeCount;
}
u32 DataLen_enum(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	22//	枚举的元素在对象属性或方法的描述中定义	0…255
{
	return ((1+1)<<8)+TypeCount;
}
u32 DataLen_float32(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	23//	octet-string（SIZE（4））
{
	return ((1+4)<<8)+TypeCount;
}
u32 DataLen_float64(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	24//	octet-string（SIZE（8））
{
	return ((1+8)<<8)+TypeCount;
}
u32 DataLen_date_time(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	25//	octet-string（SIZE（10））
{
	return ((1+10)<<8)+TypeCount;
}
u32 DataLen_date(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	26//	octet-string（SIZE（5））
{
	return ((1+5)<<8)+TypeCount;
}
u32 DataLen_time(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	27//	octet-string（SIZE（3））
{
	return ((1+3)<<8)+TypeCount;
}
u32 DataLen_date_time_s(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	28//octet-string（SIZE（7））
{
	return ((1+7)<<8)+TypeCount;
}

//保留	29-79	
u32 DataLen_OI(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	80//	见6.3.3.30　
{
	return ((1+2)<<8)+TypeCount;
}
u32 DataLen_OAD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	81//	见6.3.3.11　
{
	return ((1+4)<<8)+TypeCount;
}
u32 DataLen_ROAD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	82//	见6.3.3.12　
{
	u32 i;
	i=p8[4];
	i*=4;
	i+=6;
	return (i<<8)+TypeCount;
}
u32 DataLen_OMD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	83//	见6.3.3.18　
{
	return ((1+4)<<8)+TypeCount;
}
u32 DataLen_TI(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	84//	见6.3.3.28　
{
	return ((1+3)<<8)+TypeCount;
}
u32 DataLen_TSA(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	85//	见6.3.3.19　
{
	u32 i;
	i=p8[0];
	i+=2;
	return (i<<8)+TypeCount;
}
u32 DataLen_MAC(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	86//	见6.3.3.21　
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
		return 0;//APDU最大长度限制
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_RN(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	87//	见6.3.3.24　
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
		return 0;//APDU最大长度限制
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_Region(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	88//	见6.3.3.29　
{
	return (0<<8)+TypeCount;
}
u32 DataLen_Scaler_Unit(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	89//	见6.3.3.20　
{
	return ((1+2)<<8)+TypeCount;
}
u32 DataLen_RSD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	90//	见6.3.3.13　
{
	u32 i;
	u32 Len;
	switch(p8[0])
	{
		case 0:
//不选择       [0]  NULL
			Len=2;
			break;
		case 1:
//Selector1∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  数值            Data
//}
			p8+=5;
			Len=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			Len+=1+5;
			break;
		case 2:
//Selector2∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  起始值          Data，
//  结束值          Data，
//  数据间隔        Data
//}
			Len=5;
			Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			Len+=1;
			break;
		case 3:
//Selector3∷=SEQUENCE OF Selector2
			i=p8[1];//SEQUENCE OF
			Len=2;
			while(i--)
			{
				Len+=4;
				Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			}
			Len+=1;
			break;
		case 4:
//Selector4∷=SEQUENCE
//{
//  采集启动时间  date_time_s，
//  电能表集合    MS
//}
			Len=DataLen_MS(p8+1+7,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MS没数据类型
			}
			Len+=2+7;
			break;
		case 5:
//Selector5∷=SEQUENCE
//{
//  采集存储时间  date_time_s，
//  电能表集合    MS
//}
			Len=DataLen_MS(p8+1+7,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MS没数据类型
			}
			Len+=2+7;
			break;
		case 6:
//Selector6∷=SEQUENCE
//{
//  采集启动时间起始值  date_time_s，
//  采集启动时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			Len=DataLen_MS(p8+1+17,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MS没数据类型
			}
			Len+=2+17;
			break;
		case 7:
//Selector7∷=SEQUENCE
//{
//  采集存储时间起始值  date_time_s，
//  采集存储时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			Len=DataLen_MS(p8+1+17,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MS没数据类型
			}
			Len+=2+17;
			break;
		case 8:
//Selector8∷=SEQUENCE
//{
//  采集成功时间起始值  date_time_s，
//  采集成功时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			Len=DataLen_MS(p8+1+17,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MS没数据类型
			}
			Len+=2+17;
			break;
		case 9:
//Selector9∷=SEQUENCE
//{
//  上第n次记录  unsigned
//}
			Len=2+1;
			break;
		case 10:
//Selector10∷=SEQUENCE
//{
//上n条记录  unsigned，
//电能表集合  MS
//}
			Len=DataLen_MS(p8+1+1,0,0);
			Len>>=8;
			if(Len)
			{
				Len--;//MS没数据类型
			}
			Len+=2+1;
			break;
		default:
			Len=2;
			break;
	}
	if(Len>LENmax_APDU)
	{
		return 0;//APDU最大长度限制
	}
	return (Len<<8)+TypeCount;
}
u32 DataLen_RSD_NoMS(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//内联的MS长度不计
{
	u32 i;
	u32 Len;
	switch(p8[0])
	{
		case 0:
//不选择       [0]  NULL
			Len=2;
			break;
		case 1:
//Selector1∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  数值            Data
//}
			p8+=5;
			Len=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			Len+=1+5;
			break;
		case 2:
//Selector2∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  起始值          Data，
//  结束值          Data，
//  数据间隔        Data
//}
			Len=5;
			Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			Len+=1;
			break;
		case 3:
//Selector3∷=SEQUENCE OF Selector2
			i=p8[1];//SEQUENCE OF
			Len=2;
			while(i--)
			{
				Len+=4;
				Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				Len+=Get_DL698DataLen_S(p8+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			}
			Len+=1;
			break;
		case 4:
//Selector4∷=SEQUENCE
//{
//  采集启动时间  date_time_s，
//  电能表集合    MS
//}
			Len=2+7;
			break;
		case 5:
//Selector5∷=SEQUENCE
//{
//  采集存储时间  date_time_s，
//  电能表集合    MS
//}
			Len=2+7;
			break;
		case 6:
//Selector6∷=SEQUENCE
//{
//  采集启动时间起始值  date_time_s，
//  采集启动时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			Len=2+17;
			break;
		case 7:
//Selector7∷=SEQUENCE
//{
//  采集存储时间起始值  date_time_s，
//  采集存储时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			Len=2+17;
			break;
		case 8:
//Selector8∷=SEQUENCE
//{
//  采集成功时间起始值  date_time_s，
//  采集成功时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			Len=2+17;
			break;
		case 9:
//Selector9∷=SEQUENCE
//{
//  上第n次记录  unsigned
//}
			Len=2+1;
			break;
		case 10:
//Selector10∷=SEQUENCE
//{
//上n条记录  unsigned，
//电能表集合  MS
//}
			Len=2+1;
			break;
		default:
			Len=2;
			break;
	}
	if(Len>LENmax_APDU)
	{
		return 0;//APDU最大长度限制
	}
	return (Len<<8)+TypeCount;
}

u32 DataLen_CSD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	91//	见6.3.3.15　
{
//CSD∷=CHOICE
//{
//对象属性描述符       [0] OAD，
//记录型对象属性描述符 [1] ROAD
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
u32 DataLen_MS(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	92//	见6.3.3.16　
{
//MS∷=CHOICE
//{
//  无电能表		[0]，相当于无效配置
//  全部用户地址  	[1]，全部可采集的电能表
//  一组用户类型  	[2] 	SEQUENCE OF unsigned，指定的若干用户类型的那些电能表
//  一组用户地址  	[3] 	SEQUENCE OF TSA，指定的若干电能表通信地址的那些电能表
//  一组配置序号  	[4] 	SEQUENCE OF long-unsigned，指定的若干电能表配置序号的那些电能表
//一组用户类型区间 [5] SEQUENCE OF Region，按数组形式给出用户类型范围
//一组用户地址区间 [6] SEQUENCE OF Region，按数组形式给出电能表通信地址范围
//一组配置序号区间 [7] SEQUENCE OF Region,按数组形式给出电能表档案配置序号范围
//}
//Region∷=SEQUENCE
//{
//   单位  ENUMERATED
//   {
//       前闭后开    （0），
//       前开后闭    （1），
//       前闭后闭    （2），
//       前开后开    （3）
//    }，
//起始值  Data，
//结束值  Data
//}
//Region用于描述数据的区间范围，包括以下四种：（起始值，结束值）、[起始值，结束值）、（起始值，结束值]、[起始值，结束值]
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
		case 2://SEQUENCE OF 前无数据类型的unsigned
//68 39 00 43 05 01 00 00 00 00 00 10 71 5D 07 01 3E 60 14 7F 00 01 01 02 06 11 02 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 20 02 00 5C 02 01 00 16 03 00 E6 53 16 
			i=p8[1];
			i+=3;
			break;
		case 3://SEQUENCE OF 前无数据类型的TSA
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
		case 4://SEQUENCE OF 前无数据类型的long-unsigned
//68 3E 00 43 05 01 00 00 00 00 00 10 97 FD 07 01 00 60 14 7F 00 01 01 02 06 11 04 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 40 02 00 5C 04 03 00 01 00 02 00 03 16 06 00 DF BE 16 
			i=p8[1];
			i*=2;
			i+=3;
			break;
		case 5://SEQUENCE OF Region,有数据类型的unsigned
//68 3D 00 43 05 01 00 00 00 00 00 10 24 03 07 01 01 60 14 7F 00 01 01 02 06 11 05 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 50 02 00 
//5C 05 01 02 11 00 11 01 16 02 00 62 53 16 
			i=p8[1];
			i*=5;
			i+=3;
			break;
		case 6://SEQUENCE OF Region,有数据类型的TSA
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
		case 7://SEQUENCE OF Region,有数据类型的long-unsigned
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




u32 DataLen_SID(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	93//	见6.3.3.22　
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
		return 0;//APDU最大长度限制
	}
	return (i<<8)+TypeCount;
}
u32 DataLen_SID_MAC(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	94//	见6.3.3.23　
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
	i+=4+5;//4byte MAC前有1字节长度
	if(i>LENmax_APDU)
	{
		return 0;//APDU最大长度限制
	}
	return (i<<8)+TypeCount;
}
u32 DataLen_COMDCB(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	95//	见6.3.3.34　
{
	return ((1+5)<<8)+TypeCount;
}
u32 DataLen_RCSD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount)//	96
{
//RCSD∷=SEQUENCE OF CSD
//CSD∷=CHOICE
//{
//对象属性描述符       [0] OAD，
//记录型对象属性描述符 [1] ROAD
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

//保留	97-255	




typedef struct
{
	u32 (*DataLen_NULL)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	0//	空
	u32 (*DataLen_array)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	1//	SEQUENCE OF Data（见6.3.3.1　）数组的元素在对象属性或方法的描述中定义
	u32 (*DataLen_structure)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	2//	SEQUENCE OF Data（见6.3.3.1　）结构的元素在对象属性或方法的描述中定义	
	u32 (*DataLen_bool)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	3//	布尔值（BOOLEAN）	1或0
	u32 (*DataLen_bit_string)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	4//	比特位串（BIT STRING）	
	u32 (*DataLen_double_long)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	5//	32比特位整数（Integer32）	-231…231-1
	u32 (*DataLen_double_long_unsigned)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	6//	32比特位正整数（double-long-unsigned）	0…232-1
	u32 (*DataLen_NULL7)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//7保留
	u32 (*DataLen_NULL8)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//8保留
	u32 (*DataLen_octet_string)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	9//	8比特位位组（字节）串（OCTET STRING）	
	u32 (*DataLen_visible_string)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	10//	ASCII字符串（VisibleString）	
	u32 (*DataLen_NULL11)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//11保留
	u32 (*DataLen_UTF8_string)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	12//	UTF-8编码的字符串	
	u32 (*DataLen_NULL13)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//13保留
	u32 (*DataLen_NULL14)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//14保留
	u32 (*DataLen_integer)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	15//	8比特位整数（integer）	-128…127
	u32 (*DataLen_long)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	16//	16比特位整数（long）	-32768…32767
	u32 (*DataLen_unsigned)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	17//	8比特位正整数（Unsigned8）	0…255
	u32 (*DataLen_long_unsigned)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	18//	16比特位正整数（Unsigned16）	0…65535
	u32 (*DataLen_NULL19)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//19保留
	u32 (*DataLen_long64)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	20//	64比特位整数（Integer64）	-263…263-1
	u32 (*DataLen_long64_unsigned)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	21//	64比特位正整数（Unsigned64）	0…264-1
	u32 (*DataLen_enum)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	22//	枚举的元素在对象属性或方法的描述中定义	0…255
	u32 (*DataLen_float32)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	23//	octet-string（SIZE（4））
	u32 (*DataLen_float64)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	24//	octet-string（SIZE（8））
	u32 (*DataLen_date_time)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	25//	octet-string（SIZE（10））
	u32 (*DataLen_date)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	26//	octet-string（SIZE（5））
	u32 (*DataLen_time)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	27//	octet-string（SIZE（3））
	u32 (*DataLen_date_time_s)(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//	28//octet-string（SIZE（7））
	

//保留	29-79	
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
	//保留	96-255	
	
}DataLen_TypeDef;
__align(4) const DataLen_TypeDef  DataLen_List[]=
{
	DataLen_NULL,//	0//	空
	DataLen_array,//	1//	SEQUENCE OF Data（见6.3.3.1　）数组的元素在对象属性或方法的描述中定义
	DataLen_structure,//	2//	SEQUENCE OF Data（见6.3.3.1　）结构的元素在对象属性或方法的描述中定义	
	DataLen_bool,//	3//	布尔值（BOOLEAN）	1或0
	DataLen_bit_string,//	4//	比特位串（BIT STRING）	
	DataLen_double_long,//	5//	32比特位整数（Integer32）	-231…231-1
	DataLen_double_long_unsigned,//	6//	32比特位正整数（double-long-unsigned）	0…232-1
	DataLen_NULL,//7保留
	DataLen_NULL,//8保留
	DataLen_octet_string,//	9//	8比特位位组（字节）串（OCTET STRING）	
	DataLen_visible_string,//	10//	ASCII字符串（VisibleString）	
	DataLen_NULL,//11保留
	DataLen_UTF8_string,//	12//	UTF-8编码的字符串	
	DataLen_NULL,//13保留
	DataLen_NULL,//14保留
	DataLen_integer,//	15//	8比特位整数（integer）	-128…127
	DataLen_long,//	16//	16比特位整数（long）	-32768…32767
	DataLen_unsigned,//	17//	8比特位正整数（Unsigned8）	0…255
	DataLen_long_unsigned,//	18//	16比特位正整数（Unsigned16）	0…65535
	DataLen_NULL,//19保留
	DataLen_long64,//	20//	64比特位整数（Integer64）	-263…263-1
	DataLen_long64_unsigned,//	21//	64比特位正整数（Unsigned64）	0…264-1
	DataLen_enum,//	22//	枚举的元素在对象属性或方法的描述中定义	0…255
	DataLen_float32,//	23//	octet-string（SIZE（4））
	DataLen_float64,//	24//	octet-string（SIZE（8））
	DataLen_date_time,//	25//	octet-string（SIZE（10））
	DataLen_date,//	26//	octet-string（SIZE（5））
	DataLen_time,//	27//	octet-string（SIZE（3））
	DataLen_date_time_s,//	28//octet-string（SIZE（7））
	
	//保留	29-79	
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
	//保留	97-255	
};

u32 Get_DL698DataLen(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount,u32 LENmax)//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
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
			return 0;//错误
		}
	}
	else
	{
		DataType=p8[0];
	}
	
	if(pDataType)
	{//属性定义指针有效
		if(TypeCount>=pDataType->TypeNum)
		{
			return 0;//错误
		}
		if(DataType!=pDataType->TypeList[TypeCount])
		{
			if(pDataType->TypeList[TypeCount]==DataType_Data)
			{//类型Data
				Len=Get_DL698DataLen(p8,0,0,0);
				Len>>=8;
				Len<<=8;
				Len|=TypeCount+1;
				return Len;
			}
			else
			{//类型值固定
				if((TypeCount==0)&&(DataType==DataType_NULL))
				{//允许第1个数据为0表示空数据
					
				}
				else
				{
					return 0;//错误
				}
			}
		}
	}
	if(DataType>=80)
	{
		DataType-=(80-29);//保留	29-79	
	}
	if(DataType>=sizeof(DataLen_List)/4)
	{
		return 0;//错误
	}
	p32=(u32*)DataLen_List;
	return (*(u32(*)())p32[DataType])((u32)((u32)p8+1),(Attribute_TypeDef *)pDataType,(u32)TypeCount+1);
}

u32 Get_DL698DataLen_S(u8 *p8,u32 LENmax)//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
{
	u32 len;
	len=Get_DL698DataLen(p8,0,0,LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
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
//计算元素地址
//入口:pData=要计算的数据开始地址
//LENmax_Data=要计算的数据串可能的最大长度用于当数据在nand flash时先读入sdram的数据字节数(为快速计算,计算数据总是在ram中进行),最大长度限制128k;当数据本在ram此值无关;当数据在nand flash但LENmax_Data=0时会自动读取128k数据影响运算速度
//Element：要计算的数据地址的主元素计数,每1个数据类型为1个元素,结构和数组不论内有多少个元素计为1个元素,元素从0开始编号,要计算的元素从1开始因第0号元素为开始地址不需计算
//SubElement：若主元素为结构或数组,要计算主元素结构或数组内的元素地址需指定子元素计数,子元素也从0开始编号,要计算的子元素从1开始因第0号子元素为主元素地址不需计算,无子元素时此值必需=0
//注意:当主元素为数组时,子元素计数与第几组数组相关,子元素计数=(第几组*组内总元素个数)+当前组元素计数;
//出口:地址指向元素的类型字节,0=错误
u8 *Get_Element(u8 *pData,u32 Element,u32 SubElement,u32 LENmax_Data)//计算元素地址(虚拟地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
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
//事件缓冲使用约定
//ADDR_DATABUFF事件编辑存放
//ADDR_128KDATABUFF  32KBYTE 保留给DL698_DataDef
//ADDR_128KDATABUFF+32K 32KBYTE 事件参数,关联对象属性表等
//ADDR_128KDATABUFF+64K 32KBYTE 产生事件用临时数据
//ADDR_128KDATABUFF+96K 32KBYTE 事件中各项数据的相应OAD顺序表array OAD
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
		ElementCount=Get_DL698ByteTypeLenValue(pData+1);//得到DL698字节类型长度单元的值
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
		{//找到主元素
			if(SubElement==0)
			{
				return pData;
			}
			i=pData[0];
			if((i==DataType_array)||(i==DataType_structure))
			{
				i=pData[1];
				if(i==0)
				{//数组或结构内元素个数为0
					return 0;
				}
				if(i&0x80)
				{
					if((pData[2]|pData[3])==0)
					{//数组或结构内元素个数为0
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
				{//找到子元素
					return pData;
				}
				i=Get_DL698DataLen_S(pData,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
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
		i=Get_DL698DataLen_S(pData,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
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

u32 Get_ElementADDR(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data)//计算元素地址(实际地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
{
	u8* p8;
	p8=Get_Element((u8*)ADDR_Data,Element,SubElement,LENmax_Data);//计算元素地址(虚拟地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
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


u32 Compare_TSA_MS(u8 *pADDR_6000,u8 *pTSA,u8 *pMS)//比较TSA是否符合MS集合,入口:pADDR_6000已在RAM,pTSA,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
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
		case 0://  无电能表		[0]，
			return 0;
		case 1://  全部用户地址  	[1]，
			return 1;
		case 2://  一组用户类型  	[2] 	SEQUENCE OF 前无数据类型的unsigned，
			for(i=0;i<NUMmax_6000;i++)
			{
				if(pADDR_6000[0]==DataType_structure)
				{//设置有效
					p8=Get_Element(pADDR_6000,2,7,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
					if(p8)
					{
						for(x=0;x<n;x++)
						{
							if(pMS[x]==p8[1])
							{//用户类型相同
								//i=Compare_octet_string(pTSA,pADDR_6000+8);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
								i=Compare_TSA_string(pTSA,pADDR_6000+8);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
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
		case 3://  一组用户地址  	[3] 	SEQUENCE OF 前无数据类型的TSA，
			for(i=0;i<n;i++)
			{
				//x=Compare_octet_string(pTSA,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				x=Compare_TSA_string(pTSA,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				if(x==0)
				{
					return 1;
				}
				x=pMS[0];
				x++;
				pMS+=x;
			}
			return 0;
		case 4://  一组配置序号  	[4] 	SEQUENCE OF 前无数据类型的long-unsigned，
			for(i=0;i<n;i++)
			{
				x=(pMS[0]<<8)+pMS[1];
				if(x<NUMmax_6000)
				{
					p8=pADDR_6000+(x*LENper_6000)+8;
					//x=Compare_octet_string(pTSA,p8);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					x=Compare_TSA_string(pTSA,p8);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(x==0)
					{
						return 1;
					}
				}
				pMS+=2;
			}
			return 0;
		case 5://一组用户类型区间 [5] SEQUENCE OF Region，有数据类型的unsigned
//68 3D 00 43 05 01 00 00 00 00 00 10 24 03 07 01 01 60 14 7F 00 01 01 02 06 11 05 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 50 02 00 
//5C 05 01 02 11 00 11 01 16 02 00 62 53 16 
			for(i=0;i<NUMmax_6000;i++)
			{
				if(pADDR_6000[0]==DataType_structure)
				{//设置有效
					p8=pADDR_6000+8;
					//x=Compare_octet_string(pTSA,p8);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					x=Compare_TSA_string(pTSA,p8);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(x==0)
					{
						p8=Get_Element(pADDR_6000,2,7,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
						if(p8)
						{
							for(x=0;x<n;x++)
							{
								switch(pMS[0])
								{
									case 0://前闭后开    （0）
										if((p8[1]>=pMS[2])&&(p8[1]<pMS[4]))
										{
											return 1;
										}
										break;
									case 1://前开后闭    （1）
										if((p8[1]>pMS[2])&&(p8[1]<=pMS[4]))
										{
											return 1;
										}
										break;
									case 2://前闭后闭    （2）
										if((p8[1]>=pMS[2])&&(p8[1]<=pMS[4]))
										{
											return 1;
										}
										break;
									case 3://前开后开    （3）
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
		case 6://一组用户地址区间 [6] SEQUENCE OF Region，有数据类型的TSA
//68 4B 00 43 05 01 00 00 00 00 00 10 99 A8 07 01 02 60 14 7F 00 01 01 02 06 11 06 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 60 02 00 
//5C 06 01 02 55 07 05 00 00 00 00 00 01 55 07 05 00 00 00 00 00 02 16 06 00 86 21 16 
			for(i=0;i<n;i++)
			{
				x=pMS[0];
				pMS+=2;
				switch(x)
				{
					case 0://前闭后开    （0）
						//x=Compare_octet_string(pTSA,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						x=Compare_TSA_string(pTSA,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(x<2)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pTSA,pMS+x);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(pTSA,pMS+x);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							if(x==2)
							{
								return 1;
							}
						}
						break;
					case 1://前开后闭    （1）
						//x=Compare_octet_string(pTSA,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						x=Compare_TSA_string(pTSA,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(x==1)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pTSA,pMS+x);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(pTSA,pMS+x);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							if(x!=1)
							{
								return 1;
							}
						}
						break;
					case 2://前闭后闭    （2）
						//x=Compare_octet_string(pTSA,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						x=Compare_TSA_string(pTSA,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(x<2)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pTSA,pMS+x);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(pTSA,pMS+x);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							if(x!=1)
							{
								return 1;
							}
						}
						break;
					case 3://前开后开    （3）
						//x=Compare_octet_string(pTSA,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						x=Compare_TSA_string(pTSA,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(x==1)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pTSA,pMS+x);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(pTSA,pMS+x);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
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
		case 7://一组配置序号区间 [7] SEQUENCE OF Region 有数据类型的long-unsigned
//68 46 00 43 05 01 00 00 00 00 00 10 F7 1F 07 01 03 60 14 7F 00 01 01 02 06 11 07 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 70 02 00 
//5C 07 02 00 12 00 01 12 00 02 02 12 00 02 12 00 03 16 04 00 9D 35 16 
			for(x=0;x<NUMmax_6000;x++)
			{
				if(pADDR_6000[0]==DataType_structure)
				{//设置有效
					//i=Compare_octet_string(pTSA,pADDR_6000+8);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					i=Compare_TSA_string(pTSA,pADDR_6000+8);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
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
			//x=配置序号
			for(i=0;i<n;i++)
			{
				switch(pMS[0])
				{
					case 0://前闭后开    （0）
						if((x>=((pMS[2]<<8)+pMS[3]))&&(x<((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 1://前开后闭    （1）
						if((x>((pMS[2]<<8)+pMS[3]))&&(x<=((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 2://前闭后闭    （2）
						if((x>=((pMS[2]<<8)+pMS[3]))&&(x<=((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 3://前开后开    （3）
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

u32 Compare_SETNO_MS(u8 *pADDR_6000,u32 SETNO,u8 *pMS)//比较配置序号是否符合MS集合,入口:pADDR_6000已在RAM,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
{
	u32 i;
	u32 x;
	u32 n;
	u8 *p8;
	
	
 #if RUN_DL698TEST_1==1//0无效,1=运行;全部档案设为内部交采测试数据存储最大深度
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
		case 0://  无电能表		[0]，
			return 0;
		case 1://  全部用户地址  	[1]，
			return 1;
		case 2://  一组用户类型  	[2] 	SEQUENCE OF unsigned，
			p8=Get_Element(pADDR_6000,2,7,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8)
			{
				for(x=0;x<n;x++)
				{
					if(pMS[x]==p8[1])
					{//用户类型相同
						return 1;
					}
				}
			}
			return 0;
		case 3://  一组用户地址  	[3] 	SEQUENCE OF TSA，
			for(i=0;i<n;i++)
			{
				//x=Compare_octet_string(pADDR_6000+8,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				x=Compare_TSA_string(pADDR_6000+8,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				if(x==0)
				{
					return 1;
				}
				x=pMS[0];
				x++;
				pMS+=x;
			}
			return 0;
		case 4://  一组配置序号  	[4] 	SEQUENCE OF long-unsigned，
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
		case 5://一组用户类型区间 [5] SEQUENCE OF Region，
//68 3D 00 43 05 01 00 00 00 00 00 10 24 03 07 01 01 60 14 7F 00 01 01 02 06 11 05 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 50 02 00 
//5C 05 01 02 11 00 11 01 16 02 00 62 53 16 
			p8=Get_Element(pADDR_6000,2,7,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8)
			{
				for(x=0;x<n;x++)
				{
					switch(pMS[0])
					{
						case 0://前闭后开    （0）
							if((p8[1]>=pMS[2])&&(p8[1]<pMS[4]))
							{
								return 1;
							}
							break;
						case 1://前开后闭    （1）
							if((p8[1]>pMS[2])&&(p8[1]<=pMS[4]))
							{
								return 1;
							}
							break;
						case 2://前闭后闭    （2）
							if((p8[1]>=pMS[2])&&(p8[1]<=pMS[4]))
							{
								return 1;
							}
							break;
						case 3://前开后开    （3）
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
		case 6://一组用户地址区间 [6] SEQUENCE OF Region，
//68 4B 00 43 05 01 00 00 00 00 00 10 99 A8 07 01 02 60 14 7F 00 01 01 02 06 11 06 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 60 02 00 
//5C 06 01 02 55 07 05 00 00 00 00 00 01 55 07 05 00 00 00 00 00 02 16 06 00 86 21 16 
			for(i=0;i<n;i++)
			{
				x=pMS[0];
				pMS+=2;
				switch(x)
				{
					case 0://前闭后开    （0）
						//x=Compare_octet_string(pADDR_6000+8,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						x=Compare_TSA_string(pADDR_6000+8,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(x<2)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pADDR_6000+8,pMS+x);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(pADDR_6000+8,pMS+x);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							if(x==2)
							{
								return 1;
							}
						}
						break;
					case 1://前开后闭    （1）
						//x=Compare_octet_string(pADDR_6000+8,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						x=Compare_TSA_string(pADDR_6000+8,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(x==1)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pADDR_6000+8,pMS+x);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(pADDR_6000+8,pMS+x);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							if(x!=1)
							{
								return 1;
							}
						}
						break;
					case 2://前闭后闭    （2）
						//x=Compare_octet_string(pADDR_6000+8,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						x=Compare_TSA_string(pADDR_6000+8,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(x<2)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pADDR_6000+8,pMS+x);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(pADDR_6000+8,pMS+x);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							if(x!=1)
							{
								return 1;
							}
						}
						break;
					case 3://前开后开    （3）
						//x=Compare_octet_string(pADDR_6000+8,pMS);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						x=Compare_TSA_string(pADDR_6000+8,pMS);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(x==1)
						{
							x=pMS[0];
							x++;
							x+=1;
							//x=Compare_octet_string(pADDR_6000+8,pMS+x);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(pADDR_6000+8,pMS+x);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
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
		case 7://一组配置序号区间 [7] SEQUENCE OF Region
//68 46 00 43 05 01 00 00 00 00 00 10 F7 1F 07 01 03 60 14 7F 00 01 01 02 06 11 07 12 01 00 02 02 11 01 11 01 01 01 5B 01 50 04 02 00 01 00 70 02 00 
//5C 07 02 00 12 00 01 12 00 02 02 12 00 02 12 00 03 16 04 00 9D 35 16 
			for(i=0;i<n;i++)
			{
				switch(pMS[0])
				{
					case 0://前闭后开    （0）
						if((SETNO>=((pMS[2]<<8)+pMS[3]))&&(SETNO<((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 1://前开后闭    （1）
						if((SETNO>((pMS[2]<<8)+pMS[3]))&&(SETNO<=((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 2://前闭后闭    （2）
						if((SETNO>=((pMS[2]<<8)+pMS[3]))&&(SETNO<=((pMS[5]<<8)+pMS[6])))
						{
							return 1;
						}
						break;
					case 3://前开后开    （3）
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

void MStoAddrList(u8* pMS)//电能表集合到地址列表;入口:pMS(无数据类型);出口:地址列表在ADDR_AddrList
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
	
	NUM=0;//列表数
	p16d=(u16*)(ADDR_AddrList+2);
	pADDR_6000=(u8*)ADDR_6000_SDRAM;
	i=pMS[0];
	n=pMS[1];
	pMS+=2;
	switch(i)
	{
		case 0://  无电能表		[0]，
			break;
		case 1://  全部用户地址  	[1]
			for(i=0;i<NUMmax_6000;i++)
			{
				if(pADDR_6000[0]==DataType_structure)
				{
					p16s=(u16*)(pADDR_6000+10);//取采集档案配置序号的通信地址
					p16d[0]=p16s[0];
					p16d[1]=p16s[1];
					p16d[2]=p16s[2];
					p16d+=3;
					NUM+=1;
				}
				pADDR_6000+=LENper_6000;
			}
			break;
		case 2://  一组用户类型  	[2] 	SEQUENCE OF unsigned，
			for(x=0;x<n;x++)
			{
				pADDR_6000=(u8*)ADDR_6000_SDRAM;
				for(i=0;i<NUMmax_6000;i++)
				{
					if(pADDR_6000[0]==DataType_structure)
					{
						p8=pADDR_6000+5;
						p8=Get_Element(p8,7,0,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
						if(p8)
						{
							if(pMS[0]==p8[1])
							{//用户类型相同
								if(NUM>=NUMmax_6000)
								{
									break;
								}
								p16s=(u16*)(pADDR_6000+10);//取采集档案配置序号的通信地址
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
		case 3://  一组用户地址  	[3] 	SEQUENCE OF TSA，
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
		case 4://  一组配置序号  	[4] 	SEQUENCE OF long-unsigned，
			for(i=0;i<n;i++)
			{
				x=(pMS[0]<<8)+pMS[1];
				if(x<NUMmax_6000)
				{
					p8=(u8*)pADDR_6000+(x*LENper_6000);
					if(p8[0]==DataType_structure)
					{
						p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
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
		case 5://一组用户类型区间 [5] SEQUENCE OF Region，
			for(x=0;x<n;x++)
			{
				switch(pMS[0])
				{
					case 0://前闭后开    （0）
						pADDR_6000=(u8*)ADDR_6000_SDRAM;
						for(i=0;i<NUMmax_6000;i++)
						{
							if(pADDR_6000[0]==DataType_structure)
							{
								p8=pADDR_6000+5;
								p8=Get_Element(p8,7,0,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(p8)
								{
									if((p8[1]>=pMS[2])&&(p8[1]<pMS[4]))
									{//用户类型相同
										if(NUM>=NUMmax_6000)
										{
											break;
										}
										p16s=(u16*)(pADDR_6000+10);//取采集档案配置序号的通信地址
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
					case 1://前开后闭    （1）
						pADDR_6000=(u8*)ADDR_6000_SDRAM;
						for(i=0;i<NUMmax_6000;i++)
						{
							if(pADDR_6000[0]==DataType_structure)
							{
								p8=pADDR_6000+5;
								p8=Get_Element(p8,7,0,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(p8)
								{
									if((p8[1]>pMS[2])&&(p8[1]<=pMS[4]))
									{//用户类型相同
										if(NUM>=NUMmax_6000)
										{
											break;
										}
										p16s=(u16*)(pADDR_6000+10);//取采集档案配置序号的通信地址
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
					case 2://前闭后闭    （2）
						pADDR_6000=(u8*)ADDR_6000_SDRAM;
						for(i=0;i<NUMmax_6000;i++)
						{
							if(pADDR_6000[0]==DataType_structure)
							{
								p8=pADDR_6000+5;
								p8=Get_Element(p8,7,0,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(p8)
								{
									if((p8[1]>=pMS[2])&&(p8[1]<=pMS[4]))
									{//用户类型相同
										if(NUM>=NUMmax_6000)
										{
											break;
										}
										p16s=(u16*)(pADDR_6000+10);//取采集档案配置序号的通信地址
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
					case 3://前开后开    （3）
						pADDR_6000=(u8*)ADDR_6000_SDRAM;
						for(i=0;i<NUMmax_6000;i++)
						{
							if(pADDR_6000[0]==DataType_structure)
							{
								p8=pADDR_6000+5;
								p8=Get_Element(p8,7,0,0);//计算元素地址,使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(p8)
								{
									if((p8[1]>pMS[2])&&(p8[1]<pMS[4]))
									{//用户类型相同
										if(NUM>=NUMmax_6000)
										{
											break;
										}
										p16s=(u16*)(pADDR_6000+10);//取采集档案配置序号的通信地址
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
		case 6://一组用户地址区间 [6] SEQUENCE OF Region，
			for(i=0;i<n;i++)
			{
				x=pMS[0];
				pMS++;
				d64a=MRR((u32)pMS+3,pMS[2]+1);
				pMS+=pMS[1]+2;
				d64b=MRR((u32)pMS+3,pMS[2]+1);
				pMS+=pMS[1]+2;
				d64a=R_Inverse(d64a,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
				d64b=R_Inverse(d64b,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
				d64a=bcd16_hex(d64a);
				d64b=bcd16_hex(d64b);
				switch(x)
				{
					case 0://前闭后开    （0）
						while(d64a<d64b)
						{
							if(NUM>=NUMmax_6000)
							{
								break;
							}
							d64=hex_bcd16(d64a);
							d64=R_Inverse(d64,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
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
					case 1://前开后闭    （1）
						d64a+=1;
						while(d64a<=d64b)
						{
							if(NUM>=NUMmax_6000)
							{
								break;
							}
							d64=hex_bcd16(d64a);
							d64=R_Inverse(d64,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
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
					case 2://前闭后闭    （2）
						while(d64a<=d64b)
						{
							if(NUM>=NUMmax_6000)
							{
								break;
							}
							d64=hex_bcd16(d64a);
							d64=R_Inverse(d64,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
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
					case 3://前开后开    （3）
						d64a+=1;
						while(d64a<d64b)
						{
							if(NUM>=NUMmax_6000)
							{
								break;
							}
							d64=hex_bcd16(d64a);
							d64=R_Inverse(d64,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
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
		case 7://一组配置序号区间 [7] SEQUENCE OF Region
			for(x=0;x<n;x++)
			{
				switch(pMS[0])
				{
					case 0://前闭后开    （0）
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
									p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
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
					case 1://前开后闭    （1）
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
									p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
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
					case 2://前闭后闭    （2）
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
									p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
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
					case 3://前开后开    （3）
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
									p16s=(u16*)(p8+10);//取采集档案配置序号的通信地址
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

u32 PORTntoOAD(u32 PORTn)//通信口号转为OAD值
{
//#define OAD_RS232     0xF2000201
//#define OAD_GPRS     	0x45000000
//#define OAD_RS4851     0xF2010201
//#define OAD_RS4852     0xF2010202
//#define OAD_RS4854     0xF2090201//载波/微功率无线接口
//#define OAD_RS4853     0xF2010203//
//#define OAD_ENET     	0x45100000
//#define OAD_ACSAMPLE     0xF2080201//交采接口
	switch(PORTn)
	{
//#define RS232PORT     0//RS232通信口号
		case 0:
			return 0xF2000201;
//#define GPRSPORT     1//GPRS通信口号
		case 1:
			return 0x45000201;
//#define RS485_1PORT     2//RS485_1通信口号
		case 2:
			return 0xF2010201;
//#define RS485_2PORT     3//RS485-2通信口号
		case 3:
			return 0xF2010202;
//#define RS485_4PORT     4//RS485_4通信口号(集中器载波,专变控制模块)
		case 4:
			return 0xF2090201;
//#define RS485_3PORT     5//RS485_3通信口号(WI-FI)
		case 5:
			return 0xF2010203;
//#define IRPORT     6//红外通信口号
//#define TESAMPORT     7//终端ESAM通信口号
//#define MESAMPORT     8//电表ESAM通信口号
//#define CARDPORT     9//电表CPUCARD通信口号
//#define ENETPORT     10//以太网通信口号
		case 10:
			return 0x45100201;
//#define SMSPORT     11//短信通信口号
//#define ACSAMPLEPORT     12//交流采样通信口号(虚拟)
		case 12:
			return 0xF2080201;
//#define EFTPCMDPORT     13//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
//#define EFTPDATAPORT     14//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
//#define GFTPCMDPORT     15//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
//#define GFTPDATAPORT     16//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
		default:
			return 0;
	}
}

u32 OADtoPORTn(u32 OAD)//OAD值转为通信口号
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
				//case 0xF2090201://RS485_4通信口号(集中器载波,专变控制模块)
				case 0xF209:
					return 4;
				//case 0x45100201://ENET
				case 0x4510:
					return 10;
				//case 0xF2080201://交流采样通信口号(虚拟)
				case 0xF208:
					return 12;
				default:
					return 0;
			}
	}
}


u32 Compare_DL698DataString(u8 *p1,u8 *p2,u32 Byte)//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
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

//u32 Compare_octet_string(u8 *p1,u8 *p2)//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//{
//	u32 i;
//	u32 x;
//	u32 y;
//	
//	x=p1[0];
//	y=p2[0];
//	if(x>=y)
//	{//串长度p1>=p2
//		if(x==y)
//		{//串长度p1=p2
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
//		{//串长度p1>p2
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
//	{//串长度p1<p2
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

u32 Compare_octet_string(u8 *p1,u8 *p2)//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
{
	u32 i;
	u32 x;
	u32 y;
	
	x=p1[0];
	y=p2[0];
	if(x>=y)
	{//串长度p1>=p2
		if(x==y)
		{//串长度p1=p2
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
		{//串长度p1>p2
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
	{//串长度p1<p2
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

u32 Compare_TSA_string(u8 *p1,u8 *p2)//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
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
	{//串长度p1>=p2
		if(x==y)
		{//串长度p1=p2
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
		{//串长度p1>p2
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
	{//串长度p1<p2
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

	x=Terminal_Router->NoteNum;//已知总从节点数

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
			break;//原已存在
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
#if(USER/100)==17//江苏系
	#if 0
	x=Terminal_Router->NoteNum1;//已知总从节点数

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
			break;//原已存在
		}
	}
	#else
	x=Terminal_Router->NoteNum;//已知总从节点数

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
			break;//原已存在
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
u32 AddrToMeterSetNo(u16* pAddr)//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
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

u32 AddrTowToMeterSetNo(u16* pAddr)//搜索通信地址采集器地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
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
			p8Data=Get_Element(p8,3,0,0);//计算元素地址
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













