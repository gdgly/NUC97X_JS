
//LCD
#if MainProtocol==376
	#include "../Head/Project.h"
#endif
#if MainProtocol==698
	#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
	#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Display/LCDModule.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Display/Display.h"
#include "../Display/DisplayFlash.h"
#include "../Display/LCDdot.h"
#include "../Device/MEMRW.h"
#include "../Test/Test_RAM.h"
//#include "../emWin/emWinApp.h"





void Init_ST75161(void);
void Init_LCDModule(void)//����LCD��ʾ�ⲿ���߿���EMC��ʼ��
{
	Pin_Configure(PIN_LCD_RST,//��������
									  0|//B3-B0:���ù���ѡ��AF(0-15)
							 (1<<4)|//B5-B4:ģʽ:00�����루��λ״̬��,01��ͨ�����ģʽ,10�����ù���ģʽ,11��ģ��ģʽ
							 (0<<6)|//B6   :�������:0��������죨��λ״̬��,1�������©
							 (0<<7)|//B8-B7:����ٶ�:00��2 MHz�����٣�,01��25 MHz�����٣�,10��50 MHz�����٣�,11��30 pF ʱΪ 100 MHz�����٣���15 pF ʱΪ 80 MHz ���������ٶȣ���
							 (0<<9)|//B10-B9:����/����:00��������������,01������,10������,11������
							(0<<11));//B11  :��Ϊ���ʱ��0=���0,1=���1	
	//DisplayClr();//����
	Comm_Ram->ErrorIC|=RAM_Test(ADDR_Terminal_IRAM_Start,ADDR_TerminalDisplay_IRAM_End-ADDR_Terminal_IRAM_Start);//RAM����;����:0=��ȷ,1=����
	Comm_Ram->Language=0;//0=����,1=Ӣ��,2=
	SoftDelayUS(10*1000);//��ʱ
  	Pin_OutH(PIN_LCD_RST);//��λ���
	SoftDelayUS(100*1000);//ʵ�⸴λ��������ʱ50MS,��100MS
	
}

void Startdisplay(void)//����ʾ
{
	u16 *p16;
/*
	u32 *p32;
	p32=(u32*)(ADDR_AUTO_ERROR_RESTART);//�Զ���⵽��SDRAM�ȴ����Ź���λ��־
	if(p32[0]==0x12345678)
	{
		Pin_SCU(PIN_LCD_RST,0+(0<<3)+(1<<4)+(0<<5)+(1<<6)+(1<<17)+(1<<18));//�����λLCD
		Comm_Ram->DisplayAllErrorTimer=0;//10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
		return;
	}
*/
  DisplayString(0,2,0,(u8 *)"�õ���Ϣ�ɼ�ϵͳ");//��ʾ�ַ���
  StrBuff->Row[0].Font=1;
	
#if (Project/100)==0//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  DisplayString(2,0,0,(u8 *)"���ܱ�");//��ʾ�ַ���
#endif
#if (Project/100)==1//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  DisplayString(2,0,0,(u8 *)"�������ܱ�");//��ʾ�ַ���
#endif
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  DisplayString(2,0,0,(u8 *)"���ͼ�����");//��ʾ�ַ���
#endif
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  DisplayString(2,0,0,(u8 *)"���ɹ����ն�");//��ʾ�ַ���
#endif
#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  DisplayString(2,0,0,(u8 *)"�����ն�");//��ʾ�ַ���
#endif
#if (Project/100)==5//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  DisplayString(2,0,0,(u8 *)"���ͼ�����");//��ʾ�ַ���
#endif
#if (Project/100)==6//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  DisplayString(2,0,0,(u8 *)"ͨ��ģ��");//��ʾ�ַ���
#endif
#if (Project/100)==9//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  DisplayString(2,0,0,(u8 *)"���");//��ʾ�ַ���

	DispalyAlign(2,0,2);//����ʾ����(1�����,2����,3�Ҷ���)

//	DisplayString(3,0,0,(u8 *)"Q/GDW137x-2013");//��ʾ�ַ���
//	DispalyAlign(3,0,2);//����ʾ����(1�����,2����,3�Ҷ���)

	p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4);

	p16[0]='V'+0x2000;
	p16[1]=(SVER>>8)+0x2030;
	p16[2]='.'+0x2000;
	p16[3]=((SVER>>4)&0xf)+0x2030;
	//p16[4]=((SVER>>0)&0xf)+0x2030;

	DispalyAlign(4,0,2);//����ʾ����(1�����,2����,3�Ҷ���)

#if USER==100//�û���ʶ
  DisplayString(5,0,0,(u8 *)"��׼��");//��ʾ�ַ���
#endif
#if USER==200//�û���ʶ
  DisplayString(5,0,0,(u8 *)"ɽ����");//��ʾ�ַ���
#endif
#if USER==300//�û���ʶ
  DisplayString(5,0,0,(u8 *)"������");//��ʾ�ַ���
#endif
#if USER==400//�û���ʶ
  DisplayString(5,0,0,(u8 *)"������");//��ʾ�ַ���
#endif
#if USER==500//�û���ʶ
  DisplayString(5,0,0,(u8 *)"�Ϻ���");//��ʾ�ַ���
#endif
#if USER==600//�û���ʶ
  DisplayString(5,0,0,(u8 *)"���ϰ�");//��ʾ�ַ���
#endif
#if USER==601//�û���ʶ
  DisplayString(5,0,0,(u8 *)"������̶��");//��ʾ�ַ���
#endif

#if USER==800//�û���ʶ
  DisplayString(5,0,0,(u8 *)"ͨ�ð�");//��ʾ�ַ���
#endif
#if USER==900//�û���ʶ
  DisplayString(5,0,0,(u8 *)"���ϰ�");//��ʾ�ַ���
#endif
#if USER==1000//�û���ʶ
  DisplayString(5,0,0,(u8 *)"�ӱ���");//��ʾ�ַ���
#endif
#if USER==1100//�û���ʶ
  DisplayString(5,0,0,(u8 *)"���ְ�");//��ʾ�ַ���
#endif


	DispalyAlign(5,0,2);//����ʾ����(1�����,2����,3�Ҷ���)	
	
	
#if (USER/100)!=0//�û���ʶ
  DisplayString(6,6,0,(u8 *)"��̩�Ǳ�");//��ʾ�ַ���
#endif
  StrBuff->Row[6].Font=1;
	
	LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
}




#if IC_UC1698==1//0=û,1=��(������ʾ)
//FMC Configuration
//---NOR/PSRAM control-----------
//Memory type    |LCD Interface
//Bank           |Bank 1 NOR/PSRAM 3
//Write operation|Enabled
//Write FIFO     |Enabled
//Extended mode  |Disabled
//---NOR/PSRAM timing-----------
//Address setup time in HCLK clock cycles  |8
//Data setup time in HCLK clock cycles     |8
//Bus turn around time in HCLK clock cycles|0

void LCDDataOut(void)//�������������LCD��
{
  u32 i;
  u32 x;
  u32 y;
  u32 z;
  u8 * p8c;
  u8 * p8d;
//	u8 * p8e;

  p8c=(u8 *)0x68000000;
	
//���Ƴ�ʼ��
	//power control
  //26:111010## Set LCD Bias Ratio(Default:11b=12)
  p8c[0]=0xE8+1;//Bias Ratio:1/10 bias
  //6: 001010## Set Power Control(Default:10b)
  p8c[0]=0x28+2;//3;//power control set as internal power
  //5: 001001## Set Temp Compensation(Default:0)
  p8c[0]=0x24+1;//0=-0.00%/��,1=-0.05%/��,2=-0.15%/��,3=-0.25%/��
  //10:10000001 ######## Set Vbias Potentiometer(double-byte command)(Default:40H)
  p8c[0]=0x81;//electronic potentionmeter
  p8c[0]=0xbf;
  //display control
  //15:1010010# Set All-Pixel-ON(Default:0)
  p8c[0]=0xa4+0;//all pixel off
  //16:1010011# Set Inverse Display(Default:0)
  p8c[0]=0xa6+0;//inverse display off

  //lcd control
  //18:11000### Set LCD Mapping Control(Default:0)
//  p8c[0]=0xc0+0;//19:partial display and MX disable,MY enable(��������Ļ���л���!�Ҳ����Ϊ0��7ֵ��ʾ�޹�)
  //14:101000## Set Line Rate(Default:10b)
  p8c[0]=0xa0+2;//line rate 0=8.5kips 1=10.4kips 2=12.6kips 3=15.2klps

  //20:1101000# Set Color Patten(Default:0BGR)
  p8c[0]=0xd0+1;//rgb-rgb
  //21:110101## Set Color Mode(Default:10b)
  p8c[0]=0xd4+1;//4k color mode
  //11:1000010# Set Partial Display Control(Default:0)
  p8c[0]=0x84+0;//partial display control disable

  //n-line inversion
  //19:11001000 ---##### Set N-Line Inversion(Default:1DH)
  p8c[0]=0xc8;//
  p8c[0]=0;//
  //com scan fuction
  //22:11011### Set COM Scan Function(Default:0)
  p8c[0]=0xd8+2;//enable FRC,PWM,LRM sequence
  //window
  //30:11110100 -####### Set Window Program Starting Column Address(Default:0)
  p8c[0]=0xf4;//wpc0:column start from 130
  p8c[0]=0x25;//
  //32:11110110 -####### Set Window Program Ending Column Address(Default:127)
  p8c[0]=0xf6;//wpc1:column End 272
  p8c[0]=0x5a;//
  //31:11110101 ######## Set Window Program Starting Row Address(Default:0)
  p8c[0]=0xf5;//wpp0:row start from 0
  p8c[0]=0;//
  //33:11110111 ######## Set Window Program Ending Row Address(Default:159)
  p8c[0]=0xf7;//wpp0 row end 160
  p8c[0]=0x9f;//
  //34:1111100# Window Program Mode(Default:0:Inside)
  p8c[0]=0xf8;//UC1698Write1ByteCommand(0xf8);//inside mode
  //12:10001### Set RAM Address Control(Default:001b)
//  UC1698Write1ByteCommand(0x88+1);//RAM control,����+1,�л�0����+1
  //17:10101### Set Display Enable(Default:110b)
  p8c[0]=0xa8+5;//display on,select on/off mode.Green Enhance mode disable
  //scroll line
  //8: 0100#### Set Scroll Line LSB(Default:0)
  p8c[0]=0x40+0;//low bit of scroll line
  //   0101#### Set Scroll Line MSB(Default:0)
  p8c[0]=0x50+0;//high bit of scroll line
  p8c[0]=0xc4;//19,enable FLT and FLB
  //13:10010000 ######## Set Fixed Lines(Default:0)
  p8c[0]=0x90;//FLT,FLB set
  p8c[0]=0x0;//
  //partial display
  //11:1000010# Set Partial Display Control(Default:0)
  p8c[0]=0x84+0;//set partial display control:off
  //27:11110001 -####### Set COM End(Default:159)
  p8c[0]=0xf1;//com end 159
  p8c[0]=159;//
  //28:11110010 -####### Set Partial Display Start(Default:0)
  p8c[0]=0xf2;//display start 0
  p8c[0]=0;//
  //29:11110011 -####### Set Partial Display End(Default:159)
  p8c[0]=0xf3;//display end 159
  p8c[0]=159;//
  
  //1: ######## Write Data Byte
  //2: ######## Read Data Byte
  //3: Get Status&PM
  //4: 0000#### Set Column Address LSB(Default:0)
  //   00010### Set Column Address MSB(Default:0)
  //9: 0110#### Set Row Address LSB(Default:0)
  //   0111#### Set Row Address MSB(Default:0)
  //23:11100010 System Reset
  //24:11100011 NOP
  //35:10111000 ---##### Set MTP Opration Control(Default:10H)
  //36:10111001 -####### ------## Set MTP Write Mask(Default:0)
  //37:11110100 ######## Set Vmtp1 Potentiomter(Default:N/A)
  //38:11110101 ######## Set Vmtp2 Potentiomter(Default:N/A)
  //39:11110110 ######## Set MTP Write Timer(Default:N/A)
  //40:11110111 ######## Set MTP Read Timer(Default:N/A)




//	if((Comm_Ram->SelfTestFlags&0x3)==0)
//	{
//  DisplayFlash();//��ʾ����
  LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD��������ת��,���:R0=�ַ��������ַ,R1=���󻺳��ַ
//	}


//�Ӻ���
	if(Comm_Ram->DisplayAllErrorTimer==0)
	{
	#if (USER/100)==5//�Ϻ�
		p8d=(u8*)(ADDR_MenuICO);//1byte �˵�����ʾ���к͵���ͼ�� 0=��,1=��
		if((DOTMIXPMODE->Task==4)||(p8d[0]!=0))//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
		{
	#else
		if(DOTMIXPMODE->Task==4)//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
		{
	#endif
			//�Ϻ���
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//�º���
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
		}
	}
	
	
//���󻺳�A��B�ϳ����
  p8c[0]=0x88+1;//RAM control,����+1,�л�0����+1
//	switch(DOTMIXPMODE->Mix)//���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
//	{
//		case 0://A
			p8d=(u8 *)ADDR_DOTDATABUFF;
  		for(y=0;y<160;y++)
  		{
    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
    		p8c[0]=0x00+0x5;//Set Column Address LSB
    		p8c[0]=0x10+0x2;//Set Column Address MSB
    		for(x=0;x<20;x++)
    		{
      		z=p8d[0];
      		p8d++;
      		i=0;
      		if(z&0x01)
      		{
        		i=0x80;
      		}
      		if(z&0x02)
      		{
        		i|=0x8;
      		}
      		p8c[4]=i;
      		i=0;
      		if(z&0x04)
      		{
      	  	i=0x80;
      		}
      		if(z&0x08)
      		{
       	 		i|=0x8;
      		}
      		p8c[4]=i;
      		i=0;
      		if(z&0x10)
      		{
        		i=0x80;
      		}
      		if(z&0x20)
      		{
      	  	i|=0x8;
    	  	}
      		p8c[4]=i;
      		i=0;
      		if(z&0x40)
      		{
      	  	i=0x80;
      		}
      		if(z&0x80)
      		{
       	 		i|=0x8;
      		}
    	  	p8c[4]=i;
    		}
    		p8c[4]=0;
	 		}

//			break;
//		case 1://B
//			p8d=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//      		p8d++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 2://A|B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z|=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 3://A^B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z^=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//		case 4://A&B
//			p8d=(u8 *)ADDR_DOTDATABUFF;
//			p8e=(u8 *)ADDR_DOTDATABUFF2;
//  		for(y=0;y<160;y++)
//  		{
//    		p8c[0]=0x60+(y&0xf);//Set Row Address LSB
//    		p8c[0]=0x70+((y>>4)&0xf);//Set Row Address MSB
//    		p8c[0]=0x00+0x5;//Set Column Address LSB
//    		p8c[0]=0x10+0x2;//Set Column Address MSB
//    		for(x=0;x<20;x++)
//    		{
//      		z=p8d[0];
//					z&=p8e[0];
//      		p8d++;
//					p8e++;
//      		i=0;
//      		if(z&0x80)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x40)
//      		{
//        		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x20)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x10)
//      		{
//       	 		i|=0x8;
//      		}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x08)
//      		{
//        		i=0x80;
//      		}
//      		if(z&0x04)
//      		{
//      	  	i|=0x8;
//    	  	}
//      		p8c[4]=i;
//      		i=0;
//      		if(z&0x02)
//      		{
//      	  	i=0x80;
//      		}
//      		if(z&0x01)
//      		{
//       	 		i|=0x8;
//      		}
//    	  	p8c[4]=i;
//    		}
//    		p8c[4]=0;
//	 		}
//			break;
//	}

}
#endif //#if IC_UC1698==1//0=û,1=��(������ʾ)

#if IC_ST75161==1//0=û,1=��(������ʾ)
void LCDDataOut(void)//�������������LCD��
{
  u32 i;
  u32 x;
  u32 y;
  u32 z;
	u32 b;
	u32 c;
  u8 * p8c;
  u8 * p8d;

//���Ƴ�ʼ��
  p8c=(u8 *)0x68000000;
	p8c[0]=0x31;//Extension Command 2 
	p8c[0]=0xD7;//Disable Auto Read 
	p8c[4]=0x9F; 
	p8c[0]=0xE0;//Enable OTP read 
	p8c[4]=0x00;//Disable Auto Read 
  SoftDelayUS(40);//����us��ʱ
	p8c[0]=0xE3;//OTP Up-Load 
	SoftDelayUS(40);//����us��ʱ
	p8c[0]=0xE1;//OTP Control Out

	p8c[0]=0x30;//Extension Command 1
	p8c[0]=0x94;//Sleep out 
	
//	if((Comm_Ram->SelfTestFlags&0x3)==0)
//	{
//  DisplayFlash();//��ʾ����
#if Font_ttf==0
  LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD��������ת��,���:R0=�ַ��������ַ,R1=���󻺳��ַ
#else
	if(Comm_Ram->Init_emWin<2)//�ϵ��emWin�ѳ�ʼ����־:=0û��ʼ��,=1��ʼ��ʧ��,>=2��ʼ���ɹ�
	{
		LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD��������ת��,���:R0=�ַ��������ַ,R1=���󻺳��ַ
	}
	else
	{
		TTF_LCDDotData(ADDR_STRINGBUFF,ADDR_DOTDATABUFF);//LCD��������ת��,���:R0=�ַ��������ַ,R1=���󻺳��ַ
	}
#endif
//	}

//�Ӻ���
	if(Comm_Ram->DisplayAllErrorTimer==0)
	{
	#if (USER/100)==5//�Ϻ�
		p8d=(u8*)(ADDR_MenuICO);//1byte �˵�����ʾ���к͵���ͼ�� 0=��,1=��
		if((DOTMIXPMODE->Task==4)||(p8d[0]!=0))//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
		{
	#else
		if(DOTMIXPMODE->Task==4)//��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
		{
	#endif
			//�Ϻ���
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*15));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
			//�º���
			p8d=(u8 *)(ADDR_DOTDATABUFF+(20*144));
			for(i=0;i<20;i++)
			{
				p8d[i]=0xff;
			}
		}
	}

//���Ƴ�ʼ��
	p8c[0]=0x20;//Power Control
	p8c[4]=0x0B;//VB, VR,VF All ON
	p8c[0]=0x81;//set Vop
	p8c[4]=0x3F;//Vop[5:0]  15.5V    1/12  0x30
	p8c[4]=0x04;//Vop[8:6]
	
	p8c[0]=0x31;//Extension Command 2
	p8c[0]=0x20;//Set Gray Scale Level 
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x17;//
	p8c[4]=0x17;//
	p8c[4]=0x17;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x1D;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x1D;//
	p8c[4]=0x1D;//
	p8c[4]=0x1D;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[0]=0x32;//Analog Circuit Set
	p8c[4]=0x00;
	p8c[4]=0x01;//Booster Efficiency =6KHz 
	p8c[4]=0x02;//12 bias
	p8c[0]=0x51;//Booster Level Set
	p8c[4]=0xFa;//Booster Level x8
	
	p8c[0]=0x30;//Extension Command 1
	p8c[0]=0xF0;//Display Mode
	p8c[4]=0x10;//Mono Mode
	p8c[0]=0x0c;//LSB on top
	p8c[0]=0xCA;//Displsy Control
	p8c[4]=0x00;//CL Dividing Ratio:Not Divide 
	p8c[4]=0x9F;//Duty Set:160 Duty
	p8c[4]=0x00;//Nline=
	p8c[0]=0xBC;//Data Scan Direction 
	p8c[4]=0x00;//MV=0,MX=0
	p8c[0]=0xA6;//Normal Display
	
	p8c[0]=0x31;//Extension Command 2
	p8c[0]=0x40;//Internal Power Supply
	p8c[0]=0xF0;// Frame Rate 
	p8c[4]=0x06;// FA=46Hz
	p8c[4]=0x0C;// FB=55Hz		0B
	p8c[4]=0x0C;// FC=55Hz	  0B
	p8c[4]=0x0C;// FD=55Hz	  0B
	p8c[0]=0xF2;// Temperature Range
	p8c[4]=0x05;// TA=-35
	p8c[4]=0x23;// TB=-5
	p8c[4]=0x40;// TC=24
	p8c[0]=0xF4;// Temperature Gradient Compensation
	p8c[4]=0xAF;// 0xFF
	p8c[4]=0x07;// 0x0F
	p8c[4]=0x00;// 
	p8c[4]=0x00;// 
	p8c[4]=0x00;// 
	p8c[4]=0x00;//00 	
	p8c[4]=0xAF;//56~64;64~72
	p8c[4]=0xFF;//

//window
	p8c[0]=0x30;//Extension Command 1 
	p8c[0]=0x15;//Column Address Setting 
	p8c[4]=0x00;//Starting column address
	p8c[4]=0x9F;//Ending column address

	p8c[0]=0x75;//Row Address Setting 
	p8c[4]=0x00;//Starting page address 
	p8c[4]=0x13;//Ending page address
	p8c[0]=0x5c;//Write data

	for(y=0;y<20;y++)
	{
		p8d=(u8 *)ADDR_DOTDATABUFF+(y*160);
		for(x=0;x<20;x++)
		{
			c=0x01;
			for(z=0;z<8;z++)
			{
				i=0;
				for(b=0;b<8;b++)
				{
					if(p8d[b*20]&c)
					{
						i|=(1<<b);
					}
				}
				p8c[4]=i;
				c<<=1;
			}
			p8d++;
		}
	}
	p8c[0]=0xAF;//Display ON
}


/*
void GUI_LCDDataOut(void)//�������������LCD��
{
  u32 i;
  u32 x;
  u32 y;
  u32 z;
	u32 b;
	u32 c;
  u8 * p8c;
  u8 * p8d;
	
//���Ƴ�ʼ��
  p8c=(u8 *)0x68000000;
	p8c[0]=0x31;//Extension Command 2 
	p8c[0]=0xD7;//Disable Auto Read 
	p8c[4]=0x9F; 
	p8c[0]=0xE0;//Enable OTP read 
	p8c[4]=0x00;//Disable Auto Read 
	//SoftDelayUS(40);//����us��ʱ
	i = 40 * (168000000/4000000);
  while(i--);
	
	p8c[0]=0xE3;//OTP Up-Load 
	//SoftDelayUS(40);//����us��ʱ
	i = 40 * (168000000/4000000);
  while(i--);
	
	p8c[0]=0xE1;//OTP Control Out

	p8c[0]=0x30;//Extension Command 1
	p8c[0]=0x94;//Sleep out 
	
//���Ƴ�ʼ��
	p8c[0]=0x20;//Power Control
	p8c[4]=0x0B;//VB, VR,VF All ON
	p8c[0]=0x81;//set Vop
	p8c[4]=0x3F;//Vop[5:0]  15.5V    1/12  0x30
	p8c[4]=0x04;//Vop[8:6]
	
	p8c[0]=0x31;//Extension Command 2
	p8c[0]=0x20;//Set Gray Scale Level 
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x17;//
	p8c[4]=0x17;//
	p8c[4]=0x17;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x1D;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[4]=0x1D;//
	p8c[4]=0x1D;//
	p8c[4]=0x1D;//
	p8c[4]=0x00;//
	p8c[4]=0x00;//
	p8c[0]=0x32;//Analog Circuit Set
	p8c[4]=0x00;
	p8c[4]=0x01;//Booster Efficiency =6KHz 
	p8c[4]=0x02;//12 bias
	p8c[0]=0x51;//Booster Level Set
	p8c[4]=0xFa;//Booster Level x8
	
	p8c[0]=0x30;//Extension Command 1
	p8c[0]=0xF0;//Display Mode
	p8c[4]=0x10;//Mono Mode
	p8c[0]=0x0c;//LSB on top
	p8c[0]=0xCA;//Displsy Control
	p8c[4]=0x00;//CL Dividing Ratio:Not Divide 
	p8c[4]=0x9F;//Duty Set:160 Duty
	p8c[4]=0x00;//Nline=
	p8c[0]=0xBC;//Data Scan Direction 
	p8c[4]=0x00;//MV=0,MX=0
	p8c[0]=0xA6;//Normal Display
	
	p8c[0]=0x31;//Extension Command 2
	p8c[0]=0x40;//Internal Power Supply
	p8c[0]=0xF0;// Frame Rate 
	p8c[4]=0x06;// FA=46Hz
	p8c[4]=0x0C;// FB=55Hz		0B
	p8c[4]=0x0C;// FC=55Hz	  0B
	p8c[4]=0x0C;// FD=55Hz	  0B
	p8c[0]=0xF2;// Temperature Range
	p8c[4]=0x05;// TA=-35
	p8c[4]=0x23;// TB=-5
	p8c[4]=0x40;// TC=24
	p8c[0]=0xF4;// Temperature Gradient Compensation
	p8c[4]=0xFF;// 
	p8c[4]=0x0F;//0x08
	p8c[4]=0x00;// 
	p8c[4]=0x00;// 
	p8c[4]=0x00;// 
	p8c[4]=0x00;//00 	
	p8c[4]=0xAF;//56~64;64~72
	p8c[4]=0xFF;//

//window
	p8c[0]=0x30;//Extension Command 1 
	p8c[0]=0x15;//Column Address Setting 
	p8c[4]=0x00;//Starting column address
	p8c[4]=0x9F;//Ending column address

	p8c[0]=0x75;//Row Address Setting 
	p8c[4]=0x00;//Starting page address 
	p8c[4]=0x13;//Ending page address
	p8c[0]=0x5c;//Write data

	for(y=0;y<20;y++)
	{
		p8d=(u8*)ADDR_DOTDATABUFF;//_aPlain_0;
		p8d+=(y*160);
		for(x=0;x<20;x++)
		{
			c=0x01;
			for(z=0;z<8;z++)
			{
				i=0;
				for(b=0;b<8;b++)
				{
					if(p8d[b*20]&c)
					{
						i|=(1<<b);
					}
				}
				p8c[4]=i;
				c<<=1;
			}
			p8d++;
		}
	}
	p8c[0]=0xAF;//Display ON
}
*/
#endif



