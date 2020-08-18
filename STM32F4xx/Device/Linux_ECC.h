

#ifndef Linux_ECC_H
#define Linux_ECC_H


void NAND_Calculate_ECC(const unsigned char *buf, unsigned int eccsize,unsigned char *code);
int NAND_Correct_Data(unsigned char *buf,unsigned char *read_ecc, unsigned char *calc_ecc,unsigned int eccsize);


#endif

