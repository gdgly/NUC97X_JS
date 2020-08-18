
//��ʾ����
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Display/DisplayFlash.h"



void DisplayFlash(void)//��ʾ����
{
	u32 i;
	u32 x;
	u32 *p32s;
	u32 *p32d;
	
	switch(DOTMIXPMODE->FlashTask)//��������:0=��,1=��ʼ,2=����
	{
		case 1://��ʼ
			if(DOTMIXPMODE->FlashMode!=0)
			{
				
				DOTMIXPMODE->FlashTask=2;//��������:0=��,1=��ʼ,2=����
				DOTMIXPMODE->FlashCount=0;//�������ż���
				Comm_Ram->DisplayScanTimer=DOTMIXPMODE->FlashDelay;//1 ��ʾɨ�� ÿ10MS��1��ʱ��
				p32s=(u32*)(ADDR_DOTDATABUFF);
				p32d=(u32*)(ADDR_DOTDATABUFF2);
				for(i=0;i<((160*20)/4);i++)
				{
					p32d[i]=p32s[i];
				}
			}
			else
			{
				DOTMIXPMODE->FlashTask=0;//��������:0=��,1=��ʼ,2=����
			}
			break;
		case 2://����
			if(DOTMIXPMODE->Task==4)//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
			{//ѭ��
				if(DOTMIXPMODE->FlashCount>=(160-32))
				{
					DOTMIXPMODE->FlashTask=0;//��������:0=��,1=��ʼ,2=����
					DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
					Comm_Ram->DisplayScanTimer=0;//1 ��ʾɨ�� ÿ10MS��1��ʱ��
				}
				else
				{
					DOTMIXPMODE->FlashCount+=4;//�������ż���
					Comm_Ram->DisplayScanTimer=DOTMIXPMODE->FlashDelay;//1 ��ʾɨ�� ÿ10MS��1��ʱ��
					DOTMIXPMODE->Mix=1;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
					switch(DOTMIXPMODE->FlashMode)//����ģʽ:0=��,1=����,2=����,3=����,4=����,5=...
					{
						case 1://����
							p32d=(u32*)(ADDR_DOTDATABUFF2+(160*20)-(16*20)-4);
							p32s=(u32*)(ADDR_DOTDATABUFF2+(160*20)-(16*20)-4-(4*20));
							x=(160-32-4)*20;
							x/=4;
							for(i=0;i<x;i++)
							{
								p32d[0]=p32s[0];
								p32s--;
								p32d--;
							}
							x=(4*20)/4;
							for(i=0;i<x;i++)
							{
								p32d[0]=0;
								p32d--;
							}
							break;
						case 2://����
							p32d=(u32*)(ADDR_DOTDATABUFF2+(16*20));
							p32s=(u32*)(ADDR_DOTDATABUFF2+(16*20)+(4*20));
							x=(160-32-4)*20;
							x/=4;
							for(i=0;i<x;i++)
							{
								p32d[0]=p32s[0];
								p32s++;
								p32d++;
							}
							x=(4*20)/4;
							for(i=0;i<x;i++)
							{
								p32d[0]=0;
								p32d++;
							}
							break;
						case 3://����
						case 4://����
							break;
						default:
							DOTMIXPMODE->FlashTask=0;//��������:0=��,1=��ʼ,2=����
							DOTMIXPMODE->Mix=0;//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
							break;
					}
				}
			}
			else
			{//��ѭ��
			
			
			
			}
			break;
	}
}

/*
Terminal_Ram->DisplayFlashTimer=0;//2 ��ʾ���� ÿ10MS��1��ʱ��
DOTMIXPMODE->FlashMode;//����ģʽ:0=��,1=����,2=����,3=����,4=����,5=...
DOTMIXPMODE->FlashDelay;//�����仯��ʱ(��λ10ms),0���,255����
DOTMIXPMODE->FlashTask;//��������:0=��,1=��ʼ,2=����
	u8 FlashCount;//�������ż���
*/

