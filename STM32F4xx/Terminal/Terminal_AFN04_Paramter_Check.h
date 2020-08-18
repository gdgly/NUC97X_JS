

#ifndef Terminal_Paramter_Check_H
#define Terminal_Paramter_Check_H

void PnVSAFN04F10SetNoList(void);//�������Pn��ӦAFN04F10��������б�
u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
u32 Check_AFN04F10_Pn(u32 Pn);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
u32 Check_AFN04F11_Pn(u32 Pn);//����ն��������ò���ָ���Ĳ������;����0��,1��
u32 Check_AFN04F13_Pn(u32 Pn);//����ն˵�ѹ/����ģ�������ò���ָ���Ĳ������;����0��,1��
void AFN04F150_Computer(void);//��������Ч��־����
u32 PnSet(u32 Pn);//��Ч������ż��;����0=��Ч,1=��Ч
u32 Check_ACSamplePn(u32 Pn);//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
u32 ACSampleVoltageCurvePn(u32 Fn,u32 Pn);//��Ч���ɵ�ѹ���߲�����ż��;����0=��Ч,1=��Ч
u32 GroupTotalSet(u32 Pn);//��Ч�ܼ���ż��;����0=��Ч,1=��Ч
void Terminal_AFN0CF3_Computer(u32 Fn);//�ն���1����F3,�ն˲���״̬

void PnVSProtocolList(u32 ADDR_BUFF);//�������Pn��ӦAFN04F10����ͨ��Э���б�,�������ҷ������Ĭ��ͨ��Э��Ϊ30(DL645-2007)
void PnVSPORTProtocolList(u32 ADDR_BUFF);//�������Pn��ӦAFN04F10���ö˿ںź�ͨ��Э���б�,�������ҷ������Ĭ�϶˿ں�0ͨ��Э��Ϊ30(DL645-2007)
void PnVSSetnoList(u32 ADDR_BUFF);//�������Pn��ӦAFN04F10��������б�
u32 Get_SetTariff(u32 Pn);//ȡAFN04F10�����õķ�����

u32 F246MapPn(u32 Pn);//F246�����¼����ӳ��Pn;����0��ʾ��Ч
void F246PnList(void);//Ҫ������F246�����¼����Pn�б�


u32 Get_CurveSetFnListNo(u32 Fn);//�õ����߶����ܶȲ���Fn�б��;����:�б��0-n,0xffffffff��ʾ��

#endif
