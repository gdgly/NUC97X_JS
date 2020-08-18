/* nand_lld_def.h*/
/**************************************************************************
 * Copyright (C)2013 Spansion LLC. All Rights Reserved .
 *
 * This software is owned and published by:
 * Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software constitutes driver source code for use in programming Spansion's
 * Flash memory components. This software is licensed by Spansion to be adapted only
 * for use in systems utilizing Spansion's Flash memories. Spansion is not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein.  Spansion is providing this source code "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the source code herein.
 *
 * SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED
 * USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY,
 * FITNESS FOR A  PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.  SPANSION WILL
 * HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR
 * OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS,
 * EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Copyright notice must be included with
 * this software, whether used in part or whole, at all times.
 */
//nand_lld_ecc.h

#ifndef Spansion_ECC_H
#define Spansion_ECC_H




#ifdef __cplusplus /* allow #include in a c++ file */
extern "C" {
#endif

#define START_ADDR_ECC 0x9
#define MAIN_AREA_ONLY

#ifdef MAIN_AREA_ONLY
#define DATA_LENGTH_BYTE  512
#define DATA_LENGTH_INT   129
#define TOTAL_LENGTH_BYTE 524    // 512 + 12 instead of 9, 3 bytes reserved, no ECC
#else
#define DATA_LENGTH_BYTE  516
#define DATA_LENGTH_INT   130
#define TOTAL_LENGTH_BYTE 528    // 516 + 12 instead of 9, 3 bytes reserved, no ECC
#endif

#define PARA_LENGTH_BYTE  9
#define GF_SIZE           8191
#define ERR_NUMBER        4
#define ERR_NUMBER_DBL    8


#define ECC_MODE_DETECT_ONLY     0
#define ECC_MODE_FIX             1



typedef enum nand_lld_status
{
    NLLD_SUCCESS,
    NLLD_FAILURE,
    NLLD_ERR_EDC,
    NLLD_NOT_SUPPORTED,
    NLLD_TIMEOUT,
    NLLD_WRONG_ARG,
    NLLD_BAD_BLOCK,
    NLLD_NO_MEM,
    NLLD_ECC_ERROR_NONE,                     /* no error             */
    NLLD_ECC_ERROR_CORRECTABLE,              /* one bit data error   */
    NLLD_ECC_ERROR_PARITY,                   /* one bit parity error */
    NLLD_ECC_ERROR_UNCORRECTABLE,             /* uncorrectable error  */
    NLLD_BCH_NO_ERROR,
    NLLD_BCH_ERROR_DETECTED,
    NLLD_BCH_ERROR_CORRECTED,
    NLLD_BCH_ERROR_NOT_CORRECTABLE
}nand_lld_status;


//#ifdef NLLD_HAMMING_ECC

u8 cal32bParity(u32 data);

// Description: Calculate the ECC codes for "length" bytes of data
// length should not be more than 528 and should be a multiple of 4
void nlld_calEcc(u8 *data, u32 length, u8 ecc_code[2]);//4-528字节(4的整数倍)数据,1Bit_ECC计算,ecc_code为2Bytes


//ECC correction function for "length" bytes of data,with ECC read and ECC calculated
//length should not be more than 528 and should be a multiple of 4
// mode: ECC_MODE_DETECT_ONLY  detect error only, 
//       ECC_MODE_FIX detect error and fix error

//nand_lld_status  nlld_correctUseEcc(u8 *data, u32 length, u8 *origECC, u8 *newECC,
//        u16* loc,  u8 mode);
nand_lld_status  nlld_correctUseEcc(u8 *data, u32 length, u8 *origECC, u8 *newECC,u8 mode);//mode=ECC_MODE_DETECT_ONLY仅检测,=ECC_MODE_FIX检测和修正

//#endif

//#ifdef NLLD_BCH_ECC

void nlld_bch_encode(u8 *dataPtr);


/* This is the Spansion BCH decoding function.
    dataPtr: pointing to 528 bytes of memory
             516 bytes data, 9 bytes parity, 3 padding bytes 
    mode: ECC_MODE_DETECT_ONLY  detect error only, 
          ECC_MODE_FIX detect error and fix error  */
nand_lld_status nlld_bch_decode(u8 *dataPtr, u32 *errNum, u8 mode);
//#endif


#ifdef __cplusplus /* allow #include in a c++ file */
}
#endif
#endif //_NAND_LLD_ECC_H
