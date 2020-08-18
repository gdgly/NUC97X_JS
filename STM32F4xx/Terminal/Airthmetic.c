
#include "Project.h"
#include "../terminal/airthmetic.h"
#include "../Calculate/Calculate.h"

//#if ((Project/100)==3)&&((USER/100)==12)//四川专变
uint32_t YMDCalculateWeek(uint32_t RTCAddr)//指定地址缓冲中的年月日计算周
{
	u8 i;
	u8 * p8;
	
	s16 W;
	u8 C;//世纪-1(年份前两位数)
	u8 y;//年份后两位数
	u8 M;//月份
	u8 d;//日数
	
	p8=(uint8_t *)(RTCAddr);
//	for(i=0;i<3;i++)
//	{
//		p8[i]=bcd_hex(p8[i]);//8位压缩BCD数转为HEX
//	}
//蔡勒公式：
//W=[C/4]-2C+y+[y/4]+[13*(M+1)/5]+d-1
//C:世纪-1(年份前两位数）
//y:年份后两位数
//M:月份
//d:日数
//1月和2月要按上一年的13月和14月来算,这时C和y均按上一年取值。
//算出来的W 除以7，余数是几就是星期几。如果余数是0，则为星期日
	C=20;//世纪-1(年份前两位数）
	y=p8[2];//年份后两位数
	M=p8[1];//月份
	d=p8[0];//日数
	
	if(M<3)
	{
		M+=12;
		y--;
		if(y==99)
		{
			C--;
		}
	}
	W=(C/4)-(2*C)+y+(y/4)+((13*(M+1))/5)+d-1;
	W=((W%7)+7)%7;

	for(i=0;i<3;i++)
	{
		p8[i]=hex_bcd(p8[i]);//HEX转为BCD
	}
	return W;
}

uint8_t ChangeFloatTo376BCD(float data,uint8_t *pdata,uint8_t type1,uint8_t type2) // 1成功 0失败
//type1 相对于376.1里面的格式  1代表A14 2代表A11	3代表A23 4代表A9  5代表A5 6代表A7 7代表A25	
//type2	相当于要乘的倍数       0代表/10 1代表乘以10	2代表*1000   //3代表基准数字还要除以10
{
	uint32_t temp = 0;
	uint8_t i;
	uint8_t flag=0; //flag表示符号位置 0 表示正数 1表示负数
	uint32_t j=0;
//	uint32_t k;//系数
	if(data<0) 
	{
		if((type1 != 4) &&(type1 != 5)&&(type1 != 7))return 0;//负数占时不算	
	}
	switch(type1)
	{
		case 1: temp = (uint32_t)(data*10);
				for(i=0;i<10;i++)
				{
					if(i == 0) j = 1;
					else j=j*10;
					if((i%2) == 0)  pdata[i/2] = (uint8_t)(temp/j%10); //偶数处理
					else 			pdata[i/2] |= (((uint8_t)(temp/j%10))<<4);   //奇数处理
				}
				break;
		case 2:temp =  (uint32_t)(data/10);
				for(i=0;i<8;i++)
				{
					if(i == 0) j = 1;
					else j=j*10;
					if((i%2) == 0)  pdata[i/2] = (uint8_t)(temp/j%10); //偶数处理
					else 			pdata[i/2] |= (((uint8_t)(temp/j%10))<<4);   //奇数处理
				}
				break;
		case 3:	temp = (uint32_t)(data*10);
				for(i=0;i<6;i++)
				{
					if(i == 0) j = 1;
					else j=j*10;
					if((i%2) == 0)  pdata[i/2] = (uint8_t)(temp/j%10); //偶数处理
					else 			pdata[i/2] |= (((uint8_t)(temp/j%10))<<4);   //奇数处理
				}
				break;
		case 4:	if(data<0)	
				{
					data *=(-1); 	// n * ((n>>31<<1） +1）；
					flag =1;
				}
				temp = (uint32_t)(data*10);
				for(i=0;i<6;i++)
				{
					if(i == 0) j = 1;
					else j=j*10;
					if((i%2) == 0)  pdata[i/2] = (uint8_t)(temp/j%10); //偶数处理
					else 			pdata[i/2] |= (((uint8_t)(temp/j%10))<<4);   //奇数处理
				}
				if(flag == 1) pdata[2] |= 0x80;
				else  pdata[2] &= 0x7f;
				break;
		case 5:	if(data<0)	
				{
					data *=(-1); 	// n * ((n>>31<<1） +1）；
					flag =1;
				}
				if(type2 == 1)temp = (uint32_t)(data*10);
				else if(type2 == 2)	temp = (uint32_t)(data*1000);
				for(i=0;i<4;i++)
				{
					if(i == 0) j = 1;
					else j=j*10;
					if((i%2) == 0)  pdata[i/2] = (uint8_t)(temp/j%10); //偶数处理
					else 			pdata[i/2] |= (((uint8_t)(temp/j%10))<<4);   //奇数处理
				}
				if(flag == 1) pdata[1] |= 0x80;
				else  pdata[1] &= 0x7f;
				break;
		case 6:	temp = (uint32_t)(data*10);
				for(i=0;i<4;i++)
				{
					if(i == 0) j = 1;
					else j=j*10;
					if((i%2) == 0)  pdata[i/2] = (uint8_t)(temp/j%10); //偶数处理
					else 			pdata[i/2] |= (((uint8_t)(temp/j%10))<<4);   //奇数处理
				}
				break;
		case 7:	if(data<0)	
				{
					data *=(-1); 	// n * ((n>>31<<1） +1）；
					flag =1;
				}
				temp = (uint32_t)(data*1000);
				for(i=0;i<6;i++)
				{
					if(i == 0) j = 1;
					else j=j*10;
					if((i%2) == 0)  pdata[i/2] = (uint8_t)(temp/j%10); //偶数处理
					else 			pdata[i/2] |= (((uint8_t)(temp/j%10))<<4);   //奇数处理
				}
				if(flag == 1) pdata[2] |= 0x80;
				else  pdata[2] &= 0x7f;
				break;

		default: return 0;
	}
	return 1;
}

uint8_t  ChangeAddress(uint8_t *pdata,uint8_t startposition) 
//1成功 0有问题 startposition为实际的指针偏移量
{
	uint64_t temp=0;
	uint8_t j,k;
	if(startposition >= 249) return 0;
	for(j=7;j>1;j--)
	{
		temp =temp*100;
		k =(uint64_t)(((pdata[startposition+j-2]>>4)&0x0f)*10);
		k+= (uint64_t)(pdata[startposition+j-2]&0x0f);
		temp += k;
	}
	if(temp>0xffffffff)			 return 0;
	pdata[startposition]   = (uint8_t)(temp>>24);
	pdata[startposition+1] = (uint8_t)(temp>>16);
	pdata[startposition+2] = (uint8_t)(temp>>8);
	pdata[startposition+3] = (uint8_t)temp;
	return 1;
}

uint16_t  Crc_HX(uint8_t * buffer, uint32_t len)   //红相表计算校验
{
	uint16_t CRCConst= 0x1021;
	uint16_t crc = 0;
	uint32_t i = 0;
	signed char j = 0;
	for (i = 0; i < len; i++)
	{
	   for(j=7;j>=0;j--)
	   {
	      if ((crc & 0x8000) != 0)    crc = (crc << 1) ^ CRCConst; 
	      else    crc = crc << 1;
	      if((buffer[i] &(1<<j))!=0)  crc= crc^CRCConst;
	   }
	}
	return crc;
}		

uint8_t CheckHXTxData(uint8_t *pdata,uint8_t startposition,uint8_t datalength)
//pdata为传进来的要处理的数   数字开始处理的位置   数据处理前的长度
{
	uint8_t i;
	uint8_t count=0; //计算有几次重合的
	uint8_t result[250];
	for(i=0;i<datalength;i++)
	{
		if(i!=0)//if((i!=0)&&(i!=datalength+count-1))
		{
			if((pdata[startposition+i] == 0x02) || (pdata[startposition+i] == 0x03) || (pdata[startposition+i] == 0x10) 
				|| (pdata[startposition+i] == 0x11) || (pdata[startposition+i] == 0x13) )
			{
				result[i+count] = 0x10;
				result[i+count+1] = pdata[startposition+i]+0x40;
				count++;
			}
			else   result[i+count] = pdata[startposition+i];
		}
		else result[i+count] = pdata[startposition+i];
	}
	for(i=0;i<datalength+count;i++)
	{
		pdata[startposition+i] = result[i];
	}
	return (count+datalength);
}	

uint32_t CheckHXRxData(uint8_t *pdata,uint32_t startposition,uint32_t datalength)
//pdata为传进来的要处理的数   数字开始处理的位置   数据处理前的长度(实际长度，例如：有8个字节数据就写8)
{
   	uint32_t i;
	uint32_t count=0; //计算有几次重合的
	uint8_t result[1000];

	for(i=0;i<datalength-count;i++)
	{
		if((i!=0)&&(i!=datalength-1))
		{
			if(pdata[startposition+i+count] == 0x10) 
			{
				result[i] = pdata[startposition+i+count+1]-0x40;
				count++;
			}
			else   result[i] = pdata[startposition+i+count];
		}
		else result[i] = pdata[startposition+i+count];
	}
	if(count>datalength) return 1001;
	for(i=0;i<datalength-count;i++)
	{
		pdata[startposition+i] = result[i];
	}
	return (datalength-count);
}	

uint8_t _8HexToBCD(uint8_t pdata)  //如果pdata超过100，则只取后面2个数字，百位直接扔掉
{
	uint8_t temp;
	temp  = ((pdata/10%10)<<4);
	temp |=	(pdata%10);
	return temp;
}	

uint8_t _32HexToBCD(uint8_t *pdata,uint8_t *targetdata,uint32_t targetsp)  //如果pdata超过100，则只取后面2个数字，百位直接扔掉
{
	uint8_t i;
	uint32_t tempdata,j;
	tempdata =(((uint32_t)pdata[0])<<24)+(((uint32_t)pdata[1])<<16)+(((uint32_t)pdata[2])<<8)+(uint32_t)pdata[3];
	for(i=0;i<6;i++)
	{
		if(i == 0) j = 1;
		else j=j*10;
		if((i%2) == 0)  targetdata[targetsp+i/2] = (uint8_t)(tempdata/j%10); //偶数处理
		else 			targetdata[targetsp+i/2] |= (((uint8_t)(tempdata/j%10))<<4);   //奇数处理
	}
	return 1;
}			

uint8_t ChangeHxAdd(uint8_t *temp,uint8_t *pdata,uint8_t startposition) //0表示计算有问题，1表示成功
												  //temp表示临时用来存转化的数据，pdata表示原始数据
												  //同时这里默认用的是给红相表发数据的原始地址块

{
	uint8_t i;	   	      //临时用一下
	uint8_t okflag=0;	  //表示成功取得了一个数据
	uint32_t x;			  //临时用一下的
	uint64_t z,y=1;
	for(i=0;i<8;i++)
	{
		temp[i] = 0;
	}
	for(i=0;i<8;i++)
	{
		if(pdata[startposition+i] != 0x10) 
		{
			temp[okflag] = pdata[startposition+i];
			okflag++;
		}
		else 
		{
			if(i>=7) 
			{
				for(i=0;i<8;i++) { temp[i] = 0;}
				return 0;
			}
			else if(pdata[startposition+i+1]<=0x40) 
			{
				for(i=0;i<8;i++) { temp[i] = 0;}
				return 0;
			}
			else
			{
				temp[okflag] = pdata[startposition+i+1]-0x40;
				i++;
				okflag++;	
			}
		}
		if(okflag == 4) i = 8;
	}
	if(okflag == 4)
	{
		x  =  (uint32_t)temp[0]<<24;
		x +=  (uint32_t)temp[1]<<16;
		x +=  (uint32_t)temp[2]<<8;
		x +=  (uint32_t)temp[3];
		z = (uint64_t)x;
		for(i=0;i<6;i++)
		{
			if(i == 0) y = 1;
			else y = y*100;
			temp[i]  =  z/y%10;
			temp[i]	+=  (z/y/10%10)<<4;
		}
		return 1;
	}
	return 0;//纯防止报警的代码
}
					   
//#endif
