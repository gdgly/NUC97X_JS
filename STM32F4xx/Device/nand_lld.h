/* nand_lld.h */
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
//nand_lld.h
//Header file for NAND LLD

#ifndef __NAND_LLD_H
#define __NAND_LLD_H

#ifdef __cplusplus /* allow #include in a c++ file */
extern "C" {
#endif

#include "nand_lld_def.h"


typedef struct nand_lld_struct
{
    u32 memAllocFlag;
    u32 baseAddr;
    u8 * cmdAddr;
    void * addrAddr;
    void * dataAddr;
    nand_dev_type type;
    nand_geometry_info geoInfo;
    nand_timing *pTiming;    //device timing table
} nand_lld;



#define IS_A_PAGE_ADDR(x)    ((x% (pDev->geoInfo.pageSize)) == 0)
#define IS_A_BLOCK_ADDR(x)   ((x% (pDev->geoInfo.pagesPerBlock)) == 0)
#define IS_MULTI_PLANE_ADDR(x1, x2, pagesPerBlock)   ( ((x1/(pagesPerBlock)) >> 1) ==  ((x2/(pagesPerBlock)) >> 1) &&   \
        ((x1/(pagesPerBlock)) & 1) == 0 &&      \
        ((x2/(pagesPerBlock)) & 1) == 1  )

#define IS_SAME_PLANE_ADDR(x1, x2)   ( ((x1/pDev->geoInfo.pagesPerBlock)&0x01) == ((x2/pDev->geoInfo.pagesPerBlock)&0x01))

/******************************************************************************
 *
 * Description: Get NAND LLD Version
 * Parameter(s)
 *    u8 verStr[NAND_LLD_VERSION_STRLEN]:   Return of Nand LLD version string
                   The size of the buffer should be at lease 21 bytes.
 * Return:  function call status
 ********************************************************************************/
nand_lld_status nlld_getVersion(u8 verStr[NAND_LLD_VERSION_STRLEN]);  



/******************************************************************************
 *
 * Description: Create a NAND LLD instance with/without given mem block
 * Parameter(s)
 *      u8* pMem:  Pointer to memory to be used by Nand LLD
 *      u32 memSize: Size of the memory passed in bytes
 *             if pMem is NULL, then no memory is passed by user and NAND LLD will
 *             allocate memory dynamically.
 * Return:
 *      void *:  pointer to a NAND LLD instance.
 *                 a NULL pointer will be returned if no NAND LLD instance is created
 ********************************************************************************/
void * nlld_createInstance(u8* pMem, u32 memSize);

/******************************************************************************
 *
 * Function Description:  Function to probe device connected on the port
 *
 * Parameters:
 *  void *pDevice:   device to probe
 *                       Retrieved parameters (pages per block, page main size,
                        page spare size, total blocks and data organization
                        are stored in the structure.
 *
 * RETURNS:
      Function call status:  NLLD_SUCCESS  -  Nand Device Found
                             NLLD_FAILURE  -  Nand Device Not Found
 ********************************************************************************/
nand_lld_status nlld_probeDevice(void *pDevice);
/******************************************************************************
 *
 * Description: Return heap size required by creating a Nand LLD instance.
 * Parameter(s)
 *      u8* pMemSize:  Return of heap size required by NandLLD
 * Return:  function call status
 ********************************************************************************/
nand_lld_status nlld_getHeapSize(u32* pMemSize);


/******************************************************************************
 *
 * Description: Remove NAND LLD instance
 * Parameter(s)
 *      void *pDevice:  Pointer to a NAND LLD instance
 *
 * Return:  function call status
 ********************************************************************************/
nand_lld_status nlld_removeInstance(void *pDevice);


/******************************************************************************
 *
 * Description: Get Device Geo Info
 * Parameter(s)
 *      void *pDevice:  Pointer to a NAND LLD instance
 *      nand_geometry_info* pGeoInf:  Return of Device Geo Info
 *
 * Return:  function call status
 ********************************************************************************/
nand_lld_status nlld_getGeoInfo(void *pDevice, nand_geometry_info* pGeoInf); 


/******************************************************************************
 *
 * Function Description:  Get Device Type info
 *
 * Parameters:
 *        void *pDevice:  Device to Operate
 *        nand_dev_type* pDevTypeInfo: return of device type info
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_getDeviceTypeInfo(void *pDevice, nand_dev_type* pDevTypeInfo);

/******************************************************************************
 *
 * Function Description: Read device status
 * Parameter(s)
 *     void *pDevice:  Pointer to a NAND LLD instance, which should represent a
 *           physical NAND device it is interfacing with. Therefore, in the rest of
 *           release, NAND LLD instance will also be called  "Device" directly.
 *           The status will be read from the device.
 *   u16* status:    Return of device status
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readStatus(void *pDevice, u16* status);


/******************************************************************************
 *
 * Description:  Read enhanced status register
 *               Read the status value from a previous operation in the following cases:
 *             1.  in the case of concurrent operations on a specific die of a multi-die stack,
 *                    such as 2Gb stacked to form 4Gb, 4Gb stacked to form 8Gb etc.
 *             2.  in the case of multi-plane operations on a specific plane in the same die
 * Parameter(s)
 *   void *pDevice: Device to
 *   u32 RowAddr:  Device address to read status
 *   u16* status:    Return of extended status
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readStatusEnhanced(void *pDevice, u32 RowAddr, u16* status);



/******************************************************************************
 *
 * Function Description: Read EDC Status Register
 *               This operation is available only in copy back program and it allows
 *               the detection of erros during read for copy back. In the case of
 *               multiple plane copy back, it is not possible to know which of the two
 *               read operaitons caused error.
 * Parameter(s)
 *     void *pDevice:  Pointer to a NAND LLD instance
 *     u16* status:    Return of device EDC status
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readEDCStatus(void *pDevice, u16* status);



/******************************************************************************
 *
 * Function Description:  Reset the device
 * Parameter(s)
 *    void *pDevice: Device to Reset
 *    nand_lld_api_type: CMD or OP
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status  nlld_reset(void *pDevice, nand_lld_api_type type);  


/******************************************************************************
 *
 * Description:  Read a NAND device Page
 * Parameters:
 *       void *pDevice: Device to Read
 *       READ_PAGE_MODE mode: define how to read a page
 *               READ_PAGE       Read a page (main+spare)
 *               READ_SPARE      Read spare
 *               READ_MAIN       Read main
 *       dev_address  pageAddr:   page address which should point to the beginning of the page
 *       u8   pPageBuf:   Output page buffer
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readPage(void *pDevice, 
        READ_PAGE_MODE mode,
        dev_address  pageAddr,
        u8   pPageBuf[]);



/******************************************************************************
 *
 * Description:  Read Partial Page function.
 *                It partially program a page and can't corss pages.
 *
 * Parameters
 *       void *pDevice:  Device to Operate
 *       dev_address addr:  Device address to read
 *       u32 bytes:   Number of bytes to read
 *       u8* pBuf:    Pointer to data buffer to receive data.
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readPartialPage(void *pDevice, 
        dev_address addr,
        u32 bytes,
        u8* pBuf);



/******************************************************************************
 *
 * Description:  Program a Nand Device Page
 * Parameters:
 *       void *pDevice: Device to Program
 *       nand_lld_api_type: CMD or OP
 *       PROGRAM_PAGE_MODE mode: define how to Program a page
 *               PROGRAM_PAGE        Program a Page (main+spare)
 *               PROGRAM_SPARE       Program Spare
 *               PROGRAM_MAIN        Program Main
 *       dev_address pageAddr: Page address pointing to the beginning of a page
 *       u8 pPageBuf: Output page buffer
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_programPage( void *pDevice, 
        nand_lld_api_type type,
        PROGRAM_PAGE_MODE mode,
        dev_address pageAddr,
        u8 pPageBuf[]);


/******************************************************************************
 *
 * Description:  Erase a NAND device Block
 * Parameters
 *       void *pDevice:   Device to erase
 *       nand_lld_api_type: CMD or OP
 *       u32 RowAddr:     Row address  which points to the beginning of a block
 * Return: function call status
 ********************************************************************************/
nand_lld_status  nlld_eraseBlock(void *pDevice, 
        nand_lld_api_type type,
        u32 RowAddr);




/******************************************************************************
 *
 * Description:  Read Nand Device ID
 * Parameters
 *       void *pDevice:     Device to Operate
 *       u8  ids[5]:    output of the device IDs, ONFI device ID is defined as 5 bytes
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readID(void *pDevice, u8 ids[5]); //read ONFI ID


/******************************************************************************
 *
 * Description:  Read Nand Device ID2 using alternate identificaiotn mode
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *       u8  ids[5]:    output of the device IDs, ONFI device ID is defined as 5 bytes
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readID2(void *pDevice, u8 ids[5]); //read 



/******************************************************************************
 *
 * Description:  Enter One-Time-Programmable (OPT) area
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_enterOTPArea(void *pDevice);



/******************************************************************************
 *
 * Description:  Exit One-Time-Programmable (OPT) area
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_exitOTPArea(void *pDevice);


/******************************************************************************
 *
 * Description:  Read ONFI parameter page
 * Parameters
 *   void *pDevice: Device to Operate
 *   void* pPageBuf:     Output of the parameter page, buffer need to be page(main+spare) sized
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readParameterPage(void *pDevice, void* pPageBuf);


/******************************************************************************
 *
 * Description:  Read ONFI Signature
 * Parameters
 *   void *pDevice: Device to Operate
 *   u8 ids[4]:  Output of ONFI signature
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readONFISignature(void *pDevice, u8 ids[4]);

/******************************************************************************
 *
 * Description:  Read device using page random data output/ read column command,
 *                 read directly from a page offset, read can't cross pages
 *                 Page Buffer must be loaded before calling this function
 *
 * Parameters
        void *pDevice:  Device to Operate
 *       u32 colAddr:  Device page offset address, start from the beginning of the page
 *       u8* pReadBuf:  Output buffer, need to be readBytes sized
 *       u32 ReadBytes:  Number of bytes to read
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_randomDataOutput(void *pDevice, 
        u32 colAddr ,
        u8* pReadBuf,
        u32 readBytes);

/******************************************************************************
 *
 * Description:   Program device using Random Data Input/Change Write Column command,
 *                 program directly to a page column address, program can't cross pages.
 *                 nlld_randomDataInputStart must be called before calling this function
 *
 * Parameters
        void *pDevice:  Device to Operate
 *       dev_address progAddr:  Device page  address, start from the beginning of the page
 *       u8* pProgramBuf:  Program buffer, need to be writeBytes sized
 *       u32 writeBytes:  Number of bytes to program
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_randomDataInputStart(void *pDevice,
        dev_address progAddr ,
        u8* pProgramBuf,
        u32 writeBytes);

/******************************************************************************
 *
 * Description:   Program devcie using Random Data Input/Change Write Column command,
 *                 program directly to a page column address, program can't cross pages.
 *                 Full page address must be written before calling this funcntion
 *
 * Parameters
 *       void *pDevice:  Device to Operate
 *       u32 colAddr:  Device page offset address, start from the beginning of the page
 *       u8* pProgramBuf:  Program buffer, need to be writeBytes sized
 *       u32 writeBytes:  Number of bytes to program
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_randomDataInput(void *pDevice, 
        u32 colAddr ,
        u8* pProgramBuf,
        u32 writeBytes);



/******************************************************************************
 *
 * Description:   Program device using Write confirm command,
 *                 program directly to a page column address, program can't cross pages.
 *
 *
 * Parameters
        void *pDevice:  Device to Operate
 *       nand_lld_api_type:  CMD or OP
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_programConfirm(void *pDevice,
        nand_lld_api_type type);



/******************************************************************************
 *
 * Description:  Partial Page Programming function.
 *                It partially program a page and can't corss pages.
 *
 * Parameters
 *       void *pDevice:  Device to Operate
 *       nand_lld_api_type: CMD or OP
 *       dev_address addr:  Device address to program
 *       u16 bytes:   Number of byptes to program
 *       u8* pBuf:    Pointer to data buffer to program.
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_programPartialPage(void *pDevice, 
        nand_lld_api_type type,
        dev_address addr,
        u32 bytes,
        u8* pBuf);



/******************************************************************************
 *
 * Description:  Copy Back Page Read function
 *              It is intended to qucikly and efficiently rewrite data stored in one page without
 *              utilizing external memory.
               This function should be followed by nlld_pageCopyBackProgram function. 
 * Parameters
 *       void *pDevice:  Device to Operate
 *       dev_address sourcePageAddress:  Device address to copy from, must be a page address
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_pageCopyBackRead( void *pDevice,
        nand_lld_api_type type,
        dev_address sourcePageAddr
);



/******************************************************************************
 *
 * Description:  Copy Back Page Special Read function
 *              The Copy Back Read from target pages will be executed with an increased internal voltage.
 *              The special feature shall be used only if ECC read errors have occurred in the source page
 *              using Page Read or Copy Back Read sequences.
 * Parameters
 *       void *pDevice:  Device to Operate
 *       nand_lld_api_type type:  CMD or OP
 *       dev_address sourcePageAddress:  Device address to copy from, must be a page address
 *                         the pointer should be NULL.
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_pageCopyBackSpecialRead( void *pDevice,
        nand_lld_api_type type,
        dev_address sourcePageAddr
);


/******************************************************************************
 *
 * Description:  Copy Back Page Program function
 *              It is intended to quickly and efficiently rewrite data stored in one page without
 *              utilizing external memory.
 *              This function should be called after nlld_pageCopyBackRead function.
 *
 * Parameters
 *       void *pDevice:  Device to Operate
 *       nand_lld_api_type type:  CMD or OP
 *       nand_lld_status_reg_type regType    NLLD_STATUS_REG or NLLD_STATUS_EDC_REG
 *       dev_address sourcePageAddress:   Source address to read from, must be a page address
 *       dev_address destPageAddress:    Device address to copy to, must be a page address
 *       sourcePageAddress and   destPageAddress should be on the same plane
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_pageCopyBackProgram( void *pDevice, 
        nand_lld_api_type type,
        nand_lld_status_reg_type regType,
        dev_address sourcePageAddress,
        dev_address destPageAddress
);

/******************************************************************************
 *
 * Description:  Copy Back Page Program with random input function
 *              It is intended to quickly and efficiently rewrite data stored in one page without
 *              utilizing external memory.
 *              This function should be called after nlld_pageCopyBackRead function ,and can following
 *              nlld_randomDataInput function and end with nlld_programConfirm function.
 *
 * Parameters
 *       void *pDevice:  Device to Operate
  *      dev_address sourcePageAddress:   Source address to read from, must be a page address
 *       dev_address destPageAddress:    Device address to copy to, must be a page address
 *       u8* pProgramBuf:  Program buffer, need to be writeBytes sized
 *       u32 writeBytes:  Number of bytes to program
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_pageCopyBackRandomInputProgramStart( void *pDevice,
        dev_address sourcePageAddress,
        dev_address destPageAddress,
        u32 writeBytes,
        u8* pProgramBuf
);

/******************************************************************************
 *
 * Description:  RePrograms a page
 *              Page Program may result in a fail, which can be detected by Read Status Register. In
 *              this event, the host may call Page Re-Program. This command allows the reprogramming of
 *              the same patter of the last (failed) page into antoher memory location.
 * Parameters
 *       void *pDevice:     Device to Operate
 *       nand_lld_api_type:  CMD or OP
 *       dev_address pageAddr:     device address to reprogram to, must be a page Address.
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_reProgramPage(void *pDevice,
        nand_lld_api_type type,
        dev_address pageAddr
);



/******************************************************************************
 *
 * Description:  Read Cache Start
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *       dev_address startPageAddr:  the Page Address to start cache read
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readCacheStart(void *pDevice, dev_address startPageAddr);

/******************************************************************************
 *
 * Description:  Read Cache  (Continue)
 *               Sequential Read
 * Parameters
 *       void *pDevice:     Device to Operate
 *       u8* readBuf:   Pointer to buffer for receiving read data
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readCache(void *pDevice, u8* readBuf);


/******************************************************************************
 *
 * Description:  Read Cache Enhanced  (Continue)
 *               Random Read
 * Parameters
 *       void *pDevice:     Device to Operate
 *       dev_address nextPageAdr:   Page Address for the next read
 *       u8* readBuf:   Pointer to buffer for receiving read data
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readCacheEnhanced(void *pDevice, dev_address nextPageAdr, u8* readBuf);



/******************************************************************************
 *
 * Description:  Read Cache (Enhanced) End
 *               End Sequential/Random Cache Read
 * Parameters
 *       void *pDevice:     Device to Operate
 *       u8* readBuf:   Pointer to buffer for receiving last page of read data
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_readCacheEnd(void *pDevice, u8* readBuf);


/******************************************************************************
 *
 * Description:  Cache Program Start/Continue
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *       nand_lld_api_type type:  CMD or OP
 *       dev_address progAddr:    Device address to program
 *       u32 bytes:       Size of data to program
 *       u8* progBuf:   Pointer to data buffer for programming
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_cacheProgramStartOrContinue(void *pDevice, 
        nand_lld_api_type type,
        dev_address progAddr,
        u32 bytes,
        u8* progBuf);

/******************************************************************************
 *
 * Description:  Cache Program End
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *       nand_lld_api_type type:  CMD or OP
 *       dev_address lastProgAddr:    device address to program
 *       u32 bytes:       Size of data to program
 *       u8* progBuf:   Pointer to data buffer for programming
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_cacheProgramEnd(void *pDevice, 
        nand_lld_api_type type,
        dev_address lastProgAddr,
        u32 bytes,
        u8* buf
);


/******************************************************************************
 *
 * Description:  Start/Continue Multiplane Cache Program
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *       nand_lld_api_type type:  CMD or OP
 *       dev_address addr[],:     Program addresses of first plane and second plane
 *       u32 bytes[]:     Program sizes for first plane and second plane cache program
 *       u8* progBuf[]:   Pointer to data buffers for first plane and second plane cache program
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_multiPlaneCacheProgStartOrCont(void *pDevice, 
        nand_lld_api_type type,
        dev_address addr[2],
        u32 bytes[2],
        u8* progBuf[2]
);


/******************************************************************************
 *
 * Description:  ONFI Start/Continue Multiplane Cache Program
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *       nand_lld_api_type type:  CMD or OP
 *       dev_address addr[],:     Program addresses of first plane and second plane
 *       u32 bytes[]:     Program sizes for first plane and second plane cache program
 *       u8* progBuf[]:   Pointer to data buffers for first plane and second plane cache program
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_ONFI_multiPlaneCacheProgStartOrCont(void *pDevice, 
        nand_lld_api_type type,
        dev_address addr[2],
        u32 bytes[2],
        u8* progBuf[2]
);

/******************************************************************************
 *
 * Description:  End Multiplane Cache Program
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *       nand_lld_api_type type:  CMD or OP
 *       dev_address addr[],:     Program addresses of first plane and second plane
 *       u32 bytes[]:     Program sizes for first plane and second plane cache program
 *       u8* progBuf[]:   Pointer to data buffers for first plane and second plane cache program
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_multiPlaneCacheProgEnd(void *pDevice,
        nand_lld_api_type type,
        dev_address addr[2],
        u32 bytes[2],
        u8* progBuf[2]
);



/******************************************************************************
 *
 * Description:  ONFI End Multiplane Cache Program
 *
 * Parameters
 *       void *pDevice:     Device to Operate
 *       nand_lld_api_type type:  CMD or OP
 *       dev_address addr[],:     Program addresses of first plane and second plane
 *       u32 bytes[]:     Program sizes for first plane and second plane cache program
 *       u8* progBuf[]:   Pointer to data buffers for first plane and second plane cache program
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_ONFI_multiPlaneCacheProgEnd(void *pDevice,
        nand_lld_api_type type,
        dev_address addr[2],
        u32 bytes[2],
        u8* progBuf[2]
);



/******************************************************************************
 *
 * Function Description:  Checks if a block is marked bad
 *
 * Parameter pDevice:     Device to Operate
 * Parameter RowAddr:     Row address which points to the beginning of a block
 *
 *
 * RETURNS:               NLLD_SUCCESS if the block is not marked bad
 ********************************************************************************/
nand_lld_status nlld_checkBadBlock(void *pDevice, u32 RowAddr);



/******************************************************************************
 *
 * Function Description:  Marks a bad block
 *
 * Parameter pDevice:  Device to Operate
 * Parameter blockNo:  Block number of a device, start at 0
 *
 *
 * Return: function call status
 ********************************************************************************/
//nand_lld_status nlld_markBadBlock(void *pDevice, u16 blockNo);




/******************************************************************************
 *
 * Function Description:  multi-Plane Program Page
 *
 * Parameters:
 *        void *pDevice:  Device to Operate
 *        nand_lld_api_type:  CMD or OP
 *        dev_address pageAddr[2]:  Array of device addresses to program for Plane 1 and Plane 2
 *        u32 bytes[2]:   Array of data sizes to program for Plane 1 and Plane 2
 *        u8* pBuf[2]:   Array of data buffers to program for Plane 1 and Plane 2
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_multiPlaneProgram(void *pDevice, 
        nand_lld_api_type type,
        dev_address pageAddr[2],
        u32 bytes[2],
        u8* pBuf[2]
);

/******************************************************************************
 *
 * Function Description:  ONFI multi-Plane Program Page
 *
 * Parameters:
 *        void *pDevice:  Device to Operate
 *        nand_lld_api_type:  CMD or OP
 *        dev_address pageAddr[2]:  Array of device addresses to program for Plane 1 and Plane 2
 *        u32 bytes[2]:   Array of data sizes to program for Plane 1 and Plane 2
 *        u8* pBuf[2]:   Array of data buffers to program for Plane 1 and Plane 2
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_ONFI_multiPlaneProgram(void *pDevice, 
        nand_lld_api_type type,
        dev_address pageAddr[2],
        u32 bytes[2],
        u8* pBuf[2]
);






/******************************************************************************
 *
 * Function Description:   Multi-plane Read Pages for Copy Back Program
 *
 * Parameters:
 *        void *pDevice:  Device to Operate
 *        nand_lld_api_type: CMD or OP
 *        dev_address sourcePageAddr[]:  Source Page Addresses of First Plane and Second Plane to Read
 *        dev_address destPageAddr[]:  Destination page Addresses of First Plane and Second Plane to Program
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_multiPlaneCopyBackProgram( void *pDevice, 
        nand_lld_api_type type,
        dev_address sourcePageAddr[2],
        dev_address destPageAddr[2]
);



/******************************************************************************
 *
 * Function Description: ONFI  Multi-plane Read Pages for Copy Back Program
 *
 * Parameters:
 *        void *pDevice:  Device to Operate
 *        nand_lld_api_type: CMD or OP
 *        dev_address sourcePageAddr[]:  Source Page Addresses of First Plane and Second Plane to Read
 *        dev_address destPageAddr[]:  Destination page Addresses of First Plane and Second Plane to Program
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_ONFI_multiPlaneCopyBackProgram( void *pDevice, 
        nand_lld_api_type type,
        dev_address sourcePageAddr[2],
        dev_address destPageAddr[2]
);


/*****************************************************************************************************
 * Description:  Multi-plane Page Re-Program  allows two Page Re-program in Parallel
 *              two pages in parallel,
 *              Multi-plane Page Program may result in a fail, which can be detected by Read Status Register. In
 *              this event, the host may call Multi-plan Page Re-Program.
 * Parameters
 *       void *pDevice: Device to Operate
 *       nand_lld_api_type: CMD or OP
 *       dev_address pageAddr1: A Block Address on Plane 1. It points to beginning of the block.
 *       dev_address pageAddr2: A Block Address on Plane 2. It points to beginning of the block.
 * Return: function call status
 **********************************************************************************************************/
nand_lld_status nlld_multiPlaneReProgramPage(void *pDevice, 
        nand_lld_api_type type,
        dev_address pageAddr1,
        dev_address pageAddr2
);


/******************************************************************************
 *
 * Function Description:  erase Multple Plane blocks
 *
 * Parameters:
 *        void *pDevice:  Device to Operate
 *        nand_lld_api_type: CMD or OP
 *        u32 RowAddr1:  RowAddr of a block on Plane 1. It should point to the beginning of the block
 *        u32 RowAddr2:  RowAddr of a block on Plane 2. It should point to the beginning of the block
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_multiPlaneEraseBlocks(void *pDevice, 
        nand_lld_api_type type,
        u32 RowAddr1,
        u32 RowAddr2
);


/******************************************************************************
 *
 * Function Description:  ONFI erase Multple Plane blocks
 *
 * Parameters:
 *        void *pDevice:  Device to Operate
 *        nand_lld_api_type: CMD or OP
 *        u32 RowAddr1:  RowAddr of a block  on Plane 1. It should point to the beginning of the block
 *        u32 RowAddr2:  RowAddr of a block  on Plane 2. It should point to the beginning of the block
 *
 * Return: function call status
 ********************************************************************************/
nand_lld_status nlld_ONFI_multiPlaneEraseBlocks(void *pDevice, 
        nand_lld_api_type type,
        u32 RowAddr1,
        u32 RowAddr2
);

#ifdef __cplusplus /* allow #include in a c++ file */
}
#endif

#endif //  __NAND_LLD_H
