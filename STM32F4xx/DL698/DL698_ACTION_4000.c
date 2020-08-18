
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_4000.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../MS/MS.h"
#include "../Terminal/Terminal_Init.h"
#include "../DL698/EVENT_Record.h"
#include "../Calculate/Calculate.h"
#include "../Device/RTC.h"



#include "../Terminal/Terminal_ReadMeter_ACSample.h"
#include "../Terminal/Terminal_ReadMeter_DL645_2007_FnData.h"
#include "../Terminal/Terminal_ReadMeter_SaveFnData.h"


void TEST_JCXL(void)
{
	u8 *p8;
	u32 *p32;
	u32 i;
	UARTCtrl_TypeDef *UARTCtrl1;
	UARTCtrl1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
	i=UARTCtrl1->Task;
//	if(MRR((Get_ADDR_UARTnRx(ACSAMPLEPORT)+1+7),1)==0x19)
	{//TEST
		p8=(u8 *)Get_ADDR_UARTnRx(ACSAMPLEPORT);
		p8[0]=0x68;
		MR(Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,ADDR_4001+2,6);//ͨ�ŵ�ַ
		Data_Inverse((u32)(p8+1),6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
		p8[7]=0x68;
		p8[8]=0x19;
		p8[9]=0x08;
		p8[10]=0x35;
		p8[11]=0x33;
		p8[12]=0x33;
		p8[13]=0x33;
		p8[14]=0x45;
		p8[15]=0x33;
		p8[16]=0x33;
		p8[17]=0x33;
		p8[18]=0x00;
		p8[19]=0x16;
		UARTCtrl1->RxByte=20;
		for(i=0;i<(UARTCtrl1->RxByte-2);i++)
		{
			p8[18]+=p8[i];
		}
	}
//	else
//	{
//		MC(0xaa,Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,6);//ͨ�ŵ�ַ��0xaa
//	}
//		MC(0xaa,Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,6);//ͨ�ŵ�ַ��0xaa
	i=UARTCtrl1->Task;//ԭ����Ĵ�
	UARTCtrl1->Task=1;//1=�������
	if(FileDownload->File&0x04)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
	{
		p32=(u32 *)(ADDR_Meter_ENTRY);
		(*(void(*)())p32[130])((u32)ACSAMPLEPORT);//����void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645���շ���
	}
	UARTCtrl1->Task=i;//ԭ����ָ�
	i=UARTCtrl1->TxByte;//�����ֽ���	
}


#if (USER/100)==17//����ϵ
u32 ACTION_4048(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//????????;??:b31-b16????????(???4??OMD),b15-b0?????????????(???4??OMD),??????UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	p8d=p8d;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	i=p8rx[2];
	switch(i)
	{
		case 127:
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//??DL698??????(??:???????);??:LENmax??NAND??????ADDR_128KDATABUFF?,??:b31-b8??,b7-b0????
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//??????         (9)
				return 0;
			}
			if(i>LENmax_4048_127)
			{
				UARTCtrl->DAR=8;//??                 (8)
				return (LenRx<<16)+0;
			}
			//??????????
			MW((u32)p8rx,ADDR_4048_127,i);
			
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=4;//?????(4)
			return 0;
	}
}
#endif

//�㲥Уʱ
u32 ACTION_4000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����һ�����󷽷�;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	p8d=p8d;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	i=p8rx[2];
	switch(i)
	{
		case 127://����127���㲥Уʱ
			p8rx+=4;
			LenRx=8;
			p8rx+=1;
			i=Check_pYYMDHMS_hex(p8rx);//��������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
			if(i)
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
#if (USER/100)!=0//���ǵ��Ժ����
			//���ǵ��Ժ������Ƚ�ʱ�Ӳ�>5�֣����չ㲥Уʱ��־
			i=YYMDHMS_Sub_hex(p8rx,(u8*)ADDR_DL698YMDHMS);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
			if(i>>31)
			{
				i=~i;
				i++;
			}
			if(i>(5*60))//ʱ�Ӳ�>5��
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			i=MRR(ADDR_BroadcasTimeFlag,1);//���ܱ��չ㲥Уʱ��־
			if(i)
			{//0=û�㲥Уʱ��1=�ѹ㲥Уʱ
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			MWR(1,ADDR_BroadcasTimeFlag,1);//���ܱ��չ㲥Уʱ��־
#endif
			
//			//RS485-1
//			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
//			//��������
//			i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
//			i|=UARTCtrl->BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
//			i|=UARTCtrl->AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
//			i|=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
//			if(i==0)
//			{//����������
//				UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+2;
//				UARTCtrl->BroadcastTimeTask=1;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
//			}
//			//RS485-2
//			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
//			//��������
//			i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
//			i|=UARTCtrl->BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
//			i|=UARTCtrl->AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
//			i|=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
//			if(i==0)
//			{//����������
//				UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+2;
//				UARTCtrl->BroadcastTimeTask=1;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
//			}
//		#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//			//�ز��ڹ㲥Уʱ
//			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//			//��������
//			i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
//			i|=UARTCtrl->BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
//			i|=UARTCtrl->AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
//			i|=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
//			if(i==0)
//			{//����������
//				UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+2;
//				UARTCtrl->BroadcastTimeTask=1;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
//			}
//		#endif

			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
			p8d=(u8*)ADDR_RTCBUFF;
			i=p8rx[6];
			i=hex_bcd(i);
			p8d[0]=i;
			i=p8rx[5];
			i=hex_bcd(i);
			p8d[1]=i;
			i=p8rx[4];
			i=hex_bcd(i);
			p8d[2]=i;
			i=p8rx[3];
			i=hex_bcd(i);
			p8d[4]=i;
			i=p8rx[2];
			i=hex_bcd(i);
			p8d[5]=i;
			i=(p8rx[0]<<8)+p8rx[1];
			i=hex_bcd(i);
			p8d[6]=i;
			WriteRTC(ADDR_RTCBUFF);//дʵʱ��
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=4;//����δ���壨4��
			return 0;
	}
}


//·����Ϣ��Ԫ
u32 ACTION_4200(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����һ�����󷽷�;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenRx=0;
	LenTx=0;
	Method=p8rx[2];
	p8rx+=4;
	switch(Method)
	{
//����127��Add��·����Ϣ��Ԫ�����һ��·����Ϣ��Ԫ��
//����128��AddBatch��array·����Ϣ��Ԫ���������·����Ϣ��Ԫ��
//����129��Update��TSA��·����Ϣ��Ԫ������·�ɱ��·����Ϣ��Ԫ����
//����134��Clear()//���·�ɱ�
		case 127://����127��Add��·����Ϣ��Ԫ�����һ��·����Ϣ��Ԫ��
			
		case 128://����128��AddBatch��array·����Ϣ��Ԫ���������·����Ϣ��Ԫ��
			
		case 129://����129��Update��TSA��·����Ϣ��Ԫ������·�ɱ��·����Ϣ��Ԫ����
			
		case 134://����134��Clear()//���·�ɱ�
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=4;//����δ����           ��4��
			return 0;
	}
}

//�����豸
#include "../DL698/CALC_RESET_FLOW_TIME.h"
u32 ACTION_4300(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����һ�����󷽷�;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 Method;
	u32 LenRx;
  u8 * p8;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	//u32 i;
	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenRx=1;
	Method=p8rx[2];
	p8rx+=4;
	switch(Method)
	{
//1����λ	��ѡ
//2��ִ��	��ѡ
//3. ���ݳ�ʼ��	��ѡ
//4. �ָ���������	��ѡ
//5. �¼���ʼ��	��ѡ
//6. ������ʼ��	��ѡ
		case 1://1����λ	��ѡ
			Comm_Ram->RESET=0x55;//0x55=������(����ʹ�ÿ��Ź���λ)
			Comm_Ram->ACTION_RESET_PORTn=PORTn;//������λ��ͨ�Ŷ˿ں�
		#ifdef IC_NUC9xx
			//��ʱ�ȴ�����ֽڷ���
			Comm_Ram->WaitUartCompleteMSTimer=5000/10;//�ȴ�UART֡���ض�ʱ�� ÿ10ms��1��ʱ��
		#endif
			return (LenRx<<16)+0;
		case 2:///2��ִ��	��ѡ
			return (LenRx<<16)+0;
		case 3://3. ���ݳ�ʼ��	��ѡ
      p8=(u8 *)Get_ADDR_UARTnRx(PORTn);
      Method=(p8[4]>>4)&0x3;
      if(Method==1)
      {//�߼���ַ=1������
        Method=MRR(ADDR_F101_2+1,1);//��ȫģʽ����
        if(Method)
        {//���ð�ȫģʽSECURITY
          if((UARTCtrl->SECURITY_Request&0xC0)!=0xC0)//��ȫ����:b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID 
          {
            UARTCtrl->DAR=20;
            return (LenRx<<16)+0;
          }
        }
        //��SaveRam
        MC(0,ADDR_Meter_SaveRam_Start,LEN_Meter_SaveRam);
        //��EFLASH
        MC(0,ADDR_MeterEnergy_EFLASH_Start,(ADDR_MeterClr_End)-(ADDR_MeterEnergy_EFLASH_Start));
        MC(0,ADDR_Meter_EFLASH_Ext_Start,(ADDR_Meter_EFLASH_Ext_End)-(ADDR_Meter_EFLASH_Ext_Start));//�Ǳ������չ
        return (LenRx<<16)+0;
      }
			Init_ALLDATA();//�ն�ȫ�����ݳ�ʼ��(��0)
			Event_OAD(0x31000200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
#if (USER/100)==15//��΢ר��
//---������----
			Comm_Ram->RESET=0x55;//0x55=������(����ʹ�ÿ��Ź���λ)
			Comm_Ram->ACTION_RESET_PORTn=PORTn;//������λ��ͨ�Ŷ˿ں�
		#ifdef IC_NUC9xx
			//��ʱ�ȴ�����ֽڷ���
			Comm_Ram->USBMSTimer=5000/10;//26 USB ÿ10MS��1��ʱ��
		#endif
//---������ END----
#else
//---��������---
			//����Ժ����������������޹���bug,�ʲ�������λ,�����Ӽ�¼��λ����
			RESET_Count();//�ն˸�λ����
//---�������� END---
#endif
			return (LenRx<<16)+0;
		case 4://4. �ָ���������	��ѡ
//������=array OAD ���豸�����ûָ����������á�
//���������OAD�б�������״�����ڻָ�֮�С�
			#if (USER/100)==0//���Ժ				
				DL698_SETINIT(3,p8rx);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
			#else
				if(Comm_Ram->Factory==0x55)
				{//����ģʽ
					DL698_SETINIT(3,p8rx);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
				}
				else
				{
					DL698_SETINIT(4,p8rx);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
				}
				Init_ALLDATA();//�ն�ȫ�����ݳ�ʼ��(��0)
			#endif
			Event_OAD(0x31000200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			
			Comm_Ram->RESET=0x55;//0x55=������(����ʹ�ÿ��Ź���λ)
			Comm_Ram->ACTION_RESET_PORTn=PORTn;//������λ��ͨ�Ŷ˿ں�
		#ifdef IC_NUC9xx
			//��ʱ�ȴ�����ֽڷ���
			Comm_Ram->WaitUartCompleteMSTimer=5000/10;//�ȴ�UART֡���ض�ʱ�� ÿ10ms��1��ʱ��
		#endif
			LenRx=p8rx[1];
			LenRx*=5;
			LenRx+=2;
			return (LenRx<<16)+0;
		case 5://5. �¼���ʼ��	��ѡ
			ms=Get_ms();
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
			{
				if(file->BLOCK_ENTRY)
				{
					if((file->FILEID&0xff)==2)
					{//�ļ�����=2,�¼���¼
						ms->msfile_Delete(filename);
					}
				}
				file++;
			}
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			for(filename=0;filename<recordFILENAMEmax_RAM;filename++)
			{
				if(file->BLOCK_ENTRY)
				{
					if((file->FILEID&0xff)==2)
					{//�ļ�����=2,�¼���¼
						ms->msfile_Delete(filename+FILENAMEmax_FLASH);
					}
				}
				file++;
			}
			//���¼���ǰֵ��¼��
			MC(0,ADDR_EventCurrentvalue_3003,LENper_EventCurrentvalue*NUM_EventCurrentvalue);
			
			Event_OAD(0x31000200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			return (LenRx<<16)+0;
		case 6://6. ������ʼ��	��ѡ
//			//��ǰ������������
//			UARTCtrl1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
//			p8rxbuff=(u8 *)Get_ADDR_UARTnRx(ACSAMPLEPORT);
//			p8txbuff=(u8 *)Get_ADDR_UARTnTx(ACSAMPLEPORT);
//			Task=UARTCtrl1->Task;//ԭ����Ĵ�
//			UARTCtrl1->Task=1;//1=�������
//			UARTCtrl1->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
//			p8rxbuff[0]=0xFE;
//			p8rxbuff[1]=0xFE;
//			p8rxbuff[2]=0xFE;
//			p8rxbuff[3]=0xFE;
//			p8rxbuff+=4;
//			p8rxbuff[0]=0x68;
//			MC(0xaa,((u32)p8rxbuff)+1,6);
//			p8rxbuff[7]=0x68;
//			p8rxbuff[8]=0x19;//������
//			p8rxbuff[9]=8;//���ݳ���
//			p8rxbuff[10]=0x35;
////			p8rxbuff[11]=0x44;
////			p8rxbuff[12]=0x44;
////			p8rxbuff[13]=0x44;
//			p8rxbuff[11]=0x33;
//			p8rxbuff[12]=0x33;
//			p8rxbuff[13]=0x33;
//			p8rxbuff[14]=0x45;
//			p8rxbuff[15]=0x33;
//			p8rxbuff[16]=0x33;
//			p8rxbuff[17]=0x33;
//			p8rxbuff[18]=0x99;
//			p8rxbuff[19]=0x16;
//			UARTCtrl1->RxByte=24;
//			p32=(u32 *)(ADDR_Meter_ENTRY);
//			//MR((u32)p8rxbuff,(u32)p8txbuff,UARTCtrl->TxByte);
//			(*(void(*)())p32[130])((u32)ACSAMPLEPORT); //����void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645���շ���
//			//MR((u32)p8rxbuff,(u32)p8txbuff,UARTCtrl->TxByte);
//			UARTCtrl1->Task=Task;//ԭ����ָ�

			p8=(u8 *)Get_ADDR_UARTnRx(PORTn);
      Method=(p8[4]>>4)&0x3;
      if(Method==1)
      {//�߼���ַ=1������
        Method=MRR(ADDR_F101_2+1,1);//��ȫģʽ����
        if(Method)
        {//���ð�ȫģʽSECURITY
          if((UARTCtrl->SECURITY_Request&0xC0)!=0xC0)//��ȫ����:b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID 
          {
            UARTCtrl->DAR=20;
            return (LenRx<<16)+0;
          }
        }
				TEST_JCXL();//��645��������
				return (LenRx<<16)+0;
			}
//			
//			
//			if(Comm_Ram->Factory==0x55)
//			{//�ǹ���ģʽ
//				//��������ģʽ��ʱ
//				i=MRR(ADDR_MeterFMTimer+14,2);//����ģʽ��ʱ
//				//��SaveRam
//				MC(0,ADDR_Meter_SaveRam_Start,LEN_Meter_SaveRam*2);
//				
//				//�ָ�����ģʽ��ʱ
//				MWR(i,ADDR_MeterFMTimer+14,2);//����ģʽ��ʱ
//				//��EFLASH
//				//MC(0,ADDR_MeterEnergy_EFLASH_Start,(ADDR_MeterClr_End)-(ADDR_MeterEnergy_EFLASH_Start));
//				//MC(0,ADDR_Meter_EFLASH_Ext_Start,(ADDR_Meter_EFLASH_Ext_End)-(ADDR_Meter_EFLASH_Ext_Start));//�Ǳ������չ
//				MC(0,ADDR_MeterDemand_EFLASH_Start,(ADDR_MeterDemand_EFLASH_End)-(ADDR_MeterDemand_EFLASH_Start));
//				return (LenRx<<16)+0;
//			}
//			
			
			
			//			//��ǰ������������
				
		
			Event_OAD(0x31000200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			return (LenRx<<16)+0;
		default:
			UARTCtrl->DAR=4;//����δ����           ��4��
			return (LenRx<<16)+0;
	}
}




























