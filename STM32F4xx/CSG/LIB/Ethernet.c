
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
//#include "../DL698/DL698_DataDef.h"
//#include "../DL698/DL698_SETLIB.h"
#endif
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"

#include "../LIB/Ethernet.h"
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/LAN8720A.h"
#include "../Device/24AA02E48.h"
#include "../Device/MEMRW.h"
#include "../Display/Display.h"
#include "../Display/Warning.h"
#include "../OS/OS.h"
#include "../Calculate/Calculate.h"
#include "../TEST/LOG.h"

#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "Terminal_Task.h"

void ETH_Control(void);


int32_t tcp_cb_client(int32_t socket, netTCP_Event event, NET_ADDR *addr,uint8_t *buf, uint32_t len)
{
	u32 i;
	u32 x;
	u32 p0;
	u32 Max_LEN_BUFF;
	u8 *p8;
	u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
	
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	
  switch (event) 
	{
    case netTCP_EventConnect:
      // Connect request received
			i=i;
      break;
 
    case netTCP_EventEstablished:
      // Connection established
			p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
			p16[0]=0;
			UARTCtrl->LinkSubTask=2;//����������
      break;
 
    case netTCP_EventClosed:
      // Connection was properly closed
			p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
			p16[0]=0;
			//UARTCtrl->LinkTask=3;//ybwhz
			//UARTCtrl->LinkSubTask=0;
			UARTCtrl->LinkTask=99;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			UARTCtrl->LinkSubTask=3;//����������
      break;
 
    case netTCP_EventAborted:
      // Connection is for some reason aborted
			p16=(u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
			p16[0]=0;
			UARTCtrl->LinkSubTask=4;//����������
      break;
 
    case netTCP_EventACK:
      // Previously sent data acknowledged
      break;
 
    case netTCP_EventData:
      // Data received
			p16=(u16*)Get_ADDR_UARTn(ENETPORT);//�õ�UART�����׵�ַ
			Max_LEN_BUFF=Get_LEN_UARTnIntRx(ENETPORT);//�õ�UART�жϽ��ջ��峤��
			Max_LEN_BUFF-=10;
			p0=p16[0];
			if(p0>=Max_LEN_BUFF)
			{
				p0=0;
			}
			p8=(u8*)((u32)p16)+10;
			while(len)
			{
				x=Max_LEN_BUFF-p0;//��ǰ���嵽�����������ֽ���
				if(x>len)
				{
					x=len;
				}
				for(i=0;i<x;i++)
				{
					p8[p0]=buf[i];
					p0++;
				}
				buf+=x;
				len-=x;
				if(p0>=Max_LEN_BUFF)
				{
					p0=0;
				}
			}
			p16[0]=p0;
      break;
  }
  return (0);
}
uint32_t udp_callback(int32_t socket, const NET_ADDR *addr, const uint8_t *buf, uint32_t len)
{
  // This function is called when UDP data is received 
  // Process received data from 'buf' 
	u32 i;
	u32 x;
	u32 p0;
	u32 Max_LEN_BUFF;
	u8 *p8;
	u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(ENETPORT));
	if(UARTCtrl->LinkSubTask==6)//����������
	{
		//case 6://ETH�������ȴ�����
		for(i=0;i<16;i++)
		{
			if(buf[i]!=i)
			{
				break;
			}
		}
		if(i>=16)
		{
			UARTCtrl->LinkSubTask++;//����OK
		}
		return (0);
	}
	p16=(u16*)Get_ADDR_UARTn(ENETPORT);//�õ�UART�����׵�ַ
	Max_LEN_BUFF=Get_LEN_UARTnIntRx(ENETPORT);//�õ�UART�жϽ��ջ��峤��
	Max_LEN_BUFF-=10;
	p0=p16[0];
	if(p0>=Max_LEN_BUFF)
	{
		p0=0;
	}
	p8=(u8*)((u32)p16)+10;
	while(len)
	{
		x=Max_LEN_BUFF-p0;//��ǰ���嵽�����������ֽ���
		if(x>len)
		{
			x=len;
		}
		for(i=0;i<x;i++)
		{
			p8[p0]=buf[i];
			p0++;
		}
		buf+=x;
		len-=x;
		if(p0>=Max_LEN_BUFF)
		{
			p0=0;
		}
	}
	p16[0]=p0;
  return (0);
}
uint32_t tcp_cb_server (int32_t socket, netTCP_Event event,const NET_ADDR *addr, const uint8_t *buf, uint32_t len)
{   
	switch (event)
	{    
		case netTCP_EventConnect:
			// Connect request received 
			// Example
//			if (addr->addr_type == NET_ADDR_IP4)
//			{ // IPv4 client
//				if (addr->addr[0] == 192  &&
//						addr->addr[1] == 168  &&
//						addr->addr[2] == 0    &&
//						addr->addr[3] == 1)
//				{// Accept connection from client at 192.168.0.1 
//					return (1);
//				}
//			}
//			else
//			{// IPv6 client
//				const uint8_t ip6_addr[NET_ADDR_IP6_LEN] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//																										0x1c, 0x30, 0x6c, 0xff, 0xfe, 0xa2, 0x45, 0x5e };
//				if (memcmp (addr->addr, ip6_addr, NET_ADDR_IP6_LEN) == 0)
//				{// Accept connection from client at [fe80::1c30:6cff:fea2:455e]
//					return (1);
//				}
//			}
//			// Deny connection.
//			return (0);

			return (1);
		case netTCP_EventEstablished:
			// Connection established
			break;
		case netTCP_EventClosed:
			// Connection was properly closed
			break;
		case netTCP_EventAborted:
			// Connection is for some reason aborted
			break;
		case netTCP_EventACK:
			// Previously sent data acknowledged
			break;
		case netTCP_EventData:
			// Data received

			break;
	} 
	return (0);
}


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
#if MainProtocol==CSG
u32 Local_ASCII_IP4(void)//��д����ascii ip,����:0=û�仯,1=�仯
{
	u32 f, i, x;
	u8 TempIpAddr[4];
	
	u8 *p8s;
	u8 *p8d;
	CommunicationParaStruct CommPara;
	MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
	MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));

	f=0;		// 0=û�仯
	//MAC ascii
 #if IC_24AAxxx==1
	Read_24AA02E48(ADDR_DATABUFF,0xFA,6);//24AAxxx��
	//MW(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #else
	MR(ADDR_DATABUFF,ADDR_TERMINAL+10,6);
 #endif
	p8s=(u8*)ADDR_DATABUFF;
	p8d=(u8*)ADDR_MACADDR_ascii;
	for(i = 0; i < 6; i++)
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
			f=1;// 1=�仯
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
			f=1;// 1=�仯
			p8d[1]=x;
		}
		p8d[2]='-';
		p8d+=3;
	}
	p8d--;
	p8d[0]=0;
	//����IP4 ascii
	//MR(ADDR_DATABUFF,ADDR_AFN04F7,4);
	//f |= IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4ADDR_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
	TempIpAddr[3] = CommPara.IpAddr;
	TempIpAddr[2] = CommPara.IpAddr >> 8;
	TempIpAddr[1] = CommPara.IpAddr >> 16;
	TempIpAddr[0] = CommPara.IpAddr >> 24;
	f |= IP4_ASCII(TempIpAddr, (u8 *)ADDR_IP4ADDR_ascii);
	
	//����IP4�������� ascii
	//MR(ADDR_DATABUFF,ADDR_AFN04F7+4,4);
	//f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4MASK_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
	TempIpAddr[3] = CommPara.IpSubNetMask;
	TempIpAddr[2] = CommPara.IpSubNetMask >> 8;
	TempIpAddr[1] = CommPara.IpSubNetMask >> 16;
	TempIpAddr[0] = CommPara.IpSubNetMask >> 24;
	f |= IP4_ASCII(TempIpAddr, (u8 *)ADDR_IP4MASK_ascii);
	
	//����IP4����IP ascii
	//MR(ADDR_DATABUFF,ADDR_AFN04F7+8,4);
	//f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4GATEWAY_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
	TempIpAddr[3] = CommPara.IpGateWayAddr;
	TempIpAddr[2] = CommPara.IpGateWayAddr >> 8;
	TempIpAddr[1] = CommPara.IpGateWayAddr >> 16;
	TempIpAddr[0] = CommPara.IpGateWayAddr >> 24;
	f |= IP4_ASCII(TempIpAddr, (u8 *)ADDR_IP4GATEWAY_ascii);
	
	//����IP4 PRIMARY_DNS ascii
	//MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	//f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4PRIMARY_DNS_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
	TempIpAddr[3] = (u8)DEFAULT_IP_DNS;
	TempIpAddr[2] = (u8)(DEFAULT_IP_DNS >> 8);
	TempIpAddr[1] = (u8)(DEFAULT_IP_DNS >> 16);
	TempIpAddr[0] = (u8)(DEFAULT_IP_DNS >> 24);
	f |= IP4_ASCII(TempIpAddr,(u8 *)ADDR_IP4PRIMARY_DNS_ascii);
	
	//����IP4 SECONDARY_DNS ascii
	//MWR(192+(168<<8)+(3<<16)+(1<<24),ADDR_DATABUFF,4);
	//f|=IP4_ASCII((u8 *)ADDR_DATABUFF,(u8 *)ADDR_IP4SECONDARY_DNS_ascii);//��дip4 ascii,����:0=û�仯,1=�仯
	TempIpAddr[3] = (u8)DEFAULT_IP_DNS;
	TempIpAddr[2] = (u8)(DEFAULT_IP_DNS >> 8);
	TempIpAddr[1] = (u8)(DEFAULT_IP_DNS >> 16);
	TempIpAddr[0] = (u8)(DEFAULT_IP_DNS >> 24);
	f |= IP4_ASCII(TempIpAddr,(u8 *)ADDR_IP4SECONDARY_DNS_ascii);

	return f;

}
#endif


void Ethernet_RxTx(void)
{
	u32 i, x, sourMACMIIDR;
	u8 *p8s;
	u8 *p8d;
	u16 * p16timer;
	os_Type *os;
	NET_ADDR *addr;
  	UARTCtrl_TypeDef * UARTCtrl;
	netStatus ns;
	CommunicationParaStruct CommPara;
	
  	UARTCtrl = (UARTCtrl_TypeDef*)(Get_ADDR_UARTnCtrl(ENETPORT));
	p16timer = (u16 *)(Get_ADDR_UARTnTimer(ENETPORT));
	
	if(UARTCtrl->LinkTask < 101)					// 101://��¼ok
	{
		if(Comm_Ram->CheckLEDTimer <= (1000/10))	// 3000/10;//�ϵ�ȫ��LED�����ʾ ÿ10MS��1��ʱ��
		{
			Pin_OutH(PIN_ENET_LINK_LED);		//���������,�������ʹ����
		}
	}
	
	
	switch(UARTCtrl->LinkTask)
	{
		case 0:									// 0=��ʱ�ػ�
			if(p16timer[0] == 0x0)
			{
				UARTCtrl->LinkTask++;
			}
			return;
		case 1:									// 1=�ص�Դ
			UnInit_STM32F4xx_MAC();
			if(Comm_Ram->RTCBuff[2] == 0)
			{									// ��0ʱ,�ص�Դ
				OffE5V0();						// ��E5V0��Դ
				p16timer[0]=5000/10;			// �رյ�Դ��ʱ5��
			}
			else
			{									// ����0ʱ,���ص�Դ
				p16timer[0]=2000/10;			// ��ʱ2��
			}
			UARTCtrl->LinkTask++;
			return;
		case 2:										// ����Դ,ETHϵͳ��ʼ��
			if(Comm_Ram->After_Init_OS & (1<<4))	// OS������ʹ�ñ�־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
			{
				Comm_Ram->Init_OS |= 1<<4;			// OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
				return;
			}
			Terminal_Ram->ENETSEL = 0;				// ��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
			if(Comm_Ram->POWER < 3)					// ��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
			{
				//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
				if(UARTCtrl->PORT == 0)
				{
					UARTCtrl->PORT = 1;
				}
				else
				{
					return;							// ��ع���ʱ����1�κ�ֹͣ
				}
			}
			//����Դ
			if(p16timer[0] != 0x0)
			{
				if(p16timer[0] < (2000/10))
				{
					if(Pin_Read(PIN_ENE5V0)==0)		// ������,�������ʹ����;����0��1	
					{
						OnE5V0();					// ��E5V0��Դ
						if(p16timer[0]<(1000/10))
						{
							p16timer[0]=1000/10;	// ����1s��ʱ
						}
					}
				}
				return;
			}
			if(Pin_Read(PIN_ENE5V0)==0)				// ������,�������ʹ����;����0��1	
			{
				OnE5V0();							// ��E5V0��Դ
				p16timer[0]=1000/10;				// 1s��ʱ
				return;
			}
			//ETHϵͳ��ʼ��
			Init_STM32F4xx_MAC();
			Init_LAN8720A();
			p16timer[0] = 2000/10;					// �ȴ�����RJ45����2s��ʱ
			i = Read_ETH_PHYReg(PHYAddr_LAN8720A_0,2);
			if(i != 7)
			{
				Comm_Ram->ErrorIC|=0x80;			// Ӳ������B7=LAN8720
				Comm_Ram->DisplayAllErrorTimer=10;	// �ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
				UARTCtrl->LinkTask=0;				// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				return;
			}
			Local_ASCII_IP4();						// ��д����ascii ip4,����:0=û�仯,1=�仯
			
			os = Get_os();
			os->netInitialize();					//ÿ������OS��ֻ�ܵ��ó�ʼ��1��,��������
			Comm_Ram->After_Init_OS|=(1<<4);		//OS������ʹ�ñ�־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
			UARTCtrl->DHCPTask=0;					//DHCP�����
			UARTCtrl->TCPSocket = os->netTCP_GetSocket((netTCP_cb_t)tcp_cb_client);
			if(UARTCtrl->TCPSocket<0)
			{
				Comm_Ram->Init_OS|=1<<4;			// OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
				return;
			}
			UARTCtrl->UDPSocket=os->netUDP_GetSocket((netUDP_cb_t)udp_callback);
			if(UARTCtrl->UDPSocket<0)
			{
				Comm_Ram->Init_OS|=1<<4;			//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
				return;
			}
			UARTCtrl->LinkTask++;
			return;
		case 3://�ȴ�����RJ45����(ÿ2����)
			if(p16timer[0]==0x0)
			{
				p16timer[0]=2000/10;//�´μ�ⶨʱ
				Disable_os();//��ͣos
				while(ETH->MACMIIAR & 1)
				{//MDC��æ
				}
				sourMACMIIDR=ETH->MACMIIDR;//ԭֵ����
				i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,1);
				ETH->MACMIIDR=sourMACMIIDR;//ԭֵ�ָ�
				Enable_os();//����os
				if(i & (1<<2))//bit2 link is up,bit5 auto-negotiate process completed
				{//RJ45����
					if(Terminal_Ram->ENETSEL>=4)//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
					{
						WarningCancel(ENETPORT);//�澯ȡ��,��ڸ澯����
					}
					Terminal_Ram->ENETSEL=0;//��̫��ģ��ѡ��0=,1=,2=,3=RJ45û����1��,4=û����2��,5=û����5��,6=
				#if MainProtocol==CSG
					//i=MRR(ADDR_AFN04F8,1);
					MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
					MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
					i = (CommPara.IpLinkType << 7) | (CommPara.CommunicationMode << 4);
				#endif
					UARTCtrl->TCPUDP = i;
					if(Comm_Ram->Factory == 0x55)
					{//����ģʽʱ��¼���������ʱ���㹤����������
						p16timer[0] = 0;// 5000/10;//0;//��С��ʱ5��,�����´�¥����<=3���ڲ�������
					}
					else
					{
						p16timer[0] = 100;
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
						WarningOccur(ENETPORT,(u8 *)"��̫�����±��γ�");//�澯����,��ڸ澯����͸澯�ַ���
					}
				}
			}
			return;
		case 4://
		case 5://
		case 6://
			if((UARTCtrl->LinkCtrl != 0) && (p16timer[0] == 0x0)) UARTCtrl->LinkTask++;
			return;
		case 7://���������ʱ1-30��
			if(p16timer[0]==0x0)
			{
				UARTCtrl->LinkTask=99;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;
		case 99://����



		
			os=Get_os();
			//i = 0;	//i=MRR(ADDR_AFN04F7+64,1);
			//MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
			MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			i = CommPara.IpAddrAccessType;
			if(i == 0x0)
			{//�̶�IP����
				if(Local_ASCII_IP4())//��д����ascii ip4,����:0=û�仯,1=�仯
				{
					Comm_Ram->Init_OS |= 1 << 4;	//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
					return;
				}
			}
			else
			{//�Զ���ȡIP����
				switch(UARTCtrl->DHCPTask)//DHCP�����
				{
					case 0://����DHCP
						ns=os->netDHCP_Enable(0);
						if(ns!=netOK)
						{
							Comm_Ram->Init_OS|=1<<4;//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
							return;
						}
						WarningOccur(ENETPORT,(u8 *)"��̫�������Զ���ȡIP");//�澯����,��ڸ澯����͸澯�ַ���
						p16timer[0]=60000/10;//�ȴ���ʱ60s
						UARTCtrl->DHCPTask=1;//DHCP�����
						return;
					case 1://�ȴ�DHCP
						if(p16timer[0]==0)
						{//��ʱ
							Comm_Ram->Init_OS|=1<<4;//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
						}
						return;
					case 2://DPCH���
						p16timer[0]=0;
						UARTCtrl->DHCPTask=3;//DHCP�����
						break;
					case 3://����
						break;
					default:
						UARTCtrl->DHCPTask=0;//DHCP�����
						return;
				}
			}
			if (p16timer[0] != 0x0) return;
			switch(UARTCtrl->LinkSubTask)//����������
			{
				case 0://��������
					UARTCtrl->Task=0;// 0=���ڽ���
					UARTCtrl->Lock=0;// 0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
					UARTCtrl->RxByte=0;//�����ֽ���=0
					UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
					UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
					Terminal_Ram->ENETHeartBeat_S_Timer=0;//���������붨ʱ��
					UARTCtrl->LinkTask2=0;//ͨ��2��������(FTP����);0-99=����,100=��������
					UARTCtrl->Task2=0;//ͨ��2����(FTP����)
					
					//�����˿ں�,ͬʱ���ڵ���ʱ�����ӹ�1��ʶ��
					i = UARTCtrl->PORT;
					i++;
					if((i < SocketPortnMin) || (i > SocketPortnMax))
					{
						i = SocketPortnMin;
					}
					UARTCtrl->PORT = i;
					//��վIP
					addr=(NET_ADDR *)&UARTCtrl->NET_ADDR;
					if((UARTCtrl->TCPUDP & 0x80) == 0x0)
					{//TCP
						addr->addr_type = NET_ADDR_IP4;
						MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
						MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
						if(UARTCtrl->LinkCount < 2)
						{//��IP ��������Э�����õ� IP ��ַ��ʽ����̫��оƬ��Ҫ��ַ��ʽ�ߵ��ֽ��෴�������Ҫ����
							if(CommPara.MasterAddrType == MASTER_CH_TYPE_ETHE)
							{
								MR((u32)&addr->port, (u32)CommPara.MasterAddr, 2);
								addr->addr[0] = CommPara.MasterAddr[5];
								addr->addr[1] = CommPara.MasterAddr[4];
								addr->addr[2] = CommPara.MasterAddr[3];
								addr->addr[3] = CommPara.MasterAddr[2];
								//MR((u32)&addr->addr[0], (u32)&CommPara.MasterAddr[2], 4);
							}
							else
							{
								UARTCtrl->LinkCount = 2;
							}
							
						}
						if(UARTCtrl->LinkCount == 2)
						{//���� 1 IP
							if(CommPara.MasterBackAddr1Type== MASTER_CH_TYPE_ETHE)
							{
								MR((u32)&addr->port, (u32)CommPara.MasterBackAddr1, 2);
								addr->addr[0] = CommPara.MasterBackAddr1[5];
								addr->addr[1] = CommPara.MasterBackAddr1[4];
								addr->addr[2] = CommPara.MasterBackAddr1[3];
								addr->addr[3] = CommPara.MasterBackAddr1[2];
								//MR((u32)&addr->addr[0], (u32)&CommPara.MasterBackAddr1[2], 4);
							}
							else
							{
								UARTCtrl->LinkCount = 3;
							}
							
						}
						if(UARTCtrl->LinkCount == 3)
						{//���� 2 IP
							if(CommPara.MasterBackAddr2Type== MASTER_CH_TYPE_ETHE)
							{
								MR((u32)&addr->port, (u32)CommPara.MasterBackAddr2, 2);
								addr->addr[0] = CommPara.MasterBackAddr2[5];
								addr->addr[1] = CommPara.MasterBackAddr2[4];
								addr->addr[2] = CommPara.MasterBackAddr2[3];
								addr->addr[3] = CommPara.MasterBackAddr2[2];
								//MR((u32)&addr->addr[0], (u32)&CommPara.MasterBackAddr2[2], 4);
							}
							else
							{
								UARTCtrl->LinkCount = 4;
							}			
						}
						if(UARTCtrl->LinkCount >= 4)
						{
							//UARTCtrl->LinkCtrl = 0;
							UARTCtrl->LinkTask = 0;
							UARTCtrl->LinkCount = 0;
							//WarningOccur(ENETPORT,(u8 *)"��̫���޿�����վ����");
							return;
						}
						os->netTCP_Close(UARTCtrl->TCPSocket);
						ns=os->netTCP_Connect(UARTCtrl->TCPSocket,addr,UARTCtrl->PORT);
						if(ns!=netOK)
						{
							Comm_Ram->Init_OS|=1<<4;//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
							return;
						}
						
						p16timer[0]=30000/10;//���ӳ�ʱ��ʱ;Net_Config_TCP.h�����Զ���#define TCP_MAX_RETRY 5,#define TCP_RETRY_TOUT 4;5*4=20��
						UARTCtrl->LinkSubTask=1;//����������
					}
					else
					{//UDP
						os->netUDP_Close(UARTCtrl->UDPSocket);
						ns=os->netUDP_Open(UARTCtrl->UDPSocket,UARTCtrl->PORT);
						if(ns!=netOK)
						{
							Comm_Ram->Init_OS|=1<<4;//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
							return;
						}
						WarningCancel(ENETPORT);//�澯ȡ��,��ڸ澯����
						p16timer[0]=0;//���ӳ�ʱ��ʱ;Net_Config_TCP.h�����Զ���#define TCP_MAX_RETRY 5,#define TCP_RETRY_TOUT 4;5*4=20��
						UARTCtrl->LinkSubTask=0;//����������
						UARTCtrl->LinkTask=100;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					}
					return;
				case 1://�ȴ����ӳ�ʱ
					if(p16timer[0]==0x0)
					{
						if(UARTCtrl->LinkSubTask==1)//����������û��tcp_cb_client()�仯,tcp_cb_client()�¼�������LinkSubTaskͬʱ����p16timer[0]=0
						{
							UARTCtrl->LinkCount++;//���Ӽ���+1
							if(UARTCtrl->LinkCount >= 4)
							{
								UARTCtrl->LinkCount=0;
								UARTCtrl->LinkSubTask=5;//����������5=RJ45����,ETH�������
								return;
							}
							UARTCtrl->LinkSubTask=0;//����������
						}
					}
					return;
				case 2://case netTCP_EventEstablished:
					//WarningOccur(ENETPORT,(u8 *)"��̫�����ӽ���");//�澯����,��ڸ澯����͸澯�ַ���
					WarningCancel(ENETPORT);//�澯ȡ��,��ڸ澯����
					p16timer[0]=0;
					UARTCtrl->LinkSubTask=0;//����������
					UARTCtrl->LinkTask=100;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					return;
				case 3://case netTCP_EventClosed:
					WarningOccur(ENETPORT,(u8 *)"��̫�����ӱ��Ͽ�");//�澯����,��ڸ澯����͸澯�ַ���
					p16timer[0]=1000/10;// 1�������
					UARTCtrl->LinkSubTask=0;//����������
					return;
				case 4://case netTCP_EventAborted:
//						if(UARTCtrl->LinkCount<2)
//						{//��IP
//							WarningOccur(ENETPORT,(u8 *)"��̫��������IPʧ��");//�澯����,��ڸ澯����͸澯�ַ���
//						}
//						else
//						{//����IP
//							WarningOccur(ENETPORT,(u8 *)"��̫�����ӱ���IPʧ��");//�澯����,��ڸ澯����͸澯�ַ���
//						}
					WarningOccur(ENETPORT,(u8 *)"��̫��������վʧ��");//�澯����,��ڸ澯����͸澯�ַ���
					UARTCtrl->LinkSubTask=1;//����������
					return;
					
				case 5://RJ45����,ETH�������
					Disable_os();//��ͣos
					while(ETH->MACMIIAR&1)
					{//MDC��æ
					}
					sourMACMIIDR=ETH->MACMIIDR;//ԭֵ����
					i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,1);
					if((i&(1<<2))==0)//bit2 link is up,bit5 auto-negotiate process completed
					{//RJ45����
						ETH->MACMIIDR=sourMACMIIDR;//ԭֵ�ָ�
						Enable_os();//����os
						UARTCtrl->LinkTask=3;//�ȴ�����RJ45����(ÿ2����)
						return;
					}
					i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,0);
					Write_ETH_PHYReg(PHYAddr_LAN8720A_0,0,i|(1<<14));//bit14 loopback mode
					ETH->MACMIIDR=sourMACMIIDR;//ԭֵ�ָ�
					Enable_os();//����os
					
					addr=(NET_ADDR *)&UARTCtrl->NET_ADDR;
					addr->addr_type = NET_ADDR_IP4;
					addr->addr[0] =255;//�㲥��ַ,�����ñ�����ַ����
					addr->addr[1] =255;
					addr->addr[2] =255;
					addr->addr[3] =255;
					addr->port =1024;
					ns=os->netUDP_Open(UARTCtrl->UDPSocket,1024);
					if(ns)
					{
						ns=ns;
					}
					p8s=os->netUDP_GetBuffer(16);
					if((u32)p8s==0)
					{
						Comm_Ram->Init_OS|=1<<4;//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
						return;
					}
					for(i=0;i<16;i++)
					{
						p8s[i]=i;
					}
					ns=os->netUDP_Send(UARTCtrl->UDPSocket,addr,p8s,16);
					if(ns)
					{
						ns=ns;
					}
					p16timer[0]=1000/10;//��ʱ��ʱ
					UARTCtrl->LinkSubTask=6;//����������
					return;
				case 6://ETH�������ȴ�����
					if(p16timer[0]==0x0)
					{//���ճ�ʱ
						WarningOccur(ENETPORT,(u8 *)"��̫����ѭ�Ӳ���ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
						Comm_Ram->Init_OS|=1<<4;//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
					}
					return;
				case 7://����OK
					os->netUDP_Close(UARTCtrl->UDPSocket);
					Disable_os();//��ͣos
					while(ETH->MACMIIAR&1)
					{//MDC��æ
					}
					sourMACMIIDR=ETH->MACMIIDR;//ԭֵ����
					i=Read_ETH_PHYReg(PHYAddr_LAN8720A_0,0);
					i&=~(1<<14);
					Write_ETH_PHYReg(PHYAddr_LAN8720A_0,0,i);//bit14 loopback mode
					ETH->MACMIIDR=sourMACMIIDR;//ԭֵ�ָ�
					Enable_os();//����os
					UARTCtrl->LinkSubTask=0;//����������
					return;
				default:
					UARTCtrl->LinkSubTask=0;//����������
					return;
			}
			//return;
		case 100:	//����ok
			break;
		case 101:	//��¼ok
			Pin_OutL(PIN_ENET_LINK_LED);//���������,�������ʹ����
			break;
		case 102:	//���˳���¼
			break;
		case 103:	//�˳���¼
			if(Comm_Ram->POWER==3)			// ��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
			{//�����е������µ�¼,���������򱣳�
				UARTCtrl->LinkTask = 3;		// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			}
			return;
		case 104://�ػ�
			Comm_Ram->Init_OS|=1<<4;		// OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
			p16timer[0]=2000/10;			// �ػ���ʱ2s(��GPRSģ����ͬʱ��)
			return;
		default:
			UARTCtrl->LinkTask=0;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			return;
	}

//����ok,��¼ok

	if(Terminal_Ram->ENETCheck_S_Timer == 0)		// 4 ��̫��IP��ͻ�������޸����붨ʱ��
	{
		//��ʵ����ͣos������os��24us
		Terminal_Ram->ENETCheck_S_Timer = 60;
		Disable_os();//��ͣos
		while(ETH->MACMIIAR & 1)
		{//MDC��æ
		}
		sourMACMIIDR = ETH->MACMIIDR;//ԭֵ����
		i = Read_ETH_PHYReg(PHYAddr_LAN8720A_0,1);
		ETH->MACMIIDR=sourMACMIIDR;	//ԭֵ�ָ�
		Enable_os();				//����os
		if((i & (1 << 2)) == 0)			// bit2 link is up,bit5 auto-negotiate process completed
		{//RJ45����
			UARTCtrl->LinkTask = 3;	// �ȴ�����RJ45����(ÿ2����)
			return;
		}
	
		//MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
		MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		i = CommPara.IpAddrAccessType;
		if(i == 0x0)
		{//�̶�IP����
			if(Local_ASCII_IP4())//��д����ascii ip4,����:0=û�仯,1=�仯
			{
				Comm_Ram->Init_OS|=1<<4;//OS������ԭ���־:B0=��������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
				return;
			}
		}
	}

	switch(UARTCtrl->Task)
	{
		case 0://����
			//���߿���
			if(UARTCtrl->LinkCtrl==0)//���ӿ���0=������,1=����
			{
				if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					UARTCtrl->LinkTask=102;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					UARTCtrl->HeartberatCount=0;//������������,�յ�ȷ��֡��0
					Terminal_Ram->ENETHeartBeat_S_Timer=0;// 2 ��̫�������붨ʱ��
					return;
				}
			}
			break;
		case 2://�ȴ���֡���ݷ���
		case 5://�ȴ���֡���ݷ���
		#if (USER/100)==11//����ϵ
			USER11xx_TxAddr(PORTn);//����ϵ����֡�ն˵�ַ2�ֽڳ��ȱ�Ϊ4�ֽ�,�����¼���֡У���,�跢�͵�֡�ֽ���+2
		#endif
		#if LOG_1376_1==1					// 0=û,1=��
			LOG(13761,1,ENETPORT,(u8 *)Get_ADDR_UARTnTx(ENETPORT),UARTCtrl->TxByte,0);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
		#endif
			UARTCtrl->TxCount=0;
			p16timer[0] = 30000/10;			// 30��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
			UARTCtrl->Task++;
			break;
		case 3://���͵�֡�������ڷ���
		case 6://��֡�������ڷ���
			if(UARTCtrl->TxByte)
			{
				os=Get_os();
				if((UARTCtrl->TCPUDP & 0x80) == 0x0)
				{//TCP
					if(os->netTCP_SendReady(UARTCtrl->TCPSocket))
					{//׼���÷���
						x=os->netTCP_GetMaxSegmentSize(UARTCtrl->TCPSocket);
						if(x>UARTCtrl->TxByte)
						{
							x=UARTCtrl->TxByte;
						}
						p8d=os->netTCP_GetBuffer(x);
						if((u32)p8d==0)
						{
							if(p16timer[0]==0)			// 60��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
							{
								UARTCtrl->LinkTask=3;	// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
							}
							break;
						}
						p8s=(u8*)Get_ADDR_UARTnTx(ENETPORT);
						p8s+=UARTCtrl->TxCount;
						for(i=0;i<x;i++)
						{
							p8d[i]=p8s[i];
						}
						UARTCtrl->TxByte-=x;
						UARTCtrl->TxCount+=x;
						os->netTCP_Send(UARTCtrl->TCPSocket,p8d,x);
						Terminal_Ram->ENETLED_MS_Timer=50/10;		// 14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
					}
					else
					{
						if(p16timer[0]==0)			// 60��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
						{
							UARTCtrl->LinkTask=3;	// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
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
					p8d=os->netUDP_GetBuffer(x);
					if((u32)p8d==0)
					{
						if(p16timer[0]==0)			// 60��,�ȴ��������ݸ���վʱ�䳬ʱ��ʱ
						{
							UARTCtrl->LinkTask=3;	// ��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
						}
						break;
					}
					p8s=(u8*)Get_ADDR_UARTnTx(ENETPORT);
					p8s+=UARTCtrl->TxCount;
					for(i=0;i<x;i++)
					{
						p8d[i]=p8s[i];
					}
					UARTCtrl->TxByte-=x;
					UARTCtrl->TxCount+=x;
					addr=(NET_ADDR *)&UARTCtrl->NET_ADDR;
					addr->addr_type = NET_ADDR_IP4;
				
					MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
					if(CommPara.MasterBackAddr1Type == MASTER_CH_TYPE_ETHE)
					{
						MR((u32)&addr->port, (u32)CommPara.MasterBackAddr1, 2);
						addr->addr[0] = CommPara.MasterBackAddr1[5];
						addr->addr[1] = CommPara.MasterBackAddr1[4];
						addr->addr[2] = CommPara.MasterBackAddr1[3];
						addr->addr[3] = CommPara.MasterBackAddr1[2];
					}
					else if(CommPara.MasterBackAddr2Type == MASTER_CH_TYPE_ETHE)
					{
						MR((u32)&addr->port, (u32)CommPara.MasterBackAddr2, 2);
						addr->addr[0] = CommPara.MasterBackAddr2[5];
						addr->addr[1] = CommPara.MasterBackAddr2[4];
						addr->addr[2] = CommPara.MasterBackAddr2[3];
						addr->addr[3] = CommPara.MasterBackAddr2[2];
					}
					else //if(CommPara.MasterAddrType == MASTER_CH_TYPE_ETHE)
					{
						MR((u32)&addr->port, (u32)CommPara.MasterAddr, 2);
						addr->addr[0] = CommPara.MasterAddr[5];
						addr->addr[1] = CommPara.MasterAddr[4];
						addr->addr[2] = CommPara.MasterAddr[3];
						addr->addr[3] = CommPara.MasterAddr[2];
					}
				
					os->netUDP_Send(UARTCtrl->UDPSocket,addr,p8d,x);
					Terminal_Ram->ENETLED_MS_Timer=50/10;	// 14 ��̫�����շ���LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
				}
			}
			else
			{
				if(UARTCtrl->Task == 3)
				{
					UARTCtrl->Task = 7;		// 7=�ȴ���������תΪ����
				}
				else
				{
					UARTCtrl->Task = 4;		// 4=��֡�������������
				}
				if(UARTCtrl->LinkTask != 100)
				{
					MR((u32)&CommPara, (u32)ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
					Terminal_Ram->ENETHeartBeat_S_Timer = bcd_hex(CommPara.HeartBeatTimeGap);
					Terminal_Ram->ENETHeartBeat_S_Timer *= 60;
					if(Terminal_Ram->ENETHeartBeat_S_Timer == 0) Terminal_Ram->ENETHeartBeat_S_Timer = 15*60;//err20180228-5 ����0��������
				}
			}
			break;
	}
}



void ETH_Control(void)
{

}















