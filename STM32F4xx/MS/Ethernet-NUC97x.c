
#if MainProtocol==376
#include "../Head/Project.h"
#include "../terminal/Terminal_USER11xx.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_Uart.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#endif

#include "../MS/Ethernet.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/24AA02E48.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"

#ifdef IC_STM32F4xx
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../Device/LAN8720A.h"
#endif
#ifdef IC_NUC9xx
#include "../../NUC97x/NUC97x/NUC97x_MAC.h"
#include "../../NUC97x/Driver/NUC97x_IP101GR.h"
#endif

#include "../MS/MS.h"
#include "../Calculate/ascii.h"


#ifdef LINUX
u32 IP4_ASCII(u8 *hex,u8 *ascii)//��дip4 ascii,����:0=û�仯,1=�仯
{
	u32 i;
	u32 x;
	u32 f;
	
	f=0;//0=û�仯
	for(x=0;x<4;x++)
	{
		i=*hex;
		i=hex_bcd(i);
		if(i<0x10)
		{
			if(ascii[0]!=(i+0x30))
			{
				f=1;
				ascii[0]=i+0x30;
			}
			if(x<3)
			{//���ǵ�4�ֽ�
				if(ascii[1]!='.')
				{
					f=1;
					ascii[1]='.';
				}
			}
			ascii+=2;
			hex++;
			continue;
		}
		if(i<0x100)
		{
			if(ascii[0]!=((i>>4)+0x30))
			{
				f=1;
				ascii[0]=(i>>4)+0x30;
			}
			if(ascii[1]!=((i&0xf)+0x30))
			{
				f=1;
				ascii[1]=(i&0xf)+0x30;
			}
			if(x<3)
			{//���ǵ�4�ֽ�
				if(ascii[2]!='.')
				{
					f=1;
					ascii[2]='.';
				}
			}
			ascii+=3;
			hex++;
			continue;
		}
		if(ascii[0]!=((i>>8)+0x30))
		{
			f=1;
			ascii[0]=(i>>8)+0x30;
		}
		if(ascii[1]!=(((i>>4)&0xf)+0x30))
		{
			f=1;
			ascii[1]=((i>>4)&0xf)+0x30;
		}
		if(ascii[2]!=((i&0xf)+0x30))
		{
			f=1;
			ascii[2]=(i&0xf)+0x30;
		}
		if(x<3)
		{//���ǵ�4�ֽ�
			if(ascii[3]!='.')
			{
				f=1;
				ascii[3]='.';
			}
		}
		ascii+=4;
		hex++;
	}
	ascii--;
	ascii[0]=0;
	
	return f;
}
#if MainProtocol==376
u32 Local_ASCII_IP4(void)//��д����ascii ip,����:0=û�仯,1=�仯
{
	u32 i;
	u32 x;
	u32 f;
	u8 *p8s;
	u8 *p8d;
	
	f=0;//0=û�仯
//MAC ascii
 #if IC_24AAxxx==1
		#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
		MR(ADDR_DATABUFF,ADDR_ETHMAC,6);
		#else
		MR(ADDR_DATABUFF,ADDR_ETHMAC+6,6);
		#endif
	MW(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #else
	MR(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #endif
	p8s=(u8*)ADDR_DATABUFF;
	p8d=(u8*)ADDR_MACADDR_ascii;
	for(i=0;i<6;i++)
	{
		if((p8s[i]>>4)<=9)
		{
			x=(p8s[i]>>4)+0x30;
		}
		else
		{
			x=(p8s[i]>>4)+55;
		}
		if(p8d[0]!=x)
		{
			f=1;//1=�仯
			p8d[0]=x;
		}
		if((p8s[i]&0xf)<=9)
		{
			x=(p8s[i]&0xf)+0x30;
		}
		else
		{
			x=(p8s[i]&0xf)+55;
		}
		if(p8d[1]!=x)
		{
			f=1;//1=�仯
			p8d[1]=x;
		}
		p8d[2]='-';
		p8d+=3;
	}
	p8d--;
	p8d[0]=0;
//����IP4 ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4ADDR_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4�������� ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7+4,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4MASK_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4����IP ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7+8,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4GATEWAY_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4 PRIMARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4PRIMARY_DNS_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4 SECONDARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4SECONDARY_DNS_ascii);//��дip4 ascii,����:0=û�仯,1=�仯

	return f;
}
#endif
#if MainProtocol==698
u32 Local_ASCII_IP4(void)//��д����ascii ip,����:0=û�仯,1=�仯
{
	u32 i;
	u32 x;
	u32 f;
	u8 *p8s;
	u8 *p8d;
	
	f=0;//0=û�仯
//MAC ascii
 #if IC_24AAxxx==1
		#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
		MR(ADDR_DATABUFF,ADDR_ETHMAC,6);
		#else
		MR(ADDR_DATABUFF,ADDR_ETHMAC+6,6);
		#endif
 #else
	MR(ADDR_DATABUFF,ADDR_4510_5+2,6);
 #endif
	p8s=(u8*)ADDR_DATABUFF;
	p8d=(u8*)ADDR_MACADDR_ascii;
	for(i=0;i<6;i++)
	{
		if((p8s[i]>>4)<=9)
		{
			x=(p8s[i]>>4)+0x30;
		}
		else
		{
			x=(p8s[i]>>4)+55;
		}
		if(p8d[0]!=x)
		{
			f=1;//1=�仯
			p8d[0]=x;
		}
		if((p8s[i]&0xf)<=9)
		{
			x=(p8s[i]&0xf)+0x30;
		}
		else
		{
			x=(p8s[i]&0xf)+55;
		}
		if(p8d[1]!=x)
		{
			f=1;//1=�仯
			p8d[1]=x;
		}
		p8d[2]='-';
		p8d+=3;
	}
	p8d--;
	p8d[0]=0;
//����IP4 ascii
	MR(ADDR_DATABUFF,ADDR_4510_4+6,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4ADDR_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4�������� ascii
	i=(u32)Get_Element((u8*)ADDR_4510_4,3,0,LENmax_4510_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
	if(i)
	{
		MR(ADDR_DATABUFF,i+2,4);
	}
	else
	{
		MWR(255+(255<<8)+(255<<16)+(0<<24),ADDR_DATABUFF,4);
	}
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4MASK_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4����IP ascii
	i=(u32)Get_Element((u8*)ADDR_4510_4,4,0,LENmax_4510_4);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
	if(i)
	{
		MR(ADDR_DATABUFF,i+2,4);
	}
	else
	{
		MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	}
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4GATEWAY_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4 PRIMARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4PRIMARY_DNS_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4 SECONDARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4SECONDARY_DNS_ascii);//��дip4 ascii,����:0=û�仯,1=�仯

	return f;
}
#endif
#if MainProtocol==CSG
u32 Local_ASCII_IP4(void)//��д����ascii ip,����:0=û�仯,1=�仯
{
	u32 f;
/*
	u32 i;
	u32 x;
	u8 *p8s;
	u8 *p8d;
	
	f=0;//0=û�仯
//MAC ascii
 #if IC_24AAxxx==1
	Read_24AA02E48(ADDR_DATABUFF,0xFA,6);//24AAxxx��
	MW(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #else
	MR(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #endif
	p8s=(u8*)ADDR_DATABUFF;
	p8d=(u8*)ADDR_MACADDR_ascii;
	for(i=0;i<6;i++)
	{
		if((p8s[i]>>4)<=9)
		{
			x=(p8s[i]>>4)+0x30;
		}
		else
		{
			x=(p8s[i]>>4)+55;
		}
		if(p8d[0]!=x)
		{
			f=1;//1=�仯
			p8d[0]=x;
		}
		if((p8s[i]&0xf)<=9)
		{
			x=(p8s[i]&0xf)+0x30;
		}
		else
		{
			x=(p8s[i]&0xf)+55;
		}
		if(p8d[1]!=x)
		{
			f=1;//1=�仯
			p8d[1]=x;
		}
		p8d[2]='-';
		p8d+=3;
	}
	p8d--;
	p8d[0]=0;
//����IP4 ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4ADDR_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4�������� ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7+4,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4MASK_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4����IP ascii
	MR(ADDR_DATABUFF,ADDR_AFN04F7+8,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4GATEWAY_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4 PRIMARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4PRIMARY_DNS_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
//����IP4 SECONDARY_DNS ascii
	MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4SECONDARY_DNS_ascii);//��дip4 ascii,����:0=û�仯,1=�仯

*/
	return f;

}
#endif


void Ethernet_RxTx(void)
{
#if (IC_LAN8720|IC_IP101)==0
	u16 * p16timer;
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	if(p16timer[0]==0)
	{
		#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
			OffE5V0();//��E5V0��Դ
		#else
			Off2E5V0();//��E5V0��Դ
		#endif
		p16timer[0]=5000/10;//�رյ�Դ��ʱ5��
	}
#else
	u32 i;
	u32 x;
	u32 sourMACMIIDR;
	u8 *p8s;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 LEN_UARTnTx;
	u32 LEN_ADDR;
	u32 LEN_TAPDU;
	u32 LEN_TxAPDU;
	u32 LEN_Tx;
	char* p8tx;
	ms_Type * ms;
	struct sockaddr_in addr;
	u16* p16;
	u32 p0;
	
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	p16timer=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	
	if(UARTCtrl->LinkTask<101)//101://��¼ok
	{
		if(Comm_Ram->CheckLEDTimer<=(1000/10))//3000/10;//�ϵ�ȫ��LED�����ʾ ÿ10MS��1��ʱ��
		{
			Pin_OutH(PIN_ENET_LINK_LED);//���������,�������ʹ����
		}
	}
	ms=Get_ms();
	switch(UARTCtrl->LinkTask)
	{
		case 0://0=��ʱ�ػ�
			if(p16timer[0]==0x0)
			{
				UARTCtrl->LinkTask++;
			}
			return;
		case 1://1=�ص�Դ
			ms->link_close(UARTCtrl->TCPSocket);
			ms->link_close(UARTCtrl->UDPSocket);
		#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
			ms->link_system("ifconfig eth0 down");
		#else
			ms->link_system("ifconfig eth1 down");
		#endif
		#if MainProtocol==376
			if(Comm_Ram->TYMDHMS[2]==0)
		#endif
		#if MainProtocol==698
			if(Comm_Ram->DL698YMDHMS[2]==0)
		#endif
		#if MainProtocol==CSG
			if(Comm_Ram->RTCBuff[2]==0)
		#endif
			{//��0ʱ,�ص�Դ
			#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
				OffE5V0();//��E5V0��Դ
			#else
				Off2E5V0();//��E5V0��Դ
			#endif
				p16timer[0]=5000/10;//�رյ�Դ��ʱ5��
			}
			else
			{//����0ʱ,���ص�Դ
				p16timer[0]=2000/10;//��ʱ2��
			}
			UARTCtrl->LinkTask++;
			return;
		case 2://����Դ,ETHϵͳ��ʼ��
			Terminal_Ram->ENETSEL=0;//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
			if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
			{
				//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				if(UARTCtrl->PORT==0)
				{
					UARTCtrl->PORT=1;
				}
				else
				{
					return;//��ع���ʱ����1�κ�ֹͣ
				}
			}
			if(UARTCtrl->LinkCtrl!=1)//���ӿ���0=������,1=����
			{
				return;
			}
			//����Դ
			if(p16timer[0]!=0x0)
			{
				if(p16timer[0]<(2000/10))
				{
				#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
					if(Pin_Read(PIN_ENE5V0)==0)//������,�������ʹ����;����0��1	
					{
						OnE5V0();//��E5V0��Դ
						if(p16timer[0]<(1000/10))
						{
							p16timer[0]=1000/10;//����1s��ʱ
						}
					}
				#else
					if(Pin_Read(PIN_EN2E5V0)==0)//������,�������ʹ����;����0��1	
					{
						On2E5V0();//��E5V0��Դ
						if(p16timer[0]<(1000/10))
						{
							p16timer[0]=1000/10;//����1s��ʱ
						}
					}
				#endif
				}
				return;
			}
		#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
			if(Pin_Read(PIN_ENE5V0)==0)//������,�������ʹ����;����0��1	
			{
				OnE5V0();//��E5V0��Դ
				p16timer[0]=1000/10;//1s��ʱ
				return;
			}
		#else
			if(Pin_Read(PIN_EN2E5V0)==0)//������,�������ʹ����;����0��1	
			{
				On2E5V0();//��E5V0��Դ
				p16timer[0]=1000/10;//1s��ʱ
				return;
			}
		#endif
			//ETHϵͳ��ʼ��
		#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
			OnE5V0();//��E5V0��Դ
		#else
			On2E5V0();//��E5V0��Դ
		#endif
			p16timer[0]=2000/10;//�ȴ�����RJ45����2s��ʱ
			Local_ASCII_IP4();//��д����ascii ip4,����:0=û�仯,1=�仯
			UARTCtrl->DHCPTask=0;//DHCP�����
			UARTCtrl->LinkTask++;
			p8tx=(char*)ADDR_DATABUFF;
		#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
			p8tx=StrCopy(p8tx,"ifconfig eth0 ");
		#else
			p8tx=StrCopy(p8tx,"ifconfig eth1 ");
		#endif
			p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
			p8tx=StrCopy(p8tx," netmask ");
			p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
			p8tx=StrCopy(p8tx," hw ether ");
			p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
			p8tx=StrCopy(p8tx," broadcast ");
			p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
			p8tx=StrCopy(p8tx," up ");
			p8tx[0]=0;
			p8tx=(char*)ADDR_DATABUFF;
			for(i=0;p8tx[i]!=0;i++)
			{
				if(p8tx[i]=='-')
				{
					for(x=i;p8tx[x]!=0;x++)//�����µı�����ʼѭ����ֵ 
					{
						p8tx[x]=p8tx[x+1];
						i--;
					}
				}
			}
			ms->link_system((char*)ADDR_DATABUFF);
			return;
		case 3://�ȴ�����RJ45����(ÿ2����)
			if(p16timer[0]==0x0)
			{
			#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
				i=Read_ETH_PHYReg(PHYAddr_IP101,2);
				if(i!=0x0243)//IP101GR
			#else
				i=Read_ETH_PHYReg(PHYAddr_LAN8720,2);
				if(i!=7)//LAN8720
			#endif
				{
				#if (USER/100)!=0//���ǵ��Ժ
					Comm_Ram->ErrorIC|=0x80;//Ӳ������B7=LAN8720
					Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
				#endif
					//UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					return;
				}
				p16timer[0]=2000/10;//�´μ�ⶨʱ
			#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
				ms->Disable_os();//��ͣos
				while (inpw(REG_EMAC0_MIIDA) & 0x20000);//MDC��æ  wait busy flag clear
				sourMACMIIDR=inpw(REG_EMAC0_MIID);//ԭֵ����				
				i=Read_ETH_PHYReg(PHYAddr_IP101,1);
				outpw(REG_EMAC0_MIID, sourMACMIIDR);//ԭֵ�ָ�
				ms->Enable_os();//����os
			#else
				ms->Disable_os();//��ͣos
				while (inpw(REG_EMAC1_MIIDA) & 0x20000);//MDC��æ  wait busy flag clear
				sourMACMIIDR=inpw(REG_EMAC1_MIID);//ԭֵ����				
				i=Read_ETH_PHYReg(PHYAddr_LAN8720,1);
				outpw(REG_EMAC1_MIID, sourMACMIIDR);//ԭֵ�ָ�
				ms->Enable_os();//����os
			#endif
				if((i&0x24)==0x24)//bit2 link is up,bit5 auto-negotiate process completed
				{//RJ45����ͬʱ�Զ��Լ�����(Link status and Auto-negotiation complete)
					if(Terminal_Ram->ENETSEL>=4)//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
					{
						WarningCancel(ENETPORT);//�澯ȡ��,��ڸ澯����
					}
					Terminal_Ram->ENETSEL=0;//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
				#if MainProtocol==376
					i=MRR(ADDR_AFN04F8,1);
				#endif
				#if MainProtocol==698
					i=MRR(ADDR_4510_2+5,1);//���ӷ�ʽ  enum{TCP��0����UDP��1��}
					i<<=7;
				#endif
				#if MainProtocol==CSG
					//i=MRR(ADDR_AFN04F8,1);
					i=0;
				#endif
					UARTCtrl->TCPUDP=i;
					if(Comm_Ram->Factory==0x55)
					{//����ģʽʱ��¼���������ʱ���㹤����������
						p16timer[0]=0;//5000/10;//0;//��С��ʱ5��,�����´�¥����<=3���ڲ�������
					}
					else
					{
					#if MainProtocol==376
						i=MRR(ADDR_TERMINAL,1);//�ն˵�ַ�ĵ�1�ֽ���Ϊ�����
						i&=0x0f;
						i*=100;
						if(i<500)
						{
							i+=500;//��С��ʱ5��,�����´�¥����<=3���ڲ�������
						}
						p16timer[0]=i;
					#endif
					#if MainProtocol==698
						p16timer[0]=0;
					#endif
					#if MainProtocol==CSG
						p16timer[0]=0;
					#endif
					}
					UARTCtrl->LinkSubTask=0;//����������
					UARTCtrl->LinkTask++;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				}
				else
				{
					if(Terminal_Ram->ENETSEL<3)//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
					{
						Terminal_Ram->ENETSEL=3;//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
					}
					else
					{//�ڶ����ж�
						if(Terminal_Ram->ENETSEL<5)//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
						{
							Terminal_Ram->ENETSEL++;//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
						}
						WarningOccur(ENETPORT,(u8 *)"��̫������û����");//�澯����,��ڸ澯����͸澯�ַ���
					}
				}
			}
			return;
		case 4://
		case 5://
		case 6://
			UARTCtrl->LinkTask++;
			return;
		case 7://���������ʱ1-30��
			if(p16timer[0]==0x0)
			{
				UARTCtrl->LinkTask=99;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;
		case 99://����
		#if MainProtocol==376
			i=MRR(ADDR_AFN04F7+64,1);
			if(i==0x0)
			{//�̶�IP����
				if(Local_ASCII_IP4())//��д����ascii ip4,����:0=û�仯,1=�仯
				{
					UARTCtrl->LinkTask=0;
					return;
				}
			}
			else
			{//�Զ���ȡIP����

			}
		#endif
		#if MainProtocol==698
			//IP���÷�ʽenum{DHCP��0������̬��1����PPPoE��2��}
			i=MRR(ADDR_4510_4+3,1);
			if(i==1)
			{//��̬��1��
				if(Local_ASCII_IP4())//��д����ascii ip4,����:0=û�仯,1=�仯
				{
					p8tx=(char*)ADDR_DATABUFF;
				#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
					p8tx=StrCopy(p8tx,"ifconfig eth0 ");
				#else
					p8tx=StrCopy(p8tx,"ifconfig eth1 ");
				#endif
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
					p8tx=StrCopy(p8tx," netmask ");
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
					p8tx=StrCopy(p8tx," hw ether ");
					p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
					p8tx=StrCopy(p8tx," broadcast ");
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
					p8tx=StrCopy(p8tx," up ");
					p8tx[0]=0;
					p8tx=(char*)ADDR_DATABUFF;
					for(i=0;p8tx[i]!=0;i++)
					{
						if(p8tx[i]=='-')
						{
							for(x=i;p8tx[x]!=0;x++)//�����µı�����ʼѭ����ֵ 
							{
								p8tx[x]=p8tx[x+1];
								i--;
							}
						}
					}
					ms->link_system((char*)ADDR_DATABUFF);
					UARTCtrl->LinkTask=0;
					return;
				}
			}
			else
			{
				if(i==0)
				{//{DHCP��0��

				}
				else
				{//PPPoE��2��
					
					
				}
			}
		#endif
		#if MainProtocol==CSG
			i=0;//i=MRR(ADDR_AFN04F7+64,1);
			if(i==0x0)
			{//�̶�IP����
				if(Local_ASCII_IP4())//��д����ascii ip4,����:0=û�仯,1=�仯
				{
					UARTCtrl->LinkTask=0;
					return;
				}
			}
			else
			{//�Զ���ȡIP����

			}
		#endif
			if (p16timer[0]!=0x0)
			{
				return;
			}
			i=UARTCtrl->LinkSubTask;
			switch(i)//����������
			{
				case 0://��������
					UARTCtrl->Task=0;//0=���ڽ���
					UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
					UARTCtrl->RxByte=0;//�����ֽ���=0
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
					UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//���������붨ʱ��
					UARTCtrl->LinkTask2=0;//ͨ��2��������(FTP����);0-99=����,100=��������
					UARTCtrl->Task2=0;//ͨ��2����(FTP����)
				#if MainProtocol==698
					UARTCtrl->ReNewLink=0;//0x55=ͨ�Ų����仯��ǰ֡������ɺ���������
				#endif
				//ifconfig
//					p8tx=(char*)ADDR_DATABUFF;
//				#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
//					p8tx=StrCopy(p8tx,"ifconfig eth0 ");
//				#else
//					p8tx=StrCopy(p8tx,"ifconfig eth1 ");
//				#endif
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
//					p8tx=StrCopy(p8tx," netmask ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
//					p8tx=StrCopy(p8tx," hw ether ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
//					p8tx=StrCopy(p8tx," up ");
//					p8tx[0]=0;
//					ms->link_system((char*)ADDR_DATABUFF);
//				//gw
//					p8tx=(char*)ADDR_DATABUFF;
//					p8tx=StrCopy(p8tx,"route add default gw ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
//					p8tx[0]=0;
//					ms->link_system((char*)ADDR_DATABUFF);
//					p8tx=(char*)ADDR_DATABUFF;
//				#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
//					p8tx=StrCopy(p8tx,"ifconfig eth0 ");
//				#else
//					p8tx=StrCopy(p8tx,"ifconfig eth1 ");
//				#endif
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
//					p8tx=StrCopy(p8tx," netmask ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
//					p8tx=StrCopy(p8tx," hw ether ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
//					p8tx=StrCopy(p8tx," broadcast ");
//					p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
//					p8tx=StrCopy(p8tx," up ");
//					p8tx[0]=0;
//					p8tx=(char*)ADDR_DATABUFF;
//					for(i=0;p8tx[i]!=0;i++)
//					{
//						if(p8tx[i]=='-')
//						{
//							for(x=i;p8tx[x]!=0;x++)//�����µı�����ʼѭ����ֵ 
//							{
//								p8tx[x]=p8tx[x+1];
//								i--;
//							}
//						}
//					}
//					ms->link_system((char*)ADDR_DATABUFF);
					//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
					i=UARTCtrl->PORT;
					i++;
					if((i<SocketPortnMin)||(i>SocketPortnMax))
					{
						i=SocketPortnMin;
					}
					UARTCtrl->PORT=i;
					//��վIP
					if((UARTCtrl->TCPUDP&0x80)==0x0)
					{//TCP
						UARTCtrl->TCPSocket=ms->link_socket(AF_INET,SOCK_STREAM,0);//TCP=SOCK_STREAM ,UDP=SOCK_DGRAM
						addr.sin_family = AF_INET;// ʹ�� IPv4 Э��
						i=UARTCtrl->PORT;
						i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
						addr.sin_port = i;//�˿ں�:����
						i=IP_ASCII_HEX(ADDR_IP4ADDR_ascii);//ASCIIתΪHEX��IP;��ڿ�Ϊָ���ASCII����0-9�ĵ�ַ;����0��ʾ��IP��ַ
						addr.sin_addr.s_addr = i;//IP��ַ��ע:�ֽ�˳�� "192.168.0.1" = 0x0100A8C0
						i=ms->link_bind(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
						ms->link_printf("1-%d\n",i,0,0);
						if(i!=0)
						{//���óɹ�ʱ����0��ʧ��ʱ����-1
							UARTCtrl->LinkTask=0;
							return;
						}
					#if MainProtocol==376
						if(UARTCtrl->LinkCount<2)
						{//��IP
							MR((u32)&addr.sin_addr.s_addr,ADDR_AFN04F3,4);
							i=MRR(ADDR_AFN04F3+4,2);
							i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							addr.sin_port = i;//�˿ں�:����
						}
						else
						{//����IP
							MR((u32)&addr.sin_addr.s_addr,ADDR_AFN04F3+6,4);
							i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
							addr.sin_port = i;//�˿ں�:����
						}
					#endif
					#if MainProtocol==698
						i=MRR(ADDR_4510_3+1,1);
						if((UARTCtrl->LinkCount<2)||(i<2))
						{//��IP
							i=(u32)Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
							ms->link_printf("3-%d\n",i,0,0);
							if(i)
							{
								MR((u32)&addr.sin_addr.s_addr,i+2,4);
							}
							else
							{
								MWR(192+(168<<8)+(1<<16)+(3<<24),(u32)&addr.sin_addr.s_addr,4);
							}
							i=(u32)Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
							ms->link_printf("4-%d\n",i,0,0);
							if(i)
							{
								i=MRR(i+1,2);
								addr.sin_port = i;//�˿ں�:����
							}
							else
							{
								i=1024;
								i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
								addr.sin_port = i;//�˿ں�:����
							}
						}
						else
						{//����IP
							i=(u32)Get_Element((u8*)ADDR_4510_3,2,1,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
							if(i)
							{
								MR((u32)&addr.sin_addr.s_addr,i+2,4);
							}
							else
							{
								MWR(192+(168<<8)+(1<<16)+(3<<24),(u32)&addr.sin_addr.s_addr,4);
							}
							i=(u32)Get_Element((u8*)ADDR_4510_3,2,2,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
							if(i)
							{
								i=MRR(i+1,2);
								addr.sin_port = i;//�˿ں�:����
							}
							else
							{
								i=1024;
								i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
								addr.sin_port = i;//�˿ں�:����
							}
						}
					#endif
					#if MainProtocol==CSG
						
					/*
						if(UARTCtrl->LinkCount<2)
						{//��IP
							MR((u32)&addr->addr[0],ADDR_AFN04F3,4);
							MR((u32)&addr->port,ADDR_AFN04F3+4,2);
						}
						else
						{//����IP
							MR((u32)&addr->addr[0],ADDR_AFN04F3+6,4);
							MR((u32)&addr->port,ADDR_AFN04F3+6+4,2);
						}
					*/
					#endif
						ms->link_printf("5-%d\n",i,0,0);
						i=ms->link_connect(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
						ms->link_printf("2-%d\n",i,0,0);
						if(i!=0)
						{//���óɹ�ʱ����0��ʧ��ʱ����-1
							UARTCtrl->LinkCount++;//���Ӽ���+1
							if(UARTCtrl->LinkCount>=3)
							{
								UARTCtrl->LinkCount=0;
							}
							UARTCtrl->LinkTask=0;
							return;
						}
						WarningCancel(ENETPORT);//�澯ȡ��,��ڸ澯����
						p16timer[0]=0;//���ӳ�ʱ��ʱ;Net_Config_TCP.h�����Զ���#define TCP_MAX_RETRY 5,#define TCP_RETRY_TOUT 4;5*4=20��
						UARTCtrl->LinkSubTask=0;//����������
						UARTCtrl->LinkTask=100;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�

					}
					else
					{//UDP
						UARTCtrl->UDPSocket=ms->link_socket(AF_INET,SOCK_DGRAM,0);//TCP=SOCK_STREAM ,UDP=SOCK_DGRAM
						addr.sin_family = AF_INET;// ʹ�� IPv4 Э��
						i=UARTCtrl->PORT;
						i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
						addr.sin_port = i;//�˿ں�:����
						i=IP_ASCII_HEX(ADDR_IP4ADDR_ascii);//ASCIIתΪHEX��IP;��ڿ�Ϊָ���ASCII����0-9�ĵ�ַ;����0��ʾ��IP��ַ
						addr.sin_addr.s_addr = i;//IP��ַ��ע:�ֽ�˳�� "192.168.0.1" = 0x0100A8C0
						i=ms->link_bind(UARTCtrl->TCPSocket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
						if(i!=0)
						{//���óɹ�ʱ����0��ʧ��ʱ����-1
							UARTCtrl->LinkCount++;//���Ӽ���+1
							if(UARTCtrl->LinkCount>=3)
							{
								UARTCtrl->LinkCount=0;
							}
							UARTCtrl->LinkTask=0;
							return;
						}
						WarningCancel(ENETPORT);//�澯ȡ��,��ڸ澯����
						p16timer[0]=0;//���ӳ�ʱ��ʱ;Net_Config_TCP.h�����Զ���#define TCP_MAX_RETRY 5,#define TCP_RETRY_TOUT 4;5*4=20��
						UARTCtrl->LinkSubTask=0;//����������
						UARTCtrl->LinkTask=100;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					}
					return;
				default:
					UARTCtrl->LinkSubTask=0;//����������
					return;
			}
			//return;
		case 100://����ok
			ms->link_printf("3-0\n",0,0,0);
			break;
		case 101://��¼ok
			Pin_OutL(PIN_ENET_LINK_LED);//���������,�������ʹ����
			break;
		case 102://���˳���¼
			break;
		case 103://�˳���¼
			if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
			{//�����е������µ�¼,���������򱣳�
				UARTCtrl->LinkTask=3;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;
		case 104://�ػ�
			p16timer[0]=2000/10;//�ػ���ʱ2s(��GPRSģ����ͬʱ��)
			return;
		default:
			UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			return;
	}

//����ok,��¼ok
	ms->link_printf("3-1\n",0,0,0);
	if(Terminal_Ram->ENETCheck_S_Timer==0)//4 ��̫��IP��ͻ�������޸����붨ʱ��
	{
		ms->link_printf("3-2\n",0,0,0);
		//��ʵ����ͣos������os��24us
		Terminal_Ram->ENETCheck_S_Timer=2;
	#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
		Disable_os();//��ͣos
		while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
		{//MDC��æ
		}
		sourMACMIIDR=inpw(REG_EMAC0_MIID);//ԭֵ����
		i=Read_ETH_PHYReg(PHYAddr_IP101,1);
		outpw(REG_EMAC0_MIID, sourMACMIIDR);//ԭֵ�ָ�
		Enable_os();//����os
	#else
		Disable_os();//��ͣos
		while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
		{//MDC��æ
		}
		sourMACMIIDR=inpw(REG_EMAC1_MIID);//ԭֵ����
		i=Read_ETH_PHYReg(PHYAddr_LAN8720,1);
		outpw(REG_EMAC1_MIID, sourMACMIIDR);//ԭֵ�ָ�
		Enable_os();//����os
	#endif
		if((i&(1<<2))==0)//bit2 link is up,bit5 auto-negotiate process completed
		{//RJ45����
			UARTCtrl->LinkTask=2;//�ȴ�����RJ45����(ÿ2����)
			return;
		}
	#if MainProtocol==376
		i=MRR(ADDR_AFN04F7+64,1);
		if(i==0x0)
		{//�̶�IP����
			if(Local_ASCII_IP4())//��д����ascii ip4,����:0=û�仯,1=�仯
			{
				UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				return;
			}
		}
	#endif
	#if MainProtocol==698
		//IP���÷�ʽenum{DHCP��0������̬��1����PPPoE��2��}
		i=MRR(ADDR_4510_4+3,1);
		if(i==1)
		{//��̬��1��
			if(Local_ASCII_IP4())//��д����ascii ip4,����:0=û�仯,1=�仯
			{
				ms->link_printf("3-3\n",0,0,0);
				p8tx=(char*)ADDR_DATABUFF;
				#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
					p8tx=StrCopy(p8tx,"ifconfig eth0 ");
				#else
					p8tx=StrCopy(p8tx,"ifconfig eth1 ");
				#endif
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4ADDR_ascii);
					p8tx=StrCopy(p8tx," netmask ");
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4MASK_ascii);
					p8tx=StrCopy(p8tx," hw ether ");
					p8tx=StrCopy(p8tx,(char*)ADDR_MACADDR_ascii);
					p8tx=StrCopy(p8tx," broadcast ");
					p8tx=StrCopy(p8tx,(char*)ADDR_IP4GATEWAY_ascii);
					p8tx=StrCopy(p8tx," up ");
					p8tx[0]=0;
					p8tx=(char*)ADDR_DATABUFF;
					for(i=0;p8tx[i]!=0;i++)
					{
						if(p8tx[i]=='-')
						{
							for(x=i;p8tx[x]!=0;x++)//�����µı�����ʼѭ����ֵ 
							{
								p8tx[x]=p8tx[x+1];
								i--;
							}
						}
					}
					ms->link_system((char*)ADDR_DATABUFF);
				UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				return;
			}
		}	
	#endif
	#if MainProtocol==CSG
		i=0;//i=MRR(ADDR_AFN04F7+64,1);
		if(i==0x0)
		{//�̶�IP����
			if(Local_ASCII_IP4())//��д����ascii ip4,����:0=û�仯,1=�仯
			{
				UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				return;
			}
		}
	#endif
	}

	switch(UARTCtrl->Task)
	{
		case 0://����
			p16=(u16*)Get_ADDR_UARTn(ENETPORT);//�õ�UART�����׵�ַ
			LEN_UARTnTx=Get_LEN_UARTnIntRx(ENETPORT);//�õ�UART�жϽ��ջ��峤��
			LEN_UARTnTx-=10;
			p0=p16[0];
			if(p0>=LEN_UARTnTx)
			{
				p0=0;
			}
			x=LEN_UARTnTx-p0;//�ɽ��ճ���
			if((UARTCtrl->TCPUDP&0x80)==0x0)
			{//TCP
				i=ms->link_recv(UARTCtrl->TCPSocket,(void *)((u32)p16+10+p0),x,MSG_DONTWAIT);
			}
			else
			{//UDP
				i=ms->link_recv(UARTCtrl->UDPSocket,(void *)((u32)p16+10+p0),x,MSG_DONTWAIT);
			}
			if(i<=LEN_UARTnTx)
			{
				p0+=i;
				if(p0>=LEN_UARTnTx)
				{
					p0=0;
				}
				p16[0]=p0;
			}
			
			//���߿���
			if(UARTCtrl->LinkCtrl==0)//���ӿ���0=������,1=����
			{
				if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					UARTCtrl->LinkTask=102;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//2 ��̫�������붨ʱ��
					return;
				}
			}
			break;
		case 2://�ȴ���֡���ݷ���
		#if MainProtocol==376
		case 5://�ȴ���֡���ݷ���
			#if (USER/100)==11//����ϵ
				USER11xx_TxAddr(ENETPORT);//����ϵ����֡�ն˵�ַ2�ֽڳ��ȱ�Ϊ4�ֽ�,�����¼���֡У���,�跢�͵�֡�ֽ���+2
			#endif
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
			UARTCtrl->Task++;
		#endif
		#if MainProtocol==698
			LEN_UARTnTx=Get_LEN_UARTnTx(ENETPORT);
			p8tx=(char *)Get_ADDR_UARTnTx(ENETPORT);
			LEN_ADDR=p8tx[4];//ȡ��ַ�ֽ���
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[1]+(p8tx[2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
			LEN_TAPDU-=3+LEN_ADDR+2+2;
			if(LEN_TAPDU<=LENmax_TxAPDU)//��·���֡���ͣ�ÿ֡���APDU�ֽ���
			{
			#if LOG_698==1//0=û,1=��
				LOG(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
			#if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
				LOGRS232(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
			#endif
				UARTCtrl->TxCount=0;
				p16timer[0]=30000/10;//30��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
				UARTCtrl->Task++;
			}
			else
			{
				p8tx=(char *)Get_ADDR_UARTnTx(ENETPORT);
				MW((u32)p8tx,(u32)p8tx+LEN_UARTnTx,UARTCtrl->TxByte);
				UARTCtrl->TxAPDUSEQ=0;//��·���֡��ż���0-4095
				UARTCtrl->TxAPDUByte=0;//��·���֡�ѷ���APDU�ֽ���
				UARTCtrl->Task=5;//5=�ȴ���·���֡���ݷ���
			}
		#endif
			break;
	#if MainProtocol==698
		case 5://�ȴ���֡���ݷ���
			LEN_UARTnTx=Get_LEN_UARTnTx(ENETPORT);
			p8tx=(char *)Get_ADDR_UARTnTx(ENETPORT);
			LEN_ADDR=p8tx[4];//ȡ��ַ�ֽ���
			LEN_ADDR&=0x0f;
			LEN_ADDR++;
			LEN_ADDR+=2;
			LEN_TAPDU=p8tx[LEN_UARTnTx+1]+(p8tx[LEN_UARTnTx+2]<<8);//����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���
			LEN_TAPDU-=3+LEN_ADDR+2+2;//������L�Ϳ�����C=3byte,֡ͷУ��=2byte,֡У��=2byte
			MR((u32)p8tx,(u32)p8tx+LEN_UARTnTx,4+LEN_ADDR+2);
			p8tx[3]|=(1<<5);//��֡��־λ
			LEN_Tx=4+LEN_ADDR+2;
			i=UARTCtrl->TxAPDUByte;
			if(i>=LEN_TAPDU)
			{
				UARTCtrl->RxByte=0;//rx����=0
				UARTCtrl->Task=0;//0=����
				return;
			}
			LEN_TxAPDU=LEN_TAPDU-i;
		//��֡��ʽ����
		//bit0~bit11����ʾ��֡������̵�֡��ţ�ȡֵ��Χ0~4095��ѭ��ʹ�ã�
		//bit12~bit13��������
		//bit15=0��bit14=0����ʾ��֡����������ʼ֡��
		//bit15=1��bit14=0����ʾ��֡����ȷ��֡��ȷ��֡������APDUƬ���򣩣�
		//bit15=0��bit14=1����ʾ��֡�������֡��
		//bit15=1��bit14=1����ʾ��֡�����м�֡��
			x=UARTCtrl->TxAPDUSEQ;//��·���֡��ż���0-4095
			UARTCtrl->TxAPDUSEQ++;//��·���֡��ż���0-4095
			x&=0x7ff;
			if(UARTCtrl->TxAPDUByte==0)
			{//��1֡
				LEN_TxAPDU=LENmax_TxAPDU;
			}
			else
			{
				if(LEN_TxAPDU>LENmax_TxAPDU)//��·���֡���ͣ�ÿ֡���APDU�ֽ���
				{//�м�֡
					x|=0xC000;
					LEN_TxAPDU=LENmax_TxAPDU;
				}
				else
				{//���֡
					x|=0x4000;
				}
			}
			p8tx[LEN_Tx]=x;
			p8tx[LEN_Tx+1]=x>>8;
			LEN_Tx+=2;
			MR((u32)p8tx+LEN_Tx,(u32)p8tx+LEN_UARTnTx+4+LEN_ADDR+2+UARTCtrl->TxAPDUByte,LEN_TxAPDU);
			UARTCtrl->TxAPDUByte+=LEN_TxAPDU;
			LEN_Tx+=LEN_TxAPDU;
			LEN_Tx+=2+1;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
			Uart_698_HCSFCS((u8*)p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		 #if LOG_68==1//0=û,1=��
			LOG(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		 #endif
		 #if DEBUG_698toRS232==1//0=��,1=��;����:ENET���շ����ݴ�ӡ��RS232��
			LOGRS232(698,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		 #endif
			UARTCtrl->TxCount=0;
			p16timer[0]=30000/10;//30��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
			UARTCtrl->Task++;
			break;
	#endif
		case 3://���͵�֡�������ڷ���
		case 6://��֡�������ڷ���
			if(UARTCtrl->TxByte)
			{
				if((UARTCtrl->TCPUDP&0x80)==0x0)
				{//TCP
					x=2048;
					if(x>UARTCtrl->TxByte)
					{
						x=UARTCtrl->TxByte;
					}
					p8s=(u8*)Get_ADDR_UARTnTx(ENETPORT);
					p8s+=UARTCtrl->TxCount;
					i=ms->link_send(UARTCtrl->TCPSocket, (void *)p8s,x, MSG_DONTWAIT);
					if(i==x)
					{
						UARTCtrl->TxByte-=x;
						UARTCtrl->TxCount+=x;
						Terminal_Ram->ENETLED_MS_Timer=50/10;//14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
					}
					else
					{
						if(p16timer[0]==0)//60��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
						{
							UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
						}
					}
				}
				else
				{//UDP
					x=1440;
					if(x>UARTCtrl->TxByte)
					{
						x=UARTCtrl->TxByte;
					}
					p8s=(u8*)Get_ADDR_UARTnTx(ENETPORT);
					p8s+=UARTCtrl->TxCount;
					UARTCtrl->TxByte-=x;
					UARTCtrl->TxCount+=x;
				#if MainProtocol==376
					MR((u32)&addr.sin_addr.s_addr,ADDR_AFN04F3,4);
					i=MRR(ADDR_AFN04F3+4,2);
					i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
					addr.sin_port = i;//�˿ں�:����	
				#endif
				#if MainProtocol==698
					i=(u32)Get_Element((u8*)ADDR_4510_3,1,1,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						MR((u32)&addr.sin_addr.s_addr,i+2,4);
					}
					else
					{
						MWR(192+(168<<8)+(1<<16)+(3<<24),(u32)&addr.sin_addr.s_addr,4);
					}
					i=(u32)Get_Element((u8*)ADDR_4510_3,1,2,LENmax_4510_3);//����Ԫ�ص�ַ,ʹ��ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);
						addr.sin_port = i;//�˿ں�:����	
					}
					else
					{
						i=1024;
						i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
						addr.sin_port = i;//�˿ں�:����	
					}
				#endif
				#if MainProtocol==CSG
				/*
					MR((u32)&addr->addr[0],ADDR_AFN04F3,4);
					MR((u32)&addr->port,ADDR_AFN04F3+4,2);
				*/
				#endif
					ms->link_sendto(UARTCtrl->UDPSocket, (void *)p8s,x, MSG_DONTWAIT,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
					Terminal_Ram->ENETLED_MS_Timer=50/10;//14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
				}
			}
			else
			{
				if(UARTCtrl->Task==3)
				{
					UARTCtrl->Task=7;//7=�ȴ���������תΪ����
				}
				else
				{
				#if MainProtocol==376
					UARTCtrl->Task=4;//4=��֡�������������
				#else
					UARTCtrl->Task=7;//7=�ȴ���������תΪ����
				#endif
				}
			}
			break;
	}
#endif//#if (IC_LAN8720|IC_IP101)==0
}


#endif













