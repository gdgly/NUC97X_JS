
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Display/DisplayLoop_Contrl.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"


#if ((Project/100)!=3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
u32 DisplayLoop_Contrl(u32 Row)//�ն˿�����ʾ
{
	return Row;
}
#else
//�߷Ѹ澯��ʾ������=0
//�����բ֪ͨ��ʾ������=1
//�������:ң����բ,��ǰ�����¸���,Ӫҵ��ͣ����,���ݹ���,ʱ�ι���,�µ��,�����=2
#define DisplayRemoteCount     (3+MaxControlRound)//ң����բ��ʾ������
#define DisplayPowerCtrl1Count     (DisplayRemoteCount+MaxTotalAddGroup)//��ǰ�����¸�����ʾ������
#define DisplayPowerCtrl2Count     (DisplayPowerCtrl1Count+MaxTotalAddGroup)//Ӫҵ��ͣ������ʾ������
#define DisplayPowerCtrl3Count     (DisplayPowerCtrl2Count+MaxTotalAddGroup)//���ݹ�����ʾ������
#define DisplayPowerCtrl4Count     (DisplayPowerCtrl3Count+MaxTotalAddGroup)//ʱ�ι�����ʾ������
#define DisplayEnergyCtrl1Count     (DisplayPowerCtrl4Count+MaxTotalAddGroup)//�µ����ʾ������
#define DisplayEnergyCtrl2Count     (DisplayEnergyCtrl1Count+MaxTotalAddGroup)//�������ʾ������

u32 DisplayLoop_Contrl(u32 Row)//�ն˿�����ʾ
{
	u32 i;
	u32 x;
	u32 Pn;
	u64 d64;
	u64 a64;
	u64 b64;
	u8 *p8;
	u16 *p16;

//�߷Ѹ澯
	if(TerminalDisplayLoop->Count==0)
	{
		x=MRR(ADDR_AFN05F26,1);
		if(x==0x55)
		{
			DisplayString(Row,0,0,(u8 *)"�߷Ѹ澯!");//��ʾ�ַ���
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
		}
		return Row;
	}
//�����բ֪ͨ
	if(TerminalDisplayLoop->Count==1)
	{
		p8=(u8 *)(ADDR_Terminal_Total_State);//�ն�����բ
		x=p8[1];
		x&=0x0f;//Ŀǰ���ֻ��4��
		while(x!=0x0)
		{
			i=BitLoca(x);//�����1����λΪ1��λ��(0-32)
			DisplayString(Row,0,0,(u8 *)"�ִ�?��բ����");//��ʾ�ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[4]=i+0x2030;
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			i--;
			i=1<<i;
			i=~i;
			x&=i;
		}
		return Row;
	}

//���ƽ��
	if(TerminalDisplayLoop->Count==2)
	{
		//ң����բ
		x=0;
		for(i=0;i<MaxControlRound;i++)
		{
			x+=MRR(ADDR_AFN05F1+(2*i),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"ң����բ��ǰ�޿���");//��ʾ�ַ���
			Row++;
		}
		//��ǰ�����¸���
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWCtrl1_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"�����¸��ص�ǰ�޿���");//��ʾ�ַ���
			Row++;
		}
		//Ӫҵ��ͣ����
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWCtrl2_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"��ͣ���ص�ǰ�޿���");//��ʾ�ַ���
			Row++;
		}
		//���ݹ���
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWCtrl3_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"���ݹ��ص�ǰ�޿���");//��ʾ�ַ���
			Row++;
		}
		//ʱ�ι���
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWCtrl4_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"ʱ�ι��ص�ǰ�޿���");//��ʾ�ַ���
			Row++;
		}
		//�µ��
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWhCtrl1_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"�µ�ص�ǰ�޿���");//��ʾ�ַ���
			Row++;
		}
		//�����
		x=0;
		for(i=0;i<MaxTotalAddGroup;i++)
		{
			x+=MRR(ADDR_Terminal_kWhCtrl2_State+(i*Terminal_Ctrl_Pn_Len),1);
		}
		if(x==0)
		{
			DisplayString(Row,0,0,(u8 *)"����ص�ǰ�޿���");//��ʾ�ַ���
			Row++;
		}
		return Row;
	}
	
		
//ң����բ
	if(TerminalDisplayLoop->Count<DisplayRemoteCount)
	{
		Pn=TerminalDisplayLoop->Count-(DisplayRemoteCount-MaxControlRound);
		if(MRR(ADDR_AFN05F1+(2*Pn),1)!=0x0)
		{
			DisplayString(Row,0,0,(u8 *)"�ִ�?ң����բͶ��");//��ʾ�ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[4]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;

			p8=(u8 *)(ADDR_Terminal_Remote_State+(Terminal_Ctrl_Pn_Len*Pn));
			x=p8[0];
			if(x==100)
			{//����
				DisplayString(Row,0,0,(u8 *)"�ִ�?ң����բ����");//��ʾ�ַ���
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[4]=(Pn+1)+0x2030;
			}
			if(x>=101)
			{//��բ
				DisplayString(Row,0,0,(u8 *)"�ִ�?ң����բ");//��ʾ�ַ���
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[4]=(Pn+1)+0x2030;
			}

			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
		}
		return Row;
	}
//��ǰ�����¸���
	if(TerminalDisplayLoop->Count<DisplayPowerCtrl1Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayRemoteCount;
		p8=(u8 *)(ADDR_Terminal_kWCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//Ͷ��
			DisplayString(Row,0,0,(u8 *)"�ܼ���?�¸���Ͷ��");//��ʾ�ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��ǰ����
			DisplayString(Row,0,0,(u8 *)"��ǰ����=");//��ʾ�ַ���
			i=MRR(ADDR_AFN0CF17+(Pn*2),2);
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//������ֵ
			DisplayString(Row,0,0,(u8 *)"������ֵ=");//��ʾ�ַ���
			i=MRR(ADDR_AFN04F17,2);//�����������ݵ��Ĵ���,Byte<=8
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			if((d64>>63)!=0x0)
			{//��
				d64=(~d64)+1;
			}
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			if(x>2)
			{//��ֵ�Ѽ���
				//��ֵ����
				DisplayString(Row,0,0,(u8 *)"��ֵ����=");//��ʾ�ַ���
				i=MRR(ADDR_kWCtrl1_FixedValue+(Pn*2),2);//�����������ݵ��Ĵ���,Byte<=8
				d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
				d64=hex_bcd16(d64);
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
				//��������բ
				if(x==100)
				{//���ʳ��ޱ���
					DisplayString(Row,0,0,(u8 *)"���ʳ��ޱ���");//��ʾ�ַ���
					DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					Row++;
				}
				else
				{//��բ
					x=p8[1];
					if(x!=0x0)
					{//����բ
						DisplayString(Row,0,0,(u8 *)"��բ�ִ�");//��ʾ�ַ���
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
						for(i=1;i<=8;i++)
						{
							if((x&0x1)!=0x0)
							{
								p16[0]=i+0x2030;
								p16[1]=','+0x2000;
								p16+=2;
							}
							x>>=1;
						}
						p16--;
						p16[0]=0;//ɾ�����','
						DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
						Row++;
					}
				}
			}
		}
		else
		{
			x=p8[4];
			if(x!=0x0)
			{
				//ûͶ��ԭ��
				DisplayString(Row,0,0,(u8 *)"�ܼ���?�¸���Ͷ��");//��ʾ�ַ���
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[6]=(Pn+1)+0x2030;
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
				DisplayString(Row,0,0,(u8 *)"����ʱ�䶨ʱ��");//��ʾ�ַ���
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
		}
		return Row;
	}
//Ӫҵ��ͣ����
	if(TerminalDisplayLoop->Count<DisplayPowerCtrl2Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayPowerCtrl1Count;
		p8=(u8 *)(ADDR_Terminal_kWCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//Ͷ��
			DisplayString(Row,0,0,(u8 *)"�ܼ���?��ͣ��Ͷ��");//��ʾ�ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��ǰ����
			DisplayString(Row,0,0,(u8 *)"��ǰ����=");//��ʾ�ַ���
			i=MRR(ADDR_AFN0CF17+(Pn*2),2);
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//������ֵ
			DisplayString(Row,0,0,(u8 *)"������ֵ=");//��ʾ�ַ���
			i=MRR(ADDR_AFN04F17,2);//�����������ݵ��Ĵ���,Byte<=8
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			if((d64>>63)!=0x0)
			{//��
				d64=(~d64)+1;
			}
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��ֵ����
			DisplayString(Row,0,0,(u8 *)"��ֵ����=");//��ʾ�ַ���
			i=MRR(ADDR_AFN04F44+(Pn*8)+6,2);//�����������ݵ��Ĵ���,Byte<=8
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��������բ
			if(x==100)
			{//���ʳ��ޱ���
				DisplayString(Row,0,0,(u8 *)"���ʳ��ޱ���");//��ʾ�ַ���
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
			else
			{//��բ
				x=p8[1];
				if(x!=0x0)
				{//����բ
					DisplayString(Row,0,0,(u8 *)"��բ�ִ�");//��ʾ�ַ���
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//ɾ�����','
					DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					Row++;
				}
			}
		}
		else
		{
			x=p8[4];
			if(x!=0x0)
			{
				//ûͶ��ԭ��
				DisplayString(Row,0,0,(u8 *)"�ܼ���?��ͣ��Ͷ��");//��ʾ�ַ���
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[6]=(Pn+1)+0x2030;
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
				switch(x)
				{
					case 1:
						DisplayString(Row,0,0,(u8 *)"û����ͣ��ʼʱ��");//��ʾ�ַ���
						break;
					case 2:
						DisplayString(Row,0,0,(u8 *)"�ѵ���ͣ����ʱ��");//��ʾ�ַ���
						break;
					case 3:
						DisplayString(Row,0,0,(u8 *)"�����ȼ����¸���");//��ʾ�ַ���
						break;
					default:
						DisplayString(Row,0,0,(u8 *)"�ն�ԭ��ûͶ��");//��ʾ�ַ���
						break;
				}
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
		}
		return Row;
	}
//���ݹ���
	if(TerminalDisplayLoop->Count<DisplayPowerCtrl3Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayPowerCtrl2Count;
		p8=(u8 *)(ADDR_Terminal_kWCtrl3_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//Ͷ��
			DisplayString(Row,0,0,(u8 *)"�ܼ���?���ݿ�Ͷ��");//��ʾ�ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��ǰ����
			DisplayString(Row,0,0,(u8 *)"��ǰ����=");//��ʾ�ַ���
			i=MRR(ADDR_AFN0CF17+(Pn*2),2);
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//������ֵ
			DisplayString(Row,0,0,(u8 *)"������ֵ=");//��ʾ�ַ���
			i=MRR(ADDR_AFN04F17,2);//�����������ݵ��Ĵ���,Byte<=8
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			if((d64>>63)!=0x0)
			{//��
				d64=(~d64)+1;
			}
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��ֵ����
			DisplayString(Row,0,0,(u8 *)"��ֵ����=");//��ʾ�ַ���
			i=MRR(ADDR_AFN04F42+(Pn*6),2);//�����������ݵ��Ĵ���,Byte<=8
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��������բ
			if(x==100)
			{//���ʳ��ޱ���
				DisplayString(Row,0,0,(u8 *)"���ʳ��ޱ���");//��ʾ�ַ���
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
			else
			{//��բ
				x=p8[1];
				if(x!=0x0)
				{//����բ
					DisplayString(Row,0,0,(u8 *)"��բ�ִ�");//��ʾ�ַ���
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//ɾ�����','
					DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					Row++;
				}
			}
		}
		else
		{
			x=p8[4];
			if(x!=0x0)
			{
				//ûͶ��ԭ��
				DisplayString(Row,0,0,(u8 *)"�ܼ���?���ݿ�Ͷ��");//��ʾ�ַ���
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[6]=(Pn+1)+0x2030;
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
				switch(x)
				{
					case 1:
						DisplayString(Row,0,0,(u8 *)"ʱ��?������");//��ʾ�ַ���
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p8=(u8 *)(ADDR_Terminal_Period);
						p16[4]=p8[3]+0x2030;
						break;
					case 2:
						DisplayString(Row,0,0,(u8 *)"�ǳ�����");//��ʾ�ַ���
						break;
					case 3:
						DisplayString(Row,0,0,(u8 *)"���޵�ʱ��");//��ʾ�ַ���
						break;
					case 4:
						DisplayString(Row,0,0,(u8 *)"�����ȼ����¸���");//��ʾ�ַ���
						break;
					case 5:
						DisplayString(Row,0,0,(u8 *)"�����ȼ��߱�ͣ��");//��ʾ�ַ���
						break;
					default:
						DisplayString(Row,0,0,(u8 *)"�ն�ԭ��ûͶ��");//��ʾ�ַ���
						break;
				}
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
		}
		return Row;
	}
//ʱ�ι���	
	if(TerminalDisplayLoop->Count<DisplayPowerCtrl4Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayPowerCtrl3Count;
		p8=(u8 *)(ADDR_Terminal_kWCtrl4_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//Ͷ��
			DisplayString(Row,0,0,(u8 *)"�ܼ���?ʱ�ο�Ͷ��");//��ʾ�ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��ǰ����
			DisplayString(Row,0,0,(u8 *)"��ǰ����=");//��ʾ�ַ���
			i=MRR(ADDR_AFN0CF17+(Pn*2),2);
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//������ֵ
			DisplayString(Row,0,0,(u8 *)"������ֵ=");//��ʾ�ַ���
			i=MRR(ADDR_AFN04F17,2);//�����������ݵ��Ĵ���,Byte<=8
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			if((d64>>63)!=0x0)
			{//��
				d64=(~d64)+1;
			}
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��ֵ����
			DisplayString(Row,0,0,(u8 *)"��ֵ����=");//��ʾ�ַ���
			i=MRR(((u32)p8)+6,2);//�����������ݵ��Ĵ���,Byte<=8
			d64=Data02_kW(i);//���ݸ�ʽ02תΪ��λͳһΪkw��hex����,��3λС��
			d64=hex_bcd16(d64);
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(d64,8,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			DispalyStringContinue(p16,(u8 *)"kW");//��ʾ�ַ�������
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��������բ
			if(x==100)
			{//���ʳ��ޱ���
				DisplayString(Row,0,0,(u8 *)"���ʳ��ޱ���");//��ʾ�ַ���
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
			else
			{//��բ
				x=p8[1];
				if(x!=0x0)
				{//����բ
					DisplayString(Row,0,0,(u8 *)"��բ�ִ�");//��ʾ�ַ���
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//ɾ�����','
					DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					Row++;
				}
			}
		}
		else
		{
			x=p8[4];
			if(x!=0x0)
			{
				//ûͶ��ԭ��
				DisplayString(Row,0,0,(u8 *)"�ܼ���?ʱ�ο�Ͷ��");//��ʾ�ַ���
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
				p16[6]=(Pn+1)+0x2030;
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
				switch(x)
				{
					case 1:
						DisplayString(Row,0,0,(u8 *)"ʱ��?������");//��ʾ�ַ���
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p8=(u8 *)(ADDR_Terminal_Period);
						p16[4]=p8[2]+0x2030;
						break;
					case 2:
						DisplayString(Row,0,0,(u8 *)"����ʱ�����ó�����");//��ʾ�ַ���
						break;
					case 3:
						DisplayString(Row,0,0,(u8 *)"ʱ��?ûͶ��");//��ʾ�ַ���
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p8=(u8 *)(ADDR_Terminal_Period);
						p16[4]=p8[2]+0x2030;
						break;
					case 4:
						DisplayString(Row,0,0,(u8 *)"��Ч������");//��ʾ�ַ���
						break;
					case 5:
						DisplayString(Row,0,0,(u8 *)"����?�޶�ֵ");//��ʾ�ַ���
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p16[4]=p8[5]+1+0x2030;
						break;
					case 6:
						DisplayString(Row,0,0,(u8 *)"����?ʱ��?��ֵû����");//��ʾ�ַ���
						p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
						p16[4]=p8[5]+1+0x2030;
						p8=(u8 *)(ADDR_Terminal_Period);
						p16[9]=p8[2]+0x2030;
						break;
					case 7:
						DisplayString(Row,0,0,(u8 *)"�����ȼ����¸���");//��ʾ�ַ���
						break;
					case 8:
						DisplayString(Row,0,0,(u8 *)"�����ȼ��߱�ͣ��");//��ʾ�ַ���
						break;
					case 9:
						DisplayString(Row,0,0,(u8 *)"�����ȼ��߳��ݿ�");//��ʾ�ַ���
						break;
					default:
						DisplayString(Row,0,0,(u8 *)"�ն�ԭ��ûͶ��");//��ʾ�ַ���
						break;
				}
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
		}
		return Row;
	}
//�µ��
	if(TerminalDisplayLoop->Count<DisplayEnergyCtrl1Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayPowerCtrl4Count;
		p8=(u8 *)(ADDR_Terminal_kWhCtrl1_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//Ͷ��
			DisplayString(Row,0,0,(u8 *)"�ܼ���?�µ��Ͷ��");//��ʾ�ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//���µ���
			DisplayString(Row,0,0,(u8 *)"���µ���=");//��ʾ�ַ���
			i=MRR(ADDR_AFN0CF21+((1+4+(4*TMaxTariff))*Pn)+1,4);
			x=i&0x0fffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(x,7,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			if((i&0x40000000)==0x0)
			{//kWh
				DispalyStringContinue(p16,(u8 *)"kWh");//��ʾ�ַ�������
			}
			else
			{//MWh
				DispalyStringContinue(p16,(u8 *)"MWh");//��ʾ�ַ�������
			}
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			//��ֵ����
			DisplayString(Row,0,0,(u8 *)"������ֵ=");//��ʾ�ַ���
			
			//ȡ�µ������ض�ֵ
			i=MRR(ADDR_AFN04F46+(5*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
			i&=0xefffffff;//ȥ����λ
			a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
			//ȡ�ն��µ������ض�ֵ����ϵ��
			i=MRR(ADDR_AFN04F20,1);//�����������ݵ��Ĵ���,Byte<=8
			x=i;
			i&=0x7f;
			i=bcd_hex(i);
			b64=(a64*i)/100;
			if((x>>7)==0x0)
			{//S0=0����ʾ�ϸ�
				a64+=b64;
			}
			else
			{//S0=1����ʾ�¸�
				a64-=b64;
			}
			//��ǰ��ֵ
			i=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
			//i=MRR(((u32)p8)+2,4);
			x=i&0x0fffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(x,7,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			if((i&0x40000000)==0x0)
			{//kWh
				DispalyStringContinue(p16,(u8 *)"kWh");//��ʾ�ַ�������
			}
			else
			{//MWh
				DispalyStringContinue(p16,(u8 *)"MWh");//��ʾ�ַ�������
			}
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			
			//��������
			DisplayString(Row,0,0,(u8 *)"��������=");//��ʾ�ַ���
			i=MRR(ADDR_AFN04F46+(5*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
			i&=0xefffffff;//ȥ����λ
			a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
			//ȡ�������޸���ϵ��
			i=MRR(ADDR_AFN04F46+(5*Pn)+4,1);//�����������ݵ��Ĵ���,Byte<=8
//			x=i;
//			i&=0x7f;
//			i=bcd_hex(i);
//			b64=(a64*i)/100;
//			if((x>>7)==0x0)
//			{//S0=0����ʾ�ϸ�
//				a64+=b64;
//			}
//			else
//			{//S0=1����ʾ�¸�
//				a64-=b64;
//			}
			i=bcd_hex(i);
			b64=i;
			a64*=b64;
			a64/=100;
			//����ֵ
			i=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
			x=i&0x0fffffff;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
			p16=DisplayData_OffZero(x,7,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			if((i&0x40000000)==0x0)
			{//kWh
				DispalyStringContinue(p16,(u8 *)"kWh");//��ʾ�ַ�������
			}
			else
			{//MWh
				DispalyStringContinue(p16,(u8 *)"MWh");//��ʾ�ַ�������
			}
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			
			//��������բ
			if(p8[0]==100)
			{//�������ޱ���
				DisplayString(Row,0,0,(u8 *)"���õ糬�ޱ���");//��ʾ�ַ���
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
			else
			{//��բ
				x=p8[1];
				if(x!=0x0)
				{//����բ
					DisplayString(Row,0,0,(u8 *)"��բ�ִ�");//��ʾ�ַ���
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//ɾ�����','
					DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					Row++;
				}
			}
		}
		return Row;
	}
//�����
	if(TerminalDisplayLoop->Count<DisplayEnergyCtrl2Count)
	{
		Pn=TerminalDisplayLoop->Count-DisplayEnergyCtrl1Count;
		p8=(u8 *)(ADDR_Terminal_kWhCtrl2_State+(Terminal_Ctrl_Pn_Len*Pn));
		x=p8[0];
		if(x!=0x0)
		{
			//Ͷ��
			DisplayString(Row,0,0,(u8 *)"�ܼ���?�����Ͷ��");//��ʾ�ַ���
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4);
			p16[6]=(Pn+1)+0x2030;
			DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
			Row++;
			if(p8[6]==0x0)//���������ʣ�����(��)����ʱʶ��,0=������,1=�����
			{//0=������
				//ʣ�����
				DisplayString(Row,0,0,(u8 *)"ʣ�����=");//��ʾ�ַ���
				i=MRR(ADDR_AFN0CF23+(4*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				if((i&0x10000000)!=0x0)
				{//��ֵ
					p16[0]='-'+0x2000;
					p16++;
				}
				x=i&0x0fffffff;
				p16=DisplayData_OffZero(x,7,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				if((i&0x40000000)==0x0)
				{//kWh
					DispalyStringContinue(p16,(u8 *)"kWh");//��ʾ�ַ�������
				}
				else
				{//MWh
					DispalyStringContinue(p16,(u8 *)"MWh");//��ʾ�ַ�������
				}
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
				//��բ����
				DisplayString(Row,0,0,(u8 *)"��բ����=");//��ʾ�ַ���
				i=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//�����������ݵ��Ĵ���,Byte<=8
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				if((i&0x10000000)!=0x0)
				{//��ֵ
					p16[0]='-'+0x2000;
					p16++;
				}
				x=i&0x0fffffff;
				p16=DisplayData_OffZero(x,7,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				if((i&0x40000000)==0x0)
				{//kWh
					DispalyStringContinue(p16,(u8 *)"kWh");//��ʾ�ַ�������
				}
				else
				{//MWh
					DispalyStringContinue(p16,(u8 *)"MWh");//��ʾ�ַ�������
				}
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
			else
			{//1=�����
				//ʣ����
				DisplayString(Row,0,0,(u8 *)"ʣ����=");//��ʾ�ַ���
				i=MRR(ADDR_AFN0CF23+(4*Pn),4);//�����������ݵ��Ĵ���,Byte<=8
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				if((i&0x10000000)!=0x0)
				{//��ֵ
					p16[0]='-'+0x2000;
					p16++;
				}
				i&=0xefffffff;
				d64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				d64=hex_bcd16(d64);//HEX��תΪ16λѹ��BCD��
				p16=DisplayData_OffZero(d64,10,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				DispalyStringContinue(p16,(u8 *)"Ԫ");//��ʾ�ַ�������
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
				//��բ����
				DisplayString(Row,0,0,(u8 *)"��բ����=");//��ʾ�ַ���
				i=MRR(ADDR_AFN04F47+(17*Pn)+13,4);//�����������ݵ��Ĵ���,Byte<=8
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+18);
				if((i&0x10000000)!=0x0)
				{//��ֵ
					p16[0]='-'+0x2000;
					p16++;
				}
				i&=0xefffffff;
				d64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
				d64=hex_bcd16(d64);//HEX��תΪ16λѹ��BCD��
				p16=DisplayData_OffZero(d64,10,3,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				DispalyStringContinue(p16,(u8 *)"Ԫ");//��ʾ�ַ�������
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
			//��������բ
			if(p8[0]==100)
			{//�������ޱ���
				if(p8[6]==0x0)
				{
					DisplayString(Row,0,0,(u8 *)"ʣ�����Ƿ�ޱ���");//��ʾ�ַ���
				}
				else
				{
					DisplayString(Row,0,0,(u8 *)"ʣ����Ƿ�ޱ���");//��ʾ�ַ���
				}
				DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
				Row++;
			}
			else
			{//��բ
				x=p8[1];
				if(x!=0x0)
				{//����բ
					DisplayString(Row,0,0,(u8 *)"��բ�ִ�");//��ʾ�ַ���
					p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+16);
					for(i=1;i<=8;i++)
					{
						if((x&0x1)!=0x0)
						{
							p16[0]=i+0x2030;
							p16[1]=','+0x2000;
							p16+=2;
						}
						x>>=1;
					}
					p16--;
					p16[0]=0;//ɾ�����','
					DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
					Row++;
				}
			}
		}
		return Row;
	}

	return Row;
}
#endif//#if ((Project/100)!=3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��




