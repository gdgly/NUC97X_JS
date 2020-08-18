
//FTP文件下载显示
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Display/DisplayFTP.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"





u32 DisplayFTPStatus(u32 Row,u8 *pFileName,u32 Status)
{
//	u32 i;
	u32 x;
	u8 * p8;
//	u32 * p32;

	switch(Status)
	{//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=...
		case 1:
		case 3:
			DisplayString(Row,0,0,pFileName);//显示字符串
			DisplayString(Row,8,0,(u8 *)"正在下载...");//显示字符串
			break;
		case 2:
		case 4:
			DisplayString(Row,0,0,pFileName);//显示字符串
			p8=(u8 *)(ADDR_DATABUFF);
			x=0;
			p8[x]='O';
			x++;
			p8[x]='K';
			x++;
			p8[x]=0;
			DisplayString(Row,18,0,(u8 *)ADDR_DATABUFF);//显示字符串
			break;
		case 5:
			DisplayString(Row,0,0,pFileName);//显示字符串
			DisplayString(Row,8,0,(u8 *)"文件错误!");//显示字符串
			break;
		default:
			return Row;
	}
	Row++;
	return Row;
}


void DisplayFTP(void)//FTP文件下载显示
{
	u32 Row;
	u8 * p8;
	
  if(Comm_Ram->DisplayScanTimer!=0x0)
  {
    return;
  }
	DisplayClr();//清屏
	DisplayString(0,4,0,(u8 *)"FTP文件下载");//显示字符串
	
	MR(ADDR_DATABUFF+1024,ADDR_FTPFileFlags+5,16);
	p8=(u8 *)(ADDR_DATABUFF+1024);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
	Row=1;
	
//	Row=DisplayFTPStatus(Row,(u8 *)"MS",p8[0]);//用I0版本号
//	Row=DisplayFTPStatus(Row,(u8 *)"OS",p8[1]);	
//	Row=DisplayFTPStatus(Row,(u8 *)"Meter",p8[2]);	
//	Row=DisplayFTPStatus(Row,(u8 *)"Terminal",p8[3]);	
//	Row=DisplayFTPStatus(Row,(u8 *)"Menu",p8[4]);	
//	Row=DisplayFTPStatus(Row,(u8 *)"Font",p8[5]);	
//	Row=DisplayFTPStatus(Row,(u8 *)"PACK",p8[6]);
	Row=DisplayFTPStatus(Row,(u8 *)"PACK",p8[0]);
	if((p8[0]!=2)&&(p8[0]!=4))
	{
		Row=DisplayFTPStatus(Row,(u8 *)"PACK",p8[1]);
		if((p8[1]!=2)&&(p8[1]!=4))
		{
			Row=DisplayFTPStatus(Row,(u8 *)"PACK",p8[2]);
			{
				if((p8[2]!=2)&&(p8[2]!=4))
				{
					Row=DisplayFTPStatus(Row,(u8 *)"PACK",p8[3]);
					if((p8[3]!=2)&&(p8[3]!=4))
					{
						Row=DisplayFTPStatus(Row,(u8 *)"PACK",p8[4]);
						if((p8[4]!=2)&&(p8[4]!=4))
						{
							Row=DisplayFTPStatus(Row,(u8 *)"PACK",p8[5]);
							if((p8[5]!=2)&&(p8[5]!=4))
							{
								Row=DisplayFTPStatus(Row,(u8 *)"PACK",p8[6]);
							}
						}
					}
				}
			}
		}
	}


#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=			
	switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
	{
		case 1:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_50",p8[7]);
			break;
		case 2:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_TC",p8[7]);
			break;
		case 3:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_ES",p8[7]);
			break;
		case 4:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_CX",p8[7]);
			break;
		case 5:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_01",p8[7]);
			break;
		case 6:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_LH",p8[7]);
			break;
		case 7:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_NX",p8[7]);
			break;
		case 8:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_QL",p8[7]);
			break;
		case 101:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_MX",p8[7]);
			break;
		case 102:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_FC",p8[7]);
			break;
		case 103:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_NW",p8[7]);
			break;
		case 104:
			Row=DisplayFTPStatus(Row,(u8 *)"PLC_SR",p8[7]);
			break;
	}
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=	
	Row=DisplayFTPStatus(Row,(u8 *)"Module",p8[7]);
#endif	
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=	
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
		switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康
		{
			case 1:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_50",p8[7]);
				break;
			case 2:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_TC",p8[7]);
				break;
			case 3:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_ES",p8[7]);
				break;
			case 4:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_CX",p8[7]);
				break;
			case 5:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_01",p8[7]);
				break;
			case 6:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_LH",p8[7]);
				break;
			case 7:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_NX",p8[7]);
				break;
			case 8:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_QL",p8[7]);
				break;
			case 101:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_MX",p8[7]);
				break;
			case 102:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_FC",p8[7]);
				break;
			case 103:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_NW",p8[7]);
				break;
			case 104:
				Row=DisplayFTPStatus(Row,(u8 *)"PLC_SR",p8[7]);
				break;
		}
	}
	else
	{
		Row=DisplayFTPStatus(Row,(u8 *)"Module",p8[7]);
	}
#endif
	
	if(MRR(ADDR_FTPFileFlags+4,1)>=MaxFileNum)
	{
		DisplayString(Row,8,0,(u8 *)"结束");//显示字符串
	}

}

