#ifndef __ROM_H__
#define __ROM_H__

#include "patch.h"

#define ROM_VERSION_2_2
/*
#include "aes.h"
#include "csp.h"
#include "debug.h"
#include "dma.h"
#include "flash_vendor.h"
#include "gpio.h"
#include "hw_aes.h"
#include "hw_csp.h"
#include "hw_dma.h"
#include "hw_gpio.h"
#include "hw_i2c.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_nvic.h"
#include "hw_prcm.h"
#include "hw_qspi_flash.h"
#include "hw_sema.h"
#include "hw_spi.h"
#include "hw_timer.h"
#include "hw_types.h"
#include "hw_uart.h"
#include "hw_utc.h"
#include "hw_watchdog.h"
#include "i2c.h"
#include "interrupt.h"
#include "prcm.h"
#include "qspi_flash.h"
#include "sema.h"
#include "spi.h"
#include "systick.h"
#include "timer.h"
#include "uart.h"
#include "utc.h"
#include "watchdog.h"
#include "mcnt.h"
#include "rf_drv.h"
*/
#ifdef ROM_VERSION_1_1
#define ROM_AESKeyLenSet                             ((void (*)(unsigned long ulKeyLenMode))0x00003a51)
#define ROM_AESKeySet                                ((void (*)(unsigned char ucOffset, unsigned long ulKeyValue))0x00003a61)
#define ROM_AESIVSet                                 ((void (*)(unsigned char ucOffset, unsigned long ucIV))0x00003a69)
#define ROM_AESAADLen0Set                            ((void (*)(unsigned long ulAadLen0))0x00003a71)
#define ROM_AESAADLen1Set                            ((void (*)(unsigned long ulAadLen1))0x00003a77)
#define ROM_AESPayloadLenSet                         ((void (*)(unsigned long ulPayloadLenByte))0x00003a7d)
#define ROM_AESModeSet                               ((void (*)(unsigned long ucAESMode))0x00003a83)
#define ROM_AESEncDecSet                             ((void (*)(unsigned long ucAESEncDec))0x00003a93)
#define ROM_AESDMAEn                                 ((void (*)(void))0x00003aa3)
#define ROM_AESDMADis                                ((void (*)(void))0x00003ab1)
#define ROM_AESBlockDataInput                        ((void (*)(unsigned char ucOffset, unsigned long ulDataIn))0x00003abf)
#define ROM_AESBlockDataOutput                       ((unsigned long (*)(unsigned char ucOffset))0x00003ac7)
#define ROM_AESBlockStart                            ((void (*)(void))0x00003acf)
#define ROM_AESKeyLoadWaitDone                       ((void (*)(void))0x00003add)
#define ROM_AESBlockTransWaitDone                    ((void (*)(void))0x00003af9)
#define ROM_AESClockDiv2En                           ((void (*)(void))0x00003b09)
#define ROM_AESClockDiv2Dis                          ((void (*)(void))0x00003b17)
#define ROM_AESCCMAuthLenSet                         ((void (*)(unsigned long ucAuthLenByte))0x00003b25)
#define ROM_AESCCMLengthLenSet                       ((void (*)(unsigned char ucLengthLenByte))0x00003b37)
#define ROM_AESTagGet                                ((unsigned long (*)(unsigned char ucOffset))0x00003b49)
#define ROM_AESBlock                                 ((void (*)(unsigned char *pucInput, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput))0x00003b53)
#define ROM_AESECB                                   ((void (*)(unsigned char *pucInput, unsigned long ulInputLenByte, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput))0x00003ceb)
#define ROM_AESCCM                                   ((void (*)(unsigned char *aad, unsigned long aad_len_byte, unsigned char *input_message, unsigned long input_len_byte, unsigned long *user_key, unsigned long key_len_bits, unsigned long M, unsigned long L, unsigned char *nonce, unsigned char mode, unsigned char *output_message, unsigned char *digest))0x00003e9d)
#define ROM_AES_128_EEA2                             ((void (*)(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucOutMsg))0x00004173)
#define ROM_AES_CMAC_XOR_128                         ((void (*)(unsigned char *pucA, const unsigned char *pucB, unsigned char *pucOut))0x000042c3)
#define ROM_AES_CMAC_Leftshift_Onebit                ((void (*)(unsigned char *pucInput, unsigned char *pucOutput))0x000042dd)
#define ROM_AES_CMAC_Generate_Subkey                 ((void (*)(unsigned char *pucKey, unsigned char *pucK1, unsigned char *pucK2))0x00004303)
#define ROM_AES_128_EIA2                             ((void (*)(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucMACT))0x00004381)


#define ROM_CSPRxFrameSet                            ((void (*)(unsigned long ulBase, unsigned long ulDelay, unsigned long ulDataLen, unsigned long ulFrameLen))0x000035b9)
#define ROM_CSPTxFrameSet                            ((void (*)(unsigned long ulBase, unsigned long ulDelay, unsigned long ulDataLen, unsigned long ulSyncLen, unsigned long ulFrameLen))0x000035df)
#define ROM_CSPClockSet                              ((void (*)(unsigned long ulBase, unsigned long ulCSPMode, unsigned long ulPclk, unsigned ulClock))0x0000360f)
#define ROM_CSPEndianModeSet                         ((void (*)(unsigned long ulBase, unsigned long ulEndianMode))0x0000372b)
#define ROM_CSPOperationModeSet                      ((void (*)(unsigned long ulBase, unsigned long ulOperationMode))0x00003737)
#define ROM_CSPUARTModeSet                           ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned char ucDatabits, unsigned char ucStopbits))0x00003743)
#define ROM_CSPIrdaEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIrdaEn))0x000037c9)
#define ROM_CSPIrdaPulseWidthSet                     ((void (*)(unsigned long ulBase, unsigned long ulWidth))0x000037d5)
#define ROM_CSPIrdaIdleLevelSet                      ((void (*)(unsigned long ulBase, unsigned long ulIrdaIdleLevel))0x000037e1)
#define ROM_CSPIrdaMode                              ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned char ucDatabits, unsigned char ucStopbits, unsigned long ulIdleLevel, unsigned long ulWidth))0x000037ed)
#define ROM_CSPSPIMode                               ((void (*)(unsigned long ulBase, unsigned long ulMode1, unsigned long ulMode2, unsigned long ulTxFrameCtl, unsigned long ulRxFrameCtl))0x0000388f)
#define ROM_CSPFIFOLevelSet                          ((void (*)(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel))0x000038d7)
#define ROM_CSPCharGet                               ((unsigned char (*)(unsigned long ulBase))0x000038e1)
#define ROM_CSPCharPut                               ((void (*)(unsigned long ulBase, unsigned char ucData))0x000038f9)
#define ROM_CSPCharGetNonBlocking                    ((long (*)(unsigned long ulBase))0x0000390d)
#define ROM_CSPCharPutNonBlocking                    ((long (*)(unsigned long ulBase, unsigned char ucData))0x00003925)
#define ROM_CSPClockModeSet                          ((void (*)(unsigned long ulBase, unsigned long ulClockMode))0x0000393d)
#define ROM_CSPEnable                                ((void (*)(unsigned long ulBase))0x00003949)
#define ROM_CSPDisable                               ((void (*)(unsigned long ulBase))0x00003953)
#define ROM_CSPDrivenEdgeSet                         ((void (*)(unsigned long ulBase, unsigned long ulRxEdge, unsigned long ulTxEdge))0x0000395d)
#define ROM_CSPSyncValidLevelSet                     ((void (*)(unsigned long ulBase, unsigned long ulRFS, unsigned long ulTFS))0x0000396f)
#define ROM_CSPClockIdleModeSet                      ((void (*)(unsigned long ulBase, unsigned long ulClkIdleMode))0x00003981)
#define ROM_CSPClockIdleLevelSet                     ((void (*)(unsigned long ulBase, unsigned long ulClkIdleLevel))0x0000398d)
#define ROM_CSPPinModeSet                            ((void (*)(unsigned long ulBase, unsigned long ulCSPPin, unsigned long ulPinMode, unsigned long ulPinDirection))0x00003999)
#define ROM_CSPRxEnable                              ((void (*)(unsigned long ulBase))0x000039d7)
#define ROM_CSPRxDisable                             ((void (*)(unsigned long ulBase))0x000039e1)
#define ROM_CSPTxEnable                              ((void (*)(unsigned long ulBase))0x000039eb)
#define ROM_CSPTxDisable                             ((void (*)(unsigned long ulBase))0x000039f5)
#define ROM_CSPIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))0x000039ff)
#define ROM_CSPIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))0x00003a19)
#define ROM_CSPIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x00003a2f)
#define ROM_CSPIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x00003a37)
#define ROM_CSPIntStatus                             ((unsigned long (*)(unsigned long ulBase))0x00003a3f)
#define ROM_CSPIntClear                              ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x00003a45)


#define ROM_DMAErrorStatusGet                        ((unsigned long (*)(unsigned long ulChannelNum))0x000020ad)
#define ROM_DMAErrorStatusClear                      ((void (*)(unsigned long ulChannelNum))0x000020bf)
#define ROM_DMAChannelRequest                        ((void (*)(unsigned long ulChannelNum))0x000020d9)
#define ROM_DMAChannelControlSet                     ((void (*)(unsigned long ulChannelNum, unsigned long ulControl))0x00002123)
#define ROM_DMAChannelTransferSet                    ((void (*)(unsigned long ulChannelNum, void *pvSrcAddr, void *pvDstAddr, unsigned long ulTransferSize))0x0000212f)
#define ROM_DMAChannelNextPointerSet                 ((void (*)(unsigned long ulChannelNum, void *pvNextPointer))0x0000214d)
#define ROM_DMAIntRegister                           ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))0x00002159)
#define ROM_DMAIntUnregister                         ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors))0x00002173)
#define ROM_DMAIntStatus                             ((unsigned long (*)(unsigned long ulChannelNum))0x00002189)
#define ROM_DMAIntClear                              ((void (*)(unsigned long ulChanMask))0x000021b9)
#define ROM_DMAChannelWaitIdle                       ((void (*)(unsigned long ulChannelNum))0x000021e3)
#define ROM_DMAChannelTransfer                       ((void (*)(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum))0x0000220f)
#define ROM_DMAWaitAESDone                           ((void (*)(void))0x0000223b)


#define ROM_FLASH_Init                               ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int ref_clk_hz, unsigned int sclk_hz))0x00002419)
#define ROM_FLASH_ChipErase                          ((void (*)(QSPI_FLASH_Def *flash_vendor))0x00002473)
#define ROM_FLASH_SectorErase                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))0x000024d7)
#define ROM_FLASH_BlockErase32K                      ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))0x00002555)
#define ROM_FLASH_BlockErase64K                      ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))0x000025d3)
#define ROM_FLASH_WriteData                          ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))0x00002651)
#define ROM_FLASH_ReadData                           ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))0x00002759)
#define ROM_FLASH_WaitIdle                           ((void (*)(QSPI_FLASH_Def *flash_vendor))0x000027cd)
#define ROM_FLASH_UpdateData                         ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum, unsigned long ulBackupFlag, unsigned ulExchangeAddr))0x00002811)
#define ROM_FLASH_EnableQEFlag                       ((void (*)(QSPI_FLASH_Def *flash_vendor))0x0000291f)
#define ROM_FLASH_SetReadMode                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int mode))0x000029f3)
#define ROM_FLASH_EnterXIPMode                       ((void (*)(QSPI_FLASH_Def *flash_vendor, char xip_dummy))0x00002a03)
#define ROM_FLASH_ExitXIPMode                        ((void (*)(QSPI_FLASH_Def *flash_vendor))0x00002a4f)
#define ROM_FLASH_SetWriteMode                       ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int mode))0x00002a93)
#define ROM_FLASH_GetDeviceID                        ((unsigned int (*)(QSPI_FLASH_Def *flash_vendor))0x00002aa3)
    

#define ROM_GPIOPeripheralPad                        ((void (*)(unsigned char ucPeriNum, unsigned char ucPadNum))0x0000303d)
#define ROM_GPIODirModeSet                           ((void (*)(unsigned char ucPins, unsigned long ulPinIO))0x00003049)
#define ROM_GPIOOutputODEn                           ((void (*)(unsigned char ucPins))0x0000311f)
#define ROM_GPIOOutputODDis                          ((void (*)(unsigned char ucPins))0x0000314d)
#define ROM_GPIOAnalogEn                             ((void (*)(unsigned char ucPins))0x0000317b)
#define ROM_GPIOAnalogDis                            ((void (*)(unsigned char ucPins))0x000031ad)
#define ROM_GPIOPullupEn                             ((void (*)(unsigned char ucPins))0x000031df)
#define ROM_GPIOPullupDis                            ((void (*)(unsigned char ucPins))0x0000320d)
#define ROM_GPIOPulldownEn                           ((void (*)(unsigned char ucPins))0x0000323b)
#define ROM_GPIOPulldownDis                          ((void (*)(unsigned char ucPins))0x00003269)
#define ROM_GPIOPinSet                               ((void (*)(unsigned char ucPins))0x00003297)
#define ROM_GPIOPinClear                             ((void (*)(unsigned char ucPins))0x000032c5)
#define ROM_GPIOPinRead                              ((unsigned char (*)(unsigned char ucPins))0x000032f3)
#define ROM_GPIOIntEnable                            ((void (*)(unsigned char ucPins))0x00003323)
#define ROM_GPIOIntDisable                           ((void (*)(unsigned char ucPins))0x00003351)
//#define ROM_GPIOIntTypeSet                         ((void (*)(unsigned char ucPins, unsigned char ucConfig))0x0000337f)
//#define ROM_GPIOIntEdgeSet                         ((void (*)(unsigned char ucPins, unsigned char ucConfig))0x000033c3)
//#define ROM_GPIOIntSingleEdgeSet                   ((void (*)(unsigned char ucPins, unsigned char ucConfig))0x00003407)
#define ROM_GPIOIntStatusGet                         ((unsigned long (*)(unsigned char ucPins))0x00003451)
#define ROM_GPIOIntMaskGet                           ((unsigned long (*)(unsigned char ucPins))0x00003463)
#define ROM_GPIOConflictStatusGet                    ((unsigned long (*)(unsigned char ucPins))0x00003495)
#define ROM_GPIOConflictStatusClear                  ((void (*)(unsigned char ucPins))0x000034c7)
#define ROM_GPIOAllocationStatusGet                  ((unsigned long (*)(unsigned char ucPins))0x000034f5)
#define ROM_GPIODrvStrengthSet                       ((void (*)(unsigned char ucPins, unsigned char ucDrvStrength))0x00003529)
#define ROM_GPIOIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))0x0000355f)
#define ROM_GPIOIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))0x00003577)


#define ROM_I2CMasterEnable                          ((void (*)(unsigned long ulBase))0x00002ae5)
#define ROM_I2CMasterInitExpClk                      ((void (*)(unsigned long ulBase, unsigned long ulI2CClk, unsigned char bFast))0x00002aef)
#define ROM_I2CSlaveEnable                           ((void (*)(unsigned long ulBase))0x00002b61)
#define ROM_I2CSlaveInit                             ((void (*)(unsigned long ulBase, unsigned long ucSlaveAddr))0x00002b6b)
#define ROM_I2CIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(fnHandler)(void)))0x00002b7b)
#define ROM_I2CIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))0x00002ba7)
#define ROM_I2CIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x00002bcb)
#define ROM_I2CIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x00002bd3)
#define ROM_I2CIntStatus                             ((unsigned char (*)(unsigned long ulBase, unsigned char bMasked))0x00002bdb)
#define ROM_I2CIntClear                              ((void (*)(unsigned long ulBase))0x00002beb)
#define ROM_I2CAddrSet                               ((void (*)(unsigned long ulBase, unsigned long ulAddr, unsigned char ucNorMode, unsigned char ucReceive))0x00002bf3)
#define ROM_I2CBusBusy                               ((unsigned char (*)(unsigned long ulBase))0x00002c29)
#define ROM_I2CControlSet                            ((void (*)(unsigned long ulBase, unsigned long ulCmd))0x00002c3b)
#define ROM_I2CControlGet                            ((unsigned long (*)(unsigned long ulBase))0x00002c3f)
#define ROM_I2CDataPut                               ((void (*)(unsigned long ulBase, unsigned char ucData))0x00002c45)
#define ROM_I2CDataGet                               ((unsigned long (*)(unsigned long ulBase))0x00002c49)
#define ROM_I2CStatus                                ((unsigned long (*)(unsigned long ulBase))0x00002c4f)
#define ROM_I2CACKModeSet                            ((void (*)(unsigned long ulBase, unsigned char ucEnable))0x00002c55)
#define ROM_I2CFifoClear                             ((void (*)(unsigned long ulBase))0x00002c6b)


#define ROM_IntRegister                              ((void (*)(unsigned long ulInterrupt, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))0x00001cb5)
#define ROM_IntUnregister                            ((void (*)(unsigned long ulInterrupt, unsigned long *g_pRAMVectors))0x00001ce1)
#define ROM_IntPriorityGroupingSet                   ((void (*)(unsigned long ulBits))0x00001ce9)
#define ROM_IntPriorityGroupingGet                   ((unsigned long (*)(void))0x00001cfb)
#define ROM_IntPrioritySet                           ((void (*)(unsigned long ulInterrupt,unsigned char ucPriority))0x00001d1f)
#define ROM_IntPriorityGet                           ((long (*)(unsigned long ulInterrupt))0x00001d4d)
#define ROM_IntEnable                                ((void (*)(unsigned long ulInterrupt))0x00001d63)
#define ROM_IntDisable                               ((void (*)(unsigned long ulInterrupt))0x00001de9)
#define ROM_IntPendSet                               ((void (*)(unsigned long ulInterrupt))0x00001e71)
#define ROM_IntPendClear                             ((void (*)(unsigned long ulInterrupt))0x00001ee1)


#define ROM_PRCM_Clock_Enable                        ((void (*)(unsigned long ulBase, unsigned long ulModule))0x00003591)
#define ROM_PRCM_Clock_Disable                       ((void (*)(unsigned long ulBase, unsigned long ulModule))0x00003599)
#define ROM_PRCM_CHIP_VER_GET                        ((unsigned long (*)(unsigned long ulBase))0x000035a1)
#define ROM_PRCM_BOOTMODE_GET                        ((unsigned char (*)(unsigned long ulBase))0x000035a7)


#define ROM_QSPIControllerEnable                     ((void (*)(unsigned char *reg_base_addr))0x00001457)
#define ROM_QSPIControllerDisable                    ((void (*)(unsigned char *reg_base_addr))0x00001461)
#define ROM_QSPIWaitIdle                             ((void (*)(unsigned char *reg_base))0x0000146b)
#define ROM_QSPIReaddataCapture                      ((void (*)(unsigned char *reg_base,unsigned int bypass, unsigned int delay))0x0000147b)
#define ROM_QSPIConfigBaudrateDiv                    ((void (*)(unsigned char *reg_base,unsigned int ref_clk_hz, unsigned int sclk_hz))0x000014ad)
#define ROM_QSPISetClkMode                           ((void (*)(unsigned char *reg_base, uint mode))0x000014e3)
#define ROM_QSPIChipSelect                           ((void (*)(unsigned char *reg_base,unsigned int chip_select, unsigned int decoder_enable))0x00001513)
#define ROM_QSPIDelay                                ((void (*)(unsigned char *reg_base,unsigned int ref_clk, unsigned int sclk_hz,unsigned int tshsl_ns, unsigned int tsd2d_ns,unsigned int tchsh_ns, unsigned int tslch_ns))0x0000154f)
#define ROM_QSPIControllerInit                       ((void (*)(QSPI_FLASH_Def *plat))0x000015eb)
#define ROM_QSPICommandRead                          ((int (*)(unsigned char *reg_base_addr,unsigned int cmdlen, const u8 *cmdbuf, unsigned int rxlen, u8 *rxbuf))0x0000165b)
#define ROM_QSPICommandWrite                         ((int (*)(unsigned char *reg_base_addr,unsigned int cmdlen, const u8 *cmdbuf,unsigned int txlen,  const u8 *txbuf))0x000016e7)
#define ROM_QSPIIndirectReadSetup                    ((int (*)(QSPI_FLASH_Def *plat,unsigned int cmdlen, unsigned int rx_width, const u8 *cmdbuf))0x00001799)
#define ROM_QSPIIndirectReadExecute                  ((int (*)(QSPI_FLASH_Def *plat,unsigned int rxlen, u8 *rxbuf))0x00001815)
#define ROM_QSPIIndirectWriteSetup                   ((int (*)(QSPI_FLASH_Def *plat,unsigned int cmdlen, const u8 *cmdbuf))0x00001827)
#define ROM_QSPIIndirectWriteExecute                 ((int (*)(QSPI_FLASH_Def *plat,unsigned int txlen, const u8 *txbuf))0x00001883)
#define ROM_QSPIEnterXIP                             ((void (*)(unsigned char *reg_base, char xip_dummy))0x00001895)
#define ROM_QSPIExitXIP                              ((void (*)(unsigned char *reg_base))0x000018b3)
#define ROM_QSPIIndirectEnable                       ((void (*)(QSPI_FLASH_Def *plat))0x000018d1)
#define ROM_QSPIIndirectDisable                      ((void (*)(QSPI_FLASH_Def *plat))0x000018db)
#define ROM_QSPIIndirectWrite                        ((void (*)(QSPI_FLASH_Def *plat, unsigned int dst, unsigned int n_bytes))0x000018eb)
#define ROM_QSPIIndirectRead                         ((void (*)(QSPI_FLASH_Def *plat, unsigned int src, unsigned int n_bytes))0x00001909)
#define ROM_QSPISetReadInstruction                   ((void (*)(unsigned char *reg_base, unsigned int config))0x00001927)
#define ROM_QSPISetWriteInstruction                  ((void (*)(unsigned char *reg_base, unsigned int config))0x0000192b)
#define ROM_QSPIIntEnable                            ((void (*)(unsigned char *reg_base, unsigned int intflags))0x0000192f)
#define ROM_QSPIIntDisable                           ((void (*)(unsigned char *reg_base, unsigned int intflags))0x00001937)
#define ROM_QSPIIntStatusGet                         ((unsigned long (*)(unsigned char *reg_base))0x0000193f)
#define ROM_QSPIIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))0x00001949)
#define ROM_QSPIIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))0x00001961)


#define ROM_SEMAMasterEnable                         ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave))0x000045e9)
#define ROM_SEMAMasterDisable                        ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave))0x000045fd)
#define ROM_SEMARequest                              ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave))0x00004611)
#define ROM_SEMARequestNonBlocking                   ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave))0x00004639)
#define ROM_SEMARequestPriSet                        ((void (*)(unsigned long ulBase, unsigned char ucSemaSlave, unsigned long ucPriority))0x00004651)
#define ROM_SEMAMasterGet                            ((unsigned char (*)(unsigned long ulBase, unsigned char ucSemaSlave))0x0000465f)
#define ROM_SEMASlaveGet                             ((unsigned long (*)(unsigned long ulBase, unsigned char ucSemaMaster))0x0000466d)
#define ROM_SEMAReset                                ((void (*)(unsigned long ulBase))0x0000467f)
#define ROM_SEMAReqQueueState                        ((unsigned char (*)(unsigned long ulBase))0x0000468d)
#define ROM_SEMAMasterIntEnable                      ((void (*)(unsigned long ulBase, unsigned long ulSemaMasterInt))0x00004699)
#define ROM_SEMARelease                              ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave))0x000046a5)
#define ROM_SEMAIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))0x000046bd)
#define ROM_SEMAIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))0x000046d5)
    

#define ROM_SPIConfigSetExpClk                       ((void (*)(unsigned long ulBase, unsigned long ulDivide, unsigned long ulProtocol, unsigned long ulMode,unsigned long ulDataWidth))0x0000225d)
#define ROM_SPIEnable                                ((void (*)(unsigned long ulBase))0x00002273)
#define ROM_SPIDisable                               ((void (*)(unsigned long ulBase))0x00002279)
#define ROM_SPIIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))0x0000227f)
#define ROM_SPIIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))0x0000229f)
#define ROM_SPIIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x000022b7)
#define ROM_SPIIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x000022bf)
#define ROM_SPIIntStatus                             ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))0x000022c7)
#define ROM_SPIIntClear                              ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x000022d3)
#define ROM_SPIDataPut                               ((void (*)(unsigned long ulBase, unsigned long ulData))0x000022d7)
#define ROM_SPIDataPutNonBlocking                    ((long (*)(unsigned long ulBase, unsigned long ulData))0x000022e7)
#define ROM_SPIDataGet                               ((void (*)(unsigned long ulBase, unsigned long *pulData))0x000022fb)
#define ROM_SPIDataGetNonBlocking                    ((long (*)(unsigned long ulBase, unsigned long *pulData))0x0000230d)
#define ROM_SPISetTxFifoThreshold                    ((void (*)(unsigned long ulBase, unsigned long ulThreshold))0x00002323)
#define ROM_SPISetRxFifoThreshold                    ((void (*)(unsigned long ulBase, unsigned long ulThreshold))0x00002327)
#define ROM_SPISetDelay                              ((void (*)(unsigned long ulBase, unsigned long ulDelay))0x0000232b)
#define ROM_SPIEnManualCS                            ((void (*)(unsigned long ulBase))0x0000232f)
#define ROM_SPIDisManualCS                           ((void (*)(unsigned long ulBase))0x00002339)
#define ROM_SPIEnManualTransmit                      ((void (*)(unsigned long ulBase))0x00002343)
#define ROM_SPIDisManualTransmit                     ((void (*)(unsigned long ulBase))0x0000234d)
#define ROM_SPIStartManualTransmit                   ((void (*)(unsigned long ulBase))0x00002357)
#define ROM_SPIEnModeFail                            ((void (*)(unsigned long ulBase))0x00002361)
#define ROM_SPIDisModeFail                           ((void (*)(unsigned long ulBase))0x0000236b)


#define ROM_SysTickEnable                            ((void (*)(void))0x00001ffd)
#define ROM_SysTickDisable                           ((void (*)(void))0x0000200f)
#define ROM_SysTickIntRegister                       ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))0x00002021)
#define ROM_SysTickIntUnregister                     ((void (*)(unsigned long *g_pRAMVectors))0x00002049)
#define ROM_SysTickIntEnable                         ((void (*)(void))0x0000206d)
#define ROM_SysTickIntDisable                        ((void (*)(void))0x0000207f)
#define ROM_SysTickPeriodSet                         ((void (*)(unsigned long ulPeriod))0x00002091)
#define ROM_SysTickPeriodGet                         ((unsigned long (*)(void))0x0000209b)
#define ROM_SysTickValueGet                          ((unsigned long (*)(void))0x000020a5)


#define ROM_TimerEnable                              ((void (*)(unsigned long ulBase))0x00001f59)
#define ROM_TimerDisable                             ((void (*)(unsigned long ulBase))0x00001f63)
#define ROM_TimerConfigure                           ((void (*)(unsigned long ulBase, unsigned long ulConfig))0x00001f6d)
#define ROM_TimerControlLevel                        ((void (*)(unsigned long ulBase, unsigned char bInvert))0x00001f79)
#define ROM_TimerPrescaleSet                         ((void (*)(unsigned long ulBase, unsigned long ulValue))0x00001f8d)
#define ROM_TimerPrescaleGet                         ((unsigned long (*)(unsigned long ulBase))0x00001f99)
#define ROM_TimerLoadSet                             ((void (*)(unsigned long ulBase, unsigned long ulValue))0x00001fa3)
#define ROM_TimerValueGet                            ((unsigned long (*)(unsigned long ulBase))0x00001fa7)
#define ROM_TimerMatchSet                            ((void (*)(unsigned long ulBase, unsigned long ulValue))0x00001fad)
#define ROM_TimerMatchGet                            ((unsigned long (*)(unsigned long ulBase))0x00001fb1)
#define ROM_TimerPWMSet                              ((void (*)(unsigned long ulBase, unsigned long ulValue))0x00001fb7)
#define ROM_TimerPWMGet                              ((unsigned long (*)(unsigned long ulBase))0x00001fbb)
#define ROM_TimerIntRegister                         ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))0x00001fc1)
#define ROM_TimerIntUnregister                       ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors))0x00001fdb)
#define ROM_TimerIntEnable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x00001ff1)


#define	ROM_UARTParityModeSet                        ((void (*)(unsigned long ulBase, unsigned long ulParity))0x00001975)
#define	ROM_UARTParityModeGet                        ((unsigned long (*)(unsigned long ulBase))0x00001981)
#define	ROM_UARTFIFOLevelSet                         ((void (*)(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel))0x0000198b)
#define	ROM_UARTFIFOLevelGet                         ((void (*)(unsigned long ulBase, unsigned long *pulTxLevel, unsigned long *pulRxLevel))0x00001991)
#define	ROM_UARTDisable                              ((void (*)(unsigned long ulBase))0x000019c1)
#define	ROM_UARTConfigSetExpClk                      ((void (*)(unsigned long ulBase, unsigned long ulPreDivide, unsigned long ulCD, unsigned long ulBDIV, unsigned long ulConfig))0x000019cb)
#define	ROM_UARTConfigGetExpClk                      ((void (*)(unsigned long ulBase, unsigned long *pulPreDivide, unsigned long *pulCD, unsigned long *pulBDIV, unsigned long *pulConfig))0x00001a0d)
#define	ROM_UARTCharsAvail                           ((unsigned char (*)(unsigned long ulBase))0x00001a31)
#define	ROM_UARTSpaceAvail                           ((unsigned char (*)(unsigned long ulBase))0x00001a3d)
#define	ROM_UARTCharGetNonBlocking                   ((long (*)(unsigned long ulBase))0x00001a49)
#define	ROM_UARTCharGet                              ((long (*)(unsigned long ulBase))0x00001a5d)
#define	ROM_UARTNCharGet                             ((void (*)(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte))0x00001a6f)
#define	ROM_UARTCharGetWithTimeout                   ((unsigned long (*)(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte))0x00001a8f)
#define	ROM_UARTCharPutNonBlocking                   ((unsigned char (*)(unsigned long ulBase, unsigned char ucData))0x00001ac7)
#define	ROM_UARTCharPut                              ((void (*)(unsigned long ulBase, unsigned char ucData))0x00001adb)
#define	ROM_UARTNCharPut                             ((void (*)(unsigned long ulBase, unsigned char *pucData, unsigned long ulLenByte))0x00001aeb)
#define	ROM_UARTBreakCtl                             ((void (*)(unsigned long ulBase, unsigned char bBreakState))0x00001b0b)
#define	ROM_UARTIntRegister                          ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))0x00001b1f)
#define	ROM_UARTIntUnregister                        ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))0x00001b3f)
#define	ROM_UARTIntEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x00001b57)
#define	ROM_UARTIntDisable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))0x00001b5f)
#define	ROM_UARTIntStatus                            ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))0x00001b67)
#define	ROM_UARTIntClear                             ((void (*)(unsigned long ulBase))0x00001b73)
#define	ROM_UARTRxErrorGet                           ((unsigned long (*)(unsigned long ulBase))0x00001b7b)
#define	ROM_UARTRxErrorClear                         ((void (*)(unsigned long ulBase))0x00001b85)

#ifdef _SIM_
 #define	ROM_UARTEnable                             
 #define	ROM_UARTStringPut                            
 #define	ROM_UARTConfigSetBaudrate                    
 #define	ROM_UARTAutoBaudrate                        
 #define	ROM_UARTConfigSetMode                       
#else
 #define	ROM_UARTEnable                               ((void (*)(unsigned long ulBase))0x000019a5)
 #define	ROM_UARTStringPut                            ((void (*)(unsigned long ulBase, unsigned char *str))0x00001b91)
 #define	ROM_UARTConfigSetBaudrate                    ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned long ulConfig))0x00001bab)
 #define	ROM_UARTAutoBaudrate                         ((void (*)(unsigned long ulBase))0x00001c1f)
 #define	ROM_UARTConfigSetMode                        ((void (*)(unsigned long ulBase, unsigned long ulConfig))0x00001ca1)
#endif



#define ROM_UTCTimerStop                             ((void (*)(void))0x00002c7d)
#define ROM_UTCTimerRun                              ((void (*)(void))0x00002c8b)
#define ROM_UTCCalStop                               ((void (*)(void))0x00002c99)
#define ROM_UTCCalRun                                ((void (*)(void))0x00002ca7)
#define ROM_UTCDivStop                               ((void (*)(void))0x00002cb5)
#define ROM_UTCDivEn                                 ((void (*)(void))0x00002cc3)
#define ROM_UTCHourModeSet                           ((void (*)(unsigned long ulMode))0x00002cd1)
#define ROM_UTCHourModeGet                           ((unsigned long (*)(void))0x00002cd7)
#define ROM_UTCTimerSet                              ((void (*)(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec))0x00002cdd)
#define ROM_UTCTimerChangeGet                        ((unsigned long (*)(void))0x00002d7b)
#define ROM_UTCTimerGet                              ((void (*)(unsigned char *ulAMPM, unsigned char *ulHour, unsigned char *ulMin, unsigned char *ulSec, unsigned char *ulMinSec))0x00002d83)
#define ROM_UTCTimerAlarmSet                         ((void (*)(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec))0x00002ddd)
#define ROM_UTCCalSet                                ((void (*)(unsigned long ulCentury, unsigned long ulYear, unsigned long ulMonth, unsigned long ulData, unsigned long ulDay))0x00002e6f)
#define ROM_UTCCalChangeGet                          ((unsigned long (*)(void))0x00002f07)
#define ROM_UTCCalGet                                ((void (*)(unsigned char *ulCentury, unsigned char *ulYear, unsigned char *ulMonth, unsigned char *ulData, unsigned char *ulDay))0x00002f0f)
#define ROM_UTCCalAlarmSet                           ((void (*)(unsigned long ulMonth, unsigned long ulData))0x00002f69)
#define ROM_UTCAlarmEnable                           ((void (*)(unsigned long ulAlarmFlags))0x00002fa1)
#define ROM_UTCAlarmDisable                          ((void (*)(unsigned long ulAlarmFlags))0x00002fad)
#define ROM_UTCIntEnable                             ((void (*)(unsigned long ulIntFlags))0x00002fb9)
#define ROM_UTCIntDisable                            ((void (*)(unsigned long ulIntFlags))0x00002fc5)
#define ROM_UTCIntMaskSet                            ((void (*)(unsigned long ulIntMask))0x00002fd1)
#define ROM_UTCIntMaskGet                            ((unsigned long (*)(void))0x00002fdd)
#define ROM_UTCIntStatusSet                          ((void (*)(unsigned long ulIntFlags))0x00002fe3)
#define ROM_UTCIntStatusGet                          ((unsigned long (*)(void))0x00002fef)
#define ROM_UTCValidStatusGet                        ((unsigned long (*)(void))0x00002ff5)
#define ROM_UTCKeepRun                               ((void (*)(unsigned long ulKeepUTC))0x00002ffb)
#define ROM_UTCClkCntSet                             ((void (*)(unsigned long ulClkCnt))0x00003001)
#define ROM_UTCClkCntGet                             ((unsigned long (*)(void))0x00003007)
#define ROM_UTCIntRegister                           ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))0x0000300d)
#define ROM_UTCIntUnregister                         ((void (*)(unsigned long *g_pRAMVectors))0x00003025)


#define ROM_WatchdogRunning                          ((unsigned char (*)(unsigned long ulBase))0x00002375)
#define ROM_WatchdogEnable                           ((void (*)(unsigned long ulBase))0x0000237f)
#define ROM_WatchdogResetEnable                      ((void (*)(unsigned long ulBase))0x00002389)
#define ROM_WatchdogResetDisable                     ((void (*)(unsigned long ulBase))0x00002393)
#define ROM_WatchdogTimerRepeatEnable                ((void (*)(unsigned long ulBase))0x0000239d)
#define ROM_WatchdogTimerRepeatDisable               ((void (*)(unsigned long ulBase))0x000023a7)
#define ROM_WatchdogReloadSet                        ((void (*)(unsigned long ulBase, unsigned long ulLoadVal))0x000023b1)
#define ROM_WatchdogValueGet                         ((unsigned long (*)(unsigned long ulBase))0x000023b5)
#define ROM_WatchdogIntRegister                      ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))0x000023bb)
#define ROM_WatchdogIntUnregister                    ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))0x000023d5)
#define ROM_WatchdogIntEnable                        ((void (*)(unsigned long ulBase))0x000023eb)
#define ROM_WatchdogIntStatus                        ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))0x000023f5)
#define ROM_WatchdogIntClear                         ((void (*)(unsigned long ulBase))0x00002405)
#endif


#ifdef ROM_VERSION_1_2
#define func_DmaAddrCheck                             0x00000129
#define func_DMAChannelMemcpy                         0x0000018f
#define func_FlashImageVerify                         0x0000021f
#define func_FlashHeaderCheck                         0x0000028d
#define func_Flash_Exchange_Recover                   0x00000389
#define func_CMD_GET_CHIP_ID_HANDLER                  0x00000433
#define func_CMD_GET_VERSION_HANDLER                  0x00000475
#define func_CMD_GET_COMMANDS_HANDLER                 0x0000049b
#define func_CMD_ERASE_MEMORY_HANDLER                 0x00000557
#define func_CMD_WRITE_MEMORY_HANDLER                 0x00000671
#define func_CMD_READ_MEMORY_HANDLER                  0x0000099b
#define func_CMD_COPY_MEMORY_HANDLER                  0x00000c4b
#define func_CMD_COMPARE_MEMORY_HANDLER               0x00000d61
#define func_CMD_RUN_PROGRAM_HANDLER                  0x00000ed7
#define func_CMD_DOWNLOAD_IMAGE_HANDLER               0x00000fc7
#define func_CMD_UPDATE_FLASH_HANDLER                 0x00000fcf
#define func_CMD_ENABLE_DSP_CORE_HANDLER              0x000012e1
#define func_CMD_FLASH_QE_SET_HANDLER                 0x00001329
#define func_CMD_FLASH_XIP_EXIT_HANDLER               0x00001383
#define func_CMD_FLASH_XIP_ENTER_HANDLER              0x000013bb
#define func_BootSystemInit                           0x00001401
#define func_main                                     0x00001485
#define func_qspi_memcpy                              0x000018c5
#define func_qspi_memset                              0x000018df
#define func_qspi_apb_controller_enable               0x00001913
#define func_qspi_apb_controller_disable              0x0000191d
#define func_qspi_wait_idle                           0x00001927
#define func_qspi_apb_readdata_capture                0x00001937
#define func_qspi_apb_config_baudrate_div             0x00001969
#define func_qspi_apb_set_clk_mode                    0x0000199f
#define func_qspi_apb_chipselect                      0x000019cf
#define func_qspi_apb_delay                           0x00001a0b
#define func_qspi_apb_controller_init                 0x00001aa7
#define func_qspi_apb_command_read                    0x00001b17
#define func_qspi_apb_command_write                   0x00001ba3
#define func_qspi_apb_indirect_read_setup             0x00001c55
#define func_qspi_apb_indirect_read_execute           0x00001cd1
#define func_qspi_apb_indirect_write_setup            0x00001ce3
#define func_qspi_apb_indirect_write_execute          0x00001d3f
#define func_qspi_apb_enter_xip                       0x00001d51
#define func_qspi_apb_exit_xip                        0x00001d6f
#define func_qspi_apb_indirect_enable                 0x00001d8d
#define func_qspi_apb_indirect_disable                0x00001d97
#define func_qspi_apb_indirect_write                  0x00001da7
#define func_qspi_apb_indirect_read                   0x00001dc5
#define func_qspi_apb_set_read_instruction            0x00001de3
#define func_qspi_apb_set_write_instruction           0x00001de7
#define func_qspi_apb_interrupt_enable                0x00001deb
#define func_qspi_apb_interrupt_disable               0x00001df3
#define func_qspi_apb_interrupt_status_get            0x00001dfb
#define func_QSPIIntRegister                          0x00001e05
#define func_QSPIIntUnregister                        0x00001e1d
#define func_UARTParityModeSet                        0x00001e31
#define func_UARTParityModeGet                        0x00001e3d
#define func_UARTFIFOLevelSet                         0x00001e47
#define func_UARTFIFOLevelGet                         0x00001e4d
#define func_UARTEnable                               0x00001e61
#define func_UARTDisable                              0x00001e7d
#define func_UARTConfigSetExpClk                      0x00001e87
#define func_UARTConfigGetExpClk                      0x00001ec9
#define func_UARTCharsAvail                           0x00001eed
#define func_UARTSpaceAvail                           0x00001ef9
#define func_UARTCharGetNonBlocking                   0x00001f05
#define func_UARTCharGet                              0x00001f19
#define func_UARTNCharGet                             0x00001f2b
#define func_UARTCharGetWithTimeout                   0x00001f4b
#define func_UARTCharPutNonBlocking                   0x00001f83
#define func_UARTCharPut                              0x00001f97
#define func_UARTNCharPut                             0x00001fa7
#define func_UARTBreakCtl                             0x00001fc7
#define func_UARTIntRegister                          0x00001fdb
#define func_UARTIntUnregister                        0x00001ffb
#define func_UARTIntEnable                            0x00002013
#define func_UARTIntDisable                           0x0000201b
#define func_UARTIntStatus                            0x00002023
#define func_UARTIntClear                             0x0000202f
#define func_UARTRxErrorGet                           0x00002037
#define func_UARTRxErrorClear                         0x00002041
#define func_UARTStringPut                            0x0000204d
#define func_UARTConfigSetBaudrate                    0x00002067
#define func_UARTAutoBaudrate                         0x000020db
#define func_UARTConfigSetMode                        0x00002163
#define func_UARTWaitTxDone                           0x00002171
#define func_UARTPrintf                               0x000022f7
#define func_IntRegister                              0x00002425
#define func_IntUnregister                            0x00002451
#define func_IntPriorityGroupingSet                   0x00002459
#define func_IntPriorityGroupingGet                   0x0000246b
#define func_IntPrioritySet                           0x0000248f
#define func_IntPriorityGet                           0x000024bd
#define func_IntEnable                                0x000024d3
#define func_IntDisable                               0x00002559
#define func_IntPendSet                               0x000025e1
#define func_IntPendClear                             0x00002651
#define func_TimerEnable                              0x000026c9
#define func_TimerDisable                             0x000026d3
#define func_TimerConfigure                           0x000026dd
#define func_TimerControlLevel                        0x000026e9
#define func_TimerPrescaleSet                         0x000026fd
#define func_TimerPrescaleGet                         0x00002709
#define func_TimerLoadSet                             0x00002713
#define func_TimerValueGet                            0x00002717
#define func_TimerMatchSet                            0x0000271d
#define func_TimerMatchGet                            0x00002721
#define func_TimerPWMSet                              0x00002727
#define func_TimerPWMGet                              0x0000272b
#define func_TimerIntRegister                         0x00002731
#define func_TimerIntUnregister                       0x0000274b
#define func_TimerIntEnable                           0x00002761
#define func_SysTickEnable                            0x0000276d
#define func_SysTickDisable                           0x0000277f
#define func_SysTickIntRegister                       0x00002791
#define func_SysTickIntUnregister                     0x000027b9
#define func_SysTickIntEnable                         0x000027dd
#define func_SysTickIntDisable                        0x000027ef
#define func_SysTickPeriodSet                         0x00002801
#define func_SysTickPeriodGet                         0x0000280b
#define func_SysTickValueGet                          0x00002815
#define func_DMAErrorStatusGet                        0x0000281d
#define func_DMAErrorStatusClear                      0x0000282f
#define func_DMAChannelRequest                        0x00002849
#define func_DMAChannelControlSet                     0x00002893
#define func_DMAChannelTransferSet                    0x0000289f
#define func_DMAChannelNextPointerSet                 0x000028bd
#define func_DMAIntRegister                           0x000028c9
#define func_DMAIntUnregister                         0x000028e3
#define func_DMAIntStatus                             0x000028f9
#define func_DMAIntClear                              0x00002929
#define func_DMAChannelWaitIdle                       0x00002953
#define func_DMAChannelTransfer                       0x0000297f
#define func_DMAWaitAESDone                           0x000029ab
#define func_DMAWaitChannelDone                       0x000029c7
#define func_SPIConfigSetExpClk                       0x000029e5
#define func_SPIEnable                                0x000029fb
#define func_SPIDisable                               0x00002a01
#define func_SPIIntRegister                           0x00002a07
#define func_SPIIntUnregister                         0x00002a27
#define func_SPIIntEnable                             0x00002a3f
#define func_SPIIntDisable                            0x00002a47
#define func_SPIIntStatus                             0x00002a4f
#define func_SPIIntClear                              0x00002a5b
#define func_SPIDataPut                               0x00002a5f
#define func_SPIDataPutNonBlocking                    0x00002a6f
#define func_SPIDataGet                               0x00002a83
#define func_SPIDataGetNonBlocking                    0x00002a95
#define func_SPISetTxFifoThreshold                    0x00002aab
#define func_SPISetRxFifoThreshold                    0x00002aaf
#define func_SPISetDelay                              0x00002ab3
#define func_SPIEnManualCS                            0x00002ab7
#define func_SPIDisManualCS                           0x00002ac1
#define func_SPIEnManualTransmit                      0x00002acb
#define func_SPIDisManualTransmit                     0x00002ad5
#define func_SPIStartManualTransmit                   0x00002adf
#define func_SPIEnModeFail                            0x00002ae9
#define func_SPIDisModeFail                           0x00002af3
#define func_WatchdogRunning                          0x00002afd
#define func_WatchdogEnable                           0x00002b07
#define func_WatchdogResetEnable                      0x00002b11
#define func_WatchdogResetDisable                     0x00002b1b
#define func_WatchdogTimerRepeatEnable                0x00002b25
#define func_WatchdogTimerRepeatDisable               0x00002b2f
#define func_WatchdogReloadSet                        0x00002b39
#define func_WatchdogValueGet                         0x00002b3d
#define func_WatchdogIntRegister                      0x00002b43
#define func_WatchdogIntUnregister                    0x00002b5b
#define func_WatchdogIntEnable                        0x00002b6f
#define func_WatchdogIntStatus                        0x00002b7d
#define func_WatchdogIntClear                         0x00002b8d
#define func_WatchdogDisable                          0x00002b95
#define func_flash_vendor_delay                       0x00002ba5
#define func_FLASH_Init                               0x00002bb1
#define func_FLASH_ChipErase                          0x00002c0b
#define func_FLASH_SectorErase                        0x00002c6f
#define func_FLASH_BlockErase32K                      0x00002ced
#define func_FLASH_BlockErase64K                      0x00002d6b
#define func_FLASH_WriteData                          0x00002de9
#define func_FLASH_ReadData                           0x00002ef1
#define func_FLASH_WaitIdle                           0x00002f65
#define func_FLASH_UpdateData                         0x00002fa9
#define func_FLASH_EnableQEFlag                       0x000030b7
#define func_FLASH_SetReadMode                        0x0000318b
#define func_FLASH_EnterXIPMode                       0x0000319b
#define func_FLASH_ExitXIPMode                        0x000031e7
#define func_FLASH_SetWriteMode                       0x0000322b
#define func_FLASH_GetDeviceID                        0x0000323b
#define func_I2CMasterEnable                          0x0000327d
#define func_I2CMasterInitExpClk                      0x00003287
#define func_I2CSlaveEnable                           0x000032f9
#define func_I2CSlaveInit                             0x00003303
#define func_I2CIntRegister                           0x00003313
#define func_I2CIntUnregister                         0x0000333f
#define func_I2CIntEnable                             0x00003363
#define func_I2CIntDisable                            0x0000336b
#define func_I2CIntStatus                             0x00003373
#define func_I2CIntClear                              0x00003383
#define func_I2CAddrSet                               0x0000338b
#define func_I2CBusBusy                               0x000033c1
#define func_I2CControlSet                            0x000033d3
#define func_I2CControlGet                            0x000033d7
#define func_I2CDataPut                               0x000033dd
#define func_I2CDataGet                               0x000033e1
#define func_I2CStatus                                0x000033e7
#define func_I2CACKModeSet                            0x000033ed
#define func_I2CFifoClear                             0x00003403
#define func_UTCTimerStop                             0x00003415
#define func_UTCTimerRun                              0x00003423
#define func_UTCCalStop                               0x00003431
#define func_UTCCalRun                                0x0000343f
#define func_UTCDivStop                               0x0000344d
#define func_UTCDivEn                                 0x0000345b
#define func_UTCHourModeSet                           0x00003469
#define func_UTCHourModeGet                           0x0000346f
#define func_UTCTimerSet                              0x00003475
#define func_UTCTimerChangeGet                        0x00003513
#define func_UTCTimerGet                              0x0000351b
#define func_UTCTimerAlarmSet                         0x00003575
#define func_UTCCalSet                                0x00003607
#define func_UTCCalChangeGet                          0x0000369f
#define func_UTCCalGet                                0x000036a7
#define func_UTCCalAlarmSet                           0x00003701
#define func_UTCAlarmEnable                           0x00003739
#define func_UTCAlarmDisable                          0x00003745
#define func_UTCIntEnable                             0x00003751
#define func_UTCIntDisable                            0x0000375d
#define func_UTCIntMaskSet                            0x00003769
#define func_UTCIntMaskGet                            0x00003775
#define func_UTCIntStatusSet                          0x0000377b
#define func_UTCIntStatusGet                          0x00003787
#define func_UTCValidStatusGet                        0x0000378d
#define func_UTCKeepRun                               0x00003793
#define func_UTCClkCntSet                             0x00003799
#define func_UTCClkCntGet                             0x0000379f
#define func_UTCIntRegister                           0x000037a5
#define func_UTCIntUnregister                         0x000037bd
#define func_GPIOPeripheralPad                        0x000037d5
#define func_GPIODirModeSet                           0x000037e1
#define func_GPIOOutputODEn                           0x000038b7
#define func_GPIOOutputODDis                          0x000038e5
#define func_GPIOAnalogEn                             0x00003913
#define func_GPIOAnalogDis                            0x00003945
#define func_GPIOPullupEn                             0x00003977
#define func_GPIOPullupDis                            0x000039a5
#define func_GPIOPulldownEn                           0x000039d3
#define func_GPIOPulldownDis                          0x00003a01
#define func_GPIOPinSet                               0x00003a2f
#define func_GPIOPinClear                             0x00003a5d
#define func_GPIOPinRead                              0x00003a8b
#define func_GPIOIntEnable                            0x00003abb
#define func_GPIOIntDisable                           0x00003ae9
#define func_GPIOIntTypeSet                           0x00003b17
#define func_GPIOIntEdgeSet                           0x00003b5b
#define func_GPIOIntSingleEdgeSet                     0x00003b9f
#define func_GPIOIntStatusGet                         0x00003be9
#define func_GPIOIntMaskGet                           0x00003bfb
#define func_GPIOConflictStatusGet                    0x00003c2d
#define func_GPIOConflictStatusClear                  0x00003c5f
#define func_GPIOAllocationStatusGet                  0x00003c8d
#define func_GPIODrvStrengthSet                       0x00003cc1
#define func_GPIOIntRegister                          0x00003cf7
#define func_GPIOIntUnregister                        0x00003d0f
#define func_GPIOModeSet                              0x00003d23
#define func_GPIOPadConfigSel                         0x00003d65
#define func_GPIOPadModeSet                           0x00003da7
#define func_GPIODirectionSet                         0x00003e21
#define func_prcm_delay                               0x00003eb5
#define func_AON_DSP_Load_Flag_Get                    0x00003ec1
#define func_AON_ARM_Load_Flag_Get                    0x00003edf
#define func_AON_DSP_Load_Flag_Set                    0x00003efd
#define func_AON_ARM_Load_Flag_Set                    0x00003f1d
#define func_AON_Flash_Delay_Get                      0x00003f3d
#define func_AON_Flash_Delay_Set                      0x00003fd3
#define func_AON_Addr_Overlap_Unused_Mem              0x00003fe9
#define func_PRCM_Clock_Enable                        0x000040c7
#define func_PRCM_Clock_Disable                       0x000040cf
#define func_PRCM_CHIP_VER_GET                        0x000040d7
#define func_PRCM_BOOTMODE_GET                        0x000040dd
#define func_CSPRxFrameSet                            0x00004111
#define func_CSPTxFrameSet                            0x00004137
#define func_CSPClockSet                              0x00004167
#define func_CSPEndianModeSet                         0x00004283
#define func_CSPOperationModeSet                      0x0000428f
#define func_CSPUARTModeSet                           0x0000429b
#define func_CSPIrdaEnable                            0x00004321
#define func_CSPIrdaPulseWidthSet                     0x0000432d
#define func_CSPIrdaIdleLevelSet                      0x00004339
#define func_CSPIrdaMode                              0x00004345
#define func_CSPSPIMode                               0x000043e7
#define func_CSPFIFOLevelSet                          0x0000442f
#define func_CSPCharGet                               0x00004439
#define func_CSPCharPut                               0x00004451
#define func_CSPCharGetNonBlocking                    0x00004465
#define func_CSPCharPutNonBlocking                    0x0000447d
#define func_CSPClockModeSet                          0x00004495
#define func_CSPEnable                                0x000044a1
#define func_CSPDisable                               0x000044ab
#define func_CSPDrivenEdgeSet                         0x000044b5
#define func_CSPSyncValidLevelSet                     0x000044c7
#define func_CSPClockIdleModeSet                      0x000044d9
#define func_CSPClockIdleLevelSet                     0x000044e5
#define func_CSPPinModeSet                            0x000044f1
#define func_CSPRxEnable                              0x0000452f
#define func_CSPRxDisable                             0x00004539
#define func_CSPTxEnable                              0x00004543
#define func_CSPTxDisable                             0x0000454d
#define func_CSPIntRegister                           0x00004557
#define func_CSPIntUnregister                         0x00004571
#define func_CSPIntEnable                             0x00004587
#define func_CSPIntDisable                            0x0000458f
#define func_CSPIntStatus                             0x00004597
#define func_CSPIntClear                              0x0000459d
#define func_AESKeyLenSet                             0x000045a9
#define func_AESKeySet                                0x000045b9
#define func_AESIVSet                                 0x000045c1
#define func_AESAADLen0Set                            0x000045c9
#define func_AESAADLen1Set                            0x000045cf
#define func_AESPayloadLenSet                         0x000045d5
#define func_AESModeSet                               0x000045db
#define func_AESEncDecSet                             0x000045eb
#define func_AESDMAEn                                 0x000045fb
#define func_AESDMADis                                0x00004609
#define func_AESBlockDataInput                        0x00004617
#define func_AESBlockDataOutput                       0x0000461f
#define func_AESBlockStart                            0x00004627
#define func_AESKeyLoadWaitDone                       0x00004635
#define func_AESBlockTransWaitDone                    0x00004651
#define func_AESClockDiv2En                           0x00004661
#define func_AESClockDiv2Dis                          0x0000466f
#define func_AESCCMAuthLenSet                         0x0000467d
#define func_AESCCMLengthLenSet                       0x0000468f
#define func_AESTagGet                                0x000046a1
#define func_AESBlock                                 0x000046ab
#define func_AESECB                                   0x00004843
#define func_AESCCM                                   0x000049f5
#define func_AES_128_EEA2                             0x00004ccb
#define func_AES_CMAC_XOR_128                         0x00004e1b
#define func_AES_CMAC_Leftshift_Onebit                0x00004e35
#define func_AES_CMAC_Generate_Subkey                 0x00004e5b
#define func_AES_128_EIA2                             0x00004ed9
#define func_SEMAMasterEnable                         0x00005141
#define func_SEMAMasterDisable                        0x00005155
#define func_SEMARequest                              0x00005169
#define func_SEMARequestNonBlocking                   0x0000518b
#define func_SEMARequestPriSet                        0x0000519f
#define func_SEMAMasterGet                            0x000051ad
#define func_SEMASlaveGet                             0x000051bb
#define func_SEMAReset                                0x000051cd
#define func_SEMAReqQueueState                        0x000051db
#define func_SEMAMasterIntEnable                      0x000051e7
#define func_SEMARelease                              0x000051f3
#define func_SEMAIntRegister                          0x00005207
#define func_SEMAIntUnregister                        0x0000521f
#define func_MCNTStart                                0x00005235
#define func_MCNTStop                                 0x0000523d
#define func_MCNTSetCNT32K                            0x00005245
#define func_MCNTGetCNT32K                            0x0000524b
#define func_MCNTGetMCNT                              0x00005251
#define func_MCNTGetIntStatus                         0x00005257
#define func_MCNTSetIntStatus                         0x00005261
#define func_MCNTIntRegister                          0x00005267
#define func_MCNTIntUnregister                        0x0000527f
#define func_REG_Bus_Field_Set                        0x00005299
#define func_REG_Bus_Field_Get                        0x000053c9
#define func_PRCM_Clock_Mode_Force_XTAL               0x000054d3
#define func_PRCM_Clock_Mode_Auto                     0x000054ef
#define func_BBPLL_DEFAULT_SETUP                      0x000054fd


#define ROM_AESKeyLenSet                            ((void (*)(unsigned long ulKeyLenMode))func_AESKeyLenSet)
#define ROM_AESKeySet                               ((void (*)(unsigned char ucOffset, unsigned long ulKeyValue))func_AESKeySet)
#define ROM_AESIVSet                                ((void (*)(unsigned char ucOffset, unsigned long ucIV))func_AESIVSet)
#define ROM_AESAADLen0Set                           ((void (*)(unsigned long ulAadLen0))func_AESAADLen0Set)
#define ROM_AESAADLen1Set                           ((void (*)(unsigned long ulAadLen1))func_AESAADLen1Set)
#define ROM_AESPayloadLenSet                        ((void (*)(unsigned long ulPayloadLenByte))func_AESPayloadLenSet)
#define ROM_AESModeSet                              ((void (*)(unsigned long ucAESMode))func_AESModeSet)
#define ROM_AESEncDecSet                            ((void (*)(unsigned long ucAESEncDec))func_AESEncDecSet)
#define ROM_AESDMAEn                                ((void (*)(void))func_AESDMAEn)
#define ROM_AESDMADis                               ((void (*)(void))func_AESDMADis)
#define ROM_AESBlockDataInput                       ((void (*)(unsigned char ucOffset, unsigned long ulDataIn))func_AESBlockDataInput)
#define ROM_AESBlockDataOutput                      ((unsigned long (*)(unsigned char ucOffset))func_AESBlockDataOutput)
#define ROM_AESBlockStart                           ((void (*)(void))func_AESBlockStart)
#define ROM_AESKeyLoadWaitDone                      ((void (*)(void))func_AESKeyLoadWaitDone)
#define ROM_AESBlockTransWaitDone                   ((void (*)(void))func_AESBlockTransWaitDone)
#define ROM_AESClockDiv2En                          ((void (*)(void))func_AESClockDiv2En)
#define ROM_AESClockDiv2Dis                         ((void (*)(void))func_AESClockDiv2Dis)
#define ROM_AESCCMAuthLenSet                        ((void (*)(unsigned long ucAuthLenByte))func_AESCCMAuthLenSet)
#define ROM_AESCCMLengthLenSet                      ((void (*)(unsigned char ucLengthLenByte))func_AESCCMLengthLenSet)
#define ROM_AESTagGet                               ((unsigned long (*)(unsigned char ucOffset))func_AESTagGet)
#define ROM_AESBlock                                ((void (*)(unsigned char *pucInput, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput))func_AESBlock)
#define ROM_AESECB                                  ((void (*)(unsigned char *pucInput, unsigned long ulInputLenByte, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput))func_AESECB)
#define ROM_AESCCM                                  ((void (*)(unsigned char *aad, unsigned long aad_len_byte, unsigned char *input_message, unsigned long input_len_byte, unsigned long *user_key, unsigned long key_len_bits, unsigned long M, unsigned long L, unsigned char *nonce, unsigned char mode, unsigned char *output_message, unsigned char *digest))func_AESCCM)
#define ROM_AES_128_EEA2                            ((void (*)(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucOutMsg))func_AES_128_EEA2)
#define ROM_AES_CMAC_XOR_128                        ((void (*)(unsigned char *pucA, const unsigned char *pucB, unsigned char *pucOut))func_AES_CMAC_XOR_128)
#define ROM_AES_CMAC_Leftshift_Onebit               ((void (*)(unsigned char *pucInput, unsigned char *pucOutput))func_AES_CMAC_Leftshift_Onebit)
#define ROM_AES_CMAC_Generate_Subkey                ((void (*)(unsigned char *pucKey, unsigned char *pucK1, unsigned char *pucK2))func_AES_CMAC_Generate_Subkey)
#define ROM_AES_128_EIA2                            ((void (*)(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucMACT))func_AES_128_EIA2)
#define	ROM_AON_DSP_Load_Flag_Get                   ((unsigned char (*)(void))func_AON_DSP_Load_Flag_Get)
#define	ROM_AON_ARM_Load_Flag_Get                   ((unsigned char (*)(void))func_AON_ARM_Load_Flag_Get)
#define	ROM_AON_DSP_Load_Flag_Set                   ((void (*)(uint8_t flag))func_AON_DSP_Load_Flag_Set)
#define	ROM_AON_ARM_Load_Flag_Set                   ((void (*)(uint8_t flag))func_AON_ARM_Load_Flag_Set)
#define	ROM_AON_Flash_Delay_Get                     ((void (*)(void))func_AON_Flash_Delay_Get)
#define	ROM_AON_Flash_Delay_Set                     ((void (*)(uint8_t value))func_AON_Flash_Delay_Set)
#define	ROM_AON_Addr_Overlap_Unused_Mem             ((unsigned char (*)(unsigned char mem_status, unsigned long ulAddr, unsigned long ulSizebyte))func_AON_Addr_Overlap_Unused_Mem)
#define	ROM_BBPLL_DEFAULT_SETUP                     ((void (*)(void))func_BBPLL_DEFAULT_SETUP)
#define ROM_CSPRxFrameSet                           ((void (*)(unsigned long ulBase, unsigned long ulDelay, unsigned long ulDataLen, unsigned long ulFrameLen))func_CSPRxFrameSet)
#define ROM_CSPTxFrameSet                           ((void (*)(unsigned long ulBase, unsigned long ulDelay, unsigned long ulDataLen, unsigned long ulSyncLen, unsigned long ulFrameLen))func_CSPTxFrameSet)
#define ROM_CSPClockSet                             ((void (*)(unsigned long ulBase, unsigned long ulCSPMode, unsigned long ulPclk, unsigned ulClock))func_CSPClockSet)
#define ROM_CSPEndianModeSet                        ((void (*)(unsigned long ulBase, unsigned long ulEndianMode))func_CSPEndianModeSet)
#define ROM_CSPOperationModeSet                     ((void (*)(unsigned long ulBase, unsigned long ulOperationMode))func_CSPOperationModeSet)
#define ROM_CSPUARTModeSet                          ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned char ucDatabits, unsigned char ucStopbits))func_CSPUARTModeSet)
#define ROM_CSPIrdaEnable                           ((void (*)(unsigned long ulBase, unsigned long ulIrdaEn))func_CSPIrdaEnable)
#define ROM_CSPIrdaPulseWidthSet                    ((void (*)(unsigned long ulBase, unsigned long ulWidth))func_CSPIrdaPulseWidthSet)
#define ROM_CSPIrdaIdleLevelSet                     ((void (*)(unsigned long ulBase, unsigned long ulIrdaIdleLevel))func_CSPIrdaIdleLevelSet)
#define ROM_CSPIrdaMode                             ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned char ucDatabits, unsigned char ucStopbits, unsigned long ulIdleLevel, unsigned long ulWidth))func_CSPIrdaMode)
#define ROM_CSPSPIMode                              ((void (*)(unsigned long ulBase, unsigned long ulMode1, unsigned long ulMode2, unsigned long ulTxFrameCtl, unsigned long ulRxFrameCtl))func_CSPSPIMode)
#define ROM_CSPFIFOLevelSet                         ((void (*)(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel))func_CSPFIFOLevelSet)
#define ROM_CSPCharGet                              ((unsigned char (*)(unsigned long ulBase))func_CSPCharGet)
#define ROM_CSPCharPut                              ((void (*)(unsigned long ulBase, unsigned char ucData))func_CSPCharPut)
#define ROM_CSPCharGetNonBlocking                   ((long (*)(unsigned long ulBase))func_CSPCharGetNonBlocking)
#define ROM_CSPCharPutNonBlocking                   ((long (*)(unsigned long ulBase, unsigned char ucData))func_CSPCharPutNonBlocking)
#define ROM_CSPClockModeSet                         ((void (*)(unsigned long ulBase, unsigned long ulClockMode))func_CSPClockModeSet)
#define ROM_CSPEnable                               ((void (*)(unsigned long ulBase))func_CSPEnable)
#define ROM_CSPDisable                              ((void (*)(unsigned long ulBase))func_CSPDisable)
#define ROM_CSPDrivenEdgeSet                        ((void (*)(unsigned long ulBase, unsigned long ulRxEdge, unsigned long ulTxEdge))func_CSPDrivenEdgeSet)
#define ROM_CSPSyncValidLevelSet                    ((void (*)(unsigned long ulBase, unsigned long ulRFS, unsigned long ulTFS))func_CSPSyncValidLevelSet)
#define ROM_CSPClockIdleModeSet                     ((void (*)(unsigned long ulBase, unsigned long ulClkIdleMode))func_CSPClockIdleModeSet)
#define ROM_CSPClockIdleLevelSet                    ((void (*)(unsigned long ulBase, unsigned long ulClkIdleLevel))func_CSPClockIdleLevelSet)
#define ROM_CSPPinModeSet                           ((void (*)(unsigned long ulBase, unsigned long ulCSPPin, unsigned long ulPinMode, unsigned long ulPinDirection))func_CSPPinModeSet)
#define ROM_CSPRxEnable                             ((void (*)(unsigned long ulBase))func_CSPRxEnable)
#define ROM_CSPRxDisable                            ((void (*)(unsigned long ulBase))func_CSPRxDisable)
#define ROM_CSPTxEnable                             ((void (*)(unsigned long ulBase))func_CSPTxEnable)
#define ROM_CSPTxDisable                            ((void (*)(unsigned long ulBase))func_CSPTxDisable)
#define ROM_CSPIntRegister                          ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_CSPIntRegister)
#define ROM_CSPIntUnregister                        ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_CSPIntUnregister)
#define ROM_CSPIntEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntEnable)
#define ROM_CSPIntDisable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntDisable)
#define ROM_CSPIntStatus                            ((unsigned long (*)(unsigned long ulBase))func_CSPIntStatus)
#define ROM_CSPIntClear                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntClear)
#define ROM_DMAErrorStatusGet                       ((unsigned long (*)(unsigned long ulChannelNum))func_DMAErrorStatusGet)
#define ROM_DMAErrorStatusClear                     ((void (*)(unsigned long ulChannelNum))func_DMAErrorStatusClear)
#define ROM_DMAChannelRequest                       ((void (*)(unsigned long ulChannelNum))func_DMAChannelRequest)
#define ROM_DMAChannelControlSet                    ((void (*)(unsigned long ulChannelNum, unsigned long ulControl))func_DMAChannelControlSet)
#define ROM_DMAChannelTransferSet                   ((void (*)(unsigned long ulChannelNum, void *pvSrcAddr, void *pvDstAddr, unsigned long ulTransferSize))func_DMAChannelTransferSet)
#define ROM_DMAChannelNextPointerSet                ((void (*)(unsigned long ulChannelNum, void *pvNextPointer))func_DMAChannelNextPointerSet)
#define ROM_DMAIntRegister                          ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_DMAIntRegister)
#define ROM_DMAIntUnregister                        ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors))func_DMAIntUnregister)
#define ROM_DMAIntStatus                            ((unsigned long (*)(unsigned long ulChannelNum))func_DMAIntStatus)
#define ROM_DMAIntClear                             ((void (*)(unsigned long ulChanMask))func_DMAIntClear)
#define ROM_DMAChannelWaitIdle                      ((void (*)(unsigned long ulChannelNum))func_DMAChannelWaitIdle)
#define ROM_DMAChannelTransfer                      ((void (*)(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum))func_DMAChannelTransfer)
#define ROM_DMAWaitAESDone                          ((void (*)(void))func_DMAWaitAESDone)
#define ROM_DMAChannelMemcpy                        ((void (*)(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum))func_DMAChannelMemcpy)
#define ROM_FLASH_Init                              ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int ref_clk_hz, unsigned int sclk_hz))func_FLASH_Init)
#define ROM_FLASH_ChipErase                         ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_ChipErase)
#define ROM_FLASH_SectorErase                       ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_SectorErase)
#define ROM_FLASH_BlockErase32K                     ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_BlockErase32K)
#define ROM_FLASH_BlockErase64K                     ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_BlockErase64K)
#define ROM_FLASH_WriteData                         ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))func_FLASH_WriteData)
#define ROM_FLASH_ReadData                          ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))func_FLASH_ReadData)
#define ROM_FLASH_WaitIdle                          ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_WaitIdle)
#define ROM_FLASH_UpdateData                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum, unsigned long ulBackupFlag, unsigned ulExchangeAddr))func_FLASH_UpdateData)
#define ROM_FLASH_EnableQEFlag                      ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_EnableQEFlag)
#define ROM_FLASH_SetReadMode                       ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int mode))func_FLASH_SetReadMode)
#define ROM_FLASH_EnterXIPMode                      ((void (*)(QSPI_FLASH_Def *flash_vendor, char xip_dummy))func_FLASH_EnterXIPMode)
#define ROM_FLASH_ExitXIPMode                       ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_ExitXIPMode)
#define ROM_FLASH_SetWriteMode                      ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int mode))func_FLASH_SetWriteMode)
#define ROM_FLASH_GetDeviceID                       ((unsigned int (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_GetDeviceID)
#define ROM_GPIOPeripheralPad                       ((void (*)(unsigned char ucPeriNum, unsigned char ucPadNum))func_GPIOPeripheralPad)
#define ROM_GPIODirModeSet                          ((void (*)(unsigned char ucPins, unsigned long ulPinIO))func_GPIODirModeSet)
#define ROM_GPIOOutputODEn                          ((void (*)(unsigned char ucPins))func_GPIOOutputODEn)
#define ROM_GPIOOutputODDis                         ((void (*)(unsigned char ucPins))func_GPIOOutputODDis)
#define ROM_GPIOAnalogEn                            ((void (*)(unsigned char ucPins))func_GPIOAnalogEn)
#define ROM_GPIOAnalogDis                           ((void (*)(unsigned char ucPins))func_GPIOAnalogDis)
#define ROM_GPIOPullupEn                            ((void (*)(unsigned char ucPins))func_GPIOPullupEn)
#define ROM_GPIOPullupDis                           ((void (*)(unsigned char ucPins))func_GPIOPullupDis)
#define ROM_GPIOPulldownEn                          ((void (*)(unsigned char ucPins))func_GPIOPulldownEn)
#define ROM_GPIOPulldownDis                         ((void (*)(unsigned char ucPins))func_GPIOPulldownDis)
#define ROM_GPIOPinSet                              ((void (*)(unsigned char ucPins))func_GPIOPinSet)
#define ROM_GPIOPinClear                            ((void (*)(unsigned char ucPins))func_GPIOPinClear)
#define ROM_GPIOPinRead                             ((unsigned char (*)(unsigned char ucPins))func_GPIOPinRead)
#define ROM_GPIOIntEnable                           ((void (*)(unsigned char ucPins))func_GPIOIntEnable)
#define ROM_GPIOIntDisable                          ((void (*)(unsigned char ucPins))func_GPIOIntDisable)
#define ROM_GPIOIntTypeSet                          ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntTypeSet)
#define ROM_GPIOIntEdgeSet                          ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntEdgeSet)
#define ROM_GPIOIntSingleEdgeSet                    ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntSingleEdgeSet)
#define ROM_GPIOIntStatusGet                        ((unsigned long (*)(unsigned char ucPins))func_GPIOIntStatusGet)
#define ROM_GPIOIntMaskGet                          ((unsigned long (*)(unsigned char ucPins))func_GPIOIntMaskGet)
#define ROM_GPIOConflictStatusGet                   ((unsigned long (*)(unsigned char ucPins))func_GPIOConflictStatusGet)
#define ROM_GPIOConflictStatusClear                 ((void (*)(unsigned char ucPins))func_GPIOConflictStatusClear)
#define ROM_GPIOAllocationStatusGet                 ((unsigned long (*)(unsigned char ucPins))func_GPIOAllocationStatusGet)
#define ROM_GPIODrvStrengthSet                      ((void (*)(unsigned char ucPins, unsigned char ucDrvStrength))func_GPIODrvStrengthSet)
#define ROM_GPIOIntRegister                         ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_GPIOIntRegister)
#define ROM_GPIOIntUnregister                       ((void (*)(unsigned long *g_pRAMVectors))func_GPIOIntUnregister)
#define	ROM_GPIOModeSet                             ((void (*)(unsigned char ucPins, unsigned char ucMode))func_GPIOModeSet)
#define	ROM_GPIOPadConfigSel                        ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOPadConfigSel)
#define	ROM_GPIOPadModeSet                          ((void (*)(unsigned char ucPins, unsigned char ucMode, unsigned char ucConfig))func_GPIOPadModeSet)
#define	ROM_GPIODirectionSet                        ((void (*)(unsigned char ucPins, unsigned long ulPinIO))func_GPIODirectionSet)
#define ROM_I2CMasterEnable                         ((void (*)(unsigned long ulBase))func_I2CMasterEnable)
#define ROM_I2CMasterInitExpClk                     ((void (*)(unsigned long ulBase, unsigned long ulI2CClk, unsigned char bFast))func_I2CMasterInitExpClk)
#define ROM_I2CSlaveEnable                          ((void (*)(unsigned long ulBase))func_I2CSlaveEnable)
#define ROM_I2CSlaveInit                            ((void (*)(unsigned long ulBase, unsigned long ucSlaveAddr))func_I2CSlaveInit)
#define ROM_I2CIntRegister                          ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(fnHandler)(void)))func_I2CIntRegister)
#define ROM_I2CIntUnregister                        ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_I2CIntUnregister)
#define ROM_I2CIntEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_I2CIntEnable)
#define ROM_I2CIntDisable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_I2CIntDisable)
#define ROM_I2CIntStatus                            ((unsigned char (*)(unsigned long ulBase, unsigned char bMasked))func_I2CIntStatus)
#define ROM_I2CIntClear                             ((void (*)(unsigned long ulBase))func_I2CIntClear)
#define ROM_I2CAddrSet                              ((void (*)(unsigned long ulBase, unsigned long ulAddr, unsigned char ucNorMode, unsigned char ucReceive))func_I2CAddrSet)
#define ROM_I2CBusBusy                              ((unsigned char (*)(unsigned long ulBase))func_I2CBusBusy)
#define ROM_I2CControlSet                           ((void (*)(unsigned long ulBase, unsigned long ulCmd))func_I2CControlSet)
#define ROM_I2CControlGet                           ((unsigned long (*)(unsigned long ulBase))func_I2CControlGet)
#define ROM_I2CDataPut                              ((void (*)(unsigned long ulBase, unsigned char ucData))func_I2CDataPut)
#define ROM_I2CDataGet                              ((unsigned long (*)(unsigned long ulBase))func_I2CDataGet)
#define ROM_I2CStatus                               ((unsigned long (*)(unsigned long ulBase))func_I2CStatus)
#define ROM_I2CACKModeSet                           ((void (*)(unsigned long ulBase, unsigned char ucEnable))func_I2CACKModeSet)
#define ROM_I2CFifoClear                            ((void (*)(unsigned long ulBase))func_I2CFifoClear)
#define ROM_IntRegister                             ((void (*)(unsigned long ulInterrupt, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_IntRegister)
#define ROM_IntUnregister                           ((void (*)(unsigned long ulInterrupt, unsigned long *g_pRAMVectors))func_IntUnregister)
#define ROM_IntPriorityGroupingSet                  ((void (*)(unsigned long ulBits))func_IntPriorityGroupingSet)
#define ROM_IntPriorityGroupingGet                  ((unsigned long (*)(void))func_IntPriorityGroupingGet)
#define ROM_IntPrioritySet                          ((void (*)(unsigned long ulInterrupt,unsigned char ucPriority))func_IntPrioritySet)
#define ROM_IntPriorityGet                          ((long (*)(unsigned long ulInterrupt))func_IntPriorityGet)
#define ROM_IntEnable                               ((void (*)(unsigned long ulInterrupt))func_IntEnable)
#define ROM_IntDisable                              ((void (*)(unsigned long ulInterrupt))func_IntDisable)
#define ROM_IntPendSet                              ((void (*)(unsigned long ulInterrupt))func_IntPendSet)
#define ROM_IntPendClear                            ((void (*)(unsigned long ulInterrupt))func_IntPendClear)
#define	ROM_MCNTStart                               ((void (*)(void))func_MCNTStart)
#define	ROM_MCNTStop                                ((void (*)(void))func_MCNTStop)
#define	ROM_MCNTSetCNT32K                           ((void (*)(unsigned long ulCounter))func_MCNTSetCNT32K)
#define	ROM_MCNTGetCNT32K                           ((unsigned long (*)(void))func_MCNTGetCNT32K)
#define	ROM_MCNTGetMCNT                             ((unsigned long (*)(void))func_MCNTGetMCNT)
#define	ROM_MCNTGetIntStatus                        ((unsigned char (*)(void))func_MCNTGetIntStatus)
#define	ROM_MCNTSetIntStatus                        ((void (*)(unsigned char ucIntstatus))func_MCNTSetIntStatus)
#define	ROM_MCNTIntRegister                         ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_MCNTIntRegister)
#define	ROM_MCNTIntUnregister                       ((void (*)(unsigned long *g_pRAMVectors))func_MCNTIntUnregister)
#define ROM_PRCM_Clock_Enable                       ((void (*)(unsigned long ulBase, unsigned long ulModule))func_PRCM_Clock_Enable)
#define ROM_PRCM_Clock_Disable                      ((void (*)(unsigned long ulBase, unsigned long ulModule))func_PRCM_Clock_Disable)
#define ROM_PRCM_CHIP_VER_GET                       ((unsigned long (*)(unsigned long ulBase))func_PRCM_CHIP_VER_GET)
#define ROM_PRCM_BOOTMODE_GET                       ((unsigned char (*)(unsigned long ulBase))func_PRCM_BOOTMODE_GET)
#define ROM_PRCM_Clock_Mode_Force_XTAL              ((void (*)(void))func_PRCM_Clock_Mode_Force_XTAL)
#define ROM_PRCM_Clock_Mode_Auto                    ((void (*)(void))func_PRCM_Clock_Mode_Auto)
#define ROM_QSPIControllerEnable                    ((void (*)(unsigned char *reg_base_addr))func_QSPIControllerEnable)
#define ROM_QSPIControllerDisable                   ((void (*)(unsigned char *reg_base_addr))func_QSPIControllerDisable)
#define ROM_QSPIWaitIdle                            ((void (*)(unsigned char *reg_base))func_QSPIWaitIdle)
#define ROM_QSPIReaddataCapture                     ((void (*)(unsigned char *reg_base,unsigned int bypass, unsigned int delay))func_QSPIReaddataCapture)
#define ROM_QSPIConfigBaudrateDiv                   ((void (*)(unsigned char *reg_base,unsigned int ref_clk_hz, unsigned int sclk_hz))func_QSPIConfigBaudrateDiv)
#define ROM_QSPISetClkMode                          ((void (*)(unsigned char *reg_base, uint mode))func_QSPISetClkMode)
#define ROM_QSPIChipSelect                          ((void (*)(unsigned char *reg_base,unsigned int chip_select, unsigned int decoder_enable))func_QSPIChipSelect)
#define ROM_QSPIDelay                               ((void (*)(unsigned char *reg_base,unsigned int ref_clk, unsigned int sclk_hz,unsigned int tshsl_ns, unsigned int tsd2d_ns,unsigned int tchsh_ns, unsigned int tslch_ns))func_QSPIDelay)
#define ROM_QSPIControllerInit                      ((void (*)(QSPI_FLASH_Def *plat))func_QSPIControllerInit)
#define ROM_QSPICommandRead                         ((int (*)(unsigned char *reg_base_addr,unsigned int cmdlen, const u8 *cmdbuf, unsigned int rxlen, u8 *rxbuf))func_QSPICommandRead)
#define ROM_QSPICommandWrite                        ((int (*)(unsigned char *reg_base_addr,unsigned int cmdlen, const u8 *cmdbuf,unsigned int txlen,  const u8 *txbuf))func_QSPICommandWrite)
#define ROM_QSPIIndirectReadSetup                   ((int (*)(QSPI_FLASH_Def *plat,unsigned int cmdlen, unsigned int rx_width, const u8 *cmdbuf))func_QSPIIndirectReadSetup)
#define ROM_QSPIIndirectReadExecute                 ((int (*)(QSPI_FLASH_Def *plat,unsigned int rxlen, u8 *rxbuf))func_QSPIIndirectReadExecute)
#define ROM_QSPIIndirectWriteSetup                  ((int (*)(QSPI_FLASH_Def *plat,unsigned int cmdlen, const u8 *cmdbuf))func_QSPIIndirectWriteSetup)
#define ROM_QSPIIndirectWriteExecute                ((int (*)(QSPI_FLASH_Def *plat,unsigned int txlen, const u8 *txbuf))func_QSPIIndirectWriteExecute)
#define ROM_QSPIEnterXIP                            ((void (*)(unsigned char *reg_base, char xip_dummy))func_QSPIEnterXIP)
#define ROM_QSPIExitXIP                             ((void (*)(unsigned char *reg_base))func_QSPIExitXIP)
#define ROM_QSPIIndirectEnable                      ((void (*)(QSPI_FLASH_Def *plat))func_QSPIIndirectEnable)
#define ROM_QSPIIndirectDisable                     ((void (*)(QSPI_FLASH_Def *plat))func_QSPIIndirectDisable)
#define ROM_QSPIIndirectWrite                       ((void (*)(QSPI_FLASH_Def *plat, unsigned int dst, unsigned int n_bytes))func_QSPIIndirectWrite)
#define ROM_QSPIIndirectRead                        ((void (*)(QSPI_FLASH_Def *plat, unsigned int src, unsigned int n_bytes))func_QSPIIndirectRead)
#define ROM_QSPISetReadInstruction                  ((void (*)(unsigned char *reg_base, unsigned int config))func_QSPISetReadInstruction)
#define ROM_QSPISetWriteInstruction                 ((void (*)(unsigned char *reg_base, unsigned int config))func_QSPISetWriteInstruction)
#define ROM_QSPIIntEnable                           ((void (*)(unsigned char *reg_base, unsigned int intflags))func_QSPIIntEnable)
#define ROM_QSPIIntDisable                          ((void (*)(unsigned char *reg_base, unsigned int intflags))func_QSPIIntDisable)
#define ROM_QSPIIntStatusGet                        ((unsigned long (*)(unsigned char *reg_base))func_QSPIIntStatusGet)
#define ROM_QSPIIntRegister                         ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_QSPIIntRegister)
#define ROM_QSPIIntUnregister                       ((void (*)(unsigned long *g_pRAMVectors))func_QSPIIntUnregister)
#define ROM_REG_Bus_Field_Set                       ((unsigned char (*)(unsigned long ulRegBase, uint8_t ucBit_high, uint8_t ucBit_low, unsigned long ulValue))func_REG_Bus_Field_Set)
#define ROM_REG_Bus_Field_Get                       ((unsigned char (*)(unsigned long ulRegBase, uint8_t ucBit_high, uint8_t ucBit_low, unsigned long *ulValue))func_REG_Bus_Field_Get)
#define ROM_SEMAMasterEnable                        ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave))func_SEMAMasterEnable)
#define ROM_SEMAMasterDisable                       ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave))func_SEMAMasterDisable)
#define ROM_SEMARequest                             ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARequest)
#define ROM_SEMARequestNonBlocking                  ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARequestNonBlocking)
#define ROM_SEMARequestPriSet                       ((void (*)(unsigned long ulBase, unsigned char ucSemaSlave, unsigned long ucPriority))func_SEMARequestPriSet)
#define ROM_SEMAMasterGet                           ((unsigned char (*)(unsigned long ulBase, unsigned char ucSemaSlave))func_SEMAMasterGet)
#define ROM_SEMASlaveGet                            ((unsigned long (*)(unsigned long ulBase, unsigned char ucSemaMaster))func_SEMASlaveGet)
#define ROM_SEMAReset                               ((void (*)(unsigned long ulBase))func_SEMAReset)
#define ROM_SEMAReqQueueState                       ((unsigned char (*)(unsigned long ulBase))func_SEMAReqQueueState)
#define ROM_SEMAMasterIntEnable                     ((void (*)(unsigned long ulBase, unsigned long ulSemaMasterInt))func_SEMAMasterIntEnable)
#define ROM_SEMARelease                             ((void (*)(unsigned long ulBase, unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARelease)
#define ROM_SEMAIntRegister                         ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_SEMAIntRegister)
#define ROM_SEMAIntUnregister                       ((void (*)(unsigned long *g_pRAMVectors))func_SEMAIntUnregister)
#define ROM_SPIConfigSetExpClk                      ((void (*)(unsigned long ulBase, unsigned long ulDivide, unsigned long ulProtocol, unsigned long ulMode,unsigned long ulDataWidth))func_SPIConfigSetExpClk)
#define ROM_SPIEnable                               ((void (*)(unsigned long ulBase))func_SPIEnable)
#define ROM_SPIDisable                              ((void (*)(unsigned long ulBase))func_SPIDisable)
#define ROM_SPIIntRegister                          ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_SPIIntRegister)
#define ROM_SPIIntUnregister                        ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_SPIIntUnregister)
#define ROM_SPIIntEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntEnable)
#define ROM_SPIIntDisable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntDisable)
#define ROM_SPIIntStatus                            ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))func_SPIIntStatus)
#define ROM_SPIIntClear                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntClear)
#define ROM_SPIDataPut                              ((void (*)(unsigned long ulBase, unsigned long ulData))func_SPIDataPut)
#define ROM_SPIDataPutNonBlocking                   ((long (*)(unsigned long ulBase, unsigned long ulData))func_SPIDataPutNonBlocking)
#define ROM_SPIDataGet                              ((void (*)(unsigned long ulBase, unsigned long *pulData))func_SPIDataGet)
#define ROM_SPIDataGetNonBlocking                   ((long (*)(unsigned long ulBase, unsigned long *pulData))func_SPIDataGetNonBlocking)
#define ROM_SPISetTxFifoThreshold                   ((void (*)(unsigned long ulBase, unsigned long ulThreshold))func_SPISetTxFifoThreshold)
#define ROM_SPISetRxFifoThreshold                   ((void (*)(unsigned long ulBase, unsigned long ulThreshold))func_SPISetRxFifoThreshold)
#define ROM_SPISetDelay                             ((void (*)(unsigned long ulBase, unsigned long ulDelay))func_SPISetDelay)
#define ROM_SPIEnManualCS                           ((void (*)(unsigned long ulBase))func_SPIEnManualCS)
#define ROM_SPIDisManualCS                          ((void (*)(unsigned long ulBase))func_SPIDisManualCS)
#define ROM_SPIEnManualTransmit                     ((void (*)(unsigned long ulBase))func_SPIEnManualTransmit)
#define ROM_SPIDisManualTransmit                    ((void (*)(unsigned long ulBase))func_SPIDisManualTransmit)
#define ROM_SPIStartManualTransmit                  ((void (*)(unsigned long ulBase))func_SPIStartManualTransmit)
#define ROM_SPIEnModeFail                           ((void (*)(unsigned long ulBase))func_SPIEnModeFail)
#define ROM_SPIDisModeFail                          ((void (*)(unsigned long ulBase))func_SPIDisModeFail)
#define ROM_SysTickEnable                           ((void (*)(void))func_SysTickEnable)
#define ROM_SysTickDisable                          ((void (*)(void))func_SysTickDisable)
#define ROM_SysTickIntRegister                      ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_SysTickIntRegister)
#define ROM_SysTickIntUnregister                    ((void (*)(unsigned long *g_pRAMVectors))func_SysTickIntUnregister)
#define ROM_SysTickIntEnable                        ((void (*)(void))func_SysTickIntEnable)
#define ROM_SysTickIntDisable                       ((void (*)(void))func_SysTickIntDisable)
#define ROM_SysTickPeriodSet                        ((void (*)(unsigned long ulPeriod))func_SysTickPeriodSet)
#define ROM_SysTickPeriodGet                        ((unsigned long (*)(void))func_SysTickPeriodGet)
#define ROM_SysTickValueGet                         ((unsigned long (*)(void))func_SysTickValueGet)
#define ROM_TimerEnable                             ((void (*)(unsigned long ulBase))func_TimerEnable)
#define ROM_TimerDisable                            ((void (*)(unsigned long ulBase))func_TimerDisable)
#define ROM_TimerConfigure                          ((void (*)(unsigned long ulBase, unsigned long ulConfig))func_TimerConfigure)
#define ROM_TimerControlLevel                       ((void (*)(unsigned long ulBase, unsigned char bInvert))func_TimerControlLevel)
#define ROM_TimerPrescaleSet                        ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerPrescaleSet)
#define ROM_TimerPrescaleGet                        ((unsigned long (*)(unsigned long ulBase))func_TimerPrescaleGet)
#define ROM_TimerLoadSet                            ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerLoadSet)
#define ROM_TimerValueGet                           ((unsigned long (*)(unsigned long ulBase))func_TimerValueGet)
#define ROM_TimerMatchSet                           ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerMatchSet)
#define ROM_TimerMatchGet                           ((unsigned long (*)(unsigned long ulBase))func_TimerMatchGet)
#define ROM_TimerPWMSet                             ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerPWMSet)
#define ROM_TimerPWMGet                             ((unsigned long (*)(unsigned long ulBase))func_TimerPWMGet)
#define ROM_TimerIntRegister                        ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_TimerIntRegister)
#define ROM_TimerIntUnregister                      ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors))func_TimerIntUnregister)
#define ROM_TimerIntEnable                          ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_TimerIntEnable)
#define	ROM_UARTParityModeSet                       ((void (*)(unsigned long ulBase, unsigned long ulParity))func_UARTParityModeSet)
#define	ROM_UARTParityModeGet                       ((unsigned long (*)(unsigned long ulBase))func_UARTParityModeGet)
#define	ROM_UARTFIFOLevelSet                        ((void (*)(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel))func_UARTFIFOLevelSet)
#define	ROM_UARTFIFOLevelGet                        ((void (*)(unsigned long ulBase, unsigned long *pulTxLevel, unsigned long *pulRxLevel))func_UARTFIFOLevelGet)
#define	ROM_UARTEnable                              ((void (*)(unsigned long ulBase))func_UARTEnable)
#define	ROM_UARTDisable                             ((void (*)(unsigned long ulBase))func_UARTDisable)
#define	ROM_UARTConfigSetExpClk                     ((void (*)(unsigned long ulBase, unsigned long ulPreDivide, unsigned long ulCD, unsigned long ulBDIV, unsigned long ulConfig))func_UARTConfigSetExpClk)
#define	ROM_UARTConfigGetExpClk                     ((void (*)(unsigned long ulBase, unsigned long *pulPreDivide, unsigned long *pulCD, unsigned long *pulBDIV, unsigned long *pulConfig))func_UARTConfigGetExpClk)
#define	ROM_UARTCharsAvail                          ((unsigned char (*)(unsigned long ulBase))func_UARTCharsAvail)
#define	ROM_UARTSpaceAvail                          ((unsigned char (*)(unsigned long ulBase))func_UARTSpaceAvail)
#define	ROM_UARTCharGetNonBlocking                  ((long (*)(unsigned long ulBase))func_UARTCharGetNonBlocking)
#define	ROM_UARTCharGet                             ((long (*)(unsigned long ulBase))func_UARTCharGet)
#define	ROM_UARTNCharGet                            ((void (*)(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte))func_UARTNCharGet)
#define	ROM_UARTCharGetWithTimeout                  ((unsigned long (*)(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte))func_UARTCharGetWithTimeout)
#define	ROM_UARTCharPutNonBlocking                  ((unsigned char (*)(unsigned long ulBase, unsigned char ucData))func_UARTCharPutNonBlocking)
#define	ROM_UARTCharPut                             ((void (*)(unsigned long ulBase, unsigned char ucData))func_UARTCharPut)
#define	ROM_UARTNCharPut                            ((void (*)(unsigned long ulBase, unsigned char *pucData, unsigned long ulLenByte))func_UARTNCharPut)
#define	ROM_UARTBreakCtl                            ((void (*)(unsigned long ulBase, unsigned char bBreakState))func_UARTBreakCtl)
#define	ROM_UARTIntRegister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_UARTIntRegister)
#define	ROM_UARTIntUnregister                       ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_UARTIntUnregister)
#define	ROM_UARTIntEnable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_UARTIntEnable)
#define	ROM_UARTIntDisable                          ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_UARTIntDisable)
#define	ROM_UARTIntStatus                           ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))func_UARTIntStatus)
#define	ROM_UARTIntClear                            ((void (*)(unsigned long ulBase))func_UARTIntClear)
#define	ROM_UARTRxErrorGet                          ((unsigned long (*)(unsigned long ulBase))func_UARTRxErrorGet)
#define	ROM_UARTRxErrorClear                        ((void (*)(unsigned long ulBase))func_UARTRxErrorClear)
#define	ROM_UARTStringPut                           ((void (*)(unsigned long ulBase, unsigned char *str))func_UARTStringPut)
#define	ROM_UARTConfigSetBaudrate                   ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned long ulConfig))func_UARTConfigSetBaudrate)
#define	ROM_UARTAutoBaudrate                        ((void (*)(unsigned long ulBase))func_UARTAutoBaudrate)
#define	ROM_UARTConfigSetMode                       ((void (*)(unsigned long ulBase, unsigned long ulConfig))func_UARTConfigSetMode)
#define	ROM_UARTWaitTxDone                          ((void (*)(unsigned long ulBase))func_UARTWaitTxDone)
#define	ROM_UARTPrintf                              ((void (*)(unsigned long ulBase, char* fmt, ...))func_UARTPrintf)
#define ROM_UTCTimerStop                            ((void (*)(void))func_UTCTimerStop)
#define ROM_UTCTimerRun                             ((void (*)(void))func_UTCTimerRun)
#define ROM_UTCCalStop                              ((void (*)(void))func_UTCCalStop)
#define ROM_UTCCalRun                               ((void (*)(void))func_UTCCalRun)
#define ROM_UTCDivStop                              ((void (*)(void))func_UTCDivStop)
#define ROM_UTCDivEn                                ((void (*)(void))func_UTCDivEn)
#define ROM_UTCHourModeSet                          ((void (*)(unsigned long ulMode))func_UTCHourModeSet)
#define ROM_UTCHourModeGet                          ((unsigned long (*)(void))func_UTCHourModeGet)
#define ROM_UTCTimerSet                             ((void (*)(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec))func_UTCTimerSet)
#define ROM_UTCTimerChangeGet                       ((unsigned long (*)(void))func_UTCTimerChangeGet)
#define ROM_UTCTimerGet                             ((void (*)(unsigned char *ulAMPM, unsigned char *ulHour, unsigned char *ulMin, unsigned char *ulSec, unsigned char *ulMinSec))func_UTCTimerGet)
#define ROM_UTCTimerAlarmSet                        ((void (*)(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec))func_UTCTimerAlarmSet)
#define ROM_UTCCalSet                               ((void (*)(unsigned long ulCentury, unsigned long ulYear, unsigned long ulMonth, unsigned long ulData, unsigned long ulDay))func_UTCCalSet)
#define ROM_UTCCalChangeGet                         ((unsigned long (*)(void))func_UTCCalChangeGet)
#define ROM_UTCCalGet                               ((void (*)(unsigned char *ulCentury, unsigned char *ulYear, unsigned char *ulMonth, unsigned char *ulData, unsigned char *ulDay))func_UTCCalGet)
#define ROM_UTCCalAlarmSet                          ((void (*)(unsigned long ulMonth, unsigned long ulData))func_UTCCalAlarmSet)
#define ROM_UTCAlarmEnable                          ((void (*)(unsigned long ulAlarmFlags))func_UTCAlarmEnable)
#define ROM_UTCAlarmDisable                         ((void (*)(unsigned long ulAlarmFlags))func_UTCAlarmDisable)
#define ROM_UTCIntEnable                            ((void (*)(unsigned long ulIntFlags))func_UTCIntEnable)
#define ROM_UTCIntDisable                           ((void (*)(unsigned long ulIntFlags))func_UTCIntDisable)
#define ROM_UTCIntMaskSet                           ((void (*)(unsigned long ulIntMask))func_UTCIntMaskSet)
#define ROM_UTCIntMaskGet                           ((unsigned long (*)(void))func_UTCIntMaskGet)
#define ROM_UTCIntStatusSet                         ((void (*)(unsigned long ulIntFlags))func_UTCIntStatusSet)
#define ROM_UTCIntStatusGet                         ((unsigned long (*)(void))func_UTCIntStatusGet)
#define ROM_UTCValidStatusGet                       ((unsigned long (*)(void))func_UTCValidStatusGet)
#define ROM_UTCKeepRun                              ((void (*)(unsigned long ulKeepUTC))func_UTCKeepRun)
#define ROM_UTCClkCntSet                            ((void (*)(unsigned long ulClkCnt))func_UTCClkCntSet)
#define ROM_UTCClkCntGet                            ((unsigned long (*)(void))func_UTCClkCntGet)
#define ROM_UTCIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_UTCIntRegister)
#define ROM_UTCIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))func_UTCIntUnregister)
#define ROM_WatchdogRunning                         ((unsigned char (*)(unsigned long ulBase))func_WatchdogRunning)
#define ROM_WatchdogEnable                          ((void (*)(unsigned long ulBase))func_WatchdogEnable)
#define ROM_WatchdogResetEnable                     ((void (*)(unsigned long ulBase))func_WatchdogResetEnable)
#define ROM_WatchdogResetDisable                    ((void (*)(unsigned long ulBase))func_WatchdogResetDisable)
#define ROM_WatchdogTimerRepeatEnable               ((void (*)(unsigned long ulBase))func_WatchdogTimerRepeatEnable)
#define ROM_WatchdogTimerRepeatDisable              ((void (*)(unsigned long ulBase))func_WatchdogTimerRepeatDisable)
#define ROM_WatchdogReloadSet                       ((void (*)(unsigned long ulBase, unsigned long ulLoadVal))func_WatchdogReloadSet)
#define ROM_WatchdogValueGet                        ((unsigned long (*)(unsigned long ulBase))func_WatchdogValueGet)
#define ROM_WatchdogIntRegister                     ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_WatchdogIntRegister)
#define ROM_WatchdogIntUnregister                   ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_WatchdogIntUnregister)
#define ROM_WatchdogIntEnable                       ((void (*)(unsigned long ulBase))func_WatchdogIntEnable)
#define ROM_WatchdogIntStatus                       ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))func_WatchdogIntStatus)
#define ROM_WatchdogIntClear                        ((void (*)(unsigned long ulBase))func_WatchdogIntClear)
#define ROM_WatchdogDisable                         ((void (*)(unsigned long ulBase))func_WatchdogDisable)
#endif


#ifdef ROM_VERSION_2_1
#define func_QSPIIntRegister                          0x00002bed
#define func_QSPIIntUnregister                        0x00002c05
#define func_UARTParityModeSet                        0x00002c19
#define func_UARTParityModeGet                        0x00002c25
#define func_UARTFIFOLevelSet                         0x00002c2f
#define func_UARTFIFOLevelGet                         0x00002c35
#define func_UARTEnable                               0x00002c49
#define func_UARTDisable                              0x00002c65
#define func_UARTConfigSetExpClk                      0x00002c6f
#define func_UARTConfigGetExpClk                      0x00002cb1
#define func_UARTCharsAvail                           0x00002cd5
#define func_UARTSpaceAvail                           0x00002ce1
#define func_UARTCharGetNonBlocking                   0x00002ced
#define func_UARTCharGet                              0x00002d01
#define func_UARTNCharGet                             0x00002d13
#define func_UARTCharGetWithTimeout                   0x00002d33
#define func_UARTCharPutNonBlocking                   0x00002d6b
#define func_UARTCharPut                              0x00002d7f
#define func_UARTNCharPut                             0x00002d8f
#define func_UARTBreakCtl                             0x00002daf
#define func_UARTIntRegister                          0x00002dc3
#define func_UARTIntUnregister                        0x00002de3
#define func_UARTIntEnable                            0x00002dfb
#define func_UARTIntDisable                           0x00002e03
#define func_UARTIntStatus                            0x00002e0b
#define func_UARTIntClear                             0x00002e17
#define func_UARTRxErrorGet                           0x00002e1f
#define func_UARTRxErrorClear                         0x00002e29
#define func_UARTStringPut                            0x00002e35
#define func_UARTConfigSetBaudrate                    0x00002e4f
#define func_UARTAutoBaudrate                         0x00002ec3
#define func_UARTConfigSetMode                        0x00002f4b
#define func_UARTWaitTxDone                           0x00002f59
#define func_UARTPrintf                               0x000030f9
#define func_IntRegister                              0x00003229
#define func_IntUnregister                            0x00003255
#define func_IntPriorityGroupingSet                   0x0000325d
#define func_IntPriorityGroupingGet                   0x0000326f
#define func_IntPrioritySet                           0x00003293
#define func_IntPriorityGet                           0x000032c1
#define func_IntEnable                                0x000032d7
#define func_IntDisable                               0x0000335d
#define func_IntPendSet                               0x000033e5
#define func_IntPendClear                             0x00003455
#define func_TimerEnable                              0x000034cd
#define func_TimerDisable                             0x000034d7
#define func_TimerConfigure                           0x000034e1
#define func_TimerControlLevel                        0x000034ed
#define func_TimerPrescaleSet                         0x00003501
#define func_TimerPrescaleGet                         0x0000350d
#define func_TimerLoadSet                             0x00003517
#define func_TimerValueGet                            0x0000351b
#define func_TimerMatchSet                            0x00003521
#define func_TimerMatchGet                            0x00003525
#define func_TimerPWMSet                              0x0000352b
#define func_TimerPWMGet                              0x0000352f
#define func_TimerIntRegister                         0x00003535
#define func_TimerIntUnregister                       0x0000354f
#define func_TimerIntEnable                           0x00003565
#define func_SysTickEnable                            0x00003571
#define func_SysTickDisable                           0x00003583
#define func_SysTickIntRegister                       0x00003595
#define func_SysTickIntUnregister                     0x000035bd
#define func_SysTickIntEnable                         0x000035e1
#define func_SysTickIntDisable                        0x000035f3
#define func_SysTickPeriodSet                         0x00003605
#define func_SysTickPeriodGet                         0x0000360f
#define func_SysTickValueGet                          0x00003619
#define func_DMAErrorStatusGet                        0x00003621
#define func_DMAErrorStatusClear                      0x00003633
#define func_DMAChannelRequest                        0x0000364d
#define func_DMAChannelControlSet                     0x00003697
#define func_DMAChannelTransferSet                    0x000036a3
#define func_DMAChannelNextPointerSet                 0x000036c1
#define func_DMAIntRegister                           0x000036cd
#define func_DMAIntUnregister                         0x000036e7
#define func_DMAIntStatus                             0x000036fd
#define func_DMAIntClear                              0x0000372d
#define func_DMAChannelWaitIdle                       0x00003757
#define func_DMAChannelTransfer                       0x00003783
#define func_DMAWaitAESDone                           0x000037af
#define func_DMAWaitChannelDone                       0x000037cb
#define func_GetDMAChannelTransCnt                    0x000037e3
#define func_SPIConfigSetExpClk                       0x000037f5
#define func_SPIEnable                                0x0000380b
#define func_SPIDisable                               0x00003811
#define func_SPIIntRegister                           0x00003817
#define func_SPIIntUnregister                         0x00003837
#define func_SPIIntEnable                             0x0000384f
#define func_SPIIntDisable                            0x00003857
#define func_SPIIntStatus                             0x0000385f
#define func_SPIIntClear                              0x0000386b
#define func_SPIWordDataPut                           0x0000386f
#define func_SPIHalfWordDataPut                       0x0000387f
#define func_SPIByteDataPut                           0x0000388f
#define func_SPIDataPutNonBlocking                    0x0000389f
#define func_SPIWordDataGet                           0x000038b3
#define func_SPIHalfWordDataGet                       0x000038c5
#define func_SPIByteDataGet                           0x000038d7
#define func_SPIDataGetNonBlocking                    0x000038eb
#define func_SPISetTxFifoThreshold                    0x00003901
#define func_SPISetRxFifoThreshold                    0x00003905
#define func_SPISetDelay                              0x00003909
#define func_SPIEnManualCS                            0x0000390d
#define func_SPIDisManualCS                           0x00003917
#define func_SPIEnManualTransmit                      0x00003921
#define func_SPIDisManualTransmit                     0x0000392b
#define func_SPIStartManualTransmit                   0x00003935
#define func_SPIEnModeFail                            0x0000393f
#define func_SPIDisModeFail                           0x00003949
#define func_SPITxFifoFlush                           0x00003953
#define func_SPITxFifoNormal                          0x0000395d
#define func_SPIRxFifoFlush                           0x00003967
#define func_SPIRxFifoNormal                          0x00003971
#define func_WatchdogRunning                          0x0000397d
#define func_WatchdogEnable                           0x00003987
#define func_WatchdogResetEnable                      0x00003995
#define func_WatchdogResetDisable                     0x000039a3
#define func_WatchdogTimerRepeatEnable                0x000039b1
#define func_WatchdogTimerRepeatDisable               0x000039bf
#define func_WatchdogReloadSet                        0x000039cd
#define func_WatchdogValueGet                         0x000039d3
#define func_WatchdogIntRegister                      0x000039d9
#define func_WatchdogIntUnregister                    0x000039f1
#define func_WatchdogIntEnable                        0x00003a05
#define func_WatchdogIntStatus                        0x00003a13
#define func_WatchdogIntClear                         0x00003a27
#define func_WatchdogDisable                          0x00003a31
#define func_flash_vendor_delay                       0x00003a45
#define func_FLASH_Init                               0x00003a51
#define func_FLASH_ChipErase                          0x00003aa5
#define func_FLASH_SectorErase                        0x00003b09
#define func_FLASH_BlockErase32K                      0x00003b87
#define func_FLASH_BlockErase64K                      0x00003c05
#define func_FLASH_WriteData                          0x00003c83
#define func_FLASH_WaitIdle                           0x00003d8b
#define func_FLASH_FAST_WriteData                     0x00003dc7
#define func_FLASH_ReadData                           0x00003e13
#define func_FLASH_UpdateData                         0x00003e93
#define func_FLASH_EnableQEFlag                       0x00003fa1
#define func_FLASH_SetReadMode                        0x00004075
#define func_FLASH_EnterXIPMode                       0x00004085
#define func_FLASH_ExitXIPMode                        0x000040d1
#define func_FLASH_SetWriteMode                       0x00004115
#define func_FLASH_GetDeviceID                        0x00004125
#define func_FLASH_SecurityErase                      0x00004161
#define func_FLASH_SecurityLock                       0x000041d9
#define func_FLASH_GetUniqueID                        0x000042bb
#define func_I2CMasterEnable                          0x000042d1
#define func_I2CMasterInitExpClk                      0x000042db
#define func_I2CSlaveEnable                           0x0000434d
#define func_I2CSlaveInit                             0x00004357
#define func_I2CIntRegister                           0x00004367
#define func_I2CIntUnregister                         0x00004393
#define func_I2CIntEnable                             0x000043b7
#define func_I2CIntDisable                            0x000043bf
#define func_I2CIntStatus                             0x000043c7
#define func_I2CIntClear                              0x000043d7
#define func_I2CAddrSet                               0x000043df
#define func_I2CBusBusy                               0x00004415
#define func_I2CControlSet                            0x00004427
#define func_I2CControlGet                            0x0000442b
#define func_I2CDataPut                               0x00004431
#define func_I2CDataGet                               0x00004435
#define func_I2CStatus                                0x0000443b
#define func_I2CACKModeSet                            0x00004441
#define func_I2CFifoClear                             0x00004457
#define func_UTCTimerStop                             0x00004469
#define func_UTCTimerRun                              0x00004477
#define func_UTCCalStop                               0x00004485
#define func_UTCCalRun                                0x00004493
#define func_UTCDivStop                               0x000044a1
#define func_UTCDivEn                                 0x000044af
#define func_UTCHourModeSet                           0x000044bd
#define func_UTCHourModeGet                           0x000044c3
#define func_UTCTimerSet                              0x000044c9
#define func_UTCTimerChangeGet                        0x00004567
#define func_UTCTimerGet                              0x0000456f
#define func_UTCTimerSetBy10ms                        0x000045c9
#define func_UTCTimerGetBy10ms                        0x000046f3
#define func_UTCTimerAlarmSet                         0x0000476f
#define func_UTCTimerAlarmSetBy10ms                   0x00004801
#define func_UTCCalSet                                0x00004915
#define func_UTCCalChangeGet                          0x000049ad
#define func_UTCCalGet                                0x000049b5
#define func_UTCCalAlarmSet                           0x00004a0f
#define func_UTCAlarmEnable                           0x00004a47
#define func_UTCAlarmDisable                          0x00004a53
#define func_UTCIntEnable                             0x00004a5f
#define func_UTCIntDisable                            0x00004a6b
#define func_UTCIntMaskSet                            0x00004a77
#define func_UTCIntMaskGet                            0x00004a83
#define func_UTCIntStatusSet                          0x00004a89
#define func_UTCIntStatusGet                          0x00004a95
#define func_UTCValidStatusGet                        0x00004a9b
#define func_UTCKeepRun                               0x00004aa1
#define func_UTCClkCntSet                             0x00004aa7
#define func_UTCClkCntGet                             0x00004abb
#define func_UTCIntRegister                           0x00004add
#define func_UTCIntUnregister                         0x00004af5
#define func_GPIOPeripheralPad                        0x00004b0d
#define func_GPIODirModeSet                           0x00004b19
#define func_GPIOOutputODEn                           0x00004bef
#define func_GPIOOutputODDis                          0x00004c1d
#define func_GPIOAnalogEn                             0x00004c4b
#define func_GPIOAnalogDis                            0x00004c7d
#define func_GPIOPullupEn                             0x00004caf
#define func_GPIOPullupDis                            0x00004cdd
#define func_GPIOPulldownEn                           0x00004d0b
#define func_GPIOPulldownDis                          0x00004d39
#define func_GPIOPinSet                               0x00004d67
#define func_GPIOPinClear                             0x00004d95
#define func_GPIOPinRead                              0x00004dc3
#define func_GPIOIntEnable                            0x00004df3
#define func_GPIOIntDisable                           0x00004e21
#define func_GPIOIntTypeSet                           0x00004e4f
#define func_GPIOIntEdgeSet                           0x00004e93
#define func_GPIOIntSingleEdgeSet                     0x00004ed7
#define func_GPIOIntStatusGet                         0x00004f21
#define func_GPIOIntMaskGet                           0x00004f33
#define func_GPIOConflictStatusGet                    0x00004f65
#define func_GPIOConflictStatusClear                  0x00004f97
#define func_GPIOAllocationStatusGet                  0x00004fc5
#define func_GPIODrvStrengthSet                       0x00004ff9
#define func_GPIOIntRegister                          0x0000502f
#define func_GPIOIntUnregister                        0x00005047
#define func_GPIOModeSet                              0x0000505b
#define func_GPIOPadConfigSel                         0x0000509d
#define func_GPIOPadModeSet                           0x000050df
#define func_GPIODirectionSet                         0x00005159
#define func_AON_DSP_Load_Flag_Get                    0x000051f9
#define func_AON_ARM_Load_Flag_Get                    0x00005217
#define func_AON_SECONDARY_Load_Flag_Get              0x00005235
#define func_AON_DSP_Load_Flag_Set                    0x00005253
#define func_AON_ARM_Load_Flag_Set                    0x00005273
#define func_AON_SECONDARY_Load_Flag_Set              0x00005293
#define func_AON_Flash_Delay_Get                      0x000052b3
#define func_AON_Flash_Delay_Set                      0x00005351
#define func_AON_Addr_Overlap_Unused_Mem              0x00005367
#define func_PRCM_Clock_Enable                        0x00005445
#define func_PRCM_Clock_Disable                       0x0000544d
#define func_PRCM_CHIP_VER_GET                        0x00005455
#define func_AON_BOOTMODE_GET                         0x0000545b
#define func_CSPRxFrameSet                            0x0000547d
#define func_CSPTxFrameSet                            0x000054a3
#define func_CSPClockSet                              0x000054d3
#define func_CSPEndianModeSet                         0x000055ef
#define func_CSPOperationModeSet                      0x000055fb
#define func_CSPUARTModeSet                           0x00005607
#define func_CSPIrdaEnable                            0x0000568d
#define func_CSPIrdaPulseWidthSet                     0x00005699
#define func_CSPIrdaIdleLevelSet                      0x000056a5
#define func_CSPIrdaMode                              0x000056b1
#define func_CSPSPIMode                               0x00005753
#define func_CSPFIFOLevelSet                          0x0000579b
#define func_CSPCharGet                               0x000057a5
#define func_CSPCharPut                               0x000057bd
#define func_CSPCharGetNonBlocking                    0x000057d1
#define func_CSPCharPutNonBlocking                    0x000057e9
#define func_CSPClockModeSet                          0x00005801
#define func_CSPEnable                                0x0000580d
#define func_CSPDisable                               0x00005817
#define func_CSPDrivenEdgeSet                         0x00005821
#define func_CSPSyncValidLevelSet                     0x00005833
#define func_CSPClockIdleModeSet                      0x00005845
#define func_CSPClockIdleLevelSet                     0x00005851
#define func_CSPPinModeSet                            0x0000585d
#define func_CSPRxEnable                              0x0000589b
#define func_CSPRxDisable                             0x000058a5
#define func_CSPTxEnable                              0x000058af
#define func_CSPTxDisable                             0x000058b9
#define func_CSPIntRegister                           0x000058c3
#define func_CSPIntUnregister                         0x000058dd
#define func_CSPIntEnable                             0x000058f3
#define func_CSPIntDisable                            0x000058fb
#define func_CSPIntStatus                             0x00005903
#define func_CSPIntClear                              0x00005909
#define func_AESKeyLenSet                             0x00005915
#define func_AESKeySet                                0x00005925
#define func_AESIVSet                                 0x0000592d
#define func_AESAADLen0Set                            0x00005935
#define func_AESAADLen1Set                            0x0000593b
#define func_AESPayloadLenSet                         0x00005941
#define func_AESModeSet                               0x00005947
#define func_AESEncDecSet                             0x00005957
#define func_AESDMAEn                                 0x00005967
#define func_AESDMADis                                0x00005975
#define func_AESBlockDataInput                        0x00005983
#define func_AESBlockDataOutput                       0x0000598b
#define func_AESBlockStart                            0x00005993
#define func_AESKeyLoadWaitDone                       0x000059a1
#define func_AESBlockTransWaitDone                    0x000059bd
#define func_AESClockDiv2En                           0x000059cd
#define func_AESClockDiv2Dis                          0x000059db
#define func_AESCCMAuthLenSet                         0x000059e9
#define func_AESCCMLengthLenSet                       0x000059fb
#define func_AESTagGet                                0x00005a0d
#define func_AESBlock                                 0x00005a17
#define func_AESECB                                   0x00005baf
#define func_AESCCM                                   0x00005d61
#define func_AES_128_EEA2                             0x00006037
#define func_AES_CMAC_XOR_128                         0x0000642d
#define func_AES_CMAC_Leftshift_Onebit                0x00006447
#define func_AES_CMAC_Generate_Subkey                 0x0000646d
#define func_AES_128_EIA2                             0x000064eb
#define func_SEMAMasterEnable                         0x00006855
#define func_SEMAMasterDisable                        0x0000686d
#define func_SEMARequest                              0x00006885
#define func_SEMARequestNonBlocking                   0x000068ab
#define func_SEMARequestPriSet                        0x000068c3
#define func_SEMAMasterGet                            0x000068d3
#define func_SEMASlaveGet                             0x000068e5
#define func_SEMAReset                                0x000068f3
#define func_SEMAReqQueueState                        0x00006907
#define func_SEMAMasterIntEnable                      0x00006913
#define func_SEMARelease                              0x00006925
#define func_SEMAIntRegister                          0x0000693d
#define func_SEMAIntUnregister                        0x00006955
#define func_MCNTStart                                0x0000696d
#define func_MCNTStop                                 0x00006975
#define func_MCNTSetCNT32K                            0x0000697d
#define func_MCNTGetCNT32K                            0x00006983
#define func_MCNTGetMCNT                              0x00006989
#define func_MCNTGetIntStatus                         0x0000698f
#define func_MCNTSetIntStatus                         0x00006999
#define func_MCNTIntRegister                          0x0000699f
#define func_MCNTIntUnregister                        0x000069b7
#define func_REG_Bus_Field_Set                        0x000069d1
#define func_REG_Bus_Field_Get                        0x00006b1b
#define func_PRCM_Clock_Mode_Force_XTAL               0x00006c3f
#define func_PRCM_Clock_Mode_Auto                     0x00006c5b


#define ROM_AESKeyLenSet                             ((void (*)(unsigned long ulKeyLenMode))func_AESKeyLenSet)
#define ROM_AESKeySet                                ((void (*)(unsigned char ucOffset, unsigned long ulKeyValue))func_AESKeySet)
#define ROM_AESIVSet                                 ((void (*)(unsigned char ucOffset, unsigned long ucIV))func_AESIVSet)
#define ROM_AESAADLen0Set                            ((void (*)(unsigned long ulAadLen0))func_AESAADLen0Set)
#define ROM_AESAADLen1Set                            ((void (*)(unsigned long ulAadLen1))func_AESAADLen1Set)
#define ROM_AESPayloadLenSet                         ((void (*)(unsigned long ulPayloadLenByte))func_AESPayloadLenSet)
#define ROM_AESModeSet                               ((void (*)(unsigned long ucAESMode))func_AESModeSet)
#define ROM_AESEncDecSet                             ((void (*)(unsigned long ucAESEncDec))func_AESEncDecSet)
#define ROM_AESDMAEn                                 ((void (*)(void))func_AESDMAEn)
#define ROM_AESDMADis                                ((void (*)(void))func_AESDMADis)
#define ROM_AESBlockDataInput                        ((void (*)(unsigned char ucOffset, unsigned long ulDataIn))func_AESBlockDataInput)
#define ROM_AESBlockDataOutput                       ((unsigned long (*)(unsigned char ucOffset))func_AESBlockDataOutput)
#define ROM_AESBlockStart                            ((void (*)(void))func_AESBlockStart)
#define ROM_AESKeyLoadWaitDone                       ((void (*)(void))func_AESKeyLoadWaitDone)
#define ROM_AESBlockTransWaitDone                    ((void (*)(void))func_AESBlockTransWaitDone)
#define ROM_AESClockDiv2En                           ((void (*)(void))func_AESClockDiv2En)
#define ROM_AESClockDiv2Dis                          ((void (*)(void))func_AESClockDiv2Dis)
#define ROM_AESCCMAuthLenSet                         ((void (*)(unsigned long ucAuthLenByte))func_AESCCMAuthLenSet)
#define ROM_AESCCMLengthLenSet                       ((void (*)(unsigned char ucLengthLenByte))func_AESCCMLengthLenSet)
#define ROM_AESTagGet                                ((unsigned long (*)(unsigned char ucOffset))func_AESTagGet)
#define ROM_AESBlock                                 ((void (*)(unsigned char *pucInput, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput))func_AESBlock)
#define ROM_AESECB                                   ((void (*)(unsigned char *pucInput, unsigned long ulInputLenByte, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput))func_AESECB)
#define ROM_AESCCM                                   ((void (*)(unsigned char *aad, unsigned long aad_len_byte, unsigned char *input_message, unsigned long input_len_byte, unsigned long *user_key, unsigned long key_len_bits, unsigned long M, unsigned long L, unsigned char *nonce, unsigned char mode, unsigned char *output_message, unsigned char *digest))func_AESCCM)
#define ROM_AES_128_EEA2                             ((void (*)(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucOutMsg))func_AES_128_EEA2)
#define ROM_AES_CMAC_XOR_128                         ((void (*)(unsigned char *pucA, const unsigned char *pucB, unsigned char *pucOut))func_AES_CMAC_XOR_128)
#define ROM_AES_CMAC_Leftshift_Onebit                ((void (*)(unsigned char *pucInput, unsigned char *pucOutput))func_AES_CMAC_Leftshift_Onebit)
#define ROM_AES_CMAC_Generate_Subkey                 ((void (*)(unsigned char *pucKey, unsigned char *pucK1, unsigned char *pucK2))func_AES_CMAC_Generate_Subkey)
#define ROM_AES_128_EIA2                             ((void (*)(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucMACT, unsigned char *ucM))func_AES_128_EIA2)
#define	ROM_AON_DSP_Load_Flag_Get                    ((unsigned char (*)(void))func_AON_DSP_Load_Flag_Get)
#define	ROM_AON_ARM_Load_Flag_Get                    ((unsigned char (*)(void))func_AON_ARM_Load_Flag_Get)
#define	ROM_AON_DSP_Load_Flag_Set                    ((void (*)(uint8_t flag))func_AON_DSP_Load_Flag_Set)
#define	ROM_AON_ARM_Load_Flag_Set                    ((void (*)(uint8_t flag))func_AON_ARM_Load_Flag_Set)
#define	ROM_AON_Flash_Delay_Get                      ((void (*)(void))func_AON_Flash_Delay_Get)
#define	ROM_AON_Flash_Delay_Set                      ((void (*)(uint8_t value))func_AON_Flash_Delay_Set)
#define	ROM_AON_Addr_Overlap_Unused_Mem              ((unsigned char (*)(unsigned char mem_status, unsigned long ulAddr, unsigned long ulSizebyte))func_AON_Addr_Overlap_Unused_Mem)
#define ROM_CSPRxFrameSet                            ((void (*)(unsigned long ulBase, unsigned long ulDelay, unsigned long ulDataLen, unsigned long ulFrameLen))func_CSPRxFrameSet)
#define ROM_CSPTxFrameSet                            ((void (*)(unsigned long ulBase, unsigned long ulDelay, unsigned long ulDataLen, unsigned long ulSyncLen, unsigned long ulFrameLen))func_CSPTxFrameSet)
#define ROM_CSPClockSet                              ((void (*)(unsigned long ulBase, unsigned long ulCSPMode, unsigned long ulPclk, unsigned ulClock))func_CSPClockSet)
#define ROM_CSPEndianModeSet                         ((void (*)(unsigned long ulBase, unsigned long ulEndianMode))func_CSPEndianModeSet)
#define ROM_CSPOperationModeSet                      ((void (*)(unsigned long ulBase, unsigned long ulOperationMode))func_CSPOperationModeSet)
#define ROM_CSPUARTModeSet                           ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned char ucDatabits, unsigned char ucStopbits))func_CSPUARTModeSet)
#define ROM_CSPIrdaEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIrdaEn))func_CSPIrdaEnable)
#define ROM_CSPIrdaPulseWidthSet                     ((void (*)(unsigned long ulBase, unsigned long ulWidth))func_CSPIrdaPulseWidthSet)
#define ROM_CSPIrdaIdleLevelSet                      ((void (*)(unsigned long ulBase, unsigned long ulIrdaIdleLevel))func_CSPIrdaIdleLevelSet)
#define ROM_CSPIrdaMode                              ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned char ucDatabits, unsigned char ucStopbits, unsigned long ulIdleLevel, unsigned long ulWidth))func_CSPIrdaMode)
#define ROM_CSPSPIMode                               ((void (*)(unsigned long ulBase, unsigned long ulMode1, unsigned long ulMode2, unsigned long ulTxFrameCtl, unsigned long ulRxFrameCtl))func_CSPSPIMode)
#define ROM_CSPFIFOLevelSet                          ((void (*)(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel))func_CSPFIFOLevelSet)
#define ROM_CSPCharGet                               ((unsigned char (*)(unsigned long ulBase))func_CSPCharGet)
#define ROM_CSPCharPut                               ((void (*)(unsigned long ulBase, unsigned char ucData))func_CSPCharPut)
#define ROM_CSPCharGetNonBlocking                    ((long (*)(unsigned long ulBase))func_CSPCharGetNonBlocking)
#define ROM_CSPCharPutNonBlocking                    ((long (*)(unsigned long ulBase, unsigned char ucData))func_CSPCharPutNonBlocking)
#define ROM_CSPClockModeSet                          ((void (*)(unsigned long ulBase, unsigned long ulClockMode))func_CSPClockModeSet)
#define ROM_CSPEnable                                ((void (*)(unsigned long ulBase))func_CSPEnable)
#define ROM_CSPDisable                               ((void (*)(unsigned long ulBase))func_CSPDisable)
#define ROM_CSPDrivenEdgeSet                         ((void (*)(unsigned long ulBase, unsigned long ulRxEdge, unsigned long ulTxEdge))func_CSPDrivenEdgeSet)
#define ROM_CSPSyncValidLevelSet                     ((void (*)(unsigned long ulBase, unsigned long ulRFS, unsigned long ulTFS))func_CSPSyncValidLevelSet)
#define ROM_CSPClockIdleModeSet                      ((void (*)(unsigned long ulBase, unsigned long ulClkIdleMode))func_CSPClockIdleModeSet)
#define ROM_CSPClockIdleLevelSet                     ((void (*)(unsigned long ulBase, unsigned long ulClkIdleLevel))func_CSPClockIdleLevelSet)
#define ROM_CSPPinModeSet                            ((void (*)(unsigned long ulBase, unsigned long ulCSPPin, unsigned long ulPinMode, unsigned long ulPinDirection))func_CSPPinModeSet)
#define ROM_CSPRxEnable                              ((void (*)(unsigned long ulBase))func_CSPRxEnable)
#define ROM_CSPRxDisable                             ((void (*)(unsigned long ulBase))func_CSPRxDisable)
#define ROM_CSPTxEnable                              ((void (*)(unsigned long ulBase))func_CSPTxEnable)
#define ROM_CSPTxDisable                             ((void (*)(unsigned long ulBase))func_CSPTxDisable)
#define ROM_CSPIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_CSPIntRegister)
#define ROM_CSPIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_CSPIntUnregister)
#define ROM_CSPIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntEnable)
#define ROM_CSPIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntDisable)
#define ROM_CSPIntStatus                             ((unsigned long (*)(unsigned long ulBase))func_CSPIntStatus)
#define ROM_CSPIntClear                              ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntClear)
#define ROM_DMAErrorStatusGet                        ((unsigned long (*)(unsigned long ulChannelNum))func_DMAErrorStatusGet)
#define ROM_DMAErrorStatusClear                      ((void (*)(unsigned long ulChannelNum))func_DMAErrorStatusClear)
#define ROM_DMAChannelRequest                        ((void (*)(unsigned long ulChannelNum))func_DMAChannelRequest)
#define ROM_DMAChannelControlSet                     ((void (*)(unsigned long ulChannelNum, unsigned long ulControl))func_DMAChannelControlSet)
#define ROM_DMAChannelTransferSet                    ((void (*)(unsigned long ulChannelNum, void *pvSrcAddr, void *pvDstAddr, unsigned long ulTransferSize))func_DMAChannelTransferSet)
#define ROM_DMAChannelNextPointerSet                 ((void (*)(unsigned long ulChannelNum, void *pvNextPointer))func_DMAChannelNextPointerSet)
#define ROM_DMAIntRegister                           ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_DMAIntRegister)
#define ROM_DMAIntUnregister                         ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors))func_DMAIntUnregister)
#define ROM_DMAIntStatus                             ((unsigned long (*)(unsigned long ulChannelNum))func_DMAIntStatus)
#define ROM_DMAIntClear                              ((void (*)(unsigned long ulChanMask))func_DMAIntClear)
#define ROM_DMAChannelWaitIdle                       ((void (*)(unsigned long ulChannelNum))func_DMAChannelWaitIdle)
#define ROM_DMAChannelTransfer                       ((void (*)(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum))func_DMAChannelTransfer)
#define ROM_DMAWaitAESDone                           ((void (*)(void))func_DMAWaitAESDone)
#define ROM_DMAChannelMemcpy                         ((void (*)(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum))func_DMAChannelMemcpy)
#define ROM_DMAWaitChannelDone                       ((void (*)(unsigned char dmaChannels))func_DMAWaitChannelDone)
#define ROM_GetDMAChannelTransCnt                    ((unsigned long (*)(unsigned long ulChannelNum))func_GetDMAChannelTransCnt)
#define ROM_FLASH_Init                               ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int ref_clk_hz, unsigned int sclk_hz))func_FLASH_Init)
#define ROM_FLASH_ChipErase                          ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_ChipErase)
#define ROM_FLASH_SectorErase                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_SectorErase)
#define ROM_FLASH_BlockErase32K                      ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_BlockErase32K)
#define ROM_FLASH_BlockErase64K                      ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_BlockErase64K)
#define ROM_FLASH_WriteData                          ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))func_FLASH_WriteData)
#define ROM_FLASH_ReadData                           ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))func_FLASH_ReadData)
#define ROM_FLASH_WaitIdle                           ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_WaitIdle)
#define ROM_FLASH_UpdateData                         ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum, unsigned long ulBackupFlag, unsigned ulExchangeAddr))func_FLASH_UpdateData)
#define ROM_FLASH_EnableQEFlag                       ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_EnableQEFlag)
#define ROM_FLASH_SetReadMode                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int mode))func_FLASH_SetReadMode)
#define ROM_FLASH_EnterXIPMode                       ((void (*)(QSPI_FLASH_Def *flash_vendor, char xip_dummy))func_FLASH_EnterXIPMode)
#define ROM_FLASH_ExitXIPMode                        ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_ExitXIPMode)
#define ROM_FLASH_SetWriteMode                       ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int mode))func_FLASH_SetWriteMode)
#define ROM_FLASH_GetDeviceID                        ((unsigned int (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_GetDeviceID)
#define ROM_FLASH_SecurityErase                      ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_SecurityErase)
#define ROM_FLASH_SecurityLock                       ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_SecurityLock)
#define ROM_FLASH_FAST_WriteData                     ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))func_FLASH_FAST_WriteData)
#define ROM_FLASH_GetUniqueID                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned char *pucUniqueId))func_FLASH_GetUniqueID)
#define ROM_GPIOPeripheralPad                        ((void (*)(unsigned char ucPeriNum, unsigned char ucPadNum))func_GPIOPeripheralPad)
#define ROM_GPIODirModeSet                           ((void (*)(unsigned char ucPins, unsigned long ulPinIO))func_GPIODirModeSet)
#define ROM_GPIOOutputODEn                           ((void (*)(unsigned char ucPins))func_GPIOOutputODEn)
#define ROM_GPIOOutputODDis                          ((void (*)(unsigned char ucPins))func_GPIOOutputODDis)
#define ROM_GPIOAnalogEn                             ((void (*)(unsigned char ucPins))func_GPIOAnalogEn)
#define ROM_GPIOAnalogDis                            ((void (*)(unsigned char ucPins))func_GPIOAnalogDis)
#define ROM_GPIOPullupEn                             ((void (*)(unsigned char ucPins))func_GPIOPullupEn)
#define ROM_GPIOPullupDis                            ((void (*)(unsigned char ucPins))func_GPIOPullupDis)
#define ROM_GPIOPulldownEn                           ((void (*)(unsigned char ucPins))func_GPIOPulldownEn)
#define ROM_GPIOPulldownDis                          ((void (*)(unsigned char ucPins))func_GPIOPulldownDis)
#define ROM_GPIOPinSet                               ((void (*)(unsigned char ucPins))func_GPIOPinSet)
#define ROM_GPIOPinClear                             ((void (*)(unsigned char ucPins))func_GPIOPinClear)
#define ROM_GPIOPinRead                              ((unsigned char (*)(unsigned char ucPins))func_GPIOPinRead)
#define ROM_GPIOIntEnable                            ((void (*)(unsigned char ucPins))func_GPIOIntEnable)
#define ROM_GPIOIntDisable                           ((void (*)(unsigned char ucPins))func_GPIOIntDisable)
#define ROM_GPIOIntTypeSet                           ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntTypeSet)
#define ROM_GPIOIntEdgeSet                           ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntEdgeSet)
#define ROM_GPIOIntSingleEdgeSet                     ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntSingleEdgeSet)
#define ROM_GPIOIntStatusGet                         ((unsigned long (*)(unsigned char ucPins))func_GPIOIntStatusGet)
#define ROM_GPIOIntMaskGet                           ((unsigned long (*)(unsigned char ucPins))func_GPIOIntMaskGet)
#define ROM_GPIOConflictStatusGet                    ((unsigned long (*)(unsigned char ucPins))func_GPIOConflictStatusGet)
#define ROM_GPIOConflictStatusClear                  ((void (*)(unsigned char ucPins))func_GPIOConflictStatusClear)
#define ROM_GPIOAllocationStatusGet                  ((unsigned long (*)(unsigned char ucPins))func_GPIOAllocationStatusGet)
#define ROM_GPIODrvStrengthSet                       ((void (*)(unsigned char ucPins, unsigned char ucDrvStrength))func_GPIODrvStrengthSet)
#define ROM_GPIOIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_GPIOIntRegister)
#define ROM_GPIOIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))func_GPIOIntUnregister)
#define	ROM_GPIOModeSet                              ((void (*)(unsigned char ucPins, unsigned char ucMode))func_GPIOModeSet)
#define	ROM_GPIOPadConfigSel                         ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOPadConfigSel)
#define	ROM_GPIOPadModeSet                           ((void (*)(unsigned char ucPins, unsigned char ucMode, unsigned char ucConfig))func_GPIOPadModeSet)
#define	ROM_GPIODirectionSet                         ((void (*)(unsigned char ucPins, unsigned long ulPinIO))func_GPIODirectionSet)
#define ROM_I2CMasterEnable                          ((void (*)(unsigned long ulBase))func_I2CMasterEnable)
#define ROM_I2CMasterInitExpClk                      ((void (*)(unsigned long ulBase, unsigned long ulI2CClk, unsigned char bFast))func_I2CMasterInitExpClk)
#define ROM_I2CSlaveEnable                           ((void (*)(unsigned long ulBase))func_I2CSlaveEnable)
#define ROM_I2CSlaveInit                             ((void (*)(unsigned long ulBase, unsigned long ucSlaveAddr))func_I2CSlaveInit)
#define ROM_I2CIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(fnHandler)(void)))func_I2CIntRegister)
#define ROM_I2CIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_I2CIntUnregister)
#define ROM_I2CIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_I2CIntEnable)
#define ROM_I2CIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_I2CIntDisable)
#define ROM_I2CIntStatus                             ((unsigned char (*)(unsigned long ulBase, unsigned char bMasked))func_I2CIntStatus)
#define ROM_I2CIntClear                              ((void (*)(unsigned long ulBase))func_I2CIntClear)
#define ROM_I2CAddrSet                               ((void (*)(unsigned long ulBase, unsigned long ulAddr, unsigned char ucNorMode, unsigned char ucReceive))func_I2CAddrSet)
#define ROM_I2CBusBusy                               ((unsigned char (*)(unsigned long ulBase))func_I2CBusBusy)
#define ROM_I2CControlSet                            ((void (*)(unsigned long ulBase, unsigned long ulCmd))func_I2CControlSet)
#define ROM_I2CControlGet                            ((unsigned long (*)(unsigned long ulBase))func_I2CControlGet)
#define ROM_I2CDataPut                               ((void (*)(unsigned long ulBase, unsigned char ucData))func_I2CDataPut)
#define ROM_I2CDataGet                               ((unsigned long (*)(unsigned long ulBase))func_I2CDataGet)
#define ROM_I2CStatus                                ((unsigned long (*)(unsigned long ulBase))func_I2CStatus)
#define ROM_I2CACKModeSet                            ((void (*)(unsigned long ulBase, unsigned char ucEnable))func_I2CACKModeSet)
#define ROM_I2CFifoClear                             ((void (*)(unsigned long ulBase))func_I2CFifoClear)
#define ROM_IntRegister                              ((void (*)(unsigned long ulInterrupt, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_IntRegister)
#define ROM_IntUnregister                            ((void (*)(unsigned long ulInterrupt, unsigned long *g_pRAMVectors))func_IntUnregister)
#define ROM_IntPriorityGroupingSet                   ((void (*)(unsigned long ulBits))func_IntPriorityGroupingSet)
#define ROM_IntPriorityGroupingGet                   ((unsigned long (*)(void))func_IntPriorityGroupingGet)
#define ROM_IntPrioritySet                           ((void (*)(unsigned long ulInterrupt,unsigned char ucPriority))func_IntPrioritySet)
#define ROM_IntPriorityGet                           ((long (*)(unsigned long ulInterrupt))func_IntPriorityGet)
#define ROM_IntEnable                                ((void (*)(unsigned long ulInterrupt))func_IntEnable)
#define ROM_IntDisable                               ((void (*)(unsigned long ulInterrupt))func_IntDisable)
#define ROM_IntPendSet                               ((void (*)(unsigned long ulInterrupt))func_IntPendSet)
#define ROM_IntPendClear                             ((void (*)(unsigned long ulInterrupt))func_IntPendClear)
#define	ROM_MCNTStart                                ((void (*)(void))func_MCNTStart)
#define	ROM_MCNTStop                                 ((void (*)(void))func_MCNTStop)
#define	ROM_MCNTSetCNT32K                            ((void (*)(unsigned long ulCounter))func_MCNTSetCNT32K)
#define	ROM_MCNTGetCNT32K                            ((unsigned long (*)(void))func_MCNTGetCNT32K)
#define	ROM_MCNTGetMCNT                              ((unsigned long (*)(void))func_MCNTGetMCNT)
#define	ROM_MCNTGetIntStatus                         ((unsigned char (*)(void))func_MCNTGetIntStatus)
#define	ROM_MCNTSetIntStatus                         ((void (*)(unsigned char ucIntstatus))func_MCNTSetIntStatus)
#define	ROM_MCNTIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_MCNTIntRegister)
#define	ROM_MCNTIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))func_MCNTIntUnregister)
#define ROM_PRCM_Clock_Enable                        ((void (*)(unsigned long ulBase, unsigned long ulModule))func_PRCM_Clock_Enable)
#define ROM_PRCM_Clock_Disable                       ((void (*)(unsigned long ulBase, unsigned long ulModule))func_PRCM_Clock_Disable)
#define ROM_PRCM_CHIP_VER_GET                        ((unsigned long (*)(unsigned long ulBase))func_PRCM_CHIP_VER_GET)
#define ROM_AON_BOOTMODE_GET                         ((unsigned char (*)(void))func_AON_BOOTMODE_GET)
#define ROM_PRCM_Clock_Mode_Force_XTAL               ((void (*)(void))func_PRCM_Clock_Mode_Force_XTAL)
#define ROM_PRCM_Clock_Mode_Auto                     ((void (*)(void))func_PRCM_Clock_Mode_Auto)
#define ROM_REG_Bus_Field_Set                        ((unsigned char (*)(unsigned long ulRegBase, uint8_t ucBit_high, uint8_t ucBit_low, unsigned long ulValue))func_REG_Bus_Field_Set)
#define ROM_REG_Bus_Field_Get                        ((unsigned char (*)(unsigned long ulRegBase, uint8_t ucBit_high, uint8_t ucBit_low, unsigned long *ulValue))func_REG_Bus_Field_Get)
#define ROM_SEMAMasterEnable                         ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave))func_SEMAMasterEnable)
#define ROM_SEMAMasterDisable                        ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave))func_SEMAMasterDisable)
#define ROM_SEMARequest                              ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARequest)
#define ROM_SEMARequestNonBlocking                   ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARequestNonBlocking)
#define ROM_SEMARequestPriSet                        ((void (*)(unsigned char ucSemaSlave, unsigned long ucPriority))func_SEMARequestPriSet)
#define ROM_SEMAMasterGet                            ((unsigned char (*)(unsigned char ucSemaSlave))func_SEMAMasterGet)
#define ROM_SEMASlaveGet                             ((unsigned long (*)(unsigned char ucSemaMaster))func_SEMASlaveGet)
#define ROM_SEMAReset                                ((void (*)(void))func_SEMAReset)
#define ROM_SEMAReqQueueState                        ((unsigned char (*)(void))func_SEMAReqQueueState)
#define ROM_SEMAMasterIntEnable                      ((void (*)(unsigned long ulSemaMasterInt))func_SEMAMasterIntEnable)
#define ROM_SEMARelease                              ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARelease)
#define ROM_SEMAIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_SEMAIntRegister)
#define ROM_SEMAIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))func_SEMAIntUnregister)
#define ROM_SPIConfigSetExpClk                       ((void (*)(unsigned long ulBase, unsigned long ulDivide, unsigned long ulProtocol, unsigned long ulMode,unsigned long ulDataWidth))func_SPIConfigSetExpClk)
#define ROM_SPIEnable                                ((void (*)(unsigned long ulBase))func_SPIEnable)
#define ROM_SPIDisable                               ((void (*)(unsigned long ulBase))func_SPIDisable)
#define ROM_SPIIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_SPIIntRegister)
#define ROM_SPIIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_SPIIntUnregister)
#define ROM_SPIIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntEnable)
#define ROM_SPIIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntDisable)
#define ROM_SPIIntStatus                             ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))func_SPIIntStatus)
#define ROM_SPIIntClear                              ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntClear)
#define ROM_SPIWordDataPut                           ((void (*)(unsigned long ulBase, unsigned long ulData))func_SPIWordDataPut)
#define ROM_SPIHalfWordDataPut                       ((void (*)(unsigned long ulBase, unsigned short ulData))func_SPIHalfWordDataPut)
#define ROM_SPIByteDataPut                           ((void (*)(unsigned long ulBase, unsigned char ulData))func_SPIByteDataPut)
#define ROM_SPIDataPutNonBlocking                    ((long (*)(unsigned long ulBase, unsigned long ulData))func_SPIDataPutNonBlocking)
#define ROM_SPIWordDataGet                           ((void (*)(unsigned long ulBase, unsigned long *pulData))func_SPIWordDataGet)
#define ROM_SPIHalfWordDataGet                       ((void (*)(unsigned long ulBase, unsigned short *pulData))func_SPIHalfWordDataGet)
#define ROM_SPIByteDataGet                           ((void (*)(unsigned long ulBase, unsigned char *pulData))func_SPIByteDataGet)
#define ROM_SPIDataGetNonBlocking                    ((long (*)(unsigned long ulBase, unsigned long *pulData))func_SPIDataGetNonBlocking)
#define ROM_SPISetTxFifoThreshold                    ((void (*)(unsigned long ulBase, unsigned long ulThreshold))func_SPISetTxFifoThreshold)
#define ROM_SPISetRxFifoThreshold                    ((void (*)(unsigned long ulBase, unsigned long ulThreshold))func_SPISetRxFifoThreshold)
#define ROM_SPISetDelay                              ((void (*)(unsigned long ulBase, unsigned long ulDelay))func_SPISetDelay)
#define ROM_SPIEnManualCS                            ((void (*)(unsigned long ulBase))func_SPIEnManualCS)
#define ROM_SPIDisManualCS                           ((void (*)(unsigned long ulBase))func_SPIDisManualCS)
#define ROM_SPIEnManualTransmit                      ((void (*)(unsigned long ulBase))func_SPIEnManualTransmit)
#define ROM_SPIDisManualTransmit                     ((void (*)(unsigned long ulBase))func_SPIDisManualTransmit)
#define ROM_SPIStartManualTransmit                   ((void (*)(unsigned long ulBase))func_SPIStartManualTransmit)
#define ROM_SPIEnModeFail                            ((void (*)(unsigned long ulBase))func_SPIEnModeFail)
#define ROM_SPIDisModeFail                           ((void (*)(unsigned long ulBase))func_SPIDisModeFail)
#define ROM_SPITxFifoFlush                           ((void (*)(unsigned long ulBase))func_SPITxFifoFlush)
#define ROM_SPITxFifoNormal                          ((void (*)(unsigned long ulBase))func_SPITxFifoNormal)
#define ROM_SPIRxFifoFlush                           ((void (*)(unsigned long ulBase))func_SPIRxFifoFlush)
#define ROM_SPIRxFifoNormal                          ((void (*)(unsigned long ulBase))func_SPIRxFifoNormal)
#define ROM_SysTickEnable                            ((void (*)(void))func_SysTickEnable)
#define ROM_SysTickDisable                           ((void (*)(void))func_SysTickDisable)
#define ROM_SysTickIntRegister                       ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_SysTickIntRegister)
#define ROM_SysTickIntUnregister                     ((void (*)(unsigned long *g_pRAMVectors))func_SysTickIntUnregister)
#define ROM_SysTickIntEnable                         ((void (*)(void))func_SysTickIntEnable)
#define ROM_SysTickIntDisable                        ((void (*)(void))func_SysTickIntDisable)
#define ROM_SysTickPeriodSet                         ((void (*)(unsigned long ulPeriod))func_SysTickPeriodSet)
#define ROM_SysTickPeriodGet                         ((unsigned long (*)(void))func_SysTickPeriodGet)
#define ROM_SysTickValueGet                          ((unsigned long (*)(void))func_SysTickValueGet)
#define ROM_TimerEnable                              ((void (*)(unsigned long ulBase))func_TimerEnable)
#define ROM_TimerDisable                             ((void (*)(unsigned long ulBase))func_TimerDisable)
#define ROM_TimerConfigure                           ((void (*)(unsigned long ulBase, unsigned long ulConfig))func_TimerConfigure)
#define ROM_TimerControlLevel                        ((void (*)(unsigned long ulBase, unsigned char bInvert))func_TimerControlLevel)
#define ROM_TimerPrescaleSet                         ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerPrescaleSet)
#define ROM_TimerPrescaleGet                         ((unsigned long (*)(unsigned long ulBase))func_TimerPrescaleGet)
#define ROM_TimerLoadSet                             ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerLoadSet)
#define ROM_TimerValueGet                            ((unsigned long (*)(unsigned long ulBase))func_TimerValueGet)
#define ROM_TimerMatchSet                            ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerMatchSet)
#define ROM_TimerMatchGet                            ((unsigned long (*)(unsigned long ulBase))func_TimerMatchGet)
#define ROM_TimerPWMSet                              ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerPWMSet)
#define ROM_TimerPWMGet                              ((unsigned long (*)(unsigned long ulBase))func_TimerPWMGet)
#define ROM_TimerIntRegister                         ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_TimerIntRegister)
#define ROM_TimerIntUnregister                       ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors))func_TimerIntUnregister)
#define ROM_TimerIntEnable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_TimerIntEnable)
#define	ROM_UARTParityModeSet                        ((void (*)(unsigned long ulBase, unsigned long ulParity))func_UARTParityModeSet)
#define	ROM_UARTParityModeGet                        ((unsigned long (*)(unsigned long ulBase))func_UARTParityModeGet)
#define	ROM_UARTFIFOLevelSet                         ((void (*)(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel))func_UARTFIFOLevelSet)
#define	ROM_UARTFIFOLevelGet                         ((void (*)(unsigned long ulBase, unsigned long *pulTxLevel, unsigned long *pulRxLevel))func_UARTFIFOLevelGet)
#define	ROM_UARTEnable                               ((void (*)(unsigned long ulBase))func_UARTEnable)
#define	ROM_UARTDisable                              ((void (*)(unsigned long ulBase))func_UARTDisable)
#define	ROM_UARTConfigSetExpClk                      ((void (*)(unsigned long ulBase, unsigned long ulPreDivide, unsigned long ulCD, unsigned long ulBDIV, unsigned long ulConfig))func_UARTConfigSetExpClk)
#define	ROM_UARTConfigGetExpClk                      ((void (*)(unsigned long ulBase, unsigned long *pulPreDivide, unsigned long *pulCD, unsigned long *pulBDIV, unsigned long *pulConfig))func_UARTConfigGetExpClk)
#define	ROM_UARTCharsAvail                           ((unsigned char (*)(unsigned long ulBase))func_UARTCharsAvail)
#define	ROM_UARTSpaceAvail                           ((unsigned char (*)(unsigned long ulBase))func_UARTSpaceAvail)
#define	ROM_UARTCharGetNonBlocking                   ((long (*)(unsigned long ulBase))func_UARTCharGetNonBlocking)
#define	ROM_UARTCharGet                              ((long (*)(unsigned long ulBase))func_UARTCharGet)
#define	ROM_UARTNCharGet                             ((void (*)(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte))func_UARTNCharGet)
#define	ROM_UARTCharGetWithTimeout                   ((unsigned long (*)(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte))func_UARTCharGetWithTimeout)
#define	ROM_UARTCharPutNonBlocking                   ((unsigned char (*)(unsigned long ulBase, unsigned char ucData))func_UARTCharPutNonBlocking)
#define	ROM_UARTCharPut                              ((void (*)(unsigned long ulBase, unsigned char ucData))func_UARTCharPut)
#define	ROM_UARTNCharPut                             ((void (*)(unsigned long ulBase, unsigned char *pucData, unsigned long ulLenByte))func_UARTNCharPut)
#define	ROM_UARTBreakCtl                             ((void (*)(unsigned long ulBase, unsigned char bBreakState))func_UARTBreakCtl)
#define	ROM_UARTIntRegister                          ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_UARTIntRegister)
#define	ROM_UARTIntUnregister                        ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_UARTIntUnregister)
#define	ROM_UARTIntEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_UARTIntEnable)
#define	ROM_UARTIntDisable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_UARTIntDisable)
#define	ROM_UARTIntStatus                            ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))func_UARTIntStatus)
#define	ROM_UARTIntClear                             ((void (*)(unsigned long ulBase))func_UARTIntClear)
#define	ROM_UARTRxErrorGet                           ((unsigned long (*)(unsigned long ulBase))func_UARTRxErrorGet)
#define	ROM_UARTRxErrorClear                         ((void (*)(unsigned long ulBase))func_UARTRxErrorClear)
#define	ROM_UARTStringPut                            ((void (*)(unsigned long ulBase, unsigned char *str))func_UARTStringPut)
#define	ROM_UARTConfigSetBaudrate                    ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned long ulConfig))func_UARTConfigSetBaudrate)
#define	ROM_UARTAutoBaudrate                         ((void (*)(unsigned long ulBase))func_UARTAutoBaudrate)
#define	ROM_UARTConfigSetMode                        ((void (*)(unsigned long ulBase, unsigned long ulConfig))func_UARTConfigSetMode)
#define	ROM_UARTWaitTxDone                           ((void (*)(unsigned long ulBase))func_UARTWaitTxDone)
#define	ROM_UARTPrintf                               ((void (*)(unsigned long ulBase, char* fmt, ...))func_UARTPrintf)
#define ROM_UTCTimerStop                             ((void (*)(void))func_UTCTimerStop)
#define ROM_UTCTimerRun                              ((void (*)(void))func_UTCTimerRun)
#define ROM_UTCCalStop                               ((void (*)(void))func_UTCCalStop)
#define ROM_UTCCalRun                                ((void (*)(void))func_UTCCalRun)
#define ROM_UTCDivStop                               ((void (*)(void))func_UTCDivStop)
#define ROM_UTCDivEn                                 ((void (*)(void))func_UTCDivEn)
#define ROM_UTCHourModeSet                           ((void (*)(unsigned long ulMode))func_UTCHourModeSet)
#define ROM_UTCHourModeGet                           ((unsigned long (*)(void))func_UTCHourModeGet)
#define ROM_UTCTimerSet                              ((void (*)(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec))func_UTCTimerSet)
#define ROM_UTCTimerChangeGet                        ((unsigned long (*)(void))func_UTCTimerChangeGet)
#define ROM_UTCTimerGet                              ((void (*)(unsigned char *ulAMPM, unsigned char *ulHour, unsigned char *ulMin, unsigned char *ulSec, unsigned char *ulMinSec))func_UTCTimerGet)
#define ROM_UTCTimerAlarmSet                         ((void (*)(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec))func_UTCTimerAlarmSet)
#define ROM_UTCCalSet                                ((void (*)(unsigned long ulCentury, unsigned long ulYear, unsigned long ulMonth, unsigned long ulData, unsigned long ulDay))func_UTCCalSet)
#define ROM_UTCCalChangeGet                          ((unsigned long (*)(void))func_UTCCalChangeGet)
#define ROM_UTCCalGet                                ((void (*)(unsigned char *ulCentury, unsigned char *ulYear, unsigned char *ulMonth, unsigned char *ulData, unsigned char *ulDay))func_UTCCalGet)
#define ROM_UTCCalAlarmSet                           ((void (*)(unsigned long ulMonth, unsigned long ulData))func_UTCCalAlarmSet)
#define ROM_UTCAlarmEnable                           ((void (*)(unsigned long ulAlarmFlags))func_UTCAlarmEnable)
#define ROM_UTCAlarmDisable                          ((void (*)(unsigned long ulAlarmFlags))func_UTCAlarmDisable)
#define ROM_UTCIntEnable                             ((void (*)(unsigned long ulIntFlags))func_UTCIntEnable)
#define ROM_UTCIntDisable                            ((void (*)(unsigned long ulIntFlags))func_UTCIntDisable)
#define ROM_UTCIntMaskSet                            ((void (*)(unsigned long ulIntMask))func_UTCIntMaskSet)
#define ROM_UTCIntMaskGet                            ((unsigned long (*)(void))func_UTCIntMaskGet)
#define ROM_UTCIntStatusSet                          ((void (*)(unsigned long ulIntFlags))func_UTCIntStatusSet)
#define ROM_UTCIntStatusGet                          ((unsigned long (*)(void))func_UTCIntStatusGet)
#define ROM_UTCValidStatusGet                        ((unsigned long (*)(void))func_UTCValidStatusGet)
#define ROM_UTCKeepRun                               ((void (*)(unsigned long ulKeepUTC))func_UTCKeepRun)
#define ROM_UTCClkCntSet                             ((void (*)(unsigned long ulClkCnt))func_UTCClkCntSet)
#define ROM_UTCClkCntGet                             ((unsigned long (*)(void))func_UTCClkCntGet)
#define ROM_UTCIntRegister                           ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_UTCIntRegister)
#define ROM_UTCIntUnregister                         ((void (*)(unsigned long *g_pRAMVectors))func_UTCIntUnregister)
#define ROM_UTCTimerSetBy10ms                        ((void (*)(unsigned long ulMinSec))func_UTCTimerSetBy10ms)
#define ROM_UTCTimerGetBy10ms                        ((unsigned long (*)(void))func_UTCTimerGetBy10ms)
#define ROM_UTCTimerAlarmSetBy10ms                   ((void (*)(unsigned long ulMinSec))func_UTCTimerAlarmSetBy10ms)
#define ROM_WatchdogRunning                          ((unsigned char (*)(void))func_WatchdogRunning)
#define ROM_WatchdogEnable                           ((void (*)(void))func_WatchdogEnable)
#define ROM_WatchdogResetEnable                      ((void (*)(void))func_WatchdogResetEnable)
#define ROM_WatchdogResetDisable                     ((void (*)(void))func_WatchdogResetDisable)
#define ROM_WatchdogTimerRepeatEnable                ((void (*)(void))func_WatchdogTimerRepeatEnable)
#define ROM_WatchdogTimerRepeatDisable               ((void (*)(void))func_WatchdogTimerRepeatDisable)
#define ROM_WatchdogReloadSet                        ((void (*)(unsigned long ulLoadVal))func_WatchdogReloadSet)
#define ROM_WatchdogValueGet                         ((unsigned long (*)(void))func_WatchdogValueGet)
#define ROM_WatchdogIntRegister                      ((void (*)(unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_WatchdogIntRegister)
#define ROM_WatchdogIntUnregister                    ((void (*)(unsigned long *g_pRAMVectors))func_WatchdogIntUnregister)
#define ROM_WatchdogIntEnable                        ((void (*)(void))func_WatchdogIntEnable)
#define ROM_WatchdogIntStatus                        ((unsigned long (*)(unsigned char bMasked))func_WatchdogIntStatus)
#define ROM_WatchdogIntClear                         ((void (*)(void))func_WatchdogIntClear)
#define ROM_WatchdogDisable                          ((void (*)(void))func_WatchdogDisable)

#endif


#ifdef ROM_VERSION_2_2

#define func_QSPIIntRegister                          0x00003561
#define func_QSPIIntUnregister                        0x00003579
#define func_UARTParityModeSet                        0x0000358d
#define func_UARTParityModeGet                        0x00003599
#define func_UARTFIFOLevelSet                         0x000035a3
#define func_UARTFIFOLevelGet                         0x000035a9
#define func_UARTEnable                               0x000035bd
#define func_UARTDisable                              0x000035d9
#define func_UARTConfigSetExpClk                      0x000035e3
#define func_UARTConfigGetExpClk                      0x00003625
#define func_UARTCharsAvail                           0x00003649
#define func_UARTSpaceAvail                           0x00003655
#define func_UARTCharGetNonBlocking                   0x00003661
#define func_UARTCharGet                              0x00003675
#define func_UARTNCharGet                             0x00003687
#define func_UARTCharGetWithTimeout                   0x000036a7
#define func_UARTCharPutNonBlocking                   0x000036df
#define func_UARTCharPut                              0x000036f3
#define func_UARTNCharPut                             0x00003703
#define func_UARTBreakCtl                             0x00003723
#define func_UARTIntRegister                          0x00003737
#define func_UARTIntUnregister                        0x00003757
#define func_UARTIntEnable                            0x0000376f
#define func_UARTIntDisable                           0x00003777
#define func_UARTIntStatus                            0x0000377f
#define func_UARTIntClear                             0x0000378b
#define func_UARTRxErrorGet                           0x00003793
#define func_UARTRxErrorClear                         0x0000379d
#define func_UARTStringPut                            0x000037a9
#define func_UARTConfigSetBaudrate                    0x000037c3
#define func_UARTAutoBaudrate                         0x00003837
#define func_UARTConfigSetMode                        0x000038bf
#define func_UARTWaitTxDone                           0x000038cd
#define func_UARTPrintf                               0x00003a6d
#define func_IntRegister                              0x00003b9d
#define func_IntUnregister                            0x00003bc9
#define func_IntPriorityGroupingSet                   0x00003bd1
#define func_IntPriorityGroupingGet                   0x00003be3
#define func_IntPrioritySet                           0x00003c07
#define func_IntPriorityGet                           0x00003c35
#define func_IntEnable                                0x00003c4b
#define func_IntDisable                               0x00003cd1
#define func_IntPendSet                               0x00003d59
#define func_IntPendClear                             0x00003dc9
#define func_TimerEnable                              0x00003e41
#define func_TimerDisable                             0x00003e4b
#define func_TimerConfigure                           0x00003e55
#define func_TimerControlLevel                        0x00003e61
#define func_TimerPrescaleSet                         0x00003e75
#define func_TimerPrescaleGet                         0x00003e81
#define func_TimerLoadSet                             0x00003e8b
#define func_TimerValueGet                            0x00003e8f
#define func_TimerMatchSet                            0x00003e95
#define func_TimerMatchGet                            0x00003e99
#define func_TimerPWMSet                              0x00003e9f
#define func_TimerPWMGet                              0x00003ea3
#define func_TimerIntRegister                         0x00003ea9
#define func_TimerIntUnregister                       0x00003ec3
#define func_TimerIntEnable                           0x00003ed9
#define func_SysTickEnable                            0x00003ee5
#define func_SysTickDisable                           0x00003ef7
#define func_SysTickIntRegister                       0x00003f09
#define func_SysTickIntUnregister                     0x00003f31
#define func_SysTickIntEnable                         0x00003f55
#define func_SysTickIntDisable                        0x00003f67
#define func_SysTickPeriodSet                         0x00003f79
#define func_SysTickPeriodGet                         0x00003f83
#define func_SysTickValueGet                          0x00003f8d
#define func_DMAErrorStatusGet                        0x00003f95
#define func_DMAErrorStatusClear                      0x00003fa7
#define func_DMAChannelRequest                        0x00003fc1
#define func_DMAChannelControlSet                     0x0000400b
#define func_DMAChannelTransferSet                    0x00004017
#define func_DMAChannelNextPointerSet                 0x00004035
#define func_DMAIntRegister                           0x00004041
#define func_DMAIntUnregister                         0x0000405b
#define func_DMAIntStatus                             0x00004071
#define func_DMAIntClear                              0x000040a1
#define func_DMAChannelWaitIdle                       0x000040cb
#define func_DMAChannelTransfer                       0x000040f7
#define func_DMAWaitAESDone                           0x00004123
#define func_DMAWaitChannelDone                       0x0000413f
#define func_GetDMAChannelTransCnt                    0x00004157
#define func_SPIConfigSetExpClk                       0x00004169
#define func_SPIEnable                                0x0000417f
#define func_SPIDisable                               0x00004185
#define func_SPIIntRegister                           0x0000418b
#define func_SPIIntUnregister                         0x000041ab
#define func_SPIIntEnable                             0x000041c3
#define func_SPIIntDisable                            0x000041cb
#define func_SPIIntStatus                             0x000041d3
#define func_SPIIntClear                              0x000041df
#define func_SPIWordDataPut                           0x000041e3
#define func_SPIHalfWordDataPut                       0x000041f3
#define func_SPIByteDataPut                           0x00004203
#define func_SPIDataPutNonBlocking                    0x00004213
#define func_SPIWordDataGet                           0x00004227
#define func_SPIHalfWordDataGet                       0x00004239
#define func_SPIByteDataGet                           0x0000424b
#define func_SPIDataGetNonBlocking                    0x0000425f
#define func_SPISetTxFifoThreshold                    0x00004275
#define func_SPISetRxFifoThreshold                    0x00004279
#define func_SPISetDelay                              0x0000427d
#define func_SPIEnManualCS                            0x00004281
#define func_SPIDisManualCS                           0x0000428b
#define func_SPIEnManualTransmit                      0x00004295
#define func_SPIDisManualTransmit                     0x0000429f
#define func_SPIStartManualTransmit                   0x000042a9
#define func_SPIEnModeFail                            0x000042b3
#define func_SPIDisModeFail                           0x000042bd
#define func_SPITxFifoFlush                           0x000042c7
#define func_SPITxFifoNormal                          0x000042d1
#define func_SPIRxFifoFlush                           0x000042db
#define func_SPIRxFifoNormal                          0x000042e5
#define func_WatchdogRunning                          0x000042f1
#define func_WatchdogEnable                           0x000042fb
#define func_WatchdogResetEnable                      0x00004309
#define func_WatchdogResetDisable                     0x00004317
#define func_WatchdogTimerRepeatEnable                0x00004325
#define func_WatchdogTimerRepeatDisable               0x00004333
#define func_WatchdogReloadSet                        0x00004341
#define func_WatchdogValueGet                         0x00004347
#define func_WatchdogIntRegister                      0x0000434d
#define func_WatchdogIntUnregister                    0x00004365
#define func_WatchdogIntEnable                        0x00004379
#define func_WatchdogIntStatus                        0x00004387
#define func_WatchdogIntClear                         0x0000439b
#define func_WatchdogDisable                          0x000043a5
#define func_flash_vendor_delay                       0x000043b9
#define func_FLASH_Init                               0x000043c5
#define func_FLASH_ChipErase                          0x0000441f
#define func_FLASH_SectorErase                        0x00004483
#define func_FLASH_BlockErase32K                      0x00004501
#define func_FLASH_BlockErase64K                      0x0000457f
#define func_FLASH_WriteData                          0x000045fd
#define func_FLASH_WaitIdle                           0x00004705
#define func_FLASH_FAST_WriteData                     0x00004741
#define func_FLASH_ReadData                           0x0000478d
#define func_FLASH_UpdateData                         0x0000480f
#define func_FLASH_EnableQEFlag                       0x0000491d
#define func_FLASH_SetReadMode                        0x00004a13
#define func_FLASH_EnterXIPMode                       0x00004a23
#define func_FLASH_ExitXIPMode                        0x00004a6f
#define func_FLASH_SetWriteMode                       0x00004ab3
#define func_FLASH_GetRDID                            0x00004ac3
#define func_FLASH_SecurityErase                      0x00004aff
#define func_FLASH_SecurityEraseExt                   0x00004b77
#define func_FLASH_SecurityLock                       0x00004bf5
#define func_FLASH_SecurityLockExt                    0x00004cd7
#define func_FLASH_GetUniqueID                        0x00004e5f
#define func_FLASH_GetDeviceID                        0x00004e71
#define func_I2CMasterEnable                          0x00004ead
#define func_I2CMasterInitExpClk                      0x00004eb7
#define func_I2CSlaveEnable                           0x00004f29
#define func_I2CSlaveInit                             0x00004f33
#define func_I2CIntRegister                           0x00004f43
#define func_I2CIntUnregister                         0x00004f6f
#define func_I2CIntEnable                             0x00004f93
#define func_I2CIntDisable                            0x00004f9b
#define func_I2CIntStatus                             0x00004fa3
#define func_I2CIntClear                              0x00004fb3
#define func_I2CAddrSet                               0x00004fbb
#define func_I2CBusBusy                               0x00004ff1
#define func_I2CControlSet                            0x00005003
#define func_I2CControlGet                            0x00005007
#define func_I2CDataPut                               0x0000500d
#define func_I2CDataGet                               0x00005011
#define func_I2CStatus                                0x00005017
#define func_I2CACKModeSet                            0x0000501d
#define func_I2CFifoClear                             0x00005033
#define func_UTCTimerStop                             0x00005045
#define func_UTCTimerRun                              0x00005053
#define func_UTCCalStop                               0x00005061
#define func_UTCCalRun                                0x0000506f
#define func_UTCDivStop                               0x0000507d
#define func_UTCDivEn                                 0x0000508b
#define func_UTCHourModeSet                           0x00005099
#define func_UTCHourModeGet                           0x0000509f
#define func_UTCTimerSet                              0x000050a5
#define func_UTCTimerChangeGet                        0x00005143
#define func_UTCTimerGet                              0x0000514b
#define func_UTCTimerSetBy10ms                        0x000051a5
#define func_UTCTimerGetBy10ms                        0x000052cf
#define func_UTCTimerAlarmSet                         0x0000534b
#define func_UTCTimerAlarmSetBy10ms                   0x000053dd
#define func_UTCCalSet                                0x000054f1
#define func_UTCCalChangeGet                          0x00005589
#define func_UTCCalGet                                0x00005591
#define func_UTCCalAlarmSet                           0x000055eb
#define func_UTCAlarmEnable                           0x00005623
#define func_UTCAlarmDisable                          0x0000562f
#define func_UTCIntEnable                             0x0000563b
#define func_UTCIntDisable                            0x00005647
#define func_UTCIntMaskSet                            0x00005653
#define func_UTCIntMaskGet                            0x0000565f
#define func_UTCIntStatusSet                          0x00005665
#define func_UTCIntStatusGet                          0x00005671
#define func_UTCValidStatusGet                        0x00005677
#define func_UTCKeepRun                               0x0000567d
#define func_UTCClkCntSet                             0x00005683
#define func_UTCClkCntGet                             0x0000569b
#define func_UTCIntRegister                           0x000056c1
#define func_UTCIntUnregister                         0x000056d9
#define func_GPIOPeripheralPad                        0x000056f1
#define func_GPIODirModeSet                           0x000056fd
#define func_GPIOOutputODEn                           0x000057d3
#define func_GPIOOutputODDis                          0x00005801
#define func_GPIOAnalogEn                             0x0000582f
#define func_GPIOAnalogDis                            0x00005861
#define func_GPIOPullupEn                             0x00005893
#define func_GPIOPullupDis                            0x000058c1
#define func_GPIOPulldownEn                           0x000058ef
#define func_GPIOPulldownDis                          0x0000591d
#define func_GPIOPinSet                               0x0000594b
#define func_GPIOPinClear                             0x00005979
#define func_GPIOPinRead                              0x000059a7
#define func_GPIOIntEnable                            0x000059d7
#define func_GPIOIntDisable                           0x00005a05
#define func_GPIOIntTypeSet                           0x00005a33
#define func_GPIOIntEdgeSet                           0x00005a77
#define func_GPIOIntSingleEdgeSet                     0x00005abb
#define func_GPIOIntStatusGet                         0x00005b05
#define func_GPIOIntMaskGet                           0x00005b17
#define func_GPIOConflictStatusGet                    0x00005b49
#define func_GPIOConflictStatusClear                  0x00005b7b
#define func_GPIOAllocationStatusGet                  0x00005ba9
#define func_GPIODrvStrengthSet                       0x00005bdd
#define func_GPIOIntRegister                          0x00005c13
#define func_GPIOIntUnregister                        0x00005c2b
#define func_GPIOModeSet                              0x00005c3f
#define func_GPIOPadConfigSel                         0x00005c81
#define func_GPIOPadModeSet                           0x00005cc3
#define func_GPIODirectionSet                         0x00005d3d
#define func_prcm_delay                               0x00005dd1
#define func_AON_DSP_Load_Flag_Get                    0x00005ddd
#define func_AON_ARM_Load_Flag_Get                    0x00005dfb
#define func_AON_SECONDARY_Load_Flag_Get              0x00005e19
#define func_AON_DSP_Load_Flag_Set                    0x00005e37
#define func_AON_ARM_Load_Flag_Set                    0x00005e57
#define func_AON_SECONDARY_Load_Flag_Set              0x00005e77
#define func_AON_Flash_Delay_Get                      0x00005e97
#define func_AON_Flash_Delay_Set                      0x00005f35
#define func_AON_Addr_Overlap_Unused_Mem              0x00005f4b
#define func_PRCM_Clock_Enable                        0x00006029
#define func_PRCM_Clock_Disable                       0x00006031
#define func_PRCM_CHIP_VER_GET                        0x00006039
#define func_AON_BOOTMODE_GET                         0x0000603f
#define func_CSPRxFrameSet                            0x00006061
#define func_CSPTxFrameSet                            0x00006087
#define func_CSPClockSet                              0x000060b7
#define func_CSPEndianModeSet                         0x000061d3
#define func_CSPOperationModeSet                      0x000061df
#define func_CSPUARTModeSet                           0x000061eb
#define func_CSPIrdaEnable                            0x00006271
#define func_CSPIrdaPulseWidthSet                     0x0000627d
#define func_CSPIrdaIdleLevelSet                      0x00006289
#define func_CSPIrdaMode                              0x00006295
#define func_CSPSPIMode                               0x00006337
#define func_CSPFIFOLevelSet                          0x0000637f
#define func_CSPCharGet                               0x00006389
#define func_CSPCharPut                               0x000063a1
#define func_CSPCharGetNonBlocking                    0x000063b5
#define func_CSPCharPutNonBlocking                    0x000063cd
#define func_CSPClockModeSet                          0x000063e5
#define func_CSPEnable                                0x000063f1
#define func_CSPDisable                               0x000063fb
#define func_CSPDrivenEdgeSet                         0x00006405
#define func_CSPSyncValidLevelSet                     0x00006417
#define func_CSPClockIdleModeSet                      0x00006429
#define func_CSPClockIdleLevelSet                     0x00006435
#define func_CSPPinModeSet                            0x00006441
#define func_CSPRxEnable                              0x0000647f
#define func_CSPRxDisable                             0x00006489
#define func_CSPTxEnable                              0x00006493
#define func_CSPTxDisable                             0x0000649d
#define func_CSPIntRegister                           0x000064a7
#define func_CSPIntUnregister                         0x000064c1
#define func_CSPIntEnable                             0x000064d7
#define func_CSPIntDisable                            0x000064df
#define func_CSPIntStatus                             0x000064e7
#define func_CSPIntClear                              0x000064ed
#define func_AESKeyLenSet                             0x000064f9
#define func_AESKeySet                                0x00006509
#define func_AESIVSet                                 0x00006511
#define func_AESAADLen0Set                            0x00006519
#define func_AESAADLen1Set                            0x0000651f
#define func_AESPayloadLenSet                         0x00006525
#define func_AESModeSet                               0x0000652b
#define func_AESEncDecSet                             0x0000653b
#define func_AESDMAEn                                 0x0000654b
#define func_AESDMADis                                0x00006559
#define func_AESBlockDataInput                        0x00006567
#define func_AESBlockDataOutput                       0x0000656f
#define func_AESBlockStart                            0x00006577
#define func_AESKeyLoadWaitDone                       0x00006585
#define func_AESBlockTransWaitDone                    0x000065a1
#define func_AESClockDiv2En                           0x000065b1
#define func_AESClockDiv2Dis                          0x000065bf
#define func_AESCCMAuthLenSet                         0x000065cd
#define func_AESCCMLengthLenSet                       0x000065df
#define func_AESTagGet                                0x000065f1
#define func_AESBlock                                 0x000065fb
#define func_AESECB                                   0x00006793
#define func_AESCCM                                   0x00006945
#define func_AES_128_EEA2                             0x00006c1b
#define func_AES_CMAC_XOR_128                         0x00007011
#define func_AES_CMAC_Leftshift_Onebit                0x0000702b
#define func_AES_CMAC_Generate_Subkey                 0x00007051
#define func_AES_128_EIA2                             0x000070cf
#define func_SEMAMasterEnable                         0x00007439
#define func_SEMAMasterDisable                        0x00007451
#define func_SEMARequest                              0x00007469
#define func_SEMARequestNonBlocking                   0x0000748f
#define func_SEMARequestPriSet                        0x000074a7
#define func_SEMAMasterGet                            0x000074b7
#define func_SEMASlaveGet                             0x000074c9
#define func_SEMAReset                                0x000074d7
#define func_SEMAReqQueueState                        0x000074eb
#define func_SEMAMasterIntEnable                      0x000074f7
#define func_SEMARelease                              0x00007509
#define func_SEMAIntRegister                          0x00007521
#define func_SEMAIntUnregister                        0x00007539
#define func_MCNTStart                                0x00007551
#define func_MCNTStop                                 0x00007559
#define func_MCNTSetCNT32K                            0x00007561
#define func_MCNTGetCNT32K                            0x00007567
#define func_MCNTGetMCNT                              0x0000756d
#define func_MCNTGetIntStatus                         0x00007573
#define func_MCNTSetIntStatus                         0x0000757d
#define func_MCNTIntRegister                          0x00007583
#define func_MCNTIntUnregister                        0x0000759b
#define func_REG_Bus_Field_Set                        0x000075b5
#define func_REG_Bus_Field_Get                        0x000076ff
#define func_PRCM_Clock_Mode_Force_XTAL               0x00007823
#define func_PRCM_Clock_Mode_Auto                     0x0000783f


#define ROM_AESKeyLenSet                             ((void (*)(unsigned long ulKeyLenMode))func_AESKeyLenSet)
#define ROM_AESKeySet                                ((void (*)(unsigned char ucOffset, unsigned long ulKeyValue))func_AESKeySet)
#define ROM_AESIVSet                                 ((void (*)(unsigned char ucOffset, unsigned long ucIV))func_AESIVSet)
#define ROM_AESAADLen0Set                            ((void (*)(unsigned long ulAadLen0))func_AESAADLen0Set)
#define ROM_AESAADLen1Set                            ((void (*)(unsigned long ulAadLen1))func_AESAADLen1Set)
#define ROM_AESPayloadLenSet                         ((void (*)(unsigned long ulPayloadLenByte))func_AESPayloadLenSet)
#define ROM_AESModeSet                               ((void (*)(unsigned long ucAESMode))func_AESModeSet)
#define ROM_AESEncDecSet                             ((void (*)(unsigned long ucAESEncDec))func_AESEncDecSet)
#define ROM_AESDMAEn                                 ((void (*)(void))func_AESDMAEn)
#define ROM_AESDMADis                                ((void (*)(void))func_AESDMADis)
#define ROM_AESBlockDataInput                        ((void (*)(unsigned char ucOffset, unsigned long ulDataIn))func_AESBlockDataInput)
#define ROM_AESBlockDataOutput                       ((unsigned long (*)(unsigned char ucOffset))func_AESBlockDataOutput)
#define ROM_AESBlockStart                            ((void (*)(void))func_AESBlockStart)
#define ROM_AESKeyLoadWaitDone                       ((void (*)(void))func_AESKeyLoadWaitDone)
#define ROM_AESBlockTransWaitDone                    ((void (*)(void))func_AESBlockTransWaitDone)
#define ROM_AESClockDiv2En                           ((void (*)(void))func_AESClockDiv2En)
#define ROM_AESClockDiv2Dis                          ((void (*)(void))func_AESClockDiv2Dis)
#define ROM_AESCCMAuthLenSet                         ((void (*)(unsigned long ucAuthLenByte))func_AESCCMAuthLenSet)
#define ROM_AESCCMLengthLenSet                       ((void (*)(unsigned char ucLengthLenByte))func_AESCCMLengthLenSet)
#define ROM_AESTagGet                                ((unsigned long (*)(unsigned char ucOffset))func_AESTagGet)
#define ROM_AESBlock                                 ((void (*)(unsigned char *pucInput, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput))func_AESBlock)
#define ROM_AESECB                                   ((void (*)(unsigned char *pucInput, unsigned long ulInputLenByte, unsigned char *pucKey, unsigned long key_len_bits, unsigned char ucMode, unsigned char *pucOutput))func_AESECB)
#define ROM_AESCCM                                   ((void (*)(unsigned char *aad, unsigned long aad_len_byte, unsigned char *input_message, unsigned long input_len_byte, unsigned long *user_key, unsigned long key_len_bits, unsigned long M, unsigned long L, unsigned char *nonce, unsigned char mode, unsigned char *output_message, unsigned char *digest))func_AESCCM)
#define ROM_AES_128_EEA2                             ((void (*)(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucOutMsg))func_AES_128_EEA2)
#define ROM_AES_CMAC_XOR_128                         ((void (*)(unsigned char *pucA, const unsigned char *pucB, unsigned char *pucOut))func_AES_CMAC_XOR_128)
#define ROM_AES_CMAC_Leftshift_Onebit                ((void (*)(unsigned char *pucInput, unsigned char *pucOutput))func_AES_CMAC_Leftshift_Onebit)
#define ROM_AES_CMAC_Generate_Subkey                 ((void (*)(unsigned char *pucKey, unsigned char *pucK1, unsigned char *pucK2))func_AES_CMAC_Generate_Subkey)
#define ROM_AES_128_EIA2                             ((void (*)(unsigned char *pucMessage, unsigned char *pucKey, unsigned char *pucCount, unsigned char ucBearer, unsigned char ucDir, unsigned long ulLengthBit, unsigned char *pucMACT, unsigned char *ucM))func_AES_128_EIA2)
#define	ROM_AON_DSP_Load_Flag_Get                    ((unsigned char (*)(void))func_AON_DSP_Load_Flag_Get)
#define	ROM_AON_ARM_Load_Flag_Get                    ((unsigned char (*)(void))func_AON_ARM_Load_Flag_Get)
#define	ROM_AON_DSP_Load_Flag_Set                    ((void (*)(uint8_t flag))func_AON_DSP_Load_Flag_Set)
#define	ROM_AON_ARM_Load_Flag_Set                    ((void (*)(uint8_t flag))func_AON_ARM_Load_Flag_Set)
#define	ROM_AON_Flash_Delay_Get                      ((void (*)(void))func_AON_Flash_Delay_Get)
#define	ROM_AON_Flash_Delay_Set                      ((void (*)(uint8_t value))func_AON_Flash_Delay_Set)
#define	ROM_AON_Addr_Overlap_Unused_Mem              ((unsigned char (*)(unsigned char mem_status, unsigned long ulAddr, unsigned long ulSizebyte))func_AON_Addr_Overlap_Unused_Mem)
#define ROM_CSPRxFrameSet                            ((void (*)(unsigned long ulBase, unsigned long ulDelay, unsigned long ulDataLen, unsigned long ulFrameLen))func_CSPRxFrameSet)
#define ROM_CSPTxFrameSet                            ((void (*)(unsigned long ulBase, unsigned long ulDelay, unsigned long ulDataLen, unsigned long ulSyncLen, unsigned long ulFrameLen))func_CSPTxFrameSet)
#define ROM_CSPClockSet                              ((void (*)(unsigned long ulBase, unsigned long ulCSPMode, unsigned long ulPclk, unsigned ulClock))func_CSPClockSet)
#define ROM_CSPEndianModeSet                         ((void (*)(unsigned long ulBase, unsigned long ulEndianMode))func_CSPEndianModeSet)
#define ROM_CSPOperationModeSet                      ((void (*)(unsigned long ulBase, unsigned long ulOperationMode))func_CSPOperationModeSet)
#define ROM_CSPUARTModeSet                           ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned char ucDatabits, unsigned char ucStopbits))func_CSPUARTModeSet)
#define ROM_CSPIrdaEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIrdaEn))func_CSPIrdaEnable)
#define ROM_CSPIrdaPulseWidthSet                     ((void (*)(unsigned long ulBase, unsigned long ulWidth))func_CSPIrdaPulseWidthSet)
#define ROM_CSPIrdaIdleLevelSet                      ((void (*)(unsigned long ulBase, unsigned long ulIrdaIdleLevel))func_CSPIrdaIdleLevelSet)
#define ROM_CSPIrdaMode                              ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned char ucDatabits, unsigned char ucStopbits, unsigned long ulIdleLevel, unsigned long ulWidth))func_CSPIrdaMode)
#define ROM_CSPSPIMode                               ((void (*)(unsigned long ulBase, unsigned long ulMode1, unsigned long ulMode2, unsigned long ulTxFrameCtl, unsigned long ulRxFrameCtl))func_CSPSPIMode)
#define ROM_CSPFIFOLevelSet                          ((void (*)(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel))func_CSPFIFOLevelSet)
#define ROM_CSPCharGet                               ((unsigned char (*)(unsigned long ulBase))func_CSPCharGet)
#define ROM_CSPCharPut                               ((void (*)(unsigned long ulBase, unsigned char ucData))func_CSPCharPut)
#define ROM_CSPCharGetNonBlocking                    ((long (*)(unsigned long ulBase))func_CSPCharGetNonBlocking)
#define ROM_CSPCharPutNonBlocking                    ((long (*)(unsigned long ulBase, unsigned char ucData))func_CSPCharPutNonBlocking)
#define ROM_CSPClockModeSet                          ((void (*)(unsigned long ulBase, unsigned long ulClockMode))func_CSPClockModeSet)
#define ROM_CSPEnable                                ((void (*)(unsigned long ulBase))func_CSPEnable)
#define ROM_CSPDisable                               ((void (*)(unsigned long ulBase))func_CSPDisable)
#define ROM_CSPDrivenEdgeSet                         ((void (*)(unsigned long ulBase, unsigned long ulRxEdge, unsigned long ulTxEdge))func_CSPDrivenEdgeSet)
#define ROM_CSPSyncValidLevelSet                     ((void (*)(unsigned long ulBase, unsigned long ulRFS, unsigned long ulTFS))func_CSPSyncValidLevelSet)
#define ROM_CSPClockIdleModeSet                      ((void (*)(unsigned long ulBase, unsigned long ulClkIdleMode))func_CSPClockIdleModeSet)
#define ROM_CSPClockIdleLevelSet                     ((void (*)(unsigned long ulBase, unsigned long ulClkIdleLevel))func_CSPClockIdleLevelSet)
#define ROM_CSPPinModeSet                            ((void (*)(unsigned long ulBase, unsigned long ulCSPPin, unsigned long ulPinMode, unsigned long ulPinDirection))func_CSPPinModeSet)
#define ROM_CSPRxEnable                              ((void (*)(unsigned long ulBase))func_CSPRxEnable)
#define ROM_CSPRxDisable                             ((void (*)(unsigned long ulBase))func_CSPRxDisable)
#define ROM_CSPTxEnable                              ((void (*)(unsigned long ulBase))func_CSPTxEnable)
#define ROM_CSPTxDisable                             ((void (*)(unsigned long ulBase))func_CSPTxDisable)
#define ROM_CSPIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_CSPIntRegister)
#define ROM_CSPIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_CSPIntUnregister)
#define ROM_CSPIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntEnable)
#define ROM_CSPIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntDisable)
#define ROM_CSPIntStatus                             ((unsigned long (*)(unsigned long ulBase))func_CSPIntStatus)
#define ROM_CSPIntClear                              ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_CSPIntClear)
#define ROM_DMAErrorStatusGet                        ((unsigned long (*)(unsigned long ulChannelNum))func_DMAErrorStatusGet)
#define ROM_DMAErrorStatusClear                      ((void (*)(unsigned long ulChannelNum))func_DMAErrorStatusClear)
#define ROM_DMAChannelRequest                        ((void (*)(unsigned long ulChannelNum))func_DMAChannelRequest)
#define ROM_DMAChannelControlSet                     ((void (*)(unsigned long ulChannelNum, unsigned long ulControl))func_DMAChannelControlSet)
#define ROM_DMAChannelTransferSet                    ((void (*)(unsigned long ulChannelNum, void *pvSrcAddr, void *pvDstAddr, unsigned long ulTransferSize))func_DMAChannelTransferSet)
#define ROM_DMAChannelNextPointerSet                 ((void (*)(unsigned long ulChannelNum, void *pvNextPointer))func_DMAChannelNextPointerSet)
#define ROM_DMAIntRegister                           ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_DMAIntRegister)
#define ROM_DMAIntUnregister                         ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors))func_DMAIntUnregister)
#define ROM_DMAIntStatus                             ((unsigned long (*)(unsigned long ulChannelNum))func_DMAIntStatus)
#define ROM_DMAIntClear                              ((void (*)(unsigned long ulChanMask))func_DMAIntClear)
#define ROM_DMAChannelWaitIdle                       ((void (*)(unsigned long ulChannelNum))func_DMAChannelWaitIdle)
#define ROM_DMAChannelTransfer                       ((void (*)(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum))func_DMAChannelTransfer)
#define ROM_DMAWaitAESDone                           ((void (*)(void))func_DMAWaitAESDone)
#define ROM_DMAChannelMemcpy                         ((void (*)(unsigned long ulDst, unsigned long ulSrc, unsigned long ulLenByte, unsigned long ulChannelNum))func_DMAChannelMemcpy)
#define ROM_DMAWaitChannelDone                       ((void (*)(unsigned char dmaChannels))func_DMAWaitChannelDone)
#define ROM_GetDMAChannelTransCnt                    ((unsigned long (*)(unsigned long ulChannelNum))func_GetDMAChannelTransCnt)
#define ROM_FLASH_Init                               ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int ref_clk_hz, unsigned int sclk_hz))func_FLASH_Init)
#define ROM_FLASH_ChipErase                          ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_ChipErase)
#define ROM_FLASH_SectorErase                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_SectorErase)
#define ROM_FLASH_BlockErase32K                      ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_BlockErase32K)
#define ROM_FLASH_BlockErase64K                      ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int address))func_FLASH_BlockErase64K)
#define ROM_FLASH_WriteData                          ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))func_FLASH_WriteData)
#define ROM_FLASH_ReadData                           ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))func_FLASH_ReadData)
#define ROM_FLASH_WaitIdle                           ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_WaitIdle)
#define ROM_FLASH_UpdateData                         ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum, unsigned long ulBackupFlag, unsigned ulExchangeAddr))func_FLASH_UpdateData)
#define ROM_FLASH_EnableQEFlag                       ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_EnableQEFlag)
#define ROM_FLASH_SetReadMode                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int mode))func_FLASH_SetReadMode)
#define ROM_FLASH_EnterXIPMode                       ((void (*)(QSPI_FLASH_Def *flash_vendor, char xip_dummy))func_FLASH_EnterXIPMode)
#define ROM_FLASH_ExitXIPMode                        ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_ExitXIPMode)
#define ROM_FLASH_SetWriteMode                       ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned int mode))func_FLASH_SetWriteMode)
#define ROM_FLASH_GetDeviceID                        ((unsigned int (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_GetDeviceID)
#define ROM_FLASH_SecurityErase                      ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_SecurityErase)
#define ROM_FLASH_SecurityLock                       ((void (*)(QSPI_FLASH_Def *flash_vendor))func_FLASH_SecurityLock)
#define ROM_FLASH_FAST_WriteData                     ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulSrcAddr, unsigned long ulDstAddr, unsigned long ulLenByte, unsigned long ulChannelNum))func_FLASH_FAST_WriteData)
#define ROM_FLASH_GetUniqueID                        ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned char *pucUniqueId))func_FLASH_GetUniqueID)
#define ROM_FLASH_GetRDID                            ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned char *pucRdid))func_FLASH_GetRDID)
#define ROM_FLASH_SecurityEraseExt                   ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned long ulEraseAddr))func_FLASH_SecurityEraseExt)
#define ROM_FLASH_SecurityLockExt                    ((void (*)(QSPI_FLASH_Def *flash_vendor, unsigned char ucLockBit))func_FLASH_SecurityLockExt)
#define ROM_GPIOPeripheralPad                        ((void (*)(unsigned char ucPeriNum, unsigned char ucPadNum))func_GPIOPeripheralPad)
#define ROM_GPIODirModeSet                           ((void (*)(unsigned char ucPins, unsigned long ulPinIO))func_GPIODirModeSet)
#define ROM_GPIOOutputODEn                           ((void (*)(unsigned char ucPins))func_GPIOOutputODEn)
#define ROM_GPIOOutputODDis                          ((void (*)(unsigned char ucPins))func_GPIOOutputODDis)
#define ROM_GPIOAnalogEn                             ((void (*)(unsigned char ucPins))func_GPIOAnalogEn)
#define ROM_GPIOAnalogDis                            ((void (*)(unsigned char ucPins))func_GPIOAnalogDis)
#define ROM_GPIOPullupEn                             ((void (*)(unsigned char ucPins))func_GPIOPullupEn)
#define ROM_GPIOPullupDis                            ((void (*)(unsigned char ucPins))func_GPIOPullupDis)
#define ROM_GPIOPulldownEn                           ((void (*)(unsigned char ucPins))func_GPIOPulldownEn)
#define ROM_GPIOPulldownDis                          ((void (*)(unsigned char ucPins))func_GPIOPulldownDis)
#define ROM_GPIOPinSet                               ((void (*)(unsigned char ucPins))func_GPIOPinSet)
#define ROM_GPIOPinClear                             ((void (*)(unsigned char ucPins))func_GPIOPinClear)
#define ROM_GPIOPinRead                              ((unsigned char (*)(unsigned char ucPins))func_GPIOPinRead)
#define ROM_GPIOIntEnable                            ((void (*)(unsigned char ucPins))func_GPIOIntEnable)
#define ROM_GPIOIntDisable                           ((void (*)(unsigned char ucPins))func_GPIOIntDisable)
#define ROM_GPIOIntTypeSet                           ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntTypeSet)
#define ROM_GPIOIntEdgeSet                           ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntEdgeSet)
#define ROM_GPIOIntSingleEdgeSet                     ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOIntSingleEdgeSet)
#define ROM_GPIOIntStatusGet                         ((unsigned long (*)(unsigned char ucPins))func_GPIOIntStatusGet)
#define ROM_GPIOIntMaskGet                           ((unsigned long (*)(unsigned char ucPins))func_GPIOIntMaskGet)
#define ROM_GPIOConflictStatusGet                    ((unsigned long (*)(unsigned char ucPins))func_GPIOConflictStatusGet)
#define ROM_GPIOConflictStatusClear                  ((void (*)(unsigned char ucPins))PATCH_GPIOConflictStatusClear)
#define ROM_GPIOAllocationStatusGet                  ((unsigned long (*)(unsigned char ucPins))func_GPIOAllocationStatusGet)
#define ROM_GPIODrvStrengthSet                       ((void (*)(unsigned char ucPins, unsigned char ucDrvStrength))func_GPIODrvStrengthSet)
#define ROM_GPIOIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_GPIOIntRegister)
#define ROM_GPIOIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))func_GPIOIntUnregister)
#define	ROM_GPIOModeSet                              ((void (*)(unsigned char ucPins, unsigned char ucMode))func_GPIOModeSet)
#define	ROM_GPIOPadConfigSel                         ((void (*)(unsigned char ucPins, unsigned char ucConfig))func_GPIOPadConfigSel)
#define	ROM_GPIOPadModeSet                           ((void (*)(unsigned char ucPins, unsigned char ucMode, unsigned char ucConfig))func_GPIOPadModeSet)
#define	ROM_GPIODirectionSet                         ((void (*)(unsigned char ucPins, unsigned long ulPinIO))func_GPIODirectionSet)
#define ROM_I2CMasterEnable                          ((void (*)(unsigned long ulBase))func_I2CMasterEnable)
#define ROM_I2CMasterInitExpClk                      ((void (*)(unsigned long ulBase, unsigned long ulI2CClk, unsigned char bFast))func_I2CMasterInitExpClk)
#define ROM_I2CSlaveEnable                           ((void (*)(unsigned long ulBase))func_I2CSlaveEnable)
#define ROM_I2CSlaveInit                             ((void (*)(unsigned long ulBase, unsigned long ucSlaveAddr))func_I2CSlaveInit)
#define ROM_I2CIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(fnHandler)(void)))func_I2CIntRegister)
#define ROM_I2CIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_I2CIntUnregister)
#define ROM_I2CIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_I2CIntEnable)
#define ROM_I2CIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_I2CIntDisable)
#define ROM_I2CIntStatus                             ((unsigned char (*)(unsigned long ulBase, unsigned char bMasked))func_I2CIntStatus)
#define ROM_I2CIntClear                              ((void (*)(unsigned long ulBase))func_I2CIntClear)
#define ROM_I2CAddrSet                               ((void (*)(unsigned long ulBase, unsigned long ulAddr, unsigned char ucNorMode, unsigned char ucReceive))func_I2CAddrSet)
#define ROM_I2CBusBusy                               ((unsigned char (*)(unsigned long ulBase))func_I2CBusBusy)
#define ROM_I2CControlSet                            ((void (*)(unsigned long ulBase, unsigned long ulCmd))func_I2CControlSet)
#define ROM_I2CControlGet                            ((unsigned long (*)(unsigned long ulBase))func_I2CControlGet)
#define ROM_I2CDataPut                               ((void (*)(unsigned long ulBase, unsigned char ucData))func_I2CDataPut)
#define ROM_I2CDataGet                               ((unsigned long (*)(unsigned long ulBase))func_I2CDataGet)
#define ROM_I2CStatus                                ((unsigned long (*)(unsigned long ulBase))func_I2CStatus)
#define ROM_I2CACKModeSet                            ((void (*)(unsigned long ulBase, unsigned char ucEnable))func_I2CACKModeSet)
#define ROM_I2CFifoClear                             ((void (*)(unsigned long ulBase))func_I2CFifoClear)
#define ROM_IntRegister                              ((void (*)(unsigned long ulInterrupt, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_IntRegister)
#define ROM_IntUnregister                            ((void (*)(unsigned long ulInterrupt, unsigned long *g_pRAMVectors))func_IntUnregister)
#define ROM_IntPriorityGroupingSet                   ((void (*)(unsigned long ulBits))func_IntPriorityGroupingSet)
#define ROM_IntPriorityGroupingGet                   ((unsigned long (*)(void))func_IntPriorityGroupingGet)
#define ROM_IntPrioritySet                           ((void (*)(unsigned long ulInterrupt,unsigned char ucPriority))func_IntPrioritySet)
#define ROM_IntPriorityGet                           ((long (*)(unsigned long ulInterrupt))func_IntPriorityGet)
#define ROM_IntEnable                                ((void (*)(unsigned long ulInterrupt))func_IntEnable)
#define ROM_IntDisable                               ((void (*)(unsigned long ulInterrupt))func_IntDisable)
#define ROM_IntPendSet                               ((void (*)(unsigned long ulInterrupt))func_IntPendSet)
#define ROM_IntPendClear                             ((void (*)(unsigned long ulInterrupt))func_IntPendClear)
#define	ROM_MCNTStart                                ((void (*)(void))func_MCNTStart)
#define	ROM_MCNTStop                                 ((void (*)(void))func_MCNTStop)
#define	ROM_MCNTSetCNT32K                            ((void (*)(unsigned long ulCounter))func_MCNTSetCNT32K)
#define	ROM_MCNTGetCNT32K                            ((unsigned long (*)(void))func_MCNTGetCNT32K)
#define	ROM_MCNTGetMCNT                              ((unsigned long (*)(void))func_MCNTGetMCNT)
#define	ROM_MCNTGetIntStatus                         ((unsigned char (*)(void))func_MCNTGetIntStatus)
#define	ROM_MCNTSetIntStatus                         ((void (*)(unsigned char ucIntstatus))func_MCNTSetIntStatus)
#define	ROM_MCNTIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_MCNTIntRegister)
#define	ROM_MCNTIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))func_MCNTIntUnregister)
#define ROM_PRCM_Clock_Enable                        ((void (*)(unsigned long ulBase, unsigned long ulModule))func_PRCM_Clock_Enable)
#define ROM_PRCM_Clock_Disable                       ((void (*)(unsigned long ulBase, unsigned long ulModule))func_PRCM_Clock_Disable)
#define ROM_PRCM_CHIP_VER_GET                        ((unsigned long (*)(unsigned long ulBase))func_PRCM_CHIP_VER_GET)
#define ROM_AON_BOOTMODE_GET                         ((unsigned char (*)(void))func_AON_BOOTMODE_GET)
#define ROM_PRCM_Clock_Mode_Force_XTAL               ((void (*)(void))func_PRCM_Clock_Mode_Force_XTAL)
#define ROM_PRCM_Clock_Mode_Auto                     ((void (*)(void))func_PRCM_Clock_Mode_Auto)
#define ROM_REG_Bus_Field_Set                        ((unsigned char (*)(unsigned long ulRegBase, uint8_t ucBit_high, uint8_t ucBit_low, unsigned long ulValue))func_REG_Bus_Field_Set)
#define ROM_REG_Bus_Field_Get                        ((unsigned char (*)(unsigned long ulRegBase, uint8_t ucBit_high, uint8_t ucBit_low, unsigned long *ulValue))func_REG_Bus_Field_Get)
#define ROM_SEMAMasterEnable                         ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave))func_SEMAMasterEnable)
#define ROM_SEMAMasterDisable                        ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave))func_SEMAMasterDisable)
#define ROM_SEMARequest                              ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARequest)
#define ROM_SEMARequestNonBlocking                   ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARequestNonBlocking)
#define ROM_SEMARequestPriSet                        ((void (*)(unsigned char ucSemaSlave, unsigned long ucPriority))func_SEMARequestPriSet)
#define ROM_SEMAMasterGet                            ((unsigned char (*)(unsigned char ucSemaSlave))func_SEMAMasterGet)
#define ROM_SEMASlaveGet                             ((unsigned long (*)(unsigned char ucSemaMaster))func_SEMASlaveGet)
#define ROM_SEMAReset                                ((void (*)(void))func_SEMAReset)
#define ROM_SEMAReqQueueState                        ((unsigned char (*)(void))func_SEMAReqQueueState)
#define ROM_SEMAMasterIntEnable                      ((void (*)(unsigned long ulSemaMasterInt))func_SEMAMasterIntEnable)
#define ROM_SEMARelease                              ((void (*)(unsigned char ucSemaMaster, unsigned char ucSemaSlave, unsigned char ucMasterMask))func_SEMARelease)
#define ROM_SEMAIntRegister                          ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_SEMAIntRegister)
#define ROM_SEMAIntUnregister                        ((void (*)(unsigned long *g_pRAMVectors))func_SEMAIntUnregister)
#define ROM_SPIConfigSetExpClk                       ((void (*)(unsigned long ulBase, unsigned long ulDivide, unsigned long ulProtocol, unsigned long ulMode,unsigned long ulDataWidth))func_SPIConfigSetExpClk)
#define ROM_SPIEnable                                ((void (*)(unsigned long ulBase))func_SPIEnable)
#define ROM_SPIDisable                               ((void (*)(unsigned long ulBase))func_SPIDisable)
#define ROM_SPIIntRegister                           ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_SPIIntRegister)
#define ROM_SPIIntUnregister                         ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_SPIIntUnregister)
#define ROM_SPIIntEnable                             ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntEnable)
#define ROM_SPIIntDisable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntDisable)
#define ROM_SPIIntStatus                             ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))func_SPIIntStatus)
#define ROM_SPIIntClear                              ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_SPIIntClear)
#define ROM_SPIWordDataPut                           ((void (*)(unsigned long ulBase, unsigned long ulData))func_SPIWordDataPut)
#define ROM_SPIHalfWordDataPut                       ((void (*)(unsigned long ulBase, unsigned short ulData))func_SPIHalfWordDataPut)
#define ROM_SPIByteDataPut                           ((void (*)(unsigned long ulBase, unsigned char ulData))func_SPIByteDataPut)
#define ROM_SPIDataPutNonBlocking                    ((long (*)(unsigned long ulBase, unsigned long ulData))func_SPIDataPutNonBlocking)
#define ROM_SPIWordDataGet                           ((void (*)(unsigned long ulBase, unsigned long *pulData))func_SPIWordDataGet)
#define ROM_SPIHalfWordDataGet                       ((void (*)(unsigned long ulBase, unsigned short *pulData))func_SPIHalfWordDataGet)
#define ROM_SPIByteDataGet                           ((void (*)(unsigned long ulBase, unsigned char *pulData))func_SPIByteDataGet)
#define ROM_SPIDataGetNonBlocking                    ((long (*)(unsigned long ulBase, unsigned long *pulData))func_SPIDataGetNonBlocking)
#define ROM_SPISetTxFifoThreshold                    ((void (*)(unsigned long ulBase, unsigned long ulThreshold))func_SPISetTxFifoThreshold)
#define ROM_SPISetRxFifoThreshold                    ((void (*)(unsigned long ulBase, unsigned long ulThreshold))func_SPISetRxFifoThreshold)
#define ROM_SPISetDelay                              ((void (*)(unsigned long ulBase, unsigned long ulDelay))func_SPISetDelay)
#define ROM_SPIEnManualCS                            ((void (*)(unsigned long ulBase))func_SPIEnManualCS)
#define ROM_SPIDisManualCS                           ((void (*)(unsigned long ulBase))func_SPIDisManualCS)
#define ROM_SPIEnManualTransmit                      ((void (*)(unsigned long ulBase))func_SPIEnManualTransmit)
#define ROM_SPIDisManualTransmit                     ((void (*)(unsigned long ulBase))func_SPIDisManualTransmit)
#define ROM_SPIStartManualTransmit                   ((void (*)(unsigned long ulBase))func_SPIStartManualTransmit)
#define ROM_SPIEnModeFail                            ((void (*)(unsigned long ulBase))func_SPIEnModeFail)
#define ROM_SPIDisModeFail                           ((void (*)(unsigned long ulBase))func_SPIDisModeFail)
#define ROM_SPITxFifoFlush                           ((void (*)(unsigned long ulBase))func_SPITxFifoFlush)
#define ROM_SPITxFifoNormal                          ((void (*)(unsigned long ulBase))func_SPITxFifoNormal)
#define ROM_SPIRxFifoFlush                           ((void (*)(unsigned long ulBase))func_SPIRxFifoFlush)
#define ROM_SPIRxFifoNormal                          ((void (*)(unsigned long ulBase))func_SPIRxFifoNormal)
#define ROM_SysTickEnable                            ((void (*)(void))func_SysTickEnable)
#define ROM_SysTickDisable                           ((void (*)(void))func_SysTickDisable)
#define ROM_SysTickIntRegister                       ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_SysTickIntRegister)
#define ROM_SysTickIntUnregister                     ((void (*)(unsigned long *g_pRAMVectors))func_SysTickIntUnregister)
#define ROM_SysTickIntEnable                         ((void (*)(void))func_SysTickIntEnable)
#define ROM_SysTickIntDisable                        ((void (*)(void))func_SysTickIntDisable)
#define ROM_SysTickPeriodSet                         ((void (*)(unsigned long ulPeriod))func_SysTickPeriodSet)
#define ROM_SysTickPeriodGet                         ((unsigned long (*)(void))func_SysTickPeriodGet)
#define ROM_SysTickValueGet                          ((unsigned long (*)(void))func_SysTickValueGet)
#define ROM_TimerEnable                              ((void (*)(unsigned long ulBase))func_TimerEnable)
#define ROM_TimerDisable                             ((void (*)(unsigned long ulBase))func_TimerDisable)
#define ROM_TimerConfigure                           ((void (*)(unsigned long ulBase, unsigned long ulConfig))func_TimerConfigure)
#define ROM_TimerControlLevel                        ((void (*)(unsigned long ulBase, unsigned char bInvert))func_TimerControlLevel)
#define ROM_TimerPrescaleSet                         ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerPrescaleSet)
#define ROM_TimerPrescaleGet                         ((unsigned long (*)(unsigned long ulBase))func_TimerPrescaleGet)
#define ROM_TimerLoadSet                             ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerLoadSet)
#define ROM_TimerValueGet                            ((unsigned long (*)(unsigned long ulBase))func_TimerValueGet)
#define ROM_TimerMatchSet                            ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerMatchSet)
#define ROM_TimerMatchGet                            ((unsigned long (*)(unsigned long ulBase))func_TimerMatchGet)
#define ROM_TimerPWMSet                              ((void (*)(unsigned long ulBase, unsigned long ulValue))func_TimerPWMSet)
#define ROM_TimerPWMGet                              ((unsigned long (*)(unsigned long ulBase))func_TimerPWMGet)
#define ROM_TimerIntRegister                         ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_TimerIntRegister)
#define ROM_TimerIntUnregister                       ((void (*)(unsigned long ulIntChannel, unsigned long *g_pRAMVectors))func_TimerIntUnregister)
#define ROM_TimerIntEnable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_TimerIntEnable)
#define	ROM_UARTParityModeSet                        ((void (*)(unsigned long ulBase, unsigned long ulParity))func_UARTParityModeSet)
#define	ROM_UARTParityModeGet                        ((unsigned long (*)(unsigned long ulBase))func_UARTParityModeGet)
#define	ROM_UARTFIFOLevelSet                         ((void (*)(unsigned long ulBase, unsigned long ulTxLevel, unsigned long ulRxLevel))func_UARTFIFOLevelSet)
#define	ROM_UARTFIFOLevelGet                         ((void (*)(unsigned long ulBase, unsigned long *pulTxLevel, unsigned long *pulRxLevel))func_UARTFIFOLevelGet)
#define	ROM_UARTEnable                               ((void (*)(unsigned long ulBase))func_UARTEnable)
#define	ROM_UARTDisable                              ((void (*)(unsigned long ulBase))func_UARTDisable)
#define	ROM_UARTConfigSetExpClk                      ((void (*)(unsigned long ulBase, unsigned long ulPreDivide, unsigned long ulCD, unsigned long ulBDIV, unsigned long ulConfig))func_UARTConfigSetExpClk)
#define	ROM_UARTConfigGetExpClk                      ((void (*)(unsigned long ulBase, unsigned long *pulPreDivide, unsigned long *pulCD, unsigned long *pulBDIV, unsigned long *pulConfig))func_UARTConfigGetExpClk)
#define	ROM_UARTCharsAvail                           ((unsigned char (*)(unsigned long ulBase))func_UARTCharsAvail)
#define	ROM_UARTSpaceAvail                           ((unsigned char (*)(unsigned long ulBase))func_UARTSpaceAvail)
#define	ROM_UARTCharGetNonBlocking                   ((long (*)(unsigned long ulBase))func_UARTCharGetNonBlocking)
#define	ROM_UARTCharGet                              ((long (*)(unsigned long ulBase))func_UARTCharGet)
#define	ROM_UARTNCharGet                             ((void (*)(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte))func_UARTNCharGet)
#define	ROM_UARTCharGetWithTimeout                   ((unsigned long (*)(unsigned long ulBase, unsigned char *pulRecvData, unsigned long ulLenByte))func_UARTCharGetWithTimeout)
#define	ROM_UARTCharPutNonBlocking                   ((unsigned char (*)(unsigned long ulBase, unsigned char ucData))func_UARTCharPutNonBlocking)
#define	ROM_UARTCharPut                              ((void (*)(unsigned long ulBase, unsigned char ucData))func_UARTCharPut)
#define	ROM_UARTNCharPut                             ((void (*)(unsigned long ulBase, unsigned char *pucData, unsigned long ulLenByte))func_UARTNCharPut)
#define	ROM_UARTBreakCtl                             ((void (*)(unsigned long ulBase, unsigned char bBreakState))func_UARTBreakCtl)
#define	ROM_UARTIntRegister                          ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_UARTIntRegister)
#define	ROM_UARTIntUnregister                        ((void (*)(unsigned long ulBase, unsigned long *g_pRAMVectors))func_UARTIntUnregister)
#define	ROM_UARTIntEnable                            ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_UARTIntEnable)
#define	ROM_UARTIntDisable                           ((void (*)(unsigned long ulBase, unsigned long ulIntFlags))func_UARTIntDisable)
#define	ROM_UARTIntStatus                            ((unsigned long (*)(unsigned long ulBase, unsigned char bMasked))func_UARTIntStatus)
#define	ROM_UARTIntClear                             ((void (*)(unsigned long ulBase))func_UARTIntClear)
#define	ROM_UARTRxErrorGet                           ((unsigned long (*)(unsigned long ulBase))func_UARTRxErrorGet)
#define	ROM_UARTRxErrorClear                         ((void (*)(unsigned long ulBase))func_UARTRxErrorClear)
#define	ROM_UARTStringPut                            ((void (*)(unsigned long ulBase, unsigned char *str))func_UARTStringPut)
#define	ROM_UARTConfigSetBaudrate                    ((void (*)(unsigned long ulBase, unsigned long ulPclk, unsigned long ulBaudrate, unsigned long ulConfig))func_UARTConfigSetBaudrate)
#define	ROM_UARTAutoBaudrate                         ((void (*)(unsigned long ulBase))func_UARTAutoBaudrate)
#define	ROM_UARTConfigSetMode                        ((void (*)(unsigned long ulBase, unsigned long ulConfig))func_UARTConfigSetMode)
#define	ROM_UARTWaitTxDone                           ((void (*)(unsigned long ulBase))func_UARTWaitTxDone)
#define	ROM_UARTPrintf                               ((void (*)(unsigned long ulBase, char* fmt, ...))func_UARTPrintf)
#define ROM_UTCTimerStop                             ((void (*)(void))func_UTCTimerStop)
#define ROM_UTCTimerRun                              ((void (*)(void))func_UTCTimerRun)
#define ROM_UTCCalStop                               ((void (*)(void))func_UTCCalStop)
#define ROM_UTCCalRun                                ((void (*)(void))func_UTCCalRun)
#define ROM_UTCDivStop                               ((void (*)(void))func_UTCDivStop)
#define ROM_UTCDivEn                                 ((void (*)(void))func_UTCDivEn)
#define ROM_UTCHourModeSet                           ((void (*)(unsigned long ulMode))func_UTCHourModeSet)
#define ROM_UTCHourModeGet                           ((unsigned long (*)(void))func_UTCHourModeGet)
#define ROM_UTCTimerSet                              ((void (*)(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec))func_UTCTimerSet)
#define ROM_UTCTimerChangeGet                        ((unsigned long (*)(void))func_UTCTimerChangeGet)
#define ROM_UTCTimerGet                              ((void (*)(unsigned char *ulAMPM, unsigned char *ulHour, unsigned char *ulMin, unsigned char *ulSec, unsigned char *ulMinSec))func_UTCTimerGet)
#define ROM_UTCTimerAlarmSet                         ((void (*)(unsigned long ulAMPM, unsigned long ulHour, unsigned long ulMin, unsigned long ulSec, unsigned long ulMinSec))func_UTCTimerAlarmSet)
#define ROM_UTCCalSet                                ((void (*)(unsigned long ulCentury, unsigned long ulYear, unsigned long ulMonth, unsigned long ulData, unsigned long ulDay))func_UTCCalSet)
#define ROM_UTCCalChangeGet                          ((unsigned long (*)(void))func_UTCCalChangeGet)
#define ROM_UTCCalGet                                ((void (*)(unsigned char *ulCentury, unsigned char *ulYear, unsigned char *ulMonth, unsigned char *ulData, unsigned char *ulDay))func_UTCCalGet)
#define ROM_UTCCalAlarmSet                           ((void (*)(unsigned long ulMonth, unsigned long ulData))func_UTCCalAlarmSet)
#define ROM_UTCAlarmEnable                           ((void (*)(unsigned long ulAlarmFlags))func_UTCAlarmEnable)
#define ROM_UTCAlarmDisable                          ((void (*)(unsigned long ulAlarmFlags))func_UTCAlarmDisable)
#define ROM_UTCIntEnable                             ((void (*)(unsigned long ulIntFlags))func_UTCIntEnable)
#define ROM_UTCIntDisable                            ((void (*)(unsigned long ulIntFlags))func_UTCIntDisable)
#define ROM_UTCIntMaskSet                            ((void (*)(unsigned long ulIntMask))func_UTCIntMaskSet)
#define ROM_UTCIntMaskGet                            ((unsigned long (*)(void))func_UTCIntMaskGet)
#define ROM_UTCIntStatusSet                          ((void (*)(unsigned long ulIntFlags))func_UTCIntStatusSet)
#define ROM_UTCIntStatusGet                          ((unsigned long (*)(void))func_UTCIntStatusGet)
#define ROM_UTCValidStatusGet                        ((unsigned long (*)(void))func_UTCValidStatusGet)
#define ROM_UTCKeepRun                               ((void (*)(unsigned long ulKeepUTC))func_UTCKeepRun)
#define ROM_UTCClkCntSet                             ((void (*)(unsigned long ulClkCnt))func_UTCClkCntSet)
#define ROM_UTCClkCntGet                             ((unsigned long (*)(void))func_UTCClkCntGet)
#define ROM_UTCIntRegister                           ((void (*)(unsigned long *g_pRAMVectors, void (*pfnHandler)(void)))func_UTCIntRegister)
#define ROM_UTCIntUnregister                         ((void (*)(unsigned long *g_pRAMVectors))func_UTCIntUnregister)
#define ROM_UTCTimerSetBy10ms                        ((void (*)(unsigned long ulMinSec))func_UTCTimerSetBy10ms)
#define ROM_UTCTimerGetBy10ms                        ((unsigned long (*)(void))func_UTCTimerGetBy10ms)
#define ROM_UTCTimerAlarmSetBy10ms                   ((void (*)(unsigned long ulMinSec))func_UTCTimerAlarmSetBy10ms)
#define ROM_WatchdogRunning                          ((unsigned char (*)(void))func_WatchdogRunning)
#define ROM_WatchdogEnable                           ((void (*)(void))func_WatchdogEnable)
#define ROM_WatchdogResetEnable                      ((void (*)(void))func_WatchdogResetEnable)
#define ROM_WatchdogResetDisable                     ((void (*)(void))func_WatchdogResetDisable)
#define ROM_WatchdogTimerRepeatEnable                ((void (*)(void))func_WatchdogTimerRepeatEnable)
#define ROM_WatchdogTimerRepeatDisable               ((void (*)(void))func_WatchdogTimerRepeatDisable)
#define ROM_WatchdogReloadSet                        ((void (*)(unsigned long ulLoadVal))func_WatchdogReloadSet)
#define ROM_WatchdogValueGet                         ((unsigned long (*)(void))func_WatchdogValueGet)
#define ROM_WatchdogIntRegister                      ((void (*)(unsigned long *g_pRAMVectors, void(*pfnHandler)(void)))func_WatchdogIntRegister)
#define ROM_WatchdogIntUnregister                    ((void (*)(unsigned long *g_pRAMVectors))func_WatchdogIntUnregister)
#define ROM_WatchdogIntEnable                        ((void (*)(void))func_WatchdogIntEnable)
#define ROM_WatchdogIntStatus                        ((unsigned long (*)(unsigned char bMasked))func_WatchdogIntStatus)
#define ROM_WatchdogIntClear                         ((void (*)(void))func_WatchdogIntClear)
#define ROM_WatchdogDisable                          ((void (*)(void))func_WatchdogDisable)

#endif

#endif // __ROM_H__
