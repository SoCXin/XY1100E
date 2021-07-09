#ifndef __REPLACE_H__
#define __REPLACE_H__

#include "rom.h"

#define USE_ROM_DRIVERS    1

#define USE_ROM_AES        (1 && USE_ROM_DRIVERS)
#define USE_ROM_CSP        (1 && USE_ROM_DRIVERS)
#define USE_ROM_DMA        (0 && USE_ROM_DRIVERS)
#define USE_ROM_GPIO       (1 && USE_ROM_DRIVERS)
#define USE_ROM_I2C        (1 && USE_ROM_DRIVERS)
#define USE_ROM_INT        (1 && USE_ROM_DRIVERS)
#define USE_ROM_PRCM       (1 && USE_ROM_DRIVERS)
#define USE_ROM_SEMA       (1 && USE_ROM_DRIVERS)
#define USE_ROM_SPI        (1 && USE_ROM_DRIVERS)
#define USE_ROM_SYSTICK    (1 && USE_ROM_DRIVERS)
#define USE_ROM_TIMER      (1 && USE_ROM_DRIVERS)
#define USE_ROM_UART       (1 && USE_ROM_DRIVERS)
#define USE_ROM_UTC        (1 && USE_ROM_DRIVERS)
#define USE_ROM_WDT        (1 && USE_ROM_DRIVERS)

// AES
#if USE_ROM_AES
  #define AESKeyLenSet                   ROM_AESKeyLenSet
  #define AESKeySet                      ROM_AESKeySet
  #define AESIVSet                       ROM_AESIVSet
  #define AESAADLen0Set                  ROM_AESAADLen0Set
  #define AESAADLen1Set                  ROM_AESAADLen1Set
  #define AESPayloadLenSet               ROM_AESPayloadLenSet
  #define AESModeSet                     ROM_AESModeSet
  #define AESEncDecSet                   ROM_AESEncDecSet
  #define AESDMAEn                       ROM_AESDMAEn
  #define AESDMADis                      ROM_AESDMADis
  #define AESBlockDataInput              ROM_AESBlockDataInput
  #define AESBlockDataOutput             ROM_AESBlockDataOutput
  #define AESBlockStart                  ROM_AESBlockStart
  #define AESKeyLoadWaitDone             ROM_AESKeyLoadWaitDone
  #define AESBlockTransWaitDone          ROM_AESBlockTransWaitDone
  #define AESClockDiv2En                 ROM_AESClockDiv2En
  #define AESClockDiv2Dis                ROM_AESClockDiv2Dis
  #define AESCCMAuthLenSet               ROM_AESCCMAuthLenSet
  #define AESCCMLengthLenSet             ROM_AESCCMLengthLenSet
  #define AESTagGet                      ROM_AESTagGet
  #define AESBlock                       ROM_AESBlock
  #define AESECB                         ROM_AESECB
  #define AESCCM                         ROM_AESCCM
  #define AES_CMAC_XOR_128               ROM_AES_CMAC_XOR_128
  #define AES_CMAC_Leftshift_Onebit      ROM_AES_CMAC_Leftshift_Onebit
  #define AES_CMAC_Generate_Subkey       ROM_AES_CMAC_Generate_Subkey
#endif

#if USE_ROM_CSP
  #define CSPSPIMode                     ROM_CSPSPIMode
  #define CSPFIFOLevelSet                ROM_CSPFIFOLevelSet
  #define CSPCharGet                     ROM_CSPCharGet
  #define CSPCharGetNonBlocking          ROM_CSPCharGetNonBlocking
  #define CSPCharPutNonBlocking          ROM_CSPCharPutNonBlocking
  #define CSPOperationModeSet            ROM_CSPOperationModeSet
  #define CSPClockModeSet                ROM_CSPClockModeSet
  #define CSPIrdaEnable                  ROM_CSPIrdaEnable
  #define CSPEndianModeSet               ROM_CSPEndianModeSet
  #define CSPEnable                      ROM_CSPEnable
  #define CSPDisable                     ROM_CSPDisable
  #define CSPDrivenEdgeSet               ROM_CSPDrivenEdgeSet
  #define CSPSyncValidLevelSet           ROM_CSPSyncValidLevelSet
  #define CSPClockIdleModeSet            ROM_CSPClockIdleModeSet
  #define CSPClockIdleLevelSet           ROM_CSPClockIdleLevelSet
  #define CSPPinModeSet                  ROM_CSPPinModeSet
  #define CSPIrdaPulseWidthSet           ROM_CSPIrdaPulseWidthSet
  #define CSPIrdaIdleLevelSet            ROM_CSPIrdaIdleLevelSet
  #define CSPTxFrameSet                  ROM_CSPTxFrameSet
  #define CSPRxFrameSet                  ROM_CSPRxFrameSet
  #define CSPRxEnable                    ROM_CSPRxEnable
  #define CSPRxDisable                   ROM_CSPRxDisable
  #define CSPTxEnable                    ROM_CSPTxEnable
  #define CSPTxDisable                   ROM_CSPTxDisable
  #define CSPIntRegister                 ROM_CSPIntRegister
  #define CSPIntUnregister               ROM_CSPIntUnregister
  #define CSPIntEnable                   ROM_CSPIntEnable
  #define CSPIntDisable                  ROM_CSPIntDisable
  #define CSPIntStatus                   ROM_CSPIntStatus
  #define CSPCharPut                     ROM_CSPCharPut
#endif

// DMA
#if USE_ROM_DMA
  #define DMAErrorStatusGet              ROM_DMAErrorStatusGet
  #define DMAErrorStatusClear            ROM_DMAErrorStatusClear
  #define DMAChannelRequest              ROM_DMAChannelRequest
  #define DMAChannelControlSet           ROM_DMAChannelControlSet
  #define DMAChannelTransferSet          ROM_DMAChannelTransferSet
  #define DMAChannelNextPointerSet       ROM_DMAChannelNextPointerSet
  #define DMAIntRegister                 ROM_DMAIntRegister
  #define DMAIntUnregister               ROM_DMAIntUnregister
  #define DMAIntStatus                   ROM_DMAIntStatus
  #define DMAIntClear                    ROM_DMAIntClear
  #define DMAChannelWaitIdle             ROM_DMAChannelWaitIdle
  #define DMAChannelTransfer             ROM_DMAChannelTransfer
  #define DMAWaitAESDone                 ROM_DMAWaitAESDone
  #define DMAWaitChannelDone             ROM_DMAChannelMemcpy
#endif

// GPIO
#if USE_ROM_GPIO
  #define GPIOPeripheralPad              ROM_GPIOPeripheralPad
  #define GPIODirModeSet                 ROM_GPIODirModeSet
  #define GPIOOutputODEn                 ROM_GPIOOutputODEn
  #define GPIOOutputODDis                ROM_GPIOOutputODDis
  #define GPIOAnalogEn                   ROM_GPIOAnalogEn
  #define GPIOAnalogDis                  ROM_GPIOAnalogDis
  #define GPIOPullupEn                   ROM_GPIOPullupEn
  #define GPIOPullupDis                  ROM_GPIOPullupDis
  #define GPIOPulldownEn                 ROM_GPIOPulldownEn
  #define GPIOPulldownDis                ROM_GPIOPulldownDis
  #define GPIOPinSet                     ROM_GPIOPinSet
  #define GPIOPinClear                   ROM_GPIOPinClear
  #define GPIOPinRead                    ROM_GPIOPinRead
  #define GPIOIntEnable                  ROM_GPIOIntEnable
  #define GPIOIntDisable                 ROM_GPIOIntDisable
  #define GPIOIntTypeSet                 ROM_GPIOIntTypeSet
  #define GPIOIntEdgeSet                 ROM_GPIOIntEdgeSet
  #define GPIOIntSingleEdgeSet           ROM_GPIOIntSingleEdgeSet
  #define GPIOIntStatusGet               ROM_GPIOIntStatusGet
  #define GPIOIntMaskGet                 ROM_GPIOIntMaskGet
  #define GPIOConflictStatusGet          ROM_GPIOConflictStatusGet
  #define GPIOConflictStatusClear        ROM_GPIOConflictStatusClear
  #define GPIOAllocationStatusGet        ROM_GPIOAllocationStatusGet
  #define GPIODrvStrengthSet             ROM_GPIODrvStrengthSet
  #define GPIOIntRegister                ROM_GPIOIntRegister
  #define GPIOIntUnregister              ROM_GPIOIntUnregister
  #define GPIOModeSet                    ROM_GPIOModeSet
  #define GPIOPadConfigSel               ROM_GPIOPadConfigSel
  #define GPIOPadModeSet                 ROM_GPIOPadModeSet
  #define GPIODirectionSet               ROM_GPIODirectionSet
#endif

// I2C
#if USE_ROM_I2C
  #define I2CMasterInitExpClk            ROM_I2CMasterInitExpClk
  #define I2CSlaveInit                   ROM_I2CSlaveInit
  #define I2CMasterEnable                ROM_I2CMasterEnable
  #define I2CSlaveEnable                 ROM_I2CSlaveEnable
  #define I2CIntRegister                 ROM_I2CIntRegister
  #define I2CIntUnregister               ROM_I2CIntUnregister
  #define I2CIntEnable                   ROM_I2CIntEnable
  #define I2CIntDisable                  ROM_I2CIntDisable
  #define I2CIntStatus                   ROM_I2CIntStatus
  #define I2CIntClear                    ROM_I2CIntClear
  #define I2CAddrSet                     ROM_I2CAddrSet
  #define I2CBusBusy                     ROM_I2CBusBusy
  #define I2CControlSet                  ROM_I2CControlSet
  #define I2CControlGet                  ROM_I2CControlGet
  #define I2CDataPut                     ROM_I2CDataPut
  #define I2CDataGet                     ROM_I2CDataGet
  #define I2CStatus                      ROM_I2CStatus
  #define I2CACKModeSet                  ROM_I2CACKModeSet
  #define I2CFifoClear                   ROM_I2CFifoClear
#endif

// Interrupt
#if USE_ROM_INT
  #define IntRegister                    ROM_IntRegister
  #define IntUnregister                  ROM_IntUnregister
  #define IntPriorityGroupingSet         ROM_IntPriorityGroupingSet
  #define IntPriorityGroupingGet         ROM_IntPriorityGroupingGet
  #define IntPrioritySet                 ROM_IntPrioritySet
  #define IntPriorityGet                 ROM_IntPriorityGet
  #define IntEnable                      ROM_IntEnable
  #define IntDisable                     ROM_IntDisable
  #define IntPendSet                     ROM_IntPendSet
  #define IntPendClear                   ROM_IntPendClear
#endif

#if USE_ROM_PRCM
  #define AON_DSP_Load_Flag_Get          ROM_AON_DSP_Load_Flag_Get
  #define AON_ARM_Load_Flag_Get          ROM_AON_ARM_Load_Flag_Get
  #define AON_DSP_Load_Flag_Set          ROM_AON_DSP_Load_Flag_Set
  #define AON_ARM_Load_Flag_Set          ROM_AON_ARM_Load_Flag_Set
  #define AON_Flash_Delay_Set            ROM_AON_Flash_Delay_Set
  #define AON_Addr_Overlap_Unused_Mem    ROM_AON_Addr_Overlap_Unused_Mem
  #define PRCM_Clock_Enable              ROM_PRCM_Clock_Enable
  #define PRCM_Clock_Disable             ROM_PRCM_Clock_Disable
  #define PRCM_CHIP_VER_GET              ROM_PRCM_CHIP_VER_GET
#endif

#if USE_ROM_SEMA
  #define SEMAIntRegister                ROM_SEMAIntRegister
  #define SEMAIntUnregister              ROM_SEMAIntUnregister
#endif

// SPI
#if USE_ROM_SPI
  #define SPIConfigSetExpClk             ROM_SPIConfigSetExpClk
  #define SPIEnable                      ROM_SPIEnable
  #define SPIDisable                     ROM_SPIDisable
  #define SPIIntRegister                 ROM_SPIIntRegister
  #define SPIIntUnregister               ROM_SPIIntUnregister
  #define SPIIntEnable                   ROM_SPIIntEnable
  #define SPIIntStatus                   ROM_SPIIntStatus
  #define SPIDataPutNonBlocking          ROM_SPIDataPutNonBlocking
  #define SPIDataGetNonBlocking          ROM_SPIDataGetNonBlocking
  #define SPISetTxFifoThreshold          ROM_SPISetTxFifoThreshold
  #define SPISetRxFifoThreshold          ROM_SPISetRxFifoThreshold
  #define SPISetDelay                    ROM_SPISetDelay
  #define SPIEnManualCS                  ROM_SPIEnManualCS
  #define SPIDisManualCS                 ROM_SPIDisManualCS
  #define SPIEnManualTransmit            ROM_SPIEnManualTransmit
  #define SPIDisManualTransmit           ROM_SPIDisManualTransmit
  #define SPIStartManualTransmit         ROM_SPIStartManualTransmit
  #define SPIEnModeFail                  ROM_SPIEnModeFail
  #define SPIDisModeFail                 ROM_SPIDisModeFail
  #define SPITxFifoFlush                 ROM_SPITxFifoFlush
  #define SPITxFifoNormal                ROM_SPITxFifoNormal
  #define SPIRxFifoFlush                 ROM_SPIRxFifoFlush
  #define SPIRxFifoNormal                ROM_SPIRxFifoNormal
#endif

// SysTick
#if USE_ROM_SYSTICK
  #define SysTickEnable                  ROM_SysTickEnable
  #define SysTickDisable                 ROM_SysTickDisable
  #define SysTickIntRegister             ROM_SysTickIntRegister
  #define SysTickIntUnregister           ROM_SysTickIntUnregister
  #define SysTickIntEnable               ROM_SysTickIntEnable
  #define SysTickIntDisable              ROM_SysTickIntDisable
  #define SysTickPeriodSet               ROM_SysTickPeriodSet
  #define SysTickPeriodGet               ROM_SysTickPeriodGet
  #define SysTickValueGet                ROM_SysTickValueGet
#endif

// Timer
#if USE_ROM_TIMER
  #define TimerEnable                    ROM_TimerEnable
  #define TimerDisable                   ROM_TimerDisable
  #define TimerConfigure                 ROM_TimerConfigure
  #define TimerControlLevel              ROM_TimerControlLevel
  #define TimerPrescaleSet               ROM_TimerPrescaleSet
  #define TimerPrescaleGet               ROM_TimerPrescaleGet
  #define TimerLoadSet                   ROM_TimerLoadSet
  #define TimerValueGet                  ROM_TimerValueGet
  #define TimerMatchSet                  ROM_TimerMatchSet
  #define TimerMatchGet                  ROM_TimerMatchGet
  #define TimerPWMSet                    ROM_TimerPWMSet
  #define TimerPWMGet                    ROM_TimerPWMGet
  #define TimerIntRegister               ROM_TimerIntRegister
  #define TimerIntUnregister             ROM_TimerIntUnregister
  #define TimerIntEnable                 ROM_TimerIntEnable
#endif

// UART
#if USE_ROM_UART
  #define UARTParityModeSet              ROM_UARTParityModeSet
  #define UARTParityModeGet              ROM_UARTParityModeGet
  #define UARTFIFOLevelSet               ROM_UARTFIFOLevelSet
  #define UARTFIFOLevelGet               ROM_UARTFIFOLevelGet
  #define UARTConfigSetExpClk            ROM_UARTConfigSetExpClk
  #define UARTConfigGetExpClk            ROM_UARTConfigGetExpClk
  // #define UARTEnable                     ROM_UARTEnable            //存在bug,使用uart.c中修改后的函数
  #define UARTDisable                    ROM_UARTDisable
  #define UARTCharsAvail                 ROM_UARTCharsAvail
  #define UARTSpaceAvail                 ROM_UARTSpaceAvail
  #define UARTCharGetNonBlocking         ROM_UARTCharGetNonBlocking
  #define UARTCharGet                    ROM_UARTCharGet
  #define UARTNCharGet                   ROM_UARTNCharGet
  #define UARTCharPutNonBlocking         ROM_UARTCharPutNonBlocking
  #define UARTCharPut                    ROM_UARTCharPut
  #define UARTNCharPut                   ROM_UARTNCharPut
  #define UARTBreakCtl                   ROM_UARTBreakCtl
  #define UARTIntRegister                ROM_UARTIntRegister
  #define UARTIntUnregister              ROM_UARTIntUnregister
  #define UARTIntEnable                  ROM_UARTIntEnable
  #define UARTIntDisable                 ROM_UARTIntDisable
  #define UARTIntStatus                  ROM_UARTIntStatus
  #define UARTIntClear                   ROM_UARTIntClear
  #define UARTRxErrorGet                 ROM_UARTRxErrorGet
  #define UARTRxErrorClear               ROM_UARTRxErrorClear
  #define UARTStringPut                  ROM_UARTStringPut
//  #define UARTConfigSetBaudrate          ROM_UARTConfigSetBaudrate
  #define UARTAutoBaudrate               ROM_UARTAutoBaudrate
  #define UARTConfigSetMode              ROM_UARTConfigSetMode
  #define UARTWaitTxDone                 ROM_UARTWaitTxDone
  #define UARTPrintf                     ROM_UARTPrintf
#endif

// UTC
#if USE_ROM_UTC
  #define UTCTimerStop                   ROM_UTCTimerStop
  #define UTCTimerRun                    ROM_UTCTimerRun
  #define UTCCalStop                     ROM_UTCCalStop
  #define UTCCalRun                      ROM_UTCCalRun
  #define UTCDivStop                     ROM_UTCDivStop
  #define UTCDivEn                       ROM_UTCDivEn
  #define UTCHourModeSet                 ROM_UTCHourModeSet
  #define UTCHourModeGet                 ROM_UTCHourModeGet
  #define UTCTimerSet                    ROM_UTCTimerSet
  #define UTCTimerChangeGet              ROM_UTCTimerChangeGet
  #define UTCTimerGetBy10ms              ROM_UTCTimerGetBy10ms
  #define UTCTimerAlarmSetBy10ms         ROM_UTCTimerAlarmSetBy10ms
  #define UTCCalChangeGet                ROM_UTCCalChangeGet
  #define UTCCalAlarmSet                 ROM_UTCCalAlarmSet
  #define UTCAlarmEnable                 ROM_UTCAlarmEnable
  #define UTCAlarmDisable                ROM_UTCAlarmDisable
  #define UTCIntEnable                   ROM_UTCIntEnable
  #define UTCIntDisable                  ROM_UTCIntDisable
  #define UTCIntMaskSet                  ROM_UTCIntMaskSet
  #define UTCIntMaskGet                  ROM_UTCIntMaskGet
  #define UTCIntStatusSet                ROM_UTCIntStatusSet
  #define UTCIntStatusGet                ROM_UTCIntStatusGet
  #define UTCValidStatusGet              ROM_UTCValidStatusGet
  #define UTCKeepRun                     ROM_UTCKeepRun
  #define UTCClkCntSet                   ROM_UTCClkCntSet
  #define UTCIntRegister                 ROM_UTCIntRegister
  #define UTCIntUnregister               ROM_UTCIntUnregister
#endif

// WatchDog
#if USE_ROM_WDT
  #define WatchdogIntRegister            ROM_WatchdogIntRegister
  #define WatchdogIntUnregister          ROM_WatchdogIntUnregister
  #define WatchdogIntEnable              ROM_WatchdogIntEnable
#endif


#endif // __REPLACE_H__
