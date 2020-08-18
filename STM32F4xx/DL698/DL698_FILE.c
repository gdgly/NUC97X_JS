

//文件传输
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_FILE.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/ascii.h"
#include "../OS/OS.h"
#include "../MS/MS.h"
#include "../DL698/DL698_JSON.h"


//文件分块传输管理

u32 GET_Request_F001(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LEN_BLOCK;
	u32 LenRx;
	u8* p8s;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	switch((OAD>>8)&0xff)
	{
		case 4://文件传输块状态字
			p8s=Get_Element((u8 *)ADDR_F001_7,2,0,LENmax_F001_7);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8s==0)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			LEN_BLOCK=(p8s[1]<<8)|p8s[2];//参数块长度
			if(LEN_BLOCK==0)
			{
				UARTCtrl->DAR=255;//数据访问结果;成功
				return 0;
			}
			p8s=Get_Element((u8 *)ADDR_F001_7+2,3,0,LENmax_F001_7);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8s)
			{
				i=(p8s[1]<<24)|(p8s[2]<<16)|(p8s[3]<<8)|p8s[4];//文件大小  double-long-unsigned
			}
			else
			{
				i=0;
			}
			if(i==0)
			{
				UARTCtrl->DAR=255;//数据访问结果;成功
				return 0;
			}
			i+=LEN_BLOCK-1;
			i/=LEN_BLOCK;//包数（bit-string 数）
			LenRx=i;
			LenRx+=7;
			LenRx/=8;//整字节数
			if(LenRx>LENmax_F001_4)
			{
				UARTCtrl->DAR=255;//数据访问结果;成功
				return 0;
			}
			if(i<=127)
			{
				if(LENmax_TxSPACE<(LenRx+2))
				{
					UARTCtrl->DAR=255;//数据访问结果;成功
					return 0;
				}
				p8tx[0]=DataType_bit_string;
				p8tx[1]=i;
				MR((u32)p8tx+2,ADDR_F001_4,LenRx);
				LenRx+=2;
			}
			else
			{
				if(LENmax_TxSPACE<(LenRx+4))
				{
					UARTCtrl->DAR=255;//数据访问结果;成功
					return 0;
				}
				p8tx[0]=DataType_bit_string;
				p8tx[1]=0x82;
				p8tx[2]=i>>8;
				p8tx[3]=i;
				MR((u32)p8tx+4,ADDR_F001_4,LenRx);
				LenRx+=4;
			}
			return LenRx;
		default:
			UARTCtrl->DAR=255;//数据访问结果;成功
			return 0;
	}
}

//报文传输
u32 ACTION_F001(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 LenRx;
	u32 LenTx;
	u32 LEN_BLOCK;
	u32 NetFlag;
	u8* p8s;
	u8* p8s1;
#ifndef  LINUX
	u8* p8d;
#endif
	u32* p32;
	UARTCtrl_TypeDef *UARTCtrl;
#ifndef   LINUX
	os_Type *os;
#endif
	ms_Type* ms;
#ifdef IC_STM32F4xx
	FILE *file;
#endif
#ifdef IC_NUC9xx
#ifndef    LINUX
	int yaffs;
	char *pFileName;
#endif
#endif
#ifdef LINUX
	s32 fd;
#endif
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	switch(i)
	{
		case 1://1．复位
		case 2://2．执行
		case 3://3．删除
		case 4://4．校验
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 5://5．代发
			p8s=Get_Element((u8 *)ADDR_F001_7+2,3,0,LENmax_F001_7);
			if(!p8s[0])
			{
				return 0; // 升级文件不存在，直接返回确认帧
			}
			p8s = (u8*)ADDR_DATABUFF;
			MR(ADDR_DATABUFF,ADDR_F001_7,LENmax_F001_7);
			if((p8rx[5] == 0xF2) && (p8rx[6] == 0x09) && (p8rx[7] == 0x02) && (p8rx[8] == 0x01))
			{
				if((p8rx[10] == 0xF2) && (p8rx[11] == 0x10) && (p8rx[12] == 0x02) && (p8rx[13] == 0x00))
				{
					p8s[34] = 4;	// 将升级类型换成STA
				}
				else
				{
					p8s[34] = 1;	// 将升级类型换成CCO
				}	
			}
			MW(ADDR_DATABUFF,ADDR_F001_7,LENmax_F001_7);
			Terminal_Router->UpDateTask = 16;	// 执行升级流程
			return 0;
		case 6://6. 代收
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 7://方法7：启动传输（参数）,上传
			LenRx=0;
			LenTx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(i>LENmax_F001_7)
			{
				UARTCtrl->DAR=8;//越界(8)
				return 0;
			}
			LenRx+=i;
			MW((u32)p8rx,ADDR_F001_7,i);
			MC(0,ADDR_F001_4,LENmax_F001_4);//清传输块状态字
			MC(0xff,ADDR_Download_ENTRY,512);//清文件暂存的文件头信息			
			return (LenRx<<16)+LenTx;
		case 8://方法8：写文件（参数）,下载
//参数∷=structure
//{
//   块序号  long-unsigned，
//   块数据  octet-string
//}
//目标文件不存在则创建，并清零传输状态字，块序号从0开始。
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=(p8rx[3]<<8)|p8rx[4];//块序号
			if(i>=(LENmax_F001_4*8))
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			p8rx+=5;
			LenRx=p8rx[1];//本帧块长度
			if((LenRx&0x80)==0)
			{
				p8rx+=2;
			}
			else
			{
				switch(LenRx)
				{
					case 0x81:
						LenRx=p8rx[2];//本帧块长度
						p8rx+=3;
						break;
					case 0x82:
						LenRx=(p8rx[2]<<8)|p8rx[3];//本帧块长度
						p8rx+=4;
						break;
					default:
						UARTCtrl->DAR=255;
						return 0;
				}
			}
			p8s=Get_Element((u8 *)ADDR_F001_7,2,0,LENmax_F001_7);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8s==0)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			LEN_BLOCK=(p8s[1]<<8)|p8s[2];//参数块长度
			if(LenRx>LEN_BLOCK)
			{//块长度>设定
				UARTCtrl->DAR=255;
				return 0;
			}
			if((LEN_BLOCK*i)>=LENmax_Download)
			{//>nand空间
				UARTCtrl->DAR=255;
				return 0;
			}
			MW((u32)p8rx,ADDR_Download_ENTRY+(LEN_BLOCK*i),LEN_BLOCK);
			//文件传输块状态字
	#if NUMmax_F001_4>LEN_DATABUFF
			#error
	#endif
			MR(ADDR_DATABUFF,ADDR_F001_4,LENmax_F001_4);
			p8s=(u8*)ADDR_DATABUFF;
			p8s+=i/8;
			i%=8;
			i=0x80>>i;
			p8s[0]|=i;
			MW(ADDR_DATABUFF,ADDR_F001_4,LENmax_F001_4);
			//计算已接收块数
			p32=(u32*)(ADDR_DATABUFF);
			LenRx=0;
			for(i=0;i<(LENmax_F001_4/4);i++)
			{
				LenTx=p32[i];
				if(LenTx==0xffffffff)
				{
					LenRx+=32;//接收块
				}
				else
				{
					LenTx=BitNum(LenTx);//计算置位为1的个数(0-32)
					LenRx+=LenTx;
					break;
				}
			}
			LenRx*=LEN_BLOCK;//已接收文件长度
			p8s=Get_Element((u8 *)ADDR_F001_7+2,3,0,LENmax_F001_7);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8s)
			{
				x=(p8s[1]<<24)|(p8s[2]<<16)|(p8s[3]<<8)|p8s[4];//文件大小  double-long-unsigned
				p8s1=Get_Element((u8 *)ADDR_F001_7+2,6,0,LENmax_F001_7);
				if(p8s1)
				{
					// CCO 或者 STA
					if((p8s1[1] == 1) ||(p8s1[1] == 4))
					{
						NetFlag = 1;
					}
					else if(p8s1[1] == 255)
					{
						NetFlag = 2;
					}
					else
					{
						NetFlag = 0;
					}
				}
				else
				{
						NetFlag = 0;
				}
#ifdef LINUX
				if((LenRx>=x)&&(LenRx>1024))
				{//全部接收
					//os=Get_os();
					ms=Get_ms();

					fd=ms->link_open("/usr/app/pack.tar.gz",O_CREAT|O_RDWR);
					if(fd==-1)
					{
						Crl_AllJsonFileAndTar();//删除JSON所有的文件内容
						fd=ms->link_open("/usr/app/pack.tar.gz",O_CREAT|O_RDWR);
						if(fd==-1)
						{//远程升级失败
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:远程升级失败\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							UARTCtrl->DAR=255;
							return 0;
						}
					}
					p8s=ms->ms_malloc((size_t)x);//动态分配我的内存堆
					if(p8s==0)
					{
						ms->Init_my_heap();
						p8s=ms->ms_malloc((size_t)x);//动态分配我的内存堆
						if(p8s==0)
						{
							#ifdef DEBUG_JSON
							myprintf("[%s:%d]:内存堆空间申请失败\n",(u32)&__func__,(u32)__LINE__,0);
							#endif
							ms->link_close(fd);//关闭文件句柄
							UARTCtrl->DAR=255;
							return 0;
						}
					}
					MR((u32)p8s,ADDR_Download_ENTRY,x);
					ms->link_lseek(fd,0,SEEK_SET);
					i=ms->link_write(fd,(u8 *)p8s,x);
					if(i==x)
					{//写入文件成功
						FileDownload->New=0;//有新文件载入标志:0=有,1=没
						FileDownload->PORTn=PORTn;//完成下载的端口号
					}
					ms->link_close(fd);//关闭文件句柄
					ms->ms_free(p8s);//释放掉申请的空间
				}
#else
				if((LenRx>=x)&&(LenRx>1024))
				{//全部接收
					os=Get_os();
					ms=Get_ms();
				#ifdef IC_STM32F4xx
					i=os->ffree("N0:");
					if(i<x)
					{//空间不够
						if(NetFlag > 1)
						{
							MC(0,ADDR_F001_7,LENmax_F001_7);
						}
						else
						{
							//删除TEST文件夹
							os->fdelete("N0:/TEST/",NULL);//删除文件
							i=os->ffree("N0:");
							if(i<LenTx)
							{//空间还不够
								//删除Program Files文件夹
								os->fdelete("N0:/Program Files/",NULL);//删除文件
							}
						}
					}
				#endif
				#ifdef IC_NUC9xx
					i=os->yaffs_freespace("user/Update");
					if(i<x)
					{//空间不够
						//删除TEST文件夹
						os->yaffs_unlinkdir("user/TEST/");
						i=os->yaffs_freespace("user/Update");
						if(i<LenTx)
						{//空间还不够
							//删除Program Files文件夹
							os->yaffs_unlinkdir("user/Program Files/");
						}
					}
				#endif
					p8s=ms->ms_malloc((size_t)x);//动态分配我的内存堆
					p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//动态分配我的内存堆
					if((p8s==0)||(p8d==0))
					{
						ms->Init_my_heap();
						p8s=ms->ms_malloc((size_t)x);//动态分配我的内存堆
						p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//动态分配我的内存堆
					}
					MR((u32)p8s,ADDR_Download_ENTRY,x);
					if(NetFlag > 0)
						i=x;
					else
						i=p8s[5]+(p8s[6]<<8)+(p8s[7]<<16)+(p8s[8]<<24);//原始文件长度
					if(i>(2*LENmax_Download))
					{
						ms->ms_free(p8s);//释放我的内存堆分配
						ms->ms_free(p8d);//释放我的内存堆分配
						UARTCtrl->DAR=255;
						return 0;
					}
					// HPLC CCO升级 STA升级 bin文件不需要tar
					if(NetFlag > 0)
					{
					#ifdef IC_STM32F4xx
						file=os->fopen("N0:/Download/CCOorSTA.bin","w");
					#endif
					}
					else
					{
					LenRx=os->RAM_LzmaDecode((u32)p8s,x,(u32)p8d,i);//RAM->RAM的Lzma解压缩算法;返回:解压缩后的长度
					if(LenRx!=i)
					{
						ms->ms_free(p8s);//释放我的内存堆分配
						ms->ms_free(p8d);//释放我的内存堆分配
						UARTCtrl->DAR=255;
						return 0;
					}
					p32=(u32*)p8d;
				#ifdef IC_STM32F4xx
					switch(p32[128])//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
					{
						case 0:
							file=os->fopen("N0:/Download/MS698.zip","w");
							break;
						case 1:
							file=os->fopen("N0:/Download/OS698.zip","w");
							break;
						case 2:
							file=os->fopen("N0:/Download/Meter698.zip","w");
							break;
						case 3:
							file=os->fopen("N0:/Download/Terminal698.zip","w");
							break;
						case 4:
							file=os->fopen("N0:/Download/Menu698.zip","w");
							break;
						case 5:
							file=os->fopen("N0:/Download/Font.zip","w");
							break;
						case 6:
							file=os->fopen("N0:/Download/PACK698.zip","w");
							break;
						case 7:
							file=os->fopen("N0:/Download/Module698.zip","w");
							break;
						default:
							ms->ms_free(p8s);//释放我的内存堆分配
							ms->ms_free(p8d);//释放我的内存堆分配
							UARTCtrl->DAR=255;
							return 0;
						}
					}
					if(file==NULL)
					{
						ms->ms_free(p8s);//释放我的内存堆分配
						ms->ms_free(p8d);//释放我的内存堆分配
						UARTCtrl->DAR=255;
						return 0;
					}
					os->fwrite(p8s,1,x,file);
					os->fclose(file);
				#endif
				#ifdef IC_NUC9xx
					pFileName=(char*)ADDR_DATABUFF;
					pFileName=StrCopy(pFileName,"user/Update/");
					switch(p32[128])//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
					{
						case 0:
							pFileName=StrCopy(pFileName,FileFactory_MS);
							break;
						case 1:
							pFileName=StrCopy(pFileName,FileFactory_OS);
							break;
						case 2:
							pFileName=StrCopy(pFileName,FileFactory_Meter);
							break;
						case 3:
							pFileName=StrCopy(pFileName,FileFactory_Terminal);
							break;
						case 4:
							pFileName=StrCopy(pFileName,FileFactory_Menu);
							break;
						case 5:
							pFileName=StrCopy(pFileName,FileFactory_Font);
							break;
						case 6:
							pFileName=StrCopy(pFileName,FileFactory_PACK);
							break;
						case 7:
							pFileName=StrCopy(pFileName,FileFactory_Modele);
							break;
						default:
							ms->ms_free(p8s);//释放我的内存堆分配
							ms->ms_free(p8d);//释放我的内存堆分配
							UARTCtrl->DAR=255;
							return 0;
					}
					pFileName[0]=0;
					yaffs=os->yaffs_open((char*)ADDR_DATABUFF,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
					if(yaffs<0)
					{
						ms->ms_free(p8s);//释放我的内存堆分配
						ms->ms_free(p8d);//释放我的内存堆分配
						UARTCtrl->DAR=255;
						return 0;
					}
					os->yaffs_WRITE(yaffs,p8s,x);
					//os->yaffs_fsync(yaffs);
					os->yaffs_close(yaffs);
					//写入后打开读一次
//					yaffs=os->yaffs_open((char*)ADDR_DATABUFF,O_RDWR,0);
//					os->yaffs_close(yaffs);
					//延时等待最后字节发送
					Comm_Ram->WaitUartCompleteMSTimer=5000/10;//等待UART帧返回定时器 每10ms减1定时器
				#endif
					ms->ms_free(p8s);//释放我的内存堆分配
					ms->ms_free(p8d);//释放我的内存堆分配
					if(NetFlag > 0)
					{
						if(NetFlag == 2)
						{
							Terminal_Router->UpDateTask = 0;
						}
						else
						{
							Terminal_Router->UpDateTask = 16;
						}
						FileDownload->New=1;//有新文件载入标志:0=有,1=没
					}
					else
					{
						FileDownload->New=0;//有新文件载入标志:0=有,1=没
					}
					FileDownload->PORTn=PORTn;//完成下载的端口号
				}

			}
#endif
		}
		return 0;
		case 9://方法9：读文件（参数）
/*
//参数∷=structure
//{
//   块序号  long-unsigned
//}
//应答∷=structure
//{
//   块数据  octet-string
//}
//源文件不存在则返回错误，目标文件不存在则创建，并清零传输状态字。
			p8rx+=4;
			i=(p8rx[3]<<8)|p8rx[4];//块序号
			if(i>=(LENmax_F001_4*8))
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			MR(ADDR_DATABUFF,ADDR_F001_4,LENmax_F001_4);
			p8s=(u8*)ADDR_DATABUFF;
			p8s+=i/8;
			x=i%8;
			x=0x80>>x;
			if((p8s[0]&x)==0)
			{//块没下载
				UARTCtrl->DAR=255;
				return 0;
			}
			p8s=Get_Element((u8 *)ADDR_F001_7,2,0,LENmax_F001_7);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8s==0)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			LEN_BLOCK=(p8s[1]<<8)|p8s[2];//参数块长度
			if(LEN_BLOCK==0)
			{
				LEN_BLOCK=1024;
			}
			p8s=Get_Element((u8 *)ADDR_F001_7+2,3,0,LENmax_F001_7);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			x=0;
			if(p8s)
			{
				x=(p8s[1]<<24)|(p8s[2]<<16)|(p8s[3]<<8)|p8s[4];//文件大小  double-long-unsigned
			}
			i*=LEN_BLOCK;//地址偏移
			if(i>=x)
			{//超出
				UARTCtrl->DAR=255;
				return 0;
			}
			LenTx=x-i;
			if(LenTx>LEN_BLOCK)
			{
				LenTx=LEN_BLOCK;
			}
			if((LenTx+6)>LENmax_TxSPACE)
			{
				UARTCtrl->DAR=255;
				return 0;
			}
			p8tx[0]=DataType_structure;
			p8tx[1]=1;
			p8tx[2]=DataType_octet_string;
			p8tx[3]=0x82;
			p8tx[4]=LenTx>>8;
			p8tx[5]=LenTx;
			MR((u32)p8tx+6,ADDR_Download_ENTRY+i,LenTx);
			return LenTx+6;
*/
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 10://方法10：软件比对（参数）
//参数∷=structure
//{
//CPU编号        unsigned，
//密钥索引        unsigned，
//因子起始地址    double-long-unsigned，
//数据起始地址    double-long-unsigned，
//待加密数据长度  long-unsigned
//}
//应答∷=structure
//{
//比对块数据  octet-string
//}

			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		default:
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
	}
}


//FTP传输管理
u32 ACTION_F002(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=p8rx[2];
	switch(i)
	{
		case 1://1．复位
		case 2://2．执行
		case 3://3．删除
		case 4://4．校验
		case 5://5．代发
		case 6://6. 代收
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 7://方法7：从服务器下载（文件信息，协议类型）
			LenRx=0;
			LenTx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(i>LENmax_F002_7)
			{
				UARTCtrl->DAR=8;//越界(8)
				return 0;
			}
			LenRx+=i;
			MW((u32)p8rx,ADDR_F002_7,i);
			MC(0xff,ADDR_Download_ENTRY,512);//清文件暂存的文件头信息
			//FTP文件下载标志
			MC(0,ADDR_FTPFileFlags+4,1+16);
			//连接任务
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
			UARTCtrl->LinkTask2=0;//通道2链接任务(FTP命令)
			UARTCtrl->FTPLinkCount=0;//FTP尝试连接计数
			UARTCtrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			UARTCtrl->LinkTask2=0;//通道2链接任务(FTP命令)
			UARTCtrl->FTPLinkCount=0;//FTP尝试连接计数
			UARTCtrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
			return (LenRx<<16)+LenTx;
		case 8://方法8：上传到服务器（文件信息，协议类型）
			UARTCtrl->DAR=255;
			return 0;
		default:
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
	}
}




