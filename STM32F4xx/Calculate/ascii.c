
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Calculate/ascii.h"



char *StrCopy(char *s1,char *s2)
{
	while(*s2)
	{
		*s1=*s2;
		s1++;
		s2++;
	}
	return s1;
}

char *Byte_ascii(u32 data,char *p8str)
{
	u32 x;
	x=data>>4;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	p8str[0]=x;
	x=data&0xf;
	if(x<10)
	{
		x+=0x30;
	}
	else
	{
		x+=55;
	}
	p8str[1]=x;
	p8str+=2;
	return p8str;
}



