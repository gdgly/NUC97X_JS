
//总加组控制设置状态
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/Contrl_Manage.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"


#if ((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//当前控制状态∷=structure
//{
//	当前功控定值             long64（单位：W 换算：-1），     
//	当前功率下浮控浮动系数   integer（单位：%），
//	功控跳闸输出状态         OutputState，
//	月电控跳闸输出状态       OutputState，
//	购电控跳闸输出状态       OutputState，
//	功控越限告警状态         PCAlarmState，
//	电控越限告警状态         ECAlarmState
//}
//OutputState∷=bit-string(SIZE(8))
//bit0～bit7分别表示终端1～8轮次跳闸输出状态，置“1”：处于跳闸状态，置“0”：未处于跳闸状态。
//PCAlarmState∷=bit-string(SIZE(8))
//按顺序对位表示；置“1”：处于某种功控越限告警状态；置“0”：未处于相应状态；
//bit0：时段控
//bit1：厂休控
//bit2：营业报停控
//bit3：当前功率下浮控
//bit4～bit7：备用。
//ECAlarmState∷=bit-string(SIZE(8))
//置“1”：处于某种电控越限告警状态；置“0”：未处于相应状态；
//bit0：月电控
//bit1：购电控
//bit2～bit7：备用。
void Contrl_State(void)//总加组当前控制状态
{
	u32 i;
	u32 Pn;
	u32 Priority;//优先级
	u8* p8s;
	u8* p8d;
	u8* p8ctrl;
	u8* p8attr;
	
	for(Pn=0;Pn<NUMmax_TAG;Pn++)
	{
		p8d=(u8*)ADDR_230x_17+(Pn*LENper_230x_17);
		p8d[0]=DataType_structure;
		p8d[1]=7;
		p8d+=2;
//当前功控定值long64（单位：W 换算：-1）
		p8d[0]=DataType_long64;
		p8d[1]=0;
		p8d[2]=0;
		p8d[3]=0;
		p8d[4]=0;
		p8d[5]=0;
		p8d[6]=0;
		p8d[7]=0;
		p8d[8]=0;
		p8d+=9;
//当前功率下浮控浮动系数integer（单位：%）
		p8d[0]=DataType_integer;
		p8d[1]=0;
		p8d+=2;
//功控跳闸输出状态bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;
//月电控跳闸输出状态bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;
//购电控跳闸输出状态bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;
//功控越限告警状态bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;
//电控越限告警状态bit-string(SIZE(8))
		p8d[0]=DataType_bit_string;
		p8d[1]=8;
		p8d[2]=0;
		p8d+=3;

//终端控制-功控;控制的优先级由高到低是当前功率下浮控、营业报停功控、厂休功控、时段功控
		p8d=(u8*)ADDR_230x_17+(Pn*LENper_230x_17);
		Priority=0;//优先级
	//bit3：当前功率下浮控
		p8ctrl=(u8*)ADDR_8106_4;//当前功率下浮控-总加组控制输出状态
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
		p8attr=(u8*)ADDR_8106_5;//当前功率下浮控-总加组告警输出状态
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//未告警（0），告警（1）
		p8s=(u8*)ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len);
		if(p8s[0])
		{
			Priority=1;//优先级
			if(p8s[0]>=2)
			{
			//当前功控定值long64（单位：W 换算：-1）
				MR(ADDR_230x_17+(Pn*LENper_230x_17)+3,ADDR_Terminal_kWCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
			//当前功率下浮控浮动系数integer（单位：%）
				p8d[2+9+1]=p8s[3];
			}
			//功控跳闸输出状态bit-string(SIZE(8))
			i=p8s[1];
			i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
			p8d[2+9+2+2]=i;
			p8ctrl[7]=i;//总加组控制输出状态,bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
			//月电控跳闸输出状态bit-string(SIZE(8))
			//购电控跳闸输出状态bit-string(SIZE(8))
			//功控越限告警状态bit-string(SIZE(8))
			if(p8s[0]==100)
			{
				p8d[2+9+2+3+3+3+2]=1<<(7-3);
				p8attr[6]=1;//总加组告警输出状态,未告警（0），告警（1）
			}
			//电控越限告警状态bit-string(SIZE(8))
		}
	//bit2：营业报停控
		p8ctrl=(u8*)ADDR_8105_4;//营业报停控-总加组控制输出状态
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
		p8attr=(u8*)ADDR_8105_5;//营业报停控-总加组告警输出状态
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//未告警（0），告警（1）
		if(Priority==0)//优先级
		{
			p8s=(u8*)ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len);
			if(p8s[0]>=2)
			{
				Priority=1;//优先级
			//当前功控定值long64（单位：W 换算：-1）
				MR(ADDR_230x_17+(Pn*LENper_230x_17)+3,ADDR_Terminal_kWCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
			//当前功率下浮控浮动系数integer（单位：%）
			//功控跳闸输出状态bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
				p8d[2+9+2+2]=i;
				p8ctrl[7]=i;//总加组控制输出状态,bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
			//月电控跳闸输出状态bit-string(SIZE(8))
			//购电控跳闸输出状态bit-string(SIZE(8))
			//功控越限告警状态bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+2]=1<<(7-2);
					p8attr[6]=1;//总加组告警输出状态,未告警（0），告警（1）
				}
			//电控越限告警状态bit-string(SIZE(8))
			}
		}
		
	//bit1：厂休控
		p8ctrl=(u8*)ADDR_8104_4;//厂休控-总加组控制输出状态
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
		p8attr=(u8*)ADDR_8104_5;//厂休控-总加组告警输出状态
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//未告警（0），告警（1）
		if(Priority==0)//优先级
		{
			p8s=(u8*)ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len);
			if(p8s[0]>=2)
			{
				Priority=1;//优先级
			//当前功控定值long64（单位：W 换算：-1）
				MR(ADDR_230x_17+(Pn*LENper_230x_17)+3,ADDR_Terminal_kWCtrl3_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
			//当前功率下浮控浮动系数integer（单位：%）
			//功控跳闸输出状态bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
				p8d[2+9+2+2]=i;
				p8ctrl[7]=i;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
			//月电控跳闸输出状态bit-string(SIZE(8))
			//购电控跳闸输出状态bit-string(SIZE(8))
			//功控越限告警状态bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+2]=1<<(7-1);
					p8attr[6]=1;//未告警（0），告警（1）
				}
			//电控越限告警状态bit-string(SIZE(8))
			}
		}
	//bit0:时段功控
		p8ctrl=(u8*)ADDR_8103_4;//时段功控-总加组控制输出状态
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
		p8attr=(u8*)ADDR_8103_5;//时段功控-总加组告警输出状态
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//未告警（0），告警（1）
		if(Priority==0)//优先级
		{
			p8s=(u8*)ADDR_Terminal_kWCtrl4_State+(Pn*Terminal_Ctrl_Pn_Len);
			if(p8s[0]>=2)
			{
				Priority=1;//优先级
			//当前功控定值long64（单位：W 换算：-1）
				MR(ADDR_230x_17+(Pn*LENper_230x_17)+3,ADDR_Terminal_kWCtrl4_State+(Pn*Terminal_Ctrl_Pn_Len)+8,8);
			//当前功率下浮控浮动系数integer（单位：%）
			//功控跳闸输出状态bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
				p8d[2+9+2+2]=i;
				p8ctrl[7]=i;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
			//月电控跳闸输出状态bit-string(SIZE(8))
			//购电控跳闸输出状态bit-string(SIZE(8))
			//功控越限告警状态bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+2]=1<<(7-0);
					p8attr[6]=1;//未告警（0），告警（1）
				}
			//电控越限告警状态bit-string(SIZE(8))
			}
		}
		
	//bit0：月电控
		p8ctrl=(u8*)ADDR_8108_4;//月电控-总加组控制输出状态
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
		p8attr=(u8*)ADDR_8108_5;//月电控-总加组告警输出状态
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//未告警（0），告警（1）
		p8s=(u8*)ADDR_Terminal_kWhCtrl1_State+(Pn*Terminal_Ctrl_Pn_Len);
		//当前功控定值long64（单位：W 换算：-1）
		//当前功率下浮控浮动系数integer（单位：%）
		//功控跳闸输出状态bit-string(SIZE(8))
		//月电控跳闸输出状态bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
				p8d[2+9+2+3+2]=i;
				p8ctrl[7]=i;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
		//购电控跳闸输出状态bit-string(SIZE(8))
		//功控越限告警状态bit-string(SIZE(8))
		//电控越限告警状态bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+3+2]=1<<(7-0);
					p8attr[6]=1;//未告警（0），告警（1）
				}
	//bit1：购电控
		p8ctrl=(u8*)ADDR_8107_4;//购电控-总加组控制输出状态
		p8ctrl[0]=DataType_array;
		p8ctrl[1]=NUMmax_TAG;
		p8ctrl+=2+(Pn*LENper_8103_4);
		p8ctrl[0]=DataType_structure;
		p8ctrl[1]=2;
		p8ctrl[2]=DataType_OI;
		p8ctrl[3]=(0x2301+Pn)>>8;
		p8ctrl[4]=(0x2301+Pn);
		p8ctrl[5]=DataType_bit_string;
		p8ctrl[6]=8;
		p8ctrl[7]=0;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
		p8attr=(u8*)ADDR_8107_5;//购电控-总加组告警输出状态
		p8attr[0]=DataType_array;
		p8attr[1]=NUMmax_TAG;
		p8attr+=2+(Pn*LENper_8103_5);
		p8attr[0]=DataType_structure;
		p8attr[1]=2;
		p8attr[2]=DataType_OI;
		p8attr[3]=(0x2301+Pn)>>8;
		p8attr[4]=(0x2301+Pn);
		p8attr[5]=DataType_enum;
		p8attr[6]=0;//未告警（0），告警（1）
		p8s=(u8*)ADDR_Terminal_kWhCtrl2_State+(Pn*Terminal_Ctrl_Pn_Len);
		//当前功控定值long64（单位：W 换算：-1）
		//当前功率下浮控浮动系数integer（单位：%）
		//功控跳闸输出状态bit-string(SIZE(8))
		//月电控跳闸输出状态bit-string(SIZE(8))
		//购电控跳闸输出状态bit-string(SIZE(8))
				i=p8s[1];
				i=Bit_Inverse(i,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
				p8d[2+9+2+3+3+2]=i;
				p8ctrl[7]=i;///bit0~bit7对应1~8个开关的输出状态（0：未输出，1：输出）
		//功控越限告警状态bit-string(SIZE(8))
		//电控越限告警状态bit-string(SIZE(8))
				if(p8s[0]==100)
				{
					p8d[2+9+2+3+3+3+3+2]|=1<<(7-1);
					p8attr[6]=1;//未告警（0），告警（1）
				}
	}
}

//总加组控制设置状态∷=structure
//{
//时段控定值方案号	   unsigned，
//功控时段有效标志位  bit-string(SIZE(8))，
//功控状态            PCState，
//电控状态            ECState，
//功控轮次状态        TrunState，
//电控轮次状态        TrunState
//}
//时段控定值方案号：表示所投入的功控定值方案号。
//功控时段有效标志位：bit0～bit7按顺序对位表示1～8时段控投入的有效时段，置“1”：有效，置“0”：无效
//PCState∷=bit-string(SIZE(8))
//bit0～bit7按顺序对位表示；置“1”：投入，置“0”：解除；
//bit0：时段控
//bit1：厂休控
//bit2：营业报停控
//bit3：当前功率下浮控
//bit4～bit7：备用。
//ECState∷=bit-string(SIZE(8))
//bit0～bit7按顺序对位表示；置“1”：投入，置“0”：解除；
//bit0：月电控
//bit1：购电控
//bit2～bit7：备用
//TrunState∷=bit-string(SIZE(8))
//bit0～bit7按顺序对位表示1～8轮次开关的受控状态；置“1”：受控，置“0”：不受控。
void Contrl_SetState(u32 Pn)//总加组控制设置状态;入口:总加组号0-7
{
	u32 i;
	u32 n;
	u32 OI;
	u8* p8s;
	u8* p8d;
	
	p8d=(u8*)ADDR_230x_16+(Pn*LENper_230x_16);
	p8d[0]=DataType_structure;
	p8d[1]=6;
	p8d+=2;
//时段控定值方案号	   unsigned，
//功控时段有效标志位  bit-string(SIZE(8))，
	p8d[0]=DataType_unsigned;
	p8d[1]=0;
	p8d[2]=DataType_bit_string;
	p8d[3]=8;
	p8d[4]=0;
	MR(ADDR_DATABUFF,ADDR_8103_127,LENmax_8103_127);
	//找相应OI的方案切换
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
	}
	if(i<n)
	{//找到
		p8d[1]=p8s[11];//时段功控定值方案号
		p8d[4]=p8s[9];//功控时段有效标志位
	}
	p8d+=5;
//功控状态            PCState，
//PCState∷=bit-string(SIZE(8))
//bit0～bit7按顺序对位表示；置“1”：投入，置“0”：解除；
//bit0：时段控
//bit1：厂休控
//bit2：营业报停控
//bit3：当前功率下浮控
//bit4～bit7：备用。
	p8d[0]=DataType_bit_string;
	p8d[1]=8;
	p8d[2]=0;
	//时段控
	MR(ADDR_DATABUFF,ADDR_8103_3,LENmax_8103_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
	}
	if(i<n)
	{//找到
		if(p8s[6]==1)
		{
			p8d[2]|=1<<7;
		}
	}
	//厂休控
	MR(ADDR_DATABUFF,ADDR_8104_3,LENmax_8104_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
	}
	if(i<n)
	{//找到
		if(p8s[6]==1)
		{
			p8d[2]|=1<<6;
		}
	}
	//营业报停控
	MR(ADDR_DATABUFF,ADDR_8105_3,LENmax_8105_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
	}
	if(i<n)
	{//找到
		if(p8s[6]==1)
		{
			p8d[2]|=1<<5;
		}
	}
	//当前功率下浮控
	MR(ADDR_DATABUFF,ADDR_8106_3,LENmax_8106_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
	}
	if(i<n)
	{//找到
		if(p8s[6]==1)
		{
			p8d[2]|=1<<4;
		}
	}
	p8d+=3;
//电控状态            ECState，
	//ECState∷=bit-string(SIZE(8))
	//bit0～bit7按顺序对位表示；置“1”：投入，置“0”：解除；
	//bit0：月电控
	//bit1：购电控
	//bit2～bit7：备用
	p8d[0]=DataType_bit_string;
	p8d[1]=8;
	p8d[2]=0;
	//月电控
	MR(ADDR_DATABUFF,ADDR_8108_3,LENmax_8108_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
	}
	if(i<n)
	{//找到
		if(p8s[6]==1)
		{
			p8d[2]|=1<<7;
		}
	}
	//购电控
	MR(ADDR_DATABUFF,ADDR_8107_3,LENmax_8107_3);
	p8s=(u8*)ADDR_DATABUFF;
	n=p8s[1];
	if(n>NUMmax_TAG)
	{
		n=NUMmax_TAG;
	}
	p8s+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8s[3]<<8)+p8s[4];//总加组对象∷=OI
		if(OI==(0x2301+Pn))
		{
			break;
		}
		p8s+=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度
	}
	if(i<n)
	{//找到
		if(p8s[6]==1)
		{
			p8d[2]|=1<<6;
		}
	}
	p8d+=3;
//功控轮次状态        TrunState，
	//TrunState∷=bit-string(SIZE(8))
	//bit0～bit7按顺序对位表示1～8轮次开关的受控状态；置“1”：受控，置“0”：不受控。
	p8d[0]=DataType_bit_string;
	p8d[1]=8;
	i=MRR(ADDR_230x_14+(Pn*LENper_230x_14)+2,1);
	p8d[2]=i;
	p8d+=3;
//电控轮次状态        TrunState
	p8d[0]=DataType_bit_string;
	p8d[1]=8;
	i=MRR(ADDR_230x_15+(Pn*LENper_230x_14)+2,1);
	p8d[2]=i;
	
}

#endif



















