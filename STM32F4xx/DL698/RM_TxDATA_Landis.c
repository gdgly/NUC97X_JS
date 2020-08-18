
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_TASK.h"
#include "../Calculate/Calculate.h"





u32 RM_TxDATA_Landis(u32 PORTn,u8 *p8CSD,u8 *p8tx)//Landis(IEC1107 ģʽC)������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
{
	u32 i;
	u32 n;
	u32 x;
	u32 LEN_DATA;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_DATA=0;
	switch(UARTCtrl->CONNECT)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
	{
		default://698��Լ���Ӻ͹رչ���
			UARTCtrl->CONNECT=0;
			return 0;
		case 1://300bps����
			//��300Bd �£��� ��/?����ַ��CR LF�� ����
			p8tx[0]='/';
			p8tx[1]='?';
			LEN_DATA=2;
			for(i=1;i<3;i++)
			{
				x=UARTCtrl->RMAddr[i];
				for(n=0;n<2;n++)
				{
					p8tx[LEN_DATA]=((x>>4)&0xf)+0x30;
					LEN_DATA++;
					p8tx[LEN_DATA]=((x>>0)&0xf)+0x30;
					LEN_DATA++;
					x>>=8;
				}
			}
			p8tx[LEN_DATA]='!';
			p8tx[LEN_DATA+1]=0x0d;
			p8tx[LEN_DATA+2]=0x0a;
			LEN_DATA+=3;
			UARTCtrl->BpsCtrl=//ͨ�ſ�����(ͬ��Լ2007)(���ֶ���)
							//D8-D5λ��ʾ������,O��7���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
							//D4λ��ʾֹͣλλ��0/1�ֱ��ʾ1/2��ֹͣλ
				(1<<3)|//D3λ��ʾ����У��λ0/1�ֱ��ʾ��/��
							//D2λ��ʾż����У��λ0/1�ֱ��ʾż����У��
				2;		//D1-D0 ����λ��0��3�ֱ��ʾ5-8λ
			return LEN_DATA;
		case 101://������
			i=UARTCtrl->TaskID;
			switch((i>>8)&0xff)//�����
			{
				case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
					return 0;
				default:
					break;
			}
			switch((i>>24)&0xf)//��������
			{
				case 1://��ͨ�ɼ�����
					switch((UARTCtrl->TaskID>>20)&0xf)//�ɼ�����
					{
						default:
						case 1://1=�ɼ��ϵ�N��
							return 0;
						case 0://0=�ɼ���ǰ����
						case 2://2=������ʱ��ɼ�
							//�� ��ACK 0 x 0 CR LF�� ����
							p8tx[0]=6;//ACK
							p8tx[1]='0';
							i=UARTCtrl->BpsCtrl;
							i>>=0xf;
							i&=0x03;
							switch(i)
							{
								default:
								case 0://300
									p8tx[2]='0';
									break;
								case 1://600
									p8tx[2]='1';
									break;
								case 2://1200
									p8tx[2]='2';
									break;
								case 3://2400
									p8tx[2]='3';
									break;
								case 4://4800
									p8tx[2]='4';
									break;
								case 6://9600
									p8tx[2]='5';
									break;
							}
							p8tx[3]='0';
							p8tx[4]=0x0d;
							p8tx[5]=0x0a;
							LEN_DATA=6;
							break;
						case 3://3=��ʱ�����ɼ�
							//����������,������ִ�м��ʱ��Ͳɼ������ɼ����ʱ�����
							//�� ��ACK 0 x 0 CR LF�� ����
							p8tx[0]=6;//ACK
							p8tx[1]='0';
							i=UARTCtrl->BpsCtrl;
							i>>=0xf;
							i&=0x03;
							switch(i)
							{
								default:
								case 0://300
									p8tx[2]='0';
									break;
								case 1://600
									p8tx[2]='1';
									break;
								case 2://1200
									p8tx[2]='2';
									break;
								case 3://2400
									p8tx[2]='3';
									break;
								case 4://4800
									p8tx[2]='4';
									break;
								case 6://9600
									p8tx[2]='5';
									break;
							}
							p8tx[3]='0';
							p8tx[4]=0x0d;
							p8tx[5]=0x0a;
							LEN_DATA=6;
							break;
						case 4://4=RetryMetering ����(��ͬ,2=������ʱ��ɼ�)
							return 0;
					}
					break;
				case 2://�¼��ɼ�����
				case 3://͸������
					return 0;
				default:
					return 0;
			}
			return LEN_DATA;
		case 102://�ر�����
			//
			return 0;
	}
}












