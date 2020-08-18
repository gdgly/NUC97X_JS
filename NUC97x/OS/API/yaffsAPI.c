
#if MainProtocol==376
#include "../../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../../NUc97x/OS/API/yaffsAPI.h"





u32 yaffs_READ(int fd, void *buf, u32 byte)
{
	int yaffs;
	yaffs=yaffs_read(fd,buf,byte);
	if(yaffs<0)
	{
		return 0;
	}
	return byte;
	
//	u32 i;
//	u32 x;
//	unsigned int nbyte;
//	int yaffs;
//	
//	i=0;
//	while(i<byte)
//	{
//		x=byte-i;
//		if(x>(32*1024))
//		{
//			nbyte=(32*1024);
//		}
//		else
//		{
//			nbyte=x;
//		}
//		yaffs=yaffs_read(fd,(void *)((u32)buf+i),nbyte);
//		if(yaffs<0)
//		{
////			sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
//			return i;
//		}
//		i+=nbyte;
//	}
////	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
//	return i;
}

u32 yaffs_WRITE(int fd, const void *buf,u32 byte)
{
	int yaffs;
	yaffs=yaffs_write(fd,buf,byte);
	if(yaffs<0)
	{
		return 0;
	}
	return byte;
//	u32 i;
//	u32 x;
//	unsigned int nbyte;
//	int yaffs;
//	
////	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
//	i=0;
//	while(i<byte)
//	{
//		x=byte-i;
//		if(x>(32*1024))
//		{
//			nbyte=(32*1024);
//		}
//		else
//		{
//			nbyte=x;
//		}
//		yaffs=yaffs_write(fd,(void *)((u32)buf+i),nbyte);
//		if(yaffs<0)
//		{
//			return i;
//		}
//		i+=nbyte;
//	}
//	return i;
}


char *CopyStr(char *s1,char *s2)
{
	while(*s2)
	{
		*s1=*s2;
		s1++;
		s2++;
	}
	return s1;
}

int yaffs_unlinkdir(const YCHAR *path)//删除目录下的全部文件
{
	u32 i;
	yaffs_DIR* pdir;
	struct yaffs_dirent *de;
	char pFileName[256];
	char* p8;
	
	pdir=yaffs_opendir(path);
	if(pdir)
	{
		for (i = 0;(de = yaffs_readdir(pdir)) != NULL; i++)
		{//删除目录下全部文件
			p8=pFileName;
			p8=CopyStr(p8,(char*)path);
			p8=CopyStr(p8,(char*)&de->d_name);
			p8[0]=0;
			yaffs_unlink(pFileName);//删除文件
		}
		yaffs_closedir(pdir);
		return 0;
	}
	return -1;
}












