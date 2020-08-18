

#ifndef NAND_H
#define NAND_H



void Init_NAND(void);//��λnand
u64 ID_NAND(void);//������ID(1Gb*8=0x01F1801Dxx,1Gb*16=0x01C1805Dxx)
u32 Wait_NAND(u32 usMax);//��NAND״̬�Ĵ����ȴ���ǰ�������,����:0=��ȷ,!=0��ȡ��״̬��
void Enable_Internal_ECC(void);//����nand�ڲ�ECC
void Disable_Internal_ECC(void);//ȡ��nand�ڲ�ECC


u32 Read_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//NAND_FLASH û�滻��
void BlockErase_NAND(u32 ADDR_Dest,u32 Busy_Wait);//NAND_FLASH ������ǰ1����;û���滻;���:Busy_Wait=1NANDæʱ�ȴ�Busy_Wait=0���ȴ�
u32 BLOCK_Write_NAND(u32 ADDR_Dest);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����;û���滻


u32 RePlace_BLOCK_ADDR(u32 ADDR);//ȡ�滻���ַ
u32 RePlace_Read_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//NAND_FLASH ���滻��;����:0=û����,1=1λ����,2=�޷�У����2λ���ϴ���
void RePlace_Write_NAND(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//NAND_FLASHд;����������滻
void RePlace_Clr_NAND(u32 Data,u32 ADDR_Dest,u32 Byte);//NAND_FLASH���;����������滻
void RePlace_BlockErase_NAND(u32 ADDR_Dest,u32 Byte);//NAND_FLASH �����;�п��滻


u32 Check_4kNandBuff(void);//���4K(BKPSRAM) NAND FLASH��д����,����Ч����0;����:0=����,1=��ȷ
void NR_4kBuff(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//��NAND_FLASH��д�뻺��
void NB_4kBuff(u32 ADDR_Dest,u32 Byte);//����NAND_FLASHʱͬʱ����д�뻺��
void NW_4kBuff(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//д��Nand FLASH�������ȵ�4kBuff
void NC_4kBuff(u32 Data,u32 ADDR_Dest,u32 Byte);//���д��Nand FLASH�������ȵ�4kBuff
void NAND4kBuff_Write_NAND(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//��ͬ4K(BKPSRAM)��������д��NAND FLASH

u32 Check_128kNandBuff(void);//���128K(BKPSDRAM) NAND FLASH��д����,����Ч����0;����:0=����,1=��ȷ
void NR_128kBuff(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//��NAND_FLASH��д�뻺��
void NB_128kBuff(u32 ADDR_Dest,u32 Byte);//����NAND_FLASHʱͬʱ����д�뻺��
void NW_128kBuff(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//д��Nand FLASH�������ȵ�128kBuff
void NC_128kBuff(u32 Data,u32 ADDR_Dest,u32 Byte);//���д��Nand FLASH�������ȵ�128kBuff
void NAND128kBuff_Write_NAND(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//��ͬ128K(BKPSDRAM)��������д��NAND FLASH

#endif




