
//USB�ļ�������ʾ
#include "Project.h"
#include "../Display/DisplayUSB.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"



u32 DisplayUSBStatus(u32 Row,u8 *pFileName,u32 Status,u32 ADDR_ENTRY)
{
	u32 i;
	u32 x;
	u8 * p8;
	u32 * p32;

	switch(Status)
	{//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=...
		case 1:
		case 3:
			DisplayString(Row,0,0,pFileName);//��ʾ�ַ���
			DisplayString(Row,8,0,(u8 *)"��������...");//��ʾ�ַ���
			break;
		case 2:
		case 4:
			DisplayString(Row,0,0,pFileName);//��ʾ�ַ���
			p8=(u8 *)(ADDR_DATABUFF);
			p32=(u32 *)(ADDR_DATABUFF);
			MR(ADDR_DATABUFF,ADDR_ENTRY+(4*68),4);//ȡ�汾��Ϣ��ʼ��ַ
#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��,700=			
			if(ADDR_ENTRY==ADDR_Download_ENTRY)
			{
				MR(ADDR_DATABUFF,ADDR_Download_ENTRY+p32[0]-0x2000+16,4);
			}
			else
			{
				MR(ADDR_DATABUFF,p32[0]+16,4);
			}
#else
			MR(ADDR_DATABUFF,p32[0]+16,4);
#endif
			i=p32[0];
			x=0;
			p8[x]='V';
			x++;
			if((i>>12)!=0)
			{
				p8[x]=(i>>12)+0x30;
				x++;
			}
			p8[x]=((i>>8)&0xf)+0x30;
			x++;
			p8[x]='.';
			x++;
			p8[x]=((i>>4)&0xf)+0x30;
			x++;
			p8[x]=((i>>0)&0xf)+0x30;
			x++;
			p8[x]=' ';
			x++;
			p8[x]=' ';
			x++;
			p8[x]='O';
			x++;
			p8[x]='K';
			x++;
			p8[x]=0;
			DisplayString(Row,8,0,(u8 *)ADDR_DATABUFF);//��ʾ�ַ���
			break;
		case 5:
			DisplayString(Row,0,0,pFileName);//��ʾ�ַ���
			DisplayString(Row,8,0,(u8 *)"�ļ�����!");//��ʾ�ַ���
			break;
		default:
			return Row;
	}
	Row++;
	return Row;
}


void DisplayUSB(void)//USB�ļ�������ʾ
{
	u32 Row;
	u8 * p8;
	
  if(Comm_Ram->DisplayScanTimer!=0x0)
  {
    return;
  }
	DisplayClr();//����
	DisplayString(0,4,0,(u8 *)"USB�ļ�����");//��ʾ�ַ���
	
	p8=(u8 *)(&FileDownload->DownloadCount);//0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=...
	Row=1;
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��,700=			
	switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
	{
		case 1:
			Row=DisplayUSBStatus(Row,(u8 *)"E5_50",p8[0],ADDR_Download_ENTRY);
			break;
		case 2:
			Row=DisplayUSBStatus(Row,(u8 *)"E5_TC",p8[0],ADDR_Download_ENTRY);
			break;
		case 3:
			Row=DisplayUSBStatus(Row,(u8 *)"E5_ES",p8[0],ADDR_Download_ENTRY);
			break;
		case 4:
			Row=DisplayUSBStatus(Row,(u8 *)"E5_CX",p8[0],ADDR_Download_ENTRY);
			break;
		case 5:
			Row=DisplayUSBStatus(Row,(u8 *)"E5_01",p8[0],ADDR_Download_ENTRY);
			break;
	}
//#else
//	Row=DisplayUSBStatus(Row,(u8 *)"E5",p8[0],ADDR_Download_ENTRY);
#endif
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��,700=			
	switch(Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
	{
		case 1:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_50",p8[1],ADDR_Download_ENTRY);
			break;
		case 2:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_TC",p8[1],ADDR_Download_ENTRY);
			break;
		case 3:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_ES",p8[1],ADDR_Download_ENTRY);
			break;
		case 4:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_CX",p8[1],ADDR_Download_ENTRY);
			break;
		case 5:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_01",p8[1],ADDR_Download_ENTRY);
			break;
		case 6:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_LH",p8[1],ADDR_Download_ENTRY);
			break;
		case 7:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_NX",p8[1],ADDR_Download_ENTRY);
			break;
		case 8:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_QL",p8[1],ADDR_Download_ENTRY);
			break;
		case 101:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_MX",p8[1],ADDR_Download_ENTRY);
			break;
		case 102:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_FC",p8[1],ADDR_Download_ENTRY);
			break;
		case 103:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_NW",p8[1],ADDR_Download_ENTRY);
			break;
		case 104:
			Row=DisplayUSBStatus(Row,(u8 *)"E4_SR",p8[1],ADDR_Download_ENTRY);
			break;
			
	}
#else
	Row=DisplayUSBStatus(Row,(u8 *)"E4",p8[1],ADDR_Download_ENTRY);
#endif
	Row=DisplayUSBStatus(Row,(u8 *)"E3",p8[2],ADDR_MS_ENTRY);//��I0�汾��
	Row=DisplayUSBStatus(Row,(u8 *)"E2",p8[3],ADDR_Menu_ENTRY);	
	Row=DisplayUSBStatus(Row,(u8 *)"E1",p8[4],ADDR_Download_ENTRY);	
	Row=DisplayUSBStatus(Row,(u8 *)"I1",p8[5],ADDR_Meter_ENTRY);	
	Row=DisplayUSBStatus(Row,(u8 *)"I2",p8[6],ADDR_Terminal_ENTRY);	
	Row=DisplayUSBStatus(Row,(u8 *)"I0",p8[7],ADDR_MS_ENTRY);	

//	if(Comm_Ram->USBTask==1)//USB����
//	{
//		DisplayString(Row,8,0,(u8 *)"����");//��ʾ�ַ���
//	}

}


